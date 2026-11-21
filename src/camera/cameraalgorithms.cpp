#include "common.h"
#include "camera/trigger/cameratriggerman.h"
#include "camera/camera.h"
#include "camera/race/racecontroller.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("camera/cameraalgorithms", cChaseCameraController_createChaseAlgorithmBlend);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);
extern char D_0045B250[], D_0045B278[], D_0045B298[], D_0045B2B0[], D_0045B2D8[], str_DEFAULT_4_Chase_Camera_Algorithm[];
extern char D_0045B328[], D_0045B348[], D_0045B370[], D_0045B390[], str_Lazy_Chase_Camera_Algorithm[];
extern char D_0045B3D0[], D_0045B3F8[], D_0045B420[], D_0045B448[], D_0045B470[], D_0045B498[], D_0045B4C0[], str_Direction_NE_Chase_Camera_Algorithm[];
extern char D_0045B510[], D_0045B538[], D_0045B558[], D_0045B578[], str_POST_RACE_1_Chase_Camera_Algorithm[];

struct sCamTmp_D0A0 { float x, y, z, w; } __attribute__((aligned(16)));

class cCamAlgo_D0A0 {
public:
    char pad[0x10];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void activate();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void setTarget(void* target);
};

extern "C" void cCTBoundObjLine_worldPoint1(sCamTmp_D0A0* out, void* target);
extern "C" void cCTBoundObjLine_worldPoint2(sCamTmp_D0A0* out, void* target);
extern "C" void cCTSpline_worldPoint(sCamTmp_D0A0* out, void* target, int corner);
extern "C" cCamAlgo_D0A0* cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm(void* mem, sCamTmp_D0A0* a, sCamTmp_D0A0* b);
extern "C" cCamAlgo_D0A0* cBoundedAlgorithm_cBoundedAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cSplineCamAlgorithm_cSplineCamAlgorithm(void* mem, sCamTmp_D0A0* a, sCamTmp_D0A0* b, sCamTmp_D0A0* c, sCamTmp_D0A0* d,
                               float f0, float f1, float f2, float f3);
extern "C" cCamAlgo_D0A0* cIdleChaseAlgorithm_cIdleChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cMidRightChaseAlgorithm_cMidRightChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cManualChaseAlgorithm_cManualChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cLazyChaseAlgorithm_cLazyChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(void* mem, int index);
extern "C" cCamAlgo_D0A0* cLookBackChaseAlgorithm_cLookBackChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cMatrixChaseAlgorithm_cMatrixChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(void* mem, float angle);
extern "C" cCamAlgo_D0A0* cDEFAULT_2ChaseAlgorithm_cDEFAULT_2ChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cDEFAULT_4ChaseAlgorithm_cDEFAULT_4ChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cSPOKEChaseAlgorithm_cSPOKEChaseAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cPOST_RACE_1ChaseAlgorithm_cPOST_RACE_1ChaseAlgorithm(void* mem, void* target);
extern "C" void cChaseCameraController_addAlgorithm(char* self, cCamAlgo_D0A0* algo, float blend);

static inline int camNetActive_D0A0(int n) { return n != 0 && n < 10; }

/**
 * Creates the chase-camera algorithm for a camera type, aims it at the controller's target and blends it in.
 *
 * Types include idle, mid left and right, manual, lazy, look back, matrix, eight "Direction N" and
 * relative-angle
 * cameras, superpipe (91, falling back to a bounded camera), spline (92) and post-race; most others use
 * DEFAULT_3.
 * Type 67 and unlisted types leave no algorithm and then call through a null pointer.
 *
 * @param kind  the camera algorithm type (On Tour eCamAlgoType type)
 * @param ctx  the camera action that asked for it (On Tour cCTAction* action)
 * @param blend  blend rate; bit 1 of the controller's flags (+0x30) forces an instant switch (1.0)
 */
