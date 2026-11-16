#include "common.h"
#include "sound/ssxaudio.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "main/sectionman.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"

// 16-byte array elements; indexing as arr[i].field (rather than manual
// pointer arithmetic) is what makes GCC emit the target's base-first addu
struct sGameEntry {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
};
extern sGameEntry cSectionMan_mSectionMap[];

//100%
INCLUDE_ASM("main/game", cGame_renderModels);
#ifdef SKIP_ASM

struct sRVEntry_C078 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int, int);
};

struct sRCtx_C078 {
    char pad0[0x24];
    int cur;
    char pad28[0xE84 - 0x28];
    sDrawState* top;
    char padE88[0xF50 - 0xE88];
    int tbl[(0x10D8 - 0xF50) / 4];
    sRVEntry_C078* vt;
};


static inline int curLayer_C078(sRCtx_C078* c)
{
    return c->tbl[c->cur];
}

/**
 * Draws the models found visible by the world's frustum query, the second list with render flag 0x20 added.
 *
 * Sets the draw-state layer and blend bits first; the flags passed to GrMan's model draw (vtable slot 0x60)
 * are
 * 0xC00, or 0x400 with two or more views.
 *
 * @param world  the frustum query results (On Tour const FrustumIntersector& fi)
 */
extern "C" void cGame_renderModels(cGame* self, void* world)
{
    *(short*)((char*)GrMan->m_pCurrentDrawState + 0x12) = curLayer_C078((sRCtx_C078*)GrMan);
    cPS2GraphicsMan* ctx = GrMan;
    (ctx->m_pCurrentDrawState)->unk_00[1] = (((int)(ctx->m_pCurrentDrawState)->unk_00[1]) & ~0xF80) | 0x880;
    (*(int*)&(ctx->m_pCurrentDrawState)->unk_00[0]) |= 0x30;
    int flag = 0xC00;
    if (*(unsigned int*)((char*)self->ViewMan + 0x10) >= 2) {
        flag = 0x400;
    }
    int n = *(int*)((char*)world + 0x209C);
    int* items = (int*)((char*)world + 0x20A0);
    for (int i = 0; i < n; i++) {
        ((sRVEntry_C078*)GrMan->mVtbl)[0x60].fn((char*)GrMan + ((sRVEntry_C078*)GrMan->mVtbl)[0x60].delta, items[i], 0, 0, flag);
    }
    int secondCount = *(int*)((char*)world + 0x40A0);
    items = (int*)((char*)world + 0x40A4);
    for (int i = 0; i < secondCount; i++) {
        ((sRVEntry_C078*)GrMan->mVtbl)[0x60].fn((char*)GrMan + ((sRVEntry_C078*)GrMan->mVtbl)[0x60].delta, items[i], 0, 0, flag | 0x20);
    }
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022C1B0);
#ifdef SKIP_ASM
extern void* cBezierMan_TheBezierMan;

