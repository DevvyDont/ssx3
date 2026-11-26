#ifndef UI_UISCREEN_H
#define UI_UISCREEN_H

// cUIScreen: one screen of UI objects loaded from a screen record, owned by a cUIState (cUIState::mScreens).
// cUIEngine_addScreenByHashName creates it with `cMemMan_alloc(0xD4, "UIScreen", ...)` and constructs it inline
// (the same stores as cUIScreen_cUIScreen, the cUIScreen(cUIState*) constructor): 0xD4 bytes.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Base class: cListNode (On Tour: mpPrev 0x0, mpNext 0x4; its vptr follows at 0x8). _vt$9cUIScreen (cUIScreen_vtbl)
// is cListNode's 3 slots with cUIScreen's destructor in slot 1.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cUIScreen), placed at retail offsets by:
//   [P] the July 2003 preview's inline accessors; retail keeps out-of-line copies of them in the preview's order in
//       ui/uivector (cUIScreen_getHashName getHashName ... cUIScreen_getParentState getParentState), which places each one: unshifted up
//       to the root group, + 8 after it (the embedded cUIGroup grew from 0x88 to 0x90 bytes, as cUIThing did);
//   [R] retail's constructors (cUIScreen_cUIScreen1, cUIScreen_cUIScreen), destructor (cUIScreen__cUIScreen) and cUIScreen_setData;
//   [U] how retail code uses the field;
//   [O] On Tour's member order alone, where the fields on both sides of it are placed by [P] or [R].
// Retail = On Tour up to mRootGroup; mpState is at 0xD0 (On Tour 0xE0: its cUIGroup is 0xA0 bytes).
//
// mAttrScreen is EA's tAttrScreen (On Tour, LSB first): mFocused:1 mBufferAlloc:1. EA's type is 2 bytes (its constructor
// tAttrScreen_tAttrScreen uses lhu/sh), but retail methods read and write the aligned word (lw/sw 0x14), so it is an int here.
// The two embedded objects are kept as raw regions (no cList / cUIGroup layout yet):
//   mThreads   = cList (0x1C: mStart node 0x18, mEnd node 0x24, cList vptr at 0x30);
//   mRootGroup = cUIGroup (0x90: a cUIThing 0x40-0xB4 with its vptr at 0x48 and tAttrThing at 0x54, then its cList
//                mList at 0xB4-0xD0). getRootGroup returns &mRootGroup; isVisible/setVisible are bit 6 of 0x54.

#include "layout.h"
#include "ui/uigroup.h"
#include "util/list.h"

struct cListNode;
struct cUIState;
struct cUIThing;
struct tUIScreenHeader;
struct tUIKeyframeBlock;
struct tUIObjectBlock;

struct tUIBaseObject;
struct tUIFrameHeader;
struct tUIKCPObjectAnimate;

struct cUIThread;
struct tUIKCPKeyframeJump;

struct cUIScreen {
    cListNode* mpPrev;               // 0x000 [O] cListNode::mpPrev; [R] the constructors self-link it
    cListNode* mpNext;               // 0x004 [O] cListNode::mpNext; [R] self-linked; [U] cUIState_updateScreens walks screens by it
    void** mVtbl;                    // 0x008 [R] _vt$9cUIScreen (cUIScreen_vtbl), stored by cUIScreen_cUIScreen1/cUIScreen_cUIScreen/cUIScreen__cUIScreen
    char* mpBuffer;                  // 0x00C [O]; [R] = 0; cUIScreen_setData stores its "UIScreenData" copy of the record here;
                                     //       cUIScreen__cUIScreen frees it when mAttrScreen bit 1 (mBufferAlloc) is set
    tUIScreenHeader* mpScreenHeader; // 0x010 [P] getHashName = mpScreenHeader's first word (preview lw 0x10; retail cUIScreen_getHashName);
                                     //       [R] = 0; cUIScreen_setData stores the record
    int mAttrScreen;                 // 0x014 [P] tAttrScreen: hasFocus/setFocus (bit 0, lw/sw 0x14; retail cUIScreen_hasFocus/cUIScreen_setFocus);
                                     //       [R] tAttrScreen ctor tAttrScreen_tAttrScreen(this + 0x14); [U] bit 1 set/cleared by cUIScreen_setData
    cList mThreads;                  // 0x018 [O] cList of cUIThread; [U] cUIScreen_playFrame appends a "UIThread",
                                     //       cUIScreen_getPrimaryThread walks it; [R] the destructor tears down its vptrs (0x20/0x2C/0x30)
    tUIKeyframeBlock* mpKeyframeBlock; // 0x034 [O]; [R] = 0; cUIScreen_setData = record + record[2]; [U] the frame list searched by
                                     //       cUIScreen_getFrameByLabel and cUIScreen_getFrameByIndex
    tUIObjectBlock* mpObjectBlock;   // 0x038 [O]; [R] = 0; cUIScreen_setData = record + record[1]; [U] object count + records read by
                                     //       cUIScreen_createAllObjects, cUIScreen_createObjectByHashName, cUIScreen_getObjectByHashName
    cUIThing** mppObjectList;        // 0x03C [O]; [R] = 0; [U] cUIScreen_createAllObjects allocates count * 4 ("UIScreenObjs");
                                     //       cUIScreen_getObjectByHashName searches it; the destructor frees it
    cUIGroup mRootGroup;             // 0x040 [P] getRootGroup (preview addiu 0x40; retail cUIScreen_getRootGroup), rootGroupPushBack appends to its
                                     //       list at 0xB4 (preview 0xAC; retail cUIScreen_rootGroupPushBack); [R] cUIThing ctor + _vt$8cUIGroup
                                     //       (cUIGroup_vtbl) at 0x48; [U] cUIScreen_onInput/cUIScreen_update/cUIScreen_parseScreenFocusLose
                                     //       pass it to cUIGroup_onInput/cUIGroup_update/cUIGroup_setFocus
    cUIState* mpState;               // 0x0D0 [P] getParentState (preview lw 0xC8; retail cUIScreen_getParentState lw 0xD0), getEngine = mpState's
                                     //       mpEngine (retail cUIScreen_getEngine); [R] = pState (cUIScreen_cUIScreen, cUIEngine_addScreenByHashName)

