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
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Evidence tags: [R] retail's
// constructors / destructor / allocation; [U] how retail code uses the field (the C bodies
// named above and the asm); [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

#include "layout.h"
#include "ui/uistate.h"
#include "util/list.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEState : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEState_vtbl (ctors, dtor)
    // cFEState's notes on cUIState's fields:
    //   mHashName: [U] onGainFocus / onLoseFocus read its low byte (lb 0xC, = (signed char)mHashName) as the
    //       flow-state index
    //   mpEngine: [U] setControllerFilter / setControllerMask call the engine's custom interface (its first word, a
    //       cFECustom)
    //   mCtrlBits: [U] setControllerFilter stores the mask of the controllers not taken by users 0 .. n-1 (sb)
    //   mScreens: [U] onGainFocus / onLoseFocus look "Menu" up on cList_first(&mScreens)
};

CHECK_SIZE(cFEState, 0x48);

#endif /* FE_FESTATE_H */
