#ifndef FE_FESTATENETCREATEROOMPOPUP_H
#define FE_FESTATENETCREATEROOMPOPUP_H

// cFEStateNetCreateRoomPopup: the lobby's "create room" popup: channel list (beginner / advanced / elite / voice),
// room
// name and password via the keyboard popup; reports 0xF (done) or 0x14 (back) to its parent, cFEStateLobbyMenu
// (fe/festatelobbymenu.h), which then reads the three strings. A cFEStateNet state (fe/festatenet.h, 0x6D0). EA's
// name:
// the preview's __26cFEStateNetCreateRoomPopup and its inline accessors getRoomName / getChannelName / getPassword
// (preview offsets 0xB8 / 0xBC / 0xC0: + 0x624 = retail 0x6DC / 0x6E0 / 0x6E4). EA's source file is probably
// d:/ssx/code/source/fe/festatenetlobby.cpp (its code follows cFEStateLobbyMenu's in both builds; no assert of its
// own), so this header could also be merged into fe/festatelobbymenu.h.
//
// Size 0x6E8: cMemMan_alloc(0x6E8, "cFEStateNetCreateRoomPopup") + cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup(mem, pEngine, pParent)
// (cFEStateLobbyMenu's onWidgetEvent, the only site); mszPassword ends at 0x6E8. Constructors cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup
// (cFEStateNet(pEngine, pParent), vtable cFEStateNetCreateRoomPopup_vtbl, mHashName 0x47) and the default cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup1 (fe/fepopupmisc);
// destructor cFEStateNetCreateRoomPopup__cFEStateNetCreateRoomPopup (fe/fepopupmisc; slot 1: destroys the three strings). Retail vtable cFEStateNetCreateRoomPopup_vtbl (32 slots;
// vtables.tsv lists 24: 4 onCreateScreen cFEStateNetCreateRoomPopup_onCreateScreen, 10 cFEStateNetCreateRoomPopup_onLoseTransition, 12 onUpdate
// cFEStateNetCreateRoomPopup_onUpdate, 18 onWidgetCreate cFEStateNetCreateRoomPopup_onWidgetCreate, 19 onWidgetEvent cFEStateNetCreateRoomPopup_onWidgetEvent, 21 cFEStateNetCreateRoomPopup_onWidgetPair, 25
// cFEStateNetCreateRoomPopup_fillHelpPopup (help), 31 cFEStateNetCreateRoomPopup_onNetEvent). Methods: fe/feasyncfile 0x1BE7D8-0x1BED58.
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

struct cFEStateNetHelpPopup;

struct cFEStateNetCreateRoomPopup : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateNetCreateRoomPopup_vtbl (32 slots), stored by
    //       cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup /
    //       cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup1 after the cFEStateNet ctor and by the destructor
    //       cFEStateNetCreateRoomPopup__cFEStateNetCreateRoomPopup
    // cFEStateNetCreateRoomPopup's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup
    //       stores 0x47
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] cFEStateNet_onNestedChallengePopupEvent calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateNetCreateRoomPopup
    cUIText* mpTextRoomName;        // 0x6D0 [U] onWidgetCreate cFEStateNetCreateRoomPopup_onWidgetCreate: widget "EnterRoomName"; onWidgetEvent
                                    //       cFEStateNetCreateRoomPopup_onWidgetEvent edits it with the keyboard (cFEStateNet_openKeyboardWithLabelID,
                                    //       "kT_POPUP231NewRoom") and shows mszRoomName in it
    cUIText* mpTextPassword;        // 0x6D4 [U] onWidgetCreate: widget "EnterPassword"; onWidgetEvent edits it
                                    //       ("kT_POPUP231AddPassword") and shows mszPassword in it
    cUIListBox* mpLBChannelName;    // 0x6D8 [U] onWidgetCreate: widget "LBChannelName" (4 entries); onWidgetEvent
                                    //       event 9 compares the widget with it and sets mszChannelName from its
                                    //       selection (cUIListBox_getCurrentEntryData)
    char* mszRoomName;              // 0x6DC [P] getRoomName (preview 0xB8); [R] cBXString_EmptyString (both ctors), destroyed by
                                    //       cFEStateNetCreateRoomPopup__cFEStateNetCreateRoomPopup; [U] cBXString assigned from the keyboard text (event 13,
                                    //       keyboard id 1); cFEStateNetCreateRoomPopup_checkRoomName refuses an empty name
                                    //       ("kT_POPUP231UniqueRoom")
    char* mszChannelName;           // 0x6E0 [P] getChannelName (preview 0xBC); [R] cBXString_EmptyString, destroyed by
                                    //       cFEStateNetCreateRoomPopup__cFEStateNetCreateRoomPopup; [U] cBXString assigned D_004A1A6C by onWidgetCreate and
                                    //       one of D_004A1A60/64/6C/68 per list entry by onWidgetEvent
    char* mszPassword;              // 0x6E4 [P] getPassword (preview 0xC0); [R] cBXString_EmptyString, destroyed by
                                    //       cFEStateNetCreateRoomPopup__cFEStateNetCreateRoomPopup; [U] cBXString assigned from the keyboard text (event 13,
                                    //       keyboard id 2)

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateNetCreateRoomPopup1() __asm__("cFEStateNetCreateRoomPopup_cFEStateNetCreateRoomPopup1");
    char* getRoomName() __asm__("cFEStateNetCreateRoomPopup_getRoomName");
    char* getChannelName() __asm__("cFEStateNetCreateRoomPopup_getChannelName");
    char* getPassword() __asm__("cFEStateNetCreateRoomPopup_getPassword");
    // retail's destructor cFEStateNetCreateRoomPopup__cFEStateNetCreateRoomPopup stays a C function (g++ 2.95 takes no
    // asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateNetCreateRoomPopup();
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cFEStateNetCreateRoomPopup_onWidgetEvent");
    virtual bool onLoseTransition() __asm__("cFEStateNetCreateRoomPopup_onLoseTransition__FPv");
    virtual void onUpdate() __asm__("cFEStateNetCreateRoomPopup_onUpdate__FPv");
    virtual int onWidgetPair(cUIPair*, unsigned int) __asm__("cFEStateNetCreateRoomPopup_onWidgetPair__FPv");
    virtual void onWidgetCreate(cUIThing* pWidget) __asm__("cFEStateNetCreateRoomPopup_onWidgetCreate");
    bool checkRoomName() __asm__("cFEStateNetCreateRoomPopup_checkRoomName");
    void fillHelpPopup(cFEStateNetHelpPopup* popup) __asm__("cFEStateNetCreateRoomPopup_fillHelpPopup");
};

CHECK_OFFSET(cFEStateNetCreateRoomPopup, mpTextRoomName, 0x6D0);
CHECK_OFFSET(cFEStateNetCreateRoomPopup, mpTextPassword, 0x6D4);
CHECK_OFFSET(cFEStateNetCreateRoomPopup, mpLBChannelName, 0x6D8);
CHECK_OFFSET(cFEStateNetCreateRoomPopup, mszRoomName, 0x6DC);
CHECK_OFFSET(cFEStateNetCreateRoomPopup, mszChannelName, 0x6E0);
CHECK_OFFSET(cFEStateNetCreateRoomPopup, mszPassword, 0x6E4);
CHECK_SIZE(cFEStateNetCreateRoomPopup, 0x6E8);

#endif /* FE_FESTATENETCREATEROOMPOPUP_H */
