#ifndef FE_OVSTATEAUTOSAVE_H
#define FE_OVSTATEAUTOSAVE_H

// cOVState_AUTOSAVE: the in-game autosave overlay (screen "122Autosave": a pulsing "exclam" icon and the
// "kT_MEMAutosaving" text while the profile is written). It saves at once (onUpdate starts
// cMCOverlayManager_writeSetFileType in card state 1), then onDestroyScreen moves on to where the save was started from
// (mParentScreen).
// Retail code: cOVState_AUTOSAVE_* in fe/ovstateprofile (0x00212138 ... 0x00212508) and fe/all_in_one_fe (destructor
// 0x00222280, the empty onWidgetEvent / hideActionButtons overrides, the engine-less ctor copy func_002222B8). On Tour
// defines it in fe/all_in_one_fe.cpp (no EA path for it in the preview's asserts).
//
// Size 0x234:
//   [R] cOVStateManager_triggerOverlay / triggerPopup do operator new(564, "cOVStateOverlay") for overlay 37 and
//       cOVTemplate_Dialog_onWidgetEvent cMemMan_alloc(564, "Save Profile"); the ctor writes its own words 0x22C /
//       0x230.
//   [O] On Tour 0x16C = cFEMemCard 0x164 + int mParentScreen + bool mLastProcessState; retail: 0x22C + 2 words.
// Base: cFEMemCard (fe/fememcard.h, 0x22C) : cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8).
// The ctor cOVState_AUTOSAVE_cOVState_AUTOSAVE(this, pEngine, iParent) runs cFEMemCard_cFEMemCard(this, pEngine, 0).
//
// Derives from cFEMemCard (fe/fememcard.h); its own members at their retail offsets. fe/fememcard.h has the inherited
// fields' evidence; only this class's own uses are noted here.
// Evidence tags:
//   [R] retail's constructors and allocation sizes;
//   [P] the July 2003 preview's inline accessors (inherited fields only);
//   [U] how retail code uses the field (this class's methods, asm and C);
//   [O] SSX On Tour's DWARF (class cOVState_AUTOSAVE): member names, types and order. On Tour's bools are 1 byte
//       (retail: words), its cOVState is 0x90 (retail 0x9C) and its cFEMemCard 0x164 (retail 0x22C).

#include "layout.h"
#include "ui/uistate.h"
#include "fe/fememcard.h"
#include "fe/ovstate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cOVState_AUTOSAVE : public cFEMemCard {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_AUTOSAVE_vtbl (0x00471F80, 47 slots), stored by the ctors over cFEMemCard_vtbl
    // cOVState_AUTOSAVE's notes on cUIState's fields:
    //   mHashName: [R] cOVState_AUTOSAVE_cOVState_AUTOSAVE stores 37 (0x25, the overlay id)
    //   mpEngine: [U] onCreateScreen adds "122Autosave" to it; updateDone / onDestroyScreen use its state stack
    //       (+0x18)
    //   mpScreen: [U] onCreateScreen keeps the "122Autosave" screen here; displayStatus / displayOn look up
    //       "autosave_text" / "exclam"
    // cOVState (fe/ovstate.h, 0x9C)
    // cOVState_AUTOSAVE's notes on cOVState's fields:
    //   mOVID: [O] tOVStateEnum; [R] 0 (cFEMemCard_cFEMemCard)
    // cFEMemCard (fe/fememcard.h, 0x22C)
    // cOVState_AUTOSAVE's notes on cFEMemCard's fields:
    //   mIsProcessing: [U] onUpdate sets it when it starts the save and clears it when the card goes away; displayOn
    //       shows it
    //   mIsDone: [U] updateDone tests and sets it
    //   mState: [U] onUpdate: 1 -> start the save (3), 3 -> 6 when the card goes away; displayStatus / displayOn test
    //       0 / 6
    //   mIsSavePressed: [U] onUpdate sets it with the save and clears it on card loss
    //   mIsCancelPressed: [U] onUpdate sets it on card loss
    //   mIsOverwriteOK: [U] onUpdate sets it (no overwrite prompt)
    //   mScreenStatus: [U] the display state (cFEMemCard_triggerDisplayState stores it)
    // --- cOVState_AUTOSAVE
    int mParentScreen;              // 0x22C [O] (int); [R] the ctor's iParent (37 from cOVStateManager_triggerOverlay
                                    //       / triggerPopup, 1 from cOVTemplate_Dialog_onWidgetEvent); [U]
                                    //       onDestroyScreen switches on it: 1 cGame_exit, 6 push cFEStateTitle, 7 push
                                    //       cFEStateLodge, 0 set up a new race and cPreGameLoadScreen
    int mLastProcessState;          // 0x230 [O] (bool); [R] the ctor clears it; [U] displayOn compares mIsProcessing
                                    //       with it, returns when equal, else stores it and refreshes the icon / text

    // Methods bound to their retail symbols by asm labels.
    void displayOn() __asm__("cOVState_AUTOSAVE_displayOn");
};

CHECK_OFFSET(cOVState_AUTOSAVE, mParentScreen, 0x22C);
CHECK_OFFSET(cOVState_AUTOSAVE, mLastProcessState, 0x230);
CHECK_SIZE(cOVState_AUTOSAVE, 0x234);

#endif /* FE_OVSTATEAUTOSAVE_H */
