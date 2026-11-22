#include "vtable.h"
#include "common.h"
#include "intersect/bxnash.h"
#include "world/worldview.h"
#include "ai/focusbox.h"
#include "intersect/worldrider.h"
#include "util/surfprop.h"
#include "object/instancenode.h"
#include "ai/airpredictor.h"
#include "ui/uithing.h"
#include "animation/animsequencer.h"
#include "ai/dangerman.h"
#include "ai/rideranim.h"
#include "replay/playbackman.h"
#include "replay/frameptr.h"
#include "ai/ai.h"
#include "ai/riderhandler.h"
#include "sound/ssxaudio.h"
#include "ai/computer.h"
#include "ai/player.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion_gainFocus);
#ifdef SKIP_ASM
struct sVec4HP {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4HP vu0SubHP(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline sVec4HP vu0DivHP(const sVec4HP& v, float s)
{
    sVec4HP r;
    int t;
    __asm__ __volatile__(
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

/**
 * Sets the rider's velocity so it reaches the handplant's spline position in the entry time.
 *
 * When the handplant motion gains focus, the rider's mVelocity becomes (SplinePos - mPosition) / EnterSec,
 * computed with VU0 instructions.
 */
extern "C" void cHandplantMotion_gainFocus(cHandplantMotion* self)
{
    cRider* rider = self->mRider;
    *(sVec4HP*)&rider->mVelocity = vu0DivHP(vu0SubHP(*(sVec4HP*)&self->SplinePos, *(sVec4HP*)&rider->mPosition), self->EnterSec);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion_init);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// Global vectors and a matrix in .bss read by the handplant code (set up elsewhere at startup).
extern t4Vector t4vect_Zero_8BA0 __asm__("t4vect_Zero");

extern t4Vector t4vect_ZAxis_8BA0 __asm__("t4vect_ZAxis");

extern "C" float func_0031C128(float x);
extern "C" float fSin(float x);
extern "C" void fSinCos(float* s, float* c, float angle);
extern t4Vector t4vect_XAxis;

struct sQ_8BA0 {
    float x, y, z, w;
    sQ_8BA0() {}
    sQ_8BA0(const float& ax, const float& ay, const float& az, const float& aw) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));

struct sV_8BA0 : sQ_8BA0 {
    sV_8BA0(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

struct sHP_8BA0 {
    sQ_8BA0 start;   // 0x00
    sQ_8BA0 vel;     // 0x10
    sQ_8BA0 v20;     // 0x20
    sQ_8BA0 target;  // 0x30
    sQ_8BA0 v40;     // 0x40
    sQ_8BA0 lpos;    // 0x50
    sQ_8BA0 lvel;    // 0x60
    sQ_8BA0 axis;    // 0x70
    float spin;      // 0x80
    float dur;       // 0x84
    float time;      // 0x88
    float f8C;       // 0x8C
    int f90;         // 0x90
    int f94;         // 0x94
    char pad98[0x8];
    char* rider;     // 0xA0
};

struct sHitObj_8BA0 {
    char pad0[0x1C];
    int flags;       // 0x1C
};

struct sHit_8BA0 {
    sQ_8BA0 pos;     // 0x00
    sQ_8BA0 normal;  // 0x10
    char pad20[0x38];
    sHitObj_8BA0* obj; // 0x58
};

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sQ_8BA0 Sub_8BA0(const sQ_8BA0& a, const sQ_8BA0& b)
{
    sQ_8BA0 r;
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sQ_8BA0 Normalize_8BA0(const sQ_8BA0& v)
{
    sQ_8BA0 r;
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
static inline float Dot_8BA0(const sQ_8BA0& a, const sQ_8BA0& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_8BA0(const sQ_8BA0& v)
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sQ_8BA0 Cross_8BA0(const sQ_8BA0& a, const sQ_8BA0& b)
{
    sQ_8BA0 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "o"(a), "o"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void DivEq_8BA0(sQ_8BA0& v, float s)
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
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline sQ_8BA0 Div_8BA0(const sQ_8BA0& v, float s)
{
    sQ_8BA0 r;
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

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void ScaleEq_8BA0(sQ_8BA0& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQ_8BA0 QuatMul_8BA0(const sQ_8BA0& a, const sQ_8BA0& b)
{
    sQ_8BA0 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline sQ_8BA0 AxisAngle_8BA0(const sQ_8BA0& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQ_8BA0(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline sQ_8BA0 Conj_8BA0(const sQ_8BA0& q)
{
    sQ_8BA0 r(q.x, q.y, q.z, q.w);
    r.x = -r.x;
    r.y = -r.y;
    r.z = -r.z;
    return r;
}

static inline sQ_8BA0 up_8BA0(char* r) { return *(sQ_8BA0*)(r + 0x1C0); }

/**
 * Starts a handplant on a wall hit: records the start and plant points and the rotation to the plant pose.
 *
 * InitPos takes the rider's position, SplinePos and SplineDir the hit point and normal, ApproachDir the unit
 * vector from the rider's mBoneHead bone to the plant point, InitTangentSpeed the velocity along SplineDir
 * and IsRailSpline bit 0 of the hit's surface flags (record at +0x58, flags at +0x1C); ExitVel and ElapsedSec
 * start at 0. The plant pose is the rider turned upright, then about the vertical until its side axis lines
 * up with the horizontal wall direction or its reverse, whichever is nearer (IsToeside marks the reverse and
 * is toggled when rider +0x320 is set). SnapRotationAxis gets the axis of the rotation from the current
 * orientation to that pose and SnapRotationRate its angle / EnterSec.
 *
 * @param hit  the wall hit (On Tour tIntersectInfo): position, then normal; the caller points the normal
 *             along the velocity
 * @param dur  entry time in seconds, stored as EnterSec (the caller passes 0.25-0.75 s, shorter when faster)
 */
// Starts a handplant on a hit: stores the start/target, and works out the spin axis and angle that turn the rider
// from its current orientation to one facing the wall.
// PORT: `<?` / `>?` are the g++ minimum / maximum operators (EE min.s / max.s).
extern "C" void cHandplantMotion_init(cHandplantMotion* self, sHit_8BA0* hit, float dur)
{
    cRider* rider = self->mRider;
    *(sQ_8BA0*)&self->InitPos = *(sQ_8BA0*)&rider->mPosition;
    *(sQ_8BA0*)&self->ExitVel = (const sQ_8BA0&)t4vect_Zero_8BA0;
    *(sQ_8BA0*)&self->SplineDir = hit->normal;
    *(sQ_8BA0*)&self->SplinePos = hit->pos;
    *(sQ_8BA0*)&self->ApproachDir = Normalize_8BA0(Sub_8BA0(*(sQ_8BA0*)&self->SplinePos, *(sQ_8BA0*)((char*)rider->mAnimModel->bones + (rider->mBoneHead << 5))));
    self->EnterSec = dur;
    self->ElapsedSec = 0.0f;
    self->InitTangentSpeed = Dot_8BA0(*(sQ_8BA0*)&self->mRider->mVelocity, *(sQ_8BA0*)&self->SplineDir);
    int isRail = 0;
    if (hit->obj)
        isRail = hit->obj->flags & 1;
    self->IsRailSpline = isRail;
    sQ_8BA0 idealOrientation = *(sQ_8BA0*)&self->mRider->mRotation;
    sQ_8BA0 deltaRot;
    sQ_8BA0 axis;
    axis = Cross_8BA0(up_8BA0((char*)self->mRider), (const sQ_8BA0&)t4vect_ZAxis_8BA0);
    float len = Len_8BA0(axis);
    if (0.001f < len) {
        DivEq_8BA0(axis, len);
        deltaRot = AxisAngle_8BA0(axis, func_0031C128(len <? 1.0f));
        idealOrientation = QuatMul_8BA0(deltaRot, idealOrientation);
    }
    sQ_8BA0 fwd;
    fwd.x = 1.0f - 2.0f * (idealOrientation.y * idealOrientation.y + idealOrientation.z * idealOrientation.z);
    fwd.y = 2.0f * (idealOrientation.x * idealOrientation.y + idealOrientation.w * idealOrientation.z);
    fwd.z = 2.0f * (idealOrientation.x * idealOrientation.z - idealOrientation.w * idealOrientation.y);
    fwd.w = 0.0f;
    sQ_8BA0 flatSplineDir = *(sQ_8BA0*)&self->SplineDir;
    flatSplineDir.z = 0.0f;
    sQ_8BA0 n = Normalize_8BA0(flatSplineDir);
    self->IsToeside = Dot_8BA0(n, fwd) < 0.0f;
    if (self->IsToeside)
        ScaleEq_8BA0(n, -1.0f);
    if (self->mRider->IsRidingFakie)
        self->IsToeside ^= 1;
    axis = Cross_8BA0(fwd, n);
    len = Len_8BA0(axis);
    if (0.001f < len) {
        DivEq_8BA0(axis, len);
        deltaRot = AxisAngle_8BA0(axis, func_0031C128(len <? 1.0f));
        idealOrientation = QuatMul_8BA0(deltaRot, idealOrientation);
    }
    deltaRot = QuatMul_8BA0(idealOrientation, Conj_8BA0(*(sQ_8BA0*)&self->mRider->mRotation));
    float ang;
    if (0.0f < deltaRot.w) {
        ang = (1.5707963705062866f - func_0031C128(deltaRot.w <? 1.0f)) * 2.0f;
        self->SnapRotationRate = ang;
        if (ang >= 9.999999747378752e-06f)
            *(sQ_8BA0*)&self->SnapRotationAxis = Div_8BA0(sV_8BA0(deltaRot.x, deltaRot.y, deltaRot.z, 0.0f), fSin(ang * 0.5f));
        else
            *(sQ_8BA0*)&self->SnapRotationAxis = (const sQ_8BA0&)t4vect_XAxis;
    } else {
        ang = (1.5707963705062866f - func_0031C128(-(deltaRot.w >? -1.0f))) * 2.0f;
        self->SnapRotationRate = ang;
        if (ang >= 9.999999747378752e-06f)
            *(sQ_8BA0*)&self->SnapRotationAxis = Div_8BA0(sV_8BA0(deltaRot.x, deltaRot.y, deltaRot.z, 0.0f), fSin(ang * -0.5f));
        else
            *(sQ_8BA0*)&self->SnapRotationAxis = (const sQ_8BA0&)t4vect_XAxis;
    }
    self->SnapRotationRate = self->SnapRotationRate / self->EnterSec;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion_loseFocus);
#ifdef SKIP_ASM
// cHandplantMotion_calcExitVel returns a value in $2 that is unused here; declaring it int keeps $2 free as the target has it.
extern "C" int cHandplantMotion_calcExitVel(void* self);


/**
 * Ends the handplant motion: makes sure the exit velocity is computed and gives it to the rider.
 *
 * Calls cHandplantMotion_calcExitVel() (On Tour calcExitVel: fills ExitVel from the handplant kind at rider +0x32C unless it
 * is already set), then copies ExitVel into the rider's mVelocity.
 */
// Updates the hand plant and copies its position into the rider's hand target.
extern "C" void cHandplantMotion_loseFocus(cHandplantMotion* self)
{
    cHandplantMotion_calcExitVel(self);
    *(&self->mRider->mVelocity) = *(&self->ExitVel);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion_step);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
extern t4Vector t4vect_Zero_91A8 __asm__("t4vect_Zero");
extern t4Vector t4vect_ZAxis_91A8 __asm__("t4vect_ZAxis");
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);
extern "C" void cAirPredictor_updateFlightPath(void* ap, void* pos, void* vel, float dt);
extern "C" void cRider_alignRiderAxis(void* rider, void* a, void* b, float x, float y);
extern "C" int cHandplantMotion_calcExitVel(void* self);
extern "C" void fSinCos(float* s, float* c, float angle);

struct sQ_91A8 {
    float x, y, z, w;
    sQ_91A8() {}
    sQ_91A8(float ax, float ay, float az, float aw) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));



// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sQ_91A8 vu0Sub_91A8(const sQ_91A8& a, const sQ_91A8& b)
{
    sQ_91A8 r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sQ_91A8 vu0Add_91A8(const sQ_91A8& a, const sQ_91A8& b)
{
    sQ_91A8 r;
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sQ_91A8 vu0ScaleP_91A8(const sQ_91A8* v, float s)
{
    sQ_91A8 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sQ_91A8 vu0Scale_91A8(const sQ_91A8& v, float s)
{
    sQ_91A8 r;
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_91A8(const sQ_91A8& a, const sQ_91A8& b)
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

// PORT: PS2-only VU0 macro-mode asm (quaternion product a * b).
static inline sQ_91A8 QuatMul_91A8(const sQ_91A8& a, const sQ_91A8& b)
{
    sQ_91A8 r;
    __asm__(
        "lqc2       $vf4, %1\n"
        "lqc2       $vf5, %2\n"
        "vmul.xyzw  $vf7, $vf4, $vf5\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vmulaw.xyz ACC, $vf4, $vf5w\n"
        "vmaddaw.xyz ACC, $vf5, $vf4w\n"
        "vsubax.w   ACC, $vf7, $vf7x\n"
        "vmsubay.w  ACC, $vf0, $vf7y\n"
        "vmsubz.w   $vf8, $vf0, $vf7z\n"
        "vmaddw.xyz $vf8, $vf6, $vf0w\n"
        "sqc2       $vf8, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

static inline sQ_91A8 AxisAngle_91A8(const sQ_91A8& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQ_91A8(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline int eq_91A8(cHandplantMotion* handplantMotion, const sQ_91A8& b)
{
    return (*(sQ_91A8*)&handplantMotion->ExitVel).x == b.x && (*(sQ_91A8*)&handplantMotion->ExitVel).y == b.y && (*(sQ_91A8*)&handplantMotion->ExitVel).z == b.z && (*(sQ_91A8*)&handplantMotion->ExitVel).w == b.w;
}

static inline int landed_91A8(char* ap)
{
    int st = *(int*)(ap + 0xAC);
    int ok = 0;
    if (st == 1 || st == 3) ok = 1;
    return ok;
}

/**
 * Advances the handplant one frame: eases the rider onto the plant point, then holds it and plans the exit.
 *
 * The time step is mTimeScale / 60. While ElapsedSec <= EnterSec the position eases out from InitPos to
 * SplinePos (InitPos + (InitPos - SplinePos) * u(u - 2), u = ElapsedSec / EnterSec) and the orientation turns
 * about SnapRotationAxis at SnapRotationRate. After that the rider sits at SplinePos with zero velocity.
 * While ExitVel is still zero it calls cHandplantMotion_calcExitVel() (calcExitVel); once that succeeds it launches the air
 * predictor from 5 units above SplinePos with ExitVel and steps it (cAirPredictor_updateFlightPath, 1/60 s) at least 8 times
 * and until it has a landing. Later frames keep stepping the prediction until it has a landing, then, if the
 * landing normal faces ExitVel, turn the rider toward it (cRider_alignRiderAxis, at most 380 degrees/s).
 */
extern "C" void cHandplantMotion_step(cHandplantMotion* self)
{
    float step = self->mRider->mTimeScale * 0.01666666753590107f;
    self->ElapsedSec += step;
    if (self->ElapsedSec <= self->EnterSec) {
        float progress = self->ElapsedSec / self->EnterSec;
        *(sQ_91A8*)&self->mRider->mPosition =
            vu0Add_91A8(vu0Scale_91A8(vu0Sub_91A8(*(sQ_91A8*)&self->InitPos, *(sQ_91A8*)&self->SplinePos), progress * (progress - 2.0f)), *(sQ_91A8*)&self->InitPos);
        self->mRider->mPosition.w = 1.0f;
        sQ_91A8 deltaRot;
        {
            float sinHalf, cosHalf;
            fSinCos(&sinHalf, &cosHalf, self->SnapRotationRate * step * 0.5f);
            deltaRot = sQ_91A8(sinHalf * self->SnapRotationAxis.x, sinHalf * self->SnapRotationAxis.y, sinHalf * self->SnapRotationAxis.z, cosHalf);
        }
        *(sQ_91A8*)&self->mRider->mRotation = QuatMul_91A8(deltaRot, *(sQ_91A8*)&self->mRider->mRotation);
    } else {
        *(sQ_91A8*)&self->mRider->mPosition = *(sQ_91A8*)&self->SplinePos;
        *(sQ_91A8*)&self->mRider->mVelocity = (const sQ_91A8&)t4vect_Zero_91A8;
        if (eq_91A8((self), (const sQ_91A8&)t4vect_Zero_91A8)) {
            if (cHandplantMotion_calcExitVel((self))) {
                *(sQ_91A8*)&self->AirPredictorPos = vu0Add_91A8(*(sQ_91A8*)&self->SplinePos, vu0ScaleP_91A8((const sQ_91A8*)&t4vect_ZAxis_91A8, 5.0f));
                *(sQ_91A8*)&self->AirPredictorVel = *(sQ_91A8*)&self->ExitVel;
                cAirPredictor_startLaunchIntoAir(self->mRider->mAirPredictor, (sQ_91A8*)&self->AirPredictorPos, (sQ_91A8*)&self->AirPredictorVel,
                                                 3333.33349609375f);
                for (int i = 0; i < 8 || !landed_91A8((char*)self->mRider->mAirPredictor); i++)
                    cAirPredictor_updateFlightPath(self->mRider->mAirPredictor, (sQ_91A8*)&self->AirPredictorPos, (sQ_91A8*)&self->AirPredictorVel, 0.01666666753590107f);
            }
        } else {
            if (landed_91A8((char*)self->mRider->mAirPredictor)) {
                cAirPredictor* ap = self->mRider->mAirPredictor;
                if (0.0f < vu0Dot_91A8(*(sQ_91A8*)&ap->GuessLandNormal, *(sQ_91A8*)&self->ExitVel))
                    cRider_alignRiderAxis((char*)self->mRider, &ap->GuessLandNormal, &ap->GuessLandVelocity, self->mRider->mTimeScale + self->mRider->mTimeScale,
                                  self->mRider->mTimeScale * 6.632251739501953f);
            } else {
                cAirPredictor_updateFlightPath(self->mRider->mAirPredictor, (sQ_91A8*)&self->AirPredictorPos, (sQ_91A8*)&self->AirPredictorVel, 0.01666666753590107f);
            }
        }
    }
    cRider_updateOrientationImplicit((char*)self->mRider);
}
#endif

extern "C" void* cRider_updateBoundBox(int, int);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion_resolve__FPv);
#ifdef SKIP_ASM
/**
 * Resolves the handplant motion: only refreshes the rider's bounds with cRider_updateBoundBox(rider, 0).
 *
 * @return cRider_updateBoundBox()'s result, declared void* here; no meaningful value (On Tour's resolve
 *         returns nothing)
 */
void* cHandplantMotion_resolve(void* vself)
{
    cHandplantMotion* self = (cHandplantMotion*)vself;
    return cRider_updateBoundBox((int)self->mRider, 0);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion_calcExitVel);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
extern t4Vector t4vect_Zero_9548 __asm__("t4vect_Zero");
extern t4Vector t4vect_ZAxis_9548 __asm__("t4vect_ZAxis");


static inline int neq_9548(cHandplantMotion* a, const t4Vector& b)
{
    return a->ExitVel.x != b.x || a->ExitVel.y != b.y || a->ExitVel.z != b.z || a->ExitVel.w != b.w;
}

// PORT: PS2-only VU0 inline asm (dst = a x b, w = 0).
static inline void vu0CrossTo_9548(t4Vector& dst, const t4Vector& a, const t4Vector& b)
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
        : "o"(a), "o"(b)
        : "memory");
    dst = r;
}

// PORT: PS2-only VU0 inline asm (dst = normalized v via rsqrt).
static inline void vu0NormTo_9548(t4Vector& dst, const t4Vector& v)
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
    dst = r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_9548(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (dst = v * s).
static inline void vu0ScaleTo_9548(t4Vector& dst, const t4Vector& v, float s)
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
    dst = r;
}

// PORT: PS2-only VU0 inline asm (v *= s).
static inline void vu0ScaleEq_9548(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(v), "=&r"(t)
        : "m"(v), "f"(s));
}

// PORT: PS2-only VU0 inline asm (dst = a - b).
static inline void vu0SubTo_9548(t4Vector& dst, const t4Vector& a, const t4Vector& b)
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
    dst = r;
}

extern "C" int cHandplantMotion_calcExitVel_x(cHandplantMotion* s) __asm__("cHandplantMotion_calcExitVel");
/**
 * Works out the handplant's exit velocity (ExitVel) from the rider's HandplantMode; returns whether it is set.
 *
 * Returns 1 at once when ExitVel is already non-zero. Speeds are in km/h (x 27.78). Mode 3: SplineDir times 0.7 x
 * InitTangentSpeed, minus 2 km/h (15 with IsRailSpline) along the horizontal normal of SplineDir signed by ApproachDir,
 * and a vertical 0 (10) km/h. Modes 4 and 5: 5 (15) km/h along the horizontal normal of SplineDir, turned toward
 * ApproachDir in mode 4 and away from it in mode 5, and a vertical 0 (5) km/h. Mode 6: SplineDir x 30 km/h plus 15 km/h
 * upward. The vertical value overwrites ExitVel.z in modes 3-5. Other modes return 0.
 *
 * @param handplant  the cHandplantMotion (this)
 * @return 1 when ExitVel is set, else 0
 */
extern "C" int cHandplantMotion_calcExitVel_x(cHandplantMotion* handplant)
{
    if (neq_9548(handplant, t4vect_Zero_9548))
        return 1;
    int kind = handplant->mRider->HandplantMode;
    t4Vector n;
    if (kind == 3) {
        t4Vector exitVel;
        vu0CrossTo_9548(exitVel, t4vect_ZAxis_9548, handplant->SplineDir);
        vu0NormTo_9548(n, exitVel);
        float tangentSpeed = handplant->InitTangentSpeed * 0.699999988079071f;
        float k = 2.0f;
        if (handplant->IsRailSpline != 0)
            k = 15.0f;
        float approachDot = vu0Dot_9548(handplant->ApproachDir, n);
        float sideSpeed = k * 27.77777862548828f;
        if (approachDot < 0.0f)
            sideSpeed = -sideSpeed;
        t4Vector alongVel;
        vu0ScaleTo_9548(alongVel, handplant->SplineDir, tangentSpeed);
        t4Vector sideVel;
        vu0ScaleTo_9548(sideVel, n, sideSpeed);
        vu0SubTo_9548(exitVel, alongVel, sideVel);
        *(t4Vector*)&handplant->ExitVel.x = exitVel;
        float z = 0.0f;
        if (handplant->IsRailSpline != 0)
            z = 10.0f;
        handplant->ExitVel.z = z * 27.77777862548828f;
        return 1;
    }
    if (kind == 4) {
        t4Vector exitVel;
        vu0CrossTo_9548(exitVel, handplant->SplineDir, t4vect_ZAxis_9548);
        vu0NormTo_9548(n, exitVel);
        float z = 0.0f;
        float approachDot = vu0Dot_9548(n, handplant->ApproachDir);
        if (approachDot < z)
            vu0ScaleEq_9548(n, -1.0f);
        float k = 5.0f;
        if (handplant->IsRailSpline != 0)
            k = 15.0f;
        float kphScale = 27.77777862548828f;
        vu0ScaleTo_9548(exitVel, n, k * kphScale);
        *(t4Vector*)&handplant->ExitVel.x = exitVel;
        if (handplant->IsRailSpline != 0)
            z = 5.0f;
        k = z * kphScale;
        handplant->ExitVel.z = k;
        return 1;
    }
    if (kind == 5) {
        t4Vector exitVel;
        vu0CrossTo_9548(exitVel, handplant->SplineDir, t4vect_ZAxis_9548);
        vu0NormTo_9548(n, exitVel);
        float z = 0.0f;
        float approachDot = vu0Dot_9548(n, handplant->ApproachDir);
        if (z < approachDot)
            vu0ScaleEq_9548(n, -1.0f);
        float k = 5.0f;
        if (handplant->IsRailSpline != 0)
            k = 15.0f;
        float kphScale = 27.77777862548828f;
        vu0ScaleTo_9548(exitVel, n, k * kphScale);
        *(t4Vector*)&handplant->ExitVel.x = exitVel;
        if (handplant->IsRailSpline != 0)
            z = 5.0f;
        k = z * kphScale;
        handplant->ExitVel.z = k;
        return 1;
    }
    if (kind == 6) {
        vu0ScaleTo_9548(n, handplant->SplineDir, 833.3333740234375f);
        *(t4Vector*)&handplant->ExitVel.x = n;
        handplant->ExitVel.z += 416.66668701171875f;
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion_writeToReplayFrame);
#ifdef SKIP_ASM

/**
 * Writes the handplant state (its first 0xA0 bytes, cHandplantMotionState) to a replay frame.
 *
 * Calls the stream's write virtual (slot 1) with this object and the size 0xA0.
 *
 * @param obj  replay stream to write to (On Tour cReplayFramePtr*)
 */
extern "C" void cHandplantMotion_writeToReplayFrame(cHandplantMotion* self, cBxReplayFrame* obj)
{
    obj->write((const char*)self, 160);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion_readFromReplayFrame);
#ifdef SKIP_ASM

/**
 * Reads the handplant state (its first 0xA0 bytes, cHandplantMotionState) back from a replay frame.
 *
 * Calls the stream's read virtual (slot 2) with this object and the size 0xA0.
 *
 * @param obj  replay stream to read from (On Tour cReplayFramePtr*)
 */
extern "C" void cHandplantMotion_readFromReplayFrame(cHandplantMotion* self, cBxReplayFrame* obj)
{
    obj->read((char*)self, 160);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion_gainFocus);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);

/**
 * Starts the air motion: launches the air predictor from the rider's current position and velocity.
 *
 * IsLandVelAlign is set when the motion being left is MS_Handplant (5); cAirMotion_step() then also lines the
 * rider's heading up with the predicted landing velocity. The predictor's terminal speed is 3333.33.
 *
 * @param state  the motion state being left (eMotionState; On Tour prev)
 */
extern "C" void cAirMotion_gainFocus(cAirMotion* self, int state)
{
    *(int*)self = state == 5;
    cRider* rider = self->mRider;
    cAirPredictor_startLaunchIntoAir(rider->mAirPredictor, &rider->mPosition, &rider->mVelocity, 3333.33349609375f);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when the air motion loses focus; the body is empty.
 */
void cAirMotion_loseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion_step);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void cAirPredictor_updateFlightPath(void* ap, void* pos, void* vel, float dt);
extern "C" void cRider_alignRiderAxis(void* rider, void* a, void* b, float x, float y);
extern "C" void cRider_updateWind(char* self);
int cRider_getControlState(void* rider);
extern void* t4vect_Zero[];

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void vu0ScaleEq_139A20(sVec4HP& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(v), "=&r"(t)
        : "m"(v), "f"(s));
}

// PORT: PS2-only VU0 inline asm (4-component dot product, by-value second operand).
static inline float vu0Dot_139A20(const sVec4HP& a, sVec4HP b)
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

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_139A20(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Moves the rider one frame along the air predictor's path and turns it to meet the predicted landing.
 *
 * Applies wind (cRider_updateWind), steps position and velocity with cAirPredictor_updateFlightPath() by mTimeScale / 60, sets
 * mGroundNormal to the rider's posed up vector (+0x180) and the ground velocity (+0x3D0) to zero. When the
 * predictor has a landing (state 1 or 3) on a triangle with flag 0x8, on a surface that is neither type 0x12
 * nor a reset surface (+0x44), it turns the rider's up toward the landing normal, paced to finish around
 * touchdown (rate = time scale / time to landing, at most 80 degrees/s). In CS_Spin with |AirAdjSpin| above 5
 * degrees only the up axis turns; otherwise the heading also follows the landing velocity (flipped to the
 * rider's front) when the landing is steep (normal z < 0.3) or IsLandVelAlign is set.
 */
extern "C" void cAirMotion_step(cAirMotion* self)
{
    cRider_updateWind((char*)self->mRider);
    cRider* rider = self->mRider;
    cAirPredictor_updateFlightPath(rider->mAirPredictor, &rider->mPosition, &rider->mVelocity, rider->mTimeScale * 0.01666666753590107f);
    cRider* rider1 = self->mRider;
    {
        sVec4HP up = *(sVec4HP*)&rider1->RiderRoot.row[2];
        *(sVec4HP*)&rider1->mGroundNormal = up;
    }
    *(sVec4HP*)&self->mRider->GroundVelocity = *(sVec4HP*)t4vect_Zero;
    int st = self->mRider->mAirPredictor->SimState;
    int ok = 0;
    if (st == 1 || st == 3) ok = 1;
    if (ok) {
        cRider* rider2 = self->mRider;
        cAirPredictor* ap = rider2->mAirPredictor;
        int idx = ap->GuessLandSurface;
        tSurfData* snow = &SSXApp->Game->SurfPropCache->SurfData[idx];
        int flag = 0;
        short sf = ap->SurfaceFlags;
        if (sf != -1) {
            int orientableBit = sf & 8;
            flag = orientableBit != 0;
        }
        if (idx != 18 && snow->IsResetSurface == 0 && flag) {
            float timeLeft = ap->TotalAirTime - ap->CurAirTime;
            float timeScale = rider2->mTimeScale;
            float x;
            if (timeLeft >= 0.01666666753590107f)
                x = timeScale / timeLeft;
            else
                x = timeScale * 59.999996185302734f;
            float y = self->mRider->mTimeScale * 1.396263599395752f;
            if (cRider_getControlState(self->mRider) == 5) {
                cRider* rider3 = self->mRider;
                int big = 1;
                if (!(0.0872664749622345f < absf_139A20(rider3->mRiderHandler->SpinControl.AirAdjSpin))) big = 0;
                if (big) {
                    cRider_alignRiderAxis(rider3, &rider3->mAirPredictor->GuessLandNormal, t4vect_Zero, x, y);
                    goto done;
                }
            }
            cRider* rider4 = self->mRider;
            cAirPredictor* ap4 = rider4->mAirPredictor;
            char* tgt = (char*)&ap4->GuessLandNormal;
            if (ap4->GuessLandNormal.z < 0.30000001192092896f || *(int*)self != 0) {
                sVec4HP frontAlign = *(sVec4HP*)&ap4->GuessLandVelocity;
                if (vu0Dot_139A20(frontAlign, *(sVec4HP*)&rider4->mMatrix.row[1]) < 0.0f) {
                    vu0ScaleEq_139A20(frontAlign, -1.0f);
                }
                cRider* rider5 = self->mRider;
                cRider_alignRiderAxis(rider5, &rider5->mAirPredictor->GuessLandNormal, &frontAlign, x, y);
            } else {
                cRider_alignRiderAxis(rider4, tgt, t4vect_Zero, x, y);
            }
        }
    }
done:
    cRider_updateOrientationImplicit(self->mRider);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion_resolve);
#ifdef SKIP_ASM

struct sInfo_139C88 {
    sVec4HP pos;                // 0x00
    sVec4HP dir;                // 0x10
    sVec4HP up;                 // 0x20
    float speed;                // 0x30
} __attribute__((aligned(16)));

struct sVEnt_139C88 { short delta; short index; void (*fn)(void*, tIntersectInfo*, const sVec4HP&); };

extern int kInAirSphereMask;
extern void* cWScriptMan_TheWScriptMan;
extern "C" float cAirMotion_intersectBoard_9C88(void* self, tIntersectInfo* hit) __asm__("cAirMotion_intersectBoard");
extern "C" void cRider_updateBoundBox_9C88(void* rider, int a) __asm__("cRider_updateBoundBox");
extern "C" void cRider_checkForReset(void* rider, int a, int b);
extern "C" void cRider_moveFromCollision(void* rider, void* v);
extern "C" void cRider_checkObjectCollisions(void* rider, int a1);
extern "C" void cRider_checkObjectCollisions_p(void* rider, void* v) __asm__("cRider_checkObjectCollisions");
extern "C" void cRider_checkOpponentCollisions(void* rider);
extern "C" void cRider_flagEventWipeout_9C88(void* rider, int anim, int a2, int a3, sInfo_139C88* info) __asm__("cRider_flagEventWipeout");
extern "C" void cRider_flagEventLand(void* rider, int a1, int a2, int a3, tIntersectInfo* hit, float spd);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void* ra, int layer);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void* ra, int layer);
extern "C" bool tEventFlags_isSet1(tEventFlags* self, int bit);
extern "C" int tEventFlags_isSet1_i(void* self, int bit) __asm__("tEventFlags_isSet1");
extern "C" float cBEStatInterface_getLandingAttrib(void* be, int idx, int stat);
extern "C" void cRider_undoRailslideModeOrientation(void* rider);
extern "C" int cRider_isChangeFakieGround(void* rider);
extern "C" void cAirMotion_chooseFakieLandAnim(cAirMotion* self, float v);
extern "C" void cAirMotion_chooseLandAnim(cAirMotion* self, float v);
extern "C" int cAirMotion_checkTerrainCollisions(cAirMotion* self, t4Vector* out);
extern "C" void cWScriptMan_eventLand(void* p);
extern "C" void* cBE_getBE();
extern "C" int cRiderAnimBase_play_i(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cRider_getControlState(void* rider);
void cRider_setControlState_9C88(void* rider, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_9C88(void* rider, int v) __asm__("cRider_setMotionState__FPv");
void cAirPredictor_land_9C88(void* ap, void* pos, void* vel) __asm__("cAirPredictor_land__FPv");
int AIrand();

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4HP vu0Scale_139C88(const sVec4HP& v, float s)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void vu0ScaleEq_139C88(sVec4HP& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(v), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4HP vu0Sub_139C88(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void vu0AddEq_139C88(sVec4HP& a, const sVec4HP& b)
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
static inline float vu0Dot_139C88(const sVec4HP& a, const sVec4HP& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Len_139C88(const sVec4HP& v)
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
static inline sVec4HP vu0Normalize_139C88(const sVec4HP& v)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sVec4HP vu0Cross_139C88(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
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

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_139C88(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ minimum operator (<?).
static inline float clamp_139C88(float v, float lo, float hi)
{
    float r;
    if (v >= lo) {
        r = v <? hi;
    } else {
        r = lo;
    }
    return r;
}

// Row k of the rider's transform (side, front, up), by value.
static inline sVec4HP row_139C88(char* r, int k)
{
    return *(sVec4HP*)(r + 0x1A0 + k * 16);
}

#define RIDER_139C88 (*(char**)((char*)self + 0x4))
#define V4_139C88(p, off) (*(sVec4HP*)((p) + (off)))

/**
 * Resolves the air motion: lands or crashes the rider when the board meets a surface, then checks collisions.
 *
 * A touchdown needs a board probe hit (cAirMotion_intersectBoard) at fraction >= 0.5 with the velocity
 * relative to the surface heading into it; CS_Spin then drops to CS_Void. Reset surfaces (+0x44) and
 * triangles with flag 0x2 force an auto reset. Otherwise the rider is pushed out, its ground frame and
 * contact data come from the hit, the inward velocity is cancelled times the surface factor +0x3C (the rest
 * capped at +0x40) and the hit object gets the impulse (handler slot 43). A crash (cRider_flagEventWipeout) follows a
 * layer-2 animation of category 0x12-0x15 whose sequence flags say so (probably an unfinished trick), an
 * upside-down board, or too much fore/aft tilt for a stat-scaled limit; otherwise cRider_flagEventLand() takes the
 * landing, the motion becomes MS_Ground, and the control state and landing animation follow the press type
 * and current control. Terrain (cAirMotion_checkTerrainCollisions), cRider_checkObjectCollisions() and opponents are checked
 * every frame.
 */
extern "C" void cAirMotion_resolve(cAirMotion* self)
{
    tIntersectInfo hit;
    float intParam = cAirMotion_intersectBoard_9C88(self, &hit);
    float vn = 0.0f;
    if (intParam >= 0.5f) {
        vn = vu0Dot_139C88(vu0Sub_139C88(V4_139C88(RIDER_139C88, 0x1E0), (*(sVec4HP*)&hit.Ivel)), (*(sVec4HP*)&hit.Inorm));
    }
    if (intParam >= 0.5f && vn < 0.0f) {
        float spd = vu0Len_139C88(V4_139C88(RIDER_139C88, 0x1E0));
        if (cRider_getControlState(RIDER_139C88) == 5) {
            cRider_setControlState_9C88(RIDER_139C88, 13);
        }
        cAirPredictor_land_9C88(*(void**)(RIDER_139C88 + 0x788), RIDER_139C88 + 0x110, RIDER_139C88 + 0x1E0);
        tSurfData* surf = &SSXApp->Game->SurfPropCache->SurfData[hit.Stype];
        int fl;
        if (surf->IsResetSurface != 0 || (((char*)hit.Patch) != 0 && (fl = *(short*)(((char*)hit.Patch) + 0xA), (fl & 2) != 0))) {
            cRider_checkForReset(RIDER_139C88, 0, 1);
            return;
        }
        float pen = vu0Dot_139C88(vu0Sub_139C88((*(sVec4HP*)&hit.Ipos), V4_139C88(RIDER_139C88, 272)), (*(sVec4HP*)&hit.Inorm)) - surf->PowderDepth3G;
        if (0.0f < pen) {
            sVec4HP push = vu0Scale_139C88((*(sVec4HP*)&hit.Inorm), pen);
            cRider_moveFromCollision(RIDER_139C88, &push);
        }
        V4_139C88(RIDER_139C88, 880) = (*(sVec4HP*)&hit.Inorm);
        V4_139C88(RIDER_139C88, 976) = (*(sVec4HP*)&hit.Ivel);
        *(float*)(RIDER_139C88 + 0x770) = -vn;
        V4_139C88(RIDER_139C88, 0x380) = V4_139C88(RIDER_139C88, 880);
        if (((char*)hit.Patch) != 0) {
            *(int*)(RIDER_139C88 + 0x430) = *(int*)(((char*)hit.Patch) + 0x150);
            *(float*)(RIDER_139C88 + 0xAAC) = hit.u;
            *(float*)(RIDER_139C88 + 0xAB0) = hit.v;
        } else {
            *(unsigned int*)(RIDER_139C88 + 0x430) = 0xFFFFFFFF;
        }
        sVec4HP imp = vu0Scale_139C88(V4_139C88(RIDER_139C88, 880), -vn);
        vu0ScaleEq_139C88(imp, surf->LandingHardness);
        vu0AddEq_139C88(V4_139C88(RIDER_139C88, 0x1E0), imp);
        char* r = RIDER_139C88;
        float vn2 = -vu0Dot_139C88(V4_139C88(r, 0x1E0), V4_139C88(r, 880));
        if (surf->MaxLandingPenetrationVel < vn2) {
            vu0AddEq_139C88(V4_139C88(RIDER_139C88, 0x1E0), vu0Scale_139C88(V4_139C88(r, 880), vn2 - surf->MaxLandingPenetrationVel));
        }
        if (((char*)hit.Inst) != 0) {
            char* handler = (char*)hit.Inst->m_pActiveNode;
            if (handler != 0) {
                sVEnt_139C88* vt = *(sVEnt_139C88**)(handler + 0xC);
                vt[43].fn(vthis(handler, vt[43]), &hit, vu0Scale_139C88(vu0Scale_139C88(imp, -1.0f), (float)SSXApp->TimerHz));
            }
        }
        int anim = 438;
        char* r2 = RIDER_139C88;
        V4_139C88(r2, 928) = vu0Normalize_139C88(vu0Sub_139C88(row_139C88(r2, 1), vu0Scale_139C88(V4_139C88(r2, 880), vu0Dot_139C88(row_139C88(r2, 1), V4_139C88(r2, 880)))));
        V4_139C88(RIDER_139C88, 944) = vu0Cross_139C88(V4_139C88(RIDER_139C88, 880), V4_139C88(RIDER_139C88, 928));
        *(int*)(RIDER_139C88 + 0x438) = hit.Stype;
        V4_139C88(RIDER_139C88, 1120) = (*(sVec4HP*)&hit.Ipos);
        *(float*)(RIDER_139C88 + 0x454) = vu0Dot_139C88(vu0Sub_139C88(V4_139C88(RIDER_139C88, 272), (*(sVec4HP*)&hit.Ipos)), V4_139C88(RIDER_139C88, 880));
        if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(RIDER_139C88 + 0x784), 2) == 20) {
            if (!tEventFlags_isSet1((tEventFlags*)(cRiderAnimBase_getCurrentAnimSequence(*(void**)(RIDER_139C88 + 0x784), 2) + 0xB0), 2)) anim = 361;
        } else if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(RIDER_139C88 + 0x784), 2) == 21) {
            if (!tEventFlags_isSet1((tEventFlags*)(cRiderAnimBase_getCurrentAnimSequence(*(void**)(RIDER_139C88 + 0x784), 2) + 0xB0), 3)) anim = 361;
        } else {
            int flip = 0;
            if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(RIDER_139C88 + 0x784), 2) == 19) {
                flip = tEventFlags_isSet1_i(cRiderAnimBase_getCurrentAnimSequence(*(void**)(RIDER_139C88 + 0x784), 2) + 0xB0, 2) == 0;
            } else if (cRiderAnimBase_getCurrentAnimCategory(*(void**)(RIDER_139C88 + 0x784), 2) == 18 &&
                       !tEventFlags_isSet1((tEventFlags*)(cRiderAnimBase_getCurrentAnimSequence(*(void**)(RIDER_139C88 + 0x784), 2) + 0xB0), 2)) {
                flip = tEventFlags_isSet1_i(cRiderAnimBase_getCurrentAnimSequence(*(void**)(RIDER_139C88 + 0x784), 2) + 0xB0, 0) != 0;
            }
            char* r4 = RIDER_139C88;
            if (vu0Dot_139C88(row_139C88(r4, 2), V4_139C88(r4, 880)) < 0.0f) {
                flip = 1;
            }
            if (flip) {
                float sideDot = vu0Dot_139C88(row_139C88(r4, 0), V4_139C88(r4, 880));
                float frontDot = vu0Dot_139C88(row_139C88(r4, 1), V4_139C88(r4, 880));
                float upDot = vu0Dot_139C88(row_139C88(r4, 2), V4_139C88(r4, 880));
                if (absf_139C88(frontDot) > absf_139C88(sideDot) && absf_139C88(frontDot) > absf_139C88(upDot)) {
                    anim = frontDot < 0.0f ? 354 : 355;
                } else if (absf_139C88(upDot) > absf_139C88(sideDot) && absf_139C88(upDot) > absf_139C88(frontDot)) {
                    if (upDot < 0.0f) {
                        anim = 350;
                    } else {
                        int k;
                        if (vu0Len_139C88(V4_139C88(RIDER_139C88, 0x1E0)) > 2222.22216796875f) {
                            anim = 352;
                            k = AIrand() & 3;
                        } else {
                            anim = 352;
                            k = AIrand() & 1;
                        }
                        if (k != 0) {
                            anim = k == 1 ? 353 : 351;
                        }
                    }
                } else {
                    anim = sideDot < 0.0f ? 357 : 356;
                }
            } else if (*(int*)(r4 + 0x330) == 0) {
                float tiltDot = vu0Dot_139C88(row_139C88(r4, 2), V4_139C88(r4, 928));
                float k = cBEStatInterface_getLandingAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), *(int*)(RIDER_139C88 + 0x86C), *(int*)(RIDER_139C88 + 0xB34)) * 0.29999101161956787f + 1.0f;
                if (k * 0.699962317943573f < tiltDot) {
                    anim = 348;
                } else if (tiltDot < k * -0.699962317943573f) {
                    anim = 349;
                }
            }
        }
        if (anim != 438) {
            sInfo_139C88 info;
            info.pos = (*(sVec4HP*)&hit.Ipos);
            info.dir = vu0Normalize_139C88(V4_139C88(RIDER_139C88, 0x1E0));
            info.up = (*(sVec4HP*)&hit.Inorm);
            info.speed = absf_139C88(vn);
            cRider_flagEventWipeout_9C88(RIDER_139C88, anim, 0, *(int*)(RIDER_139C88 + 0x438), &info);
        } else {
            cRider* rider5 = (cRider*)(RIDER_139C88);
            cRider_flagEventLand(rider5, rider5->mGroundNormal.z < 0.699999988079071f, 0, rider5->PressMode, &hit, spd);
            cRider_setMotionState_9C88(RIDER_139C88, 0);
            if (cRider_getControlState(RIDER_139C88) != 0xC) {
                if (cRider_getControlState(RIDER_139C88) == 2) {
                    cRider_undoRailslideModeOrientation(RIDER_139C88);
                    if (vn < -1388.888916015625f) {
                        cAirMotion_chooseLandAnim(self, vn);
                    }
                } else if (*(int*)(RIDER_139C88 + 0x330) != 0) {
                    if (cRider_getControlState(RIDER_139C88) != 1) {
                        if (*(int*)(RIDER_139C88 + 0x330) == 1) {
                            cRiderAnimBase_play_i(*(void**)(RIDER_139C88 + 0x784), 26, 0, -1.0f);
                        } else {
                            cRiderAnimBase_play_i(*(void**)(RIDER_139C88 + 0x784), 34, 0, -1.0f);
                        }
                        cRider_setControlState_9C88(RIDER_139C88, 1);
                    }
                } else {
                    cRider_undoRailslideModeOrientation(RIDER_139C88);
                    cRider_setControlState_9C88(RIDER_139C88, 0);
                    if (cRider_isChangeFakieGround(RIDER_139C88)) {
                        cAirMotion_chooseFakieLandAnim(self, vn);
                    } else {
                        cAirMotion_chooseLandAnim(self, vn);
                    }
                }
            }
            char* r6 = RIDER_139C88;
            float k = clamp_139C88(absf_139C88(vu0Dot_139C88(row_139C88(r6, 1), vu0Normalize_139C88(V4_139C88(r6, 0x1E0)))), 1.0f, 1.0f);
            vu0ScaleEq_139C88(V4_139C88(RIDER_139C88, 0x1E0), k);
            cWScriptMan_eventLand(cWScriptMan_TheWScriptMan);
        }
    }
    cRider_updateBoundBox_9C88(RIDER_139C88, 0);
    sVec4HP out;
    *(int*)(*(char**)(RIDER_139C88 + 0xAA0) + 0x28) = kInAirSphereMask;
    int got = cAirMotion_checkTerrainCollisions(self, (t4Vector*)(&out));
    *(unsigned int*)(*(char**)(RIDER_139C88 + 0xAA0) + 0x28) = 0xFFFFFFFF;
    if (got) {
        cRider_checkObjectCollisions_p(RIDER_139C88, &out);
    } else {
        cRider_checkObjectCollisions_p(RIDER_139C88, 0);
    }
    cRider_checkOpponentCollisions(RIDER_139C88);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion_intersectBoard);
#ifdef SKIP_ASM
extern "C" void* cWorldLine_cWorldLine(void* seg, const sVec4HP& a, const sVec4HP& b, int n, float r);
extern "C" float cOverlapList_terrainIntersect1(void* world, void* seg, void* hit, int flags);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4HP vu0Scale_13A7B0(const sVec4HP& v, float s)
{
    sVec4HP r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4HP vu0Add_13A7B0(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4HP vu0Sub_13A7B0(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}


/**
 * Probes for a surface along the rider's up axis through the board, filling a hit record.
 *
 * Casts a capsule (radius 0.575) from 200 units below to 200 units above the board-root bone (mBoneBoardRoot)
 * along the rider's posed up vector (+0x180) against its overlap list with the line hint (mLineHint). A hit
 * whose object has a handler (+0xC) is reported to it (handler virtual slot 42).
 *
 * @param hit  hit record to fill (position, normal, velocity, surface type, objects)
 * @return the hit's fraction along the probe (0.5 is the bone, so >= 0.5 means the surface is at or above
 *         it), or a negative value when nothing was hit
 */
extern "C" float cAirMotion_intersectBoard(cAirMotion* self, char* hit)
{
    cRider* rider = self->mRider;
    sVec4HP* pt = (sVec4HP*)&rider->mAnimModel->bones[rider->mBoneBoardRoot];
    sVec4HP dir = *(sVec4HP*)&rider->RiderRoot.row[2];
    char seg[176];
    cWorldLine_cWorldLine(seg, vu0Sub_13A7B0(*pt, vu0Scale_13A7B0(dir, 200.0f)), vu0Add_13A7B0(*pt, vu0Scale_13A7B0(dir, 200.0f)), 2, 0.574999988079071f);
    cRider* rider2 = self->mRider;
    float hitParam = cOverlapList_terrainIntersect1(rider2->mOverlapList, seg, hit, (int)rider2->mLineHint);
    if (hitParam >= 0.0f)
    {
        char* hitObject = *(char**)(hit + 0x50);
        if (hitObject)
        {
            char* handler = *(char**)(hitObject + 0xC);
            if (handler)
            {
                sVEntry<void(void*, void*)>* vt = *(sVEntry<void(void*, void*)>**)(handler + 0xC);
                vt[42].fn(vthis(handler, vt[42]), hit);
            }
        }
    }
    return hitParam;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion_chooseFakieLandAnim);
#ifdef SKIP_ASM
// PORT: cRiderAnimBase_play is declared void elsewhere; callers here match only with an int return.
extern "C" int cRiderAnimBase_play_i(void* self, int anim, int flags, float blend) __asm__("cRiderAnimBase_play");

/**
 * Plays the landing animation for a fakie landing, hard or soft by the landing speed.
 *
 * Below -2222.22 it plays anim 0x41 and clears the rider's mSpinRate; otherwise it plays anim 0x40.
 *
 * @param normSpeed  landing velocity along the ground normal, negative into the ground (On Tour normSpeed)
 */
extern "C" void cAirMotion_chooseFakieLandAnim(cAirMotion* self, float normSpeed)
{
    if (normSpeed < -2222.22216796875f)
    {
        cRiderAnimBase_play_i(self->mRider->mRiderAnim, 65, 0, -1.0f);
        *(int*)&self->mRider->mSpinRate = 0;
    }
    else
    {
        cRiderAnimBase_play_i(self->mRider->mRiderAnim, 0x40, 0, -1.0f);
    }
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion_chooseLandAnim);
#ifdef SKIP_ASM
/**
 * Plays the landing animation for a regular (not fakie) landing from the landing speed and any spin-out.
 *
 * Below -2222.22 it plays anim 0x3F and clears mSpinRate. Otherwise a spin-out (|mSpinRate| > pi) picks 0x42
 * or 0x43 by its sign, else 0x3E below -1250 and 0x3D for a soft landing.
 *
 * @param normSpeed  landing velocity along the ground normal, negative into the ground (On Tour normSpeed)
 */
extern "C" void cAirMotion_chooseLandAnim(cAirMotion* self, float normSpeed)
{
    if (normSpeed < -2222.22216796875f)
    {
        cRiderAnimBase_play_i(self->mRider->mRiderAnim, 0x3F, 0, -1.0f);
        *(int*)&self->mRider->mSpinRate = 0;
    }
    else
    {
        cRider* rider = self->mRider;
        float pi = 3.1415929794311523f;
        float spinRate = rider->mSpinRate;
        if (spinRate < -pi)
            cRiderAnimBase_play_i(rider->mRiderAnim, 66, 0, -1.0f);
        else if (spinRate > pi)
            cRiderAnimBase_play_i(rider->mRiderAnim, 67, 0, -1.0f);
        else if (normSpeed < -1250.0f)
            cRiderAnimBase_play_i(rider->mRiderAnim, 0x3E, 0, -1.0f);
        else
            cRiderAnimBase_play_i(rider->mRiderAnim, 61, 0, -1.0f);
    }
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion_checkTerrainCollisions);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cRider_updateOrientationImplicit(void*);
int cRider_getControlState(void* rider);
extern "C" float cOverlapList_terrainIntersect1(void* world, void* seg, void* hit, int flags);
extern "C" void cRider_moveFromCollision(void* rider, void* v);
extern "C" void cRider_reactToCollision(void* rider, void* info, int id);
extern "C" void* cWorldRider_cWorldRider(void* self, void* tree, int flag);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);

struct sHit_AA48 {
    t4Vector pos;            // 0x00
    t4Vector normal;         // 0x10
    t4Vector v20;            // 0x20
    float pad30[4];         // 0x30
    float dist;             // 0x40
    int pad44[2];           // 0x44
    int m4C;                // 0x4C
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[5];           // 0x58
    float m6C;              // 0x6C
    float m70;              // 0x70
    int pad74[3];           // 0x74
} __attribute__((aligned(16)));

struct sSphere_AA48 {
    int state;              // 0x00
    int f4;                 // 0x04
    int f8;                 // 0x08
    int padC;               // 0x0C
    t4Vector dir;            // 0x10
    char rest[0x320 - 0x20];
} __attribute__((aligned(16)));

struct sInfo_AA48 {
    t4Vector pos;
    t4Vector dir;
    t4Vector normal;
    float depth;
} __attribute__((aligned(16)));

struct sVEnt_AA48 { short delta; short index; bool (*fn)(void*); };

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_AA48(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void vu0AddEq_AA48(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_AA48(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector vu0Normalize_AA48(const t4Vector& v)
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

static inline bool riderFlag_AA48(char* r)
{
    char* sub = r + 0x6C0;
    sVEnt_AA48* vt = *(sVEnt_AA48**)sub;
    return vt[8].fn(vthis(sub, vt[8]));
}

static inline t4Vector riderVec_AA48(char* r, int off)
{
    return *(t4Vector*)(r + off);
}

#define RIDER_AA48 (*(char**)((char*)self + 0x4))
#define RV_AA48(off) (*(t4Vector*)(RIDER_AA48 + (off)))

/**
 * Pushes the airborne rider out of terrain and deflects its velocity off the surface.
 *
 * Skipped in CS_Reset. A world query built from the rider's sphere tree (cWorldRider_cWorldRider) runs with its posed
 * up vector (+0x180) against the overlap list with the wall hint (mWallHint). On a hit the rider moves out by
 * 1.1 times the depth; if it is moving into the surface, the inward velocity is removed plus a bounce of
 * max(5% of it, 27.78) and the impact goes to cRider_reactToCollision() (rumble, possible crash or reset). The query is
 * destroyed with cWorldRider__cWorldRider().
 *
 * @param outp  receives the surface normal when the function returns 1
 * @return 1 when the rider hit terrain while moving into it, otherwise 0 (also 0 when it was only pushed out)
 */
extern "C" int cAirMotion_checkTerrainCollisions(cAirMotion* self, t4Vector* outp)
{
    if (cRider_getControlState(RIDER_AA48) == 9)
        return 0;
    sHit_AA48 hit;
    sSphere_AA48 sph;
    char* rider = RIDER_AA48;
    cWorldRider_cWorldRider(&sph, *(void**)(rider + 0xAA0), !riderFlag_AA48(rider));
    sph.dir = riderVec_AA48(RIDER_AA48, 0x180);
    sph.f8 = 1;
    float penetration = cOverlapList_terrainIntersect1(*(void**)(RIDER_AA48 + 0x860), &sph, &hit, *(int*)(RIDER_AA48 + 0x868));
    if (penetration < 0.0f)
        { cWorldRider__cWorldRider(&sph, 2); return 0; }
    {
        t4Vector push = vu0Scale_AA48(vu0Scale_AA48(hit.normal, 1.100000023841858f), penetration);
        cRider_moveFromCollision(RIDER_AA48, &push);
        float dot2 = vu0Dot_AA48(vu0Scale_AA48(hit.normal, -1.0f), RV_AA48(0x1E0));
        if (dot2 < 0.0f)
            { cWorldRider__cWorldRider(&sph, 2); return 0; }
        t4Vector n = vu0Normalize_AA48(RV_AA48(0x1E0));
        float k = dot2 * 0.05000000074505806f < 27.77777862548828f ? 27.77777862548828f : dot2 * 0.05000000074505806f;
        vu0AddEq_AA48(RV_AA48(0x1E0), vu0Scale_AA48(hit.normal, dot2 + k));
        cRider_updateOrientationImplicit(RIDER_AA48);
        sInfo_AA48 info;
        info.pos = hit.pos;
        info.dir = n;
        info.normal = hit.normal;
        info.depth = dot2;
        cRider_reactToCollision(RIDER_AA48, &info, hit.m4C);
        *(t4Vector*)outp = hit.normal;
    }
    cWorldRider__cWorldRider(&sph, 2);
    return 1;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion_writeToReplayFrame);
#ifdef SKIP_ASM

/**
 * Writes the air-motion state (its first 4 bytes, IsLandVelAlign) to a replay frame.
 *
 * Calls the stream's write virtual (slot 1) with this object and the size 4.
 *
 * @param obj  replay stream to write to (On Tour cReplayFramePtr*)
 */
extern "C" void cAirMotion_writeToReplayFrame(cAirMotion* self, cBxReplayFrame* obj)
{
    obj->write((const char*)self, 0x4);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion_readFromReplayFrame);
#ifdef SKIP_ASM

/**
 * Reads the air-motion state (its first 4 bytes, IsLandVelAlign) back from a replay frame.
 *
 * Calls the stream's read virtual (slot 2) with this object and the size 4.
 *
 * @param obj  replay stream to read from (On Tour cReplayFramePtr*)
 */
extern "C" void cAirMotion_readFromReplayFrame(cAirMotion* self, cBxReplayFrame* obj)
{
    obj->read((char*)self, 0x4);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideMotion_gainFocus);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void cRider_doLeanPoseAdjust(void* self, void* q);

/**
 * Starts the rail motion: resets the rider's lean channels and the rail-slide state.
 *
 * Runs cRider_doLeanPoseAdjust() on the position and rebuilds the orientation, zeroes slewed channels 0
 * (edging), 2 (braking) and 8, sets channel 9's rate to 0.05 and target to 0, clears mSpinRate,
 * RailLeanAngle, RailTime, RailOffCenter, ForceNaturalAir, CrookedSlideAngle and StuckSec, and starts PrevPos
 * at the rider's position.
 */
extern "C" void cRailslideMotion_gainFocus(cRailslideMotion* self)
{
    cRider* rider = self->mRider;
    cRider_doLeanPoseAdjust(rider, &rider->mPosition);
    cRider_updateOrientationImplicit(self->mRider);
    cRider* rider1 = self->mRider;
    // G1
    rider1->mSlewed[0].target = 0.0f;
    rider1->mSlewed[0].cur = 0.0f;
    rider1->mSlewed[0].rate = 0.0f;
    // E1
    cRider* rider2 = self->mRider;
    rider2->mSlewed[2].target = 0.0f;
    rider2->mSlewed[2].cur = 0.0f;
    rider2->mSlewed[2].rate = 0.0f;
    cRider* rider3 = self->mRider;
    rider3->mSlewed[8].target = 0.0f;
    rider3->mSlewed[8].cur = 0.0f;
    rider3->mSlewed[8].rate = 0.0f;
    *(int*)&self->RailLeanAngle = 0;
    *(int*)&self->RailTime = 0;
    *(int*)&self->RailOffCenter = 0;
    *(int*)&self->mRider->mSpinRate = 0;
    cRider* rider4 = self->mRider;
    rider4->mSlewed[9].rate = 0.05000000447034836f;
    *(int*)&rider4->mSlewed[9].target = 0;
    self->ForceNaturalAir = 0;
    *(int*)&self->CrookedSlideAngle = 0;
    *(&self->PrevPos) = *(&self->mRider->mPosition);
    *(int*)&self->StuckSec = 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideMotion_setCrookedAngle);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" sVec4HP cRailslideMotion_getRailUpNorm(void* self, sVec4HP* p);
extern "C" float func_0031C228(float); // atan

// PORT: PS2-only VU0 inline asm (3D cross product, w = 0).
static inline sVec4HP vu0Cross_13ADC0(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (4-component dot product, by-value first operand).
static inline float vu0DotV_13ADC0(sVec4HP a, const sVec4HP& b)
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

// atan2(y, x) built on a single-argument atan.
static inline float Atan2_13ADC0(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f)
            return y;
        return y >= 0.0f ? 1.5707964f : -1.5707964f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f)
            r += 3.1415927f;
        else
            r -= 3.1415927f;
    }
    return r;
}

static inline float Clamp_13ADC0(float x, float lo, float hi)
{
    return (lo <= x) ? (x <? hi) : lo;
}

/**
 * Sets CrookedSlideAngle, the signed angle between the rider's front axis and the rail (at most 45 degrees).
 *
 * The angle is atan2 of the front axis's parts along (rail up x rail) and along the rail, with the rail's up
 * vector from cRailslideMotion_getRailUpNorm().
 *
 * @param railnorm  the rail direction (On Tour railnorm; the caller passes the rail hit's normal)
 */
// Signed angle (clamped to +-45 degrees) between the rider's velocity at +0x1B0 and the target direction p.
extern "C" void cRailslideMotion_setCrookedAngle(cRailslideMotion* self, sVec4HP* railnorm)
{
    sVec4HP upNorm = cRailslideMotion_getRailUpNorm(((char*)self), railnorm);
    sVec4HP sideNorm = vu0Cross_13ADC0(upNorm, *railnorm);
    cRider* rider = self->mRider;
    self->CrookedSlideAngle = Atan2_13ADC0(vu0DotV_13ADC0(*(sVec4HP*)&rider->mMatrix.row[1], sideNorm),
                                          vu0DotV_13ADC0(*(sVec4HP*)&rider->mMatrix.row[1], *railnorm));
    float lim = 0.7853982448577881f;
    self->CrookedSlideAngle = Clamp_13ADC0(self->CrookedSlideAngle, -lim, lim);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideMotion_step);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
extern "C" int cOverlapList_splineIntersect(void* world, t4Vector* from, void* hit, int flags, float radius);
extern "C" int cRider_isRailBoardIntersect(void* rider, t4Vector* tgt, t4Vector* dir);
extern "C" void* cBE_getBE();
extern "C" void* cBE_getInterface__Fv(void* be, int kind);
extern "C" float cBEStatInterface_getBalanceAttrib(void* self, int rider, int value);
// PORT: the unit declares cRailslideMotion_getRailUpNorm earlier as returning sVec4HP; same symbol.
extern "C" t4Vector cRailslideMotion_getRailUpNorm_AF28(char* p, const t4Vector& n) __asm__("cRailslideMotion_getRailUpNorm");
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRider_alignRiderAxis(void* rider, void* a, void* b, float x, float y);
extern "C" void cRider_updateOrientationImplicit(void*);
int cRider_getControlState(void* rider);

// Function-local static `gravity(0, 0, -980, 0)` and its init guard, spelled out as globals. The guard
// is a separate scalar at 0x004A4C04 (splat only names D_004A4C00; a D_004A4C00[] view schedules differently).
extern int D_004A4C04;
extern t4Vector cRailslideMotion_step_kRailGravity;

// The surface record a world hit points at (id at 0, flags at 0x1C).
struct sSurf_AF28 {
    unsigned int id;
    char pad04[0x18];
    int flags;                  // 0x1C
};

// World ray/sphere hit record filled by cOverlapList_splineIntersect (0x80 bytes).
struct sHit_AF28 {
    t4Vector pos;               // 0x00
    t4Vector nrm;               // 0x10
    t4Vector v20;               // 0x20
    char pad30[0x1C];
    int mat;                    // 0x4C
    char* obj;                  // 0x50
    int pad54;
    sSurf_AF28* surf;           // 0x58
    char pad5C[0x24];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_AF28(const t4Vector& v)
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
static inline float Dot_AF28(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_AF28(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void ScaleEq_AF28(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline t4Vector Div_AF28(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_AF28(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void AddEq_AF28(t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_AF28(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector Cross_AF28(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Normalize_AF28(const t4Vector& v)
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

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float Abs_AF28(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float Clamp_AF28(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

// PORT: `<?` / `>?` are the g++ minimum / maximum operators (EE min.s / max.s).
static inline float Min_AF28(float a, float b)
{
    return a <? b;
}

static inline float Max_AF28(float a, float b)
{
    return a >? b;
}

static inline float ClampAbs_AF28(float v, const float& lim)
{
    return Clamp_AF28(v, -lim, lim);
}

static inline float Approach_AF28(float cur, float target, float step)
{
    float r;
    if (target + step < cur)
        r = cur - step;
    else if (cur < target - step)
        r = cur + step;
    else
        r = target;
    return r;
}

static inline t4Vector side_AF28(char* p) { return *(t4Vector*)(*(char**)(p + 0x50) + 0x1A0); }
static inline t4Vector fwd_AF28(char* p) { return *(t4Vector*)(*(char**)(p + 0x50) + 0x1B0); }
static inline t4Vector up_AF28(char* p) { return *(t4Vector*)(*(char**)(p + 0x50) + 0x1C0); }

// Virtual slots of the hit object's handler (int f46(), void f42(hit), void f43(hit, const t4Vector&)).
typedef sVEntry<int(void*)> sVE46_AF28;
typedef sVEntry<void(void*, void*)> sVE42_AF28;
typedef sVEntry<void(void*, void*, const t4Vector&)> sVE43_AF28;

// v * k, but never below lo (the threshold is where v * k reaches lo).
static inline float ScaleAtLeast_AF28(float v, float k, float lo)
{
    float r = lo;
    if (!(v < lo / k))
        r = v * k;
    return r;
}
#define RIDER_AF28 (*(char**)(self + 0x50))

/**
 * Advances the rider one frame on a rail: keeps it on the rail, moves it along and leans it into the pose.
 *
 * Caps the speed at rider +0x2E4, sets ForceNaturalAir (cleared while the rail holds) and looks for the rail
 * near the board-root bone (cOverlapList_splineIntersect, radius 300), recording SplineID, the contact data and the rail
 * object's answer to handler slot 46 (rider +0x5AC). While cRider_isRailBoardIntersect() accepts the contact, RailNorm
 * becomes the rail direction of travel, the height snaps toward the rail, the velocity turns onto the rail
 * and gains gravity (0, 0, -980) along it plus a boost push (mBoostStrength * 2450). The sideways offset gets
 * the lean input plus a drift: a pull back to the rail for the first 0.6 s or on steep rails, otherwise a
 * push outward (doubled on surfaces with flag 0x4, a pull within 2.5 units of the centre elsewhere) whose
 * gain a rider stat reduces, probably the balance. RailOffCenter, RailLeanAngle and CrookedSlideAngle then
 * set the pose for cRider_alignRiderAxis(). A rejected contact instead adds 277.78 to the velocity, pointing
 * away from the rail.
 */
extern "C" void cRailslideMotion_step(char* self)
{
    if (cRider_getControlState(RIDER_AF28) == 0xC)
        *(float*)(self + 0x1C) = 0.0f;
    cRider* rider = (cRider*)(RIDER_AF28);
    float len = Len_AF28(rider->mVelocity);
    if (rider->TerminalVelocity < len)
        ScaleEq_AF28(rider->mVelocity, rider->TerminalVelocity / len);
    float dt = *(float*)(RIDER_AF28 + 0x300) * 0.01666666753590107f;
    *(int*)(self + 0x20) = 1;
    *(int*)(RIDER_AF28 + 0x5AC) = 0;
    cRider* rider2 = (cRider*)(RIDER_AF28);
    t4Vector* pt = &rider2->mAnimModel->bones[rider2->mBoneBoardRoot].pos;
    sHit_AF28 hit;
    if (cOverlapList_splineIntersect(rider2->mOverlapList, pt, &hit, 1, 300.0f)) {
        if (hit.obj) {
            cInstanceNode* handler = (cInstanceNode*)(*(char**)(hit.obj + 0xC));
            if (handler) {
                sVE46_AF28* vt = (sVE46_AF28*)handler->mVtbl;
                if (vt[46].fn(vthis(handler, vt[46])))
                    *(int*)(RIDER_AF28 + 0x5AC) = 1;
            }
        }
        if (hit.surf)
            *(unsigned int*)(self + 0x24) = hit.surf->id;
        else
            *(unsigned int*)(self + 0x24) = 0xFFFFFFFF;
        if (hit.obj) {
            cInstanceNode* handler = (cInstanceNode*)(*(char**)(hit.obj + 0xC));
            if (handler) {
                sVE42_AF28* vt = (sVE42_AF28*)handler->mVtbl;
                vt[42].fn(vthis(handler, vt[42]), &hit);
            }
        }
        *(int*)(RIDER_AF28 + 0x438) = hit.mat;
        *(t4Vector*)(RIDER_AF28 + 0x460) = hit.pos;
        *(t4Vector*)(RIDER_AF28 + 0x3D0) = hit.v20;
        cRider* rider3 = (cRider*)(RIDER_AF28);
        float spd = ScaleAtLeast_AF28(Len_AF28(rider3->mVelocity), 0.0009000000427477062f, 0.5f);
        float speedSq = spd * spd;
        if (*(float*)(self + 0x14) < 0.6000000238418579f)
            rider3->mSlewed[9].cur = 2.0f;
        if (cRider_isRailBoardIntersect(RIDER_AF28, &hit.pos, &hit.nrm)) {
            *(int*)(self + 0x20) = 0;
            *(float*)(self + 0x14) += dt;
            t4Vector relVel = Sub_AF28(*(t4Vector*)(RIDER_AF28 + 0x1E0), *(t4Vector*)(RIDER_AF28 + 0x3D0));
            float dn = Dot_AF28(relVel, hit.nrm);
            if (dn < 0.0f) {
                dn = -dn;
                ScaleEq_AF28(hit.nrm, -1.0f);
            }
            *(t4Vector*)self = hit.nrm;
            t4Vector rel = Sub_AF28(hit.pos, *pt);
            float lim = dt * 1000.0f;
            float side = Dot_AF28(side_AF28(self), rel);
            float up = Dot_AF28(rel, up_AF28(self));
            up = ClampAbs_AF28(up, lim);
            AddEq_AF28(*(t4Vector*)(RIDER_AF28 + 0x110), Scale_AF28(up_AF28(self), up));
            t4Vector nv = Scale_AF28(hit.nrm, dn);
            float normalBlend = dt * 30.0f;
            float vl = Len_AF28(relVel);
            relVel = Add_AF28(Scale_AF28(nv, normalBlend), Scale_AF28(relVel, 1.0f - normalBlend));
            float nl = Len_AF28(relVel);
            if (nl > 0.0010000000474974513f)
                ScaleEq_AF28(relVel, vl / nl);
            *(t4Vector*)(RIDER_AF28 + 0x1E0) = Add_AF28(relVel, *(t4Vector*)(RIDER_AF28 + 0x3D0));
            if (D_004A4C04 == 0) {
                cRailslideMotion_step_kRailGravity.x = 0.0f;
                cRailslideMotion_step_kRailGravity.y = 0.0f;
                cRailslideMotion_step_kRailGravity.z = -980.0f;
                cRailslideMotion_step_kRailGravity.w = 0.0f;
                D_004A4C04 = 1;
            }
            t4Vector acc = Sub_AF28(Scale_AF28(hit.nrm, Dot_AF28(cRailslideMotion_step_kRailGravity, hit.nrm)), Scale_AF28(nv, 0.0f));
            float boostStrength = *(float*)(RIDER_AF28 + 0x2FC);
            if (boostStrength > 0.0f)
                AddEq_AF28(acc, Scale_AF28(hit.nrm, boostStrength * (dn > 0.0f ? 2450.0f : -2450.0f)));
            if (hit.obj) {
                cInstanceNode* handler = (cInstanceNode*)(*(char**)(hit.obj + 0xC));
                if (handler) {
                    sVE43_AF28* vt = (sVE43_AF28*)handler->mVtbl;
                    vt[43].fn(vthis(handler, vt[43]), &hit, Sub_AF28(cRailslideMotion_step_kRailGravity, Scale_AF28(nv, 0.0f)));
                }
            }
            AddEq_AF28(*(t4Vector*)(RIDER_AF28 + 0x1E0), Scale_AF28(acc, dt));
            AddEq_AF28(*(t4Vector*)(RIDER_AF28 + 0x110), Scale_AF28(*(t4Vector*)(RIDER_AF28 + 0x1E0), dt));
            float ang = Clamp_AF28(spd, 0.5f, 2.0f) * 180.0f;
            float turn = (up_AF28(self).z < 0.0f ? -*(float*)(RIDER_AF28 + 0x22C) : *(float*)(RIDER_AF28 + 0x22C)) * ang;
            int steep = Abs_AF28(hit.nrm.z) > 0.92f;
            float k;
            if (*(float*)(self + 0x14) < 0.6000000238418579f || steep) {
                k = -5.0f;
            } else {
                k = 0.30000001192092896f;
                int st = *(int*)(RIDER_AF28 + 0x328);
                if (st == 3 || st == 4)
                    k = 1.7999999523162842f;
                int grab = hit.surf && (hit.surf->flags & 4);
                if (grab)
                    k = k + k;
                else if (Abs_AF28(side) < 2.5f)
                    k = Min_AF28(k, -1.0f);
                k *= 1.0f / (cBEStatInterface_getBalanceAttrib(cBE_getInterface__Fv(cBE_getBE(), 3), *(int*)(RIDER_AF28 + 0x86C), *(int*)(RIDER_AF28 + 0xB34)) * 1.650076150894165f + 1.0f);
            }
            float clampedSide = side;
            if (k >= 0.0f)
                clampedSide = Clamp_AF28(side, -20.0f, 20.0f);
            turn -= clampedSide * (k * Clamp_AF28(1.0f / speedSq, 1.0f, 4.0f));
            int flip = up_AF28(self).z < 0.0f && Abs_AF28(side) < 20.0f;
            if (flip) {
                if (side > 0.0f)
                    turn = Min_AF28(turn, -360.0f);
                else
                    turn = Max_AF28(turn, 360.0f);
            }
            AddEq_AF28(*(t4Vector*)(RIDER_AF28 + 0x110), Scale_AF28(side_AF28(self), turn * dt));
            float target, roll;
            int st2 = *(int*)(RIDER_AF28 + 0x328);
            if (st2 == 3 || st2 == 4) {
                float lean = Clamp_AF28(side * 0.014285714365541935f, -1.0f, 1.0f);
                *(float*)(self + 0x18) = lean;
                target = lean * 0.523598849773407f;
                roll = ClampAbs_AF28(*(float*)(self + 0x1C), 0.698131799697876f);
            } else {
                float lean = Clamp_AF28(side * 0.03333333507180214f, -1.0f, 1.0f);
                *(float*)(self + 0x18) = lean;
                target = lean * 0.349065899848938f;
                roll = ClampAbs_AF28(*(float*)(self + 0x1C), 0.349065899848938f);
            }
            if (up_AF28(self).z < 0.0f)
                target = -target;
            float leanAngle = Approach_AF28(*(float*)(self + 0x10), target, dt * 300.0f);
            *(float*)(self + 0x10) = leanAngle;
            float sinLean, cosLean;
            fSinCos(&sinLean, &cosLean, leanAngle);
            t4Vector dir = cRailslideMotion_getRailUpNorm_AF28(self, hit.nrm);
            *(t4Vector*)(RIDER_AF28 + 0x370) = dir;
            t4Vector cr = Cross_AF28(dir, hit.nrm);
            t4Vector leanDir = Add_AF28(Scale_AF28(dir, cosLean), Scale_AF28(cr, sinLean));
            float sinRoll, cosRoll;
            fSinCos(&sinRoll, &cosRoll, roll);
            t4Vector rollDir = Sub_AF28(Scale_AF28(cr, cosRoll), Scale_AF28(hit.nrm, sinRoll));
            t4Vector axis = Normalize_AF28(Cross_AF28(rollDir, leanDir));
            float forwardWeight = Dot_AF28(axis, fwd_AF28(self)) * 0.5f + 0.5f;
            char* rider4 = RIDER_AF28;
            cRider_alignRiderAxis(rider4, &leanDir, &axis, (forwardWeight * 15.0f + (1.0f - forwardWeight)) * *(float*)(rider4 + 0x300), 10000000000.0f);
        } else {
            t4Vector fromHit = Sub_AF28(*pt, hit.pos);
            t4Vector dirn;
            float dl = Len_AF28(fromHit);
            if (dl < 0.0010000000474974513f) {
                dirn = hit.nrm;
            } else {
                dirn = Div_AF28(fromHit, dl);
                if (Abs_AF28(Dot_AF28(dirn, hit.nrm)) > 0.5f)
                    dirn = hit.nrm;
            }
            float vd = Dot_AF28(*(t4Vector*)(RIDER_AF28 + 0x1E0), dirn);
            if (vd < 0.0f) {
                cRider* rider5 = (cRider*)(RIDER_AF28);
                float vl = Len_AF28(rider5->mVelocity);
                if (vl < 0.0010000000474974513f) {
                    dirn = hit.nrm;
                } else {
                    t4Vector pushDir = Sub_AF28(dirn, Scale_AF28(rider5->mVelocity, vd / vl));
                    float pl = Len_AF28(pushDir);
                    if (pl > 0.0010000000474974513f)
                        dirn = Div_AF28(pushDir, pl);
                    else
                        dirn = Div_AF28(*(t4Vector*)(RIDER_AF28 + 0x1E0), vl);
                }
            }
            AddEq_AF28(*(t4Vector*)(RIDER_AF28 + 0x1E0), Scale_AF28(dirn, 277.77777099609375f));
        }
    }
    cRider_updateOrientationImplicit(RIDER_AF28);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideMotion_getRailUpNorm);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
extern t4Vector t4vect_ZAxis_BD80 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* s, float* c, float angle);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_BD80(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void vu0ScaleEq_BD80(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_BD80(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "o"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_BD80(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector vu0Normalize_BD80(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector vu0Cross_BD80(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_BD80(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline t4Vector up_BD80(char* p) { return *(t4Vector*)(*(char**)(p + 0x50) + 0x1C0); }

// PORT: the unit declares cRailslideMotion_getRailUpNorm earlier as returning sVec4HP; same symbol.
extern "C" t4Vector cRailslideMotion_getRailUpNorm_BD80(char* p, const t4Vector& n) __asm__("cRailslideMotion_getRailUpNorm");
/**
 * Returns the up direction to use on a rail with normal @p n, tilted by the rail lean angle.
 *
 * For a rail whose normal is not steep (|n.z| <= 0.92) it is the Z axis with its component along @p n removed,
 * normalised and flipped to agree with the rider's up (mMatrix row 2). For a steep one it builds a = normalised (up x
 * n) and b = n x a, and returns b cos(RailLeanAngle) - a sin(RailLeanAngle) (fSinCos).
 *
 * @param railslide  the cRailslideMotion (this)
 * @param n  the rail normal
 * @return the up vector
 */
extern "C" t4Vector cRailslideMotion_getRailUpNorm_BD80(char* railslide, const t4Vector& n)
{
    float sinLean, cosLean;
    fSinCos(&sinLean, &cosLean, *(float*)(railslide + 0x10));
    bool steep = absf_BD80(n.z) > 0.92f;
    t4Vector railUp;
    if (!steep) {
        railUp = vu0Normalize_BD80(vu0Sub_BD80(t4vect_ZAxis_BD80, vu0Scale_BD80(n, n.z)));
        if (vu0Dot_BD80(railUp, up_BD80(railslide)) < 0.0f)
            vu0ScaleEq_BD80(railUp, -1.0f);
    } else {
        t4Vector sideDir = vu0Normalize_BD80(vu0Cross_BD80(up_BD80(railslide), n));
        t4Vector perpDir = vu0Cross_BD80(n, sideDir);
        railUp = vu0Sub_BD80(vu0Scale_BD80(perpDir, cosLean), vu0Scale_BD80(sideDir, sinLean));
    }
    return railUp;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideMotion_resolve);
#ifdef SKIP_ASM
extern "C" void cRider_applyJumpPop(void* rider, float v);
// PORT: cRider_updateBoundBox is declared void* in this unit; this caller ignores the result (void call).
extern "C" void cRider_updateBoundBox_v(void* rider, int a1) __asm__("cRider_updateBoundBox");
extern "C" void cRider_checkObjectCollisions(void* rider, int a1);
extern "C" void cRider_checkOpponentCollisions(void* rider);
extern "C" void cRailslideMotion_checkTerrainCollisions(cRailslideMotion* self);
extern "C" void cSSXAudio_Lift(void* snd, void* rider);
int cRider_getControlState(void* rider);
int cRider_getMotionState(void* rider);
// PORT: cRider_setMotionState is defined with one parameter but this caller passes (rider, 1).
void cRider_setMotionState_impl_BFA8(void* rider, int v) __asm__("cRider_setMotionState__FPv");

struct sTbl_BFA8 {
    int v[4];
};
extern sTbl_BFA8 D_00458230;

struct sVec4_BFA8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_BFA8(const sVec4_BFA8& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void vu0ScaleEq_BFA8(sVec4_BFA8& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

#define RIDER_BFA8 (*(char**)((char*)self + 0x50))

/**
 * Resolves the rail motion: pops the rider off a lost rail, runs the collision checks and caps the speed.
 *
 * With ForceNaturalAir set it eases slewed channel 5 to 0 and pops the rider with cRider_applyJumpPop(rider, -1).
 * cRider_checkObjectCollisions() runs with the sphere tree's filter word (+0x28) set to the current uber grind's entry of
 * D_00458230 in CS_UberGrind (0x16 otherwise), cRailslideMotion_checkTerrainCollisions() with 3, then the
 * filter is reset to -1 and opponents are checked. A rider that lost the rail switches to MS_Air with the
 * lift sound (cSSXAudio_Lift), and the speed is capped at rider +0x2E4.
 */
extern "C" void cRailslideMotion_resolve(cRailslideMotion* self)
{
    cRider_updateBoundBox_v(RIDER_BFA8, 0);
    if (self->ForceNaturalAir != 0) {
        cRider* rider = (cRider*)(RIDER_BFA8);
        *(int*)&rider->mSlewed[5].target = 0;
        rider->mSlewed[5].rate = 0.03333333507180214f;
        cRider_applyJumpPop(RIDER_BFA8, -1.0f);
    }
    sTbl_BFA8 tbl = D_00458230;
    if (cRider_getControlState(RIDER_BFA8) == 0xC) {
        cRider* rider = (cRider*)(RIDER_BFA8);
        rider->mSphereTree->sphMask = tbl.v[rider->mRiderHandler->UberGrindControl.CurrentGrind];
    } else {
        *(int*)(*(char**)(RIDER_BFA8 + 0xAA0) + 0x28) = 22;
    }
    cRider_checkObjectCollisions(RIDER_BFA8, 0);
    *(int*)(*(char**)(RIDER_BFA8 + 0xAA0) + 0x28) = 3;
    cRailslideMotion_checkTerrainCollisions(self);
    *(unsigned int*)(*(char**)(RIDER_BFA8 + 0xAA0) + 0x28) = 0xFFFFFFFF;
    cRider_checkOpponentCollisions(RIDER_BFA8);
    if (self->ForceNaturalAir != 0 && cRider_getMotionState(RIDER_BFA8) == 4) {
        cRider_setMotionState_impl_BFA8(RIDER_BFA8, 1);
        cSSXAudio_Lift((char*)gSSXAudio, RIDER_BFA8);
    }
    cRider* rider = (cRider*)(RIDER_BFA8);
    float len = vu0Length_BFA8(*(sVec4_BFA8*)&rider->mVelocity);
    if (rider->TerminalVelocity < len)
        vu0ScaleEq_BFA8(*(sVec4_BFA8*)&rider->mVelocity, rider->TerminalVelocity / len);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideMotion_checkTerrainCollisions);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(void*);
int cRider_getControlState(void* rider);
extern "C" float cOverlapList_terrainIntersect1(void* world, void* seg, void* hit, int flags);
extern "C" void cRider_moveFromCollision(void* rider, void* v);
extern "C" void cRider_reactToCollision(void* rider, void* info, int id);
extern "C" void* cWorldRider_cWorldRider(void* self, void* tree, int flag);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);



struct sInfo_13C140 {
    sVec4HP pos;
    sVec4HP dir;
    sVec4HP normal;
    float depth;
} __attribute__((aligned(16)));

struct sVEnt_13C140 { short delta; short index; bool (*fn)(void*); };

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4HP vu0Scale_13C140(const sVec4HP& v, float s)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4HP vu0Sub_13C140(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void vu0AddEq_13C140(sVec4HP& a, const sVec4HP& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_13C140(const sVec4HP& a, const sVec4HP& b)
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
        : "m"(a), "m"(b)
        );
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4HP vu0Normalize_13C140(const sVec4HP& v)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_13C140(const sVec4HP& v)
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
        : "m"(v)
        );
    return r;
}

static inline bool riderFlag_13C140(char* r)
{
    char* sub = r + 0x6C0;
    sVEnt_13C140* vt = *(sVEnt_13C140**)sub;
    return vt[8].fn(vthis(sub, vt[8]));
}

#define RIDER (*(char**)((char*)self + 0x50))
#define RV(off) (*(sVec4HP*)(RIDER + (off)))
#define SV(off) (*(sVec4HP*)((char*)self + (off)))

/**
 * Pushes a rail-sliding rider out of terrain and frees it when it stays stuck against it.
 *
 * Skipped in CS_Reset. A sphere-tree query against the overlap list (mWallHint) finds the overlap; hits
 * facing straight against mGroundNormal (the rail's up) are ignored and downward ones lose their
 * mGroundNormal part. The rider moves out by 1.1 times the depth. When it has moved no more than 2 units
 * since the previous hit for over 0.5 s (StuckSec, counted at 1/60 per call, and PrevPos), its velocity
 * becomes 1111.11 along its front, less the part into the surface. Inward velocity is then removed plus a
 * bounce of max(half of it, 27.78), and the impact goes to cRider_reactToCollision() with that bounce as its speed.
 */
extern "C" void cRailslideMotion_checkTerrainCollisions(cRailslideMotion* self)
{
    if (cRider_getControlState(RIDER) == 9)
        return;
    tIntersectInfo hit;
    cWorldRider sph;
    char* rider = RIDER;
    cWorldRider_cWorldRider(&sph, *(void**)(rider + 0xAA0), !riderFlag_13C140(rider));
    float penetration = cOverlapList_terrainIntersect1(*(void**)(RIDER + 0x860), &sph, &hit, *(int*)(RIDER + 0x868));
    if (penetration < 0.0f) { cWorldRider__cWorldRider(&sph, 2); return; }
    {
        float dot = vu0Dot_13C140(RV(880), (*(sVec4HP*)&hit.Inorm));
        if (dot < -0.9998999834060669f) { cWorldRider__cWorldRider(&sph, 2); return; }
        if (dot < 0.0f)
            (*(sVec4HP*)&hit.Inorm) = vu0Normalize_13C140(vu0Sub_13C140((*(sVec4HP*)&hit.Inorm), vu0Scale_13C140(RV(880), dot)));
    }
    {
        sVec4HP push = vu0Scale_13C140(vu0Scale_13C140((*(sVec4HP*)&hit.Inorm), 1.100000023841858f), penetration);
        cRider_moveFromCollision(RIDER, &push);
        cRider_updateOrientationImplicit(RIDER);
        sVec4HP posDelta = vu0Sub_13C140(SV(0x30), RV(272));
        float len = vu0Length_13C140(posDelta);
        SV(0x30) = RV(272);
        if (2.0f < len) {
            self->StuckSec = 0.0f;
        } else {
            self->StuckSec += 0.01666666753590107f;
            if (0.5f < self->StuckSec) {
                sVec4HP fwd = RV(432);
                float dd = vu0Dot_13C140(fwd, (*(sVec4HP*)&hit.Inorm));
                float spd = 1111.111083984375f;
                if (dd < -0.9990000128746033f) {
                    RV(0x1E0) = vu0Scale_13C140(fwd, spd);
                } else {
                    sVec4HP pushDir = vu0Sub_13C140(fwd, vu0Scale_13C140((*(sVec4HP*)&hit.Inorm), dd));
                    RV(0x1E0) = vu0Scale_13C140(vu0Normalize_13C140(pushDir), spd);
                }
            }
        }
        float dot2 = vu0Dot_13C140(vu0Scale_13C140((*(sVec4HP*)&hit.Inorm), -1.0f), RV(0x1E0));
        if (dot2 < 0.0f) { cWorldRider__cWorldRider(&sph, 2); return; }
        sVec4HP n = vu0Normalize_13C140(RV(0x1E0));
        float newNormVel = dot2 * 0.5f < 27.77777862548828f ? 27.77777862548828f : dot2 * 0.5f;
        vu0AddEq_13C140(RV(0x1E0), vu0Scale_13C140((*(sVec4HP*)&hit.Inorm), dot2 + newNormVel));
        sInfo_13C140 info;
        info.pos = (*(sVec4HP*)&hit.Ipos);
        info.dir = n;
        info.normal = (*(sVec4HP*)&hit.Inorm);
        info.depth = newNormVel;
        cRider_reactToCollision(RIDER, &info, hit.Stype);
    }
    cWorldRider__cWorldRider(&sph, 2);
}
#undef SV
#undef RV
#undef RIDER
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideMotion_loseFocus);
#ifdef SKIP_ASM
/**
 * Ends the rail motion: restarts the rider's slewed channel 9 and clears the rider word at +0x5AC.
 *
 * Channel 9 is set to -0.25 and eased toward 1 at rate 1/30 (its target is written -0.25, then 1).
 */
extern "C" void cRailslideMotion_loseFocus(cRailslideMotion* self)
{
    cRider* rider = self->mRider;
    rider->mSlewed[9].cur = rider->mSlewed[9].target = -0.25f;
    rider->mSlewed[9].rate = 0.0f;
    cRider* rider2 = self->mRider;
    rider2->mSlewed[9].target = 1.0f;
    rider2->mSlewed[9].rate = 0.03333333507180214f;
    self->mRider->RidingFlexRail = 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideMotion_writeToReplayFrame);
#ifdef SKIP_ASM

/**
 * Writes the rail-slide state (its first 0x50 bytes, cRailslideMotionState) to a replay frame.
 *
 * Calls the stream's write virtual (slot 1) with this object and the size 0x50.
 *
 * @param obj  replay stream to write to (On Tour cReplayFramePtr*)
 */
extern "C" void cRailslideMotion_writeToReplayFrame(cRailslideMotion* self, cBxReplayFrame* obj)
{
    obj->write((const char*)self, 80);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideMotion_readFromReplayFrame);
#ifdef SKIP_ASM

/**
 * Reads the rail-slide state (its first 0x50 bytes, cRailslideMotionState) back from a replay frame.
 *
 * Calls the stream's read virtual (slot 2) with this object and the size 0x50.
 *
 * @param obj  replay stream to read from (On Tour cReplayFramePtr*)
 */
extern "C" void cRailslideMotion_readFromReplayFrame(cRailslideMotion* self, cBxReplayFrame* obj)
{
    obj->read((char*)self, 80);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_cGroundMotion);
#ifdef SKIP_ASM
extern "C" void cRider_cRider(void* self);

struct sVEntry_13C650 { short delta; short index; void* fn; };
struct sVtbl9_13C650 { sVEntry_13C650 e[9]; } __attribute__((aligned(8)));
struct sVtbl22_13C650 { sVEntry_13C650 e[22]; } __attribute__((aligned(8)));

extern const sVtbl9_13C650 D_00459A98;
extern char D_00459B90[];
extern const sVtbl22_13C650 D_00459AE0;

/**
 * Constructs the ground-motion part: sets up its virtual cRider base and clears the AI ticks.
 *
 * When in charge it points mRider at self + 0x20 and constructs the cRider there. It then stores
 * cGroundMotion's vtables in the cRider (D_00459A98 at +0x6E8, D_00459B90 at +0x6D0, D_00459AE0 at +0x6C0),
 * using stack copies with adjusted this-deltas when not in charge (g++ 2.95 virtual-base code), and zeroes
 * AITickOnGainFocus, AITickOnLoseFocus and rider +0x31C (the board chatter value).
 *
 * @param inChrg  g++ in-charge flag: nonzero when this is the complete object, so the cRider base is built
 *                here
 * @return self
 */
// PORT: g++ 2.95 virtual-base construction. The class derives virtually from cRider (vbase pointer at
// +0x18, vbase at +0x20); when not most-derived, g++ copies cRider's overridden vtables to the stack and
// fixes up the this-deltas (expand_upcast_fixups). Written out by hand here.
extern "C" void* cGroundMotion_cGroundMotion(cGroundMotion* self, int inChrg)
{
    if (inChrg)
    {
        char* vb = (char*)self + 0x20;
        self->mRider = (cRider*)vb;
        cRider_cRider(vb);
    }
    self->mRider->mScriptObjectVtbl = (void*)&D_00459A98;
    self->mRider->mScriptObjRiderVtbl = D_00459B90;
    self->mRider->mVtbl = (void*)&D_00459AE0;
    if (!inChrg)
    {
        sVtbl9_13C650 scriptObjVtbl = D_00459A98;
        self->mRider->mScriptObjectVtbl = &scriptObjVtbl;
        char* base = (char*)self->mRider - 0x20;
        int baseDelta = (char*)self - base;
        scriptObjVtbl.e[1].delta = D_00459A98.e[1].delta + baseDelta;
        sVtbl22_13C650 riderVtbl = D_00459AE0;
        self->mRider->mVtbl = &riderVtbl;
        riderVtbl.e[1].delta = D_00459AE0.e[1].delta + baseDelta;
    }
    self->AITickOnLoseFocus = 0;
    self->AITickOnGainFocus = 0;
    *(int*)&self->mRider->ChatterMag = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_gainFocus);
#ifdef SKIP_ASM
extern "C" int AITick();



// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void vu0ScaleEq_13C7A8(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

struct sHpCtl_13C7A8 {
    int ChatterTime;
    float PowderDepth1G;
    float PowderDepth3G;
    int ForceNaturalAir;
    int AITickOnGainFocus;
    int AITickOnLoseFocus;
    char* mRider;
};

/**
 * Starts the ground motion: resets the ground state and cuts the speed after a quick re-landing.
 *
 * Clears ChatterTime and ForceNaturalAir, sets PowderDepth1G/3G from the surface's depths (+0x14, +0x18)
 * times the model scale (mAnimModel +0x140), copies mGroundNormal to rider +0x390 and stores the AI tick in
 * AITickOnGainFocus. The velocity is scaled by 0.7 when the rider left the ground at most 40 ticks ago,
 * rising by 0.01 per tick to no cut at 70 ticks (cGroundMotion_loseFocus backdates the tick after a
 * natural-air pop, so then there is no cut).
 */
extern "C" void cGroundMotion_gainFocus(sHpCtl_13C7A8* self)
{
    self->ChatterTime = 0;
    cRider* rider = (cRider*)self->mRider;
    tSurfData* tbl = (tSurfData*)SSXApp->Game->SurfPropCache;
    tSurfData* snow = &tbl[rider->mSurfaceType];
    float k = rider->mAnimModel->ScaleVect.x;
    self->PowderDepth1G = k * snow->PowderDepth1G;
    self->PowderDepth3G = k * snow->PowderDepth3G;
    *(&rider->DampGroundNormal) = *(&rider->mGroundNormal);
    self->ForceNaturalAir = 0;
    int aiTick = AITick();
    self->AITickOnGainFocus = aiTick;
    int ticksOutOfGround = aiTick - self->AITickOnLoseFocus;
    float penaltyFraction;
    if (ticksOutOfGround > 40)
        penaltyFraction = (ticksOutOfGround - 40) * 0.010000000707805157f + 0.699999988079071f;
    else
        penaltyFraction = 0.699999988079071f;
    vu0ScaleEq_13C7A8(*(t4Vector*)(self->mRider + 0x1E0), penaltyFraction <? 1.0f);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_getSnowForce);
#ifdef SKIP_ASM
/**
 * Computes the snow's support force along the ground normal from the board's height in the powder.
 *
 * Above the surface it pulls down by g * height / 30 (g = snow +0x0), less damping (snow +0x1C) times an
 * upward normal velocity. Below it the push grows linearly to 1 g at PowderDepth1G and 3 g at PowderDepth3G
 * (the depth is capped there), less the damping times the normal velocity. Also sets mAudioPressure to the
 * depth / PowderDepth3G (at least 0). The caller passes a fifth argument (the time step) that this body does
 * not declare.
 *
 * @param snow  the surface data (SurfPropCache entry read as floats; On Tour tSurfData* snow)
 * @param height  the board's height above the snow surface (rider +0x454), negative when sunk in
 * @param normalVel  the velocity along the ground normal, relative to the ground
 * @return the normal force (positive pushes the board out of the snow)
 */
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" float cGroundMotion_getSnowForce(cGroundMotion* self, float* snow, float height, float normalVel)
{
    float force;
    if (height > 0.0f)
    {
        force = snow[0] * (height * -0.033333f);
        if (normalVel > 0.0f)
            force -= snow[7] * normalVel;
    }
    else if (-self->PowderDepth1G < height)
    {
        force = -snow[0] * height / self->PowderDepth1G;
        force -= snow[7] * normalVel;
    }
    else
    {
        height = -self->PowderDepth3G >? height;
        force = snow[0] * (1.0f - (height + self->PowderDepth1G + (height + self->PowderDepth1G)) / (self->PowderDepth3G - self->PowderDepth1G));
        force -= snow[7] * normalVel;
    }
    self->mRider->mAudioPressure = -height / self->PowderDepth3G >? 0.0f;
    return force;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_getBoostForce);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" float cBEStatInterface_getAccelerationAttrib(void* self, int rider, int value);
extern "C" float func_0031C228(float x);
int cRiderAnimBase_getCurrentAnimID(void* anim, int n);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float hpAbs_13C948(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2 FPU float->int->float round trip (truncate) via inline asm.
static inline float hpTrunc_13C948(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    return t;
}

static inline float hpFloor_13C948(float x)
{
    float t = hpTrunc_13C948(x);
    if (x < t)
        t -= 1.0f;
    return t;
}

static inline float hpAtan2_13C948(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

static inline float hpVX_13C948(sVec4HP v) { return v.x; }
static inline float hpVY_13C948(sVec4HP v) { return v.y; }

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_13C948(const sVec4HP& v)
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

#define RIDER (*(char**)((char*)self + 0x18))

/**
 * Computes the forward propulsion: the boost push plus the drive toward the surface's cruising speed.
 *
 * Zero while braking (slewed channel 2) or past the finish line. The boost term (mBoostStrength * 0.17,
 * raised to 5.5 times the path factor while the power-up boost timer +0x2E8 runs) needs |slewed channel 0|
 * (edging) below 0.08, weakens as it grows, and grows with mGroundForward.z when that is positive. The drive
 * term closes up to 1111.11 of the gap to the surface's speed (snow +0x20 * 27.78), weighted by the path
 * factor (1 on flag-0x10 triangles, else falling to 0 at 90 degrees off the path heading at rider +0x4CC).
 * Ground material (+0x434) 17 or above (On Tour's slow territory) halves the speed and the boost, and zeroes
 * everything when facing off the path; snow +0x24 scales the total and slewed channel 4 adds 56.4 per unit.
 *
 * @param snow  the surface data (SurfPropCache entry; On Tour tSurfData*)
 * @return the force along mGroundForward
 */
// cGroundMotion::getBoostForce(const tSurfData* snow) (SSX On Tour symbols; the retail state check stands in for
// !cRider::isOnRaceCourse, and the retail code has no getAccelerationAttrib scale).
extern "C" float cGroundMotion_getBoostForce(cGroundMotion* self, tSurfData* snow)
{
    float slowTerritoryBoostScalar = 1.0f;
    int inSlowTerritory = *(int*)(RIDER + 0x434) < 17;
    inSlowTerritory ^= 1;
    if (inSlowTerritory)
        slowTerritoryBoostScalar = 0.4999951124191284f;
    if (*(float*)(RIDER + 0x214) != 0.0f || *(float*)(RIDER + 0x470) >= 0.0f)
        return 0.0f;
    float totalBoost = 0.0f;
    float pathFactor = 0.0f;
    int fl = *(short*)(RIDER + 0x2D4);
    if (fl & 0x10)
    {
        pathFactor = 1.0f;
    }
    else
    {
        float kAngleLimit = 1.5707964897155762f;
        float heading = hpAtan2_13C948(hpVY_13C948(*(sVec4HP*)(RIDER + 0x1B0)), hpVX_13C948(*(sVec4HP*)(RIDER + 0x1B0)));
        float headingDiff = heading - *(float*)(RIDER + 0x4CC);
        float offPathAngle = hpAbs_13C948(headingDiff - hpFloor_13C948(headingDiff * 0.15915493667125702f + 0.5f) * 6.2831854820251465f);
        if (offPathAngle < kAngleLimit)
            pathFactor = 1.0f - offPathAngle * (1.0f / kAngleLimit);
    }
    float backwardsInSpokePenalty = 1.0f;
    if (pathFactor <= 0.0f && inSlowTerritory)
        backwardsInSpokePenalty = 0.0f;
    float boostWarp = *(float*)(RIDER + 0x2FC) * 0.16997130215168f;
    if (0.0f < *(float*)(RIDER + 0x2E8))
        boostWarp = pathFactor * 5.5f >? boostWarp;
    if (0.0f < boostWarp)
    {
        float delta = 0.07999998331069946f - hpAbs_13C948(*(float*)(RIDER + 0x1F0));
        if (0.0f < delta)
            totalBoost += boostWarp * delta * (1052.7747802734375f * (*(float*)(RIDER + 0x3A8) >= 0.0f ? *(float*)(RIDER + 0x3A8) : 0.0f) + 2350.0f);
    }
    float speed = snow->AutoBoostSpeed * slowTerritoryBoostScalar * 27.77777862548828f - vu0Length_13C948(*(sVec4HP*)(RIDER + 0x1E0));
    float boostDelta = speed < 1111.111083984375f ? speed : 1111.111083984375f;
    if (0.0f < boostDelta)
    {
        cBEStatInterface_getAccelerationAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), *(int*)(RIDER + 0x86C), *(int*)(RIDER + 0xB34));
        float getGoingFactor = cRiderAnimBase_getCurrentAnimID(*(void**)(RIDER + 0x784), 2) == 22 ? 0.20000000298023224f : 0.0f;
        int st = *(int*)(RIDER + 0x434);
        if (st == 11 || st == 12 || st == 13)
            boostDelta *= hpAbs_13C948(*(float*)(RIDER + 0x378));
        totalBoost += (pathFactor >? getGoingFactor * 1.0004451274871826f) * boostDelta;
    }
    return backwardsInSpokePenalty * (snow->AutoBoostFactor * totalBoost * slowTerritoryBoostScalar + *(float*)(RIDER + 0x220) * 56.41297912597656f);
}
#undef RIDER
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_getSlipForce);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" float cBEStatInterface_getSpeedAttrib(void* self, int rider, int value);
extern "C" float cBEStatInterface_getAccelerationAttrib(void* self, int rider, int value);
extern "C" float cBEStatInterface_getBrakingAttrib(void* self, int rider, int value);

struct sCurvePt_13CCF0 { float x, y; };

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float hpAbs_13CCF0(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float hpLerp_13CCF0(const sCurvePt_13CCF0& a, const sCurvePt_13CCF0& b, float x)
{
    float y0 = a.y;
    x -= a.x;
    return y0 + (b.y - y0) * x / (b.x - a.x);
}


static inline float hpCurve_13CCF0(const tSurfData* c, float x)
{
    if (((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[1].x < x)
    {
        if (((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[2].x < x)
        {
            if (((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[3].x < x)
                return ((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[3].y;
            return hpLerp_13CCF0(((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[2], ((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[3], x);
        }
        return hpLerp_13CCF0(((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[1], ((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[2], x);
    }
    if (x < ((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[0].x)
        return ((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[0].y;
    return hpLerp_13CCF0(((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[0], ((sCurvePt_13CCF0*)&c->SlipFriction[0][0])[1], x);
}

static inline float hpClamp_13CCF0(float v, float lo, float hi)
{
    if (v >= lo)
        return v <? hi;
    return lo;
}

static inline float hpRamp_13CCF0(float v, float lo, float hi)
{
    if (v >= lo)
    {
        if (v <= hi)
            return (v - lo) / (hi - lo);
        return 1.0f;
    }
    return 0.0f;
}

#define RIDER (*(char**)((char*)self + 0x18))

/**
 * Computes the friction along the board's forward direction from the speed, braking and the surface.
 *
 * The base is the surface's friction curve (4 points at cfg +0x90) at the speed in km/h (|a| * 0.036),
 * lowered at low speed by a rider stat. Braking (b = |slewed channel 2|) adds 3 * b * b (times a stat factor)
 * to the friction factor and a constant drag of 1000 * b, both faded out on near-vertical ground
 * (mGroundNormal.z 0.2 to 0.1). Powder surfaces (types 2, 3, 13) scale it with the sinking depth, rising
 * relative to the ground cuts it to 0.6, switch stance without boost raises it by 11%, and surface 13
 * multiplies it by 34.8 while moving forward and rising. The caller's third argument (the normal force in g)
 * is not declared.
 *
 * @param cfg  the surface data (SurfPropCache entry; On Tour tSurfData* snow)
 * @param forwardVel  the forward velocity relative to the ground (On Tour v)
 * @return the force along mGroundForward (the friction opposes a; the brake drag is always negative)
 */
extern "C" float cGroundMotion_getSlipForce(cGroundMotion* self, tSurfData* cfg, float forwardVel)
{
    float one = 1.0f;
    float spd = hpAbs_13CCF0(*(float*)(RIDER + 0x214));
    cBEStatInterface_getSpeedAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), *(int*)(RIDER + 0x86C), *(int*)(RIDER + 0xB34));
    float accelFactor = one / (cBEStatInterface_getAccelerationAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), *(int*)(RIDER + 0x86C), *(int*)(RIDER + 0xB34)) * 0.31941401958465576f + one);
    float brakeFactor = cBEStatInterface_getBrakingAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), *(int*)(RIDER + 0x86C), *(int*)(RIDER + 0xB34)) * 0.46218693256378174f + one;
    float slipCurve = hpCurve_13CCF0(cfg, hpAbs_13CCF0(forwardVel) * 0.035999998450279236f);
    float speedBlend = hpClamp_13CCF0(hpAbs_13CCF0(forwardVel) / 1666.666748046875f, 0.0f, 1.0f);
    slipCurve = slipCurve * (one * (accelFactor * (1.0f - speedBlend) + one * speedBlend));
    float ramp = hpRamp_13CCF0(*(float*)(RIDER + 0x378), 0.10000000149011612f, 0.20000000298023224f);
    float lift = ramp * (spd * 3.0004208087921143f) * spd * brakeFactor;
    float drag = ramp * (spd * 999.9600219726562f);
    float powderFactor = 1.0f;
    if (*(int*)(RIDER + 0x438) == 2 || *(int*)(RIDER + 0x438) == 3 || *(int*)(RIDER + 0x438) == 13)
        powderFactor = 0.5f - *(float*)(RIDER + 0x454) / self->PowderDepth3G;
    float risingFactor = 1.0f;
    if (0.0f < *(float*)(RIDER + 0x1E8) - *(float*)(RIDER + 0x3D8))
        risingFactor = 0.5999583005905151f;
    float switchFactor = 1.0f;
    if (*(int*)(RIDER + 0x320) != *(int*)(RIDER + 0x324) && *(float*)(RIDER + 0x2FC) == 0.0f)
        switchFactor = 1.1147289276123047f;
    float climbFactor = 1.0f;
    if (0.0f < forwardVel && 0.0f < *(float*)(RIDER + 0x1E8) - *(float*)(RIDER + 0x3D8) && *(int*)(RIDER + 0x438) == 13)
        climbFactor = 34.84144592285156f;
    return ((-forwardVel * powderFactor) * (slipCurve + lift) * risingFactor * switchFactor - drag) * climbFactor;
}
#undef RIDER
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_getDigForce);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" void cBEStatInterface_getEdgingAttrib(void* iface, int rider, int b);

struct sCurvePt_13D028 { float x, y; };
extern sCurvePt_13D028* kDigMult;

// PORT: PS2-only inline asm (float absolute value), as an SDK math-header fabsf would.
static inline float hpAbs_13D028(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

static inline float hpLerp_13D028(const sCurvePt_13D028& a, const sCurvePt_13D028& b, float x)
{
    float y0 = a.y;
    x -= a.x;
    return y0 + (b.y - y0) * x / (b.x - a.x);
}

static inline float hpCurve_13D028(const sCurvePt_13D028* p, float x)
{
    if (p[1].x < x)
    {
        if (p[2].x < x)
        {
            if (p[3].x < x)
                return p[3].y;
            return hpLerp_13D028(p[2], p[3], x);
        }
        return hpLerp_13D028(p[1], p[2], x);
    }
    if (x < p[0].x)
        return p[0].y;
    return hpLerp_13D028(p[0], p[1], x);
}

/**
 * Computes the edge (dig) force across the board when the rider leans against a sideways slide.
 *
 * Zero unless the lean and the sideways velocity have opposite signs. Otherwise it is the sideways velocity
 * times |lean|, times the curve kDigMult at the forward speed in km/h (|b| * 0.036), times -(snow +0x8) so
 * a positive factor opposes the slide, divided by (1 + 4.0 * mBoostStrength) while boosting. The stat
 * cBEStatInterface_getEdgingAttrib() is read but unused.
 *
 * @param snow  the surface data (SurfPropCache entry read as floats; On Tour tSurfData* snow)
 * @param dv  the sideways velocity relative to the ground (along mGroundSide)
 * @param sv  the forward velocity relative to the ground
 * @param lean  the lean input (minus slewed channel 0)
 * @return the force along mGroundSide (0 when not digging in)
 */
extern "C" float cGroundMotion_getDigForce(cGroundMotion* self, float* snow, float dv, float sv, float lean)
{
    if (dv < 0.0f && 0.0f < lean)
    {
        dv = dv * lean;
    }
    else if (0.0f < dv && lean < 0.0f)
    {
        dv = dv * -lean;
    }
    else
    {
        return 0.0f;
    }
    dv = dv * hpCurve_13D028(kDigMult, hpAbs_13D028(sv) * 0.035999998450279236f);
    dv = dv * -snow[2];
    cBEStatInterface_getEdgingAttrib(cBE_getInterface_Fv(cBE_getBE(), 3), self->mRider->mAIIndex, self->mRider->mStatsIndex);
    float boostStrength = self->mRider->mBoostStrength;
    float one = 1.0f;
    if (0.0f < boostStrength)
        dv = dv * (one / (boostStrength * 4.008637428283691f + one));
    return dv;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_setupGroundVectors);
#ifdef SKIP_ASM
extern "C" void* cWorldLine_cWorldLine(void* seg, const sVec4HP& a, const sVec4HP& b, int n, float r);
extern void* t4vect_Zero[];

struct sHit_13D1B8 {
    sVec4HP pos;            // 0x00
    sVec4HP normal;         // 0x10
    sVec4HP v20;            // 0x20
    float pad30[4];         // 0x30
    float dist;             // 0x40
    int pad44[2];           // 0x44
    int Stype;                // 0x4C
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[5];           // 0x58
    float u;              // 0x6C
    float v;              // 0x70
    int pad74[3];           // 0x74
    sHit_13D1B8() {}
} __attribute__((aligned(16)));

extern "C" int cOverlapList_terrainIntersect(void* world, void* seg, sHit_13D1B8* hits, int max, int flags);
extern sVec4HP t4vect_ZAxis;
extern sVec4HP t4vect_Zero_v __asm__("t4vect_Zero");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4HP vu0Scale_13D1B8(const sVec4HP& v, float s)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4HP vu0Add_13D1B8(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4HP vu0Sub_13D1B8(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_13D1B8(const sVec4HP& a, const sVec4HP& b)
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

// PORT: PS2-only VU0 inline asm (4-component dot product, by-value first operand).
static inline float vu0DotV_13D1B8(sVec4HP a, const sVec4HP& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4HP vu0Normalize_13D1B8(const sVec4HP& v)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sVec4HP vu0Cross_13D1B8(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
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
static inline float vu0Length_13D1B8(const sVec4HP& v)
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

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_13D1B8(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}


// Non-POD by-value vector (user copy ctor => passed by invisible reference, no callee copy).
struct sVec4P_13D1B8 {
    float x, y, z, w;
    sVec4P_13D1B8(const sVec4HP& v) { *(sVec4HP*)this = v; }
    sVec4P_13D1B8(const sVec4P_13D1B8& v) { *(sVec4HP*)this = *(const sVec4HP*)&v; }
} __attribute__((aligned(16)));

static inline sVec4HP vu0SubV_13D1B8(sVec4P_13D1B8 a, const sVec4HP& b)
{
    return vu0Sub_13D1B8(*(sVec4HP*)&a, b);
}

static inline int isBetter_13D1B8(sHit_13D1B8* a, sHit_13D1B8* b)
{
    if (a->obj != 0) {
        if (b->obj == 0) {
            return 1;
        }
        return *(unsigned int*)(a->obj + 0x78) < *(unsigned int*)(b->obj + 0x78);
    }
    if (b->obj != 0) {
        return 0;
    }
    return *(unsigned int*)(a->tri + 0x150) < *(unsigned int*)(b->tri + 0x150);
}

struct sVEnt_13D1B8 { short delta; short index; void (*fn)(void*, void*); };

#define RIDER (*(char**)((char*)self + 0x18))
#define RV(off) (*(sVec4HP*)(RIDER + (off)))

/**
 * Probes the ground under the rider and rebuilds its ground frame and contact data from the best hit.
 *
 * Saves mGroundNormal in rider +0x380, then casts a capsule (radius 0.5) along mGroundNormal from 100 below
 * to 200 above the rider's position, shifted sideways by the edging channel, keeping up to 64 hits. It
 * prefers hits whose normal is within about 72 degrees of the old one (dot >= 0.3) and that lie nearest the
 * probe's middle, then stores the triangle data, surface type, normal, contact point, ground velocity,
 * mGroundForward and mGroundSide and the height above ground, and reports the hit to its object (handler slot
 * 42). With no hit the frame is reset to world up and surface type 0.
 *
 * @param out  receives the chosen hit
 * @param dist  receives the probe point's distance from the contact point across the normal (On Tour
 *              latOffset; 0 with no hit)
 * @return 1 when ground was found, otherwise 0
 */
extern "C" int cGroundMotion_setupGroundVectors(cGroundMotion* self, sHit_13D1B8* out, float* dist)
{
    RV(0x380) = RV(880);
    sVec4HP pos = vu0Add_13D1B8(RV(272), vu0Scale_13D1B8(RV(944), *(float*)(*(char**)(RIDER + 0x780) + 0x140) * (*(float*)(RIDER + 0x1F0) * 45.0f)));
    sVec4HP lineBottom = vu0Add_13D1B8(pos, vu0Scale_13D1B8(RV(880), -100.0f));
    sVec4HP lineTop = vu0Add_13D1B8(pos, vu0Scale_13D1B8(RV(880), 200.0f));
    char seg[176];
    cWorldLine_cWorldLine(seg, lineBottom, lineTop, 2, 0.5f);
    sHit_13D1B8 hits[64];
    int n = cOverlapList_terrainIntersect(*(void**)(RIDER + 0x860), seg, hits, 0x40, *(int*)(RIDER + 0x864));
    if (n == 0) {
        RV(880) = t4vect_ZAxis;
        RV(976) = t4vect_Zero_v;
        RV(928) = vu0Normalize_13D1B8(vu0SubV_13D1B8(RV(432), vu0Scale_13D1B8(RV(880), vu0DotV_13D1B8(RV(432), RV(880)))));
        RV(944) = vu0Cross_13D1B8(RV(880), RV(928));
        *(int*)(RIDER + 0x438) = 0;
        *dist = 0.0f;
        return 0;
    }
    int best = 0;
    float up0 = vu0Dot_13D1B8(hits[0].normal, RV(880));
    float bestDiff = absf_13D1B8(hits[0].dist - 0.5f);
    int ok = up0 >= 0.3f;
    for (int i = 1; i < n; i++) {
        float paramDiff = absf_13D1B8(hits[i].dist - 0.5f);
        float up = vu0Dot_13D1B8(hits[i].normal, RV(880));
        if (!ok) {
            if (up >= 0.3f) {
                bestDiff = paramDiff;
                best = i;
                ok = 1;
                continue;
            }
        } else if (up < 0.3f) {
            continue;
        }
        if (paramDiff < bestDiff) {
            bestDiff = paramDiff;
            best = i;
        } else if (paramDiff == bestDiff) {
            if (isBetter_13D1B8(&hits[i], &hits[best])) {
                best = i;
            }
        }
    }
    *out = hits[best];
    if (out->tri != 0) {
        *(int*)(RIDER + 0x430) = *(int*)(out->tri + 0x150);
        *(float*)(RIDER + 0xAAC) = out->u;
        *(float*)(RIDER + 0xAB0) = out->v;
        *(short*)(RIDER + 0x2D4) = *(short*)(out->tri + 0xA);
    } else {
        *(unsigned int*)(RIDER + 0x430) = 0xFFFFFFFF;
    }
    if (out->obj != 0) {
        char* handler = *(char**)(out->obj + 0xC);
        if (handler) {
            sVEnt_13D1B8* vt = *(sVEnt_13D1B8**)(handler + 0xC);
            vt[42].fn(vthis(handler, vt[42]), out);
        }
    }
    *(int*)(RIDER + 0x438) = out->Stype;
    if (*(int*)(RIDER + 0x438) == -1) {
        *(int*)(RIDER + 0x438) = 0;
    }
    RV(880) = out->normal;
    RV(1120) = out->pos;
    RV(976) = out->v20;
    RV(928) = vu0Normalize_13D1B8(vu0SubV_13D1B8(RV(432), vu0Scale_13D1B8(RV(880), vu0DotV_13D1B8(RV(432), RV(880)))));
    RV(944) = vu0Cross_13D1B8(RV(880), RV(928));
    sVec4HP delta = vu0Sub_13D1B8(pos, out->pos);
    *(float*)(RIDER + 0x454) = vu0Dot_13D1B8(delta, RV(880));
    *dist = vu0Length_13D1B8(vu0Sub_13D1B8(delta, vu0Scale_13D1B8(RV(880), *(float*)(RIDER + 0x454))));
    return 1;
}
#undef RV
#undef RIDER
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_step);
#ifdef SKIP_ASM
#include "ai/rider.h"



struct sCurvePt_13D818 { float x, y; };
extern sCurvePt_13D818* kPowderTiltBwd;
extern sCurvePt_13D818* kMaxTurnKickAngle;
extern sCurvePt_13D818* kKickAngleSpeed;
extern sCurvePt_13D818* kSteerTuneMinAttribEffect;
extern sCurvePt_13D818* kSteerTuneMaxAttribEffect;
extern sCurvePt_13D818* D_004A1158;

extern "C" float cGroundMotion_getSnowForce_13D818(void* self, tSurfData* e, float h, float vn, float dt) __asm__("cGroundMotion_getSnowForce");
extern float cGroundMotion_getBoostForce_D818(void* self, tSurfData* e) __asm__("cGroundMotion_getBoostForce");
extern "C" float cGroundMotion_getSlipForce_13D818(void* self, tSurfData* e, float a, float b) __asm__("cGroundMotion_getSlipForce");
// PORT: cBEStatInterface_getEdgingAttrib is declared void in this unit; this caller uses its float result.
extern "C" float cBEStatInterface_getEdgingAttrib_f(void* iface, int rider, int b) __asm__("cBEStatInterface_getEdgingAttrib");
extern "C" void cRider_rotateOrientation(cRider* self, const t4Vector& axis, float angle);
extern "C" float fCos(float a);
extern "C" float func_0031C128(float x);
int cRiderAnimBase_getCurrentAnimID(void* anim, int layer);

extern t4Vector t4vect_Zero_13D818 __asm__("t4vect_Zero");
extern t4Vector t4vect_ZAxis_13D818 __asm__("t4vect_ZAxis");

struct sVEnt_13D818 { short delta; short index; void (*fn)(void*, sHit_13D1B8*, t4Vector*); };

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Len_13D818(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void vu0ScaleEq_13D818(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_13D818(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0Add_13D818(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_13D818(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void vu0AddEq_13D818(t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (in-place vector subtract).
static inline void vu0SubEq_13D818(t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_13D818(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector vu0Cross_13D818(const t4Vector& a, const t4Vector& b)
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
        : "m"(a), "o"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline t4Vector vu0Div_13D818(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void vu0DivEq_13D818(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector vu0Normalize_13D818(const t4Vector& v)
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

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_13D818(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2 FPU float->int->float round trip (truncate) via inline asm.
static inline float ffloor_13D818(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

// Wrap an angle to [-pi, pi).
static inline float wrap_13D818(float x)
{
    return x - ffloor_13D818(x * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
}

// PORT: g++ minimum operator (<?).
static inline float clamp_13D818(float v, float lo, float hi)
{
    return (lo <= v) ? (v <? hi) : lo;
}

// The rider's front and up rows, by value.
static inline t4Vector front_13D818(cRider* rider) { return rider->mMatrix.row[1]; }
static inline t4Vector up_13D818(cRider* rider) { return rider->mMatrix.row[2]; }

struct sVec4C_13D818 : t4Vector {
    sVec4C_13D818(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; }
};

// Move cur toward target by at most step.
static inline float approach_13D818(float cur, float target, float step)
{
    if (target + step < cur)
        return cur - step;
    if (cur < target - step)
        return cur + step;
    return target;
}

static inline float lerp_13D818(const sCurvePt_13D818& a, const sCurvePt_13D818& b, float x)
{
    float y0 = a.y;
    x -= a.x;
    return y0 + (b.y - y0) * x / (b.x - a.x);
}

// Piecewise-linear 4-point curve.
static inline float curve_13D818(const sCurvePt_13D818* p, float x)
{
    if (p[1].x < x)
    {
        if (p[2].x < x)
        {
            if (p[3].x < x)
                return p[3].y;
            return lerp_13D818(p[2], p[3], x);
        }
        return lerp_13D818(p[1], p[2], x);
    }
    if (x < p[0].x)
        return p[0].y;
    return lerp_13D818(p[0], p[1], x);
}

// Lerp with scalar endpoints: the y loads happen at the call site (see curveB).
static inline float lerpB_13D818(float x0, float y0, float x1, float y1, float x)
{
    return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

// Piecewise-linear 4-point curve, scalar-endpoint lerp (the kKickAngleSpeed table).
static inline float curveB_13D818(const sCurvePt_13D818* p, float x)
{
    if (p[1].x < x)
    {
        if (p[2].x < x)
        {
            if (p[3].x < x)
                return p[3].y;
            return lerpB_13D818(p[2].x, p[2].y, p[3].x, p[3].y, x);
        }
        return lerpB_13D818(p[1].x, p[1].y, p[2].x, p[2].y, x);
    }
    if (x < p[0].x)
        return p[0].y;
    return lerpB_13D818(p[0].x, p[0].y, p[1].x, p[1].y, x);
}


// atan2(y, x) built on a single-argument atan.
static inline float atan2_13D818(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707964f;
        return -1.5707964f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f)
            r += 3.1415927f;
        else
            r -= 3.1415927f;
    }
    return r;
}

/**
 * Advances the rider one frame on the ground: forces, steering, the ground probe and board chatter.
 *
 * Caps the speed at rider +0x2E4 and eases PowderDepth1G/3G toward the surface's values. The velocity
 * relative to the ground is split along the ground frame and feeds getSnowForce (normal), getBoostForce and
 * getSlipForce (forward) and getDigForce (side); up to 2 g of the normal force is redirected along the normal
 * tilted by the edge angle, and the surface gravity (0, 0, -g) is added. A board sunk below PowderDepth3G is
 * lifted out (at most 10 per frame). After integrating it yaws the rider about the ground normal from lean,
 * speed curves and surface grip (or the spin-out decay while mSpinRate is set), re-probes with
 * setupGroundVectors(), removes 40% of the normal velocity on non-powder surfaces (keeping the speed) and
 * sets ForceNaturalAir when the ground is lost or too far below (surface +0x10). On ground it tilts toward
 * the normal and drives slewed channels 2, 17 and 18 and the chatter (ChatterTime, rider +0x31C).
 */
extern "C" void cGroundMotion_step(cGroundMotion* self)
{
    cRider_updateWind((char*)self->mRider);
    float spd = vu0Len_13D818(self->mRider->mVelocity);
    if (self->mRider->TerminalVelocity < spd)
        vu0ScaleEq_13D818(self->mRider->mVelocity, self->mRider->TerminalVelocity / spd);

    float dt = self->mRider->mTimeScale * 0.01666666753590107f;
    tSurfData* snow = &((tSurfData*)SSXApp->Game->SurfPropCache)[self->mRider->mSurfaceType];
    float scale = self->mRider->mAnimModel->ScaleVect.x;
    self->PowderDepth1G = approach_13D818(self->PowderDepth1G, scale * snow->PowderDepth1G, dt * 100.0f);
    self->PowderDepth3G = approach_13D818(self->PowderDepth3G, scale * snow->PowderDepth3G, dt * 100.0f);

    t4Vector rel = vu0Sub_13D818(self->mRider->mVelocity, self->mRider->GroundVelocity);
    float dN = vu0Dot_13D818(rel, self->mRider->mGroundNormal);
    float dF = vu0Dot_13D818(rel, self->mRider->mGroundForward);
    float dS = vu0Dot_13D818(rel, self->mRider->mGroundSide);
    float lean = -self->mRider->mSlewed[0].cur;
    float push = cGroundMotion_getSnowForce_13D818((self), snow, self->mRider->SnowOffset, dN, dt);
    float drag = cGroundMotion_getBoostForce_D818((self), snow);
    float steer = cGroundMotion_getSlipForce_13D818((self), snow, dF, push / snow->Gravity);
    float edge = cGroundMotion_getDigForce(self, (float*)snow, dS, dF, lean);
    if (self->mRider->mGroundNormal.z < 0.0f && push < 0.0f)
        push = 0.0f;
    ((cGroundMotion*)self->mRider->unk_3F8)->ChatterTime = steer;

    float lo = curve_13D818(kSteerTuneMinAttribEffect, spd * 0.035999998450279236f);
    float hi = curve_13D818(kSteerTuneMaxAttribEffect, spd * 0.035999998450279236f);
    float k = cBEStatInterface_getEdgingAttrib_f(cBE_getInterface_Fv(cBE_getBE(), 3), self->mRider->mAIIndex, self->mRider->mStatsIndex);
    float curveRange = hi - lo;
    float sinBank, c;
    fSinCos(&sinBank, &c, snow->MaxLeanAngle * (lo + (k * curveRange + curveRange)) * 0.01745329424738884f * lean);
    float lift = (push <? (snow->Gravity + snow->Gravity)) / c;
    t4Vector up = vu0Add_13D818(vu0Scale_13D818(self->mRider->mGroundNormal, c), vu0Scale_13D818(self->mRider->mGroundSide, sinBank));
    push -= lift;
    sVec4C_13D818 grav(0.0f, 0.0f, -snow->Gravity, 0.0f);
    t4Vector force = vu0Add_13D818(vu0Add_13D818(vu0Add_13D818(vu0Add_13D818(
        vu0Scale_13D818(self->mRider->mGroundNormal, push), vu0Scale_13D818(up, lift)),
        vu0Scale_13D818(self->mRider->mGroundForward, steer + drag)),
        vu0Scale_13D818(self->mRider->mGroundSide, edge)), grav);
    if (self->mRider->mAllowNaturalAir == 0)
        vu0AddEq_13D818(force, vu0Scale_13D818(self->mRider->mGroundNormal, -1000.0f));

    float pen = (self->PowderDepth3G + self->mRider->SnowOffset) >? -10.000000953674316f;
    if (pen < 0.0f) {
        vu0SubEq_13D818(self->mRider->mPosition, vu0Scale_13D818(self->mRider->mGroundNormal, pen));
        self->mRider->SnowOffset -= pen;
        if (dN < 0.0f) {
            vu0SubEq_13D818(rel, vu0Scale_13D818(self->mRider->mGroundNormal, dN));
            self->mRider->mVelocity = vu0Add_13D818(rel, self->mRider->GroundVelocity);
        }
        float fn = vu0Dot_13D818(force, self->mRider->mGroundNormal);
        if (fn < 0.0f)
            vu0SubEq_13D818(force, vu0Scale_13D818(self->mRider->mGroundNormal, fn));
    }
    if (self->mRider->mAllowNaturalAir == 0 && 0.0f <= self->mRider->SnowOffset) {
        vu0SubEq_13D818(self->mRider->mPosition, vu0Scale_13D818(self->mRider->mGroundNormal, self->mRider->SnowOffset));
        self->mRider->SnowOffset = 0.0f;
    }

    float vv = vu0Dot_13D818(rel, rel);
    float heightBefore = vu0Dot_13D818(self->mRider->mPosition, self->mRider->DampGroundNormal);
    if (0.5f < absf_13D818(self->mRider->mSlewed[3].cur) && vv < 250026.640625f) {
        t4Vector n = self->mRider->mGroundNormal;
        if (0.866100013256073f < n.z) {
            float damp = 0.8999999761581421f;
            if (vv < 2.0f)
                damp = 0.0f;
            vu0ScaleEq_13D818(rel, damp);
            self->mRider->mVelocity = vu0Add_13D818(self->mRider->GroundVelocity, rel);
            force = t4vect_Zero_13D818;
        }
    }

    t4Vector diff = vu0Sub_13D818(grav, force);
    vu0AddEq_13D818(self->mRider->mPosition, vu0Scale_13D818(self->mRider->mVelocity, dt));
    vu0AddEq_13D818(self->mRider->mVelocity, vu0Scale_13D818(force, dt));
    rel = vu0Sub_13D818(self->mRider->mVelocity, self->mRider->GroundVelocity);
    float heightAfter = vu0Dot_13D818(self->mRider->mPosition, self->mRider->DampGroundNormal);
    float lo2 = (1.0f - self->mRider->mSlewed[4].cur) * -20.0f;
    float hi2 = self->mRider->mSlewed[4].cur * 10.0f;
    float ch = self->mRider->mSlewed[18].cur + (heightBefore - heightAfter) * 10.0f;
    ch *= 1.0f - dt * 20.0f;
    ch = clamp_13D818(ch, lo2, hi2);
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 10 || cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 4 ||
        cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) == 5 || cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2) == 22)
        ch = 0.0f;
    cRider* rider = self->mRider;
    rider->mSlewed[18].target = ch * scale;
    rider->mSlewed[18].rate = dt * 100.0f;

    float yawRate = 0.0f;
    float relSpeed = vu0Len_13D818(rel);
    int st = cRider_getControlState(self->mRider);
    if ((st != 2 || (cRider_getControlState(self->mRider) == 2 && 0.6499999761581421f < self->mRider->mGroundNormal.z)) &&
        9.999999747378752e-06f < relSpeed) {
        t4Vector ax = vu0Div_13D818(vu0Cross_13D818(rel, self->mRider->mGroundForward), relSpeed);
        float yaw = -func_0031C128(clamp_13D818(vu0Dot_13D818(ax, self->mRider->mGroundNormal), -1.0f, 1.0f));
        float turnKickFlare = snow->TurnKickFlare;
        float turn = (turnKickFlare * ((1.0f - lean * lean) * 0.4000000059604645f) + 1.100000023841858f) * lean *
                     (curve_13D818(kMaxTurnKickAngle, relSpeed * 0.035999998450279236f) * 0.01745329424738884f);
        turn /= self->mRider->mSlewed[4].cur * 0.9955680966377258f + 1.0f;
        turn *= curveB_13D818(kKickAngleSpeed, relSpeed * 0.035999998450279236f);
        yaw += turn;
        if (vu0Dot_13D818(rel, self->mRider->mGroundForward) < 0.0f)
            yaw = -yaw;
        float rate = (snow->TurnHeadAdjustRate * (relSpeed * 9.999999974752427e-07f) * relSpeed * dt) <? 1.0f;
        float lowSpd = 555.5555419921875f; // kph2cps(20)
        t4Vector dir;
        t4Vector side = vu0Cross_13D818(self->mRider->mGroundNormal, t4vect_ZAxis_13D818);
        float sl = vu0Len_13D818(side);
        if (0.0010000000474974513f < sl) {
            vu0DivEq_13D818(side, sl);
            dir = vu0Cross_13D818(self->mRider->mGroundNormal, side);
        } else {
            dir = front_13D818(self->mRider);
        }
        float fallLineAlign = absf_13D818(vu0Dot_13D818(rel, dir)) / relSpeed;
        float speedRatio = (relSpeed / lowSpd) <? 1.0f;
        float c = (speedRatio * (0.5f - fallLineAlign) * (snow->SlopeFactorHeadMult + snow->SlopeFactorHeadMult)) >? 0.0f;
        float grip = (snow->BackTurnHeadMult + c) <? 1.0f;
        if (grip <= (yaw <= 0.0f ? -lean : lean) * 0.8999999761581421f)
            rate = (yaw <= 0.0f) ? rate * 0.8999999761581421f * -lean : rate * 0.8999999761581421f * lean;
        else
            rate *= grip;
        if (self->mRider->mSpinRate != 0.0f)
            rate = 0.0f;
        float maxTurn = 6.283185958862305f;
        yawRate = clamp_13D818(yaw * rate, dt * -maxTurn, dt * maxTurn);
    }

    if (self->mRider->mSpinRate != 0.0f) {
        float spinDecay = 8.203048706054688f;       // deg2rad(470): kMaxSpinOutRate
        float spinDecelAng = 0.7853982448577881f;   // deg2rad(45): kSpinOutDecelAng
        float spinEndAng = 0.2617994248867035f;     // deg2rad(15): kSpinOutEndAng
        t4Vector flat = vu0Sub_13D818(rel, vu0Scale_13D818(self->mRider->mGroundNormal, vu0Dot_13D818(rel, self->mRider->mGroundNormal)));
        float ang;
        if (self->mRider->IsRidingFakie != 0)
            ang = atan2_13D818(-vu0Dot_13D818(flat, self->mRider->mGroundSide), -vu0Dot_13D818(flat, self->mRider->mGroundForward));
        else
            ang = atan2_13D818(vu0Dot_13D818(flat, self->mRider->mGroundSide), vu0Dot_13D818(flat, self->mRider->mGroundForward));
        if (0.0f < ang * self->mRider->mSpinRate)
            ang = wrap_13D818(3.1415927410125732f - ang);
        else
            ang = -ang;
        if (3.1415927410125732f < absf_13D818(self->mRider->mSpinRate)) {
            ang += (ang < 0.0f) ? -3.1415927410125732f : 3.1415927410125732f;
            float step = spinDecay * dt;
            float sr = self->mRider->mSpinRate;
            float nsr;
            if (step < sr)
                nsr = sr - step;
            else if (sr < -step)
                nsr = sr + step;
            else
                nsr = 0.0f;
            self->mRider->mSpinRate = nsr;
        }
        if (absf_13D818(ang) < spinEndAng) {
            self->mRider->mSpinRate = 0.0f;
        } else {
            float spinYaw = -dt * clamp_13D818(ang * (spinDecay / spinDecelAng), -spinDecay, spinDecay);
            if (spinYaw < 0.0f && yawRate < 0.0f)
                yawRate = yawRate <? spinYaw;
            else if (0.0f < spinYaw && 0.0f < yawRate)
                yawRate = yawRate >? spinYaw;
            else
                yawRate += spinYaw;
        }
    }

    float fwd = (0.0f < relSpeed) ? vu0Dot_13D818(rel, front_13D818(self->mRider)) / relSpeed : 0.0f;
    fwd = (fwd - 0.10000000149011612f) >? 0.0f;
    float kf = curve_13D818(D_004A1158, relSpeed * 0.035999998450279236f);
    yawRate -= fwd * self->mRider->mSlewed[0].cur * kf;
    if (yawRate != 0.0f)
        cRider_rotateOrientation(self->mRider, self->mRider->mGroundNormal, yawRate);

    sHit_13D1B8 hit;
    float dist;
    int got = cGroundMotion_setupGroundVectors(self, &hit, &dist);
    int lost = 0;
    if (got == 0) {
        lost = self->mRider->mAllowNaturalAir == 0;
    } else if (hit.obj != 0) {
        char* handler = *(char**)(hit.obj + 0xC);
        if (handler) {
            sVEnt_13D818* vt = *(sVEnt_13D818**)(handler + 0xC);
            vt[43].fn(vthis(handler, vt[43]), &hit, &diff);
        }
    }

    int surf = self->mRider->mSurfaceType;
    if (!(surf == 2 || surf == 3 || surf == 13)) {
        float vn = vu0Dot_13D818(rel, self->mRider->mGroundNormal);
        float lenBefore = vu0Len_13D818(rel);
        t4Vector bump = vu0Scale_13D818(self->mRider->mGroundNormal, vn * -0.4000000059604645f);
        if (bump.z < -40.0f)
            bump.z = -40.0f;
        vu0AddEq_13D818(rel, bump);
        float lenAfter = vu0Len_13D818(rel);
        if (lenAfter != 0.0f)
            vu0ScaleEq_13D818(rel, lenBefore / lenAfter);
        self->mRider->mVelocity = vu0Add_13D818(self->mRider->GroundVelocity, rel);
    }

    float maxH = snow->AirHeight;
    if (got == 0 || maxH < self->mRider->SnowOffset) {
        if (self->mRider->mAllowNaturalAir != 0 || lost)
            self->ForceNaturalAir = 1;
    } else {
        if (dist < 5.0f) {
            t4Vector biasedNormal = vu0Normalize_13D818(vu0Add_13D818(self->mRider->mGroundNormal,
                sVec4C_13D818(0.0f, 0.0f, self->mRider->mGroundNormal.z * 0.5f, 0.0f)));
            t4Vector axis = vu0Cross_13D818(biasedNormal, up_13D818(self->mRider));
            float al = vu0Len_13D818(axis);
            if (0.0010000000474974513f < al) {
                vu0DivEq_13D818(axis, al);
                float tilt = -func_0031C128(al <? 1.0f) * 0.01666666753590107f * snow->TurnAdjustRate;
                cRider_rotateOrientation(self->mRider, axis, tilt);
            }
        }
        int layer = cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2);
        int anim = cRiderAnimBase_getCurrentAnimID(self->mRider->mRiderAnim, 2);
        int onSlope = cRider_getControlState(self->mRider) != 1 && (self->mRider->mSurfaceType == 2 || self->mRider->mSurfaceType == 3);
        int brace = !onSlope && layer != 10 && layer != 5 && anim != 22 && anim != 2;
        if (onSlope) {
            float tgt = curve_13D818(kPowderTiltBwd, vu0Len_13D818(rel) * 0.035999998450279236f);
            cRider* rider2 = self->mRider;
            rider2->mSlewed[2].target = tgt;
            rider2->mSlewed[2].rate = absf_13D818(rider2->mSlewed[2].cur - tgt) * (dt * 3.0f);
        } else {
            cRider* rider2 = self->mRider;
            rider2->mSlewed[2].target = 0.0f;
            rider2->mSlewed[2].rate = dt * 3.0f;
        }
        if (brace) {
            cRider* rider3 = self->mRider;
            rider3->mSlewed[17].target = 1.0f;
            rider3->mSlewed[17].rate = dt * 3.0f;
        } else {
            cRider* rider3 = self->mRider;
            rider3->mSlewed[17].target = 0.0f;
            rider3->mSlewed[17].rate = dt * 3.0f;
        }
        float base = absf_13D818(lean) + absf_13D818(self->mRider->mSlewed[3].cur) + 0.10000000149011612f;
        float amt = vu0Len_13D818(rel) * 0.0003600000054575503f;
        if (1.0f < amt)
            amt = 1.0f;
        float amp = amt * base;
        self->ChatterTime += amp * 2.6500000953674316f;
        if (6.283185958862305f <= self->ChatterTime)
            self->ChatterTime -= 6.283185958862305f;
        self->mRider->ChatterMag = amp * (fCos(self->ChatterTime) * 7.5f);
    }
    cRider_updateOrientationImplicit(self->mRider);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_resolve);
#ifdef SKIP_ASM
extern "C" void cRider_applyJumpPop(void* rider, float v);
// PORT: cRider_updateBoundBox is declared void* in this unit; this caller ignores the result (void call).
extern "C" void cRider_updateBoundBox_v13F178(void* rider, int a1) __asm__("cRider_updateBoundBox");
extern "C" void cRider_checkObjectCollisions(void* rider, int a1);
extern "C" void cRider_checkOpponentCollisions(void* rider);
extern "C" void cRider_checkForReset(void* rider, int a, int b);
extern "C" void cGroundMotion_checkWallCollisions(cGroundMotion* self);
extern "C" void cSSXAudio_Lift(void* snd, void* rider);
int cRider_getMotionState(void* rider);
// PORT: cRider_setMotionState is defined with one parameter but this caller passes (rider, 1).
void cRider_setMotionState_impl_13F178(void* rider, int v) __asm__("cRider_setMotionState__FPv");
extern int kOnGroundSphereMask;


extern "C" void cRider_flagEventWipeout(void* rider, int a1, int a2, int a3, tCollideInfo* info);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4HP vu0Scale_13F178(const sVec4HP& v, float s)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4HP vu0Add_13F178(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4HP vu0Normalize_13F178(const sVec4HP& v)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_13F178(const sVec4HP& v)
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

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void vu0ScaleEq_13F178(sVec4HP& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_13F178(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

#define RIDER_13F178 (*(char**)((char*)self + 0x18))

/**
 * Resolves the ground motion: natural-air pop or fatal surfaces, collisions, and the audio values.
 *
 * With ForceNaturalAir set it pops the rider with cRider_applyJumpPop(rider, -1) and, while still in MS_Ground
 * after the checks, switches to MS_Air with the lift sound. Otherwise surface type 0x12 starts a wipeout
 * (anim 0x168) and reset surfaces (+0x44) or triangles with flag 0x2 force an auto reset. Walls
 * (cGroundMotion_checkWallCollisions, sphere-tree filter kOnGroundSphereMask), cRider_checkObjectCollisions() and opponents are
 * checked; rider +0x390 is blended toward mGroundNormal, the speed is capped at rider +0x2E4, and mAudioSpeed
 * (speed / 3333.33, at most 1), mAudioEdging (|channel 0|) and mAudioBraking (|channel 2|) are set.
 */
extern "C" void cGroundMotion_resolve(cGroundMotion* self)
{
    if (self->ForceNaturalAir != 0) {
        cRider_applyJumpPop(RIDER_13F178, -1.0f);
    } else {
        cRider* rider = (cRider*)(RIDER_13F178);
        int idx = rider->mSurfaceType;
        char* snow = (char*)&SSXApp->Game->SurfPropCache->SurfData[idx];
        int fl;
        if (idx == 18) {
            tCollideInfo info;
            (*(sVec4HP*)&info.Pos) = *(sVec4HP*)&rider->mPosition;
            (*(sVec4HP*)&info.Direct) = vu0Normalize_13F178(*(sVec4HP*)&rider->mVelocity);
            (*(sVec4HP*)&info.Reflect) = *(sVec4HP*)(RIDER_13F178 + 0x370);
            (*(int*)&info.Vel) = 0;
            cRider_flagEventWipeout(RIDER_13F178, 360, 0, *(int*)(RIDER_13F178 + 0x438), &info);
        } else if (*(int*)(snow + 0x44) != 0 || (fl = *(short*)&rider->SurfaceFlags, (fl & 2) != 0)) {
            cRider_checkForReset(rider, 0, 1);
        }
    }
    cRider_updateBoundBox_v13F178(RIDER_13F178, 0);
    *(int*)(*(char**)(RIDER_13F178 + 0xAA0) + 0x28) = kOnGroundSphereMask;
    cGroundMotion_checkWallCollisions(self);
    *(unsigned int*)(*(char**)(RIDER_13F178 + 0xAA0) + 0x28) = 0xFFFFFFFF;
    cRider_checkObjectCollisions(RIDER_13F178, 0);
    cRider_checkOpponentCollisions(RIDER_13F178);
    if (self->ForceNaturalAir != 0 && cRider_getMotionState(RIDER_13F178) == 0) {
        cRider_setMotionState_impl_13F178(RIDER_13F178, 1);
        cSSXAudio_Lift((char*)gSSXAudio, RIDER_13F178);
    }
    cRider* rider = (cRider*)(RIDER_13F178);
    *(sVec4HP*)&rider->DampGroundNormal = vu0Normalize_13F178(vu0Add_13F178(*(sVec4HP*)(RIDER_13F178 + 0x390), vu0Scale_13F178(*(sVec4HP*)&rider->mGroundNormal, 0.5f)));
    cRider* rider3 = (cRider*)(RIDER_13F178);
    float len = vu0Length_13F178(*(sVec4HP*)&rider3->mVelocity);
    if (rider3->TerminalVelocity < len) {
        vu0ScaleEq_13F178(*(sVec4HP*)&rider3->mVelocity, rider3->TerminalVelocity / len);
    }
    float k = 0.0f;
    float vmax = 3333.33349609375f;
    if (0.0f <= len) {
        k = 1.0f;
        if (len <= vmax) {
            k = len / vmax;
        }
    }
    *(float*)(RIDER_13F178 + 0x75C) = k;
    cRider* rider4 = (cRider*)(RIDER_13F178);
    rider4->mAudioEdging = absf_13F178(rider4->mSlewed[0].cur);
    cRider* rider5 = (cRider*)(RIDER_13F178);
    rider5->mAudioBraking = absf_13F178(rider5->mSlewed[3].cur);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_loseFocus);
#ifdef SKIP_ASM
extern "C" int AITick();

/**
 * Ends the ground motion: eases slewed channels 2, 17 and 18 back to 0 and records the AI tick.
 *
 * Channels 2 and 17 get rate 0.05 and channel 18 rate 1.67. AITickOnLoseFocus becomes the current AI tick, or
 * that tick minus 500 when it was -1 (marked by a natural-air pop), so cGroundMotion_gainFocus() makes no
 * landing speed cut then.
 */
extern "C" void cGroundMotion_loseFocus(cGroundMotion* self)
{
    cRider* rider = self->mRider;
    *(int*)&rider->mSlewed[17].target = 0;
    rider->mSlewed[17].rate = 0.05000000447034836f;
    cRider* rider2 = self->mRider;
    rider2->mSlewed[2].rate = 0.05000000447034836f;
    *(int*)&rider2->mSlewed[2].target = 0;
    cRider* rider3 = self->mRider;
    rider3->mSlewed[18].rate = 1.6666667461395264f;
    *(int*)&rider3->mSlewed[18].target = 0;
    if (self->AITickOnLoseFocus == -1)
        self->AITickOnLoseFocus = AITick() - 500;
    else
        self->AITickOnLoseFocus = AITick();
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_checkWallCollisions);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(void*);
int cRider_getControlState(void* rider);
extern "C" float cOverlapList_terrainIntersect1(void* world, void* seg, void* hit, int flags);
extern "C" void cRider_moveFromCollision(void* rider, void* v);
extern "C" void cRider_turnFromCollision(void* rider, void* hit);
extern "C" void cRider_reactToCollision(void* rider, void* info, int id);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_WallCollision(void* snd, void* rider);
extern "C" void* cWorldRider_cWorldRider(void* self, void* tree, int flag);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);



struct sInfo_13F488 {
    sVec4HP pos;
    sVec4HP dir;
    sVec4HP normal;
    float depth;
} __attribute__((aligned(16)));

struct sVEnt_13F488 { short delta; short index; bool (*fn)(void*); };
struct sVEntH_13F488 { short delta; short index; void (*fn)(void*, void*); };

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4HP vu0Scale_13F488(const sVec4HP& v, float s)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4HP vu0Sub_13F488(const sVec4HP& a, const sVec4HP& b)
{
    sVec4HP r;
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

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void vu0AddEq_13F488(sVec4HP& a, const sVec4HP& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_13F488(const sVec4HP& a, const sVec4HP& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sVec4HP vu0Normalize_13F488(const sVec4HP& v)
{
    sVec4HP r;
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

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_13F488(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline bool riderFlag_13F488(char* r)
{
    char* sub = r + 0x6C0;
    sVEnt_13F488* vt = *(sVEnt_13F488**)sub;
    return vt[8].fn(vthis(sub, vt[8]));
}

#define RIDER (*(char**)((char*)self + 0x18))
#define RV(off) (*(sVec4HP*)(RIDER + (off)))

/**
 * Pushes the grounded rider out of walls and bounces its velocity off them.
 *
 * Skipped in CS_Reset, and for computer riders while rider +0x4C8 < 50 and |rider +0x498 - mPosition.z| < 50
 * (probably: close to their AI path). A sphere-tree query (mWallHint) finds the overlap and reports it to the
 * hit object (handler slot 42); hits straight against mGroundNormal are ignored and downward ones lose their
 * mGroundNormal part. The rider moves out by 1.1 times the depth; when moving into the wall (relative to the
 * wall's velocity) the inward velocity is removed plus a bounce of max(5% of it, 27.78), then
 * cRider_turnFromCollision(), cRider_reactToCollision() and the wall sound (cSSXAudio_WallCollision) follow.
 */
extern "C" void cGroundMotion_checkWallCollisions(cGroundMotion* self)
{
    if (cRider_getControlState(RIDER) == 9)
        return;
    if (!riderFlag_13F488(RIDER)) {
        cRider* rider = (cRider*)(RIDER);
        if (rider->PathDistXY < 50.0f && absf_13F488(rider->PathPos.z - rider->mPosition.z) < 50.0f)
            return;
    }
    tIntersectInfo hit;
    cWorldRider sph;
    cRider* rider = (cRider*)(RIDER);
    cWorldRider_cWorldRider(&sph, rider->mSphereTree, !riderFlag_13F488(((char*)rider)));
    sph.useRejectNormal = 1;
    (*(sVec4HP*)&sph.rejectNormal) = RV(880);
    float penetration = cOverlapList_terrainIntersect1(*(void**)(RIDER + 0x860), &sph, &hit, *(int*)(RIDER + 0x868));
    if (penetration < 0.0f)
        { cWorldRider__cWorldRider(&sph, 2); return; }
    if (((char*)hit.Inst) != 0) {
        char* handler = (char*)hit.Inst->m_pActiveNode;
        if (handler) {
            sVEntH_13F488* vt = *(sVEntH_13F488**)(handler + 0xC);
            vt[42].fn(vthis(handler, vt[42]), &hit);
        }
    }
    {
        float dot = vu0Dot_13F488(RV(880), (*(sVec4HP*)&hit.Inorm));
        if (dot < -0.9998999834060669f)
            { cWorldRider__cWorldRider(&sph, 2); return; }
        if (dot < 0.0f)
            (*(sVec4HP*)&hit.Inorm) = vu0Normalize_13F488(vu0Sub_13F488((*(sVec4HP*)&hit.Inorm), vu0Scale_13F488(RV(880), dot)));
    }
    {
        sVec4HP push = vu0Scale_13F488(vu0Scale_13F488((*(sVec4HP*)&hit.Inorm), 1.100000023841858f), penetration);
        cRider_moveFromCollision(RIDER, &push);
        sVec4HP netVelocity = vu0Sub_13F488(RV(0x1E0), (*(sVec4HP*)&hit.Ivel));
        float dot2 = vu0Dot_13F488(vu0Scale_13F488((*(sVec4HP*)&hit.Inorm), -1.0f), netVelocity);
        if (dot2 < 0.0f)
            { cWorldRider__cWorldRider(&sph, 2); return; }
        sVec4HP n = vu0Normalize_13F488(netVelocity);
        float k = dot2 * 0.05000000074505806f;
        if (k < 27.77777862548828f)
            k = 27.77777862548828f;
        vu0AddEq_13F488(RV(0x1E0), vu0Scale_13F488((*(sVec4HP*)&hit.Inorm), dot2 + k));
        cRider_turnFromCollision(RIDER, &hit);
        cRider_updateOrientationImplicit(RIDER);
        sInfo_13F488 info;
        info.pos = (*(sVec4HP*)&hit.Ipos);
        info.dir = n;
        info.normal = (*(sVec4HP*)&hit.Inorm);
        info.depth = dot2;
        cRider_reactToCollision(RIDER, &info, hit.Stype);
        cSSXAudio_WallCollision(cAudioInterface_bxAudio(), RIDER);
    }
    cWorldRider__cWorldRider(&sph, 2);
}
#undef RV
#undef RIDER
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_writeToReplayFrame);
#ifdef SKIP_ASM

/**
 * Writes the ground-motion state (its first 0x18 bytes, cGroundMotionState) to a replay frame.
 *
 * Calls the stream's write virtual (slot 1) with this object and the size 0x18.
 *
 * @param obj  replay stream to write to (On Tour cReplayFramePtr*)
 */
extern "C" void cGroundMotion_writeToReplayFrame(cGroundMotion* self, cBxReplayFrame* obj)
{
    obj->write((const char*)self, 0x18);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion_readFromReplayFrame);
#ifdef SKIP_ASM

/**
 * Reads the ground-motion state (its first 0x18 bytes, cGroundMotionState) back from a replay frame.
 *
 * Calls the stream's read virtual (slot 2) with this object and the size 0x18.
 *
 * @param obj  replay stream to read from (On Tour cReplayFramePtr*)
 */
extern "C" void cGroundMotion_readFromReplayFrame(cGroundMotion* self, cBxReplayFrame* obj)
{
    obj->read((char*)self, 0x18);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013F8B8);
#ifdef SKIP_ASM
/**
 * Returns its first argument and does nothing else.
 *
 * It is variadic: the 60 bytes only spill the argument registers. Nothing in the source calls it.
 *
 * @return its first argument
 */
extern "C" void* func_0013F8B8(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013F8F8);
#ifdef SKIP_ASM
struct sSortObj_F8F8 {
    char pad[0x78];
    unsigned int key;   // 0x78
};

/**
 * Merge-sorts the pointers from @p first to @p last by the unsigned key at +0x78 of each object, into @p buf.
 *
 * Two elements are compared directly; longer ranges sort each half recursively (the second half in place in @p first)
 * and merge them. The input range is used as scratch. @p cmp is not used. Nothing in the source calls it.
 *
 * @param first  the first pointer
 * @param last  one past the last pointer
 * @param buf  receives the sorted pointers; needs room for last - first entries
 * @param cmp  unused
 */
// Merge sort of an object-pointer array by key, using buf as scratch; the sorted result ends up in buf.
extern "C" void func_0013F8F8(sSortObj_F8F8** first, sSortObj_F8F8** last, sSortObj_F8F8** buf, void* cmp)
{
    int n = last - first;
    if (n == 2) {
        if (first[0]->key < first[1]->key) {
            buf[0] = first[0];
            buf[1] = first[1];
        } else {
            buf[0] = first[1];
            buf[1] = first[0];
        }
        return;
    }
    sSortObj_F8F8** mid = first + (n + 1) / 2;
    sSortObj_F8F8** bmid = buf + n / 2;
    sSortObj_F8F8** bend = buf + n;
    if (n < 3)
        *bmid = *first;
    else
        func_0013F8F8(first, mid, bmid, cmp);
    if (n < 4)
        *first = *mid;
    else
        func_0013F8F8(mid, last, first, cmp);
    sSortObj_F8F8** rightCur = bmid;
    sSortObj_F8F8** bend2 = bmid + (n + 1) / 2;
    sSortObj_F8F8** leftCur = first;
    sSortObj_F8F8** aend = first + n / 2;
    sSortObj_F8F8** out = buf;
    while (out < bend) {
        if (leftCur < aend) {
            if (rightCur < bend2 && (*rightCur)->key < (*leftCur)->key)
                *out++ = *rightCur++;
            else
                *out++ = *leftCur++;
        } else {
            *out++ = *rightCur++;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_0013FAD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// Sort entry point: small ranges are handled inline, larger ones go to the merge sort.
struct sSortObj_FAD8;
extern "C" void func_0013F8F8_FAD8(sSortObj_FAD8**, sSortObj_FAD8**, sSortObj_FAD8**, void*) __asm__("func_0013F8F8");
/**
 * Merge-sorts the pointers from @p first to @p last into @p buf by each object's unsigned key at +0x78.
 *
 * A range of two or more goes to func_0013F8F8; a single pointer is copied; an empty range does nothing. Called twice
 * by cFocusMan_update (focus and unfocus instance lists).
 *
 * @param first  the first pointer
 * @param last  one past the last pointer
 * @param buf  receives the sorted pointers
 * @param cmp  passed on unused (an InstanceCompare in the caller)
 */
extern "C" void func_0013FAD8(sSortObj_FAD8** first, sSortObj_FAD8** last, sSortObj_FAD8** buf, void* cmp)
{
    int n = last - first;
    if (n >= 2)
        func_0013F8F8_FAD8(first, last, buf, cmp);
    else if (n == 1)
        *buf = *first;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", __sti__all_in_one_ai_cpp);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// Packed index word: low byte and upper 24 bits both hold the index (static-init objects).
struct sIdx { unsigned int id:8; int val:24; };
static inline void SetCol4(float* c, float r, float g, float b, float a) { c[0] = r; c[1] = g; c[2] = b; c[3] = a; }
static inline void SetKeys8(float* c, float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7) { c[0] = a0; c[1] = a1; c[2] = a2; c[3] = a3; c[4] = a4; c[5] = a5; c[6] = a6; c[7] = a7; }
static inline void InitIdx(sIdx* e, int n) { e->id = n; e->val = n; }
extern sIdx D_004A4C08;
extern int D_004A4C10;
extern int D_004A4C14;
extern float kHighSpeedCollideThresh;
extern float nFinishLineControl_kFinishStopThreshCPS;
extern float kSpinOutOfAng;
extern float kIntoSpinAbortAng;
extern float kMaxAdjSpinAng;
extern float kMaxAdjFlipAng;
extern float kMaxPressFlipAng;
extern float kSpinNoDecayThresh;
extern float kFlipNoDecayThresh;
extern float kAirAdjFwdMaxAng;
extern "C" void InitTerminalVelocityTable(void);
extern sIdx D_004A4C18;
extern sIdx D_004A4C20;
extern sIdx D_004A4C28;
extern sIdx D_004A4C30;
extern sIdx D_004A4C38;
extern sIdx D_004A4C40;
extern sIdx D_004A4C48;
extern sIdx D_004A4C50;
extern sIdx D_004A4C58;
extern sIdx D_004A4C60;
extern sIdx D_004A4C68;
extern sIdx D_004A4C70;
extern sIdx D_004A4C78;
extern sIdx D_004A4C80;
extern float D_004A5E60[4];
extern float D_004A5E70[4];
extern float D_004A5E80[4];
extern float D_004A5E90[4];
extern float D_004A5EA0[4];
extern float D_004A5EB0[4];
extern float D_004A5EC0[4];
extern float D_004A5ED0[4];
extern float D_004A5EE0[4];
extern float D_004A5EF0[4];
extern float D_004A5F00[4];
extern float D_004A5F10[4];
extern float D_004A5F20[4];
extern float D_004A5F30[4];
extern float D_004A5F40[4];
extern float D_004A5F50[4];
extern float D_004A5F60[4];
extern float D_004A5F70[4];
extern float D_004A5F80[4];
extern float D_004A5F90[4];
extern float D_004A5FA0[4];
extern float D_004A5FB0[4];
extern float D_004A5FC0[4];
extern float D_004A5FD0[4];
extern float D_004A5FE0[4];
extern float D_004A5FF0[4];
extern float D_004A6000[4];
extern float D_004A6010[4];
extern float D_004A6020[4];
extern float D_004A6030[4];
extern float D_004A6040[4];
extern float D_004A6050[4];
extern float D_004A6060[4];
extern float D_004A6070[4];
extern float D_004A6080[4];
extern float D_004A6090[4];
extern float D_004A60A0[4];
extern float D_004A60B0[4];
extern float D_004A60C0[4];
extern float D_004A60D0[4];
extern float D_004A60E0[4];
extern float D_004A60F0[4];
extern float D_004A6100[4];
extern float D_004A6110[4];
extern float D_004A6120[4];
extern float D_004A6130[4];
extern float D_004A6140[4];
extern float D_004A6150[4];
extern float D_004A6160[4];
extern float D_004A6170[4];
extern float D_004A6180[4];
extern float D_004A6190[4];
extern float D_004A61A0[4];
extern float D_004A61B0[4];
extern float D_004A61C0[4];
extern float D_004A61D0[4];
extern float kRiderBoxOffset[4];
extern float D_004A61F0[8];
extern float D_004A6210[8];
extern float D_004A6230[8];
extern float D_004A6250[8];
extern float D_004A6270[8];
extern float D_004A6290[8];
extern float D_004A62B0[8];
extern float D_004A62D0[8];
extern float D_004A62F0[8];
void sti_0013FB20(int init, int prio) __asm__("__sti__all_in_one_ai_cpp");
/**
 * Static initializer of the AI unit (all_in_one_ai.cpp): sets its constant tables and builds the terminal-velocity
 * table.
 *
 * Runs only when @p init is set and @p prio is 0xFFFF. It writes dozens of 4-float colour/curve constants (D_004A5E60
 * ... kRiderBoxOffset), the packed anim-id words D_004A4C08 (-1) and D_004A4C18 ... D_004A4C80 (0 ... 13), kHighSpeedCollideThresh =
 * 2222.22 (80 kph in cm/s), nFinishLineControl_kFinishStopThreshCPS = 277.78 (10 kph), angle limits in radians (45, 5,
 * 100, 20 and 360 degrees) and seven 4-key (x, y) curves, and calls InitTerminalVelocityTable (On Tour InitTerminalVelocityTable).
 *
 * The C definition is sti_0013FB20 with an asm label.
 *
 * @param init  nonzero when constructing (g++ passes 0 for the matching destructor pass)
 * @param prio  the init priority; only 0xFFFF does anything
 */
void sti_0013FB20(int init, int prio)
{
    if (prio == 0xFFFF && init) {
        D_004A5E60[0] = 1.0f;
        D_004A5E60[1] = 1.0f;
        D_004A5E60[2] = 1.0f;
        D_004A5E60[3] = 0.5f;
        D_004A5E70[0] = 0.499868006f;
        D_004A5E70[1] = 1.0f;
        D_004A5E70[2] = 1.0f;
        D_004A5E70[3] = 0.0f;
        D_004A5E80[0] = 0.129517004f;
        D_004A5E80[1] = 1.0f;
        D_004A5E80[2] = 1.0f;
        D_004A5E80[3] = 0.0f;
        D_004A5E90[0] = 0.224269003f;
        D_004A5E90[1] = 0.940231025f;
        D_004A5E90[2] = 0.54250598f;
        D_004A5E90[3] = 0.000403000013f;
        D_004A5EA0[0] = 0.465207994f;
        D_004A5EA0[1] = 0.321705014f;
        D_004A5EA0[2] = 0.994343996f;
        D_004A5EA0[3] = 0.0f;
        D_004A5EB0[0] = 0.177926004f;
        D_004A5EB0[1] = 1.0f;
        D_004A5EB0[2] = 1.0f;
        D_004A5EB0[3] = 0.0f;
        D_004A5EC0[0] = 0.441174001f;
        D_004A5EC0[1] = 0.413886011f;
        D_004A5EC0[2] = 0.321126014f;
        D_004A5EC0[3] = 0.439823985f;
        D_004A5ED0[0] = 0.500997007f;
        D_004A5ED0[1] = 0.248495996f;
        D_004A5ED0[2] = 1.0f;
        D_004A5ED0[3] = 1.0f;
        D_004A5EE0[0] = 0.276872993f;
        D_004A5EE0[1] = 0.639562011f;
        D_004A5EE0[2] = 0.134239003f;
        D_004A5EE0[3] = 0.0f;
        D_004A5EF0[0] = 0.893643975f;
        D_004A5EF0[1] = 0.526346982f;
        D_004A5EF0[2] = 0.505145013f;
        D_004A5EF0[3] = 0.868730009f;
        D_004A5F00[0] = 1.0f;
        D_004A5F00[1] = 1.0f;
        D_004A5F00[2] = 1.0f;
        D_004A5F00[3] = 1.0f;
        D_004A5F10[0] = 1.0f;
        D_004A5F10[1] = 0.0f;
        D_004A5F10[2] = 0.0f;
        D_004A5F10[3] = 0.0f;
        D_004A5F20[0] = 1.0f;
        D_004A5F20[1] = 0.550000012f;
        D_004A5F20[2] = 0.535000026f;
        D_004A5F20[3] = 0.550000012f;
        D_004A5F30[0] = 1.0f;
        D_004A5F30[1] = 1.0f;
        D_004A5F30[2] = 1.0f;
        D_004A5F30[3] = 1.0f;
        D_004A5F40[0] = 1.0f;
        D_004A5F40[1] = 1.0f;
        D_004A5F40[2] = 1.0f;
        D_004A5F40[3] = 1.0f;
        D_004A5F50[0] = 1.0f;
        D_004A5F50[1] = 0.0f;
        D_004A5F50[2] = 0.0f;
        D_004A5F50[3] = 0.0f;
        D_004A5F60[0] = 1.0f;
        D_004A5F60[1] = 0.0f;
        D_004A5F60[2] = 0.0f;
        D_004A5F60[3] = 0.0f;
        D_004A5F70[0] = 1.0f;
        D_004A5F70[1] = 1.0f;
        D_004A5F70[2] = 1.0f;
        D_004A5F70[3] = 1.0f;
        D_004A5F80[0] = 1.0f;
        D_004A5F80[1] = 0.5f;
        D_004A5F80[2] = 1.0f;
        D_004A5F80[3] = 0.5f;
        D_004A5F90[0] = 1.0f;
        D_004A5F90[1] = 0.5f;
        D_004A5F90[2] = 1.0f;
        D_004A5F90[3] = 0.5f;
        D_004A5FA0[0] = 1.0f;
        D_004A5FA0[1] = 0.5f;
        D_004A5FA0[2] = 1.0f;
        D_004A5FA0[3] = 0.5f;
        D_004A5FB0[0] = 1.0f;
        D_004A5FB0[1] = 0.782811999f;
        D_004A5FB0[2] = 0.0617190003f;
        D_004A5FB0[3] = 0.0398439988f;
        D_004A5FC0[0] = 1.0f;
        D_004A5FC0[1] = 1.0f;
        D_004A5FC0[2] = 0.798825979f;
        D_004A5FC0[3] = 0.0f;
        D_004A5FD0[0] = 0.988671005f;
        D_004A5FD0[1] = 0.923049986f;
        D_004A5FD0[2] = 0.343116999f;
        D_004A5FD0[3] = 0.0f;
        D_004A5FE0[0] = 1.0f;
        D_004A5FE0[1] = 1.0f;
        D_004A5FE0[2] = 1.0f;
        D_004A5FE0[3] = 1.0f;
        D_004A5FF0[0] = 1.0f;
        D_004A5FF0[1] = 1.0f;
        D_004A5FF0[2] = 0.797643006f;
        D_004A5FF0[3] = 0.0f;
        D_004A6000[0] = 1.0f;
        D_004A6000[1] = 0.92226702f;
        D_004A6000[2] = 0.341796994f;
        D_004A6000[3] = 0.0f;
        D_004A6010[0] = 1.0f;
        D_004A6010[1] = 0.784763992f;
        D_004A6010[2] = 0.0605470017f;
        D_004A6010[3] = 0.0406249985f;
        D_004A6020[0] = 1.0f;
        D_004A6020[1] = 0.800000012f;
        D_004A6020[2] = 0.449999988f;
        D_004A6020[3] = 1.0f;
        D_004A6030[0] = 1.0f;
        D_004A6030[1] = 0.328126013f;
        D_004A6030[2] = 1.0f;
        D_004A6030[3] = 0.353906006f;
        D_004A6040[0] = 1.0f;
        D_004A6040[1] = 1.0f;
        D_004A6040[2] = 1.0f;
        D_004A6040[3] = 1.0f;
        D_004A6050[0] = 1.0f;
        D_004A6050[1] = 1.0f;
        D_004A6050[2] = 1.0f;
        D_004A6050[3] = 1.0f;
        D_004A6060[0] = 1.0f;
        D_004A6060[1] = 0.861329019f;
        D_004A6060[2] = 0.381247997f;
        D_004A6060[3] = 0.0f;
        D_004A6070[0] = 1.0f;
        D_004A6070[1] = 1.0f;
        D_004A6070[2] = 1.0f;
        D_004A6070[3] = 1.0f;
        D_004A6080[0] = 1.0f;
        D_004A6080[1] = 0.324438006f;
        D_004A6080[2] = 1.0f;
        D_004A6080[3] = 0.0f;
        D_004A6090[0] = 1.0f;
        D_004A6090[1] = 1.0f;
        D_004A6090[2] = 1.0f;
        D_004A6090[3] = 1.0f;
        D_004A60A0[0] = 1.0f;
        D_004A60A0[1] = 1.0f;
        D_004A60A0[2] = 0.0f;
        D_004A60A0[3] = 0.0f;
        D_004A60B0[0] = 0.700706005f;
        D_004A60B0[1] = 0.0f;
        D_004A60B0[2] = 0.0f;
        D_004A60B0[3] = 0.0f;
        D_004A60C0[0] = 1.0f;
        D_004A60C0[1] = 0.782853007f;
        D_004A60C0[2] = 0.0614060014f;
        D_004A60C0[3] = 0.0414629988f;
        D_004A60D0[0] = 1.0f;
        D_004A60D0[1] = 0.32487601f;
        D_004A60D0[2] = 1.0f;
        D_004A60D0[3] = 0.352800995f;
        D_004A60E0[0] = 1.0f;
        D_004A60E0[1] = 1.0f;
        D_004A60E0[2] = 0.797735989f;
        D_004A60E0[3] = 0.0f;
        D_004A60F0[0] = 1.0f;
        D_004A60F0[1] = 0.493748993f;
        D_004A60F0[2] = 1.0f;
        D_004A60F0[3] = 0.491797f;
        D_004A6100[0] = 1.0f;
        D_004A6100[1] = 1.0f;
        D_004A6100[2] = 1.0f;
        D_004A6100[3] = 1.0f;
        D_004A6110[0] = 1.0f;
        D_004A6110[1] = 0.800000012f;
        D_004A6110[2] = 1.0f;
        D_004A6110[3] = 0.800000012f;
        D_004A6120[0] = 1.0f;
        D_004A6120[1] = 1.0f;
        D_004A6120[2] = 0.0f;
        D_004A6120[3] = 0.0f;
        D_004A6130[0] = 1.0f;
        D_004A6130[1] = 1.0f;
        D_004A6130[2] = 0.0f;
        D_004A6130[3] = 0.0f;
        D_004A6140[0] = 1.0f;
        D_004A6140[1] = 1.0f;
        D_004A6140[2] = 0.0f;
        D_004A6140[3] = 0.0f;
        D_004A6150[0] = 1.0f;
        D_004A6150[1] = 0.327342004f;
        D_004A6150[2] = 1.0f;
        D_004A6150[3] = 0.353904992f;
        D_004A6160[0] = 1.0f;
        D_004A6160[1] = 1.0f;
        D_004A6160[2] = 0.0610000007f;
        D_004A6160[3] = 0.0390000008f;
        D_004A6170[0] = 1.0f;
        D_004A6170[1] = 1.0f;
        D_004A6170[2] = 0.799997985f;
        D_004A6170[3] = 0.0f;
        D_004A6180[0] = 1.0f;
        D_004A6180[1] = 0.958984017f;
        D_004A6180[2] = 0.614378989f;
        D_004A6180[3] = 0.0f;
        D_004A6190[0] = 1.0f;
        D_004A6190[1] = 0.928515017f;
        D_004A6190[2] = 0.339984f;
        D_004A6190[3] = 0.0f;
        D_004A61A0[0] = 1.0f;
        D_004A61A0[1] = 0.873825014f;
        D_004A61A0[2] = 0.249317005f;
        D_004A61A0[3] = 0.0175780002f;
        D_004A61B0[0] = 1.0f;
        D_004A61B0[1] = 0.784373999f;
        D_004A61B0[2] = 0.0613280013f;
        D_004A61B0[3] = 0.0394529998f;
        D_004A61C0[0] = 1.0f;
        D_004A61C0[1] = 0.0f;
        D_004A61C0[2] = 1.0f;
        D_004A61C0[3] = 0.0f;
        D_004A61D0[0] = 1.0f;
        D_004A61D0[1] = 1.0f;
        D_004A61D0[2] = 0.0f;
        D_004A61D0[3] = 0.0f;
        SetCol4(kRiderBoxOffset, 15000.0f, 15000.0f, 15000.0f, 0.0f);
        InitIdx(&D_004A4C08, -1);
        D_004A4C14 = 0;
        D_004A4C10 = 0;
        InitIdx(&D_004A4C18, 0);
        InitIdx(&D_004A4C20, 1);
        InitIdx(&D_004A4C28, 2);
        InitIdx(&D_004A4C30, 3);
        InitIdx(&D_004A4C38, 4);
        InitIdx(&D_004A4C40, 5);
        InitIdx(&D_004A4C48, 6);
        InitIdx(&D_004A4C50, 7);
        InitIdx(&D_004A4C58, 8);
        InitIdx(&D_004A4C60, 9);
        InitIdx(&D_004A4C68, 10);
        InitIdx(&D_004A4C70, 11);
        InitIdx(&D_004A4C78, 12);
        InitIdx(&D_004A4C80, 13);
        kHighSpeedCollideThresh = 2222.22217f;
        InitTerminalVelocityTable();
        nFinishLineControl_kFinishStopThreshCPS = 277.777771f;
        SetKeys8(D_004A61F0, 0.0f, 0.200000003f, 0.400000006f, 0.400000006f, 0.600000024f, 0.400000006f, 1.0f, 0.200000003f);
        SetKeys8(D_004A6210, 0.0f, 0.300000012f, 0.400000006f, 1.20000005f, 0.600000024f, 1.20000005f, 1.0f, 0.300000012f);
        kSpinOutOfAng = 0.785398245f;
        kIntoSpinAbortAng = 0.0872664750f;
        kMaxAdjSpinAng = 1.74532938f;
        kMaxAdjFlipAng = 1.74532938f;
        kMaxPressFlipAng = 0.349065900f;
        kSpinNoDecayThresh = 6.28318596f;
        kFlipNoDecayThresh = 6.28318596f;
        kAirAdjFwdMaxAng = 0.785398245f;
        SetKeys8(D_004A6230, 0.0f, 0.200000003f, 16.0f, 0.200000003f, 80.0f, 1.0f, 120.0f, 1.0f);
        SetKeys8(D_004A6250, 0.0f, 30.4979992f, 20.0f, 30.4979992f, 50.0f, 30.4979992f, 120.0f, 30.4979992f);
        SetKeys8(D_004A6270, 0.0f, 1.0f, 20.0f, 1.0f, 50.0f, 1.0f, 120.0f, 1.0f);
        SetKeys8(D_004A6290, 0.0f, 0.200000003f, 20.0f, 0.699999988f, 50.0f, 0.800000012f, 120.0f, 1.0f);
        SetKeys8(D_004A62B0, 0.0f, 0.681620002f, 20.0f, 0.681620002f, 50.0f, 0.681620002f, 120.0f, 0.681620002f);
        SetKeys8(D_004A62D0, 0.0f, 0.812860012f, 20.0f, 0.812860012f, 50.0f, 0.812860012f, 120.0f, 0.812860012f);
        SetKeys8(D_004A62F0, 0.0f, 1.0f, 20.0f, 1.0f, 50.0f, 1.0f, 120.0f, 1.0f);
    }
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing__cBeing);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern void* cBeing_vtbl[];

/**
 * Destroys a cBeing: restores cBeing's vtable pointer, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cBeing__cBeing(int* self, int flags)
{
    *(void***)self = cBeing_vtbl;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

extern void* t4vect_Zero[];

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_getVelocity__FPv);
#ifdef SKIP_ASM
/**
 * Returns a cBeing's velocity: the base class has none, so this is the zero vector.
 *
 * cBeing's default for the virtual getVelocity; the rider classes override it.
 *
 * @return t4vect_Zero
 */
void* cBeing_getVelocity(void* self)
{
    return (void*)t4vect_Zero;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_isIntersecting__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the being intersects an instance: a plain cBeing never does.
 *
 * cBeing's default for the virtual isIntersecting. On Tour's version takes the tInstanceRecord to test as a second
 * argument; this body ignores it.
 *
 * @return 0
 */
int cBeing_isIntersecting(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_applyBoost__FPv);
#ifdef SKIP_ASM
/**
 * Applies no boost: cBeing's applyBoost has an empty body.
 */
void cBeing_applyBoost(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_getPosition__FPv);
#ifdef SKIP_ASM
/**
 * Returns a cBeing's position: the base class has none, so this is the zero vector.
 *
 * cBeing's default for the virtual getPosition; the rider classes override it.
 *
 * @return t4vect_Zero
 */
void* cBeing_getPosition(void* self)
{
    return (void*)t4vect_Zero;
}
#endif

extern void* t4mat_Identity[];

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_getTransform__FPv);
#ifdef SKIP_ASM
/**
 * Returns a cBeing's transform: the identity matrix for the base class.
 *
 * cBeing's default for the virtual getTransform. Derived classes that do not override it (several rider and effect
 * vtables) reuse this function.
 *
 * @return t4mat_Identity
 */
void* cBeing_getTransform(void* self)
{
    return (void*)t4mat_Identity;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_getAIIndex__FPv);
#ifdef SKIP_ASM
/**
 * Returns the being's AI index: 0 for a plain cBeing.
 *
 * cBeing's default for the virtual getAIIndex; riders return their slot in the race.
 *
 * @return 0
 */
int cBeing_getAIIndex(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_isHuman__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the being is a human-controlled rider: never, for a plain cBeing.
 *
 * @return 0
 */
int cBeing_isHuman(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_isComputer__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the being is a computer-controlled rider: never, for a plain cBeing.
 *
 * @return 0
 */
int cBeing_isComputer(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_transport__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cBeing's transport has an empty body.
 */
void cBeing_transport(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_resetToPath__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cBeing's resetToPath has an empty body.
 */
void cBeing_resetToPath(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_initForMission__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cBeing's initForMission has an empty body.
 */
void cBeing_initForMission(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_flagEventSpeedBoost__FPv);
#ifdef SKIP_ASM
/**
 * Ignores a speed-boost event: cBeing's flagEventSpeedBoost has an empty body.
 */
void cBeing_flagEventSpeedBoost(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_flagEventTrickBoost__FPv);
#ifdef SKIP_ASM
/**
 * Ignores a trick-boost event: cBeing's flagEventTrickBoost has an empty body.
 */
void cBeing_flagEventTrickBoost(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBeing_flagEventTrickMultiplier__FPv);
#ifdef SKIP_ASM
/**
 * Ignores a trick-multiplier event: cBeing's flagEventTrickMultiplier has an empty body.
 */
void cBeing_flagEventTrickMultiplier(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRiderState_cRiderState);
#ifdef SKIP_ASM

/**
 * Constructs the rider's base state: clears the 19 mSlewed channels, sets mGroundPolyId to -1 and fills the 64 words
 * from mVisitedIds (+0x5B8) with -1.
 *
 * Called first by cRider_cRider.
 *
 * @return the rider
 */
extern "C" cRider* cRiderState_cRiderState(cRider* self)
{
    ((int (*)[3])&self->mSlewed[0])[0][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[0][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[0][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[1][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[1][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[1][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[2][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[2][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[2][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[3][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[3][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[3][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[4][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[4][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[4][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[5][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[5][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[5][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[6][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[6][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[6][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[7][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[7][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[7][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[8][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[8][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[8][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[9][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[9][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[9][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[10][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[10][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[10][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[11][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[11][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[11][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[12][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[12][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[12][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[13][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[13][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[13][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[14][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[14][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[14][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[15][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[15][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[15][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[16][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[16][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[16][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[17][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[17][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[17][1] = 0;
    ((int (*)[3])&self->mSlewed[0])[18][2] = 0;
    ((int (*)[3])&self->mSlewed[0])[18][0] = 0;
    ((int (*)[3])&self->mSlewed[0])[18][1] = 0;
    self->mGroundPolyId = 0xFFFFFFFF;
    unsigned int* visited = ((unsigned int*)&self->mVisitedIds);
    for (int i = 63; i != -1; i--, visited++) {
        *visited = 0xFFFFFFFF;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setLocal);
#ifdef SKIP_ASM
/**
 * Sets the rider's mIsLocal flag.
 *
 * @param rider  the cRider
 * @param isLocal  the new value
 */
extern "C" void cRider_setLocal(cRider* rider, int isLocal) {
    rider->mIsLocal = isLocal;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isLocal);
#ifdef SKIP_ASM
/**
 * Returns the rider's mIsLocal flag.
 *
 * @param rider  the cRider
 * @return mIsLocal
 */
extern "C" int cRider_isLocal(cRider* rider) {
    return (rider->mIsLocal);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getVerticalJump);
#ifdef SKIP_ASM
/**
 * Returns the rider's float at +0x5A4 (declared as the int unk_5A4).
 *
 * @param rider  the cRider
 * @return the float at +0x5A4
 */
extern "C" float cRider_getVerticalJump(cRider* rider) {
    return rider->VerticalJump;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_didPop);
#ifdef SKIP_ASM
/**
 * Returns the rider's word at +0x5A8 (unk_5A8).
 *
 * @param rider  the cRider
 * @return the word at +0x5A8
 */
extern "C" int cRider_didPop(cRider* rider) {
    return (rider->DidPop);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isRidingFlexRail);
#ifdef SKIP_ASM
/**
 * Returns the rider's word at +0x5AC (unk_5AC).
 *
 * @param rider  the cRider
 * @return the word at +0x5AC
 */
extern "C" int cRider_isRidingFlexRail(cRider* rider) {
    return (rider->RidingFlexRail);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getAudioBraking_0_to_1);
#ifdef SKIP_ASM
/**
 * Returns the rider's mAudioBraking.
 *
 * @param rider  the cRider
 * @return mAudioBraking
 */
extern "C" float cRider_getAudioBraking_0_to_1(cRider* rider) {
    return (rider->mAudioBraking);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getAudioEdging_0_to_1);
#ifdef SKIP_ASM
/**
 * Returns the rider's mAudioEdging.
 *
 * @param rider  the cRider
 * @return mAudioEdging
 */
extern "C" float cRider_getAudioEdging_0_to_1(cRider* rider) {
    return (rider->mAudioEdging);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getAudioPressure_0_to_1);
#ifdef SKIP_ASM
/**
 * Returns the rider's mAudioPressure.
 *
 * @param rider  the cRider
 * @return mAudioPressure
 */
extern "C" float cRider_getAudioPressure_0_to_1(cRider* rider) {
    return (rider->mAudioPressure);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getAudioSpeed_0_to_1);
#ifdef SKIP_ASM
/**
 * Returns the rider's mAudioSpeed.
 *
 * @param rider  the cRider
 * @return mAudioSpeed
 */
extern "C" float cRider_getAudioSpeed_0_to_1(cRider* rider) {
    return (rider->mAudioSpeed);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getBoundBox);
#ifdef SKIP_ASM
/**
 * Returns the address of the rider's bounding box, mBoundsMin / mBoundsMax (+0x400).
 *
 * @param rider  the cRider
 * @return rider + 0x400
 */
extern "C" int cRider_getBoundBox(int rider) {
    return rider + 0x400;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getSphereTree);
#ifdef SKIP_ASM
/**
 * Returns the rider's mSphereTree.
 *
 * @param rider  the cRider
 * @return mSphereTree
 */
extern "C" int cRider_getSphereTree(cRider* rider) {
    return ((int)rider->mSphereTree);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getFront);
#ifdef SKIP_ASM
/**
 * Copies the rider's front vector (mMatrix row 1, +0x1B0) into @p outFront.
 *
 * @param outFront  receives the vector
 * @param rider  the cRider
 * @return @p outFront
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long long *cRider_getFront(long long *outFront, cRider* rider) {
    *outFront = (*(long long*)&rider->mMatrix.row[1]);
    return outFront;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getSide);
#ifdef SKIP_ASM
/**
 * Copies the rider's side vector (mMatrix row 0, +0x1A0) into @p outSide.
 *
 * @param outSide  receives the vector
 * @param rider  the cRider
 * @return @p outSide
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long long *cRider_getSide(long long *outSide, cRider* rider) {
    *outSide = (*(long long*)&rider->mMatrix);
    return outSide;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getUp);
#ifdef SKIP_ASM
/**
 * Copies the rider's up vector (mMatrix row 2, +0x1C0) into @p outUp.
 *
 * @param outUp  receives the vector
 * @param rider  the cRider
 * @return @p outUp
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long long *cRider_getUp(long long *outUp, cRider* rider) {
    *outUp = (*(long long*)&rider->mMatrix.row[2]);
    return outUp;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getPosition__FPv);
#ifdef SKIP_ASM
/**
 * Returns the address of the rider's mPosition (+0x110).
 *
 * @return self + 0x110
 */
void* cRider_getPosition(void* vself)
{
    cRider* self = (cRider*)vself;
    return (char*)&self->mPosition;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getPositionPointer);
#ifdef SKIP_ASM
/**
 * Returns the address of the rider's mPosition (+0x110), like cRider_getPosition.
 *
 * @param rider  the cRider
 * @return rider + 0x110
 */
extern "C" int cRider_getPositionPointer(int rider) {
    return rider + 272;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getRotation);
#ifdef SKIP_ASM
/**
 * Copies the rider's mRotation quaternion (+0x120) into @p outRotation.
 *
 * @param outRotation  receives the quaternion
 * @param rider  the cRider
 * @return @p outRotation
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long long *cRider_getRotation(long long *outRotation, cRider* rider) {
    *outRotation = (*(long long*)&rider->mRotation);
    return outRotation;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getVelocity__FPv);
#ifdef SKIP_ASM
/**
 * Returns the rider's velocity vector.
 *
 * @return a pointer to mVelocity (+0x1E0)
 */
void* cRider_getVelocity(void* vself)
{
    cRider* self = (cRider*)vself;
    return (char*)&self->mVelocity;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setVelocity);
#ifdef SKIP_ASM
/**
 * Sets the rider's mVelocity (+0x1E0) from @p velocity.
 *
 * @param rider  the cRider
 * @param velocity  the new velocity
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void cRider_setVelocity(cRider* rider, long long *velocity) {
    *(long long*)&rider->mVelocity = (long long) *velocity;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getRoot);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
static inline void hpCopyMat(void* dst, const void* src)
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
        : "r"(dst), "r"(src)
        : "memory");
}

/**
 * Copies the rider's 4x4 matrix at +0x160 (0x40 bytes) into @p out.
 *
 * @param out  receives the matrix
 * @return @p out
 */
extern "C" void* cRider_getRoot(void* out, cRider* self)
{
    hpCopyMat(out, &self->RiderRoot);
    return out;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getGroundNormal);
#ifdef SKIP_ASM
/**
 * Copies the rider's mGroundNormal (+0x370) into @p outNormal.
 *
 * @param outNormal  receives the vector
 * @param rider  the cRider
 * @return @p outNormal
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long long *cRider_getGroundNormal(long long *outNormal, cRider* rider) {
    *outNormal = (*(long long*)&rider->mGroundNormal);
    return outNormal;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getHeading);
#ifdef SKIP_ASM
#include "bx/math3d.h"
extern "C" float func_0031C228(float);

static inline t4Vector getVec_0968(void* self) {
    return *(t4Vector*)((char*)self + 0x1B0);
}

static inline float atan2_0968(float y, float x) {
    float a;

    if (x == 0.0f) {
        if (y == 0.0f) {
            return y;
        }
        if (y >= 0.0f) {
            return 1.5707963705062866f;
        }
        return -1.5707963705062866f;
    }
    a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f) {
            a += 3.1415927410125732f;
        } else {
            a -= 3.1415927410125732f;
        }
    }
    return a;
}

/**
 * Returns the rider's heading: atan2(front.y, front.x) of mMatrix row 1, in radians.
 *
 * @return the angle in -pi..pi
 */
extern "C" float cRider_getHeading(cRider* self) {
    return atan2_0968(getVec_0968(self).y, getVec_0968(self).x);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getAnimModel);
#ifdef SKIP_ASM
/**
 * Returns the rider's mAnimModel.
 *
 * @param rider  the cRider
 * @return mAnimModel
 */
extern "C" int cRider_getAnimModel(cRider* rider) {
    return ((int)rider->mAnimModel);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getRiderAnim);
#ifdef SKIP_ASM
/**
 * Returns the rider's mRiderAnim.
 *
 * @param rider  the cRider
 * @return mRiderAnim
 */
extern "C" int cRider_getRiderAnim(cRider* rider) {
    return ((int)rider->mRiderAnim);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getRiderHandler);
#ifdef SKIP_ASM
/**
 * Returns the rider's mRiderHandler.
 *
 * @param rider  the cRider
 * @return mRiderHandler
 */
extern "C" int cRider_getRiderHandler(cRider* rider) {
    return ((int)rider->mRiderHandler);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getPatchRID);
#ifdef SKIP_ASM
/**
 * Returns the rider's mGroundPolyId (+0x430), the resource id of the ground patch under it.
 *
 * @param rider  the cRider
 * @return mGroundPolyId
 */
extern "C" int cRider_getPatchRID(cRider* rider) {
    return ((int)rider->mGroundPolyId);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getSectionID);
#ifdef SKIP_ASM
/**
 * Returns the rider's world section id (eBESectionID), the word at +0x434 (our header calls it mGroundMaterial).
 *
 * On Tour's cRider::getSectionID; cRider_isOnRaceCourse and cRider_isOnHalfPipeCourse test the same word.
 *
 * @return the section id
 */
extern "C" int cRider_getSectionID(cRider* rider) {
    return (rider->mGroundMaterial);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isOnRaceCourse);
#ifdef SKIP_ASM
/**
 * Returns whether the rider is on a race course: its section id (+0x434, cRider_getSectionID) is below 17.
 *
 * On Tour's cRider::isOnRaceCourse.
 */
extern "C" int cRider_isOnRaceCourse(cRider* rider) {
    return (rider->mGroundMaterial) < 17;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isOnHalfPipeCourse);
#ifdef SKIP_ASM
/**
 * Returns whether the rider is on a half-pipe course: its section id (+0x434, cRider_getSectionID) is 11, 12 or 13.
 *
 * On Tour's cRider::isOnHalfPipeCourse.
 */
extern "C" int cRider_isOnHalfPipeCourse(cRider* rider) {
    return (unsigned int) ((rider->mGroundMaterial) - 11) < 3U;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getPatchUV);
#ifdef SKIP_ASM
/**
 * Returns the address of the rider's ground patch UV pair (+0xAAC, PatchUV).
 *
 * @param rider  the cRider
 * @return rider + 0xAAC
 */
extern "C" int cRider_getPatchUV(int rider) {
    return rider + 2732;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getPathPos);
#ifdef SKIP_ASM
/**
 * Returns the address of the rider's vector at +0x490.
 *
 * @param rider  the cRider
 * @return rider + 0x490
 */
extern "C" int cRider_getPathPos(int rider) {
    return rider + 1168;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getIntersectPos);
#ifdef SKIP_ASM
/**
 * Returns the address of the rider's vector at +0x4A0.
 *
 * @param rider  the cRider
 * @return rider + 0x4A0
 */
extern "C" int cRider_getIntersectPos(int rider) {
    return rider + 1184;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getTrackPath);
#ifdef SKIP_ASM
/**
 * Returns the rider's track path pointer (+0xAB4).
 *
 * @param rider  the cRider
 * @return the word at +0xAB4
 */
extern "C" int cRider_getTrackPath(cRider* rider) {
    return ((int)rider->TrackPath);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setTrackPath);
#ifdef SKIP_ASM
/**
 * Sets the rider's track path pointer (+0xAB4).
 *
 * @param rider  the cRider
 * @param value  the new path
 */
extern "C" void cRider_setTrackPath(cRider* rider, int value) {
    rider->TrackPath = (cTrackPath*)value;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getAIPath);
#ifdef SKIP_ASM
/**
 * Returns the rider's AI path pointer (+0xAB8).
 *
 * @param rider  the cRider
 * @return the word at +0xAB8
 */
extern "C" int cRider_getAIPath(cRider* rider) {
    return ((int)rider->AIPath);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setAIPath);
#ifdef SKIP_ASM
/**
 * Sets the rider's AI path pointer (+0xAB8).
 *
 * @param rider  the cRider
 * @param value  the new path
 */
extern "C" void cRider_setAIPath(cRider* rider, int value) {
    rider->AIPath = (cAIPath*)value;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setTotalPathDistance);
#ifdef SKIP_ASM
/**
 * Sets the rider's total path distance (+0x4D8), the float cRider_getTotalPathDistance returns.
 *
 * @param rider  the cRider
 * @param fparg0  the distance
 */
extern "C" void cRider_setTotalPathDistance(cRider* rider, float fparg0) {
    rider->TotalPathDistance = fparg0;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setPreviousRanking);
#ifdef SKIP_ASM
/**
 * Sets the rider's previous ranking (+0xE8).
 *
 * @param rider  the cRider
 * @param value  the ranking
 */
extern "C" void cRider_setPreviousRanking(cRider* rider, int value) {
    rider->PrevRanking = value;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getPreviousRanking);
#ifdef SKIP_ASM
/**
 * Returns the rider's previous ranking (+0xE8).
 *
 * @param rider  the cRider
 * @return the word at +0xE8
 */
extern "C" int cRider_getPreviousRanking(cRider* rider) {
    return (rider->PrevRanking);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setRaceUnfinished);
#ifdef SKIP_ASM
/**
 * Marks the rider as still racing: mFinishTime = -1.
 *
 * @param rider  the cRider
 */
extern "C" void cRider_setRaceUnfinished(cRider* rider) {
    rider->mFinishTime = -1.0f;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setFinishedRace);
#ifdef SKIP_ASM
/**
 * Marks the rider as past the finish line: a negative mFinishTime becomes 0 (counting starts).
 *
 * @param rider  the cRider
 */
extern "C" void cRider_setFinishedRace(cRider* rider) {
    if ((rider->mFinishTime) < 0.0f) {
        rider->mFinishTime = 0.0f;
    }
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isPastFinishLine);
#ifdef SKIP_ASM
/**
 * Returns whether the rider has crossed the finish line (mFinishTime >= 0).
 *
 * @param rider  the cRider
 * @return 1 when mFinishTime >= 0, else 0
 */
extern "C" int cRider_isPastFinishLine(cRider* rider) {
    int var_2;

    var_2 = 1;
    if (!((rider->mFinishTime) >= 0.0f)) {
        var_2 = 0;
    }
    return var_2;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isFinishedRace);
#ifdef SKIP_ASM
/**
 * Returns whether the rider finished at least 10 seconds ago (mFinishTime >= 10).
 *
 * @param rider  the cRider
 * @return 1 when mFinishTime >= 10, else 0
 */
extern "C" int cRider_isFinishedRace(cRider* rider) {
    int var_2;

    var_2 = 1;
    if (!((rider->mFinishTime) >= 10.0f)) {
        var_2 = 0;
    }
    return var_2;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setStartTick);
#ifdef SKIP_ASM
/**
 * Sets the rider's mStartTick.
 *
 * @param rider  the cRider
 * @param startTick  the tick
 */
extern "C" void cRider_setStartTick(cRider* rider, int startTick) {
    rider->mStartTick = startTick;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getStartTick);
#ifdef SKIP_ASM
/**
 * Returns the rider's mStartTick.
 *
 * @param rider  the cRider
 * @return mStartTick
 */
extern "C" int cRider_getStartTick(cRider* rider) {
    return (rider->mStartTick);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setFinishTick);
#ifdef SKIP_ASM
/**
 * Sets the rider's mFinishTick.
 *
 * @param rider  the cRider
 * @param finishTick  the tick
 */
extern "C" void cRider_setFinishTick(cRider* rider, int finishTick) {
    rider->mFinishTick = finishTick;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getFinishTick);
#ifdef SKIP_ASM
/**
 * Returns the rider's mFinishTick.
 *
 * @param rider  the cRider
 * @return mFinishTick
 */
extern "C" int cRider_getFinishTick(cRider* rider) {
    return (rider->mFinishTick);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setPenaltyTicks);
#ifdef SKIP_ASM
/**
 * Sets the rider's mPenaltyTicks.
 *
 * @param rider  the cRider
 * @param penaltyTicks  the penalty
 */
extern "C" void cRider_setPenaltyTicks(cRider* rider, int penaltyTicks) {
    rider->mPenaltyTicks = penaltyTicks;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getPenaltyTicks);
#ifdef SKIP_ASM
/**
 * Returns the rider's mPenaltyTicks.
 *
 * @param rider  the cRider
 * @return mPenaltyTicks
 */
extern "C" int cRider_getPenaltyTicks(cRider* rider) {
    return (rider->mPenaltyTicks);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setDidNotFinish);
#ifdef SKIP_ASM
/**
 * Sets the rider's mDidNotFinish flag.
 *
 * @param rider  the cRider
 * @param didNotFinish  the new value
 */
extern "C" void cRider_setDidNotFinish(cRider* rider, int didNotFinish) {
    rider->mDidNotFinish = didNotFinish;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getDidNotFinish);
#ifdef SKIP_ASM
/**
 * Returns the rider's mDidNotFinish flag.
 *
 * @param rider  the cRider
 * @return mDidNotFinish
 */
extern "C" int cRider_getDidNotFinish(cRider* rider) {
    return (rider->mDidNotFinish);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getAIIndex__FPv);
#ifdef SKIP_ASM
/**
 * Returns the rider's AI index, which is also its character and stat lookup index.
 *
 * @return mAIIndex (cRider offset 0x86C)
 */
int cRider_getAIIndex(void* vself)
{
    cRider* self = (cRider*)vself;
    return self->mAIIndex;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setAIIndex);
#ifdef SKIP_ASM
/**
 * Sets the rider's mAIIndex.
 *
 * @param rider  the cRider
 * @param aiIndex  the index
 */
extern "C" void cRider_setAIIndex(cRider* rider, int aiIndex) {
    rider->mAIIndex = aiIndex;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getViewIndex);
#ifdef SKIP_ASM
/**
 * Returns the rider's mViewIndex.
 *
 * @param rider  the cRider
 * @return mViewIndex
 */
extern "C" int cRider_getViewIndex(cRider* rider) {
    return (rider->mViewIndex);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setViewIndex);
#ifdef SKIP_ASM
/**
 * Sets the rider's mViewIndex.
 *
 * @param rider  the cRider
 * @param viewIndex  the index
 */
extern "C" void cRider_setViewIndex(cRider* rider, int viewIndex) {
    rider->mViewIndex = viewIndex;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setHuman);
#ifdef SKIP_ASM
/**
 * Marks the rider as human: mIsHuman = 1.
 *
 * @param rider  the cRider
 */
extern "C" void cRider_setHuman(cRider* rider) {
    rider->mIsHuman = 1;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setActor);
#ifdef SKIP_ASM
/**
 * Marks the rider as an actor: mIsActor = 1.
 *
 * @param rider  the cRider
 */
extern "C" void cRider_setActor(cRider* rider) {
    rider->mIsActor = 1;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isHuman__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the rider is controlled by a human player.
 *
 * @return mIsHuman (cRider offset 0x874): nonzero for a human rider, 0 for a computer one
 */
int cRider_isHuman(void* vself)
{
    cRider* self = (cRider*)vself;
    return self->mIsHuman;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isComputer);
#ifdef SKIP_ASM
/**
 * Whether this rider is driven by the computer rather than a player.
 *
 * @return 1 for an AI rider, 0 for a human one (the inverse of mIsHuman)
 */
extern "C" int cRider_isComputer(cRider* self)
{
    return self->mIsHuman ^ 1;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isActor);
#ifdef SKIP_ASM
/**
 * Returns the rider's mIsActor flag.
 *
 * @param rider  the cRider
 * @return mIsActor
 */
extern "C" int cRider_isActor(cRider* rider) {
    return (rider->mIsActor);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isLocalHuman);
#ifdef SKIP_ASM
/**
 * Returns whether the rider is a local human: mIsHuman and mIsLocal both set.
 *
 * @param rider  the cRider
 * @return 1 for a local human, else 0
 */
extern "C" int cRider_isLocalHuman(cRider* rider) {
    int var_3;

    var_3 = 0;
    if ((rider->mIsHuman) != 0) {
        var_3 = (rider->mIsLocal) != 0;
    }
    return var_3;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getRenderFlag);
#ifdef SKIP_ASM
/**
 * Returns the rider's mRenderFlag.
 *
 * @param rider  the cRider
 * @return mRenderFlag
 */
extern "C" int cRider_getRenderFlag(cRider* rider) {
    return (rider->mRenderFlag);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setRenderFlag);
#ifdef SKIP_ASM
/**
 * Sets the rider's mRenderFlag and resets mSafeRenderTick to -1.
 *
 * @param rider  the cRider
 * @param renderFlag  the new value
 */
extern "C" void cRider_setRenderFlag(cRider* rider, int renderFlag) {
    rider->mRenderFlag = renderFlag;
    rider->mSafeRenderTick = -1;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getSafeRenderTick);
#ifdef SKIP_ASM
/**
 * Returns the rider's mSafeRenderTick.
 *
 * @param rider  the cRider
 * @return mSafeRenderTick
 */
extern "C" int cRider_getSafeRenderTick(cRider* rider) {
    return (rider->mSafeRenderTick);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setSafeRenderTick);
#ifdef SKIP_ASM
/**
 * Sets the rider's mSafeRenderTick.
 *
 * @param rider  the cRider
 * @param safeRenderTick  the tick
 */
extern "C" void cRider_setSafeRenderTick(cRider* rider, int safeRenderTick) {
    rider->mSafeRenderTick = safeRenderTick;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isInTunnel);
#ifdef SKIP_ASM
/**
 * Returns whether the rider is in a tunnel (InTunnel, which cRider_updateSplit1 clears each frame).
 *
 * @param rider  the cRider
 * @return InTunnel
 */
extern "C" int cRider_isInTunnel(cRider* rider) {
    return (rider->InTunnel);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setInTunnel);
#ifdef SKIP_ASM
/**
 * Sets whether the rider is in a tunnel (InTunnel).
 *
 * @param rider  the cRider
 * @param value  nonzero when in a tunnel
 */
extern "C" void cRider_setInTunnel(cRider* rider, int value) {
    rider->InTunnel = value;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getMetrix);
#ifdef SKIP_ASM
/**
 * Returns the rider's mMetrix.
 *
 * @param rider  the cRider
 * @return mMetrix
 */
extern "C" int cRider_getMetrix(cRider* rider) {
    return ((int)rider->mMetrix);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getAirPredictor);
#ifdef SKIP_ASM
/**
 * Returns the rider's mAirPredictor.
 *
 * @param rider  the cRider
 * @return mAirPredictor
 */
extern "C" int cRider_getAirPredictor(cRider* rider) {
    return ((int)rider->mAirPredictor);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_GetWipeoutSlideHandle);
#ifdef SKIP_ASM
/**
 * Returns the rider's mWipeoutSlideHandle.
 *
 * @param rider  the cRider
 * @return mWipeoutSlideHandle
 */
extern "C" int cRider_GetWipeoutSlideHandle(cRider* rider) {
    return (rider->mWipeoutSlideHandle);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_SetWipeoutSlideHandle);
#ifdef SKIP_ASM
/**
 * Sets the rider's mWipeoutSlideHandle.
 *
 * @param rider  the cRider
 * @param slideHandle  the sound handle
 */
extern "C" void cRider_SetWipeoutSlideHandle(cRider* rider, int slideHandle) {
    rider->mWipeoutSlideHandle = slideHandle;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_GetWipeoutTumbleHandle);
#ifdef SKIP_ASM
/**
 * Returns the rider's mWipeoutTumbleHandle.
 *
 * @param rider  the cRider
 * @return mWipeoutTumbleHandle
 */
extern "C" int cRider_GetWipeoutTumbleHandle(cRider* rider) {
    return (rider->mWipeoutTumbleHandle);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_SetWipeoutTumbleHandle);
#ifdef SKIP_ASM
/**
 * Sets the rider's mWipeoutTumbleHandle.
 *
 * @param rider  the cRider
 * @param tumbleHandle  the sound handle
 */
extern "C" void cRider_SetWipeoutTumbleHandle(cRider* rider, int tumbleHandle) {
    rider->mWipeoutTumbleHandle = tumbleHandle;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_vibrationReset__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the cRider base version of virtual slot 16 (kRiderVt_vibrationReset).
 */
void cRider_vibrationReset(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_vibrationBump__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the cRider base version of virtual slot 17 (kRiderVt_vibrationBump).
 */
void cRider_vibrationBump(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_vibrationRumble__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the cRider base version of virtual slot 18 (kRiderVt_vibrationRumble).
 */
void cRider_vibrationRumble(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_vibrationUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the cRider base version of virtual slot 19 (kRiderVt_vibrationUpdate).
 */
void cRider_vibrationUpdate(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_GetRiderLOD);
#ifdef SKIP_ASM
/**
 * Returns the rider's mLOD (0-2).
 *
 * @param rider  the cRider
 * @return mLOD
 */
extern "C" int cRider_GetRiderLOD(cRider* rider) {
    return (rider->mLOD);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getFocusBox);
#ifdef SKIP_ASM
/**
 * Returns the rider's mFocusBox.
 *
 * @param rider  the cRider
 * @return mFocusBox
 */
extern "C" int cRider_getFocusBox(cRider* rider) {
    return ((int)rider->mFocusBox);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getShadow);
#ifdef SKIP_ASM
/**
 * Returns the rider's mShadow.
 *
 * @param rider  the cRider
 * @return mShadow
 */
extern "C" int cRider_getShadow(cRider* rider) {
    return ((int)rider->mShadow);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00140CA0);
#ifdef SKIP_ASM
/**
 * Returns the rider's mSmoothedHeading (+0x5B4).
 *
 * @param rider  the cRider
 * @return mSmoothedHeading
 */
extern "C" float func_00140CA0(cRider* rider) {
    return (rider->mSmoothedHeading);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_DoForceBoardVisualFXForNIS);
#ifdef SKIP_ASM
/**
 * Returns whether the rider is in an NIS with unk_AD0 clear and unk_AFC set.
 *
 * @param rider  the cRider
 * @return 1 when mIsInNIS and unk_AFC are set and unk_AD0 is 0, else 0
 */
extern "C" int cRider_DoForceBoardVisualFXForNIS(cRider* rider) {
    int var_3;

    var_3 = 0;
    if (((rider->mIsInNIS) != 0) && ((rider->mHiddenNIS) == 0)) {
        var_3 = (rider->mNISShowBoardVisualFX) != 0;
    }
    return var_3;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_IsOnGroundInNIS);
#ifdef SKIP_ASM
/**
 * Returns the rider's word at +0xB00 (unk_B00), which cRider_SetNISGroundVectors sets to 1.
 *
 * @param rider  the cRider
 * @return unk_B00
 */
extern "C" int cRider_IsOnGroundInNIS(cRider* rider) {
    return (rider->mOnGroundNIS);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_IsInNIS);
#ifdef SKIP_ASM
/**
 * Returns the rider's mIsInNIS flag.
 *
 * @param rider  the cRider
 * @return mIsInNIS
 */
extern "C" int cRider_IsInNIS(cRider* rider) {
    return (rider->mIsInNIS);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_GetAttachedRider__FPv);
#ifdef SKIP_ASM
/**
 * Returns the rider itself.
 *
 * @return self
 */
void* cRider_GetAttachedRider(void* self)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_IsRiderOnScreen);
#ifdef SKIP_ASM
/**
 * Returns the rider's mIsOnScreen.
 *
 * @param rider  the cRider
 * @return mIsOnScreen
 */
extern "C" int cRider_IsRiderOnScreen(cRider* rider) {
    return (rider->mIsOnScreen);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setBoostMode);
#ifdef SKIP_ASM
extern "C" void cRider_initBoost();

/**
 * Sets the rider's mBoostMode and re-initialises the boost (cRider_initBoost, which gets the rider in a0).
 *
 * @param rider  the cRider
 * @param boostMode  the mode, 0-3
 */
extern "C" void cRider_setBoostMode(cRider* rider, int boostMode) {
    rider->mBoostMode = boostMode;
    cRider_initBoost();
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setWipeoutRecoveryMode);
#ifdef SKIP_ASM
/**
 * Sets the rider's word at +0xB20 (unk_B20, 1 after construction).
 *
 * @param rider  the cRider
 * @param value  the new value
 */
extern "C" void cRider_setWipeoutRecoveryMode(cRider* rider, int value) {
    rider->mWipeoutRecoveryMode = value;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setInitialBoost);
#ifdef SKIP_ASM
/**
 * Sets the rider's mBoostMeterStart (+0xB24).
 *
 * @param rider  the cRider
 * @param fparg0  the starting boost
 */
extern "C" void cRider_setInitialBoost(cRider* rider, float fparg0) {
    rider->mBoostMeterStart = fparg0;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setBoostPowerSource);
#ifdef SKIP_ASM
/**
 * Sets the rider's mBoostSourceMask (+0xB28).
 *
 * @param rider  the cRider
 * @param sourceMask  the boost source flags
 */
extern "C" void cRider_setBoostPowerSource(cRider* rider, int sourceMask) {
    rider->mBoostSourceMask = sourceMask;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setLaunchDelaySecs);
#ifdef SKIP_ASM
/**
 * Sets the rider's mStartHandicap (+0xB30).
 *
 * @param rider  the cRider
 * @param handicap  the new value
 */
extern "C" void cRider_setLaunchDelaySecs(cRider* rider, int handicap) {
    rider->mStartHandicap = handicap;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setForcedAttribLevel);
#ifdef SKIP_ASM
/**
 * Sets the rider's mStatsIndex (+0xB34).
 *
 * @param rider  the cRider
 * @param statsIndex  the stat column
 */
extern "C" void cRider_setForcedAttribLevel(cRider* rider, int statsIndex) {
    rider->mStatsIndex = statsIndex;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_setNaturalAirMode);
#ifdef SKIP_ASM
/**
 * Sets the rider's natural-air mode, the word at +0x308 (1 after construction) that cRider_getNaturalAirMode returns.
 *
 * On Tour's cRider::setNaturalAirMode.
 */
extern "C" void cRider_setNaturalAirMode(cRider* rider, int value) {
    rider->mAllowNaturalAir = value;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getBoostMode);
#ifdef SKIP_ASM
/**
 * Returns the rider's mBoostMode.
 *
 * @param rider  the cRider
 * @return mBoostMode
 */
extern "C" int cRider_getBoostMode(cRider* rider) {
    return (rider->mBoostMode);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getWipeoutRecoveryMode);
#ifdef SKIP_ASM
/**
 * Returns the rider's word at +0xB20 (unk_B20).
 *
 * @param rider  the cRider
 * @return unk_B20
 */
extern "C" int cRider_getWipeoutRecoveryMode(cRider* rider) {
    return (rider->mWipeoutRecoveryMode);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isUberGrabAllowed);
#ifdef SKIP_ASM
/**
 * Returns bit 0 of the rider's mUberFlags.
 *
 * @param rider  the cRider
 * @return mUberFlags & 1
 */
extern "C" int cRider_isUberGrabAllowed(cRider* rider) {
    return (rider->mUberFlags) & 1;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_isUberRailAllowed);
#ifdef SKIP_ASM
/**
 * Returns bit 1 of the rider's mUberFlags.
 *
 * @param rider  the cRider
 * @return (mUberFlags >> 1) & 1
 */
extern "C" int cRider_isUberRailAllowed(cRider* rider) {
    return ((int) (rider->mUberFlags) >> 1) & 1;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getLaunchDelaySecs);
#ifdef SKIP_ASM
/**
 * Returns the rider's mStartHandicap (+0xB30).
 *
 * @param rider  the cRider
 * @return mStartHandicap
 */
extern "C" int cRider_getLaunchDelaySecs(cRider* rider) {
    return (rider->mStartHandicap);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRider_getNaturalAirMode);
#ifdef SKIP_ASM
/**
 * Returns the rider's natural-air mode, the word at +0x308 that cRider_setNaturalAirMode stores.
 *
 * On Tour's cRider::getNaturalAirMode.
 */
extern "C" int cRider_getNaturalAirMode(cRider* rider) {
    return (rider->mAllowNaturalAir);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGroundMotion__cGroundMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cGroundMotion part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cGroundMotion's (D_00459A98 at +0x6E8, D_00459AE0 at
 * +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor00140D80::~cVbDtor00140D80()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00140D80; expose it under the address-derived name.
__asm__(".globl cGroundMotion__cGroundMotion\n.type cGroundMotion__cGroundMotion,@function\ncGroundMotion__cGroundMotion = _$_15cVbDtor00140D80\n.size cGroundMotion__cGroundMotion,0x15C\n");

// PORT: cRiderVb_hpc's dtor is cRider's (cRider__cRider).
__asm__("_$_12cRiderVb_hpc = cRider__cRider\n");

// PORT: cRiderVbB0_hpc's vtable is not emitted here (key function b01 elsewhere): retail D_00459B90.
__asm__("_vt$14cRiderVbB0_hpc = D_00459B90\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00140D80$13cRiderVbC_hpc = D_00459A98\n"
        "_vt$15cVbDtor00140D80$12cRiderVb_hpc = D_00459AE0\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAirMotion__cAirMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cAirMotion part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cAirMotion's (D_004599A0 at +0x6E8, D_004599E8 at
 * +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor00140EE0::~cVbDtor00140EE0()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00140EE0; expose it under the address-derived name.
__asm__(".globl cAirMotion__cAirMotion\n.type cAirMotion__cAirMotion,@function\ncAirMotion__cAirMotion = _$_15cVbDtor00140EE0\n.size cAirMotion__cAirMotion,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00140EE0$13cRiderVbC_hpc = D_004599A0\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00140EE0$12cRiderVb_hpc = D_004599E8\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideMotion__cRailslideMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cRailslideMotion part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cRailslideMotion's (D_004598A8 at +0x6E8, D_004598F0
 * at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor00141040::~cVbDtor00141040()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00141040; expose it under the address-derived name.
__asm__(".globl cRailslideMotion__cRailslideMotion\n.type cRailslideMotion__cRailslideMotion,@function\ncRailslideMotion__cRailslideMotion = _$_15cVbDtor00141040\n.size cRailslideMotion__cRailslideMotion,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00141040$13cRiderVbC_hpc = D_004598A8\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00141040$12cRiderVb_hpc = D_004598F0\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cWipeoutMotion_getBoardSphereTree);
#ifdef SKIP_ASM
/**
 * Returns the wipeout motion's BoardSphereTree.
 *
 * @param wipeoutMotion  the cWipeoutMotion
 * @return BoardSphereTree
 */
extern "C" int cWipeoutMotion_getBoardSphereTree(cWipeoutMotion* wipeoutMotion) {
    return ((int)wipeoutMotion->BoardSphereTree);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cWipeoutMotion_getBoardBoundBox);
#ifdef SKIP_ASM
/**
 * Returns the address of the wipeout motion's BoardIntersectBox (+0x50).
 *
 * @param wipeoutMotion  the cWipeoutMotion
 * @return wipeoutMotion + 0x50
 */
extern "C" int cWipeoutMotion_getBoardBoundBox(int wipeoutMotion) {
    return wipeoutMotion + 80;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cWipeoutMotion_getWipeoutMotionState);
#ifdef SKIP_ASM
/**
 * Returns the wipeout motion's sub-state, WipeoutMotionState (its first word).
 *
 * @return the eWMotionState value: WMS_Ground (0) or WMS_Air (1)
 */
extern "C" int cWipeoutMotion_getWipeoutMotionState(int *self) {
    return *self;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cWipeoutMotion_getRiderAngVel);
#ifdef SKIP_ASM
/**
 * Returns the address of the wipeout motion's RiderAngVelocity vector (self + 0x30).
 *
 * @return self + 0x30, as an int (On Tour returns a const t4Vector&)
 */
extern "C" int cWipeoutMotion_getRiderAngVel(int self) {
    return self + 0x30;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cStaticMotion__cStaticMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cStaticMotion part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cStaticMotion's (D_004596B8 at +0x6E8, D_00459700 at
 * +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cHandPlant11C0::~cHandPlant11C0()
{
}

// PORT: the compiler names this dtor _$_14cHandPlant11C0; expose it under the address-derived name.
__asm__(".globl cStaticMotion__cStaticMotion\n.type cStaticMotion__cStaticMotion,@function\ncStaticMotion__cStaticMotion = _$_14cHandPlant11C0\n.size cStaticMotion__cStaticMotion,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$14cHandPlant11C0$13cRiderVbC_hpc = D_004596B8\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$14cHandPlant11C0$12cRiderVb_hpc = D_00459700\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion__cHandplantMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cHandplantMotion part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cHandplantMotion's (D_004595C0 at +0x6E8, D_00459608
 * at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor00141320::~cVbDtor00141320()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00141320; expose it under the address-derived name.
__asm__(".globl cHandplantMotion__cHandplantMotion\n.type cHandplantMotion__cHandplantMotion,@function\ncHandplantMotion__cHandplantMotion = _$_15cVbDtor00141320\n.size cHandplantMotion__cHandplantMotion,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00141320$13cRiderVbC_hpc = D_004595C0\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00141320$12cRiderVb_hpc = D_00459608\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantMotion_cHandplantMotion);
#ifdef SKIP_ASM
extern "C" void cRider_cRider(void* self);

struct sVEntry_00141480 { short delta; short index; void* fn; };
struct sVtblA_00141480 { sVEntry_00141480 e[9]; } __attribute__((aligned(8)));
struct sVtblB_00141480 { sVEntry_00141480 e[22]; } __attribute__((aligned(8)));

extern const sVtblA_00141480 D_004595C0_00141480 __asm__("D_004595C0");
extern char D_00459B90[];
extern const sVtblB_00141480 D_00459608_00141480 __asm__("D_00459608");

/**
 * Constructs the handplant-motion part: sets up its virtual cRider base.
 *
 * When in charge it points mRider at self + 0xB0 and constructs the cRider there. It then stores
 * cHandplantMotion's vtables in the cRider (D_004595C0 at +0x6E8, D_00459B90 at +0x6D0, D_00459608 at
 * +0x6C0), using stack copies with adjusted this-deltas when not in charge (g++ 2.95 virtual-base code). No
 * state member is initialised here.
 *
 * @param inChrg  g++ in-charge flag: nonzero when this is the complete object, so the cRider base is built
 *                here
 * @return self
 */
// PORT: g++ 2.95 virtual-base construction. The class derives virtually from cRider (vbase pointer at
// +0xA0, vbase at +0xB0); when not most-derived, g++ copies cRider's overridden vtables to the stack
// and fixes up the this-deltas (expand_upcast_fixups). Written out by hand here.
extern "C" void* cHandplantMotion_cHandplantMotion(cHandplantMotion* self, int inChrg)
{
    if (inChrg)
    {
        char* vb = (char*)self + 0xB0;
        self->mRider = (cRider*)vb;
        cRider_cRider(vb);
    }
    self->mRider->mScriptObjectVtbl = (void*)&D_004595C0_00141480;
    self->mRider->mScriptObjRiderVtbl = D_00459B90;
    self->mRider->mVtbl = (void*)&D_00459608_00141480;
    if (!inChrg)
    {
        sVtblA_00141480 scriptObjVtbl = D_004595C0_00141480;
        self->mRider->mScriptObjectVtbl = &scriptObjVtbl;
        char* base = (char*)self->mRider - 0xB0;
        int baseDelta = (char*)self - base;
        scriptObjVtbl.e[1].delta = D_004595C0_00141480.e[1].delta + baseDelta;
        sVtblB_00141480 riderVtbl = D_00459608_00141480;
        self->mRider->mVtbl = &riderVtbl;
        riderVtbl.e[1].delta = D_00459608_00141480.e[1].delta + baseDelta;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cCruiseControl__cCruiseControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cCruiseControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cCruiseControl's (D_004594C8 at +0x6E8, D_00459510
 * at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor001415C8::~cVbDtor001415C8()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor001415C8; expose it under the address-derived name.
__asm__(".globl cCruiseControl__cCruiseControl\n.type cCruiseControl__cCruiseControl,@function\ncCruiseControl__cCruiseControl = _$_15cVbDtor001415C8\n.size cCruiseControl__cCruiseControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor001415C8$13cRiderVbC_hpc = D_004594C8\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor001415C8$12cRiderVb_hpc = D_00459510\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBoardPressControl__cBoardPressControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cBoardPressControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cBoardPressControl's (D_004593D0 at +0x6E8,
 * D_00459418 at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the
 * flags are 0), destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is
 * set, and frees the object when bit 0 is set.
 */
cVbDtor00141728::~cVbDtor00141728()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00141728; expose it under the address-derived name.
__asm__(".globl cBoardPressControl__cBoardPressControl\n.type cBoardPressControl__cBoardPressControl,@function\ncBoardPressControl__cBoardPressControl = _$_15cVbDtor00141728\n.size cBoardPressControl__cBoardPressControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00141728$13cRiderVbC_hpc = D_004593D0\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00141728$12cRiderVb_hpc = D_00459418\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cPrewindControl__cPrewindControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cPrewindControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cPrewindControl's (D_004592D8 at +0x6E8, D_00459320
 * at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor00141888::~cVbDtor00141888()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00141888; expose it under the address-derived name.
__asm__(".globl cPrewindControl__cPrewindControl\n.type cPrewindControl__cPrewindControl,@function\ncPrewindControl__cPrewindControl = _$_15cVbDtor00141888\n.size cPrewindControl__cPrewindControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00141888$13cRiderVbC_hpc = D_004592D8\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00141888$12cRiderVb_hpc = D_00459320\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cBumpControl__cBumpControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cBumpControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cBumpControl's (D_004591E0 at +0x6E8, D_00459228 at
 * +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor001419E8::~cVbDtor001419E8()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor001419E8; expose it under the address-derived name.
__asm__(".globl cBumpControl__cBumpControl\n.type cBumpControl__cBumpControl,@function\ncBumpControl__cBumpControl = _$_15cVbDtor001419E8\n.size cBumpControl__cBumpControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor001419E8$13cRiderVbC_hpc = D_004591E0\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor001419E8$12cRiderVb_hpc = D_00459228\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cNaturalAirControl__cNaturalAirControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cNaturalAirControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cNaturalAirControl's (D_004590E8 at +0x6E8,
 * D_00459130 at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the
 * flags are 0), destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is
 * set, and frees the object when bit 0 is set.
 */
cVbDtor00141B48::~cVbDtor00141B48()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00141B48; expose it under the address-derived name.
__asm__(".globl cNaturalAirControl__cNaturalAirControl\n.type cNaturalAirControl__cNaturalAirControl,@function\ncNaturalAirControl__cNaturalAirControl = _$_15cVbDtor00141B48\n.size cNaturalAirControl__cNaturalAirControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00141B48$13cRiderVbC_hpc = D_004590E8\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00141B48$12cRiderVb_hpc = D_00459130\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cSpinControl__cSpinControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cSpinControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cSpinControl's (D_00458FF0 at +0x6E8, D_00459038 at
 * +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor00141CA8::~cVbDtor00141CA8()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00141CA8; expose it under the address-derived name.
__asm__(".globl cSpinControl__cSpinControl\n.type cSpinControl__cSpinControl,@function\ncSpinControl__cSpinControl = _$_15cVbDtor00141CA8\n.size cSpinControl__cSpinControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00141CA8$13cRiderVbC_hpc = D_00458FF0\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00141CA8$12cRiderVb_hpc = D_00459038\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cGateControl__cGateControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cGateControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cGateControl's (D_00458EF8 at +0x6E8, D_00458F40 at
 * +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor00141E08::~cVbDtor00141E08()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00141E08; expose it under the address-derived name.
__asm__(".globl cGateControl__cGateControl\n.type cGateControl__cGateControl,@function\ncGateControl__cGateControl = _$_15cVbDtor00141E08\n.size cGateControl__cGateControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00141E08$13cRiderVbC_hpc = D_00458EF8\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00141E08$12cRiderVb_hpc = D_00458F40\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRailslideControl__cRailslideControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cRailslideControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cRailslideControl's (D_00458E00 at +0x6E8,
 * D_00458E48 at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the
 * flags are 0), destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is
 * set, and frees the object when bit 0 is set.
 */
cVbDtor00141F68::~cVbDtor00141F68()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00141F68; expose it under the address-derived name.
__asm__(".globl cRailslideControl__cRailslideControl\n.type cRailslideControl__cRailslideControl,@function\ncRailslideControl__cRailslideControl = _$_15cVbDtor00141F68\n.size cRailslideControl__cRailslideControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00141F68$13cRiderVbC_hpc = D_00458E00\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00141F68$12cRiderVb_hpc = D_00458E48\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cUberGrindControl__cUberGrindControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cUberGrindControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cUberGrindControl's (D_00458D08 at +0x6E8,
 * D_00458D50 at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the
 * flags are 0), destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is
 * set, and frees the object when bit 0 is set.
 */
cVbDtor001420C8::~cVbDtor001420C8()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor001420C8; expose it under the address-derived name.
__asm__(".globl cUberGrindControl__cUberGrindControl\n.type cUberGrindControl__cUberGrindControl,@function\ncUberGrindControl__cUberGrindControl = _$_15cVbDtor001420C8\n.size cUberGrindControl__cUberGrindControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor001420C8$13cRiderVbC_hpc = D_00458D08\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor001420C8$12cRiderVb_hpc = D_00458D50\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cWipeoutControl__cWipeoutControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cWipeoutControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cWipeoutControl's (D_00458C10 at +0x6E8, D_00458C58
 * at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor00142228::~cVbDtor00142228()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00142228; expose it under the address-derived name.
__asm__(".globl cWipeoutControl__cWipeoutControl\n.type cWipeoutControl__cWipeoutControl,@function\ncWipeoutControl__cWipeoutControl = _$_15cVbDtor00142228\n.size cWipeoutControl__cWipeoutControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00142228$13cRiderVbC_hpc = D_00458C10\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00142228$12cRiderVb_hpc = D_00458C58\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cWipeoutControl_cWipeoutControl);
#ifdef SKIP_ASM
extern "C" void cRider_cRider(void* self);

struct sVEntry_00142388 { short delta; short index; void* fn; };
struct sVtblA_00142388 { sVEntry_00142388 e[9]; } __attribute__((aligned(8)));
struct sVtblB_00142388 { sVEntry_00142388 e[22]; } __attribute__((aligned(8)));

extern const sVtblA_00142388 D_00458C10_00142388 __asm__("D_00458C10");
extern char D_00459B90[];
extern const sVtblB_00142388 D_00458C58_00142388 __asm__("D_00458C58");

/**
 * Constructs the wipeout-control part: sets up its virtual cRider base.
 *
 * When in charge it points mRider at self + 0x90 and constructs the cRider there. It then stores
 * cWipeoutControl's vtables in the cRider (D_00458C10 at +0x6E8, D_00459B90 at +0x6D0, D_00458C58 at +0x6C0),
 * using stack copies with adjusted this-deltas when not in charge (g++ 2.95 virtual-base code). No state
 * member is initialised here.
 *
 * @param inChrg  g++ in-charge flag: nonzero when this is the complete object, so the cRider base is built
 *                here
 * @return self
 */
// PORT: g++ 2.95 virtual-base construction. The class derives virtually from cRider (vbase pointer at
// +0x80, vbase at +0x90); when not most-derived, g++ copies cRider's overridden vtables to the stack
// and fixes up the this-deltas (expand_upcast_fixups). Written out by hand here.
extern "C" void* cWipeoutControl_cWipeoutControl(cWipeoutControl* self, int inChrg)
{
    if (inChrg)
    {
        char* vb = (char*)self + 0x90;
        self->mRider = (cRider*)vb;
        cRider_cRider(vb);
    }
    self->mRider->mScriptObjectVtbl = (void*)&D_00458C10_00142388;
    self->mRider->mScriptObjRiderVtbl = D_00459B90;
    self->mRider->mVtbl = (void*)&D_00458C58_00142388;
    if (!inChrg)
    {
        sVtblA_00142388 scriptObjVtbl = D_00458C10_00142388;
        self->mRider->mScriptObjectVtbl = &scriptObjVtbl;
        char* base = (char*)self->mRider - 0x90;
        int baseDelta = (char*)self - base;
        scriptObjVtbl.e[1].delta = D_00458C10_00142388.e[1].delta + baseDelta;
        sVtblB_00142388 riderVtbl = D_00458C58_00142388;
        self->mRider->mVtbl = &riderVtbl;
        riderVtbl.e[1].delta = D_00458C58_00142388.e[1].delta + baseDelta;
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cFinishLineControl__cFinishLineControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cFinishLineControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cFinishLineControl's (D_00458B18 at +0x6E8,
 * D_00458B60 at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the
 * flags are 0), destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is
 * set, and frees the object when bit 0 is set.
 */
cVbDtor001424D0::~cVbDtor001424D0()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor001424D0; expose it under the address-derived name.
__asm__(".globl cFinishLineControl__cFinishLineControl\n.type cFinishLineControl__cFinishLineControl,@function\ncFinishLineControl__cFinishLineControl = _$_15cVbDtor001424D0\n.size cFinishLineControl__cFinishLineControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor001424D0$13cRiderVbC_hpc = D_00458B18\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor001424D0$12cRiderVb_hpc = D_00458B60\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cResetControl__cResetControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cResetControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cResetControl's (D_00458A20 at +0x6E8, D_00458A68 at
 * +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor00142630::~cVbDtor00142630()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00142630; expose it under the address-derived name.
__asm__(".globl cResetControl__cResetControl\n.type cResetControl__cResetControl,@function\ncResetControl__cResetControl = _$_15cVbDtor00142630\n.size cResetControl__cResetControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00142630$13cRiderVbC_hpc = D_00458A20\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00142630$12cRiderVb_hpc = D_00458A68\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cVoidControl__cVoidControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cVoidControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cVoidControl's (D_00458928 at +0x6E8, D_00458970 at
 * +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the flags are 0),
 * destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees
 * the object when bit 0 is set.
 */
cVbDtor00142790::~cVbDtor00142790()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00142790; expose it under the address-derived name.
__asm__(".globl cVoidControl__cVoidControl\n.type cVoidControl__cVoidControl,@function\ncVoidControl__cVoidControl = _$_15cVbDtor00142790\n.size cVoidControl__cVoidControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00142790$13cRiderVbC_hpc = D_00458928\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor00142790$12cRiderVb_hpc = D_00458970\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cHandplantControl__cHandplantControl);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a cHandplantControl part; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code points the cRider's vtables back at cHandplantControl's (D_00458830 at +0x6E8,
 * D_00458878 at +0x6C0, the shared D_00459B90 at +0x6D0; stack copies with adjusted this-deltas when the
 * flags are 0), destroys the virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is
 * set, and frees the object when bit 0 is set.
 */
cVbDtor001428F0::~cVbDtor001428F0()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor001428F0; expose it under the address-derived name.
__asm__(".globl cHandplantControl__cHandplantControl\n.type cHandplantControl__cHandplantControl,@function\ncHandplantControl__cHandplantControl = _$_15cVbDtor001428F0\n.size cHandplantControl__cHandplantControl,0x15C\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor001428F0$13cRiderVbC_hpc = D_00458830\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$15cVbDtor001428F0$12cRiderVb_hpc = D_00458878\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRiderHandler__cRiderHandler);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"

/**
 * Destroys a rider handler; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code restores cRiderHandler's vtable (cRiderHandler_vtbl at +0xDE8), destroys the FX parts with their
 * own destructors (cCheatFX__cCheatFX back to cBoardWakeFX__cBoardWakeFX), runs the inlined motion and
 * control part destructors (their cRider vtable fix-ups) and cWipeoutMotion__cWipeoutMotion(), destroys the
 * virtual cRider base with cRider__cRider() when bit 1 of the destructor flags is set, and frees the object
 * when bit 0 is set.
 */
cVbDtor00142A50::~cVbDtor00142A50()
{
}

// PORT: the compiler names this dtor _$_15cVbDtor00142A50; expose it under the address-derived name.
__asm__(".globl cRiderHandler__cRiderHandler\n.type cRiderHandler__cRiderHandler,@function\ncRiderHandler__cRiderHandler = _$_15cVbDtor00142A50\n.size cRiderHandler__cRiderHandler,0x15A0\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$15cVbDtor00142A50 = cRiderHandler_vtbl\n"
        "_vt$15cVbDtor00142A50$13cRiderVbC_hpc = D_00458738\n"
        "_vt$15cVbDtor00142A50$12cRiderVb_hpc = D_00458780\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_$_15cVbDtor002F6388 = cCheatFX__cCheatFX\n"
        "_$_15cVbDtor002F0FE8 = cPunchFX__cPunchFX\n"
        "_$_15cVbDtor002DF1C8 = cDynamicSprayFX__cDynamicSprayFX\n"
        "_$_15cVbDtor002E37D0 = cPlayerIndicatorFX__cPlayerIndicatorFX\n"
        "_$_15cVbDtor002D4A70 = cAngerFX__cAngerFX\n"
        "_$_15cVbDtor002EF530 = cAirStreamerFX__cAirStreamerFX\n"
        "_$_15cVbDtor002EAC60 = cTrickBoostFX__cTrickBoostFX\n"
        "_$_15cVbDtor002E64C0 = cSpeedBoostFX__cSpeedBoostFX\n"
        "_$_15cVbDtor002E83F0 = cBoardTrailFX__cBoardTrailFX\n"
        "_$_15cVbDtor002DA8E8 = cBoardSparkFX__cBoardSparkFX\n"
        "_$_15cVbDtor002DCDA0 = cBoardWakeFX__cBoardWakeFX\n"
        "_vt$13cVbIn00142790$13cRiderVbC_hpc = D_00458928\n"
        "_vt$13cVbIn00142790$12cRiderVb_hpc = D_00458970\n"
        "_vt$13cVbIn001420C8$13cRiderVbC_hpc = D_00458D08\n"
        "_vt$13cVbIn001420C8$12cRiderVb_hpc = D_00458D50\n"
        "_vt$13cVbIn001428F0$13cRiderVbC_hpc = D_00458830\n"
        "_vt$13cVbIn001428F0$12cRiderVb_hpc = D_00458878\n"
        "_vt$13cVbIn001424D0$13cRiderVbC_hpc = D_00458B18\n"
        "_vt$13cVbIn001424D0$12cRiderVb_hpc = D_00458B60\n"
        "_vt$13cVbIn00142630$13cRiderVbC_hpc = D_00458A20\n"
        "_vt$13cVbIn00142630$12cRiderVb_hpc = D_00458A68\n"
        "_vt$13cVbIn00142228$13cRiderVbC_hpc = D_00458C10\n"
        "_vt$13cVbIn00142228$12cRiderVb_hpc = D_00458C58\n"
        "_vt$13cVbIn00141F68$13cRiderVbC_hpc = D_00458E00\n"
        "_vt$13cVbIn00141F68$12cRiderVb_hpc = D_00458E48\n"
        "_vt$13cVbIn00141E08$13cRiderVbC_hpc = D_00458EF8\n"
        "_vt$13cVbIn00141E08$12cRiderVb_hpc = D_00458F40\n"
        "_vt$13cVbIn00141CA8$13cRiderVbC_hpc = D_00458FF0\n"
        "_vt$13cVbIn00141CA8$12cRiderVb_hpc = D_00459038\n"
        "_vt$13cVbIn00141B48$13cRiderVbC_hpc = D_004590E8\n"
        "_vt$13cVbIn00141B48$12cRiderVb_hpc = D_00459130\n"
        "_vt$13cVbIn001419E8$13cRiderVbC_hpc = D_004591E0\n"
        "_vt$13cVbIn001419E8$12cRiderVb_hpc = D_00459228\n"
        "_vt$13cVbIn00141888$13cRiderVbC_hpc = D_004592D8\n"
        "_vt$13cVbIn00141888$12cRiderVb_hpc = D_00459320\n"
        "_vt$13cVbIn00141728$13cRiderVbC_hpc = D_004593D0\n"
        "_vt$13cVbIn00141728$12cRiderVb_hpc = D_00459418\n"
        "_vt$13cVbIn001415C8$13cRiderVbC_hpc = D_004594C8\n"
        "_vt$13cVbIn001415C8$12cRiderVb_hpc = D_00459510\n"
        "_vt$13cVbIn00141320$13cRiderVbC_hpc = D_004595C0\n"
        "_vt$13cVbIn00141320$12cRiderVb_hpc = D_00459608\n"
        "_vt$13cVbIn00141040$13cRiderVbC_hpc = D_004598A8\n"
        "_vt$13cVbIn00141040$12cRiderVb_hpc = D_004598F0\n"
        "_vt$13cVbIn001411C0$13cRiderVbC_hpc = D_004596B8\n"
        "_vt$13cVbIn001411C0$12cRiderVb_hpc = D_00459700\n"
        "_$_15cVbDtor00136AD0 = cWipeoutMotion__cWipeoutMotion\n"
        "_vt$13cVbIn00140EE0$13cRiderVbC_hpc = D_004599A0\n"
        "_vt$13cVbIn00140EE0$12cRiderVb_hpc = D_004599E8\n"
        "_vt$13cVbIn00140D80$13cRiderVbC_hpc = D_00459A98\n"
        "_vt$13cVbIn00140D80$12cRiderVb_hpc = D_00459AE0\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRiderHandler__getMotionState);
#ifdef SKIP_ASM
/**
 * Returns the rider handler's current motion state (MotionState, +0xDE0).
 *
 * @return the eMotionState value (MS_Ground 0 .. MS_Handplant 5; MS_Num before the first state is set)
 */
extern "C" int cRiderHandler__getMotionState(cRiderHandler* self) {
    return (self->MotionState);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRiderHandler__getControlState);
#ifdef SKIP_ASM
/**
 * Returns the rider handler's current control state (ControlState, +0xDE4).
 *
 * @return the eControlState value (CS_Cruise 0 .. CS_Void 13; CS_Num before the first state is set)
 */
extern "C" int cRiderHandler__getControlState(cRiderHandler* self) {
    return (self->ControlState);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cComputer_setInput);
#ifdef SKIP_ASM
/**
 * Sets the computer's Input (+0xF40).
 *
 * @param computer  the cComputer
 * @param input  the cInput
 */
extern "C" void cComputer_setInput(cComputer* computer, int input) {
    computer->Input = (cInput*)input;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cComputer_getCurrentTarget);
#ifdef SKIP_ASM
/**
 * Returns the computer's mTargetID.
 *
 * @param computer  the cComputer
 * @return mTargetID
 */
extern "C" int cComputer_getCurrentTarget(cComputer* computer) {
    return (computer->mTargetID);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cComputer_isAboutToRail);
#ifdef SKIP_ASM
/**
 * Returns whether the computer is about to land on a rail: mToRail is set and @p fparg0 is under 1.
 *
 * @param computer  the cComputer
 * @param fparg0  the air time left, in seconds
 * @return 1 when about to rail, else 0
 */
extern "C" int cComputer_isAboutToRail(cComputer* computer, float fparg0) {
    int var_3;

    var_3 = 0;
    if (((computer->mToRail) != 0) && (fparg0 < 1.0f)) {
        var_3 = 1;
    }
    return var_3;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cComputer_setCruiseLogic);
#ifdef SKIP_ASM
extern tComputerCruiseLogic cComputer_mCruiseLogic[];

/**
 * Selects a computer rider's cruise logic: copies entry @p idx of the cruise-logic table (cComputer_mCruiseLogic) into
 * cruiseLogic.
 *
 * Also records the index in mCruiseLogicID.
 *
 * @param idx  the cruise-logic index (On Tour logic)
 */
extern "C" void cComputer_setCruiseLogic(cComputer* self, int idx)
{
    *(&self->cruiseLogic) = cComputer_mCruiseLogic[idx];
    self->mCruiseLogicID = idx;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAIStateHandler_OnLoseState__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing when the AI state handler loses its state; the body is empty.
 */
void cAIStateHandler_OnLoseState(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getDangerMan);
#ifdef SKIP_ASM
/**
 * Returns the AI's DangerMan.
 *
 * @param aiMan  the cAI
 * @return DangerMan
 */
extern "C" int cAI_getDangerMan(cAI* aiMan) {
    return ((int)aiMan->DangerMan);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getNumRiders);
#ifdef SKIP_ASM
/**
 * Returns the AI's NumberOfRidersAllocated.
 *
 * @param aiMan  the cAI
 * @return NumberOfRidersAllocated
 */
extern "C" int cAI_getNumRiders(cAI* aiMan) {
    return (aiMan->NumberOfRidersAllocated);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getNumPlayers);
#ifdef SKIP_ASM
/**
 * Returns the AI's NumberOfPlayersAllocated.
 *
 * @param aiMan  the cAI
 * @return NumberOfPlayersAllocated
 */
extern "C" int cAI_getNumPlayers(cAI* aiMan) {
    return (aiMan->NumberOfPlayersAllocated);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getNumComputers);
#ifdef SKIP_ASM
/**
 * Returns the AI's NumberOfComputersAllocated.
 *
 * @param aiMan  the cAI
 * @return NumberOfComputersAllocated
 */
extern "C" int cAI_getNumComputers(cAI* aiMan) {
    return (aiMan->NumberOfComputersAllocated);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getNumActors);
#ifdef SKIP_ASM
/**
 * Returns the AI's NumberOfActorsAllocated.
 *
 * @param aiMan  the cAI
 * @return NumberOfActorsAllocated
 */
extern "C" int cAI_getNumActors(cAI* aiMan) {
    return (aiMan->NumberOfActorsAllocated);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getNumMissionRiders);
#ifdef SKIP_ASM
/**
 * Returns the AI's NumberOfMissionRidersLoaded.
 *
 * @param aiMan  the cAI
 * @return NumberOfMissionRidersLoaded
 */
extern "C" int cAI_getNumMissionRiders(cAI* aiMan) {
    return (aiMan->NumberOfMissionRidersLoaded);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getRider);
#ifdef SKIP_ASM
/**
 * Returns the AI's Riders[@p index].
 *
 * @param index  the rider index
 * @return the cRider
 */
extern "C" int cAI_getRider(cAI* self, int index) {
    return (int)self->Riders[index];
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getPlayer);
#ifdef SKIP_ASM
/**
 * Returns the AI's Players[@p index].
 *
 * @param index  the player index
 * @return the cPlayer
 */
extern "C" int cAI_getPlayer(cAI* self, int index) {
    return (int)self->Players[index];
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getComputer);
#ifdef SKIP_ASM
/**
 * Returns the AI's Computers[@p index].
 *
 * @param index  the computer index
 * @return the cComputer
 */
extern "C" int cAI_getComputer(cAI* self, int index) {
    return (int)self->Computers[index];
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getActor);
#ifdef SKIP_ASM
/**
 * Returns the AI's Actors[@p index].
 *
 * @param index  the actor index
 * @return the actor's cComputer
 */
extern "C" int cAI_getActor(cAI* self, int index) {
    return (int)self->Actors[index];
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getMissionRider);
#ifdef SKIP_ASM
/**
 * Returns the AI's MissionRiders[@p index].
 *
 * @param index  the mission rider index
 * @return the cMissionRider
 */
extern "C" int cAI_getMissionRider(cAI* self, int index) {
    return (int)self->MissionRiders[index];
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_setComputerRidersActive);
#ifdef SKIP_ASM
/**
 * Sets the AI's ComputerRidersActive flag.
 *
 * @param aiMan  the cAI
 * @param active  the new value
 */
extern "C" void cAI_setComputerRidersActive(cAI* aiMan, int active) {
    aiMan->ComputerRidersActive = active;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_setComputerActorsActive);
#ifdef SKIP_ASM
/**
 * Sets the AI's ComputerActorsActive flag.
 *
 * @param aiMan  the cAI
 * @param active  the new value
 */
extern "C" void cAI_setComputerActorsActive(cAI* aiMan, int active) {
    aiMan->ComputerActorsActive = active;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getComputerActorsActive);
#ifdef SKIP_ASM
/**
 * Returns the AI's ComputerActorsActive flag.
 *
 * @param aiMan  the cAI
 * @return ComputerActorsActive
 */
extern "C" int cAI_getComputerActorsActive(cAI* aiMan) {
    return (aiMan->ComputerActorsActive);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getAITick);
#ifdef SKIP_ASM
/**
 * Returns the AI's AITick.
 *
 * @param aiMan  the cAI
 * @return AITick
 */
extern "C" int cAI_getAITick(cAI* aiMan) {
    return (aiMan->AITick);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_setAITick);
#ifdef SKIP_ASM
/**
 * Sets the AI's AITick.
 *
 * @param aiMan  the cAI
 * @param tick  the tick
 */
extern "C" void cAI_setAITick(cAI* aiMan, int tick) {
    aiMan->AITick = tick;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getEventTimerTick);
#ifdef SKIP_ASM
/**
 * Returns the AI's EventTimerTick.
 *
 * @param aiMan  the cAI
 * @return EventTimerTick
 */
extern "C" int cAI_getEventTimerTick(cAI* aiMan) {
    return (aiMan->EventTimerTick);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_incrementEventTimerTick);
#ifdef SKIP_ASM
/**
 * Adds 1 to the AI's EventTimerTick.
 *
 * @param aiMan  the cAI
 */
extern "C" void cAI_incrementEventTimerTick(cAI* aiMan) {
    aiMan->EventTimerTick = (int) ((aiMan->EventTimerTick) + 1);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_setEventTimerTick);
#ifdef SKIP_ASM
/**
 * Sets the AI's EventTimerTick.
 *
 * @param aiMan  the cAI
 * @param tick  the tick
 */
extern "C" void cAI_setEventTimerTick(cAI* aiMan, int tick) {
    aiMan->EventTimerTick = tick;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getRaceTimeElapsed);
#ifdef SKIP_ASM
/**
 * Returns the race time elapsed in whole seconds: EventTimerTick / 60, truncated.
 *
 * @return the seconds
 */
extern "C" int cAI_getRaceTimeElapsed(cAI* self)
{
    return (int)((float)self->EventTimerTick * 0.01666666753590107f);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getAIState);
#ifdef SKIP_ASM
/**
 * Returns the AI's CurrentAIState.
 *
 * @param aiMan  the cAI
 * @return CurrentAIState
 */
extern "C" int cAI_getAIState(int *aiMan) {
    return *aiMan;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getCountdownTicksLeft);
#ifdef SKIP_ASM
/**
 * Returns the AI's CountdownTicksLeft.
 *
 * @param aiMan  the cAI
 * @return CountdownTicksLeft
 */
extern "C" int cAI_getCountdownTicksLeft(cAI* aiMan) {
    return (aiMan->CountdownTicksLeft);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_setCountdownTicksLeft);
#ifdef SKIP_ASM
/**
 * Sets the AI's CountdownTicksLeft.
 *
 * @param aiMan  the cAI
 * @param ticksLeft  the ticks
 */
extern "C" void cAI_setCountdownTicksLeft(cAI* aiMan, int ticksLeft) {
    aiMan->CountdownTicksLeft = ticksLeft;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_setPodiumActor);
#ifdef SKIP_ASM
/**
 * Sets PodiumActorList[@p index] to @p value.
 *
 * @param index  the podium slot
 * @param value  the character id
 */
extern "C" void cAI_setPodiumActor(cAI* self, int index, int value) {
    self->PodiumActorList[index] = value;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getPodiumActor);
#ifdef SKIP_ASM
/**
 * Returns PodiumActorList[@p index].
 *
 * @param index  the podium slot
 * @return the character id
 */
extern "C" int cAI_getPodiumActor(cAI* self, int index) {
    return self->PodiumActorList[index];
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getEndOfRaceTimer);
#ifdef SKIP_ASM
/**
 * Returns the AI's EndOfRaceTimer (+0x18).
 *
 * @param aiMan  the cAI
 * @return EndOfRaceTimer
 */
extern "C" int cAI_getEndOfRaceTimer(cAI* aiMan) {
    return (aiMan->EndOfRaceTimer);
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cAI_getFocusMan);
#ifdef SKIP_ASM
/**
 * Returns the AI's FocusMan.
 *
 * @param aiMan  the cAI
 * @return FocusMan
 */
extern "C" int cAI_getFocusMan(cAI* aiMan) {
    return ((int)aiMan->FocusMan);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cDangerMan_DeleteTrackedDangerElemRecord__FPvT0);
#ifdef SKIP_ASM
/**
 * Frees one tracked danger element: sets its state (+0x8) to 2 (free) and decrements the tracked count (+0x704).
 *
 * @param entry  the element record
 * @return 2
 */
int cDangerMan_DeleteTrackedDangerElemRecord(void* vself, void* entry)
{
    cDangerMan* self = (cDangerMan*)vself;
    int inactiveState = 2;
    *(int*)((char*)entry + 0x8) = inactiveState;
    self->mTrackedDangerElemNum = (int)((char*)self->mTrackedDangerElemNum - 0x1);
    return inactiveState;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cDangerMan_CalcAppliedDangerLevel);
#ifdef SKIP_ASM





struct sHpGame_41B8 {
    char pad_0x0[0x28];
    char* riders[8];        // 0x28
};


extern "C" void cAvalanche_getAvalancheDataByPosition(tAvalanchePositionalData* out, t3Vector* pos);

/**
 * Works out the danger level that applies to rider @p pl: its base level raised by its height band or capped by the
 * danger zones.
 *
 * Returns 0 when the rider's level (+0x70C + pl * 12) is 0. With height checks on (+0x708) the ground height under the
 * rider (cAvalanche_getAvalancheDataByPosition at its position, virtual slot 5) gives band 3 below 1200.4, 2 below 2999.7, else 1, and a higher
 * band is returned. Otherwise levels below 2, or no zones, give 1; level 2 and up are returned only while the rider is
 * inside one of the active zones (type other than 2, sphere of its radius around its centre), else 1.
 *
 * @param pl  the rider index
 * @return the danger level
 */
// Handplant difficulty level for player pl: at least the height band's level, and only above 1 when the rider is
// inside one of the enabled zones.
extern "C" int cDangerMan_CalcAppliedDangerLevel(cDangerMan* self, int pl)
{
    int lvl = ((&self->mRiderDangerTrack[0]))[pl].baseLevel;
    char* rider = ((sHpGame_41B8*)self->mpAI)->riders[pl];
    if (lvl == 0) {
        return 0;
    }
    char* riderObj = rider + 0x6C0;
    sVEntry<float*(void*)>* vt = *(sVEntry<float*(void*)>**)riderObj;
    float* pos = vt[5].fn(vthis(riderObj, vt[5]));
    if (self->mNumActiveAvalanche != 0) {
        tAvalanchePositionalData ground;
        t3Vector probe;
        probe.x = pos[0];
        probe.y = pos[1];
        probe.z = pos[2];
        cAvalanche_getAvalancheDataByPosition(&ground, &probe);
        int band = 1;
        if (ground.m_distance < 1200.446f) {
            band = 3;
        } else if (ground.m_distance < 2999.697f) {
            band = 2;
        }
        if (lvl < band) {
            return band;
        }
    }
    if (lvl < 2) {
        return 1;
    }
    if (self->mTrackedDangerElemNum == 0) {
        return 1;
    }
    int n = self->mTrackedDangerElemNum;
    tDangerElemInfo* z = ((&self->mTrackedDangerElems[0]));
    for (int i = 0; i < 64 && n != 0; i++, z++) {
        if (z->state == 2) {
            continue;
        }
        n--;
        float distSq = (*(t3Vector*)&z->pos[0]).x - pos[0];
        float radiusSq = z->radius * z->radius;
        distSq = distSq * distSq;
        if (radiusSq < distSq) {
            continue;
        }
        float dy = (*(t3Vector*)&z->pos[0]).y - pos[1];
        dy = dy * dy;
        if (radiusSq < dy) {
            continue;
        }
        float dz = (*(t3Vector*)&z->pos[0]).z - pos[2];
        distSq = distSq + dy + dz * dz;
        if (!(radiusSq < distSq)) {
            return lvl;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", func_00144368);
#ifdef SKIP_ASM




extern "C" void FocusQuery_addcell(void* a, nash_Cell* node, int leaf);

/**
 * Walks a focus octree node and its children, calling FocusQuery_addcell for every cell that overlaps one of the boxes.
 *
 * @p key gives the cell's level and integer x/y/z; the cell spans [x, x + 1] x 2^level on each axis, widened by 0.2.
 * When the cell overlaps none of @p self's boxes it returns. Otherwise it calls FocusQuery_addcell with @p self's first
 * word, @p node and level < 14 and, unless that was a leaf (level below 14) or the level is 11, recurses into the eight
 * non-null children with the halved cell keys.
 *
 * @param node  the cell node (eight child pointers)
 * @param key  the cell's level and coordinates
 */
extern "C" void func_00144368(FocusCallback* self, nash_Cell* node, nash_CellKey* key)
{
    int n = self->numboxes;
    int i = 0;
    int ok;
    do {
        if (i == n) {
            return;
        }
        union { int i; float f; } scaleBits;
        scaleBits.i = (key->level + 0x7F) << 23;
        float cellSize = scaleBits.f;
        // PORT: pointer arithmetic through int
        tBoundBox* box = (tBoundBox*)((i << 5) + (int)((tBoundBox*)self->boxes));
        ok = 0;
        if (((float)key->x - 0.2f) * cellSize <= box->MaxPoint.x && ((float)key->y - 0.2f) * cellSize <= box->MaxPoint.y && ((float)key->z - 0.2f) * cellSize <= box->MaxPoint.z
            && box->MinPoint.x <= ((float)(key->x + 1) + 0.2f) * cellSize && box->MinPoint.y <= ((float)(key->y + 1) + 0.2f) * cellSize && box->MinPoint.z <= ((float)(key->z + 1) + 0.2f) * cellSize) {
            ok = 1;
        }
        i++;
    } while (!ok);
    int leaf = key->level < 14;
    FocusQuery_addcell(((void*)self->query), node, leaf);
    if (leaf) {
        return;
    }
    if (key->level == 11) {
        return;
    }
    nash_CellKey k;
    k.level = key->level - 1;
    k.x = key->x * 2;
    k.y = key->y * 2;
    k.z = key->z * 2;
    if (((nash_Cell**)&node->children[0][0][0])[0]) func_00144368(self, ((nash_Cell**)&node->children[0][0][0])[0], &k);
    k.x++;
    if (((nash_Cell**)&node->children[0][0][0])[4]) func_00144368(self, ((nash_Cell**)&node->children[0][0][0])[4], &k);
    k.y++;
    if (((nash_Cell**)&node->children[0][0][0])[6]) func_00144368(self, ((nash_Cell**)&node->children[0][0][0])[6], &k);
    k.x--;
    if (((nash_Cell**)&node->children[0][0][0])[2]) func_00144368(self, ((nash_Cell**)&node->children[0][0][0])[2], &k);
    k.z++;
    if (((nash_Cell**)&node->children[0][0][0])[3]) func_00144368(self, ((nash_Cell**)&node->children[0][0][0])[3], &k);
    k.x++;
    if (((nash_Cell**)&node->children[0][0][0])[7]) func_00144368(self, ((nash_Cell**)&node->children[0][0][0])[7], &k);
    k.y--;
    if (((nash_Cell**)&node->children[0][0][0])[5]) func_00144368(self, ((nash_Cell**)&node->children[0][0][0])[5], &k);
    k.x--;
    if (((nash_Cell**)&node->children[0][0][0])[1]) func_00144368(self, ((nash_Cell**)&node->children[0][0][0])[1], &k);
}
#endif

extern "C" void* cRiderAnimBase__cRiderAnimBase(void* self);

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRiderAnim__cRiderAnim__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a rider animation object by handing it to the base destructor cRiderAnimBase__cRiderAnimBase().
 *
 * That destructor expires the object's six animation sequencers and frees their storage, then frees the object
 * itself when bit 0 of the destructor flags is set. Only @p self is declared here: the flags the caller passed in
 * the second argument register reach cRiderAnimBase__cRiderAnimBase() unchanged.
 *
 * @return no meaningful value (cRiderAnimBase__cRiderAnimBase() returns nothing)
 */
void* cRiderAnim__cRiderAnim(void* self)
{
    return cRiderAnimBase__cRiderAnimBase(self);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cRiderAnim_cRiderAnim);
#ifdef SKIP_ASM
extern "C" void* cRiderAnimBase_cRiderAnimBase(void* self);
extern void* cRiderAnim_vtbl[];

/**
 * Constructs a cRiderAnim: cRiderAnimBase_cRiderAnimBase, then +0x60 and +0x64 cleared and the cRiderAnim vtable at
 * +0x58.
 *
 * @return the object
 */
extern "C" void* cRiderAnim_cRiderAnim(cRiderAnim* self)
{
    cRiderAnimBase_cRiderAnimBase(((char*)self));
    self->Rider = 0;
    self->mVtbl = cRiderAnim_vtbl;
    self->FERider = 0;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", tEventFlags_clear1);
#ifdef SKIP_ASM
/**
 * Clears event flag @p bit in a 64-bit flag set (+0x0) and sets it in the second set (+0x8) when it was set.
 *
 * @param bit  the flag number, 0-63
 */
// PORT: ulong is 64-bit here
extern "C" void tEventFlags_clear1(tEventFlags* self, int bit)
{
    ulong mask = (ulong)1 << bit;
    ulong oldBits = *(ulong*)self;
    *(ulong*)self = oldBits & ~mask;
    self->IsJustChanged |= mask & oldBits;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", tEventFlags_isSet1);
#ifdef SKIP_ASM
/**
 * Returns whether event flag @p bit is set in a 64-bit flag set (+0x0).
 *
 * @param bit  the flag number, 0-63
 * @return true when set
 */
extern "C" bool tEventFlags_isSet1(tEventFlags* self, int bit)
{
    ulong mask = (ulong)1 << bit;
    return (*(ulong*)self & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", tEventFlags_isJustChanged);
#ifdef SKIP_ASM
/**
 * Returns whether event flag @p bit is set in the second 64-bit set (+0x8), the one tEventFlags_clear1 fills.
 *
 * @param bit  the flag number, 0-63
 * @return true when set
 */
extern "C" bool tEventFlags_isJustChanged(tEventFlags* self, int bit)
{
    ulong mask = (ulong)1 << bit;
    return (self->IsJustChanged & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cPlayer_setInput);
#ifdef SKIP_ASM
/**
 * Attaches an input source and a pad device to the player, storing them in Input and Pad.
 *
 * @param input  the cInput to read controls from (passed as an int)
 * @param device  the cPadDevice (passed as an int)
 */
extern "C" void cPlayer_setInput(cPlayer* self, int input, int device) {
    self->Input = (cInput*)input;
    self->Pad = (cPadDevice*)device;
}
#endif
//100%
INCLUDE_ASM("ai/control/handplantcontrol", cPlayer_getInput);
#ifdef SKIP_ASM
/**
 * Returns the player's Input.
 *
 * @param player  the cPlayer
 * @return Input
 */
extern "C" int cPlayer_getInput(cPlayer* player) {
    return ((int)player->Input);
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cPlayer_scorePath__FPv);
#ifdef SKIP_ASM
/**
 * Scores an AI path for a human player; humans do not choose paths, so every path scores 0.
 *
 * The cPlayer override of the path-scoring virtual (cRider vtable slot 20) that cRider_checkAIPathSwitch
 * calls; cComputer_scorePath is the real one. The path argument is not read and is left out of the C signature.
 *
 * @return always 0.0
 */
float cPlayer_scorePath(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cMissionRider__cMissionRider);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "ai/control/handplantcontrol_classes.h"
#include "replay/bxreplayframe.h"

/**
 * Destroys a mission rider; the source body is empty, so all the work is compiler-generated.
 *
 * The generated code restores the vtables, runs the base destructor cPlayer__cPlayer(), destroys the virtual
 * cRider base with cRider__cRider() when in charge, and frees the object when bit 0 of the destructor flags is set.
 */
cHpD_1446F8::~cHpD_1446F8()
{
}

// PORT: the compiler names this dtor _$_11cHpD_1446F8; expose it under the address-derived name.
__asm__(".globl cMissionRider__cMissionRider\n.type cMissionRider__cMissionRider,@function\ncMissionRider__cMissionRider = _$_11cHpD_1446F8\n.size cMissionRider__cMissionRider,0x1B0\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$12cHpD1_1446F8 = cPlayer_vtbl\n"
        "_vt$11cHpD_1446F8$13cRiderVbC_hpc = D_00458240\n"
        "_vt$14cRiderVbB0_hpc = D_00459B90\n"
        "_vt$11cHpD_1446F8$12cRiderVb_hpc = D_00458288\n"
        "_$_12cHpD1_1446F8 = cPlayer__cPlayer\n"
        "_$_12cRiderVb_hpc = cRider__cRider\n");
#endif

//100%
INCLUDE_ASM("ai/control/handplantcontrol", cPlaybackMan_isRecord);
#ifdef SKIP_ASM
/**
 * Returns whether the playback manager's mode word (+0x70) is 3.
 *
 * cRider_updateSplit1 calls it on SSXApp->Game->PlaybackMan.
 *
 * @return 1 when the mode is 3, else 0
 */
extern "C" int cPlaybackMan_isRecord(cPlaybackMan* self)
{
    return self->State == 3;
}
#endif

extern "C" void* __sti__all_in_one_ai_cpp(int, int);

//99.38%
INCLUDE_ASM("ai/control/handplantcontrol", func_001448B8__FPv);
#ifdef SKIP_ASM
/**
 * Runs the unit's static initialisers: __sti__all_in_one_ai_cpp(1, 0xFFFF).
 *
 * @return __sti__all_in_one_ai_cpp's result
 */
void* func_001448B8(void* self)
{
    return __sti__all_in_one_ai_cpp(1, 0xffff);
}
#endif

