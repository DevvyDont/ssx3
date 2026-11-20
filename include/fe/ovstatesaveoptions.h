#ifndef FE_OVSTATESAVEOPTIONS_H
#define FE_OVSTATESAVEOPTIONS_H

// cOVState_SAVEOPTIONS: the in-game save-records (settings) memory-card overlay (screen "119Save_Replay" titled
// "title_Save_Records"; overlay id 19), a cFEMemCard with mType 2 (settings). Retail code: cOVState_SAVEOPTIONS_* in
// fe/ovstatepause (EA's d:/ssx/code/source/fe/ovstatepause.cpp; the class is probably declared in its ovstatepause.h,
// which the preview's asserts also name) and fe/all_in_one_fe (destructor cOVState_SAVEOPTIONS__cOVState_SAVEOPTIONS,
// the engine-less ctor copy func_002220C0).
//
// Size 0x274:
//   [R] cOVStateManager_triggerOverlay / triggerPopup: operator new(628, "cOVStateOverlay") for overlay 19, then
//       cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS(this, engine, 19), which runs func_001D5330(this, engine, 2,
//       eOVState) and clears 0x22C.
//   [O] On Tour 0x1A8 = cFEMemCard 0x164 + bool mIsHidden + unsigned short mTitle[32]; retail: 0x22C + a word + 33
//       chars (0x42) + 2 bytes of padding = 0x274.
// Base: cFEMemCard (fe/fememcard.h, 0x22C) : cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. The base classes
// cListNode / cUIState / cOVState / cFEMemCard are written out inline with fe/fememcard.h's member names (see that
// header for the full evidence of the inherited fields; only this class's own uses are added here).
// Evidence tags:
//   [R] retail's constructors and allocation sizes;
//   [P] the July 2003 preview's inline accessors (inherited fields only);
//   [U] how retail code uses the field (this class's methods, asm and C);
//   [O] SSX On Tour's DWARF (class cOVState_SAVEOPTIONS): member names, types and order. On Tour's bools are 1 byte
//       (retail: words), its cOVState is 0x90 (retail 0x9C) and its cFEMemCard 0x164 (retail 0x22C).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cOVState_SAVEOPTIONS {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVState_SAVEOPTIONS_vtbl (0x004722B8, 47 slots), stored by
                                    //       cOVState_SAVEOPTIONS_cOVState_SAVEOPTIONS over cFEMemCard_vtbl
    // --- cUIState (ui/uistate.h; same offsets as On Tour)
    int mHashName;                  // 0x00C [O]; [R] zeroed by the cUIState ctor (func_001D5330 does not set it)
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine; [U] onCreateScreen adds the screens;
                                    //       updateDone pops its state stack (+0x18)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData lw/sw
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; the accessors lw/sw the word)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen keeps the overlay screen here; every widget lookup
                                    //       (cUIScreen_getObjectByHashName)
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (bool); [R] the (inlined) cOVState ctor zeroes it
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O] (bool); [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O] (bool); [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " (str_sp_b) into it
                                    //       (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] func_001D5330's eOVState (19 from
                                    //       cOVStateManager_triggerOverlay)
    // --- cFEMemCard (fe/fememcard.h, 0x22C)
    unsigned short mLastLoadFileName[64]; // 0x09C [O] ([32] in On Tour); [R] initFEMemCard clears the first char
    unsigned short mFileName[64];   // 0x11C [O] ([32] in On Tour); [R] initFEMemCard writes the "%s-%s*" file pattern
    int mPopupType;                 // 0x19C [O]; [R] initFEMemCard sets 25; [U] the popup id (cFEMemCard_onPopupEvent)
    int mProcessingKeyboard;        // 0x1A0 [O] (bool)
    cKeyboardPopup* mpKeyboard;     // 0x1A4 [O]; [U] cFEMemCard_onUpdate skips the base update while it is set
    int mIsProcessing;              // 0x1A8 [O] (bool); [U] onWidgetEvent tests / sets it
    int mIsDone;                    // 0x1AC [O] (bool); [U] updateDone tests and sets it
    int mIsLoadWait;                // 0x1B0 [O] (bool); [U] cFEMemCard_readStart / onUpdate
    int mIsLoadDoneOK;              // 0x1B4 [O] (bool); [U] cFEMemCard_readStart / callbackReadSuccess
    int mIsInvalidConfirmed;        // 0x1B8 [O] (bool); [R] initFEMemCard clears it
    int mType;                      // 0x1BC [O]; [R] 2 (settings): the ctor runs func_001D5330(self, engine, 2,
                                    //       eOVState)
    int mState;                     // 0x1C0 [O]; [U] onWidgetEvent / displayStatus switch on it
    int mSlot;                      // 0x1C4 [O]; [U] onWidgetEvent stores the picked row
    int mIsReplayAlloc;             // 0x1C8 [O] (bool); [U] cFEMemCard_writeStart sets it for a replay save
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
    // --- cOVState_SAVEOPTIONS
    int mIsHidden;                  // 0x22C [O] (bool); [R] the ctor clears it; [U] hideActionButtons acts only when
                                    //       its argument differs, then stores it
    unsigned short mTitle[33];      // 0x230 [O] ([32] in On Tour; retail titles are kMCTitleStringSize = 33 chars,
                                    //       like cMCOverlayManager::mActiveTitleString[33]); [U] displayStatus has the
                                    //       card (mpMemCard slot 14, GetTitleString) write a slot's title here and
                                    //       shows it (cUIText_setUnicodeString)
    char unk_272[0x2];              // 0x272 padding to the 4-byte size
};

