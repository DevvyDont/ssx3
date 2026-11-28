#ifndef FE_FESTATELODGERIDERDETAIL_H
#define FE_FESTATELODGERIDERDETAIL_H
#pragma interface

// cFEStateLodgeRiderDetail: a front-end state (a cFEState), 7 methods; allocated with cMemMan_alloc(76, "cFEStateLodgeRiderDetail").
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

struct cFEStateLodgeRiderDetail : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateLodgeRiderDetail
    int mbCheatCharEnable;          // 0x048 [O] bool

    // Methods bound to their retail symbols by asm labels.
    virtual ~cFEStateLodgeRiderDetail();   // g++'s own symbol _$_24cFEStateLodgeRiderDetail (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateLodgeRiderDetail_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* w) __asm__("cFEStateLodgeRiderDetail_onWidgetCreate");
    virtual void onPreRender2D() __asm__("cFEStateLodgeRiderDetail_onPreRender2D");
    virtual void onPopupEvent(cUIState* popup, unsigned int event) __asm__("cFEStateLodgeRiderDetail_onPopupEvent");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int key, unsigned int)
        __asm__("cFEStateLodgeRiderDetail_onWidgetMenu");
    virtual void onWidgetEvent(cUIThing* w, unsigned int event) __asm__("cFEStateLodgeRiderDetail_onWidgetEvent");
};

CHECK_OFFSET(cFEStateLodgeRiderDetail, mbCheatCharEnable, 0x048);
CHECK_SIZE(cFEStateLodgeRiderDetail, 0x4C);
__asm__("_vt$24cFEStateLodgeRiderDetail = cFEStateLodgeRiderDetail_vtbl");   // g++'s name for the retail vtable

#endif
