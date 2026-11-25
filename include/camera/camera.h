#ifndef CAMERA_CAMERA_H
#define CAMERA_CAMERA_H

// cSimpleCamera and cCamera (EA: d:/ssx/code/source/camera/camera.h and camera.cpp, from the July 2003 preview's
// assert strings; camera.h asserts "maxFar>MinNear" / "minNear<MaxFar" are cSimpleCamera's setters).
//
// cSimpleCamera (0xA0): a view (FOV, near/far planes and their limits), a pose and the camera matrix. Retail vtable
// cSimpleCamera_vtbl (D_0045B8B0), stored by its ctor cSimpleCamera_cSimpleCamera and dtor cSimpleCamera__cSimpleCamera. It is also
// the base of the front end's cFECamera (fe/festatecharequip.cpp constructs one at +0x10 of its state).
//
// cCamera (0x4B0): one rider's game camera: cSimpleCamera + three controllers (manual, chase, script; the current
// one drives it), the stream focus it owns, the result algorithm the controllers blend into, camera shake and ground
// avoidance. Created by cGameViewMan_cGameViewMan (main/gamerender.cpp) with cMemMan_alloc(0x4B0, "GameCamera") +
// cCamera_cCamera(cam, viewIndex), one per view, stored at viewman + 4 + 4 * i. Retail vtable cCamera_vtbl
// (D_0045B888; slots: deleting dtor cCamera__cCamera, init = cCamera_init, update = cCamera_update). Methods in
// camera/camera.cpp 0x15DC18-0x15F568 and accessors 0x168110-0x168138.
//
// FLAT layouts, like main/game.h: every field at its retail offset, bases written out inline, no C++ inheritance.
// Evidence tags:
//   [R] retail constructors cCamera_cCamera, cSimpleCamera_cSimpleCamera (cSimpleCamera ctor), cCamera_init, and the allocation;
//   [P] the preview's inline accessors via tools/layout_xlate.py (cSimpleCamera getFOV/getNear/getFar/setMaxFOV at
//       0x0/0x4/0x8/0xC; cCamera getCurrentController/get*Controller/isStreamReady, preview 0xC0..0xD4 = retail
//       0xA0..0xB4: the preview's cSimpleCamera was 0x20 bigger);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cSimpleCamera 0xA0, cCamera 0x490): same members in the same order. Retail's
//       ResultAlgorithm is 0x10 bigger (0x390) and its bools are words, so cCamera's members after 0xC0 sit
//       0x10..0x20 later than On Tour's.

#include "layout.h"
#include "bx/math3d.h"
#include "camera/cameraalgorithms.h"
#include "camera/race/directionchasealgo.h"    // cDEFAULT_3ChaseAlgorithm

struct cCameraController;
struct cManualCameraController;
struct cChaseCameraController;
struct cScriptCameraController;
struct cRider;

// cRiderCameraTarget (0x8): what the chase algorithms follow: a cCameraTarget (On Tour: 0x4, only the vptr) bound to a
// rider. Created by cCamera_init: operator new(8, "RiderCameraTarget") + vtable cRiderCameraTarget_vtbl (26 slots, all in
// camera/camera.cpp 0x15F568-0x15F938: getPosition, getRotation, getVelocity ... each reads Rider), then handed to
// the chase controller (cChaseCameraController_setTarget) and the result algorithm (cChaseAlgorithm_setTarget).
// NOTE: its t4Vector-returning slots (cRiderCameraTarget_getPosition .. cRiderCameraTarget_getLastGroundNormal, except cRiderCameraTarget_getLean) take the hidden return
// slot in a0 and `this` in a1.
// cCameraTarget (On Tour: 0x4, only the vptr): the interface. Its 26 virtuals, slots [R] cRiderCameraTarget_vtbl
// (no destructor: getPosition is slot 1), names, order and types [O] (On Tour class cRiderCameraTarget, whose
// cCameraTarget base declares them); eSurfaceType is an int here. Nothing builds a cCameraTarget in C++ (cCamera_init
// allocates with operator new and stores the vtable), so no vtable is emitted.
class cCameraTarget {
public:
    virtual t4Vector getPosition() const;                          // 1
    virtual tQuaternion getRotation() const;                       // 2
    virtual t4Vector getVelocity() const;                          // 3
    virtual t4Vector getFront() const;                             // 4
    virtual t4Vector getSide() const;                              // 5
    virtual t4Vector getUp() const;                                // 6
    virtual float getLean() const;                                 // 7
    virtual t4Vector getNormal() const;                            // 8
    virtual t4Vector getLastGroundNormal() const;                  // 9
    virtual const t4Vector& getHandplantSplineTangent() const;     // 10
    virtual const t4Vector& getHandplantTarget() const;            // 11
    virtual float getTotalAirTime() const;                         // 12
    virtual float getRemainingAirTime() const;                     // 13
    virtual const t4Vector& getApexPos() const;                    // 14
    virtual float getApexTime() const;                             // 15
    virtual const t4Vector& getHalfPipeNormal() const;             // 16
    virtual float getBoostWarp() const;                            // 17
    virtual float getCrouch() const;                               // 18
    virtual bool isAirPredictorDone() const;                       // 19
    virtual int getSurfaceType() const;                            // 20 (eSurfaceType)
    virtual float getVerticalJump() const;                         // 21
    virtual bool didPop() const;                                   // 22
    virtual float getLandingAngle() const;                         // 23
    virtual bool isRidingFlexRail() const;                         // 24
    virtual const t4Vector& getLandingNormal() const;              // 25
    virtual const t4Vector& getLandingVelocity() const;            // 26
};

