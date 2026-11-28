#ifndef FE_FESTATEBONUSMATERIAL_H
#define FE_FESTATEBONUSMATERIAL_H

// cFEStateBonusMaterial: a front-end state (a cUIState), 5 methods; allocated with cMemMan_alloc(72, "cFEStateBonusMaterial").
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

struct cFEStateBonusMaterial : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateBonusMaterial

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateBonusMaterial1() __asm__("cFEStateBonusMaterial_cFEStateBonusMaterial1");
    // retail's destructor cFEStateBonusMaterial__cFEStateBonusMaterial stays a C function (g++ 2.95 takes no asm label
    // on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateBonusMaterial();
    virtual void onCreateScreen() __asm__("cFEStateBonusMaterial_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* menu) __asm__("cFEStateBonusMaterial_onWidgetCreate");
    virtual void onWidgetEvent(cUIThing* item, unsigned int event) __asm__("cFEStateBonusMaterial_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int key, unsigned int)
        __asm__("cFEStateBonusMaterial_onWidgetMenu");
    virtual void onGainFocus(cUIState*) __asm__("cFEStateBonusMaterial_onGainFocus__FPv");
};

CHECK_SIZE(cFEStateBonusMaterial, 0x48);

#endif
