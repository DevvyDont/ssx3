#ifndef AI_CONTROLCMD_H
#define AI_CONTROLCMD_H

// tControlCmd: the per-frame rider command a player's pad or a computer rider's brain produces (cPlayer_getControlCmd,
// cComputer_getControlCmd; On Tour `void getControlCmd(tControlCmd& _cmd)`) and the replay records (replay/replay.h
// WriteCmdFrame). It is a union of one 4- or 8-byte bit-field record per control state: cRiderHandler_updateControl
// hands it to the current control part's update (cCruiseControl_update, cBoardPressControl_update, ...), which reads
// its own view of the bits. EA's header file is not known (the control parts are ai/control/<x>control.cpp in the
// preview's assert paths); the type names are EA's.
//
// Size 0x8: [O] On Tour's DWARF (union tControlCmd, size 0x8, and the 14 command structs below with their bit
// positions); [P] the July 2003 preview's assert "CmdFrame[i].CruiseCmd.Count >= lastrecord" names the CruiseCmd
// member and its Count. Bit-fields are LSB first (MIPS little-endian g++ 2.95), like retail's (word << n) >> 26 reads.
// Every retail read found so far sits at On Tour's bit position, so the layouts below are On Tour's unchanged.
// Tags: [O] On Tour DWARF; [U] the retail read (and the unit view it retires); [P] preview assert.
// The analog fields are 6-bit signed values scaled by 1/31 (0.032258...) by the readers.
// No CHECK_OFFSET lines: offsetof does not apply to bit-fields; CHECK_SIZE pins each record.

#include "layout.h"

struct tCruiseCmd {
    unsigned int Count : 12;        // w0 bits 0-11 [O]; [P] "CmdFrame[i].CruiseCmd.Count >= lastrecord"
    unsigned int Reset : 1;         // w0 bit 12 [O]
    unsigned int Handplant : 1;     // w0 bit 13 [O]
    unsigned int JumpPressed : 1;   // w0 bit 14 [O]
    unsigned int JumpHeld : 1;      // w0 bit 15 [O]
    unsigned int BoostPressed : 1;  // w0 bit 16 [O]
    unsigned int BoostHeld : 1;     // w0 bit 17 [O]
    unsigned int AttackLeft : 1;    // w0 bit 18 [O]
    unsigned int AttackRight : 1;   // w0 bit 19 [O]
    int CruiseTurn : 6;             // w0 bits 20-25 [O]; [U] cCruiseControl_update (sAiBitsX_1620)
    int CruiseCrouch : 6;           // w0 bits 26-31 [O]; [U] cCruiseControl_update (sAiBitsY_1620)
    int CruiseBrake : 6;            // w1 bits 0-5 [O]; [U] cCruiseControl_update "desBrake" (sAiBitsC_1620)
    int BoardPress : 6;             // w1 bits 6-11 [O]
};

CHECK_SIZE(tCruiseCmd, 0x8);

struct tBoardPressCmd {
    unsigned int Count : 12;        // w0 bits 0-11 [O]
    unsigned int Reset : 1;         // w0 bit 12 [O]
    unsigned int BoostPressed : 1;  // w0 bit 13 [O]
    unsigned int BoostHeld : 1;     // w0 bit 14 [O]
    unsigned int JumpHeld : 1;      // w0 bit 15 [O]
    unsigned int JumpPressed : 1;   // w0 bit 16 [O]
    unsigned int OllieHeld : 1;     // w0 bit 17 [O]
    int CruiseTurn : 6;             // w0 bits 18-23 [O]; [U] cBoardPressControl_update: cRider_setupLean (sAiBitsD_12FC80)
    int RailBalance : 6;            // w0 bits 24-29 [O]; [U] cBoardPressControl_update: cRider_setupRailBalance (sAiBitsC_12FC80)
    int BoardPress : 6;             // w1 bits 0-5 [O]; [U] handleIntoState / handleExitState / checkForExit / checkForPivot /
                                    //      checkForWipeout "desPress" (sPadIn_130228, sPadIn_1304E0, sAiStick_131200,
                                    //      sAiBits_08D8, sAiBitsA_130DD0)
    int BoardPivot : 6;             // w1 bits 6-11 [O]; [U] the same functions (their second 6-bit field; sAiBitsB_130DD0)
};

CHECK_SIZE(tBoardPressCmd, 0x8);

