#ifndef FE_OVSTATE_H
#define FE_OVSTATE_H

// cOVState: the base of the in-game overlay states (pause menu, map, replay, autosave, big-challenge screens ...;
// cOVState_MAP, cOVState_REPLAY, ... derive from it). It is a cUIState with a few flags, a chat buffer and its
// overlay id. Retail vtable _vt$8cOVState = cOVState_vtbl (slot 4 cOVState_onCreateScreen, slot 5
// cOVState_onDestroyScreen); its constructor cOVState(cUIEngine*, tOVStateEnum) is cOVState_cOVState.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Subclasses' own
// fields start at 0x9C (e.g. the mission id at 0x9C in ovstateenterlodge.cpp), so this header covers the base only:
// retype a subclass method (cOVState_MAP_*, cOVState_REPLAY_*, ...) only with that subclass's own header.
// Evidence tags:
//   [R] retail's constructors: cUIState(cUIEngine*) = cUIState_cUIState / cUIState(cUIEngine*, cUIState*) =
//       cUIState_cUIState1 (ui/uiscreen.cpp) and cOVState(cUIEngine*, tOVStateEnum) = cOVState_cOVState;
//   [P] the July 2003 preview's cOVState::allowPauseMenu (lw 0x48), carried to retail 0x4C by tools/layout_xlate.py;
//   [U] how matched retail code uses the field;
//   [O] SSX On Tour's DWARF (class cListNode / cUIState / cOVState: same member order; On Tour's bools are
//       1 byte, retail's are words), used only where the fields on both sides are placed by other evidence.

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

// An overlay id (cOVState::mOVID, cOVStateManager_triggerOverlay's argument). Names [O] from On Tour's enum
// tOVStateEnum where the class matches; values [R] from retail's cOVStateManager_triggerOverlay, which builds for each
// id the class named here (its vtable cOVState_<NAME>_vtbl; retail has more overlays than On Tour, so the values
// differ).
// Names ours for the ids whose class has no name, after what [U] its methods show: 9 (onCreateScreen cOVState_FREESTYLE_PREEVENT_onCreateScreen
// adds "41freestyle_pre"), 14 (a bare cOVState: its vtable D_00474B80 overrides nothing), 20 (the rival / peak
// challenge pre-event: cOVState_CHALLENGE_PREEVENT_onGainTransition; cOVStateManager_gotoOverlay picks it for events kGAMEEVENT_RIVALTIME ..
// kGAMEEVENT_POINTS_PEAK3), 21 (cOVState_MULTI_PREEVENT_onCreateScreen adds "50multifs_pre"; picked on the multiplayer path), 22 (its
// onCreateScreen cOVState_PEAKCHAL_RESULTS_onCreateScreen adds "70peakchal_results"; cOVStateManager_getResultsOverlay picks it for the peak
// challenges) and 40 (cOVState_PRIVATEMESSAGE_onCreateScreen adds "282ReadPrivateMessage").
enum tOVStateEnum {
    kOVSTATE_NULL = 0, kOVSTATE_PAUSE_COMPETE = 1, kOVSTATE_PAUSE_BIGCHAL = 2, kOVSTATE_PAUSE_FREERIDE = 3,
    kOVSTATE_PAUSE_SPOKE = 4, kOVSTATE_PAUSE_QUICKPLAY = 5, kOVSTATE_PAUSE_ONLINE = 6, kOVSTATE_PAUSE_OPTIONS = 7,
    kOVSTATE_HEAT_PREEVENT = 8, kOVSTATE_FREESTYLE_PREEVENT = 9, kOVSTATE_HEAT_STANDINGS = 10,
    kOVSTATE_FINAL_STANDINGS = 11, kOVSTATE_RACE_FINISHOV = 12, kOVSTATE_BASSSP_QUAL_FINISHOV = 13,
    kOVSTATE_EMPTY = 14, kOVSTATE_TOPTIMES = 15, kOVSTATE_REWARDS = 16, kOVSTATE_REPLAY = 17,
    kOVSTATE_SAVEREPLAY = 18, kOVSTATE_SAVEOPTIONS = 19, kOVSTATE_CHALLENGE_PREEVENT = 20,
    kOVSTATE_MULTI_PREEVENT = 21, kOVSTATE_PEAKCHAL_RESULTS = 22, kOVSTATE_RESTART = 23, kOVSTATE_QUIT = 24,
    kOVSTATE_RESTARTCHALLENGE = 25, kOVSTATE_QUITCHALLENGE = 26, kOVSTATE_QUITCOMPETITION = 27,
    kOVSTATE_GIVEUP = 28, kOVSTATE_BIGCHALLENGE_START = 29, kOVSTATE_BIGCHALLENGE_FAIL = 30,
    kOVSTATE_ENTERLODGE = 31, kOVSTATE_MAP = 32, kOVSTATE_TRANSPORT = 33, kOVSTATE_MESSAGECENTER = 34,
    kOVSTATE_TOURNAMENT_STANDINGS = 35, kOVSTATE_PROFILE = 36, kOVSTATE_AUTOSAVE = 37, kOVSTATE_SAVEPROMPT = 38,
    kOVSTATE_PAUSE_ONLINE_ERROR = 39, kOVSTATE_PRIVATEMESSAGE = 40
};