    // The "no frame" frame number (names ours). [U] cUIScreen_getFrameByLabel returns it for a label the screen does
    // not have and cUIThing_getKeyframerEvent for a thing without events; callers then skip cUIScreen_playFrame.
    enum { kNoFrame = 0xFFFF };

    // Methods bound to their retail symbols by asm labels.
    void setData(tUIScreenHeader* data) __asm__("cUIScreen_setData");
    tUIFrameHeader* getFrameByIndex(unsigned short frame) __asm__("cUIScreen_getFrameByIndex");
    unsigned short getFrameByLabel(int label) __asm__("cUIScreen_getFrameByLabel__FP9cUIScreeni");
    void parseObjectAnim(tUIFrameHeader* pFrame, tUIKCPObjectAnimate* msg) __asm__("cUIScreen_parseObjectAnim");
    void parseObjectSetPos(int pFrame, int* msg) __asm__("cUIScreen_parseObjectSetPos");
    void parseEventTrigger(void* pFrame, int* msg) __asm__("cUIScreen_parseEventTrigger");
    void parseScreenFocusGain() __asm__("cUIScreen_parseScreenFocusGain");
    void parseScreenFocusLose() __asm__("cUIScreen_parseScreenFocusLose");
    cUIThing* createObjectByStruct(tUIBaseObject* rec) __asm__("cUIScreen_createObjectByStruct");
    void* onInput() __asm__("cUIScreen_onInput__FPv");
    void update() __asm__("cUIScreen_update");
    cUIThing* getObjectByHashName(int hash) __asm__("cUIScreen_getObjectByHashName");
    void* cUIScreen1() __asm__("cUIScreen_cUIScreen1");
    int getHashName() __asm__("cUIScreen_getHashName");
    int hasFocus() __asm__("cUIScreen_hasFocus");
    void setFocus(int focused) __asm__("cUIScreen_setFocus");
    void setVisible(int enable) __asm__("cUIScreen_setVisible");
    int isVisible() __asm__("cUIScreen_isVisible");
    cUIEngine* getEngine() __asm__("cUIScreen_getEngine");
    cUIState* getParentState() __asm__("cUIScreen_getParentState");
    void parseObjectVisible() __asm__("cUIScreen_parseObjectVisible__FPv");
    void parseEventAudio() __asm__("cUIScreen_parseEventAudio__FPv");
    void parseScreenStart() __asm__("cUIScreen_parseScreenStart__FPv");
    cUIThread* getPrimaryThread() __asm__("cUIScreen_getPrimaryThread");
    void jumpToFrame(unsigned short frame) __asm__("cUIScreen_jumpToFrame__FPvUs");
    void playFrameByThread(cUIThread* thread) __asm__("cUIScreen_playFrameByThread");
    void parseKeyframePause(void* pFrame, void* pKCP, cUIThread* pThread) __asm__("cUIScreen_parseKeyframePause");
    void parseKeyframeJump(void* pFrame, tUIKCPKeyframeJump* pKCP, cUIThread* pThread)
        __asm__("cUIScreen_parseKeyframeJump");
    void parseKeyframeSpawn(void* pFrame, tUIKCPKeyframeJump* pKCP) __asm__("cUIScreen_parseKeyframeSpawn");
    void parseScreenEnd(void* pFrame, void* pKCP, cUIThread* pThread) __asm__("cUIScreen_parseScreenEnd");
    void createAllObjects() __asm__("cUIScreen_createAllObjects");
    void* createObjectByHashName(int key) __asm__("cUIScreen_createObjectByHashName");
    void* playFrame(unsigned short frame, int ThreadType) __asm__("cUIScreen_playFrame");
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


// ---------------------------------------------------------------------------------------------------------------------
// cUIThread: one animation thread of a UI screen (cUIScreen::mThreads): the frame it is on, the last frame evaluated, a
// stop frame, the step, and active / primary flags. cUIScreen_playFrame creates it (one primary thread per screen,
// replacing the old one; extra threads otherwise), the keyframe handlers stop it, cUIScreen_getPrimaryThread finds the
// primary one. EA's source: d:/ssx/code/source/ui/uiscreen.cpp (the preview's assert strings name the unit). Retail
// code: deleteThread in ui/uiscreen (0x0039C518), the dtor in ui/uivector (0x003A5B48); the ctor and the accessors are
// inlined.
//
// Size 0x14: [R] cUIScreen_playFrame: cMemMan_alloc(0x14, "UIThread") and the inlined ctor (stores up to 0x13, the
// flag byte, through the word at 0x10); [O] On Tour's class is 0x14 with the same members at the same offsets.
// Base: cListNode (mpPrev / mpNext, vptr at 0x8). Vtable cUIThread_vtbl cUIThread_vtbl (3 slots: 1 dtor, 2 / 3
// cListNode_traverseFlat / traverseRecurse); the dtor stores cListNode_vtbl and runs cListNode_destroyListNode.
// The two flags are On Tour's `unsigned char mIsActive : 1, mIsPrimary : 1` at 0x13, but retail reads and writes them
// as bits 24 / 25 of the word at 0x10 (lw / and / or / sw), so they are int bitfields here, as in src/ui/uiscreen.cpp's
// matching view cUIThread_C870 (a bitfield has no CHECK_OFFSET; CHECK_SIZE covers it).
// A FLAT layout, like main/game.h. Evidence tags: [R] the inlined ctor, the dtor and the allocation; [U] how retail
// code uses the field; [O] SSX On Tour's DWARF (class cUIThread): names, types and order.

struct cUIThread {
    // --- cListNode
    cListNode* mpPrev;              // 0x000 [O] cListNode::mpPrev; [R] the inlined ctor links the node to itself, then
                                    //       cList_addToEnd(&screen->mThreads)
    cListNode* mpNext;              // 0x004 [O] cListNode::mpNext; [R] ditto; [U] cUIScreen_getPrimaryThread walks the
                                    //       list through it
    void** mVtbl;                   // 0x008 [R] cUIThread_vtbl cUIThread_vtbl (playFrame), cListNode_vtbl (dtor); [U]
                                    //       deleteThread calls slot 1 (dtor, flags 3)
    // --- cUIThread
    unsigned short mCurrentFrameNumber; // 0x00C [O]; [R] playFrame's frame (sh)
    unsigned short mLastFrameNumber; // 0x00E [O]; [R] 0xFFFF (sh)
    unsigned short mStopFrameNumber; // 0x010 [O]; [R] 0xFFFF (sh)
    signed char mFrameIncrement;    // 0x012 [O]; [R] 1 (sb)
    unsigned int mIsActive : 1;     // 0x013 bit 0 (bit 24 of the word at 0x10) [O]; [R] playFrame sets it; [U] the
                                    //       keyframe stop handlers clear it (&= 0xFEFFFFFF)
    unsigned int mIsPrimary : 1;    // 0x013 bit 1 (bit 25 of the word at 0x10) [O]; [R] playFrame: ThreadType == 0; [U]
                                    //       cUIScreen_getPrimaryThread tests it

