#ifndef FE_FESTATEPREVIEWREWARD_H
#define FE_FESTATEPREVIEWREWARD_H

// cFEStatePreviewReward: the reward preview screen (a cUIState); the reward gallery (fe/festaterewards) creates it
// with cMemMan_alloc(0x78, "cFEStatePreviewReward").
// Size 0x78 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIIcon;
struct cUIScreen;
struct cUIState;
struct cUIThing;

struct cFEStatePreviewReward {
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
    // --- cFEStatePreviewReward
    cUIIcon* mpIcon[3];             // 0x048 [O]
    cUIThing* mpGroup[3];           // 0x054 [O]
    void* mpReward;                 // 0x060 [O]
    int mCharID;                    // 0x064 [O]
    int mRewardType;                // 0x068 [O]
    int mTextureFileIndex;          // 0x06C [O]
    int mTextureID;                 // 0x070 [O]
    int mOrientation;               // 0x074 [O]
};

CHECK_OFFSET(cFEStatePreviewReward, mpIcon, 0x048);
CHECK_OFFSET(cFEStatePreviewReward, mpGroup, 0x054);
CHECK_OFFSET(cFEStatePreviewReward, mpReward, 0x060);
CHECK_OFFSET(cFEStatePreviewReward, mCharID, 0x064);
CHECK_OFFSET(cFEStatePreviewReward, mRewardType, 0x068);
CHECK_OFFSET(cFEStatePreviewReward, mTextureFileIndex, 0x06C);
CHECK_OFFSET(cFEStatePreviewReward, mTextureID, 0x070);
CHECK_OFFSET(cFEStatePreviewReward, mOrientation, 0x074);
CHECK_SIZE(cFEStatePreviewReward, 0x78);

#endif
