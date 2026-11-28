#ifndef FE_FESTATEREWARDSROOM_H
#define FE_FESTATEREWARDSROOM_H
#pragma interface

// cFEStateRewardsRoom: a front-end state (a cFEState), 7 methods; allocated with cMemMan_alloc(80, "cFEStateRewardsRoom").
// Size 0x50 [R] (the retail allocation; On Tour's is the same). Derives from cFEState (fe/festate.h); member names,
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

struct cUIText;

struct cFEStateRewardsRoom : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateRewardsRoom
    int mCharID;                    // 0x048 [O]
    int mInStore;                   // 0x04C [O] bool

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRewardsRoom1() __asm__("cFEStateRewardsRoom_cFEStateRewardsRoom1");
    int isInStore() __asm__("cFEStateRewardsRoom_isInStore");
    void setRewardsWidgetText(int index, cUIText* text) __asm__("cFEStateRewardsRoom_setRewardsWidgetText");
    virtual ~cFEStateRewardsRoom();   // g++'s own symbol _$_19cFEStateRewardsRoom (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateRewardsRoom_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* w) __asm__("cFEStateRewardsRoom_onWidgetCreate");
    virtual void onWidgetEvent(cUIThing* item, unsigned int msg) __asm__("cFEStateRewardsRoom_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int key, unsigned int)
        __asm__("cFEStateRewardsRoom_onWidgetMenu");
};

CHECK_OFFSET(cFEStateRewardsRoom, mCharID, 0x048);
CHECK_OFFSET(cFEStateRewardsRoom, mInStore, 0x04C);
CHECK_SIZE(cFEStateRewardsRoom, 0x50);
__asm__("_vt$19cFEStateRewardsRoom = cFEStateRewardsRoom_vtbl");   // g++'s name for the retail vtable

#endif
