#ifndef FE_FESTATEUBERTRICK_H
#define FE_FESTATEUBERTRICK_H

// cFEStateUberTrick: the store's uber trick screen (a cUIState). Methods in fe/festatestore and fe/festateriderbio.
// Size 0x60 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cTextureElement;
struct cUIEngine;
struct cUIMenu;
struct cUIScreen;
struct cUIState;

struct cFEStateUberTrick : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEStateUberTrick
    int mbModelVisible;             // 0x048 [O] bool
    cUIMenu* mpMainMenu;            // 0x04C [O]
    cUIMenu* mpTrickMenu;           // 0x050 [O]
    cTextureElement* mpCheckmark;   // 0x054 [O]
    cTextureElement* mpCheckbox;    // 0x058 [O]
    cTextureElement* mpDollarSign;  // 0x05C [O]
};

CHECK_OFFSET(cFEStateUberTrick, mbModelVisible, 0x048);
CHECK_OFFSET(cFEStateUberTrick, mpMainMenu, 0x04C);
CHECK_OFFSET(cFEStateUberTrick, mpTrickMenu, 0x050);
CHECK_OFFSET(cFEStateUberTrick, mpCheckmark, 0x054);
CHECK_OFFSET(cFEStateUberTrick, mpCheckbox, 0x058);
CHECK_OFFSET(cFEStateUberTrick, mpDollarSign, 0x05C);
CHECK_SIZE(cFEStateUberTrick, 0x60);

#endif
