#ifndef MAIN_GAMEMODEHANDLER_H
#define MAIN_GAMEMODEHANDLER_H

// The game-mode handlers: the subclasses of cGameModeHandler that cGameModeMan::getGM (cGameModeMan_getGM) creates into
// the static table aHandler[10] (D_00536668, "GM Handler" allocations) and cGameModeMan dispatches to by
// iCurrModeHandler (main/gamemode.h). Each one runs the rules of one kind of event through the base's 11 virtuals:
// 1 dtor, 2 init, 3 purge, 4 cleanRiderRanksPoints, 5 cleanRiderRanksTime, 6 riderCheckpointHit, 7 riderGateHit,
// 8 raceFinish, 9 getHeatScore, 10 writeToReplayFrame, 11 readFromReplayFrame (base names: cGameModeHandler_*).
// EA's source: d:/ssx/code/source/main/gamemodehandler.cpp (the July 2003 preview's assert strings); retail keeps the
// subclasses' methods in main/gamemode (0x00238E20..0x0023C618) and their destructors in main/ssxappdtor.
//
// The base cGameModeHandler has no data in retail (vptr only, no header: see the group E report); its vptr is at 0x0
// and each subclass appends from 0x4. On Tour's cGameModeHandler carries 0x2F4 bytes (sHeatResults) that retail does
// not have, so On Tour's subclass members at 0x2F4.. sit at 0x4.. here.
//
// Classes: the preview's 11 game-mode vtables (_vt$16cGameModeHandler, cGMBigAirHandler, cGMRaceHandler,
// cGMMPRaceHandler, cGMMPSlopeHandler, cGMFreerideHandler, cGMTimeHandler, cGMRivalTimeHandler, cGMRivalPointHandler,
// cGMPointHandler, cGMDebugHandler, in that order) and retail's 11 (0x0047D008 down to 0x0047CBF8, 0x68 apart) line up
// one for one: the preview pairing tool (builds/x/v2) maps the init / dtor slots of 0x0047CFA0 (cGMBigAirHandler),
// 0x0047CF38 (cGMRaceHandler), 0x0047CED0 (cGMMPRaceHandler), 0x0047CE00 (cGMFreerideHandler) and 0x0047CBF8
// (cGMDebugHandler) to those classes. The remaining ones are placed by that order and confirmed by On Tour's DWARF
// (members and overridden methods, below). The vptr-only subclasses (cGMFreerideHandler, cGMRivalTimeHandler, cGMRivalPointHandler,
// cGMDebugHandler: 4 bytes) get no struct here.
//
// Sizes: [R] cGameModeMan_getGM's allocations (cMemMan_alloc(size, "GM Handler")): aHandler[0] cGMBigAirHandler 0x7C,
// [1] cGMRaceHandler 0xD0, [2] cGMFreerideHandler 4, [4] cGMTimeHandler 0xC, [5] cGMRivalTimeHandler 4,
// [6] cGMRivalPointHandler 4, [7] cGMPointHandler 0xC, [8] cGMMPRaceHandler 0x2C, [9] cGMMPSlopeHandler 8 ([3]
// cGMDebugHandler is not created in retail).
//
// FLAT layouts, like main/game.h. Evidence tags: [R] getGM's allocations, the subclass destructors, the init methods;
// [U] how retail code uses the field (src/main/gamemode.cpp); [O] SSX On Tour's DWARF (classes cGMTimeHandler,
// cGMPointHandler, cGMMPSlopeHandler, cGMMPRaceHandler, cGMRaceHandler: names, types and order).

#include "layout.h"

