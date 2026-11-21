#ifndef FE_FESTATEPROFILE_H
#define FE_FESTATEPROFILE_H

// cFEStateProfileLoad: the front end's profile load / save / create screen ("93profile_load": six profile slot rows,
// the card panel, the name keyboard, save / delete), a cFEMemCard with mType 0. What it is opened for is the class
// static cFEStateProfileLoad::mScreenAction (cFEStateProfileLoad_mScreenAction, set by the ctor from iType), not a
// member.
// EA's source is fe/festateprofile.cpp (the July 2003 preview's assert strings; builds/x/v2 file_hints for retail
// cFEStateProfileLoad_onRender2D__FPv). Retail code: fe/festateprofile (0x0018ECA0 ... 0x00190768) and
// main/all_in_one_main (the destructor cFEStateProfileLoad__cFEStateProfileLoad, the engine-less ctor copy
// func_001DE240 and the setter func_001DE280).
//
// Size 0x280:
//   [R] cMemMan_alloc(640, "cFEStateProfileLoad") + cFEStateProfileLoad_cFEStateProfileLoad(mem, engine, iType,
//       ctrlMask) in cFECustom_getNextState (fe/fereal) and cFEStateCharSelect's handler (fe/festatecharselect); the
//       ctor writes 0x22C, 0x230, 0x234 and 0x27C (func_001DE280's word, "the last word of the object").
//   [O] On Tour 0x1B0 = cFEMemCard 0x164 + int miCntrlID, bool mIsOn, bool mIsHidden, unsigned short mTitle[32], int
//       mTypeOfQuit; retail: 0x22C + three words + 33 chars (0x42, kMCTitleStringSize as in cOVState_PROFILE,
//       fe/ovstateprofile.h) + 2 bytes of padding + a word = 0x280.
// Base: cFEMemCard (fe/fememcard.h, 0x22C) : cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8).
// The ctor runs cFEMemCard_cFEMemCard2(this, pEngine, 0) and stores cFEStateProfileLoad_vtbl (D_0046BFD0, 47 slots: 1
// dtor, 4 onCreateScreen, 5 onDestroyScreen__FPv, 15 onRender2D__FPv, 19 onWidgetEvent, 20 onWidgetMenu, 26
// displayStatus, 33 updateDone, 37 hideActionButtons, 38 hideDeleteButton, 39 hideMainButton; the rest cFEMemCard's).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. The base classes
// cListNode / cUIState / cOVState / cFEMemCard are written out inline with fe/fememcard.h's member names (see that
// header for the full evidence of the inherited fields; only this class's own uses are added here).
// Evidence tags:
//   [R] retail's constructors and allocation size;
//   [U] how retail code uses the field (this class's methods, asm and C);
//   [O] SSX On Tour's DWARF (class cFEStateProfileLoad / cFEMemCard): member names, types and order. On Tour's bools
//       are 1 byte (retail: words), its cOVState is 0x90 (retail 0x9C) and its cFEMemCard 0x164 (retail 0x22C).
// (No preview evidence: layout_xlate finds no paired cFEStateProfileLoad methods.)

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cKeyboardPopup;
struct cFEPopupConfirm;

