#ifndef FE_FESTATERIDERBIO_H
#define FE_FESTATERIDERBIO_H

// cFEStateRiderBio: the rider bio screen (a cUIState); cFEReal creates it with cMemMan_alloc(80, "cFEStateRiderBio").
// Size 0x50 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateRiderBio : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateRiderBio
    int mCharID;                    // 0x048 [O]
    int mScreen;                    // 0x04C [O]
};

CHECK_OFFSET(cFEStateRiderBio, mCharID, 0x048);
CHECK_OFFSET(cFEStateRiderBio, mScreen, 0x04C);
CHECK_SIZE(cFEStateRiderBio, 0x50);

#endif