CHECK_SIZE(cCameraTarget, 0x4);

struct cRiderCameraTarget : public cCameraTarget {
    // 0x0: the vptr: [R] cRiderCameraTarget_vtbl (cCamera_init)
    cRider* Rider;              // 0x4 [O]; [P] getRider (lw) / setRider (sw); [R] cCamera_init stores the view's rider;
                                //     [U] all 26 slots lw 0x4; cRiderCameraTarget_setRider setRider, cRiderCameraTarget_getRider getRider

    // Methods bound to their retail symbols by asm labels.
    void setRider(int rider) __asm__("cRiderCameraTarget_setRider");
    int getRider() __asm__("cRiderCameraTarget_getRider");
};

CHECK_OFFSET(cRiderCameraTarget, Rider, 0x4);
CHECK_SIZE(cRiderCameraTarget, 0x8);

struct tPose;

struct cSimpleCamera {
    float View;                 // 0x00 [O]; [P] getFOV (lwc1); [U] cSimpleCamera_getFOV getFOV, cSimpleCamera_setFOV setFOV
    float Near;                 // 0x04 [O]; [P] getNear; [U] cSimpleCamera_getNear
    float Far;                  // 0x08 [O]; [P] getFar; [U] cSimpleCamera_getFar
    float MaxView;              // 0x0C [O]; [P] setMaxFOV (swc1); [U] cSimpleCamera_setMaxFOV; cCamera_init stores its arg
    float MinNear;              // 0x10 [O]; [U] cSimpleCamera_setMinNear (swc1); cCamera_init
    float MaxFar;               // 0x14 [O]; [U] cSimpleCamera_setMaxFar (swc1); cCamera_init
    int ViewIndex;              // 0x18 [O]; [U] cCamera_init stores the view index; cCamera_updateCamMat reads it
    char unk_1C[0x4];           // 0x1C alignment gap before the 16-byte-aligned tPose
    tPose Pose;                 // 0x20 [O]; [R] ctor: P = 0, Q = (0, 1, 0, 1) on the stack, sq 0x20/0x30;
                                //      [U] cSimpleCamera_getPosition/cSimpleCamera_getPose return this + 0x20, cSimpleCamera_setPosition (sq 0x20),
                                //      cSimpleCamera_setOrientation (sq 0x30), cSimpleCamera_setPose (both)
    t4Matrix CamMat;            // 0x40 [O]; [U] cSimpleCamera_getCamMat returns this + 0x40; cSimpleCamera_setupCamWithPosAndTarget writes its rows
    t4Vector Direction;         // 0x80 [O]; [R] = t4vect_XAxis; [U] cSimpleCamera_getDirection returns this + 0x80
    void** mVtbl;               // 0x90 [R] cSimpleCamera_vtbl (D_0045B8B0)
    char unk_94[0xC];           // 0x94 padding to the 16-byte class alignment

    // Methods bound to their retail symbols by asm labels.
    float getNear() __asm__("cSimpleCamera_getNear");
    float getFar() __asm__("cSimpleCamera_getFar");
    void setPosition(long long *position) __asm__("cSimpleCamera_setPosition");
    void setOrientation(long long *orientation) __asm__("cSimpleCamera_setOrientation");
    void setPose(tPose* pose) __asm__("cSimpleCamera_setPose");
    void setMaxFar(float fparg0) __asm__("cSimpleCamera_setMaxFar");
    void setMinNear(float fparg0) __asm__("cSimpleCamera_setMinNear");
    void setMaxFOV(float fparg0) __asm__("cSimpleCamera_setMaxFOV");
    void init(float x, float y, float z) __asm__("cSimpleCamera_init");
    float getHeading() __asm__("cSimpleCamera_getHeading");
    float getPitch() __asm__("cSimpleCamera_getPitch");
};

