#ifndef CAMERA_SCRIPT_SCRIPTALGORITHMS_H
#define CAMERA_SCRIPT_SCRIPTALGORITHMS_H

// The script (cut-scene) camera algorithms (EA: d:/ssx/code/source/camera/script/scriptalgorithms.cpp, from the July
// 2003 preview's assert strings). cScriptCameraController_addCamera (camera/script/scriptcontroller.h) creates one per
// started script camera object (cScriptObjCamera, scripter/scriptobjcamera.h) by the entity's camera kind, binds it to
// the object (setScriptObj) and blends it in; each frame its update reads the entity's channel values for the frame
// (GetSSXCamera<Kind>Frame into FrameInfo) and turns them into Position / Target / Roll / Heading / Pitch.
//   cCameraAlgorithm (camera/cameraalgorithms.h, 0x14)
//    +- cScriptAlgorithm (0x30)           ScriptObj, Controller, Target; AlgorithmType 85 (On Tour SCRIPT_GENERIC)
//        +- cScriptManualAlgorithm  (0xB0)  kind 0, "Manual Script Camera Algo", type 86, tSSXFrameCameraManual
//        +- cScriptPointAlgorithm   (0xB0)  kind 1, "Target Script Camera Algo", type 87, tSSXFrameCameraTarget
//        +- cScriptTargetAlgorithm  (0xA0)  kind 2, "Subject Script Camera Algo", type 88, tSSXFrameCameraSubject
//        +- cScriptRelativeAlgorithm (0xA0) kind 3, "Algorithmic Script Camera Algo", type 89, tSSXFrameCameraAlgorithmic
// Sizes [R]: cScriptCameraController_addCamera allocates cMemMan_alloc(0xB0 / 0xB0 / 0xA0 / 0xA0, tag) before each ctor.
// cScriptAlgorithm is never allocated on its own; 0x30 = Target (+0x20, a t4Vector) + 0x10, the size On Tour gives.
// Retail vtables (11 slots; builds/x/vtables/vtables.tsv): cScriptAlgorithm_vtbl 0x0045C8B8, cScriptManualAlgorithm_vtbl
// 0x0045C850, cScriptPointAlgorithm_vtbl 0x0045C7E8, cScriptTargetAlgorithm_vtbl 0x0045C780, cScriptRelativeAlgorithm_vtbl
// 0x0045C718: 1 dtor, 2 getPose, 3 snap, 4 snapCurrent, 5 update, 6 cScriptAlgorithm_getTargetPos, 7-11 cCameraAlgorithm's.
// Retail code: ctors / snap / snapCurrent / update / getPose in camera/script/scriptcontroller (0x169418-0x16BC88); the
// dtors and getTargetPos in the camera/trigger/cameratriggerfactory segment (0x176328-0x176428).
//
// FLAT layouts, like main/game.h: every field at its retail offset, bases written out inline, no C++ inheritance.
// Member names, types and order are On Tour's (its offsets equal retail's for all five classes). Evidence tags:
//   [R] retail ctors (cScriptAlgorithm_cScriptAlgorithm and the four subclass ctors), the dtors, the allocation sizes;
//   [P] tools/layout_xlate.py: the paired preview methods (Target/Relative update, snap, snapCurrent, dtors) show every
//       offset 0x0-0x6C unchanged between the builds; no preview accessor names a field;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cScriptAlgorithm 0x30, cScriptManual/PointAlgorithm 0xB0, cScriptTarget/RelativeAlgorithm 0xA0,
//       the tSSXFrameCamera* structs).

#include "layout.h"
#include "camera/cameraalgorithms.h"
#include "bx/math3d.h"
#include "camera/camerajitterer.h"

struct cScriptObjCamera;
struct cScriptCameraController;

