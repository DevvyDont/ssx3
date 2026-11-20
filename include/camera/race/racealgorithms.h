#ifndef CAMERA_RACE_RACEALGORITHMS_H
#define CAMERA_RACE_RACEALGORITHMS_H

// The race chase-camera algorithms that add members to cLookAtChaseAlgorithm (camera/cameraalgorithms.h, 0x390):
//   cMatrixChaseAlgorithm   (0x3A0)  "Matrix Chase Camera Algorithm"   camera type 41
//   cRelativeChaseAlgorithm (0x3C0)  "Relative Chase Camera Algorithm" camera types 33..40 (angle = (type-33)*pi/4)
//   cSplineCamAlgorithm     (0x3D0)  "Spline Camera Algorithm"         camera type 92
// Sizes [R]: cChaseCameraController_createChaseAlgorithmBlend (camera/cameraalgorithms.cpp) allocates them with
// cMemMan_alloc(size, tag) and calls their constructors (cMatrixChaseAlgorithm_cMatrixChaseAlgorithm, cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(mem, angle),
// cSplineCamAlgorithm_cSplineCamAlgorithm(mem, p1..p4, time, leadDistance, heightOffset, fov)).
// Class names: the July 2003 preview's vtable slots (builds/x/v2/name_map_v2.tsv: cSplineCamAlgorithm::<virtual slot 1>
// = cSplineCamAlgorithm__cSplineCamAlgorithm ...) and SSX On Tour's DWARF, whose functions.tsv lists ctor, dtor, snap(int), update(),
// getRestDrop(), getRaceCamFinalPhaseData(cRaceCamFinalPhaseData*) for each (z:/main/game/source/camera/
// all_in_one_camera.cpp). EA's PS2 source file is not known: the preview names camera/race/racealgorithms.cpp,
// allspinchasealgo.cpp and directionchasealgo.cpp; these classes sit in retail between camera/cameraalgorithms' free
// cameras (0x15F938) and race/racecontroller's cChaseCameraController (0x161950), so racealgorithms.h is a guess.
// Field-less subclasses of cLookAtChaseAlgorithm in the same retail range (all 0x390; at the end of this file, each
// deriving from the base): cManualChaseAlgorithm, cIdleChaseAlgorithm, cLazyChaseAlgorithm, cLookBackChaseAlgorithm,
// cMidRightChaseAlgorithm, cMidLeftChaseAlgorithm. The DEFAULT_2/3/4 and Direction chase algorithms are in
// camera/race/directionchasealgo.h, SPOKE/SUPERPIPE/POST_RACE_1 in camera/race/allspinchasealgo.h.
//
// Retail vtables (13 slots: dtor, getPose, snap, snapCurrent, update, getTargetPos, getRaceCamFinalPhaseData,
// doRaceCamFinalPhase, updateAlgorithm, <pure>, shouldSnapOnUpdate, setTarget, getRestDrop):
//   cMatrixChaseAlgorithm cMatrixChaseAlgorithm_vtbl, cRelativeChaseAlgorithm cRelativeChaseAlgorithm_vtbl, cSplineCamAlgorithm cSplineCamAlgorithm_vtbl.
//
// FLAT layouts, like main/game.h: every field at its retail offset; the 0x390-byte cLookAtChaseAlgorithm base is
// copied in front of each (its evidence tags are cameraalgorithms.h's); no C++ inheritance. Own members: On Tour's
// names and order (On Tour's base is 0x380, so each sits 0x10 later in retail), placed by [R] the constructors and
// [U] the update functions.

#include "layout.h"
#include "bx/math3d.h"
#include "camera/cameraalgorithms.h"    // tPose via bx/math3d.h, cPIDFilter

struct cRiderCameraTarget;

// cMatrixChaseAlgorithm (0x3A0; On Tour 0x390). Ctor cMatrixChaseAlgorithm_cMatrixChaseAlgorithm (stores vtable cMatrixChaseAlgorithm_vtbl, AlgorithmType),
// dtor cMatrixChaseAlgorithm__cMatrixChaseAlgorithm, snap cMatrixChaseAlgorithm_snap, update cMatrixChaseAlgorithm_update, getRaceCamFinalPhaseData cMatrixChaseAlgorithm_getRaceCamFinalPhaseData,
// getRestDrop cMatrixChaseAlgorithm_getRestDrop.
struct cMatrixChaseAlgorithm {
    // --- cLookAtChaseAlgorithm (camera/cameraalgorithms.h, 0x390; its own bases cChaseAlgorithm <- cCameraAlgorithm
    //     inline), copied field for field
    // --- cCameraAlgorithm
    float View;                     // 0x000 [O]; [R] ctor: pi/4
    float Near;                     // 0x004 [O]; [R] 10.0f
    float Far;                      // 0x008 [O]; [R] 30000.0f
    int AlgorithmType;              // 0x00C [O]; [R] 0x54; subclass ctors store their own
    void** mVtbl;                   // 0x010 [R] cLookAtChaseAlgorithm_vtbl; [U] the virtual calls (updateAlgorithm cLookAtChaseAlgorithm_updateAlgorithm ...)
    char unk_014[0xC];              // 0x014 alignment gap
    // --- cChaseAlgorithm
    t4Vector CamTargetPos;          // 0x020 [O]; [U] sq/lqc2 in the update functions
    cRiderCameraTarget* Target;     // 0x030 [O]; [P]; [U] setTarget cChaseAlgorithm_setTarget; 15 functions read it
    char unk_034[0xC];              // 0x034 padding
    // --- cLookAtChaseAlgorithm
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
    char AirTargetSpline[0xC];      // 0x304 [O] cCubicSplineInterpolant (bx/cubicspline.h: bitfield word, MaxX,
                                    //       CubicSplines); [R] constructed by cCubicSplineInterpolant_cCubicSplineInterpolant, destroyed by cCubicSplineInterpolant__cCubicSplineInterpolant
    char AirPositionSpline[0xC];    // 0x310 [O] cCubicSplineInterpolant; [R] ditto
    char FullAirTargetSpline[0xC];  // 0x31C [O] cCubicSplineInterpolant; [R] ditto
    char FullAirPositionSpline[0xC]; // 0x328 [O] cCubicSplineInterpolant; [R] ditto
    char HalfPipeAngleSpline[0xC];  // 0x334 [O] cCubicSplineInterpolant; [R] ditto
    cPIDFilter TurningPIDFilter;    // 0x340 [O]; [R] the ctor sets its three factors and calls cPIDFilter_reset;
                                    //       [U] cLookAtChaseAlgorithm_setupCamDirection runs the filter inline
    char unk_38C[0x4];              // 0x38C padding to the 16-byte class alignment
    // --- cMatrixChaseAlgorithm
    float LastAngle;                // 0x390 [O] (On Tour 0x380); [R] = 0 (sw zero); [U] cMatrixChaseAlgorithm_update update (lwc1/swc1)
    char unk_394[0xC];              // 0x394 padding to the 16-byte class alignment (alloc 0x3A0)
};

