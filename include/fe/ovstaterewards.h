#ifndef FE_OVSTATEREWARDS_H
#define FE_OVSTATEREWARDS_H

// cOVState_REWARDS: the in-game rewards list overlay (screen "62reward_list", a scrollable "Menu0000" with
// up/down arrows). Retail code in the decomp's unit fe/ovstatepause.cpp, constructor/destructor in
// fe/all_in_one_fe.cpp; EA's source file is not known (the preview's linkonce sections name the class,
// __16cOVState_REWARDSP9cUIEngine12tOVStateEnum). Size 0xA0: cMemMan_alloc(0xA0, "cOVStateOverlay") for overlay
// id 16; On Tour 0x94 = 0x90 + one int. Vtable cOVState_REWARDS_vtbl (0x472108); constructor
// cOVState_REWARDS_cOVState_REWARDS, destructor cOVState_REWARDS__cOVState_REWARDS. Not cOVStateRewardsList
// (fe/ovstaterewardslist.cpp, vtable 0x4721E0), a different overlay. Derives from cOVState (fe/ovstate.h, 0x9C).
//
// Its own members are at their retail offsets. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstatepause.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_REWARDS : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_REWARDS_vtbl, stored after cOVState_vtbl by the constructor
    // cOVState_REWARDS's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the "62reward_list" screen; widget lookups go through it
    // cOVState (fe/ovstate.h, 0x9C)
    // --- cOVState_REWARDS (On Tour: size 0x94)
    int mItemSize;                  // 0x09C [O]; [U] the number of reward rows: onCreateScreen zeroes it and
                                    //       checkRewardList(self, 0, -1) stores the count; onCreateScreen /
                                    //       updateMenu / onWidgetMenu compare it with the visible rows (one lbu
                                    //       passes its low byte to cUIMenu_setScrollableMenu)

    // Methods bound to their retail symbols by asm labels.
    void updateMenu(int base) __asm__("cOVState_REWARDS_updateMenu");
    // retail's destructor cOVState_REWARDS__cOVState_REWARDS stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_REWARDS();
    virtual void onGainFocus(cUIState*) __asm__("cOVState_REWARDS_onGainFocus");
    virtual void onWidgetEvent(cUIThing* pWidget, unsigned int key) __asm__("cOVState_REWARDS_onWidgetEvent");
    virtual int onWidgetMenu(cUIMenu* id, unsigned int event, unsigned int index)
        __asm__("cOVState_REWARDS_onWidgetMenu");
    virtual bool onGainTransition(int) __asm__("cOVState_REWARDS_onGainTransition");
    virtual void onCreateScreen() __asm__("cOVState_REWARDS_onCreateScreen");
};

CHECK_OFFSET(cOVState_REWARDS, mItemSize, 0x9C);
CHECK_SIZE(cOVState_REWARDS, 0xA0);

#endif /* FE_OVSTATEREWARDS_H */
