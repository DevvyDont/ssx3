#ifndef CAMERA_CAMERAALGORITHMS_H
#define CAMERA_CAMERAALGORITHMS_H

// The camera algorithm classes (EA: d:/ssx/code/source/camera/CameraAlgorithms.h, from the July 2003 preview's
// assert strings). A camera algorithm computes a pose each frame; cameras blend the poses of a list of them.
//   cCameraAlgorithm (0x14)                 View/Near/Far, AlgorithmType, vptr at 0x10
//    +- cBasicCameraAlgorithm (0x40)        a free camera (position + pitch/heading); cManualCameraAlgorithm
//    |                                      ("Manual Cam Algo", 0x40) and cEditorCameraAlgorithm ("Editor Cam Algo",
//    |                                      0x40) add no fields
//    +- cChaseAlgorithm (0x40)              follows a cRiderCameraTarget
//        +- cLookAtChaseAlgorithm (0x390)   the race chase camera. Subclasses with no fields of their own (all
//                                           cMemMan_alloc(0x390, ...) in camera/cameracontroller.cpp): Idle, Mid
//                                           Right, Mid Left, Manual, Lazy, Look Back, DEFAULT_2/3/4 Chase Camera
//                                           Algorithm. Bigger ones add fields after 0x390: Matrix/Direction/SPOKE
//                                           (0x3A0), SUPERPIPE/POST_RACE_1 (0x3B0), Relative (0x3C0), Spline (0x3D0).
//                                           cCamera embeds a cDEFAULT_3ChaseAlgorithm (0x390) at 0xC0.
// Retail vtables: cCameraAlgorithm cCameraAlgorithm_vtbl, cBasicCameraAlgorithm cBasicCameraAlgorithm_vtbl, cChaseAlgorithm cChaseAlgorithm_vtbl,
// cLookAtChaseAlgorithm cLookAtChaseAlgorithm_vtbl (stored by its ctor cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and dtor cLookAtChaseAlgorithm__cLookAtChaseAlgorithm).
//
// FLAT layouts, like main/game.h: every field at its retail offset, bases written out inline, no C++ inheritance.
// Evidence tags:
//   [R] retail constructors: cCameraAlgorithm's inline part (in cBasicCameraAlgorithm_cBasicCameraAlgorithm and
//       cChaseAlgorithm_cChaseAlgorithm = cChaseAlgorithm::cChaseAlgorithm), cLookAtChaseAlgorithm_cLookAtChaseAlgorithm (cLookAtChaseAlgorithm ctor), the
//       allocation sizes above;
//   [P] the preview's inline accessors via tools/layout_xlate.py (shouldSnapOnUpdate lw preview 0x300 -> retail 0x2F0);
//   [U] how retail code (camera/camera.cpp, 0x1622B0-0x168F60) uses the field;
//   [O] SSX On Tour's DWARF (classes cCameraAlgorithm, cBasicCameraAlgorithm, cChaseAlgorithm, cLookAtChaseAlgorithm,
//       cPIDFilter). cLookAtChaseAlgorithm's members are On Tour's, in On Tour's order, at On Tour's offsets up to
//       0x2CC; On Tour's nine 1-byte bools at 0x2D0 are words in retail, so every later member sits 0x18 further on
//       (On Tour's 0x380-byte class is 0x390 in retail). Every retail access lands on a field of its size and kind.

#include "layout.h"
#include "bx/math3d.h"
#include "bx/cubicspline.h"

struct cRiderCameraTarget;

// A PID controller (On Tour: cPIDFilter, 0x4C). Its methods are in camera/camera.cpp: cPIDFilter_reset = reset(value)
// (fills InputHistory/ResultHistory with value, zeroes Difference and NextFreeIndex); cPIDFilter_getDefaultProportionFactor/98/A8 return the
// default factors.
struct cPIDFilter {
    float InputHistory[5];      // 0x00 [O]; [U] cPIDFilter_reset stores the value into each
    float ResultHistory[5];     // 0x14 [O]; [U] cPIDFilter_reset stores the value into each
    float Difference[5];        // 0x28 [O]; [U] cPIDFilter_reset zeroes each
    int NextFreeIndex;          // 0x3C [O]; [U] cPIDFilter_reset zeroes it; cLookAtChaseAlgorithm_setupCamDirection indexes the histories by it
    float ProportionFactor;     // 0x40 [O]; [R] cLookAtChaseAlgorithm ctor (+0x380)
    float ErrorHistoryFactor;   // 0x44 [O]; [R] ditto (+0x384)
    float DerrivativeFactor;    // 0x48 [O] (EA's spelling); [R] ditto (+0x388)

    // Methods bound to their retail symbols by asm labels.
    float getDefaultErrorHistoryFactor() __asm__("cPIDFilter_getDefaultErrorHistoryFactor");
    float getDefaultDerrivativeFactor() __asm__("cPIDFilter_getDefaultDerrivativeFactor");
    void reset(float value) __asm__("cPIDFilter_reset");
};

