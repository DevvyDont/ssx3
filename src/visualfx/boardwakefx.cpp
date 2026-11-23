#include "vtable.h"
#include "common.h"
#include "render/2drender.h"
#include "camera/camera.h"
#include "util/surfprop.h"
#include "render/particle.h"
#include "ui/uithing.h"
#include "main/gameviewman.h"
#include "main/sectionman.h"
#include "bx/appman.h"
#include "visualfx/boardwakefx.h"
#include "visualfx/snowfall.h"
#include "visualfx/lensfx.h"
#include "visualfx/transitionman.h"
#include "be/beintnewrace.h"
#include "visualfx/snowbox.h"
#include "ai/riderhandler.h"
#include "render/ps2graphicsman.h"
#include "be/beintnetwork.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"
#include "ai/ai.h"

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_cBoardWakeFX);
#ifdef SKIP_ASM
// g++ 2.95 vtable entry (no thunks): {delta, index, fn}. Vtables are double-aligned.
struct sVtEntBW {
    short delta;
    short index;
    void* fn;
};

struct sVt9_BW {
    sVtEntBW e[9];
} __attribute__((aligned(8)));

struct sVt22_BW {
    sVtEntBW e[22];
} __attribute__((aligned(8)));

extern const sVt9_BW D_00488DA8;
extern const sVt22_BW D_00488DF0;
extern char D_00459B90[];
extern char str_WakeLines_cBoardWakeFX[];
extern char str_WakeVerts_cBoardWakeFX[];
extern "C" void cRider_cRider(void* self);
struct sWakeDCF28;
extern "C" void cBoardWakeFX_reset(sWakeDCF28* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");

struct sWakePtBW {
    float v[4];
    sWakePtBW() {}
};

struct sWakeStripBW {
    int hdr[8];
    sWakePtBW pts[5];
    sWakeStripBW() {}
};

struct sWakeElemBW {
    char data[0x30];
    sWakeElemBW() {}
};

/**
 * Constructs a rider's board wake effect: 32 wake segments and five rows of 32 points, then resets it.
 *
 * When @p inChrg is set it also constructs the cRider virtual base at +0xC0. It patches the rider's vtables
 * (the g++ 2.95 this-adjust fix-ups when not in charge).
 *
 * The arrays are "WakeLines_cBoardWakeFX" (+0xC) and "WakeVerts_cBoardWakeFX" (one block of 160, split into
 * the five rows at +0x10). +0xB4 starts at -1 and the reset (cBoardWakeFX_reset) sets the
 * sizes.
 *
 * @param inChrg  nonzero when this object is complete and must construct its virtual base
 * @return self
 */
// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0xC0): vtable copies with delta fixups when not in charge.
extern "C" void* cBoardWakeFX_cBoardWakeFX(cBoardWakeFX* self, int inChrg)
{
    sVt9_BW vtbl1;
    sVt22_BW vtbl2;
    if (inChrg) {
        *(void**)self = (char*)self + 0xC0;
        cRider_cRider((char*)self + 0xC0);
    }
    *(void**)(*(char**)self + 0x6E8) = (void*)&D_00488DA8;
    *(void**)(*(char**)self + 0x6D0) = D_00459B90;
    *(void**)(*(char**)self + 0x6C0) = (void*)&D_00488DF0;
    if (inChrg == 0) {
        int vc;
        vtbl1 = D_00488DA8;
        *(void**)(*(char**)self + 0x6E8) = &vtbl1;
        {
            char* vbo = *(char**)self - 0xC0;
            vc = (char*)self - vbo;
        }
        vtbl1.e[1].delta = D_00488DA8.e[1].delta + vc;
        vtbl2 = D_00488DF0;
        *(void**)(*(char**)self + 0x6C0) = &vtbl2;
        vtbl2.e[1].delta = D_00488DF0.e[1].delta + vc;
    }
    sWakeStripBW*& strips = (sWakeStripBW*)self->mWakeLines;
    strips = new (str_WakeLines_cBoardWakeFX, 0, 0) sWakeStripBW[32];
    sWakeElemBW** rows = (sWakeElemBW**)(&self->mWakeVerts[0]);
    rows[0] = new (str_WakeVerts_cBoardWakeFX, 0, 0) sWakeElemBW[160];
    for (int i = 1; i < 5; i++) {
        rows[i] = (sWakeElemBW*)self->mWakeVerts[0] + i * 32;
    }
    self->mWakeLineSegs = -1;
    cBoardWakeFX_reset((sWakeDCF28*)self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX__cBoardWakeFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardwakefx_classes.h"

/**
 * Destroys a board wake effect's arrays: frees the wake segments (+0xC) and the point rows (+0x10).
 */
cVbDtor002DCDA0::~cVbDtor002DCDA0()
{
    if (m0C != 0) cMemMan_free(m0C);
    if (m10 != 0) cMemMan_free(m10);
}

// PORT: the compiler names this dtor _$_15cVbDtor002DCDA0; expose it under the address-derived name.
__asm__(".globl cBoardWakeFX__cBoardWakeFX\n.type cBoardWakeFX__cBoardWakeFX,@function\ncBoardWakeFX__cBoardWakeFX = _$_15cVbDtor002DCDA0\n.size cBoardWakeFX__cBoardWakeFX,0x184\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002DCDA0$13cRiderVbC_bwf = D_00488DA8\n"
        "_vt$15cVbDtor002DCDA0$14cRiderVbBC_bwf = D_00459B90\n"
        "_vt$15cVbDtor002DCDA0$12cRiderVb_bwf = D_00488DF0\n"
        "_$_12cRiderVb_bwf = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_reset);
#ifdef SKIP_ASM
struct sWakeElemDCF28 {
    int a;
    float b;
    float c;
    int d;
    char pad10[0x20];
};

struct sWakeColDCF28 {
    float r, g, b, a;
};

struct sWakeDCF28 {
    char* obj;                    // 0x00
    int f4;                       // 0x04
    int f8;                       // 0x08
    int fC;                       // 0x0C
    sWakeElemDCF28* elems[5];     // 0x10
    float vals[5];                // 0x24
    int f38;                      // 0x38
    int f3C;                      // 0x3C
    int f40;                      // 0x40
    char pad44[0x1C];             // 0x44
    char sub60[0x40];             // 0x60
    sWakeColDCF28 col;            // 0xA0
    int fB0;                      // 0xB0
    int count;                    // 0xB4
    int fB8;                      // 0xB8
    float fBC;                    // 0xBC
};

extern "C" void cWakeParams_Clear(cWakeParams* self);

/**
 * Resets a board wake effect; despite the name it reads no replay data (it takes no frame, like On Tour's
 * reset).
 *
 * A rider with a view (a player) gets 5 rows, 32 segments and a 1.25 s life; others get 4 rows, 22 segments
 * and 0.8 s. It sets the colour to white and clears the state. When the row count changed, each row's points
 * get a height factor and the rows' sink speeds (0 down to -25) are recomputed.
 */
extern "C" void cBoardWakeFX_reset(sWakeDCF28* self)
{
    int old = self->count;
    float one;
    sWakeColDCF28 white;
    int i;
    if (*(int*)(self->obj + 0x870) >= 0) {
        self->count = 5;
        self->fB8 = 0x20;
        self->fBC = 1.25f;
    } else {
        self->count = 4;
        self->fB8 = 22;
        self->fBC = 0.800000011920929f;
    }
    one = 1.0f;
    white.r = one;
    white.g = one;
    white.b = one;
    white.a = one;
    self->col = white;
    self->f38 = 0;
    self->f4 = 0;
    self->f8 = 0;
    self->f3C = 0;
    self->f40 = 0;
    cWakeParams_Clear((cWakeParams*)self->sub60);
    if (old != self->count) {
        for (i = 0; i < 5; i++) {
            float heightFactor = 0.9900000095367432f - (float)i * (0.9800000190734863f / (float)(self->count - 1));
            int j;
            for (j = 0; j < 32; j++) {
                self->elems[i][j].a = 0;
                self->elems[i][j].b = heightFactor;
                self->elems[i][j].c = 1.0f;
                self->elems[i][j].d = 0;
            }
            self->vals[i] = (float)i / (float)(self->count - 1) * -25.000001907348633f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_update);
#ifdef SKIP_ASM
struct sWakeElemD0B8 {
    t4Vector pts[5];            // 0x00
    t4Vector dir;               // 0x50
    float width;                // 0x60
    float age;                  // 0x64
    char pad68[0x8];            // 0x68
};

struct sWakeRingD0B8 {
    char pad0[0x20];            // 0x00
    t4Vector pos;               // 0x20
};

struct sWakeSurfD0B8 {
    char pad0[0x48];            // 0x00
    float f48;                  // 0x48
    float f4C;                  // 0x4C
    char pad50[0x34];           // 0x50
    int i84;                    // 0x84
    char pad88[0x28];           // 0x88
};

struct sWakeD0B8 {
    cRider* rider;              // 0x00
    int count;                  // 0x04
    int head;                   // 0x08
    sWakeElemD0B8* elems;       // 0x0C
    sWakeRingD0B8* rings[5];    // 0x10
    float vals[5];              // 0x24
    float phase;                // 0x38
    int active;                 // 0x3C
    int frame;                  // 0x40
    char pad44[0xC];            // 0x44
    t4Vector v50;               // 0x50
    t4Vector v60;               // 0x60
    t4Vector v70;               // 0x70
    t4Vector v80;               // 0x80
    float f90;                  // 0x90
    float f94;                  // 0x94
    char pad98[0x8];            // 0x98
    t4Vector col;               // 0xA0
    int fB0;                    // 0xB0
    int n;                      // 0xB4
    int max;                    // 0xB8
    float life;                 // 0xBC
};

struct sWakeParamsD0B8 {
    float strength;
    float grip;
};

int cRider_getMotionState(void*);
extern "C" void cBoardWakeFX_createSeg_D0B8(void* self, void* surf, t4Vector* pos) __asm__("cBoardWakeFX_createSeg");
extern "C" void cBoardWakeFX_fillCurrentSeg_D0B8(void* self, t4Vector* pos, t4Vector* fwd, float width) __asm__("cBoardWakeFX_fillCurrentSeg");

#define WAKE_MIN_D0B8(a, b) ((a) < (b) ? (a) : (b))
#define WAKE_MAX_D0B8(a, b) ((a) >= (b) ? (a) : (b))

// PORT: PS2-only inline asm (float absolute value).
static inline float wakeAbsD0B8(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector wakeScaleD0B8(const t4Vector& v, float s)
{
    t4Vector r;
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
static inline t4Vector wakeAddD0B8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline t4Vector wakeSubD0B8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void wakeAddEqD0B8(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector wakeCrossD0B8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wakeLenD0B8(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (v / |v| via vrsqrt).
static inline t4Vector wakeNormD0B8(const t4Vector& v)
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
static inline float wakeDotD0B8(const t4Vector& a, const t4Vector& b)
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

// PORT: g++ minimum operator (<?).
static inline float wakeClampD0B8(float v, float lo, float hi)
{
    float r;
    if (v >= lo) {
        r = v <? hi;
    } else {
        r = lo;
    }
    return r;
}

static inline t4Vector wakeRow34D0B8(cRider* rider, int k)
{
    return *(t4Vector*)((char*)rider->mAnimModel->SkelMatricesScaled + (rider->mBoneBoardChild << 6) + k * 16);
}

static inline t4Vector wakeRowD0B8(cRider* rider, int k)
{
    return *(t4Vector*)((char*)rider->mAnimModel->SkelMatrices + (rider->mBoneBoardChild << 6) + k * 16);
}

static inline float wakeMaxSpeedD0B8()
{
    return 1666.666748046875f;
}

static inline float wakeMinStrengthD0B8()
{
    return 416.66668701171875f;
}

/**
 * Advances a rider's board wake by one frame: ages and spreads the segments, then starts or extends the wake.
 *
 * Segments older than the life are dropped; the rest drift outwards and their lower rows sink. The new wake
 * strength comes from the rider's speed, lean and sideways slip and the surface's wake scale (+0x4C). There
 * is no wake off the ground, at a low strength, at a small lean or on a surface with little grip (+0x48). A
 * running wake adds a segment every 50 units (createSeg, cBoardWakeFX_createSeg) and refreshes the current one
 * (fillCurrentSeg, cBoardWakeFX_fillCurrentSeg). It ends when the grip or strength falls too low or the lean changes side.
 */
// PORT: g++ minimum/maximum operators (<?, >?).
extern "C" void cBoardWakeFX_update(sWakeD0B8* self)
{
    sWakeSurfD0B8* surf = (sWakeSurfD0B8*)((char*)SSXApp->Game->SurfPropCache + self->rider->mSurfaceType * 0xB0);
    int i;
    for (i = 0; i < self->count; i++) {
        int idx = (self->head + i) % self->max;
        sWakeElemD0B8* elem = &self->elems[idx];
        elem->age += 0.01666666753590107f;
        if (self->life < elem->age) {
            self->count = i;
            goto done;
        }
        float grow = ((1.0f - ((elem->age * 20.0f) <? 1.0f)) * elem->width + 1.0f) * 0.01666666753590107f;
        float fade = (1.0f - elem->age * 3.3333332538604736f) >? 0.0f;
        t4Vector drift = wakeScaleD0B8(wakeScaleD0B8(elem->dir, fade), 0.01666666753590107f);
        wakeAddEqD0B8(self->rings[0][idx].pos, drift);
        for (int j = 1; j < self->n; j++) {
            wakeAddEqD0B8(self->rings[j][idx].pos, wakeAddD0B8(wakeScaleD0B8(elem->pts[j], grow), drift));
            elem->pts[j].z += self->vals[j];
        }
    }
done:
    float lean = -self->rider->mSlewed[0].cur;
    float k = 1.0f - wakeAbsD0B8(self->rider->mSlewed[3].cur);
    t4Vector normal = self->rider->mGroundNormal;
    float amt = wakeAbsD0B8(lean) * k;
    t4Vector side;
    t4Vector fwd;
    sWakeParamsD0B8 curParams;
    fwd = wakeRow34D0B8(self->rider, 0);
    if (self->rider->IsRidingFakie != 0) {
        fwd = wakeScaleD0B8(fwd, -1.0f);
    }
    t4Vector right = wakeCrossD0B8(fwd, normal);
    side = lean < 0.0f ? wakeScaleD0B8(right, -1.0f) : right;
    float speed = WAKE_MAX_D0B8(WAKE_MIN_D0B8(wakeMaxSpeedD0B8(), wakeLenD0B8(self->rider->mVelocity)), 0.0f);
    curParams.grip = surf->f48;
    float sign = 1.0f;
    if (self->rider->IsRidingFakie != 0) {
        sign = -1.0f;
    }
    float off = wakeAbsD0B8(self->rider->mSlewed[2].cur) * 118.0f + -40.0f;
    t4Vector pos = wakeAddD0B8(
        wakeAddD0B8(wakeRowD0B8(self->rider, 3), wakeScaleD0B8(wakeScaleD0B8(wakeRowD0B8(self->rider, 0), off), sign)),
        wakeScaleD0B8(wakeScaleD0B8(wakeScaleD0B8(wakeRowD0B8(self->rider, 2), 15.0f), lean < 0.0f ? -1.0f : 1.0f), sign));
    t4Vector dirn = wakeNormD0B8(self->rider->mVelocity);
    float bal = (wakeAbsD0B8(wakeDotD0B8(self->rider->mGroundSide, dirn)) * 0.8f + amt * 0.5f) <? 1.0f;
    float baseStrength = wakeClampD0B8(bal + bal, 0.0f, 0.25f);
    float speedBonus = 0.0f;
    if (555.5555419921875f < speed) {
        speedBonus = wakeClampD0B8((bal - 0.25f) * 2.7f, 0.0f, 0.35f);
    }
    float width = 0.0f;
    if (surf->i84 != 0 && 1111.111083984375f < speed) {
        width = wakeClampD0B8((bal - 0.54f) * 35.0f, 0.0f, 4.5f);
    }
    curParams.strength = speed * (baseStrength + speedBonus) * surf->f4C;
    int ok = 1;
    if (cRider_getMotionState(self->rider) != 0 || wakeAbsD0B8(self->rider->mSlewed[11].cur) > 0.15f ||
        wakeMinStrengthD0B8() > curParams.strength || amt < 0.15f || curParams.grip < 0.05f) {
        curParams.strength = 0.0f;
        ok = 0;
    }
    if (self->active != 0) {
        if (self->fB0 != 0 ? lean < 0.0f : lean > 0.0f) {
            curParams.strength = 0.0f;
            self->f90 = 0.0f;
        }
        if (cRider_getMotionState(self->rider) == 0) {
            self->v60 = wakeAddD0B8(wakeScaleD0B8(self->v60, 0.8f), wakeScaleD0B8(normal, 0.19999998807907104f));
            self->v70 = wakeAddD0B8(wakeScaleD0B8(self->v70, 0.8f), wakeScaleD0B8(side, 0.19999998807907104f));
            self->v80 = wakeAddD0B8(wakeScaleD0B8(self->v80, 0.8f), wakeScaleD0B8(fwd, 0.19999998807907104f));
            self->f94 = self->f94 * 0.7f + curParams.grip * 0.3f;
            self->f90 = self->f90 * 0.8f + curParams.strength * 0.19999998807907104f;
            if (self->f94 > 0.05f && self->f90 > 55.55555725097656f) {
                if (wakeLenD0B8(wakeSubD0B8(pos, self->v50)) > 50.0f) {
                    cBoardWakeFX_createSeg_D0B8(self, surf, &pos);
                    self->v50 = pos;
                }
                cBoardWakeFX_fillCurrentSeg_D0B8(self, &pos, &fwd, width);
            } else {
                self->f94 = 0.0f;
                self->active = 0;
                cBoardWakeFX_fillCurrentSeg_D0B8(self, &pos, &fwd, self->f94);
            }
        } else {
            self->f94 = 0.0f;
            self->active = 0;
            cBoardWakeFX_fillCurrentSeg_D0B8(self, &pos, &fwd, self->f94);
        }
    } else if (ok) {
        self->f90 = 0.0f;
        width = self->f90;
        self->v80 = fwd;
        self->v70 = side;
        self->v60 = normal;
        self->v50 = pos;
        self->frame = 0;
        self->fB0 = width < lean;
        self->f94 = width;
        cBoardWakeFX_createSeg_D0B8(self, surf, &pos);
        cBoardWakeFX_fillCurrentSeg_D0B8(self, &pos, &fwd, width);
        self->f94 = curParams.grip;
        cBoardWakeFX_createSeg_D0B8(self, surf, &pos);
        cBoardWakeFX_fillCurrentSeg_D0B8(self, &pos, &fwd, width);
        self->active = 1;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_render);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char* GrMan_boardwakefx __asm__("GrMan");
extern int cRenderToggles_m_DisableBoardWake;

struct sGs_DAB8 {
    unsigned int f0_0 : 2;
    unsigned int f0_2 : 2;
    unsigned int f0_4 : 28;
    unsigned int f4_0 : 2;
    unsigned int f4_2 : 5;
    unsigned int f4_7 : 5;
    unsigned int f4_12 : 8;
    unsigned int f4_20 : 2;
    unsigned int f4_22 : 1;
    unsigned int f4_23 : 2;
    unsigned int f4_25 : 7;
    unsigned int f8_0 : 5;
    unsigned int f8_5 : 5;
    unsigned int f8_10 : 19;
    unsigned int f8_29 : 3;
    unsigned int wC;
    unsigned int f10 : 16;
    unsigned int f10hi : 16;
    void SetF22(int v) { f4_22 = v; }
};

extern sGs_DAB8 sDrawState_s_defaultDrawState_DAB8 __asm__("sDrawState_s_defaultDrawState");

struct sWakeVtx_DAB8 {
    char pad0[0x64];
    float age;               // 0x64
    char pad68[0x8];
};


class cGfx_DAB8 {
public:
    char pad[0xE84];
    sGs_DAB8* stateTop;
    char padE88[0x1030 - 0xE88];
    int f1030;
    int Get1030() { return f1030; }
    void PushState() { stateTop[1] = stateTop[0]; stateTop++; }
    char pad1034[0x10D8 - 0x1034];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void v84();
    virtual void v85();
    virtual void v86();
    virtual void v87();
    virtual void v88();
    virtual void v89();
    virtual void DrawStrip(void* verts, int tex, int first, int max, int n, float a, float da);
};

/**
 * Draws a board wake as a strip, fading its alpha from the newest point's age along the strip.
 *
 * Skipped when cRenderToggles_m_DisableBoardWake is set or there are no segments; at most max - 2 segments are drawn.
 */
extern "C" void cBoardWakeFX_render(cBoardWakeFX* self)
{
    if (cRenderToggles_m_DisableBoardWake != 0 || self->mNumWakeLines == 0)
        return;
    ((cGfx_DAB8*)GrMan_boardwakefx)->PushState();
    *((cGfx_DAB8*)GrMan_boardwakefx)->stateTop = sDrawState_s_defaultDrawState_DAB8;
    *(short*)(*(char**)(GrMan_boardwakefx + 0xE84) + 0x10) = ((cGfx_DAB8*)GrMan_boardwakefx)->Get1030();
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f0_2 = 0;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->SetF22(1);
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f4_23 = 0;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f4_20 = 0;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f4_12 = 20;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f4_0 = 2;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f8_10 = 0;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f8_5 = 7;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f4_2 = 5;
    float k = 1.25f;
    int draw = self->mNumWakeLines;
    int n = draw;
    int over = draw + 2; over -= self->mWakeMaxNumLines;
    if (over >= 1)
        draw -= over;
    int last = (self->mCurrentWakeLine + n - 1) % self->mWakeMaxNumLines;
    float ageRatio = (self->mWakeLife - ((sWakeVtx_DAB8*)self->mWakeLines)[last].age) / self->mWakeLife;
    float alphaStart = 1.0f;
    if (ageRatio <= 0.8f)
        alphaStart = ageRatio * k;
    float da = (alphaStart - k) / (float)n;
    if (draw >= 2)
        ((cGfx_DAB8*)GrMan_boardwakefx)->DrawStrip(((char*)&self->mWakeVerts[0]), self->mWakeLineSegs, self->mCurrentWakeLine, self->mWakeMaxNumLines, draw, k, da);
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop--;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_fillCurrentSeg);
#ifdef SKIP_ASM
struct sVecDD30 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sElemDD30 {
    sVecDD30 pts[5];            // 0x00
    sVecDD30 dir;               // 0x50
    float width;                // 0x60
    int i64;                    // 0x64
};

struct sSegDD30 {
    char pad0[0x1C];            // 0x00
    int f1C;                    // 0x1C
    sVecDD30 pos;               // 0x20
};

struct sWakeDD30 {
    char* obj;                  // 0x00
    int i4;                     // 0x04
    int head;                   // 0x08
    sElemDD30* elems;           // 0x0C
    sSegDD30* rings[5];         // 0x10
    char pad24[0x14];           // 0x24
    float phase;                // 0x38
    char pad3C[0x24];           // 0x3C
    sVecDD30 v60;               // 0x60
    sVecDD30 v70;               // 0x70
    sVecDD30 v80;               // 0x80
    float f90;                  // 0x90
    float f94;                  // 0x94
    char pad98[0x1C];           // 0x98
    int n;                      // 0xB4
};

extern sVecDD30 t4vect_Zero_DD30 __asm__("t4vect_Zero");
extern "C" float perlinNoise(int n, float x);

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sVecDD30 wakeScaleDD30(const sVecDD30& v, float s)
{
    sVecDD30 r;
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
static inline sVecDD30 wakeAddDD30(const sVecDD30& a, const sVecDD30& b)
{
    sVecDD30 r;
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
static inline sVecDD30 wakeSubDD30(const sVecDD30& a, const sVecDD30& b)
{
    sVecDD30 r;
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
static inline float wakeLenDD30(const sVecDD30& v)
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

/**
 * Fills the current board wake line from the wake parameters: its spray direction, its row points and the rows' head
 * positions.
 *
 * The line is +0xC[head] (head +0x8). Its age (+0x64) and first point are zeroed and its direction (+0x50) becomes 0.85
 * * (mVelocity - forward * |mVelocity|), with forward the wake parameters' third vector (+0x80). Point i of the +0xB4
 * rows is (side (+0x70) * t * t * 0.75 + up (+0x60) * t) * w, where t = i / (rows - 1) (times 0.9 for the last row) and
 * w = the parameters' speed (+0x90) * (1 + 0.25 * perlinNoise(3, phase +0x38)). Row 0's entry at the head gets @p pos
 * minus 5 * mGroundNormal and the alpha (+0x94) * 128 (+0x1C), copied to the other rows. @p width goes to the line's
 * +0x60.
 *
 * cBoardWakeFX_update calls it with an extra forward-vector argument between @p pos and @p width, which this definition
 * does not declare or use.
 *
 * @param pos  the wake position
 * @param width  the value stored at the line's +0x60
 */
extern "C" void cBoardWakeFX_fillCurrentSeg(sWakeDD30* self, sVecDD30* pos, float width)
{
    float w = self->f90 * (perlinNoise(3, self->phase) * 0.25f + 1.0f);
    sElemDD30* wakeLine = &self->elems[self->head];
    wakeLine->i64 = 0;
    wakeLine->pts[0] = t4vect_Zero_DD30;
    sVecDD30 dir = wakeScaleDD30(wakeScaleDD30(self->v80, -1.0f), wakeLenDD30(*(sVecDD30*)(self->obj + 0x1E0)));
    sVecDD30 neg = wakeScaleDD30(*(sVecDD30*)(self->obj + 0x1E0), -1.0f);
    {
        sVecDD30 x = wakeScaleDD30(wakeSubDD30(dir, neg), 0.85f);
        wakeLine->dir = x;
    }
    for (int i = 0; i < self->n; i++) {
        float rowFrac = (float)i / (float)(self->n - 1);
        float rowFracSq = rowFrac * rowFrac;
        if (i == self->n - 1) {
            rowFrac *= 0.9f;
        }
        sVecDD30 x = wakeScaleDD30(wakeAddDD30(wakeScaleDD30(wakeScaleDD30(self->v70, rowFracSq), 0.75f), wakeScaleDD30(self->v60, rowFrac)), w);
        wakeLine->pts[i] = x;
    }
    {
        sSegDD30* firstRow = self->rings[0];
        sSegDD30* headEntry = (sSegDD30*)(self->head * (int)sizeof(sSegDD30) + (int)firstRow); // PORT: pointer in int
        sVecDD30 x = wakeSubDD30(*pos, wakeScaleDD30(*(sVecDD30*)(self->obj + 0x370), 5.0f));
        headEntry->pos = x;
    }
    self->rings[0][self->head].f1C = (int)(self->f94 * 128.0f);
    for (int j = 1; j < self->n; j++) {
        self->rings[j][self->head].pos = self->rings[0][self->head].pos;
        self->rings[j][self->head].f1C = self->rings[0][self->head].f1C;
    }
    wakeLine->width = width;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_createSeg);
#ifdef SKIP_ASM

struct sEnt_E058 {
    tARGB col;              // 0x00
    char pad10[0xE0];
};

struct sSeg_E058 {
    float f0;                   // 0x00
    char pad4[0xC];             // 0x04
    int r;                      // 0x10
    int g;                      // 0x14
    int b;                      // 0x18
    char pad1C[0x14];           // 0x1C
};


struct sWake_E058 {
    char* obj;                  // 0x00
    int count;                  // 0x04
    int head;                   // 0x08
    char padC[0x4];             // 0x0C
    sSeg_E058* ring;            // 0x10
    sSeg_E058* trails[4];       // 0x14
    char pad24[0x14];           // 0x24
    float phase;                // 0x38
    char pad3C[0x4];            // 0x3C
    int frame;                  // 0x40
    char pad44[0x5C];           // 0x44
    tARGB col;              // 0xA0
    char padB0[0x8];            // 0xB0
    int cap;                    // 0xB8
};

// cRenderStateMan_m_state: the lens/surface table, 8 rows of 0xF0 bytes (config size 0x780); sEnt_E058 views a row from
// its +0x28 field
extern char cRenderStateMan_m_state[];

// PORT: g++ minimum operator (<?).
static inline float wakeClampE058(float v, float lo, float hi)
{
    float r;
    if (v >= lo) {
        r = v <? hi;
    } else {
        r = lo;
    }
    return r;
}

/**
 * Starts a new board wake segment: advances the ring head and sets the new entries' texture offset and ground colour.
 *
 * Advances the phase (+0x38) by 1/30, wrapping at 4, grows the count (+0x4) up to the maximum (+0xB8) and moves the
 * head (+0x8) back one place in the ring. The ground colour (+0xA0) is the rider's row (kRiderVt_getAIIndex) of the
 * 0xF0-byte table at cRenderStateMan_m_state + 0x28, doubled and clamped to 0..1. Row 0's entry at the head gets the
 * frame counter (+0x40) * 0.5 wrapped into -32..32 and the colour * 128 as integers; rows 1..4 copy it. Then the frame
 * counter increments.
 *
 * cBoardWakeFX_update calls it with a surface record and a position, which this definition does not declare or use.
 */
extern "C" void cBoardWakeFX_createSeg(sWake_E058* self)
{
    self->phase += 0.03333333507180214f;
    if (self->phase >= 4.0f) {
        self->phase -= 4.0f;
    }
    if (self->count < self->cap) {
        self->count++;
    }
    self->head = (self->head + self->cap - 1) % self->cap;
    char* sub = self->obj + 0x6C0;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)sub;
    tARGB col = ((sEnt_E058*)(cRenderStateMan_m_state + 0x28))[vt[7].fn(sub + vt[7].delta)].col;
    col.A *= 2.0f;
    col.R *= 2.0f;
    col.G *= 2.0f;
    col.B *= 2.0f;
    col.R = wakeClampE058(col.R, 0.0f, 1.0f);
    col.G = wakeClampE058(col.G, 0.0f, 1.0f);
    col.B = wakeClampE058(col.B, 0.0f, 1.0f);
    col.A = wakeClampE058(col.A, 0.0f, 1.0f);
    self->col = col;
    self->ring[self->head].f0 = (float)self->frame * 0.5f;
    self->ring[self->head].f0 -= (float)(int)(self->ring[self->head].f0 * 0.015625f) * 64.0f + 32.0f;
    self->ring[self->head].r = (int)(self->col.R * 128.0f);
    self->ring[self->head].g = (int)(self->col.G * 128.0f);
    self->ring[self->head].b = (int)(self->col.B * 128.0f);
    int i;
    for (i = 0; i < 4; i++) {
        self->trails[i][self->head].f0 = self->ring[self->head].f0;
        self->trails[i][self->head].r = self->ring[self->head].r;
        self->trails[i][self->head].g = self->ring[self->head].g;
        self->trails[i][self->head].b = self->ring[self->head].b;
    }
    self->frame++;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cWakeParams_Clear);
#ifdef SKIP_ASM
struct sVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sVec4 t4vect_Zero;

/**
 * Clears a board wake's parameter block: zeroes its three vectors (+0x0, +0x10, +0x20) and the words at +0x30 and
 * +0x34.
 *
 * cBoardWakeFX_reset (the wake reset) calls it on cBoardWakeFX + 0x60.
 */
extern "C" void cWakeParams_Clear(cWakeParams* self)
{
    *(int*)&self->mAlpha = 0;
    *(int*)&self->mSpeed = 0;
    *(sVec4*)&self->mUp = t4vect_Zero;
    *(sVec4*)&self->mForward = t4vect_Zero;
    *(sVec4*)&self->mSide = t4vect_Zero;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002DE398);
#ifdef SKIP_ASM
extern int D_004A3AFC;

struct sWakeVecE398 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only inline asm (float absolute value).
static inline float wakeAbsE398(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: the seed is reinterpreted as a float in [1,2) through memory (type pun).
static inline float wakeRandE398()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline void wakeScaleE398(sWakeVecE398& r, const sWakeVecE398& v, float s)
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void wakeAddE398(sWakeVecE398& a, const sWakeVecE398& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

/**
 * Scatters a spray emission position randomly around the board.
 *
 * With bias = the lean side (+0x12C, -1..1) it adds the vector at +0x50 times (bias * 55 + a random value within +-(1 -
 * |bias|) * 55), then the vector at +0x40 times a random value in -10..10. func_002DF920 fills both vectors from the
 * board bone's matrix rows 0 and 2. The random numbers come from the generator in D_004A3AFC. The spray updates
 * func_002DFE88, func_002E02B8, func_002E0EE8 and func_002E1F70 call it.
 *
 * @param out  the position, changed in place
 */
extern "C" void func_002DE398(cDynamicSprayFX* self, sWakeVecE398* out)
{
    float bias = *(float*)(((char*)self) + 0x12C);
    float spread = (1.0f - wakeAbsE398(bias)) * 55.0f;
    float lo = -spread;
    sWakeVecE398 offset;
    sWakeVecE398 scaled;
    wakeScaleE398(scaled, *(sWakeVecE398*)(((char*)self) + 0x50), bias * 55.0f + (lo + (spread - lo) * wakeRandE398()));
    offset = scaled;
    wakeAddE398(*out, offset);
    wakeScaleE398(scaled, *(sWakeVecE398*)(((char*)self) + 0x40), wakeRandE398() * 20.0f + -10.0f);
    offset = scaled;
    wakeAddE398(*out, offset);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_cDynamicSprayFX);
#ifdef SKIP_ASM
// FAKEMATCH: statements 1-32 store through self->p2C[0] and the other 548 through the inline getter, which moves cse's
// 1000-insn flush points; one spelling throughout scores 94.71%. Byte-identical and behaves like the plain form; landed
// marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sEmitterE4A8 {
    char data[0x210];
    sEmitterE4A8() __asm__("cDynamicColourEmitter_cDynamicColourEmitter");
    ~sEmitterE4A8();
};

struct sSprayParams {
    int f00;
    int f04;
    float f08;
    float f0C;
    float f10;
    float f14;
    float f18;
    int f1C;
    int f20;
    int f24;
    int f28;
    int f2C;
    float f30;
    float f34;
    float f38;
    int f3C;
    int f40;
    int f44;
    int f48;
    int f4C;
    float f50;
    float f54;
    int f58;
    int f5C;
    float f60;
    float f64;
    int f68;
    int f6C;
    int f70;
    float f74;
    int f78;
    int f7C;
    float f80;
    float f84;
    float f88;
    float f8C;
    float f90;
    float f94;
    float f98;
    float f9C;
    float fA0;
    int fA4;
    int fA8;
    int fAC;
    int fB0;
    int fB4;
    int fB8;
    int fBC;
    int fC0;
    int fC4;
    int fC8;
    float fCC;
    int fD0;
    float fD4;
    float fD8;
    float fDC;
    float fE0;
    float fE4;
};

struct sSprayFxE4A8 {
    char* vbase;                // 0x00 cRider virtual base
    char pad4[0x4];             // 0x04
    float f8;                   // 0x08
    int iC;                     // 0x0C
    int i10;                    // 0x10
    int i14;                    // 0x14
    int i18;                    // 0x18
    char pad1C[0x4];            // 0x1C
    int i20;                    // 0x20
    int i24;                    // 0x24
    sEmitterE4A8* em;           // 0x28
    sSprayParams* p2C;          // 0x2C
    char pad30[0x30];           // 0x30
    int i60;                    // 0x60
    char pad64[0x18];           // 0x64
    void* mBoneIndicies;                  // 0x7C
    char pad80[0x60];           // 0x80
    int iE0;                    // 0xE0
    char padE4[0x2C];           // 0xE4
    int i110;                   // 0x110
    sSprayParams* SprayData() { return p2C; }
};

extern const sVt9_BW D_00488360;
extern const sVt22_BW D_004883A8;
extern char str_DynamicSprayEmitter[];
extern char str_DynamicSprayData[];
extern char str_BoneIndicies[];
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
extern "C" void func_002E26B8(cDynamicSprayFX* self);
extern "C" void func_002DF448_E4A8(void* self) __asm__("func_002DF448");
extern "C" void func_002DF3B0(cDynamicSprayFX* self);

/**
 * Constructs a rider's snow spray effect: ten particle emitters and their ten spray settings.
 *
 * When @p inChrg is set it also constructs the cRider virtual base at +0x130. It patches the rider's vtables
 * (the g++ 2.95 this-adjust fix-ups when not in charge).
 *
 * Allocates the emitters ("DynamicSprayEmitter") and the spray data ("DynamicSprayData", each set filled in
 * here with its particle count, life, size, gravity, colour and speeds) and a 0x78-byte "BoneIndicies" block.
 * It then calls func_002E26B8, func_002DF448 and func_002DF3B0 (unknown) to finish the setup.
 *
 * @param inChrg  nonzero when this object is complete and must construct its virtual base
 * @return self
 */
// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0x130): vtable copies with delta fixups when not in charge.
extern "C" void* cDynamicSprayFX_cDynamicSprayFX(sSprayFxE4A8* self, int inChrg)
{
    sVt9_BW vtbl1;
    sVt22_BW vtbl2;
    if (inChrg) {
        self->vbase = (char*)self + 0x130;
        cRider_cRider((char*)self + 0x130);
    }
    *(void**)(self->vbase + 0x6E8) = (void*)&D_00488360;
    *(void**)(self->vbase + 0x6D0) = D_00459B90;
    *(void**)(self->vbase + 0x6C0) = (void*)&D_004883A8;
    sEmitterE4A8*& em = self->em;
    if (inChrg == 0) {
        int vc;
        vtbl1 = D_00488360;
        *(void**)(self->vbase + 0x6E8) = &vtbl1;
        {
            char* vbo = self->vbase - 0x130;
            vc = (char*)self - vbo;
        }
        vtbl1.e[1].delta = D_00488360.e[1].delta + vc;
        vtbl2 = D_004883A8;
        *(void**)(self->vbase + 0x6C0) = &vtbl2;
        vtbl2.e[1].delta = D_004883A8.e[1].delta + vc;
    }
    self->i10 = 0;
    self->i14 = 0;
    self->i18 = 0;
    self->i20 = 0;
    self->i24 = 0;
    self->iE0 = 0;
    self->i110 = 0;
    self->f8 = 50.0f;
    self->iC = 1;
    self->i60 = 1;
    em = new (str_DynamicSprayEmitter, 0, 0) sEmitterE4A8[10];
    self->p2C = new (str_DynamicSprayData, 0, 0) sSprayParams[10];
    self->p2C[0].f00 = 2;
    self->p2C[0].f04 = 0;
    self->p2C[0].f08 = -1.0f;
    self->p2C[0].f0C = 2.5f;
    self->p2C[0].f10 = 10.0f;
    self->p2C[0].f14 = 0.6000000238418579f;
    self->p2C[0].f18 = 10.0f;
    self->p2C[0].f1C = 0;
    self->p2C[0].f20 = 0;
    self->p2C[0].f24 = 0;
    self->p2C[0].f28 = 0;
    self->p2C[0].f2C = 0;
    self->p2C[0].f30 = 5.0f;
    self->p2C[0].f34 = 5.0f;
    self->p2C[0].f38 = 5.0f;
    self->p2C[0].f3C = 0;
    self->p2C[0].f40 = 0;
    self->p2C[0].f44 = 0;
    self->p2C[0].f48 = 0;
    self->p2C[0].f4C = 0;
    self->p2C[0].f50 = 0.0f;
    self->p2C[0].f54 = 50.0f;
    self->p2C[0].f58 = 0;
    self->p2C[0].f5C = 0;
    self->p2C[0].f60 = 0.0f;
    self->p2C[0].f64 = 50.0f;
    self->p2C[0].f68 = 0;
    self->p2C[0].f6C = 0;
    self->p2C[0].f70 = 0;
    self->p2C[0].f74 = 50.0f;
    self->p2C[0].f78 = 0;
    self->p2C[0].f7C = 0;
    self->SprayData()[0].f80 = -150.0f;
    self->SprayData()[0].f84 = 0.4000000059604645f;
    self->SprayData()[0].f88 = 1.0f;
    self->SprayData()[0].f8C = 1.0f;
    self->SprayData()[0].f90 = 1.0f;
    self->SprayData()[0].f94 = 0.0f;
    self->SprayData()[0].f98 = 1.0f;
    self->SprayData()[0].f9C = 1.0f;
    self->SprayData()[0].fA0 = 1.0f;
    self->SprayData()[0].fA4 = 0;
    self->SprayData()[0].fA8 = 0;
    self->SprayData()[0].fAC = 0;
    self->SprayData()[0].fB0 = 0;
    self->SprayData()[0].fB4 = 0;
    self->SprayData()[0].fB8 = 0;
    self->SprayData()[0].fBC = 0;
    self->SprayData()[0].fC0 = 0;
    self->SprayData()[0].fC8 = 1;
    self->SprayData()[0].fCC = 35.0f;
    self->SprayData()[0].fC4 = 5;
    self->SprayData()[0].fD0 = 1;
    self->SprayData()[0].fD4 = 20.0f;
    self->SprayData()[0].fD8 = 0.4000000059604645f;
    self->SprayData()[0].fDC = 0.0f;
    self->SprayData()[0].fE0 = 0.0f;
    self->SprayData()[0].fE4 = 0.0f;
    self->SprayData()[1].f00 = 5;
    self->SprayData()[1].f04 = 0;
    self->SprayData()[1].f08 = -1.0f;
    self->SprayData()[1].f0C = 1.0f;
    self->SprayData()[1].f10 = 8.0f;
    self->SprayData()[1].f14 = 1.0f;
    self->SprayData()[1].f18 = 6.0f;
    self->SprayData()[1].f1C = 0;
    self->SprayData()[1].f20 = 0;
    self->SprayData()[1].f24 = 0;
    self->SprayData()[1].f28 = 0;
    self->SprayData()[1].f2C = 0;
    self->SprayData()[1].f30 = 0.0f;
    self->SprayData()[1].f34 = 0.0f;
    self->SprayData()[1].f38 = 0.0f;
    self->SprayData()[1].f3C = 0;
    self->SprayData()[1].f40 = 0;
    self->SprayData()[1].f44 = 0;
    self->SprayData()[1].f48 = 0;
    self->SprayData()[1].f4C = 0;
    self->SprayData()[1].f50 = 0.0f;
    self->SprayData()[1].f54 = 200.0f;
    self->SprayData()[1].f58 = 0;
    self->SprayData()[1].f5C = 0;
    self->SprayData()[1].f60 = 0.0f;
    self->SprayData()[1].f64 = 200.0f;
    self->SprayData()[1].f68 = 0;
    self->SprayData()[1].f6C = 0;
    self->SprayData()[1].f70 = 0;
    self->SprayData()[1].f74 = 200.0f;
    self->SprayData()[1].f78 = 0;
    self->SprayData()[1].f7C = 0;
    self->SprayData()[1].f80 = -3000.0f;
    self->SprayData()[1].f84 = 1.0f;
    self->SprayData()[1].f88 = 1.0f;
    self->SprayData()[1].f8C = 1.0f;
    self->SprayData()[1].f90 = 1.0f;
    self->SprayData()[1].f94 = 1.0f;
    self->SprayData()[1].f98 = 1.0f;
    self->SprayData()[1].f9C = 1.0f;
    self->SprayData()[1].fA0 = 1.0f;
    self->SprayData()[1].fA4 = 0;
    self->SprayData()[1].fA8 = 0;
    self->SprayData()[1].fAC = 0;
    self->SprayData()[1].fB0 = 0;
    self->SprayData()[1].fB4 = 0;
    self->SprayData()[1].fB8 = 0;
    self->SprayData()[1].fBC = 0;
    self->SprayData()[1].fC0 = 0;
    self->SprayData()[1].fC8 = 1;
    self->SprayData()[1].fCC = 0.0f;
    self->SprayData()[1].fC4 = 14;
    self->SprayData()[1].fD0 = 8;
    self->SprayData()[1].fD4 = 30.0f;
    self->SprayData()[1].fD8 = 0.4000000059604645f;
    self->SprayData()[1].fDC = 1000.0f;
    self->SprayData()[1].fE0 = 0.019999999552965164f;
    self->SprayData()[1].fE4 = 0.15000000596046448f;
    self->SprayData()[2].f00 = 6;
    self->SprayData()[2].f04 = 0;
    self->SprayData()[2].f08 = -1.0f;
    self->SprayData()[2].f0C = 1.0f;
    self->SprayData()[2].f10 = 2.5f;
    self->SprayData()[2].f14 = 1.0f;
    self->SprayData()[2].f18 = 1.2000000476837158f;
    self->SprayData()[2].f1C = 0;
    self->SprayData()[2].f20 = 0;
    self->SprayData()[2].f24 = 0;
    self->SprayData()[2].f28 = 0;
    self->SprayData()[2].f2C = 0;
    self->SprayData()[2].f30 = 0.0f;
    self->SprayData()[2].f34 = 0.0f;
    self->SprayData()[2].f38 = 0.0f;
    self->SprayData()[2].f3C = 0;
    self->SprayData()[2].f40 = 0;
    self->SprayData()[2].f44 = 0;
    self->SprayData()[2].f48 = 0;
    self->SprayData()[2].f4C = 0;
    self->SprayData()[2].f50 = 0.0f;
    self->SprayData()[2].f54 = 200.0f;
    self->SprayData()[2].f58 = 0;
    self->SprayData()[2].f5C = 0;
    self->SprayData()[2].f60 = 0.0f;
    self->SprayData()[2].f64 = 200.0f;
    self->SprayData()[2].f68 = 0;
    self->SprayData()[2].f6C = 0;
    self->SprayData()[2].f70 = 0;
    self->SprayData()[2].f74 = 200.0f;
    self->SprayData()[2].f78 = 0;
    self->SprayData()[2].f7C = 0;
    self->SprayData()[2].f80 = -3000.0f;
    self->SprayData()[2].f84 = 1.0f;
    self->SprayData()[2].f88 = 1.0f;
    self->SprayData()[2].f8C = 1.0f;
    self->SprayData()[2].f90 = 1.0f;
    self->SprayData()[2].f94 = 1.0f;
    self->SprayData()[2].f98 = 1.0f;
    self->SprayData()[2].f9C = 1.0f;
    self->SprayData()[2].fA0 = 1.0f;
    self->SprayData()[2].fA4 = 0;
    self->SprayData()[2].fA8 = 0;
    self->SprayData()[2].fAC = 0;
    self->SprayData()[2].fB0 = 0;
    self->SprayData()[2].fB4 = 0;
    self->SprayData()[2].fB8 = 0;
    self->SprayData()[2].fBC = 0;
    self->SprayData()[2].fC0 = 0;
    self->SprayData()[2].fC8 = 1;
    self->SprayData()[2].fCC = 2.5f;
    self->SprayData()[2].fC4 = 14;
    self->SprayData()[2].fD0 = 8;
    self->SprayData()[2].fD4 = 45.0f;
    self->SprayData()[2].fD8 = 0.4000000059604645f;
    self->SprayData()[2].fDC = 1000.0f;
    self->SprayData()[2].fE0 = 0.019999999552965164f;
    self->SprayData()[2].fE4 = 0.30000001192092896f;
    self->SprayData()[3].f00 = 3;
    self->SprayData()[3].f04 = 0;
    self->SprayData()[3].f08 = -1.0f;
    self->SprayData()[3].f0C = 1.0f;
    self->SprayData()[3].f10 = 3.5f;
    self->SprayData()[3].f14 = 1.0f;
    self->SprayData()[3].f18 = 1.2000000476837158f;
    self->SprayData()[3].f1C = 0;
    self->SprayData()[3].f20 = 0;
    self->SprayData()[3].f24 = 0;
    self->SprayData()[3].f28 = 0;
    self->SprayData()[3].f2C = 0;
    self->SprayData()[3].f30 = 0.0f;
    self->SprayData()[3].f34 = 0.0f;
    self->SprayData()[3].f38 = 0.0f;
    self->SprayData()[3].f3C = 0;
    self->SprayData()[3].f40 = 0;
    self->SprayData()[3].f44 = 0;
    self->SprayData()[3].f48 = 0;
    self->SprayData()[3].f4C = 0;
    self->SprayData()[3].f50 = 0.0f;
    self->SprayData()[3].f54 = 100.0f;
    self->SprayData()[3].f58 = 0;
    self->SprayData()[3].f5C = 0;
    self->SprayData()[3].f60 = 0.0f;
    self->SprayData()[3].f64 = 100.0f;
    self->SprayData()[3].f68 = 0;
    self->SprayData()[3].f6C = 0;
    self->SprayData()[3].f70 = 0;
    self->SprayData()[3].f74 = 100.0f;
    self->SprayData()[3].f78 = 0;
    self->SprayData()[3].f7C = 0;
    self->SprayData()[3].f80 = -2500.0f;
    self->SprayData()[3].f84 = 1.0f;
    self->SprayData()[3].f88 = 0.20000000298023224f;
    self->SprayData()[3].f8C = 0.20000000298023224f;
    self->SprayData()[3].f90 = 0.20000000298023224f;
    self->SprayData()[3].f94 = 1.0f;
    self->SprayData()[3].f98 = 0.20000000298023224f;
    self->SprayData()[3].f9C = 0.20000000298023224f;
    self->SprayData()[3].fA0 = 0.20000000298023224f;
    self->SprayData()[3].fA4 = 0;
    self->SprayData()[3].fA8 = 0;
    self->SprayData()[3].fAC = 0;
    self->SprayData()[3].fB0 = 0;
    self->SprayData()[3].fB4 = 0;
    self->SprayData()[3].fB8 = 0;
    self->SprayData()[3].fBC = 0;
    self->SprayData()[3].fC0 = 0;
    self->SprayData()[3].fC8 = 1;
    self->SprayData()[3].fCC = 0.0f;
    self->SprayData()[3].fC4 = 14;
    self->SprayData()[3].fD0 = 8;
    self->SprayData()[3].fD4 = 45.0f;
    self->SprayData()[3].fD8 = 1.0f;
    self->SprayData()[3].fDC = 900.0f;
    self->SprayData()[3].fE0 = 0.11999999731779099f;
    self->SprayData()[3].fE4 = 0.25f;
    self->SprayData()[4].f00 = 1;
    self->SprayData()[4].f04 = 0;
    self->SprayData()[4].f08 = -1.0f;
    self->SprayData()[4].f0C = 1.100000023841858f;
    self->SprayData()[4].f10 = 5.0f;
    self->SprayData()[4].f14 = 0.44999998807907104f;
    self->SprayData()[4].f18 = 0.0f;
    self->SprayData()[4].f1C = 0;
    self->SprayData()[4].f20 = 0;
    self->SprayData()[4].f24 = 0;
    self->SprayData()[4].f28 = 0;
    self->SprayData()[4].f2C = 0;
    self->SprayData()[4].f30 = 0.0f;
    self->SprayData()[4].f34 = 0.0f;
    self->SprayData()[4].f38 = 0.0f;
    self->SprayData()[4].f3C = 0;
    self->SprayData()[4].f40 = 0;
    self->SprayData()[4].f44 = 0;
    self->SprayData()[4].f48 = 0;
    self->SprayData()[4].f4C = 0;
    self->SprayData()[4].f50 = 0.0f;
    self->SprayData()[4].f54 = 40.0f;
    self->SprayData()[4].f58 = 0;
    self->SprayData()[4].f5C = 0;
    self->SprayData()[4].f60 = 40.0f;
    self->SprayData()[4].f64 = 0.0f;
    self->SprayData()[4].f68 = 0;
    self->SprayData()[4].f6C = 0;
    self->SprayData()[4].f70 = 0;
    self->SprayData()[4].f74 = 40.0f;
    self->SprayData()[4].f78 = 0;
    self->SprayData()[4].f7C = 0;
    self->SprayData()[4].f80 = 300.0f;
    self->SprayData()[4].f84 = 0.07999999821186066f;
    self->SprayData()[4].f88 = 1.0f;
    self->SprayData()[4].f8C = 1.0f;
    self->SprayData()[4].f90 = 1.0f;
    self->SprayData()[4].f94 = 0.0f;
    self->SprayData()[4].f98 = 1.0f;
    self->SprayData()[4].f9C = 1.0f;
    self->SprayData()[4].fA0 = 1.0f;
    self->SprayData()[4].fA4 = 0;
    self->SprayData()[4].fA8 = 0;
    self->SprayData()[4].fAC = 0;
    self->SprayData()[4].fB0 = 0;
    self->SprayData()[4].fB4 = 0;
    self->SprayData()[4].fB8 = 0;
    self->SprayData()[4].fBC = 0;
    self->SprayData()[4].fC0 = 0;
    self->SprayData()[4].fC8 = 1;
    self->SprayData()[4].fCC = 32.0f;
    self->SprayData()[4].fC4 = 25;
    self->SprayData()[4].fD0 = 1;
    self->SprayData()[4].fD4 = 20.0f;
    self->SprayData()[4].fD8 = 1.0f;
    self->SprayData()[4].fDC = 0.0f;
    self->SprayData()[4].fE0 = 0.0f;
    self->SprayData()[4].fE4 = 0.0f;
    self->SprayData()[5].f00 = 2;
    self->SprayData()[5].f04 = 0;
    self->SprayData()[5].f08 = -1.0f;
    self->SprayData()[5].f0C = 4.0f;
    self->SprayData()[5].f10 = 55.0f;
    self->SprayData()[5].f14 = 1.0f;
    self->SprayData()[5].f18 = 25.0f;
    self->SprayData()[5].f1C = 0;
    self->SprayData()[5].f20 = 0;
    self->SprayData()[5].f24 = 0;
    self->SprayData()[5].f28 = 0;
    self->SprayData()[5].f2C = 0;
    self->SprayData()[5].f30 = 60.0f;
    self->SprayData()[5].f34 = 60.0f;
    self->SprayData()[5].f38 = 10.0f;
    self->SprayData()[5].f3C = 0;
    self->SprayData()[5].f40 = 0;
    self->SprayData()[5].f44 = 0;
    self->SprayData()[5].f48 = 0;
    self->SprayData()[5].f4C = 0;
    self->SprayData()[5].f50 = 400.0f;
    self->SprayData()[5].f54 = 500.0f;
    self->SprayData()[5].f58 = 0;
    self->SprayData()[5].f5C = 0;
    self->SprayData()[5].f60 = 0.0f;
    self->SprayData()[5].f64 = 500.0f;
    self->SprayData()[5].f68 = 0;
    self->SprayData()[5].f6C = 0;
    self->SprayData()[5].f70 = 0;
    self->SprayData()[5].f74 = 200.0f;
    self->SprayData()[5].f78 = 0;
    self->SprayData()[5].f7C = 0;
    self->SprayData()[5].f80 = -1800.0f;
    self->SprayData()[5].f84 = 0.6000000238418579f;
    self->SprayData()[5].f88 = 1.0f;
    self->SprayData()[5].f8C = 1.0f;
    self->SprayData()[5].f90 = 1.0f;
    self->SprayData()[5].f94 = 0.0f;
    self->SprayData()[5].f98 = 1.0f;
    self->SprayData()[5].f9C = 1.0f;
    self->SprayData()[5].fA0 = 1.0f;
    self->SprayData()[5].fA4 = 0;
    self->SprayData()[5].fA8 = 0;
    self->SprayData()[5].fAC = 0;
    self->SprayData()[5].fB0 = 0;
    self->SprayData()[5].fB4 = 0;
    self->SprayData()[5].fB8 = 0;
    self->SprayData()[5].fBC = 0;
    self->SprayData()[5].fC0 = 0;
    self->SprayData()[5].fC8 = 1;
    self->SprayData()[5].fCC = 170.0f;
    self->SprayData()[5].fC4 = 6;
    self->SprayData()[5].fD0 = 1;
    self->SprayData()[5].fD4 = 20.0f;
    self->SprayData()[5].fD8 = 0.0f;
    self->SprayData()[5].fDC = 0.0f;
    self->SprayData()[5].fE0 = 0.0f;
    self->SprayData()[5].fE4 = 0.0f;
    self->SprayData()[6].f00 = 1;
    self->SprayData()[6].f04 = 0;
    self->SprayData()[6].f08 = -1.0f;
    self->SprayData()[6].f0C = 4.0f;
    self->SprayData()[6].f10 = 30.0f;
    self->SprayData()[6].f14 = 1.0f;
    self->SprayData()[6].f18 = 20.0f;
    self->SprayData()[6].f1C = 0;
    self->SprayData()[6].f20 = 0;
    self->SprayData()[6].f24 = 0;
    self->SprayData()[6].f28 = 0;
    self->SprayData()[6].f2C = 0;
    self->SprayData()[6].f30 = 10.0f;
    self->SprayData()[6].f34 = 10.0f;
    self->SprayData()[6].f38 = 0.0f;
    self->SprayData()[6].f3C = 0;
    self->SprayData()[6].f40 = 0;
    self->SprayData()[6].f44 = 0;
    self->SprayData()[6].f48 = 0;
    self->SprayData()[6].f4C = 0;
    self->SprayData()[6].f50 = 200.0f;
    self->SprayData()[6].f54 = 60.0f;
    self->SprayData()[6].f58 = 0;
    self->SprayData()[6].f5C = 0;
    self->SprayData()[6].f60 = 0.0f;
    self->SprayData()[6].f64 = 60.0f;
    self->SprayData()[6].f68 = 0;
    self->SprayData()[6].f6C = 0;
    self->SprayData()[6].f70 = 0;
    self->SprayData()[6].f74 = 0.0f;
    self->SprayData()[6].f78 = 0;
    self->SprayData()[6].f7C = 0;
    self->SprayData()[6].f80 = -1250.0f;
    self->SprayData()[6].f84 = 0.5f;
    self->SprayData()[6].f88 = 1.0f;
    self->SprayData()[6].f8C = 1.0f;
    self->SprayData()[6].f90 = 1.0f;
    self->SprayData()[6].f94 = 0.0f;
    self->SprayData()[6].f98 = 1.0f;
    self->SprayData()[6].f9C = 1.0f;
    self->SprayData()[6].fA0 = 1.0f;
    self->SprayData()[6].fA4 = 0;
    self->SprayData()[6].fA8 = 0;
    self->SprayData()[6].fAC = 0;
    self->SprayData()[6].fB0 = 0;
    self->SprayData()[6].fB4 = 0;
    self->SprayData()[6].fB8 = 0;
    self->SprayData()[6].fBC = 0;
    self->SprayData()[6].fC0 = 0;
    self->SprayData()[6].fC8 = 1;
    self->SprayData()[6].fCC = 60.0f;
    self->SprayData()[6].fC4 = 6;
    self->SprayData()[6].fD0 = 1;
    self->SprayData()[6].fD4 = 20.0f;
    self->SprayData()[6].fD8 = 0.0f;
    self->SprayData()[6].fDC = 0.0f;
    self->SprayData()[6].fE0 = 0.0f;
    self->SprayData()[6].fE4 = 0.0f;
    self->SprayData()[7].f00 = 2;
    self->SprayData()[7].f04 = 0;
    self->SprayData()[7].f08 = -1.0f;
    self->SprayData()[7].f0C = 1.5f;
    self->SprayData()[7].f10 = 30.0f;
    self->SprayData()[7].f14 = 0.6000000238418579f;
    self->SprayData()[7].f18 = 15.0f;
    self->SprayData()[7].f1C = 0;
    self->SprayData()[7].f20 = 0;
    self->SprayData()[7].f24 = 0;
    self->SprayData()[7].f28 = 0;
    self->SprayData()[7].f2C = 0;
    self->SprayData()[7].f30 = 0.0f;
    self->SprayData()[7].f34 = 0.0f;
    self->SprayData()[7].f38 = 0.0f;
    self->SprayData()[7].f3C = 0;
    self->SprayData()[7].f40 = 0;
    self->SprayData()[7].f44 = 0;
    self->SprayData()[7].f48 = 0;
    self->SprayData()[7].f4C = 0;
    self->SprayData()[7].f50 = 0.0f;
    self->SprayData()[7].f54 = 40.0f;
    self->SprayData()[7].f58 = 0;
    self->SprayData()[7].f5C = 0;
    self->SprayData()[7].f60 = 0.0f;
    self->SprayData()[7].f64 = 40.0f;
    self->SprayData()[7].f68 = 0;
    self->SprayData()[7].f6C = 0;
    self->SprayData()[7].f70 = 0;
    self->SprayData()[7].f74 = 40.0f;
    self->SprayData()[7].f78 = 0;
    self->SprayData()[7].f7C = 0;
    self->SprayData()[7].f80 = -20.0f;
    self->SprayData()[7].f84 = 0.30000001192092896f;
    self->SprayData()[7].f88 = 1.0f;
    self->SprayData()[7].f8C = 1.0f;
    self->SprayData()[7].f90 = 1.0f;
    self->SprayData()[7].f94 = 0.0f;
    self->SprayData()[7].f98 = 1.0f;
    self->SprayData()[7].f9C = 1.0f;
    self->SprayData()[7].fA0 = 1.0f;
    self->SprayData()[7].fA4 = 0;
    self->SprayData()[7].fA8 = 0;
    self->SprayData()[7].fAC = 0;
    self->SprayData()[7].fB0 = 0;
    self->SprayData()[7].fB4 = 0;
    self->SprayData()[7].fB8 = 0;
    self->SprayData()[7].fBC = 0;
    self->SprayData()[7].fC0 = 0;
    self->SprayData()[7].fC8 = 1;
    self->SprayData()[7].fCC = 65.0f;
    self->SprayData()[7].fC4 = 5;
    self->SprayData()[7].fD0 = 1;
    self->SprayData()[7].fD4 = 20.0f;
    self->SprayData()[7].fD8 = 0.6499999761581421f;
    self->SprayData()[7].fDC = 20.0f;
    self->SprayData()[7].fE0 = 0.0f;
    self->SprayData()[7].fE4 = 0.0f;
    self->SprayData()[8].f00 = 1;
    self->SprayData()[8].f04 = 0;
    self->SprayData()[8].f08 = -1.0f;
    self->SprayData()[8].f0C = 1.7500100135803223f;
    self->SprayData()[8].f10 = 40.0f;
    self->SprayData()[8].f14 = 0.800000011920929f;
    self->SprayData()[8].f18 = 10.0f;
    self->SprayData()[8].f1C = 0;
    self->SprayData()[8].f20 = 0;
    self->SprayData()[8].f24 = 0;
    self->SprayData()[8].f28 = 0;
    self->SprayData()[8].f2C = 0;
    self->SprayData()[8].f30 = 0.0f;
    self->SprayData()[8].f34 = 0.0f;
    self->SprayData()[8].f38 = 0.0f;
    self->SprayData()[8].f3C = 0;
    self->SprayData()[8].f40 = 0;
    self->SprayData()[8].f44 = 0;
    self->SprayData()[8].f48 = 0;
    self->SprayData()[8].f4C = 0;
    self->SprayData()[8].f50 = 0.0f;
    self->SprayData()[8].f54 = 0.0f;
    self->SprayData()[8].f58 = 0;
    self->SprayData()[8].f5C = 0;
    self->SprayData()[8].f60 = 0.0f;
    self->SprayData()[8].f64 = 0.0f;
    self->SprayData()[8].f68 = 0;
    self->SprayData()[8].f6C = 0;
    self->SprayData()[8].f70 = 0;
    self->SprayData()[8].f74 = 0.0f;
    self->SprayData()[8].f78 = 0;
    self->SprayData()[8].f7C = 0;
    self->SprayData()[8].f80 = 0.0f;
    self->SprayData()[8].f84 = 0.3499999940395355f;
    self->SprayData()[8].f88 = 1.0f;
    self->SprayData()[8].f8C = 1.0f;
    self->SprayData()[8].f90 = 1.0f;
    self->SprayData()[8].f94 = 0.0f;
    self->SprayData()[8].f98 = 1.0f;
    self->SprayData()[8].f9C = 1.0f;
    self->SprayData()[8].fA0 = 1.0f;
    self->SprayData()[8].fA4 = 0;
    self->SprayData()[8].fA8 = 0;
    self->SprayData()[8].fAC = 0;
    self->SprayData()[8].fB0 = 0;
    self->SprayData()[8].fB4 = 0;
    self->SprayData()[8].fB8 = 0;
    self->SprayData()[8].fBC = 0;
    self->SprayData()[8].fC0 = 0;
    self->SprayData()[8].fC8 = 1;
    self->SprayData()[8].fCC = 65.0f;
    self->SprayData()[8].fC4 = 5;
    self->SprayData()[8].fD0 = 1;
    self->SprayData()[8].fD4 = 20.0f;
    self->SprayData()[8].fD8 = 0.30000001192092896f;
    self->SprayData()[8].fDC = 0.0f;
    self->SprayData()[8].fE0 = 0.0f;
    self->SprayData()[8].fE4 = 0.0f;
    self->SprayData()[9].f00 = 2;
    self->SprayData()[9].f04 = 0;
    self->SprayData()[9].f08 = -1.0f;
    self->SprayData()[9].f0C = 1.5f;
    self->SprayData()[9].f10 = 9.999999747378752e-06f;
    self->SprayData()[9].f14 = 0.4000000059604645f;
    self->SprayData()[9].f18 = 0.0f;
    self->SprayData()[9].f1C = 0;
    self->SprayData()[9].f20 = 0;
    self->SprayData()[9].f24 = 0;
    self->SprayData()[9].f28 = 0;
    self->SprayData()[9].f2C = 0;
    self->SprayData()[9].f30 = 0.0f;
    self->SprayData()[9].f34 = 0.0f;
    self->SprayData()[9].f38 = 0.0f;
    self->SprayData()[9].f3C = 0;
    self->SprayData()[9].f40 = 0;
    self->SprayData()[9].f44 = 0;
    self->SprayData()[9].f48 = 0;
    self->SprayData()[9].f4C = 0;
    self->SprayData()[9].f50 = 0.0f;
    self->SprayData()[9].f54 = 200.0f;
    self->SprayData()[9].f58 = 0;
    self->SprayData()[9].f5C = 0;
    self->SprayData()[9].f60 = 0.0f;
    self->SprayData()[9].f64 = 200.0f;
    self->SprayData()[9].f68 = 0;
    self->SprayData()[9].f6C = 0;
    self->SprayData()[9].f70 = 0;
    self->SprayData()[9].f74 = 200.0f;
    self->SprayData()[9].f78 = 0;
    self->SprayData()[9].f7C = 0;
    self->SprayData()[9].f80 = 0.0f;
    self->SprayData()[9].f84 = 0.25f;
    self->SprayData()[9].f88 = 1.0f;
    self->SprayData()[9].f8C = 1.0f;
    self->SprayData()[9].f90 = 1.0f;
    self->SprayData()[9].f94 = 0.0f;
    self->SprayData()[9].f98 = 1.0f;
    self->SprayData()[9].f9C = 1.0f;
    self->SprayData()[9].fA0 = 1.0f;
    self->SprayData()[9].fA4 = 0;
    self->SprayData()[9].fA8 = 0;
    self->SprayData()[9].fAC = 0;
    self->SprayData()[9].fB0 = 0;
    self->SprayData()[9].fB4 = 0;
    self->SprayData()[9].fB8 = 0;
    self->SprayData()[9].fBC = 0;
    self->SprayData()[9].fC0 = 0;
    self->SprayData()[9].fC8 = 1;
    self->SprayData()[9].fCC = 51.220001220703125f;
    self->SprayData()[9].fC4 = 5;
    self->SprayData()[9].fD0 = 1;
    self->SprayData()[9].fD4 = 20.0f;
    self->SprayData()[9].fD8 = 0.800000011920929f;
    self->SprayData()[9].fDC = 0.0f;
    self->SprayData()[9].fE0 = 0.0f;
    self->SprayData()[9].fE4 = 0.0f;
    self->mBoneIndicies = operator new(0x78, str_BoneIndicies, 0, 0);
    func_002E26B8((cDynamicSprayFX*)self);
    func_002DF448_E4A8(self);
    func_002DF3B0((cDynamicSprayFX*)self);
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX__cDynamicSprayFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardwakefx_classes.h"

/**
 * Destroys a snow spray effect's arrays: the emitters (+0x28), the spray data (+0x2C) and the bone indices
 * (+0x7C).
 */
cVbDtor002DF1C8::~cVbDtor002DF1C8()
{
    delete[] m28;
    if (m2C != 0) cMemMan_free(m2C);
    if (m7C != 0) cMemMan_free(m7C);
}

// PORT: the compiler names this dtor _$_15cVbDtor002DF1C8; expose it under the address-derived name.
__asm__(".globl cDynamicSprayFX__cDynamicSprayFX\n.type cDynamicSprayFX__cDynamicSprayFX,@function\ncDynamicSprayFX__cDynamicSprayFX = _$_15cVbDtor002DF1C8\n.size cDynamicSprayFX__cDynamicSprayFX,0x1E8\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002DF1C8$13cRiderVbC_bwf = D_00488360\n"
        "_vt$15cVbDtor002DF1C8$14cRiderVbBC_bwf = D_00459B90\n"
        "_vt$15cVbDtor002DF1C8$12cRiderVb_bwf = D_004883A8\n"
        "_$_12cRiderVb_bwf = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002DF3B0);
#ifdef SKIP_ASM
extern "C" void func_002E26B8(cDynamicSprayFX* self);
extern "C" void func_002E2550(cDynamicSprayFX* self);

/**
 * Resets a rider's snow spray effect.
 *
 * Sets +0x120 to 0.5 and the colour (+0x90) to white, and zeroes +0x4, +0x10, +0x1C, +0x74, +0x78, +0x80 (so the bone
 * table is looked up again), +0xE0 and the lean side +0x12C. When the stored player flag (+0x124) no longer matches
 * whether the rider has a view (mViewIndex >= 0) it re-tunes the emitters with func_002E26B8; then it picks the
 * emitters for the rider's detail level (func_002E2550). The cDynamicSprayFX constructor and cRiderHandler_resetRiderFX
 * (on cRiderHandler + 0xB40) call it.
 */
extern "C" void func_002DF3B0(cDynamicSprayFX* self)
{
    sVec4 white;
    white.x = 1.0f;
    white.y = 1.0f;
    white.z = 1.0f;
    white.w = 1.0f;
    *(float*)((char*)self + 0x120) = 0.5f;
    *(int*)((char*)self + 0x10) = 0;
    *(sVec4*)((char*)self + 0x90) = white;
    *(int*)((char*)self + 0x74) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x80) = 0;
    *(int*)((char*)self + 0x78) = 0;
    *(int*)((char*)self + 0xE0) = 0;
    *(int*)((char*)self + 0x12C) = 0;
    if (*(int*)((char*)self + 0x124) != (*(int*)(*(char**)self + 0x870) >= 0)) {
        func_002E26B8(self);
    }
    func_002E2550(self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002DF448);
#ifdef SKIP_ASM


struct func_002DF448_sFx {
    char pad0[0x28];
    cDynamicColourEmitter* elems;     // 0x28
    char* data;                     // 0x2C
    char pad30[0x60 - 0x30];
    int f60;                        // 0x60
};

/**
 * Re-initialises the ten spray emitters from their spray settings and clears the re-init flag (+0x60).
 *
 * Emitter i (0x210 bytes each, array at +0x28) gets its virtual slot 3 called with settings set i (0xE8 bytes each,
 * array at +0x2C) and the emitter's float at +0x58. The cDynamicSprayFX constructor calls it, and func_002DF920 does
 * when +0x60 is set.
 */
extern "C" void func_002DF448(func_002DF448_sFx* self)
{
    int i;
    for (i = 0; i < 10; i++) {
        // PORT: pointer held in int (index-first address arithmetic)
        cDynamicColourEmitter* emitter = (cDynamicColourEmitter*)(i * 0x210 + (int)self->elems);
        sVEntry<void(void*, void*, float)>* vt = ((sVEntry<void (void*, void*, float)>*)emitter->mVtbl);
        vt[3].fn((char*)emitter + vt[3].delta, self->data + i * 232, emitter->m_emitter.SeedX0);
    }
    self->f60 = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002DF4D0);
#ifdef SKIP_ASM
extern const char str_shinleft_b[];
extern const char str_footleft_b[];
extern const char str_shinright_b[];
extern const char str_footright_b[];
extern const char str_thighleft_b[];
extern const char str_thighright_b[];
extern const char str_hips_b[];
extern const char str_lowerspine[];
extern const char str_middlespine[];
extern const char str_upperspine[];
extern const char str_neck[];
extern const char str_clavicleleft[];
extern const char str_handright_b[];
extern const char str_bicepleft[];
extern const char str_biceptwistleft[];
extern const char str_forearmleft_b[];
extern const char str_handleft_b[];
extern const char str_clavicleright[];
extern const char str_bicepright[];
extern const char str_biceptwistright[];
extern const char str_forearmright_b[];
extern "C" int cAnimModel_getBoneSkelIndex(void* self, int idx, const char* name);

/**
 * Looks up the 30 rider bones the spray effect uses and marks the table as filled (+0x80 = 1).
 *
 * Stores cAnimModel_getBoneSkelIndex results for the rider's mAnimModel into the 0x78-byte table at +0x7C: shins, feet,
 * thighs, hips, spine, neck, clavicles, biceps, forearms and hands by name ("shinleft_b", "footleft_b", ...). Entries
 * 0-14 and 15-29 repeat the leg, hip and spine bones; entry 12 is "handright_b" between left-arm bones, possibly a
 * slip. func_002DF920 calls it while +0x80 is 0.
 */
extern "C" void func_002DF4D0(cDynamicSprayFX* self)
{
    (*(int**)(((char*)self) + 0x7C))[0] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_shinleft_b);
    (*(int**)(((char*)self) + 0x7C))[1] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_footleft_b);
    (*(int**)(((char*)self) + 0x7C))[2] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_shinright_b);
    (*(int**)(((char*)self) + 0x7C))[3] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_footright_b);
    (*(int**)(((char*)self) + 0x7C))[4] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_thighleft_b);
    (*(int**)(((char*)self) + 0x7C))[5] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_thighright_b);
    (*(int**)(((char*)self) + 0x7C))[6] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_hips_b);
    (*(int**)(((char*)self) + 0x7C))[7] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_lowerspine);
    (*(int**)(((char*)self) + 0x7C))[8] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_middlespine);
    (*(int**)(((char*)self) + 0x7C))[9] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_upperspine);
    (*(int**)(((char*)self) + 0x7C))[10] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_neck);
    (*(int**)(((char*)self) + 0x7C))[11] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_clavicleleft);
    (*(int**)(((char*)self) + 0x7C))[12] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_handright_b);
    (*(int**)(((char*)self) + 0x7C))[13] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_bicepleft);
    (*(int**)(((char*)self) + 0x7C))[14] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_biceptwistleft);
    (*(int**)(((char*)self) + 0x7C))[15] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_shinleft_b);
    (*(int**)(((char*)self) + 0x7C))[16] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_footleft_b);
    (*(int**)(((char*)self) + 0x7C))[17] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_shinright_b);
    (*(int**)(((char*)self) + 0x7C))[18] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_footright_b);
    (*(int**)(((char*)self) + 0x7C))[19] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_thighleft_b);
    (*(int**)(((char*)self) + 0x7C))[20] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_thighright_b);
    (*(int**)(((char*)self) + 0x7C))[21] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_hips_b);
    (*(int**)(((char*)self) + 0x7C))[22] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_lowerspine);
    (*(int**)(((char*)self) + 0x7C))[23] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_middlespine);
    (*(int**)(((char*)self) + 0x7C))[24] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_forearmleft_b);
    (*(int**)(((char*)self) + 0x7C))[25] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_handleft_b);
    (*(int**)(((char*)self) + 0x7C))[26] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_clavicleright);
    (*(int**)(((char*)self) + 0x7C))[27] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_bicepright);
    (*(int**)(((char*)self) + 0x7C))[28] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_biceptwistright);
    (*(int**)(((char*)self) + 0x7C))[29] = cAnimModel_getBoneSkelIndex(*(void**)(*(char**)self + 0x780), 0, str_forearmright_b);
    int i;
    for (i = 0; i < 30; i++) {
    }
    *(int*)(((char*)self) + 0x80) = 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002DF920);
