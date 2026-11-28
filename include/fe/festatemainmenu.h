#ifndef FE_FESTATEMAINMENU_H
#define FE_FESTATEMAINMENU_H
#pragma interface

// cFEStateMainMenu: a front-end state (a cFEState), 8 methods; allocated with cMemMan_alloc(88, "cFEStateMainMenu").
// Size 0x58 [R] (the retail allocation; On Tour's is the same). Derives from cFEState (fe/festate.h); member names,
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
struct cUIThing;

struct cFEStateMainMenu : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateMainMenu
    int mGoToNextState;                // 0x048 [O] bool
    cUIThing* mpSelectMultiModeWidget; // 0x04C [O]
    int mSaveCurrentCtrl;              // 0x050 [O]
    char mHackCheatTotal;              // 0x054 [O]
    char mNumOfCtrl;                   // 0x055 [O]
    char unk_056[0x2];                 // 0x056 padding

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateMainMenu1() __asm__("cFEStateMainMenu_cFEStateMainMenu1");
    virtual ~cFEStateMainMenu();   // g++'s own symbol _$_16cFEStateMainMenu (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateMainMenu_onCreateScreen");
    virtual void onPreRender2D() __asm__("cFEStateMainMenu_onPreRender2D");
    virtual void onWidgetCreate(cUIThing* item) __asm__("cFEStateMainMenu_onWidgetCreate");
    virtual void onUpdate() __asm__("cFEStateMainMenu_onUpdate");
    virtual void onWidgetEvent(cUIThing* item_, unsigned int type) __asm__("cFEStateMainMenu_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int Event, unsigned int) __asm__("cFEStateMainMenu_onWidgetMenu");
    virtual void onPopupEvent(cUIState* obj, unsigned int msg) __asm__("cFEStateMainMenu_onPopupEvent");
};

CHECK_OFFSET(cFEStateMainMenu, mGoToNextState, 0x048);
CHECK_OFFSET(cFEStateMainMenu, mpSelectMultiModeWidget, 0x04C);
CHECK_OFFSET(cFEStateMainMenu, mSaveCurrentCtrl, 0x050);
CHECK_OFFSET(cFEStateMainMenu, mHackCheatTotal, 0x054);
CHECK_OFFSET(cFEStateMainMenu, mNumOfCtrl, 0x055);
CHECK_SIZE(cFEStateMainMenu, 0x58);
__asm__("_vt$16cFEStateMainMenu = cFEStateMainMenu_vtbl");   // g++'s name for the retail vtable

#endif
