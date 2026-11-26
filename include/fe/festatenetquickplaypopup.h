#ifndef FE_FESTATENETQUICKPLAYPOPUP_H
#define FE_FESTATENETQUICKPLAYPOPUP_H

// cFEStateNetQuickPlayPopup: the "quick match" popup of the online main menu: five yes/no options (ranked, race, big
// air, superpipe, slopestyle) and an OK / Cancel button that starts or aborts the match search. A cFEStateNet state
// (fe/festatenet.h, 0x6D0). EA's name: the preview's __25cFEStateNetQuickPlayPopup, _$_25cFEStateNetQuickPlayPopup
// and
// onLoseTransition__25cFEStateNetQuickPlayPopup. EA's source file is not known (its code follows
// cFEStateOnlineAgreement's, after cFEStateOnlineMainMenu's festatenetmainmenu.cpp, in both builds).
//
// Size 0x734: cMemMan_alloc(0x734, "cFEStateNetQuickPlayPopup") + cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup(mem, pEngine, pParent)
// (cFEStateOnlineMainMenu_onWidgetEvent,
// cFEStateOnlineMainMenu's onWidgetEvent; the only site); the constructor zeroes up to 0x730. Constructors
// cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup (cFEStateNet(pEngine, pParent), vtable cFEStateNetQuickPlayPopup_vtbl, mHashName 0x48) and the default cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup1
// (fe/fepopupmisc); destructor cFEStateNetQuickPlayPopup__cFEStateNetQuickPlayPopup (fe/fepopupmisc; slot 1; the preview's _$_25cFEStateNetQuickPlayPopup).
// Retail vtable cFEStateNetQuickPlayPopup_vtbl (32 slots: 4 onCreateScreen cFEStateNetQuickPlayPopup_onCreateScreen, 10 cFEStateNetQuickPlayPopup_onLoseTransition, 12
// onUpdate cFEStateNetQuickPlayPopup_onUpdate, 17 onPopupEvent cFEStateNetQuickPlayPopup_onPopupEvent, 18 onWidgetCreate cFEStateNetQuickPlayPopup_onWidgetCreate, 19 onWidgetEvent
// cFEStateNetQuickPlayPopup_onWidgetEvent, 20 cFEStateNetQuickPlayPopup_onWidgetMenu, 21 cFEStateNetQuickPlayPopup_onWidgetPair, 25 func_001C1488 (help), 31 func_001C19C8 (net events)).
// Methods: fe/feasyncfile 0x1C0F40-0x1C1B88.
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
struct cUIListBox;

