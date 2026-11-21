#ifndef FE_FESTATE_H
#define FE_FESTATE_H

// cFEState: the base of most front-end menu states (cFEStateOptions*, cFEStateCharSelect, cFEStateProfileSelect,
// cFEStateSelectMultiplayerMode, ...): a cUIState that keeps each menu's selected entry in the front end's flow state
// (onGainFocus restores the "Menu" widget's selection from cFEFlowState entry (signed char)mHashName, onLoseFocus saves
// it) and sets the controller filter / mask through the front end's custom interface. It adds no data.
// EA's source file is not known (On Tour: fe/all_in_one_fe). Retail code: cFEState_onGainFocus / cFEState_onLoseFocus
// in fe/festatecredits (0x00186518, 0x001865A8); the ctors func_001DD6A8 (engine 0), func_001DD6E0 (pEngine) and
// func_001DD718 (pEngine, pParent: cUIState_cUIState1), the dtor cFEState__cFEState and setControllerFilter /
// setControllerMask in main/all_in_one_main (0x001DD6A8 ... 0x001DD800).
//
// Size 0x48:
//   [O] On Tour's class is 0x48 (no members of its own);
//   [R] the ctors write only the cUIState part and the vptr; every subclass's own members start at 0x48 (e.g.
//       cFEStateOptions' mbToSaveOptionScreen, fe/festateoptions.h). Retail never allocates a plain cFEState.
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). Vtable cFEState_vtbl (0x0046D0D0, 24 slots: 1 dtor
// cFEState__cFEState, 6 onGainFocus, 11 onLoseFocus; the rest cUIState's / cListNode's). The ctors run a cUIState ctor
// and store cFEState_vtbl; the dtor stores it again and runs cUIState__cUIState. Subclass dtors (e.g.
// cFEStateProfileSelect's) store cFEState_vtbl before cUIState__cUIState: that is how a subclass shows this base.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags: [R] retail's constructors / destructor / allocation; [U] how retail code uses the field (the C bodies
// named above and the asm); [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

#include "layout.h"
#include "util/list.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEState {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEState_vtbl (ctors, dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [U] onGainFocus / onLoseFocus read its low byte (lb 0xC, = (signed
                                    //       char)mHashName) as the flow-state index
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] setControllerFilter /
                                    //       setControllerMask call the engine's custom interface (its first word, a
                                    //       cFECustom)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF; [U] setControllerFilter stores the
                                    //       mask of the controllers not taken by users 0 .. n-1 (sb)
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    cList mScreens;                 // 0x024 [O] cList mScreens (util/list.h; ui/uistate.h); [U] onGainFocus / onLoseFocus
                                    //       look "Menu" up on cList_first(&mScreens)
    cUIScreen* mpScreen;            // 0x040 [O]
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
};

CHECK_OFFSET(cFEState, mpPrev, 0x000);
CHECK_OFFSET(cFEState, mpNext, 0x004);
CHECK_OFFSET(cFEState, mVtbl, 0x008);
CHECK_OFFSET(cFEState, mHashName, 0x00C);
CHECK_OFFSET(cFEState, mpEngine, 0x010);
CHECK_OFFSET(cFEState, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEState, mCtrlBits, 0x015);
CHECK_OFFSET(cFEState, unk_016, 0x016);
CHECK_OFFSET(cFEState, miMiscData, 0x018);
CHECK_OFFSET(cFEState, mAttrState, 0x01C);
CHECK_OFFSET(cFEState, mpParentState, 0x020);
CHECK_OFFSET(cFEState, mScreens, 0x024);
CHECK_OFFSET(cFEState, mpScreen, 0x040);
CHECK_OFFSET(cFEState, mUserNum, 0x044);
CHECK_OFFSET(cFEState, unk_045, 0x045);
CHECK_SIZE(cFEState, 0x48);

#endif /* FE_FESTATE_H */
