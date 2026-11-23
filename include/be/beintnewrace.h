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
#include "be/beinterface.h"

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
// Base cBEInterface, written out inline (the class itself: be/beinterface.h, whose virtuals give the slots): vtable
// cBENewRaceInterface_vtbl (0x45AE58), slot 1 submit = cBENewRaceInterface_submit (newRace -> cBELibrary_raceDB),
// slot 2 discard = cBENewRaceInterface_discard (back), slots 3/4 cBEInterface_writeToReplayFrame /
// cBEInterface_readFromReplayFrame.
// Evidence tags as above, plus [R] retail's getThis (allocation, bIsActive, vptr) and constructor
// cBENewRaceInterface_cBENewRaceInterface (vptr), [U] how retail code uses the field, [O] On Tour's cBEInterface
// (bCanSubmit, bDataChanged, bIsActive, then the vptr; On Tour's bools are 1 byte, retail's are words).
struct cBENewRaceInterface : public cBEInterface {
    // cBEInterface (0x00-0x0F, be/beinterface.h):
    //   bIsActive: [R] getThis sets 0; [U] cBENewRaceInterface_setNewRaceActive (static: instance->bIsActive = arg);
    //       cBEPreRaceState_onEnter/onExit set 0/1, cBERaceState_onExit clears it
    //   the vptr (0x0C): [R] getThis and cBENewRaceInterface_cBENewRaceInterface store cBENewRaceInterface_vtbl; [P]
    //       the preview's __19cBENewRaceInterface stores its vtable at 0xC; [U] slot 1 (submit) is called through it
    //       by ~18 functions (cAI_initComputerRiders, cGFTrackStartState_preloadUpdate, cSectionMan_preLoadMode,
    //       cOVTemplate_Dialog_onWidgetEvent, ...), slot 2 (discard) by cBELibrary_readFromData and
    //       cBENetworkInterface_setSyncData
};

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

// tBELocationDB: one race location (17 tracks, 5 stations and the debug track). [O] On Tour struct tBELocationDB
// {eBELocationID locationID; char locationName[32]; char shortName[16]; char fileName[16]; char bigfileName[32];
// eBEPeakID segmentID; eBELocationType type; int scriptID; eLocationDifficulty difficulty;} (0x74) and the static
// cBELibrary::locationDB; [O] On Tour `const tBELocationDB & cBENewRaceInterface::getLocation()`. Retail's
// cBELibrary_locationDB (0x0043D950) holds 23 records of 0x64 ([R] cBENewRaceInterface_getLocation indexes it by
// iLocationIdx * 0x64; the data: 0, "Snow Jam", "Snow Jam", "ARA1", "BAM" ... 22, "Debug Track", "DEBUG", "dbg", "DBG"),
// so bigfileName is 16 bytes in retail (On Tour 32). The enum members are words (int) here.
// The table itself is not declared here: be/bedbprofile, be/beintreward, be/beintplayer and be/beintnewrace still
// declare cBELibrary_locationDB with their own views.
struct tBELocationDB {
    int locationID;                 // 0x00 [O] eBELocationID; [R] each record holds its own index (0..22); [U]
                                    //      cBENewRaceInterface_getLocationIDFromScript returns it
    char locationName[32];          // 0x04 [O]; [R] "Snow Jam", "Metro-City", ...; [U] cBENewRaceInterface_getLocationName1
                                    //      returns this + 0x4; debugfe prints it
    char shortName[16];             // 0x24 [O]; [R] the short forms ("Metro", "Ridge", "Intimid", "DEBUG")
    char fileName[16];              // 0x34 [O]; [R] "ARA1", "BRA2", ... "dbg"; [U] setLocationFile strcpys the name here;
                                    //      debugfe prints it
    char bigfileName[16];           // 0x44 [O] (On Tour [32]); [R] "BAM" / "DBG"; [U] setLocationFile strcpys here (+0x10
                                    //      from fileName); debugfe prints it
    int segmentID;                  // 0x54 [O] eBEPeakID; [R] 0, 0, 1, 1, 2 ... (the location's peak); [U]
                                    //      cBERewardInterface_getTrackPeakID returns it; the map screen reads it as the peak
    int type;                       // 0x58 [O] eBELocationType; [R] 0 for the 17 tracks, 1 for the five stations; [U]
                                    //      cBENewRaceInterface_getLocationType returns it
    int scriptID;                   // 0x5C [O]; [R] 0..22 (-1 for the debug track); [U]
                                    //      cBENewRaceInterface_getLocationIDFromScript searches it
    int difficulty;                 // 0x60 [O] eLocationDifficulty; [R] 0..2 rising within each peak; [U]
                                    //      cBENewRaceInterface_getLocationDifficulty returns it
};

CHECK_OFFSET(tBELocationDB, locationID, 0x00);
CHECK_OFFSET(tBELocationDB, locationName, 0x04);
CHECK_OFFSET(tBELocationDB, shortName, 0x24);
CHECK_OFFSET(tBELocationDB, fileName, 0x34);
CHECK_OFFSET(tBELocationDB, bigfileName, 0x44);
CHECK_OFFSET(tBELocationDB, segmentID, 0x54);
CHECK_OFFSET(tBELocationDB, type, 0x58);
CHECK_OFFSET(tBELocationDB, scriptID, 0x5C);
CHECK_OFFSET(tBELocationDB, difficulty, 0x60);
CHECK_SIZE(tBELocationDB, 0x64);

// tBEPeakDB: one peak-event tier (the goals, time limit, checkpoint bonuses and prize of a race / freestyle event at one
// of three tiers). [O] On Tour struct tBEPeakDB (0x14, the same layout) and the static cBELibrary::peakDB
// (tBEPeakDB[18]); [R] retail's cBELibrary_peakDB (0x00440D18) is indexed (gameEvent - 6) * 3 + tier for the six
// peak events 6..11 and three tiers (18 records of 0x14); [U] the cBENewRaceInterface_getPeak* getters in be/beintnewrace.
// The table itself is not declared here (be/beintnewrace declares cBELibrary_peakDB with its own view).
struct tBEPeakDB {
    short eventID;                  // 0x00 [O]; no retail access seen
    short round;                    // 0x02 [O]; no retail access seen
    unsigned short goal;            // 0x04 [O]; [U] cBENewRaceInterface_getPeakGoal returns it
    unsigned short timeLimit;       // 0x06 [O]; [U] cBENewRaceInterface_getPeakTimeLimit returns it
    unsigned short splitGoals[5];   // 0x08 [O]; [U] cBENewRaceInterface_getPeakSplitTimePoints returns splitGoals[checkpoint]
    unsigned short prize;           // 0x12 [O]; [U] cBENewRaceInterface_getPeakRewardAmount returns prize * 100
};

CHECK_OFFSET(tBEPeakDB, eventID, 0x00);
CHECK_OFFSET(tBEPeakDB, round, 0x02);
CHECK_OFFSET(tBEPeakDB, goal, 0x04);
CHECK_OFFSET(tBEPeakDB, timeLimit, 0x06);
CHECK_OFFSET(tBEPeakDB, splitGoals, 0x08);
CHECK_OFFSET(tBEPeakDB, prize, 0x12);
CHECK_SIZE(tBEPeakDB, 0x14);

extern tBEPathDB cBELibrary_gamePathDB[3];      // [O] cBELibrary::gamePathDB; [R] 0x0043E700
extern tBEEventDB cBELibrary_gameEventDB[];     // [O] cBELibrary::gameEventDB (tBEEventDB[15] in On Tour); [R] 0x0043E978


#endif /* BE_BEINTNEWRACE_H */
