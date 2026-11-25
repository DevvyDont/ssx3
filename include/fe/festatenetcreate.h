#ifndef FE_FESTATENETCREATE_H
#define FE_FESTATENETCREATE_H

// The online account-creation screens (EA's d:/ssx/code/source/fe/festatenetcreate.cpp: the preview's assert at line
// 1102 sits in cFEStateCreateStep2's onPopupEvent, retail cFEStateCreateStep2_onPopupEvent; Step1's code comes right before it in
// link order). Both are cFEStateNet states (fe/festatenet.h, 0x6D0) that edit a LobbyMan account form
// (tLobbyAccount, online/lobbyman.h). Two classes live here:
//
// cFEStateCreateStep1: screen "210create_step1": account name, password, confirmation, e-mail and the two
// "inform me" / "share info" yes-no boxes. Size 0x6F4: cFEStateCreateStep1 is allocated with
// cMemMan_alloc(0x6F4, "cFEStateCreateStep1") (cFEStateSelectPersona_onWidgetEvent, cFEStateCreateStep2_onWidgetEvent, cFEStateLogin_onWidgetEvent, cFEStateOnlineAgreement_onWidgetEvent) and
// constructed by cFEStateCreateStep1_cFEStateCreateStep1(mem, pEngine, bEdit); every own field ends below 0x6F4. Constructors cFEStateCreateStep1_cFEStateCreateStep1
// and the default cFEStateCreateStep1_cFEStateCreateStep11 (fe/fepopupmisc; the preview's __19cFEStateCreateStep1), destructor cFEStateCreateStep1__cFEStateCreateStep1
// (fe/fepopupmisc; the preview's _$_19cFEStateCreateStep1). Retail vtable cFEStateCreateStep1_vtbl (32 slots: 1 dtor, 4
// onCreateScreen cFEStateCreateStep1_onCreateScreen, 7 onGainTransition cFEStateCreateStep1_onGainTransition, 12 onUpdate cFEStateCreateStep1_onUpdate, 17 onPopupEvent
// cFEStateCreateStep1_onPopupEvent, 18 onWidgetCreate cFEStateCreateStep1_onWidgetCreate, 19 onWidgetEvent cFEStateCreateStep1_onWidgetEvent, 25 func_001B9088 (help text),
// 29 func_001E0D60, 31 func_001B9488 (net event 0xCE)). Methods: fe/feasyncfile 0x1B88F0-0x1BA358.
//
// cFEStateCreateStep2: screen "211create_step2": birth date, gender and the spam option of the same form. Size
// 0x6E0: cMemMan_alloc(0x6E0, "cFEStateCreateStep2") + cFEStateCreateStep2_cFEStateCreateStep2(mem, pEngine) (cFEStateCreateStep1_onWidgetEvent, cFEStateOnlineAgreement_onWidgetEvent);
// own fields end below 0x6E0. Constructors cFEStateCreateStep2_cFEStateCreateStep2 and the default cFEStateCreateStep2_cFEStateCreateStep21 (fe/fepopupmisc),
// destructor cFEStateCreateStep2__cFEStateCreateStep2 (fe/fepopupmisc; the preview's _$_19cFEStateCreateStep2). Retail vtable cFEStateCreateStep2_vtbl
// (slots 4 onCreateScreen cFEStateCreateStep2_onCreateScreen, 12 onUpdate cFEStateCreateStep2_onUpdate, 17 onPopupEvent cFEStateCreateStep2_onPopupEvent, 18
// onWidgetCreate cFEStateCreateStep2_onWidgetCreate, 19 onWidgetEvent cFEStateCreateStep2_onWidgetEvent, 25 func_001BA8B8, 29 func_001E0DD0, 31
// func_001BAB28). Methods: fe/feasyncfile 0x1BA3E0-0x1BBC40.
//
// Derives from cFEStateNet (fe/festatenet.h); its own members at their retail offsets. There is no On Tour DWARF for
// these classes (On Tour has no online FE) and the preview keeps no accessors (tools/layout_xlate.py: preview
// 0xC4/0xC8 = retail 0x6EC/0x6F0 for Step1, a +0x628 shift; Step2 +0x624), so the own fields are named from their
// retail use. Evidence tags:
//   [R] retail's constructors / destructors named above (and cFEStateNet's, see fe/festatenet.h);
//   [P] the July 2003 preview (see fe/festatenet.h);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF for the cListNode/cUIState base only.
// Popup events (cScreenPopup_onPopupEvent / onWidgetEvent): 0xF + i = option i chosen, 0x14 = back, 0x16 = the popup
// closed (fe/screenpopup.h).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festatenet.h"

struct tLobbyAccount;
struct cUIText;

