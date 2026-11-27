#ifndef FE_FESTATENETCONFIG_H
#define FE_FESTATENETCONFIG_H

// The online network-configuration screens (EA's d:/ssx/code/source/fe/festatenetconfig.cpp: the preview's asserts
// at lines 554 and 641 sit between retail findOnlineCharacterRow and cFEStateAuthDNAS_onCreateScreen (cFEStateNetConfig / cFEStateAuthDNAS),
// those at 1381-1394 between cFEStateNetAccountMem_onUpdate and cFEStateNetAccountMem_handleEvent, both cFEStateNetAccountMem methods). Two classes
// live here, both cFEStateNet states (fe/festatenet.h, 0x6D0):
//
// cFEStateNetConfig: screen "200net_config": picks a network adapter and a saved network configuration, connects
// (a state machine over the memory card object D_004A1B78 = cMemCard_CreateInstance(), cMemCard::CreateInstance) and runs DNAS authentication. Size 0x91C:
// cMemMan_alloc(0x91C, "cFEStateNetConfig") + cFEStateNetConfig_cFEStateNetConfig(mem, pEngine) (cFEStateNet_onNetInterruptedPopupEvent, cFEStateLogin_onNetConfigPopupEvent); its
// unicode popup buffer at 0x71C runs to the end. Constructors cFEStateNetConfig_cFEStateNetConfig and cFEStateNetConfig_cFEStateNetConfig1 (fe/fepopupmisc, the
// default one: zeroes the same fields), destructor cFEStateNetConfig__cFEStateNetConfig (frees the heap texts). Retail vtable cFEStateNetConfig_vtbl
// (32 slots: 1 dtor, 4 onCreateScreen cFEStateNetConfig_onCreateScreen, 5 onDestroyScreen cFEStateNetConfig_onDestroyScreen, 12 onUpdate cFEStateNetConfig_onUpdate,
// 17 onPopupEvent cFEStateNetConfig_onPopupEvent, 18 onWidgetCreate cFEStateNetConfig_onWidgetCreate, 19 onWidgetEvent cFEStateNetConfig_onWidgetEvent, 21 cFEStateNetConfig_onWidgetPair,
// 22 cFEStateNetConfig_onWidgetListBox, 25 cFEStateNetConfig_fillHelpPopup (help), 29 cFEStateNetConfig_isOnlineState, 31 cFEStateNetConfig_onNetEvent, 32 cFEStateNetConfig_publishPresence). Methods:
// fe/feasyncfile 0x1B3F78-0x1B60A8. The DNAS popup cFEStateAuthDNAS (cFEStateAuthDNAS_onUpdate) writes mpustrAuthError
// through its mpParentState.
//
// cFEStateNetAccountMem: the memory-card popup that loads / saves / deletes the online account (name and password)
// on the card (EA's names: the preview's onGainTransition__21cFEStateNetAccountMemb, onLoseTransition__21...).
// Base: cFEBlankPopup (constructor cFEBlankPopup_cFEBlankPopup = cFEStateNet's + vtable cFEBlankPopup_vtbl; it adds no members), so its
// own fields start at 0x6D0. Size 0xB10: every allocation is cMemMan_alloc(0xB10, "cFEStateNetAccountMem") +
// cFEStateNetAccountMem_cFEStateNetAccountMem(mem, pEngine, pParent, mode, szName, szPassword) (14 sites); the constructor stores 0xB0C last.
// Destructor cFEStateNetAccountMem__cFEStateNetAccountMem. Retail vtable cFEStateNetAccountMem_vtbl (24 slots: 1 dtor, 4 onCreateScreen cFEStateNetAccountMem_onCreateScreen, 7
// onGainTransition cFEStateNetAccountMem_onGainTransition, 10 onLoseTransition cFEStateNetAccountMem_onLoseTransition, 12 onUpdate cFEStateNetAccountMem_onUpdate, 17 onPopupEvent
// cFEStateNetAccountMem_onPopupEvent, 24 handleEvent cFEStateNetAccountMem_handleEvent). Methods: fe/feasyncfile 0x1B6ED0-0x1B8890.
//
// Derives from cFEStateNet (fe/festatenet.h); its own members at their retail offsets. There is no On Tour DWARF for
// these classes and the preview keeps no accessors for them, so the own fields are named from their retail use.
// Evidence tags:
//   [R] retail's constructors / destructors named above (and cFEStateNet's, see fe/festatenet.h);
//   [P] the July 2003 preview (see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.
// Popup events: 0xF + i = option i chosen, 0x14 = back, 0x16 = the popup closed (fe/screenpopup.h).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festatenet.h"

struct cUIThing;
struct cUIMenu;
struct cUIListBox;

