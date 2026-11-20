#ifndef FE_FESTATEPEAKROOM_H
#define FE_FESTATEPEAKROOM_H

// cFEStatePeakRoom: a front-end state (a cUIState), 8 methods; allocated with cMemMan_alloc(228, "cFEStatePeakRoom").
// Size 0xE4 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cTextureElement;
struct cUIEngine;
struct cUIIcon;
struct cUIScreen;
struct cUIState;
struct cUIText;
struct cUIThing;

struct cFEStatePeakRoom {
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
    // --- cFEStatePeakRoom
    cUIIcon* mpTrophyIcons[4];           // 0x048 [O]
    cUIIcon* mpMedals[5];                // 0x058 [O]
    cUIIcon* mpMedalsBackground[5];      // 0x06C [O]
    cUIText* mpMedalsText[5];            // 0x080 [O]
    cUIThing* mpMedalsTextBackground[5]; // 0x094 [O]
    cUIIcon* mpMedalsCheckMark[5];       // 0x0A8 [O]
    cUIIcon* mpMedalsCrossMark[5];       // 0x0BC [O]
    cUIText* mpHelpText;                 // 0x0D0 [O]
    cTextureElement* mpCheckElem;        // 0x0D4 [O]
    cTextureElement* mpNoCheckElem;      // 0x0D8 [O]
    int mCharID;                         // 0x0DC [O]
    int mPeakNum;                        // 0x0E0 [O]
};

CHECK_OFFSET(cFEStatePeakRoom, mpTrophyIcons, 0x048);
CHECK_OFFSET(cFEStatePeakRoom, mpMedals, 0x058);
CHECK_OFFSET(cFEStatePeakRoom, mpMedalsBackground, 0x06C);
CHECK_OFFSET(cFEStatePeakRoom, mpMedalsText, 0x080);
CHECK_OFFSET(cFEStatePeakRoom, mpMedalsTextBackground, 0x094);
CHECK_OFFSET(cFEStatePeakRoom, mpMedalsCheckMark, 0x0A8);
CHECK_OFFSET(cFEStatePeakRoom, mpMedalsCrossMark, 0x0BC);
CHECK_OFFSET(cFEStatePeakRoom, mpHelpText, 0x0D0);
CHECK_OFFSET(cFEStatePeakRoom, mpCheckElem, 0x0D4);
CHECK_OFFSET(cFEStatePeakRoom, mpNoCheckElem, 0x0D8);
CHECK_OFFSET(cFEStatePeakRoom, mCharID, 0x0DC);
CHECK_OFFSET(cFEStatePeakRoom, mPeakNum, 0x0E0);
CHECK_SIZE(cFEStatePeakRoom, 0xE4);

#endif
