#ifndef FE_FESTATEOPTIONSHUD_H
#define FE_FESTATEOPTIONSHUD_H
#pragma interface

// cFEStateOptionsHUD: a front-end state (a cFEState), 5 methods; allocated with cMemMan_alloc(72, "cFEStateOptionsHUD").
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

struct cFEStateOptionsHUD : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateOptionsHUD

    // Methods bound to their retail symbols by asm labels.
    void updateWidget() __asm__("cFEStateOptionsHUD_updateWidget");
    virtual ~cFEStateOptionsHUD();   // g++'s own symbol _$_18cFEStateOptionsHUD (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateOptionsHUD_onCreateScreen");
    virtual void onWidgetEvent(cUIThing* widget, unsigned int event) __asm__("cFEStateOptionsHUD_onWidgetEvent");
    virtual void onPopupEvent(cUIState* widget, unsigned int msg) __asm__("cFEStateOptionsHUD_onPopupEvent");
    virtual int onWidgetMenu(cUIMenu* menu, unsigned int event, unsigned int)
        __asm__("cFEStateOptionsHUD_onWidgetMenu");
    virtual void onGainFocus(cUIState*) __asm__("cFEStateOptionsHUD_onGainFocus__FPv");
};

CHECK_SIZE(cFEStateOptionsHUD, 0x48);
__asm__("_vt$18cFEStateOptionsHUD = cFEStateOptionsHUD_vtbl");   // g++'s name for the retail vtable

#endif
