#ifndef FE_FESTATEBONUSMATERIAL_H
#define FE_FESTATEBONUSMATERIAL_H
#pragma interface

// cFEStateBonusMaterial: a front-end state (a cFEState), 5 methods; allocated with cMemMan_alloc(72, "cFEStateBonusMaterial").
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

struct cFEStateBonusMaterial : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateBonusMaterial

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateBonusMaterial1() __asm__("cFEStateBonusMaterial_cFEStateBonusMaterial1");
    virtual ~cFEStateBonusMaterial();   // g++'s own symbol _$_21cFEStateBonusMaterial (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateBonusMaterial_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* menu) __asm__("cFEStateBonusMaterial_onWidgetCreate");
    virtual void onWidgetEvent(cUIThing* item, unsigned int event) __asm__("cFEStateBonusMaterial_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int key, unsigned int)
        __asm__("cFEStateBonusMaterial_onWidgetMenu");
    virtual void onGainFocus(cUIState*) __asm__("cFEStateBonusMaterial_onGainFocus__FPv");
};

CHECK_SIZE(cFEStateBonusMaterial, 0x48);
__asm__("_vt$21cFEStateBonusMaterial = cFEStateBonusMaterial_vtbl");   // g++'s name for the retail vtable

#endif
