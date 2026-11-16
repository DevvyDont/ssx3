#ifndef UI_UISCREEN_H
#define UI_UISCREEN_H

// cUIScreen: one screen of UI objects loaded from a screen record, owned by a cUIState (cUIState::mScreens).
// cUIEngine_addScreenByHashName creates it with `cMemMan_alloc(0xD4, "UIScreen", ...)` and constructs it inline
// (the same stores as func_003A5C08, the cUIScreen(cUIState*) constructor): 0xD4 bytes.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Base class: cListNode (On Tour: mpPrev 0x0, mpNext 0x4; its vptr follows at 0x8). _vt$9cUIScreen (D_00494670)
// is cListNode's 3 slots with cUIScreen's destructor in slot 1.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cUIScreen), placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors; retail keeps out-of-line copies of them in the preview's order in
//       ui/uivector (func_003A5C98 getHashName ... func_003A5D58 getParentState), which places each one: unshifted up
//       to the root group, + 8 after it (the embedded cUIGroup grew from 0x88 to 0x90 bytes, as cUIThing did);
//   [R] retail's constructors (func_003A5B98, func_003A5C08), destructor (cUIScreen__cUIScreen) and cUIScreen_setData;
//   [U] how retail code uses the field;
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [P] or [R].
// Retail = On Tour up to mRootGroup; mpState is at 0xD0 (On Tour 0xE0: its cUIGroup is 0xA0 bytes).
//
// mAttrScreen is EA's tAttrScreen (On Tour, LSB first): mFocused:1 mBufferAlloc:1. EA's type is 2 bytes (its constructor
// func_001DD580 uses lhu/sh), but retail methods read and write the aligned word (lw/sw 0x14), so it is an int here.
// The two embedded objects are kept as raw regions (no cList / cUIGroup layout yet):
//   mThreads   = cList (0x1C: mStart node 0x18, mEnd node 0x24, cList vptr at 0x30);
//   mRootGroup = cUIGroup (0x90: a cUIThing 0x40-0xB4 with its vptr at 0x48 and tAttrThing at 0x54, then its cList
//                mList at 0xB4-0xD0). getRootGroup returns &mRootGroup; isVisible/setVisible are bit 6 of 0x54.

#include "layout.h"

struct cListNode;
struct cUIState;
struct cUIThing;
struct tUIScreenHeader;
struct tUIKeyframeBlock;
struct tUIObjectBlock;

struct cUIScreen {
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructors self-link it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] cUIState_updateScreens walks screens by it
    void** mVtbl;                    // 0x008 [R] _vt$9cUIScreen (D_00494670), stored by func_003A5B98/func_003A5C08/cUIScreen__cUIScreen
    char* mpBuffer;                  // 0x00C [O]; [R] = 0; cUIScreen_setData stores its "UIScreenData" copy of the record here;
                                     //       cUIScreen__cUIScreen frees it when mAttrScreen bit 1 (mBufferAlloc) is set
    tUIScreenHeader* mpScreenHeader; // 0x010 [P] getHashName = mpScreenHeader's first word (preview lw 0x10; retail func_003A5C98);
                                     //       [R] = 0; cUIScreen_setData stores the record
    int mAttrScreen;                 // 0x014 [P] tAttrScreen: hasFocus/setFocus (bit 0, lw/sw 0x14; retail func_003A5CC0/func_003A5CD0);
                                     //       [R] tAttrScreen ctor func_001DD580(this + 0x14); [U] bit 1 set/cleared by cUIScreen_setData
    char mThreads[0x1C];             // 0x018 [O] cList of cUIThread; [U] cUIScreen_playFrame appends a "UIThread",
                                     //       cUIScreen_getPrimaryThread walks it; [R] the destructor tears down its vptrs (0x20/0x2C/0x30)
    tUIKeyframeBlock* mpKeyframeBlock; // 0x034 [O]; [R] = 0; cUIScreen_setData = record + record[2]; [U] the frame list searched by
                                     //       cUIScreen_getFrameByLabel and func_0039C728 (getFrameByIndex)
    tUIObjectBlock* mpObjectBlock;   // 0x038 [O]; [R] = 0; cUIScreen_setData = record + record[1]; [U] object count + records read by
                                     //       cUIScreen_createAllObjects, func_0039D008, cUIScreen_getObjectByHashName
    cUIThing** mppObjectList;        // 0x03C [O]; [R] = 0; [U] cUIScreen_createAllObjects allocates count * 4 ("UIScreenObjs");
                                     //       cUIScreen_getObjectByHashName searches it; the destructor frees it
    char mRootGroup[0x90];           // 0x040 [P] getRootGroup (preview addiu 0x40; retail func_003A5D10), rootGroupPushBack appends to its
                                     //       list at 0xB4 (preview 0xAC; retail func_003A5CF0); [R] cUIThing ctor + _vt$8cUIGroup
                                     //       (D_004949E8) at 0x48; [U] cUIScreen_onInput/cUIScreen_update/cUIScreen_parseScreenFocusLose
                                     //       pass it to cUIGroup_onInput/cUIGroup_update/cUIGroup_setFocus
    cUIState* mpState;               // 0x0D0 [P] getParentState (preview lw 0xC8; retail func_003A5D58 lw 0xD0), getEngine = mpState's
                                     //       mpEngine (retail func_003A5D48); [R] = pState (func_003A5C08, cUIEngine_addScreenByHashName)
};

CHECK_OFFSET(cUIScreen, mpPrev, 0x000);
CHECK_OFFSET(cUIScreen, mpNext, 0x004);
CHECK_OFFSET(cUIScreen, mVtbl, 0x008);
CHECK_OFFSET(cUIScreen, mpBuffer, 0x00C);
CHECK_OFFSET(cUIScreen, mpScreenHeader, 0x010);
CHECK_OFFSET(cUIScreen, mAttrScreen, 0x014);
CHECK_OFFSET(cUIScreen, mThreads, 0x018);
CHECK_OFFSET(cUIScreen, mpKeyframeBlock, 0x034);
CHECK_OFFSET(cUIScreen, mpObjectBlock, 0x038);
CHECK_OFFSET(cUIScreen, mppObjectList, 0x03C);
CHECK_OFFSET(cUIScreen, mRootGroup, 0x040);
CHECK_OFFSET(cUIScreen, mpState, 0x0D0);
CHECK_SIZE(cUIScreen, 0xD4);

#endif /* UI_UISCREEN_H */
