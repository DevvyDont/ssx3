#ifndef FE_FESTATENETACCOUNT_H
#define FE_FESTATENETACCOUNT_H

// The online account screens (EA's d:/ssx/code/source/fe/festatenetaccount.cpp: the preview's assert at line 852
// sits in retail cFEStateSelectPersona_onDeletePersonaPopupEvent, a cFEStateSelectPersona popup handler; cFEUseExistingAccount's code comes right
// before it in both builds, after cFEBlankPopup). Two cFEStateNet states (fe/festatenet.h, 0x6D0) live here:
//
// cFEUseExistingAccount: screen "204use_existing_account": account name and password boxes, "forgot password" /
// "forgot account" requests (by e-mail) and the verify step, then the memory-card save popup
// (cFEStateNetAccountMem) and cFEStateSelectPersona. EA's name: the preview's __21cFEUseExistingAccount and
// _vt$21cFEUseExistingAccount. Size 0x6E4: cMemMan_alloc(0x6E4, "cFEUseExistingAccount") + cFEUseExistingAccount_cFEUseExistingAccount(mem,
// pEngine) (cFEStateLogin_onWidgetEvent, the only site). Constructors cFEUseExistingAccount_cFEUseExistingAccount (cFEStateNet(pEngine, 0), vtable cFEUseExistingAccount_vtbl,
// mHashName 0x35) and the default cFEUseExistingAccount_cFEUseExistingAccount1 (fe/fepopupmisc); destructor cFEUseExistingAccount__cFEUseExistingAccount (slot 1: destroys
// mszEmail, then cFEStateNet's cFEStateNet__cFEStateNet). Retail vtable cFEUseExistingAccount_vtbl (32 slots: 1 dtor, 4 onCreateScreen
// cFEUseExistingAccount_onCreateScreen, 12 onUpdate cFEUseExistingAccount_onUpdate, 17 onPopupEvent cFEUseExistingAccount_onPopupEvent, 18 onWidgetCreate cFEUseExistingAccount_onWidgetCreate,
// 19 onWidgetEvent cFEUseExistingAccount_onWidgetEvent, 20 cFEUseExistingAccount_onWidgetMenu, 25 cFEUseExistingAccount_fillHelpPopup (help texts), 29 cFEUseExistingAccount_isOnlineState,
// 31 cFEUseExistingAccount_onNetEvent (net events 0xC9/0xCF/0xD2/0xD4)). Methods: fe/feasyncfile 0x1AC010-0x1ACEF8.
//
// cFEStateSelectPersona: screen "220select_persona": the account's persona list ("PersonaMenu"), create / delete
// persona. EA's name: the preview's __21cFEStateSelectPersona / _$_21cFEStateSelectPersona. Size 0x6D8:
// cMemMan_alloc(0x6D8, "cFEStateSelectPersona") + cFEStateSelectPersona_cFEStateSelectPersona(mem, pEngine) at all 10 sites (fe/feasyncfile).
// Constructors cFEStateSelectPersona_cFEStateSelectPersona (vtable cFEStateSelectPersona_vtbl, mHashName 0x38) and the default cFEStateSelectPersona_cFEStateSelectPersona1 (fe/fepopupmisc);
// destructor cFEStateSelectPersona__cFEStateSelectPersona (fe/fepopupmisc; slot 1; the preview's _$_21cFEStateSelectPersona). Retail vtable
// cFEStateSelectPersona_vtbl (33 slots: 4 onCreateScreen cFEStateSelectPersona_onCreateScreen, 12 onUpdate cFEStateSelectPersona_onUpdate, 17 onPopupEvent cFEStateSelectPersona_onPopupEvent,
// 18 onWidgetCreate cFEStateSelectPersona_onWidgetCreate, 19 onWidgetEvent cFEStateSelectPersona_onWidgetEvent, 25 cFEStateSelectPersona_fillHelpPopup (help), 29 cFEStateSelectPersona_isOnlineState,
// 31 cFEStateSelectPersona_onNetEvent (net events), 32 cFEStateSelectPersona_publishPresence, 33 cFEStateSelectPersona_onDeletePersonaPopupEvent). Methods: fe/feasyncfile 0x1AD130-0x1ADC30.
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

struct cUIThing;
struct cUIText;

struct cFEStateNetHelpPopup;
struct cScreenPopup;
struct cUIState;

