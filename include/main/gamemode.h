#ifndef MAIN_GAMEMODE_H
#define MAIN_GAMEMODE_H

// cGameModeMan: the game-mode manager - which rules the current event runs under (race, big air, freeride,
// multiplayer race, ...), the heat number and the AI riders' characters, time limit, point target and the event's
// done/last-heat/locked flags. One instance, created by the static cGameModeMan::getGM (cGameModeMan_getGM:
// `cMemMan_alloc(0xA0, "GameModeMan", 0, 0)`, D_004A2C6C = instance) and reached as SSXApp->GameModeMan (0xC0) and,
// by the game-mode handlers, through the static cGameModeHandler::pManager (D_004A2C70, set by getGM).
// EA's header is ../source/main/gamemode.h (preview assert strings "iRider < kCHAR_NUM && iRider >= 0",
// "pGameModeManager").
//
// Size 0xA0: [R] the allocation in getGM. No vtable, no base class.
// Statics (not in the object; [O] names, retail sizes): aHandler[10] = D_00536668 (the cGameModeHandler subclass
// instances getGM creates; iCurrModeHandler indexes it; purgeGM deletes 10), bGiveUp[6] = cGameModeMan_bGiveUp (restartHeat
// clears 6 words), and the 10-entry score / rank tables D_00536640, D_00536708, D_00536730 that sortScores /
// sortTimes (func_00238B70 / func_00238BF8) fill and writeToReplayFrame saves.
//
// A FLAT layout, like main/game.h. Member names are EA's, from SSX On Tour's PSP DWARF (class cGameModeMan, 0x22C:
// 62-entry character tables, byte bools), placed at retail offsets by:
//   [R] retail code whose On Tour counterpart stores the same values in the same order: getGM's inlined
//       constructor (iHeatNum = 0, iCurrModeHandler = 0, iCurrentGameEvent = -1), cGameModeHandler::purge
//       (func_00238C80: iCurrModeHandler/iLocation/iNextLocation/iCurrentGameEvent = -1, bIsEventDone = 1, the rest 0,
//       memset of both character tables), cGMFreerideHandler::init (func_0023B170: iHeatNum = 1, bIsLastHeat = 1,
//       iCharacters[] = -1, iNextHeat = iRestoreHeat = 1, bAreResultsLocked = 1, iNextLocation = -1),
//       restorePreviousHeat (func_00238348: iNextHeat = iRestoreHeat), initGameMode (iCurrentGameEvent = argument,
//       iCurrentGamePath = cBENewRaceInterface::newRace.cGamePath, then iCurrModeHandler by event);
//   [U] how retail code uses the field;
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [R].
// Retail differs from On Tour: iHeatNum is first (On Tour: after the bools), iLocation/iNextLocation come right after
// the character tables (On Tour: last), there is no iCurrentMetaGame, the tables have 10 entries, the bools are words,
// and there are three extra words (unk_80, unk_90, unk_94).
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"

