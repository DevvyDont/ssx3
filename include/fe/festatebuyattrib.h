#ifndef FE_FESTATEBUYATTRIB_H
#define FE_FESTATEBUYATTRIB_H

// cFEStateBuyAttrib: the lodge's attribute store screen (a cUIState); cFEReal creates it with cMemMan_alloc(176, "cFEStateBuyAttrib").
// Size 0xB0 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIMenu;
struct cUIScreen;
struct cUIState;

struct cFEStateBuyAttrib {
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
    // --- cFEStateBuyAttrib
    cUIScreen* mpAttribScreen;      // 0x048 [O] mpScreen (On Tour's own member hides cUIState::mpScreen)
    int mExperience[7];             // 0x04C [O]
    int mExperienceLevel[7];        // 0x068 [O]
    int mPotentialExperience[7];    // 0x084 [O]
    int mTotalCost;                 // 0x0A0 [O]
    int mBank;                      // 0x0A4 [O]
    int mBuyPrompt;                 // 0x0A8 [O] bool
    cUIMenu* pAttribMenu;           // 0x0AC [O]
};

CHECK_OFFSET(cFEStateBuyAttrib, mpAttribScreen, 0x048);
CHECK_OFFSET(cFEStateBuyAttrib, mExperience, 0x04C);
CHECK_OFFSET(cFEStateBuyAttrib, mExperienceLevel, 0x068);
CHECK_OFFSET(cFEStateBuyAttrib, mPotentialExperience, 0x084);
CHECK_OFFSET(cFEStateBuyAttrib, mTotalCost, 0x0A0);
CHECK_OFFSET(cFEStateBuyAttrib, mBank, 0x0A4);
CHECK_OFFSET(cFEStateBuyAttrib, mBuyPrompt, 0x0A8);
CHECK_OFFSET(cFEStateBuyAttrib, pAttribMenu, 0x0AC);
CHECK_SIZE(cFEStateBuyAttrib, 0xB0);

#endif
