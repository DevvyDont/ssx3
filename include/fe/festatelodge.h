#ifndef FE_FESTATELODGE_H
#define FE_FESTATELODGE_H
#pragma interface

// cFEStateLodge: a front-end state (a cFEState), 6 methods; allocated with cMemMan_alloc(88, "cFEStateLodge").
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

struct cFEStateLodge : public cFEState {
    // cFEState (fe/festate.h, no data of its own; [R] the destructor stores cFEState_vtbl), on top of
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateLodge
    int mCheatCharID;               // 0x048 [O]
    int bToNextState;               // 0x04C [O] bool
    int mPathToSaveProfile;         // 0x050 [O]
    int mbIsYesOrNo;                // 0x054 [O] bool

    // The prompt mPathToSaveProfile records (names ours). [U] onWidgetEvent stores kPathQuit (the quit option's id,
    // 6) with the kT_OVRCMNQuitTitlePrompt popup and kPathSaveProgress with kT_CMNSaveProgress; onPopupEvent stores
    // kPathSaveBeforeQuit with kT_CMNSaveBeforeQuit and branches on all three.
    enum { kPathQuit = 6, kPathSaveProgress = 0x10, kPathSaveBeforeQuit = 0x11 };

    // Methods bound to their retail symbols by asm labels.
    virtual ~cFEStateLodge();   // g++'s own symbol _$_13cFEStateLodge (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateLodge_onCreateScreen");
    virtual void onWidgetCreate(cUIThing* w) __asm__("cFEStateLodge_onWidgetCreate");
    virtual void onPopupEvent(cUIState* msg, unsigned int id) __asm__("cFEStateLodge_onPopupEvent");
    virtual void onWidgetEvent(cUIThing* w, unsigned int event) __asm__("cFEStateLodge_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int key, unsigned int) __asm__("cFEStateLodge_onWidgetMenu");
};

CHECK_OFFSET(cFEStateLodge, mCheatCharID, 0x048);
CHECK_OFFSET(cFEStateLodge, bToNextState, 0x04C);
CHECK_OFFSET(cFEStateLodge, mPathToSaveProfile, 0x050);
CHECK_OFFSET(cFEStateLodge, mbIsYesOrNo, 0x054);
CHECK_SIZE(cFEStateLodge, 0x58);
__asm__("_vt$13cFEStateLodge = cFEStateLodge_vtbl");   // g++'s name for the retail vtable

#endif
