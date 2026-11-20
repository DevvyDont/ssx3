#ifndef FE_OVTEMPLATEDIALOG_H
#define FE_OVTEMPLATEDIALOG_H

// cOVTemplate_Dialog: the in-game yes/no dialog template (quit, restart, give up ...), and cOVState_QUIT, the quit
// dialog built on it. Retail code in the decomp's unit fe/ovtemplatedialog.cpp, constructors/destructors in
// fe/all_in_one_fe.cpp; EA's source file is not known (the preview's linkonce sections name _$_13cOVState_QUIT,
// _$_22cOVState_QUITCHALLENGE, _$_24cOVState_QUITCOMPETITION).
//
// cOVTemplate_Dialog: vtable D_00474C58; constructor cOVTemplate_Dialog_cOVTemplate_Dialog (out of line, no
// callers), destructor func_0021C798. On Tour size 0x94 = 0x90 + mDialogType. Its methods are
// cOVTemplate_Dialog_onCreateScreen / onWidgetCreate / onWidgetEvent / onWidgetMenu / onUpdate.
//
// The dialog overlays add no members: each is cMemMan_alloc(0xA0, "cOVStateOverlay") with its own vtable (all
// slots but the destructor are the template's, except cOVState_QUIT's onUpdate) and mDialogType set by the
// factory: id 23 D_004749D0 kind 0, id 24 D_004748F8 kind 1 (cOVState_QUIT: constructor func_0021C950,
// destructor cOVState_QUIT__cOVState_QUIT, own method cOVState_QUIT_onUpdate), id 25 D_00474820 kind 2, id 26
// D_00474748 kind 3, id 27 D_00474670 kind 4, id 28 D_00474598 kind 5, id 38 D_004744C0 kind 6 (On Tour's
// QUITCHALLENGE / QUITCOMPETITION / RESTART / RESTARTCHALLENGE / GIVEUP / SAVEPROMPT are all 0x94 = the
// template's size). So cOVState_QUIT_onUpdate and the template's methods take a cOVTemplate_Dialog*.
// Base cOVState (fe/ovstate.h, 0x9C), written out flat below with the same member names.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovtemplatedialog.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVTemplate_Dialog {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_00474C58 or a dialog overlay's vtable, stored after cOVState_vtbl
                                    //       by the constructors
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
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the dialog screen;
                                    //       widget lookups go through it
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed; [U] onWidgetEvent sets it
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (str_sp_b, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (23-28, 38)
    // --- cOVTemplate_Dialog (On Tour: size 0x94)
    int mDialogType;                // 0x09C [O]; [R] the factory and func_0021C950 store the dialog kind (0-6, see
                                    //       above); [U] onCreateScreen / onWidgetCreate / onWidgetEvent switch on it
};

CHECK_OFFSET(cOVTemplate_Dialog, mpPrev, 0x0);
CHECK_OFFSET(cOVTemplate_Dialog, mpNext, 0x4);
CHECK_OFFSET(cOVTemplate_Dialog, mVtbl, 0x8);
CHECK_OFFSET(cOVTemplate_Dialog, mHashName, 0xC);
CHECK_OFFSET(cOVTemplate_Dialog, mpEngine, 0x10);
CHECK_OFFSET(cOVTemplate_Dialog, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVTemplate_Dialog, mCtrlBits, 0x15);
CHECK_OFFSET(cOVTemplate_Dialog, unk_016, 0x16);
CHECK_OFFSET(cOVTemplate_Dialog, miMiscData, 0x18);
CHECK_OFFSET(cOVTemplate_Dialog, mAttrState, 0x1C);
CHECK_OFFSET(cOVTemplate_Dialog, mpParentState, 0x20);
CHECK_OFFSET(cOVTemplate_Dialog, mScreens, 0x24);
CHECK_OFFSET(cOVTemplate_Dialog, mpScreen, 0x40);
CHECK_OFFSET(cOVTemplate_Dialog, mUserNum, 0x44);
CHECK_OFFSET(cOVTemplate_Dialog, unk_045, 0x45);
CHECK_OFFSET(cOVTemplate_Dialog, mDoneTransition, 0x48);
CHECK_OFFSET(cOVTemplate_Dialog, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVTemplate_Dialog, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVTemplate_Dialog, mRemovePDA, 0x54);
CHECK_OFFSET(cOVTemplate_Dialog, mszChatBuff, 0x58);
CHECK_OFFSET(cOVTemplate_Dialog, mOVID, 0x98);
CHECK_OFFSET(cOVTemplate_Dialog, mDialogType, 0x9C);
CHECK_SIZE(cOVTemplate_Dialog, 0xA0);

#endif /* FE_OVTEMPLATEDIALOG_H */