struct cFEStateNetQuickPlayPopup : public cFEStateNet {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateNetQuickPlayPopup_vtbl (vtables.tsv; 32 slots), stored by
    //       cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup / cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup1
    //       after the cFEStateNet ctor and by the destructor cFEStateNetQuickPlayPopup__cFEStateNetQuickPlayPopup
    // cFEStateNetQuickPlayPopup's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName (ui/uistate.h); [R] cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup
    //       stores 0x48
    //   mpEngine: [R] the ctor's engine argument; [U] passed to every child state's ctor (cMemMan_alloc(..) +
    //       ctor(mem, mpEngine)); mpEngine + 0x18 = the state stack
    //   mpParentState: [U] func_001A92D8 calls its slot 17 (onPopupEvent)
    //   mpScreen: [U] cUIEngine_addScreenByHashName result (onCreateScreen overrides),
    //       cUIScreen_getObjectByHashName(mpScreen, ...) everywhere
    //   mUserNum: [R] cUIState ctor -1, then cFEStateNet_cFEStateNet stores 0 (sb)
    // cFEStateNet (fe/festatenet.h, 0x6D0)
    // --- cFEStateNetQuickPlayPopup
    int miSearchOptions[5];         // 0x6D0 [U] onWidgetEvent cFEStateNetQuickPlayPopup_onWidgetEvent stores the five list boxes' values ([0]
                                    //       mpLBRanked .. [4] mpLBSlopeStyle) when the search starts and passes the
                                    //       array to func_0025EE20(LobbyMan, miSearchOptions, mCurrentCtrl)
    cUIText* mpTextSearching;       // 0x6E4 [U] onWidgetCreate cFEStateNetQuickPlayPopup_onWidgetCreate: widget "TextSearching" (hidden); shown
                                    //       while searching, "kT_233AbortingQuickmatch" on abort; func_001C1B88 hides
                                    //       it
    int miBlinkFrames;              // 0x6E8 [U] onWidgetEvent sets 30 when the search starts; onUpdate cFEStateNetQuickPlayPopup_onUpdate
                                    //       counts it down while mbSearching and wraps to 30
    int mbSearching;                // 0x6EC [R] 0 (both ctors); [U] onWidgetEvent: 1 when OK starts the search, the
                                    //       OK button then aborts it (func_0025EED8); func_001C1B88 (popup 0xF/0x14)
                                    //       and the net-event handler func_001C19C8 (0xE7) clear it; message 7 passes
                                    //       to cFEStateNet only while it is 0
    int mbQuickMatchTimeout;        // 0x6F0 (name ours) [U] onWidgetEvent clears it when the search starts; the
                                    //        net-event handler func_001C19C8 sets 1 on 0xE6 and, on 0xE7, opens
                                    //        "kT_POPUP233QuickMatchTimeout" when set, else ends the search
    cUIText* mpTextOK;              // 0x6F4 [U] onWidgetCreate: widget "TextOK"; onWidgetEvent compares the selected
                                    //       widget with it and relabels it "kT_BTNCancel" while searching ("kT_BTNOK"
                                    //       again in func_001C1B88)
    cUIThing* mpStaticRanked;       // 0x6F8 [R] 0; [U] onWidgetCreate: widget "StaticRanked"; func_001C17F8(self, on)
                                    //       calls slot 8 of this and the 14 widgets after it
    cUIThing* mpStaticRace;         // 0x6FC [R] 0; [U] widget "StaticRace"
    cUIThing* mpStaticBigAir;       // 0x700 [R] 0; [U] widget "StaticBigAir"
    cUIThing* mpStaticSuperpipe;    // 0x704 [R] 0; [U] widget "StaticSuperpipe"
    cUIThing* mpStaticSlopeStyle;   // 0x708 [R] 0; [U] widget "StaticSlopeStyle"
    cUIListBox* mpLBRanked;         // 0x70C [R] 0; [U] widget "LBRanked" (yes / no, func_001A9AB8); its value is
                                    //       miSearchOptions[0] and D_00534B34 (ranked)
    cUIListBox* mpLBRace;           // 0x710 [R] 0; [U] widget "LBRace"; miSearchOptions[1]
    cUIListBox* mpLBBigAir;         // 0x714 [R] 0; [U] widget "LBBigAir"; miSearchOptions[2]
    cUIListBox* mpLBSuperpipe;      // 0x718 [R] 0; [U] widget "LBSuperpipe"; miSearchOptions[3]
    cUIListBox* mpLBSlopeStyle;     // 0x71C [R] 0; [U] widget "LBSlopeStyle"; miSearchOptions[4]
    cUIThing* mpRankedPair;         // 0x720 [R] 0; [U] widget "RankedPair"
    cUIThing* mpRacePair;           // 0x724 [R] 0; [U] widget "RacePair"
    cUIThing* mpBigAirPair;         // 0x728 [R] 0; [U] widget "BigAirPair"
    cUIThing* mpSuperpipePair;      // 0x72C [R] 0; [U] widget "SuperpipePair"
    cUIThing* mpSlopestylePair;     // 0x730 [R] 0; [U] widget "SlopestylePair"

    // Methods bound to their retail symbols by asm labels.
    void * cFEStateNetQuickPlayPopup1() __asm__("cFEStateNetQuickPlayPopup_cFEStateNetQuickPlayPopup1");
    // retail's destructor cFEStateNetQuickPlayPopup__cFEStateNetQuickPlayPopup stays a C function (g++ 2.95 takes no
    // asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateNetQuickPlayPopup();
    virtual void onCreateScreen() __asm__("cFEStateNetQuickPlayPopup_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* obj) __asm__("cFEStateNetQuickPlayPopup_onWidgetCreate");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cFEStateNetQuickPlayPopup_onWidgetEvent");
};

CHECK_OFFSET(cFEStateNetQuickPlayPopup, miSearchOptions, 0x6D0);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpTextSearching, 0x6E4);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, miBlinkFrames, 0x6E8);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mbSearching, 0x6EC);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mbQuickMatchTimeout, 0x6F0);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpTextOK, 0x6F4);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpStaticRanked, 0x6F8);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpStaticRace, 0x6FC);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpStaticBigAir, 0x700);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpStaticSuperpipe, 0x704);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpStaticSlopeStyle, 0x708);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpLBRanked, 0x70C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpLBRace, 0x710);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpLBBigAir, 0x714);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpLBSuperpipe, 0x718);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpLBSlopeStyle, 0x71C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpRankedPair, 0x720);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpRacePair, 0x724);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpBigAirPair, 0x728);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpSuperpipePair, 0x72C);
CHECK_OFFSET(cFEStateNetQuickPlayPopup, mpSlopestylePair, 0x730);
CHECK_SIZE(cFEStateNetQuickPlayPopup, 0x734);

#endif /* FE_FESTATENETQUICKPLAYPOPUP_H */
