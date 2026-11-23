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
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. The base classes cOVState / cFEMemCard
// are written out inline with fe/fememcard.h's member names (see that header for the full evidence of the inherited
// fields; only this class's own uses are added here).
// Evidence tags:
//   [R] retail's constructors and allocation sizes;
//   [P] the July 2003 preview's inline accessors (inherited fields only);
//   [U] how retail code uses the field (this class's methods, asm and C);
//   [O] SSX On Tour's DWARF (class cOVState_AUTOSAVE): member names, types and order. On Tour's bools are 1 byte
//       (retail: words), its cOVState is 0x90 (retail 0x9C) and its cFEMemCard 0x164 (retail 0x22C).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cOVState_AUTOSAVE : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_AUTOSAVE_vtbl (0x00471F80, 47 slots), stored by the ctors over cFEMemCard_vtbl
    // cOVState_AUTOSAVE's notes on cUIState's fields:
    //   mHashName: [R] cOVState_AUTOSAVE_cOVState_AUTOSAVE stores 37 (0x25, the overlay id)
    //   mpEngine: [U] onCreateScreen adds "122Autosave" to it; updateDone / onDestroyScreen use its state stack
    //       (+0x18)
    //   mpScreen: [U] onCreateScreen keeps the "122Autosave" screen here; displayStatus / displayOn look up
    //       "autosave_text" / "exclam"
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (bool); [R] the (inlined) cOVState ctor zeroes it
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O] (bool); [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O] (bool); [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " (str_sp_b) into it
                                    //       (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] 0 (cFEMemCard_cFEMemCard)
    // --- cFEMemCard (fe/fememcard.h, 0x22C)
    unsigned short mLastLoadFileName[64]; // 0x09C [O] ([32] in On Tour); [R] initFEMemCard clears the first char
    unsigned short mFileName[64];   // 0x11C [O] ([32] in On Tour); [R] initFEMemCard writes the "%s-%s*" file pattern
    int mPopupType;                 // 0x19C [O]; [R] initFEMemCard sets 25; [U] the popup id (cFEMemCard_onPopupEvent)
    int mProcessingKeyboard;        // 0x1A0 [O] (bool)
    cKeyboardPopup* mpKeyboard;     // 0x1A4 [O]; [U] cFEMemCard_onUpdate skips the base update while it is set
    int mIsProcessing;              // 0x1A8 [O] (bool); [U] onUpdate sets it when it starts the save and clears it
                                    //       when the card goes away; displayOn shows it
    int mIsDone;                    // 0x1AC [O] (bool); [U] updateDone tests and sets it
    int mIsLoadWait;                // 0x1B0 [O] (bool); [U] cFEMemCard_readStart / onUpdate
    int mIsLoadDoneOK;              // 0x1B4 [O] (bool); [U] cFEMemCard_readStart / callbackReadSuccess
    int mIsInvalidConfirmed;        // 0x1B8 [O] (bool); [R] initFEMemCard clears it
    int mType;                      // 0x1BC [O]; [R] 0 (profile): the ctor runs cFEMemCard_cFEMemCard(self, pEngine,
                                    //       0)
    int mState;                     // 0x1C0 [O]; [U] onUpdate: 1 -> start the save (3), 3 -> 6 when the card goes
                                    //       away; displayStatus / displayOn test 0 / 6
    int mSlot;                      // 0x1C4 [O]; [R] the ctor clears it
    int mIsReplayAlloc;             // 0x1C8 [O] (bool); [U] cFEMemCard_writeStart sets it for a replay save
    int mPlayerIdx;                 // 0x1CC [O]; [U] the profile slot (cFEMemCard_writeStart / readDone)
    char* mpReadBuffer;             // 0x1D0 [O]; [U] cFEMemCard_createReadBuffer / callbackReadUpdate
    char* mpStartReadBuffer;        // 0x1D4 [O]; [U] cFEMemCard_createReadBuffer / readDone / onDestroyScreen
    int mIsManagerStarted;          // 0x1D8 [O] (bool); [U] cFEMemCard_onUpdate
    int mIsSavePressed;             // 0x1DC [O] (bool); [U] onUpdate sets it with the save and clears it on card loss
    int mIsDeletePressed;           // 0x1E0 [O] (bool); [U] returned by cFEMemCard_isDeletePressed
    int mIsCancelPressed;           // 0x1E4 [O] (bool); [U] onUpdate sets it on card loss
    int mIsFormatPressed;           // 0x1E8 [O] (bool); [U] returned by cFEMemCard_isFormatPressed
    int mIsRetryPressed;            // 0x1EC [O] (bool); [U] cFEMemCard_callbackIsRetryPressed
    int mIsOverwriteOK;             // 0x1F0 [O] (bool); [U] onUpdate sets it (no overwrite prompt)
    int mIsLoadPressed;             // 0x1F4 [O] (bool); [U] returned by cFEMemCard_isLoadPressed
    int mDoNextMemCard;             // 0x1F8 [O] (bool); [U] cFEMemCard_isNextMCPressed / onInputBegin
    int mDoPrevMemCard;             // 0x1FC [O] (bool); [U] cFEMemCard_isPrevMCPressed / onInputBegin
    int mLoadBEData;                // 0x200 [O] (bool); [U] cFEMemCard_readStart / callbackReadUpdate
    int mAutoLoadOn;                // 0x204 [O] (bool); [U] cFEMemCard_callbackReadSuccess / onPopupEvent
    int mAutoSaveOn;                // 0x208 [O] (bool); [R] the ctor sets it to 1 (cFEMemCard_callbackSaveSuccess then
                                    //       exits without a popup)
    int mFirstCheck;                // 0x20C [O] (bool); [R] initFEMemCard clears it
    int mIsStatusPopupOn;           // 0x210 [O] (bool); [U] cFEMemCard_displayStatusPopup / getPopup
    int mScreenStatus;              // 0x214 [O]; [U] the display state (cFEMemCard_triggerDisplayState stores it)
    cFEPopupConfirm* mStatusPopup;  // 0x218 [O]; [U] cFEMemCard_getPopup allocates it
    int mLastSelectedDevice;        // 0x21C [O]; [R] initFEMemCard clears it
    int mDeleteButtonActive;        // 0x220 [O] (bool)
    int mMainButtonActive;          // 0x224 [O] (bool)
    int mBufferState;               // 0x228 [O] eBufferState; [U] cFEMemCard_createReadBuffer / readStart / readDone
    // --- cOVState_AUTOSAVE
    int mParentScreen;              // 0x22C [O] (int); [R] the ctor's iParent (37 from cOVStateManager_triggerOverlay
                                    //       / triggerPopup, 1 from cOVTemplate_Dialog_onWidgetEvent); [U]
                                    //       onDestroyScreen switches on it: 1 cGame_exit, 6 push cFEStateTitle, 7 push
                                    //       cFEStateLodge, 0 set up a new race and cPreGameLoadScreen
    int mLastProcessState;          // 0x230 [O] (bool); [R] the ctor clears it; [U] displayOn compares mIsProcessing
                                    //       with it, returns when equal, else stores it and refreshes the icon / text
};

