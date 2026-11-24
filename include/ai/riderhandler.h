#ifndef AI_RIDERHANDLER_H
#define AI_RIDERHANDLER_H

// cRiderHandler: the rider "brain" shared by cPlayer (ai/player.h) and cComputer (ai/computer.h), both of which start
// with it at offset 0. It is a composite of 31 rider parts - 6 motions (one per eMotionState), 14 controls (one per
// eControlState) and 11 visual FX - that each derive *virtually* from cRider, so every part holds its own g++ 2.95
// virtual-base pointer (`mRider` below) to the one cRider that follows the complete object (cPlayer + 0xE10,
// cComputer + 0xF50). After the parts come cRiderHandler's own members: a cPlayerLights, MotionState, ControlState
// and its vptr. EA's source: d:/ssx/code/source/ai/riderhandler.cpp (preview assert path; the parts have their own
// files, ai/motion/<x>motion.cpp, ai/control/<x>control.cpp, visualfx/<x>fx.cpp, and could get their own headers).
//
// Size 0xDF0: [R] _vt$13cRiderHandler$6cRider (D_00458780) has offset-to-top -0xDF0, and the constructor
// cRiderHandler_cRiderHandler constructs the cRider at this + 0xDF0 when in charge; cPlayer and cComputer put their
// own fields at 0xDF0. Part order and offsets: [R] that constructor (each part's vbptr store, the out-of-line part
// constructors with their in-charge vbase offsets, and the part vtables' offset-to-top), [R] the state dispatchers
// (cRiderHandler_loseMotionState / cRiderHandler_gainMotionState / cRiderHandler_stepMotion / cRiderHandler_resolveMotion call part N at this + offset for
// MotionState N; cRiderHandler_loseControlState / cRiderHandler_gainControlState / cRiderHandler_updateControl likewise for ControlState) and [R] the state name
// tables D_00459D70 ("Ground" .. "Handplant") and ControlStateNames ("Cruise" .. "Void"). The class names are EA's: the
// preview's _vt$<n><Class>$6cRider vtables and On Tour's base list `class cRiderHandler : cRider, cAirMotion,
// cWipeoutMotion, ... cCheatFX` (same order). NOTE builds/x/v2/vtables_v2.tsv mislabels several retail part vtables
// (e.g. D_00458A68 is cResetControl's, not cHandplantMotion's); src/ai/computer.cpp's equates are right.
//
// Each motion/control part is [state][vbptr][pad]: g++ 2.95 lays a class out as non-virtual bases, then the vbase
// pointers, then its own members, and EA wrote `class cGroundMotion : public cGroundMotionState, virtual public
// cRider` (On Tour's DWARF has the c<Part>State classes and their enums, e.g. cWipeoutMotionState::eWMotionState).
// The state members are [O] On Tour's c<Part>State members, placed by retail use: retail's bools are words, and
// three classes differ from On Tour (cSpinControlState lacks CurrentUberVar, cWipeoutControlState has one
// BoardPrevPose, cCruiseControl has no state). The parts never declare new virtuals (their vtables live in the
// cRider), so they have no vptr of their own. The FX parts are [vbptr][data]: cBoardWakeFX, cBoardSparkFX,
// cSpeedBoostFX, cTrickBoostFX and cAirStreamerFX have On Tour's members placed by retail use (On Tour's
// cSpeedBoostFX / cTrickBoostFX / cAirStreamerFX no longer derive from cRider, so their members sit 4 bytes higher
// in retail, after the vbptr); the other FX parts are raw after the vbptr (their own units have views).
// Tags: [R] retail ctor/dtor/vtables/dispatch, [P] preview inline accessor, [U] retail use, [O] On Tour DWARF.

#include "layout.h"
#include "bx/math3d.h"
// tPose: bx/math3d.h
#include "world/worldview.h" // tBoundBox
#include "render/playerlights.h" // cPlayerLights
#include "render/ps2graphicsman.h" // sDrawState (cBoardSparkFX)
#include "ui/uithing.h" // t3Vector, tARGB
#include "visualfx/boardwakefx.h" // cWakeParams (cBoardWakeFX)

struct cRider;
struct cRiderSphereTree;
struct cDynamicColourEmitter; // render/particle.h
struct tParticleData;         // render/particle.h (On Tour 0xD8; retail copies 0xD8)
struct tParticleEmitter;      // render/particle.h
struct tBoardWakeVertex;      // On Tour 0x20; retail 0x30 (cBoardWakeFX_reset's view)
struct tBxVertex;             // render/2drender.h (cBoardTrailFX's strips)

// On Tour struct tCheatState (4 chars). Retail reads m_misc with lbu only (it also holds the cheat input bits).
struct tCheatState {
    char m_state;           // 0x0 [O]; [U] lb/sb (cCheatFX_update, cCheatFX_updateState)
    char m_index;           // 0x1 [O]; [U] lb/lbu/sb
    char m_count;           // 0x2 [O]; [U] lb/lbu/sb
    char m_misc;            // 0x3 [O]; [U] lbu/sb (setCheatInput cCheatFX_setCheatInput, cCheatFX_clearInput, cCheatFX_getInput)
};

CHECK_OFFSET(tCheatState, m_state, 0x0);
CHECK_OFFSET(tCheatState, m_index, 0x1);
CHECK_OFFSET(tCheatState, m_count, 0x2);
CHECK_OFFSET(tCheatState, m_misc, 0x3);
CHECK_SIZE(tCheatState, 0x4);

// eMotionState / eControlState ([O] enum names; [R] D_00459D70 / ControlStateNames name tables; the ctor stores 6 / 14).
enum eMotionState {
    MS_Ground = 0, MS_Air = 1, MS_Wipeout = 2, MS_Static = 3, MS_Railslide = 4, MS_Handplant = 5, MS_Num = 6
};
enum eControlState {
    CS_Cruise = 0, CS_BoardPress = 1, CS_Prewind = 2, CS_Bump = 3, CS_NaturalAir = 4, CS_Spin = 5, CS_Gate = 6,
    CS_Railslide = 7, CS_Wipeout = 8, CS_Reset = 9, CS_FinishLine = 10, CS_Handplant = 11, CS_UberGrind = 12,
    CS_Void = 13, CS_Num = 14
};

// cGroundMotion (cRiderHandler + 0x000, 0x20 bytes): motion state 0 (MS_Ground). Base cGroundMotionState (0x00-0x18,
// [O] size 0x18).
struct cGroundMotion {
    float ChatterTime;     // 0x00 [O] cGroundMotionState; [U] float (gainFocus, step cGroundMotion_step)
    float PowderDepth1G;   // 0x04 [O]; [U] float (gainFocus, cGroundMotion_getSnowForce, step)
    float PowderDepth3G;   // 0x08 [O]; [U] float (cGroundMotion_getSnowForce, cGroundMotion_getSlipForce, step)
    int ForceNaturalAir;   // 0x0C [O] bool (a word in retail); [U] lw/sw (gainFocus, step, resolve)
    int AITickOnGainFocus; // 0x10 [O]; [R] ctor cGroundMotion_cGroundMotion = 0; [U] gainFocus stores it
    int AITickOnLoseFocus; // 0x14 [O]; [R] ctor = 0; [U] loseFocus (cGroundMotion_loseFocus) stores it, gainFocus reads it
    cRider* mRider;        // 0x18 [R] virtual-base pointer to the shared cRider; [R] ctor cGroundMotion_cGroundMotion (in charge:
                           //      this + 0x20)
    char pad_1C[0x4];      // 0x1C the part is 16-byte aligned
};

CHECK_OFFSET(cGroundMotion, ChatterTime, 0x00);
CHECK_OFFSET(cGroundMotion, PowderDepth1G, 0x04);
CHECK_OFFSET(cGroundMotion, PowderDepth3G, 0x08);
CHECK_OFFSET(cGroundMotion, ForceNaturalAir, 0x0C);
CHECK_OFFSET(cGroundMotion, AITickOnGainFocus, 0x10);
CHECK_OFFSET(cGroundMotion, AITickOnLoseFocus, 0x14);
CHECK_OFFSET(cGroundMotion, mRider, 0x18);
CHECK_OFFSET(cGroundMotion, pad_1C, 0x1C);
CHECK_SIZE(cGroundMotion, 0x20);

// cAirMotion (cRiderHandler + 0x020, 0x10 bytes): motion state 1 (MS_Air). Base cAirMotionState (0x00-0x04).
struct cAirMotion {
    int IsLandVelAlign; // 0x00 [O] cAirMotionState (bool; a word in retail); [U] gainFocus sw, step lw
    cRider* mRider;     // 0x04 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x10
                        //      (D_004599E8)
    char pad_08[0x8];   // 0x08
};

CHECK_OFFSET(cAirMotion, IsLandVelAlign, 0x00);
CHECK_OFFSET(cAirMotion, mRider, 0x04);
CHECK_OFFSET(cAirMotion, pad_08, 0x08);
CHECK_SIZE(cAirMotion, 0x10);

// cWipeoutMotion (cRiderHandler + 0x030, 0x70 bytes): motion state 2 (MS_Wipeout). Base cWipeoutMotionState
// (0x00-0x40, [O] size 0x40).
struct cWipeoutMotion {
    int WipeoutMotionState;            // 0x00 [P] getWipeoutMotionState (retail cWipeoutMotion_getWipeoutMotionState lw 0x0); [O]
                                       //      eWMotionState (WMS_Ground 0, WMS_Air 1)
    int ForceNaturalAir;               // 0x04 [O] bool (word); [U] lw/sw
    char pad_08[0x8];                  // 0x08
    t4Vector BoardVelocity;            // 0x10 [O]; [U] lq/sq/lqc2 (cWipeoutMotion_throwItem, func_00136F30, func_00137138)
    t4Vector BoardAngVelocity;         // 0x20 [O]; [U] lq/sq/lqc2
    t4Vector RiderAngVelocity;         // 0x30 [P] getRiderAngVel (retail cWipeoutMotion_getRiderAngVel = this + 0x30); [O]
    cRider* mRider;                    // 0x40 [R] virtual-base pointer to the shared cRider; [R] ctor cWipeoutMotion_cWipeoutMotion (in
                                       //      charge: this + 0x70)
    cRiderSphereTree* BoardSphereTree; // 0x44 [P] getBoardSphereTree (retail cWipeoutMotion_getBoardSphereTree lw 0x44; preview 0x48); [O]
    char pad_48[0x8];                  // 0x48
    tBoundBox BoardIntersectBox;       // 0x50 [P] getBoardBoundBox (cWipeoutMotion_getBoardBoundBox = this + 0x50); [O]; [U] lq/sq
};

