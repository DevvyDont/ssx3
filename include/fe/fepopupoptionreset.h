#ifndef FE_FEPOPUPOPTIONRESET_H
#define FE_FEPOPUPOPTIONRESET_H

// cFEPopupOptionReset: the options screens' yes / no popup ("111resetopts": save or reset the changed options?). The
// options states allocate it, construct it inline and push it as a popup; their onPopupEvent then read the answer
// through getData (vtable slot 23).
// EA's source file is not known (On Tour: fe/all_in_one_fe; the users are in fe/festateoptions.cpp). Retail code: the
// dtor, onCreateScreen, onWidgetMenu, onWidgetEvent and getData in main/all_in_one_main (0x001DE9F8 ... 0x001DEB50);
// there is no out-of-line ctor.
//
// Size 0x4C:
//   [R] cMemMan_alloc(76, "cFEPopupOptionReset") + cUIState_cUIState1(mem, mpEngine, this state), vtable, byte 0x48 = 0
//       in cFEStateOptions_saveOptionPrompt and cFEStateOptionsGame / HUD / Sound / Controller_onWidgetEvent;
//   [O] On Tour's class is 0x4C: char mAccept at 0x48.
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8); not a cFEState (slot 6 is cUIState_onGainFocus; the
// dtor stores cFEPopupOptionReset_vtbl and runs cUIState__cUIState). Vtable cFEPopupOptionReset_vtbl (0x0046B3D0,
// 24 slots: 1 dtor, 4 onCreateScreen, 19 onWidgetEvent, 20 onWidgetMenu, 23 getData).
// Evidence tags as below, and [P] the July 2003 preview's getData (an lb, like retail's).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags: [R] retail's constructors / destructor / allocation; [U] how retail code uses the field (the C bodies
// named above and the asm); [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEPopupOptionReset {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEPopupOptionReset_vtbl (the allocators, dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [U] onCreateScreen stores the screen's hash ("111resetopts")
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the allocating state's engine; [U] onCreateScreen adds the screen;
                                    //       onWidgetEvent pops its stack (+0x18)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]; [R] the allocating options state (cUIState_cUIState1's parent)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cFEPopupOptionReset
    char mAccept;                   // 0x048 [O][P] getData (lb); [R] the allocators store 0 (sb); [U] onWidgetEvent: 1
                                    //       on "yes", 0 on "no" (sb), then pops the popup; getData returns it
    char unk_049[0x3];              // 0x049 padding
};

CHECK_OFFSET(cFEPopupOptionReset, mpPrev, 0x000);
CHECK_OFFSET(cFEPopupOptionReset, mpNext, 0x004);
CHECK_OFFSET(cFEPopupOptionReset, mVtbl, 0x008);
CHECK_OFFSET(cFEPopupOptionReset, mHashName, 0x00C);
CHECK_OFFSET(cFEPopupOptionReset, mpEngine, 0x010);
CHECK_OFFSET(cFEPopupOptionReset, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEPopupOptionReset, mCtrlBits, 0x015);
CHECK_OFFSET(cFEPopupOptionReset, unk_016, 0x016);
CHECK_OFFSET(cFEPopupOptionReset, miMiscData, 0x018);
CHECK_OFFSET(cFEPopupOptionReset, mAttrState, 0x01C);
CHECK_OFFSET(cFEPopupOptionReset, mpParentState, 0x020);
CHECK_OFFSET(cFEPopupOptionReset, mScreens, 0x024);
CHECK_OFFSET(cFEPopupOptionReset, mpScreen, 0x040);
CHECK_OFFSET(cFEPopupOptionReset, mUserNum, 0x044);
CHECK_OFFSET(cFEPopupOptionReset, unk_045, 0x045);
CHECK_OFFSET(cFEPopupOptionReset, mAccept, 0x048);
CHECK_OFFSET(cFEPopupOptionReset, unk_049, 0x049);
CHECK_SIZE(cFEPopupOptionReset, 0x4C);

#endif /* FE_FEPOPUPOPTIONRESET_H */
