#ifndef MAIN_GAMESTATE_H
#define MAIN_GAMESTATE_H

// cGameState: the base of the game-flow states (cGF*State: track start, section load, user select, event start,
// riding, gate, finish line, next heat, pre / post event, podium, end, transport wait, session teleport, replay start)
// that cGame runs one at a time (main/game.h mStates, mPrevState / mCurrentState / mPreloadingState). Each state is
// built in place inside cGame by cGame_cGame; the cGameState ctor is inline there and in the derived ctors
// (mNextState 0, mStartupFlag 0, mOverlayDone 0, vptr cGameState_vtbl), then the derived vtable is stored over it.
// EA's source: d:/ssx/code/source/main/gamestate.cpp (preview strings); retail methods: gotoNextState,
// preloadNextState, writeToReplayFrame, readFromReplayFrame in unit fe/festateloadscreen (0x00233AA0 ... 0x00233B28);
// the destructor and the eight default virtuals in main/ssxappdtor (0x00243C80 ... 0x00243CE8).
// Class static: cGameState::mpGame = the global cGameState_mpGame (0x004A2C68, cGame*); gotoNextState and
// preloadNextState call cGame_setState / cGame_setPreloadState on it.
//
// Size 0x10:
//   [R] every derived state starts its own members at 0x10 (cGFTrackStartState below; the derived ctors write
//       0x0 / 0x4 / 0x8 / the vptr 0xC, then their own fields from 0x10).
//   [O] On Tour's class is 0x10: eGFState mNextState, int mStartupFlag, bool mOverlayDone (a word in retail: the
//       derived states lw / sw 0x8), then the vptr.
// Vtable cGameState_vtbl (0x0047D6E8, 9 slots): 1 dtor cGameState__cGameState, 2 preloadInit, 3 preloadUpdate, 4
// preloadCancel, 5 gainFocus, 6 update, 7 loseFocus, 8 isPreloadComplete, 9 onWidgetEvent (all cGameState_*__FPv,
// empty or returning a constant); the destructor stores it at 0xC again.
//
// cGFTrackStartState: the game-flow state that starts a track (pre-loads it, plays the first-time "BC" video,
// waits for the transport to end). One instance embedded in cGame (main/game.h mStates, at cGame + 0x158), built in
// place by cGame_cGame (cGFTrackStartState_cGFTrackStartState(game + 0x158)).
//
// Size 0x20: [R] cGame_cGame builds the next state (cGFSectionLoadState) at cGame + 0x178 = 0x158 + 0x20, and the
// constructor writes up to 0x1C. Same size and offsets as On Tour's class cGFTrackStartState (0x20).
// Base cGameState (On Tour size 0x10): its data at 0x0-0x8 and its vptr at 0xC (g++ 2.95 puts the vptr after the
// data of the class that declares the virtuals); written out inline below.
// EA's source: d:/ssx/code/source/main/gamestate.cpp (preview strings); retail methods: unit main/gamestate
// 0x00234EF8-0x00235A18, the dtor (0x00243F20) in unit main/ssxappdtor.
//
// Vtable cGFTrackStartState_vtbl (0x0047D428, 9 slots, On Tour cGameState order): 1 dtor, 2 preloadInit,
// 3 preloadUpdate, 4 preloadCancel, 5 gainFocus, 6 update, 7 loseFocus, 8 isPreloadComplete,
// 9 cGameState_onWidgetEvent__FPv (inherited). writeToReplayFrame / readFromReplayFrame are not in it (non-virtual
// in retail, or called directly).
//
// FLAT layouts, like main/game.h. Member names, order and types are EA's, from SSX On Tour's PSP DWARF ([O] class
// cGFTrackStartState and cGameState). Enums and bools are words. Evidence tags: [R] retail constructors /
// destructors and the derived classes' layout; [P] the July 2003 preview (layout_xlate cGameState: the vptr 0x8 -> 0xC
// in the paired destructor); [U] how retail code uses the field; [O] On Tour's DWARF.

#include "layout.h"

// A game-flow state id (cGameState::mNextState; cGame::mCurrentStateID, mPendingStateID, mPrevStateID). [O] On Tour's
// enum eGFState, names and values; [R] retail's ids are the same: cGame_updateState / cGame_readFromReplayFrame map
// 1-16 to the states cGame_cGame builds in this order (cGame's mStates, below), and each state's ctor stores the id
// of the state that follows it (cGFEventStartState 2, cGFPreEventState 3, cGFGateState 4, cGFRidingState 5).
enum eGFState {
    kGFSM_Null = 0, kGFSM_EventStart = 1, kGFSM_PreEvent = 2, kGFSM_Gate = 3, kGFSM_Riding = 4, kGFSM_Finishline = 5,
    kGFSM_End = 6, kGFSM_PostEvent = 7, kGFSM_EndWait = 8, kGFSM_TransportWait = 9, kGFSM_TrackStart = 10,
    kGFSM_SectionLoad = 11, kGFSM_Podium = 12, kGFSM_NextHeat = 13, kGFSM_UserSelect = 14, kGFSM_SessionTeleport = 15,
    kGFSM_ReplayStart = 16, kGFSM_NumStates = 17
};