struct cFEUseExistingAccount : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEUseExistingAccount_vtbl (vtables.tsv; 32 slots), stored by cFEUseExistingAccount_cFEUseExistingAccount
    //       / cFEUseExistingAccount_cFEUseExistingAccount1 after the cFEStateNet ctor and by the destructor
    //       cFEUseExistingAccount__cFEUseExistingAccount
    // cFEUseExistingAccount's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEUseExistingAccount_cFEUseExistingAccount stores
    //       0x35
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEUseExistingAccount
    int unk_6D0;                    // 0x6D0 [R] 0 (cFEUseExistingAccount_cFEUseExistingAccount, cFEUseExistingAccount_cFEUseExistingAccount1); no other access seen
    int mbAskSave;                  // 0x6D4 [U] cFEUseExistingAccount_onAccountMemPopupEvent (popup 0x82C, the cFEStateNetAccountMem result): 1 on
                                    //       0x11 (option 2), 0 on 0xF/0x10 (then pushes cFEStateSelectPersona); on
                                    //       0x16 it opens the "kT_POPUP204SaveAccount" yes/no popup (0x81E) when set,
                                    //       then clears it
    int miRequest;                  // 0x6D8 [U] the pending LobbyMan request: onWidgetEvent cFEUseExistingAccount_onWidgetEvent stores 4
                                    //       (verify, "kT_POPUP201Verify") and 2 (forgotten password), cFEUseExistingAccount_onEmailEntryPopupEvent
                                    //       stores 3 (forgotten account name); the net-event handler cFEUseExistingAccount_onNetEvent on
                                    //       0xC9 sends it: 2 -> LobbyMan_requestLostPassword(mUserName), 3 ->
                                    //       LobbyMan_requestLostAccount(mszEmail), 4 -> LobbyMan_authenticate
    char* mszEmail;                 // 0x6DC [R] cBXString: the empty-string rep cBXString_EmptyString (both ctors), destroyed by
                                    //       cFEUseExistingAccount__cFEUseExistingAccount (cBXString__cBXString(.., 2)); [U] cFEUseExistingAccount_onEmailEntryPopupEvent assigns
                                    //       the e-mail address the request popup returned (popup + 0x2B0;
                                    //       cBXString_operatorE1); cFEUseExistingAccount_checkEmailEntered opens "kT_POPUP204RequestAccount"
                                    //       when its length (the word before the text) is 0; cFEUseExistingAccount_onNetEvent passes it
                                    //       to LobbyMan_requestLostAccount
    cUIText* mpTextForgotPassword;  // 0x6E0 [R] 0; [U] onWidgetCreate cFEUseExistingAccount_onWidgetCreate: widget
                                    //       "TextForgotPassword"
                                    //       (miMiscData 2), then its slots 25 and 8 with 1; cFEUseExistingAccount_handleKeyboardText calls its
                                    //       slot 8 with 0 once an account name is entered, 1 when the name is empty

    // Methods bound to their retail symbols by asm labels.
    void* cFEUseExistingAccount1() __asm__("cFEUseExistingAccount_cFEUseExistingAccount1");
    // retail's destructor cFEUseExistingAccount__cFEUseExistingAccount stays a C function (g++ 2.95 takes no asm label
    // on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEUseExistingAccount();
    virtual void onCreateScreen() __asm__("cFEUseExistingAccount_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cFEUseExistingAccount_onWidgetEvent");
    virtual void onUpdate() __asm__("cFEUseExistingAccount_onUpdate__FPv");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int)
        __asm__("cFEUseExistingAccount_onWidgetMenu");
    virtual void onPopupEvent(cUIState* pPopup, unsigned int Event) __asm__("cFEUseExistingAccount_onPopupEvent");
    virtual void onWidgetCreate(cUIThing* pWidget) __asm__("cFEUseExistingAccount_onWidgetCreate");
    bool checkPasswordEntered() __asm__("cFEUseExistingAccount_checkPasswordEntered");
    bool checkAccountNameEntered() __asm__("cFEUseExistingAccount_checkAccountNameEntered");
    int checkLoginFields() __asm__("cFEUseExistingAccount_checkLoginFields");
    bool checkEmailEntered() __asm__("cFEUseExistingAccount_checkEmailEntered");
    void fillHelpPopup(cFEStateNetHelpPopup* popup) __asm__("cFEUseExistingAccount_fillHelpPopup");
    void onEmailEntryPopupEvent(cScreenPopup* popup, int event) __asm__("cFEUseExistingAccount_onEmailEntryPopupEvent");
    void onVerifyingPopupEvent(cUIState* popup, int event) __asm__("cFEUseExistingAccount_onVerifyingPopupEvent");
    void onAccountMemPopupEvent(cUIState* popup, int kind) __asm__("cFEUseExistingAccount_onAccountMemPopupEvent");
    void onSaveAccountPopupEvent(cUIState* popup, int kind) __asm__("cFEUseExistingAccount_onSaveAccountPopupEvent");
    void handleKeyboardText() __asm__("cFEUseExistingAccount_handleKeyboardText");
    int onNetEvent(int id, void* data) __asm__("cFEUseExistingAccount_onNetEvent");
    int isOnlineState() __asm__("cFEUseExistingAccount_isOnlineState__FPv");
};

