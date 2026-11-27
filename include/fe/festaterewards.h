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

struct cTextureElement;
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

struct SHAPEFILE;

struct cFEStateRewardGalleryBase : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [U] the subclasses' constructors install their vtables
    // cFEStateRewardGalleryBase's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // --- cFEStateRewardGalleryBase
    // cFEStateRewardGalleryBase's own slots ([R] cFEStateRewardGalleryBase_vtbl; types and order [O] On Tour's class,
    // whose extra getIconID(const char*) is retail's non-virtual cFEStateRewardGalleryBase_getIconID):
    virtual const char* getIconsFileName();                                         // 25
    virtual int getIconID(int index);                                               // 26 (retail getIconID1)
    virtual const cTextureElement* getTextureElement(int index)
        __asm__("cFEStateRewardGalleryBase_getTextureElement__FPv");                    // 27
    virtual void allocRewards()
        __asm__("cFEStateRewardGalleryBase_allocRewards__FPv");                                                    // 28
    virtual void purgeRewards()
        __asm__("cFEStateRewardGalleryBase_purgeRewards__FPv");                                                    // 29
    virtual void buyReward(int rIndex)
        __asm__("cFEStateRewardGalleryBase_buyReward");                                             // 30
    virtual void showReward(int rIndex)
        __asm__("cFEStateRewardGalleryBase_showReward");                                            // 31
    virtual void showBuyPopup(int rIndex)
        __asm__("cFEStateRewardGalleryBase_showBuyPopup");                                          // 32
    virtual void updateHelpText(int rIndex)
        __asm__("cFEStateRewardGalleryBase_updateHelpText");                                        // 33
    virtual const void* getReward(int index)
        __asm__("cFEStateRewardGalleryBase_getReward__FPv");                                       // 34
    virtual int getRewardCost(const void* pReward)
        __asm__("cFEStateRewardGalleryBase_getRewardCost__FPv");                                 // 35
    virtual int getRewardStoreNum(const void* pReward)
        __asm__("cFEStateRewardGalleryBase_getRewardStoreNum__FPv");                             // 36
    virtual const char* getRewardName(const void* pReward)
        __asm__("cFEStateRewardGalleryBase_getRewardName__FPv");                         // 37
    virtual bool isRewardOwned(int rIndex)
        __asm__("cFEStateRewardGalleryBase_isRewardOwned");                                         // 38
    virtual void updateRow(int rowNum)
        __asm__("cFEStateRewardGalleryBase_updateRow");                                             // 39
    virtual void updateList()
        __asm__("cFEStateRewardGalleryBase_updateList");                                                      // 40
    virtual void updatePageNumber()
        __asm__("cFEStateRewardGalleryBase_updatePageNumber");                                                // 41
    virtual void updateRewardInfo()
        __asm__("cFEStateRewardGalleryBase_updateRewardInfo");                                                // 42
    virtual void updateAvailableCash()
        __asm__("cFEStateRewardGalleryBase_updateAvailableCash");                                             // 43
    virtual void updateCounter()
        __asm__("cFEStateRewardGalleryBase_updateCounter");                                                   // 44
    virtual void updateHighlither()
        __asm__("cFEStateRewardGalleryBase_updateHighlither");                                                // 45
    virtual void updateArrows()
        __asm__("cFEStateRewardGalleryBase_updateArrows");                                                    // 46
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

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRewardGalleryBase1() __asm__("cFEStateRewardGalleryBase_cFEStateRewardGalleryBase1");
    int getStoreNum() __asm__("cFEStateRewardGalleryBase_getStoreNum");
    unsigned int isInStore() __asm__("cFEStateRewardGalleryBase_isInStore");
    int isIconsLoaded() __asm__("cFEStateRewardGalleryBase_isIconsLoaded");
    int isRewardsLoaded() __asm__("cFEStateRewardGalleryBase_isRewardsLoaded");
    void loadIcons() __asm__("cFEStateRewardGalleryBase_loadIcons");
    void updateIconsLoad() __asm__("cFEStateRewardGalleryBase_updateIconsLoad");
    void allocIcons(SHAPEFILE* data) __asm__("cFEStateRewardGalleryBase_allocIcons");
    void purgeIcons() __asm__("cFEStateRewardGalleryBase_purgeIcons");
    void loadRewards() __asm__("cFEStateRewardGalleryBase_loadRewards");
    void updateRewardsLoad() __asm__("cFEStateRewardGalleryBase_updateRewardsLoad");
    virtual void onCreateScreen() __asm__("cFEStateRewardGalleryBase_onCreateScreen");
    virtual void onUpdate() __asm__("cFEStateRewardGalleryBase_onUpdate");
    virtual bool onInputBegin(cUIInput* in) __asm__("cFEStateRewardGalleryBase_onInputBegin");
    int getIconID1() __asm__("cFEStateRewardGalleryBase_getIconID1__FPv");
    virtual void onGainFocus(cUIState*) __asm__("cFEStateRewardGalleryBase_onGainFocus");
    virtual void onPopupEvent(cUIState* widget, unsigned int msg) __asm__("cFEStateRewardGalleryBase_onPopupEvent");
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
struct cFEStateRewardGalleryPosters : public cFEStateRewardGalleryBase {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryPosters_vtbl (0x0046A248), stored by both ctors and the dtor
    // cFEStateRewardGalleryPosters's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // cFEStateRewardGalleryBase (fe/festaterewards.h, 0xA24)
    // --- cFEStateRewardGalleryPosters
    const tPosterInfo* mpDBPosters; // 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tPosterInfo* mpPosters[43];// 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one, getIconID reads its icon name

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRewardGalleryPosters1() __asm__("cFEStateRewardGalleryPosters_cFEStateRewardGalleryPosters1");
    // retail's destructor cFEStateRewardGalleryPosters__cFEStateRewardGalleryPosters stays a C function (g++ 2.95 takes
    // no asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateRewardGalleryPosters();
    virtual void allocRewards() __asm__("cFEStateRewardGalleryPosters_allocRewards");
    virtual void purgeRewards() __asm__("cFEStateRewardGalleryPosters_purgeRewards");
    virtual const void* getReward(int i) __asm__("cFEStateRewardGalleryPosters_getReward__FPvi");
    virtual int getRewardStoreNum(const void* other) __asm__("cFEStateRewardGalleryPosters_getRewardStoreNum__FPvT0");
    virtual const char* getRewardName(const void* other) __asm__("cFEStateRewardGalleryPosters_getRewardName__FPvT0");
    virtual const char* getIconsFileName() __asm__("cFEStateRewardGalleryPosters_getIconsFileName__FPv");
};

