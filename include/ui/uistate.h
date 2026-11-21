#ifndef UI_UISTATE_H
#define UI_UISTATE_H

// cUIState: one state (page/popup) of the UI state machine. The UI engine owns a cUIStateStack of them; each state
// owns a cList of cUIScreens (cUIState_addScreen, cUIEngine_addScreenByHashName). The FE and in-game states
// (cFEState*, cOVState*, cGameLoadState, popups ...) derive from it: they call the cUIState constructor
// cUIState_cUIState(this, pEngine) or cUIState_cUIState1(this, pEngine, pParentState), store their own vtable at 0x8, and
// put their own members from 0x48 on (cFEStateCharSelect_cFEStateCharSelect; ovstates allocates a 0x4C-byte popup), so cUIState is 0x48 bytes.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Base class: cListNode (On Tour: mpPrev 0x0, mpNext 0x4; its vptr follows at 0x8). _vt$8cUIState (cUIState_vtbl)
// keeps cListNode's traverseFlat/traverseRecurse (cListNode_traverseFlat/cListNode_traverseRecurse) in slots 2-3.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cUIState, size 0x48: the same offsets as retail),
// placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors; retail keeps out-of-line copies of them in On Tour's declaration
//       order in ui/uivector (cUIState_setHashName setHashName ... cUIState_setVisible setVisible, cUIState_setParentState ...
//       cUIState_getUserNum getUserNum), which places each one. The preview itself packed the bytes differently
//       (tAttrState at 0x16, miMiscData last), so preview offsets are not used directly;
//   [R] retail's constructors cUIState_cUIState/cUIState_cUIState1 and the destructor cUIState__cUIState;
//   [U] how retail code uses the field;
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [P] or [R].
//
// mAttrState is EA's tAttrState (On Tour, LSB first): mIsPopup:1 mHasPopup:2 mVisible:1 mFirstTime:1 mEnabled:1
// mExplicit:1 mPendingDelete:1 mMode:6 mPushCheck:1 mPopFloor:1 (struct tAttrState below). EA's type is 2 bytes (the constructors store it
// with sh 0x218 = visible, first time, mode 2), but every accessor reads and writes the aligned word (lw/sw 0x1C),
// so it is an int here and covers the 2 padding bytes.

#include "layout.h"
#include "util/list.h"

struct cListNode;
struct cUIEngine;
struct cUIScreen;

// EA's tAttrState, the type of cUIState::mAttrState (On Tour struct tAttrState, size 0x2, LSB first). cUIState keeps
// mAttrState an int (its accessors use lw/sw 0x1C, and it covers the 2 padding bytes); this type names the bits for
// code that reads one field, e.g. ((tAttrState*)&state->mAttrState)->mMode. [R] the constructors store 0x218 with sh
// (mVisible, mFirstTime, mMode = 2); [P] the inline accessors getMode (lbu 0x1D), isFirstTime ... setVisible.
struct tAttrState {
    unsigned short mIsPopup : 1;       // bit 0 [O]
    unsigned short mHasPopup : 2;      // bits 1-2 [O]
    unsigned short mVisible : 1;       // bit 3 [O]; [R] set by the ctors (0x218); [P] setVisible; [U]
                                       //       cUIStateStack_setVisibleToChain writes bit 3 along the parent chain
    unsigned short mFirstTime : 1;     // bit 4 [O]; [R] set by the ctors; [P] isFirstTime
    unsigned short mEnabled : 1;       // bit 5 [O]
    unsigned short mExplicit : 1;      // bit 6 [O]
    unsigned short mPendingDelete : 1; // bit 7 [O]
    unsigned short mMode : 6;          // bits 8-13 [O]; [R] 2 in the ctors; [P] getMode (lbu 0x1D); [U] "FEOptions"
                                       //       states set it (func_001887A0 callers mask 0x3F00)
    unsigned short mPushCheck : 1;     // bit 14 [O]
    unsigned short mPopFloor : 1;      // bit 15 [O]
};

