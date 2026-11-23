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
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstatepause.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_REWARDS : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_REWARDS_vtbl, stored after cOVState_vtbl by the constructor
    // cOVState_REWARDS's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the "62reward_list" screen; widget lookups go through it
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (str_sp_b, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (16)
    // --- cOVState_REWARDS (On Tour: size 0x94)
    int mItemSize;                  // 0x09C [O]; [U] the number of reward rows: onCreateScreen zeroes it and
                                    //       checkRewardList(self, 0, -1) stores the count; onCreateScreen /
                                    //       updateMenu / onWidgetMenu compare it with the visible rows (one lbu
                                    //       passes its low byte to cUIMenu_setScrollableMenu)
};

CHECK_OFFSET(cOVState_REWARDS, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_REWARDS, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_REWARDS, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_REWARDS, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_REWARDS, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_REWARDS, mOVID, 0x98);
CHECK_OFFSET(cOVState_REWARDS, mItemSize, 0x9C);
CHECK_SIZE(cOVState_REWARDS, 0xA0);

#endif /* FE_OVSTATEREWARDS_H */
