#ifndef FE_FESTATEREWARDS_H
#define FE_FESTATEREWARDS_H

// cFEStateRewardGalleryBase: the reward gallery and store screens' shared state (a cUIState): a grid of reward icons
// (up to 8 rows of 4 visible) with a hilighter, page and cost texts and up/down arrows. Methods in fe/festaterewards.
// Subclasses add their reward lists from 0xA24: cFEStateRewardGalleryPosters, TradingCards, ConceptArts, Toys,
// Videos and CheatChars (cFEReal creates them: cMemMan_alloc(2772, "cFEStateRewardGalleryPosters"), ...).
//
// Size 0xA24 (the subclasses' first own field; On Tour 0xA24). A FLAT layout, like main/game.h: every field at its
// retail offset, no C++ inheritance yet. Member names are EA's, from SSX On Tour's PSP DWARF (cListNode, cUIState,
// cFEStateRewardGalleryBase); every retail use falls where On Tour's order puts it (the cUIState part as in
// fe/keyboardpopup.h: retail's bools are words, which keeps the same offsets there). Evidence tags:
//   [O] On Tour's member order; [U] how retail code uses the field.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIThing;
struct cUIIcon;
struct cUIText;

// One reward icon's file name and ID (On Tour tIconInfo, 0xC).
struct tIconInfo {
    char mName[5];                  // 0x0 [O]
    char unk_5[0x3];                // 0x5 padding
    int mID;                        // 0x8 [O]
};

struct cFEStateRewardGalleryBase {
    // --- cListNode
    cListNode* mpPrev;              // 0x000 [O]
    cListNode* mpNext;              // 0x004 [O]
    void** mVtbl;                   // 0x008 [U] the subclasses' constructors install their vtables
    // --- cUIState
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]; [U] the state stack and screens
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O]
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] the gallery screen
    signed char mUserNum;           // 0x044 [O]; [U] the controlling player
    char unk_045[0x3];              // 0x045 padding
    // --- cFEStateRewardGalleryBase
    int mLoadingState;              // 0x048 [O]
    int mRewardType;                // 0x04C [O]
    int mCharID;                    // 0x050 [O]
    int mMoney;                     // 0x054 [O]
    int mMaxRewardRows;             // 0x058 [O]
    int mMaxRewardColumns;          // 0x05C [O]
    int mNumRows;                   // 0x060 [O]
    int mFirstVisibleRow;           // 0x064 [O]
    int mNumTotalRewards;           // 0x068 [O]; [U] each subclass sets it to its reward count
    int mNumUnlockedRewards;        // 0x06C [O]
    float mHorizHilighterOffset;    // 0x070 [O]
    float mVertHilighterOffset;     // 0x074 [O]
    int mNumIcons;                  // 0x078 [O]
    tIconInfo mIcons[180];          // 0x07C [O]
    int mIconsFileIndex;            // 0x8EC [O]
    int mStoreNum;                  // 0x8F0 [O]; getStoreNum, isInStore
    cUIThing* mpMenu;               // 0x8F4 [O]
    cUIThing* mpHilighter;          // 0x8F8 [O]
    cUIIcon* mpSelected;            // 0x8FC [O]
    cUIText* mpHelpText;            // 0x900 [O]
    cUIText* mpPageNumberText;      // 0x904 [O]
    cUIText* mpRewardNameText;      // 0x908 [O]
    cUIText* mpRewardCostText;      // 0x90C [O]
    cUIText* mpAvailableCashText;   // 0x910 [O]
    cUIText* mpNumRewards;          // 0x914 [O]
    cUIText* mpTotalRewards;        // 0x918 [O]
    cUIIcon* mpArrowUp;             // 0x91C [O]
    cUIIcon* mpArrowDown;           // 0x920 [O]
    cUIIcon* mpIcons[8][4];         // 0x924 [O]
    cUIThing* mpIconBacks[8][4];    // 0x9A4 [O]
};

CHECK_OFFSET(cFEStateRewardGalleryBase, mpScreen, 0x040);
CHECK_OFFSET(cFEStateRewardGalleryBase, mLoadingState, 0x048);
CHECK_OFFSET(cFEStateRewardGalleryBase, mIcons, 0x07C);
CHECK_OFFSET(cFEStateRewardGalleryBase, mIconsFileIndex, 0x8EC);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpIcons, 0x924);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpIconBacks, 0x9A4);
CHECK_SIZE(cFEStateRewardGalleryBase, 0xA24);

#endif