struct cFEStateCreateStep1 : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateCreateStep1_vtbl (32 slots), stored by cFEStateCreateStep1_cFEStateCreateStep1 /
    //       cFEStateCreateStep1_cFEStateCreateStep11 after the cFEStateNet ctor and by the destructor
    //       cFEStateCreateStep1__cFEStateCreateStep1
    // cFEStateCreateStep1's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateCreateStep1_cFEStateCreateStep1 stores 0x4D
    //       (edit) or 0x50 (create)
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateCreateStep1
    tLobbyAccount* mpAccount;       // 0x6D0 [U] the account form being edited: onCreateScreen cFEStateCreateStep1_onCreateScreen stores
                                    //       (char*)LobbyMan_mLobbyMan + 0x2B0 when mbEditAccount, else + 0x260 (online/lobbyman.h); every text box
                                    //       reads / writes its mName/mPassword/unk_08/mMail/mSpam fields (func_001B95C0..func_001B97B0, func_001B9820)
    char* mszSavedPassword;         // 0x6D4 [R] cBXString (cFEStateCreateStep1_cFEStateCreateStep1 stores the empty-string rep cBXString_EmptyString, cFEStateCreateStep1__cFEStateCreateStep1
                                    //       destroys it); [U] func_001BA178 saves mpAccount->mPassword here when editing, func_001BA218 restores it,
                                    //       func_001BA358 sets mpAccount->mChangePassword when it differs
    char* mszSavedConfirm;          // 0x6D8 [R] cBXString (as above); [U] the saved mpAccount->unk_08 (func_001BA178 / func_001BA218)
    char* mszSavedMail;             // 0x6DC [R] cBXString (as above); [U] the saved mpAccount->mMail; func_001BA358 sets mChangeMail when
                                    //       it differs
    int miSavedSpam[2];             // 0x6E0 [U] func_001BA178 saves mpAccount->mSpam[0..1], func_001BA218 restores them, func_001BA358
                                    //       sets mpAccount->mChangeSpam when either differs
    int unk_6E8;                    // 0x6E8 [U] func_001B92C8 (popup 0x82C events): 1 on 0x11 (option 2), 0 on 0xF/0x10/0x14; on 0x16
                                    //       it opens the "kT_POPUP204SaveAccount" yes/no popup when set, else pushes cFEStateSelectPersona
    int mbEditAccount;              // 0x6EC [R] cFEStateCreateStep1_cFEStateCreateStep1's 3rd argument (mHashName 0x4D when set, 0x50 when not), 1 in the default
                                    //       ctor; [U] onCreateScreen picks the form from it, onWidgetCreate/onWidgetEvent/func_001BA178 test it
    cUIText* mpStaticContinue;      // 0x6F0 [R] 0; [U] onWidgetCreate cFEStateCreateStep1_onWidgetCreate: widget "StaticContinue"; onGainTransition
                                    //       cFEStateCreateStep1_onGainTransition enables it (its slot 8) only when all four strings are set (func_001B9558)

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateCreateStep11() __asm__("cFEStateCreateStep1_cFEStateCreateStep11");
};

CHECK_OFFSET(cFEStateCreateStep1, mpAccount, 0x6D0);
CHECK_OFFSET(cFEStateCreateStep1, mszSavedPassword, 0x6D4);
CHECK_OFFSET(cFEStateCreateStep1, mszSavedConfirm, 0x6D8);
CHECK_OFFSET(cFEStateCreateStep1, mszSavedMail, 0x6DC);
CHECK_OFFSET(cFEStateCreateStep1, miSavedSpam, 0x6E0);
CHECK_OFFSET(cFEStateCreateStep1, unk_6E8, 0x6E8);
CHECK_OFFSET(cFEStateCreateStep1, mbEditAccount, 0x6EC);
CHECK_OFFSET(cFEStateCreateStep1, mpStaticContinue, 0x6F0);
CHECK_SIZE(cFEStateCreateStep1, 0x6F4);

struct cFEStateCreateStep2 : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateCreateStep2_vtbl (32 slots), stored by cFEStateCreateStep2_cFEStateCreateStep2 /
    //       cFEStateCreateStep2_cFEStateCreateStep21 and the destructor cFEStateCreateStep2__cFEStateCreateStep2
    // cFEStateCreateStep2's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateCreateStep2_cFEStateCreateStep2 stores 0x4E
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateCreateStep2
    tLobbyAccount* mpAccount;       // 0x6D0 [U] onCreateScreen cFEStateCreateStep2_onCreateScreen stores (char*)LobbyMan_mLobbyMan + 0x260 (online/lobbyman.h:
                                    //       the new-account form); the boxes read/write its mBornMonth/Day/Year (0x28/0x2C/0x30), mGender 0x34, 0x40
                                    //       (func_001BB2D0..func_001BB4B0, cFEStateCreateStep2_onWidgetEvent, func_001BB840); func_001BAED8 sends mName/mPassword
    int mbScreenReady;              // 0x6D4 [U] onCreateScreen cFEStateCreateStep2_onCreateScreen clears it first and sets it once every box is filled;
                                    //       onWidgetEvent cFEStateCreateStep2_onWidgetEvent stores box changes into mpAccount only while it is set
    int unk_6D8;                    // 0x6D8 [U] func_001BAED8 (popup events): 1 on 0x11 (option 2), 0 on 0xF/0x10/0x14; on 0x16 it opens
                                    //       "kT_POPUP204SaveAccount" when set, else "kT_POPUP201Verify" and submits the account (func_0025C610)
    int unk_6DC;                    // 0x6DC [U] onCreateScreen cFEStateCreateStep2_onCreateScreen stores 0; no read seen

    // Methods bound to their retail symbols by asm labels.
    void * cFEStateCreateStep21() __asm__("cFEStateCreateStep2_cFEStateCreateStep21");
};

CHECK_OFFSET(cFEStateCreateStep2, mpAccount, 0x6D0);
CHECK_OFFSET(cFEStateCreateStep2, mbScreenReady, 0x6D4);
CHECK_OFFSET(cFEStateCreateStep2, unk_6D8, 0x6D8);
CHECK_OFFSET(cFEStateCreateStep2, unk_6DC, 0x6DC);
CHECK_SIZE(cFEStateCreateStep2, 0x6E0);

#endif /* FE_FESTATENETCREATE_H */