// This frame's channel values of a manual script camera (On Tour tSSXFrameCameraManual, 0x28); angles in degrees.
// [U] GetSSXCameraManualFrame fills it; cScriptManualAlgorithm_update reads each field (lwc1).
struct tSSXFrameCameraManual {
    float posX;                 // 0x00 [O]; [U] camera position, through the script object's mBaseTransform
    float posY;                 // 0x04 [O]
    float posZ;                 // 0x08 [O]
    float targetX;              // 0x0C [O]; [U] look-at point, through mBaseTransform
    float targetY;              // 0x10 [O]
    float targetZ;              // 0x14 [O]
    float roll;                 // 0x18 [O]; [U] -> Roll (x pi/180, + mBaseRotX)
    float fov;                  // 0x1C [O]; [U] -> View (x pi/180)
    float jitterFocus;          // 0x20 [O]; [U] cCameraJitterer_updatePositionalJitter
    float jitterRoll;           // 0x24 [O]; [U] cCameraJitterer_updateRollJitter
};

CHECK_OFFSET(tSSXFrameCameraManual, posX, 0x00);
CHECK_OFFSET(tSSXFrameCameraManual, posY, 0x04);
CHECK_OFFSET(tSSXFrameCameraManual, posZ, 0x08);
CHECK_OFFSET(tSSXFrameCameraManual, targetX, 0x0C);
CHECK_OFFSET(tSSXFrameCameraManual, targetY, 0x10);
CHECK_OFFSET(tSSXFrameCameraManual, targetZ, 0x14);
CHECK_OFFSET(tSSXFrameCameraManual, roll, 0x18);
CHECK_OFFSET(tSSXFrameCameraManual, fov, 0x1C);
CHECK_OFFSET(tSSXFrameCameraManual, jitterFocus, 0x20);
CHECK_OFFSET(tSSXFrameCameraManual, jitterRoll, 0x24);
CHECK_SIZE(tSSXFrameCameraManual, 0x28);

// A point ("target") script camera's channel values (On Tour tSSXFrameCameraTarget, 0x28); angles in degrees.
// [U] GetSSXCameraTargetFrame fills it; cScriptPointAlgorithm_update reads each field.
struct tSSXFrameCameraTarget {
    float targetX;              // 0x00 [O]; [U] orbit centre, through mBaseTransform -> Target
    float targetY;              // 0x04 [O]
    float targetZ;              // 0x08 [O]
    float targetDistance;       // 0x0C [O]; [U] Position = Target - direction x targetDistance
    float pitch;                // 0x10 [O]; [U] -> Pitch (x pi/180, + mBaseRotY)
    float heading;              // 0x14 [O]; [U] -> Heading (x pi/180, + mBaseRotZ)
    float roll;                 // 0x18 [O]; [U] -> Roll
    float fov;                  // 0x1C [O]; [U] -> View
    float jitterFocus;          // 0x20 [O]
    float jitterRoll;           // 0x24 [O]
};

CHECK_OFFSET(tSSXFrameCameraTarget, targetX, 0x00);
CHECK_OFFSET(tSSXFrameCameraTarget, targetY, 0x04);
CHECK_OFFSET(tSSXFrameCameraTarget, targetZ, 0x08);
CHECK_OFFSET(tSSXFrameCameraTarget, targetDistance, 0x0C);
CHECK_OFFSET(tSSXFrameCameraTarget, pitch, 0x10);
CHECK_OFFSET(tSSXFrameCameraTarget, heading, 0x14);
CHECK_OFFSET(tSSXFrameCameraTarget, roll, 0x18);
CHECK_OFFSET(tSSXFrameCameraTarget, fov, 0x1C);
CHECK_OFFSET(tSSXFrameCameraTarget, jitterFocus, 0x20);
CHECK_OFFSET(tSSXFrameCameraTarget, jitterRoll, 0x24);
CHECK_SIZE(tSSXFrameCameraTarget, 0x28);

// A subject script camera's channel values (On Tour tSSXFrameCameraSubject, 0x24): a scripted position looking at the
// subject rider. [U] GetSSXCameraSubjectFrame fills it; cScriptTargetAlgorithm_update reads each field.
struct tSSXFrameCameraSubject {
    float posX;                 // 0x00 [O]; [U] camera position, through mBaseTransform
    float posY;                 // 0x04 [O]
    float posZ;                 // 0x08 [O]
    float targetLeadDistance;   // 0x0C [O]; [U] target lead along the rider's front row
    float targetLeadHeight;     // 0x10 [O]; [U] target lead along the rider's up row
    float roll;                 // 0x14 [O]
    float fov;                  // 0x18 [O]
    float jitterFocus;          // 0x1C [O]
    float jitterRoll;           // 0x20 [O]
};