CHECK_OFFSET(cMatrixChaseAlgorithm, View, 0x000);
CHECK_OFFSET(cMatrixChaseAlgorithm, Near, 0x004);
CHECK_OFFSET(cMatrixChaseAlgorithm, Far, 0x008);
CHECK_OFFSET(cMatrixChaseAlgorithm, AlgorithmType, 0x00C);
CHECK_OFFSET(cMatrixChaseAlgorithm, mVtbl, 0x010);
CHECK_OFFSET(cMatrixChaseAlgorithm, unk_014, 0x014);
CHECK_OFFSET(cMatrixChaseAlgorithm, CamTargetPos, 0x020);
CHECK_OFFSET(cMatrixChaseAlgorithm, Target, 0x030);
CHECK_OFFSET(cMatrixChaseAlgorithm, unk_034, 0x034);
CHECK_OFFSET(cMatrixChaseAlgorithm, Position, 0x040);
CHECK_OFFSET(cMatrixChaseAlgorithm, Heading, 0x050);
CHECK_OFFSET(cMatrixChaseAlgorithm, Pitch, 0x054);
CHECK_OFFSET(cMatrixChaseAlgorithm, CamRoll, 0x058);
CHECK_OFFSET(cMatrixChaseAlgorithm, unk_05C, 0x05C);
CHECK_OFFSET(cMatrixChaseAlgorithm, CalculatedPose, 0x060);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastCamDir, 0x080);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastVelLookahead, 0x090);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastRiderPos, 0x0A0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastHandplantLook, 0x0B0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastRailMove, 0x0C0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastHalfPipeMove, 0x0D0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastHalfPipeDistCorrection, 0x0E0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastLookaheadSkew, 0x0F0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastHorizontalVel, 0x100);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeInboundLook, 0x110);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeExitLook, 0x120);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeRotator, 0x130);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeNewLook, 0x140);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeFullOut, 0x150);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeOut, 0x160);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeBlendOutLastLook, 0x170);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeBlendOutLastTargetDelta, 0x180);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastSlowVelocity, 0x190);
CHECK_OFFSET(cMatrixChaseAlgorithm, SlowCamEnterLook, 0x1A0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastOriginalVel, 0x1B0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastDirectionVel, 0x1C0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastDist, 0x1D0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastDrop, 0x1D4);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastPitch, 0x1D8);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastViewScalar, 0x1DC);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondsSinceLastHandplant, 0x1E0);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastHeight, 0x1E4);
CHECK_OFFSET(cMatrixChaseAlgorithm, ExtraBoostDist, 0x1E8);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastBoostWarp, 0x1EC);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastCameraSwing, 0x1F0);
CHECK_OFFSET(cMatrixChaseAlgorithm, AirCamTimeInAir, 0x1F4);
CHECK_OFFSET(cMatrixChaseAlgorithm, AirSplineScalar, 0x1F8);
CHECK_OFFSET(cMatrixChaseAlgorithm, AirTimeScalar, 0x1FC);
CHECK_OFFSET(cMatrixChaseAlgorithm, AirCamMaxTime, 0x200);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastCrouchFactor, 0x204);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastTargetCrouchFactor, 0x208);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastAirTargetDelta, 0x20C);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastAirPositionDelta, 0x210);
CHECK_OFFSET(cMatrixChaseAlgorithm, ViewCamLastFactor, 0x214);
CHECK_OFFSET(cMatrixChaseAlgorithm, FullAirCamTimeInAir, 0x218);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastFullAirTargetDelta, 0x21C);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastFullAirPositionDelta, 0x220);
CHECK_OFFSET(cMatrixChaseAlgorithm, AirExitSmoothness, 0x224);
CHECK_OFFSET(cMatrixChaseAlgorithm, VerticalJumpFactor, 0x228);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeInitWait, 0x22C);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeTimeToLand, 0x230);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeTimeInAir, 0x234);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeRotatorAngle, 0x238);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeBlendOutTimeLeft, 0x23C);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeCrossBlendTime, 0x240);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeCrossBlendFullTime, 0x244);
CHECK_OFFSET(cMatrixChaseAlgorithm, SlowCamBlend, 0x248);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastHorizontalMoveLen, 0x24C);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastEnemyPullback, 0x250);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeNextXTime, 0x254);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeNextYTime, 0x258);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeNextZTime, 0x25C);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeXTarget, 0x260);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeYTarget, 0x264);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeZTarget, 0x268);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeLastXTarget, 0x26C);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeLastYTarget, 0x270);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeLastZTarget, 0x274);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeXCounter, 0x278);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeYCounter, 0x27C);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeZCounter, 0x280);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeNextXTime, 0x284);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeNextYTime, 0x288);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeNextZTime, 0x28C);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeXTarget, 0x290);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeYTarget, 0x294);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeZTarget, 0x298);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeLastXTarget, 0x29C);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeLastYTarget, 0x2A0);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeLastZTarget, 0x2A4);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeXCounter, 0x2A8);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeYCounter, 0x2AC);
CHECK_OFFSET(cMatrixChaseAlgorithm, SecondaryShakeZCounter, 0x2B0);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakeBlendOutTime, 0x2B4);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastHandplantRise, 0x2B8);
CHECK_OFFSET(cMatrixChaseAlgorithm, SteerTime, 0x2BC);
CHECK_OFFSET(cMatrixChaseAlgorithm, FramesSinceHandplant, 0x2C0);
CHECK_OFFSET(cMatrixChaseAlgorithm, AirCamSetupCountdown, 0x2C4);
CHECK_OFFSET(cMatrixChaseAlgorithm, JiggleFrameCount, 0x2C8);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastAirFrame, 0x2CC);
CHECK_OFFSET(cMatrixChaseAlgorithm, DoingHalfPipeCam, 0x2D0);
CHECK_OFFSET(cMatrixChaseAlgorithm, AirborneLastFrame, 0x2D4);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeCamInitialized, 0x2D8);
CHECK_OFFSET(cMatrixChaseAlgorithm, BoostingLastFrame, 0x2DC);
CHECK_OFFSET(cMatrixChaseAlgorithm, ApplyingAirFunc, 0x2E0);
CHECK_OFFSET(cMatrixChaseAlgorithm, HitGroundSinceStartingAirFunc, 0x2E4);
CHECK_OFFSET(cMatrixChaseAlgorithm, ApplyingFullAirFunc, 0x2E8);
CHECK_OFFSET(cMatrixChaseAlgorithm, ShakingLastFrame, 0x2EC);
CHECK_OFFSET(cMatrixChaseAlgorithm, DoSnap, 0x2F0);
CHECK_OFFSET(cMatrixChaseAlgorithm, Setup_Boost, 0x2F4);
CHECK_OFFSET(cMatrixChaseAlgorithm, Setup_VisualBoost, 0x2F8);
CHECK_OFFSET(cMatrixChaseAlgorithm, Setup_IsTargetHandplanting, 0x2FC);
CHECK_OFFSET(cMatrixChaseAlgorithm, SlowCamState, 0x300);
CHECK_OFFSET(cMatrixChaseAlgorithm, AirTargetSpline, 0x304);
CHECK_OFFSET(cMatrixChaseAlgorithm, AirPositionSpline, 0x310);
CHECK_OFFSET(cMatrixChaseAlgorithm, FullAirTargetSpline, 0x31C);
CHECK_OFFSET(cMatrixChaseAlgorithm, FullAirPositionSpline, 0x328);
CHECK_OFFSET(cMatrixChaseAlgorithm, HalfPipeAngleSpline, 0x334);
CHECK_OFFSET(cMatrixChaseAlgorithm, TurningPIDFilter, 0x340);
CHECK_OFFSET(cMatrixChaseAlgorithm, unk_38C, 0x38C);
CHECK_OFFSET(cMatrixChaseAlgorithm, LastAngle, 0x390);
CHECK_OFFSET(cMatrixChaseAlgorithm, unk_394, 0x394);
CHECK_SIZE(cMatrixChaseAlgorithm, 0x3A0);