CHECK_OFFSET(cOVState_AUTOSAVE, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_AUTOSAVE, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_AUTOSAVE, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_AUTOSAVE, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_AUTOSAVE, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_AUTOSAVE, mOVID, 0x98);
CHECK_OFFSET(cOVState_AUTOSAVE, mLastLoadFileName, 0x9C);
CHECK_OFFSET(cOVState_AUTOSAVE, mFileName, 0x11C);
CHECK_OFFSET(cOVState_AUTOSAVE, mPopupType, 0x19C);
CHECK_OFFSET(cOVState_AUTOSAVE, mProcessingKeyboard, 0x1A0);
CHECK_OFFSET(cOVState_AUTOSAVE, mpKeyboard, 0x1A4);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsProcessing, 0x1A8);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsDone, 0x1AC);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsLoadWait, 0x1B0);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsLoadDoneOK, 0x1B4);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsInvalidConfirmed, 0x1B8);
CHECK_OFFSET(cOVState_AUTOSAVE, mType, 0x1BC);
CHECK_OFFSET(cOVState_AUTOSAVE, mState, 0x1C0);
CHECK_OFFSET(cOVState_AUTOSAVE, mSlot, 0x1C4);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsReplayAlloc, 0x1C8);
CHECK_OFFSET(cOVState_AUTOSAVE, mPlayerIdx, 0x1CC);
CHECK_OFFSET(cOVState_AUTOSAVE, mpReadBuffer, 0x1D0);
CHECK_OFFSET(cOVState_AUTOSAVE, mpStartReadBuffer, 0x1D4);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsManagerStarted, 0x1D8);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsSavePressed, 0x1DC);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsDeletePressed, 0x1E0);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsCancelPressed, 0x1E4);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsFormatPressed, 0x1E8);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsRetryPressed, 0x1EC);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsOverwriteOK, 0x1F0);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsLoadPressed, 0x1F4);
CHECK_OFFSET(cOVState_AUTOSAVE, mDoNextMemCard, 0x1F8);
CHECK_OFFSET(cOVState_AUTOSAVE, mDoPrevMemCard, 0x1FC);
CHECK_OFFSET(cOVState_AUTOSAVE, mLoadBEData, 0x200);
CHECK_OFFSET(cOVState_AUTOSAVE, mAutoLoadOn, 0x204);
CHECK_OFFSET(cOVState_AUTOSAVE, mAutoSaveOn, 0x208);
CHECK_OFFSET(cOVState_AUTOSAVE, mFirstCheck, 0x20C);
CHECK_OFFSET(cOVState_AUTOSAVE, mIsStatusPopupOn, 0x210);
CHECK_OFFSET(cOVState_AUTOSAVE, mScreenStatus, 0x214);
CHECK_OFFSET(cOVState_AUTOSAVE, mStatusPopup, 0x218);
CHECK_OFFSET(cOVState_AUTOSAVE, mLastSelectedDevice, 0x21C);
CHECK_OFFSET(cOVState_AUTOSAVE, mDeleteButtonActive, 0x220);
CHECK_OFFSET(cOVState_AUTOSAVE, mMainButtonActive, 0x224);
CHECK_OFFSET(cOVState_AUTOSAVE, mBufferState, 0x228);
CHECK_OFFSET(cOVState_AUTOSAVE, mParentScreen, 0x22C);
CHECK_OFFSET(cOVState_AUTOSAVE, mLastProcessState, 0x230);
CHECK_SIZE(cOVState_AUTOSAVE, 0x234);

#endif /* FE_OVSTATEAUTOSAVE_H */
