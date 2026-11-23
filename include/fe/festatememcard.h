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
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. The base classes cOVState / cFEMemCard
// are written out inline with fe/fememcard.h's member names (see that header for the full evidence of the inherited
// fields; only this class's own uses are added here).
// Evidence tags:
//   [R] retail's constructor func_00186A08 / destructor and allocation size;
//   [U] how retail code uses the field (this class's methods and static callbacks, asm and C);
//   [O] SSX On Tour's DWARF (class cFEStateMemCard / cFEMemCard): member names, types and order. On Tour's bools are
//       1 byte (retail: words), its cOVState is 0x90 (retail 0x9C) and its cFEMemCard 0x164 (retail 0x22C).
// (No preview evidence: layout_xlate finds no paired cFEStateMemCard methods.)

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cFEStateMemCard : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateMemCard_vtbl, stored over cFEMemCard_vtbl by both ctors; the dtor stores it first; [U] the
    //       callbacks call getPopup (slot 36) / cancel (41)
    // cFEStateMemCard's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "06cAutoload"; startProfileRead / startSettingsRead
    //   mpScreen: [U] onCreateScreen stores it; onUpdate / displayStatus / startProfileRead look "mc messages" and "!"
    //       up on it
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (bool); [R] the (inlined) cOVState ctor zeroes it
    int mAllowPauseMenu;            // 0x04C [O] (bool); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O] (bool)
    int mRemovePDA;                 // 0x054 [O] (bool); [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " into it (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] 0 (cFEMemCard_cFEMemCard2)
    // --- cFEMemCard (fe/fememcard.h, 0x22C)
    unsigned short mLastLoadFileName[64]; // 0x09C [O] ([32] in On Tour); [U] callbackLoadReady has the card write the
                                    //       title of the file at the cursor here
    unsigned short mFileName[64];   // 0x11C [O] ([32] in On Tour); [U] startProfileRead passes it to the manager
    int mPopupType;                 // 0x19C [O]; [R] func_00186A08 sets 26; [U] the callbacks set 4 / 9 / 20 / 21 before
                                    //       getPopup; onPopupEvent switches on it
    int mProcessingKeyboard;        // 0x1A0 [O] (bool)
    cKeyboardPopup* mpKeyboard;     // 0x1A4 [O]
    int mIsProcessing;              // 0x1A8 [O] (bool); [U] startProfileRead / startSettingsRead set it
    int mIsDone;                    // 0x1AC [O] (bool); [U] updateDone / exitScreen test and set it
    int mIsLoadWait;                // 0x1B0 [O] (bool)
    int mIsLoadDoneOK;              // 0x1B4 [O] (bool)
    int mIsInvalidConfirmed;        // 0x1B8 [O] (bool)
    int mType;                      // 0x1BC [O]; [R] 2 (settings: cFEMemCard_cFEMemCard2(self, pEngine, 2)); [U]
                                    //       startProfileRead sets 0, startSettingsRead 2; updateFreeSpaceCheckState and
                                    //       callbackCardsChecked read it
    int mState;                     // 0x1C0 [O]; [U] the reads set 5 (loading); callbackFileNotFound / exitScreen set 6
                                    //       (done); onUpdate tests 6
    int mSlot;                      // 0x1C4 [O]; [U] startProfileRead / startSettingsRead store the file slot
    int mIsReplayAlloc;             // 0x1C8 [O] (bool)
    int mPlayerIdx;                 // 0x1CC [O]
    char* mpReadBuffer;             // 0x1D0 [O]
    char* mpStartReadBuffer;        // 0x1D4 [O]
    int mIsManagerStarted;          // 0x1D8 [O] (bool)
    int mIsSavePressed;             // 0x1DC [O] (bool)
    int mIsDeletePressed;           // 0x1E0 [O] (bool)
    int mIsCancelPressed;           // 0x1E4 [O] (bool)
    int mIsFormatPressed;           // 0x1E8 [O] (bool)
    int mIsRetryPressed;            // 0x1EC [O] (bool); [U] onPopupEvent sets it (popup 20, cancel)
    int mIsOverwriteOK;             // 0x1F0 [O] (bool)
    int mIsLoadPressed;             // 0x1F4 [O] (bool)
    int mDoNextMemCard;             // 0x1F8 [O] (bool)
    int mDoPrevMemCard;             // 0x1FC [O] (bool)
    int mLoadBEData;                // 0x200 [O] (bool)
    int mAutoLoadOn;                // 0x204 [O] (bool); [R] func_00186A08 sets 1
    int mAutoSaveOn;                // 0x208 [O] (bool)
    int mFirstCheck;                // 0x20C [O] (bool)
    int mIsStatusPopupOn;           // 0x210 [O] (bool)
    int mScreenStatus;              // 0x214 [O]
    cFEPopupConfirm* mStatusPopup;  // 0x218 [O]; [U] func_00186D60 / callbackConfirmNoFreeBlocks label its buttons
                                    //       (slots 25 / 26)
    int mLastSelectedDevice;        // 0x21C [O]
    int mDeleteButtonActive;        // 0x220 [O] (bool)
    int mMainButtonActive;          // 0x224 [O] (bool)
    int mBufferState;               // 0x228 [O] eBufferState
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
};

