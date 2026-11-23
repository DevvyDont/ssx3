#ifndef FE_FESTATETRANSITION_H
#define FE_FESTATETRANSITION_H

// cFEStateTransition: a front-end state (a cUIState), 3 methods; allocated with cMemMan_alloc(80, "cFEStateTransition").
// Size 0x50 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateTransition : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateTransition
    cUIScreen* mpSnow;              // 0x048 [O]
    cUIScreen* mpWhite;             // 0x04C [O]
};

CHECK_OFFSET(cFEStateTransition, mpSnow, 0x048);
CHECK_OFFSET(cFEStateTransition, mpWhite, 0x04C);
CHECK_SIZE(cFEStateTransition, 0x50);


// ---------------------------------------------------------------------------------------------------------------------
// cFEStateBackground: the front end's animated background state ("snowanim"), the first state cRealFE_loadStartState
// pushes. It plays the screen and keeps nothing.
// EA's source file is not known (On Tour: fe/all_in_one_fe). Retail code: onCreateScreen in fe/festatetransition
// (0x00194528); the ctors func_001DE558 (engine 0) and cFEStateBackground_cFEStateBackground (pEngine; the C definition
// declares only this) and the dtor in main/all_in_one_main (0x001DE558 ... 0x001DE5C8).
//
// Size 0x48: [R] cMemMan_alloc(0x48, "cFEStateBackground") in cRealFE_loadStartState (the ctor inlined:
// cUIState_cUIState + vtable); [O] On Tour's class is 0x48 (no members).
// Base: cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8); not a cFEState (the dtor stores
// cFEStateBackground_vtbl and runs cUIState__cUIState). Vtable cFEStateBackground_vtbl (0x0046BC58, 24 slots: 1 dtor, 4
// onCreateScreen; the rest cUIState's).
// Derives from cUIState (ui/uistate.h). Evidence tags: [R] retail's ctors / dtor / allocation; [U] how retail code uses
// the field; [O] SSX On Tour's DWARF (class cFEStateBackground, cUIState): names, types and order.

struct cFEStateBackground : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateBackground_vtbl (ctors, cRealFE_loadStartState, dtor)
    // cFEStateBackground's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "snowanim" through it and plays it (mpScreen is not set)
};

CHECK_SIZE(cFEStateBackground, 0x48);

#endif