// Creates the chase-camera algorithm for the given kind and blends it in.
extern "C" void cChaseCameraController_createChaseAlgorithmBlend(cChaseCameraController* self, int kind, char* ctx, float blend)
{
    cCamAlgo_D0A0* algo = 0;
    float blendRate = blend;
    if (*(int*)(((char*)self) + 0x30) & 2) {
        blendRate = 1.0f;
    }
    switch (kind) {
    case 91: {
        int n = *(int*)SSXApp->Game->Replay;
        if (!camNetActive_D0A0(n)) {
            char* target = *(char**)(ctx + 8);
            if (*(int*)(target + 0x28) == 2) {
                void* mem = cMemMan_alloc(944, D_0045B250, 0x20000000, 0);
                sCamTmp_D0A0 lineStart;
                cCTBoundObjLine_worldPoint1(&lineStart, target);
                sCamTmp_D0A0 lineEnd;
                cCTBoundObjLine_worldPoint2(&lineEnd, target);
                algo = cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm(mem, &lineStart, &lineEnd);
                break;
            }
        }
        algo = cBoundedAlgorithm_cBoundedAlgorithm(cMemMan_alloc(144, D_0045B278, 0x20000000, 0));
        *(char**)((char*)algo + 0x80) = ctx;
        break;
    }
    case 92: {
        char* target = *(char**)(ctx + 8);
        void* mem = cMemMan_alloc(976, D_0045B298, 0x20000000, 0);
        sCamTmp_D0A0 point1;
        cCTSpline_worldPoint(&point1, target, 0);
        sCamTmp_D0A0 point2;
        cCTSpline_worldPoint(&point2, target, 1);
        sCamTmp_D0A0 point3;
        cCTSpline_worldPoint(&point3, target, 2);
        sCamTmp_D0A0 point4;
        cCTSpline_worldPoint(&point4, target, 3);
        algo = cSplineCamAlgorithm_cSplineCamAlgorithm(mem, &point1, &point2, &point3, &point4,
                          *(float*)(ctx + 0x18), *(float*)(ctx + 0x14), *(float*)(ctx + 0x1C), *(float*)(ctx + 0x10));
        break;
    }
    case 7:
        algo = cIdleChaseAlgorithm_cIdleChaseAlgorithm(cMemMan_alloc(912, D_0045B328, 0x20000000, 0));
        break;
    case 9:
        algo = cMidRightChaseAlgorithm_cMidRightChaseAlgorithm(cMemMan_alloc(912, D_0045B348, 0x20000000, 0));
        break;
    case 10:
        algo = cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm(cMemMan_alloc(912, D_0045B370, 0x20000000, 0));
        break;
    case 11:
        algo = cManualChaseAlgorithm_cManualChaseAlgorithm(cMemMan_alloc(912, D_0045B390, 0x20000000, 0));
        break;
    case 13:
        algo = cLazyChaseAlgorithm_cLazyChaseAlgorithm(cMemMan_alloc(912, str_Lazy_Chase_Camera_Algorithm, 0x20000000, 0));
        break;
    case 18:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B3D0, 0x20000000, 0), 0);
        break;
    case 19:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B3F8, 0x20000000, 0), 1);
        break;
    case 20:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B420, 0x20000000, 0), 2);
        break;
    case 21:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B448, 0x20000000, 0), 3);
        break;
    case 22:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B470, 0x20000000, 0), 4);
        break;
    case 23:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B498, 0x20000000, 0), 5);
        break;
    case 24:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B4C0, 0x20000000, 0), 6);
        break;
    case 25:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, str_Direction_NE_Chase_Camera_Algorithm, 0x20000000, 0), 7);
        break;
    case 32:
        algo = cLookBackChaseAlgorithm_cLookBackChaseAlgorithm(cMemMan_alloc(912, D_0045B510, 0x20000000, 0));
        break;
    case 41:
        algo = cMatrixChaseAlgorithm_cMatrixChaseAlgorithm(cMemMan_alloc(928, D_0045B538, 0x20000000, 0));
        break;
    case 33: case 34: case 35: case 36: case 37: case 38: case 39: case 40: {
        float angle = (float)(kind - 33) * 0.7853982f;
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), angle);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    }
    case 2: case 60:
        algo = cDEFAULT_2ChaseAlgorithm_cDEFAULT_2ChaseAlgorithm(cMemMan_alloc(912, D_0045B2D8, 0x20000000, 0));
        break;
    case 1: case 62:
        algo = cDEFAULT_4ChaseAlgorithm_cDEFAULT_4ChaseAlgorithm(cMemMan_alloc(912, str_DEFAULT_4_Chase_Camera_Algorithm, 0x20000000, 0));
        break;
    case 0: case 3: case 4: case 5: case 6: case 8: case 12: case 14: case 15: case 16: case 17:
    case 26: case 27: case 28: case 29: case 30: case 31:
    case 42: case 43: case 44: case 45: case 46: case 47: case 48: case 49: case 50:
    case 51: case 52: case 53: case 54: case 55: case 56: case 57: case 58: case 59:
    case 61: case 63: case 64: case 65: case 74:
        algo = cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm(cMemMan_alloc(912, D_0045B2B0, 0x20000000, 0));
        break;
    case 66:
        algo = cSPOKEChaseAlgorithm_cSPOKEChaseAlgorithm(cMemMan_alloc(928, D_0045B578, 0x20000000, 0));
        break;
    case 67:
        algo = 0;
        break;
    case 68:
        algo = cPOST_RACE_1ChaseAlgorithm_cPOST_RACE_1ChaseAlgorithm(cMemMan_alloc(944, str_POST_RACE_1_Chase_Camera_Algorithm, 0x20000000, 0), self->Target);
        break;
    case 94:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), -0.3926991f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    case 95:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), 0.3926991f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    case 96:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), -2.3561945f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    case 97:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), 2.3561945f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    }
    algo->setTarget(self->Target);
    cChaseCameraController_addAlgorithm(((char*)self), algo, blendRate);
    algo->activate();
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cSimpleCamera_cSimpleCamera);
#ifdef SKIP_ASM
struct cSimpleCamera_cSimpleCamera_sVec4 { float x, y, z, w; } __attribute__((aligned(16)));

