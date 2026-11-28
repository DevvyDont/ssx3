#include "vtable.h"
#include "common.h"
#include "bx/math3d.h"
#include "be/beinterface.h"
#include "object/skybox.h"
#include "render/ps2bezierman.h"
#include "camera/camera.h"
#include "visualfx/lensfx.h"
#include "world/worldcache.h"
#include "main/gamerender.h"
#include "main/gameviewman.h"
#include "sound/icepick/worldtriggermanager.h"
#include "visualfx/fog.h"
#include "sound/ssxaudio.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "main/sectionman.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/gamestate.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "render/graphicsman.h"

extern tSectionMap cSectionMan_mSectionMap[];

//100%
INCLUDE_ASM("main/game", cGame_renderModels);
#ifdef SKIP_ASM



static inline int curLayer_C078(cPS2GraphicsMan* c)
{
    return c->_EffectTexMap[c->m_SpecularTextureId[6]];
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
void cGame::renderModels(FrustumIntersector* world)
{
    *(GrMan->m_pCurrentDrawState->m_textureState.m_texid + 1) = curLayer_C078(GrMan);
    cPS2GraphicsMan* ctx = GrMan;
    (ctx->m_pCurrentDrawState)->m_bitGroups[1] = (((int)(ctx->m_pCurrentDrawState)->m_bitGroups[1]) & ~0xF80) | 0x880;
    (*(int*)&(ctx->m_pCurrentDrawState)->m_bitGroups[0]) |= 0x30;
    int flag = 0xC00;
    if (ViewMan->m_uiNumView >= 2) {
        flag = 0x400;
    }
    int n = world->numinstancesinside;
    tInstanceRecord** items = world->instancesinside;
    for (int i = 0; i < n; i++) {
        GrMan->RenderModel(items[i], 0, 0, flag);
    }
    int secondCount = world->numinstancescrossing;
    items = world->instancescrossing;
    for (int i = 0; i < secondCount; i++) {
        GrMan->RenderModel(items[i], 0, 0, flag | 0x20);
    }
}
#endif

//100%
INCLUDE_ASM("main/game", patchLodContext_begin);
#ifdef SKIP_ASM

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
    return (sMat_C1B0*)ps2GraphicsMan->GetView();
}

// PORT: the unit declares patchLodContext_begin as void(void*); the body returns self, bound by asm label.
sRenderCam_C1B0* patchLodContext_begin_impl(sRenderCam_C1B0* self) __asm__("patchLodContext_begin");

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
sRenderCam_C1B0* patchLodContext_begin_impl(sRenderCam_C1B0* self)
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
    float nearLod = bezierMan->vt[8].fn(vthis(bezierMan, bezierMan->vt[8]));
    bezierMan = self->cam;
    float farLod = bezierMan->vt[9].fn(vthis(bezierMan, bezierMan->vt[9]));
    nearLod = nearLod * nearLod * 2.0f;
    farLod = farLod * farLod * 2.0f;
    self->k = (self->n - self->f) / (farLod - nearLod);
    self->c = self->f - self->k * nearLod;
    bezierMan = self->cam;
    ((sVEv_C1B0*)bezierMan->vt)[17].fn(vthis(bezierMan, bezierMan->vt[17]));
    return self;
}
#endif

//100%
INCLUDE_ASM("main/game", patchLodContext_end);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Ends a patch-render context: calls slot 18 of its bezier manager (+0x0), then frees the context when bit 0
 * of @p flags is set.
 *
 * cGame_renderPatches calls it with flags 2 on its stack context after drawing.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the context
 */
