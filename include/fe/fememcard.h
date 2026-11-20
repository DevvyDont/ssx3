#ifndef FE_FEMEMCARD_H
#define FE_FEMEMCARD_H

// cFEMemCard: the memory-card screen base (load / save / delete / format flow on top of cMCOverlayManager). The FE and
// overlay save screens derive from it: cFEStateMemCard, cFEStateProfileLoad, cFEStateOptionsDeviceSelect,
// cOVState_AUTOSAVE, cOVState_SAVEOPTIONS, cOVState_SAVEREPLAY, cOVState_PROFILE (their own fields start at 0x22C).
// EA's source is fe/fememcard.cpp (the July 2003 preview's asserts in cFEMemCard::onDestroyScreen, builds/x/v2
// file_hints). Retail splits it over fe/festatetrophyroom (0x001D5330 ... 0x001D64C0), fe/fememcardcallbacks
// (0x001D66B0 ... 0x001D9948) and main/all_in_one_main (the is*Pressed accessors 0x001DDD28 ... 0x001DDE18).
//
// Size 0x22C:
//   [R] every subclass constructor runs cFEMemCard_cFEMemCard2 and then writes its own fields from 0x22C on
//       (cFEStateOptionsDeviceSelect 0x22C-0x238, cFEStateMemCard 0x22C-0x244 (alloc 584 = 0x248), cFEStateProfileLoad
//       0x22C/0x230/0x234/0x27C), and cFEMemCard_initFEMemCard clears the last base word, 0x228.
//   On Tour's DWARF says 0x164: its cOVState is 0x90 (retail 0x9C), its two file names are unsigned short[32] (retail
//   [64]) and its 22 bools are 1 byte (retail: words). With those three changes On Tour's 36 members after the file
//   names fill retail's 0x19C-0x22C exactly, in On Tour's order, and every one is confirmed by retail use below.
//
// Base: cOVState (fe/ovstate.h, 0x9C) : cUIState (0x48) : cListNode (vptr at 0x8). There are two constructors:
// cFEMemCard_cFEMemCard2(engine, iType) (cOVState inlined, mOVID = 0) and cFEMemCard_cFEMemCard(engine, iType, eOVState)
// (mOVID = eOVState; used by cOVState_SAVEREPLAY & co. via ovstatepause.cpp), plus the default one
// cFEMemCard_cFEMemCard1. All three store cOVState_vtbl, then cFEMemCard_vtbl (_vt$10cFEMemCard, 47 slots) at 0x8.
// The one live screen is the class static cFEMemCard::mInstance = cFEMemCard_mInstance (D_004A2028), set by
// cFEMemCard_initFEMemCard; the static callbacks (cFEMemCard_callback*, confirmLoad, exitDelete, readStart, readDone,
// writeStart, callbackRead/WriteUpdate, is*Pressed) work through it instead of `this`.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags:
//   [R] retail's constructors and cFEMemCard_initFEMemCard (which sets every own word: 0x19C = 25, 0x1BC = iType,
//       the rest 0; the short at 0x9C = 0; the file-name pattern into 0x11C);
//   [U] how retail code uses the field (cFEMemCard_* asm and C bodies, and the subclasses' methods);
//   [O] SSX On Tour's DWARF (class cFEMemCard): member names, types and order.
// Retail bools are words (int here); On Tour's types are noted where they differ.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cFEMemCard {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEMemCard_vtbl (_vt$10cFEMemCard), stored over cOVState_vtbl by the
                                    //       ctors; [U] the screen's virtual calls (getPopup, cancel,
                                    //       triggerDisplayState ...)
    // --- cUIState (fe/ovstate.h; same offsets as On Tour)
    int mHashName;                  // 0x00C [O]; [R] zeroed by the cUIState ctor; subclasses store their state id
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] cFEMemCard_getPopup passes it to
                                    //       cFEPopupConfirm and pushes the popup on its state stack (+0x18)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; accessors lw/sw the word)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (vptr at 0x3C)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] cUIScreen_getObjectByHashName(mpScreen, "onmenu" ...)
                                    //       (onGainFocus, setMenuHighlight, triggerDisplayState, triggerArrowPulse,
                                    //       onUpdate)
    signed char mUserNum;           // 0x044 [O]; [U] cFEStateProfileLoad's ctor sets it from mPlayerIdx
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h; its ctor is inlined into the cFEMemCard ctors)
    int mDoneTransition;            // 0x048 [O] (bool); [R] the ctors zero it
    int mAllowPauseMenu;            // 0x04C [O] (bool); [R] the ctors zero it
    int mRestoreCutSceneMode;       // 0x050 [O] (bool)
    int mRemovePDA;                 // 0x054 [O] (bool); [R] the ctors zero it
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " into it (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] 0, or cFEMemCard_cFEMemCard's eOVState argument
    // --- cFEMemCard
    unsigned short mLastLoadFileName[64]; // 0x09C [O] ([32] in On Tour); [R] initFEMemCard clears the first char (sh);
                                    //       [U] cFEMemCard_SetLoadPressed(1) has the card (manager +0x434, slot 14)
                                    //       write
                                    //       the selected file's title here, SetLoadPressed(0) clears it
    unsigned short mFileName[64];   // 0x11C [O] ([32] in On Tour); [R] initFEMemCard sprintf1s the "<product>-<type>*"
                                    //       file pattern into it (cMCOverlayManager_getFileName, "%s-%s*")
    int mPopupType;                 // 0x19C [O] (unsigned int in On Tour); [R] initFEMemCard sets 25; [U] the popup id:
                                    //       each callback sets it before getPopup, onPopupEvent switches on it
    int mProcessingKeyboard;        // 0x1A0 [O] (bool); [U] cFEStateProfileLoad_onWidgetEvent sets it when it pushes
                                    //       the name keyboard; onPopupEvent (event 22) clears it and mpKeyboard
    cKeyboardPopup* mpKeyboard;     // 0x1A4 [O]; [U] cFEStateProfileLoad_onWidgetEvent's keyboard popup; onUpdate skips
                                    //       the base update while it is set
    int mIsProcessing;              // 0x1A8 [O] (bool); [U] returned by cFEMemCard_isDeviceSelected; cleared by
                                    //       cancel, exitSave, exitDelete, callbackReadSuccess; tested by onInputBegin
    int mIsDone;                    // 0x1AC [O] (bool); [U] the subclasses' updateDone / exitScreen test and set it
    int mIsLoadWait;                // 0x1B0 [O] (bool); [U] readStart sets 1; onUpdate: busy while 1 and
                                    //       cMCOverlayManager_isManagerDone is false
    int mIsLoadDoneOK;              // 0x1B4 [O] (bool); [U] readStart clears it, callbackReadSuccess sets it;
                                    //       onDestroyScreen tests it
    int mIsInvalidConfirmed;        // 0x1B8 [O] (bool); [R] initFEMemCard clears it (no other retail use found)
    int mType;                      // 0x1BC [O]; [R] initFEMemCard's iType; [U] 0 profile, 1 replay, 2 settings
                                    //       (readStart, readDone, writeStart, callbackRead/WriteUpdate)
    int mState;                     // 0x1C0 [O]; [U] the screen state onUpdate dispatches on: 0 updateStart,
                                    //       1 updateChecking, 2 updateSkip, 3 updateSaving, 4 updateDeleting,
                                    //       5 updateLoading, 6 updateDone; exitSave / exitDelete set it back to 1
    int mSlot;                      // 0x1C4 [O]; [U] the selected menu row (setMenuHighlight stores it); readStart
                                    //       hands
                                    //       it to the manager (+0xF8) and to cMCOverlayManager_readFile
    int mIsReplayAlloc;             // 0x1C8 [O] (bool); [U] writeStart sets it for a replay save;
                                    //       cOVState_SAVEREPLAY_onDestroyScreen / onPopupEvent test and clear it
    int mPlayerIdx;                 // 0x1CC [O] (unsigned int in On Tour); [U] the profile slot for
                                    //       cBESaveInterface_generateSaveProfile (writeStart) / loadSaveProfile
                                    //       (readDone);
                                    //       set by the cFEStateProfileLoad / cOVState_PROFILE ctors
    char* mpReadBuffer;             // 0x1D0 [O]; [U] the read cursor: createReadBuffer sets it to the new block,
                                    //       callbackReadUpdate advances it, readDone loads from it
    char* mpStartReadBuffer;        // 0x1D4 [O]; [U] the block itself: createReadBuffer / readDone / onDestroyScreen
                                    //       free it when mBufferState is 1; writeStart writes the replay from it
    int mIsManagerStarted;          // 0x1D8 [O] (bool); [U] onUpdate starts the manager (ManagerStart) once and sets it
    int mIsSavePressed;             // 0x1DC [O] (bool); [U] returned by cFEMemCard_isSavePressed
    int mIsDeletePressed;           // 0x1E0 [O] (bool); [U] returned by cFEMemCard_isDeletePressed; exitDelete clears
                                    //       it
    int mIsCancelPressed;           // 0x1E4 [O] (bool); [U] returned and cleared by cFEMemCard_isCancelPressed
    int mIsFormatPressed;           // 0x1E8 [O] (bool); [U] returned by cFEMemCard_isFormatPressed
    int mIsRetryPressed;            // 0x1EC [O] (bool); [U] returned and cleared by cFEMemCard_callbackIsRetryPressed
    int mIsOverwriteOK;             // 0x1F0 [O] (bool); [U] returned by cFEMemCard_isOWPressed; onPopupEvent sets it
    int mIsLoadPressed;             // 0x1F4 [O] (bool); [U] returned by cFEMemCard_isLoadPressed; SetLoadPressed stores
                                    //       it
    int mDoNextMemCard;             // 0x1F8 [O] (bool); [U] returned and cleared by cFEMemCard_isNextMCPressed;
                                    //       onInputBegin sets it
    int mDoPrevMemCard;             // 0x1FC [O] (bool); [U] returned and cleared by cFEMemCard_isPrevMCPressed;
                                    //       onInputBegin sets it
    int mLoadBEData;                // 0x200 [O] (bool); [U] readStart sets it for a replay read; callbackReadUpdate
                                    //       CRC-checks and loads the back end once, then clears it
    int mAutoLoadOn;                // 0x204 [O] (bool); [U] callbackReadSuccess / callbackDeleteDone / onPopupEvent
                                    //       skip
                                    //       the popup and exit when set; onUpdate skips the status popup
    int mAutoSaveOn;                // 0x208 [O] (bool); [U] callbackSaveSuccess exits without a popup when set;
                                    //       cOVState_AUTOSAVE's ctor sets it
    int mFirstCheck;                // 0x20C [O] (bool); [R] initFEMemCard clears it (no other retail use found)
    int mIsStatusPopupOn;           // 0x210 [O] (bool); [U] displayStatusPopup and writeStart set it; getPopup clears
                                    //       it
                                    //       (unless popup 22); onUpdate refreshes the popup text while it is set
    int mScreenStatus;              // 0x214 [O]; [U] the display state: triggerDisplayState stores its argument,
                                    //       updateStart and the Display* callbacks test and set it
    cFEPopupConfirm* mStatusPopup;  // 0x218 [O]; [U] getPopup allocates the cFEPopupConfirm (3068 bytes) into it or
                                    //       re-inits it; callbacks label its buttons (popup slot 26)
    int mLastSelectedDevice;        // 0x21C [O]; [R] initFEMemCard clears it; [U] the subclasses' displayStatus /
                                    //       displayOn store the device they show
    int mDeleteButtonActive;        // 0x220 [O] (bool); [U] cFEStateProfileLoad_hideDeleteButton and the init*Screen
                                    //       functions store it
    int mMainButtonActive;          // 0x224 [O] (bool); [U] cFEStateProfileLoad_hideMainButton and the init*Screen
                                    //       functions store it
    int mBufferState;               // 0x228 [O] eBufferState; [U] 1 = mpStartReadBuffer was allocated by
                                    //       createReadBuffer
                                    //       (freed by readDone / onDestroyScreen), 2 = the save interface's replay
                                    //       buffer
                                    //       (readStart), 0 = none
};