CHECK_OFFSET(cWipeoutMotion, WipeoutMotionState, 0x00);
CHECK_OFFSET(cWipeoutMotion, ForceNaturalAir, 0x04);
CHECK_OFFSET(cWipeoutMotion, pad_08, 0x08);
CHECK_OFFSET(cWipeoutMotion, BoardVelocity, 0x10);
CHECK_OFFSET(cWipeoutMotion, BoardAngVelocity, 0x20);
CHECK_OFFSET(cWipeoutMotion, RiderAngVelocity, 0x30);
CHECK_OFFSET(cWipeoutMotion, mRider, 0x40);
CHECK_OFFSET(cWipeoutMotion, BoardSphereTree, 0x44);
CHECK_OFFSET(cWipeoutMotion, pad_48, 0x48);
CHECK_OFFSET(cWipeoutMotion, BoardIntersectBox, 0x50);
CHECK_SIZE(cWipeoutMotion, 0x70);

// cStaticMotion (cRiderHandler + 0x0A0, 0x10 bytes): motion state 3 (MS_Static). No state base.
struct cStaticMotion {
    cRider* mRider;   // 0x00 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x10 (D_00459700)
    char pad_04[0xC]; // 0x04
};

CHECK_OFFSET(cStaticMotion, mRider, 0x00);
CHECK_OFFSET(cStaticMotion, pad_04, 0x04);
CHECK_SIZE(cStaticMotion, 0x10);

// cRailslideMotion (cRiderHandler + 0x0B0, 0x60 bytes): motion state 4 (MS_Railslide). Base cRailslideMotionState
// (0x00-0x50, [O] size 0x50).
struct cRailslideMotion {
    t4Vector RailNorm;       // 0x00 [O]; [R] cRiderHandler ctor = t4vect_ZAxis; [U] read via handler+0xB0 (getRailNorm)
    float RailLeanAngle;     // 0x10 [O]; [U] float
    float RailTime;          // 0x14 [O]; [U] float
    float RailOffCenter;     // 0x18 [O]; [R] cRiderHandler ctor = 0; [U] read via handler+0xC8 (getOffCenter)
    float CrookedSlideAngle; // 0x1C [O]; [U] float
    int ForceNaturalAir;     // 0x20 [O] bool (word); [U] lw/sw
    int SplineID;            // 0x24 [O] tResourceID (4 bytes); [R] cRiderHandler ctor = -1; [U] read via handler+0xD4
                             //      (getSplineID)
    char pad_28[0x8];        // 0x28
    t4Vector PrevPos;        // 0x30 [O]; [U] lqc2/sq
    float StuckSec;          // 0x40 [O]; [U] float
    char pad_44[0xC];        // 0x44
    cRider* mRider;          // 0x50 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x60
                             //      (D_004598F0)
    char pad_54[0xC];        // 0x54
};

CHECK_OFFSET(cRailslideMotion, RailNorm, 0x00);
CHECK_OFFSET(cRailslideMotion, RailLeanAngle, 0x10);
CHECK_OFFSET(cRailslideMotion, RailTime, 0x14);
CHECK_OFFSET(cRailslideMotion, RailOffCenter, 0x18);
CHECK_OFFSET(cRailslideMotion, CrookedSlideAngle, 0x1C);
CHECK_OFFSET(cRailslideMotion, ForceNaturalAir, 0x20);
CHECK_OFFSET(cRailslideMotion, SplineID, 0x24);
CHECK_OFFSET(cRailslideMotion, pad_28, 0x28);
CHECK_OFFSET(cRailslideMotion, PrevPos, 0x30);
CHECK_OFFSET(cRailslideMotion, StuckSec, 0x40);
CHECK_OFFSET(cRailslideMotion, pad_44, 0x44);
CHECK_OFFSET(cRailslideMotion, mRider, 0x50);
CHECK_OFFSET(cRailslideMotion, pad_54, 0x54);
CHECK_SIZE(cRailslideMotion, 0x60);

// cHandplantMotion (cRiderHandler + 0x110, 0xB0 bytes): motion state 5 (MS_Handplant). Base cHandplantMotionState
// (0x00-0xA0, [O] size 0xA0).
struct cHandplantMotion {
    t4Vector InitPos;          // 0x00 [O]; [U] sq/lqc2 (cHandplantMotion_init)
    t4Vector ExitVel;          // 0x10 [O]; [U] vector and x/y/z/w reads (cHandplantMotion_calcExitVel calcExitVel)
    t4Vector SplineDir;        // 0x20 [O]; [U]
    t4Vector SplinePos;        // 0x30 [O]; [U] gainFocus: rider velocity = (SplinePos - pos) / EnterSec
    t4Vector ApproachDir;      // 0x40 [O]; [U]
    t4Vector AirPredictorPos;  // 0x50 [O]; [U] sq
    t4Vector AirPredictorVel;  // 0x60 [O]; [U] sq
    t4Vector SnapRotationAxis; // 0x70 [O]; [U] sq, x/y/z reads
    float SnapRotationRate;    // 0x80 [O]; [U] float
    float EnterSec;            // 0x84 [O]; [U] float (gainFocus divides by it)
    float ElapsedSec;          // 0x88 [O]; [U] float
    float InitTangentSpeed;    // 0x8C [O]; [U] float
    int IsRailSpline;          // 0x90 [O] bool (word); [U] lw/sw
    int IsToeside;             // 0x94 [O] bool (word); [U] lw/sw; read via handler+0x1A4 (isToeside)
    char pad_98[0x8];          // 0x98
    cRider* mRider;            // 0xA0 [R] virtual-base pointer to the shared cRider; [R] ctor cHandplantMotion_cHandplantMotion (in charge:
                               //      this + 0xB0)
    char pad_A4[0xC];          // 0xA4
};

CHECK_OFFSET(cHandplantMotion, InitPos, 0x00);
CHECK_OFFSET(cHandplantMotion, ExitVel, 0x10);
CHECK_OFFSET(cHandplantMotion, SplineDir, 0x20);
CHECK_OFFSET(cHandplantMotion, SplinePos, 0x30);
CHECK_OFFSET(cHandplantMotion, ApproachDir, 0x40);
CHECK_OFFSET(cHandplantMotion, AirPredictorPos, 0x50);
CHECK_OFFSET(cHandplantMotion, AirPredictorVel, 0x60);
CHECK_OFFSET(cHandplantMotion, SnapRotationAxis, 0x70);
CHECK_OFFSET(cHandplantMotion, SnapRotationRate, 0x80);
CHECK_OFFSET(cHandplantMotion, EnterSec, 0x84);
CHECK_OFFSET(cHandplantMotion, ElapsedSec, 0x88);
CHECK_OFFSET(cHandplantMotion, InitTangentSpeed, 0x8C);
CHECK_OFFSET(cHandplantMotion, IsRailSpline, 0x90);
CHECK_OFFSET(cHandplantMotion, IsToeside, 0x94);
CHECK_OFFSET(cHandplantMotion, pad_98, 0x98);
CHECK_OFFSET(cHandplantMotion, mRider, 0xA0);
CHECK_OFFSET(cHandplantMotion, pad_A4, 0xA4);
CHECK_SIZE(cHandplantMotion, 0xB0);

// cCruiseControl (cRiderHandler + 0x1C0, 0x10 bytes): control state 0 (CS_Cruise). No state base in retail (On Tour
// added cCruiseControlState).
struct cCruiseControl {
    cRider* mRider;   // 0x00 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x10 (D_00459510)
    char pad_04[0xC]; // 0x04
};

CHECK_OFFSET(cCruiseControl, mRider, 0x00);
CHECK_OFFSET(cCruiseControl, pad_04, 0x04);
CHECK_SIZE(cCruiseControl, 0x10);

// cBoardPressControl (cRiderHandler + 0x1D0, 0x20 bytes): control state 1 (CS_BoardPress). Base
// cBoardPressControlState (0x00-0x14).
struct cBoardPressControl {
    int State;           // 0x00 [O] eBoardPressState (BP_Into 0, BP_Balance 1, BP_IntoAir 2, BP_Exit 3); [U] lw/sw
    float BalanceSec;    // 0x04 [O]; [U] float
    float OffBalanceSec; // 0x08 [O]; [U] float
    float PressTimer;    // 0x0C [O]; [U] float
    int IsOllieHeld;     // 0x10 [O] bool (word); [U] lw/sw
    cRider* mRider;      // 0x14 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x20
                         //      (D_00459418)
    char pad_18[0x8];    // 0x18
};

CHECK_OFFSET(cBoardPressControl, State, 0x00);
CHECK_OFFSET(cBoardPressControl, BalanceSec, 0x04);
CHECK_OFFSET(cBoardPressControl, OffBalanceSec, 0x08);
CHECK_OFFSET(cBoardPressControl, PressTimer, 0x0C);
CHECK_OFFSET(cBoardPressControl, IsOllieHeld, 0x10);
CHECK_OFFSET(cBoardPressControl, mRider, 0x14);
CHECK_OFFSET(cBoardPressControl, pad_18, 0x18);
CHECK_SIZE(cBoardPressControl, 0x20);

// cPrewindControl (cRiderHandler + 0x1F0, 0x10 bytes): control state 2 (CS_Prewind). No state base.
struct cPrewindControl {
    cRider* mRider;   // 0x00 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x10 (D_00459320)
    char pad_04[0xC]; // 0x04
};

CHECK_OFFSET(cPrewindControl, mRider, 0x00);
CHECK_OFFSET(cPrewindControl, pad_04, 0x04);
CHECK_SIZE(cPrewindControl, 0x10);

// cBumpControl (cRiderHandler + 0x200, 0x10 bytes): control state 3 (CS_Bump). No state base.
struct cBumpControl {
    cRider* mRider;   // 0x00 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x10 (D_00459228)
    char pad_04[0xC]; // 0x04
};

CHECK_OFFSET(cBumpControl, mRider, 0x00);
CHECK_OFFSET(cBumpControl, pad_04, 0x04);
CHECK_SIZE(cBumpControl, 0x10);

// cNaturalAirControl (cRiderHandler + 0x210, 0x20 bytes): control state 4 (CS_NaturalAir). Base
// cNaturalAirControlState (0x00-0x14; On Tour 0x10 with byte bools).
struct cNaturalAirControl {
    float LimitAng;   // 0x00 [O]; [U] float (gainFocus cNaturalAirControl_gainFocus)
    float LimitMag;   // 0x04 [O]; [U] float
    int IsPunchHeld;  // 0x08 [O] bool (word); [U] lw/sw
    int IsGrabHeld;   // 0x0C [O] bool (word); [U] lw/sw
    int InitGrabHeld; // 0x10 [O]; [U] lw/sw
    cRider* mRider;   // 0x14 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x20 (D_00459130)
    char pad_18[0x8]; // 0x18
};

