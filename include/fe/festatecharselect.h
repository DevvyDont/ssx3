#ifndef FE_FESTATECHARSELECT_H
#define FE_FESTATECHARSELECT_H

// cFEStateCharSelect: the character select screen (a cUIState); cFEReal creates it with cMemMan_alloc(144, "cFEStateCharSelect").
// Size 0x90 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;
struct cUIThing;

struct cFEStateCharSelect {
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
    // --- cFEStateCharSelect
    cUIThing* mpSelectedCharWidget; // 0x048 [O]
    int mCheatCharID;               // 0x04C [O]
    int mSaveCurrentCtrl;           // 0x050 [O]
    int mbIsFirst;                  // 0x054 [O] bool
    float mZoom;                    // 0x058 [O]
    float mPan;                     // 0x05C [O]
    float mPlayerRotation;          // 0x060 [O]
    char unk_064[0xC];              // 0x064 padding
    char mCharPos[0x10];            // 0x070 [O] t4Vector
    int mbDraw3D;                   // 0x080 [O] bool
    char unk_084[0xC];              // 0x084 padding
};

CHECK_OFFSET(cFEStateCharSelect, mpSelectedCharWidget, 0x048);
CHECK_OFFSET(cFEStateCharSelect, mCheatCharID, 0x04C);
CHECK_OFFSET(cFEStateCharSelect, mSaveCurrentCtrl, 0x050);
CHECK_OFFSET(cFEStateCharSelect, mbIsFirst, 0x054);
CHECK_OFFSET(cFEStateCharSelect, mZoom, 0x058);
CHECK_OFFSET(cFEStateCharSelect, mPan, 0x05C);
CHECK_OFFSET(cFEStateCharSelect, mPlayerRotation, 0x060);
CHECK_OFFSET(cFEStateCharSelect, mCharPos, 0x070);
CHECK_OFFSET(cFEStateCharSelect, mbDraw3D, 0x080);
CHECK_SIZE(cFEStateCharSelect, 0x90);

#endif
