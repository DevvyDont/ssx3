#ifndef FE_FESTATECHAREQUIP_H
#define FE_FESTATECHAREQUIP_H
#pragma interface

// cFEStateCharEquip: the "equip gear" / "buy gear" screen for one rider's bolts (inventory mode equips items, store
// mode buys them; the option button may switch between the two). It lists the items of a category stack in six rows,
// shows the money / equip-point bars, an icon or a large preview of the selected item, and poses the user's FE rider
// and board (cFERider, fe/ferider.h) with a zoom / pan / turn camera.
// EA's source is fe/festatecharequip.cpp (the July 2003 preview's assert strings, builds/x/v2 file_hints: retail
// cFEStateCharEquip_onLoseFocus / _onInputBegin). Retail code: fe/festatecharequipdetail 0x00198DB8 ... 0x0019C7E8 and
// the out-of-line accessors in fe/fepopupmisc (0x001DF9B0 ... 0x001DF9D0).
//
// Size 0xAF0:
//   [R] cFECustom_getNextState (fe/fereal) allocates it three times as cMemMan_alloc(2800, "cFEStateCharEquip") +
//       cFEStateCharEquip_cFEStateCharEquip(mem, engine, userNum, inStore, canSwitchMode); the last field written is
//       0xAE4 and the two t4Vectors make the class 16-byte aligned, so 0xAE8-0xAF0 is tail padding.
//   On Tour's DWARF says 0x240: SSX 3 keeps a category stack and an item list (0xD0-0x95C) that On Tour does not have,
//   and its bools are words; with those, On Tour's members fall on every retail use below, in On Tour's order.
// Base: cFEState (On Tour 0x48, no data of its own; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode
// (vptr at 0x8). The constructor runs cUIState_cUIState and stores cFEStateCharEquip_vtbl (D_0046A548, 24 slots: 1
// dtor, 4 onCreateScreen, 5 onDestroyScreen, 6 onGainFocus, 8 onInputBegin, 11 onLoseFocus, 12 onUpdate, 17
// onPopupEvent, 18 onWidgetCreate, 19 onWidgetEvent, 20 onWidgetMenu); the destructor puts cFEState_vtbl back and
// calls cUIState__cUIState.
//
// Derives from cFEState (fe/festate.h); its own members at their retail offsets. bools are ints (ee-gcc 2.95's bool is
// 4 bytes; the C bodies' `*(bool*)` reads compile to lw).
// Evidence tags:
//   [R] retail's constructor cFEStateCharEquip_cFEStateCharEquip / destructor cFEStateCharEquip__cFEStateCharEquip;
//   [P] the July 2003 preview (layout_xlate: slot-4 onCreateScreen 0x3A34/0x3A38/0x3A3C -> 0xAA4/0xAA8/0xAAC);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cFEStateCharEquip, cUIState, cListNode): names, types and order.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/festate.h"
#include "util/list.h"
#include "bx/math3d.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIThing;
struct cUIText;
struct cUIIcon;
struct cUIMenu;
struct cTextureElement;
struct tBEBoltDef;

struct cBECharProfileDB;

