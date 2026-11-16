#ifndef FE_OVSTATE_H
#define FE_OVSTATE_H

// cOVState: the base of the in-game overlay states (pause menu, map, replay, autosave, big-challenge screens ...;
// cOVState_MAP, cOVState_REPLAY, ... derive from it). It is a cUIState with a few flags, a chat buffer and its
// overlay id. Retail vtable _vt$8cOVState = D_00474E08 (slot 4 cOVState_onCreateScreen, slot 5
// cOVState_onDestroyScreen); its constructor cOVState(cUIEngine*, tOVStateEnum) is func_0021C678.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Subclasses' own
// fields start at 0x9C (e.g. the mission id at 0x9C in ovstateenterlodge.cpp), so this header covers the base only:
// retype a subclass method (cOVState_MAP_*, cOVState_REPLAY_*, ...) only with that subclass's own header.
// Evidence tags:
//   [R] retail's constructors: cUIState(cUIEngine*) = func_0039E2A0 / cUIState(cUIEngine*, cUIState*) =
//       func_0039E318 (ui/uiscreen.cpp) and cOVState(cUIEngine*, tOVStateEnum) = func_0021C678;
//   [P] the July 2003 preview's cOVState::allowPauseMenu (lw 0x48), carried to retail 0x4C by tools/layout_xlate.py;
//   [U] how matched retail code uses the field;
//   [O] SSX On Tour's DWARF (class cListNode / cUIState / cOVState: same member order; On Tour's bools are
//       1 byte, retail's are words), used only where the fields on both sides are placed by other evidence.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;          // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;          // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;               // 0x008 [R] _vt$8cOVState D_00474E08, stored by func_0021C678 after the base ctor
    // --- cUIState (On Tour: size 0x48, same offsets)
    int mHashName;              // 0x00C [O]; [P] cUIState::getHashName (ui/uistate.h); [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;        // 0x010 [O]; [R] the ctor's engine argument; [U] cUIStateStack_findState(mpEngine + 0x18)
    signed char mCurrentCtrl;   // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;    // 0x015 [O]; [R] ctor stores 0xF; [U] masks in func_0039E438 / func_001DD778
    char unk_016[0x2];          // 0x016 padding
    int miMiscData;             // 0x018 [O]; [P] cUIState::getMiscData lw/sw (ui/uistate.h)
    int mAttrState;             // 0x01C [O] tAttrState (EA: 2 bytes); [P] cUIState's accessors lw/sw the word (ui/uistate.h); [R] ctor sh 0x218
    cUIState* mpParentState;    // 0x020 [O]; [R] the 2-argument ctor's parent argument (func_0039E318)
    char mScreens[0x1C];        // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C);
                                //       [R] constructed by func_003977E8(self + 0x24), destroyed in func_0039E390
    cUIScreen* mpScreen;        // 0x040 [O]; [R] ctor zeroes it; [U] cUIScreen_getObjectByHashName(mpScreen, ...)
    signed char mUserNum;       // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];          // 0x045 padding
    // --- cOVState
    int mDoneTransition;        // 0x048 [O] (On Tour: bool at 0x48); [R] func_0021C678 zeroes it
    int mAllowPauseMenu;        // 0x04C [P] allowPauseMenu (preview 0x48); [R] func_0021C678 zeroes it;
                                //       [U] cOVState_allowPauseMenu returns it
    int mRestoreCutSceneMode;   // 0x050 [O]; [U] cOVState_onCreateScreen sets it when Game->CutSceneTransitionMan's
                                //       mode is 1 or 3 (after func_002EA820), onDestroyScreen undoes it (func_002EA860)
    int mRemovePDA;             // 0x054 [O]; [R] zeroed by func_0021C678; [U] cOVState_onDestroyScreen calls
                                //       cOVStateManager_removePDATemplate when it is set
    char mszChatBuff[0x40];     // 0x058 [O] (On Tour: char[63]); [R] func_0021C678 copies " " into it (sh);
                                //       [U] func_0020A4E0 fills it with the incoming chat name and prints it
    int mOVID;                  // 0x098 [O] tOVStateEnum; [R] func_0021C678 stores its id argument
};

CHECK_OFFSET(cOVState, mpPrev, 0x0);
CHECK_OFFSET(cOVState, mpNext, 0x4);
CHECK_OFFSET(cOVState, mVtbl, 0x8);
CHECK_OFFSET(cOVState, mHashName, 0xC);
CHECK_OFFSET(cOVState, mpEngine, 0x10);
CHECK_OFFSET(cOVState, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState, unk_016, 0x16);
CHECK_OFFSET(cOVState, miMiscData, 0x18);
CHECK_OFFSET(cOVState, mAttrState, 0x1C);
CHECK_OFFSET(cOVState, mpParentState, 0x20);
CHECK_OFFSET(cOVState, mScreens, 0x24);
CHECK_OFFSET(cOVState, mpScreen, 0x40);
CHECK_OFFSET(cOVState, mUserNum, 0x44);
CHECK_OFFSET(cOVState, unk_045, 0x45);
CHECK_OFFSET(cOVState, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState, mOVID, 0x98);
CHECK_SIZE(cOVState, 0x9C);

#endif /* FE_OVSTATE_H */
