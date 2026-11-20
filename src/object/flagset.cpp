#include "common.h"
#include "object/animnode.h"
#include "object/fencenode.h"
#include "ui/uithing.h"
#include "bx/math3d.h"
#include "world/world.h"

//100%
INCLUDE_ASM("object/flagset", cFlagSet_CreateMesh);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan;
extern char str_Flag_Verts[];
unsigned int BXrand();
extern "C" void cFlagSet_updateGrid(void* self, t3Vector* verts);

// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");

struct sFsV3_CM {
    float x, y, z;
    sFsV3_CM() {}
    sFsV3_CM(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

struct sFsV2_CM {
    float x, y;
    sFsV2_CM() {}
    sFsV2_CM(float ax, float ay) : x(ax), y(ay) {}
};

struct sFsCol_CM {
    float a, r, g, b;
    sFsCol_CM() {}
};

inline sFsV3_CM operator+(const sFsV3_CM& a, const sFsV3_CM& b) { return sFsV3_CM(a.x + b.x, a.y + b.y, a.z + b.z); }
inline sFsV3_CM operator*(const sFsV3_CM& a, float s) { return sFsV3_CM(a.x * s, a.y * s, a.z * s); }
inline sFsV2_CM operator+(const sFsV2_CM& a, const sFsV2_CM& b) { return sFsV2_CM(a.x + b.x, a.y + b.y); }
inline sFsV2_CM operator*(const sFsV2_CM& a, float s) { return sFsV2_CM(a.x * s, a.y * s); }

// Uniform float in [0, 1) built from the random mantissa bits.
static inline float randf_CM()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

struct sFsVE_CM {
    short delta;
    short index;
    void* (*fn)(void*, int, int, unsigned int);
};

struct sFsQuadVE_CM {
    short delta;
    short index;
    void (*fn)(void*, void*, int, sFsV3_CM*, sFsV2_CM*, sFsCol_CM*);
};

struct sFsMeshVE_CM {
    short delta;
    short index;
    void (*fn)(void*, sFsV3_CM*, sFsCol_CM*, sFsV2_CM*);
};

struct sFsMesh_CM {
    int pad0;
    sFsMeshVE_CM* vt;   // 0x4
};

struct sFlagElem_CM {
    char pad0[0x1C];
    float windX;            // 0x1C
    float windY;            // 0x20
    char pad24[0x3C];
    sFsMesh_CM* mesh;       // 0x60
    int cols;               // 0x64
    int rows;               // 0x68
    float colScale;         // 0x6C
    float rowScale;         // 0x70
    sFsV3_CM* verts;        // 0x74
    float phase[4];         // 0x78
    float u;                // 0x88
    float v;                // 0x8C
};

/**
 * Builds a flag set's cloth grid mesh from a flag instance's quad, then computes its first frame.
 *
 * The grid is 8 by 2 vertices when the set has almost no wind, otherwise 8 by 5; positions, UVs and colours
 * are
 * interpolated from the quad's corners (renderer slot 97). The four flag phases start random.
 *
 * @param flagSet  the flag set
 * @param item  the flag's instance record (On Tour tInstanceRecord* pInst)
 */
extern "C" void cFlagSet_CreateMesh(void* flagSet, void* item)
{
    sFlagElem_CM* self = (sFlagElem_CM*)flagSet;
    int i;
    for (i = 0; i < 4; i++) {
        self->phase[i] = randf_CM();
    }
    self->u = 0.0f;
    self->v = 0.0f;
    char* mgr = cGraphicsMan_TheGraphicsMan;
    sFsV2_CM quv[4];
    sFsCol_CM qcol[4];
    sFsV3_CM pos[4];
    sFsQuadVE_CM* vt = *(sFsQuadVE_CM**)(mgr + 0x10D8);
    vt[97].fn(mgr + vt[97].delta, item, 0, pos, quv, qcol);
    if (self->windX < 0.01f && self->windY < 0.01f) {
        self->cols = 8;
        self->rows = 2;
    } else {
        self->cols = 8;
        self->rows = 5;
    }
    self->colScale = (float)(self->cols - 1);
    self->rowScale = (float)(self->rows - 1);
    sFsVE_CM* vtEntry = &(*(sFsVE_CM**)(mgr + 0x10D8))[76];
    self->mesh = (sFsMesh_CM*)vtEntry->fn(mgr + vtEntry->delta, self->cols, self->rows, 0x60000000u);
    if (self->mesh == 0) {
        return;
    }
    int n = self->cols * self->rows;
    sFsV3_CM** pv = &self->verts;
    *pv = new (str_Flag_Verts, 0x20000000, 0) sFsV3_CM[n];
    sFsV2_CM uv[50];
    sFsCol_CM col[50];
    int idx = 0;
    for (int row = 0; row < self->rows; row++) {
        for (int column = 0; column < self->cols; column++) {
            float fc = (float)column / self->colScale;
            float fr = (float)row / self->rowScale;
            self->verts[idx] = (pos[0] * (1.0f - fc) + pos[1] * fc) * (1.0f - fr) + (pos[2] * (1.0f - fc) + pos[3] * fc) * fr;
            uv[idx] = (quv[0] * (1.0f - fc) + quv[1] * fc) * (1.0f - fr) + (quv[2] * (1.0f - fc) + quv[3] * fc) * fr;
            col[idx].a = 1.0f;
            col[idx].r = (qcol[0].r * (1.0f - fc) + qcol[1].r * fc) * (1.0f - fr) + (qcol[2].r * (1.0f - fc) + qcol[3].r * fc) * fr;
            col[idx].g = (qcol[0].g * (1.0f - fc) + qcol[1].g * fc) * (1.0f - fr) + (qcol[2].g * (1.0f - fc) + qcol[3].g * fc) * fr;
            col[idx].b = (qcol[0].b * (1.0f - fc) + qcol[1].b * fc) * (1.0f - fr) + (qcol[2].b * (1.0f - fc) + qcol[3].b * fc) * fr;
            idx++;
        }
    }
    sFsV3_CM verts[50];
    cFlagSet_updateGrid(self, (t3Vector*)verts);
    sFsMesh_CM* flagMesh = self->mesh;
    flagMesh->vt[6].fn((char*)flagMesh + flagMesh->vt[6].delta, verts, col, uv);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagSet_PurgeMesh);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan;
void cMemMan_free(void*);

struct sFlagSetVEntryB7B8 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

/**
 * Frees a flag set's grid mesh and vertex buffer.
 *
 * Releases the grid mesh (+0x60) through graphics-manager slot 77 and frees the vertex array (+0x74) with cMemMan_free,
 * nulling each pointer.
 */
extern "C" void cFlagSet_PurgeMesh(void* self)
{
    char* flagSet = (char*)self;
    void* mesh = *(void**)(flagSet + 0x60);
    if (mesh != 0) {
        char* mgr = cGraphicsMan_TheGraphicsMan;
        sFlagSetVEntryB7B8* vt = *(sFlagSetVEntryB7B8**)(mgr + 0x10D8);
        vt[77].fn(mgr + vt[77].delta, mesh);
        *(void**)(flagSet + 0x60) = 0;
    }
    void* verts = *(void**)(flagSet + 0x74);
    if (verts != 0) {
        cMemMan_free(verts);
        *(void**)(flagSet + 0x74) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagSet_update);
#ifdef SKIP_ASM
extern int D_004A4240;
extern "C" void* cObjectInterface_getFlagManager();
extern "C" int cObjectInterface_getAITick();
extern "C" void cFlagSet_updateGrid(void* self, t3Vector* verts);

struct sFlagVert_B818 {
    float x, y, z;
    sFlagVert_B818() {}
};

struct sFlagVEntry_B818 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sFlagMesh_B818 {
    int pad0;
    sFlagVEntry_B818* vt;   // 0x4
};

struct sFlagElem_B818 {
    char pad0[0x4];
    float speed[4];         // 0x4
    char pad14[0x38];
    float blend;            // 0x4C
    float scrollU;          // 0x50
    float scrollV;          // 0x54
    char pad58[0x4];
    int active;             // 0x5C
    sFlagMesh_B818* mesh;   // 0x60
    char pad64[0x14];
    float phase[4];         // 0x78
    float u;                // 0x88
    float v;                // 0x8C
    int parity;             // 0x90
};

/**
 * Updates a flag set: advances its wave clocks, regenerates the cloth every other frame and scrolls its texture.
 *
 * Returns when flag updates are off (D_004A4240), the set is empty (+0x5C) or has no grid mesh (+0x60). The wind blend
 * is mMinWindFactor (+0x4C) + the flag manager's wind factor (cObjectInterface_getFlagManager() +0x10) * (1 - mMinWindFactor); each of
 * the 4 clocks mFlagTime (+0x78) advances by mSpeed[i] (+0x4) times that and wraps at 1. On frames whose parity
 * (cObjectInterface_getAITick() % 2) equals mFrameToUpdateGrid (+0x90) it builds up to 50 vertices with cFlagSet_updateGrid and hands them
 * to the mesh's vtable entry 3. When either scroll rate (+0x50, +0x54) exceeds 0.001 it adds them to mScrollU/V (+0x88,
 * +0x8C), wrapping at 1.
 *
 * @param elem  the flag set
 */
extern "C" void cFlagSet_update(void* elem)
{
    sFlagElem_B818* self = (sFlagElem_B818*)elem;
    if (D_004A4240 == 0) {
        return;
    }
    if (self->active == 0) {
        return;
    }
    if (self->mesh == 0) {
        return;
    }
    char* wind = (char*)cObjectInterface_getFlagManager();
    float k = self->blend;
    float windBlend = k + *(float*)(wind + 0x10) * (1.0f - k);
    int i;
    for (i = 0; i < 4; i++) {
        self->phase[i] += self->speed[i] * windBlend;
        if (1.0f <= self->phase[i]) {
            self->phase[i] -= 1.0f;
        }
    }
    if (cObjectInterface_getAITick() % 2 == self->parity) {
        sFlagVert_B818 verts[50];
        cFlagSet_updateGrid(self, (t3Vector*)verts);
        sFlagMesh_B818* gridMesh = self->mesh;
        sFlagVEntry_B818* setVertsSlot = &gridMesh->vt[3];
        setVertsSlot->fn((char*)gridMesh + setVertsSlot->delta, verts);
    }
    if (0.001f < self->scrollU || 0.001f < self->scrollV) {
        self->u += self->scrollU;
        if (1.0f < self->u) {
            self->u -= 1.0f;
        }
        self->v += self->scrollV;
        if (1.0f < self->v) {
            self->v -= 1.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagSet_render);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan;
extern int D_004A4244;
// PORT: 128-bit integer mode for VU0 asm memory operands
typedef int cQuad128_34B9B0 __attribute__((mode(TI)));
extern char t4mat_Identity[];

struct sRS_34B9B0 {
    unsigned int f0_0 : 12;
    unsigned int f0_12 : 8;
    unsigned int f0_20 : 12;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 25;
    int f8;
    int fC;
    int f10;
};
extern sRS_34B9B0 sDrawState_s_defaultDrawState;

struct sVec4_34B9B0 {
    float x, y, z, w;
    sVec4_34B9B0() {}
    sVec4_34B9B0(const float& a, const float& b, const float& c, const float& d) : x(a), y(b), z(c), w(d) {}
} __attribute__((aligned(16)));

struct sRow_34B9B0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_34B9B0 {
    sRow_34B9B0 r[4];
};

struct sModel_34B9B0 {
    char pad_0x0[0x7D];
    unsigned char page;     // 0x7D
    short slot;             // 0x7E
};

static inline int IsLoaded_34B9B0(sModel_34B9B0* m)
{
    char* wc = (char*)cWorld_m_pTheWorld->m_pWorldCache;
    if (m->page == 0xFF)
        return 1;
    int ok = *(int*)(wc + (m->page << 3) + 0x24) == 6;
    return ok ? *(int*)(wc + m->slot * 0x18 + 0x3F0) == 3 : 0;
}

struct sPair_34B9B0 { sRow_34B9B0 r[4]; };
struct sVE43_34B9B0 { short delta; short index; sPair_34B9B0 (*fn)(void*); };
struct sPair_34B9B0;
struct sVE93_34B9B0 { short delta; short index; int (*fn)(void*, sVec4_34B9B0*, sVec4_34B9B0*, sPair_34B9B0*); };
struct sVE96_34B9B0 { short delta; short index; void (*fn)(void*, void*, int, int, int); };
struct sVE7_34B9B0 { short delta; short index; void (*fn)(void*, void*, void*, sMat_34B9B0*); };

static inline void SetF0_12_34B9B0(sRS_34B9B0* rs, int v) { rs->f0_12 = v; }
static inline void SetF4_2_34B9B0(sRS_34B9B0* rs, int v) { rs->f4_2 = v; }

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void MatCopy_34B9B0(sMat_34B9B0* d, const sMat_34B9B0* s)
{
    __asm__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

struct sFlagSetEntry;
struct sFSE_34B9B0 {
    char pad[0x94];
    sModel_34B9B0* models[1];
};

#define TOP_34B9B0(c) (*(sRS_34B9B0**)((c) + 0xE84))

/**
 * Draws every instance of a flag set with its cloth mesh and scrolled texture.
 *
 * Returns when flag rendering is off (D_004A4244). Pushes a copy of the default draw state; with mOverSaturate (+0x3)
 * set it forces the alpha field to 0xFF and blend mode 11, otherwise blend mode 5. For each instance in mInstances
 * (+0x94, count +0x5C) whose world-cache page is loaded and whose bounding box (+0x60..+0x74) survives the cull
 * (graphics-manager slot 93 against slot 43's view), it builds an identity texture matrix translated by mScrollU/V
 * (+0x88, +0x8C) and draws the grid mesh (+0x60, its vtable entry 7, with the instance matrix and its +0x94) or,
 * without a mesh, the instance itself (graphics-manager slot 96). Pops the draw state.
 *
 * @param flagSet  the flag set
 */
extern "C" void cFlagSet_render(sFlagSetEntry* flagSet)
{
    char* self = (char*)flagSet;
    if (D_004A4244 == 0) {
        return;
    }
    char* ctx = cGraphicsMan_TheGraphicsMan;
    TOP_34B9B0(ctx)[1] = TOP_34B9B0(ctx)[0];
    *++TOP_34B9B0(ctx) = sDrawState_s_defaultDrawState;
    if (*(signed char*)(self + 3) != 0) {
        SetF0_12_34B9B0(TOP_34B9B0(cGraphicsMan_TheGraphicsMan), 0xFF);
        SetF4_2_34B9B0(TOP_34B9B0(cGraphicsMan_TheGraphicsMan), 11);
    } else {
        SetF4_2_34B9B0(TOP_34B9B0(ctx), 5);
    }
    float one = 1.0f;
    for (int i = 0; i < *(int*)(self + 0x5C); i++) {
        sModel_34B9B0* instRecord = ((sFSE_34B9B0*)self)->models[i];
        if (!IsLoaded_34B9B0(instRecord)) {
            continue;
        }
        {
            sVE93_34B9B0* vt = *(sVE93_34B9B0**)(ctx + 0x10D8);
            char* thisp = ctx + vt[93].delta;
            sVec4_34B9B0 boxMin(*(float*)((char*)instRecord + 0x60), *(float*)((char*)instRecord + 0x64), *(float*)((char*)instRecord + 0x68), one);
            sVec4_34B9B0 boxMax(*(float*)((char*)instRecord + 0x6C), *(float*)((char*)instRecord + 0x70), *(float*)((char*)instRecord + 0x74), one);
            sVec4_34B9B0* pb = &boxMax;
            sVE43_34B9B0* vt43 = *(sVE43_34B9B0**)(ctx + 0x10D8);
            sPair_34B9B0 viewMat = vt43[43].fn(ctx + vt43[43].delta);
            if (vt[93].fn(thisp, &boxMin, pb, &viewMat) == 1) {
                continue;
            }
        }
        {
            char* pos = (char*)instRecord + 0x10;
            sMat_34B9B0 mat;
            MatCopy_34B9B0(&mat, (sMat_34B9B0*)t4mat_Identity);
            mat.r[3].x = *(float*)(self + 0x88);
            mat.r[3].y = *(float*)(self + 0x8C);
            char* gridMesh = *(char**)(self + 0x60);
            if (gridMesh != 0) {
                sVE7_34B9B0* hv = *(sVE7_34B9B0**)(gridMesh + 4);
                hv[7].fn(gridMesh + hv[7].delta, pos, *(void**)((char*)instRecord + 0x94), &mat);
            } else {
                sVE96_34B9B0* drawSlot = *(sVE96_34B9B0**)(ctx + 0x10D8);
                drawSlot[96].fn(ctx + drawSlot[96].delta, instRecord, 0, 0, 0);
            }
        }
    }
    TOP_34B9B0(ctx)--;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagSet_updateGrid);
#ifdef SKIP_ASM
extern float D_00504FB8[];

// Element i of v (x, y, z).
inline float& elem_BCA0(sFsV3_CM& v, int i) { return (&v.x)[i]; }
inline const float& elem_BCA0(const sFsV3_CM& v, int i) { return (&v.x)[i]; }

inline sFsV3_CM& operator+=(sFsV3_CM& a, const sFsV3_CM& b)
{
    elem_BCA0(a, 0) += elem_BCA0(b, 0);
    elem_BCA0(a, 1) += elem_BCA0(b, 1);
    elem_BCA0(a, 2) += elem_BCA0(b, 2);
    return a;
}

struct sFlagElem_BCA0 {
    signed char b0;         // 0x0
    signed char mode;       // 0x1
    signed char b2;         // 0x2
    char pad3[0x11];
    float amp[4];           // 0x14 (0x1C/0x20: windX/windY in cFlagSet_CreateMesh)
    float freqX;            // 0x24
    float freqY;            // 0x28
    float freqZ0;           // 0x2C
    float freqZ1;           // 0x30
    sFsV3_CM a;             // 0x34
    sFsV3_CM b;             // 0x40
    float blend;            // 0x4C
    char pad50[0x14];
    int cols;               // 0x64
    int rows;               // 0x68
    float colScale;         // 0x6C
    float rowScale;         // 0x70
    sFsV3_CM* verts;        // 0x74
    float phase[4];         // 0x78
};

/**
 * Computes the animated cloth vertices of a flag set into out.
 *
 * The wind blend is mMinWindFactor + wind factor * (1 - mMinWindFactor); wave phase i is mFlagTime[i] * -2pi and wave
 * amplitude i is mMagnitude[i] times the blend. Per column it accumulates two sine waves (frequencies +0x24, +0x28)
 * into a horizontal offset plus the low/high wind movement (+0x34, +0x40) weighted by the blend; the ratio along the
 * flag depends on which sides are held (mBaseSideHeld +0x0, mOppositeSideHeld +0x1: both, neither, or one, which
 * mirrors the columns). Per vertex it adds the column offset to the rest vertex (mVerts +0x74; the top row stays put
 * when mPerpSideHeld +0x2 is set) and two ripple waves (+0x2C, +0x30) when their amplitude exceeds 0.01. Sines and
 * cosines come from the 512-entry table D_00504FB8. Grid size: mFlagVertsHorizontal/Vertical (+0x64, +0x68).
 *
 * Likely bug: vertexIndex only advances in the moving branch, so when the top row is held its rest vertices are copied
 * but the index does not move past them, and the next row overwrites them.
 *
 * @param vself  the flag set
 * @param vout  output vertex array (cols * rows t3Vectors)
 */
// cFlagSet::updateGrid(t3Vector* verts) (SSX On Tour symbols). Animates the flag mesh: waves along the columns, then per-vertex ripples, into out[cols * rows].
extern "C" void cFlagSet_updateGrid(void* vself, t3Vector* vout)
{
    sFlagElem_BCA0* self = (sFlagElem_BCA0*)vself;
    sFsV3_CM* out = (sFsV3_CM*)vout;
    int bothHeld = self->b0 != 0 && self->mode != 0;
    int neitherHeld = self->b0 == 0 && self->mode == 0;
    float windRatio = self->blend + *(float*)((char*)cObjectInterface_getFlagManager() + 0x10) * (1.0f - self->blend);
    float aTime[4];
    float mag[4];
    int i;
    for (i = 0; i < 4; i++) {
        aTime[i] = self->phase[i] * -6.2831854820251465f;
        mag[i] = self->amp[i] * windRatio;
    }
    sFsV3_CM horizontalOffset[10];
    float fi = 0.0f;
    float accX = 0.0f;
    float accY = 0.0f;
    for (i = 0; i < self->cols; i++) {
        float iRatioMultiplier = fi / self->colScale;
        float iRatio = iRatioMultiplier;
        int idx = i;
        if (neitherHeld) {
            iRatioMultiplier = 1.0f;
        } else if (bothHeld) {
            if (iRatioMultiplier > 0.5f) {
                iRatioMultiplier = 1.0f - iRatioMultiplier;
            }
        } else if (self->mode == 1) {
            idx = self->cols - i - 1;
        }
        float colAngle = iRatio * 6.2831854820251465f;
        int n = (int)((aTime[0] + colAngle * self->freqX) * 81.4873275756836f) & 0x1FF;
        float waveSin = D_00504FB8[n];
        float waveCos = D_00504FB8[n + 0x80];
        if (!bothHeld) {
            accX += mag[0] * iRatioMultiplier * waveCos;
        }
        horizontalOffset[idx] = sFsV3_CM(mag[0] * iRatioMultiplier * waveSin, accX, 0.0f);
        n = (int)((aTime[1] + colAngle * self->freqY) * 81.4873275756836f) & 0x1FF;
        waveSin = D_00504FB8[n];
        waveCos = D_00504FB8[n + 0x80];
        if (!bothHeld) {
            accY += mag[1] * iRatioMultiplier * waveCos;
        }
        horizontalOffset[idx] += sFsV3_CM(mag[1] * iRatioMultiplier * waveSin, accY, 0.0f);
        sFsV3_CM highWindMovement(self->b.x * iRatioMultiplier * windRatio, self->b.y * iRatioMultiplier * windRatio, self->b.z * iRatioMultiplier * windRatio);
        sFsV3_CM lowWindMovement(self->a.x * iRatioMultiplier * (1.0f - windRatio), self->a.y * iRatioMultiplier * (1.0f - windRatio), self->a.z * iRatioMultiplier * (1.0f - windRatio));
        horizontalOffset[idx] += highWindMovement + lowWindMovement;
        fi += 1.0f;
    }
    int vertexIndex = 0;
    float fj = 0.0f;
    int j;
    for (j = 0; j < self->rows; j++) {
        float fi = 0.0f;
        int i;
        for (i = 0; i < self->cols; i++, fi += 1.0f) {
            float iRatioMultiplier = fi / self->colScale;
            float jRatio = fj / self->rowScale;
            float iRatio = iRatioMultiplier;
            if (bothHeld) {
                if (iRatioMultiplier > 0.5f) {
                    iRatioMultiplier = 1.0f - iRatioMultiplier;
                }
            } else if (self->mode == 1) {
                iRatioMultiplier = 1.0f - iRatioMultiplier;
                iRatio = iRatioMultiplier;
            } else if (neitherHeld) {
                iRatioMultiplier = 1.0f;
            }
            if (self->b2 && jRatio == 0.0f) {
                out[vertexIndex] = self->verts[vertexIndex];
            } else {
                out[vertexIndex] = self->verts[vertexIndex] + horizontalOffset[i];
                if (iRatio > 0.0f && mag[2] > 0.01f) {
                    float tRatio = (iRatio + jRatio) * 0.5f;
                    float tRatioMultiplier = (iRatioMultiplier + jRatio) * 0.5f;
                    int n = (int)((aTime[2] + tRatio * (self->freqZ0 * 6.2831854820251465f)) * 81.4873275756836f) & 0x1FF;
                    float waveSin = D_00504FB8[n];
                    float waveCos = D_00504FB8[n + 0x80];
                    out[vertexIndex] += sFsV3_CM(mag[2] * iRatioMultiplier * waveSin, mag[2] * tRatioMultiplier * waveCos, mag[2] * jRatio * waveSin);
                }
                if (iRatio > 0.0f && mag[3] > 0.01f) {
                    float tRatio = (iRatio + jRatio) * 0.5f;
                    float tRatioMultiplier = (iRatioMultiplier + jRatio) * 0.5f;
                    int n = (int)((aTime[3] + tRatio * (self->freqZ1 * 6.2831854820251465f)) * 81.4873275756836f) & 0x1FF;
                    float waveSin = D_00504FB8[n];
                    float waveCos = D_00504FB8[n + 0x80];
                    out[vertexIndex] += sFsV3_CM(mag[3] * iRatioMultiplier * waveSin, mag[3] * tRatioMultiplier * waveCos, mag[3] * (1.0f - jRatio) * waveSin);
                }
                vertexIndex++;
            }
        }
        fj += 1.0f;
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagSet_writeToReplayFrame);
#ifdef SKIP_ASM
struct sFlagSetVEntryC2E0a {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sFlagSetVEntryC2E0b {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sFlagSet_C2E0 {
    char pad0[0x5C];
    int count;          // 0x5C
    char pad60[0x34];
    void* items[1];     // 0x94
};

/**
 * Writes a flag set to a replay frame.
 *
 * Writes the 0x60-byte set state (flag data, model id, instance count) through the stream's write slot (vtable entry
 * 1), then each instance in mInstances (+0x94, count +0x5C) through the instance-write slot (vtable entry 5).
 *
 * @param elem  the flag set
 * @param stream  replay stream to write to
 */
extern "C" void cFlagSet_writeToReplayFrame(void* elem, void* stream)
{
    int i;
    sFlagSet_C2E0* self = (sFlagSet_C2E0*)elem;
    sFlagSetVEntryC2E0a* writeFn = &(*(sFlagSetVEntryC2E0a**)stream)[1];
    writeFn->fn((char*)stream + writeFn->delta, self, 0x60);
    for (i = 0; i < self->count; i++) {
        sFlagSetVEntryC2E0b* writeItemFn = &(*(sFlagSetVEntryC2E0b**)stream)[5];
        writeItemFn->fn((char*)stream + writeItemFn->delta, self->items[i]);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFlagSet_readFromReplayFrame);
#ifdef SKIP_ASM
struct sFlagSetVEntryC378a {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sFlagSetVEntryC378b {
    short delta;
    short index;
    void* (*fn)(void*);
};

extern "C" void cFlagSet_CreateMesh(void* self, void* item);

/**
 * Reads a flag set back from a replay frame.
 *
 * Reads the 0x60-byte set state through the stream's read slot (vtable entry 2), then each instance pointer through
 * vtable entry 3 into mInstances (+0x94), and rebuilds the cloth mesh from the first instance (cFlagSet_CreateMesh)
 * when the set is not empty.
 *
 * @param elem  the flag set
 * @param stream  replay stream to read from
 */
extern "C" void cFlagSet_readFromReplayFrame(void* elem, void* stream)
{
    int i;
    sFlagSet_C2E0* self = (sFlagSet_C2E0*)elem;
    sFlagSetVEntryC378a* readFn = &(*(sFlagSetVEntryC378a**)stream)[2];
    readFn->fn((char*)stream + readFn->delta, self, 0x60);
    for (i = 0; i < self->count; i++) {
        sFlagSetVEntryC378b* readItemFn = &(*(sFlagSetVEntryC378b**)stream)[3];
        self->items[i] = readItemFn->fn((char*)stream + readItemFn->delta);
    }
    if (self->count > 0) {
        cFlagSet_CreateMesh(self, self->items[0]);
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagManager_cFlagManager);
#ifdef SKIP_ASM
extern "C" void* cObjNode_cObjNode1(void* self, void* a1);
extern "C" void* cFlagSet_cFlagSet(void* self);
extern void* cFlagManager_vtbl[];

struct sFlagSetElem_C428 {
    char data[0x188];
};

struct sFlagSet_C428 {
    char pad0[0xC];
    void** vtable;                  // 0xC
    int field_0x10;                 // 0x10
    float field_0x14;               // 0x14
    float field_0x18;               // 0x18
    int field_0x1c;                 // 0x1C
    sFlagSetElem_C428 elems[15];    // 0x20
};

/**
 * Constructs the flag manager object node with 15 empty flag sets and its wind at base 0.5, delta 0.25.
 *
 * @param bucket  the bucket the node belongs to
 * @return @p self
 */
extern "C" sFlagSet_C428* cFlagManager_cFlagManager(sFlagSet_C428* self, void* bucket)
{
    int i;
    sFlagSetElem_C428* elem = self->elems;
    cObjNode_cObjNode1(self, bucket);
    self->vtable = cFlagManager_vtbl;
    for (i = 14; i != -1; i--, elem++) {
        cFlagSet_cFlagSet(elem);
    }
    self->field_0x14 = 0.5f;
    self->field_0x18 = 0.25f;
    self->field_0x1c = 0;
    self->field_0x10 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagManager__cFlagManager);
#ifdef SKIP_ASM
extern "C" void cObjNode__cObjNode(void* self, int flags);
extern void* cFlagManager_vtbl[];

struct sFlagSetVEntry_C4B8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sFlagSetElem_C4B8 {
    char pad0[0x184];
    sFlagSetVEntry_C4B8* vt;        // 0x184
};

struct sFlagSet_C4B8 {
    char pad0[0xC];
    void** vtable;                  // 0xC
    char pad10[0x10];
    sFlagSetElem_C4B8 elems[15];    // 0x20
};

/**
 * Destroys the flag manager: destroys its 15 flag sets, then the cObjNode base.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFlagManager__cFlagManager(sFlagSet_C4B8* self, int flags)
{
    self->vtable = cFlagManager_vtbl;
    if (self->elems != 0) {
        sFlagSetElem_C4B8* flagSet = self->elems + 15;
        while (self->elems != flagSet) {
            flagSet--;
            flagSet->vt[1].fn((char*)flagSet + flagSet->vt[1].delta, 0);
        }
    }
    cObjNode__cObjNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagManager_AddFlag);
#ifdef SKIP_ASM
struct sFlagSetElem_C548 {
    char pad0[0x5C];
    int active; // 0x5C
    char pad60[0x188 - 0x60];
};

struct sFlagSet_C548 {
    char pad0[0x20];
    sFlagSetElem_C548 elems[15]; // 0x20
};

extern "C" int cFlagSet_SameKind(void* self, void* data, void* ctx);
extern "C" void cFlagSet_AddFlag(void* self, void* data, void* ctx);

/**
 * Adds a flag instance to the flag manager, grouping it with flags of the same kind.
 *
 * First looks for a set in use (+0x5C) among the 15 sets at +0x20 (0x188 bytes each) whose cFlagSet_SameKind says it
 * matches, and adds the flag there (cFlagSet_AddFlag); otherwise adds it to the first unused set. Quirk: when all 15 sets
 * are in use and none matches, the flag is silently dropped.
 *
 * @param node  instance record of the flag
 * @param desc  converted flag data (tFlagState, 0x58 bytes)
 */
extern "C" void cFlagManager_AddFlag(sFlagSet_C548* self, void* node, void* desc)
{
    int i;
    sFlagSetElem_C548* existing;
    for (i = 0, existing = self->elems; i < 15; i++, existing++) {
        if (existing->active != 0 && cFlagSet_SameKind(existing, desc, node) != 0) {
            cFlagSet_AddFlag(existing, desc, node);
            return;
        }
    }
    sFlagSetElem_C548* freeElem = self->elems;
    for (int j = 0; j < 15; j++, freeElem++) {
        if (freeElem->active == 0) {
            cFlagSet_AddFlag(freeElem, desc, node);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagManager_RemoveFlag);
#ifdef SKIP_ASM
struct sFlagSetElem_C600 {
    char pad0[0x5C];
    int active; // 0x5C
    char pad60[0x188 - 0x60];
};

struct sFlagSet_C600 {
    char pad0[0x20];
    sFlagSetElem_C600 elems[15]; // 0x20
};

extern "C" int cFlagSet_RemoveFlag(void* elem, void* arg);

/**
 * Removes a flag instance from whichever flag-manager set holds it.
 *
 * Walks the 15 sets at +0x20 and stops at the first set in use whose cFlagSet_RemoveFlag removes the instance.
 *
 * @param instRecord  instance record of the flag
 */
extern "C" void cFlagManager_RemoveFlag(sFlagSet_C600* self, void* instRecord)
{
    int i;
    sFlagSetElem_C600* elem = self->elems;
    for (i = 0; i < 15; i++, elem++) {
        if (elem->active != 0 && cFlagSet_RemoveFlag(elem, instRecord) != 0) {
            return;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFlagManager_update);
#ifdef SKIP_ASM
struct sFlagSetElem_C668 {
    char pad0[0x5C];
    int active; // 0x5C
    char pad60[0x188 - 0x60];
};

struct sFlagSet_C668 {
    char pad0[0x10];
    float level;    // 0x10
    float base;     // 0x14
    float delta;    // 0x18
    float t;        // 0x1C
    sFlagSetElem_C668 elems[15]; // 0x20
};

extern "C" int cObjectInterface_getPeakNumber();
extern "C" void cFlagSet_update(void* elem);
unsigned int BXrand();
extern char* cAppMan_TheAppMan;

// Uniform float in [0, 1) built from the random mantissa bits.
static inline float randf_34C668()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

/**
 * Varies the wind once a second by a random step and updates every active flag set.
 *
 * The step's range is 0.15, 0.3 or 0.45 by a per-location level read from the new-race BE (cObjectInterface_getPeakNumber,
 * probably the location's wind); the wind is kept within 0..1 and interpolated over the second.
 */
extern "C" void cFlagManager_update(sFlagSet_C668* self)
{
    float amp = 0.15f;
    switch (cObjectInterface_getPeakNumber()) {
    case 1:
        break;
    case 2:
        amp = 0.3f;
        break;
    case 3:
        amp = 0.45f;
        break;
    }
    float one = 1.0f;
    self->t += one / (float)*(int*)(cAppMan_TheAppMan + 0x10);
    if (one <= self->t) {
        self->t -= one;
        self->base += self->delta;
        float lo = -amp;
        self->delta = lo + (amp - lo) * randf_34C668();
        if (self->base + self->delta < 0.0f) {
            self->delta = -self->base;
        }
        if (one < self->base + self->delta) {
            self->delta = one - self->base;
        }
    }
    self->level = self->base + self->delta * self->t;
    int i;
    for (i = 0; i < 15; i++) {
        sFlagSetElem_C668* entry = &self->elems[i];
        if (entry->active != 0) {
            cFlagSet_update(entry);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFlagManager_render);
#ifdef SKIP_ASM
struct sFlagSetEntry {
    char pad_0x0[0x5C];
    int active; // 0x5C
    char pad_0x60[0x128];
};

struct sFlagSetOwner {
    char pad_0x0[0x20];
    sFlagSetEntry entries[15]; // 0x20
};

extern "C" void cFlagSet_render(sFlagSetEntry* e);

/**
 * Renders every active flag set held by the flag manager.
 *
 * Walks the 15 entries from offset 0x20 and calls cFlagSet_render() on each whose active word is set.
 */
extern "C" void cFlagManager_render(sFlagSetOwner* self)
{
    int i;
    for (i = 0; i < 15; i++) {
        sFlagSetEntry* entry = &self->entries[i];
        if (entry->active != 0) {
            cFlagSet_render(entry);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFlagManager_writeToReplayFrame);
#ifdef SKIP_ASM
struct sFlagSetElem_C848 {
    char data[0x188];
};

struct sFlagSet_C848 {
    char pad0[0x20];
    sFlagSetElem_C848 elems[15]; // 0x20
};

extern "C" void cFlagSet_writeToReplayFrame(void* elem, void* arg);

/**
 * Saves the flag manager's flags into a replay frame.
 *
 * Calls cFlagSet_writeToReplayFrame on each of its 15 flag records with @p stream; cFlagManager_readFromReplayFrame() reads them
 * back.
 *
 * @param stream  the replay frame
 */
extern "C" void cFlagManager_writeToReplayFrame(sFlagSet_C848* self, void* stream)
{
    int i;
    for (i = 0; i < 15; i++) {
        cFlagSet_writeToReplayFrame(&self->elems[i], stream);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFlagManager_readFromReplayFrame);
#ifdef SKIP_ASM
struct sFlagSetElem_C898 {
    char data[0x188];
};

struct sFlagSet_C898 {
    char pad0[0x20];
    sFlagSetElem_C898 elems[15]; // 0x20
};

extern "C" void cFlagSet_readFromReplayFrame(void* elem, void* arg);

/**
 * Restores the flag manager's flags from a replay frame.
 *
 * Calls cFlagSet_readFromReplayFrame on each of its 15 flag records with @p stream, the inverse of cFlagManager_writeToReplayFrame().
 *
 * @param stream  the replay frame
 */
extern "C" void cFlagManager_readFromReplayFrame(sFlagSet_C898* self, void* stream)
{
    int i;
    for (i = 0; i < 15; i++) {
        cFlagSet_readFromReplayFrame(&self->elems[i], stream);
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_cFenceNode1);
#ifdef SKIP_ASM
// cFenceNode::cFenceNode(int bucket, tInstanceRecord* pInst, tMakeFenceData* tdata, const tIntersectInfo* info)
// (SSX On Tour symbols; On Tour passes the force in, retail computes it here like cFenceNode::onCollide).
struct tMakeFenceData_C8E8 {
    unsigned int w[4];
};

struct t3Vector_C8E8 {
    float x, y, z;
    t3Vector_C8E8() {}
    void* operator new[](unsigned int, void* p) { return p; }
};

struct cFenceNode_C8E8 {
    char pad0[0xC];
    void** vtable;              // 0xC
    char pad10[0x8];
    char* Instance;             // 0x18 tInstanceRecord* (matrix at +0x10)
    tMakeFenceData_C8E8 State;  // 0x1C cFenceNodeState (tResourceID first)
    int field_0x2c;             // 0x2C
    int field_0x30;             // 0x30
    int field_0x34;             // 0x34
    int field_0x38;             // 0x38
    int field_0x3c;             // 0x3C
    void* GridMesh;             // 0x40 cGridMesh*
    t3Vector_C8E8 Verts[16];    // 0x44
    char* NextInstance;         // 0x104 tInstanceRecord*
    char* PrevInstance;         // 0x108 tInstanceRecord*
};

struct sVec4_C8E8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_C8E8 {
    float m[4][4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector).
static inline sVec4_C8E8 MulMat_C8E8(const sMat_C8E8* m, const sVec4_C8E8* v)
{
    sVec4_C8E8 r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "o"(*v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4_C8E8 Normalize_C8E8(const sVec4_C8E8& v)
{
    sVec4_C8E8 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_C8E8(const sVec4_C8E8& a, const sVec4_C8E8& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: SN abs.s asm helper (PS2 FPU).
static inline float fabs_C8E8(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

extern void* cFenceNode_vtbl[];
extern sVec4_C8E8 t4vect_XAxis;
extern "C" void* cInstanceNode_cInstanceNode1(void* self, int a1, int type, void* obj);
extern "C" void cFenceNode_setupFence(cFenceNode* self);
extern "C" void cFenceNode_setForce(cFenceNode* self, float dt);
extern "C" void cFenceNode_makeNeighbour(cFenceNode* self, char* obj, int flag);

/**
 * Constructs a fence node (node type 9) for a fence instance that was hit, and sets it swinging.
 *
 * Runs the record constructor cInstanceNode_cInstanceNode1, installs cFenceNode_vtbl, constructs the 16 Verts (+0x44), copies the
 * fence make-data into FenceData (+0x1C) with InstId set to -1, zeroes Force, ForceMax and ForceStep (+0x2C..+0x34),
 * builds the grid (cFenceNode_setupFence) and zeroes NextPropTime/PrevPropTime (+0x38, +0x3C). The starting force is
 * 0.001 * |info +0x30| * (direction info +0x10 dotted with the fence's normalised x axis when it has a grid mesh
 * (+0x40), or with info +0x20 otherwise); it is applied with cFenceNode_setForce and passed on to the neighbour fences
 * (NextInstance +0x104, PrevInstance +0x108) through cFenceNode_makeNeighbour.
 *
 * @param bucket  update bucket the node joins
 * @param pInst  fence instance record
 * @param tdata  fence make-data (InstId, Type, Flexibility)
 * @param info  intersect info of the hit
 * @return self
 */
extern "C" cFenceNode_C8E8* cFenceNode_cFenceNode1(cFenceNode* self, int bucket, void* pInst, tMakeFenceData_C8E8* tdata, char* info)
{
    float force;
    cInstanceNode_cInstanceNode1(((cFenceNode_C8E8*)self), bucket, 9, pInst);
    ((unsigned int*)&self->FenceData)[0] = 0xFFFFFFFF;
    self->mVtbl = cFenceNode_vtbl;
    new (((t3Vector_C8E8*)&self->Verts[0])) t3Vector_C8E8[16];
    (*(tMakeFenceData_C8E8*)&self->FenceData) = *tdata;
    *(int*)&self->ForceMax = 0;
    *(int*)&self->Force = 0;
    *(int*)&self->ForceStep = 0;
    ((unsigned int*)&self->FenceData)[0] = 0xFFFFFFFF;
    cFenceNode_setupFence(self);
    self->NextPropTime = 0;
    self->PrevPropTime = 0;
    if (self->GridMesh != 0) {
        sVec4_C8E8 dir = Normalize_C8E8(MulMat_C8E8((sMat_C8E8*)((char*)self->Instance + 0x10), &t4vect_XAxis));
        float speed = fabs_C8E8(*(float*)(info + 0x30));
        force = Dot_C8E8(dir, *(sVec4_C8E8*)(info + 0x10));
        force = speed * (force * 0.0010000000474974513f);
    } else {
        force = Dot_C8E8(*(sVec4_C8E8*)(info + 0x20), *(sVec4_C8E8*)(info + 0x10));
        force = force * 0.0010000000474974513f * fabs_C8E8(*(float*)(info + 0x30));
    }
    cFenceNode_setForce(self, force);
    if ((char*)self->NextInstance != 0)
        cFenceNode_makeNeighbour(self, (char*)self->NextInstance, 1);
    if ((char*)self->PrevInstance != 0)
        cFenceNode_makeNeighbour(self, (char*)self->PrevInstance, 0);
    return ((cFenceNode_C8E8*)self);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_cFenceNode2);
#ifdef SKIP_ASM
struct sFlagBlock_CAB8 {
    unsigned int w[4];
};

struct sFlagElem_CAB8 {
    float v[4];
    sFlagElem_CAB8() {}
    void* operator new[](unsigned int, void* p) { return p; }
};

struct sFlagSet_CAB8 {
    char pad0[0xC];
    void** vtable;              // 0xC
    char pad10[0xC];
    sFlagBlock_CAB8 block;      // 0x1C
    int field_0x2c;             // 0x2C
    int field_0x30;             // 0x30
    int field_0x34;             // 0x34
    int field_0x38;             // 0x38
    int field_0x3c;             // 0x3C
};

extern "C" void* cInstanceNode_cInstanceNode1(void* self, int a1, int type, void* obj);
extern "C" void cFenceNode_setupFence(cFenceNode* self);
extern "C" void cFenceNode_setForce(cFenceNode* self, float dt);
extern void* cFenceNode_vtbl[];

// PORT: the unit declares this as void (callers ignore the result); bound by asm label.
extern "C" sFlagSet_CAB8* cFenceNode_cFenceNode2_ctor(sFlagSet_CAB8* self, int a1, void* obj, sFlagBlock_CAB8* desc, int flag, float dt) __asm__("cFenceNode_cFenceNode2");
/**
 * Constructs a fence node for a neighbouring fence instance that a swinging fence pushes.
 *
 * Runs the record constructor cInstanceNode_cInstanceNode1 (node type 9), installs cFenceNode_vtbl, constructs the Verts, copies desc
 * into FenceData with InstId -1, zeroes Force/ForceMax/ForceStep, builds the grid (cFenceNode_setupFence), applies dt
 * (cFenceNode_setForce) and sets both NextPropTime and PrevPropTime (+0x38, +0x3C) to 4. flag is not used.
 *
 * @param bucket  update bucket the node joins
 * @param obj  fence instance record
 * @param desc  fence make-data copied from the pushing fence
 * @param flag  whether this is the next (1) or previous (0) neighbour; unused
 * @param dt  force to start with
 * @return self
 */
extern "C" sFlagSet_CAB8* cFenceNode_cFenceNode2_ctor(sFlagSet_CAB8* self, int bucket, void* obj, sFlagBlock_CAB8* desc, int flag, float dt)
{
    cInstanceNode_cInstanceNode1(self, bucket, 9, obj);
    self->block.w[0] = 0xFFFFFFFF;
    self->vtable = cFenceNode_vtbl;
    new ((char*)self + 0x40) sFlagElem_CAB8[16];
    self->block = *desc;
    self->field_0x30 = 0;
    self->field_0x2c = 0;
    self->field_0x34 = 0;
    self->block.w[0] = 0xFFFFFFFF;
    cFenceNode_setupFence((cFenceNode*)self);
    cFenceNode_setForce((cFenceNode*)self, dt);
    self->field_0x38 = 4;
    self->field_0x3c = 4;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode__cFenceNode);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan;
extern void* cFenceNode_vtbl[];
extern "C" void cInstanceNode__cInstanceNode(void* self, int flags);

struct sFlagSetVEntryCB80 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

/**
 * Destroys a swinging fence node: releases its grid mesh (renderer slot 77), then the cInstanceNode base.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFenceNode__cFenceNode(cFenceNode* self, int flags)
{
    self->mVtbl = cFenceNode_vtbl;
    void* gridMesh = self->GridMesh;
    if (gridMesh != 0) {
        char* mgr = cGraphicsMan_TheGraphicsMan;
        sFlagSetVEntryCB80* vt = *(sFlagSetVEntryCB80**)(mgr + 0x10D8);
        vt[77].fn(mgr + vt[77].delta, gridMesh);
    }
    cInstanceNode__cInstanceNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_cFenceNode);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream);
extern "C" void cFenceNode_setupFence(cFenceNode* self);
extern void* cFenceNode_vtbl[];

struct sFlagSetVEntry_CBE8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sFlagElem_CBE8 {
    float v[4];
    sFlagElem_CBE8() {}
    void* operator new[](unsigned int, void* p) { return p; }
};

/**
 * Recreates a swinging fence node from a replay frame: the instance-node part, its 0x24-byte swing state and
 * its mesh.
 *
 * @param bucket  the bucket the node belongs to
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cFenceNode_cFenceNode(cFenceNode* self, void* bucket, void* stream)
{
    cInstanceNode_cInstanceNode(self, bucket, stream);
    self->FenceData.InstId = 0xFFFFFFFF;
    self->mVtbl = cFenceNode_vtbl;
    new ((char*)&self->GridMesh) sFlagElem_CBE8[16];
    sFlagSetVEntry_CBE8* vtEntry = &(*(sFlagSetVEntry_CBE8**)stream)[2];
    vtEntry->fn((char*)stream + vtEntry->delta, (char*)&self->FenceData, 36);
    cFenceNode_setupFence(self);
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFenceNode_makeNeighbour);
#ifdef SKIP_ASM
class cFlagObj_CC80 {
public:
    int f0, f4, f8;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual int isA(int type);
};

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cFenceNode_cFenceNode2(void* self, int a1, void* obj, void* pos, int flag, float force);
extern "C" void cFenceNode_neighbourCollide(cFenceNode* self, int flag, float force);
extern char str_Fence_b[];

// PORT: SN abs.s asm helper (PS2 FPU).
static inline float fabs_34CC80(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Passes a fence's swing on to one neighbouring fence instance.
 *
 * Does nothing without a grid mesh (+0x40) or neighbour. The force passed on is ForceMax (+0x30) * 0.4, with the sign
 * opposite to ForceStep (+0x34). If the neighbour already has a node (+0xC) that is a fence (isType(9)) and |force| is
 * at least 0.25, the node of NextInstance (+0x104) when flag is set, else of PrevInstance (+0x108), gets cFenceNode_neighbourCollide.
 * If the neighbour has no node and |force| is at least 0.25, a 268-byte "Fence" node is allocated and built with
 * cFenceNode_cFenceNode2 (bucket 1, this fence's FenceData).
 *
 * @param obj  neighbour instance record (NextInstance or PrevInstance)
 * @param flag  1 for the next neighbour, 0 for the previous
 */
extern "C" void cFenceNode_makeNeighbour(cFenceNode* self, char* obj, int flag)
{
    if ((int)self->GridMesh == 0) {
        return;
    }
    if (obj == 0) {
        return;
    }
    float force;
    if (self->ForceStep < 0.0f) {
        force = self->ForceMax * 0.4f;
    } else {
        force = self->ForceMax * -0.4f;
    }
    cFlagObj_CC80* node = *(cFlagObj_CC80**)(obj + 0xC);
    if (node != 0) {
        if (node->isA(9) == 0) {
            return;
        }
        if (!(0.25f <= fabs_34CC80(force))) {
            return;
        }
        if (flag != 0) {
            char* neighbor = (char*)self->NextInstance;
            if (neighbor == 0) {
                return;
            }
            void* neighborNode = *(void**)(neighbor + 0xC);
            if (neighborNode == 0) {
                return;
            }
            cFenceNode_neighbourCollide((cFenceNode*)neighborNode, flag, force);
        } else {
            char* neighbor = (char*)self->PrevInstance;
            if (neighbor == 0) {
                return;
            }
            void* neighborNode = *(void**)(neighbor + 0xC);
            if (neighborNode == 0) {
                return;
            }
            cFenceNode_neighbourCollide((cFenceNode*)neighborNode, 0, force);
        }
    } else if (0.25f <= fabs_34CC80(force)) {
        void* fence = cMemMan_alloc(268, str_Fence_b, 0x20000000, 0);
        cFenceNode_cFenceNode2(fence, 1, obj, ((char*)self) + 0x1C, flag, force);
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_neighbourCollide);
#ifdef SKIP_ASM
extern "C" void cFenceNode_setForce(cFenceNode* self, float dt);

/**
 * Applies a force arriving from a neighbouring fence and arms the propagation timer for that side.
 *
 * Calls cFenceNode_setForce with dt, then sets NextPropTime (+0x38) when on is set, else PrevPropTime (+0x3C), to 4 if it is
 * 0.
 *
 * @param on  1 when the force came through the next neighbour, 0 for the previous
 * @param dt  force to apply
 */
extern "C" void cFenceNode_neighbourCollide(cFenceNode* self, int on, float dt)
{
    cFenceNode_setForce(self, dt);
    if (on) {
        if (self->NextPropTime == 0) {
            self->NextPropTime = 4;
        }
    } else {
        if (self->PrevPropTime == 0) {
            self->PrevPropTime = 4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_update);
#ifdef SKIP_ASM
extern "C" void cFenceNode_makeNeighbour(cFenceNode* self, char* obj, int flag);
extern "C" void cFenceNode_updateGrid(cFenceNode* self);

struct sFlagVE_34CE48 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

// PORT: SN abs.s asm helper (PS2 FPU).
static inline float fabs_34CE48(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Swings a hit fence section back and forth with a decaying amplitude and passes the swing on to its
 * neighbours.
 *
 * Each neighbour timer that runs out pushes the next or previous fence section (cFenceNode_makeNeighbour). At each end
 * of a
 * swing the amplitude halves; once it and the angle are near 0 the node expires (slot 34).
 */
extern "C" void cFenceNode_update(cFenceNode* self)
{
    if (self->NextPropTime > 0) {
        if (--self->NextPropTime == 0) {
            cFenceNode_makeNeighbour(self, (char*)self->NextInstance, 1);
        }
    }
    if (self->PrevPropTime > 0) {
        if (--self->PrevPropTime == 0) {
            cFenceNode_makeNeighbour(self, (char*)self->PrevInstance, 0);
        }
    }
    char* osc = ((char*)self) + 0x1C;
    self->Force += self->ForceStep;
    if (self->ForceMax < 0.01f && fabs_34CE48(*(float*)(osc + 0x10)) < 0.01f) {
        sFlagVE_34CE48* vt = (sFlagVE_34CE48*)self->mVtbl;
        vt[34].fn(((char*)self) + vt[34].delta, 1);
        return;
    }
    if (0.0f <= self->ForceStep) {
        if (self->Force >= self->ForceMax) {
            float amplitude = self->ForceMax;
            self->Force = amplitude;
            float halfAmp = amplitude * 0.5f;
            self->ForceMax = halfAmp;
            self->ForceStep = -halfAmp * 0.1f;
        }
    } else {
        float amplitude = self->ForceMax;
        float na = -amplitude;
        if (self->Force <= na) {
            self->Force = na;
            float halfAmp = amplitude * 0.5f;
            self->ForceMax = halfAmp;
            self->ForceStep = halfAmp * 0.1f;
        }
    }
    cFenceNode_updateGrid(self);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_updateGrid);
#ifdef SKIP_ASM
extern float D_0044AFD0[];

struct sFsVec4_CF98 {
    float x, y, z, w;
    sFsVec4_CF98() {}
    sFsVec4_CF98(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sFsVec3_CF98 {
    float x, y, z;
    sFsVec3_CF98() {}
    sFsVec3_CF98(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

// PORT: VU0 macro-mode vector scale in place (v *= s).
static inline void fsScaleIn_CF98(sFsVec4_CF98& v, float s)
{
    __asm__(
        "mfc1       $2, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "qmtc2.ni   $2, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2       $vf5, 0x0(%1)\n"
        : "=m"(v)
        : "r"(&v), "f"(s)
        : "$2");
}

// PORT: VU0 macro-mode vector scale (a * s).
static inline sFsVec4_CF98 fsScale_CF98(const sFsVec4_CF98& a, float s)
{
    sFsVec4_CF98 r;
    __asm__(
        "mfc1       $2, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "qmtc2.ni   $2, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2       $vf5, %0\n"
        : "=m"(r)
        : "r"(&a), "f"(s)
        : "$2");
    return r;
}

static inline sFsVec3_CF98 fsAdd_CF98(const sFsVec3_CF98& a, const sFsVec4_CF98& b)
{
    return sFsVec3_CF98(a.x + b.x, a.y + b.y, a.z + b.z);
}

struct sFlagVEntry_CF98 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sFlagMesh_CF98 {
    int pad0;
    sFlagVEntry_CF98* vt;   // 0x4
};

struct sFlagSet_CF98 {
    char pad0[0x20];
    int mode;               // 0x20
    float scale;            // 0x24
    char pad28[0x4];
    float wind;             // 0x2C
    char pad30[0x10];
    sFlagMesh_CF98* mesh;   // 0x40
    sFsVec3_CF98 top[8];    // 0x44
    sFsVec3_CF98 bottom[8]; // 0xA4
};

/**
 * Bends a fence's grid mesh by its current force.
 *
 * Does nothing without a grid mesh (+0x40). The sway vector is (200, 0, 0) * Force (+0x2C) * Flexibility (+0x24); each
 * of the 8 columns gets it scaled by the weight table D_0044AFD0. The top row (+0x44) always moves; the bottom row
 * (+0xA4) moves too when the fence Type (+0x20) is 1, otherwise it stays at rest. The 16 points go to the mesh's vtable
 * entry 3.
 *
 * @param fenceNode  the fence node
 */
extern "C" void cFenceNode_updateGrid(cFenceNode* fenceNode)
{
    sFlagSet_CF98* self = (sFlagSet_CF98*)fenceNode;
    if (self->mesh == 0) {
        return;
    }
    float swing = self->wind * self->scale;
    sFsVec4_CF98 dir(200.0f, 0.0f, 0.0f, 0.0f);
    fsScaleIn_CF98(dir, swing);
    sFsVec4_CF98 offs[8];
    sFsVec4_CF98* offsetIter = offs;
    for (int i = 0; i < 8; i++) {
        *offsetIter++ = fsScale_CF98(dir, D_0044AFD0[i]);
    }
    sFsVec3_CF98 pts[16];
    if (self->mode == 1) {
        for (int i = 0; i < 8; i++) {
            pts[i] = fsAdd_CF98(self->top[i], offs[i]);
            pts[i + 8] = fsAdd_CF98(self->bottom[i], offs[i]);
        }
    } else {
        for (int i = 0; i < 8; i++) {
            pts[i] = fsAdd_CF98(self->top[i], offs[i]);
            pts[i + 8] = self->bottom[i];
        }
    }
    sFlagMesh_CF98* gridMesh = self->mesh;
    sFlagVEntry_CF98* setVertsSlot = &gridMesh->vt[3];
    setVertsSlot->fn((char*)gridMesh + setVertsSlot->delta, pts);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_setupFence);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan;
extern "C" void cFenceNode_updateGrid(cFenceNode* self);

struct sFsV3_D1E8 {
    float x, y, z;
    sFsV3_D1E8() {}
    sFsV3_D1E8(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

struct sFsV2_D1E8 {
    float x, y;
    sFsV2_D1E8() {}
    sFsV2_D1E8(float ax, float ay) : x(ax), y(ay) {}
};

struct sFsCol_D1E8 {
    float a, r, g, b;
    sFsCol_D1E8() {}
};

inline sFsV3_D1E8 operator+(const sFsV3_D1E8& a, const sFsV3_D1E8& b) { return sFsV3_D1E8(a.x + b.x, a.y + b.y, a.z + b.z); }
inline sFsV3_D1E8 operator-(const sFsV3_D1E8& a, const sFsV3_D1E8& b) { return sFsV3_D1E8(a.x - b.x, a.y - b.y, a.z - b.z); }
inline sFsV3_D1E8 operator*(const sFsV3_D1E8& a, float s) { return sFsV3_D1E8(a.x * s, a.y * s, a.z * s); }
inline sFsV2_D1E8 operator+(const sFsV2_D1E8& a, const sFsV2_D1E8& b) { return sFsV2_D1E8(a.x + b.x, a.y + b.y); }
inline sFsV2_D1E8 operator-(const sFsV2_D1E8& a, const sFsV2_D1E8& b) { return sFsV2_D1E8(a.x - b.x, a.y - b.y); }
inline sFsV2_D1E8 operator*(const sFsV2_D1E8& a, float s) { return sFsV2_D1E8(a.x * s, a.y * s); }

struct sFsVE_D1E8 {
    short delta;
    short index;
    void* (*fn)(void*, int, int, unsigned int);
};

struct sFsQuadVE_D1E8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int, sFsV3_D1E8*, sFsV2_D1E8*, sFsCol_D1E8*);
};

struct sFsMeshVE_D1E8 {
    short delta;
    short index;
    void (*fn)(void*, sFsV3_D1E8*, sFsCol_D1E8*, sFsV2_D1E8*);
};

struct sFsMesh_D1E8 {
    int pad0;
    sFsMeshVE_D1E8* vt;   // 0x4
};

struct sFsNode_D1E8 {
    char pad0[0x8];
    unsigned int flags;   // 0x8
};

struct sFlagSet_D1E8 {
    char pad0[0x18];
    sFsNode_D1E8* node;     // 0x18
    char pad1C[0x24];
    sFsMesh_D1E8* mesh;     // 0x40
    sFsV3_D1E8 top[8];      // 0x44
    sFsV3_D1E8 bottom[8];   // 0xA4
    void* obj104;           // 0x104
    void* obj108;           // 0x108
};

/**
 * Builds a fence's flexible grid mesh from its instance's quad: 8 columns of top and bottom vertices.
 *
 * Clears the neighbour instances (+0x104, +0x108) and marks the instance record (+0x18, flags +0x8: clear bit 1, set
 * bit 2). The graphics manager (cGraphicsMan_TheGraphicsMan, vtable +0x10D8) creates an 8x2 grid mesh (slot 76, flags
 * 0x60000000) into +0x40 and returns mesh 0's quad (slot 97: 4 corners, UVs, colours). The 8 top (+0x44) and bottom
 * (+0xA4) vertices, 16 UVs and 16 colours (alpha 1.0) are interpolated in sevenths between the quad's corners, handed
 * to the grid mesh (its slot 6), and cFenceNode_updateGrid updates the grid.
 *
 * @param fenceNode  the fence node
 */
extern "C" void cFenceNode_setupFence(cFenceNode* fenceNode)
{
    sFlagSet_D1E8* self = (sFlagSet_D1E8*)fenceNode;
    self->obj104 = 0;
    self->obj108 = 0;
    self->node->flags = (self->node->flags & ~2u) | 4;
    char* mgr = cGraphicsMan_TheGraphicsMan;
    sFsVE_D1E8* createSlot = &(*(sFsVE_D1E8**)(mgr + 0x10D8))[76];
    self->mesh = (sFsMesh_D1E8*)createSlot->fn(mgr + createSlot->delta, 8, 2, 0x60000000u);
    sFsV3_D1E8 pos[4];
    sFsV2_D1E8 quv[4];
    sFsCol_D1E8 qcol[4];
    sFsQuadVE_D1E8* vt = *(sFsQuadVE_D1E8**)(mgr + 0x10D8);
    vt[97].fn(mgr + vt[97].delta, self->node, 0, pos, quv, qcol);
    sFsV2_D1E8 uv[16];
    sFsCol_D1E8 col[16];
    for (int i = 0; i < 8; i++) {
        float column = (float)i;
        self->top[i] = pos[0] + (pos[1] - pos[0]) * column * 0.1428571492433548f;
        self->bottom[i] = pos[2] + (pos[3] - pos[2]) * column * 0.1428571492433548f;
        uv[i] = quv[0] + (quv[1] - quv[0]) * column * 0.1428571492433548f;
        uv[i + 8] = quv[2] + (quv[3] - quv[2]) * column * 0.1428571492433548f;
        col[i].a = 1.0f;
        col[i].r = qcol[0].r + (qcol[1].r - qcol[0].r) * column * 0.1428571492433548f;
        col[i].g = qcol[0].g + (qcol[1].g - qcol[0].g) * column * 0.1428571492433548f;
        col[i].b = qcol[0].b + (qcol[1].b - qcol[0].b) * column * 0.1428571492433548f;
        col[i + 8].a = 1.0f;
        col[i + 8].r = qcol[2].r + (qcol[3].r - qcol[2].r) * column * 0.1428571492433548f;
        col[i + 8].g = qcol[2].g + (qcol[3].g - qcol[2].g) * column * 0.1428571492433548f;
        col[i + 8].b = qcol[2].b + (qcol[3].b - qcol[2].b) * column * 0.1428571492433548f;
    }
    sFsMesh_D1E8* gridMesh = self->mesh;
    gridMesh->vt[6].fn((char*)gridMesh + gridMesh->vt[6].delta, pos, col, uv);
    cFenceNode_updateGrid((cFenceNode*)self);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_onCollide);
#ifdef SKIP_ASM
extern "C" void cFenceNode_setForce(cFenceNode* self, float dt);

struct sFsVec4_D650 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float fsDot_D650(const sFsVec4_D650& a, const sFsVec4_D650& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

/**
 * Adds a collision's push to the fence's swing, unless it is already swinging fully, and arms both neighbour
 * timers.
 *
 * The C declaration keeps only the collision info (On Tour also passes the intersect info and the being).
 *
 * @param obj  the collision info (On Tour const tCollideInfo* tc)
 */
extern "C" void cFenceNode_onCollide(cFenceNode* self, void* obj)
{
    char* hitInfo = (char*)obj;
    if (self->ForceMax >= 1.0f) return;
    float dot = fsDot_D650(*(sFsVec4_D650*)(hitInfo + 0x20), *(sFsVec4_D650*)(hitInfo + 0x10));
    cFenceNode_setForce(self, dot * 0.0010000000474974513f * *(float*)(hitInfo + 0x30));
    if (self->NextPropTime == 0) self->NextPropTime = 4;
    if (self->PrevPropTime == 0) self->PrevPropTime = 4;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_setForce);
#ifdef SKIP_ASM
struct sFlag34D6F0 {
    char pad0[0x30];
    float wind;     // 0x30
    float speed;    // 0x34
};

// PORT: SN abs.s asm helper (PS2 FPU).
static inline float fabs_34D6F0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Adds a hit force to a fence's swing.
 *
 * ForceMax (+0x30) grows by |force| up to 1. ForceStep (+0x34) becomes ForceMax * 0.1, signed like force when the fence
 * is at rest (ForceStep 0) and keeping its current sign otherwise.
 *
 * @param vself  the fence node
 * @param force  force to add (sign gives the direction when at rest)
 */
extern "C" void cFenceNode_setForce(cFenceNode* vself, float force)
{
    sFlag34D6F0* self = (sFlag34D6F0*)vself;
    self->wind += fabs_34D6F0(force);
    if (self->wind > 1.0f) {
        self->wind = 1.0f;
    }
    if (self->speed == 0.0f) {
        if (force < 0.0f) {
            self->speed = self->wind * -0.1f;
        } else {
            self->speed = self->wind * 0.1f;
        }
    } else if (self->speed < 0.0f) {
        self->speed = self->wind * -0.1f;
    } else {
        self->speed = self->wind * 0.1f;
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_render);
#ifdef SKIP_ASM
/**
 * Draws a fence section, through its swinging grid mesh when it has one, unless its model is culled or not
 * loaded.
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void cFenceNode_render(cFenceNode* self)
{
    if (!IsLoaded_34B9B0((sModel_34B9B0*)self->Instance)) {
        return;
    }
    char* ctx = cGraphicsMan_TheGraphicsMan;
    float one = 1.0f;
    {
        sModel_34B9B0* model = (sModel_34B9B0*)self->Instance;
        sVE93_34B9B0* vt = *(sVE93_34B9B0**)(ctx + 0x10D8);
        char* thisp = ctx + vt[93].delta;
        sVec4_34B9B0 boxMin(*(float*)((char*)model + 0x60), *(float*)((char*)model + 0x64), *(float*)((char*)model + 0x68), one);
        sVec4_34B9B0 boxMax(*(float*)((char*)model + 0x6C), *(float*)((char*)model + 0x70), *(float*)((char*)model + 0x74), one);
        sVec4_34B9B0* pb = &boxMax;
        sVE43_34B9B0* vt43 = *(sVE43_34B9B0**)(ctx + 0x10D8);
        sPair_34B9B0 cullInfo = vt43[43].fn(ctx + vt43[43].delta);
        if (vt[93].fn(thisp, &boxMin, pb, &cullInfo) == 1) {
            return;
        }
    }
    TOP_34B9B0(ctx)[1] = TOP_34B9B0(ctx)[0];
    sRS_34B9B0* renderState = TOP_34B9B0(ctx);
    TOP_34B9B0(ctx) = renderState + 1;
    ((int*)renderState)[6] = (((int*)renderState)[6] & ~0x7C) | 0x14;
    sModel_34B9B0* model = (sModel_34B9B0*)self->Instance;
    char* pos = (char*)model + 0x10;
    char* gridMesh = (char*)self->GridMesh;
    if (gridMesh != 0) {
        sVE7_34B9B0* hv = *(sVE7_34B9B0**)(gridMesh + 4);
        hv[7].fn(gridMesh + hv[7].delta, pos, *(void**)((char*)model + 0x94), 0);
    } else {
        sVE96_34B9B0* vt96 = *(sVE96_34B9B0**)(ctx + 0x10D8);
        vt96[96].fn(ctx + vt96[96].delta, model, 0, 0, 0);
    }
    TOP_34B9B0(ctx)--;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sSerVEntry_0034D960 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a fence node to a replay frame: the instance-node part and its 0x24-byte swing state.
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cFenceNode_writeToReplayFrame(cFenceNode* self, void* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    sSerVEntry_0034D960* vt = *(sSerVEntry_0034D960**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)&self->FenceData, 36);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode_cAnimNode);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode1(void* self, void* a1, int type, void* a3);
extern "C" void cAnimNode_setAnimMeshCache(void* self);
extern "C" float cAnimFrame_getStartTime(void* p);
struct sFlagOwner_E348;
extern "C" void cAnimNode_cacheFrame(cAnimNode* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern void* D_00490CC8[];
extern char str_AnimMats[];

struct sFlagSlot_34D9B0 {
    int v[16];
    sFlagSlot_34D9B0() {}
};

/**
 * Constructs the animation-node base of an animated object for a placed instance.
 *
 * Builds the cMoveNode part at +0x14 (cMoveNode_cMoveNode1) and installs D_00490CC8 as its vtable. Allocates one 64-byte
 * matrix per mesh of the instance's model (model def +0x80, count +0x4) into AnimMats (+0x44, "AnimMats") and builds
 * the mesh cache (cAnimNode_setAnimMeshCache). The frame block at +0x0 starts at the first frame's start time
 * (cAnimFrame_getStartTime of +0x48, else 0) in +0x0 and +0x4, frame events at 1e10 (+0x8, +0xC) and +0x10 = 1. When
 * force is set or the record flags have bit 0, the record flags get bit 0x4 and lose bit 0x2. Ends with
 * cAnimNode_cacheFrame.
 *
 * @param bucket  update bucket
 * @param nodeType  node type
 * @param instRecord  instance record to animate
 * @param force  force the instance visible
 * @return self
 */
extern "C" void* cAnimNode_cAnimNode(cAnimNode* self, void* bucket, int nodeType, void* instRecord, int force)
{
    char* node = ((char*)self) + 0x14;
    cMoveNode_cMoveNode1(node, bucket, nodeType, instRecord);
    self->mVtbl = D_00490CC8;
    sFlagSlot_34D9B0** slot = (sFlagSlot_34D9B0**)(((char*)self) + 0x44);
    *slot = new (str_AnimMats, 0x20000000, 0) sFlagSlot_34D9B0[*(int*)(*(char**)(*(char**)(node + 0x18) + 0x80) + 4)];
    cAnimNode_setAnimMeshCache(((char*)self));
    float startTime = 0.0f;
    if (self->AnimFrames != 0) {
        startTime = cAnimFrame_getStartTime(self->AnimFrames);
    }
    self->CurrTime = startTime;
    self->FrameStart = 10000000000.0f;
    self->CacheTime = startTime;
    self->MeshVelocityDirty = 1;
    self->FrameEnd = 10000000000.0f;
    char* sub = ((char*)self) + 0x14;
    if (force != 0 || (*(unsigned int*)(*(char**)(sub + 0x18) + 8) & 1)) {
        char* n = *(char**)(sub + 0x18);
        *(unsigned int*)(n + 8) = (*(unsigned int*)(n + 8) & ~2u) | 4;
    }
    cAnimNode_cacheFrame(self);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode_cAnimNode1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMoveNode_cMoveNode_3(void* self, void* a1, void* stream) __asm__("cMoveNode_cMoveNode");

struct sStreamVE_DAC8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

// Element type with an empty constructor; the array new leaves only its loop counters.
struct sFlagSlot_34DAC8 {
    int v[16];
    sFlagSlot_34DAC8() {}
};

/**
 * Constructs the animation-node base of an animated object from a replay frame.
 *
 * Runs the cMoveNode replay constructor on +0x14, installs D_00490CC8, allocates AnimMats like cAnimNode_cAnimNode, builds
 * the mesh cache, reads the 20-byte frame block at +0x0 and one more 4-byte word that it discards, then
 * cAnimNode_cacheFrame.
 *
 * @param bucket  update bucket
 * @param stream  replay stream to read from
 * @return self
 */
extern "C" void* cAnimNode_cAnimNode1(cAnimNode* self, void* bucket, void* stream)
{
    int skippedWord;
    char* node = ((char*)self) + 0x14;
    sFlagSlot_34DAC8** slot = (sFlagSlot_34DAC8**)(((char*)self) + 0x44);
    cMoveNode_cMoveNode_3(node, bucket, stream);
    self->mVtbl = D_00490CC8;
    *slot = new (str_AnimMats, 0x20000000, 0) sFlagSlot_34DAC8[*(int*)(*(char**)(*(char**)(node + 0x18) + 0x80) + 4)];
    cAnimNode_setAnimMeshCache(((char*)self));
    sStreamVE_DAC8* vt = *(sStreamVE_DAC8**)stream;
    vt[2].fn((char*)stream + vt[2].delta, ((char*)self), 20);
    sStreamVE_DAC8* vt2 = *(sStreamVE_DAC8**)stream;
    vt2[2].fn((char*)stream + vt2[2].delta, &skippedWord, 4);
    cAnimNode_cacheFrame(self);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode__cAnimNode);
#ifdef SKIP_ASM
struct sFlagVEntry_DBA8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sFlagItem_DBA8 {
    char pad_0x0[0x84];
    sFlagVEntry_DBA8* vt;       // 0x84
    char pad_0x88[0x48];
};

struct sFlagOwner_DBA8 {
    char pad_0x0[0x14];
    char node[0xC];             // 0x14
    void** vtable;              // 0x20
    char pad_0x24[0x20];
    void* cache;                // 0x44
    sFlagItem_DBA8* items;      // 0x48, array-new block (count at -0x10)
    void* buf;                  // 0x4C
};

void cMemMan_free(void*);
void operator_delete(int*);
extern "C" void cMoveNode__cMoveNode(void* self, int flags);
extern void* D_00490CC8[];

/**
 * Destroys the animation-node base of an animated object.
 *
 * Reinstalls D_00490CC8, frees AnimMats (+0x44), runs each AnimFrames element's destructor (+0x48, vtable entry 1, in
 * reverse) and frees the array, frees +0x4C, destroys the cMoveNode part (cMoveNode__cMoveNode) and deletes the object
 * when bit 0 of flags is set.
 *
 * @param flags  destructor flags; bit 0 deletes the storage
 */
extern "C" void cAnimNode__cAnimNode(cAnimNode* self, int flags)
{
    self->mVtbl = D_00490CC8;
    if (self->AnimMats != 0) {
        cMemMan_free(self->AnimMats);
    }
    sFlagItem_DBA8* items = (sFlagItem_DBA8*)self->AnimFrames;
    if (items != 0) {
        sFlagItem_DBA8* frameIter = items + ((int*)items)[-4];
        while ((sFlagItem_DBA8*)self->AnimFrames != frameIter) {
            frameIter--;
            frameIter->vt[1].fn((char*)frameIter + frameIter->vt[1].delta, 0);
        }
        cMemMan_free((char*)(sFlagItem_DBA8*)self->AnimFrames - 0x10);
    }
    if (self->MeshVelocity != 0) {
        cMemMan_free(self->MeshVelocity);
    }
    cMoveNode__cMoveNode(((char*)&self->unk_014), 0);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode_updateRootTransform);
#ifdef SKIP_ASM
struct sFlagSetVEntry_DC90a {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sFlagSetVEntry_DC90b {
    short delta;
    short index;
    void* (*fn)(void*);
};

struct sFlagSetVEntry_DC90c {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Rebuilds the animation matrices after the node's root transform changed.
 *
 * Calls virtual slot 52 (cAnimNode_cacheFrame, cache the frame) first, then makeAnimMats (slot 33,
 * cAnimNode_makeAnimMats) on the root transform from slot 24 (cMoveNode_getRootTransform) into AnimMats (+0x44),
 * and clears RootTransformDirty (bit 0 of the cMoveNode part's Flags, +0x26). Overrides
 * cMoveNode_updateRootTransform (vtable slot 51).
 */
extern "C" void cAnimNode_updateRootTransform(cAnimNode* self)
{
    sFlagSetVEntry_DC90a* vt = (sFlagSetVEntry_DC90a*)self->mVtbl;
    vt[52].fn((char*)self + vt[52].delta);
    char* sub = (char*)self + 0x14;
    sFlagSetVEntry_DC90c* vt2 = (sFlagSetVEntry_DC90c*)self->mVtbl;
    void* objB = sub + vt2[33].delta;
    void* rootTransform = ((sFlagSetVEntry_DC90b*)vt2)[24].fn(sub + vt2[24].delta);
    vt2[33].fn(objB, rootTransform, (int)self->AnimMats);
    self->Flags &= ~1;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode_makeAnimMats);
#ifdef SKIP_ASM
extern "C" void* cAnimNode_getFrameMat(cAnimNode* self, int i);

struct sFsMat44_DD18 {
    float m[4][4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMatFS(sFsMat44_DD18* d, const sFsMat44_DD18* a, const sFsMat44_DD18* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(d), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0CopyMatFS(sFsMat44_DD18* d, const sFsMat44_DD18* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

struct sFsV4_DD18 {
    float v[4];
    sFsV4_DD18() {}
    sFsV4_DD18(const float& x, const float& y, const float& z, const float& w)
    {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
    }
} __attribute__((aligned(16)));

static inline sFsV4_DD18 operator*(const sFsV4_DD18& a, const sFsV4_DD18& b)
{
    return sFsV4_DD18(a.v[0] * b.v[0], a.v[1] * b.v[1], a.v[2] * b.v[2], a.v[3] * b.v[3]);
}

struct sFsMtx_DD18 {
    float m[4][4];
    sFsMtx_DD18() {}
    sFsMtx_DD18(const sFsMtx_DD18& o) { vu0CopyMatFS((sFsMat44_DD18*)this, (const sFsMat44_DD18*)&o); }
    sFsV4_DD18 getRow(int i) const { return *(const sFsV4_DD18*)m[i]; }
    sFsMtx_DD18& operator=(const sFsMtx_DD18& o)
    {
        vu0CopyMatFS((sFsMat44_DD18*)this, (const sFsMat44_DD18*)&o);
        return *this;
    }
} __attribute__((aligned(16)));

// parent * (local bone matrix with its translation scaled)
static inline sFsMtx_DD18 mulScaledFS(const sFsMtx_DD18& parent, const sFsMtx_DD18& src, const sFsV4_DD18& s)
{
    sFsMtx_DD18 m;
    *(sFsV4_DD18*)m.m[0] = src.getRow(0);
    *(sFsV4_DD18*)m.m[1] = src.getRow(1);
    *(sFsV4_DD18*)m.m[2] = src.getRow(2);
    *(sFsV4_DD18*)m.m[3] = src.getRow(3) * s;
    sFsMtx_DD18 r;
    vu0MulMatFS((sFsMat44_DD18*)&r, (const sFsMat44_DD18*)&parent, (const sFsMat44_DD18*)&m);
    return r;
}

struct sFsBone_DD18 {
    int parent;             // 0x0
    char pad_0x4[0x4];
    int animated;           // 0x8: matrix comes from cAnimNode_getFrameMat
    sFsMtx_DD18* mtx;       // 0xC
};

struct sFsSkel_DD18 {
    char pad_0x0[0x4];
    int count;              // 0x4
    sFsBone_DD18* bones;    // 0x8
};

struct sFsModel_DD18 {
    char pad_0x0[0x80];
    sFsSkel_DD18* skel;     // 0x80
    float scale;            // 0x84
};

/**
 * Builds the world matrix of every bone of the node's model from the root or its parent bone.
 *
 * Animated bones take the current animation matrix (cAnimNode_getFrameMat), others their rest matrix; translations
 * are
 * scaled by the model's scale.
 *
 * @param vroot  the root transform (On Tour const t4Matrix* root)
 * @param out  receives one matrix per bone (On Tour animMats)
 */
// Builds the world matrix of every bone: root or parent * (bone matrix with its translation scaled).
extern "C" void cAnimNode_makeAnimMats(cAnimNode* self, t4Matrix* vroot, sFsMtx_DD18* out)
{
    sFsMtx_DD18* root = (sFsMtx_DD18*)vroot;
    sFsModel_DD18* model = (sFsModel_DD18*)self->Instance;
    sFsSkel_DD18* skel = model->skel;
    float modelScale = model->scale;
    sFsBone_DD18* bone = skel->bones;
    sFsV4_DD18 scale(modelScale, modelScale, modelScale, 1.0f);
    int k = 0;
    for (int i = 0; i < skel->count; i++, bone++) {
        if (bone->animated) {
            if (bone->parent != -1) {
                out[i] = mulScaledFS(out[bone->parent], *(sFsMtx_DD18*)cAnimNode_getFrameMat(self, k++), scale);
            } else {
                out[i] = mulScaledFS(*root, *(sFsMtx_DD18*)cAnimNode_getFrameMat(self, k++), scale);
            }
        } else {
            if (bone->parent != -1) {
                out[i] = mulScaledFS(out[bone->parent], *bone->mtx, scale);
            } else {
                out[i] = mulScaledFS(*root, *bone->mtx, scale);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode_getFrameMat);
#ifdef SKIP_ASM
extern char t4mat_Identity[];

/**
 * Returns the frame matrix of one animated mesh, or the identity matrix when there are no anim frames.
 *
 * @param i  animated-mesh index
 * @return AnimFrames[i] +0x90 (0xD0-byte frames at +0x48), or t4mat_Identity
 */
extern "C" void* cAnimNode_getFrameMat(cAnimNode* self, int i) {
    char* base = (char*)self->AnimFrames;
    if (base == 0) {
        return t4mat_Identity;
    }
    return base + i * 208 + 0x90;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode_cacheFrame);
#ifdef SKIP_ASM
struct sFlagSetVEntry_E348 {
    short delta;
    short index;
    void (*fn)(void*, float);
};

struct sFlagItem_E348 {
    char pad0[0x84];
    sFlagSetVEntry_E348* vt;    // 0x84
    char pad88[0x48];
};

struct sFlagOwner_E348 {
    int field_0x0;
    float time;                 // 0x4
    char pad8[0x8];
    int dirty;                  // 0x10
    char pad14[0x2C];
    int count;                  // 0x40
    int field_0x44;
    sFlagItem_E348* items;      // 0x48
};

/**
 * Brings every animated mesh's frame to the node's cache time and marks the mesh velocities stale.
 *
 * For each of NumAnimMeshes (+0x40) entries of AnimFrames (+0x48, 0xD0 bytes each) it calls the frame's virtual slot
 * 2 with CacheTime (+0x4); then sets MeshVelocityDirty (+0x10) so cAnimNode_updateMeshVelocity recomputes the velocities on demand.
 * Slot 52 of cAnimNode's vtable.
 */
extern "C" void cAnimNode_cacheFrame(cAnimNode* self)
{
    int i;
    if ((sFlagItem_E348*)self->AnimFrames != 0) {
        for (i = 0; i < self->NumAnimMeshes; i++) {
            // PORT: pointer arithmetic done in int (gives the target's offset-first addu).
            sFlagItem_E348* animFrame = (sFlagItem_E348*)(i * 0xD0 + (int)(sFlagItem_E348*)self->AnimFrames);
            animFrame->vt[2].fn((char*)animFrame + animFrame->vt[2].delta, self->CacheTime);
        }
    }
    self->MeshVelocityDirty = 1;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sSerVEntry_0034E3D8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cMoveNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes the animation-node base of an animated object to a replay frame.
 *
 * Writes the cMoveNode part (cMoveNode_writeToReplayFrame on +0x14), then the 20-byte frame block at +0x0 and a 4-byte
 * stack word that is never set (the replay constructor reads and drops it).
 *
 * @param stream  replay stream to write to
 */
extern "C" void cAnimNode_writeToReplayFrame(cAnimNode* self, void* stream)
{
    int dummy;
    cMoveNode_writeToReplayFrame((char*)self + 0x14, stream);
    sSerVEntry_0034E3D8* writeFn = &(*(sSerVEntry_0034E3D8**)stream)[1];
    writeFn->fn((char*)stream + writeFn->delta, self, 20);
    writeFn = &(*(sSerVEntry_0034E3D8**)stream)[1];
    writeFn->fn((char*)stream + writeFn->delta, &dummy, 4);
}
#endif

