#ifndef MAIN_GAMEMODEHANDLER_H
#define MAIN_GAMEMODEHANDLER_H
#pragma interface

// The game-mode handlers: the subclasses of cGameModeHandler that cGameModeMan::getGM (cGameModeMan_getGM) creates into
// the static table aHandler[10] (cGameModeMan_aHandler, "GM Handler" allocations) and cGameModeMan dispatches to by
// iCurrModeHandler (main/gamemode.h). Each one runs the rules of one kind of event through the base's 11 virtuals:
// 1 dtor, 2 init, 3 purge, 4 cleanRiderRanksPoints, 5 cleanRiderRanksTime, 6 riderCheckpointHit, 7 riderGateHit,
// 8 raceFinish, 9 getHeatScore, 10 writeToReplayFrame, 11 readFromReplayFrame (base names: cGameModeHandler_*).
// EA's source: d:/ssx/code/source/main/gamemodehandler.cpp (the July 2003 preview's assert strings); retail keeps the
// subclasses' methods in main/gamemode (0x00238E20..0x0023C618) and their destructors in main/ssxappdtor.
//
// The base cGameModeHandler has no data in retail (vptr only: struct cGameModeHandler below); its vptr is at 0x0
// and each subclass appends from 0x4. On Tour's cGameModeHandler carries 0x2F4 bytes (sHeatResults) that retail does
// not have, so On Tour's subclass members at 0x2F4.. sit at 0x4.. here.
//
// Classes: the preview's 11 game-mode vtables (_vt$16cGameModeHandler, cGMBigAirHandler, cGMRaceHandler,
// cGMMPRaceHandler, cGMMPSlopeHandler, cGMFreerideHandler, cGMTimeHandler, cGMRivalTimeHandler, cGMRivalPointHandler,
// cGMPointHandler, cGMDebugHandler, in that order) and retail's 11 (0x0047D008 down to 0x0047CBF8, 0x68 apart) line up
// one for one: the preview pairing tool (builds/x/v2) maps the init / dtor slots of 0x0047CFA0 (cGMBigAirHandler),
// 0x0047CF38 (cGMRaceHandler), 0x0047CED0 (cGMMPRaceHandler), 0x0047CE00 (cGMFreerideHandler) and 0x0047CBF8
// (cGMDebugHandler) to those classes. The remaining ones are placed by that order and confirmed by On Tour's DWARF
// (members and overridden methods, below). The vptr-only subclasses cGMFreerideHandler, cGMRivalTimeHandler and
// cGMRivalPointHandler (4 bytes) are declared for their destructors only; cGMDebugHandler (4 bytes, its vtable inside
// the cMCOverlayManager_vtbl label) gets no struct here.
//
// Sizes: [R] cGameModeMan_getGM's allocations (cMemMan_alloc(size, "GM Handler")): aHandler[0] cGMBigAirHandler 0x7C,
// [1] cGMRaceHandler 0xD0, [2] cGMFreerideHandler 4, [4] cGMTimeHandler 0xC, [5] cGMRivalTimeHandler 4,
// [6] cGMRivalPointHandler 4, [7] cGMPointHandler 0xC, [8] cGMMPRaceHandler 0x2C, [9] cGMMPSlopeHandler 8 ([3]
// cGMDebugHandler is not created in retail).
//
// The subclasses derive from cGameModeHandler (a real class with the 11 virtuals); their own members are FLAT.
// Evidence tags: [R] getGM's allocations, the subclass destructors, the init methods;
// [U] how retail code uses the field (src/main/gamemode.cpp); [O] SSX On Tour's DWARF (classes cGMTimeHandler,
// cGMPointHandler, cGMMPSlopeHandler, cGMMPRaceHandler, cGMRaceHandler: names, types and order).

#include "layout.h"
#include "bx/memman.h"

struct cReplayFramePtr;