CHECK_OFFSET(tSSXFrameCameraSubject, posX, 0x00);
CHECK_OFFSET(tSSXFrameCameraSubject, posY, 0x04);
CHECK_OFFSET(tSSXFrameCameraSubject, posZ, 0x08);
CHECK_OFFSET(tSSXFrameCameraSubject, targetLeadDistance, 0x0C);
CHECK_OFFSET(tSSXFrameCameraSubject, targetLeadHeight, 0x10);
CHECK_OFFSET(tSSXFrameCameraSubject, roll, 0x14);
CHECK_OFFSET(tSSXFrameCameraSubject, fov, 0x18);
CHECK_OFFSET(tSSXFrameCameraSubject, jitterFocus, 0x1C);
CHECK_OFFSET(tSSXFrameCameraSubject, jitterRoll, 0x20);
CHECK_SIZE(tSSXFrameCameraSubject, 0x24);

// An algorithmic (relative) script camera's channel values (On Tour tSSXFrameCameraAlgorithmic, 0x24): a camera placed
// around the subject rider. [U] GetSSXCameraAlgorithmicFrame fills it; cScriptRelativeAlgorithm_snap / _update read it.
struct tSSXFrameCameraAlgorithmic {
    float targetLeadDistance;   // 0x00 [O]; [U] target lead along the rider's front row
    float targetLeadHeight;     // 0x04 [O]; [U] target lead along the rider's up row
    float optimalTargetDistance; // 0x08 [O]; [U] camera distance back from the target
    float optimalPitch;         // 0x0C [O]
    float optimalHeading;       // 0x10 [O]
    float roll;                 // 0x14 [O]
    float fov;                  // 0x18 [O]
    float jitterFocus;          // 0x1C [O]
    float jitterRoll;           // 0x20 [O]
};

CHECK_OFFSET(tSSXFrameCameraAlgorithmic, targetLeadDistance, 0x00);
CHECK_OFFSET(tSSXFrameCameraAlgorithmic, targetLeadHeight, 0x04);
CHECK_OFFSET(tSSXFrameCameraAlgorithmic, optimalTargetDistance, 0x08);
CHECK_OFFSET(tSSXFrameCameraAlgorithmic, optimalPitch, 0x0C);
CHECK_OFFSET(tSSXFrameCameraAlgorithmic, optimalHeading, 0x10);
CHECK_OFFSET(tSSXFrameCameraAlgorithmic, roll, 0x14);
CHECK_OFFSET(tSSXFrameCameraAlgorithmic, fov, 0x18);
CHECK_OFFSET(tSSXFrameCameraAlgorithmic, jitterFocus, 0x1C);
CHECK_OFFSET(tSSXFrameCameraAlgorithmic, jitterRoll, 0x20);
CHECK_SIZE(tSSXFrameCameraAlgorithmic, 0x24);

// cScriptAlgorithm (On Tour: 0x30): base cCameraAlgorithm. Abstract (slot 2 of cScriptAlgorithm_vtbl, getPose, is
// __pure_virtual 0x00411530); its ctor runs inside each subclass ctor.
struct cScriptAlgorithm : public cCameraAlgorithm {
    // cCameraAlgorithm (camera/cameraalgorithms.h, 0x14): the vptr at 0x10:
    //   [R] cCameraAlgorithm_vtbl, then cScriptAlgorithm_vtbl (ctor); every dtor stores cCameraAlgorithm_vtbl; [U]
    //       snap calls slot 5 (update), snapCurrent slot 3 (snap) (lw)
    // cScriptAlgorithm's notes on cCameraAlgorithm's fields:
    //   View: [R] ctor = cCameraAlgorithm_getDefaultView() (swc1); [U] the updates set it from FrameInfo.fov (swc1)
    //   Near: [U] the updates copy the camera header's near (swc1)
    //   Far: [U] ditto (far)
    //   AlgorithmType: [R] 85 (0x55); each subclass ctor stores its own (sw)
    // --- cScriptAlgorithm
    cScriptObjCamera* ScriptObj; // 0x14 [O]; [R] 0; [U] cScriptAlgorithm_setScriptObj (sw) / cScriptAlgorithm_getScriptObj
                                //      (lw); the updates follow ScriptObj->mpCameraEntity (+0x14)->entity (+0x8) and read
                                //      its mBaseTransform (+0x30) and mBaseRotX/Y/Z (+0x70/+0x74/+0x78)
    cScriptCameraController* Controller; // 0x18 [O]; [R] = the first view camera's ScriptController (cCamera +0xAC)
                                //      (sw); no retail reader
    char unk_1C[0x4];           // 0x1C alignment gap before the 16-byte-aligned t4Vector
    t4Vector Target;            // 0x20 [O]; [R] = t4vect_Origin (sq); [U] cScriptAlgorithm_getTargetPos (lq; this in a1),
                                //      the updates store the look-at point (sq) and read it back (lqc2)

