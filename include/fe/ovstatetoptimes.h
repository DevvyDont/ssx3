#ifndef FE_OVSTATETOPTIMES_H
#define FE_OVSTATETOPTIMES_H

// cOVState_TOPTIMES: the post-event top-times / top-scores overlay (screen "61toptimes"; Cross continues or saves).
// Retail code in the decomp's unit fe/ovstateenterlodge.cpp, constructor/destructor in fe/all_in_one_fe.cpp;
// EA's source file is not known (the preview's linkonce sections name the class: __17cOVState_TOPTIMESP9...).
// Size 0xA4: cMemMan_alloc(0xA4, "cOVStateOverlay") for overlay id 15; On Tour 0x98 = 0x90 + an int and a
// 1-byte bool, i.e. two words here. Vtable cOVState_TOPTIMES_vtbl (0x4734D0, 26 slots: slot 25 is the class's own
// virtual handleNext); constructor cOVState_TOPTIMES_cOVState_TOPTIMES, destructor
// cOVState_TOPTIMES__cOVState_TOPTIMES.
// Base cOVState (fe/ovstate.h, 0x9C), written out flat below with the same member names.
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

struct cOVState_TOPTIMES : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_TOPTIMES_vtbl, stored after cOVState_vtbl by the constructor; [U] onWidgetEvent calls slot 25
    //       (handleNext) through it
    // cOVState_TOPTIMES's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the "61toptimes" screen; widget lookups go through it
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (str_sp_b, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (15)
    // --- cOVState_TOPTIMES (On Tour: size 0x98)
    int mTopTimesFlow;              // 0x09C [O]; [R] placed by the allocation size (0xA4) and On Tour's order only:
                                    //       no retail code reads or writes it
    int mIsNextScreenPending;       // 0x0A0 [O] bool; [U] onCreateScreen clears it; handleNext acts only while it is
                                    //       clear and sets it (continue: cOVStateManager_mStatus = 1; save: overlay
                                    //       19)
};

CHECK_OFFSET(cOVState_TOPTIMES, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_TOPTIMES, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_TOPTIMES, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_TOPTIMES, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_TOPTIMES, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_TOPTIMES, mOVID, 0x98);
CHECK_OFFSET(cOVState_TOPTIMES, mTopTimesFlow, 0x9C);
CHECK_OFFSET(cOVState_TOPTIMES, mIsNextScreenPending, 0xA0);
CHECK_SIZE(cOVState_TOPTIMES, 0xA4);

#endif /* FE_OVSTATETOPTIMES_H */