struct sVec_C1B0 {
    float x, y, z, w;
    sVec_C1B0() {}
    sVec_C1B0(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sMat_C1B0 {
    float m[4][4];
};

struct sVEm_C1B0 { short delta; short index; sMat_C1B0* (*fn)(void*); };
struct sVEf_C1B0 { short delta; short index; float (*fn)(void*); };
struct sVEv_C1B0 { short delta; short index; void (*fn)(void*); };

struct sRCtx_C1B0 {
    char pad0[0x10D8];
    sVEm_C1B0* vt;      // 0x10D8
};

struct sCamObj_C1B0 {
    char pad0[4];
    sVEf_C1B0* vt;      // 0x4
};

struct sRenderCam_C1B0 {
    sCamObj_C1B0* cam;  // 0x0
    char pad4[0xC];
    sVec_C1B0 pos;      // 0x10
    float k;            // 0x20
    float c;            // 0x24
    float n;            // 0x28
    float f;            // 0x2C
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_C1B0 Scale_C1B0(const sVec_C1B0& v, float s)
{
    sVec_C1B0 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

static inline sVec_C1B0 MulVec_C1B0(const sMat_C1B0* m, const sVec_C1B0& v)
{
    return sVec_C1B0(v.x * m->m[0][0] + v.y * m->m[0][1] + v.z * m->m[0][2] + v.w * m->m[0][3],
                     v.x * m->m[1][0] + v.y * m->m[1][1] + v.z * m->m[1][2] + v.w * m->m[1][3],
                     v.x * m->m[2][0] + v.y * m->m[2][1] + v.z * m->m[2][2] + v.w * m->m[2][3],
                     v.x * m->m[3][0] + v.y * m->m[3][1] + v.z * m->m[3][2] + v.w * m->m[3][3]);
}

static inline sMat_C1B0* viewMat_C1B0()
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    return ((sVEm_C1B0*)ps2GraphicsMan->mVtbl)[35].fn((char*)ps2GraphicsMan + ((sVEm_C1B0*)ps2GraphicsMan->mVtbl)[35].delta);
}

// PORT: the unit declares func_0022C1B0 as void(void*); the body returns self, bound by asm label.
sRenderCam_C1B0* func_0022C1B0_impl(sRenderCam_C1B0* self) __asm__("func_0022C1B0");

/**
 * Sets up the 0x30-byte context cGame_renderPatches uses to draw terrain patches, and starts a patch batch.
 *
 * Stores cBezierMan_TheBezierMan at +0x0; computes the eye position (+0x10) from GrMan's view matrix (vtable slot 35):
 * the negated translation row, multiplied by the matrix, with w = 1. Sets the level range n = 1 (+0x28) and f = 3
 * (+0x2C), reads two distances from the bezier manager (vtable slots 8 and 9), and stores k (+0x20) and c (+0x24) so
 * that c + k * d maps a summed squared distance d of 2 * near^2 to f and 2 * far^2 to n. Finally calls slot 17 of the
 * bezier manager.
 *
 * @return @p self
 */
sRenderCam_C1B0* func_0022C1B0_impl(sRenderCam_C1B0* self)
{
    self->cam = (sCamObj_C1B0*)cBezierMan_TheBezierMan;
    self->pos = Scale_C1B0(sVec_C1B0(*(sVec_C1B0*)viewMat_C1B0()->m[3]), -1.0f);
    sMat_C1B0* viewMatrix = viewMat_C1B0();
    self->pos = sVec_C1B0(self->pos.x * viewMatrix->m[0][0] + self->pos.y * viewMatrix->m[0][1] + self->pos.z * viewMatrix->m[0][2] + self->pos.w * viewMatrix->m[0][3],
                          self->pos.x * viewMatrix->m[1][0] + self->pos.y * viewMatrix->m[1][1] + self->pos.z * viewMatrix->m[1][2] + self->pos.w * viewMatrix->m[1][3],
                          self->pos.x * viewMatrix->m[2][0] + self->pos.y * viewMatrix->m[2][1] + self->pos.z * viewMatrix->m[2][2] + self->pos.w * viewMatrix->m[2][3],
                          self->pos.x * viewMatrix->m[3][0] + self->pos.y * viewMatrix->m[3][1] + self->pos.z * viewMatrix->m[3][2] + self->pos.w * viewMatrix->m[3][3]);
    self->n = 1.0f;
    self->f = 3.0f;
    self->pos.w = 1.0f;
    sCamObj_C1B0* bezierMan = self->cam;
    float nearLod = bezierMan->vt[8].fn((char*)bezierMan + bezierMan->vt[8].delta);
    bezierMan = self->cam;
    float farLod = bezierMan->vt[9].fn((char*)bezierMan + bezierMan->vt[9].delta);
    nearLod = nearLod * nearLod * 2.0f;
    farLod = farLod * farLod * 2.0f;
    self->k = (self->n - self->f) / (farLod - nearLod);
    self->c = self->f - self->k * nearLod;
    bezierMan = self->cam;
    ((sVEv_C1B0*)bezierMan->vt)[17].fn((char*)bezierMan + bezierMan->vt[17].delta);
    return self;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022C3B8);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
struct sVEntry_func_0022C3B8 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Ends a patch-render context: calls slot 18 of its bezier manager (+0x0), then frees the context when bit 0
 * of @p flags is set.
 *
 * cGame_renderPatches calls it with flags 2 on its stack context after drawing.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the context
 */
extern "C" void func_0022C3B8(void* self, int flags)
{
    void* obj = *(void**)self;
    sVEntry_func_0022C3B8* vt = *(sVEntry_func_0022C3B8**)((char*)obj + 0x4);
    vt[18].fn((char*)obj + vt[18].delta);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022C410);
#ifdef SKIP_ASM
// NOTE: D_004A2A10 is a 4-byte const fog table ({4, 4, 6, 8}) in small data. It must be const: its loads then don't
// conflict with the stack stores of ids[], so the scheduler places the ids[0] store before them as retail does.
extern const unsigned char D_004A2A10[4];

struct sVEp_C410 { short delta; short index; void (*fn)(void*, void*, unsigned char*, int*, int); };
struct sCamObj_C410 {
    char pad0[4];
    sVEp_C410* vt;      // 0x4
};
struct sRenderCam_C410 {
    sCamObj_C410* cam;  // 0x0
    char pad4[0xC];
    float px, py, pz, pw;   // 0x10
    float k;            // 0x20
    float c;            // 0x24
    float n;            // 0x28
    float f;            // 0x2C
};
struct sPatchVtx_C410 {
    float x, y, z;
};
struct sPatch_C410 {
    char pad0[0x170];
    sPatchVtx_C410 v[4];    // 0x170
    short id0;              // 0x1A0
    short id1;              // 0x1A2
};

static inline float distSq_C410(const sRenderCam_C410* ctx, const sPatch_C410* p, int i)
{
    float dx = ctx->px - p->v[i].x;
    float dy = ctx->py - p->v[i].y;
    float dz = ctx->pz - p->v[i].z;
    return dx * dx + dy * dy + dz * dz;
}

// PORT: g++ `<?` (min) operator.
static inline float clamp_C410(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

// PORT: the unit declares func_0022C410(void*, int, int) for its callers; this definition takes the
// patch as a typed pointer, so it binds to the same symbol through an asm label.
extern "C" void func_0022C410_impl(void* vctx, sPatch_C410* patch, int flag) __asm__("func_0022C410");
/**
 * Submits one terrain patch to the bezier manager with a level per edge, from the eye's distance to its corners.
 *
 * Takes the squared distances from the context's eye position to the patch's four corners (+0x170, 12 bytes each), sums
 * them per edge (corners 0-1, 2-3, 0-2, 1-3), maps each sum to c + k * sum clamped to [n, f] (the values func_0022C1B0
 * set up), and converts each level to a byte through the table D_004A2A10 ({4, 4, 6, 8}). Then calls slot 21 of the
 * context's bezier manager with the patch, the four bytes, the patch's two ids (+0x1A0, +0x1A2, as ints) and @p flag.
 * cGame_renderPatches calls it for each patch of its two lists, with flag 0 and 1.
 *
 * @param vctx  the patch-render context (func_0022C1B0)
 * @param patch  the terrain patch
 * @param flag  the list pass (0 or 1), passed on to the bezier manager
 */
void func_0022C410_impl(void* vctx, sPatch_C410* patch, int flag)
{
    sRenderCam_C410* ctx = (sRenderCam_C410*)vctx;
    float lo = ctx->n;
    float dx, dy, dz;
    dx = ctx->px - patch->v[0].x;
    dy = ctx->py - patch->v[0].y;
    dz = ctx->pz - patch->v[0].z;
    float d0 = dx * dx + dy * dy + dz * dz;
    dx = ctx->px - patch->v[1].x;
    dy = ctx->py - patch->v[1].y;
    dz = ctx->pz - patch->v[1].z;
    float d1 = dx * dx + dy * dy + dz * dz;
    dx = ctx->px - patch->v[2].x;
    dy = ctx->py - patch->v[2].y;
    dz = ctx->pz - patch->v[2].z;
    float d2 = dx * dx + dy * dy + dz * dz;
    dx = ctx->px - patch->v[3].x;
    dy = ctx->py - patch->v[3].y;
    dz = ctx->pz - patch->v[3].z;
    float d3 = dx * dx + dy * dy + dz * dz;
    float e01 = ctx->c + ctx->k * (d0 + d1);
    float e23 = ctx->c + ctx->k * (d2 + d3);
    float e02 = ctx->c + ctx->k * (d0 + d2);
    float e13 = ctx->c + ctx->k * (d1 + d3);
    float hi = ctx->f;
    e01 = clamp_C410(e01, lo, hi);
    e23 = clamp_C410(e23, lo, hi);
    e02 = clamp_C410(e02, lo, hi);
    e13 = clamp_C410(e13, lo, hi);
    unsigned char fog[4];
    int ids[2];
    unsigned char b01 = D_004A2A10[(int)e01];
    unsigned char b23 = D_004A2A10[(int)e23];
    unsigned char b02 = D_004A2A10[(int)e02];
    unsigned char b13 = D_004A2A10[(int)e13];
    fog[0] = b01;
    fog[1] = b23;
    fog[2] = b02;
    fog[3] = b13;
    ids[0] = patch->id0;
    ids[1] = patch->id1;
    sCamObj_C410* o = ctx->cam;
    o->vt[21].fn((char*)o + o->vt[21].delta, patch, fog, ids, flag);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/game", cGame_renderPatches);
#ifdef SKIP_ASM
extern "C" void func_0022C1B0(void* ctx);
extern "C" void func_0022C410(void* ctx, int patch, int flag);
extern "C" void func_0022C3B8(void* self, int flags);
extern void* cBezierMan_TheBezierMan;

struct sVEntry0022C620 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Draws the visible terrain patches of the frustum query (two lists, passes 0 and 1), then flushes the patch
 * batch.
 *
 * With no second view (ViewMan + 0x14 is 0) it also calls slot 5 of cBezierMan_TheBezierMan before the flush.
 *
 * @param world  the frustum query results (On Tour const FrustumIntersector& fi)
 */
extern "C" void cGame_renderPatches(cGame* self, void* world)
{
    char ctx[0x30];
    func_0022C1B0(ctx);
    int n = *(int*)((char*)world + 0x50A8);
    int* items = (int*)((char*)world + 0x50AC);
    for (int i = 0; i < n; i++) {
        func_0022C410(ctx, items[i], 0);
    }
    n = *(int*)((char*)world + 0x70AC);
    items = (int*)((char*)world + 0x70B0);
    for (int i = 0; i < n; i++) {
        func_0022C410(ctx, items[i], 1);
    }
    if (*(int*)((char*)self->ViewMan + 0x14) == 0) {
        void* obj = cBezierMan_TheBezierMan;
        sVEntry0022C620* vt = *(sVEntry0022C620**)((char*)obj + 4);
        vt[5].fn((char*)obj + vt[5].delta);
    }
    func_0022C3B8(ctx, 2);
}
#endif

//100%
INCLUDE_ASM("main/game", cGame_renderFogVolumes);
#ifdef SKIP_ASM
struct sVEntry_renderFog {
    short delta;
    short index;
    void (*fn)(void*, void*, int, int);
};
extern "C" void cFogParticleMan_render(void* r);

/**
 * Hands the frustum query's two fog-volume lists (types 0 and 2) to the fog manager, then draws them.
 *
 * @param world  the frustum query results (On Tour const FrustumIntersector& fi)
 */
extern "C" void cGame_renderFogVolumes(cGame* self, void* world)
{
    void* fog = (char*)world + 0x78B8;
    int n = *(int*)((char*)world + 0x78B4);
    if (n != 0) {
        void* fogMan = self->FogMan;
        sVEntry_renderFog* vt = *(sVEntry_renderFog**)((char*)fogMan + 0x4);
        vt[2].fn((char*)fogMan + vt[2].delta, fog, n, 0);
    }
    fog = (char*)world + 0x7AC0;
    n = *(int*)((char*)world + 0x7ABC);
    if (n != 0) {
        void* fogMan = self->FogMan;
        sVEntry_renderFog* vt = *(sVEntry_renderFog**)((char*)fogMan + 0x4);
        vt[2].fn((char*)fogMan + vt[2].delta, fog, n, 2);
    }
    cFogParticleMan_render(self->FogMan);
}
#endif

//100%
INCLUDE_ASM("main/game", cGame_renderLightHalos);
#ifdef SKIP_ASM
void func_002E2F98(void* self, int a1);
extern "C" void func_002E2FF8(void* self, void* list, int n, int mode, int a4);
extern "C" void func_002E30D0(void* self, int a1);

/**
 * Draws the light halos of the frustum query's two light lists (types 0 and 2) for one view through the
 * lens-FX manager.
 *
 * @param world  the frustum query results (On Tour const FrustumIntersector& fi)
 * @param ViewID  the view being drawn
 */
extern "C" void cGame_renderLightHalos(cGame* self, void* world, int ViewID)
{
    *(int*)((char*)self->LensFxMan + 0x6494) = 1;
    func_002E2F98(self->LensFxMan, ViewID);
    void* halos = (char*)world + 0x7BC4;
    int n = *(int*)((char*)world + 0x7BC0);
    if (n != 0) {
        func_002E2FF8(self->LensFxMan, halos, n, 0, ViewID);
    }
    halos = (char*)world + 0x7FC8;
    n = *(int*)((char*)world + 0x7FC4);
    if (n != 0) {
        func_002E2FF8(self->LensFxMan, halos, n, 2, ViewID);
    }
    func_002E30D0(self->LensFxMan, ViewID);
    *(int*)((char*)self->LensFxMan + 0x6494) = 0;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022C830);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the save / zero / restore of cDepthFog_m_Enable through a pointer local `p` (keeps the store ahead of the GrMan reload), and `pfn`, a pointer to vtable slot 23's fn field taken before the arguments; plain form 97.19% (97.20% / 99.99% without each alone). Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sV2_C830 {
    float x, y;
    sV2_C830() {}
    sV2_C830(const float& ax, const float& ay) : x(ax), y(ay) {}
};
struct sV3_C830 {
    float x, y, z;
    sV3_C830(const float& ax, const float& ay, const float& az) : x(ax), y(ay), z(az) {}
};
struct sVec4_C830 {
    float x, y, z, w;
    sVec4_C830() {}
    sVec4_C830(const float& ax, const float& ay, const float& az, const float& aw) : x(ax), y(ay), z(az), w(aw) {}
};

struct sGs_C830 {
    int w0;
    int w4;
    int w8;
    int wC;
    unsigned int f10 : 16;
    unsigned int f12 : 16;
};

class cGfx_C830 {
public:
    char pad0[0x24];
    int cur;
    char pad28[0xE84 - 0x28];
    sGs_C830* stateTop;
    char padE88[0xF50 - 0xE88];
    int layer[(0x10D8 - 0xF50) / 4];
    int GetLayer() { return layer[cur]; }
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8();
    virtual unsigned int GetWidth();
    virtual unsigned int GetHeight();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(const sV3_C830&);
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19();
    virtual void BeginScene();
    virtual void EndScene();
    virtual void v22(float, float, float, float, int);
    virtual void v23(void*, int);
    virtual void v24(float, float, float);
    virtual void SetViewport(float, float, float, float, int, int, float, float);
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void Begin2D();
    virtual void End2D();
    virtual void v32();
    virtual void SetTexture(void*);
};


static inline void SetBits_C830(int* p, int mask, int bits) { *p = (*p & ~mask) | bits; }

static inline void Set1_C830(cGfx_C830* g, int mask, int shift, int v)
{
    g->stateTop->w4 = (g->stateTop->w4 & ~mask) | ((v << shift) & mask);
}

static inline void Set2_C830(cGfx_C830* g, int mask, int shift, int v)
{
    g->stateTop->w8 = (g->stateTop->w8 & ~mask) | ((v << shift) & mask);
}

struct sVE_C830 {
    short delta;
    short index;
    void (*fn)(void*, float, float, float, float, int);
};

struct sCam_C830 {
    float x, y, z;
    char padC[0x40 - 0xC];
    char tex[1];
};

struct sView_C830 {
    float x, y, w, h;
};

struct sViews_C830 {
    int f0;
    sCam_C830* cams[3];
    unsigned int count;
    int cur;
    sView_C830 views[1];
};

struct sFont_C830 {
    char pad0[0x28];
    sV2_C830 scale;     // 0x28
    float f30;          // 0x30
    float f34;          // 0x34
    float f38;          // 0x38
    float f3C;          // 0x3C
    sVec4_C830 color;   // 0x40
    void SetScale(float sx, float sy) { f38 = f30 * sx; f3C = f34 * sy; }
};

struct sGame_C830 {
    char pad0[0x24];
    sFont_C830* font;       // 0x24
    char pad28[0x48 - 0x28];
    void* f48;              // 0x48
    char pad4C[0x84 - 0x4C];
    sViews_C830* views;     // 0x84
    char pad88[0x94 - 0x88];
    void* hudTimer;         // 0x94
    char pad98[0xA4 - 0x98];
    void* fA4;              // 0xA4
    char padA8[0xAC - 0xA8];
    int frames;             // 0xAC
};

extern char gBucketMan[4];
extern void* gWorldTriggerManager;
extern char t4mat_Identity[];
extern int cDepthFog_m_Enable;
extern "C" void cBucketMan_renderBucket(void* self, int index);
void func_002789C0(void* p);
extern "C" void func_00397DF8(void* p);
extern "C" void func_001F31E0(void* p);
extern "C" int cMenuSys_isActive(void* p);
extern "C" void func_002CC350(void* p);
extern "C" void func_00285BF8(void* self, int a, float dt);
extern "C" void func_002B5D78(void* p);

/**
 * Finishes a game frame: draws each view's bucket 3, then the full-screen 2D overlays, and runs the per-frame audio and
 * trigger updates.
 *
 * For each view of ViewMan (+0x84, count at +0x10) it selects the view, sets GrMan's viewport (slot 23) and camera
 * position (slot 24), loads the camera's matrix, sets the layer and several GS state bits, and calls
 * cBucketMan_renderBucket(gBucketMan, 3). Then it resets the viewport to the full screen (slot 22 with GrMan's width
 * and height, then SetViewport 640x480), loads t4mat_Identity, sets the debug font (+0x24) to scale 0.8, size 2 and
 * colour (1, 0.8, 0.8, 0), and draws the script engine (func_002789C0(ScriptEngine)), the UI (func_00397DF8 on +0x48),
 * the load indicator (func_001F31E0 on +0x94) and, when cMenuSys_isActive, the debug menu system (func_002CC350 on
 * +0xA4). It calls GrMan slot 14 with a zero vector and slot 19 with cDepthFog_m_Enable cleared for the call, then
 * advances the audio by mUpdateTick / 60 seconds (func_00285BF8(gSSXAudio, 1, ..)), runs
 * func_002B5D78(gWorldTriggerManager) and zeroes mUpdateTick (+0xAC). cGame_render calls it at the end of each of its
 * three render paths.
 */
extern "C" void func_0022C830(sGame_C830* self)
{
    unsigned int i = 0;
    if (self->views->count != 0) {
    do {
        self->views->cur = i;
        sCam_C830* cam = self->views->cams[self->views->cur];
        ((cGfx_C830*)GrMan)->v23(&self->views->views[i], 1);
        ((cGfx_C830*)GrMan)->v24(cam->x, cam->y, cam->z);
        ((cGfx_C830*)GrMan)->SetTexture(cam->tex);
        *(short*)((char*)((cGfx_C830*)GrMan)->stateTop + 0x12) = ((cGfx_C830*)GrMan)->GetLayer();
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        Set1_C830((cGfx_C830*)ps2GraphicsMan, 0xF80, 7, 7);
        SetBits_C830(&((cGfx_C830*)ps2GraphicsMan)->stateTop->w0, 0x30, 0x30);
        Set1_C830((cGfx_C830*)ps2GraphicsMan, 0x1800000, 23, 1);
        SetBits_C830(&((cGfx_C830*)ps2GraphicsMan)->stateTop->w4, 0x300000, 0x300000);
        Set1_C830((cGfx_C830*)ps2GraphicsMan, 0xFF000, 12, 13);
        Set2_C830((cGfx_C830*)ps2GraphicsMan, 0x3E0, 5, 4);
        cBucketMan_renderBucket(gBucketMan, 3);
        i++;
    } while (i < self->views->count);
    }
    {
        sVE_C830* vt = (sVE_C830*)GrMan->mVtbl;
        char* thisp = (char*)GrMan + vt[23].delta;
        void (**pfn)(void*, float, float, float, float, int) = &vt[23].fn;
        (*pfn)(thisp, 0.0f, 0.0f, (float)((cGfx_C830*)GrMan)->GetWidth(), (float)((cGfx_C830*)GrMan)->GetHeight(), 1);
    }
    ((cGfx_C830*)GrMan)->SetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0, 0, 0.0f, 1.0f);
    ((cGfx_C830*)GrMan)->SetTexture(t4mat_Identity);
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    Set2_C830((cGfx_C830*)ps2GraphicsMan, 0x3E0, 5, 0x18);
    Set1_C830((cGfx_C830*)ps2GraphicsMan, 0x7C, 2, 5);
    self->font->SetScale(0.8f, 0.8f);
    self->font->scale = sV2_C830(2.0f, 2.0f);
    self->font->color = sVec4_C830(1.0f, 0.8f, 0.8f, 0.0f);
    func_002789C0(ScriptEngine);
    func_00397DF8(self->f48);
    func_001F31E0(self->hudTimer);
    if (cMenuSys_isActive(self->fA4) != 0) {
        func_002CC350(self->fA4);
        self->font->scale = sV2_C830(2.0f, 2.0f);
        self->font->color = sVec4_C830(1.0f, 0.8f, 0.8f, 0.0f);
    }
    ((cGfx_C830*)GrMan)->v14(sV3_C830(0.0f, 0.0f, 0.0f));
    // Storing through a pointer keeps the store ahead of the reload of GrMan.
    int* p = &cDepthFog_m_Enable;
    int saved = *p;
    *p = 0;
    ((cGfx_C830*)GrMan)->v19();
    *p = saved;
    func_00285BF8(gSSXAudio, 1, (float)self->frames * (1.0f / 60.0f));
    func_002B5D78(gWorldTriggerManager);
    self->frames = 0;
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_cSectionMan);
#ifdef SKIP_ASM
extern "C" void cSectionMan_init(cSectionMan* self);

/**
 * Constructs the section manager for a game and initializes it.
 *
 * @param Game  the game (cGame*) that owns it
 * @return @p self
 */
extern "C" void* cSectionMan_cSectionMan(cSectionMan* self, int Game)
{
    self->mpGame = (cGame*)Game;
    cSectionMan_init(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan__cSectionMan);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys the section manager; it owns nothing, so this only frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cSectionMan__cSectionMan(cSectionMan* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
struct cWorldView;
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cWorldView_getNumSections(cWorldView* view);
extern "C" void* cWorldView_getSectionName(cWorldView* self, unsigned int i);
extern "C" int func_004165A8(const void* a, const void* b);
extern "C" void* cBENewRaceInterface_getSectionFile(void* iface, int i);
extern "C" void cSectionMan_initEventPool(void* self);

static inline cWorldView* gameWorldView_22CD40(void* self)
{
    return (cWorldView*)(*(char**)*(char**)(*(char**)((char*)self + 0x1B0) + 0x10) + 0x10);
}

/**
 * Resets the section manager: maps each back-end section to its world section by name and clears all state.
 *
 * All 50 sections start empty; section 49 maps to world section 0 unless a name matches. The location starts
 * at
 * 0x16, streaming is ready and the event pool is rebuilt.
 */
extern "C" void cSectionMan_init(cSectionMan* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    int i;
    unsigned int j;

    for (int k = 0; k < 50; k++) {
        cSectionMan_mSectionMap[k].field_0x4 = -1;
        cSectionMan_mSectionMap[k].field_0x8 = 0;
    }
    cSectionMan_mSectionMap[49].field_0x4 = 0;
    for (i = 0; i < 50; i++) {
        for (j = 0; j < (unsigned int)cWorldView_getNumSections(gameWorldView_22CD40(self)); j++) {
            void* name = cBENewRaceInterface_getSectionFile(iface, i);
            if (func_004165A8(name, cWorldView_getSectionName(gameWorldView_22CD40(self), j)) == 0) {
                cSectionMan_mSectionMap[i].field_0x4 = j;
            }
        }
    }
    self->mMode = 0;
    self->mModeState = 0;
    self->mbLocationIsActivated = 0;
    self->mWaitTransport = 0;
    self->mModeReset = 0;
    self->mBELocationID = 22;
    self->m_bStreamReady = 1;
    cSectionMan_initEventPool(self);
    self->m_iNextSkySectionID = -1;
    self->m_bTriggerSkySwap = 0;
    self->m_bSkySwapPending = 0;
    self->mStreamRate = 0;
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_load);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sGroup_CEA8 {
    int f0;
    int count;      // 0x4
    int a;          // 0x8
    int b;          // 0xC
    int ids[6];     // 0x10
};
struct sMode_CEA8 {
    int f0;
    int mode;       // 0x4
};
extern sGroup_CEA8 D_00442488_CEA8[] __asm__("D_00442488");
extern sMode_CEA8 D_00442820_CEA8[] __asm__("D_00442820");
extern "C" void cSectionMan_unloadSection(cSectionMan* self, int idx);
extern "C" void cSectionMan_loadSection(cSectionMan* self, int idx);
extern "C" void cSectionMan_loadMode(cSectionMan* self, int a1);

// PORT: the unit's other callers declare (void*, int, char*); the body reads three ints, bound by asm label
extern "C" void cSectionMan_load_impl(void* self, int idx, int mode) __asm__("cSectionMan_load");

// Shared with cSectionMan_activate: true for a real section id (0x31 = no section).
static inline bool IsSection(const int& id)
{
    return id != 0x31;
}

/**
 * Loads a location: unloads the world sections it does not use and loads the ones it does.
 *
 * Unloads each of the 0x32 sections that is not in the location's list (D_00442488) and not one of the kept
 * sections 0x2B-0x30, stores the location at +0x1BC (clearing +0x1C0), loads the location's sections, its sky
 * section (default 0x31; +0x1A4 cleared first) and its extra section, then the mode with cSectionMan_loadMode.
 * Mode 7 means the location's default mode from D_00442820.
 *
 * @param idx   the location (On Tour eBELocationID)
 * @param mode  the section manager mode, or 7 for the location's default
 */
extern "C" void cSectionMan_load_impl(void* self, int idx, int mode)
{
    int v;
    if (mode == 7) {
        v = D_00442820_CEA8[idx].mode;
    } else {
        v = mode;
    }
    int extra = 49;
    if (D_00442488_CEA8[idx].a >= 0) {
        extra = D_00442488_CEA8[idx].a;
    }
    for (int i = 0; i < 50; i++) {
        int found = 1;
        for (int j = 0; j < D_00442488_CEA8[idx].count; j++) {
            if (i == D_00442488_CEA8[idx].ids[j]) {
                found = 0;
                break;
            }
        }
        if ((unsigned)(i - 44) <= 4) {
            found = 0;
        }
        if (i == 43) {
            found = 0;
        }
        if (found) {
            cSectionMan_unloadSection((cSectionMan*)self, i);
        }
    }
    *(int*)((char*)self + 0x1BC) = idx;
    *(int*)((char*)self + 0x1C0) = 0;
    for (int k = 0; k < D_00442488_CEA8[idx].count; k++) {
        cSectionMan_loadSection((cSectionMan*)self, D_00442488_CEA8[idx].ids[k]);
    }
    if (IsSection(extra)) {
        *(int*)((char*)self + 0x1A4) = 0;
        cSectionMan_loadSection((cSectionMan*)self, extra);
    }
    int b = D_00442488_CEA8[idx].b;
    if (b >= 0) {
        cSectionMan_loadSection((cSectionMan*)self, b);
    }
    cSectionMan_loadMode((cSectionMan*)self, v);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_activate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// Shared with cSectionMan_activate: true for a real section id (0x31 = no section).
struct sGroup_D088 {
    int f0;
    int count;
    int a;
    int b;
    int ids[6];
};
struct sPair_D088 {
    int f0;
    int f4;
};
extern sGroup_D088 D_00442488_D088[] __asm__("D_00442488");
extern sPair_D088 D_00442820_D088[] __asm__("D_00442820");
extern "C" void cSectionMan_activateSection_D088(void* self, int id) __asm__("cSectionMan_activateSection");
extern "C" void cSectionMan_activateMode_D088(void* self, int v) __asm__("cSectionMan_activateMode");
extern "C" void cSectionMan_unloadSection_D088(void* self, int idx) __asm__("cSectionMan_unloadSection");
extern "C" void cSectionMan_setSky(void* self, int sky);

/**
 * Activates a location: unloads every section it does not use, activates its sections and sky, then its event
 * mode.
 *
 * Sky sections 0x2B-0x30 are never unloaded here. Mode 7 (default) uses the location's own mode.
 *
 * @param idx  the location (On Tour eBELocationID LocationID)
 * @param mode  the event mode (On Tour eSectionManMode Mode)
 */
extern "C" void cSectionMan_activate(cSectionMan* self, int idx, int mode)
{
    int sectionMode;
    if (mode == 7) {
        sectionMode = D_00442820_D088[idx].f4;
    } else {
        sectionMode = mode;
    }
    int extra = 49;
    if (D_00442488_D088[idx].a >= 0) {
        extra = D_00442488_D088[idx].a;
    }
    for (int i = 0; i < 50; i++) {
        int found = 1;
        for (int j = 0; j < D_00442488_D088[idx].count; j++) {
            if (i == D_00442488_D088[idx].ids[j]) {
                found = 0;
                break;
            }
        }
        if ((unsigned)(i - 44) <= 4) {
            found = 0;
        }
        if (i == 43) {
            found = 0;
        }
        if (found) {
            cSectionMan_unloadSection_D088(self, i);
        }
    }
    self->mbLocationIsActivated = 1;
    self->mBELocationID = idx;
    for (int k = 0; k < D_00442488_D088[idx].count; k++) {
        cSectionMan_activateSection_D088(self, D_00442488_D088[idx].ids[k]);
    }
    cSectionMan_setSky(self, idx);
    if (IsSection(extra)) {
        cSectionMan_activateSection_D088(self, extra);
    }
    int secondExtra = D_00442488_D088[idx].b;
    if (secondExtra >= 0) {
        cSectionMan_activateSection_D088(self, secondExtra);
    }
    cSectionMan_activateMode_D088(self, sectionMode);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_isLoaded);
#ifdef SKIP_ASM
struct sGroup_D278 {
    int f0;
    int count;
    int a;
    int b;
    int ids[6];
};
extern sGroup_D278 D_00442488_D278[] __asm__("D_00442488");
extern "C" int cSectionMan_isSectionLoaded(cSectionMan* self, int id);

// PORT: the unit declares cSectionMan_isLoaded(void*) for its callers; the body reads a1, bound by asm label
int cSectionMan_isLoaded_impl(void* self, int idx) __asm__("cSectionMan_isLoaded");

/**
 * Tells whether all of a location's world sections are loaded.
 *
 * Checks each section in the location's list (D_00442488) and its two optional extra sections (when they exist in
 * cSectionMan_mSectionMap) with cSectionMan_isSectionLoaded.
 *
 * @param idx  the location (On Tour eBELocationID)
 * @return 1 when everything is loaded, else 0
 */
int cSectionMan_isLoaded_impl(void* self, int idx)
{
    int ok = 1;
    for (int i = 0; i < D_00442488_D278[idx].count; i++) {
        if (cSectionMan_isSectionLoaded((cSectionMan*)self, D_00442488_D278[idx].ids[i]) == 0) {
            ok = 0;
            break;
        }
    }
    int a = D_00442488_D278[idx].a;
    if (a >= 0 && cSectionMan_mSectionMap[a].field_0x4 >= 0) {
        if (cSectionMan_isSectionLoaded((cSectionMan*)self, a) == 0) {
            ok = 0;
        }
    }
    int b = D_00442488_D278[idx].b;
    if (b >= 0 && cSectionMan_mSectionMap[b].field_0x4 >= 0) {
        if (cSectionMan_isSectionLoaded((cSectionMan*)self, b) == 0) {
            ok = 0;
        }
    }
    return ok;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/game", cSectionMan_isActive);
#ifdef SKIP_ASM
extern "C" int cSectionMan_isLoaded(void* self);

/**
 * Says whether a location is loaded and activated: cSectionMan_isLoaded (all its sections loaded) and mbLocationIsActivated.
 *
 * On Tour's isActive takes the location (eBELocationID LocationID). The C prototype here declares only @p self: the
 * location stays in $a1 and reaches cSectionMan_isLoaded, which reads it as its second argument.
 *
 * @return 1 when the location's sections are loaded and mbLocationIsActivated is set, else 0
 */
extern "C" int cSectionMan_isActive(cSectionMan* self)
{
    int active = 0;
    if (cSectionMan_isLoaded(self) != 0) {
        active = self->mbLocationIsActivated != 0;
    }
    return active;
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_loadSection);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cSectionMan_addActiveEvent(void* self);

/**
 * Queues a section for loading unless it is already loaded or on its way.
 *
 * An empty or unloading section (states 0, 7) becomes load-queued (3); one queued for unload (5) goes back to
 * loaded (1).
 *
 * @param idx  the back-end section (On Tour eBESectionID SectionID)
 */
extern "C" void cSectionMan_loadSection(cSectionMan* self, int idx)
{
    if (cSectionMan_mSectionMap[idx].field_0x4 >= 0) {
        int changed = 0;
        switch (cSectionMan_mSectionMap[idx].field_0x8) {
        case 5:
            cSectionMan_mSectionMap[idx].field_0x8 = 1;
            changed = 1;
            break;
        case 0: case 7:
            cSectionMan_mSectionMap[idx].field_0x8 = 3;
            changed = 1;
            break;
        case 1: case 2: case 3: case 4: case 6: case 8:
            break;
        }
        if (changed) {
            cSectionMan_addActiveEvent(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_activateSection);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
void cSectionMan_addActiveEvent_478(void* self, int i) __asm__("cSectionMan_addActiveEvent");
void cWorldView_activateSection_478(void* mgr, int handle) __asm__("cWorldView_activateSection");

/**
 * Makes a section active: at once when it is loaded, otherwise once its load finishes.
 *
 * A loaded section (1, or 5 queued for unload) is activated in the world view (2); empty or queued sections
 * become
 * load-and-activate queued (4), and one loading (6) is marked to activate when loaded (8).
 *
 * @param self_  the section manager
 * @param i  the back-end section (On Tour eBESectionID SectionID)
 */
extern "C" void cSectionMan_activateSection(cSectionMan* self_, int i)
{
    int changed;
    if (cSectionMan_mSectionMap[i].field_0x4 < 0)
        return;
    changed = 0;
    switch (cSectionMan_mSectionMap[i].field_0x8) {
    case 0:
        cSectionMan_mSectionMap[i].field_0x8 = 4;
        changed = 1;
        break;
    case 7:
        cSectionMan_mSectionMap[i].field_0x8 = 4;
        changed = 1;
        break;
    case 3:
        cSectionMan_mSectionMap[i].field_0x8 = 4;
        changed = 1;
        break;
    case 6:
        cSectionMan_mSectionMap[i].field_0x8 = 8;
        changed = 1;
        break;
    case 1:
    case 5:
        cSectionMan_mSectionMap[i].field_0x8 = 2;
        cWorldView_activateSection_478(*(char**)*(char**)((char*)self_->mpGame + 0x10) + 0x10, cSectionMan_mSectionMap[i].field_0x4);
        changed = 1;
        break;
    case 8:
        break;
    }
    if (changed)
        cSectionMan_addActiveEvent_478(((char*)self_), i);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_unloadSection);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cSectionMan_addActiveEvent(void* self);

/**
 * Queues a section for unloading: a load-queued one goes back to empty, a loaded, active or loading one to
 * state 5.
 *
 * @param idx  the back-end section (On Tour eBESectionID SectionID)
 */
extern "C" void cSectionMan_unloadSection(cSectionMan* self, int idx)
{
    if (cSectionMan_mSectionMap[idx].field_0x4 >= 0) {
        int changed = 0;
        switch (cSectionMan_mSectionMap[idx].field_0x8) {
        case 3:
            cSectionMan_mSectionMap[idx].field_0x8 = 0;
            changed = 1;
            break;
        case 1: case 2: case 6:
            cSectionMan_mSectionMap[idx].field_0x8 = 5;
            changed = 1;
            break;
        case 0: case 4: case 5: case 7: case 8:
            break;
        }
        if (changed) {
            cSectionMan_addActiveEvent(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_loadMode);
#ifdef SKIP_ASM
/**
 * Starts preloading an event mode when no mode change is in progress.
 *
 * @param Mode  the event mode (On Tour eSectionManMode Mode); 0 does nothing
 */
extern "C" void cSectionMan_loadMode(cSectionMan* self, int Mode)
{
    if (Mode != 0 && self->mModeState == 0) {
        self->mModeState = 1;
        self->mMode = Mode;
    }
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_activateMode);
#ifdef SKIP_ASM
/**
 * Requests a switch to an event mode, after its preload finishes when one is running.
 *
 * @param flag  the event mode (On Tour eSectionManMode Mode); 0 does nothing
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cSectionMan_activateMode(cSectionMan* self, int flag)
{
    if (flag == 0) {
        return;
    }
    switch (self->mModeState) {
    case 0:
        self->mMode = flag;
        self->mModeState = 3;
        break;
    case 1:
        self->mModeState = 3;
        break;
    case 2:
        self->mModeState = 4;
        break;
    case 3:
    case 4:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_eventbegin);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cBE_setState(int state);
extern "C" void cAI_resetRidersStatTotals(void* p);
extern "C" int cBENewRaceInterface_setGameMode(void* self, int mode);
extern "C" int cBENewRaceInterface_setGameEvent(void* self, int event);
extern "C" void cGameModeMan_initGameMode(void* self, unsigned int type);
extern "C" void cGame_setState(void* gm, int state, int a, int b);

struct sVE_22D6C8 {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline bool Is_22D6C8(int m, int v)
{
    return m == v;
}

/**
 * Switches the race setup to the event the rider has entered while riding freely, and starts it.
 *
 * Only offline in game mode 4 (probably free ride), outside locations 14-16 and not for mode 1. Maps the
 * section
 * mode to the back end's game mode and event (race, slopestyle, big air, pipe, time challenge), resets the
 * riders'
 * stat totals and initializes the game mode.
 *
 * @param mode  the event mode (On Tour eSectionManMode Mode)
 */
extern "C" void cSectionMan_eventbegin(cSectionMan* self, int mode)
{
    char* race = (char*)cBE_getInterface_Fv(cBE_getBE(), 0);
    int skip = cBENewRaceInterface_newRace.cGamePath != 0 || !Is_22D6C8(cBENewRaceInterface_newRace.cGameMode, 4) || (unsigned int)(self->mBELocationID - 14) < 3 || mode == 1;
    if (skip) {
        return;
    }
    cBE_setState(0);
    if (SSXApp->Game->mCurrentStateID != 4) {
        return;
    }
    cAI_resetRidersStatTotals(SSXApp->Game->AI);
    char* app = (char*)SSXApp;
    *(int*)(*(char**)(*(char**)(app + 0x84) + 0xC) + 0xC) = 0;
    if ((unsigned int)(*(int*)(*(char**)(*(char**)(app + 0x84) + 0x78) + 0x1BC) - 14) < 3) {
        cBENewRaceInterface_setGameMode(race, 4);
        cBENewRaceInterface_setGameEvent(race, 12);
    } else {
        switch (mode) {
        case 2:
            cBENewRaceInterface_setGameMode(race, 0);
            cBENewRaceInterface_setGameEvent(race, 0);
            break;
        case 3:
            cBENewRaceInterface_setGameMode(race, 1);
            cBENewRaceInterface_setGameEvent(race, 1);
            break;
        case 4:
            cBENewRaceInterface_setGameMode(race, 2);
            cBENewRaceInterface_setGameEvent(race, 3);
            break;
        case 5:
            cBENewRaceInterface_setGameMode(race, 3);
            cBENewRaceInterface_setGameEvent(race, 2);
            break;
        case 6:
            cBENewRaceInterface_setGameMode(race, 5);
            cBENewRaceInterface_setGameEvent(race, 4);
            break;
        case 0:
        case 1:
            break;
        }
    }
    sVE_22D6C8* vt = *(sVE_22D6C8**)(race + 0xC);
    vt[1].fn(race + vt[1].delta);
    cGameModeMan_initGameMode(SSXApp->GameModeMan, cBENewRaceInterface_newRace.cGameEvent);
    cGame_setState(self->mpGame, 1, 1, 0);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_update);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
struct cWorldView;
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
unsigned int cWorldView_getNumSections_u(cWorldView* view) __asm__("cWorldView_getNumSections__FP10cWorldView");
int cWorldView_isSectionLoaded(cWorldView* view, int section);
extern "C" void* cBENewRaceInterface_getSectionFile(void* iface, int i);
extern "C" int cBENewRaceInterface_getSectionType(void* iface, int i);
extern "C" void cSectionMan_skyLoadComplete(cSectionMan* self, int v);
extern "C" void cSectionMan_skySwap(cSectionMan* self);
extern "C" void cSectionMan_preLoadMode(cSectionMan* self);
void* cSectionMan_isPreLoadModeDone(void* self);
extern "C" void cSectionMan_changeMode(cSectionMan* self);
extern "C" int* cSectionMan_getNextActiveEvent(void* self, int* node);
extern "C" void cSectionMan_removeActiveEvent(void* self, int* prev, int* node);
extern "C" void cGame_sectionLoaded(void* game, int section);
extern "C" void cGame_sectionUnloading(void* game, int section);
extern "C" void cStreamMan_setBandWidthLimit(void* p, int v);
extern "C" int cWorldView_isSectionUnLoaded(cWorldView* view, int section);
extern "C" int cWorldView_loadSection(cWorldView* view, int section, float f);
extern "C" int cWorldView_unloadSection(cWorldView* view, int section);
extern "C" void cWorldView_activateSection(cWorldView* view, int section);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_00442150[];
extern float D_0044C208[];
extern char str_data_worlds_b[];
extern char str_data_worlds_c[];
extern char str_pct_spct_s_b[];

static inline cWorldView* gameWorldView_22D8D8(void* self)
{
    return (cWorldView*)(*(char**)*(char**)(*(char**)((char*)self + 0x1B0) + 0x10) + 0x10);
}

/**
 * Runs the section streaming each frame: advances queued loads and unloads, the sky swap and the event-mode
 * switch.
 *
 * Finished sections leave the active list; queued loads start only when no unload is pending. It then picks
 * the
 * stream bandwidth (fastest while loading, 4 while a mode switch holds the stream) and sets m_bStreamReady.
 */
extern "C" void cSectionMan_update(cSectionMan* self)
{
    char buf[0x100];
    if (self->m_bSkySwapPending != 0) {
        cSectionMan_skySwap(self);
    }
    int* node = cSectionMan_getNextActiveEvent(self, 0);
    int* prev = 0;
    if (node == 0 && self->mModeState == 0) {
        return;
    }
    int busy = 0;
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    int changed = 0;
    int forceOff = 0;
    int pending = 0;
    while (node != 0) {
        int idx = *node;
        int remove = 0;
        int section = cSectionMan_mSectionMap[idx].field_0x4;
        switch (cSectionMan_mSectionMap[idx].field_0x8) {
        case 7:
            if (cWorldView_isSectionUnLoaded(gameWorldView_22D8D8(self), section)) {
                cSectionMan_mSectionMap[idx].field_0x8 = 0;
            } else {
                busy = 1;
            }
            break;
        case 5:
            if (cWorldView_unloadSection(gameWorldView_22D8D8(self), section)) {
                cSectionMan_mSectionMap[idx].field_0x8 = 7;
                cGame_sectionUnloading(self->mpGame, section);
            }
            busy = 1;
            break;
        case 6:
            if (cWorldView_isSectionLoaded(gameWorldView_22D8D8(self), section)) {
                cSectionMan_mSectionMap[idx].field_0x8 = 1;
                cGame_sectionLoaded(self->mpGame, section);
            } else {
                changed = 1;
            }
            break;
        case 8:
            if (cWorldView_isSectionLoaded(gameWorldView_22D8D8(self), section)) {
                cSectionMan_mSectionMap[idx].field_0x8 = 2;
                cGame_sectionLoaded(self->mpGame, section);
                cWorldView_activateSection(gameWorldView_22D8D8(self), section);
            } else {
                changed = 1;
                forceOff = 1;
            }
            break;
        case 3:
            pending = 1;
            break;
        case 4:
            pending = 1;
            forceOff = 1;
            break;
        case 1:
            if (cBENewRaceInterface_getSectionType(iface, idx) == 4) {
                cSectionMan_skyLoadComplete(self, idx);
            }
            remove = 1;
            break;
        case 2:
            if (cBENewRaceInterface_getSectionType(iface, idx) == 4) {
                cSectionMan_skyLoadComplete(self, idx);
            }
            if (self->m_bSkySwapPending != 0) {
                cSectionMan_skySwap(self);
            }
            remove = 1;
            break;
        case 0:
            remove = 1;
            break;
        }
        if (remove) {
            cSectionMan_removeActiveEvent(self, prev, node);
            node = prev;
        }
        prev = node;
        node = cSectionMan_getNextActiveEvent(self, prev);
    }
    if (!busy && pending) {
        int* n = cSectionMan_getNextActiveEvent(self, 0);
        char* fmt = cWorldView_getNumSections_u(gameWorldView_22D8D8(self)) >= 2 ? str_data_worlds_b : str_data_worlds_c;
        for (; n != 0; n = cSectionMan_getNextActiveEvent(self, n)) {
            int idx = *n;
            int section = cSectionMan_mSectionMap[idx].field_0x4;
            sprintf(buf, str_pct_spct_s_b, fmt, cBENewRaceInterface_getSectionFile(iface, idx));
            int* state = &cSectionMan_mSectionMap[idx].field_0x8;
            if (*state == 3) {
                if (cWorldView_loadSection(gameWorldView_22D8D8(self), section, D_0044C208[cSectionMan_mSectionMap[idx].field_0xc])) {
                    changed = 1;
                    *state = 6;
                }
            } else if (*state == 4) {
                if (cWorldView_loadSection(gameWorldView_22D8D8(self), section, D_0044C208[cSectionMan_mSectionMap[idx].field_0xc])) {
                    *state = 8;
                    changed = 1;
                    forceOff = 1;
                }
            }
        }
    }
    if (changed) {
        self->mStreamRate = 2;
    } else if (self->mbLocationIsActivated == 0) {
        self->mStreamRate = 3;
    } else {
        char* w = *(char**)*(char**)((char*)self->mpGame + 0x10);
        if (*(int*)(w + 0x250) != 0 && *(int*)(w + 0x2A0) != 0) {
            self->mStreamRate = 1;
        } else {
            self->mStreamRate = 0;
        }
    }
    int hold = 0;
    switch ((unsigned int)self->mModeState) {
    case 0:
        break;
    case 1:
        cSectionMan_preLoadMode(self);
        self->mModeState = 2;
        break;
    case 2:
        break;
    case 3:
        hold = 1;
        cSectionMan_preLoadMode(self);
        self->mModeState = 4;
        break;
    case 4:
        if (forceOff) {
            hold = 1;
        } else if (cSectionMan_isPreLoadModeDone(self) != 0) {
            cSectionMan_changeMode(self);
            self->mModeState = 0;
            self->mMode = 0;
        } else {
            hold = 1;
        }
        break;
    }
    if (forceOff || hold) {
        self->m_bStreamReady = 0;
        self->mStreamRate = 4;
    } else {
        self->m_bStreamReady = 1;
    }
    cStreamMan_setBandWidthLimit(*(char**)*(char**)((char*)self->mpGame + 0x10) + 0x300, D_00442150[self->mStreamRate]);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_skyLoadComplete);
#ifdef SKIP_ASM
extern "C" void cSectionMan_activateSection(cSectionMan* self, int v);

/**
 * Records a newly loaded sky section, activates it and marks the sky swap pending.
 *
 * @param SectionID  the sky's back-end section
 */
extern "C" void cSectionMan_skyLoadComplete(cSectionMan* self, int SectionID)
{
    if (self->m_iNextSkySectionID != SectionID) {
        self->m_iNextSkySectionID = SectionID;
        cSectionMan_activateSection(self, SectionID);
        self->m_bSkySwapPending = 1;
    }
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_skySwap);
#ifdef SKIP_ASM
extern "C" void cSectionMan_unloadSection(cSectionMan* self, int id);
void func_00353D98(void* self);
extern "C" void func_00353CF0(void* self, int id);

/**
 * Swaps to the pending sky once triggered: unloads the other sky sections and switches the sky object's
 * section.
 */
extern "C" void cSectionMan_skySwap(cSectionMan* self)
{
    if (self->m_bSkySwapPending == 0) {
        return;
    }
    if (self->m_bTriggerSkySwap == 0) {
        return;
    }
    int i;
    for (i = 44; i < 49; i++) {
        if (i != self->m_iNextSkySectionID) {
            cSectionMan_unloadSection(self, i);
        }
    }
    int id = cSectionMan_mSectionMap[self->m_iNextSkySectionID].field_0x4;
    void* skyBox = *(void**)((char*)self->mpGame + 0x14);
    if (*(int*)((char*)skyBox + 0x10) != id) {
        func_00353D98(skyBox);
        func_00353CF0(*(void**)((char*)self->mpGame + 0x14), id);
    }
    self->m_bTriggerSkySwap = 0;
    self->m_bSkySwapPending = 0;
    self->m_iNextSkySectionID = -1;
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_preLoadMode);
#ifdef SKIP_ASM
class cBEIface22DF50 {
public:
    int pad[3];
    virtual void v01();
};
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cGame_setState(void* gm, int state, int a, int b);
void func_00231278(void* gm, int state);
extern int D_00535C08[];
extern int D_004A11B8;

/**
 * Starts preloading the next event: stores the location in the race setup and notifies the game flow (0xB,
 * then 0xA).
 */
extern "C" void cSectionMan_preLoadMode(cSectionMan* self)
{
    cGame_setState(self->mpGame, 11, 0, 1);
    cBEIface22DF50* iface = (cBEIface22DF50*)cBE_getInterface_Fv(cBE_getBE(), 0);
    D_00535C08[0] = self->mBELocationID;
    D_004A11B8 = 1;
    iface->v01();
    func_00231278(self->mpGame, 10);
}
#endif

extern "C" void* cGame_isPreloadedStateReady(int);

//100%
INCLUDE_ASM("main/game", cSectionMan_isPreLoadModeDone__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the preloaded game state is ready: asks cGame_isPreloadedStateReady on mpGame (+0x1B0).
 *
 * @return cGame_isPreloadedStateReady's answer (nonzero when ready)
 */
void* cSectionMan_isPreLoadModeDone(void* self)
{
    return cGame_isPreloadedStateReady(*(int*)((char*)self + 0x1b0));
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_changeMode);
#ifdef SKIP_ASM
class cRace22DFF0 {
public:
    char pad[0xCC];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05(int);
};
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cSectionMan_setSky(void* self, int sky);
extern "C" void cGame_setState(void* gm, int state, int a, int b);

/**
 * Finishes an event-mode switch: sets the location's sky, resets the AI if requested (mModeReset), notifies
 * the game flow.
 */
extern "C" void cSectionMan_changeMode(cSectionMan* self)
{
    cSectionMan_setSky(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)));
    if (self->mModeReset != 0) {
        self->mModeReset = 0;
        ((cRace22DFF0*)SSXApp->Game->AI)->v05(3);
    }
    cGame_setState(self->mpGame, 10, 0, 0);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_isSectionLoaded);
#ifdef SKIP_ASM
/**
 * Tells whether a back-end section is loaded or active.
 *
 * @param SectionID  the back-end section (On Tour eBESectionID SectionID)
 * @return 1 when the section's state is 1 or 2, otherwise 0
 */
extern "C" int cSectionMan_isSectionLoaded(cSectionMan* self, int SectionID)
{
    int loadState = cSectionMan_mSectionMap[SectionID].field_0x8;
    return (unsigned int)(loadState - 1) < 2;
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_getWorldSectionIDFromBELocationID);
#ifdef SKIP_ASM
struct sGameEntry2 {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
    int field_0x10;
    int field_0x14;
    int field_0x18;
    int field_0x1c;
    int field_0x20;
    int field_0x24;
};
extern sGameEntry2 D_00442488[];

/**
 * Returns the world section of a location's main section.
 *
 * @param LocationID  the location (On Tour eBELocationID LocationID)
 * @return the world section ID, or -1 when it is not in the world
 */
extern "C" int cSectionMan_getWorldSectionIDFromBELocationID(cSectionMan* self, int LocationID)
{
    return cSectionMan_mSectionMap[D_00442488[LocationID].field_0x10].field_0x4;
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_getWorldSectionIDFromBESectionID);
#ifdef SKIP_ASM
/**
 * Returns the world section a back-end section maps to.
 *
 * @param SectionID  the back-end section (On Tour eBESectionID SectionID)
 * @return the world section ID, or -1 when it is not in the world
 */
extern "C" int cSectionMan_getWorldSectionIDFromBESectionID(cSectionMan* self, int SectionID)
{
    return cSectionMan_mSectionMap[SectionID].field_0x4;
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_getBESectionIDFromWorldSectionID);
#ifdef SKIP_ASM
/**
 * Finds the back-end section that maps to a world section.
 *
 * @param WorldSectionID  the world section (On Tour WorldSectionID)
 * @return the back-end section, or 50 when none maps to it
 */
extern "C" int cSectionMan_getBESectionIDFromWorldSectionID(cSectionMan* self, int WorldSectionID)
{
    int i;
    for (i = 0; i < 50; i++) {
        if (WorldSectionID == cSectionMan_mSectionMap[i].field_0x4) {
            return i;
        }
    }
    return 50;
}
#endif


//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/game", cSectionMan_gotoLocation);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cSectionMan_activate(cSectionMan* self, int a1, int a2);
extern signed char D_00535C11[];

/**
 * Activates a location (cSectionMan_activate(.., 1)) and sets mModeReset, but only off the career path.
 *
 * Does nothing unless the new-race game path (D_00535C11, cBENewRaceInterface cGamePath) is 0. The new-race interface
 * is fetched but not used.
 *
 * @param location  the location id (On Tour eBELocationID location)
 */
extern "C" void cSectionMan_gotoLocation(cSectionMan* self, int location)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 0) {
        cSectionMan_activate(self, location, 1);
        self->mModeReset = 1;
    }
}
#endif
