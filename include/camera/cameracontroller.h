#ifndef CAMERA_CAMERACONTROLLER_H
#define CAMERA_CAMERACONTROLLER_H

// cManualCameraController: the free-camera controller of a cCamera (debug / editor camera). It owns a manual and an
// editor camera algorithm (camera/cameraalgorithms.h: cManualCameraAlgorithm and cEditorCameraAlgorithm, 0x40 each)
// and keeps exactly one of them in its algorithm list (setCurrentAlgorithmType: 0x50 editor, 0x51 manual).
// EA's source is d:/ssx/code/source/camera/cameracontroller.cpp (July 2003 preview assert strings); its methods are in
// retail's camera/cameracontroller.cpp (0x15CD80-0x15CFE8) and two inline accessors in camera/camera.cpp
// (cManualCameraController_getManualAlgo 0x168020, cManualCameraController_getEditorAlgo 0x168028).
// Size 0x20 [R]: cCamera_cCamera allocates it with cMemMan_alloc(0x20, "Manual Cam Controller") and stores it in
// cCamera::ManualController (camera/camera.h, +0xA4). Retail vtable cManualCameraController_vtbl (D_0045B8D8: deleting dtor
// cManualCameraController__cManualCameraController, update cManualCameraController_update, snap cManualCameraController_snap,
// snapCurrent cManualCameraController_snapCurrent).
// Base class cCameraController (0x18; camera/cameracontroller.cpp: cCameraController_cCameraController), written out
// inline as in camera/race/racecontroller.h (cChaseCameraController); cScriptCameraController (0x20) derives from it too.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance. Evidence tags:
//   [R] retail ctors cCameraController_cCameraController and cManualCameraController_cManualCameraController, the dtor
//       cManualCameraController__cManualCameraController, the allocation size;
//   [P] the preview's inline accessors via tools/layout_xlate.py: getManualAlgo (lw 0x18), getEditorAlgo (lw 0x1C); the
//       paired dtor / snap show 0x14-0x1C unchanged between the builds;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cCameraController 0x18, cManualCameraController 0x20): same members at the same offsets.

#include "layout.h"

struct cCameraAlgoList;
struct cInput;
struct cCamera;
struct cManualCameraAlgorithm;
struct cEditorCameraAlgorithm;

struct cManualCameraController {
    // --- cCameraController (On Tour: 0x18)
    int Type;                       // 0x00 [O] eCamControllerType; [R] base ctor 0, then 1 (manual)
    int BlendType;                  // 0x04 [O] eCamBlendType; [R] base ctor 0
    cCameraAlgoList* AlgorithmList; // 0x08 [O]; [R] base ctor: cMemMan_alloc(8, "Algorithm List") + cCameraAlgoList ctor;
                                    //      [U] cManualCameraController_addAlgorithm inserts into it, cManualCameraController_update and
                                    //      cManualCameraController_getCurrentAlgorithmType read its first algorithm
    cInput* Input;                  // 0x0C [O]; [R] base ctor 0
    cCamera* Camera;                // 0x10 [O]; [R] the ctor's camera argument (cCamera_cCamera passes itself)
    void** mVtbl;                   // 0x14 [R] cManualCameraController_vtbl (ctor and dtor); [P] vptr at 0x14 in both builds
    // --- cManualCameraController
    cManualCameraAlgorithm* ManualAlgo; // 0x18 [O]; [P] getManualAlgo (lw); [R] ctor: cMemMan_alloc(0x40, "Manual Cam Algo") +
                                    //      cManualCameraAlgorithm ctor; [U] the dtor deletes it (slot 1), snap / snapCurrent call
                                    //      its slots 3 / 4, setCurrentAlgorithmType(0x51) inserts it
    cEditorCameraAlgorithm* EditorAlgo; // 0x1C [O]; [P] getEditorAlgo (lw); [R] ctor: cMemMan_alloc(0x40, "Editor Cam Algo") +
                                    //      cEditorCameraAlgorithm ctor; [U] the dtor deletes it, setCurrentAlgorithmType(0x50)
                                    //      inserts it
};

CHECK_OFFSET(cManualCameraController, Type, 0x00);
CHECK_OFFSET(cManualCameraController, BlendType, 0x04);
CHECK_OFFSET(cManualCameraController, AlgorithmList, 0x08);
CHECK_OFFSET(cManualCameraController, Input, 0x0C);
CHECK_OFFSET(cManualCameraController, Camera, 0x10);
CHECK_OFFSET(cManualCameraController, mVtbl, 0x14);
CHECK_OFFSET(cManualCameraController, ManualAlgo, 0x18);
CHECK_OFFSET(cManualCameraController, EditorAlgo, 0x1C);
CHECK_SIZE(cManualCameraController, 0x20);

#endif /* CAMERA_CAMERACONTROLLER_H */