struct cUIThing;

struct cOVState : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] _vt$8cOVState cOVState_vtbl, stored by cOVState_cOVState after the base ctor
    // cOVState's notes on cUIState's fields:
    //   mpEngine: [R] the ctor's engine argument; [U] cUIStateStack_findState(mpEngine + 0x18)
    //   mCtrlBits: [R] ctor stores 0xF; [U] masks in cUIState_setCtrlBitsByCtrlIndex / cFEState_setControllerFilter
    //   mpScreen: [R] ctor zeroes it; [U] cUIScreen_getObjectByHashName(mpScreen, ...)
    // --- cOVState
    // cOVState's own slots ([R] cOVState_vtbl; types [O] On Tour's class cOVState):
    virtual bool allowPauseMenu();                              // 25
    int mDoneTransition;        // 0x048 [O] (On Tour: bool at 0x48); [R] cOVState_cOVState zeroes it
    int mAllowPauseMenu;        // 0x04C [P] allowPauseMenu (preview 0x48); [R] cOVState_cOVState zeroes it;
                                //       [U] cOVState_allowPauseMenu returns it
    int mRestoreCutSceneMode;   // 0x050 [O]; [U] cOVState_onCreateScreen sets it when Game->CutSceneTransitionMan's
                                //       mode is 1 or 3 (after cCutSceneTransitionMan_ForceToRegularMode), onDestroyScreen undoes it (cCutSceneTransitionMan_ForceToCutSceneMode)
    int mRemovePDA;             // 0x054 [O]; [R] zeroed by cOVState_cOVState; [U] cOVState_onDestroyScreen calls
                                //       cOVStateManager_removePDATemplate when it is set
    char mszChatBuff[0x40];     // 0x058 [O] (On Tour: char[63]); [R] cOVState_cOVState copies " " into it (sh);
                                //       [U] cOVState_onUpdate_HandleIncomingChat fills it with the incoming chat name and prints it
    int mOVID;                  // 0x098 [O] tOVStateEnum; [R] cOVState_cOVState stores its id argument

    // Methods bound to their retail symbols by asm labels.
    int onUpdate_HandleIncomingChat() __asm__("cOVState_onUpdate_HandleIncomingChat");
    void triggerTextFade(char* objName, char* animName) __asm__("cOVState_triggerTextFade");
    virtual void onCreateScreen() __asm__("cOVState_onCreateScreen");
    virtual void onDestroyScreen() __asm__("cOVState_onDestroyScreen");
    virtual void onUpdate() __asm__("cOVState_onUpdate");
    void MAP_onWidgetCreate() __asm__("cOVState_MAP_onWidgetCreate__FPv");
    int PAUSE_ONLINE_ERROR_onGainTransition() __asm__("cOVState_PAUSE_ONLINE_ERROR_onGainTransition__FPv");
    void* SAVEOPTIONS_onDestroyScreen() __asm__("cOVState_SAVEOPTIONS_onDestroyScreen__FPv");
    int AUTOSAVE_onInputBegin() __asm__("cOVState_AUTOSAVE_onInputBegin__FPv");
    void* ViewPopup_onUpdate() __asm__("cOVState_ViewPopup_onUpdate__FPv");
    void EMPTY() __asm__("cOVState_EMPTY__cOVState_EMPTY");
    void PEAKCHAL_RESULTS() __asm__("cOVState_PEAKCHAL_RESULTS__cOVState_PEAKCHAL_RESULTS");
    void * PEAKCHAL_RESULTS_cOVState_PEAKCHAL_RESULTS(int engine, int overlayId)
        __asm__("cOVState_PEAKCHAL_RESULTS_cOVState_PEAKCHAL_RESULTS");
    void * PRIVATEMESSAGE_cOVState_PRIVATEMESSAGE1() __asm__("cOVState_PRIVATEMESSAGE_cOVState_PRIVATEMESSAGE1");
    void * PRIVATEMESSAGE_cOVState_PRIVATEMESSAGE(int engine, int overlayId)
        __asm__("cOVState_PRIVATEMESSAGE_cOVState_PRIVATEMESSAGE");
    void* PRIVATEMESSAGE() __asm__("cOVState_PRIVATEMESSAGE__cOVState_PRIVATEMESSAGE");
    void* FREESTYLE_PREEVENT() __asm__("cOVState_FREESTYLE_PREEVENT__cOVState_FREESTYLE_PREEVENT");
    void * FREESTYLE_PREEVENT_cOVState_FREESTYLE_PREEVENT(int engine, int overlayId)
        __asm__("cOVState_FREESTYLE_PREEVENT_cOVState_FREESTYLE_PREEVENT");
    void* MULTI_PREEVENT() __asm__("cOVState_MULTI_PREEVENT__cOVState_MULTI_PREEVENT");
    void * MULTI_PREEVENT_cOVState_MULTI_PREEVENT(int engine, int overlayId)
        __asm__("cOVState_MULTI_PREEVENT_cOVState_MULTI_PREEVENT");
    void* CHALLENGE_PREEVENT() __asm__("cOVState_CHALLENGE_PREEVENT__cOVState_CHALLENGE_PREEVENT");
    void * CHALLENGE_PREEVENT_cOVState_CHALLENGE_PREEVENT(int engine, int overlayId)
        __asm__("cOVState_CHALLENGE_PREEVENT_cOVState_CHALLENGE_PREEVENT");
    void PEAKCHAL_RESULTS_onCreateScreen() __asm__("cOVState_PEAKCHAL_RESULTS_onCreateScreen");
    void* PEAKCHAL_RESULTS_onUpdate() __asm__("cOVState_PEAKCHAL_RESULTS_onUpdate__FPv");
    int PEAKCHAL_RESULTS_onInputBegin() __asm__("cOVState_PEAKCHAL_RESULTS_onInputBegin__FPv");
    int PEAKCHAL_RESULTS_onWidgetMenu(int widget, int event) __asm__("cOVState_PEAKCHAL_RESULTS_onWidgetMenu");
    void PEAKCHAL_RESULTS_onWidgetEvent(cUIThing* popup, int event) __asm__("cOVState_PEAKCHAL_RESULTS_onWidgetEvent");
    void PEAKCHAL_RESULTS_onGainFocus() __asm__("cOVState_PEAKCHAL_RESULTS_onGainFocus");
    void FREESTYLE_PREEVENT_onCreateScreen() __asm__("cOVState_FREESTYLE_PREEVENT_onCreateScreen");
    void FREESTYLE_PREEVENT_onGainFocus() __asm__("cOVState_FREESTYLE_PREEVENT_onGainFocus");
    void MULTI_PREEVENT_onCreateScreen() __asm__("cOVState_MULTI_PREEVENT_onCreateScreen");
    int MULTI_PREEVENT_onGainTransition(void* msg) __asm__("cOVState_MULTI_PREEVENT_onGainTransition");
    void MULTI_PREEVENT_onGainFocus() __asm__("cOVState_MULTI_PREEVENT_onGainFocus");
    void MULTI_PREEVENT_onUpdate() __asm__("cOVState_MULTI_PREEVENT_onUpdate");
    void CHALLENGE_PREEVENT_onCreateScreen() __asm__("cOVState_CHALLENGE_PREEVENT_onCreateScreen");
    int CHALLENGE_PREEVENT_onWidgetMenu(int menu, int event) __asm__("cOVState_CHALLENGE_PREEVENT_onWidgetMenu");
    void PRIVATEMESSAGE_onCreateScreen() __asm__("cOVState_PRIVATEMESSAGE_onCreateScreen");
    int PRIVATEMESSAGE_onGainTransition(int ok) __asm__("cOVState_PRIVATEMESSAGE_onGainTransition");
    void PRIVATEMESSAGE_addChatLine(char* name, int ownMessage) __asm__("cOVState_PRIVATEMESSAGE_addChatLine");
    void PRIVATEMESSAGE_onUpdate() __asm__("cOVState_PRIVATEMESSAGE_onUpdate");
    void PRIVATEMESSAGE_onPopupEvent(int popup, int event) __asm__("cOVState_PRIVATEMESSAGE_onPopupEvent");
    void PRIVATEMESSAGE_onWidgetEvent(void* item, unsigned int key) __asm__("cOVState_PRIVATEMESSAGE_onWidgetEvent");
    void PRIVATEMESSAGE_openKeyboard(int textObj, char* staticText, char* inputText, int password, int miscData,
                                     int maxChars)
        __asm__("cOVState_PRIVATEMESSAGE_openKeyboard");
    void PRIVATEMESSAGE_createKeyboard(int textObj, int password, int miscData, int maxChars)
        __asm__("cOVState_PRIVATEMESSAGE_createKeyboard");
    virtual void onRender2D() __asm__("cOVState_onRender2D__FPv");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int Event) __asm__("cOVState_onWidgetEvent");
};

CHECK_OFFSET(cOVState, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState, mOVID, 0x98);
CHECK_SIZE(cOVState, 0x9C);

#endif /* FE_OVSTATE_H */