CHECK_OFFSET(cPIDFilter, InputHistory, 0x00);
CHECK_OFFSET(cPIDFilter, ResultHistory, 0x14);
CHECK_OFFSET(cPIDFilter, Difference, 0x28);
CHECK_OFFSET(cPIDFilter, NextFreeIndex, 0x3C);
CHECK_OFFSET(cPIDFilter, ProportionFactor, 0x40);
CHECK_OFFSET(cPIDFilter, ErrorHistoryFactor, 0x44);
CHECK_OFFSET(cPIDFilter, DerrivativeFactor, 0x48);
CHECK_SIZE(cPIDFilter, 0x4C);

// The per-frame chase input of the look-at chase cameras (On Tour struct cChaseSetup, size 0x60; retail 0x70: its four
// bools are words, 0x58-0x67, and the t4Vector members make it 16-byte aligned). Each chase algorithm's update builds
// one on the stack and fills it with cLookAtChaseAlgorithm_setupChaseData (On Tour setupChaseData(cChaseSetup&
// chaseSetup_outArg, ...)), then hands it to setupHandplantBlend / setupHalfPipeCam / updateCamTarget /
// setupCamDirection / updateFollowDistance (On Tour: const cChaseSetup& setup). [Staged by views group A.]
// Tags: [O] On Tour's DWARF (member names, order and types); [U] retail cLookAtChaseAlgorithm_setupChaseData and the
// setup steps (src/camera/camera.cpp: the views sCamInfo_001607E0 / sCamInfo_1728 / sCamInfo, the 0x68-byte
// sCamInfo_* blobs in src/camera/trigger/cameratriggerfactory.cpp).
struct cChaseSetup {
    t4Vector SavedPos;              // 0x00 [O]; [U] setupChaseData stores Position (the algorithm's +0x40)
    t4Vector TPos;                  // 0x10 [O]; [U] setupChaseData stores the target's position (Target slot 1)
    t4Vector TVel;                  // 0x20 [O]; [U] setupChaseData stores the smoothed velocity; the updates read its
                                    //      horizontal direction (sCamInfo_001607E0::dir)
    t4Vector CameraDirection;       // 0x30 [O]; [U] cLookAtChaseAlgorithm_setupCamDirection writes it back
    int MotionState;                // 0x40 [O] eMotionState; [U] setupChaseData stores cRider_getMotionState
    float TargetSpeed;              // 0x44 [O]; [U] setupChaseData stores the length of TVel
    float HandplantBlendOut;        // 0x48 [O]; [U] cLookAtChaseAlgorithm_setupHandplantBlend stores the blend-out weight
    float Boost;                    // 0x4C [O]; [U] setupChaseData stores the boost (Target slot 17), copied to Setup_Boost
    float VisualBoost;              // 0x50 [O]; [U] setupChaseData: (Boost + 1) / 2 when Boost > 0, else 0; copied to
                                    //      Setup_VisualBoost
    float Crouch;                   // 0x54 [O]; [U] setupChaseData stores the crouch (Target slot 18)
    int WipedOut;                   // 0x58 [O] bool (a word in retail); [U] setupChaseData: MotionState == 2 (MS_Wipeout)
    int Handplanting;               // 0x5C [O] bool; [U] setupChaseData: MotionState == 5 (MS_Handplant); copied to
                                    //      Setup_IsTargetHandplanting; setupHandplantBlend tests it
    int Airborne;                   // 0x60 [O] bool; [U] setupChaseData: MotionState == 1 (MS_Air)
    int RailSliding;                // 0x64 [O] bool; [U] setupChaseData: MotionState == 4 (MS_Railslide)
    char unk_068[0x8];              // 0x68 padding to the 16-byte alignment (the callers reserve 0x70)
};

CHECK_OFFSET(cChaseSetup, SavedPos, 0x00);
CHECK_OFFSET(cChaseSetup, TPos, 0x10);
CHECK_OFFSET(cChaseSetup, TVel, 0x20);
CHECK_OFFSET(cChaseSetup, CameraDirection, 0x30);
CHECK_OFFSET(cChaseSetup, MotionState, 0x40);
CHECK_OFFSET(cChaseSetup, TargetSpeed, 0x44);
CHECK_OFFSET(cChaseSetup, HandplantBlendOut, 0x48);
CHECK_OFFSET(cChaseSetup, Boost, 0x4C);
CHECK_OFFSET(cChaseSetup, VisualBoost, 0x50);
CHECK_OFFSET(cChaseSetup, Crouch, 0x54);
CHECK_OFFSET(cChaseSetup, WipedOut, 0x58);
CHECK_OFFSET(cChaseSetup, Handplanting, 0x5C);
CHECK_OFFSET(cChaseSetup, Airborne, 0x60);
CHECK_OFFSET(cChaseSetup, RailSliding, 0x64);
CHECK_SIZE(cChaseSetup, 0x70);