extern void* cSimpleCamera_vtbl[];
extern cSimpleCamera_cSimpleCamera_sVec4 t4vect_XAxis;

/**
 * Constructs the simple camera base: an identity-ish pose looking down the X axis.
 *
 * Installs cSimpleCamera_vtbl at +0x90, zeroes the pose position (+0x20), sets the pose's second vector (+0x30) to
 * (0, 1, 0, 1) and copies t4vect_XAxis into the direction (+0x80). View, near/far and the matrix are left to init.
 *
 * @return @p self
 */
extern "C" void* cSimpleCamera_cSimpleCamera(cSimpleCamera* self)
{
    cSimpleCamera_cSimpleCamera_sVec4 poseVec;
    self->mVtbl = cSimpleCamera_vtbl;
    poseVec.x = 0; poseVec.y = 0; poseVec.z = 0; poseVec.w = 0;
    *(cSimpleCamera_cSimpleCamera_sVec4*)&self->Pose = poseVec;
    poseVec.w = 1.0f;
    poseVec.x = 0;
    poseVec.y = 1.0f;
    poseVec.z = 0;
    *(cSimpleCamera_cSimpleCamera_sVec4*)&self->Pose.Q = poseVec;
    *(cSimpleCamera_cSimpleCamera_sVec4*)&self->Direction = t4vect_XAxis;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cSimpleCamera_init);
#ifdef SKIP_ASM
extern "C" void* cSimpleCamera_updateCamMat(void* self);

/**
 * Sets up a camera's lens and resets its pose.
 *
 * View, Near and Far and their limits MaxView, MinNear and MaxFar take @p x, @p y and @p z. Pose's position becomes
 * (0, 0, 0, 0) and its quaternion (0, 1, 0, 1), as in cSimpleCamera_cSimpleCamera (not a unit quaternion; the identity
 * is (0, 0, 0, 1)), Direction becomes t4vect_XAxis, and CamMat is rebuilt (cSimpleCamera_updateCamMat).
 *
 * @param x  the field of view, also its maximum
 * @param y  the near distance, also its minimum
 * @param z  the far distance, also its maximum
 */
