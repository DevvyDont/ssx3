#include "common.h"
#include "main/gameviewman.h"
#include "camera/trigger/cameratriggeractions.h"
#include "bx/math3d.h"
#include "vtable.h"
#include "camera/trigger/cameratriggerman.h"
#include "camera/trigger/cameratriggervolumes.h"
#include "camera/camera.h"
#include "camera/race/racecontroller.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("camera/cameraalgorithms", cChaseCameraController_createChaseAlgorithmBlend);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cMemMan_alloc(unsigned int size, const char* tag, int flags, int d);
extern char str_SUPERPIPE_Chase_Camera_Algorithm[], D_0045B278[], D_0045B298[], D_0045B2B0[], D_0045B2D8[], str_DEFAULT_4_Chase_Camera_Algorithm[];
extern char str_Idle_Chase_Camera_Algorithm[], D_0045B348[], D_0045B370[], D_0045B390[], str_Lazy_Chase_Camera_Algorithm[];
extern char D_0045B3D0[], D_0045B3F8[], D_0045B420[], D_0045B448[], D_0045B470[], D_0045B498[], D_0045B4C0[], str_Direction_NE_Chase_Camera_Algorithm[];
extern char D_0045B510[], D_0045B538[], D_0045B558[], D_0045B578[], str_POST_RACE_1_Chase_Camera_Algorithm[];


class cCamAlgo_D0A0 {
public:
    char pad[0x10];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void activate();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void setTarget(void* target);
};