CHECK_OFFSET(cNaturalAirControl, LimitAng, 0x00);
CHECK_OFFSET(cNaturalAirControl, LimitMag, 0x04);
CHECK_OFFSET(cNaturalAirControl, IsPunchHeld, 0x08);
CHECK_OFFSET(cNaturalAirControl, IsGrabHeld, 0x0C);
CHECK_OFFSET(cNaturalAirControl, InitGrabHeld, 0x10);
CHECK_OFFSET(cNaturalAirControl, mRider, 0x14);
CHECK_OFFSET(cNaturalAirControl, pad_18, 0x18);
CHECK_SIZE(cNaturalAirControl, 0x20);

// cSpinControl (cRiderHandler + 0x230, 0x60 bytes): control state 5 (CS_Spin). Base cSpinControlState (0x00-0x58; On
// Tour 0x5C: it adds CurrentUberVar at 0xC).
struct cSpinControl {
    int AirAdjState;            // 0x00 [O] eAirAdjState (AS_AirAdj 0, AS_Spin 1, AS_LateSpin 2, AS_None 3); [U] lw/sw
                                //      (update stores 3)
    int GrabState;              // 0x04 [O] eGrabState (GS_Start 0 .. GS_Over 5); [U] isGrabHeld/isTweakHeld/isUberHeld
                                //      (cSpinControl_isGrabHeld/EF0/F30) test 2 and 4
    int CurrentGrab;            // 0x08 [O]; [U] gainFocus = -1
    int SpinState;              // 0x0C [O] eSpinState (SS_Into 0, SS_Cycle 1, SS_OutOf 2, SS_None 3); [U] update stores
                                //      1/2/3 (On Tour has CurrentUberVar here)
    float DesFlip;              // 0x10 [O]; [U] predictFlipSec (cSpinControl_predictFlipSec: DesFlip, CurFlip, MaxFlipAngVel)
    float DesSpin;              // 0x14 [O]; [U] predictSpinSec (cSpinControl_predictSpinSec: DesSpin, CurSpin, MaxSpinAngVel)
    float CurFlip;              // 0x18 [O]; [U] float
    float CurSpin;              // 0x1C [O]; [U] float
    float TrickSpin;            // 0x20 [O]; [U] float
    float TrickFlip;            // 0x24 [O]; [U] float
    float AirAdjSpin;           // 0x28 [O]; [U] float
    float AirAdjFlip;           // 0x2C [O]; [U] float
    float TotalSpinAng;         // 0x30 [O]; [U] float
    float TotalFlipAng;         // 0x34 [O]; [U] float
    float MaxSpinAngVel;        // 0x38 [O]; [U] predictSpinSec
    float MaxFlipAngVel;        // 0x3C [O]; [U] predictFlipSec
    float FlipAxisWeight;       // 0x40 [O]; [U] float
    int IsPartialLateSpin;      // 0x44 [O] bool (word); [U] lw/sw
    float DesSpinInputSec;      // 0x48 [O]; [U] float
    float DesFlipInputSec;      // 0x4C [O]; [U] float
    float FlipSpinInputAng;     // 0x50 [O]; [U] getFlipSpinInputAng (cSpinControl_getFlipSpinInputAng: 1e30 = none)
    float NullFlipSpinInputSec; // 0x54 [O]; [U] float
    cRider* mRider;             // 0x58 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x60
                                //      (D_00459038)
    char pad_5C[0x4];           // 0x5C
};

CHECK_OFFSET(cSpinControl, AirAdjState, 0x00);
CHECK_OFFSET(cSpinControl, GrabState, 0x04);
CHECK_OFFSET(cSpinControl, CurrentGrab, 0x08);
CHECK_OFFSET(cSpinControl, SpinState, 0x0C);
CHECK_OFFSET(cSpinControl, DesFlip, 0x10);
CHECK_OFFSET(cSpinControl, DesSpin, 0x14);
CHECK_OFFSET(cSpinControl, CurFlip, 0x18);
CHECK_OFFSET(cSpinControl, CurSpin, 0x1C);
CHECK_OFFSET(cSpinControl, TrickSpin, 0x20);
CHECK_OFFSET(cSpinControl, TrickFlip, 0x24);
CHECK_OFFSET(cSpinControl, AirAdjSpin, 0x28);
CHECK_OFFSET(cSpinControl, AirAdjFlip, 0x2C);
CHECK_OFFSET(cSpinControl, TotalSpinAng, 0x30);
CHECK_OFFSET(cSpinControl, TotalFlipAng, 0x34);
CHECK_OFFSET(cSpinControl, MaxSpinAngVel, 0x38);
CHECK_OFFSET(cSpinControl, MaxFlipAngVel, 0x3C);
CHECK_OFFSET(cSpinControl, FlipAxisWeight, 0x40);
CHECK_OFFSET(cSpinControl, IsPartialLateSpin, 0x44);
CHECK_OFFSET(cSpinControl, DesSpinInputSec, 0x48);
CHECK_OFFSET(cSpinControl, DesFlipInputSec, 0x4C);
CHECK_OFFSET(cSpinControl, FlipSpinInputAng, 0x50);
CHECK_OFFSET(cSpinControl, NullFlipSpinInputSec, 0x54);
CHECK_OFFSET(cSpinControl, mRider, 0x58);
CHECK_OFFSET(cSpinControl, pad_5C, 0x5C);
CHECK_SIZE(cSpinControl, 0x60);

// cGateControl (cRiderHandler + 0x290, 0x20 bytes): control state 6 (CS_Gate). Base cGateControlState (0x00-0x14).
struct cGateControl {
    int GateState;          // 0x00 [O] eGateState (GS_GateRock 0, GS_GateLaunch 1, GS_Sit 2, GS_GetupFromSit 3); [R]
                            //      cRiderHandler ctor = 0; [U] set via handler+0x290
    float HeldAtExtremeSec; // 0x04 [O]; [U] float
    float CurThrustMin;     // 0x08 [O]; [U] float
    float CurThrustMax;     // 0x0C [O]; [U] float
    float GateAnticipate;   // 0x10 [O]; [U] read via handler+0x2A0 (getGateAnticipate)
    cRider* mRider;         // 0x14 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x20
                            //      (D_00458F40)
    char pad_18[0x8];       // 0x18
};

CHECK_OFFSET(cGateControl, GateState, 0x00);
CHECK_OFFSET(cGateControl, HeldAtExtremeSec, 0x04);
CHECK_OFFSET(cGateControl, CurThrustMin, 0x08);
CHECK_OFFSET(cGateControl, CurThrustMax, 0x0C);
CHECK_OFFSET(cGateControl, GateAnticipate, 0x10);
CHECK_OFFSET(cGateControl, mRider, 0x14);
CHECK_OFFSET(cGateControl, pad_18, 0x18);
CHECK_SIZE(cGateControl, 0x20);

// cRailslideControl (cRiderHandler + 0x2B0, 0x10 bytes): control state 7 (CS_Railslide). Base cRailslideControlState
// (0x00-0x08).
struct cRailslideControl {
    float RSpinAngle;  // 0x00 [O]; [U] float
    int PrevGrindHeld; // 0x04 [O]; [U] lw/sw
    cRider* mRider;    // 0x08 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x10
                       //      (D_00458E48)
    char pad_0C[0x4];  // 0x0C
};

CHECK_OFFSET(cRailslideControl, RSpinAngle, 0x00);
CHECK_OFFSET(cRailslideControl, PrevGrindHeld, 0x04);
CHECK_OFFSET(cRailslideControl, mRider, 0x08);
CHECK_OFFSET(cRailslideControl, pad_0C, 0x0C);
CHECK_SIZE(cRailslideControl, 0x10);

// cWipeoutControl (cRiderHandler + 0x2C0, 0x90 bytes): control state 8 (CS_Wipeout). Base cWipeoutControlState
// (0x00-0x80; On Tour 0xA0: BoardPrevPose[2]).
struct cWipeoutControl {
    int RiderState;         // 0x00 [O] eWRiderState (RS_Launch 0, RS_Air 1, RS_Ground 2, RS_Recover 3, RS_Stuck 4); [U]
                            //      lw/sw
    char pad_04[0xC];       // 0x04
    tPose RiderPrevPose;    // 0x10 [O]; [U] lq/sq of both vectors
    tPose BoardPrevPose;    // 0x30 [O] (On Tour [2]: one per ski); [U] lq/sq at 0x30/0x40
    float PrevFramePercent; // 0x50 [O]; [U] float
    int IsNewImpact;        // 0x54 [O] bool (word); [U] lw/sw; set via handler+0x314 (impact)
    char pad_58[0x8];       // 0x58
    t4Vector LatestImpact;  // 0x60 [O]; [U] lqc2; stored via handler+0x320 (impact)
    float RecoverPercent;   // 0x70 [O]; [U] float; read via handler+0x330 (getRecoverPercent)
    char pad_74[0xC];       // 0x74
    cRider* mRider;         // 0x80 [R] virtual-base pointer to the shared cRider; [R] ctor cWipeoutControl_cWipeoutControl (in charge:
                            //      this + 0x90)
    char pad_84[0xC];       // 0x84
};

CHECK_OFFSET(cWipeoutControl, RiderState, 0x00);
CHECK_OFFSET(cWipeoutControl, pad_04, 0x04);
CHECK_OFFSET(cWipeoutControl, RiderPrevPose, 0x10);
CHECK_OFFSET(cWipeoutControl, BoardPrevPose, 0x30);
CHECK_OFFSET(cWipeoutControl, PrevFramePercent, 0x50);
CHECK_OFFSET(cWipeoutControl, IsNewImpact, 0x54);
CHECK_OFFSET(cWipeoutControl, pad_58, 0x58);
CHECK_OFFSET(cWipeoutControl, LatestImpact, 0x60);
CHECK_OFFSET(cWipeoutControl, RecoverPercent, 0x70);
CHECK_OFFSET(cWipeoutControl, pad_74, 0x74);
CHECK_OFFSET(cWipeoutControl, mRider, 0x80);
CHECK_OFFSET(cWipeoutControl, pad_84, 0x84);
CHECK_SIZE(cWipeoutControl, 0x90);

// cResetControl (cRiderHandler + 0x350, 0x10 bytes): control state 9 (CS_Reset). Base cResetControlState (0x00-0x08).
struct cResetControl {
    int IsAutoReset;   // 0x00 [O] bool (word); [U] lw; set via handler+0x350 (setIsAutoReset)
    float FadePercent; // 0x04 [O]; [U] float
    cRider* mRider;    // 0x08 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x10
                       //      (D_00458A68)
    char pad_0C[0x4];  // 0x0C
};

