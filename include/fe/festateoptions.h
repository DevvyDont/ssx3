#ifndef FE_FESTATEOPTIONS_H
#define FE_FESTATEOPTIONS_H

// cFEStateOptionsController: the front end's controller options screen ("22control"): per-pad vibration on/off and
// default/pro button layout lists, with a short test buzz when vibration is switched on.
// EA's source: d:/ssx/code/source/fe/festateoptions.cpp (preview strings); retail code: fe/festateoptions
// (func_0018C658 ... 0x0018D108) and the default-ctor copy, the destructor and the empty onLoseFocus in
// main/all_in_one_main (0x001DE080 ... 0x001DE0E8).
//
// Size 0x54:
//   [R] cMemMan_alloc(0x54, "cFEStateOptionsController") + func_0018C658(mem, engine) in cFECustom_getNextState
//       (state 31); the ctors write up to the byte at 0x50.
//   [O] On Tour's class is 0x54 too, with the same members at the same offsets.
// Base: cFEState (no data; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor
// func_0018C658 runs cUIState_cUIState(this, pEngine), stores cFEStateOptionsController_vtbl (0x0046C480, 24 slots: 1
// dtor, 4 onCreateScreen, 5 onDestroyScreen, 11 onLoseFocus func_001DE0E8, 12 onUpdate, 17 onPopupEvent, 19
// onWidgetEvent, 20 onWidgetMenu, 21 onWidgetPair; vtables.tsv stops at slot 10), mHashName 0x1F and mVibrateOn 0;
// the destructor stores cFEState_vtbl and runs cUIState__cUIState.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags:
//   [R] retail's constructors / destructor and allocation size;
//   [P] the July 2003 preview (layout_xlate: 0x44 -> 0x48, 0x4C -> 0x50, from onUpdate and the ctor);
//   [U] how retail code uses the field (the C bodies in src/fe/festateoptions.cpp and the asm);
//   [O] SSX On Tour's DWARF (class cFEStateOptionsController, cUIState, cListNode): names, types and order.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEStateOptionsController {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState_cUIState links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateOptionsController_vtbl (ctors), cFEState_vtbl (dtor); [P]
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [R] func_0018C658 stores 0x1F
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the engine argument; [U] onCreateScreen adds "22control" through
                                    //       it
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (word, see ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen; updateWidget /
                                    //       displayPadConfig look the lists and pictures up on it
    signed char mUserNum;           // 0x044 [O]
    char unk_045[0x3];              // 0x045 padding
    // --- cFEState (On Tour: size 0x48, no data)
    // --- cFEStateOptionsController
    int mTimer[2];                  // 0x048 [O][P] (preview 0x44); [U] onCreateScreen zeroes both; onWidgetEvent sets
                                    //       pad i's to 60 when its test buzz starts, onUpdate counts it down
    char mVibrateOn;                // 0x050 [O][P] (preview 0x4C); [R] ctors 0; [U] bit 0 / bit 1 = pad 0 / pad 1
                                    //       buzzing
                                    //       (onWidgetEvent sets, onUpdate clears)
    char unk_051[0x3];              // 0x051 padding
};

CHECK_OFFSET(cFEStateOptionsController, mpPrev, 0x000);
CHECK_OFFSET(cFEStateOptionsController, mpNext, 0x004);
CHECK_OFFSET(cFEStateOptionsController, mVtbl, 0x008);
CHECK_OFFSET(cFEStateOptionsController, mHashName, 0x00C);
CHECK_OFFSET(cFEStateOptionsController, mpEngine, 0x010);
CHECK_OFFSET(cFEStateOptionsController, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateOptionsController, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateOptionsController, unk_016, 0x016);
CHECK_OFFSET(cFEStateOptionsController, miMiscData, 0x018);
CHECK_OFFSET(cFEStateOptionsController, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateOptionsController, mpParentState, 0x020);
CHECK_OFFSET(cFEStateOptionsController, mScreens, 0x024);
CHECK_OFFSET(cFEStateOptionsController, mpScreen, 0x040);
CHECK_OFFSET(cFEStateOptionsController, mUserNum, 0x044);
CHECK_OFFSET(cFEStateOptionsController, unk_045, 0x045);
CHECK_OFFSET(cFEStateOptionsController, mTimer, 0x048);
CHECK_OFFSET(cFEStateOptionsController, mVibrateOn, 0x050);
CHECK_OFFSET(cFEStateOptionsController, unk_051, 0x051);
CHECK_SIZE(cFEStateOptionsController, 0x54);

#endif /* FE_FESTATEOPTIONS_H */
