#ifndef UI_UIMENU_H
#define UI_UIMENU_H

// cUIMenu: a UI group whose children are selectable items (the "Menu0000" / "MenuFroms" widgets of the FE and overlay
// screens). EA's source is d:/ssx/code/source/ui/uimenu.cpp (preview assert path). Retail spreads its code over
// ui/uilistbox (cUIMenu_setFocus .. cUIMenu_getNext, cUIMenu_getPrev), ui/uimenu (cUIMenu_setSelected ..
// cUIMenu_setSelectedByIndex) and ui/uivector (the constructor cUIMenu_cUIMenu, the destructor cUIMenu__cUIMenu, getType
// and the out-of-line inline accessors func_003A5338 .. func_003A5418).
//
// Size 0xA8: cUIScreen_createObjectByStruct (record type 0x13) does operator new(0xA8, "UIMenu") and builds it inline
// (cUIThing ctor cUIThing_cUIThing, cUIGroup's vtable, cList ctor cList_cList at 0x74, cUIMenu's vtable, tAttrMenu ctor
// func_003A5338 at 0x90, then the stores below); cUIMenu_cUIMenu is the same constructor out of line. Retail vtable
// _vt$7cUIMenu = D_00494928 (cUIMenu_vtbl).
// Bases: cUIGroup (0x90 bytes: cUIThing 0x74 + cList mList 0x1C; ui/uithing.h, tag "cUIGroup" 0x90) <- cUIThing <-
// cListNode, written out flat below. The vptr is cUIThing's (0x8).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals.
// Member names are EA's, from SSX On Tour's PSP DWARF (class cUIMenu, size 0xC0: its cUIGroup is 0xA0, so every
// cUIMenu member sits exactly 0x10 later there), placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors (linkonce names isFlipInput / setFlipInput / getScrollIndex /
//       getScrollOffset / getScrollNum / setScrollIndex / setScrollNum / setScrollOffset / getSelected /
//       getSelectedIndex, in that declaration order). Retail keeps out-of-line copies of them in the same order in
//       ui/uivector (func_003A5350 .. func_003A5418), which places each one; retail adds get/set pairs for two more
//       tAttrMenu bits (func_003A5380/cUIMenu_setDisableDirSound bit 3, func_003A53B0/func_003A53C0 bit 0);
//   [R] retail's constructors (cUIMenu_cUIMenu and the inlined copy in cUIScreen_createObjectByStruct), the destructor
//       cUIMenu__cUIMenu and the 0xA8 allocation;
//   [U] how retail code uses the field;
//   [O] On Tour's member order and types alone, where the fields on both sides of it are placed by [P] or [R].
//
// mAttrMenu is EA's tAttrMenu (On Tour: 1 byte, unsigned char bitfields, LSB first), defined below. Its constructor
// func_003A5338 clears the 4 bits with a byte access (lbu/andi 0xF0/sb), while every accessor and the menu code
// read-modify-write the aligned word at 0x90 (lw/sw): g++ 2.95 does exactly that for a 1-byte bitfield struct
// embedded in a word-aligned object (SLOW_BYTE_ACCESS). Checked with ee-gcc 2.95.3 -O2: `p->mFlipInput = 0 ...` on a
// tAttrMenu* gives lbu/andi 0xf0/sb, `self->mAttrMenu.mFlipInput = enable != 0` gives retail func_003A5360's
// lw/sltu/li -3/sll/and/or/sw, and a bool getter gives lw/sra/andi (func_003A5350). mInputMap follows at 0x91.

#include "layout.h"
#include "ui/uithing.h"

struct cUIAnimation;
struct cUIScreen;
struct cListNode;
struct cUIScrollBar;