CHECK_OFFSET(cFEStateRewardGalleryPosters, mpDBPosters, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryPosters, mpPosters, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryPosters, 0xAD4);

// cFEStateRewardGalleryTradingCards: size 0xBF8 (see above).
struct cFEStateRewardGalleryTradingCards : public cFEStateRewardGalleryBase {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryTradingCards_vtbl (0x0046A0C8), stored by both ctors and the dtor
    // cFEStateRewardGalleryTradingCards's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // cFEStateRewardGalleryBase (fe/festaterewards.h, 0xA24)
    // --- cFEStateRewardGalleryTradingCards
    const tTradingCardInfo* mpDBTradingCards;// 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tTradingCardInfo* mpTradingCards[116];// 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one, getIconID reads its icon name

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRewardGalleryTradingCards1()
        __asm__("cFEStateRewardGalleryTradingCards_cFEStateRewardGalleryTradingCards1");

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cFEStateRewardGalleryTradingCards__cFEStateRewardGalleryTradingCards stays a C function (g++
    // 2.95 takes no asm label on a destructor); declared first, it keeps the vtable out of the units defining the
    // overrides
    virtual ~cFEStateRewardGalleryTradingCards();
    virtual void allocRewards() __asm__("cFEStateRewardGalleryTradingCards_allocRewards");
    virtual void purgeRewards() __asm__("cFEStateRewardGalleryTradingCards_purgeRewards");
    virtual const void* getReward(int i) __asm__("cFEStateRewardGalleryTradingCards_getReward__FPvi");
    virtual int getRewardStoreNum(const void* other)
        __asm__("cFEStateRewardGalleryTradingCards_getRewardStoreNum__FPvT0");
    virtual const char* getRewardName(const void* other)
        __asm__("cFEStateRewardGalleryTradingCards_getRewardName__FPvT0");
    virtual const char* getIconsFileName() __asm__("cFEStateRewardGalleryTradingCards_getIconsFileName__FPv");
};

CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpDBTradingCards, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryTradingCards, mpTradingCards, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryTradingCards, 0xBF8);