    // Methods bound to their retail symbols by asm labels.
    void setScriptObj(int val) __asm__("cScriptAlgorithm_setScriptObj__FPvi");
    cScriptObjCamera* getScriptObj() __asm__("cScriptAlgorithm_getScriptObj__FPv"); // [O] returns cScriptObjCamera*
};

CHECK_OFFSET(cScriptAlgorithm, ScriptObj, 0x14);
CHECK_OFFSET(cScriptAlgorithm, Controller, 0x18);
CHECK_OFFSET(cScriptAlgorithm, unk_1C, 0x1C);
CHECK_OFFSET(cScriptAlgorithm, Target, 0x20);
CHECK_SIZE(cScriptAlgorithm, 0x30);

// cScriptManualAlgorithm (retail and On Tour 0xB0): base cScriptAlgorithm <- cCameraAlgorithm. Position and look-at
// point come straight from the script. [R] cMemMan_alloc(0xB0, "Manual Script Camera Algo") + ctor (type 86).
struct cScriptManualAlgorithm : public cScriptAlgorithm {
    // cScriptAlgorithm (camera/script/scriptalgorithms.h, 0x30): the vptr at 0x10:
    //   [R] cScriptManualAlgorithm_vtbl (ctor); the dtor stores cCameraAlgorithm_vtbl
    // --- cScriptManualAlgorithm
    virtual tPose getPose();                                                        // 2 ([R] cScriptManualAlgorithm_vtbl: cScriptManualAlgorithm_getPose)
    t4Vector Position;          // 0x30 [O]; [R] = t4vect_Origin (sq); [U] update (sq, lqc2), getPose (lqc2)
    float Roll;                 // 0x40 [O]; [R] 0 (sw $0); [U] update (swc1), getPose (lwc1)
    float Heading;              // 0x44 [O]; [R] 0; [U] update: yaw of Target - Position (swc1); getPose (lwc1)
    float Pitch;                // 0x48 [O]; [R] 0; [U] update: elevation (swc1); getPose (lwc1)
    tSSXFrameCameraManual FrameInfo; // 0x4C [O]; [U] update passes &FrameInfo to GetSSXCameraManualFrame, reads
                                //      0x4C..0x70 (lwc1)
    cCameraJitterer Jitterer;   // 0x74 [O]; [R] cCameraJitterer_cCameraJitterer(this + 0x74); [U] update / getPose pass
                                //      +0x74 to the jitterer's methods
    char unk_A4[0xC];           // 0xA4 padding to the 16-byte class alignment (alloc 0xB0)

    // Methods bound to their retail symbols by asm labels.
    virtual void snapCurrent() __asm__("cScriptManualAlgorithm_snapCurrent");
    virtual void update() __asm__("cScriptManualAlgorithm_update");
};