struct cGameState {
    int mNextState;                 // 0x00 [O] eGFState; [U] gotoNextState passes it to cGame_setState (with
                                    //      mStartupFlag) when non-zero; writeToReplayFrame / readFromReplayFrame copy
                                    //      its 4 bytes; derived gainFocus / update store it
    int mStartupFlag;               // 0x04 [O]; [U] gotoNextState passes it; derived ctors zero it
    int mOverlayDone;               // 0x08 [O] bool; [U] derived states: cGFPreEventState / cGFPostEventState /
                                    //      cGFUserSelectState gainFocus clear it, their update tests it
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl (inline ctor in cGame_cGame; the dtor
                                    //      cGameState__cGameState); [P] (preview 0x8)

    // Methods bound to their retail symbols by asm labels.
    void gotoNextState() __asm__("cGameState_gotoNextState");
};

CHECK_OFFSET(cGameState, mNextState, 0x00);
CHECK_OFFSET(cGameState, mStartupFlag, 0x04);
CHECK_OFFSET(cGameState, mOverlayDone, 0x08);
CHECK_OFFSET(cGameState, mVtbl, 0x0C);
CHECK_SIZE(cGameState, 0x10);

struct cGFTrackStartState {
    // --- cGameState (inline in the ctor: mNextState = 0, vptr = cGameState_vtbl, cGameState::mpGame = 0)
    int mNextState;                 // 0x00 [O] eGFState; [R] the ctor stores 0 (cGameState) then 4; [U] gainFocus
                                    //      stores the next state with mStartupFlag (inline gotoNextState)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0; [U] gainFocus stores it with mNextState
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] the ctor stores cGameState_vtbl, then cGFTrackStartState_vtbl; the dtor
                                    //      cGFTrackStartState__cGFTrackStartState (unit main/ssxappdtor) stores it
                                    //       again
    // --- cGFTrackStartState
    int mPreLoadState;              // 0x10 [O]; [R] ctor 5; [U] preloadInit / preloadUpdate / preloadCancel set it,
                                    //      isPreloadComplete and preloadUpdate test it; written to / read from the
                                    //       replay
                                    //      frame (writeToReplayFrame / readFromReplayFrame pass its address)
    // mPreLoadState's steps (names ours). [U] cGFTrackStartState_preloadUpdate waits for the track scripts and queues
    // the intro NIS (0), waits for the NIS lists (1), rebuilds the AI riders (2), loads them (3) and holds an intro
    // for 180 frames (4); 5 is done: the ctor and preloadCancel store it, isPreloadComplete tests it.
    enum ePreLoadState {
        kPreLoad_TrackScripts = 0, kPreLoad_WaitNIS = 1, kPreLoad_InitRiders = 2, kPreLoad_LoadRiders = 3,
        kPreLoad_Intro = 4, kPreLoad_Done = 5
    };
    int mPlayFirstTimeBCVideo;      // 0x14 [O] bool; [R] ctor 0; [U] preloadInit / loseFocus set it, gainFocus and
                                    //      preloadUpdate test it; in the replay frame
    int mPreLoadTicks;              // 0x18 [O]; [R] ctor 0; [U] preloadInit 0, preloadUpdate counts it; in the replay
                                    //      frame
    int mCheckTransportEnd;         // 0x1C [O] bool; [R] ctor 0; [U] gainFocus sets it, update tests and clears it

    // Methods bound to their retail symbols by asm labels.
    void preloadInit() __asm__("cGFTrackStartState_preloadInit");
    void preloadUpdate() __asm__("cGFTrackStartState_preloadUpdate");
    void preloadCancel() __asm__("cGFTrackStartState_preloadCancel");
    bool isPreloadComplete() __asm__("cGFTrackStartState_isPreloadComplete");
    void gainFocus() __asm__("cGFTrackStartState_gainFocus");
    void update() __asm__("cGFTrackStartState_update");
    void loseFocus() __asm__("cGFTrackStartState_loseFocus");
};

CHECK_OFFSET(cGFTrackStartState, mNextState, 0x00);
CHECK_OFFSET(cGFTrackStartState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFTrackStartState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFTrackStartState, mVtbl, 0x0C);
CHECK_OFFSET(cGFTrackStartState, mPreLoadState, 0x10);
CHECK_OFFSET(cGFTrackStartState, mPlayFirstTimeBCVideo, 0x14);
CHECK_OFFSET(cGFTrackStartState, mPreLoadTicks, 0x18);
CHECK_OFFSET(cGFTrackStartState, mCheckTransportEnd, 0x1C);
CHECK_SIZE(cGFTrackStartState, 0x20);

