#ifndef FE_FESTATETITLE_H
#define FE_FESTATETITLE_H
#pragma interface

// cFEStateTitle: a front-end state (a cFEState), 7 methods; allocated with cMemMan_alloc(76, "cFEStateTitle").
// Size 0x4C [R] (the retail allocation; On Tour's is the same). Derives from cFEState (fe/festate.h); member names,
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

struct cFEStateTitle : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateTitle
    int mNumTicksIdle;              // 0x048 [O]

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateTitle1() __asm__("cFEStateTitle_cFEStateTitle1");
    virtual ~cFEStateTitle();   // g++'s own symbol _$_13cFEStateTitle (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateTitle_onCreateScreen");
    virtual void onUpdate() __asm__("cFEStateTitle_onUpdate");
    virtual void onWidgetCreate(cUIThing* item) __asm__("cFEStateTitle_onWidgetCreate");
    virtual bool onGainTransition(int) __asm__("cFEStateTitle_onGainTransition");
    virtual void onPopupEvent(cUIState*, unsigned int) __asm__("cFEStateTitle_onPopupEvent__FPv");
    virtual void onWidgetEvent(cUIThing* msg, unsigned int type) __asm__("cFEStateTitle_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int) __asm__("cFEStateTitle_onWidgetMenu");
};

CHECK_OFFSET(cFEStateTitle, mNumTicksIdle, 0x048);
CHECK_SIZE(cFEStateTitle, 0x4C);
__asm__("_vt$13cFEStateTitle = cFEStateTitle_vtbl");   // g++'s name for the retail vtable

#endif
