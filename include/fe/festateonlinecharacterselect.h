#ifndef FE_FESTATEONLINECHARACTERSELECT_H
#define FE_FESTATEONLINECHARACTERSELECT_H

// cFEStateOnlineCharacterSelect: screen "245online_char_select": each player of an online game picks a rider
// ("LBChallengerSelect", 10 riders) and confirms; the screen shows the other players' picks. A cFEStateNet state
// (fe/festatenet.h, 0x6D0). EA's name: the preview's __29cFEStateOnlineCharacterSelect /
// _$_29cFEStateOnlineCharacterSelect. EA's source file is not known (its code follows
// cFEStateChallengeReceivedPopup's, fe/festatenetchallenge.h, in both builds; no assert names it).
//
// Size 0x6E4: cMemMan_alloc(0x6E4, "cFEStateOnlineCharacterSelect") + cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect(mem, pEngine) at all 7 sites
// (cFEStateNet_onMessengerPopupEvent, cFEStateChallengePopup_onNetEvent, cFEStateChallengeReceivedPopup_onNetEvent, cFEStateChatRoom_onChallengeSetupPopupEvent, cFEStateChatRoom_onChallengeReceivedPopupEvent, cFEStateOnlineMainMenu_onNetEvent, cFEStateNetQuickPlayPopup_onNetEvent).
// Constructors cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect (cFEStateNet(pEngine, 0), vtable cFEStateOnlineCharacterSelect_vtbl, mHashName 0x41) and the default
// cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect1 (fe/fepopupmisc); destructor cFEStateOnlineCharacterSelect__cFEStateOnlineCharacterSelect (fe/fepopupmisc; slot 1; the preview's
// _$_29cFEStateOnlineCharacterSelect). Retail vtable cFEStateOnlineCharacterSelect_vtbl (32 slots: 4 onCreateScreen cFEStateOnlineCharacterSelect_onCreateScreen, 7
// cFEStateOnlineCharacterSelect_onGainTransition, 8 cFEStateOnlineCharacterSelect_onInputBegin (input filter), 12 onUpdate cFEStateOnlineCharacterSelect_onUpdate, 17 onPopupEvent cFEStateOnlineCharacterSelect_onPopupEvent,
// 18 onWidgetCreate cFEStateOnlineCharacterSelect_onWidgetCreate, 19 onWidgetEvent cFEStateOnlineCharacterSelect_onWidgetEvent, 25 cFEStateOnlineCharacterSelect_fillHelpPopup (help), 31 cFEStateOnlineCharacterSelect_onNetEvent (net
// events)). Methods: fe/feasyncfile 0x1B0F60-0x1B1F18. The online game object cSetupMan_mSetupMan (cSetupMan_highlightCharacter /
// cSetupMan_confirmCharacter / cSetupMan_unconfirmCharacter) receives the picks.
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

struct cFEStateOnlineCharacterSelect : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateOnlineCharacterSelect_vtbl (vtables.tsv; 32 slots), stored by
    //       cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect /
    //       cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect1 after the cFEStateNet ctor and by the
    //       destructor cFEStateOnlineCharacterSelect__cFEStateOnlineCharacterSelect
    // cFEStateOnlineCharacterSelect's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R]
    //       cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect stores 0x41
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateOnlineCharacterSelect
    cUIListBox* mpLBChallengerSelect; // 0x6D0 [U] onWidgetCreate cFEStateOnlineCharacterSelect_onWidgetCreate: widget "LBChallengerSelect" (10 rider
                                    //       names from D_00440F68), preset to the current rider; its value
                                    //       (cUIListBox_getCurrentEntryData) is the picked rider in cFEStateOnlineCharacterSelect_onGainTransition / onWidgetEvent
                                    //       cFEStateOnlineCharacterSelect_onWidgetEvent
    int unk_6D4;                    // 0x6D4 no access seen
    int miConfirmCtrl;              // 0x6D8 [R] 0 (cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect); [U] onWidgetEvent cFEStateOnlineCharacterSelect_onWidgetEvent stores
                                    //       mCurrentCtrl (lb 0x14) when the pick is confirmed; no read seen
    int mbConfirmed;                // 0x6DC [R] 1 (cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect), 0 (cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect1); [U] cFEStateOnlineCharacterSelect_onGainTransition (slot 7)
                                    //       clears it; onWidgetEvent sets 1 when the pick is sent (cSetupMan_confirmCharacter) and
                                    //       clears it on "back" (cSetupMan_unconfirmCharacter), or asks "kT_POPUPCMNCancelChallenge"
                                    //       when clear; the input filter cFEStateOnlineCharacterSelect_onInputBegin blocks input while it is set
    int miCharID;                   // 0x6E0 [U] onCreateScreen cFEStateOnlineCharacterSelect_onCreateScreen clears it; onWidgetCreate stores the
                                    //       player's rider (cBENewPlayerInterface_getPlayerCharID); onWidgetEvent
                                    //       resets the player interface (cBENewPlayerInterface_setPlayerCheatCharID) when the pick differs from it

    // Methods bound to their retail symbols by asm labels.
    void * cFEStateOnlineCharacterSelect1() __asm__("cFEStateOnlineCharacterSelect_cFEStateOnlineCharacterSelect1");
    // retail's destructor cFEStateOnlineCharacterSelect__cFEStateOnlineCharacterSelect stays a C function (g++ 2.95
    // takes no asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateOnlineCharacterSelect();
    virtual void onPopupEvent(cUIState*pPopup, unsigned int Event)
        __asm__("cFEStateOnlineCharacterSelect_onPopupEvent");
    virtual void onCreateScreen() __asm__("cFEStateOnlineCharacterSelect_onCreateScreen");
    virtual bool onGainTransition(int bFirstTime) __asm__("cFEStateOnlineCharacterSelect_onGainTransition");
    virtual bool onInputBegin(cUIInput* obj) __asm__("cFEStateOnlineCharacterSelect_onInputBegin");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg)
        __asm__("cFEStateOnlineCharacterSelect_onWidgetEvent");
    virtual void onUpdate() __asm__("cFEStateOnlineCharacterSelect_onUpdate__FPv");
    virtual void onWidgetCreate(cUIThing* pWidget) __asm__("cFEStateOnlineCharacterSelect_onWidgetCreate");
};

CHECK_OFFSET(cFEStateOnlineCharacterSelect, mpLBChallengerSelect, 0x6D0);
CHECK_OFFSET(cFEStateOnlineCharacterSelect, unk_6D4, 0x6D4);
CHECK_OFFSET(cFEStateOnlineCharacterSelect, miConfirmCtrl, 0x6D8);
CHECK_OFFSET(cFEStateOnlineCharacterSelect, mbConfirmed, 0x6DC);
CHECK_OFFSET(cFEStateOnlineCharacterSelect, miCharID, 0x6E0);
CHECK_SIZE(cFEStateOnlineCharacterSelect, 0x6E4);

#endif /* FE_FESTATEONLINECHARACTERSELECT_H */