struct cFEStateNetConfig : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateNetConfig_vtbl (32 slots), stored by cFEStateNetConfig_cFEStateNetConfig /
    //       cFEStateNetConfig_cFEStateNetConfig1 after the cFEStateNet ctor and by the destructor
    //       cFEStateNetConfig__cFEStateNetConfig
    // cFEStateNetConfig's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateNetConfig_cFEStateNetConfig stores 0x31
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateNetConfig
    int miSelectedSetting;          // 0x6D0 [R] 0; [U] onWidgetEvent cFEStateNetConfig_onWidgetEvent stores the "LBSettings" row (list + 0x319) and
                                    //       selects it in mpLBAdapters; cFEStateNetConfig_connectWithSelectedConfig passes it to the connect call (cConnMan_connect)
    int miNumSettings;              // 0x6D4 [R] 0; [U] cFEStateNetConfig_loadConfigs: cCommSystem_mNumNetConn clamped to 4, one list row each;
                                    //       cFEStateNetConfig_onWidgetListBox tests > 1
    int mbCancelled;                // 0x6D8 (name ours) [R] 0; [U] cFEStateNetConfig_onConnectingPopupEvent / cFEStateNetConfig_onDNASAuthPopupEvent: 1 when the popup is
                                    //       cancelled (events 0xF/0x14); on 0x16 they push "NetConfigAuthPopup" /
                                    //       cFEStateNetAccountMem only while it is 0, then clear it
    int mbPushLogin;                // 0x6DC (name ours) [U] cFEStateNetConfig_onAccountMemPopupEvent: 1 on event 0xF, 0 on 0x15; on 0x16 it pushes
                                    //       cFEStateLogin when set
    int miWaitFrames;               // 0x6E0 [R] -1; [U] cFEStateNetConfig_loadConfigs sets 15; onUpdate cFEStateNetConfig_onUpdate counts it down in state 5
    cUIListBox* mpLBAdapters;       // 0x6E4 [R] 0; [U] onWidgetCreate cFEStateNetConfig_onWidgetCreate: widget "LBAdapters"; cFEStateNetConfig_loadConfigs adds a row
                                    //       per configuration (cUIListBox_addEntryByAsciiString)
    cUIListBox* mpLBSettings;       // 0x6E8 [R] 0; [U] widget "LBSettings"; cFEStateNetConfig_loadConfigs reads its font/scale (+ 0x50) and adds
                                    //       the numbered configuration names
    cUIThing* mpPairSettings;       // 0x6EC [U] widget "PairSettings"; enabled / disabled (its slot 8) by cFEStateNetConfig_showConfigLists and the popup
                                    //       handlers
    int unk_6F0;                    // 0x6F0 [R] 0 in the default constructor cFEStateNetConfig_cFEStateNetConfig1 only; no other access seen
    cUIMenu* mpMenuConfig;          // 0x6F4 [R] 0; [U] widget "MenuConfig"; cFEStateNetConfig_showConfigLists cUIMenu_setSelectedByIndex(.., 0 / 1)
    cUIThing* mpConfigRectangle;    // 0x6F8 [R] 0; [U] widget "configrectangle1"; cFEStateNetConfig_showConfigLists shows / hides it (slot 9)
    cUIThing* mpNoConfiguration;    // 0x6FC [R] 0; [U] widget "NoConfiguration"; cFEStateNetConfig_showConfigLists shows it when there is no
                                    //       configuration
    int mbAuthFailed;               // 0x700 (name ours) [R] 0; [U] cFEStateNetConfig_onDNASAuthPopupEvent: 1 on event 0x10, 0 on 0xF; on 0x16 it
                                    //       pushes cFEStateAuthDNASError (with mpustrAuthError) when set
    unsigned short* mpustrPopupText; // 0x704 [R] 0; [U] cFEStateNetConfig_onNetEvent (slot 31) frees it and stores a heap copy of a
                                    //       unicode / ascii message (operator_new_tag + USTR_copy / strcpy1), then points the popup's message (+ 0xC4)
                                    //       at it; the destructor frees it
    int miState;                    // 0x708 [R] 0; [U] the connect state machine of onUpdate cFEStateNetConfig_onUpdate (0..8); cFEStateNetConfig_startConnect sets 6,
                                    //       cFEStateNetConfig_checkCardAndLoadConfigs / cFEStateNetConfig_checkCardSlot / cFEStateNetConfig_connectWithSelectedConfig / cFEStateNetConfig_onCardErrorPopupEvent / cFEStateNetConfig_loadConfigs / cFEStateNetConfig_rescanConfigs advance it
    int miResult;                   // 0x70C [U] cFEStateNetConfig_checkCardSlot / cFEStateNetConfig_onCardErrorPopupEvent / cFEStateNetConfig_loadConfigs / cFEStateNetConfig_rescanConfigs: the last connect result
                                    //       (1, -200, the number of configurations)
    unsigned short* mpustrAuthError[3]; // 0x710 [R] 0; [U] cFEStateAuthDNAS cFEStateAuthDNAS_onUpdate stores three heap unicode
                                    //       copies of the DNAS error texts here (through its mpParentState); cFEStateNetConfig_onDNASAuthPopupEvent passes [1], [2], [0] to
                                    //       cFEStateAuthDNASError's constructor cFEStateAuthDNASError_cFEStateAuthDNASError; the destructor frees them
    unsigned short mustrPopupText[0x100]; // 0x71C [U] sprintf1 (unicode) target of cFEStateNetConfig_connectWithSelectedConfig / cFEStateNetConfig_showCheckingCardPopup /
                                    //       cFEStateNetConfig_showCardErrorPopup that becomes the popup's message (+ 0xC4); extent: to the end of the object

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateNetConfig1() __asm__("cFEStateNetConfig_cFEStateNetConfig1");
    // retail's destructor cFEStateNetConfig__cFEStateNetConfig stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateNetConfig();
    virtual void onCreateScreen() __asm__("cFEStateNetConfig_onCreateScreen");
    virtual void onDestroyScreen() __asm__("cFEStateNetConfig_onDestroyScreen");
    virtual void onUpdate() __asm__("cFEStateNetConfig_onUpdate");
    virtual void onWidgetCreate(cUIThing* item) __asm__("cFEStateNetConfig_onWidgetCreate");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cFEStateNetConfig_onWidgetEvent");
    virtual void onPopupEvent(cUIState* pPopup, unsigned int Event) __asm__("cFEStateNetConfig_onPopupEvent");
    virtual int onWidgetPair(cUIPair*, unsigned int) __asm__("cFEStateNetConfig_onWidgetPair__FPv");
    virtual int onWidgetListBox(cUIListBox* pListBox, unsigned int key) __asm__("cFEStateNetConfig_onWidgetListBox");
};

