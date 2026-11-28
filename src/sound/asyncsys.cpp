#include "vtable.h"
#include "common.h"
#include "main/gameviewman.h"
#include "sound/bankmanager.h"
#include "sound/soundsys.h"
#include "sound/ps2soundman.h"
#include "sound/asyncsys.h"
#include "ealib/hlafile.h"
#include "sound/ssxaudio.h"
#include "main/game.h"
#include "main/ssxapp.h"

void* operator_new(unsigned int size);


//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_Init__FP9cAsyncSysUi);
#ifdef SKIP_ASM
extern const char str_AsyncSysBuffer[];
// PORT: operator_new really takes (size, tag, flags, d); unit declares 1 arg
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

void cAsyncSys_ASYNCSYS_Init(cAsyncSys* self, unsigned int x, int flags) __asm__("cAsyncSys_ASYNCSYS_Init__FP9cAsyncSysUi");
/**
 * Allocates the async system's buffer.
 *
 * When @p x is nonzero, stores it at offset 0x1D0 and allocates an @p x-byte block named "AsyncSysBuffer" into
 * offset 0x1CC; with 0 it does nothing.
 *
 * @param x      buffer size in bytes, or 0 for no buffer
 * @param flags  allocation type passed to operator new
 */
void cAsyncSys::ASYNCSYS_Init(unsigned int x, int flags)
{
    if (x != 0) {
        mBufferSize = x;
        mpBuffer = (char*)operator_new_tag(x, str_AsyncSysBuffer, flags, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_DeInit);
#ifdef SKIP_ASM
extern "C" void cAsyncSys_DoCancel(void* self, int a);
extern "C" void TIMER_waitticks(int a);
void cMemMan_free(void* p);

/**
 * Shuts the async system down: services it until no request is pending, then frees its buffer.
 *
 * Calls cAsyncSys_DoCancel(self, 1) and, while the pending count (+0x4) is nonzero, sleeps (TIMER_waitticks(2)) and calls
 * it again. Frees the "AsyncSysBuffer" (+0x1CC) and clears it and its size (+0x1D0).
 */
void cAsyncSys::ASYNCSYS_DeInit()
{
    cAsyncSys_DoCancel(this, 1);
    while (mBusy != 0) {
        TIMER_waitticks(2);
        cAsyncSys_DoCancel(this, 1);
    }
    if (mpBuffer != 0) {
        cMemMan_free(mpBuffer);
        mpBuffer = 0;
        mBufferSize = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_StartLoad);
#ifdef SKIP_ASM
extern "C" int ASYNCFILE_loadat(char* name, void* buf, int size);
extern "C" int ASYNCFILE_load(char* name, int a1);





/**
 * Starts the next queued sound-bank load when the async system is idle and something is queued.
 *
 * Field names are On Tour's cAsyncSys (same offsets): sets mBusy (+0x4) and tells the owner through virtual slot 2
 * (cBankManager_ASYNCSYS_OnBeginAsyncLoad; vtable at +0x1D4) with the front user handle (queue mUserHandle, +0x18C).
 * Then by the front memory type (queue mMemType, +0x1AC, an eSoundMemLocation): keSoundMemLocation_Dynamic and
 * keSoundMemLocation_Sound load the front file name (mFilename slot from queue mFilenameIndex, +0x19C) into the shared
 * buffer mpBuffer (+0x1CC) of mBufferSize (+0x1D0) bytes with ASYNCFILE_loadat; keSoundMemLocation_Main asks the owner
 * for main memory through slots 3
 * (cBankManager_ASYNCSYS_OnAsyncMainMemAlloc, size -1 and the front flags of queue mMainMemFlags, +0x1BC) and 4
 * (cBankManager_ASYNCSYS_OnAsyncMainMemSize) and loads there with ASYNCFILE_loadat, or with ASYNCFILE_load and those flags
 * when no memory came back. The file handle goes to mHandle (+0x0). Any other memory type starts nothing but leaves
 * mBusy set.
 *
 * The queues are only peeked here; cAsyncSys_ASYNCSYS_Update pops them when the load has ended.
 */
void cAsyncSys::ASYNCSYS_StartLoad()
{
    cAsyncSys* asyncSys = this;
    if (asyncSys->mBusy != 0) {
        return;
    }
    cCircBuffer* userHandleQueue = (&asyncSys->mUserHandle);
    if ((*(&asyncSys->mUserHandle)).Head == (*(&asyncSys->mUserHandle)).Tail) {
        return;
    }
    asyncSys->mBusy = 1;
    sVEntry<void(void*, int)>* vtBegin = (sVEntry<void(void*, int)>*)asyncSys->mVtbl;
    vtBegin[2].fn(vthis(this, vtBegin[2]), userHandleQueue->Array[userHandleQueue->Tail]);
    int kind;
    {
        cCircBuffer* peekQueue = (&asyncSys->mMemType);
        if ((*(&asyncSys->mMemType)).Head == (*(&asyncSys->mMemType)).Tail) {
            kind = -1;
        } else {
            kind = peekQueue->Array[peekQueue->Tail];
        }
    }
    switch (kind) {
    case keSoundMemLocation_Dynamic: {
        int idx;
        cCircBuffer* peekQueue = (&asyncSys->mFilenameIndex);
        if ((*(&asyncSys->mFilenameIndex)).Head == (*(&asyncSys->mFilenameIndex)).Tail) {
            idx = -1;
        } else {
            idx = peekQueue->Array[peekQueue->Tail];
        }
        asyncSys->mHandle = ASYNCFILE_loadat(asyncSys->mFilename[idx], asyncSys->mpBuffer, asyncSys->mBufferSize);
        break;
    }
    case keSoundMemLocation_Sound: {
        int idx;
        cCircBuffer* peekQueue = (&asyncSys->mFilenameIndex);
        if ((*(&asyncSys->mFilenameIndex)).Head == (*(&asyncSys->mFilenameIndex)).Tail) {
            idx = -1;
        } else {
            idx = peekQueue->Array[peekQueue->Tail];
        }
        asyncSys->mHandle = ASYNCFILE_loadat(asyncSys->mFilename[idx], asyncSys->mpBuffer, asyncSys->mBufferSize);
        break;
    }
    case keSoundMemLocation_Main: {
        int idx;
        {
            cCircBuffer* peekQueue = (&asyncSys->mFilenameIndex);
            if ((*(&asyncSys->mFilenameIndex)).Head == (*(&asyncSys->mFilenameIndex)).Tail) {
                idx = -1;
            } else {
                idx = peekQueue->Array[peekQueue->Tail];
            }
        }
        char* name = asyncSys->mFilename[idx];
        int allocHandle, allocFlags;
        // PORT: g++ 2.95 virtual calls (vptr at 0x1D4) written out by hand: delta + saved &pfn, args evaluated after.
        sVEntry<void*(void*, int, int, int)>* vt = ((sVEntry<void*(void*, int, int, int)>*)asyncSys->mVtbl);
        void* thisp = vthis(asyncSys, vt[3]);
        void* (**pf)(void*, int, int, int) = &vt[3].fn;
        if ((*(&asyncSys->mUserHandle)).Head == (*(&asyncSys->mUserHandle)).Tail) {
            allocHandle = -1;
        } else {
            allocHandle = userHandleQueue->Array[userHandleQueue->Tail];
        }
        cCircBuffer* flagQueue = (&asyncSys->mMainMemFlags);
        if ((*(&asyncSys->mMainMemFlags)).Head == (*(&asyncSys->mMainMemFlags)).Tail) {
            allocFlags = -1;
        } else {
            allocFlags = flagQueue->Array[flagQueue->Tail];
        }
        void* allocBuffer = (*pf)(thisp, allocHandle, -1, allocFlags);
        sVEntry<int(void*, int)>* vt2 = ((sVEntry<int(void*, int)>*)asyncSys->mVtbl);
        void* thisp2 = vthis(asyncSys, vt2[4]);
        int (**pf2)(void*, int) = &vt2[4].fn;
        int sizeHandle;
        if ((*(&asyncSys->mUserHandle)).Head == (*(&asyncSys->mUserHandle)).Tail) {
            sizeHandle = -1;
        } else {
            sizeHandle = userHandleQueue->Array[userHandleQueue->Tail];
        }
        int allocSize = (*pf2)(thisp2, sizeHandle);
        if (allocBuffer != 0) {
            asyncSys->mHandle = ASYNCFILE_loadat(name, allocBuffer, allocSize);
        } else {
            int loadFlags;
            if ((*(&asyncSys->mMainMemFlags)).Head == (*(&asyncSys->mMainMemFlags)).Tail) {
                loadFlags = -1;
            } else {
                loadFlags = flagQueue->Array[flagQueue->Tail];
            }
            asyncSys->mHandle = ASYNCFILE_load(name, loadFlags);
        }
        break;
    }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_QueueLoad);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void cAsyncSys_ASYNCSYS_StartLoad(cAsyncSys* self);

/**
 * Queues a sound-bank file for asynchronous loading and starts it when the system is idle.
 *
 * Pushes @p bankId onto the user-handle queue (+0x18C, On Tour mUserHandle), @p memType onto the memory-type queue (+0x1AC,
 * mMemType) and @p memFlags onto the flags queue (+0x1BC, mMainMemFlags); each push is skipped when that queue is full.
 * Copies @p name into the file-name slot +0x8 (mFileIndex) of the six 64-byte names at +0xC, pushes that slot onto the
 * name-index queue (+0x19C), advances the slot (back to 0 after 5) and calls cAsyncSys_ASYNCSYS_StartLoad. The name ring has six slots
 * whatever the queue size, so a seventh pending load overwrites the oldest name. cBankManager_BANK_AsyncLoad (the bank manager's
 * async load) calls it.
 *
 * @param bankId  the bank's user handle, given back to the owner's callbacks (On Tour bankID)
 * @param name  the file name
 * @param memType  where the bank goes: 0 dynamic (shared buffer), 1 main memory, 2 sound memory (On Tour eSoundMemLocation)
 * @param memFlags  main-memory allocation flags (On Tour allocMainMemFlags)
 */
void cAsyncSys::ASYNCSYS_QueueLoad(int bankId, const char* name, int memType, int memFlags)
{
    {
        cCircBuffer* pushQueue = &mUserHandle;
        if ((mUserHandle.Head + 1) % mUserHandle.ArraySize != mUserHandle.Tail) {
            pushQueue->Array[pushQueue->Head] = bankId;
            if (++mUserHandle.Head >= mUserHandle.ArraySize) {
                mUserHandle.Head = 0;
            }
        }
    }
    {
        cCircBuffer* pushQueue = &mMemType;
        if ((mMemType.Head + 1) % mMemType.ArraySize != mMemType.Tail) {
            pushQueue->Array[pushQueue->Head] = memType;
            if (++mMemType.Head >= mMemType.ArraySize) {
                mMemType.Head = 0;
            }
        }
    }
    {
        cCircBuffer* pushQueue = &mMainMemFlags;
        if ((mMainMemFlags.Head + 1) % mMainMemFlags.ArraySize != mMainMemFlags.Tail) {
            pushQueue->Array[pushQueue->Head] = memFlags;
            if (++mMainMemFlags.Head >= mMainMemFlags.ArraySize) {
                mMainMemFlags.Head = 0;
            }
        }
    }
    strcpy(mFilename[mFileIndex], name);
    int slot = mFileIndex++;
    {
        cCircBuffer* pushQueue = &mFilenameIndex;
        if ((mFilenameIndex.Head + 1) % mFilenameIndex.ArraySize != mFilenameIndex.Tail) {
            pushQueue->Array[pushQueue->Head] = slot;
            if (++mFilenameIndex.Head >= mFilenameIndex.ArraySize) {
                mFilenameIndex.Head = 0;
            }
        }
    }
    if (mFileIndex >= 6) {
        mFileIndex = 0;
    }
    ASYNCSYS_StartLoad();
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_Cancel);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cAsyncSys_DoCancel(void* self, int a);
extern "C" void cAsyncSys_ASYNCSYS_StartLoad(cAsyncSys* self);


static inline int IsEmpty_UserHandle(cAsyncSys* s)
{
    return s->mUserHandle.Head == s->mUserHandle.Tail;
}

static inline int IsEmpty_FilenameIndex(cAsyncSys* s)
{
    return s->mFilenameIndex.Head == s->mFilenameIndex.Tail;
}

static inline int IsEmpty_MemType(cAsyncSys* s)
{
    return s->mMemType.Head == s->mMemType.Tail;
}

static inline int IsEmpty_MainMemFlags(cAsyncSys* s)
{
    return s->mMainMemFlags.Head == s->mMainMemFlags.Tail;
}

static inline int Peek_UserHandle(cAsyncSys* s)
{
    cCircBuffer* q = &s->mUserHandle;
    if (IsEmpty_UserHandle(s)) {
        return -1;
    }
    return q->Array[q->Tail];
}

static inline int Pop_UserHandle(cAsyncSys* s)
{
    cCircBuffer* q = &s->mUserHandle;
    if (IsEmpty_UserHandle(s)) {
        return -1;
    }
    int v = q->Array[q->Tail];
    if (++s->mUserHandle.Tail >= s->mUserHandle.ArraySize) {
        s->mUserHandle.Tail = 0;
    }
    return v;
}

static inline int Pop_FilenameIndex(cAsyncSys* s)
{
    cCircBuffer* q = &s->mFilenameIndex;
    if (IsEmpty_FilenameIndex(s)) {
        return -1;
    }
    int v = q->Array[q->Tail];
    if (++s->mFilenameIndex.Tail >= s->mFilenameIndex.ArraySize) {
        s->mFilenameIndex.Tail = 0;
    }
    return v;
}

static inline int Pop_MemType(cAsyncSys* s)
{
    cCircBuffer* q = &s->mMemType;
    if (IsEmpty_MemType(s)) {
        return -1;
    }
    int v = q->Array[q->Tail];
    if (++s->mMemType.Tail >= s->mMemType.ArraySize) {
        s->mMemType.Tail = 0;
    }
    return v;
}

static inline int Pop_MainMemFlags(cAsyncSys* s)
{
    cCircBuffer* q = &s->mMainMemFlags;
    if (IsEmpty_MainMemFlags(s)) {
        return -1;
    }
    int v = q->Array[q->Tail];
    if (++s->mMainMemFlags.Tail >= s->mMainMemFlags.ArraySize) {
        s->mMainMemFlags.Tail = 0;
    }
    return v;
}

static inline void Push_UserHandle(cAsyncSys* s, int v)
{
    cCircBuffer* q = &s->mUserHandle;
    if ((s->mUserHandle.Head + 1) % s->mUserHandle.ArraySize != s->mUserHandle.Tail) {
        q->Array[q->Head] = v;
        if (++s->mUserHandle.Head >= s->mUserHandle.ArraySize) {
            s->mUserHandle.Head = 0;
        }
    }
}

static inline void Push_FilenameIndex(cAsyncSys* s, int v)
{
    cCircBuffer* q = &s->mFilenameIndex;
    if ((s->mFilenameIndex.Head + 1) % s->mFilenameIndex.ArraySize != s->mFilenameIndex.Tail) {
        q->Array[q->Head] = v;
        if (++s->mFilenameIndex.Head >= s->mFilenameIndex.ArraySize) {
            s->mFilenameIndex.Head = 0;
        }
    }
}

static inline void Push_MemType(cAsyncSys* s, int v)
{
    cCircBuffer* q = &s->mMemType;
    if ((s->mMemType.Head + 1) % s->mMemType.ArraySize != s->mMemType.Tail) {
        q->Array[q->Head] = v;
        if (++s->mMemType.Head >= s->mMemType.ArraySize) {
            s->mMemType.Head = 0;
        }
    }
}

static inline void Push_MainMemFlags(cAsyncSys* s, int v)
{
    cCircBuffer* q = &s->mMainMemFlags;
    if ((s->mMainMemFlags.Head + 1) % s->mMainMemFlags.ArraySize != s->mMainMemFlags.Tail) {
        q->Array[q->Head] = v;
        if (++s->mMainMemFlags.Head >= s->mMainMemFlags.ArraySize) {
            s->mMainMemFlags.Head = 0;
        }
    }
}

/**
 * Cancels every load queued for one bank, aborting it when it is the one in progress.
 *
 * When busy (+0x4) and the front user handle (+0x18C) is @p id, calls cAsyncSys_DoCancel(this, 0) and returns. Otherwise
 * pops every queued entry from the four queues (user handle, name index, memory type, flags) and pushes back those
 * whose user handle is not @p id, which keeps their order; then, when not busy, starts the next load (cAsyncSys_ASYNCSYS_StartLoad).
 * cBankManager_BANK_SyncLoad calls it.
 *
 * @param id  the bank's user handle (On Tour bankID)
 */
void cAsyncSys::ASYNCSYS_Cancel(int id)
{
    if (mBusy != 0 && Peek_UserHandle(this) == id) {
        cAsyncSys_DoCancel(this, 0);
        return;
    }
    int n = (mUserHandle.Head - mUserHandle.Tail + mUserHandle.ArraySize) % mUserHandle.ArraySize;
    for (int i = 0; i < n; i++) {
        int queuedHandle = Pop_UserHandle(this);
        if (queuedHandle == id) {
            Pop_FilenameIndex(this);
            Pop_MemType(this);
            Pop_MainMemFlags(this);
        } else {
            Push_UserHandle(this, queuedHandle);
            Push_FilenameIndex(this, Pop_FilenameIndex(this));
            Push_MemType(this, Pop_MemType(this));
            Push_MainMemFlags(this, Pop_MainMemFlags(this));
        }
    }
    if (mBusy == 0) {
        ASYNCSYS_StartLoad();
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_Update);
#ifdef SKIP_ASM
extern "C" int SNDbankadd(int* out, void* bank);
extern "C" int SNDbankremove(int id);
extern "C" int SNDbankheadersize(int id);
extern "C" int SNDbankheadercopy(void* destMem, int bankHandle);
extern "C" int ASYNCFILE_getstatus(int handle);
extern "C" void MEM_copy(void* dst, void* src, int n);
extern "C" int ASYNCFILE_release(int handle, void** buf, int* size);
extern "C" void cAsyncSys_ASYNCSYS_StartLoad(cAsyncSys* self);

// The owner's slot 3 (OnAsyncMainMemAlloc: user handle, size, flags; returns the memory) and slot 5 (OnEndAsyncLoad:
// user handle, bank handle, end state, file name, memory, size).
typedef void* tAsyncMainMemAllocFn(void*, int, int, int);
typedef void tAsyncEndLoadFn(void*, int, int, int, char*, void*, int);

/**
 * Polls the sound-bank load in progress and, once it has finished, registers the bank, hands it to the owner and starts
 * the next load.
 *
 * Only while busy (+0x4). By the front memory type (+0x1AC, an eSoundMemLocation): for keSoundMemLocation_Dynamic and
 * keSoundMemLocation_Sound (loaded into the shared buffer) and for keSoundMemLocation_Main it checks
 * ASYNCFILE_getstatus on the file handle (+0x0): kASYNCFILE_Pending is still loading; kASYNCFILE_Done is done:
 * ASYNCFILE_release gives the data and size. Shared buffer: SNDbankadd adds the bank and writes its handle to +0x0;
 * unless it returns kSNDBANKADD_Uploaded the bank is removed again (SNDbankremove), the owner provides main memory of
 * the full size (virtual slot 3, cBankManager_ASYNCSYS_OnAsyncMainMemAlloc, with the front user handle and flags), the
 * data is copied there (MEM_copy) and added from there; on kSNDBANKADD_Uploaded only the header is kept:
 * SNDbankheadersize gives its size, the owner provides that much and SNDbankheadercopy copies it. Main memory: the data
 * is added where it is. The owner then gets virtual slot 5 (cBankManager_ASYNCSYS_OnEndAsyncLoad) with the user handle,
 * the bank handle, end state 0 (complete), the file name, the memory and the size; an empty file reports handle -1 and
 * end state 2 (error). Then mBusy is cleared, all four queues are popped and cAsyncSys_ASYNCSYS_StartLoad starts the
 * next load. Any other status (kASYNCFILE_Invalid) cancels through cAsyncSys_DoCancel(this, 0).
 *
 * cController_Update (the stream system's update) calls it.
 */
void cAsyncSys::ASYNCSYS_Update()
{
    if (mBusy == 0) {
        return;
    }
    int kind;
    {
        cCircBuffer* memTypeQueue = &mMemType;
        if (mMemType.Head == mMemType.Tail) {
            kind = -1;
        } else {
            kind = memTypeQueue->Array[memTypeQueue->Tail];
        }
    }
    switch (kind) {
    case keSoundMemLocation_Dynamic:
    case keSoundMemLocation_Sound:
        switch (ASYNCFILE_getstatus(mHandle)) {
        case kASYNCFILE_Pending:
            break;
        case kASYNCFILE_Done: {
            void* buf = 0;
            int size = 0;
            ASYNCFILE_release(mHandle, &buf, &size);
            if (size > 0) {
                if (SNDbankadd(&mHandle, buf) != kSNDBANKADD_Uploaded) {
                    SNDbankremove(mHandle);
                    cCircBuffer* userHandleQueue = &mUserHandle;
                    sVEntry<tAsyncMainMemAllocFn>* vt = (sVEntry<tAsyncMainMemAllocFn>*)mVtbl;
                    void* thisp = vthis(this, vt[3]);
                    tAsyncMainMemAllocFn** pf = &vt[3].fn;
                    int allocHandle;
                    int allocFlags;
                    if (mUserHandle.Head == mUserHandle.Tail) {
                        allocHandle = -1;
                    } else {
                        allocHandle = userHandleQueue->Array[userHandleQueue->Tail];
                    }
                    int n = size;
                    {
                        cCircBuffer* flagQueue = &mMainMemFlags;
                        if (mMainMemFlags.Head == mMainMemFlags.Tail) {
                            allocFlags = -1;
                        } else {
                            allocFlags = flagQueue->Array[flagQueue->Tail];
                        }
                    }
                    void* allocBuffer = (*pf)(thisp, allocHandle, n, allocFlags);
                    MEM_copy(allocBuffer, buf, size);
                    SNDbankadd(&mHandle, allocBuffer);
                    sVEntry<tAsyncEndLoadFn>* vt2 = (sVEntry<tAsyncEndLoadFn>*)mVtbl;
                    void* thisp2 = vthis(this, vt2[5]);
                    tAsyncEndLoadFn** pf2 = &vt2[5].fn;
                    int endHandle;
                    int idx;
                    if (mUserHandle.Head == mUserHandle.Tail) {
                        endHandle = -1;
                    } else {
                        endHandle = userHandleQueue->Array[userHandleQueue->Tail];
                    }
                    {
                        cCircBuffer* nameIndexQueue = &mFilenameIndex;
                        if (mFilenameIndex.Head == mFilenameIndex.Tail) {
                            idx = -1;
                        } else {
                            idx = nameIndexQueue->Array[nameIndexQueue->Tail];
                        }
                    }
                    (*pf2)(thisp2, endHandle, mHandle, 0, mFilename[idx], allocBuffer, size);
                } else {
                    int id = SNDbankheadersize(mHandle);
                    cCircBuffer* userHandleQueue = &mUserHandle;
                    // PORT: g++ 2.95 virtual calls (vptr at 0x1D4) written out by hand: delta + saved &pfn, args evaluated after.
                    sVEntry<tAsyncMainMemAllocFn>* vt = (sVEntry<tAsyncMainMemAllocFn>*)mVtbl;
                    void* thisp = vthis(this, vt[3]);
                    tAsyncMainMemAllocFn** pf = &vt[3].fn;
                    int allocHandle;
                    int allocFlags;
                    if (mUserHandle.Head == mUserHandle.Tail) {
                        allocHandle = -1;
                    } else {
                        allocHandle = userHandleQueue->Array[userHandleQueue->Tail];
                    }
                    {
                        cCircBuffer* flagQueue = &mMainMemFlags;
                        if (mMainMemFlags.Head == mMainMemFlags.Tail) {
                            allocFlags = -1;
                        } else {
                            allocFlags = flagQueue->Array[flagQueue->Tail];
                        }
                    }
                    void* allocBuffer = (*pf)(thisp, allocHandle, id, allocFlags);
                    SNDbankheadercopy(allocBuffer, mHandle);
                    sVEntry<tAsyncEndLoadFn>* vt2 = (sVEntry<tAsyncEndLoadFn>*)mVtbl;
                    void* thisp2 = vthis(this, vt2[5]);
                    tAsyncEndLoadFn** pf2 = &vt2[5].fn;
                    int endHandle;
                    int idx;
                    if (mUserHandle.Head == mUserHandle.Tail) {
                        endHandle = -1;
                    } else {
                        endHandle = userHandleQueue->Array[userHandleQueue->Tail];
                    }
                    {
                        cCircBuffer* nameIndexQueue = &mFilenameIndex;
                        if (mFilenameIndex.Head == mFilenameIndex.Tail) {
                            idx = -1;
                        } else {
                            idx = nameIndexQueue->Array[nameIndexQueue->Tail];
                        }
                    }
                    (*pf2)(thisp2, endHandle, mHandle, 0, mFilename[idx], allocBuffer, id);
                }
            } else {
                cCircBuffer* userHandleQueue = &mUserHandle;
                sVEntry<tAsyncEndLoadFn>* vt2 = (sVEntry<tAsyncEndLoadFn>*)mVtbl;
                void* thisp2 = vthis(this, vt2[5]);
                tAsyncEndLoadFn** pf2 = &vt2[5].fn;
                int endHandle;
                int idx;
                if (mUserHandle.Head == mUserHandle.Tail) {
                    endHandle = -1;
                } else {
                    endHandle = userHandleQueue->Array[userHandleQueue->Tail];
                }
                {
                    cCircBuffer* nameIndexQueue = &mFilenameIndex;
                    if (mFilenameIndex.Head == mFilenameIndex.Tail) {
                        idx = -1;
                    } else {
                        idx = nameIndexQueue->Array[nameIndexQueue->Tail];
                    }
                }
                (*pf2)(thisp2, endHandle, -1, 2, mFilename[idx], 0, 0);
            }
            mBusy = 0;
            if (mUserHandle.Head != mUserHandle.Tail) {
                if (++mUserHandle.Tail >= mUserHandle.ArraySize) {
                    mUserHandle.Tail = 0;
                }
            }
            if (mFilenameIndex.Head != mFilenameIndex.Tail) {
                if (++mFilenameIndex.Tail >= mFilenameIndex.ArraySize) {
                    mFilenameIndex.Tail = 0;
                }
            }
            if (mMemType.Head != mMemType.Tail) {
                if (++mMemType.Tail >= mMemType.ArraySize) {
                    mMemType.Tail = 0;
                }
            }
            if (mMainMemFlags.Head != mMainMemFlags.Tail) {
                if (++mMainMemFlags.Tail >= mMainMemFlags.ArraySize) {
                    mMainMemFlags.Tail = 0;
                }
            }
            ASYNCSYS_StartLoad();
            break;
        }
        case kASYNCFILE_Invalid:
        default:
            cAsyncSys_DoCancel(this, 0);
            break;
        }
        break;
    case keSoundMemLocation_Main:
        switch (ASYNCFILE_getstatus(mHandle)) {
        case kASYNCFILE_Pending:
            break;
        case kASYNCFILE_Done: {
            void* buf = 0;
            int size = 0;
            ASYNCFILE_release(mHandle, &buf, &size);
            if (size > 0) {
                SNDbankadd(&mHandle, buf);
                cCircBuffer* userHandleQueue = &mUserHandle;
                sVEntry<tAsyncEndLoadFn>* vt2 = (sVEntry<tAsyncEndLoadFn>*)mVtbl;
                void* thisp2 = vthis(this, vt2[5]);
                tAsyncEndLoadFn** pf2 = &vt2[5].fn;
                int endHandle;
                int idx;
                if (mUserHandle.Head == mUserHandle.Tail) {
                    endHandle = -1;
                } else {
                    endHandle = userHandleQueue->Array[userHandleQueue->Tail];
                }
                {
                    cCircBuffer* nameIndexQueue = &mFilenameIndex;
                    if (mFilenameIndex.Head == mFilenameIndex.Tail) {
                        idx = -1;
                    } else {
                        idx = nameIndexQueue->Array[nameIndexQueue->Tail];
                    }
                }
                (*pf2)(thisp2, endHandle, mHandle, 0, mFilename[idx], buf, size);
            } else {
                cCircBuffer* userHandleQueue = &mUserHandle;
                sVEntry<tAsyncEndLoadFn>* vt2 = (sVEntry<tAsyncEndLoadFn>*)mVtbl;
                void* thisp2 = vthis(this, vt2[5]);
                tAsyncEndLoadFn** pf2 = &vt2[5].fn;
                int endHandle;
                int idx;
                if (mUserHandle.Head == mUserHandle.Tail) {
                    endHandle = -1;
                } else {
                    endHandle = userHandleQueue->Array[userHandleQueue->Tail];
                }
                {
                    cCircBuffer* nameIndexQueue = &mFilenameIndex;
                    if (mFilenameIndex.Head == mFilenameIndex.Tail) {
                        idx = -1;
                    } else {
                        idx = nameIndexQueue->Array[nameIndexQueue->Tail];
                    }
                }
                (*pf2)(thisp2, endHandle, -1, 2, mFilename[idx], buf, 0);
            }
            mBusy = 0;
            if (mUserHandle.Head != mUserHandle.Tail) {
                if (++mUserHandle.Tail >= mUserHandle.ArraySize) {
                    mUserHandle.Tail = 0;
                }
            }
            if (mFilenameIndex.Head != mFilenameIndex.Tail) {
                if (++mFilenameIndex.Tail >= mFilenameIndex.ArraySize) {
                    mFilenameIndex.Tail = 0;
                }
            }
            if (mMemType.Head != mMemType.Tail) {
                if (++mMemType.Tail >= mMemType.ArraySize) {
                    mMemType.Tail = 0;
                }
            }
            if (mMainMemFlags.Head != mMainMemFlags.Tail) {
                if (++mMainMemFlags.Tail >= mMainMemFlags.ArraySize) {
                    mMainMemFlags.Tail = 0;
                }
            }
            ASYNCSYS_StartLoad();
            break;
        }
        case kASYNCFILE_Invalid:
        default:
            cAsyncSys_DoCancel(this, 0);
            break;
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_OnCurAsyncMainMemAlloc);
#ifdef SKIP_ASM


/**
 * Asks the owner for main memory for the load in progress: virtual slot 3 (cBankManager_ASYNCSYS_OnAsyncMainMemAlloc)
 * with the front user handle (+0x18C), @p size and the front allocation flags (+0x1BC).
 *
 * Declared void, but the owner's result (the memory) stays in the return register. Nothing in the program calls it.
 *
 * @param size  the number of bytes wanted
 */
void cAsyncSys::ASYNCSYS_OnCurAsyncMainMemAlloc(int size)
{
    sVEntry<void(void*, int, int, int)>* vtEntry = &((sVEntry<void(void*, int, int, int)>*)mVtbl)[3];
    void (**pf)(void*, int, int, int) = &vtEntry->fn;
    void* thisp = vthis(this, *vtEntry);
    cCircBuffer* handleQueue = &(*(&mUserHandle));
    int userHandle;
    int memFlags;
    if (mUserHandle.Head == mUserHandle.Tail)
        userHandle = -1;
    else
        userHandle = handleQueue->Array[handleQueue->Tail];
    cCircBuffer* flagsQueue = &(*(&mMainMemFlags));
    if (mMainMemFlags.Head == mMainMemFlags.Tail)
        memFlags = -1;
    else
        memFlags = flagsQueue->Array[flagsQueue->Tail];
    (*pf)(thisp, userHandle, size, memFlags);
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_bxAudio);
#ifdef SKIP_ASM

/**
 * Returns the game's audio object.
 *
 * @return gSSXAudio (On Tour cSSXAudio*)
 */
extern "C" cSSXAudio* cAudioInterface_bxAudio()
{
    return (cSSXAudio*)gSSXAudio;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_isInFE);
#ifdef SKIP_ASM

/**
 * Tells whether the front end is running: 1 when SSXApp's RealFE or DebugFE is set, else 0.
 *
 * @return 1 in the front end, else 0
 */
extern "C" int cAudioInterface_isInFE(void) {
    int var_4;

    var_4 = 0;
    if (SSXApp->RealFE != 0 || SSXApp->DebugFE != 0) {
        var_4 = 1;
    }
    return var_4;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_isInGame);
#ifdef SKIP_ASM

/**
 * Says whether a game is running: SSXApp has a cGame.
 *
 * One of the audio code's static accessors (On Tour cAudioInterface): tests SSXApp->Game (+0x84). The sound code
 * (cSSXAudio_GetClosestCamera1, cSSXAudio_GetSpeechVolumeIdentifier, cSSXAudio_SetRadioMode, bmRestartC2D0, ...) uses it to skip in-game work in the front end.
 *
 * @return true if SSXApp->Game is set
 */
extern "C" bool cAudioInterface_isInGame()
{
    return (void*)SSXApp->Game != 0;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_getApp);
#ifdef SKIP_ASM

/**
 * Returns the application object SSXApp.
 *
 * @return SSXApp
 */
extern "C" cSSXApp* cAudioInterface_getApp()
{
    return (cSSXApp*)(char*)SSXApp;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_getGame);
#ifdef SKIP_ASM

/**
 * Returns the running cGame (SSXApp->Game).
 *
 * Static accessor of the audio code (On Tour cAudioInterface). Callers such as cSSXAudio_HandleGlide read game fields
 * (+0x28, +0x214) through it.
 *
 * @return SSXApp->Game (0 outside a game)
 */
extern "C" cGame* cAudioInterface_getGame()
{
    return (cGame*)SSXApp->Game;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_getAI);
#ifdef SKIP_ASM

/**
 * Returns the game's AI manager (SSXApp->Game->AI).
 *
 * Static accessor of the audio code (On Tour cAudioInterface); dereferences SSXApp->Game without a null check, so it is
 * only valid in a game. cSSXAudio_OnFinishEvent passes it to cAI_isRaceFinished.
 *
 * @return SSXApp->Game->AI
 */
extern "C" void* cAudioInterface_getAI()
{
    return SSXApp->Game->AI;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_getBE);
#ifdef SKIP_ASM

/**
 * Returns the back end, SSXApp->BE.
 *
 * @return SSXApp->BE
 */
extern "C" void* cAudioInterface_getBE()
{
    return SSXApp->BE;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_getNumGameView);
#ifdef SKIP_ASM

/**
 * Returns the number of game views: the word at +0x10 of SSXApp->Game->ViewMan.
 *
 * cSSXAudio_GetNumViews uses it when the app runs several views.
 *
 * @return the view count
 */
extern "C" unsigned int cAudioInterface_getNumGameView()
{
    return SSXApp->Game->ViewMan->m_uiNumView;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_getGameView);
#ifdef SKIP_ASM

/**
 * Returns game view @p i's camera: SSXApp->Game->ViewMan->PlayerCamera[@p i].
 *
 * cSSXAudio_GetCameraPos and cSSXAudio_GetClosestCamera1 use it.
 *
 * @param i  the view index
 * @return the view's camera
 */
extern "C" cCamera* cAudioInterface_getGameView(int i)
{
    return SSXApp->Game->ViewMan->PlayerCamera[i];
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_getWorld);
#ifdef SKIP_ASM
/**
 * Returns the world, SSXApp->Game->World.
 *
 * Nothing in the program calls it.
 *
 * @return SSXApp->Game->World
 */
extern "C" cWorld* cAudioInterface_getWorld(void) {
    return SSXApp->Game->World;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_getTimerHz__FPv);
#ifdef SKIP_ASM
/**
 * Returns the audio timer rate, 60.
 *
 * @return 60 (0x3C)
 */
int cAudioInterface_getTimerHz(void* self)
{
    return 0x3C;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", __13cBankInstance);
#ifdef SKIP_ASM
/**
 * Constructs an empty sound-bank instance.
 *
 * On Tour's cBankInstance layout (same offsets): mBankStatus (+0x0) 0 (EMPTY), mBankHandle (+0x4) -1, mAllocLocation
 * (+0x8) 0 (dynamic), mSoundMemHandle (+0xC) -1, mpMainMemData (+0x10) and mMainMemDataSize (+0x14) 0, no share links
 * mpNext / mpPrev (+0x18 / +0x1C) and an empty mBankFilename (+0x20). cBankManager_cBankManager builds its bank table
 * ("BankInstances") with it.
 */
cBankInstance::cBankInstance()
{
    mBankStatus = 0;
    mBankHandle = -1;
    mAllocLocation = 0;
    mSoundMemHandle = -1;
    mpMainMemData = 0;
    mMainMemDataSize = 0;
    mpNext = 0;
    mpPrev = 0;
    mBankFilename[0] = 0;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", _$_13cBankInstance);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cBankInstance_Unload(cBankInstance* self);
extern "C" void cBankInstance_UnshareBank(void* self);
extern "C" void cBankInstance_DeallocMem(void* self);

/**
 * Destroys a sound-bank instance: unloads it (cBankInstance_Unload), then runs cBankInstance_UnshareBank and cBankInstance_DeallocMem, and frees it
 * with operator_delete when bit 0 of g++'s delete flag is set.
 *
 * cBankManager__cBankManager destroys its bank table with it.
 */
cBankInstance::~cBankInstance()
{
    Unload();
    cBankInstance_UnshareBank(this);
    cBankInstance_DeallocMem(this);
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cBankInstance_SharedUnload);
#ifdef SKIP_ASM
extern "C" void cBankInstance_Unload(cBankInstance* self);

/**
 * Unloads every other instance sharing this bank: walks the share ring through +0x18 (On Tour mpNext) back to this
 * instance and runs cBankInstance_Unload on each.
 *
 * Does nothing when the instance is not shared (+0x18 null).
 */
void cBankInstance::SharedUnload()
{
    if (mpNext != 0) {
        for (cBankInstance* node = mpNext; node != this; node = node->mpNext) {
            node->Unload();
        }
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cBankInstance_SyncLoad);
#ifdef SKIP_ASM
extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* FILE_loadsizez(const char* name, int* size, int align);
extern "C" void* MEM_free(void* self);
extern "C" void* MEM_alloc(void* a0, int a1, int a2);
extern "C" void cBankInstance_SharedUnload(cBankInstance* self);
extern "C" void cBankInstance_Unload(cBankInstance* self);
extern "C" int SNDbankadd(int* out, void* bank);
extern "C" int SNDbankheadersize(int id);
extern "C" int SNDbankheadercopy(void* destMem, int bankHandle);
extern "C" int FILE_loadatz(const char* name, void* data, int size);
extern cBankManager* gpBankManager;

/**
 * Loads a sound-bank file synchronously into this bank instance and registers it with the sound library.
 *
 * First unloads the instance (cBankInstance_Unload) and the instances sharing it (cBankInstance_SharedUnload). By +0x8 (On Tour
 * mAllocLocation, an eSoundMemLocation): keSoundMemLocation_Dynamic loads the whole file with FILE_loadsizez using
 * @p align as its flags, keSoundMemLocation_Sound the same with flags 0x100, keSoundMemLocation_Main loads into the
 * instance's main memory (+0x10 mpMainMemData, +0x14 mMainMemDataSize bytes) with FILE_loadatz; a missing file returns
 * at once. For sound memory, virtual slot 6 of the
 * sound system reached through gpBankManager + 0x1D8 gets +0xC (mSoundMemHandle) first.
 *
 * SNDbankadd adds the bank and writes its handle to +0x4 (mBankHandle). Result kSNDBANKADD_Uploaded (7) keeps only the
 * header (SNDbankheadersize sizes it, SNDbankheadercopy copies it): into mpMainMemData for sound memory, otherwise into
 * a new MEM_alloc block tagged with the file name, which becomes mpMainMemData / mMainMemDataSize while the loaded file
 * is freed (MEM_free). kSNDBANKADD_Resident (8) keeps the loaded data (recorded as mpMainMemData and its size for a
 * dynamic bank). Both set +0x0 (mBankStatus) to USED; any other result frees the loaded data (unless it is the
 * instance's main memory) and sets it to EMPTY. Finally calls slot 7 of the same sound-system object and copies @p file
 * into +0x20 (mBankFilename). cBankManager_BANK_SyncLoad calls it.
 *
 * @param file  the bank file name
 * @param align  allocation flags for FILE_loadsizez when the bank is dynamic (On Tour mainMemFlag); despite the name,
 *               not an alignment
 */
void cBankInstance::SyncLoad(const char* file, int align)
{
    void* data = 0;
    Unload();
    SharedUnload();
    int size = 0;
    switch (mAllocLocation) {
    case keSoundMemLocation_Dynamic:
        data = FILE_loadsizez(file, &size, align);
        if (data == 0) {
            return;
        }
        break;
    case keSoundMemLocation_Sound:
        data = FILE_loadsizez(file, &size, 0x100);
        if (data == 0) {
            return;
        }
        break;
    case keSoundMemLocation_Main:
        if (FILE_loadatz(file, mpMainMemData, mMainMemDataSize) == 0) {
            return;
        }
        size = mMainMemDataSize;
        data = mpMainMemData;
        break;
    }
    if (mAllocLocation == keSoundMemLocation_Sound) {
        ((cSoundManager*)gpBankManager->BankMonitor.mpVSoundSys->pManager)->SetSoundMemLoadLimits(mSoundMemHandle);
    }
    switch (SNDbankadd(&mBankHandle, data)) {
    case kSNDBANKADD_Uploaded: {
        int headerSize = SNDbankheadersize(mBankHandle);
        if (mAllocLocation == keSoundMemLocation_Sound) {
            SNDbankheadercopy(mpMainMemData, mBankHandle);
        } else {
            void* headerMem = MEM_alloc((void*)file, headerSize, 0);
            mpMainMemData = (char*)headerMem;
            SNDbankheadercopy(headerMem, mBankHandle);
            MEM_free(data);
            mMainMemDataSize = headerSize;
        }
        mBankStatus = USED;
        break;
    }
    case kSNDBANKADD_Resident:
        if (mAllocLocation == keSoundMemLocation_Dynamic) {
            mpMainMemData = (char*)data;
            mMainMemDataSize = size;
        }
        mBankStatus = USED;
        break;
    default:
        if (mAllocLocation != keSoundMemLocation_Main) {
            MEM_free(data);
        }
        mBankStatus = EMPTY;
        break;
    }
    ((cSoundManager*)gpBankManager->BankMonitor.mpVSoundSys->pManager)->RestoreSoundMemLoadLimits();
    strcpy(mBankFilename, file);
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cBankInstance_Unload);
#ifdef SKIP_ASM
extern "C" int SNDbankremove(int id);
extern "C" void* MEM_free(void* self);

/**
 * Unloads this sound-bank instance when it holds a bank.
 *
 * When +0x0 (On Tour mBankStatus) is USED: sets it to EMPTY, removes the bank from the sound library (SNDbankremove on
 * +0x4, mBankHandle, which becomes -1) and, for a dynamic bank (+0x8 mAllocLocation keSoundMemLocation_Dynamic),
 * frees +0x10 (mpMainMemData) with MEM_free and clears it and +0x14 (mMainMemDataSize).
 */
void cBankInstance::Unload()
{
    if (mBankStatus == USED) {
        mBankStatus = EMPTY;
        SNDbankremove(mBankHandle);
        mBankHandle = -1;
        if (mAllocLocation == keSoundMemLocation_Dynamic) {
            MEM_free(mpMainMemData);
            mpMainMemData = 0;
            mMainMemDataSize = 0;
        }
    }
}
#endif

