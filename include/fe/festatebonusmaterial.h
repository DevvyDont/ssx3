#ifndef FE_FESTATEBONUSMATERIAL_H
#define FE_FESTATEBONUSMATERIAL_H

// cFEStateBonusMaterial: a front-end state (a cUIState), 5 methods; allocated with cMemMan_alloc(72, "cFEStateBonusMaterial").
// Size 0x48 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateBonusMaterial : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateBonusMaterial

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateBonusMaterial1() __asm__("cFEStateBonusMaterial_cFEStateBonusMaterial1");
};

CHECK_SIZE(cFEStateBonusMaterial, 0x48);

#endif
