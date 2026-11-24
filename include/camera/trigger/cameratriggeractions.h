#ifndef CAMERA_TRIGGER_CAMERATRIGGERACTIONS_H
#define CAMERA_TRIGGER_CAMERATRIGGERACTIONS_H

// Camera-trigger actions (EA: d:/ssx/code/source/camera/trigger/cameratriggeractions.h, a header path in the July 2003
// preview's assert strings; the preview keeps purge__9cCTAction, purge__19cCTActionBoundedCam and
// purge__15cCTActionSpline as out-of-line inlines). A trigger's enter / exit action says what the chase camera does
// when a rider enters or leaves the trigger volume; doTriggerAction (camera/trigger/cameratriggerman.cpp) plays it.
//   cCTAction (0x8)                    Type (eCTActionType), vptr at 0x4; vtable cCTAction_vtbl (slot 1 purge, empty)
//    +- cCTActionSwitchCam (0x10)      Type 0, "Camera Switch Action" (not in this header)
//    +- cCTActionBoundedCam (0x34)     Type 1, "Camera Bound Action": camera type 91 (cBoundedAlgorithm, or a SUPERPIPE
//    |                                 chase camera when the bound object is a line)
//    +- cCTActionSpline (0x20)         Type 2, "Camera Spline Action": camera type 92 (cSplineCamAlgorithm)
//    +- cCTActionNone (0x8)            Type 3, "Camera Action None" (not in this header)
// Sizes [R]: get_camaction (camera/trigger/cameratriggerfactory, 0x1712B0) allocates cMemMan_alloc(0x10 / 0x34 / 0x20 /
// 0x8, tag), builds each action inline (cCTAction vtable cCTAction_vtbl first, then the derived vtable, then Type and the
// defaults) and reads its data (get_cCTActionBoundedCam / get_cCTActionSpline). Retail vtables (1 slot each, purge):
// cCTActionBoundedCam_vtbl cCTActionBoundedCam (cCTActionBoundedCam_purge), cCTActionSpline_vtbl cCTActionSpline (cCTActionSpline_purge).
//
// FLAT layouts, like main/game.h: every field at its retail offset, the cCTAction base written out inline, no C++
// inheritance. Member names, types and order are On Tour's (offsets equal retail's). Evidence tags:
//   [R] get_camaction's inline construction and the allocation sizes;
//   [U] how retail code uses the field: get_cCTActionBoundedCam / get_cCTActionSpline (stream reads), the purges,
//       doTriggerAction, cBoundedAlgorithm_update / _snap (camera/trigger/triggeralgorithms.h BoundAction),
//       cChaseCameraController_createChaseAlgorithmBlend (camera/cameraalgorithms.cpp);
//   [O] SSX On Tour's DWARF (cCTAction 0x8, cCTActionBoundedCam 0x34, cCTActionSpline 0x20).
// No [P]: no paired preview method reads a field.

#include "layout.h"
#include "ui/uithing.h"     // t3Vector

struct cCTBoundObj;
struct cCTSpline;

// cCTAction (retail and On Tour 0x8): the base of every action, and the type a trigger's EnterAction / ExitAction and
// cChaseCameraController_createChaseAlgorithmBlend's action argument have ([O] createChaseAlgorithmBlend(...,
// cCTAction* action, float blendRate)). [R] get_camaction stores cCTAction_vtbl first in every action it builds, then Type;
// the "Camera Action None" action is a bare cCTAction (cMemMan_alloc(0x8)).
struct cCTAction {
    int Type;                   // 0x00 [O] eCTActionType; [R] get_camaction stores 0..3 per kind; [U] doTriggerAction
                                //      switches on it
    void** mVtbl;               // 0x04 [R] cCTAction_vtbl (slot 1 purge)
};

CHECK_OFFSET(cCTAction, Type, 0x00);
CHECK_OFFSET(cCTAction, mVtbl, 0x04);
CHECK_SIZE(cCTAction, 0x8);

// cCTActionSwitchCam (retail and On Tour 0x10): base cCTAction. Switches the chase camera.
// [R] get_camaction: cMemMan_alloc(0x10, "Camera Switch Action"), Type 0; [U] get_cCTActionSwitchCam stores Type 0 and
// reads SwitchType (get_uint, +0x8) and BlendTime (get_float, +0xC) from the stream.
struct cCTActionSwitchCam {
    // --- cCTAction (0x8)
    int Type;                   // 0x00 [O] eCTActionType; [R] 0 = eCTActionSwitchCam; [U] get_cCTActionSwitchCam (sw $0)
    void** mVtbl;               // 0x04 [R] cCTAction_vtbl, then the derived vtable
    // --- cCTActionSwitchCam
    int SwitchType;             // 0x08 [O] eCTSwitchType; [U] get_cCTActionSwitchCam (get_uint)
    float BlendTime;            // 0x0C [O]; [U] get_cCTActionSwitchCam (get_float)
};

CHECK_OFFSET(cCTActionSwitchCam, Type, 0x00);
CHECK_OFFSET(cCTActionSwitchCam, mVtbl, 0x04);
CHECK_OFFSET(cCTActionSwitchCam, SwitchType, 0x08);
CHECK_OFFSET(cCTActionSwitchCam, BlendTime, 0x0C);
CHECK_SIZE(cCTActionSwitchCam, 0x10);