// cRelativeChaseAlgorithm (0x3C0; On Tour 0x3B0). Ctor cRelativeChaseAlgorithm_cRelativeChaseAlgorithm(this, angle) (vtable cRelativeChaseAlgorithm_vtbl; the factory
// then stores the camera type into AlgorithmType), dtor cRelativeChaseAlgorithm__cRelativeChaseAlgorithm, snap cRelativeChaseAlgorithm_snap, update cRelativeChaseAlgorithm_update,
// getRaceCamFinalPhaseData cRelativeChaseAlgorithm_getRaceCamFinalPhaseData, getRestDrop cRelativeChaseAlgorithm_getRestDrop.
struct cRelativeChaseAlgorithm {
    // --- cLookAtChaseAlgorithm (camera/cameraalgorithms.h, 0x390; its own bases cChaseAlgorithm <- cCameraAlgorithm
    //     inline), copied field for field
    // --- cCameraAlgorithm
    float View;                     // 0x000 [O]; [R] ctor: pi/4
    float Near;                     // 0x004 [O]; [R] 10.0f
    float Far;                      // 0x008 [O]; [R] 30000.0f
    int AlgorithmType;              // 0x00C [O]; [R] 0x54; subclass ctors store their own
    void** mVtbl;                   // 0x010 [R] cLookAtChaseAlgorithm_vtbl; [U] the virtual calls (updateAlgorithm cLookAtChaseAlgorithm_updateAlgorithm ...)
    char unk_014[0xC];              // 0x014 alignment gap
    // --- cChaseAlgorithm
    t4Vector CamTargetPos;          // 0x020 [O]; [U] sq/lqc2 in the update functions
    cRiderCameraTarget* Target;     // 0x030 [O]; [P]; [U] setTarget cChaseAlgorithm_setTarget; 15 functions read it
    char unk_034[0xC];              // 0x034 padding
    // --- cLookAtChaseAlgorithm
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
    char AirTargetSpline[0xC];      // 0x304 [O] cCubicSplineInterpolant (bx/cubicspline.h: bitfield word, MaxX,
                                    //       CubicSplines); [R] constructed by cCubicSplineInterpolant_cCubicSplineInterpolant, destroyed by cCubicSplineInterpolant__cCubicSplineInterpolant
    char AirPositionSpline[0xC];    // 0x310 [O] cCubicSplineInterpolant; [R] ditto
    char FullAirTargetSpline[0xC];  // 0x31C [O] cCubicSplineInterpolant; [R] ditto
    char FullAirPositionSpline[0xC]; // 0x328 [O] cCubicSplineInterpolant; [R] ditto
    char HalfPipeAngleSpline[0xC];  // 0x334 [O] cCubicSplineInterpolant; [R] ditto
    cPIDFilter TurningPIDFilter;    // 0x340 [O]; [R] the ctor sets its three factors and calls cPIDFilter_reset;
                                    //       [U] cLookAtChaseAlgorithm_setupCamDirection runs the filter inline
    char unk_38C[0x4];              // 0x38C padding to the 16-byte class alignment
    // --- cRelativeChaseAlgorithm
    float Angle;                    // 0x390 [O] (On Tour 0x380); [R] = angle (swc1); [U] cRelativeChaseAlgorithm_update (lwc1)
    int Initialized;                // 0x394 [O] bool (a word in retail); [R] = 0; [U] cRelativeChaseAlgorithm_update tests and sets it
    char unk_398[0x8];              // 0x398 alignment gap before the 16-byte-aligned t4Vector
    t4Vector LastLook;              // 0x3A0 [O] (On Tour 0x390); [U] cRelativeChaseAlgorithm_update (sq/lqc2)
    t4Vector LastCamTargetPos;      // 0x3B0 [O] (On Tour 0x3A0); [U] cRelativeChaseAlgorithm_update (sq/lqc2)
};

