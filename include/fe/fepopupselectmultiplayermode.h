#ifndef FE_FEPOPUPSELECTMULTIPLAYERMODE_H
#define FE_FEPOPUPSELECTMULTIPLAYERMODE_H

// cFEPopupSelectMultiplayerMode: a front-end state (a cUIState), 4 methods; allocated with cMemMan_alloc(76, "cFEPopupSelectMultiplayerMode").
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

struct cFEPopupSelectMultiplayerMode : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEPopupSelectMultiplayerMode
    char mMultiMode;                // 0x048 [O]
    char unk_049[0x3];              // 0x049 padding
};

CHECK_OFFSET(cFEPopupSelectMultiplayerMode, mMultiMode, 0x048);
CHECK_SIZE(cFEPopupSelectMultiplayerMode, 0x4C);

#endif