CHECK_OFFSET(cFEStateNetConfig, miSelectedSetting, 0x6D0);
CHECK_OFFSET(cFEStateNetConfig, miNumSettings, 0x6D4);
CHECK_OFFSET(cFEStateNetConfig, mbCancelled, 0x6D8);
CHECK_OFFSET(cFEStateNetConfig, mbPushLogin, 0x6DC);
CHECK_OFFSET(cFEStateNetConfig, miWaitFrames, 0x6E0);
CHECK_OFFSET(cFEStateNetConfig, mpLBAdapters, 0x6E4);
CHECK_OFFSET(cFEStateNetConfig, mpLBSettings, 0x6E8);
CHECK_OFFSET(cFEStateNetConfig, mpPairSettings, 0x6EC);
CHECK_OFFSET(cFEStateNetConfig, unk_6F0, 0x6F0);
CHECK_OFFSET(cFEStateNetConfig, mpMenuConfig, 0x6F4);
CHECK_OFFSET(cFEStateNetConfig, mpConfigRectangle, 0x6F8);
CHECK_OFFSET(cFEStateNetConfig, mpNoConfiguration, 0x6FC);
CHECK_OFFSET(cFEStateNetConfig, mbAuthFailed, 0x700);
CHECK_OFFSET(cFEStateNetConfig, mpustrPopupText, 0x704);
CHECK_OFFSET(cFEStateNetConfig, miState, 0x708);
CHECK_OFFSET(cFEStateNetConfig, miResult, 0x70C);
CHECK_OFFSET(cFEStateNetConfig, mpustrAuthError, 0x710);
CHECK_OFFSET(cFEStateNetConfig, mustrPopupText, 0x71C);
CHECK_SIZE(cFEStateNetConfig, 0x91C);