struct cUIState {
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructors self-link it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked
    void** mVtbl;                    // 0x008 [R] _vt$8cUIState (cUIState_vtbl), stored by cUIState_cUIState/cUIState_cUIState1/cUIState_cUIState2
    int mHashName;                   // 0x00C [P] getHashName/setHashName (retail cUIState_getHashName/cUIState_setHashName); [R] = 0
                                     //       (derived states store their state id here, e.g. 9 in cFEStateCharSelect_cFEStateCharSelect)
    cUIEngine* mpEngine;             // 0x010 [P] getEngine (retail cUIState_getEngine); [R] = pEngine; [U] cUIState_onUpdate reads the
                                     //       engine's input object through it
    signed char mCurrentCtrl;        // 0x014 [P] getCurrentCtrl/setCurrentCtrl (lb/sb; retail cUIState_getCurrentCtrl/cUIState_setCurrentCtrl);
                                     //       [R] = -1; [U] controller loop counter in cUIState_onUpdate (On Tour: char)
    unsigned char mCtrlBits;         // 0x015 [P] getCtrlBitMask/setAllCtrlBits (lbu/sb; retail cUIState_getCtrlBitMask/cUIState_setAllCtrlBits);
                                     //       [R] = 0xF; [U] setCtrlBitsByCtrlIndex/clearCtrlBits (cUIState_setCtrlBitsByCtrlIndex/cUIState_clearCtrlBits)
    char unk_016[0x2];               // 0x016 padding
    int miMiscData;                  // 0x018 [P] getMiscData/setMiscData (lw/sw 0x18; retail cUIState_getMiscData/cUIState_setMiscData,
                                     //       between getCtrlBitMask and getMode in On Tour's order); not set by the constructors
    int mAttrState;                  // 0x01C [P] tAttrState: getMode (lbu 0x1D), isFirstTime ... setVisible (lw/sw 0x1C; retail
                                     //       cUIState_getMode..cUIState_setVisible); [R] sh 0x218 in the constructors
    cUIState* mpParentState;         // 0x020 [P] getParentState (retail cUIState_getParentState; setter cUIState_setParentState); [R] = pParentState
                                     //       (cUIState_cUIState1), 0 (cUIState_cUIState)
    cList mScreens;                  // 0x024 [O] cList mScreens, the state's cUIScreens (util/list.h: mStart 0x24, mEnd 0x30, vptr 0x3C);
                                     //       [R] the ctors cList_cList(&mScreens); the destructor cUIState__cUIState restores
                                     //       mScreens.mVtbl (0x3C, cList_vtbl), mEnd.mVtbl (0x38) and mStart.mVtbl (0x2C, cListNode_vtbl);
                                     //       [U] cUIState_addScreen appends (cList_addToEnd), cUIState_updateScreens / cUIState_onUpdate
                                     //       walk it (cList_first), cUIState_onDestroyScreen destroys it
    cUIScreen* mpScreen;             // 0x040 [P] getScreen (retail cUIState_getScreen); [R] = 0
    signed char mUserNum;            // 0x044 [P] getUserNum/setUserNum (lb/sb; retail cUIState_getUserNum/cUIState_setUserNum); [R] = -1 (On Tour: char)
    char unk_045[0x3];               // 0x045 padding
};

CHECK_SIZE(tAttrState, 0x2);

CHECK_OFFSET(cUIState, mpPrev, 0x000);
CHECK_OFFSET(cUIState, mpNext, 0x004);
CHECK_OFFSET(cUIState, mVtbl, 0x008);
CHECK_OFFSET(cUIState, mHashName, 0x00C);
CHECK_OFFSET(cUIState, mpEngine, 0x010);
CHECK_OFFSET(cUIState, mCurrentCtrl, 0x014);
CHECK_OFFSET(cUIState, mCtrlBits, 0x015);
CHECK_OFFSET(cUIState, miMiscData, 0x018);
CHECK_OFFSET(cUIState, mAttrState, 0x01C);
CHECK_OFFSET(cUIState, mpParentState, 0x020);
CHECK_OFFSET(cUIState, mScreens, 0x024);
CHECK_OFFSET(cUIState, mpScreen, 0x040);
CHECK_OFFSET(cUIState, mUserNum, 0x044);
CHECK_SIZE(cUIState, 0x48);

#endif /* UI_UISTATE_H */
