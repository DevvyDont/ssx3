#ifndef UI_UIMENU_H
#define UI_UIMENU_H

// cUIMenu: a UI group whose children are selectable items (the "Menu0000" / "MenuFroms" widgets of the FE and overlay
// screens). EA's source is d:/ssx/code/source/ui/uimenu.cpp (preview assert path). Retail spreads its code over
// ui/uilistbox (cUIMenu_setFocus .. cUIMenu_getNext, cUIMenu_getPrev), ui/uimenu (cUIMenu_setSelected ..
// cUIMenu_setSelectedByIndex) and ui/uivector (the constructor cUIMenu_cUIMenu, the destructor cUIMenu__cUIMenu, getType
// and the out-of-line inline accessors tAttrMenu::tAttrMenu .. cUIMenu_getSelectedIndex).
//
// Size 0xA8: cUIScreen_createObjectByStruct (record type 0x13) does operator new(0xA8, "UIMenu") and builds it inline
// (cUIThing ctor cUIThing_cUIThing, cUIGroup's vtable, cList ctor cList_cList at 0x74, cUIMenu's vtable, tAttrMenu ctor
// tAttrMenu::tAttrMenu at 0x90, then the stores below); cUIMenu_cUIMenu is the same constructor out of line. Retail vtable
// _vt$7cUIMenu = D_00494928 (cUIMenu_vtbl).
// Bases: cUIGroup (0x90 bytes: cUIThing 0x74 + cList mList 0x1C; ui/uigroup.h, tag "cUIGroup" 0x90) <- cUIThing <-
// cListNode, written out flat below. The vptr is cUIThing's (0x8).
//
// A real subclass of cUIGroup (its own members FLAT), like main/game.h: every field at its retail offset, no C++ inheritance or virtuals.
// Member names are EA's, from SSX On Tour's PSP DWARF (class cUIMenu, size 0xC0: its cUIGroup is 0xA0, so every
// cUIMenu member sits exactly 0x10 later there), placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors (linkonce names isFlipInput / setFlipInput / getScrollIndex /
//       getScrollOffset / getScrollNum / setScrollIndex / setScrollNum / setScrollOffset / getSelected /
//       getSelectedIndex, in that declaration order). Retail keeps out-of-line copies of them in the same order in
//       ui/uivector (cUIMenu_isFlipInput .. cUIMenu_getSelectedIndex), which places each one; retail adds get/set pairs for two more
//       tAttrMenu bits (cUIMenu_isDisableDirSound/cUIMenu_setDisableDirSound bit 3, cUIMenu_isCreateThead/cUIMenu_setCreateThread bit 0);
//   [R] retail's constructors (cUIMenu_cUIMenu and the inlined copy in cUIScreen_createObjectByStruct), the destructor
//       cUIMenu__cUIMenu and the 0xA8 allocation;
//   [U] how retail code uses the field;
//   [O] On Tour's member order and types alone, where the fields on both sides of it are placed by [P] or [R].
//
// mAttrMenu is EA's tAttrMenu (On Tour: 1 byte, unsigned char bitfields, LSB first), defined below. Its constructor
// tAttrMenu::tAttrMenu clears the 4 bits with a byte access (lbu/andi 0xF0/sb), while every accessor and the menu code
// read-modify-write the aligned word at 0x90 (lw/sw): g++ 2.95 does exactly that for a 1-byte bitfield struct
// embedded in a word-aligned object (SLOW_BYTE_ACCESS). Checked with ee-gcc 2.95.3 -O2: `p->mFlipInput = 0 ...` on a
// tAttrMenu* gives lbu/andi 0xf0/sb, `self->mAttrMenu.mFlipInput = enable != 0` gives retail cUIMenu_setFlipInput's
// lw/sltu/li -3/sll/and/or/sw, and a bool getter gives lw/sra/andi (cUIMenu_isFlipInput). mInputMap follows at 0x91.

#include "layout.h"
#include "ui/uigroup.h"
#include "ui/uithing.h"
#include "util/list.h"

struct cUIAnimation;
struct cUIScreen;
struct cListNode;
struct cUIScrollBar;

