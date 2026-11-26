#ifndef CAMERA_RACE_RACECONTROLLER_H
#define CAMERA_RACE_RACECONTROLLER_H

// cChaseCameraController: the camera controller that drives a rider's chase camera in a race. It picks the chase
// algorithm (player choice, camera-trigger volumes, replay, slow-speed camera) and blends it into its algorithm list.
// EA's source is d:/ssx/code/source/camera/race/racecontroller.cpp (the July 2003 preview's asserts there test
// this->CamSwitchRecord.IsInReplay / IsSlow / ...); its methods are in retail's camera/camera.cpp, 0x161950-0x1622B0.
// Created by cCamera_cCamera: cMemMan_alloc(0x44, "Chase Cam Controller") +
// cChaseCameraController_cChaseCameraController(p, viewIndex, camera). Retail vtable cChaseCameraController_vtbl
// (D_0045B908: deleting dtor cChaseCameraController__cChaseCameraController, update cChaseCameraController_update,
// then cCameraController's cCameraController_snap / cCameraController_snapCurrent).
// Base class cCameraController (0x18; camera/cameracontroller.cpp: cCameraController_cCameraController), written out
// inline; cManualCameraController and cScriptCameraController (0x20 each) derive from it too.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance. Evidence tags:
//   [R] retail ctors cCameraController_cCameraController and cChaseCameraController_cChaseCameraController,
//       cChaseCameraController_switchCamera_HardReset (resets the switch state), the allocation size;
//   [P] the preview's inline accessors via tools/layout_xlate.py (getLatestAlgoType lw 0x18, getTarget lw 0x20;
//       vptr at 0x14 in both builds);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cCameraController 0x18, cChaseCameraController 0x44, tCameraSwitchRecord 0x14): same
//       members at the same offsets.

#include "layout.h"

struct cCameraAlgoList;
struct cInput;
struct cCamera;
struct cRiderCameraTarget;
struct cRider;
struct cCTAction;

// Why and how the chase camera last switched algorithm (On Tour: tCameraSwitchRecord, 0x14). Algorithm types are
// eCamAlgoType values (retail: 0x3D = none, 0x42 = the slow-speed camera, 0x4C = "the player's choice", 0x5D = replay).
struct tCameraSwitchRecord {
    int PlayerChoiceType;           // 0x00 [O]; [R] HardReset: persistentPlayerChoiceType[view]; [U] cChaseCameraController_switchCamera_UserChange
                                    //      (setPlayerChoice) stores it and the persistent copy
    int VolumeType;                 // 0x04 [O]; [R] HardReset: 0x3D; [U] cChaseCameraController_switchCamera_Volume stores the trigger's type
    int LatestReplayType;           // 0x08 [O]; [R] HardReset: 0x3D; [U] cChaseCameraController_switchCamera_ReplayToggle stores the replay mode
    unsigned int IsInVolume : 1;    // 0x0C bit 0 [O]; [U] cChaseCameraController_switchCamera_Volume sets it with a volume trigger
    unsigned int IsInReplay : 1;    // 0x0C bit 1 [O]; [U] cChaseCameraController_switchCamera_ReplayToggle sets it (`|= 2`), cChaseCameraController_switchCamera_EndReplay clears it
    unsigned int IsSlow : 1;        // 0x0C bit 2 [O]; [U] cChaseCameraController_switchCamera_Slow sets it, cChaseCameraController_switchCamera_EndSlow clears it
    cCTAction* TriggerAction;       // 0x10 [O]; [R] HardReset: 0; [U] cChaseCameraController_switchCamera_Volume stores the trigger's action
};

CHECK_OFFSET(tCameraSwitchRecord, PlayerChoiceType, 0x00);
CHECK_OFFSET(tCameraSwitchRecord, VolumeType, 0x04);
CHECK_OFFSET(tCameraSwitchRecord, LatestReplayType, 0x08);
CHECK_OFFSET(tCameraSwitchRecord, TriggerAction, 0x10);
CHECK_SIZE(tCameraSwitchRecord, 0x14);

struct cCameraAlgorithm;

