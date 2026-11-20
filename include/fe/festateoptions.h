#ifndef FE_FESTATEOPTIONS_H
#define FE_FESTATEOPTIONS_H

// cFEStateOptionsController: the front end's controller options screen ("22control"): per-pad vibration on/off and
// default/pro button layout lists, with a short test buzz when vibration is switched on.
// EA's source: d:/ssx/code/source/fe/festateoptions.cpp (preview strings); retail code: fe/festateoptions
// (func_0018C658 ... 0x0018D108) and the default-ctor copy, the destructor and the empty onLoseFocus in
// main/all_in_one_main (0x001DE080 ... 0x001DE0E8).
//
// Size 0x54:
//   [R] cMemMan_alloc(0x54, "cFEStateOptionsController") + func_0018C658(mem, engine) in cFECustom_getNextState
//       (state 31); the ctors write up to the byte at 0x50.
//   [O] On Tour's class is 0x54 too, with the same members at the same offsets.
// Base: cFEState (no data; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor
// func_0018C658 runs cUIState_cUIState(this, pEngine), stores cFEStateOptionsController_vtbl (0x0046C480, 24 slots: 1
// dtor, 4 onCreateScreen, 5 onDestroyScreen, 11 onLoseFocus func_001DE0E8, 12 onUpdate, 17 onPopupEvent, 19
// onWidgetEvent, 20 onWidgetMenu, 21 onWidgetPair; vtables.tsv stops at slot 10), mHashName 0x1F and mVibrateOn 0;
// the destructor stores cFEState_vtbl and runs cUIState__cUIState.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags:
//   [R] retail's constructors / destructor and allocation size;
//   [P] the July 2003 preview (layout_xlate: 0x44 -> 0x48, 0x4C -> 0x50, from onUpdate and the ctor);
//   [U] how retail code uses the field (the C bodies in src/fe/festateoptions.cpp and the asm);
//   [O] SSX On Tour's DWARF (class cFEStateOptionsController, cUIState, cListNode): names, types and order.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEStateOptionsController {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState_cUIState links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateOptionsController_vtbl (ctors), cFEState_vtbl (dtor); [P]
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [R] func_0018C658 stores 0x1F
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the engine argument; [U] onCreateScreen adds "22control" through
                                    //       it
    signed char mCurrentCtrl;       // 0x014 [O]
    unsigned char mCtrlBits;        // 0x015 [O]
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (word, see ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen; updateWidget /
                                    //       displayPadConfig look the lists and pictures up on it
    signed char mUserNum;           // 0x044 [O]
    char unk_045[0x3];              // 0x045 padding
    // --- cFEState (On Tour: size 0x48, no data)
    // --- cFEStateOptionsController
    int mTimer[2];                  // 0x048 [O][P] (preview 0x44); [U] onCreateScreen zeroes both; onWidgetEvent sets
                                    //       pad i's to 60 when its test buzz starts, onUpdate counts it down
    char mVibrateOn;                // 0x050 [O][P] (preview 0x4C); [R] ctors 0; [U] bit 0 / bit 1 = pad 0 / pad 1
                                    //       buzzing
                                    //       (onWidgetEvent sets, onUpdate clears)
    char unk_051[0x3];              // 0x051 padding
};

CHECK_OFFSET(cFEStateOptionsController, mpPrev, 0x000);
CHECK_OFFSET(cFEStateOptionsController, mpNext, 0x004);
CHECK_OFFSET(cFEStateOptionsController, mVtbl, 0x008);
CHECK_OFFSET(cFEStateOptionsController, mHashName, 0x00C);
CHECK_OFFSET(cFEStateOptionsController, mpEngine, 0x010);
CHECK_OFFSET(cFEStateOptionsController, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateOptionsController, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateOptionsController, unk_016, 0x016);
CHECK_OFFSET(cFEStateOptionsController, miMiscData, 0x018);
CHECK_OFFSET(cFEStateOptionsController, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateOptionsController, mpParentState, 0x020);
CHECK_OFFSET(cFEStateOptionsController, mScreens, 0x024);
CHECK_OFFSET(cFEStateOptionsController, mpScreen, 0x040);
CHECK_OFFSET(cFEStateOptionsController, mUserNum, 0x044);
CHECK_OFFSET(cFEStateOptionsController, unk_045, 0x045);
CHECK_OFFSET(cFEStateOptionsController, mTimer, 0x048);
CHECK_OFFSET(cFEStateOptionsController, mVibrateOn, 0x050);
CHECK_OFFSET(cFEStateOptionsController, unk_051, 0x051);
CHECK_SIZE(cFEStateOptionsController, 0x54);

// ---------------------------------------------------------------------------------------------------------------------
// cFEStateOptionsDeviceSelect: the memory-card device select screen (state id 33) for loading or saving the options /
// profile from the front end (and the online quick-messages popup): a cFEMemCard with its own save / online flags.
// EA's source is fe/festateoptions.cpp (the July 2003 preview's assert strings; builds/x/v2 file_hints for retail
// cFEStateOptionsDeviceSelect_onCreateScreen, lines 1878 ... 1903). Retail code: fe/festateoptions (the ctor
// cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect1 0x0018D7F0 ... displayStatus 0x0018E510) and the destructor /
// engine-less ctor copy in main/all_in_one_main (0x001DE150, 0x001DE178).
//
// Size 0x23C:
//   [R] cMemMan_alloc(572, "cFEStateOptionsDeviceSelect") + cFEStateOptionsDeviceSelect_cFEStateOptionsDeviceSelect1(mem,
//       engine, cardMode, isSave, isOnline) in cFECustom_getNextState (fe/fereal, four sites) and the quick-messages
//       popup (fe/feasyncfile); the ctor writes 0x22C ... 0x238.
//   [O] On Tour 0x168 = cFEMemCard 0x164 + four bools (mIsSaveScreen, mbOnline, mIsHidden, mbChoseToSave); retail's
//       bools are words: 0x22C ... 0x23C, in On Tour's order.
// Base: cFEMemCard (fe/fememcard.h, 0x22C) : cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8). The
// ctor runs cFEMemCard_cFEMemCard2(this, pEngine, cardMode), then stores cFEStateOptionsDeviceSelect_vtbl (0x0046C228,
// 47 slots: 1 dtor, 4 onCreateScreen, 19 onWidgetEvent, 20 onWidgetMenu, 26 displayStatus, 33 updateDone, 37
// hideActionButtons, 38 hideDeleteButton, 39 hideMainButton; the rest cFEMemCard's / cOVState's / cUIState's) and
// mHashName 33. The cListNode / cUIState / cOVState / cFEMemCard parts are written out inline with fe/fememcard.h's
// member names (see that header for the inherited fields' full evidence; only this class's own uses are added).
// Evidence tags: [R] retail's constructors / allocation size; [U] how retail code uses the field (this class's
// methods, asm and C in src/fe/festateoptions.cpp); [O] SSX On Tour's DWARF (class cFEStateOptionsDeviceSelect /
// cFEMemCard). (No preview evidence: layout_xlate finds no paired cFEStateOptionsDeviceSelect methods.)

struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cFEStateOptionsDeviceSelect {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateOptionsDeviceSelect_vtbl (both ctors, and the dtor first); [U]
                                    //       onWidgetEvent / displayStatus call slots 44 (confirmLoad), 47
                                    //       (displayStatusPopup)
    // --- cUIState (ui/uistate.h; same offsets as On Tour)
    int mHashName;                  // 0x00C [O]; [R] the ctor stores 33
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's pEngine; [U] onCreateScreen adds the screen; updateDone
                                    //       asks its cFECustom for the next state and pushes it on its stack (+0x18)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; lw/sw the word)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen / displayStatus / the hide* functions look widgets up
    signed char mUserNum;           // 0x044 [O]
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (bool); [R] the (inlined) cOVState ctor zeroes it
    int mAllowPauseMenu;            // 0x04C [O] (bool); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O] (bool)
    int mRemovePDA;                 // 0x054 [O] (bool); [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " into it (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] 0 (cFEMemCard_cFEMemCard2)
    // --- cFEMemCard (fe/fememcard.h, 0x22C)
    unsigned short mLastLoadFileName[64]; // 0x09C [O] ([32] in On Tour); [R] initFEMemCard clears the first char
    unsigned short mFileName[64];   // 0x11C [O] ([32] in On Tour); [R] initFEMemCard writes the "%s-%s*" file pattern
    int mPopupType;                 // 0x19C [O]
    int mProcessingKeyboard;        // 0x1A0 [O] (bool)
    cKeyboardPopup* mpKeyboard;     // 0x1A4 [O]
    int mIsProcessing;              // 0x1A8 [O] (bool); [U] onWidgetEvent tests it (busy) and sets it with a save /
                                    //       delete
    int mIsDone;                    // 0x1AC [O] (bool); [U] updateDone tests it
    int mIsLoadWait;                // 0x1B0 [O] (bool)
    int mIsLoadDoneOK;              // 0x1B4 [O] (bool); [U] updateDone tests it
    int mIsInvalidConfirmed;        // 0x1B8 [O] (bool)
    int mType;                      // 0x1BC [O]; [R] the ctor's cardMode (cFEMemCard_cFEMemCard2); [U] onCreateScreen /
                                    //       onWidgetEvent / displayStatus / updateDone test 1 / 2
    int mState;                     // 0x1C0 [O]; [U] onWidgetEvent sets 3 (saving), 4 (deleting), 6 (done)
    int mSlot;                      // 0x1C4 [O]; [U] onWidgetEvent stores the widget's slot (also the manager's
                                    //       mFileCursor)
    int mIsReplayAlloc;             // 0x1C8 [O] (bool)
    int mPlayerIdx;                 // 0x1CC [O]
    char* mpReadBuffer;             // 0x1D0 [O]
    char* mpStartReadBuffer;        // 0x1D4 [O]
    int mIsManagerStarted;          // 0x1D8 [O] (bool)
    int mIsSavePressed;             // 0x1DC [O] (bool); [U] onWidgetEvent sets it with a save
    int mIsDeletePressed;           // 0x1E0 [O] (bool)
    int mIsCancelPressed;           // 0x1E4 [O] (bool); [U] onWidgetEvent sets it (back); updateDone tests it
    int mIsFormatPressed;           // 0x1E8 [O] (bool)
    int mIsRetryPressed;            // 0x1EC [O] (bool)
    int mIsOverwriteOK;             // 0x1F0 [O] (bool)
    int mIsLoadPressed;             // 0x1F4 [O] (bool)
    int mDoNextMemCard;             // 0x1F8 [O] (bool)
    int mDoPrevMemCard;             // 0x1FC [O] (bool)
    int mLoadBEData;                // 0x200 [O] (bool)
    int mAutoLoadOn;                // 0x204 [O] (bool)
    int mAutoSaveOn;                // 0x208 [O] (bool)
    int mFirstCheck;                // 0x20C [O] (bool)
    int mIsStatusPopupOn;           // 0x210 [O] (bool)
    int mScreenStatus;              // 0x214 [O]; [U] onWidgetEvent acts only in display state 3; displayStatus tests it
    cFEPopupConfirm* mStatusPopup;  // 0x218 [O]
    int mLastSelectedDevice;        // 0x21C [O]; [U] displayStatus stores the device it shows
    int mDeleteButtonActive;        // 0x220 [O] (bool); [U] hideDeleteButton stores it
    int mMainButtonActive;          // 0x224 [O] (bool); [U] hideMainButton stores it; onWidgetEvent tests it
    int mBufferState;               // 0x228 [O] eBufferState
    // --- cFEStateOptionsDeviceSelect
    int mIsSaveScreen;              // 0x22C [O] (bool); [R] the ctor's isSave; [U] onCreateScreen / onWidgetEvent /
                                    //       onWidgetMenu / displayStatus / the hide* functions pick save or load by it
    int mbOnline;                   // 0x230 [O] (bool); [R] the ctor's isOnline; [U] updateDone asks for next-state
                                    //       choice 2 when it is set
    int mIsHidden;                  // 0x234 [O] (bool); [R] 0; [U] hideActionButtons compares and stores the new state
    int mbChoseToSave;              // 0x238 [O] (bool); [R] 0; [U] onWidgetEvent sets it on a slot pick (event 5),
                                    //       clears it on back (event 6)
};

