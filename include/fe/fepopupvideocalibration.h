#ifndef FE_FEPOPUPVIDEOCALIBRATION_H
#define FE_FEPOPUPVIDEOCALIBRATION_H

// cFEPopupVideoCalibration: a front-end state (a cUIState), 2 methods; allocated with cMemMan_alloc(76, "cFEPopupVideoCalibration").
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

struct cFEPopupVideoCalibration : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEPopupVideoCalibration
    char mImage;                    // 0x048 [O]
    char unk_049[0x3];              // 0x049 padding
};

CHECK_OFFSET(cFEPopupVideoCalibration, mImage, 0x048);
CHECK_SIZE(cFEPopupVideoCalibration, 0x4C);

#endif
