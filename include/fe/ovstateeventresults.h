#ifndef FE_OVSTATEEVENTRESULTS_H
#define FE_OVSTATEEVENTRESULTS_H

// The event-result overlays of EA's fe/ovstateeventresults.cpp (path from the July 2003 preview's assert strings in
// cOVState_RACE_FINISHOV / cOVState_TOURNAMENT_STANDINGS code): cOVState subclasses created by
// cOVStateManager_triggerOverlay(type) (fe/ovtemplatedialog.cpp) with operator new(size, "cOVStateOverlay") and an
// inlined constructor (cUIState_cUIState, the cOVState part, then the class's vtable at +0x8). Retail code: the
// slot functions in fe/fepopupmisc, the out-of-line constructors / destructors in fe/all_in_one_fe.
//
//   type  class                          size  vtable      screen                      ctor          dtor (slot 1)
//   10    cOVState_HEAT_STANDINGS        0x9C  D_00474088  "42freestyle_standings"     cOVState_HEAT_STANDINGS_cOVState_HEAT_STANDINGS cOVState_HEAT_STANDINGS__cOVState_HEAT_STANDINGS
//   11    cOVState_FINAL_STANDINGS       0xA0  D_00473FB0  "43final_standings"         cOVState_FINAL_STANDINGS_cOVState_FINAL_STANDINGS cOVState_FINAL_STANDINGS__cOVState_FINAL_STANDINGS
//   12    cOVState_RACE_FINISHOV         0xA4  D_00473E00  "finishov" (mHashName)      cOVState_RACE_FINISHOV_cOVState_RACE_FINISHOV cOVState_RACE_FINISHOV__cOVState_RACE_FINISHOV
//   13    cOVState_BASSSP_QUAL_FINISHOV  0xA4  D_00473D28  "finishov" (mHashName)      cOVState_BASSSP_QUAL_FINISHOV_cOVState_BASSSP_QUAL_FINISHOV cOVState_BASSSP_QUAL_FINISHOV__cOVState_BASSSP_QUAL_FINISHOV
//   35    cOVState_TOURNAMENT_STANDINGS  0x9C  D_00473C50  "109MultiplayerStandings"   cOVState_TOURNAMENT_STANDINGS_cOVState_TOURNAMENT_STANDINGS cOVState_TOURNAMENT_STANDINGS__cOVState_TOURNAMENT_STANDINGS
//
// Names (two signals each): HEAT_STANDINGS and FINAL_STANDINGS - On Tour's DWARF has both classes with exactly the
// retail override sets (6 and 9 slots) and sizes base+0 / base+4 (doInit), and the retail screens / texts say
// "standings" (kT_OVRCMNQFHeat1Stand / Heat2Stand; "43final_standings"). RACE_FINISHOV, BASSSP_QUAL_FINISHOV and
// TOURNAMENT_STANDINGS - the preview's _vt$ sections with matching override sets, and their onCreateScreen strings
// (finishov / firstrun / place%d; "Invalid heat for this screen"; "109MultiplayerStandings") and constructor stores
// match retail. (builds/x/v2/name_map_v2.tsv pairs 0x00473FB0 with TOURNAMENT_STANDINGS by position only; the screen
// and override set put TOURNAMENT_STANDINGS at 0x00473C50.)
//
// A FLAT layout, like main/game.h: base cOVState (0x9C) written out with fe/ovstate.h's member names; no inheritance or
// virtuals. Evidence tags: [R] retail constructors / destructors and the "cOVStateOverlay" allocation sizes; [P] the
// preview build; [U] retail usage (C in src/fe/fepopupmisc.cpp and the asm); [O] SSX On Tour's DWARF (its cOVState is
// 0x90 and its bools are 1 byte, so only member names and order are used).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

// cOVState_HEAT_STANDINGS (0x9C, no own members; On Tour likewise has none: size 0x90 = its cOVState). Slots: 4
// onCreateScreen cOVState_HEAT_STANDINGS_onCreateScreen, 6 onGainFocus cOVState_HEAT_STANDINGS_onGainFocus, 8 onInputBegin cOVState_HEAT_STANDINGS_onInputBegin, 12 onUpdate cOVState_HEAT_STANDINGS_onUpdate,
// 19 onWidgetEvent cOVState_HEAT_STANDINGS_onWidgetEvent, 20 onWidgetMenu cOVState_HEAT_STANDINGS_onWidgetMenu.