// cGameModeHandler: the base, vptr only in retail (On Tour's 0x2F4 sHeatResults lastHeatResults is not there: every
// subclass appends from 0x4). Used through a base pointer only: cGameModeMan_aHandler[i] (main/gamemode.h), which
// cGameModeMan dispatches through the 11 slots above and cGameModeMan_purgeGM deletes through slot 1.
class cGameModeHandler {
public:
    // 0x00: the vptr. [R] _vt$16cGameModeHandler (0x0047D008), stored back by every subclass dtor; [P] the preview's
    // _vt$16cGameModeHandler. Slots [R] that table (11, names from its retail entries cGameModeHandler_*); types [O]
    // (class cGameModeHandler) where On Tour has the method (purge and the two cleanRiderRanks* are retail's only).
    // inline: every handler's destructor inlines it; retail's out-of-line copy cGameModeHandler__cGameModeHandler
    // stays a C function
    virtual ~cGameModeHandler() {}                                        // 1
    virtual void init() __asm__("cGameModeHandler_init__FPv");                                                  // 2
    virtual void purge() __asm__("cGameModeHandler_purge");                                                 // 3
    virtual void cleanRiderRanksPoints();                                 // 4
    virtual void cleanRiderRanksTime();                                   // 5
    virtual bool riderCheckpointHit(int iRider, int iSecsAdded)
        __asm__("cGameModeHandler_riderCheckpointHit__FPv");          // 6
    virtual void riderGateHit(int iRider, int iGateID)
        __asm__("cGameModeHandler_riderGateHit__FPv");                   // 7
    virtual void raceFinish(int iRider)
        __asm__("cGameModeHandler_raceFinish__FPv");                                  // 8
    virtual unsigned int getHeatScore(int iRider, int iHeat)
        __asm__("cGameModeHandler_getHeatScore__FPv");             // 9
    virtual void writeToReplayFrame(cReplayFramePtr* data)
        __asm__("cGameModeHandler_writeToReplayFrame__FPv");               // 10
    virtual void readFromReplayFrame(cReplayFramePtr* data)
        __asm__("cGameModeHandler_readFromReplayFrame__FPv");              // 11
};

CHECK_SIZE(cGameModeHandler, 0x4);
__asm__("_vt$16cGameModeHandler = cGameModeHandler_vtbl");   // g++'s name for the retail vtable

// cGMBigAirHandler: the scored event (big air; heats scored by points). Vtable 0x0047CFA0 (cGMBigAirHandler_vtbl): init
// cGMBigAirHandler_init, riderCheckpointHit cGMBigAirHandler_riderCheckpointHit, raceFinish cGMBigAirHandler_raceFinish, getHeatScore cGMBigAirHandler_getHeatScore,
// writeToReplayFrame cGMBigAirHandler_writeToReplayFrame, readFromReplayFrame cGMBigAirHandler_readFromReplayFrame. No On Tour class (On Tour's
// cGMFreestyleHandler has a uiScore[62][3] in the same role).
struct cGMBigAirHandler : public cGameModeHandler {
    // 0x00: the vptr: [R] cGMBigAirHandler_vtbl (getGM), the dtor cGMBigAirHandler__cGMBigAirHandler stores the base's
    unsigned int uiScore[3][10];    // 0x04 [R] init clears 0x78 bytes; [U] getHeatScore returns uiScore[heat][rider]
                                    //      (row stride 0x28), raceFinish / cGMBigAirHandler_fillScoreTable fill it. Quirk: the replay
                                    //      slots 10 / 11 write and read 0xA0 bytes from 0x04, 0x28 past the object

    // Methods bound to their retail symbols by asm labels.
    virtual ~cGMBigAirHandler();   // g++'s own symbol _$_16cGMBigAirHandler (the retail label renamed)
    virtual void init() __asm__("cGMBigAirHandler_init");
    virtual bool riderCheckpointHit(int player, int secs) __asm__("cGMBigAirHandler_riderCheckpointHit");
    virtual unsigned int getHeatScore(int col, int row) __asm__("cGMBigAirHandler_getHeatScore");
    virtual void raceFinish(int idx) __asm__("cGMBigAirHandler_raceFinish");
    void fillScoreTable() __asm__("cGMBigAirHandler_fillScoreTable");
    virtual void writeToReplayFrame(cReplayFramePtr* stream) __asm__("cGMBigAirHandler_writeToReplayFrame");
    virtual void readFromReplayFrame(cReplayFramePtr* stream) __asm__("cGMBigAirHandler_readFromReplayFrame");
};

CHECK_OFFSET(cGMBigAirHandler, uiScore, 0x04);
CHECK_SIZE(cGMBigAirHandler, 0x7C);
__asm__("_vt$16cGMBigAirHandler = cGMBigAirHandler_vtbl");   // g++'s name for the retail vtable

