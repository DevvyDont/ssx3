#ifndef FE_OVSTATEPAUSE_H
#define FE_OVSTATEPAUSE_H

// The in-game pause menus (EA's d:/ssx/code/source/fe/ovstatepause.h: the preview's assert strings name it next to
// "cOVTemplate_PauseMenu"; retail code in the decomp's units fe/ovtemplatepausemenu.cpp and fe/ovstatepause.cpp,
// constructors/destructors in fe/all_in_one_fe.cpp). Three layouts:
//
// cOVTemplate_PauseMenu: the pause-menu template (screen "31paus_freeride": up to 8 option rows, the PDA icons,
// the new-mail pulse). Retail vtable cOVTemplate_PauseMenu_vtbl (0x473170); constructors
// cOVTemplate_PauseMenu_cOVTemplate_PauseMenu(engine, id) and func_002214F8 (no engine), destructor
// cOVTemplate_PauseMenu__cOVTemplate_PauseMenu. Size 0x114: every subclass is allocated with
// cMemMan_alloc(0x114, "cOVStateOverlay") (overlay ids 1-6) and nothing touches a word past 0x110.
// Its six subclasses add no members (On Tour: all 0xD8 = cOVTemplate_PauseMenu's size; retail: the same 0x114
// allocation, constructors that only swap the vptr) and override only onCreateScreen, which fills
// mPauseMenuOptions / mPauseMenuNumOptions / mPauseMenuType and calls cOVTemplate_PauseMenu_onCreateScreen:
//   cOVState_PAUSE_COMPETE   id 1  vtbl 0x473098      cOVState_PAUSE_BIGCHAL   id 2  vtbl 0x472FC0
//   cOVState_PAUSE_FREERIDE  id 3  vtbl 0x472EE8      cOVState_PAUSE_SPOKE     id 4  vtbl 0x472E10
//   cOVState_PAUSE_QUICKPLAY id 5  vtbl 0x472D38      cOVState_PAUSE_ONLINE    id 6  vtbl 0x472C60
// so their methods take a cOVTemplate_PauseMenu* (no separate structs).
//
// cOVState_PAUSE_OPTIONS: the in-game options screen (overlay id 7). No members of its own: allocated 0x9C,
// On Tour size 0x90 (= cOVState). Vtable cOVState_PAUSE_OPTIONS_vtbl (0x472B88).
//
// cOVState_PAUSE_ONLINE_ERROR: the online "connection problem" pause (overlay id 39, screen with Continue /
// Cancel and a ping status). Allocated 0xA4; vtable cOVState_PAUSE_ONLINE_ERROR_vtbl (0x472AB0); constructors
// cOVState_PAUSE_ONLINE_ERROR_cOVState_PAUSE_ONLINE_ERROR (stores 0xA0 = 1, 0x9C = 0) and func_00221C80.
//
// All three derive from cOVState (fe/ovstate.h, 0x9C), written out flat below with the same member names.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovtemplatepausemenu.cpp / ovstatepause.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;
struct cUIThread;