CHECK_OFFSET(cOVState_SAVEOPTIONS, mpPrev, 0x0);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mpNext, 0x4);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mVtbl, 0x8);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mHashName, 0xC);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mpEngine, 0x10);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_SAVEOPTIONS, unk_016, 0x16);
CHECK_OFFSET(cOVState_SAVEOPTIONS, miMiscData, 0x18);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mpParentState, 0x20);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mScreens, 0x24);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mpScreen, 0x40);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mUserNum, 0x44);
CHECK_OFFSET(cOVState_SAVEOPTIONS, unk_045, 0x45);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mOVID, 0x98);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mLastLoadFileName, 0x9C);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mFileName, 0x11C);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mPopupType, 0x19C);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mProcessingKeyboard, 0x1A0);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mpKeyboard, 0x1A4);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsProcessing, 0x1A8);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsDone, 0x1AC);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsLoadWait, 0x1B0);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsLoadDoneOK, 0x1B4);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsInvalidConfirmed, 0x1B8);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mType, 0x1BC);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mState, 0x1C0);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mSlot, 0x1C4);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsReplayAlloc, 0x1C8);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mPlayerIdx, 0x1CC);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mpReadBuffer, 0x1D0);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mpStartReadBuffer, 0x1D4);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsManagerStarted, 0x1D8);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsSavePressed, 0x1DC);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsDeletePressed, 0x1E0);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsCancelPressed, 0x1E4);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsFormatPressed, 0x1E8);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsRetryPressed, 0x1EC);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsOverwriteOK, 0x1F0);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsLoadPressed, 0x1F4);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mDoNextMemCard, 0x1F8);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mDoPrevMemCard, 0x1FC);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mLoadBEData, 0x200);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mAutoLoadOn, 0x204);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mAutoSaveOn, 0x208);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mFirstCheck, 0x20C);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsStatusPopupOn, 0x210);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mScreenStatus, 0x214);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mStatusPopup, 0x218);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mLastSelectedDevice, 0x21C);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mDeleteButtonActive, 0x220);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mMainButtonActive, 0x224);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mBufferState, 0x228);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mIsHidden, 0x22C);
CHECK_OFFSET(cOVState_SAVEOPTIONS, mTitle, 0x230);
CHECK_OFFSET(cOVState_SAVEOPTIONS, unk_272, 0x272);
CHECK_SIZE(cOVState_SAVEOPTIONS, 0x274);

#endif /* FE_OVSTATESAVEOPTIONS_H */