// cGMBigAirHandler: the scored event (big air; heats scored by points). Vtable 0x0047CFA0 (D_0047CFA0): init
// func_00238E20, riderCheckpointHit func_002398E8, raceFinish func_00239230, getHeatScore func_00239CE0,
// writeToReplayFrame func_00239D18, readFromReplayFrame func_00239D50. No On Tour class (On Tour's
// cGMFreestyleHandler has a uiScore[62][3] in the same role).
struct cGMBigAirHandler {
    void** mVtbl;                   // 0x00 [R] D_0047CFA0 (getGM), the dtor func_00244560 stores the base's
    unsigned int uiScore[3][10];    // 0x04 [R] init clears 0x78 bytes; [U] getHeatScore returns uiScore[heat][rider]
                                    //      (row stride 0x28), raceFinish / func_00239AA0 fill it. Quirk: the replay
                                    //      slots 10 / 11 write and read 0xA0 bytes from 0x04, 0x28 past the object
};

CHECK_OFFSET(cGMBigAirHandler, mVtbl, 0x00);
CHECK_OFFSET(cGMBigAirHandler, uiScore, 0x04);
CHECK_SIZE(cGMBigAirHandler, 0x7C);

// cGMRaceHandler: the three-heat race event (offline event 0). Vtable 0x0047CF38 (D_0047CF38): init func_0023A108,
// riderGateHit func_0023AC10, raceFinish cGMRaceHandler_raceFinish, writeToReplayFrame func_0023AC18 /
// readFromReplayFrame func_0023AC50 (the whole 0xD0 bytes); non-virtual helpers func_0023A4F0 (build the heat
// line-ups), func_0023A668 (apply a line-up).
struct cGMRaceHandler {
    void** mVtbl;                   // 0x00 [R] D_0047CF38; the dtor func_00244590 stores the base's
    int unk_04;                     // 0x04 no access seen (inside the replay copy only)
    int mHeat2Ready;                // 0x08 [U] init: heat 2's set-up runs only while it is 0, then sets it; heat 1
                                    //      clears it; raceFinish sets it (and mHeat3Ready) when the heat is repeated
    int mHeat3Ready;                // 0x0C [U] init: heat 3's set-up runs only while it is 0; heats 1 and 2 clear it;
                                    //      raceFinish sets it on a repeat
    int mLineup[3][6];              // 0x10 [U] per heat, six character ids (the player's first): func_0023A4F0 fills
                                    //      heat 1 and the random slots of heats 2-3, init fills the rest, func_0023A668
                                    //      copies row iHeatNum - 1 into cGameModeMan's iCharacters
    unsigned int uiTime[3][10];     // 0x58 [O] uiTime (On Tour [62][3], rider-major); [U] retail is heat-major:
                                    //      init fills rows with 9s, raceFinish stores tbl[heat][rider] (360000 = DNF)
};

CHECK_OFFSET(cGMRaceHandler, mVtbl, 0x00);
CHECK_OFFSET(cGMRaceHandler, unk_04, 0x04);
CHECK_OFFSET(cGMRaceHandler, mHeat2Ready, 0x08);
CHECK_OFFSET(cGMRaceHandler, mHeat3Ready, 0x0C);
CHECK_OFFSET(cGMRaceHandler, mLineup, 0x10);
CHECK_OFFSET(cGMRaceHandler, uiTime, 0x58);
CHECK_SIZE(cGMRaceHandler, 0xD0);

// cGMMPRaceHandler: the multiplayer race. Vtable 0x0047CED0 (D_0047CED0): init func_0023AC88, raceFinish
// func_0023AE00, writeToReplayFrame func_0023B038 / readFromReplayFrame func_0023B070 (the whole 0x2C bytes);
// dtor func_002445C0.
struct cGMMPRaceHandler {
    void** mVtbl;                   // 0x00 [R] D_0047CED0
    unsigned int uiTime[10];        // 0x04 [O] uiTime (On Tour [62]); [U] init fills 9s (40 bytes), raceFinish stores
                                    //      each rider's finish tick (360000 = DNF) and copies it to D_00536640
};

CHECK_OFFSET(cGMMPRaceHandler, mVtbl, 0x00);
CHECK_OFFSET(cGMMPRaceHandler, uiTime, 0x04);
CHECK_SIZE(cGMMPRaceHandler, 0x2C);

