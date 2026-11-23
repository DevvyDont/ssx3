#ifndef FE_FESTATEREWARDS_H
#define FE_FESTATEREWARDS_H

// cFEStateRewardGalleryBase: the reward gallery and store screens' shared state (a cUIState): a grid of reward icons
// (up to 4 rows of 8 visible; On Tour 8 rows of 4) with a hilighter, page and cost texts and up/down arrows. Methods in fe/festaterewards.
// Subclasses add their reward lists from 0xA24: cFEStateRewardGalleryPosters, TradingCards, ConceptArts, Toys,
// Videos and CheatChars (cFEReal creates them: cMemMan_alloc(2772, "cFEStateRewardGalleryPosters"), ...).
//
// Size 0xA24 (the subclasses' first own field; On Tour 0xA24). Derives from cUIState (ui/uistate.h); its own members at
// their retail offsets. Member names are EA's, from SSX On Tour's PSP DWARF (cListNode, cUIState,
// cFEStateRewardGalleryBase); every retail use falls where On Tour's order puts it (the cUIState part as in
// fe/keyboardpopup.h: retail's bools are words, which keeps the same offsets there). Evidence tags:
//   [O] On Tour's member order; [U] how retail code uses the field.

#include "layout.h"
#include "ui/uistate.h"

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

struct cFEStateRewardGalleryBase : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [U] the subclasses' constructors install their vtables
    // cFEStateRewardGalleryBase's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
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
    cUIIcon* mpIcons[4][8];         // 0x924 [O] ([8][4] in On Tour); [U] retail indexes [row][col] with a 0x20 row stride
                                    //       (cFEStateRewardGalleryBase_updateRow: sll row, 5; onWidgetCreate fills [row][col])
    cUIThing* mpIconBacks[4][8];    // 0x9A4 [O] ([8][4] in On Tour); [U] [row][col], 0x20 row stride (as mpIcons)
};

CHECK_OFFSET(cFEStateRewardGalleryBase, mLoadingState, 0x048);
CHECK_OFFSET(cFEStateRewardGalleryBase, mRewardType, 0x04C);
CHECK_OFFSET(cFEStateRewardGalleryBase, mCharID, 0x050);
CHECK_OFFSET(cFEStateRewardGalleryBase, mMoney, 0x054);
CHECK_OFFSET(cFEStateRewardGalleryBase, mMaxRewardRows, 0x058);
CHECK_OFFSET(cFEStateRewardGalleryBase, mMaxRewardColumns, 0x05C);
CHECK_OFFSET(cFEStateRewardGalleryBase, mNumRows, 0x060);
CHECK_OFFSET(cFEStateRewardGalleryBase, mFirstVisibleRow, 0x064);
CHECK_OFFSET(cFEStateRewardGalleryBase, mNumTotalRewards, 0x068);
CHECK_OFFSET(cFEStateRewardGalleryBase, mNumUnlockedRewards, 0x06C);
CHECK_OFFSET(cFEStateRewardGalleryBase, mHorizHilighterOffset, 0x070);
CHECK_OFFSET(cFEStateRewardGalleryBase, mVertHilighterOffset, 0x074);
CHECK_OFFSET(cFEStateRewardGalleryBase, mNumIcons, 0x078);
CHECK_OFFSET(cFEStateRewardGalleryBase, mIcons, 0x07C);
CHECK_OFFSET(cFEStateRewardGalleryBase, mIconsFileIndex, 0x8EC);
CHECK_OFFSET(cFEStateRewardGalleryBase, mStoreNum, 0x8F0);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpMenu, 0x8F4);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpHilighter, 0x8F8);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpSelected, 0x8FC);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpHelpText, 0x900);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpPageNumberText, 0x904);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpRewardNameText, 0x908);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpRewardCostText, 0x90C);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpAvailableCashText, 0x910);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpNumRewards, 0x914);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpTotalRewards, 0x918);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpArrowUp, 0x91C);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpArrowDown, 0x920);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpIcons, 0x924);
CHECK_OFFSET(cFEStateRewardGalleryBase, mpIconBacks, 0x9A4);
CHECK_SIZE(cFEStateRewardGalleryBase, 0xA24);


