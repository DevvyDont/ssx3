#include "vtable.h"
#include "common.h"
#include "bx/math3d.h"
#include "ui/uiicon.h"
#include "ui/uilistbox.h"
#include "ui/uimenu.h"
#include "ui/uipair.h"
#include "ui/uiprogress.h"
#include "ui/uitext.h"
#include "ui/uitextscroll.h"
#include "ui/uivector.h"
#include "ui/uidata.h"
#include "ui/uithing.h"
#include "ui/uiinput.h"
#include "ui/uiengine.h"
#include "ui/uislider.h"
#include "ui/uiscrollbar.h"
#include "ui/uistate.h"
#include "ui/uiscreen.h"

extern "C" void cListNode_removeFromList(void* self);

//100%
INCLUDE_ASM("ui/uiscreen", cUIThread_deleteThread__FPv);
#ifdef SKIP_ASM
/**
 * Unlinks an animation thread from its screen's thread list and deletes it.
 */
void cUIThread::deleteThread()
{
    cListNode_removeFromList(this);
    if (((void*)this) != 0) {
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)mVtbl;
        vt[1].fn(vthis(this, vt[1]), 3); // slot 1: the deleting destructor
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen__cUIScreen);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cList_destroyList_C558(void*) __asm__("cList_destroyList");
extern "C" void cListNode_destroyListNode(void*);
extern "C" void cUIThing__cUIThing(void* self, int flags);
extern "C" void cMemMan_free__FPv(void* p);
void operator_delete(int*);
extern void* cUIScreen_vtbl[];
extern void* cUIGroup_vtbl[];
extern void* cList_vtbl[];
extern void* cListNode_vtbl[];

/**
 * Destroys a UI screen: frees its object table and its own copy of the screen data, then the embedded lists
 * and group.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cUIScreen__cUIScreen(cUIScreen* self, int flags)
{
    self->mVtbl = cUIScreen_vtbl;
    if ((void*)self->mppObjectList != 0) {
        cMemMan_free__FPv(self->mppObjectList);
    }
    if ((void*)self->mpBuffer != 0 && (self->mAttrScreen & 2)) {
        cMemMan_free__FPv(self->mpBuffer);
    }
    VPTR_OF(cList, &self->mRootGroup.mList) = cList_vtbl;
    VPTR_OF(cListNode, &self->mRootGroup) = cUIGroup_vtbl;
    cList_destroyList_C558(&self->mRootGroup.mList);
    char* rootGroup = (char*)&self->mRootGroup;
    VPTR_OF(cListNode, &self->mRootGroup.mList.mEnd) = cListNode_vtbl;
    cListNode_destroyListNode(&self->mRootGroup.mList.mEnd);
    VPTR_OF(cListNode, &self->mRootGroup.mList.mStart) = cListNode_vtbl;
    cListNode_destroyListNode(&self->mRootGroup.mList);
    cUIThing__cUIThing(rootGroup, 2);
    VPTR_OF(cList, &self->mThreads) = cList_vtbl;
    cList_destroyList_C558(&self->mThreads);
    VPTR_OF(cListNode, &self->mThreads.mEnd) = cListNode_vtbl;
    cListNode_destroyListNode(&self->mThreads.mEnd);
    VPTR_OF(cListNode, &self->mThreads.mStart) = cListNode_vtbl;
    cListNode_destroyListNode(&self->mThreads);
    self->mVtbl = cListNode_vtbl;
    cListNode_destroyListNode(((char*)self));
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_setData);
#ifdef SKIP_ASM
extern "C" int unpacksizez(void* data);
extern "C" void* unpack(void* src, void* dst);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_UIScreenData[];

/**
 * Points the screen at its screen data, unpacking it first into a new "UIScreenData" buffer when it is
 * compressed.
 *
 * Sets mBufferAlloc when a buffer was allocated, and finds the keyframe and object blocks from the header.
 *
 * @param data  the screen data (On Tour tUIScreenHeader* pScreenHeader)
 */
void cUIScreen::setData(tUIScreenHeader* data)
{
    int size = unpacksizez(data);
    if (size != 0) {
        char* buf = (char*)operator_new_tag(size, str_UIScreenData, 0x100, 0);
        mpBuffer = buf;
        tUIScreenHeader* old = data;
        data = (tUIScreenHeader*)buf;
        unpack(old, data);
        mAttrScreen |= 2;
    } else {
        mAttrScreen &= ~2;
    }
    mpScreenHeader = data;
    // ORIGINAL-PTR: the screen data is a file blob; its header holds the byte offsets of the keyframe and object blocks
    mpKeyframeBlock = (tUIKeyframeBlock*)((char*)data + data->mFrameBlockOffset);
    mpObjectBlock = (tUIObjectBlock*)((char*)data + data->mObjectBlockOffset);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_getFrameByIndex);
#ifdef SKIP_ASM
/**
 * Finds a screen's keyframe header by frame number.
 *
 * The keyframe block (mpKeyframeBlock, +0x34) starts with the frame count; each header follows the previous one by its
 * mByteSize (+0x4). Returns the first header whose 16-bit mFrameNumber (+0x8) equals @p frame.
 *
 * @param frame  the frame number to find (On Tour index)
 * @return the frame header (On Tour tUIFrameHeader), or 0
 */
tUIFrameHeader* cUIScreen::getFrameByIndex(unsigned short frame)
{
    tUIKeyframeBlock* list = mpKeyframeBlock;
    // ORIGINAL-PTR: the frames follow the block's count and are variable-length records (each mByteSize bytes)
    tUIFrameHeader* frameHeader = (tUIFrameHeader*)(list + 1);
    unsigned int i;
    for (i = 0; i < list->mNumFrames; i++) {
        if (frameHeader->mFrameNumber == frame) {
            return frameHeader;
        }
        frameHeader = (tUIFrameHeader*)(((char*)frameHeader) + (int)frameHeader->mByteSize);
    }
    return 0;
}
#endif


//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_getFrameByLabel__FP9cUIScreeni);
#ifdef SKIP_ASM
/**
 * Looks up a keyframe label's frame number in the screen's keyframe block.
 *
 * @param label  the label's hash (On Tour hashname)
 * @return the frame number, or kNoFrame (0xFFFF) when the label is not found
 */
unsigned short cUIScreen::getFrameByLabel(int label)
{
    tUIKeyframeBlock* list = mpKeyframeBlock;
    // ORIGINAL-PTR: the frames follow the block's count and are variable-length records (each mByteSize bytes)
    tUIFrameHeader* entry = (tUIFrameHeader*)(list + 1);
    unsigned int i;
    for (i = 0; i < list->mNumFrames; i++) {
        if (((int)entry->mHashID) == label) {
            return entry->mFrameNumber;
        }
        entry = (tUIFrameHeader*)((char*)entry + ((int)entry->mByteSize));
    }
    return kNoFrame;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_getPrimaryThread);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

/**
 * Returns the screen's primary animation thread: the first one in mThreads with mIsPrimary (bit 25 of its word
 * at +0x10) set.
 *
 * @return that thread, or 0 when there is none
 */
cUIThread* cUIScreen::getPrimaryThread()
{
    cUIThread* n = (cUIThread*)cList_first(&mThreads);
    if (n != 0) {
        for (; !cListNode_isSentinel((cListNode*)n); n = (cUIThread*)n->mpNext) {
            if ((*(int*)&n->mStopFrameNumber >> 25) & 1) { // mIsPrimary
                return n;
            }
        }
    }
    return 0;
}
#endif

extern "C" cUIThread* cUIScreen_getPrimaryThread(cUIScreen* self);
void cUIThread_deleteThread(void* self);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);

//99.75%
INCLUDE_ASM("ui/uiscreen", cUIScreen_jumpToFrame__FPvUs);
#ifdef SKIP_ASM
/**
 * Restarts the screen's animation at a given frame.
 *
 * Deletes the primary animation thread if there is one, then calls cUIScreen_playFrame(self, frame, 0).
 *
 * @param frame  frame to play from
 */
void cUIScreen::jumpToFrame(unsigned short frame)
{
    void* thread = getPrimaryThread();
    if (thread != 0) {
        cUIThread_deleteThread(thread);
    }
    cUIScreen_playFrame(this, frame, 0);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_playFrame);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void cList_addToEnd(cList*, cListNode*);
// PORT: cMemMan_alloc is the game's tagged operator new(size, tag, flags, d); bound by asm label.
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" cUIThread* cUIScreen_getPrimaryThread(cUIScreen* self);
void cUIThread_deleteThread(void* self);
extern char str_UIThread[];
extern void* cUIThread_vtbl[];

// PORT: the unit declares cUIScreen_playFrame as void; the body returns the new thread. Bound by asm label.

