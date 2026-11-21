#include "common.h"
#include "ai/airpredictor.h"
#include "replay/frameptr.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("ai/airpredictor", cAirPredictor_reset);
#ifdef SKIP_ASM
struct sQuadR {
    int x[4];
} __attribute__((aligned(16)));

struct sPair8 {
    int x[2];
};

extern sQuadR t4vect_ZAxis;
extern sQuadR t4vect_Zero;
extern sPair8 D_004A5960[];

/**
 * Clears the air predictor's flight: no landing found yet (patch -1), zero times, zero vectors and default
 * landing data.
 */
extern "C" void cAirPredictor_reset(cAirPredictor* self)
{
    self->SimState = 0;
    *(int*)&self->CurAirTime = 0;
    *(int*)&self->TotalAirTime = 0;
    *(int*)&self->ApexTime = 0;
    *(int*)&self->PrevTickTime = 0;
    *(sQuadR*)&self->GuessLandNormal = t4vect_ZAxis;
    *(sQuadR*)&self->GuessLandVelocity = t4vect_Zero;
    self->GuessLandSurface = -1;
    self->GuessLandPatchRID = 0xFFFFFFFF;
    *(sPair8*)&self->GuessLandPatchUV[0] = D_004A5960[0];
}
#endif

//100%
INCLUDE_ASM("ai/airpredictor", cAirPredictor_simStep);
#ifdef SKIP_ASM
// FAKEMATCH: identical arms `else if (self) { state = 1; f20 = normal; f90 = surf; } else { same }` (cross-jumping merges them later; at
// GCSE time they raise the expression table from 73 to 77 buckets, which gives the &p1/&p0 PRE copies retail's s5/s6 order); plain form 99.95%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
struct sVec4_3200
{
    float x, y, z, w;
    sVec4_3200() {}
} __attribute__((aligned(16)));

struct sHit_3200 {
    sVec4_3200 pos;          // 0x00
    sVec4_3200 normal;       // 0x10
    char pad20[0x4C - 0x20];
    int surf;                // 0x4C
    char pad50[4];
    char* obj;               // 0x54
    char pad58[0x6C - 0x58];
    float u;                 // 0x6C
    float v;                 // 0x70
    char pad74[0x80 - 0x74];
};
struct sRay_3200 {
    char pad[0xB0];
};
struct sAirP_3200 {
    sVec4_3200 f00;          // 0x00
    sVec4_3200 f10;          // 0x10
    sVec4_3200 f20;          // 0x20
    unsigned int f30;        // 0x30
    float f34;               // 0x34
    float f38;               // 0x38
    char pad3C[4];
    sVec4_3200 f40;          // 0x40
    sVec4_3200 pos;          // 0x50
    sVec4_3200 vel;          // 0x60
    char pad70[0x90 - 0x70];
    int f90;                 // 0x90
    short f94;               // 0x94
    char pad96[2];
    float f98;               // 0x98
    float f9C;               // 0x9C
    char padA0[8];
    float fA8;               // 0xA8
    int state;               // 0xAC
};

extern sQuadR t4vect_ZAxis;
// cAirPredictor_simOneTick is defined later in the unit (with its own vector type).
float cAirPredictor_simOneTick_3200(void* self, sVec4_3200* pos, sVec4_3200* vel, sVec4_3200* outPos, sVec4_3200* outVel) __asm__("cAirPredictor_simOneTick");
// cWorldLine_cWorldLine (intersect/riderspheretree) builds the capsule and returns it.
extern "C" sRay_3200* cWorldLine_cWorldLine(sRay_3200* ray, sVec4_3200* a, sVec4_3200* b, int c, float d);
extern "C" float cBXNash_terrainIntersect1(void* world, sRay_3200* ray, sHit_3200* hit, int c);
extern "C" float cBXNash_staticObjectIntersect1(void* world, sRay_3200* ray, sHit_3200* hit, int c);