CHECK_OFFSET(cRelativeChaseAlgorithm, View, 0x000);
CHECK_OFFSET(cRelativeChaseAlgorithm, Near, 0x004);
CHECK_OFFSET(cRelativeChaseAlgorithm, Far, 0x008);
CHECK_OFFSET(cRelativeChaseAlgorithm, AlgorithmType, 0x00C);
CHECK_OFFSET(cRelativeChaseAlgorithm, mVtbl, 0x010);
CHECK_OFFSET(cRelativeChaseAlgorithm, unk_014, 0x014);
CHECK_OFFSET(cRelativeChaseAlgorithm, CamTargetPos, 0x020);
CHECK_OFFSET(cRelativeChaseAlgorithm, Target, 0x030);
CHECK_OFFSET(cRelativeChaseAlgorithm, unk_034, 0x034);
CHECK_OFFSET(cRelativeChaseAlgorithm, Position, 0x040);
CHECK_OFFSET(cRelativeChaseAlgorithm, Heading, 0x050);
CHECK_OFFSET(cRelativeChaseAlgorithm, Pitch, 0x054);
CHECK_OFFSET(cRelativeChaseAlgorithm, CamRoll, 0x058);
CHECK_OFFSET(cRelativeChaseAlgorithm, unk_05C, 0x05C);
CHECK_OFFSET(cRelativeChaseAlgorithm, CalculatedPose, 0x060);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastCamDir, 0x080);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastVelLookahead, 0x090);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastRiderPos, 0x0A0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastHandplantLook, 0x0B0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastRailMove, 0x0C0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastHalfPipeMove, 0x0D0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastHalfPipeDistCorrection, 0x0E0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastLookaheadSkew, 0x0F0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastHorizontalVel, 0x100);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeInboundLook, 0x110);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeExitLook, 0x120);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeRotator, 0x130);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeNewLook, 0x140);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeFullOut, 0x150);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeOut, 0x160);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeBlendOutLastLook, 0x170);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeBlendOutLastTargetDelta, 0x180);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastSlowVelocity, 0x190);
CHECK_OFFSET(cRelativeChaseAlgorithm, SlowCamEnterLook, 0x1A0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastOriginalVel, 0x1B0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastDirectionVel, 0x1C0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastDist, 0x1D0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastDrop, 0x1D4);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastPitch, 0x1D8);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastViewScalar, 0x1DC);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondsSinceLastHandplant, 0x1E0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastHeight, 0x1E4);
CHECK_OFFSET(cRelativeChaseAlgorithm, ExtraBoostDist, 0x1E8);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastBoostWarp, 0x1EC);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastCameraSwing, 0x1F0);
CHECK_OFFSET(cRelativeChaseAlgorithm, AirCamTimeInAir, 0x1F4);
CHECK_OFFSET(cRelativeChaseAlgorithm, AirSplineScalar, 0x1F8);
CHECK_OFFSET(cRelativeChaseAlgorithm, AirTimeScalar, 0x1FC);
CHECK_OFFSET(cRelativeChaseAlgorithm, AirCamMaxTime, 0x200);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastCrouchFactor, 0x204);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastTargetCrouchFactor, 0x208);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastAirTargetDelta, 0x20C);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastAirPositionDelta, 0x210);
CHECK_OFFSET(cRelativeChaseAlgorithm, ViewCamLastFactor, 0x214);
CHECK_OFFSET(cRelativeChaseAlgorithm, FullAirCamTimeInAir, 0x218);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastFullAirTargetDelta, 0x21C);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastFullAirPositionDelta, 0x220);
CHECK_OFFSET(cRelativeChaseAlgorithm, AirExitSmoothness, 0x224);
CHECK_OFFSET(cRelativeChaseAlgorithm, VerticalJumpFactor, 0x228);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeInitWait, 0x22C);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeTimeToLand, 0x230);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeTimeInAir, 0x234);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeRotatorAngle, 0x238);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeBlendOutTimeLeft, 0x23C);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeCrossBlendTime, 0x240);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeCrossBlendFullTime, 0x244);
CHECK_OFFSET(cRelativeChaseAlgorithm, SlowCamBlend, 0x248);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastHorizontalMoveLen, 0x24C);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastEnemyPullback, 0x250);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeNextXTime, 0x254);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeNextYTime, 0x258);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeNextZTime, 0x25C);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeXTarget, 0x260);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeYTarget, 0x264);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeZTarget, 0x268);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeLastXTarget, 0x26C);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeLastYTarget, 0x270);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeLastZTarget, 0x274);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeXCounter, 0x278);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeYCounter, 0x27C);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeZCounter, 0x280);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeNextXTime, 0x284);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeNextYTime, 0x288);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeNextZTime, 0x28C);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeXTarget, 0x290);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeYTarget, 0x294);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeZTarget, 0x298);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeLastXTarget, 0x29C);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeLastYTarget, 0x2A0);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeLastZTarget, 0x2A4);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeXCounter, 0x2A8);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeYCounter, 0x2AC);
CHECK_OFFSET(cRelativeChaseAlgorithm, SecondaryShakeZCounter, 0x2B0);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakeBlendOutTime, 0x2B4);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastHandplantRise, 0x2B8);
CHECK_OFFSET(cRelativeChaseAlgorithm, SteerTime, 0x2BC);
CHECK_OFFSET(cRelativeChaseAlgorithm, FramesSinceHandplant, 0x2C0);
CHECK_OFFSET(cRelativeChaseAlgorithm, AirCamSetupCountdown, 0x2C4);
CHECK_OFFSET(cRelativeChaseAlgorithm, JiggleFrameCount, 0x2C8);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastAirFrame, 0x2CC);
CHECK_OFFSET(cRelativeChaseAlgorithm, DoingHalfPipeCam, 0x2D0);
CHECK_OFFSET(cRelativeChaseAlgorithm, AirborneLastFrame, 0x2D4);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeCamInitialized, 0x2D8);
CHECK_OFFSET(cRelativeChaseAlgorithm, BoostingLastFrame, 0x2DC);
CHECK_OFFSET(cRelativeChaseAlgorithm, ApplyingAirFunc, 0x2E0);
CHECK_OFFSET(cRelativeChaseAlgorithm, HitGroundSinceStartingAirFunc, 0x2E4);
CHECK_OFFSET(cRelativeChaseAlgorithm, ApplyingFullAirFunc, 0x2E8);
CHECK_OFFSET(cRelativeChaseAlgorithm, ShakingLastFrame, 0x2EC);
CHECK_OFFSET(cRelativeChaseAlgorithm, DoSnap, 0x2F0);
CHECK_OFFSET(cRelativeChaseAlgorithm, Setup_Boost, 0x2F4);
CHECK_OFFSET(cRelativeChaseAlgorithm, Setup_VisualBoost, 0x2F8);
CHECK_OFFSET(cRelativeChaseAlgorithm, Setup_IsTargetHandplanting, 0x2FC);
CHECK_OFFSET(cRelativeChaseAlgorithm, SlowCamState, 0x300);
CHECK_OFFSET(cRelativeChaseAlgorithm, AirTargetSpline, 0x304);
CHECK_OFFSET(cRelativeChaseAlgorithm, AirPositionSpline, 0x310);
CHECK_OFFSET(cRelativeChaseAlgorithm, FullAirTargetSpline, 0x31C);
CHECK_OFFSET(cRelativeChaseAlgorithm, FullAirPositionSpline, 0x328);
CHECK_OFFSET(cRelativeChaseAlgorithm, HalfPipeAngleSpline, 0x334);
CHECK_OFFSET(cRelativeChaseAlgorithm, TurningPIDFilter, 0x340);
CHECK_OFFSET(cRelativeChaseAlgorithm, unk_38C, 0x38C);
CHECK_OFFSET(cRelativeChaseAlgorithm, Angle, 0x390);
CHECK_OFFSET(cRelativeChaseAlgorithm, Initialized, 0x394);
CHECK_OFFSET(cRelativeChaseAlgorithm, unk_398, 0x398);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastLook, 0x3A0);
CHECK_OFFSET(cRelativeChaseAlgorithm, LastCamTargetPos, 0x3B0);
CHECK_SIZE(cRelativeChaseAlgorithm, 0x3C0);

