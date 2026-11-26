#ifndef FE_FESTATEMEMCARD_H
#define FE_FESTATEMEMCARD_H

// cFEStateMemCard: the front end's start-up memory-card screen ("06cAutoload"): it checks the cards for the settings
// and profile files, checks the free space for missing ones and auto-loads them (a cFEMemCard with mType 2, settings).
// The live screen is the class static cFEStateMemCard::mInstance (cFEStateMemCard_mInstance: the ctor sets it, the dtor clears it);
// the manager callbacks it installs (cFEStateMemCard_callback*, func_00186C98, func_00186D60, startProfileRead,
// startSettingsRead, displayStatus) are static and work through it; the start-up stage is the static cFEStateMemCard_mStage
// (On Tour mStage), cFEStateMemCard_mProfileFile is cleared next to it (On Tour mProfileFile, by order).
// EA's source file is not known: the preview keeps the inline ctor / dtor / onWidgetEvent as linkonce copies
// (__15cFEStateMemCard, _$_15cFEStateMemCard, onWidgetEvent__15cFEStateMemCardP8cUIThingUi), so the class is declared
// in a header; this one is named after the class. Retail code: the ctor func_00186A08, the dtor and onWidgetMenu in
// fe/festateload (0x00186A08 ... 0x00186B48), the methods in fe/fememcard (0x00186B50 ... 0x00187C10), the engine-less
// ctor copy and the two empty slots in main/all_in_one_main (0x001DE940 ... 0x001DE950).
//
// Size 0x248:
//   [R] cMemMan_alloc(584, "cFEStateMemCard") + func_00186A08(mem, engine) in fe/festateloadhint and operator new(584,
//       "cFEStateMemCard") in main/loadscreens_prestart; the ctor writes every
//       own word 0x22C ... 0x244.
//   [O] On Tour 0x178 = cFEMemCard 0x164 + int, int, bool, int, bool, bool, bool; with retail's 4-byte bools the same
//       seven members fill 0x22C ... 0x248 exactly, in On Tour's order.
// Base: cFEMemCard (fe/fememcard.h, 0x22C) : cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8).
// func_00186A08 runs cFEMemCard_cFEMemCard2(this, pEngine, 2) and stores cFEStateMemCard_vtbl (0x0046B570: 1 dtor,
// 4 onCreateScreen, 8 onInputBegin__FPv, 12 onUpdate, 17 onPopupEvent, 19 onWidgetEvent, 20 onWidgetMenu__FPv,
// 26 displayStatus, 33 updateDone, 37 cFEStateMemCard_hideActionButtons (hideActionButtons, empty); the rest cFEMemCard's / cOVState's /
// cUIState's); the dtor stores it, clears mInstance and runs cFEMemCard__cFEMemCard.
//
// Derives from cFEMemCard (fe/fememcard.h); its own members at their retail offsets. fe/fememcard.h has the inherited
// fields' evidence; only this class's own uses are noted here.
// Evidence tags:
//   [R] retail's constructor func_00186A08 / destructor and allocation size;
//   [U] how retail code uses the field (this class's methods and static callbacks, asm and C);
//   [O] SSX On Tour's DWARF (class cFEStateMemCard / cFEMemCard): member names, types and order. On Tour's bools are
//       1 byte (retail: words), its cOVState is 0x90 (retail 0x9C) and its cFEMemCard 0x164 (retail 0x22C).
// (No preview evidence: layout_xlate finds no paired cFEStateMemCard methods.)

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

