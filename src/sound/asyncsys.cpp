#include "common.h"
#include "sound/ssxaudio.h"
#include "main/game.h"
#include "main/ssxapp.h"

void* operator_new(unsigned int size);

struct cAsyncSys {
    char pad_0x00[0x1CC];
    void* field_0x1CC;
    int field_0x1D0;
};

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
void cAsyncSys_ASYNCSYS_Init(cAsyncSys* self, unsigned int x, int flags)
{
    if (x != 0) {
        self->field_0x1D0 = x;
        self->field_0x1CC = operator_new_tag(x, str_AsyncSysBuffer, flags, 0);
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_DeInit);
#ifdef SKIP_ASM
extern "C" void func_00289DF0(void* self, int a);
extern "C" void func_003E4FB0(int a);
void cMemMan_free(void* p);

/**
 * Shuts the async system down: services it until no request is pending, then frees its buffer.
 *
 * Calls func_00289DF0(self, 1) and, while the pending count (+0x4) is nonzero, sleeps (func_003E4FB0(2)) and calls
 * it again. Frees the "AsyncSysBuffer" (+0x1CC) and clears it and its size (+0x1D0).
 */
extern "C" void cAsyncSys_ASYNCSYS_DeInit(cAsyncSys* self)
{
    func_00289DF0(self, 1);
    while (*(int*)((char*)self + 4) != 0) {
        func_003E4FB0(2);
        func_00289DF0(self, 1);
    }
    if (self->field_0x1CC != 0) {
        cMemMan_free(self->field_0x1CC);
        self->field_0x1CC = 0;
        self->field_0x1D0 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_StartLoad);
#ifdef SKIP_ASM
extern "C" int func_003DF748(char* name, int a1, int a2);
extern "C" int ASYNCFILE_load(char* name, int a1);

struct sRing_A298 {
    int size;
    int* data;
    int head;
    int tail;
};

struct sVE_A298 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int);
};

class cAsync_A298 {
public:
    char data[0x1D4];
    // vptr at 0x1D4; slot N at vtable offset N*8
    virtual void v01();
    virtual void notify(int id);
};

struct sAsync_A298 {
    int result;             // 0x0
    int busy;               // 0x4
    int slot;               // 0x8
    char names[6][0x40];    // 0xC
    char pad18C[0x18C - 0xC - 6 * 0x40];
    sRing_A298 q18C;        // 0x18C
    sRing_A298 q19C;        // 0x19C
    sRing_A298 q1AC;        // 0x1AC
    sRing_A298 q1BC;        // 0x1BC
    void* buf;              // 0x1CC
    int bufSize;            // 0x1D0
    sVE_A298* vt;           // 0x1D4
};

/**
 * Starts the next queued sound-bank load when the async system is idle and something is queued.
 *
 * Field names are On Tour's cAsyncSys (same offsets): sets mBusy (+0x4) and tells the owner through virtual slot 2
 * (cBankManager_ASYNCSYS_OnBeginAsyncLoad; vtable at +0x1D4) with the front user handle (queue mUserHandle, +0x18C).
 * Then by the front memory type (queue mMemType, +0x1AC; On Tour eSoundMemLocation 0 dynamic, 1 main, 2 sound): 0 and 2
 * load the front file name (mFilename slot from queue mFilenameIndex, +0x19C) into the shared buffer mpBuffer (+0x1CC)
 * of mBufferSize (+0x1D0) bytes with func_003DF748; 1 asks the owner for main memory through slots 3
 * (cBankManager_ASYNCSYS_OnAsyncMainMemAlloc, size -1 and the front flags of queue mMainMemFlags, +0x1BC) and 4
 * (cBankManager_ASYNCSYS_OnAsyncMainMemSize) and loads there with func_003DF748, or with ASYNCFILE_load and those flags
 * when no memory came back. The file handle goes to mHandle (+0x0). Any other memory type starts nothing but leaves
 * mBusy set.
 *
 * The queues are only peeked here; cAsyncSys_ASYNCSYS_Update pops them when the load has ended.
 */