struct cOVState_HEAT_STANDINGS {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                    // 0x008 [R] cOVState_HEAT_STANDINGS_cOVState_HEAT_STANDINGS stores D_00474088 after cOVState_vtbl
    // --- cUIState (ui/uistate.h, 0x48; same member names)
    int mHashName;                   // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;             // 0x010 [O]; [R] the ctor's engine argument
    signed char mCurrentCtrl;        // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;         // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];               // 0x016 padding
    int miMiscData;                  // 0x018 [O]; [P] cUIState::getMiscData
    int mAttrState;                  // 0x01C [O] tAttrState; [P] cUIState accessors lw/sw the word
    cUIState* mpParentState;         // 0x020 [O]; [R] the 2-argument ctor's parent argument
    char mScreens[0x1C];             // 0x024 [O] cList mScreens; [R] cList_cList(self + 0x24)
    cUIScreen* mpScreen;             // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the new screen
    signed char mUserNum;            // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];               // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C; same member names)
    int mDoneTransition;             // 0x048 [O]; [R] ctor zeroes it
    int mAllowPauseMenu;             // 0x04C [P] allowPauseMenu; [R] ctor zeroes it
    int mRestoreCutSceneMode;        // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                  // 0x054 [O]; [R] ctor zeroes it
    char mszChatBuff[0x40];          // 0x058 [O]; [R] ctor copies " " into it (sh)
    int mOVID;                       // 0x098 [O] tOVStateEnum; [R] ctor stores its id argument
};

CHECK_OFFSET(cOVState_HEAT_STANDINGS, mpPrev, 0x000);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mpNext, 0x004);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mVtbl, 0x008);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mHashName, 0x00C);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mpEngine, 0x010);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mCurrentCtrl, 0x014);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mCtrlBits, 0x015);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, unk_016, 0x016);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, miMiscData, 0x018);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mAttrState, 0x01C);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mpParentState, 0x020);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mScreens, 0x024);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mpScreen, 0x040);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mUserNum, 0x044);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, unk_045, 0x045);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mDoneTransition, 0x048);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mRemovePDA, 0x054);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mszChatBuff, 0x058);
CHECK_OFFSET(cOVState_HEAT_STANDINGS, mOVID, 0x098);
CHECK_SIZE(cOVState_HEAT_STANDINGS, 0x9C);

// cOVState_FINAL_STANDINGS (0xA0: cMemMan size for type 11). Slots: 4 onCreateScreen cOVState_FINAL_STANDINGS_onCreateScreen, 6 onGainFocus
// cOVState_FINAL_STANDINGS_onGainFocus, 7 onGainTransition cOVState_FINAL_STANDINGS_onGainTransition, 8 onInputBegin cOVState_FINAL_STANDINGS_onInputBegin, 11 onLoseFocus cOVState_FINAL_STANDINGS_onLoseFocus, 12
// onUpdate
// cOVState_FINAL_STANDINGS_onUpdate, 18 onWidgetCreate cOVState_FINAL_STANDINGS_onWidgetCreate, 19 onWidgetEvent cOVState_FINAL_STANDINGS_onWidgetEvent, 20 onWidgetMenu cOVState_FINAL_STANDINGS_onWidgetMenu.