CHECK_OFFSET(cResetControl, IsAutoReset, 0x00);
CHECK_OFFSET(cResetControl, FadePercent, 0x04);
CHECK_OFFSET(cResetControl, mRider, 0x08);
CHECK_OFFSET(cResetControl, pad_0C, 0x0C);
CHECK_SIZE(cResetControl, 0x10);

// cFinishLineControl (cRiderHandler + 0x360, 0x10 bytes): control state 10 (CS_FinishLine). Base
// cFinishLineControlState (0x00-0x04).
struct cFinishLineControl {
    int FinishState;  // 0x00 [O] eFinishState (Brake 0, IntoStance 1, Reaction 2, Done 3); [U] lw/sw
    cRider* mRider;   // 0x04 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x10 (D_00458B60)
    char pad_08[0x8]; // 0x08
};

CHECK_OFFSET(cFinishLineControl, FinishState, 0x00);
CHECK_OFFSET(cFinishLineControl, mRider, 0x04);
CHECK_OFFSET(cFinishLineControl, pad_08, 0x08);
CHECK_SIZE(cFinishLineControl, 0x10);

// cHandplantControl (cRiderHandler + 0x370, 0x20 bytes): control state 11 (CS_Handplant). Base cHandplantControlState
// (0x00-0x14; On Tour 0x10 with byte bools).
struct cHandplantControl {
    float EnterSec;        // 0x00 [O]; [U] float
    float CurBalance;      // 0x04 [O]; [U] float
    int IsMissedHandplant; // 0x08 [O] bool (word); [U] lw/sw
    int IsRailSpline;      // 0x0C [O] bool (word); [U] lw/sw
    int HandplantType;     // 0x10 [O] eHandplantType (HT_HS 0, HT_TS 1); [U] lw/sw
    cRider* mRider;        // 0x14 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x20
                           //      (D_00458878)
    char pad_18[0x8];      // 0x18
};

CHECK_OFFSET(cHandplantControl, EnterSec, 0x00);
CHECK_OFFSET(cHandplantControl, CurBalance, 0x04);
CHECK_OFFSET(cHandplantControl, IsMissedHandplant, 0x08);
CHECK_OFFSET(cHandplantControl, IsRailSpline, 0x0C);
CHECK_OFFSET(cHandplantControl, HandplantType, 0x10);
CHECK_OFFSET(cHandplantControl, mRider, 0x14);
CHECK_OFFSET(cHandplantControl, pad_18, 0x18);
CHECK_SIZE(cHandplantControl, 0x20);

// cUberGrindControl (cRiderHandler + 0x390, 0x10 bytes): control state 12 (CS_UberGrind). Base cUberGrindControlState
// (0x00-0x08).
struct cUberGrindControl {
    int GrindState;   // 0x00 [O] eGrindState (GS_Locked 0, GS_Hold 1, GS_Over 2); [U] lw/sw
    int CurrentGrind; // 0x04 [O]; [U] lw; read/written via handler+0x394 (get/setCurrentGrind)
    cRider* mRider;   // 0x08 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x10 (D_00458D50)
    char pad_0C[0x4]; // 0x0C
};

CHECK_OFFSET(cUberGrindControl, GrindState, 0x00);
CHECK_OFFSET(cUberGrindControl, CurrentGrind, 0x04);
CHECK_OFFSET(cUberGrindControl, mRider, 0x08);
CHECK_OFFSET(cUberGrindControl, pad_0C, 0x0C);
CHECK_SIZE(cUberGrindControl, 0x10);

// cVoidControl (cRiderHandler + 0x3A0, 0x10 bytes): control state 13 (CS_Void). No state base.
struct cVoidControl {
    cRider* mRider;   // 0x00 [R] virtual-base pointer to the shared cRider; [R] vtable offset-to-top -0x10 (D_00458970)
    char pad_04[0xC]; // 0x04
};

CHECK_OFFSET(cVoidControl, mRider, 0x00);
CHECK_OFFSET(cVoidControl, pad_04, 0x04);
CHECK_SIZE(cVoidControl, 0x10);

// cBoardWakeFX (cRiderHandler + 0x3B0, 0xC0 bytes): FX part; ctor cBoardWakeFX_cBoardWakeFX (in charge: cRider at this
// + 0xC0); vtable cBoardWakeFX$6cRider D_00488DF0.
// Members: [O] On Tour's cBoardWakeFX (same order; retail has 5 vertex rows where On Tour has 4, so mWakeVerts and
// mPreCalculatedGravityMultiplier have 5 entries and the members after them sit 8 bytes higher until mLastWakePos;
// retail has no mWorldOffset). [P] the preview's IsWakeActive / GetChunkySprayVelocity and its asserts
// "mWakeLineSegs %d kMaxWakeLineSegs %d" / "mWakeMaxNumLines %d kMaxWakeNumLines %d" (d:/ssx/code/source/visualfx/
// boardwakefx.cpp). [U] the unit's views (cBoardWakeFX_reset sWakeDCF28, cBoardWakeFX_update sWakeD0B8,
// cBoardWakeFX_createSeg sWake_E058).
// One wake line of cBoardWakeFX (On Tour struct tWakeLine, 0x60 with mVel[4]; retail 0x70 with five rows, like
// mWakeVerts[5]). [R] cBoardWakeFX_cBoardWakeFX: operator new[](32 x 0x70, "WakeLines_cBoardWakeFX"); [P] the preview's
// GetChunkySprayVelocity__12cBoardWakeFX steps mWakeLines by 0x70 and returns mVel[mWakeLineSegs - 1] (retail
// cBoardWakeFX_GetChunkySprayVelocity); [U] cBoardWakeFX_fillCurrentSeg fills one, cBoardWakeFX_update ages and drifts them.
struct tWakeLine {
    t4Vector mVel[5];                    // 0x00 [O] mVel[4] (retail 5 rows); [P] GetChunkySprayVelocity returns row
                                         //      mWakeLineSegs - 1; [U] fillCurrentSeg: row i = (side * t * t * 0.75 + up
                                         //      * t) * the wake speed
    t4Vector mAngleCorrection;           // 0x50 [O] (On Tour 0x40, after mVel); [U] fillCurrentSeg: 0.85 * (mVelocity
                                         //      - forward * |mVelocity|); update drifts the line by it, fading with age
    float mExtraInitalVelocityFactor;    // 0x60 [O] (EA's spelling; On Tour 0x50); [U] fillCurrentSeg stores its last
                                         //      argument ([O] fillCurrentSeg(..., float extraInitialVelocity)); update
                                         //      scales a young line's growth by it
    float mAge;                          // 0x64 [O] (On Tour 0x54); [U] fillCurrentSeg zeroes it; update adds 1/60 a
                                         //      frame and drops the line once it passes mWakeLife
    char unk_068[0x8];                   // 0x68 padding to the 16-aligned size
};

CHECK_OFFSET(tWakeLine, mVel, 0x00);
CHECK_OFFSET(tWakeLine, mAngleCorrection, 0x50);
CHECK_OFFSET(tWakeLine, mExtraInitalVelocityFactor, 0x60);
CHECK_OFFSET(tWakeLine, mAge, 0x64);
CHECK_OFFSET(tWakeLine, unk_068, 0x68);
CHECK_SIZE(tWakeLine, 0x70);

struct cBoardWakeFX {
    cRider* mRider;                           // 0x00 [R] virtual-base pointer to the shared cRider; [R] cBoardWakeFX_cBoardWakeFX
    int mNumWakeLines;                        // 0x04 [O]; [P] IsWakeActive (preview lw 0x4, > 0); [U] reset 0, createSeg
                                              //      counts it up to mWakeMaxNumLines, update cuts it at the first dead line
    int mCurrentWakeLine;                     // 0x08 [O]; [P] GetChunkySprayVelocity (preview lw 0x8, x 0x70); [U] reset 0,
                                              //      createSeg moves it back one place in the ring, update walks from it
    tWakeLine* mWakeLines;                    // 0x0C [O]; [P] GetChunkySprayVelocity (preview lw 0xC); [R] ctor operator
                                              //      new[](32 x 0x70, "WakeLines_cBoardWakeFX"); dtor frees it
    tBoardWakeVertex* mWakeVerts[5];          // 0x10 [O] (On Tour [4]); [R] ctor: one "WakeVerts_cBoardWakeFX" block of
                                              //      160 x 0x30, row i = row 0 + i * 32; dtor frees row 0; [U] reset, update
    float mPreCalculatedGravityMultiplier[5]; // 0x24 [O] (On Tour [4] at 0x20); [U] reset: row i = i / (mWakeLineSegs - 1)
                                              //      * -25; update adds row j's to its points' z each frame
    float mWakeRandomnessSeed;                // 0x38 [O] (On Tour 0x30); [U] reset 0; createSeg advances it by 1/30 (wraps
                                              //      at 4); fillCurrentSeg feeds it to perlinNoise
    int mWasSpraying;                         // 0x3C [O] bool (On Tour 0x34; a word in retail); [U] reset 0; update sets 1
                                              //      when a wake starts, 0 when it ends
    int mWakeLength;                          // 0x40 [O] (On Tour 0x38); [U] reset / update (wake start) 0; createSeg
                                              //      gives the new segment u = mWakeLength * 0.5, then increments it
    char pad_44[0xC];                         // 0x44 (mLastWakePos is 16-aligned)
    t4Vector mLastWakePos;                    // 0x50 [O] (On Tour 0x40; retail drops On Tour's mWorldOffset); [U] update:
                                              //      the wake start / last createSeg position (a new segment every 50 units)
    cWakeParams mWakeParams;                  // 0x60 [O]; [U] reset cWakeParams_Clear (this + 0x60); update blends it;
                                              //      fillCurrentSeg builds the ribbon from it (visualfx/boardwakefx.h)
    tARGB mGroundColour;                      // 0xA0 [O]; [U] reset white; createSeg the surface colour doubled, clamped
    int mLeanPositive;                        // 0xB0 [O] bool (a word in retail); [U] update: = (0 < lean) when the wake
                                              //      starts; the wake stops when the lean changes side
    int mWakeLineSegs;                        // 0xB4 [O]; [P] assert "mWakeLineSegs %d kMaxWakeLineSegs %d" and
                                              //      GetChunkySprayVelocity (lw 0xB4); [R] ctor -1; [U] reset 5 (a rider
                                              //      with a view) / 4: the rows used
    int mWakeMaxNumLines;                     // 0xB8 [O]; [P] assert "mWakeMaxNumLines %d kMaxWakeNumLines %d"; [U] reset
                                              //      32 / 22; update / createSeg: the ring size
    float mWakeLife;                          // 0xBC [O]; [U] reset 1.25f / 0.8f; update drops lines older than it
};

