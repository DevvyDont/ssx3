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
// Constructors cUIStateBuyPopup_cUIStateBuyPopup (cUIState(pEngine, pParent) func_0039E318, vtable, mInfo.clear() twice, mUserNum,
// mHashName 7, mCtrlBits) and the default func_001DDC48; destructor cUIStateBuyPopup__cUIStateBuyPopup (stores
// D_0046D0D0, then cUIState's func_0039E390). Retail vtable cUIStateBuyPopup_vtbl = D_0046CB08 (24 slots: 1 dtor,
// 4 onCreateScreen, 6 cFEState_onGainFocus, 18 onWidgetCreate, 19 onWidgetEvent, 20 onWidgetMenu).
// Bases: cFEState (no members; On Tour size 0x48: its vtable D_0046D0D0 is the one ~20 FE destructors restore, and
// slot 6 is cFEState_onGainFocus), then cUIState (ui/uistate.h) and cListNode, written out flat below.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [O] On Tour's DWARF (cUIState / cUIStateBuyPopup);
//   [P] the July 2003 preview's inline accessors (cUIState: see ui/uistate.h; isAccept lw 0x68 at the preview's
//       offset, tools/layout_xlate.py: preview 0x58/0x5C = retail 0x5C/0x60);
//   [R] retail's constructors cUIStateBuyPopup_cUIStateBuyPopup / func_001DDC48 and the destructor;
//   [U] how retail code uses the field.

#include "layout.h"
#include "fe/buypopupinfo.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cUIStateBuyPopup {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cUIStateBuyPopup_vtbl D_0046CB08, stored by cUIStateBuyPopup_cUIStateBuyPopup / func_001DDC48;
                                    //       the destructor stores cFEState's D_0046D0D0
    // --- cUIState (On Tour: size 0x48, same offsets; see ui/uistate.h); cFEState adds no members
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName; [R] cUIStateBuyPopup_cUIStateBuyPopup stores 7
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] cUIEngine_addScreenByHashName
                                    //       (onCreateScreen); cUIStateBuyPopup_cUIStateBuyPopup reads *mpEngine for the controller bits
    signed char mCurrentCtrl;       // 0x014 [O]; [R] cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] cUIState ctor 0xF, then cUIStateBuyPopup_cUIStateBuyPopup stores
                                    //       func_001A1CD0(*mpEngine, userNum) when there is an input object
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] lw/sw the word; [R] ctor sh 0x218
    cUIState* mpParentState;        // 0x020 [O]; [R] the ctor's parent argument (the shop state)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the "139buy_popup" screen
    signed char mUserNum;           // 0x044 [O]; [R] cUIStateBuyPopup_cUIStateBuyPopup stores its userNum argument (sb)
    char unk_045[0x3];              // 0x045 padding
    // --- cUIStateBuyPopup
    cBuyPopupInfo mInfo;            // 0x048 [O]; [R] clear (cBuyPopupInfo_clear) in both ctors; [U] getPopupInfo func_001DDCB8
                                    //       returns &mInfo; initBuyTrick / initBuyAttributes (cUIStateBuyPopup_initBuyAttributes) and the
                                    //       inline initBuySong* (func_001DDCC8/DCE8/DD08) fill it; onWidgetCreate shows it
    int mbAccept;                   // 0x06C [O] bool mAccept (1 byte in On Tour; retail lw/sw the word);
                                    //       [U] onWidgetEvent stores 1 for "yes", else 0; isAccept func_001DDCC0 returns it
};

CHECK_OFFSET(cUIStateBuyPopup, mpPrev, 0x000);
CHECK_OFFSET(cUIStateBuyPopup, mpNext, 0x004);
CHECK_OFFSET(cUIStateBuyPopup, mVtbl, 0x008);
CHECK_OFFSET(cUIStateBuyPopup, mHashName, 0x00C);
CHECK_OFFSET(cUIStateBuyPopup, mpEngine, 0x010);
CHECK_OFFSET(cUIStateBuyPopup, mCurrentCtrl, 0x014);
CHECK_OFFSET(cUIStateBuyPopup, mCtrlBits, 0x015);
CHECK_OFFSET(cUIStateBuyPopup, unk_016, 0x016);
CHECK_OFFSET(cUIStateBuyPopup, miMiscData, 0x018);
CHECK_OFFSET(cUIStateBuyPopup, mAttrState, 0x01C);
CHECK_OFFSET(cUIStateBuyPopup, mpParentState, 0x020);
CHECK_OFFSET(cUIStateBuyPopup, mScreens, 0x024);
CHECK_OFFSET(cUIStateBuyPopup, mpScreen, 0x040);
CHECK_OFFSET(cUIStateBuyPopup, mUserNum, 0x044);
CHECK_OFFSET(cUIStateBuyPopup, unk_045, 0x045);
CHECK_OFFSET(cUIStateBuyPopup, mInfo, 0x048);
CHECK_OFFSET(cUIStateBuyPopup, mbAccept, 0x06C);
CHECK_SIZE(cUIStateBuyPopup, 0x70);

#endif /* FE_UISTATEBUYPOPUP_H */