// ---------------------------------------------------------------------------------------------------------------------
// The six reward galleries: each is a cFEStateRewardGalleryBase (written out inline below, one copy per gallery; keep
// them in step with the base above) plus a pointer to its reward database array and a list of pointers to its
// rewards. EA's source is fe/festaterewards.cpp (builds/x/v2 file_hints for the base's methods); retail code: each
// gallery's ctor, getIconsFileName, getIconID, allocRewards, purgeRewards (CheatChars: getTextureElement) in
// fe/festaterewards (0x001D1128 ... 0x001D1A60), the engine-less ctor copy, destructor, getReward, getRewardCost,
// getRewardStoreNum and getRewardName out of line in fe/fepopupmisc (0x001DFAF0 ... 0x001DFFC8).
//
// Sizes (each [R] = [O]; On Tour's gallery classes have exactly retail's sizes):
//   [R] cFECustom_getNextState (fe/fereal) allocates each with cMemMan_alloc(size, "<class>") and runs
//       <class>_<class>(mem, pEngine, userNum, inStore), which runs cFEStateRewardGalleryBase_cFEStateRewardGalleryBase
//       (.., rewardType), stores the gallery's vtable, zeroes mNumTotalRewards (0x68) and the database pointer (0xA24)
//       and clears the list from its last element down (the last element's offset is in brackets):
//       Posters 2772 = 0xAD4 (0xAD0: 43 entries), TradingCards 3064 = 0xBF8 (0xBF4: 116), ConceptArts 3000 = 0xBB8
//       (0xBB4: 100), Videos 2608 = 0xA30 (0xA2C: 2), Toys 2712 = 0xA98 (0xA94: 28), CheatChars 2712 = 0xA98 (0xA94: 28).
//   [O] On Tour: mpDB<Rewards> at 0xA24, mp<Rewards>[N] at 0xA28 with the same N.
// [U] allocRewards stores the database array (cBERewardInterface_getDB() + a per-type offset) at 0xA24 and fills
// mNumTotalRewards entries of the list (database + i * entry size); purgeRewards clears all three again; getIconID /
// getReward / getTextureElement index the list. getRewardCost / getRewardStoreNum / getRewardName take the reward, not
// a member. Vtables (46 slots each; own slots 1 dtor, 25 getIconsFileName, 26 getIconID (CheatChars: 27
// getTextureElement), 28 allocRewards, 29 purgeRewards, 34 getReward, 35 getRewardCost, 36 getRewardStoreNum, 37
// getRewardName; the rest cFEStateRewardGalleryBase's / cUIState's).

struct tPosterInfo;
struct tTradingCardInfo;
struct tConceptArtInfo;
struct tVideoInfo;
struct tToyInfo;
struct tCheatCharInfo;

// cFEStateRewardGalleryPosters: size 0xAD4 (see above).
struct cFEStateRewardGalleryPosters : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryPosters_vtbl (0x0046A248), stored by both ctors and the dtor
    // cFEStateRewardGalleryPosters's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // --- cFEStateRewardGalleryBase (written out from the struct above)
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
    cUIIcon* mpIcons[4][8];         // 0x924 [O] ([8][4] in On Tour); [U] retail indexes [row][col] with a 0x20 row stride
                                    //       (cFEStateRewardGalleryBase_updateRow: sll row, 5; onWidgetCreate fills [row][col])
    cUIThing* mpIconBacks[4][8];    // 0x9A4 [O] ([8][4] in On Tour); [U] [row][col], 0x20 row stride (as mpIcons)
    // --- cFEStateRewardGalleryPosters
    const tPosterInfo* mpDBPosters; // 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tPosterInfo* mpPosters[43];// 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one, getIconID reads its icon name
};