extern "C" void cSimpleCamera_init(cSimpleCamera* self, float x, float y, float z)
{
    cSimpleCamera_cSimpleCamera_sVec4 poseVec;
    self->MaxView = x;
    self->MinNear = y;
    self->MaxFar = z;
    self->View = x;
    self->Near = y;
    self->Far = z;
    poseVec.x = 0; poseVec.y = 0; poseVec.z = 0; poseVec.w = 0;
    *(cSimpleCamera_cSimpleCamera_sVec4*)&self->Pose = poseVec;
    poseVec.w = 1.0f;
    poseVec.x = 0;
    poseVec.y = 1.0f;
    poseVec.z = 0;
    *(cSimpleCamera_cSimpleCamera_sVec4*)&self->Pose.Q = poseVec;
    *(cSimpleCamera_cSimpleCamera_sVec4*)&self->Direction = t4vect_XAxis;
    cSimpleCamera_updateCamMat(self);
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cSimpleCamera_getHeading);
#ifdef SKIP_ASM
struct cSimpleCamera_getHeading_V { float x, y, z, w; } __attribute__((aligned(16)));

extern "C" float func_0031C228(float);

static inline float atan2_15D780(float y, float x, float zero)
{
    if (x == zero) {
        if (y == zero)
            return y;
        return zero <= y ? 1.5707963705062866f : -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < zero) {
        if (zero < y)
            r += 3.1415927410125732f;
        else
            r -= 3.1415927410125732f;
    }
    return r;
}

/**
 * Returns the camera's heading: atan2 (built on func_0031C228) of the y and x of the axis Pose's quaternion turns y
 * onto.
 *
 * @return the heading (radians)
 */
extern "C" float cSimpleCamera_getHeading(cSimpleCamera* self)
{
    cSimpleCamera_getHeading_V look;
    float x = 2.0f * (self->Pose.Q.y * self->Pose.Q.x - self->Pose.Q.w * self->Pose.Q.z);
    float y = 1.0f - 2.0f * (self->Pose.Q.z * self->Pose.Q.z + self->Pose.Q.x * self->Pose.Q.x);
    float z = 2.0f * (self->Pose.Q.y * self->Pose.Q.z + self->Pose.Q.w * self->Pose.Q.x);
    look.x = x;
    look.y = y;
    look.z = z;
    look.w = 0;
    float w = look.w;
    return atan2_15D780(y, x, w);
}
#endif
//100%
INCLUDE_ASM("camera/cameraalgorithms", cSimpleCamera_getPitch);
#ifdef SKIP_ASM
struct cSimpleCamera_getPitch_V { float x, y, z, w; } __attribute__((aligned(16)));

extern "C" float func_0031C128(float x);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float cSimpleCamera_getPitch_Length(const cSimpleCamera_getPitch_V& v)
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
 * Returns the camera's pitch: the arcsine (func_0031C128) of z over the length of the axis Pose's quaternion turns y
 * onto.
 *
 * @return the pitch (radians)
 */
extern "C" float cSimpleCamera_getPitch(cSimpleCamera* self)
{
    cSimpleCamera_getPitch_V look;
    look.x = 2.0f * (self->Pose.Q.y * self->Pose.Q.x - self->Pose.Q.w * self->Pose.Q.z);
    look.y = 1.0f - 2.0f * (self->Pose.Q.z * self->Pose.Q.z + self->Pose.Q.x * self->Pose.Q.x);
    look.z = 2.0f * (self->Pose.Q.y * self->Pose.Q.z + self->Pose.Q.w * self->Pose.Q.x);
    look.w = 0;
    return func_0031C128(look.z / cSimpleCamera_getPitch_Length(look));
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cSimpleCamera_updateCamMat);
#ifdef SKIP_ASM
struct sV_D928 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sM_D928 {
    float m[4][4];
} __attribute__((aligned(16)));

