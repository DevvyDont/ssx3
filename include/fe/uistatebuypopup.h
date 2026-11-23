#ifndef FE_UISTATEBUYPOPUP_H
#define FE_UISTATEBUYPOPUP_H

// cUIStateBuyPopup: the "buy this item?" popup of the shops and the message center (screen "139buy_popup": item
// type / name, question, cost or remaining credits, yes / no). EA's name from the preview's linkonce sections
// (__16cUIStateBuyPopup, _$_16cUIStateBuyPopup, getPopupInfo__16cUIStateBuyPopup, isAccept__16cUIStateBuyPopup,
// initBuySong__16cUIStateBuyPopupPCcii ...) and SSX On Tour's DWARF (class cUIStateBuyPopup, size 0x70: mInfo at
// 0x48, bool mAccept at 0x6C). EA's source: all_in_one_fe.cpp in On Tour; retail fe/fepopup 0x1CAC30-0x1CAFC0 and
// main/all_in_one_main 0x1DDC48-0x1DDD08.
//
// Size 0x70: cMemMan_alloc(0x70, "cUIStateBuyPopup" / "UITRICKBUY") + cUIStateBuyPopup_cUIStateBuyPopup(mem, pEngine, pParent,
// userNum) at all 5 sites (fe/messagecenter, fe/festatecharequipdetail, fe/festatebuyattrib, fe/festatestore).
// Constructors cUIStateBuyPopup_cUIStateBuyPopup (cUIState(pEngine, pParent) cUIState_cUIState1, vtable, mInfo.clear() twice, mUserNum,
// mHashName 7, mCtrlBits) and the default cUIStateBuyPopup_cUIStateBuyPopup1; destructor cUIStateBuyPopup__cUIStateBuyPopup (stores
// cFEState_vtbl, then cUIState's cUIState__cUIState). Retail vtable cUIStateBuyPopup_vtbl = D_0046CB08 (24 slots: 1 dtor,
// 4 onCreateScreen, 6 cFEState_onGainFocus, 18 onWidgetCreate, 19 onWidgetEvent, 20 onWidgetMenu).
// Bases: cFEState (no members; On Tour size 0x48: its vtable cFEState_vtbl is the one ~20 FE destructors restore, and
// slot 6 is cFEState_onGainFocus), then cUIState (ui/uistate.h) and cListNode, written out flat below.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Evidence tags:
//   [O] On Tour's DWARF (cUIState / cUIStateBuyPopup);
//   [P] the July 2003 preview's inline accessors (cUIState: see ui/uistate.h; isAccept lw 0x68 at the preview's
//       offset, tools/layout_xlate.py: preview 0x58/0x5C = retail 0x5C/0x60);
//   [R] retail's constructors cUIStateBuyPopup_cUIStateBuyPopup / cUIStateBuyPopup_cUIStateBuyPopup1 and the destructor;
//   [U] how retail code uses the field.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/buypopupinfo.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cUIStateBuyPopup : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cUIStateBuyPopup_vtbl D_0046CB08, stored by cUIStateBuyPopup_cUIStateBuyPopup /
    //       cUIStateBuyPopup_cUIStateBuyPopup1; the destructor stores cFEState's cFEState_vtbl
    // cUIStateBuyPopup's notes on cUIState's fields:
    //   mHashName: [P] cUIState::getHashName; [R] cUIStateBuyPopup_cUIStateBuyPopup stores 7
    //   mpEngine: [R] the ctor's engine argument; [U] cUIEngine_addScreenByHashName (onCreateScreen);
    //       cUIStateBuyPopup_cUIStateBuyPopup reads *mpEngine for the controller bits
    //   mCtrlBits: [R] cUIState ctor 0xF, then cUIStateBuyPopup_cUIStateBuyPopup stores
    //       cFECustom_getPlayerCtrlMask(*mpEngine, userNum) when there is an input object
    //   mpScreen: [U] onCreateScreen stores the "139buy_popup" screen
    //   mUserNum: [R] cUIStateBuyPopup_cUIStateBuyPopup stores its userNum argument (sb)
    // --- cUIStateBuyPopup
    cBuyPopupInfo mInfo;            // 0x048 [O]; [R] clear (cBuyPopupInfo_clear) in both ctors; [U] getPopupInfo cUIStateBuyPopup_getPopupInfo
                                    //       returns &mInfo; initBuyTrick / initBuyAttributes (cUIStateBuyPopup_initBuyAttributes) and the
                                    //       inline initBuySong* (cUIStateBuyPopup_initBuySongByCredit/DCE8/DD08) fill it; onWidgetCreate shows it
    int mbAccept;                   // 0x06C [O] bool mAccept (1 byte in On Tour; retail lw/sw the word);
                                    //       [U] onWidgetEvent stores 1 for "yes", else 0; isAccept cUIStateBuyPopup_isAccept returns it
};

CHECK_OFFSET(cUIStateBuyPopup, mInfo, 0x048);
CHECK_OFFSET(cUIStateBuyPopup, mbAccept, 0x06C);
CHECK_SIZE(cUIStateBuyPopup, 0x70);

#endif /* FE_UISTATEBUYPOPUP_H */
