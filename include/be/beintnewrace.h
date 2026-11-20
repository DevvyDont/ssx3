#ifndef BE_BEINTNEWRACE_H
#define BE_BEINTNEWRACE_H

// cBERaceDB: the settings of the race being set up / run - the rider slot maps, how many riders, humans, mission
// riders and AI riders, the location, race time and the game mode / path / event bytes. It is not allocated: the one
// instance is the static cBENewRaceInterface::newRace ([O] On Tour: `static cBERaceDB newRace`), retail D_00535BC8,
// which the back end's new-race interface (cBENewRaceInterface_*, be/beintnewrace) writes and ~45 units read (the AI,
// game flow, front end, sound, network). The labels D_00535BF8 ... D_00535C12 are its fields (below).
// EA's header is ../source/be/beintnewrace.h (preview assert path); the class is declared with cBENewRaceInterface.
//
// Size 0x4C: [R] cBENewRaceInterface_submit / cBENewRaceInterface_discard save and restore it whole into cBELibrary_raceDB (a 0x4C-byte struct copy,
// 0x00-0x4B). The six-rider cap is playerID[6] / riderID[6] (notes/subsystems/riders-cap.md).
// No vtable, no base class. A FLAT layout, like main/game.h.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cBERaceDB, 0x48: 4-entry maps, iOpponentID, cMetaGame,
// cHeatNumber, cSeries, cSeriesLevel, iEventID, none of which retail has), placed at retail offsets by:
//   [P] the July 2003 preview's cBENewRaceInterface inline accessors on newRace (preview 0x005B6540, same offsets
//       as retail): getNumberRiders lw 0x30, getNumberHumans 0x34, getNumberMission 0x38, getAIRiders_Num 0x3C,
//       setLocationID sw 0x40, get/setRaceTime 0x44, getGameMode lb 0x48, getGamePath lb 0x49, getGameEvent lb 0x4A,
//       get/setDifficultyLevel lb/sb 0x4B. Retail keeps some out of line (be/bedbreward: cBENewRaceInterface_getNumberRiders = 0x30,
//       cBENewRaceInterface_getNumberMission = 0x38, cBENewRaceInterface_setRaceTime/cBENewRaceInterface_getRaceTime = set/get 0x44);
//   [R] retail's setters: cBENewRaceInterface_assignPlayer(gate, player) writes playerID[gate] = player and
//       riderID[player] = gate; setNumberHumans/setNumberMission/setNumberAI recompute iNumberRiders;
//       setGameMode sb 0x48, cBENewRaceInterface_setGamePath (setGamePath) sb 0x49, setGameEvent sb 0x4A;
//   [O] On Tour's member order.
// The preview's newRace continues past 0x4C (getGameTournament lw 0x50, getCurrentTournamentRound lw 0x54); retail's
// struct copy stops at 0x4C, so those are separate statics in retail.

#include "layout.h"

struct cBERaceDB {
    int playerID[6];            // 0x00 [O] (On Tour [4]); [R] assignPlayer: playerID[gate] = player;
                                //      setNumberHumans resets it to 0..5
    int riderID[6];             // 0x18 [O]; [R] assignPlayer: riderID[player] = gate; setNumberHumans resets 0..5
    int iNumberRiders;          // 0x30 [P] getNumberRiders; [R] = humans + mission + AI (D_00535BF8)
    int iNumberHumans;          // 0x34 [P] getNumberHumans; [R] setNumberHumans (D_00535BFC); [U] cAI_InitPlayers
    int iNumberMissionRiders;   // 0x38 [P] getNumberMission; [R] setNumberMission (D_00535C00)
    int iNumberAI;              // 0x3C [P] getAIRiders_Num; [R] setNumberAI (D_00535C04); [U] cAI_initComputerRiders
    int iLocationIdx;           // 0x40 [P] setLocationID; [U] cBENewRaceInterface_getLocation(ID) (D_00535C08)
    int iRaceTime;              // 0x44 [P] get/setRaceTime; [R] cBENewRaceInterface_setRaceTime / cBENewRaceInterface_getRaceTime (D_00535C0C)
    signed char cGameMode;      // 0x48 [P] getGameMode; [R] setGameMode stores its argument (D_00535C10)
    signed char cGamePath;      // 0x49 [P] getGamePath; [R] cBENewRaceInterface_setGamePath (setGamePath): 0, 1, 2 = online
                                //      (D_00535C11); [U] cGameModeMan::iCurrentGamePath copies it
    signed char cGameEvent;     // 0x4A [P] getGameEvent; [R] setGameEvent (D_00535C12); [U] initGameMode's argument
    signed char cDifficultyLevel; // 0x4B [P] get/setDifficultyLevel (lb/sb 0x4B; no On Tour counterpart)
};

CHECK_OFFSET(cBERaceDB, playerID, 0x00);
CHECK_OFFSET(cBERaceDB, riderID, 0x18);
CHECK_OFFSET(cBERaceDB, iNumberRiders, 0x30);
CHECK_OFFSET(cBERaceDB, iNumberHumans, 0x34);
CHECK_OFFSET(cBERaceDB, iNumberMissionRiders, 0x38);
CHECK_OFFSET(cBERaceDB, iNumberAI, 0x3C);
CHECK_OFFSET(cBERaceDB, iLocationIdx, 0x40);
CHECK_OFFSET(cBERaceDB, iRaceTime, 0x44);
CHECK_OFFSET(cBERaceDB, cGameMode, 0x48);
CHECK_OFFSET(cBERaceDB, cGamePath, 0x49);
CHECK_OFFSET(cBERaceDB, cGameEvent, 0x4A);
CHECK_OFFSET(cBERaceDB, cDifficultyLevel, 0x4B);
CHECK_SIZE(cBERaceDB, 0x4C);

// The race settings (On Tour: cBENewRaceInterface::newRace): who races, as which rider, where and in which mode.
extern cBERaceDB cBENewRaceInterface_newRace;

#endif /* BE_BEINTNEWRACE_H */
