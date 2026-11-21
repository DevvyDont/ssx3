#ifndef AI_AI_H
#define AI_AI_H

// cAI: the AI / rider manager of a running event (one instance per cGame: cGame_load does
// `cAI_cAI(operator new(0xD0, "AI"))` and stores it in cGame::AI; reached as SSXApp->Game->AI). It owns every
// rider of the race (players, computer riders, podium/lodge actors, the mission rider), runs the AI state machine
// (game init, freeride, pre-race, countdown, race, end race, shutdown) and holds the focus and danger managers.
// EA's header is ../source/ai/ai.h (preview assert strings "idx >= 0 && idx < getNumRiders()" ...).
//
// Size 0xD0: [R] the allocation above. The 6-rider cap of the race is the fixed arrays at 0x28..0x74:
// Riders[6], Players[2], Computers[5], Actors[5], MissionRiders[1] (notes/subsystems/riders-cap.md).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
//   Base: cAIState (On Tour: size 0x24, 0x00-0x24; retail 0x00-0x28: its bools are words and it has the extra
//   EndOfRaceTimer). cAI's own vptr follows all its data, at 0xCC (_vt$3cAI = D_00458488, 8 slots).
//   The seven AI state handlers are member objects (cAIStateHandler subclasses: a vptr each, cPreRaceHandler
//   also one int); cAI_cAI stores their vtables (names from the preview vtables, builds/x/v2/vtables_v2.tsv).
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cAI, base cAIState), placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors (getAIState lw 0x0, getAITick 0x8, getEventTimerTick 0xC,
//       setComputerRidersActive 0x14, getCountdownTicksLeft 0x18, get/setPodiumActor 0x1C[i], getRider 0x24[i],
//       getPlayer 0x3C[i], getComputer 0x44[i], getActor 0x58[i], getMissionRider 0x6C[i], getNumRiders 0x74 ...
//       getNumMissionRiders 0x84, getComputerActorsActive 0x88, getFocusMan 0xA0, getDangerMan 0xA4).
//       Retail = preview + 4 from 0x18 on (retail adds EndOfRaceTimer at 0x18); the array sizes are the same;
//   [R] retail's constructor cAI_cAI (vtables, "FocusMan" 0x8E0 and "DangerMan" 0xB60 allocations, 0x74 = 0) and
//       cAI::init (cAI_init, clears the counts, the arrays and the state);
//   [U] how retail code uses the field;
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [P] or [R].
// On Tour has 4 riders / 4 players / 3 computers / 3 actors and byte bools; retail has 6 / 2 / 5 / 5 and word bools.
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"

struct cRider;
struct cPlayer;
struct cComputer;
struct cMissionRider;
struct cFocusMan;
struct cDangerMan;

// cAIStateHandler and its subclasses (cGameInitHandler, cFreerideHandler, cCountdownHandler, cRaceHandler,
// cEndRaceHandler, cShutdownHandler): no data, only a vptr (On Tour size 0x4). Their methods take (this, cAI& ai):
// OnGainState / OnLoseState / OnUpdate (e.g. cRaceHandler_OnUpdate, cRaceHandler: ai.EventTimerTick++).
struct cAIStateHandler {
    void** mVtbl; // 0x0 [R] stored by cAI_cAI
};

CHECK_OFFSET(cAIStateHandler, mVtbl, 0x0);
CHECK_SIZE(cAIStateHandler, 0x4);

// cPreRaceHandler: a cAIStateHandler with one int (On Tour size 0x8).
struct cPreRaceHandler {
    void** mVtbl; // 0x0 [R] _vt$15cPreRaceHandler (cPreRaceHandler_vtbl), stored by cAI_cAI
    int unk_04;   // 0x4 [U] written by its OnGainState (cPreRaceHandler_OnGainState)
};

CHECK_OFFSET(cPreRaceHandler, mVtbl, 0x0);
CHECK_OFFSET(cPreRaceHandler, unk_04, 0x4);
CHECK_SIZE(cPreRaceHandler, 0x8);

struct cAI {
    // ---- base cAIState (0x00-0x28) ----
    int CurrentAIState;             // 0x00 [P] getAIState; [O] tAIState; [U] setAIState/forceAIState
    int PreviousAIState;            // 0x04 [O]; [U] setAIState saves the old state here
    int AITick;                     // 0x08 [P] getAITick/setAITick; [R] commonInit zeroes it; [U] AITick()
    int EventTimerTick;             // 0x0C [P] get/set/incrementEventTimerTick, getRaceTimeElapsed; [U] race clock
    int IsAIReadyToRace;            // 0x10 [O] (bool); [R] init zeroes it, initComputerRiders sets 1
    int ComputerRidersActive;       // 0x14 [P] setComputerRidersActive (bool); [U] prepareToPurgeComputerRiders = 0
    int EndOfRaceTimer;             // 0x18 [O]; [R] commonInit = -1; [U] onlineAICheck counts it down to end the race
    int CountdownTicksLeft;         // 0x1C [P] get/setCountdownTicksLeft (preview 0x18); [U] cCountdownHandler
    int PodiumActorList[2];         // 0x20 [P] get/setPodiumActor (eBECharacters, "Index < kMaxNumberOfAIActors")
    // ---- cAI ----
    cRider* Riders[6];              // 0x28 [P] getRider; [R] init clears 6; [U] the init functions fill it
    cPlayer* Players[2];            // 0x40 [P] getPlayer; [R] InitPlayers
    cComputer* Computers[5];        // 0x48 [P] getComputer; [R] initComputerRiders ("ComputerRider", 0x1A90)
    cComputer* Actors[5];           // 0x5C [P] getActor; [R] initComputerActors
    cMissionRider* MissionRiders[1]; // 0x70 [P] getMissionRider; [R] initMissionRiders
    int RaceRankType;               // 0x74 [O] tRaceRankType; [R] cAI_cAI = 0; [U] setRaceRankType (cAI_setRaceRankType)
    int NumberOfRidersAllocated;    // 0x78 [P] getNumRiders; [U] the init functions' next rider index
    int NumberOfPlayersAllocated;   // 0x7C [P] getNumPlayers
    int NumberOfComputersAllocated; // 0x80 [P] getNumComputers; [U] initComputerRiders
    int NumberOfActorsAllocated;    // 0x84 [P] getNumActors; [U] initComputerActors
    int NumberOfMissionRidersLoaded; // 0x88 [P] getNumMissionRiders; [U] initMissionRiders
    int ComputerActorsActive;       // 0x8C [P] get/setComputerActorsActive (bool)
    int MissionRidersActive;        // 0x90 [O] (bool); [U] init / purgeMissionRiders clear it
    int NumberOfActorsRequired;     // 0x94 [O]; [U] initAIActors(n) stores n, initComputerActors reads it
    cAIStateHandler* CurrentAIStateHandler;  // 0x98 [O]; [U] setAIState / forceAIState / AIStateUpdate
    cAIStateHandler* PreviousAIStateHandler; // 0x9C [O]; [U] setAIState
    const char* AIStateName;        // 0xA0 [O]; [U] forceAIState stores it
    cFocusMan* FocusMan;            // 0xA4 [P] getFocusMan; [R] cMemMan_alloc(0x8E0, "FocusMan")
    cDangerMan* DangerMan;          // 0xA8 [P] getDangerMan; [R] cDangerMan_cDangerMan(alloc(0xB60, "DangerMan"), this)
    cAIStateHandler mGameInitHandler; // 0xAC [R] _vt$16cGameInitHandler (cGameInitHandler_vtbl)
    cAIStateHandler mFreerideHandler; // 0xB0 [R] _vt$16cFreerideHandler (cFreerideHandler_vtbl)
    cPreRaceHandler mPreRaceHandler;  // 0xB4 [R] _vt$15cPreRaceHandler (cPreRaceHandler_vtbl)
    cAIStateHandler mCountdownHandler; // 0xBC [R] _vt$17cCountdownHandler (cCountdownHandler_vtbl)
    cAIStateHandler mRaceHandler;     // 0xC0 [R] _vt$12cRaceHandler (D_00458528)
    cAIStateHandler mEndRaceHandler;  // 0xC4 [R] cEndRaceHandler_vtbl (unnamed in the preview); [O] the member in this place
    cAIStateHandler mShutdownHandler; // 0xC8 [R] _vt$16cShutdownHandler (D_004584D8)
    void** mVtbl;                   // 0xCC [R] _vt$3cAI (D_00458488), stored by cAI_cAI and the destructor
};

// Virtual slots of _vt$3cAI (D_00458488), as `vt[k].fn(this + vt[k].delta)`. Names from On Tour (merged.tsv).
enum {
    kAIVt_delete = 1,               // cAI__cAI ~cAI
    kAIVt_init = 2,                 // cAI_init
    kAIVt_update = 3,               // cAI_update
    kAIVt_render = 4,               // cAI_render
    kAIVt_restart = 5,              // cAI_restart (tAIState)
    kAIVt_InitPlayers = 6,          // cAI_InitPlayers
    kAIVt_writeToReplayFrame = 7,   // cAI_writeToReplayFrame
    kAIVt_readFromReplayFrame = 8   // cAI_readFromReplayFrame
};

CHECK_OFFSET(cAI, CurrentAIState, 0x00);
CHECK_OFFSET(cAI, PreviousAIState, 0x04);
CHECK_OFFSET(cAI, AITick, 0x08);
CHECK_OFFSET(cAI, EventTimerTick, 0x0C);
CHECK_OFFSET(cAI, IsAIReadyToRace, 0x10);
CHECK_OFFSET(cAI, ComputerRidersActive, 0x14);
CHECK_OFFSET(cAI, EndOfRaceTimer, 0x18);
CHECK_OFFSET(cAI, CountdownTicksLeft, 0x1C);
CHECK_OFFSET(cAI, PodiumActorList, 0x20);
CHECK_OFFSET(cAI, Riders, 0x28);
CHECK_OFFSET(cAI, Players, 0x40);
CHECK_OFFSET(cAI, Computers, 0x48);
CHECK_OFFSET(cAI, Actors, 0x5C);
CHECK_OFFSET(cAI, MissionRiders, 0x70);
CHECK_OFFSET(cAI, RaceRankType, 0x74);
CHECK_OFFSET(cAI, NumberOfRidersAllocated, 0x78);
CHECK_OFFSET(cAI, NumberOfPlayersAllocated, 0x7C);
CHECK_OFFSET(cAI, NumberOfComputersAllocated, 0x80);
CHECK_OFFSET(cAI, NumberOfActorsAllocated, 0x84);
CHECK_OFFSET(cAI, NumberOfMissionRidersLoaded, 0x88);
CHECK_OFFSET(cAI, ComputerActorsActive, 0x8C);
CHECK_OFFSET(cAI, MissionRidersActive, 0x90);
CHECK_OFFSET(cAI, NumberOfActorsRequired, 0x94);
CHECK_OFFSET(cAI, CurrentAIStateHandler, 0x98);
CHECK_OFFSET(cAI, PreviousAIStateHandler, 0x9C);
CHECK_OFFSET(cAI, AIStateName, 0xA0);
CHECK_OFFSET(cAI, FocusMan, 0xA4);
CHECK_OFFSET(cAI, DangerMan, 0xA8);
CHECK_OFFSET(cAI, mGameInitHandler, 0xAC);
CHECK_OFFSET(cAI, mFreerideHandler, 0xB0);
CHECK_OFFSET(cAI, mPreRaceHandler, 0xB4);
CHECK_OFFSET(cAI, mCountdownHandler, 0xBC);
CHECK_OFFSET(cAI, mRaceHandler, 0xC0);
CHECK_OFFSET(cAI, mEndRaceHandler, 0xC4);
CHECK_OFFSET(cAI, mShutdownHandler, 0xC8);
CHECK_OFFSET(cAI, mVtbl, 0xCC);
CHECK_SIZE(cAI, 0xD0);

#endif /* AI_AI_H */