// cGMRaceHandler: the three-heat race event (offline event 0). Vtable 0x0047CF38 (cGMRaceHandler_vtbl): init cGMRaceHandler_init,
// riderGateHit cGMRaceHandler_riderGateHit, raceFinish cGMRaceHandler_raceFinish, writeToReplayFrame cGMRaceHandler_writeToReplayFrame /
// readFromReplayFrame cGMRaceHandler_readFromReplayFrame (the whole 0xD0 bytes); non-virtual helpers cGMRaceHandler_buildHeatLineups (build the heat
// line-ups), cGMRaceHandler_applyHeatLineup (apply a line-up).
struct cGMRaceHandler : public cGameModeHandler {
    // 0x00: the vptr: [R] cGMRaceHandler_vtbl; the dtor cGMRaceHandler__cGMRaceHandler stores the base's
    int unk_04;                     // 0x04 no access seen (inside the replay copy only)
    int mHeat2Ready;                // 0x08 [U] init: heat 2's set-up runs only while it is 0, then sets it; heat 1
                                    //      clears it; raceFinish sets it (and mHeat3Ready) when the heat is repeated
    int mHeat3Ready;                // 0x0C [U] init: heat 3's set-up runs only while it is 0; heats 1 and 2 clear it;
                                    //      raceFinish sets it on a repeat
    int mLineup[3][6];              // 0x10 [U] per heat, six character ids (the player's first): cGMRaceHandler_buildHeatLineups fills
                                    //      heat 1 and the random slots of heats 2-3, init fills the rest, cGMRaceHandler_applyHeatLineup
                                    //      copies row iHeatNum - 1 into cGameModeMan's iCharacters
    unsigned int uiTime[3][10];     // 0x58 [O] uiTime (On Tour [62][3], rider-major); [U] retail is heat-major:
                                    //      init fills rows with 9s, raceFinish stores tbl[heat][rider] (360000 = DNF)

    // Methods bound to their retail symbols by asm labels.
    virtual ~cGMRaceHandler();   // g++'s own symbol _$_14cGMRaceHandler (the retail label renamed)
    virtual void init() __asm__("cGMRaceHandler_init");
    virtual void raceFinish(int idx) __asm__("cGMRaceHandler_raceFinish");
    virtual void riderGateHit(int, int) __asm__("cGMRaceHandler_riderGateHit__FPv");
    virtual void writeToReplayFrame(cReplayFramePtr* stream) __asm__("cGMRaceHandler_writeToReplayFrame");
    virtual void readFromReplayFrame(cReplayFramePtr* stream) __asm__("cGMRaceHandler_readFromReplayFrame");
    void buildHeatLineups() __asm__("cGMRaceHandler_buildHeatLineups");
    void applyHeatLineup() __asm__("cGMRaceHandler_applyHeatLineup");
};

CHECK_OFFSET(cGMRaceHandler, unk_04, 0x04);
CHECK_OFFSET(cGMRaceHandler, mHeat2Ready, 0x08);
CHECK_OFFSET(cGMRaceHandler, mHeat3Ready, 0x0C);
CHECK_OFFSET(cGMRaceHandler, mLineup, 0x10);
CHECK_OFFSET(cGMRaceHandler, uiTime, 0x58);
CHECK_SIZE(cGMRaceHandler, 0xD0);
__asm__("_vt$14cGMRaceHandler = cGMRaceHandler_vtbl");   // g++'s name for the retail vtable

// cGMMPRaceHandler: the multiplayer race. Vtable 0x0047CED0 (cGMMPRaceHandler_vtbl): init cGMMPRaceHandler_init, raceFinish
// cGMMPRaceHandler_raceFinish, writeToReplayFrame cGMMPRaceHandler_writeToReplayFrame / readFromReplayFrame cGMMPRaceHandler_readFromReplayFrame (the whole 0x2C bytes);
// dtor cGMMPRaceHandler__cGMMPRaceHandler.
struct cGMMPRaceHandler : public cGameModeHandler {
    // 0x00: the vptr: [R] cGMMPRaceHandler_vtbl
    unsigned int uiTime[10];        // 0x04 [O] uiTime (On Tour [62]); [U] init fills 9s (40 bytes), raceFinish stores
                                    //      each rider's finish tick (360000 = DNF) and copies it to cGameModeMan_uiFinalScores

    // Methods bound to their retail symbols by asm labels.
    virtual ~cGMMPRaceHandler();   // g++'s own symbol _$_16cGMMPRaceHandler (the retail label renamed)
    virtual void init() __asm__("cGMMPRaceHandler_init");
    virtual void raceFinish(int idx) __asm__("cGMMPRaceHandler_raceFinish");
    virtual void writeToReplayFrame(cReplayFramePtr* stream) __asm__("cGMMPRaceHandler_writeToReplayFrame");
    virtual void readFromReplayFrame(cReplayFramePtr* stream) __asm__("cGMMPRaceHandler_readFromReplayFrame");
};

