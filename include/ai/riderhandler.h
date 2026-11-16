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
// tables D_00459D70 ("Ground" .. "Handplant") and D_00459DC8 ("Cruise" .. "Void"). The class names are EA's: the
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
// cRider), so they have no vptr of their own. The FX parts are raw after the vbptr (their own units have views).
// Tags: [R] retail ctor/dtor/vtables/dispatch, [P] preview inline accessor, [U] retail use, [O] On Tour DWARF.

#include "layout.h"
#include "bx/math3d.h"
// tPose: bx/math3d.h
#include "world/worldview.h" // tBoundBox
#include "render/playerlights.h" // cPlayerLights

struct cRider;
struct cRiderSphereTree;

// On Tour struct tCheatState (4 chars). Retail reads m_misc with lbu only (it also holds the cheat input bits).
struct tCheatState {
    char m_state;           // 0x0 [O]; [U] lb/sb (cCheatFX_update, func_002F6910)
    char m_index;           // 0x1 [O]; [U] lb/lbu/sb
    char m_count;           // 0x2 [O]; [U] lb/lbu/sb
    char m_misc;            // 0x3 [O]; [U] lbu/sb (setCheatInput cCheatFX_setCheatInput, func_002F6B28, func_002F6B38)
};

CHECK_OFFSET(tCheatState, m_state, 0x0);
CHECK_OFFSET(tCheatState, m_index, 0x1);
CHECK_OFFSET(tCheatState, m_count, 0x2);
CHECK_OFFSET(tCheatState, m_misc, 0x3);
CHECK_SIZE(tCheatState, 0x4);

// eMotionState / eControlState ([O] enum names; [R] D_00459D70 / D_00459DC8 name tables; the ctor stores 6 / 14).
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
    cRiderSphereTree* BoardSphereTree; // 0x44 [P] getBoardSphereTree (retail func_001411A0 lw 0x44; preview 0x48); [O]
    char pad_48[0x8];                  // 0x48
    tBoundBox BoardIntersectBox;       // 0x50 [P] getBoardBoundBox (func_001411A8 = this + 0x50); [O]; [U] lq/sq
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
    t4Vector RailNorm;       // 0x00 [O]; [R] cRiderHandler ctor = D_004FF160; [U] read via handler+0xB0 (getRailNorm)
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
    t4Vector ExitVel;          // 0x10 [O]; [U] vector and x/y/z/w reads (func_00139548 calcExitVel)
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
struct cBoardWakeFX {
    cRider* mRider;     // 0x00 [R] virtual-base pointer to the shared cRider; [R] cBoardWakeFX_cBoardWakeFX
    char unk_004[0xBC]; // 0x04 not laid out yet (see the unit's local view)
};

CHECK_OFFSET(cBoardWakeFX, mRider, 0x00);
CHECK_OFFSET(cBoardWakeFX, unk_004, 0x04);
CHECK_SIZE(cBoardWakeFX, 0xC0);

// cBoardSparkFX (cRiderHandler + 0x470, 0xB0 bytes): FX part; ctor cBoardSparkFX_cBoardSparkFX (in charge: cRider at
// this + 0xB0); vtable cBoardSparkFX$6cRider D_00488CF8.
struct cBoardSparkFX {
    cRider* mRider;     // 0x00 [R] virtual-base pointer to the shared cRider; [R] cBoardSparkFX_cBoardSparkFX
    char unk_004[0xAC]; // 0x04 not laid out yet (see the unit's local view)
};

CHECK_OFFSET(cBoardSparkFX, mRider, 0x00);
CHECK_OFFSET(cBoardSparkFX, unk_004, 0x04);
CHECK_SIZE(cBoardSparkFX, 0xB0);

// cBoardTrailFX (cRiderHandler + 0x520, 0xF0 bytes): FX part; ctor cBoardTrailFX_initialize (in charge: cRider at this
// + 0xF0); vtable cBoardTrailFX$6cRider D_00488C00.
struct cBoardTrailFX {
    cRider* mRider;     // 0x00 [R] virtual-base pointer to the shared cRider; [R] cBoardTrailFX_initialize
    char unk_004[0xEC]; // 0x04 not laid out yet (see the unit's local view)
};

CHECK_OFFSET(cBoardTrailFX, mRider, 0x00);
CHECK_OFFSET(cBoardTrailFX, unk_004, 0x04);
CHECK_SIZE(cBoardTrailFX, 0xF0);

