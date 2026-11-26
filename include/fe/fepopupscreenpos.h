#ifndef FE_FEPOPUPSCREENPOS_H
#define FE_FEPOPUPSCREENPOS_H

// cFEPopupScreenPos: a front-end state (a cUIState), 3 methods; allocated with cMemMan_alloc(84, "cFEPopupScreenPos").
// Size 0x54 [R] (the retail allocation; On Tour's is the same). Derives from cUIState (ui/uistate.h); member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEPopupScreenPos : public cUIState {
    // cUIState (ui/uistate.h, 0x48)
    // --- cFEPopupScreenPos
    int mX;                         // 0x048 [O]
    int mY;                         // 0x04C [O]
    char mAccept;                   // 0x050 [O]
    char unk_051[0x3];              // 0x051 padding

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cFEPopupScreenPos__cFEPopupScreenPos stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEPopupScreenPos();
    virtual void onCreateScreen() __asm__("cFEPopupScreenPos_onCreateScreen");
    virtual void onUpdate() __asm__("cFEPopupScreenPos_onUpdate");
};

CHECK_OFFSET(cFEPopupScreenPos, mX, 0x048);
CHECK_OFFSET(cFEPopupScreenPos, mY, 0x04C);
CHECK_OFFSET(cFEPopupScreenPos, mAccept, 0x050);
CHECK_SIZE(cFEPopupScreenPos, 0x54);

#endif
