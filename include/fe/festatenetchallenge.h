#ifndef FE_FESTATENETCHALLENGE_H
#define FE_FESTATENETCHALLENGE_H

// The online challenge popups (EA's d:/ssx/code/source/fe/festatenetchallenge.cpp: the preview's assert sits in
// cFEStateChallengePopup's onWidgetEvent, retail cFEStateChallengePopup_onWidgetEvent; cFEStateChallengeReceivedPopup's
// code follows it in
// both builds, so it is placed here too). Both are cFEStateNet states (fe/festatenet.h, 0x6D0) pushed by the
// cFEStateNet method cFEStateNet_challengePlayer (a challenge from / to LobbyMan's mOpponent):
//
// cFEStateChallengePopup: screen "240challenge_popup": sets up the race options of a challenge (ranked game, CPU
// riders, power-ups, multipliers, point icons, event) and sends it. EA's name: the preview's
// __22cFEStateChallengePopup,
// _$_22cFEStateChallengePopup and onLoseTransition__22cFEStateChallengePopup. Size 0x6F0: cMemMan_alloc(0x6F0,
// "cFEStateChallengePopup") + cFEStateChallengePopup_cFEStateChallengePopup(mem, pEngine, pParent) (cFEStateNet_challengePlayer, the
// only site). Constructors
// cFEStateChallengePopup_cFEStateChallengePopup (cFEStateNet(pEngine, pParent), vtable cFEStateChallengePopup_vtbl,
// mHashName 0x3F; resets the new-race interface) and
// the default cFEStateChallengePopup_cFEStateChallengePopup1 (fe/fepopupmisc); destructor
// cFEStateChallengePopup__cFEStateChallengePopup (fe/fepopupmisc; slot 1: calls mpParentState's
// slot 32, then cFEStateNet's cFEStateNet__cFEStateNet). Retail vtable cFEStateChallengePopup_vtbl (33 slots;
// vtables.tsv lists 24: 4
// onCreateScreen cFEStateChallengePopup_onCreateScreen,
// 6 onGainFocus cFEStateChallengePopup_onGainFocus, 7 cFEStateChallengePopup_onGainTransition, 10
// cFEStateChallengePopup_onLoseTransition, 12 onUpdate cFEStateChallengePopup_onUpdate, 17 onPopupEvent
// cFEStateChallengePopup_onPopupEvent, 18 onWidgetCreate cFEStateChallengePopup_onWidgetCreate, 19 onWidgetEvent
// cFEStateChallengePopup_onWidgetEvent, 20 cFEStateChallengePopup_onWidgetMenu, 31 cFEStateChallengePopup_onNetEvent
// (net events), 32 cFEStateChallengePopup_publishPresence, 33 cFEStateChallengePopup_onErrorPopupEvent).
// Methods: fe/feasyncfile 0x1ADDD0-0x1AFD88.
//
// cFEStateChallengeReceivedPopup: screen "241challenge_received_popup": shows a received challenge's options and
// accepts / declines it. EA's name: the preview's __30cFEStateChallengeReceivedPopup, _$_30...,
// onGainTransition__30...
// and onLoseTransition__30.... Size 0x6D8: cMemMan_alloc(0x6D8, "cFEStateChallengeReceivedPopup") +
// cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup(mem,
// pEngine, pParent, szChallenger) (cFEStateNet_challengePlayer, the only site). Constructors
// cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup (vtable cFEStateChallengeReceivedPopup_vtbl,
// mHashName 0x40) and the default cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup1 (fe/fepopupmisc);
// destructor cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup (fe/fepopupmisc; slot 1:
// mpParentState's slot 32, destroys mszChallenger, cFEStateNet's dtor). Retail vtable
// cFEStateChallengeReceivedPopup_vtbl (33 slots;
// vtables.tsv lists 24: 4
// onCreateScreen cFEStateChallengeReceivedPopup_onCreateScreen, 7 cFEStateChallengeReceivedPopup_onGainTransition, 10
// cFEStateChallengeReceivedPopup_onLoseTransition, 12 onUpdate cFEStateChallengeReceivedPopup_onUpdate, 17 onPopupEvent
// cFEStateChallengeReceivedPopup_onPopupEvent, 18 onWidgetCreate cFEStateChallengeReceivedPopup_onWidgetCreate, 19
// onWidgetEvent cFEStateChallengeReceivedPopup_onWidgetEvent, 20 cFEStateChallengeReceivedPopup_onWidgetMenu, 24
// handleEvent
// cFEStateNet_handleEvent, 31 cFEStateChallengeReceivedPopup_onNetEvent (net events), 32 cFEStateChallengeReceivedPopup_publishPresence, 33 cFEStateChallengeReceivedPopup_onErrorPopupEvent). Methods: fe/feasyncfile
// 0x1AFE08-0x1B0F28.
//
// Derives from cFEStateNet (fe/festatenet.h); its own members at their retail offsets. There is no On Tour DWARF for
// these classes (On Tour has no online FE), so the own fields are named from their retail use unless noted.
// Evidence tags:
//   [R] retail's constructors / destructors named above (and cFEStateNet's, see fe/festatenet.h);
//   [P] the July 2003 preview (its default constructors, destructors and inline accessors, see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.
// Popup events (onPopupEvent / the cFEStateNet popup handlers): 0xF + i = option i chosen, 0x14 = back, 0x16 = the
// popup closed (fe/screenpopup.h). The popup id is the cScreenPopup's miMiscData (+ 0x18), set to 0x8xx by its
// creator.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festatenet.h"