// EA's tAttrMenu (On Tour DWARF: struct tAttrMenu, size 0x1). Retail: tAttrMenu::tAttrMenu() is func_003A5338.
struct tAttrMenu {
    unsigned char mCreateThread : 1;    // bit 0 [U] func_003A53B0/func_003A53C0; passed to cUIMenu_setSelected by onInputChangeItem
    unsigned char mFlipInput : 1;       // bit 1 [P] isFlipInput/setFlipInput (func_003A5350/func_003A5360)
    unsigned char mScrollMenu : 1;      // bit 2 [O]; [U] cUIMenu_setScrollableMenu sets it; getNext / cUIMenu_getPrev /
                                        //       cUIMenu_findFirstHighlightable / cUIMenu_reverseFindFirstHighlightable / onInputChangeItem test it
    unsigned char mDisableDirSound : 1; // bit 3 [O]; [U] func_003A5380/cUIMenu_setDisableDirSound; cUIMenu_onInput tests it
};

CHECK_SIZE(tAttrMenu, 0x1);

struct cUIMenu {
    // --- cListNode / cUIThing (ui/uithing.h, 0x74)
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the cUIThing ctor self-links it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked
    void** mVtbl;                    // 0x008 [R] _vt$7cUIMenu D_00494928 (cUIMenu_vtbl), stored after cUIGroup's D_004949E8;
                                     //       the destructor stores cUIGroup's back
    cUIAnimation* mpAnim;            // 0x00C [P] cUIThing::getAnimation (ui/uithing.h)
    unsigned char mAnimPlaybackStatus; // 0x010 [O]; [R] = 0 (cUIThing ctor)
    char unk_011[0x3];               // 0x011 padding
    int mAttr;                       // 0x014 [P] cUIThing's tAttrThing word (lw/sw 0x14); [U] cUIMenu_setFocus,
                                     //       cUIMenu_getNext and cUIMenu_onInput test its bits
    int mMiscData;                   // 0x018 [P] cUIThing::getMiscData
    tARGB mColour;                   // 0x01C [P] cUIThing::setColour
    t3Vector mAngle;                 // 0x02C [O]; [P] cUIThing::getRadAngle (mAngle.z)
    int mHashName;                   // 0x038 [P] cUIThing::getHashName
    unsigned short* mpEvents;        // 0x03C [O]; [U] cUIThing_getKeyframerEvent
    cUIThing* mpParent;              // 0x040 [P] cUIThing::getParent; [R] = pParent (0 from cUIScreen_createObjectByStruct)
    t3Vector mPosition;              // 0x044 [P] cUIThing::getPosition; [U] cUIMenu_render2D
    t3Vector mScale;                 // 0x050 [P] cUIThing::getScale
    cUIScreen* mpScreen;             // 0x05C [P] cUIThing::getScreen; [U] cUIScreen_playFrame(mpScreen, ...) in cUIMenu_setSelected,
                                     //       cUIMenu_setFocus, cUIMenu_onInput
    t3Vector mSize;                  // 0x060 [P] cUIThing::getSize/setSize
    int mSound[2];                   // 0x06C [O]; [R] = {4, 6} (cUIThing ctor)
    // --- cUIGroup (0x90)
    char mList[0x1C];                // 0x074 [O] cList mList (cListNode mStart 0x74, mEnd 0x80, the list's vptr 0x8C): the menu
                                     //       items; [R] constructed by cList_cList(this + 0x74); the destructor stores
                                     //       cList's vtable at 0x8C and cListNode's (cListNode_vtbl) at 0x88 / 0x7C;
                                     //       [U] cList_size(this + 0x74) in cUIMenu_setScrollableMenu / setUIData
    // --- cUIMenu
    tAttrMenu mAttrMenu;             // 0x090 [O] tAttrMenu; [P] isFlipInput/setFlipInput (bit 1, lw/sw the word at 0x90;
                                     //       retail func_003A5350/func_003A5360); [R] tAttrMenu() func_003A5338 clears
                                     //       bits 0-3 (byte); [U] cUIMenu_getNext / onInput / setScrollableMenu (bit 2)
    signed char mInputMap[4];        // 0x091 [O] char mInputMap[4]; [U] cUIMenu_setInputMap stores 4 bytes (sb 0x91..0x94),
                                     //       cUIMenu_onInput reads them (lb)
    unsigned char mScrollItemCurrent; // 0x095 [P] getScrollIndex/setScrollIndex/getSelectedIndex (lbu/sb 0x95; retail
                                     //       func_003A53E0/func_003A53F8/func_003A5418); [R] = 0; [U] the selected row
                                     //       (fe/fepopupmisc reads/writes menu + 0x95)
    unsigned char mScrollItemNum;    // 0x096 [P] getScrollNum/setScrollNum (lbu/sb 0x96; retail func_003A53F0/func_003A5400);
                                     //       [R] = 0; [U] cUIMenu_setUIData / setScrollableMenu store the item count
    unsigned char mScrollItemNumVisible; // 0x097 [O]; [R] = 0; [U] cUIMenu_setUIData / setScrollableMenu store it,
                                     //       cUIMenu_render2D / onInputChangeItem / setSelectedByIndex read it
    unsigned char mScrollItemOffset; // 0x098 [P] getScrollOffset/setScrollOffset (lbu/sb 0x98; retail func_003A53E8/
                                     //       func_003A5408); [R] = 0
    char unk_099[0x3];               // 0x099 padding
    cUIScrollBar* mpScrollBar;       // 0x09C [O]; [R] = 0; [U] func_0039B7A8 stores it, cUIMenu_render2D reads it
    cUIThing* mpSelected;            // 0x0A0 [P] getSelected (lw 0xA0; retail func_003A5410); [R] = 0; [U] cUIMenu_setSelected
                                     //       stores the item, cUIMenu_setFocus / onInput read it
    unsigned char mStoredIndex;      // 0x0A4 [O]; [R] = 0xFF; [U] cUIMenu_setSelected / setSelectedByIndex store it (sb),
                                     //       cUIMenu_setFocus reads it (lbu)
    char unk_0A5[0x3];               // 0x0A5 padding (placed by the 0xA8 allocation)
};

