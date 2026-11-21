#ifndef CAMERA_SCRIPT_SCRIPTCONTROLLER_H
#define CAMERA_SCRIPT_SCRIPTCONTROLLER_H

// cScriptCameraController: the cut-scene camera controller of a cCamera (cCamera::ScriptController, camera/camera.h
// +0xAC). It owns the view's two script camera objects (cScriptObjCamera, scripter/scriptobjcamera.h); when a script
// camera starts, addCamera creates the camera algorithm for its entity type (manual, target/point, subject/target,
// algorithmic/relative), binds it to the camera object and inserts it into the algorithm list; update blends the
// algorithms by each camera object's weight (smoothstep) and runs them; removeCamera takes one out again.
// EA's source: d:/ssx/code/source/camera/script/scriptcontroller.cpp (July 2003 preview assert strings). Retail
// methods: unit camera/script/scriptcontroller (0x00168FE8 ctor .. 0x00169368 update).
//
// Size 0x20: [R] cCamera_cCamera allocates cMemMan_alloc(0x20, "Script Cam Controller") (camera/camera.cpp); the
// ctor writes up to 0x1C. [O] On Tour: 0x20 (IsActive 0x18 as a bool, ScriptObj 0x1C).
// Base cCameraController (0x18; camera/cameracontroller.cpp: cCameraController_cCameraController(self, camera)),
// written out inline with the names of camera/cameracontroller.h (cManualCameraController, same base) and
// camera/race/racecontroller.h (cChaseCameraController). Retail vtable cScriptCameraController_vtbl (0x0045C428,
// 4 slots: 1 dtor, 2 update, 3 cCameraController_snap, 4 cCameraController_snapCurrent) at 0x14.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail ctor cScriptCameraController_cScriptCameraController (allocation tags "ScriptObjCamArray" 8,
//       "ScriptObjCam" 0x80), the dtor, the allocation size;
//   [P] tools/layout_xlate.py: 3 paired methods show 0x8, 0x10, 0x14, 0x1C unchanged; the preview's inline
//       IsControllerActive reads 0x18 (bracketed 0x14..0x1C);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cCameraController 0x18, cScriptCameraController 0x20).

#include "layout.h"

struct cCameraAlgoList;
struct cInput;
struct cCamera;
struct cScriptObjCamera;

struct cScriptCameraController {
    // --- cCameraController (On Tour: 0x18; camera/cameracontroller.h)
    int Type;                       // 0x00 [O] eCamControllerType; [R] base ctor 0, then 3 (script)
    int BlendType;                  // 0x04 [O] eCamBlendType; [R] base ctor 0
    cCameraAlgoList* AlgorithmList; // 0x08 [O]; [R] base ctor: cMemMan_alloc(8, "Algorithm List"); [U] addAlgorithm
                                    //      inserts into it (cCameraAlgoList_insert, weight 1.0), removeCamera removes
                                    //      from it, update walks its nodes; cCameraController_snap / snapCurrent
    cInput* Input;                  // 0x0C [O]; [R] base ctor 0
    cCamera* Camera;                // 0x10 [O]; [R] the ctor's camera argument (cCamera_cCamera passes itself);
                                    //      [U] cCameraController_snap / snapCurrent
    void** mVtbl;                   // 0x14 [R] cScriptCameraController_vtbl (ctor and dtor)
    // --- cScriptCameraController
    int IsActive;                   // 0x18 [O] bool (word here); [P] IsControllerActive (lw 0x18); [R] ctor 0;
                                    //      [U] addCamera sets 1; removeCamera works only while set and clears it
                                    //      when the algorithm list empties
    cScriptObjCamera** ScriptObj;   // 0x1C [O]; [R] ctor: operator new(8, "ScriptObjCamArray"), then 2 x
                                    //      cScriptObjCamera_cScriptObjCamera(cMemMan_alloc(0x80, "ScriptObjCam"),
                                    //      viewIndex) + cScriptObject_InitScriptObject; [U] the dtor deinitialises and
                                    //      deletes both, then frees the array
};

CHECK_OFFSET(cScriptCameraController, Type, 0x00);
CHECK_OFFSET(cScriptCameraController, BlendType, 0x04);
CHECK_OFFSET(cScriptCameraController, AlgorithmList, 0x08);
CHECK_OFFSET(cScriptCameraController, Input, 0x0C);
CHECK_OFFSET(cScriptCameraController, Camera, 0x10);
CHECK_OFFSET(cScriptCameraController, mVtbl, 0x14);
CHECK_OFFSET(cScriptCameraController, IsActive, 0x18);
CHECK_OFFSET(cScriptCameraController, ScriptObj, 0x1C);
CHECK_SIZE(cScriptCameraController, 0x20);

#endif /* CAMERA_SCRIPT_SCRIPTCONTROLLER_H */
