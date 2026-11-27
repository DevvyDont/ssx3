#include "common.h"
#include "world/instancerecord.h"
#include "intersect/worldrider.h"
#include "bx/math3d.h"
#include "object/instancenode.h"
#include "ai/airpredictor.h"
#include "vtable.h"
#include "animation/animmodel.h"
#include "replay/frameptr.h"
#include "ai/riderhandler.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"
#include "animation/rideranimbase.h"
#include "replay/bxreplayframe.h"
#include "object/being.h"
#include "intersect/worldline.h"
#include "world/patchrecord.h"

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_gainFocus);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);
struct cRiderSphereTree;
cRiderSphereTree* cRiderSphereTree_cRiderSphereTree(cRiderSphereTree* self);
extern char str_BoardSphereTree[];


// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0DotWG(const t4Vector& a, const t4Vector& b)
{
    float r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf5, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %0, $vf4\n"
        : "=r"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0ScaleMemWG(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__ __volatile__(
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
static inline void vu0SubWG(t4Vector& dst, const t4Vector& a, const t4Vector& b)
{
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(a), "m"(b));
}

/**
 * Enters the wipeout motion: sets up the board's collision and starts the rider's tumble on the ground or in
 * the air.
 *
 * Creates the BoardSphereTree (0x2B0 bytes, "BoardSphereTree") the first time and collapses BoardIntersectBox
 * onto the rider's position. Coming from MS_Ground it removes the ground-normal part of the rider's velocity
 * and uses WMS_Ground; otherwise WMS_Air, launching the air predictor from the position and velocity
 * (3333.33). Clears ForceNaturalAir and the rider's mSpinRate / mFlipRate.
 *
 * @param prev  the motion state being left (eMotionState; MS_Ground = 0)
 */
void cWipeoutMotion::gainFocus(int prev)
{
    if (BoardSphereTree == 0)
    {
        BoardSphereTree = (cRiderSphereTree*)cRiderSphereTree_cRiderSphereTree((cRiderSphereTree*)cMemMan_alloc(688,
                str_BoardSphereTree, 0, 0));
    }
    cRider* rider = mRider;
    t4Vector* pos = (t4Vector*)(&rider->mPosition);
    BoardIntersectBox.MinPoint = *pos;
    BoardIntersectBox.MaxPoint = *pos;
    if (prev == 0)
    {
        t4Vector* n = (t4Vector*)(&rider->mGroundNormal);
        float normalSpeed = vu0DotWG(rider->mVelocity, *n);
        t4Vector normalPart = vu0ScaleMemWG(*n, normalSpeed);
        t4Vector* velocity = (t4Vector*)(&mRider->mVelocity);
        vu0SubWG(*velocity, *velocity, normalPart);
        *(int*)this = 0;
    }
    else
    {
        *(int*)this = 1;
        cAirPredictor_startLaunchIntoAir(rider->mAirPredictor, pos, &rider->mVelocity, 3333.33349609375f);
    }
    ForceNaturalAir = 0;
    mRider->mSpinRate = 0.0f;
    mRider->mFlipRate = 0.0f;
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_throwItem);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0DotWM(const t4Vector& a, const t4Vector& b)
{
    float r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf5, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %0, $vf4\n"
        : "=r"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0ScaleWM(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__ __volatile__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline void vu0SubWM(t4Vector& dst, const t4Vector& a, const t4Vector& b)
{
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(dst)
        : "m"(a), "m"(b));
}

/**
 * Throws the rider's board free with a given velocity and spin.
 *
 * Copies @p itemBoneID into the rider's root pose override (+0x130: position, then rotation) and enables it
 * (mUseRootPoseOverride), and sets BoardVelocity and BoardAngVelocity. On the ground (WMS_Ground) the
 * ground-normal part of the board velocity is removed.
 *
 * @param initVel  the board's initial velocity (On Tour initVel)
 * @param initAngVel  the board's initial angular velocity (On Tour initAngVel)
 * @param itemBoneID  the board's starting pose, position then rotation (On Tour passes an item bone ID instead)
 */
extern "C" void cWipeoutMotion_throwItem(cWipeoutMotion* self, t4Vector* initVel, t4Vector* initAngVel,
        tPose* itemBoneID)
{
    *(tPose*)&self->mRider->mRootPoseOverridePos = *itemBoneID;
    self->mRider->mUseRootPoseOverride = 1;
    self->BoardAngVelocity = *initAngVel;
    self->BoardVelocity = *initVel;
    if (*(int*)self == 0)
    {
        t4Vector* n = (t4Vector*)(&self->mRider->mGroundNormal);
        float normalSpeed = vu0DotWM(*n, *initVel);
        t4Vector normalPart = vu0ScaleWM(*n, normalSpeed);
        vu0SubWM(self->BoardVelocity, self->BoardVelocity, normalPart);
    }
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_throwRider);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0ScaleMemWM_136DE0(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__ __volatile__(
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

/**
 * Throws the rider with a given velocity and spin during a wipeout.
 *
 * Sets RiderAngVelocity and the rider's mVelocity. On the ground (WMS_Ground) the ground-normal part of the
 * velocity is removed; in the air the air predictor is relaunched from mPosition and mVelocity (3333.33).
 * Clears ForceNaturalAir.
 *
 * @param initVel  initial velocity (On Tour initVel)
 * @param initAngVel  initial angular velocity (On Tour initAngVel)
 */
void cWipeoutMotion::throwRider(t4Vector* initVel, t4Vector* initAngVel)
{
    RiderAngVelocity = *initAngVel;
    mRider->mVelocity = *initVel;
    if (*(int*)this == 0)
    {
        t4Vector* n = (t4Vector*)(&mRider->mGroundNormal);
        float normalSpeed = vu0DotWM(*n, *initVel);
        t4Vector normalPart = vu0ScaleMemWM_136DE0(*n, normalSpeed);
        t4Vector* velocity = (t4Vector*)(&mRider->mVelocity);
        vu0SubWM(*velocity, *velocity, normalPart);
    }
    else
    {
        cRider* rider = mRider;
        cAirPredictor_startLaunchIntoAir(rider->mAirPredictor, &rider->mPosition, &rider->mVelocity, 3333.33349609375f);
    }
    ForceNaturalAir = 0;
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_step);
#ifdef SKIP_ASM
extern "C" void cWipeoutMotion_stepThrownBoard();
extern "C" void cWipeoutMotion_stepGroundMotion(cWipeoutMotion*);
extern "C" void cWipeoutMotion_stepAirMotion(cWipeoutMotion*);

/**
 * Steps the wipeout physics one tick: the thrown board, then the rider on the ground or in the air.
 *
 * Calls cWipeoutMotion_stepThrownBoard (the board, while the root pose override is on), then cWipeoutMotion_stepGroundMotion for WMS_Ground or
 * cWipeoutMotion_stepAirMotion (air predictor) for WMS_Air. cWipeoutMotion_stepThrownBoard is declared without arguments here but still
 * receives self.
 */
void cWipeoutMotion::step()
{
    cWipeoutMotion_stepThrownBoard();
    if (WipeoutMotionState == 0)
    {
        this->stepGroundMotion();
    }
    else
    {
        this->stepAirMotion();
    }
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_resolve);
#ifdef SKIP_ASM
extern "C" void cWipeoutMotion_resolveThrownBoard();
extern "C" void cWipeoutMotion_resolveGroundMotion(cWipeoutMotion*);
extern "C" void cWipeoutMotion_resolveAirMotion(cWipeoutMotion*);

/**
 * Resolves the wipeout's collisions for this tick: the thrown board, then the rider on the ground or in the
 * air.
 *
 * Calls cWipeoutMotion_resolveThrownBoard (the board against the world), then cWipeoutMotion_resolveGroundMotion for WMS_Ground or cWipeoutMotion_resolveAirMotion for
 * WMS_Air. cWipeoutMotion_resolveThrownBoard is declared without arguments here but still receives self.
 */
void cWipeoutMotion::resolve()
{
    cWipeoutMotion_resolveThrownBoard();
    if (WipeoutMotionState == 0)
    {
        this->resolveGroundMotion();
    }
    else
    {
        this->resolveAirMotion();
    }
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_loseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Leaves the wipeout motion; there is nothing to undo, so the body is empty.
 *
 * On Tour's version also takes the next eMotionState, which this declaration omits.
 */
void cWipeoutMotion::loseFocus()
{
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_stepThrownBoard);
#ifdef SKIP_ASM


// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline void vu0Scale_6F30(t4Vector& out, const t4Vector& v, float s)
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
    out = r;
}

// PORT: PS2-only VU0 inline asm (in-place vector * scalar).
static inline void vu0ScaleEq_6F30(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq_6F30(t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_6F30(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (quaternion derivative 0.5 * (0, w) * q).
static inline void vu0QuatDeriv_6F30(t4Vector& out, const t4Vector& q, const t4Vector& w)
{
    t4Vector r;
    __asm__(
        "qmtc2.ni  %3, $vf1\n"
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsuba.w   ACC, $vf0, $vf0\n"
        "vmsubx.w  $vf2, $vf0, $vf1x\n"
        "vmul.xyz  $vf7, $vf4, $vf5\n"
        "vmulw.xyz $vf3, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vopmsub.xyz $vf6, $vf4, $vf5\n"
        "vmulax.w  ACC, $vf2, $vf7x\n"
        "vmadday.w ACC, $vf2, $vf7y\n"
        "vmaddz.w  $vf8, $vf2, $vf7z\n"
        "vmulax.xyz ACC, $vf3, $vf1x\n"
        "vmaddx.xyz $vf8, $vf6, $vf1x\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(w), "r"(0x3F000000)
        : "memory");
    out = r;
}

// PORT: PS2-only VU0 inline asm (in-place normalize).
static inline void vu0NormalizeEq_6F30(t4Vector& v)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v)
        :
        : "memory");
}

/**
 * Moves the thrown board one tick while the rider's root pose override is on (mUseRootPoseOverride).
 *
 * With dt = mTimeScale / 60 it moves mRootPoseOverridePos by BoardVelocity, adds a drag of -0.2 x the horizontal
 * velocity and gravity -1800 to BoardVelocity and clamps its length to 3333.3 (120 km/h), damps BoardAngVelocity by 1 -
 * dt / 2, and integrates the quaternion mRootPoseOverrideRot by it (VU0 quaternion derivative), then renormalises it.
 * cWipeoutMotion_step calls it without arguments; self still arrives in a0.
 */
extern "C" void cWipeoutMotion_stepThrownBoard(cWipeoutMotion* self)
{
    if (self->mRider->mUseRootPoseOverride == 0) {
        return;
    }
    float dt = self->mRider->mTimeScale * 0.01666666753590107f;
    t4Vector tmp;
    vu0Scale_6F30(tmp, self->BoardVelocity, dt);
    vu0AddEq_6F30(self->mRider->mRootPoseOverridePos, tmp);
    t4Vector v2;
    v2.x = self->BoardVelocity.x * -0.20000000298023224f;
    v2.y = self->BoardVelocity.y * -0.20000000298023224f;
    v2.z = -1800.0f;
    v2.w = 0.0f;
    vu0Scale_6F30(tmp, v2, dt);
    vu0AddEq_6F30(self->BoardVelocity, tmp);
    float len = vu0Length_6F30(self->BoardVelocity);
    if (3333.33349609375f < len) {
        vu0ScaleEq_6F30(self->BoardVelocity, 3333.33349609375f / len);
    }
    vu0ScaleEq_6F30(self->BoardAngVelocity, 1.0f - dt * 0.5f);
    vu0QuatDeriv_6F30(v2, self->mRider->mRootPoseOverrideRot, self->BoardAngVelocity);
    vu0Scale_6F30(tmp, v2, dt);
    cRider* r = self->mRider;
    r->mRootPoseOverrideRot.x += tmp.x;
    r->mRootPoseOverrideRot.y += tmp.y;
    r->mRootPoseOverrideRot.z += tmp.z;
    r->mRootPoseOverrideRot.w += tmp.w;
    vu0NormalizeEq_6F30(self->mRider->mRootPoseOverrideRot);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_resolveThrownBoard);
#ifdef SKIP_ASM
extern "C" void* cWorldRider_cWorldRider(void* self, void* tree, int flag);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);
extern "C" float cOverlapList_terrainIntersect1(void* world, void* seg, void* hit, tIntHint* hint);
extern "C" void cWipeoutMotion_updateBoardSphereTree(cWipeoutMotion* self);
extern "C" float cBXNash_terrainIntersect1_137138(void* self, void* q, void* out, int a3) __asm__("cBXNash_terrainIntersect1");
extern "C" void cRiderSphereTree_ApplyOffset_137138(void* tree, const t4Vector* d) __asm__("cRiderSphereTree_ApplyOffset");
extern "C" void fSinCos(float* s, float* c, float angle);
float AIrandf(float lo, float hi);
extern t4Vector t4vect_Zero_137138 __asm__("t4vect_Zero");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_137138(const t4Vector& a, const t4Vector& b)
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
static inline float vu0Length_137138(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_137138(const t4Vector& v, const float& s)
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq_137138(t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v *= s).
static inline void vu0ScaleEq_137138(t4Vector& v, float s)
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

// t4Vector built from its four components.
struct t4VectorCtor : public t4Vector {
    t4VectorCtor() {}
    t4VectorCtor(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

/**
 * Collides the thrown board with the world and bounces or stops it.
 *
 * Only while mUseRootPoseOverride is set. Updates BoardSphereTree (cWipeoutMotion_updateBoardSphereTree), builds a
 * cWorldRider probe from it, stores its segment in BoardIntersectBox and tests it against SSXApp->Game->BXNash
 * (cBXNash_terrainIntersect1). On a hit it pushes mRootPoseOverridePos, the sphere tree and the box out along the hit normal by 1.1
 * x the depth. A fast impact (normal speed 555.6 or more) or a steep surface (normal.z below 0.8) bounces the board:
 * BoardVelocity gains (v + max(v / 2, 27.8)) along the normal, the hit object's owner (virtual slot 43) gets the
 * impulse scaled by -0.05 x SSXApp + 0x10, BoardAngVelocity is damped by 0.85 and, under 4 pi rad/s, gets a random
 * 45-180 degree spin. Otherwise the owner gets the board velocity scaled by 0.05 x SSXApp + 0x10 and both board
 * velocities are zeroed. The probe is destroyed at the end. cWipeoutMotion_resolve calls it without arguments; self
 * still arrives in a0.
 *
 * @param vself  the cWipeoutMotion (this)
 */
extern "C" void cWipeoutMotion_resolveThrownBoard(cWipeoutMotion* vself)
{
    cWipeoutMotion* self = vself;
    if (self->mRider->mUseRootPoseOverride == 0)
        return;
    self->updateBoardSphereTree();
    tIntersectInfo hit;
    cWorldRider sph;
    cWorldRider_cWorldRider(&sph, ((void*)self->BoardSphereTree), 1);
    tBoundBox seg;
    seg.MinPoint = sph.boxMin;
    seg.MaxPoint = sph.boxMax;
    self->BoardIntersectBox = seg;
    float d = cBXNash_terrainIntersect1_137138(SSXApp->Game->BXNash, &sph, &hit, 0);
    if (d < 0.0f) {
        cWorldRider__cWorldRider(&sph, 2);
        return;
    }
    t4Vector push = vu0Scale_137138(vu0Scale_137138(hit.Inorm, 1.100000023841858f), d);
    vu0AddEq_137138(self->mRider->mRootPoseOverridePos, push);
    cRiderSphereTree_ApplyOffset_137138(((void*)self->BoardSphereTree), &push);
    // The binary copies the segment back into the local before moving it (the copy is never read).
    seg = self->BoardIntersectBox;
    vu0AddEq_137138(self->BoardIntersectBox.MinPoint, push);
    vu0AddEq_137138(self->BoardIntersectBox.MaxPoint, push);
    float v = vu0Dot_137138(vu0Scale_137138(hit.Inorm, -1.0f), self->BoardVelocity);
    if (555.5555419921875f <= v || hit.Inorm.z < 0.800000011920929f) {
        float k = v * 0.5f;
        if (k < 27.77777862548828f)
            k = 27.77777862548828f;
        t4Vector impulse = vu0Scale_137138(hit.Inorm, v + k);
        vu0AddEq_137138(self->BoardVelocity, impulse);
        if (hit.Inst != 0) {
            cInstanceNode* o = hit.Inst->m_pActiveNode;
            if (o != 0) {
                o->applyForce(hit, vu0Scale_137138(impulse, (float)SSXApp->TimerHz * -0.05000000074505806f));
            }
        }
        vu0ScaleEq_137138(self->BoardAngVelocity, 0.8500000238418579f);
        if (vu0Length_137138(self->BoardAngVelocity) < 12.56637191772461f) {
            float pitch = AIrandf(-1.5707963705062866f, 1.5707963705062866f);
            float yaw = AIrandf(-3.1415927410125732f, 3.1415927410125732f);
            float sp, cp, sy, cy;
            fSinCos(&sp, &cp, pitch);
            fSinCos(&sy, &cy, yaw);
            float deg = AIrandf(45.0f, 180.0f);
            vu0AddEq_137138(self->BoardAngVelocity,
                            vu0Scale_137138(t4VectorCtor(cp * sy, cp * cy, sp, 0.0f), deg * 0.01745329424738884f));
        }
    } else {
        if (hit.Inst != 0) {
            cInstanceNode* o = hit.Inst->m_pActiveNode;
            if (o != 0) {
                o->applyForce(hit, vu0Scale_137138(self->BoardVelocity, (float)SSXApp->TimerHz * 0.05000000074505806f));
            }
        }
        self->BoardVelocity = t4vect_Zero_137138;
        self->BoardAngVelocity = t4vect_Zero_137138;
    }
    cWorldRider__cWorldRider(&sph, 2);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_updateBoardSphereTree);
#ifdef SKIP_ASM

struct sRadii_7550 {
    float r[4];
};
extern const sRadii_7550 D_00458220;

extern "C" void cRiderSphereTree_Update(void* tree, int n, t4Vector* pos, float* radii, t4Vector* center, float radius);

// PORT: PS2-only VU0 inline asm (non-volatile) (vector times scalar).
static inline t4VectorCtor vu0Scale_7550(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (non-volatile) (vector add).
static inline t4VectorCtor vu0Add_7550(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (non-volatile) (vector subtract).
static inline t4VectorCtor vu0Sub_7550(const t4Vector& a, const t4Vector& b)
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

static inline t4VectorCtor xAxis_7550(const sAnimModelBone* b)
{
    return t4VectorCtor(1.0f - 2.0f * (b->rot.y * b->rot.y + b->rot.z * b->rot.z),
                        2.0f * (b->rot.x * b->rot.y + b->rot.w * b->rot.z),
                        2.0f * (b->rot.x * b->rot.z - b->rot.w * b->rot.y),
                        0.0f);
}

/**
 * Rebuilds the wiping-out rider's board collision spheres along the board bone.
 *
 * Takes the rider's mAnimModel bone mBoneBoardChild (+0x8A4) and the model's scale (+0x140), and places four sphere
 * centres along the bone's x axis at +70, +30, -30 and -70 scaled units from the bone position, with radii from
 * D_00458220. cRiderSphereTree_Update then refits BoardSphereTree around them, centred on the bone with radius scale * 90.
 */
void cWipeoutMotion::updateBoardSphereTree()
{
    t4VectorCtor pts[4];
    sRadii_7550 radii = D_00458220;
    cRider* rider = mRider;
    cAnimModel* model = rider->mAnimModel;
    sAnimModelBone* bone = &(model->bones)[rider->mBoneBoardChild];
    float modelScale = model->ScaleVect.x;
    t4VectorCtor boardAxis = vu0Scale_7550(xAxis_7550(bone), modelScale);
    pts[0] = vu0Add_7550(bone->pos, vu0Scale_7550(boardAxis, 70.0f));
    pts[1] = vu0Add_7550(bone->pos, vu0Scale_7550(boardAxis, 30.0f));
    pts[2] = vu0Sub_7550(bone->pos, vu0Scale_7550(boardAxis, 30.0f));
    pts[3] = vu0Sub_7550(bone->pos, vu0Scale_7550(boardAxis, 70.0f));
    cRiderSphereTree_Update(BoardSphereTree, 4, pts, radii.r, &bone->pos,
                  mRider->mAnimModel->ScaleVect.x * 90.0f);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_stepAirMotion);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void cAirPredictor_updateFlightPath(void* ap, void* pos, void* vel, float dt);
extern "C" void cRider_alignRiderAxis(void* rider, void* a, void* b, float x, float y);


// PORT: PS2-only VU0 inline asm (vector times-assign scalar).
static inline void vu0ScaleEqW7(t4Vector& v, float s)
{
    int t;
    __asm__ __volatile__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(v), "=&r"(t)
        : "m"(v), "f"(s));
}

/**
 * Moves the wiped-out rider through the air one tick with the air predictor.
 *
 * Runs cAirPredictor_updateFlightPath on mAirPredictor with mPosition, mVelocity and dt = mTimeScale / 60. Once the
 * predictor's SimState (+0xAC) is kSS_SimDone or kSS_SimCheckObjectsDone (landing found) it turns the rider toward the
 * landing (cRider_alignRiderAxis with the landing normal and velocity, a rate of mTimeScale over the remaining air
 * time, or x 60 when under one tick, and mTimeScale x 6.63). RiderAngVelocity is damped by 1 - 1.5 dt and the
 * orientation updated (cRider_updateOrientationImplicit).
 *
 */
void cWipeoutMotion::stepAirMotion()
{
    cRider* rider = mRider;
    float dt = rider->mTimeScale * 0.01666666753590107f;
    cAirPredictor_updateFlightPath(rider->mAirPredictor, &rider->mPosition, &rider->mVelocity, dt);
    int st = mRider->mAirPredictor->SimState;
    int ok = 0;
    if (st == cAirPredictor::kSS_SimDone || st == cAirPredictor::kSS_SimCheckObjectsDone) ok = 1;
    if (ok)
    {
        cRider* airRider = mRider;
        cAirPredictor* ap = airRider->mAirPredictor;
        float timeLeft = ap->TotalAirTime - ap->CurAirTime;
        float x = airRider->mTimeScale;
        if (timeLeft >= 0.01666666753590107f)
            x /= timeLeft;
        else
            x *= 59.999996185302734f;
        cRider* alignRider = mRider;
        float k = 6.632251739501953f;
        cRider_alignRiderAxis(alignRider, &alignRider->mAirPredictor->GuessLandNormal, &alignRider->mAirPredictor->GuessLandVelocity, x, alignRider->mTimeScale * k);
    }
    vu0ScaleEqW7(RiderAngVelocity, 1.0f - dt * 1.5f);
    cRider_updateOrientationImplicit(mRider);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_resolveAirMotion);
#ifdef SKIP_ASM


extern "C" void* cWorldRider_cWorldRider(void* self, void* tree, int flag);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);
extern "C" float cOverlapList_terrainIntersect1(void* world, void* seg, void* hit, tIntHint* hint);
extern "C" float cWipeoutMotion_intersectGround(cWipeoutMotion* self, tIntersectInfo* hit, t4Vector* dir);
// PORT: the project names these __FPv (one void* param); this caller passes more.
void cAirPredictor_land_137860(void* ap, void* pos, void* vel) __asm__("cAirPredictor_land__FPv");
void cRiderHandler_recordSprayBurst_137860(void* self, void* a, void* b, int c, float x) __asm__("cRiderHandler_recordSprayBurst__FPv");
extern "C" void cRider_moveFromCollision(void* rider, const t4Vector& v);
extern "C" void cRider_updateBoundBox(void* rider, int a1);
extern "C" void cRider_checkObjectCollisions(void* rider, int a1);
extern "C" void cRider_checkOpponentCollisions(void* rider);

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_137860(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector vu0Scale_137860(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_137860(const t4Vector& a, const t4Vector& b)
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
static inline void vu0AddEq_137860(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void vu0SubEq_137860(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

static inline bool isHuman_137860(cRider* rider)
{
    return rider->isHuman();
}


/**
 * Resolves a wiping-out rider in the air against the world: lands it on usable ground, otherwise pushes it out and
 * relaunches it.
 *
 * Builds a world query from the rider's sphere tree (cWorldRider_cWorldRider; terrain only for a computer rider) and intersects
 * it with the rider's overlap list (cOverlapList_terrainIntersect1, mWallHint). On a hit, cWipeoutMotion_intersectGround (On Tour intersectGround) checks
 * the ground below; if that is solid enough (>= 0.4), the rider is not moving away from it and is within 10 of it, the
 * rider lands: cAirPredictor_land (On Tour cAirPredictor::land), the impact effect (cRiderHandler_recordSprayBurst), the handler's landed
 * flag and velocity (+0x314/+0x320), the ground normal, surface, contact point and height, the velocity loses its
 * into-ground part (passed to a hit object's vtable slot 43 scaled by the frame rate), any penetration is pushed out
 * (cRider_moveFromCollision) and the wipeout state (+0x0) becomes 0 (ground).
 *
 * Otherwise the rider is pushed out by 1.1 x the depth along the hit normal, loses its into-wall velocity when the wall
 * is not ground-like (normals differ by more than ~25 degrees), and cAirPredictor_startLaunchIntoAir relaunches it with
 * gravity 3333.33. Always ends with the bound box, object and opponent collision checks.
 *
 */
void cWipeoutMotion::resolveAirMotion()
{
    tIntersectInfo hitA;
    cWorldRider sph;
    tIntersectInfo hitB;
    cRider* rider = mRider;
    cWorldRider_cWorldRider(&sph, rider->mSphereTree, !isHuman_137860(rider));
    float depth = cOverlapList_terrainIntersect1(mRider->mOverlapList, &sph, &hitA, mRider->mWallHint);
    if (depth >= 0.0f)
    {
        float groundParam = intersectGround((tIntersectInfo*)((char*)&hitB), (t4Vector*)&hitA.Inorm);
        float vn = 0.0f;
        float height = 0.0f;
        if (groundParam >= 0.4000000059604645f)
        {
            vn = vu0Dot_137860(mRider->mVelocity, hitB.Inorm);
            height = vu0Dot_137860(vu0Sub_137860(mRider->mPosition, hitB.Ipos), hitB.Inorm);
        }
        if (groundParam >= 0.4000000059604645f && vn <= 0.0f && height <= 10.0f)
        {
            cAirPredictor_land_137860(mRider->mAirPredictor, &mRider->mPosition, &mRider->mVelocity);
            cRiderHandler_recordSprayBurst_137860(mRider->mRiderHandler, &hitB, &hitB.Inorm, hitB.Stype, vn);
            cRider* landedRider = mRider;
            cRiderHandler* hd = landedRider->mRiderHandler;
            hd->WipeoutControl.IsNewImpact = 1;
            hd->WipeoutControl.LatestImpact = landedRider->mVelocity;
            mRider->GroundVelocity = hitB.Ivel;
            mRider->mGroundNormal = hitB.Inorm;
            mRider->mSurfaceType = hitB.Stype;
            mRider->mGroundContactPoint = hitB.Ipos;
            mRider->SnowOffset = vu0Dot_137860(vu0Sub_137860(mRider->mPosition, hitB.Ipos), mRider->mGroundNormal);
            t4Vector impulse = vu0Scale_137860(mRider->mGroundNormal, -vn);
            vu0AddEq_137860(mRider->mVelocity, impulse);
            if (hitB.Inst != 0)
            {
                cInstanceNode* hitOwner = hitB.Inst->m_pActiveNode;
                if (((char*)hitOwner) != 0)
                {
                    hitOwner->applyForce(hitB, vu0Scale_137860(vu0Scale_137860(impulse, -1.0f), (float)SSXApp->TimerHz));
                }
            }
            float pen = vu0Dot_137860(vu0Sub_137860(mRider->mPosition, hitB.Ipos), hitB.Inorm);
            if (pen < 0.0f)
            {
                cRider_moveFromCollision(mRider, vu0Scale_137860(hitB.Inorm, -pen));
            }
            *(int*)this = 0;
        }
        else
        {
            t4Vector push = vu0Scale_137860(vu0Scale_137860(hitA.Inorm, 1.100000023841858f), depth);
            cRider_moveFromCollision(mRider, push);
            if (groundParam < 0.0f || vu0Dot_137860(hitB.Inorm, hitA.Inorm) < 0.8999999761581421f)
            {
                float normSpeed = vu0Dot_137860(hitA.Inorm, mRider->mVelocity);
                if (normSpeed < 0.0f)
                {
                    vu0SubEq_137860(mRider->mVelocity, vu0Scale_137860(hitA.Inorm, normSpeed));
                }
            }
            cAirPredictor_startLaunchIntoAir(mRider->mAirPredictor, &mRider->mPosition, &mRider->mVelocity,
                    3333.33349609375f);
        }
    }
    cRider_updateBoundBox(mRider, 0);
    cRider_checkObjectCollisions(mRider, 0);
    cRider_checkOpponentCollisions(mRider);
    cWorldRider__cWorldRider(&sph, 2);
}

#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_stepGroundMotion);
#ifdef SKIP_ASM

// A point of a piecewise-linear curve.
struct sCurvePt_137D18 { float x, y; };


extern "C" float cWipeoutMotion_intersectGround(cWipeoutMotion* self, tIntersectInfo* hit, t4Vector* dir);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void cRider_alignRiderAxis(void* rider, void* a, void* b, float x, float y);
void cRiderHandler_recordSprayBurst_137D18(void* self, void* a, void* b, int c, float x) __asm__("cRiderHandler_recordSprayBurst__FPv");
extern "C" int cRider_checkForReset(void* rider, int a, int b);
extern "C" void cRider_updateOrientationImplicit(void*);
extern t4Vector t4vect_ZAxis;

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_137D18(const t4Vector& v)
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
static inline float vu0Dot_137D18(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector vu0Scale_137D18(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline t4Vector vu0Div_137D18(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_137D18(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0Add_137D18(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector vu0Normalize_137D18(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq_137D18(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void vu0SubEq_137D18(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

// PORT: PS2-only VU0 inline asm (v *= s).
static inline void vu0ScaleEq_137D18(t4Vector& v, float s)
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

static inline float lerp_137D18(const sCurvePt_137D18& a, const sCurvePt_137D18& b, float x)
{
    float y0 = a.y;
    x -= a.x;
    return y0 + (b.y - y0) * x / (b.x - a.x);
}

static inline float curve_137D18(const tSurfData* c, float x)
{
    if (((sCurvePt_137D18*)&c->SlipFriction[0][0])[1].x < x)
    {
        if (((sCurvePt_137D18*)&c->SlipFriction[0][0])[2].x < x)
        {
            if (((sCurvePt_137D18*)&c->SlipFriction[0][0])[3].x < x)
                return ((sCurvePt_137D18*)&c->SlipFriction[0][0])[3].y;
            return lerp_137D18(((sCurvePt_137D18*)&c->SlipFriction[0][0])[2], ((sCurvePt_137D18*)&c->SlipFriction[0][0])[3], x);
        }
        return lerp_137D18(((sCurvePt_137D18*)&c->SlipFriction[0][0])[1], ((sCurvePt_137D18*)&c->SlipFriction[0][0])[2], x);
    }
    if (x < ((sCurvePt_137D18*)&c->SlipFriction[0][0])[0].x)
        return ((sCurvePt_137D18*)&c->SlipFriction[0][0])[0].y;
    return lerp_137D18(((sCurvePt_137D18*)&c->SlipFriction[0][0])[0], ((sCurvePt_137D18*)&c->SlipFriction[0][0])[1], x);
}

// PORT: <? (g++ min operator).
static inline float clamp_137D18(float v, float lo, float hi)
{
    return lo <= v ? v <? hi : lo;
}

static inline tSurfData* materials_137D18()
{
    return (tSurfData*)SSXApp->Game->SurfPropCache;
}

static inline short triFlags_137D18(char* t)
{
    return *(short*)(t + 0xA);
}

static inline t4Vector getFront_137D18(cRider* rider)
{
    return rider->mMatrix.row[1];
}


/**
 * Moves the wiped-out rider along the ground one tick: sliding friction, gravity, landing on and leaving the surface.
 *
 * RiderAngVelocity's length eases toward the speed x 0.0125. Friction comes from the surface material's curve
 * (SurfPropCache entry for mSurfaceType, its four points at +0x90) at the tangential speed relative to the ground
 * (GroundVelocity), x 3 + 1.5; slow slides (under 1111) during the root pose override or anim categories kANCAT_WSlide
 * / kANCAT_WSlideNoBoard blend toward 40. It is scaled by mGroundNormal.z + 0.3 (clamped 0-1) and capped at 1 / dt.
 * Position and velocity are integrated with gravity -1800 and the speed capped at 3333.3, and the rider is aligned to
 * the ground (cRider_alignRiderAxis). cWipeoutMotion_intersectGround then probes the ground: a hit updates
 * GroundVelocity, mGroundNormal, mSurfaceType, mGroundContactPoint and unk_454; sinking deeper than the material limit
 * (mAnimModel unk_140.x x material +0x18) pushes the rider out and cancels the inward velocity, a shallower dip springs
 * it back, both spawning the impact effect (cRiderHandler_recordSprayBurst); more than 10 above it launches the rider
 * (cAirPredictor_startLaunchIntoAir, ForceNaturalAir (+0x4) = 1), as does a miss. The hit object's owner (virtual slot
 * 43) gets the velocity change, and a material with +0x44 set or a triangle with flag 2 forces a reset
 * (cRider_checkForReset with 0, 1).
 *
 * @param self  the cWipeoutMotion
 */
// Wipeout tumble: damps the spin, slides along the ground with surface friction, then re-probes the ground.
void cWipeoutMotion::stepGroundMotion()
{
    float len = vu0Length_137D18(RiderAngVelocity);
    float vl = vu0Length_137D18(mRider->mVelocity) * 0.012500000186264515f;
    float dt = mRider->mTimeScale * 0.01666666753590107f;
    float nl = len + (vl - len) * 5.0f * dt;
    if (0.0010000000474974513f < len)
        vu0ScaleEq_137D18(RiderAngVelocity, nl / len);
    tSurfData* mat = &materials_137D18()[mRider->mSurfaceType];
    t4Vector netVelocity = vu0Sub_137D18(mRider->mVelocity, mRider->GroundVelocity);
    float vn = vu0Dot_137D18(netVelocity, mRider->mGroundNormal);
    t4Vector n = vu0Scale_137D18(mRider->mGroundNormal, vn);
    t4Vector tang = vu0Sub_137D18(netVelocity, n);
    float tl = vu0Length_137D18(tang);
    float fric = curve_137D18(mat, tl * 0.035999998450279236f) * 3.0f + 1.5f;
    if (tl < 1111.111083984375f
        && (mRider->mUseRootPoseOverride != 0
            || cRiderAnimBase_getCurrentAnimCategory(mRider->mRiderAnim, 2) == kANCAT_WSlide
            || cRiderAnimBase_getCurrentAnimCategory(mRider->mRiderAnim, 2) == kANCAT_WSlideNoBoard)) {
        float slowBlend = 1.0f - tl * 0.0009000000427477062f;
        slowBlend *= slowBlend;
        slowBlend *= slowBlend;
        fric = slowBlend * 40.0f + (1.0f - slowBlend) * fric;
    }
    fric *= clamp_137D18(mRider->mGroundNormal.z + 0.30000001192092896f, 0.0f, 1.0f);
    if (1.0f / dt < fric)
        fric = 1.0f / dt;
    t4Vector grav = vu0Scale_137D18(t4vect_ZAxis, -1800.0f);
    t4Vector drag = vu0Scale_137D18(tang, -fric);
    t4Vector acc = vu0Add_137D18(grav, drag);
    vu0AddEq_137D18(mRider->mPosition, vu0Scale_137D18(mRider->mVelocity, dt));
    vu0AddEq_137D18(mRider->mVelocity, vu0Scale_137D18(acc, dt));
    float sp = vu0Length_137D18(mRider->mVelocity);
    if (3333.33349609375f < sp)
        vu0ScaleEq_137D18(mRider->mVelocity, 3333.33349609375f / sp);
    t4Vector dir = vu0Normalize_137D18(vu0Add_137D18(getFront_137D18(mRider), vu0Scale_137D18(mRider->mVelocity,
            0.0036000001709908247f)));
    cRider_alignRiderAxis(mRider, &mRider->mGroundNormal, &dir, mRider->mTimeScale * 2.0f,
            mRider->mTimeScale * 6.632251739501953f);
    tIntersectInfo hit;
    float groundParam = intersectGround((tIntersectInfo*)((char*)&hit), (t4Vector*)&mRider->mGroundNormal);
    if (0.0f <= groundParam) {
        mRider->GroundVelocity = hit.Ivel;
        mRider->mGroundNormal = hit.Inorm;
        mRider->mSurfaceType = hit.Stype;
        mRider->mGroundContactPoint = hit.Ipos;
        mRider->SnowOffset = vu0Dot_137D18(vu0Sub_137D18(mRider->mPosition, hit.Ipos), mRider->mGroundNormal);
        float lim = mRider->mAnimModel->ScaleVect.x * mat->PowderDepth3G;
        float pen = vu0Dot_137D18(vu0Sub_137D18(mRider->mPosition, hit.Ipos), hit.Inorm);
        if (pen < -lim) {
            float spd = vu0Length_137D18(hit.Ivel);
            float vn2 = vu0Dot_137D18(mRider->mVelocity, hit.Inorm);
            if (vn2 < 0.0f)
                vu0SubEq_137D18(mRider->mVelocity, vu0Scale_137D18(hit.Inorm, vn2));
            vu0SubEq_137D18(mRider->mPosition, vu0Scale_137D18(hit.Inorm, pen + lim));
            cRiderHandler_recordSprayBurst_137D18(mRider->mRiderHandler, &hit, &hit.Inorm, hit.Stype, spd);
        } else if (pen < 0.0f) {
            float spd = vu0Length_137D18(mRider->mVelocity);
            vu0AddEq_137D18(mRider->mVelocity, vu0Scale_137D18(mRider->mGroundNormal,
                    dt * (pen * -2700.0f / lim - vu0Dot_137D18(mRider->mVelocity, mRider->mGroundNormal))));
            cRiderHandler_recordSprayBurst_137D18(mRider->mRiderHandler, &hit, &hit.Inorm, hit.Stype, spd);
        } else if (10.0f < pen) {
            float vn3 = vu0Dot_137D18(mRider->mVelocity, mRider->mGroundNormal);
            if (vn3 < 0.0f)
                vu0SubEq_137D18(mRider->mVelocity, vu0Scale_137D18(mRider->mGroundNormal, vn3));
            cAirPredictor_startLaunchIntoAir(mRider->mAirPredictor, &mRider->mPosition, &mRider->mVelocity,
                    3333.33349609375f);
            ForceNaturalAir = 1;
        }
        if (hit.Inst != 0) {
            cInstanceNode* hitOwner = hit.Inst->m_pActiveNode;
            if (hitOwner != 0) {
                hitOwner->applyForce(hit, vu0Div_137D18(vu0Add_137D18(vu0Sub_137D18(netVelocity, mRider->mVelocity),
                        mRider->GroundVelocity), dt));
            }
        }
        tSurfData* ground = &materials_137D18()[hit.Stype];
        if (ground->IsResetSurface != 0 || (hit.Patch != 0 && (triFlags_137D18(((char*)hit.Patch)) & 2)))
            cRider_checkForReset(mRider, 0, 1);
    } else {
        cAirPredictor_startLaunchIntoAir(mRider->mAirPredictor, &mRider->mPosition, &mRider->mVelocity,
                3333.33349609375f);
        ForceNaturalAir = 1;
    }
    cRider_updateOrientationImplicit(mRider);
}

#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_resolveGroundMotion);
#ifdef SKIP_ASM



extern "C" void* cWorldRider_cWorldRider(void* self, void* tree, int flag);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);
extern "C" float cOverlapList_terrainIntersect1(void* world, void* seg, void* hit, tIntHint* hint);
extern "C" void cRider_moveFromCollision(void* rider, const t4Vector& v);
extern "C" void cRider_checkObjectCollisions(void* rider, int a1);
extern "C" void cRider_checkOpponentCollisions(void* rider);
extern t4Vector t4vect_Zero_138640 __asm__("t4vect_Zero");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_138640(const t4Vector& a, const t4Vector& b)
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
static inline float vu0Length_138640(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_138640(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_138640(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector vu0Normalize_138640(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void vu0AddEq_138640(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b));
}

// PORT: <? (g++ min operator).
static inline float clamp_138640(float v, float lo, float hi)
{
    return lo <= v ? v <? hi : lo;
}


/**
 * Resolves a wiping-out rider sliding on the ground against walls and objects.
 *
 * Intersects the rider's sphere tree with its overlap list, rejecting along mGroundNormal. A hit that is not straight
 * into the ground (normal dot > -0.9999) has its normal flattened against the ground when facing down, notifies the hit
 * object (vtable slot 42), pushes the rider out by 1.1 x the depth (cRider_moveFromCollision) and, when moving into the wall
 * relative to the object's own velocity, bounces it back by that speed plus half of it clamped to 27.8..555.6 (1..20
 * kph).
 *
 * After the bound box, object and opponent collision checks, a pending flag (+0x4) switches the motion to state 1
 * (air), and a speed under 50 stops the rider.
 *
 */
void cWipeoutMotion::resolveGroundMotion()
{
    tIntersectInfo hit;
    cWorldRider sph;
    cWorldRider_cWorldRider(&sph, mRider->mSphereTree, 1);
    sph.useRejectNormal = 1;
    sph.rejectNormal = mRider->mGroundNormal;
    float depth = cOverlapList_terrainIntersect1(mRider->mOverlapList, &sph, &hit, mRider->mWallHint);
    if (0.0f <= depth) {
        float dot = vu0Dot_138640(mRider->mGroundNormal, hit.Inorm);
        if (-0.9998999834060669f < dot) {
            if (dot < 0.0f)
                hit.Inorm = vu0Normalize_138640(vu0Sub_138640(hit.Inorm, vu0Scale_138640(mRider->mGroundNormal, dot)));
            if (hit.Inst != 0) {
                cInstanceNode* hitOwner = hit.Inst->m_pActiveNode;
                if (hitOwner != 0) {
                    hitOwner->updateIntersectInfo(hit);
                }
            }
            t4Vector push = vu0Scale_138640(vu0Scale_138640(hit.Inorm, 1.100000023841858f), depth);
            cRider_moveFromCollision(mRider, push);
            t4Vector netVelocity = vu0Sub_138640(mRider->mVelocity, hit.Ivel);
            float dot2 = vu0Dot_138640(vu0Scale_138640(hit.Inorm, -1.0f), netVelocity);
            if (0.0f <= dot2) {
                float k = clamp_138640(dot2 * 0.5f, 27.77777862548828f, 555.5555419921875f);
                vu0AddEq_138640(mRider->mVelocity, vu0Scale_138640(hit.Inorm, dot2 + k));
            }
        }
    }
    cRider_updateBoundBox(mRider, 0);
    cRider_checkObjectCollisions(mRider, 0);
    cRider_checkOpponentCollisions(mRider);
    if (ForceNaturalAir != 0) {
        ForceNaturalAir = 0;
        *(int*)this = 1;
    }
    if (vu0Length_138640(mRider->mVelocity) < 50.0f)
        mRider->mVelocity = t4vect_Zero_138640;
    cWorldRider__cWorldRider(&sph, 2);
}

#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_intersectGround);
#ifdef SKIP_ASM
extern "C" void* cWorldLine_cWorldLine(void* seg, const t4Vector& a, const t4Vector& b, int n, float r);
extern "C" float cOverlapList_terrainIntersect1(void* world, void* seg, void* hit, tIntHint* hint);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_138960(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_138960(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0Add_138960(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}


/**
 * Probes the ground under the wiped-out rider along @p dir and records the patch it hits.
 *
 * Builds a capsule (cWorldLine_cWorldLine, radius 0.575) from mPosition - 200 x @p dir to mPosition + 200 x @p dir and tests it
 * against the rider's mOverlapList with mLineHint (cOverlapList_terrainIntersect1). On a hit with a patch object it stores the patch's
 * resource id (m_RID) in mGroundPolyId, the hit's u/v in PatchUV and the patch's m_sSurfaceFlags in SurfaceFlags
 * (else mGroundPolyId = -1), and calls the hit object's owner's updateIntersectInfo (virtual slot 42) with the hit.
 *
 * @param hit  receives the intersection
 * @param dir  the probe direction (the ground normal)
 * @return the hit distance, negative on a miss
 */
float cWipeoutMotion::intersectGround(tIntersectInfo* hit, t4Vector* dir)
{
    cWorldLine seg;
    t4Vector* pt = &mRider->mPosition;
    cWorldLine_cWorldLine(&seg, vu0Sub_138960(*pt, vu0Scale_138960(*dir, 200.0f)),
            vu0Add_138960(*pt, vu0Scale_138960(*dir, 200.0f)), 2, 0.574999988079071f);
    cRider* probeRider = mRider;
    float hitDist = cOverlapList_terrainIntersect1(probeRider->mOverlapList, &seg, hit, probeRider->mLineHint);
    if (hitDist >= 0.0f)
    {
        tPatchRecord* patch = hit->Patch;
        if (patch)
        {
            mRider->mGroundPolyId = patch->m_RID;
            mRider->PatchUV.x = hit->u;
            mRider->PatchUV.y = hit->v;
            mRider->SurfaceFlags = hit->Patch->m_sSurfaceFlags;
        }
        else
        {
            mRider->mGroundPolyId = 0xFFFFFFFF;
        }
        tInstanceRecord* hitObj = hit->Inst;
        if (hitObj)
        {
            cInstanceNode* hitOwner = hitObj->m_pActiveNode;
            if (hitOwner)
            {
                hitOwner->updateIntersectInfo(*hit);
            }
        }
    }
    return hitDist;
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_writeToReplayFrame);
#ifdef SKIP_ASM

/**
 * Saves the wipeout motion's state to a replay frame.
 *
 * Writes the first 0x40 bytes (WipeoutMotionState, ForceNaturalAir, BoardVelocity, BoardAngVelocity,
 * RiderAngVelocity) through the frame's write virtual (slot 1).
 *
 * @param obj  the replay frame pointer to write to (On Tour cReplayFramePtr*)
 */
void cWipeoutMotion::writeToReplayFrame(cBxReplayFrame* obj)
{
    obj->write((const char*)this, 0x40);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_readFromReplayFrame);
#ifdef SKIP_ASM

/**
 * Restores the wipeout motion's state from a replay frame.
 *
 * Reads the first 0x40 bytes (WipeoutMotionState, ForceNaturalAir, BoardVelocity, BoardAngVelocity,
 * RiderAngVelocity) through the frame's read virtual (slot 2).
 *
 * @param obj  the replay frame pointer to read from (On Tour cReplayFramePtr*)
 */
void cWipeoutMotion::readFromReplayFrame(cBxReplayFrame* obj)
{
    obj->read((char*)this, 0x40);
}
#endif