struct cFEStateCharEquip : public cFEState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateCharEquip_vtbl (ctor; dtor first), cFEState_vtbl (dtor)
    // cFEStateCharEquip's notes on cUIState's fields:
    //   mpEngine: [U] the state stack (+0x18) and texture bank (+0x58) are reached through it
    //   mCtrlBits: [R] the ctor stores cFECustom_getPlayerCtrlMask(engine, userNum)
    //   mAttrState: [U] onPopupEvent clears / sets bit 3 (mVisible) while its buy popup is up (lw/sw)
    //   mScreens: [U] onUpdate: cList_first(&mScreens)
    //   mUserNum: [U] passed to the BE player / economy / shop interfaces and cFERiderManager everywhere
    // cFEState (fe/festate.h, 0x48)
    // --- cFEStateCharEquip
    cUIText* mpTitleText;           // 0x048 [O]; [R] 0; [U] onWidgetCreate "title_equip_character" (kT_CMNBuyGear /
                                    //       kT_CMNEquipGear)
    cUIText* mpHelpText;            // 0x04C [O]; [R] 0; [U] "helptext", updateHelpText
    cUIText* mpButtonHelpText;      // 0x050 [O]; [R] 0; [U] "btext5" (the switch-mode button), switchEquipMode
    cUIIcon* mpButtonHelpIcon;      // 0x054 [O]; [R] 0; [U] "ps2sq" / "xby" / "ngcy"
    cUIText* mpHeading;             // 0x058 [O]; [R] 0; [U] "heading", updateHeading
    cUIMenu* mpMenu;                // 0x05C [O]; [R] 0; [U] "MnuGroups": cUIMenu_setScrollableMenu(mpMenu, mNumItems)
    cUIThing* mpMoneyGroup;         // 0x060 [O]; [R] 0; [U] "buytotals" (shown in store mode only)
    cUIText* mpBank;                // 0x064 [O]; [R] 0; [U] "1right", updateBankWidget (getDollarString(money))
    cUIText* mpCost;                // 0x068 [O]; [R] 0; [U] "2right", the store preview's price x 10
    cUIThing* mpPointsGroup;        // 0x06C [O]; [R] 0; [U] "barandpoints"
    cUIThing* mpPointsBar;          // 0x070 [O]; [R] 0; [U] "overbar", updatePolyBar
    cUIThing* mpPointsEquipBar;     // 0x074 [O]; [U] "overbar2", updatePolyEquipPreviewBar (the ctor does not clear it)
    cUIText* mpPointsBarValue;      // 0x078 [O]; [R] 0; [U] "equip_points"
    cUIText* mpPointsCost;          // 0x07C [O]; [R] 0; [U] "points"
    cUIThing* mpSmallIconPreviewGroup; // 0x080 [O]; [R] 0; [U] "equip_btm_left"
    cUIIcon* mpSmallBoltIcon;       // 0x084 [O]; [R] 0; [U] "bolt01", the inventory preview's icon
    cUIIcon* mpLargePreview;        // 0x088 [O]; [R] 0; [U] "largeshape", the store preview texture (mPreviewTextureID)
    cUIText* mpBoltNameWidgets[6];  // 0x08C [O]; [R] 0 x6; [U] rows "1".."6" (misc data 0-5): item names
    cUIIcon* mpBoltCheckWidgets[6]; // 0x0A4 [O]; [R] 0 x6; [U] "check1".."check6": the dollar / check box textures
    int mCharID;                    // 0x0BC [O]; [R] cBENewPlayerInterface_getPlayerCharID(player, mUserNum)
    int mMoneyAvailable;            // 0x0C0 [O]; [R] cBEEconInterface_getMoney(.., mUserNum, mCharID); [U] buying
                                    //       subtracts the price, the store rows colour by it
    int mCurrentPolyCount;          // 0x0C4 [O]; [R] cBEBoltInterface_getUsedEquipPoints(shop, mUserNum, mCharID) (the equip points)
    int mEquipMode;                 // 0x0C8 [O]; [R] 1 = store (inStore), 0 = inventory; [U] isInStore tests == 1,
                                    //       switchEquipMode flips it
    int mStoreNum;                  // 0x0CC [O]; [R] the last hub's peak (1/2/3) with a store or mode switch, else -1;
                                    //       [U] the store lists only items of this store number (cFEStateCharEquip_buildItemList)
    int mCategoryStack[20];         // 0x0D0 [U] (no On Tour member) MEM_fill(.., kBolt_None, 80) in the ctor and
                                    //       switchEquipMode; onWidgetEvent pushes the opened category id at
                                    //       [mCurrentStackPos], back pops it
    int mCurrentStackPos;           // 0x120 [O] (On Tour's member after mStoreNum); [R] 0; [U] the depth into
                                    //       mCategoryStack (onWidgetMenu event 7 steps it down)
    tBEBoltDef* mpItems[0x20D];     // 0x124 [U] (no On Tour member) the listed items (the BE library's bolt
                                    //       definitions, be/bedbprofile.h) built by cFEStateCharEquip_buildItemList (MEM_fill(mpItems,
                                    //       0, 2100))
    int mNumItems;                  // 0x958 [R] 0; [U] the count of mpItems (cFEStateCharEquip_buildItemList clears and appends,
                                    //       cFEStateCharEquip_removeItem removes one); cFEStateCharEquip_redrawRows passes its low byte (lbu) to
                                    //       cUIMenu_setScrollableMenu
    int mLargePreviewDelay;         // 0x95C [O]; [R] -1; [U] startLargePreviewLoad sets 20 or 1, updatePreviewLoad
                                    //       counts it down, cancelLargePreviewLoad sets -1
    char mLargePreviewFileName[255]; // 0x960 [O]; [R] [0] = 0; [U] startLargePreviewLoad builds the file name here
    char unk_A5F[0x1];              // 0xA5F padding
    int mIconsLoaded;               // 0xA60 [O] (bool); [R] 0; [U] the out-of-line isIconsLoaded cFEStateCharEquip_isIconsLoaded (lw);
                                    //       onUpdate sets it once the icon file is in (cFEBoltFileManager_isFileLoaded)
    int mIconsFileIndex;            // 0xA64 [O]; [R] -1, then cFEBoltFileManager_requestFile(RealFE + 0x1A70, mCharID, name, 9, 1, 0);
                                    //       [R] the dtor releases it (cFEBoltFileManager_releaseFile) and sets -1
    int mIconTextureID;             // 0xA68 [O]; [R] -1; [U] the dtor releases it (graphics manager slot 50)
    int mPreviewTextureFileIndex;   // 0xA6C [O]; [R] -1; [U] updatePreviewLoad queues the large preview file here
    int mPreviewTextureID;          // 0xA70 [O]; [R] -1; the dtor releases it
    int mOldPreviewTextureID;       // 0xA74 [O]; [R] -1; the dtor releases it
    int mRowPoints[6];              // 0xA78 [U] (no On Tour member) [R] -99999 x6; cFEStateCharEquip_drawInventoryRow stores each inventory
                                    //       row's change in equip points, cFEStateCharEquip_updateInventoryPreview / onWidgetMenu read it
    cTextureElement* mpGreenDollarSignElem; // 0xA90 [O]; [R] the texture bank's "dollargreen" element
    cTextureElement* mpRedDollarSignElem;   // 0xA94 [O]; [R] "dollarred"
    cTextureElement* mpCheckMarkElem;       // 0xA98 [O]; [R] "checkbox"
    cTextureElement* mpEmptyBoxElem;        // 0xA9C [O]; [R] "empty_box"
    cTextureElement* mpPlusElem;            // 0xAA0 [O]; [R] 0; [U] cFEStateCharEquip_drawStoreRow shows it for a store category
    float mZoom;                    // 0xAA4 [O]; [P] preview 0x3A34; [U] onCreateScreen 0, onInputBegin clamps 0..1,
                                    //       update3DPositions
    float mPan;                     // 0xAA8 [O]; [P] 0x3A38; [U] onCreateScreen -1, onInputBegin clamps -1..1
    float mPlayerRotation;          // 0xAAC [O]; [P] 0x3A3C; [U] onCreateScreen 0, onInputBegin keeps it in 0..360,
                                    //       update3DPositions turns the rider by it
    float mBoardRotation;           // 0xAB0 [O]; [R] 0; [U] update3DPositions: the board's spin (one degree per call)
    char unk_AB4[0xC];              // 0xAB4 padding (t4Vector alignment)
    t4Vector mCharPos;              // 0xAC0 [O]; [R] (-105, -100, -80, 1); [U] update3DPositions (lq/sq, the rider)
    t4Vector mBoardPos;             // 0xAD0 [O]; [R] (265.75, -1073.5, -232, 1); [U] update3DPositions (the board)
    int mCameraState;               // 0xAE0 [O]; [R] 1; [U] 1-4: onInputBegin works in 1 and 4, onWidgetEvent eases to
                                    //       the board (3) and back (4), initInventoryPreview sets 1
    int mCanSwitchMode;             // 0xAE4 [O] (bool); [R] the ctor's canSwitchMode; [U] canSwitchEquipMode (lw) and
                                    //       setCanSwitchEquipMode cFEStateCharEquip_setCanSwitchEquipMode (sw); onWidgetCreate hides the button
                                    //       when 0
    char unk_AE8[0x8];              // 0xAE8 tail padding (16-byte class alignment)

    // mEquipMode's values (names ours). [R] cFEStateCharEquip_cFEStateCharEquip stores kEquipModeStore when opened
    // from a store, kEquipModeInventory otherwise; [U] cFEStateCharEquip_switchEquipMode flips it.
    enum { kEquipModeInventory = 0, kEquipModeStore = 1 };

    // mCameraState's values (names ours). [R] the constructor stores kCameraRider; [U] update3DPositions poses the
    // rider in front (1) or the board in front (2) and eases toward the board view (3) or back to the rider view (4),
    // switching to 2 / 1 when close; onWidgetEvent starts 3 on opening the boards category and 4 on leaving it;
    // onInputBegin turns / zooms / pans only in 1 and 4.
    enum { kCameraRider = 1, kCameraBoard = 2, kCameraToBoard = 3, kCameraToRider = 4 };

    // mIconTextureID before the first icon texture exists (name ours): [R] the constructor and destructor store -1;
    // [U] getIconID creates the texture when it is -1, otherwise reloads it.
    enum { kNoTextureID = -1 };

    // Methods bound to their retail symbols by asm labels.
    bool isInStore() __asm__("cFEStateCharEquip_isInStore");
    int isIconsLoaded() __asm__("cFEStateCharEquip_isIconsLoaded");
    void setCanSwitchEquipMode(int value) __asm__("cFEStateCharEquip_setCanSwitchEquipMode");
    int canSwitchEquipMode() __asm__("cFEStateCharEquip_canSwitchEquipMode");
    int getIconID(const char* name) __asm__("cFEStateCharEquip_getIconID");
    void updateIconTexturesLoad() __asm__("cFEStateCharEquip_updateIconTexturesLoad");
    void initInventoryPreview() __asm__("cFEStateCharEquip_initInventoryPreview");
    void initStorePreview() __asm__("cFEStateCharEquip_initStorePreview");
    void switchEquipMode() __asm__("cFEStateCharEquip_switchEquipMode");
    void cancelLargePreviewLoad() __asm__("cFEStateCharEquip_cancelLargePreviewLoad");
    void startLargePreviewLoad(int flag) __asm__("cFEStateCharEquip_startLargePreviewLoad");
    void cancelPreviewLoad() __asm__("cFEStateCharEquip_cancelPreviewLoad");
    void updateHelpText() __asm__("cFEStateCharEquip_updateHelpText");
    void updateCharacterModel() __asm__("cFEStateCharEquip_updateCharacterModel");
    void updateBankWidget() __asm__("cFEStateCharEquip_updateBankWidget");
    void updateHeading(int id) __asm__("cFEStateCharEquip_updateHeading");
    void updatePolyBar() __asm__("cFEStateCharEquip_updatePolyBar");
    void updatePolyEquipPreviewBar(int delta) __asm__("cFEStateCharEquip_updatePolyEquipPreviewBar");
    void update3DPositions() __asm__("cFEStateCharEquip_update3DPositions");
    virtual ~cFEStateCharEquip();   // g++'s own symbol _$_17cFEStateCharEquip (the retail label renamed)
    virtual void onCreateScreen() __asm__("cFEStateCharEquip_onCreateScreen");
    virtual void onDestroyScreen() __asm__("cFEStateCharEquip_onDestroyScreen");
    virtual void onUpdate() __asm__("cFEStateCharEquip_onUpdate");
    virtual void onWidgetEvent(cUIThing* w, unsigned int ev) __asm__("cFEStateCharEquip_onWidgetEvent");
    virtual bool onInputBegin(cUIInput* in) __asm__("cFEStateCharEquip_onInputBegin");
    virtual void onPopupEvent(cUIState* widget, unsigned int msg) __asm__("cFEStateCharEquip_onPopupEvent");
    void updatePreviewLoad() __asm__("cFEStateCharEquip_updatePreviewLoad");
    void updatePreview() __asm__("cFEStateCharEquip_updatePreview");
    void updateInventoryPreview() __asm__("cFEStateCharEquip_updateInventoryPreview");
    void updateStorePreview() __asm__("cFEStateCharEquip_updateStorePreview");
    void scrollRows(int n) __asm__("cFEStateCharEquip_scrollRows");
    bool hasInventoryItemInCategory(int library, cBECharProfileDB* db, int itemId)
        __asm__("cFEStateCharEquip_hasInventoryItemInCategory");
    bool isStoreCategoryEmpty(int library, cBECharProfileDB* db, int itemId)
        __asm__("cFEStateCharEquip_isStoreCategoryEmpty");
    int drawInventoryRows() __asm__("cFEStateCharEquip_drawInventoryRows");
    int drawStoreRows() __asm__("cFEStateCharEquip_drawStoreRows");
    void redrawRows(int reset, int select) __asm__("cFEStateCharEquip_redrawRows");
    void drawInventoryRow(int i) __asm__("cFEStateCharEquip_drawInventoryRow");
    void drawStoreRow(int i) __asm__("cFEStateCharEquip_drawStoreRow");
    void buySelectedItem() __asm__("cFEStateCharEquip_buySelectedItem");
    bool toggleSelectedItem() __asm__("cFEStateCharEquip_toggleSelectedItem");
    void openBuyPopup(tBEBoltDef* item) __asm__("cFEStateCharEquip_openBuyPopup");
    virtual void onLoseFocus(cUIState*) __asm__("cFEStateCharEquip_onLoseFocus");
    virtual void onGainFocus(cUIState* pPrevState) __asm__("cFEStateCharEquip_onGainFocus");
    virtual int onWidgetMenu(cUIMenu* pMenu, unsigned int kind, unsigned int idx)
        __asm__("cFEStateCharEquip_onWidgetMenu");
};

