#ifndef BE_BEINTNEWRACE_H
#define BE_BEINTNEWRACE_H

// cBERaceDB: the settings of the race being set up / run - the rider slot maps, how many riders, humans, mission
// riders and AI riders, the location, race time and the game mode / path / event bytes. It is not allocated: the one
// instance is the static cBENewRaceInterface::newRace ([O] On Tour: `static cBERaceDB newRace`), retail D_00535BC8,
// which the back end's new-race interface (cBENewRaceInterface_*, be/beintnewrace) writes and ~45 units read (the AI,
// game flow, front end, sound, network). The symbol carries size:0x4C in config/symbol_addrs.txt, so splat writes
// accesses to its fields as cBENewRaceInterface_newRace + offset (its old field labels D_00535BF8 ... D_00535C12 are
// noted per member below).
// EA's header is ../source/be/beintnewrace.h (preview assert path); the class is declared with cBENewRaceInterface.
//
// Size 0x4C: [R] cBENewRaceInterface_submit / cBENewRaceInterface_discard save and restore it whole into
// cBELibrary_raceDB (a 0x4C-byte struct copy,
// 0x00-0x4B). The six-rider cap is playerID[6] / riderID[6] (notes/subsystems/riders-cap.md).
// No vtable, no base class. A FLAT layout, like main/game.h.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cBERaceDB, 0x48: 4-entry maps, iOpponentID, cMetaGame,
// cHeatNumber, cSeries, cSeriesLevel, iEventID, none of which retail has), placed at retail offsets by:
//   [P] the July 2003 preview's cBENewRaceInterface inline accessors on newRace (preview 0x005B6540, same offsets
//       as retail): getNumberRiders lw 0x30, getNumberHumans 0x34, getNumberMission 0x38, getAIRiders_Num 0x3C,
//       setLocationID sw 0x40, get/setRaceTime 0x44, getGameMode lb 0x48, getGamePath lb 0x49, getGameEvent lb 0x4A,
//       get/setDifficultyLevel lb/sb 0x4B. Retail keeps some out of line (be/bedbreward:
//       cBENewRaceInterface_getNumberRiders = 0x30,
//       cBENewRaceInterface_getNumberMission = 0x38, cBENewRaceInterface_setRaceTime/cBENewRaceInterface_getRaceTime =
//       set/get 0x44);
//   [R] retail's setters: cBENewRaceInterface_assignPlayer(gate, player) writes playerID[gate] = player and
//       riderID[player] = gate; setNumberHumans/setNumberMission/setNumberAI recompute iNumberRiders;
//       setGameMode sb 0x48, cBENewRaceInterface_setGamePath sb 0x49, setGameEvent sb 0x4A;
//   [O] On Tour's member order.
// The preview's newRace continues past 0x4C (getGameTournament lw 0x50, getCurrentTournamentRound lw 0x54); retail's
// struct copy stops at 0x4C, so those are separate statics in retail.

#include "layout.h"