CHECK_OFFSET(cUIMenu, mpPrev, 0x000);
CHECK_OFFSET(cUIMenu, mpNext, 0x004);
CHECK_OFFSET(cUIMenu, mVtbl, 0x008);
CHECK_OFFSET(cUIMenu, mpAnim, 0x00C);
CHECK_OFFSET(cUIMenu, mAnimPlaybackStatus, 0x010);
CHECK_OFFSET(cUIMenu, unk_011, 0x011);
CHECK_OFFSET(cUIMenu, mAttr, 0x014);
CHECK_OFFSET(cUIMenu, mMiscData, 0x018);
CHECK_OFFSET(cUIMenu, mColour, 0x01C);
CHECK_OFFSET(cUIMenu, mAngle, 0x02C);
CHECK_OFFSET(cUIMenu, mHashName, 0x038);
CHECK_OFFSET(cUIMenu, mpEvents, 0x03C);
CHECK_OFFSET(cUIMenu, mpParent, 0x040);
CHECK_OFFSET(cUIMenu, mPosition, 0x044);
CHECK_OFFSET(cUIMenu, mScale, 0x050);
CHECK_OFFSET(cUIMenu, mpScreen, 0x05C);
CHECK_OFFSET(cUIMenu, mSize, 0x060);
CHECK_OFFSET(cUIMenu, mSound, 0x06C);
CHECK_OFFSET(cUIMenu, mList, 0x074);
CHECK_OFFSET(cUIMenu, mAttrMenu, 0x090);
CHECK_OFFSET(cUIMenu, mInputMap, 0x091);
CHECK_OFFSET(cUIMenu, mScrollItemCurrent, 0x095);
CHECK_OFFSET(cUIMenu, mScrollItemNum, 0x096);
CHECK_OFFSET(cUIMenu, mScrollItemNumVisible, 0x097);
CHECK_OFFSET(cUIMenu, mScrollItemOffset, 0x098);
CHECK_OFFSET(cUIMenu, unk_099, 0x099);
CHECK_OFFSET(cUIMenu, mpScrollBar, 0x09C);
CHECK_OFFSET(cUIMenu, mpSelected, 0x0A0);
CHECK_OFFSET(cUIMenu, mStoredIndex, 0x0A4);
CHECK_OFFSET(cUIMenu, unk_0A5, 0x0A5);
CHECK_SIZE(cUIMenu, 0xA8);

#endif /* UI_UIMENU_H */