// cGMMPSlopeHandler: the two-player online event (online events 1-3; On Tour: the multiplayer slope event). Vtable
// 0x0047CE68 (D_0047CE68): init func_00239D88, riderCheckpointHit func_0023A070, raceFinish func_00239EC8,
// writeToReplayFrame func_00244620 / readFromReplayFrame func_00244628 (empty); dtor func_002445F0. On Tour's class
// overrides exactly these five.
struct cGMMPSlopeHandler {
    void** mVtbl;                   // 0x00 [R] D_0047CE68
    short LastCheckpointHit[2];     // 0x04 [O] short LastCheckpointHit[2]; [R] init clears both (sh 0x4 / 0x6);
                                    //      [U] riderCheckpointHit counts per player and compares with the other
};

CHECK_OFFSET(cGMMPSlopeHandler, mVtbl, 0x00);
CHECK_OFFSET(cGMMPSlopeHandler, LastCheckpointHit, 0x04);
CHECK_SIZE(cGMMPSlopeHandler, 0x8);

// cGMTimeHandler: the timed peak challenge (events 6-8). Vtable 0x0047CD98 (D_0047CD98): init func_0023B268,
// riderCheckpointHit func_0023B5F8, raceFinish func_0023B468; dtor func_00244660. On Tour's class overrides exactly
// these three (the preview's does not override riderCheckpointHit yet).
struct cGMTimeHandler {
    void** mVtbl;                   // 0x00 [R] D_0047CD98
    unsigned int uiTimeToBeat;      // 0x04 [O]; [R] init: cBENewRaceInterface_getPeakGoal(..) * 60, also stored in
                                    //      cGameModeMan's uiTimeLimit / uiPointTarget; [U] raceFinish compares the time
    unsigned short uiTrackProgress; // 0x08 [O]; [R] init 0; [U] riderCheckpointHit passes it to
                                    //      cBENewRaceInterface_getPeakSplitTimePoints and increments it
    short mCurrentDifficulty;       // 0x0A [O]; [R] init: 0 / 1 / 2 from the best medal won (the goal tier);
                                    //      [U] riderCheckpointHit
};

CHECK_OFFSET(cGMTimeHandler, mVtbl, 0x00);
CHECK_OFFSET(cGMTimeHandler, uiTimeToBeat, 0x04);
CHECK_OFFSET(cGMTimeHandler, uiTrackProgress, 0x08);
CHECK_OFFSET(cGMTimeHandler, mCurrentDifficulty, 0x0A);
CHECK_SIZE(cGMTimeHandler, 0xC);

// cGMPointHandler: the points peak challenge (events 9-11). Vtable 0x0047CC60 (D_0047CC60): init func_0023C0D0,
// riderCheckpointHit func_0023C560, raceFinish func_0023C2D8; dtor func_002446F0. On Tour's class overrides exactly
// these three.
struct cGMPointHandler {
    void** mVtbl;                   // 0x00 [R] D_0047CC60
    int uiScoreToBeat;              // 0x04 [O] (int in On Tour); [R] init: cBENewRaceInterface_getPeakGoal(..) * 100,
                                    //      also cGameModeMan's uiPointTarget
    unsigned short uiTrackProgress; // 0x08 [O]; [R] init 0; [U] riderCheckpointHit passes it on and increments it
    short mCurrentDifficulty;       // 0x0A [O]; [R] init: the goal tier from the best medal; [U] riderCheckpointHit
};

CHECK_OFFSET(cGMPointHandler, mVtbl, 0x00);
CHECK_OFFSET(cGMPointHandler, uiScoreToBeat, 0x04);
CHECK_OFFSET(cGMPointHandler, uiTrackProgress, 0x08);
CHECK_OFFSET(cGMPointHandler, mCurrentDifficulty, 0x0A);
CHECK_SIZE(cGMPointHandler, 0xC);

#endif /* MAIN_GAMEMODEHANDLER_H */
