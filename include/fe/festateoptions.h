#ifndef FE_FESTATEOPTIONS_H
#define FE_FESTATEOPTIONS_H

// This header: cFEStateOptionsController, cFEStateOptionsDeviceSelect, cFEStateOptions and cFEStateOptionsSaveLoad
// (the last two below).
//
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
// dtor, 4 onCreateScreen, 5 onDestroyScreen, 11 onLoseFocus cFEStateOptionsController_onLoseFocus, 12 onUpdate, 17 onPopupEvent, 19
// onWidgetEvent, 20 onWidgetMenu, 21 onWidgetPair; vtables.tsv stops at slot 10), mHashName 0x1F and mVibrateOn 0;
// the destructor stores cFEState_vtbl and runs cUIState__cUIState.
//
// Derives from cUIState (ui/uistate.h); its own members at their retail offsets.
// Evidence tags:
//   [R] retail's constructors / destructor and allocation size;
//   [P] the July 2003 preview (layout_xlate: 0x44 -> 0x48, 0x4C -> 0x50, from onUpdate and the ctor);
//   [U] how retail code uses the field (the C bodies in src/fe/festateoptions.cpp and the asm);
//   [O] SSX On Tour's DWARF (class cFEStateOptionsController, cUIState, cListNode): names, types and order.

#include "layout.h"
#include "ui/uistate.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cFEStateOptionsController : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateOptionsController_vtbl (ctors), cFEState_vtbl (dtor); [P]
    // cFEStateOptionsController's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "22control" through it
    //   mpScreen: [U] onCreateScreen stores the screen; updateWidget / displayPadConfig look the lists and pictures up
    //       on it
    // --- cFEState (On Tour: size 0x48, no data)
    // --- cFEStateOptionsController
    int mTimer[2];                  // 0x048 [O][P] (preview 0x44); [U] onCreateScreen zeroes both; onWidgetEvent sets
                                    //       pad i's to 60 when its test buzz starts, onUpdate counts it down
    char mVibrateOn;                // 0x050 [O][P] (preview 0x4C); [R] ctors 0; [U] bit 0 / bit 1 = pad 0 / pad 1
                                    //       buzzing
                                    //       (onWidgetEvent sets, onUpdate clears)
    char unk_051[0x3];              // 0x051 padding
};

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

struct cFEStateOptionsDeviceSelect : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateOptionsDeviceSelect_vtbl (both ctors, and the dtor first); [U] onWidgetEvent / displayStatus call
    //       slots 44 (confirmLoad), 47 (displayStatusPopup)
    // cFEStateOptionsDeviceSelect's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds the screen; updateDone asks its cFECustom for the next state and pushes it
    //       on its stack (+0x18)
    //   mpScreen: [U] onCreateScreen / displayStatus / the hide* functions look widgets up
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

// ---------------------------------------------------------------------------------------------------------------------
// cFEStateOptions: the front end's options menu ("18options", state id 0x18): entries "1" ... "8" open the game, sound,
// HUD, controller, save/load, network-name keyboard and device-select states; leaving with unsaved changes first asks
// "save options?" (saveOptionPrompt pushes a cFEPopupOptionReset). EA's source: fe/festateoptions.cpp (see above).
// Retail code: the engine ctor func_001887A0 (splat puts it at the end of fe/festatelegal),
// cFEStateOptions_onCreateScreen ... cFEStateOptions_saveOptionPrompt in fe/festateoptions (0x00188800 ...
// 0x00188E58), and the engine-less ctor and the dtor in main/all_in_one_main (0x001DDEE8, 0x001DDF20).
//
// Size 0x54:
//   [R] cMemMan_alloc(84, "cFEStateOptions" / "FEOptions") + func_001887A0(mem, engine, reset) in
//       cFECustom_getNextState (fe/fereal), fe/festatecharsetup and fe/ovstates; the last member is the pointer at
//       0x50.
//   [O] On Tour's class is 0x50: two bools (0x48, 0x49) then mpKeyboard (0x4C); retail's bools are words.
// Base: cFEState (no data; vtable cFEState_vtbl) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor
// func_001887A0 runs cUIState_cUIState(this, pEngine), stores mHashName 0x18 and cFEStateOptions_vtbl (0x0046C7C0,
// 24 slots: 1 dtor, 4 onCreateScreen, 8 onInputBegin, 17 onPopupEvent, 18 onWidgetCreate, 19 onWidgetEvent,
// 20 onWidgetMenu; the rest cFEState's / cUIState's) and, when reset is set, clears flow-state entry 0x18; the dtor
// stores cFEState_vtbl and runs cUIState__cUIState.
// Evidence tags: [R] retail's constructors / destructor and allocation size; [U] how retail code uses the field (the C
// bodies in src/fe/festateoptions.cpp and the asm); [O] SSX On Tour's DWARF (class cFEStateOptions, cUIState): names,
// types and order. (No preview evidence: layout_xlate pairs only the dtor, which touches the vptr.)

struct cFEStateOptions : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateOptions_vtbl (both ctors), cFEState_vtbl (dtor); [U] onPopupEvent calls slot 19 (onWidgetEvent)
    //       with entry "8"
    // cFEStateOptions's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "18options" through it; onWidgetEvent / saveOptionPrompt push states on its
    //       stack (+0x18)
    //   mCtrlBits: [U] onWidgetEvent passes it (lbu) to the cKeyboardPopup ctor
    //   mAttrState: [U] onWidgetEvent ORs in 0x80 (pending delete) when the menu is left
    //   mpScreen: [U] onCreateScreen stores the screen; onWidgetEvent plays its "TransitionOut", onPopupEvent looks
    //       entry "8" up on it
    // --- cFEState (On Tour: size 0x48, no data)
    // --- cFEStateOptions
    int mbToSaveOptionScreen;       // 0x048 [O] (On Tour: bool at 0x48; a word in retail); [U] onCreateScreen 0;
                                    //       onPopupEvent stores the save-options answer (1 yes, 0 no); onWidgetEvent
                                    //       (entry "8") then sets the custom interface's +0xC to 0x18 when it is set
    int mbLetFrameGo;               // 0x04C [O] (On Tour: bool at 0x49; a word in retail); [U] onCreateScreen 1,
                                    //       saveOptionPrompt 0, onPopupEvent 1 once answered; onWidgetMenu holds keys
                                    //       (returns 1) while it is 0
    cKeyboardPopup* mpKeyboard;     // 0x050 [O] (On Tour 0x4C); [U] onWidgetEvent: entry "6" allocates the
                                    //       cKeyboardPopup (0x444, "cKeyboardPopup") into it and pushes it as a popup;
                                    //       event 13 (keyboard done) reads its text (+0x74) and clears it
};

CHECK_OFFSET(cFEStateOptions, mbToSaveOptionScreen, 0x048);
CHECK_OFFSET(cFEStateOptions, mbLetFrameGo, 0x04C);
CHECK_OFFSET(cFEStateOptions, mpKeyboard, 0x050);
CHECK_SIZE(cFEStateOptions, 0x54);

// ---------------------------------------------------------------------------------------------------------------------
// cFEStateOptionsSaveLoad: the options' save/load menu ("25saveload", state id 0x20): save options, load options, load
// replay, and "new game" (a cFEPopupConfirm, kT_HELPCreateNewFilePrompt; yes resets the profile and both front-end
// riders). EA's source: fe/festateoptions.cpp (see above). Retail code: fe/festateoptions (the engine ctor
// cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad1 0x0018D240 ... cFEStateOptionsSaveLoad_onWidgetEvent 0x0018D648) and
// the engine-less ctor and the dtor in main/all_in_one_main (0x001DE0F0, 0x001DE128).
//
// Size 0x4C:
//   [R] cMemMan_alloc(76, "cFEStateOptionsSaveLoad") + cFEStateOptionsSaveLoad_cFEStateOptionsSaveLoad1(mem, engine) in
//       cFECustom_getNextState / cFECustom_getPrevState (fe/fereal, three sites); the ctor's last store is the byte
//       at 0x48.
//   [O] On Tour's class is 0x4C too, with its one member (char mCurrentCtrl) at 0x48.
// Base: cFEState (no data) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The ctor runs
// cUIState_cUIState(this, pEngine), stores mHashName 0x20, cFEStateOptionsSaveLoad_vtbl (0x0046C3B0, 24 slots: 1 dtor,
// 4 onCreateScreen, 14 onPreRender2D, 17 onPopupEvent, 18 onWidgetCreate, 19 onWidgetEvent, 20 onWidgetMenu) and
// clears the byte at 0x48; the dtor stores cFEState_vtbl and runs cUIState__cUIState.
// Evidence tags as for cFEStateOptions (no preview evidence: layout_xlate pairs only the ctor's vptr store).

struct cFEStateOptionsSaveLoad : public cUIState {
    // cUIState (ui/uistate.h, 0x48): the vptr at 0x008:
    //   [R] cFEStateOptionsSaveLoad_vtbl (both ctors), cFEState_vtbl (dtor)
    // cFEStateOptionsSaveLoad's notes on cUIState's fields:
    //   mpEngine: [U] onCreateScreen adds "25saveload"; onWidgetEvent / onPopupEvent push states and popups on its
    //       stack (+0x18) and call its custom interface (+0x0) slots 4 / 5
    //   mCurrentCtrl: [U] onWidgetEvent copies it into mSaveLoadCurrentCtrl and adds (1 << it) to the next-state id
    //   mpScreen: [U] onCreateScreen stores the screen; onPreRender2D looks entries up on it; onPopupEvent plays its
    //       "TransitionOut"
    //   mUserNum: [U] onPopupEvent: the profile to reset (lb; at least 0)
    // --- cFEState (On Tour: size 0x48, no data)
    // --- cFEStateOptionsSaveLoad
    signed char mSaveLoadCurrentCtrl; // 0x048 [O] cFEStateOptionsSaveLoad::mCurrentCtrl (char; renamed here because
                                    //       the flat struct already has cUIState::mCurrentCtrl at 0x14); [R] ctor
                                    //       sb 0; [U] onWidgetEvent stores mCurrentCtrl before the "new game"
                                    //       popup; onPopupEvent's yes pushes the state for (1 << it) | 0x100 (lb)
    char unk_049[0x3];              // 0x049 padding
};

CHECK_OFFSET(cFEStateOptionsSaveLoad, mSaveLoadCurrentCtrl, 0x048);
CHECK_OFFSET(cFEStateOptionsSaveLoad, unk_049, 0x049);
CHECK_SIZE(cFEStateOptionsSaveLoad, 0x4C);

#endif /* FE_FESTATEOPTIONS_H */