extern "C" void cAsyncSys_ASYNCSYS_StartLoad(void* self)
{
    sAsync_A298* asyncSys = (sAsync_A298*)self;
    if (asyncSys->busy != 0) {
        return;
    }
    sRing_A298* userHandleQueue = &asyncSys->q18C;
    if (asyncSys->q18C.head == asyncSys->q18C.tail) {
        return;
    }
    asyncSys->busy = 1;
    ((cAsync_A298*)self)->notify(userHandleQueue->data[userHandleQueue->tail]);
    int kind;
    {
        sRing_A298* peekQueue = &asyncSys->q1AC;
        if (asyncSys->q1AC.head == asyncSys->q1AC.tail) {
            kind = -1;
        } else {
            kind = peekQueue->data[peekQueue->tail];
        }
    }
    switch (kind) {
    case 0: {
        int idx;
        sRing_A298* peekQueue = &asyncSys->q19C;
        if (asyncSys->q19C.head == asyncSys->q19C.tail) {
            idx = -1;
        } else {
            idx = peekQueue->data[peekQueue->tail];
        }
        asyncSys->result = func_003DF748(asyncSys->names[idx], (int)asyncSys->buf, asyncSys->bufSize);
        break;
    }
    case 2: {
        int idx;
        sRing_A298* peekQueue = &asyncSys->q19C;
        if (asyncSys->q19C.head == asyncSys->q19C.tail) {
            idx = -1;
        } else {
            idx = peekQueue->data[peekQueue->tail];
        }
        asyncSys->result = func_003DF748(asyncSys->names[idx], (int)asyncSys->buf, asyncSys->bufSize);
        break;
    }
    case 1: {
        int idx;
        {
            sRing_A298* peekQueue = &asyncSys->q19C;
            if (asyncSys->q19C.head == asyncSys->q19C.tail) {
                idx = -1;
            } else {
                idx = peekQueue->data[peekQueue->tail];
            }
        }
        char* name = asyncSys->names[idx];
        int allocHandle, allocFlags;
        // PORT: g++ 2.95 virtual calls (vptr at 0x1D4) written out by hand: delta + saved &pfn, args evaluated after.
        sVE_A298* vt = asyncSys->vt;
        char* thisp = (char*)asyncSys + vt[3].delta;
        int (**pf)(void*, int, int, int) = &vt[3].fn;
        if (asyncSys->q18C.head == asyncSys->q18C.tail) {
            allocHandle = -1;
        } else {
            allocHandle = userHandleQueue->data[userHandleQueue->tail];
        }
        sRing_A298* flagQueue = &asyncSys->q1BC;
        if (asyncSys->q1BC.head == asyncSys->q1BC.tail) {
            allocFlags = -1;
        } else {
            allocFlags = flagQueue->data[flagQueue->tail];
        }
        int allocBuffer = (*pf)(thisp, allocHandle, -1, allocFlags);
        sVE_A298* vt2 = asyncSys->vt;
        char* thisp2 = (char*)asyncSys + vt2[4].delta;
        int (**pf2)(void*, int, int, int) = &vt2[4].fn;
        int sizeHandle;
        if (asyncSys->q18C.head == asyncSys->q18C.tail) {
            sizeHandle = -1;
        } else {
            sizeHandle = userHandleQueue->data[userHandleQueue->tail];
        }
        int allocSize = ((int (*)(void*, int))*pf2)(thisp2, sizeHandle);
        if (allocBuffer != 0) {
            asyncSys->result = func_003DF748(name, allocBuffer, allocSize);
        } else {
            int loadFlags;
            if (asyncSys->q1BC.head == asyncSys->q1BC.tail) {
                loadFlags = -1;
            } else {
                loadFlags = flagQueue->data[flagQueue->tail];
            }
            asyncSys->result = ASYNCFILE_load(name, loadFlags);
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
extern "C" void cAsyncSys_ASYNCSYS_StartLoad(void* self);

struct sRing28A558 {
    int size;
    int* data;
    int head;
    int tail;
};

struct sAsync28A558 {
    char pad0[0x8];
    int slot;              // 0x8
    char names[6][0x40];   // 0xC
    char pad18C[0x18C - 0xC - 6 * 0x40];
    sRing28A558 q18C;      // 0x18C
    sRing28A558 q19C;      // 0x19C
    sRing28A558 q1AC;      // 0x1AC
    sRing28A558 q1BC;      // 0x1BC
};

/**
 * Queues a sound-bank file for asynchronous loading and starts it when the system is idle.
 *
 * Pushes @p bankId onto the user-handle queue (+0x18C, On Tour mUserHandle), @p memType onto the memory-type queue (+0x1AC,
 * mMemType) and @p memFlags onto the flags queue (+0x1BC, mMainMemFlags); each push is skipped when that queue is full.
 * Copies @p name into the file-name slot +0x8 (mFileIndex) of the six 64-byte names at +0xC, pushes that slot onto the
 * name-index queue (+0x19C), advances the slot (back to 0 after 5) and calls cAsyncSys_ASYNCSYS_StartLoad. The name ring has six slots
 * whatever the queue size, so a seventh pending load overwrites the oldest name. func_0028BD10 (the bank manager's
 * async load) calls it.
 *
 * @param bankId  the bank's user handle, given back to the owner's callbacks (On Tour bankID)
 * @param name  the file name
 * @param memType  where the bank goes: 0 dynamic (shared buffer), 1 main memory, 2 sound memory (On Tour eSoundMemLocation)
 * @param memFlags  main-memory allocation flags (On Tour allocMainMemFlags)
 */
extern "C" void cAsyncSys_ASYNCSYS_QueueLoad(void* self, int bankId, const char* name, int memType, int memFlags)
{
    sAsync28A558* asyncSys = (sAsync28A558*)self;
    {
        sRing28A558* pushQueue = &asyncSys->q18C;
        if ((asyncSys->q18C.head + 1) % asyncSys->q18C.size != asyncSys->q18C.tail) {
            pushQueue->data[pushQueue->head] = bankId;
            if (++asyncSys->q18C.head >= asyncSys->q18C.size) {
                asyncSys->q18C.head = 0;
            }
        }
    }
    {
        sRing28A558* pushQueue = &asyncSys->q1AC;
        if ((asyncSys->q1AC.head + 1) % asyncSys->q1AC.size != asyncSys->q1AC.tail) {
            pushQueue->data[pushQueue->head] = memType;
            if (++asyncSys->q1AC.head >= asyncSys->q1AC.size) {
                asyncSys->q1AC.head = 0;
            }
        }
    }
    {
        sRing28A558* pushQueue = &asyncSys->q1BC;
        if ((asyncSys->q1BC.head + 1) % asyncSys->q1BC.size != asyncSys->q1BC.tail) {
            pushQueue->data[pushQueue->head] = memFlags;
            if (++asyncSys->q1BC.head >= asyncSys->q1BC.size) {
                asyncSys->q1BC.head = 0;
            }
        }
    }
    strcpy(asyncSys->names[asyncSys->slot], name);
    int slot = asyncSys->slot++;
    {
        sRing28A558* pushQueue = &asyncSys->q19C;
        if ((asyncSys->q19C.head + 1) % asyncSys->q19C.size != asyncSys->q19C.tail) {
            pushQueue->data[pushQueue->head] = slot;
            if (++asyncSys->q19C.head >= asyncSys->q19C.size) {
                asyncSys->q19C.head = 0;
            }
        }
    }
    if (asyncSys->slot >= 6) {
        asyncSys->slot = 0;
    }
    cAsyncSys_ASYNCSYS_StartLoad(self);
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_Cancel);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void func_00289DF0(void* self, int a);
extern "C" void cAsyncSys_ASYNCSYS_StartLoad(void* self);

struct sRing_A728 {
    int size;
    int* data;
    int head;
    int tail;
};

struct sAsync_A728 {
    int handle;             // 0x0
    int busy;               // 0x4
    int slot;               // 0x8
    char names[6][0x40];    // 0xC
    char pad18C[0x18C - 0xC - 6 * 0x40];
    sRing_A728 q18C;        // 0x18C
    sRing_A728 q19C;        // 0x19C
    sRing_A728 q1AC;        // 0x1AC
    sRing_A728 q1BC;        // 0x1BC
};

static inline int IsEmpty18C_A728(sAsync_A728* s)
{
    return s->q18C.head == s->q18C.tail;
}

static inline int IsEmpty19C_A728(sAsync_A728* s)
{
    return s->q19C.head == s->q19C.tail;
}

static inline int IsEmpty1AC_A728(sAsync_A728* s)
{
    return s->q1AC.head == s->q1AC.tail;
}

static inline int IsEmpty1BC_A728(sAsync_A728* s)
{
    return s->q1BC.head == s->q1BC.tail;
}

static inline int Peek18C_A728(sAsync_A728* s)
{
    sRing_A728* q = &s->q18C;
    if (IsEmpty18C_A728(s)) {
        return -1;
    }
    return q->data[q->tail];
}

static inline int Pop18C_A728(sAsync_A728* s)
{
    sRing_A728* q = &s->q18C;
    if (IsEmpty18C_A728(s)) {
        return -1;
    }
    int v = q->data[q->tail];
    if (++s->q18C.tail >= s->q18C.size) {
        s->q18C.tail = 0;
    }
    return v;
}

static inline int Pop19C_A728(sAsync_A728* s)
{
    sRing_A728* q = &s->q19C;
    if (IsEmpty19C_A728(s)) {
        return -1;
    }
    int v = q->data[q->tail];
    if (++s->q19C.tail >= s->q19C.size) {
        s->q19C.tail = 0;
    }
    return v;
}

static inline int Pop1AC_A728(sAsync_A728* s)
{
    sRing_A728* q = &s->q1AC;
    if (IsEmpty1AC_A728(s)) {
        return -1;
    }
    int v = q->data[q->tail];
    if (++s->q1AC.tail >= s->q1AC.size) {
        s->q1AC.tail = 0;
    }
    return v;
}

static inline int Pop1BC_A728(sAsync_A728* s)
{
    sRing_A728* q = &s->q1BC;
    if (IsEmpty1BC_A728(s)) {
        return -1;
    }
    int v = q->data[q->tail];
    if (++s->q1BC.tail >= s->q1BC.size) {
        s->q1BC.tail = 0;
    }
    return v;
}

static inline void Push18C_A728(sAsync_A728* s, int v)
{
    sRing_A728* q = &s->q18C;
    if ((s->q18C.head + 1) % s->q18C.size != s->q18C.tail) {
        q->data[q->head] = v;
        if (++s->q18C.head >= s->q18C.size) {
            s->q18C.head = 0;
        }
    }
}

static inline void Push19C_A728(sAsync_A728* s, int v)
{
    sRing_A728* q = &s->q19C;
    if ((s->q19C.head + 1) % s->q19C.size != s->q19C.tail) {
        q->data[q->head] = v;
        if (++s->q19C.head >= s->q19C.size) {
            s->q19C.head = 0;
        }
    }
}

static inline void Push1AC_A728(sAsync_A728* s, int v)
{
    sRing_A728* q = &s->q1AC;
    if ((s->q1AC.head + 1) % s->q1AC.size != s->q1AC.tail) {
        q->data[q->head] = v;
        if (++s->q1AC.head >= s->q1AC.size) {
            s->q1AC.head = 0;
        }
    }
}

static inline void Push1BC_A728(sAsync_A728* s, int v)
{
    sRing_A728* q = &s->q1BC;
    if ((s->q1BC.head + 1) % s->q1BC.size != s->q1BC.tail) {
        q->data[q->head] = v;
        if (++s->q1BC.head >= s->q1BC.size) {
            s->q1BC.head = 0;
        }
    }
}

/**
 * Cancels every load queued for one bank, aborting it when it is the one in progress.
 *
 * When busy (+0x4) and the front user handle (+0x18C) is @p id, calls func_00289DF0(this, 0) and returns. Otherwise
 * pops every queued entry from the four queues (user handle, name index, memory type, flags) and pushes back those
 * whose user handle is not @p id, which keeps their order; then, when not busy, starts the next load (cAsyncSys_ASYNCSYS_StartLoad).
 * func_0028BC58 calls it.
 *
 * @param id  the bank's user handle (On Tour bankID)
 */
extern "C" void cAsyncSys_ASYNCSYS_Cancel(void* self, int id)
{
    sAsync_A728* asyncSys = (sAsync_A728*)self;
    if (asyncSys->busy != 0 && Peek18C_A728(asyncSys) == id) {
        func_00289DF0(self, 0);
        return;
    }
    int n = (asyncSys->q18C.head - asyncSys->q18C.tail + asyncSys->q18C.size) % asyncSys->q18C.size;
    for (int i = 0; i < n; i++) {
        int queuedHandle = Pop18C_A728(asyncSys);
        if (queuedHandle == id) {
            Pop19C_A728(asyncSys);
            Pop1AC_A728(asyncSys);
            Pop1BC_A728(asyncSys);
        } else {
            Push18C_A728(asyncSys, queuedHandle);
            Push19C_A728(asyncSys, Pop19C_A728(asyncSys));
            Push1AC_A728(asyncSys, Pop1AC_A728(asyncSys));
            Push1BC_A728(asyncSys, Pop1BC_A728(asyncSys));
        }
    }
    if (asyncSys->busy == 0) {
        cAsyncSys_ASYNCSYS_StartLoad(self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_Update);
#ifdef SKIP_ASM
extern "C" int func_003B6098(void* sema, int a1);
extern "C" int func_003B6300(int id);
extern "C" int func_003B6528(int id);
extern "C" int f_6548_AAF8(void* data, int id) __asm__("func_003B6548");
extern "C" int ASYNCFILE_getstatus(int handle);
extern "C" void MEM_copy(void* dst, void* src, int n);
extern "C" int ASYNCFILE_release(int handle, int* buf, int* size);
extern "C" void cAsyncSys_ASYNCSYS_StartLoad(void* self);

struct sRing_AAF8 {
    int size;
    int* data;
    int head;
    int tail;
};

struct sVE_AAF8 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int);
};

typedef void (*tDone_AAF8)(void*, int, int, int, char*, int, int);

struct sAsync_AAF8 {
    int result;             // 0x0
    int busy;               // 0x4
    int slot;               // 0x8
    char names[6][0x40];    // 0xC
    char pad18C[0x18C - 0xC - 6 * 0x40];
    sRing_AAF8 q18C;        // 0x18C
    sRing_AAF8 q19C;        // 0x19C
    sRing_AAF8 q1AC;        // 0x1AC
    sRing_AAF8 q1BC;        // 0x1BC
    void* buf;              // 0x1CC
    int bufSize;            // 0x1D0
    sVE_AAF8* vt;           // 0x1D4
};

/**
 * Polls the sound-bank load in progress and, once it has finished, registers the bank, hands it to the owner and starts
 * the next load.
 *
 * Only while busy (+0x4). By the front memory type (+0x1AC): for 0 and 2 (loaded into the shared buffer) and for 1
 * (main memory) it checks ASYNCFILE_getstatus on the file handle (+0x0): 0 is still loading; 1 is done:
 * ASYNCFILE_release gives the data and size. Shared buffer: func_003B6098 adds the bank and writes its handle to +0x0;
 * unless it returns 7 the bank is removed again (func_003B6300), the owner provides main memory of the full size
 * (virtual slot 3, cBankManager_ASYNCSYS_OnAsyncMainMemAlloc, with the front user handle and flags), the data is copied
 * there (MEM_copy) and added from there; on 7 only the header is kept: func_003B6528 gives its size, the owner provides
 * that much and func_003B6548 copies it. Main memory: the data is added where it is. The owner then gets virtual slot 5
 * (cBankManager_ASYNCSYS_OnEndAsyncLoad) with the user handle, the bank handle, end state 0 (complete), the file name,
 * the memory and the size; an empty file reports handle -1 and end state 2 (error). Then mBusy is cleared, all four
 * queues are popped and cAsyncSys_ASYNCSYS_StartLoad starts the next load. Any status other than 0 or 1 cancels through
 * func_00289DF0(this, 0).
 *
 * func_002AB958 (the stream system's update) calls it.
 */
extern "C" void cAsyncSys_ASYNCSYS_Update(void* self)
{
    sAsync_AAF8* asyncSys = (sAsync_AAF8*)self;
    if (asyncSys->busy == 0) {
        return;
    }
    int kind;
    {
        sRing_AAF8* memTypeQueue = &asyncSys->q1AC;
        if (asyncSys->q1AC.head == asyncSys->q1AC.tail) {
            kind = -1;
        } else {
            kind = memTypeQueue->data[memTypeQueue->tail];
        }
    }
    switch (kind) {
    case 0:
    case 2:
        switch (ASYNCFILE_getstatus(asyncSys->result)) {
        case 0:
            break;
        case 1: {
            int buf = 0;
            int size = 0;
            ASYNCFILE_release(asyncSys->result, &buf, &size);
            if (size > 0) {
                if (func_003B6098(asyncSys, buf) != 7) {
                    func_003B6300(asyncSys->result);
                    sRing_AAF8* userHandleQueue = &asyncSys->q18C;
                    sVE_AAF8* vt = asyncSys->vt;
                    char* thisp = (char*)asyncSys + vt[3].delta;
                    int (**pf)(void*, int, int, int) = &vt[3].fn;
                    int allocHandle;
                    int allocFlags;
                    if (asyncSys->q18C.head == asyncSys->q18C.tail) {
                        allocHandle = -1;
                    } else {
                        allocHandle = userHandleQueue->data[userHandleQueue->tail];
                    }
                    int n = size;
                    {
                        sRing_AAF8* flagQueue = &asyncSys->q1BC;
                        if (asyncSys->q1BC.head == asyncSys->q1BC.tail) {
                            allocFlags = -1;
                        } else {
                            allocFlags = flagQueue->data[flagQueue->tail];
                        }
                    }
                    int allocBuffer = (*pf)(thisp, allocHandle, n, allocFlags);
                    MEM_copy((void*)allocBuffer, (void*)buf, size);
                    func_003B6098(asyncSys, allocBuffer);
                    sVE_AAF8* vt2 = asyncSys->vt;
                    char* thisp2 = (char*)asyncSys + vt2[5].delta;
                    tDone_AAF8* pf2 = (tDone_AAF8*)&vt2[5].fn;
                    int endHandle;
                    int idx;
                    if (asyncSys->q18C.head == asyncSys->q18C.tail) {
                        endHandle = -1;
                    } else {
                        endHandle = userHandleQueue->data[userHandleQueue->tail];
                    }
                    {
                        sRing_AAF8* nameIndexQueue = &asyncSys->q19C;
                        if (asyncSys->q19C.head == asyncSys->q19C.tail) {
                            idx = -1;
                        } else {
                            idx = nameIndexQueue->data[nameIndexQueue->tail];
                        }
                    }
                    (*pf2)(thisp2, endHandle, asyncSys->result, 0, asyncSys->names[idx], allocBuffer, size);
                } else {
                    int id = func_003B6528(asyncSys->result);
                    sRing_AAF8* userHandleQueue = &asyncSys->q18C;
                    // PORT: g++ 2.95 virtual calls (vptr at 0x1D4) written out by hand: delta + saved &pfn, args evaluated after.
                    sVE_AAF8* vt = asyncSys->vt;
                    char* thisp = (char*)asyncSys + vt[3].delta;
                    int (**pf)(void*, int, int, int) = &vt[3].fn;
                    int allocHandle;
                    int allocFlags;
                    if (asyncSys->q18C.head == asyncSys->q18C.tail) {
                        allocHandle = -1;
                    } else {
                        allocHandle = userHandleQueue->data[userHandleQueue->tail];
                    }
                    {
                        sRing_AAF8* flagQueue = &asyncSys->q1BC;
                        if (asyncSys->q1BC.head == asyncSys->q1BC.tail) {
                            allocFlags = -1;
                        } else {
                            allocFlags = flagQueue->data[flagQueue->tail];
                        }
                    }
                    int allocBuffer = (*pf)(thisp, allocHandle, id, allocFlags);
                    f_6548_AAF8((void*)allocBuffer, asyncSys->result);
                    sVE_AAF8* vt2 = asyncSys->vt;
                    char* thisp2 = (char*)asyncSys + vt2[5].delta;
                    tDone_AAF8* pf2 = (tDone_AAF8*)&vt2[5].fn;
                    int endHandle;
                    int idx;
                    if (asyncSys->q18C.head == asyncSys->q18C.tail) {
                        endHandle = -1;
                    } else {
                        endHandle = userHandleQueue->data[userHandleQueue->tail];
                    }
                    {
                        sRing_AAF8* nameIndexQueue = &asyncSys->q19C;
                        if (asyncSys->q19C.head == asyncSys->q19C.tail) {
                            idx = -1;
                        } else {
                            idx = nameIndexQueue->data[nameIndexQueue->tail];
                        }
                    }
                    (*pf2)(thisp2, endHandle, asyncSys->result, 0, asyncSys->names[idx], allocBuffer, id);
                }
            } else {
                sRing_AAF8* userHandleQueue = &asyncSys->q18C;
                sVE_AAF8* vt2 = asyncSys->vt;
                char* thisp2 = (char*)asyncSys + vt2[5].delta;
                tDone_AAF8* pf2 = (tDone_AAF8*)&vt2[5].fn;
                int endHandle;
                int idx;
                if (asyncSys->q18C.head == asyncSys->q18C.tail) {
                    endHandle = -1;
                } else {
                    endHandle = userHandleQueue->data[userHandleQueue->tail];
                }
                {
                    sRing_AAF8* nameIndexQueue = &asyncSys->q19C;
                    if (asyncSys->q19C.head == asyncSys->q19C.tail) {
                        idx = -1;
                    } else {
                        idx = nameIndexQueue->data[nameIndexQueue->tail];
                    }
                }
                (*pf2)(thisp2, endHandle, -1, 2, asyncSys->names[idx], 0, 0);
            }
            asyncSys->busy = 0;
            if (asyncSys->q18C.head != asyncSys->q18C.tail) {
                if (++asyncSys->q18C.tail >= asyncSys->q18C.size) {
                    asyncSys->q18C.tail = 0;
                }
            }
            if (asyncSys->q19C.head != asyncSys->q19C.tail) {
                if (++asyncSys->q19C.tail >= asyncSys->q19C.size) {
                    asyncSys->q19C.tail = 0;
                }
            }
            if (asyncSys->q1AC.head != asyncSys->q1AC.tail) {
                if (++asyncSys->q1AC.tail >= asyncSys->q1AC.size) {
                    asyncSys->q1AC.tail = 0;
                }
            }
            if (asyncSys->q1BC.head != asyncSys->q1BC.tail) {
                if (++asyncSys->q1BC.tail >= asyncSys->q1BC.size) {
                    asyncSys->q1BC.tail = 0;
                }
            }
            cAsyncSys_ASYNCSYS_StartLoad(asyncSys);
            break;
        }
        case -1:
        default:
            func_00289DF0(asyncSys, 0);
            break;
        }
        break;
    case 1:
        switch (ASYNCFILE_getstatus(asyncSys->result)) {
        case 0:
            break;
        case 1: {
            int buf = 0;
            int size = 0;
            ASYNCFILE_release(asyncSys->result, &buf, &size);
            if (size > 0) {
                func_003B6098(asyncSys, buf);
                sRing_AAF8* userHandleQueue = &asyncSys->q18C;
                sVE_AAF8* vt2 = asyncSys->vt;
                char* thisp2 = (char*)asyncSys + vt2[5].delta;
                tDone_AAF8* pf2 = (tDone_AAF8*)&vt2[5].fn;
                int endHandle;
                int idx;
                if (asyncSys->q18C.head == asyncSys->q18C.tail) {
                    endHandle = -1;
                } else {
                    endHandle = userHandleQueue->data[userHandleQueue->tail];
                }
                {
                    sRing_AAF8* nameIndexQueue = &asyncSys->q19C;
                    if (asyncSys->q19C.head == asyncSys->q19C.tail) {
                        idx = -1;
                    } else {
                        idx = nameIndexQueue->data[nameIndexQueue->tail];
                    }
                }
                (*pf2)(thisp2, endHandle, asyncSys->result, 0, asyncSys->names[idx], buf, size);
            } else {
                sRing_AAF8* userHandleQueue = &asyncSys->q18C;
                sVE_AAF8* vt2 = asyncSys->vt;
                char* thisp2 = (char*)asyncSys + vt2[5].delta;
                tDone_AAF8* pf2 = (tDone_AAF8*)&vt2[5].fn;
                int endHandle;
                int idx;
                if (asyncSys->q18C.head == asyncSys->q18C.tail) {
                    endHandle = -1;
                } else {
                    endHandle = userHandleQueue->data[userHandleQueue->tail];
                }
                {
                    sRing_AAF8* nameIndexQueue = &asyncSys->q19C;
                    if (asyncSys->q19C.head == asyncSys->q19C.tail) {
                        idx = -1;
                    } else {
                        idx = nameIndexQueue->data[nameIndexQueue->tail];
                    }
                }
                (*pf2)(thisp2, endHandle, -1, 2, asyncSys->names[idx], buf, 0);
            }
            asyncSys->busy = 0;
            if (asyncSys->q18C.head != asyncSys->q18C.tail) {
                if (++asyncSys->q18C.tail >= asyncSys->q18C.size) {
                    asyncSys->q18C.tail = 0;
                }
            }
            if (asyncSys->q19C.head != asyncSys->q19C.tail) {
                if (++asyncSys->q19C.tail >= asyncSys->q19C.size) {
                    asyncSys->q19C.tail = 0;
                }
            }
            if (asyncSys->q1AC.head != asyncSys->q1AC.tail) {
                if (++asyncSys->q1AC.tail >= asyncSys->q1AC.size) {
                    asyncSys->q1AC.tail = 0;
                }
            }
            if (asyncSys->q1BC.head != asyncSys->q1BC.tail) {
                if (++asyncSys->q1BC.tail >= asyncSys->q1BC.size) {
                    asyncSys->q1BC.tail = 0;
                }
            }
            cAsyncSys_ASYNCSYS_StartLoad(asyncSys);
            break;
        }
        case -1:
        default:
            func_00289DF0(asyncSys, 0);
            break;
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAsyncSys_ASYNCSYS_OnCurAsyncMainMemAlloc);
#ifdef SKIP_ASM
struct sVE_0028B0F0 { short delta; short index; void (*fn)(void*, int, int, int); };

struct sRing_0028B0F0 {
    int pad0;
    int* data;  // 0x4
    int tail;   // 0x8
    int head;   // 0xC
    char pad10[0x20];
};

struct sAsync_0028B0F0 {
    char pad0[0x18C];
    sRing_0028B0F0 q1;       // 0x18C
    sRing_0028B0F0 q2;       // 0x1BC
    char pad1ec[0];
};

/**
 * Asks the owner for main memory for the load in progress: virtual slot 3 (cBankManager_ASYNCSYS_OnAsyncMainMemAlloc)
 * with the front user handle (+0x18C), @p size and the front allocation flags (+0x1BC).
 *
 * Declared void, but the owner's result (the memory) stays in the return register. Its local queue view names the
 * ring's head and tail words the other way round from the rest of the unit (it reads the tail, +0xC). Nothing in the
 * program calls it.
 *
 * @param size  the number of bytes wanted
 */
extern "C" void cAsyncSys_ASYNCSYS_OnCurAsyncMainMemAlloc(sAsync_0028B0F0* self, int size)
{
    sVE_0028B0F0* vtEntry = &(*(sVE_0028B0F0**)((char*)self + 0x1D4))[3];
    void (**pf)(void*, int, int, int) = &vtEntry->fn;
    char* thisp = (char*)self + vtEntry->delta;
    sRing_0028B0F0* handleQueue = &self->q1;
    int userHandle;
    int memFlags;
    if (self->q1.tail == self->q1.head)
        userHandle = -1;
    else
        userHandle = handleQueue->data[handleQueue->head];
    sRing_0028B0F0* flagsQueue = &self->q2;
    if (self->q2.tail == self->q2.head)
        memFlags = -1;
    else
        memFlags = flagsQueue->data[flagsQueue->head];
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
extern "C" void* cAudioInterface_bxAudio()
{
    return gSSXAudio;
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
    if ((((int)SSXApp->RealFE) != 0) || (((int)SSXApp->DebugFE) != 0)) {
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
 * (func_00288A20, func_0029EEE0, func_0028BF78, bmRestartC2D0, ...) uses it to skip in-game work in the front end.
 *
 * @return 1 if SSXApp->Game is set, else 0
 */
extern "C" int cAudioInterface_isInGame()
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
extern "C" void* cAudioInterface_getApp()
{
    return (char*)SSXApp;
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
extern "C" void* cAudioInterface_getGame()
{
    return SSXApp->Game;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_getAI);
#ifdef SKIP_ASM

/**
 * Returns the game's AI manager (SSXApp->Game->AI).
 *
 * Static accessor of the audio code (On Tour cAudioInterface); dereferences SSXApp->Game without a null check, so it is
 * only valid in a game. func_00286EA0 passes it to cAI_isRaceFinished.
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
extern "C" void* cAudioInterface_getNumGameView()
{
    return *(void**)((char*)SSXApp->Game->ViewMan + 0x10);
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cAudioInterface_getGameView);
#ifdef SKIP_ASM

/**
 * Returns game view @p i's camera: entry @p i of the camera array at +0x4 of SSXApp->Game->ViewMan.
 *
 * cSSXAudio_GetCameraPos and func_00288A20 use it.
 *
 * @param i  the view index
 * @return the view's camera
 */
extern "C" void* cAudioInterface_getGameView(int i)
{
    char* riders = (char*)SSXApp->Game->ViewMan;
    return *(void**)(riders + (i << 2) + 4);
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
 * @return SSXApp->Game->World, as an int
 */
extern "C" int cAudioInterface_getWorld(void) {
    return ((int)SSXApp->Game->World);
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
INCLUDE_ASM("sound/asyncsys", cBankInstance_cBankInstance);
#ifdef SKIP_ASM
/**
 * Constructs an empty sound-bank instance.
 *
 * On Tour's cBankInstance layout (same offsets): mBankStatus (+0x0) 0 (EMPTY), mBankHandle (+0x4) -1, mAllocLocation
 * (+0x8) 0 (dynamic), mSoundMemHandle (+0xC) -1, mpMainMemData (+0x10) and mMainMemDataSize (+0x14) 0, no share links
 * mpNext / mpPrev (+0x18 / +0x1C) and an empty mBankFilename (+0x20). cBankManager_cBankManager builds its bank table
 * ("BankInstances") with it.
 *
 * @return @p self
 */
extern "C" void* cBankInstance_cBankInstance(void* self)
{
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xc) = -1;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x18) = 0;
    *(int*)((char*)self + 0x1c) = 0;
    *(char*)((char*)self + 0x20) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cBankInstance__cBankInstance);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cBankInstance_Unload(void* self);
extern "C" void func_0028B730(void* self);
extern "C" void func_0028B650(void* self);

/**
 * Destroys a sound-bank instance: unloads it (cBankInstance_Unload), then runs func_0028B730 and func_0028B650, and frees it
 * with operator_delete when bit 0 of @p flags is set.
 *
 * cBankManager__cBankManager destroys its bank table with it.
 *
 * @param flags  g++ destructor flags: bit 0 frees the object
 */
extern "C" void cBankInstance__cBankInstance(void* self, int flags)
{
    cBankInstance_Unload(self);
    func_0028B730(self);
    func_0028B650(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cBankInstance_SharedUnload);
#ifdef SKIP_ASM
extern "C" void cBankInstance_Unload(void* self);

/**
 * Unloads every other instance sharing this bank: walks the share ring through +0x18 (On Tour mpNext) back to this
 * instance and runs cBankInstance_Unload on each.
 *
 * Does nothing when the instance is not shared (+0x18 null).
 */
extern "C" void cBankInstance_SharedUnload(void* self)
{
    if (*(void**)((char*)self + 0x18) != 0) {
        for (void* node = *(void**)((char*)self + 0x18); node != self; node = *(void**)((char*)node + 0x18)) {
            cBankInstance_Unload(node);
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
extern "C" void cBankInstance_SharedUnload(void* self);
extern "C" void cBankInstance_Unload(void* self);
extern "C" int func_003B6098(void* sema, int a1);
extern "C" int func_003B6528(int id);
extern "C" void func_003B6548(void* data, int id);
extern "C" int FILE_loadatz(const char* name, void* data, int size);
extern char* gpBankManager;

class cSndStream_B320 {
public:
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6(int id);
    virtual void v7();
};

/**
 * Loads a sound-bank file synchronously into this bank instance and registers it with the sound library.
 *
 * First unloads the instance (cBankInstance_Unload) and the instances sharing it (cBankInstance_SharedUnload). By +0x8 (On Tour
 * mAllocLocation: 0 dynamic, 1 main, 2 sound memory): 0 loads the whole file with FILE_loadsizez using @p align as its
 * flags, 2 the same with flags 0x100, 1 loads into the instance's main memory (+0x10 mpMainMemData, +0x14
 * mMainMemDataSize bytes) with FILE_loadatz; a missing file returns at once. For sound memory, virtual slot 6 of the
 * sound system reached through gpBankManager + 0x1D8 gets +0xC (mSoundMemHandle) first.
 *
 * func_003B6098 adds the bank and writes its handle to +0x4 (mBankHandle). Result 7 keeps only the header
 * (func_003B6528 sizes it, func_003B6548 copies it): into mpMainMemData for sound memory, otherwise into a new
 * MEM_alloc block tagged with the file name, which becomes mpMainMemData / mMainMemDataSize while the loaded file is
 * freed (MEM_free). Result 8 keeps the loaded data (recorded as mpMainMemData and its size for a dynamic bank). Both
 * set +0x0 (mBankStatus) to 1; any other result frees the loaded data (unless it is the instance's main memory) and
 * sets it to 0. Finally calls slot 7 of the same sound-system object and copies @p file into +0x20 (mBankFilename).
 * func_0028BC58 calls it.
 *
 * @param file  the bank file name
 * @param align  allocation flags for FILE_loadsizez when the bank is dynamic (On Tour mainMemFlag); despite the name,
 *               not an alignment
 */
// PORT: data pointers are passed through int parameters of the semaphore/bank helpers
extern "C" void cBankInstance_SyncLoad(void* self, const char* file, int align)
{
    void* data = 0;
    cBankInstance_Unload(self);
    cBankInstance_SharedUnload(self);
    int size = 0;
    switch (*(int*)((char*)self + 0x8)) {
    case 0:
        data = FILE_loadsizez(file, &size, align);
        if (data == 0) {
            return;
        }
        break;
    case 2:
        data = FILE_loadsizez(file, &size, 0x100);
        if (data == 0) {
            return;
        }
        break;
    case 1:
        if (FILE_loadatz(file, *(void**)((char*)self + 0x10), *(int*)((char*)self + 0x14)) == 0) {
            return;
        }
        size = *(int*)((char*)self + 0x14);
        data = *(void**)((char*)self + 0x10);
        break;
    }
    if (*(int*)((char*)self + 0x8) == 2) {
        (**(cSndStream_B320***)(gpBankManager + 0x1D8))->v6(*(int*)((char*)self + 0xC));
    }
    switch (func_003B6098((char*)self + 0x4, (int)data)) {
    case 7: {
        int headerSize = func_003B6528(*(int*)((char*)self + 0x4));
        if (*(int*)((char*)self + 0x8) == 2) {
            func_003B6548(*(void**)((char*)self + 0x10), *(int*)((char*)self + 0x4));
        } else {
            void* headerMem = MEM_alloc((void*)file, headerSize, 0);
            *(void**)((char*)self + 0x10) = headerMem;
            func_003B6548(headerMem, *(int*)((char*)self + 0x4));
            MEM_free(data);
            *(int*)((char*)self + 0x14) = headerSize;
        }
        *(int*)self = 1;
        break;
    }
    case 8:
        if (*(int*)((char*)self + 0x8) == 0) {
            *(void**)((char*)self + 0x10) = data;
            *(int*)((char*)self + 0x14) = size;
        }
        *(int*)self = 1;
        break;
    default:
        if (*(int*)((char*)self + 0x8) != 1) {
            MEM_free(data);
        }
        *(int*)self = 0;
        break;
    }
    (**(cSndStream_B320***)(gpBankManager + 0x1D8))->v7();
    strcpy((char*)self + 0x20, file);
}
#endif

//100%
INCLUDE_ASM("sound/asyncsys", cBankInstance_Unload);
#ifdef SKIP_ASM
extern "C" int func_003B6300(int id);
extern "C" void* MEM_free(void* self);

/**
 * Unloads this sound-bank instance when it holds a bank.
 *
 * When +0x0 (On Tour mBankStatus) is 1: sets it to 0, removes the bank from the sound library (func_003B6300 on +0x4,
 * mBankHandle, which becomes -1) and, for a dynamic bank (+0x8 mAllocLocation 0), frees +0x10 (mpMainMemData) with
 * MEM_free and clears it and +0x14 (mMainMemDataSize).
 */
extern "C" void cBankInstance_Unload(void* self)
{
    if (*(int*)self == 1) {
        *(int*)self = 0;
        func_003B6300(*(int*)((char*)self + 0x4));
        *(int*)((char*)self + 0x4) = -1;
        if (*(int*)((char*)self + 0x8) == 0) {
            MEM_free(*(void**)((char*)self + 0x10));
            *(int*)((char*)self + 0x10) = 0;
            *(int*)((char*)self + 0x14) = 0;
        }
    }
}
#endif

