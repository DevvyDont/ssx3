#ifndef FE_OVSTATEMANAGER_H
#define FE_OVSTATEMANAGER_H

// cOVStateManager: the in-game overlay manager - opens the pre-event / results / reward / pause overlays
// (triggerOverlay / triggerPopup build each cOVState with operator new(size, "cOVStateOverlay")), steps the game
// flow after an overlay closes (mStatus, read by update once a frame) and keeps the PDA template state.
// A STATIC-ONLY class: [O] On Tour's DWARF has `class cOVStateManager` of size 1 with only static data and static
// methods (z:/main/game/source/fe/all_in_one_fe.cpp); [R] retail has no allocation, no vtable and no `this`: every
// method takes plain values (or nothing) and works on the statics below. So there is no struct to lay out and no
// method to retype; this header only gives the statics and the methods one declaration with retail's types.
// All retail methods are in the decomp's unit fe/ovtemplatedialog.cpp (0x0020A8B0 .. 0x0020D1D8); EA's SSX 3
// source file is not known (the preview's assert paths don't name it).
//
// Statics (retail .sbss, 4-byte words; order differs from On Tour's declaration order):
//   0x004A26FC cOVStateManager_mStatus     tOVManagerStatus [O]; [U] update switches on it (1 gotoNextOverlay, 2 resume
//                                          play, 3 removePDATemplate + purge, 4 cGame_setState(0xE)); each case clears it
//   0x004A2700 cOVStateManager_mMiscData   int [O]; [U] gotoNextOverlay switches on it after a results overlay
//                                          (the results/pause menus store the chosen option)
//   0x004A2704 D_004A2704                  [O] mShowPeakPassOverlay (bool, a word in retail); [U] gotoOverlay(2) clears it
//                                          before testing cBESaveInterface_bDoReplay, as On Tour's gotoOverlay does
//   0x004A2708 cOVStateManager_mIsPauseDialog                  [O] mIsPauseDialog (bool, a word); [U] the pause menu sets it to 1 before opening
//                                          the quit dialog (overlay 23), gotoNextOverlay clears it before reopening it
//                                          (On Tour: same store before triggerOverlay), the quit dialog tests it
//   0x004A270C cOVStateManager_mpPDATemplate cPDATemplate* [O]; [U] addPDATemplate allocates the 0x48-byte
//                                          "PDATemplate" cUIState (vtable cPDATemplate_vtbl) and pushes it once;
//                                          removePDATemplate unlinks and deletes it, then clears the pointer
// Every unit declares these with the types used below (14x int mStatus, 8x int mMiscData, 3x void* mpPDATemplate,
// int D_004A2704 / cOVStateManager_mIsPauseDialog), so the declarations here are compatible with all of them.

// [O] On Tour enum tOVManagerStatus (values match retail cOVStateManager_update's cases).
enum tOVManagerStatus {
    kOVSTATUS_IDLE = 0,
    kOVSTATUS_GOTONEXT = 1,         // update: cOVStateManager_gotoNextOverlay
    kOVSTATUS_EXIT = 2,             // update: stop the auto replay, let the current game state continue
    kOVSTATUS_PURGENORMAL = 3,      // update: removePDATemplate + cUIStateStack_purgeNormalStates
    kOVSTATUS_GOTOMAP = 4           // update: cGame_setState(game, 0xE, 2, 0)
};

extern int cOVStateManager_mStatus;          // tOVManagerStatus (declared int everywhere)
extern int cOVStateManager_mMiscData;
extern void* cOVStateManager_mpPDATemplate;  // cPDATemplate*

// Static methods ([O] On Tour names; all `static` in the class). Retail addresses:
//   0x0020A8B0 getResultsOverlay(int currentGameEvent, int currentHeat) -> tOVStateEnum
//   0x0020A8F8 gotoOverlay(eGFState)       0x0020AB50 triggerOverlay(tOVStateEnum)   0x0020BAB8 triggerPopup(tOVStateEnum)
//   0x0020CA10 cOVStateManager_triggerPauseMenu = [O] triggerPauseMenu() (picks the pause overlay 1..5; called by cGame_update / updateUI)
//   0x0020CB58 cOVStateManager_replayOverlayActive = [O] replayOverlayActive() ("64replay" state on the stack)
//   0x0020CBA0 pauseOverlayActive()        0x0020CBE8 allowPauseMenu()               0x0020CC60 triggerFinishOverlay()
//   0x0020CCF8 gotoNextOverlay()           0x0020CFD0 update()                       0x0020D110 addPDATemplate()
//   0x0020D190 removePDATemplate()
extern "C" int cOVStateManager_getResultsOverlay(int currentGameEvent, int currentHeat);
extern "C" void cOVStateManager_gotoOverlay(int mode);
extern "C" void cOVStateManager_triggerOverlay(int state);
extern "C" void cOVStateManager_triggerPopup(int state);
extern "C" int cOVStateManager_pauseOverlayActive(void);
extern "C" int cOVStateManager_allowPauseMenu(void);
extern "C" void cOVStateManager_triggerFinishOverlay(void);
extern "C" void cOVStateManager_gotoNextOverlay(void);
extern "C" void cOVStateManager_update(void);
extern "C" void cOVStateManager_addPDATemplate(void);
extern "C" void cOVStateManager_removePDATemplate(void);

#endif /* FE_OVSTATEMANAGER_H */
