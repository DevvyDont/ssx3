#ifndef FE_OVSTATEREWARDS_H
#define FE_OVSTATEREWARDS_H

// cOVState_REWARDS: the in-game rewards list overlay (screen "62reward_list", a scrollable "Menu0000" with
// up/down arrows). Retail code in the decomp's unit fe/ovstatepause.cpp, constructor/destructor in
// fe/all_in_one_fe.cpp; EA's source file is not known (the preview's linkonce sections name the class,
// __16cOVState_REWARDSP9cUIEngine12tOVStateEnum). Size 0xA0: cMemMan_alloc(0xA0, "cOVStateOverlay") for overlay
// id 16; On Tour 0x94 = 0x90 + one int. Vtable cOVState_REWARDS_vtbl (0x472108); constructor
// cOVState_REWARDS_cOVState_REWARDS, destructor cOVState_REWARDS__cOVState_REWARDS. Not cOVStateRewardsList
// (fe/ovstaterewardslist.cpp, vtable 0x4721E0), a different overlay. Base cOVState (fe/ovstate.h, 0x9C), written
// out flat below with the same member names.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstatepause.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_REWARDS {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVState_REWARDS_vtbl, stored after cOVState_vtbl by the constructor
    // --- cUIState (On Tour: size 0x48, same offsets; see ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U]
                                    //       cUIEngine_addScreenByHashName(mpEngine, ...)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] lw/sw the word; [R] triggerOverlay
                                    //       sets bits 0x3F00 to 0x100
    cUIState* mpParentState;        // 0x020 [O]; [R] 0 (1-argument cUIState ctor)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens; [R] constructed by cList_cList(self + 0x24)
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the "62reward_list"
                                    //       screen; widget lookups go through it
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (D_004A2088, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (16)
    // --- cOVState_REWARDS (On Tour: size 0x94)
    int mItemSize;                  // 0x09C [O]; [U] the number of reward rows: onCreateScreen zeroes it and
                                    //       checkRewardList(self, 0, -1) stores the count; onCreateScreen /
                                    //       updateMenu / onWidgetMenu compare it with the visible rows (one lbu
                                    //       passes its low byte to cUIMenu_setScrollableMenu)
};

CHECK_OFFSET(cOVState_REWARDS, mpPrev, 0x0);
CHECK_OFFSET(cOVState_REWARDS, mpNext, 0x4);
CHECK_OFFSET(cOVState_REWARDS, mVtbl, 0x8);
CHECK_OFFSET(cOVState_REWARDS, mHashName, 0xC);
CHECK_OFFSET(cOVState_REWARDS, mpEngine, 0x10);
CHECK_OFFSET(cOVState_REWARDS, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_REWARDS, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_REWARDS, unk_016, 0x16);
CHECK_OFFSET(cOVState_REWARDS, miMiscData, 0x18);
CHECK_OFFSET(cOVState_REWARDS, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_REWARDS, mpParentState, 0x20);
CHECK_OFFSET(cOVState_REWARDS, mScreens, 0x24);
CHECK_OFFSET(cOVState_REWARDS, mpScreen, 0x40);
CHECK_OFFSET(cOVState_REWARDS, mUserNum, 0x44);
CHECK_OFFSET(cOVState_REWARDS, unk_045, 0x45);
CHECK_OFFSET(cOVState_REWARDS, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_REWARDS, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_REWARDS, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_REWARDS, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_REWARDS, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_REWARDS, mOVID, 0x98);
CHECK_OFFSET(cOVState_REWARDS, mItemSize, 0x9C);
CHECK_SIZE(cOVState_REWARDS, 0xA0);

#endif /* FE_OVSTATEREWARDS_H */
