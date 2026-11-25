#ifndef FE_FESTATELODGE_H
#define FE_FESTATELODGE_H

// cFEStateLodge: a front-end state (a cUIState), 6 methods; allocated with cMemMan_alloc(88, "cFEStateLodge").
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

struct cFEStateLodge : public cUIState {
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
};

CHECK_OFFSET(cFEStateLodge, mCheatCharID, 0x048);
CHECK_OFFSET(cFEStateLodge, bToNextState, 0x04C);
CHECK_OFFSET(cFEStateLodge, mPathToSaveProfile, 0x050);
CHECK_OFFSET(cFEStateLodge, mbIsYesOrNo, 0x054);
CHECK_SIZE(cFEStateLodge, 0x58);

#endif