CHECK_OFFSET(cScriptManualAlgorithm, Position, 0x30);
CHECK_OFFSET(cScriptManualAlgorithm, Roll, 0x40);
CHECK_OFFSET(cScriptManualAlgorithm, Heading, 0x44);
CHECK_OFFSET(cScriptManualAlgorithm, Pitch, 0x48);
CHECK_OFFSET(cScriptManualAlgorithm, FrameInfo, 0x4C);
CHECK_OFFSET(cScriptManualAlgorithm, Jitterer, 0x74);
CHECK_OFFSET(cScriptManualAlgorithm, unk_A4, 0xA4);
CHECK_SIZE(cScriptManualAlgorithm, 0xB0);

// cScriptPointAlgorithm (retail and On Tour 0xB0): base cScriptAlgorithm. Orbits a scripted point at a distance,
// heading and pitch. [R] cMemMan_alloc(0xB0, "Target Script Camera Algo") + ctor (type 87).
struct cScriptPointAlgorithm : public cScriptAlgorithm {
    // cScriptAlgorithm (camera/script/scriptalgorithms.h, 0x30): the vptr at 0x10:
    //   [R] cScriptPointAlgorithm_vtbl (ctor); the dtor stores cCameraAlgorithm_vtbl
    // --- cScriptPointAlgorithm
    virtual tPose getPose();                                                        // 2 ([R] cScriptPointAlgorithm_vtbl: cScriptPointAlgorithm_getPose)
    t4Vector Position;          // 0x30 [O]; [R] = t4vect_Origin (sq); [U] update: Target - direction x targetDistance
                                //      (sq); getPose (lqc2)
    float Roll;                 // 0x40 [O]; [R] 0; [U] update (swc1), getPose (lwc1)
    float Heading;              // 0x44 [O]; [R] 0; [U] update (swc1, lwc1), getPose (lwc1)
    float Pitch;                // 0x48 [O]; [R] 0; [U] update (swc1, lwc1), getPose (lwc1)
    tSSXFrameCameraTarget FrameInfo; // 0x4C [O]; [U] update: GetSSXCameraTargetFrame(&FrameInfo), reads 0x4C..0x70
    cCameraJitterer Jitterer;   // 0x74 [O]; [R] cCameraJitterer_cCameraJitterer(this + 0x74); [U] update / getPose
    char unk_A4[0xC];           // 0xA4 padding to the 16-byte class alignment (alloc 0xB0)

    // Methods bound to their retail symbols by asm labels.
    virtual void snapCurrent() __asm__("cScriptPointAlgorithm_snapCurrent");
    virtual void update() __asm__("cScriptPointAlgorithm_update");
};

CHECK_OFFSET(cScriptPointAlgorithm, Position, 0x30);
CHECK_OFFSET(cScriptPointAlgorithm, Roll, 0x40);
CHECK_OFFSET(cScriptPointAlgorithm, Heading, 0x44);
CHECK_OFFSET(cScriptPointAlgorithm, Pitch, 0x48);
CHECK_OFFSET(cScriptPointAlgorithm, FrameInfo, 0x4C);
CHECK_OFFSET(cScriptPointAlgorithm, Jitterer, 0x74);
CHECK_OFFSET(cScriptPointAlgorithm, unk_A4, 0xA4);
CHECK_SIZE(cScriptPointAlgorithm, 0xB0);

// cScriptTargetAlgorithm (retail and On Tour 0xA0): base cScriptAlgorithm. A scripted position looking at the subject
// rider. [R] cMemMan_alloc(0xA0, "Subject Script Camera Algo") + ctor (type 88).
struct cScriptTargetAlgorithm : public cScriptAlgorithm {
    // cScriptAlgorithm (camera/script/scriptalgorithms.h, 0x30): the vptr at 0x10:
    //   [R] cScriptTargetAlgorithm_vtbl (ctor); the dtor stores cCameraAlgorithm_vtbl
    // --- cScriptTargetAlgorithm
    virtual tPose getPose();                                                        // 2 ([R] cScriptTargetAlgorithm_vtbl: cScriptTargetAlgorithm_getPose)
    t4Vector Position;          // 0x30 [O]; [R] = t4vect_Origin (sq); [U] update keeps .w (lwc1 +0x3C), stores the
                                //      transformed position (sq); getPose (lqc2)
    float Roll;                 // 0x40 [O]; [R] 0; [U] update (swc1), getPose (lwc1)
    float Heading;              // 0x44 [O]; [R] 0; [U] update (swc1), getPose (lwc1)
    float Pitch;                // 0x48 [O]; [R] 0; [U] update (swc1), getPose (lwc1)
    tSSXFrameCameraSubject FrameInfo; // 0x4C [O]; [U] update: GetSSXCameraSubjectFrame(&FrameInfo), reads 0x4C..0x6C
    cCameraJitterer Jitterer;   // 0x70 [O]; [R] cCameraJitterer_cCameraJitterer(this + 0x70); [U] update / getPose

