#ifndef CAMERA_TRIGGER_TRIGGERALGORITHMS_H
#define CAMERA_TRIGGER_TRIGGERALGORITHMS_H

// The camera-trigger algorithms (EA: d:/ssx/code/source/camera/trigger/triggeralgorithms.cpp, from the July 2003
// preview's assert strings). A camera-trigger volume (camera/trigger/cameratriggerman) switches the chase camera to
// one of these while the rider is inside it:
//   cChaseAlgorithm (camera/cameraalgorithms.h, 0x40)
//    +- cTriggerAlgorithm (0x80)    a free-standing camera that keeps IdealDist from the rider and turns to look at
//        |                          it; AlgorithmType 90 (On Tour CAMERA_ALGORITHM_TRIGGER_GENERIC)
//        +- cBoundedAlgorithm (0x90) the same, but its settings come from a cCTActionBoundedCam and its position is
//                                   kept inside the action's bound object; AlgorithmType 91 (CAMERA_ALGORITHM_TRIGGER_BOUNDED)
// Retail vtables (builds/x/vtables/vtables.tsv): cTriggerAlgorithm_vtbl (D_0045C4C8: dtor, getPose, snap, snapCurrent,
// update, then cChaseAlgorithm's / cCameraAlgorithm's slots) and cBoundedAlgorithm_vtbl (D_0045C458: its own dtor, snap,
// snapCurrent, update; getPose is cTriggerAlgorithm_getPose). No other vtable uses a trigger-algorithm slot.
// Retail code: camera/trigger/cameratriggerfactory.cpp, 0x173208-0x1747A0 (+ setBoundAction/getBoundAction at 0x176428).
//
// Sizes: cBoundedAlgorithm 0x90 [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(144, ...) +
// cBoundedAlgorithm_cBoundedAlgorithm. cTriggerAlgorithm is never allocated on its own in retail (only as the base
// of cBoundedAlgorithm); 0x80 = its last field PitchOffset (+0x78) rounded up to the class's 16-byte alignment
// (t4Vector members), the same size On Tour's DWARF gives, and cBoundedAlgorithm's own field starts at 0x80.
//
// FLAT layouts, like main/game.h: every field at its retail offset, bases written out inline, no C++ inheritance.
// Evidence tags:
//   [R] retail ctors cTriggerAlgorithm_cTriggerAlgorithm (all defaults) and cBoundedAlgorithm_cBoundedAlgorithm,
//       the allocation size;
//   [P] the preview's paired methods via tools/layout_xlate.py: every cTriggerAlgorithm offset 0x10-0x78 is unchanged
//       between the builds (snap/snapCurrent/update/dtor); no preview accessor names a field;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cTriggerAlgorithm 0x80, cBoundedAlgorithm 0x90): same members at the same offsets.

#include "layout.h"
#include "camera/cameraalgorithms.h"
#include "bx/math3d.h"

struct cRiderCameraTarget;
struct cCTActionBoundedCam;