struct cOVState_FINAL_STANDINGS {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                    // 0x008 [R] cOVState_FINAL_STANDINGS_cOVState_FINAL_STANDINGS stores D_00473FB0 after cOVState_vtbl
    // --- cUIState (ui/uistate.h, 0x48; same member names)
    int mHashName;                   // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;             // 0x010 [O]; [R] the ctor's engine argument
    signed char mCurrentCtrl;        // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;         // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];               // 0x016 padding
    int miMiscData;                  // 0x018 [O]; [P] cUIState::getMiscData
    int mAttrState;                  // 0x01C [O] tAttrState; [P] cUIState accessors lw/sw the word
    cUIState* mpParentState;         // 0x020 [O]; [R] the 2-argument ctor's parent argument
    char mScreens[0x1C];             // 0x024 [O] cList mScreens; [R] cList_cList(self + 0x24)
    cUIScreen* mpScreen;             // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the new screen
    signed char mUserNum;            // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];               // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C; same member names)
    int mDoneTransition;             // 0x048 [O]; [R] ctor zeroes it
    int mAllowPauseMenu;             // 0x04C [P] allowPauseMenu; [R] ctor zeroes it
    int mRestoreCutSceneMode;        // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                  // 0x054 [O]; [R] ctor zeroes it
    char mszChatBuff[0x40];          // 0x058 [O]; [R] ctor copies " " into it (sh)
    int mOVID;                       // 0x098 [O] tOVStateEnum; [R] ctor stores its id argument
    // --- cOVState_FINAL_STANDINGS
    int doInit;                      // 0x09C [O] bool doInit (On Tour 0x90, its only member); [U] onCreateScreen
                                     //       cOVState_FINAL_STANDINGS_onCreateScreen sets it, onGainTransition cOVState_FINAL_STANDINGS_onGainTransition tests and clears it
                                     //       (lw/sw: a word, as retail bools are)
};

CHECK_OFFSET(cOVState_FINAL_STANDINGS, mpPrev, 0x000);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mpNext, 0x004);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mVtbl, 0x008);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mHashName, 0x00C);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mpEngine, 0x010);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mCurrentCtrl, 0x014);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mCtrlBits, 0x015);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, unk_016, 0x016);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, miMiscData, 0x018);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mAttrState, 0x01C);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mpParentState, 0x020);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mScreens, 0x024);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mpScreen, 0x040);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mUserNum, 0x044);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, unk_045, 0x045);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mDoneTransition, 0x048);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mRemovePDA, 0x054);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mszChatBuff, 0x058);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, mOVID, 0x098);
CHECK_OFFSET(cOVState_FINAL_STANDINGS, doInit, 0x09C);
CHECK_SIZE(cOVState_FINAL_STANDINGS, 0xA0);

// cOVState_RACE_FINISHOV (0xA4: cMemMan size for type 12). The ctor also sets mHashName = GetHashValue32("finishov").
// Slots: 4 onCreateScreen cOVState_RACE_FINISHOV_onCreateScreen, 12 onUpdate func_001E8098, 19 onWidgetEvent func_001E8160, 20 onWidgetMenu
// func_001E81B0, 24 handleEvent func_001E81D0. Preview: _vt$22cOVState_RACE_FINISHOV (overrides 4 / 13 / 19 / 22 in
// its 23-slot layout = retail 4 / 12 / 19 / 24).

struct cOVState_RACE_FINISHOV {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                    // 0x008 [R] cOVState_RACE_FINISHOV_cOVState_RACE_FINISHOV stores D_00473E00 after cOVState_vtbl
    // --- cUIState (ui/uistate.h, 0x48; same member names)
    int mHashName;                   // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;             // 0x010 [O]; [R] the ctor's engine argument
    signed char mCurrentCtrl;        // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;         // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];               // 0x016 padding
    int miMiscData;                  // 0x018 [O]; [P] cUIState::getMiscData
    int mAttrState;                  // 0x01C [O] tAttrState; [P] cUIState accessors lw/sw the word
    cUIState* mpParentState;         // 0x020 [O]; [R] the 2-argument ctor's parent argument
    char mScreens[0x1C];             // 0x024 [O] cList mScreens; [R] cList_cList(self + 0x24)
    cUIScreen* mpScreen;             // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the new screen
    signed char mUserNum;            // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];               // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C; same member names)
    int mDoneTransition;             // 0x048 [O]; [R] ctor zeroes it
    int mAllowPauseMenu;             // 0x04C [P] allowPauseMenu; [R] ctor zeroes it
    int mRestoreCutSceneMode;        // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                  // 0x054 [O]; [R] ctor zeroes it
    char mszChatBuff[0x40];          // 0x058 [O]; [R] ctor copies " " into it (sh)
    int mOVID;                       // 0x098 [O] tOVStateEnum; [R] ctor stores its id argument
    // --- cOVState_RACE_FINISHOV
    float unk_09C;                   // 0x09C [U] onUpdate (slot 12) advances it by 1/60 per frame (lwc1/swc1);
                                     //       onCreateScreen zeroes it
    int unk_0A0;                     // 0x0A0 [R] the ctor stores 1; [U] handleEvent (slot 24) clears it on event 0 and
                                     //       sets it on 1; onUpdate, onWidgetEvent / onWidgetMenu (slots 19 / 20) test
                                     //       it. Preview: the same store of 1 in its ctor and the same handleEvent
};