// ------------------------------------------------------------------------------------------------------------------
// The other game-flow states. All derive from cGameState (0x10, above; written out inline in each struct) and live in
// cGame's mStates (main/game.h, 0xB0-0x1FB), built in place by cGame_cGame in eGFState order (On Tour's enum; retail's
// state ids are the same: cGame_updateState / cGame_readFromReplayFrame map ids 1-16 to these offsets):
//   0x0B0 cGFEventStartState (1)   0x0D0 cGFPreEventState (2)   0x0E0 cGFGateState (3)       0x0F0 cGFRidingState (4)
//   0x100 cGFFinishlineState (5)   0x118 cGFEndState (6)        0x128 cGFPostEventState (7)  0x138 cGFEndWaitState (8)
//   0x148 cGFTransportWaitState (9) 0x158 cGFTrackStartState (10) 0x178 cGFSectionLoadState (11)
//   0x18C cGFPodiumState (12)      0x19C cGFNextHeatState (13)  0x1B8 cGFUserSelectState (14)
//   0x1DC cGFSessionTeleportState (15)                           0x1EC cGFReplayStartState (16); mPrevState at 0x1FC.
// Sizes: [R] each state's size is the distance to the next one in cGame_cGame (the last one ends at mPrevState,
// main/game.h); [O] On Tour's sizes agree once its 1-byte bools are retail words. Each ctor (out of line in
// main/ssxappdtor, main/gamestate or fe/festateloadscreen, and inlined in cGame_cGame) runs the inline cGameState ctor
// (mNextState 0, cGameState_vtbl, cGameState::mpGame 0), then stores its own vtable, its default mNextState (the state
// that follows it) and mStartupFlag 0 (cGFNextHeatState: 3). Each destructor (main/ssxappdtor) stores cGameState_vtbl
// at 0xC and frees the object when bit 0 of its flags is set. Every vtable has 9 slots in cGameState's order (1 dtor, 2
// preloadInit, 3 preloadUpdate, 4 preloadCancel, 5 gainFocus, 6 update, 7 loseFocus, 8 isPreloadComplete, 9
// onWidgetEvent); a slot a state does not override holds cGameState_*__FPv. writeToReplayFrame / readFromReplayFrame
// are not virtual (cGame_writeToReplayFrame / cGame_readFromReplayFrame call them directly): the field-less states'
// versions only call cGameState_writeToReplayFrame / cGameState_readFromReplayFrame (mNextState).
// EA's source: d:/ssx/code/source/main/gamestate.cpp (preview strings; the retail methods are contiguous with
// cGameState's and cGFTrackStartState's: units fe/festateloadscreen 0x00233B60-0x00234A30 and main/gamestate
// 0x00234AD0-0x00237038, plus the ctors / dtors in main/ssxappdtor 0x00243CF0-0x00244220).
// Evidence tags as above: [R] retail ctors / dtors / cGame_cGame placement, [P] the July 2003 preview (its cGameState
// is 0xC: two data words, vptr 0x8, so derived fields sit 4 lower), [U] retail use, [O] On Tour's DWARF.

// cGFEventStartState (kGFSM_EventStart, 1): starts an event. gainFocus re-picks the computer riders' characters and
// queues the start sequence for its startup flag; update readies the event's opening NIS lists (ReadyEvent), sets up
// and loads the AI riders (or purges the computer riders and actors for the modes that have none), waits for the NIS
// lists and moves on (mNextState, default kGFSM_PreEvent).
// Size 0x20: [R] cGame + 0xB0 .. 0xD0 (cGFPreEventState); the ctor (fe/festateloadscreen 0x00234188) writes to 0x1C.
//   [O] On Tour's class is 0x18: bools mPurgeCPURiders 0x10, mHasTransitionNIS 0x11, mPauseOnLoad 0x12, then
//   mSubState 0x14; retail's bools are words, which puts mSubState at 0x1C (the ctor's 4 = Complete, update's switch).
// Vtable cGFEventStartState_vtbl (0x0047D530): 1 dtor (0x00243E20), 5 gainFocus, 6 update, 7 loseFocus.
struct cGFEventStartState {
    // --- cGameState (inline in the ctor: mNextState = 0, vptr = cGameState_vtbl, cGameState::mpGame = 0)
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 (cGameState) then 2 (kGFSM_PreEvent); [U] update
                                    //      goes on with it (cGameState_gotoNextState); in the replay frame
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; [R] ctor 0; [U] written to / read from the replay frame
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFEventStartState_vtbl (ctor); the dtor
                                    //      cGFEventStartState__cGFEventStartState stores cGameState_vtbl
    // --- cGFEventStartState
    int mPurgeCPURiders;            // 0x10 [O] bool; [R] ctor 0; [U] update sets it after cAI_prepareToPurgeComputer-
                                    //      Riders / -Actors (game modes 4-6) and clears it once the riders and actors
                                    //      are cleaned up; in the replay frame
    int mHasTransitionNIS;          // 0x14 [O] bool; [R] ctor 0; [U] ReadyEvent sets it when ScriptEngine's
                                    //      mPlayingNISList is 1, update plays NIS list 0 with (it == 0), loseFocus
                                    //      clears it; in the replay frame
    int mPauseOnLoad;               // 0x18 [O] bool (On Tour 0x12; placed by On Tour's order and the 0x20 size); no
                                    //      retail access (not set by the ctor, not in the replay frame)
    int mSubState;                  // 0x1C [O] eEventStartSubState (0 WaitNISTransition, 1 SetupEvent, 2 WaitLoading,
                                    //      3 WaitNIS, 4 Complete); [R] ctor 4; [U] gainFocus 0, update's switch; in
                                    //      the replay frame

    // EA's eEventStartSubState (mSubState). [O] On Tour's enum cGFEventStartState::eEventStartSubState, names and
    // values; [U] update's switch runs them in this order and the ctor starts at keEventStartSubState_Complete.
    enum eEventStartSubState {
        keEventStartSubState_WaitNISTransition = 0, keEventStartSubState_SetupEvent = 1,
        keEventStartSubState_WaitLoading = 2, keEventStartSubState_WaitNIS = 3, keEventStartSubState_Complete = 4
    };

