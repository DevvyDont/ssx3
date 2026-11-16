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
extern sGameEntry D_00442168[];

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
    int n2 = *(int*)((char*)world + 0x40A0);
    items = (int*)((char*)world + 0x40A4);
    for (int i = 0; i < n2; i++) {
        ((sRVEntry_C078*)GrMan->mVtbl)[0x60].fn((char*)GrMan + ((sRVEntry_C078*)GrMan->mVtbl)[0x60].delta, items[i], 0, 0, flag | 0x20);
    }
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022C1B0);
#ifdef SKIP_ASM
extern void* D_004A4248;

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
    cPS2GraphicsMan* c = GrMan;
    return ((sVEm_C1B0*)c->mVtbl)[35].fn((char*)c + ((sVEm_C1B0*)c->mVtbl)[35].delta);
}

// PORT: the unit declares func_0022C1B0 as void(void*); the body returns self, bound by asm label.
sRenderCam_C1B0* func_0022C1B0_impl(sRenderCam_C1B0* self) __asm__("func_0022C1B0");

sRenderCam_C1B0* func_0022C1B0_impl(sRenderCam_C1B0* self)
{
    self->cam = (sCamObj_C1B0*)D_004A4248;
    self->pos = Scale_C1B0(sVec_C1B0(*(sVec_C1B0*)viewMat_C1B0()->m[3]), -1.0f);
    sMat_C1B0* m = viewMat_C1B0();
    self->pos = sVec_C1B0(self->pos.x * m->m[0][0] + self->pos.y * m->m[0][1] + self->pos.z * m->m[0][2] + self->pos.w * m->m[0][3],
                          self->pos.x * m->m[1][0] + self->pos.y * m->m[1][1] + self->pos.z * m->m[1][2] + self->pos.w * m->m[1][3],
                          self->pos.x * m->m[2][0] + self->pos.y * m->m[2][1] + self->pos.z * m->m[2][2] + self->pos.w * m->m[2][3],
                          self->pos.x * m->m[3][0] + self->pos.y * m->m[3][1] + self->pos.z * m->m[3][2] + self->pos.w * m->m[3][3]);
    self->n = 1.0f;
    self->f = 3.0f;
    self->pos.w = 1.0f;
    sCamObj_C1B0* o = self->cam;
    float a = o->vt[8].fn((char*)o + o->vt[8].delta);
    o = self->cam;
    float b = o->vt[9].fn((char*)o + o->vt[9].delta);
    a = a * a * 2.0f;
    b = b * b * 2.0f;
    self->k = (self->n - self->f) / (b - a);
    self->c = self->f - self->k * a;
    o = self->cam;
    ((sVEv_C1B0*)o->vt)[17].fn((char*)o + o->vt[17].delta);
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
extern void* D_004A4248;

struct sVEntry0022C620 {
    short delta;
    short index;
    void (*fn)(void*);
};

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
        void* obj = D_004A4248;
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
extern "C" void func_002DBF98(void* r);

extern "C" void cGame_renderFogVolumes(cGame* self, void* world)
{
    void* fog = (char*)world + 0x78B8;
    int n = *(int*)((char*)world + 0x78B4);
    if (n != 0) {
        void* r = self->FogMan;
        sVEntry_renderFog* vt = *(sVEntry_renderFog**)((char*)r + 0x4);
        vt[2].fn((char*)r + vt[2].delta, fog, n, 0);
    }
    fog = (char*)world + 0x7AC0;
    n = *(int*)((char*)world + 0x7ABC);
    if (n != 0) {
        void* r = self->FogMan;
        sVEntry_renderFog* vt = *(sVEntry_renderFog**)((char*)r + 0x4);
        vt[2].fn((char*)r + vt[2].delta, fog, n, 2);
    }
    func_002DBF98(self->FogMan);
}
#endif

//100%
INCLUDE_ASM("main/game", cGame_renderLightHalos);
#ifdef SKIP_ASM
void func_002E2F98(void* self, int a1);
extern "C" void func_002E2FF8(void* self, void* list, int n, int mode, int a4);
extern "C" void func_002E30D0(void* self, int a1);

extern "C" void cGame_renderLightHalos(cGame* self, void* world, int a2)
{
    *(int*)((char*)self->LensFxMan + 0x6494) = 1;
    func_002E2F98(self->LensFxMan, a2);
    void* p = (char*)world + 0x7BC4;
    int n = *(int*)((char*)world + 0x7BC0);
    if (n != 0) {
        func_002E2FF8(self->LensFxMan, p, n, 0, a2);
    }
    p = (char*)world + 0x7FC8;
    n = *(int*)((char*)world + 0x7FC4);
    if (n != 0) {
        func_002E2FF8(self->LensFxMan, p, n, 2, a2);
    }
    func_002E30D0(self->LensFxMan, a2);
    *(int*)((char*)self->LensFxMan + 0x6494) = 0;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022C830);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the save / zero / restore of D_004A4324 through a pointer local `p` (keeps the store ahead of the GrMan reload), and `pfn`, a pointer to vtable slot 23's fn field taken before the arguments; plain form 97.19% (97.20% / 99.99% without each alone). Byte-identical and behaves like the plain form; landed marked under
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
extern int D_004A4324;
extern "C" void func_003550A0(void* self, int index);
void func_002789C0(void* p);
extern "C" void func_00397DF8(void* p);
extern "C" void func_001F31E0(void* p);
extern "C" int func_002CC098(void* p);
extern "C" void func_002CC350(void* p);
extern "C" void func_00285BF8(void* self, int a, float dt);
extern "C" void func_002B5D78(void* p);

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
        cPS2GraphicsMan* g = GrMan;
        Set1_C830((cGfx_C830*)g, 0xF80, 7, 7);
        SetBits_C830(&((cGfx_C830*)g)->stateTop->w0, 0x30, 0x30);
        Set1_C830((cGfx_C830*)g, 0x1800000, 23, 1);
        SetBits_C830(&((cGfx_C830*)g)->stateTop->w4, 0x300000, 0x300000);
        Set1_C830((cGfx_C830*)g, 0xFF000, 12, 0xD);
        Set2_C830((cGfx_C830*)g, 0x3E0, 5, 4);
        func_003550A0(gBucketMan, 3);
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
    cPS2GraphicsMan* g = GrMan;
    Set2_C830((cGfx_C830*)g, 0x3E0, 5, 0x18);
    Set1_C830((cGfx_C830*)g, 0x7C, 2, 5);
    self->font->SetScale(0.8f, 0.8f);
    self->font->scale = sV2_C830(2.0f, 2.0f);
    self->font->color = sVec4_C830(1.0f, 0.8f, 0.8f, 0.0f);
    func_002789C0(ScriptEngine);
    func_00397DF8(self->f48);
    func_001F31E0(self->hudTimer);
    if (func_002CC098(self->fA4) != 0) {
        func_002CC350(self->fA4);
        self->font->scale = sV2_C830(2.0f, 2.0f);
        self->font->color = sVec4_C830(1.0f, 0.8f, 0.8f, 0.0f);
    }
    ((cGfx_C830*)GrMan)->v14(sV3_C830(0.0f, 0.0f, 0.0f));
    // Storing through a pointer keeps the store ahead of the reload of GrMan.
    int* p = &D_004A4324;
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
INCLUDE_ASM("main/game", func_0022CCE8);
#ifdef SKIP_ASM
extern "C" void func_0022CD40(cSectionMan* self);

extern "C" void* func_0022CCE8(cSectionMan* self, int a1)
{
    self->mpGame = (cGame*)a1;
    func_0022CD40(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022CD18);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_0022CD18(cSectionMan* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022CD40);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
struct cWorldView;
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cWorldView_getNumSections(cWorldView* view);
extern "C" void* func_003A9820(cWorldView* self, unsigned int i);
extern "C" int func_004165A8(const void* a, const void* b);
extern "C" void* func_00144D38(void* iface, int i);
extern "C" void func_0022E228(void* self);

static inline cWorldView* gameWorldView_22CD40(void* self)
{
    return (cWorldView*)(*(char**)*(char**)(*(char**)((char*)self + 0x1B0) + 0x10) + 0x10);
}

extern "C" void func_0022CD40(cSectionMan* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    int i;
    unsigned int j;

    for (int k = 0; k < 50; k++) {
        D_00442168[k].field_0x4 = -1;
        D_00442168[k].field_0x8 = 0;
    }
    D_00442168[49].field_0x4 = 0;
    for (i = 0; i < 50; i++) {
        for (j = 0; j < (unsigned int)cWorldView_getNumSections(gameWorldView_22CD40(self)); j++) {
            void* name = func_00144D38(iface, i);
            if (func_004165A8(name, func_003A9820(gameWorldView_22CD40(self), j)) == 0) {
                D_00442168[i].field_0x4 = j;
            }
        }
    }
    self->mMode = 0;
    self->mModeState = 0;
    self->mbLocationIsActivated = 0;
    self->mWaitTransport = 0;
    self->mModeReset = 0;
    self->mBELocationID = 0x16;
    self->m_bStreamReady = 1;
    func_0022E228(self);
    self->m_iNextSkySectionID = -1;
    self->m_bTriggerSkySwap = 0;
    self->m_bSkySwapPending = 0;
    self->mStreamRate = 0;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022CEA8);
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
extern "C" void func_0022D598(cSectionMan* self, int idx);
extern "C" void func_0022D3D8(cSectionMan* self, int idx);
extern "C" void func_0022D640(cSectionMan* self, int a1);

// PORT: the unit's other callers declare (void*, int, char*); the body reads three ints, bound by asm label
extern "C" void func_0022CEA8_impl(void* self, int idx, int mode) __asm__("func_0022CEA8");

// Shared with func_0022D088: true for a real section id (0x31 = no section).
static inline bool IsSection(const int& id)
{
    return id != 0x31;
}

extern "C" void func_0022CEA8_impl(void* self, int idx, int mode)
{
    int v;
    if (mode == 7) {
        v = D_00442820_CEA8[idx].mode;
    } else {
        v = mode;
    }
    int extra = 0x31;
    if (D_00442488_CEA8[idx].a >= 0) {
        extra = D_00442488_CEA8[idx].a;
    }
    for (int i = 0; i < 0x32; i++) {
        int found = 1;
        for (int j = 0; j < D_00442488_CEA8[idx].count; j++) {
            if (i == D_00442488_CEA8[idx].ids[j]) {
                found = 0;
                break;
            }
        }
        if ((unsigned)(i - 0x2C) <= 4) {
            found = 0;
        }
        if (i == 0x2B) {
            found = 0;
        }
        if (found) {
            func_0022D598((cSectionMan*)self, i);
        }
    }
    *(int*)((char*)self + 0x1BC) = idx;
    *(int*)((char*)self + 0x1C0) = 0;
    for (int k = 0; k < D_00442488_CEA8[idx].count; k++) {
        func_0022D3D8((cSectionMan*)self, D_00442488_CEA8[idx].ids[k]);
    }
    if (IsSection(extra)) {
        *(int*)((char*)self + 0x1A4) = 0;
        func_0022D3D8((cSectionMan*)self, extra);
    }
    int b = D_00442488_CEA8[idx].b;
    if (b >= 0) {
        func_0022D3D8((cSectionMan*)self, b);
    }
    func_0022D640((cSectionMan*)self, v);
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022D088);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// Shared with func_0022D088: true for a real section id (0x31 = no section).
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
extern "C" void func_0022D478_D088(void* self, int id) __asm__("func_0022D478");
extern "C" void func_0022D668_D088(void* self, int v) __asm__("func_0022D668");
extern "C" void func_0022D598_D088(void* self, int idx) __asm__("func_0022D598");
extern "C" void cSectionMan_setSky(void* self, int sky);

extern "C" void func_0022D088(cSectionMan* self, int idx, int mode)
{
    int v;
    if (mode == 7) {
        v = D_00442820_D088[idx].f4;
    } else {
        v = mode;
    }
    int extra = 0x31;
    if (D_00442488_D088[idx].a >= 0) {
        extra = D_00442488_D088[idx].a;
    }
    for (int i = 0; i < 0x32; i++) {
        int found = 1;
        for (int j = 0; j < D_00442488_D088[idx].count; j++) {
            if (i == D_00442488_D088[idx].ids[j]) {
                found = 0;
                break;
            }
        }
        if ((unsigned)(i - 0x2C) <= 4) {
            found = 0;
        }
        if (i == 0x2B) {
            found = 0;
        }
        if (found) {
            func_0022D598_D088(self, i);
        }
    }
    self->mbLocationIsActivated = 1;
    self->mBELocationID = idx;
    for (int k = 0; k < D_00442488_D088[idx].count; k++) {
        func_0022D478_D088(self, D_00442488_D088[idx].ids[k]);
    }
    cSectionMan_setSky(self, idx);
    if (IsSection(extra)) {
        func_0022D478_D088(self, extra);
    }
    int b = D_00442488_D088[idx].b;
    if (b >= 0) {
        func_0022D478_D088(self, b);
    }
    func_0022D668_D088(self, v);
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022D278);
#ifdef SKIP_ASM
struct sGroup_D278 {
    int f0;
    int count;
    int a;
    int b;
    int ids[6];
};
extern sGroup_D278 D_00442488_D278[] __asm__("D_00442488");
extern "C" int func_0022E078(cSectionMan* self, int id);

// PORT: the unit declares func_0022D278(void*) for its callers; the body reads a1, bound by asm label
int func_0022D278_impl(void* self, int idx) __asm__("func_0022D278");

int func_0022D278_impl(void* self, int idx)
{
    int ok = 1;
    for (int i = 0; i < D_00442488_D278[idx].count; i++) {
        if (func_0022E078((cSectionMan*)self, D_00442488_D278[idx].ids[i]) == 0) {
            ok = 0;
            break;
        }
    }
    int a = D_00442488_D278[idx].a;
    if (a >= 0 && D_00442168[a].field_0x4 >= 0) {
        if (func_0022E078((cSectionMan*)self, a) == 0) {
            ok = 0;
        }
    }
    int b = D_00442488_D278[idx].b;
    if (b >= 0 && D_00442168[b].field_0x4 >= 0) {
        if (func_0022E078((cSectionMan*)self, b) == 0) {
            ok = 0;
        }
    }
    return ok;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/game", func_0022D390);
#ifdef SKIP_ASM
extern "C" int func_0022D278(void* self);

extern "C" int func_0022D390(cSectionMan* self)
{
    int r = 0;
    if (func_0022D278(self) != 0) {
        r = self->mbLocationIsActivated != 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022D3D8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_0022E298(void* self);

extern "C" void func_0022D3D8(cSectionMan* self, int idx)
{
    if (D_00442168[idx].field_0x4 >= 0) {
        int changed = 0;
        switch (D_00442168[idx].field_0x8) {
        case 5:
            D_00442168[idx].field_0x8 = 1;
            changed = 1;
            break;
        case 0: case 7:
            D_00442168[idx].field_0x8 = 3;
            changed = 1;
            break;
        case 1: case 2: case 3: case 4: case 6: case 8:
            break;
        }
        if (changed) {
            func_0022E298(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022D478);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
void func_0022E298_478(void* self, int i) __asm__("func_0022E298");
void func_003A99D8_478(void* mgr, int handle) __asm__("func_003A99D8");

extern "C" void func_0022D478(cSectionMan* self_, int i)
{
    int changed;
    if (D_00442168[i].field_0x4 < 0)
        return;
    changed = 0;
    switch (D_00442168[i].field_0x8) {
    case 0:
        D_00442168[i].field_0x8 = 4;
        changed = 1;
        break;
    case 7:
        D_00442168[i].field_0x8 = 4;
        changed = 1;
        break;
    case 3:
        D_00442168[i].field_0x8 = 4;
        changed = 1;
        break;
    case 6:
        D_00442168[i].field_0x8 = 8;
        changed = 1;
        break;
    case 1:
    case 5:
        D_00442168[i].field_0x8 = 2;
        func_003A99D8_478(*(char**)*(char**)((char*)self_->mpGame + 0x10) + 0x10, D_00442168[i].field_0x4);
        changed = 1;
        break;
    case 8:
        break;
    }
    if (changed)
        func_0022E298_478(((char*)self_), i);
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022D598);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_0022E298(void* self);

extern "C" void func_0022D598(cSectionMan* self, int idx)
{
    if (D_00442168[idx].field_0x4 >= 0) {
        int changed = 0;
        switch (D_00442168[idx].field_0x8) {
        case 3:
            D_00442168[idx].field_0x8 = 0;
            changed = 1;
            break;
        case 1: case 2: case 6:
            D_00442168[idx].field_0x8 = 5;
            changed = 1;
            break;
        case 0: case 4: case 5: case 7: case 8:
            break;
        }
        if (changed) {
            func_0022E298(self);
        }
    }
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022D640);
#ifdef SKIP_ASM
extern "C" void func_0022D640(cSectionMan* self, int a1)
{
    if (a1 != 0 && self->mModeState == 0) {
        self->mModeState = 1;
        self->mMode = a1;
    }
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022D668);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_0022D668(cSectionMan* self, int flag)
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
extern "C" void func_00231250(void* gm, int state, int a, int b);

struct sVE_22D6C8 {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline bool Is_22D6C8(int m, int v)
{
    return m == v;
}

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
    char* g = (char*)SSXApp;
    *(int*)(*(char**)(*(char**)(g + 0x84) + 0xC) + 0xC) = 0;
    if ((unsigned int)(*(int*)(*(char**)(*(char**)(g + 0x84) + 0x78) + 0x1BC) - 14) < 3) {
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
    func_00231250(self->mpGame, 1, 1, 0);
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
extern "C" void* func_00144D38(void* iface, int i);
extern "C" int func_00144D50(void* iface, int i);
extern "C" void func_0022DE58(cSectionMan* self, int v);
extern "C" void cSectionMan_skySwap(cSectionMan* self);
extern "C" void func_0022DF50(cSectionMan* self);
void* func_0022DFD0(void* self);
extern "C" void func_0022DFF0(cSectionMan* self);
extern "C" int* cSectionMan_getNextActiveEvent(void* self, int* node);
extern "C" void func_0022E360(void* self, int* prev, int* node);
extern "C" void func_00230338(void* game, int section);
extern "C" void cGame_sectionUnloading(void* game, int section);
extern "C" void func_003A7058(void* p, int v);
extern "C" int func_003A9890(cWorldView* view, int section);
extern "C" int func_003A98C8(cWorldView* view, int section, float f);
extern "C" int func_003A9958(cWorldView* view, int section);
extern "C" void func_003A99D8(cWorldView* view, int section);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern int D_00442150[];
extern float D_0044C208[];
extern char D_0047B470[];
extern char D_0047B480[];
extern char D_004A2A18[];

static inline cWorldView* gameWorldView_22D8D8(void* self)
{
    return (cWorldView*)(*(char**)*(char**)(*(char**)((char*)self + 0x1B0) + 0x10) + 0x10);
}

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
        int section = D_00442168[idx].field_0x4;
        switch (D_00442168[idx].field_0x8) {
        case 7:
            if (func_003A9890(gameWorldView_22D8D8(self), section)) {
                D_00442168[idx].field_0x8 = 0;
            } else {
                busy = 1;
            }
            break;
        case 5:
            if (func_003A9958(gameWorldView_22D8D8(self), section)) {
                D_00442168[idx].field_0x8 = 7;
                cGame_sectionUnloading(self->mpGame, section);
            }
            busy = 1;
            break;
        case 6:
            if (cWorldView_isSectionLoaded(gameWorldView_22D8D8(self), section)) {
                D_00442168[idx].field_0x8 = 1;
                func_00230338(self->mpGame, section);
            } else {
                changed = 1;
            }
            break;
        case 8:
            if (cWorldView_isSectionLoaded(gameWorldView_22D8D8(self), section)) {
                D_00442168[idx].field_0x8 = 2;
                func_00230338(self->mpGame, section);
                func_003A99D8(gameWorldView_22D8D8(self), section);
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
            if (func_00144D50(iface, idx) == 4) {
                func_0022DE58(self, idx);
            }
            remove = 1;
            break;
        case 2:
            if (func_00144D50(iface, idx) == 4) {
                func_0022DE58(self, idx);
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
            func_0022E360(self, prev, node);
            node = prev;
        }
        prev = node;
        node = cSectionMan_getNextActiveEvent(self, prev);
    }
    if (!busy && pending) {
        int* n = cSectionMan_getNextActiveEvent(self, 0);
        char* fmt = cWorldView_getNumSections_u(gameWorldView_22D8D8(self)) >= 2 ? D_0047B470 : D_0047B480;
        for (; n != 0; n = cSectionMan_getNextActiveEvent(self, n)) {
            int idx = *n;
            int section = D_00442168[idx].field_0x4;
            sprintf(buf, D_004A2A18, fmt, func_00144D38(iface, idx));
            int* state = &D_00442168[idx].field_0x8;
            if (*state == 3) {
                if (func_003A98C8(gameWorldView_22D8D8(self), section, D_0044C208[D_00442168[idx].field_0xc])) {
                    changed = 1;
                    *state = 6;
                }
            } else if (*state == 4) {
                if (func_003A98C8(gameWorldView_22D8D8(self), section, D_0044C208[D_00442168[idx].field_0xc])) {
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
        func_0022DF50(self);
        self->mModeState = 2;
        break;
    case 2:
        break;
    case 3:
        hold = 1;
        func_0022DF50(self);
        self->mModeState = 4;
        break;
    case 4:
        if (forceOff) {
            hold = 1;
        } else if (func_0022DFD0(self) != 0) {
            func_0022DFF0(self);
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
    func_003A7058(*(char**)*(char**)((char*)self->mpGame + 0x10) + 0x300, D_00442150[self->mStreamRate]);
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022DE58);
#ifdef SKIP_ASM
extern "C" void func_0022D478(cSectionMan* self, int v);

extern "C" void func_0022DE58(cSectionMan* self, int v)
{
    if (self->m_iNextSkySectionID != v) {
        self->m_iNextSkySectionID = v;
        func_0022D478(self, v);
        self->m_bSkySwapPending = 1;
    }
}
#endif

//100%
INCLUDE_ASM("main/game", cSectionMan_skySwap);
#ifdef SKIP_ASM
extern "C" void func_0022D598(cSectionMan* self, int id);
void func_00353D98(void* self);
extern "C" void func_00353CF0(void* self, int id);

extern "C" void cSectionMan_skySwap(cSectionMan* self)
{
    if (self->m_bSkySwapPending == 0) {
        return;
    }
    if (self->m_bTriggerSkySwap == 0) {
        return;
    }
    int i;
    for (i = 0x2C; i < 0x31; i++) {
        if (i != self->m_iNextSkySectionID) {
            func_0022D598(self, i);
        }
    }
    int id = D_00442168[self->m_iNextSkySectionID].field_0x4;
    void* o = *(void**)((char*)self->mpGame + 0x14);
    if (*(int*)((char*)o + 0x10) != id) {
        func_00353D98(o);
        func_00353CF0(*(void**)((char*)self->mpGame + 0x14), id);
    }
    self->m_bTriggerSkySwap = 0;
    self->m_bSkySwapPending = 0;
    self->m_iNextSkySectionID = -1;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022DF50);
#ifdef SKIP_ASM
class cBEIface22DF50 {
public:
    int pad[3];
    virtual void v01();
};
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00231250(void* gm, int state, int a, int b);
void func_00231278(void* gm, int state);
extern int D_00535C08[];
extern int D_004A11B8;

extern "C" void func_0022DF50(cSectionMan* self)
{
    func_00231250(self->mpGame, 0xB, 0, 1);
    cBEIface22DF50* iface = (cBEIface22DF50*)cBE_getInterface_Fv(cBE_getBE(), 0);
    D_00535C08[0] = self->mBELocationID;
    D_004A11B8 = 1;
    iface->v01();
    func_00231278(self->mpGame, 0xA);
}
#endif

extern "C" void* func_002312D8(int);

//100%
INCLUDE_ASM("main/game", func_0022DFD0__FPv);
#ifdef SKIP_ASM
void* func_0022DFD0(void* self)
{
    return func_002312D8(*(int*)((char*)self + 0x1b0));
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022DFF0);
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
extern "C" void func_00231250(void* gm, int state, int a, int b);

extern "C" void func_0022DFF0(cSectionMan* self)
{
    cSectionMan_setSky(self, *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)));
    if (self->mModeReset != 0) {
        self->mModeReset = 0;
        ((cRace22DFF0*)SSXApp->Game->AI)->v05(3);
    }
    func_00231250(self->mpGame, 0xA, 0, 0);
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022E078);
#ifdef SKIP_ASM
extern "C" int func_0022E078(cSectionMan* self, int a1)
{
    int v = D_00442168[a1].field_0x8;
    return (unsigned int)(v - 1) < 2;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022E098);
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

extern "C" int func_0022E098(cSectionMan* self, int a1)
{
    return D_00442168[D_00442488[a1].field_0x10].field_0x4;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022E0C8);
#ifdef SKIP_ASM
extern "C" int func_0022E0C8(cSectionMan* self, int a1)
{
    return D_00442168[a1].field_0x4;
}
#endif

//100%
INCLUDE_ASM("main/game", func_0022E0E0);
#ifdef SKIP_ASM
extern "C" int func_0022E0E0(cSectionMan* self, int a1)
{
    int i;
    for (i = 0; i < 50; i++) {
        if (a1 == D_00442168[i].field_0x4) {
            return i;
        }
    }
    return 50;
}
#endif


//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("main/game", func_0022E118);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0022D088(cSectionMan* self, int a1, int a2);
extern signed char D_00535C11[];

extern "C" void func_0022E118(cSectionMan* self, int a1)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    if (D_00535C11[0] == 0) {
        func_0022D088(self, a1, 1);
        self->mModeReset = 1;
    }
}
#endif