/**
 * Starts an animation thread at a frame; a primary thread (type 0) replaces the screen's current primary
 * thread.
 *
 * @param frame  the frame to start at
 * @param ThreadType  0 for the primary thread, otherwise an extra thread
 * @return the new thread
 */
void* cUIScreen::playFrame(unsigned short frame, int ThreadType)
{
    if (ThreadType == 0) {
        void* pPrimary = getPrimaryThread();
        if (pPrimary != 0) {
            cUIThread_deleteThread(pPrimary);
        }
    }
    cUIThread* pNewThread = new (str_UIThread, 0x100, 0) cUIThread;
    // inlined cUIThread ctor: direct member stores (the target writes these twice: the
    // second pair goes through the setters, whose promoted int params keep separate constants)
    pNewThread->mStopFrameNumber = 0xFFFF;
    pNewThread->mFrameIncrement = 1;
    pNewThread->mIsActive = 0;
    pNewThread->mIsPrimary = 0;
    pNewThread->setFrameIncrement(1);
    pNewThread->setStopFrameNumber(0xFFFF);
    pNewThread->mVtbl = cUIThread_vtbl;
    pNewThread->setCurrentFrameNumber(frame);
    pNewThread->setLastFrameNumber(0xFFFF);
    // same layout: a cUIThread is a cListNode (flat here: mpPrev, mpNext and the vptr come first)
    pNewThread->mpNext = (cListNode*)pNewThread;
    pNewThread->mpPrev = (cListNode*)pNewThread;
    pNewThread->setActive(true);
    pNewThread->setPrimary(ThreadType == 0);
    cList_addToEnd((cList*)(&mThreads), (cListNode*)pNewThread);
    return pNewThread;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_playFrameByThread);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// Command handlers: every one is called with (self, list, cmd, thread); the unit's definitions declare only the
// parameters each reads, so they're bound here by asm label with the full argument list.
extern void cUIScreen_parseKeyframePause_4(void *, void *, void *, void *) __asm__("cUIScreen_parseKeyframePause");
extern void cUIScreen_parseKeyframeJump_4(void *, void *, void *, void *) __asm__("cUIScreen_parseKeyframeJump");
extern void cUIScreen_parseKeyframeSpawn_4(void *, void *, void *, void *) __asm__("cUIScreen_parseKeyframeSpawn");
extern void cUIScreen_parseObjectAnim_4(void *, void *, void *, void *) __asm__("cUIScreen_parseObjectAnim");
extern void cUIScreen_parseObjectSetPos_4(void *, void *, void *, void *) __asm__("cUIScreen_parseObjectSetPos");
extern void cUIScreen_parseObjectVisible_4(void *, void *, void *, void *) __asm__("cUIScreen_parseObjectVisible__FPv");
extern void cUIScreen_parseEventAudio_4(void *, void *, void *, void *) __asm__("cUIScreen_parseEventAudio__FPv");
extern void cUIScreen_parseEventTrigger_4(void *, void *, void *, void *) __asm__("cUIScreen_parseEventTrigger");
extern void cUIScreen_parseScreenFocusGain_4(void *, void *, void *, void *) __asm__("cUIScreen_parseScreenFocusGain");
extern void cUIScreen_parseScreenFocusLose_4(void *, void *, void *, void *) __asm__("cUIScreen_parseScreenFocusLose");
extern void cUIScreen_parseScreenStart_4(void *, void *, void *, void *) __asm__("cUIScreen_parseScreenStart__FPv");
extern void cUIScreen_parseScreenEnd_4(void *, void *, void *, void *) __asm__("cUIScreen_parseScreenEnd");

/**
 * Advances a thread one frame (toward its stop frame, if any) and runs the commands of the frame it was on.
 *
 * Commands (tUIKCPBase::eKCPTag) 0x10-0x12 are keyframe stop, jump and spawn, 0x2x object and 0x3x event commands,
 * 0x40-0x43 screen and focus events.
 *
 * @param thread  the thread (On Tour cUIThread* pThread)
 */
