#ifndef FE_OVSTATETOPTIMES_H
#define FE_OVSTATETOPTIMES_H

// cOVState_TOPTIMES: the post-event top-times / top-scores overlay (screen "61toptimes"; Cross continues or saves).
// Retail code in the decomp's unit fe/ovstateenterlodge.cpp, constructor/destructor in fe/all_in_one_fe.cpp;
// EA's source file is not known (the preview's linkonce sections name the class: __17cOVState_TOPTIMESP9...).
// Size 0xA4: cMemMan_alloc(0xA4, "cOVStateOverlay") for overlay id 15; On Tour 0x98 = 0x90 + an int and a
// 1-byte bool, i.e. two words here. Vtable cOVState_TOPTIMES_vtbl (0x4734D0, 26 slots: slot 25 is the class's own
// virtual handleNext); constructor cOVState_TOPTIMES_cOVState_TOPTIMES, destructor
// cOVState_TOPTIMES__cOVState_TOPTIMES.
// Base cOVState (fe/ovstate.h, 0x9C), written out flat below with the same member names.
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

struct cOVState_TOPTIMES {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVState_TOPTIMES_vtbl, stored after cOVState_vtbl by the
                                    //       constructor; [U] onWidgetEvent calls slot 25 (handleNext) through it
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
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the "61toptimes"
                                    //       screen; widget lookups go through it
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (D_004A2088, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (15)
    // --- cOVState_TOPTIMES (On Tour: size 0x98)
    int mTopTimesFlow;              // 0x09C [O]; [R] placed by the allocation size (0xA4) and On Tour's order only:
                                    //       no retail code reads or writes it
    int mIsNextScreenPending;       // 0x0A0 [O] bool; [U] onCreateScreen clears it; handleNext acts only while it is
                                    //       clear and sets it (continue: cOVStateManager_mStatus = 1; save: overlay
                                    //       19)
};

CHECK_OFFSET(cOVState_TOPTIMES, mpPrev, 0x0);
CHECK_OFFSET(cOVState_TOPTIMES, mpNext, 0x4);
CHECK_OFFSET(cOVState_TOPTIMES, mVtbl, 0x8);
CHECK_OFFSET(cOVState_TOPTIMES, mHashName, 0xC);
CHECK_OFFSET(cOVState_TOPTIMES, mpEngine, 0x10);
CHECK_OFFSET(cOVState_TOPTIMES, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_TOPTIMES, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_TOPTIMES, unk_016, 0x16);
CHECK_OFFSET(cOVState_TOPTIMES, miMiscData, 0x18);
CHECK_OFFSET(cOVState_TOPTIMES, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_TOPTIMES, mpParentState, 0x20);
CHECK_OFFSET(cOVState_TOPTIMES, mScreens, 0x24);
CHECK_OFFSET(cOVState_TOPTIMES, mpScreen, 0x40);
CHECK_OFFSET(cOVState_TOPTIMES, mUserNum, 0x44);
CHECK_OFFSET(cOVState_TOPTIMES, unk_045, 0x45);
CHECK_OFFSET(cOVState_TOPTIMES, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_TOPTIMES, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_TOPTIMES, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_TOPTIMES, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_TOPTIMES, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_TOPTIMES, mOVID, 0x98);
CHECK_OFFSET(cOVState_TOPTIMES, mTopTimesFlow, 0x9C);
CHECK_OFFSET(cOVState_TOPTIMES, mIsNextScreenPending, 0xA0);
CHECK_SIZE(cOVState_TOPTIMES, 0xA4);

#endif /* FE_OVSTATETOPTIMES_H */
