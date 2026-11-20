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

struct cGameState {
    int mNextState;                 // 0x00 [O] eGFState; [U] gotoNextState passes it to cGame_setState (with
                                    //      mStartupFlag) when non-zero; writeToReplayFrame / readFromReplayFrame copy
                                    //      its 4 bytes; derived gainFocus / update store it
    int mStartupFlag;               // 0x04 [O]; [U] gotoNextState passes it; derived ctors zero it
    int mOverlayDone;               // 0x08 [O] bool; [U] derived states: cGFPreEventState / cGFPostEventState /
                                    //      cGFUserSelectState gainFocus clear it, their update tests it
    void** mVtbl;                   // 0x0C [R] cGameState_vtbl (inline ctor in cGame_cGame; the dtor
                                    //      cGameState__cGameState); [P] (preview 0x8)
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
    int mPlayFirstTimeBCVideo;      // 0x14 [O] bool; [R] ctor 0; [U] preloadInit / loseFocus set it, gainFocus and
                                    //      preloadUpdate test it; in the replay frame
    int mPreLoadTicks;              // 0x18 [O]; [R] ctor 0; [U] preloadInit 0, preloadUpdate counts it; in the replay
                                    //      frame
    int mCheckTransportEnd;         // 0x1C [O] bool; [R] ctor 0; [U] gainFocus sets it, update tests and clears it
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

#endif /* MAIN_GAMESTATE_H */
