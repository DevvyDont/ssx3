#ifndef FE_FESTATEMPCIRCUIT_H
#define FE_FESTATEMPCIRCUIT_H

// cFEStateMPCircuit: a front-end state (a cUIState), 5 methods; allocated with cMemMan_alloc(72, "cFEStateMPCircuit").
// Size 0x48 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateMPCircuit : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateMPCircuit

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateMPCircuit1() __asm__("cFEStateMPCircuit_cFEStateMPCircuit1");
    // retail's destructor cFEStateMPCircuit__cFEStateMPCircuit stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateMPCircuit();
    virtual void onCreateScreen() __asm__("cFEStateMPCircuit_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* item, unsigned int msg) __asm__("cFEStateMPCircuit_onWidgetEvent");
    void initCircuitList() __asm__("cFEStateMPCircuit_initCircuitList__FPv");
    void updateWidget() __asm__("cFEStateMPCircuit_updateWidget__FPv");
    virtual void onWidgetCreate(cUIThing*) __asm__("cFEStateMPCircuit_onWidgetCreate__FPv");
};

CHECK_SIZE(cFEStateMPCircuit, 0x48);

#endif