struct cGameModeMan {
    int iHeatNum;               // 0x00 [R] getGM = 0, purge = 0, freeride init = 1, big air init = iNextHeat;
                                //      [U] the current heat (1..3); [O] On Tour getCurrentHeat
    int iCurrModeHandler;       // 0x04 [R] indexes aHandler (initGameMode, restartHeat, riderCheckpointHit ...);
                                //      purge = -1; [O]
    int iCurrentGameEvent;      // 0x08 [R] initGameMode(iBEGameEvent) stores its argument; getGM/purge = -1; [O]
    int iCurrentGamePath;       // 0x0C [R] initGameMode stores newRace.cGamePath (D_00535C11; 2 = online); [O]
    int iNumberAI;              // 0x10 [O]; [U] preview assert `iRider < (pManager->iNumberAI+1)`; big air = 5
    int iSimAI;                 // 0x14 [O]; [U] big air = 4 or 5, race = 0; gamestate sets newRace's AI count to
                                //      iNumberAI - iSimAI (the AI riders that really ride; On Tour getNumberAIToSim)
    int iCharacters[10];        // 0x18 [O] (On Tour [62]); [R] purge clears 0x28 bytes, freeride fills -1;
                                //      [U] the AI riders' character IDs (cBENewPlayerInterface_setRiderCharID)
    int iCheatCharacters[10];   // 0x40 [O]; [R] purge clears 0x28 bytes; [U] gamestate passes them to func_001473D0
    int iLocation;              // 0x68 [R] purge = -1 (On Tour purge: iLocation = -1); [U] handlers read/write it
    int iNextLocation;          // 0x6C [R] purge = -1, freeride init = -1 (On Tour: iNextLocation = -1)
    int iNextHeat;              // 0x70 [R] restorePreviousHeat: iNextHeat = iRestoreHeat; freeride = 1
    int iRestoreHeat;           // 0x74 [R] restorePreviousHeat reads it; freeride = 1; not saved to replays
    unsigned int uiTimeLimit;   // 0x78 [R] big air init = time limit of the heat * 60; [U] HUD, cRider_updateFinishTest
    unsigned int uiPointTarget; // 0x7C [R] purge / freeride / big air = 0; [U] HUD onRender2D
    int unk_80;                 // 0x80 [R] getGM = 0, purge = 0, freeride/big air = 0 (no On Tour counterpart)
    int bIsEventDone;           // 0x84 [R] purge = 1 (as On Tour), freeride = 0
    int bIsTimedEvent;          // 0x88 [R] purge = 0; big air = 1 with a time limit, race = 0 with uiTimeLimit = 0;
                                //      [U] read with uiTimeLimit by cRider_updateFinishTest, and by the trophy screens
    int bHasPointTarget;        // 0x8C [R] cleared with uiPointTarget (big air init); [O] after bIsTimedEvent
    int unk_90;                 // 0x90 [R] purge = 0; cGMRaceHandler init (func_0023A108) = 1 in heat 1, big air = 0
    int unk_94;                 // 0x94 [R] purge = 0; big air: 1 unless iCurrentGameEvent == 2
    int bIsLastHeat;            // 0x98 [R] freeride init = 1 (On Tour: bIsLastHeat = 1), heat 3 = 1; [U] script engine
    int bAreResultsLocked;      // 0x9C [R] freeride init = 1 (On Tour: bAreResultsLocked = 1); [U] raceFinish reads it
};

CHECK_OFFSET(cGameModeMan, iHeatNum, 0x00);
CHECK_OFFSET(cGameModeMan, iCurrModeHandler, 0x04);
CHECK_OFFSET(cGameModeMan, iCurrentGameEvent, 0x08);
CHECK_OFFSET(cGameModeMan, iCurrentGamePath, 0x0C);
CHECK_OFFSET(cGameModeMan, iNumberAI, 0x10);
CHECK_OFFSET(cGameModeMan, iSimAI, 0x14);
CHECK_OFFSET(cGameModeMan, iCharacters, 0x18);
CHECK_OFFSET(cGameModeMan, iCheatCharacters, 0x40);
CHECK_OFFSET(cGameModeMan, iLocation, 0x68);
CHECK_OFFSET(cGameModeMan, iNextLocation, 0x6C);
CHECK_OFFSET(cGameModeMan, iNextHeat, 0x70);
CHECK_OFFSET(cGameModeMan, iRestoreHeat, 0x74);
CHECK_OFFSET(cGameModeMan, uiTimeLimit, 0x78);
CHECK_OFFSET(cGameModeMan, uiPointTarget, 0x7C);
CHECK_OFFSET(cGameModeMan, unk_80, 0x80);
CHECK_OFFSET(cGameModeMan, bIsEventDone, 0x84);
CHECK_OFFSET(cGameModeMan, bIsTimedEvent, 0x88);
CHECK_OFFSET(cGameModeMan, bHasPointTarget, 0x8C);
CHECK_OFFSET(cGameModeMan, unk_90, 0x90);
CHECK_OFFSET(cGameModeMan, unk_94, 0x94);
CHECK_OFFSET(cGameModeMan, bIsLastHeat, 0x98);
CHECK_OFFSET(cGameModeMan, bAreResultsLocked, 0x9C);
CHECK_SIZE(cGameModeMan, 0xA0);

// The one manager (On Tour: cGameModeMan::instance, created by getGM) and the pointer the game-mode handlers
// read it through (cGameModeHandler::pManager, set alongside).
extern cGameModeMan* cGameModeMan_instance;
extern cGameModeMan* cGameModeHandler_pManager;

#endif /* MAIN_GAMEMODE_H */
