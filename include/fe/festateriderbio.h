#ifndef FE_FESTATERIDERBIO_H
#define FE_FESTATERIDERBIO_H

// cFEStateRiderBio: the rider bio screen (a cUIState); cFEReal creates it with cMemMan_alloc(80, "cFEStateRiderBio").
// Size 0x50 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateRiderBio {
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
    // --- cFEStateRiderBio
    int mCharID;                    // 0x048 [O]
    int mScreen;                    // 0x04C [O]
};

CHECK_OFFSET(cFEStateRiderBio, mCharID, 0x048);
CHECK_OFFSET(cFEStateRiderBio, mScreen, 0x04C);
CHECK_SIZE(cFEStateRiderBio, 0x50);

#endif
