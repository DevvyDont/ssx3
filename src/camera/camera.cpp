#include "common.h"
#include "ai/riderhandler.h"
#include "ai/airpredictor.h"
#include "main/gameviewman.h"
#include "world/worldcache.h"
#include "camera/cameracontroller.h"
#include "camera/cameraalgolist.h"
#include "ai/rider.h"
#include "camera/race/racealgorithms.h"
#include "camera/camerajitterer.h"
#include "camera/cameraalgorithms.h"
#include "camera/camera.h"
#include "camera/race/racecontroller.h"
#include "world/world.h"
#include "main/game.h"
#include "main/ssxapp.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("camera/camera", cCamera_resetChaseControllerSwitches);
#ifdef SKIP_ASM
struct sCam162138;
extern "C" void cChaseCameraController_switchCamera_SoftReset(cChaseCameraController* self);

/**
 * Soft-resets the camera-switch state of the chase controller in each view.
 *
 * Reads the game's view manager (cGame::ViewMan). It resets the first view's camera, and also the second
 * when the manager's first word is 2 or more (probably the split-screen view count). Each reset is
 * cChaseCameraController_switchCamera_SoftReset() on that camera's ChaseController. Does nothing without a
 * view manager.
 */
extern "C" void cCamera_resetChaseControllerSwitches(void)
{
    cGame* cam = SSXApp->Game;
    int* chase = (int*)cam->ViewMan;
    if (chase != 0) {
        cChaseCameraController_switchCamera_SoftReset((cChaseCameraController*)(*(sCam162138**)(*(char**)((char*)chase + 4) + 0xA8)));
        if (*chase >= 2) {
            cChaseCameraController_switchCamera_SoftReset((cChaseCameraController*)(*(sCam162138**)(*(char**)((char*)chase + 8) + 0xA8)));
        }
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_cCamera);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cSimpleCamera_cSimpleCamera(void* self);
extern "C" void* cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm(void* self);
extern "C" void* cManualCameraController_cManualCameraController(void* self, void* cam);
extern "C" void* cChaseCameraController_cChaseCameraController(cChaseCameraController* self, int idx, int a);
extern "C" void* cScriptCameraController_cScriptCameraController(void* self, int idx, void* cam);
extern void* cCamera_vtbl[];
extern const char str_Manual_Cam_Controller[];
extern const char str_Chase_Cam_Controller[];
extern const char str_Script_Cam_Controller[];
// PORT: cQuad128 (the unit's 128-bit TImode typedef) is a 16-byte register copy.
extern cQuad128 t4vect_ZAxis;
extern cQuad128 t4vect_Origin;

/**
 * Constructs a rider's game camera with its three controllers.
 *
 * Runs the cSimpleCamera constructor, sets cCamera_vtbl, builds ResultAlgorithm (cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm) and allocates
 * the manual (0x20), chase (0x44, for the view) and script (0x20) controllers. StreamFocusID starts at -1;
 * the shake, ground-avoidance and safe-position state is reset (LastSafePositionInvalid = 1).
 *
 * @param idx  view index, passed to the chase and script controllers
 * @return self
 */
extern "C" void* cCamera_cCamera(cCamera* self, int idx)
{
    cSimpleCamera_cSimpleCamera(self);
    self->mVtbl = cCamera_vtbl;
    cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm((char*)&self->ResultAlgorithm);
    self->ManualController = (cManualCameraController*)cManualCameraController_cManualCameraController(cMemMan_alloc(0x20, str_Manual_Cam_Controller, 0, 0), self);
    self->ChaseController = (cChaseCameraController*)cChaseCameraController_cChaseCameraController((cChaseCameraController*)cMemMan_alloc(68, str_Chase_Cam_Controller, 0, 0), idx, (int)self);
    self->ScriptController = (cScriptCameraController*)cScriptCameraController_cScriptCameraController(cMemMan_alloc(0x20, str_Script_Cam_Controller, 0, 0), idx, self);
    self->StreamFocusID = -1;
    self->bStreamIsReady = 0;
    *(int*)&self->MaxShakeVariance = 0;
    self->DoingCamShake = 0;
    self->DoingCamShakeLastFrame = 0;
    self->DominantCamShakeType = 0;
    *(int*)&self->LastGroundMove = 0;
    *(cQuad128*)&self->LastGroundNormal = t4vect_ZAxis;
    *(cQuad128*)&self->LastSafePosition = t4vect_Origin;
    self->LastSafePositionInvalid = 1;
    self->UsingSafePositionLastFrame = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera__cCamera);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cCamera_vtbl[];
extern void* cSimpleCamera_vtbl[];
void cCameraController_clearAlgorithms(void* p);
void cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm_2(void* self, int flags) __asm__("cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm__FPv");

class cCamPartK2 {
public:
    char pad[0x14];
    // vptr at 0x14
    virtual ~cCamPartK2();
};

/**
 * Destroys a game camera: frees its controllers and its result algorithm.
 *
 * Clears each controller's algorithm list (cCameraController_clearAlgorithms) and deletes it, destroys ResultAlgorithm, restores
 * cSimpleCamera_vtbl and frees the camera when bit 0 of flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 = free the memory
 */
// PORT: cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm__FPv is a destructor taking (self, flags); bind the 2-arg form.
extern "C" void cCamera__cCamera(cCamera* self, int flags)
{
    self->mVtbl = cCamera_vtbl;
    cCameraController_clearAlgorithms((cCamPartK2*)self->ManualController);
    cCameraController_clearAlgorithms((cCamPartK2*)self->ChaseController);
    cCameraController_clearAlgorithms((cCamPartK2*)self->ScriptController);
    delete (cCamPartK2*)self->ManualController;
    delete (cCamPartK2*)self->ChaseController;
    delete (cCamPartK2*)self->ScriptController;
    cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm_2((char*)&self->ResultAlgorithm, 2);
    self->mVtbl = cSimpleCamera_vtbl;
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_init);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc is the game's tagged operator new(size, tag, flags, d); bound by asm label.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void cManualCameraController_setCurrentAlgorithmType(void* self, int msg);
extern "C" void cChaseCameraController_setTarget(cChaseCameraController* self, int* p);
struct sCam162060;
extern "C" void cChaseCameraController_switchCamera_Volume(cChaseCameraController* self, int mode, int arg, float blend);
void cChaseAlgorithm_setTarget(void* self, int val);
void* cCamera_snapResultAlgorithm(void* self);
extern const char str_RiderCameraTarget[];
extern void* cRiderCameraTarget_vtbl[];

struct sCamCtrl_15DE60
{
    char pad_0x00[0xC];
    int viewport;   // 0xC
};

struct sCamera_15DE60
{
    char pad_0x00[0xC];
    float f0C;      // 0xC
    float f10;      // 0x10
    float f14;      // 0x14
    int index;      // 0x18
    char pad_0x1C[0xA4 - 0x1C];
    sCamCtrl_15DE60* manual;   // 0xA4
    sCamCtrl_15DE60* chase;    // 0xA8
    sCamCtrl_15DE60* script;   // 0xAC
    int fB0;        // 0xB0
    int fB4;        // 0xB4
    char pad_0xB8[0x454 - 0xB8];
    int f454;       // 0x454
    int f458;       // 0x458
    int f45C;       // 0x45C
    int f460;       // 0x460
};

struct sCamWorld_15DE60
{
    char pad_0x00[0xC];
    char* riders;   // 0xC
};

struct sApp_15DE60
{
    char pad_0x00[0x84];
    sCamWorld_15DE60* world;   // 0x84
    char pad_0x88[0xB0 - 0x88];
    int viewports[4];          // 0xB0
};


/**
 * Sets up the camera for one view: limits, controllers, rider target and stream focus.
 *
 * The float arguments are misnamed: MIPS EABI passes them in order, so nearPlane is stored as MaxView,
 * farPlane as MinNear and riderIndex as MaxFar (On Tour: init(float v, float nearPlane, float farPlane, int
 * riderIndex)).
 *
 * Gives the manual and script controllers input 0 and the chase controller the view's input, selects the
 * manual algorithm 0x51, gives the chase controller and ResultAlgorithm a new cRiderCameraTarget on the
 * view's rider, switches the chase camera to the player's choice (0x4C), snaps the result and uses stream
 * focus 0 or 1.
 *
 * @param riderIndex  view index (also picks the rider and the stream focus)
 * @param view  stored as MaxView (the maximum field of view; see above)
 * @param nearPlane  stored as MinNear
 * @param farPlane  stored as MaxFar
 */
extern "C" void cCamera_init(sCamera_15DE60* self, int riderIndex, float view, float nearPlane, float farPlane)
{
    self->f0C = view;
    self->f10 = nearPlane;
    self->f14 = farPlane;
    self->index = riderIndex;
    self->f460 = 0;
    self->manual->viewport = ((sApp_15DE60*)SSXApp)->viewports[0];
    cManualCameraController_setCurrentAlgorithmType(self->manual, 81);
    self->chase->viewport = ((sApp_15DE60*)SSXApp)->viewports[riderIndex];
    cRiderCameraTarget* riderRef = (cRiderCameraTarget*)operator new(8, str_RiderCameraTarget, 0, 0);
    riderRef->mVtbl = cRiderCameraTarget_vtbl;
    (*(int*)&riderRef->Rider) = *(int*)(((sApp_15DE60*)SSXApp)->world->riders + (riderIndex << 2) + 0x28);
    cChaseCameraController_setTarget((cChaseCameraController*)self->chase, (int*)riderRef);
    cChaseCameraController_switchCamera_Volume((cChaseCameraController*)((sCam162060*)self->chase), 76, 0, 1.0f);
    cChaseAlgorithm_setTarget((char*)self + 0xC0, (int)riderRef);
    cCamera_snapResultAlgorithm(self);
    self->f454 = 0;
    self->f458 = 0;
    self->f45C = 0;
    switch (riderIndex) {
    case 0:
        self->fB0 = 0;
        break;
    case 1:
        self->fB0 = 1;
        break;
    }
    self->fB4 = 0;
    self->script->viewport = ((sApp_15DE60*)SSXApp)->viewports[0];
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_update);
#ifdef SKIP_ASM
class cCamVObj_0015DF98 {
public:
    char pad[0x14];
    // vptr at 0x14
    virtual void v01();
    virtual void update();
};

extern "C" void* cCamera_updateCamMat(void* self);

/**
 * Updates the current controller, then the camera itself.
 *
 * Calls the current controller's virtual slot 2 (update), then cCamera_updateCamMat (unknown).
 */
extern "C" void cCamera_update(cCamera* self)
{
    ((cCamVObj_0015DF98*)self->Controller)->update();
    cCamera_updateCamMat(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_setCurrentController);
#ifdef SKIP_ASM
/**
 * Selects which controller drives the camera.
 *
 * 1 = manual, 2 = chase, 3 = script; any other value leaves Controller unchanged.
 *
 * @param mode  controller type (On Tour eCamControllerType)
 */
extern "C" void cCamera_setCurrentController(cCamera* self, int mode)
{
    switch (mode) {
    case 1:
        self->Controller = (cCameraController*)((int)self->ManualController);
        break;
    case 2:
        self->Controller = (cCameraController*)((int)self->ChaseController);
        break;
    case 3:
        self->Controller = (cCameraController*)((int)self->ScriptController);
        break;
    }
}
#endif

extern "C" void* cLookAtChaseAlgorithm_snapCurrent(cLookAtChaseAlgorithm*);

//100%
INCLUDE_ASM("camera/camera", cCamera_snapResultAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Snaps the camera's blended result straight to where it should be, skipping the transition.
 *
 * cLookAtChaseAlgorithm_snapCurrent on the result algorithm embedded at +0xC0.
 *
 * @return cLookAtChaseAlgorithm_snapCurrent's result
 */
void* cCamera_snapResultAlgorithm(void* vself)
{
    cCamera* self = (cCamera*)vself;
    return cLookAtChaseAlgorithm_snapCurrent((cLookAtChaseAlgorithm*)(&self->ResultAlgorithm));
}
#endif

//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_setupCamWithPosAndTarget);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// A transform as position + rotation quaternion, returned by value (tPose_tPose builds one from a matrix).
struct sXf {
    t4Vector pos;
    t4Vector rot;
};

extern t4Matrix CamAxisFlip_E050 __asm__("CamAxisFlip");


extern "C" void* cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm(void* self);
extern "C" void cLookAtChaseAlgorithm_calcPitchHeading(void* self, int a1);
extern "C" void cLookAtChaseAlgorithm_calcPose(void* self);
extern "C" sXf cLookAtChaseAlgorithm_getPose_E050(void* a1) __asm__("cLookAtChaseAlgorithm_getPose");
void cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm_E050(void* self, int flags) __asm__("cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm__FPv");

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_E050(t4Matrix* dst, const t4Matrix* src)
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

// PORT: PS2-only VU0 inline asm (unit quaternion -> 3x3 rotation, row 3 = (0,0,0,1)).
static inline void vu0QuatToMtx_E050(t4Matrix* m, const t4Vector* q)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2       $vf4, 0x0(%1)\n"
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
        : "r"(m), "r"(q)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in place, m = m * b).
static inline void vu0MulEqMtx_E050(t4Matrix* m, const t4Matrix* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
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
        : "r"(m), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_E050(t4Matrix* m, const t4Vector& v)
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

extern "C" void cSimpleCamera_setupCamWithPosAndTarget_impl(char* self, char* a1, char* a2) __asm__("cSimpleCamera_setupCamWithPosAndTarget");
/**
 * Points a camera from a position at a target point.
 *
 * Constructs a temporary cDEFAULT_3ChaseAlgorithm on the stack, sets its Position to @p camPos and its CamTargetPos to
 * @p camTargetPos (both with w = 1), computes its pitch and heading (cLookAtChaseAlgorithm_calcPitchHeading with 1) and pose (cLookAtChaseAlgorithm_calcPose), and stores
 * cLookAtChaseAlgorithm_getPose's result in Pose. CamMat becomes the axis-flip matrix CamAxisFlip times the rotation of
 * Pose's quaternion, translated by minus Pose's position; Direction becomes the z column of its rotation rows (w = 0).
 * The temporary is destroyed with cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm.
 *
 * @param camPos  the camera position
 * @param camTargetPos  the point to look at
 */
extern "C" void cSimpleCamera_setupCamWithPosAndTarget_impl(char* vself, char* camPos, char* camTargetPos)
{
    cSimpleCamera* self = (cSimpleCamera*)vself;
    cLookAtChaseAlgorithm lookAtAlgo;
    cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm(&lookAtAlgo);
    *(cQuad128*)&(&lookAtAlgo)->CamTargetPos = *(cQuad128*)camTargetPos;
    *(cQuad128*)&(&lookAtAlgo)->Position = *(cQuad128*)camPos;
    (&lookAtAlgo)->Position.w = 1.0f;
    (&lookAtAlgo)->CamTargetPos.w = 1.0f;
    cLookAtChaseAlgorithm_calcPitchHeading(&lookAtAlgo, 1);
    cLookAtChaseAlgorithm_calcPose(&lookAtAlgo);
    sXf out = cLookAtChaseAlgorithm_getPose_E050(&lookAtAlgo);
    t4Vector* poseQuat = (t4Vector*)(&self->Pose.Q);
    t4Matrix* camMatrix = (t4Matrix*)(&self->CamMat);
    *(sXf*)&self->Pose = out;
    vu0CopyMtx_E050(camMatrix, &CamAxisFlip_E050);
    t4Matrix rot;
    vu0QuatToMtx_E050(&rot, poseQuat);
    vu0MulEqMtx_E050(camMatrix, &rot);
    t4Vector posePos;
    posePos.x = self->Pose.P.x;
    posePos.y = self->Pose.P.y;
    posePos.z = self->Pose.P.z;
    out.rot.x = -posePos.x;
    out.rot.y = -posePos.y;
    out.rot.z = -posePos.z;
    out.pos.x = -posePos.x;
    out.pos.y = -posePos.y;
    out.pos.z = -posePos.z;
    out.pos.w = 1.0f;
    vu0TransMtx_E050(camMatrix, out.pos);
    out.pos.x = self->CamMat.row[0].z;
    out.pos.y = self->CamMat.row[1].z;
    out.pos.z = self->CamMat.row[2].z;
    out.pos.w = 0.0f;
    self->Direction = out.pos;
    cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm_E050(&lookAtAlgo, 2);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_0015E2A8);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sCamLag_15E2A8 {
    float f0, f4, f8, fC;
    struct {
        float a, b, c, d;
    } group[3];             // 0x10
    float f40, f44, f48, f4C;
};
extern sCamLag_15E2A8 kCamShakeValues;
extern int kCamShakeDependsOnDistance[5];

/**
 * Fills the camera-shake table: five 16-byte entries at kCamShakeValues, one per shake type, and their flags kCamShakeDependsOnDistance.
 *
 * An entry holds the minimum time step and the time variance (passed to cLookAtChaseAlgorithm_setupCameraShake and
 * cLookAtChaseAlgorithm_updateCameraShake), the weight cCamera_shakeCamera scales a request by, and the distance at
 * which a shake fades out (-1: no fade). Type 0: (0.05, 0.01, 0.4, 20000), flag 0; types 1-3 (the boost shakes
 * cCamera_applyCameraShake requests): (0.02, 0.01366, 0.07 / 0.08 / 0.09, -1), flag 1; type 4: (0.1, 0.03, 1.4, -1),
 * flag 1. The flag is cLookAtChaseAlgorithm_updateCameraShake's second argument. Called only by the unit's static
 * initializer func_001673F8.
 */
extern "C" void func_0015E2A8(void)
{
    kCamShakeValues.f0 = 0.05f;
    kCamShakeValues.f4 = 0.01f;
    kCamShakeValues.f8 = 0.4f;
    kCamShakeValues.fC = 20000.0f;
    kCamShakeDependsOnDistance[0] = 0;
    kCamShakeValues.group[0].a = 0.02f;
    kCamShakeValues.group[0].b = 0.01366f;
    kCamShakeValues.group[0].c = 0.07f;
    kCamShakeValues.group[0].d = -1.0f;
    kCamShakeDependsOnDistance[1] = 1;
    kCamShakeValues.group[1].a = 0.02f;
    kCamShakeValues.group[1].b = 0.01366f;
    kCamShakeValues.group[1].c = 0.08f;
    kCamShakeValues.group[1].d = -1.0f;
    kCamShakeDependsOnDistance[2] = 1;
    kCamShakeValues.group[2].a = 0.02f;
    kCamShakeValues.group[2].b = 0.01366f;
    kCamShakeValues.group[2].c = 0.09f;
    kCamShakeValues.group[2].d = -1.0f;
    kCamShakeDependsOnDistance[3] = 1;
    kCamShakeValues.f40 = 0.1f;
    kCamShakeValues.f44 = 0.03f;
    kCamShakeValues.f48 = 1.4f;
    kCamShakeValues.f4C = -1.0f;
    kCamShakeDependsOnDistance[4] = 1;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_shakeCamera);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sCamShake_E360 {
    float f0;
    float f4;
    float weight;   // 0x8
    float range;    // 0xC
};
// View of the tuning block initialised by func_0015E2A8 as one entry per shake type.
extern sCamShake_E360 kCamShakeValues_E360[] __asm__("kCamShakeValues");

#define MIN_E360(a, b) ((a) <= (b) ? (a) : (b))
#define MAX_E360(a, b) ((b) <= (a) ? (a) : (b))

/**
 * Requests a camera shake for this frame, keeping the strongest request.
 *
 * The amount is scaled by the type's weight and by distance: full strength up to half the type's range,
 * fading to 0 at the range (no fade when either is 0). Types 1-4 are ignored during a replay.
 *
 * @param type  shake type, index into the shake table kCamShakeValues
 * @param amount  requested strength
 * @param dist  distance from the shake source
 */
// Requests a camera shake of 'type' with strength 'amount' at 'dist' from the camera; keeps the strongest request.
// Replays ignore types 1-4.
extern "C" void cCamera_shakeCamera(cCamera* self, int type, float amount, float dist)
{
    int state = *(int*)SSXApp->Game->Replay;
    int replay = state != 0 && state < 10;
    if (replay && (unsigned int)(type - 1) < 4) {
        return;
    }
    float scale;
    float range = kCamShakeValues_E360[type].range;
    if (0.0f < dist && 0.0f < range) {
        float t = 2.0f - (dist + dist) / range;
        scale = MAX_E360(MIN_E360(t, 1.0f), 0.0f);
    } else {
        scale = 1.0f;
    }
    float variance = amount * scale * kCamShakeValues_E360[type].weight;
    if (self->MaxShakeVariance < variance) {
        self->DominantCamShakeType = type;
        self->MaxShakeVariance = variance;
    }
    self->DoingCamShake = 1;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_applyCameraShake);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

#include "vtable.h"

// The camera base matrix (set up by func_001673F8), followed by the camera-shake tuning
// table: one entry per shake type, reset by func_0015E2A8, plus each type's selector in kCamShakeDependsOnDistance.
struct sCamShake {
    float base;     // passed to cLookAtChaseAlgorithm_setupCameraShake_E460 / cLookAtChaseAlgorithm_updateCameraShake
    float spread;
    float weight;   // scales the requested amount
    float range;    // distance at which the shake fades out
};

extern sCamShake kCamShakeValues_E460[5] __asm__("kCamShakeValues");

extern int kCamShakeDependsOnDistance[5];

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_15E460(const t4Vector& v)
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

extern int cPauseState_Flags;
extern "C" void cLookAtChaseAlgorithm_updateCameraShake(cLookAtChaseAlgorithm* self, int flag, int sel, float base, float spread, float r);
extern "C" void cLookAtChaseAlgorithm_setupCameraShake_E460(char* self, float base, float spread, float r) __asm__("cLookAtChaseAlgorithm_setupCameraShake");
extern "C" void cCamera_shakeCamera_E460(char* self, int type, float amount, float dist) __asm__("cCamera_shakeCamera");

/**
 * Adds the boost shake and applies this frame's dominant camera shake to the result algorithm.
 *
 * While bit 0 of the pause-mode flags cPauseState_Flags is clear and the target rider's mBoostStrength is over 0.1,
 * requests shake type 1, 2 or 3 (boost over 0.4 or 0.7) with a strength from the target's vector at slot 3
 * (probably the velocity): 0 below 60, rising to 1 at 120 after scaling by 0.036.
 *
 * Starts the shake (cLookAtChaseAlgorithm_setupCameraShake) on the first frame, runs cLookAtChaseAlgorithm_updateCameraShake with the dominant type's table
 * values, then clears the per-frame request.
 */
extern "C" void cCamera_applyCameraShake(cCamera* self)
{
    char* tgt = (char*)self->ChaseController->Target;
    sVEntry<float(void*)>* vf = *(sVEntry<float(void*)>**)tgt;
    float spd = vf[17].fn(vthis(tgt, vf[17]));
    if (((cPauseState_Flags & 1) ^ 1) != 0 && spd > 0.10000000149011612f) {
        sVEntry<t4Vector(void*)>* vv = *(sVEntry<t4Vector(void*)>**)tgt;
        t4Vector velocity = vv[3].fn(vthis(tgt, vv[3]));
        float speedKmh = Length_15E460(velocity) * 0.035999998450279236f;
        float k;
        if (60.0f <= speedKmh) {
            if (speedKmh <= 120.0f)
                k = (speedKmh - 60.0f) / 60.0f;
            else
                k = 1.0f;
        } else {
            k = 0.0f;
        }
        if (spd > 0.699999988079071f)
            cCamera_shakeCamera_E460(((char*)self), 3, k, 0.0f);
        else if (spd > 0.4000000059604645f)
            cCamera_shakeCamera_E460(((char*)self), 2, k, 0.0f);
        else
            cCamera_shakeCamera_E460(((char*)self), 1, k, 0.0f);
    }
    int idx = self->DominantCamShakeType;
    float shakeBase = kCamShakeValues_E460[idx].base;
    float timeVariance = kCamShakeValues_E460[idx].spread;
    int sel = kCamShakeDependsOnDistance[idx];
    char* resultAlgorithm = (char*)&self->ResultAlgorithm;
    float spatialVariance = self->MaxShakeVariance;
    if (self->DoingCamShake != 0 && self->DoingCamShakeLastFrame == 0)
        cLookAtChaseAlgorithm_setupCameraShake_E460(resultAlgorithm, shakeBase, timeVariance, spatialVariance);
    cLookAtChaseAlgorithm_updateCameraShake((cLookAtChaseAlgorithm*)resultAlgorithm, self->DoingCamShake, sel, shakeBase, timeVariance, spatialVariance);
    self->DoingCamShakeLastFrame = self->DoingCamShake;
    self->DoingCamShake = 0;
    *(int*)&self->MaxShakeVariance = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_updateCamMat);
#ifdef SKIP_ASM
// cCamera::updateCamMat(void) (SSX On Tour PSP symbols: all_in_one_camera.obj / camera.cpp).
#include "bx/math3d.h"

extern "C" void cCamera_avoidGround(void*);                                   // cCamera::avoidGround
extern void cCamera_applyCameraShake_E668(void*) __asm__("cCamera_applyCameraShake");         // cCamera::applyCameraShake
extern "C" void cLookAtChaseAlgorithm_calcPitchHeading(void* self, int a1);                      // cLookAtChaseAlgorithm::calcPitchHeading(bool)
extern "C" void cLookAtChaseAlgorithm_calcPose(void* self);                              // cLookAtChaseAlgorithm::calcPose
extern "C" void cRenderStateMan_update(int, float, float, float);                // cRenderStateMan::update
extern "C" float cRenderStateMan_DrawDistance(int);                                    // cRenderStateMan::DrawDistance
extern t4Vector t4vect_Zero_E668 __asm__("t4vect_Zero");                  // t4vect::Zero
extern t4Vector t4vect_Origin_E668 __asm__("t4vect_Origin");                  // t4vect::Origin
extern t4Matrix CamAxisFlip_E668 __asm__("CamAxisFlip");                  // CamAxisFlip
extern float cRenderStateMan_m_farPlane;                                                // cRenderStateMan::m_farPlane

// EA's quaternion: the ctor takes w first.
struct tQuaternion_E668 {
    float x, y, z, w;
    tQuaternion_E668() {}
    tQuaternion_E668(const float& aw, const float& ax, const float& ay, const float& az) { w = aw; x = ax; y = ay; z = az; }
} __attribute__((aligned(16)));

struct tPose_E668 {
    t4Vector pos;
    tQuaternion_E668 orient;
    void setIdentity() { orient = tQuaternion_E668(1.0f, 0.0f, 0.0f, 0.0f); pos = t4vect_Origin_E668; }
};

struct t3Vector_E668 {
    float x, y, z;
    t3Vector_E668() {}
    t3Vector_E668(float ax, float ay, float az) { x = ax; y = ay; z = az; }
    t3Vector_E668(const t4Vector& v) { x = v.x; y = v.y; z = v.z; }
    t3Vector_E668 operator-() const { return t3Vector_E668(-x, -y, -z); }
};

// t4Vector with EA's constructors (bx/math3d.h's t4Vector is a POD view).
struct t4Vec_E668 : t4Vector {
    t4Vec_E668(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
    t4Vec_E668(const t3Vector_E668& v, const float& aw) { x = v.x; y = v.y; z = v.z; w = aw; }
};

extern "C" tPose_E668 cLookAtChaseAlgorithm_getPose_E668(void* sub) __asm__("cLookAtChaseAlgorithm_getPose");   // cLookAtChaseAlgorithm::getPose

struct cCameraAlgorithm_E668 {
    float fov;      // 0x0
    float nearZ;    // 0x4
    float farZ;     // 0x8
    int type;       // 0xC
    virtual void v0();
    virtual tPose_E668 getPose();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual t4Vector getTarget();
};


struct cCameraAlgoList_E668 {
    cCameraAlgoListNode* head;
    int length;
};

struct cCameraController_E668 {
    int pad[2];
    cCameraAlgoList_E668* list;
};


// PORT: PS2-only VU0 inline asm (scalar * vector).
static inline t4Vector operator*(float s, const t4Vector& v)
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
static inline t4Vector operator+(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void CopyMtx_E668(t4Matrix* dst, const t4Matrix* src)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in place, m = m * b).
static inline void MulEqMtx_E668(t4Matrix* m, const t4Matrix* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
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
        : "r"(m), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (unit quaternion -> rotation matrix, row 3 = identity).
static inline void QuatToMtx_E668(const tQuaternion_E668* q, t4Matrix* m)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2       $vf4, 0x0(%1)\n"
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
        : "r"(m), "r"(q)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void Translate_E668(t4Matrix* m, const t4Vector& v)
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

static inline float bracket_E668(float lo, float v, float hi)
{
    return (lo <= v) ? (v <? hi) : lo; // PORT: g++ <? operator
}

extern "C" void cCamera_updateCamMat_E668(cCamera* self) __asm__("cCamera_updateCamMat");
/**
 * Computes the camera's pose, matrix, direction and lens from its current Controller's algorithm list.
 *
 * With one algorithm in the list, or a first algorithm of AlgorithmType 74, Pose is that algorithm's pose (virtual slot
 * 2, e.g. cLookAtChaseAlgorithm_getPose); when Controller is the ChaseController, ResultAlgorithm's Position and
 * CamTargetPos also take that position and the algorithm's target (slot 6). Otherwise every algorithm whose blend
 * weight (list node +0xC) is over 0.0001 is blended by running weight into ResultAlgorithm's Position and CamTargetPos,
 * and ResultAlgorithm's pitch and heading (cLookAtChaseAlgorithm_calcPitchHeading with 1) and pose (cLookAtChaseAlgorithm_calcPose,
 * cLookAtChaseAlgorithm_getPose) become Pose.
 *
 * For the chase controller it then keeps the eye out of the ground (cCamera_avoidGround), applies the camera shake
 * (cCamera_applyCameraShake) and recomputes Pose from ResultAlgorithm (cLookAtChaseAlgorithm_calcPitchHeading with 0). CamMat becomes the axis
 * flip CamAxisFlip times the rotation of Pose's quaternion, translated by minus Pose's position; Direction its z column.
 * View, Near and Far are blended from the algorithms by lens weight (list node +0x4). Last, cRenderStateMan_update runs
 * for view ViewIndex + 6 at the camera's x/y, Far is capped by that view's draw distance (cRenderStateMan_DrawDistance) with MinNear
 * as the floor and stored in cRenderStateMan_m_farPlane (the render far plane), and View is clamped to [0, MaxView] and Near and Far to
 * [MinNear, MaxFar].
 */
extern "C" void cCamera_updateCamMat_E668(cCamera* self)
{
    cCameraAlgoListNode* algoNode;
    tPose_E668 tempPose;
    float totalWeight = 0.0f;

    (*(tPose_E668*)&self->Pose).setIdentity();
    algoNode = ((cCameraController_E668*)self->Controller)->list->head;
    cCameraAlgorithm_E668* algo = ((cCameraAlgorithm_E668*)algoNode->Algorithm);
    bool listLengthOne = ((cCameraController_E668*)self->Controller)->list->length == 1;
    if (listLengthOne || algo->type == 74) {
        (*(tPose_E668*)&self->Pose) = algo->getPose();
        if (((cCameraController_E668*)self->Controller) == ((cCameraController_E668*)self->ChaseController)) {
            self->ResultAlgorithm.Position = (*(tPose_E668*)&self->Pose).pos;
            self->ResultAlgorithm.CamTargetPos = algo->getTarget();
        }
    } else {
        self->ResultAlgorithm.CamTargetPos = t4vect_Zero_E668;
        self->ResultAlgorithm.Position = t4vect_Zero_E668;
        for (algoNode = ((cCameraController_E668*)self->Controller)->list->head; algoNode; algoNode = algoNode->Next) {
            if (algoNode->SmoothWeight > 0.0001f) {
                totalWeight += algoNode->SmoothWeight;
                float weightFactor = algoNode->SmoothWeight / totalWeight;
                tempPose = ((cCameraAlgorithm_E668*)algoNode->Algorithm)->getPose();
                t4Vector tempPosePos = tempPose.pos;
                t4Vector tempAlgoPos = self->ResultAlgorithm.Position;
                self->ResultAlgorithm.Position = weightFactor * tempPosePos + (1.0f - weightFactor) * tempAlgoPos;
                t4Vector algoTargetPos = ((cCameraAlgorithm_E668*)algoNode->Algorithm)->getTarget();
                t4Vector tempAlgoTargetPos = self->ResultAlgorithm.CamTargetPos;
                self->ResultAlgorithm.CamTargetPos = weightFactor * algoTargetPos + (1.0f - weightFactor) * tempAlgoTargetPos;
            }
        }
        self->ResultAlgorithm.Position.w = 1.0f;
        self->ResultAlgorithm.CamTargetPos.w = 1.0f;
        cLookAtChaseAlgorithm_calcPitchHeading(((char*)&self->ResultAlgorithm), 1);
        cLookAtChaseAlgorithm_calcPose(((char*)&self->ResultAlgorithm));
        (*(tPose_E668*)&self->Pose) = cLookAtChaseAlgorithm_getPose_E668(((char*)&self->ResultAlgorithm));
    }
    if (((cCameraController_E668*)self->Controller) == ((cCameraController_E668*)self->ChaseController)) {
        cCamera_avoidGround(self);
        cCamera_applyCameraShake_E668(self);
        cLookAtChaseAlgorithm_calcPitchHeading(((char*)&self->ResultAlgorithm), 0);
        cLookAtChaseAlgorithm_calcPose(((char*)&self->ResultAlgorithm));
        (*(tPose_E668*)&self->Pose) = cLookAtChaseAlgorithm_getPose_E668(((char*)&self->ResultAlgorithm));
    }
    CopyMtx_E668(&self->CamMat, &CamAxisFlip_E668);
    t4Matrix quaternionMat;
    QuatToMtx_E668(&(*(tPose_E668*)&self->Pose).orient, &quaternionMat);
    MulEqMtx_E668(&self->CamMat, &quaternionMat);
    Translate_E668(&self->CamMat, t4Vec_E668(-t3Vector_E668((*(tPose_E668*)&self->Pose).pos.x, (*(tPose_E668*)&self->Pose).pos.y, (*(tPose_E668*)&self->Pose).pos.z), 1.0f));
    self->Direction = t4Vec_E668(self->CamMat.row[0].z, self->CamMat.row[1].z, self->CamMat.row[2].z, 0.0f);
    self->View = 0.0f;
    self->Near = 0.0f;
    self->Far = 0.0f;
    totalWeight = 0.0f;
    for (algoNode = ((cCameraController_E668*)self->Controller)->list->head; algoNode; algoNode = algoNode->Next) {
        if (algoNode->Weight != 0.0f) {
            totalWeight += algoNode->Weight;
            float weightFactor = algoNode->Weight / totalWeight;
            self->View = weightFactor * ((cCameraAlgorithm_E668*)algoNode->Algorithm)->fov + (1.0f - weightFactor) * self->View;
            self->Near = weightFactor * ((cCameraAlgorithm_E668*)algoNode->Algorithm)->nearZ + (1.0f - weightFactor) * self->Near;
            self->Far = weightFactor * ((cCameraAlgorithm_E668*)algoNode->Algorithm)->farZ + (1.0f - weightFactor) * self->Far;
        }
    }
    cRenderStateMan_update(self->ViewIndex + 6, (*(tPose_E668*)&self->Pose).pos.x, (*(tPose_E668*)&self->Pose).pos.y, -99999.0f);
    self->Far = bracket_E668(self->MinNear, self->Far, cRenderStateMan_DrawDistance(self->ViewIndex + 6));
    cRenderStateMan_m_farPlane = self->Far;
    self->View = bracket_E668(0.0f, self->View, self->MaxView);
    self->Near = bracket_E668(self->MinNear, self->Near, self->MaxFar);
    self->Far = bracket_E668(self->MinNear, self->Far, self->MaxFar);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_updateStreamFocus);
#ifdef SKIP_ASM

extern "C" void cWorldView_setFocus(void* self, unsigned int i, t4Vector* v, float f);
extern "C" int cWorldView_getFocus(void* self, int i, t4Vector* outPos, float* outT);

struct sCacheSlot_15EC98
{
    int active;
    char pad[0x4C];
};

struct sWorld_15EC98
{
    char pad[0x250];
    sCacheSlot_15EC98 slots[2];
};

// PORT: g++ min operator (<?), removed in GCC 4.3.
static inline float clamp_15EC98(float v, float lo, float hi)
{
    float r;
    if (v >= lo)
        r = v <? hi;
    else
        r = lo;
    return r;
}

/**
 * Moves the camera's stream focus to its position and checks whether streaming has caught up.
 *
 * The radius is Far (clamped to 15000 unless the view manager's word at +0x10 is 1); the focus is set at the
 * camera position with 1.5 x radius. bStreamIsReady becomes 1 when the world cache's focus distance reaches
 * the radius or 2/3 of it clamped to 15000-30000.
 */
extern "C" void cCamera_updateStreamFocus(cCamera* self)
{
    float viewDist;
    if ((int)SSXApp->Game->ViewMan->m_uiNumView == 1)
        viewDist = self->Far;
    else
        viewDist = clamp_15EC98(self->Far, 0.0f, 15000.0f);
    float streamRadius = viewDist * 1.5f;
    float far = clamp_15EC98(viewDist * 0.6666666865348816f, 15000.0f, 30000.0f);
    cWorld* sys = cWorld_m_pTheWorld;
    ((sWorld_15EC98*)(char*)sys->m_pWorldCache)->slots[self->StreamFocusID].active = 1;
    cWorldView_setFocus(&sys->m_pWorldCache->m_WorldView, self->StreamFocusID, (t4Vector*)(&self->Pose), streamRadius);
    self->bStreamIsReady = 0;
    t4Vector pos;
    float focusDist;
    if (cWorldView_getFocus(&sys->m_pWorldCache->m_WorldView, self->StreamFocusID, &pos, &focusDist)) {
        if (far <= focusDist || viewDist <= focusDist)
            self->bStreamIsReady = 1;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_switchChaseControllerTargetRider);
#ifdef SKIP_ASM
class cCamVObj_0015EDC8 {
public:
    char pad[0x14];
    // vptr at 0x14 (g++ 2.95 places it after the class's own data)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void snapCurrent();
};

/**
 * Points the chase controller's rider target at another rider.
 *
 * Stores the value as the target's Rider (+0x4) and calls the chase controller's virtual slot 4.
 *
 * @param value  the rider to follow (On Tour: cRider *)
 */
extern "C" void cCamera_switchChaseControllerTargetRider(cCamera* self, int value)
{
    cChaseCameraController* obj = self->ChaseController;
    obj->Target->Rider = (cRider*)value;
    ((cCamVObj_0015EDC8*)self->ChaseController)->snapCurrent();
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCamera_avoidGround);
#ifdef SKIP_ASM
struct sVec_EE00 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// Probe hit record filled by cBXNash_terrainIntersect1 (0x80 bytes).
struct sHit_EE00 {
    sVec_EE00 pos;      // 0x0
    sVec_EE00 normal;   // 0x10
    char pad_0x20[0x60];
} __attribute__((aligned(16)));

extern sVec_EE00 t4vect_ZAxis_vEE00 __asm__("t4vect_ZAxis");
extern sVec_EE00 t4vect_XAxis_vEE00 __asm__("t4vect_XAxis");
// cWorldLine::cWorldLine(const t4Vector&, const t4Vector&, float, tIntPrecision)
extern "C" void* cWorldLine_cWorldLine(void* seg, const sVec_EE00& a, const sVec_EE00& b, float r, int n);
extern "C" float cBXNash_terrainIntersect1(void* world, void* seg, sHit_EE00* out, int a3);
extern "C" int cChaseCameraController_shouldDoExtraGroundTest(void* self);

static inline const sVec_EE00& Forward_EE00()
{
    return t4vect_XAxis_vEE00;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_EE00 Scale_EE00(const sVec_EE00& v, float s)
{
    sVec_EE00 r;
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_EE00(sVec_EE00& v, float s)
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
static inline sVec_EE00 Add_EE00(const sVec_EE00& a, const sVec_EE00& b)
{
    sVec_EE00 r;
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

// PORT: PS2-only VU0 inline asm (in-place v += a).
static inline void AddEq_EE00(sVec_EE00& v, const sVec_EE00& a)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v)
        : "m"(a)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sVec_EE00 Sub_EE00(const sVec_EE00& a, const sVec_EE00& b)
{
    sVec_EE00 r;
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
static inline sVec_EE00 Cross_EE00(const sVec_EE00& a, const sVec_EE00& b)
{
    sVec_EE00 r;
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
static inline float Dot_EE00(const sVec_EE00& a, const sVec_EE00& b)
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
static inline float Length_EE00(const sVec_EE00& v)
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

// PORT: sqrt.s via inline asm.
static inline float Sqrt_EE00(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: abs.s via inline asm.
static inline float Abs_EE00(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// Third-person follow camera: probes beside the look-at line to raise/lower the camera height
// (self+0x460), keeps the eye-target distance, then probes eye->target and moves the eye (ResultAlgorithm's Position,
// +0x100) relative to the target (+0xE0) when it would end up inside geometry.
// cCamera::avoidGround(void) (SSX On Tour symbols). PORT: g++ `>?` (max) operator.
extern "C" void cCamera_avoidGround_impl(char* self) __asm__("cCamera_avoidGround");
/**
 * Keeps the chase camera's eye (ResultAlgorithm's Position, +0x100) out of the ground and walls.
 *
 * Ground test: a vertical line through the eye raised by LastGroundMove, 300 / s above and below it (s = the sine of
 * the look direction's slope, at least 0.05) and moved 50 to the side away from LastGroundNormal, is tested against
 * the world SSXApp->Game->BXNash (cWorldLine_cWorldLine builds the line, cBXNash_terrainIntersect1 tests it with IntersectionHint). On a
 * hit, LastGroundMove is raised for an upward-facing hit normal and lowered for a downward one: by 50 / s minus the
 * hit's distance from the eye when the eye is on the side the normal faces, else by that distance plus 50 / s
 * (nothing when that is over 150); LastGroundNormal takes the normal. The eye is then raised by LastGroundMove, put
 * back at its old distance from the target (CamTargetPos, +0xE0), and LastGroundMove decays by 0.97.
 *
 * Extra test, when cChaseCameraController_shouldDoExtraGroundTest on ChaseController is nonzero: a line from 100 above the target to the eye. On a hit
 * the eye moves to LastSafePosition (to target + 180 up + 180 along x when LastSafePositionInvalid), is pushed 50
 * along the hit normal on the first such frame (UsingSafePositionLastFrame), and goes to target + 150 up + 100 along x
 * if it is then within 2 of the target (or within 1 in both x and y); LastSafePosition takes the eye. Finally
 * LastSafePositionInvalid is cleared.
 */
extern "C" void cCamera_avoidGround_impl(char* vself)
{
    cCamera* self = (cCamera*)vself;
    sHit_EE00 info;
    void* nash = SSXApp->Game->BXNash;
    sVec_EE00 correctedPos = Add_EE00(*(sVec_EE00*)&self->ResultAlgorithm.Position, Scale_EE00(t4vect_ZAxis_vEE00, self->LastGroundMove));
    sVec_EE00 look = Sub_EE00(*(sVec_EE00*)&self->ResultAlgorithm.CamTargetPos, correctedPos);
    float lookLen = Length_EE00(look);
    sVec_EE00 levelLook = look;
    levelLook.z = 0.0f;
    float levelLookLen = Length_EE00(levelLook);
    float sinSlopeRaw = levelLookLen / Sqrt_EE00(levelLookLen * levelLookLen + lookLen * lookLen);
    float originalLookLen = Length_EE00(Sub_EE00(*(sVec_EE00*)&self->ResultAlgorithm.CamTargetPos, *(sVec_EE00*)&self->ResultAlgorithm.Position));
    sVec_EE00 testRoot = Add_EE00(*(sVec_EE00*)&self->ResultAlgorithm.Position, Scale_EE00(t4vect_ZAxis_vEE00, self->LastGroundMove));
    float slopeScalar = 1.0f / (sinSlopeRaw >? 0.05000000074505806f);
    float testLen = slopeScalar * 300.0f;
    sVec_EE00 whisker = Scale_EE00(t4vect_ZAxis_vEE00, testLen);
    float avoidDist = slopeScalar * 50.0f;
    sVec_EE00 testLow = Sub_EE00(testRoot, whisker);
    sVec_EE00 testHigh = Add_EE00(testRoot, whisker);
    sVec_EE00 screenLeft = Cross_EE00(look, t4vect_ZAxis_vEE00);
    float dir = 1.0f;
    if (Dot_EE00(*(sVec_EE00*)&self->LastGroundNormal, screenLeft) > 0.0f)
        dir = -1.0f;
    ScaleEq_EE00(screenLeft, dir * 50.0f / Length_EE00(screenLeft));
    char line[176];
    cWorldLine_cWorldLine(line, Add_EE00(testHigh, screenLeft), Add_EE00(testLow, screenLeft), 0.5f, 2);
    float hitFraction = cBXNash_terrainIntersect1(nash, line, &info, (int)(&self->IntersectionHint[0]));
    if (hitFraction > 0.0f) {
        float normZ = info.normal.z;
        float move;
        if (hitFraction > 0.5f) {
            float hitDistance = testLen * (hitFraction + hitFraction - 1.0f);
            if (normZ > 0.0f) {
                move = 0.0f;
                if (hitDistance < avoidDist)
                    move = avoidDist - hitDistance;
                self->LastGroundMove += move;
            } else {
                move = hitDistance + avoidDist;
                if (move > 150.0f)
                    move = 0.0f;
                self->LastGroundMove -= move;
            }
        } else {
            float hitDistance = testLen * (1.0f - (hitFraction + hitFraction));
            if (normZ > 0.0f) {
                move = hitDistance + avoidDist;
                if (move > 150.0f)
                    move = 0.0f;
                self->LastGroundMove += move;
            } else {
                move = 0.0f;
                if (hitDistance < avoidDist)
                    move = avoidDist - hitDistance;
                self->LastGroundMove -= move;
            }
        }
        *(sVec_EE00*)&self->LastGroundNormal = info.normal;
    }
    AddEq_EE00(*(sVec_EE00*)&self->ResultAlgorithm.Position, Scale_EE00(t4vect_ZAxis_vEE00, self->LastGroundMove));
    sVec_EE00 newLook = Sub_EE00(*(sVec_EE00*)&self->ResultAlgorithm.CamTargetPos, *(sVec_EE00*)&self->ResultAlgorithm.Position);
    ScaleEq_EE00(newLook, originalLookLen / Length_EE00(newLook));
    *(sVec_EE00*)&self->ResultAlgorithm.Position = Sub_EE00(*(sVec_EE00*)&self->ResultAlgorithm.CamTargetPos, newLook);
    self->LastGroundMove = self->LastGroundMove * 0.9700000286102295f;
    if (cChaseCameraController_shouldDoExtraGroundTest(self->ChaseController) != 0) {
        sVec_EE00 riderPoint = Add_EE00(*(sVec_EE00*)&self->ResultAlgorithm.CamTargetPos, Scale_EE00(t4vect_ZAxis_vEE00, 100.0f));
        sVec_EE00 viewPoint = *(sVec_EE00*)&self->ResultAlgorithm.Position;
        char line2[176];
        cWorldLine_cWorldLine(line2, riderPoint, viewPoint, 0.5f, 2);
        sHit_EE00 info2;
        if (cBXNash_terrainIntersect1(SSXApp->Game->BXNash, line2, &info2, 0) > 0.0f) {
            if (self->LastSafePositionInvalid != 0)
                *(sVec_EE00*)&self->ResultAlgorithm.Position = Add_EE00(Add_EE00(*(sVec_EE00*)&self->ResultAlgorithm.CamTargetPos, Scale_EE00(t4vect_ZAxis_vEE00, 180.0f)), Scale_EE00(Forward_EE00(), 180.0f));
            else
                *(sVec_EE00*)&self->ResultAlgorithm.Position = *(sVec_EE00*)&self->LastSafePosition;
            if (self->UsingSafePositionLastFrame == 0)
                AddEq_EE00(*(sVec_EE00*)&self->ResultAlgorithm.Position, Scale_EE00(info2.normal, 50.0f));
            self->UsingSafePositionLastFrame = 1;
            sVec_EE00 look2 = Sub_EE00(*(sVec_EE00*)&self->ResultAlgorithm.CamTargetPos, *(sVec_EE00*)&self->ResultAlgorithm.Position);
            if (Length_EE00(look2) < 2.0f || (Abs_EE00(look2.x) < 1.0f && Abs_EE00(look2.y) < 1.0f))
                *(sVec_EE00*)&self->ResultAlgorithm.Position = Add_EE00(Add_EE00(*(sVec_EE00*)&self->ResultAlgorithm.CamTargetPos, Scale_EE00(t4vect_ZAxis_vEE00, 150.0f)), Scale_EE00(Forward_EE00(), 100.0f));
        } else {
            self->UsingSafePositionLastFrame = 0;
        }
        *(sVec_EE00*)&self->LastSafePosition = *(sVec_EE00*)&self->ResultAlgorithm.Position;
    } else {
        self->UsingSafePositionLastFrame = 0;
    }
    self->LastSafePositionInvalid = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getPosition);
#ifdef SKIP_ASM
extern "C" void* cRider_getHipPosition(void* dst, void* self);

/**
 * Returns the camera target's position: the rider's hips.
 *
 * cRider_getHipPosition of the rider at +0x4 of the target.
 *
 * @param result  the hidden result buffer
 * @param target  the camera target (this)
 * @return @p result, holding the position
 */
extern "C" void* cRiderCameraTarget_getPosition(void* result, cRiderCameraTarget* target)
{
    cRider_getHipPosition(result, target->Rider);
    return result;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getRotation);
#ifdef SKIP_ASM
/**
 * Returns the rider's orientation as a quaternion.
 *
 * Copies the 16 bytes at +0x120 of the rider (+0x4 of the target).
 *
 * @param result  the hidden result buffer
 * @param target  the camera target (this)
 * @return @p result, holding the rotation
 */
extern "C" void* cRiderCameraTarget_getRotation(void* result, cRiderCameraTarget* target)
{
    cRider* rider = target->Rider;
    *(cQuad128*)result = *(cQuad128*)&rider->mRotation;
    return result;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getVelocity);
#ifdef SKIP_ASM

class cCamVObj_0015F5B0 {
public:
    // vptr at 0x0
    virtual void v01();
    virtual t4Vector* getVelocity();
};

/**
 * Returns the rider's velocity.
 *
 * Calls virtual slot 2 (getVelocity) of the rider's cBeing part (+0x6C0) and copies the result.
 *
 * @param result  the hidden result buffer
 * @param target  the camera target (this)
 * @return @p result, holding the velocity
 */
extern "C" void* cRiderCameraTarget_getVelocity(void* result, cRiderCameraTarget* target)
{
    cRider* rider = target->Rider;
    *(t4Vector*)result = *((cCamVObj_0015F5B0*)(&rider->mVtbl))->getVelocity();
    return result;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getFront);
#ifdef SKIP_ASM
/**
 * Returns the direction the rider faces.
 *
 * Copies the 16 bytes at +0x1B0 of the rider, the front row of its orientation matrix (side +0x1A0, up +0x1C0).
 *
 * @param result  the hidden result buffer
 * @param target  the camera target (this)
 * @return @p result, holding the front vector
 */
extern "C" void* cRiderCameraTarget_getFront(void* result, cRiderCameraTarget* target)
{
    cRider* rider = target->Rider;
    *(cQuad128*)result = *(cQuad128*)&rider->mMatrix.row[1];
    return result;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getSide);
#ifdef SKIP_ASM
/**
 * Returns the rider's sideways axis.
 *
 * Copies the 16 bytes at +0x1A0 of the rider.
 *
 * @param result  the hidden result buffer
 * @param target  the camera target (this)
 * @return @p result, holding the side vector
 */
extern "C" void* cRiderCameraTarget_getSide(void* result, cRiderCameraTarget* target)
{
    cRider* rider = target->Rider;
    *(cQuad128*)result = *(cQuad128*)&rider->mMatrix;
    return result;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getUp);
#ifdef SKIP_ASM
/**
 * Returns the rider's up axis.
 *
 * Copies the 16 bytes at +0x1C0 of the rider.
 *
 * @param result  the hidden result buffer
 * @param target  the camera target (this)
 * @return @p result, holding the up vector
 */
extern "C" void* cRiderCameraTarget_getUp(void* result, cRiderCameraTarget* target)
{
    cRider* rider = target->Rider;
    *(cQuad128*)result = *(cQuad128*)&rider->mMatrix.row[2];
    return result;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getLean__FPv);
#ifdef SKIP_ASM
/**
 * Returns how far the rider leans.
 *
 * The float at +0x1F0 of the rider (+0x4 of the target).
 *
 * @return the lean value
 */
float cRiderCameraTarget_getLean(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return self->Rider->mSlewed[0].cur;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getNormal);
#ifdef SKIP_ASM
/**
 * Returns the normal of the ground under the rider.
 *
 * Copies the 16 bytes at +0x370 of the rider; the last ground normal follows at +0x380.
 *
 * @param result  the hidden result buffer
 * @param target  the camera target (this)
 * @return @p result, holding the normal
 */
extern "C" void* cRiderCameraTarget_getNormal(void* result, cRiderCameraTarget* target)
{
    cRider* rider = target->Rider;
    *(cQuad128*)result = *(cQuad128*)&rider->mGroundNormal;
    return result;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getLastGroundNormal);
#ifdef SKIP_ASM
/**
 * Returns the ground normal from the last time the rider touched the ground.
 *
 * Copies the 16 bytes at +0x380 of the rider.
 *
 * @param result  the hidden result buffer
 * @param target  the camera target (this)
 * @return @p result, holding the normal
 */
extern "C" void* cRiderCameraTarget_getLastGroundNormal(void* result, cRiderCameraTarget* target)
{
    cRider* rider = target->Rider;
    *(cQuad128*)result = *(cQuad128*)&rider->LastGroundNormal;
    return result;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getHandplantSplineTangent__FPv);
#ifdef SKIP_ASM
/**
 * Returns the direction of the handplant motion's path.
 *
 * Address of +0x130 in the rider's handler (cRider mRiderHandler, +0x77C); the handplant target follows at +0x140.
 *
 * @return a pointer to the tangent
 */
void* cRiderCameraTarget_getHandplantSplineTangent(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return (char*)&self->Rider->mRiderHandler->HandplantMotion.SplineDir;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getHandplantTarget__FPv);
#ifdef SKIP_ASM
/**
 * Returns the point the rider plants a hand on.
 *
 * Address of +0x140 in the rider's handler (+0x77C).
 *
 * @return a pointer to the target point
 */
void* cRiderCameraTarget_getHandplantTarget(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return (char*)&self->Rider->mRiderHandler->HandplantMotion.SplinePos;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getTotalAirTime__FPv);
#ifdef SKIP_ASM
/**
 * Returns the predicted total time of the rider's current jump.
 *
 * The float at +0x98 of the air predictor at +0x788 of the rider.
 *
 * @return the total air time in seconds
 */
float cRiderCameraTarget_getTotalAirTime(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return self->Rider->mAirPredictor->TotalAirTime;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getRemainingAirTime);
#ifdef SKIP_ASM
/**
 * Returns how much longer the target rider's current jump will last, from its air predictor.
 *
 * Reads the rider's mAirPredictor: the word at +0x98 (the predicted total air time) minus the one at +0xA0 (the time
 * in the air so far).
 *
 * @return the remaining air time
 */
extern "C" float cRiderCameraTarget_getRemainingAirTime(cRiderCameraTarget* self)
{
    cRider* rider = self->Rider;
    cAirPredictor* airPredictor = rider->mAirPredictor;
    return airPredictor->TotalAirTime - airPredictor->CurAirTime;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getApexTime);
#ifdef SKIP_ASM
/**
 * Returns the time left until the target rider reaches the top of its jump.
 *
 * Reads the rider's mAirPredictor: the word at +0x9C (the predicted apex time) minus the one at +0xA0 (the time in
 * the air so far).
 *
 * @return the time until the apex (negative once past it)
 */
extern "C" float cRiderCameraTarget_getApexTime(cRiderCameraTarget* self)
{
    cRider* rider = self->Rider;
    cAirPredictor* airPredictor = rider->mAirPredictor;
    return airPredictor->ApexTime - airPredictor->CurAirTime;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getApexPos__FPv);
#ifdef SKIP_ASM
/**
 * Returns the predicted highest point of the rider's jump.
 *
 * Address of +0x40 in the air predictor at +0x788 of the rider.
 *
 * @return a pointer to the apex position
 */
void* cRiderCameraTarget_getApexPos(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return (char*)&self->Rider->mAirPredictor->GuessApexPos;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getHalfPipeNormal__FPv);
#ifdef SKIP_ASM
/**
 * Returns the normal of the half-pipe wall the rider is using.
 *
 * Address of +0x3C0 in the rider.
 *
 * @return a pointer to the normal
 */
void* cRiderCameraTarget_getHalfPipeNormal(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return (char*)&self->Rider->HalfPipeNormal;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getBoostWarp__FPv);
#ifdef SKIP_ASM
/**
 * Returns how strongly the rider is boosting, for the camera's speed effect.
 *
 * The float at +0x2FC of the rider.
 *
 * @return the boost amount
 */
float cRiderCameraTarget_getBoostWarp(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return self->Rider->mBoostStrength;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getCrouch__FPv);
#ifdef SKIP_ASM
/**
 * Returns how far the rider is crouched.
 *
 * The float at +0x220 of the rider.
 *
 * @return the crouch amount
 */
float cRiderCameraTarget_getCrouch(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return self->Rider->mSlewed[4].cur;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_isAirPredictorDone);
#ifdef SKIP_ASM
/**
 * Tells whether the target rider's air predictor has finished simulating: its state word (+0xAC) is 1 or 3.
 *
 * @return 1 when done, else 0
 */
extern "C" int cRiderCameraTarget_isAirPredictorDone(cRiderCameraTarget* self)
{
    cRider* rider = self->Rider;
    cAirPredictor* airPredictor = rider->mAirPredictor;
    int simState = airPredictor->SimState;
    int done = 0;
    if (simState == 1 || simState == 3) {
        done = 1;
    }
    return done;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getSurfaceType__FPv);
#ifdef SKIP_ASM
/**
 * Returns the type of surface the rider is on.
 *
 * The int at +0x438 of the rider.
 *
 * @return the surface type (On Tour eSurfaceType)
 */
int cRiderCameraTarget_getSurfaceType(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return self->Rider->mSurfaceType;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getVerticalJump__FPv);
#ifdef SKIP_ASM
/**
 * Returns the rider's vertical jump value.
 *
 * The float at +0x5A4 of the rider.
 *
 * @return the vertical jump value
 */
float cRiderCameraTarget_getVerticalJump(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return self->Rider->VerticalJump;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_didPop__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the rider popped (jumped actively) on takeoff.
 *
 * The int at +0x5A8 of the rider.
 *
 * @return nonzero when the rider popped
 */
int cRiderCameraTarget_didPop(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return self->Rider->DidPop;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getLandingAngle);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern "C" float func_0031C128(float x);

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_15F780(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_15F780(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_15F780(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Returns the angle at which the target rider's predicted landing meets the ground.
 *
 * Takes the air predictor's vectors at +0x20 and +0x10; when both are non-zero, returns pi/2 minus the angle between
 * the first and the negated second (acosf of their normalised dot product, clamped to [-1, 1]). Returns 0 when either
 * vector is zero or their length product is below 0.001.
 *
 * @return the landing angle in radians, or 0
 */
extern "C" float cRiderCameraTarget_getLandingAngle(cRiderCameraTarget* self)
{
    cAirPredictor* airPredictor = self->Rider->mAirPredictor;
    t4Vector landNormal = airPredictor->GuessLandNormal;
    t4Vector landVelocity = airPredictor->GuessLandVelocity;
    int ok = 0;
    if (Dot_15F780(landNormal, landNormal) > 0.0f) {
        if (Dot_15F780(landVelocity, landVelocity) > 0.0f)
            ok = 1;
    }
    if (ok) {
        t4Vector n = Scale_15F780(landVelocity, -1.0f);
        float lengthProduct = Sqrt_15F780(Dot_15F780(landNormal, landNormal) * Dot_15F780(n, n));
        if (lengthProduct < 0.0010000000474974513f)
            return 0.0f;
        float cosAngle = Dot_15F780(landNormal, n) / lengthProduct;
        float clampedCos;
        if (cosAngle >= -1.0f)
            clampedCos = cosAngle <? 1.0f;
        else
            clampedCos = -1.0f;
        return 1.5707963705062866f - func_0031C128(clampedCos);
    }
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_isRidingFlexRail__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the rider is on a flexible rail.
 *
 * The int at +0x5AC of the rider.
 *
 * @return nonzero on a flex rail
 */
int cRiderCameraTarget_isRidingFlexRail(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return self->Rider->RidingFlexRail;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getLandingNormal__FPv);
#ifdef SKIP_ASM
/**
 * Returns the predicted ground normal where the rider will land.
 *
 * Address of +0x20 in the air predictor at +0x788 of the rider.
 *
 * @return a pointer to the landing normal
 */
void* cRiderCameraTarget_getLandingNormal(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return (char*)&self->Rider->mAirPredictor->GuessLandNormal;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getLandingVelocity__FPv);
#ifdef SKIP_ASM
/**
 * Returns the rider's predicted velocity on landing.
 *
 * Address of +0x10 in the air predictor at +0x788 of the rider.
 *
 * @return a pointer to the landing velocity
 */
void* cRiderCameraTarget_getLandingVelocity(void* vself)
{
    cRiderCameraTarget* self = (cRiderCameraTarget*)vself;
    return (char*)&self->Rider->mAirPredictor->GuessLandVelocity;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cBasicCameraAlgorithm_cBasicCameraAlgorithm);
#ifdef SKIP_ASM
extern void* cCameraAlgorithm_vtbl[];
extern void* cBasicCameraAlgorithm_vtbl[];
// PORT: cQuad128 (the unit's 128-bit TImode typedef) is a 16-byte register copy.
extern cQuad128 t4vect_Origin;
extern "C" float cCameraAlgorithm_getDefaultView(cCameraAlgorithm* self);

/**
 * Constructs a basic camera algorithm (a position with pitch and heading).
 *
 * Sets the default view, near 10, far 30000, type 0x4F, Position (0, 0, 0, 1) and zero pitch and heading.
 *
 * @return self
 */
extern "C" void* cBasicCameraAlgorithm_cBasicCameraAlgorithm(cBasicCameraAlgorithm* self)
{
    self->mVtbl = cCameraAlgorithm_vtbl;
    *(float*)self = cCameraAlgorithm_getDefaultView((cCameraAlgorithm*)self);
    self->AlgorithmType = 78;
    self->Near = 10.0f;
    self->Far = 30000.0f;
    self->mVtbl = cBasicCameraAlgorithm_vtbl;
    *(int*)&self->Pitch = 0;
    *(int*)&self->Heading = 0;
    cQuad128 initPos = t4vect_Origin;
    self->AlgorithmType = 79;
    *(cQuad128*)&self->Position = initPos;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cBasicCameraAlgorithm_getPose);
#ifdef SKIP_ASM



extern t4Matrix t4mat_Identity_15F9B8 __asm__("t4mat_Identity");
extern t4Vector t4vect_YAxis_15F9B8 __asm__("t4vect_YAxis");
extern t4Vector t4vect_ZAxis_v15F9B8 __asm__("t4vect_ZAxis");
extern "C" void fSinCos(float* sout, float* cout, float x);
extern "C" tPose tPose_tPose_15F9B8(t4Matrix* m) __asm__("tPose_tPose");

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_15F9B8(t4Matrix* dst, const t4Matrix* src)
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
static inline void vu0MulMtx_15F9B8(t4Matrix* dst, const t4Matrix* a, const t4Matrix* b)
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

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_15F9B8(t4Matrix* m, const t4Vector& v)
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

static inline void SinCos_15F9B8(float* s, float* c, float angle)
{
    fSinCos(s, c, angle);
}

static inline void RotAxis_15F9B8(t4Matrix* m, const t4Vector& axis, float angle)
{
    t4Matrix r;
    float s;
    float c;
    float one = 1.0f;
    SinCos_15F9B8(&s, &c, angle);
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    ((float (*)[4])&r.row[0])[0][0] = tx * x + c;
    ((float (*)[4])&r.row[0])[1][0] = tx * y + sz;
    ((float (*)[4])&r.row[0])[2][0] = tx * z - sy;
    ((float (*)[4])&r.row[0])[0][1] = ty * x - sz;
    ((float (*)[4])&r.row[0])[1][1] = ty * y + c;
    ((float (*)[4])&r.row[0])[2][1] = ty * z + sx;
    ((float (*)[4])&r.row[0])[0][2] = tz * x + sy;
    ((float (*)[4])&r.row[0])[1][2] = tz * y - sx;
    ((float (*)[4])&r.row[0])[2][2] = tz * z + c;
    ((float (*)[4])&r.row[0])[3][0] = 0.0f;
    ((float (*)[4])&r.row[0])[3][1] = 0.0f;
    ((float (*)[4])&r.row[0])[3][2] = 0.0f;
    ((float (*)[4])&r.row[0])[0][3] = 0.0f;
    ((float (*)[4])&r.row[0])[1][3] = 0.0f;
    ((float (*)[4])&r.row[0])[2][3] = 0.0f;
    ((float (*)[4])&r.row[0])[3][3] = one;
    vu0MulMtx_15F9B8(m, m, &r);
}

/**
 * Returns the free camera's pose: t4mat_Identity translated to Position, turned by -Pitch about the y axis and by
 * Heading about the z axis, converted with tPose_tPose.
 *
 * @param src  the algorithm (a cBasicCameraAlgorithm)
 * @return the pose (position and rotation quaternion), through the hidden return slot
 */
extern "C" tPose cBasicCameraAlgorithm_getPose(cBasicCameraAlgorithm* src)
{
    t4Matrix poseMatrix;
    vu0CopyMtx_15F9B8(&poseMatrix, &t4mat_Identity_15F9B8);
    vu0TransMtx_15F9B8(&poseMatrix, *(&src->Position));
    RotAxis_15F9B8(&poseMatrix, t4vect_YAxis_15F9B8, -src->Pitch);
    RotAxis_15F9B8(&poseMatrix, t4vect_ZAxis_v15F9B8, src->Heading);
    return tPose_tPose_15F9B8(&poseMatrix);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", cManualCameraAlgorithm_cManualCameraAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cBasicCameraAlgorithm_cBasicCameraAlgorithm(cBasicCameraAlgorithm* self);
extern void* cManualCameraAlgorithm_vtbl[];

/**
 * Constructs a manual camera algorithm on top of the basic one.
 *
 * Sets type 0x51 and cManualCameraAlgorithm_vtbl.
 *
 * @return self
 */
extern "C" void* cManualCameraAlgorithm_cManualCameraAlgorithm(cBasicCameraAlgorithm* self)
{
    cBasicCameraAlgorithm_cBasicCameraAlgorithm(self);
    self->AlgorithmType = 81;
    self->mVtbl = cManualCameraAlgorithm_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cManualCameraAlgorithm_snap__FPv);
#ifdef SKIP_ASM
/**
 * Snaps the manual camera algorithm to its subject: an empty stub in retail (On Tour snap(int subjectID)).
 */
void cManualCameraAlgorithm_snap(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", cManualCameraAlgorithm_update);
#ifdef SKIP_ASM
// PORT: cInput_getValue returns a float (input axis value, $f0).
extern "C" float cInput_getValue(int pad, int axis);
extern "C" float fSin(float x);
extern "C" float fCos(float x);

struct sVec_0015FD50 {
    float x, y, z, w;
    sVec_0015FD50() {}
    sVec_0015FD50(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    sVec_0015FD50& operator+=(const sVec_0015FD50& b);
} __attribute__((aligned(16)));

struct sFreeCam_0015FD50 {
    char pad0[0x20];
    sVec_0015FD50 pos;  // 0x20
    float pitch;        // 0x30
    float yaw;          // 0x34
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_0015FD50 fcScale_0015FD50(const sVec_0015FD50& v, float s)
{
    sVec_0015FD50 r;
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

// PORT: PS2-only VU0 inline asm (vector add, in place).
inline sVec_0015FD50& sVec_0015FD50::operator+=(const sVec_0015FD50& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(*this)
        : "m"(*this), "m"(b)
        : "memory");
    return *this;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline sVec_0015FD50 fcAdd_0015FD50(const sVec_0015FD50& a, const sVec_0015FD50& b)
{
    sVec_0015FD50 r;
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

static inline float fcClamp_0015FD50(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_0015FD50(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

/**
 * Moves the free-flying manual camera one frame from the analogue inputs of the viewing player's pad.
 *
 * Takes the pad from SSXApp->Game->ViewMan (+0x4, then +0xA4, then +0xC) and reads cInput_getValue channels 0x34
 * (speed), 0x31 and 0x30 (pitch and heading rates), 0x32 (forward) and 0x33 (strafe). Pitch changes by max(speed, 2)
 * * input * pi/5 per 1/60 s and is clamped to +-pi/2; heading changes the same way and wraps into [0, 2pi). Position
 * then moves along the look vector built from them (cos/sin: fCos/fSin) by speed * forward * 25 and
 * along the side vector by speed * strafe * -25.
 */
// PORT: g++ `>?` / `<?` (max/min) operators.
extern "C" void cManualCameraAlgorithm_update(cBasicCameraAlgorithm* self)
{
    int input = (int)SSXApp->Game->ViewMan->PlayerCamera[0]->ManualController->Input;
    float speed = cInput_getValue(input, 52);
    float ry = cInput_getValue(input, 49);
    float rx = cInput_getValue(input, 0x30);
    float mf = cInput_getValue(input, 50);
    float ms = cInput_getValue(input, 51);
    float rs = speed >? 2.0f;
    float pitch = fcClamp_0015FD50(self->Pitch + rs * (ry * 0.6283185482025146f) * 0.01666666753590107f,
                                   -1.5707963705062866f, 1.5707963705062866f);
    float yaw = self->Heading + rs * (rx * 0.6283185482025146f) * 0.01666666753590107f;
    yaw -= ffloor_0015FD50(yaw * 0.15915493667125702f) * 6.2831854820251465f;
    sVec_0015FD50 fwd(fCos(pitch) * fCos(yaw), fCos(pitch) * fSin(yaw),
                      fSin(pitch), 0.0f);
    sVec_0015FD50 right(fSin(yaw), -fCos(yaw), 0.0f, 0.0f);
    sVec_0015FD50 pos = fcAdd_0015FD50(sVec_0015FD50(*(sVec_0015FD50*)&self->Position), fcScale_0015FD50(fwd, speed * (mf * 25.0f)));
    pos += fcScale_0015FD50(right, speed * (ms * -25.0f));
    self->Pitch = pitch;
    self->Heading = yaw;
    *(sVec_0015FD50*)&self->Position = pos;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", cEditorCameraAlgorithm_cEditorCameraAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cBasicCameraAlgorithm_cBasicCameraAlgorithm(cBasicCameraAlgorithm* self);
extern void* cEditorCameraAlgorithm_vtbl[];

/**
 * Constructs an editor camera algorithm on top of the basic one.
 *
 * Sets type 0x50 and cEditorCameraAlgorithm_vtbl.
 *
 * @return self
 */
extern "C" void* cEditorCameraAlgorithm_cEditorCameraAlgorithm(cBasicCameraAlgorithm* self)
{
    cBasicCameraAlgorithm_cBasicCameraAlgorithm(self);
    self->AlgorithmType = 80;
    self->mVtbl = cEditorCameraAlgorithm_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraJitterer_cCameraJitterer);
#ifdef SKIP_ASM
/**
 * Constructs a camera jitterer: focus and roll jitter times 0.05, current times 1.05, all random jitter values zero.
 *
 * @return @p self
 */
extern "C" void* cCameraJitterer_cCameraJitterer(cCameraJitterer* self)
{
    self->FocusJitterTime = 0.05000000074505806f;
    self->RollJitterTime = 0.05000000074505806f;
    *(int*)&self->OldRandomSideJitter = 0;
    *(int*)&self->OldRandomUpJitter = 0;
    *(int*)&self->OldRandomRollJitter = 0;
    *(int*)&self->RandomSideJitter = 0;
    *(int*)&self->RandomUpJitter = 0;
    *(int*)&self->RandomRollJitter = 0;
    self->CurrFocusJitterTime = 1.0499999523162842f;
    self->CurrRollJitterTime = 1.0499999523162842f;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraJitterer_updatePositionalJitter);
#ifdef SKIP_ASM
int BXrand();

struct sShake_160028
{
    float x;        // 0x0
    float y;        // 0x4
    float pad8;
    float nx;       // 0xC
    float ny;       // 0x10
    float pad14;
    float amount;   // 0x18
    float pad1C;
    float timer;    // 0x20
    float pad24;
    float period;   // 0x28
};

// Uniform float in [0, 1) built from the random mantissa bits.
static inline float randf_160028()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

/**
 * Advances the positional (focus) jitter one frame: stores @p amount and picks new random side/up offsets every
 * FocusJitterTime seconds.
 *
 * With a nonzero amount, once CurrFocusJitterTime passes FocusJitterTime the current RandomSideJitter/RandomUpJitter
 * become the Old values, new ones are drawn in [-1, 1) and the timer restarts; otherwise the timer advances by 1/60.
 * A zero amount clears the four offsets and the timer.
 *
 * @param amount  the jitter strength, stored in FocusJitterFactor; 0 turns the jitter off (On Tour posJitter)
 */
extern "C" void cCameraJitterer_updatePositionalJitter(cCameraJitterer* self, float amount)
{
    self->FocusJitterFactor = amount;
    if (amount != 0.0f) {
        if (self->CurrFocusJitterTime > self->FocusJitterTime) {
            self->OldRandomSideJitter = self->RandomSideJitter;
            self->OldRandomUpJitter = self->RandomUpJitter;
            self->RandomSideJitter = (randf_160028() - 0.5f) * 2.0f;
            self->RandomUpJitter = (randf_160028() - 0.5f) * 2.0f;
            self->CurrFocusJitterTime = 0.0f;
        } else {
            self->CurrFocusJitterTime = self->CurrFocusJitterTime + 0.01666666753590107f;
        }
    } else {
        self->OldRandomSideJitter = 0.0f;
        self->OldRandomUpJitter = 0.0f;
        self->RandomSideJitter = 0.0f;
        self->RandomUpJitter = 0.0f;
        self->CurrFocusJitterTime = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraJitterer_updateRollJitter);
#ifdef SKIP_ASM
int BXrand();

struct sRollShake_160130
{
    float pad0[2];
    float cur;      // 0x8
    float pad0C[2];
    float next;     // 0x14
    float pad18;
    float amount;   // 0x1C
    float pad20;
    float timer;    // 0x24
    float pad28;
    float period;   // 0x2C
};

// Uniform float in [0, 1) built from the random mantissa bits.
static inline float randf_160130()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

// PORT: abs.s via inline asm (as an SDK math-header fabsf would).
static inline float fabs_160130(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Advances the roll jitter one frame: stores @p amount and picks a new random roll every RollJitterTime seconds.
 *
 * With a nonzero amount, once CurrRollJitterTime passes RollJitterTime the current RandomRollJitter becomes
 * OldRandomRollJitter and a new value in [-1, 1) is drawn until it differs from the old one by at least 0.5; the
 * timer then restarts. Otherwise the timer advances by 1/60. A zero amount clears the timer and both roll values.
 *
 * @param amount  the roll jitter strength, stored in RollJitterFactor; 0 turns it off (On Tour rollJitter)
 */
extern "C" void cCameraJitterer_updateRollJitter(cCameraJitterer* self, float amount)
{
    self->RollJitterFactor = amount;
    if (amount != 0.0f) {
        if (self->CurrRollJitterTime > self->RollJitterTime) {
            self->OldRandomRollJitter = self->RandomRollJitter;
            while (fabs_160130(self->OldRandomRollJitter - self->RandomRollJitter) < 0.5f)
                self->RandomRollJitter = (randf_160130() - 0.5f) * 2.0f;
            self->CurrRollJitterTime = 0.0f;
        } else {
            self->CurrRollJitterTime = self->CurrRollJitterTime + 0.01666666753590107f;
        }
    } else {
        self->CurrRollJitterTime = 0.0f;
        self->RandomRollJitter = 0.0f;
        self->OldRandomRollJitter = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraJitterer_getPositionalJitter);
#ifdef SKIP_ASM
extern "C" float fSin(float x);
extern "C" float fCos(float x);

struct sVec_00160228 {
    float x, y, z, w;
    sVec_00160228() {}
    sVec_00160228(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern sVec_00160228 t4vect_Zero_v160228 __asm__("t4vect_Zero");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec_00160228 Scale_00160228(const sVec_00160228& v, float s)
{
    sVec_00160228 r;
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
static inline sVec_00160228 Add_00160228(const sVec_00160228& a, const sVec_00160228& b)
{
    sVec_00160228 r;
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
static inline sVec_00160228 Cross_00160228(const sVec_00160228& a, const sVec_00160228& b)
{
    sVec_00160228 r;
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

/**
 * Returns the current positional jitter as an offset across the view.
 *
 * Returns t4vect_Zero when FocusJitterFactor is 0. Otherwise builds the view's forward vector from the angles @p heading and
 * @p pitch (cosines and sines from fCos and fSin), the side vector (sin a, -cos a, 0) and up = forward x
 * side, and returns (side * sx + up * sy) * FocusJitterFactor * 10, where k = CurrFocusJitterTime / FocusJitterTime,
 * sx = k * (RandomSideJitter - OldRandomSideJitter) and sy = k * (RandomUpJitter - OldRandomUpJitter).
 *
 * @param jitterer  the jitterer
 * @param heading  the view heading (radians)
 * @param pitch  the view pitch (radians)
 * @return the offset (w = 0)
 */
extern "C" sVec_00160228 cCameraJitterer_getPositionalJitter(cCameraJitterer* jitterer, float heading, float pitch)
{
    float zero = 0.0f;
    if (jitterer->FocusJitterFactor == zero) return t4vect_Zero_v160228;
    sVec_00160228 fwd(fCos(pitch) * fCos(heading), fCos(pitch) * fSin(heading), -fSin(pitch), zero);
    sVec_00160228 right(fSin(heading), -fCos(heading), zero, zero);
    sVec_00160228 up = Cross_00160228(fwd, right);
    float k = jitterer->CurrFocusJitterTime / jitterer->FocusJitterTime;
    float sx = k * (jitterer->RandomSideJitter - jitterer->OldRandomSideJitter);
    float sy = k * (jitterer->RandomUpJitter - jitterer->OldRandomUpJitter);
    float sz = jitterer->FocusJitterFactor * 10.0f;
    sVec_00160228 res = Scale_00160228(Add_00160228(Scale_00160228(right, sx), Scale_00160228(up, sy)), sz);
    return res;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraJitterer_getRollJitter);
#ifdef SKIP_ASM
/**
 * Returns the current roll jitter angle.
 *
 * RollJitterFactor * (CurrRollJitterTime / RollJitterTime) * pi/400 * (RandomRollJitter - OldRandomRollJitter), or 0
 * when RollJitterFactor is 0.
 *
 * @return the roll offset in radians
 */
extern "C" float cCameraJitterer_getRollJitter(cCameraJitterer* self)
{
    float jitterFactor = self->RollJitterFactor;
    if (jitterFactor == 0.0f)
        return 0.0f;
    return jitterFactor * (self->CurrRollJitterTime / self->RollJitterTime * 0.007853982038795948f)
             * (self->RandomRollJitter - self->OldRandomRollJitter);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cManualChaseAlgorithm_cManualChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(cLookAtChaseAlgorithm* self);
extern void* cManualChaseAlgorithm_vtbl[];

/**
 * Constructs the player-steered (manual) chase camera algorithm.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 0xB (On Tour CAMERA_ALGORITHM_CHASE_MANUAL = 11) and
 * cManualChaseAlgorithm_vtbl.
 *
 * @return @p self
 */
extern "C" void* cManualChaseAlgorithm_cManualChaseAlgorithm(cManualChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(((cLookAtChaseAlgorithm*)self));
    self->AlgorithmType = 11;
    self->mVtbl = cManualChaseAlgorithm_vtbl;
    return ((void*)self);
}
#endif

extern void* cManualChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/camera", cManualChaseAlgorithm__cManualChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a manual chase camera algorithm: stores cManualChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cManualChaseAlgorithm__cManualChaseAlgorithm(void* vself)
{
    cManualChaseAlgorithm* self = (cManualChaseAlgorithm*)vself;
    self->mVtbl = (void**)((int)(void*)cManualChaseAlgorithm_vtbl);
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cManualChaseAlgorithm_getRestDrop__FPv);
#ifdef SKIP_ASM
/**
 * Returns the manual chase camera's rest drop (vtable slot 13, On Tour getRestDrop): always 0.
 *
 * @return 0
 */
float cManualChaseAlgorithm_getRestDrop(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cManualChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the manual chase camera: only clears DoPitchBlending (+0x10), so
 * doRaceCamFinalPhase skips pitch blending.
 *
 * @param other  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
void cManualChaseAlgorithm_getRaceCamFinalPhaseData(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cManualChaseAlgorithm_update);
#ifdef SKIP_ASM
extern "C" float cInput_getValue(int pad, int axis);
extern "C" void cLookAtChaseAlgorithm_setupChaseData(void* self, struct cChaseSetup* out, float a, float b, float c, float d,
                              float e, float f, float g, float h);
extern "C" void cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading(void* self, void* out);
extern float D_004A127C;
extern float D_004A1280;
extern float D_004A1284;



// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_1604B8(const t4Vector& v, float s)
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
static inline t4Vector Sub_1604B8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (in-place v /= s).
static inline void DivEq_1604B8(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1604B8(const t4Vector& v)
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

// PORT: PS2-only inline asm (float absolute value).
static inline float Abs_1604B8(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `<?` (min) operator.
static inline float Clamp_1604B8(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}


struct sGame_1604B8 {
    char pad0[0x84];
    char* f84;          // 0x84
    char pad88[0xB0 - 0x88];
    int pads[4];        // 0xB0
};

/**
 * Runs one frame of the manual chase camera: the player orbits the rider and zooms with three analogue axes.
 *
 * Runs setupChaseData (cLookAtChaseAlgorithm_setupChaseData) with the standard smoothing constants, then reads cInput_getValue channels
 * 0xBE, 0xBF and 0xC0 (negated) of the pad SSXApp->pads selects through +0x84, +0x28, +0x28, squaring each but
 * keeping its sign. They drive three globals shared by every manual chase camera: heading D_004A127C (+= 0.05 * x,
 * wrapped into [0, 2pi]), pitch D_004A1280 (+= 0.05 * y, clamped to 0..0.48pi) and distance D_004A1284 (*= 1 + 0.05 *
 * z, clamped to 150..850). Pitch and Heading take the globals, CamTargetPos becomes the setup's target position and
 * Position sits that distance back along the look vector (cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading, normalised).
 */
extern "C" void cManualChaseAlgorithm_update(cManualChaseAlgorithm* self)
{
    cChaseSetup info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), (cChaseSetup*)&info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    sGame_1604B8* app = (sGame_1604B8*)SSXApp;
    int inputPad = app->pads[*(int*)(*(char**)(app->f84 + 0x28) + 0x28)];
    float ax = cInput_getValue(inputPad, 190);
    float ay = cInput_getValue(inputPad, 191);
    float az = -cInput_getValue(inputPad, 0xC0);
    ax *= Abs_1604B8(ax);
    ay *= Abs_1604B8(ay);
    az *= Abs_1604B8(az);
    ax *= 0.05000000074505806f;
    D_004A127C += ax;
    if (D_004A127C > 6.2831854820251465f) {
        D_004A127C = D_004A127C - 6.2831854820251465f;
    } else if (D_004A127C < 0.0f) {
        D_004A127C = D_004A127C + 6.2831854820251465f;
    }
    float k = 0.05000000074505806f;
    D_004A1280 += ay * k;
    D_004A1280 = Clamp_1604B8(D_004A1280, 0.0f, 1.5079644918441772f);
    self->Pitch = D_004A1280;
    self->Heading = D_004A127C;
    t4Vector dir;
    cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading(((char*)self), &dir);
    DivEq_1604B8(dir, Length_1604B8(dir));
    D_004A1284 = Clamp_1604B8(D_004A1284 * (az * k + 1.0f), 150.0f, 850.0f);
    *(&self->CamTargetPos) = info.TPos;
    *(&self->Position) = Sub_1604B8(*(&self->CamTargetPos), Scale_1604B8(dir, D_004A1284));
}
#endif

//100%
INCLUDE_ASM("camera/camera", cManualChaseAlgorithm_snap);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(cLookAtChaseAlgorithm* self, float a, float b);

struct sCamVEntryIntA {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sCamVEntryVoidA {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Snaps the manual chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(100, 100) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cManualChaseAlgorithm_snap(cManualChaseAlgorithm* self, int id)
{
    char* obj = (char*)&self->Target->Rider->mVtbl;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(vthis(obj, vt[7])))
    {
        cLookAtChaseAlgorithm_snap(((cLookAtChaseAlgorithm*)self), id);
        cLookAtChaseAlgorithm_setupInitialCamDir(((cLookAtChaseAlgorithm*)self), 100.0f, 100.0f);
        sCamVEntryVoidA* vt2 = (sCamVEntryVoidA*)self->mVtbl;
        vt2[5].fn(vthis(self, vt2[5]));
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMatrixChaseAlgorithm_cMatrixChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(cLookAtChaseAlgorithm* self);
extern void* cMatrixChaseAlgorithm_vtbl[];

/**
 * Constructs the matrix (orbiting) chase camera algorithm.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 0x29 (On Tour CAMERA_ALGORITHM_CHASE_MATRIX = 41),
 * cMatrixChaseAlgorithm_vtbl and LastAngle 0.
 *
 * @return @p self
 */
extern "C" void* cMatrixChaseAlgorithm_cMatrixChaseAlgorithm(cMatrixChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm((cLookAtChaseAlgorithm*)self);
    self->AlgorithmType = 41;
    self->mVtbl = cMatrixChaseAlgorithm_vtbl;
    *(int*)&self->LastAngle = 0;
    return ((void*)self);
}
#endif

extern void* cMatrixChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/camera", cMatrixChaseAlgorithm__cMatrixChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a matrix chase camera algorithm: stores cMatrixChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cMatrixChaseAlgorithm__cMatrixChaseAlgorithm(void* vself)
{
    cMatrixChaseAlgorithm* self = (cMatrixChaseAlgorithm*)vself;
    self->mVtbl = (void**)((int)(void*)cMatrixChaseAlgorithm_vtbl);
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMatrixChaseAlgorithm_getRestDrop__FPv);
#ifdef SKIP_ASM
/**
 * Returns the matrix chase camera's rest drop: always 0.
 *
 * The last virtual of the chase algorithms (after cChaseAlgorithm_setTarget); each algorithm supplies its own value.
 *
 * @return 0.0f
 */
float cMatrixChaseAlgorithm_getRestDrop(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMatrixChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the matrix chase camera: only clears DoPitchBlending (+0x10), so
 * doRaceCamFinalPhase skips pitch blending.
 *
 * @param other  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
void cMatrixChaseAlgorithm_getRaceCamFinalPhaseData(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMatrixChaseAlgorithm_update);
#ifdef SKIP_ASM
extern "C" float fSin(float x);
extern "C" float fCos(float x);



// asm-label views: camera.cpp declares these globals as cQuad128
extern t4Vector t4vect_XAxis_v1607E0 __asm__("t4vect_XAxis");
extern t4Vector t4vect_ZAxis_v1607E0 __asm__("t4vect_ZAxis");
extern "C" void cLookAtChaseAlgorithm_setupChaseData(void* self, cChaseSetup* out, float a, float b, float c, float d,
                              float e, float f, float g, float h);

struct sVEv_001607E0 {
    short delta;
    short index;
    t4Vector (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector cmScale_001607E0(const t4Vector& v, float s)
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
static inline t4Vector cmAdd_001607E0(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector cmCross_001607E0(const t4Vector& a, const t4Vector& b)
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
static inline float cmLength_001607E0(const t4Vector& v)
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
static inline void cmDivEq_001607E0(t4Vector& v, float s)
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

static inline void cmNormalize_001607E0(t4Vector& v)
{
    cmDivEq_001607E0(v, cmLength_001607E0(v));
}

// PORT: PS2-only inline asm (EE cvt.w.s truncates in the FPU; the C cast goes through a GPR).
static inline float ffloor_001607E0(float x)
{
    float t;
    __asm__("cvt.w.s %0,%1\n\tcvt.s.w %0,%0" : "=f"(t) : "f"(x));
    if (x < t) {
        t -= 1.0f;
    }
    return t;
}

static inline t4Vector cmGet_001607E0(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEv_001607E0* e = &(*(sVEv_001607E0**)o)[slot];
    return e->fn(vthis(o, *e));
}

/**
 * Runs one frame of the matrix camera: circles the rider at a fixed radius of 216.2, advancing LastAngle each frame.
 *
 * After setupChaseData (cLookAtChaseAlgorithm_setupChaseData) takes the horizontal travel direction from the setup's velocity, or the
 * target's front (slot 4) or the X axis (t4vect_XAxis) when that is near zero, with z cleared, and its side vector
 * (t4vect_ZAxis x dir); both are normalised. LastAngle (+0x390) advances by 0.0614 rad and wraps into [-pi, pi).
 * CamTargetPos becomes the rider position (target slot 1) and Position = target + dir * cos(angle) * 216.2 + side *
 * sin(angle) * 216.2 - Z * 0.56.
 */
extern "C" void cMatrixChaseAlgorithm_update(cMatrixChaseAlgorithm* self)
{
    cChaseSetup info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    float eps = 0.0010000000474974513f;
    t4Vector dir;
    if (__builtin_fabsf(info.TVel.x) > eps || __builtin_fabsf(info.TVel.y) > eps) {
        dir = info.TVel;
    } else {
        dir = cmGet_001607E0(((char*)self), 4);
        if (__builtin_fabsf(dir.x) < eps && __builtin_fabsf(dir.y) < eps) {
            dir = t4vect_XAxis_v1607E0;
        }
    }
    dir.z = 0.0f;
    t4Vector side = cmCross_001607E0(t4vect_ZAxis_v1607E0, dir);
    cmNormalize_001607E0(dir);
    cmNormalize_001607E0(side);
    float* ang = (float*)(&self->LastAngle);
    *ang += 0.061352409422397614f;
    *ang -= ffloor_001607E0(*ang * 0.15915493667125702f + 0.5f) * 6.2831854820251465f;
    float radius = 216.23182678222656f;
    *(&self->CamTargetPos) = cmAdd_001607E0(cmGet_001607E0(((char*)self), 1), cmScale_001607E0(t4vect_ZAxis_v1607E0, 0.0f));
    *(&self->Position) = cmAdd_001607E0(
        cmAdd_001607E0(cmAdd_001607E0(*(&self->CamTargetPos), cmScale_001607E0(dir, fCos(*ang) * radius)),
                       cmScale_001607E0(side, fSin(*ang) * radius)),
        cmScale_001607E0(t4vect_ZAxis_v1607E0, -0.5597707033157349f));
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMatrixChaseAlgorithm_snap);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(cLookAtChaseAlgorithm* self, float a, float b);

/**
 * Snaps the matrix chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(100, 100) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cMatrixChaseAlgorithm_snap(cMatrixChaseAlgorithm* self, int id)
{
    char* obj = (char*)&self->Target->Rider->mVtbl;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(vthis(obj, vt[7])))
    {
        cLookAtChaseAlgorithm_snap(self, id);
        cLookAtChaseAlgorithm_setupInitialCamDir((cLookAtChaseAlgorithm*)self, 100.0f, 100.0f);
        sCamVEntryVoidA* vt2 = (sCamVEntryVoidA*)self->mVtbl;
        vt2[5].fn(vthis(self, vt2[5]));
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRelativeChaseAlgorithm_cRelativeChaseAlgorithm);
#ifdef SKIP_ASM
extern void* cRelativeChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(cLookAtChaseAlgorithm* self);

/**
 * Constructs a relative chase camera that watches the rider from a fixed @p angle around its travel direction.
 *
 * Runs the look-at chase constructor, then stores Angle, cRelativeChaseAlgorithm_vtbl and Initialized 0. It leaves
 * AlgorithmType at the base's 0x54; the factory stores the camera type afterwards.
 *
 * @param angle  the angle around the rider in radians (the factory passes (type - 33) * pi/4)
 * @return @p self
 */
extern "C" void* cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(cRelativeChaseAlgorithm* self, float angle)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm((cLookAtChaseAlgorithm*)self);
    self->Angle = angle;
    self->mVtbl = cRelativeChaseAlgorithm_vtbl;
    self->Initialized = 0;
    return ((void*)self);
}
#endif

extern void* cRelativeChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/camera", cRelativeChaseAlgorithm__cRelativeChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a relative chase camera algorithm: stores cRelativeChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cRelativeChaseAlgorithm__cRelativeChaseAlgorithm(void* vself)
{
    cRelativeChaseAlgorithm* self = (cRelativeChaseAlgorithm*)vself;
    self->mVtbl = (void**)((int)(void*)cRelativeChaseAlgorithm_vtbl);
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRelativeChaseAlgorithm_getRestDrop__FPv);
#ifdef SKIP_ASM
/**
 * Returns the relative chase camera's rest drop: always 0.
 *
 * The last virtual of the chase algorithms (after cChaseAlgorithm_setTarget); each algorithm supplies its own value.
 *
 * @return 0.0f
 */
float cRelativeChaseAlgorithm_getRestDrop(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRelativeChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the relative chase camera: only clears DoPitchBlending (+0x10), so
 * doRaceCamFinalPhase skips pitch blending.
 *
 * @param other  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
void cRelativeChaseAlgorithm_getRaceCamFinalPhaseData(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRelativeChaseAlgorithm_update);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub_00160BF8(const t4Vector& a, const t4Vector& b)
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

/**
 * Runs one frame of a relative chase camera: holds the eye 385.9 units from the rider at Angle around its horizontal
 * travel direction and 53.2 units up, smoothing both points after the first frame.
 *
 * After setupChaseData takes the travel direction from the setup's velocity (else the target's front, slot 4, else
 * the X axis t4vect_XAxis) with z cleared, and its side vector. CamTargetPos becomes the rider position (slot 1); once
 * Initialized it is blended 0.64 LastCamTargetPos + 0.36 new. Position = target + dir * cos(Angle) * 385.9 + side *
 * sin(Angle) * 385.9 + Z * 53.2; once Initialized the look vector is blended 0.97 LastLook + 0.03 new. Stores
 * LastCamTargetPos and LastLook and sets Initialized.
 */
extern "C" void cRelativeChaseAlgorithm_update(cRelativeChaseAlgorithm* self)
{
    cChaseSetup info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    float eps = 0.0010000000474974513f;
    t4Vector dir;
    if (__builtin_fabsf(info.TVel.x) > eps || __builtin_fabsf(info.TVel.y) > eps) {
        dir = info.TVel;
    } else {
        dir = cmGet_001607E0(((char*)self), 4);
        if (__builtin_fabsf(dir.x) < eps && __builtin_fabsf(dir.y) < eps) {
            dir = t4vect_XAxis_v1607E0;
        }
    }
    dir.z = 0.0f;
    t4Vector side = cmCross_001607E0(t4vect_ZAxis_v1607E0, dir);
    cmNormalize_001607E0(dir);
    cmNormalize_001607E0(side);
    *(&self->CamTargetPos) = cmAdd_001607E0(cmGet_001607E0(((char*)self), 1), cmScale_001607E0(t4vect_ZAxis_v1607E0, 0.0f));
    if (self->Initialized) {
        *(&self->CamTargetPos) = cmAdd_001607E0(cmScale_001607E0(*(&self->LastCamTargetPos), 0.6399999856948853f),
                                                    cmScale_001607E0(*(&self->CamTargetPos), 0.36000001430511475f));
    }
    *(&self->LastCamTargetPos) = *(&self->CamTargetPos);
    float radius = 385.91839599609375f;
    *(&self->Position) = cmAdd_001607E0(
        cmAdd_001607E0(cmAdd_001607E0(*(&self->CamTargetPos), cmScale_001607E0(dir, fCos(self->Angle) * radius)),
                       cmScale_001607E0(side, fSin(self->Angle) * radius)),
        cmScale_001607E0(t4vect_ZAxis_v1607E0, 53.241573333740234f));
    if (self->Initialized) {
        t4Vector look = cmSub_00160BF8(*(&self->CamTargetPos), *(&self->Position));
        t4Vector newLook = cmAdd_001607E0(cmScale_001607E0(*(&self->LastLook), 0.9700000286102295f),
                                         cmScale_001607E0(look, 0.029999971389770508f));
        *(&self->Position) = cmSub_00160BF8(*(&self->CamTargetPos), newLook);
    }
    *(&self->LastLook) = cmSub_00160BF8(*(&self->CamTargetPos), *(&self->Position));
    self->Initialized = 1;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cRelativeChaseAlgorithm_snap);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(cLookAtChaseAlgorithm* self, float a, float b);

/**
 * Snaps the relative chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(100, 100), clears Initialized and runs the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cRelativeChaseAlgorithm_snap(cRelativeChaseAlgorithm* self, int id)
{
    char* obj = (char*)&self->Target->Rider->mVtbl;
    sCamVEntryIntA* vt = *(sCamVEntryIntA**)obj;
    if (id == vt[7].fn(vthis(obj, vt[7])))
    {
        cLookAtChaseAlgorithm_snap(self, id);
        cLookAtChaseAlgorithm_setupInitialCamDir((cLookAtChaseAlgorithm*)self, 100.0f, 100.0f);
        self->Initialized = 0;
        sCamVEntryVoidA* vt2 = (sCamVEntryVoidA*)self->mVtbl;
        vt2[5].fn(vthis(self, vt2[5]));
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cSplineCamAlgorithm_cSplineCamAlgorithm);
#ifdef SKIP_ASM
extern void* cSplineCamAlgorithm_vtbl[];
extern "C" void cCubicSplineInterpolant_cCubicSplineInterpolant(void* spline);
// PORT: cCubicSplineInterpolant_initAsNatural is declared (void*) but forwards a second argument (the key count) in $a1.
void cCubicSplineInterpolant_initAsNatural_2(void* spline, int n) __asm__("cCubicSplineInterpolant_initAsNatural__FPv");
extern "C" void cCubicSplineInterpolant_addPoint(void* spline, float x, float y);
extern "C" float cCubicSplineInterpolant_evaluate(void* spline, float x);

struct sVec_161060 {
    float x, y, z, w;
    sVec_161060() {}
    sVec_161060(float a, float b, float c, const float& d) : x(a), y(b), z(c), w(d) {}
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec_161060 Sub_161060(const sVec_161060& a, const sVec_161060& b)
{
    sVec_161060 r;
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
static inline float Length_161060(const sVec_161060& v)
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
 * Constructs a spline camera that flies through four points in @p dur seconds.
 *
 * Runs the look-at chase constructor, stores cSplineCamAlgorithm_vtbl and constructs the X/Y/Z interpolants XOfT,
 * YOfT and ZOfT (+0x390, +0x39C, +0x3A8; cCubicSplineInterpolant_cCubicSplineInterpolant). Sets LeadDistance, HeightOffset, View = @p fov and
 * AlgorithmType 0x4B (On Tour CAMERA_ALGORITHM_SPLINE = 75). Each interpolant gets four keys (cCubicSplineInterpolant_initAsNatural,
 * cCubicSplineInterpolant_addPoint) at times 0, dur/3, 2dur/3 and dur from the points' x, y and z. TotalTime = dur and LastEvaluatedTime
 * = 0; the path length is then summed over 100 samples (cCubicSplineInterpolant_evaluate, cCubicSplineInterpolant::evaluate) and Speed =
 * 0.9 * length / dur.
 *
 * @param point1  first point, three floats x, y, z (On Tour const t4Vector&)
 * @param point2  second point, reached at dur/3
 * @param point3  third point, reached at 2dur/3
 * @param point4  last point, reached at dur
 * @param dur  the flight time in seconds (On Tour time)
 * @param leadDistance  stored in LeadDistance
 * @param heightOffset  stored in HeightOffset
 * @param fov  the field of view, stored in View
 * @return @p self
 */
extern "C" void* cSplineCamAlgorithm_cSplineCamAlgorithm(cSplineCamAlgorithm* self, float* point1, float* point2, float* point3, float* point4, float dur, float leadDistance, float heightOffset,
                               float fov)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm((cLookAtChaseAlgorithm*)self);
    self->mVtbl = cSplineCamAlgorithm_vtbl;
    char* sx = self->XOfT;
    char* sy = self->YOfT;
    char* sz = self->ZOfT;
    cCubicSplineInterpolant_cCubicSplineInterpolant(sx);
    cCubicSplineInterpolant_cCubicSplineInterpolant(sy);
    cCubicSplineInterpolant_cCubicSplineInterpolant(sz);
    self->LeadDistance = leadDistance;
    self->HeightOffset = heightOffset;
    self->View = fov;
    self->AlgorithmType = 75;
    cCubicSplineInterpolant_initAsNatural_2(sx, 4);
    cCubicSplineInterpolant_initAsNatural_2(sy, 4);
    cCubicSplineInterpolant_initAsNatural_2(sz, 4);
    float firstKeyTime = dur * 0.33333298563957214f;
    float secondKeyTime = dur * 0.6666669845581055f;
    cCubicSplineInterpolant_addPoint(sx, 0.0f, point1[0]);
    cCubicSplineInterpolant_addPoint(sx, firstKeyTime, point2[0]);
    cCubicSplineInterpolant_addPoint(sx, secondKeyTime, point3[0]);
    cCubicSplineInterpolant_addPoint(sx, dur, point4[0]);
    cCubicSplineInterpolant_addPoint(sy, 0.0f, point1[1]);
    cCubicSplineInterpolant_addPoint(sy, firstKeyTime, point2[1]);
    cCubicSplineInterpolant_addPoint(sy, secondKeyTime, point3[1]);
    cCubicSplineInterpolant_addPoint(sy, dur, point4[1]);
    cCubicSplineInterpolant_addPoint(sz, 0.0f, point1[2]);
    cCubicSplineInterpolant_addPoint(sz, firstKeyTime, point2[2]);
    cCubicSplineInterpolant_addPoint(sz, secondKeyTime, point3[2]);
    cCubicSplineInterpolant_addPoint(sz, dur, point4[2]);
    self->TotalTime = dur;
    self->LastEvaluatedTime = 0.0f;
    float total = 0.0f;
    sVec_161060 prev(cCubicSplineInterpolant_evaluate(sx, total), cCubicSplineInterpolant_evaluate(sy, total), cCubicSplineInterpolant_evaluate(sz, total), 1.0f);
    for (int i = 1; i < 101; i++) {
        float testTime = (dur * (float)i * 0.009999999776482582f) <? dur;
        sVec_161060 cur(cCubicSplineInterpolant_evaluate(self->XOfT, testTime), cCubicSplineInterpolant_evaluate(self->YOfT, testTime), cCubicSplineInterpolant_evaluate(self->ZOfT, testTime), 1.0f);
        sVec_161060 chord = Sub_161060(cur, prev);
        total += Length_161060(chord);
        prev = cur;
    }
    self->Speed = total * 0.8999999761581421f / dur;
    return ((char*)self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cSplineCamAlgorithm__cSplineCamAlgorithm);
#ifdef SKIP_ASM
extern void* cSplineCamAlgorithm_vtbl[];
extern "C" void cCubicSplineInterpolant__cCubicSplineInterpolant(void* p, int flags);
// PORT: cLookAtChaseAlgorithm__cLookAtChaseAlgorithm is the base deleting dtor (self, flags); the unit declares it with one arg.
void cLookAtChaseAlgorithm__cLookAtChaseAlgorithm_dtor(void* self, int flags) __asm__("cLookAtChaseAlgorithm__cLookAtChaseAlgorithm");

/**
 * Destroys a spline camera algorithm: its three coordinate splines (+0x390, +0x39C, +0x3A8), then the chase base.
 *
 * Restores the vtable cSplineCamAlgorithm_vtbl, runs cCubicSplineInterpolant__cCubicSplineInterpolant (the spline destructor, flags 2) on the z, y and x splines and
 * chains to the cLookAtChaseAlgorithm destructor with @p flags.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the base destructor)
 */
extern "C" void cSplineCamAlgorithm__cSplineCamAlgorithm(cSplineCamAlgorithm* self, int flags)
{
    self->mVtbl = cSplineCamAlgorithm_vtbl;
    cCubicSplineInterpolant__cCubicSplineInterpolant((char*)&self->ZOfT, 2);
    cCubicSplineInterpolant__cCubicSplineInterpolant((char*)&self->YOfT, 2);
    cCubicSplineInterpolant__cCubicSplineInterpolant((char*)&self->XOfT, 2);
    cLookAtChaseAlgorithm__cLookAtChaseAlgorithm_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cSplineCamAlgorithm_getRestDrop__FPv);
#ifdef SKIP_ASM
/**
 * Returns the spline camera's rest drop (vtable slot 13, On Tour getRestDrop): always 0.
 *
 * @return 0
 */
float cSplineCamAlgorithm_getRestDrop(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cSplineCamAlgorithm_getRaceCamFinalPhaseData__FPvT0);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the spline camera: only clears DoPitchBlending (+0x10), so
 * doRaceCamFinalPhase skips pitch blending.
 *
 * @param other  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
void cSplineCamAlgorithm_getRaceCamFinalPhaseData(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cSplineCamAlgorithm_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


// The same vector with a constructor taking w by const reference (its callers compile differently from sVec4).
struct sVec4R : t4Vector
{
    sVec4R() {}
    sVec4R(float ax, float ay, float az, const float& aw) { x = ax; y = ay; z = az; w = aw; }
} __attribute__((aligned(16)));

extern t4Vector t4vect_ZAxis_13F0 __asm__("t4vect_ZAxis");

extern "C" float cCubicSplineInterpolant_evaluate(void* spline, float x);


// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4R Sub_13F0(const sVec4R& a, const sVec4R& b)
{
    sVec4R r;
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
static inline float Length_13F0(const sVec4R& v)
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

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline sVec4R ScaleP_13F0(float s, const sVec4R* v)
{
    sVec4R r;
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
static inline sVec4R Scale_13F0(float s, const sVec4R& v)
{
    sVec4R r;
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
static inline sVec4R Add_13F0(const sVec4R& a, const sVec4R& b)
{
    sVec4R r;
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

static inline sVec4R Get_13F0(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<sVec4R(void*)>* vEntry = &(*(sVEntry<sVec4R(void*)>**)o)[slot];
    return vEntry->fn(vthis(o, *vEntry));
}

/**
 * Moves the spline camera one frame along its path at constant speed and aims it ahead of the rider.
 *
 * Evaluates the x/y/z splines (+0x390/+0x39C/+0x3A8, cCubicSplineInterpolant_evaluate) one 1/60 s step past LastEvaluatedTime (capped at
 * TotalTime), then rescales the step so the camera moves Speed / 60 units: LastEvaluatedTime advances by the step
 * times (Speed / 60) / distance and Position is re-evaluated there. CamTargetPos becomes the target's position +
 * HeightOffset up (t4vect_ZAxis) + LeadDistance along the target's front (target slots 1 and 4).
 */
extern "C" void cSplineCamAlgorithm_update(cSplineCamAlgorithm* self)
{
    char* sx = self->XOfT;
    char* sy = self->YOfT;
    char* sz = self->ZOfT;
    float nextTime = (self->LastEvaluatedTime + 0.01666666753590107f) <? self->TotalTime;
    float speed = self->Speed * 0.01666666753590107f;
    {
        sVec4R prev = *(sVec4R*)&self->Position;
        sVec4R cur(cCubicSplineInterpolant_evaluate(sx, nextTime), cCubicSplineInterpolant_evaluate(sy, nextTime), cCubicSplineInterpolant_evaluate(sz, nextTime), 1.0f);
        sVec4R step = Sub_13F0(cur, prev);
        float len = Length_13F0(step);
        if (0.0f < len) {
            float lastTime = self->LastEvaluatedTime;
            float nt = (lastTime + (nextTime - lastTime) / (len / speed)) <? self->TotalTime;
            self->LastEvaluatedTime = nt;
            *(sVec4R*)&self->Position = sVec4R(cCubicSplineInterpolant_evaluate(sx, nt), cCubicSplineInterpolant_evaluate(sy, nt), cCubicSplineInterpolant_evaluate(sz, nt), 1.0f);
        }
    }
    *(sVec4R*)&self->CamTargetPos = Add_13F0(Add_13F0(Get_13F0(((char*)self), 1), ScaleP_13F0(self->HeightOffset, (const sVec4R*)&t4vect_ZAxis_13F0)),
                                          Scale_13F0(self->LeadDistance, Get_13F0(((char*)self), 4)));
}
#endif

//100%
INCLUDE_ASM("camera/camera", cSplineCamAlgorithm_snap);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);

struct sCamVEntryInt {
    short delta;
    short index;
    int (*fn)(void*);
};
struct sCamVEntryVoid {
    short delta;
    short index;
    void (*fn)(void*);
};

/**
 * Snaps the spline camera when @p id is the target rider's subject id.
 *
 * Compares @p id with slot 7 of the target rider's cRider vtable (+0x6C0); on a match runs the chase snap
 * (cLookAtChaseAlgorithm_snap) and then its own slot 5.
 *
 * @param id  the subject id to snap to (On Tour snap(int subjectID))
 */
extern "C" void cSplineCamAlgorithm_snap(cSplineCamAlgorithm* self, int id)
{
    char* obj = (char*)&self->Target->Rider->mVtbl;
    sCamVEntryInt* vt = *(sCamVEntryInt**)obj;
    if (id == vt[7].fn(vthis(obj, vt[7])))
    {
        cLookAtChaseAlgorithm_snap(self, id);
        sCamVEntryVoid* vt2 = (sCamVEntryVoid*)self->mVtbl;
        vt2[5].fn(vthis(self, vt2[5]));
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cIdleChaseAlgorithm_cIdleChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(cLookAtChaseAlgorithm* self);
extern void* cIdleChaseAlgorithm_vtbl[];

/**
 * Constructs the idle chase camera algorithm.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 7 (On Tour CAMERA_ALGORITHM_CHASE_IDLE) and
 * cIdleChaseAlgorithm_vtbl.
 *
 * @return @p self
 */
extern "C" void* cIdleChaseAlgorithm_cIdleChaseAlgorithm(cIdleChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(((cLookAtChaseAlgorithm*)self));
    self->AlgorithmType = 0x7;
    self->mVtbl = cIdleChaseAlgorithm_vtbl;
    return ((void*)self);
}
#endif

extern void* cIdleChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/camera", cIdleChaseAlgorithm__cIdleChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys an idle chase camera algorithm: stores cIdleChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cIdleChaseAlgorithm__cIdleChaseAlgorithm(void* vself)
{
    cIdleChaseAlgorithm* self = (cIdleChaseAlgorithm*)vself;
    self->mVtbl = (void**)((int)(void*)cIdleChaseAlgorithm_vtbl);
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cIdleChaseAlgorithm_getRestDrop__FPv);
#ifdef SKIP_ASM
/**
 * Returns the idle chase camera's rest drop (vtable slot 13, On Tour getRestDrop): always 0.
 *
 * @return 0
 */
float cIdleChaseAlgorithm_getRestDrop(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cIdleChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the idle chase camera: only clears DoPitchBlending (+0x10), so
 * doRaceCamFinalPhase skips pitch blending.
 *
 * @param other  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
void cIdleChaseAlgorithm_getRaceCamFinalPhaseData(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cIdleChaseAlgorithm_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector t4vect_ZAxis_1728 __asm__("t4vect_ZAxis");


extern "C" void cLookAtChaseAlgorithm_setupChaseData_1728(void* self, cChaseSetup* out, float a, float b, float c, float d,
                              float e, float f, float g, float h) __asm__("cLookAtChaseAlgorithm_setupChaseData");

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector cmAdd_001607E0_1728(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float cmLength_001607E0_1728(const t4Vector& v)
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

static inline t4Vector cmGet_001607E0_1728(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)o)[slot];
    return vEntry->fn(vthis(o, *vEntry));
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub_00160BF8_1728(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline t4Vector cmScaleP_1728(const t4Vector* v, float s)
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
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

extern "C" void cIdleChaseAlgorithm_update_v(char* self) __asm__("cIdleChaseAlgorithm_update");

/**
 * Runs one frame of the idle chase camera: the eye stays put and the view narrows as the rider moves away.
 *
 * After setupChaseData (cLookAtChaseAlgorithm_setupChaseData) sets CamTargetPos to the rider position (target slot 1) + Z * 13.5. When the
 * eye-to-target distance is positive, View = min(531.66 / distance, 1) * pi/4. Position is not changed.
 */
extern "C" void cIdleChaseAlgorithm_update_v(char* vself)
{
    cIdleChaseAlgorithm* self = (cIdleChaseAlgorithm*)vself;
    cChaseSetup info;
    cLookAtChaseAlgorithm_setupChaseData_1728(self, &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    t4Vector* camTargetPos = (t4Vector*)(&self->CamTargetPos);
    t4Vector* eyePos = (t4Vector*)(&self->Position);
    *camTargetPos = cmAdd_001607E0_1728(cmGet_001607E0_1728(((char*)self), 1), cmScaleP_1728(&t4vect_ZAxis_1728, 13.495580673217773f));
    t4Vector eyeToTarget = cmSub_00160BF8_1728(*camTargetPos, *eyePos);
    float len = cmLength_001607E0_1728(eyeToTarget);
    if (0.0f < len) {
        *(float*)self = (531.6604614257812f / len <? 1.0f) * 0.7853981852531433f;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cIdleChaseAlgorithm_snap);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector t4vect_XAxis;

extern t4Vector t4vect_ZAxis_1848 __asm__("t4vect_ZAxis");

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector cmAdd_001607E0_1848(const t4Vector& a, const t4Vector& b)
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

static inline t4Vector cmGet_001607E0_1848(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)o)[slot];
    return vEntry->fn(vthis(o, *vEntry));
}

extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(cLookAtChaseAlgorithm* self, float a, float b);

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline t4Vector cmScaleP_1848b(const t4Vector* v, float s)
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
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector plus vector at pointer).
static inline t4Vector cmAddP_1848(const t4Vector& a, const t4Vector* b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "o"(*b)
        : "memory");
    return r;
}

extern "C" void cIdleChaseAlgorithm_snap_v(char* self, int id) __asm__("cIdleChaseAlgorithm_snap");

/**
 * Snaps the idle chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match puts Position at the rider
 * position (target slot 1) + Z * 100.05 + the X axis (t4vect_XAxis), then runs the look-at chase snap,
 * setupInitialCamDir(559.744, 300) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cIdleChaseAlgorithm_snap_v(char* vself, int id)
{
    cIdleChaseAlgorithm* self = (cIdleChaseAlgorithm*)vself;
    char* obj = (char*)&self->Target->Rider->mVtbl;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)obj;
    if (id == vt[7].fn(vthis(obj, vt[7])))
    {
        self->Position = cmAddP_1848(cmAdd_001607E0_1848(cmGet_001607E0_1848(((char*)self), 1), cmScaleP_1848b(&t4vect_ZAxis_1848, 100.05000305175781f)), &t4vect_XAxis);
        cLookAtChaseAlgorithm_snap(self, id);
        cLookAtChaseAlgorithm_setupInitialCamDir((cLookAtChaseAlgorithm*)self, 559.7440185546875f, 300.0f);
        sVEntry<void(void*)>* vt2 = (sVEntry<void (void*)>*)self->mVtbl;
        vt2[5].fn(vthis(self, vt2[5]));
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_HardReset);
#ifdef SKIP_ASM
struct cChaseCameraController_switchCamera_HardReset_flags {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 1;
};

extern int persistentPlayerChoiceType[2];

/**
 * Fully resets the chase camera's switch state for a view.
 *
 * Clears the volume/replay/slow flags and timers, sets the preferred, replay and volume types to 0x3D (none),
 * drops the trigger action and restores the view's persistent player choice.
 *
 * @param i  view index into persistentPlayerChoiceType
 */
extern "C" void cChaseCameraController_switchCamera_HardReset(cChaseCameraController* self, int i)
{
    self->PreferredChaseAlgoLastFrame = 61;
    *(int*)&self->TimeSinceLastSuggestedVolumeTrigger = 0;
    ((cChaseCameraController_switchCamera_HardReset_flags*)((char*)self + 0x30))->b1 = 0; ((cChaseCameraController_switchCamera_HardReset_flags*)((char*)self + 0x30))->b0 = 0; ((cChaseCameraController_switchCamera_HardReset_flags*)((char*)self + 0x30))->b2 = 0;
    self->GoingSlow = 0;
    self->NumFramesSlowGround = 0;
    self->CamSwitchRecord.LatestReplayType = 61;
    self->CamSwitchRecord.PlayerChoiceType = persistentPlayerChoiceType[i];
    self->CamSwitchRecord.VolumeType = 61;
    self->CamSwitchRecord.TriggerAction = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", cChaseCameraController_cChaseCameraController);
#ifdef SKIP_ASM
extern void* cChaseCameraController_vtbl[];
extern "C" void* cCameraController_cCameraController(void* self, int a);
extern "C" void cChaseCameraController_switchCamera_HardReset(cChaseCameraController* self, int idx);

/**
 * Constructs a chase camera controller for one view.
 *
 * Runs the base cCameraController constructor, then sets the chase vtable, Type 2, BlendType 1 and no Target.
 * It then clears the camera-switch state with cChaseCameraController_switchCamera_HardReset(), which also
 * takes the view's persistent camera choice (persistentPlayerChoiceType).
 *
 * @param idx  view index, used to pick persistentPlayerChoiceType[idx]
 * @param camera    owning cCamera, passed as an int to the base constructor
 * @return @p self
 */
extern "C" void* cChaseCameraController_cChaseCameraController(cChaseCameraController* self, int idx, int camera)
{
    cCameraController_cCameraController(self, camera);
    self->mVtbl = cChaseCameraController_vtbl;
    self->Type = 2;
    self->BlendType = 1;
    self->Target = 0;
    cChaseCameraController_switchCamera_HardReset(self, idx);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController__cChaseCameraController);
#ifdef SKIP_ASM
extern void* cChaseCameraController_vtbl[];
void operator_delete(int* ptr);
extern "C" void cCameraController__cCameraController(void* self, int flags);

/**
 * Destroys the chase camera controller: frees its rider camera target, then runs the base controller destructor.
 *
 * Target is released with operator delete (no destructor runs). cCameraController__cCameraController() then
 * deletes the algorithm list and frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cChaseCameraController__cChaseCameraController(cChaseCameraController* self, int flags)
{
    self->mVtbl = cChaseCameraController_vtbl;
    operator_delete((int*)self->Target);
    cCameraController__cCameraController(self, flags);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_setTarget);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Replaces the chase controller's rider target, deleting the old one.
 *
 * @param target  the new cRiderCameraTarget (owned by the controller)
 */
extern "C" void cChaseCameraController_setTarget(cChaseCameraController* self, int* target)
{
    int* old = (int*)self->Target;
    if (old != 0) {
        operator_delete(old);
    }
    self->Target = (cRiderCameraTarget*)target;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_getRider);
#ifdef SKIP_ASM
/**
 * Returns the rider the chase camera follows: Target's Rider (Target is not checked for 0).
 *
 * @param chaseCameraController  the controller
 * @return the rider (a cRider*)
 */
extern "C" int cChaseCameraController_getRider(cChaseCameraController* chaseCameraController) {
    return ((int)chaseCameraController->Target->Rider);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_processInput);
#ifdef SKIP_ASM
struct sCam162138;
extern "C" void cChaseCameraController_switchCamera_UserChange(cChaseCameraController* self, int v);
extern "C" void cChaseCameraController_switchCamera_Slow(cChaseCameraController* self);
extern "C" void cChaseCameraController_switchCamera_EndSlow(cChaseCameraController* self);
int cRider_getMotionState(void*);

struct sCamVec4_161AB0
{
    float x, y, z, w;
} __attribute__((aligned(16)));

class cCamRider_161AB0
{
public:
    virtual void v01();
    virtual void v02();
    virtual sCamVec4_161AB0 getVelocity();
};

// PORT: VU0 macro-mode vector length; the PC port needs plain C.
static inline float vlength_161AB0(const sCamVec4_161AB0& v)
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
 * Per-frame chase camera choice: player preference and the slow camera.
 *
 * The preferred algorithm is always 0x3D here, so the user-change path only fires once after something else
 * was preferred (and not while the latest algorithm is 0x4A). The slow camera starts when the rider's motion
 * state is 5 (MS_Handplant) and ends once the speed (velocity x 0.036) is over 35.
 */
extern "C" void cChaseCameraController_processInput(cChaseCameraController* self)
{
    int mode = 61;
    if (self->PreferredChaseAlgoLastFrame != mode && self->LatestAlgoType != 74)
        cChaseCameraController_switchCamera_UserChange(self, mode);
    self->PreferredChaseAlgoLastFrame = mode;
    float speed = vlength_161AB0(((cCamRider_161AB0*)self->Target)->getVelocity()) * 0.035999998450279236f;
    int boarding = cRider_getMotionState(self->Target->Rider) == 5;
    if (self->GoingSlow == 0) {
        if (boarding) {
            self->GoingSlow = 1;
            cChaseCameraController_switchCamera_Slow(self);
        }
    } else if (speed > 35.0f) {
        self->GoingSlow = 0;
        cChaseCameraController_switchCamera_EndSlow(self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_update);
#ifdef SKIP_ASM
struct sCam162060;
extern "C" void cChaseCameraController_switchCamera_Volume(cChaseCameraController* self, int mode, int arg, float blend);
extern int cPauseState_Flags;

class cCamAlgo_161BB8 {
public:
    char pad_0x00[0xC];
    int type;  // 0xC
    // vptr at 0x10
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void updateAlgorithm(int on);
};



extern "C" cCameraAlgoListNode* cCameraAlgoList_remove(cCameraAlgoList* list, cCameraAlgoListNode* n);

static inline float Approach_161BB8(float cur, float target, float rate)
{
    if (target + rate < cur)
        return cur - rate;
    if (cur < target - rate)
        return cur + rate;
    return target;
}

static inline float ApproachZero_161BB8(float cur, float rate)
{
    if (rate < cur)
        return cur - rate;
    if (cur < -rate)
        return cur + rate;
    return 0.0f;
}

static inline void Smooth_161BB8(cCameraAlgoListNode* n, float u)
{
    float u2 = u * u;
    float s = u2 * 3.0f - (u2 + u2) * u;
    n->Weight = u;
    if (s > 1.0f)
        s = 1.0f;
    n->SmoothWeight = s;
}

static inline int InRace_161BB8(int* st)
{
    return *st && *st < 10;
}

/**
 * Updates the chase controller: blends its algorithm list and re-suggests the player's camera.
 *
 * While bit 0 of the pause-mode flags cPauseState_Flags is clear: processes input, blends the head algorithm's
 * weight towards 1 and the others towards 0, dropping finished ones (type 0x4A at once).
 *
 * During a replay (replay state 1-9) of type 0x5D showing another camera than the player's choice, switches
 * back to the player's choice after 10 s; the timer is held at 0 while the replay's word at +0x484 is 0.
 */
extern "C" void cChaseCameraController_update(cChaseCameraController* self)
{
    if ((cPauseState_Flags & 1) == 0) {
        cChaseCameraController_processInput(self);
        cCameraAlgoListNode* n = ((self->AlgorithmList)->Head);
        ((cCamAlgo_161BB8*)n->Algorithm)->updateAlgorithm(0);
        float rate = n->BlendRate;
        Smooth_161BB8(n, Approach_161BB8(n->Weight, 1.0f, rate));
        n = n->Next;
        while (n) {
            Smooth_161BB8(n, ApproachZero_161BB8(n->Weight, rate));
            if (((cCamAlgo_161BB8*)n->Algorithm)->type == 74)
                n->Weight = 0.0f;
            if (n->Weight <= 0.0f || n->SmoothWeight <= 0.0f) {
                n = cCameraAlgoList_remove(self->AlgorithmList, n);
            } else {
                ((cCamAlgo_161BB8*)n->Algorithm)->updateAlgorithm(1);
                n = n->Next;
            }
        }
    }
    int* st = (int*)SSXApp->Game->Replay;
    int idle = InRace_161BB8(st) && st[1156 / 4] == 0;
    if (idle)
        self->TimeSinceLastSuggestedVolumeTrigger = 0.0f;
    if (InRace_161BB8(st) && self->CamSwitchRecord.LatestReplayType == 93 && self->LatestAlgoType != self->CamSwitchRecord.PlayerChoiceType) {
        self->TimeSinceLastSuggestedVolumeTrigger += 0.01666666753590107f;
        if (self->TimeSinceLastSuggestedVolumeTrigger > 10.0f) {
            self->TimeSinceLastSuggestedVolumeTrigger = 0.0f;
            cChaseCameraController_switchCamera_Volume(self, self->CamSwitchRecord.PlayerChoiceType, 0, 1.0f);
        }
    } else {
        self->TimeSinceLastSuggestedVolumeTrigger = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_addAlgorithm);
#ifdef SKIP_ASM
extern "C" void cCameraAlgoList_insert(void* list, void* algo, int b, float w);
extern "C" void cCameraAlgoList_removeAll(void* list);

/**
 * Adds a chase algorithm to the controller's blend list.
 *
 * Records its type as LatestAlgoType. Into an empty list it goes in at weight 1; a blend rate of 1.0 clears
 * the list first (a cut).
 *
 * @param algo  the algorithm to add
 * @param w  blend rate (1.0 = cut)
 */
extern "C" void cChaseCameraController_addAlgorithm(cChaseCameraController* self, cCameraAlgorithm* algo, float w)
{
    self->LatestAlgoType = algo->AlgorithmType;
    cCameraAlgoList* list = self->AlgorithmList;
    if (list->Length == 0) {
        cCameraAlgoList_insert(list, algo, 1, 1.0f);
        return;
    }
    if (w == 1.0f) {
        cCameraAlgoList_removeAll(self->AlgorithmList);
    }
    cCameraAlgoList_insert(self->AlgorithmList, algo, 1, w);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_UserChange);
#ifdef SKIP_ASM
extern int persistentPlayerChoiceType[2];
extern "C" void* cChaseCameraController_createChaseAlgorithmCut(void*, int, int);

/**
 * Stores the player's chosen chase camera and switches to it when nothing overrides it.
 *
 * Also saves it in persistentPlayerChoiceType for the rider's index (+0x870); switches (cChaseCameraController_createChaseAlgorithmCut) only
 * when no volume, replay or slow flag is set.
 *
 * @param type  the chosen algorithm type
 */
extern "C" void cChaseCameraController_switchCamera_UserChange(cChaseCameraController* self, int type)
{
    self->CamSwitchRecord.PlayerChoiceType = type;
    persistentPlayerChoiceType[self->Target->Rider->mViewIndex] = type;
    if ((*(int*)((char*)self + 0x30) & 7) == 0) {
        cChaseCameraController_createChaseAlgorithmCut(self, type, 0);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_EndReplay);
#ifdef SKIP_ASM
struct sCam161F50 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int pad_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
};

extern "C" void* cChaseCameraController_createChaseAlgorithmCut(void*, int, int);

/**
 * Leaves replay camera mode and cuts back to the normal chase camera.
 *
 * Clears the volume and replay flags and cuts (cChaseCameraController_createChaseAlgorithmCut) to the slow camera 0x42 when slow, else the
 * player's choice, keeping the trigger action.
 */
extern "C" void cChaseCameraController_switchCamera_EndReplay(cChaseCameraController* self)
{
    int mode;
    ((sCam161F50*)self)->b1 = 0;
    ((sCam161F50*)self)->b0 = 0;
    mode = 66;
    if (!((sCam161F50*)self)->b2) {
        mode = self->CamSwitchRecord.PlayerChoiceType;
    }
    cChaseCameraController_createChaseAlgorithmCut(((sCam161F50*)self), mode, (int)self->CamSwitchRecord.TriggerAction);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_ReplayToggle);
#ifdef SKIP_ASM
/**
 * Enters replay camera mode with the given camera type.
 *
 * Algorithm types: 0x3D none, 0x42 slow camera, 0x44 end of race, 0x4C the player's choice, 0x5D replay. 0x4C
 * cuts to the player's choice; 0x5D cuts to the volume camera, the slow camera or the player's choice by the
 * current flags; other types are used as is. The type is kept as LatestReplayType.
 *
 * @param mode  replay camera type
 */
extern "C" void cChaseCameraController_switchCamera_ReplayToggle(cChaseCameraController* self, int mode)
{
    int flags = *(int*)((char*)self + 0x30) | 2;
    *(int*)((char*)self + 0x30) = flags;
    if (mode == 76) {
        mode = self->CamSwitchRecord.PlayerChoiceType;
        cChaseCameraController_createChaseAlgorithmCut(self, mode, 0);
    } else if (mode == 93) {
        if (flags & 1)
            cChaseCameraController_createChaseAlgorithmCut(self, self->CamSwitchRecord.VolumeType, (int)self->CamSwitchRecord.TriggerAction);
        else if (flags & 4)
            cChaseCameraController_createChaseAlgorithmCut(self, 66, 0);
        else
            cChaseCameraController_createChaseAlgorithmCut(self, self->CamSwitchRecord.PlayerChoiceType, 0);
    } else {
        cChaseCameraController_createChaseAlgorithmCut(self, mode, 0);
    }
    self->CamSwitchRecord.LatestReplayType = mode;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_Volume);
#ifdef SKIP_ASM
struct sCam162060 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int field_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
};

extern "C" void* cChaseCameraController_createChaseAlgorithmBlend(void*, int, int, float);

/**
 * Switches the chase camera for a camera-trigger volume.
 *
 * 0x4C (leave the volume) clears the volume flag and uses the player's choice; any other type is stored with
 * its action. The blend is made unless a replay is on with a type other than 0x5D.
 *
 * @param mode  camera type of the volume (0x4C = back to the player's choice)
 * @param action  the trigger's action (cCTAction *)
 * @param blend  blend rate
 */
extern "C" void cChaseCameraController_switchCamera_Volume(cChaseCameraController* self, int mode, int action, float blend)
{
    if (mode == 76) {
        ((sCam162060*)self)->b0 = 0;
        mode = self->CamSwitchRecord.PlayerChoiceType;
    } else {
        self->CamSwitchRecord.VolumeType = mode;
        ((sCam162060*)self)->b0 = 1;
        self->CamSwitchRecord.TriggerAction = (cCTAction*)action;
    }
    int ok = !((sCam162060*)self)->b1 ? 1 : self->CamSwitchRecord.LatestReplayType == 93;
    if (ok) {
        cChaseCameraController_createChaseAlgorithmBlend(((sCam162060*)self), mode, action, blend);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_Chapter);
#ifdef SKIP_ASM
struct sCam1620D0 {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
    int field_0x2C;
    unsigned int b0 : 1; // 0x30
    unsigned int b1 : 1;
    unsigned int b2 : 1;
    unsigned int rest : 29;
    int field_0x34;
};

extern "C" void cCamera_invalidateSafePosition();
extern "C" void* cChaseCameraController_createChaseAlgorithmBlend(void*, int, int, float);

/**
 * Re-applies the chase camera at a chapter change.
 *
 * Clears the volume flag, marks view 0's camera safe position invalid (cCamera_invalidateSafePosition sets its
 * LastSafePositionInvalid) and cuts to the latest replay type, or the player's choice when that is 0x5D.
 */
extern "C" void cChaseCameraController_switchCamera_Chapter(cChaseCameraController* self)
{
    int mode = self->CamSwitchRecord.LatestReplayType;
    ((sCam1620D0*)self)->b0 = 0;
    if (mode == 93)
        mode = self->CamSwitchRecord.PlayerChoiceType;
    cCamera_invalidateSafePosition();
    cChaseCameraController_createChaseAlgorithmBlend(((sCam1620D0*)self), mode, 0, 1.0f);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_SoftReset);
#ifdef SKIP_ASM
struct sCam162138 {
    char pad_0x00[0x24];
    int PlayerChoiceType;
    int VolumeType;
    int pad_0x2C;
    unsigned int IsInVolume : 1; // 0x30
    unsigned int IsInReplay : 1;
    unsigned int IsSlow : 1;
    unsigned int rest : 29;
    int TriggerAction;
    int pad_0x38;
    int field_0x3C;
    int field_0x40;
};


/**
 * Resets the chase camera's switch flags, keeping the player's choice.
 *
 * Clears the volume, replay and slow flags, the slow state and the trigger action and sets the volume type to
 * the player's choice.
 */
extern "C" void cChaseCameraController_switchCamera_SoftReset(cChaseCameraController* self)
{
    ((sCam162138*)self)->IsInReplay = 0;
    ((sCam162138*)self)->IsInVolume = 0;
    ((sCam162138*)self)->IsSlow = 0;
    self->CamSwitchRecord.VolumeType = self->CamSwitchRecord.PlayerChoiceType;
    self->GoingSlow = 0;
    self->NumFramesSlowGround = 0;
    self->CamSwitchRecord.TriggerAction = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_Restart);
#ifdef SKIP_ASM
extern "C" void* cChaseCameraController_createChaseAlgorithmCut(void*, int, int);

/**
 * Soft-resets the chase camera and cuts to the player's choice.
 *
 * @return whatever cChaseCameraController_createChaseAlgorithmCut returns (callers ignore it)
 */
extern "C" void* cChaseCameraController_switchCamera_Restart(cChaseCameraController* self)
{
    cChaseCameraController_switchCamera_SoftReset(self);
    return cChaseCameraController_createChaseAlgorithmCut(((sCam162138*)self), self->CamSwitchRecord.PlayerChoiceType, 0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_Slow);
#ifdef SKIP_ASM
int cRider_getMotionState(void*);

/**
 * Starts the slow-speed chase camera (0x42).
 *
 * Sets the slow flag; unless a volume or replay is active, blends to 0x42 at 1.2/60 per frame in motion state
 * 5 (handplant), else 0.6/60.
 */
extern "C" void cChaseCameraController_switchCamera_Slow(cChaseCameraController* self)
{
    ((sCam162138*)self)->IsSlow = 1;
    if (!((sCam162138*)self)->IsInVolume && !((sCam162138*)self)->IsInReplay) {
        float blendTime = cRider_getMotionState(self->Target->Rider) == 5 ? 1.2000000476837158f : 0.6000000238418579f;
        cChaseCameraController_createChaseAlgorithmBlend(((sCam162138*)self), 66, 0, blendTime * 0.01666666753590107f);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_EndSlow);
#ifdef SKIP_ASM
/**
 * Ends the slow-speed chase camera.
 *
 * Clears the slow flag; unless a volume or replay is active, blends back to the player's choice at about
 * 1/150 per frame.
 */
extern "C" void cChaseCameraController_switchCamera_EndSlow(cChaseCameraController* self)
{
    ((sCam162138*)self)->IsSlow = 0;
    if (!((sCam162138*)self)->IsInVolume && !((sCam162138*)self)->IsInReplay) {
        cChaseCameraController_createChaseAlgorithmBlend(((sCam162138*)self), self->CamSwitchRecord.PlayerChoiceType, 0, 0.0066666672937572f);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_EndRace);
#ifdef SKIP_ASM
/**
 * Blends to the end-of-race camera (0x44) unless a volume camera is active.
 */
extern "C" void cChaseCameraController_switchCamera_EndRace(cChaseCameraController* self)
{
    if (!((sCam162138*)self)->IsInVolume) {
        cChaseCameraController_createChaseAlgorithmBlend(((sCam162138*)self), 68, 0, 0.016793444752693176f);
    }
}
#endif

extern "C" void* cChaseCameraController_createChaseAlgorithmCut(void*, int, int);

//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_switchCamera_EndPostRace__FPv);
#ifdef SKIP_ASM
/**
 * Switches the chase camera back to the player's chosen algorithm with a cut.
 *
 * Calls cChaseCameraController_createChaseAlgorithmCut with CamSwitchRecord.PlayerChoiceType and no trigger action.
 *
 * @return cChaseCameraController_createChaseAlgorithmCut's result (declared void*)
 */
void* cChaseCameraController_switchCamera_EndPostRace(void* vself)
{
    cChaseCameraController* self = (cChaseCameraController*)vself;
    return cChaseCameraController_createChaseAlgorithmCut(self, self->CamSwitchRecord.PlayerChoiceType, 0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseAlgorithm_cChaseAlgorithm);
#ifdef SKIP_ASM
extern void* cCameraAlgorithm_vtbl[];
extern void* cChaseAlgorithm_vtbl[];
extern "C" float cCameraAlgorithm_getDefaultView(cCameraAlgorithm* self);

/**
 * Constructs a chase camera algorithm on top of the base camera algorithm.
 *
 * Stores the base vtable cCameraAlgorithm_vtbl, View = cCameraAlgorithm_getDefaultView (the default field of view), AlgorithmType 0x4E, Near 10
 * and Far 30000, then cChaseAlgorithm_vtbl and AlgorithmType 0x53.
 *
 * @return @p self
 */
extern "C" void* cChaseAlgorithm_cChaseAlgorithm(cChaseAlgorithm* self)
{
    self->mVtbl = cCameraAlgorithm_vtbl;
    *(float*)self = cCameraAlgorithm_getDefaultView((cCameraAlgorithm*)self);
    self->AlgorithmType = 78;
    self->Near = 10.0f;
    self->Far = 30000.0f;
    self->mVtbl = cChaseAlgorithm_vtbl;
    self->AlgorithmType = 83;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseAlgorithm_setTarget__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the rider a chase camera algorithm follows.
 *
 * Stores @p val at +0x30.
 *
 * @param val  the camera target (On Tour cRiderCameraTarget *camTarget)
 */
void cChaseAlgorithm_setTarget(void* vself, int val)
{
    cChaseAlgorithm* self = (cChaseAlgorithm*)vself;
    self->Target = (cRiderCameraTarget*)val;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_cLookAtChaseAlgorithm);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern void* cLookAtChaseAlgorithm_vtbl[];
extern cQuad128 t4vect_Zero_q162318 __asm__("t4vect_Zero");
extern "C" float cPIDFilter_getDefaultProportionFactor(void);
// PORT: these take an unused self; they are called here with none.
float cPIDFilter_getDefaultErrorHistoryFactor_0() __asm__("cPIDFilter_getDefaultErrorHistoryFactor");
float cPIDFilter_getDefaultDerrivativeFactor_0() __asm__("cPIDFilter_getDefaultDerrivativeFactor");
struct sCamFilter;
extern "C" void cPIDFilter_reset(cPIDFilter* self, float value);

/**
 * Constructs the race chase camera algorithm with its default tuning.
 *
 * Runs the cChaseAlgorithm constructor, stores cLookAtChaseAlgorithm_vtbl, constructs the five splines at
 * +0x304..+0x334 (cCubicSplineInterpolant_cCubicSplineInterpolant) and resets TurningPIDFilter (cPIDFilter_reset with 0). Then sets Position to
 * t4vect_Origin, pitch, heading and roll 0, AlgorithmType 0x54, the zero vectors (LastVelLookahead, LastRailMove and the
 * half-pipe moves), LastDist 200, LastDrop 0, LastViewScalar 1, SecondsSinceLastHandplant 10, FramesSinceHandplant
 * 5000, half-pipe flags 0, View pi/4 and the PID factors (P 0.0114, history 0, D 0.00128).
 *
 * @return @p self
 */
// Constructor: base ctor, five splines, the filter and default tuning.
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(cLookAtChaseAlgorithm* self)
{
    cChaseAlgorithm_cChaseAlgorithm((cChaseAlgorithm*)self);
    sCamFilter* filter = (sCamFilter*)(&self->TurningPIDFilter);
    self->mVtbl = cLookAtChaseAlgorithm_vtbl;
    cCubicSplineInterpolant_cCubicSplineInterpolant(&self->AirTargetSpline);
    cCubicSplineInterpolant_cCubicSplineInterpolant(&self->AirPositionSpline);
    cCubicSplineInterpolant_cCubicSplineInterpolant(&self->FullAirTargetSpline);
    cCubicSplineInterpolant_cCubicSplineInterpolant(&self->FullAirPositionSpline);
    cCubicSplineInterpolant_cCubicSplineInterpolant(&self->HalfPipeAngleSpline);
    self->TurningPIDFilter.ProportionFactor = cPIDFilter_getDefaultProportionFactor();
    self->TurningPIDFilter.ErrorHistoryFactor = cPIDFilter_getDefaultErrorHistoryFactor_0();
    self->TurningPIDFilter.DerrivativeFactor = cPIDFilter_getDefaultDerrivativeFactor_0();
    cPIDFilter_reset((cPIDFilter*)filter, 0.0f);
    *(cQuad128*)&self->Position = t4vect_Origin;
    *(int*)&self->Pitch = 0;
    self->AlgorithmType = 84;
    *(int*)&self->Heading = 0;
    *(int*)&self->CamRoll = 0;
    *(cQuad128*)&self->LastVelLookahead = t4vect_Zero_q162318;
    self->LastDist = 200.0f;
    *(int*)&self->LastDrop = 0;
    self->LastPitch = self->Pitch;
    self->LastViewScalar = 1.0f;
    self->SecondsSinceLastHandplant = 10.0f;
    self->LastHeight = self->Pitch;
    self->FramesSinceHandplant = 5000;
    self->DoingHalfPipeCam = 0;
    self->HalfPipeCamInitialized = 0;
    *(cQuad128*)&self->LastRailMove = t4vect_Zero_q162318;
    *(cQuad128*)&self->LastHalfPipeMove = t4vect_Zero_q162318;
    *(cQuad128*)&self->LastHalfPipeDistCorrection = t4vect_Zero_q162318;
    *(float*)self = 0.7853981852531433f;
    self->TurningPIDFilter.DerrivativeFactor = 0.0012785084545612335f;
    self->TurningPIDFilter.ErrorHistoryFactor = self->Pitch;
    self->TurningPIDFilter.ProportionFactor = 0.01139452401548624f;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm__cLookAtChaseAlgorithm);
#ifdef SKIP_ASM
extern void* cLookAtChaseAlgorithm_vtbl[];
extern void* cCameraAlgorithm_vtbl[];

/**
 * Destroys a chase camera algorithm: its five splines, then the base camera algorithm part.
 *
 * Restores cLookAtChaseAlgorithm_vtbl, runs cCubicSplineInterpolant__cCubicSplineInterpolant (flags 2) on the splines at +0x334 down to +0x304 and
 * restores the base vtable cCameraAlgorithm_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (operator delete)
 */
// PORT: cLookAtChaseAlgorithm__cLookAtChaseAlgorithm is the base deleting dtor (self, flags); the unit declares it with one arg
// (cLookAtChaseAlgorithm__cLookAtChaseAlgorithm_dtor is declared earlier in the unit with this asm label).
void cLookAtChaseAlgorithm__cLookAtChaseAlgorithm_dtor(void* vself, int flags)
{
    cLookAtChaseAlgorithm* self = (cLookAtChaseAlgorithm*)vself;
    self->mVtbl = cLookAtChaseAlgorithm_vtbl;
    cCubicSplineInterpolant__cCubicSplineInterpolant(&self->HalfPipeAngleSpline, 2);
    cCubicSplineInterpolant__cCubicSplineInterpolant(&self->FullAirPositionSpline, 2);
    cCubicSplineInterpolant__cCubicSplineInterpolant(&self->FullAirTargetSpline, 2);
    cCubicSplineInterpolant__cCubicSplineInterpolant(&self->AirPositionSpline, 2);
    cCubicSplineInterpolant__cCubicSplineInterpolant(&self->AirTargetSpline, 2);
    self->mVtbl = cCameraAlgorithm_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateAlgorithm);
#ifdef SKIP_ASM

/**
 * Runs one frame of the chase camera: snaps first when DoSnap is set, then updates through its virtuals.
 *
 * Calls slot 4 (snapCurrent) when DoSnap is set, then slot 5 (the subclass's per-frame setup) and slot 8
 * (cLookAtChaseAlgorithm_doRaceCamFinalPhase) with @p ignoreCollisions.
 *
 * @param ignoreCollisions  passed on to slot 8 (On Tour updateAlgorithm(bool ignoreCollisions))
 */
extern "C" void cLookAtChaseAlgorithm_updateAlgorithm(cLookAtChaseAlgorithm* self, int ignoreCollisions)
{
    if (self->DoSnap != 0) {
        sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)self->mVtbl;
        vt[4].fn(vthis(self, vt[4]));
    }
    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)self->mVtbl;
    vt[5].fn(vthis(self, vt[5]));
    sVEntry<void(void*, int)>* vt2 = (sVEntry<void(void*, int)>*)self->mVtbl;
    vt2[8].fn(vthis(self, vt2[8]), ignoreCollisions);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00162560);
#ifdef SKIP_ASM
/**
 * Does nothing (nothing in the code or data refers to it).
 */
extern "C" void func_00162560(void) {

}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_setupChaseData);
#ifdef SKIP_ASM
int cRider_getMotionState(void*);
extern t4Vector t4vect_Zero_v162568 __asm__("t4vect_Zero");

struct sVEi_162568 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sVEf_162568 {
    short delta;
    short index;
    float (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub_162568(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (dot product).
static inline float cmDot_162568(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v / |v| via vrsqrt).
static inline t4Vector cmNormalized_162568(const t4Vector& v)
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

static inline int cmGetI_162568(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEi_162568* e = &(*(sVEi_162568**)o)[slot];
    return e->fn(vthis(o, *e));
}

static inline float cmGetF_162568(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEf_162568* e = &(*(sVEf_162568**)o)[slot];
    return e->fn(vthis(o, *e));
}

static inline t4Vector cmLerp_162568(const t4Vector& a, const t4Vector& b, float t)
{
    t4Vector r = cmAdd_001607E0(cmScale_001607E0(a, t), cmScale_001607E0(b, 1.0f - t));
    return r;
}

static inline t4Vector cmGetR_162568(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEv_001607E0* e = &(*(sVEv_001607E0**)o)[slot];
    t4Vector r = e->fn(vthis(o, *e));
    return r;
}

static inline void SetFrame_162568(char* out, const t4Vector& pos, t4Vector dir, t4Vector vel)
{
    *(t4Vector*)(out + 0x0) = pos;
    *(t4Vector*)(out + 0x10) = vel;
    *(t4Vector*)(out + 0x20) = dir;
}

extern "C" void cLookAtChaseAlgorithm_setupChaseData_impl(char* self, char* out, float a, float b, float c, float d, float e, float f,
                                   float g, float h) __asm__("cLookAtChaseAlgorithm_setupChaseData");
/**
 * Fills a chase setup with this frame's target data and the smoothed target velocity.
 *
 * The target's velocity (Target virtual slot 3, cRiderCameraTarget_getVelocity) is replaced by LastOriginalVel when
 * slower than 27.78 and stored back there. Its horizontal part is smoothed into LastHorizontalVel with a lerp factor
 * between the min and max smoothness, picked by the steepness of the velocity (|z| of its direction, mapped from the
 * start slope to the end slope): the powder parameters (@p powderVelMinSmoothness .. @p powderVelEndSlope) when the target's surface type (slot 20) is 2, 3
 * or 13, else the normal ones (@p normalVelMinSmoothness .. @p normalVelEndSlope). While airborne (motion state 1) with a horizontal speed under 27.78,
 * LastHorizontalVel is the target of the lerp instead.
 *
 * The setup gets: +0x0 (SavedPos) = Position, +0x10 (TPos) = the target's position (slot 1), +0x20 (TVel) = smoothed
 * horizontal velocity + the vertical part, +0x40 the motion state (cRider_getMotionState of Target's Rider), +0x44 the
 * length of TVel, +0x4C the boost (slot 17), +0x50 (boost + 1) / 2 when the boost is over 0 else 0, +0x54 the crouch
 * (slot 18), and the words +0x58 (state 2), +0x5C (state 5), +0x60 (state 1) and +0x64 (state 4). Setup_Boost,
 * Setup_VisualBoost and Setup_IsTargetHandplanting copy +0x4C, +0x50 and +0x5C.
 *
 * @param out  the setup to fill (On Tour's cChaseSetup)
 * @param normalVelMinSmoothness  minimum smoothness on normal snow
 * @param normalVelMaxSmoothness  maximum smoothness on normal snow
 * @param normalVelStartSlope  slope at which normal smoothing starts rising from the minimum
 * @param normalVelEndSlope  slope at which normal smoothing reaches the maximum
 * @param powderVelMinSmoothness  minimum smoothness on powder
 * @param powderVelMaxSmoothness  maximum smoothness on powder
 * @param powderVelStartSlope  slope at which powder smoothing starts rising
 * @param powderVelEndSlope  slope at which powder smoothing reaches the maximum
 */
extern "C" void cLookAtChaseAlgorithm_setupChaseData_impl(char* self, char* out, float normalVelMinSmoothness, float normalVelMaxSmoothness, float normalVelStartSlope, float normalVelEndSlope, float powderVelMinSmoothness, float powderVelMaxSmoothness,
                                   float powderVelStartSlope, float powderVelEndSlope)
{
    t4Vector targetVelocity = cmGet_001607E0(self, 3);
    int mode = cRider_getMotionState(*(void**)(*(char**)(self + 0x30) + 4));
    if (cmLength_001607E0(targetVelocity) < 27.77777862548828f) {
        targetVelocity = *(t4Vector*)(self + 0x1B0);
    }
    *(t4Vector*)(self + 0x1B0) = targetVelocity;
    t4Vector w = targetVelocity;
    w.z = 0.0f;
    t4Vector dz = cmSub_162568(targetVelocity, w);
    t4Vector n = cmDot_162568(targetVelocity, targetVelocity) > 0.0f ? cmNormalized_162568(targetVelocity) : t4vect_Zero_v162568;
    int k = cmGetI_162568(self, 20);
    float lo, hi, startSlope, endSlope;
    if ((unsigned)(k - 2) < 2 || k == 13) {
        lo = powderVelMinSmoothness;
        hi = powderVelMaxSmoothness;
        startSlope = powderVelStartSlope;
        endSlope = powderVelEndSlope;
    } else {
        lo = normalVelMinSmoothness;
        hi = normalVelMaxSmoothness;
        startSlope = normalVelStartSlope;
        endSlope = normalVelEndSlope;
    }
    float x = __builtin_fabsf(n.z);
    float frac;
    if (x >= startSlope) {
        if (x <= endSlope) {
            frac = (x - startSlope) / (endSlope - startSlope);
        } else {
            frac = 1.0f;
        }
    } else {
        frac = 0.0f;
    }
    float smoothness = lo + (hi - lo) * frac;
    if (mode == 1) {
        if (cmLength_001607E0(w) < 27.77777862548828f) {
            w = *(t4Vector*)(self + 0x100);
        }
    }
    t4Vector nf = cmLerp_162568(*(t4Vector*)(self + 0x100), w, smoothness);
    *(t4Vector*)(self + 0x100) = nf;
    t4Vector chaseVelocity = cmAdd_001607E0(nf, dz);
    *(int*)(out + 0x64) = mode == 4;
    *(int*)(out + 0x58) = mode == 2;
    *(int*)(out + 0x5C) = mode == 5;
    *(int*)(out + 0x60) = mode == 1;
    *(int*)(out + 0x40) = mode;
    *(float*)(out + 0x4C) = cmGetF_162568(self, 17);
    *(float*)(out + 0x54) = cmGetF_162568(self, 18);
    SetFrame_162568(out, *(t4Vector*)(self + 0x40), chaseVelocity, cmGetR_162568(self, 1));
    *(float*)(out + 0x44) = cmLength_001607E0(*(t4Vector*)(out + 0x20));
    float visualBoost;
    if (*(float*)(out + 0x4C) > 0.0f) {
        visualBoost = (*(float*)(out + 0x4C) + 1.0f) * 0.5f;
    } else {
        visualBoost = 0.0f;
    }
    *(float*)(out + 0x50) = visualBoost;
    *(float*)(self + 0x2F4) = *(float*)(out + 0x4C);
    *(float*)(self + 0x2F8) = *(float*)(out + 0x50);
    *(int*)(self + 0x2FC) = *(int*)(out + 0x5C);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_setupHandplantBlend);
#ifdef SKIP_ASM
/**
 * Sets the chase setup's handplant blend-out weight (+0x48, On Tour HandplantBlendOut) from the frames since the last
 * handplant.
 *
 * While the setup says the rider is handplanting (+0x5C) FramesSinceHandplant is reset and the weight is 1. Otherwise
 * the counter counts up (capped at 820); the weight stays 1 for 120 frames, then falls by 1/700 per frame to 0.
 *
 * @param obj  the frame's chase setup (On Tour cChaseSetup& chaseSetup_outArg)
 */
extern "C" void cLookAtChaseAlgorithm_setupHandplantBlend(cLookAtChaseAlgorithm* self, cChaseSetup* obj)
{
    float handplantBlendOut;
    if (obj->Handplanting == 0) {
        if (self->FramesSinceHandplant < 820)
            self->FramesSinceHandplant = self->FramesSinceHandplant + 1;
        int elapsedFrames = self->FramesSinceHandplant;
        handplantBlendOut = 1.0f;
        if (elapsedFrames >= 0x78)
            handplantBlendOut = 1.0f - ((float)elapsedFrames - 120.0f) * 0.0014285714132711291f;
        if (handplantBlendOut < 0.0f)
            handplantBlendOut = 0.0f;
    } else {
        self->FramesSinceHandplant = 0;
        handplantBlendOut = 1.0f;
    }
    obj->HandplantBlendOut = handplantBlendOut;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_setupHalfPipeCam);
#ifdef SKIP_ASM
extern "C" int AITick();



// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_162A20(const t4Vector& v)
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
static inline void DivEq_162A20(t4Vector& v, float s)
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

static inline void Normalize_162A20(t4Vector& v)
{
    DivEq_162A20(v, Length_162A20(v));
}

/**
 * Tracks take-offs for the air cameras and decides whether the half-pipe swing camera should run.
 *
 * While the setup is airborne (+0x60): on the first airborne frame (AirborneLastFrame clear), when the speed (+0x44)
 * is positive, the normalised velocity (+0x20) points steeply up (z > 0.85) and the target's last ground normal (slot
 * 9) is nearly horizontal (z < 0.1, a wall), it starts the half-pipe cam (HalfPipeInitWait 0, DoingHalfPipeCam 1). It
 * then sets AirborneLastFrame and, when no air function is running and at least 31 AI ticks (AITick, AITick)
 * have passed since LastAirFrame, AirCamSetupCountdown = 15. Every airborne frame stores the AI tick in LastAirFrame.
 * On the first frame back on the ground it clears AirborneLastFrame, DoingHalfPipeCam and HalfPipeCamInitialized.
 *
 * @param trig  the frame's chase setup, not a trigger (On Tour cChaseSetup& setup_outArg)
 */
extern "C" void cLookAtChaseAlgorithm_setupHalfPipeCam(cLookAtChaseAlgorithm* self, cChaseSetup* trig)
{
    if (trig->Airborne)
    {
        if (self->AirborneLastFrame == 0)
        {
            t4Vector dir = trig->TVel;
            if (trig->TargetSpeed > 0.0f)
            {
                Normalize_162A20(dir);
                if (dir.z > 0.8500000238418579f)
                {
                    cRiderCameraTarget* obj = self->Target;
                    sVEntry<t4Vector(void*)>* vt = *(sVEntry<t4Vector(void*)>**)obj;
                    t4Vector groundNormal = vt[9].fn(vthis(obj, vt[9]));
                    if (groundNormal.z < 0.10000000149011612f)
                    {
                        self->HalfPipeInitWait = 0.0f;
                        self->DoingHalfPipeCam = 1;
                    }
                }
            }
            self->AirborneLastFrame = 1;
            int late = AITick() - self->LastAirFrame >= 31;
            if (self->ApplyingAirFunc == 0 && late)
                self->AirCamSetupCountdown = 15;
        }
        else
        {
            self->LastAirFrame = AITick();
            return;
        }
    }
    else if (self->AirborneLastFrame)
    {
        self->AirborneLastFrame = 0;
        self->DoingHalfPipeCam = 0;
        self->HalfPipeCamInitialized = 0;
    }
    if (trig->Airborne)
        self->LastAirFrame = AITick();
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateCamTarget);
#ifdef SKIP_ASM
/**
 * Aims the chase camera at the rider: copies the setup's target position (+0x10, On Tour TPos) into CamTargetPos.
 *
 * @param setup  the frame's chase setup (On Tour const cChaseSetup&)
 * @return the copied position (On Tour's returns void)
 */
extern "C" cQuad128 cLookAtChaseAlgorithm_updateCamTarget(cLookAtChaseAlgorithm* self, cChaseSetup* setup)
{
    cQuad128 targetPos = *(cQuad128*)&setup->TPos;
    *(cQuad128*)&self->CamTargetPos = targetPos;
    return targetPos;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateCamTargetLate);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector t4vect_ZAxis_2B90 __asm__("t4vect_ZAxis");

// The per-frame camera input record (0x70 bytes) that cLookAtChaseAlgorithm_setupChaseData fills from the target for the chase algorithms.
struct sCamInfo {
    t4Vector f00;               // 0x00
    t4Vector pos;               // 0x10
    t4Vector dir;               // 0x20
    t4Vector out;               // 0x30: written back by cLookAtChaseAlgorithm_setupCamDirection
    int mode;                   // 0x40
    float speed;                // 0x44: length of dir
    char pad48[4];
    float f4C;                  // 0x4C
    float f50;                  // 0x50
    float f54;                  // 0x54
    int f58;                    // 0x58
    int f5C;                    // 0x5C
    int f60;                    // 0x60
    int f64;                    // 0x64
};

extern "C" void cLookAtChaseAlgorithm_verify_v(void* self) __asm__("cLookAtChaseAlgorithm_verify");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_162B90(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void AddTo_162B90(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

static inline float Min_162B90(float a, float b)
{
    return a <? b;
}

static inline float ClampLo_162B90(float v, float lo)
{
    if (v >= lo) {
        return v;
    }
    return lo;
}



extern "C" void cLookAtChaseAlgorithm_updateCamTargetLate_i(float a, float b, float c, float d, float e, cLookAtChaseAlgorithm* self, sCamInfo* src) __asm__("cLookAtChaseAlgorithm_updateCamTargetLate");
/**
 * Raises the look-at point above the rider by a smoothed height that grows with speed, boost and crouch.
 *
 * t = min(max(speed - 1388.9, 0) / 1944.4, 1) (speeds are cm/s: 50 km/h up to 120 km/h). The crouch term x is the
 * setup's Crouch (+0x54), or LastTargetCrouchFactor while airborne (+0x60), and is stored in LastTargetCrouchFactor.
 * LastHeight = e * LastHeight + (1 - e) * (a + t * b + Boost (+0x4C) * d + c * x); CamTargetPos += Z * LastHeight,
 * then cLookAtChaseAlgorithm_verify (the eye sanity check) runs.
 *
 * The floats come first in the declaration: they travel in $f12..$f16 wherever they sit, so the callers' order
 * (setup, then the five floats) still lines up.
 *
 * @param defaultHeight  base height (On Tour defaultHeight)
 * @param height  extra height at full speed (On Tour height)
 * @param crouchCorrection  height per unit of crouch (On Tour crouchCorrection)
 * @param boostHeight  height per unit of boost (On Tour boostHeight)
 * @param smoothness  smoothing: the weight kept from last frame (On Tour smoothness)
 * @param src  the frame's chase setup (On Tour const cChaseSetup&)
 */
extern "C" void cLookAtChaseAlgorithm_updateCamTargetLate_i(float defaultHeight, float height, float crouchCorrection, float boostHeight, float smoothness, cLookAtChaseAlgorithm* self, sCamInfo* src)
{
    float dist = src->speed - 1388.888916015625f;
    float speedFrac = Min_162B90(ClampLo_162B90(dist, 0.0f) * 0.0005142856971360743f, 1.0f);
    float x;
    if (src->f60 != 0) x = self->LastTargetCrouchFactor;
    else x = src->f54;
    self->LastTargetCrouchFactor = x;
    float smoothedHeight = smoothness * self->LastHeight + (1.0f - smoothness) * (defaultHeight + speedFrac * height + src->f4C * boostHeight + crouchCorrection * x);
    self->LastHeight = smoothedHeight;
    AddTo_162B90(self->CamTargetPos, Scale_162B90(t4vect_ZAxis_2B90, smoothedHeight));
    cLookAtChaseAlgorithm_verify_v(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_setupCamDirection);
#ifdef SKIP_ASM

struct sVE_162C78 {
    short delta;
    short index;
    t4Vector (*fn)(void*);
};


struct sHist_162C78 {
    float a[5];         // 0x00
    float b[5];         // 0x14
    float c[5];         // 0x28
    int idx;            // 0x3C
    float k0;           // 0x40
    float k1;           // 0x44
    float k2;           // 0x48
};

extern "C" float func_0031C128(float x);

// PORT: PS2-only VU0 inline asm (dot product).
static inline float Dot_162C78(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector Scale_162C78(const t4Vector& v, float s)
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
static inline t4Vector Add_162C78(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_162C78(const t4Vector& v)
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
static inline void DivEq_162C78(t4Vector& v, float s)
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

static inline void Normalize_162C78(t4Vector& v)
{
    DivEq_162C78(v, Length_162C78(v));
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_162C78(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `<?` / `>?` operators.
static inline float Clamp_162C78(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

static inline t4Vector Get_162C78(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_162C78* e = &(*(sVE_162C78**)o)[slot];
    return e->fn(vthis(o, *e));
}

static inline t4Vector Lerp_162C78(const t4Vector& p, const t4Vector& q, float t)
{
    return Add_162C78(Scale_162C78(p, t), Scale_162C78(q, 1.0f - t));
}

static inline void LerpTo_162C78(t4Vector& out, const t4Vector& p, const t4Vector& q, float t)
{
    out = Add_162C78(Scale_162C78(p, t), Scale_162C78(q, 1.0f - t));
}

extern "C" void cLookAtChaseAlgorithm_setupCamDirection_impl(char* self, cChaseSetup* info) __asm__("cLookAtChaseAlgorithm_setupCamDirection");
/**
 * Computes the frame's chase direction (setup +0x30, On Tour CameraDirection) from the rider's velocity, damping
 * turns with TurningPIDFilter.
 *
 * Above 27.8 cm/s (1 km/h): takes the target's velocity (slot 3; LastDirectionVel when shorter than 1) into
 * LastDirectionVel and gets an angle term between it and LastCamDir (pi/2 - func_0031C128 (acosf) of their normalised
 * dot product), less 0.19 rad and floored at 0. The PID filter at +0x340 runs inline on that term (InputHistory,
 * ResultHistory, Difference, NextFreeIndex). Its result sets a weight s from 0.995 (no turn) down to 0.956 (0.40 or
 * more); s becomes its fourth root for rider motion states 0xB..0xD (rider +0x434). LastCamDir = s * LastCamDir + (1
 * - s) * velocity / speed. Below that speed LastCamDir is kept. The normalised result goes to the setup.
 *
 * @param info  the frame's chase setup (On Tour cChaseSetup& chaseSetup_outArg)
 */
extern "C" void cLookAtChaseAlgorithm_setupCamDirection_impl(char* self, cChaseSetup* info)
{
    t4Vector n;
    if (info->TargetSpeed > 27.77777862548828f) {
        t4Vector velocity = Get_162C78(self, 3);
        if (Dot_162C78(velocity, velocity) < 1.0f) {
            velocity = *(t4Vector*)(self + 0x1C0);
        }
        *(t4Vector*)(self + 0x1C0) = velocity;
        float ang;
        float len = Sqrt_162C78(Dot_162C78(*(t4Vector*)(self + 0x80), *(t4Vector*)(self + 0x80)) *
                                Dot_162C78(*(t4Vector*)(self + 0x1C0), *(t4Vector*)(self + 0x1C0)));
        if (len < 0.0010000000474974513f) {
            ang = 0.0f;
        } else {
            ang = 1.5707963705062866f -
                  func_0031C128(Clamp_162C78(
                      Dot_162C78(*(t4Vector*)(self + 0x80), *(t4Vector*)(self + 0x1C0)) / len, -1.0f, 1.0f));
        }
        ang = (ang - 0.19082477688789368f) >? 0.0f;
        float prev = ((sHist_162C78*)(self + 0x340))->b[(((sHist_162C78*)(self + 0x340))->idx + 4) % 5];
        float error = ang - ((sHist_162C78*)(self + 0x340))->b[(((sHist_162C78*)(self + 0x340))->idx + 4) % 5];
        sHist_162C78* pidFilter = (sHist_162C78*)(self + 0x340);
        float sum = 0.0f;
        for (int i = 0; i < 5; i++) {
            sum += pidFilter->c[i];
        }
        int prevIndex = (pidFilter->idx + 4) % 5;
        float val = prev + error * pidFilter->k0 + sum * pidFilter->k1 + (pidFilter->a[prevIndex] - ang) * pidFilter->k2;
        pidFilter->a[pidFilter->idx] = ang;
        pidFilter->b[pidFilter->idx] = val;
        pidFilter->c[pidFilter->idx] = ang - val;
        float smoothing = (1.0f - ((val * 2.5214130878448486f) <? 1.0f >? 0.0f)) * 0.03946405649185181f +
                  0.95552659034729f;
        pidFilter->idx = (pidFilter->idx + 1) % 5;
        if ((unsigned)(*(int*)(*(char**)(*(char**)(self + 0x30) + 4) + 0x434) - 11) < 3) {
            smoothing = Sqrt_162C78(Sqrt_162C78(smoothing));
        }
        LerpTo_162C78(n, *(t4Vector*)(self + 0x80), Scale_162C78((*(&info->TVel)), 1.0f / info->TargetSpeed), smoothing);
        *(t4Vector*)(self + 0x80) = n;
    } else {
        n = *(t4Vector*)(self + 0x80);
    }
    Normalize_162C78(n);
    (*(&info->CameraDirection)) = n;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateFollowDistance);
#ifdef SKIP_ASM
// FAKEMATCH: a dead two-statement chain `int u = 0; int w = u;` in the select's else arm (it outlives jump1's unused-set pass, so the
// `pk = &f` select is rewritten only after purge_addressof, which then gives g the lower stack slot); plain form 99.95%. Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void* cLookAtChaseAlgorithm_verify(void* self);


// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_163010(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_163010(const t4Vector& v)
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_163010(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_163010(const t4Vector& a, const t4Vector& b)
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

extern "C" void cLookAtChaseAlgorithm_updateFollowDistance_impl(char* self, char* trig, float a0, float b, float c, float d, float e, float f, float g) __asm__("cLookAtChaseAlgorithm_updateFollowDistance");
/**
 * Sets how far the eye trails the look-at point, from the rider's speed and boost, smoothed in LastDist.
 *
 * The wanted distance is a0 + b * 1e-4 * h^1.5 with h = max(speed (+0x44) - c, 0), clamped to [a0, 5 * a0]. When the
 * setup's Boost (+0x4C) rises above LastBoostWarp, ExtraBoostDist restarts at d * sqrt(boost); each frame it loses e
 * (floored at 0) and is added on; LastBoostWarp takes the boost. LastDist = k * LastDist + (1 - k) * wanted, with k =
 * g when the distance shrinks and f when it grows. Position = CamTargetPos - CameraDirection (+0x30) scaled to
 * LastDist, then cLookAtChaseAlgorithm_verify (the eye sanity check). Callers pass seven floats; On Tour's eighth
 * (defaultViewScalar) is gone.
 *
 * @param trig  the frame's chase setup, not a trigger (On Tour const cChaseSetup& setup)
 * @param stoppedDist  distance when stopped (On Tour stoppedDist)
 * @param distPerSpeed  distance added with speed (On Tour distPerSpeed)
 * @param reduceSpeed  speed below which no distance is added (On Tour reduceSpeed)
 * @param boostDist  boost distance (On Tour boostDist)
 * @param boostBleed  boost distance lost per frame (On Tour boostBleed)
 * @param accelSmoothness  smoothing while the distance grows (On Tour accelSmoothness)
 * @param decelSmoothness  smoothing while it shrinks (On Tour decelSmoothness)
 */
extern "C" void cLookAtChaseAlgorithm_updateFollowDistance_impl(char* self, char* trig, float stoppedDist, float distPerSpeed, float reduceSpeed, float boostDist, float boostBleed, float accelSmoothness, float decelSmoothness)
{
    t4Vector cameraDirection = *(t4Vector*)(trig + 0x30);
    float reducedSpeed = *(float*)(trig + 0x44) - reduceSpeed;
    if (reducedSpeed < 0.0f)
        reducedSpeed = 0.0f;
    float idealDist = stoppedDist + distPerSpeed * (reducedSpeed * 9.999999747378752e-05f) * Sqrt_163010(reducedSpeed);
    idealDist = (stoppedDist * 5.0f) <? (idealDist >? stoppedDist);
    if (*(float*)(self + 0x1EC) < *(float*)(trig + 0x4C))
        *(float*)(self + 0x1E8) = boostDist * Sqrt_163010(*(float*)(trig + 0x4C));
    float extra = (*(float*)(self + 0x1E8) - boostBleed) >? 0.0f;
    *(float*)(self + 0x1EC) = *(float*)(trig + 0x4C);
    *(float*)(self + 0x1E8) = extra;
    idealDist += extra;
    const float* pk;
    if (idealDist < *(float*)(self + 0x1D0))
        pk = &decelSmoothness;
    else
    {
        int deadZero = 0; // FAKEMATCH: dead chain, see header
        int w = deadZero;
        pk = &accelSmoothness;
    }
    const float& k = *pk;
    float cur = k * *(float*)(self + 0x1D0) + (1.0f - k) * idealDist;
    *(float*)(self + 0x1D0) = cur;
    ScaleEq_163010(cameraDirection, cur / Length_163010(cameraDirection));
    t4Vector eyePos = Sub_163010(*(t4Vector*)(self + 0x20), cameraDirection);
    *(t4Vector*)(self + 0x40) = eyePos;
    cLookAtChaseAlgorithm_verify(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateLateralShift);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector t4vect_ZAxis_3158 __asm__("t4vect_ZAxis");

extern "C" void* cLookAtChaseAlgorithm_verify(void* self);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_163158(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__ volatile(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add into destination).
static inline void AddTo_163158(t4Vector& r, const t4Vector& a, const t4Vector& b)
{
    __asm__ volatile(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_163158(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__ volatile(
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
static inline void ScaleEq_163158(t4Vector& v, float s)
{
    int t;
    __asm__ volatile(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector Cross_163158(const t4Vector* a, const t4Vector& b)
{
    t4Vector r;
    __asm__ volatile(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "o"(*a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_163158(const t4Vector& v)
{
    float r;
    int t;
    __asm__ volatile(
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
 * Shifts the eye, and part of the way the look-at point, sideways from the line of sight.
 *
 * The shift is Z x (CamTargetPos - Position) scaled to @p proportion times the eye distance. When it is nonzero,
 * Position moves by it and CamTargetPos by it times @p targetProportion. Ends with cLookAtChaseAlgorithm_verify (the eye sanity
 * check).
 *
 * @param self_  the chase algorithm (this)
 * @param info  the frame's chase setup; not read
 * @param proportion  the sideways shift as a fraction of the eye distance; the sign picks the side
 * @param targetProportion  the fraction of the shift also applied to the look-at point
 */
extern "C" void cLookAtChaseAlgorithm_updateLateralShift(cLookAtChaseAlgorithm* self_, void* info, float proportion, float targetProportion)
{
    float zero = 0.0f;
    t4Vector look = Sub_163158(self_->CamTargetPos, self_->Position);
    t4Vector left = Cross_163158(&t4vect_ZAxis_3158, look);
    float len = Length_163158(left);
    if (len > zero) {
        ScaleEq_163158(left, proportion * Length_163158(look) / len);
        AddTo_163158(self_->Position, self_->Position, left);
        t4Vector targetShift = Scale_163158(left, targetProportion);
        AddTo_163158(self_->CamTargetPos, self_->CamTargetPos, targetShift);
    }
    cLookAtChaseAlgorithm_verify(((char*)self_));
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateDropWithSpeed);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector t4vect_ZAxis_3270 __asm__("t4vect_ZAxis");

// The per-frame camera input record (0x70 bytes) that cLookAtChaseAlgorithm_setupChaseData fills from the target for the chase algorithms.
extern "C" void cLookAtChaseAlgorithm_verify_v(void* self) __asm__("cLookAtChaseAlgorithm_verify");

// PORT: PS2-only VU0 inline asm (vector times scalar).
// PORT: PS2-only VU0 inline asm (dst -= b).
static inline void SubTo_163270(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "=m"(dst)
        : "m"(dst), "m"(b));
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_163270(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}



extern "C" void cLookAtChaseAlgorithm_updateDropWithSpeed_i(float a, float b, float c, float d, float e, cLookAtChaseAlgorithm* self, sCamInfo* src) __asm__("cLookAtChaseAlgorithm_updateDropWithSpeed");
/**
 * Lowers the eye by a smoothed drop that grows with speed, less a fixed rise, plus a crouch or boost term.
 *
 * t ramps from 0 to 1 between speeds 833.3 and 1666.7 cm/s (30 to 60 km/h). The crouch term x = t^2 * d * (Crouch
 * (+0x54) - 0.5), or LastCrouchFactor while airborne (+0x60), is stored in LastCrouchFactor. LastDrop = e * LastDrop
 * + (1 - e) * (a * 1e-4 * s^1.5 - b + max(x, c * sqrt(Boost))), with s = max(speed - 1388.9, 0); Position -= Z *
 * LastDrop, then cLookAtChaseAlgorithm_verify (the eye sanity check). Floats first in the declaration, as in updateCamTargetLate.
 *
 * @param dropPerSpeed  drop added with speed (On Tour dropPerSpeed)
 * @param defaultRise  constant rise (On Tour defaultRise)
 * @param boostDrop  drop per square root of boost (On Tour boostDrop)
 * @param crouchDrop  drop per unit of crouch at speed (On Tour crouchDrop)
 * @param smoothness  smoothing: the weight kept from last frame (On Tour smoothness)
 * @param src  the frame's chase setup (On Tour const cChaseSetup&)
 */
extern "C" void cLookAtChaseAlgorithm_updateDropWithSpeed_i(float dropPerSpeed, float defaultRise, float boostDrop, float crouchDrop, float smoothness, cLookAtChaseAlgorithm* self, sCamInfo* src)
{
    float dist = ClampLo_162B90(src->speed - 1388.888916015625f, 0.0f);
    float speedFrac;
    if (src->speed >= 833.3333740234375f) {
        if (src->speed <= 1666.666748046875f)
            speedFrac = (src->speed - 833.3333740234375f) / 833.3333740234375f;
        else
            speedFrac = 1.0f;
    } else {
        speedFrac = 0.0f;
    }
    float tt = speedFrac * speedFrac;
    float x;
    if (src->f60 != 0) x = self->LastCrouchFactor;
    else x = tt * crouchDrop * (src->f54 - 0.5f);
    self->LastCrouchFactor = x;
    float smoothedDrop = smoothness * self->LastDrop + (1.0f - smoothness) * (dropPerSpeed * (dist * 9.999999747378752e-05f) * Sqrt_163270(dist) - defaultRise + (x >? (boostDrop * Sqrt_163270(src->f4C))));
    self->LastDrop = smoothedDrop;
    SubTo_163270(self->Position, Scale_162B90(t4vect_ZAxis_3270, smoothedDrop));
    cLookAtChaseAlgorithm_verify_v(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateFOVWithSpeed);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_verify(void* self);
/**
 * Widens the field of view with speed and boost.
 *
 * x = 1 + fOVSpeedScalar * 1e-7 * speed^2 + (fOVBoostScalar - 1) * Boost (+0x4C). The smoothing is @p smoothness on
 * the ground; for setup motion state (+0x40) 1 (MS_Air) it is 1, freezing the view, and 0.999 for the other states.
 * LastViewScalar = k * LastViewScalar + (1 - k) * x; View = defaultViewScalar * pi/4 * LastViewScalar. Ends with
 * cLookAtChaseAlgorithm_verify (the eye sanity check).
 *
 * @param obj  the frame's chase setup (On Tour const cChaseSetup& setup)
 * @param fOVSpeedScalar  how much speed widens the view
 * @param defaultViewScalar  the base view as a multiple of pi/4
 * @param fOVBoostScalar  the view multiple at full boost
 * @param smoothness  the smoothing in motion state 0 (MS_Ground)
 */
extern "C" void cLookAtChaseAlgorithm_updateFOVWithSpeed(cLookAtChaseAlgorithm* self, cChaseSetup* obj, float fOVSpeedScalar, float defaultViewScalar, float fOVBoostScalar, float smoothness)
{
    float k = obj->TargetSpeed;
    float x = fOVSpeedScalar * (k * 1.0000000116860974e-07f) * k + 1.0f + (fOVBoostScalar - 1.0f) * obj->Boost;
    int mode = obj->MotionState;
    if (mode == 1)
        smoothness = 1.0f;
    else if (mode != 0)
        smoothness = 0.9990000128746033f;
    self->LastViewScalar = smoothness * self->LastViewScalar + (1.0f - smoothness) * x;
    *(float*)self = defaultViewScalar * 0.7853981852531433f * self->LastViewScalar;
    cLookAtChaseAlgorithm_verify(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateHandplant);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_verify(void* self);


// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_163450(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_163450(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_163450(const t4Vector& v, float s)
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

/**
 * Handles the eye around handplants: holds the look vector while planted, eases back afterwards and adds a smoothed
 * rise.
 *
 * While the setup says handplanting (+0x5C): SecondsSinceLastHandplant = 0 and LastHandplantLook = CamTargetPos -
 * Position. After it, for up to 5 s the counter advances by 1/60, and while it is under @p exitDelay Position blends by a
 * smoothstep from CamTargetPos - LastHandplantLook back to the current eye. LastRiderPos takes the setup's position
 * (+0x10). LastHandplantRise = f * LastHandplantRise + (1 - f) * e while planted and g * LastHandplantRise after;
 * Position.z rises by it, then cLookAtChaseAlgorithm_verify (the eye sanity check).
 *
 * The first three floats are not read: On Tour's tangent-following handplant camera is gone. Callers pass an eighth
 * float (0.455, On Tour zScalar) that is not declared either.
 *
 * @param trig  the frame's chase setup, not a trigger (On Tour const cChaseSetup& setup)
 * @param distanceAlongTangent  not read (On Tour distanceAlongTangent; callers pass 348.6)
 * @param distanceAboveTangent  not read (On Tour distanceAboveTangent; callers pass 66.3)
 * @param speedToTarget  not read (On Tour speedToTarget; callers pass 162.3)
 * @param exitDelay  seconds over which the eye eases back after a handplant (On Tour exitDelay)
 * @param rise  rise while planted (On Tour rise)
 * @param riseEnterSmoothness  smoothing of the rise while planted (On Tour riseEnterSmoothness)
 * @param riseExitSmoothness  per-frame decay of the rise after the handplant (On Tour riseExitSmoothness)
 */
extern "C" void cLookAtChaseAlgorithm_updateHandplant(cLookAtChaseAlgorithm* self, cChaseSetup* trig, float distanceAlongTangent, float distanceAboveTangent, float speedToTarget, float exitDelay, float rise, float riseEnterSmoothness, float riseExitSmoothness)
{
    if (trig->Handplanting)
    {
        t4Vector t = Sub_163450(*(&self->CamTargetPos), *(&self->Position));
        self->SecondsSinceLastHandplant = 0.0f;
        *(&self->LastHandplantLook) = t;
    }
    else if (self->SecondsSinceLastHandplant < 5.0f)
    {
        self->SecondsSinceLastHandplant += 0.01666666753590107f;
        if (self->SecondsSinceLastHandplant < exitDelay)
        {
            float fractionDone = self->SecondsSinceLastHandplant / exitDelay;
            float fractionSquared = fractionDone * fractionDone;
            float smoothFraction = fractionSquared * 3.0f - (fractionSquared + fractionSquared) * fractionDone;
            if (smoothFraction > 1.0f)
                smoothFraction = 1.0f;
            t4Vector t = Add_163450(Scale_163450(Sub_163450(*(&self->CamTargetPos), *(&self->LastHandplantLook)), 1.0f - smoothFraction),
                                        Scale_163450(*(&self->Position), smoothFraction));
            *(&self->Position) = t;
        }
    }
    *(&self->LastRiderPos) = trig->TPos;
    float z;
    if (trig->Handplanting)
        z = riseEnterSmoothness * self->LastHandplantRise + (1.0f - riseEnterSmoothness) * rise;
    else
        z = riseExitSmoothness * self->LastHandplantRise;
    self->LastHandplantRise = z;
    self->Position.z += z;
    cLookAtChaseAlgorithm_verify(((char*)self));
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001635C0);
#ifdef SKIP_ASM
/**
 * Does nothing (nothing in the code or data refers to it).
 */
extern "C" void func_001635C0(void) {

}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001635C8);
#ifdef SKIP_ASM
/**
 * Returns 0.0f; nothing in the code or data refers to it.
 *
 * One of a run of unreferenced stubs (func_001635C0 .. func_001635E0) before the camera's air-spline getters
 * func_001635E8 and func_001635F0.
 *
 * @return 0.0f
 */
extern "C" float func_001635C8(void) {
    return 0.0f;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001635D8);
#ifdef SKIP_ASM
/**
 * Returns 0; nothing in the code or data refers to it.
 *
 * One of the unreferenced stubs func_001635C0 .. func_001635E0.
 *
 * @return 0
 */
extern "C" int func_001635D8(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001635E0);
#ifdef SKIP_ASM
/**
 * Returns 0; nothing in the code or data refers to it.
 *
 * The last of the unreferenced stubs func_001635C0 .. func_001635E0.
 *
 * @return 0
 */
extern "C" int func_001635E0(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001635E8);
#ifdef SKIP_ASM
extern int D_004A4DD8;

/**
 * Returns D_004A4DD8: the AirPositionSpline that cLookAtChaseAlgorithm_updateAirCam evaluated this frame, or 0.
 *
 * @return the spline's address (0 when no air curve is running; D_004A1288 is the matching flag)
 */
extern "C" int func_001635E8(void) {
    return D_004A4DD8;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_001635F0);
#ifdef SKIP_ASM
extern int D_004A4DDC;

/**
 * Returns D_004A4DDC: the AirTargetSpline that cLookAtChaseAlgorithm_updateAirCam evaluated this frame, or 0.
 *
 * @return the spline's address (0 when no air curve is running)
 */
extern "C" int func_001635F0(void) {
    return D_004A4DDC;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateAirCam);
#ifdef SKIP_ASM

struct sVE_1635F8 {
    short delta;
    short index;
    void* fn;
};

extern t4Vector t4vect_ZAxis_v1635F8 __asm__("t4vect_ZAxis");
extern int D_004A1288;

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_1635F8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_1635F8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_1635F8(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_1635F8(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1635F8(const t4Vector& v)
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

// 0 below lo, 1 above hi, linear in between.
static inline float Ramp_1635F8(const float& x, float lo, float hi)
{
    if (lo <= x) {
        if (x <= hi) {
            return (x - lo) / (hi - lo);
        }
        return 1.0f;
    }
    return 0.0f;
}

// Walks x toward limit by one 60 Hz frame.
static inline void Approach_1635F8(float& x, float limit)
{
    if (limit - 0.01666666753590107f <= x) {
        x = limit;
    } else {
        x = (x + 0.01666666753590107f) <? limit;
    }
}

static inline float Lerp_1635F8(float a, float b, float t)
{
    return t * b + (1.0f - t) * a;
}

// PORT: PS2-only VU0 inline asm (in-place v += b).
static inline void AddEq_1635F8(t4Vector& v, const t4Vector& b)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "+m"(v)
        : "m"(b)
        : "memory");
}

static inline int CallI_1635F8(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_1635F8* e = &(*(sVE_1635F8**)o)[slot];
    return ((int (*)(void*))e->fn)(vthis(o, *e));
}

static inline float CallF_1635F8(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_1635F8* e = &(*(sVE_1635F8**)o)[slot];
    return ((float (*)(void*))e->fn)(vthis(o, *e));
}

#define S_1635F8 ((char*)self)
#define I_1635F8 ((char*)info)
/**
 * Adds the jump camera's vertical swoop: builds height curves for the look-at point and the eye at take-off, plays
 * them through the air and fades them out after landing.
 *
 * While airborne (+0x60) outside the half-pipe cam, AirCamSetupCountdown counts down (cut short once the target's air
 * predictor is done, slot 19). At 14 it builds the full-air curves (FullAirTargetSpline, FullAirPositionSpline: 13
 * keys over 4 s) scaled by amplitude * fullScalar * a 0..1 ramp of the target's vertical jump (slot 21, 300..500),
 * and sets ApplyingFullAirFunc. At 0 it builds AirTargetSpline and AirPositionSpline with the same keys stretched to
 * the total air time (slot 12) and scaled by amplitude * VerticalJumpFactor * a smoothstep of the air time over
 * 1.7..2.8 s, and sets ApplyingAirFunc. Running curves are evaluated (cCubicSplineInterpolant_evaluate,
 * cCubicSplineInterpolant::evaluate) into the Last*Delta fields; D_004A1288, D_004A4DD8 and D_004A4DDC publish the
 * active air splines. On landing AirExitSmoothness is set (0.93 toward @p exitSmoothness over 1.2..2.2 s of full-air
 * time, then halfway toward 1 for landing angles 1.05..1.4, slot 23), the curves stop, and from then on the deltas
 * decay by it each frame. The deltas, blended by AirCamTimeInAir / @p blendTime and scaled by eye distance / 300,
 * raise CamTargetPos and Position; after cLookAtChaseAlgorithm_verify (the eye sanity check) the eye is put back at its old distance
 * (at least 50).
 *
 * @param info  the frame's chase setup (On Tour const cChaseSetup& setup)
 * @param fullScalar  scale of the full-air curves relative to @p amplitude
 * @param blendTime  seconds over which the air curves take over from the full-air curves
 * @param amplitude  overall height of the curves
 * @param exitSmoothness  the landing decay factor reached after long airs
 */
// Builds and evaluates the chase camera's jump/landing curves (splines at +0x304..+0x328).
extern "C" void cLookAtChaseAlgorithm_updateAirCam(cLookAtChaseAlgorithm* self, cChaseSetup* info, float fullScalar, float blendTime, float amplitude, float exitSmoothness)
{
    float dist = Length_1635F8(Sub_1635F8(*(&self->CamTargetPos), *(&self->Position)));
    if (info->Airborne != 0 && self->DoingHalfPipeCam == 0) {
        float start = 0.0f;
        if (--self->AirCamSetupCountdown > 0 && CallI_1635F8(S_1635F8, 19)) {
            start = (float)(16 - self->AirCamSetupCountdown) * 0.01666666753590107f;
            self->AirCamSetupCountdown = 0;
        }
        if (self->AirCamSetupCountdown == 14) {
            self->FullAirCamTimeInAir = 0.01666666753590107f;
            float k = Ramp_1635F8(CallF_1635F8(S_1635F8, 21), 300.0f, 500.0f);
            float airAmplitude = amplitude * fullScalar * k;
            self->VerticalJumpFactor = k;
            if (0.0f < airAmplitude) {
                void* sa = S_1635F8 + 0x31C;
                void* sb = S_1635F8 + 0x328;
                cCubicSplineInterpolant_initAsNatural_2(sa, 13);
                cCubicSplineInterpolant_addPoint(sa, 0.0f, 0.0f);
                cCubicSplineInterpolant_addPoint(sa, 0.33000001311302185f, airAmplitude * -121.1346664428711f);
                cCubicSplineInterpolant_addPoint(sa, 0.6600000262260437f, airAmplitude * -217.7401123046875f);
                cCubicSplineInterpolant_addPoint(sa, 0.998885452747345f, airAmplitude * -254.97467041015625f);
                cCubicSplineInterpolant_addPoint(sa, 1.3300000429153442f, airAmplitude * -250.5922088623047f);
                cCubicSplineInterpolant_addPoint(sa, 1.659999966621399f, airAmplitude * -193.38156127929688f);
                cCubicSplineInterpolant_addPoint(sa, 2.0f, airAmplitude * -63.02951431274414f);
                cCubicSplineInterpolant_addPoint(sa, 2.3299999237060547f, airAmplitude * 110.80419158935547f);
                cCubicSplineInterpolant_addPoint(sa, 2.6600000858306885f, airAmplitude * 307.46771240234375f);
                cCubicSplineInterpolant_addPoint(sa, 3.0f, airAmplitude * 496.44952392578125f);
                cCubicSplineInterpolant_addPoint(sa, 3.3299999237060547f, airAmplitude * 689.6500854492188f);
                cCubicSplineInterpolant_addPoint(sa, 3.6600000858306885f, airAmplitude * 820.5333862304688f);
                cCubicSplineInterpolant_addPoint(sa, 4.0f, airAmplitude * 849.3922119140625f);
                cCubicSplineInterpolant_initAsNatural_2(sb, 13);
                cCubicSplineInterpolant_addPoint(sb, 0.0f, 0.0f);
                cCubicSplineInterpolant_addPoint(sb, 0.33000001311302185f, airAmplitude * -22.06365966796875f);
                cCubicSplineInterpolant_addPoint(sb, 0.6600000262260437f, airAmplitude * -34.630374908447266f);
                cCubicSplineInterpolant_addPoint(sb, 1.0f, airAmplitude * 13.823330879211426f);
                cCubicSplineInterpolant_addPoint(sb, 1.3290001153945923f, airAmplitude * 92.12850952148438f);
                cCubicSplineInterpolant_addPoint(sb, 1.659500002861023f, airAmplitude * 213.1912384033203f);
                cCubicSplineInterpolant_addPoint(sb, 2.0000672340393066f, airAmplitude * 383.46197509765625f);
                cCubicSplineInterpolant_addPoint(sb, 2.333711624145508f, airAmplitude * 624.2830200195312f);
                cCubicSplineInterpolant_addPoint(sb, 2.6603856086730957f, airAmplitude * 813.0820922851562f);
                cCubicSplineInterpolant_addPoint(sb, 3.0f, airAmplitude * 977.65869140625f);
                cCubicSplineInterpolant_addPoint(sb, 3.3299999237060547f, airAmplitude * 1070.9466552734375f);
                cCubicSplineInterpolant_addPoint(sb, 3.659675359725952f, airAmplitude * 1104.7698974609375f);
                cCubicSplineInterpolant_addPoint(sb, 4.0f, airAmplitude * 1131.4307861328125f);
                self->AirCamMaxTime = 4.0f;
                self->ApplyingFullAirFunc = 1;
                self->AirSplineScalar = 1.0f;
                self->AirTimeScalar = 1.0f;
                self->HitGroundSinceStartingAirFunc = 0;
            }
        }
        if (self->AirCamSetupCountdown == 0) {
            self->AirCamTimeInAir = start;
            float len = CallF_1635F8(S_1635F8, 12);
            float sx = len * 0.25f;
            float k;
            if (1.7f <= len) {
                if (len <= 2.8f) {
                    float airTimeFrac = (len - 1.7f) / (2.8f - 1.7f);
                    float airTimeFracSq = airTimeFrac * airTimeFrac;
                    k = airTimeFracSq * 3.0f - (airTimeFrac + airTimeFrac) * airTimeFracSq;
                    if (k > 1.0f) {
                        k = 1.0f;
                    }
                } else {
                    k = 1.0f;
                }
            } else {
                k = 0.0f;
            }
            float airAmplitude = k * amplitude * self->VerticalJumpFactor;
            if (0.0f < airAmplitude) {
                void* sc = S_1635F8 + 0x304;
                void* sd = S_1635F8 + 0x310;
                cCubicSplineInterpolant_initAsNatural_2(sc, 13);
                cCubicSplineInterpolant_addPoint(sc, 0.0f, 0.0f);
                cCubicSplineInterpolant_addPoint(sc, sx * 0.33000001311302185f, airAmplitude * -121.1346664428711f);
                cCubicSplineInterpolant_addPoint(sc, sx * 0.6600000262260437f, airAmplitude * -217.7401123046875f);
                cCubicSplineInterpolant_addPoint(sc, sx * 0.998885452747345f, airAmplitude * -254.97467041015625f);
                cCubicSplineInterpolant_addPoint(sc, sx * 1.3300000429153442f, airAmplitude * -250.5922088623047f);
                cCubicSplineInterpolant_addPoint(sc, sx * 1.659999966621399f, airAmplitude * -193.38156127929688f);
                cCubicSplineInterpolant_addPoint(sc, sx * 2.0f, airAmplitude * -63.02951431274414f);
                cCubicSplineInterpolant_addPoint(sc, sx * 2.3299999237060547f, airAmplitude * 110.80419158935547f);
                cCubicSplineInterpolant_addPoint(sc, sx * 2.6600000858306885f, airAmplitude * 307.46771240234375f);
                cCubicSplineInterpolant_addPoint(sc, sx * 3.0f, airAmplitude * 496.44952392578125f);
                cCubicSplineInterpolant_addPoint(sc, sx * 3.3299999237060547f, airAmplitude * 689.6500854492188f);
                cCubicSplineInterpolant_addPoint(sc, sx * 3.6600000858306885f, airAmplitude * 820.5333862304688f);
                cCubicSplineInterpolant_addPoint(sc, sx * 4.0f, airAmplitude * 849.3922119140625f);
                cCubicSplineInterpolant_initAsNatural_2(sd, 13);
                cCubicSplineInterpolant_addPoint(sd, 0.0f, 0.0f);
                cCubicSplineInterpolant_addPoint(sd, sx * 0.33000001311302185f, airAmplitude * -22.06365966796875f);
                cCubicSplineInterpolant_addPoint(sd, sx * 0.6600000262260437f, airAmplitude * -34.630374908447266f);
                cCubicSplineInterpolant_addPoint(sd, sx * 1.0f, airAmplitude * 13.823330879211426f);
                cCubicSplineInterpolant_addPoint(sd, sx * 1.3290001153945923f, airAmplitude * 92.12850952148438f);
                cCubicSplineInterpolant_addPoint(sd, sx * 1.659500002861023f, airAmplitude * 213.1912384033203f);
                cCubicSplineInterpolant_addPoint(sd, sx * 2.0000672340393066f, airAmplitude * 383.46197509765625f);
                cCubicSplineInterpolant_addPoint(sd, sx * 2.333711624145508f, airAmplitude * 624.2830200195312f);
                cCubicSplineInterpolant_addPoint(sd, sx * 2.6603856086730957f, airAmplitude * 813.0820922851562f);
                cCubicSplineInterpolant_addPoint(sd, sx * 3.0f, airAmplitude * 977.65869140625f);
                cCubicSplineInterpolant_addPoint(sd, sx * 3.3299999237060547f, airAmplitude * 1070.9466552734375f);
                cCubicSplineInterpolant_addPoint(sd, sx * 3.659675359725952f, airAmplitude * 1104.7698974609375f);
                cCubicSplineInterpolant_addPoint(sd, sx * 4.0f, airAmplitude * 1131.4307861328125f);
                self->ApplyingAirFunc = 1;
                self->AirSplineScalar = 1.0f;
                self->AirTimeScalar = 1.0f;
                self->AirCamMaxTime = len;
                self->HitGroundSinceStartingAirFunc = 0;
            }
        }
    }
    if (self->ApplyingFullAirFunc != 0) {
        if (4.0f - 0.01666666753590107f <= self->FullAirCamTimeInAir) {
            self->FullAirCamTimeInAir = 4.0f;
        } else {
            self->FullAirCamTimeInAir = (self->FullAirCamTimeInAir + 0.01666666753590107f) <? 4.0f;
        }
        float yb = self->AirSplineScalar * cCubicSplineInterpolant_evaluate(S_1635F8 + 0x328, self->AirTimeScalar * self->FullAirCamTimeInAir);
        float ya = self->AirSplineScalar * cCubicSplineInterpolant_evaluate(S_1635F8 + 0x31C, self->AirTimeScalar * self->FullAirCamTimeInAir);
        self->LastFullAirPositionDelta = yb;
        self->LastFullAirTargetDelta = ya;
    }
    if (self->ApplyingAirFunc != 0) {
        Approach_1635F8(self->AirCamTimeInAir, self->AirCamMaxTime);
        float yd = self->AirSplineScalar * cCubicSplineInterpolant_evaluate(S_1635F8 + 0x310, self->AirTimeScalar * self->AirCamTimeInAir);
        float yc = self->AirSplineScalar * cCubicSplineInterpolant_evaluate(S_1635F8 + 0x304, self->AirTimeScalar * self->AirCamTimeInAir);
        self->LastAirPositionDelta = yd;
        self->LastAirTargetDelta = yc;
        D_004A1288 = 1;
        // PORT: spline pointers kept in int globals.
        D_004A4DD8 = (int)(S_1635F8 + 0x310);
        D_004A4DDC = (int)(S_1635F8 + 0x304);
    } else {
        D_004A1288 = 0;
        D_004A4DD8 = 0;
        D_004A4DDC = 0;
    }
    if (info->Airborne == 0) {
        if (self->HitGroundSinceStartingAirFunc == 0) {
            self->AirExitSmoothness = Lerp_1635F8(0.93f, exitSmoothness, Ramp_1635F8(self->FullAirCamTimeInAir, 1.2f, 2.2f));
            float landAngle = CallF_1635F8(S_1635F8, 23);
            float cur = self->AirExitSmoothness;
            float up = cur + (1.0f - cur) * 0.5f;
            float impactFraction = Ramp_1635F8(landAngle, 1.05f, 1.4f);
            self->AirExitSmoothness = Lerp_1635F8(self->AirExitSmoothness, up, impactFraction);
        }
        self->ApplyingAirFunc = 0;
        self->HitGroundSinceStartingAirFunc = 1;
        self->ApplyingFullAirFunc = 0;
    }
    if (self->HitGroundSinceStartingAirFunc != 0) {
        float smoothness = self->AirExitSmoothness;
        self->LastAirTargetDelta *= smoothness;
        self->LastAirPositionDelta *= smoothness;
        self->LastFullAirTargetDelta *= smoothness;
        self->LastFullAirPositionDelta *= smoothness;
    }
    float w;
    if (self->ApplyingAirFunc != 0) {
        w = (self->AirCamTimeInAir / blendTime) <? 1.0f;
    } else {
        w = 0.0f;
    }
    float scale = Length_1635F8(Sub_1635F8(*(&self->CamTargetPos), *(&self->Position))) *
                  0.0033333334140479565f;
    float ue = Lerp_1635F8(scale * self->LastFullAirTargetDelta, scale * self->LastAirTargetDelta, w);
    float ul = Lerp_1635F8(scale * self->LastFullAirPositionDelta, scale * self->LastAirPositionDelta, w);
    AddEq_1635F8(*(&self->CamTargetPos), Scale_1635F8(t4vect_ZAxis_v1635F8, ue));
    AddEq_1635F8(*(&self->Position), Scale_1635F8(t4vect_ZAxis_v1635F8, ul));
    cLookAtChaseAlgorithm_verify(self);
    t4Vector off = Sub_1635F8(*(&self->CamTargetPos), *(&self->Position));
    ScaleEq_1635F8(off, (dist >? 50.0f) / Length_1635F8(off));
    *(&self->Position) = Sub_1635F8(*(&self->CamTargetPos), off);
}
#undef S_1635F8
#undef I_1635F8
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateViewCam);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector t4vect_ZAxis_41C0 __asm__("t4vect_ZAxis");

extern "C" void* cLookAtChaseAlgorithm_verify(void* self);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_41C0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void AddEq_41C0(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "+m"(dst)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_41C0(const t4Vector& v, float s)
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
static inline void ScaleEq_41C0(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector Cross_41C0(const t4Vector* a, const t4Vector& b)
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
        : "o"(*a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_41C0(const t4Vector& v)
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
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector Add_41C0(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector Normalize_41C0(const t4Vector& v)
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
static inline float Dot_41C0(const t4Vector& a, const t4Vector& b)
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
        : "memory");
    return r;
}

extern "C" void cLookAtChaseAlgorithm_updateViewCam_v(char* self, float a, float b, float c, float ease) __asm__("cLookAtChaseAlgorithm_updateViewCam");

/**
 * Pulls the eye back, up and sideways while the rider is on a flex rail.
 *
 * ViewCamLastFactor eases toward 1 while the target's virtual slot 24 (isRidingFlexRail in On Tour's order) is true
 * and toward 0 otherwise, keeping @p ease of the old value. The offset is the unit eye-minus-target direction times
 * factor * b, plus Z * factor * c, plus its horizontal perpendicular (Z x offset) scaled to factor * a; it is added
 * to Position, then cLookAtChaseAlgorithm_verify (the eye sanity check). Callers pass the chase setup in $a1; it is neither declared
 * nor read.
 *
 * @param side  sideways offset (On Tour side)
 * @param back  backward offset (On Tour back)
 * @param up  upward offset (On Tour up)
 * @param ease  smoothing: the weight kept from last frame (On Tour smoothness)
 */
extern "C" void cLookAtChaseAlgorithm_updateViewCam_v(char* self, float side, float back, float up, float ease)
{
    char* target = *(char**)(self + 0x30);
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)target;
    int onFlexRail = vt[24].fn(vthis(target, vt[24]));
    t4Vector eyeOffset = Sub_41C0(*(t4Vector*)(self + 0x40), *(t4Vector*)(self + 0x20));
    float wantedFactor = 0.0f;
    if (onFlexRail) {
        wantedFactor = 1.0f;
    }
    float factor = ease * *(float*)(self + 0x214) + (1.0f - ease) * wantedFactor;
    *(float*)(self + 0x214) = factor;
    t4Vector offset = Scale_41C0(Normalize_41C0(eyeOffset), factor * back);
    offset.z += factor * up;
    t4Vector cr = Cross_41C0(&t4vect_ZAxis_41C0, offset);
    if (0.0f < Dot_41C0(cr, cr)) {
        ScaleEq_41C0(cr, factor * side / Length_41C0(cr));
    }
    t4Vector sum = Add_41C0(offset, cr);
    AddEq_41C0(*(t4Vector*)(self + 0x40), sum);
    cLookAtChaseAlgorithm_verify(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateSlowSpeedCam);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_1643A8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_1643A8(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_1643A8(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_1643A8(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1643A8(const t4Vector& v)
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

#define S_1643A8 ((char*)self)
#define I_1643A8 ((char*)info)
/**
 * Blends to a held look direction while the rider is nearly stopped.
 *
 * Slow means speed * 0.036 (km/h) under @p slowSpeed while neither airborne (+0x60) nor handplanting (+0x5C).
 * SlowCamState: 3 (off) goes to 0 (blending in) when slow, recording SlowCamEnterLook = CamTargetPos - Position; 0
 * goes to 2 (held) once SlowCamBlend reaches 1; 0 or 2 go to 1 (blending out) when no longer slow; 1 goes back to 0
 * if slow again, or to 3 at blend 0. SlowCamBlend changes by 1 / (60 * blendInTime) or 1 / (60 * blendOutTime) per
 * frame. While not off, with s = smoothstep(SlowCamBlend), the look vector blends toward SlowCamEnterLook (length the
 * mean of the two), CamTargetPos toward the setup's position (+0x10) and Position = CamTargetPos - look. Ends with
 * cLookAtChaseAlgorithm_verify (the eye sanity check).
 *
 * @param info  the frame's chase setup (On Tour const cChaseSetup& setup)
 * @param slowSpeed  the speed threshold in km/h
 * @param blendInTime  seconds to blend in
 * @param blendOutTime  seconds to blend out
 */
extern "C" void cLookAtChaseAlgorithm_updateSlowSpeedCam(cLookAtChaseAlgorithm* self, cChaseSetup* info, float slowSpeed, float blendInTime, float blendOutTime)
{
    int trig = info->TargetSpeed * 0.035999998450279236f < slowSpeed && info->Airborne == 0 &&
               info->Handplanting == 0;
    if (self->SlowCamState == 3 && trig) {
        self->SlowCamState = 0;
        self->SlowCamBlend = 0.0f;
        *(&self->SlowCamEnterLook) = Sub_1643A8(*(&self->CamTargetPos), *(&self->Position));
    } else if (self->SlowCamState == 2 && !trig) {
        self->SlowCamState = 1;
        self->SlowCamBlend = 1.0f;
    } else if (self->SlowCamState == 0 && !trig) {
        self->SlowCamState = 1;
    } else if (self->SlowCamState == 1 && trig) {
        self->SlowCamState = 0;
    }
    if (self->SlowCamState == 0) {
        self->SlowCamBlend = (self->SlowCamBlend + 0.01666666753590107f / blendInTime) <? 1.0f;
        if (self->SlowCamBlend == 1.0f)
            self->SlowCamState = 2;
    } else if (self->SlowCamState == 1) {
        self->SlowCamBlend = (self->SlowCamBlend - 0.01666666753590107f / blendOutTime) >? 0.0f;
        if (self->SlowCamBlend == 0.0f)
            self->SlowCamState = 3;
    }
    if (self->SlowCamState != 3) {
        float fractionIn = self->SlowCamBlend;
        float fractionSquared = fractionIn * fractionIn;
        float smoothFraction = fractionSquared * 3.0f - (fractionSquared + fractionSquared) * fractionIn;
        if (smoothFraction > 1.0f)
            smoothFraction = 1.0f;
        t4Vector look = Sub_1643A8(*(&self->CamTargetPos), *(&self->Position));
        t4Vector newLook = Add_1643A8(Scale_1643A8(look, 1.0f - smoothFraction), Scale_1643A8(*(&self->SlowCamEnterLook), smoothFraction));
        float lookLen = Length_1643A8(look);
        float newLookLen = Length_1643A8(newLook);
        ScaleEq_1643A8(newLook, (lookLen * 0.5f + newLookLen * 0.5f) / newLookLen);
        *(&self->CamTargetPos) =
            Add_1643A8(Scale_1643A8(*(&self->CamTargetPos), 1.0f - smoothFraction), Scale_1643A8(info->TPos, smoothFraction));
        *(&self->Position) = Sub_1643A8(*(&self->CamTargetPos), newLook);
    }
    cLookAtChaseAlgorithm_verify(self);
}
#undef S_1643A8
#undef I_1643A8
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateRailCam);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// Global vectors and a matrix in .bss that the camera code reads (set up elsewhere at startup).
extern t4Vector t4vect_Zero_46A0 __asm__("t4vect_Zero");

extern t4Vector t4vect_ZAxis_46A0 __asm__("t4vect_ZAxis");

extern "C" void cLookAtChaseAlgorithm_updateRailCam_def(char* self, char* src, float a, float b) __asm__("cLookAtChaseAlgorithm_updateRailCam");
extern "C" void* cLookAtChaseAlgorithm_verify(void* self);

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub_1646A0(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector cmAddL_1646A0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector cmScaleL_1646A0(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline t4Vector cmScaleP_1646A0(const t4Vector* v, float s)
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
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v *= s).
static inline void cmScaleEq_1646A0(t4Vector& v, float s)
{
    int t;
    __asm__ __volatile__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (a -= b).
static inline void cmSubEq_1646A0(t4Vector& a, t4Vector b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length, vector passed by value).
static inline float cmLengthV_1646A0(t4Vector v)
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

static inline t4Vector cmLerp_1646A0(const t4Vector& a, const t4Vector& b, float t)
{
    t4Vector r = cmAddL_1646A0(cmScaleL_1646A0(a, t), cmScaleL_1646A0(b, 1.0f - t));
    return r;
}

// PORT: PS2-only VU0 inline asm (dst += b).
static inline void cmAddEq_1646A0(t4Vector& dst, const t4Vector& b)
{
    __asm__(
        ".set noreorder\n"
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set reorder\n"
        : "+m"(dst)
        : "m"(b)
        : "memory");
}

// Sets the velocity and moves the position by it.
static inline void cmStep_1646A0(t4Vector& pos, t4Vector& vel, t4Vector v)
{
    vel = v;
    cmAddEq_1646A0(pos, v);
}


/**
 * Moves the eye in line behind the rider while rail-sliding.
 *
 * When the speed (+0x44) is positive the wanted eye is CamTargetPos minus the velocity (+0x20) scaled to the current
 * eye distance, raised by Z * @p rise; otherwise the current eye. The correction toward it counts only while the setup
 * is rail-sliding (+0x64) and is zero otherwise; LastRailMove = b * LastRailMove + (1 - b) * correction is stored and
 * added to Position, then cLookAtChaseAlgorithm_verify (the eye sanity check).
 *
 * @param src  the frame's chase setup (On Tour const cChaseSetup& setup)
 * @param rise  rise of the eye above the rail line (On Tour rise)
 * @param smoothness  smoothing: the weight kept from last frame (On Tour smoothness)
 */
extern "C" void cLookAtChaseAlgorithm_updateRailCam_def(char* self, char* src, float rise, float smoothness)
{
    t4Vector wantedEye;
    if (0.0f < *(float*)(src + 0x44)) {
        float eyeDist = cmLengthV_1646A0(cmSub_1646A0(*(t4Vector*)(self + 0x20), *(t4Vector*)(self + 0x40)));
        float velocityLen = cmLengthV_1646A0(*(t4Vector*)(src + 0x20));
        float scale = eyeDist / velocityLen;
        t4Vector sv = *(t4Vector*)(src + 0x20);
        cmScaleEq_1646A0(sv, scale);
        cmSubEq_1646A0(sv, cmScaleP_1646A0(&t4vect_ZAxis_46A0, rise));
        wantedEye = cmSub_1646A0(*(t4Vector*)(self + 0x20), sv);
    } else {
        wantedEye = *(t4Vector*)(self + 0x40);
    }
    t4Vector correction = cmSub_1646A0(wantedEye, *(t4Vector*)(self + 0x40));
    const t4Vector* pe = *(int*)(src + 0x64) ? &correction : &t4vect_Zero_46A0;
    t4Vector appliedCorrection = *pe;
    cmStep_1646A0(*(t4Vector*)(self + 0x40), *(t4Vector*)(self + 0xC0), cmLerp_1646A0(*(t4Vector*)(self + 0xC0), appliedCorrection, smoothness));
    cLookAtChaseAlgorithm_verify(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_00164868);
#ifdef SKIP_ASM
/**
 * Returns 0; nothing in the code or data refers to it.
 *
 * An unreferenced stub between cLookAtChaseAlgorithm_updateRailCam and func_00164870.
 *
 * @return 0
 */
extern "C" int func_00164868(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00164870);
#ifdef SKIP_ASM
extern int D_004A4DE0;

/**
 * Returns D_004A4DE0: the HalfPipeAngleSpline that cLookAtChaseAlgorithm_updateHalfPipeCam last evaluated.
 *
 * @return the spline's address (0 until one is evaluated; never cleared afterwards, D_004A128C is the activity flag)
 */
extern "C" int func_00164870(void) {
    return D_004A4DE0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateHalfPipeCam);
#ifdef SKIP_ASM
struct sVec_164878 {
    float x, y, z, w;
    sVec_164878() {}
    sVec_164878(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sVE_164878 {
    short delta;
    short index;
    void* fn;
};

extern sVec_164878 t4vect_ZAxis_v164878 __asm__("t4vect_ZAxis");
extern int D_004A128C;
// PORT: cCubicSplineInterpolant_initAsClamped takes (spline, knot count, x, y); its mangled name says (void*, float, float).
void cCubicSplineInterpolant_initAsClamped_ivff(void* spline, int n, float x, float y) __asm__("cCubicSplineInterpolant_initAsClamped__FPvff");

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec_164878 Sub_164878(const sVec_164878& a, const sVec_164878& b)
{
    sVec_164878 r;
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline sVec_164878 Add_164878(const sVec_164878& a, const sVec_164878& b)
{
    sVec_164878 r;
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

// PORT: PS2-only VU0 inline asm (in-place v += b).
static inline void AddEq_164878(sVec_164878& v, const sVec_164878& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (in-place v -= b).
static inline void SubEq_164878(sVec_164878& v, const sVec_164878& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline sVec_164878 Scale_164878(const sVec_164878& v, float s)
{
    sVec_164878 r;
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_164878(sVec_164878& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place v /= s).
static inline void DivEq_164878(sVec_164878& v, float s)
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

// PORT: PS2-only VU0 inline asm (dot product).
static inline float Dot_164878(const sVec_164878& a, const sVec_164878& b)
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
static inline float Length_164878(const sVec_164878& v)
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
static inline sVec_164878 Cross_164878(const sVec_164878& a, const sVec_164878& b)
{
    sVec_164878 r;
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

// PORT: PS2-only VU0 inline asm (rotate v by quaternion q).
static inline sVec_164878 Rot_164878(const sVec_164878& q, const sVec_164878& v)
{
    sVec_164878 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_164878(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline void Normalize_164878(sVec_164878& v)
{
    DivEq_164878(v, Length_164878(v));
}

// The part of v along n.
static inline sVec_164878 Project_164878(const sVec_164878& v, const sVec_164878& n)
{
    return Scale_164878(n, Dot_164878(v, n) / Dot_164878(n, n));
}

struct sQuat_164878 {
    float x, y, z, w;
    sQuat_164878() {}
    sQuat_164878(float ax, float ay, float az, float aw)
    {
        w = aw;
        x = ax;
        y = ay;
        z = az;
    }
} __attribute__((aligned(16)));

static inline sQuat_164878 Quat_164878(float x, float y, float z, float w)
{
    sQuat_164878 r(x, y, z, w);
    return r;
}

// PORT: PS2-only VU0 inline asm (rotate v by quaternion q).
static inline sVec_164878 RotQ_164878(const sQuat_164878& q, const sVec_164878& v)
{
    sVec_164878 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

// PORT: g++ `<?` / `>?` operators.
static inline float Clamp_164878(float v, float lo, float hi)
{
    if (v >= lo) {
        return v <? hi;
    }
    return lo;
}

// 3t^2 - 2t^3, capped at 1.
static inline float Smooth_164878(float t)
{
    float t2 = t * t;
    float s = t2 * 3.0f - (t2 + t2) * t;
    if (s > 1.0f) {
        s = 1.0f;
    }
    return s;
}

#define MAX_164878(a, b) ((b) <= (a) ? (a) : (b))

static inline int CallI_164878(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_164878* e = &(*(sVE_164878**)o)[slot];
    return ((int (*)(void*))e->fn)(vthis(o, *e));
}

static inline float CallF_164878(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_164878* e = &(*(sVE_164878**)o)[slot];
    return ((float (*)(void*))e->fn)(vthis(o, *e));
}

static inline sVec_164878* CallV_164878(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVE_164878* e = &(*(sVE_164878**)o)[slot];
    return ((sVec_164878* (*)(void*))e->fn)(vthis(o, *e));
}

#define S_164878 ((char*)self)
#define I_164878 ((char*)info)
#define V_164878(off) (*(sVec_164878*)(S_164878 + (off)))
#define F_164878(off) (*(float*)(S_164878 + (off)))
/**
 * Swings the eye around the rider through a half-pipe air (DoingHalfPipeCam) and blends it back afterwards.
 *
 * Set-up, once the target's air predictor is done (slot 19; HalfPipeInitWait counts the wait): HalfPipeInboundLook =
 * CamTargetPos - Position; HalfPipeTimeToLand = max(total air time (slot 12) - wait, 2.3); HalfPipeBlendOutTimeLeft =
 * @p blendOut; HalfPipeExitLook mirrors the inbound look about the half-pipe normal (slot 16) with z negated, bent 20
 * % toward the wall's side vector and kept at the inbound length (pushed 200 sideways and 100 down when it would pass
 * close to the normal); HalfPipeFullOut is its component along the normal; HalfPipeRotator and HalfPipeRotatorAngle
 * are the axis (inbound x exit) and the angle term (pi/2 - func_0031C128 (acosf) of their normalised dot); the 9-key
 * ease curve goes into HalfPipeAngleSpline. Then HalfPipeCamInitialized is set.
 *
 * During the swing the spline value at HalfPipeTimeInAir / HalfPipeTimeToLand rotates the inbound look about the axis
 * (HalfPipeNewLook) and scales FullOut (HalfPipeOut); D_004A128C and D_004A4DE0 publish the spline. CamTargetPos
 * eases toward the setup's position over the first 30 % and Position = CamTargetPos - NewLook + Out. After the swing,
 * while HalfPipeBlendOutTimeLeft lasts, both points blend back toward the chase pose, saving
 * HalfPipeBlendOutLastLook, HalfPipeBlendOutLastTargetDelta and the cross-blend times, which a following half-pipe
 * swing blends out of. Finally the eye is kept at least 60 % of its starting distance from the target, then
 * cLookAtChaseAlgorithm_verify (the eye sanity check).
 *
 * @param info  the frame's chase setup (On Tour const cChaseSetup& setup)
 * @param blendOut  seconds to blend back after the swing (On Tour blendOutTime)
 */
// Air swing for drops: swings the eye around the rider while airborne (spline +0x334), then blends back.
extern "C" void cLookAtChaseAlgorithm_updateHalfPipeCam(cLookAtChaseAlgorithm* self, cChaseSetup* info, float blendOut)
{
    D_004A128C = 0;
    sVec_164878 off0 = Sub_164878(V_164878(0x20), V_164878(0x40));
    sVec_164878 eye0 = V_164878(0x40);
    if (self->DoingHalfPipeCam != 0) {
        if (self->HalfPipeCamInitialized == 0) {
            F_164878(556) += 0.01666666753590107f;
            if (CallI_164878(S_164878, 19)) {
                V_164878(272) = Sub_164878(V_164878(0x20), V_164878(0x40));
                float dist = Length_164878(V_164878(272));
                float* dur = &F_164878(560);
                *dur = MAX_164878(CallF_164878(S_164878, 12) - F_164878(556), 2.299999952316284f);
                F_164878(572) = blendOut;
                F_164878(564) = 0.0f;
                sVec_164878* up = CallV_164878(S_164878, 16);
                sVec_164878 side = Cross_164878(t4vect_ZAxis_v164878, *up);
                Normalize_164878(side);
                if (Dot_164878(side, *(sVec_164878*)&info->TVel) < 0.0f) {
                    ScaleEq_164878(side, -1.0f);
                }
                ScaleEq_164878(side, dist / Length_164878(side));
                V_164878(288) = Sub_164878(V_164878(272), Scale_164878(Project_164878(V_164878(272), *up), 2.0f));
                V_164878(288).z = -V_164878(288).z;
                if (Dot_164878(V_164878(288), V_164878(288)) <= 0.0f) {
                    V_164878(288) = side;
                }
                ScaleEq_164878(V_164878(288), 0.800000011920929f);
                AddEq_164878(V_164878(288), Scale_164878(side, 0.20000000298023224f));
                ScaleEq_164878(V_164878(288), dist / Length_164878(V_164878(288)));
                V_164878(336) = Project_164878(V_164878(288), *up);
                sVec_164878 lat = Sub_164878(V_164878(288), V_164878(336));
                if (Dot_164878(lat, lat) < 40000.0f) {
                    lat.z = 0.0f;
                    float length = Length_164878(lat);
                    if (1.0f < length) {
                        ScaleEq_164878(lat, 200.0f / length);
                        AddEq_164878(V_164878(288), lat);
                        SubEq_164878(V_164878(288), Scale_164878(t4vect_ZAxis_v164878, 100.0f));
                    }
                }
                V_164878(304) = Cross_164878(V_164878(272), V_164878(288));
                Normalize_164878(V_164878(304));
                float ang;
                float len = Dot_164878(V_164878(272), V_164878(272)) * Dot_164878(V_164878(288), V_164878(288));
                len = Sqrt_164878(len);
                if (len < 0.0010000000474974513f) {
                    ang = 0.0f;
                } else {
                    float cosAngle = Dot_164878(V_164878(272), V_164878(288)) / len;
                    if (-1.0f <= cosAngle) {
                        cosAngle = cosAngle <? 1.0f;
                    } else {
                        cosAngle = -1.0f;
                    }
                    ang = 1.5707963705062866f - func_0031C128(cosAngle);
                }
                F_164878(568) = ang;
                void* spl = S_164878 + 0x334;
                cCubicSplineInterpolant_initAsClamped_ivff(spl, 9, 0.0f, 0.0f);
                cCubicSplineInterpolant_addPoint(spl, 0.0f, 0.0f);
                cCubicSplineInterpolant_addPoint(spl, 0.125f, 0.0639725923538208f);
                cCubicSplineInterpolant_addPoint(spl, 0.25f, 0.30785250663757324f);
                cCubicSplineInterpolant_addPoint(spl, 0.375f, 0.5528385043144226f);
                cCubicSplineInterpolant_addPoint(spl, 0.5f, 0.6577425003051758f);
                cCubicSplineInterpolant_addPoint(spl, 0.625f, 0.728873610496521f);
                cCubicSplineInterpolant_addPoint(spl, 0.75f, 0.827086865901947f);
                cCubicSplineInterpolant_addPoint(spl, 0.875f, 0.9414384365081787f);
                cCubicSplineInterpolant_addPoint(spl, 1.0f, 1.0f);
                self->HalfPipeCamInitialized = 1;
            }
        }
        if (self->HalfPipeCamInitialized != 0) {
            F_164878(564) += 0.01666666753590107f;
            if (F_164878(564) < F_164878(560)) {
                float timeFraction = F_164878(564) / F_164878(560);
                D_004A128C = 1;
                // PORT: spline pointer kept in an int global.
                D_004A4DE0 = (int)(S_164878 + 0x334);
                float fraction = cCubicSplineInterpolant_evaluate(S_164878 + 0x334, timeFraction);
                float sn, cs;
                fSinCos(&sn, &cs, fraction * F_164878(568) * 0.5f);
                sQuat_164878 rotation = Quat_164878(sn * V_164878(304).x, sn * V_164878(304).y, sn * V_164878(304).z, cs);
                V_164878(320) = RotQ_164878(rotation, V_164878(272));
                V_164878(352) = Scale_164878(V_164878(336), fraction);
            }
            float k = (F_164878(564) / (F_164878(560) * 0.30000001192092896f)) <? 1.0f >? 0.0f;
            k = Smooth_164878(k);
            V_164878(0x20) = Add_164878(Scale_164878(V_164878(0x20), 1.0f - k), Scale_164878(*(sVec_164878*)&info->TPos, k));
            V_164878(0x40) = Add_164878(Sub_164878(V_164878(0x20), V_164878(320)), V_164878(352));
        }
    } else if (0.0f < F_164878(572)) {
        float bt = (F_164878(572) - 0.01666666753590107f) >? 0.0f;
        float fraction = bt / blendOut;
        F_164878(572) = bt;
        fraction = Smooth_164878(fraction);
        sVec_164878 eye = V_164878(0x20);
        V_164878(0x20) = Add_164878(Scale_164878(V_164878(0x20), 1.0f - fraction), Scale_164878(*(sVec_164878*)&info->TPos, fraction));
        V_164878(0x40) = Add_164878(Scale_164878(V_164878(0x40), 1.0f - fraction),
                                    Scale_164878(Add_164878(Sub_164878(*(sVec_164878*)&info->TPos, V_164878(320)), V_164878(352)), fraction));
        V_164878(368) = Sub_164878(V_164878(0x20), V_164878(0x40));
        V_164878(0x180) = Sub_164878(V_164878(0x20), eye);
        F_164878(580) = F_164878(576) = F_164878(572);
    }
    if (self->DoingHalfPipeCam != 0 && 0.0f < F_164878(576) && 0.0f < F_164878(580)) {
        float bt = (F_164878(576) - 0.01666666753590107f) >? 0.0f;
        float fraction = bt / F_164878(580);
        F_164878(576) = bt;
        fraction = Smooth_164878(fraction);
        sVec_164878 look = Sub_164878(V_164878(0x20), V_164878(0x40));
        sVec_164878 dd = Add_164878(Scale_164878(V_164878(368), fraction), Scale_164878(look, 1.0f - fraction));
        AddEq_164878(V_164878(0x20), Scale_164878(V_164878(0x180), fraction));
        V_164878(0x40) = Sub_164878(V_164878(0x20), dd);
    }
    sVec_164878 cur = Sub_164878(V_164878(0x20), V_164878(0x40));
    float lmin = Length_164878(off0) * 0.6000000238418579f;
    float length = Length_164878(cur);
    if (length < lmin) {
        if (1.0f < length) {
            ScaleEq_164878(cur, lmin / length);
            V_164878(0x40) = Sub_164878(V_164878(0x20), cur);
        } else {
            V_164878(0x40) = eye0;
        }
    }
    cLookAtChaseAlgorithm_verify(self);
}
#undef S_164878
#undef I_164878
#undef V_164878
#undef F_164878
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateRotation);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_verify(void* self);
extern "C" void fSinCos(float* sout, float* cout, float x);

struct sVec4_165540
{
    float x, y, z, w;
    sVec4_165540() {}
    sVec4_165540(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

extern sVec4_165540 t4vect_XAxis_v165540 __asm__("t4vect_XAxis");
extern sVec4_165540 t4vect_ZAxis_v165540 __asm__("t4vect_ZAxis");

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4_165540 Sub_165540(const sVec4_165540& a, const sVec4_165540& b)
{
    sVec4_165540 r;
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
static inline float Length_165540(const sVec4_165540& v)
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_165540(sVec4_165540& v, float s)
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

// PORT: PS2-only VU0 inline asm (rotate v by quaternion q).
static inline sVec4_165540 Rot_165540(const sVec4_165540& q, const sVec4_165540& v)
{
    sVec4_165540 r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vsub.w    $vf8, $vf8, $vf8\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vopmula.xyz ACC, $vf4, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf4\n"
        "vmulaw.xyz ACC, $vf5, $vf0w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf6, $vf4w\n"
        "vmaddaw.xyz ACC, $vf7, $vf0w\n"
        "vmaddw.xyz $vf8, $vf7, $vf0w\n"
        "sqc2      $vf8, %0\n"
        : "=m"(r)
        : "m"(q), "m"(v)
        : "memory");
    return r;
}

static inline sVec4_165540 AxisAngle_165540(const sVec4_165540& axis, float ang)
{
    float s, c;
    fSinCos(&s, &c, ang * 0.5f);
    sVec4_165540 r(s * axis.x, s * axis.y, s * axis.z, c);
    return r;
}

/**
 * Turns the eye around the look-at point to a fixed world heading, keeping its horizontal distance and its height.
 *
 * With off = CamTargetPos - Position, if its horizontal length is positive the X axis scaled to that length, with
 * off's z, is rotated by @p ang about Z (axis-angle quaternion) and Position = CamTargetPos - that. Ends with
 * cLookAtChaseAlgorithm_verify (the eye sanity check).
 *
 * @param ang  the heading in radians (cDirectionChaseAlgorithm passes its Angle; On Tour angle)
 */
extern "C" void cLookAtChaseAlgorithm_updateRotation(cLookAtChaseAlgorithm* self, float ang)
{
    sVec4_165540 off = Sub_165540(*(sVec4_165540*)&self->CamTargetPos, *(sVec4_165540*)&self->Position);
    sVec4_165540 dir = t4vect_XAxis_v165540;
    float z = off.z;
    off.z = 0.0f;
    float len = Length_165540(off);
    if (off.z < len)
    {
        ScaleEq_165540(dir, len);
        dir.z = z;
        sVec4_165540 rotator = AxisAngle_165540(t4vect_ZAxis_v165540, ang);
        sVec4_165540 rd = Rot_165540(rotator, dir);
        sVec4_165540 newPos = Sub_165540(*(sVec4_165540*)&self->CamTargetPos, rd);
        *(sVec4_165540*)&self->Position = newPos;
    }
    cLookAtChaseAlgorithm_verify(((char*)self));
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_setupCameraShake);
#ifdef SKIP_ASM
// Uniform float in [0, 1) built from the random mantissa bits.
static inline float randf_1656B0()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

static inline float RandRange_1656B0(float lo, float hi)
{
    return lo + (hi - lo) * randf_1656B0();
}

/**
 * Starts a camera shake: seeds each axis's first period and target offset for the primary and secondary shakes.
 *
 * Primary: periods base + spread * random, targets random within +-spatialVariance (z within +-1.6 times that); last
 * targets and counters cleared. Secondary: the same at half the period, spread and amplitude. Also clears
 * ShakingLastFrame and ShakeBlendOutTime.
 *
 * @param base  the minimum seconds between targets (On Tour minTimeStep)
 * @param spread  the random extra seconds (On Tour timeVariance)
 * @param spatialVariance  the largest x/y offset (On Tour spatialVariance)
 */
extern "C" void cLookAtChaseAlgorithm_setupCameraShake(cLookAtChaseAlgorithm* self, float base, float spread, float spatialVariance)
{
    self->ShakeNextXTime = base + spread * randf_1656B0();
    self->ShakeNextYTime = base + spread * randf_1656B0();
    self->ShakeNextZTime = base + spread * randf_1656B0();
    float rz = spatialVariance * 1.600000023841858f;
    self->ShakeXTarget = RandRange_1656B0(-spatialVariance, spatialVariance);
    self->ShakeYTarget = RandRange_1656B0(-spatialVariance, spatialVariance);
    self->ShakeZTarget = RandRange_1656B0(-rz, rz);
    self->ShakeLastXTarget = 0.0f;
    self->ShakeLastYTarget = 0.0f;
    self->ShakeLastZTarget = 0.0f;
    self->ShakeXCounter = 0.0f;
    self->ShakeYCounter = 0.0f;
    self->ShakeZCounter = 0.0f;
    base *= 0.5f;
    spread *= 0.5f;
    spatialVariance *= 0.5f;
    rz *= 0.5f;
    self->SecondaryShakeNextXTime = base + spread * randf_1656B0();
    self->SecondaryShakeNextYTime = base + spread * randf_1656B0();
    self->SecondaryShakeNextZTime = base + spread * randf_1656B0();
    self->SecondaryShakeXTarget = RandRange_1656B0(-spatialVariance, spatialVariance);
    self->SecondaryShakeYTarget = RandRange_1656B0(-spatialVariance, spatialVariance);
    self->SecondaryShakeZTarget = RandRange_1656B0(-rz, rz);
    self->SecondaryShakeLastXTarget = 0.0f;
    self->SecondaryShakeLastYTarget = 0.0f;
    self->SecondaryShakeLastZTarget = 0.0f;
    self->SecondaryShakeXCounter = 0.0f;
    self->SecondaryShakeYCounter = 0.0f;
    self->SecondaryShakeZCounter = 0.0f;
    self->ShakingLastFrame = 0;
    self->ShakeBlendOutTime = 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateCameraShake);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// 16-byte vector with inline constructors (a non-POD sibling of t4Vector, as in ai/ai); stores x, y, z, w.
struct sVec4 : t4Vector
{
    sVec4() {}
    sVec4(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
    sVec4& operator+=(const sVec4& b);
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector add, in place).
inline sVec4& sVec4::operator+=(const sVec4& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(*this)
        : "m"(*this), "m"(b)
        : "memory");
    return *this;
}

int BXrand();

// Uniform float in [0, 1) built from the random mantissa bits.
static inline float randf_165938()
{
    union {
        int i;
        float f;
    } u;
    u.i = (BXrand() & 0x7FFFFF) | 0x3F800000;
    return u.f - 1.0f;
}

static inline float RandRange_165938(float lo, float hi)
{
    return lo + (hi - lo) * randf_165938();
}

// Smooth (3t^2 - 2t^3) ramp: 0 below lo, 1 above hi, in between eased on t = x / hi
// (the callers always start the ramp at lo = 0).
static inline float SmoothStep_165938(float lo, float hi, float x)
{
    if (x >= lo) {
        if (x <= hi) {
            float t = x / hi;
            float sq = t * t;
            float e = 3.0f * sq - 2.0f * t * sq;
            if (e > 1.0f) {
                e = 1.0f;
            }
            return e;
        }
        return 1.0f;
    }
    return 0.0f;
}

static inline float Lerp_165938(const float& a, const float& b, float k)
{
    return k * b + (1.0f - k) * a;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sVec4 Sub_165938(const sVec4& a, const sVec4& b)
{
    sVec4 r;
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
static inline float Length_165938(const sVec4& v)
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
static inline sVec4 Scale_165938(const sVec4& v, float s)
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
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

/**
 * Shakes the look-at point while @p flag is set and for a 2 s fade after it clears.
 *
 * Each active frame the three primary counters advance by 1/60; an axis whose counter passes its period picks a new
 * period and target, keeping the old target as the last one. The secondary set is re-rolled the same way at half
 * scale, but its counters are never advanced here, so after setupCameraShake it never re-rolls and adds nothing (its
 * last targets stay 0). Each axis eases from its last to its new target by a smoothstep of counter / period. The sum
 * is added to CamTargetPos scaled by the fade (ShakeBlendOutTime / 2, else 1) times 15, or a tenth of the eye
 * distance when @p sel is set and the eye is within 150.
 *
 * @param flag  nonzero to shake this frame (On Tour doShake)
 * @param sel  nonzero to scale the shake with a close eye's distance (On Tour dependsOnDistance)
 * @param base  the minimum seconds between targets (On Tour minTimeStep)
 * @param spread  the random extra seconds (On Tour timeVariance)
 * @param spatialVariance  the largest x/y offset (On Tour spatialVariance)
 */
// Camera shake update: re-rolls each axis's target when its timer runs out, eases between the
// previous and new targets and adds the result (scaled by the fade-out and distance) to the position.
extern "C" void cLookAtChaseAlgorithm_updateCameraShake(cLookAtChaseAlgorithm* self, int flag, int sel, float base, float spread, float spatialVariance)
{
    if (flag) {
        self->ShakeBlendOutTime = 0.0f;
    } else if (self->ShakingLastFrame) {
        self->ShakeBlendOutTime = 2.0f;
    }
    self->ShakingLastFrame = flag;
    if (self->ShakeBlendOutTime > 0.0f) {
        // PORT: g++ `>?` (max) operator.
        self->ShakeBlendOutTime = (self->ShakeBlendOutTime - 0.01666666753590107f) >? 0.0f;
    }
    float scale;
    if (self->ShakeBlendOutTime > 0.0f) {
        scale = self->ShakeBlendOutTime * 0.5f;
    } else {
        scale = 1.0f;
    }
    if (!flag && !(self->ShakeBlendOutTime > 0.0f)) {
        return;
    }
    self->ShakeXCounter += 0.01666666753590107f;
    self->ShakeYCounter += 0.01666666753590107f;
    self->ShakeZCounter += 0.01666666753590107f;
    if (self->ShakeXCounter > self->ShakeNextXTime) {
        self->ShakeNextXTime = base + spread * randf_165938();
        self->ShakeLastXTarget = self->ShakeXTarget;
        self->ShakeXTarget = RandRange_165938(-spatialVariance, spatialVariance);
        self->ShakeXCounter -= self->ShakeNextXTime;
    }
    if (self->ShakeYCounter > self->ShakeNextYTime) {
        self->ShakeNextYTime = base + spread * randf_165938();
        self->ShakeLastYTarget = self->ShakeYTarget;
        self->ShakeYTarget = RandRange_165938(-spatialVariance, spatialVariance);
        self->ShakeYCounter -= self->ShakeNextYTime;
    }
    float rz = spatialVariance * 1.600000023841858f;
    if (self->ShakeZCounter > self->ShakeNextZTime) {
        self->ShakeNextZTime = base + spread * randf_165938();
        self->ShakeLastZTarget = self->ShakeZTarget;
        self->ShakeZTarget = RandRange_165938(-rz, rz);
        self->ShakeZCounter -= self->ShakeNextZTime;
    }
    rz *= 0.5f;
    float base2 = base * 0.5f;
    float spread2 = spread * 0.5f;
    float secondaryVariance = spatialVariance * 0.5f;
    if (self->SecondaryShakeXCounter > self->SecondaryShakeNextXTime) {
        self->SecondaryShakeNextXTime = base2 + spread2 * randf_165938();
        self->SecondaryShakeLastXTarget = self->SecondaryShakeXTarget;
        self->SecondaryShakeXTarget = RandRange_165938(-secondaryVariance, secondaryVariance);
        self->SecondaryShakeXCounter -= self->SecondaryShakeNextXTime;
    }
    if (self->SecondaryShakeYCounter > self->SecondaryShakeNextYTime) {
        self->SecondaryShakeNextYTime = base2 + spread2 * randf_165938();
        self->SecondaryShakeLastYTarget = self->SecondaryShakeYTarget;
        self->SecondaryShakeYTarget = RandRange_165938(-secondaryVariance, secondaryVariance);
        self->SecondaryShakeYCounter -= self->SecondaryShakeNextYTime;
    }
    if (self->SecondaryShakeZCounter > self->SecondaryShakeNextZTime) {
        self->SecondaryShakeNextZTime = base2 + spread2 * randf_165938();
        self->SecondaryShakeLastZTarget = self->SecondaryShakeZTarget;
        self->SecondaryShakeZTarget = RandRange_165938(-rz, rz);
        self->SecondaryShakeZCounter -= self->SecondaryShakeNextZTime;
    }
    float xFraction = SmoothStep_165938(0.0f, self->ShakeNextXTime, self->ShakeXCounter);
    float yFraction = SmoothStep_165938(0.0f, self->ShakeNextYTime, self->ShakeYCounter);
    float zFraction = SmoothStep_165938(0.0f, self->ShakeNextZTime, self->ShakeZCounter);
    float xSecondaryFraction = SmoothStep_165938(0.0f, self->SecondaryShakeNextXTime, self->SecondaryShakeXCounter);
    float ySecondaryFraction = SmoothStep_165938(0.0f, self->SecondaryShakeNextYTime, self->SecondaryShakeYCounter);
    float zSecondaryFraction = SmoothStep_165938(0.0f, self->SecondaryShakeNextZTime, self->SecondaryShakeZCounter);
    float x = Lerp_165938(self->ShakeLastXTarget, self->ShakeXTarget, xFraction);
    float y = Lerp_165938(self->ShakeLastYTarget, self->ShakeYTarget, yFraction);
    float z = Lerp_165938(self->ShakeLastZTarget, self->ShakeZTarget, zFraction);
    x += Lerp_165938(self->SecondaryShakeLastXTarget, self->SecondaryShakeXTarget, xSecondaryFraction);
    y += Lerp_165938(self->SecondaryShakeLastYTarget, self->SecondaryShakeYTarget, ySecondaryFraction);
    z += Lerp_165938(self->SecondaryShakeLastZTarget, self->SecondaryShakeZTarget, zSecondaryFraction);
    float amp;
    if (sel && Length_165938(Sub_165938(*(sVec4*)&self->CamTargetPos, *(sVec4*)&self->Position)) <= 150.0f) {
        amp = Length_165938(Sub_165938(*(sVec4*)&self->CamTargetPos, *(sVec4*)&self->Position)) * 0.1f;
    } else {
        amp = 15.0f;
    }
    *(sVec4*)&self->CamTargetPos += Scale_165938(sVec4(x, y, z, 0.0f), scale * amp);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_doRaceCamFinalPhase);
#ifdef SKIP_ASM

struct sCamVE166228 {
    short delta;
    short index;
    void (*fn)(void*, cRaceCamFinalPhaseData*);
};

extern "C" void cLookAtChaseAlgorithm_calcPitchHeading(void* self, int a1);
extern "C" void cLookAtChaseAlgorithm_updatePitchBlending(cLookAtChaseAlgorithm* self, float x, float y, float z, float w);
void* cLookAtChaseAlgorithm_updateDebugCam(void* self);
extern "C" void cLookAtChaseAlgorithm_calcPose(void* self);

/**
 * Finishes a race camera frame: pitch, optional pitch blending and the final pose.
 *
 * Gets the final-phase data from the algorithm (virtual slot 7, getRaceCamFinalPhaseData), recomputes pitch and
 * heading (cLookAtChaseAlgorithm_calcPitchHeading, calcPitchHeading(false)), runs updatePitchBlending with the data's four values when its
 * DoPitchBlending flag is set, then updateDebugCam and calcPose (cLookAtChaseAlgorithm_calcPose). On Tour's takes a bool
 * ignoreCollisions; this one takes none.
 */
extern "C" void cLookAtChaseAlgorithm_doRaceCamFinalPhase(cLookAtChaseAlgorithm* self)
{
    cRaceCamFinalPhaseData finalPhaseData;
    sCamVE166228* vt = (sCamVE166228*)self->mVtbl;
    vt[7].fn(vthis(self, vt[7]), &finalPhaseData);
    cLookAtChaseAlgorithm_calcPitchHeading(self, 0);
    if (finalPhaseData.DoPitchBlending != 0)
        cLookAtChaseAlgorithm_updatePitchBlending(self, finalPhaseData.PitchBlendingMaxAngleUp, finalPhaseData.PitchBlendingMaxAngleDown, finalPhaseData.PitchBlendingSmoothnessUp, finalPhaseData.PitchBlendingSmoothnessDown);
    cLookAtChaseAlgorithm_updateDebugCam(self);
    cLookAtChaseAlgorithm_calcPose(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updatePitchBlending);
#ifdef SKIP_ASM
extern "C" float func_0031C128(float x);


struct sVEv_1662A0 {
    short delta;
    short index;
    t4Vector (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_1662A0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_1662A0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_1662A0(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_1662A0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_1662A0(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Clamp_1662A0(float v, float lo, float hi)
{
    if (v >= lo)
        return v <? hi;
    return lo;
}

static inline float Angle_1662A0(const t4Vector& a, const t4Vector& b)
{
    float l = Sqrt_1662A0(Dot_1662A0(a, a) * Dot_1662A0(b, b));
    if (l < 0.0010000000474974513f)
        return 0.0f;
    return 1.5707963705062866f - func_0031C128(Clamp_1662A0(Dot_1662A0(a, b) / l, -1.0f, 1.0f));
}

static inline t4Vector Get_1662A0(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEv_1662A0* e = &(*(sVEv_1662A0**)o)[slot];
    return e->fn(vthis(o, *e));
}

#define S_1662A0 ((char*)self)
/**
 * Smooths pitch changes and caps how far the look line may move in a frame, then re-aims the look-at point.
 *
 * When Pitch has risen above LastPitch it uses smoothing @p w and limit @p y, else @p z and @p x. Pitch = k *
 * LastPitch + (1 - k) * Pitch; if the angle between the eye's look (CamTargetPos - Position) and the new
 * pitch/heading look vector (cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading) exceeds the limit (degrees), k is divided by angle / limit and the blend
 * redone from the original pitch. LastPitch takes the result. CamTargetPos becomes the rider position (target slot 1)
 * projected onto the new look line through Position, then cLookAtChaseAlgorithm_verify (the eye sanity check).
 *
 * @param x  the angle limit in degrees while pitch falls (On Tour maxAngleUp)
 * @param y  the angle limit while pitch rises (On Tour maxAngleDown)
 * @param z  the smoothing while pitch falls (On Tour smoothnessUp)
 * @param w  the smoothing while pitch rises (On Tour smoothnessDown)
 */
extern "C" void cLookAtChaseAlgorithm_updatePitchBlending(cLookAtChaseAlgorithm* self, float x, float y, float z, float w)
{
    float k, ang;
    int up = self->LastPitch < self->Pitch;
    if (up) {
        k = w;
        ang = y;
    } else {
        k = z;
        ang = x;
    }
    float old = self->Pitch;
    self->Pitch = k * self->LastPitch + (1.0f - k) * old;
    t4Vector toTarget = Sub_1662A0(*(&self->CamTargetPos), *(&self->Position));
    t4Vector look;
    cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading(self, &look);
    float bearing = Angle_1662A0(toTarget, look);
    float lim = ang * 0.01745329424738884f;
    float historyFactor;
    if (lim < bearing)
        historyFactor = k / (bearing / lim);
    else
        historyFactor = k;
    self->LastPitch = self->Pitch = historyFactor * self->LastPitch + (1.0f - historyFactor) * old;
    t4Vector newLook;
    cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading(self, &newLook);
    t4Vector off = Sub_1662A0(Get_1662A0(S_1662A0, 1), *(&self->Position));
    float alongLook = Dot_1662A0(off, newLook) / Dot_1662A0(newLook, newLook);
    t4Vector sc = Scale_1662A0(newLook, alongLook);
    *(&self->CamTargetPos) = Add_1662A0(*(&self->Position), sc);
    cLookAtChaseAlgorithm_verify(self);
}
#undef S_1662A0
#endif

extern "C" void* cLookAtChaseAlgorithm_verify(void* self);

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_updateDebugCam__FPv);
#ifdef SKIP_ASM
/**
 * On Tour's updateDebugCam, reduced to the eye sanity check: calls cLookAtChaseAlgorithm_verify and returns its result.
 *
 * @return cLookAtChaseAlgorithm_verify's result (the function is really void; declared void*)
 */
void* cLookAtChaseAlgorithm_updateDebugCam(void* self)
{
    return cLookAtChaseAlgorithm_verify(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_setupInitialCamDir);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector t4vect_ZAxis_6550 __asm__("t4vect_ZAxis");


// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector Scale_166550(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector Sub_166550(const t4Vector& a, const t4Vector& b)
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
static inline float Length_166550(const t4Vector& v)
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
static inline void DivEq_166550(t4Vector& v, float s)
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

static inline t4Vector Get_166550(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)o)[slot];
    return vEntry->fn(vthis(o, *vEntry));
}

// PORT: PS2-only VU0 inline asm (vector subtract into destination).
static inline void SubTo_166550(t4Vector& r, const t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}

/**
 * Seeds LastCamDir for a snap: the target's front (slot 4) times @p backDist, minus Z times @p height, normalised.
 *
 * @param self_  the chase algorithm (this)
 * @param backDist  the horizontal part of the starting look direction
 * @param height  the downward part of the starting look direction
 */
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(cLookAtChaseAlgorithm* self_, float backDist, float height)
{
    t4Vector& camDir = self_->LastCamDir;
    camDir = Scale_166550(Get_166550(((char*)self_), 4), backDist);
    SubTo_166550(camDir, camDir, Scale_166550(t4vect_ZAxis_6550, height));
    DivEq_166550(camDir, Length_166550(camDir));
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_calcPitchHeading);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector t4vect_XAxis;

extern t4Vector t4vect_YAxis;

extern "C" float func_0031C228(float x);
extern "C" float func_0031C128(float x);


// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_166640(const t4Vector& a, const t4Vector& b)
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
static inline float Length_166640(const t4Vector& v)
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
static inline float Dot_166640(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component dot product, second vector through an address register).
static inline float DotP_166640(const t4Vector& a, const t4Vector& b)
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
        : "m"(a), "o"(b));
    return r;
}

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_166640(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Angle_166640(const t4Vector& a, const t4Vector& b)
{
    float l = Sqrt_166640(Dot_166640(a, a) * Dot_166640(b, b));
    if (l < 0.0010000000474974513f)
        return 0.0f;
    float c = Dot_166640(a, b) / l;
    c = (c >= -1.0f) ? (c <? 1.0f) : -1.0f;
    return 1.5707963705062866f - func_0031C128(c);
}

extern "C" void cLookAtChaseAlgorithm_calcPitchHeading_def(char* self) __asm__("cLookAtChaseAlgorithm_calcPitchHeading");
/**
 * Computes Heading and Pitch for looking from the eye (Position) at CamTargetPos.
 *
 * Pitch = -func_0031C228(dz / horizontal distance) (an arctangent). Heading is the angle between the horizontal look
 * direction and the x axis (an arccosine through func_0031C128), or 2 pi minus it when the direction's y is not
 * positive, and 0 when the horizontal distance is 0. Callers pass a second argument (1 or 0) that is not read.
 */
extern "C" void cLookAtChaseAlgorithm_calcPitchHeading_def(char* self)
{
    const t4Vector& off0 = Sub_166640(*(t4Vector*)(self + 0x20), *(t4Vector*)(self + 0x40));
    t4Vector off = off0;
    float* headingPtr = (float*)(self + 0x50);
    float z = off.z;
    off.z = 0.0f;
    float len = Length_166640(off);
    *(float*)(self + 0x54) = -func_0031C228(z / len);
    if (0.0f < len) {
        float heading;
        if (0.0f < DotP_166640(off, t4vect_YAxis)) {
            heading = Angle_166640(off, t4vect_XAxis);
        } else {
            heading = 6.2831854820251465f - Angle_166640(off, t4vect_XAxis);
        }
        *headingPtr = heading;
    } else {
        *headingPtr = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading);
#ifdef SKIP_ASM



extern t4Matrix t4mat_Identity_1668B8 __asm__("t4mat_Identity");
extern t4Vector t4vect_YAxis_1668B8 __asm__("t4vect_YAxis");
extern t4Vector t4vect_ZAxis_v1668B8 __asm__("t4vect_ZAxis");
extern t4Vector t4vect_Origin_v1668B8 __asm__("t4vect_Origin");
extern "C" void fSinCos(float* sout, float* cout, float x);

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_1668B8(t4Matrix* dst, const t4Matrix* src)
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
static inline void vu0MulMtx_1668B8(t4Matrix* dst, const t4Matrix* a, const t4Matrix* b)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in place, m = m * b).
static inline void vu0MulEqMtx_1668B8(t4Matrix* m, const t4Matrix* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
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
        : "r"(m), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_1668B8(t4Matrix* m, const t4Vector& v)
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

static inline void SinCos_1668B8(float* s, float* c, float angle)
{
    fSinCos(s, c, angle);
}

static inline void Rotate_1668B8(t4Matrix* m, const t4Vector& axis, float angle)
{
    t4Matrix tm;
    vu0CopyMtx_1668B8(&tm, m);
    t4Vector pos = *(t4Vector*)((float (*)[4])&tm.row[0])[3];
    *(t4Vector*)((float (*)[4])&tm.row[0])[3] = t4vect_Origin_v1668B8;
    float s;
    float c;
    SinCos_1668B8(&s, &c, angle);
    float one = 1.0f;
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    ((float (*)[4])&m->row[0])[0][0] = tx * x + c;
    ((float (*)[4])&m->row[0])[1][0] = tx * y + sz;
    ((float (*)[4])&m->row[0])[2][0] = tx * z - sy;
    ((float (*)[4])&m->row[0])[0][1] = ty * x - sz;
    ((float (*)[4])&m->row[0])[1][1] = ty * y + c;
    ((float (*)[4])&m->row[0])[2][1] = ty * z + sx;
    ((float (*)[4])&m->row[0])[0][2] = tz * x + sy;
    ((float (*)[4])&m->row[0])[1][2] = tz * y - sx;
    ((float (*)[4])&m->row[0])[2][2] = tz * z + c;
    ((float (*)[4])&m->row[0])[3][0] = 0.0f;
    ((float (*)[4])&m->row[0])[3][1] = 0.0f;
    ((float (*)[4])&m->row[0])[3][2] = 0.0f;
    ((float (*)[4])&m->row[0])[3][3] = one;
    ((float (*)[4])&m->row[0])[0][3] = 0.0f;
    ((float (*)[4])&m->row[0])[1][3] = 0.0f;
    ((float (*)[4])&m->row[0])[2][3] = 0.0f;
    *(t4Vector*)((float (*)[4])&m->row[0])[3] = pos;
    vu0MulEqMtx_1668B8(m, &tm);
}

static inline t4Vector Row_1668B8(const t4Matrix& m, int i)
{
    return *(const t4Vector*)((float (*)[4])&m.row[0])[i];
}

extern "C" void cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading_impl(char* self, t4Vector* out) __asm__("cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading");
/**
 * Computes the look vector for the current Pitch and Heading: row 0 of t4mat_Identity turned by -Pitch about the y
 * axis and then by -Heading about the z axis.
 *
 * @param out  receives the look vector
 */
extern "C" void cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading_impl(char* self, t4Vector* out)
{
    t4Matrix lookMatrix;
    vu0CopyMtx_1668B8(&lookMatrix, &t4mat_Identity_1668B8);
    Rotate_1668B8(&lookMatrix, t4vect_YAxis_1668B8, -*(float*)(self + 0x54));
    Rotate_1668B8(&lookMatrix, t4vect_ZAxis_v1668B8, -*(float*)(self + 0x50));
    *out = Row_1668B8(lookMatrix, 0);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_snap);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


// 16-byte vector with inline constructors (a non-POD sibling of t4Vector, as in ai/ai); stores x, y, z, w.

// Global vectors and a matrix in .bss that the camera code reads (set up elsewhere at startup).
extern t4Vector t4vect_Zero_6C60 __asm__("t4vect_Zero");

extern t4Vector t4vect_ZAxis_6C60 __asm__("t4vect_ZAxis");

extern "C" void f_6C60_def(void* self, long id) __asm__("cLookAtChaseAlgorithm_snap");


typedef long (*LongFn_6C60)(void*);
typedef sVec4 (*VecFn_6C60)(void*);
typedef void (*VecFn2_6C60)(sVec4*, void*);
typedef float (*FloatFn_6C60)(void*);
typedef void (*VoidFn_6C60)(void*);
typedef void (*IntFn_6C60)(void*, int);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline void Scale_6C60(sVec4& r, const sVec4& v, float s)
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline void Cross_6C60(sVec4& r, const sVec4& a, const sVec4& b)
{
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "o"(a), "m"(b)
        : "memory");
}

static inline sVec4 getVec_6C60(char* self, int slot)
{
    char* cam = *(char**)(self + 0x30);
    sVEntry<void>* cvt = *(sVEntry<void>**)cam;
    return ((VecFn_6C60)cvt[slot].fn)(vthis(cam, cvt[slot]));
}

/**
 * Snaps the chase camera to the target rider, resetting its smoothing state, when @p id is the rider's subject id.
 *
 * Compares @p id with slot 7 of the target rider's cRider vtable (+0x6C0). On a match: FramesSinceHandplant 5000,
 * SecondsSinceLastHandplant 10, LastViewScalar 1, VerticalJumpFactor 1, AirExitSmoothness 0.92, SlowCamBlend 0.5; the
 * rail and half-pipe vectors are zeroed and most timers and flags cleared. LastCamDir and LastDirectionVel come from
 * the target's front (slot 4; the latter doubled), LastSlowVelocity and LastHorizontalVel from its velocity (slot 3,
 * z cleared), SlowCamEnterLook is front * 250, LastLookaheadSkew is t4vect_ZAxis x front, LastDrop is the algorithm's
 * slot 13, and the PID filter histories are cleared. Finishes with slot 5 and slot 8 (with 1).
 *
 * @param algorithm  the chase algorithm (this)
 * @param id  the subject id to snap to (On Tour snap(int subjectID))
 */
extern "C" void f_6C60_def(void* algorithm, long id)
{
    cLookAtChaseAlgorithm* self = (cLookAtChaseAlgorithm*)algorithm;
    char* mgr = (char*)&self->Target->Rider->mVtbl;
    sVEntry<void>* mvt = *(sVEntry<void>**)mgr;
    if (id == ((LongFn_6C60)mvt[7].fn)(vthis(mgr, mvt[7]))) {
        sVec4 front = getVec_6C60(((char*)self), 4);
        sVec4 velocity = getVec_6C60(((char*)self), 3);
        sVec4 vecCopy;
        sVec4 scratchVec;
        sVec4 directionVel;
        self->FramesSinceHandplant = 5000;
        self->SecondsSinceLastHandplant = 10.0f;
        self->LastViewScalar = 1.0f;
        *(int*)&self->LastHeight = 0;
        self->DoingHalfPipeCam = 0;
        self->HalfPipeCamInitialized = 0;
        self->LastRailMove = t4vect_Zero_6C60;
        self->LastHalfPipeMove = t4vect_Zero_6C60;
        self->LastHalfPipeDistCorrection = t4vect_Zero_6C60;
        *(sVec4*)&self->LastCamDir = front;
        sVEntry<void>* svt1 = (sVEntry<void>*)self->mVtbl;
        self->LastDrop = ((FloatFn_6C60)svt1[13].fn)(vthis(self, svt1[13]));
        *(int*)&self->ExtraBoostDist = 0;
        *(int*)&self->LastBoostWarp = 0;
        Cross_6C60(scratchVec, (const sVec4&)t4vect_ZAxis_6C60, front);
        vecCopy = scratchVec;
        self->AirExitSmoothness = 0.92f;
        *(sVec4*)&self->LastLookaheadSkew = scratchVec;
        self->HitGroundSinceStartingAirFunc = 1;
        self->AirCamSetupCountdown = -1;
        self->VerticalJumpFactor = 1.0f;
        *(int*)&self->LastCameraSwing = 0;
        self->JiggleFrameCount = 0;
        self->BoostingLastFrame = 0;
        *(int*)&self->AirSplineScalar = 0;
        self->ApplyingAirFunc = 0;
        *(int*)&self->LastCrouchFactor = 0;
        *(int*)&self->LastTargetCrouchFactor = 0;
        *(int*)&self->LastAirPositionDelta = 0;
        *(int*)&self->LastAirTargetDelta = 0;
        *(int*)&self->LastFullAirPositionDelta = 0;
        *(int*)&self->LastFullAirTargetDelta = 0;
        *(int*)&self->ViewCamLastFactor = 0;
        self->ApplyingFullAirFunc = 0;
        self->LastAirFrame = 0;
        *(int*)&self->HalfPipeBlendOutTimeLeft = 0;
        self->HalfPipeBlendOutLastLook = t4vect_Zero_6C60;
        *(int*)&self->HalfPipeCrossBlendFullTime = 0;
        *(int*)&self->HalfPipeCrossBlendTime = 0;
        *(sVec4*)&self->LastSlowVelocity = velocity;
        Scale_6C60(scratchVec, front, 250.0f);
        self->SlowCamState = 1;
        vecCopy = scratchVec;
        *(sVec4*)&self->SlowCamEnterLook = scratchVec;
        self->SlowCamBlend = 0.5f;
        *(int*)&self->LastHorizontalMoveLen = 0;
        *(int*)&self->LastEnemyPullback = 0;
        {
            char* cam = (char*)self->Target;
            sVEntry<void>* cvt = *(sVEntry<void>**)cam;
            ((VecFn2_6C60)cvt[4].fn)(&vecCopy, vthis(cam, cvt[4]));
        }
        self->ShakingLastFrame = 0;
        *(sVec4*)&self->LastOriginalVel = vecCopy;
        *(int*)&self->ShakeBlendOutTime = 0;
        *(int*)&self->LastHandplantRise = 0;
        self->DoSnap = 0;
        *(int*)&self->SteerTime = 0;
        self->TurningPIDFilter.NextFreeIndex = 0;
        int* pidHistory = (int*)(&self->TurningPIDFilter);
        for (int i = 4; i >= 0; i--) {
            pidHistory[0] = 0;
            pidHistory[5] = 0;
            pidHistory[10] = 0;
            pidHistory++;
        }
        {
            char* cam = (char*)self->Target;
            sVEntry<void>* cvt = *(sVEntry<void>**)cam;
            ((VecFn2_6C60)cvt[4].fn)(&scratchVec, vthis(cam, cvt[4]));
        }
        Scale_6C60(directionVel, scratchVec, 2.0f);
        vecCopy = directionVel;
        *(sVec4*)&self->LastDirectionVel = directionVel;
        *(sVec4*)&self->LastHorizontalVel = velocity;
        *(int*)&self->LastHorizontalVel.z = 0;
        sVEntry<void>* svt2 = (sVEntry<void>*)self->mVtbl;
        ((VoidFn_6C60)svt2[5].fn)(vthis(self, svt2[5]));
        sVEntry<void>* svt3 = (sVEntry<void>*)self->mVtbl;
        ((IntFn_6C60)svt3[8].fn)(vthis(self, svt3[8]), 1);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_snapCurrent);
#ifdef SKIP_ASM


/**
 * Snaps the camera to its current target: calls its own slot 3 (snap) with the target rider's subject id.
 *
 * The id comes from slot 7 of the target rider's cRider vtable (+0x6C0).
 *
 * @return slot 3's result (declared void* here; On Tour snapCurrent returns void)
 */
extern "C" void* cLookAtChaseAlgorithm_snapCurrent(cLookAtChaseAlgorithm* self)
{
    sVEntry<void*(void*, void*)>* vt = (sVEntry<void*(void*, void*)>*)self->mVtbl;
    char* targetMgr = (char*)&self->Target->Rider->mVtbl;
    sVEntry<void*(void*)>* vt2 = *(sVEntry<void*(void*)>**)targetMgr;
    return vt[3].fn(vthis(self, vt[3]), vt2[7].fn(vthis(targetMgr, vt2[7])));
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_calcPose);
#ifdef SKIP_ASM



extern t4Matrix t4mat_Identity_166F90 __asm__("t4mat_Identity");
extern t4Vector t4vect_YAxis_166F90 __asm__("t4vect_YAxis");
extern t4Vector t4vect_ZAxis_v166F90 __asm__("t4vect_ZAxis");
extern t4Vector t4vect_Origin_v166F90 __asm__("t4vect_Origin");
extern "C" void fSinCos(float* sout, float* cout, float x);

struct sPair_166F90 {
    t4Vector pos;
    t4Vector rot;
};

struct sCamXf_166F90 {
    char pad0[0x60];
    sPair_166F90 xf;
};

// tPose_tPose (bx/ps2main) fills *out from the matrix and returns out.
extern "C" sPair_166F90* tPose_tPose_166F90(sPair_166F90* out, t4Matrix* m) __asm__("tPose_tPose");

// PORT: PS2-only VU0 inline asm (64-byte matrix copy).
static inline void vu0CopyMtx_166F90(t4Matrix* dst, const t4Matrix* src)
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
static inline void vu0MulMtx_166F90(t4Matrix* dst, const t4Matrix* a, const t4Matrix* b)
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

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in place, m = m * b).
static inline void vu0MulEqMtx_166F90(t4Matrix* m, const t4Matrix* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
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
        : "r"(m), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (row 3 of m = m * v).
static inline void vu0TransMtx_166F90(t4Matrix* m, const t4Vector* v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, 0x0(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "r"(v)
        : "memory");
}

static inline void SinCos_166F90(float* s, float* c, float angle)
{
    fSinCos(s, c, angle);
}

static inline void Rotate_166F90(t4Matrix* m, const t4Vector& axis, float angle)
{
    t4Matrix tm;
    vu0CopyMtx_166F90(&tm, m);
    t4Vector pos = *(t4Vector*)((float (*)[4])&tm.row[0])[3];
    *(t4Vector*)((float (*)[4])&tm.row[0])[3] = t4vect_Origin_v166F90;
    float s;
    float c;
    SinCos_166F90(&s, &c, angle);
    float one = 1.0f;
    float t = one - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    ((float (*)[4])&m->row[0])[0][0] = tx * x + c;
    ((float (*)[4])&m->row[0])[1][0] = tx * y + sz;
    ((float (*)[4])&m->row[0])[2][0] = tx * z - sy;
    ((float (*)[4])&m->row[0])[0][1] = ty * x - sz;
    ((float (*)[4])&m->row[0])[1][1] = ty * y + c;
    ((float (*)[4])&m->row[0])[2][1] = ty * z + sx;
    ((float (*)[4])&m->row[0])[0][2] = tz * x + sy;
    ((float (*)[4])&m->row[0])[1][2] = tz * y - sx;
    ((float (*)[4])&m->row[0])[2][2] = tz * z + c;
    ((float (*)[4])&m->row[0])[3][0] = 0.0f;
    ((float (*)[4])&m->row[0])[3][1] = 0.0f;
    ((float (*)[4])&m->row[0])[3][2] = 0.0f;
    ((float (*)[4])&m->row[0])[3][3] = one;
    ((float (*)[4])&m->row[0])[0][3] = 0.0f;
    ((float (*)[4])&m->row[0])[1][3] = 0.0f;
    ((float (*)[4])&m->row[0])[2][3] = 0.0f;
    *(t4Vector*)((float (*)[4])&m->row[0])[3] = pos;
    vu0MulEqMtx_166F90(m, &tm);
}

static inline t4Vector ConjF_166F90(float x, float y, float z, float w)
{
    t4Vector r;
    r.w = w;
    r.x = -x;
    r.y = -y;
    r.z = -z;
    return r;
}

extern "C" void cLookAtChaseAlgorithm_calcPose_impl(char* self) __asm__("cLookAtChaseAlgorithm_calcPose");
/**
 * Builds CalculatedPose from Position, Pitch and Heading.
 *
 * Sets Position.w to 1, translates t4mat_Identity to Position, turns it by -Pitch about the y axis and by -Heading
 * about the z axis, converts the matrix with tPose_tPose into CalculatedPose, and then negates the quaternion's x,
 * y and z (its conjugate).
 */
extern "C" void cLookAtChaseAlgorithm_calcPose_impl(char* self)
{
    *(float*)(self + 0x4C) = 1.0f;
    t4Vector* pos = (t4Vector*)(self + 0x40);
    t4Matrix poseMatrix;
    vu0CopyMtx_166F90(&poseMatrix, &t4mat_Identity_166F90);
    vu0TransMtx_166F90(&poseMatrix, pos);
    Rotate_166F90(&poseMatrix, t4vect_YAxis_166F90, -*(float*)(self + 0x54));
    Rotate_166F90(&poseMatrix, t4vect_ZAxis_v166F90, -*(float*)(self + 0x50));
    {
        sPair_166F90 xf;
        tPose_tPose_166F90(&xf, &poseMatrix);
        ((sCamXf_166F90*)self)->xf = xf;
    }
    *(t4Vector*)(self + 0x70) = ConjF_166F90(*(float*)(self + 0x70), *(float*)(self + 0x74), *(float*)(self + 0x78), *(float*)(self + 0x7C));
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_getPose);
#ifdef SKIP_ASM
/**
 * Returns the camera pose (position and rotation) the algorithm computed.
 *
 * Copies the two 16-byte halves at +0x60 and +0x70 of the algorithm.
 *
 * @param pose  the hidden result buffer
 * @param algorithm  the algorithm (this)
 * @return @p pose, holding the pose
 */
extern "C" void* cLookAtChaseAlgorithm_getPose(void* pose, cLookAtChaseAlgorithm* algorithm)
{
    *(cQuad128*)pose = *(cQuad128*)&algorithm->CalculatedPose;
    *(cQuad128*)((char*)pose + 0x10) = *(cQuad128*)&algorithm->CalculatedPose.Q;
    return pose;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001673B8);
#ifdef SKIP_ASM
/**
 * Returns its first argument and ignores the rest (a varargs stub, like a compiled-out debug print).
 *
 * @return @p self
 */
extern "C" void* func_001673B8(void* self, ...)
{
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", func_001673F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
inline void* operator new(unsigned int, void* p) { return p; }

struct sColor_1673F8 {
    float r, g, b, a;
    sColor_1673F8(float ar, float ag, float ab, float aa) : r(ar), g(ag), b(ab), a(aa) {}
};

struct sFlags_1673F8 {
    unsigned char a;
    unsigned int b : 24;
    sFlags_1673F8(int aa, int ab) : a(aa), b(ab) {}
};

struct sMatrix_1673F8 {
    float m[4][4];
    sMatrix_1673F8(const float& m00, const float& m01, const float& m02, const float& m03,
                   const float& m10, const float& m11, const float& m12, const float& m13,
                   const float& m20, const float& m21, const float& m22, const float& m23,
                   const float& m30, const float& m31, const float& m32, const float& m33)
    {
        m[0][0] = m00; m[1][0] = m10; m[2][0] = m20; m[3][0] = m30;
        m[0][1] = m01; m[1][1] = m11; m[2][1] = m21; m[3][1] = m31;
        m[0][2] = m02; m[1][2] = m12; m[2][2] = m22; m[3][2] = m32;
        m[0][3] = m03; m[1][3] = m13; m[2][3] = m23; m[3][3] = m33;
    }
};

extern sColor_1673F8 D_004C5020;
extern sColor_1673F8 D_004C5030;
extern sColor_1673F8 D_004C5040;
extern sColor_1673F8 D_004C5050;
extern sColor_1673F8 D_004C5060;
extern sColor_1673F8 D_004C5070;
extern sColor_1673F8 D_004C5080;
extern sColor_1673F8 D_004C5090;
extern sColor_1673F8 D_004C50A0;
extern sColor_1673F8 D_004C50B0;
extern sColor_1673F8 D_004C50C0;
extern sColor_1673F8 D_004C50D0;
extern sColor_1673F8 D_004C50E0;
extern sColor_1673F8 D_004C50F0;
extern sColor_1673F8 D_004C5100;
extern sColor_1673F8 D_004C5110;
extern sColor_1673F8 D_004C5120;
extern sColor_1673F8 D_004C5130;
extern sColor_1673F8 D_004C5140;
extern sColor_1673F8 D_004C5150;
extern sColor_1673F8 D_004C5160;
extern sColor_1673F8 D_004C5170;
extern sColor_1673F8 D_004C5180;
extern sColor_1673F8 D_004C5190;
extern sColor_1673F8 D_004C51A0;
extern sColor_1673F8 D_004C51B0;
extern sColor_1673F8 D_004C51C0;
extern sColor_1673F8 D_004C51D0;
extern sColor_1673F8 D_004C51E0;
extern sColor_1673F8 D_004C51F0;
extern sColor_1673F8 D_004C5200;
extern sColor_1673F8 D_004C5210;
extern sColor_1673F8 D_004C5220;
extern sColor_1673F8 D_004C5230;
extern sColor_1673F8 D_004C5240;
extern sColor_1673F8 D_004C5250;
extern sColor_1673F8 D_004C5260;
extern sColor_1673F8 D_004C5270;
extern sColor_1673F8 D_004C5280;
extern sColor_1673F8 D_004C5290;
extern sColor_1673F8 D_004C52A0;
extern sColor_1673F8 D_004C52B0;
extern sColor_1673F8 D_004C52C0;
extern sColor_1673F8 D_004C52D0;
extern sColor_1673F8 D_004C52E0;
extern sColor_1673F8 D_004C52F0;
extern sColor_1673F8 D_004C5300;
extern sColor_1673F8 D_004C5310;
extern sColor_1673F8 D_004C5320;
extern sColor_1673F8 D_004C5330;
extern sColor_1673F8 D_004C5340;
extern sColor_1673F8 D_004C5350;
extern sColor_1673F8 D_004C5360;
extern sColor_1673F8 D_004C5370;
extern sColor_1673F8 D_004C5380;
extern sColor_1673F8 D_004C5390;
extern int D_004A4D40;
extern int D_004A4D44;
extern sFlags_1673F8 D_004A4D48;
extern sFlags_1673F8 D_004A4D50;
extern sFlags_1673F8 D_004A4D58;
extern sFlags_1673F8 D_004A4D60;
extern sFlags_1673F8 D_004A4D68;
extern sFlags_1673F8 D_004A4D70;
extern sFlags_1673F8 D_004A4D78;
extern sFlags_1673F8 D_004A4D80;
extern sFlags_1673F8 D_004A4D88;
extern sFlags_1673F8 D_004A4D90;
extern sFlags_1673F8 D_004A4D98;
extern sFlags_1673F8 D_004A4DA0;
extern sFlags_1673F8 D_004A4DA8;
extern sFlags_1673F8 D_004A4DB0;
extern sMatrix_1673F8 CamAxisFlip;
extern sFlags_1673F8 D_004A4DC0;
extern int persistentPlayerChoiceType[2];

extern "C" void func_0015E2A8(void);

// The unit declares func_001673F8 later as returning void*; bind this body by asm label.
extern "C" void func_001673F8_impl(int initialize, int priority) __asm__("func_001673F8");

/**
 * Constructs camera/camera.cpp's static constants (its static initializer).
 *
 * Runs only when @p initialize is set and @p priority is 0xFFFF. Zeroes the 8-byte name field D_004A4D40, builds the
 * fourteen region-type keys D_004A4D48..D_004A4DB0 with ids 0-13 and 56 float4 colour constants D_004C5020..D_004C5390
 * (the same per-unit copies the world-painter unit's func_002BBC38 builds), sets the axis-flip matrix CamAxisFlip (rows
 * (0, 0, 1, 0), (-1, 0, 0, 0), (0, 1, 0, 0), (0, 0, 0, 1)) that CamMat is built from, fills the camera-shake table
 * (func_0015E2A8), builds the key D_004A4DC0 with id -1, and sets both views' persistentPlayerChoiceType to 61 (0x3D,
 * no choice). func_00168298 calls it from the constructor table.
 *
 * @param initialize  nonzero to construct (nothing is done for 0)
 * @param priority  the init priority; only 0xFFFF does anything
 */
extern "C" void func_001673F8_impl(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
        new (&D_004C5020) sColor_1673F8(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004C5030) sColor_1673F8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5040) sColor_1673F8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5050) sColor_1673F8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004C5060) sColor_1673F8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004C5070) sColor_1673F8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004C5080) sColor_1673F8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004C5090) sColor_1673F8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004C50A0) sColor_1673F8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004C50B0) sColor_1673F8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004C50C0) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C50D0) sColor_1673F8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C50E0) sColor_1673F8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004C50F0) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5100) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5110) sColor_1673F8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5120) sColor_1673F8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5130) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5140) sColor_1673F8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C5150) sColor_1673F8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C5160) sColor_1673F8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004C5170) sColor_1673F8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004C5180) sColor_1673F8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004C5190) sColor_1673F8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004C51A0) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C51B0) sColor_1673F8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004C51C0) sColor_1673F8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004C51D0) sColor_1673F8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004C51E0) sColor_1673F8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004C51F0) sColor_1673F8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004C5200) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5210) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5220) sColor_1673F8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004C5230) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5240) sColor_1673F8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004C5250) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C5260) sColor_1673F8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5270) sColor_1673F8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004C5280) sColor_1673F8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004C5290) sColor_1673F8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004C52A0) sColor_1673F8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004C52B0) sColor_1673F8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004C52C0) sColor_1673F8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004C52D0) sColor_1673F8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004C52E0) sColor_1673F8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C52F0) sColor_1673F8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5300) sColor_1673F8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004C5310) sColor_1673F8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004C5320) sColor_1673F8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004C5330) sColor_1673F8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004C5340) sColor_1673F8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004C5350) sColor_1673F8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004C5360) sColor_1673F8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004C5370) sColor_1673F8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004C5380) sColor_1673F8(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004C5390) sColor_1673F8(1.0f, 1.0f, 0.0f, 0.0f);
        D_004A4D44 = 0;
        D_004A4D40 = 0;
        new (&D_004A4D48) sFlags_1673F8(0, 0);
        new (&D_004A4D50) sFlags_1673F8(1, 1);
        new (&D_004A4D58) sFlags_1673F8(2, 2);
        new (&D_004A4D60) sFlags_1673F8(3, 3);
        new (&D_004A4D68) sFlags_1673F8(4, 4);
        new (&D_004A4D70) sFlags_1673F8(5, 5);
        new (&D_004A4D78) sFlags_1673F8(6, 6);
        new (&D_004A4D80) sFlags_1673F8(7, 7);
        new (&D_004A4D88) sFlags_1673F8(8, 8);
        new (&D_004A4D90) sFlags_1673F8(9, 9);
        new (&D_004A4D98) sFlags_1673F8(10, 10);
        new (&D_004A4DA0) sFlags_1673F8(11, 11);
        new (&D_004A4DA8) sFlags_1673F8(12, 12);
        new (&D_004A4DB0) sFlags_1673F8(13, 13);
        new (&CamAxisFlip) sMatrix_1673F8(0.0f, 0.0f, 1.0f, 0.0f,
                                         -1.0f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f);
        func_0015E2A8();
        new (&D_004A4DC0) sFlags_1673F8(-1, -1);
        persistentPlayerChoiceType[0] = 61;
        persistentPlayerChoiceType[1] = 61;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cPIDFilter_getDefaultProportionFactor);
#ifdef SKIP_ASM
/**
 * Returns 0.1f, the value cLookAtChaseAlgorithm_cLookAtChaseAlgorithm first stores in TurningPIDFilter's
 * ProportionFactor.
 *
 * @return 0.1f
 */
extern "C" float cPIDFilter_getDefaultProportionFactor(void)
{
    return 0.1f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cPIDFilter_getDefaultErrorHistoryFactor);
#ifdef SKIP_ASM
/**
 * Returns 0.1f, the value cLookAtChaseAlgorithm_cLookAtChaseAlgorithm first stores in TurningPIDFilter's
 * ErrorHistoryFactor.
 *
 * @return 0.1f
 */
extern "C" float cPIDFilter_getDefaultErrorHistoryFactor(cPIDFilter* self)
{
    return 0.10000000149011612f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cPIDFilter_getDefaultDerrivativeFactor);
#ifdef SKIP_ASM
/**
 * Returns 0.1f, the value cLookAtChaseAlgorithm_cLookAtChaseAlgorithm first stores in TurningPIDFilter's
 * DerrivativeFactor.
 *
 * @return 0.1f
 */
extern "C" float cPIDFilter_getDefaultDerrivativeFactor(cPIDFilter* self)
{
    return 0.10000000149011612f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cPIDFilter_reset);
#ifdef SKIP_ASM
struct sCamFilter {
    float field_0x00[5];
    float field_0x14[5];
    int field_0x28[5];
    int field_0x3C;
};

/**
 * Resets the PID filter: InputHistory and ResultHistory all @p value, Difference and NextFreeIndex 0.
 *
 * @param value  the starting input and result
 */
extern "C" void cPIDFilter_reset(cPIDFilter* self, float value)
{
    int i;
    self->NextFreeIndex = 0;
    for (i = 0; i < 5; i++) {
        self->InputHistory[i] = value;
        self->ResultHistory[i] = value;
        ((int*)&self->Difference[0])[i] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm__cCameraAlgorithm);
#ifdef SKIP_ASM
extern void* cCameraAlgorithm_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys a camera algorithm base: restores cCameraAlgorithm's vtable (cCameraAlgorithm_vtbl) and frees the object when bit 0
 * of @p flags is set (operator delete).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cCameraAlgorithm__cCameraAlgorithm(cCameraAlgorithm* self, int flags)
{
    self->mVtbl = cCameraAlgorithm_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_snap__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cCameraAlgorithm's default for vtable slot 3, which cLookAtChaseAlgorithm_snap overrides.
 */
void cCameraAlgorithm_snap(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_snapCurrent__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cCameraAlgorithm's default for vtable slot 4, which cLookAtChaseAlgorithm_snapCurrent overrides.
 */
void cCameraAlgorithm_snapCurrent(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cCameraAlgorithm's default for vtable slot 5, which the chase algorithms' update functions (e.g.
 * cIdleChaseAlgorithm_update) override.
 */
void cCameraAlgorithm_update(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_getDefaultView);
#ifdef SKIP_ASM
/**
 * Returns the default field of view of a camera algorithm: pi/4.
 *
 * @return pi/4 (45 degrees)
 */
extern "C" float cCameraAlgorithm_getDefaultView(cCameraAlgorithm* self)
{
    return 0.7853981852531433f;
}
#endif

extern cQuad128 t4vect_Zero;

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_getTargetPos);
#ifdef SKIP_ASM
/**
 * Returns t4vect_Zero: cCameraAlgorithm's default for vtable slot 6, which cChaseAlgorithm_getTargetPos overrides.
 *
 * @return @p self, the hidden return slot, holding t4vect_Zero
 */
extern "C" void* cCameraAlgorithm_getTargetPos(void* self)
{
    *(cQuad128*)self = t4vect_Zero;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_getRaceCamFinalPhaseData__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cCameraAlgorithm's default for vtable slot 7 (no class in the camera unit overrides it).
 */
void cCameraAlgorithm_getRaceCamFinalPhaseData(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_doRaceCamFinalPhase__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: cCameraAlgorithm's default for vtable slot 8, which cLookAtChaseAlgorithm_doRaceCamFinalPhase
 * overrides.
 */
void cCameraAlgorithm_doRaceCamFinalPhase(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_updateAlgorithm);
#ifdef SKIP_ASM
class cCameraAlgorithm_updateAlgorithm_cObj {
public:
    char pad[0x10];
    // vptr lands at 0x10 (g++ 2.95 places it after the class's own data);
    // slot N lives at vtable offset N*8 (delta at +0, function at +4)
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void update();
};

/**
 * Base updateAlgorithm: just calls the algorithm's update (virtual slot 5). On Tour's takes a bool ignoreCollisions.
 */
extern "C" void cCameraAlgorithm_updateAlgorithm(cCameraAlgorithm* self)
{
    ((cCameraAlgorithm_updateAlgorithm_cObj*)self)->update();
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_notifyMoveZ__FPv);
#ifdef SKIP_ASM
/**
 * Default handler for the camera having moved vertically: does nothing.
 *
 * Empty base-class virtual; the On Tour flag argument (didMoveZ) is unused, so the C omits it.
 */
void cCameraAlgorithm_notifyMoveZ(void* self)
{
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_shouldSnapOnUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Returns 0 (false): cCameraAlgorithm's default for vtable slot 11, which cLookAtChaseAlgorithm_shouldSnapOnUpdate__FPv
 * overrides.
 *
 * @return 0
 */
int cCameraAlgorithm_shouldSnapOnUpdate(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cBasicCameraAlgorithm__cBasicCameraAlgorithm);
#ifdef SKIP_ASM
extern void* cCameraAlgorithm_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys a basic camera algorithm: restores the base vtable cCameraAlgorithm_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (operator delete)
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cBasicCameraAlgorithm__cBasicCameraAlgorithm(cBasicCameraAlgorithm* self, int flags)
{
    self->mVtbl = cCameraAlgorithm_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cBasicCameraAlgorithm_setPosition);
#ifdef SKIP_ASM
/**
 * Sets Position: a 16-byte copy of *@p position.
 *
 * @param basicCameraAlgorithm  the algorithm
 * @param position  the new position
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void cBasicCameraAlgorithm_setPosition(cBasicCameraAlgorithm* basicCameraAlgorithm, long long *position) {
    *(long long*)&basicCameraAlgorithm->Position = (long long) *position;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cBasicCameraAlgorithm_setPitch);
#ifdef SKIP_ASM
/**
 * Sets the camera algorithm's pitch (Pitch).
 *
 * @param basicCameraAlgorithm  the algorithm
 * @param fparg0  the new pitch (radians)
 */
extern "C" void cBasicCameraAlgorithm_setPitch(cBasicCameraAlgorithm* basicCameraAlgorithm, float fparg0) {
    (basicCameraAlgorithm->Pitch) = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cBasicCameraAlgorithm_setHeading);
#ifdef SKIP_ASM
/**
 * Sets the camera algorithm's heading (Heading).
 *
 * @param basicCameraAlgorithm  the algorithm
 * @param fparg0  the new heading (radians)
 */
extern "C" void cBasicCameraAlgorithm_setHeading(cBasicCameraAlgorithm* basicCameraAlgorithm, float fparg0) {
    (basicCameraAlgorithm->Heading) = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cBasicCameraAlgorithm_getPosition);
#ifdef SKIP_ASM
/**
 * Returns Position by value: copies it into @p outPosition.
 *
 * @param outPosition  the hidden return slot
 * @param algorithm  the algorithm (a cBasicCameraAlgorithm)
 * @return @p outPosition
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" long long *cBasicCameraAlgorithm_getPosition(long long *outPosition, cBasicCameraAlgorithm* algorithm) {
    *outPosition = (*(long long*)&algorithm->Position);
    return outPosition;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cBasicCameraAlgorithm_getPitch);
#ifdef SKIP_ASM
/**
 * Returns the camera algorithm's pitch (Pitch).
 *
 * @param basicCameraAlgorithm  the algorithm
 * @return the pitch (radians)
 */
extern "C" float cBasicCameraAlgorithm_getPitch(cBasicCameraAlgorithm* basicCameraAlgorithm) {
    return (basicCameraAlgorithm->Pitch);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cBasicCameraAlgorithm_getHeading);
#ifdef SKIP_ASM
/**
 * Returns the camera algorithm's heading (Heading).
 *
 * @param basicCameraAlgorithm  the algorithm
 * @return the heading (radians)
 */
extern "C" float cBasicCameraAlgorithm_getHeading(cBasicCameraAlgorithm* basicCameraAlgorithm) {
    return (basicCameraAlgorithm->Heading);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cManualCameraAlgorithm__cManualCameraAlgorithm);
#ifdef SKIP_ASM
extern void* cCameraAlgorithm_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys a manual camera algorithm: restores the base vtable cCameraAlgorithm_vtbl (the class adds no fields).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (operator delete)
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cManualCameraAlgorithm__cManualCameraAlgorithm(cBasicCameraAlgorithm* self, int flags)
{
    self->mVtbl = cCameraAlgorithm_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cEditorCameraAlgorithm__cEditorCameraAlgorithm);
#ifdef SKIP_ASM
extern void* cCameraAlgorithm_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys an editor camera algorithm: restores the base vtable cCameraAlgorithm_vtbl (the class adds no fields).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (operator delete)
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cEditorCameraAlgorithm__cEditorCameraAlgorithm(cBasicCameraAlgorithm* self, int flags)
{
    self->mVtbl = cCameraAlgorithm_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseAlgorithm__cChaseAlgorithm);
#ifdef SKIP_ASM
extern void* cCameraAlgorithm_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys a chase camera algorithm: restores the base vtable cCameraAlgorithm_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (operator delete)
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cChaseAlgorithm__cChaseAlgorithm(cChaseAlgorithm* self, int flags)
{
    self->mVtbl = cCameraAlgorithm_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cChaseAlgorithm_getTargetPos);
#ifdef SKIP_ASM
/**
 * Returns the chase algorithm's target position (CamTargetPos).
 *
 * self is the hidden t4Vector return slot; the algorithm itself comes second.
 *
 * @param algorithm  the cChaseAlgorithm
 * @return self, holding a copy of CamTargetPos (+0x20)
 */
extern "C" void* cChaseAlgorithm_getTargetPos(void* self, cChaseAlgorithm* algorithm)
{
    *(cQuad128*)self = *(cQuad128*)&algorithm->CamTargetPos;
    return self;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_shouldSnapOnUpdate__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the look-at chase algorithm should snap on its next update.
 *
 * @return the DoSnap flag (nonzero to snap)
 */
int cLookAtChaseAlgorithm_shouldSnapOnUpdate(void* vself)
{
    cLookAtChaseAlgorithm* self = (cLookAtChaseAlgorithm*)vself;
    return self->DoSnap;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraController_update__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the camera controller's update has an empty body.
 */
void cCameraController_update(void* vself)
{
    cCameraController* self = (cCameraController*)vself;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_setAlgorithmType);
#ifdef SKIP_ASM
/**
 * Sets the camera algorithm's type (AlgorithmType).
 *
 * @param cameraAlgorithm  the algorithm
 * @param algoType  the new type (an eCamAlgoType)
 */
extern "C" void cCameraAlgorithm_setAlgorithmType(cCameraAlgorithm* cameraAlgorithm, int algoType) {
    (cameraAlgorithm->AlgorithmType) = algoType;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cCameraAlgorithm_getType);
#ifdef SKIP_ASM
/**
 * Returns the camera algorithm's type (AlgorithmType).
 *
 * @param cameraAlgorithm  the algorithm
 * @return the type (an eCamAlgoType)
 */
extern "C" int cCameraAlgorithm_getType(cCameraAlgorithm* cameraAlgorithm) {
    return (cameraAlgorithm->AlgorithmType);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cCameraController_getType);
#ifdef SKIP_ASM
/**
 * Returns a camera controller's Type (its first word).
 *
 * @param controller  the controller
 * @return the type (an eCamControllerType)
 */
extern "C" int cCameraController_getType(int *controller) {
    return *controller;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cCameraController_getBlendType);
#ifdef SKIP_ASM
/**
 * Returns the controller's BlendType.
 *
 * Declared on cChaseCameraController but reads the field every cCameraController has.
 *
 * @param chaseCameraController  the controller
 * @return BlendType (an eCamBlendType)
 */
extern "C" int cCameraController_getBlendType(cCameraController* chaseCameraController) {
    return (chaseCameraController->BlendType);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cCameraController_getAlgorithmList);
#ifdef SKIP_ASM
/**
 * Returns the controller's AlgorithmList.
 *
 * Declared on cChaseCameraController but reads the field every cCameraController has.
 *
 * @param chaseCameraController  the controller
 * @return AlgorithmList (a cCameraAlgoList pointer, returned as int)
 */
extern "C" int cCameraController_getAlgorithmList(cCameraController* chaseCameraController) {
    return ((int)chaseCameraController->AlgorithmList);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_getLatestAlgoType);
#ifdef SKIP_ASM
/**
 * Returns LatestAlgoType, the type of the algorithm cChaseCameraController_addAlgorithm added last.
 *
 * @param chaseCameraController  the controller
 * @return the algorithm type (an eCamAlgoType)
 */
extern "C" int cChaseCameraController_getLatestAlgoType(cChaseCameraController* chaseCameraController) {
    return (chaseCameraController->LatestAlgoType);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_getTarget);
#ifdef SKIP_ASM
/**
 * Returns Target, the camera target the chase controller follows.
 *
 * @param chaseCameraController  the controller
 * @return the target (a cRiderCameraTarget*)
 */
extern "C" int cChaseCameraController_getTarget(cChaseCameraController* chaseCameraController) {
    return ((int)chaseCameraController->Target);
}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00167FF8);
#ifdef SKIP_ASM
/**
 * Does nothing (nothing in the code or data refers to it).
 */
extern "C" void func_00167FF8(void) {

}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168000);
#ifdef SKIP_ASM
/**
 * Does nothing (nothing in the code or data refers to it).
 */
extern "C" void func_00168000(void) {

}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168008);
#ifdef SKIP_ASM
/**
 * Does nothing (nothing in the code or data refers to it).
 */
extern "C" void func_00168008(void) {

}
#endif
//100%
INCLUDE_ASM("camera/camera", func_00168010);
#ifdef SKIP_ASM
/**
 * Does nothing (nothing in the code or data refers to it).
 */
extern "C" void func_00168010(void) {

}
#endif
//100%
INCLUDE_ASM("camera/camera", cChaseCameraController_getPreferredChaseAlgorithm);
#ifdef SKIP_ASM
/**
 * Returns 61 (0x3D, the "no algorithm" eCamAlgoType that cChaseCameraController_switchCamera_HardReset stores).
 *
 * @return 61
 */
extern "C" int cChaseCameraController_getPreferredChaseAlgorithm(void) {
    return 61;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cManualCameraController_getManualAlgo);
#ifdef SKIP_ASM
/**
 * Returns the manual camera controller's manual algorithm (+0x18, the "Manual Cam Algo" its constructor creates).
 *
 * @param manualController  the controller
 * @return the algorithm (a cManualCameraAlgorithm*)
 */
extern "C" int cManualCameraController_getManualAlgo(cManualCameraController* manualController) {
    return ((int)manualController->ManualAlgo);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cManualCameraController_getEditorAlgo);
#ifdef SKIP_ASM
/**
 * Returns the manual camera controller's editor algorithm (+0x1C, the "Editor Cam Algo" its constructor creates).
 *
 * @param manualController  the controller
 * @return the algorithm (a cEditorCameraAlgorithm*)
 */
extern "C" int cManualCameraController_getEditorAlgo(cManualCameraController* manualController) {
    return ((int)manualController->EditorAlgo);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cSimpleCamera__cSimpleCamera);
#ifdef SKIP_ASM
extern void* cSimpleCamera_vtbl[];
void operator_delete(int* ptr);

/**
 * Destroys a simple camera: restores its vtable pointer, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
// deleting destructor: reset the vtable, free when bit 0 of flags is set
extern "C" void cSimpleCamera__cSimpleCamera(cSimpleCamera* self, int flags)
{
    self->mVtbl = cSimpleCamera_vtbl;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* cSimpleCamera_updateCamMat(void* self);

//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_update__FPv);
#ifdef SKIP_ASM
/**
 * Rebuilds the camera matrix from the pose: calls cSimpleCamera_updateCamMat (slot 3 of cSimpleCamera_vtbl).
 *
 * @return cSimpleCamera_updateCamMat's result (declared void*)
 */
void* cSimpleCamera_update(void* self)
{
    return cSimpleCamera_updateCamMat(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_getFOV);
#ifdef SKIP_ASM
/**
 * Returns the camera's field of view (View, its first float).
 *
 * @param camera  the camera (a cSimpleCamera)
 * @return the field of view (radians)
 */
extern "C" float cSimpleCamera_getFOV(float *camera) {
    return *camera;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_getNear);
#ifdef SKIP_ASM
/**
 * Returns Near, the near plane distance.
 *
 * @param simpleCamera  the camera
 * @return the near distance
 */
extern "C" float cSimpleCamera_getNear(cSimpleCamera* simpleCamera) {
    return (simpleCamera->Near);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_getFar);
#ifdef SKIP_ASM
/**
 * Returns Far, the far plane distance.
 *
 * @param simpleCamera  the camera
 * @return the far distance
 */
extern "C" float cSimpleCamera_getFar(cSimpleCamera* simpleCamera) {
    return (simpleCamera->Far);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_getCamMat);
#ifdef SKIP_ASM
/**
 * Returns the address of CamMat (camera + 0x40).
 *
 * @param camera  the camera's address (a cSimpleCamera)
 * @return the matrix's address
 */
extern "C" int cSimpleCamera_getCamMat(int camera) {
    return camera + 0x40;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_getPosition);
#ifdef SKIP_ASM
/**
 * Returns the address of Pose (camera + 0x20), whose position comes first.
 *
 * @param camera  the camera's address (a cSimpleCamera)
 * @return the pose's address
 */
extern "C" int cSimpleCamera_getPosition(int camera) {
    return camera + 0x20;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_getPose);
#ifdef SKIP_ASM
/**
 * Returns the address of Pose (camera + 0x20).
 *
 * @param camera  the camera's address (a cSimpleCamera)
 * @return the pose's address
 */
extern "C" int cSimpleCamera_getPose(int camera) {
    return camera + 0x20;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_getDirection);
#ifdef SKIP_ASM
/**
 * Returns the address of Direction (camera + 0x80).
 *
 * @param camera  the camera's address (a cSimpleCamera)
 * @return the direction's address
 */
extern "C" int cSimpleCamera_getDirection(int camera) {
    return camera + 0x80;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_setPosition);
#ifdef SKIP_ASM
/**
 * Sets Pose's position: a 16-byte copy of *@p position.
 *
 * @param simpleCamera  the camera
 * @param position  the new position
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void cSimpleCamera_setPosition(cSimpleCamera* simpleCamera, long long *position) {
    *(long long*)&simpleCamera->Pose = (long long) *position;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_setOrientation);
#ifdef SKIP_ASM
/**
 * Sets Pose's rotation quaternion (Pose.Q): a 16-byte copy of *@p orientation.
 *
 * @param simpleCamera  the camera
 * @param orientation  the new quaternion
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void cSimpleCamera_setOrientation(cSimpleCamera* simpleCamera, long long *orientation) {
    *(long long*)&simpleCamera->Pose.Q = (long long) *orientation;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_setPose);
#ifdef SKIP_ASM
/**
 * Sets Pose: copies the position (+0x0) and the quaternion (+0x10) of @p pose.
 *
 * @param simpleCamera  the camera
 * @param pose  the new pose (a tPose)
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" void cSimpleCamera_setPose(cSimpleCamera* simpleCamera, tPose* pose) {
    *(long long*)&simpleCamera->Pose = (long long) *(long long*)&pose->P;
    *(long long*)&simpleCamera->Pose.Q = (long long) *(long long*)&pose->Q;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_setFOV);
#ifdef SKIP_ASM
/**
 * Sets the field of view (View, the camera's first float).
 *
 * @param camera  the camera (a cSimpleCamera)
 * @param fparg0  the new field of view (radians)
 */
extern "C" void cSimpleCamera_setFOV(float *camera, float fparg0) {
    *camera = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_setMaxFar);
#ifdef SKIP_ASM
/**
 * Sets MaxFar, the upper limit for Near and Far.
 *
 * @param simpleCamera  the camera
 * @param fparg0  the new limit
 */
extern "C" void cSimpleCamera_setMaxFar(cSimpleCamera* simpleCamera, float fparg0) {
    (simpleCamera->MaxFar) = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_setMinNear);
#ifdef SKIP_ASM
/**
 * Sets MinNear, the lower limit for Near and Far.
 *
 * @param simpleCamera  the camera
 * @param fparg0  the new limit
 */
extern "C" void cSimpleCamera_setMinNear(cSimpleCamera* simpleCamera, float fparg0) {
    (simpleCamera->MinNear) = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cSimpleCamera_setMaxFOV);
#ifdef SKIP_ASM
/**
 * Sets MaxView, the upper limit for the field of view.
 *
 * @param simpleCamera  the camera
 * @param fparg0  the new limit (radians)
 */
extern "C" void cSimpleCamera_setMaxFOV(cSimpleCamera* simpleCamera, float fparg0) {
    (simpleCamera->MaxView) = fparg0;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cCamera_getCurrentController);
#ifdef SKIP_ASM
/**
 * Returns Controller, the controller currently driving the camera.
 *
 * @param camera  the camera
 * @return the controller (a cCameraController*)
 */
extern "C" int cCamera_getCurrentController(cCamera* camera) {
    return ((int)camera->Controller);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cCamera_getManualController);
#ifdef SKIP_ASM
/**
 * Returns the camera's manual controller (ManualController).
 *
 * @param camera  the camera
 * @return the manual controller
 */
extern "C" int cCamera_getManualController(cCamera* camera) {
    return ((int)camera->ManualController);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cCamera_getChaseController);
#ifdef SKIP_ASM
/**
 * Returns the camera's chase controller (ChaseController).
 *
 * @param camera  the camera
 * @return the chase controller
 */
extern "C" int cCamera_getChaseController(cCamera* camera) {
    return ((int)camera->ChaseController);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cCamera_getScriptController);
#ifdef SKIP_ASM
/**
 * Returns the camera's script controller (ScriptController).
 *
 * @param camera  the camera
 * @return the script controller
 */
extern "C" int cCamera_getScriptController(cCamera* camera) {
    return ((int)camera->ScriptController);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cCamera_isStreamReady);
#ifdef SKIP_ASM
/**
 * Returns bStreamIsReady, which cCamera_updateStreamFocus sets.
 *
 * @param camera  the camera
 * @return nonzero when the camera's stream is ready
 */
extern "C" int cCamera_isStreamReady(cCamera* camera) {
    return (camera->bStreamIsReady);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cCamera_getStreamFocusID);
#ifdef SKIP_ASM
/**
 * Returns StreamFocusID (0 for view 0, 1 for view 1, -1 before cCamera_init).
 *
 * @param camera  the camera
 * @return the stream focus id (an eStreamFocusID)
 */
extern "C" int cCamera_getStreamFocusID(cCamera* camera) {
    return (camera->StreamFocusID);
}
#endif
//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_setRider);
#ifdef SKIP_ASM
/**
 * Sets the camera target's Rider (+0x4).
 *
 * @param cameraTarget  the target (a cRiderCameraTarget)
 * @param rider  the rider (a cRider*)
 */
extern "C" void cRiderCameraTarget_setRider(void *cameraTarget, int rider) {
    (*(int *)((char*)(cameraTarget) + (4))) = rider;
}
#endif
//100%
INCLUDE_ASM("camera/camera", cRiderCameraTarget_getRider);
#ifdef SKIP_ASM
/**
 * Returns the camera target's Rider.
 *
 * @param riderCameraTarget  the target
 * @return the rider (a cRider*)
 */
extern "C" int cRiderCameraTarget_getRider(cRiderCameraTarget* riderCameraTarget) {
    return ((int)riderCameraTarget->Rider);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookAtChaseAlgorithm_verify);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector t4vect_ZAxis_8150 __asm__("t4vect_ZAxis");

// PORT: the unit declares cLookAtChaseAlgorithm_verify as returning void*; this is the real (void) definition.
extern "C" void cLookAtChaseAlgorithm_verify_def(char* self) __asm__("cLookAtChaseAlgorithm_verify");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub_168150b(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub2_168150(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__ __volatile__(
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
static inline t4Vector cmAdd_168150(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector cmScale_168150(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline t4Vector cmScaleP_168150(const t4Vector* v, float s)
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
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float cmLength_168150b(const t4Vector& v)
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
 * Puts the eye back behind the target when it is too close to or too far from it.
 *
 * When the distance from Position to CamTargetPos is under 20 or over 1e20, CamTargetPos becomes the target's position
 * (Target virtual slot 1, cRiderCameraTarget_getPosition), Position becomes that position minus 200 times the vector at
 * +0x1B0 of Target's Rider, plus 50 up (t4vect_ZAxis), and DoSnap is set.
 */
// cLookAtChaseAlgorithm::verify(void): if the camera is closer to / farther from its target than the
// allowed range, snap it back behind the target.
extern "C" void cLookAtChaseAlgorithm_verify_def(char* self)
{
    t4Vector eyeToTarget = cmSub_168150b(*(t4Vector*)(self + 0x20), *(t4Vector*)(self + 0x40));
    float len = cmLength_168150b(eyeToTarget);
    if (len < 20.0f || len > 1.0000000200408773e+20f) {
        char* riderTarget = *(char**)(self + 0x30);
        t4Vector x = *(t4Vector*)(*(char**)(riderTarget + 4) + 0x1B0);
        sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)riderTarget)[1];
        t4Vector targetPos = vEntry->fn(vthis(riderTarget, *vEntry));
        int one = 1;
        float liftHeight = 50.0f;
        t4Vector np = cmAdd_168150(cmSub2_168150(targetPos, cmScale_168150(x, 200.0f)), cmScaleP_168150(&t4vect_ZAxis_8150, liftHeight));
        *(t4Vector*)(self + 0x40) = np;
        *(t4Vector*)(self + 0x20) = targetPos;
        *(int*)(self + 0x2F0) = one;
    }
}
#endif

extern "C" void* func_001673F8(int, int);

//99.38%
INCLUDE_ASM("camera/camera", func_00168298__FPv);
#ifdef SKIP_ASM
/**
 * Runs the camera unit's static initializer func_001673F8(1, 0xFFFF): a global constructor entry.
 *
 * @return func_001673F8's return value (declared void*; that function returns nothing)
 */
void* func_00168298(void* self)
{
    return func_001673F8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLazyChaseAlgorithm_cLazyChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(cLookAtChaseAlgorithm* self);
extern void* cLazyChaseAlgorithm_vtbl[];

/**
 * Constructs the lazy chase camera algorithm.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 0xD (On Tour CAMERA_ALGORITHM_CHASE_LAZY = 13) and
 * cLazyChaseAlgorithm_vtbl.
 *
 * @return @p self
 */
extern "C" void* cLazyChaseAlgorithm_cLazyChaseAlgorithm(cLazyChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(((cLookAtChaseAlgorithm*)self));
    self->AlgorithmType = 13;
    self->mVtbl = cLazyChaseAlgorithm_vtbl;
    return ((void*)self);
}
#endif

extern void* cLazyChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/camera", cLazyChaseAlgorithm__cLazyChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a lazy chase camera algorithm: stores cLazyChaseAlgorithm_vtbl and chains to the look-at chase destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cLazyChaseAlgorithm__cLazyChaseAlgorithm(void* vself)
{
    cLazyChaseAlgorithm* self = (cLazyChaseAlgorithm*)vself;
    self->mVtbl = (void**)((int)(void*)cLazyChaseAlgorithm_vtbl);
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLazyChaseAlgorithm_getRestDrop__FPv);
#ifdef SKIP_ASM
/**
 * Returns the lazy chase camera's rest drop (vtable slot 13, On Tour getRestDrop): always 0.
 *
 * @return 0
 */
float cLazyChaseAlgorithm_getRestDrop(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLazyChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the lazy chase camera: only clears DoPitchBlending (+0x10), so
 * doRaceCamFinalPhase skips pitch blending.
 *
 * @param other  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
void cLazyChaseAlgorithm_getRaceCamFinalPhaseData(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLazyChaseAlgorithm_update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

extern t4Vector t4vect_ZAxis_8338 __asm__("t4vect_ZAxis");

// The per-frame camera input record (0x70 bytes) that cLookAtChaseAlgorithm_setupChaseData_8338 fills from the target for the chase algorithms.
extern "C" void cLookAtChaseAlgorithm_setupChaseData_8338(void* self, sCamInfo* out, float a, float b, float c, float d,
                              float e, float f, float g, float h) __asm__("cLookAtChaseAlgorithm_setupChaseData");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector cmScale_001607E0_8338(const t4Vector& v, float s)
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
static inline t4Vector cmAdd_001607E0_8338(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).
static inline t4Vector cmScaleP_1848(const t4Vector* v, float s)
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
        : "o"(*v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector cmSub_168150(const t4Vector& a, const t4Vector& b)
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
static inline float cmLength_168150(const t4Vector& v)
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
static inline void cmScaleEq_8338(t4Vector& v, float s)
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

extern "C" void cLazyChaseAlgorithm_update_v(char* self) __asm__("cLazyChaseAlgorithm_update");

/**
 * Runs one frame of the lazy chase camera: the eye trails the rider on a loose leash.
 *
 * After setupChaseData (cLookAtChaseAlgorithm_setupChaseData) sets CamTargetPos to the setup's position + Z * 2.36. With d = CamTargetPos -
 * Position, the wanted eye is 180.2 units back from the target along d; Position eases toward it, keeping 0.992 of
 * the old eye at distances up to 188.1 and 0.860 from 775.5 on (linear between).
 */
extern "C" void cLazyChaseAlgorithm_update_v(char* vself)
{
    cLazyChaseAlgorithm* self = (cLazyChaseAlgorithm*)vself;
    sCamInfo info;
    cLookAtChaseAlgorithm_setupChaseData_8338(self, &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f,
                  0.8513929843902588f, 0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f,
                  0.6000000238418579f);
    self->CamTargetPos = cmAdd_001607E0_8338(info.pos, cmScaleP_1848(&t4vect_ZAxis_8338, 2.3602666854858398f));
    t4Vector offset = cmSub_168150(self->CamTargetPos, self->Position);
    float len = cmLength_168150(offset);
    if (len > 0.0f) {
        cmScaleEq_8338(offset, 180.19183349609375f / len);
        t4Vector wantedEye = cmSub_168150(self->CamTargetPos, offset);
        float blend;
        if (len >= 188.07302856445312f) {
            if (len <= 775.4580078125f) {
                blend = (len - 188.07302856445312f) / 587.385009765625f;
            } else {
                blend = 1.0f;
            }
        } else {
            blend = 0.0f;
        }
        float keepFraction = (1.0f - blend) * 0.9916903972625732f + blend * 0.8601375222206116f;
        self->Position = cmAdd_001607E0_8338(cmScale_001607E0_8338(self->Position, keepFraction), cmScale_001607E0_8338(wantedEye, 1.0f - keepFraction));
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLazyChaseAlgorithm_snap);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type;
// sVEntry<void> is a raw slot). Virtual calls in this unit go through these by hand.


extern t4Vector t4vect_ZAxis_8508 __asm__("t4vect_ZAxis");

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector cmScale_001607E0_8508(const t4Vector& v, float s)
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
static inline t4Vector cmAdd_001607E0_8508(const t4Vector& a, const t4Vector& b)
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

static inline t4Vector cmGet_001607E0_8508(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEntry<t4Vector(void*)>* vEntry = &(*(sVEntry<t4Vector(void*)>**)o)[slot];
    return vEntry->fn(vthis(o, *vEntry));
}

// PORT: PS2-only VU0 inline asm (vector at pointer times scalar).


// PORT: PS2-only VU0 inline asm (vector subtract).


extern "C" void cLookAtChaseAlgorithm_snap(void* self, int id);
extern "C" void cLookAtChaseAlgorithm_setupInitialCamDir(cLookAtChaseAlgorithm* self, float a, float b);
extern "C" void f_6C60_8508(void* self, long id) __asm__("cLookAtChaseAlgorithm_snap");

extern "C" void cLazyChaseAlgorithm_snap_v(char* self, long id) __asm__("cLazyChaseAlgorithm_snap");

/**
 * Snaps the lazy chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match puts Position at the rider
 * position (target slot 1) + Z * 100 - front (slot 4) * 200, then runs the look-at chase snap,
 * setupInitialCamDir(100, 100) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cLazyChaseAlgorithm_snap_v(char* vself, long id)
{
    cLazyChaseAlgorithm* self = (cLazyChaseAlgorithm*)vself;
    char* obj = (char*)&self->Target->Rider->mVtbl;
    sVEntry<int(void*)>* vt = *(sVEntry<int(void*)>**)obj;
    if (id == vt[7].fn(vthis(obj, vt[7])))
    {
        t4Vector nv = cmSub_168150(cmAdd_001607E0_8508(cmGet_001607E0_8508(((char*)self), 1), cmScaleP_1848(&t4vect_ZAxis_8508, 100.0f)), cmScale_001607E0_8508(cmGet_001607E0_8508(((char*)self), 4), 200.0f));
        self->Position = nv;
        f_6C60_8508(self, id);
        cLookAtChaseAlgorithm_setupInitialCamDir((cLookAtChaseAlgorithm*)self, 100.0f, 100.0f);
        sVEntry<void(void*)>* vt2 = (sVEntry<void (void*)>*)self->mVtbl;
        vt2[5].fn(vthis(self, vt2[5]));
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookBackChaseAlgorithm_cLookBackChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(cLookAtChaseAlgorithm* self);
extern void* cLookBackChaseAlgorithm_vtbl[];

/**
 * Constructs the look-back chase camera algorithm.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 0x20 (On Tour CAMERA_ALGORITHM_CHASE_LOOKBACK = 32) and
 * cLookBackChaseAlgorithm_vtbl.
 *
 * @return @p self
 */
extern "C" void* cLookBackChaseAlgorithm_cLookBackChaseAlgorithm(cLookBackChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(((cLookAtChaseAlgorithm*)self));
    self->AlgorithmType = 0x20;
    self->mVtbl = cLookBackChaseAlgorithm_vtbl;
    return ((void*)self);
}
#endif

extern void* cLookBackChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/camera", cLookBackChaseAlgorithm__cLookBackChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a look-back chase camera algorithm: stores cLookBackChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cLookBackChaseAlgorithm__cLookBackChaseAlgorithm(void* vself)
{
    cLookBackChaseAlgorithm* self = (cLookBackChaseAlgorithm*)vself;
    self->mVtbl = (void**)((int)(void*)cLookBackChaseAlgorithm_vtbl);
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookBackChaseAlgorithm_getRestDrop__FPv);
#ifdef SKIP_ASM
/**
 * Returns the look-back chase camera's rest drop (vtable slot 13, On Tour getRestDrop): always 0.
 *
 * @return 0
 */
float cLookBackChaseAlgorithm_getRestDrop(void* self)
{
    return 0.0f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookBackChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the look-back chase camera: only clears DoPitchBlending (+0x10), so
 * doRaceCamFinalPhase skips pitch blending.
 *
 * @param other  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
void cLookBackChaseAlgorithm_getRaceCamFinalPhaseData(void* self, void* other)
{
    *(int*)((char*)other + 0x10) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", cLookBackChaseAlgorithm_update);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_setupCamDirection(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateFollowDistance(void* self, void* info, float a, float b, float c, float d, float e, float f, float g);
extern "C" void cLookAtChaseAlgorithm_updateDropWithSpeed(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateCamTargetLate(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateSlowSpeedCam(cLookAtChaseAlgorithm* self, cChaseSetup* info, float a, float b, float c);
extern "C" void cLookAtChaseAlgorithm_updateLateralShift(cLookAtChaseAlgorithm* self, void* info, float a, float b);


struct sVEv_1686D0 {
    short delta;
    short index;
    t4Vector (*fn)(void*);
};

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_1686D0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_1686D0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_1686D0(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (in-place v *= s).
static inline void ScaleEq_1686D0(t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_1686D0(const t4Vector& v)
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
static inline float Dot_1686D0(const t4Vector& a, const t4Vector& b)
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

static inline t4Vector Get_1686D0(char* self, int slot)
{
    char* o = *(char**)(self + 0x30);
    sVEv_1686D0* e = &(*(sVEv_1686D0**)o)[slot];
    return e->fn(vthis(o, *e));
}

/**
 * Runs one frame of the look-back camera: builds a normal chase pose, then moves the eye past the rider to look back
 * at them.
 *
 * Runs setupChaseData (cLookAtChaseAlgorithm_setupChaseData), setupHalfPipeCam, updateCamTarget, setupCamDirection, updateFollowDistance
 * (193.2 base, no speed or boost terms), updateDropWithSpeed (rise 1.24), updateFOVWithSpeed (0.993 * pi/4),
 * updateCamTargetLate, updateSlowSpeedCam (15 km/h, 1.5 s each way) and updateLateralShift (0.135, 1.77). Then
 * Position moves along the look line to 294.9 units beyond the look-at point, and CamTargetPos becomes the rider
 * position (target slot 1) projected onto that line, behind the new eye.
 */
extern "C" void cLookBackChaseAlgorithm_update(cLookBackChaseAlgorithm* self)
{
    cChaseSetup info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f, 0.8513929843902588f,
                  0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f, 0.6000000238418579f);
    float zero = 0.0f;
    cLookAtChaseAlgorithm_setupHalfPipeCam(((cLookAtChaseAlgorithm*)self), (cChaseSetup*)((char*)&info));
    cLookAtChaseAlgorithm_updateCamTarget(((cLookAtChaseAlgorithm*)self), &info);
    cLookAtChaseAlgorithm_setupCamDirection(((char*)self), &info);
    cLookAtChaseAlgorithm_updateFollowDistance(((char*)self), &info, 193.1998291015625f, zero, 76.68663024902344f, zero, 1.7668397426605225f,
                  0.9239780306816101f, 0.907414972782135f);
    cLookAtChaseAlgorithm_updateDropWithSpeed(((char*)self), &info, zero, 1.2432044744491577f, zero, zero, 0.9783917665481567f);
    cLookAtChaseAlgorithm_updateFOVWithSpeed(((cLookAtChaseAlgorithm*)self), &info, zero, 0.9927757978439331f, 1.0f, 0.949999988079071f);
    cLookAtChaseAlgorithm_updateCamTargetLate(((char*)self), &info, 0.721226155757904f, 0.01082259975373745f, 0.22186216711997986f, 0.5425665378570557f,
                  0.9241908192634583f);
    cLookAtChaseAlgorithm_updateSlowSpeedCam(((cLookAtChaseAlgorithm*)self), &info, 15.0f, 1.5f, 1.5f);
    cLookAtChaseAlgorithm_updateLateralShift(((cLookAtChaseAlgorithm*)self), &info, 0.13458502292633057f, 1.7663990259170532f);

    t4Vector look = Sub_1686D0(*(&self->CamTargetPos), *(&self->Position));
    float len = Length_1686D0(look);
    ScaleEq_1686D0(look, (len + 294.9358825683594f) / len);
    t4Vector pos = Add_1686D0(*(&self->Position), look);
    t4Vector off = Sub_1686D0(Get_1686D0(((char*)self), 1), pos);
    float alongLook = Dot_1686D0(off, look) / Dot_1686D0(look, look);
    t4Vector np = Add_1686D0(Scale_1686D0(look, alongLook), pos);
    *(&self->CamTargetPos) = np;
    *(&self->Position) = pos;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cLookBackChaseAlgorithm_snap);
#ifdef SKIP_ASM
class cCamTargetK8940 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual int getAIIndex();
};
class cCamCtrlK8940 {
public:
    char pad_0x00[0x10];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void update();
};

/**
 * Snaps the look-back chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(559.744, 300) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cLookBackChaseAlgorithm_snap(cLookBackChaseAlgorithm* self, int id)
{
    cRider* rider = self->Target->Rider;
    if (id == ((cCamTargetK8940*)(&rider->mVtbl))->getAIIndex()) {
        cLookAtChaseAlgorithm_snap(((cCamCtrlK8940*)self), id);
        cLookAtChaseAlgorithm_setupInitialCamDir(((cLookAtChaseAlgorithm*)self), 559.7440185546875f, 300.0f);
        ((cCamCtrlK8940*)self)->update();
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMidRightChaseAlgorithm_cMidRightChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(cLookAtChaseAlgorithm* self);
extern void* cMidRightChaseAlgorithm_vtbl[];

/**
 * Constructs the mid-right chase camera algorithm.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 9 (On Tour CAMERA_ALGORITHM_CHASE_MID_RIGHT) and
 * cMidRightChaseAlgorithm_vtbl.
 *
 * @return @p self
 */
extern "C" void* cMidRightChaseAlgorithm_cMidRightChaseAlgorithm(cMidRightChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(((cLookAtChaseAlgorithm*)self));
    self->AlgorithmType = 0x9;
    self->mVtbl = cMidRightChaseAlgorithm_vtbl;
    return ((void*)self);
}
#endif

extern void* cMidRightChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/camera", cMidRightChaseAlgorithm__cMidRightChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a mid-right chase camera algorithm: stores cMidRightChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cMidRightChaseAlgorithm__cMidRightChaseAlgorithm(void* vself)
{
    cMidRightChaseAlgorithm* self = (cMidRightChaseAlgorithm*)vself;
    self->mVtbl = (void**)((int)(void*)cMidRightChaseAlgorithm_vtbl);
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMidRightChaseAlgorithm_getRestDrop);
#ifdef SKIP_ASM
/**
 * Returns the mid-right chase camera's rest drop (vtable slot 13, On Tour getRestDrop): 61.688.
 *
 * @return 61.688
 */
extern "C" float cMidRightChaseAlgorithm_getRestDrop(cMidRightChaseAlgorithm* self)
{
    return 61.68796157836914f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMidRightChaseAlgorithm_getRaceCamFinalPhaseData);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the mid-right chase camera, turning pitch blending on.
 *
 * PitchBlendingMaxAngleUp 16.14 and MaxAngleDown 6.65 (degrees), SmoothnessUp 0.926 and SmoothnessDown 0.843,
 * DoPitchBlending (+0x10) 1.
 *
 * @param out  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
extern "C" void cMidRightChaseAlgorithm_getRaceCamFinalPhaseData(cMidRightChaseAlgorithm* self, cRaceCamFinalPhaseData* out)
{
    out->PitchBlendingMaxAngleDown = 6.654887676239014f;
    out->PitchBlendingMaxAngleUp = 16.140756607055664f;
    out->PitchBlendingSmoothnessDown = 0.8425687551498413f;
    out->PitchBlendingSmoothnessUp = 0.9264262914657593f;
    out->DoPitchBlending = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", cMidRightChaseAlgorithm_update);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_setupCamDirection(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateFollowDistance(void* self, void* info, float a, float b, float c, float d, float e, float f, float g);
extern "C" void cLookAtChaseAlgorithm_updateDropWithSpeed(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateCamTargetLate(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateAirCam(cLookAtChaseAlgorithm* self, cChaseSetup* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateHalfPipeCam(cLookAtChaseAlgorithm* self, cChaseSetup* info, float a);
extern "C" void cLookAtChaseAlgorithm_updateRailCam(char* self, char* p, float k, float t);
extern "C" void cLookAtChaseAlgorithm_updateViewCam(char* self, char* p, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateSlowSpeedCam(cLookAtChaseAlgorithm* self, cChaseSetup* info, float a, float b, float c);
extern "C" void cLookAtChaseAlgorithm_updateLateralShift(cLookAtChaseAlgorithm* self, void* info, float a, float b);
// PORT: cLookAtChaseAlgorithm_updateHandplant is defined with 7 float parameters; this caller passes 8.
void cLookAtChaseAlgorithm_updateHandplant_8(char* self, char* trig, float a, float b, float c, float d, float e, float f, float g,
                     float h) __asm__("cLookAtChaseAlgorithm_updateHandplant");

/**
 * Runs one frame of the mid-right chase camera: the full look-at chase pipeline with the rider shifted off centre.
 *
 * Runs setupChaseData (cLookAtChaseAlgorithm_setupChaseData), setupHandplantBlend, setupHalfPipeCam, updateCamTarget, setupCamDirection,
 * updateFollowDistance (555.1 stopped, boost 307.1), updateDropWithSpeed, updateFOVWithSpeed (0.857 * pi/4),
 * updateHandplant, updateCamTargetLate, updateAirCam, updateHalfPipeCam (1.53 s), updateRailCam (rise 200.3),
 * updateViewCam (600, 300, 100), updateSlowSpeedCam (15 km/h) and updateLateralShift (-0.294, 0.652).
 * cMidLeftChaseAlgorithm_update is the same with a +0.294 shift.
 */
extern "C" void cMidRightChaseAlgorithm_update(cMidRightChaseAlgorithm* self)
{
    cChaseSetup info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f, 0.8513929843902588f,
                  0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f, 0.6000000238418579f);
    float zero = 0.0f;
    float k = 0.9800000190734863f;
    cLookAtChaseAlgorithm_setupHandplantBlend(((cLookAtChaseAlgorithm*)self), &info);
    cLookAtChaseAlgorithm_setupHalfPipeCam(((cLookAtChaseAlgorithm*)self), (cChaseSetup*)((char*)&info));
    cLookAtChaseAlgorithm_updateCamTarget(((cLookAtChaseAlgorithm*)self), &info);
    cLookAtChaseAlgorithm_setupCamDirection(((char*)self), &info);
    cLookAtChaseAlgorithm_updateFollowDistance(((char*)self), &info, 555.0844116210938f, 11.549837112426758f, 76.68663024902344f, 307.0549621582031f,
                  1.7668397426605225f, 0.9239780306816101f, 0.907414972782135f);
    cLookAtChaseAlgorithm_updateDropWithSpeed(((char*)self), &info, 12.331472396850586f, 84.0909652709961f, 38.64322280883789f, 93.62934875488281f,
                  0.9783917665481567f);
    cLookAtChaseAlgorithm_updateFOVWithSpeed(((cLookAtChaseAlgorithm*)self), &info, zero, 0.8567028641700745f, 1.0f, 0.949999988079071f);
    cLookAtChaseAlgorithm_updateHandplant_8(((char*)self), (char*)&info, 348.64111328125f, 66.33821868896484f, 162.2997283935547f, 2.00368070602417f,
                    zero, k, 0.9599999785423279f, 0.4552607834339142f);
    cLookAtChaseAlgorithm_updateCamTargetLate(((char*)self), &info, -8.730510711669922f, 64.15198516845703f, 58.114906311035156f, 5.931424617767334f,
                  0.9241908192634583f);
    cLookAtChaseAlgorithm_updateAirCam(((cLookAtChaseAlgorithm*)self), &info, 1.878627061843872f, 2.494720935821533f, 0.34134337306022644f, 0.8672914505004883f);
    cLookAtChaseAlgorithm_updateHalfPipeCam(((cLookAtChaseAlgorithm*)self), &info, 1.5269116163253784f);
    cLookAtChaseAlgorithm_updateRailCam(((char*)self), (char*)&info, 200.33114624023438f, 0.9706981778144836f);
    cLookAtChaseAlgorithm_updateViewCam(((char*)self), (char*)&info, 600.0f, 300.0f, 100.0f, k);
    cLookAtChaseAlgorithm_updateSlowSpeedCam(((cLookAtChaseAlgorithm*)self), &info, 15.0f, 1.5f, 1.5f);
    cLookAtChaseAlgorithm_updateLateralShift(((cLookAtChaseAlgorithm*)self), &info, -0.2944446802139282f, 0.6521888375282288f);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMidRightChaseAlgorithm_snap);
#ifdef SKIP_ASM
/**
 * Snaps the mid-right chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(559.744, 300) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cMidRightChaseAlgorithm_snap(cMidRightChaseAlgorithm* self, int id)
{
    cRider* rider = self->Target->Rider;
    if (id == ((cCamTargetK8940*)(&rider->mVtbl))->getAIIndex()) {
        cLookAtChaseAlgorithm_snap(((cCamCtrlK8940*)self), id);
        cLookAtChaseAlgorithm_setupInitialCamDir(((cLookAtChaseAlgorithm*)self), 559.7440185546875f, 300.0f);
        ((cCamCtrlK8940*)self)->update();
    }
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm);
#ifdef SKIP_ASM
extern "C" void* cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(cLookAtChaseAlgorithm* self);
extern void* cMidLeftChaseAlgorithm_vtbl[];

/**
 * Constructs the mid-left chase camera algorithm.
 *
 * Runs the look-at chase constructor, then sets AlgorithmType 9 and cMidLeftChaseAlgorithm_vtbl. 9 is the mid-right
 * camera's type (On Tour CAMERA_ALGORITHM_CHASE_MID_RIGHT); On Tour's CHASE_MID_LEFT is 10, so retail tags both mid
 * cameras alike.
 *
 * @return @p self
 */
extern "C" void* cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm(cMidLeftChaseAlgorithm* self)
{
    cLookAtChaseAlgorithm_cLookAtChaseAlgorithm(((cLookAtChaseAlgorithm*)self));
    self->AlgorithmType = 0x9;
    self->mVtbl = cMidLeftChaseAlgorithm_vtbl;
    return ((void*)self);
}
#endif

extern void* cMidLeftChaseAlgorithm_vtbl[];
extern "C" void* cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(void*);

//100%
INCLUDE_ASM("camera/camera", cMidLeftChaseAlgorithm__cMidLeftChaseAlgorithm__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a mid-left chase camera algorithm: stores cMidLeftChaseAlgorithm_vtbl and chains to the look-at chase
 * destructor.
 *
 * Declared with only the this pointer: the caller's destructor flags stay untouched in $a1 and so reach
 * cLookAtChaseAlgorithm__cLookAtChaseAlgorithm, which frees the object when bit 0 is set.
 *
 * @return the base destructor's result (declared void*)
 */
void* cMidLeftChaseAlgorithm__cMidLeftChaseAlgorithm(void* vself)
{
    cMidLeftChaseAlgorithm* self = (cMidLeftChaseAlgorithm*)vself;
    self->mVtbl = (void**)((int)(void*)cMidLeftChaseAlgorithm_vtbl);
    return cLookAtChaseAlgorithm__cLookAtChaseAlgorithm(self);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMidLeftChaseAlgorithm_getRestDrop);
#ifdef SKIP_ASM
/**
 * Returns the mid-left chase camera's rest drop (vtable slot 13, On Tour getRestDrop): 61.688.
 *
 * @return 61.688
 */
extern "C" float cMidLeftChaseAlgorithm_getRestDrop(cMidLeftChaseAlgorithm* self)
{
    return 61.68796157836914f;
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMidLeftChaseAlgorithm_getRaceCamFinalPhaseData);
#ifdef SKIP_ASM
/**
 * Fills the race-camera final-phase data for the mid-left chase camera, turning pitch blending on.
 *
 * PitchBlendingMaxAngleUp 16.14 and MaxAngleDown 6.65 (degrees), SmoothnessUp 0.926 and SmoothnessDown 0.843,
 * DoPitchBlending (+0x10) 1.
 *
 * @param out  the cRaceCamFinalPhaseData to fill (On Tour data_outArg)
 */
extern "C" void cMidLeftChaseAlgorithm_getRaceCamFinalPhaseData(cMidLeftChaseAlgorithm* self, cRaceCamFinalPhaseData* out)
{
    out->PitchBlendingMaxAngleDown = 6.654887676239014f;
    out->PitchBlendingMaxAngleUp = 16.140756607055664f;
    out->PitchBlendingSmoothnessDown = 0.8425687551498413f;
    out->PitchBlendingSmoothnessUp = 0.9264262914657593f;
    out->DoPitchBlending = 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("camera/camera", cMidLeftChaseAlgorithm_update);
#ifdef SKIP_ASM
extern "C" void cLookAtChaseAlgorithm_setupCamDirection(void* self, void* info);
extern "C" void cLookAtChaseAlgorithm_updateFollowDistance(void* self, void* info, float a, float b, float c, float d, float e, float f, float g);
extern "C" void cLookAtChaseAlgorithm_updateDropWithSpeed(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateCamTargetLate(void* self, void* info, float a, float b, float c, float d, float e);
extern "C" void cLookAtChaseAlgorithm_updateAirCam(cLookAtChaseAlgorithm* self, cChaseSetup* info, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateHalfPipeCam(cLookAtChaseAlgorithm* self, cChaseSetup* info, float a);
extern "C" void cLookAtChaseAlgorithm_updateRailCam(char* self, char* p, float k, float t);
extern "C" void cLookAtChaseAlgorithm_updateViewCam(char* self, char* p, float a, float b, float c, float d);
extern "C" void cLookAtChaseAlgorithm_updateSlowSpeedCam(cLookAtChaseAlgorithm* self, cChaseSetup* info, float a, float b, float c);
extern "C" void cLookAtChaseAlgorithm_updateLateralShift(cLookAtChaseAlgorithm* self, void* info, float a, float b);
// PORT: cLookAtChaseAlgorithm_updateHandplant is defined with 7 float parameters; this caller passes 8.
void cLookAtChaseAlgorithm_updateHandplant_8(char* self, char* trig, float a, float b, float c, float d, float e, float f, float g,
                     float h) __asm__("cLookAtChaseAlgorithm_updateHandplant");

/**
 * Runs one frame of the mid-left chase camera: the full look-at chase pipeline with the rider shifted off centre.
 *
 * The same calls and constants as cMidRightChaseAlgorithm_update (setupChaseData, the setup steps, follow distance
 * 555.1, drop, FOV 0.857 * pi/4, handplant, late target, air, half-pipe, rail, view and slow-speed cams), except that
 * updateLateralShift gets +0.294 instead of -0.294, putting the eye on the other side.
 */
extern "C" void cMidLeftChaseAlgorithm_update(cMidLeftChaseAlgorithm* self)
{
    cChaseSetup info;
    cLookAtChaseAlgorithm_setupChaseData(((char*)self), &info, 0.2713613510131836f, 0.8999999761581421f, 0.29420721530914307f, 0.8513929843902588f,
                  0.8500000238418579f, 0.9700000286102295f, 0.10000000149011612f, 0.6000000238418579f);
    float zero = 0.0f;
    float k = 0.9800000190734863f;
    cLookAtChaseAlgorithm_setupHandplantBlend(((cLookAtChaseAlgorithm*)self), &info);
    cLookAtChaseAlgorithm_setupHalfPipeCam(((cLookAtChaseAlgorithm*)self), (cChaseSetup*)((char*)&info));
    cLookAtChaseAlgorithm_updateCamTarget(((cLookAtChaseAlgorithm*)self), &info);
    cLookAtChaseAlgorithm_setupCamDirection(((char*)self), &info);
    cLookAtChaseAlgorithm_updateFollowDistance(((char*)self), &info, 555.0844116210938f, 11.549837112426758f, 76.68663024902344f, 307.0549621582031f,
                  1.7668397426605225f, 0.9239780306816101f, 0.907414972782135f);
    cLookAtChaseAlgorithm_updateDropWithSpeed(((char*)self), &info, 12.331472396850586f, 84.0909652709961f, 38.64322280883789f, 93.62934875488281f,
                  0.9783917665481567f);
    cLookAtChaseAlgorithm_updateFOVWithSpeed(((cLookAtChaseAlgorithm*)self), &info, zero, 0.8567028641700745f, 1.0f, 0.949999988079071f);
    cLookAtChaseAlgorithm_updateHandplant_8(((char*)self), (char*)&info, 348.64111328125f, 66.33821868896484f, 162.2997283935547f, 2.00368070602417f,
                    zero, k, 0.9599999785423279f, 0.4552607834339142f);
    cLookAtChaseAlgorithm_updateCamTargetLate(((char*)self), &info, -8.730510711669922f, 64.15198516845703f, 58.114906311035156f, 5.931424617767334f,
                  0.9241908192634583f);
    cLookAtChaseAlgorithm_updateAirCam(((cLookAtChaseAlgorithm*)self), &info, 1.878627061843872f, 2.494720935821533f, 0.34134337306022644f, 0.8672914505004883f);
    cLookAtChaseAlgorithm_updateHalfPipeCam(((cLookAtChaseAlgorithm*)self), &info, 1.5269116163253784f);
    cLookAtChaseAlgorithm_updateRailCam(((char*)self), (char*)&info, 200.33114624023438f, 0.9706981778144836f);
    cLookAtChaseAlgorithm_updateViewCam(((char*)self), (char*)&info, 600.0f, 300.0f, 100.0f, k);
    cLookAtChaseAlgorithm_updateSlowSpeedCam(((cLookAtChaseAlgorithm*)self), &info, 15.0f, 1.5f, 1.5f);
    cLookAtChaseAlgorithm_updateLateralShift(((cLookAtChaseAlgorithm*)self), &info, 0.2944446802139282f, 0.6521888375282288f);
}
#endif

//100%
INCLUDE_ASM("camera/camera", cMidLeftChaseAlgorithm_snap);
#ifdef SKIP_ASM
/**
 * Snaps the mid-left chase camera when @p id is the followed rider's subject id.
 *
 * Compares @p id with slot 7 of the rider's +0x6C0 interface (Target->Rider). On a match runs the look-at chase snap,
 * setupInitialCamDir(559.744, 300) and the algorithm's update (vtable slot 5).
 *
 * @param id  the subject id to snap to (On Tour subjectID); other ids are ignored
 */
extern "C" void cMidLeftChaseAlgorithm_snap(cMidLeftChaseAlgorithm* self, int id)
{
    cRider* rider = self->Target->Rider;
    if (id == ((cCamTargetK8940*)(&rider->mVtbl))->getAIIndex()) {
        cLookAtChaseAlgorithm_snap(((cCamCtrlK8940*)self), id);
        cLookAtChaseAlgorithm_setupInitialCamDir(((cLookAtChaseAlgorithm*)self), 559.7440185546875f, 300.0f);
        ((cCamCtrlK8940*)self)->update();
    }
}
#endif