    // Methods bound to their retail symbols by asm labels.
    void gainFocus(int mode) __asm__("cGFEventStartState_gainFocus");
    void update() __asm__("cGFEventStartState_update");
    void loseFocus() __asm__("cGFEventStartState_loseFocus");
    void ReadyEvent() __asm__("cGFEventStartState_ReadyEvent");
};

CHECK_OFFSET(cGFEventStartState, mNextState, 0x00);
CHECK_OFFSET(cGFEventStartState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFEventStartState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFEventStartState, mVtbl, 0x0C);
CHECK_OFFSET(cGFEventStartState, mPurgeCPURiders, 0x10);
CHECK_OFFSET(cGFEventStartState, mHasTransitionNIS, 0x14);
CHECK_OFFSET(cGFEventStartState, mPauseOnLoad, 0x18);
CHECK_OFFSET(cGFEventStartState, mSubState, 0x1C);
CHECK_SIZE(cGFEventStartState, 0x20);

// cGFPreEventState (kGFSM_PreEvent, 2): opens the pre-event overlay (cOVStateManager_gotoOverlay(2)) under pause state
// 1 and waits until the overlay sets mOverlayDone (unless the online session has failed); loseFocus pops the pause
// state and resets the game state (ResetGameState: NIS list, AI, audio).
// Size 0x10, no fields of its own: [R] cGame + 0xD0 .. 0xE0 (cGFGateState); [O] 0x10.
// Ctor cGFPreEventState_cGFPreEventState (main/ssxappdtor 0x002440C0; mNextState 3 = kGFSM_Gate). Vtable
// cGFPreEventState_vtbl (0x0047D270): 1 dtor (0x00244090), 5 gainFocus, 6 update, 7 loseFocus.
struct cGFPreEventState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 3 (kGFSM_Gate); [U] update goes on with it
                                    //      (cGameState_gotoNextState)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; [U] gainFocus clears it, update waits for it to be 1 (the
                                    //      overlay sets Game->mCurrentState's word 0x8)
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFPreEventState_vtbl (ctor); cGameState_vtbl
                                    //      (dtor)

    // Methods bound to their retail symbols by asm labels.
    void gainFocus() __asm__("cGFPreEventState_gainFocus");
    void update() __asm__("cGFPreEventState_update");
    void loseFocus() __asm__("cGFPreEventState_loseFocus");
    void ResetGameState() __asm__("cGFPreEventState_ResetGameState");
};

CHECK_OFFSET(cGFPreEventState, mNextState, 0x00);
CHECK_OFFSET(cGFPreEventState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFPreEventState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFPreEventState, mVtbl, 0x0C);
CHECK_SIZE(cGFPreEventState, 0x10);

// cGFGateState (kGFSM_Gate, 3): the start-gate countdown. gainFocus resets the replay for the race, puts the AI in
// state 4 and runs the location's "StartlightBegin" world script (runScript); update announces each second and at
// zero runs "StartgateOpen" and moves on. Game modes 4-6 skip the gate (gotoNextState at once).
// Size 0x10, no fields of its own: [R] cGame + 0xE0 .. 0xF0 (cGFRidingState); [O] 0x10.
// Ctor cGFGateState_cGFGateState (main/ssxappdtor 0x00243E80; mNextState 4 = kGFSM_Riding). Vtable cGFGateState_vtbl
// (0x0047D4D8): 1 dtor (0x00243E50), 5 gainFocus, 6 update; runScript is non-virtual.
struct cGFGateState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 4 (kGFSM_Riding); [U] gainFocus / update go
                                    //      on with it (cGameState_gotoNextState)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFGateState_vtbl (ctor); cGameState_vtbl (dtor)

    // Methods bound to their retail symbols by asm labels.
    void gainFocus() __asm__("cGFGateState_gainFocus");
    void runScript(int hashID) __asm__("cGFGateState_runScript");
    void update() __asm__("cGFGateState_update");
};

CHECK_OFFSET(cGFGateState, mNextState, 0x00);
CHECK_OFFSET(cGFGateState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFGateState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFGateState, mVtbl, 0x0C);
CHECK_SIZE(cGFGateState, 0x10);

// cGFRidingState (kGFSM_Riding, 4): the race itself. gainFocus lets the AI riders go (AI state 5), hands the back end
// its racing state and purges the UI's normal states unless a replay plays; the default update / loseFocus.
// Size 0x10, no fields of its own: [R] cGame + 0xF0 .. 0x100 (cGFFinishlineState); [O] 0x10.
// Ctor cGFRidingState_cGFRidingState (main/ssxappdtor 0x00243EE8; mNextState 5 = kGFSM_Finishline). Vtable
// cGFRidingState_vtbl (0x0047D480): 1 dtor (0x00243EB8), 5 gainFocus.
struct cGFRidingState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 5 (kGFSM_Finishline)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFRidingState_vtbl (ctor); cGameState_vtbl (dtor)
};

CHECK_OFFSET(cGFRidingState, mNextState, 0x00);
CHECK_OFFSET(cGFRidingState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFRidingState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFRidingState, mVtbl, 0x0C);
CHECK_SIZE(cGFRidingState, 0x10);