struct cFEStateNetAccountMem : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateNetAccountMem_vtbl (24 slots), stored by cFEStateNetAccountMem_cFEStateNetAccountMem after
    //       cFEBlankPopup's ctor (cFEBlankPopup_cFEBlankPopup, vtable cFEBlankPopup_vtbl) and by the destructor
    //       cFEStateNetAccountMem__cFEStateNetAccountMem (which restores cFEBlankPopup_vtbl before cFEStateNet's)
    // cFEStateNetAccountMem's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] 0 (not set by
    //       cFEStateNetAccountMem_cFEStateNetAccountMem)
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateNetAccountMem
    int miResult;                   // 0x6D0 [U] onPopupEvent cFEStateNetAccountMem_onPopupEvent: the event (0xF/0x10/0x11) reported to mpParentState's
                                    //       onPopupEvent (slot 17) when the popup closes (0x16)
    int miError;                    // 0x6D4 [R] 0; [U] cFEStateNetAccountMem_setStep / cFEStateNetAccountMem_updateDeleteReadStep / cFEStateNetAccountMem_onPopupEvent store the card result (2, 3, 6, 0x18 ..)
    int mbCardError;                // 0x6D8 (name ours) [R] 0; [U] cFEStateNetAccountMem_updateDeleteReadStep sets it on a card read error or an
                                    //        account mismatch, the card-change check in onUpdate
                                    //        cFEStateNetAccountMem_onUpdate sets it; cFEStateNetAccountMem_setStep clears it in states
                                    //        0/5/7/11/13/19/21 and skips to the error step when it is set (8 -> 9, 22
                                    //        -> 23, 26 -> 28); onPopupEvent cFEStateNetAccountMem_onPopupEvent reports
                                    //        error 6 when it is set
    int miState;                    // 0x6DC [U] cFEStateNetAccountMem_setStep(self, state) stores it and sets up the state's popup text; onUpdate
                                    //       cFEStateNetAccountMem_onUpdate and onPopupEvent cFEStateNetAccountMem_onPopupEvent switch on it
    char mszUserName[0x11];         // 0x6E0 [R] strcpy of the constructor's 5th argument (LobbyMan mUserName / tLobbyAccount mName at
                                    //       the call sites), AccountData in mode 3; [U] cFEStateNetAccountMem_copyToAccountData hands it to the card code (sharedlogin_setAccountName),
                                    //       cFEStateNetAccountMem_updateDeleteReadStep compares it with AccountData
    char mszPassword[0x11];         // 0x6F1 [R] strcpy of the 6th argument (mPassword), AccountData + 0x11 in mode 3; [U] sharedlogin_setAccountPassword,
                                    //       cFEStateNetAccountMem_updateDeleteReadStep compares it with AccountData + 0x11
    unsigned short mustrText[0x201]; // 0x702 [U] cFEStateNetAccountMem_setStep: sprintf1 / strcpy1 (unicode) target of the state's message,
                                    //       passed to the popup (cFEStateNetAccountMem_showPopup); cFEStateNetAccountMem_showErrorPopup / cFEStateNetAccountMem_formatNotDetectedText / cFEStateNetAccountMem_formatFailedText; extent: up to 0xB04
    int mbCardBusy;                 // 0xB04 [R] 0; [U] cFEStateNetAccountMem_setStep: 1 while a card operation runs (states 8/14/16/22), 0 otherwise;
                                    //       cFEStateNetAccountMem_showErrorPopup tests it
    int mbAccountMismatch;          // 0xB08 (name ours) [R] 0; [U] cFEStateNetAccountMem_updateDeleteReadStep sets it with miError 0x18 when the
                                    //        card's account (AccountData) is not mszUserName / mszPassword;
                                    //        cFEStateNetAccountMem_onPopupEvent: in step 23, OK restarts from step 0
                                    //        when it is set
    int miStartState;               // 0xB0C [R] by the constructor's mode: 0 -> 0, 1 -> 5, 2 -> 0xB, 3 -> 0x13 (cFEStateNetAccountMem_setStep starts there);
                                    //       [U] cFEStateNetAccountMem_onPopupEvent restarts from it

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cFEStateNetAccountMem__cFEStateNetAccountMem stays a C function (g++ 2.95 takes no asm label
    // on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateNetAccountMem();
    virtual void onUpdate() __asm__("cFEStateNetAccountMem_onUpdate");
    virtual void onPopupEvent(cUIState* w, unsigned int msg) __asm__("cFEStateNetAccountMem_onPopupEvent");
    virtual bool onLoseTransition() __asm__("cFEStateNetAccountMem_onLoseTransition__FPv");
};

CHECK_OFFSET(cFEStateNetAccountMem, miResult, 0x6D0);
CHECK_OFFSET(cFEStateNetAccountMem, miError, 0x6D4);
CHECK_OFFSET(cFEStateNetAccountMem, mbCardError, 0x6D8);
CHECK_OFFSET(cFEStateNetAccountMem, miState, 0x6DC);
CHECK_OFFSET(cFEStateNetAccountMem, mszUserName, 0x6E0);
CHECK_OFFSET(cFEStateNetAccountMem, mszPassword, 0x6F1);
CHECK_OFFSET(cFEStateNetAccountMem, mustrText, 0x702);
CHECK_OFFSET(cFEStateNetAccountMem, mbCardBusy, 0xB04);
CHECK_OFFSET(cFEStateNetAccountMem, mbAccountMismatch, 0xB08);
CHECK_OFFSET(cFEStateNetAccountMem, miStartState, 0xB0C);
CHECK_SIZE(cFEStateNetAccountMem, 0xB10);

#endif /* FE_FESTATENETCONFIG_H */
