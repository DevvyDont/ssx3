#ifndef FE_FESTATECHAREQUIP_H
#define FE_FESTATECHAREQUIP_H

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
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet. bools are
// ints (ee-gcc 2.95's bool is 4 bytes; the C bodies' `*(bool*)` reads compile to lw).
// Evidence tags:
//   [R] retail's constructor cFEStateCharEquip_cFEStateCharEquip / destructor cFEStateCharEquip__cFEStateCharEquip;
//   [P] the July 2003 preview (layout_xlate: slot-4 onCreateScreen 0x3A34/0x3A38/0x3A3C -> 0xAA4/0xAA8/0xAAC);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cFEStateCharEquip, cUIState, cListNode): names, types and order.

#include "layout.h"
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

struct cFEStateCharEquip {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState_cUIState links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateCharEquip_vtbl (ctor; dtor first), cFEState_vtbl (dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [R] the ctor stores 0xE
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the engine argument (cUIState_cUIState); [U] the state stack
                                    //       (+0x18) and texture bank (+0x58) are reached through it
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the ctor stores cFECustom_getPlayerCtrlMask(engine, userNum)
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (word, see ui/uistate.h); [U] onPopupEvent clears / sets
                                    //       bit 3 (mVisible) while its buy popup is up (lw/sw)
    cUIState* mpParentState;        // 0x020 [O]
    cList mScreens;                 // 0x024 [O] cList mScreens (util/list.h; ui/uistate.h); [U] onUpdate: cList_first(&mScreens)
    cUIScreen* mpScreen;            // 0x040 [O]
    signed char mUserNum;           // 0x044 [O]; [R] the ctor's userNum argument (sb); [U] passed to the BE player /
                                    //       economy / shop interfaces and cFERiderManager everywhere
    char unk_045[0x3];              // 0x045 padding
    // --- cFEState (On Tour: size 0x48, no data)
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
    int mCurrentPolyCount;          // 0x0C4 [O]; [R] func_0014B478(shop, mUserNum, mCharID) (the equip points)
    int mEquipMode;                 // 0x0C8 [O]; [R] 1 = store (inStore), 0 = inventory; [U] isInStore tests == 1,
                                    //       switchEquipMode flips it
    int mStoreNum;                  // 0x0CC [O]; [R] the last hub's peak (1/2/3) with a store or mode switch, else -1;
                                    //       [U] the store lists only items of this store number (func_0019B180)
    int mCategoryStack[20];         // 0x0D0 [U] (no On Tour member) MEM_fill(.., D_004A11D0, 80) in the ctor and
                                    //       switchEquipMode; onWidgetEvent pushes the opened category id at
                                    //       [mCurrentStackPos], back pops it
    int mCurrentStackPos;           // 0x120 [O] (On Tour's member after mStoreNum); [R] 0; [U] the depth into
                                    //       mCategoryStack (onWidgetMenu event 7 steps it down)
    void* mpItems[0x20D];           // 0x124 [U] (no On Tour member) the listed items (BE library entries: +0x30
                                    //       name, +0x34 flags) built by func_0019B180 (MEM_fill(mpItems, 0, 2100))
    int mNumItems;                  // 0x958 [R] 0; [U] the count of mpItems (func_0019B180 clears and appends,
                                    //       func_0019B458 removes one); func_0019B618 passes its low byte (lbu) to
                                    //       cUIMenu_setScrollableMenu
    int mLargePreviewDelay;         // 0x95C [O]; [R] -1; [U] startLargePreviewLoad sets 20 or 1, updatePreviewLoad
                                    //       counts it down, cancelLargePreviewLoad sets -1
    char mLargePreviewFileName[255]; // 0x960 [O]; [R] [0] = 0; [U] startLargePreviewLoad builds the file name here
    char unk_A5F[0x1];              // 0xA5F padding
    int mIconsLoaded;               // 0xA60 [O] (bool); [R] 0; [U] the out-of-line isIconsLoaded func_001DF9C0 (lw);
                                    //       onUpdate sets it once the icon file is in (func_0019E238)
    int mIconsFileIndex;            // 0xA64 [O]; [R] -1, then func_0019DA10(RealFE + 0x1A70, mCharID, name, 9, 1, 0);
                                    //       [R] the dtor releases it (func_0019DC20) and sets -1
    int mIconTextureID;             // 0xA68 [O]; [R] -1; [U] the dtor releases it (graphics manager slot 50)
    int mPreviewTextureFileIndex;   // 0xA6C [O]; [R] -1; [U] updatePreviewLoad queues the large preview file here
    int mPreviewTextureID;          // 0xA70 [O]; [R] -1; the dtor releases it
    int mOldPreviewTextureID;       // 0xA74 [O]; [R] -1; the dtor releases it
    int mRowPoints[6];              // 0xA78 [U] (no On Tour member) [R] -99999 x6; func_0019B7E0 stores each inventory
                                    //       row's change in equip points, func_0019AA08 / onWidgetMenu read it
    cTextureElement* mpGreenDollarSignElem; // 0xA90 [O]; [R] the texture bank's "dollargreen" element
    cTextureElement* mpRedDollarSignElem;   // 0xA94 [O]; [R] "dollarred"
    cTextureElement* mpCheckMarkElem;       // 0xA98 [O]; [R] "checkbox"
    cTextureElement* mpEmptyBoxElem;        // 0xA9C [O]; [R] "empty_box"
    cTextureElement* mpPlusElem;            // 0xAA0 [O]; [R] 0; [U] func_0019BA60 shows it for a store category
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
                                    //       setCanSwitchEquipMode func_001DF9C8 (sw); onWidgetCreate hides the button
                                    //       when 0
    char unk_AE8[0x8];              // 0xAE8 tail padding (16-byte class alignment)
};

CHECK_OFFSET(cFEStateCharEquip, mpPrev, 0x000);
CHECK_OFFSET(cFEStateCharEquip, mpNext, 0x004);
CHECK_OFFSET(cFEStateCharEquip, mVtbl, 0x008);
CHECK_OFFSET(cFEStateCharEquip, mHashName, 0x00C);
CHECK_OFFSET(cFEStateCharEquip, mpEngine, 0x010);
CHECK_OFFSET(cFEStateCharEquip, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateCharEquip, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateCharEquip, unk_016, 0x016);
CHECK_OFFSET(cFEStateCharEquip, miMiscData, 0x018);
CHECK_OFFSET(cFEStateCharEquip, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateCharEquip, mpParentState, 0x020);
CHECK_OFFSET(cFEStateCharEquip, mScreens, 0x024);
CHECK_OFFSET(cFEStateCharEquip, mpScreen, 0x040);
CHECK_OFFSET(cFEStateCharEquip, mUserNum, 0x044);
CHECK_OFFSET(cFEStateCharEquip, unk_045, 0x045);
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

#endif /* FE_FESTATECHAREQUIP_H */