CHECK_OFFSET(cFEMemCard, mpPrev, 0x0);
CHECK_OFFSET(cFEMemCard, mpNext, 0x4);
CHECK_OFFSET(cFEMemCard, mVtbl, 0x8);
CHECK_OFFSET(cFEMemCard, mHashName, 0xC);
CHECK_OFFSET(cFEMemCard, mpEngine, 0x10);
CHECK_OFFSET(cFEMemCard, mCurrentCtrl, 0x14);
CHECK_OFFSET(cFEMemCard, mCtrlBits, 0x15);
CHECK_OFFSET(cFEMemCard, unk_016, 0x16);
CHECK_OFFSET(cFEMemCard, miMiscData, 0x18);
CHECK_OFFSET(cFEMemCard, mAttrState, 0x1C);
CHECK_OFFSET(cFEMemCard, mpParentState, 0x20);
CHECK_OFFSET(cFEMemCard, mScreens, 0x24);
CHECK_OFFSET(cFEMemCard, mpScreen, 0x40);
CHECK_OFFSET(cFEMemCard, mUserNum, 0x44);
CHECK_OFFSET(cFEMemCard, unk_045, 0x45);
CHECK_OFFSET(cFEMemCard, mDoneTransition, 0x48);
CHECK_OFFSET(cFEMemCard, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cFEMemCard, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cFEMemCard, mRemovePDA, 0x54);
CHECK_OFFSET(cFEMemCard, mszChatBuff, 0x58);
CHECK_OFFSET(cFEMemCard, mOVID, 0x98);
CHECK_OFFSET(cFEMemCard, mLastLoadFileName, 0x9C);
CHECK_OFFSET(cFEMemCard, mFileName, 0x11C);
CHECK_OFFSET(cFEMemCard, mPopupType, 0x19C);
CHECK_OFFSET(cFEMemCard, mProcessingKeyboard, 0x1A0);
CHECK_OFFSET(cFEMemCard, mpKeyboard, 0x1A4);
CHECK_OFFSET(cFEMemCard, mIsProcessing, 0x1A8);
CHECK_OFFSET(cFEMemCard, mIsDone, 0x1AC);
CHECK_OFFSET(cFEMemCard, mIsLoadWait, 0x1B0);
CHECK_OFFSET(cFEMemCard, mIsLoadDoneOK, 0x1B4);
CHECK_OFFSET(cFEMemCard, mIsInvalidConfirmed, 0x1B8);
CHECK_OFFSET(cFEMemCard, mType, 0x1BC);
CHECK_OFFSET(cFEMemCard, mState, 0x1C0);
CHECK_OFFSET(cFEMemCard, mSlot, 0x1C4);
CHECK_OFFSET(cFEMemCard, mIsReplayAlloc, 0x1C8);
CHECK_OFFSET(cFEMemCard, mPlayerIdx, 0x1CC);
CHECK_OFFSET(cFEMemCard, mpReadBuffer, 0x1D0);
CHECK_OFFSET(cFEMemCard, mpStartReadBuffer, 0x1D4);
CHECK_OFFSET(cFEMemCard, mIsManagerStarted, 0x1D8);
CHECK_OFFSET(cFEMemCard, mIsSavePressed, 0x1DC);
CHECK_OFFSET(cFEMemCard, mIsDeletePressed, 0x1E0);
CHECK_OFFSET(cFEMemCard, mIsCancelPressed, 0x1E4);
CHECK_OFFSET(cFEMemCard, mIsFormatPressed, 0x1E8);
CHECK_OFFSET(cFEMemCard, mIsRetryPressed, 0x1EC);
CHECK_OFFSET(cFEMemCard, mIsOverwriteOK, 0x1F0);
CHECK_OFFSET(cFEMemCard, mIsLoadPressed, 0x1F4);
CHECK_OFFSET(cFEMemCard, mDoNextMemCard, 0x1F8);
CHECK_OFFSET(cFEMemCard, mDoPrevMemCard, 0x1FC);
CHECK_OFFSET(cFEMemCard, mLoadBEData, 0x200);
CHECK_OFFSET(cFEMemCard, mAutoLoadOn, 0x204);
CHECK_OFFSET(cFEMemCard, mAutoSaveOn, 0x208);
CHECK_OFFSET(cFEMemCard, mFirstCheck, 0x20C);
CHECK_OFFSET(cFEMemCard, mIsStatusPopupOn, 0x210);
CHECK_OFFSET(cFEMemCard, mScreenStatus, 0x214);
CHECK_OFFSET(cFEMemCard, mStatusPopup, 0x218);
CHECK_OFFSET(cFEMemCard, mLastSelectedDevice, 0x21C);
CHECK_OFFSET(cFEMemCard, mDeleteButtonActive, 0x220);
CHECK_OFFSET(cFEMemCard, mMainButtonActive, 0x224);
CHECK_OFFSET(cFEMemCard, mBufferState, 0x228);
CHECK_SIZE(cFEMemCard, 0x22C);

// The live memory-card screen (On Tour: static cFEMemCard::mInstance) is cFEMemCard_mInstance (D_004A2028), set by
// cFEMemCard_initFEMemCard. Not declared here yet: 38 units still declare it locally as `void*`.

#endif /* FE_FEMEMCARD_H */