// cSplineCamAlgorithm (0x3D0; On Tour 0x3C0). Ctor cSplineCamAlgorithm_cSplineCamAlgorithm(this, point1..point4, time, leadDistance,
// heightOffset, fov) (vtable cSplineCamAlgorithm_vtbl; builds the three interpolants with cCubicSplineInterpolant_cCubicSplineInterpolant / cCubicSplineInterpolant_initAsNatural /
// cCubicSplineInterpolant_addPoint / cCubicSplineInterpolant_evaluate), dtor cSplineCamAlgorithm__cSplineCamAlgorithm (cCubicSplineInterpolant__cCubicSplineInterpolant on each interpolant), snap cSplineCamAlgorithm_snap,
// update cSplineCamAlgorithm_update, getRaceCamFinalPhaseData cSplineCamAlgorithm_getRaceCamFinalPhaseData, getRestDrop cSplineCamAlgorithm_getRestDrop.
struct cSplineCamAlgorithm {
    // --- cLookAtChaseAlgorithm (camera/cameraalgorithms.h, 0x390; its own bases cChaseAlgorithm <- cCameraAlgorithm
    //     inline), copied field for field
    // --- cCameraAlgorithm
    float View;                     // 0x000 [O]; [R] ctor: pi/4
    float Near;                     // 0x004 [O]; [R] 10.0f
    float Far;                      // 0x008 [O]; [R] 30000.0f
    int AlgorithmType;              // 0x00C [O]; [R] 0x54; subclass ctors store their own
    void** mVtbl;                   // 0x010 [R] cLookAtChaseAlgorithm_vtbl; [U] the virtual calls (updateAlgorithm cLookAtChaseAlgorithm_updateAlgorithm ...)
    char unk_014[0xC];              // 0x014 alignment gap
    // --- cChaseAlgorithm
    t4Vector CamTargetPos;          // 0x020 [O]; [U] sq/lqc2 in the update functions
    cRiderCameraTarget* Target;     // 0x030 [O]; [P]; [U] setTarget cChaseAlgorithm_setTarget; 15 functions read it
    char unk_034[0xC];              // 0x034 padding
    // --- cLookAtChaseAlgorithm
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
    char AirTargetSpline[0xC];      // 0x304 [O] cCubicSplineInterpolant (bx/cubicspline.h: bitfield word, MaxX,
                                    //       CubicSplines); [R] constructed by cCubicSplineInterpolant_cCubicSplineInterpolant, destroyed by cCubicSplineInterpolant__cCubicSplineInterpolant
    char AirPositionSpline[0xC];    // 0x310 [O] cCubicSplineInterpolant; [R] ditto
    char FullAirTargetSpline[0xC];  // 0x31C [O] cCubicSplineInterpolant; [R] ditto
    char FullAirPositionSpline[0xC]; // 0x328 [O] cCubicSplineInterpolant; [R] ditto
    char HalfPipeAngleSpline[0xC];  // 0x334 [O] cCubicSplineInterpolant; [R] ditto
    cPIDFilter TurningPIDFilter;    // 0x340 [O]; [R] the ctor sets its three factors and calls cPIDFilter_reset;
                                    //       [U] cLookAtChaseAlgorithm_setupCamDirection runs the filter inline
    char unk_38C[0x4];              // 0x38C padding to the 16-byte class alignment
    // --- cSplineCamAlgorithm
    char XOfT[0xC];                 // 0x390 [O] cCubicSplineInterpolant (On Tour 0x380); [R] ctor passes this+0x390
                                    //       to cCubicSplineInterpolant_cCubicSplineInterpolant/cCubicSplineInterpolant_evaluate, dtor to cCubicSplineInterpolant__cCubicSplineInterpolant
    char YOfT[0xC];                 // 0x39C [O] cCubicSplineInterpolant; [R] ditto (this+0x39C)
    char ZOfT[0xC];                 // 0x3A8 [O] cCubicSplineInterpolant; [R] ditto (this+0x3A8)
    float TotalTime;                // 0x3B4 [O]; [R] swc1; [U] cSplineCamAlgorithm_update (lwc1)
    float LastEvaluatedTime;        // 0x3B8 [O]; [R] swc1; [U] cSplineCamAlgorithm_update (lwc1/swc1)
    float Speed;                    // 0x3BC [O]; [R] swc1; [U] cSplineCamAlgorithm_update
    float LeadDistance;             // 0x3C0 [O]; [R] = leadDistance; [U] cSplineCamAlgorithm_update
    float HeightOffset;             // 0x3C4 [O]; [R] = heightOffset; [U] cSplineCamAlgorithm_update
    char unk_3C8[0x8];              // 0x3C8 padding to the 16-byte class alignment (alloc 0x3D0)
};