    // Methods bound to their retail symbols by asm labels.
    virtual void snapCurrent() __asm__("cScriptTargetAlgorithm_snapCurrent");
    virtual void update() __asm__("cScriptTargetAlgorithm_update");
};

CHECK_OFFSET(cScriptTargetAlgorithm, Position, 0x30);
CHECK_OFFSET(cScriptTargetAlgorithm, Roll, 0x40);
CHECK_OFFSET(cScriptTargetAlgorithm, Heading, 0x44);
CHECK_OFFSET(cScriptTargetAlgorithm, Pitch, 0x48);
CHECK_OFFSET(cScriptTargetAlgorithm, FrameInfo, 0x4C);
CHECK_OFFSET(cScriptTargetAlgorithm, Jitterer, 0x70);
CHECK_SIZE(cScriptTargetAlgorithm, 0xA0);

// cScriptRelativeAlgorithm (retail and On Tour 0xA0): base cScriptAlgorithm. A camera placed at an optimal distance,
// pitch and heading around the subject rider, chasing that place at a capped speed. [R] cMemMan_alloc(0xA0,
// "Algorithmic Script Camera Algo") + ctor (type 89).
struct cScriptRelativeAlgorithm : public cScriptAlgorithm {
    // cScriptAlgorithm (camera/script/scriptalgorithms.h, 0x30): the vptr at 0x10:
    //   [R] cScriptRelativeAlgorithm_vtbl (ctor); the dtor stores cCameraAlgorithm_vtbl
    // --- cScriptRelativeAlgorithm
    virtual tPose getPose();                                                        // 2 ([R] cScriptRelativeAlgorithm_vtbl: cScriptRelativeAlgorithm_getPose)
    t4Vector Position;          // 0x30 [O]; [R] = t4vect_Origin (sq); [U] snap / update (sq, lqc2; update sets .w =
                                //      1.0f with swc1 +0x3C); getPose (lqc2)
    float Roll;                 // 0x40 [O]; [R] 0; [U] snap / update (swc1), getPose (lwc1)
    float Heading;              // 0x44 [O]; [R] 0; [U] update (swc1), getPose (lwc1)
    float Pitch;                // 0x48 [O]; [R] 0; [U] update (swc1), getPose (lwc1)
    tSSXFrameCameraAlgorithmic FrameInfo; // 0x4C [O]; [U] snap / update: GetSSXCameraAlgorithmicFrame(&FrameInfo),
                                //      read 0x4C..0x6C
    cCameraJitterer Jitterer;   // 0x70 [O]; [R] cCameraJitterer_cCameraJitterer(this + 0x70); [U] snap / update / getPose

    // Methods bound to their retail symbols by asm labels.
    virtual void snapCurrent() __asm__("cScriptRelativeAlgorithm_snapCurrent");
    virtual void update() __asm__("cScriptRelativeAlgorithm_update");
};

CHECK_OFFSET(cScriptRelativeAlgorithm, Position, 0x30);
CHECK_OFFSET(cScriptRelativeAlgorithm, Roll, 0x40);
CHECK_OFFSET(cScriptRelativeAlgorithm, Heading, 0x44);
CHECK_OFFSET(cScriptRelativeAlgorithm, Pitch, 0x48);
CHECK_OFFSET(cScriptRelativeAlgorithm, FrameInfo, 0x4C);
CHECK_OFFSET(cScriptRelativeAlgorithm, Jitterer, 0x70);
CHECK_SIZE(cScriptRelativeAlgorithm, 0xA0);

#endif /* CAMERA_SCRIPT_SCRIPTALGORITHMS_H */
