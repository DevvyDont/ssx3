#ifndef FE_FEPOPUPSCREENPOS_H
#define FE_FEPOPUPSCREENPOS_H

// cFEPopupScreenPos: a front-end state (a cUIState), 3 methods; allocated with cMemMan_alloc(84, "cFEPopupScreenPos").
// Size 0x54 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEPopupScreenPos {
    // --- cListNode
    cListNode* mpPrev;              // 0x000 [O]
    cListNode* mpNext;              // 0x004 [O]
    void** mVtbl;                   // 0x008 the vtable
    // --- cUIState
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O]
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]
    signed char mUserNum;           // 0x044 [O]
    char unk_045[0x3];              // 0x045 padding
    // --- cFEPopupScreenPos
    int mX;                         // 0x048 [O]
    int mY;                         // 0x04C [O]
    char mAccept;                   // 0x050 [O]
    char unk_051[0x3];              // 0x051 padding
};

CHECK_OFFSET(cFEPopupScreenPos, mX, 0x048);
CHECK_OFFSET(cFEPopupScreenPos, mY, 0x04C);
CHECK_OFFSET(cFEPopupScreenPos, mAccept, 0x050);
CHECK_SIZE(cFEPopupScreenPos, 0x54);

#endif
