#ifndef FE_FESTATEOPTIONSHUD_H
#define FE_FESTATEOPTIONSHUD_H

// cFEStateOptionsHUD: a front-end state (a cUIState), 5 methods; allocated with cMemMan_alloc(72, "cFEStateOptionsHUD").
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

struct cFEStateOptionsHUD : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateOptionsHUD

    // Methods bound to their retail symbols by asm labels.
    void updateWidget() __asm__("cFEStateOptionsHUD_updateWidget");
    // retail's destructor cFEStateOptionsHUD__cFEStateOptionsHUD stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateOptionsHUD();
    virtual void onCreateScreen() __asm__("cFEStateOptionsHUD_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* widget, unsigned int event) __asm__("cFEStateOptionsHUD_onWidgetEvent");
};

CHECK_SIZE(cFEStateOptionsHUD, 0x48);

#endif
