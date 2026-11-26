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

// cCameraController: the base of a cCamera's three camera controllers (cManualCameraController below,
// cChaseCameraController in camera/race/racecontroller.h, cScriptCameraController in camera/script/scriptcontroller.h),
// which write it out inline. It owns the controller's algorithm list and knows its camera. EA's source:
// d:/ssx/code/source/camera/cameracontroller.cpp (preview assert strings).
// Size 0x18: [O] On Tour's cCameraController is 0x18 (Type, BlendType, AlgorithmList, Input, Camera; vptr after them);
// [R] the subclasses' own fields start at 0x18 (cManualCameraController ManualAlgo, cScriptCameraController IsActive);
// never allocated on its own. No base class. Retail vtable cCameraController_vtbl (0x0045B938, 4 slots: 1 deleting dtor
// cCameraController__cCameraController, 2 update cCameraController_update__FPv (empty), 3 cCameraController_snap,
// 4 cCameraController_snapCurrent); cChaseCameraController_vtbl and cScriptCameraController_vtbl keep slots 3-4.
// Methods: ctor cCameraController_cCameraController (camera/cameracontroller.cpp 0x15CBA0) .. clearAlgorithms 0x15CD60;
// update and the getType / getBlendType / getAlgorithmList accessors in camera/camera.cpp (0x167FB8, 0x167FD0-0x167FE0).
// Tags: [R] the ctor / dtor; [P] the preview's inline accessors via tools/layout_xlate.py (getAlgorithmList seen at 0x8,
// getInput/setInput bracketed 0x8..0x14 at 0xC, the dtor's vptr store at 0x14 unchanged); [U] retail use; [O] On Tour.
struct cCameraController {
    int Type;                       // 0x00 [O] eCamControllerType; [P] getType (lw); [R] ctor 0 (subclass ctors then
                                    //      store 1 manual / 2 chase / 3 script); [U] cCameraController_getType (lw)
    int BlendType;                  // 0x04 [O] eCamBlendType; [P] getBlendType (lw); [R] ctor 0; [U]
                                    //      cCameraController_getBlendType (lw)
    cCameraAlgoList* AlgorithmList; // 0x08 [O]; [P] getAlgorithmList (lw, seen); [R] ctor: cMemMan_alloc(8, "Algorithm
                                    //      List") + cCameraAlgoList_cCameraAlgoList; [U] the dtor destroys it (flags 3),
                                    //      snap / snapCurrent walk its nodes (lw 0x8, then the head), clearAlgorithms
                                    //      passes it to cCameraAlgoList_removeAll, getAlgorithmList (lw)
    cInput* Input;                  // 0x0C [O]; [P] getInput (lw) / setInput (sw), bracketed; [R] ctor 0
    cCamera* Camera;                // 0x10 [O]; [R] the ctor's camera argument (sw); [U] snap / snapCurrent pass it to
                                    //      cCamera_snapResultAlgorithm and cSplashManager_CameraSnapped (lw)
    void** mVtbl;                   // 0x14 [R] cCameraController_vtbl (ctor and dtor); [P] vptr at 0x14 in both builds

    // Methods bound to their retail symbols by asm labels.
    void update() __asm__("cCameraController_update__FPv");
    int getBlendType() __asm__("cCameraController_getBlendType");
    cCameraAlgoList* getAlgorithmList() __asm__("cCameraController_getAlgorithmList");
    void snapCurrent() __asm__("cCameraController_snapCurrent");
    void* clearAlgorithms() __asm__("cCameraController_clearAlgorithms__FPv");
    void snap(int subjectID) __asm__("cCameraController_snap");
};

CHECK_OFFSET(cCameraController, Type, 0x00);
CHECK_OFFSET(cCameraController, BlendType, 0x04);
CHECK_OFFSET(cCameraController, AlgorithmList, 0x08);
CHECK_OFFSET(cCameraController, Input, 0x0C);
CHECK_OFFSET(cCameraController, Camera, 0x10);
CHECK_OFFSET(cCameraController, mVtbl, 0x14);
CHECK_SIZE(cCameraController, 0x18);

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

    // Methods bound to their retail symbols by asm labels.
    cManualCameraAlgorithm* getManualAlgo() __asm__("cManualCameraController_getManualAlgo");
    cEditorCameraAlgorithm* getEditorAlgo() __asm__("cManualCameraController_getEditorAlgo");
    void snap(int subjectID) __asm__("cManualCameraController_snap");
    void snapCurrent() __asm__("cManualCameraController_snapCurrent");
    void addAlgorithm(int algo, int deleteAlgorithmOnDestruction) __asm__("cManualCameraController_addAlgorithm");
    void setCurrentAlgorithmType(int msg) __asm__("cManualCameraController_setCurrentAlgorithmType");
    int getCurrentAlgorithmType() __asm__("cManualCameraController_getCurrentAlgorithmType");
    void update() __asm__("cManualCameraController_update");
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