#ifdef SKIP_ASM
struct sWakeFxF920 {
    char* obj;                  // 0x00
    char pad4[0x24];            // 0x04
    char* p28;                  // 0x28
    char pad2C[0x4];            // 0x2C
    sWakeVecE398 v30;           // 0x30
    sWakeVecE398 v40;           // 0x40
    sWakeVecE398 v50;           // 0x50
    int i60;                    // 0x60
    char pad64[0x10];           // 0x64
    float f74;                  // 0x74
    char pad78[0x8];            // 0x78
    int i80;                    // 0x80
    char pad84[0xC];            // 0x84
    sWakeVecE398 col;           // 0x90
    sWakeVecE398 pos;           // 0xA0
    int iB0;                    // 0xB0
    float fB4;                  // 0xB4
    float fB8;                  // 0xB8
    float fBC;                  // 0xBC
    sWakeVecE398 dir;           // 0xC0
    char* pD0;                  // 0xD0
    int iD4;                    // 0xD4
    char padD8[0x50];           // 0xD8
    int i128;                   // 0x128
    float f12C;                 // 0x12C
};

struct sWakeVtF920 {
    short delta;
    short index;
    sWakeVecE398* (*fn)(void*);
};
struct sWakeVtIF920 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sMtxF920 {
    sWakeVecE398 r0, r1, r2, r3;
};

