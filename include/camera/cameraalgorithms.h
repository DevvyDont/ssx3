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
// Retail vtables: cCameraAlgorithm D_0045BDE0, cBasicCameraAlgorithm cBasicCameraAlgorithm_vtbl, cChaseAlgorithm cChaseAlgorithm_vtbl,
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

struct cRiderCameraTarget;

// A PID controller (On Tour: cPIDFilter, 0x4C). Its methods are in camera/camera.cpp: func_00167DB8 = reset(value)
// (fills InputHistory/ResultHistory with value, zeroes Difference and NextFreeIndex); func_00167D88/98/A8 return the
// default factors.
struct cPIDFilter {
    float InputHistory[5];      // 0x00 [O]; [U] func_00167DB8 stores the value into each
    float ResultHistory[5];     // 0x14 [O]; [U] func_00167DB8 stores the value into each
    float Difference[5];        // 0x28 [O]; [U] func_00167DB8 zeroes each
    int NextFreeIndex;          // 0x3C [O]; [U] func_00167DB8 zeroes it; func_00162C78 indexes the histories by it
    float ProportionFactor;     // 0x40 [O]; [R] cLookAtChaseAlgorithm ctor (+0x380)
    float ErrorHistoryFactor;   // 0x44 [O]; [R] ditto (+0x384)
    float DerrivativeFactor;    // 0x48 [O] (EA's spelling); [R] ditto (+0x388)
};

CHECK_OFFSET(cPIDFilter, InputHistory, 0x00);
CHECK_OFFSET(cPIDFilter, ResultHistory, 0x14);
CHECK_OFFSET(cPIDFilter, Difference, 0x28);
CHECK_OFFSET(cPIDFilter, NextFreeIndex, 0x3C);
CHECK_OFFSET(cPIDFilter, ProportionFactor, 0x40);
CHECK_OFFSET(cPIDFilter, ErrorHistoryFactor, 0x44);
CHECK_OFFSET(cPIDFilter, DerrivativeFactor, 0x48);
CHECK_SIZE(cPIDFilter, 0x4C);

// cCameraAlgorithm (On Tour: 0x14). Abstract; its defaults are func_00167DE8 (dtor) .. func_00167E98.
struct cCameraAlgorithm {
    float View;                 // 0x00 [O]; [R] = getDefaultView() (func_00167E30) in every ctor
    float Near;                 // 0x04 [O]; [R] 10.0f
    float Far;                  // 0x08 [O]; [R] 30000.0f
    int AlgorithmType;          // 0x0C [O] eCamAlgoType; [R] 0x4E, then each ctor stores its own (cBasic 0x4F,
                                //      cChase 0x53, cLookAtChase 0x54); [U] func_00167FC0 setAlgorithmType (sw),
                                //      func_00167FC8 getType (lw)
    void** mVtbl;               // 0x10 [R] D_0045BDE0, then each ctor's own vtable
};

CHECK_OFFSET(cCameraAlgorithm, View, 0x00);
CHECK_OFFSET(cCameraAlgorithm, Near, 0x04);
CHECK_OFFSET(cCameraAlgorithm, Far, 0x08);
CHECK_OFFSET(cCameraAlgorithm, AlgorithmType, 0x0C);
CHECK_OFFSET(cCameraAlgorithm, mVtbl, 0x10);
CHECK_SIZE(cCameraAlgorithm, 0x14);

// cBasicCameraAlgorithm (On Tour: 0x40): a camera at a position with a pitch and heading.
struct cBasicCameraAlgorithm {
    // --- cCameraAlgorithm
    float View;                 // 0x00 [O]; [R]
    float Near;                 // 0x04 [O]; [R]
    float Far;                  // 0x08 [O]; [R]
    int AlgorithmType;          // 0x0C [O]; [R] 0x4F
    void** mVtbl;               // 0x10 [R] cBasicCameraAlgorithm_vtbl (cManual: D_0045BD10, cEditor: D_0045BCA8)
    char unk_14[0xC];           // 0x14 alignment gap before the 16-byte-aligned t4Vector
    // --- cBasicCameraAlgorithm
    t4Vector Position;          // 0x20 [O]; [R] = D_004FF130; [U] func_00167ED0 setPosition (sq), func_00167EF0
                                //      getPosition (lq)
    float Pitch;                // 0x30 [O]; [P] getPitch/setPitch (preview 0x30); [R] 0; [U] func_00167EE0 (swc1),
                                //      func_00167F00 (lwc1)
    float Heading;              // 0x34 [O]; [P] getHeading/setHeading (preview 0x34); [R] 0; [U] func_00167EE8,
                                //      func_00167F08
    char unk_38[0x8];           // 0x38 padding to the 16-byte class alignment
};