// The pitch-blending settings a chase algorithm gives its final phase (On Tour struct cRaceCamFinalPhaseData, 0x14).
// cLookAtChaseAlgorithm_doRaceCamFinalPhase fills one through vtable slot 7 (getRaceCamFinalPhaseData; On Tour
// getRaceCamFinalPhaseData(cRaceCamFinalPhaseData* data_outArg)) and, when DoPitchBlending is set, passes the four
// floats to cLookAtChaseAlgorithm_updatePitchBlending. [Staged by views group A.]
struct cRaceCamFinalPhaseData {
    float PitchBlendingMaxAngleUp;      // 0x00 [O]; [U] doRaceCamFinalPhase: updatePitchBlending's 1st float
    float PitchBlendingMaxAngleDown;    // 0x04 [O]; [U] 2nd float
    float PitchBlendingSmoothnessUp;    // 0x08 [O]; [U] 3rd float
    float PitchBlendingSmoothnessDown;  // 0x0C [O]; [U] 4th float
    int DoPitchBlending;                // 0x10 [O] bool (a word in retail); [U] doRaceCamFinalPhase tests it (lw);
                                        //      the field-less overrides (cMatrixChaseAlgorithm_getRaceCamFinalPhaseData ...)
                                        //      store 0 there (sw 0x10)
};

CHECK_OFFSET(cRaceCamFinalPhaseData, PitchBlendingMaxAngleUp, 0x00);
CHECK_OFFSET(cRaceCamFinalPhaseData, PitchBlendingMaxAngleDown, 0x04);
CHECK_OFFSET(cRaceCamFinalPhaseData, PitchBlendingSmoothnessUp, 0x08);
CHECK_OFFSET(cRaceCamFinalPhaseData, PitchBlendingSmoothnessDown, 0x0C);
CHECK_OFFSET(cRaceCamFinalPhaseData, DoPitchBlending, 0x10);
CHECK_SIZE(cRaceCamFinalPhaseData, 0x14);

// cCameraAlgorithm (On Tour: 0x14). Abstract; its defaults are cCameraAlgorithm__cCameraAlgorithm (dtor) .. cCameraAlgorithm_shouldSnapOnUpdate.
struct cCameraAlgorithm {
    float View;                 // 0x00 [O]; [R] = getDefaultView() (cCameraAlgorithm_getDefaultView) in every ctor
    float Near;                 // 0x04 [O]; [R] 10.0f
    float Far;                  // 0x08 [O]; [R] 30000.0f
    int AlgorithmType;          // 0x0C [O] eCamAlgoType; [R] 0x4E, then each ctor stores its own (cBasic 0x4F,
                                //      cChase 0x53, cLookAtChase 0x54); [U] cCameraAlgorithm_setAlgorithmType setAlgorithmType (sw),
                                //      cCameraAlgorithm_getType getType (lw)
    // 0x10: the vptr. [R] cCameraAlgorithm_vtbl, then each ctor's own vtable
    // Slots: names [R] the vtables (cCameraAlgorithm_vtbl; getPose is pure there, cBasicCameraAlgorithm_getPose and
    // cLookAtChaseAlgorithm_getPose fill it), types [O] On Tour's cCameraAlgorithm / cBasicCameraAlgorithm::getPose
    // (retail has getPose at 2 and no renderDebugInfo; the rest keep On Tour's order).
    virtual ~cCameraAlgorithm();                                                    // 1
    virtual tPose getPose() = 0;                                                    // 2
    virtual void snap(int subjectID);                                               // 3
    virtual void snapCurrent();                                                     // 4
    virtual void update();                                                          // 5
    virtual t4Vector getTargetPos();                                                // 6
    virtual void getRaceCamFinalPhaseData(cRaceCamFinalPhaseData* data_outArg);     // 7
    virtual void doRaceCamFinalPhase(int ignoreCollisions);                         // 8 (On Tour: bool)
    virtual void updateAlgorithm(int ignoreCollisions);                             // 9 (bool)
    virtual void notifyMoveZ(int didMoveZ);                                         // 10 (bool)
    virtual bool shouldSnapOnUpdate();                                              // 11

    // Methods bound to their retail symbols by asm labels.
    float getDefaultView() __asm__("cCameraAlgorithm_getDefaultView");
    void setAlgorithmType(int algoType) __asm__("cCameraAlgorithm_setAlgorithmType");
    int getType() __asm__("cCameraAlgorithm_getType");
};

CHECK_OFFSET(cCameraAlgorithm, View, 0x00);
CHECK_OFFSET(cCameraAlgorithm, Near, 0x04);
CHECK_OFFSET(cCameraAlgorithm, Far, 0x08);
CHECK_OFFSET(cCameraAlgorithm, AlgorithmType, 0x0C);
CHECK_SIZE(cCameraAlgorithm, 0x14);

// cBasicCameraAlgorithm (On Tour: 0x40): a camera at a position with a pitch and heading.
struct cBasicCameraAlgorithm : public cCameraAlgorithm {
    // cCameraAlgorithm (camera/cameraalgorithms.h, 0x14): the vptr at 0x10:
    //   [R] cBasicCameraAlgorithm_vtbl (cManual: D_0045BD10, cEditor: D_0045BCA8)
    virtual tPose getPose();                                                        // 2 ([R] cBasicCameraAlgorithm_vtbl: cBasicCameraAlgorithm_getPose)
    char unk_14[0xC];           // 0x14 alignment gap before the 16-byte-aligned t4Vector
    // --- cBasicCameraAlgorithm
    t4Vector Position;          // 0x20 [O]; [R] = t4vect_Origin; [U] cBasicCameraAlgorithm_setPosition setPosition (sq), cBasicCameraAlgorithm_getPosition
                                //      getPosition (lq)
    float Pitch;                // 0x30 [O]; [P] getPitch/setPitch (preview 0x30); [R] 0; [U] cBasicCameraAlgorithm_setPitch (swc1),
                                //      cBasicCameraAlgorithm_getPitch (lwc1)
    float Heading;              // 0x34 [O]; [P] getHeading/setHeading (preview 0x34); [R] 0; [U] cBasicCameraAlgorithm_setHeading,
                                //      cBasicCameraAlgorithm_getHeading
    char unk_38[0x8];           // 0x38 padding to the 16-byte class alignment