struct cFEStateProfileLoad {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateProfileLoad_vtbl, stored over cFEMemCard_vtbl by both ctors;
                                    //       the dtor stores it first
    // --- cUIState (ui/uistate.h; same offsets as On Tour)
    int mHashName;                  // 0x00C [O]; [R] the ctor stores 35 (0x23)
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's pEngine; [U] onCreateScreen adds "93profile_load";
                                    //       onWidgetEvent builds the cKeyboardPopup for it
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the ctor stores its ctrlMask argument (sb)
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; lw/sw the word)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] every widget lookup (cUIScreen_getObjectByHashName)
    signed char mUserNum;           // 0x044 [O]; [R] the ctor stores (signed char)mPlayerIdx (sb)
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
    int mPopupType;                 // 0x19C [O]; [U] func_0018FC98 sets 22 before the status popup; onWidgetEvent
    int mProcessingKeyboard;        // 0x1A0 [O] (bool); [U] onWidgetEvent sets it when it pushes the name keyboard
    cKeyboardPopup* mpKeyboard;     // 0x1A4 [O]; [U] onWidgetEvent creates the cKeyboardPopup once and reads its
                                    //       text; onDestroyScreen clears it
    int mIsProcessing;              // 0x1A8 [O] (bool); [U] func_0018FC98 sets it with the save; onWidgetEvent tests it
    int mIsDone;                    // 0x1AC [O] (bool); [U] updateDone tests and sets it
    int mIsLoadWait;                // 0x1B0 [O] (bool)
    int mIsLoadDoneOK;              // 0x1B4 [O] (bool)
    int mIsInvalidConfirmed;        // 0x1B8 [O] (bool); [R] initFEMemCard clears it
    int mType;                      // 0x1BC [O]; [R] 0 (profile: cFEMemCard_cFEMemCard2(self, pEngine, 0)); [U]
                                    //       updateDone reads it
    int mState;                     // 0x1C0 [O]; [U] func_0018FC98 sets 3 (saving); onWidgetEvent tests it
    int mSlot;                      // 0x1C4 [O]; [U] onWidgetEvent stores the picked row
    int mIsReplayAlloc;             // 0x1C8 [O] (bool)
    int mPlayerIdx;                 // 0x1CC [O]; [R] the ctor clears it, then stores the player (0/1) using the
                                    //       opening controller and copies its low byte (lb) to mUserNum; [U]
                                    //       func_0018FC98 passes it to cMCOverlayManager_writeSetFileType
    char* mpReadBuffer;             // 0x1D0 [O]
    char* mpStartReadBuffer;        // 0x1D4 [O]
    int mIsManagerStarted;          // 0x1D8 [O] (bool)
    int mIsSavePressed;             // 0x1DC [O] (bool); [U] func_0018FC98 sets it with the save
    int mIsDeletePressed;           // 0x1E0 [O] (bool)
    int mIsCancelPressed;           // 0x1E4 [O] (bool); [U] onWidgetEvent sets it (back)
    int mIsFormatPressed;           // 0x1E8 [O] (bool)
    int mIsRetryPressed;            // 0x1EC [O] (bool)
    int mIsOverwriteOK;             // 0x1F0 [O] (bool)
    int mIsLoadPressed;             // 0x1F4 [O] (bool)
    int mDoNextMemCard;             // 0x1F8 [O] (bool)
    int mDoPrevMemCard;             // 0x1FC [O] (bool)
    int mLoadBEData;                // 0x200 [O] (bool)
    int mAutoLoadOn;                // 0x204 [O] (bool); [U] func_0018FC98 skips the status popup when set
    int mAutoSaveOn;                // 0x208 [O] (bool)
    int mFirstCheck;                // 0x20C [O] (bool)
    int mIsStatusPopupOn;           // 0x210 [O] (bool); [U] func_0018FC98 tests and sets it
    int mScreenStatus;              // 0x214 [O]; [U] displayStatus / displayOn / onWidgetEvent test it
    cFEPopupConfirm* mStatusPopup;  // 0x218 [O]
    int mLastSelectedDevice;        // 0x21C [O]; [U] displayOn stores the device it shows
    int mDeleteButtonActive;        // 0x220 [O] (bool); [U] hideDeleteButton and the init*Screen functions store it
    int mMainButtonActive;          // 0x224 [O] (bool); [U] hideMainButton and the init*Screen functions store it;
                                    //       func_0018FC98 saves only while it is set
    int mBufferState;               // 0x228 [O] eBufferState
    // --- cFEStateProfileLoad
    int miCntrlID;                  // 0x22C [O]; [R] the index of the lowest set bit of the ctor's ctrlMask (of the
                                    //       first three); compared with cBENewPlayerInterface_getPlayerCtrlID
    int mIsOn;                      // 0x230 [O] (bool); [R] the ctor clears it (no other retail access)
    int mIsHidden;                  // 0x234 [O] (bool); [R] the ctor clears it; [U] hideActionButtons compares and
                                    //       stores the new hidden state
    unsigned short mTitle[33];      // 0x238 [O] ([32] in On Tour; 33 chars as cOVState_PROFILE::mTitle); [U] displayOn
                                    //       has the card (manager +0x434, slot 14) write each slot's title here and
                                    //       shows it (cUIText_setUnicodeString)
    char unk_27A[0x2];              // 0x27A padding
    int mTypeOfQuit;                // 0x27C [O]; [R] the ctor clears it; [U] the setter func_001DE280 (0, 1 or 2);
                                    //       updateDone: 0 pushes the parent factory's next state, 1 starts a pre-game
                                    //       load, 2 returns to the title screen
};

CHECK_OFFSET(cFEStateProfileLoad, mpPrev, 0x000);
CHECK_OFFSET(cFEStateProfileLoad, mpNext, 0x004);
CHECK_OFFSET(cFEStateProfileLoad, mVtbl, 0x008);
CHECK_OFFSET(cFEStateProfileLoad, mHashName, 0x00C);
CHECK_OFFSET(cFEStateProfileLoad, mpEngine, 0x010);
CHECK_OFFSET(cFEStateProfileLoad, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateProfileLoad, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateProfileLoad, unk_016, 0x016);
CHECK_OFFSET(cFEStateProfileLoad, miMiscData, 0x018);
CHECK_OFFSET(cFEStateProfileLoad, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateProfileLoad, mpParentState, 0x020);
CHECK_OFFSET(cFEStateProfileLoad, mScreens, 0x024);
CHECK_OFFSET(cFEStateProfileLoad, mpScreen, 0x040);
CHECK_OFFSET(cFEStateProfileLoad, mUserNum, 0x044);
CHECK_OFFSET(cFEStateProfileLoad, unk_045, 0x045);
CHECK_OFFSET(cFEStateProfileLoad, mDoneTransition, 0x048);
CHECK_OFFSET(cFEStateProfileLoad, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cFEStateProfileLoad, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cFEStateProfileLoad, mRemovePDA, 0x054);
CHECK_OFFSET(cFEStateProfileLoad, mszChatBuff, 0x058);
CHECK_OFFSET(cFEStateProfileLoad, mOVID, 0x098);
CHECK_OFFSET(cFEStateProfileLoad, mLastLoadFileName, 0x09C);
CHECK_OFFSET(cFEStateProfileLoad, mFileName, 0x11C);
CHECK_OFFSET(cFEStateProfileLoad, mPopupType, 0x19C);
CHECK_OFFSET(cFEStateProfileLoad, mProcessingKeyboard, 0x1A0);
CHECK_OFFSET(cFEStateProfileLoad, mpKeyboard, 0x1A4);
CHECK_OFFSET(cFEStateProfileLoad, mIsProcessing, 0x1A8);
CHECK_OFFSET(cFEStateProfileLoad, mIsDone, 0x1AC);
CHECK_OFFSET(cFEStateProfileLoad, mIsLoadWait, 0x1B0);
CHECK_OFFSET(cFEStateProfileLoad, mIsLoadDoneOK, 0x1B4);
CHECK_OFFSET(cFEStateProfileLoad, mIsInvalidConfirmed, 0x1B8);
CHECK_OFFSET(cFEStateProfileLoad, mType, 0x1BC);
CHECK_OFFSET(cFEStateProfileLoad, mState, 0x1C0);
CHECK_OFFSET(cFEStateProfileLoad, mSlot, 0x1C4);
CHECK_OFFSET(cFEStateProfileLoad, mIsReplayAlloc, 0x1C8);
CHECK_OFFSET(cFEStateProfileLoad, mPlayerIdx, 0x1CC);
CHECK_OFFSET(cFEStateProfileLoad, mpReadBuffer, 0x1D0);
CHECK_OFFSET(cFEStateProfileLoad, mpStartReadBuffer, 0x1D4);
CHECK_OFFSET(cFEStateProfileLoad, mIsManagerStarted, 0x1D8);
CHECK_OFFSET(cFEStateProfileLoad, mIsSavePressed, 0x1DC);
CHECK_OFFSET(cFEStateProfileLoad, mIsDeletePressed, 0x1E0);
CHECK_OFFSET(cFEStateProfileLoad, mIsCancelPressed, 0x1E4);
CHECK_OFFSET(cFEStateProfileLoad, mIsFormatPressed, 0x1E8);
CHECK_OFFSET(cFEStateProfileLoad, mIsRetryPressed, 0x1EC);
CHECK_OFFSET(cFEStateProfileLoad, mIsOverwriteOK, 0x1F0);
CHECK_OFFSET(cFEStateProfileLoad, mIsLoadPressed, 0x1F4);
CHECK_OFFSET(cFEStateProfileLoad, mDoNextMemCard, 0x1F8);
CHECK_OFFSET(cFEStateProfileLoad, mDoPrevMemCard, 0x1FC);
CHECK_OFFSET(cFEStateProfileLoad, mLoadBEData, 0x200);
CHECK_OFFSET(cFEStateProfileLoad, mAutoLoadOn, 0x204);
CHECK_OFFSET(cFEStateProfileLoad, mAutoSaveOn, 0x208);
CHECK_OFFSET(cFEStateProfileLoad, mFirstCheck, 0x20C);
CHECK_OFFSET(cFEStateProfileLoad, mIsStatusPopupOn, 0x210);
CHECK_OFFSET(cFEStateProfileLoad, mScreenStatus, 0x214);
CHECK_OFFSET(cFEStateProfileLoad, mStatusPopup, 0x218);
CHECK_OFFSET(cFEStateProfileLoad, mLastSelectedDevice, 0x21C);
CHECK_OFFSET(cFEStateProfileLoad, mDeleteButtonActive, 0x220);
CHECK_OFFSET(cFEStateProfileLoad, mMainButtonActive, 0x224);
CHECK_OFFSET(cFEStateProfileLoad, mBufferState, 0x228);
CHECK_OFFSET(cFEStateProfileLoad, miCntrlID, 0x22C);
CHECK_OFFSET(cFEStateProfileLoad, mIsOn, 0x230);
CHECK_OFFSET(cFEStateProfileLoad, mIsHidden, 0x234);
CHECK_OFFSET(cFEStateProfileLoad, mTitle, 0x238);
CHECK_OFFSET(cFEStateProfileLoad, unk_27A, 0x27A);
CHECK_OFFSET(cFEStateProfileLoad, mTypeOfQuit, 0x27C);
CHECK_SIZE(cFEStateProfileLoad, 0x280);