CHECK_OFFSET(cOVState_RACE_FINISHOV, mpPrev, 0x000);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mpNext, 0x004);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mVtbl, 0x008);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mHashName, 0x00C);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mpEngine, 0x010);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mCurrentCtrl, 0x014);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mCtrlBits, 0x015);
CHECK_OFFSET(cOVState_RACE_FINISHOV, unk_016, 0x016);
CHECK_OFFSET(cOVState_RACE_FINISHOV, miMiscData, 0x018);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mAttrState, 0x01C);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mpParentState, 0x020);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mScreens, 0x024);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mpScreen, 0x040);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mUserNum, 0x044);
CHECK_OFFSET(cOVState_RACE_FINISHOV, unk_045, 0x045);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mDoneTransition, 0x048);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mRemovePDA, 0x054);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mszChatBuff, 0x058);
CHECK_OFFSET(cOVState_RACE_FINISHOV, mOVID, 0x098);
CHECK_OFFSET(cOVState_RACE_FINISHOV, unk_09C, 0x09C);
CHECK_OFFSET(cOVState_RACE_FINISHOV, unk_0A0, 0x0A0);
CHECK_SIZE(cOVState_RACE_FINISHOV, 0xA4);

// cOVState_BASSSP_QUAL_FINISHOV (0xA4: cMemMan size for type 13; same members as RACE_FINISHOV, both written by the
// same ctor pattern). Slots: 4 onCreateScreen cOVState_BASSSP_QUAL_FINISHOV_onCreateScreen, 12 onUpdate func_001E87B8, 19 onWidgetEvent func_001E8880,
// 20 onWidgetMenu func_001E88D0, 24 handleEvent func_001E88F0. Preview: _vt$29cOVState_BASSSP_QUAL_FINISHOV (same
// override set as RACE_FINISHOV; its onCreateScreen labels the runs by heat).

struct cOVState_BASSSP_QUAL_FINISHOV {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                    // 0x008 [R] cOVState_BASSSP_QUAL_FINISHOV_cOVState_BASSSP_QUAL_FINISHOV stores D_00473D28 after cOVState_vtbl
    // --- cUIState (ui/uistate.h, 0x48; same member names)
    int mHashName;                   // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;             // 0x010 [O]; [R] the ctor's engine argument
    signed char mCurrentCtrl;        // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;         // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];               // 0x016 padding
    int miMiscData;                  // 0x018 [O]; [P] cUIState::getMiscData
    int mAttrState;                  // 0x01C [O] tAttrState; [P] cUIState accessors lw/sw the word
    cUIState* mpParentState;         // 0x020 [O]; [R] the 2-argument ctor's parent argument
    char mScreens[0x1C];             // 0x024 [O] cList mScreens; [R] cList_cList(self + 0x24)
    cUIScreen* mpScreen;             // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the new screen
    signed char mUserNum;            // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];               // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C; same member names)
    int mDoneTransition;             // 0x048 [O]; [R] ctor zeroes it
    int mAllowPauseMenu;             // 0x04C [P] allowPauseMenu; [R] ctor zeroes it
    int mRestoreCutSceneMode;        // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                  // 0x054 [O]; [R] ctor zeroes it
    char mszChatBuff[0x40];          // 0x058 [O]; [R] ctor copies " " into it (sh)
    int mOVID;                       // 0x098 [O] tOVStateEnum; [R] ctor stores its id argument
    // --- cOVState_BASSSP_QUAL_FINISHOV
    float unk_09C;                   // 0x09C [U] onUpdate (slot 12) advances it by 1/60 per frame (lwc1/swc1);
                                     //       onCreateScreen zeroes it
    int unk_0A0;                     // 0x0A0 [R] the ctor stores 1; [U] handleEvent (slot 24) clears it on event 0 and
                                     //       sets it on 1; onUpdate, onWidgetEvent / onWidgetMenu (slots 19 / 20) test
                                     //       it. Preview: the same store of 1 in its ctor and the same handleEvent
};

CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mpPrev, 0x000);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mpNext, 0x004);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mVtbl, 0x008);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mHashName, 0x00C);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mpEngine, 0x010);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mCurrentCtrl, 0x014);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mCtrlBits, 0x015);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, unk_016, 0x016);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, miMiscData, 0x018);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mAttrState, 0x01C);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mpParentState, 0x020);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mScreens, 0x024);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mpScreen, 0x040);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mUserNum, 0x044);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, unk_045, 0x045);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mDoneTransition, 0x048);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mRemovePDA, 0x054);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mszChatBuff, 0x058);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, mOVID, 0x098);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, unk_09C, 0x09C);
CHECK_OFFSET(cOVState_BASSSP_QUAL_FINISHOV, unk_0A0, 0x0A0);
CHECK_SIZE(cOVState_BASSSP_QUAL_FINISHOV, 0xA4);

// cOVState_TOURNAMENT_STANDINGS (0x9C, no own members). Slots: 4 onCreateScreen cOVState_TOURNAMENT_STANDINGS_onCreateScreen, 19 onWidgetEvent
// func_001E8FC8. Preview: _vt$29cOVState_TOURNAMENT_STANDINGS overrides the same two slots, screen
// "109MultiplayerStandings".

struct cOVState_TOURNAMENT_STANDINGS {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                    // 0x008 [R] cOVState_TOURNAMENT_STANDINGS_cOVState_TOURNAMENT_STANDINGS stores D_00473C50 after cOVState_vtbl
    // --- cUIState (ui/uistate.h, 0x48; same member names)
    int mHashName;                   // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;             // 0x010 [O]; [R] the ctor's engine argument
    signed char mCurrentCtrl;        // 0x014 [O]; [R] ctor stores -1
    unsigned char mCtrlBits;         // 0x015 [O]; [R] ctor stores 0xF
    char unk_016[0x2];               // 0x016 padding
    int miMiscData;                  // 0x018 [O]; [P] cUIState::getMiscData
    int mAttrState;                  // 0x01C [O] tAttrState; [P] cUIState accessors lw/sw the word
    cUIState* mpParentState;         // 0x020 [O]; [R] the 2-argument ctor's parent argument
    char mScreens[0x1C];             // 0x024 [O] cList mScreens; [R] cList_cList(self + 0x24)
    cUIScreen* mpScreen;             // 0x040 [O]; [R] ctor zeroes it; [U] onCreateScreen stores the new screen
    signed char mUserNum;            // 0x044 [O]; [R] ctor stores -1
    char unk_045[0x3];               // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C; same member names)
    int mDoneTransition;             // 0x048 [O]; [R] ctor zeroes it
    int mAllowPauseMenu;             // 0x04C [P] allowPauseMenu; [R] ctor zeroes it
    int mRestoreCutSceneMode;        // 0x050 [O]; [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                  // 0x054 [O]; [R] ctor zeroes it
    char mszChatBuff[0x40];          // 0x058 [O]; [R] ctor copies " " into it (sh)
    int mOVID;                       // 0x098 [O] tOVStateEnum; [R] ctor stores its id argument
};

CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mpPrev, 0x000);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mpNext, 0x004);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mVtbl, 0x008);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mHashName, 0x00C);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mpEngine, 0x010);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mCurrentCtrl, 0x014);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mCtrlBits, 0x015);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, unk_016, 0x016);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, miMiscData, 0x018);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mAttrState, 0x01C);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mpParentState, 0x020);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mScreens, 0x024);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mpScreen, 0x040);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mUserNum, 0x044);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, unk_045, 0x045);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mDoneTransition, 0x048);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mRemovePDA, 0x054);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mszChatBuff, 0x058);
CHECK_OFFSET(cOVState_TOURNAMENT_STANDINGS, mOVID, 0x098);
CHECK_SIZE(cOVState_TOURNAMENT_STANDINGS, 0x9C);

#endif /* FE_OVSTATEEVENTRESULTS_H */