struct cFEStateMemCard : public cFEMemCard {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateMemCard_vtbl, stored over cFEMemCard_vtbl by both ctors; the dtor stores it first; [U] the
    //       callbacks call getPopup (slot 36) / cancel (41)
    // cFEStateMemCard's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "06cAutoload"; startProfileRead / startSettingsRead
    //   mpScreen: [U] onCreateScreen stores it; onUpdate / displayStatus / startProfileRead look "mc messages" and "!"
    //       up on it
    // cOVState (fe/ovstate.h, 0x9C)
    // cFEStateMemCard's notes on cOVState's fields:
    //   mOVID: [O] tOVStateEnum; [R] 0 (cFEMemCard_cFEMemCard2)
    // cFEMemCard (fe/fememcard.h, 0x22C)
    // cFEStateMemCard's notes on cFEMemCard's fields:
    //   mLastLoadFileName: [U] callbackLoadReady has the card write the title of the file at the cursor here
    //   mFileName: [U] startProfileRead passes it to the manager
    //   mPopupType: [U] the callbacks set 4 / 9 / 20 / 21 before getPopup; onPopupEvent switches on it
    //   mIsProcessing: [U] startProfileRead / startSettingsRead set it
    //   mIsDone: [U] updateDone / exitScreen test and set it
    //   mType: [U] startProfileRead sets 0, startSettingsRead 2; updateFreeSpaceCheckState and callbackCardsChecked
    //       read it
    //   mState: [U] the reads set 5 (loading); callbackFileNotFound / exitScreen set 6 (done); onUpdate tests 6
    //   mSlot: [U] startProfileRead / startSettingsRead store the file slot
    //   mIsRetryPressed: [U] onPopupEvent sets it (popup 20, cancel)
    //   mStatusPopup: [U] func_00186D60 / callbackConfirmNoFreeBlocks label its buttons (slots 25 / 26)
    // --- cFEStateMemCard
    int mMissingSavedFileTypeFlags; // 0x22C [O]; [R] -1; [U] callbackCardsChecked / callbackSysRecheckFile store the
                                    //       card's missing-file bits, callbackLoadReady tests bit 0 (profile),
                                    //       updateFreeSpaceCheckState reads bits 0-2 (a bitfield view there)
    int mTotalSpaceRequired;        // 0x230 [O]; [R] 0; [U] callbackLoadReady totals the card space the missing files need
    int mFreespaceCheckDone;        // 0x234 [O] (bool); [R] 0; [U] callbackLoadReady sets it when the card is too full;
                                    //       terminateFreeSpaceCheck / updateFreeSpaceCheckState / the check callbacks
    int mCard1MissingSavedFileTypeFlags; // 0x238 [O]; [R] -1; [U] updateFreeSpaceCheckState saves the flags here;
                                    //       onPopupEvent (popup 21): bit 1 settings, bit 0 profile; updateDone,
                                    //       terminateFreeSpaceCheck
    int mCard1Available;            // 0x23C [O] (bool); [R] 1 (no other retail access)
    int mCard1Full;                 // 0x240 [O] (bool); [R] 0 (no other retail access)
    int mCard1Error;                // 0x244 [O] (bool); [R] 0 (no other retail access)

    // Methods bound to their retail symbols by asm labels.
    void terminateFreeSpaceCheck() __asm__("cFEStateMemCard_terminateFreeSpaceCheck");
    void exitScreen() __asm__("cFEStateMemCard_exitScreen");
    void updateFreeSpaceCheckState() __asm__("cFEStateMemCard_updateFreeSpaceCheckState");
    // retail's destructor cFEStateMemCard__cFEStateMemCard stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFEStateMemCard();
    virtual void onCreateScreen() __asm__("cFEStateMemCard_onCreateScreen");
    virtual void onUpdate() __asm__("cFEStateMemCard_onUpdate");
    virtual void updateDone() __asm__("cFEStateMemCard_updateDone");
};

CHECK_OFFSET(cFEStateMemCard, mMissingSavedFileTypeFlags, 0x22C);
CHECK_OFFSET(cFEStateMemCard, mTotalSpaceRequired, 0x230);
CHECK_OFFSET(cFEStateMemCard, mFreespaceCheckDone, 0x234);
CHECK_OFFSET(cFEStateMemCard, mCard1MissingSavedFileTypeFlags, 0x238);
CHECK_OFFSET(cFEStateMemCard, mCard1Available, 0x23C);
CHECK_OFFSET(cFEStateMemCard, mCard1Full, 0x240);
CHECK_OFFSET(cFEStateMemCard, mCard1Error, 0x244);
CHECK_SIZE(cFEStateMemCard, 0x248);

#endif /* FE_FESTATEMEMCARD_H */
