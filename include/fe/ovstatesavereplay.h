#ifndef FE_OVSTATESAVEREPLAY_H
#define FE_OVSTATESAVEREPLAY_H

// cOVState_SAVEREPLAY: the in-game save-replay memory-card overlay (screen "119Save_Replay"; overlay id 18), a
// cFEMemCard with mType 1 (replay). Retail code: cOVState_SAVEREPLAY_* in fe/ovstatepause (EA's
// d:/ssx/code/source/fe/ovstatepause.cpp; the class is probably declared in its ovstatepause.h, which the preview's
// asserts also name) and fe/all_in_one_fe (destructor cOVState_SAVEREPLAY__cOVState_SAVEREPLAY, the engine-less ctor
// copy func_00222058).
//
// Size 0x274:
//   [R] cOVStateManager_triggerOverlay / triggerPopup: operator new(628, "cOVStateOverlay") for overlay 18, then
//       cOVState_SAVEREPLAY_cOVState_SAVEREPLAY(this, engine, 18), which runs func_001D5330(this, engine, 1, eOVState)
//       and clears 0x22C.
//   [O] On Tour 0x1A8 = cFEMemCard 0x164 + bool mIsHidden + unsigned short mTitle[32]; retail: 0x22C + a word + 33
//       chars (0x42) + 2 bytes of padding = 0x274.
// Base: cFEMemCard (fe/fememcard.h, 0x22C) : cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8).
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets. The base classes cOVState / cFEMemCard
// are written out inline with fe/fememcard.h's member names (see that header for the full evidence of the inherited
// fields; only this class's own uses are added here).
// Evidence tags:
//   [R] retail's constructors and allocation sizes;
//   [P] the July 2003 preview's inline accessors (inherited fields only);
//   [U] how retail code uses the field (this class's methods, asm and C);
//   [O] SSX On Tour's DWARF (class cOVState_SAVEREPLAY): member names, types and order. On Tour's bools are 1 byte
//       (retail: words), its cOVState is 0x90 (retail 0x9C) and its cFEMemCard 0x164 (retail 0x22C).

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cOVState_SAVEREPLAY : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cOVState_SAVEREPLAY_vtbl (0x00472440, 47 slots), stored by cOVState_SAVEREPLAY_cOVState_SAVEREPLAY over
    //       cFEMemCard_vtbl
    // cOVState_SAVEREPLAY's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds the screens; updateDone pops its state stack (+0x18)
    //   mpScreen: [U] onCreateScreen keeps the overlay screen here; every widget lookup
    //       (cUIScreen_getObjectByHashName)
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (bool); [R] the (inlined) cOVState ctor zeroes it
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O] (bool); [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O] (bool); [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " (str_sp_b) into it
                                    //       (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] func_001D5330's eOVState (18 from
                                    //       cOVStateManager_triggerOverlay)
    // --- cFEMemCard (fe/fememcard.h, 0x22C)
    unsigned short mLastLoadFileName[64]; // 0x09C [O] ([32] in On Tour); [R] initFEMemCard clears the first char
    unsigned short mFileName[64];   // 0x11C [O] ([32] in On Tour); [R] initFEMemCard writes the "%s-%s*" file pattern
    int mPopupType;                 // 0x19C [O]; [R] initFEMemCard sets 25; [U] onPopupEvent frees the replay buffer
                                    //       on popup 5 (save done)
    int mProcessingKeyboard;        // 0x1A0 [O] (bool)
    cKeyboardPopup* mpKeyboard;     // 0x1A4 [O]; [U] cFEMemCard_onUpdate skips the base update while it is set
    int mIsProcessing;              // 0x1A8 [O] (bool); [U] onWidgetEvent tests / sets it
    int mIsDone;                    // 0x1AC [O] (bool); [U] updateDone tests and sets it
    int mIsLoadWait;                // 0x1B0 [O] (bool); [U] cFEMemCard_readStart / onUpdate
    int mIsLoadDoneOK;              // 0x1B4 [O] (bool); [U] cFEMemCard_readStart / callbackReadSuccess
    int mIsInvalidConfirmed;        // 0x1B8 [O] (bool); [R] initFEMemCard clears it
    int mType;                      // 0x1BC [O]; [R] 1 (replay): the ctor runs func_001D5330(self, engine, 1,
                                    //       eOVState)
    int mState;                     // 0x1C0 [O]; [U] onWidgetEvent / displayStatus switch on it
    int mSlot;                      // 0x1C4 [O]; [U] onWidgetEvent stores the picked row
    int mIsReplayAlloc;             // 0x1C8 [O] (bool); [U] onDestroyScreen / onPopupEvent purge the replay save
                                    //       buffer (cReplay_purgeSave) and clear it
    int mPlayerIdx;                 // 0x1CC [O]; [U] the profile slot (cFEMemCard_writeStart / readDone)
    char* mpReadBuffer;             // 0x1D0 [O]; [U] cFEMemCard_createReadBuffer / callbackReadUpdate
    char* mpStartReadBuffer;        // 0x1D4 [O]; [U] cFEMemCard_createReadBuffer / readDone / onDestroyScreen
    int mIsManagerStarted;          // 0x1D8 [O] (bool); [U] cFEMemCard_onUpdate
    int mIsSavePressed;             // 0x1DC [O] (bool); [U] onWidgetEvent sets it
    int mIsDeletePressed;           // 0x1E0 [O] (bool); [U] returned by cFEMemCard_isDeletePressed
    int mIsCancelPressed;           // 0x1E4 [O] (bool); [U] onWidgetEvent sets it (back)
    int mIsFormatPressed;           // 0x1E8 [O] (bool); [U] returned by cFEMemCard_isFormatPressed
    int mIsRetryPressed;            // 0x1EC [O] (bool); [U] cFEMemCard_callbackIsRetryPressed
    int mIsOverwriteOK;             // 0x1F0 [O] (bool); [U] returned by cFEMemCard_isOWPressed
    int mIsLoadPressed;             // 0x1F4 [O] (bool); [U] returned by cFEMemCard_isLoadPressed
    int mDoNextMemCard;             // 0x1F8 [O] (bool); [U] cFEMemCard_isNextMCPressed / onInputBegin
    int mDoPrevMemCard;             // 0x1FC [O] (bool); [U] cFEMemCard_isPrevMCPressed / onInputBegin
    int mLoadBEData;                // 0x200 [O] (bool); [U] cFEMemCard_readStart / callbackReadUpdate
    int mAutoLoadOn;                // 0x204 [O] (bool); [U] cFEMemCard_callbackReadSuccess / onPopupEvent
    int mAutoSaveOn;                // 0x208 [O] (bool); [U] cFEMemCard_callbackSaveSuccess exits without a popup when
                                    //       set
    int mFirstCheck;                // 0x20C [O] (bool); [R] initFEMemCard clears it
    int mIsStatusPopupOn;           // 0x210 [O] (bool); [U] cFEMemCard_displayStatusPopup / getPopup
    int mScreenStatus;              // 0x214 [O]; [U] displayStatus tests 3 and moves there (virtual
                                    //       triggerDisplayState, slot 35)
    cFEPopupConfirm* mStatusPopup;  // 0x218 [O]; [U] cFEMemCard_getPopup allocates it
    int mLastSelectedDevice;        // 0x21C [O]; [R] initFEMemCard clears it
    int mDeleteButtonActive;        // 0x220 [O] (bool); [U] hideDeleteButton stores !hide
    int mMainButtonActive;          // 0x224 [O] (bool); [U] hideMainButton stores !hide
    int mBufferState;               // 0x228 [O] eBufferState; [U] cFEMemCard_createReadBuffer / readStart / readDone
    // --- cOVState_SAVEREPLAY
    int mIsHidden;                  // 0x22C [O] (bool); [R] the ctor clears it; [U] hideActionButtons acts only when
                                    //       its argument differs, then stores it
    unsigned short mTitle[33];      // 0x230 [O] ([32] in On Tour; retail titles are kMCTitleStringSize = 33 chars,
                                    //       like cMCOverlayManager::mActiveTitleString[33]); [U] displayStatus has the
                                    //       card (mpMemCard slot 14, GetTitleString) write a slot's title here and
                                    //       shows it (cUIText_setUnicodeString)
    char unk_272[0x2];              // 0x272 padding to the 4-byte size
};