CHECK_OFFSET(cGMMPRaceHandler, uiTime, 0x04);
CHECK_SIZE(cGMMPRaceHandler, 0x2C);
__asm__("_vt$16cGMMPRaceHandler = cGMMPRaceHandler_vtbl");   // g++'s name for the retail vtable

// cGMMPSlopeHandler: the two-player online event (online events 1-3; On Tour: the multiplayer slope event). Vtable
// 0x0047CE68 (cGMMPSlopeHandler_vtbl): init cGMMPSlopeHandler_init, riderCheckpointHit cGMMPSlopeHandler_riderCheckpointHit, raceFinish cGMMPSlopeHandler_raceFinish,
// writeToReplayFrame cGMMPSlopeHandler_writeToReplayFrame / readFromReplayFrame cGMMPSlopeHandler_readFromReplayFrame (empty); dtor cGMMPSlopeHandler__cGMMPSlopeHandler. On Tour's class
// overrides exactly these five.
struct cGMMPSlopeHandler : public cGameModeHandler {
    // 0x00: the vptr: [R] cGMMPSlopeHandler_vtbl
    short LastCheckpointHit[2];     // 0x04 [O] short LastCheckpointHit[2]; [R] init clears both (sh 0x4 / 0x6);
                                    //      [U] riderCheckpointHit counts per player and compares with the other

    // Methods bound to their retail symbols by asm labels.
    virtual ~cGMMPSlopeHandler();   // g++'s own symbol _$_17cGMMPSlopeHandler (the retail label renamed)
    virtual void init() __asm__("cGMMPSlopeHandler_init");
    virtual void raceFinish(int idx) __asm__("cGMMPSlopeHandler_raceFinish");
    virtual bool riderCheckpointHit(int player, int secs) __asm__("cGMMPSlopeHandler_riderCheckpointHit");
    virtual void writeToReplayFrame(cReplayFramePtr*) __asm__("cGMMPSlopeHandler_writeToReplayFrame__FPv");
    virtual void readFromReplayFrame(cReplayFramePtr*) __asm__("cGMMPSlopeHandler_readFromReplayFrame__FPv");
};

CHECK_OFFSET(cGMMPSlopeHandler, LastCheckpointHit, 0x04);
CHECK_SIZE(cGMMPSlopeHandler, 0x8);
__asm__("_vt$17cGMMPSlopeHandler = cGMMPSlopeHandler_vtbl");   // g++'s name for the retail vtable

// cGMTimeHandler: the timed peak challenge (events 6-8). Vtable 0x0047CD98 (cGMTimeHandler_vtbl): init cGMTimeHandler_init,
// riderCheckpointHit cGMTimeHandler_riderCheckpointHit, raceFinish cGMTimeHandler_raceFinish; dtor cGMTimeHandler__cGMTimeHandler. On Tour's class overrides exactly
// these three (the preview's does not override riderCheckpointHit yet).
struct cGMTimeHandler : public cGameModeHandler {
    // 0x00: the vptr: [R] cGMTimeHandler_vtbl
    unsigned int uiTimeToBeat;      // 0x04 [O]; [R] init: cBENewRaceInterface_getPeakGoal(..) * 60, also stored in
                                    //      cGameModeMan's uiTimeLimit / uiPointTarget; [U] raceFinish compares the time
    unsigned short uiTrackProgress; // 0x08 [O]; [R] init 0; [U] riderCheckpointHit passes it to
                                    //      cBENewRaceInterface_getPeakSplitTimePoints and increments it
    short mCurrentDifficulty;       // 0x0A [O]; [R] init: 0 / 1 / 2 from the best medal won (the goal tier);
                                    //      [U] riderCheckpointHit

    // Methods bound to their retail symbols by asm labels.
    virtual ~cGMTimeHandler();   // g++'s own symbol _$_14cGMTimeHandler (the retail label renamed)
    virtual void init() __asm__("cGMTimeHandler_init");
    virtual void raceFinish(int riderIndex) __asm__("cGMTimeHandler_raceFinish");
    virtual bool riderCheckpointHit(int, int) __asm__("cGMTimeHandler_riderCheckpointHit");
};