CHECK_OFFSET(cBoardWakeFX, mRider, 0x00);
CHECK_OFFSET(cBoardWakeFX, mNumWakeLines, 0x04);
CHECK_OFFSET(cBoardWakeFX, mCurrentWakeLine, 0x08);
CHECK_OFFSET(cBoardWakeFX, mWakeLines, 0x0C);
CHECK_OFFSET(cBoardWakeFX, mWakeVerts, 0x10);
CHECK_OFFSET(cBoardWakeFX, mPreCalculatedGravityMultiplier, 0x24);
CHECK_OFFSET(cBoardWakeFX, mWakeRandomnessSeed, 0x38);
CHECK_OFFSET(cBoardWakeFX, mWasSpraying, 0x3C);
CHECK_OFFSET(cBoardWakeFX, mWakeLength, 0x40);
CHECK_OFFSET(cBoardWakeFX, pad_44, 0x44);
CHECK_OFFSET(cBoardWakeFX, mLastWakePos, 0x50);
CHECK_OFFSET(cBoardWakeFX, mWakeParams, 0x60);
CHECK_OFFSET(cBoardWakeFX, mGroundColour, 0xA0);
CHECK_OFFSET(cBoardWakeFX, mLeanPositive, 0xB0);
CHECK_OFFSET(cBoardWakeFX, mWakeLineSegs, 0xB4);
CHECK_OFFSET(cBoardWakeFX, mWakeMaxNumLines, 0xB8);
CHECK_OFFSET(cBoardWakeFX, mWakeLife, 0xBC);
CHECK_SIZE(cBoardWakeFX, 0xC0);

// cBoardSparkFX (cRiderHandler + 0x470, 0xB0 bytes): FX part; ctor cBoardSparkFX_cBoardSparkFX (in charge: cRider at
// this + 0xB0); vtable cBoardSparkFX$6cRider D_00488CF8.
// Members: [O] On Tour's cBoardSparkFX (same order and, up to mSparkEmitter, the same offsets; retail keeps the three
// flash sprites as 3 positions + 3 sizes instead of On Tour's tSpriteBasic mFlashSprites[3], its bools are words and
// its sDrawState is 0x14 bytes). [R] the ctor's allocation tags ("mEmitter", "mEmitterData" are the member names).
// [U] the unit's views (cBoardSparkFX_reset, cBoardSparkFX_update, cBoardSparkFX_render).
struct cBoardSparkFX {
    cRider* mRider;                     // 0x00 [R] virtual-base pointer to the shared cRider; [R] cBoardSparkFX_cBoardSparkFX
    char pad_04[0xC];                   // 0x04 (mFlashBasePosition is 16-aligned)
    t4Vector mFlashBasePosition;        // 0x10 [O]; [U] reset t4vect_Origin; update = board position + velocity * 0.01,
                                        //      the flash sprites are scattered +-4 around it
    cDynamicColourEmitter* mEmitter;    // 0x20 [O]; [R] ctor cDynamicColourEmitter_cDynamicColourEmitter(cMemMan_alloc(
                                        //      0x210, "mEmitter")); [U] update (cDynamicColourEmitter_update), render
    tParticleData* mEmitterData;        // 0x24 [O]; [R] cMemMan_alloc(0xD8, "mEmitterData"), filled by the ctor; [U]
                                        //      update hands it to mEmitter's setupEmitter (vtable slot 3)
    tParticleEmitter* mSparkEmitter;    // 0x28 [O]; [R] tParticleEmitter_tParticleEmitter(cMemMan_alloc(0x150, "Board
                                        //      Spark Emitter")); [U] update steps / colours it; render draws it (graphics
                                        //      vtable slot 82)
    char unk_02C[0x40];                 // 0x2C 0x30: the flash sprites' positions (t4Vector[3]), 0x60: their sizes
                                        //      (float[3]) (update / render); no name: On Tour's tSpriteBasic
                                        //      mFlashSprites[3] (pos, size, color) is laid out differently
    int mSetupEmitters;                 // 0x6C [O] bool (a word in retail); [R] ctor 1; [U] update: when set, re-runs
                                        //      mEmitter's setupEmitter with mEmitterData
    int mRenderSparks;                  // 0x70 [O] bool (word); [U] reset 0; update sets it on a glowing surface above
                                        //      ~278 speed; render draws mSparkEmitter with mSparkDrawState
    int mRenderSparkFlash;              // 0x74 [O] bool (word); [U] reset 0; update sets it while rail sliding; render
                                        //      draws the 3 flash sprites with mSparkFlashDrawState
    int mRenderChunks;                  // 0x78 [O] bool (word); [U] reset 0; update: throw grit through mEmitter
    sDrawState mSparkDrawState;         // 0x7C [O]; [U] reset: sDrawState_s_defaultDrawState + additive bits, effect
                                        //      texture 22; render pushes it
    sDrawState mSparkFlashDrawState;    // 0x90 [O]; [U] reset (the second state, word 0 |= 0xC); render pushes it
    char pad_A4[0xC];                   // 0xA4 (the part is 16-byte aligned)
};

CHECK_OFFSET(cBoardSparkFX, mRider, 0x00);
CHECK_OFFSET(cBoardSparkFX, pad_04, 0x04);
CHECK_OFFSET(cBoardSparkFX, mFlashBasePosition, 0x10);
CHECK_OFFSET(cBoardSparkFX, mEmitter, 0x20);
CHECK_OFFSET(cBoardSparkFX, mEmitterData, 0x24);
CHECK_OFFSET(cBoardSparkFX, mSparkEmitter, 0x28);
CHECK_OFFSET(cBoardSparkFX, unk_02C, 0x2C);
CHECK_OFFSET(cBoardSparkFX, mSetupEmitters, 0x6C);
CHECK_OFFSET(cBoardSparkFX, mRenderSparks, 0x70);
CHECK_OFFSET(cBoardSparkFX, mRenderSparkFlash, 0x74);
CHECK_OFFSET(cBoardSparkFX, mRenderChunks, 0x78);
CHECK_OFFSET(cBoardSparkFX, mSparkDrawState, 0x7C);
CHECK_OFFSET(cBoardSparkFX, mSparkFlashDrawState, 0x90);
CHECK_OFFSET(cBoardSparkFX, pad_A4, 0xA4);
CHECK_SIZE(cBoardSparkFX, 0xB0);

// cBoardTrailFX (cRiderHandler + 0x520, 0xF0 bytes): FX part; ctor cBoardTrailFX_initialize (in charge: cRider at this
// + 0xF0); vtable cBoardTrailFX$6cRider D_00488C00.
// Members: On Tour class cBoardTrailFX (0x110) in the same order. Retail has the virtual-base pointer first (so the
// first four members sit 4 bytes later) and lacks On Tour's m_L2WOffset and the three index-buffer pointer pairs
// (0x24 bytes), so from CurSliceCap on every member sits 0x20 bytes before On Tour's. Placed by [U]
// cBoardTrailFX_reset (State = 2 eBTSNormal, BufLen / BufHead / CurAlpha / CurDepth / CurHeight / LightSide = 0,
// LastCen = t4vect_Origin, LastVel / LastSide = t4vect_Zero, Left/RightRandWidth = 1.0, DelaySampling = 0),
// cBoardTrailFX_initialize (CurSliceCap = 0, SliceCaps[] = -1, BufVerts[0..5] = new [324] "BoardTrailVertices"),
// cBoardTrailFX_update and cBoardTrailFX_shouldSample (the views sBoardTrailFXView, sBTrail_8938, sBTrail_86F0).
struct cBoardTrailFX {
    cRider* mRider;                 // 0x00 [R] virtual-base pointer to the shared cRider; [R] cBoardTrailFX_initialize
    float LeftRandWidth;            // 0x04 [O] (0x00); [U] reset 1.0; update: 1 + random spread, scales the inner edge
    float RightRandWidth;           // 0x08 [O] (0x04); [U] reset 1.0; update: the same for the outer edge
    int BufId;                      // 0x0C [O] (0x08); [U] update flips it (1 - BufId)
    tBxVertex* BufVerts[6];         // 0x10 [O] (0x0C; On Tour tColourUVVertex*, the PSP vertex); [R] initialize:
                                    //      new tBxVertex[324] each ("BoardTrailVertices", 0x30 bytes with the
                                    //      empty construction loop); [U] reset rewrites u / v / one of 54 per strip
    int CurSliceCap;                // 0x28 [O] (0x48); [R] initialize 0
    short SliceCaps[48];            // 0x2C [O] (0x4C); [R] initialize fills -1 (from the last)
    int BufHead;                    // 0x8C [O] (0xAC); [U] reset 0; update: the ring-buffer head
    int BufLen;                     // 0x90 [O] (0xB0); [U] reset 0; update counts up to the segment count
    int State;                      // 0x94 [O] eBoardTrailState (0xB4): 0 eBTSInAir, 1 eBTSLand, 2 eBTSNormal;
                                    //      [U] reset 2; update / shouldSample test 0 and 1
    float CurAlpha;                 // 0x98 [O] (0xB8); [U] reset 0
    float CurDepth;                 // 0x9C [O] (0xBC); [U] reset 0; update: CurHeight = a - CurDepth * b
    float CurHeight;                // 0xA0 [O] (0xC0); [U] reset 0; update scales the rider's up vector by it
    float LightSide;                // 0xA4 [O] (0xC4); [U] reset 0
    char unk_0A8[0x8];              // 0xA8 padding to the 16-byte aligned LastCen (On Tour 0xC8 likewise)
    t4Vector LastCen;               // 0xB0 [O] (0xD0); [U] reset t4vect_Origin; shouldSample: distance from it
    t4Vector LastVel;               // 0xC0 [O] (0xE0); [U] reset t4vect_Zero; update: the last direction
    t4Vector LastSide;              // 0xD0 [O] (0xF0); [U] reset t4vect_Zero; update: the last side vector
    int DelaySampling;              // 0xE0 [O] (0x100); [U] reset 0; shouldSample counts it down, sets 2 on landing
    char unk_0E4[0xC];              // 0xE4 tail padding to 0xF0 (On Tour 0x104 likewise)
};