    // Methods bound to their retail symbols by asm labels.
    void setPosition(long long *position) __asm__("cBasicCameraAlgorithm_setPosition");
    void setPitch(float fparg0) __asm__("cBasicCameraAlgorithm_setPitch");
    void setHeading(float fparg0) __asm__("cBasicCameraAlgorithm_setHeading");
    float getPitch() __asm__("cBasicCameraAlgorithm_getPitch");
    float getHeading() __asm__("cBasicCameraAlgorithm_getHeading");
};

CHECK_OFFSET(cBasicCameraAlgorithm, Position, 0x20);
CHECK_OFFSET(cBasicCameraAlgorithm, Pitch, 0x30);
CHECK_OFFSET(cBasicCameraAlgorithm, Heading, 0x34);
CHECK_SIZE(cBasicCameraAlgorithm, 0x40);

// cManualCameraAlgorithm / cEditorCameraAlgorithm: the two free cameras a cManualCameraController owns
// (camera/cameracontroller.h ManualAlgo / EditorAlgo). Both add no fields to cBasicCameraAlgorithm: [R]
// cManualCameraController_cManualCameraController allocates each with cMemMan_alloc(0x40, "Manual Cam Algo" /
// "Editor Cam Algo") and runs cManualCameraAlgorithm_cManualCameraAlgorithm /
// cEditorCameraAlgorithm_cEditorCameraAlgorithm (vtables D_0045BD10 / D_0045BCA8); [U] the controller's dtor deletes
// them (slot 1), snap / snapCurrent call slots 3 / 4.
struct cManualCameraAlgorithm : public cBasicCameraAlgorithm {
};

struct cEditorCameraAlgorithm : public cBasicCameraAlgorithm {
};

CHECK_SIZE(cManualCameraAlgorithm, 0x40);
CHECK_SIZE(cEditorCameraAlgorithm, 0x40);

// cChaseAlgorithm (On Tour: 0x40): an algorithm that follows a rider.
struct cChaseAlgorithm : public cCameraAlgorithm {
    // cCameraAlgorithm (camera/cameraalgorithms.h, 0x14): the vptr at 0x10:
    //   [R] cChaseAlgorithm_vtbl
    // cChaseAlgorithm's own slots ([R] cChaseAlgorithm_vtbl; types [O] On Tour's class cChaseAlgorithm):
    virtual void setTarget(cRiderCameraTarget* camTarget);                         // 12
    char unk_14[0xC];           // 0x14 alignment gap
    // --- cChaseAlgorithm
    t4Vector CamTargetPos;      // 0x20 [O]; [U] cChaseAlgorithm_getTargetPos returns it (lq); the chase algorithms
                                //      write it (sq) and read it (lqc2)
    cRiderCameraTarget* Target; // 0x30 [O]; [P] (preview verify reads 0x30); [U] cChaseAlgorithm_setTarget setTarget (sw; cCamera_init
                                //      passes the "RiderCameraTarget"); the algorithms call its virtuals
    char unk_34[0xC];           // 0x34 padding to the 16-byte class alignment
};

CHECK_OFFSET(cChaseAlgorithm, CamTargetPos, 0x20);
CHECK_OFFSET(cChaseAlgorithm, Target, 0x30);
CHECK_SIZE(cChaseAlgorithm, 0x40);