int cRider_getMotionState(void*);
int cRider_getControlState(void*);
int cRiderAnimBase_getCurrentAnimID(void*, int);
extern "C" void func_002DF448_F920(void*) __asm__("func_002DF448");
extern "C" void func_002DF4D0_F920(void*) __asm__("func_002DF4D0");
extern "C" void func_002E23E0_F920(void*, void*, void*, int, int, float) __asm__("func_002E23E0");
extern "C" void func_002E02B8_F920(void*) __asm__("func_002E02B8");
extern "C" void func_002DFE88_F920(void*) __asm__("func_002DFE88");
extern "C" void func_002E0EE8_F920(void*) __asm__("func_002E0EE8");
extern "C" void func_002E1120_F920(void*) __asm__("func_002E1120");
extern "C" void func_002E1598_F920(void*) __asm__("func_002E1598");
extern "C" void func_002E1A80_F920(void*) __asm__("func_002E1A80");
extern "C" void func_002E2260_F920(void*) __asm__("func_002E2260");
extern "C" void func_002E1F70_F920(void*) __asm__("func_002E1F70");

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wakeLenF920(const sWakeVecE398& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void wakeDivEqF920(sWakeVecE398& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sWakeVecE398 wakeScaleF920(const sWakeVecE398& v, float s)
{
    sWakeVecE398 r;
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
static inline sWakeVecE398 wakeAddF920(const sWakeVecE398& a, const sWakeVecE398& b)
{
    sWakeVecE398 r;
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

static inline sWakeVecE398 wakeRowF920(char* o, int k)
{
    return ((sWakeVecE398*)(*(char**)(*(char**)(o + 0x780) + 0x30) + (*(int*)(o + 0x8A4) << 6)))[k];
}

/**
 * Updates a rider's snow spray effect for this frame: samples the rider's state, then runs each enabled emitter's
 * update.
 *
 * Re-initialises the emitters when +0x60 is set (func_002DF448) and looks up the bones while +0x80 is 0
 * (func_002DF4D0). Sets the lean side +0x12C to +-1 from PressMode, the layer-2 animation (23..28, 37, 38) and
 * IsRidingFakie, or 0; +0xB4 = -mSlewed[0].cur * (1 - |mSlewed[3].cur|), negated when riding fakie; +0xB8 the speed
 * (length of kRiderVt_getVelocity's vector), +0xBC its absolute value and +0xC0 the normalised mVelocity.
 *
 * The spray flag +0xB0 is cleared in motion states 1, 2 and 4 (cRider_getMotionState), in control state 9, or when
 * mIsInNIS is set without (unk_AD0 clear, unk_AFC and unk_B00 set). +0x30/+0x40/+0x50/+0xA0 get rows 1, 2, 0 and 3 of
 * the mBoneBoardChild matrix; +0xD0 the SurfPropCache record for mSurfaceType. A sharp slow-down (speed drop * 60 above
 * 555.6 at a speed above 555.6, or at least 27.8 in the NIS case) reports a burst through func_002E23E0. The emitters
 * are re-chosen (func_002E2550) when mLOD changed, the colour (+0x90) becomes the doubled table colour for the rider
 * (as in cBoardWakeFX_createSeg) with alpha 1, and every emitter whose enable word (+0x174) is set runs its update: 0
 * func_002E0EE8, 1 func_002E02B8, 3 func_002DFE88, 4 func_002E1120, 5 func_002E1598, 7 func_002E1A80, 8 func_002E1F70,
 * 9 func_002E2260. cRiderHandler_updateRiderFX and cAI_update call it on cRiderHandler + 0xB40.
 */
extern "C" void func_002DF920(sWakeFxF920* self)
{
    if (self->i60 != 0) {
        func_002DF448_F920(self);
    }
    if (self->i80 == 0) {
        func_002DF4D0_F920(self);
    }
    self->f12C = 0.0f;
    cRider* obj = (cRider*)self->obj;
    if (obj->PressMode != 0) {
        int animId = cRiderAnimBase_getCurrentAnimID(obj->mRiderAnim, 2);
        if ((animId >= 23 && animId <= 28) || animId == 37 || animId == 38) {
            self->f12C = *(int*)(self->obj + 0x320) ? -1.0f : 1.0f;
        } else {
            self->f12C = *(int*)(self->obj + 0x320) ? 1.0f : -1.0f;
        }
    }
    obj = (cRider*)self->obj;
    self->fB4 = -obj->mSlewed[0].cur * (1.0f - wakeAbsE398(obj->mSlewed[3].cur));
    if (obj->IsRidingFakie != 0) {
        self->fB4 = -self->fB4;
    }
    char* sub = self->obj + 0x6C0;
    sWakeVtF920* vt = *(sWakeVtF920**)sub;
    float sp = wakeLenF920(*vt[2].fn(sub + vt[2].delta));
    self->fB8 = sp;
    self->fBC = wakeAbsE398(sp);
    self->dir = *(sWakeVecE398*)(self->obj + 0x1E0);
    if (0.0f < sp) {
        wakeDivEqF920(self->dir, wakeLenF920(self->dir));
    }
    self->iB0 = 1;
    int st = cRider_getMotionState(self->obj);
    self->iD4 = st;
    if (st == 1 || st == 2 || st == 4 || cRider_getControlState(self->obj) == 9) {
        self->iB0 = 0;
    } else {
        cRider* riderObj = (cRider*)self->obj;
        if (riderObj->mIsInNIS != 0) {
            int nisSpray = riderObj->mHiddenNIS == 0 && riderObj->mNISShowBoardVisualFX != 0;
            if (!nisSpray || riderObj->mOnGroundNIS == 0) {
                self->iB0 = 0;
            }
        }
    }
    cRider* riderObj = (cRider*)self->obj;
    self->v50 = wakeRowF920(((char*)riderObj), 0);
    float acc = (self->f74 - self->fB8) * 59.999996185302734f;
    self->v30 = wakeRowF920(((char*)riderObj), 1);
    self->v40 = wakeRowF920(((char*)riderObj), 2);
    self->pos = wakeRowF920(((char*)riderObj), 3);
    self->pD0 = (char*)&SSXApp->Game->SurfPropCache->SurfData[riderObj->mSurfaceType];
    int x = 0;
    if (555.5555419921875f < acc && 555.5555419921875f < self->fB8) {
        x = cRider_getControlState(riderObj) != 9;
    }
    float burstStrength = acc * 0.5f;
    cRider* nisRider = (cRider*)self->obj;
    int y = 0;
    if (nisRider->mIsInNIS != 0 && nisRider->mHiddenNIS == 0) {
        y = nisRider->mNISShowBoardVisualFX != 0;
    }
    if (y) {
        x = 27.77777862548828f <= acc;
        burstStrength = acc;
    }
    if (self->iB0 != 0 && x) {
        sWakeVecE398 burstPos = wakeAddF920(self->pos, wakeScaleF920(*(sWakeVecE398*)(self->obj + 0x370), 10.0f));
        func_002E23E0_F920(self, &burstPos, self->obj + 0x370, *(int*)(self->obj + 0x438), 1, burstStrength);
    }
    self->f74 = self->fB8;
    if (self->i128 != *(int*)(self->obj + 0x898)) {
        func_002E2550((cDynamicSprayFX*)self);
    }
    char* sub2 = self->obj + 0x6C0;
    sWakeVtIF920* vt2 = *(sWakeVtIF920**)sub2;
    int idx = vt2[7].fn(sub2 + vt2[7].delta);
    sEnt_E058* colourRow = &((sEnt_E058*)(cRenderStateMan_m_state + 0x28))[idx];
    sWakeVecE398 groundColour;
    groundColour.x = colourRow->col.R;
    groundColour.y = colourRow->col.G;
    groundColour.z = colourRow->col.B;
    groundColour.w = colourRow->col.A;
    self->col = groundColour;
    wakeScaleE398(self->col, self->col, 2.0f);
    self->col.w = 1.0f;
    if (*(int*)(self->p28 + 0x384)) func_002E02B8_F920(self);
    if (*(int*)(self->p28 + 0x7A4)) func_002DFE88_F920(self);
    if (*(int*)(self->p28 + 0x174)) func_002E0EE8_F920(self);
    if (*(int*)(self->p28 + 0x9B4)) func_002E1120_F920(self);
    if (*(int*)(self->p28 + 0xBC4)) func_002E1598_F920(self);
    if (*(int*)(self->p28 + 0xFE4)) func_002E1A80_F920(self);
    if (*(int*)(self->p28 + 0x1404)) func_002E2260_F920(self);
    if (*(int*)(self->p28 + 0x11F4)) func_002E1F70_F920(self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002DFE88);
#ifdef SKIP_ASM
// FAKEMATCH: identical if/else arms `if (self) { m = k; } else { m = k; }` inside the `k < 1.0f` arm of the m clamp
// (the extra branch changes
// cse1's path, so the clamps' 1.0 keeps its own copy instead of being folded into fs0); plain form 99.53%.
// Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void cDynamicColourEmitter_update(void* em, void* pos, void* d, void* vel, int alive, float dt);

struct sWakeFxFE88 {
    char* obj;                  // 0x00
    char pad4[0x24];            // 0x04
    char* p28;                  // 0x28
    char* p2C;                  // 0x2C
    sWakeVecE398 dir;           // 0x30
    char pad40[0x10];           // 0x40
    sWakeVecE398 v50;           // 0x50
    char pad60[0x30];           // 0x60
    sWakeVecE398 col;           // 0x90
    sWakeVecE398 pos;           // 0xA0
    int iB0;                    // 0xB0
    float fB4;                  // 0xB4
    float fB8;                  // 0xB8
    float fBC;                  // 0xBC
    sWakeVecE398 vC0;           // 0xC0
    char* pD0;                  // 0xD0
};

struct sWakeVtFE88 {
    short delta;
    short index;
    sWakeVecE398* (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sWakeVecE398 wakeScaleFE88(const sWakeVecE398& v, float s)
{
    sWakeVecE398 r;
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
static inline sWakeVecE398 wakeAddFE88(const sWakeVecE398& a, const sWakeVecE398& b)
{
    sWakeVecE398 r;
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sWakeVecE398 wakeCrossFE88(const sWakeVecE398& a, const sWakeVecE398& b)
{
    sWakeVecE398 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float wakeDotFE88(const sWakeVecE398& a, const sWakeVecE398& b)
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

// PORT: g++ minimum operator (<?).
static inline float wakeClampFE88(float v, float lo, float hi)
{
    float r;
    if (v >= lo) {
        r = v <? hi;
    } else {
        r = lo;
    }
    return r;
}

/**
 * Updates spray emitter 3 (+0x28 + 0x630): a side spray thrown off the board edge while carving.
 *
 * Runs only while spraying (+0xB0) and when a random number is below the surface record's chance (+0xD0, +0x6C). The
 * emission point is 15 units ahead of the board (+0xA0 plus +0x30 * 15), scattered by func_002DE398. The emit chance
 * combines the rider's lean (|mSlewed[3].cur|), the carve value +0xB4 and how far the board faces across its velocity
 * (rider +0x3B0 against +0xC0), scaled down below a speed of about 555. The velocity mixes the rider velocity,
 * mGroundNormal and the board's side direction with settings set 3 (+0x2C, +0x390..+0x39C), the last two scaled by the
 * speed (+0xBC). Otherwise the emitter is updated with nothing to emit. Steps are 1/60 s
 * (cDynamicColourEmitter_update).
 */
extern "C" void func_002DFE88(sWakeFxFE88* self)
{
    void* em = self->p28 + 0x630;
    if (self->iB0 != 0) {
        float lim = *(float*)(self->pD0 + 0x6C);
        if (wakeRandE398() < lim) {
        sWakeVecE398 pos = wakeAddFE88(self->pos, wakeScaleFE88(self->dir, 15.0f));
        func_002DE398((cDynamicSprayFX*)self, &pos);
        float k = wakeAbsE398(wakeDotFE88(*(sWakeVecE398*)(self->obj + 0x3B0), self->vC0)) * 1.5f;
        float faceFactor;
        if (k < 1.0f) {
            if (self) {
                faceFactor = k;
            } else {
                faceFactor = k;
            }
        } else {
            faceFactor = 1.0f;
        }
        float leanFactor = (wakeAbsE398(*(float*)(self->obj + 0x214)) - 0.2f) * 3.0f;
        float carveFactor = (wakeAbsE398(self->fB4) - -0.2f) * 0.8f + faceFactor;
        float leanChance = wakeClampFE88(leanFactor, 0.0f, 1.0f) * 0.3f;
        float w = leanChance < wakeClampFE88(carveFactor, 0.0f, 1.0f) ? wakeClampFE88(carveFactor, 0.0f, 1.0f) : leanChance;
        float dt = 0.01666666753590107f;
        w *= wakeClampFE88((self->fB8 * 0.035999998450279236f - 20.0f) * dt, 0.0f, 1.0f);
        int alive = wakeRandE398() < w;
        sWakeVecE398 side = wakeCrossFE88(self->v50, *(sWakeVecE398*)(self->obj + 0x370));
        if (self->fB4 < 0.0f) {
            sWakeVecE398 n = wakeScaleFE88(side, -1.0f);
            side = n;
        }
        char* sub = self->obj + 0x6C0;
        sWakeVtFE88* vt = *(sWakeVtFE88**)sub;
        sWakeVecE398* riderVel = vt[2].fn(sub + vt[2].delta);
        sWakeVecE398 vel = wakeAddFE88(
            wakeAddFE88(
                wakeAddFE88(wakeScaleFE88(*riderVel, *(float*)(self->p2C + 0x390)),
                            wakeScaleFE88(*(sWakeVecE398*)(self->obj + 0x370), *(float*)(self->p2C + 0x394))),
                wakeScaleFE88(wakeScaleFE88(*(sWakeVecE398*)(self->obj + 0x370), *(float*)(self->p2C + 0x398)), self->fBC)),
            wakeScaleFE88(wakeScaleFE88(side, *(float*)(self->p2C + 0x39C)), self->fBC));
        cDynamicColourEmitter_update(em, &pos, &vel, &self->col, alive, dt);
        return;
        }
    }
    cDynamicColourEmitter_update(em, &self->pos, 0, 0, 0, 0.01666666753590107f);
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E02B8);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E0EE8);
#ifdef SKIP_ASM
struct sWakeFx0EE8 {
    char* obj;                  // 0x00
    char pad4[0x24];            // 0x04
    char* em;                   // 0x28
    char* p2C;                  // 0x2C
    sWakeVecE398 dir;           // 0x30
    char pad40[0x50];           // 0x40
    sWakeVecE398 col;           // 0x90
    sWakeVecE398 pos;           // 0xA0
    int iB0;                    // 0xB0
    char padB4[0x8];            // 0xB4
    float fBC;                  // 0xBC
    char padC0[0x10];           // 0xC0
    char* pD0;                  // 0xD0
};

struct sWakeVt0EE8 {
    short delta;
    short index;
    sWakeVecE398* (*fn)(void*);
};

extern "C" void cDynamicColourEmitter_update(void* em, void* pos, void* d, void* vel, int alive, float dt);

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sWakeVecE398 wakeScale0EE8(const sWakeVecE398& v, float s)
{
    sWakeVecE398 r;
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
static inline sWakeVecE398 wakeAdd0EE8(const sWakeVecE398& a, const sWakeVecE398& b)
{
    sWakeVecE398 r;
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

static inline float wakeLimit0EE8()
{
    return 222.22222900390625f;
}

/**
 * Updates spray emitter 0 (+0x28): a spray from the board while moving fast on a spraying surface.
 *
 * Emits when spraying (+0xB0), the speed (+0xBC) is above 222.2, the surface record (+0xD0) flag +0x58 is set and the
 * word at the rider's +0x88C object + 0xA0 is clear. The emission point is 15 units ahead of the board (+0xA0 plus
 * +0x30 * 15), scattered by func_002DE398; the velocity mixes the rider velocity and mGroundNormal with settings set 0
 * (+0x2C, +0xD8..+0xE0), the last scaled by the speed. Otherwise the emitter is updated with nothing to emit.
 */
extern "C" void func_002E0EE8(sWakeFx0EE8* self)
{
    int flag = *(int*)(*(char**)(self->obj + 0x88C) + 0xA0);
    char* em = self->em;
    if (self->iB0 != 0 && wakeLimit0EE8() < self->fBC && *(int*)(self->pD0 + 0x58) != 0 && flag == 0) {
        sWakeVecE398 pos = wakeAdd0EE8(self->pos, wakeScale0EE8(self->dir, 15.0f));
        func_002DE398((cDynamicSprayFX*)self, &pos);
        char* sub = self->obj + 0x6C0;
        sWakeVt0EE8* vt = *(sWakeVt0EE8**)sub;
        sWakeVecE398* riderVel = vt[2].fn(sub + vt[2].delta);
        sWakeVecE398 vel = wakeAdd0EE8(
            wakeAdd0EE8(wakeScale0EE8(*riderVel, *(float*)(self->p2C + 0xD8)),
                        wakeScale0EE8(*(sWakeVecE398*)(self->obj + 0x370), *(float*)(self->p2C + 0xDC))),
            wakeScale0EE8(wakeScale0EE8(*(sWakeVecE398*)(self->obj + 0x370), *(float*)(self->p2C + 0xE0)), self->fBC));
        cDynamicColourEmitter_update(em, &pos, &vel, &self->col, 1, 0.01666666753590107f);
    } else {
        cDynamicColourEmitter_update(em, &self->pos, 0, 0, 0, 0.01666666753590107f);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E10B8);
#ifdef SKIP_ASM
extern int D_004A3AFC;

struct sWake10B8 {
    char pad0[0x18];
    int f18;
    char pad1C[0x4];
    int f20;
    float f24;
};

// PORT: the seed is reinterpreted as a float in [1,2) through memory (type pun).
static inline float wakeRand10B8()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

/**
 * Initialises the head-puff timing: blink on (+0x18 = 1), timer (+0x20) 0 and the period (+0x24) 0.4 to 0.5 s at
 * random.
 *
 * Nothing in the retail code references it; func_002E1120 runs the puffs from these fields.
 */
extern "C" void func_002E10B8(sWake10B8* self)
{
    self->f18 = 1;
    self->f20 = 0;
    self->f24 = wakeRand10B8() * 0.09999999403953552f + 0.4000000059604645f;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E1120);
#ifdef SKIP_ASM
extern "C" void cDynamicColourEmitter_update(void* em, void* pos, void* d, void* vel, int alive, float dt);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void* self, int i);
extern "C" float cRenderStateMan_Temperature(int i);

struct sVec1120 {
    float x, y, z, w;
    sVec1120() {}
    sVec1120(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sRow1120 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat1120 {
    sRow1120 v[4];
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    sMat1120(const sMat1120& o)
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
            : "r"(this), "r"(&o)
            : "memory");
    }
} __attribute__((aligned(16)));

extern sRow1120 t4vect_Origin_1120 __asm__("t4vect_Origin");

struct sWakeFx1120 {
    char* obj;                  // 0x00
    char pad4[0x10];            // 0x04
    float rate;                 // 0x14
    int blink;                  // 0x18
    float glow;                 // 0x1C
    float timer;                // 0x20
    float period;               // 0x24
    char* p28;                  // 0x28
    char* p2C;                  // 0x2C
    char pad30[0x8C];           // 0x30
    float fBC;                  // 0xBC
};

struct sWakeVtI1120 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sWakeVtV1120 {
    short delta;
    short index;
    sVec1120* (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector).
static inline sVec1120 wakeMulMat1120(const sMat1120* m, const sVec1120& v)
{
    sVec1120 r;
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
static inline sVec1120 wakeScale1120(const sVec1120& v, float s)
{
    sVec1120 r;
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
static inline sVec1120 wakeAdd1120(const sVec1120& a, const sVec1120& b)
{
    sVec1120 r;
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void wakeAddEq1120(sVec1120& a, const sVec1120& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: g++ minimum operator (<?).
static inline float wakeClamp1120(float v, float lo, float hi)
{
    float r;
    if (v >= lo) {
        r = v <? hi;
    } else {
        r = lo;
    }
    return r;
}

/**
 * Updates spray emitter 4 (+0x28 + 0x840): blinking puffs in front of the rider's head.
 *
 * The emission point is (-3, 0, 4) in the head bone's frame (mBoneHead). Above a speed (+0xBC) of 1111.1 it emits
 * nothing and clears the rate (+0x14). Otherwise the glow (+0x1C) jumps to 1 in animation category 30 (layer 2,
 * cRiderAnimBase_getCurrentAnimCategory) and decays by 0.1 a second otherwise; the blink flag (+0x18) toggles every
 * period (+0x24: 0.4 s on, 0.4 to 1.0 s off by the glow). The alpha is (glow + 0.5) * (1 - (value + 5) * 0.04), clamped
 * to 0..2, where value is cRenderStateMan_Temperature for the rider (kRiderVt_getAIIndex), so the puffs fade out as that value rises
 * to 20. While blinking on with alpha above 0.02 the rate grows by 18 to 24 a second and each whole unit emits one
 * white puff with settings set 4 (+0x2C, +0x478..+0x480) plus 150 units along the head's z axis.
 */
extern "C" void func_002E1120(sWakeFx1120* self)
{
    void* em = self->p28 + 0x840;
    cRider* obj = (cRider*)self->obj;
    int k = obj->mBoneHead;
    sMat1120* mats = (sMat1120*)obj->mAnimModel->SkelMatrices;
    sMat1120 mat = mats[k];
    float back = -3.0f;
    float up = 4.0f;
    float one = 1.0f;
    sVec1120 pos = wakeMulMat1120(&mat, sVec1120(back, 0.0f, up, one));
    if (1111.111083984375f < self->fBC) {
        self->rate = 0.0f;
        cDynamicColourEmitter_update(em, &pos, 0, 0, 0, 0.01666666753590107f);
        return;
    } else {
        float target = 0.0f;
        if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(self->obj + 0x784), 2) == 30) {
            target = 1.0f;
        }
        if (self->glow <= target) {
            self->glow = target;
        } else {
            self->glow -= 0.0016666668234393f;
        }
        self->timer += 0.01666666753590107f;
        if (self->period <= self->timer) {
            self->timer = 0.0f;
            self->blink ^= 1;
            if (self->blink != 0) {
                self->period = 0.4000000059604645f;
            } else {
                self->period = (1.0f - self->glow) * 0.6000000238418579f + 0.4000000059604645f;
            }
        }
        char* sub = self->obj + 0x6C0;
        sWakeVtI1120* vi = *(sWakeVtI1120**)sub;
        float stateValue = cRenderStateMan_Temperature(vi[7].fn(sub + vi[7].delta));
        float zero = 0.0f;
        float one2 = 1.0f;
        float puffAlpha = wakeClamp1120((self->glow + 0.5f) * (one2 - (stateValue + 5.0f) * 0.03999999910593033f), zero, 2.0f);
        sVec1120 col(one2, one2, one2, puffAlpha);
        sVec1120* pcol = &col;
        if (self->blink == 0 || !(0.019999999552965164f < puffAlpha)) {
            goto off;
        }
        {
            float dt = 0.01666666753590107f;
            self->rate += ((wakeRandE398()) * 6.0f + 18.0f) * dt;
            if (1.0f < self->rate) {
                self->rate -= 1.0f;
                mat.v[3] = t4vect_Origin_1120;
                char* sub2 = self->obj + 0x6C0;
                sWakeVtV1120* vv = *(sWakeVtV1120**)sub2;
                sVec1120* riderVel = vv[2].fn(sub2 + vv[2].delta);
                sVec1120 vel = wakeAdd1120(
                    wakeAdd1120(wakeScale1120(*riderVel, *(float*)(self->p2C + 0x478)),
                                wakeScale1120(*(sVec1120*)(self->obj + 0x370), *(float*)(self->p2C + 0x47C))),
                    wakeScale1120(wakeScale1120(*(sVec1120*)(self->obj + 0x370), *(float*)(self->p2C + 0x480)), self->fBC));
                wakeAddEq1120(vel, wakeMulMat1120(&mat, sVec1120(zero, zero, 150.0f, one2)));
                cDynamicColourEmitter_update(em, &pos, &vel, pcol, 1, dt);
                return;
            }
        }
    }
    cDynamicColourEmitter_update(em, &pos, 0, 0, 0, 0.01666666753590107f);
    return;
off:
    self->rate = 0.0f;
    cDynamicColourEmitter_update(em, &pos, 0, 0, 0, 0.01666666753590107f);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E1598);
#ifdef SKIP_ASM
struct sWakeFx1598 {
    char* obj;                  // 0x00
    float f4;                   // 0x04
    char pad8[0x20];            // 0x08
    char* p28;                  // 0x28
    char pad2C[0x64];           // 0x2C
    sWakeVecE398 col;           // 0x90
    sWakeVecE398 pos;           // 0xA0
    char padB0[0x20];           // 0xB0
    char* pD0;                  // 0xD0
    int iD4;                    // 0xD4
    char padD8[0x8];            // 0xD8
    float fE0;                  // 0xE0
    char padE4[0xC];            // 0xE4
    sWakeVecE398 vF0;           // 0xF0
    sWakeVecE398 dir;           // 0x100
    char pad110[0x4];           // 0x110
    int i114;                   // 0x114
    char pad118[0x8];           // 0x118
    float f120;                 // 0x120
    int i124;                   // 0x124
};

extern "C" void ReportImpact(void*, float);

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float wakeDot1598(const sWakeVecE398& a, const sWakeVecE398& b)
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline sWakeVecE398 wakeSub1598(const sWakeVecE398& a, const sWakeVecE398& b)
{
    sWakeVecE398 r;
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

struct sWakeVt1598 {
    short delta;
    short index;
    sWakeVecE398* (*fn)(void*);
};

// PORT: g++ minimum operator (<?).
static inline float wakeClamp1598(float v, float lo, float hi)
{
    float r;
    if (v >= lo) {
        r = v <? hi;
    } else {
        r = lo;
    }
    return r;
}

static inline float wakeMin1598(float a, float b)
{
    return a <= b ? a : b;
}

static inline sWakeVecE398* wakeVel1598(sWakeFx1598* self)
{
    char* sub = self->obj + 0x6C0;
    sWakeVt1598* vt = *(sWakeVt1598**)sub;
    return vt[2].fn(sub + vt[2].delta);
}

/**
 * Updates spray emitters 5 and 6 (+0x28 + 0xA50, + 0xC60): the burst thrown up by a sharp slow-down.
 *
 * Runs while the burst strength (+0xE0) is above 27.8 and the surface record (+0xD0) flag +0x60 is set; otherwise both
 * emitters stop and the strength is cleared. Emitter 6 is used, or emitter 5 when the surface flag +0x64 is set and the
 * strength is above 277.8; with emitter 5 and no NIS (mIsInNIS) the burst point +0xF0 is updated by ReportImpact. The
 * point is moved along the burst direction (+0x100) by 20 (emitter 6) or 45 and scattered by up to 25 (60 for a player
 * on a +0x64 surface) in x and y. The level +0x4 grows with strength / 3333 (to 2 in motion state 2, 1.5 with +0x114
 * clear, else 1), the alpha +0x120 eases toward 1 (0.5 with +0x114 set), and the velocity is the rider velocity minus
 * 0.75 of its mGroundNormal part, times 0.4 (0.3 with +0x114 set). Below 83.3 the strength drops to 0, otherwise it is
 * multiplied by the surface's +0x68 (times 0.4 for a non-player).
 */
// PORT: g++ minimum operator (<?).
extern "C" void func_002E1598(sWakeFx1598* self)
{
    tSurfData* cfg = (tSurfData*)self->pD0;
    if (27.77777862548828f < self->fE0 && cfg->ImpactActive != 0) {
        int emitterIdx = 6;
        int otherIdx = 5;
        if (cfg->ImpactLarge != 0 && 277.77777099609375f < self->fE0) {
            emitterIdx = 5;
            otherIdx = 6;
        }
        if (emitterIdx == 5 && *(int*)(self->obj + 0xAC4) == 0) {
            ReportImpact(&self->vF0, self->fE0);
        }
        sWakeVecE398 pos = self->vF0;
        float k = 1.0f;
        if (emitterIdx == 6) {
            k = 0.5f;
            wakeAddE398(pos, wakeScale0EE8(self->dir, 20.0f));
        } else {
            wakeAddE398(pos, wakeScale0EE8(self->dir, 45.0f));
        }
        float lim = 3333.33349609375f;
        float strengthFrac = wakeClamp1598(self->fE0 / lim, 0.0f, 1.0f);
        float level = self->f4;
        if (level < 1.0f || emitterIdx == 5) {
            if (self->iD4 == 2) {
                if (level < 2.0f) {
                    float n = level + (strengthFrac + strengthFrac) * k;
                    self->f4 = wakeMin1598(n, 2.0f);
                }
            } else if (self->i114 == 0) {
                if (level < 1.5f) {
                    self->f4 = (level + strengthFrac * (k * 1.5f)) <? 2.0f;
                }
            } else {
                if (level < 1.0f) {
                    self->f4 = (level + strengthFrac * k) <? 2.0f;
                }
            }
        }
        float j = 25.0f;
        if (cfg->ImpactLarge != 0 && self->i124 != 0) {
            j = 60.0f;
        }
        float lo = -j;
        {
            sWakeVecE398 t;
            t.z = 0.0f;
            t.w = 0.0f;
            t.x = lo + (j - lo) * wakeRandE398();
            t.y = lo + (j - lo) * wakeRandE398();
            wakeAddE398(pos, t);
        }
        sWakeVecE398* va = wakeVel1598(self);
        sWakeVecE398* vb = wakeVel1598(self);
        sWakeVecE398* up = (sWakeVecE398*)(self->obj + 0x370);
        sWakeVecE398 t = wakeSub1598(*va, wakeScale0EE8(*up, wakeDot1598(*vb, *up) * 0.75f));
        float m = 0.30000001192092896f;
        if (self->i114 == 0) m = 0.4000000059604645f;
        sWakeVecE398 vel = wakeScale0EE8(t, m);
        float al = 0.5f;
        if (self->i114 == 0) al = 1.0f;
        if (self->f120 < al) {
            self->f120 = al;
        } else {
            self->f120 = self->f120 * 0.800000011920929f + al * 0.19999998807907104f;
        }
        sWakeVecE398 col = self->col;
        col.w = self->f120;
        cDynamicColourEmitter_update(self->p28 + emitterIdx * 528, &pos, &vel, &col, 1, 0.01666666753590107f);
        cDynamicColourEmitter_update(self->p28 + otherIdx * 528, &self->vF0, 0, 0, 0, 0.01666666753590107f);
        if (self->fE0 < 83.33333587646484f) {
            self->fE0 = 0.0f;
        } else {
            float m = cfg->ImpactMultiplier;
            if (self->i124 == 0) {
                m *= 0.4000000059604645f;
            }
            self->fE0 = self->fE0 * m;
        }
    } else {
        cDynamicColourEmitter_update(self->p28 + 0xC60, &self->pos, 0, 0, 0, 0.01666666753590107f);
        cDynamicColourEmitter_update(self->p28 + 0xA50, &self->pos, 0, 0, 0, 0.01666666753590107f);
        self->fE0 = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E1A80);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// ---- Shared declarations: one type per real thing, used by the functions below ----


extern int D_004A3AFC;

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector vu0Scale_1A80(const t4Vector& v, float s)
{
    t4Vector r;
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
static inline t4Vector vu0Add(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline t4Vector vu0Sub_1A80(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only inline asm (float absolute value).
static inline float fsAbs(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: the seed is reinterpreted as a float in [1,2) through memory (type pun).
static inline float wakeRand()
{
    D_004A3AFC = ((D_004A3AFC * 0x18FCD + 0xE9507C) & 0x7FFFFF) | 0x3F800000;
    return *(float*)&D_004A3AFC - 1.0f;
}

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq_1A80(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_1A80(const t4Vector& a, const t4Vector& b)
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

extern "C" void cDynamicColourEmitter_update(void* em, void* pos, void* d, void* vel, int alive, float dt);

struct sWakeFx1A80 {
    char* obj;                  // 0x00
    char pad4[0x24];            // 0x04
    char* em;                   // 0x28
    char* p2C;                  // 0x2C
    char pad30[0x20];           // 0x30
    t4Vector v50;               // 0x50
    char pad60[0x30];           // 0x60
    t4Vector col;               // 0x90
    t4Vector pos;               // 0xA0
    int iB0;                    // 0xB0
    float fB4;                  // 0xB4
    float fB8;                  // 0xB8
    float fBC;                  // 0xBC
    t4Vector vC0;               // 0xC0
    char* pD0;                  // 0xD0
    char padD4[0x58];           // 0xD4
    float f12C;                 // 0x12C
};

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector wakeCross1A80(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline float wakeRamp1A80(float lo, float range, float v)
{
    return (v - lo) / range;
}

static inline t4Vector* wakeVel1A80(sWakeFx1A80* self)
{
    char* sub = self->obj + 0x6C0;
    sVEntry<t4Vector*(void*)>* vt = *(sVEntry<t4Vector*(void*)>**)sub;
    return vt[2].fn(vthis(sub, vt[2]));
}

/**
 * Updates spray emitter 7 (+0x28 + 0xE70): snow kicked up when the board faces across its motion at speed.
 *
 * The threshold is (|mSlewed[0].cur| + the facing factor - 0.1) * ramp(speed 416.7..1250) squared, where the facing
 * factor is twice |rider +0x3B0 . +0xC0| capped at 1. Emits when spraying (+0xB0), a random value in 0..0.4 is below
 * it, the surface record flag +0x50 is set and the lean side (+0x12C) is near 0. The point is lifted along
 * mGroundNormal by the surface's +0x54 times a random 0.5..1.5 and pulled back along mVelocity; the velocity mixes the
 * rider velocity and mGroundNormal with settings set 7 (+0x2C, +0x730..+0x738) plus 0.3 of (the board's forward axis
 * times -speed + mVelocity).
 */
extern "C" void func_002E1A80(sWakeFx1A80* self)
{
    float facing = fsAbs(vu0Dot_1A80(*(t4Vector*)(self->obj + 0x3B0), self->vC0));
    facing = facing + facing;
    char* em = self->em + 0xE70;
    float k = 1.0f;
    if (facing <= 1.0f)
        k = facing;
    float ak = fsAbs(k);
    float speedRamp = wakeRamp1A80(416.66668701171875f, 1250.0f, self->fB8);
    float leanSum = fsAbs(*(float*)(self->obj + 0x1F0)) + ak - 0.10000000149011612f;
    float rampSq = 1.0f;
    if (speedRamp < 1.0f)
        rampSq = speedRamp;
    rampSq *= rampSq;
    float thr = leanSum * rampSq;
    float roll = wakeRand() * 0.4000000059604645f;
    if (self->iB0 != 0 && roll <= thr && *(int*)(self->pD0 + 0x50) != 0 && fsAbs(self->f12C) < 0.20000000298023224f) {
        t4Vector pos = self->pos;
        float spd = *(float*)(self->pD0 + 0x54);
        float lift = wakeRand() + 0.5f;
        vu0AddEq_1A80(pos, vu0Sub_1A80(vu0Scale_1A80(*(t4Vector*)(self->obj + 0x370), lift * spd),
                             vu0Scale_1A80(vu0Scale_1A80(vu0Scale_1A80(*(t4Vector*)(self->obj + 0x1E0), 3.0f), 0.01666666753590107f), lift)));
        t4Vector side = wakeCross1A80(self->v50, *(t4Vector*)(self->obj + 0x370));
        if (self->fB4 < 0.0f)
            side = vu0Scale_1A80(side, -1.0f);
        t4Vector n = *(int*)(self->obj + 0x320) != 0 ? vu0Scale_1A80(self->v50, -1.0f) : self->v50;
        t4Vector boardBack = vu0Scale_1A80(vu0Scale_1A80(n, -1.0f), self->fB8);
        t4Vector negVel = vu0Scale_1A80(*(t4Vector*)(self->obj + 0x1E0), -1.0f);
        t4Vector w = vu0Scale_1A80(vu0Sub_1A80(boardBack, negVel), 0.30000001192092896f);
        t4Vector* riderVel = wakeVel1A80(self);
        t4Vector vel = vu0Add(
            vu0Add(vu0Add(vu0Add(vu0Scale_1A80(*riderVel, *(float*)(self->p2C + 0x730)), vu0Scale_1A80(vu0Scale_1A80(side, self->fB8), 0.0f)),
                          vu0Scale_1A80(*(t4Vector*)(self->obj + 0x370), *(float*)(self->p2C + 0x734))),
                   vu0Scale_1A80(vu0Scale_1A80(*(t4Vector*)(self->obj + 0x370), *(float*)(self->p2C + 0x738)), self->fBC)),
            w);
        cDynamicColourEmitter_update(em, &pos, &vel, &self->col, 1, 0.01666666753590107f);
    } else {
        cDynamicColourEmitter_update(em, &self->pos, 0, 0, 0, 0.01666666753590107f);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E1F70);
#ifdef SKIP_ASM
struct sWakeFx1F70 {
    char* obj;                  // 0x00
    char pad4[0xC];             // 0x04
    float f10;                  // 0x10
    char pad14[0x14];           // 0x14
    char* p28;                  // 0x28
    char* p2C;                  // 0x2C
    sWakeVecE398 dir;           // 0x30
    char pad40[0x50];           // 0x40
    sWakeVecE398 col;           // 0x90
    sWakeVecE398 pos;           // 0xA0
    int iB0;                    // 0xB0
    char padB4[0x8];            // 0xB4
    float fBC;                  // 0xBC
    char padC0[0x10];           // 0xC0
    char* pD0;                  // 0xD0
};

struct sWakeVt1F70 {
    short delta;
    short index;
    sWakeVecE398* (*fn)(void*);
};

// PORT: g++ minimum operator (<?).
static inline float wakeClamp1F70(float v, float lo, float hi)
{
    float r;
    if (v >= lo) {
        r = v <? hi;
    } else {
        r = lo;
    }
    return r;
}

static inline float wakeLimit1F70()
{
    return 277.77777099609375f;
}

static inline sWakeVecE398* wakeVel1F70(sWakeFx1F70* self)
{
    char* sub = self->obj + 0x6C0;
    sWakeVt1F70* vt = *(sWakeVt1F70**)sub;
    return vt[2].fn(vthis(sub, vt[2]));
}

/**
 * Updates spray emitter 8 (+0x28 + 0x1080): a steady powder trail whose level follows the surface.
 *
 * While spraying (+0xB0) with a surface record level (+0xD0, +0x5C) of at least 0.01 the level +0x10 rises by 0.01 a
 * frame up to that value and nothing is emitted that frame; otherwise it falls by 1/60. It drops to 0 when the speed
 * (+0xBC) is below 277.8 or, not spraying, the velocity's z is below -3333.3. With a positive level it emits at the
 * board (+0xA0, scattered by func_002DE398) with alpha level * 1.25 (faded as the rider falls), velocity from the rider
 * velocity and +0x30 with settings set 8 (+0x2C, +0x818..+0x820).
 */
// PORT: g++ minimum/maximum operators (<?, >?).
extern "C" void func_002E1F70(sWakeFx1F70* self)
{
    int done = 0;
    void* em = self->p28 + 0x1080;
    if (self->iB0 == 0 || *(float*)(self->pD0 + 0x5C) < 0.009999999776482582f) {
        self->f10 = (self->f10 - 0.01666666753590107f) >? 0.0f;
    } else {
        done = 1;
        self->f10 = (self->f10 + 0.010000000707805157f) <? *(float*)(self->pD0 + 0x5C);
    }
    if ((self->iB0 == 0 && wakeVel1F70(self)->z < -3333.33349609375f) || self->fBC < wakeLimit1F70()) {
        self->f10 = 0.0f;
    }
    sWakeVecE398 pos = self->pos;
    if (!done && 0.0f < self->f10) {
        float alpha = wakeClamp1F70(self->f10 * 1.25f, 0.0f, 1.0f);
        if (wakeVel1F70(self)->z < 0.0f) {
            alpha *= wakeClamp1F70((wakeVel1F70(self)->z + 3333.33349609375f) / 3333.33349609375f, 0.0f, 1.0f);
        }
        sWakeVecE398 col = self->col;
        col.w = alpha;
        sWakeVecE398* riderVel = wakeVel1F70(self);
        sWakeVecE398 vel = wakeAdd0EE8(
            wakeAdd0EE8(wakeScale0EE8(*riderVel, *(float*)(self->p2C + 0x818)),
                        wakeScale0EE8(self->dir, *(float*)(self->p2C + 0x81C))),
            wakeScale0EE8(wakeScale0EE8(self->dir, *(float*)(self->p2C + 0x820)), self->fBC));
        func_002DE398((cDynamicSprayFX*)self, &pos);
        cDynamicColourEmitter_update(em, &pos, &vel, &col, 1, 0.01666666753590107f);
    } else {
        cDynamicColourEmitter_update(em, &self->pos, 0, 0, 0, 0.01666666753590107f);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E2260);
#ifdef SKIP_ASM

struct sWakeFx2260 {
    char* obj;                  // 0x00
    float f4;                   // 0x04
    char pad8[0x20];            // 0x08
    char* p28;                  // 0x28
    char* p2C;                  // 0x2C
    char pad30[0x48];           // 0x30
    int i78;                    // 0x78
    int* p7C;                   // 0x7C
    char pad80[0x10];           // 0x80
    sWakeVecE398 col;           // 0x90
    char padA0[0x18];           // 0xA0
    float fB8;                  // 0xB8
    char padBC[0x18];           // 0xBC
    int iD4;                    // 0xD4
};

extern "C" void cDynamicColourEmitter_update(void* em, void* pos, void* d, void* vel, int alive, float dt);

// PORT: g++ minimum operator (<?).
static inline float wakeClamp2260(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

static inline float wakeLimit2260()
{
    return 83.33333587646484f;
}

/**
 * Updates spray emitter 9 (+0x28 + 0x1290): snow shed from the rider's body bones after a fall.
 *
 * While the level (+0x4) is positive and the speed (+0xB8) above 83.3 it emits one particle a frame at the next of the
 * 30 bones in the table (+0x7C, cursor +0x78), with alpha level * 1.5 and velocity mVelocity times settings set 9's
 * +0x900 (+0x2C), and lowers the level by 1/60 unless in motion state 2 (+0xD4). Otherwise the emitter is updated at
 * the hips bone (mBoneHips) with nothing to emit.
 */
// PORT: g++ maximum operator (>?).
extern "C" void func_002E2260(sWakeFx2260* self)
{
    float zero = 0.0f;
    void* em = self->p28 + 0x1290;
    if (zero < self->f4 && wakeLimit2260() < self->fB8) {
        float dt;
        float alpha;
        sWakeVecE398 col;
        sWakeVecE398 pos;
        sWakeVecE398 vel;
        sWakeVecE398 scaledVel;
        cRider* obj;
        t4Matrix* boneMats;
        int k;
        alpha = wakeClamp2260(self->f4 * 1.5f, zero, 1.0f);
        col = self->col;
        col.w = alpha;
        dt = 0.01666666753590107f;
        obj = (cRider*)self->obj;
        k = self->p7C[self->i78];
        boneMats = obj->mAnimModel->SkelMatrices;
        pos = ((sWakeVecE398*)&((t4Matrix*)((char*)boneMats + (k << 6)))->row[0])[3];
        wakeScaleE398(scaledVel, *(sWakeVecE398*)&obj->mVelocity, *(float*)(self->p2C + 0x900));
        vel = scaledVel;
        cDynamicColourEmitter_update(em, &pos, &vel, &col, 1, dt);
        self->i78++;
        if (self->i78 >= 30) {
            self->i78 = 0;
        }
        if (self->iD4 != 2) {
            self->f4 = (self->f4 - dt) >? zero;
        }
    } else {
        cRider* obj = (cRider*)self->obj;
        int k = obj->mBoneHips;
        t4Matrix* boneMats = obj->mAnimModel->SkelMatrices;
        sWakeVecE398 pos = ((sWakeVecE398*)&((t4Matrix*)((char*)boneMats + (k << 6)))->row[0])[3];
        cDynamicColourEmitter_update(em, &pos, 0, 0, 0, 0.01666666753590107f);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E23E0);
#ifdef SKIP_ASM

// PORT: PS2-only inline asm (float absolute value).
static inline float Abs_23E0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_23E0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector length via vsqrt).
static inline float Length_23E0(const t4Vector& v)
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

struct sRiderState_23E0 {
    char pad[0xAC4];
    int fAC4;
    char padAC8[8];
    int fAD0;
    char padAD4[0x28];
    int fAFC;
    int fB00;
};

static inline int Carving_23E0(sRiderState_23E0* r)
{
    return r->fAC4 != 0 && r->fAD0 == 0 && r->fAFC != 0;
}

struct sWake_23E0 {
    sRiderState_23E0* rider;    // 0x00
    char pad4[0xDC];
    float lastSpeed;            // 0xE0
    char padE4[0xC];
    t4Vector pos;               // 0xF0
    t4Vector dir;               // 0x100
    int f110;                   // 0x110
    int f114;                   // 0x114
    char pad118[0xC];
    int f124;                   // 0x124
};

/**
 * Records a spray burst when it is stronger than the current one and far enough from it.
 *
 * Takes it when |@p speed| exceeds the stored strength (+0xE0) and the point is more than 180 units (100 for a player,
 * +0x124) from the stored point (+0xF0), or the rider is in the NIS state that allows a trail (mIsInNIS set, unk_AD0
 * clear, unk_AFC and unk_B00 set). It then stores the strength (doubled when @p isSlowdown is 0), @p pos (+0xF0), @p dir
 * (+0x100), @p surfaceType (+0x110) and @p isSlowdown (+0x114). func_002DF920 reports slow-downs with @p isSlowdown = 1; func_00111AA0 (wipeout
 * impacts) passes 0. func_002E1598 plays the burst.
 *
 * @param pos  the burst position
 * @param dir  the burst direction (the surface normal)
 * @param surfaceType  the surface type
 * @param isSlowdown  1 for a slow-down spray, 0 for an impact
 * @param speed  the burst strength
 */
extern "C" void func_002E23E0(sWake_23E0* self, t4Vector* pos, t4Vector* dir, int surfaceType, int isSlowdown, float speed)
{
    float absSpeed = Abs_23E0(speed);
    float distance = Abs_23E0(Length_23E0(Sub_23E0(self->pos, *pos)));
    float maxd = 180.0f;
    if (self->f124 != 0)
        maxd = 100.0f;
    if (self->lastSpeed < absSpeed) {
        if (maxd < distance || (Carving_23E0(self->rider) && self->rider->fB00 != 0)) {
            if (isSlowdown == 0)
                self->lastSpeed = absSpeed + absSpeed;
            else
                self->lastSpeed = absSpeed;
            self->pos = *pos;
            self->dir = *dir;
            self->f110 = surfaceType;
            self->f114 = isSlowdown;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E24D0);
#ifdef SKIP_ASM
extern "C" void cDynamicColourEmitter_render(void* self, int mode);


struct func_002E24D0_sFx {
    void* owner;                    // 0x0
    char pad_0x04[0x24];
    cDynamicColourEmitter* wakes;     // 0x28
};

/**
 * Draws a rider's snow spray: renders each enabled emitter that has live particles, when the rider is on screen.
 *
 * Does nothing unless the rider's mIsOnScreen is set. Of the ten emitters (+0x28) it renders those whose enable word
 * (+0x174) is set and whose particle count (+0x1E0) is positive, with cDynamicColourEmitter_render mode 7.
 * cRiderHandler_renderRiderFX and cAI_render call it on cRiderHandler + 0xB40.
 */
extern "C" void func_002E24D0(func_002E24D0_sFx* self)
{
    if (*(int*)((char*)self->owner + 0xB18) != 0) {
        int i;
        for (i = 0; i < 10; i++) {
            cDynamicColourEmitter* w = &self->wakes[i];
            if (((void*)w->m_enabled) != 0 && w->m_enabledCount > 0) {
                cDynamicColourEmitter_render(w, 7);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E2550);
#ifdef SKIP_ASM
#define S28(off) (*(int*)((char*)*(void**)((char*)self + 0x28) + (off)))

/**
 * Chooses which spray emitters run for the rider's detail level (mLOD), which it also stores at +0x128.
 *
 * Sets the enable word (+0x174) of each 0x210-byte emitter at +0x28. Level 0: emitters 0, 5 and 6 on, 4 on for a player
 * (+0x124) or in an NIS (mIsInNIS), 1, 2, 3, 7, 8 and 9 on for a player. Level 1: 0, 5 and 6 on, 1, 2, 3, 7 and 8 for a
 * player, 4 and 9 off. Other levels: 5 on, 6 for a player, the rest off. func_002DF3B0 and func_002DF920 (when mLOD
 * changes) call it.
 */
extern "C" void func_002E2550(cDynamicSprayFX* self)
{
    int mode = *(int*)((char*)*(void**)self + 0x898);
    *(int*)((char*)self + 0x128) = mode;
    switch (mode) {
    case 0:
        S28(3012) = 1;
        S28(3540) = 1;
        S28(372) = 1;
        {
            int enabled = 0;
            if (*(int*)((char*)self + 0x124) != 0 || *(int*)((char*)*(void**)self + 0xAC4) != 0)
                enabled = 1;
            S28(2484) = enabled;
        }
        S28(900) = *(int*)((char*)self + 0x124);
        S28(1428) = *(int*)((char*)self + 0x124);
        S28(1956) = *(int*)((char*)self + 0x124);
        S28(4068) = *(int*)((char*)self + 0x124);
        S28(4596) = *(int*)((char*)self + 0x124);
        S28(5124) = *(int*)((char*)self + 0x124);
        break;
    case 1:
        S28(3012) = 1;
        S28(3540) = 1;
        S28(372) = 1;
        S28(900) = *(int*)((char*)self + 0x124);
        S28(1428) = *(int*)((char*)self + 0x124);
        S28(1956) = *(int*)((char*)self + 0x124);
        S28(4068) = *(int*)((char*)self + 0x124);
        S28(4596) = *(int*)((char*)self + 0x124);
        S28(2484) = 0;
        S28(5124) = 0;
        break;
    default:
        S28(3012) = 1;
        S28(3540) = *(int*)((char*)self + 0x124);
        S28(900) = 0;
        S28(372) = 0;
        S28(1428) = 0;
        S28(1956) = 0;
        S28(4068) = 0;
        S28(2484) = 0;
        S28(4596) = 0;
        S28(5124) = 0;
        break;
    }
}
#undef S28
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E26B8);
#ifdef SKIP_ASM
struct sWakeVt26B8 { short delta; short index; void (*fn)(void*, void*, float); };

/**
 * Tunes the burst emitters 5 and 6 for a player or a computer rider and re-initialises them.
 *
 * Stores whether the rider has a view (mViewIndex >= 0) at +0x124. Sets settings sets 5 and 6 (+0x2C, 0xE8 bytes each)
 * at +0x10, +0x18 and +0xCC to 55, 25, 170 and 30, 20, 60 for a player, or 24.75, 5, 76.5 and 19.5, 8, 39 otherwise,
 * then re-initialises emitters 5 and 6 through their virtual slot 3 with their settings and float +0x58. The
 * cDynamicSprayFX constructor and func_002DF3B0 call it.
 *
 * @param vself  the spray effect (cDynamicSprayFX, this)
 */
extern "C" void func_002E26B8(cDynamicSprayFX* vself)
{
    int on = *(int*)(*(char**)vself + 0x870) >= 0;
    *(int*)(((char*)vself) + 0x124) = on;
    if (on) {
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x498) = 55.0f;
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x4A0) = 25.0f;
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x554) = 170.0f;
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x580) = 30.0f;
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x588) = 20.0f;
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x63C) = 60.0f;
    } else {
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x498) = 24.75f;
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x4A0) = 5.0f;
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x554) = 76.5f;
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x580) = 19.5f;
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x588) = 8.0f;
        *(float*)(*(char**)(((char*)vself) + 0x2C) + 0x63C) = 39.0f;
    }
    {
        char* emitters = *(char**)(((char*)vself) + 0x28);
        char* emitter = emitters + 0xA50;
        sWakeVt26B8* vt = *(sWakeVt26B8**)(emitters + 0xC48);
        vt[3].fn(vthis(emitter, vt[3]), *(char**)(((char*)vself) + 0x2C) + 0x488, *(float*)(emitters + 0xAA8));
    }
    {
        char* emitters = *(char**)(((char*)vself) + 0x28);
        char* emitter = emitters + 0xC60;
        sWakeVt26B8* vt = *(sWakeVt26B8**)(emitters + 0xE58);
        vt[3].fn(vthis(emitter, vt[3]), *(char**)(((char*)vself) + 0x2C) + 0x570, *(float*)(emitters + 0xCB8));
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_getHaloRadius);
#ifdef SKIP_ASM
/**
 * Returns the radius of a light halo type: 180 for 0x10, 100 for 0x20, 350 for 0x40, otherwise 0.
 *
 * The halo resolver reaches it through getHaloRadius (cChunkResolve_resolveHalo); cLightHalo_Render draws the halos with
 * the same sizes.
 *
 * @param type  the halo type bits (flags & 0x70)
 * @return the radius
 */
extern "C" float cLensFxMan_getHaloRadius(int type)
{
    switch (type) {
    case 0x10:
        return 180.0f;
    case 0x20:
        return 100.0f;
    case 0x40:
        return 350.0f;
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cHalo_cHalo__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a halo: does nothing and returns @p self.
 *
 * cLensFxMan_cLensFxMan runs it on each of its halo slots, and the sun halo constructor (src/seg/seg_1F1548.cpp) on its
 * base.
 *
 * @return @p self
 */
void* cHalo_cHalo(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLightHalo_Render);
#ifdef SKIP_ASM
struct sV4_2868 { float x, y, z, w; sV4_2868(const float& a, const float& b, const float& c, const float& d) { x = a; y = b; z = c; w = d; } };
struct sV2_2868 { float x, y; sV2_2868() { x = 0.0f; y = 0.0f; } sV2_2868(const float& a, const float& b) { x = a; y = b; } };

struct sRsState_2868 {
    int f0;                 // 0x0
    int flagsA;             // 0x4
    int flagsB;             // 0x8
    int fC;                 // 0xC
    short tex;              // 0x10
    short pad;
};

struct sRsVEnt_2868 {
    short delta;
    short index;
    void (*fn)(void*, sV4_2868*, sV2_2868*, sV2_2868*, sV2_2868*, tARGB*, float, int);
};

struct sRsCtx_2868 {
    char pad0[0xE84];
    sRsState_2868* top;         // 0xE84
    char pad1[0x1024 - 0xE88];
    int tex1024;                // 0x1024
    int tex1028;                // 0x1028
    char pad2[0x10D8 - 0x102C];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void Draw(const sV4_2868& pos, const sV2_2868& size, const sV2_2868& uv0, const sV2_2868& uv1, const tARGB& col, float rot, int flags);
};



extern sRsCtx_2868* cGraphicsMan_TheGraphicsMan_2868 __asm__("cGraphicsMan_TheGraphicsMan");

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_2868(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int rsTex1024_2868(sRsCtx_2868* ctx)
{
    return ctx->tex1024;
}

static inline int rsTex1028_2868(sRsCtx_2868* ctx)
{
    return ctx->tex1028;
}

static inline void rsSetA_2868(sRsCtx_2868* ctx, int mask, int shift, int v)
{
    ctx->top->flagsA = (ctx->top->flagsA & ~mask) | ((v << shift) & mask);
}

/**
 * Draws one light halo as a camera-facing sprite in the halo's colour.
 *
 * Does nothing unless the halo is on screen (+0x2C). The colour's red, green and blue are the halo record's (+0x20)
 * vector at +0x10 normalised to unit length. With a z-buffer sample (+0x2E) the alpha is the record's visibility for
 * view @p idx (+0x40 + 4 * @p idx) and the render state's flag A bits 23-24 are set to 2, otherwise the alpha is 1 with
 * bits 23-24 = 1; bits 20-21 are cleared and bits 12-19 set to 20 either way. Skips a non-positive alpha. The halo type
 * (record flags & 0x70) picks the texture and size: 0x10 GrMan +0x1024 at 180, 0x20 GrMan +0x1028 at 100, 0x40 GrMan
 * +0x1024 at 350. Draws at the record's position (+0x1C) with UVs 0..1 and rotation +0x28 through GrMan virtual slot
 * 79. cLensFxMan_DrawLens calls it for every halo on screen.
 *
 * @param idx  the view index
 */
extern "C" void cLightHalo_Render(cLightHalo* self, int idx)
{
    if (self->mbOnScreen == 0) {
        return;
    }
    t3Vector n;
    tHaloRecord* haloRecord = (self->mpHalo);
    float x = (*(t3Vector*)&haloRecord->m_fvColor[0]).x;
    float y = (*(t3Vector*)&haloRecord->m_fvColor[0]).y;
    float z = (*(t3Vector*)&haloRecord->m_fvColor[0]).z;
    float len = Sqrt_2868(x * x + y * y + z * z);
    if (len != 0.0f) {
        float inv = 1.0f / len;
        n.x = x * inv;
        n.y = y * inv;
        n.z = z * inv;
    } else {
        n.x = (*(t3Vector*)&haloRecord->m_fvColor[0]).x;
        n.y = (*(t3Vector*)&haloRecord->m_fvColor[0]).y;
        n.z = (*(t3Vector*)&haloRecord->m_fvColor[0]).z;
    }
    tARGB col;
    (*(t3Vector*)&col.R).x = n.x;
    (*(t3Vector*)&col.R).y = n.y;
    (*(t3Vector*)&col.R).z = n.z;
    sRsCtx_2868* ctx = cGraphicsMan_TheGraphicsMan_2868;
    if (self->mbRequestZSample != 0) {
        col.A = (self->mpHalo)->m_fOcclude[idx];
        rsSetA_2868(ctx, 0x1800000, 23, 2);
        rsSetA_2868(ctx, 0x300000, 20, 0);
        rsSetA_2868(ctx, 0xFF000, 12, 20);
    } else {
        col.A = 1.0f;
        rsSetA_2868(ctx, 0x1800000, 23, 1);
        rsSetA_2868(ctx, 0x300000, 20, 0);
        rsSetA_2868(ctx, 0xFF000, 12, 20);
    }
    if (col.A <= 0.0f) {
        return;
    }
    float size = 0.0f;
    switch ((unsigned int)(((int)(self->mpHalo)->m_iFlags) & 0x70)) {
    case 0x10:
        ctx->top->tex = rsTex1024_2868(ctx);
        size = 180.0f;
        break;
    case 0x20:
        ctx->top->tex = rsTex1028_2868(ctx);
        size = 100.0f;
        break;
    case 0x40:
        ctx->top->tex = rsTex1024_2868(ctx);
        size = 350.0f;
        break;
    }
    tHaloRecord* drawRecord = (self->mpHalo);
    ctx->Draw(sV4_2868((*(t3Vector*)&drawRecord->m_fvWorldPos[0]).x, (*(t3Vector*)&drawRecord->m_fvWorldPos[0]).y, (*(t3Vector*)&drawRecord->m_fvWorldPos[0]).z, 1.0f), sV2_2868(size, size), sV2_2868(), sV2_2868(1.0f, 1.0f), col, self->mAngle, 2);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLightHalo_Process);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char* cGraphicsMan_TheGraphicsMan_boardwakefx __asm__("cGraphicsMan_TheGraphicsMan");

struct sV3c_2B00 {
    float x, y, z;
    sV3c_2B00(const float& a, const float& b, const float& c) { x = a; y = b; z = c; }
};

struct sV4c_2B00 {
    float x, y, z, w;
    sV4c_2B00(const sV3c_2B00& v, const float& d) { x = v.x; y = v.y; z = v.z; w = d; }
} __attribute__((aligned(16)));


// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_2B00(const t4Vector& a, const sV4c_2B00& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

static inline int clampI_2B00(int v, int lo, int hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

/**
 * Projects a light halo for a view: decides whether it is on screen and sets its z-sample rectangles, depth and angle.
 *
 * Clips the halo record's position (+0x20, +0x1C) with GrMan virtual slot 91: when clipped, clears +0x2C and +0x2E.
 * Otherwise it sets +0x2C and projects the point with slot 92; at a depth (z * 2^-24) of 0.005 or less it stops there.
 * Else it sets +0x2E, the sample rectangle +0x0..+0xC (depth * 800 by depth * 400 pixels, clamped to 16 by 8, centred
 * on the point) and the 16 by 8 rectangle +0x10..+0x1C clamped inside the view rectangle (@p vrect +0x0..+0xC). +0x24
 * becomes the projected depth of the point moved toward the camera by 100, 80 or 200 units by type (along the view
 * matrix column at @p vrect +0x18, +0x28, +0x38), and +0x28 the angle (screen x mapped to -1..1) * pi/2. cLensFxMan_ProcessLens
 * calls it for each halo of the view.
 *
 * @param vself  the halo
 * @param vrect  the view information: view rectangle at +0x0, view matrix from +0x10
 */
extern "C" void cLightHalo_Process(cLightHalo* vself, void* vrect)
{
    cLightHalo* self = vself;
    float* rect = (float*)vrect;
    cPS2GraphicsMan* obj = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_boardwakefx;
    t4Vector pos;
    pos.x = (*(t3Vector*)&(self->mpHalo)->m_fvWorldPos[0]).x;
    pos.y = (*(t3Vector*)&(self->mpHalo)->m_fvWorldPos[0]).y;
    pos.z = (*(t3Vector*)&(self->mpHalo)->m_fvWorldPos[0]).z;
    pos.w = 1.0f;
    sVEntry<int(void*, t4Vector*)>* vEntry = &((sVEntry<int (void*, t4Vector*)>*)VPTR_AT(obj, 0x10D8))[91];
    if (vEntry->fn(vthis(obj, *vEntry), &pos)) {
        self->mbOnScreen = 0;
        self->mbRequestZSample = 0;
    } else {
        self->mbOnScreen = 1;
        t4Vector scr;
        sVEntry<void(t4Vector*, void*, t4Vector*)>* projectSlot = &((sVEntry<void (t4Vector*, void*, t4Vector*)>*)VPTR_AT(obj, 0x10D8))[92];
        projectSlot->fn(&scr, vthis(obj, *projectSlot), &pos);
        float depth = scr.z * 5.960465188081798e-08f;
        if (depth <= 0.005f) {
            return;
        }
        {
            int w = (int)(depth * 800.0f);
            int pixelHeight = (int)(depth * 400.0f);
            self->mbRequestZSample = 1;
            int cw = clampI_2B00(w, 1, 16);
            pixelHeight = clampI_2B00(pixelHeight, 1, 8);
            self->mZRect.X = scr.x - (float)(cw >> 1);
            self->mZRect.Y = scr.y - (float)(pixelHeight >> 1);
            self->mZRect.W = (float)cw;
            self->mZRect.H = (float)pixelHeight;
            self->mZSRect.X = scr.x - 8.0f;
            self->mZSRect.Y = scr.y - 4.0f;
            self->mZSRect.W = 16.0f;
            self->mZSRect.H = 8.0f;
            self->mZSRect.X = wakeClampD0B8(self->mZSRect.X, 0.0f, (rect[0] + rect[2]) - self->mZSRect.W);
            self->mZSRect.Y = wakeClampD0B8(self->mZSRect.Y, 0.0f, (rect[1] + rect[3]) - self->mZSRect.H);
            float pullBack = 0.0f;
            switch ((unsigned int)(((int)(self->mpHalo)->m_iFlags) & 0x70)) {
            case 0x10:
                pullBack = 100.0f;
                break;
            case 0x20:
                pullBack = 80.0f;
                break;
            case 0x40:
                pullBack = 200.0f;
                break;
            }
            t3Vector viewDir;
            viewDir.x = rect[6];
            viewDir.y = rect[10];
            viewDir.z = rect[14];
            t4Vector lp = Sub_2B00(pos, sV4c_2B00(sV3c_2B00(viewDir.x * pullBack, viewDir.y * pullBack, viewDir.z * pullBack), 0.0f));
            t4Vector sc;
            sVEntry<void(t4Vector*, void*, t4Vector*)>* projectSlot2 = &((sVEntry<void (t4Vector*, void*, t4Vector*)>*)VPTR_AT(obj, 0x10D8))[92];
            projectSlot2->fn(&sc, vthis(obj, *projectSlot2), &lp);
            self->mZDist = sc.z;
            // screen x mapped to -1..1, then to an angle
            float screenFrac = scr.x - rect[0];
            screenFrac = (screenFrac + screenFrac) / rect[2] - 1.0f;
            self->mAngle = screenFrac * 1.5707964f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_cLensFxMan);
#ifdef SKIP_ASM

struct sWakePair2E18 {
    int a;
    int b;
};

struct sWakeMgr2E18 {
    char pad0[0x60];
    sWakePair2E18 pairs[1];     // 0x60
};

extern void* cLensFxMan_vtbl[];
extern char* cGraphicsMan_TheGraphicsMan_boardwakefx __asm__("cGraphicsMan_TheGraphicsMan");
extern int cLensFxMan_mbDrawSun;
void* cHalo_cHalo(void* self);
void cLensFxMan__drawFrameCallBack(void* self);
extern "C" void cSunHalo_cSunHalo(void* self);

/**
 * Constructs the lens effects manager base: its element arrays, then registers its callback with the graphics
 * manager.
 *
 * It builds 2 x 256 elements of 0x30 bytes at +0x68 (cHalo_cHalo) and two 0x210-byte objects at +0x6070
 * (cSunHalo_cSunHalo), sets +0x6494 to 1 and clears the pair at +0x60. It then passes cLensFxMan__drawFrameCallBack and itself to
 * graphics manager slot 117 and sets cLensFxMan_mbDrawSun. The PS2 version's constructor (cPS2LensFxMan_cPS2LensFxMan) calls this.
 *
 * @return self
 */
// PORT: hand-written g++ 2.95 member-array construction loops (vptr at 0x649C).
extern "C" void* cLensFxMan_cLensFxMan(cLensFxMan* self)
{
    self->mVtbl = cLensFxMan_vtbl;
    {
        char* rowPtr = (char*)&self->mLightHalo[0];
        int rowsLeft = 0;
        do {
            char* colPtr = rowPtr;
            int colsLeft = 1;
            do {
                char* elemPtr = colPtr;
                int elemsLeft = 0xFF;
                do {
                    cHalo_cHalo(elemPtr);
                    elemPtr += 0x30;
                } while (--elemsLeft != -1);
                colPtr += 0x3000;
            } while (--colsLeft != -1);
            rowPtr += 0x6000;
        } while (--rowsLeft != -1);
    }
    {
        char* rowPtr = (char*)&self->mSunHalo[0];
        int rowsLeft = 0;
        do {
            char* colPtr = rowPtr;
            int colsLeft = 1;
            do {
                cSunHalo_cSunHalo(colPtr);
                colPtr += 0x210;
            } while (--colsLeft != -1);
            rowPtr += 0x420;
        } while (--rowsLeft != -1);
    }
    self->mbLocked = 1;
    {
        int i;
        for (i = 0; i < 1; i++) {
            ((sWakeMgr2E18*)self)->pairs[i].a = 0;
            ((sWakeMgr2E18*)self)->pairs[i].b = 0;
        }
    }
    {
        cPS2GraphicsMan* obj = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_boardwakefx;
        sVEntry<void(void*, void*, void*)>* setPhase2Slot = &((sVEntry<void (void*, void*, void*)>*)VPTR_AT(obj, 0x10D8))[117];
        setPhase2Slot->fn(vthis(obj, *setPhase2Slot), (void*)cLensFxMan__drawFrameCallBack, self);
    }
    cLensFxMan_mbDrawSun = 1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_Reset__FPvi);
#ifdef SKIP_ASM
/**
 * Empties the lens manager's halo list for one view: sets its count (+0x60 + 4 * @p i) to 0.
 *
 * cGame_renderLightHalos calls it on the game's LensFxMan before adding the view's halos.
 *
 * @param i  the view index
 */
void cLensFxMan_Reset(void* vself, int i)
{
    cLensFxMan* self = (cLensFxMan*)vself;
    self->mNumHalo[i] = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_AddHalo);
#ifdef SKIP_ASM
struct sWakePoint {
    int a;
    int pad04;
    int c;
    short d;
    short e;
    char pad10[0x20];
};

struct sWakeFx {
    char pad00[0x60];
    int counts[10];
    sWakePoint points[1][256];
};

/**
 * Adds a halo record to the lens manager's list for one view.
 *
 * Writes the next 0x30-byte halo of view @p i (count at +0x60 + 4 * @p i): its record (+0x20) is @p haloRecord and the on-screen
 * and sample flags and the word at +0x28 are cleared; then the count goes up. There is no bound check against the 256
 * slots. cLensFxMan_AddLightHalos calls it for each record.
 *
 * @param haloRecord  the halo record
 * @param i  the view index
 */
extern "C" void cLensFxMan_AddHalo(sWakeFx* self, int haloRecord, int i)
{
    int n = self->counts[i];
    self->points[i][n].a = haloRecord;
    self->points[i][n].d = 0;
    self->points[i][n].e = 0;
    self->points[i][n].c = 0;
    self->counts[i]++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_AddLightHalos);
#ifdef SKIP_ASM
extern "C" void cLensFxMan_AddHalo(sWakeFx* self, int v, int i);

/**
 * Adds an array of halo records to the lens manager's list for one view (cLensFxMan_AddHalo for each).
 *
 * cGame_renderLightHalos calls it twice, with clip codes 0 and 2.
 *
 * @param vals  the halo records
 * @param count  number of records
 * @param ignored  the clip code, unused
 * @param i  the view index
 */
extern "C" void cLensFxMan_AddLightHalos(cLensFxMan* self, int* vals, int count, int ignored, int i)
{
    int n;
    for (n = 0; n < count; n++) {
        cLensFxMan_AddHalo(((sWakeFx*)self), vals[n], i);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan__cLensFxMan);
#ifdef SKIP_ASM
struct sBWVEntry2 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

extern void* cLensFxMan_vtbl[];
extern char* cGraphicsMan_TheGraphicsMan_boardwakefx __asm__("cGraphicsMan_TheGraphicsMan");
extern int cLensFxMan_mbDrawSun;
void operator_delete(int* p);

/**
 * Destroys the lens effects manager: unregisters its graphics-manager callback (GrMan slot 117 with 0, 0), clears
 * cLensFxMan_mbDrawSun and frees the object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cLensFxMan__cLensFxMan(cLensFxMan* self, int flags)
{
    cPS2GraphicsMan* obj = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_boardwakefx;
    self->mVtbl = cLensFxMan_vtbl;
    sBWVEntry2* setPhase2Slot = &((sBWVEntry2*)VPTR_AT(obj, 0x10D8))[117];
    setPhase2Slot->fn(vthis(obj, *setPhase2Slot), 0, 0);
    cLensFxMan_mbDrawSun = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_render);
#ifdef SKIP_ASM
extern "C" void cLensFxMan_ProcessLens(cLensFxMan* self, int i);
extern "C" void cLensFxMan_DrawLens(cLensFxMan* self, int i);

/**
 * Renders the light halos of one view: processes them (cLensFxMan_ProcessLens), then draws them (cLensFxMan_DrawLens).
 *
 * cGame_renderLightHalos calls it on the game's LensFxMan.
 *
 * @param i  the view index
 */
extern "C" void cLensFxMan_render(cLensFxMan* self, int i)
{
    cLensFxMan_ProcessLens(self, i);
    cLensFxMan_DrawLens(self, i);
}
#endif

extern "C" void cLensFxMan_SampleLens(cLensFxMan* self);

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan__drawFrameCallBack__FPv);
#ifdef SKIP_ASM
/**
 * Frame callback of the lens manager: runs cLensFxMan_SampleLens (the visibility sampling) on it.
 *
 * cLensFxMan_cLensFxMan registers it with the graphics manager, passing the manager as the parameter.
 */
void cLensFxMan__drawFrameCallBack(void* self)
{
    cLensFxMan_SampleLens((cLensFxMan*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_SampleLens);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sLight3130 {
    char pad0[0x40];
    float vis[2];               // 0x40, per view
};

struct sHalo3130 {
    char pad0[0x20];
    sLight3130* light;          // 0x20
    char pad24[0xA];
    unsigned short active;      // 0x2E
};

struct sSun3130 {
    char pad0[0x2E];
    unsigned short active;      // 0x2E
    char pad30[0x204 - 0x30];
    float vis;                  // 0x204
    char pad208[0x210 - 0x208];
} __attribute__((aligned(16)));

typedef sVEntry<int(void*, void*, float*)> sVE3130;

struct sLens3130 {
    char pad0[0x60];
    int haloCount[1][2];        // 0x60
    sHalo3130 halos[1][2][256]; // 0x68
    char pad6068[8];
    sSun3130 suns[1][2];        // 0x6070
    int viewCount;              // 0x6490
    int f6494;                  // 0x6494
    int pad6498;
    sVE3130* vt;                // 0x649C
};

// PORT: retail reads the "previous" bank [!bufferId] = [1] although the arrays have one bank (an out-of-range read in
// the original). These two views give that read a second bank in range, so no compiler can treat it as undefined;
// it reads the same memory after the first bank as retail. A port should give the class a real previous bank.
struct sLensHaloBanks3130 {
    char pad0[0x68];
    sHalo3130 halos[2][2][256]; // 0x68
};


/**
 * Samples the z-buffer visibility of the sun and of every active light halo, per view.
 *
 * Does nothing while +0x6494 is set. For each of the +0x6490 views: when cLensFxMan_mbDrawSun is set and the view's sun
 * (+0x6070, 0x210 bytes each) is active (+0x2E), stores the value from the manager's virtual slot 4 (sun, &value) at
 * the sun's +0x204, keeping the previous bank's value when the slot returns 0. Each active halo of the view does the
 * same into its record's visibility for that view (record +0x40 + 4 * view). The arrays have one bank, so the
 * previous-bank read goes past it (see the code's note). cLensFxMan__drawFrameCallBack calls it once a frame.
 *
 * @param selfp  the lens manager (this)
 */
// FAKEMATCH: the invented zero-valued bank index `int bufferId = 0;` (as landed cLensFxMan_DrawLens's `bank`), so
// retail's
// zeroed register times 0x6000 is kept; plain form (constant bank 0, prev bank 1 through the same views) 79.77%.
// Byte-identical and behaves like the plain form; replace it when an honest spelling matches.
extern "C" void cLensFxMan_SampleLens(cLensFxMan* selfp)
{
    sLens3130* self = (sLens3130*)selfp;
    if (self->f6494 == 0) {
        // Double-buffered state with a bank dimension of one: bank 0 is current, the previous bank is !bufferId.
        int bufferId = 0;
        if (cLensFxMan_mbDrawSun != 0) {
            for (int viewIdx = 0; viewIdx < self->viewCount; viewIdx++) {
                int prev = !bufferId;
                if (self->suns[bufferId][viewIdx].active != 0) {
                    float visibility = 0;
                    sVE3130* vEntry = &self->vt[4];
                    if (vEntry->fn(vthis(self, *vEntry), &self->suns[bufferId][viewIdx], &visibility)) {
                        self->suns[bufferId][viewIdx].vis = visibility;
                    } else {
                        self->suns[bufferId][viewIdx].vis = ((sSun3130 (*)[2])&((cLensFxMan*)self)->mSunHalo[0])[prev][viewIdx].vis;
                    }
                }
            }
        }
        for (int viewIdx = 0; viewIdx < self->viewCount; viewIdx++) {
            int prev = !bufferId;
            for (int i = 0; i < self->haloCount[bufferId][viewIdx]; i++) {
                if (self->halos[bufferId][viewIdx][i].active != 0) {
                    float visibility = 0;
                    sVE3130* vEntry = &self->vt[4];
                    if (vEntry->fn(vthis(self, *vEntry), &self->halos[bufferId][viewIdx][i], &visibility)) {
                        self->halos[bufferId][viewIdx][i].light->vis[viewIdx] = visibility;
                    } else {
                        self->halos[bufferId][viewIdx][i].light->vis[viewIdx] = ((sLensHaloBanks3130*)self)->halos[prev][viewIdx][i].light->vis[viewIdx];
                    }
                }
            }
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_ProcessLens);
#ifdef SKIP_ASM
extern "C" void cLightHalo_Process(cLightHalo* part, void* owner);
extern "C" void cSunHalo_Process(void* emit, void* owner, int idx);

struct sWkVec3338 { float x, y, z, w; } __attribute__((aligned(16)));
struct sWkMtx3338 { sWkVec3338 row[4]; };
struct sWkPart3338 { char data[0x30]; };
struct sWkEmit3338 { char data[0x210]; };

struct sWkVtR3338 { short delta; short index; void (*fn)(tRect*, void*); };
struct sWkVtM3338 { short delta; short index; sWkMtx3338* (*fn)(void*); };
struct sWkVtP3338 { short delta; short index; void (*fn)(void*, float*, float*, float*); };

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrix3338(sWkMtx3338* dst, sWkMtx3338* src)
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
        : "r"(dst), "r"(src)
        : "memory");
}

/**
 * Prepares one view's halos: stores the view's rectangle, matrix and projection values, then processes the sun and each
 * halo.
 *
 * Fills +0x0..+0xC from GrMan virtual slot 28 (the view rectangle), copies the matrix from slot 35 to +0x10 and gets
 * three floats into +0x50, +0x54, +0x58 from slot 30. Then processes view @p idx's sun (+0x6070 + @p idx * 0x210) with
 * cSunHalo_Process and each of its halos with cLightHalo_Process. cLensFxMan_render calls it.
 *
 * @param vself  the lens manager (this)
 * @param idx  the view index
 */
extern "C" void cLensFxMan_ProcessLens(cLensFxMan* vself, int idx)
{
    cLensFxMan* self = vself;
    cPS2GraphicsMan* ctx = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_boardwakefx;
    tRect viewRect;
    {
        sWkVtR3338* vt = (sWkVtR3338*)VPTR_AT(ctx, 0x10D8);
        vt[28].fn(&viewRect, vthis(ctx, vt[28]));
    }
    self->mViewInfo.mViewRect = viewRect;
    {
        sWkVtM3338* vt = (sWkVtM3338*)VPTR_AT(ctx, 0x10D8);
        vu0CopyMatrix3338(&(*(sWkMtx3338*)&self->mViewInfo.mViewMat), vt[35].fn(vthis(ctx, vt[35])));
    }
    int i = 0;
    {
        sWkVtP3338* vt = (sWkVtP3338*)VPTR_AT(ctx, 0x10D8);
        vt[30].fn(vthis(ctx, vt[30]), &self->mViewInfo.mFOV, &self->mViewInfo.mZNear, &self->mViewInfo.mZFar);
    }
    sWkEmit3338* em = ((sWkEmit3338*)&self->mSunHalo[0]);
    cSunHalo_Process(&em[idx], self, idx);
    if (i < self->mNumHalo[idx]) {
        sWkPart3338(*pp)[0x100] = ((sWkPart3338 (*)[256])&self->mLightHalo[0][0]);
        sWkPart3338* halo = pp[idx];
        do {
            cLightHalo_Process((cLightHalo*)halo, self);
            i++;
            halo++;
        } while (i < self->mNumHalo[idx]);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_DrawLens);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the invented zero-valued index `bank` over size-1 leading array dimensions (cnt / strip / blk[1][...]), so
// retail's zero register times 0x6000 is kept; plain form (no bank dimension) 94.97%. Byte-identical and behaves like
// the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sBW3478Blk { char d[0x210]; };

struct sBW3478Data {
    char pad0[0x60];
    int cnt[1][2];                      // 0x60
    cLightHalo strip[1][2][0x100];      // 0x68
    char pad6068[0x6070 - 0x6068];
    sBW3478Blk blk[1][2];               // 0x6070
    char pad6490[0x649C - 0x6490];
};

struct sBW3478Fx : sBW3478Data {
    virtual void vf08();
    virtual void vf10();
    virtual void vf18();
};

extern int cLensFxMan_mbDrawSun;
extern "C" void cSunHalo_Render(void* dst, void* src, int i);

/**
 * Draws one view's lens effects: the sun when enabled, then every on-screen light halo, between the manager's begin and
 * end draw calls.
 *
 * Calls cSunHalo_Render for the view's sun (+0x6070 + @p i * 0x210) when cLensFxMan_mbDrawSun is set, the manager's
 * virtual slot 2, cLightHalo_Render for each halo of view @p i with +0x2C set, then virtual slot 3. cLensFxMan_render calls it
 * after cLensFxMan_ProcessLens.
 *
 * @param selfv  the lens manager (this)
 * @param i  the view index
 */
extern "C" void cLensFxMan_DrawLens(cLensFxMan* selfv, int i)
{
    sBW3478Fx* self = (sBW3478Fx*)selfv;
    // The wake arrays have a leading bank dimension of one, indexed through a variable:
    // cLensFxMan_SampleLens keeps it as a zeroed register multiplied by 0x6000 at run time.
    int bank = 0;
    if (cLensFxMan_mbDrawSun != 0) {
        cSunHalo_Render(&self->blk[bank][i], self, i);
    }
    self->vf10();
    for (int n = 0; n < self->cnt[bank][i]; n++) {
        if (self->strip[bank][i][n].mbOnScreen != 0) {
            cLightHalo_Render(&self->strip[bank][i][n], i);
        }
    }
    self->vf18();
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_beginDrawFrame);
#ifdef SKIP_ASM
struct sWakeRState {
    int f0;
    unsigned a0 : 2;
    unsigned a2 : 5;
    unsigned a7 : 15;
    unsigned a22 : 1;
    unsigned a23 : 9;
    unsigned b0 : 5;
    unsigned b5 : 5;
    unsigned b10 : 19;
    unsigned b29 : 3;
    int fC;
    int f10;
};

struct sWakeRCtx {
    char pad[0xE84];
    sWakeRState* top;
};

extern char* cGraphicsMan_TheGraphicsMan_boardwakefx __asm__("cGraphicsMan_TheGraphicsMan");
extern sWakeRState sDrawState_s_defaultDrawState;

static inline void setA22_3578(sWakeRCtx* c, int v) { c->top->a22 = v; }
static inline void setA0_3578(sWakeRCtx* c, int v) { c->top->a0 = v; }
static inline void setB10_3578(sWakeRCtx* c, int v) { c->top->b10 = v; }
static inline void setA2_3578(sWakeRCtx* c, int v) { c->top->a2 = v; }
static inline void setB5_3578(sWakeRCtx* c, int v) { c->top->b5 = v; }

/**
 * Sets up the graphics state for drawing the lens flare effects.
 *
 * Pushes the graphics manager's draw-state stack, resets the new top to sDrawState_s_defaultDrawState and changes a few
 * of its blend, depth and test bit fields.
 */
extern "C" void cLensFxMan_beginDrawFrame(void)
{
    sWakeRCtx* ctx = (sWakeRCtx*)cGraphicsMan_TheGraphicsMan_boardwakefx;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
    *ctx->top = sDrawState_s_defaultDrawState;
    setA22_3578(ctx, 1);
    setA0_3578(ctx, 2);
    setB10_3578(ctx, 0);
    setA2_3578(ctx, 7);
    setB5_3578(ctx, 7);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_endDrawFrame);
#ifdef SKIP_ASM
extern char* GrMan_boardwakefx __asm__("GrMan");

/**
 * Restores the graphics state after the lens flare effects are drawn.
 *
 * Pops one 0x14-byte entry off the draw-state stack (top pointer at GrMan+0xE84).
 */
extern "C" void cLensFxMan_endDrawFrame(void)
{
    *(char**)(GrMan_boardwakefx + 0xE84) -= 0x14;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cPlayerIndicatorFX_cPlayerIndicatorFX);
#ifdef SKIP_ASM
// g++ 2.95 vtable entry (no thunks): {delta, index, fn}. Vtables are double-aligned.
struct sVtEnt3680 {
    short delta;
    short index;
    void* fn;
};

struct sVt9_3680 {
    sVtEnt3680 e[9];
} __attribute__((aligned(8)));

struct sVt22_3680 {
    sVtEnt3680 e[22];
} __attribute__((aligned(8)));

extern const sVt9_3680 D_004889C8;
extern const sVt22_3680 D_00488A10;
extern char D_00459B90[];

struct sWakeFade_3930;
extern "C" void cRider_cRider(void* self);
extern "C" void cPlayerIndicatorFX_reset(cPlayerIndicatorFX* self);

/**
 * Constructs a rider's player indicator (the light beam above another player) and resets its colours.
 *
 * When @p inChrg is set it also constructs the cRider virtual base at +0x40. It patches the rider's vtables
 * (the g++ 2.95 this-adjust fix-ups when not in charge).
 *
 * @param inChrg  nonzero when this object is complete and must construct its virtual base
 * @return self
 */
// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0x40): vtable copies with delta fixups when not in charge.
extern "C" void* cPlayerIndicatorFX_cPlayerIndicatorFX(cPlayerIndicatorFX* self, int inChrg)
{
    sVt9_3680 vtbl1;
    sVt22_3680 vtbl2;
    if (inChrg) {
        *(void**)self = (char*)self + 0x40;
        cRider_cRider((char*)self + 0x40);
    }
    *(void**)(*(char**)self + 0x6E8) = (void*)&D_004889C8;
    *(void**)(*(char**)self + 0x6D0) = D_00459B90;
    *(void**)(*(char**)self + 0x6C0) = (void*)&D_00488A10;
    if (inChrg == 0) {
        int vc;
        vtbl1 = D_004889C8;
        *(void**)(*(char**)self + 0x6E8) = &vtbl1;
        {
            char* vbo = *(char**)self - 0x40;
            vc = (char*)self - vbo;
        }
        vtbl1.e[1].delta = D_004889C8.e[1].delta + vc;
        vtbl2 = D_00488A10;
        *(void**)(*(char**)self + 0x6C0) = &vtbl2;
        vtbl2.e[1].delta = D_00488A10.e[1].delta + vc;
    }
    cPlayerIndicatorFX_reset(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cPlayerIndicatorFX__cPlayerIndicatorFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardwakefx_classes.h"

/**
 * Destroys a player indicator; this part of the destructor has nothing to free.
 */
cVbDtor002E37D0::~cVbDtor002E37D0()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor002E37D0; expose it under the address-derived name.
__asm__(".globl cPlayerIndicatorFX__cPlayerIndicatorFX\n.type cPlayerIndicatorFX__cPlayerIndicatorFX,@function\ncPlayerIndicatorFX__cPlayerIndicatorFX = _$_15cVbDtor002E37D0\n.size cPlayerIndicatorFX__cPlayerIndicatorFX,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002E37D0$13cRiderVbC_bwf = D_004889C8\n"
        "_vt$15cVbDtor002E37D0$14cRiderVbBC_bwf = D_00459B90\n"
        "_vt$15cVbDtor002E37D0$12cRiderVb_bwf = D_00488A10\n"
        "_$_12cRiderVb_bwf = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cPlayerIndicatorFX_reset);
#ifdef SKIP_ASM
struct sWakeVec4_3930 {
    float x;
    float y;
    float z;
    float w;
    sWakeVec4_3930() {}
    sWakeVec4_3930(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};


/**
 * Resets a player indicator's colours; despite the name it reads no replay data (it takes no frame).
 *
 * The pulse runs between (0.5, 1, 0, 0) and (0.7, 1, 0.7, 0.7), starting at the first; +0x4 is cleared.
 */
extern "C" void cPlayerIndicatorFX_reset(cPlayerIndicatorFX* self)
{
    (*(sWakeVec4_3930*)&self->MinColor) = sWakeVec4_3930(0.5f, 1.0f, 0.0f, 0.0f);
    (*(sWakeVec4_3930*)&self->MaxColor) = sWakeVec4_3930(0.699999988079071f, 1.0f, 0.699999988079071f, 0.699999988079071f);
    (*(sWakeVec4_3930*)&self->CurColor) = (*(sWakeVec4_3930*)&self->MinColor);
    (*(int*)&self->PulseAng) = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cPlayerIndicatorFX_update);
#ifdef SKIP_ASM
extern "C" float fCos(float x);
extern float kBeamPulseRate;


/**
 * Pulses a player indicator's colour between its two colours with a sine wave, while its rider is active.
 *
 * The phase advances by kBeamPulseRate / 60 each frame and wraps at 2 pi. Nothing happens when the rider's
 * virtual slot 8 (on its +0x6C0 part) returns 0.
 */
extern "C" void cPlayerIndicatorFX_update(cPlayerIndicatorFX* self)
{
    char* riderVptr = (char*)&self->mRider->mVtbl;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)riderVptr;
    if (vt[8].fn(vthis(riderVptr, vt[8])) == 0) return;
    float half = 0.5f;
    float newPhase = self->PulseAng + kBeamPulseRate * 0.01666666753590107f;
    float turns = newPhase * 0.15915493667125702f + half;
    float wholeTurns;
    // PORT: FPU-only float->int->float round trip (floor helper); no C cast reproduces it.
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(wholeTurns) : "f"(turns));
    if (turns < wholeTurns) wholeTurns -= 1.0f;
    float wrapped = newPhase - wholeTurns * 6.2831854820251465f;
    self->PulseAng = wrapped;
    float blend = fCos(wrapped) * half + half;
    float invBlend = 1.0f - blend;
    self->CurColor.A = blend * self->MinColor.A + invBlend * self->MaxColor.A;
    self->CurColor.R = blend * self->MinColor.R + invBlend * self->MaxColor.R;
    self->CurColor.G = blend * self->MinColor.G + invBlend * self->MaxColor.G;
    self->CurColor.B = blend * self->MinColor.B + invBlend * self->MaxColor.B;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cPlayerIndicatorFX_render);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

struct sVtx_3AF8 {
    float s, t, q, pad;
    int r, g, b, a;
    t4Vector pos;
    sVtx_3AF8() {}
};

struct sWkColor_3AF8 {
    float a, r, g, b;
};


extern sRsState_2868 sDrawState_s_defaultDrawState_3AF8 __asm__("sDrawState_s_defaultDrawState");

// Render context (GrMan): state stack at 0xE84, vtable at 0x10D8.
struct sRCtx_3AF8 {
    char pad[0xE84];
    sRsState_2868* top;         // 0xE84
    char padE88[0xFFC - 0xE88];
    int tex;                    // 0xFFC
    char pad1000[0x10D8 - 0x1000];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual float* GetView();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void RenderTriStrip(int n, sVtx_3AF8* v, int a);
};

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector Scale_3AF8(const t4Vector& v, float s)
{
    t4Vector r;
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
static inline t4Vector Add_3AF8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline t4Vector Sub_3AF8(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

static inline void Set4_3AF8(t4Vector& v, float x, float y, float z, float w)
{
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
}

static inline void rsSetA_3AF8(sRCtx_3AF8* ctx, int mask, int shift, int v)
{
    ctx->top->flagsA = (ctx->top->flagsA & ~mask) | ((v << shift) & mask);
}

static inline void rsSetB_3AF8(sRCtx_3AF8* ctx, int mask, int shift, int v)
{
    ctx->top->flagsB = (ctx->top->flagsB & ~mask) | ((v << shift) & mask);
}

static inline int rsTex_3AF8(sRCtx_3AF8* ctx)
{
    return ctx->tex;
}

static inline int IsHuman_3AF8(cRider* rider)
{
    char* obj = (char*)&rider->mVtbl;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)obj;
    return vt[kRiderVt_isHuman].fn(vthis(obj, vt[kRiderVt_isHuman]));
}

static inline int ShouldDraw_3AF8(cPlayerIndicatorFX* self)
{
    int vis = 0;
    if (SSXApp->Game->AI->NumberOfPlayersAllocated >= 2) {
        if (IsHuman_3AF8(self->mRider)) {
            cGameViewMan* views = SSXApp->Game->ViewMan;
            if (views->m_uiNumView >= 2) {
                vis = (int)views->m_uiCurrentView != self->mRider->mViewIndex;
            }
        }
        cBE_getInterface_Fv(SSXApp->BE, 7);
        if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
            vis = self->mRider->mIsLocal == 0;
        }
    } else if (!IsHuman_3AF8(self->mRider)) {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        if ((unsigned int)(cBENewRaceInterface_newRace.cGameEvent - 4) < 2) {
            int mode = SSXApp->Game->SectionMan->mBELocationID;
            if ((unsigned int)(mode - 14) < 3) vis = 1;
        }
    }
    if (self->mRider->mIsInNIS) {
        vis = 0;
    }
    return vis;
}

/**
 * Draws a player indicator: a tall beam of light (about 20000 units) rising from the rider's head.
 *
 * It faces the camera, is 170 units wide and fades out at both ends. It is drawn in split screen for the
 * other player's rider seen from the other view, for remote riders in network games, and for AI riders in
 * some event modes. It is never drawn during an in-game cutscene (mIsInNIS).
 */
extern "C" void cPlayerIndicatorFX_render(cPlayerIndicatorFX* self)
{
    if (!ShouldDraw_3AF8(self)) {
        return;
    }
    {
        sRCtx_3AF8* renderCtx = (sRCtx_3AF8*)GrMan_boardwakefx;
        renderCtx->top[1] = renderCtx->top[0];
        renderCtx->top++;
    }
    *((sRCtx_3AF8*)GrMan_boardwakefx)->top = sDrawState_s_defaultDrawState_3AF8;
    {
        sRCtx_3AF8* renderCtx = (sRCtx_3AF8*)GrMan_boardwakefx;
        rsSetA_3AF8(renderCtx, 0x400000, 22, 1);
        rsSetA_3AF8(renderCtx, 0x7C, 2, 7);
        rsSetA_3AF8(renderCtx, 0x3, 0, 2);
        rsSetB_3AF8(renderCtx, 0x1FFFFC00, 10, 0);
        rsSetB_3AF8(renderCtx, 0x3E0, 5, 7);
        *(short*)((char*)renderCtx->top + 0x10) = rsTex_3AF8(renderCtx);
    }
    float* viewMat = ((sRCtx_3AF8*)GrMan_boardwakefx)->GetView();
    t4Vector side;
    Set4_3AF8(side, viewMat[0], viewMat[4], viewMat[8], 0.0f);
    t4Vector up;
    Set4_3AF8(up, viewMat[1], viewMat[5], viewMat[9], 0.0f);
    t4Vector pos = *(t4Vector*)((char*)self->mRider->mAnimModel->SkelMatrices + (self->mRider->mBoneHead << 6) + 0x30);
    const t4Vector* base = &pos;
    t4Vector beamTip = Add_3AF8(*base, Scale_3AF8(up, 20260.0f));
    t4Vector beamUpper = Add_3AF8(*base, Scale_3AF8(up, 20130.0f));
    t4Vector beamLower = Add_3AF8(*base, Scale_3AF8(up, 130.0f));
    t4Vector off = Scale_3AF8(side, 85.0f);
    sWkColor_3AF8 fade;
    fade.a = 0.0f;
    fade.r = (*(sWkColor_3AF8*)&self->CurColor).r;
    fade.g = (*(sWkColor_3AF8*)&self->CurColor).g;
    fade.b = (*(sWkColor_3AF8*)&self->CurColor).b;
    sVtx_3AF8 verts[8];
    {
        t4Vector vertPos = Sub_3AF8(*base, off);
        verts[0].r = (int)(fade.r * 255.0f);
        verts[0].g = (int)(fade.g * 255.0f);
        verts[0].b = (int)(fade.b * 255.0f);
        verts[0].a = (int)(fade.a * 128.0f);
        verts[0].q = 1.0f;
        verts[0].pos = vertPos;
        verts[0].s = 0.0f;
        verts[0].t = 1.0f;
    }
    {
        t4Vector vertPos = Add_3AF8(*base, off);
        verts[1].r = (int)(fade.r * 255.0f);
        verts[1].g = (int)(fade.g * 255.0f);
        verts[1].b = (int)(fade.b * 255.0f);
        verts[1].a = (int)(fade.a * 128.0f);
        verts[1].q = 1.0f;
        verts[1].pos = vertPos;
        verts[1].s = 1.0f;
        verts[1].t = 1.0f;
    }
    {
        t4Vector vertPos = Sub_3AF8(beamLower, off);
        verts[2].r = (int)((*(sWkColor_3AF8*)&self->CurColor).r * 255.0f);
        verts[2].g = (int)((*(sWkColor_3AF8*)&self->CurColor).g * 255.0f);
        verts[2].b = (int)((*(sWkColor_3AF8*)&self->CurColor).b * 255.0f);
        verts[2].a = (int)((*(sWkColor_3AF8*)&self->CurColor).a * 128.0f);
        verts[2].q = 1.0f;
        verts[2].pos = vertPos;
        verts[2].s = 0.0f;
        verts[2].t = 0.0f;
    }
    {
        t4Vector vertPos = Add_3AF8(beamLower, off);
        verts[3].r = (int)((*(sWkColor_3AF8*)&self->CurColor).r * 255.0f);
        verts[3].g = (int)((*(sWkColor_3AF8*)&self->CurColor).g * 255.0f);
        verts[3].b = (int)((*(sWkColor_3AF8*)&self->CurColor).b * 255.0f);
        verts[3].a = (int)((*(sWkColor_3AF8*)&self->CurColor).a * 128.0f);
        verts[3].q = 1.0f;
        verts[3].pos = vertPos;
        verts[3].s = 1.0f;
        verts[3].t = 0.0f;
    }
    {
        t4Vector vertPos = Sub_3AF8(beamUpper, off);
        verts[4].r = (int)((*(sWkColor_3AF8*)&self->CurColor).r * 255.0f);
        verts[4].g = (int)((*(sWkColor_3AF8*)&self->CurColor).g * 255.0f);
        verts[4].b = (int)((*(sWkColor_3AF8*)&self->CurColor).b * 255.0f);
        verts[4].a = (int)((*(sWkColor_3AF8*)&self->CurColor).a * 128.0f);
        verts[4].q = 1.0f;
        verts[4].pos = vertPos;
        verts[4].s = 0.0f;
        verts[4].t = 1.0f;
    }
    {
        t4Vector vertPos = Add_3AF8(beamUpper, off);
        verts[5].r = (int)((*(sWkColor_3AF8*)&self->CurColor).r * 255.0f);
        verts[5].g = (int)((*(sWkColor_3AF8*)&self->CurColor).g * 255.0f);
        verts[5].b = (int)((*(sWkColor_3AF8*)&self->CurColor).b * 255.0f);
        verts[5].a = (int)((*(sWkColor_3AF8*)&self->CurColor).a * 128.0f);
        verts[5].q = 1.0f;
        verts[5].pos = vertPos;
        verts[5].s = 1.0f;
        verts[5].t = 1.0f;
    }
    {
        t4Vector vertPos = Sub_3AF8(beamTip, off);
        verts[6].r = (int)(fade.r * 255.0f);
        verts[6].g = (int)(fade.g * 255.0f);
        verts[6].b = (int)(fade.b * 255.0f);
        verts[6].a = (int)(fade.a * 128.0f);
        verts[6].q = 1.0f;
        verts[6].pos = vertPos;
        verts[6].s = 0.0f;
        verts[6].t = 0.0f;
    }
    {
        t4Vector vertPos = Add_3AF8(beamTip, off);
        verts[7].r = (int)(fade.r * 255.0f);
        verts[7].g = (int)(fade.g * 255.0f);
        verts[7].b = (int)(fade.b * 255.0f);
        verts[7].a = (int)(fade.a * 128.0f);
        verts[7].q = 1.0f;
        verts[7].pos = vertPos;
        verts[7].s = 1.0f;
        verts[7].t = 0.0f;
    }
    ((sRCtx_3AF8*)GrMan_boardwakefx)->RenderTriStrip(8, verts, 0);
    *(char**)(GrMan_boardwakefx + 0xE84) -= 0x14;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_cTransitionMan);
#ifdef SKIP_ASM
extern "C" void* cObjNode_cObjNode1(void* self, void* a1);
extern "C" void* cScriptObjTransition_cScriptObjTransition(void* self);
extern "C" void cScriptObject_InitScriptObject(void* self);
extern "C" void cTransitionObj_InitializeBuffers(void);
extern void* cTransitionMan_cScriptObjTransition_vtbl[];
extern void* cTransitionMan_vtbl[];

/**
 * Constructs the screen transition manager for one view.
 *
 * Runs cObjNode_cObjNode1 with the bucket and the script-object constructor (cScriptObjTransition_cScriptObjTransition) on +0x10, stores the view
 * index at +0x74, zeroes the transition fields, sets the vtables (cTransitionMan_cScriptObjTransition_vtbl at +0x1C, cTransitionMan_vtbl at +0xC) and
 * the screen-grab handle at +0x6C to -1, then calls cTransitionObj_InitializeBuffers and cScriptObject_InitScriptObject on the script object.
 *
 * @param bucket  object bucket for the node (On Tour bucket)
 * @param viewIndex  the view it serves (On Tour viewIndex)
 * @return this
 */
extern "C" void* cTransitionMan_cTransitionMan(cTransitionMan* self, void* bucket, int viewIndex)
{
    cObjNode_cObjNode1(self, bucket);
    cScriptObjTransition_cScriptObjTransition((char*)&self->mObjType);
    self->mViewIndex = viewIndex;
    self->mNumPause = 0;
    self->mbPlayingTransition = 0;
    self->mTransitionIn = 0;
    self->mTransitionOut = 0;
    self->mUsingScreenGrab = 0;
    self->mRequestingScreenGrab = 0;
    *(int*)&self->mScreenGrabHandle = 0;
    self->mExternalTimePercentage = 0;
    self->mCurrentInitiator = 0;
    self->mRequestingDelete = 0;
    self->mRequestDeleteFrame = 0;
    self->mScriptVtbl = cTransitionMan_cScriptObjTransition_vtbl;
    self->mVtbl = cTransitionMan_vtbl;
    self->mScreenGrabHandle.mTextureId = -1;
    cTransitionObj_InitializeBuffers();
    cScriptObject_InitScriptObject((char*)&self->mObjType);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan__cTransitionMan);
#ifdef SKIP_ASM
struct sBWVEntry1 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern char* cGraphicsMan_TheGraphicsMan_boardwakefx __asm__("cGraphicsMan_TheGraphicsMan");
extern "C" void cTransitionMan_EndTransition(cTransitionMan* self);
extern "C" void cScriptObject_DeInitScriptObject(void* self);
extern "C" void cScriptObject__cScriptObject(void* self, int flags);
extern "C" void cObjNode__cObjNode(void* self, int flags);

/**
 * Destroys a transition manager: ends the transition and releases its held texture.
 *
 * The texture at +0x68 goes back through graphics manager slot 60. It then shuts down and destroys the script
 * object part (+0x10) and runs the cObjNode destructor.
 *
 * @param self   the transition manager
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cTransitionMan__cTransitionMan(cTransitionMan* self, int flags)
{
    self->mScriptVtbl = cTransitionMan_cScriptObjTransition_vtbl;
    self->mVtbl = cTransitionMan_vtbl;
    char* scriptObj = (char*)&self->mObjType;
    cTransitionMan_EndTransition(self);
    if (self->mRequestingDelete != 0) {
        cPS2GraphicsMan* obj = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_boardwakefx;
        sBWVEntry1* removeScreenGrab = &((sBWVEntry1*)VPTR_AT(obj, 0x10D8))[60];
        removeScreenGrab->fn(vthis(obj, *removeScreenGrab), (char*)&self->mScreenGrabHandle);
        self->mRequestingDelete = 0;
    }
    cScriptObject_DeInitScriptObject(scriptObj);
    cScriptObject__cScriptObject(scriptObj, 0);
    cObjNode__cObjNode(self, flags);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_StartTransition);
#ifdef SKIP_ASM


extern char* cGraphicsMan_TheGraphicsMan_boardwakefx __asm__("cGraphicsMan_TheGraphicsMan");
extern "C" void cTransitionMan_EndTransition(cTransitionMan* self);

// PORT: later callers declare this as void (and an int alias); the body returns int.
extern "C" int cTransitionMan_StartTransition_def(void* self, int a1, void* o1, void* o2, float f0, float f1, float f2, int a4, int a5) __asm__("cTransitionMan_StartTransition");

/**
 * Starts a screen transition made of an in part, a hold and an out part.
 *
 * If a transition is active (+0x44) it is ended first (cTransitionMan_EndTransition). The initiator goes to +0x78 and the external
 * time pointer to +0x70. With a screen grab, a 0x100 x 0x100 grab texture is requested through the graphics manager's
 * virtual slot 59 if the handle at +0x6C is unset; if it stays unset, both transition objects get their virtual slot 1
 * with 3 and the call fails. Otherwise the objects (+0x4C, +0x50) and times (+0x54..+0x5C) are stored and the
 * transition is marked active from time 0.
 *
 * @param initiator  who started it (On Tour initiator)
 * @param transitionIn  transition object for the in part, or 0 (On Tour transitionIn)
 * @param transitionOut  transition object for the out part, or 0 (On Tour transitionOut)
 * @param inTime  length of the in part (On Tour inTime)
 * @param holdTime  length of the hold (On Tour holdTime)
 * @param outTime  length of the out part (On Tour outTime)
 * @param useScreenGrab  nonzero to grab the screen as the transition's texture (On Tour useScreenGrab)
 * @param externalTimePercentage  optional float that drives the progress from outside (On Tour externalTimePercentage)
 * @return 1 when the transition started, 0 when the screen grab could not be set up
 */
extern "C" int cTransitionMan_StartTransition_def(void* vself, int initiator, void* transitionIn, void* transitionOut, float inTime, float holdTime, float outTime, int useScreenGrab, int externalTimePercentage)
{
    cTransitionMan* self = (cTransitionMan*)vself;
    if (self->mbPlayingTransition != 0) {
        cTransitionMan_EndTransition((cTransitionMan*)self);
    }
    self->mCurrentInitiator = initiator;
    self->mExternalTimePercentage = (float*)externalTimePercentage;
    if (useScreenGrab != 0) {
        if (self->mRequestingDelete != 0) {
            self->mRequestingDelete = 0;
        }
        char* ref = (char*)&self->mScreenGrabHandle;
        if (~self->mScreenGrabHandle.mTextureId == 0) {
            cPS2GraphicsMan* obj = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_boardwakefx;
            sVEntry<void(void*, void*, int, int)>* vtblEntry = &((sVEntry<void(void*, void*, int, int)>*)VPTR_AT(obj, 0x10D8))[59];
            vtblEntry->fn(vthis(obj, *vtblEntry), ref, 0x100, 0x100);
            if (~self->mScreenGrabHandle.mTextureId == 0) {
                if (transitionIn != 0) {
                    sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)transitionIn;
                    vt[1].fn(vthis(transitionIn, vt[1]), 3);
                }
                if (transitionOut != 0) {
                    sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)transitionOut;
                    vt[1].fn(vthis(transitionOut, vt[1]), 3);
                }
                return 0;
            }
        }
    }
    self->mTransitionIn = (cTransitionObj*)transitionIn;
    self->mTransitionOut = (cTransitionObj*)transitionOut;
    self->mfInTime = inTime;
    self->mfHoldTime = holdTime;
    self->mfOutTime = outTime;
    self->mbPlayingTransition = 1;
    *(int*)&self->mfCurrentTime = 0;
    self->mUsingScreenGrab = useScreenGrab;
    if (useScreenGrab != 0) {
        self->mRequestingScreenGrab = 1;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_StartTransitionIn);
#ifdef SKIP_ASM
extern "C" void cTransitionMan_StartTransition(void* self, int a1, int a2, int a3, float f0, float f1, int a4, int a5, float f2);

/**
 * Starts a transition with only an in-object: cTransitionMan_StartTransition with no out-object, no out time and no
 * screen grab.
 *
 * Nothing in the retail code references it.
 *
 * @param initiator  the initiator
 * @param transitionIn  the transition object to play in
 * @param inTime  the in time
 * @param holdTime  the hold time
 */
extern "C" void cTransitionMan_StartTransitionIn(cTransitionMan* self, int initiator, int transitionIn, float inTime, float holdTime)
{
    cTransitionMan_StartTransition(self, initiator, transitionIn, 0, inTime, holdTime, 0, 0, 0.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_StartTransitionOut);
#ifdef SKIP_ASM
// PORT: the unit declares cTransitionMan_StartTransition as void; this caller returns its int result.
extern "C" int cTransitionMan_StartTransition_i(void* self, int a1, int a2, int a3, float f0, float f1, int a4, int a5, float f2) __asm__("cTransitionMan_StartTransition");

/**
 * Starts the out part of a screen transition, or adds it to the transition already running.
 *
 * When no transition is active it calls cTransitionMan_StartTransition with only an out object and @p outTime as the out time. Otherwise it
 * stores @p transition as the out object (+0x50) and @p outTime as the out time (+0x5C) and returns 1.
 *
 * @param initiator  who started it (On Tour initiator)
 * @param transition  transition object for the out part (On Tour transition)
 * @param outTime  length of the out part (On Tour outTime)
 * @return cTransitionMan_StartTransition's result, or 1 when added to a running transition
 */
extern "C" int cTransitionMan_StartTransitionOut(cTransitionMan* self, int initiator, int transition, float outTime)
{
    if (self->mbPlayingTransition == 0) {
        return cTransitionMan_StartTransition_i(self, initiator, 0, transition, 0.0f, 0.0f, 0, 0, outTime);
    }
    self->mTransitionOut = (cTransitionObj*)transition;
    self->mfOutTime = outTime;
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_StartTransitionScreenGrab);
#ifdef SKIP_ASM
extern "C" void cTransitionMan_StartTransition(void* self, int a1, int a2, int a3, float f0, float f1, int a4, int a5, float f2);

/**
 * Starts a transition that plays out over a grab of the current screen.
 *
 * Calls cTransitionMan_StartTransition with no in object, @p transition as the out object, zero in and hold times, @p transitionTime as the out time, the
 * screen grab on and @p externalTimePercentage as the external time pointer.
 *
 * @param initiator  who started it (On Tour initiator)
 * @param transition  transition object to play (On Tour transition)
 * @param externalTimePercentage  optional float that drives the progress from outside (On Tour externalTimePercentage)
 * @param transitionTime  length of the transition (On Tour transitionTime)
 */
extern "C" void cTransitionMan_StartTransitionScreenGrab(cTransitionMan* self, int initiator, int transition, int externalTimePercentage, float transitionTime)
{
    cTransitionMan_StartTransition(self, initiator, 0, transition, 0.0f, 0.0f, 1, externalTimePercentage, transitionTime);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_EndTransition);
#ifdef SKIP_ASM


extern char* cGraphicsMan_TheGraphicsMan_boardwakefx __asm__("cGraphicsMan_TheGraphicsMan");

/**
 * Ends the current transition: deletes its in and out objects and resets the transition state.
 *
 * Does nothing unless a transition is active (+0x44). Clears +0x44 and the pause count (+0x40), deletes the in (+0x4C)
 * and out (+0x50) objects through their deleting destructor (virtual slot 1, flags 3) and clears both pointers and
 * +0x70. When a screen-grab handle is held (+0x6C not -1) it sets +0x7C, stores GrMan virtual slot 114's result at
 * +0x80 and clears +0x60 and +0x64. Finally clears +0x78. Script commands, the rider and AI code, cTransitionMan_update
 * and cTransitionMan_OnEndObjectUse call it.
 */
extern "C" void cTransitionMan_EndTransition(cTransitionMan* self)
{
    if (self->mbPlayingTransition != 0) {
        self->mbPlayingTransition = 0;
        self->mNumPause = 0;
        char* object = (char*)self->mTransitionIn;
        if (object != 0) {
            sVEntry<void(void*, int)>* entry = &(*(sVEntry<void(void*, int)>**)object)[1];
            entry->fn(vthis(object, *entry), 3);
        }
        self->mTransitionIn = 0;
        object = (char*)self->mTransitionOut;
        if (object != 0) {
            sVEntry<void(void*, int)>* entry = &(*(sVEntry<void(void*, int)>**)object)[1];
            entry->fn(vthis(object, *entry), 3);
        }
        self->mTransitionOut = 0;
        self->mExternalTimePercentage = 0;
        if (~self->mScreenGrabHandle.mTextureId != 0) {
            cPS2GraphicsMan* obj = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_boardwakefx;
            self->mRequestingDelete = 1;
            sVEntry<int(void*)>* entry = &((sVEntry<int (void*)>*)VPTR_AT(obj, 0x10D8))[114];
            self->mRequestDeleteFrame = entry->fn(vthis(obj, *entry));
            self->mRequestingScreenGrab = 0;
            self->mUsingScreenGrab = 0;
        }
        self->mCurrentInitiator = 0;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_PauseTransition);
#ifdef SKIP_ASM
/**
 * Counts a pause or resume request for the running transition.
 *
 * With no transition active (+0x44) it clears the pause count (+0x40). Otherwise @p up increments it and a zero @p up
 * decrements it when positive. tNISList_DoPause calls it on the transition manager.
 *
 * @param up  nonzero to pause, zero to resume
 */
extern "C" void cTransitionMan_PauseTransition(cTransitionMan* self, int up)
{
    if (self->mbPlayingTransition == 0) {
        self->mNumPause = 0;
        return;
    }
    if (up) {
        self->mNumPause += 1;
        return;
    }
    if (self->mNumPause != 0) {
        self->mNumPause -= 1;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_GetTransitionTimeLeft);
#ifdef SKIP_ASM
/**
 * Returns the time left in the running transition: in + hold + out time (+0x54, +0x58, +0x5C) minus the elapsed time
 * (+0x48), not below 0.
 *
 * Returns 0 with no transition active (+0x44). tNISList_PlayNextScript uses it as the cut-scene fade time.
 *
 * @return the remaining time in seconds
 */
extern "C" float cTransitionMan_GetTransitionTimeLeft(cTransitionMan* self)
{
    if (self->mbPlayingTransition == 0) {
        return 0.0f;
    }
    float remaining = self->mfInTime + self->mfHoldTime
            + self->mfOutTime - self->mfCurrentTime;
    if (remaining < 0.0f) {
        return 0.0f;
    }
    return remaining;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_update);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan_boardwakefx __asm__("cGraphicsMan_TheGraphicsMan");


/**
 * Advances a screen transition's clock and ends it once the fade in, hold and fade out are over.
 *
 * A held texture is released once graphics slot 114 (probably the frame count) is 3 past +0x80. While active
 * (+0x44) and not paused (+0x40), the time (+0x48) is set from +0x3C when +0x18 is positive, from a tracked
 * value (+0x70) times the total length, or else advanced by one frame (1 / frame rate).
 */
extern "C" void cTransitionMan_update(cTransitionMan* self)
{
    cPS2GraphicsMan* ctx = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_boardwakefx;
    if (self->mRequestingDelete != 0) {
        sVEntry<int(void*)>* vt = (sVEntry<int (void*)>*)VPTR_AT(ctx, 0x10D8);
        if (vt[114].fn(vthis(ctx, vt[114])) - self->mRequestDeleteFrame >= 3) {
            sVEntry<void(void*, void*)>* vt2 = (sVEntry<void (void*, void*)>*)VPTR_AT(ctx, 0x10D8);
            vt2[60].fn(vthis(ctx, vt2[60]), (char*)&self->mScreenGrabHandle);
            self->mRequestingDelete = 0;
        }
    }
    if (self->mbPlayingTransition == 0) return;
    if (self->mNumPause != 0) return;
    if (self->mInUse > 0) {
        self->mfCurrentTime = self->mInfo.currTime;
    } else if (self->mExternalTimePercentage != 0) {
        self->mfCurrentTime = *self->mExternalTimePercentage * (self->mfInTime + self->mfHoldTime + self->mfOutTime);
    } else {
        self->mfCurrentTime += 1.0f / (float)cAppMan_TheAppMan->TimerHz;
    }
    if (self->mfInTime + self->mfHoldTime + self->mfOutTime <= self->mfCurrentTime) {
        cTransitionMan_EndTransition(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_render);
#ifdef SKIP_ASM
extern char* cGraphicsMan_TheGraphicsMan_boardwakefx __asm__("cGraphicsMan_TheGraphicsMan");
extern "C" void FigureOutTransitionUVs(void);
// PORT: cWorldPainterMan_reset is defined with an unused self param; this caller passes none.
void cWorldPainterMan_reset_47E8() __asm__("cWorldPainterMan_reset__FPv");




struct sRsVtV47E8 {
    short delta;
    short index;
    void (*fn)(void*, int, int, float, float, float, float, float, float);
};


struct sFadeVt47E8 {
    short delta;
    short index;
    void (*fn)(void*, float);
};

struct sFade47E8 {
    char pad0[0x44];
    int active;                 // 0x44
    float time;                 // 0x48
    char* o1;                   // 0x4C
    char* o2;                   // 0x50
    float fadeIn;               // 0x54
    float hold;                 // 0x58
    float fadeOut;              // 0x5C
    int useTex;                 // 0x60
    int pending;                // 0x64
    int ref;                    // 0x68
    int tex;                    // 0x6C
    int i70;                    // 0x70
    int rider;                  // 0x74
};

static inline void rsSetA47E8(cPS2GraphicsMan* ctx, int mask, int shift, int v)
{
    (ctx->m_pCurrentDrawState)->m_bitGroups[1] = (((int)(ctx->m_pCurrentDrawState)->m_bitGroups[1]) & ~mask) | ((v << shift) & mask);
}

static inline void rsSetB47E8(cPS2GraphicsMan* ctx, int mask, int shift, int v)
{
    (ctx->m_pCurrentDrawState)->m_bitGroups[2] = (((int)(ctx->m_pCurrentDrawState)->m_bitGroups[2]) & ~mask) | ((v << shift) & mask);
}

static inline void rsSet047E8(cPS2GraphicsMan* ctx, int mask, int shift, int v)
{
    (ctx->m_pCurrentDrawState)->m_bitGroups[0] = (((int)(ctx->m_pCurrentDrawState)->m_bitGroups[0]) & ~mask) | ((v << shift) & mask);
}

static inline int fadeTex47E8(sFade47E8* self)
{
    return self->tex;
}

/**
 * Draws a full-screen transition for its rider's view: the first overlay fading in, then the second fading
 * out.
 *
 * The overlay object's virtual slot 2 draws it with the alpha, over a 640 x 480 screen. The pending texture
 * is released when the hold ends. At an alpha of 0.93 or more it also resets the world painter
 * (cWorldPainterMan_reset).
 */
extern "C" void cTransitionMan_render(cTransitionMan* self)
{
    if (self->mViewIndex != (int)SSXApp->Game->ViewMan->m_uiCurrentView) {
        return;
    }
    FigureOutTransitionUVs();
    cPS2GraphicsMan* ctx = (cPS2GraphicsMan*)cGraphicsMan_TheGraphicsMan_boardwakefx;
    if (self->mbPlayingTransition == 0) {
        return;
    }
    float curTime = self->mfCurrentTime;
    if (self->mRequestingScreenGrab != 0 && self->mfInTime + self->mfHoldTime <= curTime) {
        sVEntry<void(void*, void*)>* vt = (sVEntry<void(void*, void*)>*)((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8));
        vt[61].fn(vthis(ctx, vt[61]), (int*)&self->mScreenGrabHandle);
        self->mRequestingScreenGrab = 0;
    }
    char* transition;
    float alpha;
    if ((char*)self->mTransitionIn == 0) {
        alpha = 1.0f - (curTime - self->mfHoldTime) / self->mfOutTime;
        transition = (char*)self->mTransitionOut;
    } else if (curTime < self->mfInTime || (char*)self->mTransitionOut == 0) {
        alpha = curTime / self->mfInTime;
        transition = (char*)self->mTransitionIn;
    } else if (curTime < self->mfInTime + self->mfHoldTime) {
        alpha = 1.0f;
        transition = (char*)self->mTransitionIn;
    } else {
        alpha = 1.0f - (curTime - (self->mfInTime + self->mfHoldTime)) / self->mfOutTime;
        transition = (char*)self->mTransitionOut;
    }
    float clamped = wakeClampE058(alpha, 0.0f, 1.0f);
    if (clamped >= 0.93f) {
        cWorldPainterMan_reset_47E8();
    }
    (ctx->m_pCurrentDrawState)[1] = (ctx->m_pCurrentDrawState)[0];
    (*(sDrawState**)&ctx->m_pCurrentDrawState)++;
    ((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8))[21].fn(vthis(ctx, ((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8))[21]));
    ((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8))[31].fn(vthis(ctx, ((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8))[31]));
    ((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8))[37].fn(vthis(ctx, ((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8))[37]));
    sRsVtV47E8* vv = (sRsVtV47E8*)((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8));
    vv[26].fn(vthis(ctx, vv[26]), 0, 0, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    rsSetB47E8(ctx, 0x3E0, 5, 8);
    rsSetA47E8(ctx, 0x400000, 22, 1);
    rsSetA47E8(ctx, 0x1800000, 23, 2);
    rsSetA47E8(ctx, 0x300000, 20, 0);
    rsSetA47E8(ctx, 0xFF000, 12, 20);
    rsSetA47E8(ctx, 0x7C, 2, 5);
    rsSet047E8(ctx, 0xC, 2, 3);
    rsSetA47E8(ctx, 0x3, 0, 2);
    rsSetB47E8(ctx, 0x1FFFFC00, 10, 0);
    if (self->mUsingScreenGrab != 0) {
        (ctx->m_pCurrentDrawState)->m_textureState.m_texid[0] = fadeTex47E8(((sFade47E8*)self));
    } else {
        (ctx->m_pCurrentDrawState)->m_textureState.m_texid[0] = -1;
    }
    if (self->mUsingScreenGrab == 0 || self->mRequestingScreenGrab == 0) {
        sFadeVt47E8* ov = *(sFadeVt47E8**)transition;
        ov[2].fn(vthis(transition, ov[2]), clamped);
    }
    ((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8))[32].fn(vthis(ctx, ((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8))[32]));
    ((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8))[22].fn(vthis(ctx, ((sVEntry<void(void*)>*)VPTR_AT(ctx, 0x10D8))[22]));
    (*(sDrawState**)&ctx->m_pCurrentDrawState)--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_OnStartObjectUse);
#ifdef SKIP_ASM

void cScriptObject_OnStartObjectUse(void* self);
extern "C" int cSSXScriptEngine_MakeTransitionObject(void* tbl, int mode, int id);
extern "C" void cTransitionMan_StartTransition(void* self, int a1, int a2, int a3, float f0, float f1, int a4, int a5, float f2);
extern "C" void cTransitionMan_StartTransitionScreenGrab(cTransitionMan* self, int a1, int a3, int a5, float f2);

/**
 * Starts the transition a script object describes when a player starts using it.
 *
 * Calls the cScriptObject hook cScriptObject_OnStartObjectUse__FPv on the script object at +0x10. For a transition type (+0x28) other than 7
 * it makes an in and an out object with cSSXScriptEngine_MakeTransitionObject and starts a transition (initiator 2)
 * with in, hold and out times from +0x34, +0x30 and +0x2C. Type 7 starts a screen-grab transition with one object and
 * the time at +0x34. The level at +0x3C is then copied to +0x48.
 */
extern "C" void cTransitionMan_OnStartObjectUse(cTransitionMan* self)
{
    tScriptObjTransitionInfo* transitionInfo = (&self->mInfo);
    cScriptObject_OnStartObjectUse((char*)&self->mObjType);
    if (self->mInfo.transition != 7) {
        int transitionIn = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, self->mInfo.transition, self->mInfo.transitionColour);
        int transitionOut = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, self->mInfo.transition, self->mInfo.transitionColour);
        cTransitionMan_StartTransition(self, 2, transitionIn, transitionOut, self->mInfo.outTime, self->mInfo.holdTime, 0, 0, self->mInfo.inTime);
    } else {
        cTransitionMan_StartTransitionScreenGrab(self, 2, cSSXScriptEngine_MakeTransitionObject(ScriptEngine, 7, self->mInfo.transitionColour), 0, self->mInfo.outTime);
    }
    self->mfCurrentTime = transitionInfo->currTime;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_OnEndObjectUse);
#ifdef SKIP_ASM
void cScriptObject_OnEndObjectUse(void*);
extern "C" void cTransitionMan_EndTransition(cTransitionMan* self);

/**
 * Ends the transition when a player stops using the transition script object.
 *
 * Runs cScriptObject_OnEndObjectUse on the script object at +0x10, then cTransitionMan_EndTransition. It sits in the
 * cScriptObjTransition part's vtable (cTransitionMan_cScriptObjTransition_vtbl) right after cTransitionMan_OnStartObjectUse.
 */
extern "C" void cTransitionMan_OnEndObjectUse(cTransitionMan* self)
{
    cScriptObject_OnEndObjectUse((char*)&self->mObjType);
    cTransitionMan_EndTransition(self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_IsTransitionPending);
#ifdef SKIP_ASM
/**
 * Returns whether both screen-grab flags (+0x60 and +0x64) are set.
 *
 * It fills a slot that the base cScriptObjTransition vtable leaves pure (__pure_virtual), in the transition manager's
 * vtable cTransitionMan_cScriptObjTransition_vtbl.
 *
 * @return 1 when both flags are set, otherwise 0
 */
extern "C" int cTransitionMan_IsTransitionPending(cTransitionMan* self)
{
    int result = 0;
    if (self->mUsingScreenGrab != 0) {
        result = self->mRequestingScreenGrab != 0;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionObj_InitializeBuffers);
#ifdef SKIP_ASM
extern int cTransitionObj_mbBufferUsed[];

/**
 * Marks every transition object buffer as free.
 *
 * Clears the four in-use flags at cTransitionObj_mbBufferUsed.
 */
extern "C" void cTransitionObj_InitializeBuffers(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        cTransitionObj_mbBufferUsed[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionObj_operator_new);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sWakePoolEntry {
    char data[0x64];
};

extern int cTransitionObj_mbBufferUsed[];
// same object as the unit's later `extern sWakeSlot cTransitionObj_mBuffer[]` (0x64-byte slots)
extern sWakePoolEntry D_wakePool[] __asm__("cTransitionObj_mBuffer");
extern sWakePoolEntry* cTransitionObj_operator_new_no_warnings;

/**
 * Allocates a transition object from the fixed pool of four buffers.
 *
 * Returns the first buffer of cTransitionObj_mBuffer whose in-use flag in cTransitionObj_mbBufferUsed is clear, marking it used, or the pointer in
 * cTransitionObj_operator_new_no_warnings when all four are taken.
 *
 * @return the buffer for the new object
 */
extern "C" sWakePoolEntry* cTransitionObj_operator_new(void)
{
    for (int i = 0; i < 4; i++) {
        if (cTransitionObj_mbBufferUsed[i] == 0) {
            cTransitionObj_mbBufferUsed[i] = 1;
            return &D_wakePool[i];
        }
    }
    return cTransitionObj_operator_new_no_warnings;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionObj_operator_delete);
#ifdef SKIP_ASM
struct sWakeSlot {
    char pad[0x64];
};

extern sWakeSlot cTransitionObj_mBuffer[];
extern int cTransitionObj_mbBufferUsed[];

/**
 * Returns a transition object's buffer to the fixed pool.
 *
 * Ignores null. Otherwise finds which of the four cTransitionObj_mBuffer buffers @p memory is and clears its in-use flag; any other
 * pointer is ignored.
 *
 * @param memory  the object's memory (On Tour memory)
 */
extern "C" void cTransitionObj_operator_delete(void* memory)
{
    int i;
    if (memory == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (&cTransitionObj_mBuffer[i] == memory) {
            cTransitionObj_mbBufferUsed[i] = 0;
            return;
        }
    }
}
#endif

extern void* cTransitionObj_vtbl[];

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionObj_cTransitionObj__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a transition object: installs cTransitionObj_vtbl.
 *
 * The wipe and circle constructors (cWipe_TObj_cWipe_TObj, func_002EBD88, cCircle_TObj_cCircle_TObj), the fade object code and
 * cSSXScriptEngine_MakeTransitionObject run it as the base constructor.
 *
 * @return @p self
 */
void* cTransitionObj_cTransitionObj(void* self)
{
    *(int*)self = (int)(void*)cTransitionObj_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowBox_cSnowBox);
#ifdef SKIP_ASM

struct sMatD88 {
    t4Vector r[4];
};

extern char t4mat_Identity[];
int BXrand();

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void wakeCopyMatD88(sMatD88* dst, const sMatD88* src)
{
    __asm__ __volatile__(
        "lqc2       $vf1, 0x0(%1)\n"
        "lqc2       $vf2, 0x10(%1)\n"
        "lqc2       $vf3, 0x20(%1)\n"
        "lqc2       $vf4, 0x30(%1)\n"
        "sqc2       $vf1, 0x0(%0)\n"
        "sqc2       $vf2, 0x10(%0)\n"
        "sqc2       $vf3, 0x20(%0)\n"
        "sqc2       $vf4, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (scale matrix rows by the components of s).
static inline void wakeScaleMatD88(sMatD88* dst, const sMatD88* src, const t4Vector& s)
{
    __asm__ __volatile__(
        "lqc2       $vf1, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "lqc2       $vf5, 0x10(%1)\n"
        "lqc2       $vf6, 0x20(%1)\n"
        "lqc2       $vf7, 0x30(%1)\n"
        "vmulx.xyzw $vf8, $vf4, $vf1x\n"
        "vmuly.xyzw $vf9, $vf5, $vf1y\n"
        "vmulz.xyzw $vf10, $vf6, $vf1z\n"
        "vmulw.xyzw $vf11, $vf7, $vf1w\n"
        "sqc2       $vf8, 0x0(%0)\n"
        "sqc2       $vf9, 0x10(%0)\n"
        "sqc2       $vf10, 0x20(%0)\n"
        "sqc2       $vf11, 0x30(%0)\n"
        :
        : "r"(dst), "r"(src), "m"(s)
        : "memory");
}

/**
 * Constructs one snowfall box for a view.
 *
 * Zeroes the state fields, sets the wind scale (+0xA4..+0xAC) to 1 and the reset flag (+0xB0), stores the view at +0x0,
 * the type at +0x4 and the four floats at +0xC, +0x14, +0x18 and +0x1C, takes three BXrand values for +0x98..+0xA0, and
 * sets the matrix at +0x50 to the identity scaled by @p boxScale, with the position at +0x20 at the origin.
 *
 * @param viewNum  the view this box belongs to (On Tour viewNum)
 * @param snowBoxType  kind of snow box (On Tour eSnowBoxType snowBoxType)
 * @param boxScale  size of the box (On Tour boxScale)
 * @param flakeSize  flake size (On Tour flakeSize)
 * @param opacity  flake opacity (On Tour opacity)
 * @param gravity  fall speed (On Tour gravity)
 * @return this
 */
extern "C" void* cSnowBox_cSnowBox(cSnowBox* self, int viewNum, int snowBoxType, float boxScale, float flakeSize, float opacity, float gravity)
{
    self->mNumFlakes = 0;
    *(int*)&self->mWindScale = 0;
    *(int*)&self->mOffset = 0;
    *(int*)&self->mOffset.y = 0;
    *(int*)&self->mOffset.z = 0;
    self->mOffset.w = 1.0f;
    *(int*)&self->mWindNext = 0;
    *(int*)&self->mWindNext.y = 0;
    *(int*)&self->mWindNext.z = 0;
    *(int*)&self->mWindNext.w = 0;
    *(int*)&self->mWindLast = 0;
    *(int*)&self->mWindLast.y = 0;
    *(int*)&self->mWindLast.z = 0;
    *(int*)&self->mWindLast.w = 0;
    *(int*)&self->mCurrentTimeToWindChange = 0;
    *(int*)&self->mTotalTimeToWindChange = 0;
    self->mColourR = 1.0f;
    self->mColourG = 1.0f;
    self->mColourB = 1.0f;
    self->mViewNum = viewNum;
    self->mSnowBoxType = snowBoxType;
    self->mBoxScale = boxScale;
    self->mFlakeSize = flakeSize;
    self->mOpacity = opacity;
    self->mGravity = gravity;
    self->mResetWind = 1;
    self->mRandSeed = BXrand();
    self->mRandSeed2 = BXrand();
    self->mRandSeed3 = BXrand();
    wakeCopyMatD88((sMatD88*)&self->mBoxMatrix, (sMatD88*)t4mat_Identity);
    {
        sMatD88 scaledCopy;
        t4Vector scale;
        sMatD88 scaled;
        scale.x = self->mBoxScale;
        scale.y = self->mBoxScale;
        scale.z = self->mBoxScale;
        scale.w = 1.0f;
        wakeScaleMatD88(&scaled, (sMatD88*)&self->mBoxMatrix, scale);
        wakeCopyMatD88(&scaledCopy, &scaled);
        wakeCopyMatD88((sMatD88*)&self->mBoxMatrix, &scaledCopy);
    }
    {
        t4Vector origin;
        origin.x = 0.0f;
        origin.y = 0.0f;
        origin.z = 0.0f;
        origin.w = 1.0f;
        *(&self->mOffset) = origin;
    }
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowBox_resetWind__FPv);
#ifdef SKIP_ASM
/**
 * Resets a snow box's wind so it is recomputed.
 *
 * Zeroes +0x90 and +0x94 and sets +0xB0 to 1.
 */
void cSnowBox_resetWind(void* vself)
{
    cSnowBox* self = (cSnowBox*)vself;
    *(int*)&self->mTotalTimeToWindChange = 0;
    self->mResetWind = 1;
    *(int*)&self->mCurrentTimeToWindChange = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowBox__cSnowBox);
#ifdef SKIP_ASM
void operator_delete(int* p);

/**
 * Destroys a snow box.
 *
 * Has nothing to release; frees the object with operator delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ in-charge flags; bit 0 deletes the object
 */
extern "C" void cSnowBox__cSnowBox(int* self, int flags)
{
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowBox_update);
#ifdef SKIP_ASM

// tWorldPainterSnowfallParameters (filled by cSnowFall_update).
struct tWorldPainterSnowfallParameters_4F50 {
    float f[8];                     // 0x00
    float c0[4];                    // 0x20
    float c1[4];                    // 0x30
};

// t4Vector with EA's 4-float constructor.
struct t4Vec_4F50 : sWakeVecE398 {
    t4Vec_4F50(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

extern sWakeVecE398 t4vect_ZAxis_4F50 __asm__("t4vect_ZAxis");     // t4vect::ZAxis
extern float cSinCosTable_gSinCosTable[];                                      // cSinCosTable::gSinCosTable

static inline float FXRand_4F50(float lo, float hi)
{
    return lo + (hi - lo) * wakeRandE398();
}

static inline float deg2rad_4F50(float d)
{
    return d * 0.01745329424738884f;
}

static inline void sincos_4F50(float a, float& s, float& c)
{
    int i = (int)(a * 81.4873275756836f) & 0x1FF;
    s = cSinCosTable_gSinCosTable[i];
    c = cSinCosTable_gSinCosTable[i + 0x80];
}

static inline void BringIntoRange_4F50(float& v, float range)
{
    if (v > range * 0.5f) {
        float q = v / range;
        v = (q - (float)(int)q) * range - range;
    } else if (v < -(range * 0.5f)) {
        float q = v / range;
        v = (q - (float)(int)q) * range + range;
    }
}

// PORT: PS2-only VU0 inline asm (scalar * vector).
static inline sWakeVecE398 Scale_4F50(float s, const sWakeVecE398& v)
{
    sWakeVecE398 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

// The unit declares cSnowBox_update(int*, sWakeRec_5DA0*) later; define it through a typed alias.
extern "C" void cSnowBox_update_4F50(cSnowBox* self, const tWorldPainterSnowfallParameters_4F50* params) __asm__("cSnowBox_update");
/**
 * Updates a snow box for this frame: takes its flake settings from the snowfall parameters, varies the wind and moves
 * the box offset.
 *
 * Mode 0 (+0x4) takes its alpha, colour (+0xA4..+0xAC), flake count (parameter 0 * 500), wind strength (parameter 1 *
 * 500) and +0x14 from the first parameter set; mode 1 takes the second colour set, parameter 2 * 6 flakes and parameter
 * 3 * 250 wind. When the wind timer (+0x90) runs out the old wind (+0x30) is kept at +0x40 and a new one is picked: the
 * strength along the parameter-5 angle, mixed with random flurries by parameter 6, for 1 to 5 seconds. The offset
 * (+0x20) moves by the blended wind and the fall (+0x1C * parameter 7) times the frame time (1 / cAppMan_TheAppMan
 * +0x10) and is wrapped into the box size (+0xC). cSnowFall_update calls it per box.
 *
 * @param params  the snowfall parameters filled by cSnowFall_update
 */
extern "C" void cSnowBox_update_4F50(cSnowBox* self, const tWorldPainterSnowfallParameters_4F50* params)
{
    switch (self->mSnowBoxType) {
    case 0:
        self->mOpacity = params->c0[0];
        ((float*)&self->mColourR)[0] = params->c0[1];
        ((float*)&self->mColourR)[1] = params->c0[2];
        ((float*)&self->mColourR)[2] = params->c0[3];
        self->mNumFlakes = (int)(params->f[0] * 500.0f);
        self->mWindScale = params->f[1] * 500.0f;
        self->mFlakeSize = params->f[4];
        break;
    case 1:
        self->mOpacity = params->c1[0];
        ((float*)&self->mColourR)[0] = params->c1[1];
        ((float*)&self->mColourR)[1] = params->c1[2];
        ((float*)&self->mColourR)[2] = params->c1[3];
        self->mNumFlakes = (int)(params->f[2] * 6.0f);
        self->mWindScale = params->f[3] * 250.0f;
        break;
    }
    const float kTimeDiff = 1.0f / (float)cAppMan_TheAppMan->TimerHz;
    float windBlend;
    float x;
    float y;
    float z;
    self->mCurrentTimeToWindChange -= kTimeDiff;
    if (self->mCurrentTimeToWindChange < 0.0f) {
        *(sWakeVecE398*)&self->mWindLast = *(sWakeVecE398*)&self->mWindNext;
        float windSin;
        float windCos;
        sincos_4F50(deg2rad_4F50(params->f[5]), windSin, windCos);
        t4Vec_4F50 windBaseRotated(self->mWindScale * windCos, self->mWindScale * windSin, 0.0f, 0.0f);
        t4Vec_4F50 windBaseFlurries(windBaseRotated.x * (1.0f - params->f[6]) + params->f[6] * FXRand_4F50(-self->mWindScale, self->mWindScale),
                                    windBaseRotated.y * (1.0f - params->f[6]) + params->f[6] * FXRand_4F50(-self->mWindScale, self->mWindScale),
                                    params->f[6] * FXRand_4F50(-self->mWindScale, self->mWindScale), 0.0f);
        *(sWakeVecE398*)&self->mWindNext = windBaseFlurries;
        self->mCurrentTimeToWindChange = self->mTotalTimeToWindChange = FXRand_4F50(1.0f, 5.0f);
        if (self->mResetWind) {
            *(sWakeVecE398*)&self->mWindLast = *(sWakeVecE398*)&self->mWindNext;
            self->mResetWind = 0;
        }
    }
    windBlend = self->mCurrentTimeToWindChange / self->mTotalTimeToWindChange;
    wakeAddE398(*(sWakeVecE398*)&self->mOffset, wakeScale0EE8(wakeAdd0EE8(wakeScale0EE8(*(sWakeVecE398*)&self->mWindNext, 1.0f - windBlend), wakeScale0EE8(*(sWakeVecE398*)&self->mWindLast, windBlend)), kTimeDiff));
    wakeAddE398(*(sWakeVecE398*)&self->mOffset, Scale_4F50(self->mGravity * params->f[7] * kTimeDiff, t4vect_ZAxis_4F50));
    x = (*(sWakeVecE398*)&self->mOffset).x;
    y = (*(sWakeVecE398*)&self->mOffset).y;
    z = (*(sWakeVecE398*)&self->mOffset).z;
    BringIntoRange_4F50(x, self->mBoxScale);
    BringIntoRange_4F50(y, self->mBoxScale);
    BringIntoRange_4F50(z, self->mBoxScale);
    *(sWakeVecE398*)&self->mOffset = t4Vec_4F50(x, y, z, 1.0f);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowBox_updateForCameraPosition);
#ifdef SKIP_ASM
struct sWakeVec5430 {
    float x, y, z, w;
    sWakeVec5430() {}
    sWakeVec5430(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sWakeGrid5430 {
    char pad_0x0[0xC];
    float size;                 // 0xC
    char pad_0x10[0x10];
    sWakeVec5430 pos;           // 0x20
};

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void wakeSub5430(sWakeVec5430& a, const sWakeVec5430& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

static inline void wakeWrap5430(float& r, float v, float s)
{
    if (v > s * 0.5f) {
        float q = v / s;
        r = (q - (float)(int)q) * s - s;
    } else if (v < -(s * 0.5f)) {
        float q = v / s;
        r = (q - (float)(int)q) * s + s;
    }
}

/**
 * Moves a snow box's offset by the camera's movement and wraps it back into the box size.
 *
 * Subtracts @p delta from the offset (+0x20) and wraps each axis into the box size (+0xC). cSnowFall_render calls it
 * with the camera's change in position.
 *
 * @param delta  the camera movement since the last frame
 */
extern "C" void cSnowBox_updateForCameraPosition(cSnowBox* self, sWakeVec5430* delta)
{
    wakeSub5430(*(sWakeVec5430*)&self->mOffset, *delta);
    float x = self->mOffset.x;
    float y = self->mOffset.y;
    float z = self->mOffset.z;
    float px = x;
    float py = y;
    float pz = z;
    wakeWrap5430(x, px, self->mBoxScale);
    wakeWrap5430(y, py, self->mBoxScale);
    wakeWrap5430(z, pz, self->mBoxScale);
    *(sWakeVec5430*)&self->mOffset = sWakeVec5430(x, y, z, 1.0f);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowBox_render);
#ifdef SKIP_ASM
struct sMat_6008;
extern char* GrMan_boardwakefx __asm__("GrMan");

struct sVec55D8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat55D8 {
    sVec55D8 v[4];
    sMat55D8() {}
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    sMat55D8(const sMat55D8& o)
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
            : "r"(this), "r"(&o)
            : "memory");
    }
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    sMat55D8& operator=(const sMat55D8& o)
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
            : "r"(this), "r"(&o)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));



struct sVt55D8 {
    short delta;
    short index;
    void (*fn)(void*, tSnowBoxData*);
};

// PORT: 128-bit TImode quadword (the PC port needs a 16-byte struct).
typedef int cQuad55D8 __attribute__((mode(TI)));
extern cQuad55D8 t4vect_XAxis;
extern cQuad55D8 t4vect_YAxis;
extern cQuad55D8 t4vect_ZAxis;

// PORT: PS2-only VU0 inline asm (4x4 matrix * matrix).
static inline sMat55D8 wakeMulMat55D8(const sMat55D8& a, const sMat55D8& b)
{
    sMat55D8 r;
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
        : "r"(&r), "r"(&a), "r"(&b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s).
static inline sVec55D8 wakeDiv55D8(const sVec55D8& v, float s)
{
    sVec55D8 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a += b).
static inline void wakeAddEq55D8(sVec55D8& a, const cQuad55D8& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void wakeSubEq55D8(sVec55D8& a, const cQuad55D8& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void wakeSubV55D8(sVec55D8& a, const sVec55D8& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

/**
 * Draws one snow box: builds its draw record and hands it to the graphics manager.
 *
 * Does nothing while the box is inactive (+0x8). The record holds @p snowMatrix times the box matrix (+0x50), the offset (+0x20)
 * divided by the size (+0xC) and shifted by one box toward the cell (@p x, @p y, @p z) to (x+1, y+1, z+1), minus 1.5;
 * the colour (+0xA4..+0xAC) and alpha (+0x18) times 128; the flake values +0x8, +0x98, +0x9C, +0xA0; and the cell
 * bounds with +0x14 and the size. Type 0 (+0x4) draws through GrMan virtual slot 85, type 1 through slot 86.
 * cSnowFall_render calls it for each box.
 *
 * @param snowBox  the snow box
 * @param snowMatrix  the snow matrix
 * @param x  minimum x of the cell
 * @param y  minimum y of the cell
 * @param z  minimum z of the cell
 */
extern "C" void cSnowBox_render(int* snowBox, sMat_6008* snowMatrix, float x, float y, float z)
{
    cSnowBox* self = (cSnowBox*)snowBox;
    if (self->mNumFlakes == 0) {
        return;
    }
    float x1 = x + 1.0f;
    float y1 = y + 1.0f;
    float z1 = z + 1.0f;
    tSnowBoxData info;
    (*(sMat55D8*)&info.mSnowBoxToWorldMatrix) = wakeMulMat55D8(*(sMat55D8*)snowMatrix, (*(sMat55D8*)&self->mBoxMatrix));
    sVec55D8 tempVec = wakeDiv55D8((*(sVec55D8*)&self->mOffset), self->mBoxScale);
    (*(sVec55D8*)&info.mParticleOffset) = tempVec;
    float red = self->mColourR * 128.0f;
    float green = self->mColourG * 128.0f;
    float blue = self->mColourB * 128.0f;
    float alpha = self->mOpacity * 128.0f;
    tempVec.y = green;
    info.mNumFlakes = self->mNumFlakes;
    info.mRandSeed = self->mRandSeed;
    info.mRandSeed2 = self->mRandSeed2;
    info.mRandSeed3 = self->mRandSeed3;
    (*(sVec55D8*)&info.mMinX).w = self->mFlakeSize;
    (*(sVec55D8*)&info.mMaxX).w = self->mBoxScale;
    tempVec.x = red;
    tempVec.z = blue;
    (*(sVec55D8*)&info.mMinX).x = x;
    (*(sVec55D8*)&info.mMinX).y = y;
    (*(sVec55D8*)&info.mMinX).z = z;
    (*(sVec55D8*)&info.mMaxX).x = x1;
    (*(sVec55D8*)&info.mMaxX).y = y1;
    (*(sVec55D8*)&info.mMaxX).z = z1;
    tempVec.w = alpha;
    (*(sVec55D8*)&info.mColour) = tempVec;
    if ((*(sVec55D8*)&info.mParticleOffset).x < x + -0.5f) {
        wakeAddEq55D8((*(sVec55D8*)&info.mParticleOffset), t4vect_XAxis);
    }
    if (x1 + 0.5f < (*(sVec55D8*)&info.mParticleOffset).x) {
        wakeSubEq55D8((*(sVec55D8*)&info.mParticleOffset), t4vect_XAxis);
    }
    if ((*(sVec55D8*)&info.mParticleOffset).y < y + -0.5f) {
        wakeAddEq55D8((*(sVec55D8*)&info.mParticleOffset), t4vect_YAxis);
    }
    if (y1 + 0.5f < (*(sVec55D8*)&info.mParticleOffset).y) {
        wakeSubEq55D8((*(sVec55D8*)&info.mParticleOffset), t4vect_YAxis);
    }
    if ((*(sVec55D8*)&info.mParticleOffset).z < z + -0.5f) {
        wakeAddEq55D8((*(sVec55D8*)&info.mParticleOffset), t4vect_ZAxis);
    }
    if (z1 + 0.5f < (*(sVec55D8*)&info.mParticleOffset).z) {
        wakeSubEq55D8((*(sVec55D8*)&info.mParticleOffset), t4vect_ZAxis);
    }
    tempVec.x = 1.5f;
    tempVec.y = 1.5f;
    tempVec.z = 1.5f;
    tempVec.w = 0.0f;
    wakeSubV55D8((*(sVec55D8*)&info.mParticleOffset), tempVec);
    switch (self->mSnowBoxType) {
    case 0: {
        sVt55D8* vt = *(sVt55D8**)(GrMan_boardwakefx + 0x10D8);
        vt[85].fn(vthis(GrMan_boardwakefx, vt[85]), &info);
        break;
    }
    case 1: {
        sVt55D8* vt = *(sVt55D8**)(GrMan_boardwakefx + 0x10D8);
        vt[86].fn(vthis(GrMan_boardwakefx, vt[86]), &info);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowFall_cSnowFall);
#ifdef SKIP_ASM
extern void* cSnowFall_vtbl[];
extern "C" void cMemMan_validate(void);
extern char str_SnowBox[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

struct sVec5920 {
    float x, y, z, w;
    sVec5920() {}
    sVec5920(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    void* operator new[](unsigned int, void* p) { return p; }
} __attribute__((aligned(16)));


/**
 * Constructs the falling snow: six snow boxes ("SnowBox") per view, the second set only for two players.
 *
 * Each view gets four boxes of one kind (sizes probably 1500, 1500, 2000 and 3000) and two of another kind
 * (probably the fluff). cMemMan_validate (unknown) is called before and after.
 *
 * @param bucket  update bucket (On Tour const int bucket)
 * @return self
 */
extern "C" void* cSnowFall_cSnowFall(cSnowFall* self, void* bucket)
{
    cObjNode_cObjNode1(self, bucket);
    self->mVtbl = cSnowFall_vtbl;
    new (((sVec5920*)&self->mLastCameraPosition[0])) sVec5920[2];
    cMemMan_validate();
    for (int i = 11; i >= 0; i--) {
        self->mSnowBoxes[i] = 0;
    }
    for (int j = 0; j < 2; j++) {
        ((sVec5920*)&self->mLastCameraPosition[0])[j] = sVec5920(0.0f, 0.0f, 0.0f, 1.0f);
    }
    self->mSnowBoxes[0] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 0, 0, 1500.0f, 3.0f, 1.0f, -200.0f);
    self->mSnowBoxes[1] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 0, 0, 1500.0f, 3.0f, 1.0f, -200.0f);
    self->mSnowBoxes[2] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 0, 0, 2000.0f, 3.0f, 1.0f, -200.0f);
    self->mSnowBoxes[3] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 0, 0, 3000.0f, 3.0f, 1.0f, -200.0f);
    self->mSnowBoxes[4] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 0, 1, 2000.0f, 600.0f, 0.05999999865889549f, -100.0f);
    self->mSnowBoxes[5] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 0, 1, 2000.0f, 600.0f, 0.05999999865889549f, -100.0f);
    if (SSXApp->Game->AI->NumberOfPlayersAllocated >= 2) {
        self->mSnowBoxes[6] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 1, 0, 1500.0f, 3.0f, 1.0f, -200.0f);
        self->mSnowBoxes[7] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 1, 0, 1500.0f, 3.0f, 1.0f, -200.0f);
        self->mSnowBoxes[8] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 1, 0, 2000.0f, 3.0f, 1.0f, -200.0f);
        self->mSnowBoxes[9] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 1, 0, 3000.0f, 3.0f, 1.0f, -200.0f);
        self->mSnowBoxes[10] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 1, 1, 2000.0f, 600.0f, 0.05999999865889549f, -100.0f);
        self->mSnowBoxes[11] = (cSnowBox*)cSnowBox_cSnowBox((cSnowBox*)cMemMan_alloc(0xC0, str_SnowBox, 0, 0), 1, 1, 2000.0f, 600.0f, 0.05999999865889549f, -100.0f);
    }
    cMemMan_validate();
    return (self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cSnowFall__cSnowFall);
#ifdef SKIP_ASM
extern void* cSnowFall_vtbl[];
extern "C" void cMemMan_validate(void);
extern "C" void cSnowBox__cSnowBox(int* self, int flags);
extern "C" void cObjNode__cObjNode(void* self, int flags);

/**
 * Destroys the falling snow: deletes its twelve snow boxes and runs the cObjNode destructor.
 *
 * @param self   the snowfall node
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cSnowFall__cSnowFall(cSnowFall* self, int flags)
{
    self->mVtbl = cSnowFall_vtbl;
    cMemMan_validate();
    int i;
    for (i = 0; i < 12; i++) {
        int* snowBox = ((int*)self->mSnowBoxes[i]);
        if (snowBox != 0) {
            cSnowBox__cSnowBox(snowBox, 3);
        }
    }
    cMemMan_validate();
    cObjNode__cObjNode(self, flags);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cSnowFall_update);
#ifdef SKIP_ASM
extern int cSnowfallToggles_m_EnableTest;
extern float cSnowfallToggles_m_TestSnowIntensity;
extern float cSnowfallToggles_m_TestWindIntensity;
extern float cSnowfallToggles_m_TestFlakeSize;
extern float cSnowfallToggles_m_TestWindRotation;
extern float cSnowfallToggles_m_TestSnowFlurries;
extern float cSnowfallToggles_m_SnowGravityMultiplier;
extern float cSnowfallToggles_m_SnowFluffItensity;
extern float cSnowfallToggles_m_SnowFluffWindIntensity;
struct sWakeV4_5DA0 { float x, y, z, w; };
extern sWakeV4_5DA0 cSnowfallToggles_m_SnowFlakeColour;
extern sWakeV4_5DA0 cSnowfallToggles_m_SnowFluffColour;
extern "C" float cRenderStateMan_SnowIntensity(int i);
extern "C" float cRenderStateMan_WindIntensity(int i);
extern "C" float cRenderStateMan_SnowFluffItensity(int i);
extern "C" float cRenderStateMan_SnowFluffWindIntensity(int i);
extern "C" float cRenderStateMan_FlakeSize(int i);
extern "C" float cRenderStateMan_WindRotation(int i);
extern "C" float cRenderStateMan_SnowFlurries(int i);
extern "C" float cRenderStateMan_SnowGravityMultiplier(int i);
extern "C" float cRenderStateMan_SnowFlakeColourA(int i);
extern "C" float cRenderStateMan_SnowFlakeColourR(int i);
extern "C" float cRenderStateMan_SnowFlakeColourG(int i);
extern "C" float cRenderStateMan_SnowFlakeColourB(int i);
extern "C" float cRenderStateMan_SnowFluffColourA(int i);
extern "C" float cRenderStateMan_SnowFluffColourR(int i);
extern "C" float cRenderStateMan_SnowFluffColourG(int i);
extern "C" float cRenderStateMan_SnowFluffColourB(int i);
void* cGame_getSplashManager(void* self, int i);
extern "C" void cSplashManager_ReportSnowfall(void* obj, float v);

struct sWakeRec_5DA0 {
    float f[8];
    sWakeV4_5DA0 c0;
    sWakeV4_5DA0 c1;
};
extern "C" void cSnowBox_update(int* elem, sWakeRec_5DA0* rec);

/**
 * Updates the falling snow from each view's snow settings, then advances every snow box with its view's
 * settings.
 *
 * The settings (snow and wind intensity, fluff, flake size, wind rotation, flurries, gravity and two colours)
 * come from the debug snowfall toggles when cSnowfallToggles_m_EnableTest is set. Otherwise they come from
 * the render-state manager (entries 6 and 7). The snow intensity also goes to cSplashManager_ReportSnowfall with the view's
 * cGame_getSplashManager object.
 *
 * @param self  the snowfall node
 */
extern "C" void cSnowFall_update(cSnowFall* self)
{
    sWakeRec_5DA0 recs[2];
    unsigned int i;
    for (i = 0; i < SSXApp->Game->ViewMan->m_uiNumView; i++) {
        if (cSnowfallToggles_m_EnableTest != 0) {
            recs[i].f[0] = cSnowfallToggles_m_TestSnowIntensity;
            recs[i].f[1] = cSnowfallToggles_m_TestWindIntensity;
            recs[i].f[2] = cSnowfallToggles_m_SnowFluffItensity;
            recs[i].f[3] = cSnowfallToggles_m_SnowFluffWindIntensity;
            recs[i].f[4] = cSnowfallToggles_m_TestFlakeSize;
            recs[i].f[5] = cSnowfallToggles_m_TestWindRotation;
            recs[i].f[6] = cSnowfallToggles_m_TestSnowFlurries;
            recs[i].f[7] = cSnowfallToggles_m_SnowGravityMultiplier;
            const sWakeV4_5DA0* flakeColour = &cSnowfallToggles_m_SnowFlakeColour; recs[i].c0.x = flakeColour->x; recs[i].c0.y = flakeColour->y; recs[i].c0.z = flakeColour->z; recs[i].c0.w = flakeColour->w;
            const sWakeV4_5DA0* fluffColour = &cSnowfallToggles_m_SnowFluffColour; recs[i].c1.x = fluffColour->x; recs[i].c1.y = fluffColour->y; recs[i].c1.z = fluffColour->z; recs[i].c1.w = fluffColour->w;
        } else {
            int k = (i == 0) ? 6 : 7;
            recs[i].f[0] = cRenderStateMan_SnowIntensity(k);
            recs[i].f[1] = cRenderStateMan_WindIntensity(k);
            recs[i].f[2] = cRenderStateMan_SnowFluffItensity(k);
            recs[i].f[3] = cRenderStateMan_SnowFluffWindIntensity(k);
            recs[i].f[4] = cRenderStateMan_FlakeSize(k);
            recs[i].f[5] = cRenderStateMan_WindRotation(k);
            recs[i].f[6] = cRenderStateMan_SnowFlurries(k);
            recs[i].f[7] = cRenderStateMan_SnowGravityMultiplier(k);
            recs[i].c0.x = cRenderStateMan_SnowFlakeColourA(k);
            recs[i].c0.y = cRenderStateMan_SnowFlakeColourR(k);
            recs[i].c0.z = cRenderStateMan_SnowFlakeColourG(k);
            recs[i].c0.w = cRenderStateMan_SnowFlakeColourB(k);
            recs[i].c1.x = cRenderStateMan_SnowFluffColourA(k);
            recs[i].c1.y = cRenderStateMan_SnowFluffColourR(k);
            recs[i].c1.z = cRenderStateMan_SnowFluffColourG(k);
            recs[i].c1.w = cRenderStateMan_SnowFluffColourB(k);
        }
        cSplashManager_ReportSnowfall(cGame_getSplashManager(SSXApp->Game, i), recs[i].f[0]);
    }
    int j;
    for (j = 0; j < 12; j++) {
        int* snowBox = ((int*)self->mSnowBoxes[j]);
        if (snowBox != 0) {
            cSnowBox_update(snowBox, &recs[*snowBox]);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cSnowFall_render);
#ifdef SKIP_ASM
struct sV4_6008 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sMat_6008 {
    sV4_6008 v[4];
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    sMat_6008& operator=(const sMat_6008& o)
    {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"(this), "r"(&o)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

struct sRsState_6008 {
    int f0;                 // 0x0
    int flagsA;             // 0x4
    int flagsB;             // 0x8
    int fC;                 // 0xC
    short tex;              // 0x10
    short pad;
};

struct sRsVEnt_6008 {
    short delta;
    short index;
    sMat_6008* (*fn)(void*);
};

struct sRsCtx_6008 {
    char pad0[0xE84];
    sRsState_6008* top;         // 0xE84
    char pad1[0xF70 - 0xE88];
    int texF70;                 // 0xF70
    char pad2[0x10D8 - 0xF74];
    sRsVEnt_6008* vtable;       // 0x10D8
};

struct sRiders_6008 {
    int f0;
    char* riders[4];            // 0x4
    int cur;                    // 0x14
};

struct sWakeFx_6008 {
    char pad0[0x10];
    int* parts[12];             // 0x10
    sWakeVec5430 last[4];       // 0x40
};

extern int cRenderToggles_m_DisableSnowFall;
extern int cSnowfallToggles_m_TestAdditive[1];
extern sRsCtx_6008* cGraphicsMan_TheGraphicsMan_6008 __asm__("cGraphicsMan_TheGraphicsMan");
extern sRsState_6008 sDrawState_s_defaultDrawState_6008[] __asm__("sDrawState_s_defaultDrawState");
extern sV4_6008 t4vect_Origin;
extern "C" void cSnowBox_render(int* p, sMat_6008* m, float r, float g, float b);

static inline sRiders_6008* riders_6008()
{
    return (sRiders_6008*)SSXApp->Game->ViewMan;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sWakeVec5430 sub_6008(const sWakeVec5430& a, const sWakeVec5430& b)
{
    sWakeVec5430 r;
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

static inline int rsTexF70_6008(sRsCtx_6008* ctx)
{
    return ctx->texF70;
}

static inline void rsSetA_6008(sRsCtx_6008* ctx, int mask, int shift, int v)
{
    ctx->top->flagsA = (ctx->top->flagsA & ~mask) | ((v << shift) & mask);
}

static inline void rsSetB_6008(sRsCtx_6008* ctx, int mask, int shift, int v)
{
    ctx->top->flagsB = (ctx->top->flagsB & ~mask) | ((v << shift) & mask);
}

/**
 * Draws the falling snow for the current view, moving its boxes with the camera.
 *
 * The boxes of the current view move by that view's position change since the last frame (+0x20). The colour
 * comes from the view's +0x80 colour (times 0.6, minus 0.5). Skipped when cRenderToggles_m_DisableSnowFall is set;
 * cSnowfallToggles_m_TestAdditive selects additive blending.
 */
extern "C" void cSnowFall_render(sWakeFx_6008* self)
{
    if (cRenderToggles_m_DisableSnowFall != 0) {
        return;
    }
    sRiders_6008* rs = riders_6008();
    int k = rs->cur;
    sWakeVec5430 camPos = *(sWakeVec5430*)(rs->riders[k] + 0x20);
    sWakeVec5430 camDiff = sub_6008(camPos, self->last[k]);
    self->last[k] = camPos;
    sRsCtx_6008* ctx = cGraphicsMan_TheGraphicsMan_6008;
    ctx->top[1] = ctx->top[0];
    ctx->top++;
    *ctx->top = sDrawState_s_defaultDrawState_6008[0];
    rsSetA_6008(ctx, 0x400000, 22, 1);
    rsSetB_6008(ctx, 0x3E0, 5, 7);
    if (cSnowfallToggles_m_TestAdditive[0] != 0) {
        rsSetA_6008(ctx, 0x7C, 2, 7);
    } else {
        rsSetA_6008(ctx, 0x7C, 2, 5);
    }
    rsSetA_6008(ctx, 0x3, 0, 2);
    rsSetB_6008(ctx, 0x1FFFFC00, 10, 0);
    ctx->top->tex = rsTexF70_6008(ctx);
    sMat_6008 snowMatrix;
    snowMatrix = *ctx->vtable[35].fn(vthis(ctx, ctx->vtable[35]));
    snowMatrix.v[3] = t4vect_Origin;
    sRiders_6008* rs2 = riders_6008();
    sWakeVec5430 camDir = *(sWakeVec5430*)(rs2->riders[rs2->cur] + 0x80);
    float boxMinX = camDir.x * 0.6f + -0.5f;
    float boxMinY = camDir.y * 0.6f + -0.5f;
    float boxMinZ = camDir.z * 0.6f + -0.5f;
    int i;
    for (i = 0; i < 12; i++) {
        int* snowBox = self->parts[i];
        if (snowBox != 0 && *snowBox == k) {
            cSnowBox_updateForCameraPosition((cSnowBox*)((sWakeGrid5430*)snowBox), &camDiff);
            cSnowBox_render(self->parts[i], &snowMatrix, boxMinX, boxMinY, boxMinZ);
        }
    }
    ctx->top--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cSnowFall_readFromReplayFrame);
#ifdef SKIP_ASM
void cSnowBox_resetWind(void*);

/**
 * Resets every snow box (cSnowBox_resetWind); despite the name it reads no replay data (it takes no frame).
 *
 * @param self  the snowfall node
 */
extern "C" void cSnowFall_readFromReplayFrame(cSnowFall* self)
{
    int i;
    void** snowBoxes = (void**)(&self->mSnowBoxes[0]);
    for (i = 0; i < 12; i++) {
        if (snowBoxes[i] != 0) {
            cSnowBox_resetWind(snowBoxes[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSpeedBoostFX_cSpeedBoostFX);
#ifdef SKIP_ASM
extern const sVt9_3680 D_00488550;
extern const sVt22_3680 D_00488598;
extern char str_cSpeedBoostStreamerFX[];
extern "C" void cSpeedBoostFX_reset(cSpeedBoostFX* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, unsigned int flags, int d) __asm__("operator_new__FUi");
inline void* operator new[](unsigned int, void* p) { return p; }

struct sWakeSeg_6320 {
    char data[0x1C];
    sWakeSeg_6320() {}
};

struct sWakePt_6320 {
    char data[0x20];
    sWakePt_6320() {}
};

/**
 * Constructs a rider's speed boost streamers: 30 trail segments and 20 ribbon points, then resets them.
 *
 * When @p inChrg is set it also constructs the cRider virtual base at +0x3B0. It patches the rider's vtables
 * (the g++ 2.95 this-adjust fix-ups when not in charge).
 *
 * The points are "cSpeedBoostStreamerFX" (+0x3AC).
 *
 * @param inChrg  nonzero when this object is complete and must construct its virtual base
 * @return self
 */
// PORT: hand-written form of g++ 2.95's constructor for a class with a virtual base
// (cRider at +0x3B0): vtable copies with delta fixups when not in charge.
extern "C" void* cSpeedBoostFX_cSpeedBoostFX(cSpeedBoostFX* self, int inChrg)
{
    sVt9_3680 vtbl1;
    sVt22_3680 vtbl2;
    if (inChrg) {
        *(void**)self = (char*)self + 0x3B0;
        cRider_cRider((char*)self + 0x3B0);
    }
    *(void**)(*(char**)self + 0x6E8) = (void*)&D_00488550;
    *(void**)(*(char**)self + 0x6D0) = D_00459B90;
    *(void**)(*(char**)self + 0x6C0) = (void*)&D_00488598;
    if (inChrg == 0) {
        int vc;
        vtbl1 = D_00488550;
        *(void**)(*(char**)self + 0x6E8) = &vtbl1;
        {
            char* vbo = *(char**)self - 0x3B0;
            vc = (char*)self - vbo;
        }
        vtbl1.e[1].delta = D_00488550.e[1].delta + vc;
        vtbl2 = D_00488598;
        *(void**)(*(char**)self + 0x6C0) = &vtbl2;
        vtbl2.e[1].delta = D_00488598.e[1].delta + vc;
    }
    new ((char*)&self->mSpeedBoostTrail[0]) sWakeSeg_6320[30];
    sWakePt_6320*& pts = (sWakePt_6320*)self->mStreamBuf;
    pts = new (str_cSpeedBoostStreamerFX, 0, 0) sWakePt_6320[20];
    cSpeedBoostFX_reset(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSpeedBoostFX__cSpeedBoostFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardwakefx_classes.h"

/**
 * Destroys a speed boost effect: frees its ribbon points and calls func_0037C720 on the graphics manager with
 * 0.
 */
cVbDtor002E64C0::~cVbDtor002E64C0()
{
    if (m3AC != 0) cMemMan_free(m3AC);
    func_0037C720(GrMan_boardwakefx, 0.0f);
}

// PORT: the compiler names this dtor _$_15cVbDtor002E64C0; expose it under the address-derived name.
__asm__(".globl cSpeedBoostFX__cSpeedBoostFX\n.type cSpeedBoostFX__cSpeedBoostFX,@function\ncSpeedBoostFX__cSpeedBoostFX = _$_15cVbDtor002E64C0\n.size cSpeedBoostFX__cSpeedBoostFX,0x17C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor002E64C0$13cRiderVbC_bwf = D_00488550\n"
        "_vt$15cVbDtor002E64C0$14cRiderVbBC_bwf = D_00459B90\n"
        "_vt$15cVbDtor002E64C0$12cRiderVb_bwf = D_00488598\n"
        "_$_12cRiderVb_bwf = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSpeedBoostFX_reset);
#ifdef SKIP_ASM
struct sWakeVec4 {
    float x;
    float y;
    float z;
    float w;
} __attribute__((aligned(16)));

/**
 * Resets a speed boost effect; despite the name it reads no replay data (it takes no frame).
 *
 * Sets the colour to red, the ribbon texture to 57 and clears the trail. +0x3A4 is 20 for a rider with a view
 * (a player) and 18 for others.
 */
extern "C" void cSpeedBoostFX_reset(cSpeedBoostFX* self)
{
    sWakeVec4 defaultSide;
    defaultSide.x = 1.0f;
    defaultSide.y = 0.0f;
    defaultSide.z = 0.0f;
    defaultSide.w = 1.0f;
    self->mCurrentTexture = 57;
    *(int*)&self->mAlpha = 0;
    *(sWakeVec4*)&self->mLastSide = defaultSide;
    *(int*)&self->mWidth = 0;
    *(int*)&self->mVOffset = 0;
    *(int*)&self->mSpeedBoostTrailCurrentLength = 0;
    self->mSpeedBoostTrailNumSegs = 0;
    self->mSpeedBoostTrailCurrentSeg = 0;
    self->mStreamBufLen = 0;
    self->mStreamBufHead = 0;
    self->mMaxStreamerSegments = (*(int*)(*(char**)self + 0x870) >= 0) ? 20 : 18;
    self->mWasInAir = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSpeedBoostFX_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector vu0Scale(const t4Vector& v, float s)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector vu0Sub(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot(const t4Vector& a, const t4Vector& b)
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

int cRider_getMotionState(void*);


struct sSeg_66B8 {
    t4Vector a;             // 0x00
    t4Vector b;             // 0x10
};

struct sWake_66B8 {
    char* rider;            // 0x000 (virtual base)
    float f4;               // 0x004
    float f8;               // 0x008
    float fC;               // 0x00C
    float phase;            // 0x010
    int f14;                // 0x014
    int f18;                // 0x018
    char pad1C[0x398 - 0x1C];
    int tex;                // 0x398
    int head;               // 0x39C
    int count;              // 0x3A0
    int max;                // 0x3A4
    int air;                // 0x3A8
    sSeg_66B8* segs;        // 0x3AC
    float& Width() { return fC; }
};

extern int cPowerUpFXToggles_m_EnableSpeedBoostStreamers;
extern int cPowerUpFXToggles_m_TestSpeedBoostRibbon;
extern int cPowerUpFXToggles_m_TestRedRibbon;
extern int cPowerUpFXToggles_m_TestOrangeRibbon;
extern "C" void cSpeedBoostFX_UpdateSpeedBoostTrail_66B8(sWake_66B8* self) __asm__("cSpeedBoostFX_UpdateSpeedBoostTrail");

// PORT: <? and >? (g++ min/max operators)
static inline float Clamp_66B8(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

static inline float Max_66B8(float a, float b)
{
    return a >? b;
}

static inline int Approach_66B8(int x, int target, int step)
{
    if (target + step < x) {
        return x - step;
    }
    if (x < target - step) {
        return x + step;
    }
    return target;
}





// PORT: PS2-only VU0 inline asm (a -= b).
static inline void SubEq_66B8(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}


static inline t4Vector BonePos_66B8(char* o, int off)
{
    return ((t4Vector*)(*(char**)(*(char**)(o + 0x780) + 0x30) + (*(int*)(o + off) << 6)))[3];
}

static inline void Trail_66B8(sWake_66B8* self, const t4Vector& pos, int on, float k, float speed)
{
    if (on) {
        int prev = self->head;
        self->head++;
        self->head %= 20;
        if (self->count < self->max) {
            self->count++;
        }
        self->segs[self->head] = self->segs[prev];
        t4Vector d = vu0Scale(pos, 1.4f);
        for (int i = 0; i < 20; i++) {
            SubEq_66B8(self->segs[i].a, d);
            vu0AddEq(self->segs[i].b, d);
        }
    } else {
        self->count = Approach_66B8(self->count, 0, 1);
    }
    if (self->count > 0) {
        char* o = self->rider;
        t4Vector a = BonePos_66B8(o, 0x89C);
        t4Vector b = BonePos_66B8(o, 0x8B0);
        t4Vector c = BonePos_66B8(o, 0x8B8);
        if (vu0Dot(b, pos) < vu0Dot(c, pos)) {
            self->segs[self->head].a = vu0Sub(b, vu0Scale(a, 0.7f));
            self->segs[self->head].b = vu0Sub(c, vu0Scale(a, 0.7f));
        } else {
            self->segs[self->head].a = vu0Sub(c, vu0Scale(a, 0.7f));
            self->segs[self->head].b = vu0Sub(b, vu0Scale(a, 0.7f));
        }
        self->phase -= k * (speed * 0.016666668f);
        if (self->phase < 0.0f) {
            self->phase += 1.0f;
        }
        if (1.0f < self->phase) {
            self->phase -= 1.0f;
        }
    }
}

/**
 * Updates a rider's speed boost streamers: picks the ribbon by the uber level and grows or fades the trail.
 *
 * The ribbon texture is 61 at uber level 11, 60 while a power-up boost runs, 59 (red) at level 10 or more, 58
 * (orange) at 5 or more, otherwise 57. On the ground with a boost running, the trail length and width follow
 * the boost and speed; otherwise they fade by 5 % a frame. The trail is cleared on landing.
 */
extern "C" void cSpeedBoostFX_update(sWake_66B8* self)
{
    int air = cRider_getMotionState(self->rider) == 1;
    int on = 0;
    if (!air) {
        cRider* riderPtr = (cRider*)self->rider;
        if (0.0f < riderPtr->mBoostStrength || 0.0f < riderPtr->mPowerUpBoostTimer) {
            on = 1;
        }
    }
    if (self->air != 0 && !air) {
        self->count = 0;
        self->f18 = 0;
        self->f14 = 0;
    }
    self->air = air;
    cRider* riderPtr = (cRider*)self->rider;
    float k = 0.002f;
    int type = riderPtr->mUberLevel;
    if (type == 11) {
        k = 0.01f;
        self->tex = 61;
    } else if (0.01f < riderPtr->mPowerUpBoostTimer || cPowerUpFXToggles_m_TestSpeedBoostRibbon != 0) {
        k = 0.01f;
        self->tex = 60;
    } else if (type >= 10 || cPowerUpFXToggles_m_TestRedRibbon != 0) {
        k = 0.01f;
        self->tex = 59;
    } else if (type >= 5 || cPowerUpFXToggles_m_TestOrangeRibbon != 0) {
        k = 0.005f;
        self->tex = 58;
    } else {
        self->tex = 57;
    }
    riderPtr = (cRider*)self->rider;
    float speed = vu0Length(riderPtr->mVelocity);
    if (on) {
        float boostAmt = Clamp_66B8(Max_66B8(riderPtr->mPowerUpBoostTimer, riderPtr->mBoostStrength), 0.0f, 1.0f);
        self->f4 = (boostAmt * 400.0f + 400.0f) * (speed / 3333.3335f);
        self->Width() = (riderPtr->mUberLevel == 11) ? 50.0f : boostAmt * 9.0f + 6.0f;
        self->f8 = 1.0f;
    } else {
        self->f4 *= 0.95f;
        self->f8 *= 0.95f;
        self->Width() *= 0.95f;
    }
    if (cPowerUpFXToggles_m_EnableSpeedBoostStreamers != 0) {
        t4Vector pos = *(t4Vector*)(self->rider + 0x1A0);
        Trail_66B8(self, pos, on, k, speed);
    }
    cSpeedBoostFX_UpdateSpeedBoostTrail_66B8(self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSpeedBoostFX_UpdateSpeedBoostTrail);
#ifdef SKIP_ASM
// Three floats (12 bytes, unaligned); built through a temporary like a constructed value.
struct sV3E6C08 {
    float x, y, z;
    sV3E6C08() {}
    sV3E6C08(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};




int cRiderAnimBase_getCurrentAnimID(void*, int);

#define WAKE_MAX_E6C08(a, b) ((a) > (b) ? (a) : (b))

// PORT: PS2-only inline asm (float absolute value).
static inline float wakeAbsE6C08(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector wakeScaleE6C08(const t4Vector& v, float s)
{
    t4Vector r;
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
static inline t4Vector wakeAddE6C08(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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
static inline t4Vector wakeSubE6C08(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void wakeAddEqE6C08(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector wakeCrossE6C08(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wakeLenE6C08(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float wakeDotE6C08(const t4Vector& a, const t4Vector& b)
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

static inline t4Vector wakeRowE6C08(cRider* rider, int k)
{
    return ((t4Vector*)((char*)rider->mAnimModel->SkelMatrices + (rider->mBoneBoardChild << 6)))[k];
}

// Pull a corner down onto the ground plane, 10 units below.
static inline t4Vector wakeDropE6C08(cSpeedBoostFX* self, const t4Vector& c, const t4Vector& d)
{
    return wakeSubE6C08(c, wakeScaleE6C08(self->mRider->mGroundNormal, wakeDotE6C08(self->mRider->mGroundNormal, d) - 10.0f));
}

/**
 * Adds this frame's sample to a rider's speed-boost trail and trims the trail to its maximum length.
 *
 * Works only while the maximum length (+0x4) or the current total (+0x18) is positive and the rider moves. A sample
 * takes the ring's next slot (30 entries of 0x1C bytes at +0x50, head +0x30, count +0x14; a full ring drops its oldest
 * length from the total) and adds the distance from the last position (+0x20, from kRiderVt_getPosition). The board's
 * corners are the mBoneBoardChild position +-75 along its row 0 (reversed when IsRidingFakie) and +-17.5 along row 2;
 * with PressMode one end is pushed 150 further by the layer-2 animation. Measured along the side axis (mVelocity x
 * mGroundNormal, kept facing the previous one at +0x40), the two outermost corners are projected onto the ground plane
 * (10 units above mGroundContactPoint) and stored as the sample's edges. Then the oldest samples are dropped while the
 * total exceeds the maximum, keeping at least 3 when the maximum is above 1. cSpeedBoostFX_update calls it.
 */
extern "C" void cSpeedBoostFX_UpdateSpeedBoostTrail(cSpeedBoostFX* self)
{
    if (0.0f < self->mTrailLength || 0.0f < self->mSpeedBoostTrailCurrentLength) {
        if (0.0f < wakeLenE6C08(self->mRider->mVelocity)) {
            tSpeedBoostTrailSeg* ring;
            if (self->mSpeedBoostTrailNumSegs == 30) {
                self->mSpeedBoostTrailCurrentLength -= self->mSpeedBoostTrail[(self->mSpeedBoostTrailCurrentSeg + 29) % 30].mLength;
                ring = self->mSpeedBoostTrail;
            } else {
                ring = self->mSpeedBoostTrail;
                self->mSpeedBoostTrailNumSegs++;
            }
            self->mSpeedBoostTrailCurrentSeg = (self->mSpeedBoostTrailCurrentSeg + 29) % 30;
            char* sub = (char*)&self->mRider->mVtbl;
            sVEntry<t4Vector*(void*)>* vt = *(sVEntry<t4Vector*(void*)>**)sub;
            t4Vector pos = *vt[5].fn(vthis(sub, vt[5]));
            t4Vector boardPos = wakeRowE6C08(self->mRider, 3);
            t4Vector boardSide = wakeRowE6C08(self->mRider, 2);
            t4Vector boardFwd = wakeRowE6C08(self->mRider, 0);
            if (self->mRider->IsRidingFakie != 0) {
                boardFwd = wakeScaleE6C08(boardFwd, -1.0f);
            }
            float dist;
            if (self->mSpeedBoostTrailNumSegs >= 2) {
                dist = wakeLenE6C08(wakeSubE6C08(pos, self->mSpeedBoostTrailLastPos));
            } else {
                dist = 0.0f;
            }
            self->mSpeedBoostTrailLastPos = pos;
            ring[self->mSpeedBoostTrailCurrentSeg].mLength = 0.0f;
            ring[(self->mSpeedBoostTrailCurrentSeg + 1) % 30].mLength = dist;
            self->mSpeedBoostTrailCurrentLength += dist;
            t4Vector noseNeg = wakeSubE6C08(wakeAddE6C08(boardPos, wakeScaleE6C08(boardFwd, 75.0f)), wakeScaleE6C08(boardSide, 17.5f));
            t4Vector nosePos = wakeAddE6C08(wakeAddE6C08(boardPos, wakeScaleE6C08(boardFwd, 75.0f)), wakeScaleE6C08(boardSide, 17.5f));
            t4Vector tailNeg = wakeSubE6C08(wakeSubE6C08(boardPos, wakeScaleE6C08(boardFwd, 75.0f)), wakeScaleE6C08(boardSide, 17.5f));
            t4Vector tailPos = wakeAddE6C08(wakeSubE6C08(boardPos, wakeScaleE6C08(boardFwd, 75.0f)), wakeScaleE6C08(boardSide, 17.5f));
            float lean = 0.0f;
            if (self->mRider->PressMode != 0) {
                int s = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
                if ((s >= 23 && s <= 28) || s == 37 || s == 38) {
                    lean = 1.0f;
                } else {
                    lean = -1.0f;
                }
            }
            if (0.0f < lean) {
                wakeAddEqE6C08(tailNeg, wakeScaleE6C08(wakeScaleE6C08(boardFwd, 150.0f), lean));
                wakeAddEqE6C08(tailPos, wakeScaleE6C08(wakeScaleE6C08(boardFwd, 150.0f), lean));
            } else if (lean < 0.0f) {
                wakeAddEqE6C08(noseNeg, wakeScaleE6C08(wakeScaleE6C08(boardFwd, 150.0f), lean));
                wakeAddEqE6C08(nosePos, wakeScaleE6C08(wakeScaleE6C08(boardFwd, 150.0f), lean));
            }
            t4Vector n = wakeCrossE6C08(self->mRider->mVelocity, self->mRider->mGroundNormal);
            if (wakeDotE6C08(n, self->mLastSide) < 0.0f) {
                n = wakeScaleE6C08(n, -1.0f);
            }
            self->mLastSide = n;
            t4Vector noseNegOff = wakeSubE6C08(noseNeg, self->mRider->mGroundContactPoint);
            t4Vector nosePosOff = wakeSubE6C08(nosePos, self->mRider->mGroundContactPoint);
            t4Vector tailNegOff = wakeSubE6C08(tailNeg, self->mRider->mGroundContactPoint);
            t4Vector tailPosOff = wakeSubE6C08(tailPos, self->mRider->mGroundContactPoint);
            float h0 = wakeDotE6C08(noseNegOff, n);
            float h1 = wakeDotE6C08(nosePosOff, n);
            float h2 = wakeDotE6C08(tailNegOff, n);
            float h3 = wakeDotE6C08(tailPosOff, n);
            float s = 1.0f / WAKE_MAX_E6C08(wakeAbsE6C08(h0), WAKE_MAX_E6C08(wakeAbsE6C08(h1), WAKE_MAX_E6C08(wakeAbsE6C08(h2), wakeAbsE6C08(h3))));
            float noseNegNorm = h0 * s;
            float nosePosNorm = h1 * s;
            float tailNegNorm = h2 * s;
            float tailPosNorm = h3 * s;
            t4Vector lo;
            t4Vector hi;
            if (noseNegNorm < nosePosNorm && noseNegNorm < tailNegNorm + 0.5f && noseNegNorm < tailPosNorm + 0.5f) {
                lo = wakeDropE6C08(self, noseNeg, noseNegOff);
            } else if (nosePosNorm < noseNegNorm && nosePosNorm < tailNegNorm + 0.5f && nosePosNorm < tailPosNorm + 0.5f) {
                lo = wakeDropE6C08(self, nosePos, nosePosOff);
            } else if (tailNegNorm < noseNegNorm && tailNegNorm < nosePosNorm && tailNegNorm < tailPosNorm) {
                lo = wakeDropE6C08(self, tailNeg, tailNegOff);
            } else {
                lo = wakeDropE6C08(self, tailPos, tailPosOff);
            }
            if (nosePosNorm <= noseNegNorm && tailNegNorm - 0.5f <= noseNegNorm && tailPosNorm - 0.5f <= noseNegNorm) {
                hi = wakeDropE6C08(self, noseNeg, noseNegOff);
            } else if (noseNegNorm <= nosePosNorm && tailNegNorm - 0.5f <= nosePosNorm && tailPosNorm - 0.5f <= nosePosNorm) {
                hi = wakeDropE6C08(self, nosePos, nosePosOff);
            } else if (noseNegNorm <= tailNegNorm && nosePosNorm <= tailNegNorm && tailPosNorm <= tailNegNorm) {
                hi = wakeDropE6C08(self, tailNeg, tailNegOff);
            } else {
                hi = wakeDropE6C08(self, tailPos, tailPosOff);
            }
            (*(sV3E6C08*)&self->mSpeedBoostTrail[self->mSpeedBoostTrailCurrentSeg].mLeft) = sV3E6C08(lo.x, lo.y, lo.z);
            (*(sV3E6C08*)&self->mSpeedBoostTrail[self->mSpeedBoostTrailCurrentSeg].mRight) = sV3E6C08(hi.x, hi.y, hi.z);
        }
    }
    int keep = 0;
    if (1.0f < self->mTrailLength) {
        keep = 3;
    }
    while (keep < self->mSpeedBoostTrailNumSegs && self->mTrailLength < self->mSpeedBoostTrailCurrentLength) {
        self->mSpeedBoostTrailCurrentLength -= self->mSpeedBoostTrail[(self->mSpeedBoostTrailCurrentSeg + self->mSpeedBoostTrailNumSegs - 1) % 30].mLength;
        self->mSpeedBoostTrailNumSegs--;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSpeedBoostFX_render);
#ifdef SKIP_ASM
extern int cPowerUpFXToggles_m_EnableSpeedBoostFX;
extern "C" void* func_0037A260(void* gfx, int n);
extern "C" void func_0037A430(void* gfx);

struct sWakeV3_7A10 {
    float x, y, z;
};

struct sWakeV4_7A10 {
    float x, y, z, w;
    sWakeV4_7A10() {}
    void SetXYZ(const sWakeV3_7A10& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline sWakeV4_7A10 scale_7A10(const sWakeV4_7A10& v, float s)
{
    sWakeV4_7A10 r;
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
static inline sWakeV4_7A10 add_7A10(const sWakeV4_7A10& a, const sWakeV4_7A10& b)
{
    sWakeV4_7A10 r;
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
static inline sWakeV4_7A10 sub_7A10(const sWakeV4_7A10& a, const sWakeV4_7A10& b)
{
    sWakeV4_7A10 r;
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

// Vertex colour, alpha first (as sCol_E058).
struct sWakeCol_7A10 {
    float a, r, g, b;
};


// Trail segment (0x1C bytes, 30 of them at self+0x50).
struct sWakeSeg_7A10 {
    float len;              // 0x00
    sWakeV3_7A10 p0;        // 0x04
    sWakeV3_7A10 p1;        // 0x10
};

// Ribbon point pair (0x20 bytes, 20 of them at self->pts).
struct sWakePt_7A10 {
    sWakeV4_7A10 a;
    sWakeV4_7A10 b;
};

struct sBoardWake_7A10 {
    cRider* rider;              // 0x000 (virtual base pointer)
    float f4;                   // 0x004
    float alpha;                // 0x008
    float width;                // 0x00C
    float v0;                   // 0x010
    int nsegs;                  // 0x014
    float f18;                  // 0x018
    char pad1C[0x14];
    int head;                   // 0x030
    char pad34[0x1C];
    sWakeSeg_7A10 segs[30];     // 0x050
    int tex;                    // 0x398
    int ptHead;                 // 0x39C
    int npts;                   // 0x3A0
    int f3A4;                   // 0x3A4
    int f3A8;                   // 0x3A8
    sWakePt_7A10* pts;          // 0x3AC
};

// The renderer at GrMan; its 71st virtual draws a strip of n vertices.
class cGfx_7A10 {
public:
    char pad[0x10D8];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void DrawRibbon(int n, tBxVertex* v, int flags);
};

struct sGfxTex_7A10 {
    char pad[0xF50];
    int tex[1];             // 0xF50
};

static inline int texId_7A10(int i)
{
    return ((sGfxTex_7A10*)GrMan_boardwakefx)->tex[i];
}

// Bone position (row 3 of the rider's bone matrix unk_89C).
static inline sWakeV4_7A10 bonePos_7A10(cRider* rider)
{
    return *(sWakeV4_7A10*)((char*)rider->mAnimModel->SkelMatrices + (rider->mBoneHips << 6) + 0x30);
}

// Fills vertex i of a strip: colour (alpha scaled to 0..128), q = 1, position, then st.
static inline void setVtx_7A10(tBxVertex* base, int i, float s, float t, const sWakeCol_7A10& c, const sWakeV4_7A10& p)
{
    tBxVertex* vx = &base[i];
    vx->r = (int)(c.r * 255.0f);
    vx->g = (int)(c.g * 255.0f);
    vx->b = (int)(c.b * 255.0f);
    vx->a = (int)(c.a * 128.0f);
    vx->one = 1.0f;
    (*(sWakeV4_7A10*)&vx->p) = p;
    vx->u = s;
    vx->v = t;
}

/**
 * Draws a rider's speed boost: two ribbons through the trail points and a strip along the trail segments.
 *
 * The ribbons fade with the square of their distance from the newest point; the strip fades along its length.
 * Nothing is drawn when the trail is empty, short (under 3) or nearly transparent, or when
 * cPowerUpFXToggles_m_EnableSpeedBoostFX is off.
 */
extern "C" void cSpeedBoostFX_render(sBoardWake_7A10* self)
{
    if (self->npts == 0 || self->f4 < 3.0f || self->alpha < 0.05f || cPowerUpFXToggles_m_EnableSpeedBoostFX == 0)
        return;
    ((cGfx_DAB8*)GrMan_boardwakefx)->PushState();
    *((cGfx_DAB8*)GrMan_boardwakefx)->stateTop = sDrawState_s_defaultDrawState_DAB8;
    *(short*)(*(char**)(GrMan_boardwakefx + 0xE84) + 0x10) = texId_7A10(self->tex);
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f0_2 = 0;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->SetF22(1);
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f4_23 = 0;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f4_20 = 0;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f4_12 = 20;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f4_0 = 2;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f8_10 = 0;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f8_5 = 7;
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop->f4_2 = 7;
    {
        tBxVertex left[42];
        tBxVertex right[42];
        int n = 0;
        int vertIdx = 0;
        float k = 1.0f / (float)((self->npts - 1) * (self->npts - 1));
        sWakeV4_7A10 offs = scale_7A10(bonePos_7A10(self->rider), 0.7f);
        sWakeCol_7A10 col;
        col.a = 1.0f;
        col.r = 1.0f;
        col.g = 1.0f;
        col.b = 1.0f;
        sWakeV4_7A10 pa, pb;
        float dv = 1.0f / (float)self->npts;
        float x = 0.0f;
        float texV = self->v0;
        sWakeV4_7A10 dir = *(sWakeV4_7A10*)&self->rider->mMatrix.row[0];
        sWakeV4_7A10 side = scale_7A10(dir, self->width * 0.5f);
        int idx = self->ptHead;
        for (int i = 0; i < self->npts; i++) {
            if (idx < 0)
                idx = 19;
            col.a = self->alpha * (1.0f - x * x * k);
            if (col.a < 0.0f)
                col.a = 0.0f;
            n += 2;
            pa = add_7A10(self->pts[idx].a, offs);
            pb = add_7A10(self->pts[idx].b, offs);
            idx--;
            x += 1.0f;
            setVtx_7A10(left, vertIdx, 0.0f, texV, col, sub_7A10(pa, side));
            setVtx_7A10(right, vertIdx, 0.0f, texV, col, sub_7A10(pb, side));
            vertIdx++;
            setVtx_7A10(left, vertIdx, 1.0f, texV, col, add_7A10(pa, side));
            setVtx_7A10(right, vertIdx, 1.0f, texV, col, add_7A10(pb, side));
            vertIdx++;
            texV += dv;
        }
        if (n >= 4) {
            ((cGfx_7A10*)GrMan_boardwakefx)->DrawRibbon(n, left, 0);
            ((cGfx_7A10*)GrMan_boardwakefx)->DrawRibbon(n, right, 0);
        }
    }
    if (self->nsegs >= 2) {
        char* gfx = cGraphicsMan_TheGraphicsMan_boardwakefx;
        tBxVertex* verts = (tBxVertex*)func_0037A260(gfx, self->nsegs * 2);
        // PORT: g++ minimum operator (<?).
        float len = self->f4 <? self->f18;
        float acc = 0.0f;
        int j;
        int vertIdx = 0;
        for (j = 0; j < self->nsegs; j++) {
            acc += self->segs[j].len;
            float trailFrac = acc / len;
            float vv = self->v0 + trailFrac;
            float al = wakeClampE058(self->alpha * (1.0f - trailFrac), 0.0f, 1.0f);
            sWakeCol_7A10 vertColour;
            vertColour.a = wakeClampE058(al, 0.0f, 1.0f);
            vertColour.r = 1.0f;
            vertColour.g = 1.0f;
            vertColour.b = 1.0f;
            int si = (self->head + j) % 30;
            sWakeV4_7A10 pos;
            pos.SetXYZ(self->segs[si].p0);
            pos.w = 1.0f;
            setVtx_7A10(verts, vertIdx++, 0.0f, vv, vertColour, pos);
            pos.SetXYZ(self->segs[si].p1);
            pos.w = 1.0f;
            setVtx_7A10(verts, vertIdx++, 1.0f, vv, vertColour, pos);
        }
        func_0037A430(gfx);
    }
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop--;
}
#endif

