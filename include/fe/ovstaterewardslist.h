#ifndef FE_OVSTATEREWARDSLIST_H
#define FE_OVSTATEREWARDSLIST_H

// cOVStateRewardsList: the in-game rewards list overlay ("rewardslist"): widget events 5 / 6 ask the custom
// interface (the engine's first word) for the next / previous state and push it.
// EA's source: d:/ssx/code/source/fe/ovstaterewardslist.cpp (preview assert strings). Retail code: onCreateScreen /
// onWidgetEvent in fe/ovstaterewardslist (0x00200218, 0x00200288); the ctors func_00222100 (engine 0) / cOVStateRewardsList_cOVStateRewardsList
// (pEngine, id) and the dtor in fe/all_in_one_fe (0x00222100 ... 0x002221C8).
//
// Size 0x9C: [O] On Tour's class is 0x90 = On Tour's cOVState (no members of its own); retail's cOVState is 0x9C
// (fe/ovstate.h) and the ctors write nothing past mOVID (0x98). Retail never calls the ctors (no allocation site).
// Base: cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8). Both ctors (engine 0 / (pEngine, id)) run
// cUIState_cUIState, the inlined cOVState ctor (mOVID, the three bools, cOVState_vtbl, mszChatBuff = " ") and store
// cOVStateRewardsList_vtbl (0x004721E0, 25 slots: 1 dtor, 4 onCreateScreen, 19 onWidgetEvent,
// 25 cOVState_allowPauseMenu); the dtor stores cOVState_vtbl and runs cUIState__cUIState.
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. The cOVState part is written out with
// fe/ovstate.h's names (see there for the inherited fields' full evidence).
// Evidence tags: [R] retail's ctors / dtor; [P] the July 2003 preview (fe/ovstate.h); [U] how retail code uses the
// field; [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVStateRewardsList : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVStateRewardsList_vtbl (ctors), cOVState_vtbl (dtor)
    // cOVStateRewardsList's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "rewardslist" through it; onWidgetEvent calls its custom interface (+0x0)
    //       slots 4 / 5 and pushes the result on its stack (+0x18)
    //   mpScreen: [U] onCreateScreen stores the screen (then cOVState_onCreateScreen)
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (On Tour: bool at 0x48); [R] the ctors zero it
    int mAllowPauseMenu;            // 0x04C [O][P] (fe/ovstate.h); [R] the ctors zero it
    int mRestoreCutSceneMode;       // 0x050 [O] (bool); [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O] (bool); [R] the ctors zero it
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " into it (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the (pEngine, id) ctor stores id, the default ctor 0
};

CHECK_OFFSET(cOVStateRewardsList, mDoneTransition, 0x048);
CHECK_OFFSET(cOVStateRewardsList, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cOVStateRewardsList, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cOVStateRewardsList, mRemovePDA, 0x054);
CHECK_OFFSET(cOVStateRewardsList, mszChatBuff, 0x058);
CHECK_OFFSET(cOVStateRewardsList, mOVID, 0x098);
CHECK_SIZE(cOVStateRewardsList, 0x9C);

#endif /* FE_OVSTATEREWARDSLIST_H */
