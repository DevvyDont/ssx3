#include "common.h"
#include "ai/riderhandler.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/rider.h"

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_gainFocus);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);
struct cRiderSphereTree;
cRiderSphereTree* cRiderSphereTree_cRiderSphereTree(cRiderSphereTree* self);
extern char str_BoardSphereTree[];

struct sVec4WG {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0DotWG(const sVec4WG& a, const sVec4WG& b)
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
static inline sVec4WG vu0ScaleMemWG(const sVec4WG& v, float s)
{
    sVec4WG r;
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
static inline void vu0SubWG(sVec4WG& dst, const sVec4WG& a, const sVec4WG& b)
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
extern "C" void cWipeoutMotion_gainFocus(cWipeoutMotion* self, int prev)
{
    if (self->BoardSphereTree == 0)
    {
        self->BoardSphereTree = (cRiderSphereTree*)cRiderSphereTree_cRiderSphereTree((cRiderSphereTree*)cMemMan_alloc(688, str_BoardSphereTree, 0, 0));
    }
    char* rider = (char*)self->mRider;
    sVec4WG* pos = (sVec4WG*)(rider + 0x110);
    *(sVec4WG*)&self->BoardIntersectBox = *pos;
    *(sVec4WG*)&self->BoardIntersectBox.MaxPoint = *pos;
    if (prev == 0)
    {
        sVec4WG* n = (sVec4WG*)(rider + 0x370);
        float normalSpeed = vu0DotWG(*(sVec4WG*)(rider + 0x1E0), *n);
        sVec4WG normalPart = vu0ScaleMemWG(*n, normalSpeed);
        sVec4WG* velocity = (sVec4WG*)((char*)self->mRider + 0x1E0);
        vu0SubWG(*velocity, *velocity, normalPart);
        *(int*)self = 0;
    }
    else
    {
        *(int*)self = 1;
        cAirPredictor_startLaunchIntoAir(*(void**)(rider + 0x788), pos, rider + 0x1E0, 3333.33349609375f);
    }
    self->ForceNaturalAir = 0;
    *(int*)((char*)self->mRider + 0x2DC) = 0;
    *(int*)((char*)self->mRider + 0x2E0) = 0;
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_throwItem);
#ifdef SKIP_ASM
struct sVec4WM {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sVecPairWM {
    sVec4WM a;
    sVec4WM b;
};

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0DotWM(const sVec4WM& a, const sVec4WM& b)
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
static inline sVec4WM vu0ScaleWM(const sVec4WM& v, float s)
{
    sVec4WM r;
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
static inline void vu0SubWM(sVec4WM& dst, const sVec4WM& a, const sVec4WM& b)
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
extern "C" void cWipeoutMotion_throwItem(cWipeoutMotion* self, sVec4WM* initVel, sVec4WM* initAngVel, sVecPairWM* itemBoneID)
{
    *(sVecPairWM*)((char*)self->mRider + 0x130) = *itemBoneID;
    *(int*)((char*)self->mRider + 0x150) = 1;
    *(sVec4WM*)&self->BoardAngVelocity = *initAngVel;
    *(sVec4WM*)&self->BoardVelocity = *initVel;
    if (*(int*)self == 0)
    {
        sVec4WM* n = (sVec4WM*)((char*)self->mRider + 0x370);
        float normalSpeed = vu0DotWM(*n, *initVel);
        sVec4WM normalPart = vu0ScaleWM(*n, normalSpeed);
        vu0SubWM(*(sVec4WM*)&self->BoardVelocity, *(sVec4WM*)&self->BoardVelocity, normalPart);
    }
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_throwRider);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_startLaunchIntoAir(void* self, void* a, void* b, float t);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4WM vu0ScaleMemWM_136DE0(const sVec4WM& v, float s)
{
    sVec4WM r;
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
extern "C" void cWipeoutMotion_throwRider(cWipeoutMotion* self, sVec4WM* initVel, sVec4WM* initAngVel)
{
    *(sVec4WM*)&self->RiderAngVelocity = *initAngVel;
    *(sVec4WM*)((char*)self->mRider + 0x1E0) = *initVel;
    if (*(int*)self == 0)
    {
        sVec4WM* n = (sVec4WM*)((char*)self->mRider + 0x370);
        float normalSpeed = vu0DotWM(*n, *initVel);
        sVec4WM normalPart = vu0ScaleMemWM_136DE0(*n, normalSpeed);
        sVec4WM* velocity = (sVec4WM*)((char*)self->mRider + 0x1E0);
        vu0SubWM(*velocity, *velocity, normalPart);
    }
    else
    {
        char* rider = (char*)self->mRider;
        cAirPredictor_startLaunchIntoAir(*(void**)(rider + 0x788), rider + 0x110, rider + 0x1E0, 3333.33349609375f);
    }
    self->ForceNaturalAir = 0;
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_step);
#ifdef SKIP_ASM
extern "C" void func_00136F30();
extern "C" void func_00137D18(void*);
extern "C" void func_00137750(cWipeoutMotion*);

/**
 * Steps the wipeout physics one tick: the thrown board, then the rider on the ground or in the air.
 *
 * Calls func_00136F30 (the board, while the root pose override is on), then func_00137D18 for WMS_Ground or
 * func_00137750 (air predictor) for WMS_Air. func_00136F30 is declared without arguments here but still
 * receives self.
 */
extern "C" void cWipeoutMotion_step(int* self)
{
    func_00136F30();
    if (*self == 0)
    {
        func_00137D18(self);
    }
    else
    {
        func_00137750((cWipeoutMotion*)self);
    }
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_resolve);
#ifdef SKIP_ASM
extern "C" void func_00137138();
extern "C" void cWipeoutMotion_resolveGroundMotion(void*);
extern "C" void cWipeoutMotion_resolveAirMotion(void*);

/**
 * Resolves the wipeout's collisions for this tick: the thrown board, then the rider on the ground or in the
 * air.
 *
 * Calls func_00137138 (the board against the world), then cWipeoutMotion_resolveGroundMotion for WMS_Ground or cWipeoutMotion_resolveAirMotion for
 * WMS_Air. func_00137138 is declared without arguments here but still receives self.
 */
extern "C" void cWipeoutMotion_resolve(int* self)
{
    func_00137138();
    if (*self == 0)
    {
        cWipeoutMotion_resolveGroundMotion(self);
    }
    else
    {
        cWipeoutMotion_resolveAirMotion(self);
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
void cWipeoutMotion_loseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00136F30);
#ifdef SKIP_ASM
struct sVec4_6F30 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sRider_6F30 {
    char pad_0x0[0x130];
    sVec4_6F30 pos;         // 0x130
    sVec4_6F30 quat;        // 0x140
    int active;             // 0x150
    char pad_0x154[0x1AC];
    float rate;             // 0x300
};

struct sWipeout_6F30 {
    char pad_0x0[0x10];
    sVec4_6F30 vel;         // 0x10
    sVec4_6F30 angVel;      // 0x20
    char pad_0x30[0x10];
    sRider_6F30* rider;     // 0x40
};

// PORT: PS2-only VU0 inline asm (vector * scalar).
static inline void vu0Scale_6F30(sVec4_6F30& out, const sVec4_6F30& v, float s)
{
    sVec4_6F30 r;
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
static inline void vu0ScaleEq_6F30(sVec4_6F30& v, float s)
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
static inline void vu0AddEq_6F30(sVec4_6F30& a, const sVec4_6F30& b)
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
static inline float vu0Length_6F30(const sVec4_6F30& v)
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
static inline void vu0QuatDeriv_6F30(sVec4_6F30& out, const sVec4_6F30& q, const sVec4_6F30& w)
{
    sVec4_6F30 r;
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
static inline void vu0NormalizeEq_6F30(sVec4_6F30& v)
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
 * dt / 2, and integrates the quaternion at rider + 0x140 by it (VU0 quaternion derivative), then renormalises it.
 * cWipeoutMotion_step calls it without arguments; self still arrives in a0.
 */
extern "C" void func_00136F30(sWipeout_6F30* self)
{
    if (self->rider->active == 0) {
        return;
    }
    float dt = self->rider->rate * 0.01666666753590107f;
    sVec4_6F30 tmp;
    vu0Scale_6F30(tmp, self->vel, dt);
    vu0AddEq_6F30(self->rider->pos, tmp);
    sVec4_6F30 v2;
    v2.x = self->vel.x * -0.20000000298023224f;
    v2.y = self->vel.y * -0.20000000298023224f;
    v2.z = -1800.0f;
    v2.w = 0.0f;
    vu0Scale_6F30(tmp, v2, dt);
    vu0AddEq_6F30(self->vel, tmp);
    float len = vu0Length_6F30(self->vel);
    if (3333.33349609375f < len) {
        vu0ScaleEq_6F30(self->vel, 3333.33349609375f / len);
    }
    vu0ScaleEq_6F30(self->angVel, 1.0f - dt * 0.5f);
    vu0QuatDeriv_6F30(v2, self->rider->quat, self->angVel);
    vu0Scale_6F30(tmp, v2, dt);
    sRider_6F30* r = self->rider;
    r->quat.x += tmp.x;
    r->quat.y += tmp.y;
    r->quat.z += tmp.z;
    r->quat.w += tmp.w;
    vu0NormalizeEq_6F30(self->rider->quat);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137138);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sHit_137138 {
    t4Vector pos;       // 0x00
    t4Vector normal;    // 0x10
    t4Vector v20;       // 0x20
    float pad30[7];     // 0x30
    int material;       // 0x4C
    char* obj;          // 0x50
    char pad54[0x2C];   // 0x54
} __attribute__((aligned(16)));

struct sSphere_137138 {
    int state;          // 0x00
    int f4;             // 0x04
    int f8;             // 0x08
    int padC;           // 0x0C
    t4Vector dir;       // 0x10
    t4Vector v20;       // 0x20
    t4Vector v30;       // 0x30
    char rest[0x320 - 0x40];
} __attribute__((aligned(16)));

struct sVEntH_137138 { short delta; short index; void (*fn)(void*, void*, const t4Vector&); };


struct sSeg_137138 {
    t4Vector a, b;
    sSeg_137138() {}
    sSeg_137138(const t4Vector& pa, const t4Vector& pb) : a(pa), b(pb) {}
};


extern "C" void* cWorldRider_cWorldRider(void* self, void* tree, int flag);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);
extern "C" float func_003342D0(void* world, void* seg, void* hit, int flags);
extern "C" void cWipeoutMotion_updateBoardSphereTree(cWipeoutMotion* self);
extern "C" float func_00336850_137138(void* self, void* q, void* out, int a3) __asm__("func_00336850");
extern "C" void func_00329B40_137138(void* tree, const t4Vector* d) __asm__("func_00329B40");
extern "C" void func_0031BE50(float* s, float* c, float angle);
float AIrandf(float lo, float hi);
extern t4Vector D_004FF120_137138 __asm__("t4vect_Zero");

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

struct sVec4c_137138 : t4Vector {
    sVec4c_137138(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

/**
 * Collides the thrown board with the world and bounces or stops it.
 *
 * Only while mUseRootPoseOverride is set. Updates BoardSphereTree (cWipeoutMotion_updateBoardSphereTree), builds a
 * cWorldRider probe from it, stores its segment in BoardIntersectBox and tests it against SSXApp->Game->BXNash
 * (func_00336850). On a hit it pushes mRootPoseOverridePos, the sphere tree and the box out along the hit normal by 1.1
 * x the depth. A fast impact (normal speed 555.6 or more) or a steep surface (normal.z below 0.8) bounces the board:
 * BoardVelocity gains (v + max(v / 2, 27.8)) along the normal, the hit object's owner (virtual slot 43) gets the
 * impulse scaled by -0.05 x SSXApp + 0x10, BoardAngVelocity is damped by 0.85 and, under 4 pi rad/s, gets a random
 * 45-180 degree spin. Otherwise the owner gets the board velocity scaled by 0.05 x SSXApp + 0x10 and both board
 * velocities are zeroed. The probe is destroyed at the end. cWipeoutMotion_resolve calls it without arguments; self
 * still arrives in a0.
 *
 * @param vself  the cWipeoutMotion (this)
 */
extern "C" void func_00137138(cWipeoutMotion* vself)
{
    cWipeoutMotion* self = vself;
    if ((self->mRider)->mUseRootPoseOverride == 0)
        return;
    cWipeoutMotion_updateBoardSphereTree((cWipeoutMotion*)((char*)self));
    sHit_137138 hit;
    sSphere_137138 sph;
    cWorldRider_cWorldRider(&sph, ((void*)self->BoardSphereTree), 1);
    sSeg_137138 seg(sph.v30, sph.v20);
    (*(sSeg_137138*)&self->BoardIntersectBox) = seg;
    float d = func_00336850_137138(SSXApp->Game->BXNash, &sph, &hit, 0);
    if (d < 0.0f) {
        cWorldRider__cWorldRider(&sph, 2);
        return;
    }
    t4Vector push = vu0Scale_137138(vu0Scale_137138(hit.normal, 1.100000023841858f), d);
    vu0AddEq_137138((self->mRider)->mRootPoseOverridePos, push);
    func_00329B40_137138(((void*)self->BoardSphereTree), &push);
    // The binary copies the segment back into the local before moving it (the copy is never read).
    seg = (*(sSeg_137138*)&self->BoardIntersectBox);
    vu0AddEq_137138((*(sSeg_137138*)&self->BoardIntersectBox).a, push);
    vu0AddEq_137138((*(sSeg_137138*)&self->BoardIntersectBox).b, push);
    float v = vu0Dot_137138(vu0Scale_137138(hit.normal, -1.0f), self->BoardVelocity);
    if (555.5555419921875f <= v || hit.normal.z < 0.800000011920929f) {
        float k = v * 0.5f;
        if (k < 27.77777862548828f)
            k = 27.77777862548828f;
        t4Vector impulse = vu0Scale_137138(hit.normal, v + k);
        vu0AddEq_137138(self->BoardVelocity, impulse);
        if (hit.obj != 0) {
            char* o = *(char**)(hit.obj + 0xC);
            if (o != 0) {
                sVEntH_137138* vt = *(sVEntH_137138**)(o + 0xC);
                vt[43].fn(o + vt[43].delta, &hit,
                          vu0Scale_137138(impulse, (float)*(int*)((char*)SSXApp + 0x10) * -0.05000000074505806f));
            }
        }
        vu0ScaleEq_137138(self->BoardAngVelocity, 0.8500000238418579f);
        if (vu0Length_137138(self->BoardAngVelocity) < 12.56637191772461f) {
            float pitch = AIrandf(-1.5707963705062866f, 1.5707963705062866f);
            float yaw = AIrandf(-3.1415927410125732f, 3.1415927410125732f);
            float sp, cp, sy, cy;
            func_0031BE50(&sp, &cp, pitch);
            func_0031BE50(&sy, &cy, yaw);
            float deg = AIrandf(45.0f, 180.0f);
            vu0AddEq_137138(self->BoardAngVelocity,
                            vu0Scale_137138(sVec4c_137138(cp * sy, cp * cy, sp, 0.0f), deg * 0.01745329424738884f));
        }
    } else {
        if (hit.obj != 0) {
            char* o = *(char**)(hit.obj + 0xC);
            if (o != 0) {
                sVEntH_137138* vt = *(sVEntH_137138**)(o + 0xC);
                vt[43].fn(o + vt[43].delta, &hit,
                          vu0Scale_137138(self->BoardVelocity, (float)*(int*)((char*)SSXApp + 0x10) * 0.05000000074505806f));
            }
        }
        self->BoardVelocity = D_004FF120_137138;
        self->BoardAngVelocity = D_004FF120_137138;
    }
    cWorldRider__cWorldRider(&sph, 2);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_updateBoardSphereTree);
#ifdef SKIP_ASM
struct sVec4_7550 {
    float x, y, z, w;
    sVec4_7550() {}
    sVec4_7550(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sQuat_7550 {
    float x, y, z, w;
};

struct sBone_7550 {
    sVec4_7550 pos;     // 0x00
    sQuat_7550 rot;     // 0x10
};

struct sRadii_7550 {
    float r[4];
};
extern const sRadii_7550 D_00458220;

extern "C" void func_00329A90(void* tree, int n, sVec4_7550* pos, float* radii, sVec4_7550* center, float radius);

// PORT: PS2-only VU0 inline asm (non-volatile) (vector times scalar).
static inline sVec4_7550 vu0Scale_7550(const sVec4_7550& v, float s)
{
    sVec4_7550 r;
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
static inline sVec4_7550 vu0Add_7550(const sVec4_7550& a, const sVec4_7550& b)
{
    sVec4_7550 r;
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
static inline sVec4_7550 vu0Sub_7550(const sVec4_7550& a, const sVec4_7550& b)
{
    sVec4_7550 r;
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

static inline sVec4_7550 xAxis_7550(const sBone_7550* b)
{
    return sVec4_7550(1.0f - 2.0f * (b->rot.y * b->rot.y + b->rot.z * b->rot.z),
                      2.0f * (b->rot.x * b->rot.y + b->rot.w * b->rot.z),
                      2.0f * (b->rot.x * b->rot.z - b->rot.w * b->rot.y),
                      0.0f);
}

/**
 * Rebuilds the wiping-out rider's board collision spheres along the board bone.
 *
 * Takes the rider's mAnimModel bone mBoneBoardChild (+0x8A4) and the model's scale (+0x140), and places four sphere
 * centres along the bone's x axis at +70, +30, -30 and -70 scaled units from the bone position, with radii from
 * D_00458220. func_00329A90 then refits BoardSphereTree around them, centred on the bone with radius scale * 90.
 */
extern "C" void cWipeoutMotion_updateBoardSphereTree(cWipeoutMotion* self)
{
    sVec4_7550 pts[4];
    sRadii_7550 radii = D_00458220;
    char* rider = (char*)self->mRider;
    char* model = *(char**)(rider + 0x780);
    sBone_7550* bone = &(*(sBone_7550**)(model + 0x2C))[*(int*)(rider + 0x8A4)];
    float modelScale = *(float*)(model + 0x140);
    sVec4_7550 boardAxis = vu0Scale_7550(xAxis_7550(bone), modelScale);
    pts[0] = vu0Add_7550(bone->pos, vu0Scale_7550(boardAxis, 70.0f));
    pts[1] = vu0Add_7550(bone->pos, vu0Scale_7550(boardAxis, 30.0f));
    pts[2] = vu0Sub_7550(bone->pos, vu0Scale_7550(boardAxis, 30.0f));
    pts[3] = vu0Sub_7550(bone->pos, vu0Scale_7550(boardAxis, 70.0f));
    func_00329A90(self->BoardSphereTree, 4, pts, radii.r, &bone->pos,
                  *(float*)(*(char**)((char*)self->mRider + 0x780) + 0x140) * 90.0f);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137750);
#ifdef SKIP_ASM
extern "C" void cRider_updateOrientationImplicit(void*);
extern "C" void cAirPredictor_updateFlightPath(void* ap, void* pos, void* vel, float dt);
extern "C" void cRider_alignRiderAxis(void* rider, void* a, void* b, float x, float y);

struct sVec4W7 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times-assign scalar).
static inline void vu0ScaleEqW7(sVec4W7& v, float s)
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
 * predictor's SimState (+0xAC) is 1 or 3 (landing found) it turns the rider toward the landing (cRider_alignRiderAxis
 * with the landing normal and velocity, a rate of mTimeScale over the remaining air time, or x 60 when under one tick,
 * and mTimeScale x 6.63). RiderAngVelocity is damped by 1 - 1.5 dt and the orientation updated
 * (cRider_updateOrientationImplicit).
 *
 * @param wipeoutMotion  the cWipeoutMotion
 */
extern "C" void func_00137750(cWipeoutMotion* wipeoutMotion)
{
    char* rider = (char*)wipeoutMotion->mRider;
    float dt = *(float*)(rider + 0x300) * 0.01666666753590107f;
    cAirPredictor_updateFlightPath(*(void**)(rider + 0x788), rider + 0x110, rider + 0x1E0, dt);
    int st = *(int*)(*(char**)((char*)wipeoutMotion->mRider + 0x788) + 0xAC);
    int ok = 0;
    if (st == 1 || st == 3) ok = 1;
    if (ok)
    {
        char* airRider = (char*)wipeoutMotion->mRider;
        char* ap = *(char**)(airRider + 0x788);
        float timeLeft = *(float*)(ap + 0x98) - *(float*)(ap + 0xA0);
        float x = *(float*)(airRider + 0x300);
        if (timeLeft >= 0.01666666753590107f)
            x /= timeLeft;
        else
            x *= 59.999996185302734f;
        char* alignRider = (char*)wipeoutMotion->mRider;
        float k = 6.632251739501953f;
        cRider_alignRiderAxis(alignRider, *(char**)(alignRider + 0x788) + 0x20, *(char**)(alignRider + 0x788) + 0x10, x, *(float*)(alignRider + 0x300) * k);
    }
    vu0ScaleEqW7(*(sVec4W7*)&wipeoutMotion->RiderAngVelocity, 1.0f - dt * 1.5f);
    cRider_updateOrientationImplicit(wipeoutMotion->mRider);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_resolveAirMotion);
#ifdef SKIP_ASM
// self is a cWipeoutMotion (no header yet); +0x40 is its cRider virtual-base pointer, +0x0 its state.
struct sHit_137860 {
    t4Vector pos;       // 0x00
    t4Vector normal;    // 0x10
    t4Vector v20;       // 0x20
    float pad30[7];     // 0x30
    int material;       // 0x4C
    char* obj;          // 0x50
    char pad54[0x2C];   // 0x54
} __attribute__((aligned(16)));

struct sSphere_137860 {
    char data[0x320];
} __attribute__((aligned(16)));

struct sVEntR_137860 { short delta; short index; bool (*fn)(void*); };
struct sVEntH_137860 { short delta; short index; void (*fn)(void*, void*, const t4Vector&); };

extern "C" void* cWorldRider_cWorldRider(void* self, void* tree, int flag);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);
extern "C" float func_003342D0(void* world, void* seg, void* hit, int flags);
extern "C" float func_00138960(void* self, char* hit, sVec4WG* dir);
// PORT: the project names these __FPv (one void* param); this caller passes more.
void func_00113998_137860(void* ap, void* pos, void* vel) __asm__("func_00113998__FPv");
void func_00111AA0_137860(void* self, void* a, void* b, int c, float x) __asm__("func_00111AA0__FPv");
extern "C" void func_00106538(void* rider, const t4Vector& v);
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
    char* sub = (char*)&rider->mVtbl;
    sVEntR_137860* vt = *(sVEntR_137860**)sub;
    return vt[kRiderVt_isHuman].fn(sub + vt[kRiderVt_isHuman].delta);
}

#define RIDER (*(cRider**)(self + 0x40))

/**
 * Resolves a wiping-out rider in the air against the world: lands it on usable ground, otherwise pushes it out and
 * relaunches it.
 *
 * Builds a world query from the rider's sphere tree (cWorldRider_cWorldRider; terrain only for a computer rider) and intersects
 * it with the rider's overlap list (func_003342D0, mWallHint). On a hit, func_00138960 (On Tour intersectGround) checks
 * the ground below; if that is solid enough (>= 0.4), the rider is not moving away from it and is within 10 of it, the
 * rider lands: func_00113998 (On Tour cAirPredictor::land), the impact effect (func_00111AA0), the handler's landed
 * flag and velocity (+0x314/+0x320), the ground normal, surface, contact point and height, the velocity loses its
 * into-ground part (passed to a hit object's vtable slot 43 scaled by the frame rate), any penetration is pushed out
 * (func_00106538) and the wipeout state (+0x0) becomes 0 (ground).
 *
 * Otherwise the rider is pushed out by 1.1 x the depth along the hit normal, loses its into-wall velocity when the wall
 * is not ground-like (normals differ by more than ~25 degrees), and cAirPredictor_startLaunchIntoAir relaunches it with
 * gravity 3333.33. Always ends with the bound box, object and opponent collision checks.
 *
 * @param vself  the wipeout motion (this); its rider is reached through the motion's rider pointer
 */
extern "C" void cWipeoutMotion_resolveAirMotion(void* vself)
{
    char* self = (char*)vself;
    sHit_137860 hitA;
    sSphere_137860 sph;
    sHit_137860 hitB;
    cRider* rider = RIDER;
    cWorldRider_cWorldRider(&sph, rider->mSphereTree, !isHuman_137860(rider));
    float d = func_003342D0(RIDER->mOverlapList, &sph, &hitA, (int)RIDER->mWallHint);
    if (d >= 0.0f)
    {
        float t = func_00138960(self, (char*)&hitB, (sVec4WG*)&hitA.normal);
        float vn = 0.0f;
        float h = 0.0f;
        if (t >= 0.4000000059604645f)
        {
            vn = vu0Dot_137860(RIDER->mVelocity, hitB.normal);
            h = vu0Dot_137860(vu0Sub_137860(RIDER->mPosition, hitB.pos), hitB.normal);
        }
        if (t >= 0.4000000059604645f && vn <= 0.0f && h <= 10.0f)
        {
            func_00113998_137860(RIDER->mAirPredictor, &RIDER->mPosition, &RIDER->mVelocity);
            func_00111AA0_137860(RIDER->mRiderHandler, &hitB, &hitB.normal, hitB.material, vn);
            cRider* r2 = RIDER;
            char* hd = (char*)r2->mRiderHandler;
            *(int*)(hd + 0x314) = 1;
            *(t4Vector*)(hd + 0x320) = r2->mVelocity;
            RIDER->unk_3D0 = hitB.v20;
            RIDER->mGroundNormal = hitB.normal;
            RIDER->mSurfaceType = hitB.material;
            RIDER->mGroundContactPoint = hitB.pos;
            RIDER->unk_454 = vu0Dot_137860(vu0Sub_137860(RIDER->mPosition, hitB.pos), RIDER->mGroundNormal);
            t4Vector impulse = vu0Scale_137860(RIDER->mGroundNormal, -vn);
            vu0AddEq_137860(RIDER->mVelocity, impulse);
            if (hitB.obj != 0)
            {
                char* o = *(char**)(hitB.obj + 0xC);
                if (o != 0)
                {
                    sVEntH_137860* vt = *(sVEntH_137860**)(o + 0xC);
                    vt[43].fn(o + vt[43].delta, &hitB,
                              vu0Scale_137860(vu0Scale_137860(impulse, -1.0f), (float)*(int*)((char*)SSXApp + 0x10)));
                }
            }
            float pen = vu0Dot_137860(vu0Sub_137860(RIDER->mPosition, hitB.pos), hitB.normal);
            if (pen < 0.0f)
            {
                func_00106538(RIDER, vu0Scale_137860(hitB.normal, -pen));
            }
            *(int*)self = 0;
        }
        else
        {
            t4Vector push = vu0Scale_137860(vu0Scale_137860(hitA.normal, 1.100000023841858f), d);
            func_00106538(RIDER, push);
            if (t < 0.0f || vu0Dot_137860(hitB.normal, hitA.normal) < 0.8999999761581421f)
            {
                float v = vu0Dot_137860(hitA.normal, RIDER->mVelocity);
                if (v < 0.0f)
                {
                    vu0SubEq_137860(RIDER->mVelocity, vu0Scale_137860(hitA.normal, v));
                }
            }
            cAirPredictor_startLaunchIntoAir(RIDER->mAirPredictor, &RIDER->mPosition, &RIDER->mVelocity, 3333.33349609375f);
        }
    }
    cRider_updateBoundBox(RIDER, 0);
    cRider_checkObjectCollisions(RIDER, 0);
    cRider_checkOpponentCollisions(RIDER);
    cWorldRider__cWorldRider(&sph, 2);
}

#undef RIDER
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00137D18);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sHit_137D18 {
    t4Vector pos;       // 0x00
    t4Vector normal;    // 0x10
    t4Vector v20;       // 0x20
    float pad30[7];     // 0x30
    int material;       // 0x4C
    char* obj;          // 0x50
    char* tri;          // 0x54
    char pad58[0x28];   // 0x58
} __attribute__((aligned(16)));

// A point of a piecewise-linear curve.
struct sCurvePt_137D18 { float x, y; };

// Per-material surface settings (0xB0 bytes each).
struct sMat_137D18 {
    char pad00[0x18];
    float f18;                  // 0x18
    char pad1C[0x28];
    int f44;                    // 0x44
    char pad48[0x48];
    sCurvePt_137D18 p[4];       // 0x90
};

struct sVEntH_137D18 { short delta; short index; void (*fn)(void*, void*, const t4Vector&); };

extern "C" float func_00138960(void* self, char* hit, sVec4WG* dir);
extern "C" int cRiderAnimBase_getCurrentAnimCategory(void*, int);
extern "C" void cRider_alignRiderAxis(void* rider, void* a, void* b, float x, float y);
void func_00111AA0_137D18(void* self, void* a, void* b, int c, float x) __asm__("func_00111AA0__FPv");
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

static inline float curve_137D18(const sMat_137D18* c, float x)
{
    if (c->p[1].x < x)
    {
        if (c->p[2].x < x)
        {
            if (c->p[3].x < x)
                return c->p[3].y;
            return lerp_137D18(c->p[2], c->p[3], x);
        }
        return lerp_137D18(c->p[1], c->p[2], x);
    }
    if (x < c->p[0].x)
        return c->p[0].y;
    return lerp_137D18(c->p[0], c->p[1], x);
}

// PORT: <? (g++ min operator).
static inline float clamp_137D18(float v, float lo, float hi)
{
    return lo <= v ? v <? hi : lo;
}

static inline sMat_137D18* materials_137D18()
{
    return (sMat_137D18*)SSXApp->Game->SurfPropCache;
}

static inline short triFlags_137D18(char* t)
{
    return *(short*)(t + 0xA);
}

static inline t4Vector getFront_137D18(cRider* rider)
{
    return rider->mMatrix.row[1];
}

#define RIDER (*(cRider**)(self + 0x40))

/**
 * Moves the wiped-out rider along the ground one tick: sliding friction, gravity, landing on and leaving the surface.
 *
 * RiderAngVelocity's length eases toward the speed x 0.0125. Friction comes from the surface material's curve
 * (SurfPropCache entry for mSurfaceType, its four points at +0x90) at the tangential speed relative to the ground
 * (unk_3D0), x 3 + 1.5; slow slides (under 1111) during the root pose override or anim categories 29 / 25 blend toward
 * 40. It is scaled by mGroundNormal.z + 0.3 (clamped 0-1) and capped at 1 / dt. Position and velocity are integrated
 * with gravity -1800 and the speed capped at 3333.3, and the rider is aligned to the ground (cRider_alignRiderAxis).
 * func_00138960 then probes the ground: a hit updates unk_3D0, mGroundNormal, mSurfaceType, mGroundContactPoint and
 * unk_454; sinking deeper than the material limit (mAnimModel unk_140.x x material +0x18) pushes the rider out and
 * cancels the inward velocity, a shallower dip springs it back, both spawning the impact effect (func_00111AA0); more
 * than 10 above it launches the rider (cAirPredictor_startLaunchIntoAir, ForceNaturalAir (+0x4) = 1), as does a miss.
 * The hit object's owner (virtual slot 43) gets the velocity change, and a material with +0x44 set or a triangle with
 * flag 2 forces a reset (cRider_checkForReset with 0, 1).
 *
 * @param vself  the cWipeoutMotion
 */
// Wipeout tumble: damps the spin, slides along the ground with surface friction, then re-probes the ground.
extern "C" void func_00137D18(void* vself)
{
    char* self = (char*)vself;
    float len = vu0Length_137D18(*(t4Vector*)(self + 0x30));
    float vl = vu0Length_137D18(RIDER->mVelocity) * 0.012500000186264515f;
    float dt = RIDER->mTimeScale * 0.01666666753590107f;
    float nl = len + (vl - len) * 5.0f * dt;
    if (0.0010000000474974513f < len)
        vu0ScaleEq_137D18(*(t4Vector*)(self + 0x30), nl / len);
    sMat_137D18* mat = &materials_137D18()[RIDER->mSurfaceType];
    t4Vector v = vu0Sub_137D18(RIDER->mVelocity, RIDER->unk_3D0);
    float vn = vu0Dot_137D18(v, RIDER->mGroundNormal);
    t4Vector n = vu0Scale_137D18(RIDER->mGroundNormal, vn);
    t4Vector tang = vu0Sub_137D18(v, n);
    float tl = vu0Length_137D18(tang);
    float fric = curve_137D18(mat, tl * 0.035999998450279236f) * 3.0f + 1.5f;
    if (tl < 1111.111083984375f
        && (RIDER->mUseRootPoseOverride != 0 || cRiderAnimBase_getCurrentAnimCategory(RIDER->mRiderAnim, 2) == 29
            || cRiderAnimBase_getCurrentAnimCategory(RIDER->mRiderAnim, 2) == 25)) {
        float q = 1.0f - tl * 0.0009000000427477062f;
        q *= q;
        q *= q;
        fric = q * 40.0f + (1.0f - q) * fric;
    }
    fric *= clamp_137D18(RIDER->mGroundNormal.z + 0.30000001192092896f, 0.0f, 1.0f);
    if (1.0f / dt < fric)
        fric = 1.0f / dt;
    t4Vector grav = vu0Scale_137D18(t4vect_ZAxis, -1800.0f);
    t4Vector drag = vu0Scale_137D18(tang, -fric);
    t4Vector acc = vu0Add_137D18(grav, drag);
    vu0AddEq_137D18(RIDER->mPosition, vu0Scale_137D18(RIDER->mVelocity, dt));
    vu0AddEq_137D18(RIDER->mVelocity, vu0Scale_137D18(acc, dt));
    float sp = vu0Length_137D18(RIDER->mVelocity);
    if (3333.33349609375f < sp)
        vu0ScaleEq_137D18(RIDER->mVelocity, 3333.33349609375f / sp);
    t4Vector dir = vu0Normalize_137D18(vu0Add_137D18(getFront_137D18(RIDER), vu0Scale_137D18(RIDER->mVelocity, 0.0036000001709908247f)));
    cRider_alignRiderAxis(RIDER, &RIDER->mGroundNormal, &dir, RIDER->mTimeScale * 2.0f, RIDER->mTimeScale * 6.632251739501953f);
    sHit_137D18 hit;
    float d = func_00138960(self, (char*)&hit, (sVec4WG*)&RIDER->mGroundNormal);
    if (0.0f <= d) {
        RIDER->unk_3D0 = hit.v20;
        RIDER->mGroundNormal = hit.normal;
        RIDER->mSurfaceType = hit.material;
        RIDER->mGroundContactPoint = hit.pos;
        RIDER->unk_454 = vu0Dot_137D18(vu0Sub_137D18(RIDER->mPosition, hit.pos), RIDER->mGroundNormal);
        float lim = RIDER->mAnimModel->unk_140.x * mat->f18;
        float pen = vu0Dot_137D18(vu0Sub_137D18(RIDER->mPosition, hit.pos), hit.normal);
        if (pen < -lim) {
            float spd = vu0Length_137D18(hit.v20);
            float vn2 = vu0Dot_137D18(RIDER->mVelocity, hit.normal);
            if (vn2 < 0.0f)
                vu0SubEq_137D18(RIDER->mVelocity, vu0Scale_137D18(hit.normal, vn2));
            vu0SubEq_137D18(RIDER->mPosition, vu0Scale_137D18(hit.normal, pen + lim));
            func_00111AA0_137D18(RIDER->mRiderHandler, &hit, &hit.normal, hit.material, spd);
        } else if (pen < 0.0f) {
            float spd = vu0Length_137D18(RIDER->mVelocity);
            vu0AddEq_137D18(RIDER->mVelocity, vu0Scale_137D18(RIDER->mGroundNormal, dt * (pen * -2700.0f / lim - vu0Dot_137D18(RIDER->mVelocity, RIDER->mGroundNormal))));
            func_00111AA0_137D18(RIDER->mRiderHandler, &hit, &hit.normal, hit.material, spd);
        } else if (10.0f < pen) {
            float vn3 = vu0Dot_137D18(RIDER->mVelocity, RIDER->mGroundNormal);
            if (vn3 < 0.0f)
                vu0SubEq_137D18(RIDER->mVelocity, vu0Scale_137D18(RIDER->mGroundNormal, vn3));
            cAirPredictor_startLaunchIntoAir(RIDER->mAirPredictor, &RIDER->mPosition, &RIDER->mVelocity, 3333.33349609375f);
            *(int*)(self + 0x4) = 1;
        }
        if (hit.obj != 0) {
            char* o = *(char**)(hit.obj + 0xC);
            if (o != 0) {
                sVEntH_137D18* vt = *(sVEntH_137D18**)(o + 0xC);
                vt[43].fn(o + vt[43].delta, &hit,
                          vu0Div_137D18(vu0Add_137D18(vu0Sub_137D18(v, RIDER->mVelocity), RIDER->unk_3D0), dt));
            }
        }
        sMat_137D18* ground = &materials_137D18()[hit.material];
        if (ground->f44 != 0 || (hit.tri != 0 && (triFlags_137D18(hit.tri) & 2)))
            cRider_checkForReset(RIDER, 0, 1);
    } else {
        cAirPredictor_startLaunchIntoAir(RIDER->mAirPredictor, &RIDER->mPosition, &RIDER->mVelocity, 3333.33349609375f);
        *(int*)(self + 0x4) = 1;
    }
    cRider_updateOrientationImplicit(RIDER);
}

#undef RIDER
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_resolveGroundMotion);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sHit_138640 {
    t4Vector pos;       // 0x00
    t4Vector normal;    // 0x10
    t4Vector v20;       // 0x20
    float pad30[7];     // 0x30
    int material;       // 0x4C
    char* obj;          // 0x50
    char pad54[0x2C];   // 0x54
} __attribute__((aligned(16)));

struct sSphere_138640 {
    int state;          // 0x00
    int f4;             // 0x04
    int f8;             // 0x08
    int padC;           // 0x0C
    t4Vector dir;       // 0x10
    char rest[0x320 - 0x20];
} __attribute__((aligned(16)));

struct sVEntH_138640 { short delta; short index; void (*fn)(void*, void*); };

extern "C" void* cWorldRider_cWorldRider(void* self, void* tree, int flag);
extern "C" void cWorldRider__cWorldRider(void* self, int flags);
extern "C" float func_003342D0(void* world, void* seg, void* hit, int flags);
extern "C" void func_00106538(void* rider, const t4Vector& v);
extern "C" void cRider_checkObjectCollisions(void* rider, int a1);
extern "C" void cRider_checkOpponentCollisions(void* rider);
extern t4Vector D_004FF120_138640 __asm__("t4vect_Zero");

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

#define RIDER (*(cRider**)(self + 0x40))

/**
 * Resolves a wiping-out rider sliding on the ground against walls and objects.
 *
 * Intersects the rider's sphere tree with its overlap list, rejecting along mGroundNormal. A hit that is not straight
 * into the ground (normal dot > -0.9999) has its normal flattened against the ground when facing down, notifies the hit
 * object (vtable slot 42), pushes the rider out by 1.1 x the depth (func_00106538) and, when moving into the wall
 * relative to the object's own velocity, bounces it back by that speed plus half of it clamped to 27.8..555.6 (1..20
 * kph).
 *
 * After the bound box, object and opponent collision checks, a pending flag (+0x4) switches the motion to state 1
 * (air), and a speed under 50 stops the rider.
 *
 * @param vself  the wipeout motion (this)
 */
extern "C" void cWipeoutMotion_resolveGroundMotion(void* vself)
{
    char* self = (char*)vself;
    sHit_138640 hit;
    sSphere_138640 sph;
    cWorldRider_cWorldRider(&sph, RIDER->mSphereTree, 1);
    sph.f8 = 1;
    sph.dir = RIDER->mGroundNormal;
    float d = func_003342D0(RIDER->mOverlapList, &sph, &hit, (int)RIDER->mWallHint);
    if (0.0f <= d) {
        float dot = vu0Dot_138640(RIDER->mGroundNormal, hit.normal);
        if (-0.9998999834060669f < dot) {
            if (dot < 0.0f)
                hit.normal = vu0Normalize_138640(vu0Sub_138640(hit.normal, vu0Scale_138640(RIDER->mGroundNormal, dot)));
            if (hit.obj != 0) {
                char* o = *(char**)(hit.obj + 0xC);
                if (o != 0) {
                    sVEntH_138640* vt = *(sVEntH_138640**)(o + 0xC);
                    vt[42].fn(o + vt[42].delta, &hit);
                }
            }
            t4Vector push = vu0Scale_138640(vu0Scale_138640(hit.normal, 1.100000023841858f), d);
            func_00106538(RIDER, push);
            t4Vector a = vu0Sub_138640(RIDER->mVelocity, hit.v20);
            float dot2 = vu0Dot_138640(vu0Scale_138640(hit.normal, -1.0f), a);
            if (0.0f <= dot2) {
                float k = clamp_138640(dot2 * 0.5f, 27.77777862548828f, 555.5555419921875f);
                vu0AddEq_138640(RIDER->mVelocity, vu0Scale_138640(hit.normal, dot2 + k));
            }
        }
    }
    cRider_updateBoundBox(RIDER, 0);
    cRider_checkObjectCollisions(RIDER, 0);
    cRider_checkOpponentCollisions(RIDER);
    if (*(int*)(self + 0x4) != 0) {
        *(int*)(self + 0x4) = 0;
        *(int*)self = 1;
    }
    if (vu0Length_138640(RIDER->mVelocity) < 50.0f)
        RIDER->mVelocity = D_004FF120_138640;
    cWorldRider__cWorldRider(&sph, 2);
}

#undef RIDER
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", func_00138960);
#ifdef SKIP_ASM
extern "C" void* func_0032E100(void* seg, const sVec4WG& a, const sVec4WG& b, int n, float r);
extern "C" float func_003342D0(void* world, void* seg, void* hit, int flags);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4WG vu0Scale_138960(const sVec4WG& v, float s)
{
    sVec4WG r;
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
static inline sVec4WG vu0Sub_138960(const sVec4WG& a, const sVec4WG& b)
{
    sVec4WG r;
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
static inline sVec4WG vu0Add_138960(const sVec4WG& a, const sVec4WG& b)
{
    sVec4WG r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

struct sVEntry_138960 { short delta; short index; void (*fn)(void*, void*); };

/**
 * Probes the ground under the wiped-out rider along @p dir and records the patch it hits.
 *
 * Builds a capsule (func_0032E100, radius 0.575) from mPosition - 200 x @p dir to mPosition + 200 x @p dir and tests it
 * against the rider's mOverlapList with mLineHint (func_003342D0). On a hit with a patch object it stores the patch's
 * resource id (+0x150) in mGroundPolyId, the hit's u/v in unk_AAC / unk_AB0 and the patch's flags (+0xA) in unk_2D4
 * (else mGroundPolyId = -1), and calls the hit object's owner's virtual slot 42 with the hit.
 *
 * @param hit  receives the intersection
 * @param dir  the probe direction (the ground normal)
 * @return the hit distance, negative on a miss
 */
extern "C" float func_00138960(void* self, char* hit, sVec4WG* dir)
{
    char* rider = *(char**)((char*)self + 0x40);
    char seg[176];
    sVec4WG* pt = (sVec4WG*)(rider + 0x110);
    func_0032E100(seg, vu0Sub_138960(*pt, vu0Scale_138960(*dir, 200.0f)), vu0Add_138960(*pt, vu0Scale_138960(*dir, 200.0f)), 2, 0.574999988079071f);
    char* probeRider = *(char**)((char*)self + 0x40);
    float hitDist = func_003342D0(*(void**)(probeRider + 0x860), seg, hit, *(int*)(probeRider + 0x864));
    if (hitDist >= 0.0f)
    {
        char* obj = *(char**)(hit + 0x54);
        if (obj)
        {
            *(int*)(*(char**)((char*)self + 0x40) + 0x430) = *(int*)(obj + 0x150);
            *(float*)(*(char**)((char*)self + 0x40) + 0xAAC) = *(float*)(hit + 0x6C);
            *(float*)(*(char**)((char*)self + 0x40) + 0xAB0) = *(float*)(hit + 0x70);
            *(unsigned short*)(*(char**)((char*)self + 0x40) + 0x2D4) = *(unsigned short*)(*(char**)(hit + 0x54) + 0xA);
        }
        else
        {
            *(unsigned int*)(*(char**)((char*)self + 0x40) + 0x430) = 0xFFFFFFFF;
        }
        char* hitObj = *(char**)(hit + 0x50);
        if (hitObj)
        {
            char* hitOwner = *(char**)(hitObj + 0xC);
            if (hitOwner)
            {
                sVEntry_138960* vt = *(sVEntry_138960**)(hitOwner + 0xC);
                vt[42].fn(hitOwner + vt[42].delta, hit);
            }
        }
    }
    return hitDist;
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_writeToReplayFrame);
#ifdef SKIP_ASM
struct sVEntry00138AD8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Saves the wipeout motion's state to a replay frame.
 *
 * Writes the first 0x40 bytes (WipeoutMotionState, ForceNaturalAir, BoardVelocity, BoardAngVelocity,
 * RiderAngVelocity) through the frame's write virtual (slot 1).
 *
 * @param obj  the replay frame pointer to write to (On Tour cReplayFramePtr*)
 */
extern "C" void cWipeoutMotion_writeToReplayFrame(cWipeoutMotion* self, void* obj)
{
    sVEntry00138AD8* vt = *(sVEntry00138AD8**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x40);
}
#endif

//100%
INCLUDE_ASM("ai/motion/wipeoutmotion", cWipeoutMotion_readFromReplayFrame);
#ifdef SKIP_ASM
struct sVEntry00138B10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Restores the wipeout motion's state from a replay frame.
 *
 * Reads the first 0x40 bytes (WipeoutMotionState, ForceNaturalAir, BoardVelocity, BoardAngVelocity,
 * RiderAngVelocity) through the frame's read virtual (slot 2).
 *
 * @param obj  the replay frame pointer to read from (On Tour cReplayFramePtr*)
 */
extern "C" void cWipeoutMotion_readFromReplayFrame(cWipeoutMotion* self, void* obj)
{
    sVEntry00138B10* vt = *(sVEntry00138B10**)obj;
    vt[2].fn((char*)obj + vt[2].delta, self, 0x40);
}
#endif

