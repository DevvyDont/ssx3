#include "common.h"
#include "ui/uiengine.h"
#include "ui/uislider.h"
#include "ui/uiscrollbar.h"
#include "ui/uistate.h"
#include "ui/uiscreen.h"

extern "C" void cListNode_removeFromList(void* self);

struct sDeleteTarget {
    char pad_0x00[0x8];
    short field_0x8;
    char pad_0xA[2];
    void (*fn)(void*, int); // 0xC
};

//100%
INCLUDE_ASM("ui/uiscreen", cUIThread_deleteThread__FPv);
#ifdef SKIP_ASM
/**
 * Unlinks an animation thread from its screen's thread list and deletes it.
 */
void cUIThread_deleteThread(void* self)
{
    cListNode_removeFromList(self);
    if (self != 0) {
        sDeleteTarget* target = *(sDeleteTarget**)((char*)self + 0x8);
        target->fn((char*)self + target->field_0x8, 3);
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
    self->mRootGroup.mList.mVtbl = cList_vtbl;
    self->mRootGroup.mVtbl = cUIGroup_vtbl;
    cList_destroyList_C558(((char*)self) + 0xB4);
    char* rootGroup = ((char*)self) + 0x40;
    self->mRootGroup.mList.mEnd.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode(((char*)self) + 0xC0);
    self->mRootGroup.mList.mStart.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode(((char*)self) + 0xB4);
    cUIThing__cUIThing(rootGroup, 2);
    self->mThreads.mVtbl = cList_vtbl;
    cList_destroyList_C558(((char*)self) + 0x18);
    self->mThreads.mEnd.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode(((char*)self) + 0x24);
    self->mThreads.mStart.mVtbl = cListNode_vtbl;
    cListNode_destroyListNode(((char*)self) + 0x18);
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
extern "C" void cUIScreen_setData(cUIScreen* self, char* data)
{
    int size = unpacksizez(data);
    if (size != 0) {
        char* buf = (char*)operator_new_tag(size, str_UIScreenData, 0x100, 0);
        self->mpBuffer = buf;
        char* old = data;
        data = buf;
        unpack(old, data);
        self->mAttrScreen |= 2;
    } else {
        self->mAttrScreen &= ~2;
    }
    self->mpScreenHeader = (tUIScreenHeader*)data;
    self->mpKeyframeBlock = (tUIKeyframeBlock*)(data + *(int*)(data + 8));
    self->mpObjectBlock = (tUIObjectBlock*)(data + *(int*)(data + 4));
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
extern "C" void* cUIScreen_getFrameByIndex(cUIScreen* self, unsigned short frame)
{
    int* list = (int*)self->mpKeyframeBlock;
    char* frameHeader = (char*)list + 4;
    unsigned int i;
    for (i = 0; i < (unsigned int)*list; i++) {
        if (*(unsigned short*)(frameHeader + 0x8) == frame) {
            return frameHeader;
        }
        frameHeader = frameHeader + *(int*)(frameHeader + 0x4);
    }
    return 0;
}
#endif

struct sFrameEntry {
    int label; // 0x0
    int stride; // 0x4
    unsigned short field_0x8; // 0x8
};

struct sFrameList {
    int count; // 0x0
};


//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_getFrameByLabel__FP9cUIScreeni);
#ifdef SKIP_ASM
/**
 * Looks up a keyframe label's frame number in the screen's keyframe block.
 *
 * @param label  the label's hash (On Tour hashname)
 * @return the frame number, or 0xFFFF when the label is not found
 */
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label)
{
    sFrameList* list = (sFrameList*)self->mpKeyframeBlock;
    sFrameEntry* entry = (sFrameEntry*)((char*)list + 4);
    unsigned int i;
    for (i = 0; i < (unsigned int)list->count; i++) {
        if (entry->label == label) {
            return entry->field_0x8;
        }
        entry = (sFrameEntry*)((char*)entry + entry->stride);
    }
    return 0xFFFF;
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
 * Returns the screen's primary animation thread: the first one in mThreads with bit 25 of its word at +0x10
 * set.
 *
 * @return that thread, or 0 when there is none
 */
extern "C" void* cUIScreen_getPrimaryThread(cUIScreen* self)
{
    void* n = cList_first(&self->mThreads);
    if (n != 0) {
        for (; !cListNode_isSentinel((cListNode*)n); n = *(void**)((char*)n + 4)) {
            if ((*(int*)((char*)n + 0x10) >> 25) & 1) {
                return n;
            }
        }
    }
    return 0;
}
#endif

extern "C" void* cUIScreen_getPrimaryThread(cUIScreen* self);
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
void cUIScreen_jumpToFrame(void* self, unsigned short frame)
{
    void* thread = cUIScreen_getPrimaryThread((cUIScreen*)self);
    if (thread != 0) {
        cUIThread_deleteThread(thread);
    }
    cUIScreen_playFrame(self, frame, 0);
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
extern "C" void* cUIScreen_getPrimaryThread(cUIScreen* self);
void cUIThread_deleteThread(void* self);
extern char str_UIThread[];
extern void* D_004946A0[];

// cUIThread (On Tour member names): a cListNode (prev, next, vptr) plus frame state.
struct cUIThread_C870 {
    cUIThread_C870* mpPrev;             // 0x00
    cUIThread_C870* mpNext;             // 0x04
    void** vt;                          // 0x08
    unsigned short mCurrentFrameNumber; // 0x0C
    unsigned short mLastFrameNumber;    // 0x0E
    unsigned short mStopFrameNumber;    // 0x10
    signed char mFrameIncrement;        // 0x12
    unsigned int mIsActive : 1;         // 0x10 bit 24
    unsigned int mIsPrimary : 1;        // 0x10 bit 25

    unsigned short getCurrentFrameNumber() { return mCurrentFrameNumber; }
    signed char getFrameIncrement() { return mFrameIncrement; }
    void setCurrentFrameNumber(unsigned short v) { mCurrentFrameNumber = v; }
    void setLastFrameNumber(unsigned short v) { mLastFrameNumber = v; }
    void setStopFrameNumber(unsigned short v) { mStopFrameNumber = v; }
    void setFrameIncrement(signed char v) { mFrameIncrement = v; }
    void setActive(bool b) { mIsActive = b; }
    void setPrimary(bool b) { mIsPrimary = b; }
};

// PORT: the unit declares cUIScreen_playFrame as void; the body returns the new thread. Bound by asm label.
void* cUIScreen_playFrame_impl(void* self, unsigned short frame, int ThreadType) __asm__("cUIScreen_playFrame");

/**
 * Starts an animation thread at a frame; a primary thread (type 0) replaces the screen's current primary
 * thread.
 *
 * @param frame  the frame to start at
 * @param ThreadType  0 for the primary thread, otherwise an extra thread
 * @return the new thread
 */
void* cUIScreen_playFrame_impl(void* self, unsigned short frame, int ThreadType)
{
    if (ThreadType == 0) {
        void* pPrimary = cUIScreen_getPrimaryThread((cUIScreen*)self);
        if (pPrimary != 0) {
            cUIThread_deleteThread(pPrimary);
        }
    }
    cUIThread_C870* pNewThread = new (str_UIThread, 0x100, 0) cUIThread_C870;
    // inlined cUIThread ctor: direct member stores (the target writes these twice: the
    // second pair goes through the setters, whose promoted int params keep separate constants)
    pNewThread->mStopFrameNumber = 0xFFFF;
    pNewThread->mFrameIncrement = 1;
    pNewThread->mIsActive = 0;
    pNewThread->mIsPrimary = 0;
    pNewThread->setFrameIncrement(1);
    pNewThread->setStopFrameNumber(0xFFFF);
    pNewThread->vt = D_004946A0;
    pNewThread->setCurrentFrameNumber(frame);
    pNewThread->setLastFrameNumber(0xFFFF);
    pNewThread->mpNext = pNewThread;
    pNewThread->mpPrev = pNewThread;
    pNewThread->setActive(true);
    pNewThread->setPrimary(ThreadType == 0);
    cList_addToEnd((cList*)((char*)self + 0x18), (cListNode*)pNewThread);
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
extern void func_0039CD98_4(void *, void *, void *, void *) __asm__("func_0039CD98__FPv");
extern void cUIScreen_parseEventTrigger_4(void *, void *, void *, void *) __asm__("cUIScreen_parseEventTrigger");
extern void cUIScreen_parseScreenFocusGain_4(void *, void *, void *, void *) __asm__("cUIScreen_parseScreenFocusGain");
extern void cUIScreen_parseScreenFocusLose_4(void *, void *, void *, void *) __asm__("cUIScreen_parseScreenFocusLose");
extern void cUIScreen_parseScreenStart_4(void *, void *, void *, void *) __asm__("cUIScreen_parseScreenStart__FPv");
extern void cUIScreen_parseScreenEnd_4(void *, void *, void *, void *) __asm__("cUIScreen_parseScreenEnd");

// One frame command: opcode and byte size (commands are packed back to back).
struct sCmd_C978 {
    unsigned char op;         // 0x0
    char pad1;
    unsigned short size;      // 0x2
};

// The frame's command list (from cUIScreen_getFrameByIndex).
struct sCmdList_C978 {
    char pad0[0xA];
    unsigned short count;     // 0xA
    sCmd_C978 first;          // 0xC
};

/**
 * Advances a thread one frame (toward its stop frame, if any) and runs the commands of the frame it was on.
 *
 * Commands 0x10-0x12 are keyframe stop, jump and spawn, 0x2x and 0x3x object commands, 0x40-0x43 screen and
 * focus
 * events.
 *
 * @param thread  the thread (On Tour cUIThread* pThread)
 */
extern "C" void cUIScreen_playFrameByThread(cUIScreen* self, void *thread)
{
    cUIThread_C870 *uiThread = (cUIThread_C870 *)thread;
    sCmdList_C978 *list;
    sCmd_C978 *cmd;
    unsigned int i;
    unsigned short CurrentFrame = uiThread->getCurrentFrameNumber();
    uiThread->setLastFrameNumber(uiThread->getCurrentFrameNumber());
    if (uiThread->mStopFrameNumber != 0xFFFF) {
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
    list = (sCmdList_C978 *)cUIScreen_getFrameByIndex(self, CurrentFrame);
    cmd = &list->first;
    if (list != 0) {
        i = 0;
        if (list->count != 0) {
            do {
                switch (cmd->op) {
                case 0x10:
                    cUIScreen_parseKeyframePause_4(self, list, cmd, uiThread);
                    break;
                case 17:
                    cUIScreen_parseKeyframeJump_4(self, list, cmd, uiThread);
                    break;
                case 18:
                    cUIScreen_parseKeyframeSpawn_4(self, list, cmd, uiThread);
                    break;
                case 0x20:
                    cUIScreen_parseObjectAnim_4(self, list, cmd, uiThread);
                    break;
                case 33:
                    cUIScreen_parseObjectSetPos_4(self, list, cmd, uiThread);
                    break;
                case 34:
                    cUIScreen_parseObjectVisible_4(self, list, cmd, uiThread);
                    break;
                case 0x30:
                    cUIScreen_parseEventTrigger_4(self, list, cmd, uiThread);
                    break;
                case 49:
                    func_0039CD98_4(self, list, cmd, uiThread);
                    break;
                case 0x40:
                    cUIScreen_parseScreenStart_4(self, list, cmd, uiThread);
                    break;
                case 65:
                    cUIScreen_parseScreenEnd_4(self, list, cmd, uiThread);
                    break;
                case 66:
                    cUIScreen_parseScreenFocusGain_4(self, list, cmd, uiThread);
                    break;
                case 67:
                    cUIScreen_parseScreenFocusLose_4(self, list, cmd, uiThread);
                    break;
                }
                i++;
                cmd = (sCmd_C978 *)((char *)cmd + cmd->size);
            } while (i < list->count);
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
extern "C" void cUIScreen_parseKeyframePause(cUIScreen* self, void* pFrame, void* pKCP, void* pThread)
{
    *(unsigned int*)((char*)pThread + 0x10) &= 0xFEFFFFFFU;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseKeyframeJump);
#ifdef SKIP_ASM
/**
 * Runs a keyframe jump command: moves the thread to the frame with the command's label.
 *
 * @param pFrame  the frame's command list (unused)
 * @param pKCP  the jump command; its label hash is at +4
 * @param pThread  the thread to move
 */
extern "C" void cUIScreen_parseKeyframeJump(cUIScreen* self, void* pFrame, void* pKCP, void* pThread)
{
    sFrameEntry* entry = (sFrameEntry*)((char*)self->mpKeyframeBlock + 4);
    unsigned int i;
    for (i = 0; i < (unsigned int)((sFrameList*)self->mpKeyframeBlock)->count; i++) {
        if (entry->label == *(int*)((char*)pKCP + 4)) {
            *(unsigned short*)((char*)pThread + 0xC) = entry->field_0x8;
            return;
        }
        entry = (sFrameEntry*)((char*)entry + entry->stride);
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
 * @param pKCP  the spawn command; its label hash is at +4 (On Tour pKCP)
 */
extern "C" void cUIScreen_parseKeyframeSpawn(cUIScreen* self, void* pFrame, void* pKCP)
{
    sFrameEntry* entry = (sFrameEntry*)((char*)self->mpKeyframeBlock + 4);
    unsigned int i;
    for (i = 0; i < (unsigned int)((sFrameList*)self->mpKeyframeBlock)->count; i++) {
        if (entry->label == *(int*)((char*)pKCP + 4)) {
            cUIScreen_playFrame(self, entry->field_0x8, 1);
            return;
        }
        entry = (sFrameEntry*)((char*)entry + entry->stride);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseObjectAnim);
#ifdef SKIP_ASM
extern "C" void* cUIScreen_getObjectByHashName(cUIScreen* self, int hash);
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
extern "C" void cUIScreen_parseObjectAnim(cUIScreen* self, void* pFrame, void* msg)
{
    void* obj = cUIScreen_getObjectByHashName((cUIScreen*)self, *(int*)((char*)msg + 8));
    if (obj != 0) {
        void* anim = cUIAnimationBank_getAnimationByHashName(
            (char*)*(void**)((char*)self->mpState + 0x10) + 0x50,
            *(int*)((char*)msg + 4));
        if (anim != 0) {
            cUIThing_playAnimation(obj, anim, *(unsigned char*)((char*)msg + 0xC), *(unsigned short*)((char*)pFrame + 8), 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseObjectSetPos);
#ifdef SKIP_ASM
extern "C" void* cUIScreen_getObjectByHashName(cUIScreen* self, int hash);
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
    virtual void v13(int, void*);
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
extern "C" void cUIScreen_parseObjectSetPos(cUIScreen* self, int pFrame, int* msg)
{
    cUIScreen_parseObjectSetPos_cVirt* obj = (cUIScreen_parseObjectSetPos_cVirt*)cUIScreen_getObjectByHashName((cUIScreen*)self, msg[1]);
    if (obj != 0) {
        cUIThing_deleteAnimation(obj);
        obj->v13(msg[2], msg + 3);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseObjectVisible__FPv);
#ifdef SKIP_ASM
/**
 * Handles a keyframe visibility command; does nothing in this build.
 */
void cUIScreen_parseObjectVisible(void* self)
{
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", func_0039CD98__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * cUIScreen_playFrameByThread dispatches keyframe command 0x31 to it (0x30 goes to cUIScreen_parseEventTrigger); it
 * sits between cUIScreen_parseObjectVisible__FPv and cUIScreen_parseEventTrigger.
 */
void func_0039CD98(void* self)
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
    virtual int v16(void*, int);
};

class cUIScreen_parseEventTrigger_cVirtB {
public:
    char pad[0x4];
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual int v03(void*, int);
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
extern "C" void cUIScreen_parseEventTrigger(cUIScreen* self, void* pFrame, int* msg)
{
    cUIScreen_parseEventTrigger_cVirtA* state = (cUIScreen_parseEventTrigger_cVirtA*)self->mpState;
    if (state->v16(self, msg[1]) == 0) {
        cUIScreen_parseEventTrigger_cVirtB* customInterface = **(cUIScreen_parseEventTrigger_cVirtB***)((char*)self->mpState + 0x10);
        if (customInterface != 0) {
            customInterface->v03(self, msg[1]);
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
extern "C" void cUIScreen_parseScreenFocusGain(cUIScreen* self)
{
    void* state = self->mpState;
    if (state != 0) {
        ((sUIFlags1C*)((char*)state + 0x1c))->mode = 3;
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
extern "C" void cUIScreen_parseScreenFocusLose(cUIScreen* self)
{
    cUIGroup_setFocus(&self->mRootGroup, 0);
    void* state = self->mpState;
    if (state != 0) {
        ((sUIFlags1C*)((char*)state + 0x1c))->mode = 6;
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_parseScreenStart__FPv);
#ifdef SKIP_ASM
/**
 * Handles a keyframe screen-start command; does nothing in this build.
 */
void cUIScreen_parseScreenStart(void* self)
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
extern "C" void cUIScreen_parseScreenEnd(cUIScreen* self, void* pFrame, void* pKCP, void* pThread)
{
    *(unsigned int*)((char*)pThread + 0x10) &= 0xFEFFFFFFU;
    void* state = self->mpState;
    if (state != 0) {
        ((sUIFlags1C*)((char*)state + 0x1c))->mode = 7;
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
extern "C" char* cUIScreen_createObjectByStruct(cUIScreen* self, char* rec);
extern "C" void* MEM_fill(void* dst, int c, int n);
extern char str_UIScreenObjs[];

struct sUIVtCED0 { short delta; short index; void (*fn)(void*, void*); };

/**
 * Creates every widget described in the screen's object block, registers it by name and tells the state about
 * each one.
 *
 * The objects go into a "UIScreenObjs" table and the screen's list; the state's slot 18 (onWidgetCreate) then
 * sees each one.
 */
extern "C" void cUIScreen_createAllObjects(cUIScreen* self)
{
    char* data = (char*)self->mpObjectBlock;
    char* rec = data + 4;
    if (*(unsigned int*)data != 0) {
        self->mppObjectList = (cUIThing**)((void**)operator_new_tag(*(unsigned int*)data * 4, str_UIScreenObjs, 0x20000000, 0));
        MEM_fill((void**)self->mppObjectList, 0, *(unsigned int*)self->mpObjectBlock * 4);
    }
    void** out = (void**)self->mppObjectList;
    for (unsigned int i = 0; i < *(unsigned int*)self->mpObjectBlock; i++) {
        char* obj = cUIScreen_createObjectByStruct(self, rec);
        if (obj != 0) {
            *out++ = obj;
            *(int*)(obj + 0x38) = *(int*)(rec + 4);
            cList_addToEnd(&self->mRootGroup.mList, (cListNode*)obj);
        }
        rec += *(unsigned short*)(rec + 2);
    }
    for (unsigned int i = 0; i < *(unsigned int*)self->mpObjectBlock; i++) {
        void* uiObj = ((void**)self->mppObjectList)[i];
        if (uiObj != 0) {
            char* state = (char*)self->mpState;
            sUIVtCED0* vt = *(sUIVtCED0**)(state + 8);
            vt[18].fn(state + vt[18].delta, uiObj);
        }
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScreen_createObjectByHashName);
#ifdef SKIP_ASM
struct sObjRec_0039D008 {
    short pad0;
    unsigned short size; // 0x2
    int key;             // 0x4
};

/**
 * Creates the object of a UI screen whose record has hash name @p key.
 *
 * Walks the screen's object records (count at the start of the block at +0x38, each record sized by its +0x2 and hashed
 * at +0x4); the match is built with cUIScreen_createObjectByStruct and gets @p key at +0x38.
 *
 * @param key  the object hash name
 * @return the new object, or 0 when no record matches
 */
extern "C" void* cUIScreen_createObjectByHashName(void* self, int key)
{
    sObjRec_0039D008* record = (sObjRec_0039D008*)(*(char**)((char*)self + 0x38) + 4);
    unsigned int i;
    for (i = 0; i < *(*(unsigned int**)((char*)self + 0x38)); i++) {
        if (record->key == key) {
            char* obj = cUIScreen_createObjectByStruct((cUIScreen*)self, (char*)record);
            *(int*)(obj + 0x38) = key;
            return obj;
        }
        record = (sObjRec_0039D008*)((char*)record + record->size);
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
extern "C" void* func_0021CCC8(void* self);
extern "C" void* cList_cList(void* self);
extern "C" void* tAttrMenu_tAttrMenu(void* self);
extern "C" void* tAttrPair_tAttrPair(void* self);
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

struct sUIVtRec_D078 { short delta; short index; void (*fn)(void*, char*); };
struct sUIVtDel_D078 { short delta; short index; void (*fn)(void*, int); };

// UI objects keep their vtable at +0x8; slot 10 builds the object from its screen record
static inline void initFromRec_D078(char* o, char* rec)
{
    sUIVtRec_D078* vt = *(sUIVtRec_D078**)(o + 8);
    vt[10].fn(o + vt[10].delta, rec);
}

// slot 1: deleting destructor
static inline void deleteObj_D078(char* o)
{
    sUIVtDel_D078* vt = *(sUIVtDel_D078**)(o + 8);
    vt[1].fn(o + vt[1].delta, 3);
}

static inline char* alloc_D078(unsigned int size, const char* tag)
{
    return (char*)operator new(size, tag, 0x20000000, 0);
}

/**
 * Creates the widget a screen-data record describes, by record type, and adopts the child widgets it names.
 *
 * Types: 0x10 group, 0x11 icon, 0x12 list box, 0x13 menu, 0x15 pair, 0x16 slider, 0x17 text, 0x18 vector,
 * 0x19
 * progress, 0x20 text scroll, 0x21 scroll.
 *
 * @param rec  the object record (On Tour tUIBaseObject* pObject)
 * @return the new widget, or 0 for an unknown type
 */
extern "C" char* cUIScreen_createObjectByStruct(cUIScreen* self, char* rec)
{
    switch (*(unsigned short*)rec) {
    case 17: {
        char* created = alloc_D078(136, str_UIIcon);
        cUIThing_cUIThing(created, self, 0);
        *(void***)(created + 8) = cUIIcon_vtbl;
        func_0021CCC8(created + 0x74);
        *(int*)(created + 0x78) = -1;
        *(int*)(created + 0x7C) = 0;
        initFromRec_D078(created, rec);
        return created;
    }
    case 0x10: {
        char* created = alloc_D078(144, str_cUIGroup);
        cUIThing_cUIThing(created, self, 0);
        *(void***)(created + 8) = cUIGroup_vtbl;
        cList_cList(created + 0x74);
        int* hashes = (int*)(rec + 0x24);
        for (unsigned int i = 0; i < *(unsigned int*)(rec + 0x20); i++, hashes++) {
            char* child = (char*)cUIScreen_getObjectByHashName(self, *hashes);
            if (child != 0) {
                cListNode_removeFromList(child);
                *(char**)(child + 0x40) = created;
                cList_addToEnd((cList*)(created + 0x74), (cListNode*)child);
            }
        }
        initFromRec_D078(created, rec);
        return created;
    }
    case 18: {
        char* created = (char*)cUIListBox_cUIListBox(alloc_D078(796, str_UIListBox), self, 0);
        initFromRec_D078(created, rec);
        return created;
    }
    case 19: {
        char* created = alloc_D078(168, str_UIMenu);
        cUIThing_cUIThing(created, self, 0);
        *(void***)(created + 8) = cUIGroup_vtbl;
        cList_cList(created + 0x74);
        *(void***)(created + 8) = cUIMenu_vtbl;
        tAttrMenu_tAttrMenu(created + 0x90);
        *(unsigned char*)(created + 0xA4) = 0xFF;
        *(char*)(created + 0x95) = 0;
        *(char*)(created + 0x96) = 0;
        *(char*)(created + 0x97) = 0;
        *(char*)(created + 0x98) = 0;
        *(int*)(created + 0x9C) = 0;
        *(int*)(created + 0xA0) = 0;
        int* hashes = (int*)(rec + 0x24);
        for (unsigned int i = 0; i < *(unsigned char*)(rec + 0x20); i++, hashes++) {
            char* child = (char*)cUIScreen_getObjectByHashName(self, *hashes);
            if (child != 0) {
                cListNode_removeFromList(child);
                *(char**)(child + 0x40) = created;
                cList_addToEnd((cList*)(created + 0x74), (cListNode*)child);
            }
        }
        initFromRec_D078(created, rec);
        return created;
    }
    case 21: {
        char* created = alloc_D078(0x80, str_UIPair);
        int* hashes = (int*)(rec + 0x20);
        cUIThing_cUIThing(created, self, 0);
        *(void***)(created + 8) = cUIPair_vtbl;
        tAttrPair_tAttrPair(created + 0x74);
        *(int*)(created + 0x14) |= 2;
        *(int*)(created + 0x78) = 0;
        *(int*)(created + 0x7C) = 0;
        initFromRec_D078(created, rec);
        char* child = (char*)cUIScreen_getObjectByHashName(self, hashes[0]);
        if (child != 0) {
            cListNode_removeFromList(child);
            *(char**)(child + 0x40) = created;
            char* old = *(char**)(created + 0x78);
            if (old != 0)
                deleteObj_D078(old);
            *(char**)(created + 0x78) = child;
        }
        child = (char*)cUIScreen_getObjectByHashName(self, hashes[1]);
        if (child != 0) {
            *(int*)(child + 0x14) &= ~1;
            cListNode_removeFromList(child);
            *(char**)(child + 0x40) = created;
            char* old = *(char**)(created + 0x7C);
            if (old != 0)
                deleteObj_D078(old);
            *(char**)(created + 0x7C) = child;
        }
        return created;
    }
    case 25: {
        char* created = alloc_D078(136, str_cUIProgress);
        cUIThing_cUIThing(created, self, 0);
        *(int*)(created + 0x74) = 0;
        *(int*)(created + 0x78) = 0;
        *(int*)(created + 0x7C) = 0;
        *(int*)(created + 0x80) = 0;
        *(int*)(created + 0x84) = 0;
        *(void***)(created + 8) = cUIProgress_vtbl;
        initFromRec_D078(created, rec);
        {
            char* child = (char*)cUIScreen_getObjectByHashName(self, *(int*)(rec + 0x20));
            if (child != 0) {
                cListNode_removeFromList(child);
                *(char**)(child + 0x40) = created;
                *(char**)(created + 0x74) = child;
            }
        }
        {
            char* child = (char*)cUIScreen_getObjectByHashName(self, *(int*)(rec + 0x24));
            if (child != 0) {
                cListNode_removeFromList(child);
                *(char**)(child + 0x40) = created;
                *(char**)(created + 0x78) = child;
            }
        }
        return created;
    }
    case 33: {
        char* created = alloc_D078(204, str_cUIScroll);
        cUIThing_cUIThing(created, self, 0);
        *(int*)(created + 0x74) = 0;
        *(int*)(created + 0x78) = 0;
        *(int*)(created + 0x7C) = 0;
        *(int*)(created + 0x80) = 0;
        *(int*)(created + 0x84) = 0;
        *(int*)(created + 0x88) = 0;
        *(int*)(created + 0xC0) = 0;
        *(int*)(created + 0xC4) = 0;
        *(int*)(created + 0xC8) = 0;
        *(char*)(created + 0x8C) = 0;
        *(char*)(created + 0xA5) = 0;
        *(void***)(created + 8) = cUIScrollBar_vtbl;
        initFromRec_D078(created, rec);
        {
            char* child = (char*)cUIScreen_getObjectByHashName(self, *(int*)(rec + 0x20));
            if (child != 0) {
                cListNode_removeFromList(child);
                *(char**)(child + 0x40) = created;
                *(char**)(created + 0x74) = child;
            }
        }
        {
            char* child = (char*)cUIScreen_getObjectByHashName(self, *(int*)(rec + 0x24));
            if (child != 0) {
                cListNode_removeFromList(child);
                *(char**)(child + 0x40) = created;
                *(char**)(created + 0x78) = child;
            }
        }
        return created;
    }
    case 22: {
        char* created = alloc_D078(144, str_cUISlider);
        cUIThing_cUIThing(created, self, 0);
        *(int*)(created + 0x74) = 0;
        *(int*)(created + 0x78) = 0;
        *(int*)(created + 0x7C) = 0;
        *(int*)(created + 0x80) = 0;
        *(int*)(created + 0x88) = 0;
        *(int*)(created + 0x8C) = 0;
        *(void***)(created + 8) = cUISlider_vtbl;
        initFromRec_D078(created, rec);
        {
            char* child = (char*)cUIScreen_getObjectByHashName(self, *(int*)(rec + 0x24));
            if (child != 0) {
                cListNode_removeFromList(child);
                *(char**)(child + 0x40) = created;
                *(char**)(created + 0x88) = child;
            }
        }
        {
            char* child = (char*)cUIScreen_getObjectByHashName(self, *(int*)(rec + 0x28));
            if (child != 0) {
                cListNode_removeFromList(child);
                *(char**)(child + 0x40) = created;
                *(char**)(created + 0x8C) = child;
            }
        }
        return created;
    }
    case 23: {
        char* created = (char*)cUIText_cUIText(alloc_D078(188, str_UIText), self, 0);
        initFromRec_D078(created, rec);
        return created;
    }
    case 0x20: {
        char* created = (char*)cUITextScroll_cUITextScroll(alloc_D078(208, str_cUITextScroll), self, 0);
        int* hashes = (int*)(rec + 0x2C);
        for (unsigned int i = 0; i < *(unsigned char*)(rec + 0x22); i++, hashes++) {
            char* child = (char*)cUIScreen_getObjectByHashName(self, *hashes);
            if (child != 0) {
                cListNode_removeFromList(child);
                *(char**)(child + 0x40) = created;
                cList_addToEnd((cList*)(created + 0x8C), (cListNode*)child);
            }
        }
        initFromRec_D078(created, rec);
        return created;
    }
    case 0x18: {
        char* created = (char*)cUIVector_cUIVector(alloc_D078(144, str_UIVector), self, 0);
        initFromRec_D078(created, rec);
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
void* cUIScreen_onInput(void* self)
{
    return cUIGroup_onInput((char*)self + 0x40);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ui/uiscreen", cUIScreen_update);
#ifdef SKIP_ASM
extern "C" void cUIScreen_playFrameByThread(cUIScreen* self, void* thread);
extern "C" void cUIGroup_update(void* list, unsigned short frame);

/**
 * Runs the screen's animation threads for a frame, deleting finished ones, then updates the root group.
 *
 * The group is updated with the primary thread's frame (or the first thread's).
 */
extern "C" void cUIScreen_update(cUIScreen* self)
{
    void* n = cList_first(&self->mThreads);
    if (n != 0) {
        while (!cListNode_isSentinel((cListNode*)n)) {
            void* next = *(void**)((char*)n + 4);
            if (*(unsigned short*)((char*)n + 0xE) != *(unsigned short*)((char*)n + 0xC)) {
                cUIScreen_playFrameByThread(self, n);
                int flags = *(signed char*)((char*)n + 0x13);
                if ((flags & 1) == 0) {
                    cListNode_removeFromList(n);
                    if (n != 0) {
                        sDeleteTarget* target = *(sDeleteTarget**)((char*)n + 0x8);
                        target->fn((char*)n + target->field_0x8, 3);
                    }
                }
            }
            n = next;
        }
    }
    unsigned short frame = 0xFFFF;
    void* thread = cUIScreen_getPrimaryThread(self);
    if (thread != 0) {
        frame = *(unsigned short*)((char*)thread + 0xC);
    } else {
        thread = cList_first((cList*)((char*)self + 0x18));
        if (thread != 0) {
            frame = *(unsigned short*)((char*)thread + 0xC);
        }
    }
    cUIGroup_update((char*)self + 0x40, frame);
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
extern "C" void* cUIScreen_getObjectByHashName(cUIScreen* self, int hash)
{
    void** objs = (void**)self->mppObjectList;
    if (objs != 0) {
        for (unsigned int i = 0; i < *(unsigned int*)self->mpObjectBlock; i++) {
            void* uiObj = objs[i];
            if (uiObj != 0 && *(int*)((char*)uiObj + 0x38) == hash) {
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
    self->mVtbl = cUIScrollBar_vtbl;
    void* background = self->mpObjBackground;
    if (background != 0) {
        sDeleteTarget* dtorEntry = *(sDeleteTarget**)((char*)background + 0x8);
        dtorEntry->fn((char*)background + dtorEntry->field_0x8, 3);
    }
    void* knob = self->mpObjKnob;
    if (knob != 0) {
        sDeleteTarget* dtorEntry = *(sDeleteTarget**)((char*)knob + 0x8);
        dtorEntry->fn((char*)knob + dtorEntry->field_0x8, 3);
    }
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
void* cUIScrollBar_setUIData(void* self)
{
    return cUIThing_setUIData(self);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIScrollBar_render2D);
#ifdef SKIP_ASM
struct cUIScrollBar_render2D_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, float, float);
};

/**
 * Draws the scroll bar's background and knob (each when visible, slot 17 render2D) at its position plus (@p x, @p y);
 * nothing unless both exist.
 *
 * @param x  the parent's x offset
 * @param y  the parent's y offset
 */
extern "C" void cUIScrollBar_render2D(cUIScrollBar* self, float x, float y)
{
    char* background = (char*)self->mpObjBackground;
    if (background != 0 && self->mpObjKnob != 0) {
        float px = x + self->mPosition.x;
        float py = y + self->mPosition.y;
        if ((*(int*)(background + 0x14) >> 6) & 1) {
            cUIScrollBar_render2D_sVEntry* vt = *(cUIScrollBar_render2D_sVEntry**)(background + 8);
            vt[17].fn(background + vt[17].delta, px, py);
        }
        char* knob = (char*)self->mpObjKnob;
        if ((*(int*)(knob + 0x14) >> 6) & 1) {
            cUIScrollBar_render2D_sVEntry* vt = *(cUIScrollBar_render2D_sVEntry**)(knob + 8);
            vt[17].fn(knob + vt[17].delta, px, py);
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

struct sV3_DA20 {
    float x, y, z;
};
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
    sV3_DA20 pos;       // 0x44
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
    ((tShow_DA20)w->vt[9].fn)((char*)w + w->vt[9].delta, on);
}

static inline void wGet_DA20(sWidget_DA20* w, sV4_DA20* out)
{
    ((tGet_DA20)w->vt[20].fn)((char*)w + w->vt[20].delta, out);
}

static inline void wSet_DA20(sWidget_DA20* w, sV4_DA20* in)
{
    ((tGet_DA20)w->vt[21].fn)((char*)w + w->vt[21].delta, in);
}

/**
 * Updates a scroll bar's position, visible count and total, and resizes its knob to match.
 *
 * When the position goes up (or down) and an up (down) label name is set, the screen plays the frame with that label.
 * With a non-zero total and the vertical flag, the knob is placed at track y + track height * pos / total and sized
 * track height * visible / total (capped at the track). The up and down arrows are hidden at either end. When the knob
 * fills the track, the mode at self->mode hides the whole bar (0), the arrows and knob (1) or only the knob (2);
 * otherwise the knob, track and extra widget are shown. The knob's size is set through its virtual slot 21.
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
            int frameNum = cUIScreen_getFrameByLabel(scr, GetHashValue32(name));
            if (frameNum != 0xFFFF)
                cUIScreen_playFrame(scr, frameNum, 1);
        }
    }
    name = self->downLabel;
    if (strlen(name) != 0 && self->pos < pos) {
        cUIScreen* scr = self->screen;
        if (scr != 0) {
            int frameNum = cUIScreen_getFrameByLabel(scr, GetHashValue32(name));
            if (frameNum != 0xFFFF)
                cUIScreen_playFrame(scr, frameNum, 1);
        }
    }
    self->total = total;
    self->pos = pos;
    self->visible = visible;
    if (self->track == 0 || self->thumb == 0)
        return;
    sV3_DA20 tpos = self->track->pos;
    sV3_DA20 npos = self->thumb->pos;
    sV4_DA20 tsize;
    sV4_DA20 nsize;
    {
        sWidget_DA20* w = self->track;
        ((tGet_DA20)w->vt[20].fn)((char*)w + w->vt[20].delta, &tsize);
    }
    {
        sWidget_DA20* w = self->thumb;
        ((tGet_DA20)w->vt[20].fn)((char*)w + w->vt[20].delta, &nsize);
    }
    cUIScrollBar_setScrollVisibile((cUIScrollBar*)self, 1);
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
        case 0:
            cUIScrollBar_setScrollVisibile((cUIScrollBar*)self, 0);
            break;
        case 1:
            if (self->up != 0)
                wShow_DA20(self->up, 0);
            if (self->down != 0)
                wShow_DA20(self->down, 0);
            wShow_DA20(self->thumb, 0);
            break;
        case 2:
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
        ((tGet_DA20)w->vt[21].fn)((char*)w + w->vt[21].delta, &nsize);
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
    vt[9].fn(o + vt[9].delta, a);
}

/**
 * Shows or hides the whole scroll bar: setVisible (slot 9) on the up and down arrows and mpHideOnEmpty when they
 * exist, then on the knob and the background (not null-checked).
 *
 * @param bVisible  nonzero to show (On Tour bool bVisible)
 */
extern "C" void cUIScrollBar_setScrollVisibile(cUIScrollBar* self, int bVisible)
{
    char* element = (char*)self->mpUpArrow;
    if (element != 0) {
        cUIScrollBar_setScrollVisibile_call(element, bVisible);
    }
    element = (char*)self->mpDownArrow;
    if (element != 0) {
        cUIScrollBar_setScrollVisibile_call(element, bVisible);
    }
    element = (char*)self->mpHideOnEmpty;
    if (element != 0) {
        cUIScrollBar_setScrollVisibile_call(element, bVisible);
    }
    cUIScrollBar_setScrollVisibile_call((char*)self->mpObjKnob, bVisible);
    cUIScrollBar_setScrollVisibile_call((char*)self->mpObjBackground, bVisible);
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
    self->mVtbl = cUISlider_vtbl;
    void* background = self->mpObjBackground;
    if (background != 0) {
        sDeleteTarget* dtorEntry = *(sDeleteTarget**)((char*)background + 0x8);
        dtorEntry->fn((char*)background + dtorEntry->field_0x8, 3);
    }
    void* knob = self->mpObjKnob;
    if (knob != 0) {
        sDeleteTarget* dtorEntry = *(sDeleteTarget**)((char*)knob + 0x8);
        dtorEntry->fn((char*)knob + dtorEntry->field_0x8, 3);
    }
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
extern "C" void cUISlider_setUIData(cUISlider* self)
{
    cUIThing_setUIData(self);
    self->mNumValues = 10;
    self->mBackWidth = 100;
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

struct cUISlider_onInput_sVEntryI {
    short delta;
    short index;
    int (*fn)(void*);
};
struct cUISlider_onInput_sVEntryM {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};
struct cUISlider_onInput_sVEntryS {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Moves the slider's value one step on left / right and reports it.
 *
 * Left lowers mValue while above 0 (keyframer event 8), right raises it while below mNumValues - 1 (event 9); a
 * change is reported to the screen's state as widget event 9 (onWidgetEvent) and to slot 2 of the engine's +0x14
 * listener (gUISoundNull when none) with 1.
 *
 * @param input  the input (On Tour const cUIInput* pInput)
 * @return 1 for left or right (even at the end of the range), else 0 (On Tour returns bool)
 */
extern "C" int cUISlider_onInput(cUISlider* self, char* input)
{
    char* customSnd = *(char**)((char*)cUIThing_getEngine(((char*)self)) + 0x14);
    char* snd = &gUISoundNull;
    if (customSnd) snd = customSnd;
    cUISlider_onInput_sVEntryI* vi = *(cUISlider_onInput_sVEntryI**)(input + 8);
    if (vi[19].fn(input + vi[19].delta)) {
        if (self->mValue > 0) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)self, 8);
            if (ev != 0xFFFF) {
                cUIScreen_playFrame(self->mpScreen, ev, 1);
            }
            self->mValue = self->mValue - 1;
            char* mgr = *(char**)((char*)self->mpScreen + 0xD0);
            cUISlider_onInput_sVEntryM* vm = *(cUISlider_onInput_sVEntryM**)(mgr + 8);
            vm[19].fn(mgr + vm[19].delta, ((char*)self), 9);
            cUISlider_onInput_sVEntryS* vs = *(cUISlider_onInput_sVEntryS**)snd;
            vs[2].fn(snd + vs[2].delta, 1);
        }
        return 1;
    }
    cUISlider_onInput_sVEntryI* vi2 = *(cUISlider_onInput_sVEntryI**)(input + 8);
    if (vi2[20].fn(input + vi2[20].delta)) {
        if (self->mValue + 1 < self->mNumValues) {
            unsigned short ev = cUIThing_getKeyframerEvent((cUIThing*)self, 9);
            if (ev != 0xFFFF) {
                cUIScreen_playFrame(self->mpScreen, ev, 1);
            }
            self->mValue = self->mValue + 1;
            char* mgr = *(char**)((char*)self->mpScreen + 0xD0);
            cUISlider_onInput_sVEntryM* vm = *(cUISlider_onInput_sVEntryM**)(mgr + 8);
            vm[19].fn(mgr + vm[19].delta, ((char*)self), 9);
            cUISlider_onInput_sVEntryS* vs = *(cUISlider_onInput_sVEntryS**)snd;
            vs[2].fn(snd + vs[2].delta, 1);
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUISlider_render2D);
#ifdef SKIP_ASM
struct cUISlider_render2D_sVec3 {
    float x, y, z;
};
struct cUISlider_render2D_sVEntryF {
    short delta;
    short index;
    void (*fn)(void*, float, float);
};
struct cUISlider_render2D_sVEntryV {
    short delta;
    short index;
    void (*fn)(void*, cUISlider_render2D_sVec3*);
};

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
extern "C" void cUISlider_render2D(cUISlider* self, float dx, float dy)
{
    char* background = (char*)self->mpObjBackground;
    if (background == 0 || (char*)self->mpObjKnob == 0) {
        return;
    }
    float x = dx + self->mPosition.x;
    float y = dy + self->mPosition.y;
    cUISlider_render2D_sVEntryF* va = *(cUISlider_render2D_sVEntryF**)(background + 8);
    va[17].fn(background + va[17].delta, x, y);
    char* bgObj = (char*)self->mpObjBackground;
    cUISlider_render2D_sVec3 pos = *(cUISlider_render2D_sVec3*)(bgObj + 0x44);
    cUISlider_render2D_sVec3 bgSize;
    cUISlider_render2D_sVEntryV* vo = *(cUISlider_render2D_sVEntryV**)(bgObj + 8);
    vo[20].fn(bgObj + vo[20].delta, &bgSize);
    char* knob = (char*)self->mpObjKnob;
    cUISlider_render2D_sVec3 knobSize;
    cUISlider_render2D_sVEntryV* vb = *(cUISlider_render2D_sVEntryV**)(knob + 8);
    vb[20].fn(knob + vb[20].delta, &knobSize);
    float w = bgSize.x - knobSize.x;
    char* knobObj = (char*)self->mpObjKnob;
    cUISlider_render2D_sVec3 knobPos = *(cUISlider_render2D_sVec3*)(knobObj + 0x44);
    knobPos.x = pos.x + w / ((float)self->mNumValues - 1.0f) * (float)self->mValue;
    *(cUISlider_render2D_sVec3*)(knobObj + 0x44) = knobPos;
    char* knobWidget = (char*)self->mpObjKnob;
    cUISlider_render2D_sVEntryF* vb3 = *(cUISlider_render2D_sVEntryF**)(knobWidget + 8);
    vb3[17].fn(knobWidget + vb3[17].delta, x, y);
}
#endif

extern void* D_0046DD60[];

//100%
INCLUDE_ASM("ui/uiscreen", cUISoundInterface_cUISoundInterface__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a UI sound interface: installs the base sound-interface vtable D_0046DD60.
 *
 * cFESound_cFESound runs it as its base constructor, and the static gUISoundNull is built with it.
 *
 * @return @p self
 */
void* cUISoundInterface_cUISoundInterface(void* self)
{
    *(int*)self = (int)(void*)D_0046DD60;
    return self;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_cUIState);
#ifdef SKIP_ASM
extern "C" void* cList_cList(void* self);
extern void* D_004946C8[];

/**
 * Constructs a UI state for an engine.
 *
 * Links the list node to itself, installs the vtable D_004946C8, clears the hash name (+0xC), stores @p engine (+0x10),
 * sets the current controller to -1 (+0x14), the controller bits to 0xF (+0x15) and the attribute word (+0x1C) to 0x218
 * (visible, first time, mode 2), clears the parent (+0x20), constructs the screen list (+0x24), clears the screen
 * (+0x40) and sets the user number to -1 (+0x44).
 *
 * @param engine  the UI engine
 * @return @p self
 */
extern "C" void* cUIState_cUIState(void* self, int engine)
{
    char* stateBytes = (char*)self;
    *(void**)(stateBytes + 0x4) = stateBytes;
    *(void**)(stateBytes + 0x0) = stateBytes;
    *(void***)(stateBytes + 0x8) = D_004946C8;
    *(int*)(stateBytes + 0xC) = 0;
    *(int*)(stateBytes + 0x10) = engine;
    *(signed char*)(stateBytes + 0x14) = -1;
    *(signed char*)(stateBytes + 0x15) = 0xF;
    *(short*)(stateBytes + 0x1C) = 536;
    *(int*)(stateBytes + 0x20) = 0;
    cList_cList(stateBytes + 0x24);
    *(int*)(stateBytes + 0x40) = 0;
    *(signed char*)(stateBytes + 0x44) = -1;
    return self;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_cUIState1);
#ifdef SKIP_ASM
extern "C" void* cList_cList(void* self);
extern void* D_004946C8[];

/**
 * Constructs a UI state for an engine with a parent state.
 *
 * The same as cUIState_cUIState except that the parent (+0x20) is @p parentState.
 *
 * @param engine  the UI engine
 * @param parentState  the parent state
 * @return @p self
 */
extern "C" void* cUIState_cUIState1(void* self, int engine, int parentState)
{
    char* stateBytes = (char*)self;
    *(void**)(stateBytes + 0x4) = stateBytes;
    *(void**)(stateBytes + 0x0) = stateBytes;
    *(void***)(stateBytes + 0x8) = D_004946C8;
    *(int*)(stateBytes + 0xC) = 0;
    *(int*)(stateBytes + 0x10) = engine;
    *(signed char*)(stateBytes + 0x14) = -1;
    *(signed char*)(stateBytes + 0x15) = 0xF;
    *(short*)(stateBytes + 0x1C) = 536;
    *(int*)(stateBytes + 0x20) = parentState;
    cList_cList(stateBytes + 0x24);
    *(int*)(stateBytes + 0x40) = 0;
    *(signed char*)(stateBytes + 0x44) = -1;
    return self;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState__cUIState);
#ifdef SKIP_ASM
// PORT: unit declares cList_destroyList as returning void*; called here as void (bound by asm label).
extern "C" void cList_destroyList_v(void*) __asm__("cList_destroyList");
extern "C" void cListNode_destroyListNode(void*);
void operator_delete(int*);
extern void* D_004946C8[];
extern void* cList_vtbl[];
extern void* cListNode_vtbl[];

/**
 * Destroys a UI state: destroys its screen list and list nodes, then frees it when bit 0 of @p flags is set.
 *
 * It is a slot of the cUIState vtable D_004946C8.
 *
 * @param flags  g++ destructor flags; bit 0 frees the object
 */
extern "C" void cUIState__cUIState(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_004946C8;
    cList_destroyList_v((char*)self + 0x24);
    *(void***)((char*)self + 0x3C) = cList_vtbl;
    cList_destroyList_v((char*)self + 0x24);
    *(void***)((char*)self + 0x38) = cListNode_vtbl;
    cListNode_destroyListNode((char*)self + 0x30);
    *(void***)((char*)self + 0x2C) = cListNode_vtbl;
    cListNode_destroyListNode((char*)self + 0x24);
    *(void***)((char*)self + 0x8) = cListNode_vtbl;
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
 * @param uiState  the UI state
 * @param bit  the controller index, or -1 for all
 */
extern "C" void cUIState_setCtrlBitsByCtrlIndex(void *uiState, signed char bit) {
    if (bit < 0) {
        (*(unsigned char *)((char*)(uiState) + (0x15))) = 0xFFU;
        return;
    }
    (*(unsigned char *)((char*)(uiState) + (0x15))) = (unsigned char) ((*(unsigned char *)((char*)(uiState) + (0x15))) | ((int) (0x01000000 << bit) >> 0x18));
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
extern "C" void cUIState_clearCtrlBits(void* self, signed char bit)
{
    signed char mask = ~(1 << bit);
    *(unsigned char*)((char*)self + 0x15) &= mask;
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_onDestroyScreen__FPv);
#ifdef SKIP_ASM
/**
 * Destroys all screens of a UI state by destroying its screen list (mScreens, +0x24) with cList_destroyList().
 *
 * @return cList_destroyList()'s return value (On Tour's onDestroyScreen returns nothing)
 */
void* cUIState_onDestroyScreen(void* self)
{
    return cList_destroyList(((char*)self + 0x24));
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_onGainFocus);
#ifdef SKIP_ASM
struct cList;
void* cList_first(cList*);

struct sVEntry39E4C0 {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

/**
 * Gives a UI state focus by focusing its first screen's root group (slot 13).
 *
 * On Tour passes the previous state; this build's declaration drops it.
 */
extern "C" void cUIState_onGainFocus(cUIState* self)
{
    void* n = cList_first((cList*)((char*)self + 0x24));
    if (n != 0) {
        sVEntry39E4C0* vt = (sVEntry39E4C0*)self->mVtbl;
        vt[13].fn((char*)self + vt[0xD].delta, (char*)n + 0x40);
    }
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_onInputAlways__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base UI state's onInputAlways has an empty body.
 */
void cUIState_onInputAlways(void* self)
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

struct cUIState_onUpdate_sVEntryC {
    short delta;
    short index;
    void (*fn)(void*, unsigned char);
};
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
    return vt[slot].fn(pad + vt[slot].delta);
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
extern "C" void cUIState_onUpdate(cUIState* self)
{
    int mask = *(signed char*)&self->mCtrlBits;
    char* input = *(char**)&self->mpEngine->mpInput;
    self->mCurrentCtrl = 0;
    do {
        cUIState_onUpdate_sVEntryC* vc = *(cUIState_onUpdate_sVEntryC**)(input + 8);
        vc[44].fn(input + vc[44].delta, *(unsigned char*)&self->mCurrentCtrl);
        if (cUIState_onUpdate_padTest(input, 45)) {
            if (cUIState_onUpdate_padTest(input, 41) || (cUIState_onUpdate_padTest(input, 40) && (mask & 1))) {
                cUIState_onUpdate_sVEntryP* vs = (cUIState_onUpdate_sVEntryP*)self->mVtbl;
                if (!vs[8].fn(((char*)self) + vs[8].delta, input)) {
                    char* n = (char*)cList_first((cList*)(((char*)self) + 0x24));
                    if (n != 0) {
                        do {
                            if (*(int*)(n + 0x14) & 1) {
                                cUIState_onUpdate_sVEntryP* vn = *(cUIState_onUpdate_sVEntryP**)(n + 8);
                                if (vn[4].fn(n + vn[4].delta, input)) break;
                            }
                            n = *(char**)(n + 4);
                        } while (!cListNode_isSentinel((cListNode*)n));
                    }
                }
                cUIState_onUpdate_sVEntryV* vs2 = (cUIState_onUpdate_sVEntryV*)self->mVtbl;
                vs2[9].fn(((char*)self) + vs2[9].delta, input);
                break;
            }
        }
        mask >>= 1;
    } while (++self->mCurrentCtrl < 2);
    self->mCurrentCtrl = -1;
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
    virtual void v07(int);
};

/**
 * Default handling when a state's menu group gains focus: gives the group focus.
 *
 * Calls virtual slot 7 of @p obj with 1.
 *
 * @param obj  the menu group gaining focus (On Tour pRootGroup)
 */
extern "C" void cUIState_onMenuGainFocus(cUIState* self, cUIState_onMenuGainFocus_cVirt* obj)
{
    obj->v07(1);
}
#endif

//100%
INCLUDE_ASM("ui/uiscreen", cUIState_onRender2D);
#ifdef SKIP_ASM
struct cList;
struct cListNode;
void* cList_first(cList*);
int cListNode_isSentinel(cListNode*);

struct sVEntry39E6B8a {
    short delta;
    short index;
    void (*fn)(void*);
};
struct sVEntry39E6B8b {
    short delta;
    short index;
    void (*fn)(void*, float, float);
};

/**
 * Base 2D render of a UI state: draws each of its visible screens.
 *
 * If the screen list at +0x24 is not empty, calls virtual slot 14 (cUIState_onPreRender2D) and then each screen whose
 * visible bit (bit 6 of +0x54) is set is drawn through the virtual slot 17 of its object at +0x40 with (0.0f, 0.0f).
 */
extern "C" void cUIState_onRender2D(cUIState* self)
{
    void* n = cList_first((cList*)((char*)self + 0x24));
    if (n != 0) {
        sVEntry39E6B8a* vt = (sVEntry39E6B8a*)self->mVtbl;
        vt[0xE].fn((char*)self + vt[0xE].delta);
        do {
            char* obj = (char*)n + 0x40;
            if ((*(int*)((char*)n + 0x54) >> 6) & 1) {
                sVEntry39E6B8b* vt2 = *(sVEntry39E6B8b**)((char*)n + 0x48);
                vt2[17].fn(obj + vt2[17].delta, 0.0f, 0.0f);
            }
            n = *(void**)((char*)n + 4);
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
extern "C" void cUIState_addScreen(cUIState* self, cListNode* node)
{
    cList_addToEnd((cList*)((char*)self + 0x24), node);
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
extern "C" void cUIState_hideScreens(void* self)
{
    void* n = cList_first((cList*)((char*)self + 0x24));
    if (n != 0) {
        do {
            char* obj = (char*)n + 0x40;
            if ((*(int*)((char*)n + 0x54) >> 6) & 1) {
                sVEntry_cUIState_hideScreens* vt = *(sVEntry_cUIState_hideScreens**)((char*)n + 0x48);
                vt[9].fn(obj + vt[9].delta, 0);
            }
            n = *(void**)((char*)n + 4);
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
extern "C" void cUIState_showScreens(void* self)
{
    void* n = cList_first((cList*)((char*)self + 0x24));
    if (n != 0) {
        do {
            char* obj = (char*)n + 0x40;
            if ((*(int*)((char*)n + 0x54) >> 6) & 1) {
                sVEntry_cUIState_showScreens* vt = *(sVEntry_cUIState_showScreens**)((char*)n + 0x48);
                vt[9].fn(obj + vt[9].delta, 1);
            }
            n = *(void**)((char*)n + 4);
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
extern "C" void cUIState_updateScreens(cUIState* self)
{
    void* n = cList_first((cList*)((char*)self + 0x24));
    if (n != 0) {
        do {
            cUIScreen_update((cUIScreen*)n);
            n = *(void**)((char*)n + 4);
        } while (!cListNode_isSentinel((cListNode*)n));
    }
}
#endif

