#ifndef FE_FESTATETITLE_H
#define FE_FESTATETITLE_H

// cFEStateTitle: a front-end state (a cUIState), 7 methods; allocated with cMemMan_alloc(76, "cFEStateTitle").
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

struct cFEStateTitle : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateTitle
    int mNumTicksIdle;              // 0x048 [O]

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateTitle1() __asm__("cFEStateTitle_cFEStateTitle1");
};

CHECK_OFFSET(cFEStateTitle, mNumTicksIdle, 0x048);
CHECK_SIZE(cFEStateTitle, 0x4C);

#endif