// EA's tAttrMenu (On Tour DWARF: struct tAttrMenu, size 0x1). Retail: tAttrMenu::tAttrMenu() is tAttrMenu::tAttrMenu.
struct tAttrMenu {
    unsigned char mCreateThread : 1;    // bit 0 [U] cUIMenu_isCreateThead/cUIMenu_setCreateThread; passed to cUIMenu_setSelected by onInputChangeItem
    unsigned char mFlipInput : 1;       // bit 1 [P] isFlipInput/setFlipInput (cUIMenu_isFlipInput/cUIMenu_setFlipInput)
    unsigned char mScrollMenu : 1;      // bit 2 [O]; [U] cUIMenu_setScrollableMenu sets it; getNext / cUIMenu_getPrev /
                                        //       cUIMenu_findFirstHighlightable / cUIMenu_reverseFindFirstHighlightable / onInputChangeItem test it
    unsigned char mDisableDirSound : 1; // bit 3 [O]; [U] cUIMenu_isDisableDirSound/cUIMenu_setDisableDirSound; cUIMenu_onInput tests it

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    tAttrMenu();
};

CHECK_SIZE(tAttrMenu, 0x1);

struct cUIThing;

struct cUIMenu : public cUIGroup {
    // cUIGroup (ui/uigroup.h, 0x90): the vptr at 0x008:
    //   [R] _vt$7cUIMenu D_00494928 (cUIMenu_vtbl), stored after cUIGroup's D_004949E8; the destructor stores
    //       cUIGroup's back
    // cUIMenu's uses of cUIThing's fields:
    //   mAttr: [U] cUIMenu_setFocus, cUIMenu_getNext and cUIMenu_onInput test its bits
    //   mpEvents: [U] cUIThing_getKeyframerEvent
    //   mPosition: [U] cUIMenu_render2D
    //   mpScreen: [U] cUIScreen_playFrame(mpScreen, ...) in cUIMenu_setSelected, cUIMenu_setFocus, cUIMenu_onInput
    //   mList: [U] cList_size(&mList) in cUIMenu_setScrollableMenu / setUIData, cList_getByIndex in the menu code
    // --- cUIMenu
    // EA's tUIMenuEvent: the Event a state's onWidgetMenu(pMenu, Event, index) receives. [O] On Tour enum
    // cUIMenu::tUIMenuEvent, names and values. [U] retail fits: cUIMenu_getNext / getPrev ask kISHIGHLIGHTABLE (1)
    // for each row; cUIMenu_onInputChangeItem sends kSCROLLUP (2) or kSCROLLDOWN (3), then kSCROLLUPDATE (4) with the
    // new offset and kSCROLLCHANGE (5); cUIMenu_onInput sends kONNEXT / kONPREVIOUS / kONOPTION / kONMISC (6..9) for
    // the state events kONNEXT / kONPREV / kONOPTION / kONMISC (5..8).
    enum tUIMenuEvent {
        kISSELECTABLE = 0,
        kISHIGHLIGHTABLE = 1,
        kSCROLLUP = 2,
        kSCROLLDOWN = 3,
        kSCROLLUPDATE = 4,
        kSCROLLCHANGE = 5,
        kONNEXT = 6,
        kONPREVIOUS = 7,
        kONOPTION = 8,
        kONMISC = 9
    };

    // The "no item" index (names ours): [U] cUIMenu_findFirstHighlightable returns it when no item can be highlighted
    // and treats it as "from the start"; [R] the ctor stores it in mStoredIndex.
    enum { kNoItem = 0xFF };