CHECK_OFFSET(cBasicCameraAlgorithm, View, 0x00);
CHECK_OFFSET(cBasicCameraAlgorithm, Near, 0x04);
CHECK_OFFSET(cBasicCameraAlgorithm, Far, 0x08);
CHECK_OFFSET(cBasicCameraAlgorithm, AlgorithmType, 0x0C);
CHECK_OFFSET(cBasicCameraAlgorithm, mVtbl, 0x10);
CHECK_OFFSET(cBasicCameraAlgorithm, Position, 0x20);
CHECK_OFFSET(cBasicCameraAlgorithm, Pitch, 0x30);
CHECK_OFFSET(cBasicCameraAlgorithm, Heading, 0x34);
CHECK_SIZE(cBasicCameraAlgorithm, 0x40);

// cChaseAlgorithm (On Tour: 0x40): an algorithm that follows a rider.
struct cChaseAlgorithm {
    // --- cCameraAlgorithm
    float View;                 // 0x00 [O]; [R] cChaseAlgorithm_cChaseAlgorithm
    float Near;                 // 0x04 [O]; [R]
    float Far;                  // 0x08 [O]; [R]
    int AlgorithmType;          // 0x0C [O]; [R] 0x53
    void** mVtbl;               // 0x10 [R] cChaseAlgorithm_vtbl
    char unk_14[0xC];           // 0x14 alignment gap
    // --- cChaseAlgorithm
    t4Vector CamTargetPos;      // 0x20 [O]; [U] cChaseAlgorithm_getTargetPos returns it (lq); the chase algorithms
                                //      write it (sq) and read it (lqc2)
    cRiderCameraTarget* Target; // 0x30 [O]; [P] (preview verify reads 0x30); [U] func_00162310 setTarget (sw; cCamera_init
                                //      passes the "RiderCameraTarget"); the algorithms call its virtuals
    char unk_34[0xC];           // 0x34 padding to the 16-byte class alignment
};

CHECK_OFFSET(cChaseAlgorithm, View, 0x00);
CHECK_OFFSET(cChaseAlgorithm, AlgorithmType, 0x0C);
CHECK_OFFSET(cChaseAlgorithm, mVtbl, 0x10);
CHECK_OFFSET(cChaseAlgorithm, CamTargetPos, 0x20);
CHECK_OFFSET(cChaseAlgorithm, Target, 0x30);
CHECK_SIZE(cChaseAlgorithm, 0x40);

