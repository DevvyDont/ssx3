#ifndef FE_FESTATEMPCIRCUIT_H
#define FE_FESTATEMPCIRCUIT_H
#pragma interface

// cFEStateMPCircuit: a front-end state (a cFEState), 5 methods; allocated with cMemMan_alloc(72, "cFEStateMPCircuit").
// Size 0x48 [R] (the retail allocation; On Tour's is the same). Derives from cFEState (fe/festate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateMPCircuit : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateMPCircuit

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateMPCircuit1() __asm__("cFEStateMPCircuit_cFEStateMPCircuit1");
    virtual ~cFEStateMPCircuit();   // g++'s own symbol _$_17cFEStateMPCircuit (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateMPCircuit_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* item, unsigned int msg) __asm__("cFEStateMPCircuit_onWidgetEvent");
    void initCircuitList() __asm__("cFEStateMPCircuit_initCircuitList__FPv");
    void updateWidget() __asm__("cFEStateMPCircuit_updateWidget__FPv");
    virtual void onWidgetCreate(cUIThing*) __asm__("cFEStateMPCircuit_onWidgetCreate__FPv");
    virtual void onGainFocus(cUIState* pPreSt) __asm__("cFEStateMPCircuit_onGainFocus");
};

CHECK_SIZE(cFEStateMPCircuit, 0x48);
__asm__("_vt$17cFEStateMPCircuit = cFEStateMPCircuit_vtbl");   // g++'s name for the retail vtable

#endif
