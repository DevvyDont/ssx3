#ifndef FE_OVSTATEHEAT_H
#define FE_OVSTATEHEAT_H

// cOVState_HEAT_PREEVENT: the pre-event overlay of a heat event (its strings: "title", "track_event",
// "rider%d", the Multi Play / Quick Play state text). Retail code in the decomp's unit fe/ovstatepause.cpp
// (onCreateScreen still asm), destructor and constructor in fe/all_in_one_fe.cpp. No members of its own:
// cMemMan_alloc(0x9C, "cOVStateOverlay") for overlay id 8; On Tour size 0x90. Its vtable D_00472850 reuses
// cOVTemplate_Simple_onWidgetEvent / onWidgetMenu (slots 18/19), so it derives from cOVTemplate_Simple (vtable
// 0x474D30), which adds no members either (On Tour 0x90; its constructor func_0021C730 stores nothing past 0x98).
// Constructor func_00221E18, destructor cOVState_HEAT_PREEVENT__cOVState_HEAT_PREEVENT. On Tour's other heat
// overlay, cOVState_HEAT_STANDINGS, has no retail counterpart identified (the overlays next to this one in the
// unit, ids 9 / 21 / 20: func_001FBD20 "41freestyle_pre", func_001FC878 "50multifs_pre", func_001FD268
// "68rival_pre", are other pre-event screens without names). Base cOVState (fe/ovstate.h, 0x9C), written out
// flat below with the same member names.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail's constructors (the cOVState part is inlined: mOVID, mDoneTransition, mAllowPauseMenu, mRemovePDA,
//       mszChatBuff, vptr), destructors and the cMemMan_alloc(size, "cOVStateOverlay") sizes in
//       cOVStateManager_triggerOverlay / cOVStateManager_triggerPopup (fe/ovtemplatedialog.cpp);
//   [P] the July 2003 preview's inline accessors (inherited cUIState/cOVState fields only; see ui/uistate.h);
//   [U] how retail code uses the field (C in src/fe/ovstatepause.cpp, and the asm);
//   [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): member names and order. On Tour's cOVState is
//       0x90 (retail 0x9C) and its bools are 1 byte (retail's are words), so its offsets are not used.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_HEAT_PREEVENT {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_00472850, stored after cOVState_vtbl by the constructor
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
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's overlay-id argument (8)
    // --- cOVTemplate_Simple (vtable 0x474D30; On Tour 0x90): no members
};

CHECK_OFFSET(cOVState_HEAT_PREEVENT, mpPrev, 0x0);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mpNext, 0x4);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mVtbl, 0x8);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mHashName, 0xC);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mpEngine, 0x10);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, unk_016, 0x16);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, miMiscData, 0x18);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mpParentState, 0x20);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mScreens, 0x24);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mpScreen, 0x40);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mUserNum, 0x44);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, unk_045, 0x45);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_HEAT_PREEVENT, mOVID, 0x98);
CHECK_SIZE(cOVState_HEAT_PREEVENT, 0x9C);

#endif /* FE_OVSTATEHEAT_H */