// cLookAtChaseAlgorithm (retail 0x390: the 0x390-byte allocations of its field-less subclasses, and cCamera's
// embedded cDEFAULT_3ChaseAlgorithm spanning 0xC0..0x450).
struct cLookAtChaseAlgorithm : public cChaseAlgorithm {
    // cChaseAlgorithm (camera/cameraalgorithms.h, 0x40): the vptr at 0x10:
    //   [R] cLookAtChaseAlgorithm_vtbl; [U] the virtual calls (updateAlgorithm cLookAtChaseAlgorithm_updateAlgorithm
    //       ...)
    // --- cLookAtChaseAlgorithm
    // cLookAtChaseAlgorithm's own slots ([R] cLookAtChaseAlgorithm_vtbl; types [O] On Tour's class cLookAtChaseAlgorithm):
    virtual tPose getPose();                                                        // 2 ([R] cLookAtChaseAlgorithm_vtbl: cLookAtChaseAlgorithm_getPose)
    virtual float getRestDrop() = 0;                                                // 13 ([R] pure here; cManualChaseAlgorithm_getRestDrop)
    t4Vector Position;              // 0x040 [O]; [R] = t4vect_Origin; [U] lq/sq/lqc2, .z/.w by lwc1/swc1
    float Heading;                  // 0x050 [O]; [R] 0; [U] lwc1/swc1 (cLookAtChaseAlgorithm_calcPitchHeading, cLookAtChaseAlgorithm_calcLookVectorFromPitchAndHeading)
    float Pitch;                    // 0x054 [O]; [R] 0; [U] lwc1/swc1 (cLookAtChaseAlgorithm_updatePitchBlending, cLookAtChaseAlgorithm_calcPitchHeading)
    float CamRoll;                  // 0x058 [O]; [R] 0
    char unk_05C[0x4];              // 0x05C alignment gap
    tPose CalculatedPose;           // 0x060 [O]; [U] cLookAtChaseAlgorithm_calcPose writes P and Q (sq), reads Q.x..w
    t4Vector LastCamDir;            // 0x080 [O]; [U] sq/lqc2
    t4Vector LastVelLookahead;      // 0x090 [O]; [R] = t4vect_Zero
    t4Vector LastRiderPos;          // 0x0A0 [O]; [U] cLookAtChaseAlgorithm_updateHandplant
    t4Vector LastHandplantLook;     // 0x0B0 [O]; [U] cLookAtChaseAlgorithm_updateHandplant
    t4Vector LastRailMove;          // 0x0C0 [O]; [R] = t4vect_Zero
    t4Vector LastHalfPipeMove;      // 0x0D0 [O]; [R] = t4vect_Zero
    t4Vector LastHalfPipeDistCorrection; // 0x0E0 [O]; [R] = t4vect_Zero
    t4Vector LastLookaheadSkew;     // 0x0F0 [O]; [U] cLookAtChaseAlgorithm_snap clears it
    t4Vector LastHorizontalVel;     // 0x100 [O]; [U] lq/sq (cLookAtChaseAlgorithm_setupChaseData), .z by sw in snap
    t4Vector HalfPipeInboundLook;   // 0x110 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    t4Vector HalfPipeExitLook;      // 0x120 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam, .z lwc1/swc1
    t4Vector HalfPipeRotator;       // 0x130 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam, .x/.y/.z lwc1
    t4Vector HalfPipeNewLook;       // 0x140 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    t4Vector HalfPipeFullOut;       // 0x150 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    t4Vector HalfPipeOut;           // 0x160 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    t4Vector HalfPipeBlendOutLastLook; // 0x170 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam, snap
    t4Vector HalfPipeBlendOutLastTargetDelta; // 0x180 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    t4Vector LastSlowVelocity;      // 0x190 [O]; [U] snap
    t4Vector SlowCamEnterLook;      // 0x1A0 [O]; [U] snap
    t4Vector LastOriginalVel;       // 0x1B0 [O]; [U] cLookAtChaseAlgorithm_setupChaseData, snap
    t4Vector LastDirectionVel;      // 0x1C0 [O]; [U] cLookAtChaseAlgorithm_setupCamDirection, snap
    float LastDist;                 // 0x1D0 [O]; [R] 200.0f; [U] cLookAtChaseAlgorithm_updateFollowDistance
    float LastDrop;                 // 0x1D4 [O]; [R] 0; [U] cLookAtChaseAlgorithm_updateDropWithSpeed
    float LastPitch;                // 0x1D8 [O]; [R] = Pitch; [U] cLookAtChaseAlgorithm_updatePitchBlending
    float LastViewScalar;           // 0x1DC [O]; [R] 1.0f; [U] cLookAtChaseAlgorithm_updateFOVWithSpeed
    float SecondsSinceLastHandplant; // 0x1E0 [O]; [R] 10.0f; [U] cLookAtChaseAlgorithm_updateHandplant (lwc1/swc1; one sw)
    float LastHeight;               // 0x1E4 [O]; [R] 0; [U] cLookAtChaseAlgorithm_updateCamTargetLate
    float ExtraBoostDist;           // 0x1E8 [O]; [U] cLookAtChaseAlgorithm_updateFollowDistance
    float LastBoostWarp;            // 0x1EC [O]; [U] cLookAtChaseAlgorithm_updateFollowDistance
    float LastCameraSwing;          // 0x1F0 [O]; [U] snap zeroes it (sw)
    float AirCamTimeInAir;          // 0x1F4 [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float AirSplineScalar;          // 0x1F8 [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float AirTimeScalar;            // 0x1FC [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float AirCamMaxTime;            // 0x200 [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float LastCrouchFactor;         // 0x204 [O]; [U] cLookAtChaseAlgorithm_updateDropWithSpeed
    float LastTargetCrouchFactor;   // 0x208 [O]; [U] cLookAtChaseAlgorithm_updateCamTargetLate
    float LastAirTargetDelta;       // 0x20C [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float LastAirPositionDelta;     // 0x210 [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float ViewCamLastFactor;        // 0x214 [O]; [U] cLookAtChaseAlgorithm_updateViewCam
    float FullAirCamTimeInAir;      // 0x218 [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float LastFullAirTargetDelta;   // 0x21C [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float LastFullAirPositionDelta; // 0x220 [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float AirExitSmoothness;        // 0x224 [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float VerticalJumpFactor;       // 0x228 [O]; [U] cLookAtChaseAlgorithm_updateAirCam
    float HalfPipeInitWait;         // 0x22C [O]; [U] cLookAtChaseAlgorithm_setupHalfPipeCam, cLookAtChaseAlgorithm_updateHalfPipeCam
    float HalfPipeTimeToLand;       // 0x230 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    float HalfPipeTimeInAir;        // 0x234 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    float HalfPipeRotatorAngle;     // 0x238 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    float HalfPipeBlendOutTimeLeft; // 0x23C [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    float HalfPipeCrossBlendTime;   // 0x240 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    float HalfPipeCrossBlendFullTime; // 0x244 [O]; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    float SlowCamBlend;             // 0x248 [O]; [U] cLookAtChaseAlgorithm_updateSlowSpeedCam (updateSlowSpeedCam; one sw zero)
    float LastHorizontalMoveLen;    // 0x24C [O]; [U] snap
    float LastEnemyPullback;        // 0x250 [O]; [U] snap
    float ShakeNextXTime;           // 0x254 [O]; [U] cLookAtChaseAlgorithm_setupCameraShake, cLookAtChaseAlgorithm_updateCameraShake
    float ShakeNextYTime;           // 0x258 [O]; [U] ditto
    float ShakeNextZTime;           // 0x25C [O]; [U] ditto
    float ShakeXTarget;             // 0x260 [O]; [U] ditto
    float ShakeYTarget;             // 0x264 [O]; [U] ditto
    float ShakeZTarget;             // 0x268 [O]; [U] ditto
    float ShakeLastXTarget;         // 0x26C [O]; [U] ditto
    float ShakeLastYTarget;         // 0x270 [O]; [U] ditto
    float ShakeLastZTarget;         // 0x274 [O]; [U] ditto
    float ShakeXCounter;            // 0x278 [O]; [U] ditto
    float ShakeYCounter;            // 0x27C [O]; [U] ditto
    float ShakeZCounter;            // 0x280 [O]; [U] ditto
    float SecondaryShakeNextXTime;  // 0x284 [O]; [U] ditto
    float SecondaryShakeNextYTime;  // 0x288 [O]; [U] ditto
    float SecondaryShakeNextZTime;  // 0x28C [O]; [U] ditto
    float SecondaryShakeXTarget;    // 0x290 [O]; [U] ditto
    float SecondaryShakeYTarget;    // 0x294 [O]; [U] ditto
    float SecondaryShakeZTarget;    // 0x298 [O]; [U] ditto
    float SecondaryShakeLastXTarget; // 0x29C [O]; [U] ditto
    float SecondaryShakeLastYTarget; // 0x2A0 [O]; [U] ditto
    float SecondaryShakeLastZTarget; // 0x2A4 [O]; [U] ditto
    float SecondaryShakeXCounter;   // 0x2A8 [O]; [U] ditto
    float SecondaryShakeYCounter;   // 0x2AC [O]; [U] ditto
    float SecondaryShakeZCounter;   // 0x2B0 [O]; [U] ditto
    float ShakeBlendOutTime;        // 0x2B4 [O]; [U] ditto
    float LastHandplantRise;        // 0x2B8 [O]; [U] cLookAtChaseAlgorithm_updateHandplant
    float SteerTime;                // 0x2BC [O]; [U] snap
    int FramesSinceHandplant;       // 0x2C0 [O]; [R] 5000; [U] cLookAtChaseAlgorithm_setupHandplantBlend
    int AirCamSetupCountdown;       // 0x2C4 [O]; [U] cLookAtChaseAlgorithm_setupHalfPipeCam, cLookAtChaseAlgorithm_updateAirCam
    int JiggleFrameCount;           // 0x2C8 [O]; [U] snap
    int LastAirFrame;               // 0x2CC [O]; [U] cLookAtChaseAlgorithm_setupHalfPipeCam
    int DoingHalfPipeCam;           // 0x2D0 [O] bool (a word in retail); [R] 0; [U] cLookAtChaseAlgorithm_setupHalfPipeCam, cLookAtChaseAlgorithm_updateAirCam,
                                    //       cLookAtChaseAlgorithm_updateHalfPipeCam
    int AirborneLastFrame;          // 0x2D4 [O] bool; [U] cLookAtChaseAlgorithm_setupHalfPipeCam (lw/sw)
    int HalfPipeCamInitialized;     // 0x2D8 [O] bool; [R] 0; [U] cLookAtChaseAlgorithm_setupHalfPipeCam, cLookAtChaseAlgorithm_updateHalfPipeCam
    int BoostingLastFrame;          // 0x2DC [O] bool; [U] snap
    int ApplyingAirFunc;            // 0x2E0 [O] bool; [U] cLookAtChaseAlgorithm_setupHalfPipeCam, cLookAtChaseAlgorithm_updateAirCam
    int HitGroundSinceStartingAirFunc; // 0x2E4 [O] bool; [U] cLookAtChaseAlgorithm_updateAirCam
    int ApplyingFullAirFunc;        // 0x2E8 [O] bool; [U] cLookAtChaseAlgorithm_updateAirCam
    int ShakingLastFrame;           // 0x2EC [O] bool; [U] cLookAtChaseAlgorithm_setupCameraShake, cLookAtChaseAlgorithm_updateCameraShake
    int DoSnap;                     // 0x2F0 [O] bool; [P] shouldSnapOnUpdate (lw); [U] updateAlgorithm cLookAtChaseAlgorithm_updateAlgorithm
                                    //       calls snapCurrent when set
    float Setup_Boost;              // 0x2F4 [O]; [U] cLookAtChaseAlgorithm_setupChaseData (swc1)
    float Setup_VisualBoost;        // 0x2F8 [O]; [U] cLookAtChaseAlgorithm_setupChaseData (swc1)
    int Setup_IsTargetHandplanting; // 0x2FC [O] bool; [U] cLookAtChaseAlgorithm_setupChaseData (sw)
    int SlowCamState;               // 0x300 [O] eCamAttributeBlendState; [U] cLookAtChaseAlgorithm_updateSlowSpeedCam (lw/sw)
    cCubicSplineInterpolant AirTargetSpline; // 0x304 [O] cCubicSplineInterpolant (bx/cubicspline.h: bitfield word, MaxX,
                                    //       CubicSplines); [R] constructed by cCubicSplineInterpolant_cCubicSplineInterpolant, destroyed by cCubicSplineInterpolant__cCubicSplineInterpolant
    cCubicSplineInterpolant AirPositionSpline; // 0x310 [O] cCubicSplineInterpolant; [R] ditto; [U] cLookAtChaseAlgorithm_updateAirCam (initAsNatural / addPoint / evaluate)
    cCubicSplineInterpolant FullAirTargetSpline; // 0x31C [O] cCubicSplineInterpolant; [R] ditto; [U] cLookAtChaseAlgorithm_updateAirCam
    cCubicSplineInterpolant FullAirPositionSpline; // 0x328 [O] cCubicSplineInterpolant; [R] ditto; [U] cLookAtChaseAlgorithm_updateAirCam
    cCubicSplineInterpolant HalfPipeAngleSpline; // 0x334 [O] cCubicSplineInterpolant; [R] ditto; [U] cLookAtChaseAlgorithm_updateHalfPipeCam
    cPIDFilter TurningPIDFilter;    // 0x340 [O]; [R] the ctor sets its three factors and calls cPIDFilter_reset;
                                    //       [U] cLookAtChaseAlgorithm_setupCamDirection runs the filter inline
    char unk_38C[0x4];              // 0x38C padding to the 16-byte class alignment