void cUIScreen::playFrameByThread(cUIThread* thread)
{
    cUIThread *uiThread = thread;
    tUIFrameHeader *list;
    tUIKCPBase *cmd;
    unsigned int i;
    unsigned short CurrentFrame = uiThread->getCurrentFrameNumber();
    uiThread->setLastFrameNumber(uiThread->getCurrentFrameNumber());
    if (uiThread->mStopFrameNumber != cUIScreen::kNoFrame) {
        if (uiThread->getFrameIncrement() > 0) {
            if (uiThread->mLastFrameNumber < uiThread->mStopFrameNumber) {
                uiThread->mCurrentFrameNumber = uiThread->getCurrentFrameNumber() + uiThread->getFrameIncrement();
            }
        } else if (uiThread->getFrameIncrement() < 0) {
            if (uiThread->mLastFrameNumber > uiThread->mStopFrameNumber) {
                uiThread->mCurrentFrameNumber = uiThread->getCurrentFrameNumber() + uiThread->getFrameIncrement();
            }
        }
    } else {
        uiThread->mCurrentFrameNumber = uiThread->getCurrentFrameNumber() + uiThread->getFrameIncrement();
    }
    list = getFrameByIndex(CurrentFrame);
    // ORIGINAL-PTR: the frame's key-channel packets follow its header in the loaded screen data and are
    // variable-length (each mPacketSize bytes), so they are walked by byte offsets
    cmd = (tUIKCPBase *)(list + 1);
    if (list != 0) {
        i = 0;
        if (list->mNumKCP != 0) {
            do {
                switch (cmd->mTag) {
                case tUIKCPBase::kKCP_KeyframePause:
                    cUIScreen_parseKeyframePause_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_KeyframeJump:
                    cUIScreen_parseKeyframeJump_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_KeyframeSpawn:
                    cUIScreen_parseKeyframeSpawn_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_ObjectAnim:
                    cUIScreen_parseObjectAnim_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_ObjectSetPos:
                    cUIScreen_parseObjectSetPos_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_ObjectVisible:
                    cUIScreen_parseObjectVisible_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_EventTrigger:
                    cUIScreen_parseEventTrigger_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_EventAudio:
                    cUIScreen_parseEventAudio_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_ScreenStart:
                    cUIScreen_parseScreenStart_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_ScreenEnd:
                    cUIScreen_parseScreenEnd_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_ScreenFocusGain:
                    cUIScreen_parseScreenFocusGain_4(this, list, cmd, uiThread);
                    break;
                case tUIKCPBase::kKCP_ScreenFocusLose:
                    cUIScreen_parseScreenFocusLose_4(this, list, cmd, uiThread);
                    break;
                }
                i++;
                cmd = (tUIKCPBase *)((char *)cmd + cmd->mPacketSize);
            } while (i < list->mNumKCP);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseKeyframePause);
#ifdef SKIP_ASM
/**
 * Handles a keyframe pause command by stopping the playing thread.
 *
 * Clears bit 24 of the flags at +0x10 of the thread @p pThread; the frame and command are unused.
 *
 * @param pFrame  the frame header holding the command (On Tour pFrame)
 * @param pKCP  the command (On Tour pKCP)
 * @param pThread  the animation thread being played (On Tour pThread)
 */
void cUIScreen::parseKeyframePause(void* pFrame, void* pKCP, cUIThread* pThread)
{
    *(unsigned int*)&pThread->mStopFrameNumber &= 0xFEFFFFFFU;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseKeyframeJump);
#ifdef SKIP_ASM
/**
 * Runs a keyframe jump command: moves the thread to the frame with the command's label.
 *
 * @param pFrame  the frame's command list (unused)
 * @param pKCP  the jump command; its label hash is mFrameHashName
 * @param pThread  the thread to move
 */
void cUIScreen::parseKeyframeJump(void* pFrame, tUIKCPKeyframeJump* pKCP, cUIThread* pThread)
{
    // ORIGINAL-PTR: the frames follow the block's count and are variable-length records (each mByteSize bytes)
    tUIFrameHeader* entry = (tUIFrameHeader*)(mpKeyframeBlock + 1);
    unsigned int i;
    for (i = 0; i < mpKeyframeBlock->mNumFrames; i++) {
        if (entry->mHashID == pKCP->mFrameHashName) {
            pThread->mCurrentFrameNumber = entry->mFrameNumber;
            return;
        }
        entry = (tUIFrameHeader*)((char*)entry + ((int)entry->mByteSize));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseKeyframeSpawn);
#ifdef SKIP_ASM
/**
 * Runs a keyframe spawn command: starts an extra thread at the frame with the command's label.
 *
 * @param pFrame  the frame's command list (unused; On Tour pFrame)
 * @param pKCP  the spawn command; its label hash is mFrameHashName (On Tour pKCP)
 */
void cUIScreen::parseKeyframeSpawn(void* pFrame, tUIKCPKeyframeJump* pKCP)
{
    // ORIGINAL-PTR: the frames follow the block's count and are variable-length records (each mByteSize bytes)
    tUIFrameHeader* entry = (tUIFrameHeader*)(mpKeyframeBlock + 1);
    unsigned int i;
    for (i = 0; i < mpKeyframeBlock->mNumFrames; i++) {
        if (entry->mHashID == pKCP->mFrameHashName) {
            cUIScreen_playFrame(this, entry->mFrameNumber, 1);
            return;
        }
        entry = (tUIFrameHeader*)((char*)entry + ((int)entry->mByteSize));
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseObjectAnim);
#ifdef SKIP_ASM
extern "C" cUIThing* cUIScreen_getObjectByHashName(cUIScreen* self, int hash);
extern "C" void* cUIAnimationBank_getAnimationByHashName(void* bank, int hash);
// PORT: callers pass a 5th arg (0) that cUIThing_playAnimation never reads
extern "C" void cUIThing_playAnimation(void* self, void* anim, unsigned char mode, unsigned short v, int x);

/**
 * Handles a keyframe command that starts an animation on one of the screen's objects.
 *
 * The command holds the object hash (+0x8), the animation hash (+0x4) and the playback mode (+0xC). When both are found
 * the animation plays on the object from the frame number at @p pFrame +0x8, not expiring.
 *
 * @param pFrame  the frame header holding the command (On Tour pFrame)
 * @param msg  the command (On Tour pKCP)
 */
void cUIScreen::parseObjectAnim(tUIFrameHeader* pFrame, tUIKCPObjectAnimate* msg)
{
    void* obj = getObjectByHashName((int)msg->mObjectHashID);
    if (obj != 0) {
        void* anim = cUIAnimationBank_getAnimationByHashName(
            &mpState->mpEngine->mAnimBank,
            (int)msg->mAnimationID);
        if (anim != 0) {
            cUIThing_playAnimation(obj, anim, *(unsigned char*)&msg->mPlaybackStatus, pFrame->mFrameNumber, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseObjectSetPos);
#ifdef SKIP_ASM
extern "C" cUIThing* cUIScreen_getObjectByHashName(cUIScreen* self, int hash);
extern "C" void cUIThing_deleteAnimation(void* self);

class cUIScreen_parseObjectSetPos_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void setChannelDataKCP(int, void*);
};

/**
 * Handles a keyframe command that sets one of the screen's objects' channel values directly.
 *
 * Finds the object named by the hash in @p msg [1], removes any running animation from it and passes the channel count
 * @p msg [2] and the values from @p msg [3] on to its virtual slot 13 (On Tour setChannelDataKCP).
 *
 * @param pFrame  the frame header holding the command (On Tour pFrame)
 * @param msg  the command words (On Tour pKCP)
 */
void cUIScreen::parseObjectSetPos(int pFrame, int* msg)
{
    cUIScreen_parseObjectSetPos_cVirt* obj = (cUIScreen_parseObjectSetPos_cVirt*)getObjectByHashName(msg[1]);
    if (obj != 0) {
        cUIThing_deleteAnimation(obj);
        obj->setChannelDataKCP(msg[2], msg + 3);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseObjectVisible__FPv);
#ifdef SKIP_ASM
/**
 * Handles a keyframe visibility command; does nothing in this build.
 */
void cUIScreen::parseObjectVisible()
{
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseEventAudio__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * cUIScreen_playFrameByThread dispatches keyframe command 0x31 to it (0x30 goes to cUIScreen_parseEventTrigger); it
 * sits between cUIScreen_parseObjectVisible__FPv and cUIScreen_parseEventTrigger.
 */
void cUIScreen::parseEventAudio()
{
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseEventTrigger);
#ifdef SKIP_ASM
class cUIScreen_parseEventTrigger_cVirtA {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual int onScreenEvent(void*, int);
};

class cUIScreen_parseEventTrigger_cVirtB {
public:
    char pad[0x4];
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int onScreenEvent(void*, int);
};

/**
 * Handles a keyframe event command by raising a screen event.
 *
 * The owning state at +0xD0 gets the event hash @p msg [1] through its virtual slot 16 (cUIState onScreenEvent); if
 * that returns 0, the object pointed to by the engine's first word (state+0xD0 -> +0x10) gets it through its virtual
 * slot 3.
 *
 * @param pFrame  the frame header holding the command (On Tour pFrame)
 * @param msg  the command words; word 1 is the event hash (On Tour pKCP)
 */
void cUIScreen::parseEventTrigger(void* pFrame, int* msg)
{
    cUIScreen_parseEventTrigger_cVirtA* state = (cUIScreen_parseEventTrigger_cVirtA*)mpState;
    if (state->onScreenEvent(this, msg[1]) == 0) {
        cUIScreen_parseEventTrigger_cVirtB* customInterface = *(cUIScreen_parseEventTrigger_cVirtB**)mpState->mpEngine;
        if (customInterface != 0) {
            customInterface->onScreenEvent(this, msg[1]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseScreenFocusGain);
#ifdef SKIP_ASM
struct sUIFlags1C {
    unsigned int lo : 8;
    unsigned int mode : 6;
};

/**
 * Handles a keyframe command that marks the owning state as having gained focus.
 *
 * If the screen has an owning state (+0xD0), sets the mode bits of its flags at +0x1C to 3.
 */
void cUIScreen::parseScreenFocusGain()
{
    cUIState* state = mpState;
    if (state != 0) {
        ((sUIFlags1C*)&state->mAttrState)->mode = 3;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseScreenFocusLose);
#ifdef SKIP_ASM
extern "C" void cUIGroup_setFocus(void*, int);

/**
 * Runs a screen focus-lose command: unfocuses the screen's root group and sets the owning state's mode to 6.
 */
void cUIScreen::parseScreenFocusLose()
{
    cUIGroup_setFocus(&mRootGroup, 0);
    cUIState* state = mpState;
    if (state != 0) {
        ((sUIFlags1C*)&state->mAttrState)->mode = 6;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseScreenStart__FPv);
#ifdef SKIP_ASM
/**
 * Handles a keyframe screen-start command; does nothing in this build.
 */
void cUIScreen::parseScreenStart()
{
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseScreenEnd);
#ifdef SKIP_ASM
/**
 * Handles a keyframe screen-end command: stops the thread and marks the owning state as finished.
 *
 * Clears bit 24 of the thread's flags at +0x10, then, if the screen has an owning state (+0xD0), sets the mode bits of
 * its flags at +0x1C to 7.
 *
 * @param pFrame  the frame header holding the command (On Tour pFrame)
 * @param pKCP  the command (On Tour pKCP)
 * @param pThread  the animation thread being played (On Tour pThread)
 */
void cUIScreen::parseScreenEnd(void* pFrame, void* pKCP, cUIThread* pThread)
{
    *(unsigned int*)&pThread->mStopFrameNumber &= 0xFEFFFFFFU;
    cUIState* state = mpState;
    if (state != 0) {
        ((sUIFlags1C*)&state->mAttrState)->mode = 7;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_createAllObjects);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void cList_addToEnd(cList*, cListNode*);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" cUIThing* cUIScreen_createObjectByStruct(cUIScreen* self, tUIBaseObject* rec);
extern "C" void* MEM_fill(void* dst, int c, int n);
extern char str_UIScreenObjs[];

/**
 * Creates every widget described in the screen's object block, registers it by name and tells the state about
 * each one.
 *
 * The objects go into a "UIScreenObjs" table and the screen's list; the state's slot 18 (onWidgetCreate) then
 * sees each one.
 */
void cUIScreen::createAllObjects()
{
    tUIObjectBlock* data = mpObjectBlock;
    // ORIGINAL-PTR: the object records follow the block's count and are variable-length (each mByteSize bytes)
    tUIBaseObject* rec = (tUIBaseObject*)(data + 1);
    if (data->mNumObjects != 0) {
        mppObjectList =
            (cUIThing**)operator_new_tag(data->mNumObjects * 4, str_UIScreenObjs, 0x20000000, 0);
        MEM_fill(mppObjectList, 0, mpObjectBlock->mNumObjects * 4);
    }
    cUIThing** out = mppObjectList;
    for (unsigned int i = 0; i < mpObjectBlock->mNumObjects; i++) {
        cUIThing* obj = createObjectByStruct(rec);
        if (obj != 0) {
            *out++ = obj;
            obj->mHashName = rec->mHashID;
            cList_addToEnd(&mRootGroup.mList, (cListNode*)obj);
        }
        rec = (tUIBaseObject*)((char*)rec + rec->mByteSize);
    }
    for (unsigned int i = 0; i < mpObjectBlock->mNumObjects; i++) {
        void* uiObj = mppObjectList[i];
        if (uiObj != 0) {
            cUIState* state = mpState;
            state->onWidgetCreate((cUIThing*)uiObj);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_createObjectByHashName);
#ifdef SKIP_ASM

/**
 * Creates the object of a UI screen whose record has hash name @p key.
 *
 * Walks the screen's object records (mpObjectBlock's mNumObjects of them, each mByteSize long, matched by mHashID);
 * the match is built with cUIScreen_createObjectByStruct and gets @p key as its mHashName.
 *
 * @param key  the object hash name
 * @return the new object, or 0 when no record matches
 */
void* cUIScreen::createObjectByHashName(int key)
{
    // ORIGINAL-PTR: the object records follow the block's count and are variable-length (each mByteSize bytes)
    tUIBaseObject* record = (tUIBaseObject*)(mpObjectBlock + 1);
    unsigned int i;
    for (i = 0; i < mpObjectBlock->mNumObjects; i++) {
        if (((int)record->mHashID) == key) {
            cUIThing* obj = createObjectByStruct(record);
            obj->mHashName = key;
            return obj;
        }
        record = (tUIBaseObject*)((char*)record + record->mByteSize);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_createObjectByStruct);
#ifdef SKIP_ASM
// PORT: cMemMan_alloc called as the game's operator new(size, tag, flags, align): gcc then treats the
// result as fresh memory (no aliasing with the record or the vtables).
void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
extern "C" void* cUIThing_cUIThing(void* self, void* screen, int a2);
extern "C" void* tAttrIcon_tAttrIcon1(void* self);
extern "C" void* cList_cList(void* self);
extern "C" void* __9tAttrMenu(void* self);
extern "C" void* __9tAttrPair(void* self);
extern "C" void* cUIListBox_cUIListBox(void* mem, void* screen, int a2);
extern "C" void* cUIText_cUIText(void* mem, void* screen, int a2);
extern "C" void* cUITextScroll_cUITextScroll(void* mem, void* screen, int a2);
extern "C" void* cUIVector_cUIVector(void* mem, void* screen, int a2);
extern char str_UIIcon[];
extern char str_UIMenu[];
extern char str_UIPair[];
extern char str_UIText[];
extern char str_cUIGroup[];
extern char str_UIListBox[];
extern char str_cUIProgress[];
extern char str_cUIScroll[];
extern char str_cUISlider[];
extern char str_cUITextScroll[];
extern char str_UIVector[];
extern void* cUISlider_vtbl[];
extern void* cUIProgress_vtbl[];
extern void* cUIScrollBar_vtbl[];
extern void* cUIPair_vtbl[];
extern void* cUIMenu_vtbl[];
extern void* cUIGroup_vtbl[];
extern void* cUIIcon_vtbl[];

static inline void* alloc_D078(unsigned int size, const char* tag)
{
    return operator new(size, tag, 0x20000000, 0);
}

/**
 * Creates the widget a screen-data record describes, by record type, and adopts the child widgets it names.
 *
 * Types (tUIBaseObject::eUIObjectType): 0x10 group, 0x11 icon, 0x12 list box, 0x13 menu, 0x15 pair, 0x16 slider, 0x17
 * text, 0x18 vector, 0x19 progress, 0x20 text scroll, 0x21 scroll. Each kind is built inline or by its constructor and
 * reads the record in its virtual setUIData; the container kinds also adopt the children their records name (tUIGroup,
 * tUIMenu, tUIPair, tUIProgress, tUIScrollBar, tUISlider, tUITextScroll in ui/uidata.h).
 *
 * @param rec  the object record (On Tour tUIBaseObject* pObject)
 * @return the new widget, or 0 for an unknown type
 */
cUIThing* cUIScreen::createObjectByStruct(tUIBaseObject* rec)
{
    switch (rec->mTag) {
    case tUIBaseObject::kUIObj_Icon: {
        cUIIcon* created = (cUIIcon*)alloc_D078(sizeof(cUIIcon), str_UIIcon);
        cUIThing_cUIThing(created, this, 0);
        VPTR_OF(cListNode, created) = cUIIcon_vtbl;
        tAttrIcon_tAttrIcon1(&created->mAttrIcon);
        created->mTextureID = -1;
        created->mpTextureElem = 0;
        created->setUIData(rec);
        return created;
    }
    case tUIBaseObject::kUIObj_Group: {
        cUIGroup* created = (cUIGroup*)alloc_D078(sizeof(cUIGroup), str_cUIGroup);
        cUIThing_cUIThing(created, this, 0);
        VPTR_OF(cListNode, created) = cUIGroup_vtbl;
        cList_cList(&created->mList);
        tUIGroup* data = (tUIGroup*)rec;
        tUIPlaceWidget* pPBW = data->mWidgets;
        for (unsigned int i = 0; i < data->mNumWidgets; i++, pPBW++) {
            cUIThing* child = getObjectByHashName(pPBW->mHashID);
            if (child != 0) {
                cListNode_removeFromList(child);
                child->mpParent = created;
                cList_addToEnd(&created->mList, (cListNode*)child);
            }
        }
        created->setUIData(rec);
        return created;
    }
    case tUIBaseObject::kUIObj_ListBox: {
        cUIListBox* created =
            (cUIListBox*)cUIListBox_cUIListBox(alloc_D078(sizeof(cUIListBox), str_UIListBox), this, 0);
        created->setUIData(rec);
        return created;
    }
    case tUIBaseObject::kUIObj_Menu: {
        cUIMenu* created = (cUIMenu*)alloc_D078(sizeof(cUIMenu), str_UIMenu);
        cUIThing_cUIThing(created, this, 0);
        VPTR_OF(cListNode, created) = cUIGroup_vtbl;
        cList_cList(&created->mList);
        VPTR_OF(cListNode, created) = cUIMenu_vtbl;
        __9tAttrMenu(&created->mAttrMenu);
        created->mStoredIndex = 0xFF;
        created->mScrollItemCurrent = 0;
        created->mScrollItemNum = 0;
        created->mScrollItemNumVisible = 0;
        created->mScrollItemOffset = 0;
        created->mpScrollBar = 0;
        created->mpSelected = 0;
        tUIMenu* data = (tUIMenu*)rec;
        tUIPlaceWidget* pPBW = data->mWidgets;
        for (unsigned int i = 0; i < data->mNumWidgets; i++, pPBW++) {
            cUIThing* child = getObjectByHashName(pPBW->mHashID);
            if (child != 0) {
                cListNode_removeFromList(child);
                child->mpParent = created;
                cList_addToEnd(&created->mList, (cListNode*)child);
            }
        }
        created->setUIData(rec);
        return created;
    }
    case tUIBaseObject::kUIObj_Pair: {
        cUIPair* created = (cUIPair*)alloc_D078(sizeof(cUIPair), str_UIPair);
        tUIPlaceWidget* pPBW = ((tUIPair*)rec)->mWidgets;
        cUIThing_cUIThing(created, this, 0);
        VPTR_OF(cListNode, created) = cUIPair_vtbl;
        __9tAttrPair(&created->mAttrPair);
        created->mAttr |= 2;
        created->mpEntry = 0;
        created->mpWidget = 0;
        created->setUIData(rec);
        cUIThing* child = getObjectByHashName(pPBW[0].mHashID);
        if (child != 0) {
            cListNode_removeFromList(child);
            child->mpParent = created;
            cUIThing* old = created->mpEntry;
            if (old != 0)
                delete old;
            created->mpEntry = child;
        }
        child = getObjectByHashName(pPBW[1].mHashID);
        if (child != 0) {
            child->mAttr &= ~1;
            cListNode_removeFromList(child);
            child->mpParent = created;
            cUIThing* old = created->mpWidget;
            if (old != 0)
                delete old;
            created->mpWidget = child;
        }
        return created;
    }
    case tUIBaseObject::kUIObj_Progress: {
        cUIProgress* created = (cUIProgress*)alloc_D078(sizeof(cUIProgress), str_cUIProgress);
        tUIProgress* data = (tUIProgress*)rec;
        cUIThing_cUIThing(created, this, 0);
        created->mpObjBackground = 0;
        created->mpObjKnob = 0;
        created->mValue = 0;
        created->mNumValues = 0;
        created->mBackWidth = 0;
        VPTR_OF(cListNode, created) = cUIProgress_vtbl;
        created->setUIData(rec);
        {
            cUIThing* child = getObjectByHashName(data->mBackground.mHashID);
            if (child != 0) {
                cListNode_removeFromList(child);
                child->mpParent = created;
                created->mpObjBackground = child;
            }
        }
        {
            cUIThing* child = getObjectByHashName(data->mKnob.mHashID);
            if (child != 0) {
                cListNode_removeFromList(child);
                child->mpParent = created;
                created->mpObjKnob = child;
            }
        }
        return created;
    }
    case tUIBaseObject::kUIObj_ScrollBar: {
        cUIScrollBar* created = (cUIScrollBar*)alloc_D078(sizeof(cUIScrollBar), str_cUIScroll);
        tUIScrollBar* data = (tUIScrollBar*)rec;
        cUIThing_cUIThing(created, this, 0);
        created->mpObjBackground = 0;
        created->mpObjKnob = 0;
        created->mpUpArrow = 0;
        created->mpDownArrow = 0;
        created->mpHideOnEmpty = 0;
        created->meHideState = 0;
        created->mValue = 0;
        created->mNumValues = 0;
        created->mPage = 0;
        created->mszUpAnimation[0] = 0;
        created->mszDownAnimation[0] = 0;
        VPTR_OF(cListNode, created) = cUIScrollBar_vtbl;
        created->setUIData(rec);
        {
            cUIThing* child = getObjectByHashName(data->mBackground.mHashID);
            if (child != 0) {
                cListNode_removeFromList(child);
                child->mpParent = created;
                created->mpObjBackground = child;
            }
        }
        {
            cUIThing* child = getObjectByHashName(data->mKnob.mHashID);
            if (child != 0) {
                cListNode_removeFromList(child);
                child->mpParent = created;
                created->mpObjKnob = child;
            }
        }
        return created;
    }
    case tUIBaseObject::kUIObj_Slider: {
        cUISlider* created = (cUISlider*)alloc_D078(sizeof(cUISlider), str_cUISlider);
        tUISlider* data = (tUISlider*)rec;
        cUIThing_cUIThing(created, this, 0);
        created->mValue = 0;
        created->mNumValues = 0;
        created->mStartPixel = 0;
        created->mBackWidth = 0;
        created->mpObjBackground = 0;
        created->mpObjKnob = 0;
        VPTR_OF(cListNode, created) = cUISlider_vtbl;
        created->setUIData(rec);
        {
            cUIThing* child = getObjectByHashName(data->mBackground.mHashID);
            if (child != 0) {
                cListNode_removeFromList(child);
                child->mpParent = created;
                created->mpObjBackground = child;
            }
        }
        {
            cUIThing* child = getObjectByHashName(data->mKnob.mHashID);
            if (child != 0) {
                cListNode_removeFromList(child);
                child->mpParent = created;
                created->mpObjKnob = child;
            }
        }
        return created;
    }
    case tUIBaseObject::kUIObj_Text: {
        cUIText* created = (cUIText*)cUIText_cUIText(alloc_D078(sizeof(cUIText), str_UIText), this, 0);
        created->setUIData(rec);
        return created;
    }
    case tUIBaseObject::kUIObj_TextScroll: {
        cUITextScroll* created =
            (cUITextScroll*)cUITextScroll_cUITextScroll(alloc_D078(sizeof(cUITextScroll), str_cUITextScroll), this, 0);
        tUITextScroll* data = (tUITextScroll*)rec;
        // ORIGINAL-PTR: a text scroll's record is variable-length: its mNumItems item widgets follow it
        tUIPlaceWidget* pPBW = (tUIPlaceWidget*)(data + 1);
        for (unsigned int i = 0; i < data->mNumItems; i++, pPBW++) {
            cUIThing* child = getObjectByHashName(pPBW->mHashID);
            if (child != 0) {
                cListNode_removeFromList(child);
                child->mpParent = created;
                cList_addToEnd(&created->mList, (cListNode*)child);
            }
        }
        created->setUIData(rec);
        return created;
    }
    case tUIBaseObject::kUIObj_Vector: {
        cUIVector* created = (cUIVector*)cUIVector_cUIVector(alloc_D078(sizeof(cUIVector), str_UIVector), this, 0);
        created->setUIData(rec);
        return created;
    }
    }
    return 0;
}
#endif

extern "C" void* cUIGroup_onInput(void*);

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_onInput__FPv);
#ifdef SKIP_ASM
/**
 * Passes controller input to the screen's root group (+0x40) through cUIGroup_onInput().
 *
 * The input object (On Tour onInput(pInput)) passes through in its register.
 *
 * @return cUIGroup_onInput()'s result (On Tour: whether the input was handled)
 */
void* cUIScreen::onInput()
{
    return cUIGroup_onInput(&mRootGroup);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uiscreen", cUIScreen_update);
#ifdef SKIP_ASM
extern "C" void cUIScreen_playFrameByThread(cUIScreen* self, cUIThread* thread);
extern "C" void cUIGroup_update(void* list, unsigned short frame);

/**
 * Runs the screen's animation threads for a frame, deleting finished ones, then updates the root group.
 *
 * The group is updated with the primary thread's frame (or the first thread's).
 */
void cUIScreen::update()
{
    cUIThread* n = (cUIThread*)cList_first(&mThreads);
    if (((void*)n) != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            void* next = n->mpNext;
            if (n->mLastFrameNumber != n->mCurrentFrameNumber) {
                this->playFrameByThread(n);
                int flags = *(signed char*)((char*)n + 0x13);
                if ((flags & 1) == 0) {
                    cListNode_removeFromList(n);
                    if (((void*)n) != 0) {
                        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)n->mVtbl;
                        vt[1].fn(vthis(n, vt[1]), 3); // slot 1: the deleting destructor
                    }
                }
            }
            n = (cUIThread*)next;
        }
    }
    unsigned short frame = 0xFFFF;
    cUIThread* thread = this->getPrimaryThread();
    if (thread != 0) {
        frame = thread->mCurrentFrameNumber;
    } else {
        thread = (cUIThread*)cList_first(&mThreads);
        if (thread != 0) {
            frame = thread->mCurrentFrameNumber;
        }
    }
    cUIGroup_update((char*)&mRootGroup, frame);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_getObjectByHashName);
#ifdef SKIP_ASM
/**
 * Finds a screen widget by its name hash.
 *
 * @param hash  the name's hash (On Tour hashname)
 * @return the widget, or 0
 */
cUIThing* cUIScreen::getObjectByHashName(int hash)
{
    cUIThing** objs = mppObjectList;
    if (objs != 0) {
        for (unsigned int i = 0; i < mpObjectBlock->mNumObjects; i++) {
            cUIThing* uiObj = objs[i];
            if (((void*)uiObj) != 0 && uiObj->mHashName == hash) {
                return uiObj;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScrollBar__cUIScrollBar);
#ifdef SKIP_ASM
extern "C" void cUIThing__cUIThing(void* self, int flags);
extern void* cUIScrollBar_vtbl[];

/**
 * Destroys a scroll bar: deletes mpObjBackground and mpObjKnob (deleting destructor, flags 3), then runs the cUIThing
 * destructor.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the cUIThing destructor)
 */
extern "C" void cUIScrollBar__cUIScrollBar(cUIScrollBar* self, int flags)
{
    VPTR_OF(cListNode, self) = cUIScrollBar_vtbl;
    cUIThing* background = self->mpObjBackground;
    delete background;
    cUIThing* knob = self->mpObjKnob;
    delete knob;
    cUIThing__cUIThing(self, flags);
}
#endif

extern "C" void* cUIThing_setUIData(void* self);

//100%
INCLUDE_ASM("ui/uiscreen", cUIScrollBar_setUIData__FPv);
#ifdef SKIP_ASM
/**
 * Sets a scroll bar up from its screen record: only cUIThing's setUIData.
 *
 * The record (On Tour tUIBaseObject* pUIBase) is not declared here; it stays in $a1 for the call.
 *
 * @return cUIThing_setUIData's result (declared void* here)
 */
void* cUIScrollBar_setUIData(void* vself)
{
    cUIScrollBar* self = (cUIScrollBar*)vself;
    return cUIThing_setUIData(self);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScrollBar_render2D);
#ifdef SKIP_ASM
/**
 * Draws the scroll bar's background and knob (each when visible, slot 17 render2D) at its position plus (@p x, @p y);
 * nothing unless both exist.
 *
 * @param x  the parent's x offset
 * @param y  the parent's y offset
 */
void cUIScrollBar::render2D(float x, float y)
{
    cUIThing* background = mpObjBackground;
    if (((char*)background) != 0 && mpObjKnob != 0) {
        float px = x + mPosition.x;
        float py = y + mPosition.y;
        if ((background->mAttr >> 6) & 1) {
            background->render2D(px, py);
        }
        cUIThing* knob = mpObjKnob;
        if ((knob->mAttr >> 6) & 1) {
            knob->render2D(px, py);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScrollBar_setValues);
#ifdef SKIP_ASM
extern "C" int strlen(const char*);
int GetHashValue32(char* str);
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cUIScrollBar_setScrollVisibile(cUIScrollBar* self, int a);

struct sV4_DA20 {
    float x, y, z, w;
};

struct sWVt_DA20 { short delta; short index; void* fn; };
typedef void (*tShow_DA20)(void*, int);
typedef void (*tGet_DA20)(void*, sV4_DA20*);

struct sWidget_DA20 {
    char pad0[8];
    sWVt_DA20* vt;      // 0x08
    char padC[0x44 - 0xC];
    t3Vector pos;       // 0x44
};

struct sScroll_DA20 {
    char pad0[0x14];
    int flags;                  // 0x14
    char pad18[0x5C - 0x18];
    cUIScreen* screen;          // 0x5C
    char pad60[0x74 - 0x60];
    sWidget_DA20* track;        // 0x74
    sWidget_DA20* thumb;        // 0x78
    sWidget_DA20* up;           // 0x7C
    sWidget_DA20* down;         // 0x80
    sWidget_DA20* extra;        // 0x84
    int mode;                   // 0x88
    char upLabel[0x19];         // 0x8C
    char downLabel[0x1B];       // 0xA5
    int pos;                    // 0xC0
    int total;                  // 0xC4
    int visible;                // 0xC8
};

static inline void wShow_DA20(sWidget_DA20* w, int on)
{
    ((tShow_DA20)w->vt[9].fn)(vthis(w, w->vt[9]), on);
}

static inline void wGet_DA20(sWidget_DA20* w, sV4_DA20* out)
{
    ((tGet_DA20)w->vt[20].fn)(vthis(w, w->vt[20]), out);
}

static inline void wSet_DA20(sWidget_DA20* w, sV4_DA20* in)
{
    ((tGet_DA20)w->vt[21].fn)(vthis(w, w->vt[21]), in);
}

/**
 * Updates a scroll bar's position, visible count and total, and resizes its knob to match.
 *
 * When the position goes up (or down) and an up (down) label name is set, the screen plays the frame with that label.
 * With a non-zero total and the vertical flag, the knob is placed at track y + track height * pos / total and sized
 * track height * visible / total (capped at the track). The up and down arrows are hidden at either end. When the knob
 * fills the track, the eScrollBarHide mode at self->mode hides the whole bar (All), the arrows and knob
 * (Slider_HArrows) or only the knob (Slider_SArrows); otherwise the knob, track and extra widget are shown. The knob's
 * size is set through its virtual slot 21.
 *
 * @param pos  first visible item (On Tour pos)
 * @param visible  number of items visible at once (On Tour page)
 * @param total  total number of items (On Tour range)
 */
extern "C" void cUIScrollBar_setValues(sScroll_DA20* self, int pos, int visible, int total)
{
    char* name = self->upLabel;
    if (strlen(name) != 0 && pos < self->pos) {
        cUIScreen* scr = self->screen;
        if (scr != 0) {
            int frameNum = scr->getFrameByLabel(GetHashValue32(name));
            if (frameNum != cUIScreen::kNoFrame)
                cUIScreen_playFrame(scr, frameNum, 1);
        }
    }
    name = self->downLabel;
    if (strlen(name) != 0 && self->pos < pos) {
        cUIScreen* scr = self->screen;
        if (scr != 0) {
            int frameNum = scr->getFrameByLabel(GetHashValue32(name));
            if (frameNum != cUIScreen::kNoFrame)
                cUIScreen_playFrame(scr, frameNum, 1);
        }
    }
    self->total = total;
    self->pos = pos;
    self->visible = visible;
    if (self->track == 0 || self->thumb == 0)
        return;
    t3Vector tpos = self->track->pos;
    t3Vector npos = self->thumb->pos;
    sV4_DA20 tsize;
    sV4_DA20 nsize;
    {
        sWidget_DA20* w = self->track;
        ((tGet_DA20)w->vt[20].fn)(vthis(w, w->vt[20]), &tsize);
    }
    {
        sWidget_DA20* w = self->thumb;
        ((tGet_DA20)w->vt[20].fn)(vthis(w, w->vt[20]), &nsize);
    }
    ((cUIScrollBar*)self)->setScrollVisibile(1);
    if (self->total != 0) {
        if (self->flags & 1) {
            float posRatio = (float)self->pos / (float)self->total;
            float fv = (float)self->visible / (float)self->total;
            npos.x = tpos.x;
            nsize.x = tsize.x;
            npos.y = tpos.y + tsize.y * posRatio;
            nsize.y = tsize.y * fv;
            if (tsize.y < nsize.y)
                nsize.y = tsize.y;
        }
        if (self->up != 0) {
            if (self->pos == 0)
                wShow_DA20(self->up, 0);
            else
                wShow_DA20(self->up, 1);
        }
        if (self->down != 0) {
            if (self->pos + self->visible >= self->total)
                wShow_DA20(self->down, 0);
            else
                wShow_DA20(self->down, 1);
        }
    } else {
        npos.y = tpos.y;
        npos.x = tpos.x;
        nsize.x = tsize.x;
        nsize.y = tsize.y;
    }
    if (nsize.y == tsize.y) {
        switch (self->mode) {
        case eScrollBarHide_All:
            ((cUIScrollBar*)self)->setScrollVisibile(0);
            break;
        case eScrollBarHide_Slider_HArrows:
            if (self->up != 0)
                wShow_DA20(self->up, 0);
            if (self->down != 0)
                wShow_DA20(self->down, 0);
            wShow_DA20(self->thumb, 0);
            break;
        case eScrollBarHide_Slider_SArrows:
            if (self->up != 0)
                wShow_DA20(self->up, 1);
            if (self->down != 0)
                wShow_DA20(self->down, 1);
            wShow_DA20(self->thumb, 0);
            break;
        }
    } else {
        if (self->extra != 0)
            wShow_DA20(self->extra, 1);
        wShow_DA20(self->thumb, 1);
        wShow_DA20(self->track, 1);
    }
    self->thumb->pos = npos;
    {
        sWidget_DA20* w = self->thumb;
        ((tGet_DA20)w->vt[21].fn)(vthis(w, w->vt[21]), &nsize);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScrollBar_setScrollVisibile);
#ifdef SKIP_ASM
struct cUIScrollBar_setScrollVisibile_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

static inline void cUIScrollBar_setScrollVisibile_call(char* o, int a)
{
    cUIScrollBar_setScrollVisibile_sVEntry* vt = *(cUIScrollBar_setScrollVisibile_sVEntry**)(o + 8);
    vt[9].fn(vthis(o, vt[9]), a);
}

/**
 * Shows or hides the whole scroll bar: setVisible (slot 9) on the up and down arrows and mpHideOnEmpty when they
 * exist, then on the knob and the background (not null-checked).
 *
 * @param bVisible  nonzero to show (On Tour bool bVisible)
 */
void cUIScrollBar::setScrollVisibile(int bVisible)
{
    char* element = (char*)mpUpArrow;
    if (element != 0) {
        cUIScrollBar_setScrollVisibile_call(element, bVisible);
    }
    element = (char*)mpDownArrow;
    if (element != 0) {
        cUIScrollBar_setScrollVisibile_call(element, bVisible);
    }
    element = (char*)mpHideOnEmpty;
    if (element != 0) {
        cUIScrollBar_setScrollVisibile_call(element, bVisible);
    }
    cUIScrollBar_setScrollVisibile_call((char*)mpObjKnob, bVisible);
    cUIScrollBar_setScrollVisibile_call((char*)mpObjBackground, bVisible);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUISlider__cUISlider);
#ifdef SKIP_ASM
extern "C" void cUIThing__cUIThing(void* self, int flags);
extern void* cUISlider_vtbl[];

/**
 * Destroys a slider: deletes mpObjBackground and mpObjKnob (deleting destructor, flags 3), then runs the cUIThing
 * destructor.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object (handled by the cUIThing destructor)
 */
extern "C" void cUISlider__cUISlider(cUISlider* self, int flags)
{
    VPTR_OF(cListNode, self) = cUISlider_vtbl;
    cUIThing* background = self->mpObjBackground;
    delete background;
    cUIThing* knob = self->mpObjKnob;
    delete knob;
    cUIThing__cUIThing(self, flags);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUISlider_setUIData);
#ifdef SKIP_ASM
/**
 * Sets a slider up from its screen record: cUIThing's setUIData, then mNumValues 10 and mBackWidth 100.
 *
 * The record (On Tour tUIBaseObject* pUIBase) is not declared here; it stays in $a1 for the cUIThing call.
 */
void cUISlider::setUIData(tUIBaseObject*)
{
    cUIThing_setUIData(this);
    mNumValues = 10;
    mBackWidth = 100;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUISlider_onInput);
#ifdef SKIP_ASM
struct cUIThing;
unsigned short cUIThing_getKeyframerEvent(cUIThing* self, int x);
extern "C" void cUIScreen_playFrame(void* self, unsigned short frame, int flag);
void* cUIThing_getEngine(void* self);
extern char gUISoundNull;

struct cUISlider_onInput_sVEntryS {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Moves the slider's value one step on left / right and reports it.
 *
 * Left lowers mValue while above 0 (keyframer event 8), right raises it while below mNumValues - 1 (event 9); a change
 * is reported to the screen's state as widget event kONCHANGE (onWidgetEvent) and to slot 2 of the engine's +0x14
 * listener (gUISoundNull when none) with 1.
 *
 * @param input  the input (On Tour const cUIInput* pInput)
 * @return true for left or right (even at the end of the range), else false (On Tour returns bool)
 */
bool cUISlider::onInput(const cUIInput* input)
{
    char* customSnd = (char*)((cUIEngine*)cUIThing_getEngine(((char*)this)))->mpSound;
    char* snd = &gUISoundNull;
    if (customSnd) snd = customSnd;
    if (input->isLeft()) {
        if (mValue > 0) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)this, 8);
            if (ev != cUIScreen::kNoFrame) {
                cUIScreen_playFrame(mpScreen, ev, 1);
            }
            mValue = mValue - 1;
            cUIState* mgr = mpScreen->mpState;
            mgr->onWidgetEvent((cUIThing*)this, cUIState::kONCHANGE);
            cUISlider_onInput_sVEntryS* vs = *(cUISlider_onInput_sVEntryS**)snd;
            vs[2].fn(vthis(snd, vs[2]), 1);
        }
        return true;
    }
    if (input->isRight()) {
        if (mValue + 1 < mNumValues) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)this, 9);
            if (ev != cUIScreen::kNoFrame) {
                cUIScreen_playFrame(mpScreen, ev, 1);
            }
            mValue = mValue + 1;
            cUIState* mgr = mpScreen->mpState;
            mgr->onWidgetEvent((cUIThing*)this, cUIState::kONCHANGE);
            cUISlider_onInput_sVEntryS* vs = *(cUISlider_onInput_sVEntryS**)snd;
            vs[2].fn(vthis(snd, vs[2]), 1);
        }
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUISlider_render2D);
#ifdef SKIP_ASM
/**
 * Draws the slider's background, then its knob moved along it by the value.
 *
 * The knob's x becomes the background's x + (background width - knob width) / (mNumValues - 1) * mValue (stored in
 * the knob's mPosition); both are drawn at the slider's position plus (@p dx, @p dy), with no visibility check.
 * Nothing is drawn unless both exist.
 *
 * @param dx  the parent's x offset (On Tour x)
 * @param dy  the parent's y offset (On Tour y)
 */
void cUISlider::render2D(float dx, float dy)
{
    cUIThing* background = mpObjBackground;
    if (((char*)background) == 0 || (char*)mpObjKnob == 0) {
        return;
    }
    float x = dx + mPosition.x;
    float y = dy + mPosition.y;
    background->render2D(x, y);
    cUIThing* bgObj = mpObjBackground;
    t3Vector pos = bgObj->mPosition;
    t3Vector bgSize;
    bgObj->getSize((t3Vector*)&bgSize);
    cUIThing* knob = mpObjKnob;
    t3Vector knobSize;
    knob->getSize((t3Vector*)&knobSize);
    float w = bgSize.x - knobSize.x;
    cUIThing* knobObj = mpObjKnob;
    t3Vector knobPos = knobObj->mPosition;
    knobPos.x = pos.x + w / ((float)mNumValues - 1.0f) * (float)mValue;
    knobObj->mPosition = knobPos;
    cUIThing* knobWidget = mpObjKnob;
    knobWidget->render2D(x, y);
}
#endif

extern void* cUISoundInterface_vtbl[];

//100%
INCLUDE_ASM("ui/uiscreen", cUISoundInterface_cUISoundInterface__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a UI sound interface: installs the base sound-interface vtable cUISoundInterface_vtbl.
 *
 * cFESound_cFESound runs it as its base constructor, and the static gUISoundNull is built with it.
 *
 * @return @p self
 */
void* cUISoundInterface_cUISoundInterface(void* vself)
{
    cUISoundInterface* self = (cUISoundInterface*)vself;
    *(int*)self = (int)(void*)cUISoundInterface_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_cUIState);
#ifdef SKIP_ASM
extern "C" void* cList_cList(void* self);
extern void* cUIState_vtbl[];

/**
 * Constructs a UI state for an engine.
 *
 * Links the list node to itself, installs the vtable cUIState_vtbl, clears the hash name (+0xC), stores @p engine (+0x10),
 * sets the current controller to -1 (+0x14), the controller bits to 0xF (+0x15) and the attribute word (+0x1C) to 0x218
 * (visible, first time, mode 2), clears the parent (+0x20), constructs the screen list (+0x24), clears the screen
 * (+0x40) and sets the user number to -1 (+0x44).
 *
 * @param engine  the UI engine
 * @return @p self
 */
extern "C" void* cUIState_cUIState(cUIState* self, cUIEngine* engine)
{
    self->mpNext = (cListNode*)self;
    self->mpPrev = (cListNode*)self;
    VPTR_OF(cListNode, self) = cUIState_vtbl;
    self->mHashName = 0;
    self->mpEngine = engine;
    self->mCurrentCtrl = -1;
    self->mCtrlBits = 0xF;
    *(short*)&self->mAttrState = 536;
    self->mpParentState = 0;
    cList_cList(&self->mScreens);
    self->mpScreen = 0;
    self->mUserNum = -1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_cUIState1);
#ifdef SKIP_ASM
extern "C" void* cList_cList(void* self);
extern void* cUIState_vtbl[];

/**
 * Constructs a UI state for an engine with a parent state.
 *
 * The same as cUIState_cUIState except that the parent (+0x20) is @p parentState.
 *
 * @param engine  the UI engine
 * @param parentState  the parent state
 * @return @p self
 */
void* cUIState::cUIState1(int engine, int parentState)
{
    mpNext = (cListNode*)this;
    mpPrev = (cListNode*)this;
    VPTR_OF(cListNode, this) = cUIState_vtbl;
    mHashName = 0;
    mpEngine = (cUIEngine*)engine;
    mCurrentCtrl = -1;
    mCtrlBits = 0xF;
    *(short*)&mAttrState = 536;
    mpParentState = (cUIState*)parentState;
    cList_cList(&mScreens);
    mpScreen = 0;
    mUserNum = -1;
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState__cUIState);
#ifdef SKIP_ASM
// PORT: unit declares cList_destroyList as returning void*; called here as void (bound by asm label).
extern "C" void cList_destroyList_v(void*) __asm__("cList_destroyList");
extern "C" void cListNode_destroyListNode(void*);
void operator_delete(int*);
extern void* cUIState_vtbl[];
extern void* cList_vtbl[];
extern void* cListNode_vtbl[];

/**
 * Destroys a UI state: destroys its screen list and list nodes, then frees it when bit 0 of @p flags is set.
 *
 * It is a slot of the cUIState vtable cUIState_vtbl.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cUIState__cUIState(cUIState* self, int flags)
{
    VPTR_OF(cListNode, self) = cUIState_vtbl;
    cList_destroyList_v((char*)&self->mScreens);
    VPTR_OF(cList, &self->mScreens) = cList_vtbl;
    cList_destroyList_v((char*)&self->mScreens);
    VPTR_OF(cListNode, &self->mScreens.mEnd) = cListNode_vtbl;
    cListNode_destroyListNode(&self->mScreens.mEnd);
    VPTR_OF(cListNode, &self->mScreens.mStart) = cListNode_vtbl;
    cListNode_destroyListNode((char*)&self->mScreens);
    VPTR_OF(cListNode, self) = cListNode_vtbl;
    cListNode_destroyListNode(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

extern "C" void* cList_destroyList(void*);

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_setCtrlBitsByCtrlIndex);
#ifdef SKIP_ASM
/**
 * Sets controller bit @p bit of a UI state's controller mask (+0x15), or all 8 bits when @p bit is negative.
 *
 * Nothing in the retail code references it.
 *
 * @param bit  the controller index, or -1 for all
 */
void cUIState::setCtrlBitsByCtrlIndex(signed char bit)
{
    if (bit < 0) {
        mCtrlBits = 0xFFU;
        return;
    }
    mCtrlBits = (unsigned char) ((mCtrlBits) | ((int) (0x01000000 << bit) >> 0x18));
}
#endif
//100%
INCLUDE_ASM("ui/uiscreen", cUIState_clearCtrlBits);
#ifdef SKIP_ASM
/**
 * Clears controller bit @p bit of a UI state's controller mask (+0x15).
 *
 * Nothing in the retail code references it.
 *
 * @param bit  the controller index
 */
void cUIState::clearCtrlBits(signed char bit)
{
    signed char mask = ~(1 << bit);
    mCtrlBits &= mask;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_onDestroyScreen__FPv);
#ifdef SKIP_ASM
/**
 * Destroys all screens of a UI state by destroying its screen list (mScreens, +0x24) with cList_destroyList().
 */
void cUIState::onDestroyScreen()
{
    cList_destroyList((&mScreens));
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_onGainFocus);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);


/**
 * Gives a UI state focus by focusing its first screen's root group (slot 13).
 *
 * On Tour passes the previous state; this build's declaration drops it.
 */
void cUIState::onGainFocus(cUIState*)
{
    cUIScreen* n = (cUIScreen*)cList_first((cList*)&mScreens);
    if (((void*)n) != 0) {
        sVEntry<void(void*, void*)>* vt = (sVEntry<void(void*, void*)>*)VPTR_OF(cListNode, this);
        vt[13].fn(vthis(this, vt[13]), (char*)&n->mRootGroup);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_onInputAlways__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base UI state's onInputAlways has an empty body.
 */
void cUIState::onInputAlways(cUIInput*)
{
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_onUpdate);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct cUIState_onUpdate_sVEntryI {
    short delta;
    short index;
    int (*fn)(void*);
};
struct cUIState_onUpdate_sVEntryV {
    short delta;
    short index;
    void (*fn)(void*, void*);
};
struct cUIState_onUpdate_sVEntryP {
    short delta;
    short index;
    int (*fn)(void*, void*);
};

static inline int cUIState_onUpdate_padTest(char* pad, int slot)
{
    cUIState_onUpdate_sVEntryI* vt = *(cUIState_onUpdate_sVEntryI**)(pad + 8);
    return vt[slot].fn(vthis(pad, vt[slot]));
}

/**
 * Base per-frame update of a UI state: routes controller input to the state and its screens.
 *
 * For controller index 0 and 1 (stored at +0x14 while running, -1 after) it refreshes the input object through its
 * virtual slot 44 and, when button 45 is down and either button 41 or button 40 with the controller enabled in the mask
 * at +0x15 is down, offers the input to virtual slot 8; if that does not consume it, each screen in the list at +0x24
 * with its enabled bit set gets it through its virtual slot 4 until one returns non-zero. Virtual slot 9
 * (cUIState_onInputAlways) is then called and the loop stops.
 */
void cUIState::onUpdate()
{
    int mask = (signed char)mCtrlBits;
    cUIInput* input = (cUIInput*)((char*)mpEngine->mpInput);
    mCurrentCtrl = 0;
    do {
        input->setPadNum((unsigned char)mCurrentCtrl);
        if (cUIState_onUpdate_padTest(((char*)input), 45)) {
            if (cUIState_onUpdate_padTest(((char*)input), 41) || (cUIState_onUpdate_padTest(((char*)input), 40) && (mask & 1))) {
                cUIState_onUpdate_sVEntryP* vs = (cUIState_onUpdate_sVEntryP*)VPTR_OF(cListNode, this);
                if (!vs[8].fn(vthis(this, vs[8]), ((char*)input))) {
                    cUIScreen* n = (cUIScreen*)cList_first((cList*)(&mScreens));
                    if (((char*)n) != 0) {
                        do {
                            if (n->mAttrScreen & 1) {
                                cUIState_onUpdate_sVEntryP* vn = (cUIState_onUpdate_sVEntryP*)n->mVtbl;
                                if (vn[4].fn(vthis(n, vn[4]), ((char*)input))) break;
                            }
                            n = (cUIScreen*)n->mpNext;
                        } while (!cListNode_isSentinel((cListNode*)n));
                    }
                }
                cUIState_onUpdate_sVEntryV* vs2 = (cUIState_onUpdate_sVEntryV*)VPTR_OF(cListNode, this);
                vs2[9].fn(vthis(this, vs2[9]), ((char*)input));
                break;
            }
        }
        mask >>= 1;
    } while (++mCurrentCtrl < 2);
    mCurrentCtrl = -1;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_onMenuGainFocus);
#ifdef SKIP_ASM
class cUIState_onMenuGainFocus_cVirt {
public:
    char pad[0x8];
    // vptr at 0x8; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void setFocus(int);
};

/**
 * Default handling when a state's menu group gains focus: gives the group focus.
 *
 * Calls virtual slot 7 of @p obj with 1.
 *
 * @param obj  the menu group gaining focus (On Tour pRootGroup)
 */
void cUIState::onMenuGainFocus(cUIGroup*obj)
{
    obj->setFocus(1);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_onRender2D);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);


/**
 * Base 2D render of a UI state: draws each of its visible screens.
 *
 * If the screen list at +0x24 is not empty, calls virtual slot 14 (cUIState_onPreRender2D) and then each screen whose
 * visible bit (bit 6 of +0x54) is set is drawn through the virtual slot 17 of its object at +0x40 with (0.0f, 0.0f).
 */
void cUIState::onRender2D()
{
    cUIScreen* n = (cUIScreen*)cList_first((cList*)&mScreens);
    if (((void*)n) != 0) {
        onPreRender2D();
        do {
            char* obj = (char*)&n->mRootGroup;
            if ((n->mRootGroup.mAttr >> 6) & 1) {
                sVEntry<void(void*, float, float)>* vt2 = (sVEntry<void(void*, float, float)>*)VPTR_OF(cListNode, &n->mRootGroup);
                vt2[17].fn(vthis(obj, vt2[17]), 0.0f, 0.0f);
            }
            n = (cUIScreen*)((void*)n->mpNext);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif

struct cList;
struct cListNode;
void cList_addToEnd(cList*, cListNode*);

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_addScreen);
#ifdef SKIP_ASM
/**
 * Appends a screen to the UI state's screen list (mScreens, +0x24).
 *
 * @param node  the screen to add (On Tour cUIScreen* pScreen)
 */
void cUIState::addScreen(cListNode* node)
{
    cList_addToEnd((cList*)&mScreens, node);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_hideScreens);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct sVEntry_cUIState_hideScreens {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Hides a UI state's active screens: calls virtual slot 9 with 0 on each screen in its list (+0x24) whose flag bit 6 is
 * set.
 *
 * Nothing in the retail code references it.
 */
void cUIState::hideScreens()
{
    cUIScreen* n = (cUIScreen*)cList_first((cList*)&mScreens);
    if (((void*)n) != 0) {
        do {
            char* obj = (char*)&n->mRootGroup;
            if ((n->mRootGroup.mAttr >> 6) & 1) {
                sVEntry_cUIState_hideScreens* vt = (sVEntry_cUIState_hideScreens*)VPTR_OF(cListNode, &n->mRootGroup);
                vt[9].fn(vthis(obj, vt[9]), 0);
            }
            n = (cUIScreen*)((void*)n->mpNext);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif
//100%
INCLUDE_ASM("ui/uiscreen", cUIState_showScreens);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct sVEntry_cUIState_showScreens {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Shows a UI state's active screens: calls virtual slot 9 with 1 on each screen in its list (+0x24) whose flag bit 6 is
 * set.
 *
 * Nothing in the retail code references it.
 */
void cUIState::showScreens()
{
    cUIScreen* n = (cUIScreen*)cList_first((cList*)&mScreens);
    if (((void*)n) != 0) {
        do {
            char* obj = (char*)&n->mRootGroup;
            if ((n->mRootGroup.mAttr >> 6) & 1) {
                sVEntry_cUIState_showScreens* vt = (sVEntry_cUIState_showScreens*)VPTR_OF(cListNode, &n->mRootGroup);
                vt[9].fn(vthis(obj, vt[9]), 1);
            }
            n = (cUIScreen*)((void*)n->mpNext);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uiscreen", cUIState_updateScreens);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);
extern "C" void cUIScreen_update(cUIScreen*);

/**
 * Updates every screen of the UI state with cUIScreen_update(), in list order.
 */
void cUIState::updateScreens()
{
    cUIScreen* n = (cUIScreen*)cList_first((cList*)&mScreens);
    if (((void*)n) != 0) {
        do {
            n->update();
            n = (cUIScreen*)((void*)n->mpNext);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif

