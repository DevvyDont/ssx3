#ifndef FE_OVSTATEBIGCHALLENGE_H
#define FE_OVSTATEBIGCHALLENGE_H

// The Big Challenge overlays (retail code in the decomp's unit fe/ovstateenterlodge.cpp; constructors/destructors in
// fe/all_in_one_fe.cpp; EA's source file is not known: the preview's linkonce sections name the classes
// (__27cOVState_BIGCHALLENGE_STARTP9cUIEngine12tOVStateEnum, _$_26cOVState_BIGCHALLENGE_FAIL) but no path).
// Both derive from cOVState (fe/ovstate.h, 0x9C), written out flat below with the same member names.
//
// cOVState_BIGCHALLENGE_START: the challenge intro (overlay id 29). Allocated 0xA0 by cOVStateManager_triggerOverlay /
// triggerPopup, which then call cOVState_BIGCHALLENGE_START_getChallengeData. Vtable
// cOVState_BIGCHALLENGE_START_vtbl (0x473688); constructors cOVState_BIGCHALLENGE_START_cOVState_BIGCHALLENGE_START
// (engine, id; calls getChallengeData) and func_00220F48 (no engine); destructor
// cOVState_BIGCHALLENGE_START__cOVState_BIGCHALLENGE_START. On Tour: size 0x94, one member.
//
// cOVState_BIGCHALLENGE_FAIL: the challenge-failed screen (overlay id 30). Allocated 0xA4 (+ getChallengeData).
// Vtable D_004735B0; constructors func_002210A0 (engine, id; calls getChallengeData) and func_00221040 (no engine);
// destructor cOVState_BIGCHALLENGE_FAIL__cOVState_BIGCHALLENGE_FAIL. On Tour: size 0x98, mMissionId + mRider.
//
// cOVState_BIGCHALLENGE_PASS is On Tour only: retail has no vtable, constructor or overlay id for it (the preview
// has BIGCHALLENGE_PREVIEW / CONFIRM / LIST instead, which retail dropped too).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstateenterlodge.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_BIGCHALLENGE_START {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVState_BIGCHALLENGE_START_vtbl, stored after cOVState_vtbl by the
                                    //       constructors
    // --- cUIState (On Tour: size 0x48, same offsets; see ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U]
                                    //       cUIEngine_addScreenByHashName(mpEngine, ...)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] lw/sw the word; [R] triggerOverlay
                                    //       sets bits 0x3F00 to 0x100
    cUIState* mpParentState;        // 0x020 [O]; [R] 0 (1-argument cUIState ctor)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens; [R] constructed by cList_cList(self + 0x24)
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the screen; widget
                                    //       lookups go through it
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (D_004A2088, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (29)
    // --- cOVState_BIGCHALLENGE_START (On Tour: size 0x94)
    int mMissionId;                 // 0x09C [O] (On Tour unsigned int); [U] getChallengeData stores the next
                                    //       mission-queue entry's id (or cWScriptMan's current mission, +0x2A0);
                                    //       onGainTransition / onWidgetEvent read it
};

CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mpPrev, 0x0);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mpNext, 0x4);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mVtbl, 0x8);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mHashName, 0xC);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mpEngine, 0x10);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, unk_016, 0x16);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, miMiscData, 0x18);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mpParentState, 0x20);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mScreens, 0x24);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mpScreen, 0x40);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mUserNum, 0x44);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, unk_045, 0x45);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mOVID, 0x98);
CHECK_OFFSET(cOVState_BIGCHALLENGE_START, mMissionId, 0x9C);
CHECK_SIZE(cOVState_BIGCHALLENGE_START, 0xA0);

struct cOVState_BIGCHALLENGE_FAIL {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_004735B0, stored after cOVState_vtbl by the constructors
    // --- cUIState (On Tour: size 0x48, same offsets; see ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor; [U]
                                    //       onCreateScreen stores the screen's hash
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U]
                                    //       cUIEngine_addScreenByHashName(mpEngine, ...)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [P] lw/sw the word; [R] triggerOverlay
                                    //       sets bits 0x3F00 to 0x100
    cUIState* mpParentState;        // 0x020 [O]; [R] 0 (1-argument cUIState ctor)
    char mScreens[0x1C];            // 0x024 [O] cList mScreens; [R] constructed by cList_cList(self + 0x24)
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the screen; widget
                                    //       lookups go through it
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (D_004A2088, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (30)
    // --- cOVState_BIGCHALLENGE_FAIL (On Tour: size 0x98)
    int mMissionId;                 // 0x09C [O]; [U] getChallengeData stores the next mission-queue entry's id;
                                    //       onWidgetEvent reads it
    int mRider;                     // 0x0A0 [O]; [R] placed by the allocation size (0xA4) and On Tour's order only:
                                    //       no retail code reads or writes it
};

CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mpPrev, 0x0);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mpNext, 0x4);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mVtbl, 0x8);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mHashName, 0xC);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mpEngine, 0x10);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, unk_016, 0x16);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, miMiscData, 0x18);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mpParentState, 0x20);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mScreens, 0x24);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mpScreen, 0x40);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mUserNum, 0x44);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, unk_045, 0x45);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mOVID, 0x98);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mMissionId, 0x9C);
CHECK_OFFSET(cOVState_BIGCHALLENGE_FAIL, mRider, 0xA0);
CHECK_SIZE(cOVState_BIGCHALLENGE_FAIL, 0xA4);

#endif /* FE_OVSTATEBIGCHALLENGE_H */
