#ifndef FE_OVSTATESAVEOPTIONS_H
#define FE_OVSTATESAVEOPTIONS_H

// cOVState_SAVEOPTIONS: the in-game save-records (settings) memory-card overlay (screen "119Save_Replay" titled
// "title_Save_Records"; overlay id 19), a cFEMemCard with mType 2 (settings). Retail code: cOVState_SAVEOPTIONS_* in
// fe/ovstatepause (EA's d:/ssx/code/source/fe/ovstatepause.cpp; the class is probably declared in its ovstatepause.h,
// which the preview's asserts also name) and fe/all_in_one_fe (destructor cOVState_SAVEOPTIONS__cOVState_SAVEOPTIONS,
// the engine-less ctor copy cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS1).
//
// Size 0x274:
//   [R] cOVStateManager_triggerOverlay / triggerPopup: operator new(628, "cOVStateOverlay") for overlay 19, then
//       cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS(this, engine, 19), which runs func_001D5330(this, engine, 2,
//       eOVState) and clears 0x22C.
//   [O] On Tour 0x1A8 = cFEMemCard 0x164 + bool mIsHidden + unsigned short mTitle[32]; retail: 0x22C + a word + 33
//       chars (0x42) + 2 bytes of padding = 0x274.
// Base: cFEMemCard (fe/fememcard.h, 0x22C) : cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8).
//
// Derives from cFEMemCard (fe/fememcard.h); its own members at their retail offsets. fe/fememcard.h has the inherited
// fields' evidence; only this class's own uses are noted here.
// Evidence tags:
//   [R] retail's constructors and allocation sizes;
//   [P] the July 2003 preview's inline accessors (inherited fields only);
//   [U] how retail code uses the field (this class's methods, asm and C);
//   [O] SSX On Tour's DWARF (class cOVState_SAVEOPTIONS): member names, types and order. On Tour's bools are 1 byte
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

struct cOVState_SAVEOPTIONS : public cFEMemCard {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_SAVEOPTIONS_vtbl (0x004722B8, 47 slots), stored by cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS over
    //       cFEMemCard_vtbl
    // cOVState_SAVEOPTIONS's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds the screens; updateDone pops its state stack (+0x18)
    //   mpScreen: [U] onCreateScreen keeps the overlay screen here; every widget lookup
    //       (cUIScreen_getObjectByHashName)
    // cOVState (fe/ovstate.h, 0x9C)
    // cOVState_SAVEOPTIONS's notes on cOVState's fields:
    //   mOVID: [O] tOVStateEnum; [R] func_001D5330's eOVState (19 from cOVStateManager_triggerOverlay)
    // cFEMemCard (fe/fememcard.h, 0x22C)
    // cOVState_SAVEOPTIONS's notes on cFEMemCard's fields:
    //   mIsProcessing: [U] onWidgetEvent tests / sets it
    //   mIsDone: [U] updateDone tests and sets it
    //   mState: [U] onWidgetEvent / displayStatus switch on it
    //   mSlot: [U] onWidgetEvent stores the picked row
    //   mIsSavePressed: [U] onWidgetEvent sets it
    //   mIsCancelPressed: [U] onWidgetEvent sets it (back)
    //   mScreenStatus: [U] displayStatus tests 3 and moves there (virtual triggerDisplayState, slot 35)
    //   mDeleteButtonActive: [U] hideDeleteButton stores !hide
    //   mMainButtonActive: [U] hideMainButton stores !hide
    // --- cOVState_SAVEOPTIONS
    int mIsHidden;                  // 0x22C [O] (bool); [R] the ctor clears it; [U] hideActionButtons acts only when
                                    //       its argument differs, then stores it
    unsigned short mTitle[33];      // 0x230 [O] ([32] in On Tour; retail titles are kMCTitleStringSize = 33 chars,
                                    //       like cMCOverlayManager::mActiveTitleString[33]); [U] displayStatus has the
                                    //       card (mpMemCard slot 14, GetTitleString) write a slot's title here and
                                    //       shows it (cUIText_setUnicodeString)
    char unk_272[0x2];              // 0x272 padding to the 4-byte size

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cOVState_SAVEOPTIONS__cOVState_SAVEOPTIONS stays a C function (g++ 2.95 takes no asm label on
    // a destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cOVState_SAVEOPTIONS();
    virtual void updateDone() __asm__("cOVState_SAVEOPTIONS_updateDone");
    virtual void displayStatus() __asm__("cOVState_SAVEOPTIONS_displayStatus");
    virtual void onWidgetEvent(cUIThing* msg, unsigned int type) __asm__("cOVState_SAVEOPTIONS_onWidgetEvent");
};

CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsHidden, 0x22C);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mTitle, 0x230);
CHECK_OFFSET(cOVState_SAVEOPTIONS, unk_272, 0x272);
CHECK_SIZE(cOVState_SAVEOPTIONS, 0x274);

#endif /* FE_OVSTATESAVEOPTIONS_H */