CHECK_OFFSET(cSplineCamAlgorithm, View, 0x000);
CHECK_OFFSET(cSplineCamAlgorithm, Near, 0x004);
CHECK_OFFSET(cSplineCamAlgorithm, Far, 0x008);
CHECK_OFFSET(cSplineCamAlgorithm, AlgorithmType, 0x00C);
CHECK_OFFSET(cSplineCamAlgorithm, mVtbl, 0x010);
CHECK_OFFSET(cSplineCamAlgorithm, unk_014, 0x014);
CHECK_OFFSET(cSplineCamAlgorithm, CamTargetPos, 0x020);
CHECK_OFFSET(cSplineCamAlgorithm, Target, 0x030);
CHECK_OFFSET(cSplineCamAlgorithm, unk_034, 0x034);
CHECK_OFFSET(cSplineCamAlgorithm, Position, 0x040);
CHECK_OFFSET(cSplineCamAlgorithm, Heading, 0x050);
CHECK_OFFSET(cSplineCamAlgorithm, Pitch, 0x054);
CHECK_OFFSET(cSplineCamAlgorithm, CamRoll, 0x058);
CHECK_OFFSET(cSplineCamAlgorithm, unk_05C, 0x05C);
CHECK_OFFSET(cSplineCamAlgorithm, CalculatedPose, 0x060);
CHECK_OFFSET(cSplineCamAlgorithm, LastCamDir, 0x080);
CHECK_OFFSET(cSplineCamAlgorithm, LastVelLookahead, 0x090);
CHECK_OFFSET(cSplineCamAlgorithm, LastRiderPos, 0x0A0);
CHECK_OFFSET(cSplineCamAlgorithm, LastHandplantLook, 0x0B0);
CHECK_OFFSET(cSplineCamAlgorithm, LastRailMove, 0x0C0);
CHECK_OFFSET(cSplineCamAlgorithm, LastHalfPipeMove, 0x0D0);
CHECK_OFFSET(cSplineCamAlgorithm, LastHalfPipeDistCorrection, 0x0E0);
CHECK_OFFSET(cSplineCamAlgorithm, LastLookaheadSkew, 0x0F0);
CHECK_OFFSET(cSplineCamAlgorithm, LastHorizontalVel, 0x100);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeInboundLook, 0x110);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeExitLook, 0x120);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeRotator, 0x130);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeNewLook, 0x140);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeFullOut, 0x150);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeOut, 0x160);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeBlendOutLastLook, 0x170);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeBlendOutLastTargetDelta, 0x180);
CHECK_OFFSET(cSplineCamAlgorithm, LastSlowVelocity, 0x190);
CHECK_OFFSET(cSplineCamAlgorithm, SlowCamEnterLook, 0x1A0);
CHECK_OFFSET(cSplineCamAlgorithm, LastOriginalVel, 0x1B0);
CHECK_OFFSET(cSplineCamAlgorithm, LastDirectionVel, 0x1C0);
CHECK_OFFSET(cSplineCamAlgorithm, LastDist, 0x1D0);
CHECK_OFFSET(cSplineCamAlgorithm, LastDrop, 0x1D4);
CHECK_OFFSET(cSplineCamAlgorithm, LastPitch, 0x1D8);
CHECK_OFFSET(cSplineCamAlgorithm, LastViewScalar, 0x1DC);
CHECK_OFFSET(cSplineCamAlgorithm, SecondsSinceLastHandplant, 0x1E0);
CHECK_OFFSET(cSplineCamAlgorithm, LastHeight, 0x1E4);
CHECK_OFFSET(cSplineCamAlgorithm, ExtraBoostDist, 0x1E8);
CHECK_OFFSET(cSplineCamAlgorithm, LastBoostWarp, 0x1EC);
CHECK_OFFSET(cSplineCamAlgorithm, LastCameraSwing, 0x1F0);
CHECK_OFFSET(cSplineCamAlgorithm, AirCamTimeInAir, 0x1F4);
CHECK_OFFSET(cSplineCamAlgorithm, AirSplineScalar, 0x1F8);
CHECK_OFFSET(cSplineCamAlgorithm, AirTimeScalar, 0x1FC);
CHECK_OFFSET(cSplineCamAlgorithm, AirCamMaxTime, 0x200);
CHECK_OFFSET(cSplineCamAlgorithm, LastCrouchFactor, 0x204);
CHECK_OFFSET(cSplineCamAlgorithm, LastTargetCrouchFactor, 0x208);
CHECK_OFFSET(cSplineCamAlgorithm, LastAirTargetDelta, 0x20C);
CHECK_OFFSET(cSplineCamAlgorithm, LastAirPositionDelta, 0x210);
CHECK_OFFSET(cSplineCamAlgorithm, ViewCamLastFactor, 0x214);
CHECK_OFFSET(cSplineCamAlgorithm, FullAirCamTimeInAir, 0x218);
CHECK_OFFSET(cSplineCamAlgorithm, LastFullAirTargetDelta, 0x21C);
CHECK_OFFSET(cSplineCamAlgorithm, LastFullAirPositionDelta, 0x220);
CHECK_OFFSET(cSplineCamAlgorithm, AirExitSmoothness, 0x224);
CHECK_OFFSET(cSplineCamAlgorithm, VerticalJumpFactor, 0x228);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeInitWait, 0x22C);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeTimeToLand, 0x230);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeTimeInAir, 0x234);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeRotatorAngle, 0x238);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeBlendOutTimeLeft, 0x23C);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeCrossBlendTime, 0x240);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeCrossBlendFullTime, 0x244);
CHECK_OFFSET(cSplineCamAlgorithm, SlowCamBlend, 0x248);
CHECK_OFFSET(cSplineCamAlgorithm, LastHorizontalMoveLen, 0x24C);
CHECK_OFFSET(cSplineCamAlgorithm, LastEnemyPullback, 0x250);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeNextXTime, 0x254);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeNextYTime, 0x258);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeNextZTime, 0x25C);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeXTarget, 0x260);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeYTarget, 0x264);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeZTarget, 0x268);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeLastXTarget, 0x26C);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeLastYTarget, 0x270);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeLastZTarget, 0x274);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeXCounter, 0x278);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeYCounter, 0x27C);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeZCounter, 0x280);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeNextXTime, 0x284);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeNextYTime, 0x288);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeNextZTime, 0x28C);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeXTarget, 0x290);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeYTarget, 0x294);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeZTarget, 0x298);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeLastXTarget, 0x29C);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeLastYTarget, 0x2A0);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeLastZTarget, 0x2A4);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeXCounter, 0x2A8);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeYCounter, 0x2AC);
CHECK_OFFSET(cSplineCamAlgorithm, SecondaryShakeZCounter, 0x2B0);
CHECK_OFFSET(cSplineCamAlgorithm, ShakeBlendOutTime, 0x2B4);
CHECK_OFFSET(cSplineCamAlgorithm, LastHandplantRise, 0x2B8);
CHECK_OFFSET(cSplineCamAlgorithm, SteerTime, 0x2BC);
CHECK_OFFSET(cSplineCamAlgorithm, FramesSinceHandplant, 0x2C0);
CHECK_OFFSET(cSplineCamAlgorithm, AirCamSetupCountdown, 0x2C4);
CHECK_OFFSET(cSplineCamAlgorithm, JiggleFrameCount, 0x2C8);
CHECK_OFFSET(cSplineCamAlgorithm, LastAirFrame, 0x2CC);
CHECK_OFFSET(cSplineCamAlgorithm, DoingHalfPipeCam, 0x2D0);
CHECK_OFFSET(cSplineCamAlgorithm, AirborneLastFrame, 0x2D4);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeCamInitialized, 0x2D8);
CHECK_OFFSET(cSplineCamAlgorithm, BoostingLastFrame, 0x2DC);
CHECK_OFFSET(cSplineCamAlgorithm, ApplyingAirFunc, 0x2E0);
CHECK_OFFSET(cSplineCamAlgorithm, HitGroundSinceStartingAirFunc, 0x2E4);
CHECK_OFFSET(cSplineCamAlgorithm, ApplyingFullAirFunc, 0x2E8);
CHECK_OFFSET(cSplineCamAlgorithm, ShakingLastFrame, 0x2EC);
CHECK_OFFSET(cSplineCamAlgorithm, DoSnap, 0x2F0);
CHECK_OFFSET(cSplineCamAlgorithm, Setup_Boost, 0x2F4);
CHECK_OFFSET(cSplineCamAlgorithm, Setup_VisualBoost, 0x2F8);
CHECK_OFFSET(cSplineCamAlgorithm, Setup_IsTargetHandplanting, 0x2FC);
CHECK_OFFSET(cSplineCamAlgorithm, SlowCamState, 0x300);
CHECK_OFFSET(cSplineCamAlgorithm, AirTargetSpline, 0x304);
CHECK_OFFSET(cSplineCamAlgorithm, AirPositionSpline, 0x310);
CHECK_OFFSET(cSplineCamAlgorithm, FullAirTargetSpline, 0x31C);
CHECK_OFFSET(cSplineCamAlgorithm, FullAirPositionSpline, 0x328);
CHECK_OFFSET(cSplineCamAlgorithm, HalfPipeAngleSpline, 0x334);
CHECK_OFFSET(cSplineCamAlgorithm, TurningPIDFilter, 0x340);
CHECK_OFFSET(cSplineCamAlgorithm, unk_38C, 0x38C);
CHECK_OFFSET(cSplineCamAlgorithm, XOfT, 0x390);
CHECK_OFFSET(cSplineCamAlgorithm, YOfT, 0x39C);
CHECK_OFFSET(cSplineCamAlgorithm, ZOfT, 0x3A8);
CHECK_OFFSET(cSplineCamAlgorithm, TotalTime, 0x3B4);
CHECK_OFFSET(cSplineCamAlgorithm, LastEvaluatedTime, 0x3B8);
CHECK_OFFSET(cSplineCamAlgorithm, Speed, 0x3BC);
CHECK_OFFSET(cSplineCamAlgorithm, LeadDistance, 0x3C0);
CHECK_OFFSET(cSplineCamAlgorithm, HeightOffset, 0x3C4);
CHECK_OFFSET(cSplineCamAlgorithm, unk_3C8, 0x3C8);
CHECK_SIZE(cSplineCamAlgorithm, 0x3D0);