CHECK_OFFSET(cFEStateRewardGalleryPosters, mLoadingState, 0x048);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mRewardType, 0x04C);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mCharID, 0x050);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mMoney, 0x054);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mMaxRewardRows, 0x058);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mMaxRewardColumns, 0x05C);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mNumRows, 0x060);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mFirstVisibleRow, 0x064);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mNumTotalRewards, 0x068);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mNumUnlockedRewards, 0x06C);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mHorizHilighterOffset, 0x070);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mVertHilighterOffset, 0x074);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mNumIcons, 0x078);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mIcons, 0x07C);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mIconsFileIndex, 0x8EC);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mStoreNum, 0x8F0);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpMenu, 0x8F4);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpHilighter, 0x8F8);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpSelected, 0x8FC);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpHelpText, 0x900);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpPageNumberText, 0x904);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpRewardNameText, 0x908);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpRewardCostText, 0x90C);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpAvailableCashText, 0x910);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpNumRewards, 0x914);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpTotalRewards, 0x918);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpArrowUp, 0x91C);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpArrowDown, 0x920);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpIcons, 0x924);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpIconBacks, 0x9A4);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpDBPosters, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpPosters, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryPosters, 0xAD4);

// cFEStateRewardGalleryTradingCards: size 0xBF8 (see above).
struct cFEStateRewardGalleryTradingCards : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryTradingCards_vtbl (0x0046A0C8), stored by both ctors and the dtor
    // cFEStateRewardGalleryTradingCards's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // --- cFEStateRewardGalleryBase (written out from the struct above)
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
    cUIIcon* mpIcons[4][8];         // 0x924 [O] ([8][4] in On Tour); [U] retail indexes [row][col] with a 0x20 row stride
                                    //       (cFEStateRewardGalleryBase_updateRow: sll row, 5; onWidgetCreate fills [row][col])
    cUIThing* mpIconBacks[4][8];    // 0x9A4 [O] ([8][4] in On Tour); [U] [row][col], 0x20 row stride (as mpIcons)
    // --- cFEStateRewardGalleryTradingCards
    const tTradingCardInfo* mpDBTradingCards;// 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tTradingCardInfo* mpTradingCards[116];// 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one, getIconID reads its icon name
};

CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mLoadingState, 0x048);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mRewardType, 0x04C);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mCharID, 0x050);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mMoney, 0x054);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mMaxRewardRows, 0x058);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mMaxRewardColumns, 0x05C);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mNumRows, 0x060);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mFirstVisibleRow, 0x064);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mNumTotalRewards, 0x068);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mNumUnlockedRewards, 0x06C);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mHorizHilighterOffset, 0x070);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mVertHilighterOffset, 0x074);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mNumIcons, 0x078);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mIcons, 0x07C);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mIconsFileIndex, 0x8EC);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mStoreNum, 0x8F0);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpMenu, 0x8F4);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpHilighter, 0x8F8);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpSelected, 0x8FC);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpHelpText, 0x900);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpPageNumberText, 0x904);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpRewardNameText, 0x908);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpRewardCostText, 0x90C);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpAvailableCashText, 0x910);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpNumRewards, 0x914);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpTotalRewards, 0x918);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpArrowUp, 0x91C);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpArrowDown, 0x920);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpIcons, 0x924);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpIconBacks, 0x9A4);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpDBTradingCards, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpTradingCards, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryTradingCards, 0xBF8);

// cFEStateRewardGalleryConceptArts: size 0xBB8 (see above).
struct cFEStateRewardGalleryConceptArts : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryConceptArts_vtbl (0x00469F48), stored by both ctors and the dtor
    // cFEStateRewardGalleryConceptArts's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // --- cFEStateRewardGalleryBase (written out from the struct above)
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
    cUIIcon* mpIcons[4][8];         // 0x924 [O] ([8][4] in On Tour); [U] retail indexes [row][col] with a 0x20 row stride
                                    //       (cFEStateRewardGalleryBase_updateRow: sll row, 5; onWidgetCreate fills [row][col])
    cUIThing* mpIconBacks[4][8];    // 0x9A4 [O] ([8][4] in On Tour); [U] [row][col], 0x20 row stride (as mpIcons)
    // --- cFEStateRewardGalleryConceptArts
    const tConceptArtInfo* mpDBConceptArts;// 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tConceptArtInfo* mpConceptArts[100];// 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one, getIconID reads its icon name
};

CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mLoadingState, 0x048);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mRewardType, 0x04C);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mCharID, 0x050);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mMoney, 0x054);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mMaxRewardRows, 0x058);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mMaxRewardColumns, 0x05C);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mNumRows, 0x060);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mFirstVisibleRow, 0x064);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mNumTotalRewards, 0x068);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mNumUnlockedRewards, 0x06C);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mHorizHilighterOffset, 0x070);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mVertHilighterOffset, 0x074);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mNumIcons, 0x078);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mIcons, 0x07C);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mIconsFileIndex, 0x8EC);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mStoreNum, 0x8F0);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpMenu, 0x8F4);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpHilighter, 0x8F8);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpSelected, 0x8FC);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpHelpText, 0x900);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpPageNumberText, 0x904);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpRewardNameText, 0x908);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpRewardCostText, 0x90C);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpAvailableCashText, 0x910);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpNumRewards, 0x914);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpTotalRewards, 0x918);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpArrowUp, 0x91C);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpArrowDown, 0x920);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpIcons, 0x924);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpIconBacks, 0x9A4);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpDBConceptArts, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpConceptArts, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryConceptArts, 0xBB8);

// cFEStateRewardGalleryVideos: size 0xA30 (see above).
struct cFEStateRewardGalleryVideos : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryVideos_vtbl (0x00469DC8), stored by both ctors and the dtor
    // cFEStateRewardGalleryVideos's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // --- cFEStateRewardGalleryBase (written out from the struct above)
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
    cUIIcon* mpIcons[4][8];         // 0x924 [O] ([8][4] in On Tour); [U] retail indexes [row][col] with a 0x20 row stride
                                    //       (cFEStateRewardGalleryBase_updateRow: sll row, 5; onWidgetCreate fills [row][col])
    cUIThing* mpIconBacks[4][8];    // 0x9A4 [O] ([8][4] in On Tour); [U] [row][col], 0x20 row stride (as mpIcons)
    // --- cFEStateRewardGalleryVideos
    const tVideoInfo* mpDBVideos;   // 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tVideoInfo* mpVideos[2];  // 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one, getIconID reads its icon name
};

CHECK_OFFSET(cFEStateRewardGalleryVideos, mLoadingState, 0x048);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mRewardType, 0x04C);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mCharID, 0x050);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mMoney, 0x054);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mMaxRewardRows, 0x058);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mMaxRewardColumns, 0x05C);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mNumRows, 0x060);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mFirstVisibleRow, 0x064);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mNumTotalRewards, 0x068);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mNumUnlockedRewards, 0x06C);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mHorizHilighterOffset, 0x070);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mVertHilighterOffset, 0x074);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mNumIcons, 0x078);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mIcons, 0x07C);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mIconsFileIndex, 0x8EC);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mStoreNum, 0x8F0);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpMenu, 0x8F4);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpHilighter, 0x8F8);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpSelected, 0x8FC);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpHelpText, 0x900);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpPageNumberText, 0x904);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpRewardNameText, 0x908);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpRewardCostText, 0x90C);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpAvailableCashText, 0x910);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpNumRewards, 0x914);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpTotalRewards, 0x918);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpArrowUp, 0x91C);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpArrowDown, 0x920);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpIcons, 0x924);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpIconBacks, 0x9A4);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpDBVideos, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpVideos, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryVideos, 0xA30);

// cFEStateRewardGalleryToys: size 0xA98 (see above).
struct cFEStateRewardGalleryToys : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryToys_vtbl (0x00469C48), stored by both ctors and the dtor
    // cFEStateRewardGalleryToys's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // --- cFEStateRewardGalleryBase (written out from the struct above)
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
    cUIIcon* mpIcons[4][8];         // 0x924 [O] ([8][4] in On Tour); [U] retail indexes [row][col] with a 0x20 row stride
                                    //       (cFEStateRewardGalleryBase_updateRow: sll row, 5; onWidgetCreate fills [row][col])
    cUIThing* mpIconBacks[4][8];    // 0x9A4 [O] ([8][4] in On Tour); [U] [row][col], 0x20 row stride (as mpIcons)
    // --- cFEStateRewardGalleryToys
    const tToyInfo* mpDBToys;       // 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tToyInfo* mpToys[28];     // 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one, getIconID reads its icon name
};