CHECK_OFFSET(cBoardTrailFX, mRider, 0x00);
CHECK_OFFSET(cBoardTrailFX, LeftRandWidth, 0x04);
CHECK_OFFSET(cBoardTrailFX, RightRandWidth, 0x08);
CHECK_OFFSET(cBoardTrailFX, BufId, 0x0C);
CHECK_OFFSET(cBoardTrailFX, BufVerts, 0x10);
CHECK_OFFSET(cBoardTrailFX, CurSliceCap, 0x28);
CHECK_OFFSET(cBoardTrailFX, SliceCaps, 0x2C);
CHECK_OFFSET(cBoardTrailFX, BufHead, 0x8C);
CHECK_OFFSET(cBoardTrailFX, BufLen, 0x90);
CHECK_OFFSET(cBoardTrailFX, State, 0x94);
CHECK_OFFSET(cBoardTrailFX, CurAlpha, 0x98);
CHECK_OFFSET(cBoardTrailFX, CurDepth, 0x9C);
CHECK_OFFSET(cBoardTrailFX, CurHeight, 0xA0);
CHECK_OFFSET(cBoardTrailFX, LightSide, 0xA4);
CHECK_OFFSET(cBoardTrailFX, unk_0A8, 0xA8);
CHECK_OFFSET(cBoardTrailFX, LastCen, 0xB0);
CHECK_OFFSET(cBoardTrailFX, LastVel, 0xC0);
CHECK_OFFSET(cBoardTrailFX, LastSide, 0xD0);
CHECK_OFFSET(cBoardTrailFX, DelaySampling, 0xE0);
CHECK_OFFSET(cBoardTrailFX, unk_0E4, 0xE4);
CHECK_SIZE(cBoardTrailFX, 0xF0);

// cSpeedBoostFX (cRiderHandler + 0x610, 0x3B0 bytes): FX part; ctor cSpeedBoostFX_cSpeedBoostFX (in charge: cRider at
// this + 0x3B0); vtable cSpeedBoostFX$6cRider D_00488598.
// On Tour's tSpeedBoostTrailSeg (0x30 there: t4Vector ends); retail 0x1C: [U] cSpeedBoostFX_UpdateSpeedBoostTrail
// stores the two ends as 3-float values (sV3E6C08) at +0x4 / +0x10, the ctor's 30-entry loop strides 0x1C.
struct tSpeedBoostTrailSeg {
    float mLength;   // 0x00 [O]; [U] UpdateSpeedBoostTrail: the distance from the previous sample (summed into
                     //      mSpeedBoostTrailCurrentLength)
    t3Vector mLeft;  // 0x04 [O] (On Tour t4Vector at 0x10); [U] UpdateSpeedBoostTrail: the first ground-projected corner
    t3Vector mRight; // 0x10 [O] (On Tour t4Vector at 0x20); [U] the second corner
};

CHECK_OFFSET(tSpeedBoostTrailSeg, mLength, 0x00);
CHECK_OFFSET(tSpeedBoostTrailSeg, mLeft, 0x04);
CHECK_OFFSET(tSpeedBoostTrailSeg, mRight, 0x10);
CHECK_SIZE(tSpeedBoostTrailSeg, 0x1C);

// On Tour's tSpeedBoostStreamerSeg (0x20, same in retail): one streamer ribbon step.
struct tSpeedBoostStreamerSeg {
    t4Vector l; // 0x00 [O]; [U] cSpeedBoostFX_update (sSeg_66B8.a)
    t4Vector r; // 0x10 [O]; [U] cSpeedBoostFX_update (sSeg_66B8.b)
};

CHECK_OFFSET(tSpeedBoostStreamerSeg, l, 0x00);
CHECK_OFFSET(tSpeedBoostStreamerSeg, r, 0x10);
CHECK_SIZE(tSpeedBoostStreamerSeg, 0x20);

// Members: [O] On Tour's cSpeedBoostFX (same order; On Tour has no vbptr, so the members up to
// mSpeedBoostTrailCurrentLength sit 4 bytes higher in retail; tSpeedBoostTrailSeg is 0x1C here). [U] the unit's views
// (cSpeedBoostFX_reset, cSpeedBoostFX_update sWake_66B8, UpdateSpeedBoostTrail sTrailE6C08, render sBoardWake_7A10).
struct cSpeedBoostFX {
    cRider* mRider;                          // 0x000 [R] virtual-base pointer to the shared cRider; [R] cSpeedBoostFX_cSpeedBoostFX
    float mTrailLength;                      // 0x004 [O] (On Tour 0x0); [U] update: (boost * 400 + 400) * speed / 3333,
                                             //       else * 0.95; UpdateSpeedBoostTrail trims the trail to it
    float mAlpha;                            // 0x008 [O]; [U] reset 0; update 1.0 while boosting, else * 0.95; render
    float mWidth;                            // 0x00C [O]; [U] reset 0; update boost * 9 + 6 (50 at uber level 11), else
                                             //       * 0.95; render
    float mVOffset;                          // 0x010 [O]; [U] reset 0; update scrolls it with the speed (wraps 0..1);
                                             //       render: the texture v
    int mSpeedBoostTrailNumSegs;             // 0x014 [O]; [U] reset 0; UpdateSpeedBoostTrail counts up to 30; update
                                             //       clears it on landing
    float mSpeedBoostTrailCurrentLength;     // 0x018 [O]; [U] reset 0; UpdateSpeedBoostTrail: the summed mLength
    char pad_01C[0x4];                       // 0x01C (t4Vector alignment)
    t4Vector mSpeedBoostTrailLastPos;        // 0x020 [O]; [U] UpdateSpeedBoostTrail: the previous sample's position
    int mSpeedBoostTrailCurrentSeg;          // 0x030 [O]; [U] reset 0; UpdateSpeedBoostTrail: the ring head (mod 30)
    char pad_034[0xC];                       // 0x034 (t4Vector alignment)
    t4Vector mLastSide;                      // 0x040 [O]; [U] reset (1, 0, 0, 1); UpdateSpeedBoostTrail keeps the side
                                             //       axis facing it and stores it back
    tSpeedBoostTrailSeg mSpeedBoostTrail[30]; // 0x050 [O]; [R] ctor's 30-entry placement loop; [U] UpdateSpeedBoostTrail
    int mCurrentTexture;                     // 0x398 [O]; [U] reset 57; update 57..61 by the uber level / boost
    int mStreamBufHead;                      // 0x39C [O]; [U] reset 0; update: the streamer ring head (mod 20)
    int mStreamBufLen;                       // 0x3A0 [O]; [U] reset 0; update counts up to mMaxStreamerSegments while
                                             //       boosting, clears it on landing
    int mMaxStreamerSegments;                // 0x3A4 [O]; [U] reset 20 for a rider with a view, else 18
    int mWasInAir;                           // 0x3A8 [O] bool (a word in retail); [U] reset 0; update stores
                                             //       (motion state == MS_Air)
    tSpeedBoostStreamerSeg* mStreamBuf;      // 0x3AC [O]; [R] ctor operator new[](20 x 0x20, "cSpeedBoostStreamerFX");
                                             //       dtor frees it; [U] update, render
};

CHECK_OFFSET(cSpeedBoostFX, mRider, 0x000);
CHECK_OFFSET(cSpeedBoostFX, mTrailLength, 0x004);
CHECK_OFFSET(cSpeedBoostFX, mAlpha, 0x008);
CHECK_OFFSET(cSpeedBoostFX, mWidth, 0x00C);
CHECK_OFFSET(cSpeedBoostFX, mVOffset, 0x010);
CHECK_OFFSET(cSpeedBoostFX, mSpeedBoostTrailNumSegs, 0x014);
CHECK_OFFSET(cSpeedBoostFX, mSpeedBoostTrailCurrentLength, 0x018);
CHECK_OFFSET(cSpeedBoostFX, pad_01C, 0x01C);
CHECK_OFFSET(cSpeedBoostFX, mSpeedBoostTrailLastPos, 0x020);
CHECK_OFFSET(cSpeedBoostFX, mSpeedBoostTrailCurrentSeg, 0x030);
CHECK_OFFSET(cSpeedBoostFX, pad_034, 0x034);
CHECK_OFFSET(cSpeedBoostFX, mLastSide, 0x040);
CHECK_OFFSET(cSpeedBoostFX, mSpeedBoostTrail, 0x050);
CHECK_OFFSET(cSpeedBoostFX, mCurrentTexture, 0x398);
CHECK_OFFSET(cSpeedBoostFX, mStreamBufHead, 0x39C);
CHECK_OFFSET(cSpeedBoostFX, mStreamBufLen, 0x3A0);
CHECK_OFFSET(cSpeedBoostFX, mMaxStreamerSegments, 0x3A4);
CHECK_OFFSET(cSpeedBoostFX, mWasInAir, 0x3A8);
CHECK_OFFSET(cSpeedBoostFX, mStreamBuf, 0x3AC);
CHECK_SIZE(cSpeedBoostFX, 0x3B0);

// cTrickBoostFX (cRiderHandler + 0x9C0, 0x110 bytes): FX part; ctor cTrickBoostFX_cTrickBoostFX (in charge: cRider at this + 0x110);
// vtable cTrickBoostFX$6cRider D_004884A0.
// Members: [O] On Tour's cTrickBoostFX (same order; no vbptr there, so retail's members sit 4 bytes higher, and 0x10
// higher from the 16-aligned mPositionBuffer on). [U] cTrickBoostFX_reset__FPv and the views of cTrickBoostFX_update /
// cTrickBoostFX_render (sTrail_2EADD0).
struct cTrickBoostFX {
    cRider* mRider;                 // 0x000 [R] virtual-base pointer to the shared cRider; [R] cTrickBoostFX_cTrickBoostFX
    char unk_004[0x8];              // 0x004 On Tour's mAttachedLeftSparkleID / mAttachedRightSparkleID would sit here;
                                    //       no retail access
    int mNumActiveBlurBuffers;      // 0x00C [O] (On Tour 0x8); [U] reset 0; update: in the air min(n + 1, 3), else 0;
                                    //       render needs 2 or more
    int mCurrentBlurBuffer;         // 0x010 [O] (On Tour 0xC); [U] reset 0; update (n + 2) % 3; render walks from it
    char pad_014[0xC];              // 0x014 (t4Vector alignment)
    t4Vector mPositionBuffer[3][4]; // 0x020 [O] (On Tour declares t4Vector [4][3] at 0x10); [U] update stores the four
                                    //       board-quad corners in buffer mCurrentBlurBuffer; render draws strips through them
    t4Vector mConnectVertLeft;      // 0x0E0 [O] (On Tour 0xD0); [U] update: board position - side * 12; render
    t4Vector mConnectVertRight;     // 0x0F0 [O] (On Tour 0xE0); [U] update: board position + side * 12; render
    char unk_100[0x10];             // 0x100 On Tour's mVOffset (0xF0) by order; no retail access seen
};

CHECK_OFFSET(cTrickBoostFX, mRider, 0x000);
CHECK_OFFSET(cTrickBoostFX, unk_004, 0x004);
CHECK_OFFSET(cTrickBoostFX, mNumActiveBlurBuffers, 0x00C);
CHECK_OFFSET(cTrickBoostFX, mCurrentBlurBuffer, 0x010);
CHECK_OFFSET(cTrickBoostFX, pad_014, 0x014);
CHECK_OFFSET(cTrickBoostFX, mPositionBuffer, 0x020);
CHECK_OFFSET(cTrickBoostFX, mConnectVertLeft, 0x0E0);
CHECK_OFFSET(cTrickBoostFX, mConnectVertRight, 0x0F0);
CHECK_OFFSET(cTrickBoostFX, unk_100, 0x100);
CHECK_SIZE(cTrickBoostFX, 0x110);