// ---------------------------------------------------------------------------------------------------------------
// The field-less subclasses. All six live in retail camera/camera (0x160438-0x168F60) among cMatrix/cRelative-
// ChaseAlgorithm, cChaseCameraController and cLookAtChaseAlgorithm's own methods; in the preview their methods span
// 0x15F8F0-0x1718F0, where the only camera/ assert strings are camera/race/racealgorithms.cpp's (0x1675A8-0x16CC80).
// Each overrides exactly vtable slots 1, 3, 5, 7, 13 (On Tour declares exactly ctor, dtor, snap,
// update, getRestDrop, getRaceCamFinalPhaseData for each), so On Tour's 13-slot vtable maps slot for slot.

// cManualChaseAlgorithm (0x390: no fields of its own) - the rider-steered chase camera ("Manual Chase Camera Algorithm").
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Manual Chase Camera Algorithm") then the ctor cManualChaseAlgorithm_cManualChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 0xB and vtable cManualChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cManualChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_MANUAL = 11.
// Methods (vtable slot): dtor cManualChaseAlgorithm__cManualChaseAlgorithm__FPv (1), snap cManualChaseAlgorithm_snap (3), update cManualChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cManualChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0 (7), getRestDrop cManualChaseAlgorithm_getRestDrop__FPv (13); the other slots are the base's.
struct cManualChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
};

CHECK_SIZE(cManualChaseAlgorithm, 0x390);