CHECK_OFFSET(cFEStateMemCard, mDoneTransition, 0x048);
CHECK_OFFSET(cFEStateMemCard, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cFEStateMemCard, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cFEStateMemCard, mRemovePDA, 0x054);
CHECK_OFFSET(cFEStateMemCard, mszChatBuff, 0x058);
CHECK_OFFSET(cFEStateMemCard, mOVID, 0x098);
CHECK_OFFSET(cFEStateMemCard, mLastLoadFileName, 0x09C);
CHECK_OFFSET(cFEStateMemCard, mFileName, 0x11C);
CHECK_OFFSET(cFEStateMemCard, mPopupType, 0x19C);
CHECK_OFFSET(cFEStateMemCard, mProcessingKeyboard, 0x1A0);
CHECK_OFFSET(cFEStateMemCard, mpKeyboard, 0x1A4);
CHECK_OFFSET(cFEStateMemCard, mIsProcessing, 0x1A8);
CHECK_OFFSET(cFEStateMemCard, mIsDone, 0x1AC);
CHECK_OFFSET(cFEStateMemCard, mIsLoadWait, 0x1B0);
CHECK_OFFSET(cFEStateMemCard, mIsLoadDoneOK, 0x1B4);
CHECK_OFFSET(cFEStateMemCard, mIsInvalidConfirmed, 0x1B8);
CHECK_OFFSET(cFEStateMemCard, mType, 0x1BC);
CHECK_OFFSET(cFEStateMemCard, mState, 0x1C0);
CHECK_OFFSET(cFEStateMemCard, mSlot, 0x1C4);
CHECK_OFFSET(cFEStateMemCard, mIsReplayAlloc, 0x1C8);
CHECK_OFFSET(cFEStateMemCard, mPlayerIdx, 0x1CC);
CHECK_OFFSET(cFEStateMemCard, mpReadBuffer, 0x1D0);
CHECK_OFFSET(cFEStateMemCard, mpStartReadBuffer, 0x1D4);
CHECK_OFFSET(cFEStateMemCard, mIsManagerStarted, 0x1D8);
CHECK_OFFSET(cFEStateMemCard, mIsSavePressed, 0x1DC);
CHECK_OFFSET(cFEStateMemCard, mIsDeletePressed, 0x1E0);
CHECK_OFFSET(cFEStateMemCard, mIsCancelPressed, 0x1E4);
CHECK_OFFSET(cFEStateMemCard, mIsFormatPressed, 0x1E8);
CHECK_OFFSET(cFEStateMemCard, mIsRetryPressed, 0x1EC);
CHECK_OFFSET(cFEStateMemCard, mIsOverwriteOK, 0x1F0);
CHECK_OFFSET(cFEStateMemCard, mIsLoadPressed, 0x1F4);
CHECK_OFFSET(cFEStateMemCard, mDoNextMemCard, 0x1F8);
CHECK_OFFSET(cFEStateMemCard, mDoPrevMemCard, 0x1FC);
CHECK_OFFSET(cFEStateMemCard, mLoadBEData, 0x200);
CHECK_OFFSET(cFEStateMemCard, mAutoLoadOn, 0x204);
CHECK_OFFSET(cFEStateMemCard, mAutoSaveOn, 0x208);
CHECK_OFFSET(cFEStateMemCard, mFirstCheck, 0x20C);
CHECK_OFFSET(cFEStateMemCard, mIsStatusPopupOn, 0x210);
CHECK_OFFSET(cFEStateMemCard, mScreenStatus, 0x214);
CHECK_OFFSET(cFEStateMemCard, mStatusPopup, 0x218);
CHECK_OFFSET(cFEStateMemCard, mLastSelectedDevice, 0x21C);
CHECK_OFFSET(cFEStateMemCard, mDeleteButtonActive, 0x220);
CHECK_OFFSET(cFEStateMemCard, mMainButtonActive, 0x224);
CHECK_OFFSET(cFEStateMemCard, mBufferState, 0x228);
CHECK_OFFSET(cFEStateMemCard, mMissingSavedFileTypeFlags, 0x22C);
CHECK_OFFSET(cFEStateMemCard, mTotalSpaceRequired, 0x230);
CHECK_OFFSET(cFEStateMemCard, mFreespaceCheckDone, 0x234);
CHECK_OFFSET(cFEStateMemCard, mCard1MissingSavedFileTypeFlags, 0x238);
CHECK_OFFSET(cFEStateMemCard, mCard1Available, 0x23C);
CHECK_OFFSET(cFEStateMemCard, mCard1Full, 0x240);
CHECK_OFFSET(cFEStateMemCard, mCard1Error, 0x244);
CHECK_SIZE(cFEStateMemCard, 0x248);

#endif /* FE_FESTATEMEMCARD_H */