struct tPrewindCmd {
    unsigned int Count : 12;        // w0 bits 0-11 [O]
    unsigned int Reset : 1;         // w0 bit 12 [O]; [U] cPrewindControl_update ((w >> 12) & 1 to cRider_checkForReset)
    unsigned int JumpHeld : 1;      // w0 bit 13 [O]; [U] cPrewindControl_update (w & 0x2000)
    unsigned int BoostHeld : 1;     // w0 bit 14 [O]; [U] cPrewindControl_update ((w >> 14) & 1 to cRider_setupBoostWarp)
    int PrewindSpin : 6;            // w0 bits 15-20 [O]
    int PrewindFlip : 6;            // w0 bits 21-26 [O]
    int PrewindTurn : 6;            // w1 bits 0-5 [O]; [U] cPrewindControl_update (sAiBits0_12E9B8)
    int RailBalance : 6;            // w1 bits 6-11 [O]; [U] cPrewindControl_update (sAiBits6_12E9B8)
};

CHECK_SIZE(tPrewindCmd, 0x8);

struct tBumpCmd {
    unsigned int Count : 12;        // bits 0-11 [O]
    unsigned int Reset : 1;         // bit 12 [O]
    unsigned int BoostPressed : 1;  // bit 13 [O]
    unsigned int BoostHeld : 1;     // bit 14 [O]
    int CruiseTurn : 6;             // bits 15-20 [O]; [U] cBumpControl_update "turn" (sAiBitsB_12E778)
    int RailBalance : 6;            // bits 21-26 [O]; [U] cBumpControl_update: cRider_setupRailBalance (sAiBitsA_12E778)
};

CHECK_SIZE(tBumpCmd, 0x4);

struct tNaturalAirCmd {
    unsigned int Count : 12;        // w0 bits 0-11 [O]
    unsigned int Reset : 1;         // w0 bit 12 [O]
    unsigned int Handplant : 1;     // w0 bit 13 [O]
    unsigned int AttackLeft : 1;    // w0 bit 14 [O]
    unsigned int AttackRight : 1;   // w0 bit 15 [O]
    int GrabID : 8;                 // w0 bits 16-23 [O]
    int CruiseTurn : 6;             // w0 bits 24-29 [O]; [U] cNaturalAirControl_update (sAiBitsX_12F730)
    int CruiseCrouch : 6;           // w1 bits 0-5 [O]; [U] cNaturalAirControl_update (sAiBitsY_12F730)
};

CHECK_SIZE(tNaturalAirCmd, 0x8);

struct tSpinCmd {
    unsigned int Count : 12;        // w0 bits 0-11 [O]
    unsigned int Reset : 1;         // w0 bit 12 [O]; [U] cSpinControl_update ((*input >> 12) & 1)
    unsigned int Handplant : 1;     // w0 bit 13 [O]; [U] cSpinControl_update (*input & 0x2000, then the handplant test)
    unsigned int Tweak : 1;         // w0 bit 14 [O]; [U] cSpinControl_update ((*input >> 14) & 1 to isGrabPlaying)
    unsigned int LateSpin : 1;      // w0 bit 15 [O]; [U] cSpinControl_update (*input & 0x8000)
    int GrabID : 8;                 // w0 bits 16-23 [O]; [U] cSpinControl_update (signed byte 2: the trick)
    int SpinAdjust : 6;             // w0 bits 24-29 [O]; [U] cSpinControl_update ((input[0] << 2) >> 26)
    int FlipAdjust : 6;             // w1 bits 0-5 [O]; [U] cSpinControl_update ((input[1] << 26) >> 26)
    int AirAdjRotFB : 6;            // w1 bits 6-11 [O]; [U] cSpinControl_update ((input[1] << 20) >> 26)
    int AirAdjRotLR : 6;            // w1 bits 12-17 [O]; [U] cSpinControl_update ((input[1] << 14) >> 26)
    int BoardPress : 2;             // w1 bits 18-19 [O]; [U] cSpinControl_update ((input[1] << 12) >> 30)
};

CHECK_SIZE(tSpinCmd, 0x8);

struct tGateCmd {
    unsigned int Count : 12;        // bits 0-11 [O]
    int Anticipate : 6;             // bits 12-17 [O]; [U] cGateControl_doSit / cGateControl_doGateRock (sAiBits0012C0C0,
                                    //      sAiBits0012C230)
};

CHECK_SIZE(tGateCmd, 0x4);

struct tRailslideCmd {
    unsigned int Count : 12;        // w0 bits 0-11 [O]
    unsigned int Reset : 1;         // w0 bit 12 [O]
    unsigned int JumpPressed : 1;   // w0 bit 13 [O]
    unsigned int JumpHeld : 1;      // w0 bit 14 [O]
    unsigned int BoostPressed : 1;  // w0 bit 15 [O]
    unsigned int BoostHeld : 1;     // w0 bit 16 [O]
    int UberGrindID : 8;            // w0 bits 17-24 [O]; [U] cRailslideControl_update: cRailslideControl_checkForUberGrind
                                    //      (sPadA_131D30)
    int RailBalance : 6;            // w0 bits 25-30 [O]; [U] cRailslideControl_update: cRider_setupRailBalance (sPadB_131D30)
    int RailSpin : 6;               // w1 bits 0-5 [O]; [U] cRailslideControl_update (sPadC_131D30 x),
                                    //      cRailslideControl_checkAirMotion (sAiBits00132770)
    int BoardPress : 6;             // w1 bits 6-11 [O]; [U] cRailslideControl_update (sPadC_131D30 y)
};