CHECK_OFFSET(cGMTimeHandler, uiTimeToBeat, 0x04);
CHECK_OFFSET(cGMTimeHandler, uiTrackProgress, 0x08);
CHECK_OFFSET(cGMTimeHandler, mCurrentDifficulty, 0x0A);
CHECK_SIZE(cGMTimeHandler, 0xC);
__asm__("_vt$14cGMTimeHandler = cGMTimeHandler_vtbl");   // g++'s name for the retail vtable

// cGMPointHandler: the points peak challenge (events 9-11). Vtable 0x0047CC60 (cGMPointHandler_vtbl): init cGMPointHandler_init,
// riderCheckpointHit cGMPointHandler_riderCheckpointHit, raceFinish cGMPointHandler_raceFinish; dtor cGMPointHandler__cGMPointHandler. On Tour's class overrides exactly
// these three.
struct cGMPointHandler : public cGameModeHandler {
    // 0x00: the vptr: [R] cGMPointHandler_vtbl
    int uiScoreToBeat;              // 0x04 [O] (int in On Tour); [R] init: cBENewRaceInterface_getPeakGoal(..) * 100,
                                    //      also cGameModeMan's uiPointTarget
    unsigned short uiTrackProgress; // 0x08 [O]; [R] init 0; [U] riderCheckpointHit passes it on and increments it
    short mCurrentDifficulty;       // 0x0A [O]; [R] init: the goal tier from the best medal; [U] riderCheckpointHit

    // Methods bound to their retail symbols by asm labels.
    virtual ~cGMPointHandler();   // g++'s own symbol _$_15cGMPointHandler (the retail label renamed)
    virtual void init() __asm__("cGMPointHandler_init");
    virtual void raceFinish(int idx) __asm__("cGMPointHandler_raceFinish");
    virtual bool riderCheckpointHit(int, int) __asm__("cGMPointHandler_riderCheckpointHit");
};

CHECK_OFFSET(cGMPointHandler, uiScoreToBeat, 0x04);
CHECK_OFFSET(cGMPointHandler, uiTrackProgress, 0x08);
CHECK_OFFSET(cGMPointHandler, mCurrentDifficulty, 0x0A);
CHECK_SIZE(cGMPointHandler, 0xC);
__asm__("_vt$15cGMPointHandler = cGMPointHandler_vtbl");   // g++'s name for the retail vtable

// cGMFreerideHandler: the freeride handler (vptr only; [R] getGM's 4-byte allocation; vtable
// cGMFreerideHandler_vtbl).
struct cGMFreerideHandler : public cGameModeHandler {
    virtual ~cGMFreerideHandler();   // g++'s own symbol _$_18cGMFreerideHandler (the retail label renamed)

    // Methods bound to their retail symbols by asm labels.
    virtual void init() __asm__("cGMFreerideHandler_init");
};

CHECK_SIZE(cGMFreerideHandler, 0x4);
__asm__("_vt$18cGMFreerideHandler = cGMFreerideHandler_vtbl");   // g++'s name for the retail vtable

// cGMRivalTimeHandler: the head-to-head event handler (vptr only; [R] getGM's 4-byte allocation; vtable
// cGMRivalTimeHandler_vtbl).
struct cGMRivalTimeHandler : public cGameModeHandler {
    virtual ~cGMRivalTimeHandler();   // g++'s own symbol _$_19cGMRivalTimeHandler (the retail label renamed)

    // Methods bound to their retail symbols by asm labels.
    virtual void raceFinish(int idx) __asm__("cGMRivalTimeHandler_raceFinish");
    virtual void init() __asm__("cGMRivalTimeHandler_init");
};

CHECK_SIZE(cGMRivalTimeHandler, 0x4);
__asm__("_vt$19cGMRivalTimeHandler = cGMRivalTimeHandler_vtbl");   // g++'s name for the retail vtable

// cGMRivalPointHandler: the head-to-head scored event handler (vptr only; [R] getGM's 4-byte allocation; vtable
// cGMRivalPointHandler_vtbl).
struct cGMRivalPointHandler : public cGameModeHandler {
    virtual ~cGMRivalPointHandler();   // g++'s own symbol _$_20cGMRivalPointHandler (the retail label renamed)

    // Methods bound to their retail symbols by asm labels.
    virtual void raceFinish(int idx) __asm__("cGMRivalPointHandler_raceFinish");
    virtual void init() __asm__("cGMRivalPointHandler_init");
};

CHECK_SIZE(cGMRivalPointHandler, 0x4);
__asm__("_vt$20cGMRivalPointHandler = cGMRivalPointHandler_vtbl");   // g++'s name for the retail vtable

#endif /* MAIN_GAMEMODEHANDLER_H */