struct cChaseCameraController {
    // --- cCameraController (On Tour: 0x18)
    int Type;                       // 0x00 [O] eCamControllerType; [R] base ctor 0, then 2 (chase)
    int BlendType;                  // 0x04 [O] eCamBlendType; [R] base ctor 0, then 1
    cCameraAlgoList* AlgorithmList; // 0x08 [O]; [R] base ctor: cMemMan_alloc(8, "Algorithm List") + cCameraAlgoList ctor;
                                    //      [U] update and cChaseCameraController_addAlgorithm (setAlgorithm) walk / insert into it
    cInput* Input;                  // 0x0C [O]; [R] base ctor 0; [U] cCamera_init stores SSXApp's input of the view
    cCamera* Camera;                // 0x10 [O]; [R] the ctor's camera argument (cCamera_cCamera passes itself)
    void** mVtbl;                   // 0x14 [R] cChaseCameraController_vtbl; [P] preview vptr at 0x14
    // --- cChaseCameraController
    int LatestAlgoType;             // 0x18 [O] eCamAlgoType; [P] getLatestAlgoType (lw); [U] cChaseCameraController_addAlgorithm copies the
                                    //      new algorithm's AlgorithmType; cChaseCameraController_getLatestAlgoType returns it
    int PreferredChaseAlgoLastFrame; // 0x1C [O] eCamAlgoType; [R] HardReset: 0x3D; [U] cChaseCameraController_processInput
    cRiderCameraTarget* Target;     // 0x20 [O]; [P] getTarget (lw); [R] ctor 0; [U] cChaseCameraController_setTarget deletes the
                                    //      old one; the dtor deletes it; cChaseCameraController_getTarget returns it
    tCameraSwitchRecord CamSwitchRecord; // 0x24 [O]; [P] the preview asserts on this->CamSwitchRecord.*
    float TimeSinceLastSuggestedVolumeTrigger; // 0x38 [O]; [R] HardReset: 0; [U] update (lwc1/swc1)
    int GoingSlow;                  // 0x3C [O] bool (a word in retail); [R] HardReset: 0; [U] cChaseCameraController_processInput
    int NumFramesSlowGround;        // 0x40 [O]; [R] HardReset: 0; [U] cChaseCameraController_switchCamera_SoftReset

    // Methods bound to their retail symbols by asm labels.
    void switchCamera_HardReset(int i) __asm__("cChaseCameraController_switchCamera_HardReset");
    void setTarget(int* target) __asm__("cChaseCameraController_setTarget");
    cRider* getRider() __asm__("cChaseCameraController_getRider");
    void processInput() __asm__("cChaseCameraController_processInput");
    void update() __asm__("cChaseCameraController_update");
    void addAlgorithm(cCameraAlgorithm* algo, float w) __asm__("cChaseCameraController_addAlgorithm");
    void switchCamera_UserChange(int type) __asm__("cChaseCameraController_switchCamera_UserChange");
    void switchCamera_EndReplay() __asm__("cChaseCameraController_switchCamera_EndReplay");
    void switchCamera_ReplayToggle(int mode) __asm__("cChaseCameraController_switchCamera_ReplayToggle");
    void switchCamera_Volume(int mode, int action, float blend) __asm__("cChaseCameraController_switchCamera_Volume");
    void switchCamera_Chapter() __asm__("cChaseCameraController_switchCamera_Chapter");
    void switchCamera_SoftReset() __asm__("cChaseCameraController_switchCamera_SoftReset");
    void* switchCamera_Restart() __asm__("cChaseCameraController_switchCamera_Restart");
    void switchCamera_Slow() __asm__("cChaseCameraController_switchCamera_Slow");
    void switchCamera_EndSlow() __asm__("cChaseCameraController_switchCamera_EndSlow");
    void switchCamera_EndRace() __asm__("cChaseCameraController_switchCamera_EndRace");
    void* switchCamera_EndPostRace() __asm__("cChaseCameraController_switchCamera_EndPostRace__FPv");
    int getLatestAlgoType() __asm__("cChaseCameraController_getLatestAlgoType");
    cRiderCameraTarget* getTarget() __asm__("cChaseCameraController_getTarget");
    void createChaseAlgorithmBlend(int kind, char* ctx, float blend)
        __asm__("cChaseCameraController_createChaseAlgorithmBlend");
    int shouldDoExtraGroundTest() __asm__("cChaseCameraController_shouldDoExtraGroundTest");
    void* createChaseAlgorithmCut(int type, int action) __asm__("cChaseCameraController_createChaseAlgorithmCut");
};

CHECK_OFFSET(cChaseCameraController, Type, 0x00);
CHECK_OFFSET(cChaseCameraController, BlendType, 0x04);
CHECK_OFFSET(cChaseCameraController, AlgorithmList, 0x08);
CHECK_OFFSET(cChaseCameraController, Input, 0x0C);
CHECK_OFFSET(cChaseCameraController, Camera, 0x10);
CHECK_OFFSET(cChaseCameraController, mVtbl, 0x14);
CHECK_OFFSET(cChaseCameraController, LatestAlgoType, 0x18);
CHECK_OFFSET(cChaseCameraController, PreferredChaseAlgoLastFrame, 0x1C);
CHECK_OFFSET(cChaseCameraController, Target, 0x20);
CHECK_OFFSET(cChaseCameraController, CamSwitchRecord, 0x24);
CHECK_OFFSET(cChaseCameraController, TimeSinceLastSuggestedVolumeTrigger, 0x38);
CHECK_OFFSET(cChaseCameraController, GoingSlow, 0x3C);
CHECK_OFFSET(cChaseCameraController, NumFramesSlowGround, 0x40);
CHECK_SIZE(cChaseCameraController, 0x44);

#endif /* CAMERA_RACE_RACECONTROLLER_H */
