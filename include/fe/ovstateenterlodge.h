#ifndef FE_OVSTATEENTERLODGE_H
#define FE_OVSTATEENTERLODGE_H

// cOVState_ENTERLODGE: the "enter the lodge?" prompt (Yes leaves the race for the lodge, No / Triangle closes it).
// Retail code in the decomp's unit fe/ovstateenterlodge.cpp, constructor/destructor in fe/all_in_one_fe.cpp; EA's
// source file is not known (preview linkonce: __19cOVState_ENTERLODGEP9cUIEngine12tOVStateEnum). No members of
// its own: cMemMan_alloc(0x9C, "cOVStateOverlay") for overlay id 31; On Tour size 0x90 (= its cOVState). Vtable
// cOVState_ENTERLODGE_vtbl (0x473760); constructor cOVState_ENTERLODGE_cOVState_ENTERLODGE, destructor
// cOVState_ENTERLODGE__cOVState_ENTERLODGE. Derives from cOVState (fe/ovstate.h, 0x9C).
//
// Its own members are at their retail offsets. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstateenterlodge.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"
#include "ui/uistate.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_ENTERLODGE : public cOVState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_ENTERLODGE_vtbl, stored after cOVState_vtbl by the constructor
    // cOVState_ENTERLODGE's notes on cUIState's fields:
    //   mpEngine: [U] cUIEngine_addScreenByHashName(mpEngine, ...)
    //   mpScreen: [U] onCreateScreen stores the screen; onGainFocus looks widgets up through it
    // cOVState (fe/ovstate.h, 0x9C)
    // cOVState_ENTERLODGE's notes on cOVState's fields:
    //   mRestoreCutSceneMode: [U] cOVState_onCreateScreen; onDestroyScreen clears it before cOVState_onDestroyScreen
    //   mRemovePDA: [U] onWidgetEvent sets it on No / Triangle

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cOVState_ENTERLODGE__cOVState_ENTERLODGE stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_ENTERLODGE();
    virtual void onCreateScreen() __asm__("cOVState_ENTERLODGE_onCreateScreen");
    virtual void onGainFocus(cUIState* pPrevState) __asm__("cOVState_ENTERLODGE_onGainFocus");
};

CHECK_SIZE(cOVState_ENTERLODGE, 0x9C);

#endif /* FE_OVSTATEENTERLODGE_H */
