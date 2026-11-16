#include "common.h"
#include "ai/riderhandler.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"

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
extern char str_mEmitter[];
extern char str_mEmitterData[];
extern char str_Board_Spark_Emitter[];

extern "C" void cRider_cRider(void* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cDynamicColourEmitter_cDynamicColourEmitter(void* self);
void* func_002F7A68(void* self);
extern "C" void func_0036CCB8(void* self);
extern "C" void tParticleEmitter_setSize(void* self, float a, float b, float c);
extern "C" void tParticleEmitter_setLife(void* self, float a, float b);
extern "C" void tParticleEmitter_stopMotion(void* self);
extern "C" void tParticleEmitter_setTime(void* self, float val);
extern "C" void func_00416210(void* dst, int c, int n);
extern "C" void cBoardSparkFX_readFromReplayFrame(cBoardSparkFX* self);

struct sBsVec4 {
    float x, y, z, w;
    sBsVec4() {}
};

extern "C" void tParticleEmitter_init(void* sys, int w, int h, float x, float y, float s, sBsVec4* v);

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

struct sBoardSparkFXView {  // this unit's view of cBoardSparkFX (ai/riderhandler.h, where the FX parts are not yet mapped)
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

/**
 * Constructs a rider's board spark effect: a particle emitter for grit, a glow and three spark points.
 *
 * When @p inChrg is set it also constructs the cRider virtual base at +0xB0. It patches the rider's vtables
 * (the g++ 2.95 this-adjust fix-ups when not in charge).
 *
 * It creates the emitter (0x210 bytes), its particle settings (0xD8 bytes: brown grit, probably with gravity
 * -800) and the glow object (0x150 bytes, set up through tParticleEmitter_init and related calls). It hands the
 * settings to the emitter, then resets the render states (cBoardSparkFX_readFromReplayFrame).
 *
 * @param inChrg  nonzero when this object is complete and must construct its virtual base
 * @return self
 */
// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0xB0): vtable copies with delta fixups when not in charge.
extern "C" void* cBoardSparkFX_cBoardSparkFX(sBoardSparkFXView* self, int inChrg)
{
    sVt9_BS vtbl1;
    sVt22_BS vtbl2;
    if (inChrg) {
        self->vbase = (char*)self + 0xB0;
        cRider_cRider((char*)self + 0xB0);
    }
    *(const void**)(self->vbase + 0x6E8) = &D_00488CB0;
    *(void**)(self->vbase + 0x6D0) = D_00459B90;
    *(const void**)(self->vbase + 0x6C0) = &D_00488CF8;
    if (inChrg == 0) {
        int vc;
        vtbl1 = D_00488CB0;
        *(void**)(self->vbase + 0x6E8) = &vtbl1;
        {
            char* vbo = self->vbase - 0xB0;
            vc = (char*)self - vbo;
        }
        vtbl1.e[1].delta = D_00488CB0.e[1].delta + vc;
        vtbl2 = D_00488CF8;
        *(void**)(self->vbase + 0x6C0) = &vtbl2;
        vtbl2.e[1].delta = D_00488CF8.e[1].delta + vc;
    }
    new ((char*)self + 0x2C) sBsElem[3];
    self->active = 1;
    new (&self->slotA) sBsSlot;
    new (&self->slotB) sBsSlot;
    float zeroF = 0.0f;
    self->emitter = (sBsEmitter*)cDynamicColourEmitter_cDynamicColourEmitter(cMemMan_alloc(0x210, str_mEmitter, 0, 0));
    self->params = (sBsParams*)cMemMan_alloc(0xD8, str_mEmitterData, 0, 0);
    self->gfx = func_002F7A68(cMemMan_alloc(0x150, str_Board_Spark_Emitter, 0, 0));
    func_0036CCB8(self->gfx);
    sBsVec4 force;
    force.x = zeroF;
    force.y = zeroF;
    force.z = -3000.0f;
    force.w = zeroF;
    tParticleEmitter_init(self->gfx, 0x1E, 8, 0.4000000059604645f, 0.012000000104308128f, 1.5f, &force);
    tParticleEmitter_setSize(self->gfx, 7.5f, zeroF, 7.5f);
    tParticleEmitter_setLife(self->gfx, 0.10000000149011612f, 0.1940000057220459f);
    tParticleEmitter_stopMotion(self->gfx);
    tParticleEmitter_setTime(self->gfx, zeroF);
    self->params->mode = 1;
    self->params->f04 = 0;
    self->params->f08 = -1.0f;
    self->params->f0C = 1.0f;
    self->params->f10 = 6.0f;
    self->params->f14 = 4.0f;
    self->params->f18 = 6.0f;
    self->params->f1C[0] = zeroF;
    self->params->f1C[1] = zeroF;
    self->params->f1C[2] = zeroF;
    self->params->f1C[3] = zeroF;
    self->params->f1C[4] = zeroF;
    self->params->f1C[5] = zeroF;
    self->params->f1C[6] = zeroF;
    self->params->f1C[7] = zeroF;
    self->params->f1C[8] = zeroF;
    self->params->f1C[9] = zeroF;
    self->params->f1C[10] = zeroF;
    self->params->f1C[11] = zeroF;
    self->params->f1C[12] = zeroF;
    self->params->f50 = 600.0f;
    self->params->f54 = 200.0f;
    self->params->f58 = zeroF;
    self->params->f5C = zeroF;
    self->params->f60 = zeroF;
    self->params->f64 = 200.0f;
    self->params->f68 = zeroF;
    self->params->f6C = zeroF;
    self->params->f70 = zeroF;
    self->params->f74 = 100.0f;
    self->params->f78 = zeroF;
    self->params->f7C = zeroF;
    self->params->f80 = -800.0f;
    self->params->f84 = 1.0f;
    self->params->f88 = 1.0f;
    self->params->f8C = 1.0f;
    self->params->f90 = 1.0f;
    self->params->f94 = zeroF;
    self->params->f98 = 0.21559999883174896f;
    self->params->f9C = 0.18000000715255737f;
    self->params->fA0 = 0.13699999451637268f;
    self->params->fA4[0] = zeroF;
    self->params->fA4[1] = zeroF;
    self->params->fA4[2] = zeroF;
    self->params->fA4[3] = zeroF;
    self->params->fA4[4] = zeroF;
    self->params->fA4[5] = zeroF;
    self->params->fA4[6] = zeroF;
    self->params->fA4[7] = zeroF;
    self->params->fC8 = 1;
    self->params->fCC = 6.0f;
    self->params->count = 5;
    self->params->fD0 = 8;
    self->params->fD4 = 27.0f;
    self->params->count = 14;
    sBsEmitter* em = self->emitter;
    em->vt[3].fn((char*)em + em->vt[3].delta, self->params, em->rate);
    self->active = 0;
    cBoardSparkFX_readFromReplayFrame((cBoardSparkFX*)self);
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardsparkfx", cBoardSparkFX__cBoardSparkFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardsparkfx_classes.h"

/**
 * Destroys a board spark effect's parts: the emitter (+0x20), the particle settings (+0x24) and the glow
 * (+0x28).
 */
cVbDtor002DA8E8::~cVbDtor002DA8E8()
{
    delete m20;
    operator_delete((int*)m24);
    operator_delete((int*)m28);
}

// PORT: the compiler names this dtor _$_15cVbDtor002DA8E8; expose it under the address-derived name.
__asm__(".globl cBoardSparkFX__cBoardSparkFX\n.type cBoardSparkFX__cBoardSparkFX,@function\ncBoardSparkFX__cBoardSparkFX = _$_15cVbDtor002DA8E8\n.size cBoardSparkFX__cBoardSparkFX,0x18C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002DA8E8$13cRiderVbC_bsf = D_00488CB0\n"
        "_vt$15cVbDtor002DA8E8$14cRiderVbBC_bsf = D_00459B90\n"
        "_vt$15cVbDtor002DA8E8$12cRiderVb_bsf = D_00488CF8\n"
        "_$_12cRiderVb_bsf = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/boardsparkfx", cBoardSparkFX_readFromReplayFrame);
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
extern sV4_AA78 t4vect_Origin;
extern sRS_AA78 D_00501420_AA78 __asm__("sDrawState_s_defaultDrawState");

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

/**
 * Resets a board spark effect; despite the name it reads no replay data (it takes no frame).
 *
 * Clears the position and flags and builds the two additive render states (glow and sparks, effect texture
 * 22).
 *
 * @param vself  the spark effect
 */
// Spark FX init: two render states copied from the defaults with their mode bits set (the second also sets f0 bits 2-3).
extern "C" void cBoardSparkFX_readFromReplayFrame(cBoardSparkFX* vself)
{
    sSparkFx_AA78* self = (sSparkFx_AA78*)vself;
    self->pos = t4vect_Origin;
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
    int texA = GrMan->_EffectTexMap[22];
    self->rsA.tex = texA;
    self->rsB = D_00501420_AA78;
    RS_SETB_AA78(self->rsB, 0x3E0, 5, 7);
    RS_SETA_AA78(self->rsB, 0x400000, 22, 1);
    RS_SETA_AA78(self->rsB, 0x1800000, 23, 0);
    RS_SETA_AA78(self->rsB, 0x300000, 20, 0);
    RS_SETA_AA78(self->rsB, 0xFF000, 12, 9);
    RS_SETA_AA78(self->rsB, 0x3, 0, 2);
    RS_SETA_AA78(self->rsB, 0x7C, 2, 7);
    int texB = GrMan->_EffectTexMap[22];
    self->rsB.tex = texB;
    self->rsB.f0 |= 0xC;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardsparkfx", cBoardSparkFX_update);
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

extern char* cAppMan_TheAppMan;
extern int cPowerUpFXToggles_m_SuperUberStateTest;
extern int cPowerUpFXToggles_m_UberStateTest;
extern int D_004A3AFC;
// cRenderStateMan_m_state: the lens/surface table, 8 rows of 0xF0 bytes (config size 0x780); sEnt_ABC8 views a row from its +0x28 field
extern char cRenderStateMan_m_state[];

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

/**
 * Updates a rider's board sparks: grit particles, the glow under the board and random spark points.
 *
 * Above a speed of about 278 the surface decides: a surface with glow (+0x8C) lights the glow, and while rail
 * sliding also the sparks. Grit is thrown on surface type 9 (brown) or on a rail whose surface has grit
 * (+0x88), with a chance that grows with speed. The glow is magenta in the uber states, otherwise blue and
 * red.
 */
// sBoardSparkFXView::update(void) (On Tour name, unverified pairing; callees fit).
// Board spark FX update: emitter direction/colour from the rider's motion, then the glow and spark points.
extern "C" void cBoardSparkFX_update(sSparkFx_ABC8* self)
{
    char* rider = self->rider;
    float dt = 1.0f / (float)*(int*)(cAppMan_TheAppMan + 0x10);
    char* surfaceData = (char*)SSXApp->Game->SurfPropCache + *(int*)(rider + 0x438) * 0xB0;
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
            sV4_ABC8 presetColour;
            presetColour.x = 0.21559999883174896f;
            presetColour.y = 0.18000000715255737f;
            presetColour.z = 0.13699999451637268f;
            presetColour.w = 1.0f;
            groundColour = presetColour;
            maxChance = 0.10000000149011612f;
        } else {
            char* sub = self->rider + 0x6C0;
            sRiderVE_ABC8* vt = *(sRiderVE_ABC8**)sub;
            sCol_ABC8 c = ((sEnt_ABC8*)(cRenderStateMan_m_state + 0x28))[vt[7].fn(sub + vt[7].delta)].col;
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
        char* riderObj = self->rider;
        const sV4_ABC8& side = BoneRow_ABC8(riderObj, 2);
        const sV4_ABC8& front = BoneRow_ABC8(riderObj, 0);
        bool uberState = cPowerUpFXToggles_m_SuperUberStateTest != 0 || cPowerUpFXToggles_m_UberStateTest != 0 || 0.0f < *(float*)(riderObj + 0x2F0);
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
INCLUDE_ASM("visualfx/boardsparkfx", cBoardSparkFX_render);
#ifdef SKIP_ASM
extern "C" void func_00371688(void* self, int a1);

struct sRS_B478 {
    int v[5];
};
struct sRenderCtx_B478 {
    char pad[0xE84];
    sRS_B478* top;
};

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

/**
 * Draws a rider's board sparks: the emitter's particles, the glow (graphics slot 82) and up to three spark
 * sprites.
 *
 * The sprites use the second render state and a full white UV quad (statics set up on first use).
 */
extern "C" void cBoardSparkFX_render(cBoardSparkFX* self)
{
    char* gfx = *(char**)(((char*)self) + 0x20);
    if (*(int*)(gfx + 0x1E0) > 0) {
        func_00371688(gfx, 7);
    }
    if (*(int*)(((char*)self) + 0x70) != 0) {
        push_B478((sRenderCtx_B478*)GrMan);
        *(sRS_B478*)GrMan->m_pCurrentDrawState = *(sRS_B478*)(((char*)self) + 0x7C);
        cPS2GraphicsMan* ctx = GrMan;
        sVE_B478a* vt = (sVE_B478a*)ctx->mVtbl;
        vt[82].fn((char*)ctx + vt[82].delta, *(int*)(((char*)self) + 0x28));
        pop_B478((sRenderCtx_B478*)GrMan);
    }
    if (*(int*)(((char*)self) + 0x74) != 0) {
        push_B478((sRenderCtx_B478*)GrMan);
        *(sRS_B478*)GrMan->m_pCurrentDrawState = *(sRS_B478*)(((char*)self) + 0x90);
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
            sVec2_B478 size(((float*)(((char*)self) + 0x60))[i], ((float*)(((char*)self) + 0x60))[i]);
            cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
            sVE_B478b* vt = (sVE_B478b*)ctx->mVtbl;
            vt[78].fn((char*)ctx + vt[78].delta, ((char*)self) + 0x30 + i * 0x10, &size, uv0, uv1, &D_004D57A0);
        }
        pop_B478((sRenderCtx_B478*)GrMan);
    }
}
#endif