extern "C" void cCTBoundObjLine_worldPoint1(t4Vector* out, void* target);
extern "C" void cCTBoundObjLine_worldPoint2(t4Vector* out, void* target);
extern "C" void cCTSpline_worldPoint(t4Vector* out, void* target, int corner);
extern "C" cCamAlgo_D0A0* cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm(void* mem, t4Vector* a, t4Vector* b);
extern "C" cCamAlgo_D0A0* cBoundedAlgorithm_cBoundedAlgorithm(void* mem);
extern "C" cCamAlgo_D0A0* cSplineCamAlgorithm_cSplineCamAlgorithm(void* mem, t4Vector* a, t4Vector* b, t4Vector* c, t4Vector* d,
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
 * Types include idle, mid left and right, manual, lazy, look back, matrix, eight "Direction N" and relative-angle
 * cameras, CAMERA_ALGORITHM_TRIGGER_BOUNDED (a superpipe camera along an eCTBoundObjLine target, else a bounded
 * camera), CAMERA_ALGORITHM_TRIGGER_SPLINE and post-race; most others use DEFAULT_3. CAMERA_ALGORITHM_CHASE_SUPERPIPE
 * and unlisted types leave no algorithm and then call through a null pointer.
 *
 * @param kind  the camera algorithm type (On Tour eCamAlgoType type)
 * @param ctx  the camera action that asked for it (On Tour cCTAction* action)
 * @param blend  blend rate; bit 1 of the controller's flags (+0x30) forces an instant switch (1.0)
 */
// Creates the chase-camera algorithm for the given kind and blends it in.
void cChaseCameraController::createChaseAlgorithmBlend(int kind, char* ctx, float blend)
{
    cCamAlgo_D0A0* algo = 0;
    float blendRate = blend;
    if (CamSwitchRecord.IsInReplay) {
        blendRate = 1.0f;
    }
    switch (kind) {
    case CAMERA_ALGORITHM_TRIGGER_BOUNDED: {
        int n = *(int*)SSXApp->Game->Replay;
        if (!camNetActive_D0A0(n)) {
            char* target = *(char**)(ctx + 8);
            if (*(int*)(target + 0x28) == eCTBoundObjLine) {
                void* mem = cMemMan_alloc(944, str_SUPERPIPE_Chase_Camera_Algorithm, 0x20000000, 0);
                t4Vector lineStart;
                cCTBoundObjLine_worldPoint1(&lineStart, target);
                t4Vector lineEnd;
                cCTBoundObjLine_worldPoint2(&lineEnd, target);
                algo = cSUPERPIPEChaseAlgorithm_cSUPERPIPEChaseAlgorithm(mem, &lineStart, &lineEnd);
                break;
            }
        }
        algo = cBoundedAlgorithm_cBoundedAlgorithm(cMemMan_alloc(144, D_0045B278, 0x20000000, 0));
        *(char**)((char*)algo + 0x80) = ctx;
        break;
    }
    case CAMERA_ALGORITHM_TRIGGER_SPLINE: {
        char* target = *(char**)(ctx + 8);
        void* mem = cMemMan_alloc(976, D_0045B298, 0x20000000, 0);
        t4Vector point1;
        cCTSpline_worldPoint(&point1, target, 0);
        t4Vector point2;
        cCTSpline_worldPoint(&point2, target, 1);
        t4Vector point3;
        cCTSpline_worldPoint(&point3, target, 2);
        t4Vector point4;
        cCTSpline_worldPoint(&point4, target, 3);
        algo = cSplineCamAlgorithm_cSplineCamAlgorithm(mem, &point1, &point2, &point3, &point4,
                          *(float*)(ctx + 0x18), *(float*)(ctx + 0x14), *(float*)(ctx + 0x1C), *(float*)(ctx + 0x10));
        break;
    }
    case CAMERA_ALGORITHM_CHASE_IDLE:
        algo = cIdleChaseAlgorithm_cIdleChaseAlgorithm(cMemMan_alloc(912, str_Idle_Chase_Camera_Algorithm, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_MID_RIGHT:
        algo = cMidRightChaseAlgorithm_cMidRightChaseAlgorithm(cMemMan_alloc(912, D_0045B348, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_MID_LEFT:
        algo = cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm(cMemMan_alloc(912, D_0045B370, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_MANUAL:
        algo = cManualChaseAlgorithm_cManualChaseAlgorithm(cMemMan_alloc(912, D_0045B390, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_LAZY:
        algo = cLazyChaseAlgorithm_cLazyChaseAlgorithm(cMemMan_alloc(912, str_Lazy_Chase_Camera_Algorithm, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_DIRECTION_N:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B3D0, 0x20000000, 0), 0);
        break;
    case CAMERA_ALGORITHM_CHASE_DIRECTION_NW:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B3F8, 0x20000000, 0), 1);
        break;
    case CAMERA_ALGORITHM_CHASE_DIRECTION_W:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B420, 0x20000000, 0), 2);
        break;
    case CAMERA_ALGORITHM_CHASE_DIRECTION_SW:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B448, 0x20000000, 0), 3);
        break;
    case CAMERA_ALGORITHM_CHASE_DIRECTION_S:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B470, 0x20000000, 0), 4);
        break;
    case CAMERA_ALGORITHM_CHASE_DIRECTION_SE:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B498, 0x20000000, 0), 5);
        break;
    case CAMERA_ALGORITHM_CHASE_DIRECTION_E:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, D_0045B4C0, 0x20000000, 0), 6);
        break;
    case CAMERA_ALGORITHM_CHASE_DIRECTION_NE:
        algo = cDirectionChaseAlgorithm_cDirectionChaseAlgorithm(cMemMan_alloc(928, str_Direction_NE_Chase_Camera_Algorithm, 0x20000000, 0), 7);
        break;
    case CAMERA_ALGORITHM_CHASE_LOOKBACK:
        algo = cLookBackChaseAlgorithm_cLookBackChaseAlgorithm(cMemMan_alloc(912, D_0045B510, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_MATRIX:
        algo = cMatrixChaseAlgorithm_cMatrixChaseAlgorithm(cMemMan_alloc(928, D_0045B538, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_RELATIVE_FRONT:
    case CAMERA_ALGORITHM_CHASE_RELATIVE_FRONT_LEFT:
    case CAMERA_ALGORITHM_CHASE_RELATIVE_LEFT:
    case CAMERA_ALGORITHM_CHASE_RELATIVE_REAR_LEFT:
    case CAMERA_ALGORITHM_CHASE_RELATIVE_REAR:
    case CAMERA_ALGORITHM_CHASE_RELATIVE_REAR_RIGHT:
    case CAMERA_ALGORITHM_CHASE_RELATIVE_RIGHT:
    case CAMERA_ALGORITHM_CHASE_RELATIVE_FRONT_RIGHT: {
        float angle = (float)(kind - CAMERA_ALGORITHM_CHASE_RELATIVE_FRONT) * 0.7853982f;
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), angle);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    }
    case CAMERA_ALGORITHM_CHASE_NEAR: case CAMERA_ALGORITHM_CHASE_DEFAULT_2:
        algo = cDEFAULT_2ChaseAlgorithm_cDEFAULT_2ChaseAlgorithm(cMemMan_alloc(912, D_0045B2D8, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_FAR: case CAMERA_ALGORITHM_CHASE_DEFAULT_4:
        algo = cDEFAULT_4ChaseAlgorithm_cDEFAULT_4ChaseAlgorithm(cMemMan_alloc(912, str_DEFAULT_4_Chase_Camera_Algorithm, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_MID:
    case CAMERA_ALGORITHM_CHASE_FIRST_PERSON:
    case CAMERA_ALGORITHM_CHASE_HALFPIPE:
    case CAMERA_ALGORITHM_CHASE_TIGHT:
    case CAMERA_ALGORITHM_CHASE_SLOPE_STYLE:
    case CAMERA_ALGORITHM_CHASE_TRICKY:
    case CAMERA_ALGORITHM_CHASE_MID_FOV:
    case CAMERA_ALGORITHM_CHASE_STUBBORN:
    case CAMERA_ALGORITHM_CHASE_VERY_FAR:
    case CAMERA_ALGORITHM_CHASE_BOOT_LEFT:
    case CAMERA_ALGORITHM_CHASE_BOOT_RIGHT:
    case CAMERA_ALGORITHM_CHASE_PAUL_STUBBORN:
    case CAMERA_ALGORITHM_CHASE_LARRY_STUBBORN:
    case CAMERA_ALGORITHM_CHASE_DYNAMIC_LARRY_STUBBORN:
    case CAMERA_ALGORITHM_CHASE_DYNAMIC_PAUL_STUBBORN:
    case CAMERA_ALGORITHM_CHASE_DYNAMIC_STUBBORN:
    case CAMERA_ALGORITHM_CHASE_HIGH_STUBBORN:
    case CAMERA_ALGORITHM_CHASE_ARA1:
    case CAMERA_ALGORITHM_CHASE_ASS1:
    case CAMERA_ALGORITHM_CHASE_ABA1:
    case CAMERA_ALGORITHM_CHASE_ABC1:
    case CAMERA_ALGORITHM_CHASE_BRA2:
    case CAMERA_ALGORITHM_CHASE_BHP1:
    case CAMERA_ALGORITHM_CHASE_CRA3:
    case CAMERA_ALGORITHM_CHASE_CHP2:
    case CAMERA_ALGORITHM_CHASE_CBA2:
    case CAMERA_ALGORITHM_CHASE_DRA4:
    case CAMERA_ALGORITHM_CHASE_DBC2:
    case CAMERA_ALGORITHM_CHASE_DSS2:
    case CAMERA_ALGORITHM_CHASE_ERA5:
    case CAMERA_ALGORITHM_CHASE_ESS3:
    case CAMERA_ALGORITHM_CHASE_EBA3:
    case CAMERA_ALGORITHM_CHASE_EHP3:
    case CAMERA_ALGORITHM_CHASE_EBC3:
    case CAMERA_ALGORITHM_CHASE_DEFAULT_1:
    case CAMERA_ALGORITHM_CHASE_DEFAULT_3:
    case CAMERA_ALGORITHM_CHASE_DEFAULT_5:
    case CAMERA_ALGORITHM_CHASE_SAFE_LOW:
    case CAMERA_ALGORITHM_CHASE_SAFE_HIGH:
    case CAMERA_ALGORITHM_CHASE_REPLAY:
        algo = cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm(cMemMan_alloc(912, D_0045B2B0, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_SPOKE:
        algo = cSPOKEChaseAlgorithm_cSPOKEChaseAlgorithm(cMemMan_alloc(928, D_0045B578, 0x20000000, 0));
        break;
    case CAMERA_ALGORITHM_CHASE_SUPERPIPE:
        algo = 0;
        break;
    case CAMERA_ALGORITHM_CHASE_POST_RACE_1:
        algo = cPOST_RACE_1ChaseAlgorithm_cPOST_RACE_1ChaseAlgorithm(cMemMan_alloc(944,
                str_POST_RACE_1_Chase_Camera_Algorithm, 0x20000000, 0), Target);
        break;
    case CAMERA_ALGORITHM_REPLAY_FRONT_RIGHT:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), -0.3926991f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    case CAMERA_ALGORITHM_REPLAY_FRONT_LEFT:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), 0.3926991f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    case CAMERA_ALGORITHM_REPLAY_REAR_RIGHT:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), -2.3561945f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    case CAMERA_ALGORITHM_REPLAY_REAR_LEFT:
        algo = cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cMemMan_alloc(0x3C0, D_0045B558, 0x20000000, 0), 2.3561945f);
        *(int*)((char*)algo + 0xC) = kind;
        break;
    }
    algo->setTarget(Target);
    cChaseCameraController_addAlgorithm(((char*)this), algo, blendRate);
    algo->activate();
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cSimpleCamera_cSimpleCamera);
#ifdef SKIP_ASM

extern void* cSimpleCamera_vtbl[];
extern t4Vector t4vect_XAxis;

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
    t4Vector poseVec;
    self->mVtbl = cSimpleCamera_vtbl;
    poseVec.x = 0; poseVec.y = 0; poseVec.z = 0; poseVec.w = 0;
    *(t4Vector*)&self->Pose = poseVec;
    poseVec.w = 1.0f;
    poseVec.x = 0;
    poseVec.y = 1.0f;
    poseVec.z = 0;
    *(t4Vector*)&self->Pose.Q = poseVec;
    *(t4Vector*)&self->Direction = t4vect_XAxis;
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
void cSimpleCamera::init(float x, float y, float z)
{
    t4Vector poseVec;
    MaxView = x;
    MinNear = y;
    MaxFar = z;
    View = x;
    Near = y;
    Far = z;
    poseVec.x = 0; poseVec.y = 0; poseVec.z = 0; poseVec.w = 0;
    *(t4Vector*)&Pose = poseVec;
    poseVec.w = 1.0f;
    poseVec.x = 0;
    poseVec.y = 1.0f;
    poseVec.z = 0;
    *(t4Vector*)&Pose.Q = poseVec;
    *(t4Vector*)&Direction = t4vect_XAxis;
    cSimpleCamera_updateCamMat(this);
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cSimpleCamera_getHeading);
#ifdef SKIP_ASM

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
float cSimpleCamera::getHeading()
{
    t4Vector look;
    float x = 2.0f * (Pose.Q.y * Pose.Q.x - Pose.Q.w * Pose.Q.z);
    float y = 1.0f - 2.0f * (Pose.Q.z * Pose.Q.z + Pose.Q.x * Pose.Q.x);
    float z = 2.0f * (Pose.Q.y * Pose.Q.z + Pose.Q.w * Pose.Q.x);
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

extern "C" float func_0031C128(float x);

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float cSimpleCamera_getPitch_Length(const t4Vector& v)
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
float cSimpleCamera::getPitch()
{
    t4Vector look;
    look.x = 2.0f * (Pose.Q.y * Pose.Q.x - Pose.Q.w * Pose.Q.z);
    look.y = 1.0f - 2.0f * (Pose.Q.z * Pose.Q.z + Pose.Q.x * Pose.Q.x);
    look.z = 2.0f * (Pose.Q.y * Pose.Q.z + Pose.Q.w * Pose.Q.x);
    look.w = 0;
    return func_0031C128(look.z / cSimpleCamera_getPitch_Length(look));
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cSimpleCamera_updateCamMat);
#ifdef SKIP_ASM


extern char CamAxisFlip[];

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
static inline void vu0QuatToMtx_D928(void* m, const t4Vector& q)
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
static inline void vu0TransMtx_D928(void* m, const t4Vector& v)
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
 * Rebuilds CamMat from Pose: the axis-flip matrix CamAxisFlip times the rotation of Pose's quaternion, translated by
 * minus Pose's position (w = 1). Direction is not updated.
 */
extern "C" void cSimpleCamera_updateCamMat_impl(void* vself)
{
    cSimpleCamera* self = (cSimpleCamera*)vself;
    char* view = (char*)&self->CamMat;
    vu0CopyMtx_D928(view, CamAxisFlip);
    t4Matrix rot;
    vu0QuatToMtx_D928(&rot, self->Pose.Q);
    vu0MulMtx_D928(view, view, &rot);
    t4Vector invTranslation;
    t4Vector neg;
    t4Vector pos;
    pos.x = self->Pose.P.x;
    pos.y = self->Pose.P.y;
    pos.z = self->Pose.P.z;
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
 * Follows SSXApp->Game's view manager to the first view's camera (PlayerCamera[0]) and sets its
 * LastSafePositionInvalid flag to 1. Only the first view is touched, as in On Tour.
 */
extern "C" void cCamera_invalidateSafePosition(void)
{
    cGame* cam = SSXApp->Game;
    cGameViewMan* chase = cam->ViewMan;
    cCamera* camera = chase->PlayerCamera[0];
    camera->LastSafePositionInvalid = 1;
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cCamera_snapChaseControllers);
#ifdef SKIP_ASM

/**
 * Snaps the chase controller of each view's camera.
 *
 * For the first camera of SSXApp->Game->ViewMan, and for the second when its m_uiNumPlayers is at least 2,
 * calls the ChaseController's virtual slot 4 (cCameraController_snapCurrent in cChaseCameraController_vtbl).
 */
extern "C" void cCamera_snapChaseControllers(void)
{
    cGame* cam = SSXApp->Game;
    cGameViewMan* ctx = cam->ViewMan;
    int count = ctx->m_uiNumPlayers;
    cChaseCameraController* firstController = ctx->PlayerCamera[0]->ChaseController;
    sVEntry<void(void*)>* firstVtbl = (sVEntry<void(void*)>*)firstController->mVtbl;
    sVEntry<void(void*)>* firstSlot = &firstVtbl[4];
    firstSlot->fn(vthis(firstController, *firstSlot));
    if (count >= 2) {
        cChaseCameraController* secondController = ctx->PlayerCamera[1]->ChaseController;
        sVEntry<void(void*)>* secondVtbl = (sVEntry<void(void*)>*)secondController->mVtbl;
        sVEntry<void(void*)>* secondSlot = &secondVtbl[4];
        secondSlot->fn(vthis(secondController, *secondSlot));
    }
}
#endif

//100%
INCLUDE_ASM("camera/cameraalgorithms", cCamera_onRestart);
#ifdef SKIP_ASM
extern "C" void cCameraTriggerMan_initOnRestart(void* self);
void cCameraTriggerMan_setInGameTriggers(void* self);
extern "C" void cChaseCameraController_switchCamera_Restart(void* cam);


/**
 * Resets the cameras after a restart: re-arms the camera triggers and restarts each view's chase camera.
 *
 * Calls cCameraTriggerMan_initOnRestart and cCameraTriggerMan_setInGameTriggers on gCamTriggerMan, then for the first view (and the
 * second when the view manager holds two or more): cChaseCameraController_switchCamera_Restart on its chase
 * controller (ChaseController), the controller's snapCurrent virtual (slot 4) and clears LastGroundMove. Only the
 * first view's LastSafePositionInvalid flag is set.
 */
extern "C" void cCamera_onRestart(void)
{
    cCameraTriggerMan_initOnRestart(((char*)&gCamTriggerMan));
    cCameraTriggerMan_setInGameTriggers(((char*)&gCamTriggerMan));
    cGameViewMan* riders = SSXApp->Game->ViewMan;
    int n = riders->m_uiNumPlayers;
    cChaseCameraController_switchCamera_Restart(riders->PlayerCamera[0]->ChaseController);
    cChaseCameraController* cam = riders->PlayerCamera[0]->ChaseController;
    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)cam->mVtbl;
    sVEntry<void(void*)>* snapEntry = &vt[4];
    snapEntry->fn(vthis(cam, *snapEntry));
    *(int*)&riders->PlayerCamera[0]->LastGroundMove = 0;
    riders->PlayerCamera[0]->LastSafePositionInvalid = 1;
    if (n >= 2)
    {
        cChaseCameraController_switchCamera_Restart(riders->PlayerCamera[1]->ChaseController);
        cChaseCameraController* cam2 = riders->PlayerCamera[1]->ChaseController;
        sVEntry<void(void*)>* vt2 = (sVEntry<void(void*)>*)cam2->mVtbl;
        sVEntry<void(void*)>* snapEntry2 = &vt2[4];
        snapEntry2->fn(vthis(cam2, *snapEntry2));
        *(int*)&riders->PlayerCamera[1]->LastGroundMove = 0;
    }
}
#endif