struct cBERaceDB {
    int playerID[6];            // 0x00 [O] (On Tour [4]); [R] assignPlayer: playerID[gate] = player;
                                //      setNumberHumans resets it to 0..5
    int riderID[6];             // 0x18 [O]; [R] assignPlayer: riderID[player] = gate; setNumberHumans resets 0..5
    int iNumberRiders;          // 0x30 [P] getNumberRiders; [R] = humans + mission + AI (was D_00535BF8)
    int iNumberHumans;          // 0x34 [P] getNumberHumans; [R] setNumberHumans (was D_00535BFC); [U] cAI_InitPlayers
    int iNumberMissionRiders;   // 0x38 [P] getNumberMission; [R] setNumberMission (was D_00535C00)
    int iNumberAI;              // 0x3C [P] getAIRiders_Num; [R] setNumberAI (was D_00535C04); [U]
                                //       cAI_initComputerRiders
    int iLocationIdx;           // 0x40 [P] setLocationID; [U] cBENewRaceInterface_getLocation(ID) (was D_00535C08)
    int iRaceTime;              // 0x44 [P] get/setRaceTime; [R] cBENewRaceInterface_setRaceTime /
                                //       cBENewRaceInterface_getRaceTime (was D_00535C0C)
    signed char cGameMode;      // 0x48 [P] getGameMode; [R] setGameMode stores its argument (was D_00535C10)
    signed char cGamePath;      // 0x49 [P] getGamePath; [R] cBENewRaceInterface_setGamePath: 0, 1, 2
                                //      (was D_00535C11); 2 is On Tour's kGAMEPATH_MULTIPLAYER, which the main menu's
                                //       online
                                //      item also sets; [U] cGameModeMan::iCurrentGamePath copies it
    signed char cGameEvent;     // 0x4A [P] getGameEvent; [R] setGameEvent (was D_00535C12); [U] initGameMode's argument
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

// cBENewRaceInterface: the back end's new-race interface - sets up the next race in cBENewRaceInterface_newRace above
// (assignPlayer, setNumberHumans / Mission / AI, setGameMode / Path / Event, the location) and answers questions about
// locations, events, goals, time limits and rivals from the library tables. The object holds no race data: like every
// BE interface it is a cBEInterface (three word bools + vptr, 0x10 bytes), and its methods ignore `this` and work on
// newRace and the other statics (mNextLocation D_004A11BC, commandLineTrack cBENewRaceInterface_commandLineTrack; On Tour also has
// bIsBamLocation). One instance, created on demand by cBENewRaceInterface_getThis (`cMemMan_alloc(0x10,
// "BENewRaceInt", 0, 0)`, bIsActive = 0, vtable cBENewRaceInterface_vtbl) into cBENewRaceInterface::instance (retail
// cBENewRaceInterface_instance, D_004A11B4); the rest of the game reaches it as cBE_getInterface(cBE_getBE(), 0).
// EA's source: d:/ssx/code/source/be/beintnewrace.cpp (preview assert string).
//
// Size 0x10 [R] (the allocation; the July 2003 preview's getThis allocates 0x10 too). [O] On Tour's
// cBENewRaceInterface has no data members of its own (size 0x8 = its cBEInterface), and neither does retail's.
// Base cBEInterface, written out inline as in be/beintreward.h, whose kBEInterfaceVt_* slot enum applies: vtable
// cBENewRaceInterface_vtbl (0x45AE58), slot 1 submit = cBENewRaceInterface_submit (newRace -> cBELibrary_raceDB),
// slot 2 discard = cBENewRaceInterface_discard (back), slots 3/4 cBEInterface_writeToReplayFrame /
// cBEInterface_readFromReplayFrame.
// Evidence tags as above, plus [R] retail's getThis (allocation, bIsActive, vptr) and constructor
// cBENewRaceInterface_cBENewRaceInterface (vptr), [U] how retail code uses the field, [O] On Tour's cBEInterface
// (bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools are 1 byte, retail's are words).
struct cBENewRaceInterface {
    // cBEInterface (0x00-0x0F; see be/beintreward.h)
    int bCanSubmit;                  // 0x000 [O] cBEInterface::bCanSubmit; no retail access seen
    int bDataChanged;                // 0x004 [O] cBEInterface::bDataChanged; no retail access seen
    int bIsActive;                   // 0x008 [O] cBEInterface::bIsActive; [R] getThis sets 0; [U]
                                     //       cBENewRaceInterface_setNewRaceActive (static: instance->bIsActive = arg);
                                     //       cBEPreRaceState_onEnter/onExit set 0/1, cBERaceState_onExit clears it
    void** mVtbl;                    // 0x00C [R] getThis and cBENewRaceInterface_cBENewRaceInterface store
                                     //       cBENewRaceInterface_vtbl; [P] the preview's __19cBENewRaceInterface stores
                                     //       its vtable at 0xC; [U] slot 1 (submit) is called through it by ~18
                                     //       functions (cAI_initComputerRiders, cGFTrackStartState_preloadUpdate,
                                     //       cSectionMan_preLoadMode, cOVTemplate_Dialog_onWidgetEvent, ...), slot 2
                                     //       (discard) by cBELibrary_readFromData and cBENetworkInterface_setSyncData
};

CHECK_OFFSET(cBENewRaceInterface, bCanSubmit, 0x00);
CHECK_OFFSET(cBENewRaceInterface, bDataChanged, 0x04);
CHECK_OFFSET(cBENewRaceInterface, bIsActive, 0x08);
CHECK_OFFSET(cBENewRaceInterface, mVtbl, 0x0C);
CHECK_SIZE(cBENewRaceInterface, 0x10);

// The one new-race interface (On Tour: cBENewRaceInterface::instance), created by cBENewRaceInterface_getThis.
extern cBENewRaceInterface* cBENewRaceInterface_instance;

// ---- The game-path and game-event tables (views group D) ----
//
// tBEPathDB: one game path (World Circuit, Quick Play, Multiplayer) and the race modes it allows.
// [O] On Tour struct tBEPathDB {char cModes[8]; char strPathName[60];} (0x44) and the static cBELibrary::gamePathDB
// (tBEPathDB[3]); [R] retail's cBELibrary_gamePathDB (0x0043E700) holds three records 0x43 apart ("World Circuit",
// "Quick Play", "Multiplayer" at +0x7 of each), so retail has seven mode flags; [U] cBENewRaceInterface_setGameMode copies
// the current path's record and refuses a mode whose flag is not 1.
struct tBEPathDB {
    signed char cModes[7];          // 0x00 [O] (On Tour [8]); [U] setGameMode: cModes[mode] == 1 allows the mode
    char strPathName[60];           // 0x07 [O]; [R] the path's name ("World Circuit", ...)
};

CHECK_OFFSET(tBEPathDB, cModes, 0x00);
CHECK_OFFSET(tBEPathDB, strPathName, 0x07);
CHECK_SIZE(tBEPathDB, 0x43);

// tBEEventDB: one game event. [O] On Tour struct tBEEventDB {int iValidMode; char strEventName[80];} (0x54) and the
// static cBELibrary::gameEventDB; [R] retail's cBELibrary_gameEventDB (0x0043E978) has 0x54-byte records;
// [U] cBENewRaceInterface_setGameEvent copies record [event] and reads iValidMode.
struct tBEEventDB {
    int iValidMode;                 // 0x00 [O]; [U] setGameEvent
    char strEventName[80];          // 0x04 [O]
};

CHECK_OFFSET(tBEEventDB, iValidMode, 0x00);
CHECK_OFFSET(tBEEventDB, strEventName, 0x04);
CHECK_SIZE(tBEEventDB, 0x54);


#endif /* BE_BEINTNEWRACE_H */
