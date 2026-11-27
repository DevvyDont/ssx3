#ifndef FE_FESTATECREATEDUPLICATE_H
#define FE_FESTATECREATEDUPLICATE_H

// cFEStateCreateDuplicate: screen "212create_duplicate": the server refused a new account name (mbAccount) or persona
// name; it lists the server's suggested names (tLobbyAccount mNameOptions) and lets the player pick one or type
// another, then resubmits. A cFEStateNet state (fe/festatenet.h, 0x6D0). EA's name: the preview's
// __23cFEStateCreateDuplicate / _$_23cFEStateCreateDuplicate. EA's source file is probably
// d:/ssx/code/source/fe/festatenetcreate.cpp (its code follows cFEStateCreateStep2's, see fe/festatenetcreate.h, in
// both builds; no assert of its own), so this header could also be merged into fe/festatenetcreate.h.
//
// Size 0x6E0: cMemMan_alloc(0x6E0, "cFEStateCreateDuplicate") + cFEStateCreateDuplicate_cFEStateCreateDuplicate(mem, pEngine, bAccount) at both sites
// (cFEStateSelectPersona_handlePersonaError with 0, cFEStateCreateStep2_handleAccountError). Constructors cFEStateCreateDuplicate_cFEStateCreateDuplicate (cFEStateNet(pEngine, 0), vtable cFEStateCreateDuplicate_vtbl,
// mHashName 0x4F, mNeedsLobby = 0) and the default cFEStateCreateDuplicate_cFEStateCreateDuplicate1 (fe/fepopupmisc); destructor cFEStateCreateDuplicate__cFEStateCreateDuplicate
// (fe/fepopupmisc;
// slot 1; the preview's _$_23cFEStateCreateDuplicate). Retail vtable cFEStateCreateDuplicate_vtbl (32 slots: 4 onCreateScreen
// cFEStateCreateDuplicate_onCreateScreen, 12 onUpdate cFEStateCreateDuplicate_onUpdate, 17 onPopupEvent cFEStateCreateDuplicate_onPopupEvent, 18 onWidgetCreate cFEStateCreateDuplicate_onWidgetCreate,
// 19 onWidgetEvent cFEStateCreateDuplicate_onWidgetEvent, 25 cFEStateCreateDuplicate_fillHelpPopup (help), 29 cFEStateCreateDuplicate_isOnlineState, 31 cFEStateCreateDuplicate_onNetEvent (net events)).
// Methods: fe/feasyncfile 0x1BBD08-0x1BC990.
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

struct tLobbyAccount;

struct cFEStateCreateDuplicate : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateCreateDuplicate_vtbl (vtables.tsv; 32 slots), stored by
    //       cFEStateCreateDuplicate_cFEStateCreateDuplicate / cFEStateCreateDuplicate_cFEStateCreateDuplicate1 after
    //       the cFEStateNet ctor and by the destructor cFEStateCreateDuplicate__cFEStateCreateDuplicate
    // cFEStateCreateDuplicate's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateCreateDuplicate_cFEStateCreateDuplicate
    //       stores 0x4F
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateCreateDuplicate
    int mbAccount;                  // 0x6D0 [R] the ctor's 3rd argument (0 from cFEStateSelectPersona cFEStateSelectPersona_handlePersonaError:
                                    //       a persona name; cFEStateCreateStep2_handleAccountError: the account name); [U] onWidgetCreate
                                    //       cFEStateCreateDuplicate_onWidgetCreate / onWidgetEvent cFEStateCreateDuplicate_onWidgetEvent / help cFEStateCreateDuplicate_fillHelpPopup pick the
                                    //       account or persona texts and actions by it
    int mbAskSave;                  // 0x6D4 [U] cFEStateCreateDuplicate_onAccountMemPopupEvent (popup 0x82C, the cFEStateNetAccountMem result): 1 on
                                    //       0x11, 0 on 0xF/0x10/0x14; on 0x16 it opens "kT_POPUP204SaveAccount"
                                    //       (0x81F) when set, else verifies the account (LobbyMan_authenticate); then clears
                                    //       it
    tLobbyAccount* mpAccount;       // 0x6D8 [U] onCreateScreen cFEStateCreateDuplicate_onCreateScreen stores (char*)LobbyMan_mLobbyMan + 0x260
                                    //       (the new-account form, online/lobbyman.h); cFEStateCreateDuplicate_showNameSuggestions shows its
                                    //       mNameOptions[0..mNumNameOptions) in "SelectName0".."3"; onWidgetEvent
                                    //       assigns the chosen option or the typed name to its mName; cFEStateCreateDuplicate_onSaveAccountPopupEvent /
                                    //       cFEStateCreateDuplicate_onAccountMemPopupEvent pass mName / mPassword on
    int unk_6DC;                    // 0x6DC no access seen (inside the 0x6E0 allocation)

    // Methods bound to their retail symbols by asm labels.
    void * cFEStateCreateDuplicate1() __asm__("cFEStateCreateDuplicate_cFEStateCreateDuplicate1");
    // retail's destructor cFEStateCreateDuplicate__cFEStateCreateDuplicate stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateCreateDuplicate();
    virtual void onCreateScreen() __asm__("cFEStateCreateDuplicate_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cFEStateCreateDuplicate_onWidgetEvent");
    virtual void onUpdate() __asm__("cFEStateCreateDuplicate_onUpdate__FPv");
    virtual void onPopupEvent(cUIState* pPopup, unsigned int Event) __asm__("cFEStateCreateDuplicate_onPopupEvent");
    virtual void onWidgetCreate(cUIThing* pWidget) __asm__("cFEStateCreateDuplicate_onWidgetCreate");
};

CHECK_OFFSET(cFEStateCreateDuplicate, mbAccount, 0x6D0);
CHECK_OFFSET(cFEStateCreateDuplicate, mbAskSave, 0x6D4);
CHECK_OFFSET(cFEStateCreateDuplicate, mpAccount, 0x6D8);
CHECK_OFFSET(cFEStateCreateDuplicate, unk_6DC, 0x6DC);
CHECK_SIZE(cFEStateCreateDuplicate, 0x6E0);

#endif /* FE_FESTATECREATEDUPLICATE_H */
