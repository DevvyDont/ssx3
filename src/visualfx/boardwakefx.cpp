#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
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
#include "animation/rideranimbase.h"
#include "visualfx/renderstateman.h"

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
extern "C" void cBoardWakeFX_reset(cBoardWakeFX* self);
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
        (*(char**)&self->mRider) = (char*)self + 0xC0;
        cRider_cRider((char*)self + 0xC0);
    }
    self->mRider->mScriptObjectVtbl = (void*)&D_00488DA8;
    self->mRider->mScriptObjRiderVtbl = D_00459B90;
    VPTR_AT(self->mRider, sizeof(cRiderState)) = (void**)&D_00488DF0;
    if (inChrg == 0) {
        int vc;
        vtbl1 = D_00488DA8;
        self->mRider->mScriptObjectVtbl = &vtbl1;
        {
            char* vbo = ((char*)self->mRider) - 0xC0;
            vc = (char*)self - vbo;
        }
        vtbl1.e[1].delta = D_00488DA8.e[1].delta + vc;
        vtbl2 = D_00488DF0;
        VPTR_AT(self->mRider, sizeof(cRiderState)) = (void**)&vtbl2;
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
    self->reset();
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX__cBoardWakeFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardwakefx_classes.h"

/**
 * Destroys a board wake effect's arrays: frees the wake segments (mWakeLines) and the point rows (mWakeVerts).
 */
cVbDtor002DCDA0::~cVbDtor002DCDA0()
{
    if (mWakeLines != 0) cMemMan_free(mWakeLines);
    if (mWakeVerts != 0) cMemMan_free(mWakeVerts);
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
extern "C" void cWakeParams_Clear(cWakeParams* self);

/**
 * Resets a board wake effect; despite the name it reads no replay data (it takes no frame, like On Tour's
 * reset).
 *
 * A rider with a view (a player) gets 5 rows, 32 segments and a 1.25 s life; others get 4 rows, 22 segments
 * and 0.8 s. It sets the colour to white and clears the state. When the row count changed, each row's points
 * get a height factor and the rows' sink speeds (0 down to -25) are recomputed.
 */
void cBoardWakeFX::reset()
{
    int old = mWakeLineSegs;
    float one;
    tARGB white;
    int i;
    if (mRider->mViewIndex >= 0) {
        mWakeLineSegs = 5;
        mWakeMaxNumLines = 0x20;
        mWakeLife = 1.25f;
    } else {
        mWakeLineSegs = 4;
        mWakeMaxNumLines = 22;
        mWakeLife = 0.800000011920929f;
    }
    one = 1.0f;
    white.A = one;
    white.R = one;
    white.G = one;
    white.B = one;
    mGroundColour = white;
    mWakeRandomnessSeed = 0.0f;
    mNumWakeLines = 0;
    mCurrentWakeLine = 0;
    mWasSpraying = 0;
    mWakeLength = 0;
    mWakeParams.Clear();
    if (old != mWakeLineSegs) {
        for (i = 0; i < 5; i++) {
            float heightFactor =
                0.9900000095367432f - (float)i * (0.9800000190734863f / (float)(mWakeLineSegs - 1));
            int j;
            for (j = 0; j < 32; j++) {
                mWakeVerts[i][j].u = 0.0f;
                mWakeVerts[i][j].v = heightFactor;
                mWakeVerts[i][j].one = 1.0f;
                mWakeVerts[i][j].w = 0;
            }
            mPreCalculatedGravityMultiplier[i] =
                (float)i / (float)(mWakeLineSegs - 1) * -25.000001907348633f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_update);
#ifdef SKIP_ASM
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
 * strength comes from the rider's speed, lean and sideways slip and the surface's WakeSpeed. There
 * is no wake off the ground, at a low strength, at a small lean or on a surface with little grip (WakeAlpha). A
 * running wake adds a segment every 50 units (createSeg, cBoardWakeFX_createSeg) and refreshes the current one
 * (fillCurrentSeg, cBoardWakeFX_fillCurrentSeg). It ends when the grip or strength falls too low or the lean changes side.
 */
// PORT: g++ minimum/maximum operators (<?, >?).
void cBoardWakeFX::update()
{
    tSurfData* surf = &SSXApp->Game->SurfPropCache->SurfData[mRider->mSurfaceType];
    int i;
    for (i = 0; i < mNumWakeLines; i++) {
        int idx = (mCurrentWakeLine + i) % mWakeMaxNumLines;
        tWakeLine* elem = &mWakeLines[idx];
        elem->mAge += 0.01666666753590107f;
        if (mWakeLife < elem->mAge) {
            mNumWakeLines = i;
            goto done;
        }
        float grow =
            ((1.0f - ((elem->mAge * 20.0f) <? 1.0f)) * elem->mExtraInitalVelocityFactor + 1.0f) * 0.01666666753590107f;
        float fade = (1.0f - elem->mAge * 3.3333332538604736f) >? 0.0f;
        t4Vector drift = wakeScaleD0B8(wakeScaleD0B8(elem->mAngleCorrection, fade), 0.01666666753590107f);
        wakeAddEqD0B8(mWakeVerts[0][idx].p, drift);
        for (int j = 1; j < mWakeLineSegs; j++) {
            wakeAddEqD0B8(mWakeVerts[j][idx].p, wakeAddD0B8(wakeScaleD0B8(elem->mVel[j], grow), drift));
            elem->mVel[j].z += mPreCalculatedGravityMultiplier[j];
        }
    }
done:
    float lean = -mRider->mSlewed[0].cur;
    float k = 1.0f - wakeAbsD0B8(mRider->mSlewed[3].cur);
    t4Vector normal = mRider->mGroundNormal;
    float amt = wakeAbsD0B8(lean) * k;
    t4Vector side;
    t4Vector fwd;
    sWakeParamsD0B8 curParams;
    fwd = wakeRow34D0B8(mRider, 0);
    if (mRider->IsRidingFakie != 0) {
        fwd = wakeScaleD0B8(fwd, -1.0f);
    }
    t4Vector right = wakeCrossD0B8(fwd, normal);
    side = lean < 0.0f ? wakeScaleD0B8(right, -1.0f) : right;
    float speed = WAKE_MAX_D0B8(WAKE_MIN_D0B8(wakeMaxSpeedD0B8(), wakeLenD0B8(mRider->mVelocity)), 0.0f);
    curParams.grip = surf->WakeAlpha;
    float sign = 1.0f;
    if (mRider->IsRidingFakie != 0) {
        sign = -1.0f;
    }
    float off = wakeAbsD0B8(mRider->mSlewed[2].cur) * 118.0f + -40.0f;
    t4Vector pos = wakeAddD0B8(
        wakeAddD0B8(wakeRowD0B8(mRider, 3),
                    wakeScaleD0B8(wakeScaleD0B8(wakeRowD0B8(mRider, 0), off), sign)),
        wakeScaleD0B8(wakeScaleD0B8(wakeScaleD0B8(wakeRowD0B8(mRider, 2), 15.0f), lean < 0.0f ? -1.0f : 1.0f),
            sign));
    t4Vector dirn = wakeNormD0B8(mRider->mVelocity);
    float bal = (wakeAbsD0B8(wakeDotD0B8(mRider->mGroundSide, dirn)) * 0.8f + amt * 0.5f) <? 1.0f;
    float baseStrength = wakeClampD0B8(bal + bal, 0.0f, 0.25f);
    float speedBonus = 0.0f;
    if (555.5555419921875f < speed) {
        speedBonus = wakeClampD0B8((bal - 0.25f) * 2.7f, 0.0f, 0.35f);
    }
    float width = 0.0f;
    if (surf->HasVeryTightTurnWake != 0 && 1111.111083984375f < speed) {
        width = wakeClampD0B8((bal - 0.54f) * 35.0f, 0.0f, 4.5f);
    }
    curParams.strength = speed * (baseStrength + speedBonus) * surf->WakeSpeed;
    int ok = 1;
    if (cRider_getMotionState(mRider) != 0 || wakeAbsD0B8(mRider->mSlewed[11].cur) > 0.15f ||
        wakeMinStrengthD0B8() > curParams.strength || amt < 0.15f || curParams.grip < 0.05f) {
        curParams.strength = 0.0f;
        ok = 0;
    }
    if (mWasSpraying != 0) {
        if (mLeanPositive != 0 ? lean < 0.0f : lean > 0.0f) {
            curParams.strength = 0.0f;
            mWakeParams.mSpeed = 0.0f;
        }
        if (cRider_getMotionState(mRider) == 0) {
            mWakeParams.mUp = wakeAddD0B8(wakeScaleD0B8(mWakeParams.mUp, 0.8f),
                wakeScaleD0B8(normal, 0.19999998807907104f));
            mWakeParams.mSide = wakeAddD0B8(wakeScaleD0B8(mWakeParams.mSide, 0.8f),
                wakeScaleD0B8(side, 0.19999998807907104f));
            mWakeParams.mForward = wakeAddD0B8(wakeScaleD0B8(mWakeParams.mForward, 0.8f),
                wakeScaleD0B8(fwd, 0.19999998807907104f));
            mWakeParams.mAlpha = mWakeParams.mAlpha * 0.7f + curParams.grip * 0.3f;
            mWakeParams.mSpeed = mWakeParams.mSpeed * 0.8f + curParams.strength * 0.19999998807907104f;
            if (mWakeParams.mAlpha > 0.05f && mWakeParams.mSpeed > 55.55555725097656f) {
                if (wakeLenD0B8(wakeSubD0B8(pos, mLastWakePos)) > 50.0f) {
                    cBoardWakeFX_createSeg_D0B8(this, surf, &pos);
                    mLastWakePos = pos;
                }
                cBoardWakeFX_fillCurrentSeg_D0B8(this, &pos, &fwd, width);
            } else {
                mWakeParams.mAlpha = 0.0f;
                mWasSpraying = 0;
                cBoardWakeFX_fillCurrentSeg_D0B8(this, &pos, &fwd, mWakeParams.mAlpha);
            }
        } else {
            mWakeParams.mAlpha = 0.0f;
            mWasSpraying = 0;
            cBoardWakeFX_fillCurrentSeg_D0B8(this, &pos, &fwd, mWakeParams.mAlpha);
        }
    } else if (ok) {
        mWakeParams.mSpeed = 0.0f;
        width = mWakeParams.mSpeed;
        mWakeParams.mForward = fwd;
        mWakeParams.mSide = side;
        mWakeParams.mUp = normal;
        mLastWakePos = pos;
        mWakeLength = 0;
        mLeanPositive = width < lean;
        mWakeParams.mAlpha = width;
        cBoardWakeFX_createSeg_D0B8(this, surf, &pos);
        cBoardWakeFX_fillCurrentSeg_D0B8(this, &pos, &fwd, width);
        mWakeParams.mAlpha = curParams.grip;
        cBoardWakeFX_createSeg_D0B8(this, surf, &pos);
        cBoardWakeFX_fillCurrentSeg_D0B8(this, &pos, &fwd, width);
        mWasSpraying = 1;
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

class cGfx_DAB8 {
public:
    char pad[0xE84];
    sGs_DAB8* stateTop;
    void PushState() { stateTop[1] = stateTop[0]; stateTop++; }
    char padE88[0x10D8 - 0xE88];
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
void cBoardWakeFX::render()
{
    if (cRenderToggles_m_DisableBoardWake != 0 || mNumWakeLines == 0)
        return;
    ((cGfx_DAB8*)GrMan_boardwakefx)->PushState();
    *((cGfx_DAB8*)GrMan_boardwakefx)->stateTop = sDrawState_s_defaultDrawState_DAB8;
    *GrMan->m_pCurrentDrawState->m_textureState.m_texid = GrMan->getEffectTexMap(56);
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
    int draw = mNumWakeLines;
    int n = draw;
    int over = draw + 2; over -= mWakeMaxNumLines;
    if (over >= 1)
        draw -= over;
    int last = (mCurrentWakeLine + n - 1) % mWakeMaxNumLines;
    float ageRatio = (mWakeLife - mWakeLines[last].mAge) / mWakeLife;
    float alphaStart = 1.0f;
    if (ageRatio <= 0.8f)
        alphaStart = ageRatio * k;
    float da = (alphaStart - k) / (float)n;
    if (draw >= 2)
        ((cGfx_DAB8*)GrMan_boardwakefx)->DrawStrip(((char*)&mWakeVerts[0]), mWakeLineSegs, mCurrentWakeLine,
                mWakeMaxNumLines, draw, k, da);
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop--;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_fillCurrentSeg);
#ifdef SKIP_ASM

extern t4Vector t4vect_Zero_DD30 __asm__("t4vect_Zero");
extern "C" float perlinNoise(int n, float x);

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector wakeScaleDD30(const t4Vector& v, float s)
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
static inline t4Vector wakeAddDD30(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector wakeSubDD30(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wakeLenDD30(const t4Vector& v)
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

// Places a new segment's head vertex 5 units below @p pos along the ground normal. (Its address is formed before the
// VU0 arithmetic, as in retail.)
static inline void placeWakeHead(tBoardWakeVertex* head, const t4Vector& pos, const t4Vector& groundNormal)
{
    t4Vector x = wakeSubDD30(pos, wakeScaleDD30(groundNormal, 5.0f));
    head->p = x;
}

/**
 * Fills the current board wake line from the wake parameters: its spray direction, its row points and the rows' head
 * positions.
 *
 * The line is mWakeLines[mCurrentWakeLine]. Its age and first point are zeroed and its mAngleCorrection becomes 0.85 *
 * (mVelocity - mForward * |mVelocity|). Point i of the mWakeLineSegs rows is (mSide * t * t * 0.75 + mUp * t) * w,
 * where t = i / (rows - 1) (times 0.9 for the last row) and w = mSpeed * (1 + 0.25 * perlinNoise(3,
 * mWakeRandomnessSeed)). Row 0's vertex at the head gets @p pos minus 5 * mGroundNormal and the alpha mAlpha * 128,
 * copied to the other rows. @p width goes to the line's mExtraInitalVelocityFactor.
 *
 * cBoardWakeFX_update calls it with an extra forward-vector argument between @p pos and @p width, which this definition
 * does not declare or use.
 *
 * @param pos  the wake position
 * @param width  the line's extra initial velocity factor
 */
void cBoardWakeFX::fillCurrentSeg(t4Vector* pos, float width)
{
    float w = mWakeParams.mSpeed * (perlinNoise(3, mWakeRandomnessSeed) * 0.25f + 1.0f);
    tWakeLine* wakeLine = &mWakeLines[mCurrentWakeLine];
    wakeLine->mAge = 0.0f;
    wakeLine->mVel[0] = t4vect_Zero_DD30;
    t4Vector dir = wakeScaleDD30(wakeScaleDD30(mWakeParams.mForward, -1.0f),
        wakeLenDD30(mRider->mVelocity));
    t4Vector neg = wakeScaleDD30(mRider->mVelocity, -1.0f);
    {
        t4Vector x = wakeScaleDD30(wakeSubDD30(dir, neg), 0.85f);
        wakeLine->mAngleCorrection = x;
    }
    for (int i = 0; i < mWakeLineSegs; i++) {
        float rowFrac = (float)i / (float)(mWakeLineSegs - 1);
        float rowFracSq = rowFrac * rowFrac;
        if (i == mWakeLineSegs - 1) {
            rowFrac *= 0.9f;
        }
        t4Vector x = wakeScaleDD30(wakeAddDD30(wakeScaleDD30(wakeScaleDD30(mWakeParams.mSide, rowFracSq), 0.75f),
            wakeScaleDD30(mWakeParams.mUp, rowFrac)), w);
        wakeLine->mVel[i] = x;
    }
    placeWakeHead(&mWakeVerts[0][mCurrentWakeLine], *pos, mRider->mGroundNormal);
    mWakeVerts[0][mCurrentWakeLine].a = (int)(mWakeParams.mAlpha * 128.0f);
    for (int j = 1; j < mWakeLineSegs; j++) {
        mWakeVerts[j][mCurrentWakeLine].p = mWakeVerts[0][mCurrentWakeLine].p;
        mWakeVerts[j][mCurrentWakeLine].a = mWakeVerts[0][mCurrentWakeLine].a;
    }
    wakeLine->mExtraInitalVelocityFactor = width;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_createSeg);
#ifdef SKIP_ASM

// cRenderStateMan::m_state: the per-view render states (visualfx/renderstateman.h, 8 rows of 0xF0 bytes).
extern tRenderState cRenderStateMan_m_state[];

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
 * Starts a new board wake segment: advances the ring head and sets the new vertices' texture offset and ground colour.
 *
 * Advances mWakeRandomnessSeed by 1/30, wrapping at 4, grows mNumWakeLines up to mWakeMaxNumLines and moves
 * mCurrentWakeLine back one place in the ring. mGroundColour is the rider's render-state colour (row
 * kRiderVt_getAIIndex of cRenderStateMan_m_state), doubled and clamped to 0..1. Row 0's vertex at the head gets u =
 * mWakeLength * 0.5 wrapped into -32..32 and the colour * 128 as integers; rows 1..4 copy it. Then mWakeLength
 * increments.
 *
 * cBoardWakeFX_update calls it with a surface record and a position, which this definition does not declare or use.
 */
void cBoardWakeFX::createSeg()
{
    mWakeRandomnessSeed += 0.03333333507180214f;
    if (mWakeRandomnessSeed >= 4.0f) {
        mWakeRandomnessSeed -= 4.0f;
    }
    if (mNumWakeLines < mWakeMaxNumLines) {
        mNumWakeLines++;
    }
    mCurrentWakeLine = (mCurrentWakeLine + mWakeMaxNumLines - 1) % mWakeMaxNumLines;
    char* sub = (char*)&VPTR_AT(mRider, sizeof(cRiderState));
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)sub;
    tARGB col = cRenderStateMan_m_state[vt[7].fn(sub + vt[7].delta)].m_colour;
    col.A *= 2.0f;
    col.R *= 2.0f;
    col.G *= 2.0f;
    col.B *= 2.0f;
    col.R = wakeClampE058(col.R, 0.0f, 1.0f);
    col.G = wakeClampE058(col.G, 0.0f, 1.0f);
    col.B = wakeClampE058(col.B, 0.0f, 1.0f);
    col.A = wakeClampE058(col.A, 0.0f, 1.0f);
    mGroundColour = col;
    mWakeVerts[0][mCurrentWakeLine].u = (float)mWakeLength * 0.5f;
    mWakeVerts[0][mCurrentWakeLine].u -=
        (float)(int)(mWakeVerts[0][mCurrentWakeLine].u * 0.015625f) * 64.0f + 32.0f;
    mWakeVerts[0][mCurrentWakeLine].r = (int)(mGroundColour.R * 128.0f);
    mWakeVerts[0][mCurrentWakeLine].g = (int)(mGroundColour.G * 128.0f);
    mWakeVerts[0][mCurrentWakeLine].b = (int)(mGroundColour.B * 128.0f);
    int i;
    for (i = 1; i < 5; i++) {
        mWakeVerts[i][mCurrentWakeLine].u = mWakeVerts[0][mCurrentWakeLine].u;
        mWakeVerts[i][mCurrentWakeLine].r = mWakeVerts[0][mCurrentWakeLine].r;
        mWakeVerts[i][mCurrentWakeLine].g = mWakeVerts[0][mCurrentWakeLine].g;
        mWakeVerts[i][mCurrentWakeLine].b = mWakeVerts[0][mCurrentWakeLine].b;
    }
    mWakeLength++;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cWakeParams_Clear);
#ifdef SKIP_ASM

extern t4Vector t4vect_Zero;

/**
 * Clears a board wake's parameter block: zeroes its three vectors (+0x0, +0x10, +0x20) and the words at +0x30 and
 * +0x34.
 *
 * cBoardWakeFX_reset (the wake reset) calls it on cBoardWakeFX + 0x60.
 */
void cWakeParams::Clear()
{
    mAlpha = 0.0f;
    mSpeed = 0.0f;
    mUp = t4vect_Zero;
    mForward = t4vect_Zero;
    mSide = t4vect_Zero;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_scatterEmitPosition);
#ifdef SKIP_ASM
extern int D_004A3AFC;


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
static inline void wakeScaleE398(t4Vector& r, const t4Vector& v, float s)
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
static inline void wakeAddE398(t4Vector& a, const t4Vector& b)
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

extern "C" void cDynamicSprayFX_scatterEmitPosition(cDynamicSprayFX* self, t4Vector* out);

/**
 * Scatters a spray emission position randomly around the board.
 *
 * With bias = the lean side (mLeanSide, -1..1) it adds mBoardForward times (bias * 55 + a random value within +-(1 -
 * |bias|) * 55), then mBoardSide times a random value in -10..10. cDynamicSprayFX_update fills both vectors from the board
 * bone's matrix rows 0 and 2. The random numbers come from the generator in D_004A3AFC. The spray updates
 * cDynamicSprayFX_updateCarveSpray, func_002E02B8, cDynamicSprayFX_updateBoardSpray and cDynamicSprayFX_updatePowderTrail call it.
 *
 * @param out  the position, changed in place
 */
void cDynamicSprayFX::scatterEmitPosition(t4Vector* out)
{
    float bias = mLeanSide;
    float spread = (1.0f - wakeAbsE398(bias)) * 55.0f;
    float lo = -spread;
    t4Vector offset;
    t4Vector scaled;
    wakeScaleE398(scaled, mBoardForward, bias * 55.0f + (lo + (spread - lo) * wakeRandE398()));
    offset = scaled;
    wakeAddE398(*out, offset);
    wakeScaleE398(scaled, mBoardSide, wakeRandE398() * 20.0f + -10.0f);
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
extern "C" void cDynamicSprayFX_tuneBurstEmitters(cDynamicSprayFX* self);
extern "C" void cDynamicSprayFX_reinitEmitters_E4A8(void* self) __asm__("cDynamicSprayFX_reinitEmitters");
extern "C" void cDynamicSprayFX_reset(cDynamicSprayFX* self);

/**
 * Constructs a rider's snow spray effect: ten particle emitters and their ten spray settings.
 *
 * When @p inChrg is set it also constructs the cRider virtual base at +0x130. It patches the rider's vtables
 * (the g++ 2.95 this-adjust fix-ups when not in charge).
 *
 * Allocates the emitters ("DynamicSprayEmitter") and the spray data ("DynamicSprayData", each set filled in
 * here with its particle count, life, size, gravity, colour and speeds) and a 0x78-byte "BoneIndicies" block.
 * It then calls cDynamicSprayFX_tuneBurstEmitters, cDynamicSprayFX_reinitEmitters and cDynamicSprayFX_reset (unknown) to finish the setup.
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
    cDynamicSprayFX_tuneBurstEmitters((cDynamicSprayFX*)self);
    cDynamicSprayFX_reinitEmitters_E4A8(self);
    cDynamicSprayFX_reset((cDynamicSprayFX*)self);
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX__cDynamicSprayFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardwakefx_classes.h"

/**
 * Destroys a snow spray effect's arrays: the emitters (mEmitters), the spray data (mSprayData) and the bone indices
 * (mBoneIndicies).
 */
cVbDtor002DF1C8::~cVbDtor002DF1C8()
{
    delete[] mEmitters;
    if (mSprayData != 0) cMemMan_free(mSprayData);
    if (mBoneIndicies != 0) cMemMan_free(mBoneIndicies);
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
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_reset);
#ifdef SKIP_ASM
extern "C" void cDynamicSprayFX_tuneBurstEmitters(cDynamicSprayFX* self);
extern "C" void cDynamicSprayFX_selectEmittersForLOD(cDynamicSprayFX* self);

/**
 * Resets a rider's snow spray effect.
 *
 * Sets mBurstAlpha to 0.5 and mColour to white, and zeroes mShedAmount, mPowderLevel, mHeadPuffGlow, mLastSpeed,
 * mShedBone, mBoneIndiciesValid (so the bone table is looked up again), mBurstStrength and the lean side mLeanSide.
 * When the stored player flag (mIsPlayer) no longer matches
 * whether the rider has a view (mViewIndex >= 0) it re-tunes the emitters with cDynamicSprayFX_tuneBurstEmitters; then it picks the
 * emitters for the rider's detail level (cDynamicSprayFX_selectEmittersForLOD). The cDynamicSprayFX constructor and cRiderHandler_resetRiderFX
 * (on cRiderHandler + 0xB40) call it.
 */
void cDynamicSprayFX::reset()
{
    t4Vector white;
    white.x = 1.0f;
    white.y = 1.0f;
    white.z = 1.0f;
    white.w = 1.0f;
    mBurstAlpha = 0.5f;
    mPowderLevel = 0.0f;
    mColour = white;
    mLastSpeed = 0.0f;
    mShedAmount = 0.0f;
    mHeadPuffGlow = 0.0f;
    mBoneIndiciesValid = 0;
    mShedBone = 0;
    mBurstStrength = 0.0f;
    mLeanSide = 0.0f;
    if (mIsPlayer != (mRider->mViewIndex >= 0)) {
        tuneBurstEmitters();
    }
    selectEmittersForLOD();
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_reinitEmitters);
#ifdef SKIP_ASM


struct cDynamicSprayFX_reinitEmitters_sFx {
    char pad0[0x28];
    cDynamicColourEmitter* elems;     // 0x28
    char* data;                     // 0x2C
    char pad30[0x60 - 0x30];
    int mbReinitEmitters;           // 0x60
};

/**
 * Re-initialises the ten spray emitters from their spray settings and clears the re-init flag (+0x60).
 *
 * Emitter i (0x210 bytes each, array at +0x28) gets its virtual slot 3 called with settings set i (0xE8 bytes each,
 * array at +0x2C) and the emitter's float at +0x58. The cDynamicSprayFX constructor calls it, and cDynamicSprayFX_update does
 * when +0x60 is set.
 */
extern "C" void cDynamicSprayFX_reinitEmitters(cDynamicSprayFX_reinitEmitters_sFx* self)
{
    int i;
    for (i = 0; i < 10; i++) {
        sVEntry<void(void*, void*, float)>* vt = ((sVEntry<void (void*, void*, float)>*)self->elems[i].mVtbl);
        vt[3].fn(vthis(&self->elems[i], vt[3]), self->data + i * 232, self->elems[i].m_emitter.SeedX0);
    }
    self->mbReinitEmitters = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_lookUpBones);
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
 * Looks up the 30 rider bones the spray effect uses and marks the table as filled (mBoneIndiciesValid = 1).
 *
 * Stores cAnimModel_getBoneSkelIndex results for the rider's mAnimModel into the 0x78-byte table mBoneIndicies: shins,
 * feet, thighs, hips, spine, neck, clavicles, biceps, forearms and hands by name ("shinleft_b", "footleft_b", ...).
 * Entries 0-14 and 15-29 repeat the leg, hip and spine bones; entry 12 is "handright_b" between left-arm bones,
 * possibly a slip. cDynamicSprayFX_update calls it while mBoneIndiciesValid is 0.
 */
void cDynamicSprayFX::lookUpBones()
{
    mBoneIndicies[0] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_shinleft_b);
    mBoneIndicies[1] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_footleft_b);
    mBoneIndicies[2] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_shinright_b);
    mBoneIndicies[3] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_footright_b);
    mBoneIndicies[4] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_thighleft_b);
    mBoneIndicies[5] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_thighright_b);
    mBoneIndicies[6] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_hips_b);
    mBoneIndicies[7] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_lowerspine);
    mBoneIndicies[8] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_middlespine);
    mBoneIndicies[9] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_upperspine);
    mBoneIndicies[10] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_neck);
    mBoneIndicies[11] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_clavicleleft);
    mBoneIndicies[12] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_handright_b);
    mBoneIndicies[13] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_bicepleft);
    mBoneIndicies[14] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_biceptwistleft);
    mBoneIndicies[15] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_shinleft_b);
    mBoneIndicies[16] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_footleft_b);
    mBoneIndicies[17] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_shinright_b);
    mBoneIndicies[18] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_footright_b);
    mBoneIndicies[19] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_thighleft_b);
    mBoneIndicies[20] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_thighright_b);
    mBoneIndicies[21] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_hips_b);
    mBoneIndicies[22] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_lowerspine);
    mBoneIndicies[23] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_middlespine);
    mBoneIndicies[24] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_forearmleft_b);
    mBoneIndicies[25] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_handleft_b);
    mBoneIndicies[26] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_clavicleright);
    mBoneIndicies[27] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_bicepright);
    mBoneIndicies[28] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_biceptwistright);
    mBoneIndicies[29] = cAnimModel_getBoneSkelIndex(mRider->mAnimModel, 0, str_forearmright_b);
    int i;
    for (i = 0; i < 30; i++) {
    }
    mBoneIndiciesValid = 1;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_update);