// cGFFinishlineState (kGFSM_Finishline, 5): after the finish line. gainFocus ends the replay recording, clears the UI
// states and the NIS list and starts a 180-frame delay; update counts it down, shows the finish overlay and plays the
// end-of-event sequence; loseFocus switches the cameras to the post-race view and plays NIS list 0.
// Size 0x18: [R] cGame + 0x100 .. 0x118 (cGFEndState); the ctor (fe/festateloadscreen 0x00233C10) writes up to 0x14.
//   [O] 0x18, same offsets.
// Vtable cGFFinishlineState_vtbl (0x0047D5E0): 1 dtor (0x00243D88), 5 gainFocus, 6 update, 7 loseFocus.
struct cBxReplayFrame;

struct cGFFinishlineState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 7 (kGFSM_PostEvent); [U] update stores 12
                                    //      (kGFSM_Podium) on one path and goes on (cGameState_gotoNextState),
                                    //      loseFocus stores 7; in the replay frame
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFFinishlineState_vtbl (ctor); cGameState_vtbl
                                    //      (dtor)
    // --- cGFFinishlineState
    int mSubState;                  // 0x10 [O]; [R] ctor 2; [U] gainFocus 0, update's switch (sets 1 and 2),
                                    //      loseFocus 2; in the replay frame
    int mShowEventResultsDelay;     // 0x14 [O]; [R] ctor 0; [U] gainFocus 180, update counts it down; in the replay
                                    //      frame

    // Methods bound to their retail symbols by asm labels.
    void gainFocus() __asm__("cGFFinishlineState_gainFocus");
    void loseFocus() __asm__("cGFFinishlineState_loseFocus");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cGFFinishlineState_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cGFFinishlineState_readFromReplayFrame");
};

CHECK_OFFSET(cGFFinishlineState, mNextState, 0x00);
CHECK_OFFSET(cGFFinishlineState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFFinishlineState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFFinishlineState, mVtbl, 0x0C);
CHECK_OFFSET(cGFFinishlineState, mSubState, 0x10);
CHECK_OFFSET(cGFFinishlineState, mShowEventResultsDelay, 0x14);
CHECK_SIZE(cGFFinishlineState, 0x18);

// cGFEndState (kGFSM_End, 6): gainFocus asks the game for kGFSM_TrackStart (cGame_setState(mpGame, 10, 0, 0)); its
// loseFocus is empty.
// Size 0x10, no fields of its own: [R] cGame + 0x118 .. 0x128 (cGFPostEventState); [O] 0x10.
// Ctor cGFEndState_cGFEndState (main/ssxappdtor 0x00243D20; mNextState 8 = kGFSM_EndWait). Vtable cGFEndState_vtbl
// (0x0047D690): 1 dtor (0x00243CF0), 5 gainFocus, 7 loseFocus__FPv.
struct cGFEndState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 8 (kGFSM_EndWait)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFEndState_vtbl (ctor); cGameState_vtbl (dtor)
};

CHECK_OFFSET(cGFEndState, mNextState, 0x00);
CHECK_OFFSET(cGFEndState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFEndState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFEndState, mVtbl, 0x0C);
CHECK_SIZE(cGFEndState, 0x10);

// cGFPostEventState (kGFSM_PostEvent, 7): gainFocus resets the riders' first-time flags when cGamePath is 0, clears
// the NIS list and mOverlayDone and opens overlay 7; update waits for the overlay to set mOverlayDone, then asks the
// game for kGFSM_UserSelect with startup flag 2 (cGame_setState(Game, 14, 2, 0)); loseFocus purges the UI's normal
// states.
// Size 0x10, no fields of its own: [R] cGame + 0x128 .. 0x138 (cGFEndWaitState); [O] 0x10.
// Ctor cGFPostEventState_cGFPostEventState (main/ssxappdtor 0x00244128; mNextState 8 = kGFSM_EndWait). Vtable
// cGFPostEventState_vtbl (0x0047D218): 1 dtor (0x002440F8), 5 gainFocus, 6 update, 7 loseFocus.
struct cGFPostEventState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 8 (kGFSM_EndWait)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; [U] gainFocus clears it, update waits for it to be 1
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFPostEventState_vtbl (ctor); cGameState_vtbl
                                    //      (dtor)

    // Methods bound to their retail symbols by asm labels.
    void gainFocus() __asm__("cGFPostEventState_gainFocus");
    void update() __asm__("cGFPostEventState_update");
};

CHECK_OFFSET(cGFPostEventState, mNextState, 0x00);
CHECK_OFFSET(cGFPostEventState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFPostEventState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFPostEventState, mVtbl, 0x0C);
CHECK_SIZE(cGFPostEventState, 0x10);

// cGFEndWaitState (kGFSM_EndWait, 8): overrides nothing but the destructor (On Tour: only the replay methods).
// Size 0x10, no fields of its own: [R] cGame + 0x138 .. 0x148 (cGFTransportWaitState); [O] 0x10.
// No out-of-line ctor: cGame_cGame builds it inline (mNextState 11 = kGFSM_SectionLoad, mStartupFlag 0, vtable
// cGFEndWaitState_vtbl). Its vtable, cGFEndWaitState_vtbl (9 slots: 1 cGFEndWaitState__cGFEndWaitState = the
// destructor (main/ssxappdtor), 2-9 cGameState_*__FPv), was unnamed (D_0047D638) until 2026-11-20;
// cGame_updateState / cGame_readFromReplayFrame map id 8 to cGame + 0x138.
struct cGFEndWaitState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] cGame_cGame 0 then 11 (kGFSM_SectionLoad)
    int mStartupFlag;               // 0x04 [O]; [R] cGame_cGame 0
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFEndWaitState_vtbl (cGame_cGame);
                                    //      cGameState_vtbl (cGFEndWaitState__cGFEndWaitState, the dtor)
};