struct cOVTemplate_PauseMenu {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVTemplate_PauseMenu_vtbl (or a subclass's), stored after
                                    //       cOVState_vtbl by the constructors
    // --- cUIState (On Tour: size 0x48, same offsets; see ui/uistate.h)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor; [U]
                                    //       onCreateScreen stores GetHashValue32("cOVTemplate_PauseMenu")
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
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] unused by the template (it keeps its own
                                    //       mpPauseScreen at 0xC0)
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (str_sp_b, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (1-6)
    // --- cOVTemplate_PauseMenu (On Tour: size 0xD8)
    int mPauseMenuOptions[8];       // 0x09C [O]; [U] the subclasses' onCreateScreen copy the option ids here;
                                    //       onCreateScreen / onGainTransition / onGainFocus index row i
    int mPauseMenuNumOptions;       // 0x0BC [O]; [U] set to 5..7 (2/3 online) by the subclasses' onCreateScreen;
                                    //       rows i >= it are hidden
    cUIScreen* mpPauseScreen;       // 0x0C0 [O] cOVTemplate_PauseMenu::mpScreen (it hides cUIState::mpScreen at
                                    //       0x40; renamed here); [U] onCreateScreen stores
                                    //       cUIEngine_addScreenByHashName("31paus_freeride"); every lookup goes
                                    //       through it
    cUIThread* mpThread;            // 0x0C4 [O] (On Tour onDestroyScreen clears it: sw zero,0xB8); [R] constructors
                                    //       zero it; [U] setPDAIconColor keeps cUIScreen_playFrame's result here;
                                    //       onDestroyScreen clears it
    float mNewMailAlpha;            // 0x0C8 [O]; [U] onUpdate pulses it between 0 and 1 and sets the "NewMail"
                                    //       icon's alpha; onCreateScreen zeroes it (sw $zero)
    float mNewMailAlphaDiff;        // 0x0CC [O]; [U] onCreateScreen stores 0.05f; onUpdate negates it at the ends
    int mScreenExiting;             // 0x0D0 [O] bool; [U] set by onWidgetEvent (option chosen) and onUpdate (online
                                    //       timeout) when "trans_out" plays; onWidgetEvent ignores input while it is
                                    //       set
    int mMoveIcons;                 // 0x0D4 [O] bool; [U] onCreateScreen sets it, onGainTransition lays the PDA icon
                                    //       groups out once and clears it
    int mPauseOptionUnlocked[14];   // 0x0D8 [O] (On Tour bool[13]); [U] onCreateScreen sets 14 words (option ids
                                    //       0-13) to 1, then clears the ones not allowed; onGainFocus /
                                    //       onWidgetEvent index it by option id
    int mPauseMenuType;             // 0x110 [O] tOVStateEnum; [U] the subclasses' onCreateScreen store their overlay
                                    //       id (1-6); onUpdate pulses the mail icon for 1, 2 and 4
};

CHECK_OFFSET(cOVTemplate_PauseMenu, mpPrev, 0x0);
CHECK_OFFSET(cOVTemplate_PauseMenu, mpNext, 0x4);
CHECK_OFFSET(cOVTemplate_PauseMenu, mVtbl, 0x8);
CHECK_OFFSET(cOVTemplate_PauseMenu, mHashName, 0xC);
CHECK_OFFSET(cOVTemplate_PauseMenu, mpEngine, 0x10);
CHECK_OFFSET(cOVTemplate_PauseMenu, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVTemplate_PauseMenu, mCtrlBits, 0x15);
CHECK_OFFSET(cOVTemplate_PauseMenu, unk_016, 0x16);
CHECK_OFFSET(cOVTemplate_PauseMenu, miMiscData, 0x18);
CHECK_OFFSET(cOVTemplate_PauseMenu, mAttrState, 0x1C);
CHECK_OFFSET(cOVTemplate_PauseMenu, mpParentState, 0x20);
CHECK_OFFSET(cOVTemplate_PauseMenu, mScreens, 0x24);
CHECK_OFFSET(cOVTemplate_PauseMenu, mpScreen, 0x40);
CHECK_OFFSET(cOVTemplate_PauseMenu, mUserNum, 0x44);
CHECK_OFFSET(cOVTemplate_PauseMenu, unk_045, 0x45);
CHECK_OFFSET(cOVTemplate_PauseMenu, mDoneTransition, 0x48);
CHECK_OFFSET(cOVTemplate_PauseMenu, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVTemplate_PauseMenu, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVTemplate_PauseMenu, mRemovePDA, 0x54);
CHECK_OFFSET(cOVTemplate_PauseMenu, mszChatBuff, 0x58);
CHECK_OFFSET(cOVTemplate_PauseMenu, mOVID, 0x98);
CHECK_OFFSET(cOVTemplate_PauseMenu, mPauseMenuOptions, 0x9C);
CHECK_OFFSET(cOVTemplate_PauseMenu, mPauseMenuNumOptions, 0xBC);
CHECK_OFFSET(cOVTemplate_PauseMenu, mpPauseScreen, 0xC0);
CHECK_OFFSET(cOVTemplate_PauseMenu, mpThread, 0xC4);
CHECK_OFFSET(cOVTemplate_PauseMenu, mNewMailAlpha, 0xC8);
CHECK_OFFSET(cOVTemplate_PauseMenu, mNewMailAlphaDiff, 0xCC);
CHECK_OFFSET(cOVTemplate_PauseMenu, mScreenExiting, 0xD0);
CHECK_OFFSET(cOVTemplate_PauseMenu, mMoveIcons, 0xD4);
CHECK_OFFSET(cOVTemplate_PauseMenu, mPauseOptionUnlocked, 0xD8);
CHECK_OFFSET(cOVTemplate_PauseMenu, mPauseMenuType, 0x110);
CHECK_SIZE(cOVTemplate_PauseMenu, 0x114);

// cOVState_PAUSE_OPTIONS: cOVState with no members of its own.
struct cOVState_PAUSE_OPTIONS {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVState_PAUSE_OPTIONS_vtbl, stored after cOVState_vtbl by
                                    //       cOVState_PAUSE_OPTIONS_cOVState_PAUSE_OPTIONS
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
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the options screen;
                                    //       every widget lookup goes through it
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (str_sp_b, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (7)
};

CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mpPrev, 0x0);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mpNext, 0x4);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mVtbl, 0x8);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mHashName, 0xC);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mpEngine, 0x10);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, unk_016, 0x16);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, miMiscData, 0x18);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mpParentState, 0x20);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mScreens, 0x24);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mpScreen, 0x40);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mUserNum, 0x44);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, unk_045, 0x45);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_PAUSE_OPTIONS, mOVID, 0x98);
CHECK_SIZE(cOVState_PAUSE_OPTIONS, 0x9C);