// cFEStateRewardGalleryConceptArts: size 0xBB8 (see above).
struct cFEStateRewardGalleryConceptArts : public cFEStateRewardGalleryBase {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryConceptArts_vtbl (0x00469F48), stored by both ctors and the dtor
    // cFEStateRewardGalleryConceptArts's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // cFEStateRewardGalleryBase (fe/festaterewards.h, 0xA24)
    // --- cFEStateRewardGalleryConceptArts
    const tConceptArtInfo* mpDBConceptArts;// 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tConceptArtInfo* mpConceptArts[100];// 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one, getIconID reads its icon name

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRewardGalleryConceptArts1()
        __asm__("cFEStateRewardGalleryConceptArts_cFEStateRewardGalleryConceptArts1");

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cFEStateRewardGalleryConceptArts__cFEStateRewardGalleryConceptArts stays a C function (g++
    // 2.95 takes no asm label on a destructor); declared first, it keeps the vtable out of the units defining the
    // overrides
    virtual ~cFEStateRewardGalleryConceptArts();
    virtual void allocRewards() __asm__("cFEStateRewardGalleryConceptArts_allocRewards");
    virtual void purgeRewards() __asm__("cFEStateRewardGalleryConceptArts_purgeRewards");
    virtual const void* getReward(int i) __asm__("cFEStateRewardGalleryConceptArts_getReward__FPvi");
    virtual int getRewardStoreNum(const void* other)
        __asm__("cFEStateRewardGalleryConceptArts_getRewardStoreNum__FPvT0");
    virtual const char* getRewardName(const void* other)
        __asm__("cFEStateRewardGalleryConceptArts_getRewardName__FPvT0");
    virtual const char* getIconsFileName() __asm__("cFEStateRewardGalleryConceptArts_getIconsFileName__FPv");
};

CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpDBConceptArts, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryConceptArts, mpConceptArts, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryConceptArts, 0xBB8);

// cFEStateRewardGalleryVideos: size 0xA30 (see above).
struct cFEStateRewardGalleryVideos : public cFEStateRewardGalleryBase {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryVideos_vtbl (0x00469DC8), stored by both ctors and the dtor
    // cFEStateRewardGalleryVideos's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // cFEStateRewardGalleryBase (fe/festaterewards.h, 0xA24)
    // --- cFEStateRewardGalleryVideos
    const tVideoInfo* mpDBVideos;   // 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tVideoInfo* mpVideos[2];  // 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one, getIconID reads its icon name

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRewardGalleryVideos1() __asm__("cFEStateRewardGalleryVideos_cFEStateRewardGalleryVideos1");
    // retail's destructor cFEStateRewardGalleryVideos__cFEStateRewardGalleryVideos stays a C function (g++ 2.95 takes
    // no asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateRewardGalleryVideos();
    virtual void allocRewards() __asm__("cFEStateRewardGalleryVideos_allocRewards");
    virtual void purgeRewards() __asm__("cFEStateRewardGalleryVideos_purgeRewards");
    virtual const void* getReward(int i) __asm__("cFEStateRewardGalleryVideos_getReward__FPvi");
    virtual int getRewardStoreNum(const void* other) __asm__("cFEStateRewardGalleryVideos_getRewardStoreNum__FPvT0");
    virtual const char* getRewardName(const void* other) __asm__("cFEStateRewardGalleryVideos_getRewardName__FPvT0");
    virtual const char* getIconsFileName() __asm__("cFEStateRewardGalleryVideos_getIconsFileName__FPv");
};

CHECK_OFFSET(cFEStateRewardGalleryVideos, mpDBVideos, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryVideos, mpVideos, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryVideos, 0xA30);

