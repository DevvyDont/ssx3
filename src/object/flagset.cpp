#include "vtable.h"
#include "common.h"
#include "object/flagnode.h"
#include "world/worldcache.h"
#include "object/movenode.h"
#include "ai/rider.h"
#include "object/animframe.h"
#include "render/ps2gridmesh.h"
#include "world/instancerecord.h"
#include "world/worldrecords.h"
#include "bx/appman.h"
#include "object/flagset.h"
#include "object/animnode.h"
#include "object/fencenode.h"
#include "ui/uithing.h"
#include "bx/math3d.h"
#include "world/world.h"
#include "replay/bxreplayframe.h"
#include "object/instancenode.h"
#include "render/ps2graphicsman.h"

//100%
INCLUDE_ASM("object/flagset", cFlagSet_CreateMesh);
#ifdef SKIP_ASM
extern char str_Flag_Verts[];
unsigned int BXrand();
extern "C" void cFlagSet_updateGrid(cFlagSet* self, t3Vector* verts);

// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");

// t3Vector / t2Vector / tARGB with the constructors the unit's arithmetic needs.
struct sFsV3_CM : t3Vector {
    sFsV3_CM() {}
    sFsV3_CM(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};

struct sFsV2_CM : t2Vector {
    sFsV2_CM() {}
    sFsV2_CM(float ax, float ay) { x = ax; y = ay; }
};

struct sFsCol_CM : tARGB {
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

struct sFsMeshVE_CM {
    short delta;
    short index;
    void (*fn)(void*, sFsV3_CM*, sFsCol_CM*, sFsV2_CM*);
};

struct sFsMesh_CM {
    int pad0;
    sFsMeshVE_CM* vt;   // 0x4
};


/**
 * Builds a flag set's cloth grid mesh from a flag instance's quad, then computes its first frame.
 *
 * The grid is 8 by 2 vertices when the set has almost no wind, otherwise 8 by 5; positions, UVs and colours
 * are
 * interpolated from the quad's corners (renderer slot 97). The four flag phases start random.
 *
 * @param item  the flag's instance record (On Tour tInstanceRecord* pInst)
 */
void cFlagSet::CreateMesh(tInstanceRecord* item)
{
    cFlagSet* self = this;
    int i;
    for (i = 0; i < 4; i++) {
        self->mFlagTime[i] = randf_CM();
    }
    self->mScrollU = 0.0f;
    self->mScrollV = 0.0f;
    cPS2GraphicsMan* mgr = cGraphicsMan_TheGraphicsMan;
    sFsV2_CM quv[4];
    sFsCol_CM qcol[4];
    sFsV3_CM pos[4];
    mgr->GetMeshQuad(item, 0, pos, quv, qcol);
    if (self->mFlagData.mMagnitude[2] < 0.01f && self->mFlagData.mMagnitude[3] < 0.01f) {
        self->mFlagVertsHorizontal = 8;
        self->mFlagVertsVertical = 2;
    } else {
        self->mFlagVertsHorizontal = 8;
        self->mFlagVertsVertical = 5;
    }
    self->mfFlagVertsHorizontalMinusOne = (float)(self->mFlagVertsHorizontal - 1);
    self->mfFlagVertsVerticalMinusOne = (float)(self->mFlagVertsVertical - 1);
    self->mGridMesh = mgr->CreateGridMesh(self->mFlagVertsHorizontal, self->mFlagVertsVertical, 0x60000000u);
    if (self->mGridMesh == 0) {
        return;
    }
    int n = self->mFlagVertsHorizontal * self->mFlagVertsVertical;
    t3Vector** pv = &self->mVerts;
    *pv = new (str_Flag_Verts, 0x20000000, 0) sFsV3_CM[n];
    sFsV2_CM uv[50];
    sFsCol_CM col[50];
    int idx = 0;
    for (int row = 0; row < self->mFlagVertsVertical; row++) {
        for (int column = 0; column < self->mFlagVertsHorizontal; column++) {
            float fc = (float)column / self->mfFlagVertsHorizontalMinusOne;
            float fr = (float)row / self->mfFlagVertsVerticalMinusOne;
            self->mVerts[idx] =
                (pos[0] * (1.0f - fc) + pos[1] * fc) * (1.0f - fr) + (pos[2] * (1.0f - fc) + pos[3] * fc) * fr;
            uv[idx] = (quv[0] * (1.0f - fc) + quv[1] * fc) * (1.0f - fr) + (quv[2] * (1.0f - fc) + quv[3] * fc) * fr;
            col[idx].A = 1.0f;
            col[idx].R = (qcol[0].R * (1.0f - fc) + qcol[1].R * fc) * (1.0f - fr) +
                         (qcol[2].R * (1.0f - fc) + qcol[3].R * fc) * fr;
            col[idx].G = (qcol[0].G * (1.0f - fc) + qcol[1].G * fc) * (1.0f - fr) +
                         (qcol[2].G * (1.0f - fc) + qcol[3].G * fc) * fr;
            col[idx].B = (qcol[0].B * (1.0f - fc) + qcol[1].B * fc) * (1.0f - fr) +
                         (qcol[2].B * (1.0f - fc) + qcol[3].B * fc) * fr;
            idx++;
        }
    }
    sFsV3_CM verts[50];
    self->updateGrid((t3Vector*)verts);
    sFsMesh_CM* flagMesh = ((sFsMesh_CM*)self->mGridMesh);
    flagMesh->vt[6].fn(vthis(flagMesh, flagMesh->vt[6]), verts, col, uv);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagSet_PurgeMesh);
#ifdef SKIP_ASM
void cMemMan_free(void*);

/**
 * Frees a flag set's grid mesh and vertex buffer.
 *
 * Releases the grid mesh (+0x60) through graphics-manager slot 77 and frees the vertex array (+0x74) with cMemMan_free,
 * nulling each pointer.
 */
void cFlagSet::PurgeMesh()
{
    cGridMesh* mesh = mGridMesh;
    if (mesh != 0) {
        cGraphicsMan_TheGraphicsMan->DeleteGridMesh(mesh);
        mGridMesh = 0;
    }
    void* verts = mVerts;
    if (verts != 0) {
        cMemMan_free(verts);
        mVerts = 0;
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagSet_update);
#ifdef SKIP_ASM
extern int cFlags_m_EnableUpdate;
extern "C" void* cObjectInterface_getFlagManager();
extern "C" int cObjectInterface_getAITick();
extern "C" void cFlagSet_updateGrid(cFlagSet* self, t3Vector* verts);





/**
 * Updates a flag set: advances its wave clocks, regenerates the cloth every other frame and scrolls its texture.
 *
 * Returns when flag updates are off (cFlags_m_EnableUpdate), the set is empty (+0x5C) or has no grid mesh (+0x60). The wind blend
 * is mMinWindFactor (+0x4C) + the flag manager's wind factor (cObjectInterface_getFlagManager() +0x10) * (1 - mMinWindFactor); each of
 * the 4 clocks mFlagTime (+0x78) advances by mSpeed[i] (+0x4) times that and wraps at 1. On frames whose parity
 * (cObjectInterface_getAITick() % 2) equals mFrameToUpdateGrid (+0x90) it builds up to 50 vertices with cFlagSet_updateGrid and hands them
 * to the mesh's vtable entry 3. When either scroll rate (+0x50, +0x54) exceeds 0.001 it adds them to mScrollU/V (+0x88,
 * +0x8C), wrapping at 1.
 *
 */
void cFlagSet::update()
{
    cFlagSet* self = this;
    if (cFlags_m_EnableUpdate == 0) {
        return;
    }
    if (self->mCurrentNumInstances == 0) {
        return;
    }
    if (self->mGridMesh == 0) {
        return;
    }
    cFlagManager* wind = (cFlagManager*)cObjectInterface_getFlagManager();
    float k = self->mFlagData.mMinWindFactor;
    float windBlend = k + wind->mCurrentWindFactor * (1.0f - k);
    int i;
    for (i = 0; i < 4; i++) {
        self->mFlagTime[i] += self->mFlagData.mSpeed[i] * windBlend;
        if (1.0f <= self->mFlagTime[i]) {
            self->mFlagTime[i] -= 1.0f;
        }
    }
    if (cObjectInterface_getAITick() % 2 == self->mFrameToUpdateGrid) {
        sFsV3_CM verts[50];
        self->updateGrid(verts);
        cGridMesh* gridMesh = self->mGridMesh;
        sVEntry<void(void*, void*)>* vt = (sVEntry<void(void*, void*)>*)gridMesh->mVtbl;
        sVEntry<void(void*, void*)>* setVertsSlot = &vt[3];
        setVertsSlot->fn(vthis(gridMesh, *setVertsSlot), verts);
    }
    if (0.001f < self->mFlagData.mUScrollRate || 0.001f < self->mFlagData.mVScrollRate) {
        self->mScrollU += self->mFlagData.mUScrollRate;
        if (1.0f < self->mScrollU) {
            self->mScrollU -= 1.0f;
        }
        self->mScrollV += self->mFlagData.mVScrollRate;
        if (1.0f < self->mScrollV) {
            self->mScrollV -= 1.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagSet_render);
#ifdef SKIP_ASM
extern int cFlags_m_EnableRender;
// PORT: 128-bit integer mode for VU0 asm memory operands
typedef int cQuad128_34B9B0 __attribute__((mode(TI)));
extern t4Matrix t4mat_Identity;

extern sDrawState sDrawState_s_defaultDrawState;

// t4Vector with a 4-float constructor (the bound-box corners).
struct t4VectorXYZW : t4Vector {
    t4VectorXYZW() {}
    t4VectorXYZW(const float& a, const float& b, const float& c, const float& d) { x = a; y = b; z = c; w = d; }
};

// A draw state's alpha field (bits 12-19 of its first word) and blend mode (bits 2-6 of its second).
static inline void SetDrawStateAlpha(sDrawState* ds, int v)
{
    ds->m_bitGroups[0] = ((v << 12) & 0xFF000) | ((int)ds->m_bitGroups[0] & ~0xFF000);
}
static inline void SetDrawStateBlendMode(sDrawState* ds, int v)
{
    ds->m_bitGroups[1] = ((int)ds->m_bitGroups[1] & ~0x7C) | ((v << 2) & 0x7C);
}

// The graphics-manager slots the renders call by hand: 43 GetNetMatrix, 93 ClipBoundBox, 96 RenderModel.
typedef sVEntry<t4Matrix(void*)> tNetMatrixSlot;
typedef sVEntry<int(void*, t4Vector*, t4Vector*, t4Matrix*)> tClipBoundBoxSlot;
typedef sVEntry<void(void*, tInstanceRecord*, int, int, int)> tRenderModelSlot;
// The grid mesh's draw slot 7: (instance matrix, material block, texture matrix).
typedef sVEntry<void(void*, void*, void*, t4Matrix*)> tGridMeshRenderSlot;


static inline int IsLoaded_34B9B0(tInstanceRecord* m)
{
    cWorldCache* wc = cWorld_m_pTheWorld->m_pWorldCache;
    if (m->m_SMap.m_uiSectionID == 0xFF)
        return 1;
    int ok = wc->m_WorldView.mp_SectionState[m->m_SMap.m_uiSectionID].state == 6;
    return ok ? wc->m_HullPages[m->m_SMap.m_iHullID].m_State == 3 : 0;
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void MatCopy_34B9B0(t4Matrix* d, const t4Matrix* s)
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


/**
 * Draws every instance of a flag set with its cloth mesh and scrolled texture.
 *
 * Returns when flag rendering is off (cFlags_m_EnableRender). Pushes a copy of the default draw state; with mOverSaturate (+0x3)
 * set it forces the alpha field to 0xFF and blend mode 11, otherwise blend mode 5. For each instance in mInstances
 * (+0x94, count +0x5C) whose world-cache page is loaded and whose bounding box (+0x60..+0x74) survives the cull
 * (graphics-manager slot 93 against slot 43's view), it builds an identity texture matrix translated by mScrollU/V
 * (+0x88, +0x8C) and draws the grid mesh (+0x60, its vtable entry 7, with the instance matrix and its +0x94) or,
 * without a mesh, the instance itself (graphics-manager slot 96). Pops the draw state.
 *
 */
void cFlagSet::render()
{
    if (cFlags_m_EnableRender == 0) {
        return;
    }
    cPS2GraphicsMan* gm = cGraphicsMan_TheGraphicsMan;
    gm->m_pCurrentDrawState[1] = gm->m_pCurrentDrawState[0];
    *++gm->m_pCurrentDrawState = sDrawState_s_defaultDrawState;
    if (*(signed char*)&mFlagData.mOverSaturate != 0) {
        SetDrawStateAlpha(cGraphicsMan_TheGraphicsMan->m_pCurrentDrawState, 0xFF);
        SetDrawStateBlendMode(cGraphicsMan_TheGraphicsMan->m_pCurrentDrawState, 11);
    } else {
        SetDrawStateBlendMode(gm->m_pCurrentDrawState, 5);
    }
    float one = 1.0f;
    for (int i = 0; i < mCurrentNumInstances; i++) {
        tInstanceRecord* instRecord = mInstances[i];
        if (!IsLoaded_34B9B0(instRecord)) {
            continue;
        }
        {
            tClipBoundBoxSlot* vt = (tClipBoundBoxSlot*)VPTR_OF(cGraphicsMan, gm);
            void* thisp = vthis(gm, vt[93]);
            t4VectorXYZW boxMin(instRecord->m_fvMin.m[0], instRecord->m_fvMin.m[1], instRecord->m_fvMin.m[2], one);
            t4VectorXYZW boxMax(instRecord->m_fvMax.m[0], instRecord->m_fvMax.m[1], instRecord->m_fvMax.m[2], one);
            t4Vector* pb = &boxMax;
            tNetMatrixSlot* vt43 = (tNetMatrixSlot*)VPTR_OF(cGraphicsMan, gm);
            t4Matrix viewMat = vt43[43].fn(vthis(gm, vt43[43]));
            if (vt[93].fn(thisp, &boxMin, pb, &viewMat) == 1) {
                continue;
            }
        }
        {
            void* pos = &instRecord->m_fmTransform;
            t4Matrix mat;
            MatCopy_34B9B0(&mat, &t4mat_Identity);
            mat.row[3].x = mScrollU;
            mat.row[3].y = mScrollV;
            cGridMesh* gridMesh = mGridMesh;
            if (gridMesh != 0) {
                tGridMeshRenderSlot* hv = (tGridMeshRenderSlot*)gridMesh->mVtbl;
                hv[7].fn(vthis(gridMesh, hv[7]), pos, instRecord->m_MaterialBlock, &mat);
            } else {
                tRenderModelSlot* drawSlot = (tRenderModelSlot*)VPTR_OF(cGraphicsMan, gm);
                drawSlot[96].fn(vthis(gm, drawSlot[96]), instRecord, 0, 0, 0);
            }
        }
    }
    gm->m_pCurrentDrawState--;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagSet_updateGrid);
#ifdef SKIP_ASM
extern float cSinCosTable_gSinCosTable[];

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
 * cosines come from the 512-entry table cSinCosTable_gSinCosTable. Grid size: mFlagVertsHorizontal/Vertical (+0x64, +0x68).
 *
 * Likely bug: vertexIndex only advances in the moving branch, so when the top row is held its rest vertices are copied
 * but the index does not move past them, and the next row overwrites them.
 *
 * @param vself  the flag set
 * @param vout  output vertex array (cols * rows t3Vectors)
 */
// cFlagSet::updateGrid(t3Vector* verts) (SSX On Tour symbols). Animates the flag mesh: waves along the columns, then per-vertex ripples, into out[cols * rows].
void cFlagSet::updateGrid(t3Vector* vout)
{
    sFlagElem_BCA0* self = (sFlagElem_BCA0*)this;
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
        float waveSin = cSinCosTable_gSinCosTable[n];
        float waveCos = cSinCosTable_gSinCosTable[n + 0x80];
        if (!bothHeld) {
            accX += mag[0] * iRatioMultiplier * waveCos;
        }
        horizontalOffset[idx] = sFsV3_CM(mag[0] * iRatioMultiplier * waveSin, accX, 0.0f);
        n = (int)((aTime[1] + colAngle * self->freqY) * 81.4873275756836f) & 0x1FF;
        waveSin = cSinCosTable_gSinCosTable[n];
        waveCos = cSinCosTable_gSinCosTable[n + 0x80];
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
                    float waveSin = cSinCosTable_gSinCosTable[n];
                    float waveCos = cSinCosTable_gSinCosTable[n + 0x80];
                    out[vertexIndex] += sFsV3_CM(mag[2] * iRatioMultiplier * waveSin, mag[2] * tRatioMultiplier * waveCos, mag[2] * jRatio * waveSin);
                }
                if (iRatio > 0.0f && mag[3] > 0.01f) {
                    float tRatio = (iRatio + jRatio) * 0.5f;
                    float tRatioMultiplier = (iRatioMultiplier + jRatio) * 0.5f;
                    int n = (int)((aTime[3] + tRatio * (self->freqZ1 * 6.2831854820251465f)) * 81.4873275756836f) & 0x1FF;
                    float waveSin = cSinCosTable_gSinCosTable[n];
                    float waveCos = cSinCosTable_gSinCosTable[n + 0x80];
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


/**
 * Writes a flag set to a replay frame.
 *
 * Writes the 0x60-byte set state (flag data, model id, instance count) through the stream's write slot (vtable entry
 * 1), then each instance in mInstances (+0x94, count +0x5C) through the instance-write slot (vtable entry 5).
 *
 * @param stream  replay stream to write to
 */
void cFlagSet::writeToReplayFrame(cBxReplayFrame* stream)
{
    int i;
    cFlagSet* self = this;
    stream->write((const char*)self, 0x60);
    for (i = 0; i < self->mCurrentNumInstances; i++) {
        stream->writeReplayInstance(self->mInstances[i]);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFlagSet_readFromReplayFrame);
#ifdef SKIP_ASM


extern "C" void cFlagSet_CreateMesh(cFlagSet* self, tInstanceRecord* item);

/**
 * Reads a flag set back from a replay frame.
 *
 * Reads the 0x60-byte set state through the stream's read slot (vtable entry 2), then each instance pointer through
 * vtable entry 3 into mInstances (+0x94), and rebuilds the cloth mesh from the first instance (cFlagSet_CreateMesh)
 * when the set is not empty.
 *
 * @param stream  replay stream to read from
 */
void cFlagSet::readFromReplayFrame(cBxReplayFrame* stream)
{
    int i;
    cFlagSet* self = this;
    stream->read((char*)self, 0x60);
    for (i = 0; i < self->mCurrentNumInstances; i++) {
        self->mInstances[i] = stream->readReplayInstance();
    }
    if (self->mCurrentNumInstances > 0) {
        self->CreateMesh(self->mInstances[0]);
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagManager_cFlagManager);
#ifdef SKIP_ASM
extern "C" void* cObjNode_cObjNode1(void* self, void* a1);
extern "C" void* cFlagSet_cFlagSet(void* self);
extern void* cFlagManager_vtbl[];



/**
 * Constructs the flag manager object node with 15 empty flag sets and its wind at base 0.5, delta 0.25.
 *
 * @param bucket  the bucket the node belongs to
 * @return @p self
 */
extern "C" cFlagManager* cFlagManager_cFlagManager(cFlagManager* self, void* bucket)
{
    int i;
    cFlagSet* elem = ((&self->mFlagSets[0]));
    cObjNode_cObjNode1((self), bucket);
    VPTR_OF(cObjNode, self) = cFlagManager_vtbl;
    for (i = 14; i != -1; i--, elem++) {
        cFlagSet_cFlagSet(elem);
    }
    self->mWindBase = 0.5f;
    self->mWindDelta = 0.25f;
    *(int*)&self->mWindTime = 0;
    *(int*)&self->mCurrentWindFactor = 0;
    return (self);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagManager__cFlagManager);
#ifdef SKIP_ASM
extern "C" void cObjNode__cObjNode(void* self, int flags);
extern void* cFlagManager_vtbl[];



/**
 * Destroys the flag manager: destroys its 15 flag sets, then the cObjNode base.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFlagManager__cFlagManager(cFlagManager* self, int flags)
{
    VPTR_OF(cObjNode, self) = cFlagManager_vtbl;
    // The array destructor: each set's virtual destructor (slot 1), last to first.
    if (&self->mFlagSets[0] != 0) {
        cFlagSet* flagSet = &self->mFlagSets[15];
        while (&self->mFlagSets[0] != flagSet) {
            flagSet--;
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)flagSet->mVtbl;
            vt[1].fn(vthis(flagSet, vt[1]), 0);
        }
    }
    cObjNode__cObjNode((self), flags);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagManager_AddFlag);
#ifdef SKIP_ASM

struct sFlagSet_C548 {
    char pad0[0x20];
    cFlagSet elems[15]; // 0x20
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
void cFlagManager::AddFlag(cFlagNode* node, void* desc)
{
    int i;
    cFlagSet* existing;
    for (i = 0, existing = ((&mFlagSets[0])); i < 15; i++, existing++) {
        if (existing->mCurrentNumInstances != 0 && cFlagSet_SameKind(existing, desc, node) != 0) {
            cFlagSet_AddFlag(existing, desc, node);
            return;
        }
    }
    cFlagSet* freeElem = ((&mFlagSets[0]));
    for (int j = 0; j < 15; j++, freeElem++) {
        if (freeElem->mCurrentNumInstances == 0) {
            cFlagSet_AddFlag(freeElem, desc, node);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFlagManager_RemoveFlag);
#ifdef SKIP_ASM

struct sFlagSet_C600 {
    char pad0[0x20];
    cFlagSet elems[15]; // 0x20
};

extern "C" int cFlagSet_RemoveFlag(void* elem, void* arg);

/**
 * Removes a flag instance from whichever flag-manager set holds it.
 *
 * Walks the 15 sets at +0x20 and stops at the first set in use whose cFlagSet_RemoveFlag removes the instance.
 *
 * @param instRecord  instance record of the flag
 */
void cFlagManager::RemoveFlag(tInstanceRecord* instRecord)
{
    int i;
    cFlagSet* elem = ((&mFlagSets[0]));
    for (i = 0; i < 15; i++, elem++) {
        if (elem->mCurrentNumInstances != 0 && cFlagSet_RemoveFlag(elem, instRecord) != 0) {
            return;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFlagManager_update);
#ifdef SKIP_ASM

struct sFlagSet_C668 {
    char pad0[0x10];
    float level;    // 0x10
    float base;     // 0x14
    float delta;    // 0x18
    float t;        // 0x1C
    cFlagSet elems[15]; // 0x20
};

extern "C" int cObjectInterface_getPeakNumber();
extern "C" void cFlagSet_update(cFlagSet* elem);
unsigned int BXrand();

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
extern "C" void cFlagManager_update(cFlagManager* self)
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
    self->mWindTime += one / (float)cAppMan_TheAppMan->TimerHz;
    if (one <= self->mWindTime) {
        self->mWindTime -= one;
        self->mWindBase += self->mWindDelta;
        float lo = -amp;
        self->mWindDelta = lo + (amp - lo) * randf_34C668();
        if (self->mWindBase + self->mWindDelta < 0.0f) {
            self->mWindDelta = -self->mWindBase;
        }
        if (one < self->mWindBase + self->mWindDelta) {
            self->mWindDelta = one - self->mWindBase;
        }
    }
    self->mCurrentWindFactor = self->mWindBase + self->mWindDelta * self->mWindTime;
    int i;
    for (i = 0; i < 15; i++) {
        cFlagSet* entry = &((&self->mFlagSets[0]))[i];
        if (entry->mCurrentNumInstances != 0) {
            entry->update();
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFlagManager_render);
#ifdef SKIP_ASM
extern "C" void cFlagSet_render(cFlagSet* e);

/**
 * Renders every active flag set held by the flag manager.
 *
 * Walks the 15 flag sets (mFlagSets) and calls cFlagSet_render() on each that holds instances (mCurrentNumInstances).
 */
extern "C" void cFlagManager_render(cFlagManager* self)
{
    int i;
    for (i = 0; i < 15; i++) {
        cFlagSet* entry = &self->mFlagSets[i];
        if (entry->mCurrentNumInstances != 0) {
            entry->render();
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFlagManager_writeToReplayFrame);
#ifdef SKIP_ASM

struct sFlagSet_C848 {
    char pad0[0x20];
    cFlagSet elems[15]; // 0x20
};

extern "C" void cFlagSet_writeToReplayFrame(cFlagSet* elem, cBxReplayFrame* arg);

/**
 * Saves the flag manager's flags into a replay frame.
 *
 * Calls cFlagSet_writeToReplayFrame on each of its 15 flag records with @p stream; cFlagManager_readFromReplayFrame() reads them
 * back.
 *
 * @param stream  the replay frame
 */
extern "C" void cFlagManager_writeToReplayFrame(cFlagManager* self, cBxReplayFrame* stream)
{
    int i;
    for (i = 0; i < 15; i++) {
        (((&self->mFlagSets[0]))[i]).writeToReplayFrame(stream);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/flagset", cFlagManager_readFromReplayFrame);
#ifdef SKIP_ASM

struct sFlagSet_C898 {
    char pad0[0x20];
    cFlagSet elems[15]; // 0x20
};

extern "C" void cFlagSet_readFromReplayFrame(cFlagSet* elem, cBxReplayFrame* arg);

/**
 * Restores the flag manager's flags from a replay frame.
 *
 * Calls cFlagSet_readFromReplayFrame on each of its 15 flag records with @p stream, the inverse of cFlagManager_writeToReplayFrame().
 *
 * @param stream  the replay frame
 */
extern "C" void cFlagManager_readFromReplayFrame(cFlagManager* self, cBxReplayFrame* stream)
{
    int i;
    for (i = 0; i < 15; i++) {
        (((&self->mFlagSets[0]))[i]).readFromReplayFrame(stream);
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_cFenceNode1);
#ifdef SKIP_ASM

struct t3Vector_C8E8 {
    float x, y, z;
    t3Vector_C8E8() {}
    void* operator new[](unsigned int, void* p) { return p; }
};



struct sMat_C8E8 {
    float m[4][4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector).
static inline t4Vector MulMat_C8E8(const sMat_C8E8* m, const t4Vector* v)
{
    t4Vector r;
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
static inline t4Vector Normalize_C8E8(const t4Vector& v)
{
    t4Vector r;
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
static inline float Dot_C8E8(const t4Vector& a, const t4Vector& b)
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
extern t4Vector t4vect_XAxis;
extern "C" void* cInstanceNode_cInstanceNode1(void* self, int a1, int type, void* obj);
extern "C" void cFenceNode_setupFence(cFenceNode* self);
extern "C" void cFenceNode_setForce(cFenceNode* self, float dt);
extern "C" void cFenceNode_makeNeighbour(cFenceNode* self, tInstanceRecord* obj, int flag);

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
cFenceNode* cFenceNode::cFenceNode1(int bucket, void* pInst, tMakeFenceData* tdata, tCollideInfo* info)
{
    float force;
    cInstanceNode_cInstanceNode1((this), bucket, 9, pInst);
    FenceData.InstId = 0xFFFFFFFF;
    VPTR_OF(cObjNode, this) = cFenceNode_vtbl;
    new (((t3Vector_C8E8*)&Verts[0])) t3Vector_C8E8[16];
    *(&FenceData) = *tdata;
    *(int*)&ForceMax = 0;
    *(int*)&Force = 0;
    *(int*)&ForceStep = 0;
    FenceData.InstId = 0xFFFFFFFF;
    setupFence();
    NextPropTime = 0;
    PrevPropTime = 0;
    if (GridMesh != 0) {
        t4Vector dir = Normalize_C8E8(MulMat_C8E8((sMat_C8E8*)(&Instance->m_fmTransform), &t4vect_XAxis));
        float speed = fabs_C8E8(info->Vel);
        force = Dot_C8E8(dir, *(t4Vector*)&info->Direct);
        force = speed * (force * 0.0010000000474974513f);
    } else {
        force = Dot_C8E8(*(t4Vector*)&info->Reflect, *(t4Vector*)&info->Direct);
        force = force * 0.0010000000474974513f * fabs_C8E8(info->Vel);
    }
    setForce(force);
    if ((char*)NextInstance != 0)
        makeNeighbour((tInstanceRecord*)NextInstance, 1);
    if ((char*)PrevInstance != 0)
        makeNeighbour((tInstanceRecord*)PrevInstance, 0);
    return (this);
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
    ((cFenceNode*)self)->setupFence();
    ((cFenceNode*)self)->setForce(dt);
    self->field_0x38 = 4;
    self->field_0x3c = 4;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode__cFenceNode);
#ifdef SKIP_ASM
extern void* cFenceNode_vtbl[];
extern "C" void cInstanceNode__cInstanceNode(void* self, int flags);

/**
 * Destroys a swinging fence node: releases its grid mesh (renderer slot 77), then the cInstanceNode base.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cFenceNode__cFenceNode(cFenceNode* self, int flags)
{
    VPTR_OF(cObjNode, self) = cFenceNode_vtbl;
    cGridMesh* gridMesh = self->GridMesh;
    if (gridMesh != 0) {
        cGraphicsMan_TheGraphicsMan->DeleteGridMesh(gridMesh);
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
extern "C" void* cFenceNode_cFenceNode(cFenceNode* self, void* bucket, cBxReplayFrame* stream)
{
    cInstanceNode_cInstanceNode(self, bucket, stream);
    self->FenceData.InstId = 0xFFFFFFFF;
    VPTR_OF(cObjNode, self) = cFenceNode_vtbl;
    new ((char*)&self->GridMesh) sFlagElem_CBE8[16];
    stream->read((char*)&self->FenceData, 36);
    self->setupFence();
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
void cFenceNode::makeNeighbour(tInstanceRecord* obj, int flag)
{
    if ((int)GridMesh == 0) {
        return;
    }
    if (((char*)obj) == 0) {
        return;
    }
    float force;
    if (ForceStep < 0.0f) {
        force = ForceMax * 0.4f;
    } else {
        force = ForceMax * -0.4f;
    }
    cFlagObj_CC80* node = (cFlagObj_CC80*)obj->m_pActiveNode;
    if (node != 0) {
        if (node->isA(9) == 0) {
            return;
        }
        if (!(0.25f <= fabs_34CC80(force))) {
            return;
        }
        if (flag != 0) {
            tInstanceRecord* neighbor = NextInstance;
            if (((char*)neighbor) == 0) {
                return;
            }
            void* neighborNode = neighbor->m_pActiveNode;
            if (neighborNode == 0) {
                return;
            }
            ((cFenceNode*)neighborNode)->neighbourCollide(flag, force);
        } else {
            tInstanceRecord* neighbor = PrevInstance;
            if (((char*)neighbor) == 0) {
                return;
            }
            void* neighborNode = neighbor->m_pActiveNode;
            if (neighborNode == 0) {
                return;
            }
            ((cFenceNode*)neighborNode)->neighbourCollide(0, force);
        }
    } else if (0.25f <= fabs_34CC80(force)) {
        void* fence = cMemMan_alloc(268, str_Fence_b, 0x20000000, 0);
        cFenceNode_cFenceNode2(fence, 1, obj, &FenceData, flag, force);
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
void cFenceNode::neighbourCollide(int on, float dt)
{
    setForce(dt);
    if (on) {
        if (NextPropTime == 0) {
            NextPropTime = 4;
        }
    } else {
        if (PrevPropTime == 0) {
            PrevPropTime = 4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_update);
#ifdef SKIP_ASM
extern "C" void cFenceNode_makeNeighbour(cFenceNode* self, tInstanceRecord* obj, int flag);
extern "C" void cFenceNode_updateGrid(cFenceNode* self);


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
            self->makeNeighbour((tInstanceRecord*)self->NextInstance, 1);
        }
    }
    if (self->PrevPropTime > 0) {
        if (--self->PrevPropTime == 0) {
            self->makeNeighbour((tInstanceRecord*)self->PrevInstance, 0);
        }
    }
    cFenceNodeState* state = (cFenceNodeState*)&self->FenceData; // the cFenceNodeState part (+0x1C)
    self->Force += self->ForceStep;
    if (self->ForceMax < 0.01f && fabs_34CE48(state->Force) < 0.01f) {
        self->expire(1);
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
    self->updateGrid();
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_updateGrid);
#ifdef SKIP_ASM
extern float cFenceNode_updateGrid_bendtable[];

struct sFsVec4_CF98 {
    float x, y, z, w;
    sFsVec4_CF98() {}
    sFsVec4_CF98(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sFsVec3_CF98 : t3Vector {
    sFsVec3_CF98() {}
    sFsVec3_CF98(float ax, float ay, float az) { x = ax; y = ay; z = az; }
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

static inline sFsVec3_CF98 fsAdd_CF98(const t3Vector& a, const sFsVec4_CF98& b)
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


/**
 * Bends a fence's grid mesh by its current force.
 *
 * Does nothing without a grid mesh (+0x40). The sway vector is (200, 0, 0) * Force (+0x2C) * Flexibility (+0x24); each
 * of the 8 columns gets it scaled by the weight table cFenceNode_updateGrid_bendtable. The top row (+0x44) always moves; the bottom row
 * (+0xA4) moves too when the fence Type (+0x20) is 1, otherwise it stays at rest. The 16 points go to the mesh's vtable
 * entry 3.
 *
 */
void cFenceNode::updateGrid()
{
    cFenceNode* self = this;
    if (((sFlagMesh_CF98*)self->GridMesh) == 0) {
        return;
    }
    float swing = self->Force * self->FenceData.Flexibility;
    sFsVec4_CF98 dir(200.0f, 0.0f, 0.0f, 0.0f);
    fsScaleIn_CF98(dir, swing);
    sFsVec4_CF98 offs[8];
    sFsVec4_CF98* offsetIter = offs;
    for (int i = 0; i < 8; i++) {
        *offsetIter++ = fsScale_CF98(dir, cFenceNode_updateGrid_bendtable[i]);
    }
    sFsVec3_CF98 pts[16];
    if (self->FenceData.Type == 1) {
        for (int i = 0; i < 8; i++) {
            pts[i] = fsAdd_CF98(self->Verts[i], offs[i]);
            pts[i + 8] = fsAdd_CF98(self->Verts[i + 8], offs[i]);
        }
    } else {
        for (int i = 0; i < 8; i++) {
            pts[i] = fsAdd_CF98(self->Verts[i], offs[i]);
            pts[i + 8] = static_cast<const sFsVec3_CF98&>(self->Verts[i + 8]); // the bottom row stays at rest
        }
    }
    sFlagMesh_CF98* gridMesh = ((sFlagMesh_CF98*)self->GridMesh);
    sFlagVEntry_CF98* setVertsSlot = &gridMesh->vt[3];
    setVertsSlot->fn(vthis(gridMesh, *setVertsSlot), pts);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_setupFence);
#ifdef SKIP_ASM
extern "C" void cFenceNode_updateGrid(cFenceNode* self);

// t3Vector / t2Vector / tARGB with the constructors the interpolation needs.
struct sFsV3_D1E8 : t3Vector {
    sFsV3_D1E8() {}
    sFsV3_D1E8(float ax, float ay, float az) { x = ax; y = ay; z = az; }
};

struct sFsV2_D1E8 : t2Vector {
    sFsV2_D1E8() {}
    sFsV2_D1E8(float ax, float ay) { x = ax; y = ay; }
};

struct sFsCol_D1E8 : tARGB {
    sFsCol_D1E8() {}
};

inline sFsV3_D1E8 operator+(const sFsV3_D1E8& a, const sFsV3_D1E8& b) { return sFsV3_D1E8(a.x + b.x, a.y + b.y, a.z + b.z); }
inline sFsV3_D1E8 operator-(const sFsV3_D1E8& a, const sFsV3_D1E8& b) { return sFsV3_D1E8(a.x - b.x, a.y - b.y, a.z - b.z); }
inline sFsV3_D1E8 operator*(const sFsV3_D1E8& a, float s) { return sFsV3_D1E8(a.x * s, a.y * s, a.z * s); }
inline sFsV2_D1E8 operator+(const sFsV2_D1E8& a, const sFsV2_D1E8& b) { return sFsV2_D1E8(a.x + b.x, a.y + b.y); }
inline sFsV2_D1E8 operator-(const sFsV2_D1E8& a, const sFsV2_D1E8& b) { return sFsV2_D1E8(a.x - b.x, a.y - b.y); }
inline sFsV2_D1E8 operator*(const sFsV2_D1E8& a, float s) { return sFsV2_D1E8(a.x * s, a.y * s); }

struct sFsMeshVE_D1E8 {
    short delta;
    short index;
    void (*fn)(void*, sFsV3_D1E8*, sFsCol_D1E8*, sFsV2_D1E8*);
};

struct sFsMesh_D1E8 {
    int pad0;
    sFsMeshVE_D1E8* vt;   // 0x4
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
 */
void cFenceNode::setupFence()
{
    cFenceNode* self = this;
    self->NextInstance = 0;
    self->PrevInstance = 0;
    self->Instance->m_iFlags = (self->Instance->m_iFlags & ~2u) | 4;
    cPS2GraphicsMan* mgr = cGraphicsMan_TheGraphicsMan;
    self->GridMesh = mgr->CreateGridMesh(8, 2, 0x60000000u);
    sFsV3_D1E8 pos[4];
    sFsV2_D1E8 quv[4];
    sFsCol_D1E8 qcol[4];
    mgr->GetMeshQuad(self->Instance, 0, pos, quv, qcol);
    sFsV2_D1E8 uv[16];
    sFsCol_D1E8 col[16];
    for (int i = 0; i < 8; i++) {
        float column = (float)i;
        self->Verts[i] = pos[0] + (pos[1] - pos[0]) * column * 0.1428571492433548f;
        self->Verts[i + 8] = pos[2] + (pos[3] - pos[2]) * column * 0.1428571492433548f;
        uv[i] = quv[0] + (quv[1] - quv[0]) * column * 0.1428571492433548f;
        uv[i + 8] = quv[2] + (quv[3] - quv[2]) * column * 0.1428571492433548f;
        col[i].A = 1.0f;
        col[i].R = qcol[0].R + (qcol[1].R - qcol[0].R) * column * 0.1428571492433548f;
        col[i].G = qcol[0].G + (qcol[1].G - qcol[0].G) * column * 0.1428571492433548f;
        col[i].B = qcol[0].B + (qcol[1].B - qcol[0].B) * column * 0.1428571492433548f;
        col[i + 8].A = 1.0f;
        col[i + 8].R = qcol[2].R + (qcol[3].R - qcol[2].R) * column * 0.1428571492433548f;
        col[i + 8].G = qcol[2].G + (qcol[3].G - qcol[2].G) * column * 0.1428571492433548f;
        col[i + 8].B = qcol[2].B + (qcol[3].B - qcol[2].B) * column * 0.1428571492433548f;
    }
    sFsMesh_D1E8* gridMesh = ((sFsMesh_D1E8*)self->GridMesh);
    gridMesh->vt[6].fn(vthis(gridMesh, gridMesh->vt[6]), pos, col, uv);
    self->updateGrid();
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_onCollide);
#ifdef SKIP_ASM
extern "C" void cFenceNode_setForce(cFenceNode* self, float dt);


// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float fsDot_D650(const t4Vector& a, const t4Vector& b)
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
    tCollideInfo* hitInfo = (tCollideInfo*)obj;
    if (self->ForceMax >= 1.0f) return;
    float dot = fsDot_D650(hitInfo->Reflect, hitInfo->Direct);
    self->setForce(dot * 0.0010000000474974513f * hitInfo->Vel);
    if (self->NextPropTime == 0) self->NextPropTime = 4;
    if (self->PrevPropTime == 0) self->PrevPropTime = 4;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_setForce);
#ifdef SKIP_ASM

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
 * @param force  force to add (sign gives the direction when at rest)
 */
void cFenceNode::setForce(float force)
{
    cFenceNode* self = this;
    self->ForceMax += fabs_34D6F0(force);
    if (self->ForceMax > 1.0f) {
        self->ForceMax = 1.0f;
    }
    if (self->ForceStep == 0.0f) {
        if (force < 0.0f) {
            self->ForceStep = self->ForceMax * -0.1f;
        } else {
            self->ForceStep = self->ForceMax * 0.1f;
        }
    } else if (self->ForceStep < 0.0f) {
        self->ForceStep = self->ForceMax * -0.1f;
    } else {
        self->ForceStep = self->ForceMax * 0.1f;
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
    if (!IsLoaded_34B9B0(self->Instance)) {
        return;
    }
    cPS2GraphicsMan* gm = cGraphicsMan_TheGraphicsMan;
    float one = 1.0f;
    {
        tInstanceRecord* model = self->Instance;
        tClipBoundBoxSlot* vt = (tClipBoundBoxSlot*)VPTR_OF(cGraphicsMan, gm);
        void* thisp = vthis(gm, vt[93]);
        t4VectorXYZW boxMin(model->m_fvMin.m[0], model->m_fvMin.m[1], model->m_fvMin.m[2], one);
        t4VectorXYZW boxMax(model->m_fvMax.m[0], model->m_fvMax.m[1], model->m_fvMax.m[2], one);
        t4Vector* pb = &boxMax;
        tNetMatrixSlot* vt43 = (tNetMatrixSlot*)VPTR_OF(cGraphicsMan, gm);
        t4Matrix cullInfo = vt43[43].fn(vthis(gm, vt43[43]));
        if (vt[93].fn(thisp, &boxMin, pb, &cullInfo) == 1) {
            return;
        }
    }
    gm->m_pCurrentDrawState[1] = gm->m_pCurrentDrawState[0];
    sDrawState* renderState = gm->m_pCurrentDrawState;
    gm->m_pCurrentDrawState = renderState + 1;
    // the pushed state's blend mode (bits 2-6 of its second word) = 5
    renderState[1].m_bitGroups[1] = ((int)renderState[1].m_bitGroups[1] & ~0x7C) | 0x14;
    tInstanceRecord* model = self->Instance;
    void* pos = &model->m_fmTransform;
    cGridMesh* gridMesh = self->GridMesh;
    if (gridMesh != 0) {
        tGridMeshRenderSlot* hv = (tGridMeshRenderSlot*)gridMesh->mVtbl;
        hv[7].fn(vthis(gridMesh, hv[7]), pos, model->m_MaterialBlock, 0);
    } else {
        tRenderModelSlot* vt96 = (tRenderModelSlot*)VPTR_OF(cGraphicsMan, gm);
        vt96[96].fn(vthis(gm, vt96[96]), model, 0, 0, 0);
    }
    gm->m_pCurrentDrawState--;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cFenceNode_writeToReplayFrame);
#ifdef SKIP_ASM

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a fence node to a replay frame: the instance-node part and its 0x24-byte swing state.
 *
 * @param stream  the replay frame (On Tour cBxReplayFrame* data)
 */
extern "C" void cFenceNode_writeToReplayFrame(cFenceNode* self, cBxReplayFrame* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    stream->write((const char*)&self->FenceData, 36);
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
extern void* cAnimNode_vtbl[];
extern char str_AnimMats[];

// t4Matrix with EA's empty constructor: the array new keeps only its element loop.
struct tAnimMat : t4Matrix {
    tAnimMat() {}
};

/**
 * Constructs the animation-node base of an animated object for a placed instance.
 *
 * Builds the cMoveNode part at +0x14 (cMoveNode_cMoveNode1) and installs cAnimNode_vtbl as its vtable. Allocates one 64-byte
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
extern "C" void* cAnimNode_cAnimNode(cAnimNode* self, void* bucket, int nodeType, tInstanceRecord* instRecord, int force)
{
    cMoveNode* node = (cMoveNode*)((char*)&self->MoveNodeBase);
    cMoveNode_cMoveNode1(node, bucket, nodeType, instRecord);
    self->mVtbl = cAnimNode_vtbl;
    t4Matrix** slot = &self->AnimMats;
    *slot = new (str_AnimMats, 0x20000000, 0) tAnimMat[node->Instance->m_Model->m_iNumMeshes];
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
    cMoveNode* sub = (cMoveNode*)((char*)&self->MoveNodeBase);
    if (force != 0 || ((unsigned int)sub->Instance->m_iFlags & 1)) {
        tInstanceRecord* n = (tInstanceRecord*)sub->Instance;
        n->m_iFlags = ((unsigned int)n->m_iFlags & ~2u) | 4;
    }
    self->cacheFrame();
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode_cAnimNode1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cMoveNode_cMoveNode_3(void* self, void* a1, void* stream) __asm__("cMoveNode_cMoveNode");


/**
 * Constructs the animation-node base of an animated object from a replay frame.
 *
 * Runs the cMoveNode replay constructor on +0x14, installs cAnimNode_vtbl, allocates AnimMats like cAnimNode_cAnimNode, builds
 * the mesh cache, reads the 20-byte frame block at +0x0 and one more 4-byte word that it discards, then
 * cAnimNode_cacheFrame.
 *
 * @param bucket  update bucket
 * @param stream  replay stream to read from
 * @return self
 */
void* cAnimNode::cAnimNode1(void* bucket, cBxReplayFrame* stream)
{
    int skippedWord;
    cMoveNode* node = (cMoveNode*)&MoveNodeBase;
    t4Matrix** slot = &AnimMats;
    cMoveNode_cMoveNode_3(node, bucket, stream);
    mVtbl = cAnimNode_vtbl;
    // The element type has an empty constructor (tAnimMat, cAnimNode_cAnimNode); the array new keeps its loop.
    *slot = new (str_AnimMats, 0x20000000, 0) tAnimMat[node->Instance->m_Model->m_iNumMeshes];
    cAnimNode_setAnimMeshCache(((char*)this));
    stream->read((char*)this, 20);
    stream->read((char*)&skippedWord, 4);
    cacheFrame();
    return ((char*)this);
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode__cAnimNode);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);
extern "C" void cMoveNode__cMoveNode(void* self, int flags);
extern void* cAnimNode_vtbl[];

/**
 * Destroys the animation-node base of an animated object.
 *
 * Reinstalls cAnimNode_vtbl, frees AnimMats (+0x44), runs each AnimFrames element's destructor (+0x48, vtable entry 1, in
 * reverse) and frees the array, frees +0x4C, destroys the cMoveNode part (cMoveNode__cMoveNode) and deletes the object
 * when bit 0 of flags is set.
 *
 * @param flags  destructor flags; bit 0 deletes the storage
 */
extern "C" void cAnimNode__cAnimNode(cAnimNode* self, int flags)
{
    self->mVtbl = cAnimNode_vtbl;
    if (self->AnimMats != 0) {
        cMemMan_free(self->AnimMats);
    }
    // ORIGINAL-PTR: delete[] AnimFrames as g++ 2.95 inlines it: the element count sits in the 16-byte new[] cookie
    // before the array (a heap block header), and the block itself starts at the cookie
    cMeshAnimFrame* items = self->AnimFrames;
    if (items != 0) {
        cMeshAnimFrame* frameIter = items + ((int*)items)[-4];
        while (self->AnimFrames != frameIter) {
            frameIter--;
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)frameIter->mVtbl;
            vt[1].fn(vthis(frameIter, vt[1]), 0);
        }
        cMemMan_free((char*)self->AnimFrames - 0x10);
    }
    if (self->MeshVelocity != 0) {
        cMemMan_free(self->MeshVelocity);
    }
    cMoveNode__cMoveNode(((char*)&self->MoveNodeBase), 0);
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
void cAnimNode::updateRootTransform()
{
    sFlagSetVEntry_DC90a* vt = (sFlagSetVEntry_DC90a*)mVtbl;
    vt[52].fn(vthis(this, vt[52]));
    char* sub = (char*)&MoveNodeBase;
    sFlagSetVEntry_DC90c* vt2 = (sFlagSetVEntry_DC90c*)mVtbl;
    sFlagSetVEntry_DC90b* vtGet = (sFlagSetVEntry_DC90b*)vt2;
    void* objB = vthis(sub, vt2[33]);
    void* rootTransform = vtGet[24].fn(vthis(sub, vt2[24]));
    vt2[33].fn(objB, rootTransform, (int)AnimMats);
    Flags &= ~1;
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
                out[i] = mulScaledFS(out[bone->parent], *(sFsMtx_DD18*)self->getFrameMat(k++), scale);
            } else {
                out[i] = mulScaledFS(*root, *(sFsMtx_DD18*)self->getFrameMat(k++), scale);
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
extern t4Matrix t4mat_Identity;

/**
 * Returns the frame matrix of one animated mesh, or the identity matrix when there are no anim frames.
 *
 * @param i  animated-mesh index
 * @return AnimFrames[i] +0x90 (0xD0-byte frames at +0x48), or t4mat_Identity
 */
void* cAnimNode::getFrameMat(int i)
{
    char* base = (char*)AnimFrames;
    if (base == 0) {
        return &t4mat_Identity;
    }
    return base + i * 208 + 0x90;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode_cacheFrame);
#ifdef SKIP_ASM


struct sFlagOwner_E348 {
    int field_0x0;
    float time;                 // 0x4
    char pad8[0x8];
    int dirty;                  // 0x10
    char pad14[0x2C];
    int count;                  // 0x40
    int field_0x44;
    cMeshAnimFrame* items;      // 0x48
};

/**
 * Brings every animated mesh's frame to the node's cache time and marks the mesh velocities stale.
 *
 * For each of NumAnimMeshes (+0x40) entries of AnimFrames (+0x48, 0xD0 bytes each) it calls the frame's virtual slot
 * 2 with CacheTime (+0x4); then sets MeshVelocityDirty (+0x10) so cAnimNode_updateMeshVelocity recomputes the velocities on demand.
 * Slot 52 of cAnimNode's vtable.
 */
void cAnimNode::cacheFrame()
{
    int i;
    if (AnimFrames != 0) {
        for (i = 0; i < NumAnimMeshes; i++) {
            // PORT: pointer arithmetic done in int (gives the target's offset-first addu).
            cMeshAnimFrame* animFrame = (cMeshAnimFrame*)(i * 0xD0 + (int)AnimFrames);
            sVEntry<void(void*, float)>* vt = (sVEntry<void(void*, float)>*)animFrame->mVtbl;
            vt[2].fn(vthis(animFrame, vt[2]), CacheTime);
        }
    }
    MeshVelocityDirty = 1;
}
#endif

//100%
INCLUDE_ASM("object/flagset", cAnimNode_writeToReplayFrame);
#ifdef SKIP_ASM

extern "C" void cMoveNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes the animation-node base of an animated object to a replay frame.
 *
 * Writes the cMoveNode part (cMoveNode_writeToReplayFrame on +0x14), then the 20-byte frame block at +0x0 and a 4-byte
 * stack word that is never set (the replay constructor reads and drops it).
 *
 * @param stream  replay stream to write to
 */
void cAnimNode::writeToReplayFrame(cBxReplayFrame* stream)
{
    int dummy;
    cMoveNode_writeToReplayFrame(&MoveNodeBase, stream);
    stream->write((const char*)this, 20);
    stream->write((const char*)&dummy, 4);
}
#endif

