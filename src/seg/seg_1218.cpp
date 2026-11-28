#include "vtable.h"
#include "common.h"
#include "ai/riderhandler.h"
#include "be/beinterface.h"
#include "intersect/bxnash.h"
#include "animation/rideranimbase.h"
#include "world/instancerecord.h"
#include "animation/animsequencer.h"
#include "animation/gameanimmap.h"
#include "fe/ferider.h"
#include "animation/animmodel.h"
#include "ai/focusbox.h"
#include "ai/dangerman.h"
#include "ai/rideranim.h"
#include "camera/trigger/cameratriggerman.h"
#include "sound/ssxaudio.h"
#include "ai/computer.h"
#include "ai/controlcmd.h"
#include "world/world.h"
#include "world/worldcache.h"
#include "world/worldrecords.h"
#include "replay/bxreplayframe.h"
#include "main/ssxapp.h"
#include "ai/rider.h"
#include "main/game.h"
#include "ai/ai.h"
#include "intersect/riderspheretree.h"
#include "object/instancenode.h"
#include "ai/airpredictor.h"
#include "path/aipath.h"

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_calcTilt);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vDot_0250(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector vScale_0250(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vSub_0250(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector vNorm_0250(const t4Vector& v)
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

/**
 * Returns how steeply a direction runs along the computer rider's slope, scaled by its accuracy setting.
 *
 * Takes the rider (RiderHandler.GroundMotion.mRider), removes from @p direction its component along the rider's side
 * axis (mMatrix row 0), normalizes the rest and returns the negated dot product with the rider's up axis (mMatrix row
 * 2). When mAccuracy is not 1.0 the result is multiplied by 0.12667641 and by that value. Nothing in the build
 * references it.
 *
 * @param direction  the direction to measure
 * @return the slope value
 */
float cComputer::calcTilt(const t4Vector* direction)
{
    cRider* rider = RiderHandler.GroundMotion.mRider;
    t4Vector riderSide = rider->mMatrix.row[0];
    t4Vector riderUp = *(&rider->mMatrix.row[2]);
    t4Vector planarDir = vSub_0250(*direction, vScale_0250(riderSide, vDot_0250(*direction, riderSide)));
    float slope = -vDot_0250(vNorm_0250(planarDir), riderUp);
    float accuracy = mAccuracy;
    if (accuracy == 1.0f)
        return slope;
    return slope * 0.12667641043663025f * accuracy;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_calcTurn);
#ifdef SKIP_ASM

extern t4Vector t4vect_ZAxis_v0348 __asm__("t4vect_ZAxis");
extern "C" float func_0031C228(float);

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector Cross_0348(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_0348(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_0348(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Norm_0348(const t4Vector& v)
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
static inline float Dot_0348(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_0348(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Atan2_0348(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            r += 3.1415927410125732f;
        else
            r -= 3.1415927410125732f;
    }
    return r;
}

/**
 * Returns the steering amount that turns the computer rider towards a point.
 *
 * The up axis is the rider's mMatrix row 2 (t4vect_ZAxis when its z is not positive), the forward axis is the rider's
 * velocity (cBeing::getVelocity) and the side axis is their cross product. The direction from the rider's position
 * (cBeing::getPosition) to @p target is flattened (z = 0), normalized, made perpendicular to the up axis and normalized
 * again; its angle is atan2 of its side and forward parts (func_0031C228 for the arctangent). The result is |angle| *
 * 6.2897024: 0 below 0.02, at most 0.98, with the angle's sign. The cruise routines multiply it by 31 for the command's
 * turn field.
 *
 * @param target  the point to steer towards
 * @return the turn amount, -0.98 to 0.98
 */
float cComputer::calcTurn(t4Vector* target)
{
    t4Vector up = *(&RiderHandler.GroundMotion.mRider->mMatrix.row[2]);
    if (up.z <= 0.0f)
        up = t4vect_ZAxis_v0348;
    t4Vector fwd = *RiderHandler.GroundMotion.mRider->getVelocity();
    t4Vector side = Cross_0348(fwd, up);
    t4Vector toTarget = Sub_0348(*target, *RiderHandler.GroundMotion.mRider->getPosition());
    toTarget.z = 0.0f;
    toTarget = Norm_0348(toTarget);
    toTarget = Norm_0348(Sub_0348(toTarget, Scale_0348(up, Dot_0348(toTarget, up))));
    float x = Dot_0348(toTarget, fwd);
    float y = Dot_0348(toTarget, side);
    float turnAngle = Atan2_0348(y, x);
    float turnAmount = Abs_0348(turnAngle) * 6.289702415466309f;
    if (turnAmount < 0.019999999552965164f) {
        turnAmount = 0.0f;
    } else {
        if (turnAmount > 0.9800000190734863f)
            turnAmount = 0.9800000190734863f;
        if (turnAngle < 0.0f)
            turnAmount = -turnAmount;
    }
    return turnAmount;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_calculateTargetPos);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_0610(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_0610(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_0610(const t4Vector& v, float s)
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


// PORT: the unit later declares cComputer_calculateTargetPos with the out-pointer spelled out; same symbol, by-value return here
t4Vector cComputer_calculateTargetPos_0610(char* self) __asm__("cComputer_calculateTargetPos");

/**
 * Returns the point the computer rider steers towards.
 *
 * Takes the rider (RiderHandler.GroundMotion.mRider) and returns its AimTargetPos plus a tenth of the vector from its
 * mPosition to its PathPos. The
 * cruise routines hand it to cComputer_calcTurn for the turn amount.
 *
 * @return the target point
 */
t4Vector cComputer_calculateTargetPos_0610(char* vself)
{
    cComputer* self = (cComputer*)vself;
    t4Vector toTarget = Sub_0610(self->RiderHandler.GroundMotion.mRider->PathPos, self->RiderHandler.GroundMotion.mRider->mPosition);
    toTarget = Scale_0610(toTarget, 0.1f);
    return Add_0610(self->RiderHandler.GroundMotion.mRider->AimTargetPos, toTarget);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cComputer_followPathCruise);
#ifdef SKIP_ASM

// EA's cruise logic table ([O] cComputer::mCruiseLogic, member-function pointers like cruiseLogic).
extern "C" cComputer::tCruiseLogicFn cComputer_mCruiseLogic[4];

extern "C" void cComputer_calculateTargetPos_0680(t4Vector*, void*) __asm__("cComputer_calculateTargetPos");
extern "C" float cComputer_calcTurn_0680(void*, t4Vector*) __asm__("cComputer_calcTurn");
extern "C" int cComputer_checkForSpeedZone_0680(void*, float*, int*) __asm__("cComputer_checkForSpeedZone");
extern "C" int cComputer_checkForBoardPress_0680(void*, int*) __asm__("cComputer_checkForBoardPress");
extern "C" void cComputer_setupRecklessCrouch_0680(void*, void*) __asm__("cComputer_setupRecklessCrouch");
extern "C" void cComputer_setupCrouch_0680(void*, void*) __asm__("cComputer_setupCrouch");
extern "C" int cComputer_checkForJumpEvent_0680(char* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6) __asm__("cComputer_checkForJumpEvent");
extern "C" int cComputer_checkForTargets_0680(char* self, int* out) __asm__("cComputer_checkForTargets");

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_0680(const t4Vector& v)
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
 * Fills a cruise command that follows the path, and switches the computer rider's cruise logic when needed.
 *
 * Sets the command's CruiseTurn from cComputer_calcTurn towards cComputer_calculateTargetPos's point (times 31). Then,
 * in order: - cComputer_checkForSpeedZone (speed in mDesiredSpeed, zone flag): more than 138.9 over that speed sets
 * CruiseBrake to 31 when in the zone; more than 138.9 under it crouches (cComputer_setupRecklessCrouch in the zone,
 * else cComputer_setupCrouch). - otherwise cComputer_checkForBoardPress (direction flag): fills BoardPress, the
 * opposite way for the first 60 ticks (counter mBoardPressTimer, then -1) and the given way after that. - otherwise
 * clears mBoardPressTimer, sets BoostHeld when the rider's BoostPowerBar is at least 0.9, its mUberTimer is positive
 * and mRiderDifficulty is over 50, and crouches. Last, a jump event (cComputer_checkForJumpEvent) clears mTargetID and
 * selects cruise entry 0; a target found by cComputer_checkForTargets selects entry 3 when attacking, or entry 2 when
 * that rider is not human (cBeing::isHuman). The entry's pointer-to-member goes to cruiseLogic and its index to
 * mCruiseLogicID. This function is entry 1 of the table cComputer_mCruiseLogic.
 *
 * @param cruiseCmd  the cruise command to fill
 */
void cComputer::followPathCruise(tCruiseCmd* cruiseCmd)
{
    t4Vector targetPos;
    int x;
    int y;
    int below, toPipe, toRail, isSpin, isFlip;
    int z;
    cComputer_calculateTargetPos_0680(&targetPos, this);
    cruiseCmd->CruiseTurn = (int)(cComputer_calcTurn_0680(this, &targetPos) * 31.0f);
    if (cComputer_checkForSpeedZone_0680(this, &mDesiredSpeed, &x)) {
        float sp = mDesiredSpeed;
        float len = Len_0680(RiderHandler.GroundMotion.mRider->mVelocity);
        if (sp + 138.88890075683594f < len) {
            if (x)
                cruiseCmd->CruiseBrake = 0x1F;
        } else if (len < sp - 138.88890075683594f) {
            if (x)
                cComputer_setupRecklessCrouch_0680(this, cruiseCmd);
            else
                cComputer_setupCrouch_0680(this, cruiseCmd);
        }
    } else if (cComputer_checkForBoardPress_0680(this, &y)) {
        float turnSign = 1.0f;
        if (y)
            turnSign = -1.0f;
        int steerTimer = mBoardPressTimer;
        if (steerTimer == 0) {
            cruiseCmd->BoardPress = (int)(-turnSign * 31.0f);
            mBoardPressTimer = 60;
        } else if (steerTimer > 0) {
            mBoardPressTimer = steerTimer - 1;
            if (steerTimer - 1 <= 0)
                mBoardPressTimer = -1;
            cruiseCmd->BoardPress = (int)(-turnSign * 31.0f);
        } else {
            cruiseCmd->BoardPress = (int)(turnSign * 31.0f);
        }
    } else {
        mBoardPressTimer = 0;
        cRider* rider = RiderHandler.GroundMotion.mRider;
        if (rider->BoostPowerBar >= 0.8999999761581421f && rider->mUberTimer > 0.0f && mRiderDifficulty > 50.0f)
            cruiseCmd->BoostHeld = 1;
        cComputer_setupCrouch_0680(this, cruiseCmd);
    }
    // Retail reaches mDesiredSpeed and mTargetID below through the cComputerState base at +0xDF0.
    cComputerState* state = (cComputerState*)&mDesiredSpeed;
    z = 0;
    if (cComputer_checkForJumpEvent_0680((char*)this, &state->mDesiredSpeed, &below, &toPipe, &toRail, &isSpin,
            &isFlip)) {
        mTargetID = -1;
        cruiseLogic = cComputer_mCruiseLogic[0];
        mCruiseLogicID = 0;
    } else if (cComputer_checkForTargets_0680((char*)this, &z)) {
        if (z) {
            cruiseLogic = cComputer_mCruiseLogic[3];
            mCruiseLogicID = 3;
        } else if (!SSXApp->Game->AI->Riders[state->mTargetID]->isHuman()) {
            cruiseLogic = cComputer_mCruiseLogic[2];
            mCruiseLogicID = 2;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cComputer_jumpSetupCruise);
#ifdef SKIP_ASM

extern "C" int cComputer_checkForJumpEvent(cComputer* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6);
extern "C" void cComputer_setupPrewindState_09E0(char*, int, int, int, int) __asm__("cComputer_setupPrewindState");
extern "C" void cComputer_calculateTargetPos_09E0(t4Vector*, void*) __asm__("cComputer_calculateTargetPos");
extern "C" float cComputer_calcTurn_09E0(void*, t4Vector*) __asm__("cComputer_calcTurn");
extern "C" void cComputer_followPathCruise(void*, void*);
extern "C" void cComputer_setupRecklessCrouch(cComputer*, void*);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_09E0(const t4Vector& v)
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

// PORT: PS2 abs.s helper (stands in for fabsf).
static inline float vAbs_09E0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Cruise logic 0: lines the computer rider up for the jump ahead and starts the prewind once it gets there.
 *
 * Without a jump event ahead (checkForJumpEvent(), which also sets mDesiredSpeed to the approach speed) it
 * switches to followPathCruise (logic 1, cComputer_followPathCruise) and runs it. Otherwise it steers at the path target
 * (CruiseTurn). At the jump start, within 10000 of its path (PathDistXY) and with the turn under 1 (always:
 * calcTurn caps it at 0.98) it calls setupPrewindState() and sets JumpPressed and JumpHeld; else it
 * crouches recklessly when 5 km/h too slow, or sets CruiseBrake to 31 when 5 km/h too fast.
 *
 * @param cmd  the cruise command being built (On Tour tCruiseCmd)
 */
void cComputer::jumpSetupCruise(tCruiseCmd* cmd)
{
    int below, toPipe, toRail, isSpin, isFlip;
    if (!checkForJumpEvent((float*)(&mDesiredSpeed), &below, &toPipe, &toRail, &isSpin, &isFlip)) {
        cruiseLogic = cComputer_mCruiseLogic[1];
        mCruiseLogicID = 1;
        cComputer_followPathCruise(((char*)this), cmd);
        return;
    }
    t4Vector targetPos;
    cComputer_calculateTargetPos_09E0(&targetPos, ((char*)this));
    float steer = cComputer_calcTurn_09E0(((char*)this), &targetPos);
    cmd->CruiseTurn = (int)(steer * 31.0f);
    cRider* rider = RiderHandler.GroundMotion.mRider;
    float len = vLen_09E0(*(&rider->mVelocity));
    if (below && rider->PathDistXY < 10000.0f && vAbs_09E0(steer) < 1.0f) {
        cComputer_setupPrewindState_09E0(((char*)this), isSpin, isFlip, toRail, toPipe);
        cmd->JumpPressed = 1;
        cmd->JumpHeld = 1;
        return;
    }
    float sp = mDesiredSpeed;
    if (len < sp - 138.88890075683594f) {
        setupRecklessCrouch(cmd);
        return;
    }
    if (sp + 138.88890075683594f < len)
        cmd->CruiseBrake = 0x1F;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_attackTargetCruise);
#ifdef SKIP_ASM

// cComputer from mDesiredSpeed on, the way retail reads mTargetID (0x80 past &mDesiredSpeed; probably EA's
// cComputerState base, which the flat ai/computer.h does not model).
extern "C" void cComputer_calculateTargetPos_0B90(t4Vector*, void*) __asm__("cComputer_calculateTargetPos");
extern "C" float cComputer_calcTurn_0B90(void*, t4Vector*) __asm__("cComputer_calcTurn");
extern "C" void cComputer_setupRecklessCrouch_0B90(void*, void*) __asm__("cComputer_setupRecklessCrouch");
extern "C" void cComputer_setupCrouch_0B90(void*, void*) __asm__("cComputer_setupCrouch");
extern "C" int cComputer_checkForJumpEvent_0B90(char* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6) __asm__("cComputer_checkForJumpEvent");
extern "C" int cComputer_checkForTargets_0B90(char* self, int* out) __asm__("cComputer_checkForTargets");
extern "C" float func_0031C228(float);
extern "C" void cRider_checkAIPathSwitch(void*, int);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_0B90(const t4Vector& v)
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

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_0B90(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only FPU asm (float -> int -> float truncation).
static inline float Trunc_0B90(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    return t;
}

static inline float Floor_0B90(float x)
{
    float t = Trunc_0B90(x);
    if (x < t)
        t -= 1.0f;
    return t;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Norm_0B90(const t4Vector& v)
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
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_0B90(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_0B90(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

static inline sRiderPair* Rec_0B90(cComputer* self)
{
    return &self->RiderHandler.GroundMotion.mRider->mRelations[self->mTargetID];
}

/**
 * Cruise logic 3: chases the target rider (mTargetID), crouching to catch up and braking to stay with it.
 *
 * With the target more than 90 degrees off its heading it follows the path and brakes (CruiseBrake) when the target
 * is over 200 away and slower. Otherwise it steers at a point 300 ahead of the
 * target along its velocity: within 15 degrees it crouches recklessly, within 50 it crouches, beyond that it
 * crouches when slower and brakes when faster. It then switches to jumpSetupCruise (dropping the target) at a
 * jump event, to followPathCruise when checkForTargets() finds none, or to avoidTargetCruise for a new target
 * it should not attack that is not human.
 *
 * @param cmd  the cruise command being built (On Tour tCruiseCmd)
 */
void cComputer::attackTargetCruise(tCruiseCmd* cmd)
{
    int below, toPipe, toRail, isSpin, isFlip;
    int z;
    sRiderPair* rec = Rec_0B90(this);
    cAI* list = SSXApp->Game->AI;
    cComputerState* ctl = (cComputerState*)(&mDesiredSpeed);
    cRider* tgt = list->Riders[ctl->mTargetID];
    float diff = rec->mAngle - RiderHandler.GroundMotion.mRider->PathHeading;
    float ang = diff - Floor_0B90(diff * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
    float mySpd = Len_0B90(*(&RiderHandler.GroundMotion.mRider->mVelocity));
    float absAng = Abs_0B90(ang);
    // Angle thresholds: 90, 50 and 15 degrees in radians.
    float behindAng = 1.5707964897155762f;
    float wideAng = 0.8726646900177002f;
    float narrowAng = 0.2617994248867035f;
    float hisSpd = Len_0B90(*tgt->getVelocity());
    float steer;
    if (absAng > behindAng) {
        t4Vector pathPos;
        cComputer_calculateTargetPos_0B90(&pathPos, ((char*)this));
        steer = cComputer_calcTurn_0B90(((char*)this), &pathPos);
        if (Rec_0B90(this)->mDistance > 200.0f && hisSpd < mySpd)
            cmd->CruiseBrake = 0x1F;
    } else {
        t4Vector tgtDir = Norm_0B90(*tgt->getVelocity());
        t4Vector aimPos = Add_0B90(*tgt->getPosition(), Scale_0B90(tgtDir, 300.0f));
        if (absAng < wideAng) {
            if (absAng < narrowAng)
                cComputer_setupRecklessCrouch_0B90(((char*)this), cmd);
            else
                cComputer_setupCrouch_0B90(((char*)this), cmd);
        } else if (mySpd < hisSpd) {
            cComputer_setupCrouch_0B90(((char*)this), cmd);
        } else if (hisSpd < mySpd) {
            cmd->CruiseBrake = 0x1F;
        }
        steer = cComputer_calcTurn_0B90(((char*)this), &aimPos);
    }
    cmd->CruiseTurn = (int)(steer * 31.0f);
    z = 1;
    ctl = (cComputerState*)(&mDesiredSpeed);
    if (cComputer_checkForJumpEvent_0B90(((char*)this), &ctl->mDesiredSpeed, &below, &toPipe, &toRail, &isSpin,
            &isFlip)) {
        mTargetID = -1;
        cruiseLogic = cComputer_mCruiseLogic[0];
        mCruiseLogicID = 0;
    } else if (!cComputer_checkForTargets_0B90(((char*)this), &z)) {
        cruiseLogic = cComputer_mCruiseLogic[1];
        mCruiseLogicID = 1;
    } else if (!z && !list->Riders[ctl->mTargetID]->isHuman()) {
        cruiseLogic = cComputer_mCruiseLogic[2];
        mCruiseLogicID = 2;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cComputer_avoidTargetCruise);
#ifdef SKIP_ASM

// cComputer from mDesiredSpeed on, the way retail reads mTargetID (0x80 past &mDesiredSpeed; probably EA's
// cComputerState base, which the flat ai/computer.h does not model).
extern "C" void cComputer_calculateTargetPos_0F88(t4Vector*, void*) __asm__("cComputer_calculateTargetPos");
extern "C" float cComputer_calcTurn_0F88(void*, t4Vector*) __asm__("cComputer_calcTurn");
extern "C" void cComputer_setupRecklessCrouch_0F88(void*, void*) __asm__("cComputer_setupRecklessCrouch");
extern "C" void cComputer_setupCrouch_0F88(void*, void*) __asm__("cComputer_setupCrouch");
extern "C" int cComputer_checkForJumpEvent_0F88(char* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6) __asm__("cComputer_checkForJumpEvent");
extern "C" int cComputer_checkForTargets_0F88(char* self, int* out) __asm__("cComputer_checkForTargets");
extern "C" float func_0031C228(float);
extern "C" void cRider_checkAIPathSwitch(void*, int);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_0F88(const t4Vector& v)
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

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_0F88(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only FPU asm (float -> int -> float truncation).
static inline float Trunc_0F88(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    return t;
}

static inline float Floor_0F88(float x)
{
    float t = Trunc_0F88(x);
    if (x < t)
        t -= 1.0f;
    return t;
}

static inline sRiderPair* Rec_0F88(cComputer* self)
{
    return &self->RiderHandler.GroundMotion.mRider->mRelations[self->mTargetID];
}

/**
 * Cruise logic 2: keeps the computer rider out of the target rider's way (mTargetID) while following the
 * path.
 *
 * With the target more than 90 degrees off its heading it follows the path and crouches, recklessly when
 * slower than the target. Otherwise, when the target is outside atan(150 / distance) of the heading or not
 * slower, it follows the path and crouches. When it is in the way and slower, a computer target gets the same
 * (after re-picking the route with cRider_checkAIPathSwitch if they share a path), while a human target leaves the
 * steering at 0. It then switches to jumpSetupCruise at a jump event, to followPathCruise with no target, or
 * to attackTargetCruise when checkForTargets() picks a target to attack.
 *
 * @param cmd  the cruise command being built (On Tour tCruiseCmd)
 */
void cComputer::avoidTargetCruise(tCruiseCmd* cmd)
{
    t4Vector targetPos;
    int below, toPipe, toRail, isSpin, isFlip;
    int z;
    cComputerState* ctl = (cComputerState*)(&mDesiredSpeed);
    sRiderPair* rec = Rec_0F88(this);
    cRider* tgt = SSXApp->Game->AI->Riders[ctl->mTargetID];
    float diff = rec->mAngle - RiderHandler.GroundMotion.mRider->PathHeading;
    float ang = diff - Floor_0F88(diff * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
    float mySpd = Len_0F88(*(&RiderHandler.GroundMotion.mRider->mVelocity));
    float hisSpd = Len_0F88(*tgt->getVelocity());
    float steer = 0.0f;
    if (Abs_0F88(ang) > 1.5707964897155762f) {
        cComputer_calculateTargetPos_0F88(&targetPos, ((char*)this));
        steer = cComputer_calcTurn_0F88(((char*)this), &targetPos);
        if (mySpd < hisSpd)
            cComputer_setupRecklessCrouch_0F88(((char*)this), cmd);
        else
            cComputer_setupCrouch_0F88(((char*)this), cmd);
    } else {
        float targetDist = Rec_0F88(this)->mDistance;
        float lim;
        if (targetDist == 0.0f) {
            lim = 1.5707963705062866f;
        } else {
            lim = func_0031C228(150.0f / targetDist);
            if (targetDist < 0.0f)
                lim += 3.1415927410125732f;
        }
        if (Abs_0F88(ang) > lim || mySpd <= hisSpd) {
            cComputer_calculateTargetPos_0F88(&targetPos, ((char*)this));
            steer = cComputer_calcTurn_0F88(((char*)this), &targetPos);
            cComputer_setupCrouch_0F88(((char*)this), cmd);
        } else if (tgt->isComputer()) {
            if (tgt->AIPath == RiderHandler.GroundMotion.mRider->AIPath)
                cRider_checkAIPathSwitch((char*)RiderHandler.GroundMotion.mRider, 0);
            cComputer_calculateTargetPos_0F88(&targetPos, ((char*)this));
            steer = cComputer_calcTurn_0F88(((char*)this), &targetPos);
            cComputer_setupCrouch_0F88(((char*)this), cmd);
        }
    }
    cmd->CruiseTurn = (int)(steer * 31.0f);
    z = 0;
    if (cComputer_checkForJumpEvent_0F88(((char*)this), (float*)(&mDesiredSpeed), &below, &toPipe, &toRail, &isSpin,
            &isFlip)) {
        mTargetID = -1;
        cruiseLogic = cComputer_mCruiseLogic[0];
        mCruiseLogicID = 0;
    } else if (!cComputer_checkForTargets_0F88(((char*)this), &z)) {
        cruiseLogic = cComputer_mCruiseLogic[1];
        mCruiseLogicID = 1;
    } else if (z) {
        cruiseLogic = cComputer_mCruiseLogic[3];
        mCruiseLogicID = 3;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_cDangerMan);
#ifdef SKIP_ASM
extern "C" void cDangerMan_Reset(cDangerMan* self);

struct sSlotA_101310 {
    char pad[0x18];
    unsigned int id;
    sSlotA_101310() : id(0xFFFFFFFF) {}
};

struct sSlotB_101310 {
    unsigned int id;
    char pad[0xC];
    sSlotB_101310() : id(0xFFFFFFFF) {}
};

class cTable_101310 {
public:
    void* owner;
    sSlotA_101310 a[64];
    char pad704[0x50];
    sSlotB_101310 b[64];
    cTable_101310(void* o);
};

/**
 * Constructs the AI's danger manager: stores the owning cAI, marks every slot unused and resets it.
 *
 * The member constructors set the instance ID of each of the 64 tracked elements (+0x18 of each) and of the
 * 64 danger-zone nodes to -1 before cDangerMan_Reset() runs.
 *
 * @param o  the owning cAI (On Tour pAI), whose riders cDangerMan_Update (On Tour Update) checks
 */
cTable_101310::cTable_101310(void* o)
{
    owner = o;
    ((cDangerMan*)(this))->Reset();
}
// PORT: the compiler names this ctor __13cTable_101310Pv; expose it under the address-derived name.
__asm__(".globl cDangerMan_cDangerMan\n.type cDangerMan_cDangerMan,@function\ncDangerMan_cDangerMan = __13cTable_101310Pv\n.size cDangerMan_cDangerMan,0x94\n");
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_Update);
#ifdef SKIP_ASM
// FAKEMATCH: `do { ... } while (0)` around the recs loop (its loop notes weight self's ref, so global-alloc takes self before zoneCheckAIIndex); plain form 99.31%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.

struct sXform_13A8 {
    char pad[0x30];
    t4Vector pos;
};

class cComp_13A8 {
public:
    int f0;
    int f4;
    int f8;
    virtual void v0();
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
    virtual int IsDead();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual sXform_13A8* GetXform();
};

struct sObj_13A8 {
    int pad[3];
    cComp_13A8* comp;
};

struct sBank_13A8 {
    char pad[0x1C];
    unsigned int* handles;
};

struct sWorld_13A8 {
    int pad[2];
    sBank_13A8** banks;
};


// Entry of cRider's primary vtable (_vt$6cRider, at self->mVtbl): g++ 2.95 {delta, index, pfn}.

struct sRiders_13A8 {
    char pad[0x28];
    cRider* riders[20];
    char pad78[0x78 - 0x28 - 20 * 4];
    int count;
    int getNumRiders() { return count; }
    cRider* getRider(int idx) { return riders[idx]; }
};

struct sNode_13A8 {
    unsigned int id;
    int val;
    int pad8;
    sNode_13A8* next;
};




void cDangerMan_DeleteTrackedDangerElemRecord(void*, void*);
extern "C" int cDangerMan_CalcAppliedDangerLevel(cDangerMan*, int);

static inline sObj_13A8* toPtr_13A8(unsigned int v)
{
    return (sObj_13A8*)(v << 2); // PORT: packed pointer in a 32-bit handle
}

static inline sObj_13A8* lookup_13A8(unsigned int id)
{
    sBank_13A8* b = ((sWorld_13A8*)cWorld_m_pTheWorld->m_pWorldCache)->banks[id & 0xFF];
    if (b) {
        unsigned int v = b->handles[id >> 8] >> 8;
        if (v)
            return toPtr_13A8(v);
    }
    return 0;
}

/**
 * Updates the danger tracking: one rider's danger zones, the per-rider countdowns and the tracked danger elements.
 *
 * When danger zones exist (list +0xB58), tests the rider whose index is at +0xB5C (0 past the rider count) against each
 * zone's instance (the rider's virtual isIntersecting). The highest zone level found goes into that rider's record
 * (+0x70C, 12 bytes per rider) with a 60-tick countdown, then the record's applied level is recomputed
 * (cDangerMan_CalcAppliedDangerLevel). Each of the 6 records with a level counts down and is cleared at 0. Then each
 * used element of the 64 tracked ones (+4, 0x1C bytes, keDangerElemState_Dead = free): while dying its delay (+4) drops
 * by 1/60 and it is deleted (cDangerMan_DeleteTrackedDangerElemRecord) at 0; when its instance is gone or dead it
 * starts dying (keDangerElemState_Dying), otherwise its position is copied from the instance. It ends with an empty
 * loop over the riders. cAI_update calls it.
 */
// cDangerMan::Update(void) (SSX On Tour symbols; riders are cAI::getRider/getNumRiders, the zone lookup is
// cWorld::getTheWorld()->getInstanceResource(id)).
extern "C" void cDangerMan_Update(cDangerMan* self)
{
    if (((sNode_13A8*)self->mpActiveList)) {
        int zoneCheckAIIndex = self->mRiderIndexDangerZoneCheck;
        if (!(zoneCheckAIIndex < ((sRiders_13A8*)self->mpAI)->getNumRiders()))
            zoneCheckAIIndex = 0;
        cRider* pRider = ((sRiders_13A8*)self->mpAI)->getRider(zoneCheckAIIndex);
        char* sub = (char*)&VPTR_AT(pRider, sizeof(cRiderState));
        tRiderDangerTrack* pCurrentRiderTrack = &((&self->mRiderDangerTrack[0]))[zoneCheckAIIndex];
        sNode_13A8* pCurrDangerZone = ((sNode_13A8*)self->mpActiveList);
        int baseDangerZoneLevel = 0;
        do {
            sVEntry<int(void*, sObj_13A8*)>* vt = *(sVEntry<int(void*, sObj_13A8*)>**)&VPTR_AT(pRider, sizeof(cRiderState));
            int (**pf)(void*, sObj_13A8*) = &vt[kRiderVt_isIntersecting].fn;
            if ((*pf)(vthis(sub, vt[kRiderVt_isIntersecting]), lookup_13A8(pCurrDangerZone->id))) {
                if (baseDangerZoneLevel < pCurrDangerZone->val)
                    baseDangerZoneLevel = pCurrDangerZone->val;
            }
            pCurrDangerZone = pCurrDangerZone->next;
        } while (pCurrDangerZone);
        if (baseDangerZoneLevel) {
            pCurrentRiderTrack->baseLevel = baseDangerZoneLevel;
            pCurrentRiderTrack->expiryTicksLeft = 60;
        }
        pCurrentRiderTrack->appliedLevel = cDangerMan_CalcAppliedDangerLevel(self, zoneCheckAIIndex);
    }
    do { // FAKEMATCH: see header
    for (int i = 0; i < 6; i++) {
        tRiderDangerTrack* riderTrack = &((&self->mRiderDangerTrack[0]))[i];
        if (riderTrack->baseLevel) {
            if (--riderTrack->expiryTicksLeft == 0) {
                riderTrack->baseLevel = 0;
                riderTrack->appliedLevel = 0;
            }
        }
    }
    } while (0);
    if (self->mTrackedDangerElemNum) {
        int numTrackedProcessed = self->mTrackedDangerElemNum;
        for (int currElem = 0; currElem < 64; currElem++) {
            if (!numTrackedProcessed)
                break;
            tDangerElemInfo* pCurrElem = &((&self->mTrackedDangerElems[0]))[currElem];
            if (pCurrElem->state == cDangerMan::keDangerElemState_Dead)
                continue;
            numTrackedProcessed--;
            if (pCurrElem->state == cDangerMan::keDangerElemState_Dying) {
                pCurrElem->endDelaySecsLeft -= 0.01666666753590107f;
                if (pCurrElem->endDelaySecsLeft <= 0.0f) {
                    cDangerMan_DeleteTrackedDangerElemRecord(self, pCurrElem);
                    continue;
                }
            }
            cComp_13A8* pInstNode = lookup_13A8(pCurrElem->rID)->comp;
            if (pInstNode == 0 || pInstNode->IsDead()) {
                pCurrElem->state = cDangerMan::keDangerElemState_Dying;
            } else {
                t4Vector instPos = pInstNode->GetXform()->pos;
                pCurrElem->pos[0] = instPos.x;
                pCurrElem->pos[1] = instPos.y;
                pCurrElem->pos[2] = instPos.z;
            }
        }
    }
    for (int i = 0; i < ((sRiders_13A8*)self->mpAI)->count; i++) {
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_Reset);
#ifdef SKIP_ASM
/**
 * Empties the danger manager: frees every tracked element and danger zone and clears the riders' danger
 * levels.
 *
 * Sets the tracked count to 0 and all 64 element states to 2 (free), clears the danger level and the
 * cDangerMan_CalcAppliedDangerLevel value of each of the 6 rider records, chains the 64 zone nodes into the free list,
 * empties the active zone list, sets the zone-check rider index to 0 and clears the active avalanche count.
 */
void cDangerMan::Reset()
{
    int i;
    mTrackedDangerElemNum = 0;
    int j;
    for (j = 0; j < 64; j++)
        mTrackedDangerElems[j].state = 2;
    for (i = 0; i < 6; i++) {
        mRiderDangerTrack[i].baseLevel = 0;
        mRiderDangerTrack[i].appliedLevel = 0;
    }
    for (i = 0; i < 63; i++)
        mDangerZoneTrackHeap[i].pNext = &mDangerZoneTrackHeap[i + 1];
    mpInactiveList = mDangerZoneTrackHeap;
    mDangerZoneTrackHeap[63].pNext = 0;
    mpActiveList = 0;
    mRiderIndexDangerZoneCheck = 0;
    mNumActiveAvalanche = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_AddDangerElem);
#ifdef SKIP_ASM
extern "C" void cDangerMan_RemoveDangerElem(cDangerMan*, unsigned int*);

// The world instance with resource id @p id (On Tour cWorld::getInstanceResource, inlined): the instance table (cache
// type 3) of the section in the id's low byte holds the record's address / 4 at index id >> 8; 0 when not loaded.
static inline tInstanceRecord* GetInstanceResource(unsigned int id)
{
    cWorldCacheTable* section = cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable[id & 0xFF];
    if (section) {
        unsigned int memLoc = section->m_pCacheTable[3].m_pDataTag[id >> 8].m_bfuiMemLoc;
        if (memLoc)
            return (tInstanceRecord*)(memLoc << 2); // PORT: packed pointer in a 32-bit handle
    }
    return 0;
}

/**
 * Starts tracking a world instance as a danger element.
 *
 * Does nothing when the instance (looked up by id in the world cache) is missing or dead. Otherwise drops any element
 * with the same id (cDangerMan_RemoveDangerElem), takes the first free element (keDangerElemState_Dead) of the 64 and
 * fills it: keDangerElemState_Active, the id,
 * @p x and @p y, and the instance's position; the element count +0x704 goes up. Nothing happens when all are in use.
 * DangerStartFunc calls it.
 *
 * @param idp  the instance id
 * @param x  stored in the element's first word
 * @param y  how long, in seconds, the element stays after its instance is gone (cDangerMan_Update counts it down)
 */
void cDangerMan::AddDangerElem(unsigned int* idp, float x, float y)
{
    cInstanceNode* instNode = GetInstanceResource(*idp)->m_pActiveNode;
    if (instNode == 0)
        return;
    if (instNode->isComplete())
        return;
    RemoveDangerElem(idp);
    for (int i = 0; i < 64; i++) {
        tDangerElemInfo* elemInfo = &((&mTrackedDangerElems[0]))[i];
        if (elemInfo->state == keDangerElemState_Dead) {
            elemInfo->state = keDangerElemState_Active;
            elemInfo->rID = *idp;
            elemInfo->radius = x;
            elemInfo->endDelaySecsLeft = y;
            t4Vector instPos = instNode->getRootTransform()->row[3];
            elemInfo->pos[0] = instPos.x;
            elemInfo->pos[1] = instPos.y;
            elemInfo->pos[2] = instPos.z;
            mTrackedDangerElemNum++;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_RemoveDangerElem);
#ifdef SKIP_ASM
void cDangerMan_DeleteTrackedDangerElemRecord(void*, void*);

/**
 * Stops tracking the danger element of an instance.
 *
 * Walks the used elements of the 64 and deletes the first one with the id (cDangerMan_DeleteTrackedDangerElemRecord).
 * cDangerMan_AddDangerElem and DangerEndFunc call it.
 *
 * @param idp  the instance id
 */
void cDangerMan::RemoveDangerElem(unsigned int* idp)
{
    int n = mTrackedDangerElemNum;
    if (n != 0) {
        int remaining = n;
        for (int i = 0; i < 64 && remaining != 0; i++) {
            tDangerElemInfo* elemInfo = &((&mTrackedDangerElems[0]))[i];
            if (elemInfo->state != keDangerElemState_Dead) {
                remaining--;
                if (elemInfo->rID == *idp) {
                    cDangerMan_DeleteTrackedDangerElemRecord((this), elemInfo);
                    return;
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_AddDangerVolume);
#ifdef SKIP_ASM
struct P_1900 { char pad[0xB54]; tDangerZoneTrack *freelist; tDangerZoneTrack *used; };

/**
 * Adds a danger zone for an instance, or one more reference to it.
 *
 * When the zone list (+0xB58) already holds the id, its reference count goes up. Otherwise a node is taken from the
 * free list (+0xB54; there is no check for an empty list) and put at the front of the zone list with the id, @p level
 * and one reference. cDangerMan_Update tests riders against these zones. EnableDangerZoneFunc calls it.
 *
 * @param instId  the zone's instance id
 * @param level  the zone's danger level
 */
void cDangerMan::AddDangerVolume(int *instId, int level)
{
    tDangerZoneTrack *currActiveZone;
    tDangerZoneTrack *n;

    for (currActiveZone = mpActiveList; currActiveZone != 0; currActiveZone = currActiveZone->pNext) {
        if (((int)currActiveZone->rID) == *instId) {
            currActiveZone->refCount++;
            return;
        }
    }
    n = mpInactiveList;
    mpInactiveList = (tDangerZoneTrack*)n->pNext;
    n->rID = *instId;
    n->level = level;
    n->refCount = 1;
    n->pNext = mpActiveList;
    mpActiveList = (tDangerZoneTrack*)n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_RemoveDangerVolume);
#ifdef SKIP_ASM
/**
 * Drops one reference to an instance's danger zone, and frees the zone at zero.
 *
 * The freed node goes back to the free list (mpInactiveList). When no zone is left, the zone level and applied level
 * of all 6 rider records are cleared. DisableDangerZoneFunc calls it.
 *
 * @param key  the zone's instance id
 */
void cDangerMan::RemoveDangerVolume(int* key)
{
    tDangerZoneTrack* prev = 0;
    tDangerZoneTrack* n = mpActiveList;
    while (n) {
        if (((int)n->rID) == *key) {
            if (--n->refCount == 0) {
                if (prev) prev->pNext = n->pNext;
                else mpActiveList = n->pNext;
                n->pNext = mpInactiveList;
                mpInactiveList = n;
                if (mpActiveList == 0) {
                    for (int k = 0; k < 6; k++) {
                        mRiderDangerTrack[k].baseLevel = 0;
                        mRiderDangerTrack[k].appliedLevel = 0;
                    }
                }
            }
            return;
        }
        prev = n;
        n = n->pNext;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_GetRiderDangerLevel);
#ifdef SKIP_ASM
/**
 * Returns a rider's applied danger level (the second word of its 12-byte record at +0x70C, set by cDangerMan_Update).
 *
 * cRider_updateDangerLevel calls it with the rider's mAIIndex.
 *
 * @param aiIndex  the rider's index
 * @return the applied danger level
 */
int cDangerMan::GetRiderDangerLevel(int aiIndex)
{
    return mRiderDangerTrack[aiIndex].appliedLevel;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", FocusQuery_addcell);
#ifdef SKIP_ASM



extern int cWScriptMan_TheWScriptMan;
extern "C" int cWScriptMan_isFocusInstance(int, nash_Node*);

/**
 * Adds a world cell's focus instances to a list, and optionally those of every cell below it.
 *
 * Each instance in the cell's instance list (nodesinside[0]) that cWScriptMan_isFocusInstance accepts is appended to
 * @p list (instances[numinstances++]; no bound check). With @p recurse the same is done for each of the eight
 * children, recursively.
 * The cell visitor FocusCallback_operatorCall and cFocusMan_update call it.
 *
 * @param node  the cell (eight child pointers, then the instance list)
 * @param recurse  nonzero to take the instances of all cells below too
 */
void FocusQuery::addcell(nash_Cell* node, int recurse)
{
    nash_Node* item;
    for (item = node->nodesinside[0].head; item; item = item->next) {
        if (cWScriptMan_isFocusInstance(cWScriptMan_TheWScriptMan, item))
            instances[numinstances++] = (tInstanceRecord*)item; // the link is its m_iIntersect (offset 0)
    }
    if (recurse) {
        if (node->children[0][0][0]) addcell(node->children[0][0][0], 1);
        if (node->children[0][0][1]) addcell(node->children[0][0][1], 1);
        if (node->children[0][1][0]) addcell(node->children[0][1][0], 1);
        if (node->children[0][1][1]) addcell(node->children[0][1][1], 1);
        if (node->children[1][0][0]) addcell(node->children[1][0][0], 1);
        if (node->children[1][0][1]) addcell(node->children[1][0][1], 1);
        if (node->children[1][1][0]) addcell(node->children[1][1][0], 1);
        if (node->children[1][1][1]) addcell(node->children[1][1][1], 1);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_1B60(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector Add_1B60(const t4Vector& a, const t4Vector& b)
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
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_1B60(const t4Vector& v)
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
extern "C" int AITick();
struct sNashTree;
struct sGame_1B60 {
    char pad_0x0[0xC];
    void* race;                 // 0xC
    char pad_0x10[0x10];
    sNashTree* nash;            // 0x20: the world's nash cell tree
};
// The game singleton (SSXApp).
struct sApp_1B60 {
    char pad_0x0[0x84];
    sGame_1B60* game;           // 0x84
};
extern sApp_1B60* SSXApp_1B60 __asm__("SSXApp");
// ai/focusbox.cpp: the focus box update (prototype asserts: NumFocusInstances, numfocusinstances <
// arraysize(focusinstances), numunfocusinstances < arraysize(unfocusinstances)).
// nash::CellKey: a cell's level (log2 of its size) and integer coordinates.
struct sNashKey {
    int level;
    int x;
    int y;
    int z;
};

// One root of the nash cell tree: its key and its cell (nash::Cell = sNode_1A28: eight children, then the
// instance list).
struct sNashRoot {
    sNashKey key;
    nash_Cell* cell;
};

struct sNashTree {
    sNashRoot roots[8];
};

// An axis-aligned focus box (the empty ctor leaves the dead `li 5; li -1` of g++'s array-ctor loop).
struct sFocusBox {
    t4Vector min;
    t4Vector max;
    sFocusBox() {}
};

// An instance record in a cell's list (tInstanceRecord): 0x100 = in focus, 0x200 = the focus pass parity.
struct tFocusRec {
    tFocusRec* next;
    int f4;
    unsigned int flags;
};

// The instances FocusQuery_addcell collects.
struct sFocusList {
    int n;
    tFocusRec* items[512];
};

// One focus instance: the position it follows and that position at the last update.
struct sFocusInst {
    const t4Vector* pos;
    int pad[3];
    t4Vector last;
};

struct sFocusBoxes {
    int NumFocusInstances;              // 0x0
    int pad4[3];
    sFocusInst inst[6];                 // 0x10
    int lastTime;                       // 0xD0
    int parity;                         // 0xD4
    int numInstances;                   // 0xD8
    tFocusRec* instances[512];          // 0xDC
};

struct InstanceCompare {
};

extern t4Vector kRiderBoxOffset;
extern "C" void FocusQuery_addcell_1B60(sFocusList* list, const nash_Cell* node, int recurse) __asm__("FocusQuery_addcell");
extern "C" void MEM_copy(void* dst, void* src, int size);
extern "C" void mergesort(tFocusRec** first, tFocusRec** last, tFocusRec** out, const InstanceCompare& cmp);
extern "C" void cWScriptMan_downFocusScript(int, tFocusRec*);
extern "C" void cWScriptMan_upFocusScript(int, tFocusRec*);

struct FocusCallbackInline;
// Retail's out-of-line copy of FocusCallback::operator() (matched in ai/control/handplantcontrol); the
// recursion inside the inline operator goes to it.
extern "C" void FocusCallback_operatorCall_1B60(const FocusCallbackInline* cb, const nash_Cell* cell, const sNashKey* key) __asm__("FocusCallback_operatorCall");

// FocusCallback (ai/focusbox.h) as cFocusMan_update inlines it: collects the instances of every cell that touches
// one of the focus boxes. A local copy because its inline operator() and members use this unit's views of the
// focus list and cell tree; the header's FocusCallback types the out-of-line copy (ai/control/handplantcontrol).
struct FocusCallbackInline {
    sFocusList* list;
    sFocusBox* boxes;
    int count;

    void operator()(const nash_Cell& cell, const sNashKey& key) const
    {
        int i = 0;
        for (;;) {
            if (i == count) {
                return;
            }
            union { int i; float f; } u;
            u.i = (key.level + 0x7F) << 23;
            float s = u.f;
            int ok = 0;
            if (((float)key.x - 0.2f) * s <= boxes[i].max.x && ((float)key.y - 0.2f) * s <= boxes[i].max.y && ((float)key.z - 0.2f) * s <= boxes[i].max.z
                && boxes[i].min.x <= ((float)(key.x + 1) + 0.2f) * s && boxes[i].min.y <= ((float)(key.y + 1) + 0.2f) * s && boxes[i].min.z <= ((float)(key.z + 1) + 0.2f) * s) {
                ok = 1;
            }
            if (ok) {
                goto found;
            }
            i++;
        }
    found:
        int leaf = key.level < 14;
        FocusQuery_addcell_1B60(list, &cell, leaf);
        if (leaf) {
            return;
        }
        if (key.level == 11) {
            return;
        }
        sNashKey k;
        k.level = key.level - 1;
        k.x = key.x * 2;
        k.y = key.y * 2;
        k.z = key.z * 2;
        if (((nash_Cell**)&cell.children[0][0][0])[0]) FocusCallback_operatorCall_1B60(this, ((nash_Cell**)&cell.children[0][0][0])[0], &k);
        k.x++;
        if (((nash_Cell**)&cell.children[0][0][0])[4]) FocusCallback_operatorCall_1B60(this, ((nash_Cell**)&cell.children[0][0][0])[4], &k);
        k.y++;
        if (((nash_Cell**)&cell.children[0][0][0])[6]) FocusCallback_operatorCall_1B60(this, ((nash_Cell**)&cell.children[0][0][0])[6], &k);
        k.x--;
        if (((nash_Cell**)&cell.children[0][0][0])[2]) FocusCallback_operatorCall_1B60(this, ((nash_Cell**)&cell.children[0][0][0])[2], &k);
        k.z++;
        if (((nash_Cell**)&cell.children[0][0][0])[3]) FocusCallback_operatorCall_1B60(this, ((nash_Cell**)&cell.children[0][0][0])[3], &k);
        k.x++;
        if (((nash_Cell**)&cell.children[0][0][0])[7]) FocusCallback_operatorCall_1B60(this, ((nash_Cell**)&cell.children[0][0][0])[7], &k);
        k.y--;
        if (((nash_Cell**)&cell.children[0][0][0])[5]) FocusCallback_operatorCall_1B60(this, ((nash_Cell**)&cell.children[0][0][0])[5], &k);
        k.x--;
        if (((nash_Cell**)&cell.children[0][0][0])[1]) FocusCallback_operatorCall_1B60(this, ((nash_Cell**)&cell.children[0][0][0])[1], &k);
    }
};

// Visit root k of the tree (same shape as intersect/worldsphtree's visit helpers: the root's address is
// taken before the test).
static inline void Visit(const FocusCallbackInline& cb, const sNashTree* tree, int k)
{
    const sNashRoot* r = &tree->roots[k];
    if (tree->roots[k].cell) {
        cb(*r->cell, r->key);
    }
}

/**
 * Updates which world instances are in focus around the focus positions, and runs their focus scripts.
 *
 * Builds a box of plus and minus kRiderBoxOffset around each focus position (count +0, entries of 0x20 bytes at +0x10). It
 * returns early unless a position moved 2000 or more since the last pass, 20 AI ticks (AITick) have passed, or there
 * was no pass yet (+0xD0 negative). Then it walks the eight roots of the game's nash cell tree (game +0x20): cells
 * crossing a box add their instances (FocusQuery_addcell), whole subtrees once below level 14. It stores the tick and the
 * positions. Instances newly in focus get flag 0x100; flag 0x200 follows the pass parity (+0xD4), and instances of the
 * previous pass (+0xD8 count, +0xDC list) not seen again lose 0x100. The new list replaces the old one (MEM_copy). The
 * instances that left focus are sorted (mergesort) and get cWScriptMan_downFocusScript, then those that entered get
 * cWScriptMan_upFocusScript. cAI_update calls it.
 */
extern "C" void cFocusMan_update(sFocusBoxes* self)
{
    sFocusList list;
    list.n = 0;
    int moved = 0;
    sFocusBox boxes[6];
    for (int i = 0; i < self->NumFocusInstances; i++) {
        t4Vector focusPos = *self->inst[i].pos;
        boxes[i].min = Sub_1B60(focusPos, kRiderBoxOffset);
        boxes[i].max = Add_1B60(focusPos, kRiderBoxOffset);
        if (Len_1B60(Sub_1B60(self->inst[i].last, focusPos)) >= 2000.0f) {
            moved = 1;
        }
    }
    if (!moved && self->lastTime >= 0 && AITick() - self->lastTime < 20) {
        return;
    }
    if (self->NumFocusInstances != 0) {
        sNashTree* tree = SSXApp_1B60->game->nash;
        FocusCallbackInline cb;
        cb.list = &list;
        cb.boxes = boxes;
        cb.count = self->NumFocusInstances;
        Visit(cb, tree, 0);
        Visit(cb, tree, 1);
        Visit(cb, tree, 2);
        Visit(cb, tree, 3);
        Visit(cb, tree, 4);
        Visit(cb, tree, 5);
        Visit(cb, tree, 6);
        Visit(cb, tree, 7);
    }
    self->lastTime = AITick();
    for (int i = 0; i < self->NumFocusInstances; i++) {
        t4Vector focusPos = *self->inst[i].pos;
        self->inst[i].last = focusPos;
    }
    tFocusRec* focusinstances[256];
    tFocusRec* unfocusinstances[256];
    int numfocusinstances = 0;
    int numunfocusinstances = 0;
    self->parity ^= 1;
    for (int i = 0; i < list.n; i++) {
        tFocusRec* focusRec = list.items[i];
        if (!(focusRec->flags & 0x100)) {
            focusinstances[numfocusinstances++] = focusRec;
            focusRec->flags |= 0x100;
        }
        if (self->parity) {
            focusRec->flags &= ~0x200;
        } else {
            focusRec->flags |= 0x200;
        }
    }
    for (int i = 0; i < self->numInstances; i++) {
        tFocusRec* focusRec = self->instances[i];
        if (((focusRec->flags & 0x200) == 0) != self->parity) {
            unfocusinstances[numunfocusinstances++] = focusRec;
            focusRec->flags &= ~0x100;
        }
    }
    self->numInstances = list.n;
    MEM_copy(self->instances, list.items, list.n << 2);
    tFocusRec* sorted[256];
    if (numunfocusinstances) {
        mergesort(unfocusinstances, unfocusinstances + numunfocusinstances, sorted, InstanceCompare());
        for (int i = 0; i < numunfocusinstances; i++) {
            cWScriptMan_downFocusScript(cWScriptMan_TheWScriptMan, sorted[i]);
        }
    }
    if (numfocusinstances) {
        mergesort(focusinstances, focusinstances + numfocusinstances, sorted, InstanceCompare());
        for (int i = 0; i < numfocusinstances; i++) {
            cWScriptMan_upFocusScript(cWScriptMan_TheWScriptMan, sorted[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_addfocusinstance);
#ifdef SKIP_ASM
/**
 * Puts an instance into the focus manager's in-focus list.
 *
 * Sets the in-focus flag 0x100 in the instance's m_iFlags and sets its parity flag 0x200 to match the manager's pass
 * parity (IsEven: cleared when that is nonzero, set when it is 0), as cFocusMan_update does, then appends it to
 * FocusInstances (NumFocusInstances++; no bound check). cObjectInterface_addToFocusBox uses it.
 *
 * @param instance  the instance record
 */
void cFocusMan::addfocusinstance(tInstanceRecord* instance)
{
    int oldFlags;
    int inFocusFlags;
    int newFlags;
    int n;

    oldFlags = instance->m_iFlags;
    inFocusFlags = oldFlags | 0x100;
    instance->m_iFlags = inFocusFlags;
    if (IsEven != 0) {
        newFlags = inFocusFlags & 0xFFFFFDFF;
    } else {
        newFlags = oldFlags | 0x300;
    }
    instance->m_iFlags = newFlags;
    n = NumFocusInstances;
    FocusInstances[n] = instance;
    NumFocusInstances = n + 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_purgesection);
#ifdef SKIP_ASM
struct S_3308 { char pad[0xD8]; int cnt; void *arr[8]; };
/**
 * Drops every focus instance that belongs to a world section being unloaded.
 *
 * Compacts FocusInstances (the instance-record pointers at +0xDC; NumFocusInstances at +0xD8) in place, keeping only
 * the records whose section byte (+0x78) differs from @p sectionId, and stores the new count. Called by
 * cGame_sectionUnloading.
 *
 * @param sectionId  the id of the section being unloaded, compared with each record's byte at +0x78
 */
void cFocusMan::purgesection(int sectionId)
{
    int n = 0;
    int i;

    for (i = 0; i < NumFocusInstances; i++) {
        tInstanceRecord* instance = FocusInstances[i];
        if (*(unsigned char*)&instance->m_RID != sectionId) {
            FocusInstances[n] = (tInstanceRecord*)instance;
            n++;
        }
    }
    NumFocusInstances = n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_reset);
#ifdef SKIP_ASM
struct S_3358 { char pad[0xD0]; int f_d0; int pad2; int cnt; void *arr[8]; };
/**
 * Empties the focus manager's list of up to 8 objects and sets its +0xD0 word to -1.
 *
 * Clears bit 0x100 in each listed object's +0x8 flags, the mark that cFocusMan_addfocusinstance sets when it adds one.
 */
void cFocusMan::reset()
{
    int i;

    for (i = 0; i < NumFocusInstances; i++) {
        FocusInstances[i]->m_iFlags &= 0xFFFFFEFF;
    }
    NumFocusInstances = 0;
    LastUpdateTick = -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_addbox);
#ifdef SKIP_ASM

/**
 * Registers a focus box with the focus manager.
 *
 * Appends a record to Boxes (+0x10, 0x20 bytes each): the box pointer at +0x0 and a copy of the box's first 16 bytes at
 * +0x10, then NumBoxes (+0x0)++. LastUpdateTick (+0xD0) is reset to -1 so the next update recomputes the focus. There
 * is no capacity check here (On Tour asserts NumBoxes < MaxBoxes, 6 records). Callers: cRider_addFocusBox and
 * cScriptObjCameraManager_OnStartObjectUse.
 *
 * @param focusBox  the cFocusBox to add
 */
void cFocusMan::addbox(cFocusBox *focusBox)
{
    t4Vector boxCopy;

    LastUpdateTick = -1;
    Boxes[NumBoxes].Box = focusBox;
    boxCopy = focusBox->Position;
    Boxes[NumBoxes].LastUpdatePos = boxCopy;
    NumBoxes = (int) (NumBoxes + 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_removebox);
#ifdef SKIP_ASM

struct sList1033F8 {
    int count;
    char pad4[0xC];
    FocusRecord entries[6];    // 0x10
    int fD0;                    // 0xD0
};

/**
 * Unregisters a focus box, closing the gap in the box records.
 *
 * Resets LastUpdateTick (+0xD0) to -1, finds the record (Boxes, +0x10, 0x20 bytes each) whose box pointer equals @p id,
 * moves the later records down one and decrements NumBoxes (+0x0); a box that is not registered changes nothing else.
 * Callers: cRider__cRider and cScriptObjCameraManager_OnEndObjectUse.
 *
 * @param id  the cFocusBox to remove, compared with each record's box pointer
 */
// Removes the entry with the given id from the list, shifting the rest down.
void cFocusMan::removebox(cFocusBox* id)
{
    int i;
    LastUpdateTick = -1;
    for (i = 0; i < NumBoxes; i++) {
        if (((&Boxes[0]))[i].Box == id) {
            for (i++; i < NumBoxes; i++) {
                ((&Boxes[0]))[i - 1] = ((&Boxes[0]))[i];
            }
            NumBoxes--;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_writeToReplayFrame);
#ifdef SKIP_ASM
/**
 * Saves the focus manager's state into a replay frame.
 *
 * Through the frame's write it stores LastUpdateTick (4 bytes) and the 16-byte LastUpdatePos of each registered
 * record in Boxes, then NumFocusInstances; each of FocusInstances goes through writeReplayInstance. Called by
 * cReplay_saveFrame; cFocusMan_readFromReplayFrame reads it back.
 *
 * @param data  the replay frame (cReplayFramePtr) written to
 */
void cFocusMan::writeToReplayFrame(cBxReplayFrame* data)
{
    data->write((const char*)&LastUpdateTick, 4);
    for (int i = 0; i < NumBoxes; i++)
        data->write((const char*)&Boxes[i].LastUpdatePos, 0x10);
    data->write((const char*)&NumFocusInstances, 4);
    for (int j = 0; j < NumFocusInstances; j++)
        data->writeReplayInstance(FocusInstances[j]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_readFromReplayFrame);
#ifdef SKIP_ASM
/**
 * Restores the focus manager's state from a replay frame.
 *
 * Reads back what cFocusMan_writeToReplayFrame stored: LastUpdateTick, the LastUpdatePos of each registered box record,
 * NumFocusInstances and the instance records (the frame's readReplayInstance). It sets IsEven to 1
 * and marks every restored record as in focus: m_iFlags gets 0x100 set and 0x200 cleared. Called by
 * cReplay_restoreFrame.
 *
 * @param data  the replay frame (cReplayFramePtr) read from
 */
void cFocusMan::readFromReplayFrame(cBxReplayFrame* data)
{
    data->read((char*)&LastUpdateTick, 4);
    for (int i = 0; i < NumBoxes; i++)
        data->read((char*)&Boxes[i].LastUpdatePos, 0x10);
    IsEven = 1;
    data->read((char*)&NumFocusInstances, 4);
    for (int j = 0; j < NumFocusInstances; j++) {
        tInstanceRecord* it = data->readReplayInstance();
        it->m_iFlags = (it->m_iFlags | 0x100) & 0xFFFFFDFF;
        FocusInstances[j] = it;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_updateRule);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" tAIAnimPlayAttr* cAIAnimEventMap_getAnimPlayAttr(int id);
// PORT: the handlers are defined later in the unit with mixed parameter/return types; bind uniform views by asm label
extern "C" int f_001048C0_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateFWD");
extern "C" int f_00104940_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateHOLD_LAST");
extern "C" int f_001049C0_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateCYCLE");
extern "C" int f_00104110_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_CRUISE3");
extern "C" int f_00104178_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_CRUISE5");
extern "C" int f_00104238_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_RAIL");
extern "C" int f_001042A8_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_BRAKE");
extern "C" int f_00104358_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_PREWIND");
extern "C" int f_001045D8_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_GATEROCK");
extern "C" int f_00104660_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_HANDPLANT");
extern "C" int f_001042E0_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_UBERGRIND");
extern "C" int f_001043F8_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_AIR_ADJ");
extern "C" int f_001045B8_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_AIR_CYC");
extern "C" int f_001047F0_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_BP_PIVOT");
extern "C" int f_00104728_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_BP_INTO");
extern "C" int f_001046B0_36A0(void* self, void* x, void* anim, float dt) __asm__("cRiderAnim_onUpdateASYNC_BP_BAL");
extern "C" void cRiderAnim_checkForSoundEvents_36A0(void* self, void* anim) __asm__("cRiderAnim_checkForSoundEvents");

/**
 * Runs the per-frame update rule of the animation playing in one of the rider's sequencers.
 *
 * Looks up the animation's play attributes (cAIAnimEventMap_getAnimPlayAttr(*anim): a 0x1C-byte record of
 * cAIAnimEventMap_AnimPlayAttr; On Tour calls this lookup getAnimPlayAttr) and dispatches on its update rule (+0x4, a
 * tAIAnimUpdateRuleID) to one of 16 handlers, cRiderAnim_onUpdateFWD .. cRiderAnim_onUpdateASYNC_BP_BAL (On Tour
 * onUpdateFWD .. onUpdateASYNC_BP_BAL). When the handler returns 0 and Rider (+0x60) is a human (mIsHuman) local
 * (mIsLocal) rider, it then runs cRiderAnim_checkForSoundEvents (On Tour checkForSoundEvents). Virtual: slot 2 of
 * cRiderAnim_vtbl.
 *
 * @param x  the animation sequencer (cAnimSequencer) being updated
 * @param anim  the sequence (cAnimSequence); its first word is the tAIAnimID
 * @param dt  the time step in seconds
 */
void cRiderAnim::updateRule(void* x, int* anim, float dt)
{
    int done = 0;
    switch (cAIAnimEventMap_getAnimPlayAttr(*anim)->UpdateRule) {
    case kAAU_FWD:
        done = f_001048C0_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_HOLD_LAST:
        done = f_00104940_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_CYCLE:
        done = f_001049C0_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_CRUISE3:
        done = f_00104110_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_CRUISE5:
        done = f_00104178_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_RAIL:
        done = f_00104238_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_BRAKE:
        done = f_001042A8_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_PREWIND:
        done = f_00104358_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_GATEROCK:
        done = f_001045D8_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_HANDPLANT:
        done = f_00104660_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_UBERGRIND:
        done = f_001042E0_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_AIR_ADJ:
        done = f_001043F8_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_AIR_CYC:
        done = f_001045B8_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_BP_PIVOT:
        done = f_001047F0_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_BP_INTO:
        done = f_00104728_36A0(((char*)this), x, anim, dt);
        break;
    case kAAU_ASYNC_BP_BAL:
        done = f_001046B0_36A0(((char*)this), x, anim, dt);
        break;
    }
    if (done == 0) {
        cRider* rider = Rider;
        if (((char*)rider) != 0) {
            int ok = 0;
            if (rider->mIsHuman != 0) {
                ok = rider->mIsLocal != 0;
            }
            if (ok) {
                cRiderAnim_checkForSoundEvents_36A0(((char*)this), anim);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_expireRule);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" tAIAnimPlayAttr* cAIAnimEventMap_getAnimPlayAttr(int id);
extern "C" int cRiderAnim_onExpireNOP_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireNOP");
extern "C" int cRiderAnim_onExpireINTO_AIR_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireINTO_AIR");
extern "C" int cRiderAnim_onExpireINTO_SPIN_CYC_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireINTO_SPIN_CYC");
extern "C" int cRiderAnim_onExpireINTO_CRUISE_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireINTO_CRUISE");
extern "C" int cRiderAnim_onExpireBLENDTO_CRUISE_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireBLENDTO_CRUISE");
extern "C" int cRiderAnim_onExpireBLENDTO_AIR_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireBLENDTO_AIR");
extern "C" int cRiderAnim_onExpireBLENDTO_RAIL_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireBLENDTO_RAIL");
extern "C" int cRiderAnim_onExpireINTO_FL_STAND_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireINTO_FL_STAND");
extern "C" int cRiderAnim_onExpireINTO_RSFS_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireINTO_RSFS");
extern "C" int cRiderAnim_onExpireINTO_PRESS_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireINTO_PRESS");
extern "C" int cRiderAnim_onExpireBLENDTO_GEAR_CYC_3918(void* self, void* ev, void* data) __asm__("cRiderAnim_onExpireBLENDTO_GEAR_CYC");



/**
 * Runs the expire rule of the animation that finished in a sequencer and records the animation to play next.
 *
 * The sequencer index is @p ev's position in the 8-byte sequencer array at +0x50. It looks up the play attributes of
 * the sequence's animation (cAIAnimEventMap_getAnimPlayAttr(*data); On Tour getAnimPlayAttr) and dispatches on the
 * expire rule (+0x8, a tAIAnimExpireRuleID) to one of 11 handlers, cRiderAnim_onExpireNOP ..
 * cRiderAnim_onExpireBLENDTO_GEAR_CYC (On Tour onExpireNOP, onExpireINTO_AIR, ...; On Tour adds SK_ skier variants).
 * The handler's result, the next tAIAnimID, goes into the word at +index*4; a rule outside 0..10 stores nothing.
 * Virtual: slot 3 of cRiderAnim_vtbl.
 *
 * @param ev  the sequencer whose animation expired (an element of the array at +0x50)
 * @param data  the expiring sequence; its first word is the tAIAnimID
 */
// Runs the handler for an animation event's type and stores its result in the per-event slot.
void cRiderAnim::expireRule(cAnimSequencer* ev, int* data)
{
    int idx = ev - Sequencer;
    tAIAnimPlayAttr* info = cAIAnimEventMap_getAnimPlayAttr(*data);
    switch (info->ExpireRule) {
    case kAAE_NOP:
        CurrentAnimID[idx] = cRiderAnim_onExpireNOP_3918((this), ev, data);
        break;
    case kAAE_INTO_AIR:
        CurrentAnimID[idx] = cRiderAnim_onExpireINTO_AIR_3918((this), ev, data);
        break;
    case kAAE_INTO_SPIN_CYC:
        CurrentAnimID[idx] = cRiderAnim_onExpireINTO_SPIN_CYC_3918((this), ev, data);
        break;
    case kAAE_INTO_CRUISE:
        CurrentAnimID[idx] = cRiderAnim_onExpireINTO_CRUISE_3918((this), ev, data);
        break;
    case kAAE_BLENDTO_CRUISE:
        CurrentAnimID[idx] = cRiderAnim_onExpireBLENDTO_CRUISE_3918((this), ev, data);
        break;
    case kAAE_BLENDTO_AIR:
        CurrentAnimID[idx] = cRiderAnim_onExpireBLENDTO_AIR_3918((this), ev, data);
        break;
    case kAAE_BLENDTO_RAIL:
        CurrentAnimID[idx] = cRiderAnim_onExpireBLENDTO_RAIL_3918((this), ev, data);
        break;
    case kAAE_INTO_FL_STAND:
        CurrentAnimID[idx] = cRiderAnim_onExpireINTO_FL_STAND_3918((this), ev, data);
        break;
    case kAAE_INTO_RSFS:
        CurrentAnimID[idx] = cRiderAnim_onExpireINTO_RSFS_3918((this), ev, data);
        break;
    case kAAE_INTO_PRESS:
        CurrentAnimID[idx] = cRiderAnim_onExpireINTO_PRESS_3918((this), ev, data);
        break;
    case kAAE_BLENDTO_GEAR_CYC:
        CurrentAnimID[idx] = cRiderAnim_onExpireBLENDTO_GEAR_CYC_3918((this), ev, data);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_checkForSoundEvents);
#ifdef SKIP_ASM
struct sEnt_3AA0 {
    char pad[0x10];
    unsigned short first;
    unsigned short count;
};

struct sRec_3AA0 {
    unsigned short a;
    unsigned short b;
};

struct sBank_3AA0 {
    int pad[2];
    sEnt_3AA0* ents;
    int padC;
    sRec_3AA0* recs;
};


struct sBankTab_3AA0 {
    sBank_3AA0* banks[1];
};

extern sBankTab_3AA0* cAnimDataBase_TheAnimDataBase;
extern "C" int tEventFlags_isSet1(void*, int);
extern "C" int tEventFlags_isJustChanged(void*, int);
extern "C" void tEventFlags_clear1(void*, int);
extern "C" void cRiderAnim_triggerAnimSound(cRiderAnim*, int);

/**
 * Plays the sounds tagged on the frames a rider animation just passed.
 *
 * The tag count and tag words come from cAnimDataBase_TheAnimDataBase (bank = id & 0xFF, entry = id >> 8 of the
 * sequence's animation id at +4). For each tag whose event the sequence's +0xB0 object reports through tEventFlags_isSet1
 * and tEventFlags_isJustChanged, and whose tag word has bit 15 set, it clears the event (tEventFlags_clear1) and plays the sound
 * (cRiderAnim_triggerAnimSound with the low 15 bits). cRiderAnim_updateRule calls it for local human riders.
 *
 * @param sequence  the animation sequence
 */
void cRiderAnim::checkForSoundEvents(cAnimSequence* sequence)
{
    int n = cAnimDataBase_TheAnimDataBase->banks[sequence->SubAnims[0].AnimID & 0xFF]->ents[sequence->SubAnims[0].AnimID >> 8].count;
    for (int i = 0; i < n; i++) {
        int ok = 0;
        if (tEventFlags_isSet1(((char*)&sequence->Events), i) && tEventFlags_isJustChanged(((char*)&sequence->Events), i))
            ok = 1;
        if (ok) {
            sBank_3AA0* animBank = cAnimDataBase_TheAnimDataBase->banks[sequence->SubAnims[0].AnimID & 0xFF];
            unsigned short soundEvent = animBank->recs[animBank->ents[sequence->SubAnims[0].AnimID >> 8].first + i].b;
            if (soundEvent & 0x8000) {
                tEventFlags_clear1(((char*)&sequence->Events), i);
                triggerAnimSound(soundEvent & 0x7FFF);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_updatePosNegSlider);
#ifdef SKIP_ASM
// The rider-anim helpers below read the map as *(GameAnimMap + gad): the subscript GameAnimMap[gad] swaps the addu
// operands against retail.
extern cGameAnimMap* cGameAnimMap_TheGameAnimMap;
extern "C" void cAnimSequence_setSubAnim(void*, int, int);
extern "C" void cAnimSequence_setSubAnimTime(void*, int, float);
extern "C" bool cAnimSequence_updateWeight(void*, float);
extern "C" void cAnimSequencer_expire(void*, void*);
/**
 * Shows one of two animations at the point a signed weight selects, then advances the sequence's blend.
 *
 * For a negative @p weight sub-animation 0 becomes @p negAnim, otherwise @p posAnim (each mapped through GameAnimMap of
 * cGameAnimMap_TheGameAnimMap), at |weight| times its length (SubAnims[0].TotalTime). Then the blend weight
 * advances by @p dt / 60 (cAnimSequence_updateWeight); when that reports the end, the sequence is expired
 * (cAnimSequencer_expire).
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param posAnim  the game animation for positive weights
 * @param negAnim  the game animation for negative weights
 * @param dt  the time step, multiplied by 1/60
 * @param weight  the slider position, -1 to 1
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::updatePosNegSlider(void* x, cAnimSequence* anim, int posAnim, int negAnim, float dt, float weight)
{
    if (weight < 0.0f) {
        cAnimSequence_setSubAnim(anim, 0, *(cGameAnimMap_TheGameAnimMap->GameAnimMap + negAnim));
        cAnimSequence_setSubAnimTime(anim, 0, -weight * anim->SubAnims[0].TotalTime);
    } else {
        cAnimSequence_setSubAnim(anim, 0, *(cGameAnimMap_TheGameAnimMap->GameAnimMap + posAnim));
        cAnimSequence_setSubAnimTime(anim, 0, weight * anim->SubAnims[0].TotalTime);
    }
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f) != 0) {
        cAnimSequencer_expire(x, anim);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_update3Cycle);
#ifdef SKIP_ASM
extern cGameAnimMap* cGameAnimMap_TheGameAnimMap;
extern "C" void cAnimSequence_setSubAnimType(void*, int, int);
extern "C" void cAnimSequence_updateSubAnim(void*, int, float);
extern "C" void cAnimSequence_setSubAnim(void*, int, int);
extern "C" void cAnimSequence_setSubAnimTime(void*, int, float);
extern "C" void cAnimSequence_setSubAnimWeight(void*, int, float);
extern "C" bool cAnimSequence_updateWeight(void*, float);
extern "C" void cAnimSequencer_expire(void*, void*);

/**
 * Blends a centre animation cycle with its negative or positive variant by a signed weight.
 *
 * Sub-animation 0 becomes @p cenAnim; sub-animation 1 becomes @p posAnim when @p w is positive, otherwise @p negAnim
 * (and w is negated); the weights are 1 - w and w (ids mapped through GameAnimMap of
 * cGameAnimMap_TheGameAnimMap). Sub-animation 0 loops (type 1) and advances by @p dt / 60; sub-animation 1 follows at
 * the same fraction of its own length (SubAnims[1].TotalTime). Then the blend weight advances, expiring the sequence
 * at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param negAnim  the game animation for negative weights
 * @param cenAnim  the centre game animation
 * @param dt  the time step, multiplied by 1/60
 * @param w  the weight, -1 to 1
 * @param posAnim  the game animation for positive weights
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::update3Cycle(void* x, cAnimSequence* anim, int negAnim, int cenAnim, float dt, float w, int posAnim)
{
    cAnimSequence_setSubAnim(anim, 0, *(cGameAnimMap_TheGameAnimMap->GameAnimMap + cenAnim));
    if (0.0f < w) {
        cAnimSequence_setSubAnim(anim, 1, *(cGameAnimMap_TheGameAnimMap->GameAnimMap + posAnim));
    } else {
        w = -w;
        cAnimSequence_setSubAnim(anim, 1, *(cGameAnimMap_TheGameAnimMap->GameAnimMap + negAnim));
    }
    cAnimSequence_setSubAnimWeight(anim, 0, 1.0f - w);
    cAnimSequence_setSubAnimWeight(anim, 1, w);
    cAnimSequence_setSubAnimType(anim, 0, 1);
    float stepTime = dt * 0.01666666753590107f;
    cAnimSequence_updateSubAnim(anim, 0, stepTime);
    cAnimSequence_setSubAnimTime(anim, 1,
                                 anim->SubAnims[0].Time / anim->SubAnims[0].TotalTime * anim->SubAnims[1].TotalTime);
    if (cAnimSequence_updateWeight(anim, stepTime)) {
        cAnimSequencer_expire(x, anim);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_update5Cycle);
#ifdef SKIP_ASM
extern cGameAnimMap* cGameAnimMap_TheGameAnimMap;
extern "C" void cAnimSequence_setSubAnimType(void*, int, int);
extern "C" void cAnimSequence_updateSubAnim(void*, int, float);
extern "C" void cAnimSequence_setSubAnim(void*, int, int);
extern "C" void cAnimSequence_setSubAnimTime(void*, int, float);
extern "C" void cAnimSequence_setSubAnimWeight(void*, int, float);
extern "C" bool cAnimSequence_updateWeight(void*, float);
extern "C" void cAnimSequencer_expire(void*, void*);

/**
 * Blends two neighbouring animation cycles out of five, chosen by a signed weight.
 *
 * Above 0.5 it blends @p posEndAnim with @p posMidAnim (w = (weight - 0.5) * 2), above 0 @p posMidAnim with @p cenAnim
 * (w = 2 * weight), above -0.5 @p cenAnim with @p negMidAnim (w = 2 * weight + 1), otherwise @p negMidAnim with
 * @p negEndAnim (w = (weight + 1) * 2). The first goes to sub-animation 0 (replaced only when it changes, keeping its
 * time fraction), the second to sub-animation 1; the weights are w and 1 - w. Sub-animation 0 loops and advances by
 * @p dt / 60, sub-animation 1 follows at the same fraction of its length, then the blend weight advances, expiring the
 * sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param negEndAnim  the game animation at weight -1
 * @param negMidAnim  the game animation at weight -0.5
 * @param cenAnim  the game animation at weight 0
 * @param posMidAnim  the game animation at weight 0.5
 * @param posEndAnim  the game animation at weight 1
 * @param dt  the time step, multiplied by 1/60
 * @param weight  the weight, -1 to 1
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::update5Cycle(void* x, cAnimSequence* anim, int negEndAnim, int negMidAnim, int cenAnim, int posMidAnim,
                             int posEndAnim, float dt, float weight)
{
    int from, to;
    float w;
    if (weight > 0.5f) {
        from = posEndAnim;
        to = posMidAnim;
        w = (weight - 0.5f) * 2.0f;
    } else if (weight > 0.0f) {
        from = posMidAnim;
        to = cenAnim;
        w = weight * 2.0f;
    } else if (weight > -0.5f) {
        from = cenAnim;
        to = negMidAnim;
        w = weight * 2.0f + 1.0f;
    } else {
        from = negMidAnim;
        to = negEndAnim;
        w = (weight + 1.0f) * 2.0f;
    }
    if (*(cGameAnimMap_TheGameAnimMap->GameAnimMap + from) != anim->SubAnims[0].AnimID) {
        float frameFraction = anim->SubAnims[0].Time / anim->SubAnims[0].TotalTime;
        cAnimSequence_setSubAnim(anim, 0, *(cGameAnimMap_TheGameAnimMap->GameAnimMap + from));
        cAnimSequence_setSubAnimTime(anim, 0, anim->SubAnims[0].TotalTime * frameFraction);
    }
    cAnimSequence_setSubAnim(anim, 1, *(cGameAnimMap_TheGameAnimMap->GameAnimMap + to));
    cAnimSequence_setSubAnimWeight(anim, 0, w);
    cAnimSequence_setSubAnimWeight(anim, 1, 1.0f - w);
    cAnimSequence_setSubAnimType(anim, 0, 1);
    float stepTime = dt * 0.01666666753590107f;
    cAnimSequence_updateSubAnim(anim, 0, stepTime);
    cAnimSequence_setSubAnimTime(anim, 1,
                                 anim->SubAnims[0].Time / anim->SubAnims[0].TotalTime * anim->SubAnims[1].TotalTime);
    if (cAnimSequence_updateWeight(anim, stepTime)) {
        cAnimSequencer_expire(x, anim);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_updateIntoCycle);
#ifdef SKIP_ASM
extern cGameAnimMap* cGameAnimMap_TheGameAnimMap;
extern "C" void cAnimSequence_setSubAnimType(void*, int, int);
extern "C" void cAnimSequence_updateSubAnim(void*, int, float);
extern "C" void cAnimSequence_setSubAnim(void*, int, int);
extern "C" void cAnimSequence_setSubAnimTime(void*, int, float);
extern "C" bool cAnimSequence_updateWeight(void*, float);
extern "C" void cAnimSequencer_expire(void*, void*);

/**
 * Plays an animation once and then switches the sequence to a looping one.
 *
 * While sub-animation 0 is @p intoAnim (mapped through GameAnimMap of cGameAnimMap_TheGameAnimMap) it plays
 * once (type 0) by @p dt / 60, and once its time reaches its length it becomes @p cycleAnim, looping (type 1) from time
 * 0. Otherwise sub-animation 0 just advances. Then the blend weight advances, expiring the sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param intoAnim  the game animation played first
 * @param dt  the time step, multiplied by 1/60
 * @param cycleAnim  the looping game animation that follows
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::updateIntoCycle(void* x, cAnimSequence* anim, int intoAnim, float dt, int cycleAnim)
{
    if (anim->SubAnims[0].AnimID == *(cGameAnimMap_TheGameAnimMap->GameAnimMap + intoAnim)) {
        cAnimSequence_setSubAnimType(anim, 0, 0);
        cAnimSequence_updateSubAnim(anim, 0, dt * 0.01666666753590107f);
        if (anim->SubAnims[0].Time >= anim->SubAnims[0].TotalTime) {
            cAnimSequence_setSubAnim(anim, 0, *(cGameAnimMap_TheGameAnimMap->GameAnimMap + cycleAnim));
            cAnimSequence_setSubAnimType(anim, 0, 1);
            cAnimSequence_setSubAnimTime(anim, 0, 0.0f);
        }
    } else {
        cAnimSequence_updateSubAnim(anim, 0, dt * 0.01666666753590107f);
    }
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f)) {
        cAnimSequencer_expire(x, anim);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_CRUISE3);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cRiderAnim_update3Cycle_4110(void*, void*, int*, int, int, int, float, float) __asm__("cRiderAnim_update3Cycle");

/**
 * Update rule 3 of cRiderAnim_updateRule: a three-way cruise blend.
 *
 * The weight is the rider's (+0x60) +0x1FC, negated when the rider's +0x320 is set. Sequence kAID_CruiseCrouchSlow (7)
 * blends game animations 17 / 15 / 16, kAID_CruiseAirCrouch (10) 26 / 23 / 25, any other 26 / 24 / 25 (negative /
 * centre / positive) with cRiderAnim_update3Cycle.
 *
 * @param sequencer  the sequencer
 * @param sequence  the sequence (first word: its animation id)
 * @param deltaTime  the time step
 * @return cRiderAnim_update3Cycle's result: 1 when the sequence was expired
 */
int cRiderAnim::onUpdateASYNC_CRUISE3(void *sequencer, int *sequence, float deltaTime)
{
    float var_f13;
    cRider* temp_3;

    temp_3 = Rider;
    var_f13 = temp_3->mSlewed[1].cur;
    if (temp_3->IsRidingFakie != 0) {
        var_f13 = -var_f13;
    }
    if (*sequence == kAID_CruiseCrouchSlow) {
        return cRiderAnim_update3Cycle_4110(this, sequencer, sequence, 17, 0xF, 0x10, deltaTime, var_f13);
    }
    if (*sequence == kAID_CruiseAirCrouch) {
        return cRiderAnim_update3Cycle_4110(this, sequencer, sequence, 26, 23, 25, deltaTime, var_f13);
    }
    return cRiderAnim_update3Cycle_4110(this, sequencer, sequence, 26, 0x18, 25, deltaTime, var_f13);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_CRUISE5);
#ifdef SKIP_ASM
extern "C" int cRiderAnim_update5Cycle(cRiderAnim* self, void* x, cAnimSequence* anim, int i0, int i1, int i2, int i3,
                                       int i4, float dt, float v);
/**
 * Update rule 4 of cRiderAnim_updateRule: a five-way cruise blend.
 *
 * The weight is the rider's (+0x60) +0x1FC, negated when the rider's +0x320 is set. The sequence picks the five game
 * animations (negative end, negative middle, centre, positive middle, positive end): kAID_CruiseCrouch (6) gives 14 13
 * 10 11 12, kAID_CruiseFast (8) 22 21 18 19 20, kAID_CruiseIce (14) 32 31 27 29 30, kAID_CruiseCrouchIce (15) 32 31 28
 * 29 30, any other 9 8 5 6 7. They go to cRiderAnim_update5Cycle.
 *
 * @param x  the sequencer
 * @param anim  the sequence (first word: its animation id)
 * @param dt  the time step
 * @return cRiderAnim_update5Cycle's result: 1 when the sequence was expired
 */
int cRiderAnim::onUpdateASYNC_CRUISE5(void* x, int* anim, float dt)
{
    cRider* rider = Rider;
    float weight = rider->mSlewed[1].cur;
    if (rider->IsRidingFakie != 0)
        weight = -weight;
    int negEndAnim, negMidAnim, cenAnim, posMidAnim, posEndAnim;
    int animId = *anim;
    if (animId == kAID_CruiseCrouch) {
        negEndAnim = 0xE; negMidAnim = 13; cenAnim = 10; posMidAnim = 11; posEndAnim = 0xC;
    } else if (animId == kAID_CruiseFast) {
        negEndAnim = 22; negMidAnim = 21; cenAnim = 18; posMidAnim = 19; posEndAnim = 20;
    } else if (animId == kAID_CruiseIce) {
        negEndAnim = 0x20; negMidAnim = 0x1F; cenAnim = 27; posMidAnim = 29; posEndAnim = 0x1E;
    } else if (animId != kAID_CruiseCrouchIce) {
        negEndAnim = 9; negMidAnim = 8; cenAnim = 5; posMidAnim = 6; posEndAnim = 7;
    } else {
        negEndAnim = 0x20; negMidAnim = 0x1F; cenAnim = 0x1C; posMidAnim = 29; posEndAnim = 0x1E;
    }
    return update5Cycle(x, (cAnimSequence*)anim, negEndAnim, negMidAnim, cenAnim, posMidAnim,
                                   posEndAnim, dt, weight);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_RAIL);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cRiderAnim_update3Cycle_4238(void*, void*, int*, int, int, int, float, float) __asm__("cRiderAnim_update3Cycle");

/**
 * Update rule 5 of cRiderAnim_updateRule: a three-way rail blend.
 *
 * The weight is the rider's (+0x60) +0x238, negated when the rider's +0x320 is set. Sequence kAID_RSBalance (18) blends
 * game animations 65 / 63 / 64, kAID_RSFSBalance (19) 68 / 66 / 67, any other 71 / 69 / 70 (negative / centre /
 * positive) with cRiderAnim_update3Cycle. The C declares no result, but cRiderAnim_update3Cycle's is left in the return
 * register.
 *
 * @param sequencer  the sequencer
 * @param sequence  the sequence (first word: its animation id)
 * @param deltaTime  the time step
 */
void cRiderAnim::onUpdateASYNC_RAIL(void *sequencer, int *sequence, float deltaTime)
{
    int var_7;
    int var_8;
    int var_9;
    float var_f13;
    cRider* temp_3;

    temp_3 = Rider;
    var_f13 = temp_3->mSlewed[6].cur;
    if (temp_3->IsRidingFakie != 0) {
        var_f13 = -var_f13;
    }
    switch (*sequence) {
    case kAID_RSBalance:
        var_7 = 65;
        var_8 = 0x3F;
        var_9 = 0x40;
        break;
    default:
        var_7 = 71;
        var_8 = 69;
        var_9 = 70;
        break;
    case kAID_RSFSBalance:
        var_7 = 68;
        var_8 = 66;
        var_9 = 67;
        break;
    }
    cRiderAnim_update3Cycle_4238(this, sequencer, sequence, var_7, var_8, var_9, deltaTime, var_f13);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_BRAKE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cRiderAnim_update3Cycle_42A8(void*, void*, char*, int, int, int, float, float) __asm__("cRiderAnim_update3Cycle");

/**
 * Update rule 6 of cRiderAnim_updateRule: a three-way brake blend.
 *
 * The weight is the rider's (+0x60) +0x214, negated when the rider's +0x320 is 0 (the other rules negate when it is
 * set). It blends game animations 36 / 33 / 34 (negative / centre / positive) with cRiderAnim_update3Cycle. The C declares no
 * result, but cRiderAnim_update3Cycle's is left in the return register.
 *
 * @param sequencer  the sequencer
 * @param sequence  the sequence
 * @param dt  the time step
 */
void cRiderAnim::onUpdateASYNC_BRAKE(void *sequencer, char *sequence, float dt)
{
    float var_f13;
    cRider* temp_3;

    temp_3 = Rider;
    var_f13 = temp_3->mSlewed[3].cur;
    if (temp_3->IsRidingFakie == 0) {
        var_f13 = -var_f13;
    }
    cRiderAnim_update3Cycle_42A8(this, sequencer, sequence, 36, 33, 34, dt, var_f13);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_UBERGRIND);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cRiderAnim_updatePosNegSlider_42E0(void *, void *, int *, int, int, float, float) __asm__("cRiderAnim_updatePosNegSlider");

/**
 * Update rule 10 of cRiderAnim_updateRule: a positive / negative slider.
 *
 * Sequence kAID_RSFSGrind1Bal (218) uses game animations 362 / 363, kAID_RSFSGrind2Bal (226) 371 / 372,
 * kAID_RSFSGrind3Bal (234) 380 / 381, any other 389 / 390 (positive / negative), with the rider's (+0x60) +0x238 as the
 * weight, through cRiderAnim_updatePosNegSlider.
 *
 * @param sequencer  the sequencer
 * @param sequence  the sequence (first word: its animation id)
 * @param deltaTime  the time step
 * @return cRiderAnim_updatePosNegSlider's result: 1 when the sequence was expired
 */
int cRiderAnim::onUpdateASYNC_UBERGRIND(void *sequencer, int *sequence, float deltaTime)
{
    int x = *sequence;
    if (x == kAID_RSFSGrind1Bal) {
        return cRiderAnim_updatePosNegSlider_42E0(this, sequencer, sequence, 362, 363, deltaTime,
                Rider->mSlewed[6].cur);
    }
    if (x == kAID_RSFSGrind2Bal) {
        return cRiderAnim_updatePosNegSlider_42E0(this, sequencer, sequence, 371, 372, deltaTime,
                Rider->mSlewed[6].cur);
    }
    if (x == kAID_RSFSGrind3Bal) {
        return cRiderAnim_updatePosNegSlider_42E0(this, sequencer, sequence, 380, 381, deltaTime,
                Rider->mSlewed[6].cur);
    }
    return cRiderAnim_updatePosNegSlider_42E0(this, sequencer, sequence, 389, 390, deltaTime, Rider->mSlewed[6].cur);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_PREWIND);
#ifdef SKIP_ASM
// PORT: PS2 abs.s helper (stands in for fabsf).
static inline float vAbs_4358(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}
/**
 * Update rule 7 of cRiderAnim_updateRule: sets the animation's frame from the rider's prewind amounts.
 *
 * The time of sub-animation 0 is the larger of |rider +0x2A4| and |rider +0x2B0| (rider at +0x60) times the sequence's
 * length. Then the blend weight advances by @p dt / 60, expiring the sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param dt  the time step
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::onUpdateASYNC_PREWIND(void* x, cAnimSequence* anim, float dt)
{
    cRider* rider = Rider;
    float absSlew15 = vAbs_4358(rider->mSlewed[15].cur);
    float absSlew16 = vAbs_4358(rider->mSlewed[16].cur);
    float framePercent = (absSlew16 < absSlew15) ? absSlew15 : absSlew16;
    cAnimSequence_setSubAnimTime(anim, 0, framePercent * anim->SubAnims[0].TotalTime);
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f) != 0) {
        cAnimSequencer_expire(x, anim);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_AIR_ADJ);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2 abs.s helper (stands in for fabsf).
static inline float vAbs_43F8(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}
// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float clamp_43F8(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

/**
 * Update rule 11 of cRiderAnim_updateRule: adjusts an air animation by the rider's control amounts.
 *
 * Sequences kAID_IntoAirAdjL .. kAID_IntoAirAdjBwdL (297-304) select a looping sub-animation 1 (game animations
 * 165-172; 165 for others). The amount is the larger of |rider +0x298| and |rider +0x28C| (rider at +0x60), clamped to
 * 0..1: sub-animation 1 gets weight 0.25 + 0.75 * amount and sub-animation 0 the rest, sub-animation 0's time is the
 * amount times the length, and sub-animation 1 advances by @p dt / 60. Then the blend weight advances, expiring the
 * sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence (first word: its animation id)
 * @param dt  the time step
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::onUpdateASYNC_AIR_ADJ(void* x, cAnimSequence* anim, float dt)
{
    int slot = 165;
    switch (anim->SequenceID) {
    case kAID_IntoAirAdjL: slot = 165; break;
    case kAID_IntoAirAdjFwdL: slot = 171; break;
    case kAID_IntoAirAdjFwd: slot = 168; break;
    case kAID_IntoAirAdjFwdR: slot = 172; break;
    case kAID_IntoAirAdjR: slot = 166; break;
    case kAID_IntoAirAdjBwdR: slot = 170; break;
    case kAID_IntoAirAdjBwd: slot = 167; break;
    case kAID_IntoAirAdjBwdL: slot = 169; break;
    }
    cAnimSequence_setSubAnim(anim, 1, *(cGameAnimMap_TheGameAnimMap->GameAnimMap + slot));
    cAnimSequence_setSubAnimType(anim, 1, 1);
    cRider* rider = Rider;
    float absSlew14 = vAbs_43F8(rider->mSlewed[14].cur);
    float absSlew13 = vAbs_43F8(rider->mSlewed[13].cur);
    float framePercent = (absSlew13 < absSlew14) ? absSlew14 : absSlew13;
    framePercent = clamp_43F8(framePercent, 0.0f, 1.0f);
    float w = framePercent * 0.75f + 0.25f;
    cAnimSequence_setSubAnimWeight(anim, 0, 1.0f - w);
    cAnimSequence_setSubAnimWeight(anim, 1, w);
    cAnimSequence_setSubAnimTime(anim, 0, framePercent * anim->SubAnims[0].TotalTime);
    float stepTime = dt * 0.01666666753590107f;
    cAnimSequence_updateSubAnim(anim, 1, stepTime);
    if (cAnimSequence_updateWeight(anim, stepTime)) {
        cAnimSequencer_expire(x, anim);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_AIR_CYC);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRiderAnim_updateIntoCycle_45B8(int, int, int, int, int) __asm__("cRiderAnim_updateIntoCycle");

/**
 * Update rule 12 of cRiderAnim_updateRule: plays game animation 137 once, then loops 138 (cRiderAnim_updateIntoCycle).
 *
 * The time step is passed on untouched in its register, and cRiderAnim_updateIntoCycle's result is left in the return register
 * although the C declares none.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the sequence
 */
extern "C" void cRiderAnim_onUpdateASYNC_AIR_CYC(int riderAnim, int sequencer, int sequence) {
    cRiderAnim_updateIntoCycle_45B8(riderAnim, sequencer, sequence, 137, 138);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_GATEROCK);
#ifdef SKIP_ASM
/**
 * Update rule 8 of cRiderAnim_updateRule: sets the animation's frame from the rider's gate state.
 *
 * The time of sub-animation 0 is the sequence's length times the float at +0x2A0 of the rider's +0x77C object (rider at
 * +0x60). Then the blend weight advances by @p dt / 60, expiring the sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param dt  the time step
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::onUpdateASYNC_GATEROCK(void* x, cAnimSequence* anim, float dt)
{
    cAnimSequence_setSubAnimTime(anim, 0,
            anim->SubAnims[0].TotalTime * Rider->mRiderHandler->GateControl.GateAnticipate);
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f) != 0) {
        cAnimSequencer_expire(x, anim);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_HANDPLANT);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cRiderAnim_updatePosNegSlider_4660(void *, void *, int *, int, int, float, float) __asm__("cRiderAnim_updatePosNegSlider");

/**
 * Update rule 9 of cRiderAnim_updateRule: a positive / negative slider for handplants.
 *
 * Sequence kAID_HPHS_Balance (45) uses game animations 353 / 352, any other 347 / 346 (positive / negative), with the
 * rider's (+0x60) +0x244 as the weight, through cRiderAnim_updatePosNegSlider.
 *
 * @param sequencer  the sequencer
 * @param sequence  the sequence (first word: its animation id)
 * @param deltaTime  the time step
 * @return cRiderAnim_updatePosNegSlider's result: 1 when the sequence was expired
 */
int cRiderAnim::onUpdateASYNC_HANDPLANT(void *sequencer, int *sequence, float deltaTime)
{
    float w = Rider->mSlewed[7].cur;

    if (*sequence == kAID_HPHS_Balance) {
        return cRiderAnim_updatePosNegSlider_4660(this, sequencer, sequence, 353, 352, deltaTime, w);
    }
    return cRiderAnim_updatePosNegSlider_4660(this, sequencer, sequence, 347, 346, deltaTime, w);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_BP_BAL);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
extern "C" int cRiderAnim_update3Cycle(cRiderAnim* self, void* x, cAnimSequence* anim, int a, int b, float dt, float w,
                                       int c);
/**
 * Update rule 15 of cRiderAnim_updateRule: a three-way board-press balance blend.
 *
 * The weight is 2 * (rider +0x274) - 1 (rider at +0x60), clamped to -1..1. Sequence kAID_NoseBalance (28) blends game
 * animations 49 / 48 / 50, any other 60 / 58 / 59 (negative / centre / positive) with cRiderAnim_update3Cycle.
 *
 * @param x  the sequencer
 * @param anim  the sequence (first word: its animation id)
 * @param dt  the time step
 * @return cRiderAnim_update3Cycle's result: 1 when the sequence was expired
 */
int cRiderAnim::onUpdateASYNC_BP_BAL(void* x, int* anim, float dt)
{
    float centeredSlew = Rider->mSlewed[11].cur;
    centeredSlew = centeredSlew + centeredSlew - 1.0f;
    float w;
    if (centeredSlew >= -1.0f) {
        w = centeredSlew <? 1.0f;
    } else {
        w = -1.0f;
    }
    int negAnim, cenAnim, posAnim;
    negAnim = 0x3C;
    if (*anim != kAID_NoseBalance) {
        cenAnim = 58; posAnim = 59;
    } else {
        negAnim = 49; cenAnim = 0x30; posAnim = 50;
    }
    return update3Cycle(x, (cAnimSequence*)anim, negAnim, cenAnim, dt, w, posAnim);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_BP_INTO);
#ifdef SKIP_ASM
// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float clamp_4728(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}
/**
 * Update rule 14 of cRiderAnim_updateRule: sets the board-press animation's frame from the rider's press amount.
 *
 * The amount is the rider's (+0x60) +0x268; kAID_NoseOutof (25) and kAID_TailOutof (33) use 1 minus it, clamped to
 * 0..1. Sub-animation 0's time is the amount times the length. Then the blend weight advances by @p dt / 60, expiring
 * the sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence (first word: its animation id)
 * @param dt  the time step
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::onUpdateASYNC_BP_INTO(void* x, cAnimSequence* anim, float dt)
{
    float framePercent;
    int animId = anim->SequenceID;
    if (animId == kAID_NoseOutof || animId == kAID_TailOutof) {
        framePercent = clamp_4728(1.0f - Rider->mSlewed[10].cur, 0.0f, 1.0f);
    } else {
        framePercent = Rider->mSlewed[10].cur;
    }
    cAnimSequence_setSubAnimTime(anim, 0, framePercent * anim->SubAnims[0].TotalTime);
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f) != 0) {
        cAnimSequencer_expire(x, anim);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_BP_PIVOT);
#ifdef SKIP_ASM
// PORT: PS2 abs.s helper (stands in for fabsf).
static inline float vAbs_47F0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}
// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float clamp_47F0(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}
/**
 * Update rule 13 of cRiderAnim_updateRule: sets the board-press pivot animation's frame from the rider's pivot amount.
 *
 * The amount is |rider +0x280| (rider at +0x60); kAID_TailPivotL (37) and kAID_TailPivotR (38) use 1 minus it. Clamped
 * to 0..1, times the length, it sets sub-animation 0's time. Then the blend weight advances by @p dt / 60, expiring the
 * sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence (first word: its animation id)
 * @param dt  the time step
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::onUpdateASYNC_BP_PIVOT(void* x, cAnimSequence* anim, float dt)
{
    float framePercent;
    int animId = anim->SequenceID;
    if (animId == kAID_TailPivotL) {
        framePercent = 1.0f - vAbs_47F0(Rider->mSlewed[12].cur);
    } else if (animId == kAID_TailPivotR) {
        framePercent = 1.0f - vAbs_47F0(Rider->mSlewed[12].cur);
    } else {
        framePercent = vAbs_47F0(Rider->mSlewed[12].cur);
    }
    cAnimSequence_setSubAnimTime(anim, 0, clamp_47F0(framePercent, 0.0f, 1.0f) * anim->SubAnims[0].TotalTime);
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f) != 0) {
        cAnimSequencer_expire(x, anim);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateFWD);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cAnimSequence_updateSubAnim_48C0(void*, int, float) __asm__("cAnimSequence_updateSubAnim");
/**
 * Update rule 0 of cRiderAnim_updateRule: advances the animation.
 *
 * Sub-animation 0 advances by @p dt / 60 (cAnimSequence_updateSubAnim); unless that reports the end, the blend weight
 * advances too. When either reports the end the sequence is expired (cAnimSequencer_expire). cRiderAnim_onUpdateHOLD_LAST and
 * cRiderAnim_onUpdateCYCLE have the same code.
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param dt  the time step
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::onUpdateFWD(void* x, void* anim, float dt)
{
    float stepTime = dt * 0.01666666753590107f;
    int done;
    if (cAnimSequence_updateSubAnim_48C0(anim, 0, stepTime) != 0) {
        done = 1;
    } else {
        done = cAnimSequence_updateWeight(anim, stepTime);
    }
    if (done == 0) {
        return false;
    }
    cAnimSequencer_expire(x, anim);
    return true;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateHOLD_LAST);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cAnimSequence_updateSubAnim_4940(void*, int, float) __asm__("cAnimSequence_updateSubAnim");
/**
 * Update rule 1 of cRiderAnim_updateRule: advances the animation (the same code as cRiderAnim_onUpdateFWD).
 *
 * Sub-animation 0 advances by @p dt / 60; unless that reports the end, the blend weight advances too. When either
 * reports the end the sequence is expired.
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param dt  the time step
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::onUpdateHOLD_LAST(void* x, void* anim, float dt)
{
    float stepTime = dt * 0.01666666753590107f;
    int done;
    if (cAnimSequence_updateSubAnim_4940(anim, 0, stepTime) != 0) {
        done = 1;
    } else {
        done = cAnimSequence_updateWeight(anim, stepTime);
    }
    if (done == 0) {
        return false;
    }
    cAnimSequencer_expire(x, anim);
    return true;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateCYCLE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cAnimSequence_updateSubAnim_49C0(void*, int, float) __asm__("cAnimSequence_updateSubAnim");
/**
 * Update rule 2 of cRiderAnim_updateRule: advances the animation (the same code as cRiderAnim_onUpdateFWD).
 *
 * Sub-animation 0 advances by @p dt / 60; unless that reports the end, the blend weight advances too. When either
 * reports the end the sequence is expired.
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param dt  the time step
 * @return true when the sequence was expired, else false
 */
bool cRiderAnim::onUpdateCYCLE(void* x, void* anim, float dt)
{
    float stepTime = dt * 0.01666666753590107f;
    int done;
    if (cAnimSequence_updateSubAnim_49C0(anim, 0, stepTime) != 0) {
        done = 1;
    } else {
        done = cAnimSequence_updateWeight(anim, stepTime);
    }
    if (done == 0) {
        return false;
    }
    cAnimSequencer_expire(x, anim);
    return true;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireINTO_AIR);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRiderAnimBase_playInto_4A40(int, int, int, int) __asm__("cRiderAnimBase_playInto");

/**
 * Expire rule 1 of cRiderAnim_expireRule: plays animation 287 into the sequencer (cRiderAnimBase_playInto).
 *
 * cRiderAnimBase_playInto's result is left in the return register (cRiderAnim_expireRule stores it) although the C
 * declares none.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 */
extern "C" void cRiderAnim_onExpireINTO_AIR(int riderAnim, int sequencer, int sequence) {
    cRiderAnimBase_playInto_4A40(riderAnim, sequencer, sequence, 287);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireINTO_SPIN_CYC);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cRiderAnimBase_playInto_4A60(void*, void*, int*, int) __asm__("cRiderAnimBase_playInto");

/**
 * Expire rule 2 of cRiderAnim_expireRule: plays the follow-on animation matching the expired one.
 *
 * An into-spin or into-flip anim (straight, or off a rail's BS / FS side) plays its spin or flip cycle (the misty and
 * rodeo intos 273-276 play 293-296) with cRiderAnimBase_playInto. kAID_RSBSIntoFlipBwd goes to kAID_FlipFwdCycle (in
 * retail too). Any other returns kAID_Invalid (438) without playing.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence (first word: its animation id)
 * @return cRiderAnimBase_playInto's result, or kAID_Invalid
 */
// Plays the landing animation that matches the current trick state.
int cRiderAnim::onExpireINTO_SPIN_CYC(void* sequencer, int* sequence)
{
    switch (*sequence) {
    case kAID_IntoFlipBwd: case kAID_RSFSIntoFlipBwd:
        return cRiderAnimBase_playInto_4A60(this, sequencer, sequence, kAID_FlipBwdCycle);
    case kAID_IntoFlipFwd: case kAID_RSBSIntoFlipBwd: case kAID_RSBSIntoFlipFwd: case kAID_RSFSIntoFlipFwd:
        return cRiderAnimBase_playInto_4A60(this, sequencer, sequence, kAID_FlipFwdCycle);
    case kAID_IntoSpinBS: case kAID_RSBSIntoSpinBS: case kAID_RSFSIntoSpinBS:
        return cRiderAnimBase_playInto_4A60(this, sequencer, sequence, kAID_SpinBSCycle);
    case kAID_IntoSpinFS: case kAID_RSBSIntoSpinFS: case kAID_RSFSIntoSpinFS:
        return cRiderAnimBase_playInto_4A60(this, sequencer, sequence, kAID_SpinFSCycle);
    case kAID_IntoMistyBS:
        return cRiderAnimBase_playInto_4A60(this, sequencer, sequence, kAID_MistyBSCycle);
    case kAID_IntoMistyFS:
        return cRiderAnimBase_playInto_4A60(this, sequencer, sequence, kAID_MistyFSCycle);
    case kAID_IntoRodeoBS:
        return cRiderAnimBase_playInto_4A60(this, sequencer, sequence, kAID_RodeoBSCycle);
    case kAID_IntoRodeoFS:
        return cRiderAnimBase_playInto_4A60(this, sequencer, sequence, kAID_RodeoFSCycle);
    }
    return kAID_Invalid;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireBLENDTO_AIR);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cRiderAnimBase_playBlendIn_4B48(void*, int, int, int) __asm__("cRiderAnimBase_playBlendIn");

/**
 * Expire rule 5 of cRiderAnim_expireRule: blends into an air animation.
 *
 * Plays animation 436 (cRiderAnimBase_playBlendIn) when the object is linked to a front-end rider (+0x64, see
 * cRiderAnim_linkToFERider), otherwise 287.
 *
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 * @return cRiderAnimBase_playBlendIn's result
 */
int cRiderAnim::onExpireBLENDTO_AIR(int sequencer, int sequence)
{
    if (FERider != 0) {
        return cRiderAnimBase_playBlendIn_4B48(this, sequencer, sequence, 436);
    }
    return cRiderAnimBase_playBlendIn_4B48(this, sequencer, sequence, 287);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireINTO_CRUISE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRiderAnimBase_playInto_4B78(int, int, int, int) __asm__("cRiderAnimBase_playInto");

/**
 * Expire rule 3 of cRiderAnim_expireRule: plays animation 5 into the sequencer (cRiderAnimBase_playInto).
 *
 * cRiderAnimBase_playInto's result is left in the return register (cRiderAnim_expireRule stores it) although the C
 * declares none.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 */
extern "C" void cRiderAnim_onExpireINTO_CRUISE(int riderAnim, int sequencer, int sequence) {
    cRiderAnimBase_playInto_4B78(riderAnim, sequencer, sequence, 5);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireINTO_FL_STAND);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRiderAnimBase_playInto_4B98(int, int, int, int) __asm__("cRiderAnimBase_playInto");

/**
 * Expire rule 7 of cRiderAnim_expireRule: plays animation 3 into the sequencer (cRiderAnimBase_playInto).
 *
 * cRiderAnimBase_playInto's result is left in the return register (cRiderAnim_expireRule stores it) although the C
 * declares none.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 */
extern "C" void cRiderAnim_onExpireINTO_FL_STAND(int riderAnim, int sequencer, int sequence) {
    cRiderAnimBase_playInto_4B98(riderAnim, sequencer, sequence, 3);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireINTO_RSFS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRiderAnimBase_playInto_4BB8(int, int, int, int) __asm__("cRiderAnimBase_playInto");

/**
 * Expire rule 8 of cRiderAnim_expireRule: plays animation 19 into the sequencer (cRiderAnimBase_playInto).
 *
 * cRiderAnimBase_playInto's result is left in the return register (cRiderAnim_expireRule stores it) although the C
 * declares none.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 */
extern "C" void cRiderAnim_onExpireINTO_RSFS(int riderAnim, int sequencer, int sequence) {
    cRiderAnimBase_playInto_4BB8(riderAnim, sequencer, sequence, 19);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireINTO_PRESS);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cRiderAnimBase_playBlendIn_4BD8(void *, int, int, int) __asm__("cRiderAnimBase_playBlendIn");

/**
 * Expire rule 9 of cRiderAnim_expireRule: blends into a board-press animation.
 *
 * Plays kAID_NoseBalance (cRiderAnimBase_playBlendIn) when the rider's (+0x60) PressMode is kPM_Nose, otherwise
 * kAID_TailBalance.
 *
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 * @return cRiderAnimBase_playBlendIn's result
 */
int cRiderAnim::onExpireINTO_PRESS(int sequencer, int sequence)
{
    if (Rider->PressMode == kPM_Nose) {
        return cRiderAnimBase_playBlendIn_4BD8(this, sequencer, sequence, kAID_NoseBalance);
    }
    return cRiderAnimBase_playBlendIn_4BD8(this, sequencer, sequence, kAID_TailBalance);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireBLENDTO_CRUISE);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRiderAnimBase_playBlendIn_4C18(int, int, int, int) __asm__("cRiderAnimBase_playBlendIn");

/**
 * Expire rule 4 of cRiderAnim_expireRule: blends into animation 5 (cRiderAnimBase_playBlendIn).
 *
 * cRiderAnimBase_playBlendIn's result is left in the return register (cRiderAnim_expireRule stores it) although the C
 * declares none.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 */
extern "C" void cRiderAnim_onExpireBLENDTO_CRUISE(int riderAnim, int sequencer, int sequence) {
    cRiderAnimBase_playBlendIn_4C18(riderAnim, sequencer, sequence, 5);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireBLENDTO_RAIL);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRiderAnimBase_playBlendIn_4C38(int, int, int*, int) __asm__("cRiderAnimBase_playBlendIn");

/**
 * Expire rule 6 of cRiderAnim_expireRule: blends into a rail animation.
 *
 * kAID_RSBSLandNormal (69) blends into kAID_RSBSBalance, kAID_RSFSLandNormal (70) into kAID_RSFSBalance, any other into
 * kAID_RSBalance (cRiderAnimBase_playBlendIn). Its result is left in the return register (cRiderAnim_expireRule stores
 * it) although the C declares none.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence (first word: its animation id)
 */
extern "C" void cRiderAnim_onExpireBLENDTO_RAIL(int riderAnim, int sequencer, int *sequence) {
    int var_7;
    switch (*sequence) {
    case kAID_RSBSLandNormal:
        var_7 = kAID_RSBSBalance;
        break;
    case kAID_RSFSLandNormal:
        var_7 = kAID_RSFSBalance;
        break;
    default:
        var_7 = kAID_RSBalance;
        break;
    }
    cRiderAnimBase_playBlendIn_4C38(riderAnim, sequencer, sequence, var_7);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireBLENDTO_GEAR_CYC);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRiderAnimBase_playBlendIn_4C80(int, int, int, int) __asm__("cRiderAnimBase_playBlendIn");

/**
 * Expire rule 10 of cRiderAnim_expireRule: blends into animation 434 (cRiderAnimBase_playBlendIn).
 *
 * cRiderAnimBase_playBlendIn's result is left in the return register (cRiderAnim_expireRule stores it) although the C
 * declares none.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 */
extern "C" void cRiderAnim_onExpireBLENDTO_GEAR_CYC(int riderAnim, int sequencer, int sequence) {
    cRiderAnimBase_playBlendIn_4C80(riderAnim, sequencer, sequence, 434);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onExpireNOP);
#ifdef SKIP_ASM
/**
 * Expire rule 0 of cRiderAnim_expireRule: expires the sequence (cAnimSequencer_expire) and plays nothing.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 * @return 438, the value cRiderAnim_onExpireINTO_SPIN_CYC also returns when it plays nothing
 */
extern "C" int cRiderAnim_onExpireNOP(int riderAnim, void* sequencer, void* sequence) {
    cAnimSequencer_expire(sequencer, sequence);
    return 438;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_triggerAnimSound);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cSSXAudio_PlayAniTag_4CC8(int, cRider *, int) __asm__("cSSXAudio_PlayAniTag");
extern int gSSXAudio_seg_1218 __asm__("gSSXAudio");

/**
 * Plays an animation sound tag for the rider: cSSXAudio_PlayAniTag(gSSXAudio, rider, id & 0xFFFF) when the rider
 * (+0x60) is set.
 *
 * cRiderAnim_checkForSoundEvents calls it.
 *
 * @param soundId  the sound tag
 */
void cRiderAnim::triggerAnimSound(int soundId)
{
    cRider* rider = Rider;
    int maskedSoundId = soundId & 0xFFFF;
    if (rider != 0) {
        cSSXAudio_PlayAniTag_4CC8(gSSXAudio_seg_1218, rider, maskedSoundId);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_mapAIDToGAD);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cAIAnimEventMap_mapAIDToGAD_4CF8(int, int, int) __asm__("cAIAnimEventMap_mapAIDToGAD");
extern int cRiderAnimBase_AIAnimEventMap;

/**
 * Maps an AI animation id to the game animation data id for this rider's animation set.
 *
 * The animation filter comes from FERider (its mAnimFilter) when linked to a front-end rider, else from Rider (its
 * mAnimFilter), else 0. The result is cAIAnimEventMap_mapAIDToGAD(cRiderAnimBase_AIAnimEventMap, aid, filter) (On Tour
 * cAIAnimEventMap::mapAIDToGAD); the C view is void and the id passes through in v0. Virtual: slot 4 of
 * cRiderAnim_vtbl.
 *
 * @param aid  the AI animation id (tAIAnimID)
 */
void cRiderAnim::mapAIDToGAD(int aid)
{
    int animFilter;
    cFERider* feRider;
    cRider* rider;

    feRider = FERider;
    animFilter = 0;
    if (feRider != 0) {
        animFilter = feRider->mAnimFilter;
    } else {
        rider = Rider;
        if (rider != 0) {
            animFilter = rider->mAnimFilter;
        }
    }
    cAIAnimEventMap_mapAIDToGAD_4CF8(cRiderAnimBase_AIAnimEventMap, aid, animFilter);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_linkToRider);
#ifdef SKIP_ASM
/**
 * Attaches the rider animation object to an in-game rider.
 *
 * Sets Rider (+0x60) to @p rider, clears FERider (+0x64) and copies the rider's mAnimModel (+0x780) into +0x54 (On Tour
 * cRiderAnimBase::AnimModel). Called once by cRider_loadRider right after it constructs the object.
 *
 * @param rider  the rider (cRider) to drive
 */
void cRiderAnim::linkToRider(cRider* rider)
{
    FERider = 0;
    Rider = rider;
    AnimModel = rider->mAnimModel;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_linkToFERider);
#ifdef SKIP_ASM
/**
 * Links the rider animation object to a front-end rider instead of a game rider.
 *
 * Stores @p feRider at +0x64, clears the rider pointer +0x60 and copies the word at +8 of @p feRider into +0x54.
 * cFERider_compileModel calls it; cRiderAnim_onExpireBLENDTO_AIR tests +0x64.
 *
 * @param feRider  the front-end rider
 */
void cRiderAnim::linkToFERider(cFERider* feRider)
{
    FERider = feRider;
    Rider = 0;
    AnimModel = feRider->mpAnimModel;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void cAnimSequencer_writeToReplayFrame(void*, cBxReplayFrame*);
/**
 * Writes the rider animation state into a replay frame.
 *
 * Writes the object's first 80 bytes through the stream's write, then each of the 6 sequencers (8 bytes each,
 * array at +0x50) with cAnimSequencer_writeToReplayFrame. cRiderHandler_writeToReplayFrame calls it.
 *
 * @param stream  the replay frame stream
 */
void cRiderAnim::writeToReplayFrame(cBxReplayFrame* stream)
{
    stream->write((const char*)this, 80);
    for (int i = 0; i < 6; i++) {
        cAnimSequencer_writeToReplayFrame(&Sequencer[i], stream);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_readFromReplayFrame);
#ifdef SKIP_ASM
extern "C" void cAnimSequencer_expireAll(void*);
extern "C" void cAnimSequencer_readFromReplayFrame(void*, cBxReplayFrame*);
/**
 * Restores the rider animation state from a replay frame.
 *
 * Reads the object's first 80 bytes through the stream's read, then for each of the 6 sequencers (array at
 * +0x50) drops what it plays (cAnimSequencer_expireAll) and reads it (cAnimSequencer_readFromReplayFrame).
 * cRiderHandler_readFromReplayFrame calls it.
 *
 * @param stream  the replay frame stream
 */
void cRiderAnim::readFromReplayFrame(cBxReplayFrame* stream)
{
    stream->read((char*)this, 80);
    for (int i = 0; i < 6; i++) {
        cAnimSequencer_expireAll(&Sequencer[i]);
        cAnimSequencer_readFromReplayFrame(&Sequencer[i], stream);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_objectIntersect);
#ifdef SKIP_ASM
#include "intersect/worldinttypes.h"
#include "world/patchrecord.h"

// A tIntersectInfo with its empty constructor ([O] On Tour calls tIntersectInfo::tIntersectInfo(void)): retail runs
// the empty construction loop over the 64 hits below.
struct tIntersectInfoCtor : tIntersectInfo {
    tIntersectInfoCtor() {}
};

extern t4Vector t4vect_Zero_4E70 __asm__("t4vect_Zero");
extern "C" int cOverlapList_staticObjectIntersect_4E70(void*, cWorldIntersectType*, tIntersectInfo*, int, int)
    __asm__("cOverlapList_staticObjectIntersect");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_4E70(const t4Vector& a, const t4Vector& b)
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
static inline float Len_4E70(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_4E70(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void AddEq_4E70(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b));
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline t4Vector Div_4E70(const t4Vector& v, float s)
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

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_4E70(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int IsWall_4E70(const tIntersectInfo& h)
{
    tCollideSurface* t = &h.Inst->m_pCollideRecord->Surfaces[h.Mesh];
    int r = 0;
    if ((t->Flags & 1) && t->Bounce != 0.0f)
        r = 1;
    return r;
}

static inline int Facing_4E70(cRider* self, const tIntersectInfo& h)
{
    int r = 1;
    if (!(Dot_4E70(Sub_4E70(h.Ipos, self->mSphereTree->boundCen), h.Inorm) < 0.0f))
        r = 0;
    return r;
}

// Whether hit h wins a distance tie against the current pick: the lower surface id, a surface over none, or (neither
// has one) the lower owner order. (Passing hits[i] to an inline function keeps retail's index-first address add.)
static inline int WinsTie_4E70(const tIntersectInfo& h, const tIntersectInfo& pick)
{
    int take;
    if (h.Inst) {
        if (!pick.Inst)
            take = 1;
        else
            take = h.Inst->m_RID < pick.Inst->m_RID;
    } else {
        if (pick.Inst)
            take = 0;
        else
            take = h.Patch->m_RID < pick.Patch->m_RID;
    }
    return take;
}

/**
 * Tests the rider against static objects and returns the distance of the contact it picks.
 *
 * Gets up to 64 hits from cOverlapList_staticObjectIntersect (on mOverlapList); returns -1.0 when there are none.
 * The distinct instance ids (Inst->m_RID) of the hits are kept in mVisitedIds, ended by 0xFFFFFFFF
 * (cRider_isIntersecting reads them). The pick prefers wall hits (the mesh's tCollideSurface has Flags bit 0 and a
 * nonzero Bounce), then hits whose normal points back towards the rider's sphere-tree centre
 * (mSphereTree->boundCen), then the smallest |Iparam - ideal| (the query's GetIdealParam); ties go to the lower
 * instance id, or the lower patch id (Patch->m_RID) when neither hit an instance. The pick is copied to @p out; with
 * two or more hits its normal becomes the normalized sum of the wall hits' normals (when longer than 0.001). Then the
 * instance's active node gets the hit (cInstanceNode::updateIntersectInfo).
 *
 * @param sc  the intersection query
 * @param out  receives the chosen hit
 * @return the chosen hit's distance, or -1.0 when nothing was hit
 */
float cRider::objectIntersect(cWorldIntersectType* sc, tIntersectInfo* out)
{
    // mVisitedIds is written as *(mVisitedIds + k): the subscript form keeps &mVisitedIds in a register (retail
    // adds the index to this and stores at 0x5B8 off that).
    int nIds = 0;
    tIntersectInfoCtor hits[64];
    int n = cOverlapList_staticObjectIntersect_4E70(mOverlapList, sc, hits, 64, 0);
    if (n == 0)
        return -1.0f;
    if (hits[0].Inst)
        *(mVisitedIds + nIds++) = hits[0].Inst->m_RID;
    int best = 0;
    float hgt = sc->GetIdealParam();
    float bestD = Abs_4E70(hits[0].Iparam - hgt);
    int bestWall = IsWall_4E70(hits[0]);
    int bestFacing = Facing_4E70(this, hits[0]);
    for (int i = 1; i < n; i++) {
        if (hits[i].Inst) {
            int j;
            for (j = 0; j < nIds; j++) {
                if (*(mVisitedIds + j) == hits[i].Inst->m_RID)
                    break;
            }
            if (j >= nIds)
                *(mVisitedIds + nIds++) = hits[i].Inst->m_RID;
        }
        float dd = Abs_4E70(hits[i].Iparam - hgt);
        int wall = IsWall_4E70(hits[i]);
        if (wall && !bestWall) {
            best = i;
            bestD = dd;
            bestWall = 1;
            continue;
        }
        else if (bestWall && !wall)
            continue;
        int facing = Facing_4E70(this, hits[i]);
        if (facing && !bestFacing) {
            best = i;
            bestD = dd;
            bestFacing = 1;
            continue;
        }
        else if (bestFacing && !facing)
            continue;
        if (dd < bestD) {
            best = i;
            bestD = dd;
            continue;
        }
        if (dd == bestD) {
            if (WinsTie_4E70(hits[i], hits[best]))
                best = i;
        }
    }
    *out = hits[best];
    if (n >= 2) {
        t4Vector sum = t4vect_Zero_4E70;
        for (int k = 0; k < n; k++) {
            if (IsWall_4E70(hits[k]))
                AddEq_4E70(sum, hits[k].Inorm);
        }
        float len = Len_4E70(sum);
        if (len > 0.0010000000474974513f)
            out->Inorm = Div_4E70(sum, len);
    }
    *(mVisitedIds + nIds) = 0xFFFFFFFF;
    if (out->Inst && out->Inst->m_pActiveNode)
        out->Inst->m_pActiveNode->updateIntersectInfo(*out);
    return out->Iparam;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cRider_checkObjectCollisions);
#ifdef SKIP_ASM
// FAKEMATCH: the scope object's address goes through a copy `ps` to one call while the destructor gets `&sc`
// (an inconsistently used address copy); plain `&sc` scores 99.98%. Landed 2026-10-18 from a saved result before
// this was noticed; marked under the 2026-11-14 ruling (portable, behaviour-preserving steering only).

struct sTri_5398 {
    int a;
    float b;
    unsigned short flags;
    unsigned short pad;
};

struct sMat_5398 {
    char pad[0x10];
    sTri_5398 tris[1];
};




struct cScope_5398 {
    char data[0x320];
    cScope_5398(void* tree, int flag) __asm__("cWorldRider_cWorldRider");
};

int cRider_getControlState_5398(void*) __asm__("cRider_getControlState__FPv");
int cRider_getMotionState(void*);
extern "C" void cRider_checkRailBodyCollisions(void*);
extern "C" float cRider_objectIntersect_5398(char*, cScope_5398*, tIntersectInfo*) __asm__("cRider_objectIntersect");
extern "C" int cRider_isGroundNormalValid(void*);
extern "C" void cRider_doObjectCollision_5398(char*, tInstanceRecord*, tCollideInfo*, tIntersectInfo*, float) __asm__("cRider_doObjectCollision");
extern "C" void cSSXAudio_CollisionSound_5398(void*, char*, tInstanceRecord*, int) __asm__("cSSXAudio_CollisionSound");
extern "C" void cCameraTriggerMan_checkCollisions(void*, void*);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);
extern char* gSSXAudio_5398 __asm__("gSSXAudio");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_5398(const t4Vector& a, const t4Vector& b)
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
static inline float Len_5398(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_5398(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_5398(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline t4Vector Div_5398(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Norm_5398(const t4Vector& v)
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
        : "m"(v));
    return r;
}

static inline unsigned short TriFlags_5398(tInstanceRecord* s, int i)
{
    return ((sTri_5398*)(i * sizeof(sTri_5398) + (int)((sMat_5398*)s->m_pCollideRecord)->tris))->flags; // PORT: pointer in int
}

/**
 * Tests the rider against world objects and resolves the deepest contact.
 *
 * Skipped in CS_Reset. Runs cRider_checkRailBodyCollisions (rail body collisions) first, then queries a cWorldRider
 * (cWorldRider_cWorldRider) built on mSphereTree (+0xAA0) for the deepest hit (cRider_objectIntersect, On Tour
 * objectIntersect). With a hit, the contact normal is flattened against mGroundNormal (when cRider_isGroundNormalValid)
 * or @p norm, or in MS_Railslide snapped to the handler's up vector (+0xB0); the impact speed is the closing speed
 * along it. A triangle with flag 2 on an object without flag 0x2000 caches the contact at +0x9E0 and +0xA60; flag 1
 * runs cRider_doObjectCollision (doObjectCollision); objects without 0x2000 play cSSXAudio_CollisionSound. Always ends
 * with the camera trigger check (cCameraTriggerMan_checkCollisions on gCamTriggerMan) and cWorldRider__cWorldRider.
 *
 * @param norm  a ground normal to project the contact normal onto, or 0 (cAirMotion_resolve passes one)
 */
extern "C" void cRider_checkObjectCollisions(cRider* self, t4Vector* norm)
{
    tIntersectInfo contact;
    tCollideInfo info;
    if (cRider_getControlState_5398((char*)self) == CS_Reset)
        return;
    float zero = 0.0f;
    cRider_checkRailBodyCollisions((char*)self);
    cScope_5398 sc(self->mSphereTree, 1);
    cScope_5398* ps = &sc;
    float overlap = cRider_objectIntersect_5398(((char*)self), ps, &contact);
    if (overlap >= zero) {
        if (cRider_isGroundNormalValid((char*)self))
            norm = (t4Vector*)(&self->mGroundNormal);
        if (norm) {
            t4Vector flatNormal = Sub_5398((*(t4Vector*)&contact.Inorm), Scale_5398(*norm, Dot_5398((*(t4Vector*)&contact.Inorm), *norm)));
            float len = Len_5398(flatNormal);
            if (len < 0.0010000000474974513f)
                (*(t4Vector*)&contact.Inorm) = *norm;
            else
                (*(t4Vector*)&contact.Inorm) = Div_5398(flatNormal, len);
        } else if (cRider_getMotionState((char*)self) == MS_Railslide) {
            t4Vector* up = (t4Vector*)(&self->mRiderHandler->RailslideMotion);
            if (Dot_5398((*(t4Vector*)&contact.Inorm), *up) > zero)
                (*(t4Vector*)&contact.Inorm) = *up;
            else
                (*(t4Vector*)&contact.Inorm) = Scale_5398(*up, -1.0f);
        }
        float k = Dot_5398(Scale_5398((*(t4Vector*)&contact.Inorm), -1.0f), Sub_5398(*(t4Vector*)&self->mVelocity, (*(t4Vector*)&contact.Ivel))) >= 0.0f
                      ? Dot_5398(Scale_5398((*(t4Vector*)&contact.Inorm), -1.0f), Sub_5398(*(t4Vector*)&self->mVelocity, (*(t4Vector*)&contact.Ivel)))
                      : 0.0f;
        (*(t4Vector*)&info.Pos) = (*(t4Vector*)&contact.Ipos);
        (*(t4Vector*)&info.Direct) = Norm_5398(*(t4Vector*)&self->mVelocity);
        (*(t4Vector*)&info.Reflect) = (*(t4Vector*)&contact.Inorm);
        info.Vel = k;
        if ((TriFlags_5398(contact.Inst, contact.Mesh) & 2) && !(contact.Inst->m_iFlags & 0x2000)) {
            *(&self->CollideIntersectInfo) = contact;
            *(&self->CollideInfo) = info;
        }
        if (TriFlags_5398(contact.Inst, contact.Mesh) & 1)
            cRider_doObjectCollision_5398(((char*)self), contact.Inst, &info, &contact, overlap);
        if (!(contact.Inst->m_iFlags & 0x2000))
            cSSXAudio_CollisionSound_5398(gSSXAudio_5398, ((char*)self), contact.Inst, contact.Mesh);
    }
    cCameraTriggerMan_checkCollisions(((char*)&gCamTriggerMan), ((char*)self));
    cWorldRider__cWorldRider(&sc, 2);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_doObjectCollision);
#ifdef SKIP_ASM
// Rider hits a surface: hit->n is the contact normal, hit->vel the surface velocity.
int cRider_getControlState_57B8(void*) __asm__("cRider_getControlState__FPv");
int cRider_getMotionState(void*);
// PORT: the project names this cRider_setControlState__FPv (one param); callers pass a state too.
void cRider_setControlState_57B8(void* self, int v) __asm__("cRider_setControlState__FPv");
int cRiderAnimBase_getCurrentAnimID(void* anim, int ch);
extern "C" float cRiderAnimBase_getCurrentAnimFramePercent(void* anim, int ch);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void cRiderAnimBase_play(void*, int, int, float);
extern "C" void cRider_updateOrientationImplicit(cRider* self);
// The unit declares this later with its own vector type; bind it by asm label.
extern "C" void cAirPredictor_startLaunchIntoAir_57B8(void*, t4Vector*, t4Vector*, float) __asm__("cAirPredictor_startLaunchIntoAir");
extern "C" void cRider_doBump(cRider* self, tCollideInfo* info, int flag);
extern "C" void cRider_reactToCollision(void* rider, void* info, int id);
extern "C" void cRider_moveFromCollision(void* rider, const t4Vector& v);
extern "C" void cRider_turnFromCollision(void* rider, void* hit);
extern "C" void cRider_flagEventLand(void* rider, int a, int b, int c, void* hit, float speed);
extern "C" void cRiderMetrix_evTakeoff(void* metrix, int a, int b);


// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_57B8(const t4Vector& a, const t4Vector& b)
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
static inline float Len_57B8(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector Scale_57B8(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place vector * scalar).
static inline void ScaleEq_57B8(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_57B8(const t4Vector& a, const t4Vector& b)
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
static inline void AddEq_57B8(t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_57B8(const t4Vector& a, const t4Vector& b)
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

static inline float DotV_57B8(t4Vector a, const t4Vector& b)
{
    return Dot_57B8(a, b);
}

// PORT: g++ minimum operator.
static inline float Clamp_57B8(float x, float lo, float hi)
{
    if (lo <= x)
        return x <? hi;
    return lo;
}

static inline float Max_57B8(float x, float lo)
{
    if (lo < x)
        return x;
    return lo;
}

// PORT: PS2 FPU sqrt.s via inline asm; use sqrtf off-PS2.
static inline float Sqrt_57B8(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Responds to the rider hitting a world object: pushes it out, rebounds it and picks a reaction.
 *
 * Skipped in CS_Reset. The hit surface's Bounce and Grip come from the object's m_pCollideRecord (its Surfaces entry
 * for the hit mesh); with no bounce it only bumps (cRider_doBump). A solid object (no m_pActiveNode, or one whose
 * shouldRebound is true) moves the rider out by 1.1 x @p depth (cRider_moveFromCollision), rebounds by the closing
 * speed plus clamp(grip x speed, 55.6, 1388.9) and, on the ground, turns the rider along it (cRider_turnFromCollision,
 * cRider_updateOrientationImplicit); other objects get a softer rebound. Airborne below 833.3 the hit adds to
 * mImpactAccum2.
 *
 * In the air with channel 2 in animation category kANCAT_AirAdj (the motion state's value, reused), kANCAT_AirCycle,
 * kANCAT_IntoAir or kANCAT_OutOfAir and unk_180 facing the surface (> 0.3), the hit becomes a takeoff: mVelocity is
 * rebuilt along the surface, then cRider_flagEventLand, cRiderMetrix_evTakeoff (evTakeoff), control states CS_Void and
 * CS_Spin, animation kAID_IntoAir (unless already in its first 20%) and cAirPredictor_startLaunchIntoAir. Otherwise the
 * rebound is added to mVelocity and cRider_reactToCollision picks the reaction.
 *
 * @param obj  the hit object's instance record (tInstanceRecord)
 * @param info  the collide info (tCollideInfo) passed on to the bump or reaction
 * @param hit  the intersection (tIntersectInfo): normal, surface velocity, surface and material
 * @param depth  the penetration depth (On Tour overlap)
 */
void cRider::doObjectCollision(tInstanceRecord* obj, tCollideInfo* info, tIntersectInfo* hit, float depth)
{
    if (cRider_getControlState_57B8(this) == CS_Reset)
        return;
    tCollideSurface* surf = &obj->m_pCollideRecord->Surfaces[hit->Mesh];
    float grip = Clamp_57B8(surf->Grip, 0.0f, 1.0f);
    float bounce = surf->Bounce;
    if (bounce == 0.0f) {
        doBump(info, 1);
        return;
    }
    float vn = Dot_57B8(hit->Inorm, Sub_57B8(hit->Ivel, mVelocity)) >= 0.0f
                   ? Dot_57B8(hit->Inorm, Sub_57B8(hit->Ivel, mVelocity)) : 0.0f;
    cInstanceNode* handler = obj->m_pActiveNode;
    if (handler == 0 || handler->shouldRebound()) {
        cRider_moveFromCollision(this, Scale_57B8(hit->Inorm, depth * 1.100000023841858f));
        if (vn < 0.0f)
            vn = 0.0f;
        bounce = vn + Clamp_57B8(grip * vn, 55.55555725097656f, 1388.888916015625f);
        if (cRider_getMotionState(this) == MS_Ground)
            cRider_turnFromCollision(this, hit);
        cRider_updateOrientationImplicit(this);
    } else {
        float inv = 1.0f / (100.0f / bounce + 1.0f);
        float linearTerm = vn * inv;
        float constTerm = (1.0f - grip) * inv;
        bounce = Sqrt_57B8((linearTerm * linearTerm - constTerm) >? 0.0f) - linearTerm; // PORT: g++ maximum operator
    }
    float facing = DotV_57B8(RiderRoot.row[2], hit->Inorm);
    if (cRider_getMotionState(this) == MS_Air
        || (cRider_getMotionState(this) == MS_Wipeout
            && mRiderHandler->WipeoutMotion.WipeoutMotionState == cWipeoutMotion::kWMS_Air)) {
        if (Len_57B8(mVelocity) < 833.3333740234375f) {
            float* acc = &mImpactAccum2;
            *acc = *acc + (2.5f - facing) * (hit->Inorm.z >= 0.20000000298023224f ? hit->Inorm.z : 0.20000000298023224f);
        }
    }
    int anim = cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 2);
    int state = cRider_getMotionState(this);
    if (state == MS_Air
        && (anim == kANCAT_IntoAir || anim == kANCAT_AirCycle || anim == kANCAT_OutOfAir || anim == state)
        && facing > 0.30000001192092896f) {
        t4Vector w = Add_57B8(Sub_57B8(mVelocity, hit->Ivel), Scale_57B8(hit->Inorm, vn));
        float speed = Len_57B8(w);
        float out = Max_57B8(bounce - vn, 277.77777099609375f);
        float velScale = (speed + 555.5555419921875f) * 0.5f;
        if (speed <= 27.77777862548828f)
            velScale = velScale / 27.77777862548828f;
        else
            velScale = velScale / speed;
        ScaleEq_57B8(w, velScale);
        mVelocity = Add_57B8(w, Scale_57B8(hit->Inorm, out));
        sVEntry<void(void*, float)>* vt = (sVEntry<void(void*, float)>*)VPTR_AT(this, sizeof(cRiderState));
        vt[kRiderVt_vibrationBump].fn(vthis(this, vt[kRiderVt_vibrationBump]), vn);
        if (cRiderAnimBase_getCurrentAnimID(mRiderAnim,
                2) != kAID_IntoAir || cRiderAnimBase_getCurrentAnimFramePercent(mRiderAnim, 2) > 0.20000000298023224f) {
            mSurfaceType = 0;
            cRider_flagEventLand(this, 0, 0, 0, hit, speed);
            cRiderMetrix_evTakeoff(mMetrix, IsRidingFakie != IsGoofyStance, 0);
            cRider_setControlState_57B8(this, CS_Void);
            cRider_setControlState_57B8(this, CS_Spin);
            cRiderAnimBase_play(mRiderAnim, kAID_IntoAir, 1, -1.0f);
        }
        t4Vector* pos = getPosition();
        t4Vector* vel = getVelocity();
        cAirPredictor_startLaunchIntoAir_57B8(mAirPredictor, pos, vel, TerminalVelocity);
        return;
    }
    AddEq_57B8(mVelocity, Scale_57B8(hit->Inorm, bounce));
    cRider_reactToCollision(this, info, hit->Stype);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_reactToCollision);
#ifdef SKIP_ASM
int cRider_getControlState_5D98(void*) __asm__("cRider_getControlState__FPv");
int cRider_getMotionState(void*);
int AIrand();
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void cRider_checkForReset(void* rider, int a, int b);
extern "C" void cRider_flagEventWipeout(void* rider, int anim, int a2, int a3, void* info);
extern "C" void cRider_doBump(cRider* self, tCollideInfo* info, int flag);
// The unit declares this later with its own vector type; bind it by asm label.
extern "C" void cAirPredictor_startLaunchIntoAir_5D98(void*, t4Vector*, t4Vector*, float) __asm__("cAirPredictor_startLaunchIntoAir");
// Per-material records (0xB0 bytes) reached from the game object; +0x44 makes the contact fatal.
struct sMat_5D98 {
    char pad0[0x44];
    int fatal;          // 0x44
    char pad48[0x68];
};
struct sMatList_5D98 {
    char pad0[0x44];
    sMat_5D98* mats;    // 0x44
    sMat_5D98* getMat(int i) { return &mats[i]; }
};
struct sGame_5D98 {
    char pad0[0x84];
    sMatList_5D98* matList; // 0x84
};
extern sGame_5D98* SSXApp_5D98 __asm__("SSXApp");
extern float kHighSpeedCollideThresh;


// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_5D98(const t4Vector& a, const t4Vector& b)
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
static inline float Len_5D98(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_5D98(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_5D98(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float Clamp_5D98(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

static inline t4Vector Get_5D98(char* p) { return *(t4Vector*)p; }

// Rider bumps into something: pick a wipeout/stumble animation from the contact normal
// against the rider's axes (RiderRoot rows 0-2, origin in row 3).
// The unit declares this earlier as (void*, void*, int); bind the typed body by asm label.

/**
 * Chooses the rider's reaction to a collision: a reset on a fatal surface, a wipeout animation, or a bump.
 *
 * Skipped in CS_Reset. Adds 1 - max(0, contact . mImpulse) to mImpactAccum and stores the contact direction in
 * mImpulse. If the surface material's record (SSXApp's material list at +0x84, flag +0x44) is fatal it resets the rider
 * (cRider_checkForReset(self, 0, 1)). Otherwise it sends the vibrationBump virtual; in MS_Wipeout it may relaunch the
 * air predictor, flags the handler's WipeoutControl (IsNewImpact = 1, LatestImpact = mVelocity) and stops; in the air
 * it relaunches the air predictor.
 *
 * It then compares the contact direction with the rider's axes (RiderRoot rows 0-2) and, from the dominant axis, the
 * normal speed and channel 2's animation category, picks a wipeout hit animation (kAID_WH_GrndFromLeft2RollRight ..
 * kAID_WH_NB_LandOnFace2FwdFaceDown, kAID_Invalid for none; random variants by AIrand) for cRider_flagEventWipeout;
 * when none applies it bumps (cRider_doBump).
 *
 * @param info  the collide info: contact position (Pos), direction, normal (Reflect) and normal speed (Vel)
 * @param id  the surface type (On Tour eSurfaceType surf), or eST_Undefined
 */
void cRider::reactToCollision(tCollideInfo* info, int id)
{
    if (cRider_getControlState_5D98(this) == CS_Reset)
        return;
    mImpactAccum += 1.0f - (Dot_5D98(info->Reflect, mImpulse) >? 0.0f); // PORT: g++ maximum operator
    mImpulse = info->Reflect;
    if (id != eST_Undefined) {
        if (SSXApp_5D98->matList->getMat(id)->fatal != 0) {
            cRider_checkForReset(this, 0, 1);
            return;
        }
    }
    sVEntry<void(void*, float)>* vt = (sVEntry<void(void*, float)>*)VPTR_AT(this, sizeof(cRiderState));
    vt[kRiderVt_vibrationBump].fn(vthis(this, vt[kRiderVt_vibrationBump]), info->Vel);
    char* sub = (char*)&VPTR_AT(this, sizeof(cRiderState));
    if (cRider_getMotionState(this) == MS_Wipeout) {
        if (mRiderHandler->WipeoutMotion.WipeoutMotionState == cWipeoutMotion::kWMS_Air) {
            sVEntry<t4Vector*(void*)>* vt2 = *(sVEntry<t4Vector*(void*)>**)sub;
            t4Vector* pos = vt2[kRiderVt_getPosition].fn(vthis(sub, vt2[kRiderVt_getPosition]));
            sVEntry<t4Vector*(void*)>* vt3 = *(sVEntry<t4Vector*(void*)>**)sub;
            t4Vector* vel = vt3[kRiderVt_getVelocity].fn(vthis(sub, vt3[kRiderVt_getVelocity]));
            cAirPredictor_startLaunchIntoAir_5D98(mAirPredictor, pos, vel, TerminalVelocity);
        }
        cRiderHandler* handler = mRiderHandler;
        handler->WipeoutControl.IsNewImpact = 1;
        handler->WipeoutControl.LatestImpact = mVelocity;
        return;
    }
    if (cRider_getMotionState(this) == MS_Air) {
        sVEntry<t4Vector*(void*)>* vt2 = *(sVEntry<t4Vector*(void*)>**)sub;
        t4Vector* pos = vt2[kRiderVt_getPosition].fn(vthis(sub, vt2[kRiderVt_getPosition]));
        sVEntry<t4Vector*(void*)>* vt3 = *(sVEntry<t4Vector*(void*)>**)sub;
        t4Vector* vel = vt3[kRiderVt_getVelocity].fn(vthis(sub, vt3[kRiderVt_getVelocity]));
        cAirPredictor_startLaunchIntoAir_5D98(mAirPredictor, pos, vel, TerminalVelocity);
    }
    float sideDot = Dot_5D98(Get_5D98((char*)&RiderRoot), info->Reflect);
    float frontDot = Dot_5D98(Get_5D98((char*)&RiderRoot.row[1]), info->Reflect);
    float upDot = Dot_5D98(Get_5D98((char*)&RiderRoot.row[2]), info->Reflect);
    t4Vector origin = Get_5D98((char*)&RiderRoot.row[3]);
    t4Vector rel = Sub_5D98(info->Pos, origin);
    float contactHeight = Dot_5D98(rel, Get_5D98((char*)&RiderRoot.row[2]));
    int cur = cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 2);
    int anim = kAID_Invalid;
    if (cRider_getControlState_5D98(this) != CS_Handplant) {
        if (cRider_getMotionState(this) == MS_Air && info->Reflect.z > 0.800000011920929f) {
            if (cur == kANCAT_Uber || cur == kANCAT_UberGrind) {
                anim = kAID_WH_NB_LandOnFace2FwdFaceDown;
            } else if (Abs_5D98(frontDot) > Abs_5D98(sideDot) && Abs_5D98(frontDot) > Abs_5D98(upDot)) {
                anim = frontDot < 0.0f ? kAID_WH_LandOnFace2FwdFaceDown : kAID_WH_LandOnBack2FwdFaceUp;
            } else if (Abs_5D98(upDot) > Abs_5D98(sideDot) && Abs_5D98(upDot) > Abs_5D98(frontDot)) {
                if (upDot < 0.0f)
                    anim = kAID_WH_LandOnHead2RollFwd;
                else if (cur == kANCAT_Grab || cur == kANCAT_Tweak)
                    anim = kAID_WH_LandOnFeet2CartFwd;
            } else {
                anim = sideDot < 0.0f ? kAID_WH_LandOnRight2RollRight : kAID_WH_LandOnLeft2RollLeft;
            }
        } else if (cRider_getMotionState(this) == MS_Air) {
            if (Abs_5D98(frontDot) > Abs_5D98(sideDot) && Abs_5D98(frontDot) > Abs_5D98(upDot)) {
                float minSpeed = 416.5320129394531f;
                if (minSpeed < info->Vel) {
                    if (frontDot < 0.0f) {
                        float len = Len_5D98(mVelocity);
                        if (contactHeight < 60.0f && len > kHighSpeedCollideThresh) {
                            anim = kAID_WH_AirFromFrontLow2CartFwd;
                        } else {
                            unsigned int k;
                            if (len > kHighSpeedCollideThresh)
                                k = (unsigned int)AIrand() % 6;
                            else
                                k = (unsigned int)AIrand() % 3;
                            if (k == 0)
                                anim = kAID_WH_AirFromFront2FwdFaceUp;
                            else if (k == 1)
                                anim = kAID_WH_AirFromFront2BwdFaceUp;
                            else if (k == 2)
                                anim = kAID_WH_AirFromFront2BwdFaceDown;
                            else if (k == 3)
                                anim = kAID_WH_AirFromFront2CartBwd;
                            else
                                anim = kAID_WH_AirFromFront2RollBwd;
                        }
                    } else {
                        anim = (AIrand() & 1) == 0 ? kAID_WH_AirFromBack2RollFwd : kAID_WH_AirFromBack2FwdFaceDown;
                    }
                }
            } else if (Abs_5D98(upDot) > Abs_5D98(sideDot) && Abs_5D98(upDot) > Abs_5D98(frontDot)) {
                if (upDot < 0.0f)
                    anim = kAID_WH_LandOnHead2RollFwd;
                else if (cur >= kANCAT_Grab && cur <= kANCAT_UberGrind)
                    anim = kAID_WH_LandOnFeet2CartFwd;
            } else {
                float minSpeed = 833.337646484375f;
                if (minSpeed < info->Vel)
                    anim = sideDot < 0.0f ? kAID_WH_AirFromRight2RollLeft : kAID_WH_AirFromLeft2RollRight;
            }
            if (anim != kAID_Invalid && (cur == kANCAT_Uber || cur == kANCAT_UberGrind))
                anim = kAID_WH_NB_LandOnFace2FwdFaceDown;
        } else {
            float k = Clamp_5D98(Get_5D98((char*)&mMatrix.row[2]).z + 1.0f - Abs_5D98(info->Reflect.z),
                                 0.20000000298023224f, 1.399999976158142f);
            if (Abs_5D98(frontDot) > Abs_5D98(sideDot) && Abs_5D98(frontDot) > Abs_5D98(upDot)) {
                if (info->Vel > k * 1388.880859375f) {
                    if (frontDot < 0.0f) {
                        float len = Len_5D98(mVelocity);
                        if (contactHeight < 60.0f && len > kHighSpeedCollideThresh) {
                            anim = kAID_WH_GrndFromFrontLow2CartFwd;
                        } else {
                            unsigned int n;
                            if (len > kHighSpeedCollideThresh)
                                n = (unsigned int)AIrand() % 6;
                            else
                                n = (unsigned int)AIrand() % 3;
                            if (n == 0)
                                anim = kAID_WH_GrndFromFront2FwdFaceUp;
                            else if (n == 1)
                                anim = kAID_WH_GrndFromFront2BwdFaceUp;
                            else if (n == 2)
                                anim = kAID_WH_GrndFromFront2BwdFaceDown;
                            else if (n == 3)
                                anim = kAID_WH_GrndFromFront2CartBwd;
                            else
                                anim = kAID_WH_GrndFromFront2RollBwd;
                        }
                    } else {
                        anim = (AIrand() & 1) == 0 ? kAID_WH_GrndFromBack2RollFwd : kAID_WH_GrndFromBack2FwdFaceDown;
                    }
                }
            } else if (Abs_5D98(upDot) > Abs_5D98(sideDot) && Abs_5D98(upDot) > Abs_5D98(frontDot)) {
            } else if (info->Vel > k * 2083.227294921875f) {
                anim = sideDot < 0.0f ? kAID_WH_GrndFromRight2RollLeft : kAID_WH_GrndFromLeft2RollRight;
            }
        }
    }
    if (anim != kAID_Invalid)
        cRider_flagEventWipeout(this, anim, 0, id, info);
    else
        doBump(info, 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_moveFromCollision);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only VU0 inline asm (a += b).
static inline void AddEq_6538(t4Vector& a, const t4Vector& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}
extern "C" void cRiderSphereTree_ApplyOffset(void* p, const t4Vector* d);

/**
 * Moves the rider by an offset out of a collision.
 *
 * Adds @p offset to the vectors at +0x110, +0x9D0, +0x400 and +0x410 and to the sphere tree (+0xAA0,
 * cRiderSphereTree_ApplyOffset).
 *
 * @param offset  the offset
 */
void cRider::moveFromCollision(const t4Vector* offset)
{
    AddEq_6538(mPosition, *offset);
    AddEq_6538(mModelOffset, *offset);
    cRiderSphereTree_ApplyOffset(mSphereTree, offset);
    AddEq_6538(mBoundsMin, *offset);
    AddEq_6538(mBoundsMax, *offset);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_turnFromCollision);
#ifdef SKIP_ASM
// A t4Vector with constructors (no fields of its own, so a t4Vector can be used as one); the 4-float constructor
// stores w first, as retail's axis-angle quaternion temporary does.
struct t4VectorInitW : t4Vector {
    t4VectorInitW() {}
    t4VectorInitW(float ax, float ay, float az, float aw) { w = aw; x = ax; y = ay; z = az; }
};

extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" float func_0031C228(float);

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4VectorInitW Cross_65B0(const t4Vector& a, const t4Vector& b)
{
    t4VectorInitW r;
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
static inline t4VectorInitW Norm_65B0(const t4VectorInitW& v)
{
    t4VectorInitW r;
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
static inline float Dot_65B0(const t4VectorInitW& a, const t4VectorInitW& b)
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
static inline t4VectorInitW QuatMul_65B0(const t4VectorInitW& a, const t4VectorInitW& b)
{
    t4VectorInitW r;
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
        : "m"(a), "m"(b));
    return r;
}

static inline float Atan2_65B0(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            r += 3.1415927410125732f;
        else
            r -= 3.1415927410125732f;
    }
    return r;
}

static inline float Clamp_65B0(float v, float lo, float hi)
{
    // PORT: `<?` is the g++ minimum operator (EE min.s).
    return (v >= lo) ? (v <? hi) : lo;
}

static inline t4VectorInitW AxisAngle_65B0(const t4VectorInitW& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return t4VectorInitW(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline t4VectorInitW Up_65B0(cRider* self) { return *(t4VectorInitW*)&self->mMatrix.row[2]; }
static inline t4VectorInitW Fwd_65B0(cRider* self) { return *(t4VectorInitW*)&self->mMatrix.row[1]; }

static inline void Rotate_65B0(cRider* self, const t4VectorInitW& axis, float angle)
{
    *(t4VectorInitW*)&self->mRotation = QuatMul_65B0(*(t4VectorInitW*)&self->mRotation, AxisAngle_65B0(axis, angle));
}

/**
 * Turns the rider away from a surface it hit.
 *
 * n is the normalized cross product of the up axis (mMatrix row 2) and the hit normal (@p hitInfo Inorm), and the
 * in-plane direction is up x n. The angle is atan2 of the forward axis (mMatrix row 1) against n and the in-plane
 * direction. A positive angle below 110 degrees (1.9198622 rad) gives a turn of min(110 degrees - angle, 20 degrees),
 * mirrored for negative angles; the orientation quaternion (mRotation) is then rotated about the up axis by minus that
 * turn.
 *
 * @param hitInfo  the hit
 */
void cRider::turnFromCollision(tIntersectInfo* hitInfo)
{
    t4VectorInitW n = Norm_65B0(Cross_65B0(Up_65B0(this), hitInfo->Inorm));
    t4VectorInitW inPlaneDir = Cross_65B0(Up_65B0(this), n);
    float x = Dot_65B0(Fwd_65B0(this), inPlaneDir);
    float y = Dot_65B0(Fwd_65B0(this), n);
    float angle = Atan2_65B0(y, x);
    float lim = 1.9198623895645142f;
    float step = 0.349065899848938f;
    float turnAngle;
    if (angle > 0.0f)
        turnAngle = Clamp_65B0(lim - angle, 0.0f, step);
    else
        turnAngle = Clamp_65B0(-lim - angle, -step, 0.0f);
    if (turnAngle != 0.0f) {
        Rotate_65B0(this, Up_65B0(this), -turnAngle);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_updateCollideInfo);
#ifdef SKIP_ASM
extern "C" void cRiderSphereTree_Update1(cRiderSphereTree*);

/**
 * Refreshes the rider's collision sphere tree.
 *
 * Calls cRiderSphereTree_Update1 on mSphereTree (+0xAA0), the sphere tree the object collision queries use (On Tour:
 * cRiderSphereTree::Update). Callers: cRider_updateSkeleton, cRiderHandler_readFromReplayFrame and
 * cRider_DoInSeqAnimUpdate.
 *
 */
void cRider::updateCollideInfo()
{
    cRiderSphereTree_Update1(mSphereTree);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_checkRailBoardCollisions);
#ifdef SKIP_ASM
// Rider touches down: kill the velocity into the ground, classify the landing from the board's
// orientation against the ground normal (state at 0x328), and start the landing reaction.
int cRider_getControlState_57B8(void*) __asm__("cRider_getControlState__FPv");
int cRider_getMotionState(void*);
// PORT: the project names these __FPv (one param); callers pass a state too.
void cRider_setControlState_57B8(void* self, int v) __asm__("cRider_setControlState__FPv");
void cRider_setMotionState_6848(void* self, int v) __asm__("cRider_setMotionState__FPv");
extern "C" float func_0031C228(float);
extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" void cRiderAnimBase_play(void*, int, int, float);
extern "C" void cRiderAnimBase_changeHeadingOffset(void* anim, float a);
extern "C" void cRider_rotateOrientation(cRider* self, const t4Vector& axis, float angle);
extern "C" void cRider_flagEventLand(void* rider, int a, int b, int c, void* hit, float speed);
extern "C" float cRiderMetrix_evLand(void* metrix, int a, int b, int c, int d);
extern "C" void cRider_flagEventBoostPowerUp(cRider* self, int a, float f);
extern "C" void cRider_doRailslideModeOrientation(cRider* self);
extern "C" void cRailslideControl_playGainFocusAnim(void*, int);
extern "C" void cRailslideMotion_setCrookedAngle(void* p, t4Vector* n);
extern t4Vector t4vect_Origin_6848 __asm__("t4vect_Origin");

// The unit defines this later with its own vector type; bind it by asm label.
extern "C" int cRider_isRailLandSuccess_6848(cRider* self, tIntersectInfo* land) __asm__("cRider_isRailLandSuccess");

// A t4Vector with constructors (no fields of its own); the 4-float constructor stores x, y, z, w in order. Quaternion
// temporaries use t4VectorInitW (cRider_turnFromCollision's block), which stores w first.
struct t4VectorInit : t4Vector {
    t4VectorInit() {}
    t4VectorInit(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

extern t4Vector t4vect_ZAxis_6848 __asm__("t4vect_ZAxis");


// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_6848(const t4Vector& a, const t4Vector& b)
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
static inline float Len_6848(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector Scale_6848(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place vector * scalar).
static inline void ScaleEq_6848(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_6848(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float Abs_6848(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Atan2_6848(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

static inline t4Vector GetUp_6848(cRider* rider)
{
    return rider->mMatrix.row[2];
}

static inline float AtLeast_6848(float x, float lo)
{
    return x <= lo ? lo : x;
}

static inline float AtMost_6848(float x, float hi)
{
    return x <= hi ? x : hi;
}

static inline t4VectorInit Side_6848(const sAnimModelBone* animModelBone)
{
    return t4VectorInit(
        1.0f - 2.0f * (animModelBone->rot.y * animModelBone->rot.y + animModelBone->rot.z * animModelBone->rot.z),
                   2.0f * (animModelBone->rot.x * animModelBone->rot.y + animModelBone->rot.w * animModelBone->rot.z),
                   2.0f * (animModelBone->rot.x * animModelBone->rot.z - animModelBone->rot.w * animModelBone->rot.y),
                   0.0f);
}

static inline t4VectorInit Front_6848(const sAnimModelBone* animModelBone)
{
    return t4VectorInit(
        2.0f * (animModelBone->rot.y * animModelBone->rot.x - animModelBone->rot.w * animModelBone->rot.z),
                   1.0f - 2.0f * (animModelBone->rot.z * animModelBone->rot.z + animModelBone->rot.x * animModelBone->rot.x),
                   2.0f * (animModelBone->rot.y * animModelBone->rot.z + animModelBone->rot.w * animModelBone->rot.x),
                   0.0f);
}

static inline t4VectorInit Up_6848(const sAnimModelBone* animModelBone)
{
    return t4VectorInit(
        2.0f * (animModelBone->rot.z * animModelBone->rot.x + animModelBone->rot.w * animModelBone->rot.y),
                   2.0f * (animModelBone->rot.z * animModelBone->rot.y - animModelBone->rot.w * animModelBone->rot.x),
                   1.0f - 2.0f * (animModelBone->rot.x * animModelBone->rot.x + animModelBone->rot.y * animModelBone->rot.y),
                   0.0f);
}

static inline t4VectorInitW AxisAngle_6848(const t4Vector& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return t4VectorInitW(s * axis.x, s * axis.y, s * axis.z, c);
}

#define RAIL_STYLE(r) ((r)->RailslideMode)
#define PRESS_TYPE(r) ((r)->PressMode)

/**
 * Checks for the board touching a rail and, on contact, puts the rider into the railslide motion.
 *
 * cRider_isRailLandSuccess finds the contact. The velocity is turned along its direction at the along-rail speed, at
 * least 20 km/h and at most the old speed, with z cut to a tenth; the hit object's handler gets the opposite change
 * (applyForce). RailslideMode (kRSM_Riding .. kRSM_BS) comes from the control state and the board root bone's angle to
 * the rail. The trick is then awarded (cRider_flagEventLand from the air, else the score commit fed to
 * cRider_flagEventBoostPowerUp()) and MS_Railslide set. From cruise, natural air, spin or handplant the rider starts
 * the board-press (press landing) or railslide control; otherwise a style change away from kRSM_FS or kRSM_BS turns the
 * rider back by 90 degrees.
 *
 * @return true when the rider touched a rail, otherwise false
 */
bool cRider::checkRailBoardCollisions()
{
    tIntersectInfo land;
    if (!cRider_isRailLandSuccess_6848(this, &land))
        return false;
    float vn = Dot_6848(mVelocity, land.Inorm);
    if (vn < 0.0f) {
        vn = -vn;
        ScaleEq_6848(land.Inorm, -1.0f);
    }
    vn = AtLeast_6848(vn, 555.5555419921875f);
    float imp = AtMost_6848(Len_6848(mVelocity), vn);
    t4Vector dv = mVelocity;
    mVelocity = Scale_6848(land.Inorm, imp);
    mVelocity.z = mVelocity.z * 0.10000000149011612f;
    dv = Sub_6848(mVelocity, dv);
    if (land.Inst != 0) {
        cInstanceNode* landInst = land.Inst->m_pActiveNode;
        if (landInst != 0) {
            landInst->applyForce(land, Scale_6848(dv, -1.0f));
        }
    }
    sAnimModelBone* bone = &mAnimModel->bones[mBoneBoardRoot];
    t4VectorInit side = Side_6848(bone);
    t4VectorInit up = Up_6848(bone);
    float sideDot = Dot_6848(side, land.Inorm);
    float upDot = Dot_6848(up, land.Inorm);
    float angle = Atan2_6848(upDot, sideDot);
    int prev = RAIL_STYLE(this);
    if (cRider_getControlState_57B8(this) == CS_BoardPress) {
        if (IsRidingFakie == 0)
            RAIL_STYLE(this) = kRSM_Riding;
        else
            RAIL_STYLE(this) = kRSM_Fakie;
    } else if (PRESS_TYPE(this) != 0) {
        if (Abs_6848(angle) > 1.5707964897155762f)
            RAIL_STYLE(this) = kRSM_Fakie;
        else
            RAIL_STYLE(this) = kRSM_Riding;
    } else if (cRider_getControlState_57B8(this) == CS_UberGrind) {
        RAIL_STYLE(this) = kRSM_FS;
    } else {
        t4VectorInit front = Front_6848(bone);
        if (front.z < 0.0f) {
            if (upDot < 0.0f)
                RAIL_STYLE(this) = kRSM_BS;
            else
                RAIL_STYLE(this) = kRSM_FS;
        } else if (Abs_6848(angle) > 2.6179940700531006f) {
            RAIL_STYLE(this) = kRSM_Fakie;
        } else if (Abs_6848(angle) < 0.523598849773407f) {
            RAIL_STYLE(this) = kRSM_Riding;
        } else if (angle < -0.523598849773407f) {
            RAIL_STYLE(this) = kRSM_BS;
        } else {
            RAIL_STYLE(this) = kRSM_FS;
        }
    }
    mSurfaceType = land.Stype;
    if (cRider_getMotionState(this) == MS_Air) {
        cRider_flagEventLand(this, 0, RAIL_STYLE(this), PRESS_TYPE(this), &land, imp);
    } else {
        cRider_flagEventBoostPowerUp(this, 1, cRiderMetrix_evLand(mMetrix, IsRidingFakie != IsGoofyStance, 0,
                RAIL_STYLE(this), PRESS_TYPE(this)));
    }
    gSSXAudio->mRailType = land.Stype;
    cRider_setMotionState_6848(this, MS_Railslide);
    int st = cRider_getControlState_57B8(this);
    if (st == CS_Cruise || cRider_getControlState_57B8(this) == CS_NaturalAir
        || cRider_getControlState_57B8(this) == CS_Spin
        || cRider_getControlState_57B8(this) == CS_Handplant) {
        int fall = 0;
        if (cRider_getControlState_57B8(this) == CS_Spin || cRider_getControlState_57B8(this) == CS_NaturalAir)
            fall = 1;
        cRider_setControlState_57B8(this, CS_Void);
        cRider_doRailslideModeOrientation(this);
        int kind = PRESS_TYPE(this);
        if (kind != 0) {
            if (kind == kPM_Nose)
                cRiderAnimBase_play(mRiderAnim, kAID_NoseLand, 0, -1.0f);
            else
                cRiderAnimBase_play(mRiderAnim, kAID_TailLand, 0, -1.0f);
            cRider_setControlState_57B8(this, CS_BoardPress);
        } else {
            cRailslideControl_playGainFocusAnim(&mRiderHandler->RailslideControl, fall);
            cRider_setControlState_57B8(this, CS_Railslide);
        }
    } else if (prev != RAIL_STYLE(this)) {
        if (prev == kRSM_FS) {
            cRider_rotateOrientation(this, GetUp_6848(this), 1.5707963705062866f);
            cRiderAnimBase_changeHeadingOffset(mRiderAnim, 1.5707963705062866f);
            float yaw = 0.0f;
            cRiderAnim* riderAnim = mRiderAnim;
            riderAnim->OrientationOffset.P = t4vect_Origin_6848;
            riderAnim->OrientationOffset.Q = AxisAngle_6848(t4vect_ZAxis_6848, -yaw);
        } else if (prev == kRSM_BS) {
            cRider_rotateOrientation(this, GetUp_6848(this), -1.5707963705062866f);
            cRiderAnimBase_changeHeadingOffset(mRiderAnim, -1.5707963705062866f);
            float yaw = 0.0f;
            cRiderAnim* riderAnim = mRiderAnim;
            riderAnim->OrientationOffset.P = t4vect_Origin_6848;
            riderAnim->OrientationOffset.Q = AxisAngle_6848(t4vect_ZAxis_6848, -yaw);
        }
        cRider_doRailslideModeOrientation(this);
    }
    cRailslideMotion_setCrookedAngle(&mRiderHandler->RailslideMotion, &land.Inorm);
    return true;
}

#undef RAIL_STYLE
#undef PRESS_TYPE
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_checkRailBodyCollisions);
#ifdef SKIP_ASM
extern "C" int cRider_isRailLandSuccess_6F78(cRider* self, void* land) __asm__("cRider_isRailLandSuccess");
extern "C" int cOverlapList_splineIntersect_6F78(void*, t4Vector*, t4Vector*, int, float) __asm__("cOverlapList_splineIntersect");





// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_6F78(const t4Vector& a, const t4Vector& b)
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
static inline float Len_6F78(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Norm_6F78(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline t4Vector Scale_6F78(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place vector * scalar).
static inline void ScaleEq_6F78(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place vector / scalar).
static inline void DivEq_6F78(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_6F78(const t4Vector& a, const t4Vector& b)
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
static inline void AddEq_6F78(t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_6F78(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void SubEq_6F78(t4Vector& a, const t4Vector& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: g++ minimum operator.
static inline float Clamp_6F78(float x, float lo, float hi)
{
    if (lo <= x)
        return x <? hi;
    return lo;
}


static inline t4VectorInit Front_6F78(const sAnimModelBone* animModelBone)
{
    return t4VectorInit(
        2.0f * (animModelBone->rot.y * animModelBone->rot.x - animModelBone->rot.w * animModelBone->rot.z),
                    1.0f - 2.0f * (animModelBone->rot.z * animModelBone->rot.z + animModelBone->rot.x * animModelBone->rot.x),
                    2.0f * (animModelBone->rot.y * animModelBone->rot.z + animModelBone->rot.w * animModelBone->rot.x),
                    0.0f);
}

// Snaps the rider onto the rail found ahead of its current bone; returns true when it lands on it.
// PORT: the unit declares cRider_checkRailBodyCollisions as void(void*); it returns a bool.
/**
 * Pushes the rider's body off a rail it runs into without landing on it.
 *
 * Not in MS_Railslide. Looks for a rail spline within 300 of the hip bone (cOverlapList_splineIntersect on
 * mOverlapList); rails behind the motion, more than 50 off the hips' forward line, or that the board can land on
 * (cRider_isRailLandSuccess, isRailLandSuccess) are ignored. For a body hit it notifies the rail's handler
 * (updateIntersectInfo), moves the rider out to 50 from the rail (cRider_moveFromCollision), pushes mVelocity away
 * along the contact normal by the closing speed + 55.6, reports the impulse (applyForce), on the ground turns the rider
 * (cRider_turnFromCollision, cRider_updateOrientationImplicit) and runs cRider_reactToCollision.
 *
 * The // note above the definition says it snaps the rider onto the rail, but a landable rail returns false here;
 * landing is cRider_checkRailBoardCollisions's job.
 *
 * @return true when the body was pushed off a rail, else false
 */
bool cRider::checkRailBodyCollisions()
{
    if (cRider_getMotionState(this) == MS_Railslide) {
        return false;
    }
    sAnimModelBone* animModelBone = &mAnimModel->bones[mBoneHips];
    const t4Vector& dir = Front_6F78(animModelBone);
    tIntersectInfo hit;
    if (!cOverlapList_splineIntersect_6F78(mOverlapList, &animModelBone->pos, &hit.Ipos, 1, 300.0f)) {
        return false;
    }
    float speed = Len_6F78(mVelocity);
    if (speed > 0.0010000000474974513f) {
        float back = Dot_6F78(Sub_6F78(hit.Ipos, animModelBone->pos), mVelocity);
        if (back < speed * -0.20000000298023224f) {
            return false;
        }
    }
    float along = Clamp_6F78(Dot_6F78(dir, Sub_6F78(hit.Ipos, animModelBone->pos)), -50.0f, 50.0f);
    t4Vector delta = Sub_6F78(Add_6F78(animModelBone->pos, Scale_6F78(dir, along)), hit.Ipos);
    if (Len_6F78(delta) > 50.0f) {
        return false;
    }
    tIntersectInfo land;
    if (cRider_isRailLandSuccess_6F78(this, &land)) {
        return false;
    }
    float vn = Dot_6F78(mVelocity, hit.Inorm);
    if (vn < 0.0f) {
        vn = -vn;
        ScaleEq_6F78(hit.Inorm, -1.0f);
    }
    if (hit.Inst != 0) {
        cInstanceNode* handler = hit.Inst->m_pActiveNode;
        if (handler != 0) {
            handler->updateIntersectInfo(hit);
        }
    }
    tCollideInfo info;
    info.Pos = hit.Ipos;
    info.Direct = Norm_6F78(mVelocity);
    info.Vel = vn;
    t4Vector off = Sub_6F78(animModelBone->pos, hit.Ipos);
    SubEq_6F78(off, Scale_6F78(hit.Inorm, Dot_6F78(off, hit.Inorm)));
    if (cRider_isGroundNormalValid(this)) {
        SubEq_6F78(off, Scale_6F78(mGroundNormal, Dot_6F78(off, mGroundNormal)));
    }
    float offLen = Len_6F78(off);
    if (offLen < 0.0010000000474974513f) {
        off = hit.Inorm;
    } else {
        DivEq_6F78(off, offLen);
    }
    info.Reflect = off;
    float clearance = Dot_6F78(delta, off);
    if (clearance < 50.0f) {
        cRider_moveFromCollision(this, Scale_6F78(off, 50.0f - clearance));
    }
    t4Vector oldVel = mVelocity;
    float push = (Dot_6F78(off, Sub_6F78(hit.Ivel, mVelocity)) >? 0.0f) + 55.55555725097656f; // PORT: g++ maximum operator
    AddEq_6F78(mVelocity, Scale_6F78(off, push));
    t4Vector imp = Scale_6F78(Sub_6F78(oldVel, mVelocity), (float)SSXApp->TimerHz);
    if (hit.Inst != 0) {
        cInstanceNode* handler = hit.Inst->m_pActiveNode;
        if (handler != 0) {
            handler->applyForce(hit, imp);
        }
    }
    if (cRider_getMotionState(this) == MS_Ground) {
        hit.Inorm = off;
        cRider_turnFromCollision(this, &hit);
        cRider_updateOrientationImplicit(this);
    }
    cRider_reactToCollision(this, &info, hit.Stype);
    return true;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_checkHandplantCollisions);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// 16-byte vector with inline constructors (a non-POD sibling of t4Vector, as in ai/ai); stores x, y, z, w.
struct sVec4 : t4Vector
{
    sVec4() {}
    sVec4(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract; sVec4 operands).
static inline sVec4 SubV(const t4Vector& a, const t4Vector& b)
{
    sVec4 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len(const t4Vector& v)
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

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float Clamp(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar; sVec4 operands).
static inline sVec4 DivV(const t4Vector& v, float s)
{
    sVec4 r;
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

// PORT: PS2-only VU0 inline asm (in-place vector * scalar).
static inline void ScaleEq(t4Vector& v, float s)
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

int cRider_getMotionState(void*);

// PORT: the project names this cRider_setControlState__FPv (one param); callers pass a state too.
void cRider_setControlState_2(void* self, int v) __asm__("cRider_setControlState__FPv");

extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);

// PORT: the project names these __FPv (one param); callers pass a state too.
void cRider_setMotionState_7578(void* self, int v) __asm__("cRider_setMotionState__FPv");


extern "C" void cCheatFX_setCheatInput(void*, int);
extern "C" int cOverlapList_splineIntersect_7578(void*, sVec4*, tIntersectInfo*, int, float) __asm__("cOverlapList_splineIntersect");
extern "C" void cHandplantMotion_init(float, void*, tIntersectInfo*);
extern "C" void cHandplantControl_init(float, void*, tIntersectInfo*, int);

// PORT: PS2-only VU0 inline asm (vector times scalar, sVec4 operands).
static inline sVec4 ScaleV_7578(const t4Vector& v, float s)
{
    sVec4 r;
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

// PORT: PS2-only VU0 inline asm (vector add, sVec4 operands; "memory" clobber).
static inline sVec4 AddV_7578(const t4Vector& a, const t4Vector& b)
{
    sVec4 r;
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

// PORT: `>?` is the g++ maximum operator (EE max.s).
static inline float Max_7578(float a, float b)
{
    return a >? b;
}

static inline sVec4 Up_7578(cRider* self) { return *(sVec4*)&self->RiderRoot.row[2]; }
static inline sVec4 Front_7578(cRider* self) { return *(sVec4*)&self->mMatrix.row[1]; }

// Speeds are written in km/h and converted to game units (m/s).
static inline float Kph_7578(float kph) { return kph / 3.6f; }
/**
 * Starts a handplant when the rider reaches a near-vertical rail surface ahead.
 *
 * Sends cheat input 2 (cCheatFX_setCheatInput on the handler's +0xD20). In the air (MS_Air) it gives up when falling
 * faster than 2000 km/h, upside down, or in channel 2 animation categories kANCAT_Grab .. kANCAT_Uber. It probes 300
 * ahead along the velocity (the front axis below 100 km/h) for a spline (cOverlapList_splineIntersect, type 2); the hit
 * must be within 300, steep (|normal.z| <= 0.9) and ahead of the motion. Then CS_Void, cHandplantMotion_init and
 * cHandplantControl_init (cHandplantControl init) with clamp(5000 km/h / max(speed, 1000 km/h), 0.5, 1.5) / 2,
 * CS_Handplant and cRider_setMotionState(MS_Handplant).
 *
 * Callers pass a second bool (On Tour isTransThrough); this version takes and reads only the rider.
 *
 * @return true when a handplant started, else false
 */
bool cRider::checkHandplantCollisions()
{
    cCheatFX_setCheatInput(&mRiderHandler->CheatFX, 2);
    if (cRider_getMotionState((char*)this) == MS_Air) {
        if (mVelocity.z < Kph_7578(-2000.0f))
            return false;
        if (Up_7578(this).z < 0.0f)
            return false;
        if ((unsigned int)(cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 2) - 18) < 3)
            return false;
    }
    float spd = Len(mVelocity);
    sVec4 dir;
    tIntersectInfo hit;
    if (spd > Kph_7578(100.0f))
        dir = DivV(mVelocity, spd);
    else
        dir = Front_7578(this);
    sVec4 probePos = AddV_7578(mPosition, ScaleV_7578(dir, 300.0f));
    if (!cOverlapList_splineIntersect_7578(mOverlapList, &probePos, &hit, 2, 300.0f))
        return false;
    if (Abs(hit.Inorm.z) > 0.8999999761581421f)
        return false;
    sVec4 toHit = SubV(hit.Ipos, probePos);
    if (Len(toHit) > 300.0f)
        return false;
    toHit = SubV(hit.Ipos, mPosition);
    if (Dot(mVelocity, toHit) < 0.0f)
        return false;
    spd = Max_7578(spd, Kph_7578(1000.0f));
    float speedRatio = Clamp(Kph_7578(5000.0f) / spd, 0.5f, 1.5f);
    float enterSec = speedRatio * 0.5f;
    if (Dot(hit.Inorm, mVelocity) < 0.0f)
        ScaleEq(hit.Inorm, -1.0f);
    cRider_setControlState_2(((char*)this), CS_Void);
    cHandplantMotion_init(enterSec, &mRiderHandler->HandplantMotion, &hit);
    cHandplantControl_init(enterSec, &mRiderHandler->HandplantControl, &hit, mRiderHandler->HandplantMotion.IsToeside);
    cRider_setControlState_2(((char*)this), CS_Handplant);
    cRider_setMotionState_7578(((char*)this), MS_Handplant);
    return true;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_checkOpponentCollisions);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_7888(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector Scale_7888(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}
// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_7888(const t4Vector& a, const t4Vector& b)
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
// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Norm_7888(const t4Vector& v)
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
// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_7888(const t4Vector& a, const t4Vector& b)
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
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_7888(const t4Vector& v)
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
extern "C" cAnimSequence* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" int AITick();
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_C258(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cRider_moveFromCollision_m2537(void* rider, const t4Vector& v) __asm__("cRider_moveFromCollision");
// The race (seg/seg_1FBE38 layout): riders by being index at 0x28.
struct cRider_1218;
struct sRace_7888 {
    char pad_0x0[0x28];
    cRider_1218* riders[1];     // 0x28
};
struct sGame_7888 {
    char pad_0x0[0xC];
    sRace_7888* race;           // 0xC
};
// The game singleton (SSXApp).
struct sApp_7888 {
    char pad_0x0[0x84];
    sGame_7888* game;           // 0x84
};
extern sApp_7888* SSXApp_7888 __asm__("SSXApp");
// Rider-rider collisions: for every other active rider, push the two apart when their sphere trees touch
// (and exchange momentum along the contact), then shove the other rider when this one runs into it.

extern "C" float cRider_getMass(cRider* self);
extern "C" int cRiderSphereTree_IsRiderCollision(void* a, void* b, t4Vector* dir, t4Vector* pos);
extern "C" float cBEStatInterface_getStrikingAttrib(void* iface, int index, int column);
extern "C" void cRider_doOpponentCollision_7888(cRider* self, cRider* other, t4Vector* n, int flag, float speed) __asm__("cRider_doOpponentCollision");

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void SubEq_7888(t4Vector& a, const t4Vector& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(a)
        : "m"(a), "m"(b));
}

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline t4Vector Div_7888(const t4Vector& v, float s)
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

// Vector times scalar.
static inline t4Vector operator*(const t4Vector& v, float s) { return Scale_7888(v, s); }

/**
 * Handles this rider's contacts with the other riders: pushes overlapping bodies apart and lands shoves.
 *
 * Once per tick per pair, overlapping sphere trees (cRiderSphereTree_IsRiderCollision) push both riders 0.55 of the
 * contact vector apart along their ground planes and, at most every 3 ticks, exchange momentum along the contact
 * (cRider_doOpponentCollision). While this rider's channel-1 animation (kANCAT_Punch) is in its hit window, a rider
 * within 150 in front of PunchVect that is not blocking (kANCAT_Block) is shoved with a strength from the collision
 * stat and PunchStrength, at most every 3 ticks per pair. Actors (mIsActor) are skipped.
 */
void cRider::checkOpponentCollisions()
{
    int busy = 0;
    float mass = cRider_getMass(this);
    if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 1) != kANCAT_Punch ||
        tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events, 1) ||
        !tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 1)->Events, 0))
        busy = 1;
    for (int i = 0; i < 6; i++) {
        if (!mRelations[i].mValid)
            continue;
        cRider* other = (cRider*)SSXApp_7888->game->race->riders[i];
        if (other->mIsActor != 0)
            continue;
        sRiderPair* orec = &other->mRelations[mAIIndex];
        if (orec->OverlapTick < AITick()) {
            sRiderPair* srec = &mRelations[other->mAIIndex];
            srec->OverlapTick = AITick();
            orec->OverlapTick = AITick();
            t4Vector dir;
            t4Vector pos;
            if (cRiderSphereTree_IsRiderCollision(mSphereTree, other->mSphereTree, &dir, &pos)) {
                float omass = cRider_getMass(other);
                t4Vector selfPush = dir * -0.550000011920929f;
                t4Vector otherPush = dir * 0.550000011920929f;
                if (cRider_isGroundNormalValid(this))
                    SubEq_7888(selfPush, Scale_7888(mGroundNormal, Dot_7888(selfPush, mGroundNormal)));
                if (cRider_isGroundNormalValid(other))
                    SubEq_7888(otherPush, Scale_7888(other->mGroundNormal, Dot_7888(otherPush, other->mGroundNormal)));
                cRider_moveFromCollision_m2537(this, selfPush);
                cRider_moveFromCollision_m2537(other, otherPush);
                if (srec->CollideTick < AITick() - 3) {
                    t4Vector n = Norm_7888(dir);
                    float impulse = (Dot_7888(other->mVelocity, n) - Dot_7888(mVelocity, n)) / (mass + omass);
                    cRider_doOpponentCollision_7888(this, other, &n, 0, omass * impulse);
                    cRider_doOpponentCollision_7888(other, this, &n, 0, -mass * impulse);
                    srec->CollideTick = AITick();
                    orec->CollideTick = AITick();
                }
            }
        }
        if (busy)
            continue;
        if (orec->PunchTick >= AITick() - 3)
            continue;
        if (cRiderAnimBase_getCurrentAnimCategory(other->mRiderAnim, 1) == kANCAT_Block &&
            tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(other->mRiderAnim, 1)->Events, 0) &&
            !tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(other->mRiderAnim, 1)->Events, 1))
            continue;
        t4Vector toOther = Sub_7888(other->mPosition, mPosition);
        float len = Len_7888(toOther);
        if (len > 150.0f)
            continue;
        if (len < 0.001f)
            continue;
        t4Vector n = Div_7888(toOther, len);
        if (!(Dot_7888(n, PunchVect) > 0.0f))
            continue;
        float attr = cBEStatInterface_getStrikingAttrib(cBE_getInterface_C258(cBE_getBE(), 3), mAIIndex, mStatsIndex);
        float spd = Len_7888(mVelocity);
        float k = (attr * 3.0001144409179688f + 1.0f) * (PunchStrength + 0.5f) * 599.9739990234375f;
        if (0.001f < spd)
            n = Add_7888(Scale_7888(n, 0.5f), Scale_7888(mVelocity, 0.5f / spd));
        cRider_doOpponentCollision_7888(other, this, &n, 1, k);
        orec->PunchTick = AITick();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_doOpponentCollision);
#ifdef SKIP_ASM

class cAI_7E70 {
public:
    virtual void vm1();
    virtual t4Vector* GetDir();
    virtual void v1();
    virtual void v2();
    virtual t4Vector* GetPos();
};

int cRider_getControlState_7E70(void*) __asm__("cRider_getControlState__FPv");
int cRider_getMotionState(void*);
int AIrand();
extern "C" int cRider_isGroundNormalValid(void*);
extern "C" void cAirPredictor_startLaunchIntoAir(void*, t4Vector*, t4Vector*, float);
extern "C" void cRider_flagEventWipeout_7E70(char*, int, int, int, tCollideInfo*) __asm__("cRider_flagEventWipeout");
extern "C" void cRider_doBump_7E70(char*, tCollideInfo*, int) __asm__("cRider_doBump");
extern "C" void cRider_flagEventPunchKnockdown(void*, void*);
extern "C" void cRider_flagEventBumpKnockdown(void*, void*);
extern "C" void cRider_flagEventPunch(void*, void*);
extern "C" void cRider_flagEventBump(void*, void*);
extern float kHighSpeedCollideThresh_7E70 __asm__("kHighSpeedCollideThresh");
extern int D_005308D0_7E70[] __asm__("D_005308D0");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_7E70(const t4Vector& a, const t4Vector& b)
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
static inline float Len_7E70(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_7E70(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a += b).
static inline void AddEq_7E70(t4Vector& a, const t4Vector& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_7E70(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_7E70(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector Norm_7E70(const t4Vector& v)
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

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_7E70(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float Clamp_7E70(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

static inline t4Vector Get_7E70(char* p) { return *(t4Vector*)p; }

/**
 * Applies a rider-vs-rider shove or punch to this rider and picks a bump or a knockdown.
 *
 * Ignored in CS_FinishLine. Adds @p n x clamp(@p speed, +-555.6) to mVelocity (flattened onto mGroundNormal when it is
 * valid); in the air it relaunches the air predictor and ignores a push along the up axis. No reaction follows in
 * MS_Wipeout, CS_Reset or below 40 speed.
 *
 * Above 599.97 it picks a knockdown animation (kAID_WH_Grnd*) from the push direction against the rider's axes for
 * cRider_flagEventWipeout (flagEventWipeout) and flags cRider_flagEventPunchKnockdown (punch knockdown) or
 * cRider_flagEventBumpKnockdown (bump knockdown); otherwise it bumps (cRider_doBump) and flags cRider_flagEventPunch
 * (punch) or cRider_flagEventBump (bump). Rules bit 5 (D_005308D0) sets the speed to exactly 599.97 and the test is
 * strict, so that rule turns every knockdown into a bump. Called by cRider_checkOpponentCollisions.
 *
 * @param other  the other rider
 * @param n  the unit push direction
 * @param flag  nonzero for a punch (On Tour isPunch), 0 for a collision
 * @param speed  the signed impulse along @p n (On Tour impulse)
 */
void cRider::doOpponentCollision(cRider* other, t4Vector* n, int flag, float speed)
{
    if (cRider_getControlState_7E70((char*)this) == CS_FinishLine)
        return;
    float absSpd = Abs_7E70(speed);
    float lim = 555.5555419921875f;
    float clampedSpeed = Clamp_7E70(speed, -lim, lim);
    t4Vector imp = Scale_7E70(*n, clampedSpeed);
    if (cRider_isGroundNormalValid((char*)this)) {
        AddEq_7E70(*(&mVelocity),
                   Sub_7E70(imp, Scale_7E70(*(&mGroundNormal), Dot_7E70(imp, *(&mGroundNormal)))));
    } else {
        AddEq_7E70(*(&mVelocity), imp);
        if (cRider_getMotionState((char*)this) == MS_Air || (cRider_getMotionState((char*)this) == MS_Wipeout
            && mRiderHandler->WipeoutMotion.WipeoutMotionState == cWipeoutMotion::kWMS_Air)) {
            cAI_7E70* ai = (cAI_7E70*)(&VPTR_AT(this, sizeof(cRiderState)));
            t4Vector* pos = ai->GetPos();
            cAirPredictor_startLaunchIntoAir(mAirPredictor, pos, ai->GetDir(), TerminalVelocity);
            if (Dot_7E70(*n, Get_7E70((char*)&mMatrix.row[2])) * clampedSpeed > 0.0f)
                absSpd = 0.0f;
        }
    }
    if (cRider_getMotionState((char*)this) == MS_Wipeout)
        return;
    if (cRider_getControlState_7E70((char*)this) == CS_Reset)
        return;
    if (absSpd < 39.99532699584961f)
        return;
    float otherDist = mRelations[other->mAIIndex].mDistance;
    tCollideInfo info;
    info.Pos = Add_7E70(*(&mPosition), Scale_7E70(*n, otherDist));
    info.Direct = Norm_7E70(*(&mVelocity));
    info.Reflect = *n;
    info.Vel = clampedSpeed;
    if ((D_005308D0_7E70[0] >> 5) & 1)
        absSpd = 599.9739990234375f;
    if (absSpd > 599.9739990234375f) {
        float dz = Dot_7E70(Get_7E70((char*)&RiderRoot), *n);
        float dy = Dot_7E70(Get_7E70((char*)&RiderRoot.row[1]), *n);
        float dx = Dot_7E70(Get_7E70((char*)&RiderRoot.row[2]), *n);
        int anim;
        if (Abs_7E70(dz) < Abs_7E70(dy) && Abs_7E70(dx) < Abs_7E70(dy)) {
            if (dy < 0.0f) {
                unsigned int k;
                if (Len_7E70(*(&mVelocity)) > kHighSpeedCollideThresh_7E70)
                    k = (unsigned int)AIrand() % 6;
                else
                    k = (unsigned int)AIrand() % 3;
                if (k == 0)
                    anim = kAID_WH_GrndFromFront2FwdFaceUp;
                else if (k == 1)
                    anim = kAID_WH_GrndFromFront2BwdFaceUp;
                else if (k == 2)
                    anim = kAID_WH_GrndFromFront2BwdFaceDown;
                else if (k == 3)
                    anim = kAID_WH_GrndFromFront2CartBwd;
                else
                    anim = kAID_WH_GrndFromFront2RollBwd;
            } else {
                anim = (AIrand() & 1) == 0 ? kAID_WH_GrndFromBack2RollFwd : kAID_WH_GrndFromBack2FwdFaceDown;
            }
        } else if (Abs_7E70(dz) < Abs_7E70(dx) && Abs_7E70(dy) < Abs_7E70(dx)) {
            anim = kAID_WH_GrndFromBack2FwdFaceDown;
        } else if (dz < 0.0f) {
            anim = kAID_WH_GrndFromRight2RollLeft;
        } else {
            anim = kAID_WH_GrndFromLeft2RollRight;
        }
        cRider_flagEventWipeout_7E70(((char*)this), anim, flag, 0, &info);
        if (flag)
            cRider_flagEventPunchKnockdown(((char*)this), ((char*)other));
        else
            cRider_flagEventBumpKnockdown(((char*)this), ((char*)other));
    } else {
        cRider_doBump_7E70(((char*)this), &info, 0);
        if (flag)
            cRider_flagEventPunch(((char*)this), ((char*)other));
        else
            cRider_flagEventBump(((char*)this), ((char*)other));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_doBump);
#ifdef SKIP_ASM

int cRider_getMotionState(void*);
int cRider_getControlState_8388(void*) __asm__("cRider_getControlState__FPv");
void cRider_setControlState_8388(void* self, int v) __asm__("cRider_setControlState__FPv");
int AIrand();
extern "C" void cBoardPressControl_forceExit(void*);
extern "C" void cRiderAnimBase_play(void*, int, int, float);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_SpeakBump(void*, void*, int);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_8388(const t4Vector& v)
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
static inline float Dot_8388(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_8388(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline t4Vector Get_8388(char* p) { return *(t4Vector*)p; }

/**
 * Plays a stumble (bump) animation for a glancing collision.
 *
 * Only in MS_Ground or MS_Railslide, and only in CS_Cruise, CS_BoardPress (ended by cBoardPressControl_forceExit on the
 * handler's +0x1D0), CS_Prewind or CS_Railslide. A contact normal (info +0x20) along the front axis (+0x1B0) plays
 * kAID_BumpFromBehind or kAID_BumpFromFront. Sideways (side axis +0x1A0, mirrored when riding fakie, +0x320) a slow
 * contact (normal speed < 1111.1) at over 1388.9 speed off MS_Railslide adds a random number of half turns to mSpinRate
 * and plays kAID_BumpHardFromTS / kAID_BumpHardFromHS; otherwise kAID_BumpFromTS / kAID_BumpFromHS. With @p flag it
 * plays the bump sound (cSSXAudio_SpeakBump). Ends in CS_Bump.
 *
 * @param info  the collide info (contact normal at +0x20, normal speed at +0x30)
 * @param flag  nonzero to play the bump sound (On Tour isObject)
 */
void cRider::doBump(tCollideInfo* info, int flag)
{
    if (cRider_getMotionState((char*)this) != MS_Ground && cRider_getMotionState((char*)this) != MS_Railslide)
        return;
    if (cRider_getControlState_8388((char*)this) == CS_BoardPress) {
        cBoardPressControl_forceExit(&mRiderHandler->BoardPressControl);
    } else if (cRider_getControlState_8388((char*)this) != CS_Cruise
               && cRider_getControlState_8388((char*)this) != CS_Prewind
               && cRider_getControlState_8388((char*)this) != CS_Railslide) {
        return;
    }
    int played = 0;
    float frontDot = Dot_8388(Get_8388((char*)&mMatrix.row[1]), ((const tCollideInfo*)info)->Reflect);
    if (frontDot > 0.7071067690849304f) {
        cRiderAnimBase_play(mRiderAnim, kAID_BumpFromBehind, 0, -1.0f);
    } else if (frontDot < -0.7071067690849304f) {
        cRiderAnimBase_play(mRiderAnim, kAID_BumpFromFront, 0, -1.0f);
    } else {
        float side = Dot_8388(Get_8388((char*)&mMatrix), ((const tCollideInfo*)info)->Reflect);
        if (IsRidingFakie)
            side = -side;
        if (((const tCollideInfo*)info)->Vel < 1111.111083984375f && Len_8388(*(&mVelocity)) > 1388.888916015625f &&
            cRider_getMotionState((char*)this) != MS_Railslide) {
            float spinRate = mSpinRate;
            if (Abs_8388(spinRate) < 6.2831854820251465f) {
                if (spinRate < 0.0f)
                    mSpinRate += (int)((unsigned int)AIrand() % 3) * -3.1415927410125732f;
                else if (spinRate > 0.0f)
                    mSpinRate += (int)((unsigned int)AIrand() % 3) * 3.1415927410125732f;
                else
                    mSpinRate += ((int)((unsigned int)AIrand() % 5) - 2) * 3.1415927410125732f;
            }
            cRiderAnimBase_play(mRiderAnim, side < 0.0f ? kAID_BumpHardFromTS : kAID_BumpHardFromHS, 0, -1.0f);
            played = 1;
        } else {
            cRiderAnimBase_play(mRiderAnim, side < 0.0f ? kAID_BumpFromTS : kAID_BumpFromHS, 0, -1.0f);
        }
    }
    if (flag)
        cSSXAudio_SpeakBump(cAudioInterface_bxAudio(), ((char*)this), played);
    cRider_setControlState_8388(((char*)this), CS_Bump);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_isRailBoardIntersect);
#ifdef SKIP_ASM

extern "C" int cOverlapList_splineIntersect_86B8(void*, t4Vector*, t4Vector*, int, float)
    __asm__("cOverlapList_splineIntersect");

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4VectorInit Add_86B8(const t4Vector& a, const t4Vector& b)
{
    t4VectorInit r;
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
static inline t4VectorInit Sub_86B8(const t4Vector& a, const t4Vector& b)
{
    t4VectorInit r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4VectorInit Scale_86B8(const t4Vector& v, float s)
{
    t4VectorInit r;
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

// PORT: PS2-only VU0 inline asm (vector divided by scalar).
static inline t4VectorInit Div_86B8(const t4Vector& v, float s)
{
    t4VectorInit r;
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_86B8(const t4Vector& v)
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
static inline float Dot_86B8(const t4Vector& a, const t4Vector& b)
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

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float Clamp_86B8(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

static inline float Lerp_86B8(float a, float b, float t)
{
    return t * a + (1.0f - t) * b;
}

static inline t4VectorInit Fwd_86B8(const sAnimModelBone* n)
{
    return t4VectorInit(1.0f - (n->rot.y * n->rot.y + n->rot.z * n->rot.z) * 2.0f,
                    (n->rot.x * n->rot.y + n->rot.w * n->rot.z) * 2.0f,
                    (n->rot.x * n->rot.z - n->rot.w * n->rot.y) * 2.0f,
                    0.0f);
}

/**
 * Tells whether the rider's board reaches a rail.
 *
 * The board's axis and position come from the animation model's board-root bone (mAnimModel, mBoneBoardRoot; axis from
 * the bone's quaternion, position plus mModelOffset). With the blend mSlewed[9].cur, the reach is lerp(30, 50) and the
 * half length lerp(100, 170) (lerp(50, 170) when PressMode is set), less the reach. It returns true when the board
 * segment's point nearest @p tgt is within reach. Otherwise, unless @p dir is almost parallel to the board (1 - d * d <
 * 0.001), it takes the board point nearest the rail line, finds the spline nearest it (cOverlapList_splineIntersect,
 * mOverlapList, range 300) and returns true when that point is within reach.
 *
 * @param tgt  the rail point
 * @param dir  the rail direction
 * @return true when the board reaches the rail, else false
 */
bool cRider::isRailBoardIntersect(t4Vector* tgt, t4Vector* dir)
{
    sAnimModelBone* n = &(mAnimModel->bones)[mBoneBoardRoot];
    const t4Vector& fwd = Fwd_86B8(n);
    t4VectorInit pos = Add_86B8(n->pos, mModelOffset);
    float blend = mSlewed[9].cur;
    float nearR = Lerp_86B8(50.0f, 30.0f, blend);
    float farR;
    if (!PressMode)
        farR = Lerp_86B8(170.0f, 100.0f, blend);
    else
        farR = Lerp_86B8(170.0f, 50.0f, blend);
    float proj = Dot_86B8(fwd, Sub_86B8(*tgt, pos));
    float lo = nearR - farR;
    float hi = farR - nearR;
    t4VectorInit diff = Sub_86B8(Add_86B8(pos, Scale_86B8(fwd, Clamp_86B8(proj, lo, hi))), *tgt);
    if (Len_86B8(diff) <= nearR)
        return true;
    float dd = Dot_86B8(*dir, fwd);
    float den = 1.0f - dd * dd;
    if (den < 0.0010000000474974513f)
        return false;
    t4VectorInit relScaled = Div_86B8(Sub_86B8(pos, *tgt), den);
    float along = Dot_86B8(relScaled, Sub_86B8(Scale_86B8(*dir, dd), fwd));
    t4VectorInit probePos = Add_86B8(pos, Scale_86B8(fwd, Clamp_86B8(along, lo, hi)));
    tIntersectInfo hit;
    if (!cOverlapList_splineIntersect_86B8(mOverlapList, &probePos, &hit.Ipos, 1, 300.0f))
        return false;
    diff = Sub_86B8(probePos, hit.Ipos);
    return Len_86B8(diff) <= nearR;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cRider_isRailLandSuccess);
#ifdef SKIP_ASM



// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vAdd_8A48(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector vSub_8A48(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_8A48(const t4Vector& v)
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
static inline float vDot_8A48(const t4Vector& a, const t4Vector& b)
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
extern "C" int cOverlapList_splineIntersect(void*, t4Vector*, t4Vector*, int, float);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" cAnimSequence* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" int tEventFlags_isSet1(void*, int);
extern "C" int cRider_isRailBoardIntersect(void*, t4Vector*, t4Vector*);

/**
 * Tests whether the rider's board can land on a nearby rail.
 *
 * Only on the ground or in the air (MS_Ground and MS_Air). It probes for a rail (cOverlapList_splineIntersect, type 1,
 * radius 300) from the board root bone (mAnimModel bone mBoneBoardRoot + mModelOffset) and rejects rails behind the
 * motion. In channel 2 animation categories kANCAT_Grab .. kANCAT_Uber the current sequence's flags (tEventFlags_isSet1
 * on cRiderAnimBase_getCurrentAnimSequence + 0xB0) must allow it. The answer is cRider_isRailBoardIntersect (On Tour
 * isRailBoardIntersect) on the hit. Callers: cRider_checkRailBoardCollisions and cRider_checkRailBodyCollisions.
 *
 * @param hit  receives the rail intersection (Ipos; Inorm is the rail direction)
 * @return nonzero when the board lands on the rail
 */
int cRider::isRailLandSuccess(tIntersectInfo* hit)
{
    if (cRider_getMotionState((char*)this) != MS_Ground && cRider_getMotionState((char*)this) != MS_Air)
        return 0;
    sAnimModelBone* boardRoot = &mAnimModel->bones[mBoneBoardRoot];
    t4Vector boardCenter = vAdd_8A48(boardRoot->pos, *(&mModelOffset));
    if (!cOverlapList_splineIntersect(mOverlapList, &boardCenter, &hit->Ipos, 1, 300.0f))
        return 0;
    float spd = vLen_8A48(*(&mVelocity));
    if (0.001f < spd) {
        t4Vector toRail = vSub_8A48(hit->Ipos, boardCenter);
        if (vDot_8A48(toRail, *(&mVelocity)) < spd * -0.20000000298023224f)
            return 0;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 2) == kANCAT_Grab) {
        if (!tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 2)->Events, 2) &&
            tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 2)->Events, 0))
            return 0;
    } else if (cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 2) == kANCAT_Tweak ||
               cRiderAnimBase_getCurrentAnimCategory(mRiderAnim, 2) == kANCAT_Uber) {
        if (!tEventFlags_isSet1(&cRiderAnimBase_getCurrentAnimSequence(mRiderAnim, 2)->Events, 2))
            return 0;
    }
    return cRider_isRailBoardIntersect(((char*)this), &hit->Ipos, &hit->Inorm);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_isIntersecting);
#ifdef SKIP_ASM
/**
 * Tells whether the rider's last static-object test hit an instance.
 *
 * Compares the instance's id (m_RID) with the ids in mVisitedIds that cRider_objectIntersect records (at most 64, ended
 * by kInvalidRID). Virtual slot 3 (word 7) of the rider vtables.
 *
 * @param instance  the instance
 * @return true when the id is listed, else false
 */
bool cRider::isIntersecting(tInstanceRecord* instance)
{
    int i = 0;
    if (~mVisitedIds[0]) {
        do {
            if (mVisitedIds[i] == instance->m_RID) {
                return true;
            }
            i++;
        } while (i < 64 && mVisitedIds[i] != cRiderState::kInvalidRID);
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_cComputer);
#ifdef SKIP_ASM
extern "C" void cRider_cRider(cRider* self);
extern "C" void* cRiderHandler_cRiderHandler(void* self, int inChrg);
// PORT: the unit declares cComputer_Init as (void); this caller passes self.
extern "C" void cComputer_Init_self(void* self) __asm__("cComputer_Init");

struct sVEntry_108C80 { short delta; short index; void* fn; };
struct sVtbl9_108C80 { sVEntry_108C80 e[9]; } __attribute__((aligned(8)));
struct sVtbl22_108C80 { sVEntry_108C80 e[22]; } __attribute__((aligned(8)));

extern const sVtbl9_108C80 D_00458618;
extern char cRider_cScriptObjRider_vtbl[];
extern const sVtbl22_108C80 D_00458660;
extern char cComputer_vtbl[];

/**
 * Constructs a computer rider: its cRiderHandler parts, the cRider virtual base and cComputer's vtables.
 *
 * In charge (both cAI callers pass 1) it points all 31 part virtual-base pointers at the cRider at +0xF50 and
 * constructs it. It then constructs cRiderHandler not in charge, installs cComputer's three cRider vtables
 * (not in charge: patched copies on its own stack) and its own vtable, sets mIsActor when @p actor is set,
 * and runs cComputer's Init (cComputer_Init).
 *
 * @param inChrg  g++ in-charge flag: nonzero when this is the complete object that owns the cRider base
 * @param actor  nonzero for a podium or lodge actor (On Tour isActor)
 * @return this object
 */
// PORT: g++ 2.95 most-derived constructor of a class deriving virtually from cRider (vbase at +0xF50,
// vbase pointers in every subobject); the not-in-charge vtable fix-up is written out by hand.
extern "C" char* cComputer_cComputer(cComputer* self, int inChrg, int actor)
{
    if (inChrg)
    {
        cRider* vb = (cRider*)(&self->Rider);
        self->RiderHandler.CheatFX.mRider = vb;
        self->RiderHandler.PunchFX.mRider = vb;
        self->RiderHandler.DynamicSprayFX.mRider = vb;
        self->RiderHandler.PlayerIndicatorFX.mRider = vb;
        self->RiderHandler.AngerFX.mRider = vb;
        self->RiderHandler.AirStreamerFX.mRider = vb;
        self->RiderHandler.TrickBoostFX.mRider = vb;
        self->RiderHandler.SpeedBoostFX.mRider = vb;
        self->RiderHandler.BoardTrailFX.mRider = vb;
        self->RiderHandler.BoardSparkFX.mRider = vb;
        self->RiderHandler.BoardWakeFX.mRider = vb;
        self->RiderHandler.VoidControl.mRider = vb;
        self->RiderHandler.UberGrindControl.mRider = vb;
        self->RiderHandler.HandplantControl.mRider = vb;
        self->RiderHandler.FinishLineControl.mRider = vb;
        self->RiderHandler.ResetControl.mRider = vb;
        self->RiderHandler.WipeoutControl.mRider = vb;
        self->RiderHandler.RailslideControl.mRider = vb;
        self->RiderHandler.GateControl.mRider = vb;
        self->RiderHandler.SpinControl.mRider = vb;
        self->RiderHandler.NaturalAirControl.mRider = vb;
        self->RiderHandler.BumpControl.mRider = vb;
        self->RiderHandler.PrewindControl.mRider = vb;
        self->RiderHandler.BoardPressControl.mRider = vb;
        self->RiderHandler.CruiseControl.mRider = vb;
        self->RiderHandler.HandplantMotion.mRider = vb;
        self->RiderHandler.RailslideMotion.mRider = vb;
        self->RiderHandler.StaticMotion.mRider = vb;
        self->RiderHandler.WipeoutMotion.mRider = vb;
        self->RiderHandler.AirMotion.mRider = vb;
        self->RiderHandler.GroundMotion.mRider = vb;
        cRider_cRider(vb);
    }
    cRiderHandler_cRiderHandler(((char*)self), 0);
    self->RiderHandler.GroundMotion.mRider->mScriptObjectVtbl = (void*)&D_00458618;
    self->RiderHandler.GroundMotion.mRider->mScriptObjRiderVtbl = cRider_cScriptObjRider_vtbl;
    VPTR_AT(self->RiderHandler.GroundMotion.mRider, sizeof(cRiderState)) = (void**)&D_00458660;
    if (!inChrg)
    {
        sVtbl9_108C80 ctorVtbl9 = D_00458618;
        self->RiderHandler.GroundMotion.mRider->mScriptObjectVtbl = &ctorVtbl9;
        char* base = (char*)self->RiderHandler.GroundMotion.mRider - 0xF50;
        int vbOffset = ((char*)self) - base;
        ctorVtbl9.e[1].delta = D_00458618.e[1].delta + vbOffset;
        sVtbl22_108C80 ctorVtbl22 = D_00458660;
        VPTR_AT(self->RiderHandler.GroundMotion.mRider, sizeof(cRiderState)) = (void**)&ctorVtbl22;
        ctorVtbl22.e[1].delta = D_00458660.e[1].delta + vbOffset;
        ctorVtbl22.e[20].delta = D_00458660.e[20].delta + vbOffset;
    }
    self->RiderHandler.mVtbl = (void**)cComputer_vtbl;
    if (actor)
        self->RiderHandler.GroundMotion.mRider->mIsActor = 1;
    cComputer_Init_self((char*)self);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_Init);
#ifdef SKIP_ASM
extern t4Vector t4vect_Origin;
extern "C" void cComputer_initRiderDifficulty(void* self);
extern "C" void cComputer_resetCounters(void* self);
extern "C" void* MEM_fill_8E88(void* dst, int value, int size) __asm__("MEM_fill");

// PORT: the unit later declares cComputer_Init without parameters; it takes the rider.
void cComputer_Init_self(char* self) __asm__("cComputer_Init");

/**
 * Resets a computer rider's AI state.
 *
 * Clears its trick flags, timers and counters (mGrabID and later mTargetID = -1), zeroes mStartJumpPos and
 * mEndJumpPos, runs cComputer_initRiderDifficulty, sets mAccuracy = 1.0, installs the cruise logic member-function
 * pointer (cruiseLogic = cComputer_mCruiseLogic[1]; On Tour cComputer::mCruiseLogic) with mCruiseLogicID = 1, clears
 * the three 0x3C-byte history tables (GrabHistoryCounter, UberHistoryCounter, TweakHistoryCounter; MEM_fill) and runs
 * cComputer_resetCounters. Called at the end of cComputer_cComputer and by func_00108F88.
 */
void cComputer::Init()
{
    mGrabID = -1;
    mIsSpinTrick = 0;
    mIsGonnaMessUp = 0;
    mIsNaturalAir = 0;
    mToRail = 0;
    mIsUber = 0;
    mIsTweak = 0;
    mToPipe = 0;
    mHoldToLandTime = 0.0f;
    mSpinAxis = 0.0f;
    mFlipAxis = 0.0f;
    mGrabStartTime = 0.0f;
    mGrabEndTime = 0.0f;
    mDesiredSpeed = 0.0f;
    mWanderTime = 0;
    mBadHeadingTime = 0;
    mCruiseLogicID = 0;
    mBlockTimer = 0.0f;
    mShouldIBlock = 0;
    mStartJumpPos = t4vect_Origin;
    mEndJumpPos = t4vect_Origin;
    mBoardPressTimer = 0;
    cComputer_initRiderDifficulty(this);
    mTargetID = -1;
    mAccuracy = 1.0f;
    cruiseLogic = cComputer_mCruiseLogic[1];
    mCruiseLogicID = 1;
    do {
        MEM_fill_8E88(&GrabHistoryCounter[0], 0, 0x3C);
        MEM_fill_8E88(&UberHistoryCounter[0], 0, 0x3C);
        MEM_fill_8E88(&TweakHistoryCounter[0], 0, 0x3C);
    } while (0);
    cComputer_resetCounters(this);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", func_00108F88);
#ifdef SKIP_ASM
extern "C" void cComputer_Init();

/**
 * Calls cComputer_Init, passing its own first argument register through (the C declares no parameters).
 *
 * Nothing in the build references it.
 */
extern "C" void func_00108F88(void) {
    cComputer_Init();
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer__cComputer);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "seg/seg_1218_classes.h"
#include "replay/bxreplayframe.h"
#include "object/being.h"

/**
 * Destroys a computer rider (compiler-generated destructor).
 *
 * Destroys the object's parts in reverse order (calling the out-of-line destructors of the FX and wipeout-motion parts,
 * inlining the others), then the cRider virtual base (cRider__cRider) when called for the most-derived object, and
 * frees the memory (operator_delete__FPi) when the delete flag is set. It fills the destructor slot of the vtables
 * D_00458618 and D_00458660.
 */
// Compiler-generated: the composite's parts are destroyed in reverse order (the out-of-line part destructors
// are called, the others are inlined), then the cRider virtual base when this is the most-derived object.
// Needs -fno-rtti for this unit: the retail vtables carry no typeinfo pointer, and every level here stores
// cRider's own 0x6D0 vtable (cRider_cScriptObjRider_vtbl), which g++ only shares between classes without RTTI.
cRiderE_108C80::~cRiderE_108C80()
{
}

// PORT: the compiler names these dtors _$_<len><Class>; map them to the address-derived names.
__asm__(".globl cComputer__cComputer\n.type cComputer__cComputer,@function\ncComputer__cComputer = _$_14cRiderE_108C80\n.size cComputer__cComputer,0x17C0\n");

__asm__("_$_12cRiderVb_s12 = cRider__cRider\n"
        "_$_15cCompPart02_s12 = cWipeoutMotion__cWipeoutMotion\n"
        "_$_15cCompPart20_s12 = cBoardWakeFX__cBoardWakeFX\n"
        "_$_15cCompPart21_s12 = cBoardSparkFX__cBoardSparkFX\n"
        "_$_15cCompPart22_s12 = cBoardTrailFX__cBoardTrailFX\n"
        "_$_15cCompPart23_s12 = cSpeedBoostFX__cSpeedBoostFX\n"
        "_$_15cCompPart24_s12 = cTrickBoostFX__cTrickBoostFX\n"
        "_$_15cCompPart25_s12 = cAirStreamerFX__cAirStreamerFX\n"
        "_$_15cCompPart26_s12 = cAngerFX__cAngerFX\n"
        "_$_15cCompPart27_s12 = cPlayerIndicatorFX__cPlayerIndicatorFX\n"
        "_$_15cCompPart28_s12 = cDynamicSprayFX__cDynamicSprayFX\n"
        "_$_15cCompPart29_s12 = cPunchFX__cPunchFX\n"
        "_$_15cCompPart30_s12 = cCheatFX__cCheatFX\n");

// PORT: the retail names of the vtables and dtors g++ references here.
__asm__("_vt$14cRiderE_108C80 = cComputer_vtbl\n"
        "_vt$14cRiderE_108C80$13cRiderVbC_s12 = D_00458618\n"
        "_vt$14cRiderE_108C80$12cRiderVb_s12 = D_00458660\n"
        "_vt$9cComp_s12 = cRiderHandler_vtbl\n"
        "_vt$9cComp_s12$13cRiderVbC_s12 = D_00458738\n"
        "_vt$14cRiderVbB0_s12 = cRider_cScriptObjRider_vtbl\n"
        "_vt$9cComp_s12$12cRiderVb_s12 = D_00458780\n"
        "_vt$15cCompPart19_s12$13cRiderVbC_s12 = D_00458928\n"
        "_vt$15cCompPart19_s12$12cRiderVb_s12 = D_00458970\n"
        "_vt$15cCompPart18_s12$13cRiderVbC_s12 = D_00458D08\n"
        "_vt$15cCompPart18_s12$12cRiderVb_s12 = D_00458D50\n"
        "_vt$15cCompPart17_s12$13cRiderVbC_s12 = D_00458830\n"
        "_vt$15cCompPart17_s12$12cRiderVb_s12 = D_00458878\n"
        "_vt$15cCompPart16_s12$13cRiderVbC_s12 = D_00458B18\n"
        "_vt$15cCompPart16_s12$12cRiderVb_s12 = D_00458B60\n"
        "_vt$15cCompPart15_s12$13cRiderVbC_s12 = D_00458A20\n"
        "_vt$15cCompPart15_s12$12cRiderVb_s12 = D_00458A68\n"
        "_vt$15cCompPart14_s12$13cRiderVbC_s12 = D_00458C10\n"
        "_vt$15cCompPart14_s12$12cRiderVb_s12 = D_00458C58\n"
        "_vt$15cCompPart13_s12$13cRiderVbC_s12 = D_00458E00\n"
        "_vt$15cCompPart13_s12$12cRiderVb_s12 = D_00458E48\n"
        "_vt$15cCompPart12_s12$13cRiderVbC_s12 = D_00458EF8\n"
        "_vt$15cCompPart12_s12$12cRiderVb_s12 = D_00458F40\n"
        "_vt$15cCompPart11_s12$13cRiderVbC_s12 = D_00458FF0\n"
        "_vt$15cCompPart11_s12$12cRiderVb_s12 = D_00459038\n"
        "_vt$15cCompPart10_s12$13cRiderVbC_s12 = D_004590E8\n"
        "_vt$15cCompPart10_s12$12cRiderVb_s12 = D_00459130\n"
        "_vt$15cCompPart09_s12$13cRiderVbC_s12 = D_004591E0\n"
        "_vt$15cCompPart09_s12$12cRiderVb_s12 = D_00459228\n"
        "_vt$15cCompPart08_s12$13cRiderVbC_s12 = D_004592D8\n"
        "_vt$15cCompPart08_s12$12cRiderVb_s12 = D_00459320\n"
        "_vt$15cCompPart07_s12$13cRiderVbC_s12 = D_004593D0\n"
        "_vt$15cCompPart07_s12$12cRiderVb_s12 = D_00459418\n"
        "_vt$15cCompPart06_s12$13cRiderVbC_s12 = D_004594C8\n"
        "_vt$15cCompPart06_s12$12cRiderVb_s12 = D_00459510\n"
        "_vt$15cCompPart05_s12$13cRiderVbC_s12 = D_004595C0\n"
        "_vt$15cCompPart05_s12$12cRiderVb_s12 = D_00459608\n"
        "_vt$15cCompPart04_s12$13cRiderVbC_s12 = D_004598A8\n"
        "_vt$15cCompPart04_s12$12cRiderVb_s12 = D_004598F0\n"
        "_vt$15cCompPart03_s12$13cRiderVbC_s12 = D_004596B8\n"
        "_vt$15cCompPart03_s12$12cRiderVb_s12 = D_00459700\n"
        "_vt$15cCompPart01_s12$13cRiderVbC_s12 = D_004599A0\n"
        "_vt$15cCompPart01_s12$12cRiderVb_s12 = D_004599E8\n"
        "_vt$15cCompPart00_s12$13cRiderVbC_s12 = D_00459A98\n"
        "_vt$15cCompPart00_s12$12cRiderVb_s12 = D_00459AE0\n");
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getControlCmd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void MEM_fill(void* dst, int value, int size);
extern "C" float cComputer_setupWarp(void* self);
extern "C" void cRider_updateTimeWarp(void* self, float v);
int cRider_getControlState_A768(void*) __asm__("cRider_getControlState__FPv");
// cComputer_getCruiseCmd's own self is still a view class (it calls the cruise logic pointer-to-member).
extern "C" void cComputer_getCruiseCmd_A768(cComputer* self, tCruiseCmd* cmd) __asm__("cComputer_getCruiseCmd");
extern "C" void cComputer_getPrewindCmd(cComputer* self, tPrewindCmd* cmd);
extern "C" void cComputer_getGateCmd(cComputer* self, tGateCmd* cmd);
extern "C" void cComputer_getNaturalAirCmd(cComputer* self, tNaturalAirCmd* cmd);
extern "C" void cComputer_getRailslideCmd(cComputer* self, tRailslideCmd* cmd);
extern "C" int cComputer_getSpinCmd(cComputer* self, tSpinCmd* cmd);
extern "C" void cComputer_getWipeoutCmd(cComputer* computer, tWipeoutCmd* controlCmd);
extern "C" void cComputer_getBoardPressCmd(cComputer* self, tBoardPressCmd* cmd);

/**
 * Builds this computer rider's control command for the frame, the AI's stand-in for reading a pad.
 *
 * Zeroes the 8-byte command, applies the rubber-band time warp (cRider_updateTimeWarp() with cComputer_setupWarp's
 * factor), then fills it with the get*Cmd for the rider's control state: cruise (cComputer_getCruiseCmd), board press,
 * prewind, natural air, spin, gate, railslide or wipeout (cComputer_getWipeoutCmd); other states leave it empty.
 * Outside CS_Spin it clears mGrabEndTime.
 *
 * @param _cmd  the command to fill (On Tour tControlCmd&)
 */
// Clears the pad input, updates the rider, then runs the input handler for the rider state.
void cComputer::getControlCmd(tControlCmd* _cmd)
{
    MEM_fill(_cmd, 0, 8);
    cRider_updateTimeWarp(RiderHandler.GroundMotion.mRider, cComputer_setupWarp((char*)this));
    int state = cRider_getControlState_A768(RiderHandler.GroundMotion.mRider);
    switch (state) {
    case CS_Cruise:
        cComputer_getCruiseCmd_A768(this, &_cmd->CruiseCmd);
        break;
    case CS_Prewind:
        getPrewindCmd(&_cmd->PrewindCmd);
        break;
    case CS_Gate:
        getGateCmd(&_cmd->GateCmd);
        break;
    case CS_NaturalAir:
        getNaturalAirCmd(&_cmd->NaturalAirCmd);
        break;
    case CS_Railslide:
        getRailslideCmd(&_cmd->RailslideCmd);
        break;
    case CS_Spin:
        getSpinCmd(&_cmd->SpinCmd);
        break;
    case CS_Wipeout:
        getWipeoutCmd(&_cmd->WipeoutCmd);
        break;
    case CS_BoardPress:
        getBoardPressCmd(&_cmd->BoardPressCmd);
        break;
    case CS_Bump: case CS_Reset: case CS_FinishLine: case CS_Handplant: case CS_UberGrind: case CS_Void:
        break;
    }
    if (state != CS_Spin) {
        mGrabEndTime = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_writeToReplayFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRiderHandler_writeToReplayFrame_A898(char*, void*) __asm__("cRiderHandler_writeToReplayFrame");

/**
 * Writes the AI rider into a replay frame: the rider handler state, then the computer state.
 *
 * Calls cRiderHandler_writeToReplayFrame, then the stream's first virtual (write) with the cComputerState block at
 * +0xDF0 (336 bytes, up to +0xF40: it includes mCruiseLogicID at +0xF38 but not the cruise-logic pointer at +0xF44,
 * which cComputer_readFromReplayFrame rebuilds).
 *
 * @param data      the replay frame stream written to (On Tour cReplayFramePtr* data)
 */
void cComputer::writeToReplayFrame(cBxReplayFrame* data)
{
    cRiderHandler_writeToReplayFrame_A898(((char*)this), data);
    data->write((const char*)&mDesiredSpeed, 336);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" cComputer::tCruiseLogicFn cComputer_mCruiseLogic_A8E8[4] __asm__("cComputer_mCruiseLogic");
extern "C" void cRiderHandler_readFromReplayFrame(void* self, void* src);

/**
 * Reads the AI rider back from a replay frame: the rider handler state, then the computer state.
 *
 * Calls cRiderHandler_readFromReplayFrame, then the stream's second virtual (read) into the cComputerState block at
 * +0xDF0 (336 bytes), and rebuilds the cruise-logic member-function pointer (+0xF44) from cComputer_mCruiseLogic[mCruiseLogicID]
 * (the short at +0xF38), as cComputer_setCruiseLogic does, since the saved pointer is not portable between runs.
 *
 * @param data  the replay frame stream read from (On Tour cReplayFramePtr* data)
 */
void cComputer::readFromReplayFrame(cBxReplayFrame* data)
{
    cRiderHandler_readFromReplayFrame(((char*)this), data);
    data->read((char*)&mDesiredSpeed, 336);
    short state = mCruiseLogicID;
    cruiseLogic = cComputer_mCruiseLogic_A8E8[state];
    mCruiseLogicID = state;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getCruiseCmd);
#ifdef SKIP_ASM
class cObj_A960;
typedef void (cObj_A960::*Fn_A960)(tCruiseCmd*);

class cObj_A960 {
public:
    virtual void v0();
    char pad[0xE20];
    int fE20;
    char padE24[0xF44 - 0xE24];
    Fn_A960 state;
};

extern "C" int cComputer_checkForReset(void*);
extern "C" int cComputer_checkForBlock(void*);
extern "C" int cComputer_checkForPunching(void*, int*);

/**
 * Fills the computer rider's cruise command.
 *
 * Sets bit 12 of the command from cComputer_checkForReset and clears +0xE20. When cComputer_checkForPunching finds a
 * punch it sets bit 19 (its flag set) or bit 18 (flag clear); otherwise when cComputer_checkForBlock is true it sets
 * both. Then it calls the current cruise logic, the pointer-to-member at +0xF44 (set from cComputer_mCruiseLogic), with the
 * command. cComputer_getControlCmd calls it.
 *
 * @param cruiseCmd  the cruise command
 */
extern "C" void cComputer_getCruiseCmd(cObj_A960* self, tCruiseCmd* cruiseCmd)
{
    int punchRight;
    cruiseCmd->Reset = cComputer_checkForReset(self);
    self->fE20 = 0;
    if (cComputer_checkForPunching(self, &punchRight)) {
        if (punchRight)
            cruiseCmd->AttackRight = 1;
        else
            cruiseCmd->AttackLeft = 1;
    } else if (cComputer_checkForBlock(self)) {
        cruiseCmd->AttackRight = 1;
        cruiseCmd->AttackLeft = 1;
    }
    (self->*(self->state))(cruiseCmd);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getPrewindCmd);
#ifdef SKIP_ASM
struct sRec_AA70 {
    int type;
    int flags;
    float a;
    float b;
};

class cTrack_AA70 {
public:
    char pad[0x34];
    virtual int Query(sRec_AA70* buf, int max, float x, float y);
};

extern "C" t4Vector cPath_getPathPos_AA70(cTrack_AA70*, int, float) __asm__("cPath_getPathPos");
extern "C" float cComputer_calcTurn_AA70(void*, t4Vector*) __asm__("cComputer_calcTurn");

// PORT: PS2-only VU0 inline asm (vector length via vsqrt).
static inline float Len_AA70(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_AA70(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_AA70(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

/**
 * Builds the prewind command: speed crouch, jump timing at the lip, steering at the jump and the trick axes.
 *
 * Sets bit 14 when 5 km/h under mDesiredSpeed. For a pipe jump (mToPipe) it sets bit 13 until the rider is within 50 of
 * the jump's length from mStartJumpPos, and returns. Otherwise a jump event (PE_Jump) within 50 ahead sets bit 13 and
 * steers (low field of word 1) at the jump's end point minus half of the rider's +0x490 point minus its position. The
 * fields at bits 15-20 and 21-26 get mSpinAxis and mFlipAxis.
 *
 * @param cmd  the prewind command (On Tour tPrewindCmd)
 */
void cComputer::getPrewindCmd(tPrewindCmd* cmd)
{
    sRec_AA70 buf[12];
    cRider* rider = RiderHandler.GroundMotion.mRider;
    cTrack_AA70* trk = (cTrack_AA70*)rider->AIPath;
    int n = trk->Query(buf, 12, rider->PrevPathParam, rider->PathParam + 50.0f);
    const int kNoJumpEvent = -1; // sel: no jump event in range
    int sel = kNoJumpEvent;
    if (Len_AA70(RiderHandler.GroundMotion.mRider->mVelocity) < mDesiredSpeed - 138.88890075683594f)
        cmd->BoostHeld = 1;
    if (mToPipe) {
        t4Vector jumpSpan = Sub_AA70(mEndJumpPos, mStartJumpPos);
        t4Vector fromJumpStart = Sub_AA70(RiderHandler.GroundMotion.mRider->mPosition,
                mStartJumpPos);
        if (Len_AA70(fromJumpStart) < Len_AA70(jumpSpan) - 50.0f)
            cmd->JumpHeld = 1;
        return;
    }
    for (int i = 0; i < n; i++) {
        if (buf[i].type == PE_Jump) {
            sel = i;
            cmd->JumpHeld = 1;
            break;
        }
    }
    float* ctl = (float*)(&mDesiredSpeed);
    if (sel != kNoJumpEvent) {
        cRider* riderBytes = RiderHandler.GroundMotion.mRider;
        t4Vector half = Sub_AA70(riderBytes->PathPos, riderBytes->mPosition);
        half = Scale_AA70(half, 0.5f);
        t4Vector pos = cPath_getPathPos_AA70(trk, 0, buf[sel].b);
        t4Vector toTarget = Sub_AA70(pos, half);
        cmd->PrewindTurn = (int)(cComputer_calcTurn_AA70(((char*)this), &toTarget) * 31.0f);
    }
    cmd->PrewindSpin = (int)(ctl[18] * 31.0f);
    cmd->PrewindFlip = (int)(ctl[19] * 31.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cComputer_getBoardPressCmd);
#ifdef SKIP_ASM

extern "C" int cComputer_checkForBoardPress(cComputer*, int*);
extern "C" void cComputer_calculateTargetPos(t4Vector*, void*);
extern "C" float cComputer_calcTurn(void*, t4Vector*);

// PORT: PS2 abs.s helper (stands in for fabsf).
static inline float vAbs_AD78(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float clamp_AD78(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

/**
 * Builds the board-press command: steering at the path target and, at a press event, the press and balance.
 *
 * The field at bits 18-23 gets the turn toward the path target (calcTurn). When checkForBoardPress
 * (cComputer_checkForBoardPress) finds a press event ahead, the low field of word 1 gets -31 for a positive event value and
 * +31 otherwise, and the field at bits 24-29 gets twice the side vector's z, clamped to 1 (when over 0.1 in
 * size); with no event the word-1 field is 0.
 *
 * @param cmd  the board-press command (On Tour tBoardPressCmd)
 */
void cComputer::getBoardPressCmd(tBoardPressCmd* cmd)
{
    int flag;
    int pressMarkerFound = checkForBoardPress(&flag);
    float pressDir = 1.0f;
    if (flag)
        pressDir = -1.0f;
    {
        t4Vector targetPos;
        cComputer_calculateTargetPos(&targetPos, ((char*)this));
        cmd->CruiseTurn = (int)(cComputer_calcTurn(((char*)this), &targetPos) * 31.0f);
    }
    if (pressMarkerFound) {
        cmd->BoardPress = (int)(pressDir * 31.0f);
        t4Vector sideAxis = RiderHandler.GroundMotion.mRider->mMatrix.row[0];
        float z = sideAxis.z + sideAxis.z;
        float y = clamp_AD78(z, -1.0f, 1.0f);
        if (vAbs_AD78(y) > 0.1f)
            cmd->RailBalance = (int)(y * 31.0f);
    } else {
        cmd->BoardPress = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getRailslideCmd);
#ifdef SKIP_ASM

extern "C" int cComputer_checkForJumpEvent(cComputer* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6);
extern "C" void cComputer_setupPrewindState_AED8(char*, int, int, int, int) __asm__("cComputer_setupPrewindState");
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" int cComputer_checkForSpeedZone(cComputer*, float*, int*);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_AED8(const t4Vector& v)
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

// PORT: PS2 abs.s helper (stands in for fabsf).
static inline float vAbs_AED8(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: `<?` is the g++ minimum operator (EE min.s).
static inline float clamp_AED8(float v, float lo, float hi)
{
    return (v >= lo) ? (v <? hi) : lo;
}

/**
 * Builds the railslide command: jumping off at a jump event, balance, rail spins and speed pumping.
 *
 * Copies the jump event's toRail flag into mToRail (uninitialised stack data when there is no event) and always sets
 * bits 17-24. At a jump the rider has reached it sets bits 13-14 and calls setupPrewindState(). Otherwise the field at
 * bits 25-30 gets twice the side vector's z (clamped, when over 0.1), the low field of word 1 gets +31 for a negative
 * mSpinAxis and -31 otherwise (not for RailslideMode kRSM_FS or kRSM_BS, or channel-2 animation category
 * kANCAT_RailSpin), and bit 16 is set in a reached speed zone (cComputer_checkForSpeedZone) when 5 km/h too slow.
 *
 * @param cmd  the railslide command (On Tour tRailslideCmd)
 */
void cComputer::getRailslideCmd(tRailslideCmd* cmd)
{
    int below, toPipe, toRail, isSpin, isFlip, ok;
    int jumpEvent = checkForJumpEvent((float*)(&mDesiredSpeed), &below, &toPipe, &toRail, &isSpin, &isFlip);
    mToRail = toRail;
    *(unsigned int*)cmd |= 0x1FE0000;
    if (jumpEvent && below) {
        *(unsigned int*)cmd |= 0x6000;
        cComputer_setupPrewindState_AED8(((char*)this), isSpin, isFlip, toRail, toPipe);
        return;
    }
    t4Vector sideAxis = RiderHandler.GroundMotion.mRider->mMatrix.row[0];
    float z = sideAxis.z + sideAxis.z;
    float y = clamp_AED8(z, -1.0f, 1.0f);
    if (vAbs_AED8(y) > 0.1f)
        cmd->RailBalance = (int)(y * 31.0f);
    cRider* rider = RiderHandler.GroundMotion.mRider;
    if ((unsigned int)(rider->RailslideMode - kRSM_FS) >= 2 &&
        cRiderAnimBase_getCurrentAnimCategory(rider->mRiderAnim, 2) != kANCAT_RailSpin) {
        float spinSign = (mSpinAxis < 0.0f) ? 1.0f : -1.0f;
        cmd->RailSpin = (int)(spinSign * 31.0f);
    }
    int speedMarkerFound = checkForSpeedZone((float*)(&mDesiredSpeed), &ok);
    float len = vLen_AED8(RiderHandler.GroundMotion.mRider->mVelocity);
    if (speedMarkerFound && ok && len < mDesiredSpeed - 138.88890075683594f)
        *(unsigned int*)cmd |= 0x10000;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_setupEndSpin);
#ifdef SKIP_ASM
extern "C" void cSpinControl_predictSpinSec(void*, float*, float*);
extern "C" void cSpinControl_predictFlipSec(void*, float*, float*);

/**
 * Stops the AI's spin and flip inputs in time to land, using the spin control's time predictions.
 *
 * Both axes are zeroed when heading for a rail with under 1 s of air. Otherwise, for a spin trick, the spin
 * stops when the current half turn (predictSpinSec) cannot finish within @p airTime + 0.1 s, or when another half
 * turn would not fit and the current one needs under 0.4 s; the flip uses the same test (predictFlipSec) and
 * stops both axes.
 *
 * @param airTime  air time left, in seconds (On Tour airTime)
 */
void cComputer::setupEndSpin(float airTime)
{
    float spinCur, spinNext, flipCur, flipNext;
    cSpinControl_predictSpinSec(&RiderHandler.GroundMotion.mRider->mRiderHandler->SpinControl, &spinCur, &spinNext);
    cSpinControl_predictFlipSec(&RiderHandler.GroundMotion.mRider->mRiderHandler->SpinControl, &flipCur, &flipNext);
    int stop = 0;
    if (mToRail && airTime < 1.0f)
        stop = 1;
    if (stop) {
        mFlipAxis = 0.0f;
        mSpinAxis = 0.0f;
        return;
    }
    if (!mIsSpinTrick)
        return;
    if (mSpinAxis != 0.0f) {
        float lim = airTime + 0.1f;
        if (lim < spinCur)
            mSpinAxis = 0.0f;
        else if (lim < spinNext && spinCur - 0.4f < 0.0f)
            mSpinAxis = 0.0f;
    }
    if (mFlipAxis != 0.0f) {
        float lim = airTime + 0.1f;
        if (lim < flipCur) {
            mFlipAxis = 0.0f;
            mSpinAxis = 0.0f;
        } else if (lim < flipNext && flipCur - 0.4f < 0.0f) {
            mFlipAxis = 0.0f;
            mSpinAxis = 0.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getSpinCmd);
#ifdef SKIP_ASM
// Pad stick setter (an inline accessor: the value is expanded before the field mask).
static inline void SetW12_B250(tSpinCmd* pad, int v)
{
    pad->AirAdjRotLR = v;
}

int AIrand();
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void cComputer_setupEndSpin(cComputer*, float);
extern "C" int cComputer_chooseGrabMain(char*, float);

/**
 * Builds the in-air trick command: spin and flip axes, the grab to hold, and when the grab starts and ends.
 *
 * Takes the air time left from the air predictor (100 when it has no landing) and counts frames with over 15 s of air
 * at rider +0x484; at 31 it sets bit 12 instead (probably a reset). A rider not set to mess up runs setupEndSpin(),
 * picks a grab with chooseGrabMain() when there is time and none is held, and drops it near the landing
 * (mHoldToLandTime + 0.3 s), 2 s into a non-uber (kANCAT_Uber) grab or before a rail; a messing-up rider picks a random
 * grab once it is not flipping. The command gets mSpinAxis, mFlipAxis, mGrabID (bits 16-23), the tweak/uber bit 14 and,
 * toward a rail, -31 at word 1 bits 12-17.
 *
 * @param cmd  the spin command (On Tour tSpinCmd)
 * @return -1 when no grab is chosen (mGrabID is kNoGrab); otherwise no value is set (On Tour's getSpinCmd
 *         returns nothing)
 */
// Returns -1 only when no trick is queued, and has no value otherwise (the caller ignores it): the retail
// tail keeps `lw E0C; li v0,-1` with no branch. The noop `v0 = -1` return arm survives reload as a USE,
// so jump2 only deletes the branch after sched2 and the compare loads stay.
int cComputer::getSpinCmd(tSpinCmd* cmd)
{
    cAirPredictor* obj = RiderHandler.GroundMotion.mRider->mAirPredictor;
    int simState = obj->SimState;
    int ok = 0;
    if (simState == cAirPredictor::kSS_SimDone || simState == cAirPredictor::kSS_SimCheckObjectsDone)
        ok = 1;
    float gap;
    if (ok) {
        gap = obj->TotalAirTime - obj->CurAirTime;
        if (gap > 15.0f)
            (RiderHandler.GroundMotion.mRider->mEndOfTheWorldTimer)++;
        else
            RiderHandler.GroundMotion.mRider->mEndOfTheWorldTimer = 0;
    } else {
        gap = 100.0f;
        RiderHandler.GroundMotion.mRider->mEndOfTheWorldTimer = 0;
    }
    if (RiderHandler.GroundMotion.mRider->mEndOfTheWorldTimer >= 31) {
        RiderHandler.GroundMotion.mRider->mEndOfTheWorldTimer = 0;
        cmd->Reset = 1;
    } else {
        if (mIsGonnaMessUp == 0) {
            setupEndSpin(gap);
            if (mGrabEndTime <= 0.0f)
                mGrabEndTime = 15.0f;
            if (mIsNaturalAir) {
                mGrabEndTime = gap;
                mIsNaturalAir = 0;
            } else if (mGrabID == kNoGrab && gap > 0.5f && gap < mGrabEndTime &&
                       cRiderAnimBase_getCurrentAnimCategory(RiderHandler.GroundMotion.mRider->mRiderAnim, 2)
                           != kANCAT_Uber) {
                mGrabID = cComputer_chooseGrabMain(((char*)this), gap);
            } else {
                int stop = 0;
                if (mToRail && gap < 1.0f)
                    stop = 1;
                if (stop) {
                    mIsTweak = 0;
                    mGrabID = kNoGrab;
                } else if (gap <= mHoldToLandTime + 0.30000001192092896f ||
                           (mGrabStartTime - gap > 2.0f && mIsUber == 0)) {
                    mIsTweak = 0;
                    mGrabID = kNoGrab;
                    mIsUber = 0;
                }
            }
        } else if (mFlipAxis == 0.0f && mGrabID == kNoGrab) {
            mGrabID = (unsigned int)AIrand() % 15;
            if (AIrand() & 1)
                mIsTweak = 1;
        }
        float* ctl = (float*)(&mDesiredSpeed);
        cmd->SpinAdjust = (int)(ctl[18] * 31.0f);
        cmd->FlipAdjust = (int)(ctl[19] * 31.0f);
        cmd->GrabID = mGrabID;
        if (mIsUber)
            cmd->Tweak = 1;
        else if (mIsTweak)
            cmd->Tweak = 1;
        else
            cmd->Tweak = 0;
        if (mToRail == true)
            SetW12_B250(cmd, -0x1F);
        if (mGrabID == kNoGrab)
            return -1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cComputer_getNaturalAirCmd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

int AIrand();
extern "C" void cComputer_calculateTargetPos_B590(t4Vector*, void*) __asm__("cComputer_calculateTargetPos");
extern "C" float cComputer_calcTurn_B590(void*, t4Vector*) __asm__("cComputer_calcTurn");

/**
 * Builds the command for natural (unplanned) air: random spins and flips sized to the air time.
 *
 * Takes the air time from the air predictor (0 without a landing), sets mIsNaturalAir and clears mIsGonnaMessUp,
 * mToRail and the grab. Over 1 s it spins in a random direction and over 3 s it also flips (either sets mIsSpinTrick).
 * Under 0.5 s with neither it steers at the path target (bits 24-29, negated calcTurn). Byte 2 gets mGrabID, which is
 * always kNoGrab here.
 *
 * @param cmd  the natural-air command (On Tour tNaturalAirCmd)
 */
void cComputer::getNaturalAirCmd(tNaturalAirCmd* cmd)
{
    cAirPredictor* airPredictor = RiderHandler.GroundMotion.mRider->mAirPredictor;
    int st = airPredictor->SimState;
    int ok = 0;
    if (st == cAirPredictor::kSS_SimDone || st == cAirPredictor::kSS_SimCheckObjectsDone)
        ok = 1;
    float airTimeLeft;
    if (ok)
        airTimeLeft = airPredictor->TotalAirTime - airPredictor->CurAirTime;
    else
        airTimeLeft = 0.0f;
    mIsSpinTrick = 0;
    mIsGonnaMessUp = 0;
    mIsNaturalAir = 1;
    mToRail = 0;
    mGrabID = kNoGrab;
    if (airTimeLeft > 1.0f) {
        mIsSpinTrick = 1;
        mSpinAxis = 1.0f;
        if (AIrand() & 1)
            mSpinAxis = -mSpinAxis;
    } else {
        mSpinAxis = 0.0f;
    }
    if (airTimeLeft > 3.0f) {
        mIsSpinTrick = 1;
        mFlipAxis = 1.0f;
        if (AIrand() & 1)
            mFlipAxis = -mFlipAxis;
    } else {
        mFlipAxis = 0.0f;
    }
    if (airTimeLeft < 0.5f && mFlipAxis == 0.0f && mSpinAxis == 0.0f) {
        t4Vector targetPos;
        cComputer_calculateTargetPos_B590(&targetPos, ((char*)this));
        cmd->CruiseTurn = (int)(-cComputer_calcTurn_B590(((char*)this), &targetPos) * 31.0f);
        mGrabID = kNoGrab;
    }
    cmd->GrabID = mGrabID;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getWipeoutCmd);
#ifdef SKIP_ASM
/**
 * Fills the computer rider's wipeout command: bit 12 is set from cComputer_checkForReset, the rest is kept.
 *
 * cComputer_getControlCmd calls it.
 *
 * @param controlCmd  the wipeout command (On Tour tWipeoutCmd): only Reset is set
 */
void cComputer::getWipeoutCmd(tWipeoutCmd* controlCmd)
{
    controlCmd->Reset = cComputer_checkForReset(this);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getGateCmd);
#ifdef SKIP_ASM
int AIrand();
extern "C" int AITick();

/**
 * Builds the start-gate command: random rocking, then a timed pull-back and launch for the best computer
 * riders.
 *
 * With t = 180 minus the AI tick, riders with AI index 1-3 push forward (+31 at bits 12-17) once t is under
 * 33 - 10 * (index - 1), and riders 1-2 pull back (-31) once it is under 72. Otherwise every 20th tick stores
 * a new random value with a random sign in mDesiredSpeed and sends it as the rock input; AIrand() is not
 * reduced first (only scaled by 0.01), so only the low bits survive the 6-bit field.
 *
 * @param cmd  the gate command (On Tour tGateCmd)
 */
void cComputer::getGateCmd(tGateCmd* cmd)
{
    float framesLeft = 180.0f - (float)AITick();
    float th = 33.0f;
    th -= (float)((RiderHandler.GroundMotion.mRider->getAIIndex() - 1) * 10);
    if (framesLeft < th && RiderHandler.GroundMotion.mRider->getAIIndex() < 4) {
        cmd->Anticipate = 0x1F;
        return;
    }
    if (framesLeft < 72.0f && RiderHandler.GroundMotion.mRider->getAIIndex() < 3) {
        cmd->Anticipate = -0x1F;
        return;
    }
    if (AITick() % 20 == 0) {
        unsigned int randVal = AIrand();
        float* ctlSpeed = (float*)(&mDesiredSpeed);
        *ctlSpeed = (float)randVal * 0.009999999776482582f;
        if ((AIrand() & 1) == 0)
            mDesiredSpeed = -mDesiredSpeed;
    }
    cmd->Anticipate = (int)(mDesiredSpeed * 31.0f);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_checkForJumpEvent);
#ifdef SKIP_ASM


extern "C" t4Vector cPath_getPathPos(cAIPath*, int, float);

/**
 * Looks for a jump event on the rider's path just ahead and returns its settings and end points.
 *
 * Queries the rider's AIPath (cPath_getEventsInRange, vtable slot 1) for events from its PrevPathParam to 300 past its
 * PathParam and takes the first of type PE_Jump. Its flag word gives the approach speed (bits 4 up, km/h, returned in
 * cm/s) and the pipe, rail, spin and flip bits (3 down to 0); mStartJumpPos and mEndJumpPos are set from the event's
 * two path distances. The outputs are untouched when there is no event.
 *
 * @param approachSpeed  receives the speed to approach the jump at, in cm/s
 * @param below  receives 1 when the rider has reached the jump's start (On Tour isPrewind)
 * @param toPipe  receives the jump-into-pipe flag
 * @param toRail  receives the jump-onto-rail flag
 * @param isSpin  receives the spin flag
 * @param isFlip  receives the flip flag
 * @return true when a jump event was found, otherwise false
 */
bool cComputer::checkForJumpEvent(float* approachSpeed, int* below, int* toPipe, int* toRail, int* isSpin, int* isFlip)
{
    tPathEventNode buf[12];
    cRider* rider = RiderHandler.GroundMotion.mRider;
    cAIPath* trk = rider->AIPath;
    sVEntry<int(void*, tPathEventNode*, int, float, float)>* vt =
        (sVEntry<int(void*, tPathEventNode*, int, float, float)>*)trk->mVtbl;
    int n = vt[1].fn(vthis(trk, vt[1]), buf, 12, rider->PrevPathParam, rider->PathParam + 300.0f); // getEventsInRange
    for (int i = 0; i < n; i++) {
        if (buf[i].Type == PE_Jump) {
            int fl = buf[i].Arg;
            *approachSpeed = (float)(fl >> 4) * 27.777780532836914f;
            *toPipe = (fl >> 3) & 1;
            *toRail = (fl >> 2) & 1;
            *isSpin = (fl >> 1) & 1;
            *isFlip = fl & 1;
            *below = (buf[i].ParamStart <= RiderHandler.GroundMotion.mRider->PathParam) ? 1 : 0;
            *(&mStartJumpPos) = cPath_getPathPos(trk, 0, buf[i].ParamStart);
            *(&mEndJumpPos) = cPath_getPathPos(trk, 0, buf[i].ParamEnd);
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_checkForSpeedZone);
#ifdef SKIP_ASM
struct sHit_BB18 { int type; int val; float lim; int d; };
/**
 * Looks for a speed marker on the computer rider's path just ahead.
 *
 * Asks the rider's AIPath (cPath_getEventsInRange, vtable slot 1) for up to 12 markers between the rider's
 * PrevPathParam and PathParam plus 600. For the first marker of type PE_SpeedZone it stores the marker's value times
 * 27.77778 in @p out and in @p ok whether the marker's limit is at most PathParam, and returns true.
 *
 * @param out  receives the speed
 * @param ok  receives 1 when the rider is inside the zone
 * @return true when a speed marker was found, else false
 */
bool cComputer::checkForSpeedZone(float* out, int* ok)
{
    sHit_BB18 buf[12];
    cRider* rider = RiderHandler.GroundMotion.mRider;
    cAIPath* track = rider->AIPath;
    sVEntry<int(void*, sHit_BB18*, int, float, float)>* vt =
        (sVEntry<int(void*, sHit_BB18*, int, float, float)>*)track->mVtbl;
    int n = vt[1].fn(vthis(track, vt[1]), buf, 12, rider->PrevPathParam, rider->PathParam + 600.0f);
    for (int i = 0; i < n; i++) {
        if (buf[i].type == PE_SpeedZone) {
            *out = (float)buf[i].val * 27.777780532836914f;
            *ok = buf[i].lim <= RiderHandler.GroundMotion.mRider->PathParam;
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_checkForBoardPress);
#ifdef SKIP_ASM
struct sHit_BBF8 { int type; int val; int d[2]; };
/**
 * Looks for a board-press marker on the computer rider's path just ahead.
 *
 * Asks the rider's AIPath (cPath_getEventsInRange) for up to 12 markers between PrevPathParam and PathParam plus 50.
 * For the first marker of type PE_BoardPress it stores in @p out whether its value is positive and returns true; @p out
 * is 0 otherwise.
 *
 * @param out  receives 1 for a positive marker value
 * @return true when a marker was found, else false
 */
bool cComputer::checkForBoardPress(int* out)
{
    sHit_BBF8 buf[12];
    cRider* rider = RiderHandler.GroundMotion.mRider;
    cAIPath* track = rider->AIPath;
    sVEntry<int(void*, sHit_BBF8*, int, float, float)>* vt =
        (sVEntry<int(void*, sHit_BBF8*, int, float, float)>*)track->mVtbl;
    int n = vt[1].fn(vthis(track, vt[1]), buf, 12, rider->PrevPathParam, rider->PathParam + 50.0f);
    *out = 0;
    for (int i = 0; i < n; i++) {
        if (buf[i].type == PE_BoardPress) {
            *out = buf[i].val > 0;
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_checkForRail);
#ifdef SKIP_ASM
struct sHit_BC98 { int type; int d[3]; };
/**
 * Tells whether the computer rider's path has a marker of type PE_Railslide at its current position.
 *
 * Asks the rider's AIPath (cPath_getEventsInRange) for up to 12 markers between PrevPathParam and PathParam. Nothing
 * in the build references it.
 *
 * @return true when such a marker was found, else false
 */
bool cComputer::checkForRail()
{
    sHit_BC98 buf[12];
    cRider* rider = RiderHandler.GroundMotion.mRider;
    cAIPath* track = rider->AIPath;
    sVEntry<int(void*, sHit_BC98*, int, float, float)>* vt =
        (sVEntry<int(void*, sHit_BC98*, int, float, float)>*)track->mVtbl;
    int n = vt[1].fn(vthis(track, vt[1]), buf, 12, rider->PrevPathParam, rider->PathParam);
    for (int i = 0; i < n; i++) {
        if (buf[i].type == PE_Railslide) return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_checkForTargets);
#ifdef SKIP_ASM
struct sV4_BD10
{
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRec_BD10
{
    int active;
    int pad4;
    float dist;
    float angle;
    char pad10[0x24 - 0x10];
};

// The object at cComputer+0x18 (On Tour reads the same fields through its first member).
struct sRiderT_BD10
{
    sRec_BD10 targets[6];       // 0x000
    char padD8[0x1B0 - 0xD8];
    sV4_BD10 fwd;               // 0x1B0
    char pad1C0[0x4C8 - 0x1C0];
    float pathDist;             // 0x4C8
};

extern "C" int cComputer_checkForAttack(char* self, int i);
extern "C" float fCos(float);
extern "C" float func_0031C228(float);

// PORT: PS2-only FPU asm (absolute value).
static inline float Abs_BD10(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only FPU asm (float -> int -> float truncation).
static inline float Trunc_BD10(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    return t;
}

static inline float Floor_BD10(float x)
{
    float t = Trunc_BD10(x);
    if (x < t)
        t -= 1.0f;
    return t;
}

static inline float Atan2_BD10(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            r += 3.1415927410125732f;
        else
            r -= 3.1415927410125732f;
    }
    return r;
}

static inline sV4_BD10 Fwd_BD10(sRiderT_BD10* o) { return o->fwd; }


/**
 * Keeps or picks the rider this computer rider deals with (mTargetID) and says whether to attack it.
 *
 * Drops the target when it is over 1050 away or the rider has wandered over 600 from its path (+0x4C8). With
 * no target, within 540 of the path and on every 12th tick, it scores each active rider within 700 by its
 * distance times (|cos| + 1) of its bearing off the rider's facing for the rival (checkForAttack()), or times
 * (1 - cos) for the others, and takes the lowest.
 *
 * @param isAttack  receives whether the new target is the rival to attack; written only when one is picked
 * @return true when the rider has a target, otherwise false
 */
// On Tour: bool cComputer::checkForTargets(bool& isAttack); retail stores a word through isAttack.
bool cComputer::checkForTargets(int* isAttack)
{
    const float kAIMaxTargetDist = 700.0f;
    const float kAIMaxPathWanderDist = 600.0f;
    if (mTargetID >= 0) {
        if (((sRiderT_BD10*)RiderHandler.GroundMotion.mRider)->targets[mTargetID].dist > kAIMaxTargetDist * 1.5f
            || ((sRiderT_BD10*)RiderHandler.GroundMotion.mRider)->pathDist > kAIMaxPathWanderDist)
            mTargetID = kNoTarget;
    }
    if (mTargetID < 0 && ((sRiderT_BD10*)RiderHandler.GroundMotion.mRider)->pathDist < kAIMaxPathWanderDist * 0.9f
        && AITick() % 12 == 0) {
        float bestScore = 10000000000.0f;
        float aScore;
        int shouldAttack;
        int i;
        for (i = 0; i < 6; i++) {
            if (((sRiderT_BD10*)RiderHandler.GroundMotion.mRider)->targets[i].active == 0)
                continue;
            aScore = ((sRiderT_BD10*)RiderHandler.GroundMotion.mRider)->targets[i].dist;
            if (aScore > kAIMaxTargetDist)
                continue;
            shouldAttack = cComputer_checkForAttack(((char*)this), i);
            float del = ((sRiderT_BD10*)RiderHandler.GroundMotion.mRider)->targets[i].angle - Atan2_BD10(Fwd_BD10((sRiderT_BD10*)RiderHandler.GroundMotion.mRider).y, Fwd_BD10((sRiderT_BD10*)RiderHandler.GroundMotion.mRider).x);
            float cosDelta = fCos(del - Floor_BD10(del * 0.15915493667125702f + 0.5f) * 6.2831854820251465f);
            if (shouldAttack)
                aScore *= Abs_BD10(cosDelta) + 1.0f;
            else
                aScore *= 1.0f - cosDelta;
            if (aScore <= bestScore) {
                mTargetID = i;
                bestScore = aScore;
                *isAttack = shouldAttack;
            }
        }
    }
    return mTargetID >= 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_setupPrewindState);
#ifdef SKIP_ASM
int AIrand();
extern "C" int cComputer_shouldRiderWipeout(cComputer*);

struct sObj_BFA8 {
    char pad[0xE0C];
    int fE0C;
    int fE10;
    int fE14;
    int fE18;
    int fE1C;
    int fE20;
    char padE24[0x14];
    float fE38;
    float fE3C;
};

/**
 * Prepares the trick for a coming jump: which axes to rotate on, whether to mess it up, and where it lands.
 *
 * Clears mGrabID and mIsNaturalAir, sets mIsSpinTrick when @p isSpin or @p isFlip is set, rolls mIsGonnaMessUp with
 * shouldRiderWipeout() and stores @p toRail and @p toPipe in mToRail and mToPipe. For a spin trick mSpinAxis and
 * mFlipAxis become 1 for the requested rotations (0 otherwise), each with a random sign.
 *
 * @param isSpin  the jump asks for a spin (On Tour isSpin)
 * @param isFlip  the jump asks for a flip (On Tour isFlip)
 * @param toRail  the jump leads onto a rail (On Tour toRail)
 * @param toPipe  the jump leads into a pipe (On Tour toPipe)
 */
void cComputer::setupPrewindState(int isSpin, int isFlip, int toRail, int toPipe)
{
    mGrabID = -1;
    int* trickFlags = &mIsSpinTrick;
    *trickFlags = (isSpin || isFlip);
    mIsGonnaMessUp = shouldRiderWipeout();
    mToRail = toRail;
    mToPipe = toPipe;
    mIsNaturalAir = 0;
    if (mIsSpinTrick) {
        mSpinAxis = isSpin ? 1.0f : 0.0f;
        mFlipAxis = isFlip ? 1.0f : 0.0f;
        if (AIrand() & 1)
            mSpinAxis = -mSpinAxis;
        if (AIrand() & 1)
            mFlipAxis = -mFlipAxis;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_setupCrouch);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_C0A8(const t4Vector& v)
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
 * Sets the cruise command's crouch field (CruiseCrouch) unless the rider is weak (mRiderDifficulty under 20).
 *
 * The field gets 31 below 833.3 cm/s (30 km/h) and 0.97 * 31 above it, or 0 for a weak rider.
 *
 * @param cmd  the cruise command (On Tour tCruiseCmd)
 */
void cComputer::setupCrouch(void* cmd)
{
    tCruiseCmd* crouchCmd = (tCruiseCmd*)cmd;
    float len = vLen_C0A8(RiderHandler.GroundMotion.mRider->mVelocity);
    float pressAmount = 0.97f;
    if (len < 833.3334350585938f)
        pressAmount = 1.0f;
    if (mRiderDifficulty < 20.0f)
        pressAmount = 0.0f;
    crouchCmd->CruiseCrouch = (int)(pressAmount * 31.0f);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_setupRecklessCrouch);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_C140(const t4Vector& v)
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
 * Sets the cruise command's crouch field as setupCrouch() does but for every rider, and sets BoostPressed and
 * BoostHeld.
 *
 * @param cmd  the cruise command (On Tour tCruiseCmd)
 */
void cComputer::setupRecklessCrouch(void* cmd)
{
    tCruiseCmd* crouchCmd = (tCruiseCmd*)cmd;
    float len = vLen_C140(*(&RiderHandler.GroundMotion.mRider->mVelocity));
    float pressAmount = 0.97f;
    if (len < 833.3334350585938f)
        pressAmount = 1.0f;
    crouchCmd->CruiseCrouch = (int)(pressAmount * 31.0f);
    crouchCmd->BoostPressed = 1;
    crouchCmd->BoostHeld = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_shouldRiderWipeout);
#ifdef SKIP_ASM
/**
 * Rolls whether this computer rider will mess up its next trick, more often for a low mTrickDifficulty.
 *
 * @return true when AIrand() % 100 is at least mTrickDifficulty * 100 (a 1 - mTrickDifficulty chance), else false
 */
bool cComputer::shouldRiderWipeout()
{
    unsigned int roll = AIrand();
    float thr = mTrickDifficulty * 100.0f;
    return thr <= (float)(roll % 100U);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_shouldRiderUber);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_C258(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
/**
 * Rolls whether this computer rider tries an uber trick; needs uber enabled and never at location 14.
 *
 * Requires mUberFlags bit 0 and a location ID other than 14; the chance is then mTrickDifficulty.
 *
 * @return true when the roll (AIrand() % 100 under mTrickDifficulty * 100) passes, otherwise false
 */
bool cComputer::shouldRiderUber()
{
    if ((unsigned int)RiderHandler.GroundMotion.mRider->mUberFlags & 1) {
        if (cBENewRaceInterface_getLocationID(cBE_getInterface_C258(cBE_getBE(), 0)) != 0xE) {
            unsigned int roll = AIrand();
            float thr = mTrickDifficulty * 100.0f;
            return (float)(roll % 100U) < thr;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_shouldRiderTweak);
#ifdef SKIP_ASM
/**
 * Rolls whether this computer rider tweaks its grab: a 30 + 70 * mTrickDifficulty percent chance.
 *
 * @return true when AIrand() % 100 is under 30 + 70 * mTrickDifficulty, otherwise false
 */
bool cComputer::shouldRiderTweak()
{
    unsigned int roll = AIrand();
    float thr = mTrickDifficulty * 70.0f + 30.0f;
    return (float)(roll % 100U) < thr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_shouldRiderGrab);
#ifdef SKIP_ASM
/**
 * Rolls whether this computer rider grabs at all: a 60 + 40 * mTrickDifficulty percent chance.
 *
 * @return true when AIrand() % 100 is under 60 + 40 * mTrickDifficulty, otherwise false
 */
bool cComputer::shouldRiderGrab()
{
    unsigned int roll = AIrand();
    float thr = mTrickDifficulty * 40.0f + 60.0f;
    return (float)(roll % 100U) < thr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_updateRiderPathDifficulty);
#ifdef SKIP_ASM
/**
 * Picks the path difficulty (0 easy, 1 medium, 2 hard) for a computer rider from its slot and the heat.
 *
 * Riders 1 and 2 take medium paths in heats 1-2 and hard ones after; rider 3 easy in heat 1, then medium;
 * rider 4 easy in heats 1-2, then medium; rider 5 and any other value always easy.
 *
 * @param RiderID  the rider's AI index, 1-5 (On Tour RiderID)
 * @param Heat  the heat number, 1-3 (On Tour Heat)
 * @return the path difficulty, 0-2 (the caller stores it in mPathDifficulty)
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int cComputer::updateRiderPathDifficulty(int RiderID, int Heat)
{
    switch (RiderID) {
    case 1:
        if (Heat == 1)
            return 1;
        return (Heat == 2) ? 1 : 2;
    case 2:
        if (Heat == 1)
            return 1;
        return (Heat == 2) ? 1 : 2;
    case 3:
        if (Heat == 1)
            return 0;
        if (Heat == 2)
            return 1;
        return 1;
    case 4:
        if (Heat == 1)
            return 0;
        if (Heat == 2)
            return 0;
        return 1;
    case 5:
        if (Heat == 1)
            return 0;
        if (Heat == 2)
            return 0;
        return 0;
    }
    return 0;
}
#endif