    // cUIMenu's own slots ([R] cUIMenu_vtbl; types [O] On Tour's class cUIMenu):
    virtual char getType();                                               // 22 cUIMenu_getType__FPv
    tAttrMenu mAttrMenu;             // 0x090 [O] tAttrMenu; [P] isFlipInput/setFlipInput (bit 1, lw/sw the word at 0x90;
                                     //       retail cUIMenu_isFlipInput/cUIMenu_setFlipInput); [R] tAttrMenu() tAttrMenu::tAttrMenu clears
                                     //       bits 0-3 (byte); [U] cUIMenu_getNext / onInput / setScrollableMenu (bit 2)
    signed char mInputMap[4];        // 0x091 [O] char mInputMap[4]; [U] cUIMenu_setInputMap stores 4 bytes (sb 0x91..0x94),
                                     //       cUIMenu_onInput reads them (lb)
    unsigned char mScrollItemCurrent; // 0x095 [P] getScrollIndex/setScrollIndex/getSelectedIndex (lbu/sb 0x95; retail
                                     //       cUIMenu_getScrollIndex/cUIMenu_setScrollIndex/cUIMenu_getSelectedIndex); [R] = 0; [U] the selected row
                                     //       (fe/fepopupmisc reads/writes menu + 0x95)
    unsigned char mScrollItemNum;    // 0x096 [P] getScrollNum/setScrollNum (lbu/sb 0x96; retail cUIMenu_getScrollNum/cUIMenu_setScrollNum);
                                     //       [R] = 0; [U] cUIMenu_setUIData / setScrollableMenu store the item count
    unsigned char mScrollItemNumVisible; // 0x097 [O]; [R] = 0; [U] cUIMenu_setUIData / setScrollableMenu store it,
                                     //       cUIMenu_render2D / onInputChangeItem / setSelectedByIndex read it
    unsigned char mScrollItemOffset; // 0x098 [P] getScrollOffset/setScrollOffset (lbu/sb 0x98; retail cUIMenu_getScrollOffset/
                                     //       cUIMenu_setScrollOffset); [R] = 0
    char unk_099[0x3];               // 0x099 padding
    cUIScrollBar* mpScrollBar;       // 0x09C [O]; [R] = 0; [U] cUIMenu_setScrollBar stores it, cUIMenu_render2D reads it
    cUIThing* mpSelected;            // 0x0A0 [P] getSelected (lw 0xA0; retail cUIMenu_getSelected); [R] = 0; [U] cUIMenu_setSelected
                                     //       stores the item, cUIMenu_setFocus / onInput read it
    unsigned char mStoredIndex;      // 0x0A4 [O]; [R] = 0xFF; [U] cUIMenu_setSelected / setSelectedByIndex store it (sb),
                                     //       cUIMenu_setFocus reads it (lbu)
    char unk_0A5[0x3];               // 0x0A5 padding (placed by the 0xA8 allocation)

    // Methods bound to their retail symbols by asm labels.
    void setInputMap(char value) __asm__("cUIMenu_setInputMap");
    unsigned char getNext() __asm__("cUIMenu_getNext");
    unsigned char getPrev() __asm__("cUIMenu_getPrev");
    void setSelected(cUIThing* item, int play) __asm__("cUIMenu_setSelected");
    bool onInputChangeItem(unsigned char OldScrollItemCurrent) __asm__("cUIMenu_onInputChangeItem");
    void setScrollableMenu(char NumItems) __asm__("cUIMenu_setScrollableMenu");
    void setScrollBar(cUIScrollBar* scrollBar) __asm__("cUIMenu_setScrollBar");
    unsigned char findFirstHighlightable(unsigned char start) __asm__("cUIMenu_findFirstHighlightable");
    unsigned char reverseFindFirstHighlightable(unsigned char idx) __asm__("cUIMenu_reverseFindFirstHighlightable");
    void setSelectedByIndex(unsigned char idx) __asm__("cUIMenu_setSelectedByIndex");
    int isFlipInput() __asm__("cUIMenu_isFlipInput");
    void setFlipInput(int enable) __asm__("cUIMenu_setFlipInput");
    int isDisableDirSound() __asm__("cUIMenu_isDisableDirSound");
    void setDisableDirSound(int enable) __asm__("cUIMenu_setDisableDirSound");
    int isCreateThead() __asm__("cUIMenu_isCreateThead");
    void setCreateThread(int createThread) __asm__("cUIMenu_setCreateThread");
    unsigned char getScrollIndex() __asm__("cUIMenu_getScrollIndex");
    unsigned char getScrollOffset() __asm__("cUIMenu_getScrollOffset");
    unsigned char getScrollNum() __asm__("cUIMenu_getScrollNum");
    void setScrollIndex(signed char scrollItemCurrent) __asm__("cUIMenu_setScrollIndex");
    void setScrollNum(signed char scrollItemNum) __asm__("cUIMenu_setScrollNum");
    void setScrollOffset(signed char scrollItemOffset) __asm__("cUIMenu_setScrollOffset");
    cUIThing* getSelected() __asm__("cUIMenu_getSelected");
    unsigned char getSelectedIndex() __asm__("cUIMenu_getSelectedIndex");
    virtual void setUIData(tUIBaseObject*) __asm__("cUIMenu_setUIData");
};

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