// cLookAtChaseAlgorithm (retail 0x390: the 0x390-byte allocations of its field-less subclasses, and cCamera's
// embedded cDEFAULT_3ChaseAlgorithm spanning 0xC0..0x450).
struct cLookAtChaseAlgorithm {
    // --- cCameraAlgorithm
    float View;                     // 0x000 [O]; [R] ctor: pi/4
    float Near;                     // 0x004 [O]; [R] 10.0f
    float Far;                      // 0x008 [O]; [R] 30000.0f
    int AlgorithmType;              // 0x00C [O]; [R] 0x54; subclass ctors store their own
    void** mVtbl;                   // 0x010 [R] cLookAtChaseAlgorithm_vtbl; [U] the virtual calls (updateAlgorithm func_001624E8 ...)
    char unk_014[0xC];              // 0x014 alignment gap
    // --- cChaseAlgorithm
    t4Vector CamTargetPos;          // 0x020 [O]; [U] sq/lqc2 in the update functions
    cRiderCameraTarget* Target;     // 0x030 [O]; [P]; [U] setTarget func_00162310; 15 functions read it
    char unk_034[0xC];              // 0x034 padding
    // --- cLookAtChaseAlgorithm
    t4Vector Position;              // 0x040 [O]; [R] = D_004FF130; [U] lq/sq/lqc2, .z/.w by lwc1/swc1
    float Heading;                  // 0x050 [O]; [R] 0; [U] lwc1/swc1 (func_00166640, func_001668B8)
    float Pitch;                    // 0x054 [O]; [R] 0; [U] lwc1/swc1 (func_001662A0, func_00166640)
    float CamRoll;                  // 0x058 [O]; [R] 0
    char unk_05C[0x4];              // 0x05C alignment gap
    tPose CalculatedPose;           // 0x060 [O]; [U] func_00166F90 (calcPose) writes P and Q (sq), reads Q.x..w
    t4Vector LastCamDir;            // 0x080 [O]; [U] sq/lqc2
    t4Vector LastVelLookahead;      // 0x090 [O]; [R] = t4vect_Zero
    t4Vector LastRiderPos;          // 0x0A0 [O]; [U] func_00163450
    t4Vector LastHandplantLook;     // 0x0B0 [O]; [U] func_00163450
    t4Vector LastRailMove;          // 0x0C0 [O]; [R] = t4vect_Zero
    t4Vector LastHalfPipeMove;      // 0x0D0 [O]; [R] = t4vect_Zero
    t4Vector LastHalfPipeDistCorrection; // 0x0E0 [O]; [R] = t4vect_Zero
    t4Vector LastLookaheadSkew;     // 0x0F0 [O]; [U] func_00166C60 (snap) clears it
    t4Vector LastHorizontalVel;     // 0x100 [O]; [U] lq/sq (func_00162568), .z by sw in snap
    t4Vector HalfPipeInboundLook;   // 0x110 [O]; [U] func_00164878 (updateHalfPipeCam)
    t4Vector HalfPipeExitLook;      // 0x120 [O]; [U] func_00164878, .z lwc1/swc1
    t4Vector HalfPipeRotator;       // 0x130 [O]; [U] func_00164878, .x/.y/.z lwc1
    t4Vector HalfPipeNewLook;       // 0x140 [O]; [U] func_00164878
    t4Vector HalfPipeFullOut;       // 0x150 [O]; [U] func_00164878
    t4Vector HalfPipeOut;           // 0x160 [O]; [U] func_00164878
    t4Vector HalfPipeBlendOutLastLook; // 0x170 [O]; [U] func_00164878, snap
    t4Vector HalfPipeBlendOutLastTargetDelta; // 0x180 [O]; [U] func_00164878
    t4Vector LastSlowVelocity;      // 0x190 [O]; [U] snap
    t4Vector SlowCamEnterLook;      // 0x1A0 [O]; [U] snap
    t4Vector LastOriginalVel;       // 0x1B0 [O]; [U] func_00162568, snap
    t4Vector LastDirectionVel;      // 0x1C0 [O]; [U] func_00162C78, snap
    float LastDist;                 // 0x1D0 [O]; [R] 200.0f; [U] func_00163010
    float LastDrop;                 // 0x1D4 [O]; [R] 0; [U] func_00163270
    float LastPitch;                // 0x1D8 [O]; [R] = Pitch; [U] func_001662A0
    float LastViewScalar;           // 0x1DC [O]; [R] 1.0f; [U] func_001633B0
    float SecondsSinceLastHandplant; // 0x1E0 [O]; [R] 10.0f; [U] func_00163450 (lwc1/swc1; one sw)
    float LastHeight;               // 0x1E4 [O]; [R] 0; [U] func_00162B90
    float ExtraBoostDist;           // 0x1E8 [O]; [U] func_00163010
    float LastBoostWarp;            // 0x1EC [O]; [U] func_00163010
    float LastCameraSwing;          // 0x1F0 [O]; [U] snap zeroes it (sw)
    float AirCamTimeInAir;          // 0x1F4 [O]; [U] func_001635F8 (updateAirCam)
    float AirSplineScalar;          // 0x1F8 [O]; [U] func_001635F8
    float AirTimeScalar;            // 0x1FC [O]; [U] func_001635F8
    float AirCamMaxTime;            // 0x200 [O]; [U] func_001635F8
    float LastCrouchFactor;         // 0x204 [O]; [U] func_00163270
    float LastTargetCrouchFactor;   // 0x208 [O]; [U] func_00162B90
    float LastAirTargetDelta;       // 0x20C [O]; [U] func_001635F8
    float LastAirPositionDelta;     // 0x210 [O]; [U] func_001635F8
    float ViewCamLastFactor;        // 0x214 [O]; [U] func_001641C0 (updateViewCam)
    float FullAirCamTimeInAir;      // 0x218 [O]; [U] func_001635F8
    float LastFullAirTargetDelta;   // 0x21C [O]; [U] func_001635F8
    float LastFullAirPositionDelta; // 0x220 [O]; [U] func_001635F8
    float AirExitSmoothness;        // 0x224 [O]; [U] func_001635F8
    float VerticalJumpFactor;       // 0x228 [O]; [U] func_001635F8
    float HalfPipeInitWait;         // 0x22C [O]; [U] func_00162A20, func_00164878
    float HalfPipeTimeToLand;       // 0x230 [O]; [U] func_00164878
    float HalfPipeTimeInAir;        // 0x234 [O]; [U] func_00164878
    float HalfPipeRotatorAngle;     // 0x238 [O]; [U] func_00164878
    float HalfPipeBlendOutTimeLeft; // 0x23C [O]; [U] func_00164878
    float HalfPipeCrossBlendTime;   // 0x240 [O]; [U] func_00164878
    float HalfPipeCrossBlendFullTime; // 0x244 [O]; [U] func_00164878
    float SlowCamBlend;             // 0x248 [O]; [U] func_001643A8 (updateSlowSpeedCam; one sw zero)
    float LastHorizontalMoveLen;    // 0x24C [O]; [U] snap
    float LastEnemyPullback;        // 0x250 [O]; [U] snap
    float ShakeNextXTime;           // 0x254 [O]; [U] func_001656B0 (setupCameraShake), func_00165938 (updateCameraShake)
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
    float LastHandplantRise;        // 0x2B8 [O]; [U] func_00163450 (updateHandplant)
    float SteerTime;                // 0x2BC [O]; [U] snap
    int FramesSinceHandplant;       // 0x2C0 [O]; [R] 5000; [U] func_00162998
    int AirCamSetupCountdown;       // 0x2C4 [O]; [U] func_00162A20, func_001635F8
    int JiggleFrameCount;           // 0x2C8 [O]; [U] snap
    int LastAirFrame;               // 0x2CC [O]; [U] func_00162A20
    int DoingHalfPipeCam;           // 0x2D0 [O] bool (a word in retail); [R] 0; [U] func_00162A20, func_001635F8,
                                    //       func_00164878
    int AirborneLastFrame;          // 0x2D4 [O] bool; [U] func_00162A20 (lw/sw)
    int HalfPipeCamInitialized;     // 0x2D8 [O] bool; [R] 0; [U] func_00162A20, func_00164878
    int BoostingLastFrame;          // 0x2DC [O] bool; [U] snap
    int ApplyingAirFunc;            // 0x2E0 [O] bool; [U] func_00162A20, func_001635F8
    int HitGroundSinceStartingAirFunc; // 0x2E4 [O] bool; [U] func_001635F8
    int ApplyingFullAirFunc;        // 0x2E8 [O] bool; [U] func_001635F8
    int ShakingLastFrame;           // 0x2EC [O] bool; [U] func_001656B0, func_00165938
    int DoSnap;                     // 0x2F0 [O] bool; [P] shouldSnapOnUpdate (lw); [U] updateAlgorithm func_001624E8
                                    //       calls snapCurrent when set
    float Setup_Boost;              // 0x2F4 [O]; [U] func_00162568 (swc1)
    float Setup_VisualBoost;        // 0x2F8 [O]; [U] func_00162568 (swc1)
    int Setup_IsTargetHandplanting; // 0x2FC [O] bool; [U] func_00162568 (sw)
    int SlowCamState;               // 0x300 [O] eCamAttributeBlendState; [U] func_001643A8 (lw/sw)
    char AirTargetSpline[0xC];      // 0x304 [O] cCubicSplineInterpolant (bx/cubicspline.h: bitfield word, MaxX,
                                    //       CubicSplines); [R] constructed by func_0031D5E8, destroyed by func_0031D618
    char AirPositionSpline[0xC];    // 0x310 [O] cCubicSplineInterpolant; [R] ditto
    char FullAirTargetSpline[0xC];  // 0x31C [O] cCubicSplineInterpolant; [R] ditto
    char FullAirPositionSpline[0xC]; // 0x328 [O] cCubicSplineInterpolant; [R] ditto
    char HalfPipeAngleSpline[0xC];  // 0x334 [O] cCubicSplineInterpolant; [R] ditto
    cPIDFilter TurningPIDFilter;    // 0x340 [O]; [R] the ctor sets its three factors and calls func_00167DB8 (reset);
                                    //       [U] func_00162C78 runs the filter inline
    char unk_38C[0x4];              // 0x38C padding to the 16-byte class alignment
};

CHECK_OFFSET(cLookAtChaseAlgorithm, View, 0x000);
CHECK_OFFSET(cLookAtChaseAlgorithm, Near, 0x004);
CHECK_OFFSET(cLookAtChaseAlgorithm, Far, 0x008);
CHECK_OFFSET(cLookAtChaseAlgorithm, AlgorithmType, 0x00C);
CHECK_OFFSET(cLookAtChaseAlgorithm, mVtbl, 0x010);
CHECK_OFFSET(cLookAtChaseAlgorithm, CamTargetPos, 0x020);
CHECK_OFFSET(cLookAtChaseAlgorithm, Target, 0x030);
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
