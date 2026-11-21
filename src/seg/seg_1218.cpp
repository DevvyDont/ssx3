#include "common.h"
#include "ai/focusbox.h"
#include "ai/dangerman.h"
#include "ai/rideranim.h"
#include "camera/trigger/cameratriggerman.h"
#include "sound/ssxaudio.h"
#include "ai/computer.h"
#include "world/world.h"
#include "main/ssxapp.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("seg/seg_1218", func_00100250);
#ifdef SKIP_ASM
struct sVec4_0250 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vDot_0250(const sVec4_0250& a, const sVec4_0250& b)
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
static inline sVec4_0250 vScale_0250(const sVec4_0250& v, float s)
{
    sVec4_0250 r;
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
static inline sVec4_0250 vSub_0250(const sVec4_0250& a, const sVec4_0250& b)
{
    sVec4_0250 r;
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
static inline sVec4_0250 vNorm_0250(const sVec4_0250& v)
{
    sVec4_0250 r;
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
 * Takes the rider (pointer at +0x18), removes from @p direction its component along the rider's side axis (+0x1A0),
 * normalizes the rest and returns the negated dot product with the rider's up axis (+0x1C0). When the float at +0xDF4
 * is not 1.0 the result is multiplied by 0.12667641 and by that value. Nothing in the build references it.
 *
 * @param direction  the direction to measure
 * @return the slope value
 */
extern "C" float func_00100250(void* self, const sVec4_0250* direction)
{
    char* rider = *(char**)((char*)self + 0x18);
    sVec4_0250 riderSide = *(sVec4_0250*)(rider + 0x1A0);
    sVec4_0250 riderUp = *(sVec4_0250*)(rider + 0x1C0);
    sVec4_0250 planarDir = vSub_0250(*direction, vScale_0250(riderSide, vDot_0250(*direction, riderSide)));
    float slope = -vDot_0250(vNorm_0250(planarDir), riderUp);
    float accuracy = *(float*)((char*)self + 0xDF4);
    if (accuracy == 1.0f)
        return slope;
    return slope * 0.12667641043663025f * accuracy;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_calcTurn);
#ifdef SKIP_ASM
struct sV4_0348
{
    float x, y, z, w;
} __attribute__((aligned(16)));

class cObj_0348 {
public:
    virtual void vm1();
    virtual sV4_0348* GetDir();
    virtual void v1();
    virtual void v2();
    virtual sV4_0348* GetPos();
};

extern sV4_0348 D_004FF160_v0348 __asm__("t4vect_ZAxis");
extern "C" float func_0031C228(float);

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sV4_0348 Cross_0348(const sV4_0348& a, const sV4_0348& b)
{
    sV4_0348 r;
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
static inline sV4_0348 Sub_0348(const sV4_0348& a, const sV4_0348& b)
{
    sV4_0348 r;
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
static inline sV4_0348 Scale_0348(const sV4_0348& v, float s)
{
    sV4_0348 r;
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
static inline sV4_0348 Norm_0348(const sV4_0348& v)
{
    sV4_0348 r;
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
static inline float Dot_0348(const sV4_0348& a, const sV4_0348& b)
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

static inline cObj_0348* Obj_0348(char* self) { return (cObj_0348*)(*(char**)(self + 0x18) + 0x6C0); }

extern "C" float cComputer_calcTurn_0348(char* self, sV4_0348* target) __asm__("cComputer_calcTurn");
/**
 * Returns the steering amount that turns the computer rider towards a point.
 *
 * The up axis is the rider's +0x1C0 (t4vect_ZAxis when its z is not positive), the forward axis comes from the rider's
 * +0x6C0 object (virtual GetDir) and the side axis is their cross product. The direction from that object's position
 * (virtual GetPos) to @p target is flattened (z = 0), normalized, made perpendicular to the up axis and normalized
 * again; its angle is atan2 of its side and forward parts (func_0031C228 for the arctangent). The result is |angle| *
 * 6.2897024: 0 below 0.02, at most 0.98, with the angle's sign. The cruise routines multiply it by 31 for the command's
 * turn field.
 *
 * @param target  the point to steer towards
 * @return the turn amount, -0.98 to 0.98
 */
extern "C" float cComputer_calcTurn_0348(char* self, sV4_0348* target)
{
    sV4_0348 up = *(sV4_0348*)(*(char**)(self + 0x18) + 0x1C0);
    if (up.z <= 0.0f)
        up = D_004FF160_v0348;
    sV4_0348 fwd = *Obj_0348(self)->GetDir();
    sV4_0348 side = Cross_0348(fwd, up);
    sV4_0348 toTarget = Sub_0348(*target, *Obj_0348(self)->GetPos());
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
struct sV4_0610 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV4_0610 Add_0610(const sV4_0610& a, const sV4_0610& b)
{
    sV4_0610 r;
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
static inline sV4_0610 Sub_0610(const sV4_0610& a, const sV4_0610& b)
{
    sV4_0610 r;
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
static inline sV4_0610 Scale_0610(const sV4_0610& v, float s)
{
    sV4_0610 r;
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

struct sCam_0610 {
    char pad0[0x110];
    sV4_0610 pos;       // 0x110
    char pad120[0x490 - 0x120];
    sV4_0610 target;    // 0x490
    char pad4A0[0x10];
    sV4_0610 base;      // 0x4B0
};

// PORT: the unit later declares cComputer_calculateTargetPos with the out-pointer spelled out; same symbol, by-value return here
sV4_0610 cComputer_calculateTargetPos_0610(char* self) __asm__("cComputer_calculateTargetPos");

/**
 * Returns the point the computer rider steers towards.
 *
 * Takes the rider (pointer at +0x18) and returns its +0x4B0 point plus a tenth of the vector from +0x110 to +0x490. The
 * cruise routines hand it to cComputer_calcTurn for the turn amount.
 *
 * @return the target point
 */
sV4_0610 cComputer_calculateTargetPos_0610(char* self)
{
    sV4_0610 toTarget = Sub_0610((*(sCam_0610**)(self + 0x18))->target, (*(sCam_0610**)(self + 0x18))->pos);
    toTarget = Scale_0610(toTarget, 0.1f);
    return Add_0610((*(sCam_0610**)(self + 0x18))->base, toTarget);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cComputer_followPathCruise);
#ifdef SKIP_ASM
struct sV4_0680 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_0680 {
    unsigned int pad0 : 17;
    unsigned int b17 : 1;
    unsigned int pad18 : 2;
    int b20 : 6;
    unsigned int pad26 : 6;
    int w0 : 6;
    int w6 : 6;
};

class cObj_0680;
typedef void (cObj_0680::*Fn_0680)();
extern "C" Fn_0680 D_0043CF20_0680[4] __asm__("D_0043CF20");

class cAI_0680 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual int Busy();
};

struct sRiders_0680 {
    char pad[0x28];
    char* riders[1];
};

struct sGame_0680 {
    char pad[0xC];
    sRiders_0680* list;
};

struct sWorld_0680 {
    char pad[0x84];
    sGame_0680* game;
};

extern sWorld_0680* D_004A28A8_0680 __asm__("SSXApp");

extern "C" void cComputer_calculateTargetPos_0680(sV4_0680*, void*) __asm__("cComputer_calculateTargetPos");
extern "C" float cComputer_calcTurn_0680(void*, sV4_0680*) __asm__("cComputer_calcTurn");
extern "C" int cComputer_checkForSpeedZone_0680(void*, float*, int*) __asm__("cComputer_checkForSpeedZone");
extern "C" int cComputer_checkForBoardPress_0680(void*, int*) __asm__("cComputer_checkForBoardPress");
extern "C" void cComputer_setupRecklessCrouch_0680(void*, void*) __asm__("cComputer_setupRecklessCrouch");
extern "C" void cComputer_setupCrouch_0680(void*, void*) __asm__("cComputer_setupCrouch");
extern "C" int cComputer_checkForJumpEvent_0680(char* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6) __asm__("cComputer_checkForJumpEvent");
extern "C" int cComputer_checkForTargets_0680(char* self, int* out) __asm__("cComputer_checkForTargets");
extern "C" void cComputer_followPathCruise_0680(char* self, sPad_0680* pad) __asm__("cComputer_followPathCruise");

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_0680(const sV4_0680& v)
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
 * Sets the command's turn field from cComputer_calcTurn towards cComputer_calculateTargetPos's point (times 31). Then, in order: -
 * cComputer_checkForSpeedZone (speed in +0xDF0, zone flag): more than 138.9 over that speed sets the brake field to 31 when in the
 * zone; more than 138.9 under it crouches (cComputer_setupRecklessCrouch in the zone, else cComputer_setupCrouch). -
 * otherwise cComputer_checkForBoardPress (direction flag): fills the board-press field, the opposite way for the first 60 ticks
 * (counter +0xE40, then -1) and the given way after that. - otherwise clears +0xE40, sets bit 17 when the rider's
 * +0x2F8 is at least 0.9, its +0x2F0 is positive and +0xDF8 is over 50, and crouches. Last, a jump event
 * (cComputer_checkForJumpEvent) clears the target index +0xE70 and selects cruise entry 0; a target found by
 * cComputer_checkForTargets selects entry 3 when attacking, or entry 2 when that rider's AI (its +0x6C0 object, virtual
 * 7) returns 0. The entry's pointer-to-member goes to +0xF44 and its index to +0xF38. This function is entry 1 of the
 * table D_0043CF20.
 *
 * @param cruiseCmd  the cruise command to fill
 */
extern "C" void cComputer_followPathCruise_0680(char* self, sPad_0680* cruiseCmd)
{
    sV4_0680 targetPos;
    int x;
    int y;
    int below, toPipe, toRail, isSpin, isFlip;
    int z;
    cComputer_calculateTargetPos_0680(&targetPos, self);
    cruiseCmd->b20 = (int)(cComputer_calcTurn_0680(self, &targetPos) * 31.0f);
    if (cComputer_checkForSpeedZone_0680(self, (float*)(self + 0xDF0), &x)) {
        float sp = *(float*)(self + 0xDF0);
        float len = Len_0680(*(sV4_0680*)(*(char**)(self + 0x18) + 0x1E0));
        if (sp + 138.88890075683594f < len) {
            if (x)
                cruiseCmd->w0 = 0x1F;
        } else if (len < sp - 138.88890075683594f) {
            if (x)
                cComputer_setupRecklessCrouch_0680(self, cruiseCmd);
            else
                cComputer_setupCrouch_0680(self, cruiseCmd);
        }
    } else if (cComputer_checkForBoardPress_0680(self, &y)) {
        float turnSign = 1.0f;
        if (y)
            turnSign = -1.0f;
        int steerTimer = *(int*)(self + 0xE40);
        if (steerTimer == 0) {
            cruiseCmd->w6 = (int)(-turnSign * 31.0f);
            *(int*)(self + 0xE40) = 60;
        } else if (steerTimer > 0) {
            *(int*)(self + 0xE40) = steerTimer - 1;
            if (steerTimer - 1 <= 0)
                *(int*)(self + 0xE40) = -1;
            cruiseCmd->w6 = (int)(-turnSign * 31.0f);
        } else {
            cruiseCmd->w6 = (int)(turnSign * 31.0f);
        }
    } else {
        *(int*)(self + 0xE40) = 0;
        char* riderPtr = *(char**)(self + 0x18);
        if (*(float*)(riderPtr + 0x2F8) >= 0.8999999761581421f && *(float*)(riderPtr + 0x2F0) > 0.0f && *(float*)(self + 0xDF8) > 50.0f)
            cruiseCmd->b17 = 1;
        cComputer_setupCrouch_0680(self, cruiseCmd);
    }
    float* spd = (float*)(self + 0xDF0);
    z = 0;
    if (cComputer_checkForJumpEvent_0680(self, spd, &below, &toPipe, &toRail, &isSpin, &isFlip)) {
        *(int*)(self + 0xE70) = -1;
        *(Fn_0680*)(self + 0xF44) = D_0043CF20_0680[0];
        *(short*)(self + 0xF38) = 0;
    } else if (cComputer_checkForTargets_0680(self, &z)) {
        if (z) {
            *(Fn_0680*)(self + 0xF44) = D_0043CF20_0680[3];
            *(short*)(self + 0xF38) = 3;
        } else if (!((cAI_0680*)(D_004A28A8_0680->game->list->riders[*(int*)((char*)spd + 0x80)] + 0x6C0))->Busy()) {
            *(Fn_0680*)(self + 0xF44) = D_0043CF20_0680[2];
            *(short*)(self + 0xF38) = 2;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cComputer_jumpSetupCruise);
#ifdef SKIP_ASM
struct sVec4_09E0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_09E0 {
    unsigned int pad0 : 14;
    unsigned int b14 : 2;
    unsigned int pad16 : 4;
    int b20 : 6;
    unsigned int pad26 : 6;
    int w0 : 6;
};

class cObj_8E88;
typedef void (cObj_8E88::*Fn_8E88)();
extern "C" Fn_8E88 D_0043CF20[4];

extern "C" int cComputer_checkForJumpEvent(cComputer* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6);
extern "C" void cComputer_setupPrewindState_09E0(char*, int, int, int, int) __asm__("cComputer_setupPrewindState");
extern "C" void cComputer_calculateTargetPos_09E0(sVec4_09E0*, void*) __asm__("cComputer_calculateTargetPos");
extern "C" float cComputer_calcTurn_09E0(void*, sVec4_09E0*) __asm__("cComputer_calcTurn");
extern "C" void cComputer_followPathCruise(void*, void*);
extern "C" void cComputer_setupRecklessCrouch(cComputer*, void*);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_09E0(const sVec4_09E0& v)
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
 * (bits 20-25). At the jump start, within 10000 of its path (+0x4C8) and with the turn under 1 (always:
 * calcTurn caps it at 0.98) it calls setupPrewindState() and sets the 2-bit field at bit 14 to 3; else it
 * crouches recklessly when 5 km/h too slow, or sets the low field of word 1 (probably brake) to 31 when 5
 * km/h too fast.
 *
 * @param cmd  the cruise command being built (On Tour tCruiseCmd)
 */
extern "C" void cComputer_jumpSetupCruise(cComputer* self, sPad_09E0* cmd)
{
    int below, toPipe, toRail, isSpin, isFlip;
    if (!cComputer_checkForJumpEvent(self, (float*)(&self->mDesiredSpeed), &below, &toPipe, &toRail, &isSpin, &isFlip)) {
        *(Fn_8E88*)&self->cruiseLogic = D_0043CF20[1];
        self->mCruiseLogicID = 1;
        cComputer_followPathCruise(((char*)self), cmd);
        return;
    }
    sVec4_09E0 targetPos;
    cComputer_calculateTargetPos_09E0(&targetPos, ((char*)self));
    float steer = cComputer_calcTurn_09E0(((char*)self), &targetPos);
    cmd->b20 = (int)(steer * 31.0f);
    cRider* rider = self->mRider;
    float len = vLen_09E0(*(sVec4_09E0*)&rider->mVelocity);
    if (below && *(float*)(((char*)rider) + 0x4C8) < 10000.0f && vAbs_09E0(steer) < 1.0f) {
        cComputer_setupPrewindState_09E0(((char*)self), isSpin, isFlip, toRail, toPipe);
        cmd->b14 = 3;
        return;
    }
    float sp = self->mDesiredSpeed;
    if (len < sp - 138.88890075683594f) {
        cComputer_setupRecklessCrouch(self, cmd);
        return;
    }
    if (sp + 138.88890075683594f < len)
        cmd->w0 = 0x1F;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_attackTargetCruise);
#ifdef SKIP_ASM
struct sV4_0B90 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_0B90 {
    unsigned int pad0 : 20;
    int b20 : 6;
    unsigned int pad26 : 6;
    int w0 : 6;
};

struct sRec_0B90 {
    int active;
    int pad4;
    float dist;
    float angle;
    char pad10[0x24 - 0x10];
};

class cObj_0B90;
typedef void (cObj_0B90::*Fn_0B90)();
extern "C" Fn_0B90 D_0043CF20_0B90[4] __asm__("D_0043CF20");

class cAI_0B90 {
public:
    virtual void vm1();
    virtual sV4_0B90* GetVel();
    virtual void v2();
    virtual void v3();
    virtual sV4_0B90* GetPos();
    virtual void v5();
    virtual void v6();
    virtual int Busy();
};

class cRiderBase_0B90 {
public:
    char pad[0x6C0];
};

class cRider_0B90 : public cRiderBase_0B90, public cAI_0B90 {
};

struct sRiders_0B90 {
    char pad[0x28];
    cRider_0B90* riders[1];
};

struct sGame_0B90 {
    char pad[0xC];
    sRiders_0B90* list;
};

struct sWorld_0B90 {
    char pad[0x84];
    sGame_0B90* game;
};

struct sCtl_0B90 {
    float speed;
    char pad4[0x80 - 4];
    int target;
};

extern sWorld_0B90* D_004A28A8_0B90 __asm__("SSXApp");

extern "C" void cComputer_calculateTargetPos_0B90(sV4_0B90*, void*) __asm__("cComputer_calculateTargetPos");
extern "C" float cComputer_calcTurn_0B90(void*, sV4_0B90*) __asm__("cComputer_calcTurn");
extern "C" void cComputer_setupRecklessCrouch_0B90(void*, void*) __asm__("cComputer_setupRecklessCrouch");
extern "C" void cComputer_setupCrouch_0B90(void*, void*) __asm__("cComputer_setupCrouch");
extern "C" int cComputer_checkForJumpEvent_0B90(char* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6) __asm__("cComputer_checkForJumpEvent");
extern "C" int cComputer_checkForTargets_0B90(char* self, int* out) __asm__("cComputer_checkForTargets");
extern "C" float func_0031C228(float);
extern "C" void cRider_checkAIPathSwitch(void*, int);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_0B90(const sV4_0B90& v)
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
static inline sV4_0B90 Norm_0B90(const sV4_0B90& v)
{
    sV4_0B90 r;
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
static inline sV4_0B90 Add_0B90(const sV4_0B90& a, const sV4_0B90& b)
{
    sV4_0B90 r;
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
static inline sV4_0B90 Scale_0B90(const sV4_0B90& v, float s)
{
    sV4_0B90 r;
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

static inline sRec_0B90* Rec_0B90(char* self) { return &((sRec_0B90*)*(char**)(self + 0x18))[*(int*)(self + 0xE70)]; }

/**
 * Cruise logic 3: chases the target rider (mTargetID), crouching to catch up and braking to stay with it.
 *
 * With the target more than 90 degrees off its heading it follows the path and brakes (low field of word 1,
 * probably) when the target is over 200 away and slower. Otherwise it steers at a point 300 ahead of the
 * target along its velocity: within 15 degrees it crouches recklessly, within 50 it crouches, beyond that it
 * crouches when slower and brakes when faster. It then switches to jumpSetupCruise (dropping the target) at a
 * jump event, to followPathCruise when checkForTargets() finds none, or to avoidTargetCruise for a new target
 * it should not attack that is not human.
 *
 * @param cmd  the cruise command being built (On Tour tCruiseCmd)
 */
extern "C" void cComputer_attackTargetCruise(cComputer* self, sPad_0B90* cmd)
{
    int below, toPipe, toRail, isSpin, isFlip;
    int z;
    sRec_0B90* rec = Rec_0B90((char*)self);
    sRiders_0B90* list = D_004A28A8_0B90->game->list;
    sCtl_0B90* ctl = (sCtl_0B90*)(&self->mDesiredSpeed);
    cRider_0B90* tgt = list->riders[ctl->target];
    float diff = rec->angle - *(float*)((char*)self->mRider + 0x4CC);
    float ang = diff - Floor_0B90(diff * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
    float mySpd = Len_0B90(*(sV4_0B90*)&self->mRider->mVelocity);
    float absAng = Abs_0B90(ang);
    // Angle thresholds: 90, 50 and 15 degrees in radians.
    float behindAng = 1.5707964897155762f;
    float wideAng = 0.8726646900177002f;
    float narrowAng = 0.2617994248867035f;
    float hisSpd = Len_0B90(*tgt->GetVel());
    float steer;
    if (absAng > behindAng) {
        sV4_0B90 v;
        cComputer_calculateTargetPos_0B90(&v, ((char*)self));
        steer = cComputer_calcTurn_0B90(((char*)self), &v);
        if (Rec_0B90((char*)self)->dist > 200.0f && hisSpd < mySpd)
            cmd->w0 = 0x1F;
    } else {
        sV4_0B90 v = Norm_0B90(*tgt->GetVel());
        sV4_0B90 w = Add_0B90(*tgt->GetPos(), Scale_0B90(v, 300.0f));
        if (absAng < wideAng) {
            if (absAng < narrowAng)
                cComputer_setupRecklessCrouch_0B90(((char*)self), cmd);
            else
                cComputer_setupCrouch_0B90(((char*)self), cmd);
        } else if (mySpd < hisSpd) {
            cComputer_setupCrouch_0B90(((char*)self), cmd);
        } else if (hisSpd < mySpd) {
            cmd->w0 = 0x1F;
        }
        steer = cComputer_calcTurn_0B90(((char*)self), &w);
    }
    cmd->b20 = (int)(steer * 31.0f);
    z = 1;
    ctl = (sCtl_0B90*)(&self->mDesiredSpeed);
    if (cComputer_checkForJumpEvent_0B90(((char*)self), &ctl->speed, &below, &toPipe, &toRail, &isSpin, &isFlip)) {
        self->mTargetID = -1;
        *(Fn_0B90*)&self->cruiseLogic = D_0043CF20_0B90[0];
        self->mCruiseLogicID = 0;
    } else if (!cComputer_checkForTargets_0B90(((char*)self), &z)) {
        *(Fn_0B90*)&self->cruiseLogic = D_0043CF20_0B90[1];
        self->mCruiseLogicID = 1;
    } else if (!z && !list->riders[ctl->target]->Busy()) {
        *(Fn_0B90*)&self->cruiseLogic = D_0043CF20_0B90[2];
        self->mCruiseLogicID = 2;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cComputer_avoidTargetCruise);
#ifdef SKIP_ASM
struct sV4_0F88 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_0F88 {
    unsigned int pad0 : 20;
    int b20 : 6;
};

struct sRec_0F88 {
    int active;
    int pad4;
    float dist;
    float angle;
    char pad10[0x24 - 0x10];
};

class cObj_0F88;
typedef void (cObj_0F88::*Fn_0F88)();
extern "C" Fn_0F88 D_0043CF20_0F88[4] __asm__("D_0043CF20");

class cAI_0F88 {
public:
    virtual void vm1();
    virtual sV4_0F88* GetVel();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual int Ready();
};

class cRiderBase_0F88 {
public:
    char pad[0x6C0];
};

class cRider_0F88 : public cRiderBase_0F88, public cAI_0F88 {
};

struct sRiders_0F88 {
    char pad[0x28];
    cRider_0F88* riders[1];
};

struct sGame_0F88 {
    char pad[0xC];
    sRiders_0F88* list;
};

struct sWorld_0F88 {
    char pad[0x84];
    sGame_0F88* game;
};

struct sCtl_0F88 {
    float speed;
    char pad4[0x80 - 4];
    int target;
};

extern sWorld_0F88* D_004A28A8_0F88 __asm__("SSXApp");

extern "C" void cComputer_calculateTargetPos_0F88(sV4_0F88*, void*) __asm__("cComputer_calculateTargetPos");
extern "C" float cComputer_calcTurn_0F88(void*, sV4_0F88*) __asm__("cComputer_calcTurn");
extern "C" void cComputer_setupRecklessCrouch_0F88(void*, void*) __asm__("cComputer_setupRecklessCrouch");
extern "C" void cComputer_setupCrouch_0F88(void*, void*) __asm__("cComputer_setupCrouch");
extern "C" int cComputer_checkForJumpEvent_0F88(char* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6) __asm__("cComputer_checkForJumpEvent");
extern "C" int cComputer_checkForTargets_0F88(char* self, int* out) __asm__("cComputer_checkForTargets");
extern "C" float func_0031C228(float);
extern "C" void cRider_checkAIPathSwitch(void*, int);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_0F88(const sV4_0F88& v)
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

static inline sRec_0F88* Rec_0F88(char* self) { return &((sRec_0F88*)*(char**)(self + 0x18))[*(int*)(self + 0xE70)]; }

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
extern "C" void cComputer_avoidTargetCruise(cComputer* self, sPad_0F88* cmd)
{
    sV4_0F88 targetPos;
    int below, toPipe, toRail, isSpin, isFlip;
    int z;
    sCtl_0F88* ctl = (sCtl_0F88*)(&self->mDesiredSpeed);
    sRec_0F88* rec = Rec_0F88((char*)self);
    cRider_0F88* tgt = D_004A28A8_0F88->game->list->riders[ctl->target];
    float diff = rec->angle - *(float*)((char*)self->mRider + 0x4CC);
    float ang = diff - Floor_0F88(diff * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
    float mySpd = Len_0F88(*(sV4_0F88*)&self->mRider->mVelocity);
    float hisSpd = Len_0F88(*tgt->GetVel());
    float steer = 0.0f;
    if (Abs_0F88(ang) > 1.5707964897155762f) {
        cComputer_calculateTargetPos_0F88(&targetPos, ((char*)self));
        steer = cComputer_calcTurn_0F88(((char*)self), &targetPos);
        if (mySpd < hisSpd)
            cComputer_setupRecklessCrouch_0F88(((char*)self), cmd);
        else
            cComputer_setupCrouch_0F88(((char*)self), cmd);
    } else {
        float targetDist = Rec_0F88((char*)self)->dist;
        float lim;
        if (targetDist == 0.0f) {
            lim = 1.5707963705062866f;
        } else {
            lim = func_0031C228(150.0f / targetDist);
            if (targetDist < 0.0f)
                lim += 3.1415927410125732f;
        }
        if (Abs_0F88(ang) > lim || mySpd <= hisSpd) {
            cComputer_calculateTargetPos_0F88(&targetPos, ((char*)self));
            steer = cComputer_calcTurn_0F88(((char*)self), &targetPos);
            cComputer_setupCrouch_0F88(((char*)self), cmd);
        } else if (tgt->Ready()) {
            if (*(int*)((char*)tgt + 0xAB8) == *(int*)((char*)self->mRider + 0xAB8))
                cRider_checkAIPathSwitch((char*)self->mRider, 0);
            cComputer_calculateTargetPos_0F88(&targetPos, ((char*)self));
            steer = cComputer_calcTurn_0F88(((char*)self), &targetPos);
            cComputer_setupCrouch_0F88(((char*)self), cmd);
        }
    }
    cmd->b20 = (int)(steer * 31.0f);
    z = 0;
    if (cComputer_checkForJumpEvent_0F88(((char*)self), (float*)(&self->mDesiredSpeed), &below, &toPipe, &toRail, &isSpin, &isFlip)) {
        self->mTargetID = -1;
        *(Fn_0F88*)&self->cruiseLogic = D_0043CF20_0F88[0];
        self->mCruiseLogicID = 0;
    } else if (!cComputer_checkForTargets_0F88(((char*)self), &z)) {
        *(Fn_0F88*)&self->cruiseLogic = D_0043CF20_0F88[1];
        self->mCruiseLogicID = 1;
    } else if (z) {
        *(Fn_0F88*)&self->cruiseLogic = D_0043CF20_0F88[3];
        self->mCruiseLogicID = 3;
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
    cDangerMan_Reset((cDangerMan*)(this));
}
// PORT: the compiler names this ctor __13cTable_101310Pv; expose it under the address-derived name.
__asm__(".globl cDangerMan_cDangerMan\n.type cDangerMan_cDangerMan,@function\ncDangerMan_cDangerMan = __13cTable_101310Pv\n.size cDangerMan_cDangerMan,0x94\n");
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_Update);
#ifdef SKIP_ASM
// FAKEMATCH: `do { ... } while (0)` around the recs loop (its loop notes weight self's ref, so global-alloc takes self before zoneCheckAIIndex); plain form 99.31%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sVec4_13A8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sXform_13A8 {
    char pad[0x30];
    sVec4_13A8 pos;
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
struct sVEi_13A8 { short delta; short index; int (*fn)(void*, sObj_13A8*); };

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

struct sEnt_13A8 {
    float x;
    float y;
    int state;
    float px, py, pz;
    unsigned int id;
};

struct sRec_13A8 {
    int a;
    int b;
    int c;
};

struct sMgr_13A8 {
    sRiders_13A8* riders;
    sEnt_13A8 ents[64];
    int count;
    int pad708;
    sRec_13A8 recs[6];
    char pad[0xB58 - 0x70C - 6 * 0xC];
    sNode_13A8* list;
    int cur;
};

void cDangerMan_DeleteTrackedDangerElemRecord(void*, void*);
extern "C" int cDangerMan_CalcAppliedDangerLevel(sMgr_13A8*, int);

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
 * used element of the 64 tracked ones (+4, 0x1C bytes, state 2 = free): in state 1 its delay (+4) drops by 1/60 and it
 * is deleted (cDangerMan_DeleteTrackedDangerElemRecord) at 0; when its instance is gone or dead it enters state 1,
 * otherwise its position is copied from the instance. It ends with an empty loop over the riders. cAI_update calls it.
 */
// cDangerMan::Update(void) (SSX On Tour symbols; riders are cAI::getRider/getNumRiders, the zone lookup is
// cWorld::getTheWorld()->getInstanceResource(id)).
extern "C" void cDangerMan_Update(sMgr_13A8* self)
{
    if (self->list) {
        int zoneCheckAIIndex = self->cur;
        if (!(zoneCheckAIIndex < self->riders->getNumRiders()))
            zoneCheckAIIndex = 0;
        cRider* pRider = self->riders->getRider(zoneCheckAIIndex);
        char* sub = (char*)&pRider->mVtbl;
        sRec_13A8* pCurrentRiderTrack = &self->recs[zoneCheckAIIndex];
        sNode_13A8* pCurrDangerZone = self->list;
        int baseDangerZoneLevel = 0;
        do {
            sVEi_13A8* vt = *(sVEi_13A8**)&pRider->mVtbl;
            int (**pf)(void*, sObj_13A8*) = &vt[kRiderVt_isIntersecting].fn;
            if ((*pf)(sub + vt[kRiderVt_isIntersecting].delta, lookup_13A8(pCurrDangerZone->id))) {
                if (baseDangerZoneLevel < pCurrDangerZone->val)
                    baseDangerZoneLevel = pCurrDangerZone->val;
            }
            pCurrDangerZone = pCurrDangerZone->next;
        } while (pCurrDangerZone);
        if (baseDangerZoneLevel) {
            pCurrentRiderTrack->a = baseDangerZoneLevel;
            pCurrentRiderTrack->c = 60;
        }
        pCurrentRiderTrack->b = cDangerMan_CalcAppliedDangerLevel(self, zoneCheckAIIndex);
    }
    do { // FAKEMATCH: see header
    for (int i = 0; i < 6; i++) {
        sRec_13A8* riderTrack = &self->recs[i];
        if (riderTrack->a) {
            if (--riderTrack->c == 0) {
                riderTrack->a = 0;
                riderTrack->b = 0;
            }
        }
    }
    } while (0);
    if (self->count) {
        int numTrackedProcessed = self->count;
        for (int currElem = 0; currElem < 64; currElem++) {
            if (!numTrackedProcessed)
                break;
            sEnt_13A8* pCurrElem = &self->ents[currElem];
            if (pCurrElem->state == 2)
                continue;
            numTrackedProcessed--;
            if (pCurrElem->state == 1) {
                pCurrElem->y -= 0.01666666753590107f;
                if (pCurrElem->y <= 0.0f) {
                    cDangerMan_DeleteTrackedDangerElemRecord(self, pCurrElem);
                    continue;
                }
            }
            cComp_13A8* pInstNode = lookup_13A8(pCurrElem->id)->comp;
            if (pInstNode == 0 || pInstNode->IsDead()) {
                pCurrElem->state = 1;
            } else {
                sVec4_13A8 instPos = pInstNode->GetXform()->pos;
                pCurrElem->px = instPos.x;
                pCurrElem->py = instPos.y;
                pCurrElem->pz = instPos.z;
            }
        }
    }
    for (int i = 0; i < self->riders->count; i++) {
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_Reset);
#ifdef SKIP_ASM
struct sSlotA_101688 {
    int f0;
    int f4;
    int state;          // 0x8
    char padC[0x10];
};

struct sPair_101688 {
    int a;
    int b;
    int c;
};

struct sNode_101688 {
    int f0;
    int f4;
    int f8;
    sNode_101688* next; // 0xC
};

struct sTable_101688 {
    void* owner;                // 0x000
    sSlotA_101688 slots[64];    // 0x004
    int count;                  // 0x704
    int f708;                   // 0x708
    sPair_101688 pairs[6];      // 0x70C
    sNode_101688 nodes[64];     // 0x754
    sNode_101688* freeList;     // 0xB54
    int fB58;                   // 0xB58
    int fB5C;                   // 0xB5C
};

/**
 * Empties the danger manager: frees every tracked element and danger zone and clears the riders' danger
 * levels.
 *
 * Sets the tracked count to 0 and all 64 element states to 2 (free), clears the danger level and the
 * cDangerMan_CalcAppliedDangerLevel value of each of the 6 rider records, chains the 64 zone nodes into the free list, empties
 * the active zone list, sets the zone-check rider index to 0 and clears +0x708.
 *
 * @param vself  the danger manager (cDangerMan)
 */
extern "C" void cDangerMan_Reset(cDangerMan* vself)
{
    sTable_101688* self = (sTable_101688*)vself;
    int i;
    self->count = 0;
    int j;
    for (j = 0; j < 64; j++)
        self->slots[j].state = 2;
    for (i = 0; i < 6; i++) {
        self->pairs[i].a = 0;
        self->pairs[i].b = 0;
    }
    for (i = 0; i < 63; i++)
        self->nodes[i].next = &self->nodes[i + 1];
    self->freeList = self->nodes;
    self->nodes[63].next = 0;
    self->fB58 = 0;
    self->fB5C = 0;
    self->f708 = 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_AddDangerElem);
#ifdef SKIP_ASM
struct sVec4_1728 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sXform_1728 {
    char pad[0x30];
    sVec4_1728 pos;
};

class cComp_1728 {
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
    virtual sXform_1728* GetXform();
};

struct sObj_1728 {
    int pad[3];
    cComp_1728* comp;
};

struct sBank_1728 {
    char pad[0x1C];
    unsigned int* handles;
};

struct sWorld_1728 {
    int pad[2];
    sBank_1728** banks;
};

struct sEnt_1728 {
    float x;
    float y;
    int state;
    float px, py, pz;
    unsigned int id;
};

struct sMgr_1728 {
    int f0;
    sEnt_1728 ents[64];
    int count;
};

extern "C" void cDangerMan_RemoveDangerElem(cDangerMan*, unsigned int*);

static inline sObj_1728* lookup_1728(unsigned int id)
{
    sBank_1728* b = ((sWorld_1728*)cWorld_m_pTheWorld->m_pWorldCache)->banks[id & 0xFF];
    if (b) {
        unsigned int v = b->handles[id >> 8] >> 8;
        if (v)
            return (sObj_1728*)(v << 2); // PORT: packed pointer in a 32-bit handle
    }
    return 0;
}

/**
 * Starts tracking a world instance as a danger element.
 *
 * Does nothing when the instance (looked up by id in the world cache) is missing or dead. Otherwise drops any element
 * with the same id (cDangerMan_RemoveDangerElem), takes the first free element (state 2) of the 64 and fills it: state 0, the id,
 * @p x and @p y, and the instance's position; the element count +0x704 goes up. Nothing happens when all are in use.
 * DangerStartFunc calls it.
 *
 * @param idp  the instance id
 * @param x  stored in the element's first word
 * @param y  how long, in seconds, the element stays after its instance is gone (cDangerMan_Update counts it down)
 */
extern "C" void cDangerMan_AddDangerElem(cDangerMan* self, unsigned int* idp, float x, float y)
{
    cComp_1728* instNode = lookup_1728(*idp)->comp;
    if (instNode == 0)
        return;
    if (instNode->IsDead())
        return;
    cDangerMan_RemoveDangerElem(self, idp);
    for (int i = 0; i < 64; i++) {
        sEnt_1728* elemInfo = &((sEnt_1728*)&self->mTrackedDangerElems[0])[i];
        if (elemInfo->state == 2) {
            elemInfo->state = 0;
            elemInfo->id = *idp;
            elemInfo->x = x;
            elemInfo->y = y;
            sVec4_1728 instPos = instNode->GetXform()->pos;
            elemInfo->px = instPos.x;
            elemInfo->py = instPos.y;
            elemInfo->pz = instPos.z;
            self->mTrackedDangerElemNum++;
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
extern "C" void cDangerMan_RemoveDangerElem(cDangerMan* self, unsigned int* idp)
{
    int n = self->mTrackedDangerElemNum;
    if (n != 0) {
        int remaining = n;
        for (int i = 0; i < 64 && remaining != 0; i++) {
            sEnt_1728* elemInfo = &((sEnt_1728*)&self->mTrackedDangerElems[0])[i];
            if (elemInfo->state != 2) {
                remaining--;
                if (elemInfo->id == *idp) {
                    cDangerMan_DeleteTrackedDangerElemRecord(((sMgr_1728*)self), elemInfo);
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
struct N_1900 { int key; int val; int refs; N_1900 *next; };
struct P_1900 { char pad[0xB54]; N_1900 *freelist; N_1900 *used; };

/**
 * Adds a danger zone for an instance, or one more reference to it.
 *
 * When the zone list (+0xB58) already holds the id, its reference count goes up. Otherwise a node is taken from the
 * free list (+0xB54; there is no check for an empty list) and put at the front of the zone list with the id, @p level
 * and one reference. cDangerMan_Update tests riders against these zones. EnableDangerZoneFunc calls it.
 *
 * @param dangerMan  the danger manager
 * @param instId  the zone's instance id
 * @param level  the zone's danger level
 */
extern "C" void cDangerMan_AddDangerVolume(cDangerMan* dangerMan, int *instId, int level) {
    N_1900 *currActiveZone;
    N_1900 *n;

    for (currActiveZone = (N_1900*)dangerMan->mpActiveList; currActiveZone != 0; currActiveZone = currActiveZone->next) {
        if (currActiveZone->key == *instId) {
            currActiveZone->refs++;
            return;
        }
    }
    n = (N_1900*)dangerMan->mpInactiveList;
    dangerMan->mpInactiveList = (tDangerZoneTrack*)n->next;
    n->key = *instId;
    n->val = level;
    n->refs = 1;
    n->next = (N_1900*)dangerMan->mpActiveList;
    dangerMan->mpActiveList = (tDangerZoneTrack*)n;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cDangerMan_RemoveDangerVolume);
#ifdef SKIP_ASM
struct sNode_1970 { int key; int pad; int ref; sNode_1970* next; };
struct sC_1970 { int x, a, b; };
struct sSelf_1970 {
    char pad0[0x708];
    sC_1970 arrC[6];
    char pad1[0x754 - 0x750];
    char pad2[0xB54 - 0x754];
    sNode_1970* freeList;
    sNode_1970* used;
};
/**
 * Drops one reference to an instance's danger zone, and frees the zone at zero.
 *
 * The freed node goes back to the free list (+0xB54). When no zone is left, the zone level and applied level of all 6
 * rider records are cleared. DisableDangerZoneFunc calls it.
 *
 * @param key  the zone's instance id
 */
extern "C" void cDangerMan_RemoveDangerVolume(sSelf_1970* self, int* key) {
    sNode_1970* prev = 0;
    sNode_1970* n = self->used;
    while (n) {
        if (n->key == *key) {
            if (--n->ref == 0) {
                if (prev) prev->next = n->next;
                else self->used = n->next;
                n->next = self->freeList;
                self->freeList = n;
                if (self->used == 0) {
                    for (int k = 0; k < 6; k++) { self->arrC[k].a = 0; self->arrC[k].b = 0; }
                }
            }
            return;
        }
        prev = n;
        n = n->next;
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
 * @param dangerMan  the danger manager
 * @param aiIndex  the rider's index
 * @return the applied danger level
 */
extern "C" int cDangerMan_GetRiderDangerLevel(int dangerMan, int aiIndex) {
    return (*(int *)((char*)(((aiIndex * 0xC) + dangerMan)) + (0x710)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", FocusQuery_addcell);
#ifdef SKIP_ASM
struct sLink_1A28 {
    sLink_1A28* next;
};

struct sNode_1A28 {
    sNode_1A28* kids[8];
    sLink_1A28* items;
};

struct sList_1A28 {
    int n;
    sLink_1A28* items[1];
};

extern int cWScriptMan_TheWScriptMan;
extern "C" int cWScriptMan_isFocusInstance(int, sLink_1A28*);

/**
 * Adds a world cell's focus instances to a list, and optionally those of every cell below it.
 *
 * Each instance in the cell's list (+0x20) that cWScriptMan_isFocusInstance accepts is appended to @p list (count at
 * +0, then the pointers; no bound check). With @p recurse the same is done for each of the eight children, recursively.
 * The cell visitor func_00144368 and cFocusMan_update call it.
 *
 * @param list  the instance list to append to
 * @param node  the cell (eight child pointers, then the instance list)
 * @param recurse  nonzero to take the instances of all cells below too
 */
extern "C" void FocusQuery_addcell(sList_1A28* list, sNode_1A28* node, int recurse)
{
    sLink_1A28* item;
    for (item = node->items; item; item = item->next) {
        if (cWScriptMan_isFocusInstance(cWScriptMan_TheWScriptMan, item))
            list->items[list->n++] = item;
    }
    if (recurse) {
        if (node->kids[0]) FocusQuery_addcell(list, node->kids[0], 1);
        if (node->kids[1]) FocusQuery_addcell(list, node->kids[1], 1);
        if (node->kids[2]) FocusQuery_addcell(list, node->kids[2], 1);
        if (node->kids[3]) FocusQuery_addcell(list, node->kids[3], 1);
        if (node->kids[4]) FocusQuery_addcell(list, node->kids[4], 1);
        if (node->kids[5]) FocusQuery_addcell(list, node->kids[5], 1);
        if (node->kids[6]) FocusQuery_addcell(list, node->kids[6], 1);
        if (node->kids[7]) FocusQuery_addcell(list, node->kids[7], 1);
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
extern sApp_1B60* D_004A28A8_1B60 __asm__("SSXApp");
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
    sNode_1A28* cell;
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

extern t4Vector D_004A61E0;
extern "C" void FocusQuery_addcell_1B60(sFocusList* list, const sNode_1A28* node, int recurse) __asm__("FocusQuery_addcell");
extern "C" void MEM_copy(void* dst, void* src, int size);
extern "C" void func_0013FAD8(tFocusRec** first, tFocusRec** last, tFocusRec** out, const InstanceCompare& cmp);
extern "C" void cWScriptMan_downFocusScript(int, tFocusRec*);
extern "C" void cWScriptMan_upFocusScript(int, tFocusRec*);

struct FocusCallback;
// Retail's out-of-line copy of FocusCallback::operator() (matched in ai/control/handplantcontrol); the
// recursion inside the inline operator goes to it.
extern "C" void func_00144368_1B60(const FocusCallback* cb, const sNode_1A28* cell, const sNashKey* key) __asm__("func_00144368");

// FocusCallback: collects the instances of every cell that touches one of the focus boxes.
struct FocusCallback {
    sFocusList* list;
    sFocusBox* boxes;
    int count;

    void operator()(const sNode_1A28& cell, const sNashKey& key) const
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
        if (cell.kids[0]) func_00144368_1B60(this, cell.kids[0], &k);
        k.x++;
        if (cell.kids[4]) func_00144368_1B60(this, cell.kids[4], &k);
        k.y++;
        if (cell.kids[6]) func_00144368_1B60(this, cell.kids[6], &k);
        k.x--;
        if (cell.kids[2]) func_00144368_1B60(this, cell.kids[2], &k);
        k.z++;
        if (cell.kids[3]) func_00144368_1B60(this, cell.kids[3], &k);
        k.x++;
        if (cell.kids[7]) func_00144368_1B60(this, cell.kids[7], &k);
        k.y--;
        if (cell.kids[5]) func_00144368_1B60(this, cell.kids[5], &k);
        k.x--;
        if (cell.kids[1]) func_00144368_1B60(this, cell.kids[1], &k);
    }
};

// Visit root k of the tree (same shape as intersect/worldsphtree's visit helpers: the root's address is
// taken before the test).
static inline void Visit(const FocusCallback& cb, const sNashTree* tree, int k)
{
    const sNashRoot* r = &tree->roots[k];
    if (tree->roots[k].cell) {
        cb(*r->cell, r->key);
    }
}

/**
 * Updates which world instances are in focus around the focus positions, and runs their focus scripts.
 *
 * Builds a box of plus and minus D_004A61E0 around each focus position (count +0, entries of 0x20 bytes at +0x10). It
 * returns early unless a position moved 2000 or more since the last pass, 20 AI ticks (AITick) have passed, or there
 * was no pass yet (+0xD0 negative). Then it walks the eight roots of the game's nash cell tree (game +0x20): cells
 * crossing a box add their instances (FocusQuery_addcell), whole subtrees once below level 14. It stores the tick and the
 * positions. Instances newly in focus get flag 0x100; flag 0x200 follows the pass parity (+0xD4), and instances of the
 * previous pass (+0xD8 count, +0xDC list) not seen again lose 0x100. The new list replaces the old one (MEM_copy). The
 * instances that left focus are sorted (func_0013FAD8) and get cWScriptMan_downFocusScript, then those that entered get
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
        boxes[i].min = Sub_1B60(focusPos, D_004A61E0);
        boxes[i].max = Add_1B60(focusPos, D_004A61E0);
        if (Len_1B60(Sub_1B60(self->inst[i].last, focusPos)) >= 2000.0f) {
            moved = 1;
        }
    }
    if (!moved && self->lastTime >= 0 && AITick() - self->lastTime < 20) {
        return;
    }
    if (self->NumFocusInstances != 0) {
        sNashTree* tree = D_004A28A8_1B60->game->nash;
        FocusCallback cb;
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
        func_0013FAD8(unfocusinstances, unfocusinstances + numunfocusinstances, sorted, InstanceCompare());
        for (int i = 0; i < numunfocusinstances; i++) {
            cWScriptMan_downFocusScript(cWScriptMan_TheWScriptMan, sorted[i]);
        }
    }
    if (numfocusinstances) {
        func_0013FAD8(focusinstances, focusinstances + numfocusinstances, sorted, InstanceCompare());
        for (int i = 0; i < numfocusinstances; i++) {
            cWScriptMan_upFocusScript(cWScriptMan_TheWScriptMan, sorted[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_addfocusinstance);
#ifdef SKIP_ASM
struct S_32C0 {
    char pad[0xD4];
    int flag;
    int n;
    void *arr[8];
};

/**
 * Puts an instance into the focus manager's in-focus list.
 *
 * Sets the in-focus flag 0x100 in the instance's flags (+8) and sets its parity flag 0x200 to match the manager's pass
 * parity (+0xD4: cleared when that is nonzero, set when it is 0), as cFocusMan_update does, then appends it to the list
 * (+0xD8 count, +0xDC entries; no bound check). cObjectInterface_addToFocusBox uses it.
 *
 * @param focusMan  the focus manager
 * @param instance  the instance record
 */
extern "C" void cFocusMan_addfocusinstance(S_32C0 *focusMan, void *instance) {
    int temp_3;
    int temp_6;
    int var_2;
    int n;

    temp_3 = (*(int *)((char*)(instance) + (8)));
    temp_6 = temp_3 | 0x100;
    (*(int *)((char*)(instance) + (8))) = temp_6;
    if (focusMan->flag != 0) {
        var_2 = temp_6 & 0xFFFFFDFF;
    } else {
        var_2 = temp_3 | 0x300;
    }
    (*(int *)((char*)(instance) + (8))) = var_2;
    n = focusMan->n;
    focusMan->arr[n] = instance;
    focusMan->n = n + 1;
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
 * @param focusMan  the focus manager (this; cAI's FocusMan, +0xA4)
 * @param sectionId  the id of the section being unloaded, compared with each record's byte at +0x78
 */
extern "C" void cFocusMan_purgesection(cFocusMan* focusMan, int sectionId) {
    int n = 0;
    int i;

    for (i = 0; i < focusMan->NumFocusInstances; i++) {
        void *instance = ((void**)&focusMan->FocusInstances[0])[i];
        if (*(unsigned char *)((char*)instance + 0x78) != sectionId) {
            ((void**)&focusMan->FocusInstances[0])[n] = instance;
            n++;
        }
    }
    focusMan->NumFocusInstances = n;
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
extern "C" void cFocusMan_reset(cFocusMan* self) {
    int i;

    for (i = 0; i < self->NumFocusInstances; i++) {
        *(int *)((char*)((void**)&self->FocusInstances[0])[i] + 8) &= 0xFFFFFEFF;
    }
    self->NumFocusInstances = 0;
    self->LastUpdateTick = -1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_addbox);
#ifdef SKIP_ASM
struct V_33B0 { int x, y, z, w; } __attribute__((aligned(16)));

/**
 * Registers a focus box with the focus manager.
 *
 * Appends a record to Boxes (+0x10, 0x20 bytes each): the box pointer at +0x0 and a copy of the box's first 16 bytes at
 * +0x10, then NumBoxes (+0x0)++. LastUpdateTick (+0xD0) is reset to -1 so the next update recomputes the focus. There
 * is no capacity check here (On Tour asserts NumBoxes < MaxBoxes, 6 records). Callers: cRider_addFocusBox and
 * cScriptObjCameraManager_OnStartObjectUse.
 *
 * @param focusMan  the focus manager (this; cAI's FocusMan, +0xA4)
 * @param focusBox  the cFocusBox to add
 */
extern "C" void cFocusMan_addbox(cFocusMan* focusMan, V_33B0 *focusBox) {
    V_33B0 boxCopy;

    (focusMan->LastUpdateTick) = -1;
    (*(V_33B0 **)((char*)(focusMan) + ((focusMan->NumBoxes) << 5) + (0x10))) = focusBox;
    boxCopy = *focusBox;
    (*(V_33B0 *)((char*)((((focusMan->NumBoxes) << 5) + (int)focusMan)) + (0x20))) = boxCopy;
    (focusMan->NumBoxes) = (int) ((focusMan->NumBoxes) + 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_removebox);
#ifdef SKIP_ASM
struct sEntry1033F8 {
    int id;
    int data[7];
} __attribute__((aligned(16)));

struct sList1033F8 {
    int count;
    char pad4[0xC];
    sEntry1033F8 entries[6];    // 0x10
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
extern "C" void cFocusMan_removebox(cFocusMan* self, int id)
{
    int i;
    self->LastUpdateTick = -1;
    for (i = 0; i < self->NumBoxes; i++) {
        if (((sEntry1033F8*)&self->Boxes[0])[i].id == id) {
            for (i++; i < self->NumBoxes; i++) {
                ((sEntry1033F8*)&self->Boxes[0])[i - 1] = ((sEntry1033F8*)&self->Boxes[0])[i];
            }
            self->NumBoxes--;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream_3480 {
public:
    virtual void Write(void* p, int n);
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void WriteInt(int v);
};

struct sEnt_3480 {
    char d[0x20];
};

struct sObj_3480 {
    int n;
    char pad4[0x1C];
    sEnt_3480 ents[5];
    char padC0[0x10];
    int fD0;
    int pad;
    int m;
    int vals[1];
};

/**
 * Saves the focus manager's state into a replay frame.
 *
 * Through the frame's virtual block write it stores LastUpdateTick (+0xD0, 4 bytes) and the 16-byte box copy of each
 * registered record (Boxes +0x10, copy at +0x10 of each 0x20-byte record), then NumFocusInstances (+0xD8); each focus
 * instance record (+0xDC) goes through the frame's virtual pointer write (its fifth method). Called by
 * cReplay_saveFrame; cFocusMan_readFromReplayFrame reads it back.
 *
 * @param data  the replay frame (cReplayFramePtr) written to
 */
extern "C" void cFocusMan_writeToReplayFrame(cFocusMan* self, cStream_3480* data)
{
    data->Write(&self->LastUpdateTick, 4);
    for (int i = 0; i < self->NumBoxes; i++)
        data->Write(&((sEnt_3480*)&self->Boxes[0].LastUpdatePos)[i], 0x10);
    data->Write(&self->NumFocusInstances, 4);
    for (int j = 0; j < self->NumFocusInstances; j++)
        data->WriteInt(((int*)&self->FocusInstances[0])[j]);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cFocusMan_readFromReplayFrame);
#ifdef SKIP_ASM
struct sItem_3578 {
    int f0;
    int f4;
    unsigned int flags;
};

class cStream_3578 {
public:
    virtual void v0();
    virtual void Read(void* p, int n);
    virtual sItem_3578* ReadPtr();
};

struct sEnt_3578 {
    char d[0x20];
};

struct sObj_3578 {
    int n;
    char pad4[0x1C];
    sEnt_3578 ents[5];
    char padC0[0x10];
    int fD0;
    int fD4;
    int m;
    sItem_3578* items[1];
};

/**
 * Restores the focus manager's state from a replay frame.
 *
 * Reads back what cFocusMan_writeToReplayFrame stored: LastUpdateTick (+0xD0), the 16-byte copy of each registered box
 * record, NumFocusInstances (+0xD8) and the instance records (the frame's virtual pointer read, its third method). It
 * sets IsEven (+0xD4) to 1 and marks every restored record as in focus: flags word +0x8 gets 0x100 set and 0x200
 * cleared. Called by cReplay_restoreFrame.
 *
 * @param data  the replay frame (cReplayFramePtr) read from
 */
extern "C" void cFocusMan_readFromReplayFrame(cFocusMan* self, cStream_3578* data)
{
    data->Read(&self->LastUpdateTick, 4);
    for (int i = 0; i < self->NumBoxes; i++)
        data->Read(&((sEnt_3578*)&self->Boxes[0].LastUpdatePos)[i], 0x10);
    self->IsEven = 1;
    data->Read(&self->NumFocusInstances, 4);
    for (int j = 0; j < self->NumFocusInstances; j++) {
        sItem_3578* it = data->ReadPtr();
        it->flags = (it->flags | 0x100) & 0xFFFFFDFF;
        ((sItem_3578**)&self->FocusInstances[0])[j] = it;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_updateRule);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" char* cAIAnimEventMap_getAnimPlayAttr(int id);
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
 * Looks up the animation's play attributes (cAIAnimEventMap_getAnimPlayAttr(*anim): a 0x1C-byte record of D_00446990; On
 * Tour calls this lookup getAnimPlayAttr) and dispatches on its update rule (+0x4) to one of 16 handlers, cRiderAnim_onUpdateFWD
 * .. cRiderAnim_onUpdateASYNC_BP_BAL (On Tour onUpdateFWD .. onUpdateASYNC_BP_BAL). When the handler returns 0 and Rider (+0x60) is a
 * human (mIsHuman) local (mIsLocal) rider, it then runs cRiderAnim_checkForSoundEvents (On Tour checkForSoundEvents). Virtual: slot 2 of
 * cRiderAnim_vtbl.
 *
 * @param x  the animation sequencer (cAnimSequencer) being updated
 * @param anim  the sequence (cAnimSequence); its first word is the tAIAnimID
 * @param dt  the time step in seconds
 */
extern "C" void cRiderAnim_updateRule(cRiderAnim* self, void* x, int* anim, float dt)
{
    int done = 0;
    switch (*(int*)(cAIAnimEventMap_getAnimPlayAttr(*anim) + 4)) {
    case 0:
        done = f_001048C0_36A0(((char*)self), x, anim, dt);
        break;
    case 1:
        done = f_00104940_36A0(((char*)self), x, anim, dt);
        break;
    case 2:
        done = f_001049C0_36A0(((char*)self), x, anim, dt);
        break;
    case 3:
        done = f_00104110_36A0(((char*)self), x, anim, dt);
        break;
    case 4:
        done = f_00104178_36A0(((char*)self), x, anim, dt);
        break;
    case 5:
        done = f_00104238_36A0(((char*)self), x, anim, dt);
        break;
    case 6:
        done = f_001042A8_36A0(((char*)self), x, anim, dt);
        break;
    case 7:
        done = f_00104358_36A0(((char*)self), x, anim, dt);
        break;
    case 8:
        done = f_001045D8_36A0(((char*)self), x, anim, dt);
        break;
    case 9:
        done = f_00104660_36A0(((char*)self), x, anim, dt);
        break;
    case 10:
        done = f_001042E0_36A0(((char*)self), x, anim, dt);
        break;
    case 11:
        done = f_001043F8_36A0(((char*)self), x, anim, dt);
        break;
    case 12:
        done = f_001045B8_36A0(((char*)self), x, anim, dt);
        break;
    case 13:
        done = f_001047F0_36A0(((char*)self), x, anim, dt);
        break;
    case 14:
        done = f_00104728_36A0(((char*)self), x, anim, dt);
        break;
    case 15:
        done = f_001046B0_36A0(((char*)self), x, anim, dt);
        break;
    }
    if (done == 0) {
        cRider* rider = self->Rider;
        if (((char*)rider) != 0) {
            int ok = 0;
            if (rider->mIsHuman != 0) {
                ok = rider->mIsLocal != 0;
            }
            if (ok) {
                cRiderAnim_checkForSoundEvents_36A0(((char*)self), anim);
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
extern "C" char* cAIAnimEventMap_getAnimPlayAttr(int id);
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

struct sEvent3918 { int a; int b; };

struct sEventOwner3918 {
    int results[20];        // 0x00
    sEvent3918* events;     // 0x50
};

/**
 * Runs the expire rule of the animation that finished in a sequencer and records the animation to play next.
 *
 * The sequencer index is @p ev's position in the 8-byte sequencer array at +0x50. It looks up the play attributes of
 * the sequence's animation (cAIAnimEventMap_getAnimPlayAttr(*data); On Tour getAnimPlayAttr) and dispatches on the
 * expire rule (+0x8) to one of 11 handlers, cRiderAnim_onExpireNOP .. cRiderAnim_onExpireBLENDTO_GEAR_CYC (On Tour onExpireNOP, onExpireINTO_AIR, ...;
 * On Tour adds SK_ skier variants). The handler's result, the next tAIAnimID, goes into the word at +index*4; a rule
 * outside 0..10 stores nothing. Virtual: slot 3 of cRiderAnim_vtbl.
 *
 * @param ev  the sequencer whose animation expired (an element of the array at +0x50)
 * @param data  the expiring sequence; its first word is the tAIAnimID
 */
// Runs the handler for an animation event's type and stores its result in the per-event slot.
extern "C" void cRiderAnim_expireRule(cRiderAnim* self, sEvent3918* ev, int* data)
{
    int idx = ev - (sEvent3918*)self->Sequencer;
    char* info = cAIAnimEventMap_getAnimPlayAttr(*data);
    switch (*(int*)(info + 8)) {
    case 0:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireNOP_3918(((sEventOwner3918*)self), ev, data);
        break;
    case 1:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireINTO_AIR_3918(((sEventOwner3918*)self), ev, data);
        break;
    case 2:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireINTO_SPIN_CYC_3918(((sEventOwner3918*)self), ev, data);
        break;
    case 3:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireINTO_CRUISE_3918(((sEventOwner3918*)self), ev, data);
        break;
    case 4:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireBLENDTO_CRUISE_3918(((sEventOwner3918*)self), ev, data);
        break;
    case 5:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireBLENDTO_AIR_3918(((sEventOwner3918*)self), ev, data);
        break;
    case 6:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireBLENDTO_RAIL_3918(((sEventOwner3918*)self), ev, data);
        break;
    case 7:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireINTO_FL_STAND_3918(((sEventOwner3918*)self), ev, data);
        break;
    case 8:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireINTO_RSFS_3918(((sEventOwner3918*)self), ev, data);
        break;
    case 9:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireINTO_PRESS_3918(((sEventOwner3918*)self), ev, data);
        break;
    case 10:
        self->CurrentAnimID[idx] = cRiderAnim_onExpireBLENDTO_GEAR_CYC_3918(((sEventOwner3918*)self), ev, data);
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

struct sObj_3AA0 {
    int f0;
    unsigned int id;
    char pad[0xA8];
    char sub[1];
};

struct sBankTab_3AA0 {
    sBank_3AA0* banks[1];
};

extern sBankTab_3AA0* cAnimDataBase_TheAnimDataBase;
extern "C" int func_001446A0(void*, int);
extern "C" int func_001446B8(void*, int);
extern "C" void func_00144670(void*, int);
extern "C" void cRiderAnim_triggerAnimSound(cRiderAnim*, int);

/**
 * Plays the sounds tagged on the frames a rider animation just passed.
 *
 * The tag count and tag words come from cAnimDataBase_TheAnimDataBase (bank = id & 0xFF, entry = id >> 8 of the
 * sequence's animation id at +4). For each tag whose event the sequence's +0xB0 object reports through func_001446A0
 * and func_001446B8, and whose tag word has bit 15 set, it clears the event (func_00144670) and plays the sound
 * (cRiderAnim_triggerAnimSound with the low 15 bits). cRiderAnim_updateRule calls it for local human riders.
 *
 * @param sequence  the animation sequence
 */
extern "C" void cRiderAnim_checkForSoundEvents(cRiderAnim* self, sObj_3AA0* sequence)
{
    int n = cAnimDataBase_TheAnimDataBase->banks[sequence->id & 0xFF]->ents[sequence->id >> 8].count;
    for (int i = 0; i < n; i++) {
        int ok = 0;
        if (func_001446A0(sequence->sub, i) && func_001446B8(sequence->sub, i))
            ok = 1;
        if (ok) {
            sBank_3AA0* animBank = cAnimDataBase_TheAnimDataBase->banks[sequence->id & 0xFF];
            unsigned short soundEvent = animBank->recs[animBank->ents[sequence->id >> 8].first + i].b;
            if (soundEvent & 0x8000) {
                func_00144670(sequence->sub, i);
                cRiderAnim_triggerAnimSound(self, soundEvent & 0x7FFF);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_updatePosNegSlider);
#ifdef SKIP_ASM
extern char* cGameAnimMap_TheGameAnimMap;
extern "C" void cAnimSequence_setSubAnim(void*, int, int);
extern "C" void cAnimSequence_setSubAnimTime(void*, int, float);
extern "C" int cAnimSequence_updateWeight(void*, float);
extern "C" void cAnimSequencer_expire(void*, void*);
/**
 * Shows one of two animations at the point a signed weight selects, then advances the sequence's blend.
 *
 * For a negative @p weight sub-animation 0 becomes @p negAnim, otherwise @p posAnim (each mapped through the table at
 * +0x1030 of cGameAnimMap_TheGameAnimMap), at |weight| times the sequence's length (+0x10). Then the blend weight
 * advances by @p dt / 60 (cAnimSequence_updateWeight); when that reports the end, the sequence is expired
 * (cAnimSequencer_expire).
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param posAnim  the game animation for positive weights
 * @param negAnim  the game animation for negative weights
 * @param dt  the time step, multiplied by 1/60
 * @param weight  the slider position, -1 to 1
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_updatePosNegSlider(cRiderAnim* self, void* x, char* anim, int posAnim, int negAnim, float dt, float weight)
{
    if (weight < 0.0f) {
        cAnimSequence_setSubAnim(anim, 0, *(int*)(cGameAnimMap_TheGameAnimMap + negAnim * 4 + 0x1030));
        cAnimSequence_setSubAnimTime(anim, 0, -weight * *(float*)(anim + 0x10));
    } else {
        cAnimSequence_setSubAnim(anim, 0, *(int*)(cGameAnimMap_TheGameAnimMap + posAnim * 4 + 0x1030));
        cAnimSequence_setSubAnimTime(anim, 0, weight * *(float*)(anim + 0x10));
    }
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f) != 0) {
        cAnimSequencer_expire(x, anim);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_update3Cycle);
#ifdef SKIP_ASM
extern char* cGameAnimMap_TheGameAnimMap;
extern "C" void cAnimSequence_setSubAnimType(void*, int, int);
extern "C" void cAnimSequence_updateSubAnim(void*, int, float);
extern "C" void cAnimSequence_setSubAnim(void*, int, int);
extern "C" void cAnimSequence_setSubAnimTime(void*, int, float);
extern "C" void cAnimSequence_setSubAnimWeight(void*, int, float);
extern "C" int cAnimSequence_updateWeight(void*, float);
extern "C" void cAnimSequencer_expire(void*, void*);

/**
 * Blends a centre animation cycle with its negative or positive variant by a signed weight.
 *
 * Sub-animation 0 becomes @p cenAnim; sub-animation 1 becomes @p posAnim when @p w is positive, otherwise @p negAnim
 * (and w is negated); the weights are 1 - w and w (ids mapped through the table at +0x1030 of
 * cGameAnimMap_TheGameAnimMap). Sub-animation 0 loops (type 1) and advances by @p dt / 60; sub-animation 1 follows at
 * the same fraction of its own length (+0x2C). Then the blend weight advances, expiring the sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param negAnim  the game animation for negative weights
 * @param cenAnim  the centre game animation
 * @param dt  the time step, multiplied by 1/60
 * @param w  the weight, -1 to 1
 * @param posAnim  the game animation for positive weights
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_update3Cycle(cRiderAnim* self, void* x, char* anim, int negAnim, int cenAnim, float dt, float w, int posAnim)
{
    cAnimSequence_setSubAnim(anim, 0, *(int*)(cGameAnimMap_TheGameAnimMap + cenAnim * 4 + 0x1030));
    if (0.0f < w) {
        cAnimSequence_setSubAnim(anim, 1, *(int*)(cGameAnimMap_TheGameAnimMap + posAnim * 4 + 0x1030));
    } else {
        w = -w;
        cAnimSequence_setSubAnim(anim, 1, *(int*)(cGameAnimMap_TheGameAnimMap + negAnim * 4 + 0x1030));
    }
    cAnimSequence_setSubAnimWeight(anim, 0, 1.0f - w);
    cAnimSequence_setSubAnimWeight(anim, 1, w);
    cAnimSequence_setSubAnimType(anim, 0, 1);
    float stepTime = dt * 0.01666666753590107f;
    cAnimSequence_updateSubAnim(anim, 0, stepTime);
    cAnimSequence_setSubAnimTime(anim, 1, *(float*)(anim + 8) / *(float*)(anim + 0x10) * *(float*)(anim + 0x2C));
    if (cAnimSequence_updateWeight(anim, stepTime)) {
        cAnimSequencer_expire(x, anim);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_update5Cycle);
#ifdef SKIP_ASM
extern char* cGameAnimMap_TheGameAnimMap;
extern "C" void cAnimSequence_setSubAnimType(void*, int, int);
extern "C" void cAnimSequence_updateSubAnim(void*, int, float);
extern "C" void cAnimSequence_setSubAnim(void*, int, int);
extern "C" void cAnimSequence_setSubAnimTime(void*, int, float);
extern "C" void cAnimSequence_setSubAnimWeight(void*, int, float);
extern "C" int cAnimSequence_updateWeight(void*, float);
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
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_update5Cycle(cRiderAnim* self, void* x, char* anim, int negEndAnim, int negMidAnim, int cenAnim, int posMidAnim, int posEndAnim, float dt, float weight)
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
    if (*(int*)(cGameAnimMap_TheGameAnimMap + from * 4 + 0x1030) != *(int*)(anim + 4)) {
        float frameFraction = *(float*)(anim + 8) / *(float*)(anim + 0x10);
        cAnimSequence_setSubAnim(anim, 0, *(int*)(cGameAnimMap_TheGameAnimMap + from * 4 + 0x1030));
        cAnimSequence_setSubAnimTime(anim, 0, *(float*)(anim + 0x10) * frameFraction);
    }
    cAnimSequence_setSubAnim(anim, 1, *(int*)(cGameAnimMap_TheGameAnimMap + to * 4 + 0x1030));
    cAnimSequence_setSubAnimWeight(anim, 0, w);
    cAnimSequence_setSubAnimWeight(anim, 1, 1.0f - w);
    cAnimSequence_setSubAnimType(anim, 0, 1);
    float stepTime = dt * 0.01666666753590107f;
    cAnimSequence_updateSubAnim(anim, 0, stepTime);
    cAnimSequence_setSubAnimTime(anim, 1, *(float*)(anim + 8) / *(float*)(anim + 0x10) * *(float*)(anim + 0x2C));
    if (cAnimSequence_updateWeight(anim, stepTime)) {
        cAnimSequencer_expire(x, anim);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_updateIntoCycle);
#ifdef SKIP_ASM
extern char* cGameAnimMap_TheGameAnimMap;
extern "C" void cAnimSequence_setSubAnimType(void*, int, int);
extern "C" void cAnimSequence_updateSubAnim(void*, int, float);
extern "C" void cAnimSequence_setSubAnim(void*, int, int);
extern "C" void cAnimSequence_setSubAnimTime(void*, int, float);
extern "C" int cAnimSequence_updateWeight(void*, float);
extern "C" void cAnimSequencer_expire(void*, void*);

/**
 * Plays an animation once and then switches the sequence to a looping one.
 *
 * While sub-animation 0 is @p intoAnim (mapped through the table at +0x1030 of cGameAnimMap_TheGameAnimMap) it plays
 * once (type 0) by @p dt / 60, and once its time reaches its length it becomes @p cycleAnim, looping (type 1) from time
 * 0. Otherwise sub-animation 0 just advances. Then the blend weight advances, expiring the sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence
 * @param intoAnim  the game animation played first
 * @param dt  the time step, multiplied by 1/60
 * @param cycleAnim  the looping game animation that follows
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_updateIntoCycle(cRiderAnim* self, void* x, char* anim, int intoAnim, float dt, int cycleAnim)
{
    if (*(int*)(anim + 4) == *(int*)(cGameAnimMap_TheGameAnimMap + intoAnim * 4 + 0x1030)) {
        cAnimSequence_setSubAnimType(anim, 0, 0);
        cAnimSequence_updateSubAnim(anim, 0, dt * 0.01666666753590107f);
        if (*(float*)(anim + 8) >= *(float*)(anim + 0x10)) {
            cAnimSequence_setSubAnim(anim, 0, *(int*)(cGameAnimMap_TheGameAnimMap + cycleAnim * 4 + 0x1030));
            cAnimSequence_setSubAnimType(anim, 0, 1);
            cAnimSequence_setSubAnimTime(anim, 0, 0.0f);
        }
    } else {
        cAnimSequence_updateSubAnim(anim, 0, dt * 0.01666666753590107f);
    }
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f)) {
        cAnimSequencer_expire(x, anim);
        return 1;
    }
    return 0;
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
 * The weight is the rider's (+0x60) +0x1FC, negated when the rider's +0x320 is set. Sequence 7 blends game animations
 * 17 / 15 / 16, sequence 10 26 / 23 / 25, any other 26 / 24 / 25 (negative / centre / positive) with cRiderAnim_update3Cycle.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the sequence (first word: its animation id)
 * @param deltaTime  the time step
 * @return cRiderAnim_update3Cycle's result: 1 when the sequence was expired
 */
extern "C" int cRiderAnim_onUpdateASYNC_CRUISE3(cRiderAnim* riderAnim, void *sequencer, int *sequence, float deltaTime) {
    float var_f13;
    void *temp_3;

    temp_3 = ((void*)riderAnim->Rider);
    var_f13 = (*(float *)((char*)(temp_3) + (0x1FC)));
    if ((*(int *)((char*)(temp_3) + (0x320))) != 0) {
        var_f13 = -var_f13;
    }
    if (*sequence == 7) {
        return cRiderAnim_update3Cycle_4110(riderAnim, sequencer, sequence, 17, 0xF, 0x10, deltaTime, var_f13);
    }
    if (*sequence == 10) {
        return cRiderAnim_update3Cycle_4110(riderAnim, sequencer, sequence, 26, 23, 25, deltaTime, var_f13);
    }
    return cRiderAnim_update3Cycle_4110(riderAnim, sequencer, sequence, 26, 0x18, 25, deltaTime, var_f13);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_CRUISE5);
#ifdef SKIP_ASM
extern "C" int cRiderAnim_update5Cycle(cRiderAnim* self, void* x, char* anim, int i0, int i1, int i2, int i3, int i4, float dt, float v);
/**
 * Update rule 4 of cRiderAnim_updateRule: a five-way cruise blend.
 *
 * The weight is the rider's (+0x60) +0x1FC, negated when the rider's +0x320 is set. The sequence picks the five game
 * animations (negative end, negative middle, centre, positive middle, positive end): 6 gives 14 13 10 11 12, 8 gives 22
 * 21 18 19 20, 14 gives 32 31 27 29 30, 15 gives 32 31 28 29 30, any other 9 8 5 6 7. They go to cRiderAnim_update5Cycle.
 *
 * @param x  the sequencer
 * @param anim  the sequence (first word: its animation id)
 * @param dt  the time step
 * @return cRiderAnim_update5Cycle's result: 1 when the sequence was expired
 */
extern "C" int cRiderAnim_onUpdateASYNC_CRUISE5(cRiderAnim* self, void* x, int* anim, float dt)
{
    cRider* rider = self->Rider;
    float weight = rider->mSlewed[1].cur;
    if (rider->IsRidingFakie != 0)
        weight = -weight;
    int negEndAnim, negMidAnim, cenAnim, posMidAnim, posEndAnim;
    int animId = *anim;
    if (animId == 6) {
        negEndAnim = 0xE; negMidAnim = 13; cenAnim = 10; posMidAnim = 11; posEndAnim = 0xC;
    } else if (animId == 8) {
        negEndAnim = 22; negMidAnim = 21; cenAnim = 18; posMidAnim = 19; posEndAnim = 20;
    } else if (animId == 14) {
        negEndAnim = 0x20; negMidAnim = 0x1F; cenAnim = 27; posMidAnim = 29; posEndAnim = 0x1E;
    } else if (animId != 15) {
        negEndAnim = 9; negMidAnim = 8; cenAnim = 5; posMidAnim = 6; posEndAnim = 7;
    } else {
        negEndAnim = 0x20; negMidAnim = 0x1F; cenAnim = 0x1C; posMidAnim = 29; posEndAnim = 0x1E;
    }
    return cRiderAnim_update5Cycle(self, x, (char*)anim, negEndAnim, negMidAnim, cenAnim, posMidAnim, posEndAnim, dt, weight);
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
 * The weight is the rider's (+0x60) +0x238, negated when the rider's +0x320 is set. Sequence 18 blends game animations
 * 65 / 63 / 64, sequence 19 68 / 66 / 67, any other 71 / 69 / 70 (negative / centre / positive) with cRiderAnim_update3Cycle. The
 * C declares no result, but cRiderAnim_update3Cycle's is left in the return register.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the sequence (first word: its animation id)
 * @param deltaTime  the time step
 */
extern "C" void cRiderAnim_onUpdateASYNC_RAIL(cRiderAnim* riderAnim, void *sequencer, int *sequence, float deltaTime) {
    int var_7;
    int var_8;
    int var_9;
    float var_f13;
    void *temp_3;

    temp_3 = ((void*)riderAnim->Rider);
    var_f13 = (*(float *)((char*)(temp_3) + (0x238)));
    if ((*(int *)((char*)(temp_3) + (0x320))) != 0) {
        var_f13 = -var_f13;
    }
    switch (*sequence) {
    case 18:
        var_7 = 65;
        var_8 = 0x3F;
        var_9 = 0x40;
        break;
    default:
        var_7 = 71;
        var_8 = 69;
        var_9 = 70;
        break;
    case 19:
        var_7 = 68;
        var_8 = 66;
        var_9 = 67;
        break;
    }
    cRiderAnim_update3Cycle_4238(riderAnim, sequencer, sequence, var_7, var_8, var_9, deltaTime, var_f13);
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
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the sequence
 * @param dt  the time step
 */
extern "C" void cRiderAnim_onUpdateASYNC_BRAKE(cRiderAnim* riderAnim, void *sequencer, char *sequence, float dt) {
    float var_f13;
    void *temp_3;

    temp_3 = ((void*)riderAnim->Rider);
    var_f13 = (*(float *)((char*)(temp_3) + (0x214)));
    if ((*(int *)((char*)(temp_3) + (0x320))) == 0) {
        var_f13 = -var_f13;
    }
    cRiderAnim_update3Cycle_42A8(riderAnim, sequencer, sequence, 36, 33, 34, dt, var_f13);
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
 * Sequence 218 uses game animations 362 / 363, 226 371 / 372, 234 380 / 381, any other 389 / 390 (positive / negative),
 * with the rider's (+0x60) +0x238 as the weight, through cRiderAnim_updatePosNegSlider.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the sequence (first word: its animation id)
 * @param deltaTime  the time step
 * @return cRiderAnim_updatePosNegSlider's result: 1 when the sequence was expired
 */
extern "C" int cRiderAnim_onUpdateASYNC_UBERGRIND(cRiderAnim* riderAnim, void *sequencer, int *sequence, float deltaTime) {
    int x = *sequence;
    if (x == 218) {
        return cRiderAnim_updatePosNegSlider_42E0(riderAnim, sequencer, sequence, 362, 363, deltaTime, (riderAnim->Rider->mSlewed[6].cur));
    }
    if (x == 226) {
        return cRiderAnim_updatePosNegSlider_42E0(riderAnim, sequencer, sequence, 371, 372, deltaTime, (riderAnim->Rider->mSlewed[6].cur));
    }
    if (x == 234) {
        return cRiderAnim_updatePosNegSlider_42E0(riderAnim, sequencer, sequence, 380, 381, deltaTime, (riderAnim->Rider->mSlewed[6].cur));
    }
    return cRiderAnim_updatePosNegSlider_42E0(riderAnim, sequencer, sequence, 389, 390, deltaTime, (riderAnim->Rider->mSlewed[6].cur));
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
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_onUpdateASYNC_PREWIND(cRiderAnim* self, void* x, char* anim, float dt) {
    cRider* rider = self->Rider;
    float absSlew15 = vAbs_4358(rider->mSlewed[15].cur);
    float absSlew16 = vAbs_4358(rider->mSlewed[16].cur);
    float framePercent = (absSlew16 < absSlew15) ? absSlew15 : absSlew16;
    cAnimSequence_setSubAnimTime(anim, 0, framePercent * *(float*)(anim + 0x10));
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f) != 0) {
        cAnimSequencer_expire(x, anim);
        return 1;
    }
    return 0;
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
 * Sequences 297-304 select a looping sub-animation 1 (game animations 165-172; 165 for others). The amount is the
 * larger of |rider +0x298| and |rider +0x28C| (rider at +0x60), clamped to 0..1: sub-animation 1 gets weight 0.25 +
 * 0.75 * amount and sub-animation 0 the rest, sub-animation 0's time is the amount times the length, and sub-animation
 * 1 advances by @p dt / 60. Then the blend weight advances, expiring the sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence (first word: its animation id)
 * @param dt  the time step
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_onUpdateASYNC_AIR_ADJ(cRiderAnim* self, void* x, char* anim, float dt)
{
    int slot = 165;
    switch (*(int*)anim) {
    case 297: slot = 165; break;
    case 298: slot = 171; break;
    case 299: slot = 168; break;
    case 300: slot = 172; break;
    case 301: slot = 166; break;
    case 302: slot = 170; break;
    case 303: slot = 167; break;
    case 304: slot = 169; break;
    }
    cAnimSequence_setSubAnim(anim, 1, *(int*)(cGameAnimMap_TheGameAnimMap + slot * 4 + 0x1030));
    cAnimSequence_setSubAnimType(anim, 1, 1);
    cRider* rider = self->Rider;
    float absSlew14 = vAbs_43F8(rider->mSlewed[14].cur);
    float absSlew13 = vAbs_43F8(rider->mSlewed[13].cur);
    float framePercent = (absSlew13 < absSlew14) ? absSlew14 : absSlew13;
    framePercent = clamp_43F8(framePercent, 0.0f, 1.0f);
    float w = framePercent * 0.75f + 0.25f;
    cAnimSequence_setSubAnimWeight(anim, 0, 1.0f - w);
    cAnimSequence_setSubAnimWeight(anim, 1, w);
    cAnimSequence_setSubAnimTime(anim, 0, framePercent * *(float*)(anim + 0x10));
    float stepTime = dt * 0.01666666753590107f;
    cAnimSequence_updateSubAnim(anim, 1, stepTime);
    if (cAnimSequence_updateWeight(anim, stepTime)) {
        cAnimSequencer_expire(x, anim);
        return 1;
    }
    return 0;
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
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_onUpdateASYNC_GATEROCK(cRiderAnim* self, void* x, char* anim, float dt) {
    cAnimSequence_setSubAnimTime(anim, 0, *(float*)(anim + 0x10) * *(float*)((char*)self->Rider->mRiderHandler + 0x2A0));
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f) != 0) {
        cAnimSequencer_expire(x, anim);
        return 1;
    }
    return 0;
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
 * Sequence 45 uses game animations 353 / 352, any other 347 / 346 (positive / negative), with the rider's (+0x60)
 * +0x244 as the weight, through cRiderAnim_updatePosNegSlider.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the sequence (first word: its animation id)
 * @param deltaTime  the time step
 * @return cRiderAnim_updatePosNegSlider's result: 1 when the sequence was expired
 */
extern "C" int cRiderAnim_onUpdateASYNC_HANDPLANT(cRiderAnim* riderAnim, void *sequencer, int *sequence, float deltaTime) {
    float w = (riderAnim->Rider->mSlewed[7].cur);

    if (*sequence == 45) {
        return cRiderAnim_updatePosNegSlider_4660(riderAnim, sequencer, sequence, 353, 352, deltaTime, w);
    }
    return cRiderAnim_updatePosNegSlider_4660(riderAnim, sequencer, sequence, 347, 346, deltaTime, w);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_onUpdateASYNC_BP_BAL);
#ifdef SKIP_ASM
// PORT: g++ >?/<? (min/max) operator, removed in GCC 4.3.
extern "C" int cRiderAnim_update3Cycle(cRiderAnim* self, void* x, char* anim, int a, int b, float dt, float w, int c);
/**
 * Update rule 15 of cRiderAnim_updateRule: a three-way board-press balance blend.
 *
 * The weight is 2 * (rider +0x274) - 1 (rider at +0x60), clamped to -1..1. Sequence 28 blends game animations 49 / 48 /
 * 50, any other 60 / 58 / 59 (negative / centre / positive) with cRiderAnim_update3Cycle.
 *
 * @param x  the sequencer
 * @param anim  the sequence (first word: its animation id)
 * @param dt  the time step
 * @return cRiderAnim_update3Cycle's result: 1 when the sequence was expired
 */
extern "C" int cRiderAnim_onUpdateASYNC_BP_BAL(cRiderAnim* self, void* x, int* anim, float dt)
{
    float centeredSlew = self->Rider->mSlewed[11].cur;
    centeredSlew = centeredSlew + centeredSlew - 1.0f;
    float w;
    if (centeredSlew >= -1.0f) {
        w = centeredSlew <? 1.0f;
    } else {
        w = -1.0f;
    }
    int negAnim, cenAnim, posAnim;
    negAnim = 0x3C;
    if (*anim != 0x1C) {
        cenAnim = 58; posAnim = 59;
    } else {
        negAnim = 49; cenAnim = 0x30; posAnim = 50;
    }
    return cRiderAnim_update3Cycle(self, x, (char*)anim, negAnim, cenAnim, dt, w, posAnim);
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
 * The amount is the rider's (+0x60) +0x268; sequences 25 and 33 use 1 minus it, clamped to 0..1. Sub-animation 0's time
 * is the amount times the length. Then the blend weight advances by @p dt / 60, expiring the sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence (first word: its animation id)
 * @param dt  the time step
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_onUpdateASYNC_BP_INTO(cRiderAnim* self, void* x, char* anim, float dt) {
    float framePercent;
    int animId = *(int*)anim;
    if (animId == 25 || animId == 33) {
        framePercent = clamp_4728(1.0f - self->Rider->mSlewed[10].cur, 0.0f, 1.0f);
    } else {
        framePercent = self->Rider->mSlewed[10].cur;
    }
    cAnimSequence_setSubAnimTime(anim, 0, framePercent * *(float*)(anim + 0x10));
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f) != 0) {
        cAnimSequencer_expire(x, anim);
        return 1;
    }
    return 0;
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
 * The amount is |rider +0x280| (rider at +0x60); sequences 37 and 38 use 1 minus it. Clamped to 0..1, times the length,
 * it sets sub-animation 0's time. Then the blend weight advances by @p dt / 60, expiring the sequence at its end.
 *
 * @param x  the sequencer
 * @param anim  the sequence (first word: its animation id)
 * @param dt  the time step
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_onUpdateASYNC_BP_PIVOT(cRiderAnim* self, void* x, char* anim, float dt) {
    float framePercent;
    int animId = *(int*)anim;
    if (animId == 37) {
        framePercent = 1.0f - vAbs_47F0(self->Rider->mSlewed[12].cur);
    } else if (animId == 38) {
        framePercent = 1.0f - vAbs_47F0(self->Rider->mSlewed[12].cur);
    } else {
        framePercent = vAbs_47F0(self->Rider->mSlewed[12].cur);
    }
    cAnimSequence_setSubAnimTime(anim, 0, clamp_47F0(framePercent, 0.0f, 1.0f) * *(float*)(anim + 0x10));
    if (cAnimSequence_updateWeight(anim, dt * 0.01666666753590107f) != 0) {
        cAnimSequencer_expire(x, anim);
        return 1;
    }
    return 0;
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
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_onUpdateFWD(cRiderAnim* self, void* x, void* anim, float dt) {
    float stepTime = dt * 0.01666666753590107f;
    int done;
    if (cAnimSequence_updateSubAnim_48C0(anim, 0, stepTime) != 0) {
        done = 1;
    } else {
        done = cAnimSequence_updateWeight(anim, stepTime);
    }
    if (done == 0) {
        return 0;
    }
    cAnimSequencer_expire(x, anim);
    return 1;
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
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_onUpdateHOLD_LAST(cRiderAnim* self, void* x, void* anim, float dt) {
    float stepTime = dt * 0.01666666753590107f;
    int done;
    if (cAnimSequence_updateSubAnim_4940(anim, 0, stepTime) != 0) {
        done = 1;
    } else {
        done = cAnimSequence_updateWeight(anim, stepTime);
    }
    if (done == 0) {
        return 0;
    }
    cAnimSequencer_expire(x, anim);
    return 1;
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
 * @return 1 when the sequence was expired, else 0
 */
extern "C" int cRiderAnim_onUpdateCYCLE(cRiderAnim* self, void* x, void* anim, float dt) {
    float stepTime = dt * 0.01666666753590107f;
    int done;
    if (cAnimSequence_updateSubAnim_49C0(anim, 0, stepTime) != 0) {
        done = 1;
    } else {
        done = cAnimSequence_updateWeight(anim, stepTime);
    }
    if (done == 0) {
        return 0;
    }
    cAnimSequencer_expire(x, anim);
    return 1;
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
 * Sequences 271 / 285 play 291; 272, 280, 281, 286 play 292; 269, 278, 283 play 289; 270, 279, 284 play 290; 273-276
 * play 293-296 (cRiderAnimBase_playInto). Any other returns 438 without playing.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence (first word: its animation id)
 * @return cRiderAnimBase_playInto's result, or 438
 */
// Plays the landing animation that matches the current trick state.
extern "C" int cRiderAnim_onExpireINTO_SPIN_CYC(cRiderAnim* riderAnim, void* sequencer, int* sequence)
{
    switch (*sequence) {
    case 271: case 285:
        return cRiderAnimBase_playInto_4A60(riderAnim, sequencer, sequence, 291);
    case 272: case 280: case 281: case 286:
        return cRiderAnimBase_playInto_4A60(riderAnim, sequencer, sequence, 292);
    case 269: case 278: case 283:
        return cRiderAnimBase_playInto_4A60(riderAnim, sequencer, sequence, 289);
    case 270: case 279: case 284:
        return cRiderAnimBase_playInto_4A60(riderAnim, sequencer, sequence, 290);
    case 273:
        return cRiderAnimBase_playInto_4A60(riderAnim, sequencer, sequence, 293);
    case 274:
        return cRiderAnimBase_playInto_4A60(riderAnim, sequencer, sequence, 294);
    case 275:
        return cRiderAnimBase_playInto_4A60(riderAnim, sequencer, sequence, 295);
    case 276:
        return cRiderAnimBase_playInto_4A60(riderAnim, sequencer, sequence, 296);
    }
    return 438;
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
 * func_00104D50), otherwise 287.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 * @return cRiderAnimBase_playBlendIn's result
 */
extern "C" int cRiderAnim_onExpireBLENDTO_AIR(cRiderAnim* riderAnim, int sequencer, int sequence) {
    if ((int)riderAnim->FERider != 0) {
        return cRiderAnimBase_playBlendIn_4B48(riderAnim, sequencer, sequence, 436);
    }
    return cRiderAnimBase_playBlendIn_4B48(riderAnim, sequencer, sequence, 287);
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
 * Plays animation 28 (cRiderAnimBase_playBlendIn) when the rider's (+0x60) +0x330 is 1, otherwise 36.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence
 * @return cRiderAnimBase_playBlendIn's result
 */
extern "C" int cRiderAnim_onExpireINTO_PRESS(cRiderAnim* riderAnim, int sequencer, int sequence) {
    if ((riderAnim->Rider->PressMode) == 1) {
        return cRiderAnimBase_playBlendIn_4BD8(riderAnim, sequencer, sequence, 0x1C);
    }
    return cRiderAnimBase_playBlendIn_4BD8(riderAnim, sequencer, sequence, 36);
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
 * Sequence 69 blends into animation 20, 70 into 19, any other into 18 (cRiderAnimBase_playBlendIn). Its result is left
 * in the return register (cRiderAnim_expireRule stores it) although the C declares none.
 *
 * @param riderAnim  the rider animation object
 * @param sequencer  the sequencer
 * @param sequence  the expired sequence (first word: its animation id)
 */
extern "C" void cRiderAnim_onExpireBLENDTO_RAIL(int riderAnim, int sequencer, int *sequence) {
    int var_7;
    switch (*sequence) {
    case 69:
        var_7 = 20;
        break;
    case 70:
        var_7 = 19;
        break;
    default:
        var_7 = 18;
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
extern "C" void cSSXAudio_PlayAniTag_4CC8(int, int, int) __asm__("cSSXAudio_PlayAniTag");
extern int gSSXAudio_seg_1218 __asm__("gSSXAudio");

/**
 * Plays an animation sound tag for the rider: cSSXAudio_PlayAniTag(gSSXAudio, rider, id & 0xFFFF) when the rider
 * (+0x60) is set.
 *
 * cRiderAnim_checkForSoundEvents calls it.
 *
 * @param riderAnim  the rider animation object
 * @param soundId  the sound tag
 */
extern "C" void cRiderAnim_triggerAnimSound(cRiderAnim* riderAnim, int soundId) {
    int temp_2 = ((int)riderAnim->Rider);
    int maskedSoundId = soundId & 0xFFFF;
    if (temp_2 != 0) {
        cSSXAudio_PlayAniTag_4CC8(gSSXAudio_seg_1218, temp_2, maskedSoundId);
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
 * The animation filter comes from FERider (+0x64, word +0xCD8) when linked to a front-end rider, else from Rider
 * (+0x60, word +0x364), else 0. The result is cAIAnimEventMap_mapAIDToGAD(cRiderAnimBase_AIAnimEventMap, aid, filter) (On Tour
 * cAIAnimEventMap::mapAIDToGAD); the C view is void and the id passes through in v0. Virtual: slot 4 of
 * cRiderAnim_vtbl.
 *
 * @param riderAnim  the rider animation object (this)
 * @param aid  the AI animation id (tAIAnimID)
 */
extern "C" void cRiderAnim_mapAIDToGAD(cRiderAnim* riderAnim, int aid) {
    int var_6;
    void *temp_2;
    void *temp_4;

    temp_2 = ((void*)riderAnim->FERider);
    var_6 = 0;
    if (temp_2 != 0) {
        var_6 = (*(int *)((char*)(temp_2) + (0xCD8)));
    } else {
        temp_4 = ((void*)riderAnim->Rider);
        if (temp_4 != 0) {
            var_6 = (*(int *)((char*)(temp_4) + (0x364)));
        }
    }
    cAIAnimEventMap_mapAIDToGAD_4CF8(cRiderAnimBase_AIAnimEventMap, aid, var_6);
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
 * @param riderAnim  the rider animation object (this)
 * @param rider  the rider (cRider) to drive
 */
extern "C" void cRiderAnim_linkToRider(void *riderAnim, cRider* rider) {
    (*(int *)((char*)(riderAnim) + (0x64))) = 0;
    (*(void **)((char*)(riderAnim) + (0x60))) = ((void*)rider);
    (*(int *)((char*)(riderAnim) + (0x54))) = (int) ((int)rider->mAnimModel);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", func_00104D50);
#ifdef SKIP_ASM
/**
 * Links the rider animation object to a front-end rider instead of a game rider.
 *
 * Stores @p feRider at +0x64, clears the rider pointer +0x60 and copies the word at +8 of @p feRider into +0x54.
 * cFERider_compileModel calls it; cRiderAnim_onExpireBLENDTO_AIR tests +0x64.
 *
 * @param riderAnim  the rider animation object
 * @param feRider  the front-end rider
 */
extern "C" void func_00104D50(void *riderAnim, void *feRider) {
    (*(void **)((char*)(riderAnim) + (0x64))) = feRider;
    (*(int *)((char*)(riderAnim) + (0x60))) = 0;
    (*(int *)((char*)(riderAnim) + (0x54))) = (int) (*(int *)((char*)(feRider) + (8)));
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_writeToReplayFrame);
#ifdef SKIP_ASM
class cStream_4D68 {
public:
    virtual void write(void* src, int n);  // slot 1 (slot 2 is read)
};
extern "C" void cAnimSequencer_writeToReplayFrame(void*, cStream_4D68*);
/**
 * Writes the rider animation state into a replay frame.
 *
 * Writes the object's first 80 bytes through the stream's virtual write, then each of the 6 sequencers (8 bytes each,
 * array at +0x50) with cAnimSequencer_writeToReplayFrame. cRiderHandler_writeToReplayFrame calls it.
 *
 * @param stream  the replay frame stream
 */
extern "C" void cRiderAnim_writeToReplayFrame(cRiderAnim* self, cStream_4D68* stream) {
    stream->write(((char*)self), 80);
    for (int i = 0; i < 6; i++) {
        cAnimSequencer_writeToReplayFrame((char*)self->Sequencer + i * 8, stream);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRiderAnim_readFromReplayFrame);
#ifdef SKIP_ASM
class cStream_4DE0 {
public:
    virtual void v0();
    virtual void Read(void* dst, int n);
};
extern "C" void cAnimSequencer_expireAll(void*);
extern "C" void cAnimSequencer_readFromReplayFrame(void*, cStream_4DE0*);
/**
 * Restores the rider animation state from a replay frame.
 *
 * Reads the object's first 80 bytes through the stream's virtual Read, then for each of the 6 sequencers (array at
 * +0x50) drops what it plays (cAnimSequencer_expireAll) and reads it (cAnimSequencer_readFromReplayFrame).
 * cRiderHandler_readFromReplayFrame calls it.
 *
 * @param stream  the replay frame stream
 */
extern "C" void cRiderAnim_readFromReplayFrame(cRiderAnim* self, cStream_4DE0* stream) {
    stream->Read(((char*)self), 80);
    for (int i = 0; i < 6; i++) {
        cAnimSequencer_expireAll((char*)self->Sequencer + i * 8);
        cAnimSequencer_readFromReplayFrame((char*)self->Sequencer + i * 8, stream);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_objectIntersect);
#ifdef SKIP_ASM
struct sV4_4E70 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sTri_4E70 {
    float a;
    float b;
    unsigned short flags;
    unsigned short pad;
};

struct sMat_4E70 {
    char pad[0x10];
    sTri_4E70 tris[1];
};

class cSurfObj_4E70;

struct sSurf_4E70 {
    char pad[0xC];
    cSurfObj_4E70* obj;
    char pad10[0x78 - 0x10];
    unsigned int id;
    char pad7C[0x88 - 0x7C];
    sMat_4E70* mat;
};

struct sOwner_4E70 {
    char pad[0x150];
    unsigned int order;
};

struct sHit_4E70 {
    sV4_4E70 pos;
    sV4_4E70 normal;
    char pad20[0x20];
    float dist;
    char pad44[0xC];
    sSurf_4E70* surf;
    sOwner_4E70* owner;
    int pad58;
    int idx;
    char pad60[0x20];
    sHit_4E70() {}
} __attribute__((aligned(16)));

class cSurfObj_4E70 {
public:
    int f0, f4, f8;
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
    virtual void OnHit(sHit_4E70* h);
};

class cScope_4E70 {
public:
    char pad[0x50];
    virtual void v0();
    virtual void v1();
    virtual float Height();
};

extern sV4_4E70 D_004FF120_4E70 __asm__("t4vect_Zero");
extern "C" int cOverlapList_staticObjectIntersect_4E70(void*, cScope_4E70*, sHit_4E70*, int, int) __asm__("cOverlapList_staticObjectIntersect");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_4E70(const sV4_4E70& a, const sV4_4E70& b)
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
static inline float Len_4E70(const sV4_4E70& v)
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
static inline sV4_4E70 Sub_4E70(const sV4_4E70& a, const sV4_4E70& b)
{
    sV4_4E70 r;
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
static inline void AddEq_4E70(sV4_4E70& a, const sV4_4E70& b)
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
static inline sV4_4E70 Div_4E70(const sV4_4E70& v, float s)
{
    sV4_4E70 r;
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

static inline int IsWall_4E70(const sHit_4E70& h)
{
    sTri_4E70* t = &h.surf->mat->tris[h.idx];
    int r = 0;
    if ((t->flags & 1) && t->a != 0.0f)
        r = 1;
    return r;
}

static inline int Facing_4E70(char* self, const sHit_4E70& h)
{
    int r = 1;
    if (!(Dot_4E70(Sub_4E70(h.pos, *(sV4_4E70*)(*(char**)(self + 0xAA0) + 0x10)), h.normal) < 0.0f))
        r = 0;
    return r;
}

#define IDS_4E70 ((unsigned int*)(self + 0x5B8))

/**
 * Tests the rider against static objects and returns the distance of the contact it picks.
 *
 * Gets up to 64 hits from cOverlapList_staticObjectIntersect (overlap list +0x860); returns -1.0 when there are none.
 * The distinct surface ids of the hits are kept at +0x5B8, ended by 0xFFFFFFFF (cRider_isIntersecting reads them). The pick
 * prefers wall hits (triangle flag 1 with a nonzero first value), then hits whose normal points back towards the
 * rider's sphere-tree centre (+0xAA0, +0x10), then the smallest |distance - height| (the query's virtual Height); ties
 * go to the lower surface id, or the lower owner order (+0x150) when neither has a surface. The pick is copied to
 * @p out; with two or more hits its normal becomes the normalized sum of the wall hits' normals (when longer than
 * 0.001). Then the surface's object gets the hit through its virtual OnHit.
 *
 * @param sc  the intersection query
 * @param out  receives the chosen hit
 * @return the chosen hit's distance, or -1.0 when nothing was hit
 */
extern "C" float cRider_objectIntersect(char* self, cScope_4E70* sc, sHit_4E70* out)
{
    int nIds = 0;
    sHit_4E70 hits[64];
    int n = cOverlapList_staticObjectIntersect_4E70(*(void**)(self + 0x860), sc, hits, 64, 0);
    if (n == 0)
        return -1.0f;
    if (hits[0].surf)
        IDS_4E70[nIds++] = hits[0].surf->id;
    int best = 0;
    float hgt = sc->Height();
    float bestD = Abs_4E70(hits[0].dist - hgt);
    int bestWall = IsWall_4E70(hits[0]);
    int bestFacing = Facing_4E70(self, hits[0]);
    for (int i = 1; i < n; i++) {
        if (hits[i].surf) {
            int j;
            for (j = 0; j < nIds; j++) {
                if (IDS_4E70[j] == hits[i].surf->id)
                    break;
            }
            if (j >= nIds)
                IDS_4E70[nIds++] = hits[i].surf->id;
        }
        float dd = Abs_4E70(hits[i].dist - hgt);
        int wall = IsWall_4E70(hits[i]);
        if (wall && !bestWall) {
            best = i;
            bestD = dd;
            bestWall = 1;
            continue;
        }
        else if (bestWall && !wall)
            continue;
        int facing = Facing_4E70(self, hits[i]);
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
            // PORT: index-first address arithmetic through int
            sHit_4E70* tieHit = (sHit_4E70*)((i << 7) + (int)hits);
            sHit_4E70* bestHit = (sHit_4E70*)((best << 7) + (int)hits);
            int take;
            if (tieHit->surf) {
                if (!bestHit->surf)
                    take = 1;
                else
                    take = tieHit->surf->id < bestHit->surf->id;
            } else {
                if (bestHit->surf)
                    take = 0;
                else
                    take = tieHit->owner->order < bestHit->owner->order;
            }
            if (take)
                best = i;
        }
    }
    *out = hits[best];
    if (n >= 2) {
        sV4_4E70 sum = D_004FF120_4E70;
        for (int k = 0; k < n; k++) {
            if (IsWall_4E70(hits[k]))
                AddEq_4E70(sum, hits[k].normal);
        }
        float len = Len_4E70(sum);
        if (len > 0.0010000000474974513f)
            out->normal = Div_4E70(sum, len);
    }
    IDS_4E70[nIds] = 0xFFFFFFFF;
    if (out->surf && out->surf->obj)
        out->surf->obj->OnHit(out);
    return out->dist;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cRider_checkObjectCollisions);
#ifdef SKIP_ASM
// FAKEMATCH: the scope object's address goes through a copy `ps` to one call while the destructor gets `&sc`
// (an inconsistently used address copy); plain `&sc` scores 99.98%. Landed 2026-10-18 from a saved result before
// this was noticed; marked under the 2026-11-14 ruling (portable, behaviour-preserving steering only).
struct sV4_5398 {
    float x, y, z, w;
} __attribute__((aligned(16)));

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

struct sSurf_5398 {
    char pad[8];
    int flags;
    char padC[0x88 - 0xC];
    sMat_5398* mat;
};

struct sContact_5398 {
    sV4_5398 pos;
    sV4_5398 normal;
    sV4_5398 vel;
    char pad30[0x20];
    sSurf_5398* surf;
    int pad54;
    int pad58;
    int idx;
    char pad60[0x20];
} __attribute__((aligned(16)));

struct sInfo_5398 {
    sV4_5398 pos;
    sV4_5398 dir;
    sV4_5398 normal;
    float speed;
} __attribute__((aligned(16)));

struct cScope_5398 {
    char data[0x320];
    cScope_5398(void* tree, int flag) __asm__("cWorldRider_cWorldRider");
};

int cRider_getControlState_5398(void*) __asm__("cRider_getControlState__FPv");
int cRider_getMotionState(void*);
extern "C" void cRider_checkRailBodyCollisions(void*);
extern "C" float cRider_objectIntersect_5398(char*, cScope_5398*, sContact_5398*) __asm__("cRider_objectIntersect");
extern "C" int cRider_isGroundNormalValid(void*);
extern "C" void cRider_doObjectCollision_5398(char*, sSurf_5398*, sInfo_5398*, sContact_5398*, float) __asm__("cRider_doObjectCollision");
extern "C" void cSSXAudio_CollisionSound_5398(void*, char*, sSurf_5398*, int) __asm__("cSSXAudio_CollisionSound");
extern "C" void cCameraTriggerMan_checkCollisions(void*, void*);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);
extern char* D_004A3500_5398 __asm__("gSSXAudio");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_5398(const sV4_5398& a, const sV4_5398& b)
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
static inline float Len_5398(const sV4_5398& v)
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
static inline sV4_5398 Sub_5398(const sV4_5398& a, const sV4_5398& b)
{
    sV4_5398 r;
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
static inline sV4_5398 Scale_5398(const sV4_5398& v, float s)
{
    sV4_5398 r;
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
static inline sV4_5398 Div_5398(const sV4_5398& v, float s)
{
    sV4_5398 r;
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
static inline sV4_5398 Norm_5398(const sV4_5398& v)
{
    sV4_5398 r;
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

static inline unsigned short TriFlags_5398(sSurf_5398* s, int i)
{
    return ((sTri_5398*)(i * sizeof(sTri_5398) + (int)s->mat->tris))->flags; // PORT: pointer in int
}

/**
 * Tests the rider against world objects and resolves the deepest contact.
 *
 * Skipped in control state 9. Runs cRider_checkRailBodyCollisions (rail body collisions) first, then queries a cWorldRider
 * (cWorldRider_cWorldRider) built on mSphereTree (+0xAA0) for the deepest hit (cRider_objectIntersect, On Tour objectIntersect). With a
 * hit, the contact normal is flattened against mGroundNormal (when cRider_isGroundNormalValid) or @p norm, or in motion
 * state 4 snapped to the handler's up vector (+0xB0); the impact speed is the closing speed along it. A triangle with
 * flag 2 on an object without flag 0x2000 caches the contact at +0x9E0 and +0xA60; flag 1 runs cRider_doObjectCollision
 * (doObjectCollision); objects without 0x2000 play cSSXAudio_CollisionSound. Always ends with the camera trigger check
 * (cCameraTriggerMan_checkCollisions on gCamTriggerMan) and cWorldRider__cWorldRider.
 *
 * @param norm  a ground normal to project the contact normal onto, or 0 (cAirMotion_resolve passes one)
 */
extern "C" void cRider_checkObjectCollisions(cRider* self, sV4_5398* norm)
{
    sContact_5398 contact;
    sInfo_5398 info;
    if (cRider_getControlState_5398((char*)self) == 9)
        return;
    float zero = 0.0f;
    cRider_checkRailBodyCollisions((char*)self);
    cScope_5398 sc(self->mSphereTree, 1);
    cScope_5398* ps = &sc;
    float overlap = cRider_objectIntersect_5398(((char*)self), ps, &contact);
    if (overlap >= zero) {
        if (cRider_isGroundNormalValid((char*)self))
            norm = (sV4_5398*)(&self->mGroundNormal);
        if (norm) {
            sV4_5398 flatNormal = Sub_5398(contact.normal, Scale_5398(*norm, Dot_5398(contact.normal, *norm)));
            float len = Len_5398(flatNormal);
            if (len < 0.0010000000474974513f)
                contact.normal = *norm;
            else
                contact.normal = Div_5398(flatNormal, len);
        } else if (cRider_getMotionState((char*)self) == 4) {
            sV4_5398* up = (sV4_5398*)((char*)self->mRiderHandler + 0xB0);
            if (Dot_5398(contact.normal, *up) > zero)
                contact.normal = *up;
            else
                contact.normal = Scale_5398(*up, -1.0f);
        }
        float k = Dot_5398(Scale_5398(contact.normal, -1.0f), Sub_5398(*(sV4_5398*)&self->mVelocity, contact.vel)) >= 0.0f
                      ? Dot_5398(Scale_5398(contact.normal, -1.0f), Sub_5398(*(sV4_5398*)&self->mVelocity, contact.vel))
                      : 0.0f;
        info.pos = contact.pos;
        info.dir = Norm_5398(*(sV4_5398*)&self->mVelocity);
        info.normal = contact.normal;
        info.speed = k;
        if ((TriFlags_5398(contact.surf, contact.idx) & 2) && !(contact.surf->flags & 0x2000)) {
            *(sContact_5398*)(((char*)self) + 0x9E0) = contact;
            *(sInfo_5398*)(((char*)self) + 0xA60) = info;
        }
        if (TriFlags_5398(contact.surf, contact.idx) & 1)
            cRider_doObjectCollision_5398(((char*)self), contact.surf, &info, &contact, overlap);
        if (!(contact.surf->flags & 0x2000))
            cSSXAudio_CollisionSound_5398(D_004A3500_5398, ((char*)self), contact.surf, contact.idx);
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
extern "C" void cRider_doBump(cRider* self, char* info, int flag);
extern "C" void cRider_reactToCollision(void* rider, void* info, int id);
extern "C" void cRider_moveFromCollision(void* rider, const t4Vector& v);
extern "C" void cRider_turnFromCollision(void* rider, void* hit);
extern "C" void cRider_flagEventLand(void* rider, int a, int b, int c, void* hit, float speed);
extern "C" void cRiderMetrix_evTakeoff(void* metrix, int a, int b);

struct sHit_57B8 {
    char pad0[0x10];
    t4Vector n;         // 0x10
    t4Vector vel;       // 0x20
    char pad30[0x1C];
    int mat;            // 0x4C
    char pad50[0xC];
    int surf;           // 0x5C
};

struct sVE_57B8 { short delta; short index; int (*fn)(void*); };
struct sVEf_57B8 { short delta; short index; void (*fn)(void*, float); };
struct sVEv_57B8 { short delta; short index; t4Vector* (*fn)(void*); };

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
 * Skipped in control state 9. The hit surface's bounce and grip come from the object's surface table (+0x88, 12 bytes
 * per surface); with no bounce it only bumps (cRider_doBump). A solid object (no handler at +0xC, or one whose virtual
 * slot 14 is nonzero) moves the rider out by 1.1 x @p depth (cRider_moveFromCollision), rebounds by the closing speed plus
 * clamp(grip x speed, 55.6, 1388.9) and, on the ground, turns the rider along it (cRider_turnFromCollision,
 * cRider_updateOrientationImplicit); other objects get a softer rebound. Airborne below 833.3 the hit adds to
 * mImpactAccum2.
 *
 * In the air with channel 2 in animation category 1, 2, 9 or 0xB and unk_180 facing the surface (> 0.3), the hit
 * becomes a takeoff: mVelocity is rebuilt along the surface, then cRider_flagEventLand, cRiderMetrix_evTakeoff
 * (evTakeoff), control states 0xD and 5, animation 0x10C and cAirPredictor_startLaunchIntoAir. Otherwise the rebound is
 * added to mVelocity and cRider_reactToCollision picks the reaction.
 *
 * @param rider  the rider (this)
 * @param obj  the hit object's instance record (tInstanceRecord)
 * @param info  the collide info (tCollideInfo) passed on to the bump or reaction
 * @param hit  the intersection (tIntersectInfo): normal, surface velocity, surface and material
 * @param depth  the penetration depth (On Tour overlap)
 */
extern "C" void cRider_doObjectCollision(cRider* rider, char* obj, char* info, sHit_57B8* hit, float depth)
{
    if (cRider_getControlState_57B8(rider) == 9)
        return;
    int off = hit->surf * 12 + 0x10;
    float* surf = (float*)(*(char**)(obj + 0x88) + off);
    float grip = Clamp_57B8(surf[1], 0.0f, 1.0f);
    float bounce = surf[0];
    if (bounce == 0.0f) {
        cRider_doBump(rider, info, 1);
        return;
    }
    float vn = Dot_57B8(hit->n, Sub_57B8(hit->vel, rider->mVelocity)) >= 0.0f
                   ? Dot_57B8(hit->n, Sub_57B8(hit->vel, rider->mVelocity)) : 0.0f;
    char* handler = *(char**)(obj + 0xC);
    if (handler == 0 || (*(sVE_57B8**)(handler + 0xC))[14].fn(handler + (*(sVE_57B8**)(handler + 0xC))[14].delta) != 0) {
        cRider_moveFromCollision(rider, Scale_57B8(hit->n, depth * 1.100000023841858f));
        if (vn < 0.0f)
            vn = 0.0f;
        bounce = vn + Clamp_57B8(grip * vn, 55.55555725097656f, 1388.888916015625f);
        if (cRider_getMotionState(rider) == 0)
            cRider_turnFromCollision(rider, hit);
        cRider_updateOrientationImplicit(rider);
    } else {
        float inv = 1.0f / (100.0f / bounce + 1.0f);
        float linearTerm = vn * inv;
        float constTerm = (1.0f - grip) * inv;
        bounce = Sqrt_57B8((linearTerm * linearTerm - constTerm) >? 0.0f) - linearTerm; // PORT: g++ maximum operator
    }
    float facing = DotV_57B8(rider->unk_180, hit->n);
    if (cRider_getMotionState(rider) == 1 || (cRider_getMotionState(rider) == 2 && *(int*)((char*)rider->mRiderHandler + 0x30) == 1)) {
        if (Len_57B8(rider->mVelocity) < 833.3333740234375f) {
            float* acc = &rider->mImpactAccum2;
            *acc = *acc + (2.5f - facing) * (hit->n.z >= 0.20000000298023224f ? hit->n.z : 0.20000000298023224f);
        }
    }
    int anim = cRiderAnimBase_getCurrentAnimCategory(rider->mRiderAnim, 2);
    int state = cRider_getMotionState(rider);
    if (state == 1 && (anim == 9 || anim == 2 || anim == 11 || anim == state) && facing > 0.30000001192092896f) {
        t4Vector w = Add_57B8(Sub_57B8(rider->mVelocity, hit->vel), Scale_57B8(hit->n, vn));
        float speed = Len_57B8(w);
        float out = Max_57B8(bounce - vn, 277.77777099609375f);
        float velScale = (speed + 555.5555419921875f) * 0.5f;
        if (speed <= 27.77777862548828f)
            velScale = velScale / 27.77777862548828f;
        else
            velScale = velScale / speed;
        ScaleEq_57B8(w, velScale);
        rider->mVelocity = Add_57B8(w, Scale_57B8(hit->n, out));
        sVEf_57B8* vt = *(sVEf_57B8**)&rider->mVtbl;
        vt[kRiderVt_vibrationBump].fn((char*)rider + vt[kRiderVt_vibrationBump].delta, vn);
        if (cRiderAnimBase_getCurrentAnimID(rider->mRiderAnim, 2) != 268 || cRiderAnimBase_getCurrentAnimFramePercent(rider->mRiderAnim, 2) > 0.20000000298023224f) {
            rider->mSurfaceType = 0;
            cRider_flagEventLand(rider, 0, 0, 0, hit, speed);
            cRiderMetrix_evTakeoff(rider->mMetrix, rider->IsRidingFakie != rider->IsGoofyStance, 0);
            cRider_setControlState_57B8(rider, 13);
            cRider_setControlState_57B8(rider, 5);
            cRiderAnimBase_play(rider->mRiderAnim, 268, 1, -1.0f);
        }
        char* sub = (char*)&rider->mVtbl;
        sVEv_57B8* vt2 = *(sVEv_57B8**)sub;
        t4Vector* pos = vt2[kRiderVt_getPosition].fn(sub + vt2[kRiderVt_getPosition].delta);
        sVEv_57B8* vt3 = *(sVEv_57B8**)sub;
        t4Vector* vel = vt3[kRiderVt_getVelocity].fn(sub + vt3[kRiderVt_getVelocity].delta);
        cAirPredictor_startLaunchIntoAir_57B8(rider->mAirPredictor, pos, vel, rider->unk_2E4);
        return;
    }
    AddEq_57B8(rider->mVelocity, Scale_57B8(hit->n, bounce));
    cRider_reactToCollision(rider, info, hit->mat);
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
extern "C" void cRider_doBump(cRider* self, char* info, int flag);
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
extern sGame_5D98* D_004A28A8_5D98 __asm__("SSXApp");
extern float D_004A4C84;

struct sInfo_5D98 {
    t4Vector pos;       // 0x00
    t4Vector dir;       // 0x10
    t4Vector off;       // 0x20
    float vn;           // 0x30
} __attribute__((aligned(16)));

struct sVEf_5D98 { short delta; short index; void (*fn)(void*, float); };
struct sVEv_5D98 { short delta; short index; t4Vector* (*fn)(void*); };

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
// against the rider's axes (rows at 0x160/0x170/0x180, origin at 0x190).
// The unit declares this earlier as (void*, void*, int); bind the typed body by asm label.
extern "C" void cRider_reactToCollision_impl(cRider* self, sInfo_5D98* info, int id) __asm__("cRider_reactToCollision");

/**
 * Chooses the rider's reaction to a collision: a reset on a fatal surface, a wipeout animation, or a bump.
 *
 * Skipped in control state 9. Adds 1 - max(0, contact . mImpulse) to mImpactAccum and stores the contact direction in
 * mImpulse. If the surface material's record (SSXApp's material list at +0x84, flag +0x44) is fatal it resets the rider
 * (cRider_checkForReset(self, 0, 1)). Otherwise it sends the vibrationBump virtual; in motion state 2 it may relaunch
 * the air predictor, flags the handler (+0x314 = 1, +0x320 = mVelocity) and stops; in the air it relaunches the air
 * predictor.
 *
 * It then compares the contact direction with the rider's axes (+0x160, +0x170, +0x180) and, from the dominant axis,
 * the normal speed and channel 2's animation category, picks a wipeout animation (0x148 .. 0x169, random variants by
 * AIrand) for cRider_flagEventWipeout; when none applies it bumps (cRider_doBump).
 *
 * @param info  the collide info: contact position, direction, normal (+0x20) and normal speed (+0x30)
 * @param id  the surface material id, or -1
 */
extern "C" void cRider_reactToCollision_impl(cRider* self, sInfo_5D98* info, int id)
{
    if (cRider_getControlState_5D98(self) == 9)
        return;
    self->mImpactAccum += 1.0f - (Dot_5D98(info->off, self->mImpulse) >? 0.0f); // PORT: g++ maximum operator
    self->mImpulse = info->off;
    if (id != -1) {
        if (D_004A28A8_5D98->matList->getMat(id)->fatal != 0) {
            cRider_checkForReset(self, 0, 1);
            return;
        }
    }
    sVEf_5D98* vt = *(sVEf_5D98**)&self->mVtbl;
    vt[kRiderVt_vibrationBump].fn((char*)self + vt[kRiderVt_vibrationBump].delta, info->vn);
    char* sub = (char*)&self->mVtbl;
    if (cRider_getMotionState(self) == 2) {
        if (*(int*)((char*)self->mRiderHandler + 0x30) == 1) {
            sVEv_5D98* vt2 = *(sVEv_5D98**)sub;
            t4Vector* pos = vt2[kRiderVt_getPosition].fn(sub + vt2[kRiderVt_getPosition].delta);
            sVEv_5D98* vt3 = *(sVEv_5D98**)sub;
            t4Vector* vel = vt3[kRiderVt_getVelocity].fn(sub + vt3[kRiderVt_getVelocity].delta);
            cAirPredictor_startLaunchIntoAir_5D98(self->mAirPredictor, pos, vel, self->unk_2E4);
        }
        char* h = (char*)self->mRiderHandler;
        *(int*)(h + 0x314) = 1;
        *(t4Vector*)(h + 0x320) = self->mVelocity;
        return;
    }
    if (cRider_getMotionState(self) == 1) {
        sVEv_5D98* vt2 = *(sVEv_5D98**)sub;
        t4Vector* pos = vt2[kRiderVt_getPosition].fn(sub + vt2[kRiderVt_getPosition].delta);
        sVEv_5D98* vt3 = *(sVEv_5D98**)sub;
        t4Vector* vel = vt3[kRiderVt_getVelocity].fn(sub + vt3[kRiderVt_getVelocity].delta);
        cAirPredictor_startLaunchIntoAir_5D98(self->mAirPredictor, pos, vel, self->unk_2E4);
    }
    float sideDot = Dot_5D98(Get_5D98((char*)self + 0x160), info->off);
    float frontDot = Dot_5D98(Get_5D98((char*)&self->unk_170), info->off);
    float upDot = Dot_5D98(Get_5D98((char*)&self->unk_180), info->off);
    t4Vector origin = Get_5D98((char*)self + 0x190);
    t4Vector rel = Sub_5D98(info->pos, origin);
    float h = Dot_5D98(rel, Get_5D98((char*)&self->unk_180));
    int cur = cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 2);
    int anim = 438;
    if (cRider_getControlState_5D98(self) != 11) {
        if (cRider_getMotionState(self) == 1 && info->off.z > 0.800000011920929f) {
            if (cur == 20 || cur == 21) {
                anim = 361;
            } else if (Abs_5D98(frontDot) > Abs_5D98(sideDot) && Abs_5D98(frontDot) > Abs_5D98(upDot)) {
                anim = frontDot < 0.0f ? 354 : 355;
            } else if (Abs_5D98(upDot) > Abs_5D98(sideDot) && Abs_5D98(upDot) > Abs_5D98(frontDot)) {
                if (upDot < 0.0f)
                    anim = 350;
                else if (cur == 18 || cur == 19)
                    anim = 351;
            } else {
                anim = sideDot < 0.0f ? 357 : 356;
            }
        } else if (cRider_getMotionState(self) == 1) {
            if (Abs_5D98(frontDot) > Abs_5D98(sideDot) && Abs_5D98(frontDot) > Abs_5D98(upDot)) {
                float minSpeed = 416.5320129394531f;
                if (minSpeed < info->vn) {
                    if (frontDot < 0.0f) {
                        float len = Len_5D98(self->mVelocity);
                        if (h < 60.0f && len > D_004A4C84) {
                            anim = 342;
                        } else {
                            unsigned int k;
                            if (len > D_004A4C84)
                                k = (unsigned int)AIrand() % 6;
                            else
                                k = (unsigned int)AIrand() % 3;
                            if (k == 0)
                                anim = 347;
                            else if (k == 1)
                                anim = 343;
                            else if (k == 2)
                                anim = 344;
                            else if (k == 3)
                                anim = 346;
                            else
                                anim = 345;
                        }
                    } else {
                        anim = (AIrand() & 1) == 0 ? 340 : 341;
                    }
                }
            } else if (Abs_5D98(upDot) > Abs_5D98(sideDot) && Abs_5D98(upDot) > Abs_5D98(frontDot)) {
                if (upDot < 0.0f)
                    anim = 350;
                else if (cur >= 18 && cur <= 21)
                    anim = 351;
            } else {
                float minSpeed = 833.337646484375f;
                if (minSpeed < info->vn)
                    anim = sideDot < 0.0f ? 339 : 338;
            }
            if (anim != 438 && (cur == 20 || cur == 21))
                anim = 361;
        } else {
            float k = Clamp_5D98(Get_5D98((char*)&self->mMatrix.row[2]).z + 1.0f - Abs_5D98(info->off.z),
                                 0.20000000298023224f, 1.399999976158142f);
            if (Abs_5D98(frontDot) > Abs_5D98(sideDot) && Abs_5D98(frontDot) > Abs_5D98(upDot)) {
                if (info->vn > k * 1388.880859375f) {
                    if (frontDot < 0.0f) {
                        float len = Len_5D98(self->mVelocity);
                        if (h < 60.0f && len > D_004A4C84) {
                            anim = 332;
                        } else {
                            unsigned int n;
                            if (len > D_004A4C84)
                                n = (unsigned int)AIrand() % 6;
                            else
                                n = (unsigned int)AIrand() % 3;
                            if (n == 0)
                                anim = 334;
                            else if (n == 1)
                                anim = 335;
                            else if (n == 2)
                                anim = 337;
                            else if (n == 3)
                                anim = 333;
                            else
                                anim = 336;
                        }
                    } else {
                        anim = (AIrand() & 1) == 0 ? 330 : 331;
                    }
                }
            } else if (Abs_5D98(upDot) > Abs_5D98(sideDot) && Abs_5D98(upDot) > Abs_5D98(frontDot)) {
            } else if (info->vn > k * 2083.227294921875f) {
                anim = sideDot < 0.0f ? 329 : 328;
            }
        }
    }
    if (anim != 438)
        cRider_flagEventWipeout(self, anim, 0, id, info);
    else
        cRider_doBump((cRider*)((char*)self), (char*)info, 1);
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
// PORT: the unit declares cRider_moveFromCollision(void*, const t4Vector&) for its callers; same symbol.
extern "C" void cRider_moveFromCollision_6538(char* self, const t4Vector* d) __asm__("cRider_moveFromCollision");

/**
 * Moves the rider by an offset out of a collision.
 *
 * Adds @p offset to the vectors at +0x110, +0x9D0, +0x400 and +0x410 and to the sphere tree (+0xAA0,
 * cRiderSphereTree_ApplyOffset).
 *
 * @param offset  the offset
 */
extern "C" void cRider_moveFromCollision_6538(char* self, const t4Vector* offset)
{
    AddEq_6538(*(t4Vector*)(self + 0x110), *offset);
    AddEq_6538(*(t4Vector*)(self + 0x9D0), *offset);
    cRiderSphereTree_ApplyOffset(*(void**)(self + 0xAA0), offset);
    AddEq_6538(*(t4Vector*)(self + 0x400), *offset);
    AddEq_6538(*(t4Vector*)(self + 0x410), *offset);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_turnFromCollision);
#ifdef SKIP_ASM
struct sV4_65B0
{
    float x, y, z, w;
    sV4_65B0() {}
    sV4_65B0(float ax, float ay, float az, float aw) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));

extern "C" void fSinCos(float* s, float* c, float angle);
extern "C" float func_0031C228(float);

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline sV4_65B0 Cross_65B0(const sV4_65B0& a, const sV4_65B0& b)
{
    sV4_65B0 r;
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
static inline sV4_65B0 Norm_65B0(const sV4_65B0& v)
{
    sV4_65B0 r;
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
static inline float Dot_65B0(const sV4_65B0& a, const sV4_65B0& b)
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
static inline sV4_65B0 QuatMul_65B0(const sV4_65B0& a, const sV4_65B0& b)
{
    sV4_65B0 r;
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

static inline sV4_65B0 AxisAngle_65B0(const sV4_65B0& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sV4_65B0(s * axis.x, s * axis.y, s * axis.z, c);
}

static inline sV4_65B0 Up_65B0(char* self) { return *(sV4_65B0*)(self + 0x1C0); }
static inline sV4_65B0 Fwd_65B0(char* self) { return *(sV4_65B0*)(self + 0x1B0); }

static inline void Rotate_65B0(char* self, const sV4_65B0& axis, float angle)
{
    *(sV4_65B0*)(self + 0x120) = QuatMul_65B0(*(sV4_65B0*)(self + 0x120), AxisAngle_65B0(axis, angle));
}

extern "C" void cRider_turnFromCollision_impl(char* self, char* arg) __asm__("cRider_turnFromCollision");
/**
 * Turns the rider away from a surface it hit.
 *
 * n is the normalized cross product of the up axis (+0x1C0) and the hit normal (@p hitInfo + 0x10), and the in-plane
 * direction is up x n. The angle is atan2 of the forward axis (+0x1B0) against n and the in-plane direction. A positive
 * angle below 110 degrees (1.9198622 rad) gives a turn of min(110 degrees - angle, 20 degrees), mirrored for negative
 * angles; the orientation quaternion (+0x120) is then rotated about the up axis by minus that turn.
 *
 * @param hitInfo  the hit (normal at +0x10)
 */
extern "C" void cRider_turnFromCollision_impl(char* self, char* hitInfo)
{
    sV4_65B0 n = Norm_65B0(Cross_65B0(Up_65B0(self), *(sV4_65B0*)(hitInfo + 0x10)));
    sV4_65B0 inPlaneDir = Cross_65B0(Up_65B0(self), n);
    float x = Dot_65B0(Fwd_65B0(self), inPlaneDir);
    float y = Dot_65B0(Fwd_65B0(self), n);
    float angle = Atan2_65B0(y, x);
    float lim = 1.9198623895645142f;
    float step = 0.349065899848938f;
    float turnAngle;
    if (angle > 0.0f)
        turnAngle = Clamp_65B0(lim - angle, 0.0f, step);
    else
        turnAngle = Clamp_65B0(-lim - angle, -step, 0.0f);
    if (turnAngle != 0.0f) {
        Rotate_65B0(self, Up_65B0(self), -turnAngle);
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_updateCollideInfo);
#ifdef SKIP_ASM
extern "C" void cRiderSphereTree_Update1(int);

/**
 * Refreshes the rider's collision sphere tree.
 *
 * Calls cRiderSphereTree_Update1 on mSphereTree (+0xAA0), the sphere tree the object collision queries use (On Tour:
 * cRiderSphereTree::Update). Callers: cRider_updateSkeleton, cRiderHandler_readFromReplayFrame and
 * cRider_DoInSeqAnimUpdate.
 *
 * @param rider  the rider (this)
 */
extern "C" void cRider_updateCollideInfo(cRider* rider) {
    cRiderSphereTree_Update1(((int)rider->mSphereTree));
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
extern char* D_004A3500_6848 __asm__("gSSXAudio");
extern t4Vector D_004FF130_6848 __asm__("t4vect_Origin");

struct sLand_6848 {
    char pad0[0x10];
    t4Vector n;         // 0x10
    char pad20[0x2C];
    int mat;            // 0x4C
    char* obj;          // 0x50
    char pad54[0x2C];
} __attribute__((aligned(16)));

// The unit defines this later with its own vector type; bind it by asm label.
extern "C" int cRider_isRailLandSuccess_6848(cRider* self, sLand_6848* land) __asm__("cRider_isRailLandSuccess");

struct sV_6848 {
    float x, y, z, w;
    sV_6848() {}
    sV_6848(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern sV_6848 D_004FF160_6848 __asm__("t4vect_ZAxis");

struct sQuat_6848 {
    float x, y, z, w;
    sQuat_6848() {}
    sQuat_6848(float ax, float ay, float az, float aw) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));

struct sVEH_6848 { short delta; short index; void (*fn)(void*, void*, const t4Vector&); };

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

static inline sV_6848 Side_6848(const sAnimModelBone* animModelBone)
{
    return sV_6848(1.0f - 2.0f * (animModelBone->rot.y * animModelBone->rot.y + animModelBone->rot.z * animModelBone->rot.z),
                   2.0f * (animModelBone->rot.x * animModelBone->rot.y + animModelBone->rot.w * animModelBone->rot.z),
                   2.0f * (animModelBone->rot.x * animModelBone->rot.z - animModelBone->rot.w * animModelBone->rot.y),
                   0.0f);
}

static inline sV_6848 Front_6848(const sAnimModelBone* animModelBone)
{
    return sV_6848(2.0f * (animModelBone->rot.y * animModelBone->rot.x - animModelBone->rot.w * animModelBone->rot.z),
                   1.0f - 2.0f * (animModelBone->rot.z * animModelBone->rot.z + animModelBone->rot.x * animModelBone->rot.x),
                   2.0f * (animModelBone->rot.y * animModelBone->rot.z + animModelBone->rot.w * animModelBone->rot.x),
                   0.0f);
}

static inline sV_6848 Up_6848(const sAnimModelBone* animModelBone)
{
    return sV_6848(2.0f * (animModelBone->rot.z * animModelBone->rot.x + animModelBone->rot.w * animModelBone->rot.y),
                   2.0f * (animModelBone->rot.z * animModelBone->rot.y - animModelBone->rot.w * animModelBone->rot.x),
                   1.0f - 2.0f * (animModelBone->rot.x * animModelBone->rot.x + animModelBone->rot.y * animModelBone->rot.y),
                   0.0f);
}

static inline sQuat_6848 AxisAngle_6848(const sV_6848& axis, float angle)
{
    float s, c;
    fSinCos(&s, &c, angle * 0.5f);
    return sQuat_6848(s * axis.x, s * axis.y, s * axis.z, c);
}

#define RAIL_STYLE(r) ((r)->RailslideMode)
#define PRESS_TYPE(r) ((r)->PressMode)

/**
 * Checks for the board touching a rail and, on contact, puts the rider into the railslide motion.
 *
 * cRider_isRailLandSuccess finds the contact. The velocity is turned along its direction at the along-rail speed, at
 * least 20 km/h and at most the old speed, with z cut to a tenth; the hit object's handler gets the opposite
 * change (virtual slot 43). RailslideMode (1-4) comes from the control state and the board root bone's angle to
 * the rail. The trick is then awarded (cRider_flagEventLand from the air, else the score commit fed to
 * cRider_flagEventBoostPowerUp()) and motion 4 set. From cruise, natural air, spin or handplant the rider
 * starts the board-press (press landing) or railslide control; otherwise a style change away from 3 or 4
 * turns the rider back by 90 degrees.
 *
 * @return 1 when the rider touched a rail, otherwise 0
 */
extern "C" int cRider_checkRailBoardCollisions(cRider* self)
{
    sLand_6848 land;
    if (!cRider_isRailLandSuccess_6848(self, &land))
        return 0;
    float vn = Dot_6848(self->mVelocity, land.n);
    if (vn < 0.0f) {
        vn = -vn;
        ScaleEq_6848(land.n, -1.0f);
    }
    vn = AtLeast_6848(vn, 555.5555419921875f);
    float imp = AtMost_6848(Len_6848(self->mVelocity), vn);
    t4Vector dv = self->mVelocity;
    self->mVelocity = Scale_6848(land.n, imp);
    self->mVelocity.z = self->mVelocity.z * 0.10000000149011612f;
    dv = Sub_6848(self->mVelocity, dv);
    if (land.obj != 0) {
        char* landInst = *(char**)(land.obj + 0xC);
        if (landInst != 0) {
            sVEH_6848* vt = *(sVEH_6848**)(landInst + 0xC);
            vt[43].fn(landInst + vt[43].delta, &land, Scale_6848(dv, -1.0f));
        }
    }
    sAnimModelBone* bone = &self->mAnimModel->bones[self->mBoneBoardRoot];
    sV_6848 side = Side_6848(bone);
    sV_6848 up = Up_6848(bone);
    float sideDot = Dot_6848(*(t4Vector*)&side, land.n);
    float upDot = Dot_6848(*(t4Vector*)&up, land.n);
    float angle = Atan2_6848(upDot, sideDot);
    int prev = RAIL_STYLE(self);
    if (cRider_getControlState_57B8(self) == 1) {
        if (self->IsRidingFakie == 0)
            RAIL_STYLE(self) = 1;
        else
            RAIL_STYLE(self) = 2;
    } else if (PRESS_TYPE(self) != 0) {
        if (Abs_6848(angle) > 1.5707964897155762f)
            RAIL_STYLE(self) = 2;
        else
            RAIL_STYLE(self) = 1;
    } else if (cRider_getControlState_57B8(self) == 0xC) {
        RAIL_STYLE(self) = 3;
    } else {
        sV_6848 front = Front_6848(bone);
        if (front.z < 0.0f) {
            if (upDot < 0.0f)
                RAIL_STYLE(self) = 4;
            else
                RAIL_STYLE(self) = 3;
        } else if (Abs_6848(angle) > 2.6179940700531006f) {
            RAIL_STYLE(self) = 2;
        } else if (Abs_6848(angle) < 0.523598849773407f) {
            RAIL_STYLE(self) = 1;
        } else if (angle < -0.523598849773407f) {
            RAIL_STYLE(self) = 4;
        } else {
            RAIL_STYLE(self) = 3;
        }
    }
    self->mSurfaceType = land.mat;
    if (cRider_getMotionState(self) == 1) {
        cRider_flagEventLand(self, 0, RAIL_STYLE(self), PRESS_TYPE(self), &land, imp);
    } else {
        cRider_flagEventBoostPowerUp(self, 1, cRiderMetrix_evLand(self->mMetrix, self->IsRidingFakie != self->IsGoofyStance, 0, RAIL_STYLE(self), PRESS_TYPE(self)));
    }
    *(int*)(D_004A3500_6848 + 0x598C) = land.mat;
    cRider_setMotionState_6848(self, 4);
    int st = cRider_getControlState_57B8(self);
    if (st == 0 || cRider_getControlState_57B8(self) == 4 || cRider_getControlState_57B8(self) == 5 || cRider_getControlState_57B8(self) == 11) {
        int fall = 0;
        if (cRider_getControlState_57B8(self) == 5 || cRider_getControlState_57B8(self) == 4)
            fall = 1;
        cRider_setControlState_57B8(self, 13);
        cRider_doRailslideModeOrientation(self);
        int kind = PRESS_TYPE(self);
        if (kind != 0) {
            if (kind == 1)
                cRiderAnimBase_play(self->mRiderAnim, 26, 0, -1.0f);
            else
                cRiderAnimBase_play(self->mRiderAnim, 34, 0, -1.0f);
            cRider_setControlState_57B8(self, 1);
        } else {
            cRailslideControl_playGainFocusAnim((char*)self->mRiderHandler + 0x2B0, fall);
            cRider_setControlState_57B8(self, 7);
        }
    } else if (prev != RAIL_STYLE(self)) {
        if (prev == 3) {
            cRider_rotateOrientation(self, GetUp_6848(self), 1.5707963705062866f);
            cRiderAnimBase_changeHeadingOffset(self->mRiderAnim, 1.5707963705062866f);
            float yaw = 0.0f;
            cRiderAnim* riderAnim = self->mRiderAnim;
            riderAnim->OrientationOffset.P = D_004FF130_6848;
            *(sQuat_6848*)&riderAnim->OrientationOffset.Q = AxisAngle_6848(D_004FF160_6848, -yaw);
        } else if (prev == 4) {
            cRider_rotateOrientation(self, GetUp_6848(self), -1.5707963705062866f);
            cRiderAnimBase_changeHeadingOffset(self->mRiderAnim, -1.5707963705062866f);
            float yaw = 0.0f;
            cRiderAnim* riderAnim = self->mRiderAnim;
            riderAnim->OrientationOffset.P = D_004FF130_6848;
            *(sQuat_6848*)&riderAnim->OrientationOffset.Q = AxisAngle_6848(D_004FF160_6848, -yaw);
        }
        cRider_doRailslideModeOrientation(self);
    }
    cRailslideMotion_setCrookedAngle((char*)self->mRiderHandler + 0xB0, &land.n);
    return 1;
}

#undef RAIL_STYLE
#undef PRESS_TYPE
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_checkRailBodyCollisions);
#ifdef SKIP_ASM
extern "C" int cRider_isRailLandSuccess_6F78(cRider* self, void* land) __asm__("cRider_isRailLandSuccess");
extern "C" int cOverlapList_splineIntersect_6F78(void*, t4Vector*, t4Vector*, int, float) __asm__("cOverlapList_splineIntersect");

struct sHit_6F78 {
    t4Vector pos;       // 0x00
    t4Vector n;         // 0x10
    t4Vector vel;       // 0x20
    char pad30[0x1C];
    int mat;            // 0x4C
    char* obj;          // 0x50
    char pad54[0x2C];
} __attribute__((aligned(16)));

struct sLand_6F78 {
    char pad0[0x80];
} __attribute__((aligned(16)));

struct sInfo_6F78 {
    t4Vector pos;       // 0x00
    t4Vector dir;       // 0x10
    t4Vector off;       // 0x20
    float vn;           // 0x30
} __attribute__((aligned(16)));

struct sVEh_6F78 { short delta; short index; void (*fn)(void*, sHit_6F78*); };
struct sVEhv_6F78 { short delta; short index; void (*fn)(void*, sHit_6F78*, t4Vector*); };

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

struct sV4_6F78 : t4Vector {
    sV4_6F78(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

static inline sV4_6F78 Front_6F78(const sAnimModelBone* animModelBone)
{
    return sV4_6F78(2.0f * (animModelBone->rot.y * animModelBone->rot.x - animModelBone->rot.w * animModelBone->rot.z),
                    1.0f - 2.0f * (animModelBone->rot.z * animModelBone->rot.z + animModelBone->rot.x * animModelBone->rot.x),
                    2.0f * (animModelBone->rot.y * animModelBone->rot.z + animModelBone->rot.w * animModelBone->rot.x),
                    0.0f);
}

// Snaps the rider onto the rail found ahead of its current bone; returns 1 when it lands on it.
// PORT: the unit declares cRider_checkRailBodyCollisions as void(void*); it returns 0/1.
extern "C" int cRider_checkRailBodyCollisions_impl(cRider* self) __asm__("cRider_checkRailBodyCollisions");
/**
 * Pushes the rider's body off a rail it runs into without landing on it.
 *
 * Not in motion state 4. Looks for a rail spline within 300 of the hip bone (cOverlapList_splineIntersect on mOverlapList); rails
 * behind the motion, more than 50 off the hips' forward line, or that the board can land on (cRider_isRailLandSuccess,
 * isRailLandSuccess) are ignored. For a body hit it notifies the rail's handler (virtual 42), moves the rider out to 50
 * from the rail (cRider_moveFromCollision), pushes mVelocity away along the contact normal by the closing speed + 55.6, reports
 * the impulse (virtual 43), on the ground turns the rider (cRider_turnFromCollision, cRider_updateOrientationImplicit) and runs
 * cRider_reactToCollision.
 *
 * The // note above the definition says it snaps the rider onto the rail, but a landable rail returns 0 here; landing
 * is cRider_checkRailBoardCollisions's job.
 *
 * @return 1 when the body was pushed off a rail, else 0
 */
extern "C" int cRider_checkRailBodyCollisions_impl(cRider* self)
{
    if (cRider_getMotionState(self) == 4) {
        return 0;
    }
    sAnimModelBone* animModelBone = &self->mAnimModel->bones[self->mBoneHips];
    const t4Vector& dir = Front_6F78(animModelBone);
    sHit_6F78 hit;
    if (!cOverlapList_splineIntersect_6F78(self->mOverlapList, &animModelBone->pos, &hit.pos, 1, 300.0f)) {
        return 0;
    }
    float speed = Len_6F78(self->mVelocity);
    if (speed > 0.0010000000474974513f) {
        float back = Dot_6F78(Sub_6F78(hit.pos, animModelBone->pos), self->mVelocity);
        if (back < speed * -0.20000000298023224f) {
            return 0;
        }
    }
    float along = Clamp_6F78(Dot_6F78(dir, Sub_6F78(hit.pos, animModelBone->pos)), -50.0f, 50.0f);
    t4Vector delta = Sub_6F78(Add_6F78(animModelBone->pos, Scale_6F78(dir, along)), hit.pos);
    if (Len_6F78(delta) > 50.0f) {
        return 0;
    }
    sLand_6F78 land;
    if (cRider_isRailLandSuccess_6F78(self, &land)) {
        return 0;
    }
    float vn = Dot_6F78(self->mVelocity, hit.n);
    if (vn < 0.0f) {
        vn = -vn;
        ScaleEq_6F78(hit.n, -1.0f);
    }
    if (hit.obj != 0) {
        char* handler = *(char**)(hit.obj + 0xC);
        if (handler != 0) {
            sVEh_6F78* vt = *(sVEh_6F78**)(handler + 0xC);
            vt[42].fn(handler + vt[42].delta, &hit);
        }
    }
    sInfo_6F78 info;
    info.pos = hit.pos;
    info.dir = Norm_6F78(self->mVelocity);
    info.vn = vn;
    t4Vector off = Sub_6F78(animModelBone->pos, hit.pos);
    SubEq_6F78(off, Scale_6F78(hit.n, Dot_6F78(off, hit.n)));
    if (cRider_isGroundNormalValid(self)) {
        SubEq_6F78(off, Scale_6F78(self->mGroundNormal, Dot_6F78(off, self->mGroundNormal)));
    }
    float offLen = Len_6F78(off);
    if (offLen < 0.0010000000474974513f) {
        off = hit.n;
    } else {
        DivEq_6F78(off, offLen);
    }
    info.off = off;
    float clearance = Dot_6F78(delta, off);
    if (clearance < 50.0f) {
        cRider_moveFromCollision(self, Scale_6F78(off, 50.0f - clearance));
    }
    t4Vector oldVel = self->mVelocity;
    float push = (Dot_6F78(off, Sub_6F78(hit.vel, self->mVelocity)) >? 0.0f) + 55.55555725097656f; // PORT: g++ maximum operator
    AddEq_6F78(self->mVelocity, Scale_6F78(off, push));
    t4Vector imp = Scale_6F78(Sub_6F78(oldVel, self->mVelocity), (float)*(int*)((char*)SSXApp + 0x10));
    if (hit.obj != 0) {
        char* handler = *(char**)(hit.obj + 0xC);
        if (handler != 0) {
            sVEhv_6F78* vt = *(sVEhv_6F78**)(handler + 0xC);
            vt[43].fn(handler + vt[43].delta, &hit, &imp);
        }
    }
    if (cRider_getMotionState(self) == 0) {
        hit.n = off;
        cRider_turnFromCollision(self, &hit);
        cRider_updateOrientationImplicit(self);
    }
    cRider_reactToCollision(self, &info, hit.mat);
    return 1;
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
static inline sVec4 SubV(const sVec4& a, const sVec4& b)
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
static inline sVec4 DivV(const sVec4& v, float s)
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

struct sHit_7578 {
    sVec4 pos;
    sVec4 normal;
    char pad[0x80 - 0x20];
};

extern "C" void cCheatFX_setCheatInput(void*, int);
extern "C" int cOverlapList_splineIntersect_7578(void*, sVec4*, sHit_7578*, int, float) __asm__("cOverlapList_splineIntersect");
extern "C" void cHandplantMotion_init(float, void*, sHit_7578*);
extern "C" void cHandplantControl_init(float, void*, sHit_7578*, int);

// PORT: PS2-only VU0 inline asm (vector times scalar, sVec4 operands).
static inline sVec4 ScaleV_7578(const sVec4& v, float s)
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
static inline sVec4 AddV_7578(const sVec4& a, const sVec4& b)
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

static inline sVec4 Up_7578(char* self) { return *(sVec4*)(self + 0x180); }
static inline sVec4 Front_7578(char* self) { return *(sVec4*)(self + 0x1B0); }

// Speeds are written in km/h and converted to game units (m/s).
static inline float Kph_7578(float kph) { return kph / 3.6f; }
/**
 * Starts a handplant when the rider reaches a near-vertical rail surface ahead.
 *
 * Sends cheat input 2 (cCheatFX_setCheatInput on the handler's +0xD20). In the air (motion state 1) it gives up when
 * falling faster than 2000 km/h, upside down, or in channel 2 animation categories 0x12 .. 0x14. It probes 300 ahead
 * along the velocity (the front axis below 100 km/h) for a spline (cOverlapList_splineIntersect, type 2); the hit must be within 300,
 * steep (|normal.z| <= 0.9) and ahead of the motion. Then control state 0xD, cHandplantMotion_init and cHandplantControl_init
 * (cHandplantControl init) with clamp(5000 km/h / max(speed, 1000 km/h), 0.5, 1.5) / 2, control state 0xB and
 * cRider_setMotionState(5).
 *
 * Callers pass a second bool (On Tour isTransThrough); this version takes and reads only the rider.
 *
 * @return 1 when a handplant started
 */
extern "C" int cRider_checkHandplantCollisions(cRider* self)
{
    cCheatFX_setCheatInput((char*)self->mRiderHandler + 0xD20, 2);
    if (cRider_getMotionState((char*)self) == 1) {
        if (self->mVelocity.z < Kph_7578(-2000.0f))
            return 0;
        if (Up_7578((char*)self).z < 0.0f)
            return 0;
        if ((unsigned int)(cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 2) - 18) < 3)
            return 0;
    }
    float spd = Len(*(sVec4*)&self->mVelocity);
    sVec4 dir;
    sHit_7578 hit;
    if (spd > Kph_7578(100.0f))
        dir = DivV(*(sVec4*)&self->mVelocity, spd);
    else
        dir = Front_7578((char*)self);
    sVec4 probePos = AddV_7578(*(sVec4*)&self->mPosition, ScaleV_7578(dir, 300.0f));
    if (!cOverlapList_splineIntersect_7578(self->mOverlapList, &probePos, &hit, 2, 300.0f))
        return 0;
    if (Abs(hit.normal.z) > 0.8999999761581421f)
        return 0;
    sVec4 toHit = SubV(hit.pos, probePos);
    if (Len(toHit) > 300.0f)
        return 0;
    toHit = SubV(hit.pos, *(sVec4*)&self->mPosition);
    if (Dot(*(sVec4*)&self->mVelocity, toHit) < 0.0f)
        return 0;
    spd = Max_7578(spd, Kph_7578(1000.0f));
    float speedRatio = Clamp(Kph_7578(5000.0f) / spd, 0.5f, 1.5f);
    float enterSec = speedRatio * 0.5f;
    if (Dot(hit.normal, *(sVec4*)&self->mVelocity) < 0.0f)
        ScaleEq(hit.normal, -1.0f);
    cRider_setControlState_2(((char*)self), 13);
    cHandplantMotion_init(enterSec, (char*)self->mRiderHandler + 0x110, &hit);
    cHandplantControl_init(enterSec, (char*)self->mRiderHandler + 0x370, &hit, *(int*)((char*)self->mRiderHandler + 0x1A4));
    cRider_setControlState_2(((char*)self), 11);
    cRider_setMotionState_7578(((char*)self), 5);
    return 1;
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
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" int AITick();
extern "C" void* cBE_getBE();
void* cBE_getInterface_C258(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern sApp_7888* D_004A28A8_7888 __asm__("SSXApp");
// Rider-rider collisions: for every other active rider, push the two apart when their sphere trees touch
// (and exchange momentum along the contact), then shove the other rider when this one runs into it.

// A rider's per-rider record (indexed by the other rider's AI index): set flag and the frame counters of the
// last contact, momentum exchange and shove.
struct sRec_7888 {
    int active;         // 0x00
    char pad4[0xC];
    int lastExchange;   // 0x10
    int lastContact;    // 0x14
    int lastShove;      // 0x18
    char pad1C[0x24 - 0x1C];
};

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

static inline sRec_7888* Rec_7888(cRider* rider, int idx) { return &((sRec_7888*)rider)[idx]; }

/**
 * Handles this rider's contacts with the other riders: pushes overlapping bodies apart and lands shoves.
 *
 * Once per tick per pair, overlapping sphere trees (cRiderSphereTree_IsRiderCollision) push both riders 0.55 of the contact
 * vector apart along their ground planes and, at most every 3 ticks, exchange momentum along the contact
 * (cRider_doOpponentCollision). While this rider's channel-1 animation (class 13, probably the punch) is in its hit
 * window, a rider within 150 in front of PunchVect that is not blocking is shoved with a strength from the
 * collision stat and PunchStrength, at most every 3 ticks per pair. Actors (mIsActor) are skipped.
 */
extern "C" void cRider_checkOpponentCollisions(cRider* self)
{
    int busy = 0;
    float mass = cRider_getMass(self);
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 1) != 13 ||
        func_001446A0(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0, 1) ||
        !func_001446A0(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 1) + 0xB0, 0))
        busy = 1;
    for (int i = 0; i < 6; i++) {
        if (!Rec_7888(self, i)->active)
            continue;
        cRider* other = (cRider*)D_004A28A8_7888->game->race->riders[i];
        if (other->mIsActor != 0)
            continue;
        sRec_7888* orec = Rec_7888(other, self->mAIIndex);
        if (orec->lastContact < AITick()) {
            sRec_7888* srec = Rec_7888(self, other->mAIIndex);
            srec->lastContact = AITick();
            orec->lastContact = AITick();
            t4Vector dir;
            t4Vector pos;
            if (cRiderSphereTree_IsRiderCollision(self->mSphereTree, other->mSphereTree, &dir, &pos)) {
                float omass = cRider_getMass(other);
                t4Vector selfPush = dir * -0.550000011920929f;
                t4Vector otherPush = dir * 0.550000011920929f;
                if (cRider_isGroundNormalValid(self))
                    SubEq_7888(selfPush, Scale_7888(self->mGroundNormal, Dot_7888(selfPush, self->mGroundNormal)));
                if (cRider_isGroundNormalValid(other))
                    SubEq_7888(otherPush, Scale_7888(other->mGroundNormal, Dot_7888(otherPush, other->mGroundNormal)));
                cRider_moveFromCollision_m2537(self, selfPush);
                cRider_moveFromCollision_m2537(other, otherPush);
                if (srec->lastExchange < AITick() - 3) {
                    t4Vector n = Norm_7888(dir);
                    float impulse = (Dot_7888(other->mVelocity, n) - Dot_7888(self->mVelocity, n)) / (mass + omass);
                    cRider_doOpponentCollision_7888(self, other, &n, 0, omass * impulse);
                    cRider_doOpponentCollision_7888(other, self, &n, 0, -mass * impulse);
                    srec->lastExchange = AITick();
                    orec->lastExchange = AITick();
                }
            }
        }
        if (busy)
            continue;
        if (orec->lastShove >= AITick() - 3)
            continue;
        if (cRiderAnimBase_getCurrentAnimCategory(other->mRiderAnim, 1) == 3 &&
            func_001446A0(cRiderAnimBase_getCurrentAnimSequence(other->mRiderAnim, 1) + 0xB0, 0) &&
            !func_001446A0(cRiderAnimBase_getCurrentAnimSequence(other->mRiderAnim, 1) + 0xB0, 1))
            continue;
        t4Vector toOther = Sub_7888(other->mPosition, self->mPosition);
        float len = Len_7888(toOther);
        if (len > 150.0f)
            continue;
        if (len < 0.001f)
            continue;
        t4Vector n = Div_7888(toOther, len);
        if (!(Dot_7888(n, self->PunchVect) > 0.0f))
            continue;
        float attr = cBEStatInterface_getStrikingAttrib(cBE_getInterface_C258(cBE_getBE(), 3), self->mAIIndex, self->mStatsIndex);
        float spd = Len_7888(self->mVelocity);
        float k = (attr * 3.0001144409179688f + 1.0f) * (self->PunchStrength + 0.5f) * 599.9739990234375f;
        if (0.001f < spd)
            n = Add_7888(Scale_7888(n, 0.5f), Scale_7888(self->mVelocity, 0.5f / spd));
        cRider_doOpponentCollision_7888(other, self, &n, 1, k);
        orec->lastShove = AITick();
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_doOpponentCollision);
#ifdef SKIP_ASM
struct sV4_7E70 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sInfo_7E70 {
    sV4_7E70 pos;
    sV4_7E70 dir;
    sV4_7E70 normal;
    float speed;
} __attribute__((aligned(16)));

struct sRec_7E70 {
    int active;
    int pad4;
    float dist;
    float angle;
    char pad10[0x24 - 0x10];
};

class cAI_7E70 {
public:
    virtual void vm1();
    virtual sV4_7E70* GetDir();
    virtual void v1();
    virtual void v2();
    virtual sV4_7E70* GetPos();
};

int cRider_getControlState_7E70(void*) __asm__("cRider_getControlState__FPv");
int cRider_getMotionState(void*);
int AIrand();
extern "C" int cRider_isGroundNormalValid(void*);
extern "C" void cAirPredictor_startLaunchIntoAir(void*, sV4_7E70*, sV4_7E70*, float);
extern "C" void cRider_flagEventWipeout_7E70(char*, int, int, int, sInfo_7E70*) __asm__("cRider_flagEventWipeout");
extern "C" void cRider_doBump_7E70(char*, sInfo_7E70*, int) __asm__("cRider_doBump");
extern "C" void cRider_flagEventPunchKnockdown(void*, void*);
extern "C" void cRider_flagEventBumpKnockdown(void*, void*);
extern "C" void cRider_flagEventPunch(void*, void*);
extern "C" void cRider_flagEventBump(void*, void*);
extern float D_004A4C84_7E70 __asm__("D_004A4C84");
extern int D_005308D0_7E70[] __asm__("D_005308D0");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_7E70(const sV4_7E70& a, const sV4_7E70& b)
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
static inline float Len_7E70(const sV4_7E70& v)
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
static inline sV4_7E70 Add_7E70(const sV4_7E70& a, const sV4_7E70& b)
{
    sV4_7E70 r;
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
static inline void AddEq_7E70(sV4_7E70& a, const sV4_7E70& b)
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
static inline sV4_7E70 Sub_7E70(const sV4_7E70& a, const sV4_7E70& b)
{
    sV4_7E70 r;
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
static inline sV4_7E70 Scale_7E70(const sV4_7E70& v, float s)
{
    sV4_7E70 r;
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
static inline sV4_7E70 Norm_7E70(const sV4_7E70& v)
{
    sV4_7E70 r;
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

static inline sV4_7E70 Get_7E70(char* p) { return *(sV4_7E70*)p; }

/**
 * Applies a rider-vs-rider shove or punch to this rider and picks a bump or a knockdown.
 *
 * Ignored in control state 10. Adds @p n x clamp(@p speed, +-555.6) to mVelocity (flattened onto mGroundNormal when it
 * is valid); in the air it relaunches the air predictor and ignores a push along the up axis. No reaction follows in
 * motion state 2, control state 9 or below 40 speed.
 *
 * Above 599.97 it picks a knockdown animation from the push direction against the rider's axes for cRider_flagEventWipeout
 * (flagEventWipeout) and flags cRider_flagEventPunchKnockdown (punch knockdown) or cRider_flagEventBumpKnockdown (bump knockdown); otherwise it bumps
 * (cRider_doBump) and flags cRider_flagEventPunch (punch) or cRider_flagEventBump (bump). Rules bit 5 (D_005308D0) sets the speed to
 * exactly 599.97 and the test is strict, so that rule turns every knockdown into a bump. Called by
 * cRider_checkOpponentCollisions.
 *
 * @param other  the other rider
 * @param n  the unit push direction
 * @param flag  nonzero for a punch (On Tour isPunch), 0 for a collision
 * @param speed  the signed impulse along @p n (On Tour impulse)
 */
extern "C" void cRider_doOpponentCollision(cRider* self, cRider* other, sV4_7E70* n, int flag, float speed)
{
    if (cRider_getControlState_7E70((char*)self) == 10)
        return;
    float absSpd = Abs_7E70(speed);
    float lim = 555.5555419921875f;
    float clampedSpeed = Clamp_7E70(speed, -lim, lim);
    sV4_7E70 imp = Scale_7E70(*n, clampedSpeed);
    if (cRider_isGroundNormalValid((char*)self)) {
        AddEq_7E70(*(sV4_7E70*)&self->mVelocity,
                   Sub_7E70(imp, Scale_7E70(*(sV4_7E70*)&self->mGroundNormal, Dot_7E70(imp, *(sV4_7E70*)&self->mGroundNormal))));
    } else {
        AddEq_7E70(*(sV4_7E70*)&self->mVelocity, imp);
        if (cRider_getMotionState((char*)self) == 1 || (cRider_getMotionState((char*)self) == 2 && *(int*)((char*)self->mRiderHandler + 0x30) == 1)) {
            cAI_7E70* ai = (cAI_7E70*)(&self->mVtbl);
            sV4_7E70* pos = ai->GetPos();
            cAirPredictor_startLaunchIntoAir(self->mAirPredictor, pos, ai->GetDir(), self->unk_2E4);
            if (Dot_7E70(*n, Get_7E70((char*)&self->mMatrix.row[2])) * clampedSpeed > 0.0f)
                absSpd = 0.0f;
        }
    }
    if (cRider_getMotionState((char*)self) == 2)
        return;
    if (cRider_getControlState_7E70((char*)self) == 9)
        return;
    if (absSpd < 39.99532699584961f)
        return;
    float otherDist = ((sRec_7E70*)self)[other->mAIIndex].dist;
    sInfo_7E70 info;
    info.pos = Add_7E70(*(sV4_7E70*)&self->mPosition, Scale_7E70(*n, otherDist));
    info.dir = Norm_7E70(*(sV4_7E70*)&self->mVelocity);
    info.normal = *n;
    info.speed = clampedSpeed;
    if ((D_005308D0_7E70[0] >> 5) & 1)
        absSpd = 599.9739990234375f;
    if (absSpd > 599.9739990234375f) {
        float dz = Dot_7E70(Get_7E70(((char*)self) + 0x160), *n);
        float dy = Dot_7E70(Get_7E70((char*)&self->unk_170), *n);
        float dx = Dot_7E70(Get_7E70((char*)&self->unk_180), *n);
        int anim;
        if (Abs_7E70(dz) < Abs_7E70(dy) && Abs_7E70(dx) < Abs_7E70(dy)) {
            if (dy < 0.0f) {
                unsigned int k;
                if (Len_7E70(*(sV4_7E70*)&self->mVelocity) > D_004A4C84_7E70)
                    k = (unsigned int)AIrand() % 6;
                else
                    k = (unsigned int)AIrand() % 3;
                if (k == 0)
                    anim = 334;
                else if (k == 1)
                    anim = 335;
                else if (k == 2)
                    anim = 337;
                else if (k == 3)
                    anim = 333;
                else
                    anim = 336;
            } else {
                anim = (AIrand() & 1) == 0 ? 330 : 331;
            }
        } else if (Abs_7E70(dz) < Abs_7E70(dx) && Abs_7E70(dy) < Abs_7E70(dx)) {
            anim = 331;
        } else if (dz < 0.0f) {
            anim = 329;
        } else {
            anim = 328;
        }
        cRider_flagEventWipeout_7E70(((char*)self), anim, flag, 0, &info);
        if (flag)
            cRider_flagEventPunchKnockdown(((char*)self), ((char*)other));
        else
            cRider_flagEventBumpKnockdown(((char*)self), ((char*)other));
    } else {
        cRider_doBump_7E70(((char*)self), &info, 0);
        if (flag)
            cRider_flagEventPunch(((char*)self), ((char*)other));
        else
            cRider_flagEventBump(((char*)self), ((char*)other));
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_doBump);
#ifdef SKIP_ASM
struct sV4_8388
{
    float x, y, z, w;
} __attribute__((aligned(16)));

int cRider_getMotionState(void*);
int cRider_getControlState_8388(void*) __asm__("cRider_getControlState__FPv");
void cRider_setControlState_8388(void* self, int v) __asm__("cRider_setControlState__FPv");
int AIrand();
extern "C" void cBoardPressControl_forceExit(void*);
extern "C" void cRiderAnimBase_play(void*, int, int, float);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void func_002A0E70(void*, void*, int);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Len_8388(const sV4_8388& v)
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
static inline float Dot_8388(const sV4_8388& a, const sV4_8388& b)
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

static inline sV4_8388 Get_8388(char* p) { return *(sV4_8388*)p; }

/**
 * Plays a stumble (bump) animation for a glancing collision.
 *
 * Only on the ground or in motion state 4, and only in control states 0, 1 (a board press, ended by
 * cBoardPressControl_forceExit on the handler's +0x1D0), 2 or 7. A contact normal (info +0x20) along the front axis
 * (+0x1B0) plays 0x37 (front) or 0x38 (back). Sideways (side axis +0x1A0, mirrored when riding fakie, +0x320) a slow
 * contact (normal speed < 1111.1) at over 1388.9 speed off motion state 4 adds a random number of half turns to
 * mSpinRate and plays 0x3A/0x3C; otherwise 0x39/0x3B. With @p flag it plays the bump sound (func_002A0E70). Ends in
 * control state 3.
 *
 * @param info  the collide info (contact normal at +0x20, normal speed at +0x30)
 * @param flag  nonzero to play the bump sound (On Tour isObject)
 */
extern "C" void cRider_doBump(cRider* self, char* info, int flag)
{
    if (cRider_getMotionState((char*)self) != 0 && cRider_getMotionState((char*)self) != 4)
        return;
    if (cRider_getControlState_8388((char*)self) == 1) {
        cBoardPressControl_forceExit((char*)self->mRiderHandler + 0x1D0);
    } else if (cRider_getControlState_8388((char*)self) != 0 && cRider_getControlState_8388((char*)self) != 2 && cRider_getControlState_8388((char*)self) != 7) {
        return;
    }
    int played = 0;
    float frontDot = Dot_8388(Get_8388((char*)&self->mMatrix.row[1]), *(sV4_8388*)(info + 0x20));
    if (frontDot > 0.7071067690849304f) {
        cRiderAnimBase_play(self->mRiderAnim, 55, 0, -1.0f);
    } else if (frontDot < -0.7071067690849304f) {
        cRiderAnimBase_play(self->mRiderAnim, 0x38, 0, -1.0f);
    } else {
        float side = Dot_8388(Get_8388((char*)&self->mMatrix), *(sV4_8388*)(info + 0x20));
        if (self->IsRidingFakie)
            side = -side;
        if (*(float*)(info + 0x30) < 1111.111083984375f && Len_8388(*(sV4_8388*)&self->mVelocity) > 1388.888916015625f &&
            cRider_getMotionState((char*)self) != 4) {
            float spinRate = self->mSpinRate;
            if (Abs_8388(spinRate) < 6.2831854820251465f) {
                if (spinRate < 0.0f)
                    self->mSpinRate += (int)((unsigned int)AIrand() % 3) * -3.1415927410125732f;
                else if (spinRate > 0.0f)
                    self->mSpinRate += (int)((unsigned int)AIrand() % 3) * 3.1415927410125732f;
                else
                    self->mSpinRate += ((int)((unsigned int)AIrand() % 5) - 2) * 3.1415927410125732f;
            }
            cRiderAnimBase_play(self->mRiderAnim, side < 0.0f ? 58 : 0x3C, 0, -1.0f);
            played = 1;
        } else {
            cRiderAnimBase_play(self->mRiderAnim, side < 0.0f ? 57 : 59, 0, -1.0f);
        }
    }
    if (flag)
        func_002A0E70(cAudioInterface_bxAudio(), ((char*)self), played);
    cRider_setControlState_8388(((char*)self), 3);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_isRailBoardIntersect);
#ifdef SKIP_ASM
struct sV4_86B8
{
    float x, y, z, w;
    sV4_86B8() {}
    sV4_86B8(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

struct sNode_86B8 {
    sV4_86B8 pos;
    float qx, qy, qz, qw;
};

struct sPath_86B8 {
    char pad[0x2C];
    sNode_86B8* nodes;
};

struct sHit_86B8 {
    sV4_86B8 pos;
    char pad[0x80 - 0x10];
};

extern "C" int cOverlapList_splineIntersect_86B8(void*, sV4_86B8*, sV4_86B8*, int, float) __asm__("cOverlapList_splineIntersect");

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV4_86B8 Add_86B8(const sV4_86B8& a, const sV4_86B8& b)
{
    sV4_86B8 r;
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
static inline sV4_86B8 Sub_86B8(const sV4_86B8& a, const sV4_86B8& b)
{
    sV4_86B8 r;
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
static inline sV4_86B8 Scale_86B8(const sV4_86B8& v, float s)
{
    sV4_86B8 r;
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
static inline sV4_86B8 Div_86B8(const sV4_86B8& v, float s)
{
    sV4_86B8 r;
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
static inline float Len_86B8(const sV4_86B8& v)
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
static inline float Dot_86B8(const sV4_86B8& a, const sV4_86B8& b)
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

static inline sV4_86B8 Fwd_86B8(const sNode_86B8* n)
{
    return sV4_86B8(1.0f - (n->qy * n->qy + n->qz * n->qz) * 2.0f,
                    (n->qx * n->qy + n->qw * n->qz) * 2.0f,
                    (n->qx * n->qz - n->qw * n->qy) * 2.0f,
                    0.0f);
}

extern "C" int cRider_isRailBoardIntersect_86B8(char* self, sV4_86B8* tgt, sV4_86B8* dir) __asm__("cRider_isRailBoardIntersect");
/**
 * Tells whether the rider's board reaches a rail.
 *
 * The board's axis and position come from the path node (+0x780 path, node index +0x8A0; axis from the node's
 * quaternion, position plus +0x9D0). With the blend +0x25C, the reach is lerp(30, 50) and the half length lerp(100,
 * 170) (lerp(50, 170) when +0x330 is set), less the reach. It returns 1 when the board segment's point nearest @p tgt
 * is within reach. Otherwise, unless @p dir is almost parallel to the board (1 - d * d < 0.001), it takes the board
 * point nearest the rail line, finds the spline nearest it (cOverlapList_splineIntersect, overlap list +0x860, range
 * 300) and returns 1 when that point is within reach.
 *
 * @param tgt  the rail point
 * @param dir  the rail direction
 * @return 1 when the board reaches the rail, else 0
 */
extern "C" int cRider_isRailBoardIntersect_86B8(char* self, sV4_86B8* tgt, sV4_86B8* dir)
{
    sNode_86B8* n = &(*(sPath_86B8**)(self + 0x780))->nodes[*(int*)(self + 0x8A0)];
    const sV4_86B8& fwd = Fwd_86B8(n);
    sV4_86B8 pos = Add_86B8(n->pos, *(sV4_86B8*)(self + 0x9D0));
    float blend = *(float*)(self + 0x25C);
    float nearR = Lerp_86B8(50.0f, 30.0f, blend);
    float farR;
    if (!*(int*)(self + 0x330))
        farR = Lerp_86B8(170.0f, 100.0f, blend);
    else
        farR = Lerp_86B8(170.0f, 50.0f, blend);
    float proj = Dot_86B8(fwd, Sub_86B8(*tgt, pos));
    float lo = nearR - farR;
    float hi = farR - nearR;
    sV4_86B8 diff = Sub_86B8(Add_86B8(pos, Scale_86B8(fwd, Clamp_86B8(proj, lo, hi))), *tgt);
    if (Len_86B8(diff) <= nearR)
        return 1;
    float dd = Dot_86B8(*dir, fwd);
    float den = 1.0f - dd * dd;
    if (den < 0.0010000000474974513f)
        return 0;
    sV4_86B8 relScaled = Div_86B8(Sub_86B8(pos, *tgt), den);
    float along = Dot_86B8(relScaled, Sub_86B8(Scale_86B8(*dir, dd), fwd));
    sV4_86B8 probePos = Add_86B8(pos, Scale_86B8(fwd, Clamp_86B8(along, lo, hi)));
    sHit_86B8 hit;
    if (!cOverlapList_splineIntersect_86B8(*(void**)(self + 0x860), &probePos, &hit.pos, 1, 300.0f))
        return 0;
    diff = Sub_86B8(probePos, hit.pos);
    return Len_86B8(diff) <= nearR;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cRider_isRailLandSuccess);
#ifdef SKIP_ASM
struct sVec4_8A48 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sNode_8A48 {
    sVec4_8A48 pos;
    char pad[0x10];
};

struct sPath_8A48 {
    char pad[0x2C];
    sNode_8A48* nodes;
};

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec4_8A48 vAdd_8A48(const sVec4_8A48& a, const sVec4_8A48& b)
{
    sVec4_8A48 r;
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
static inline sVec4_8A48 vSub_8A48(const sVec4_8A48& a, const sVec4_8A48& b)
{
    sVec4_8A48 r;
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
static inline float vLen_8A48(const sVec4_8A48& v)
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
static inline float vDot_8A48(const sVec4_8A48& a, const sVec4_8A48& b)
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
extern "C" int cOverlapList_splineIntersect(void*, sVec4_8A48*, sVec4_8A48*, int, float);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" char* cRiderAnimBase_getCurrentAnimSequence(void*, int);
extern "C" int func_001446A0(void*, int);
extern "C" int cRider_isRailBoardIntersect(void*, sVec4_8A48*, sVec4_8A48*);

/**
 * Tests whether the rider's board can land on a nearby rail.
 *
 * Only on the ground or in the air (motion states 0 and 1). It probes for a rail (cOverlapList_splineIntersect, type 1, radius 300)
 * from the board root bone (mAnimModel bone mBoneBoardRoot + mModelOffset) and rejects rails behind the motion. In
 * channel 2 animation categories 0x12 .. 0x14 the current sequence's flags (func_001446A0 on cRiderAnimBase_getCurrentAnimSequence + 0xB0) must
 * allow it. The answer is cRider_isRailBoardIntersect (On Tour isRailBoardIntersect) on the hit. Callers:
 * cRider_checkRailBoardCollisions and cRider_checkRailBodyCollisions.
 *
 * @param tgt  receives the rail intersection (tIntersectInfo)
 * @return nonzero when the board lands on the rail
 */
extern "C" int cRider_isRailLandSuccess(cRider* self, sVec4_8A48* tgt)
{
    if (cRider_getMotionState((char*)self) != 0 && cRider_getMotionState((char*)self) != 1)
        return 0;
    sVec4_8A48 boardCenter = vAdd_8A48(*(sVec4_8A48*)((char*)((sPath_8A48*)self->mAnimModel)->nodes + (self->mBoneBoardRoot << 5)),
                             *(sVec4_8A48*)&self->mModelOffset);
    if (!cOverlapList_splineIntersect(self->mOverlapList, &boardCenter, tgt, 1, 300.0f))
        return 0;
    float spd = vLen_8A48(*(sVec4_8A48*)&self->mVelocity);
    if (0.001f < spd) {
        sVec4_8A48 toRail = vSub_8A48(*tgt, boardCenter);
        if (vDot_8A48(toRail, *(sVec4_8A48*)&self->mVelocity) < spd * -0.20000000298023224f)
            return 0;
    }
    if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 2) == 18) {
        if (!func_001446A0(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 2) + 0xB0, 2) &&
            func_001446A0(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 2) + 0xB0, 0))
            return 0;
    } else if (cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 2) == 19 ||
               cRiderAnimBase_getCurrentAnimCategory(self->mRiderAnim, 2) == 20) {
        if (!func_001446A0(cRiderAnimBase_getCurrentAnimSequence(self->mRiderAnim, 2) + 0xB0, 2))
            return 0;
    }
    return cRider_isRailBoardIntersect(((char*)self), tgt, tgt + 1);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cRider_isIntersecting);
#ifdef SKIP_ASM
struct S_8C28 { char pad[0x5B8]; unsigned arr[64]; };
/**
 * Tells whether the rider's last static-object test hit an instance.
 *
 * Compares the instance's id (+0x78) with the ids at +0x5B8 that cRider_objectIntersect records (at most 64, ended by
 * 0xFFFFFFFF). Virtual slot 3 (word 7) of the rider vtables.
 *
 * @param rider  the rider
 * @param instance  the instance
 * @return 1 when the id is listed, else 0
 */
extern "C" int cRider_isIntersecting(cRider* rider, void *instance) {
    int i = 0;
    if (~((unsigned int*)&rider->mVisitedIds)[0]) {
        do {
            if (((unsigned int*)&rider->mVisitedIds)[i] == *(unsigned *)((char*)instance + 0x78)) {
                return 1;
            }
            i++;
        } while (i < 64 && ((unsigned int*)&rider->mVisitedIds)[i] != 0xFFFFFFFF);
    }
    return 0;
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
extern char D_00459B90[];
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
        *(cRider**)(((char*)self) + 0xD20) = vb;
        *(cRider**)(((char*)self) + 0xC70) = vb;
        *(cRider**)(((char*)self) + 0xB40) = vb;
        *(cRider**)(((char*)self) + 0xB00) = vb;
        *(cRider**)(((char*)self) + 0xAF0) = vb;
        *(cRider**)(((char*)self) + 0xAD0) = vb;
        *(cRider**)(((char*)self) + 0x9C0) = vb;
        *(cRider**)(((char*)self) + 0x610) = vb;
        *(cRider**)(((char*)self) + 0x520) = vb;
        *(cRider**)(((char*)self) + 0x470) = vb;
        *(cRider**)(((char*)self) + 0x3B0) = vb;
        *(cRider**)(((char*)self) + 0x3A0) = vb;
        *(cRider**)(((char*)self) + 0x398) = vb;
        *(cRider**)(((char*)self) + 0x384) = vb;
        *(cRider**)(((char*)self) + 0x364) = vb;
        *(cRider**)(((char*)self) + 0x358) = vb;
        *(cRider**)(((char*)self) + 0x340) = vb;
        *(cRider**)(((char*)self) + 0x2B8) = vb;
        *(cRider**)(((char*)self) + 0x2A4) = vb;
        *(cRider**)(((char*)self) + 0x288) = vb;
        *(cRider**)(((char*)self) + 0x224) = vb;
        *(cRider**)(((char*)self) + 0x200) = vb;
        *(cRider**)(((char*)self) + 0x1F0) = vb;
        *(cRider**)(((char*)self) + 0x1E4) = vb;
        *(cRider**)(((char*)self) + 0x1C0) = vb;
        *(cRider**)(((char*)self) + 0x1B0) = vb;
        *(cRider**)(((char*)self) + 0x100) = vb;
        *(cRider**)(((char*)self) + 0xA0) = vb;
        *(cRider**)(((char*)self) + 0x70) = vb;
        *(cRider**)(((char*)self) + 0x24) = vb;
        self->mRider = vb;
        cRider_cRider(vb);
    }
    cRiderHandler_cRiderHandler(((char*)self), 0);
    (self->mRider)->mScriptObjectVtbl = (void*)&D_00458618;
    (self->mRider)->mScriptObjRiderVtbl = D_00459B90;
    (self->mRider)->mVtbl = (void*)&D_00458660;
    if (!inChrg)
    {
        sVtbl9_108C80 ctorVtbl9 = D_00458618;
        (self->mRider)->mScriptObjectVtbl = &ctorVtbl9;
        char* base = (char*)self->mRider - 0xF50;
        int vbOffset = ((char*)self) - base;
        ctorVtbl9.e[1].delta = D_00458618.e[1].delta + vbOffset;
        sVtbl22_108C80 ctorVtbl22 = D_00458660;
        (self->mRider)->mVtbl = &ctorVtbl22;
        ctorVtbl22.e[1].delta = D_00458660.e[1].delta + vbOffset;
        ctorVtbl22.e[20].delta = D_00458660.e[20].delta + vbOffset;
    }
    self->mVtbl = (void**)cComputer_vtbl;
    if (actor)
        (self->mRider)->mIsActor = 1;
    cComputer_Init_self((char*)self);
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_Init);
#ifdef SKIP_ASM
class cObj_8E88b;
typedef void (cObj_8E88b::*Fn_8E88b)();
extern "C" Fn_8E88b D_0043CF20_8E88b[4] __asm__("D_0043CF20");
struct sQ_8E88 {
    int v[4];
} __attribute__((aligned(16)));
extern sQ_8E88 D_004FF130_8E88 __asm__("t4vect_Origin");
extern "C" void cComputer_initRiderDifficulty(void* self);
extern "C" void cComputer_resetCounters(void* self);
extern "C" void* MEM_fill_8E88(void* dst, int value, int size) __asm__("MEM_fill");

// PORT: the unit later declares cComputer_Init without parameters; it takes the rider.
void cComputer_Init_self(char* self) __asm__("cComputer_Init");

/**
 * Resets a computer rider's AI state.
 *
 * Clears its counters and flags (+0xDF0, +0xE10 .. +0xE3C, +0xE40, +0xE74, +0xE78, +0xF30, +0xF34; +0xE0C and later
 * +0xE70 = -1), zeroes the vectors at +0xE50 and +0xE60, runs cComputer_initRiderDifficulty, sets +0xDF4 = 1.0,
 * installs the cruise logic member-function pointer (+0xF44 = D_0043CF20[1]; On Tour cComputer::mCruiseLogic) with
 * +0xF38 = 1, clears the three 0x3C-byte tables at +0xE7C, +0xEB8 and +0xEF4 (MEM_fill) and runs
 * cComputer_resetCounters. Called at the end of cComputer_cComputer and by func_00108F88.
 */
void cComputer_Init_self(char* self)
{
    *(int*)(self + 0xE0C) = -1;
    *(int*)(self + 0xE10) = 0;
    *(int*)(self + 0xE14) = 0;
    *(int*)(self + 0xE18) = 0;
    *(int*)(self + 0xE20) = 0;
    *(int*)(self + 0xE24) = 0;
    *(int*)(self + 0xE28) = 0;
    *(int*)(self + 0xE1C) = 0;
    *(int*)(self + 0xE2C) = 0;
    *(int*)(self + 0xE38) = 0;
    *(int*)(self + 0xE3C) = 0;
    *(int*)(self + 0xE30) = 0;
    *(int*)(self + 0xE34) = 0;
    *(int*)(self + 0xDF0) = 0;
    *(int*)(self + 0xE74) = 0;
    *(int*)(self + 0xE78) = 0;
    *(short*)(self + 0xF38) = 0;
    *(int*)(self + 0xF30) = 0;
    *(int*)(self + 0xF34) = 0;
    *(sQ_8E88*)(self + 0xE50) = D_004FF130_8E88;
    *(sQ_8E88*)(self + 0xE60) = D_004FF130_8E88;
    *(int*)(self + 0xE40) = 0;
    cComputer_initRiderDifficulty(self);
    *(int*)(self + 0xE70) = -1;
    *(float*)(self + 0xDF4) = 1.0f;
    *(Fn_8E88b*)(self + 0xF44) = D_0043CF20_8E88b[1];
    *(short*)(self + 0xF38) = 1;
    do {
        MEM_fill_8E88(self + 0xE7C, 0, 0x3C);
        MEM_fill_8E88(self + 0xEB8, 0, 0x3C);
        MEM_fill_8E88(self + 0xEF4, 0, 0x3C);
    } while (0);
    cComputer_resetCounters(self);
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
// cRider's own 0x6D0 vtable (D_00459B90), which g++ only shares between classes without RTTI.
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
        "_vt$14cRiderVbB0_s12 = D_00459B90\n"
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
extern "C" void cComputer_getCruiseCmd_A768(void* self, void* pad) __asm__("cComputer_getCruiseCmd");
extern "C" void cComputer_getPrewindCmd_A768(void* self, void* pad) __asm__("cComputer_getPrewindCmd");
extern "C" void cComputer_getGateCmd_A768(void* self, void* pad) __asm__("cComputer_getGateCmd");
extern "C" void cComputer_getNaturalAirCmd_A768(void* self, void* pad) __asm__("cComputer_getNaturalAirCmd");
extern "C" void cComputer_getRailslideCmd_A768(void* self, void* pad) __asm__("cComputer_getRailslideCmd");
extern "C" void cComputer_getSpinCmd_A768(void* self, void* pad) __asm__("cComputer_getSpinCmd");
extern "C" void cComputer_getWipeoutCmd_A768(void* self, void* pad) __asm__("cComputer_getWipeoutCmd");
extern "C" void cComputer_getBoardPressCmd_A768(void* self, void* pad) __asm__("cComputer_getBoardPressCmd");

/**
 * Builds this computer rider's control command for the frame, the AI's stand-in for reading a pad.
 *
 * Zeroes the 8-byte command, applies the rubber-band time warp (cRider_updateTimeWarp() with cComputer_setupWarp's
 * factor), then fills it with the get*Cmd for the rider's control state: cruise (cComputer_getCruiseCmd), board press,
 * prewind, natural air, spin, gate, railslide or wipeout (cComputer_getWipeoutCmd); other states leave it empty.
 * Outside the spin state it clears mGrabEndTime.
 *
 * @param _cmd  the command to fill (On Tour tControlCmd&)
 */
// Clears the pad input, updates the rider, then runs the input handler for the rider state.
extern "C" void cComputer_getControlCmd(cComputer* self, void* _cmd)
{
    MEM_fill(_cmd, 0, 8);
    cRider_updateTimeWarp(self->mRider, cComputer_setupWarp((char*)self));
    int state = cRider_getControlState_A768(self->mRider);
    switch (state) {
    case 0:
        cComputer_getCruiseCmd_A768(((char*)self), _cmd);
        break;
    case 2:
        cComputer_getPrewindCmd_A768(((char*)self), _cmd);
        break;
    case 6:
        cComputer_getGateCmd_A768(((char*)self), _cmd);
        break;
    case 4:
        cComputer_getNaturalAirCmd_A768(((char*)self), _cmd);
        break;
    case 7:
        cComputer_getRailslideCmd_A768(((char*)self), _cmd);
        break;
    case 5:
        cComputer_getSpinCmd_A768(((char*)self), _cmd);
        break;
    case 8:
        cComputer_getWipeoutCmd_A768(((char*)self), _cmd);
        break;
    case 1:
        cComputer_getBoardPressCmd_A768(((char*)self), _cmd);
        break;
    case 3: case 9: case 10: case 11: case 12: case 13:
        break;
    }
    if (state != 5) {
        *(int*)&self->mGrabEndTime = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_writeToReplayFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cRiderHandler_writeToReplayFrame_A898(char*, void*) __asm__("cRiderHandler_writeToReplayFrame");

struct C_A898 {
    virtual void m0(char*, int);
};

/**
 * Writes the AI rider into a replay frame: the rider handler state, then the computer state.
 *
 * Calls cRiderHandler_writeToReplayFrame, then the stream's first virtual (write) with the cComputerState block at
 * +0xDF0 (336 bytes, up to +0xF40: it includes mCruiseLogicID at +0xF38 but not the cruise-logic pointer at +0xF44,
 * which cComputer_readFromReplayFrame rebuilds).
 *
 * @param computer  the AI rider (this)
 * @param data      the replay frame stream written to (On Tour cReplayFramePtr* data)
 */
extern "C" void cComputer_writeToReplayFrame(cComputer* computer, C_A898 *data) {
    cRiderHandler_writeToReplayFrame_A898(((char*)computer), data);
    data->m0((char*)&computer->mDesiredSpeed, 336);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_readFromReplayFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cObj_A8E8;
typedef void (cObj_A8E8::*Fn_A8E8)();
extern "C" Fn_A8E8 D_0043CF20_A8E8[4] __asm__("D_0043CF20");
extern "C" void cRiderHandler_readFromReplayFrame(void* self, void* src);

class cStream_A8E8 {
public:
    virtual void v1();
    virtual void read(void* dst, int size);
};

/**
 * Reads the AI rider back from a replay frame: the rider handler state, then the computer state.
 *
 * Calls cRiderHandler_readFromReplayFrame, then the stream's second virtual (read) into the cComputerState block at
 * +0xDF0 (336 bytes), and rebuilds the cruise-logic member-function pointer (+0xF44) from D_0043CF20[mCruiseLogicID]
 * (the short at +0xF38), as cComputer_setCruiseLogic does, since the saved pointer is not portable between runs.
 *
 * @param data  the replay frame stream read from (On Tour cReplayFramePtr* data)
 */
extern "C" void cComputer_readFromReplayFrame(cComputer* self, cStream_A8E8* data)
{
    cRiderHandler_readFromReplayFrame(((char*)self), data);
    data->read(&self->mDesiredSpeed, 336);
    short state = self->mCruiseLogicID;
    *(Fn_A8E8*)&self->cruiseLogic = D_0043CF20_A8E8[state];
    self->mCruiseLogicID = state;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getCruiseCmd);
#ifdef SKIP_ASM
struct sFlags_A960 {
    unsigned int pad0 : 12;
    unsigned int b12 : 1;
    unsigned int pad13 : 5;
    unsigned int b18 : 1;
    unsigned int b19 : 1;
};

class cObj_A960;
typedef void (cObj_A960::*Fn_A960)(sFlags_A960*);

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
 * both. Then it calls the current cruise logic, the pointer-to-member at +0xF44 (set from D_0043CF20), with the
 * command. cComputer_getControlCmd calls it.
 *
 * @param cruiseCmd  the cruise command
 */
extern "C" void cComputer_getCruiseCmd(cObj_A960* self, sFlags_A960* cruiseCmd)
{
    int punchRight;
    cruiseCmd->b12 = cComputer_checkForReset(self);
    self->fE20 = 0;
    if (cComputer_checkForPunching(self, &punchRight)) {
        if (punchRight)
            cruiseCmd->b19 = 1;
        else
            cruiseCmd->b18 = 1;
    } else if (cComputer_checkForBlock(self)) {
        cruiseCmd->b19 = 1;
        cruiseCmd->b18 = 1;
    }
    (self->*(self->state))(cruiseCmd);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getPrewindCmd);
#ifdef SKIP_ASM
struct sV4_AA70
{
    float x, y, z, w;
} __attribute__((aligned(16)));

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

struct sPad_AA70 {
    unsigned int pad0 : 13;
    unsigned int b13 : 1;
    unsigned int b14 : 1;
    int b15 : 6;
    int b21 : 6;
    unsigned int pad27 : 5;
    int w0 : 6;
};

extern "C" sV4_AA70 cPath_getPathPos_AA70(cTrack_AA70*, int, float) __asm__("cPath_getPathPos");
extern "C" float cComputer_calcTurn_AA70(void*, sV4_AA70*) __asm__("cComputer_calcTurn");

// PORT: PS2-only VU0 inline asm (vector length via vsqrt).
static inline float Len_AA70(const sV4_AA70& v)
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
static inline sV4_AA70 Sub_AA70(const sV4_AA70& a, const sV4_AA70& b)
{
    sV4_AA70 r;
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
static inline sV4_AA70 Scale_AA70(const sV4_AA70& v, float s)
{
    sV4_AA70 r;
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
 * Sets bit 14 when 5 km/h under mDesiredSpeed. For a pipe jump (mToPipe) it sets bit 13 until the rider is
 * within 50 of the jump's length from mStartJumpPos, and returns. Otherwise a jump event within 50 ahead sets
 * bit 13 and steers (low field of word 1) at the jump's end point minus half of the rider's +0x490 point
 * minus its position. The fields at bits 15-20 and 21-26 get mSpinAxis and mFlipAxis.
 *
 * @param cmd  the prewind command (On Tour tPrewindCmd)
 */
extern "C" void cComputer_getPrewindCmd(cComputer* self, sPad_AA70* cmd)
{
    sRec_AA70 buf[12];
    cRider* rider = self->mRider;
    cTrack_AA70* trk = *(cTrack_AA70**)(((char*)rider) + 0xAB8);
    int n = trk->Query(buf, 12, *(float*)(((char*)rider) + 0x4C0), *(float*)(((char*)rider) + 0x4C4) + 50.0f);
    int sel = -1;
    if (Len_AA70(*(sV4_AA70*)&self->mRider->mVelocity) < self->mDesiredSpeed - 138.88890075683594f)
        cmd->b14 = 1;
    if (self->mToPipe) {
        sV4_AA70 jumpSpan = Sub_AA70(*(sV4_AA70*)&self->mEndJumpPos, *(sV4_AA70*)&self->mStartJumpPos);
        sV4_AA70 fromJumpStart = Sub_AA70(*(sV4_AA70*)&self->mRider->mPosition, *(sV4_AA70*)&self->mStartJumpPos);
        if (Len_AA70(fromJumpStart) < Len_AA70(jumpSpan) - 50.0f)
            cmd->b13 = 1;
        return;
    }
    for (int i = 0; i < n; i++) {
        if (buf[i].type == 0x10) {
            sel = i;
            cmd->b13 = 1;
            break;
        }
    }
    float* ctl = (float*)(&self->mDesiredSpeed);
    if (sel != -1) {
        cRider* riderBytes = self->mRider;
        sV4_AA70 half = Sub_AA70(*(sV4_AA70*)(((char*)riderBytes) + 0x490), *(sV4_AA70*)&riderBytes->mPosition);
        half = Scale_AA70(half, 0.5f);
        sV4_AA70 pos = cPath_getPathPos_AA70(trk, 0, buf[sel].b);
        sV4_AA70 toTarget = Sub_AA70(pos, half);
        cmd->w0 = (int)(cComputer_calcTurn_AA70(((char*)self), &toTarget) * 31.0f);
    }
    cmd->b15 = (int)(ctl[18] * 31.0f);
    cmd->b21 = (int)(ctl[19] * 31.0f);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cComputer_getBoardPressCmd);
#ifdef SKIP_ASM
struct sVec4_AD78 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_AD78 {
    unsigned int pad0 : 18;
    int b18 : 6;
    int b24 : 6;
    unsigned int pad30 : 2;
    int w0 : 6;
};

extern "C" int cComputer_checkForBoardPress(cComputer*, int*);
extern "C" void cComputer_calculateTargetPos(sVec4_AD78*, void*);
extern "C" float cComputer_calcTurn(void*, sVec4_AD78*);

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
extern "C" void cComputer_getBoardPressCmd(cComputer* self, sPad_AD78* cmd)
{
    int flag;
    int pressMarkerFound = cComputer_checkForBoardPress(self, &flag);
    float pressDir = 1.0f;
    if (flag)
        pressDir = -1.0f;
    {
        sVec4_AD78 targetPos;
        cComputer_calculateTargetPos(&targetPos, ((char*)self));
        cmd->b18 = (int)(cComputer_calcTurn(((char*)self), &targetPos) * 31.0f);
    }
    if (pressMarkerFound) {
        cmd->w0 = (int)(pressDir * 31.0f);
        sVec4_AD78 sideAxis = *(sVec4_AD78*)&self->mRider->mMatrix;
        float z = sideAxis.z + sideAxis.z;
        float y = clamp_AD78(z, -1.0f, 1.0f);
        if (vAbs_AD78(y) > 0.1f)
            cmd->b24 = (int)(y * 31.0f);
    } else {
        cmd->w0 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getRailslideCmd);
#ifdef SKIP_ASM
struct sVec4_AED8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_AED8 {
    unsigned int pad0 : 25;
    int b25 : 6;
    unsigned int pad31 : 1;
    int w0 : 6;
};

extern "C" int cComputer_checkForJumpEvent(cComputer* self, float* speed, int* below, int* o3, int* o4, int* o5, int* o6);
extern "C" void cComputer_setupPrewindState_AED8(char*, int, int, int, int) __asm__("cComputer_setupPrewindState");
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" int cComputer_checkForSpeedZone(cComputer*, float*, int*);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_AED8(const sVec4_AED8& v)
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
 * Copies the jump event's toRail flag into mToRail (uninitialised stack data when there is no event) and
 * always sets bits 17-24. At a jump the rider has reached it sets bits 13-14 and calls setupPrewindState().
 * Otherwise the field at bits 25-30 gets twice the side vector's z (clamped, when over 0.1), the low field of
 * word 1 gets +31 for a negative mSpinAxis and -31 otherwise (not for RailslideMode 3 or 4, or channel-2
 * animation class 14), and bit 16 is set in a reached speed zone (cComputer_checkForSpeedZone) when 5 km/h too slow.
 *
 * @param cmd  the railslide command (On Tour tRailslideCmd)
 */
extern "C" void cComputer_getRailslideCmd(cComputer* self, sPad_AED8* cmd)
{
    int below, toPipe, toRail, isSpin, isFlip, ok;
    int jumpEvent = cComputer_checkForJumpEvent(self, (float*)(&self->mDesiredSpeed), &below, &toPipe, &toRail, &isSpin, &isFlip);
    self->mToRail = toRail;
    *(unsigned int*)cmd |= 0x1FE0000;
    if (jumpEvent && below) {
        *(unsigned int*)cmd |= 0x6000;
        cComputer_setupPrewindState_AED8(((char*)self), isSpin, isFlip, toRail, toPipe);
        return;
    }
    sVec4_AED8 sideAxis = *(sVec4_AED8*)&self->mRider->mMatrix;
    float z = sideAxis.z + sideAxis.z;
    float y = clamp_AED8(z, -1.0f, 1.0f);
    if (vAbs_AED8(y) > 0.1f)
        cmd->b25 = (int)(y * 31.0f);
    cRider* rider = self->mRider;
    if ((unsigned int)(rider->RailslideMode - 3) >= 2 &&
        cRiderAnimBase_getCurrentAnimCategory(rider->mRiderAnim, 2) != 0xE) {
        float spinSign = (self->mSpinAxis < 0.0f) ? 1.0f : -1.0f;
        cmd->w0 = (int)(spinSign * 31.0f);
    }
    int speedMarkerFound = cComputer_checkForSpeedZone(self, (float*)(&self->mDesiredSpeed), &ok);
    float len = vLen_AED8(*(sVec4_AED8*)&self->mRider->mVelocity);
    if (speedMarkerFound && ok && len < self->mDesiredSpeed - 138.88890075683594f)
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
extern "C" void cComputer_setupEndSpin(cComputer* self, float airTime)
{
    float spinCur, spinNext, flipCur, flipNext;
    cSpinControl_predictSpinSec((char*)self->mRider->mRiderHandler + 0x230, &spinCur, &spinNext);
    cSpinControl_predictFlipSec((char*)self->mRider->mRiderHandler + 0x230, &flipCur, &flipNext);
    int stop = 0;
    if (self->mToRail && airTime < 1.0f)
        stop = 1;
    if (stop) {
        self->mFlipAxis = 0.0f;
        self->mSpinAxis = 0.0f;
        return;
    }
    if (!self->mIsSpinTrick)
        return;
    if (self->mSpinAxis != 0.0f) {
        float lim = airTime + 0.1f;
        if (lim < spinCur)
            self->mSpinAxis = 0.0f;
        else if (lim < spinNext && spinCur - 0.4f < 0.0f)
            self->mSpinAxis = 0.0f;
    }
    if (self->mFlipAxis != 0.0f) {
        float lim = airTime + 0.1f;
        if (lim < flipCur) {
            self->mFlipAxis = 0.0f;
            self->mSpinAxis = 0.0f;
        } else if (lim < flipNext && flipCur - 0.4f < 0.0f) {
            self->mFlipAxis = 0.0f;
            self->mSpinAxis = 0.0f;
        }
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getSpinCmd);
#ifdef SKIP_ASM
struct sPad_B250 {
    unsigned int pad0 : 12;
    unsigned int b12 : 1;
    unsigned int pad13 : 1;
    unsigned int b14 : 1;
    unsigned int pad15 : 1;
    unsigned int b16 : 8;
    int b24 : 6;
    unsigned int pad30 : 2;
    int w0 : 6;
    unsigned int pad6 : 6;
    int w12 : 6;
};

// Pad stick setter (an inline accessor: the value is expanded before the field mask).
static inline void SetW12_B250(sPad_B250* pad, int v)
{
    pad->w12 = v;
}

int AIrand();
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void cComputer_setupEndSpin(cComputer*, float);
extern "C" int cComputer_chooseGrabMain(char*, float);

/**
 * Builds the in-air trick command: spin and flip axes, the grab to hold, and when the grab starts and ends.
 *
 * Takes the air time left from the air predictor (100 when it has no landing) and counts frames with over 15
 * s of air at rider +0x484; at 31 it sets bit 12 instead (probably a reset). A rider not set to mess up runs
 * setupEndSpin(), picks a grab with chooseGrabMain() when there is time and none is held, and drops it near
 * the landing (mHoldToLandTime + 0.3 s), 2 s into a non-uber grab or before a rail; a messing-up rider picks
 * a random grab once it is not flipping. The command gets mSpinAxis, mFlipAxis, mGrabID (bits 16-23), the
 * tweak/uber bit 14 and, toward a rail, -31 at word 1 bits 12-17.
 *
 * @param cmd  the spin command (On Tour tSpinCmd)
 * @return -1 when no grab is chosen; otherwise no value is set (On Tour's getSpinCmd returns nothing)
 */
// Returns -1 only when no trick is queued, and has no value otherwise (the caller ignores it): the retail
// tail keeps `lw E0C; li v0,-1` with no branch. The noop `v0 = -1` return arm survives reload as a USE,
// so jump2 only deletes the branch after sched2 and the compare loads stay.
extern "C" int cComputer_getSpinCmd(cComputer* self, sPad_B250* cmd)
{
    char* obj = (char*)self->mRider->mAirPredictor;
    int simState = *(int*)(obj + 0xAC);
    int ok = 0;
    if (simState == 1 || simState == 3)
        ok = 1;
    float gap;
    if (ok) {
        gap = *(float*)(obj + 0x98) - *(float*)(obj + 0xA0);
        if (gap > 15.0f)
            (*(int*)((char*)self->mRider + 0x484))++;
        else
            *(int*)((char*)self->mRider + 0x484) = 0;
    } else {
        gap = 100.0f;
        *(int*)((char*)self->mRider + 0x484) = 0;
    }
    if (*(int*)((char*)self->mRider + 0x484) >= 31) {
        *(int*)((char*)self->mRider + 0x484) = 0;
        cmd->b12 = 1;
    } else {
        if (self->mIsGonnaMessUp == 0) {
            cComputer_setupEndSpin(self, gap);
            if (self->mGrabEndTime <= 0.0f)
                self->mGrabEndTime = 15.0f;
            if (self->mIsNaturalAir) {
                self->mGrabEndTime = gap;
                self->mIsNaturalAir = 0;
            } else if (self->mGrabID == -1 && gap > 0.5f && gap < self->mGrabEndTime &&
                       cRiderAnimBase_getCurrentAnimCategory(self->mRider->mRiderAnim, 2) != 20) {
                self->mGrabID = cComputer_chooseGrabMain(((char*)self), gap);
            } else {
                int stop = 0;
                if (self->mToRail && gap < 1.0f)
                    stop = 1;
                if (stop) {
                    self->mIsTweak = 0;
                    self->mGrabID = -1;
                } else if (gap <= self->mHoldToLandTime + 0.30000001192092896f ||
                           (self->mGrabStartTime - gap > 2.0f && self->mIsUber == 0)) {
                    self->mIsTweak = 0;
                    self->mGrabID = -1;
                    self->mIsUber = 0;
                }
            }
        } else if (self->mFlipAxis == 0.0f && self->mGrabID == -1) {
            self->mGrabID = (unsigned int)AIrand() % 15;
            if (AIrand() & 1)
                self->mIsTweak = 1;
        }
        float* ctl = (float*)(&self->mDesiredSpeed);
        cmd->b24 = (int)(ctl[18] * 31.0f);
        cmd->w0 = (int)(ctl[19] * 31.0f);
        cmd->b16 = self->mGrabID;
        if (self->mIsUber)
            cmd->b14 = 1;
        else if (self->mIsTweak)
            cmd->b14 = 1;
        else
            cmd->b14 = 0;
        if (self->mToRail == 1)
            SetW12_B250(cmd, -0x1F);
        if (self->mGrabID == -1)
            return -1;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("seg/seg_1218", cComputer_getNaturalAirCmd);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec4_B590 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPad_B590 {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
};

struct sPadBits_B590 {
    unsigned int pad0 : 24;
    int b24 : 6;
};

int AIrand();
extern "C" void cComputer_calculateTargetPos_B590(sVec4_B590*, void*) __asm__("cComputer_calculateTargetPos");
extern "C" float cComputer_calcTurn_B590(void*, sVec4_B590*) __asm__("cComputer_calcTurn");

/**
 * Builds the command for natural (unplanned) air: random spins and flips sized to the air time.
 *
 * Takes the air time from the air predictor (0 without a landing), sets mIsNaturalAir and clears
 * mIsGonnaMessUp, mToRail and the grab. Over 1 s it spins in a random direction and over 3 s it also flips
 * (either sets mIsSpinTrick). Under 0.5 s with neither it steers at the path target (bits 24-29, negated
 * calcTurn). Byte 2 gets mGrabID, which is always -1 here.
 *
 * @param cmd  the natural-air command (On Tour tNaturalAirCmd)
 */
extern "C" void cComputer_getNaturalAirCmd(cComputer* self, sPad_B590* cmd)
{
    char* airPredictor = (char*)self->mRider->mAirPredictor;
    int st = *(int*)(airPredictor + 0xAC);
    int ok = 0;
    if (st == 1 || st == 3)
        ok = 1;
    float airTimeLeft;
    if (ok)
        airTimeLeft = *(float*)(airPredictor + 0x98) - *(float*)(airPredictor + 0xA0);
    else
        airTimeLeft = 0.0f;
    self->mIsSpinTrick = 0;
    self->mIsGonnaMessUp = 0;
    self->mIsNaturalAir = 1;
    self->mToRail = 0;
    self->mGrabID = -1;
    if (airTimeLeft > 1.0f) {
        self->mIsSpinTrick = 1;
        self->mSpinAxis = 1.0f;
        if (AIrand() & 1)
            self->mSpinAxis = -self->mSpinAxis;
    } else {
        self->mSpinAxis = 0.0f;
    }
    if (airTimeLeft > 3.0f) {
        self->mIsSpinTrick = 1;
        self->mFlipAxis = 1.0f;
        if (AIrand() & 1)
            self->mFlipAxis = -self->mFlipAxis;
    } else {
        self->mFlipAxis = 0.0f;
    }
    if (airTimeLeft < 0.5f && self->mFlipAxis == 0.0f && self->mSpinAxis == 0.0f) {
        sVec4_B590 targetPos;
        cComputer_calculateTargetPos_B590(&targetPos, ((char*)self));
        ((sPadBits_B590*)cmd)->b24 = (int)(-cComputer_calcTurn_B590(((char*)self), &targetPos) * 31.0f);
        self->mGrabID = -1;
    }
    cmd->b2 = self->mGrabID;
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
 * @param computer  the computer rider
 * @param controlCmd  the command word
 */
extern "C" void cComputer_getWipeoutCmd(cComputer* computer, int *controlCmd) {
    *controlCmd = (*controlCmd & ~0x1000) | ((cComputer_checkForReset(computer) & 1) << 0xC);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_getGateCmd);
#ifdef SKIP_ASM
struct sPad_B790 {
    unsigned int pad0 : 12;
    int b12 : 6;
};

class cLvl_B790 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual int Level();
};

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
extern "C" void cComputer_getGateCmd(cComputer* self, sPad_B790* cmd)
{
    float framesLeft = 180.0f - (float)AITick();
    float th = 33.0f;
    th -= (float)((((cLvl_B790*)(&self->mRider->mVtbl))->Level() - 1) * 10);
    if (framesLeft < th && ((cLvl_B790*)(&self->mRider->mVtbl))->Level() < 4) {
        cmd->b12 = 0x1F;
        return;
    }
    if (framesLeft < 72.0f && ((cLvl_B790*)(&self->mRider->mVtbl))->Level() < 3) {
        cmd->b12 = -0x1F;
        return;
    }
    if (AITick() % 20 == 0) {
        unsigned int randVal = AIrand();
        float* ctlSpeed = (float*)(&self->mDesiredSpeed);
        *ctlSpeed = (float)randVal * 0.009999999776482582f;
        if ((AIrand() & 1) == 0)
            self->mDesiredSpeed = -self->mDesiredSpeed;
    }
    cmd->b12 = (int)(self->mDesiredSpeed * 31.0f);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_checkForJumpEvent);
#ifdef SKIP_ASM
struct sVec4_B980 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRec_B980 {
    int type;
    int flags;
    float a;
    float b;
};

class cTrack_B980 {
public:
    char pad[0x34];
    virtual int Query(sRec_B980* buf, int max, float x, float y);
};

extern "C" sVec4_B980 cPath_getPathPos(cTrack_B980*, int, float);

/**
 * Looks for a jump event on the rider's path just ahead and returns its settings and end points.
 *
 * Queries the path at +0xAB8 for events from the rider's previous path distance (+0x4C0) to 300 past the
 * current one (+0x4C4) and takes the first of type 0x10. Its flag word gives the approach speed (bits 4 up,
 * km/h, returned in cm/s) and the pipe, rail, spin and flip bits (3 down to 0); mStartJumpPos and mEndJumpPos
 * are set from the event's two path distances. The outputs are untouched when there is no event.
 *
 * @param approachSpeed  receives the speed to approach the jump at, in cm/s
 * @param below  receives 1 when the rider has reached the jump's start (On Tour isPrewind)
 * @param toPipe  receives the jump-into-pipe flag
 * @param toRail  receives the jump-onto-rail flag
 * @param isSpin  receives the spin flag
 * @param isFlip  receives the flip flag
 * @return 1 when a jump event was found, otherwise 0
 */
extern "C" int cComputer_checkForJumpEvent(cComputer* self, float* approachSpeed, int* below, int* toPipe, int* toRail, int* isSpin, int* isFlip)
{
    sRec_B980 buf[12];
    cRider* rider = self->mRider;
    cTrack_B980* trk = *(cTrack_B980**)(((char*)rider) + 0xAB8);
    int n = trk->Query(buf, 12, *(float*)(((char*)rider) + 0x4C0), *(float*)(((char*)rider) + 0x4C4) + 300.0f);
    for (int i = 0; i < n; i++) {
        if (buf[i].type == 0x10) {
            int fl = buf[i].flags;
            *approachSpeed = (float)(fl >> 4) * 27.777780532836914f;
            *toPipe = (fl >> 3) & 1;
            *toRail = (fl >> 2) & 1;
            *isSpin = (fl >> 1) & 1;
            *isFlip = fl & 1;
            *below = (buf[i].a <= *(float*)((char*)self->mRider + 0x4C4)) ? 1 : 0;
            *(sVec4_B980*)&self->mStartJumpPos = cPath_getPathPos(trk, 0, buf[i].a);
            *(sVec4_B980*)&self->mEndJumpPos = cPath_getPathPos(trk, 0, buf[i].b);
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_checkForSpeedZone);
#ifdef SKIP_ASM
struct sHit_BB18 { int type; int val; float lim; int d; };
/**
 * Looks for a speed marker on the computer rider's path just ahead.
 *
 * Asks the rider's path object (+0xAB8, virtual at +0xC of its +0x34 table) for up to 12 markers between the rider's
 * path distance (+0x4C0) and +0x4C4 plus 600. For the first marker of type 17 it stores the marker's value times
 * 27.77778 in @p out and in @p ok whether the marker's limit is at most +0x4C4, and returns 1.
 *
 * @param self_  the computer rider
 * @param out  receives the speed
 * @param ok  receives 1 when the rider is inside the zone
 * @return 1 when a speed marker was found, else 0
 */
extern "C" int cComputer_checkForSpeedZone(cComputer* self_, float* out, int* ok) {
    sHit_BB18 buf[12];
    cRider* rider = self_->mRider;
    char* track = *(char**)(((char*)rider) + 0xAB8);
    char* vt = *(char**)(track + 0x34);
    int n = (*(int (**)(char*, sHit_BB18*, int, float, float))(vt + 0xC))(track + *(short*)(vt + 8), buf, 12, *(float*)(((char*)rider) + 0x4C0), *(float*)(((char*)rider) + 0x4C4) + 600.0f);
    for (int i = 0; i < n; i++) {
        if (buf[i].type == 17) {
            *out = (float)buf[i].val * 27.777780532836914f;
            *ok = buf[i].lim <= *(float*)((char*)self_->mRider + 0x4C4);
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_checkForBoardPress);
#ifdef SKIP_ASM
struct sHit_BBF8 { int type; int val; int d[2]; };
/**
 * Looks for a board-press marker on the computer rider's path just ahead.
 *
 * Asks the rider's path object (+0xAB8) for up to 12 markers between the path distance (+0x4C0) and +0x4C4 plus 50. For
 * the first marker of type 19 it stores in @p out whether its value is positive and returns 1; @p out is 0 otherwise.
 *
 * @param self_  the computer rider
 * @param out  receives 1 for a positive marker value
 * @return 1 when a marker was found, else 0
 */
extern "C" int cComputer_checkForBoardPress(cComputer* self_, int* out) {
    sHit_BBF8 buf[12];
    cRider* rider = self_->mRider;
    char* track = *(char**)(((char*)rider) + 0xAB8);
    char* vt = *(char**)(track + 0x34);
    int n = (*(int (**)(char*, sHit_BBF8*, int, float, float))(vt + 0xC))(track + *(short*)(vt + 8), buf, 12, *(float*)(((char*)rider) + 0x4C0), *(float*)(((char*)rider) + 0x4C4) + 50.0f);
    *out = 0;
    for (int i = 0; i < n; i++) {
        if (buf[i].type == 19) {
            *out = buf[i].val > 0;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_checkForRail);
#ifdef SKIP_ASM
struct sHit_BC98 { int type; int d[3]; };
/**
 * Tells whether the computer rider's path has a marker of type 15 at its current position.
 *
 * Asks the rider's path object (+0xAB8) for up to 12 markers between the path distance (+0x4C0) and +0x4C4. Nothing in
 * the build references it.
 *
 * @return 1 when such a marker was found, else 0
 */
extern "C" int cComputer_checkForRail(cComputer* self) {
    sHit_BC98 buf[12];
    cRider* rider = self->mRider;
    char* track = *(char**)(((char*)rider) + 0xAB8);
    char* vt = *(char**)(track + 0x34);
    int n = (*(int (**)(char*, sHit_BC98*, int, float, float))(vt + 0xC))(track + *(short*)(vt + 8), buf, 12, *(float*)(((char*)rider) + 0x4C0), *(float*)(((char*)rider) + 0x4C4));
    for (int i = 0; i < n; i++) {
        if (buf[i].type == 0xF) return 1;
    }
    return 0;
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
 * @return 1 when the rider has a target, otherwise 0
 */
// On Tour: bool cComputer::checkForTargets(bool& isAttack); retail stores a word through isAttack.
extern "C" int cComputer_checkForTargets(cComputer* self, int* isAttack)
{
    const float kAIMaxTargetDist = 700.0f;
    const float kAIMaxPathWanderDist = 600.0f;
    if (self->mTargetID >= 0) {
        if (((sRiderT_BD10*)self->mRider)->targets[self->mTargetID].dist > kAIMaxTargetDist * 1.5f
            || ((sRiderT_BD10*)self->mRider)->pathDist > kAIMaxPathWanderDist)
            self->mTargetID = -1;
    }
    if (self->mTargetID < 0 && ((sRiderT_BD10*)self->mRider)->pathDist < kAIMaxPathWanderDist * 0.9f
        && AITick() % 12 == 0) {
        float bestScore = 10000000000.0f;
        float aScore;
        int shouldAttack;
        int i;
        for (i = 0; i < 6; i++) {
            if (((sRiderT_BD10*)self->mRider)->targets[i].active == 0)
                continue;
            aScore = ((sRiderT_BD10*)self->mRider)->targets[i].dist;
            if (aScore > kAIMaxTargetDist)
                continue;
            shouldAttack = cComputer_checkForAttack(((char*)self), i);
            float del = ((sRiderT_BD10*)self->mRider)->targets[i].angle - Atan2_BD10(Fwd_BD10((sRiderT_BD10*)self->mRider).y, Fwd_BD10((sRiderT_BD10*)self->mRider).x);
            float cosDelta = fCos(del - Floor_BD10(del * 0.15915493667125702f + 0.5f) * 6.2831854820251465f);
            if (shouldAttack)
                aScore *= Abs_BD10(cosDelta) + 1.0f;
            else
                aScore *= 1.0f - cosDelta;
            if (aScore <= bestScore) {
                self->mTargetID = i;
                bestScore = aScore;
                *isAttack = shouldAttack;
            }
        }
    }
    return self->mTargetID >= 0;
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
extern "C" void cComputer_setupPrewindState(cComputer* self, int isSpin, int isFlip, int toRail, int toPipe)
{
    self->mGrabID = -1;
    int* trickFlags = &self->mIsSpinTrick;
    *trickFlags = (isSpin || isFlip);
    self->mIsGonnaMessUp = cComputer_shouldRiderWipeout(self);
    self->mToRail = toRail;
    self->mToPipe = toPipe;
    self->mIsNaturalAir = 0;
    if (self->mIsSpinTrick) {
        self->mSpinAxis = isSpin ? 1.0f : 0.0f;
        self->mFlipAxis = isFlip ? 1.0f : 0.0f;
        if (AIrand() & 1)
            self->mSpinAxis = -self->mSpinAxis;
        if (AIrand() & 1)
            self->mFlipAxis = -self->mFlipAxis;
    }
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_setupCrouch);
#ifdef SKIP_ASM
struct sVec4_C0A8 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sPad_C0A8 {
    unsigned int pad0 : 16;
    unsigned int b16 : 1;
    unsigned int b17 : 1;
    unsigned int pad18 : 8;
    int w0 : 6;
};
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_C0A8(const sVec4_C0A8& v)
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
 * Sets the cruise command's crouch field (bits 26-31) unless the rider is weak (mRiderDifficulty under 20).
 *
 * The field gets 31 below 833.3 cm/s (30 km/h) and 0.97 * 31 above it, or 0 for a weak rider.
 *
 * @param cmd  the cruise command (On Tour tCruiseCmd)
 */
extern "C" void cComputer_setupCrouch(cComputer* self, void* cmd)
{
    sPad_C0A8* crouchCmd = (sPad_C0A8*)cmd;
    float len = vLen_C0A8(*(sVec4_C0A8*)&self->mRider->mVelocity);
    float pressAmount = 0.97f;
    if (len < 833.3334350585938f)
        pressAmount = 1.0f;
    if (self->mRiderDifficulty < 20.0f)
        pressAmount = 0.0f;
    crouchCmd->w0 = (int)(pressAmount * 31.0f);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_setupRecklessCrouch);
#ifdef SKIP_ASM
struct sVec4_C140 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sPad_C140 {
    unsigned int pad0 : 16;
    unsigned int b16 : 1;
    unsigned int b17 : 1;
    unsigned int pad18 : 8;
    int w0 : 6;
};
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vLen_C140(const sVec4_C140& v)
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
 * Sets the cruise command's crouch field as setupCrouch() does but for every rider, plus bits 16 and 17.
 *
 * @param cmd  the cruise command (On Tour tCruiseCmd)
 */
extern "C" void cComputer_setupRecklessCrouch(cComputer* self, void* cmd)
{
    sPad_C140* crouchCmd = (sPad_C140*)cmd;
    float len = vLen_C140(*(sVec4_C140*)&self->mRider->mVelocity);
    float pressAmount = 0.97f;
    if (len < 833.3334350585938f)
        pressAmount = 1.0f;
    crouchCmd->w0 = (int)(pressAmount * 31.0f);
    crouchCmd->b16 = 1;
    crouchCmd->b17 = 1;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_shouldRiderWipeout);
#ifdef SKIP_ASM
/**
 * Rolls whether this computer rider will mess up its next trick, more often for a low mTrickDifficulty.
 *
 * @return 1 when AIrand() % 100 is at least mTrickDifficulty * 100 (a 1 - mTrickDifficulty chance), else 0
 */
extern "C" int cComputer_shouldRiderWipeout(cComputer* self) {
    unsigned int roll = AIrand();
    float thr = self->mTrickDifficulty * 100.0f;
    return thr <= (float)(roll % 100U);
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_shouldRiderUber);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_C258(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getLocationID(void* iface);
/**
 * Rolls whether this computer rider tries an uber trick; needs uber enabled and never at location 14.
 *
 * Requires mUberFlags bit 0 and a location ID other than 14; the chance is then mTrickDifficulty.
 *
 * @return 1 when the roll (AIrand() % 100 under mTrickDifficulty * 100) passes, otherwise 0
 */
extern "C" int cComputer_shouldRiderUber(cComputer* self) {
    if ((unsigned int)self->mRider->mUberFlags & 1) {
        if (cBENewRaceInterface_getLocationID(cBE_getInterface_C258(cBE_getBE(), 0)) != 0xE) {
            unsigned int roll = AIrand();
            float thr = self->mTrickDifficulty * 100.0f;
            return (float)(roll % 100U) < thr;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_shouldRiderTweak);
#ifdef SKIP_ASM
/**
 * Rolls whether this computer rider tweaks its grab: a 30 + 70 * mTrickDifficulty percent chance.
 *
 * @return 1 when AIrand() % 100 is under 30 + 70 * mTrickDifficulty, otherwise 0
 */
extern "C" int cComputer_shouldRiderTweak(cComputer* self) {
    unsigned int roll = AIrand();
    float thr = self->mTrickDifficulty * 70.0f + 30.0f;
    return (float)(roll % 100U) < thr;
}
#endif

//100%
INCLUDE_ASM("seg/seg_1218", cComputer_shouldRiderGrab);
#ifdef SKIP_ASM
/**
 * Rolls whether this computer rider grabs at all: a 60 + 40 * mTrickDifficulty percent chance.
 *
 * @return 1 when AIrand() % 100 is under 60 + 40 * mTrickDifficulty, otherwise 0
 */
extern "C" int cComputer_shouldRiderGrab(cComputer* self) {
    unsigned int roll = AIrand();
    float thr = self->mTrickDifficulty * 40.0f + 60.0f;
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
extern "C" int cComputer_updateRiderPathDifficulty(cComputer* self, int RiderID, int Heat)
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