// ---------------------------------------------------------------------------------------------------------------------
// cFEStateProfileSelect: the front end's profile menu ("05sel_prof", state id 8): each entry's misc data goes to the
// custom interface (cFECustom, the engine's first word) for the next / previous state (slots 4 / 5); the entry under
// the cursor turns yellow (events 1 / 2 / 5 recolour it). Retail never constructs it (no caller of either ctor).
// EA's source: fe/festateprofile.cpp (see above). Retail code: the engine ctor
// cFEStateProfileSelect_cFEStateProfileSelect1 (splat puts it at the end of fe/festateoptions, 0x0018EA98),
// onCreateScreen / onWidgetEvent in fe/festateprofile (0x0018EAD8, 0x0018EB40), and the engine-less ctor and the dtor
// in main/all_in_one_main (0x001DE1B8, 0x001DE1F0).
//
// Size 0x48: [O] On Tour's class is 0x48 (no members); [R] the ctors write nothing past the cUIState part.
// Base: cFEState (fe/festate.h, no data) : cUIState (ui/uistate.h, 0x48) : cListNode (vptr at 0x8). The engine ctor
// runs cUIState_cUIState(this, pEngine) (its C definition declares only this), stores cFEStateProfileSelect_vtbl
// (0x0046C158, 24 slots: 1 dtor, 4 onCreateScreen, 6 cFEState_onGainFocus, 11 cFEState_onLoseFocus, 19 onWidgetEvent)
// and mHashName 8; the dtor stores cFEState_vtbl and runs cUIState__cUIState.
// A FLAT layout, like main/game.h. Evidence tags: [R] retail's ctors / dtor; [U] how retail code uses the field;
// [O] SSX On Tour's DWARF (class cFEStateProfileSelect, cUIState): names, types and order.

struct cFEStateProfileSelect {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cFEStateProfileSelect_vtbl (ctors), cFEState_vtbl (dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [R] the engine ctor stores 8
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] onCreateScreen adds "05sel_prof"
                                    //       through it; onWidgetEvent asks its custom interface (+0x0) for the next /
                                    //       previous state and pushes it on its stack (+0x18)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes; every accessor lw/sw's the word, see
                                    //       ui/uistate.h)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen stores the screen
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
};

CHECK_OFFSET(cFEStateProfileSelect, mpPrev, 0x000);
CHECK_OFFSET(cFEStateProfileSelect, mpNext, 0x004);
CHECK_OFFSET(cFEStateProfileSelect, mVtbl, 0x008);
CHECK_OFFSET(cFEStateProfileSelect, mHashName, 0x00C);
CHECK_OFFSET(cFEStateProfileSelect, mpEngine, 0x010);
CHECK_OFFSET(cFEStateProfileSelect, mCurrentCtrl, 0x014);
CHECK_OFFSET(cFEStateProfileSelect, mCtrlBits, 0x015);
CHECK_OFFSET(cFEStateProfileSelect, unk_016, 0x016);
CHECK_OFFSET(cFEStateProfileSelect, miMiscData, 0x018);
CHECK_OFFSET(cFEStateProfileSelect, mAttrState, 0x01C);
CHECK_OFFSET(cFEStateProfileSelect, mpParentState, 0x020);
CHECK_OFFSET(cFEStateProfileSelect, mScreens, 0x024);
CHECK_OFFSET(cFEStateProfileSelect, mpScreen, 0x040);
CHECK_OFFSET(cFEStateProfileSelect, mUserNum, 0x044);
CHECK_OFFSET(cFEStateProfileSelect, unk_045, 0x045);
CHECK_SIZE(cFEStateProfileSelect, 0x48);

#endif /* FE_FESTATEPROFILE_H */