CHECK_OFFSET(cFEUseExistingAccount, unk_6D0, 0x6D0);
CHECK_OFFSET(cFEUseExistingAccount, mbAskSave, 0x6D4);
CHECK_OFFSET(cFEUseExistingAccount, miRequest, 0x6D8);
CHECK_OFFSET(cFEUseExistingAccount, mszEmail, 0x6DC);
CHECK_OFFSET(cFEUseExistingAccount, mpTextForgotPassword, 0x6E0);
CHECK_SIZE(cFEUseExistingAccount, 0x6E4);

struct cFEStateNetHelpPopup;

struct cFEStateSelectPersona : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateSelectPersona_vtbl (vtables.tsv; 33 slots), stored by cFEStateSelectPersona_cFEStateSelectPersona
    //       / cFEStateSelectPersona_cFEStateSelectPersona1 after the cFEStateNet ctor and by the destructor
    //       cFEStateSelectPersona__cFEStateSelectPersona
    // cFEStateSelectPersona's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateSelectPersona_cFEStateSelectPersona stores
    //       0x38
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateSelectPersona
    int mbExistingPersona;          // 0x6D0 [R] 0 (cFEStateSelectPersona_cFEStateSelectPersona; not set by the default ctor); [U] onWidgetEvent
                                    //       cFEStateSelectPersona_onWidgetEvent: 1 when an existing persona row is chosen (LobbyMan_selectPersonaAt
                                    //       selects it), 0 when the "create new persona" row opens the keyboard; the
                                    //       net-event handler cFEStateSelectPersona_handlePersonaError picks "kT_POPUPCMNProfanity" (set) or
                                    //       "kT_POPUPCMNProfanityPersona" (clear) on the "filt" reply
    int miDeletePersona;            // 0x6D4 [U] onWidgetEvent cFEStateSelectPersona_onWidgetEvent (message 8, the delete key) stores the
                                    //       persona row and opens "kT_POPUP220DeleteInitial"; cFEStateSelectPersona_onDeletePersonaPopupEvent (popup
                                    //       0x816, option 0xF) deletes it: LobbyMan_deletePersona(LobbyMan, it) (the C body
                                    //       reads it as a void*)

    // Methods bound to their retail symbols by asm labels.
    void * cFEStateSelectPersona1() __asm__("cFEStateSelectPersona_cFEStateSelectPersona1");
    // retail's destructor cFEStateSelectPersona__cFEStateSelectPersona stays a C function (g++ 2.95 takes no asm label
    // on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateSelectPersona();
    virtual void onCreateScreen() __asm__("cFEStateSelectPersona_onCreateScreen");
    virtual void onUpdate() __asm__("cFEStateSelectPersona_onUpdate__FPv");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int Event) __asm__("cFEStateSelectPersona_onWidgetEvent");
    virtual void onPopupEvent(cUIState* pPopup, unsigned int Event) __asm__("cFEStateSelectPersona_onPopupEvent");
    virtual void onWidgetCreate(cUIThing* pWidget) __asm__("cFEStateSelectPersona_onWidgetCreate");
    void createPersona(const char* name) __asm__("cFEStateSelectPersona_createPersona");
    void fillPersonaMenu(cUIText* text) __asm__("cFEStateSelectPersona_fillPersonaMenu");
    void fillHelpPopup(cFEStateNetHelpPopup* popup) __asm__("cFEStateSelectPersona_fillHelpPopup");
    void onMessengerErrorPopupEvent(void* popup, int event) __asm__("cFEStateSelectPersona_onMessengerErrorPopupEvent");
    void onDeletePersonaPopupEvent(void* popup, int event) __asm__("cFEStateSelectPersona_onDeletePersonaPopupEvent");
    int onNetEvent(int msgId, void* data) __asm__("cFEStateSelectPersona_onNetEvent");
    void handlePersonaError(void* msgPtr) __asm__("cFEStateSelectPersona_handlePersonaError");
    int isOnlineState() __asm__("cFEStateSelectPersona_isOnlineState__FPv");
};

CHECK_OFFSET(cFEStateSelectPersona, mbExistingPersona, 0x6D0);
CHECK_OFFSET(cFEStateSelectPersona, miDeletePersona, 0x6D4);
CHECK_SIZE(cFEStateSelectPersona, 0x6D8);

#endif /* FE_FESTATENETACCOUNT_H */
