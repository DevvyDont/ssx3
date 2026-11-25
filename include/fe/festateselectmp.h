#ifndef FE_FESTATESELECTMP_H
#define FE_FESTATESELECTMP_H

// cFEStateSelectMultiplayerMode: the front end's "select multiplayer mode" menu ("86sel_multi", state id 22): two
// entries (misc data 0 / 1) whose pick asks the custom interface (cFECustom, the engine's first word) for the next /
// previous state (slots 4 / 5). Retail never constructs it (no caller of either ctor; the online FE uses
// cFEPopupSelectMultiplayerMode, fe/fepopupselectmultiplayermode.h).
// EA's source file is not known. Retail code: the engine ctor
// cFEStateSelectMultiplayerMode_cFEStateSelectMultiplayerMode in fe/fereal (0x001A3050); onCreateScreen /
// onWidgetCreate / onWidgetEvent in fe/festateselectmp (0x001A3090 ... 0x001A3160); the engine-less ctor
// ..._cFEStateSelectMultiplayerMode1 and the dtor in fe/fepopupmisc (0x001E07C0, 0x001E07F8).
//
// Size 0x48: [O] On Tour's class is 0x48 (no members); [R] the ctors write nothing past the cUIState part.
// Base: cFEState (fe/festate.h, no data) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor runs
// cUIState_cUIState(this, pEngine), stores mHashName 22 and cFEStateSelectMultiplayerMode_vtbl (0x00469248, 24 slots:
// 1 dtor, 4 onCreateScreen, 6 cFEState_onGainFocus, 11 cFEState_onLoseFocus, 18 onWidgetCreate, 19 onWidgetEvent); the
// dtor stores cFEState_vtbl and runs cUIState__cUIState.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Evidence tags: [R] retail's
// constructors / destructor / allocation; [U] how retail code uses the field (the C bodies
// named above and the asm); [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEStateSelectMultiplayerMode : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateSelectMultiplayerMode_vtbl (ctors), cFEState_vtbl (dtor)
    // cFEStateSelectMultiplayerMode's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "86sel_multi" through it; onWidgetEvent asks its custom interface (+0x0) for
    //       the next state and pushes it on its stack (+0x18)
    //   mpScreen: [U] onCreateScreen stores the screen

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateSelectMultiplayerMode1() __asm__("cFEStateSelectMultiplayerMode_cFEStateSelectMultiplayerMode1");
};

CHECK_SIZE(cFEStateSelectMultiplayerMode, 0x48);

#endif /* FE_FESTATESELECTMP_H */