CHECK_OFFSET(cSimpleCamera, View, 0x00);
CHECK_OFFSET(cSimpleCamera, Near, 0x04);
CHECK_OFFSET(cSimpleCamera, Far, 0x08);
CHECK_OFFSET(cSimpleCamera, MaxView, 0x0C);
CHECK_OFFSET(cSimpleCamera, MinNear, 0x10);
CHECK_OFFSET(cSimpleCamera, MaxFar, 0x14);
CHECK_OFFSET(cSimpleCamera, ViewIndex, 0x18);
CHECK_OFFSET(cSimpleCamera, Pose, 0x20);
CHECK_OFFSET(cSimpleCamera, CamMat, 0x40);
CHECK_OFFSET(cSimpleCamera, Direction, 0x80);
CHECK_OFFSET(cSimpleCamera, mVtbl, 0x90);
CHECK_SIZE(cSimpleCamera, 0xA0);

struct cCamera {
    // --- cSimpleCamera
    float View;                     // 0x000 [O]; [U] cCamera_updateCamMat
    float Near;                     // 0x004 [O]; [U] cCamera_updateCamMat
    float Far;                      // 0x008 [O]; [U] cCamera_updateCamMat, cCamera_updateStreamFocus
    float MaxView;                  // 0x00C [O]; [U] cCamera_init stores its arg
    float MinNear;                  // 0x010 [O]; [U] cCamera_init
    float MaxFar;                   // 0x014 [O]; [U] cCamera_init
    int ViewIndex;                  // 0x018 [O]; [U] cCamera_init stores the view index
    char unk_01C[0x4];              // 0x01C alignment gap
    tPose Pose;                     // 0x020 [O]; [U] cSimpleCamera_setupCamWithPosAndTarget, cCamera_updateCamMat (sq, .x/.y/.z by lwc1)
    t4Matrix CamMat;                // 0x040 [O]; [U] cSimpleCamera_setupCamWithPosAndTarget (setupCamWithPosAndTarget, which inlines the CamMat rebuild: sqc2 rows)
    t4Vector Direction;             // 0x080 [O]; [U] cSimpleCamera_setupCamWithPosAndTarget, cCamera_updateCamMat (sq)
    void** mVtbl;                   // 0x090 [R] cCamera_vtbl (D_0045B888); the dtor restores cSimpleCamera_vtbl
    char unk_094[0xC];              // 0x094 padding
    // --- cCamera
    cCameraController* Controller;  // 0x0A0 [O]; [P] getCurrentController; [U] cCamera_setCurrentController
                                    //       copies one of the three below, cCamera_update calls its slot 2
    cManualCameraController* ManualController; // 0x0A4 [O]; [P] getManualController; [R] "Manual Cam Controller" (0x20)
    cChaseCameraController* ChaseController;   // 0x0A8 [O]; [P] getChaseController; [R] "Chase Cam Controller" (0x44)
    cScriptCameraController* ScriptController; // 0x0AC [O]; [P] getScriptController; [R] "Script Cam Controller" (0x20)
    int StreamFocusID;              // 0x0B0 [O] eStreamFocusID; [R] -1, then cCamera_init: 0 for view 0, 1 for view 1;
                                    //       [U] cCamera_updateStreamFocus, cCamera_getStreamFocusID getStreamFocusID
    int bStreamIsReady;             // 0x0B4 [O] bool (a word in retail); [P] isStreamReady (lw); [R] 0;
                                    //       [U] cCamera_updateStreamFocus sets it, cCamera_isStreamReady returns it
    char unk_0B8[0x8];              // 0x0B8 alignment gap
    cDEFAULT_3ChaseAlgorithm ResultAlgorithm; // 0x0C0 [O] cDEFAULT_3ChaseAlgorithm (no fields of its own); [R] constructed
                                    //       by cDEFAULT_3ChaseAlgorithm_cDEFAULT_3ChaseAlgorithm, destroyed by cDEFAULT_3ChaseAlgorithm__cDEFAULT_3ChaseAlgorithm__FPv; [U] cCamera_init sets its
                                    //       Target (cChaseAlgorithm_setTarget), cCamera_snapResultAlgorithm snaps it (cLookAtChaseAlgorithm_snapCurrent)
    int DominantCamShakeType;       // 0x450 [O] eCamShakeType; [R] 0; [U] cCamera_shakeCamera sw,
                                    //       cCamera_applyCameraShake lw
    float MaxShakeVariance;         // 0x454 [O]; [R] 0; [U] cCamera_shakeCamera lwc1/swc1, cCamera_applyCameraShake lwc1
    int DoingCamShake;              // 0x458 [O] bool; [R] 0; [U] cCamera_shakeCamera sw, cCamera_applyCameraShake lw/sw
    int DoingCamShakeLastFrame;     // 0x45C [O] bool; [R] 0; [U] cCamera_applyCameraShake lw/sw
    float LastGroundMove;           // 0x460 [O]; [R] 0; [U] cCamera_avoidGround lwc1/swc1
    char unk_464[0xC];              // 0x464 alignment gap
    t4Vector LastGroundNormal;      // 0x470 [O]; [R] = t4vect_ZAxis; [U] cCamera_avoidGround
    char IntersectionHint[0x10];    // 0x480 [O] tIntHint (intersect: mPatch, mU/mV, mFirstTri, mPrecision); only its
                                    //       address is used (cCamera_avoidGround)
    t4Vector LastSafePosition;      // 0x490 [O]; [R] = t4vect_Origin; [U] cCamera_avoidGround
    int UsingSafePositionLastFrame; // 0x4A0 [O] bool; [R] 0; [U] cCamera_avoidGround
    int LastSafePositionInvalid;    // 0x4A4 [O] bool; [R] 1; [U] cCamera_avoidGround
    char unk_4A8[0x8];              // 0x4A8 padding to the 16-byte class alignment