CHECK_OFFSET(cGFEndWaitState, mNextState, 0x00);
CHECK_OFFSET(cGFEndWaitState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFEndWaitState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFEndWaitState, mVtbl, 0x0C);
CHECK_SIZE(cGFEndWaitState, 0x10);

// cGFTransportWaitState (kGFSM_TransportWait, 9): waits for a transport; loseFocus calls the AI's virtual slot 5
// with 3.
// Size 0x10, no fields of its own: [R] cGame + 0x148 .. 0x158 (cGFTrackStartState); [O] 0x10.
// Ctor cGFTransportWaitState_cGFTransportWaitState (main/ssxappdtor 0x00243DE8; mNextState 10 = kGFSM_TrackStart).
// Vtable cGFTransportWaitState_vtbl (0x0047D588): 1 dtor (0x00243DB8), 7 loseFocus.
struct cGFTransportWaitState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 10 (kGFSM_TrackStart)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFTransportWaitState_vtbl (ctor);
                                    //      cGameState_vtbl (dtor)
};

CHECK_OFFSET(cGFTransportWaitState, mNextState, 0x00);
CHECK_OFFSET(cGFTransportWaitState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFTransportWaitState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFTransportWaitState, mVtbl, 0x0C);
CHECK_SIZE(cGFTransportWaitState, 0x10);

// cGFSectionLoadState (kGFSM_SectionLoad, 11): loads the next location's world section during a transport. gainFocus
// resets the back end state and the replay (and the riders' first-time flags when cGamePath is 0); update waits for
// the section manager's transport, focuses the world view on the first rider's NIS end position, then ends the
// transport sequence and activates the location.
// Size 0x14: [R] cGame + 0x178 .. 0x18C (cGFPodiumState); [O] 0x14, same offsets.
// Ctor cGFSectionLoadState_cGFSectionLoadState (main/gamestate 0x00236868; mNextState 10 = kGFSM_TrackStart; it does
// not set mSubState). Vtable cGFSectionLoadState_vtbl (0x0047D2C8): 1 dtor (0x00244060), 5 gainFocus, 6 update,
// 7 loseFocus__FPv (empty).
struct cGFSectionLoadState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 10 (kGFSM_TrackStart)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFSectionLoadState_vtbl (ctor); cGameState_vtbl
                                    //      (dtor)
    // --- cGFSectionLoadState
    int mSubState;                  // 0x10 [O]; [U] gainFocus 0, update's switch (0 wait for the section, 1 wait for
                                    //      the focus / transition, 2 done); not in the replay frame
    // mSubState's steps (names ours). [U] update waits in kSectionLoad_WaitSection until the location's section is
    // loaded, in kSectionLoad_WaitTransport until the stream focus is far enough and no transition or NIS runs, then
    // ends the transport and stays in kSectionLoad_Done; gainFocus starts it at kSectionLoad_WaitSection.
    enum eSubState { kSectionLoad_WaitSection = 0, kSectionLoad_WaitTransport = 1, kSectionLoad_Done = 2 };

    // Methods bound to their retail symbols by asm labels.
    void gainFocus() __asm__("cGFSectionLoadState_gainFocus");
    void update() __asm__("cGFSectionLoadState_update");
};

CHECK_OFFSET(cGFSectionLoadState, mNextState, 0x00);
CHECK_OFFSET(cGFSectionLoadState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFSectionLoadState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFSectionLoadState, mVtbl, 0x0C);
CHECK_OFFSET(cGFSectionLoadState, mSubState, 0x10);
CHECK_SIZE(cGFSectionLoadState, 0x14);

// cGFPodiumState (kGFSM_Podium, 12): gainFocus turns the AI's computer actors on, update waits for NIS list 0 to end
// and moves on, loseFocus turns the actors off.
// Size 0x10, no fields of its own: [R] cGame + 0x18C .. 0x19C (cGFNextHeatState); [O] 0x10.
// Ctor cGFPodiumState_cGFPodiumState (main/ssxappdtor 0x00244190; mNextState 7 = kGFSM_PostEvent). Vtable
// cGFPodiumState_vtbl (0x0047D1C0): 1 dtor (0x00244160), 5 gainFocus, 6 update, 7 loseFocus.
struct cGFPodiumState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 7 (kGFSM_PostEvent); [U] update goes on with
                                    //      it (cGameState_gotoNextState)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFPodiumState_vtbl (ctor); cGameState_vtbl (dtor)

    // Methods bound to their retail symbols by asm labels.
    void update() __asm__("cGFPodiumState_update");
};

CHECK_OFFSET(cGFPodiumState, mNextState, 0x00);
CHECK_OFFSET(cGFPodiumState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFPodiumState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFPodiumState, mVtbl, 0x0C);
CHECK_SIZE(cGFPodiumState, 0x10);