// cAirStreamerFX (cRiderHandler + 0xAD0, 0x20 bytes): FX part; ctor cAirStreamerFX_cAirStreamerFX (in charge: cRider
// at this + 0x20); vtable cAirStreamerFX$6cRider D_00488918.
// On Tour's tAirStreamerSeg (0x20, same in retail): one air-streamer ribbon step.
struct tAirStreamerSeg {
    t4Vector l; // 0x00 [O]; [U] cAirStreamerFX_update: board point + board axis * 90 (sRsSeg_EF6D0.p0)
    t4Vector r; // 0x10 [O]; [U] cAirStreamerFX_update: board point - board axis * 90 (p1)

    tAirStreamerSeg() {} // [O] tAirStreamerSeg::tAirStreamerSeg(); [R] cAirStreamerFX's ctor keeps the empty
                         //     construction loop of its new tAirStreamerSeg[25] (view cRsmItem_2EF3B0)
};

CHECK_OFFSET(tAirStreamerSeg, l, 0x00);
CHECK_OFFSET(tAirStreamerSeg, r, 0x10);
CHECK_SIZE(tAirStreamerSeg, 0x20);

// Members: [O] On Tour's cAirStreamerFX (0x14 there, no vbptr: every member sits 4 bytes higher in retail). [U]
// cAirStreamerFX_reset and the views of cAirStreamerFX_update / cAirStreamerFX_render (sTrail_EF6D0 / sTrail_EF950).
struct cAirStreamerFX {
    cRider* mRider;            // 0x00 [R] virtual-base pointer to the shared cRider; [R] cAirStreamerFX_cAirStreamerFX
    tAirStreamerSeg* StreamBuf; // 0x04 [O] (On Tour 0x0); [R] ctor operator new[](25 x 0x20, "cAirStreamerFX"); dtor
                               //      frees it; [U] update writes StreamBuf[StreamBufHead]
    int StreamBufHead;         // 0x08 [O]; [U] reset 0; update (head + 1) % 25 on even AI ticks
    int StreamBufLen;          // 0x0C [O]; [U] reset 0; update counts up to mMaxStreamerSegments in the air, down by 2
                               //      otherwise
    float mVOffset;            // 0x10 [O]; [U] reset 0 (sw $0); update scrolls it by speed / 6000 (wraps 0..1)
    int mMaxStreamerSegments;  // 0x14 [O]; [U] reset 25 for a rider with a view, else 18
    char pad_18[0x8];          // 0x18 (the part is 16-byte aligned)
};

CHECK_OFFSET(cAirStreamerFX, mRider, 0x00);
CHECK_OFFSET(cAirStreamerFX, StreamBuf, 0x04);
CHECK_OFFSET(cAirStreamerFX, StreamBufHead, 0x08);
CHECK_OFFSET(cAirStreamerFX, StreamBufLen, 0x0C);
CHECK_OFFSET(cAirStreamerFX, mVOffset, 0x10);
CHECK_OFFSET(cAirStreamerFX, mMaxStreamerSegments, 0x14);
CHECK_OFFSET(cAirStreamerFX, pad_18, 0x18);
CHECK_SIZE(cAirStreamerFX, 0x20);

// cAngerFX (cRiderHandler + 0xAF0, 0x10 bytes): FX part; ctor cAngerFX_cAngerFX (in charge: cRider at this + 0x10);
// vtable cAngerFX$6cRider D_00488B08.
struct cAngerFX {
    cRider* mRider;    // 0x00 [R] virtual-base pointer to the shared cRider; [R] cAngerFX_cAngerFX
    char unk_004[0xC]; // 0x04 not laid out yet (see the unit's local view)
};

CHECK_OFFSET(cAngerFX, mRider, 0x00);
CHECK_OFFSET(cAngerFX, unk_004, 0x04);
CHECK_SIZE(cAngerFX, 0x10);

// cPlayerIndicatorFX (cRiderHandler + 0xB00, 0x40 bytes): FX part; ctor cPlayerIndicatorFX_cPlayerIndicatorFX (in charge: cRider at this +
// 0x40); vtable cPlayerIndicatorFX$6cRider D_00488A10.
// Members from On Tour's DWARF (class cPlayerIndicatorFX : virtual cRider: PulseAng, MinColor, MaxColor, CurColor);
// retail's tARGB is only 4-aligned, so the colours sit at 0x08 / 0x18 / 0x28 (On Tour 0x10 / 0x20 / 0x30).
struct cPlayerIndicatorFX {
    cRider* mRider;     // 0x00 [R] virtual-base pointer to the shared cRider; [R] cPlayerIndicatorFX_cPlayerIndicatorFX
    float PulseAng;     // 0x04 [O]; [U] cPlayerIndicatorFX_reset zeroes it; cPlayerIndicatorFX_update advances it and
                        //      blends CurColor between MinColor and MaxColor by it
    tARGB MinColor;     // 0x08 [O]; [U] reset: (0.5, 1, 0, 0)
    tARGB MaxColor;     // 0x18 [O]; [U] reset: (0.7, 1, 0.7, 0.7)
    tARGB CurColor;     // 0x28 [O]; [U] reset copies MinColor; update stores the blend
    char unk_038[0x8];  // 0x38 no retail access seen (the cRider virtual base starts at 0x40)
};

CHECK_OFFSET(cPlayerIndicatorFX, mRider, 0x00);
CHECK_OFFSET(cPlayerIndicatorFX, PulseAng, 0x04);
CHECK_OFFSET(cPlayerIndicatorFX, MinColor, 0x08);
CHECK_OFFSET(cPlayerIndicatorFX, MaxColor, 0x18);
CHECK_OFFSET(cPlayerIndicatorFX, CurColor, 0x28);
CHECK_OFFSET(cPlayerIndicatorFX, unk_038, 0x38);
CHECK_SIZE(cPlayerIndicatorFX, 0x40);

// cDynamicSprayFX (cRiderHandler + 0xB40, 0x130 bytes): FX part; ctor cDynamicSprayFX_cDynamicSprayFX (in charge: cRider at this +
// 0x130); vtable cDynamicSprayFX$6cRider D_004883A8.
// Members: partly laid out (names ours); the gaps are still covered by the unit's local views (src/visualfx/
// boardwakefx.cpp: sSprayFxE4A8, sWakeFxF920, ...).
struct cDynamicSprayFX {
    cRider* mRider;                     // 0x000 [R] virtual-base pointer to the shared cRider; [R]
                                        //       cDynamicSprayFX_cDynamicSprayFX
    float mShedAmount;                  // 0x004 (name ours) [U] func_002DF3B0 clears it; func_002E1598 raises it on an
                                        //       impact (at most 2); func_002E2260 sheds snow from the body bones while
                                        //       it is positive (alpha = it * 1.5) and lowers it
    char unk_008[0x8];                  // 0x008
    float mPowderLevel;                 // 0x010 (name ours) [U] func_002DF3B0 clears it; func_002E1F70 moves it towards
                                        //       the surface's SnowBuildupAmount and fades the powder trail with it
    char unk_014[0x8];                  // 0x014
    float mHeadPuffGlow;                // 0x01C (name ours) [U] func_002DF3B0 clears it; func_002E1120 steps it towards
                                        //       the anim's target and derives the head puffs' period and alpha from it
    char unk_020[0x8];                  // 0x020
    cDynamicColourEmitter* mEmitters;   // 0x028 (name ours) [R] ctor new cDynamicColourEmitter[10]
                                        //       ("DynamicSprayEmitter"), dtor delete[]; [U] func_002E2550 sets their
                                        //       m_enabled, func_002E26B8 re-runs emitters 5 and 6's setupEmitter
    char unk_02C[0x14];                 // 0x02C 0x2C: the 10 spray settings (0xE8 bytes each: a tParticleData and 4
                                        //       velocity scales; ctor "DynamicSprayData"; local view sSprayParams);
                                        //       0x30: row 1 of the board bone's matrix (func_002DF920)
    t4Vector mBoardSide;                // 0x040 (name ours) [U] func_002DF920: row 2 of the board bone's
                                        //       (mBoneBoardChild) SkelMatrices (cBoardTrailFX_update steps across
                                        //       the board along it); func_002DE398 scatters the emission point +-10
                                        //       along it
    t4Vector mBoardForward;             // 0x050 (name ours) [U] func_002DF920: row 0 of that matrix (the board's
                                        //       length); func_002DE398 scatters the emission point along it by the lean
                                        //       side
    char unk_060[0x14];                 // 0x060
    float mLastSpeed;                   // 0x074 (name ours) [U] func_002DF920: last frame's speed (the slow-down test);
                                        //       func_002DF3B0 clears it
    int mShedBone;                      // 0x078 (name ours) [U] func_002E2260 walks mBoneIndicies with it (wraps at
                                        //       30); func_002DF3B0 clears it
    int* mBoneIndicies;                 // 0x07C (name ours, the allocation tag's spelling) [R] ctor: a 0x78-byte
                                        //       "BoneIndicies" block (30 ints), dtor frees it; [U] func_002DF4D0 fills
                                        //       it with cAnimModel_getBoneSkelIndex results
    int mBoneIndiciesValid;             // 0x080 (name ours) [U] func_002DF4D0 sets 1 after filling mBoneIndicies;
                                        //       func_002DF3B0 clears it; func_002DF920 refills the table while 0
    char unk_084[0xC];                  // 0x084
    t4Vector mColour;                   // 0x090 (name ours) [U] func_002DF3B0: white; func_002DF920: the rider's render
                                        //       state colour doubled, alpha 1 (the emitters' colour)
    char unk_0A0[0x40];                 // 0x0A0
    float mBurstStrength;               // 0x0E0 (name ours) [U] func_002E23E0 keeps the strongest burst's strength;
                                        //       func_002E1598 plays it and decays it; func_002DF3B0 clears it
    char unk_0E4[0x3C];                 // 0x0E4
    float mBurstAlpha;                  // 0x120 (name ours) [U] func_002DF3B0 0.5; func_002E1598 blends the burst
                                        //       colour's alpha with it
    int mIsPlayer;                      // 0x124 (name ours) [U] func_002E26B8: = (mRider->mViewIndex >= 0);
                                        //       func_002DF3B0 re-tunes when it changes; func_002E2550 enables the
                                        //       player emitters with it
    int mLOD;                           // 0x128 (name ours) [U] func_002E2550 stores mRider->mLOD; func_002DF920
                                        //       re-chooses the emitters when it differs
    float mLeanSide;                    // 0x12C (name ours) [U] func_002DF920: +-1 from PressMode, the anim and
                                        //       IsRidingFakie, else 0; func_002DF3B0 clears it; func_002DE398 biases
                                        //       the emission point with it
};