    // Methods bound to their retail symbols by asm labels.
    void update() __asm__("cCamera_update");
    void setCurrentController(int mode) __asm__("cCamera_setCurrentController");
    void* snapResultAlgorithm() __asm__("cCamera_snapResultAlgorithm__FPv");
    void shakeCamera(int type, float amount, float dist) __asm__("cCamera_shakeCamera");
    void applyCameraShake() __asm__("cCamera_applyCameraShake");
    void updateStreamFocus() __asm__("cCamera_updateStreamFocus");
    void switchChaseControllerTargetRider(int value) __asm__("cCamera_switchChaseControllerTargetRider");
    int getCurrentController() __asm__("cCamera_getCurrentController");
    int getManualController() __asm__("cCamera_getManualController");
    int getChaseController() __asm__("cCamera_getChaseController");
    int getScriptController() __asm__("cCamera_getScriptController");
    int isStreamReady() __asm__("cCamera_isStreamReady");
    int getStreamFocusID() __asm__("cCamera_getStreamFocusID");
};

CHECK_OFFSET(cCamera, View, 0x000);
CHECK_OFFSET(cCamera, Near, 0x004);
CHECK_OFFSET(cCamera, Far, 0x008);
CHECK_OFFSET(cCamera, MaxView, 0x00C);
CHECK_OFFSET(cCamera, MinNear, 0x010);
CHECK_OFFSET(cCamera, MaxFar, 0x014);
CHECK_OFFSET(cCamera, ViewIndex, 0x018);
CHECK_OFFSET(cCamera, Pose, 0x020);
CHECK_OFFSET(cCamera, CamMat, 0x040);
CHECK_OFFSET(cCamera, Direction, 0x080);
CHECK_OFFSET(cCamera, mVtbl, 0x090);
CHECK_OFFSET(cCamera, Controller, 0x0A0);
CHECK_OFFSET(cCamera, ManualController, 0x0A4);
CHECK_OFFSET(cCamera, ChaseController, 0x0A8);
CHECK_OFFSET(cCamera, ScriptController, 0x0AC);
CHECK_OFFSET(cCamera, StreamFocusID, 0x0B0);
CHECK_OFFSET(cCamera, bStreamIsReady, 0x0B4);
CHECK_OFFSET(cCamera, ResultAlgorithm, 0x0C0);
CHECK_OFFSET(cCamera, DominantCamShakeType, 0x450);
CHECK_OFFSET(cCamera, MaxShakeVariance, 0x454);
CHECK_OFFSET(cCamera, DoingCamShake, 0x458);
CHECK_OFFSET(cCamera, DoingCamShakeLastFrame, 0x45C);
CHECK_OFFSET(cCamera, LastGroundMove, 0x460);
CHECK_OFFSET(cCamera, LastGroundNormal, 0x470);
CHECK_OFFSET(cCamera, IntersectionHint, 0x480);
CHECK_OFFSET(cCamera, LastSafePosition, 0x490);
CHECK_OFFSET(cCamera, UsingSafePositionLastFrame, 0x4A0);
CHECK_OFFSET(cCamera, LastSafePositionInvalid, 0x4A4);
CHECK_SIZE(cCamera, 0x4B0);

#endif /* CAMERA_CAMERA_H */