// cGFNextHeatState (kGFSM_NextHeat, 13): sets up the next heat of an event. gainFocus restarts the heat and the game,
// re-picks the computer riders and queues the start sequence (plays the transport sequence in race mode); update
// purges and reloads the AI riders, focuses the world view on the first rider, waits for the transport and moves on.
// Size 0x1C: [R] cGame + 0x19C .. 0x1B8 (cGFUserSelectState); the ctor (main/ssxappdtor 0x00243F80) writes up to 0x18.
//   [O] 0x1C, same offsets.
// Vtable cGFNextHeatState_vtbl (0x0047D3D0): 1 dtor (0x00243F50), 5 gainFocus, 6 update, 7 loseFocus.
struct cGFNextHeatState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 1 (kGFSM_EventStart); [U] update goes on
                                    //      with it (cGameState_gotoNextState); in the replay frame
    int mStartupFlag;               // 0x04 [O]; [R] ctor 3 (the only state whose ctor does not store 0; passed on to
                                    //      the next state by gotoNextState)
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFNextHeatState_vtbl (ctor); cGameState_vtbl
                                    //      (dtor)
    // --- cGFNextHeatState (On Tour's names have no m prefix)
    // CurrentNextHeatState's steps. [O] On Tour's enum cGFNextHeatState::eNextHeatStatus for 0 .. 2, names and values;
    // retail's update also has a fourth step, 3 (NH_DONE, name ours), that goes on to the next game state.
    enum eNextHeatStatus { NH_PURGING = 0, NH_LOADINGAI = 1, NH_READY = 2, NH_DONE = 3 };
    int CurrentNextHeatState;       // 0x10 [O] eNextHeatStatus (0 NH_PURGING, 1 NH_LOADINGAI, 2 NH_READY; retail also
                                    //      uses 3 = go on); [U] gainFocus 0 (3 when it skips: modes 5 / 6, game
                                    //      path), update's switch; in the replay frame; not set by the ctor
    int WaitTicks;                  // 0x14 [O]; [R] ctor 0; [U] gainFocus 0, update counts it and waits for 120; in
                                    //      the replay frame
    int PlayTransport;              // 0x18 [O] bool; [R] ctor 0; [U] gainFocus sets it (game mode 0, with
                                    //      PlayTransportSequence), update waits for the transport only when set; in the
                                    //      replay frame

    // Methods bound to their retail symbols by asm labels.
    void gainFocus() __asm__("cGFNextHeatState_gainFocus");
    void update() __asm__("cGFNextHeatState_update");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cGFNextHeatState_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cGFNextHeatState_readFromReplayFrame");
};

CHECK_OFFSET(cGFNextHeatState, mNextState, 0x00);
CHECK_OFFSET(cGFNextHeatState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFNextHeatState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFNextHeatState, mVtbl, 0x0C);
CHECK_OFFSET(cGFNextHeatState, CurrentNextHeatState, 0x10);
CHECK_OFFSET(cGFNextHeatState, WaitTicks, 0x14);
CHECK_OFFSET(cGFNextHeatState, PlayTransport, 0x18);
CHECK_SIZE(cGFNextHeatState, 0x1C);

// cGFUserSelectState (kGFSM_UserSelect, 14): the user picks where to go next (lodge session menu or transport map).
// gainFocus plays the NIS for its mode (the startup flag), update opens the session overlay or the map, then either
// teleports (same location: cGame_setState(Game, kGFSM_SessionTeleport)) or plays the transport sequence and loads the
// chosen location.
// Size 0x24: [R] cGame + 0x1B8 .. 0x1DC (cGFSessionTeleportState); the methods reach 0x1C.
//   [O] On Tour's class is 0x20: mCurrState 0x10, mMode 0x14, mNextLocation 0x18, bools mWaitingForOverlay 0x1C and
//   mShowLoadMessage 0x1D; retail's bools are words (0x1C, 0x20).
//   [P] the preview's setNextLocation stores preview 0x14 (its cGameState is 4 smaller): retail 0x18.
// Ctor cGFUserSelectState_cGFUserSelectState (main/gamestate 0x00236208). Vtable cGFUserSelectState_vtbl
// (0x0047D320): 1 dtor (0x00244028), 5 gainFocus, 6 update; setNextLocation (main/ssxappdtor 0x00244058, no callers)
// is non-virtual.
struct cGFUserSelectState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 4 (kGFSM_Riding); [U] update goes on with it
                                    //      (cGameState_gotoNextState, lodge mode)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; [R] ctor 0; [U] gainFocus clears it (sets it for mode 1),
                                    //      update clears / tests / sets it; in the replay frame
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFUserSelectState_vtbl (ctor); cGameState_vtbl
                                    //      (dtor)
    // --- cGFUserSelectState
    int mCurrState;                 // 0x10 [O] eUserSelectStatus (0 US_WAITSTARTNIS, 1 US_WAITOVERLAY, 2 US_WAITLEAVE,
                                    //      3 US_WAITEND); [U] gainFocus 0, update's switch; in the replay frame
    int mMode;                      // 0x14 [O] eUserSelectMode (0 Lodge, 1 TransportPauseMenu, 2 Transport,
                                    //      3 TransportNoTransitionIn); [R] ctor 0; [U] gainFocus stores its startup
                                    //      flag, update tests it; in the replay frame
    // mCurrState and mMode's values. [O] On Tour's enums cGFUserSelectState::eUserSelectStatus (US_*) and
    // eUserSelectMode (keUserSelectMode_*), names and values; [U] update's switch walks US_WAITSTARTNIS ..
    // US_WAITEND; gainFocus plays the lodge NIS for keUserSelectMode_Lodge and starts the transport for the others.
    enum eUserSelectStatus { US_WAITSTARTNIS = 0, US_WAITOVERLAY = 1, US_WAITLEAVE = 2, US_WAITEND = 3 };
    enum eUserSelectMode {
        keUserSelectMode_Lodge = 0, keUserSelectMode_TransportPauseMenu = 1, keUserSelectMode_Transport = 2,
        keUserSelectMode_TransportNoTransitionIn = 3, keUserSelectMode_Num = 4
    };
    int mNextLocation;              // 0x18 [O][P] (preview 0x14, setNextLocation); [R] ctor 23; [U] setNextLocation
                                    //      stores it, gainFocus 23 (mode 1: cBENewRaceInterface_mNextLocation), update
                                    //      compares it with the current location and loads it; in the replay frame
    int mWaitingForOverlay;         // 0x1C [O] bool; [U] gainFocus 0; update (US_WAITLEAVE) sets it with mOverlayDone
                                    //      and clears it before the load
    int mShowLoadMessage;           // 0x20 [O] bool (On Tour 0x1D; placed by On Tour's order and the 0x24 size); no
                                    //      retail access

    // Methods bound to their retail symbols by asm labels.
    void gainFocus(int mode) __asm__("cGFUserSelectState_gainFocus");
    void update() __asm__("cGFUserSelectState_update");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cGFUserSelectState_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cGFUserSelectState_readFromReplayFrame");
    void setNextLocation(int location) __asm__("cGFUserSelectState_setNextLocation");
};