CHECK_OFFSET(cFEStateCharEquip, mpTitleText, 0x048);
CHECK_OFFSET(cFEStateCharEquip, mpHelpText, 0x04C);
CHECK_OFFSET(cFEStateCharEquip, mpButtonHelpText, 0x050);
CHECK_OFFSET(cFEStateCharEquip, mpButtonHelpIcon, 0x054);
CHECK_OFFSET(cFEStateCharEquip, mpHeading, 0x058);
CHECK_OFFSET(cFEStateCharEquip, mpMenu, 0x05C);
CHECK_OFFSET(cFEStateCharEquip, mpMoneyGroup, 0x060);
CHECK_OFFSET(cFEStateCharEquip, mpBank, 0x064);
CHECK_OFFSET(cFEStateCharEquip, mpCost, 0x068);
CHECK_OFFSET(cFEStateCharEquip, mpPointsGroup, 0x06C);
CHECK_OFFSET(cFEStateCharEquip, mpPointsBar, 0x070);
CHECK_OFFSET(cFEStateCharEquip, mpPointsEquipBar, 0x074);
CHECK_OFFSET(cFEStateCharEquip, mpPointsBarValue, 0x078);
CHECK_OFFSET(cFEStateCharEquip, mpPointsCost, 0x07C);
CHECK_OFFSET(cFEStateCharEquip, mpSmallIconPreviewGroup, 0x080);
CHECK_OFFSET(cFEStateCharEquip, mpSmallBoltIcon, 0x084);
CHECK_OFFSET(cFEStateCharEquip, mpLargePreview, 0x088);
CHECK_OFFSET(cFEStateCharEquip, mpBoltNameWidgets, 0x08C);
CHECK_OFFSET(cFEStateCharEquip, mpBoltCheckWidgets, 0x0A4);
CHECK_OFFSET(cFEStateCharEquip, mCharID, 0x0BC);
CHECK_OFFSET(cFEStateCharEquip, mMoneyAvailable, 0x0C0);
CHECK_OFFSET(cFEStateCharEquip, mCurrentPolyCount, 0x0C4);
CHECK_OFFSET(cFEStateCharEquip, mEquipMode, 0x0C8);
CHECK_OFFSET(cFEStateCharEquip, mStoreNum, 0x0CC);
CHECK_OFFSET(cFEStateCharEquip, mCategoryStack, 0x0D0);
CHECK_OFFSET(cFEStateCharEquip, mCurrentStackPos, 0x120);
CHECK_OFFSET(cFEStateCharEquip, mpItems, 0x124);
CHECK_OFFSET(cFEStateCharEquip, mNumItems, 0x958);
CHECK_OFFSET(cFEStateCharEquip, mLargePreviewDelay, 0x95C);
CHECK_OFFSET(cFEStateCharEquip, mLargePreviewFileName, 0x960);
CHECK_OFFSET(cFEStateCharEquip, unk_A5F, 0xA5F);
CHECK_OFFSET(cFEStateCharEquip, mIconsLoaded, 0xA60);
CHECK_OFFSET(cFEStateCharEquip, mIconsFileIndex, 0xA64);
CHECK_OFFSET(cFEStateCharEquip, mIconTextureID, 0xA68);
CHECK_OFFSET(cFEStateCharEquip, mPreviewTextureFileIndex, 0xA6C);
CHECK_OFFSET(cFEStateCharEquip, mPreviewTextureID, 0xA70);
CHECK_OFFSET(cFEStateCharEquip, mOldPreviewTextureID, 0xA74);
CHECK_OFFSET(cFEStateCharEquip, mRowPoints, 0xA78);
CHECK_OFFSET(cFEStateCharEquip, mpGreenDollarSignElem, 0xA90);
CHECK_OFFSET(cFEStateCharEquip, mpRedDollarSignElem, 0xA94);
CHECK_OFFSET(cFEStateCharEquip, mpCheckMarkElem, 0xA98);
CHECK_OFFSET(cFEStateCharEquip, mpEmptyBoxElem, 0xA9C);
CHECK_OFFSET(cFEStateCharEquip, mpPlusElem, 0xAA0);
CHECK_OFFSET(cFEStateCharEquip, mZoom, 0xAA4);
CHECK_OFFSET(cFEStateCharEquip, mPan, 0xAA8);
CHECK_OFFSET(cFEStateCharEquip, mPlayerRotation, 0xAAC);
CHECK_OFFSET(cFEStateCharEquip, mBoardRotation, 0xAB0);
CHECK_OFFSET(cFEStateCharEquip, unk_AB4, 0xAB4);
CHECK_OFFSET(cFEStateCharEquip, mCharPos, 0xAC0);
CHECK_OFFSET(cFEStateCharEquip, mBoardPos, 0xAD0);
CHECK_OFFSET(cFEStateCharEquip, mCameraState, 0xAE0);
CHECK_OFFSET(cFEStateCharEquip, mCanSwitchMode, 0xAE4);
CHECK_OFFSET(cFEStateCharEquip, unk_AE8, 0xAE8);
CHECK_SIZE(cFEStateCharEquip, 0xAF0);
__asm__("_vt$17cFEStateCharEquip = cFEStateCharEquip_vtbl");   // g++'s name for the retail vtable

#endif /* FE_FESTATECHAREQUIP_H */