struct cUIListBox;

struct cBENewRaceInterface;
struct cUIScreen;
struct cUIText;

struct cFEStateChallengePopup : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateChallengePopup_vtbl (33 slots), stored by cFEStateChallengePopup_cFEStateChallengePopup /
    //       cFEStateChallengePopup_cFEStateChallengePopup1 after the cFEStateNet ctor and by the destructor
    //       cFEStateChallengePopup__cFEStateChallengePopup
    // cFEStateChallengePopup's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateChallengePopup_cFEStateChallengePopup stores
    //       0x3F
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateChallengePopup
    int mbChallengeSent;            // 0x6D0 [R] 0 (both ctors); [U] cFEStateChallengePopup_setChallengeSent sets 1 when the challenge goes out
                                    //       ("staticTextSetchallenge" = "kT_240WaitingChallenge", options locked:
                                    //       cFEStateChallengePopup_setOptionsLocked(self, 1)), cFEStateChallengePopup_clearChallengeSent sets 0
                                    //       ("kT_241SetChallengeOptions"); onWidgetEvent
                                    //       cFEStateChallengePopup_onWidgetEvent ignores
                                    //       "submit" (9) while it is set and passes it to LobbyMan_cancelChallenge (cancel);
                                    //       onGainFocus cFEStateChallengePopup_onGainFocus re-locks the options with it
    int mbFirstCreate;              // 0x6D4 [R] 1 (cFEStateChallengePopup_cFEStateChallengePopup, after resetting the
                                    //       race interface); [U]
                                    //       onCreateScreen cFEStateChallengePopup_onCreateScreen clears it on the first
                                    //       screen, and selects
                                    //       "MenuChallenge" row 6 when it is already 0
    int* mpEventOptions;            // 0x6D8 [R] D_00441268 (both ctors); [U] onGainFocus
                                    //       cFEStateChallengePopup_onGainFocus points it at
                                    //       one of six 4-word tables (D_00441268 .. D_004412B8) by the event kind
                                    //       newRace.cGameEvent: [0] CPU riders, [1] power-ups, [2] point icons, [3]
                                    //       multipliers allowed (cFEStateChallengePopup_setOptionsLocked, cFEStateChallengePopup_setOptionListBoxes)
    cUIListBox* mpLBRankedGame;     // 0x6DC [R] 0 (cFEStateChallengePopup_cFEStateChallengePopup); [U] onWidgetCreate
                                    //       cFEStateChallengePopup_onWidgetCreate: widget
                                    //       "ListBoxRankedGame" (yes / no); cFEStateChallengePopup_applyRaceOptions / onWidgetEvent read its
                                    //       value (cUIListBox_getCurrentEntryData) into D_00534B34; onGainFocus sets it
                                    //       (cUIListBox_setCurrentEntryByIndex)
    cUIListBox* mpLBCPURiders;      // 0x6E0 [U] onWidgetCreate: widget "ListBoxCPURiders" (none, 1-4); its value is
                                    //       the new-race interface's number of AI riders
                                    //       (cBENewRaceInterface_setNumberAI); cFEStateChallengePopup_fillRaceTexts reads its slot 8
    cUIListBox* mpLBPowerUps;       // 0x6E4 [U] onWidgetCreate: widget "ListBoxPowerUps"; its value is race-option
                                    //       bit 8 of cBERules_mRules (cFEStateChallengePopup_applyRaceOptions, onWidgetEvent)
    cUIListBox* mpLBMultipliers;    // 0x6E8 [U] onWidgetCreate: widget "ListBoxMultipliers"; race-option bit 6 of
                                    //       cBERules_mRules
    cUIListBox* mpLBPointIcons;     // 0x6EC [U] onWidgetCreate: widget "ListBoxPointicons"; race-option bit 7 of
                                    //       cBERules_mRules

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateChallengePopup1() __asm__("cFEStateChallengePopup_cFEStateChallengePopup1");
    // retail's destructor cFEStateChallengePopup__cFEStateChallengePopup stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateChallengePopup();
    virtual void onPopupEvent(cUIState*pPopup, unsigned int Event) __asm__("cFEStateChallengePopup_onPopupEvent");
    virtual void onCreateScreen() __asm__("cFEStateChallengePopup_onCreateScreen");
    virtual bool onLoseTransition() __asm__("cFEStateChallengePopup_onLoseTransition__FPv");
    virtual bool onGainTransition(int) __asm__("cFEStateChallengePopup_onGainTransition");
    virtual void onUpdate() __asm__("cFEStateChallengePopup_onUpdate__FPv");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int key, unsigned int)
        __asm__("cFEStateChallengePopup_onWidgetMenu");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cFEStateChallengePopup_onWidgetEvent");
    virtual void onWidgetCreate(cUIThing* pWidget) __asm__("cFEStateChallengePopup_onWidgetCreate");
    void applyRaceOptions() __asm__("cFEStateChallengePopup_applyRaceOptions");
    void setOptionsLocked(int on) __asm__("cFEStateChallengePopup_setOptionsLocked");
    void setOptionListBoxes(int all) __asm__("cFEStateChallengePopup_setOptionListBoxes");
    void showListBoxArrows(cUIText* item, int on) __asm__("cFEStateChallengePopup_showListBoxArrows");
    int onErrorPopupEvent(void* popup, int event) __asm__("cFEStateChallengePopup_onErrorPopupEvent");
    void* onNetEvent(int msgId, void* data) __asm__("cFEStateChallengePopup_onNetEvent");
    void clearChallengeSent() __asm__("cFEStateChallengePopup_clearChallengeSent");
    void fillRaceTexts(cBENewRaceInterface* iface, cUIScreen* screen) __asm__("cFEStateChallengePopup_fillRaceTexts");
    virtual void onGainFocus(cUIState* pPrevState) __asm__("cFEStateChallengePopup_onGainFocus");
    void setChallengeSent() __asm__("cFEStateChallengePopup_setChallengeSent");
};