// cFEStateRewardGalleryToys: size 0xA98 (see above).
struct cFEStateRewardGalleryToys : public cFEStateRewardGalleryBase {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryToys_vtbl (0x00469C48), stored by both ctors and the dtor
    // cFEStateRewardGalleryToys's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // cFEStateRewardGalleryBase (fe/festaterewards.h, 0xA24)
    // --- cFEStateRewardGalleryToys
    const tToyInfo* mpDBToys;       // 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tToyInfo* mpToys[28];     // 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one, getIconID reads its icon name

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRewardGalleryToys1() __asm__("cFEStateRewardGalleryToys_cFEStateRewardGalleryToys1");
    // retail's destructor cFEStateRewardGalleryToys__cFEStateRewardGalleryToys stays a C function (g++ 2.95 takes no
    // asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateRewardGalleryToys();
    virtual void allocRewards() __asm__("cFEStateRewardGalleryToys_allocRewards");
    virtual void purgeRewards() __asm__("cFEStateRewardGalleryToys_purgeRewards");
    virtual const void* getReward(int i) __asm__("cFEStateRewardGalleryToys_getReward__FPvi");
    virtual int getRewardStoreNum(const void* other) __asm__("cFEStateRewardGalleryToys_getRewardStoreNum__FPvT0");
    virtual const char* getRewardName(const void* other) __asm__("cFEStateRewardGalleryToys_getRewardName__FPvT0");
    virtual const char* getIconsFileName() __asm__("cFEStateRewardGalleryToys_getIconsFileName__FPv");
};

CHECK_OFFSET(cFEStateRewardGalleryToys, mpDBToys, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryToys, mpToys, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryToys, 0xA98);

// cFEStateRewardGalleryCheatChars: size 0xA98 (see above).
struct cFEStateRewardGalleryCheatChars : public cFEStateRewardGalleryBase {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateRewardGalleryCheatChars_vtbl (0x00469AC8), stored by both ctors and the dtor
    // cFEStateRewardGalleryCheatChars's notes on cUIState's fields:
    //   mpEngine: [U] the state stack and screens
    //   mpScreen: [U] the gallery screen
    //   mUserNum: [U] the controlling player
    // cFEStateRewardGalleryBase (fe/festaterewards.h, 0xA24)
    // --- cFEStateRewardGalleryCheatChars
    const tCheatCharInfo* mpDBCheatChars;// 0xA24 [O]; [R] 0; [U] allocRewards stores the database array, purgeRewards clears it
    const tCheatCharInfo* mpCheatChars[28];// 0xA28 [O]; [R] all 0; [U] allocRewards fills mNumTotalRewards entries
                                    //       (database + i * entry size), purgeRewards clears them, getReward
                                    //       returns one; getTextureElement looks up its texture

    // Methods bound to their retail symbols by asm labels.
    void* cFEStateRewardGalleryCheatChars1()
        __asm__("cFEStateRewardGalleryCheatChars_cFEStateRewardGalleryCheatChars1");

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cFEStateRewardGalleryCheatChars__cFEStateRewardGalleryCheatChars stays a C function (g++ 2.95
    // takes no asm label on a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateRewardGalleryCheatChars();
    virtual void allocRewards() __asm__("cFEStateRewardGalleryCheatChars_allocRewards");
    virtual void purgeRewards() __asm__("cFEStateRewardGalleryCheatChars_purgeRewards");
    virtual const void* getReward(int i) __asm__("cFEStateRewardGalleryCheatChars_getReward__FPvi");
    virtual int getRewardStoreNum(const void* other)
        __asm__("cFEStateRewardGalleryCheatChars_getRewardStoreNum__FPvT0");
    virtual const char* getRewardName(const void* other)
        __asm__("cFEStateRewardGalleryCheatChars_getRewardName__FPvT0");
};

CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpDBCheatChars, 0xA24);
CHECK_OFFSET(cFEStateRewardGalleryCheatChars, mpCheatChars, 0xA28);
CHECK_SIZE(cFEStateRewardGalleryCheatChars, 0xA98);

#endif /* FE_FESTATEREWARDS_H */