    // Methods bound to their retail symbols by asm labels.
    void setupHandplantBlend(cChaseSetup* obj) __asm__("cLookAtChaseAlgorithm_setupHandplantBlend");
    void setupHalfPipeCam(cChaseSetup* trig) __asm__("cLookAtChaseAlgorithm_setupHalfPipeCam");
    void updateHandplant(cChaseSetup* trig, float distanceAlongTangent, float distanceAboveTangent,
                         float speedToTarget, float exitDelay, float rise, float riseEnterSmoothness,
                         float riseExitSmoothness)
        __asm__("cLookAtChaseAlgorithm_updateHandplant");
    void updateSlowSpeedCam(cChaseSetup* info, float slowSpeed, float blendInTime, float blendOutTime)
        __asm__("cLookAtChaseAlgorithm_updateSlowSpeedCam");
    void updateRotation(float ang) __asm__("cLookAtChaseAlgorithm_updateRotation");
    void setupCameraShake(float base, float spread, float spatialVariance)
        __asm__("cLookAtChaseAlgorithm_setupCameraShake");
    void updateCameraShake(int flag, int sel, float base, float spread, float spatialVariance)
        __asm__("cLookAtChaseAlgorithm_updateCameraShake");
    void updatePitchBlending(float x, float y, float z, float w) __asm__("cLookAtChaseAlgorithm_updatePitchBlending");
    void setupInitialCamDir(float backDist, float height) __asm__("cLookAtChaseAlgorithm_setupInitialCamDir");
};

