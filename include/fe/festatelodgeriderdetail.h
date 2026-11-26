#ifndef FE_FESTATELODGERIDERDETAIL_H
#define FE_FESTATELODGERIDERDETAIL_H

// cFEStateLodgeRiderDetail: a front-end state (a cUIState), 7 methods; allocated with cMemMan_alloc(76, "cFEStateLodgeRiderDetail").
// Size 0x4C [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateLodgeRiderDetail : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateLodgeRiderDetail
    int mbCheatCharEnable;          // 0x048 [O] bool

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cFEStateLodgeRiderDetail__cFEStateLodgeRiderDetail stays a C function (g++ 2.95 takes no asm
    // label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateLodgeRiderDetail();
    virtual void onCreateScreen() __asm__("cFEStateLodgeRiderDetail_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* w) __asm__("cFEStateLodgeRiderDetail_onWidgetCreate");
    virtual void onPreRender2D() __asm__("cFEStateLodgeRiderDetail_onPreRender2D");
};

CHECK_OFFSET(cFEStateLodgeRiderDetail, mbCheatCharEnable, 0x048);
CHECK_SIZE(cFEStateLodgeRiderDetail, 0x4C);

#endif