CHECK_OFFSET(cGFUserSelectState, mNextState, 0x00);
CHECK_OFFSET(cGFUserSelectState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFUserSelectState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFUserSelectState, mVtbl, 0x0C);
CHECK_OFFSET(cGFUserSelectState, mCurrState, 0x10);
CHECK_OFFSET(cGFUserSelectState, mMode, 0x14);
CHECK_OFFSET(cGFUserSelectState, mNextLocation, 0x18);
CHECK_OFFSET(cGFUserSelectState, mWaitingForOverlay, 0x1C);
CHECK_OFFSET(cGFUserSelectState, mShowLoadMessage, 0x20);
CHECK_SIZE(cGFUserSelectState, 0x24);

// cGFSessionTeleportState (kGFSM_SessionTeleport, 15): teleports the rider for a new session (free-ride game mode 12,
// partial restart at the mission start point, a white fade); update asks the game for kGFSM_EventStart
// (cGame_setState(Game, 1, 0, 0)).
// Size 0x10, no fields of its own: [R] cGame + 0x1DC .. 0x1EC (cGFReplayStartState); [O] 0x10.
// Ctor cGFSessionTeleportState_cGFSessionTeleportState (main/ssxappdtor 0x00243FF0; mNextState 10 =
// kGFSM_TrackStart). Vtable cGFSessionTeleportState_vtbl (0x0047D378): 1 dtor (0x00243FC0), 5 gainFocus, 6 update.
struct cGFSessionTeleportState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 10 (kGFSM_TrackStart)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFSessionTeleportState_vtbl (ctor);
                                    //      cGameState_vtbl (dtor)

    // Methods bound to their retail symbols by asm labels.
    void gainFocus() __asm__("cGFSessionTeleportState_gainFocus");
};

CHECK_OFFSET(cGFSessionTeleportState, mNextState, 0x00);
CHECK_OFFSET(cGFSessionTeleportState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFSessionTeleportState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFSessionTeleportState, mVtbl, 0x0C);
CHECK_SIZE(cGFSessionTeleportState, 0x10);

// cGFReplayStartState (kGFSM_ReplayStart, 16): starts a replay (missions, global scripts, the replay overlay 0x11);
// loseFocus purges and sets up the missions again; update is empty.
// Size 0x10, no fields of its own: [R] cGame + 0x1EC .. 0x1FC (cGame::mPrevState, main/game.h); [O] 0x10.
// Ctor cGFReplayStartState_cGFReplayStartState (main/ssxappdtor 0x002441F8; mNextState 4 = kGFSM_Riding). Vtable
// cGFReplayStartState_vtbl (0x0047D168): 1 dtor (0x002441C8), 5 gainFocus, 6 update__FPv, 7 loseFocus. Retail has no
// replay-frame methods for it (cGame_writeToReplayFrame / cGame_readFromReplayFrame skip it).
struct cGFReplayStartState {
    // --- cGameState
    int mNextState;                 // 0x00 [O] eGFState; [R] ctor 0 then 4 (kGFSM_Riding)
    int mStartupFlag;               // 0x04 [O]; [R] ctor 0
    int mOverlayDone;               // 0x08 [O] bool; no access in this class's methods
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl, then cGFReplayStartState_vtbl (ctor); cGameState_vtbl
                                    //      (dtor)
};

CHECK_OFFSET(cGFReplayStartState, mNextState, 0x00);
CHECK_OFFSET(cGFReplayStartState, mStartupFlag, 0x04);
CHECK_OFFSET(cGFReplayStartState, mOverlayDone, 0x08);
CHECK_OFFSET(cGFReplayStartState, mVtbl, 0x0C);
CHECK_SIZE(cGFReplayStartState, 0x10);

#endif /* MAIN_GAMESTATE_H */
