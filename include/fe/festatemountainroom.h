#ifndef FE_FESTATEMOUNTAINROOM_H
#define FE_FESTATEMOUNTAINROOM_H

// cFEStateMountainRoom: the lodge's mountain room screen (a cUIState); cFEReal creates it with cMemMan_alloc(200, "cFEStateMountainRoom").
// Size 0xC8 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cTextureElement;
struct cUIEngine;
struct cUIGroup;
struct cUIIcon;
struct cUIScreen;
struct cUIState;
struct cUIText;
struct tPeakPassInfo;

struct cFEStateMountainRoom {
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
    // --- cFEStateMountainRoom
    cUIGroup* mpPassGroup;                     // 0x048 [O]
    cUIGroup* mpPassFrontGroup;                // 0x04C [O]
    cUIIcon* mpPassIcon;                       // 0x050 [O]
    cUIText* mpHelpText;                       // 0x054 [O]
    cUIIcon* mpRaceMarkers[5];                 // 0x058 [O]
    cUIIcon* mpSlopeStyleMarkers[3];           // 0x06C [O]
    cUIIcon* mpRaceBackCountryMarkers[3];      // 0x078 [O]
    cUIIcon* mpFreestyleBackCountryMarkers[3]; // 0x084 [O]
    cUIIcon* mpSuperPipeMarkers[3];            // 0x090 [O]
    cUIIcon* mpBigAirMarkers[3];               // 0x09C [O]
    cTextureElement* mpDoneElem;               // 0x0A8 [O]
    cTextureElement* mpNotDoneElem;            // 0x0AC [O]
    int mCharID;                               // 0x0B0 [O]
    int mPeakNum;                              // 0x0B4 [O]
    int mLevel;                                // 0x0B8 [O]
    int mTextureFileIndex;                     // 0x0BC [O]
    tPeakPassInfo* mpPeakPassInfo;             // 0x0C0 [O]
    int mFrontTextureID;                       // 0x0C4 [O]
};

CHECK_OFFSET(cFEStateMountainRoom, mpPassGroup, 0x048);
CHECK_OFFSET(cFEStateMountainRoom, mpPassFrontGroup, 0x04C);
CHECK_OFFSET(cFEStateMountainRoom, mpPassIcon, 0x050);
CHECK_OFFSET(cFEStateMountainRoom, mpHelpText, 0x054);
CHECK_OFFSET(cFEStateMountainRoom, mpRaceMarkers, 0x058);
CHECK_OFFSET(cFEStateMountainRoom, mpSlopeStyleMarkers, 0x06C);
CHECK_OFFSET(cFEStateMountainRoom, mpRaceBackCountryMarkers, 0x078);
CHECK_OFFSET(cFEStateMountainRoom, mpFreestyleBackCountryMarkers, 0x084);
CHECK_OFFSET(cFEStateMountainRoom, mpSuperPipeMarkers, 0x090);
CHECK_OFFSET(cFEStateMountainRoom, mpBigAirMarkers, 0x09C);
CHECK_OFFSET(cFEStateMountainRoom, mpDoneElem, 0x0A8);
CHECK_OFFSET(cFEStateMountainRoom, mpNotDoneElem, 0x0AC);
CHECK_OFFSET(cFEStateMountainRoom, mCharID, 0x0B0);
CHECK_OFFSET(cFEStateMountainRoom, mPeakNum, 0x0B4);
CHECK_OFFSET(cFEStateMountainRoom, mLevel, 0x0B8);
CHECK_OFFSET(cFEStateMountainRoom, mTextureFileIndex, 0x0BC);
CHECK_OFFSET(cFEStateMountainRoom, mpPeakPassInfo, 0x0C0);
CHECK_OFFSET(cFEStateMountainRoom, mFrontTextureID, 0x0C4);
CHECK_SIZE(cFEStateMountainRoom, 0xC8);

#endif
