#include "common.h"

//100%
INCLUDE_ASM("visualfx/boardsparkfx", cBoardSparkFX_cBoardSparkFX);
#ifdef SKIP_ASM
// g++ 2.95 vtable entry (no thunks): {delta, index, fn}. Vtables are double-aligned.
struct sVtEntBS {
    short delta;
    short index;
    void* fn;
};

struct sVt9_BS {
    sVtEntBS e[9];
} __attribute__((aligned(8)));

struct sVt22_BS {
    sVtEntBS e[22];
} __attribute__((aligned(8)));

extern const sVt9_BS D_00488CB0;
extern const sVt22_BS D_00488CF8;
extern char D_00459B90[];
extern char D_00487588[];
extern char D_00487598[];
extern char D_004875A8[];

extern "C" void cRider_cRider(void* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_003714B8(void* self);
void* func_002F7A68(void* self);
extern "C" void func_0036CCB8(void* self);
extern "C" void func_0036CE00(void* self, float a, float b, float c);
extern "C" void func_0036CE28(void* self, float a, float b);
extern "C" void func_0036D008(void* self);
extern "C" void func_0036D3D8(void* self, float val);
extern "C" void func_00416210(void* dst, int c, int n);
extern "C" void func_002DAA78(void* self);

struct sBsVec4 {
    float x, y, z, w;
    sBsVec4() {}
};

extern "C" void func_0036CBF8(void* sys, int w, int h, float x, float y, float s, sBsVec4* v);

struct sBsIds {
    short id[2];
    sBsIds()
    {
        for (unsigned int i = 0; i < 2; i++) {
            id[i] = -1;
        }
    }
};

struct sBsSlot {
    char data[0x10];
    sBsIds ids;
    sBsSlot() { func_00416210(data, 0, 0x10); }
    void* operator new(unsigned int, void* p) { return p; }
};

struct sBsElem {
    float v[4];
    sBsElem() {}
    void* operator new[](unsigned int, void* p) { return p; }
};

struct sBsParams {
    int mode;           // 0x00
    int f04;            // 0x04
    float f08;          // 0x08
    float f0C;          // 0x0C
    float f10;          // 0x10
    float f14;          // 0x14
    float f18;          // 0x18
    float f1C[13];      // 0x1C
    float f50;          // 0x50
    float f54;          // 0x54
    float f58;          // 0x58
    float f5C;          // 0x5C
    float f60;          // 0x60
    float f64;          // 0x64
    float f68;          // 0x68
    float f6C;          // 0x6C
    float f70;          // 0x70
    float f74;          // 0x74
    float f78;          // 0x78
    float f7C;          // 0x7C
    float f80;          // 0x80
    float f84;          // 0x84
    float f88;          // 0x88
    float f8C;          // 0x8C
    float f90;          // 0x90
    float f94;          // 0x94
    float f98;          // 0x98
    float f9C;          // 0x9C
    float fA0;          // 0xA0
    float fA4[8];       // 0xA4
    int count;          // 0xC4
    int fC8;            // 0xC8
    float fCC;          // 0xCC
    int fD0;            // 0xD0
    float fD4;          // 0xD4
};

struct sBsVE {
    short delta;
    short index;
    void (*fn)(void*, sBsParams*, float);
};

struct sBsEmitter {
    char pad0[0x58];
    float rate;         // 0x58
    char pad5C[0x1F8 - 0x5C];
    sBsVE* vt;          // 0x1F8
};

struct cBoardSparkFX {
    char* vbase;            // 0x00
    char pad04[0x1C];
    sBsEmitter* emitter;    // 0x20
    sBsParams* params;      // 0x24
    void* gfx;              // 0x28
    char pad2C[0x40];
    int active;             // 0x6C
    char pad70[0xC];
    sBsSlot slotA;          // 0x7C
    sBsSlot slotB;          // 0x90
};

// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0xB0): vtable copies with delta fixups when not in charge.
extern "C" void* cBoardSparkFX_cBoardSparkFX(cBoardSparkFX* self, int inChrg)
{
    sVt9_BS t1;
    sVt22_BS t2;
    if (inChrg) {
        self->vbase = (char*)self + 0xB0;
        cRider_cRider((char*)self + 0xB0);
    }
    *(const void**)(self->vbase + 0x6E8) = &D_00488CB0;
    *(void**)(self->vbase + 0x6D0) = D_00459B90;
    *(const void**)(self->vbase + 0x6C0) = &D_00488CF8;
    if (inChrg == 0) {
        int vc;
        t1 = D_00488CB0;
        *(void**)(self->vbase + 0x6E8) = &t1;
        {
            char* vbo = self->vbase - 0xB0;
            vc = (char*)self - vbo;
        }
        t1.e[1].delta = D_00488CB0.e[1].delta + vc;
        t2 = D_00488CF8;
        *(void**)(self->vbase + 0x6C0) = &t2;
        t2.e[1].delta = D_00488CF8.e[1].delta + vc;
    }
    new ((char*)self + 0x2C) sBsElem[3];
    self->active = 1;
    new (&self->slotA) sBsSlot;
    new (&self->slotB) sBsSlot;
    float zero = 0.0f;
    self->emitter = (sBsEmitter*)func_003714B8(cMemMan_alloc(0x210, D_00487588, 0, 0));
    self->params = (sBsParams*)cMemMan_alloc(0xD8, D_00487598, 0, 0);
    self->gfx = func_002F7A68(cMemMan_alloc(0x150, D_004875A8, 0, 0));
    func_0036CCB8(self->gfx);
    sBsVec4 v;
    v.x = zero;
    v.y = zero;
    v.z = -3000.0f;
    v.w = zero;
    func_0036CBF8(self->gfx, 0x1E, 8, 0.4000000059604645f, 0.012000000104308128f, 1.5f, &v);
    func_0036CE00(self->gfx, 7.5f, zero, 7.5f);
    func_0036CE28(self->gfx, 0.10000000149011612f, 0.1940000057220459f);
    func_0036D008(self->gfx);
    func_0036D3D8(self->gfx, zero);
    self->params->mode = 1;
    self->params->f04 = 0;
    self->params->f08 = -1.0f;
    self->params->f0C = 1.0f;
    self->params->f10 = 6.0f;
    self->params->f14 = 4.0f;
    self->params->f18 = 6.0f;
    self->params->f1C[0] = zero;
    self->params->f1C[1] = zero;
    self->params->f1C[2] = zero;
    self->params->f1C[3] = zero;
    self->params->f1C[4] = zero;
    self->params->f1C[5] = zero;
    self->params->f1C[6] = zero;
    self->params->f1C[7] = zero;
    self->params->f1C[8] = zero;
    self->params->f1C[9] = zero;
    self->params->f1C[10] = zero;
    self->params->f1C[11] = zero;
    self->params->f1C[12] = zero;
    self->params->f50 = 600.0f;
    self->params->f54 = 200.0f;
    self->params->f58 = zero;
    self->params->f5C = zero;
    self->params->f60 = zero;
    self->params->f64 = 200.0f;
    self->params->f68 = zero;
    self->params->f6C = zero;
    self->params->f70 = zero;
    self->params->f74 = 100.0f;
    self->params->f78 = zero;
    self->params->f7C = zero;
    self->params->f80 = -800.0f;
    self->params->f84 = 1.0f;
    self->params->f88 = 1.0f;
    self->params->f8C = 1.0f;
    self->params->f90 = 1.0f;
    self->params->f94 = zero;
    self->params->f98 = 0.21559999883174896f;
    self->params->f9C = 0.18000000715255737f;
    self->params->fA0 = 0.13699999451637268f;
    self->params->fA4[0] = zero;
    self->params->fA4[1] = zero;
    self->params->fA4[2] = zero;
    self->params->fA4[3] = zero;
    self->params->fA4[4] = zero;
    self->params->fA4[5] = zero;
    self->params->fA4[6] = zero;
    self->params->fA4[7] = zero;
    self->params->fC8 = 1;
    self->params->fCC = 6.0f;
    self->params->count = 5;
    self->params->fD0 = 8;
    self->params->fD4 = 27.0f;
    self->params->count = 14;
    sBsEmitter* em = self->emitter;
    em->vt[3].fn((char*)em + em->vt[3].delta, self->params, em->rate);
    self->active = 0;
    func_002DAA78(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardsparkfx", func_002DA8E8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardsparkfx_classes.h"

cVbDtor002DA8E8::~cVbDtor002DA8E8()
{
    delete m20;
    operator_delete((int*)m24);
    operator_delete((int*)m28);
}

// PORT: the compiler names this dtor _$_15cVbDtor002DA8E8; expose it under the address-derived name.
__asm__(".globl func_002DA8E8\n.type func_002DA8E8,@function\nfunc_002DA8E8 = _$_15cVbDtor002DA8E8\n.size func_002DA8E8,0x18C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002DA8E8$13cRiderVbC_bsf = D_00488CB0\n"
        "_vt$15cVbDtor002DA8E8$14cRiderVbBC_bsf = D_00459B90\n"
        "_vt$15cVbDtor002DA8E8$12cRiderVb_bsf = D_00488CF8\n"
        "_$_12cRiderVb_bsf = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/boardsparkfx", func_002DAA78);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sV4_AA78 { float x, y, z, w; } __attribute__((aligned(16)));
struct sRS_AA78 {
    int f0;
    int flagsA;     // 0x4
    int flagsB;     // 0x8
    int fC;
    short tex;      // 0x10
    short pad;
};
extern sV4_AA78 D_004FF130;
extern sRS_AA78 D_00501420_AA78 __asm__("D_00501420");
extern char* D_004A289C_AA78 __asm__("D_004A289C");

// Render-state field setters (mask, shift, value), as the unit's other effects write them.
#define RS_SETA_AA78(rs, mask, shift, v) ((rs).flagsA = ((rs).flagsA & ~(mask)) | (((v) << (shift)) & (mask)))
#define RS_SETB_AA78(rs, mask, shift, v) ((rs).flagsB = ((rs).flagsB & ~(mask)) | (((v) << (shift)) & (mask)))

struct sSparkFx_AA78 {
    char pad0[0x10];
    sV4_AA78 pos;           // 0x10
    char pad20[0x50];
    int f70, f74, f78;      // 0x70
    sRS_AA78 rsA;           // 0x7C
    sRS_AA78 rsB;           // 0x90
};

// Spark FX init: two render states copied from the defaults with their mode bits set (the second also sets f0 bits 2-3).
extern "C" void func_002DAA78(void* vself)
{
    sSparkFx_AA78* self = (sSparkFx_AA78*)vself;
    self->pos = D_004FF130;
    self->f70 = 0;
    self->f74 = 0;
    self->f78 = 0;
    self->rsA = D_00501420_AA78;
    RS_SETB_AA78(self->rsA, 0x3E0, 5, 7);
    RS_SETA_AA78(self->rsA, 0x400000, 22, 1);
    RS_SETA_AA78(self->rsA, 0x1800000, 23, 0);
    RS_SETA_AA78(self->rsA, 0x300000, 20, 0);
    RS_SETA_AA78(self->rsA, 0xFF000, 12, 0);
    RS_SETA_AA78(self->rsA, 0x3, 0, 2);
    RS_SETA_AA78(self->rsA, 0x7C, 2, 7);
    int texA = *(int*)(D_004A289C_AA78 + 0xFA8);
    self->rsA.tex = texA;
    self->rsB = D_00501420_AA78;
    RS_SETB_AA78(self->rsB, 0x3E0, 5, 7);
    RS_SETA_AA78(self->rsB, 0x400000, 22, 1);
    RS_SETA_AA78(self->rsB, 0x1800000, 23, 0);
    RS_SETA_AA78(self->rsB, 0x300000, 20, 0);
    RS_SETA_AA78(self->rsB, 0xFF000, 12, 9);
    RS_SETA_AA78(self->rsB, 0x3, 0, 2);
    RS_SETA_AA78(self->rsB, 0x7C, 2, 7);
    int texB = *(int*)(D_004A289C_AA78 + 0xFA8);
    self->rsB.tex = texB;
    self->rsB.f0 |= 0xC;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardsparkfx", func_002DABC8);
#ifdef SKIP_ASM
struct sV4_ABC8 {
    float x, y, z, w;
    sV4_ABC8() {}
    sV4_ABC8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sCol_ABC8 {
    float a, r, g, b;
};

struct sEnt_ABC8 {
    sCol_ABC8 col;              // 0x00
    char pad10[0xE0];
};

struct sEmVE_ABC8 {
    short delta;
    short index;
    void (*fn)(void*, void*, float);
};

struct sEmitter_ABC8 {
    char pad0[0x58];
    float rate;                 // 0x58
    char pad5C[0x1F8 - 0x5C];
    sEmVE_ABC8* vt;             // 0x1F8
};

struct sRiderVE_ABC8 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sSparkFx_ABC8 {
    char* rider;                // 0x00 (cRider*)
    char pad04[0xC];
    sV4_ABC8 pos;               // 0x10
    sEmitter_ABC8* emitter;     // 0x20
    void* params;               // 0x24
    void* gfx;                  // 0x28
    char pad2C[0x4];
    sV4_ABC8 pts[3];            // 0x30
    float size[3];              // 0x60
    int active;                 // 0x6C
    int f70;                    // 0x70
    int f74;                    // 0x74
    int f78;                    // 0x78
};

extern char* D_004A5B64;
extern char* D_004A28A8;
extern int D_004A472C;
extern int D_004A4728;
extern int D_004A3AFC;
extern sEnt_ABC8 D_004FA398[];

extern "C" void func_0036D428(void* self, float val);
int func_0011FE98(void* self);
extern "C" void func_003717C0(void* em, void* pos, void* d, void* vel, int alive, float dt);
extern "C" void func_0036D318(void* self, sV4_ABC8* a, sV4_ABC8* b, sV4_ABC8* c, sV4_ABC8* d, float t);
extern "C" void func_0036D1F0(void* self, sV4_ABC8* a, sV4_ABC8* b, sV4_ABC8* c, sV4_ABC8* d);
extern "C" void func_0036CEF8(void* self, sV4_ABC8* a, sV4_ABC8* b, sV4_ABC8* c);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV4_ABC8 Scale_ABC8(const sV4_ABC8& v, float s)
{
    sV4_ABC8 r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar, into r).
static inline void ScaleTo_ABC8(sV4_ABC8& r, const sV4_ABC8& v, float s)
{
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
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sV4_ABC8 Add_ABC8(const sV4_ABC8& a, const sV4_ABC8& b)
{
    sV4_ABC8 r;
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
static inline sV4_ABC8 Sub_ABC8(const sV4_ABC8& a, const sV4_ABC8& b)
{
    sV4_ABC8 r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_ABC8(const sV4_ABC8& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: the seed is reinterpreted as a float in [1,2) through memory (type pun).
static inline float Rand_ABC8()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

static inline float RandRange_ABC8(float lo, float range)
{
    return Rand_ABC8() * range + lo;
}

static inline sV4_ABC8 BoneRow_ABC8(char* r, int i)
{
    return ((sV4_ABC8*)(*(char**)(*(char**)(r + 0x780) + 0x34) + (*(int*)(r + 0x8A4) << 6)))[i];
}

static inline sV4_ABC8 RiderSide_ABC8(char* rider)
{
    return *(sV4_ABC8*)(rider + 0x1A0);
}

#define MIN_ABC8(a, b) ((a) < (b) ? (a) : (b))

// cBoardSparkFX::update(void) (On Tour name, unverified pairing; callees fit).
// Board spark FX update: emitter direction/colour from the rider's motion, then the glow and spark points.
extern "C" void func_002DABC8(sSparkFx_ABC8* self)
{
    char* rider = self->rider;
    float dt = 1.0f / (float)*(int*)(D_004A5B64 + 0x10);
    char* surfaceData = *(char**)(*(char**)(D_004A28A8 + 0x84) + 0x44) + *(int*)(rider + 0x438) * 0xB0;
    func_0036D428(self->gfx, dt);
    if (self->active) {
        sEmitter_ABC8* em = self->emitter;
        em->vt[3].fn((char*)em + em->vt[3].delta, self->params, em->rate);
        self->active = 1;
    }
    self->f70 = 0;
    self->f74 = 0;
    self->f78 = 0;
    float bspeed = Len_ABC8(*(sV4_ABC8*)(self->rider + 0x1E0));
    bool fastEnough = 277.77777099609375f < bspeed;
    bool isRailSlide = func_0011FE98(self->rider) == 4;
    if (fastEnough) {
        if (*(int*)(surfaceData + 0x8C)) {
            self->f70 = 1;
            if (isRailSlide) {
                self->f74 = 1;
            }
        }
        if ((isRailSlide && *(int*)(surfaceData + 0x88)) || *(int*)(self->rider + 0x438) == 9) {
            self->f78 = 1;
        }
    }
    sV4_ABC8 particlePosition = Add_ABC8(*(sV4_ABC8*)(self->rider + 0x460), Scale_ABC8(*(sV4_ABC8*)(self->rider + 0x1E0), 0.009999999776482582f));
    self->pos = particlePosition;
    if (self->f78) {
        float maxChance = 0.6000000238418579f;
        sV4_ABC8 groundColour;
        if (*(int*)(self->rider + 0x438) == 9) {
            sV4_ABC8 c9;
            c9.x = 0.21559999883174896f;
            c9.y = 0.18000000715255737f;
            c9.z = 0.13699999451637268f;
            c9.w = 1.0f;
            groundColour = c9;
            maxChance = 0.10000000149011612f;
        } else {
            char* sub = self->rider + 0x6C0;
            sRiderVE_ABC8* vt = *(sRiderVE_ABC8**)sub;
            sCol_ABC8 c = D_004FA398[vt[7].fn(sub + vt[7].delta)].col;
            groundColour = sV4_ABC8(c.r, c.g, c.b, c.a);
            ScaleTo_ABC8(groundColour, groundColour, 2.0f);
            groundColour.w = 1.0f;
        }
        sV4_ABC8 sideways = Scale_ABC8(Scale_ABC8(RiderSide_ABC8(self->rider), bspeed), 0.25f);
        sV4_ABC8 particleVelocity = Sub_ABC8(Add_ABC8(Scale_ABC8(*(sV4_ABC8*)(self->rider + 0x1E0), 1.2000000476837158f), sideways),
                                Scale_ABC8(sideways, Rand_ABC8() * 2.0f));
        float chance = MIN_ABC8(bspeed / 1666.666748046875f * 0.10000000149011612f, maxChance);
        bool active = Rand_ABC8() < chance;
        func_003717C0(self->emitter, &particlePosition, &particleVelocity, &groundColour, active, 0.01666666753590107f);
    } else {
        func_003717C0(self->emitter, &particlePosition, 0, 0, 0, 0.01666666753590107f);
    }
    if (self->f70) {
        char* r = self->rider;
        const sV4_ABC8& side = BoneRow_ABC8(r, 2);
        const sV4_ABC8& front = BoneRow_ABC8(r, 0);
        bool uberState = D_004A472C != 0 || D_004A4728 != 0 || 0.0f < *(float*)(r + 0x2F0);
        if (uberState) {
            sV4_ABC8 a, b, c, d;
            a.x = 255.0f; a.y = 0.0f; a.z = 255.0f; a.w = 153.0f;
            b.x = 255.0f; b.y = 0.0f; b.z = 255.0f; b.w = 102.0f;
            c.x = 0.0f; c.y = 0.0f; c.z = 0.0f; c.w = 0.0f;
            d.x = 0.0f; d.y = 0.0f; d.z = 0.0f; d.w = 0.0f;
            func_0036D318(self->gfx, &a, &b, &c, &d, 0.1940000057220459f);
        } else {
            sV4_ABC8 a, b, c, d;
            a.x = 77.0f; a.y = 77.0f; a.z = 255.0f; a.w = 153.0f;
            b.x = 255.0f; b.y = 51.0f; b.z = 51.0f; b.w = 102.0f;
            c.x = 0.0f; c.y = 0.0f; c.z = 0.0f; c.w = 0.0f;
            d.x = 0.0f; d.y = 0.0f; d.z = 0.0f; d.w = 0.0f;
            func_0036D318(self->gfx, &a, &b, &c, &d, 0.1940000057220459f);
        }
        {
            sV4_ABC8 a, b, c, d;
            a.x = 0.0f; a.y = 0.0f; a.z = 400.0f; a.w = 0.0f;
            b.x = 600.0f; b.y = 0.0f; b.z = 0.0f; b.w = 0.0f;
            c.x = 0.0f; c.y = 600.0f; c.z = 0.0f; c.w = 0.0f;
            d.x = 0.0f; d.y = 0.0f; d.z = 600.0f; d.w = 0.0f;
            func_0036D1F0(self->gfx, &a, &b, &c, &d);
        }
        if (isRailSlide) {
            sV4_ABC8 a = Scale_ABC8(side, 10.0f);
            sV4_ABC8 b = Scale_ABC8(front, 10.0f);
            func_0036CEF8(self->gfx, &particlePosition, &a, &b);
        } else {
            sV4_ABC8 a = Scale_ABC8(side, 15.0f);
            sV4_ABC8 b = Scale_ABC8(front, 90.0f);
            func_0036CEF8(self->gfx, &particlePosition, &a, &b);
        }
    }
    if (self->f74 && 0.699999988079071f < Rand_ABC8()) {
        for (int i = 0; i < 3; i++) {
            self->size[i] = RandRange_ABC8(20.0f, 10.0f);
            self->pts[i] = Add_ABC8(self->pos, sV4_ABC8(RandRange_ABC8(-4.0f, 8.0f), RandRange_ABC8(-4.0f, 8.0f),
                                                         RandRange_ABC8(-4.0f, 8.0f), 0.0f));
        }
    } else {
        self->f74 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardsparkfx", func_002DB478);
#ifdef SKIP_ASM
extern "C" void func_00371688(void* self, int a1);

struct sRS_B478 {
    int v[5];
};
struct sRenderCtx_B478 {
    char pad[0xE84];
    sRS_B478* top;
};
extern sRenderCtx_B478* D_004A289C;
extern char* D_004A5B80;

struct sVec2_B478 {
    float x, y;
    sVec2_B478(float ax, float ay) : x(ax), y(ay) {}
};
struct sVec4_B478 {
    float x, y, z, w;
    sVec4_B478(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
};

// Function-local statics `uv0(0,0)`, `uv1(1,1)`, `col(1,1,1,1)` and their init guards, spelled out
// as the globals splat named.
extern float D_004A54D0;
extern float D_004A54D4;
extern int D_004A54D8;
extern float D_004A54E0;
extern float D_004A54E4;
extern int D_004A54E8;
extern int D_004A54EC;
extern sVec4_B478 D_004D57A0;

struct sVE_B478a {
    short delta;
    short index;
    void (*fn)(void*, int);
};
struct sVE_B478b {
    short delta;
    short index;
    void (*fn)(void*, void*, sVec2_B478*, const sVec2_B478*, const sVec2_B478*, const sVec4_B478*);
};

static inline void push_B478(sRenderCtx_B478* ctx)
{
    ctx->top[1] = ctx->top[0];
    ctx->top++;
}

static inline void pop_B478(sRenderCtx_B478* ctx)
{
    ctx->top--;
}

extern "C" void func_002DB478(char* self)
{
    char* o = *(char**)(self + 0x20);
    if (*(int*)(o + 0x1E0) > 0) {
        func_00371688(o, 7);
    }
    if (*(int*)(self + 0x70) != 0) {
        push_B478(D_004A289C);
        *D_004A289C->top = *(sRS_B478*)(self + 0x7C);
        char* ctx = (char*)D_004A289C;
        sVE_B478a* vt = *(sVE_B478a**)(ctx + 0x10D8);
        vt[82].fn(ctx + vt[82].delta, *(int*)(self + 0x28));
        pop_B478(D_004A289C);
    }
    if (*(int*)(self + 0x74) != 0) {
        push_B478(D_004A289C);
        *D_004A289C->top = *(sRS_B478*)(self + 0x90);
        if (D_004A54D8 == 0) {
            D_004A54D8 = 1;
            D_004A54D0 = 0.0f;
            D_004A54D4 = 0.0f;
        }
        if (D_004A54E8 == 0) {
            D_004A54E8 = 1;
            D_004A54E0 = 1.0f;
            D_004A54E4 = 1.0f;
        }
        if (D_004A54EC == 0) {
            D_004A54EC = 1;
            D_004D57A0.x = 1.0f;
            D_004D57A0.y = 1.0f;
            D_004D57A0.z = 1.0f;
            D_004D57A0.w = 1.0f;
        }
        sVec2_B478* uv0 = (sVec2_B478*)&D_004A54D0;
        sVec2_B478* uv1 = (sVec2_B478*)&D_004A54E0;
        for (int i = 0; i < 3; i++) {
            sVec2_B478 size(((float*)(self + 0x60))[i], ((float*)(self + 0x60))[i]);
            char* ctx = D_004A5B80;
            sVE_B478b* vt = *(sVE_B478b**)(ctx + 0x10D8);
            vt[78].fn(ctx + vt[78].delta, self + 0x30 + i * 0x10, &size, uv0, uv1, &D_004D57A0);
        }
        pop_B478(D_004A289C);
    }
}
#endif