struct cOVState_PAUSE_ONLINE_ERROR {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVState_PAUSE_ONLINE_ERROR_vtbl, stored after cOVState_vtbl by the
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
    cUIScreen* mpScreen;            // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the screen; every
                                    //       widget lookup goes through it
    signed char mUserNum;           // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O]; [R] zeroed by the (inlined) cOVState ctor
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O]; [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O]; [R] the ctor copies " " (str_sp_b, one halfword)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (39)
    // --- cOVState_PAUSE_ONLINE_ERROR (On Tour: size 0x98)
    unsigned int miStartPingTick;   // 0x09C [O]; [R] the constructor zeroes it; [U] onCreateScreen stores the app
                                    //       tick (cAppMan_TheAppMan + 0x18) when it pings the server; onUpdate tests
                                    //       it and clears it when the ping is answered or times out
    int mbShouldDisplayAuxInfo;     // 0x0A0 [O] bool; [R] the constructor stores 1; [U] onCreateScreen clears it for
                                    //       every error but the local cheat/delay ones; displayPingTimedOut /
                                    //       displayPingReceived test it
};

CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mpPrev, 0x0);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mpNext, 0x4);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mVtbl, 0x8);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mHashName, 0xC);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mpEngine, 0x10);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, unk_016, 0x16);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, miMiscData, 0x18);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mpParentState, 0x20);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mScreens, 0x24);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mpScreen, 0x40);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mUserNum, 0x44);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, unk_045, 0x45);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mOVID, 0x98);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, miStartPingTick, 0x9C);
CHECK_OFFSET(cOVState_PAUSE_ONLINE_ERROR, mbShouldDisplayAuxInfo, 0xA0);
CHECK_SIZE(cOVState_PAUSE_ONLINE_ERROR, 0xA4);

#endif /* FE_OVSTATEPAUSE_H */