CHECK_OFFSET(cFEStateOptionsDeviceSelect, mpPrev, 0x000);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mpNext, 0x004);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mVtbl, 0x008);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mHashName, 0x00C);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mpEngine, 0x010);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, unk_016, 0x016);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, miMiscData, 0x018);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mpParentState, 0x020);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mScreens, 0x024);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mpScreen, 0x040);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mUserNum, 0x044);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, unk_045, 0x045);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mDoneTransition, 0x048);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mRemovePDA, 0x054);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mszChatBuff, 0x058);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mOVID, 0x098);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mLastLoadFileName, 0x09C);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mFileName, 0x11C);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mPopupType, 0x19C);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mProcessingKeyboard, 0x1A0);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mpKeyboard, 0x1A4);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsProcessing, 0x1A8);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsDone, 0x1AC);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsLoadWait, 0x1B0);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsLoadDoneOK, 0x1B4);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsInvalidConfirmed, 0x1B8);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mType, 0x1BC);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mState, 0x1C0);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mSlot, 0x1C4);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsReplayAlloc, 0x1C8);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mPlayerIdx, 0x1CC);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mpReadBuffer, 0x1D0);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mpStartReadBuffer, 0x1D4);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsManagerStarted, 0x1D8);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsSavePressed, 0x1DC);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsDeletePressed, 0x1E0);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsCancelPressed, 0x1E4);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsFormatPressed, 0x1E8);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsRetryPressed, 0x1EC);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsOverwriteOK, 0x1F0);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsLoadPressed, 0x1F4);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mDoNextMemCard, 0x1F8);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mDoPrevMemCard, 0x1FC);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mLoadBEData, 0x200);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mAutoLoadOn, 0x204);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mAutoSaveOn, 0x208);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mFirstCheck, 0x20C);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsStatusPopupOn, 0x210);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mScreenStatus, 0x214);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mStatusPopup, 0x218);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mLastSelectedDevice, 0x21C);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mDeleteButtonActive, 0x220);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mMainButtonActive, 0x224);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mBufferState, 0x228);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsSaveScreen, 0x22C);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mbOnline, 0x230);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mIsHidden, 0x234);
CHECK_OFFSET(cFEStateOptionsDeviceSelect, mbChoseToSave, 0x238);
CHECK_SIZE(cFEStateOptionsDeviceSelect, 0x23C);

#endif /* FE_FESTATEOPTIONS_H */