CHECK_OFFSET(cLookAtChaseAlgorithm, Position, 0x040);
CHECK_OFFSET(cLookAtChaseAlgorithm, Heading, 0x050);
CHECK_OFFSET(cLookAtChaseAlgorithm, Pitch, 0x054);
CHECK_OFFSET(cLookAtChaseAlgorithm, CamRoll, 0x058);
CHECK_OFFSET(cLookAtChaseAlgorithm, CalculatedPose, 0x060);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastCamDir, 0x080);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastVelLookahead, 0x090);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastRiderPos, 0x0A0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastHandplantLook, 0x0B0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastRailMove, 0x0C0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastHalfPipeMove, 0x0D0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastHalfPipeDistCorrection, 0x0E0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastLookaheadSkew, 0x0F0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastHorizontalVel, 0x100);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeInboundLook, 0x110);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeExitLook, 0x120);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeRotator, 0x130);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeNewLook, 0x140);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeFullOut, 0x150);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeOut, 0x160);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeBlendOutLastLook, 0x170);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeBlendOutLastTargetDelta, 0x180);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastSlowVelocity, 0x190);
CHECK_OFFSET(cLookAtChaseAlgorithm, SlowCamEnterLook, 0x1A0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastOriginalVel, 0x1B0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastDirectionVel, 0x1C0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastDist, 0x1D0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastDrop, 0x1D4);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastPitch, 0x1D8);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastViewScalar, 0x1DC);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondsSinceLastHandplant, 0x1E0);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastHeight, 0x1E4);
CHECK_OFFSET(cLookAtChaseAlgorithm, ExtraBoostDist, 0x1E8);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastBoostWarp, 0x1EC);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastCameraSwing, 0x1F0);
CHECK_OFFSET(cLookAtChaseAlgorithm, AirCamTimeInAir, 0x1F4);
CHECK_OFFSET(cLookAtChaseAlgorithm, AirSplineScalar, 0x1F8);
CHECK_OFFSET(cLookAtChaseAlgorithm, AirTimeScalar, 0x1FC);
CHECK_OFFSET(cLookAtChaseAlgorithm, AirCamMaxTime, 0x200);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastCrouchFactor, 0x204);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastTargetCrouchFactor, 0x208);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastAirTargetDelta, 0x20C);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastAirPositionDelta, 0x210);
CHECK_OFFSET(cLookAtChaseAlgorithm, ViewCamLastFactor, 0x214);
CHECK_OFFSET(cLookAtChaseAlgorithm, FullAirCamTimeInAir, 0x218);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastFullAirTargetDelta, 0x21C);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastFullAirPositionDelta, 0x220);
CHECK_OFFSET(cLookAtChaseAlgorithm, AirExitSmoothness, 0x224);
CHECK_OFFSET(cLookAtChaseAlgorithm, VerticalJumpFactor, 0x228);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeInitWait, 0x22C);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeTimeToLand, 0x230);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeTimeInAir, 0x234);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeRotatorAngle, 0x238);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeBlendOutTimeLeft, 0x23C);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeCrossBlendTime, 0x240);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeCrossBlendFullTime, 0x244);
CHECK_OFFSET(cLookAtChaseAlgorithm, SlowCamBlend, 0x248);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastHorizontalMoveLen, 0x24C);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastEnemyPullback, 0x250);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeNextXTime, 0x254);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeNextYTime, 0x258);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeNextZTime, 0x25C);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeXTarget, 0x260);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeYTarget, 0x264);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeZTarget, 0x268);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeLastXTarget, 0x26C);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeLastYTarget, 0x270);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeLastZTarget, 0x274);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeXCounter, 0x278);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeYCounter, 0x27C);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeZCounter, 0x280);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeNextXTime, 0x284);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeNextYTime, 0x288);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeNextZTime, 0x28C);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeXTarget, 0x290);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeYTarget, 0x294);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeZTarget, 0x298);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeLastXTarget, 0x29C);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeLastYTarget, 0x2A0);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeLastZTarget, 0x2A4);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeXCounter, 0x2A8);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeYCounter, 0x2AC);
CHECK_OFFSET(cLookAtChaseAlgorithm, SecondaryShakeZCounter, 0x2B0);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakeBlendOutTime, 0x2B4);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastHandplantRise, 0x2B8);
CHECK_OFFSET(cLookAtChaseAlgorithm, SteerTime, 0x2BC);
CHECK_OFFSET(cLookAtChaseAlgorithm, FramesSinceHandplant, 0x2C0);
CHECK_OFFSET(cLookAtChaseAlgorithm, AirCamSetupCountdown, 0x2C4);
CHECK_OFFSET(cLookAtChaseAlgorithm, JiggleFrameCount, 0x2C8);
CHECK_OFFSET(cLookAtChaseAlgorithm, LastAirFrame, 0x2CC);
CHECK_OFFSET(cLookAtChaseAlgorithm, DoingHalfPipeCam, 0x2D0);
CHECK_OFFSET(cLookAtChaseAlgorithm, AirborneLastFrame, 0x2D4);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeCamInitialized, 0x2D8);
CHECK_OFFSET(cLookAtChaseAlgorithm, BoostingLastFrame, 0x2DC);
CHECK_OFFSET(cLookAtChaseAlgorithm, ApplyingAirFunc, 0x2E0);
CHECK_OFFSET(cLookAtChaseAlgorithm, HitGroundSinceStartingAirFunc, 0x2E4);
CHECK_OFFSET(cLookAtChaseAlgorithm, ApplyingFullAirFunc, 0x2E8);
CHECK_OFFSET(cLookAtChaseAlgorithm, ShakingLastFrame, 0x2EC);
CHECK_OFFSET(cLookAtChaseAlgorithm, DoSnap, 0x2F0);
CHECK_OFFSET(cLookAtChaseAlgorithm, Setup_Boost, 0x2F4);
CHECK_OFFSET(cLookAtChaseAlgorithm, Setup_VisualBoost, 0x2F8);
CHECK_OFFSET(cLookAtChaseAlgorithm, Setup_IsTargetHandplanting, 0x2FC);
CHECK_OFFSET(cLookAtChaseAlgorithm, SlowCamState, 0x300);
CHECK_OFFSET(cLookAtChaseAlgorithm, AirTargetSpline, 0x304);
CHECK_OFFSET(cLookAtChaseAlgorithm, AirPositionSpline, 0x310);
CHECK_OFFSET(cLookAtChaseAlgorithm, FullAirTargetSpline, 0x31C);
CHECK_OFFSET(cLookAtChaseAlgorithm, FullAirPositionSpline, 0x328);
CHECK_OFFSET(cLookAtChaseAlgorithm, HalfPipeAngleSpline, 0x334);
CHECK_OFFSET(cLookAtChaseAlgorithm, TurningPIDFilter, 0x340);
CHECK_SIZE(cLookAtChaseAlgorithm, 0x390);

#endif /* CAMERA_CAMERAALGORITHMS_H */