CHECK_SIZE(tRailslideCmd, 0x8);

struct tWipeoutCmd {
    unsigned int Count : 12;        // bits 0-11 [O]
    unsigned int Reset : 1;         // bit 12 [O]; [U] cWipeoutControl_update ((*input >> 12) & 1)
    unsigned int Recover : 1;       // bit 13 [O]; [U] cWipeoutControl_update (*input & 0x2000)
};

CHECK_SIZE(tWipeoutCmd, 0x4);

struct tResetCmd {
    unsigned int Count : 12;        // bits 0-11 [O]
};

CHECK_SIZE(tResetCmd, 0x4);

struct tFinishLineCmd {
    unsigned int Count : 12;        // bits 0-11 [O]
    int CruiseTurn : 6;             // bits 12-17 [O]
};

CHECK_SIZE(tFinishLineCmd, 0x4);

struct tHandplantCmd {
    unsigned int Count : 12;        // bits 0-11 [O]
    unsigned int Reset : 1;         // bit 12 [O]; [U] cHandplantControl_update ((flags >> 12) & 1; sPad_2A30)
    unsigned int Handplant : 1;     // bit 13 [O]; [U] cHandplantControl_update (flags & 0x2000)
    int HandplantBalance : 6;       // bits 14-19 [O]; [U] cHandplantControl_update "desBal" ((flags << 12) >> 26)
};

CHECK_SIZE(tHandplantCmd, 0x4);

struct tUberGrindCmd {
    unsigned int Count : 12;        // bits 0-11 [O]
    unsigned int Reset : 1;         // bit 12 [O]
    unsigned int BoostPressed : 1;  // bit 13 [O]
    unsigned int BoostHeld : 1;     // bit 14 [O]
    int UberGrindID : 8;            // bits 15-22 [O]
    int RailBalance : 6;            // bits 23-28 [O]; [U] cUberGrindControl_update: cRider_setupRailBalance (sAiBitsA_136508)
};

CHECK_SIZE(tUberGrindCmd, 0x4);

struct tVoidCmd {
    unsigned int Count : 12;        // bits 0-11 [O]
};

CHECK_SIZE(tVoidCmd, 0x4);

union tControlCmd {
    tCruiseCmd CruiseCmd;           // 0x0 [O]; [P] the replay assert "CmdFrame[i].CruiseCmd.Count"
    tBoardPressCmd BoardPressCmd;   // 0x0 [O]
    tPrewindCmd PrewindCmd;         // 0x0 [O]
    tBumpCmd BumpCmd;               // 0x0 [O]
    tNaturalAirCmd NaturalAirCmd;   // 0x0 [O]
    tSpinCmd SpinCmd;               // 0x0 [O]
    tGateCmd GateCmd;               // 0x0 [O]
    tRailslideCmd RailslideCmd;     // 0x0 [O]
    tWipeoutCmd WipeoutCmd;         // 0x0 [O]
    tResetCmd ResetCmd;             // 0x0 [O]
    tFinishLineCmd FinishLineCmd;   // 0x0 [O]
    tHandplantCmd HandplantCmd;     // 0x0 [O]
    tUberGrindCmd UberGrindCmd;     // 0x0 [O]
    tVoidCmd VoidCmd;               // 0x0 [O]
};

CHECK_OFFSET(tControlCmd, CruiseCmd, 0x0);
CHECK_OFFSET(tControlCmd, BoardPressCmd, 0x0);
CHECK_OFFSET(tControlCmd, PrewindCmd, 0x0);
CHECK_OFFSET(tControlCmd, BumpCmd, 0x0);
CHECK_OFFSET(tControlCmd, NaturalAirCmd, 0x0);
CHECK_OFFSET(tControlCmd, SpinCmd, 0x0);
CHECK_OFFSET(tControlCmd, GateCmd, 0x0);
CHECK_OFFSET(tControlCmd, RailslideCmd, 0x0);
CHECK_OFFSET(tControlCmd, WipeoutCmd, 0x0);
CHECK_OFFSET(tControlCmd, ResetCmd, 0x0);
CHECK_OFFSET(tControlCmd, FinishLineCmd, 0x0);
CHECK_OFFSET(tControlCmd, HandplantCmd, 0x0);
CHECK_OFFSET(tControlCmd, UberGrindCmd, 0x0);
CHECK_OFFSET(tControlCmd, VoidCmd, 0x0);
CHECK_SIZE(tControlCmd, 0x8);

#endif /* AI_CONTROLCMD_H */
