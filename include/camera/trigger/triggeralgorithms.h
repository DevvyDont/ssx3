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
#include "bx/math3d.h"

struct cRiderCameraTarget;
struct cCTActionBoundedCam;

// cTriggerAlgorithm (On Tour: 0x80): base cChaseAlgorithm <- cCameraAlgorithm.
struct cTriggerAlgorithm {
    // --- cCameraAlgorithm (camera/cameraalgorithms.h, 0x14)
    float View;                 // 0x00 [O]; [R] cChaseAlgorithm_cChaseAlgorithm (getDefaultView)
    float Near;                 // 0x04 [O]; [R] 10.0f (base ctor)
    float Far;                  // 0x08 [O]; [R] 30000.0f (base ctor)
    int AlgorithmType;          // 0x0C [O] eCamAlgoType; [R] cTriggerAlgorithm_cTriggerAlgorithm stores 90
    void** mVtbl;               // 0x10 [R] cTriggerAlgorithm_vtbl (ctor); the dtor puts back cCameraAlgorithm's D_0045BDE0;
                                //      [U] snap calls slot 5 (update), snapCurrent calls slot 3 (snap)
    char unk_14[0xC];           // 0x14 alignment gap before the 16-byte-aligned t4Vector
    // --- cChaseAlgorithm (0x40)
    t4Vector CamTargetPos;      // 0x20 [O]; [U] cTriggerAlgorithm_update stores the focus point (sq)
    cRiderCameraTarget* Target; // 0x30 [O]; [U] snap / snapCurrent / update call its virtuals (getPosition slot 1,
                                //      getVelocity slot 3, getFront slot 4) and reach the rider through its +0x4
    char unk_34[0xC];           // 0x34 padding to cChaseAlgorithm's 16-byte size
    // --- cTriggerAlgorithm
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

CHECK_OFFSET(cTriggerAlgorithm, View, 0x00);
CHECK_OFFSET(cTriggerAlgorithm, Near, 0x04);
CHECK_OFFSET(cTriggerAlgorithm, Far, 0x08);
CHECK_OFFSET(cTriggerAlgorithm, AlgorithmType, 0x0C);
CHECK_OFFSET(cTriggerAlgorithm, mVtbl, 0x10);
CHECK_OFFSET(cTriggerAlgorithm, unk_14, 0x14);
CHECK_OFFSET(cTriggerAlgorithm, CamTargetPos, 0x20);
CHECK_OFFSET(cTriggerAlgorithm, Target, 0x30);
CHECK_OFFSET(cTriggerAlgorithm, unk_34, 0x34);
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
struct cBoundedAlgorithm {
    // --- cCameraAlgorithm (camera/cameraalgorithms.h, 0x14)
    float View;                 // 0x00 [O]; [U] cBoundedAlgorithm_update copies the action's FieldOfView (+0x14) (swc1)
    float Near;                 // 0x04 [O]; [R] base ctor
    float Far;                  // 0x08 [O]; [R] base ctor
    int AlgorithmType;          // 0x0C [O] eCamAlgoType; [R] cBoundedAlgorithm_cBoundedAlgorithm stores 91
    void** mVtbl;               // 0x10 [R] cBoundedAlgorithm_vtbl (ctor and dtor); [U] snapCurrent calls slot 3 (snap)
    char unk_14[0xC];           // 0x14 alignment gap
    // --- cChaseAlgorithm (0x40)
    t4Vector CamTargetPos;      // 0x20 [O]; [U] cBoundedAlgorithm_update stores the look target (sq)
    cRiderCameraTarget* Target; // 0x30 [O]; [U] update / snap / snapCurrent call its virtuals
    char unk_34[0xC];           // 0x34 padding to cChaseAlgorithm's 16-byte size
    // --- cTriggerAlgorithm (0x80)
    t4Vector Position;          // 0x40 [O]; [U] update: &Position to the bound object's limitPoint, then lq
    float Heading;              // 0x50 [O]; [U] update/snap (swc1)
    float Pitch;                // 0x54 [O]; [U] update/snap (swc1)
    float PitchMultiplier;      // 0x58 [O]; [R] cTriggerAlgorithm ctor
    float HeadingAdjustRate;    // 0x5C [O]; [R] cTriggerAlgorithm ctor
    float PitchAdjustRate;      // 0x60 [O]; [R] cTriggerAlgorithm ctor
    float HeadingLookRate;      // 0x64 [O]; [R] cTriggerAlgorithm ctor
    float PitchLookRate;        // 0x68 [O]; [R] cTriggerAlgorithm ctor
    float HeightOffset;         // 0x6C [O]; [U] update/snap copy the action's HeightOffset
    float IdealDist;            // 0x70 [O]; [U] update/snap copy the action's IdealDist
    float LeadDistance;         // 0x74 [O]; [R] cBoundedAlgorithm ctor zeroes it again (sw $0); [U] update/snap copy
                                //      the action's LeadDistance
    float PitchOffset;          // 0x78 [O]; [U] update/snap copy the action's PitchOffset
    char unk_7C[0x4];           // 0x7C padding to cTriggerAlgorithm's 16-byte size
    // --- cBoundedAlgorithm
    cCTActionBoundedCam* BoundAction; // 0x80 [O]; [R] ctor 0; cChaseCameraController_createChaseAlgorithmBlend stores the
                                //      action right after construction; [U] update/snap read its settings, its
                                //      TargetType/TargetPoint and its BoundObj (+0x8); cBoundedAlgorithm_setBoundAction (sw) /
                                //      cBoundedAlgorithm_getBoundAction (lw)
    char unk_84[0xC];           // 0x84 padding to the 16-byte class alignment
};

CHECK_OFFSET(cBoundedAlgorithm, View, 0x00);
CHECK_OFFSET(cBoundedAlgorithm, Near, 0x04);
CHECK_OFFSET(cBoundedAlgorithm, Far, 0x08);
CHECK_OFFSET(cBoundedAlgorithm, AlgorithmType, 0x0C);
CHECK_OFFSET(cBoundedAlgorithm, mVtbl, 0x10);
CHECK_OFFSET(cBoundedAlgorithm, unk_14, 0x14);
CHECK_OFFSET(cBoundedAlgorithm, CamTargetPos, 0x20);
CHECK_OFFSET(cBoundedAlgorithm, Target, 0x30);
CHECK_OFFSET(cBoundedAlgorithm, unk_34, 0x34);
CHECK_OFFSET(cBoundedAlgorithm, Position, 0x40);
CHECK_OFFSET(cBoundedAlgorithm, Heading, 0x50);
CHECK_OFFSET(cBoundedAlgorithm, Pitch, 0x54);
CHECK_OFFSET(cBoundedAlgorithm, PitchMultiplier, 0x58);
CHECK_OFFSET(cBoundedAlgorithm, HeadingAdjustRate, 0x5C);
CHECK_OFFSET(cBoundedAlgorithm, PitchAdjustRate, 0x60);
CHECK_OFFSET(cBoundedAlgorithm, HeadingLookRate, 0x64);
CHECK_OFFSET(cBoundedAlgorithm, PitchLookRate, 0x68);
CHECK_OFFSET(cBoundedAlgorithm, HeightOffset, 0x6C);
CHECK_OFFSET(cBoundedAlgorithm, IdealDist, 0x70);
CHECK_OFFSET(cBoundedAlgorithm, LeadDistance, 0x74);
CHECK_OFFSET(cBoundedAlgorithm, PitchOffset, 0x78);
CHECK_OFFSET(cBoundedAlgorithm, unk_7C, 0x7C);
CHECK_OFFSET(cBoundedAlgorithm, BoundAction, 0x80);
CHECK_OFFSET(cBoundedAlgorithm, unk_84, 0x84);
CHECK_SIZE(cBoundedAlgorithm, 0x90);

#endif /* CAMERA_TRIGGER_TRIGGERALGORITHMS_H */
