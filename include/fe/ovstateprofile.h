#ifndef FE_OVSTATEPROFILE_H
#define FE_OVSTATEPROFILE_H

// cOVState_PROFILE: the in-game profile save overlay (screen "121Profilesave_pda" on the PDA template; six "player%d"
// slots, the name keyboard, save / delete), a cFEMemCard with mType 0. Retail code: the ctor
// cOVState_PROFILE_cOVState_PROFILE in fe/ovstatetrophy (0x00211088), cOVState_PROFILE_* in fe/ovstateprofile
// (0x002111A0 ... 0x00212080), the destructor cOVState_PROFILE__cOVState_PROFILE and the engine-less ctor copy
// func_00221DB0 in fe/all_in_one_fe. On Tour defines it in fe/all_in_one_fe.cpp.
//
// Size 0x27C:
//   [R] operator new(636, "cOVStateOverlay") for overlay 36 (cOVStateManager_triggerOverlay / triggerPopup) and
//       cMemMan_alloc(636, "Save Profile") in cOVState_PAUSE_OPTIONS_onWidgetEvent / cOVTemplate_Dialog_onWidgetEvent;
//       the ctor writes 0x22C, 0x230 and 0x278 (its last word).
//   [O] On Tour 0x1AC = cFEMemCard 0x164 + bool mIsOn, bool mIsHidden, unsigned short mTitle[32] and tOVStateEnum
//       mParentOV; retail: 0x22C + two words + 33 chars (0x42) + 2 bytes of padding + a word = 0x27C.
// Base: cFEMemCard (fe/fememcard.h, 0x22C) : cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8).
// The ctor cOVState_PROFILE(cUIEngine*, unsigned char ucActiveControl, tOVStateEnum ov) runs
// cFEMemCard_cFEMemCard(this, pEngine, 0).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. The base classes
// cListNode / cUIState / cOVState / cFEMemCard are written out inline with fe/fememcard.h's member names (see that
// header for the full evidence of the inherited fields; only this class's own uses are added here).
// Evidence tags:
//   [R] retail's constructors and allocation sizes;
//   [P] the July 2003 preview's inline accessors (inherited fields only);
//   [U] how retail code uses the field (this class's methods, asm and C);
//   [O] SSX On Tour's DWARF (class cOVState_PROFILE): member names, types and order. On Tour's bools are 1 byte
//       (retail: words), its cOVState is 0x90 (retail 0x9C) and its cFEMemCard 0x164 (retail 0x22C).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cOVState_PROFILE {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVState_PROFILE_vtbl (0x00472928, 47 slots), stored by
                                    //       cOVState_PROFILE_cOVState_PROFILE over cFEMemCard_vtbl
    // --- cUIState (ui/uistate.h; same offsets as On Tour)
    int mHashName;                  // 0x00C [O]; [R] the ctor stores 36 (0x24)
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's pEngine; [U] onCreateScreen adds "121Profilesave_pda";
                                    //       onWidgetEvent builds the cKeyboardPopup for it
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF; [U] onWidgetEvent passes it to the
                                    //       cKeyboardPopup ctor
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData lw/sw
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; lw/sw the word); [U]
                                    //       cOVState_PAUSE_OPTIONS_onWidgetEvent sets mode 1 (bits 8-13) after
                                    //       constructing it
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen keeps the "121Profilesave_pda" screen here; every
                                    //       widget lookup
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (bool); [R] the (inlined) cOVState ctor zeroes it
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O] (bool); [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O] (bool); [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " (D_004A2088) into it
                                    //       (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] 0 (cFEMemCard_cFEMemCard)
    // --- cFEMemCard (fe/fememcard.h, 0x22C)
    unsigned short mLastLoadFileName[64]; // 0x09C [O] ([32] in On Tour); [R] initFEMemCard clears the first char
    unsigned short mFileName[64];   // 0x11C [O] ([32] in On Tour); [R] initFEMemCard writes the "%s-%s*" file pattern
    int mPopupType;                 // 0x19C [O]; [U] onWidgetEvent clears it when it opens the name keyboard
    int mProcessingKeyboard;        // 0x1A0 [O] (bool); [U] onWidgetEvent sets it when it pushes the name keyboard
    cKeyboardPopup* mpKeyboard;     // 0x1A4 [O]; [U] onWidgetEvent creates the "Keyboard" cKeyboardPopup (1092 bytes)
                                    //       once and reads its text (+0x74); onDestroyScreen clears it
    int mIsProcessing;              // 0x1A8 [O] (bool); [U] onWidgetEvent tests / sets it
    int mIsDone;                    // 0x1AC [O] (bool); [U] updateDone tests and sets it
    int mIsLoadWait;                // 0x1B0 [O] (bool); [U] cFEMemCard_readStart / onUpdate
    int mIsLoadDoneOK;              // 0x1B4 [O] (bool); [U] cFEMemCard_readStart / callbackReadSuccess
    int mIsInvalidConfirmed;        // 0x1B8 [O] (bool); [R] initFEMemCard clears it
    int mType;                      // 0x1BC [O]; [R] 0 (profile): the ctor runs cFEMemCard_cFEMemCard(self, pEngine,
                                    //       0)
    int mState;                     // 0x1C0 [O]; [U] onWidgetEvent / displayStatus test it
    int mSlot;                      // 0x1C4 [O]; [U] onWidgetEvent stores the picked item's misc data
    int mIsReplayAlloc;             // 0x1C8 [O] (bool); [U] cFEMemCard_writeStart sets it for a replay save
    int mPlayerIdx;                 // 0x1CC [O]; [R] the ctor clears it, then stores the player (0/1) using the
                                    //       opening controller; [U] onWidgetEvent names that profile
    char* mpReadBuffer;             // 0x1D0 [O]; [U] cFEMemCard_createReadBuffer / callbackReadUpdate
    char* mpStartReadBuffer;        // 0x1D4 [O]; [U] cFEMemCard_createReadBuffer / readDone / onDestroyScreen
    int mIsManagerStarted;          // 0x1D8 [O] (bool); [U] cFEMemCard_onUpdate
    int mIsSavePressed;             // 0x1DC [O] (bool); [U] onWidgetEvent sets it with the save
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
    int mScreenStatus;              // 0x214 [O]; [U] displayStatus / displayOn test 3; onWidgetEvent tests it
    cFEPopupConfirm* mStatusPopup;  // 0x218 [O]; [U] cFEMemCard_getPopup allocates it
    int mLastSelectedDevice;        // 0x21C [O]; [R] initFEMemCard clears it; [U] displayOn stores the device it shows
    int mDeleteButtonActive;        // 0x220 [O] (bool); [U] hideDeleteButton stores !hide
    int mMainButtonActive;          // 0x224 [O] (bool); [U] hideMainButton stores !hide
    int mBufferState;               // 0x228 [O] eBufferState; [U] cFEMemCard_createReadBuffer / readStart / readDone
    // --- cOVState_PROFILE
    int mIsOn;                      // 0x22C [O] (bool); [R] the ctor clears it; [U] displayStatus sets it once the
                                    //       card state is nonzero; hideActionButtons keeps the buttons hidden while it
                                    //       is clear
    int mIsHidden;                  // 0x230 [O] (bool); [R] the ctor clears it; [U] hideActionButtons compares and
                                    //       stores the new hidden state
    unsigned short mTitle[33];      // 0x234 [O] ([32] in On Tour; retail titles are kMCTitleStringSize = 33 chars);
                                    //       [U] displayOn has the card (mpMemCard slot 14) write each slot's title
                                    //       here and shows it
    char unk_276[0x2];              // 0x276 padding
    int mParentOV;                  // 0x278 [O] tOVStateEnum; [R] the ctor's last argument (42 from
                                    //       cOVStateManager_triggerOverlay and cOVState_PAUSE_OPTIONS_onWidgetEvent,
                                    //       0x18 from cOVTemplate_Dialog_onWidgetEvent); [U] updateDone tests 0x18
};