// PORT: PS2-only VU0 inline asm (normalize).
static inline sVec4_3200 vu0Normalize_3200(const sVec4_3200& v)
{
    sVec4_3200 r;
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
static inline float vu0Dot_3200(const sVec4_3200& a, const sVec4_3200& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec4_3200 vu0Sub_3200(const sVec4_3200& a, const sVec4_3200& b)
{
    sVec4_3200 r;
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
static inline sVec4_3200 vu0Add_3200(const sVec4_3200& a, const sVec4_3200& b)
{
    sVec4_3200 r;
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
static inline sVec4_3200 vu0Scale_3200(const sVec4_3200& v, float s)
{
    sVec4_3200 r;
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

// PORT: PS2-only VU0 inline asm (vector times-assign scalar).
static inline void vu0ScaleEq_3200(sVec4_3200& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector subtract-assign).
static inline void vu0SubEq_3200(sVec4_3200& a, const sVec4_3200& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b));
}

// PORT: PS2-only VU0 inline asm (vector add-assign).
static inline void vu0AddEq_3200(sVec4_3200& a, const sVec4_3200& b)
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
        : "m"(a), "m"(b));
}

/**
 * Runs the air predictor's simulation forward until the flight changes direction, then looks for the landing.
 *
 * Saves CurSimPos/CurSimVel (0x50/0x60), then repeatedly adds 1/60 to TotalAirTime (+0x98) and steps the sim one tick
 * (cAirPredictor_simOneTick), recording ApexTime/GuessApexPos (+0x9C/+0x40) while the height keeps rising; it stops when the new
 * velocity's direction differs from the start (speed * 0.99 > dot of the old unit velocity and the new velocity) or
 * after 270000 / TerminalVel (+0xA8) ticks. It then casts a capsule (cWorldLine_cWorldLine) along the travelled segment,
 * lengthened by 2% at each end, against the world (cBXNash_terrainIntersect1 in SimState 0, cBXNash_staticObjectIntersect1 otherwise). A hit facing
 * the motion sets GuessLandPosition, rewinds TotalAirTime by the unused part of the step, blends GuessLandVelocity from
 * the old and new velocities, and sets SimState 1 with the hit's normal and surface (or 3 with the Z axis and surface
 * 13 when SimState was 2); the landing patch's flags, resource id and UV come from the hit object when SimState is 1,
 * else -1. The duplicated "else if (self)" branch is a marked fakematch steering, not game logic.
 *
 * @param vself  the cAirPredictor (this)
 */
// cAirPredictor::simStep (On Tour names; the loop is a do-while in SSX 3).
extern "C" void cAirPredictor_simStep(cAirPredictor* vself)
{
    sAirP_3200* self = (sAirP_3200*)vself;
    sVec4_3200 prevPos = self->pos;
    sVec4_3200 prevVel = self->vel;
    sVec4_3200 uPrevVel = vu0Normalize_3200(self->vel);
    float numSubStep = 0.0f;
    float maxResampleSteps = 270000.0f / self->fA8;
    float speed;
    do {
        self->f98 += 0.01666666753590107f;
        numSubStep += 1.0f;
        float prevZ = self->pos.z;
        speed = cAirPredictor_simOneTick_3200(self, &self->pos, &self->vel, &self->pos, &self->vel);
        if (prevZ <= self->pos.z) {
            self->f9C = self->f98;
            self->f40 = self->pos;
        }
        if (maxResampleSteps < numSubStep) break;
    } while (!(speed * 0.9900000095367432f > vu0Dot_3200(uPrevVel, self->vel)));
    sVec4_3200 segStart = prevPos;
    sVec4_3200 segEnd = self->pos;
    sVec4_3200 ray = vu0Sub_3200(segEnd, segStart);
    vu0ScaleEq_3200(ray, 0.019999999552965164f);
    vu0SubEq_3200(segStart, ray);
    vu0AddEq_3200(segEnd, ray);
    sHit_3200 ti;
    sRay_3200 w1;
    cWorldLine_cWorldLine(&w1, &segEnd, &segStart, 1, 1.0f);
    float hitParam;
    if (self->state == 0) {
        hitParam = cBXNash_terrainIntersect1(SSXApp->Game->BXNash, &w1, &ti, 0);
    } else {
        hitParam = cBXNash_staticObjectIntersect1(SSXApp->Game->BXNash, &w1, &ti, 0);
    }
    if (hitParam >= 0.0f && vu0Dot_3200(ray, ti.normal) < 0.0f) {
        self->f00 = ti.pos;
        self->f98 = self->f98 - numSubStep * (hitParam * 0.01666666753590107f);
        self->f10 = vu0Add_3200(vu0Scale_3200(prevVel, hitParam), vu0Scale_3200(self->vel, 1.0f - hitParam));
        if (self->state == 2) {
            self->state = 3;
            *(sQuadR*)&self->f20 = t4vect_ZAxis;
            self->f90 = 13;
        } else if (self) { // FAKEMATCH: identical arms, see header
            self->state = 1;
            self->f20 = ti.normal;
            self->f90 = ti.surf;
        } else {
            self->state = 1;
            self->f20 = ti.normal;
            self->f90 = ti.surf;
        }
        char* hitObj = ti.obj;
        if (hitObj != 0 && self->state == 1) {
            self->f94 = *(short*)(hitObj + 0xA);
            self->f30 = *(int*)(hitObj + 0x150);
            self->f34 = ti.u;
            self->f38 = ti.v;
        } else {
            self->f94 = -1;
            self->f30 = 0xFFFFFFFF;
        }
    }
}
#endif

//100%
INCLUDE_ASM("ai/airpredictor", cAirPredictor_startLaunchIntoAir);
#ifdef SKIP_ASM
struct sQuad;
void cAirPredictor_initLaunch(char* self, sQuad* a, sQuad* b);

/**
 * Starts predicting a jump: resets the predictor, sets the terminal velocity and seeds the flight with the
 * launch state.
 *
 * @param launchPos  launch position (On Tour const t4Vector&)
 * @param launchVel  launch velocity (On Tour const t4Vector&)
 * @param terminalVel  maximum falling speed
 */
extern "C" void cAirPredictor_startLaunchIntoAir(cAirPredictor* self, sQuad* launchPos, sQuad* launchVel, float terminalVel)
{
    cAirPredictor_reset(self);
    self->TerminalVel = terminalVel;
    cAirPredictor_initLaunch(((char*)self), launchPos, launchVel);
}
#endif

// 16-byte aligned: the original copies these with lq/sq, which require it
// (PS2 VU-style quadword).
struct sQuad {
    int x[4];
} __attribute__((aligned(16)));

//100%
INCLUDE_ASM("ai/airpredictor", cAirPredictor_initLaunch__FPcP5sQuadT1);
#ifdef SKIP_ASM
/**
 * Seeds the predicted flight with a launch position and velocity, as both the start and the current
 * simulation state.
 *
 * @param launchPos  launch position (On Tour const t4Vector&)
 * @param launchVel  launch velocity (On Tour const t4Vector&)
 */
void cAirPredictor_initLaunch(char* self, sQuad* launchPos, sQuad* launchVel)
{
    *(int*)(self + 0xAC) = 0;
    *(sQuad*)(self + 0x70) = *launchPos;
    *(sQuad*)(self + 0x80) = *launchVel;
    *(sQuad*)(self + 0x50) = *launchPos;
    *(sQuad*)(self + 0x60) = *launchVel;
    *(float*)(self + 0x98) = *(float*)(self + 0xA0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/airpredictor", cAirPredictor_updateFlightPath);
#ifdef SKIP_ASM
extern sQuadR t4vect_Origin;
extern sQuadR t4vect_XAxis;
extern sQuadR t4vect_ZAxis;
extern "C" void cAirPredictor_simStep(cAirPredictor* self);

struct sVec4_3648
{
    float x, y, z, w;
    sVec4_3648() {}
} __attribute__((aligned(16)));

// cAirPredictor_simOneTick is defined later in the unit (with its own vector type).
float cAirPredictor_simOneTick_3648(char* self, sVec4_3648* pos, sVec4_3648* vel, sVec4_3648* outPos, sVec4_3648* outVel) __asm__("cAirPredictor_simOneTick");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_3648 vu0Scale_3648(const sVec4_3648& v, float s)
{
    sVec4_3648 r;
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
static inline sVec4_3648 vu0Add_3648(const sVec4_3648& a, const sVec4_3648& b)
{
    sVec4_3648 r;
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
 * Advances the predicted flight by @p dt and returns the rider's position and velocity along it.
 *
 * Past 60 s of simulated time (+0x98) it gives up: in state 2 (+0xAC) it switches to state 3 with a fixed frame
 * (t4vect_Origin, t4vect_XAxis, t4vect_ZAxis) and +0x90 = 13, otherwise it re-launches from @p airPos / @p airVel
 * (cAirPredictor_initLaunch) in state 2. Otherwise, in state 1 a sim more than 0.2 s behind the clock (+0xA0) is
 * re-launched, and in states 0 and 2 cAirPredictor_simStep (On Tour simStep) runs until the sim passes the clock + dt or the
 * state changes.
 *
 * Then the air position/velocity (+0x70/+0x80) are stepped in fixed 1/60 s ticks with cAirPredictor_simOneTick (On Tour
 * simOneTick), +0xA4 counting the time done. A remainder over 0.01 s is blended linearly between this tick and a
 * predicted next one (position w = 1), else the current values are copied out. The clock +0xA0 then advances by @p dt.
 *
 * @param airPos  receives the predicted air position (On Tour airPos); also the launch position on a re-launch
 * @param airVel  receives the predicted air velocity (On Tour airVel); also the launch velocity on a re-launch
 * @param dt  seconds to advance
 */
extern "C" void cAirPredictor_updateFlightPath(cAirPredictor* self, sQuad* airPos, sQuad* airVel, float dt)
{
    if (self->TotalAirTime > 60.0f) {
        if (self->SimState == 2) {
            self->SimState = 3;
            *(sQuadR*)&self->GuessLandPosition = t4vect_Origin;
            *(sQuadR*)&self->GuessLandVelocity = t4vect_XAxis;
            *(sQuadR*)&self->GuessLandNormal = t4vect_ZAxis;
            self->GuessLandSurface = 13;
        } else {
            cAirPredictor_initLaunch(((char*)self), airPos, airVel);
            self->SimState = 2;
        }
    } else {
        if (self->SimState == 1 && self->TotalAirTime - self->CurAirTime < -0.2f) {
            cAirPredictor_initLaunch(((char*)self), airPos, airVel);
        }
        int st = self->SimState;
        if (st == 0) {
            cAirPredictor_simStep(self);
            while (self->SimState == 0 && self->TotalAirTime < self->CurAirTime + dt) {
                cAirPredictor_simStep(self);
            }
        } else if (st == 2) {
            cAirPredictor_simStep(self);
            while (self->SimState == 2 && self->TotalAirTime < self->CurAirTime + dt) {
                cAirPredictor_simStep(self);
            }
        }
    }
    float sinceLastTick = self->CurAirTime - self->PrevTickTime + dt;
    while (sinceLastTick >= 0.01666666753590107f) {
        sinceLastTick -= 0.01666666753590107f;
        cAirPredictor_simOneTick_3648(((char*)self), (sVec4_3648*)(&self->PrevTickPos), (sVec4_3648*)(&self->PrevTickVel), (sVec4_3648*)(&self->PrevTickPos), (sVec4_3648*)(&self->PrevTickVel));
        self->PrevTickTime += 0.01666666753590107f;
    }
    if (sinceLastTick > 0.009999999776482582f) {
        sVec4_3648 nextSimPos;
        sVec4_3648 nextSimVel;
        cAirPredictor_simOneTick_3648(((char*)self), (sVec4_3648*)(&self->PrevTickPos), (sVec4_3648*)(&self->PrevTickVel), &nextSimPos, &nextSimVel);
        float k = sinceLastTick * 59.999996185302734f;
        *(sVec4_3648*)airPos = vu0Add_3648(vu0Scale_3648(nextSimPos, k), vu0Scale_3648(*(sVec4_3648*)&self->PrevTickPos, 1.0f - k));
        *(sVec4_3648*)airVel = vu0Add_3648(vu0Scale_3648(nextSimVel, k), vu0Scale_3648(*(sVec4_3648*)&self->PrevTickVel, 1.0f - k));
        ((float*)airPos)[3] = 1.0f;
    } else {
        *(sQuad*)airPos = *(sQuad*)&self->PrevTickPos;
        *(sQuad*)airVel = *(sQuad*)&self->PrevTickVel;
    }
    self->CurAirTime += dt;
}
#endif

//100%
INCLUDE_ASM("ai/airpredictor", cAirPredictor_land__FPv);
#ifdef SKIP_ASM
/**
 * Clears the air predictor's TotalAirTime (+0x98) when the rider lands.
 *
 * cWipeoutMotion_resolveAirMotion calls it with the rider's mPosition and mVelocity, which are ignored (the C
 * declaration takes only self).
 */
void cAirPredictor_land(void* vself)
{
    cAirPredictor* self = (cAirPredictor*)vself;
    *(int*)&self->TotalAirTime = 0;
}
#endif

//100%
INCLUDE_ASM("ai/airpredictor", cAirPredictor_simOneTick);
#ifdef SKIP_ASM
struct sVec4_1139A0
{
    float x, y, z, w;
    sVec4_1139A0() {}
    sVec4_1139A0(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_1139A0 vu0Scale_1139A0(const sVec4_1139A0& v, float s)
{
    sVec4_1139A0 r;
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
static inline sVec4_1139A0 vu0Add_1139A0(const sVec4_1139A0& a, const sVec4_1139A0& b)
{
    sVec4_1139A0 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times-assign scalar).
static inline void vu0ScaleEq_1139A0(sVec4_1139A0& v, float s)
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

// PORT: PS2-only VU0 inline asm (4-component length).
static inline float vu0Length_1139A0(const sVec4_1139A0& v)
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
 * Advances a flight by one 1/60 s tick and returns the new speed.
 *
 * nextPos = curPos + curVel / 60. nextVel = curVel plus a drag of -vel.x / 300 and -vel.y / 300 and gravity -31.67 per
 * tick (-14.17 while still rising). When the new speed exceeds TerminalVel (+0xA8) the velocity is scaled down to it.
 * Input and output may be the same vectors (cAirPredictor_simStep passes them so).
 *
 * @param pos  the current position
 * @param vel  the current velocity
 * @param outPos  receives the next position
 * @param outVel  receives the next velocity
 * @return the length of the new velocity, at most TerminalVel
 */
extern "C" float cAirPredictor_simOneTick(cAirPredictor* self, sVec4_1139A0* pos, sVec4_1139A0* vel, sVec4_1139A0* outPos, sVec4_1139A0* outVel)
{
    *outPos = vu0Add_1139A0(*pos, vu0Scale_1139A0(*vel, 0.01666666753590107f));
    float dx = vel->x * -0.0033333336468786f;
    float dy = vel->y * -0.0033333336468786f;
    float gravity = -31.666667938232422f;
    if (vel->z > 0.0f) gravity = -14.166666984558105f;
    *outVel = vu0Add_1139A0(*vel, sVec4_1139A0(dx, dy, gravity, 0.0f));
    float len = vu0Length_1139A0(*outVel);
    if (self->TerminalVel < len)
    {
        vu0ScaleEq_1139A0(*outVel, self->TerminalVel / len);
        len = self->TerminalVel;
    }
    return len;
}
#endif

//100%
INCLUDE_ASM("ai/airpredictor", func_00113A98);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty body). Nothing in the source calls it.
 */
extern "C" void func_00113A98(void) {

}
#endif

//100%
INCLUDE_ASM("ai/airpredictor", cAirPredictor_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry00113AA0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes the whole air predictor (0xB0 bytes) into a replay frame: calls @p obj's virtual slot 1 with (self, 176).
 *
 * @param obj  the replay frame (cReplayFramePtr)
 */
extern "C" void cAirPredictor_writeToReplayFrame(cAirPredictor* self, cReplayFramePtr* obj)
{
    sVEntry00113AA0* vt = *(sVEntry00113AA0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 176);
}
#endif

//100%
INCLUDE_ASM("ai/airpredictor", cAirPredictor_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry00113AD8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Reads the whole air predictor (0xB0 bytes) back from a replay frame: calls @p obj's virtual slot 2 with (self, 176).
 *
 * @param obj  the replay frame (cReplayFramePtr)
 */
extern "C" void cAirPredictor_readFromReplayFrame(cAirPredictor* self, cReplayFramePtr* obj)
{
    sVEntry00113AD8* vt = *(sVEntry00113AD8**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 176);
}
#endif

