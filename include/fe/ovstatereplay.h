#ifndef FE_OVSTATEREPLAY_H
#define FE_OVSTATEREPLAY_H

// cOVState_REPLAY: the replay viewer overlay (screen "64replay": the timeline panel sliding at the bottom, the
// camera name, the progress ticker and the Start menu Save / Exit / Continue). EA's source
// d:/ssx/code/source/fe/ovstatereplay.cpp (preview assert strings); retail code in the decomp's units
// fe/ovstatereplay.cpp and fe/ovtemplatedialog.cpp (onCreateScreen), constructors in fe/all_in_one_fe.cpp.
// Size 0xB8: cMemMan_alloc(0xB8, "cOVStateOverlay") for overlay id 17; On Tour 0xA8 = 0x90 + its 7 members with
// 1-byte bools, i.e. 7 words here. Vtable cOVState_REPLAY_vtbl (0x474AA8); constructor
// cOVState_REPLAY_cOVState_REPLAY (out of line, no callers; triggerOverlay inlines it), destructor
// cOVState_REPLAY__cOVState_REPLAY. Base cOVState (fe/ovstate.h, 0x9C), written out flat below with the same
// member names. On Tour's onCreateScreen stores the same constants (mBottom_y = mMinBottom_y = 245,
// mMaxBottom_y = 480, mBottom_ydiff = 0) in the same member order.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstatereplay.cpp / ovtemplatedialog.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_REPLAY {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVState_REPLAY_vtbl, stored after cOVState_vtbl by the constructor
    // --- cUIState (On Tour: size 0x48, same offsets; see ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor; [U]
                                    //       onCreateScreen stores GetHashValue32("64replay")
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
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the "64replay"
                                    //       screen; widget lookups go through it
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (D_004A2088, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (17)
    // --- cOVState_REPLAY (On Tour: size 0xA8)
    int mShowPopupMenu;             // 0x09C [O] bool; [U] onCreateScreen clears it; onInputBegin sets it when Start
                                    //       opens the replay menu and clears it on close; onUpdate / onWidgetMenu
                                    //       test it
    int mTimeLineHidden;            // 0x0A0 [O] bool; [U] onCreateScreen clears it and shows the matching hints;
                                    //       onInputBegin toggles it (ReplayTimeLine)
    int mBottom_y;                  // 0x0A4 [O]; [U] the panel position: onCreateScreen 245; onUpdate adds
                                    //       mBottom_ydiff and clamps it (read once as lwc1 + cvt.s.w for the
                                    //       object's y)
    int mBottom_ydiff;              // 0x0A8 [O]; [U] the panel speed: onCreateScreen 0; onInputBegin -4 / +4;
                                    //       onUpdate zeroes it at the ends
    int mMinBottom_y;               // 0x0AC [O]; [U] onCreateScreen 245; onUpdate / onInputBegin compare with it
    int mMaxBottom_y;               // 0x0B0 [O]; [U] onCreateScreen 480; onUpdate / onInputBegin compare with it
    int mLoaded;                    // 0x0B4 [O] bool; [U] onCreateScreen copies the replay's loaded flag
                                    //       (Game->Replay + 0x608) and, when set, hides "option_savereplay";
                                    //       onInputBegin's Save / Exit test it
};

CHECK_OFFSET(cOVState_REPLAY, mpPrev, 0x0);
CHECK_OFFSET(cOVState_REPLAY, mpNext, 0x4);
CHECK_OFFSET(cOVState_REPLAY, mVtbl, 0x8);
CHECK_OFFSET(cOVState_REPLAY, mHashName, 0xC);
CHECK_OFFSET(cOVState_REPLAY, mpEngine, 0x10);
CHECK_OFFSET(cOVState_REPLAY, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_REPLAY, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_REPLAY, unk_016, 0x16);
CHECK_OFFSET(cOVState_REPLAY, miMiscData, 0x18);
CHECK_OFFSET(cOVState_REPLAY, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_REPLAY, mpParentState, 0x20);
CHECK_OFFSET(cOVState_REPLAY, mScreens, 0x24);
CHECK_OFFSET(cOVState_REPLAY, mpScreen, 0x40);
CHECK_OFFSET(cOVState_REPLAY, mUserNum, 0x44);
CHECK_OFFSET(cOVState_REPLAY, unk_045, 0x45);
CHECK_OFFSET(cOVState_REPLAY, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_REPLAY, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_REPLAY, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_REPLAY, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_REPLAY, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_REPLAY, mOVID, 0x98);
CHECK_OFFSET(cOVState_REPLAY, mShowPopupMenu, 0x9C);
CHECK_OFFSET(cOVState_REPLAY, mTimeLineHidden, 0xA0);
CHECK_OFFSET(cOVState_REPLAY, mBottom_y, 0xA4);
CHECK_OFFSET(cOVState_REPLAY, mBottom_ydiff, 0xA8);
CHECK_OFFSET(cOVState_REPLAY, mMinBottom_y, 0xAC);
CHECK_OFFSET(cOVState_REPLAY, mMaxBottom_y, 0xB0);
CHECK_OFFSET(cOVState_REPLAY, mLoaded, 0xB4);
CHECK_SIZE(cOVState_REPLAY, 0xB8);

#endif /* FE_OVSTATEREPLAY_H */