// cCTActionBoundedCam (retail and On Tour 0x34): base cCTAction. A free trigger camera kept inside a bound object.
struct cCTActionBoundedCam {
    // --- cCTAction (0x8)
    int Type;                   // 0x00 [O] eCTActionType; [R] 1 = eCTActionBoundedCam (sw); [U] get_cCTActionBoundedCam
                                //      stores 1 again; doTriggerAction switches on it (lw)
    void** mVtbl;               // 0x04 [R] cCTAction_vtbl (cCTAction), then cCTActionBoundedCam_vtbl (sw)
    // --- cCTActionBoundedCam
    cCTBoundObj* BoundObj;      // 0x08 [O]; [R] 0 (sw $0); [U] cCTActionBoundedCam_purge deletes it (lw);
                                //      cBoundedAlgorithm_update / _snap call its limitPoint (slot 6 of its vtable at
                                //      +0x24); createChaseAlgorithmBlend tests its Type (+0x28) for a line (2)
    float BlendTime;            // 0x0C [O]; [R] 1.0f (swc1); [U] get_cCTActionBoundedCam (get_float); doTriggerAction:
                                //      blend rate 1 / (BlendTime x 60)
    float IdealDist;            // 0x10 [O]; [R] 400.0f; [U] get (get_float); cBoundedAlgorithm_update / _snap copy it
                                //      to IdealDist (lwc1)
    float FieldOfView;          // 0x14 [O]; [R] pi/6 (0.5236f); [U] get; update / snap copy it to View (lwc1)
    float HeightOffset;         // 0x18 [O]; [R] 70.0f; [U] get; update / snap copy it to HeightOffset (lwc1)
    float PitchOffset;          // 0x1C [O]; [R] 25.0f; [U] get; update / snap copy it to PitchOffset (lwc1)
    float LeadDistance;         // 0x20 [O]; [R] 0 (sw $0); [U] get; update / snap copy it to LeadDistance (lwc1)
    int TargetType;             // 0x24 [O] eCTTargetType (0 rider, 1 point, 2 instance); [R] 0 (sw $0); [U] get
                                //      (get_uint); update / snap compare it with 1 (lw)
    t3Vector TargetPoint;       // 0x28 [O]; [R] = t3vect_Zero (sdl/sdr + sw); [U] get (get_t3Vector); update / snap
                                //      look at it when TargetType is 1 (lwc1 0x28/0x2C/0x30)
};

CHECK_OFFSET(cCTActionBoundedCam, Type, 0x00);
CHECK_OFFSET(cCTActionBoundedCam, mVtbl, 0x04);
CHECK_OFFSET(cCTActionBoundedCam, BoundObj, 0x08);
CHECK_OFFSET(cCTActionBoundedCam, BlendTime, 0x0C);
CHECK_OFFSET(cCTActionBoundedCam, IdealDist, 0x10);
CHECK_OFFSET(cCTActionBoundedCam, FieldOfView, 0x14);
CHECK_OFFSET(cCTActionBoundedCam, HeightOffset, 0x18);
CHECK_OFFSET(cCTActionBoundedCam, PitchOffset, 0x1C);
CHECK_OFFSET(cCTActionBoundedCam, LeadDistance, 0x20);
CHECK_OFFSET(cCTActionBoundedCam, TargetType, 0x24);
CHECK_OFFSET(cCTActionBoundedCam, TargetPoint, 0x28);
CHECK_SIZE(cCTActionBoundedCam, 0x34);

// cCTActionSpline (retail and On Tour 0x20): base cCTAction. A camera that runs along a 4-point spline.
struct cCTActionSpline {
    // --- cCTAction (0x8)
    int Type;                   // 0x00 [O] eCTActionType; [R] 2 = eCTActionSpline (sw); [U] get_cCTActionSpline stores
                                //      2 again; doTriggerAction (lw)
    void** mVtbl;               // 0x04 [R] cCTAction_vtbl (cCTAction), then cCTActionSpline_vtbl (sw)
    // --- cCTActionSpline
    cCTSpline* Spline;          // 0x08 [O]; [R] 0 (sw $0); [U] cCTActionSpline_purge deletes it (lw);
                                //      createChaseAlgorithmBlend passes it to cCTSpline_worldPoint (points 0..3)
    float BlendTime;            // 0x0C [O]; [R] 1.0f (swc1); [U] get_cCTActionSpline (get_float); doTriggerAction
    float FieldOfView;          // 0x10 [O]; [R] pi/6 (0.5236f); [U] get; createChaseAlgorithmBlend: the
                                //      cSplineCamAlgorithm ctor's fov
    float LeadDistance;         // 0x14 [O]; [R] 0 (sw $0); [U] get; the ctor's leadDistance
    float Time;                 // 0x18 [O]; [R] 4.0f; [U] get; the ctor's time
    float HeightOffset;         // 0x1C [O]; [R] 0 (sw $0); [U] get; the ctor's heightOffset
};

CHECK_OFFSET(cCTActionSpline, Type, 0x00);
CHECK_OFFSET(cCTActionSpline, mVtbl, 0x04);
CHECK_OFFSET(cCTActionSpline, Spline, 0x08);
CHECK_OFFSET(cCTActionSpline, BlendTime, 0x0C);
CHECK_OFFSET(cCTActionSpline, FieldOfView, 0x10);
CHECK_OFFSET(cCTActionSpline, LeadDistance, 0x14);
CHECK_OFFSET(cCTActionSpline, Time, 0x18);
CHECK_OFFSET(cCTActionSpline, HeightOffset, 0x1C);
CHECK_SIZE(cCTActionSpline, 0x20);

#endif /* CAMERA_TRIGGER_CAMERATRIGGERACTIONS_H */
