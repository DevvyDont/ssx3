#ifndef FE_FESTATEOPTIONSGAME_H
#define FE_FESTATEOPTIONSGAME_H

// cFEStateOptionsGame: a front-end state (a cUIState), 6 methods; allocated with cMemMan_alloc(76, "cFEStateOptionsGame").
// Size 0x4C [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;
struct cUIState;

struct cFEStateOptionsGame {
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
    // --- cFEStateOptionsGame
    char mLanguageSlot;             // 0x048 [O]
    char mSpeedSlot;                // 0x049 [O]
    char mScreenSlot;               // 0x04A [O]
    char unk_04B[0x1];              // 0x04B padding
};

CHECK_OFFSET(cFEStateOptionsGame, mLanguageSlot, 0x048);
CHECK_OFFSET(cFEStateOptionsGame, mSpeedSlot, 0x049);
CHECK_OFFSET(cFEStateOptionsGame, mScreenSlot, 0x04A);
CHECK_SIZE(cFEStateOptionsGame, 0x4C);

#endif