CHECK_OFFSET(cOVState_PROFILE, mpPrev, 0x0);
CHECK_OFFSET(cOVState_PROFILE, mpNext, 0x4);
CHECK_OFFSET(cOVState_PROFILE, mVtbl, 0x8);
CHECK_OFFSET(cOVState_PROFILE, mHashName, 0xC);
CHECK_OFFSET(cOVState_PROFILE, mpEngine, 0x10);
CHECK_OFFSET(cOVState_PROFILE, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_PROFILE, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_PROFILE, unk_016, 0x16);
CHECK_OFFSET(cOVState_PROFILE, miMiscData, 0x18);
CHECK_OFFSET(cOVState_PROFILE, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_PROFILE, mpParentState, 0x20);
CHECK_OFFSET(cOVState_PROFILE, mScreens, 0x24);
CHECK_OFFSET(cOVState_PROFILE, mpScreen, 0x40);
CHECK_OFFSET(cOVState_PROFILE, mUserNum, 0x44);
CHECK_OFFSET(cOVState_PROFILE, unk_045, 0x45);
CHECK_OFFSET(cOVState_PROFILE, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_PROFILE, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_PROFILE, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_PROFILE, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_PROFILE, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_PROFILE, mOVID, 0x98);
CHECK_OFFSET(cOVState_PROFILE, mLastLoadFileName, 0x9C);
CHECK_OFFSET(cOVState_PROFILE, mFileName, 0x11C);
CHECK_OFFSET(cOVState_PROFILE, mPopupType, 0x19C);
CHECK_OFFSET(cOVState_PROFILE, mProcessingKeyboard, 0x1A0);
CHECK_OFFSET(cOVState_PROFILE, mpKeyboard, 0x1A4);
CHECK_OFFSET(cOVState_PROFILE, mIsProcessing, 0x1A8);
CHECK_OFFSET(cOVState_PROFILE, mIsDone, 0x1AC);
CHECK_OFFSET(cOVState_PROFILE, mIsLoadWait, 0x1B0);
CHECK_OFFSET(cOVState_PROFILE, mIsLoadDoneOK, 0x1B4);
CHECK_OFFSET(cOVState_PROFILE, mIsInvalidConfirmed, 0x1B8);
CHECK_OFFSET(cOVState_PROFILE, mType, 0x1BC);
CHECK_OFFSET(cOVState_PROFILE, mState, 0x1C0);
CHECK_OFFSET(cOVState_PROFILE, mSlot, 0x1C4);
CHECK_OFFSET(cOVState_PROFILE, mIsReplayAlloc, 0x1C8);
CHECK_OFFSET(cOVState_PROFILE, mPlayerIdx, 0x1CC);
CHECK_OFFSET(cOVState_PROFILE, mpReadBuffer, 0x1D0);
CHECK_OFFSET(cOVState_PROFILE, mpStartReadBuffer, 0x1D4);
CHECK_OFFSET(cOVState_PROFILE, mIsManagerStarted, 0x1D8);
CHECK_OFFSET(cOVState_PROFILE, mIsSavePressed, 0x1DC);
CHECK_OFFSET(cOVState_PROFILE, mIsDeletePressed, 0x1E0);
CHECK_OFFSET(cOVState_PROFILE, mIsCancelPressed, 0x1E4);
CHECK_OFFSET(cOVState_PROFILE, mIsFormatPressed, 0x1E8);
CHECK_OFFSET(cOVState_PROFILE, mIsRetryPressed, 0x1EC);
CHECK_OFFSET(cOVState_PROFILE, mIsOverwriteOK, 0x1F0);
CHECK_OFFSET(cOVState_PROFILE, mIsLoadPressed, 0x1F4);
CHECK_OFFSET(cOVState_PROFILE, mDoNextMemCard, 0x1F8);
CHECK_OFFSET(cOVState_PROFILE, mDoPrevMemCard, 0x1FC);
CHECK_OFFSET(cOVState_PROFILE, mLoadBEData, 0x200);
CHECK_OFFSET(cOVState_PROFILE, mAutoLoadOn, 0x204);
CHECK_OFFSET(cOVState_PROFILE, mAutoSaveOn, 0x208);
CHECK_OFFSET(cOVState_PROFILE, mFirstCheck, 0x20C);
CHECK_OFFSET(cOVState_PROFILE, mIsStatusPopupOn, 0x210);
CHECK_OFFSET(cOVState_PROFILE, mScreenStatus, 0x214);
CHECK_OFFSET(cOVState_PROFILE, mStatusPopup, 0x218);
CHECK_OFFSET(cOVState_PROFILE, mLastSelectedDevice, 0x21C);
CHECK_OFFSET(cOVState_PROFILE, mDeleteButtonActive, 0x220);
CHECK_OFFSET(cOVState_PROFILE, mMainButtonActive, 0x224);
CHECK_OFFSET(cOVState_PROFILE, mBufferState, 0x228);
CHECK_OFFSET(cOVState_PROFILE, mIsOn, 0x22C);
CHECK_OFFSET(cOVState_PROFILE, mIsHidden, 0x230);
CHECK_OFFSET(cOVState_PROFILE, mTitle, 0x234);
CHECK_OFFSET(cOVState_PROFILE, unk_276, 0x276);
CHECK_OFFSET(cOVState_PROFILE, mParentOV, 0x278);
CHECK_SIZE(cOVState_PROFILE, 0x27C);

#endif /* FE_OVSTATEPROFILE_H */