#ifdef SKIP_ASM
struct sWakeFxF920 {
    cRider* obj;                // 0x00
    char pad4[0x24];            // 0x04
    cDynamicColourEmitter* mEmitters; // 0x28
    char pad2C[0x4];            // 0x2C
    t4Vector mBoardUp;      // 0x30
    t4Vector mBoardSide;    // 0x40
    t4Vector mBoardForward; // 0x50
    int mbReinitEmitters;       // 0x60
    char pad64[0x10];           // 0x64
    float mLastSpeed;           // 0x74
    char pad78[0x8];            // 0x78
    int mBoneIndiciesValid;     // 0x80
    char pad84[0xC];            // 0x84
    t4Vector col;           // 0x90
    t4Vector pos;           // 0xA0
    int mbSprayOn;              // 0xB0
    float mEdgeLean;            // 0xB4
    float mSpeed;               // 0xB8
    float mAbsSpeed;            // 0xBC
    t4Vector dir;           // 0xC0
    tSurfData* mpSurfData;      // 0xD0
    int mMotionState;           // 0xD4
    char padD8[0x50];           // 0xD8
    int mLOD;                   // 0x128
    float mLeanSide;            // 0x12C
};

struct sWakeVtF920 {
    short delta;
    short index;
    t4Vector* (*fn)(void*);
};
struct sWakeVtIF920 {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sMtxF920 {
    t4Vector r0, r1, r2, r3;
};

int cRider_getMotionState(void*);
int cRider_getControlState(void*);
int cRiderAnimBase_getCurrentAnimID(void*, int);
extern "C" void cDynamicSprayFX_reinitEmitters_F920(void*) __asm__("cDynamicSprayFX_reinitEmitters");
extern "C" void cDynamicSprayFX_lookUpBones_F920(void*) __asm__("cDynamicSprayFX_lookUpBones");
extern "C" void cDynamicSprayFX_recordBurst_F920(void*, void*, void*, int, int, float) __asm__("cDynamicSprayFX_recordBurst");
extern "C" void func_002E02B8_F920(void*) __asm__("func_002E02B8");
extern "C" void cDynamicSprayFX_updateCarveSpray_F920(void*) __asm__("cDynamicSprayFX_updateCarveSpray");
extern "C" void cDynamicSprayFX_updateBoardSpray_F920(void*) __asm__("cDynamicSprayFX_updateBoardSpray");
extern "C" void cDynamicSprayFX_updateHeadPuff_F920(void*) __asm__("cDynamicSprayFX_updateHeadPuff");
extern "C" void cDynamicSprayFX_updateBurst_F920(void*) __asm__("cDynamicSprayFX_updateBurst");
extern "C" void cDynamicSprayFX_updateSkidSpray_F920(void*) __asm__("cDynamicSprayFX_updateSkidSpray");
extern "C" void cDynamicSprayFX_updateBodyShed_F920(void*) __asm__("cDynamicSprayFX_updateBodyShed");
extern "C" void cDynamicSprayFX_updatePowderTrail_F920(void*) __asm__("cDynamicSprayFX_updatePowderTrail");

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wakeLenF920(const t4Vector& v)
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
static inline void wakeDivEqF920(t4Vector& v, float s)
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
static inline t4Vector wakeScaleF920(const t4Vector& v, float s)
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
static inline t4Vector wakeAddF920(const t4Vector& a, const t4Vector& b)
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

static inline t4Vector wakeRowF920(cRider* o, int k)
{
    // through a pointer to the bone's matrix: retail adds base first (SkelMatrices[b].row[k] puts the index first)
    t4Matrix* bone = &o->mAnimModel->SkelMatrices[o->mBoneBoardChild];
    return bone->row[k];
}

/**
 * Updates a rider's snow spray effect for this frame: samples the rider's state, then runs each enabled emitter's
 * update.
 *
 * Re-initialises the emitters when +0x60 is set (cDynamicSprayFX_reinitEmitters) and looks up the bones while +0x80 is 0
 * (cDynamicSprayFX_lookUpBones). Sets the lean side +0x12C to +-1 from PressMode, the layer-2 animation (23..28, 37, 38) and
 * IsRidingFakie, or 0; +0xB4 = -mSlewed[0].cur * (1 - |mSlewed[3].cur|), negated when riding fakie; +0xB8 the speed
 * (length of kRiderVt_getVelocity's vector), +0xBC its absolute value and +0xC0 the normalised mVelocity.
 *
 * The spray flag +0xB0 is cleared in motion states 1, 2 and 4 (cRider_getMotionState), in control state 9, or when
 * mIsInNIS is set without (mHiddenNIS clear, mNISShowBoardVisualFX and mOnGroundNIS set). +0x30/+0x40/+0x50/+0xA0 get
 * rows 1, 2, 0 and 3 of
 * the mBoneBoardChild matrix; +0xD0 the SurfPropCache record for mSurfaceType. A sharp slow-down (speed drop * 60 above
 * 555.6 at a speed above 555.6, or at least 27.8 in the NIS case) reports a burst through cDynamicSprayFX_recordBurst. The emitters
 * are re-chosen (cDynamicSprayFX_selectEmittersForLOD) when mLOD changed, the colour (+0x90) becomes the doubled table colour for the rider
 * (as in cBoardWakeFX_createSeg) with alpha 1, and every emitter whose m_enabled is set runs its update: 0
 * cDynamicSprayFX_updateBoardSpray, 1 func_002E02B8, 3 cDynamicSprayFX_updateCarveSpray, 4 cDynamicSprayFX_updateHeadPuff, 5 cDynamicSprayFX_updateBurst, 7 cDynamicSprayFX_updateSkidSpray, 8 cDynamicSprayFX_updatePowderTrail,
 * 9 cDynamicSprayFX_updateBodyShed. cRiderHandler_updateRiderFX and cAI_update call it on cRiderHandler + 0xB40.
 */
extern "C" void cDynamicSprayFX_update(sWakeFxF920* self)
{
    if (self->mbReinitEmitters != 0) {
        cDynamicSprayFX_reinitEmitters_F920(self);
    }
    if (self->mBoneIndiciesValid == 0) {
        cDynamicSprayFX_lookUpBones_F920(self);
    }
    self->mLeanSide = 0.0f;
    cRider* obj = self->obj;
    if (obj->PressMode != 0) {
        int animId = cRiderAnimBase_getCurrentAnimID(obj->mRiderAnim, 2);
        if ((animId >= kAID_NoseChangeFakie && animId <= kAID_NoseBalance) || animId == kAID_TailPivotL ||
            animId == kAID_TailPivotR) {
            self->mLeanSide = self->obj->IsRidingFakie ? -1.0f : 1.0f;
        } else {
            self->mLeanSide = self->obj->IsRidingFakie ? 1.0f : -1.0f;
        }
    }
    obj = self->obj;
    self->mEdgeLean = -obj->mSlewed[0].cur * (1.0f - wakeAbsE398(obj->mSlewed[3].cur));
    if (obj->IsRidingFakie != 0) {
        self->mEdgeLean = -self->mEdgeLean;
    }
    char* sub = (char*)&VPTR_AT(self->obj, sizeof(cRiderState));
    sWakeVtF920* vt = *(sWakeVtF920**)sub;
    float sp = wakeLenF920(*vt[2].fn(sub + vt[2].delta));
    self->mSpeed = sp;
    self->mAbsSpeed = wakeAbsE398(sp);
    self->dir = self->obj->mVelocity;
    if (0.0f < sp) {
        wakeDivEqF920(self->dir, wakeLenF920(self->dir));
    }
    self->mbSprayOn = 1;
    int st = cRider_getMotionState(self->obj);
    self->mMotionState = st;
    if (st == MS_Air || st == MS_Wipeout || st == MS_Railslide || cRider_getControlState(self->obj) == CS_Reset) {
        self->mbSprayOn = 0;
    } else {
        cRider* riderObj = self->obj;
        if (riderObj->mIsInNIS != 0) {
            int nisSpray = riderObj->mHiddenNIS == 0 && riderObj->mNISShowBoardVisualFX != 0;
            if (!nisSpray || riderObj->mOnGroundNIS == 0) {
                self->mbSprayOn = 0;
            }
        }
    }
    cRider* riderObj = self->obj;
    self->mBoardForward = wakeRowF920(riderObj, 0);
    float acc = (self->mLastSpeed - self->mSpeed) * 59.999996185302734f;
    self->mBoardUp = wakeRowF920(riderObj, 1);
    self->mBoardSide = wakeRowF920(riderObj, 2);
    self->pos = wakeRowF920(riderObj, 3);
    self->mpSurfData = &SSXApp->Game->SurfPropCache->SurfData[riderObj->mSurfaceType];
    int x = 0;
    if (555.5555419921875f < acc && 555.5555419921875f < self->mSpeed) {
        x = cRider_getControlState(riderObj) != CS_Reset;
    }
    float burstStrength = acc * 0.5f;
    cRider* nisRider = self->obj;
    int y = 0;
    if (nisRider->mIsInNIS != 0 && nisRider->mHiddenNIS == 0) {
        y = nisRider->mNISShowBoardVisualFX != 0;
    }
    if (y) {
        x = 27.77777862548828f <= acc;
        burstStrength = acc;
    }
    if (self->mbSprayOn != 0 && x) {
        t4Vector burstPos = wakeAddF920(self->pos, wakeScaleF920(self->obj->mGroundNormal, 10.0f));
        cDynamicSprayFX_recordBurst_F920(self, &burstPos, &self->obj->mGroundNormal, self->obj->mSurfaceType, 1, burstStrength);
    }
    self->mLastSpeed = self->mSpeed;
    if (self->mLOD != self->obj->mLOD) {
        ((cDynamicSprayFX*)self)->selectEmittersForLOD();
    }
    char* sub2 = (char*)&VPTR_AT(self->obj, sizeof(cRiderState));
    sWakeVtIF920* vt2 = *(sWakeVtIF920**)sub2;
    int idx = vt2[7].fn(sub2 + vt2[7].delta);
    const tARGB* colourRow = &cRenderStateMan_m_state[idx].m_colour;
    t4Vector groundColour;
    groundColour.x = colourRow->R;
    groundColour.y = colourRow->G;
    groundColour.z = colourRow->B;
    groundColour.w = colourRow->A;
    self->col = groundColour;
    wakeScaleE398(self->col, self->col, 2.0f);
    self->col.w = 1.0f;
    if (self->mEmitters[1].m_enabled) func_002E02B8_F920(self);
    if (self->mEmitters[3].m_enabled) cDynamicSprayFX_updateCarveSpray_F920(self);
    if (self->mEmitters[0].m_enabled) cDynamicSprayFX_updateBoardSpray_F920(self);
    if (self->mEmitters[4].m_enabled) cDynamicSprayFX_updateHeadPuff_F920(self);
    if (self->mEmitters[5].m_enabled) cDynamicSprayFX_updateBurst_F920(self);
    if (self->mEmitters[7].m_enabled) cDynamicSprayFX_updateSkidSpray_F920(self);
    if (self->mEmitters[9].m_enabled) cDynamicSprayFX_updateBodyShed_F920(self);
    if (self->mEmitters[8].m_enabled) cDynamicSprayFX_updatePowderTrail_F920(self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_updateCarveSpray);
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
    t4Vector dir;           // 0x30
    char pad40[0x10];           // 0x40
    t4Vector v50;           // 0x50
    char pad60[0x30];           // 0x60
    t4Vector col;           // 0x90
    t4Vector pos;           // 0xA0
    int iB0;                    // 0xB0
    float fB4;                  // 0xB4
    float fB8;                  // 0xB8
    float fBC;                  // 0xBC
    t4Vector vC0;           // 0xC0
    char* pD0;                  // 0xD0
};

struct sWakeVtFE88 {
    short delta;
    short index;
    t4Vector* (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector wakeScaleFE88(const t4Vector& v, float s)
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
static inline t4Vector wakeAddFE88(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector wakeCrossFE88(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float wakeDotFE88(const t4Vector& a, const t4Vector& b)
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
 * emission point is 15 units ahead of the board (+0xA0 plus +0x30 * 15), scattered by cDynamicSprayFX_scatterEmitPosition. The emit chance
 * combines the rider's lean (|mSlewed[3].cur|), the carve value +0xB4 and how far the board faces across its velocity
 * (rider +0x3B0 against +0xC0), scaled down below a speed of about 555. The velocity mixes the rider velocity,
 * mGroundNormal and the board's side direction with settings set 3 (+0x2C, +0x390..+0x39C), the last two scaled by the
 * speed (+0xBC). Otherwise the emitter is updated with nothing to emit. Steps are 1/60 s
 * (cDynamicColourEmitter_update).
 */
extern "C" void cDynamicSprayFX_updateCarveSpray(sWakeFxFE88* self)
{
    void* em = self->p28 + 0x630;
    if (self->iB0 != 0) {
        float lim = *(float*)(self->pD0 + 0x6C);
        if (wakeRandE398() < lim) {
        t4Vector pos = wakeAddFE88(self->pos, wakeScaleFE88(self->dir, 15.0f));
        cDynamicSprayFX_scatterEmitPosition((cDynamicSprayFX*)self, &pos);
        float k = wakeAbsE398(wakeDotFE88(*(t4Vector*)(self->obj + 0x3B0), self->vC0)) * 1.5f;
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
        t4Vector side = wakeCrossFE88(self->v50, *(t4Vector*)(self->obj + 0x370));
        if (self->fB4 < 0.0f) {
            t4Vector n = wakeScaleFE88(side, -1.0f);
            side = n;
        }
        char* sub = self->obj + 0x6C0;
        sWakeVtFE88* vt = *(sWakeVtFE88**)sub;
        t4Vector* riderVel = vt[2].fn(sub + vt[2].delta);
        t4Vector vel = wakeAddFE88(
            wakeAddFE88(
                wakeAddFE88(wakeScaleFE88(*riderVel, *(float*)(self->p2C + 0x390)),
                            wakeScaleFE88(*(t4Vector*)(self->obj + 0x370), *(float*)(self->p2C + 0x394))),
                wakeScaleFE88(wakeScaleFE88(*(t4Vector*)(self->obj + 0x370), *(float*)(self->p2C + 0x398)), self->fBC)),
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
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_updateBoardSpray);
#ifdef SKIP_ASM
struct sWakeFx0EE8 {
    cRider* obj;                // 0x00
    char pad4[0x24];            // 0x04
    cDynamicColourEmitter* em;  // 0x28
    sSprayParams* mSprayData;   // 0x2C
    t4Vector dir;           // 0x30
    char pad40[0x50];           // 0x40
    t4Vector col;           // 0x90
    t4Vector pos;           // 0xA0
    int mbSprayOn;              // 0xB0
    char padB4[0x8];            // 0xB4
    float mAbsSpeed;            // 0xBC
    char padC0[0x10];           // 0xC0
    tSurfData* mpSurfData;      // 0xD0
};

struct sWakeVt0EE8 {
    short delta;
    short index;
    t4Vector* (*fn)(void*);
};

extern "C" void cDynamicColourEmitter_update(void* em, void* pos, void* d, void* vel, int alive, float dt);

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector wakeScale0EE8(const t4Vector& v, float s)
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
static inline t4Vector wakeAdd0EE8(const t4Vector& a, const t4Vector& b)
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

static inline float wakeLimit0EE8()
{
    return 222.22222900390625f;
}

/**
 * Updates spray emitter 0 (+0x28): a spray from the board while moving fast on a spraying surface.
 *
 * Emits when spraying (+0xB0), the speed (+0xBC) is above 222.2, the surface record (+0xD0) flag +0x58 is set and the
 * word at the rider's +0x88C object + 0xA0 is clear. The emission point is 15 units ahead of the board (+0xA0 plus
 * +0x30 * 15), scattered by cDynamicSprayFX_scatterEmitPosition; the velocity mixes the rider velocity and mGroundNormal with settings set 0
 * (+0x2C, +0xD8..+0xE0), the last scaled by the speed. Otherwise the emitter is updated with nothing to emit.
 */
extern "C" void cDynamicSprayFX_updateBoardSpray(sWakeFx0EE8* self)
{
    int flag = *(int*)((char*)self->obj->mAttachedEmitterFX + 0xA0);
    cDynamicColourEmitter* em = self->em;
    if (self->mbSprayOn != 0 && wakeLimit0EE8() < self->mAbsSpeed && self->mpSurfData->ParticleSnowTrailActive != 0 &&
        flag == 0) {
        t4Vector pos = wakeAdd0EE8(self->pos, wakeScale0EE8(self->dir, 15.0f));
        ((cDynamicSprayFX*)self)->scatterEmitPosition(&pos);
        char* sub = (char*)&VPTR_AT(self->obj, sizeof(cRiderState));
        sWakeVt0EE8* vt = *(sWakeVt0EE8**)sub;
        t4Vector* riderVel = vt[2].fn(sub + vt[2].delta);
        t4Vector vel = wakeAdd0EE8(
            wakeAdd0EE8(wakeScale0EE8(*riderVel, self->mSprayData[0].fD8),
                        wakeScale0EE8(self->obj->mGroundNormal, self->mSprayData[0].fDC)),
            wakeScale0EE8(wakeScale0EE8(self->obj->mGroundNormal, self->mSprayData[0].fE0), self->mAbsSpeed));
        cDynamicColourEmitter_update(em, &pos, &vel, &self->col, 1, 0.01666666753590107f);
    } else {
        cDynamicColourEmitter_update(em, &self->pos, 0, 0, 0, 0.01666666753590107f);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_initHeadPuff);
#ifdef SKIP_ASM
extern int D_004A3AFC;

struct sWake10B8 {
    char pad0[0x18];
    int mHeadPuffBlink;
    char pad1C[0x4];
    int mHeadPuffTimer;
    float mHeadPuffPeriod;
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
 * Nothing in the retail code references it; cDynamicSprayFX_updateHeadPuff runs the puffs from these fields.
 */
extern "C" void cDynamicSprayFX_initHeadPuff(sWake10B8* self)
{
    self->mHeadPuffBlink = 1;
    self->mHeadPuffTimer = 0;
    self->mHeadPuffPeriod = wakeRand10B8() * 0.09999999403953552f + 0.4000000059604645f;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_updateHeadPuff);
#ifdef SKIP_ASM
extern "C" void cDynamicColourEmitter_update(void* em, void* pos, void* d, void* vel, int alive, float dt);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void* self, int i);
extern "C" float cRenderStateMan_Temperature(int i);

struct sVec1120 {
    float x, y, z, w;
    sVec1120() {}
    sVec1120(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));


struct sMat1120 {
    t4Vector v[4];
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

extern t4Vector t4vect_Origin_1120 __asm__("t4vect_Origin");

struct sWakeFx1120 {
    cRider* obj;                // 0x00
    char pad4[0x10];            // 0x04
    float rate;                 // 0x14
    int blink;                  // 0x18
    float glow;                 // 0x1C
    float timer;                // 0x20
    float period;               // 0x24
    cDynamicColourEmitter* mEmitters; // 0x28
    sSprayParams* mSprayData;   // 0x2C
    char pad30[0x8C];           // 0x30
    float mAbsSpeed;            // 0xBC
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
extern "C" void cDynamicSprayFX_updateHeadPuff(sWakeFx1120* self)
{
    cDynamicColourEmitter* em = &self->mEmitters[4];
    cRider* obj = self->obj;
    int k = obj->mBoneHead;
    sMat1120* mats = (sMat1120*)obj->mAnimModel->SkelMatrices;
    sMat1120 mat = mats[k];
    float back = -3.0f;
    float up = 4.0f;
    float one = 1.0f;
    sVec1120 pos = wakeMulMat1120(&mat, sVec1120(back, 0.0f, up, one));
    if (1111.111083984375f < self->mAbsSpeed) {
        self->rate = 0.0f;
        cDynamicColourEmitter_update(em, &pos, 0, 0, 0, 0.01666666753590107f);
        return;
    } else {
        float target = 0.0f;
        if (cRiderAnimBase_getCurrentAnimCategory(self->obj->mRiderAnim, 2) == kANCAT_WRecover) {
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
        char* sub = (char*)&VPTR_AT(self->obj, sizeof(cRiderState));
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
                char* sub2 = (char*)&VPTR_AT(self->obj, sizeof(cRiderState));
                sWakeVtV1120* vv = *(sWakeVtV1120**)sub2;
                sVec1120* riderVel = vv[2].fn(sub2 + vv[2].delta);
                sVec1120 vel = wakeAdd1120(
                    wakeAdd1120(wakeScale1120(*riderVel, self->mSprayData[4].fD8),
                                wakeScale1120(*(sVec1120*)&self->obj->mGroundNormal, self->mSprayData[4].fDC)),
                    wakeScale1120(wakeScale1120(*(sVec1120*)&self->obj->mGroundNormal, self->mSprayData[4].fE0),
                                  self->mAbsSpeed));
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
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_updateBurst);
#ifdef SKIP_ASM
struct sWakeFx1598 {
    cRider* obj;                // 0x00
    float mShedAmount;          // 0x04
    char pad8[0x20];            // 0x08
    cDynamicColourEmitter* mEmitters; // 0x28
    char pad2C[0x64];           // 0x2C
    t4Vector col;           // 0x90
    t4Vector pos;           // 0xA0
    char padB0[0x20];           // 0xB0
    tSurfData* mpSurfData;      // 0xD0
    int mMotionState;           // 0xD4
    char padD8[0x8];            // 0xD8
    float mBurstStrength;       // 0xE0
    char padE4[0xC];            // 0xE4
    t4Vector mBurstPos;     // 0xF0
    t4Vector dir;           // 0x100
    char pad110[0x4];           // 0x110
    int mbBurstIsSlowdown;      // 0x114
    char pad118[0x8];           // 0x118
    float mBurstAlpha;          // 0x120
    int mIsPlayer;              // 0x124
};

extern "C" void ReportImpact(void*, float);

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float wakeDot1598(const t4Vector& a, const t4Vector& b)
{
    float r;
    int bits;
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
        : "=f"(r), "=&r"(bits)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector wakeSub1598(const t4Vector& a, const t4Vector& b)
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

struct sWakeVt1598 {
    short delta;
    short index;
    t4Vector* (*fn)(void*);
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

static inline t4Vector* wakeVel1598(sWakeFx1598* self)
{
    char* sub = (char*)&VPTR_AT(self->obj, sizeof(cRiderState));
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
extern "C" void cDynamicSprayFX_updateBurst(sWakeFx1598* self)
{
    tSurfData* cfg = self->mpSurfData;
    if (27.77777862548828f < self->mBurstStrength && cfg->ImpactActive != 0) {
        int emitterIdx = cDynamicSprayFX::kEmitter_SmallBurst;
        int otherIdx = cDynamicSprayFX::kEmitter_BigBurst;
        if (cfg->ImpactLarge != 0 && 277.77777099609375f < self->mBurstStrength) {
            emitterIdx = cDynamicSprayFX::kEmitter_BigBurst;
            otherIdx = cDynamicSprayFX::kEmitter_SmallBurst;
        }
        if (emitterIdx == cDynamicSprayFX::kEmitter_BigBurst && self->obj->mIsInNIS == 0) {
            ReportImpact(&self->mBurstPos, self->mBurstStrength);
        }
        t4Vector pos = self->mBurstPos;
        float k = 1.0f;
        if (emitterIdx == cDynamicSprayFX::kEmitter_SmallBurst) {
            k = 0.5f;
            wakeAddE398(pos, wakeScale0EE8(self->dir, 20.0f));
        } else {
            wakeAddE398(pos, wakeScale0EE8(self->dir, 45.0f));
        }
        float lim = 3333.33349609375f;
        float strengthFrac = wakeClamp1598(self->mBurstStrength / lim, 0.0f, 1.0f);
        float level = self->mShedAmount;
        if (level < 1.0f || emitterIdx == cDynamicSprayFX::kEmitter_BigBurst) {
            if (self->mMotionState == MS_Wipeout) {
                if (level < 2.0f) {
                    float n = level + (strengthFrac + strengthFrac) * k;
                    self->mShedAmount = wakeMin1598(n, 2.0f);
                }
            } else if (self->mbBurstIsSlowdown == 0) {
                if (level < 1.5f) {
                    self->mShedAmount = (level + strengthFrac * (k * 1.5f)) <? 2.0f;
                }
            } else {
                if (level < 1.0f) {
                    self->mShedAmount = (level + strengthFrac * k) <? 2.0f;
                }
            }
        }
        float j = 25.0f;
        if (cfg->ImpactLarge != 0 && self->mIsPlayer != 0) {
            j = 60.0f;
        }
        float lo = -j;
        {
            t4Vector jitter;
            jitter.z = 0.0f;
            jitter.w = 0.0f;
            jitter.x = lo + (j - lo) * wakeRandE398();
            jitter.y = lo + (j - lo) * wakeRandE398();
            wakeAddE398(pos, jitter);
        }
        t4Vector* va = wakeVel1598(self);
        t4Vector* vb = wakeVel1598(self);
        t4Vector* up = &self->obj->mGroundNormal;
        t4Vector flatVel = wakeSub1598(*va, wakeScale0EE8(*up, wakeDot1598(*vb, *up) * 0.75f));
        float velScale = 0.30000001192092896f;
        if (self->mbBurstIsSlowdown == 0) velScale = 0.4000000059604645f;
        t4Vector vel = wakeScale0EE8(flatVel, velScale);
        float al = 0.5f;
        if (self->mbBurstIsSlowdown == 0) al = 1.0f;
        if (self->mBurstAlpha < al) {
            self->mBurstAlpha = al;
        } else {
            self->mBurstAlpha = self->mBurstAlpha * 0.800000011920929f + al * 0.19999998807907104f;
        }
        t4Vector col = self->col;
        col.w = self->mBurstAlpha;
        cDynamicColourEmitter_update(&self->mEmitters[emitterIdx], &pos, &vel, &col, 1, 0.01666666753590107f);
        cDynamicColourEmitter_update(&self->mEmitters[otherIdx], &self->mBurstPos, 0, 0, 0, 0.01666666753590107f);
        if (self->mBurstStrength < 83.33333587646484f) {
            self->mBurstStrength = 0.0f;
        } else {
            float decay = cfg->ImpactMultiplier;
            if (self->mIsPlayer == 0) {
                decay *= 0.4000000059604645f;
            }
            self->mBurstStrength = self->mBurstStrength * decay;
        }
    } else {
        cDynamicColourEmitter_update(&self->mEmitters[6], &self->pos, 0, 0, 0, 0.01666666753590107f);
        cDynamicColourEmitter_update(&self->mEmitters[5], &self->pos, 0, 0, 0, 0.01666666753590107f);
        self->mBurstStrength = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_updateSkidSpray);
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
    cRider* obj;                // 0x00
    char pad4[0x24];            // 0x04
    cDynamicColourEmitter* em;  // 0x28
    sSprayParams* mSprayData;   // 0x2C
    char pad30[0x20];           // 0x30
    t4Vector mBoardForward;     // 0x50
    char pad60[0x30];           // 0x60
    t4Vector col;               // 0x90
    t4Vector pos;               // 0xA0
    int mbSprayOn;              // 0xB0
    float mEdgeLean;            // 0xB4
    float mSpeed;               // 0xB8
    float mAbsSpeed;            // 0xBC
    t4Vector mVelocityDir;      // 0xC0
    tSurfData* mpSurfData;      // 0xD0
    char padD4[0x58];           // 0xD4
    float mLeanSide;            // 0x12C
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
    char* sub = (char*)&VPTR_AT(self->obj, sizeof(cRiderState));
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
extern "C" void cDynamicSprayFX_updateSkidSpray(sWakeFx1A80* self)
{
    float facing = fsAbs(vu0Dot_1A80(self->obj->mGroundSide, self->mVelocityDir));
    facing = facing + facing;
    cDynamicColourEmitter* em = &self->em[7];
    float k = 1.0f;
    if (facing <= 1.0f)
        k = facing;
    float ak = fsAbs(k);
    float speedRamp = wakeRamp1A80(416.66668701171875f, 1250.0f, self->mSpeed);
    float leanSum = fsAbs(self->obj->mSlewed[0].cur) + ak - 0.10000000149011612f;
    float rampSq = 1.0f;
    if (speedRamp < 1.0f)
        rampSq = speedRamp;
    rampSq *= rampSq;
    float thr = leanSum * rampSq;
    float roll = wakeRand() * 0.4000000059604645f;
    if (self->mbSprayOn != 0 && roll <= thr && self->mpSurfData->CloudySprayActive != 0 &&
        fsAbs(self->mLeanSide) < 0.20000000298023224f) {
        t4Vector pos = self->pos;
        float spd = self->mpSurfData->CloudySprayMaxHeight;
        float lift = wakeRand() + 0.5f;
        vu0AddEq_1A80(pos, vu0Sub_1A80(vu0Scale_1A80(self->obj->mGroundNormal, lift * spd),
                             vu0Scale_1A80(vu0Scale_1A80(vu0Scale_1A80(self->obj->mVelocity, 3.0f),
                                                         0.01666666753590107f), lift)));
        t4Vector side = wakeCross1A80(self->mBoardForward, self->obj->mGroundNormal);
        if (self->mEdgeLean < 0.0f)
            side = vu0Scale_1A80(side, -1.0f);
        t4Vector n = self->obj->IsRidingFakie != 0 ? vu0Scale_1A80(self->mBoardForward, -1.0f) : self->mBoardForward;
        t4Vector boardBack = vu0Scale_1A80(vu0Scale_1A80(n, -1.0f), self->mSpeed);
        t4Vector negVel = vu0Scale_1A80(self->obj->mVelocity, -1.0f);
        t4Vector w = vu0Scale_1A80(vu0Sub_1A80(boardBack, negVel), 0.30000001192092896f);
        t4Vector* riderVel = wakeVel1A80(self);
        t4Vector vel = vu0Add(
            vu0Add(vu0Add(vu0Add(vu0Scale_1A80(*riderVel, self->mSprayData[7].fD8),
                                 vu0Scale_1A80(vu0Scale_1A80(side, self->mSpeed), 0.0f)),
                          vu0Scale_1A80(self->obj->mGroundNormal, self->mSprayData[7].fDC)),
                   vu0Scale_1A80(vu0Scale_1A80(self->obj->mGroundNormal, self->mSprayData[7].fE0), self->mAbsSpeed)),
            w);
        cDynamicColourEmitter_update(em, &pos, &vel, &self->col, 1, 0.01666666753590107f);
    } else {
        cDynamicColourEmitter_update(em, &self->pos, 0, 0, 0, 0.01666666753590107f);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_updatePowderTrail);
#ifdef SKIP_ASM
struct sWakeFx1F70 {
    cRider* obj;                // 0x00
    char pad4[0xC];             // 0x04
    float mPowderLevel;         // 0x10
    char pad14[0x14];           // 0x14
    cDynamicColourEmitter* mEmitters; // 0x28
    sSprayParams* mSprayData;   // 0x2C
    t4Vector dir;           // 0x30
    char pad40[0x50];           // 0x40
    t4Vector col;           // 0x90
    t4Vector pos;           // 0xA0
    int mbSprayOn;              // 0xB0
    char padB4[0x8];            // 0xB4
    float mAbsSpeed;            // 0xBC
    char padC0[0x10];           // 0xC0
    tSurfData* mpSurfData;      // 0xD0
};

struct sWakeVt1F70 {
    short delta;
    short index;
    t4Vector* (*fn)(void*);
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

static inline t4Vector* wakeVel1F70(sWakeFx1F70* self)
{
    char* sub = (char*)&VPTR_AT(self->obj, sizeof(cRiderState));
    sWakeVt1F70* vt = *(sWakeVt1F70**)sub;
    return vt[2].fn(vthis(sub, vt[2]));
}

/**
 * Updates spray emitter 8 (+0x28 + 0x1080): a steady powder trail whose level follows the surface.
 *
 * While spraying (+0xB0) with a surface record level (+0xD0, +0x5C) of at least 0.01 the level +0x10 rises by 0.01 a
 * frame up to that value and nothing is emitted that frame; otherwise it falls by 1/60. It drops to 0 when the speed
 * (+0xBC) is below 277.8 or, not spraying, the velocity's z is below -3333.3. With a positive level it emits at the
 * board (+0xA0, scattered by cDynamicSprayFX_scatterEmitPosition) with alpha level * 1.25 (faded as the rider falls), velocity from the rider
 * velocity and +0x30 with settings set 8 (+0x2C, +0x818..+0x820).
 */
// PORT: g++ minimum/maximum operators (<?, >?).
extern "C" void cDynamicSprayFX_updatePowderTrail(sWakeFx1F70* self)
{
    int done = 0;
    cDynamicColourEmitter* em = &self->mEmitters[8];
    if (self->mbSprayOn == 0 || self->mpSurfData->SnowBuildupAmount < 0.009999999776482582f) {
        self->mPowderLevel = (self->mPowderLevel - 0.01666666753590107f) >? 0.0f;
    } else {
        done = 1;
        self->mPowderLevel = (self->mPowderLevel + 0.010000000707805157f) <? self->mpSurfData->SnowBuildupAmount;
    }
    if ((self->mbSprayOn == 0 && wakeVel1F70(self)->z < -3333.33349609375f) || self->mAbsSpeed < wakeLimit1F70()) {
        self->mPowderLevel = 0.0f;
    }
    t4Vector pos = self->pos;
    if (!done && 0.0f < self->mPowderLevel) {
        float alpha = wakeClamp1F70(self->mPowderLevel * 1.25f, 0.0f, 1.0f);
        if (wakeVel1F70(self)->z < 0.0f) {
            alpha *= wakeClamp1F70((wakeVel1F70(self)->z + 3333.33349609375f) / 3333.33349609375f, 0.0f, 1.0f);
        }
        t4Vector col = self->col;
        col.w = alpha;
        t4Vector* riderVel = wakeVel1F70(self);
        t4Vector vel = wakeAdd0EE8(
            wakeAdd0EE8(wakeScale0EE8(*riderVel, self->mSprayData[8].fD8),
                        wakeScale0EE8(self->dir, self->mSprayData[8].fDC)),
            wakeScale0EE8(wakeScale0EE8(self->dir, self->mSprayData[8].fE0), self->mAbsSpeed));
        ((cDynamicSprayFX*)self)->scatterEmitPosition(&pos);
        cDynamicColourEmitter_update(em, &pos, &vel, &col, 1, 0.01666666753590107f);
    } else {
        cDynamicColourEmitter_update(em, &self->pos, 0, 0, 0, 0.01666666753590107f);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_updateBodyShed);
#ifdef SKIP_ASM

struct sWakeFx2260 {
    cRider* obj;                // 0x00
    float mShedAmount;          // 0x04
    char pad8[0x20];            // 0x08
    cDynamicColourEmitter* mEmitters; // 0x28
    sSprayParams* mSprayData;   // 0x2C
    char pad30[0x48];           // 0x30
    int mShedBone;              // 0x78
    int* mBoneIndicies;         // 0x7C
    char pad80[0x10];           // 0x80
    t4Vector col;           // 0x90
    char padA0[0x18];           // 0xA0
    float mSpeed;               // 0xB8
    char padBC[0x18];           // 0xBC
    int mMotionState;           // 0xD4
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
extern "C" void cDynamicSprayFX_updateBodyShed(sWakeFx2260* self)
{
    float zero = 0.0f;
    cDynamicColourEmitter* em = &self->mEmitters[9];
    if (zero < self->mShedAmount && wakeLimit2260() < self->mSpeed) {
        float dt;
        float alpha;
        t4Vector col;
        t4Vector pos;
        t4Vector vel;
        t4Vector scaledVel;
        cRider* obj;
        t4Matrix* boneMats;
        int k;
        alpha = wakeClamp2260(self->mShedAmount * 1.5f, zero, 1.0f);
        col = self->col;
        col.w = alpha;
        dt = 0.01666666753590107f;
        obj = self->obj;
        k = self->mBoneIndicies[self->mShedBone];
        boneMats = obj->mAnimModel->SkelMatrices;
        pos = ((t4Vector*)&((t4Matrix*)((char*)boneMats + (k << 6)))->row[0])[3];
        wakeScaleE398(scaledVel, obj->mVelocity, self->mSprayData[9].fD8);
        vel = scaledVel;
        cDynamicColourEmitter_update(em, &pos, &vel, &col, 1, dt);
        self->mShedBone++;
        if (self->mShedBone >= 30) {
            self->mShedBone = 0;
        }
        if (self->mMotionState != MS_Wipeout) {
            self->mShedAmount = (self->mShedAmount - dt) >? zero;
        }
    } else {
        cRider* obj = self->obj;
        int k = obj->mBoneHips;
        t4Matrix* boneMats = obj->mAnimModel->SkelMatrices;
        t4Vector pos = ((t4Vector*)&((t4Matrix*)((char*)boneMats + (k << 6)))->row[0])[3];
        cDynamicColourEmitter_update(em, &pos, 0, 0, 0, 0.01666666753590107f);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_recordBurst);
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

static inline int Carving_23E0(cRider* r)
{
    return r->mIsInNIS != 0 && r->mHiddenNIS == 0 && r->mNISShowBoardVisualFX != 0;
}

struct sWake_23E0 {
    cRider* rider;              // 0x00
    char pad4[0xDC];
    float mBurstStrength;       // 0xE0 (cDynamicSprayFX's names)
    char padE4[0xC];
    t4Vector mBurstPos;         // 0xF0
    t4Vector mBurstDir;         // 0x100
    int mBurstSurface;          // 0x110
    int mbBurstIsSlowdown;      // 0x114
    char pad118[0xC];
    int mIsPlayer;              // 0x124
};

/**
 * Records a spray burst when it is stronger than the current one and far enough from it.
 *
 * Takes it when |@p speed| exceeds the stored strength (+0xE0) and the point is more than 180 units (100 for a player,
 * +0x124) from the stored point (+0xF0), or the rider is in the NIS state that allows a trail (mIsInNIS set,
 * mHiddenNIS clear, mNISShowBoardVisualFX and mOnGroundNIS set). It then stores the strength (doubled when
 * @p isSlowdown is 0), @p pos (+0xF0), @p dir
 * (+0x100), @p surfaceType (+0x110) and @p isSlowdown (+0x114). cDynamicSprayFX_update reports slow-downs with @p isSlowdown = 1; cRiderHandler_recordSprayBurst (wipeout
 * impacts) passes 0. cDynamicSprayFX_updateBurst plays the burst.
 *
 * @param pos  the burst position
 * @param dir  the burst direction (the surface normal)
 * @param surfaceType  the surface type
 * @param isSlowdown  1 for a slow-down spray, 0 for an impact
 * @param speed  the burst strength
 */
extern "C" void cDynamicSprayFX_recordBurst(sWake_23E0* self, t4Vector* pos, t4Vector* dir, int surfaceType, int isSlowdown, float speed)
{
    float absSpeed = Abs_23E0(speed);
    float distance = Abs_23E0(Length_23E0(Sub_23E0(self->mBurstPos, *pos)));
    float maxd = 180.0f;
    if (self->mIsPlayer != 0)
        maxd = 100.0f;
    if (self->mBurstStrength < absSpeed) {
        if (maxd < distance || (Carving_23E0(self->rider) && self->rider->mOnGroundNIS != 0)) {
            if (isSlowdown == 0)
                self->mBurstStrength = absSpeed + absSpeed;
            else
                self->mBurstStrength = absSpeed;
            self->mBurstPos = *pos;
            self->mBurstDir = *dir;
            self->mBurstSurface = surfaceType;
            self->mbBurstIsSlowdown = isSlowdown;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_render);
#ifdef SKIP_ASM
extern "C" void cDynamicColourEmitter_render(void* self, int mode);


struct cDynamicSprayFX_render_sFx {
    cRider* owner;                  // 0x0
    char pad_0x04[0x24];
    cDynamicColourEmitter* wakes;     // 0x28
};

/**
 * Draws a rider's snow spray: renders each enabled emitter that has live particles, when the rider is on screen.
 *
 * Does nothing unless the rider's mIsOnScreen is set. Of the ten emitters (+0x28) it renders those whose m_enabled is
 * set and whose m_enabledCount is positive, with cDynamicColourEmitter_render mode 7.
 * cRiderHandler_renderRiderFX and cAI_render call it on cRiderHandler + 0xB40.
 */
extern "C" void cDynamicSprayFX_render(cDynamicSprayFX_render_sFx* self)
{
    if (self->owner->mIsOnScreen != 0) {
        int i;
        for (i = 0; i < 10; i++) {
            cDynamicColourEmitter* w = &self->wakes[i];
            if (w->m_enabled != 0 && w->m_enabledCount > 0) {
                cDynamicColourEmitter_render(w, 7);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_selectEmittersForLOD);
#ifdef SKIP_ASM
/**
 * Chooses which spray emitters run for the rider's detail level (mLOD), which it also stores in its own mLOD.
 *
 * Sets m_enabled of each of the ten mEmitters. Level 0: emitters 0, 5 and 6 on, 4 on for a player (mIsPlayer) or in
 * an NIS (mIsInNIS), 1, 2, 3, 7, 8 and 9 on for a player. Level 1: 0, 5 and 6 on, 1, 2, 3, 7 and 8 for a player, 4 and
 * 9 off. Other levels: 5 on, 6 for a player, the rest off. cDynamicSprayFX_reset and cDynamicSprayFX_update (when mLOD
 * changes) call it.
 */
void cDynamicSprayFX::selectEmittersForLOD()
{
    int mode = mRider->mLOD;
    mLOD = mode;
    switch (mode) {
    case cRider::kLOD_Near:
        mEmitters[5].m_enabled = 1;
        mEmitters[6].m_enabled = 1;
        mEmitters[0].m_enabled = 1;
        {
            int enabled = 0;
            if (mIsPlayer != 0 || mRider->mIsInNIS != 0)
                enabled = 1;
            mEmitters[4].m_enabled = enabled;
        }
        mEmitters[1].m_enabled = mIsPlayer;
        mEmitters[2].m_enabled = mIsPlayer;
        mEmitters[3].m_enabled = mIsPlayer;
        mEmitters[7].m_enabled = mIsPlayer;
        mEmitters[8].m_enabled = mIsPlayer;
        mEmitters[9].m_enabled = mIsPlayer;
        break;
    case cRider::kLOD_Mid:
        mEmitters[5].m_enabled = 1;
        mEmitters[6].m_enabled = 1;
        mEmitters[0].m_enabled = 1;
        mEmitters[1].m_enabled = mIsPlayer;
        mEmitters[2].m_enabled = mIsPlayer;
        mEmitters[3].m_enabled = mIsPlayer;
        mEmitters[7].m_enabled = mIsPlayer;
        mEmitters[8].m_enabled = mIsPlayer;
        mEmitters[4].m_enabled = 0;
        mEmitters[9].m_enabled = 0;
        break;
    default:
        mEmitters[5].m_enabled = 1;
        mEmitters[6].m_enabled = mIsPlayer;
        mEmitters[1].m_enabled = 0;
        mEmitters[0].m_enabled = 0;
        mEmitters[2].m_enabled = 0;
        mEmitters[3].m_enabled = 0;
        mEmitters[7].m_enabled = 0;
        mEmitters[4].m_enabled = 0;
        mEmitters[8].m_enabled = 0;
        mEmitters[9].m_enabled = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cDynamicSprayFX_tuneBurstEmitters);
#ifdef SKIP_ASM
struct sWakeVt26B8 { short delta; short index; void (*fn)(void*, void*, float); };

/**
 * Tunes the burst emitters 5 and 6 for a player or a computer rider and re-initialises them.
 *
 * Stores whether the rider has a view (mViewIndex >= 0) in mIsPlayer. Sets settings sets 5 and 6 (+0x2C, 0xE8 bytes
 * each) at +0x10, +0x18 and +0xCC to 55, 25, 170 and 30, 20, 60 for a player, or 24.75, 5, 76.5 and 19.5, 8, 39
 * otherwise, then re-initialises emitters 5 and 6 through their virtual slot 3 with their settings and their
 * m_emitter.SeedX0. The
 * cDynamicSprayFX constructor and cDynamicSprayFX_reset call it.
 *
 */
void cDynamicSprayFX::tuneBurstEmitters()
{
    int on = mRider->mViewIndex >= 0;
    mIsPlayer = on;
    if (on) {
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x498) = 55.0f;
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x4A0) = 25.0f;
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x554) = 170.0f;
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x580) = 30.0f;
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x588) = 20.0f;
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x63C) = 60.0f;
    } else {
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x498) = 24.75f;
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x4A0) = 5.0f;
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x554) = 76.5f;
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x580) = 19.5f;
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x588) = 8.0f;
        *(float*)(*(char**)(((char*)this) + 0x2C) + 0x63C) = 39.0f;
    }
    {
        cDynamicColourEmitter* emitters = mEmitters;
        cDynamicColourEmitter* emitter = &emitters[5];
        sWakeVt26B8* vt = (sWakeVt26B8*)emitters[5].mVtbl;
        vt[3].fn(vthis(emitter, vt[3]), *(char**)(((char*)this) + 0x2C) + 0x488, emitters[5].m_emitter.SeedX0);
    }
    {
        cDynamicColourEmitter* emitters = mEmitters;
        cDynamicColourEmitter* emitter = &emitters[6];
        sWakeVt26B8* vt = (sWakeVt26B8*)emitters[6].mVtbl;
        vt[3].fn(vthis(emitter, vt[3]), *(char**)(((char*)this) + 0x2C) + 0x570, emitters[6].m_emitter.SeedX0);
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
    case tHaloRecord::kHaloType_Medium:
        return 180.0f;
    case tHaloRecord::kHaloType_Small:
        return 100.0f;
    case tHaloRecord::kHaloType_Large:
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
void* cHalo_cHalo(void* vself)
{
    cHalo* self = (cHalo*)vself;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLightHalo_Render);
#ifdef SKIP_ASM
// t4Vector with the On Tour constructors the halo temporaries below use (t4Vector(float, float, float, float),
// t4Vector(const t3vect&, float)).
struct t4VectorInit : t4Vector {
    t4VectorInit(const float& ax, const float& ay, const float& az, const float& aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
    t4VectorInit(const t3Vector& v, const float& aw) // [O] t4Vector(const t3vect&, float); cLightHalo_Process
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = aw;
    }
};

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_2868(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// Sets one field of the current draw state's second state word.
static inline void rsSetA_2868(cPS2GraphicsMan* ctx, int mask, int shift, int v)
{
    ctx->m_pCurrentDrawState->m_bitGroups[1] = ((int)ctx->m_pCurrentDrawState->m_bitGroups[1] & ~mask)
        | ((v << shift) & mask);
}

extern "C" void cLightHalo_Render(cLightHalo* self, int idx);

/**
 * Draws one light halo as a camera-facing sprite in the halo's colour.
 *
 * Does nothing unless the halo is on screen (+0x2C). The colour's red, green and blue are the halo record's (+0x20)
 * vector at +0x10 normalised to unit length. With a z-buffer sample (+0x2E) the alpha is the record's visibility for
 * view @p idx (+0x40 + 4 * @p idx) and the render state's flag A bits 23-24 are set to 2, otherwise the alpha is 1 with
 * bits 23-24 = 1; bits 20-21 are cleared and bits 12-19 set to 20 either way. Skips a non-positive alpha. The halo type
 * (record flags & 0x70) picks the texture and size: 0x10 effect texture 53 at 180, 0x20 effect texture 54 at 100,
 * 0x40 effect texture 53 at 350. Draws at the record's position (+0x1C) with UVs 0..1 and rotation +0x28 through GrMan
 * virtual slot 79 (DrawRotatedSprite). cLensFxMan_DrawLens calls it for every halo on screen.
 *
 * @param idx  the view index
 */
void cLightHalo::Render(int idx)
{
    if (mbOnScreen == 0) {
        return;
    }
    t3Vector n;
    tHaloRecord* haloRecord = mpHalo;
    float x = haloRecord->m_fvColor[0];
    float y = haloRecord->m_fvColor[1];
    float z = haloRecord->m_fvColor[2];
    float len = Sqrt_2868(x * x + y * y + z * z);
    if (len != 0.0f) {
        float inv = 1.0f / len;
        n.x = x * inv;
        n.y = y * inv;
        n.z = z * inv;
    } else {
        n.x = haloRecord->m_fvColor[0];
        n.y = haloRecord->m_fvColor[1];
        n.z = haloRecord->m_fvColor[2];
    }
    tARGB col;
    col.R = n.x;
    col.G = n.y;
    col.B = n.z;
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    if (mbRequestZSample != 0) {
        col.A = mpHalo->m_fOcclude[idx];
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
    switch ((unsigned int)(((int)(mpHalo)->m_iFlags) & 0x70)) {
    case tHaloRecord::kHaloType_Medium:
        *ctx->m_pCurrentDrawState->m_textureState.m_texid = ctx->getEffectTexMap(53);
        size = 180.0f;
        break;
    case tHaloRecord::kHaloType_Small:
        *ctx->m_pCurrentDrawState->m_textureState.m_texid = ctx->getEffectTexMap(54);
        size = 100.0f;
        break;
    case tHaloRecord::kHaloType_Large:
        *ctx->m_pCurrentDrawState->m_textureState.m_texid = ctx->getEffectTexMap(53);
        size = 350.0f;
        break;
    }
    tHaloRecord* drawRecord = mpHalo;
    ctx->DrawRotatedSprite(t4VectorInit(drawRecord->m_fvWorldPos[0],
                                        drawRecord->m_fvWorldPos[1],
                                        drawRecord->m_fvWorldPos[2], 1.0f),
                           t2Vector(size, size), t2Vector(0.0f, 0.0f), t2Vector(1.0f, 1.0f), col, mAngle, 2);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLightHalo_Process);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char* cGraphicsMan_TheGraphicsMan_boardwakefx __asm__("cGraphicsMan_TheGraphicsMan");

// t3Vector with On Tour's t3Vector(float, float, float) constructor.
struct t3VectorInit : t3Vector {
    t3VectorInit(const float& ax, const float& ay, const float& az)
    {
        x = ax;
        y = ay;
        z = az;
    }
};


// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_2B00(const t4Vector& a, const t4Vector& b)
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
 * on the point) and the 16 by 8 rectangle +0x10..+0x1C clamped inside the view rectangle (mViewRect). +0x24 becomes
 * the projected depth of the point moved toward the camera by 100, 80 or 200 units by type (along the view matrix's
 * z column, mViewMat rows 0..2 .z), and +0x28 the angle (screen x mapped to -1..1) * pi/2. cLensFxMan_ProcessLens
 * calls it for each halo of the view.
 *
 * @param viewInfo  the view information ([O] tViewInfo): its view rectangle and view matrix
 */
void cLightHalo::Process(tViewInfo* viewInfo)
{
    cLightHalo* self = this;
    cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
    t4Vector pos;
    pos.x = self->mpHalo->m_fvWorldPos[0];
    pos.y = self->mpHalo->m_fvWorldPos[1];
    pos.z = self->mpHalo->m_fvWorldPos[2];
    pos.w = 1.0f;
    if (obj->GetOutCode(pos)) {
        self->mbOnScreen = 0;
        self->mbRequestZSample = 0;
    } else {
        self->mbOnScreen = 1;
        t4Vector scr = obj->GetScreenCoord(pos);
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
            self->mZSRect.X = wakeClampD0B8(self->mZSRect.X, 0.0f,
                (viewInfo->mViewRect.X + viewInfo->mViewRect.W) - self->mZSRect.W);
            self->mZSRect.Y = wakeClampD0B8(self->mZSRect.Y, 0.0f,
                (viewInfo->mViewRect.Y + viewInfo->mViewRect.H) - self->mZSRect.H);
            float pullBack = 0.0f;
            switch ((unsigned int)(((int)(self->mpHalo)->m_iFlags) & 0x70)) {
            case tHaloRecord::kHaloType_Medium:
                pullBack = 100.0f;
                break;
            case tHaloRecord::kHaloType_Small:
                pullBack = 80.0f;
                break;
            case tHaloRecord::kHaloType_Large:
                pullBack = 200.0f;
                break;
            }
            t3Vector viewDir;
            viewDir.x = viewInfo->mViewMat.row[0].z;
            viewDir.y = viewInfo->mViewMat.row[1].z;
            viewDir.z = viewInfo->mViewMat.row[2].z;
            t4Vector lp = Sub_2B00(pos, t4VectorInit(t3VectorInit(viewDir.x * pullBack, viewDir.y * pullBack,
                                                              viewDir.z * pullBack), 0.0f));
            t4Vector sc = obj->GetScreenCoord(lp);
            self->mZDist = sc.z;
            // screen x mapped to -1..1, then to an angle
            float screenFrac = scr.x - viewInfo->mViewRect.X;
            screenFrac = (screenFrac + screenFrac) / viewInfo->mViewRect.W - 1.0f;
            self->mAngle = screenFrac * 1.5707964f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan_cLensFxMan);
#ifdef SKIP_ASM

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
 * It builds the 2 x 256 halos mLightHalo (cHalo_cHalo) and the two suns mSunHalo (cSunHalo_cSunHalo), sets
 * mbLocked to 1 and clears both views' mNumHalo. It then passes cLensFxMan__drawFrameCallBack
 * and itself to graphics manager slot 117 and sets cLensFxMan_mbDrawSun. The PS2 version's constructor
 * (cPS2LensFxMan_cPS2LensFxMan) calls this.
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
        // both views' halo counts, cleared a pair at a time (retail's one-pass loop keeps two strided stores)
        int* firstViewCount = &self->mNumHalo[0];
        int* secondViewCount = &self->mNumHalo[1];
        int i;
        for (i = 0; i < 1; i++) {
            firstViewCount[i * 2] = 0;
            secondViewCount[i * 2] = 0;
        }
    }
    {
        cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
        obj->SetLensFXCallBack((void*)cLensFxMan__drawFrameCallBack, (void*)self);
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
void cLensFxMan::Reset(int i)
{
    mNumHalo[i] = 0;
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
void cLensFxMan::AddLightHalos(int* vals, int count, int ignored, int i)
{
    int n;
    for (n = 0; n < count; n++) {
        cLensFxMan_AddHalo(((sWakeFx*)this), vals[n], i);
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cLensFxMan__cLensFxMan);
#ifdef SKIP_ASM
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
    cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
    self->mVtbl = cLensFxMan_vtbl;
    obj->SetLensFXCallBack(0, (void*)(0));
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
void cLensFxMan::render(int i)
{
    ProcessLens(i);
    cLensFxMan_DrawLens(this, i);
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
void cLensFxMan__drawFrameCallBack(void* vself)
{
    cLensFxMan* self = (cLensFxMan*)vself;
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
extern "C" void cLightHalo_Process(cLightHalo* part, tViewInfo* viewInfo);
extern "C" void cSunHalo_Process(void* emit, void* owner, int idx);

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void vu0CopyMatrix3338(t4Matrix* dst, t4Matrix* src)
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
 * Fills mViewInfo.mViewRect from GrMan virtual slot 28 (the view rectangle), copies the matrix from slot 35 to
 * mViewInfo.mViewMat and gets mFOV, mZNear and mZFar from slot 30. Then processes view @p idx's sun (mSunHalo[@p idx])
 * with cSunHalo_Process and each of its halos with cLightHalo_Process. cLensFxMan_render calls it.
 *
 * @param idx  the view index
 */
void cLensFxMan::ProcessLens(int idx)
{
    cLensFxMan* self = this;
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    tRect viewRect = ctx->GetViewportWindow();
    self->mViewInfo.mViewRect = viewRect;
    {
        vu0CopyMatrix3338(&self->mViewInfo.mViewMat, (t4Matrix*)ctx->GetView());
    }
    int i = 0;
    {
        ctx->GetViewportPerspective(self->mViewInfo.mFOV, self->mViewInfo.mZNear, self->mViewInfo.mZFar);
    }
    cSunHalo* suns = self->mSunHalo;
    cSunHalo_Process(&suns[idx], self, idx);
    if (i < self->mNumHalo[idx]) {
        cLightHalo (*viewHalos)[256] = self->mLightHalo;
        cLightHalo* halo = viewHalos[idx];
        do {
            halo->Process(&self->mViewInfo);
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
 * Pops one 0x14-byte entry off the draw-state stack (GrMan->m_pCurrentDrawState).
 */
extern "C" void cLensFxMan_endDrawFrame(void)
{
    GrMan->m_pCurrentDrawState--;
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
        (*(char**)&self->mRider) = (char*)self + 0x40;
        cRider_cRider((char*)self + 0x40);
    }
    self->mRider->mScriptObjectVtbl = (void*)&D_004889C8;
    self->mRider->mScriptObjRiderVtbl = D_00459B90;
    VPTR_AT(self->mRider, sizeof(cRiderState)) = (void**)&D_00488A10;
    if (inChrg == 0) {
        int vc;
        vtbl1 = D_004889C8;
        self->mRider->mScriptObjectVtbl = &vtbl1;
        {
            char* vbo = ((char*)self->mRider) - 0x40;
            vc = (char*)self - vbo;
        }
        vtbl1.e[1].delta = D_004889C8.e[1].delta + vc;
        vtbl2 = D_00488A10;
        VPTR_AT(self->mRider, sizeof(cRiderState)) = (void**)&vtbl2;
        vtbl2.e[1].delta = D_00488A10.e[1].delta + vc;
    }
    self->reset();
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
// tARGB with On Tour's tARGB(float a, float r, float g, float b) constructor.
struct tARGBInit : tARGB {
    tARGBInit(float a, float r, float g, float b)
    {
        A = a;
        R = r;
        G = g;
        B = b;
    }
};

/**
 * Resets a player indicator's colours; despite the name it reads no replay data (it takes no frame).
 *
 * The pulse runs between (0.5, 1, 0, 0) and (0.7, 1, 0.7, 0.7), starting at the first; PulseAng is cleared.
 */
void cPlayerIndicatorFX::reset()
{
    MinColor = tARGBInit(0.5f, 1.0f, 0.0f, 0.0f);
    MaxColor = tARGBInit(0.699999988079071f, 1.0f, 0.699999988079071f, 0.699999988079071f);
    CurColor = MinColor;
    PulseAng = 0.0f;
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
void cPlayerIndicatorFX::update()
{
    if (mRider->isHuman() == 0) return;
    float half = 0.5f;
    float newPhase = PulseAng + kBeamPulseRate * 0.01666666753590107f;
    float turns = newPhase * 0.15915493667125702f + half;
    float wholeTurns;
    // PORT: FPU-only float->int->float round trip (floor helper); no C cast reproduces it.
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(wholeTurns) : "f"(turns));
    if (turns < wholeTurns) wholeTurns -= 1.0f;
    float wrapped = newPhase - wholeTurns * 6.2831854820251465f;
    PulseAng = wrapped;
    float blend = fCos(wrapped) * half + half;
    float invBlend = 1.0f - blend;
    CurColor.A = blend * MinColor.A + invBlend * MaxColor.A;
    CurColor.R = blend * MinColor.R + invBlend * MaxColor.R;
    CurColor.G = blend * MinColor.G + invBlend * MaxColor.G;
    CurColor.B = blend * MinColor.B + invBlend * MaxColor.B;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cPlayerIndicatorFX_render);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

struct sVtx_3AF8 {
    float s, t, q, pad;
    int r, g, b, a;
    t4Vector pos;
    sVtx_3AF8() {}
};


extern sDrawState sDrawState_s_defaultDrawState_3AF8 __asm__("sDrawState_s_defaultDrawState");

// Render context (GrMan): state stack at 0xE84, vtable at 0x10D8.
struct sRCtx_3AF8 {
    char pad[0xE84];
    sDrawState* top;            // 0xE84
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
    ctx->top->m_bitGroups[1] = ((int)ctx->top->m_bitGroups[1] & ~mask) | ((v << shift) & mask);
}

static inline void rsSetB_3AF8(sRCtx_3AF8* ctx, int mask, int shift, int v)
{
    ctx->top->m_bitGroups[2] = ((int)ctx->top->m_bitGroups[2] & ~mask) | ((v << shift) & mask);
}

static inline int rsTex_3AF8(sRCtx_3AF8* ctx)
{
    return ctx->tex;
}

static inline int IsHuman_3AF8(cRider* rider)
{
    return rider->isHuman();
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
void cPlayerIndicatorFX::render()
{
    if (!ShouldDraw_3AF8(this)) {
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
        *renderCtx->top->m_textureState.m_texid = rsTex_3AF8(renderCtx);
    }
    float* viewMat = (float *)GrMan->GetView();
    t4Vector side;
    Set4_3AF8(side, viewMat[0], viewMat[4], viewMat[8], 0.0f);
    t4Vector up;
    Set4_3AF8(up, viewMat[1], viewMat[5], viewMat[9], 0.0f);
    const t4Matrix& headBone = mRider->mAnimModel->SkelMatrices[mRider->mBoneHead];
    t4Vector pos = headBone.row[3];
    const t4Vector* base = &pos;
    t4Vector beamTip = Add_3AF8(*base, Scale_3AF8(up, 20260.0f));
    t4Vector beamUpper = Add_3AF8(*base, Scale_3AF8(up, 20130.0f));
    t4Vector beamLower = Add_3AF8(*base, Scale_3AF8(up, 130.0f));
    t4Vector off = Scale_3AF8(side, 85.0f);
    tARGB fade;
    fade.A = 0.0f;
    fade.R = CurColor.R;
    fade.G = CurColor.G;
    fade.B = CurColor.B;
    sVtx_3AF8 verts[8];
    {
        t4Vector vertPos = Sub_3AF8(*base, off);
        verts[0].r = (int)(fade.R * 255.0f);
        verts[0].g = (int)(fade.G * 255.0f);
        verts[0].b = (int)(fade.B * 255.0f);
        verts[0].a = (int)(fade.A * 128.0f);
        verts[0].q = 1.0f;
        verts[0].pos = vertPos;
        verts[0].s = 0.0f;
        verts[0].t = 1.0f;
    }
    {
        t4Vector vertPos = Add_3AF8(*base, off);
        verts[1].r = (int)(fade.R * 255.0f);
        verts[1].g = (int)(fade.G * 255.0f);
        verts[1].b = (int)(fade.B * 255.0f);
        verts[1].a = (int)(fade.A * 128.0f);
        verts[1].q = 1.0f;
        verts[1].pos = vertPos;
        verts[1].s = 1.0f;
        verts[1].t = 1.0f;
    }
    {
        t4Vector vertPos = Sub_3AF8(beamLower, off);
        verts[2].r = (int)(CurColor.R * 255.0f);
        verts[2].g = (int)(CurColor.G * 255.0f);
        verts[2].b = (int)(CurColor.B * 255.0f);
        verts[2].a = (int)(CurColor.A * 128.0f);
        verts[2].q = 1.0f;
        verts[2].pos = vertPos;
        verts[2].s = 0.0f;
        verts[2].t = 0.0f;
    }
    {
        t4Vector vertPos = Add_3AF8(beamLower, off);
        verts[3].r = (int)(CurColor.R * 255.0f);
        verts[3].g = (int)(CurColor.G * 255.0f);
        verts[3].b = (int)(CurColor.B * 255.0f);
        verts[3].a = (int)(CurColor.A * 128.0f);
        verts[3].q = 1.0f;
        verts[3].pos = vertPos;
        verts[3].s = 1.0f;
        verts[3].t = 0.0f;
    }
    {
        t4Vector vertPos = Sub_3AF8(beamUpper, off);
        verts[4].r = (int)(CurColor.R * 255.0f);
        verts[4].g = (int)(CurColor.G * 255.0f);
        verts[4].b = (int)(CurColor.B * 255.0f);
        verts[4].a = (int)(CurColor.A * 128.0f);
        verts[4].q = 1.0f;
        verts[4].pos = vertPos;
        verts[4].s = 0.0f;
        verts[4].t = 1.0f;
    }
    {
        t4Vector vertPos = Add_3AF8(beamUpper, off);
        verts[5].r = (int)(CurColor.R * 255.0f);
        verts[5].g = (int)(CurColor.G * 255.0f);
        verts[5].b = (int)(CurColor.B * 255.0f);
        verts[5].a = (int)(CurColor.A * 128.0f);
        verts[5].q = 1.0f;
        verts[5].pos = vertPos;
        verts[5].s = 1.0f;
        verts[5].t = 1.0f;
    }
    {
        t4Vector vertPos = Sub_3AF8(beamTip, off);
        verts[6].r = (int)(fade.R * 255.0f);
        verts[6].g = (int)(fade.G * 255.0f);
        verts[6].b = (int)(fade.B * 255.0f);
        verts[6].a = (int)(fade.A * 128.0f);
        verts[6].q = 1.0f;
        verts[6].pos = vertPos;
        verts[6].s = 0.0f;
        verts[6].t = 0.0f;
    }
    {
        t4Vector vertPos = Add_3AF8(beamTip, off);
        verts[7].r = (int)(fade.R * 255.0f);
        verts[7].g = (int)(fade.G * 255.0f);
        verts[7].b = (int)(fade.B * 255.0f);
        verts[7].a = (int)(fade.A * 128.0f);
        verts[7].q = 1.0f;
        verts[7].pos = vertPos;
        verts[7].s = 1.0f;
        verts[7].t = 0.0f;
    }
    GrMan->RenderTriStrip(8, (tBxVertex*)verts, 0);
    GrMan->m_pCurrentDrawState--;
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
    self->mScreenGrabHandle.mMemory = 0;
    self->mExternalTimePercentage = 0;
    self->mCurrentInitiator = 0;
    self->mRequestingDelete = 0;
    self->mRequestDeleteFrame = 0;
    self->mScriptVtbl = cTransitionMan_cScriptObjTransition_vtbl;
    VPTR_OF(cObjNode, self) = cTransitionMan_vtbl;
    self->mScreenGrabHandle.mTextureId = -1;
    cTransitionObj_InitializeBuffers();
    cScriptObject_InitScriptObject((char*)&self->mObjType);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan__cTransitionMan);
#ifdef SKIP_ASM
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
    VPTR_OF(cObjNode, self) = cTransitionMan_vtbl;
    char* scriptObj = (char*)&self->mObjType;
    self->EndTransition();
    if (self->mRequestingDelete != 0) {
        cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
        obj->RemoveScreenGrabTexture(self->mScreenGrabHandle);
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
extern "C" bool cTransitionMan_StartTransition_def(void* self, int a1, void* o1, void* o2, float f0, float f1,
        float f2, int a4, float* externalTimePercentage) __asm__("cTransitionMan_StartTransition");

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
 * @return true when the transition started, false when the screen grab could not be set up
 */
bool cTransitionMan::StartTransition(int initiator, void* transitionIn, void* transitionOut, float inTime,
                                     float holdTime, float outTime, int useScreenGrab, float* externalTimePercentage)
{
    if (mbPlayingTransition != 0) {
        EndTransition();
    }
    mCurrentInitiator = initiator;
    mExternalTimePercentage = externalTimePercentage;
    if (useScreenGrab != 0) {
        if (mRequestingDelete != 0) {
            mRequestingDelete = 0;
        }
        char* ref = (char*)&mScreenGrabHandle;
        if (~mScreenGrabHandle.mTextureId == 0) {
            cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
            obj->CreateScreenGrabTexture(*(tScreenGrabHandle*)ref, 0x100, 0x100);
            if (~mScreenGrabHandle.mTextureId == 0) {
                if (transitionIn != 0) {
                    sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)transitionIn;
                    vt[1].fn(vthis(transitionIn, vt[1]), 3);
                }
                if (transitionOut != 0) {
                    sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)transitionOut;
                    vt[1].fn(vthis(transitionOut, vt[1]), 3);
                }
                return false;
            }
        }
    }
    mTransitionIn = (cTransitionObj*)transitionIn;
    mTransitionOut = (cTransitionObj*)transitionOut;
    mfInTime = inTime;
    mfHoldTime = holdTime;
    mfOutTime = outTime;
    mbPlayingTransition = 1;
    mfCurrentTime = 0.0f;
    mUsingScreenGrab = useScreenGrab;
    if (useScreenGrab != 0) {
        mRequestingScreenGrab = 1;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_StartTransitionIn);
#ifdef SKIP_ASM
extern "C" void cTransitionMan_StartTransition(void* self, int initiator, cTransitionObj* transitionIn,
                                               cTransitionObj* transitionOut, float f0, float f1, int useScreenGrab,
                                               float* externalTimePercentage, float f2);

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
void cTransitionMan::StartTransitionIn(int initiator, cTransitionObj* transitionIn, float inTime, float holdTime)
{
    cTransitionMan_StartTransition(this, initiator, transitionIn, 0, inTime, holdTime, 0, 0, 0.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_StartTransitionOut);
#ifdef SKIP_ASM
// PORT: the unit declares cTransitionMan_StartTransition as void; this caller returns its result.
extern "C" bool cTransitionMan_StartTransition_i(void* self, int initiator, cTransitionObj* transitionIn,
                                                 cTransitionObj* transitionOut, float f0, float f1, int useScreenGrab,
                                                 float* externalTimePercentage, float f2)
    __asm__("cTransitionMan_StartTransition");

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
int cTransitionMan::StartTransitionOut(int initiator, cTransitionObj* transition, float outTime)
{
    if (mbPlayingTransition == 0) {
        return cTransitionMan_StartTransition_i(this, initiator, 0, transition, 0.0f, 0.0f, 0, 0, outTime);
    }
    mTransitionOut = transition;
    mfOutTime = outTime;
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_StartTransitionScreenGrab);
#ifdef SKIP_ASM
extern "C" void cTransitionMan_StartTransition(void* self, int initiator, cTransitionObj* transitionIn,
                                               cTransitionObj* transitionOut, float f0, float f1, int useScreenGrab,
                                               float* externalTimePercentage, float f2);

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
void cTransitionMan::StartTransitionScreenGrab(int initiator, cTransitionObj* transition, float* externalTimePercentage,
                                               float transitionTime)
{
    cTransitionMan_StartTransition(this, initiator, 0, transition, 0.0f, 0.0f, 1, externalTimePercentage,
            transitionTime);
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
void cTransitionMan::EndTransition()
{
    if (mbPlayingTransition != 0) {
        mbPlayingTransition = 0;
        mNumPause = 0;
        char* object = (char*)mTransitionIn;
        if (object != 0) {
            sVEntry<void(void*, int)>* entry = &(*(sVEntry<void(void*, int)>**)object)[1];
            entry->fn(vthis(object, *entry), 3);
        }
        mTransitionIn = 0;
        object = (char*)mTransitionOut;
        if (object != 0) {
            sVEntry<void(void*, int)>* entry = &(*(sVEntry<void(void*, int)>**)object)[1];
            entry->fn(vthis(object, *entry), 3);
        }
        mTransitionOut = 0;
        mExternalTimePercentage = 0;
        if (~mScreenGrabHandle.mTextureId != 0) {
            cPS2GraphicsMan* obj = cGraphicsMan_TheGraphicsMan;
            mRequestingDelete = 1;
            mRequestDeleteFrame = obj->getCurrentFrameCount();
            mRequestingScreenGrab = 0;
            mUsingScreenGrab = 0;
        }
        mCurrentInitiator = 0;
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
void cTransitionMan::PauseTransition(int up)
{
    if (mbPlayingTransition == 0) {
        mNumPause = 0;
        return;
    }
    if (up) {
        mNumPause += 1;
        return;
    }
    if (mNumPause != 0) {
        mNumPause -= 1;
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
float cTransitionMan::GetTransitionTimeLeft()
{
    if (mbPlayingTransition == 0) {
        return 0.0f;
    }
    float remaining = mfInTime + mfHoldTime
            + mfOutTime - mfCurrentTime;
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
void cTransitionMan::update()
{
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    if (mRequestingDelete != 0) {
        if (ctx->getCurrentFrameCount() - mRequestDeleteFrame >= 3) {
            ctx->RemoveScreenGrabTexture(mScreenGrabHandle);
            mRequestingDelete = 0;
        }
    }
    if (mbPlayingTransition == 0) return;
    if (mNumPause != 0) return;
    if (mInUse > 0) {
        mfCurrentTime = mInfo.currTime;
    } else if (mExternalTimePercentage != 0) {
        mfCurrentTime = *mExternalTimePercentage * (mfInTime + mfHoldTime + mfOutTime);
    } else {
        mfCurrentTime += 1.0f / (float)cAppMan_TheAppMan->TimerHz;
    }
    if (mfInTime + mfHoldTime + mfOutTime <= mfCurrentTime) {
        EndTransition();
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





// cTransitionObj's slot 2, Draw(float amount) (visualfx/transitionobj.h).
typedef sVEntry<void(void*, float)> tTransitionDrawSlot;


static inline void rsSetA47E8(cPS2GraphicsMan* ctx, int mask, int shift, int v)
{
    ctx->m_pCurrentDrawState->m_bitGroups[1] = (((int)(ctx->m_pCurrentDrawState)->m_bitGroups[1]) & ~mask) | ((v << shift) & mask);
}

static inline void rsSetB47E8(cPS2GraphicsMan* ctx, int mask, int shift, int v)
{
    ctx->m_pCurrentDrawState->m_bitGroups[2] = (((int)(ctx->m_pCurrentDrawState)->m_bitGroups[2]) & ~mask) | ((v << shift) & mask);
}

static inline void rsSet047E8(cPS2GraphicsMan* ctx, int mask, int shift, int v)
{
    ctx->m_pCurrentDrawState->m_bitGroups[0] = (((int)(ctx->m_pCurrentDrawState)->m_bitGroups[0]) & ~mask) | ((v << shift) & mask);
}

static inline int fadeTex47E8(cTransitionMan* self)
{
    return self->mScreenGrabHandle.mTextureId;
}

/**
 * Draws a full-screen transition for its rider's view: the first overlay fading in, then the second fading
 * out.
 *
 * The overlay object's virtual slot 2 draws it with the alpha, over a 640 x 480 screen. The pending texture
 * is released when the hold ends. At an alpha of 0.93 or more it also resets the world painter
 * (cWorldPainterMan_reset).
 */
void cTransitionMan::render()
{
    if (mViewIndex != (int)SSXApp->Game->ViewMan->m_uiCurrentView) {
        return;
    }
    FigureOutTransitionUVs();
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    if (mbPlayingTransition == 0) {
        return;
    }
    float curTime = mfCurrentTime;
    if (mRequestingScreenGrab != 0 && mfInTime + mfHoldTime <= curTime) {
        ctx->TakeScreenGrab(mScreenGrabHandle);
        mRequestingScreenGrab = 0;
    }
    cTransitionObj* transition;
    float alpha;
    if (mTransitionIn == 0) {
        alpha = 1.0f - (curTime - mfHoldTime) / mfOutTime;
        transition = mTransitionOut;
    } else if (curTime < mfInTime || mTransitionOut == 0) {
        alpha = curTime / mfInTime;
        transition = mTransitionIn;
    } else if (curTime < mfInTime + mfHoldTime) {
        alpha = 1.0f;
        transition = mTransitionIn;
    } else {
        alpha = 1.0f - (curTime - (mfInTime + mfHoldTime)) / mfOutTime;
        transition = mTransitionOut;
    }
    float clamped = wakeClampE058(alpha, 0.0f, 1.0f);
    if (clamped >= 0.93f) {
        cWorldPainterMan_reset_47E8();
    }
    (ctx->m_pCurrentDrawState)[1] = (ctx->m_pCurrentDrawState)[0];
    ctx->m_pCurrentDrawState++;
    ctx->PushViewport();
    ctx->PushView();
    ctx->IdentityView();
    ctx->SetViewportOrthographic(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f, 0, 0);
    rsSetB47E8(ctx, 0x3E0, 5, 8);
    rsSetA47E8(ctx, 0x400000, 22, 1);
    rsSetA47E8(ctx, 0x1800000, 23, 2);
    rsSetA47E8(ctx, 0x300000, 20, 0);
    rsSetA47E8(ctx, 0xFF000, 12, 20);
    rsSetA47E8(ctx, 0x7C, 2, 5);
    rsSet047E8(ctx, 0xC, 2, 3);
    rsSetA47E8(ctx, 0x3, 0, 2);
    rsSetB47E8(ctx, 0x1FFFFC00, 10, 0);
    if (mUsingScreenGrab != 0) {
        ctx->m_pCurrentDrawState->m_textureState.m_texid[0] = fadeTex47E8((this));
    } else {
        ctx->m_pCurrentDrawState->m_textureState.m_texid[0] = -1;
    }
    if (mUsingScreenGrab == 0 || mRequestingScreenGrab == 0) {
        tTransitionDrawSlot* ov = *(tTransitionDrawSlot**)transition; // its vptr, at 0x0
        ov[2].fn(vthis(transition, ov[2]), clamped);
    }
    ctx->PopView();
    ctx->PopViewport();
    ctx->m_pCurrentDrawState--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cTransitionMan_OnStartObjectUse);
#ifdef SKIP_ASM

void cScriptObject_OnStartObjectUse(void* self);
extern "C" cTransitionObj* cSSXScriptEngine_MakeTransitionObject(void* tbl, int mode, int id);
extern "C" void cTransitionMan_StartTransition(void* self, int initiator, cTransitionObj* transitionIn,
                                               cTransitionObj* transitionOut, float f0, float f1, int useScreenGrab,
                                               float* externalTimePercentage, float f2);
extern "C" void cTransitionMan_StartTransitionScreenGrab(cTransitionMan* self, int initiator,
        cTransitionObj* transition,
                                                         float* externalTimePercentage, float transitionTime);

/**
 * Starts the transition a script object describes when a player starts using it.
 *
 * Calls the cScriptObject hook cScriptObject_OnStartObjectUse__FPv on the script object at +0x10. For a transition type (+0x28) other than 7
 * it makes an in and an out object with cSSXScriptEngine_MakeTransitionObject and starts a transition (initiator 2)
 * with in, hold and out times from +0x34, +0x30 and +0x2C. Type 7 starts a screen-grab transition with one object and
 * the time at +0x34. The level at +0x3C is then copied to +0x48.
 */
void cTransitionMan::OnStartObjectUse()
{
    tScriptObjTransitionInfo* transitionInfo = (&mInfo);
    cScriptObject_OnStartObjectUse((char*)&mObjType);
    if (mInfo.transition != keScriptTransition_CrossFade) {
        cTransitionObj* transitionIn = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, mInfo.transition,
                mInfo.transitionColour);
        cTransitionObj* transitionOut = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, mInfo.transition,
                mInfo.transitionColour);
        cTransitionMan_StartTransition(this, 2, transitionIn, transitionOut, mInfo.outTime, mInfo.holdTime, 0, 0,
                mInfo.inTime);
    } else {
        StartTransitionScreenGrab(2, cSSXScriptEngine_MakeTransitionObject(ScriptEngine, 7, mInfo.transitionColour), 0,
                mInfo.outTime);
    }
    mfCurrentTime = transitionInfo->currTime;
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
void cTransitionMan::OnEndObjectUse()
{
    cScriptObject_OnEndObjectUse((char*)&mObjType);
    EndTransition();
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
int cTransitionMan::IsTransitionPending()
{
    int result = 0;
    if (mUsingScreenGrab != 0) {
        result = mRequestingScreenGrab != 0;
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
 * The wipe and circle constructors (cWipe_TObj_cWipe_TObj, cCircle_TObj_cCircle_TObj1, cCircle_TObj_cCircle_TObj), the fade object code and
 * cSSXScriptEngine_MakeTransitionObject run it as the base constructor.
 *
 * @return @p self
 */
void* cTransitionObj_cTransitionObj(void* self)
{
    VPTR_AT(self, 0) = cTransitionObj_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowBox_cSnowBox);
#ifdef SKIP_ASM

struct sMatD88 {
    t4Vector r[4];
};

extern t4Matrix t4mat_Identity;
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
    self->mWindScale = 0.0f;
    self->mOffset.x = 0.0f;
    self->mOffset.y = 0.0f;
    self->mOffset.z = 0.0f;
    self->mOffset.w = 1.0f;
    self->mWindNext.x = 0.0f;
    self->mWindNext.y = 0.0f;
    self->mWindNext.z = 0.0f;
    self->mWindNext.w = 0.0f;
    self->mWindLast.x = 0.0f;
    self->mWindLast.y = 0.0f;
    self->mWindLast.z = 0.0f;
    self->mWindLast.w = 0.0f;
    self->mCurrentTimeToWindChange = 0.0f;
    self->mTotalTimeToWindChange = 0.0f;
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
    wakeCopyMatD88((sMatD88*)&self->mBoxMatrix, (sMatD88*)&t4mat_Identity);
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
void cSnowBox::resetWind()
{
    mTotalTimeToWindChange = 0.0f;
    mResetWind = 1;
    mCurrentTimeToWindChange = 0.0f;
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
extern "C" void cSnowBox__cSnowBox(cSnowBox* self, int flags)
{
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowBox_update);
#ifdef SKIP_ASM

// t4Vector with EA's 4-float constructor.
struct t4VectorXYZW : t4Vector {
    t4VectorXYZW(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

extern t4Vector t4vect_ZAxis_4F50 __asm__("t4vect_ZAxis");     // t4vect::ZAxis
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
static inline t4Vector Scale_4F50(float s, const t4Vector& v)
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
        : "o"(v), "f"(s)
        : "memory");
    return r;
}

/**
 * Updates a snow box for this frame: takes its flake settings from the snowfall parameters, varies the wind and moves
 * the box offset.
 *
 * A flake box (mSnowBoxType 0) takes its opacity and colour from the flake colour, mNumFlakes = mFlakeRatio * 500,
 * mWindScale = mFlakeWindRatio * 500 and mFlakeSize; a fluff box (1) the fluff colour, mFluffRatio * 6 flakes and
 * mFluffWindRatio * 250 wind. When the wind timer mCurrentTimeToWindChange runs out the old wind mWindNext is kept in
 * mWindLast and a new one is picked: the strength along the mWindRotation angle, mixed with random flurries by
 * mSnowFlurries, for 1 to 5 seconds. mOffset moves by the blended wind and the fall (mGravity * mGravityMultiplier)
 * times the frame time (1 / cAppMan_TheAppMan->TimerHz) and is wrapped into the box size mBoxScale. cSnowFall_update
 * calls it per box.
 *
 * @param params  the snowfall parameters filled by cSnowFall_update
 */
void cSnowBox::update(const tWorldPainterSnowfallParameters* params)
{
    switch (mSnowBoxType) {
    case 0:
        mOpacity = params->mFlakeColourA;
        mColourR = params->mFlakeColourR;
        mColourG = params->mFlakeColourG;
        mColourB = params->mFlakeColourB;
        mNumFlakes = (int)(params->mFlakeRatio * 500.0f);
        mWindScale = params->mFlakeWindRatio * 500.0f;
        mFlakeSize = params->mFlakeSize;
        break;
    case eSnowBoxFluff:
        mOpacity = params->mFluffColourA;
        mColourR = params->mFluffColourR;
        mColourG = params->mFluffColourG;
        mColourB = params->mFluffColourB;
        mNumFlakes = (int)(params->mFluffRatio * 6.0f);
        mWindScale = params->mFluffWindRatio * 250.0f;
        break;
    }
    const float kTimeDiff = 1.0f / (float)cAppMan_TheAppMan->TimerHz;
    float windBlend;
    float x;
    float y;
    float z;
    mCurrentTimeToWindChange -= kTimeDiff;
    if (mCurrentTimeToWindChange < 0.0f) {
        mWindLast = mWindNext;
        float windSin;
        float windCos;
        sincos_4F50(deg2rad_4F50(params->mWindRotation), windSin, windCos);
        t4VectorXYZW windBaseRotated(mWindScale * windCos, mWindScale * windSin, 0.0f, 0.0f);
        t4VectorXYZW windBaseFlurries(
            windBaseRotated.x * (1.0f - params->mSnowFlurries) +
                params->mSnowFlurries * FXRand_4F50(-mWindScale, mWindScale),
            windBaseRotated.y * (1.0f - params->mSnowFlurries) +
                params->mSnowFlurries * FXRand_4F50(-mWindScale, mWindScale),
            params->mSnowFlurries * FXRand_4F50(-mWindScale, mWindScale), 0.0f);
        mWindNext = windBaseFlurries;
        mCurrentTimeToWindChange = mTotalTimeToWindChange = FXRand_4F50(1.0f, 5.0f);
        if (mResetWind) {
            mWindLast = mWindNext;
            mResetWind = 0;
        }
    }
    windBlend = mCurrentTimeToWindChange / mTotalTimeToWindChange;
    wakeAddE398(mOffset, wakeScale0EE8(wakeAdd0EE8(wakeScale0EE8(mWindNext, 1.0f - windBlend),
                                                         wakeScale0EE8(mWindLast, windBlend)),
                                             kTimeDiff));
    wakeAddE398(mOffset, Scale_4F50(mGravity * params->mGravityMultiplier * kTimeDiff, t4vect_ZAxis_4F50));
    x = mOffset.x;
    y = mOffset.y;
    z = mOffset.z;
    BringIntoRange_4F50(x, mBoxScale);
    BringIntoRange_4F50(y, mBoxScale);
    BringIntoRange_4F50(z, mBoxScale);
    mOffset = t4VectorXYZW(x, y, z, 1.0f);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowBox_updateForCameraPosition);
#ifdef SKIP_ASM
// t4Vector with On Tour's t4Vector() and t4Vector(float, float, float, float) constructors (the snow and speed boost
// temporaries), and the placement new[] cSnowFall's constructor runs over its camera positions.
struct t4VectorCtor : t4Vector {
    t4VectorCtor() {}
    t4VectorCtor(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
    void* operator new[](unsigned int, void* p) { return p; }
};

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void wakeSub5430(t4Vector& a, const t4Vector& b)
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
 * Subtracts @p delta from mOffset and wraps each axis into the box size (mBoxScale). cSnowFall_render calls it with
 * the camera's change in position.
 *
 * @param delta  the camera movement since the last frame
 */
void cSnowBox::updateForCameraPosition(const t4Vector* delta)
{
    wakeSub5430(mOffset, *delta);
    float x = mOffset.x;
    float y = mOffset.y;
    float z = mOffset.z;
    float px = x;
    float py = y;
    float pz = z;
    wakeWrap5430(x, px, mBoxScale);
    wakeWrap5430(y, py, mBoxScale);
    wakeWrap5430(z, pz, mBoxScale);
    mOffset = t4VectorCtor(x, y, z, 1.0f);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSnowBox_render);
#ifdef SKIP_ASM
// t4Matrix copied through the VU0 registers (lqc2/sqc2), as retail's inlined matrix copies.
struct t4MatrixVu0 : t4Matrix {
    t4MatrixVu0() {}
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    t4MatrixVu0(const t4MatrixVu0& o)
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
};





// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
static inline void vu0SetMatrix55D8(t4Matrix& dst, const t4Matrix& o)
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
        : "r"(&dst), "r"(&o)
        : "memory");
}

// PORT: 128-bit TImode quadword (the PC port needs a 16-byte struct); retail takes the axes' addresses into a register
// (lui/addiu) before the lqc2, as for a TImode object.
typedef int cQuad128 __attribute__((mode(TI)));
extern cQuad128 t4vect_XAxis;
extern cQuad128 t4vect_YAxis;
extern cQuad128 t4vect_ZAxis;

// PORT: PS2-only VU0 inline asm (4x4 matrix * matrix).
static inline t4MatrixVu0 wakeMulMat55D8(const t4Matrix& a, const t4Matrix& b)
{
    t4MatrixVu0 r;
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
static inline t4Vector wakeDiv55D8(const t4Vector& v, float s)
{
    t4Vector r;
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
static inline void wakeAddEq55D8(t4Vector& a, const cQuad128& b)
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
static inline void wakeSubEq55D8(t4Vector& a, const cQuad128& b)
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
static inline void wakeSubV55D8(t4Vector& a, const t4Vector& b)
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
 * Does nothing while the box has no flakes (mNumFlakes). The record holds @p snowMatrix times mBoxMatrix, mOffset
 * divided by mBoxScale and shifted by one box toward the cell (@p x, @p y, @p z) to (x+1, y+1, z+1), minus 1.5; the
 * colour (mColourR..B) and mOpacity times 128; mNumFlakes and the three random seeds; and the cell bounds with
 * mFlakeSize and mBoxScale. Type 0 (mSnowBoxType) draws through RenderSnowBoxFlakes (GrMan slot 85), type 1 through
 * RenderSnowBoxFluffs (slot 86). cSnowFall_render calls it for each box.
 *
 * @param snowMatrix  the snow matrix
 * @param x  minimum x of the cell
 * @param y  minimum y of the cell
 * @param z  minimum z of the cell
 */
void cSnowBox::render(const t4Matrix* snowMatrix, float x, float y, float z)
{
    if (mNumFlakes == 0) {
        return;
    }
    float x1 = x + 1.0f;
    float y1 = y + 1.0f;
    float z1 = z + 1.0f;
    tSnowBoxData info;
    vu0SetMatrix55D8(info.mSnowBoxToWorldMatrix, wakeMulMat55D8(*snowMatrix, mBoxMatrix));
    t4Vector tempVec = wakeDiv55D8(mOffset, mBoxScale);
    info.mParticleOffset = tempVec;
    float red = mColourR * 128.0f;
    float green = mColourG * 128.0f;
    float blue = mColourB * 128.0f;
    float alpha = mOpacity * 128.0f;
    tempVec.y = green;
    info.mNumFlakes = mNumFlakes;
    info.mRandSeed = mRandSeed;
    info.mRandSeed2 = mRandSeed2;
    info.mRandSeed3 = mRandSeed3;
    info.mFlakeSize = mFlakeSize;
    info.mBoxScale = mBoxScale;
    tempVec.x = red;
    tempVec.z = blue;
    info.mMinX = x;
    info.mMinY = y;
    info.mMinZ = z;
    info.mMaxX = x1;
    info.mMaxY = y1;
    info.mMaxZ = z1;
    tempVec.w = alpha;
    *(t4Vector*)&info.mColour = tempVec; // the colour as one quadword (tARGB, same layout as t4Vector here)
    if (info.mParticleOffset.x < x + -0.5f) {
        wakeAddEq55D8(info.mParticleOffset, t4vect_XAxis);
    }
    if (x1 + 0.5f < info.mParticleOffset.x) {
        wakeSubEq55D8(info.mParticleOffset, t4vect_XAxis);
    }
    if (info.mParticleOffset.y < y + -0.5f) {
        wakeAddEq55D8(info.mParticleOffset, t4vect_YAxis);
    }
    if (y1 + 0.5f < info.mParticleOffset.y) {
        wakeSubEq55D8(info.mParticleOffset, t4vect_YAxis);
    }
    if (info.mParticleOffset.z < z + -0.5f) {
        wakeAddEq55D8(info.mParticleOffset, t4vect_ZAxis);
    }
    if (z1 + 0.5f < info.mParticleOffset.z) {
        wakeSubEq55D8(info.mParticleOffset, t4vect_ZAxis);
    }
    tempVec.x = 1.5f;
    tempVec.y = 1.5f;
    tempVec.z = 1.5f;
    tempVec.w = 0.0f;
    wakeSubV55D8(info.mParticleOffset, tempVec);
    switch (mSnowBoxType) {
    case 0: {
        GrMan->RenderSnowBoxFlakes(info);
        break;
    }
    case eSnowBoxFluff: {
        GrMan->RenderSnowBoxFluffs(info);
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
    VPTR_OF(cObjNode, self) = cSnowFall_vtbl;
    new (self->mLastCameraPosition) t4VectorCtor[2];
    cMemMan_validate();
    for (int i = 11; i >= 0; i--) {
        self->mSnowBoxes[i] = 0;
    }
    for (int j = 0; j < 2; j++) {
        self->mLastCameraPosition[j] = t4VectorCtor(0.0f, 0.0f, 0.0f, 1.0f);
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
extern "C" void cSnowBox__cSnowBox(cSnowBox* self, int flags);
extern "C" void cObjNode__cObjNode(void* self, int flags);

/**
 * Destroys the falling snow: deletes its twelve snow boxes and runs the cObjNode destructor.
 *
 * @param self   the snowfall node
 * @param flags  g++ destructor flags, passed on (bit 0 frees the object)
 */
extern "C" void cSnowFall__cSnowFall(cSnowFall* self, int flags)
{
    VPTR_OF(cObjNode, self) = cSnowFall_vtbl;
    cMemMan_validate();
    int i;
    for (i = 0; i < 12; i++) {
        int* snowBox = ((int*)self->mSnowBoxes[i]);
        if (snowBox != 0) {
            cSnowBox__cSnowBox((cSnowBox*)snowBox, 3);
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
extern tARGB cSnowfallToggles_m_SnowFlakeColour;  // [O] static tARGB cSnowfallToggles::m_SnowFlakeColour
extern tARGB cSnowfallToggles_m_SnowFluffColour;  // [O] static tARGB cSnowfallToggles::m_SnowFluffColour
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


/**
 * Updates the falling snow from each view's snow settings, then advances every snow box with its view's
 * settings.
 *
 * The settings (snow and wind intensity, fluff, flake size, wind rotation, flurries, gravity and two colours)
 * come from the debug snowfall toggles when cSnowfallToggles_m_EnableTest is set. Otherwise they come from
 * the render-state manager (entries 6 and 7). The snow intensity also goes to cSplashManager_ReportSnowfall with the view's
 * cGame_getSplashManager object.
 *
 */
void cSnowFall::update()
{
    tWorldPainterSnowfallParameters params[2];
    unsigned int i;
    for (i = 0; i < SSXApp->Game->ViewMan->m_uiNumView; i++) {
        if (cSnowfallToggles_m_EnableTest != 0) {
            params[i].mFlakeRatio = cSnowfallToggles_m_TestSnowIntensity;
            params[i].mFlakeWindRatio = cSnowfallToggles_m_TestWindIntensity;
            params[i].mFluffRatio = cSnowfallToggles_m_SnowFluffItensity;
            params[i].mFluffWindRatio = cSnowfallToggles_m_SnowFluffWindIntensity;
            params[i].mFlakeSize = cSnowfallToggles_m_TestFlakeSize;
            params[i].mWindRotation = cSnowfallToggles_m_TestWindRotation;
            params[i].mSnowFlurries = cSnowfallToggles_m_TestSnowFlurries;
            params[i].mGravityMultiplier = cSnowfallToggles_m_SnowGravityMultiplier;
            const tARGB* flakeColour = &cSnowfallToggles_m_SnowFlakeColour;
            params[i].mFlakeColourA = flakeColour->A;
            params[i].mFlakeColourR = flakeColour->R;
            params[i].mFlakeColourG = flakeColour->G;
            params[i].mFlakeColourB = flakeColour->B;
            const tARGB* fluffColour = &cSnowfallToggles_m_SnowFluffColour;
            params[i].mFluffColourA = fluffColour->A;
            params[i].mFluffColourR = fluffColour->R;
            params[i].mFluffColourG = fluffColour->G;
            params[i].mFluffColourB = fluffColour->B;
        } else {
            int k = (i == 0) ? 6 : 7;
            params[i].mFlakeRatio = cRenderStateMan_SnowIntensity(k);
            params[i].mFlakeWindRatio = cRenderStateMan_WindIntensity(k);
            params[i].mFluffRatio = cRenderStateMan_SnowFluffItensity(k);
            params[i].mFluffWindRatio = cRenderStateMan_SnowFluffWindIntensity(k);
            params[i].mFlakeSize = cRenderStateMan_FlakeSize(k);
            params[i].mWindRotation = cRenderStateMan_WindRotation(k);
            params[i].mSnowFlurries = cRenderStateMan_SnowFlurries(k);
            params[i].mGravityMultiplier = cRenderStateMan_SnowGravityMultiplier(k);
            params[i].mFlakeColourA = cRenderStateMan_SnowFlakeColourA(k);
            params[i].mFlakeColourR = cRenderStateMan_SnowFlakeColourR(k);
            params[i].mFlakeColourG = cRenderStateMan_SnowFlakeColourG(k);
            params[i].mFlakeColourB = cRenderStateMan_SnowFlakeColourB(k);
            params[i].mFluffColourA = cRenderStateMan_SnowFluffColourA(k);
            params[i].mFluffColourR = cRenderStateMan_SnowFluffColourR(k);
            params[i].mFluffColourG = cRenderStateMan_SnowFluffColourG(k);
            params[i].mFluffColourB = cRenderStateMan_SnowFluffColourB(k);
        }
        cSplashManager_ReportSnowfall(cGame_getSplashManager(SSXApp->Game, i), params[i].mFlakeRatio);
    }
    int j;
    for (j = 0; j < 12; j++) {
        cSnowBox* snowBox = mSnowBoxes[j];
        if (snowBox != 0) {
            snowBox->update(&params[snowBox->mViewNum]);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cSnowFall_render);
#ifdef SKIP_ASM

// The view matrix copy: a t4Matrix assigned through the VU0 registers (lqc2/sqc2), as retail's inlined copy.
struct t4MatrixVu0Set : t4Matrix {
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    t4MatrixVu0Set& operator=(const t4Matrix& o)
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
};

extern int cRenderToggles_m_DisableSnowFall;
extern int cSnowfallToggles_m_TestAdditive[1];
extern sDrawState sDrawState_s_defaultDrawState_6008[] __asm__("sDrawState_s_defaultDrawState");
extern t4Vector t4vect_Origin;
extern "C" void cSnowBox_render(cSnowBox* self, const t4Matrix* snowMatrix, float x, float y, float z);

static inline cGameViewMan* riders_6008()
{
    return SSXApp->Game->ViewMan;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector sub_6008(const t4Vector& a, const t4Vector& b)
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

static inline void rsSetA_6008(cPS2GraphicsMan* ctx, int mask, int shift, int v)
{
    ctx->m_pCurrentDrawState->m_bitGroups[1] = ((int)ctx->m_pCurrentDrawState->m_bitGroups[1] & ~mask)
        | ((v << shift) & mask);
}

static inline void rsSetB_6008(cPS2GraphicsMan* ctx, int mask, int shift, int v)
{
    ctx->m_pCurrentDrawState->m_bitGroups[2] = ((int)ctx->m_pCurrentDrawState->m_bitGroups[2] & ~mask)
        | ((v << shift) & mask);
}

/**
 * Draws the falling snow for the current view, moving its boxes with the camera.
 *
 * The boxes of the current view move by that view's camera position change since the last frame
 * (mLastCameraPosition). The cell the boxes draw in starts at the camera direction times 0.6, minus 0.5. Skipped when
 * cRenderToggles_m_DisableSnowFall is set; cSnowfallToggles_m_TestAdditive selects additive blending.
 */
void cSnowFall::render()
{
    if (cRenderToggles_m_DisableSnowFall != 0) {
        return;
    }
    cGameViewMan* views = riders_6008();
    int k = views->m_uiCurrentView;
    t4Vector camPos = views->PlayerCamera[k]->Pose.P;
    t4Vector camDiff = sub_6008(camPos, mLastCameraPosition[k]);
    mLastCameraPosition[k] = camPos;
    cPS2GraphicsMan* ctx = cGraphicsMan_TheGraphicsMan;
    ctx->m_pCurrentDrawState[1] = ctx->m_pCurrentDrawState[0];
    ctx->m_pCurrentDrawState++;
    *ctx->m_pCurrentDrawState = sDrawState_s_defaultDrawState_6008[0];
    rsSetA_6008(ctx, 0x400000, 22, 1);
    rsSetB_6008(ctx, 0x3E0, 5, 7);
    if (cSnowfallToggles_m_TestAdditive[0] != 0) {
        rsSetA_6008(ctx, 0x7C, 2, 7);
    } else {
        rsSetA_6008(ctx, 0x7C, 2, 5);
    }
    rsSetA_6008(ctx, 0x3, 0, 2);
    rsSetB_6008(ctx, 0x1FFFFC00, 10, 0);
    *ctx->m_pCurrentDrawState->m_textureState.m_texid = ctx->getEffectTexMap(8);
    t4MatrixVu0Set snowMatrix;
    snowMatrix = *ctx->GetView();
    snowMatrix.row[3] = t4vect_Origin;
    cGameViewMan* viewsNow = riders_6008();
    t4Vector camDir = viewsNow->PlayerCamera[viewsNow->m_uiCurrentView]->Direction;
    float boxMinX = camDir.x * 0.6f + -0.5f;
    float boxMinY = camDir.y * 0.6f + -0.5f;
    float boxMinZ = camDir.z * 0.6f + -0.5f;
    int i;
    for (i = 0; i < 12; i++) {
        cSnowBox* snowBox = mSnowBoxes[i];
        if (snowBox != 0 && snowBox->mViewNum == k) {
            snowBox->updateForCameraPosition(&camDiff);
            mSnowBoxes[i]->render(&snowMatrix, boxMinX, boxMinY, boxMinZ);
        }
    }
    ctx->m_pCurrentDrawState--;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("visualfx/boardwakefx", cSnowFall_readFromReplayFrame);
#ifdef SKIP_ASM
void cSnowBox_resetWind(void*);

/**
 * Resets every snow box (cSnowBox_resetWind); despite the name it reads no replay data (it takes no frame).
 *
 */
void cSnowFall::readFromReplayFrame(cBxReplayFrame*)
{
    int i;
    void** snowBoxes = (void**)(&mSnowBoxes[0]);
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
        (*(char**)&self->mRider) = (char*)self + 0x3B0;
        cRider_cRider((char*)self + 0x3B0);
    }
    self->mRider->mScriptObjectVtbl = (void*)&D_00488550;
    self->mRider->mScriptObjRiderVtbl = D_00459B90;
    VPTR_AT(self->mRider, sizeof(cRiderState)) = (void**)&D_00488598;
    if (inChrg == 0) {
        int vc;
        vtbl1 = D_00488550;
        self->mRider->mScriptObjectVtbl = &vtbl1;
        {
            char* vbo = ((char*)self->mRider) - 0x3B0;
            vc = (char*)self - vbo;
        }
        vtbl1.e[1].delta = D_00488550.e[1].delta + vc;
        vtbl2 = D_00488598;
        VPTR_AT(self->mRider, sizeof(cRiderState)) = (void**)&vtbl2;
        vtbl2.e[1].delta = D_00488598.e[1].delta + vc;
    }
    new ((char*)&self->mSpeedBoostTrail[0]) sWakeSeg_6320[30];
    sWakePt_6320*& pts = (sWakePt_6320*)self->mStreamBuf;
    pts = new (str_cSpeedBoostStreamerFX, 0, 0) sWakePt_6320[20];
    self->reset();
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSpeedBoostFX__cSpeedBoostFX);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "visualfx/boardwakefx_classes.h"

/**
 * Destroys a speed boost effect: frees its ribbon points (mStreamBuf) and calls cPS2GraphicsMan_setScreenFade on the graphics manager
 * with 0.
 */
cVbDtor002E64C0::~cVbDtor002E64C0()
{
    if (mStreamBuf != 0) cMemMan_free(mStreamBuf);
    cPS2GraphicsMan_setScreenFade(GrMan_boardwakefx, 0.0f);
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
/**
 * Resets a speed boost effect; despite the name it reads no replay data (it takes no frame).
 *
 * Sets the colour to red, the ribbon texture to 57 and clears the trail. mMaxStreamerSegments is 20 for a rider with a
 * view (a player) and 18 for others.
 */
void cSpeedBoostFX::reset()
{
    t4Vector defaultSide;
    defaultSide.x = 1.0f;
    defaultSide.y = 0.0f;
    defaultSide.z = 0.0f;
    defaultSide.w = 1.0f;
    mCurrentTexture = 57;
    mAlpha = 0.0f;
    mLastSide = defaultSide;
    mWidth = 0.0f;
    mVOffset = 0.0f;
    mSpeedBoostTrailCurrentLength = 0.0f;
    mSpeedBoostTrailNumSegs = 0;
    mSpeedBoostTrailCurrentSeg = 0;
    mStreamBufLen = 0;
    mStreamBufHead = 0;
    mMaxStreamerSegments = (mRider->mViewIndex >= 0) ? 20 : 18;
    mWasInAir = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSpeedBoostFX_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
#include "object/being.h"
#include "render/graphicsman.h"

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




extern int cPowerUpFXToggles_m_EnableSpeedBoostStreamers;
extern int cPowerUpFXToggles_m_TestSpeedBoostRibbon;
extern int cPowerUpFXToggles_m_TestRedRibbon;
extern int cPowerUpFXToggles_m_TestOrangeRibbon;
extern "C" void cSpeedBoostFX_UpdateSpeedBoostTrail_66B8(cSpeedBoostFX* self)
    __asm__("cSpeedBoostFX_UpdateSpeedBoostTrail");

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


static inline t4Vector BonePos_66B8(cRider* o, int bone)
{
    const t4Matrix& boneMatrix = o->mAnimModel->SkelMatrices[bone];
    return boneMatrix.row[3];
}

// The trail width through a reference accessor, as the landed code had it (the update's register allocation follows
// the accessor form; plain member access allocates the on-flag to another register).
static inline float& trailWidth(cSpeedBoostFX* self)
{
    return self->mWidth;
}

static inline void Trail_66B8(cSpeedBoostFX* self, const t4Vector& pos, int on, float k, float speed)
{
    if (on) {
        int prev = self->mStreamBufHead;
        self->mStreamBufHead++;
        self->mStreamBufHead %= 20;
        if (self->mStreamBufLen < self->mMaxStreamerSegments) {
            self->mStreamBufLen++;
        }
        self->mStreamBuf[self->mStreamBufHead] = self->mStreamBuf[prev];
        t4Vector d = vu0Scale(pos, 1.4f);
        for (int i = 0; i < 20; i++) {
            SubEq_66B8(self->mStreamBuf[i].l, d);
            vu0AddEq(self->mStreamBuf[i].r, d);
        }
    } else {
        self->mStreamBufLen = Approach_66B8(self->mStreamBufLen, 0, 1);
    }
    if (self->mStreamBufLen > 0) {
        cRider* o = self->mRider;
        t4Vector a = BonePos_66B8(o, o->mBoneHips);
        t4Vector b = BonePos_66B8(o, o->LHandSkelIndex);
        t4Vector c = BonePos_66B8(o, o->RHandSkelIndex);
        if (vu0Dot(b, pos) < vu0Dot(c, pos)) {
            self->mStreamBuf[self->mStreamBufHead].l = vu0Sub(b, vu0Scale(a, 0.7f));
            self->mStreamBuf[self->mStreamBufHead].r = vu0Sub(c, vu0Scale(a, 0.7f));
        } else {
            self->mStreamBuf[self->mStreamBufHead].l = vu0Sub(c, vu0Scale(a, 0.7f));
            self->mStreamBuf[self->mStreamBufHead].r = vu0Sub(b, vu0Scale(a, 0.7f));
        }
        self->mVOffset -= k * (speed * 0.016666668f);
        if (self->mVOffset < 0.0f) {
            self->mVOffset += 1.0f;
        }
        if (1.0f < self->mVOffset) {
            self->mVOffset -= 1.0f;
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
void cSpeedBoostFX::update()
{
    int air = cRider_getMotionState(mRider) == MS_Air;
    int on = 0;
    if (!air) {
        cRider* riderPtr = mRider;
        if (0.0f < riderPtr->mBoostStrength || 0.0f < riderPtr->mPowerUpBoostTimer) {
            on = 1;
        }
    }
    if (mWasInAir != 0 && !air) {
        mStreamBufLen = 0;
        mSpeedBoostTrailCurrentLength = 0.0f;
        mSpeedBoostTrailNumSegs = 0;
    }
    mWasInAir = air;
    cRider* riderPtr = mRider;
    float k = 0.002f;
    int type = riderPtr->mUberLevel;
    if (type == eUS_UltraUber) {
        k = 0.01f;
        mCurrentTexture = 61;
    } else if (0.01f < riderPtr->mPowerUpBoostTimer || cPowerUpFXToggles_m_TestSpeedBoostRibbon != 0) {
        k = 0.01f;
        mCurrentTexture = 60;
    } else if (type >= 10 || cPowerUpFXToggles_m_TestRedRibbon != 0) {
        k = 0.01f;
        mCurrentTexture = 59;
    } else if (type >= 5 || cPowerUpFXToggles_m_TestOrangeRibbon != 0) {
        k = 0.005f;
        mCurrentTexture = 58;
    } else {
        mCurrentTexture = 57;
    }
    riderPtr = mRider;
    float speed = vu0Length(riderPtr->mVelocity);
    if (on) {
        float boostAmt = Clamp_66B8(Max_66B8(riderPtr->mPowerUpBoostTimer, riderPtr->mBoostStrength), 0.0f, 1.0f);
        mTrailLength = (boostAmt * 400.0f + 400.0f) * (speed / 3333.3335f);
        trailWidth(this) = (riderPtr->mUberLevel == eUS_UltraUber) ? 50.0f : boostAmt * 9.0f + 6.0f;
        mAlpha = 1.0f;
    } else {
        mTrailLength *= 0.95f;
        mAlpha *= 0.95f;
        trailWidth(this) *= 0.95f;
    }
    if (cPowerUpFXToggles_m_EnableSpeedBoostStreamers != 0) {
        t4Vector pos = mRider->mMatrix.row[0];
        Trail_66B8(this, pos, on, k, speed);
    }
    cSpeedBoostFX_UpdateSpeedBoostTrail_66B8(this);
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSpeedBoostFX_UpdateSpeedBoostTrail);
#ifdef SKIP_ASM




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
void cSpeedBoostFX::UpdateSpeedBoostTrail()
{
    if (0.0f < mTrailLength || 0.0f < mSpeedBoostTrailCurrentLength) {
        if (0.0f < wakeLenE6C08(mRider->mVelocity)) {
            tSpeedBoostTrailSeg* ring;
            if (mSpeedBoostTrailNumSegs == kMaxTrailSegs) {
                mSpeedBoostTrailCurrentLength -= mSpeedBoostTrail[(mSpeedBoostTrailCurrentSeg + 29) % 30].mLength;
                ring = mSpeedBoostTrail;
            } else {
                ring = mSpeedBoostTrail;
                mSpeedBoostTrailNumSegs++;
            }
            mSpeedBoostTrailCurrentSeg = (mSpeedBoostTrailCurrentSeg + 29) % 30;
            t4Vector pos = *mRider->getPosition();
            t4Vector boardPos = wakeRowE6C08(mRider, 3);
            t4Vector boardSide = wakeRowE6C08(mRider, 2);
            t4Vector boardFwd = wakeRowE6C08(mRider, 0);
            if (mRider->IsRidingFakie != 0) {
                boardFwd = wakeScaleE6C08(boardFwd, -1.0f);
            }
            float dist;
            if (mSpeedBoostTrailNumSegs >= 2) {
                dist = wakeLenE6C08(wakeSubE6C08(pos, mSpeedBoostTrailLastPos));
            } else {
                dist = 0.0f;
            }
            mSpeedBoostTrailLastPos = pos;
            ring[mSpeedBoostTrailCurrentSeg].mLength = 0.0f;
            ring[(mSpeedBoostTrailCurrentSeg + 1) % 30].mLength = dist;
            mSpeedBoostTrailCurrentLength += dist;
            t4Vector noseNeg = wakeSubE6C08(wakeAddE6C08(boardPos, wakeScaleE6C08(boardFwd, 75.0f)), wakeScaleE6C08(boardSide, 17.5f));
            t4Vector nosePos = wakeAddE6C08(wakeAddE6C08(boardPos, wakeScaleE6C08(boardFwd, 75.0f)), wakeScaleE6C08(boardSide, 17.5f));
            t4Vector tailNeg = wakeSubE6C08(wakeSubE6C08(boardPos, wakeScaleE6C08(boardFwd, 75.0f)), wakeScaleE6C08(boardSide, 17.5f));
            t4Vector tailPos = wakeAddE6C08(wakeSubE6C08(boardPos, wakeScaleE6C08(boardFwd, 75.0f)), wakeScaleE6C08(boardSide, 17.5f));
            float lean = 0.0f;
            if (mRider->PressMode != 0) {
                int curID = cRiderAnimBase_getCurrentAnimID(mRider->mRiderAnim, 2);
                if ((curID >= kAID_NoseChangeFakie && curID <= kAID_NoseBalance) || curID == kAID_TailPivotL ||
                    curID == kAID_TailPivotR) {
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
            t4Vector n = wakeCrossE6C08(mRider->mVelocity, mRider->mGroundNormal);
            if (wakeDotE6C08(n, mLastSide) < 0.0f) {
                n = wakeScaleE6C08(n, -1.0f);
            }
            mLastSide = n;
            t4Vector noseNegOff = wakeSubE6C08(noseNeg, mRider->mGroundContactPoint);
            t4Vector nosePosOff = wakeSubE6C08(nosePos, mRider->mGroundContactPoint);
            t4Vector tailNegOff = wakeSubE6C08(tailNeg, mRider->mGroundContactPoint);
            t4Vector tailPosOff = wakeSubE6C08(tailPos, mRider->mGroundContactPoint);
            float noseNegDist = wakeDotE6C08(noseNegOff, n);
            float nosePosDist = wakeDotE6C08(nosePosOff, n);
            float tailNegDist = wakeDotE6C08(tailNegOff, n);
            float tailPosDist = wakeDotE6C08(tailPosOff, n);
            float invMaxD = 1.0f / WAKE_MAX_E6C08(wakeAbsE6C08(noseNegDist), WAKE_MAX_E6C08(wakeAbsE6C08(nosePosDist), WAKE_MAX_E6C08(wakeAbsE6C08(tailNegDist), wakeAbsE6C08(tailPosDist))));
            float noseNegNorm = noseNegDist * invMaxD;
            float nosePosNorm = nosePosDist * invMaxD;
            float tailNegNorm = tailNegDist * invMaxD;
            float tailPosNorm = tailPosDist * invMaxD;
            t4Vector lo;
            t4Vector hi;
            if (noseNegNorm < nosePosNorm && noseNegNorm < tailNegNorm + 0.5f && noseNegNorm < tailPosNorm + 0.5f) {
                lo = wakeDropE6C08(this, noseNeg, noseNegOff);
            } else if (nosePosNorm < noseNegNorm && nosePosNorm < tailNegNorm + 0.5f && nosePosNorm < tailPosNorm + 0.5f) {
                lo = wakeDropE6C08(this, nosePos, nosePosOff);
            } else if (tailNegNorm < noseNegNorm && tailNegNorm < nosePosNorm && tailNegNorm < tailPosNorm) {
                lo = wakeDropE6C08(this, tailNeg, tailNegOff);
            } else {
                lo = wakeDropE6C08(this, tailPos, tailPosOff);
            }
            if (nosePosNorm <= noseNegNorm && tailNegNorm - 0.5f <= noseNegNorm && tailPosNorm - 0.5f <= noseNegNorm) {
                hi = wakeDropE6C08(this, noseNeg, noseNegOff);
            } else if (noseNegNorm <= nosePosNorm && tailNegNorm - 0.5f <= nosePosNorm && tailPosNorm - 0.5f <= nosePosNorm) {
                hi = wakeDropE6C08(this, nosePos, nosePosOff);
            } else if (noseNegNorm <= tailNegNorm && nosePosNorm <= tailNegNorm && tailPosNorm <= tailNegNorm) {
                hi = wakeDropE6C08(this, tailNeg, tailNegOff);
            } else {
                hi = wakeDropE6C08(this, tailPos, tailPosOff);
            }
            mSpeedBoostTrail[mSpeedBoostTrailCurrentSeg].mLeft = t3VectorInit(lo.x, lo.y, lo.z);
            mSpeedBoostTrail[mSpeedBoostTrailCurrentSeg].mRight = t3VectorInit(hi.x, hi.y, hi.z);
        }
    }
    int keep = 0;
    if (1.0f < mTrailLength) {
        keep = 3;
    }
    while (keep < mSpeedBoostTrailNumSegs && mTrailLength < mSpeedBoostTrailCurrentLength) {
        mSpeedBoostTrailCurrentLength -= mSpeedBoostTrail[(mSpeedBoostTrailCurrentSeg + mSpeedBoostTrailNumSegs - 1) % 30].mLength;
        mSpeedBoostTrailNumSegs--;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", cSpeedBoostFX_render);
#ifdef SKIP_ASM
extern int cPowerUpFXToggles_m_EnableSpeedBoostFX;
extern "C" void* cPS2GraphicsMan_beginTriStripPacket(void* gfx, int n);
extern "C" void cPS2GraphicsMan_endTriStripPacket(void* gfx);


// The x, y, z of a vertex position from a trail corner (On Tour t4Vector = t3Vector form).
static inline void setXYZ(t4Vector& v, const t3Vector& p)
{
    v.x = p.x;
    v.y = p.y;
    v.z = p.z;
}

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4VectorCtor scale_7A10(const t4Vector& v, float s)
{
    t4VectorCtor r;
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
static inline t4VectorCtor add_7A10(const t4Vector& a, const t4Vector& b)
{
    t4VectorCtor r;
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
static inline t4VectorCtor sub_7A10(const t4Vector& a, const t4Vector& b)
{
    t4VectorCtor r;
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

// The rider's hip bone position (row 3 of its bone matrix).
static inline t4Vector bonePos_7A10(cRider* rider)
{
    const t4Matrix& hips = rider->mAnimModel->SkelMatrices[rider->mBoneHips];
    return hips.row[3];
}

// Fills vertex i of a strip: colour (alpha scaled to 0..128), q = 1, position, then st.
static inline void setVtx_7A10(tBxVertex* base, int i, float s, float t, const tARGB& c, const t4Vector& p)
{
    tBxVertex* vx = &base[i];
    vx->r = (int)(c.R * 255.0f);
    vx->g = (int)(c.G * 255.0f);
    vx->b = (int)(c.B * 255.0f);
    vx->a = (int)(c.A * 128.0f);
    vx->one = 1.0f;
    vx->p = p;
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
void cSpeedBoostFX::render()
{
    if (mStreamBufLen == 0 || mTrailLength < 3.0f || mAlpha < 0.05f
        || cPowerUpFXToggles_m_EnableSpeedBoostFX == 0)
        return;
    ((cGfx_DAB8*)GrMan_boardwakefx)->PushState();
    *((cGfx_DAB8*)GrMan_boardwakefx)->stateTop = sDrawState_s_defaultDrawState_DAB8;
    *GrMan->m_pCurrentDrawState->m_textureState.m_texid = GrMan->getEffectTexMap(mCurrentTexture);
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
        float k = 1.0f / (float)((mStreamBufLen - 1) * (mStreamBufLen - 1));
        t4VectorCtor offs = scale_7A10(bonePos_7A10(mRider), 0.7f);
        tARGB col;
        col.A = 1.0f;
        col.R = 1.0f;
        col.G = 1.0f;
        col.B = 1.0f;
        t4VectorCtor pa, pb;
        float dv = 1.0f / (float)mStreamBufLen;
        float x = 0.0f;
        float texV = mVOffset;
        t4Vector dir = mRider->mMatrix.row[0];
        t4VectorCtor side = scale_7A10(dir, mWidth * 0.5f);
        int idx = mStreamBufHead;
        for (int i = 0; i < mStreamBufLen; i++) {
            if (idx < 0)
                idx = 19;
            col.A = mAlpha * (1.0f - x * x * k);
            if (col.A < 0.0f)
                col.A = 0.0f;
            n += 2;
            pa = add_7A10(mStreamBuf[idx].l, offs);
            pb = add_7A10(mStreamBuf[idx].r, offs);
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
            GrMan->RenderTriStrip(n, left, 0);
            GrMan->RenderTriStrip(n, right, 0);
        }
    }
    if (mSpeedBoostTrailNumSegs >= 2) {
        cPS2GraphicsMan* gfx = cGraphicsMan_TheGraphicsMan;
        tBxVertex* verts = (tBxVertex*)cPS2GraphicsMan_beginTriStripPacket(gfx, mSpeedBoostTrailNumSegs * 2);
        // PORT: g++ minimum operator (<?).
        float len = mTrailLength <? mSpeedBoostTrailCurrentLength;
        float acc = 0.0f;
        int j;
        int vertIdx = 0;
        for (j = 0; j < mSpeedBoostTrailNumSegs; j++) {
            acc += mSpeedBoostTrail[j].mLength;
            float trailFrac = acc / len;
            float vv = mVOffset + trailFrac;
            float al = wakeClampE058(mAlpha * (1.0f - trailFrac), 0.0f, 1.0f);
            tARGB vertColour;
            vertColour.A = wakeClampE058(al, 0.0f, 1.0f);
            vertColour.R = 1.0f;
            vertColour.G = 1.0f;
            vertColour.B = 1.0f;
            int si = (mSpeedBoostTrailCurrentSeg + j) % 30;
            t4VectorCtor pos;
            setXYZ(pos, mSpeedBoostTrail[si].mLeft);
            pos.w = 1.0f;
            setVtx_7A10(verts, vertIdx++, 0.0f, vv, vertColour, pos);
            setXYZ(pos, mSpeedBoostTrail[si].mRight);
            pos.w = 1.0f;
            setVtx_7A10(verts, vertIdx++, 1.0f, vv, vertColour, pos);
        }
        cPS2GraphicsMan_endTriStripPacket(gfx);
    }
    ((cGfx_DAB8*)GrMan_boardwakefx)->stateTop--;
}
#endif

