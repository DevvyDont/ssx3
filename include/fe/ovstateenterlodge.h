#ifndef FE_OVSTATEENTERLODGE_H
#define FE_OVSTATEENTERLODGE_H

// cOVState_ENTERLODGE: the "enter the lodge?" prompt (Yes leaves the race for the lodge, No / Triangle closes it).
// Retail code in the decomp's unit fe/ovstateenterlodge.cpp, constructor/destructor in fe/all_in_one_fe.cpp; EA's
// source file is not known (preview linkonce: __19cOVState_ENTERLODGEP9cUIEngine12tOVStateEnum). No members of
// its own: cMemMan_alloc(0x9C, "cOVStateOverlay") for overlay id 31; On Tour size 0x90 (= its cOVState). Vtable
// cOVState_ENTERLODGE_vtbl (0x473760); constructor cOVState_ENTERLODGE_cOVState_ENTERLODGE, destructor
// cOVState_ENTERLODGE__cOVState_ENTERLODGE. Base cOVState (fe/ovstate.h, 0x9C), written out flat below with the
// same member names.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstateenterlodge.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_ENTERLODGE : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_ENTERLODGE_vtbl, stored after cOVState_vtbl by the constructor
    // cOVState_ENTERLODGE's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the screen; onGainFocus looks widgets up through it
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen; onDestroyScreen clears it before
                                    //       cOVState_onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed; [U] onWidgetEvent sets it on No / Triangle
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (str_sp_b, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (31)
};

CHECK_OFFSET(cOVState_ENTERLODGE, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_ENTERLODGE, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_ENTERLODGE, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_ENTERLODGE, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_ENTERLODGE, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_ENTERLODGE, mOVID, 0x98);
CHECK_SIZE(cOVState_ENTERLODGE, 0x9C);

#endif /* FE_OVSTATEENTERLODGE_H */