CHECK_OFFSET(cOVState_SAVEREPLAY, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_SAVEREPLAY, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_SAVEREPLAY, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_SAVEREPLAY, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_SAVEREPLAY, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_SAVEREPLAY, mOVID, 0x98);
CHECK_OFFSET(cOVState_SAVEREPLAY, mLastLoadFileName, 0x9C);
CHECK_OFFSET(cOVState_SAVEREPLAY, mFileName, 0x11C);
CHECK_OFFSET(cOVState_SAVEREPLAY, mPopupType, 0x19C);
CHECK_OFFSET(cOVState_SAVEREPLAY, mProcessingKeyboard, 0x1A0);
CHECK_OFFSET(cOVState_SAVEREPLAY, mpKeyboard, 0x1A4);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsProcessing, 0x1A8);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsDone, 0x1AC);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsLoadWait, 0x1B0);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsLoadDoneOK, 0x1B4);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsInvalidConfirmed, 0x1B8);
CHECK_OFFSET(cOVState_SAVEREPLAY, mType, 0x1BC);
CHECK_OFFSET(cOVState_SAVEREPLAY, mState, 0x1C0);
CHECK_OFFSET(cOVState_SAVEREPLAY, mSlot, 0x1C4);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsReplayAlloc, 0x1C8);
CHECK_OFFSET(cOVState_SAVEREPLAY, mPlayerIdx, 0x1CC);
CHECK_OFFSET(cOVState_SAVEREPLAY, mpReadBuffer, 0x1D0);
CHECK_OFFSET(cOVState_SAVEREPLAY, mpStartReadBuffer, 0x1D4);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsManagerStarted, 0x1D8);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsSavePressed, 0x1DC);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsDeletePressed, 0x1E0);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsCancelPressed, 0x1E4);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsFormatPressed, 0x1E8);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsRetryPressed, 0x1EC);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsOverwriteOK, 0x1F0);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsLoadPressed, 0x1F4);
CHECK_OFFSET(cOVState_SAVEREPLAY, mDoNextMemCard, 0x1F8);
CHECK_OFFSET(cOVState_SAVEREPLAY, mDoPrevMemCard, 0x1FC);
CHECK_OFFSET(cOVState_SAVEREPLAY, mLoadBEData, 0x200);
CHECK_OFFSET(cOVState_SAVEREPLAY, mAutoLoadOn, 0x204);
CHECK_OFFSET(cOVState_SAVEREPLAY, mAutoSaveOn, 0x208);
CHECK_OFFSET(cOVState_SAVEREPLAY, mFirstCheck, 0x20C);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsStatusPopupOn, 0x210);
CHECK_OFFSET(cOVState_SAVEREPLAY, mScreenStatus, 0x214);
CHECK_OFFSET(cOVState_SAVEREPLAY, mStatusPopup, 0x218);
CHECK_OFFSET(cOVState_SAVEREPLAY, mLastSelectedDevice, 0x21C);
CHECK_OFFSET(cOVState_SAVEREPLAY, mDeleteButtonActive, 0x220);
CHECK_OFFSET(cOVState_SAVEREPLAY, mMainButtonActive, 0x224);
CHECK_OFFSET(cOVState_SAVEREPLAY, mBufferState, 0x228);
CHECK_OFFSET(cOVState_SAVEREPLAY, mIsHidden, 0x22C);
CHECK_OFFSET(cOVState_SAVEREPLAY, mTitle, 0x230);
CHECK_OFFSET(cOVState_SAVEREPLAY, unk_272, 0x272);
CHECK_SIZE(cOVState_SAVEREPLAY, 0x274);

#endif /* FE_OVSTATESAVEREPLAY_H */