CHECK_OFFSET(cFEStateRewardGalleryToys, mLoadingState, 0x048);
CHECK_OFFSET(cFEStateRewardGalleryToys, mRewardType, 0x04C);
CHECK_OFFSET(cFEStateRewardGalleryToys, mCharID, 0x050);
CHECK_OFFSET(cFEStateRewardGalleryToys, mMoney, 0x054);
CHECK_OFFSET(cFEStateRewardGalleryToys, mMaxRewardRows, 0x058);
CHECK_OFFSET(cFEStateRewardGalleryToys, mMaxRewardColumns, 0x05C);
CHECK_OFFSET(cFEStateRewardGalleryToys, mNumRows, 0x060);
CHECK_OFFSET(cFEStateRewardGalleryToys, mFirstVisibleRow, 0x064);
CHECK_OFFSET(cFEStateRewardGalleryToys, mNumTotalRewards, 0x068);
CHECK_OFFSET(cFEStateRewardGalleryToys, mNumUnlockedRewards, 0x06C);
CHECK_OFFSET(cFEStateRewardGalleryToys, mHorizHilighterOffset, 0x070);
CHECK_OFFSET(cFEStateRewardGalleryToys, mVertHilighterOffset, 0x074);
CHECK_OFFSET(cFEStateRewardGalleryToys, mNumIcons, 0x078);
CHECK_OFFSET(cFEStateRewardGalleryToys, mIcons, 0x07C);
CHECK_OFFSET(cFEStateRewardGalleryToys, mIconsFileIndex, 0x8EC);
CHECK_OFFSET(cFEStateRewardGalleryToys, mStoreNum, 0x8F0);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpMenu, 0x8F4);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpHilighter, 0x8F8);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpSelected, 0x8FC);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpHelpText, 0x900);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpPageNumberText, 0x904);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpRewardNameText, 0x908);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpRewardCostText, 0x90C);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpAvailableCashText, 0x910);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpNumRewards, 0x914);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpTotalRewards, 0x918);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpArrowUp, 0x91C);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpArrowDown, 0x920);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpIcons, 0x924);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpIconBacks, 0x9A4);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpDBToys, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpToys, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryToys, 0xA98);

// cFEStateRewardGalleryCheatChars: size 0xA98 (see above).
struct cFEStateRewardGalleryCheatChars : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryCheatChars_vtbl (0x00469AC8), stored by both ctors and the dtor
    // cFEStateRewardGalleryCheatChars's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // --- cFEStateRewardGalleryBase (written out from the struct above)
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
    cUIIcon* mpIcons[4][8];         // 0x924 [O] ([8][4] in On Tour); [U] retail indexes [row][col] with a 0x20 row stride
                                    //       (cFEStateRewardGalleryBase_updateRow: sll row, 5; onWidgetCreate fills [row][col])
    cUIThing* mpIconBacks[4][8];    // 0x9A4 [O] ([8][4] in On Tour); [U] [row][col], 0x20 row stride (as mpIcons)
    // --- cFEStateRewardGalleryCheatChars
    const tCheatCharInfo* mpDBCheatChars;// 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tCheatCharInfo* mpCheatChars[28];// 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one; getTextureElement looks up its texture
};

CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mLoadingState, 0x048);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mRewardType, 0x04C);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mCharID, 0x050);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mMoney, 0x054);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mMaxRewardRows, 0x058);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mMaxRewardColumns, 0x05C);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mNumRows, 0x060);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mFirstVisibleRow, 0x064);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mNumTotalRewards, 0x068);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mNumUnlockedRewards, 0x06C);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mHorizHilighterOffset, 0x070);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mVertHilighterOffset, 0x074);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mNumIcons, 0x078);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mIcons, 0x07C);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mIconsFileIndex, 0x8EC);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mStoreNum, 0x8F0);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpMenu, 0x8F4);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpHilighter, 0x8F8);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpSelected, 0x8FC);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpHelpText, 0x900);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpPageNumberText, 0x904);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpRewardNameText, 0x908);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpRewardCostText, 0x90C);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpAvailableCashText, 0x910);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpNumRewards, 0x914);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpTotalRewards, 0x918);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpArrowUp, 0x91C);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpArrowDown, 0x920);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpIcons, 0x924);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpIconBacks, 0x9A4);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpDBCheatChars, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpCheatChars, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryCheatChars, 0xA98);

#endif /* FE_FESTATEREWARDS_H */
