#ifndef FE_OVSTATETROPHY_H
#define FE_OVSTATETROPHY_H

// cOVStateTrophy: the in-game trophy overlay ("34trophy"): widget events 5 / 6 ask the custom interface (the
// engine's first word) for the next / previous state and push it.
// EA's source file is not known (no assert of its own). Retail code: onCreateScreen / onWidgetEvent in fe/ovstatetrophy
// (0x0020E938, 0x0020E9A0); the ctors func_002222F8 (engine 0) / func_00222358 (pEngine, id) and the dtor in
// fe/all_in_one_fe (0x002222F8 ... 0x002223C0).
//
// Size 0x9C: [O] On Tour's class is 0x90 = On Tour's cOVState (no members of its own); retail's cOVState is 0x9C
// (fe/ovstate.h) and the ctors write nothing past mOVID (0x98). Retail never calls the ctors (no allocation site).
// Base: cOVState (fe/ovstate.h, 0x9C) : cUIState : cListNode (vptr at 0x8). Both ctors (engine 0 / (pEngine, id)) run
// cUIState_cUIState, the inlined cOVState ctor (mOVID, the three bools, cOVState_vtbl, mszChatBuff = " ") and store
// cOVStateTrophy_vtbl (0x00471EA8, 25 slots: 1 dtor, 4 onCreateScreen, 19 onWidgetEvent,
// 25 cOVState_allowPauseMenu); the dtor stores cOVState_vtbl and runs cUIState__cUIState.
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet. The cOVState
// part is written out with fe/ovstate.h's names (see there for the inherited fields' full evidence).
// Evidence tags: [R] retail's ctors / dtor; [P] the July 2003 preview (fe/ovstate.h); [U] how retail code uses the
// field; [O] SSX On Tour's DWARF (builds/x/ontour_kor/dwarf/types.h): names, types and order.

#include "layout.h"

struct cListNode;
struct cUIEngine;
struct cUIState;
struct cUIScreen;

struct cOVStateTrophy {
    // --- cListNode (On Tour: size 0xC, mpPrev/mpNext + vptr)
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the cUIState ctor links the node to itself
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto
    void** mVtbl;                   // 0x008 [R] cOVStateTrophy_vtbl (ctors), cOVState_vtbl (dtor)
    // --- cUIState (On Tour: size 0x48, same offsets; ui/uistate.h)
    int mHashName;                  // 0x00C [O]
    cUIEngine* mpEngine;            // 0x010 [O]; [R] the ctor's engine argument; [U] onCreateScreen adds "34trophy"
                                    //       through it; onWidgetEvent calls its custom interface (+0x0) slots 4 / 5 and
                                    //       pushes the result on its stack (+0x18)
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
    // --- cOVState (fe/ovstate.h, 0x9C)
    int mDoneTransition;            // 0x048 [O] (On Tour: bool at 0x48); [R] the ctors zero it
    int mAllowPauseMenu;            // 0x04C [O][P] (fe/ovstate.h); [R] the ctors zero it
    int mRestoreCutSceneMode;       // 0x050 [O] (bool); [U] cOVState_onCreateScreen / onDestroyScreen
    int mRemovePDA;                 // 0x054 [O] (bool); [R] the ctors zero it
    char mszChatBuff[0x40];         // 0x058 [O] (char[63] in On Tour); [R] the ctors copy " " into it (one sh)
    int mOVID;                      // 0x098 [O] tOVStateEnum; [R] the (pEngine, id) ctor stores id, the default ctor 0
};

CHECK_OFFSET(cOVStateTrophy, mpPrev, 0x000);
CHECK_OFFSET(cOVStateTrophy, mpNext, 0x004);
CHECK_OFFSET(cOVStateTrophy, mVtbl, 0x008);
CHECK_OFFSET(cOVStateTrophy, mHashName, 0x00C);
CHECK_OFFSET(cOVStateTrophy, mpEngine, 0x010);
CHECK_OFFSET(cOVStateTrophy, mCurrentCtrl, 0x014);
CHECK_OFFSET(cOVStateTrophy, mCtrlBits, 0x015);
CHECK_OFFSET(cOVStateTrophy, unk_016, 0x016);
CHECK_OFFSET(cOVStateTrophy, miMiscData, 0x018);
CHECK_OFFSET(cOVStateTrophy, mAttrState, 0x01C);
CHECK_OFFSET(cOVStateTrophy, mpParentState, 0x020);
CHECK_OFFSET(cOVStateTrophy, mScreens, 0x024);
CHECK_OFFSET(cOVStateTrophy, mpScreen, 0x040);
CHECK_OFFSET(cOVStateTrophy, mUserNum, 0x044);
CHECK_OFFSET(cOVStateTrophy, unk_045, 0x045);
CHECK_OFFSET(cOVStateTrophy, mDoneTransition, 0x048);
CHECK_OFFSET(cOVStateTrophy, mAllowPauseMenu, 0x04C);
CHECK_OFFSET(cOVStateTrophy, mRestoreCutSceneMode, 0x050);
CHECK_OFFSET(cOVStateTrophy, mRemovePDA, 0x054);
CHECK_OFFSET(cOVStateTrophy, mszChatBuff, 0x058);
CHECK_OFFSET(cOVStateTrophy, mOVID, 0x098);
CHECK_SIZE(cOVStateTrophy, 0x9C);

#endif /* FE_OVSTATETROPHY_H */
