#include "common.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("visualfx/angerfx", cAngerFX_cAngerFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/angerfx_classes.h"

cAngerFX_ang::cAngerFX_ang()
{
    cAngerPart_ang** slot = &parts;
    *slot = new (D_00487518, 0, 0) cAngerPart_ang[2];
    func_002D4BE0_erFX(this);
}

// PORT: the compiler names this ctor __12cAngerFX_angi; expose it under the address-derived name.
__asm__(".globl cAngerFX_cAngerFX\n.type cAngerFX_cAngerFX,@function\ncAngerFX_cAngerFX = __12cAngerFX_angi\n.size cAngerFX_cAngerFX,0x180\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("__12cRiderVb_agf = cRider_cRider\n"
        "_vt$12cAngerFX_ang$13cRiderVbC_agf = D_00488AC0\n"
        "_vt$12cAngerFX_ang$14cRiderVbBC_agf = D_00459B90\n"
        "_vt$12cAngerFX_ang$12cRiderVb_agf = D_00488B08\n");
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D4A70);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/angerfx_classes.h"

cVbDtor002D4A70::~cVbDtor002D4A70()
{
    if (m04 != 0) cMemMan_free(m04);
}

// PORT: the compiler names this dtor _$_15cVbDtor002D4A70; expose it under the address-derived name.
__asm__(".globl func_002D4A70\n.type func_002D4A70,@function\nfunc_002D4A70 = _$_15cVbDtor002D4A70\n.size func_002D4A70,0x170\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002D4A70$13cRiderVbC_agf = D_00488AC0\n"
        "_vt$15cVbDtor002D4A70$14cRiderVbBC_agf = D_00459B90\n"
        "_vt$15cVbDtor002D4A70$12cRiderVb_agf = D_00488B08\n"
        "_$_12cRiderVb_agf = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D4BE0);
#ifdef SKIP_ASM
extern "C" void* MEM_fill(void* dst, int c, int n);

extern "C" void func_002D4BE0(void* self)
{
    MEM_fill(*(void**)((char*)self + 4), 0, 0x58);
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D4C08);
#ifdef SKIP_ASM
void* cBEAggressionInterface_getThis();
extern "C" int func_00155B50(void* self, int a, int b);
extern char* D_004A28A8;
extern float D_00488EA0[];
extern float D_00488F48[];
extern float D_00488FF0[];
extern float D_00489070[];
extern float D_004890F0[];
extern float D_00489150[];

struct sCol_2D4C08 {
    float r, g, b, a;
};
extern sCol_2D4C08 D_004D5B30;
extern sCol_2D4C08 D_004D5B40;
extern sCol_2D4C08 D_004D5B50;
extern sCol_2D4C08 D_004D5B60;

struct sAngerSlot_2D4C08 {
    float level;            // 0x0
    sCol_2D4C08 col;        // 0x4
    float glowA;            // 0x14
    float glowB;            // 0x18
    int anger;              // 0x1C
    int t20;                // 0x20
    int t24;                // 0x24
    int t28;                // 0x28
};

struct sAnger_2D4C08 {
    char* rider;                    // 0x0
    sAngerSlot_2D4C08* slots;       // 0x4
};

struct sPlayers_2D4C08 {
    int f0;
    char* p[3];             // 0x4
    unsigned int count;     // 0x10
};

struct sVE_2D4C08 {
    short delta;
    short index;
    int (*fn)(void*);
};

static inline int riderV_2D4C08(char* rider, int n)
{
    char* obj = rider + 0x6C0;
    sVE_2D4C08* vt = *(sVE_2D4C08**)obj;
    return vt[n].fn(obj + vt[n].delta);
}

static inline float approach_2D4C08(float x, float target, float step)
{
    if (target + step < x) {
        return x - step;
    }
    if (x < target - step) {
        return x + step;
    }
    return target;
}

static inline sPlayers_2D4C08* players_2D4C08()
{
    return *(sPlayers_2D4C08**)(*(char**)(D_004A28A8 + 0x84) + 0x84);
}

extern "C" void func_002D4C08(sAnger_2D4C08* self)
{
    if (riderV_2D4C08(self->rider, 8) != 0) {
        return;
    }
    for (unsigned int i = 0; i < players_2D4C08()->count; i++) {
        char* g = *(char**)(D_004A28A8 + 0x84);
        char* other = *(char**)(*(char**)(*(char**)((*(sPlayers_2D4C08**)(g + 0x84))->p[i] + 0xA8) + 0x20) + 4);
        if (other == 0) {
            continue;
        }
        sAngerSlot_2D4C08* st = &self->slots[i];
        if (*(int*)(self->rider + 0xAC4) != 0) {
            st->level = 0;
            return;
        }
        int n = **(int**)(g + 0x28);
        int ok = n != 0 && n < 10;
        if (ok) {
            st->level = 0;
            return;
        }
        void* aggr = cBEAggressionInterface_getThis();
        int a = riderV_2D4C08(self->rider, 7);
        int lvl = func_00155B50(aggr, a, riderV_2D4C08(other, 7));
        int old = st->anger;
        st->anger = lvl;
        if (lvl >= 4) {
            st->col = D_004D5B30;
        } else if (lvl >= 3) {
            st->col = D_004D5B40;
        } else if (lvl >= 2) {
            st->col = D_004D5B50;
        } else {
            st->col = D_004D5B60;
        }
        if (lvl >= 2) {
            st->level = approach_2D4C08(st->level, 1.0f, 0.10000000894069672f);
        } else {
            st->level = approach_2D4C08(st->level, 0.0f, 0.06666667014360428f);
        }
        st->col.r = st->level * 0.800000011920929f;
        if (st->t20 > 0 || old != lvl) {
            st->t20++;
            st->t20 %= 41;
        }
        if (st->t24 > 0 || (lvl >= 4 && st->t20 == 0)) {
            st->t24++;
            st->t24 %= 31;
        }
        if (st->t28 > 0 || old < lvl) {
            if (old < lvl) {
                st->t28 = 0;
            }
            st->t28++;
            st->t28 %= 24;
            if (st->t20 > 0 || st->t24 > 0) {
                st->t28 = 0;
            }
        }
        float x = st->level;
        float ga = x * D_00488EA0[st->t20] * D_00488FF0[st->t24] * D_004890F0[st->t28];
        float gb = x * D_00488F48[st->t20] * D_00489070[st->t24] * D_00489150[st->t28];
        st->glowA = ga;
        st->glowB = gb;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D5048);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sV4_2D5048 {
    float x, y, z, w;
    sV4_2D5048() {}
    sV4_2D5048(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sM4_2D5048 {
    sV4_2D5048 v[4];
};

struct sVtx_2D5048 {
    float u, v, q, pad;     // 0x00
    int c[4];               // 0x10
    sV4_2D5048 pos;         // 0x20
    sVtx_2D5048() {}
    void SetC(int r, int g, int b, int a) { c[0] = r; c[1] = g; c[2] = b; c[3] = a; }
};

struct sBones_2D5048 {
    char pad[0x30];
    sM4_2D5048* mats;       // 0x30
};

struct sRState_2D5048 {
    int f0;                 // 0x0
    int flagsA;             // 0x4
    int flagsB;             // 0x8
    int fC;                 // 0xC
    short tex;              // 0x10
    short pad;
};

struct sVE_2D5048 {
    short delta;
    short index;
    void* fn;
};

struct sGfx_2D5048 {
    char pad0[0xE84];
    sRState_2D5048* top;    // 0xE84
    char pad1[0xFAC - 0xE88];
    int texFAC;             // 0xFAC
    char pad2[0x10D8 - 0xFB0];
    sVE_2D5048* vtable;     // 0x10D8
};

struct sViews_2D5048 {
    char pad[0x14];
    int cur;                // 0x14
};

extern char* D_004A289C;
extern sRState_2D5048 D_00501420_2D5048 __asm__("D_00501420");

static inline sViews_2D5048* views_2D5048()
{
    return *(sViews_2D5048**)(*(char**)(D_004A28A8 + 0x84) + 0x84);
}

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline sV4_2D5048 MatVec_2D5048(const sM4_2D5048* m, const sV4_2D5048& v)
{
    sV4_2D5048 r;
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
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sV4_2D5048 Scale_2D5048(const sV4_2D5048& v, float s)
{
    sV4_2D5048 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sV4_2D5048 Add_2D5048(const sV4_2D5048& a, const sV4_2D5048& b)
{
    sV4_2D5048 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV4_2D5048 Sub_2D5048(const sV4_2D5048& a, const sV4_2D5048& b)
{
    sV4_2D5048 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline int rsTexFAC_2D5048(sGfx_2D5048* g)
{
    return g->texFAC;
}

static inline void rsSetA_2D5048(sGfx_2D5048* g, int mask, int shift, int v)
{
    g->top->flagsA = (g->top->flagsA & ~mask) | ((v << shift) & mask);
}

static inline void rsSetB_2D5048(sGfx_2D5048* g, int mask, int shift, int v)
{
    g->top->flagsB = (g->top->flagsB & ~mask) | ((v << shift) & mask);
}

extern "C" void func_002D5048(sAnger_2D4C08* self)
{
    if (riderV_2D4C08(self->rider, 8) != 0) {
        return;
    }
    float fade = 1.0f;
    sAngerSlot_2D4C08* st = &self->slots[views_2D5048()->cur];
    if (st->level == 0.0f) {
        return;
    }
    sGfx_2D5048* g = (sGfx_2D5048*)D_004A289C;
    sM4_2D5048* m = ((sM4_2D5048* (*)(void*))g->vtable[35].fn)((char*)g + g->vtable[35].delta);
    sM4_2D5048* bm = &(*(sBones_2D5048**)(self->rider + 0x780))->mats[*(int*)(self->rider + 0x8A8)];
    sV4_2D5048 p = bm->v[3];
    sV4_2D5048 v = MatVec_2D5048(m, p);
    float z = v.z;
    if (z <= 0.0f || 2000.0f <= z) {
        return;
    }
    float s = z * 0.003f;
    float b = st->glowA + s;
    float a = st->glowB + s;
    if (1500.0f < z) {
        fade = (2000.0f - z) * 0.002f;
    }
    sV4_2D5048 right(m->v[0].x, m->v[1].x, m->v[2].x, 0.0f);
    sV4_2D5048 up(m->v[0].y, m->v[1].y, m->v[2].y, 0.0f);
    sV4_2D5048 c0 = Add_2D5048(p, Scale_2D5048(up, 35.0f));
    sV4_2D5048 c1 = Add_2D5048(c0, Scale_2D5048(up, a * 18.0f));
    sV4_2D5048 rr = Scale_2D5048(right, b * 9.0f);

    sGfx_2D5048* gx = (sGfx_2D5048*)D_004A289C;
    gx->top[1] = gx->top[0];
    gx->top++;
    *((sGfx_2D5048*)D_004A289C)->top = D_00501420_2D5048;
    sGfx_2D5048* gs = (sGfx_2D5048*)D_004A289C;
    rsSetA_2D5048(gs, 0x400000, 22, 1);
    rsSetA_2D5048(gs, 0x3, 0, 2);
    rsSetB_2D5048(gs, 0x1FFFFC00, 10, 0);
    rsSetB_2D5048(gs, 0x3E0, 5, 7);
    gs->top->tex = rsTexFAC_2D5048(gs);

    sCol_2D4C08 col = st->col;
    col.r *= fade;
    sVtx_2D5048 vtx[4];
    {
        sV4_2D5048 t0 = Sub_2D5048(c0, rr);
        vtx[0].SetC((int)(col.g * 255.0f), (int)(col.b * 255.0f), (int)(col.a * 255.0f), (int)(col.r * 128.0f));
        vtx[0].q = 1.0f;
        vtx[0].pos = t0;
        vtx[0].u = 0.0f;
        vtx[0].v = 1.0f;
    }
    {
        sV4_2D5048 t1 = Add_2D5048(c0, rr);
        vtx[1].SetC((int)(col.g * 255.0f), (int)(col.b * 255.0f), (int)(col.a * 255.0f), (int)(col.r * 128.0f));
        vtx[1].q = 1.0f;
        vtx[1].pos = t1;
        vtx[1].u = 1.0f;
        vtx[1].v = 1.0f;
    }
    {
        sV4_2D5048 t2 = Sub_2D5048(c1, rr);
        vtx[2].SetC((int)(col.g * 255.0f), (int)(col.b * 255.0f), (int)(col.a * 255.0f), (int)(col.r * 128.0f));
        vtx[2].q = 1.0f;
        vtx[2].pos = t2;
        vtx[2].u = 0.0f;
        vtx[2].v = 0.0f;
    }
    {
        sV4_2D5048 t3 = Add_2D5048(c1, rr);
        vtx[3].SetC((int)(col.g * 255.0f), (int)(col.b * 255.0f), (int)(col.a * 255.0f), (int)(col.r * 128.0f));
        vtx[3].q = 1.0f;
        vtx[3].pos = t3;
        vtx[3].u = 1.0f;
        vtx[3].v = 0.0f;
    }
    sGfx_2D5048* gd = (sGfx_2D5048*)D_004A289C;
    ((void (*)(void*, int, sVtx_2D5048*, int))gd->vtable[71].fn)((char*)gd + gd->vtable[71].delta, 4, vtx, 0);
    ((sGfx_2D5048*)D_004A289C)->top--;
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D5598);
#ifdef SKIP_ASM
extern "C" void func_002D5598(unsigned char* rgb, float r, float g, float b)
{
    rgb[0] = (int)r;
    rgb[1] = (int)g;
    rgb[2] = (int)b;
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D55C0);
#ifdef SKIP_ASM
extern float D_004A3A38;

// PORT: <? (g++ min operator)
extern "C" void func_002D55C0(unsigned char* p, float r, float g, float b)
{
    float zero = 0.0f;
    float one = 1.0f;
    r *= D_004A3A38;
    g *= D_004A3A38;
    b *= D_004A3A38;
    float cr, cg, cb;
    if (r >= zero) cr = r <? one; else cr = zero;
    if (g >= zero) cg = g <? one; else cg = zero;
    if (b >= zero) cb = b <? one; else cb = zero;
    p[3] = (int)(cr * 255.0f);
    p[4] = (int)(cg * 255.0f);
    p[5] = (int)(cb * 255.0f);
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D5658);
#ifdef SKIP_ASM
extern "C" void func_002D5658(unsigned char* p, float r, float g, float b, float a)
{
    p[6] = (int)(r * 127.0f);
    p[7] = (int)(g * 127.0f);
    p[8] = (int)(b * 127.0f);
    p[9] = (int)(a * 255.0f);
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", tActiveAvalancheNode__tActiveAvalancheNode);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cDynamicColourEmitter_purge(void* p, int a1);
extern "C" void cDynamicColourEmitter__cDynamicColourEmitter(void* p, int a1);

extern "C" void tActiveAvalancheNode__tActiveAvalancheNode(int* self, int flags)
{
    cDynamicColourEmitter_purge((char*)self + 0xD0, 1);
    cDynamicColourEmitter__cDynamicColourEmitter((char*)self + 0xD0, 2);
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D5718);
#ifdef SKIP_ASM
extern "C" int func_002D5718(const void* a, const void* b)
{
    float x = *(const float*)a;
    float y = *(const float*)b;
    if (x < y) {
        return -1;
    }
    if (y < x) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D5750);
#ifdef SKIP_ASM
extern "C" int func_002D5750(const void* a, const void* b)
{
    unsigned short x = *(const unsigned short*)a;
    unsigned short y = *(const unsigned short*)b;
    if (x < y) {
        return -1;
    }
    return y < x;
}
#endif