extern char D_004C53A0[];

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_D928(void* dst, void* src)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, dst = a * b).
static inline void vu0MulMtx_D928(void* dst, void* a, void* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (unit quaternion -> rotation matrix, row 3 = identity).
static inline void vu0QuatToMtx_D928(void* m, const sV_D928& q)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2       $vf4, %1\n"
        "vaddw.xyz  $vf1, $vf0, $vf0w\n"
        "vadd.xyz   $vf5, $vf4, $vf4\n"
        "vsub.w     $vf10, $vf10, $vf10\n"
        "vsub.w     $vf11, $vf11, $vf11\n"
        "vsub.w     $vf12, $vf12, $vf12\n"
        "vmul.xyz   $vf6, $vf5, $vf4\n"
        "vmulw.xyz  $vf7, $vf5, $vf4w\n"
        "vopmula.xyz ACC, $vf5, $vf4\n"
        "vmadd.xyz  $vf8, $vf0, $vf0\n"
        "vsubay.x   ACC, $vf1, $vf6y\n"
        "vmsubz.x   $vf10, $vf1, $vf6z\n"
        "vsubaz.y   ACC, $vf1, $vf6z\n"
        "vmsubx.y   $vf11, $vf1, $vf6x\n"
        "vsubax.z   ACC, $vf1, $vf6x\n"
        "vmsuby.z   $vf12, $vf1, $vf6y\n"
        "vaddaz.y   ACC, $vf0, $vf8z\n"
        "vmaddz.y   $vf10, $vf1, $vf7z\n"
        "vaddax.z   ACC, $vf0, $vf8x\n"
        "vmaddx.z   $vf11, $vf1, $vf7x\n"
        "vaddax.y   ACC, $vf0, $vf8x\n"
        "vmsubx.y   $vf12, $vf1, $vf7x\n"
        "vadday.z   ACC, $vf0, $vf8y\n"
        "vmsuby.z   $vf10, $vf1, $vf7y\n"
        "vaddaz.x   ACC, $vf0, $vf8z\n"
        "vmsubz.x   $vf11, $vf1, $vf7z\n"
        "vadday.x   ACC, $vf0, $vf8y\n"
        "vmaddy.x   $vf12, $vf1, $vf7y\n"
        "sqc2       $vf0, 0x30(%0)\n"
        "sqc2       $vf10, 0x0(%0)\n"
        "sqc2       $vf11, 0x10(%0)\n"
        "sqc2       $vf12, 0x20(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(q)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_D928(void* m, const sV_D928& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "m"(v)
        : "memory");
}

extern "C" void cSimpleCamera_updateCamMat_impl(void* self) __asm__("cSimpleCamera_updateCamMat");
/**
 * Rebuilds CamMat from Pose: the axis-flip matrix D_004C53A0 times the rotation of Pose's quaternion, translated by
 * minus Pose's position (w = 1). Direction is not updated.
 */
extern "C" void cSimpleCamera_updateCamMat_impl(void* self)
{
    char* view = (char*)self + 0x40;
    vu0CopyMtx_D928(view, D_004C53A0);
    sM_D928 rot;
    vu0QuatToMtx_D928(&rot, *(sV_D928*)((char*)self + 0x30));
    vu0MulMtx_D928(view, view, &rot);
    sV_D928 invTranslation;
    sV_D928 neg;
    sV_D928 pos;
    pos.x = *(float*)((char*)self + 0x20);
    pos.y = *(float*)((char*)self + 0x24);
    pos.z = *(float*)((char*)self + 0x28);
    neg.x = -pos.x;
    neg.y = -pos.y;
    neg.z = -pos.z;
    invTranslation.x = neg.x;
    invTranslation.y = neg.y;
    invTranslation.z = neg.z;
    invTranslation.w = 1.0f;
    vu0TransMtx_D928(view, invTranslation);
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cCamera_invalidateSafePosition);
#ifdef SKIP_ASM