    // [O] On Tour's inline accessors (cUIThread::getCurrentFrameNumber ... setPrimary); retail inlines them in
    // cUIScreen_playFrame / cUIScreen_playFrameByThread.
    unsigned short getCurrentFrameNumber() { return mCurrentFrameNumber; }
    signed char getFrameIncrement() { return mFrameIncrement; }
    void setCurrentFrameNumber(unsigned short v) { mCurrentFrameNumber = v; }
    void setLastFrameNumber(unsigned short v) { mLastFrameNumber = v; }
    void setStopFrameNumber(unsigned short v) { mStopFrameNumber = v; }
    void setFrameIncrement(signed char v) { mFrameIncrement = v; }
    void setActive(bool b) { mIsActive = b; }
    void setPrimary(bool b) { mIsPrimary = b; }

    // Methods bound to their retail symbols by asm labels.
    void deleteThread() __asm__("cUIThread_deleteThread__FPv");
};

CHECK_OFFSET(cUIThread, mpPrev, 0x000);
CHECK_OFFSET(cUIThread, mpNext, 0x004);
CHECK_OFFSET(cUIThread, mVtbl, 0x008);
CHECK_OFFSET(cUIThread, mCurrentFrameNumber, 0x00C);
CHECK_OFFSET(cUIThread, mLastFrameNumber, 0x00E);
CHECK_OFFSET(cUIThread, mStopFrameNumber, 0x010);
CHECK_OFFSET(cUIThread, mFrameIncrement, 0x012);
CHECK_SIZE(cUIThread, 0x14);

#endif /* UI_UISCREEN_H */
