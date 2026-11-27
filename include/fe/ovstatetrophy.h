#ifndef FE_OVSTATETROPHY_H
#define FE_OVSTATETROPHY_H

// cOVStateTrophy: the in-game trophy overlay ("34trophy"): widget events 5 / 6 ask the custom interface (the
// engine's first word) for the next / previous state and push it.
// EA's source file is not known (no assert of its own). Retail code: onCreateScreen / onWidgetEvent in fe/ovstatetrophy
// (0x0020E938, 0x0020E9A0); the ctors func_002222F8 (engine 0) / cOVStateTrophy_cOVStateTrophy (pEngine, id) and the dtor in
// fe/all_in_one_fe (0x002222F8 ... 0x002223C0).
//
// Size 0x9C: [O] On Tour's class is 0x90 = On Tour's cOVState (no members of its own); retail's cOVState is 0x9C
// (fe/ovstate.h) and the ctors write nothing past mOVID (0x98). Retail never calls the ctors (no allocation site).
// Base: cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8). Both ctors (engine 0 / (pEngine, id)) run
// cUIState_cUIState, the inlined cOVState ctor (mOVID, the three bools, cOVState_vtbl, mszChatBuff = " ") and store
// cOVStateTrophy_vtbl (0x00471EA8, 25 slots: 1 dtor, 4 onCreateScreen, 19 onWidgetEvent,
// 25 cOVState_allowPauseMenu); the dtor stores cOVState_vtbl and runs cUIState__cUIState.
// Derives from cOVState (fe/ovstate.h); its own members at their retail offsets. fe/ovstate.h has the inherited fields'
// full evidence. Evidence tags: [R] retail's ctors / dtor; [P] the July 2003 preview (fe/ovstate.h); [U] how retail
// code uses the field; [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVStateTrophy : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVStateTrophy_vtbl (ctors), cOVState_vtbl (dtor)
    // cOVStateTrophy's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "34trophy" through it; onWidgetEvent calls its custom interface (+0x0) slots
    //       4 / 5 and pushes the result on its stack (+0x18)
    //   mpScreen: [U] onCreateScreen stores the screen
    // cOVState (fe/ovstate.h, 0x9C)

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cOVStateTrophy__cOVStateTrophy stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVStateTrophy();
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int msg) __asm__("cOVStateTrophy_onWidgetEvent");
};

CHECK_SIZE(cOVStateTrophy, 0x9C);

#endif /* FE_OVSTATETROPHY_H */