extern "C" void patchLodContext_end(void* self, int flags)
{
    cPS2BezierMan* obj = (cPS2BezierMan*)(*(void**)self);
    sVEntry<void(void*)>* vt = (sVEntry<void (void*)>*)obj->mVtbl;
    vt[18].fn(vthis(obj, vt[18]));
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/game", patchLodContext_submitPatch);
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
struct sPatch_C410 {
    char pad0[0x170];
    t3Vector v[4];    // 0x170
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

// PORT: the unit declares patchLodContext_submitPatch(void*, int, int) for its callers; this definition takes the
// patch as a typed pointer, so it binds to the same symbol through an asm label.
extern "C" void patchLodContext_submitPatch_impl(void* vctx, sPatch_C410* patch, int flag) __asm__("patchLodContext_submitPatch");
/**
 * Submits one terrain patch to the bezier manager with a level per edge, from the eye's distance to its corners.
 *
 * Takes the squared distances from the context's eye position to the patch's four corners (+0x170, 12 bytes each), sums
 * them per edge (corners 0-1, 2-3, 0-2, 1-3), maps each sum to c + k * sum clamped to [n, f] (the values patchLodContext_begin
 * set up), and converts each level to a byte through the table D_004A2A10 ({4, 4, 6, 8}). Then calls slot 21 of the
 * context's bezier manager with the patch, the four bytes, the patch's two ids (+0x1A0, +0x1A2, as ints) and @p flag.
 * cGame_renderPatches calls it for each patch of its two lists, with flag 0 and 1.
 *
 * @param vctx  the patch-render context (patchLodContext_begin)
 * @param patch  the terrain patch
 * @param flag  the list pass (0 or 1), passed on to the bezier manager
 */
void patchLodContext_submitPatch_impl(void* vctx, sPatch_C410* patch, int flag)
{
    sRenderCam_C410* ctx = (sRenderCam_C410*)vctx;
    float lo = ctx->n;
    float dx, dy, dz;
    dx = ctx->px - patch->v[0].x;
    dy = ctx->py - patch->v[0].y;
    dz = ctx->pz - patch->v[0].z;
    float corner0DistSq = dx * dx + dy * dy + dz * dz;
    dx = ctx->px - patch->v[1].x;
    dy = ctx->py - patch->v[1].y;
    dz = ctx->pz - patch->v[1].z;
    float corner1DistSq = dx * dx + dy * dy + dz * dz;
    dx = ctx->px - patch->v[2].x;
    dy = ctx->py - patch->v[2].y;
    dz = ctx->pz - patch->v[2].z;
    float corner2DistSq = dx * dx + dy * dy + dz * dz;
    dx = ctx->px - patch->v[3].x;
    dy = ctx->py - patch->v[3].y;
    dz = ctx->pz - patch->v[3].z;
    float corner3DistSq = dx * dx + dy * dy + dz * dz;
    float level01 = ctx->c + ctx->k * (corner0DistSq + corner1DistSq);
    float level23 = ctx->c + ctx->k * (corner2DistSq + corner3DistSq);
    float level02 = ctx->c + ctx->k * (corner0DistSq + corner2DistSq);
    float level13 = ctx->c + ctx->k * (corner1DistSq + corner3DistSq);
    float hi = ctx->f;
    level01 = clamp_C410(level01, lo, hi);
    level23 = clamp_C410(level23, lo, hi);
    level02 = clamp_C410(level02, lo, hi);
    level13 = clamp_C410(level13, lo, hi);
    unsigned char fog[4];
    int ids[2];
    unsigned char detail01 = D_004A2A10[(int)level01];
    unsigned char detail23 = D_004A2A10[(int)level23];
    unsigned char detail02 = D_004A2A10[(int)level02];
    unsigned char detail13 = D_004A2A10[(int)level13];
    fog[0] = detail01;
    fog[1] = detail23;
    fog[2] = detail02;
    fog[3] = detail13;
    ids[0] = patch->id0;
    ids[1] = patch->id1;
    sCamObj_C410* bezierMan = ctx->cam;
    bezierMan->vt[21].fn(vthis(bezierMan, bezierMan->vt[21]), patch, fog, ids, flag);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/game", cGame_renderPatches);
#ifdef SKIP_ASM
extern "C" void patchLodContext_begin(void* ctx);
extern "C" void patchLodContext_submitPatch(void* ctx, int patch, int flag);
extern "C" void patchLodContext_end(void* self, int flags);


/**
 * Draws the visible terrain patches of the frustum query (two lists, passes 0 and 1), then flushes the patch
 * batch.
 *
 * With no second view (ViewMan + 0x14 is 0) it also calls slot 5 of cBezierMan_TheBezierMan before the flush.
 *
 * @param world  the frustum query results (On Tour const FrustumIntersector& fi)
 */
void cGame::renderPatches(FrustumIntersector* world)
{
    char ctx[0x30];
    patchLodContext_begin(ctx);
    int n = world->numpatchesinside;
    int* items = (int*)(&world->patchesinside[0]);
    for (int i = 0; i < n; i++) {
        patchLodContext_submitPatch(ctx, items[i], 0);
    }
    n = world->numpatchescrossing;
    items = (int*)(&world->patchescrossing[0]);
    for (int i = 0; i < n; i++) {
        patchLodContext_submitPatch(ctx, items[i], 1);
    }
    if ((int)ViewMan->m_uiCurrentView == 0) {
        cPS2BezierMan* obj = (cPS2BezierMan*)cBezierMan_TheBezierMan;
        sVEntry<void(void*)>* vt = (sVEntry<void (void*)>*)obj->mVtbl;
        vt[5].fn(vthis(obj, vt[5]));
    }
    patchLodContext_end(ctx, 2);
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
void cGame::renderFogVolumes(FrustumIntersector* world)
{
    void* fog = (char*)&world->fogvolumesinside[0];
    int n = world->numfogvolumesinside;
    if (n != 0) {
        cFogParticleMan* fogMan = FogMan;
        sVEntry_renderFog* vt = (sVEntry_renderFog*)fogMan->mVtbl;
        vt[2].fn(vthis(fogMan, vt[2]), fog, n, 0);
    }
    fog = (char*)&world->fogvolumescrossing[0];
    n = world->numfogvolumescrossing;
    if (n != 0) {
        cFogParticleMan* fogMan = FogMan;
        sVEntry_renderFog* vt = (sVEntry_renderFog*)fogMan->mVtbl;
        vt[2].fn(vthis(fogMan, vt[2]), fog, n, 2);
    }
    cFogParticleMan_render(FogMan);
}
#endif

//100%
INCLUDE_ASM("main/game", cGame_renderLightHalos);
#ifdef SKIP_ASM
void cLensFxMan_Reset(void* self, int a1);
extern "C" void cLensFxMan_AddLightHalos(void* self, void* list, int n, int mode, int a4);
extern "C" void cLensFxMan_render(void* self, int a1);

/**
 * Draws the light halos of the frustum query's two light lists (types 0 and 2) for one view through the
 * lens-FX manager.
 *
 * @param world  the frustum query results (On Tour const FrustumIntersector& fi)
 * @param ViewID  the view being drawn
 */
void cGame::renderLightHalos(FrustumIntersector* world, int ViewID)
{
    LensFxMan->mbLocked = 1;
    cLensFxMan_Reset(LensFxMan, ViewID);
    void* halos = (char*)&world->lighthalosinside[0];
    int n = world->numlighthalosinside;
    if (n != 0) {
        cLensFxMan_AddLightHalos(LensFxMan, halos, n, 0, ViewID);
    }
    halos = (char*)&world->lighthaloscrossing[0];
    n = world->numlighthaloscrossing;
    if (n != 0) {
        cLensFxMan_AddLightHalos(LensFxMan, halos, n, 2, ViewID);
    }
    cLensFxMan_render(LensFxMan, ViewID);
    LensFxMan->mbLocked = 0;
}
#endif

//100%
INCLUDE_ASM("main/game", cGame_renderVideo);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the save / zero / restore of cDepthFog_m_Enable through a pointer local `p` (keeps the store ahead of the
// GrMan reload), and `pfn`, a pointer to vtable slot 23's fn field taken before the arguments; plain form 97.19%
// (97.20% / 99.99% without each alone). Byte-identical and behaves like the plain form; landed marked under
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


class cGfx_C830 {
public:
    char pad0[0x24];
    int cur;
    char pad28[0xE84 - 0x28];
    sDrawState* stateTop;
    char padE88[0xF50 - 0xE88];
    int layer[(0x10D8 - 0xF50) / 4];
    int GetLayer() { return layer[cur]; }
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual void v8();
    virtual unsigned int GetScreenWidth();
    virtual unsigned int GetScreenHeight();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual void SetScreenClearColour(const sV3_C830&);
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void DrawFrame();
    virtual void PushViewport();
    virtual void PopViewport();
    virtual void v22(float, float, float, float, int);
    virtual void SetViewportWindow1(void*, int);
    virtual void SetViewportPerspective(float, float, float);
    virtual void SetViewportOrthographic(float, float, float, float, int, int, float, float);
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void PushView();
    virtual void PopView();
    virtual void v32();
    virtual void SetView(void*);
};


static inline void SetBits_C830(int* p, int mask, int bits) { *p = (*p & ~mask) | bits; }

static inline void Set1_C830(cGfx_C830* g, int mask, int shift, int v)
{
    g->stateTop->m_bitGroups[1] = (((int)g->stateTop->m_bitGroups[1]) & ~mask) | ((v << shift) & mask);
}

static inline void Set2_C830(cGfx_C830* g, int mask, int shift, int v)
{
    g->stateTop->m_bitGroups[2] = (((int)g->stateTop->m_bitGroups[2]) & ~mask) | ((v << shift) & mask);
}

struct sVE_C830 {
    short delta;
    short index;
    void (*fn)(void*, float, float, float, float, int);
};


struct sView_C830 {
    float x, y, w, h;
};

struct sViews_C830 {
    int f0;
    cCamera* cams[3];
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


extern char gBucketMan[4];
extern t4Matrix t4mat_Identity;
extern int cDepthFog_m_Enable;
extern "C" void cBucketMan_renderBucket(void* self, int index);
void cSSXScriptEngine_Render(void* p);
extern "C" void cUIEngine_render2D(void* p);
extern "C" void cBXLoadIndicator_Render(void* p);
extern "C" int cMenuSys_isActive(void* p);
extern "C" void cMenuSys_render(void* p);
extern "C" void cSSXAudio_Update(void* self, int a, float dt);
extern "C" void cWorldTriggerManager_Update(void* p);

/**
 * Finishes a game frame: draws each view's bucket 3, then the full-screen 2D overlays, and runs the per-frame audio and
 * trigger updates.
 *
 * For each view of ViewMan (+0x84, count at +0x10) it selects the view, sets GrMan's viewport (slot 23) and camera
 * position (slot 24), loads the camera's matrix, sets the layer and several GS state bits, and calls
 * cBucketMan_renderBucket(gBucketMan, 3). Then it resets the viewport to the full screen (slot 22 with GrMan's width
 * and height, then SetViewportOrthographic 640x480), loads t4mat_Identity, sets the debug font (+0x24) to scale 0.8, size 2 and
 * colour (1, 0.8, 0.8, 0), and draws the script engine (cSSXScriptEngine_Render(ScriptEngine)), the UI (cUIEngine_render2D on +0x48),
 * the load indicator (cBXLoadIndicator_Render on +0x94) and, when cMenuSys_isActive, the debug menu system (cMenuSys_render on
 * +0xA4). It calls GrMan slot 14 with a zero vector and slot 19 with cDepthFog_m_Enable cleared for the call, then
 * advances the audio by mUpdateTick / 60 seconds (cSSXAudio_Update(gSSXAudio, 1, ..)), runs
 * cWorldTriggerManager_Update(gWorldTriggerManager) and zeroes mUpdateTick (+0xAC). cGame_render calls it at the end of each of its
 * three render paths.
 */
extern "C" void cGame_renderVideo(cGame* self)
{
    unsigned int i = 0;
    if (((sViews_C830*)self->ViewMan)->count != 0) {
    do {
        ((sViews_C830*)self->ViewMan)->cur = i;
        cCamera* cam = ((sViews_C830*)self->ViewMan)->cams[((sViews_C830*)self->ViewMan)->cur];
        ((cGfx_C830*)GrMan)->SetViewportWindow1(&((sViews_C830*)self->ViewMan)->views[i], 1);
        ((cGfx_C830*)GrMan)->SetViewportPerspective(cam->View, cam->Near, cam->Far);
        ((cGfx_C830*)GrMan)->SetView(((char*)&cam->CamMat));
        *(short*)((char*)((cGfx_C830*)GrMan)->stateTop + 0x12) = ((cGfx_C830*)GrMan)->GetLayer();
        cPS2GraphicsMan* ps2GraphicsMan = GrMan;
        Set1_C830((cGfx_C830*)ps2GraphicsMan, 0xF80, 7, 7);
        SetBits_C830(&(*(int*)&((cGfx_C830*)ps2GraphicsMan)->stateTop->m_bitGroups[0]), 0x30, 0x30);
        Set1_C830((cGfx_C830*)ps2GraphicsMan, 0x1800000, 23, 1);
        SetBits_C830(&(*(int*)&((cGfx_C830*)ps2GraphicsMan)->stateTop->m_bitGroups[1]), 0x300000, 0x300000);
        Set1_C830((cGfx_C830*)ps2GraphicsMan, 0xFF000, 12, 13);
        Set2_C830((cGfx_C830*)ps2GraphicsMan, 0x3E0, 5, 4);
        cBucketMan_renderBucket(gBucketMan, 3);
        i++;
    } while (i < ((sViews_C830*)self->ViewMan)->count);
    }
    {
        sVE_C830* vt = (sVE_C830*)VPTR_OF(cGraphicsMan, GrMan);
        char* thisp = (char*)GrMan + vt[23].delta;
        void (**pfn)(void*, float, float, float, float, int) = &vt[23].fn;
        (*pfn)(thisp, 0.0f, 0.0f, (float)((cGfx_C830*)GrMan)->GetScreenWidth(), (float)((cGfx_C830*)GrMan)->GetScreenHeight(), 1);
    }
    ((cGfx_C830*)GrMan)->SetViewportOrthographic(0.0f, 0.0f, 640.0f, 480.0f, 0, 0, 0.0f, 1.0f);
    ((cGfx_C830*)GrMan)->SetView(&t4mat_Identity);
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    Set2_C830((cGfx_C830*)ps2GraphicsMan, 0x3E0, 5, 0x18);
    Set1_C830((cGfx_C830*)ps2GraphicsMan, 0x7C, 2, 5);
    ((sFont_C830*)self->DebugFont)->SetScale(0.8f, 0.8f);
    ((sFont_C830*)self->DebugFont)->scale = sV2_C830(2.0f, 2.0f);
    ((sFont_C830*)self->DebugFont)->color = sVec4_C830(1.0f, 0.8f, 0.8f, 0.0f);
    cSSXScriptEngine_Render(ScriptEngine);
    cUIEngine_render2D(((void*)self->UI));
    cBXLoadIndicator_Render(((void*)self->LoadIndicator));
    if (cMenuSys_isActive(((void*)self->MenuSys)) != 0) {
        cMenuSys_render(((void*)self->MenuSys));
        ((sFont_C830*)self->DebugFont)->scale = sV2_C830(2.0f, 2.0f);
        ((sFont_C830*)self->DebugFont)->color = sVec4_C830(1.0f, 0.8f, 0.8f, 0.0f);
    }
    ((cGfx_C830*)GrMan)->SetScreenClearColour(sV3_C830(0.0f, 0.0f, 0.0f));
    // Storing through a pointer keeps the store ahead of the reload of GrMan.
    int* depthFogEnable = &cDepthFog_m_Enable;
    int saved = *depthFogEnable;
    *depthFogEnable = 0;
    ((cGfx_C830*)GrMan)->DrawFrame();
    *depthFogEnable = saved;
    cSSXAudio_Update(gSSXAudio, 1, (float)self->mUpdateTick * (1.0f / 60.0f));
    cWorldTriggerManager_Update(gWorldTriggerManager);
    self->mUpdateTick = 0;
}
#endif

//100%
INCLUDE_ASM("main/game", __11cSectionMani);
#ifdef SKIP_ASM
extern "C" void cSectionMan_init(cSectionMan* self);

/**
 * Constructs the section manager for a game and initializes it.
 *
 * @param Game  the game (cGame*) that owns it
 */
cSectionMan::cSectionMan(int Game)
{
    mpGame = (cGame*)Game;
    init();
}
#endif

//100%
INCLUDE_ASM("main/game", _$_11cSectionMan);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys the section manager; it owns nothing, so this only frees the object when asked to.
 */
cSectionMan::~cSectionMan()
{
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
struct cWorldView;
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cWorldView_getNumSections(cWorldView* view);
extern "C" void* cWorldView_getSectionName(cWorldView* self, unsigned int i);
extern "C" int strcmp(const void* a, const void* b);
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
void cSectionMan::init()
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    int i;
    unsigned int j;

    for (int k = 0; k < 50; k++) {
        cSectionMan_mSectionMap[k].iWorldSectionID = -1;
        cSectionMan_mSectionMap[k].State = 0;
    }
    cSectionMan_mSectionMap[49].iWorldSectionID = 0;
    for (i = 0; i < 50; i++) {
        for (j = 0; j < (unsigned int)cWorldView_getNumSections(gameWorldView_22CD40(this)); j++) {
            void* name = cBENewRaceInterface_getSectionFile(iface, i);
            if (strcmp(name, cWorldView_getSectionName(gameWorldView_22CD40(this), j)) == 0) {
                cSectionMan_mSectionMap[i].iWorldSectionID = j;
            }
        }
    }
    mMode = 0;
    mModeState = 0;
    mbLocationIsActivated = 0;
    mWaitTransport = 0;
    mModeReset = 0;
    mBELocationID = 22;
    m_bStreamReady = 1;
    cSectionMan_initEventPool(this);
    m_iNextSkySectionID = -1;
    m_bTriggerSkySwap = 0;
    m_bSkySwapPending = 0;
    mStreamRate = 0;
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_load);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// A location's sections (cSectionMan_mLocationMap; On Tour tLocationMap, field names from it).
struct sGroup_CEA8 {
    int LocationID;           // 0x0
    int iNumSection;          // 0x4
    int iBESkySectionID;      // 0x8 (-1: the default sky, 0x31)
    int iBETransportSectionID; // 0xC (-1: none)
    int iBESectionID[6];      // 0x10
};
struct sMode_CEA8 {
    int f0;
    int mode;       // 0x4
};
extern sGroup_CEA8 cSectionMan_mLocationMap_CEA8[] __asm__("cSectionMan_mLocationMap");
extern sMode_CEA8 cSectionMan_mLocationModeMap[] __asm__("cSectionMan_mLocationModeMap");
extern "C" void cSectionMan_unloadSection(cSectionMan* self, int idx);
extern "C" void cSectionMan_loadSection(cSectionMan* self, int idx);
extern "C" void cSectionMan_loadMode(cSectionMan* self, int a1);

// PORT: the unit's other callers declare (void*, int, char*); the body reads three ints, bound by asm label

// Shared with cSectionMan_activate: true for a real section id (0x31 = no section).
static inline bool IsSection(const int& id)
{
    return id != 0x31;
}

/**
 * Loads a location: unloads the world sections it does not use and loads the ones it does.
 *
 * Unloads each of the 0x32 sections that is not in the location's list (cSectionMan_mLocationMap) and not one of the kept
 * sections 0x2B-0x30, stores the location at +0x1BC (clearing +0x1C0), loads the location's sections, its sky
 * section (default 0x31; +0x1A4 cleared first) and its transport section, then the mode with cSectionMan_loadMode.
 * kSMMode_Default means the location's default mode from cSectionMan_mLocationModeMap.
 *
 * @param idx   the location (On Tour eBELocationID)
 * @param mode  the section manager mode, or kSMMode_Default for the location's default
 */
void cSectionMan::load(int idx, int mode)
{
    int resolvedMode;
    if (mode == kSMMode_Default) {
        resolvedMode = cSectionMan_mLocationModeMap[idx].mode;
    } else {
        resolvedMode = mode;
    }
    int skySection = 49;
    if (cSectionMan_mLocationMap_CEA8[idx].iBESkySectionID >= 0) {
        skySection = cSectionMan_mLocationMap_CEA8[idx].iBESkySectionID;
    }
    for (int i = 0; i < 50; i++) {
        int found = 1;
        for (int j = 0; j < cSectionMan_mLocationMap_CEA8[idx].iNumSection; j++) {
            if (i == cSectionMan_mLocationMap_CEA8[idx].iBESectionID[j]) {
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
            unloadSection(i);
        }
    }
    mBELocationID = idx;
    mbLocationIsActivated = 0;
    for (int k = 0; k < cSectionMan_mLocationMap_CEA8[idx].iNumSection; k++) {
        loadSection(cSectionMan_mLocationMap_CEA8[idx].iBESectionID[k]);
    }
    if (IsSection(skySection)) {
        m_bTriggerSkySwap = 0;
        loadSection(skySection);
    }
    int transportSection = cSectionMan_mLocationMap_CEA8[idx].iBETransportSectionID;
    if (transportSection >= 0) {
        loadSection(transportSection);
    }
    loadMode(resolvedMode);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_activate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// Shared with cSectionMan_activate: true for a real section id (0x31 = no section).
// A location's sections (cSectionMan_mLocationMap; On Tour tLocationMap, field names from it).
struct sGroup_D088 {
    int LocationID;           // 0x0
    int iNumSection;          // 0x4
    int iBESkySectionID;      // 0x8 (-1: the default sky, 0x31)
    int iBETransportSectionID; // 0xC (-1: none)
    int iBESectionID[6];      // 0x10
};
struct sPair_D088 {
    int f0;
    int f4;
};
extern sGroup_D088 cSectionMan_mLocationMap_D088[] __asm__("cSectionMan_mLocationMap");
extern sPair_D088 cSectionMan_mLocationModeMap_D088[] __asm__("cSectionMan_mLocationModeMap");
extern "C" void cSectionMan_activateSection_D088(void* self, int id) __asm__("cSectionMan_activateSection");
extern "C" void cSectionMan_activateMode_D088(void* self, int v) __asm__("cSectionMan_activateMode");
extern "C" void cSectionMan_unloadSection_D088(void* self, int idx) __asm__("cSectionMan_unloadSection");
extern "C" void cSectionMan_setSky(void* self, int sky);

/**
 * Activates a location: unloads every section it does not use, activates its sections and sky, then its event
 * mode.
 *
 * Sky sections 0x2B-0x30 are never unloaded here. kSMMode_Default uses the location's own mode.
 *
 * @param idx  the location (On Tour eBELocationID LocationID)
 * @param mode  the event mode (On Tour eSectionManMode Mode)
 */
void cSectionMan::activate(int idx, int mode)
{
    int sectionMode;
    if (mode == kSMMode_Default) {
        sectionMode = cSectionMan_mLocationModeMap_D088[idx].f4;
    } else {
        sectionMode = mode;
    }
    int skySection = 49;
    if (cSectionMan_mLocationMap_D088[idx].iBESkySectionID >= 0) {
        skySection = cSectionMan_mLocationMap_D088[idx].iBESkySectionID;
    }
    for (int i = 0; i < 50; i++) {
        int found = 1;
        for (int j = 0; j < cSectionMan_mLocationMap_D088[idx].iNumSection; j++) {
            if (i == cSectionMan_mLocationMap_D088[idx].iBESectionID[j]) {
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
            cSectionMan_unloadSection_D088(this, i);
        }
    }
    mbLocationIsActivated = 1;
    mBELocationID = idx;
    for (int k = 0; k < cSectionMan_mLocationMap_D088[idx].iNumSection; k++) {
        cSectionMan_activateSection_D088(this, cSectionMan_mLocationMap_D088[idx].iBESectionID[k]);
    }
    cSectionMan_setSky(this, idx);
    if (IsSection(skySection)) {
        cSectionMan_activateSection_D088(this, skySection);
    }
    int transportSection = cSectionMan_mLocationMap_D088[idx].iBETransportSectionID;
    if (transportSection >= 0) {
        cSectionMan_activateSection_D088(this, transportSection);
    }
    cSectionMan_activateMode_D088(this, sectionMode);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_isLoaded);
#ifdef SKIP_ASM
struct sGroup_D278 {
    int LocationID;           // 0x0
    int iNumSection;          // 0x4
    int iBESkySectionID;      // 0x8
    int iBETransportSectionID; // 0xC
    int iBESectionID[6];      // 0x10
};
extern sGroup_D278 cSectionMan_mLocationMap_D278[] __asm__("cSectionMan_mLocationMap");
extern "C" int cSectionMan_isSectionLoaded(cSectionMan* self, int id);

/**
 * Tells whether all of a location's world sections are loaded.
 *
 * Checks each section in the location's list (cSectionMan_mLocationMap) and its optional sky and transport sections (when they exist in
 * cSectionMan_mSectionMap) with cSectionMan_isSectionLoaded.
 *
 * @param idx  the location (On Tour eBELocationID)
 * @return 1 when everything is loaded, else 0
 */
int cSectionMan::isLoaded(int idx)
{
    int ok = 1;
    for (int i = 0; i < cSectionMan_mLocationMap_D278[idx].iNumSection; i++) {
        if (((cSectionMan*)this)->isSectionLoaded(cSectionMan_mLocationMap_D278[idx].iBESectionID[i]) == 0) {
            ok = 0;
            break;
        }
    }
    int skySection = cSectionMan_mLocationMap_D278[idx].iBESkySectionID;
    if (skySection >= 0 && cSectionMan_mSectionMap[skySection].iWorldSectionID >= 0) {
        if (((cSectionMan*)this)->isSectionLoaded(skySection) == 0) {
            ok = 0;
        }
    }
    int transportSection = cSectionMan_mLocationMap_D278[idx].iBETransportSectionID;
    if (transportSection >= 0 && cSectionMan_mSectionMap[transportSection].iWorldSectionID >= 0) {
        if (((cSectionMan*)this)->isSectionLoaded(transportSection) == 0) {
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
int cSectionMan::isActive()
{
    int active = 0;
    if (cSectionMan_isLoaded(this) != 0) {
        active = mbLocationIsActivated != 0;
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
 * An empty or unloading section (kSectionMap_Empty, kSectionMap_UnLoading) becomes kSectionMap_LoadQueue; one queued
 * for unload (kSectionMap_UnLoadQueue) goes back to kSectionMap_Loaded.
 *
 * @param idx  the back-end section (On Tour eBESectionID SectionID)
 */
void cSectionMan::loadSection(int idx)
{
    if (cSectionMan_mSectionMap[idx].iWorldSectionID >= 0) {
        int changed = 0;
        switch (cSectionMan_mSectionMap[idx].State) {
        case kSectionMap_UnLoadQueue:
            cSectionMan_mSectionMap[idx].State = kSectionMap_Loaded;
            changed = 1;
            break;
        case kSectionMap_Empty: case kSectionMap_UnLoading:
            cSectionMan_mSectionMap[idx].State = kSectionMap_LoadQueue;
            changed = 1;
            break;
        case kSectionMap_Loaded:
        case kSectionMap_Active:
        case kSectionMap_LoadQueue:
        case kSectionMap_LoadActivateQueue:
        case kSectionMap_Loading:
        case kSectionMap_LoadingActivate:
            break;
        }
        if (changed) {
            cSectionMan_addActiveEvent(this);
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
 * A loaded section (kSectionMap_Loaded, or kSectionMap_UnLoadQueue) is activated in the world view
 * (kSectionMap_Active); empty, unloading or load-queued sections become kSectionMap_LoadActivateQueue, and one
 * kSectionMap_Loading is marked to activate when loaded (kSectionMap_LoadingActivate).
 *
 * @param i  the back-end section (On Tour eBESectionID SectionID)
 */
void cSectionMan::activateSection(int i)
{
    int changed;
    if (cSectionMan_mSectionMap[i].iWorldSectionID < 0)
        return;
    changed = 0;
    switch (cSectionMan_mSectionMap[i].State) {
    case kSectionMap_Empty:
        cSectionMan_mSectionMap[i].State = kSectionMap_LoadActivateQueue;
        changed = 1;
        break;
    case kSectionMap_UnLoading:
        cSectionMan_mSectionMap[i].State = kSectionMap_LoadActivateQueue;
        changed = 1;
        break;
    case kSectionMap_LoadQueue:
        cSectionMan_mSectionMap[i].State = kSectionMap_LoadActivateQueue;
        changed = 1;
        break;
    case kSectionMap_Loading:
        cSectionMan_mSectionMap[i].State = kSectionMap_LoadingActivate;
        changed = 1;
        break;
    case kSectionMap_Loaded:
    case kSectionMap_UnLoadQueue:
        cSectionMan_mSectionMap[i].State = kSectionMap_Active;
        cWorldView_activateSection_478(*(char**)(char*)mpGame->World + 0x10,
                cSectionMan_mSectionMap[i].iWorldSectionID);
        changed = 1;
        break;
    case kSectionMap_LoadingActivate:
        break;
    }
    if (changed)
        cSectionMan_addActiveEvent_478(((char*)this), i);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_unloadSection);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cSectionMan_addActiveEvent(void* self);

/**
 * Queues a section for unloading: a load-queued one goes back to kSectionMap_Empty, a loaded, active or loading one
 * to kSectionMap_UnLoadQueue.
 *
 * @param idx  the back-end section (On Tour eBESectionID SectionID)
 */
void cSectionMan::unloadSection(int idx)
{
    if (cSectionMan_mSectionMap[idx].iWorldSectionID >= 0) {
        int changed = 0;
        switch (cSectionMan_mSectionMap[idx].State) {
        case kSectionMap_LoadQueue:
            cSectionMan_mSectionMap[idx].State = kSectionMap_Empty;
            changed = 1;
            break;
        case kSectionMap_Loaded: case kSectionMap_Active: case kSectionMap_Loading:
            cSectionMan_mSectionMap[idx].State = kSectionMap_UnLoadQueue;
            changed = 1;
            break;
        case kSectionMap_Empty:
        case kSectionMap_LoadActivateQueue:
        case kSectionMap_UnLoadQueue:
        case kSectionMap_UnLoading:
        case kSectionMap_LoadingActivate:
            break;
        }
        if (changed) {
            cSectionMan_addActiveEvent(this);
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
void cSectionMan::loadMode(int Mode)
{
    if (Mode != 0 && mModeState == 0) {
        mModeState = 1;
        mMode = Mode;
    }
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_activateMode);
#ifdef SKIP_ASM
/**
 * Requests a switch to an event mode, after its preload finishes when one is running.
 *
 * Idle, it stores the mode and goes to kSectionM_PreLoadQueueWait; a queued preload (kSectionM_PreLoadQueue) also
 * goes to kSectionM_PreLoadQueueWait and a running one (kSectionM_PreLoading) to kSectionM_PreLoadingWait.
 *
 * @param flag  the event mode (On Tour eSectionManMode Mode); kSMMode_None does nothing
 */
// From peak3project's fork (github.com/peak3project/ssx3).
void cSectionMan::activateMode(int flag)
{
    if (flag == 0) {
        return;
    }
    switch (mModeState) {
    case kSectionM_Idle:
        mMode = flag;
        mModeState = kSectionM_PreLoadQueueWait;
        break;
    case kSectionM_PreLoadQueue:
        mModeState = kSectionM_PreLoadQueueWait;
        break;
    case kSectionM_PreLoading:
        mModeState = kSectionM_PreLoadingWait;
        break;
    case kSectionM_PreLoadQueueWait:
    case kSectionM_PreLoadingWait:
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


static inline bool Is_22D6C8(int m, int v)
{
    return m == v;
}

/**
 * Switches the race setup to the event the rider has entered while riding freely, and starts it.
 *
 * Only offline in kGAMEMODE_FREERIDE, outside locations 14-16, not for kSMMode_Freeride and only while riding
 * (kGFSM_Riding). Maps the section mode to the back end's game mode and event (race, slopestyle, big air, pipe, time
 * challenge), resets the riders' stat totals, initializes the game mode and enters kGFSM_EventStart.
 *
 * @param mode  the event mode (On Tour eSectionManMode Mode)
 */
void cSectionMan::eventbegin(int mode)
{
    cBEInterface* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int skip = cBENewRaceInterface_newRace.cGamePath != 0
               || !Is_22D6C8(cBENewRaceInterface_newRace.cGameMode, kGAMEMODE_FREERIDE)
               || (unsigned int)(mBELocationID - 14) < 3 || mode == kSMMode_Freeride;
    if (skip) {
        return;
    }
    cBE_setState(0);
    if (SSXApp->Game->mCurrentStateID != kGFSM_Riding) {
        return;
    }
    cAI_resetRidersStatTotals(SSXApp->Game->AI);
    cSSXApp* app = SSXApp;
    app->Game->AI->EventTimerTick = 0;
    if ((unsigned int)(app->Game->SectionMan->mBELocationID - 14) < 3) {
        cBENewRaceInterface_setGameMode(race, kGAMEMODE_FREERIDE);
        cBENewRaceInterface_setGameEvent(race, kGAMEEVENT_FREERIDE);
    } else {
        switch (mode) {
        case kSMMode_Race:
            cBENewRaceInterface_setGameMode(race, kGAMEMODE_RACE);
            cBENewRaceInterface_setGameEvent(race, kGAMEEVENT_RACE);
            break;
        case kSMMode_SlopeStype:
            cBENewRaceInterface_setGameMode(race, kGAMEMODE_SLOPESTYLE);
            cBENewRaceInterface_setGameEvent(race, kGAMEEVENT_SLOPESTYLE);
            break;
        case kSMMode_BigAir:
            cBENewRaceInterface_setGameMode(race, kGAMEMODE_BIGAIR);
            cBENewRaceInterface_setGameEvent(race, kGAMEEVENT_BIGAIR);
            break;
        case kSMMode_HalfPipe:
            cBENewRaceInterface_setGameMode(race, kGAMEMODE_HALFPIPE);
            cBENewRaceInterface_setGameEvent(race, kGAMEEVENT_SUPERPIPE);
            break;
        case kSMMode_TimeChallenge:
            cBENewRaceInterface_setGameMode(race, kGAMEMODE_TIMECHALLENGE);
            cBENewRaceInterface_setGameEvent(race, kGAMEEVENT_RIVALTIME);
            break;
        case kSMMode_None:
        case kSMMode_Freeride:
            break;
        }
    }
    race->submit();
    cGameModeMan_initGameMode(SSXApp->GameModeMan, cBENewRaceInterface_newRace.cGameEvent);
    cGame_setState(mpGame, kGFSM_EventStart, 1, 0);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_update);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
struct cWorldView;
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern int cSectionMan_mStreamBandWidthRates[];
extern float cWorldCache_m_SectionLoadPriority[];
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
 * Finished sections leave the active list (a finished kSECTIONTYPE_SKY section goes to skyLoadComplete); queued
 * loads start only when no unload is pending. It then picks the stream bandwidth (eStreamRate: kStreamRate_NormalLoad1
 * while loading, kStreamRate_WaitLoad while a mode switch holds the stream) and sets m_bStreamReady.
 */
void cSectionMan::update()
{
    char buf[0x100];
    if (m_bSkySwapPending != 0) {
        skySwap();
    }
    int* node = cSectionMan_getNextActiveEvent(this, 0);
    int* prev = 0;
    if (node == 0 && mModeState == kSectionM_Idle) {
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
        int section = cSectionMan_mSectionMap[idx].iWorldSectionID;
        switch (cSectionMan_mSectionMap[idx].State) {
        case kSectionMap_UnLoading:
            if (cWorldView_isSectionUnLoaded(gameWorldView_22D8D8(this), section)) {
                cSectionMan_mSectionMap[idx].State = kSectionMap_Empty;
            } else {
                busy = 1;
            }
            break;
        case kSectionMap_UnLoadQueue:
            if (cWorldView_unloadSection(gameWorldView_22D8D8(this), section)) {
                cSectionMan_mSectionMap[idx].State = kSectionMap_UnLoading;
                cGame_sectionUnloading(mpGame, section);
            }
            busy = 1;
            break;
        case kSectionMap_Loading:
            if (cWorldView_isSectionLoaded(gameWorldView_22D8D8(this), section)) {
                cSectionMan_mSectionMap[idx].State = kSectionMap_Loaded;
                cGame_sectionLoaded(mpGame, section);
            } else {
                changed = 1;
            }
            break;
        case kSectionMap_LoadingActivate:
            if (cWorldView_isSectionLoaded(gameWorldView_22D8D8(this), section)) {
                cSectionMan_mSectionMap[idx].State = kSectionMap_Active;
                cGame_sectionLoaded(mpGame, section);
                cWorldView_activateSection(gameWorldView_22D8D8(this), section);
            } else {
                changed = 1;
                forceOff = 1;
            }
            break;
        case kSectionMap_LoadQueue:
            pending = 1;
            break;
        case kSectionMap_LoadActivateQueue:
            pending = 1;
            forceOff = 1;
            break;
        case kSectionMap_Loaded:
            if (cBENewRaceInterface_getSectionType(iface, idx) == kSECTIONTYPE_SKY) {
                skyLoadComplete(idx);
            }
            remove = 1;
            break;
        case kSectionMap_Active:
            if (cBENewRaceInterface_getSectionType(iface, idx) == kSECTIONTYPE_SKY) {
                skyLoadComplete(idx);
            }
            if (m_bSkySwapPending != 0) {
                skySwap();
            }
            remove = 1;
            break;
        case kSectionMap_Empty:
            remove = 1;
            break;
        }
        if (remove) {
            cSectionMan_removeActiveEvent(this, prev, node);
            node = prev;
        }
        prev = node;
        node = cSectionMan_getNextActiveEvent(this, prev);
    }
    if (!busy && pending) {
        int* n = cSectionMan_getNextActiveEvent(this, 0);
        char* fmt = cWorldView_getNumSections_u(gameWorldView_22D8D8(this)) >= 2 ? str_data_worlds_b : str_data_worlds_c;
        for (; n != 0; n = cSectionMan_getNextActiveEvent(this, n)) {
            int idx = *n;
            int section = cSectionMan_mSectionMap[idx].iWorldSectionID;
            sprintf(buf, str_pct_spct_s_b, fmt, cBENewRaceInterface_getSectionFile(iface, idx));
            int* state = &cSectionMan_mSectionMap[idx].State;
            if (*state == kSectionMap_LoadQueue) {
                if (cWorldView_loadSection(gameWorldView_22D8D8(this), section,
                        cWorldCache_m_SectionLoadPriority[cSectionMan_mSectionMap[idx].Priority])) {
                    changed = 1;
                    *state = kSectionMap_Loading;
                }
            } else if (*state == kSectionMap_LoadActivateQueue) {
                if (cWorldView_loadSection(gameWorldView_22D8D8(this), section,
                        cWorldCache_m_SectionLoadPriority[cSectionMan_mSectionMap[idx].Priority])) {
                    *state = kSectionMap_LoadingActivate;
                    changed = 1;
                    forceOff = 1;
                }
            }
        }
    }
    if (changed) {
        mStreamRate = kStreamRate_NormalLoad1;
    } else if (mbLocationIsActivated == 0) {
        mStreamRate = kStreamRate_NormalLoad2;
    } else {
        cWorldCache* w = (cWorldCache*)(*(char**)(char*)mpGame->World);
        if (w->m_WorldView.m_StreamFocus[0].m_bActive != 0 && w->m_WorldView.m_StreamFocus[1].m_bActive != 0) {
            mStreamRate = kStreamRate_Default2Player;
        } else {
            mStreamRate = kStreamRate_Default1Player;
        }
    }
    int hold = 0;
    switch ((unsigned int)mModeState) {
    case kSectionM_Idle:
        break;
    case kSectionM_PreLoadQueue:
        preLoadMode();
        mModeState = kSectionM_PreLoading;
        break;
    case kSectionM_PreLoading:
        break;
    case kSectionM_PreLoadQueueWait:
        hold = 1;
        preLoadMode();
        mModeState = kSectionM_PreLoadingWait;
        break;
    case kSectionM_PreLoadingWait:
        if (forceOff) {
            hold = 1;
        } else if (isPreLoadModeDone() != 0) {
            changeMode();
            mModeState = kSectionM_Idle;
            mMode = kSMMode_None;
        } else {
            hold = 1;
        }
        break;
    }
    if (forceOff || hold) {
        m_bStreamReady = 0;
        mStreamRate = kStreamRate_WaitLoad;
    } else {
        m_bStreamReady = 1;
    }
    cStreamMan_setBandWidthLimit(*(char**)(char*)mpGame->World + 0x300, cSectionMan_mStreamBandWidthRates[mStreamRate]);
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
void cSectionMan::skyLoadComplete(int SectionID)
{
    if (m_iNextSkySectionID != SectionID) {
        m_iNextSkySectionID = SectionID;
        activateSection(SectionID);
        m_bSkySwapPending = 1;
    }
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_skySwap);
#ifdef SKIP_ASM
extern "C" void cSectionMan_unloadSection(cSectionMan* self, int id);
void cSkyBox_reset(void* self);
extern "C" void cSkyBox_setSection(void* self, int id);

/**
 * Swaps to the pending sky once triggered: unloads the other sky sections and switches the sky object's
 * section.
 */
void cSectionMan::skySwap()
{
    if (m_bSkySwapPending == 0) {
        return;
    }
    if (m_bTriggerSkySwap == 0) {
        return;
    }
    int i;
    for (i = 44; i < 49; i++) {
        if (i != m_iNextSkySectionID) {
            unloadSection(i);
        }
    }
    int id = cSectionMan_mSectionMap[m_iNextSkySectionID].iWorldSectionID;
    cSkyBox* skyBox = mpGame->SkyBox;
    if (skyBox->m_iSectionID != id) {
        cSkyBox_reset(skyBox);
        cSkyBox_setSection(mpGame->SkyBox, id);
    }
    m_bTriggerSkySwap = 0;
    m_bSkySwapPending = 0;
    m_iNextSkySectionID = -1;
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_preLoadMode);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cGame_setState(void* gm, int state, int a, int b);
void cGame_setPreloadState(void* gm, int state);
extern int cBENewRaceInterface_bIsBamLocation;

/**
 * Starts preloading the next event: stores the location in the race setup and notifies the game flow (0xB,
 * then 0xA).
 */
void cSectionMan::preLoadMode()
{
    cGame_setState(mpGame, 11, 0, 1);
    cBEInterface* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    cBENewRaceInterface_newRace.iLocationIdx = mBELocationID;
    cBENewRaceInterface_bIsBamLocation = 1;
    iface->submit();
    cGame_setPreloadState(mpGame, 10);
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
void* cSectionMan::isPreLoadModeDone()
{
    return cGame_isPreloadedStateReady((int)mpGame);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_changeMode);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" void cSectionMan_setSky(void* self, int sky);
extern "C" void cGame_setState(void* gm, int state, int a, int b);

/**
 * Finishes an event-mode switch: sets the location's sky, resets the AI if requested (mModeReset), notifies
 * the game flow.
 */
void cSectionMan::changeMode()
{
    cSectionMan_setSky(this, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)));
    if (mModeReset != 0) {
        mModeReset = 0;
        cAI* ai = SSXApp->Game->AI;
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)ai->mVtbl;
        vt[5].fn(vthis(ai, vt[5]), 3); // the AI's restart
    }
    cGame_setState(mpGame, 10, 0, 0);
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_isSectionLoaded);
#ifdef SKIP_ASM
/**
 * Tells whether a back-end section is loaded or active.
 *
 * @param SectionID  the back-end section (On Tour eBESectionID SectionID)
 * @return true when the section's state is kSectionMap_Loaded or kSectionMap_Active
 */
bool cSectionMan::isSectionLoaded(int SectionID)
{
    int loadState = cSectionMan_mSectionMap[SectionID].State;
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
extern sGameEntry2 cSectionMan_mLocationMap[];

/**
 * Returns the world section of a location's main section.
 *
 * @param LocationID  the location (On Tour eBELocationID LocationID)
 * @return the world section ID, or -1 when it is not in the world
 */
int cSectionMan::getWorldSectionIDFromBELocationID(int LocationID)
{
    return cSectionMan_mSectionMap[cSectionMan_mLocationMap[LocationID].field_0x10].iWorldSectionID;
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
int cSectionMan::getWorldSectionIDFromBESectionID(int SectionID)
{
    return cSectionMan_mSectionMap[SectionID].iWorldSectionID;
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
int cSectionMan::getBESectionIDFromWorldSectionID(int WorldSectionID)
{
    int i;
    for (i = 0; i < 50; i++) {
        if (WorldSectionID == cSectionMan_mSectionMap[i].iWorldSectionID) {
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cSectionMan_activate(cSectionMan* self, int a1, int a2);

/**
 * Activates a location (cSectionMan_activate(.., 1)) and sets mModeReset, but only off the career path.
 *
 * Does nothing unless the new-race game path (newRace.cGamePath) is 0. The new-race interface
 * is fetched but not used.
 *
 * @param location  the location id (On Tour eBELocationID location)
 */
void cSectionMan::gotoLocation(int location)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (cBENewRaceInterface_newRace.cGamePath == 0) {
        activate(location, 1);
        mModeReset = 1;
    }
}
#endif
