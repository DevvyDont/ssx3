#ifndef FE_OVSTATETRANSPORT_H
#define FE_OVSTATETRANSPORT_H

// cOVState_TRANSPORT: the in-game transport (mountain map / travel) overlay, overlay id 33 (screen "Map"). It is a thin
// cOVState around an embedded cUITemplate_MAP (the same map template the FE's event select and credits use at +0x48):
// every method forwards to the template's same-named method with &mMap.
// Retail code: cOVState_TRANSPORT_* in fe/uitemplatemap (0x00208498 ... 0x00208678) and fe/all_in_one_fe (destructor
// cOVState_TRANSPORT__cOVState_TRANSPORT, the out-of-line ctor copies func_00221228 (engine, id) and func_002211C8
// (engine-less)). On Tour defines it in fe/all_in_one_fe.cpp.
//
// Size 0x390:
//   [R] cOVStateManager_triggerOverlay builds overlay 33 inline in operator new(912, "cOVStateOverlay") and runs
//       cUITemplate_MAP_init(this + 0x9C);
//   [O] On Tour 0x1B4 = cOVState 0x90 + cUITemplate_MAP mMap (0x124), nothing after it; so retail's cUITemplate_MAP is
//       0x390 - 0x9C = 0x2F4, which matches its own methods' highest access (a word at 0x2F0, On Tour's last member
//       mScreenExiting).
// Base: cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8), written out flat.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals.
// Evidence tags:
//   [R] retail's constructors and allocation size;
//   [P] the July 2003 preview's inline accessors (inherited fields only);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cOVState_TRANSPORT / cUITemplate_MAP).

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVState_TRANSPORT {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] D_004733F8 (no symbol yet; vtables.tsv: cOVState_TRANSPORT, slot 0
                                    //       cOVState_TRANSPORT__cOVState_TRANSPORT), stored after cOVState_vtbl by
                                    //       func_00221228 / func_002211C8 / the inlined ctor in
                                    //       cOVStateManager_triggerOverlay
    // --- cUIState (ui/uistate.h; same offsets as On Tour)
    int mHashName;                  // 0x00C [O]; [P] cUIState::getHashName; [R] zeroed by the cUIState ctor
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine; [U] onCreateScreen adds "Map" to it;
                                    //       onWidgetEvent pops its state stack (+0x18)
    signed char mCurrentCtrl;       // 0x014 [O]; [R] the cUIState ctor stores -1
    unsigned char mCtrlBits;        // 0x015 [O]; [R] the cUIState ctor stores 0xF
    char unk_016[0x2];              // 0x016 padding
    int miMiscData;                 // 0x018 [O]; [P] cUIState::getMiscData lw/sw
    int mAttrState;                 // 0x01C [O] tAttrState (EA: 2 bytes); [U] onWidgetEvent sets mode 7 + the
                                    //       pending-delete bit (lw/sw the word)
    cUIState* mpParentState;        // 0x020 [O]
    char mScreens[0x1C];            // 0x024 [O] cList mScreens (two cListNodes + the list's vptr at 0x3C)
    cUIScreen* mpScreen;            // 0x040 [O]; [U] onCreateScreen keeps the "Map" screen here and hands it to
                                    //       cUITemplate_MAP_onCreateScreen
    signed char mUserNum;           // 0x044 [O]; [R] the cUIState ctor stores -1
    char unk_045[0x3];              // 0x045 padding
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (bool); [R] the (inlined) cOVState ctor zeroes it
    int mAllowPauseMenu;            // 0x04C [P] cOVState::allowPauseMenu (preview 0x48); [R] zeroed
    int mRestoreCutSceneMode;       // 0x050 [O] (bool); [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O] (bool); [R] zeroed
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " (str_sp_b) into it
                                    //       (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the ctor's id (33 from
                                    //       cOVStateManager_triggerOverlay)
    // --- cOVState_TRANSPORT
    char mMap[0x2F4];               // 0x09C [O] cUITemplate_MAP mMap (On Tour 0x124; retail 0x2F4: its bools,
                                    //       including the two bool[8][3][3] tables, are words); [R] the ctors run
                                    //       cUITemplate_MAP_init on it; [U] every method passes &mMap to the
                                    //       same-named cUITemplate_MAP_* method (cUITemplate_MAP's own accesses reach
                                    //       0x2F0). Raw until cUITemplate_MAP has a header
};

CHECK_OFFSET(cOVState_TRANSPORT, mpPrev, 0x0);
CHECK_OFFSET(cOVState_TRANSPORT, mpNext, 0x4);
CHECK_OFFSET(cOVState_TRANSPORT, mVtbl, 0x8);
CHECK_OFFSET(cOVState_TRANSPORT, mHashName, 0xC);
CHECK_OFFSET(cOVState_TRANSPORT, mpEngine, 0x10);
CHECK_OFFSET(cOVState_TRANSPORT, mCurrentCtrl, 0x14);
CHECK_OFFSET(cOVState_TRANSPORT, mCtrlBits, 0x15);
CHECK_OFFSET(cOVState_TRANSPORT, unk_016, 0x16);
CHECK_OFFSET(cOVState_TRANSPORT, miMiscData, 0x18);
CHECK_OFFSET(cOVState_TRANSPORT, mAttrState, 0x1C);
CHECK_OFFSET(cOVState_TRANSPORT, mpParentState, 0x20);
CHECK_OFFSET(cOVState_TRANSPORT, mScreens, 0x24);
CHECK_OFFSET(cOVState_TRANSPORT, mpScreen, 0x40);
CHECK_OFFSET(cOVState_TRANSPORT, mUserNum, 0x44);
CHECK_OFFSET(cOVState_TRANSPORT, unk_045, 0x45);
CHECK_OFFSET(cOVState_TRANSPORT, mDoneTransition, 0x48);
CHECK_OFFSET(cOVState_TRANSPORT, mAllowPauseMenu, 0x4C);
CHECK_OFFSET(cOVState_TRANSPORT, mRestoreCutSceneMode, 0x50);
CHECK_OFFSET(cOVState_TRANSPORT, mRemovePDA, 0x54);
CHECK_OFFSET(cOVState_TRANSPORT, mszChatBuff, 0x58);
CHECK_OFFSET(cOVState_TRANSPORT, mOVID, 0x98);
CHECK_OFFSET(cOVState_TRANSPORT, mMap, 0x9C);
CHECK_SIZE(cOVState_TRANSPORT, 0x390);

#endif /* FE_OVSTATETRANSPORT_H */