// cSpeedBoostFX (cRiderHandler + 0x610, 0x3B0 bytes): FX part; ctor cSpeedBoostFX_cSpeedBoostFX (in charge: cRider at
// this + 0x3B0); vtable cSpeedBoostFX$6cRider D_00488598.
struct cSpeedBoostFX {
    cRider* mRider;      // 0x00 [R] virtual-base pointer to the shared cRider; [R] cSpeedBoostFX_cSpeedBoostFX
    char unk_004[0x3AC]; // 0x04 not laid out yet (see the unit's local view)
};

CHECK_OFFSET(cSpeedBoostFX, mRider, 0x00);
CHECK_OFFSET(cSpeedBoostFX, unk_004, 0x04);
CHECK_SIZE(cSpeedBoostFX, 0x3B0);

// cTrickBoostFX (cRiderHandler + 0x9C0, 0x110 bytes): FX part; ctor cTrickBoostFX_cTrickBoostFX (in charge: cRider at this + 0x110);
// vtable cTrickBoostFX$6cRider D_004884A0.
struct cTrickBoostFX {
    cRider* mRider;      // 0x00 [R] virtual-base pointer to the shared cRider; [R] cTrickBoostFX_cTrickBoostFX
    char unk_004[0x10C]; // 0x04 not laid out yet (see the unit's local view)
};

CHECK_OFFSET(cTrickBoostFX, mRider, 0x00);
CHECK_OFFSET(cTrickBoostFX, unk_004, 0x04);
CHECK_SIZE(cTrickBoostFX, 0x110);

// cAirStreamerFX (cRiderHandler + 0xAD0, 0x20 bytes): FX part; ctor cAirStreamerFX_cAirStreamerFX (in charge: cRider
// at this + 0x20); vtable cAirStreamerFX$6cRider D_00488918.
struct cAirStreamerFX {
    cRider* mRider;     // 0x00 [R] virtual-base pointer to the shared cRider; [R] cAirStreamerFX_cAirStreamerFX
    char unk_004[0x1C]; // 0x04 not laid out yet (see the unit's local view)
};

CHECK_OFFSET(cAirStreamerFX, mRider, 0x00);
CHECK_OFFSET(cAirStreamerFX, unk_004, 0x04);
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
struct cPlayerIndicatorFX {
    cRider* mRider;     // 0x00 [R] virtual-base pointer to the shared cRider; [R] cPlayerIndicatorFX_cPlayerIndicatorFX
    char unk_004[0x3C]; // 0x04 not laid out yet (see the unit's local view)
};

CHECK_OFFSET(cPlayerIndicatorFX, mRider, 0x00);
CHECK_OFFSET(cPlayerIndicatorFX, unk_004, 0x04);
CHECK_SIZE(cPlayerIndicatorFX, 0x40);

// cDynamicSprayFX (cRiderHandler + 0xB40, 0x130 bytes): FX part; ctor cDynamicSprayFX_cDynamicSprayFX (in charge: cRider at this +
// 0x130); vtable cDynamicSprayFX$6cRider D_004883A8.
struct cDynamicSprayFX {
    cRider* mRider;      // 0x00 [R] virtual-base pointer to the shared cRider; [R] cDynamicSprayFX_cDynamicSprayFX
    char unk_004[0x12C]; // 0x04 not laid out yet (see the unit's local view)
};

CHECK_OFFSET(cDynamicSprayFX, mRider, 0x00);
CHECK_OFFSET(cDynamicSprayFX, unk_004, 0x04);
CHECK_SIZE(cDynamicSprayFX, 0x130);

// cPunchFX (cRiderHandler + 0xC70, 0xB0 bytes): FX part; ctor cPunchFX_cPunchFX (in charge: cRider at this + 0xB0); vtable
// _vt$8cPunchFX$6cRider D_00488820.
struct cPunchFX {
    cRider* mRider;     // 0x00 [R] virtual-base pointer to the shared cRider; [R] cPunchFX_cPunchFX
    char unk_004[0xAC]; // 0x04 not laid out yet
};

CHECK_OFFSET(cPunchFX, mRider, 0x00);
CHECK_OFFSET(cPunchFX, unk_004, 0x04);
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
    kRiderHandlerVt_getControlCmd = 1,      // pure (func_00411530); cPlayer func_00127998, cComputer cComputer_getControlCmd
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
