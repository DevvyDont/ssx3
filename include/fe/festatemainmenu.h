#ifndef FE_FESTATEMAINMENU_H
#define FE_FESTATEMAINMENU_H

// cFEStateMainMenu: a front-end state (a cUIState), 8 methods; allocated with cMemMan_alloc(88, "cFEStateMainMenu").
// Size 0x58 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;
struct cUIThing;

struct cFEStateMainMenu : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateMainMenu
    int mGoToNextState;                // 0x048 [O] bool
    cUIThing* mpSelectMultiModeWidget; // 0x04C [O]
    int mSaveCurrentCtrl;              // 0x050 [O]
    char mHackCheatTotal;              // 0x054 [O]
    char mNumOfCtrl;                   // 0x055 [O]
    char unk_056[0x2];                 // 0x056 padding
};

CHECK_OFFSET(cFEStateMainMenu, mGoToNextState, 0x048);
CHECK_OFFSET(cFEStateMainMenu, mpSelectMultiModeWidget, 0x04C);
CHECK_OFFSET(cFEStateMainMenu, mSaveCurrentCtrl, 0x050);
CHECK_OFFSET(cFEStateMainMenu, mHackCheatTotal, 0x054);
CHECK_OFFSET(cFEStateMainMenu, mNumOfCtrl, 0x055);
CHECK_SIZE(cFEStateMainMenu, 0x58);

#endif