CHECK_OFFSET(cDynamicSprayFX, mRider, 0x000);
CHECK_OFFSET(cDynamicSprayFX, mShedAmount, 0x004);
CHECK_OFFSET(cDynamicSprayFX, mPowderLevel, 0x010);
CHECK_OFFSET(cDynamicSprayFX, mHeadPuffGlow, 0x01C);
CHECK_OFFSET(cDynamicSprayFX, mEmitters, 0x028);
CHECK_OFFSET(cDynamicSprayFX, unk_02C, 0x02C);
CHECK_OFFSET(cDynamicSprayFX, mBoardSide, 0x040);
CHECK_OFFSET(cDynamicSprayFX, mBoardForward, 0x050);
CHECK_OFFSET(cDynamicSprayFX, mLastSpeed, 0x074);
CHECK_OFFSET(cDynamicSprayFX, mShedBone, 0x078);
CHECK_OFFSET(cDynamicSprayFX, mBoneIndicies, 0x07C);
CHECK_OFFSET(cDynamicSprayFX, mBoneIndiciesValid, 0x080);
CHECK_OFFSET(cDynamicSprayFX, mColour, 0x090);
CHECK_OFFSET(cDynamicSprayFX, mBurstStrength, 0x0E0);
CHECK_OFFSET(cDynamicSprayFX, mBurstAlpha, 0x120);
CHECK_OFFSET(cDynamicSprayFX, mIsPlayer, 0x124);
CHECK_OFFSET(cDynamicSprayFX, mLOD, 0x128);
CHECK_OFFSET(cDynamicSprayFX, mLeanSide, 0x12C);
CHECK_SIZE(cDynamicSprayFX, 0x130);

// cPunchFX (cRiderHandler + 0xC70, 0xB0 bytes): FX part; ctor cPunchFX_cPunchFX (in charge: cRider at this + 0xB0); vtable
// _vt$8cPunchFX$6cRider D_00488820.
struct cPunchFX {
    cRider* mRider;     // 0x00 [R] virtual-base pointer to the shared cRider; [R] cPunchFX_cPunchFX
    char unk_004[0x9C]; // 0x04 not laid out yet (sprite positions / colours / sizes: sFx_1150 in seg/seg_1F1548)
    int mActive;        // 0xA0 [O] cPunchFX::mActive (bool, after mSprites[4]); [R] cPunchFX_reset clears it, as On Tour's
                        //      reset does (sb zero,0xD0); [U] cPunchFX_update sets it to rider PunchStrength > 0 (sFx_1150::active)
    char unk_0A4[0xC];  // 0xA4
};

CHECK_OFFSET(cPunchFX, mRider, 0x00);
CHECK_OFFSET(cPunchFX, unk_004, 0x04);
CHECK_OFFSET(cPunchFX, mActive, 0xA0);
CHECK_SIZE(cPunchFX, 0xB0);

// cCheatFX (cRiderHandler + 0xD20, 0x10 bytes): FX part, new in retail (no preview vtable); [O] On Tour's last
// cRiderHandler base. ctor cCheatFX_cCheatFX (in charge: cRider at this + 0x10), vtables D_004886E0/D_00488728.
struct cCheatFX {
    cRider* mRider;           // 0x00 [R] virtual-base pointer to the shared cRider; [R] cCheatFX_cCheatFX
    tCheatState m_cheatState; // 0x04 [O]; [R] ctor -> resetStates (cCheatFX_resetStates: memset(this + 4, 0, 4)); [U]
                              //      update/updateState byte fields
    char pad_08[0x8];         // 0x08 On Tour's m_input (0x8) is not in retail: setCheatInput/getInput use
                              //      m_cheatState.m_misc
};

CHECK_OFFSET(cCheatFX, mRider, 0x00);
CHECK_OFFSET(cCheatFX, m_cheatState, 0x04);
CHECK_OFFSET(cCheatFX, pad_08, 0x08);
CHECK_SIZE(cCheatFX, 0x10);

struct cRiderHandler {
    cGroundMotion GroundMotion;             // 0x000
    cAirMotion AirMotion;                   // 0x020
    cWipeoutMotion WipeoutMotion;           // 0x030
    cStaticMotion StaticMotion;             // 0x0A0
    cRailslideMotion RailslideMotion;       // 0x0B0
    cHandplantMotion HandplantMotion;       // 0x110
    cCruiseControl CruiseControl;           // 0x1C0
    cBoardPressControl BoardPressControl;   // 0x1D0
    cPrewindControl PrewindControl;         // 0x1F0
    cBumpControl BumpControl;               // 0x200
    cNaturalAirControl NaturalAirControl;   // 0x210
    cSpinControl SpinControl;               // 0x230
    cGateControl GateControl;               // 0x290
    cRailslideControl RailslideControl;     // 0x2B0
    cWipeoutControl WipeoutControl;         // 0x2C0
    cResetControl ResetControl;             // 0x350
    cFinishLineControl FinishLineControl;   // 0x360
    cHandplantControl HandplantControl;     // 0x370
    cUberGrindControl UberGrindControl;     // 0x390
    cVoidControl VoidControl;               // 0x3A0
    cBoardWakeFX BoardWakeFX;               // 0x3B0
    cBoardSparkFX BoardSparkFX;             // 0x470
    cBoardTrailFX BoardTrailFX;             // 0x520
    cSpeedBoostFX SpeedBoostFX;             // 0x610
    cTrickBoostFX TrickBoostFX;             // 0x9C0
    cAirStreamerFX AirStreamerFX;           // 0xAD0
    cAngerFX AngerFX;                       // 0xAF0
    cPlayerIndicatorFX PlayerIndicatorFX;   // 0xB00
    cDynamicSprayFX DynamicSprayFX;         // 0xB40
    cPunchFX PunchFX;                       // 0xC70
    cCheatFX CheatFX;                       // 0xD20
    cPlayerLights PlayerLights;             // 0xD30 [O] class cPlayerLights (render/playerlights.h); [U] ai/rider.cpp
                                            //       clears it (cPlayerLights_Clear, handler + 0xD30); cTrickBoostFX update
                                            //       (cTrickBoostFX_update) calls AddDirectional / BoardIsAdditive on it
    int MotionState;                        // 0xDE0 [P] _getMotionState (preview 0xDA0); [R] cRiderHandler__getMotionState;
                                            //       ctor = MS_Num; [O] eMotionState
    int ControlState;                       // 0xDE4 [P] _getControlState (preview 0xDA4); [R] cRiderHandler__getControlState;
                                            //       ctor = CS_Num; [O] eControlState
    void** mVtbl;                           // 0xDE8 [R] _vt$13cRiderHandler cRiderHandler_vtbl (cPlayer and cComputer
                                            //       store their own here: D_00458338, D_004585F0)
    char pad_DEC[0x4];                      // 0xDEC cRiderHandler is 16-byte aligned
};

// Virtual slots of _vt$13cRiderHandler (cRiderHandler_vtbl; [O] cRiderHandler's virtual order).
enum {
    kRiderHandlerVt_getControlCmd = 1,      // pure (__pure_virtual); cPlayer cPlayer_getControlCmd, cComputer cComputer_getControlCmd
    kRiderHandlerVt_writeToReplayFrame = 2, // cRiderHandler_writeToReplayFrame (calls every part's writeToReplayFrame)
    kRiderHandlerVt_readFromReplayFrame = 3 // cRiderHandler_readFromReplayFrame (calls every part's readFromReplayFrame / FX reset)
};

CHECK_OFFSET(cRiderHandler, GroundMotion, 0x000);
CHECK_OFFSET(cRiderHandler, AirMotion, 0x020);
CHECK_OFFSET(cRiderHandler, WipeoutMotion, 0x030);
CHECK_OFFSET(cRiderHandler, StaticMotion, 0x0A0);
CHECK_OFFSET(cRiderHandler, RailslideMotion, 0x0B0);
CHECK_OFFSET(cRiderHandler, HandplantMotion, 0x110);
CHECK_OFFSET(cRiderHandler, CruiseControl, 0x1C0);
CHECK_OFFSET(cRiderHandler, BoardPressControl, 0x1D0);
CHECK_OFFSET(cRiderHandler, PrewindControl, 0x1F0);
CHECK_OFFSET(cRiderHandler, BumpControl, 0x200);
CHECK_OFFSET(cRiderHandler, NaturalAirControl, 0x210);
CHECK_OFFSET(cRiderHandler, SpinControl, 0x230);
CHECK_OFFSET(cRiderHandler, GateControl, 0x290);
CHECK_OFFSET(cRiderHandler, RailslideControl, 0x2B0);
CHECK_OFFSET(cRiderHandler, WipeoutControl, 0x2C0);
CHECK_OFFSET(cRiderHandler, ResetControl, 0x350);
CHECK_OFFSET(cRiderHandler, FinishLineControl, 0x360);
CHECK_OFFSET(cRiderHandler, HandplantControl, 0x370);
CHECK_OFFSET(cRiderHandler, UberGrindControl, 0x390);
CHECK_OFFSET(cRiderHandler, VoidControl, 0x3A0);
CHECK_OFFSET(cRiderHandler, BoardWakeFX, 0x3B0);
CHECK_OFFSET(cRiderHandler, BoardSparkFX, 0x470);
CHECK_OFFSET(cRiderHandler, BoardTrailFX, 0x520);
CHECK_OFFSET(cRiderHandler, SpeedBoostFX, 0x610);
CHECK_OFFSET(cRiderHandler, TrickBoostFX, 0x9C0);
CHECK_OFFSET(cRiderHandler, AirStreamerFX, 0xAD0);
CHECK_OFFSET(cRiderHandler, AngerFX, 0xAF0);
CHECK_OFFSET(cRiderHandler, PlayerIndicatorFX, 0xB00);
CHECK_OFFSET(cRiderHandler, DynamicSprayFX, 0xB40);
CHECK_OFFSET(cRiderHandler, PunchFX, 0xC70);
CHECK_OFFSET(cRiderHandler, CheatFX, 0xD20);
CHECK_OFFSET(cRiderHandler, PlayerLights, 0xD30);
CHECK_OFFSET(cRiderHandler, MotionState, 0xDE0);
CHECK_OFFSET(cRiderHandler, ControlState, 0xDE4);
CHECK_OFFSET(cRiderHandler, mVtbl, 0xDE8);
CHECK_OFFSET(cRiderHandler, pad_DEC, 0xDEC);
CHECK_SIZE(cRiderHandler, 0xDF0);

#endif /* AI_RIDERHANDLER_H */