/**
 * Marks the first view camera's last safe position as invalid, so it is not used again until re-established.
 *
 * Follows SSXApp->Game's view manager (+0x84) to the first view's camera (+0x4) and sets its +0x4A4 flag (On Tour
 * cCamera::LastSafePositionInvalid) to 1. Only the first view is touched, as in On Tour.
 */
extern "C" void cCamera_invalidateSafePosition(void)
{
    cGame* cam = SSXApp->Game;
    void* chase = cam->ViewMan;
    void* ctrl = *(void**)((char*)chase + 0x4);
    *(int*)((char*)ctrl + 0x4A4) = 1;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cCamera_snapChaseControllers);
#ifdef SKIP_ASM
struct cCamera_snapChaseControllers_VE { short delta; short index; void (*fn)(void*); };

/**
 * Snaps the chase controller of each view's camera.
 *
 * For the first camera of SSXApp->Game->ViewMan, and for the second when the view count (its first word) is at least 2,
 * calls the ChaseController's virtual slot 4 (cCameraController_snapCurrent in cChaseCameraController_vtbl).
 */
extern "C" void cCamera_snapChaseControllers(void)
{
    cGame* cam = SSXApp->Game;
    char* ctx = (char*)cam->ViewMan;
    int count = *(int*)ctx;
    char* firstController = *(char**)(*(char**)(ctx + 4) + 0xA8);
    cCamera_snapChaseControllers_VE* firstSlot = &(*(cCamera_snapChaseControllers_VE**)(firstController + 0x14))[4];
    firstSlot->fn(firstController + firstSlot->delta);
    if (count >= 2) {
        char* secondController = *(char**)(*(char**)(ctx + 8) + 0xA8);
        cCamera_snapChaseControllers_VE* secondSlot = &(*(cCamera_snapChaseControllers_VE**)(secondController + 0x14))[4];
        secondSlot->fn(secondController + secondSlot->delta);
    }
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cCamera_onRestart);
#ifdef SKIP_ASM
extern "C" void cCameraTriggerMan_initOnRestart(void* self);
void cCameraTriggerMan_setInGameTriggers(void* self);
extern "C" void cChaseCameraController_switchCamera_Restart(void* cam);

struct sCamVE_0015DB58 {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Resets the cameras after a restart: re-arms the camera triggers and restarts each view's chase camera.
 *
 * Calls cCameraTriggerMan_initOnRestart and cCameraTriggerMan_setInGameTriggers on gCamTriggerMan, then for the first view (and the
 * second when the view manager holds two or more): cChaseCameraController_switchCamera_Restart on its chase
 * controller (+0xA8), the controller's snapCurrent virtual (slot 4) and clears +0x460. Only the first view's
 * safe-position-invalid flag (+0x4A4) is set.
 */
extern "C" void cCamera_onRestart(void)
{
    cCameraTriggerMan_initOnRestart(((char*)&gCamTriggerMan));
    cCameraTriggerMan_setInGameTriggers(((char*)&gCamTriggerMan));
    char* riders = (char*)SSXApp->Game->ViewMan;
    int n = *(int*)riders;
    cChaseCameraController_switchCamera_Restart(*(void**)(*(char**)(riders + 4) + 0xA8));
    char* cam = *(char**)(*(char**)(riders + 4) + 0xA8);
    sCamVE_0015DB58* snapEntry = &(*(sCamVE_0015DB58**)(cam + 0x14))[4];
    snapEntry->fn(cam + snapEntry->delta);
    *(int*)(*(char**)(riders + 4) + 0x460) = 0;
    *(int*)(*(char**)(riders + 4) + 0x4A4) = 1;
    if (n >= 2)
    {
        cChaseCameraController_switchCamera_Restart(*(void**)(*(char**)(riders + 8) + 0xA8));
        char* cam2 = *(char**)(*(char**)(riders + 8) + 0xA8);
        sCamVE_0015DB58* snapEntry2 = &(*(sCamVE_0015DB58**)(cam2 + 0x14))[4];
        snapEntry2->fn(cam2 + snapEntry2->delta);
        *(int*)(*(char**)(riders + 8) + 0x460) = 0;
    }
}
#endif