// cIdleChaseAlgorithm (0x390: no fields of its own) - the idle chase camera.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Idle Chase Camera Algorithm") then the ctor cIdleChaseAlgorithm_cIdleChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 7 and vtable cIdleChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cIdleChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_IDLE = 7.
// Methods (vtable slot): dtor cIdleChaseAlgorithm__cIdleChaseAlgorithm__FPv (1), snap cIdleChaseAlgorithm_snap (3), update cIdleChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cIdleChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0 (7), getRestDrop cIdleChaseAlgorithm_getRestDrop__FPv (13); the other slots are the base's.
// [P] the preview's _vt$19cIdleChaseAlgorithm matches retail cIdleChaseAlgorithm_vtbl (builds/x/v2/vtables_v2.tsv).
struct cIdleChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
};

CHECK_SIZE(cIdleChaseAlgorithm, 0x390);

// cLazyChaseAlgorithm (0x390: no fields of its own) - the lazy chase camera.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Lazy Chase Camera Algorithm") then the ctor cLazyChaseAlgorithm_cLazyChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 0xD and vtable cLazyChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cLazyChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_LAZY = 13.
// Methods (vtable slot): dtor cLazyChaseAlgorithm__cLazyChaseAlgorithm__FPv (1), snap cLazyChaseAlgorithm_snap (3), update cLazyChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cLazyChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0 (7), getRestDrop cLazyChaseAlgorithm_getRestDrop__FPv (13); the other slots are the base's.
// [P] the preview's _vt$19cLazyChaseAlgorithm matches retail cLazyChaseAlgorithm_vtbl.
struct cLazyChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
};

CHECK_SIZE(cLazyChaseAlgorithm, 0x390);

// cLookBackChaseAlgorithm (0x390: no fields of its own) - the look-back chase camera.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Look Back Chase Camera Algorithm") then the ctor cLookBackChaseAlgorithm_cLookBackChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 0x20 and vtable cLookBackChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cLookBackChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_LOOKBACK = 32.
// Methods (vtable slot): dtor cLookBackChaseAlgorithm__cLookBackChaseAlgorithm__FPv (1), snap cLookBackChaseAlgorithm_snap (3), update cLookBackChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cLookBackChaseAlgorithm_getRaceCamFinalPhaseData__FPvT0 (7), getRestDrop cLookBackChaseAlgorithm_getRestDrop__FPv (13); the other slots are the base's.
// [P] the preview's _vt$23cLookBackChaseAlgorithm matches retail cLookBackChaseAlgorithm_vtbl.
struct cLookBackChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
};

CHECK_SIZE(cLookBackChaseAlgorithm, 0x390);

// cMidRightChaseAlgorithm (0x390: no fields of its own) - the mid-distance chase camera shifted to the rider's right.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Mid Right Chase Camera Algorithm") then the ctor cMidRightChaseAlgorithm_cMidRightChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 9 and vtable cMidRightChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cMidRightChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_MID_RIGHT = 9.
// Methods (vtable slot): dtor cMidRightChaseAlgorithm__cMidRightChaseAlgorithm__FPv (1), snap cMidRightChaseAlgorithm_snap (3), update cMidRightChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cMidRightChaseAlgorithm_getRaceCamFinalPhaseData (7), getRestDrop cMidRightChaseAlgorithm_getRestDrop (13); the other slots are the base's.
// Which mid vtable is Right: (1) [R] the "Mid Right" allocation (createChaseAlgorithmBlend case 9 =
// CAMERA_ALGORITHM_CHASE_MID_RIGHT) calls cMidRightChaseAlgorithm_cMidRightChaseAlgorithm, which stores cMidRightChaseAlgorithm_vtbl; (2) [P] the preview's
// _vt$23cMidRightChaseAlgorithm update passes -0.29444468 to updateLateralShift, as retail cMidRightChaseAlgorithm_update does;
// (3) [O] On Tour's cMidRightChaseAlgorithm::update passes -kLateralShift_MIDSIDE (= 0.29444468, set by
// __sti__all_in_one_camera_cpp). Both mid ctors store type 9 in retail AND in On Tour (EA copy-paste).
struct cMidRightChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
};

CHECK_SIZE(cMidRightChaseAlgorithm, 0x390);

// cMidLeftChaseAlgorithm (0x390: no fields of its own) - the mid-distance chase camera shifted to the rider's left.
// [R] cChaseCameraController_createChaseAlgorithmBlend: cMemMan_alloc(0x390, "Mid Left Chase Camera Algorithm") then the ctor cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm,
// which calls cLookAtChaseAlgorithm_cLookAtChaseAlgorithm and stores AlgorithmType 9 (sic: MidRight's; On Tour's ctor also stores 9) and vtable cMidLeftChaseAlgorithm_vtbl; the ctor
// writes nothing past 0x390 and no method of the class touches self+0x390 or later.
// [O] On Tour: class cMidLeftChaseAlgorithm, size 0x380 (= its base cLookAtChaseAlgorithm, no members; the retail base is
// 0x390), ctor stores CAMERA_ALGORITHM_CHASE_MID_RIGHT = 9.
// Methods (vtable slot): dtor cMidLeftChaseAlgorithm__cMidLeftChaseAlgorithm__FPv (1), snap cMidLeftChaseAlgorithm_snap (3), update cMidLeftChaseAlgorithm_update (5),
// getRaceCamFinalPhaseData cMidLeftChaseAlgorithm_getRaceCamFinalPhaseData (7), getRestDrop cMidLeftChaseAlgorithm_getRestDrop (13); the other slots are the base's.
// Left, not Right: the "Mid Left" allocation (case 10 = CAMERA_ALGORITHM_CHASE_MID_LEFT) calls cMidLeftChaseAlgorithm_cMidLeftChaseAlgorithm;
// its update cMidLeftChaseAlgorithm_update passes +0.29444468 to updateLateralShift, like the preview's
// _vt$22cMidLeftChaseAlgorithm update and On Tour's cMidLeftChaseAlgorithm::update (+kLateralShift_MIDSIDE).
struct cMidLeftChaseAlgorithm : public cLookAtChaseAlgorithm {
    // no fields of its own
};

CHECK_SIZE(cMidLeftChaseAlgorithm, 0x390);


#endif /* CAMERA_RACE_RACEALGORITHMS_H */