// cTriggerAlgorithm (On Tour: 0x80): base cChaseAlgorithm <- cCameraAlgorithm.
struct cTriggerAlgorithm : public cChaseAlgorithm {
    // cChaseAlgorithm (camera/cameraalgorithms.h, 0x40): the vptr at 0x10:
    //   [R] cTriggerAlgorithm_vtbl (ctor); the dtor puts back cCameraAlgorithm's cCameraAlgorithm_vtbl; [U] snap calls
    //       slot 5 (update), snapCurrent calls slot 3 (snap)
    // cTriggerAlgorithm's notes on cChaseAlgorithm's fields:
    //   View: [R] cChaseAlgorithm_cChaseAlgorithm (getDefaultView)
    //   Near: [R] 10.0f (base ctor)
    //   Far: [R] 30000.0f (base ctor)
    //   AlgorithmType: [R] cTriggerAlgorithm_cTriggerAlgorithm stores 90
    //   CamTargetPos: [U] cTriggerAlgorithm_update stores the focus point (sq)
    //   Target: [U] snap / snapCurrent / update call its virtuals (getPosition slot 1, getVelocity slot 3, getFront
    //       slot 4) and reach the rider through its +0x4
    // --- cTriggerAlgorithm
    virtual tPose getPose();                                                        // 2 ([R] cTriggerAlgorithm_vtbl: cTriggerAlgorithm_getPose)
    t4Vector Position;          // 0x40 [O]; [R] = t4vect_Origin; [U] snap/update store the new camera position (sq),
                                //      cTriggerAlgorithm_getPose translates by it (lq); cBoundedAlgorithm_update passes
                                //      &Position to the bound object's limitPoint
    float Heading;              // 0x50 [O]; [R] 0 (sw $0); [U] snap/update (lwc1/swc1), getPose rotates about Z by it
    float Pitch;                // 0x54 [O]; [R] 0 (sw $0); [U] snap/update (lwc1/swc1), getPose rotates about Y by -Pitch
    float PitchMultiplier;      // 0x58 [O]; [R] 0.8f; [U] snap/update scale the rider's front pitch by -PitchMultiplier
    float HeadingAdjustRate;    // 0x5C [O]; [R] 10.3f; [U] update (x 1/60)
    float PitchAdjustRate;      // 0x60 [O]; [R] 1.1f; [U] update (x 1/60)
    float HeadingLookRate;      // 0x64 [O]; [R] 5.0f; [U] update (x 1/60)
    float PitchLookRate;        // 0x68 [O]; [R] 10.0f; [U] update (x 1/60)
    float HeightOffset;         // 0x6C [O]; [R] 70.0f; [U] snap/update lift the focus along t4vect_ZAxis by it;
                                //      cBoundedAlgorithm_update copies the action's HeightOffset (+0x18)
    float IdealDist;            // 0x70 [O]; [R] 400.0f; [U] snap/update; cBoundedAlgorithm_update copies the action's
                                //      IdealDist (+0x10)
    float LeadDistance;         // 0x74 [O]; [R] 0 (sw $0); [U] snap/update push the focus along the rider's front by it;
                                //      cBoundedAlgorithm_update copies the action's LeadDistance (+0x20)
    float PitchOffset;          // 0x78 [O]; [R] 20.0f (degrees); [U] snap/update (x pi/180); cBoundedAlgorithm_update
                                //      copies the action's PitchOffset (+0x1C)
    char unk_7C[0x4];           // 0x7C padding to the 16-byte class alignment
};

CHECK_OFFSET(cTriggerAlgorithm, Position, 0x40);
CHECK_OFFSET(cTriggerAlgorithm, Heading, 0x50);
CHECK_OFFSET(cTriggerAlgorithm, Pitch, 0x54);
CHECK_OFFSET(cTriggerAlgorithm, PitchMultiplier, 0x58);
CHECK_OFFSET(cTriggerAlgorithm, HeadingAdjustRate, 0x5C);
CHECK_OFFSET(cTriggerAlgorithm, PitchAdjustRate, 0x60);
CHECK_OFFSET(cTriggerAlgorithm, HeadingLookRate, 0x64);
CHECK_OFFSET(cTriggerAlgorithm, PitchLookRate, 0x68);
CHECK_OFFSET(cTriggerAlgorithm, HeightOffset, 0x6C);
CHECK_OFFSET(cTriggerAlgorithm, IdealDist, 0x70);
CHECK_OFFSET(cTriggerAlgorithm, LeadDistance, 0x74);
CHECK_OFFSET(cTriggerAlgorithm, PitchOffset, 0x78);
CHECK_OFFSET(cTriggerAlgorithm, unk_7C, 0x7C);
CHECK_SIZE(cTriggerAlgorithm, 0x80);

// cBoundedAlgorithm (retail and On Tour: 0x90): base cTriggerAlgorithm <- cChaseAlgorithm <- cCameraAlgorithm.
struct cBoundedAlgorithm : public cTriggerAlgorithm {
    // cTriggerAlgorithm (camera/trigger/triggeralgorithms.h, 0x80): the vptr at 0x10:
    //   [R] cBoundedAlgorithm_vtbl (ctor and dtor); [U] snapCurrent calls slot 3 (snap)
    // --- cBoundedAlgorithm
    cCTActionBoundedCam* BoundAction; // 0x80 [O]; [R] ctor 0; cChaseCameraController_createChaseAlgorithmBlend stores the
                                //      action right after construction; [U] update/snap read its settings, its
                                //      TargetType/TargetPoint and its BoundObj (+0x8); cBoundedAlgorithm_setBoundAction (sw) /
                                //      cBoundedAlgorithm_getBoundAction (lw)
    char unk_84[0xC];           // 0x84 padding to the 16-byte class alignment

    // Methods bound to their retail symbols by asm labels.
    void setBoundAction(int boundAction) __asm__("cBoundedAlgorithm_setBoundAction");
    int getBoundAction() __asm__("cBoundedAlgorithm_getBoundAction");
};

CHECK_OFFSET(cBoundedAlgorithm, BoundAction, 0x80);
CHECK_OFFSET(cBoundedAlgorithm, unk_84, 0x84);
CHECK_SIZE(cBoundedAlgorithm, 0x90);

#endif /* CAMERA_TRIGGER_TRIGGERALGORITHMS_H */