CHECK_OFFSET(cFEStateChallengePopup, mbChallengeSent, 0x6D0);
CHECK_OFFSET(cFEStateChallengePopup, mbFirstCreate, 0x6D4);
CHECK_OFFSET(cFEStateChallengePopup, mpEventOptions, 0x6D8);
CHECK_OFFSET(cFEStateChallengePopup, mpLBRankedGame, 0x6DC);
CHECK_OFFSET(cFEStateChallengePopup, mpLBCPURiders, 0x6E0);
CHECK_OFFSET(cFEStateChallengePopup, mpLBPowerUps, 0x6E4);
CHECK_OFFSET(cFEStateChallengePopup, mpLBMultipliers, 0x6E8);
CHECK_OFFSET(cFEStateChallengePopup, mpLBPointIcons, 0x6EC);
CHECK_SIZE(cFEStateChallengePopup, 0x6F0);

struct cUIScreen;

struct cFEStateChallengeReceivedPopup : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateChallengeReceivedPopup_vtbl (33 slots), stored by
    //       cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup /
    //       cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup1 after the cFEStateNet ctor and by the
    //       destructor cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup
    // cFEStateChallengeReceivedPopup's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R]
    //       cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup stores 0x40
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateChallengeReceivedPopup
    char* mszChallenger;            // 0x6D0 [R] cBXString: built from the ctor's 4th argument (cBXString_cBXString2),
                                    //       the empty rep cBXString_EmptyString in the default ctor, destroyed by
                                    //       cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup;
                                    //       [U] onCreateScreen cFEStateChallengeReceivedPopup_onCreateScreen looks the
                                    //       challenge up by it
                                    //       (BuddyMan_getChallengeFrom(BuddyMan, ..)) and copies it to LobbyMan (+ 0x98);
                                    //       cFEStateChallengeReceivedPopup_publishPresence (BuddyMan_setPresence) and cFEStateChallengeReceivedPopup_onNetEvent (net event 0xEF,
                                    //       BuddyMan_declineChallenge) pass its text
    int mbAccepted;                 // 0x6D4 [R] 0 (cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup; not
                                    //       set by the default ctor); [U] cFEStateChallengeReceivedPopup_setAccepted
                                    //       sets 1 when the challenge is accepted ("TextAccept" enabled,
                                    //       "kT_OVRCMNWaitPlayer"), cFEStateChallengeReceivedPopup_clearAccepted sets 0; onWidgetEvent
                                    //       cFEStateChallengeReceivedPopup_onWidgetEvent
                                    //       accepts (row 0xB) only while it is 0; cFEStateChallengeReceivedPopup_declineChallenge (decline) cancels the
                                    //       accepted game (LobbyMan_leaveGame) when it is set

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateChallengeReceivedPopup1() __asm__("cFEStateChallengeReceivedPopup_cFEStateChallengeReceivedPopup1");
    // retail's destructor cFEStateChallengeReceivedPopup__cFEStateChallengeReceivedPopup stays a C function (g++ 2.95
    // takes no asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateChallengeReceivedPopup();
    virtual bool onGainTransition(int bFirstTime) __asm__("cFEStateChallengeReceivedPopup_onGainTransition");
    virtual void onCreateScreen() __asm__("cFEStateChallengeReceivedPopup_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* obj) __asm__("cFEStateChallengeReceivedPopup_onWidgetCreate");
    virtual bool onLoseTransition() __asm__("cFEStateChallengeReceivedPopup_onLoseTransition__FPv");
    virtual void onUpdate() __asm__("cFEStateChallengeReceivedPopup_onUpdate__FPv");
    virtual void onWidgetEvent(cUIThing* obj, unsigned int msg) __asm__("cFEStateChallengeReceivedPopup_onWidgetEvent");
    virtual void onPopupEvent(cUIState* pPopup, unsigned int Event)
        __asm__("cFEStateChallengeReceivedPopup_onPopupEvent");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int key, unsigned int)
        __asm__("cFEStateChallengeReceivedPopup_onWidgetMenu");
    void publishPresence() __asm__("cFEStateChallengeReceivedPopup_publishPresence");
    int onErrorPopupEvent(void* popup, int event) __asm__("cFEStateChallengeReceivedPopup_onErrorPopupEvent");
    void* onNetEvent(int msgId, void* data) __asm__("cFEStateChallengeReceivedPopup_onNetEvent");
    void fillRaceTexts(void* iface, cUIScreen* screen) __asm__("cFEStateChallengeReceivedPopup_fillRaceTexts");
    void setAccepted() __asm__("cFEStateChallengeReceivedPopup_setAccepted");
    void clearAccepted() __asm__("cFEStateChallengeReceivedPopup_clearAccepted");
    void declineChallenge() __asm__("cFEStateChallengeReceivedPopup_declineChallenge");
};

CHECK_OFFSET(cFEStateChallengeReceivedPopup, mszChallenger, 0x6D0);
CHECK_OFFSET(cFEStateChallengeReceivedPopup, mbAccepted, 0x6D4);
CHECK_SIZE(cFEStateChallengeReceivedPopup, 0x6D8);

#endif /* FE_FESTATENETCHALLENGE_H */
