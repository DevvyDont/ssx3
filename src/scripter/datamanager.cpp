#include "common.h"
#include "scripter/datamanager.h"
#include "scripter/nislist.h"
#include "inseq/scriptplayermain.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_cDataManager);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cDataManager_AddToEmptyInstanceList(void* self, void* node);
extern char cDataManager_vtbl[];
extern const char str_tDataInstanceInfo[];



/**
 * Constructs a data manager with a pool of data-instance records, all placed on the empty list.
 *
 * @param flags  memory flags for the record pool ("tDataInstanceInfo")
 * @param count  number of instance records (On Tour maxDataInstances)
 * @return @p self
 */
extern "C" cDataManager* cDataManager_cDataManager(cDataManager* self, int flags, int count)
{
    *(void**)&self->mVtbl = cDataManager_vtbl;
    self->mMaxDataInstances = count;
    *(tDataInstanceInfo**)&self->mpDataInstanceHeap = (tDataInstanceInfo*)operator_new_tag(count * sizeof(tDataInstanceInfo), str_tDataInstanceInfo, flags, 0);
    *(int*)&self->mpEmptyDataInstanceList = 0;
    for (int i = 0; i < count; i++) {
        cDataManager_AddToEmptyInstanceList(self, &(self->mpDataInstanceHeap)[i]);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager__cDataManager);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern char cDataManager_vtbl[];

/**
 * Destroys the data manager.
 *
 * Restores cDataManager_vtbl (its vptr is at +0xC), frees the instance heap at +0x4 with cMemMan_free() when there is
 * one, and frees the object with operator_delete() when bit 0 of @p flags is set.
 *
 * @param flags  the destructor flags (bit 0: free the memory)
 */
extern "C" void cDataManager__cDataManager(cDataManager* self, int flags)
{
    self->mVtbl = (void**)cDataManager_vtbl;
    void* instanceHeap = self->mpDataInstanceHeap;
    if (instanceHeap != 0) {
        cMemMan_free(instanceHeap);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_AttachData);
#ifdef SKIP_ASM
void* cDataManager_GetEmptyInstance(void* self);
extern "C" int BIG_sizeofheader(void* data);
struct sDmNode5B98;
extern "C" sDmNode5B98* cDataManager_AttachDataFromBigfile(cDataManager* self, char* big, sDmNode5B98* prev);
extern int REALabortflag[];

struct sDmVEntry5920 {
    short delta;
    short index;
    void (*fn)(void*, int, void*, int, void*, void*);
};

struct sDmNode5920 {
    int field_0x0;
    int field_0x4;          // 0x4
    void* data;             // 0x8
    int size;               // 0xC
    int refs;               // 0x10
};

/**
 * Registers a block of data with the data manager and returns its instance record.
 *
 * Takes a free instance (cDataManager_GetEmptyInstance, On Tour GetEmptyInstance) and stores the data (+0x8), its size (+0xC) and a
 * reference count of 1 (+0x10). BIG_sizeofheader() then tells whether the data is a BIG archive (REALabortflag is
 * cleared around the call so a non-BIG block does not abort). A BIG file calls virtual slot 4 of the manager's vtable
 * (+0xC) and attaches its members with cDataManager_AttachDataFromBigfile; any other block calls slot 2. Both callbacks get the name, data,
 * size, the record and the record's +0x4 word. cSSXScriptEngine_SyncLoadScriptBank and
 * cSSXScriptEngine_ProcessAsyncLoadScriptBankComplete attach the script banks with it.
 *
 * @param pName  the data's name, passed to the callbacks
 * @param data  the data block
 * @param size  its size in bytes
 * @return the instance record (the data handle)
 */
extern "C" sDmNode5920* cDataManager_AttachData(cDataManager* self, int pName, void* data, int size)
{
    sDmNode5920* n = (sDmNode5920*)cDataManager_GetEmptyInstance(self);
    int saved = REALabortflag[0];
    n->data = data;
    n->refs = 1;
    n->size = size;
    REALabortflag[0] = 0;
    int big = BIG_sizeofheader(data);
    REALabortflag[0] = saved;
    if (big != 0) {
        sDmVEntry5920* vt = (sDmVEntry5920*)self->mVtbl;
        vt[4].fn((char*)self + vt[4].delta, pName, data, size, n, &n->field_0x4);
        cDataManager_AttachDataFromBigfile(self, (char*)data, (sDmNode5B98*)n);
    } else {
        sDmVEntry5920* vt = (sDmVEntry5920*)self->mVtbl;
        vt[2].fn((char*)self + vt[2].delta, pName, data, size, n, &n->field_0x4);
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_DetachData);
#ifdef SKIP_ASM
void cDataManager_AddToEmptyInstanceList(void* self, void* node);

struct sDmNode5A20 {
    int a;              // 0x0
    int b;              // 0x4
    int key;            // 0x8
    int c;              // 0xC
    int pad_10;         // 0x10
    sDmNode5A20* next;  // 0x14
};

class cDataManager5A20 {
public:
    int count;          // 0x0
    void* nodes;        // 0x4
    void* freeList;     // 0x8
    // vptr at 0xC
    virtual void v01();
    virtual void v02();
    virtual void OnDetachData(int key, int c, int a, int b);
    virtual void v04();
    virtual void OnDetachDataBigFile(int key, int c, int a, int b);
};

// PORT: the unit declares cDataManager_DetachData as returning void; it really returns the entry's key.
int cDataManager_DetachData_impl(cDataManager5A20* self, sDmNode5A20* entry) __asm__("cDataManager_DetachData");

/**
 * Detaches an attached data handle and every instance linked to it, recycling their records.
 *
 * Each linked instance goes through the detach callback (virtual slot 3); the head then gets slot 5 when it
 * had
 * linked instances, otherwise slot 3.
 *
 * @param entry  the data handle returned by the attach
 * @return the detached data pointer, the handle's first word (On Tour returns void* pData)
 */
int cDataManager_DetachData_impl(cDataManager5A20* self, sDmNode5A20* entry)
{
    sDmNode5A20* first = entry->next;
    if (first != 0) {
        sDmNode5A20* n = first;
        do {
            sDmNode5A20* next = n->next;
            self->OnDetachData(n->key, n->c, n->a, n->b);
            cDataManager_AddToEmptyInstanceList(self, n);
            n = next;
        } while (n != 0);
        self->OnDetachDataBigFile(entry->key, entry->c, entry->a, entry->b);
    } else {
        self->OnDetachData(entry->key, entry->c, entry->a, entry->b);
    }
    int key = entry->key;
    cDataManager_AddToEmptyInstanceList(self, entry);
    return key;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", cDataManager_DetachAll);
#ifdef SKIP_ASM
extern "C" void cDataManager_DetachData(void* self, void* entry);



/**
 * Detaches every data entry of the data manager that is in use, through cDataManager_DetachData().
 */
extern "C" void cDataManager_DetachAll(cDataManager* self)
{
    int i;
    for (i = 0; i < self->mMaxDataInstances; i++) {
        if ((self->mpDataInstanceHeap)[i].isRoot != 0) {
            cDataManager_DetachData(self, &(self->mpDataInstanceHeap)[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_GetData);
#ifdef SKIP_ASM
/**
 * Returns the data of an attached-data handle: its pData word (+0x8 of the instance record).
 *
 * @param dataHandle  the handle cDataManager_AttachData returned
 * @return the attached data
 */
extern "C" int cDataManager_GetData(cDataManager* self, char* dataHandle)
{
    return *(int*)(dataHandle + 8);
}
#endif
//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_GetDataUserData);
#ifdef SKIP_ASM
/**
 * Reads the two user-data words of an attached-data handle (+0x0 and +0x4 of the instance record).
 *
 * @param dataHandle  the handle cDataManager_AttachData returned
 * @param out0  receives the first user-data word
 * @param out1  receives the second user-data word
 */
extern "C" void cDataManager_GetDataUserData(cDataManager* self, char* dataHandle, int* out0, int* out1)
{
    *out0 = *(int*)dataHandle;
    *out1 = *(int*)(dataHandle + 4);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_AttachDataFromBigfile);
#ifdef SKIP_ASM
void* cDataManager_GetEmptyInstance(void* self);
extern "C" int BIG_sizeofheader(void* data);
extern "C" char* BIG_locateentryz(void* big, int a1, int index, int* offset, int* size);
extern int REALabortflag[];

struct sDmVEntry5B98 {
    short delta;
    short index;
    void (*fn)(void*, char*, char*, int, void*, void*);
};

struct sDmNode5B98 {
    int field_0x0;
    int field_0x4;          // 0x4
    char* data;             // 0x8
    int size;               // 0xC
    int refs;               // 0x10
    sDmNode5B98* next;      // 0x14
};

/**
 * Attaches every member of a BIG archive to the data manager as a chain of instance records, recursing into nested
 * archives.
 *
 * Walks the entries with BIG_locateentryz() (entry 0, then 1, 2, ... until it returns 0). A member that is itself a BIG
 * file (BIG_sizeofheader() with REALabortflag cleared) is attached recursively; any other member gets a free instance
 * (cDataManager_GetEmptyInstance) holding its address (+0x8) and size (+0xC), is linked after the previous record through +0x14 and is
 * announced through virtual slot 2 of the manager's vtable (+0xC) with its name, data, size, record and record+0x4.
 *
 * Likely bug (also in On Tour): only the first BIG_locateentryz() call passes a size pointer; the later calls pass 0,
 * so every member after the first is recorded with the first member's size.
 *
 * @param big  the BIG archive in memory
 * @param prev  the record to chain the new records after
 * @return the last record of the chain
 */
extern "C" sDmNode5B98* cDataManager_AttachDataFromBigfile(cDataManager* self, char* big, sDmNode5B98* prev)
{
    int ent[2];
    ent[0] = 0;
    ent[1] = 0;
    int i = 1;
    char* name = BIG_locateentryz(big, 0, 0, &ent[0], &ent[1]);
    while (name != 0) {
        char* sub = big + ent[0];
        int saved = REALabortflag[0];
        REALabortflag[0] = 0;
        int headerSize = BIG_sizeofheader(sub);
        REALabortflag[0] = saved;
        if (headerSize != 0) {
            prev = cDataManager_AttachDataFromBigfile(self, sub, prev);
        } else {
            sDmNode5B98* n = (sDmNode5B98*)cDataManager_GetEmptyInstance(self);
            n->data = sub;
            n->size = ent[1];
            prev->next = n;
            prev = n;
            sDmVEntry5B98* vt = (sDmVEntry5B98*)self->mVtbl;
            vt[2].fn((char*)self + vt[2].delta, name, sub, ent[1], prev, &prev->field_0x4);
        }
        name = BIG_locateentryz(big, 0, i++, &ent[0], 0);
    }
    return prev;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_AddToEmptyInstanceList__FPvT0);
#ifdef SKIP_ASM
/**
 * Clears a data instance record and pushes it onto the data manager's free list.
 *
 * Zeroes the record's first five words, links it in front of the list head at +0x8 through its next field
 * (+0x14), and makes it the new head.
 *
 * @param node  the instance record (On Tour tDataInstanceInfo*)
 */
void cDataManager_AddToEmptyInstanceList(void* vself, void* node)
{
    cDataManager* self = (cDataManager*)vself;
    *(int*)((char*)node + 0x8) = 0;
    *(int*)((char*)node + 0xc) = 0;
    *(int*)((char*)node + 0x10) = 0;
    *(int*)((char*)node + 0x0) = 0;
    *(int*)((char*)node + 0x4) = 0;
    *(void**)((char*)node + 0x14) = (void*)self->mpEmptyDataInstanceList;
    self->mpEmptyDataInstanceList = (tDataInstanceInfo*)node;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_GetEmptyInstance__FPv);
#ifdef SKIP_ASM
/**
 * Takes the first instance record off the manager's empty list (+0x8) and returns it with its next link (+0x14)
 * cleared.
 *
 * Quirk: the list is not checked for being empty.
 *
 * @return the instance record
 */
void* cDataManager_GetEmptyInstance(void* vself)
{
    cDataManager* self = (cDataManager*)vself;
    tDataInstanceInfo* head = self->mpEmptyDataInstanceList;
    self->mpEmptyDataInstanceList = (tDataInstanceInfo*)(head->pNext);
    head->pNext = 0;
    return ((void*)head);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_tNISList);
#ifdef SKIP_ASM
extern "C" void cCircularBuffer_cCircularBuffer(void* self);
void cCircularBuffer_Reset(void* self);

/**
 * Constructs an empty NIS list.
 *
 * Initialises the entry buffer at +0xC (cCircularBuffer_cCircularBuffer, cCircularBuffer_Reset), sets the next pending index (+0xBC) to -1 and
 * the cut-scene flag (+0x4) to 1, and zeroes the engine pointer (+0x0), view index (+0x8), state (+0xA4) and the flag
 * and counter words at +0xA8-+0xC8.
 *
 * @return @p self
 */
extern "C" void* tNISList_tNISList(tNISList* self)
{
    void* list = (char*)&self->mList;
    cCircularBuffer_cCircularBuffer(list);
    cCircularBuffer_Reset(list);
    self->mNextPendingScriptIndex = -1;
    self->mIsCutSceneHandler = 1;
    self->mState = 0;
    self->mPausedOnNextScriptLoading = 0;
    self->mDoSkip = 0;
    self->mInScriptTransition = 0;
    self->mIsStartTransition = 0;
    self->mTransitionWaitFrames = 0;
    self->mNumScriptsToSkipInProgress = 0;
    self->mNumPause = 0;
    self->mpNISEngine = 0;
    self->mPlayOnReady = 0;
    self->mViewIndex = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList__tNISList);
#ifdef SKIP_ASM
extern "C" void tNISList_Reset(tNISList* self, int a1);
void operator_delete(int* ptr);

/**
 * Destroys a NIS list: resets it (tNISList_Reset with resetTransition 1), clears its engine pointer (+0x0), and frees it
 * with operator_delete when bit 0 of @p flags is set.
 *
 * @param flags  destructor flags; bit 0 frees the memory
 */
extern "C" void tNISList__tNISList(int* self, int flags)
{
    tNISList_Reset((tNISList*)self, 1);
    *self = 0;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_AddByVideoCategory);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_WaitOnPendingNextScript(tNISList* self);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
int cCircularBuffer_IsFull(void* self);
extern "C" int cCircularBuffer_AddToTail(void* list, void* cmd);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");
struct sDmCmd5DD8 {
    int a1;         // 0x0
    int id;         // 0x4
    int a2;         // 0x8
    int field_0xC;  // 0xC
    int a4;         // 0x10
    int a5;         // 0x14
    int a6;         // 0x18
};

/**
 * Appends a video entry of category @p videoCategory to the NIS list, then sets up the next script if the list is running.
 *
 * Returns 0 when the five-entry buffer is full (cCircularBuffer_IsFull). The entry (category, script index -1, control bits
 * @p controlBits, no handle, AI rider indexes -1) is pushed with cCircularBuffer_AddToTail. When the list is active (state +0xA4 nonzero)
 * and nothing is pending yet (+0xBC < 0), the new entry is set up as the pending one (tNISList_SetupNextScript), and if
 * the list is playing (3) while the head's script player (cBXScriptEngine_GetRequestPlayerByReqHandle) is ending
 * (mPlayState 4), tNISList_WaitOnPendingNextScript checks whether to wait for it.
 *
 * Quirk: unlike tNISList_AddByScriptCategory and tNISList_AddByScriptIndex it does not test whether the head entry is a video before asking for
 * its script player.
 *
 * @param videoCategory  the video category
 * @param controlBits  the entry's control bits
 * @return 1 when queued, 0 when the list is full
 */
extern "C" int tNISList_AddByVideoCategory(tNISList* self, int videoCategory, int controlBits)
{
    if (cCircularBuffer_IsFull(&self->mList) != 0) {
        return 0;
    }
    sDmCmd5DD8 cmd;
    cmd.a1 = videoCategory;
    cmd.id = -1;
    cmd.a2 = controlBits;
    cmd.field_0xC = -1;
    cmd.a4 = -1;
    cmd.a5 = -1;
    // 64-bit "none" value compared against the pending index below; needed for the target's branch layout
    long none = 0;
    cmd.a6 = -1;
    void* list = (char*)&self->mList;
    cCircularBuffer_AddToTail(list, &cmd);
    if (self->mState == 0) {
        return 1;
    }
    if (self->mNextPendingScriptIndex >= none) {
        return 1;
    }
    self->mNextPendingScriptIndex = tNISList_SetupNextScript(self, cCircularBuffer_GetNumEntries_count(list) - 1, 0);
    if (self->mState == 3) {
        int ok = *(int*)((char*)cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)self, *(int*)((char*)cCircularBuffer_operatorIndex(list, 0) + 0xC)) + 0x20) == 4;
        if (ok)
            tNISList_WaitOnPendingNextScript(self);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_AddByScriptCategory);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_WaitOnPendingNextScript(tNISList* self);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cSSXScriptEngine_SelectScriptFromCategory(void* eng, int a1, int a2, int a3, int a4);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" int cVidEngine_IsVideoOnLastFrame(void* self);
int cCircularBuffer_IsFull(void* self);
extern "C" int cCircularBuffer_AddToTail(void* list, void* cmd);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

struct sDmCmd5ED0 {
    int a1;         // 0x0
    int id;         // 0x4
    int a2;         // 0x8
    int field_0xC;  // 0xC
    int a4;         // 0x10
    int a5;         // 0x14
    int a6;         // 0x18
};

/**
 * Appends a script entry of category @p scriptCategory to the NIS list, then sets up the next script if the list is running.
 *
 * Returns 0 when the buffer is full (cCircularBuffer_IsFull). With @p lookup set, the script is chosen now
 * (cSSXScriptEngine_SelectScriptFromCategory with the three AI rider indexes) and a failed choice returns 0; otherwise
 * the entry keeps script index -1 for tNISList_SetupNextScript to resolve. After the push (cCircularBuffer_AddToTail), an active
 * list with nothing pending sets the new entry up as pending; while playing, if the head (a video: cVidEngine_IsVideoOnLastFrame on the
 * video engine; a script: its player in mPlayState 4) is ending, tNISList_WaitOnPendingNextScript checks whether to wait for it.
 *
 * @param scriptCategory  the script category
 * @param controlBits  the entry's control bits
 * @param lookup  nonzero to choose the script now
 * @param aiCharIndexMain  the main AI rider index
 * @param aiCharIndexSupport0  the first supporting AI rider index
 * @param aiCharIndexSupport1  the second supporting AI rider index
 * @return 1 when queued, 0 when full or no script was found
 */
extern "C" int tNISList_AddByScriptCategory(tNISList* self, int scriptCategory, int controlBits, int lookup, int aiCharIndexMain, int aiCharIndexSupport0, int aiCharIndexSupport1)
{
    if (cCircularBuffer_IsFull(&self->mList) != 0) {
        return 0;
    }
    sDmCmd5ED0 cmd;
    cmd.a1 = scriptCategory;
    cmd.id = -1;
    cmd.a2 = controlBits;
    cmd.field_0xC = -1;
    cmd.a4 = aiCharIndexMain;
    cmd.a5 = aiCharIndexSupport0;
    cmd.a6 = aiCharIndexSupport1;
    if (lookup != 0) {
        int id = cSSXScriptEngine_SelectScriptFromCategory(*(void**)self, scriptCategory, aiCharIndexMain, aiCharIndexSupport0, aiCharIndexSupport1);
        if (id < 0) {
            return 0;
        }
        cmd.id = id;
    }
    void* list = (char*)&self->mList;
    cCircularBuffer_AddToTail(list, &cmd);
    if (self->mState == 0) {
        return 1;
    }
    if (self->mNextPendingScriptIndex >= 0) {
        return 1;
    }
    self->mNextPendingScriptIndex = tNISList_SetupNextScript(self, cCircularBuffer_GetNumEntries_count(list) - 1, 0);
    if (self->mState == 3) {
        int ok;
        if (tNISList_IsVideo(self, cCircularBuffer_operatorIndex(list, 0)) == 0) {
            ok = *(int*)((char*)cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)self, *(int*)((char*)cCircularBuffer_operatorIndex(list, 0) + 0xC)) + 0x20) == 4;
        } else {
            ok = cVidEngine_IsVideoOnLastFrame((char*)*(void**)self + 0x500);
        }
        if (ok) {
            tNISList_WaitOnPendingNextScript(self);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_AddByScriptIndex);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_WaitOnPendingNextScript(tNISList* self);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" int cVidEngine_IsVideoOnLastFrame(void* self);
int cCircularBuffer_IsFull(void* self);
extern "C" int cCircularBuffer_AddToTail(void* list, void* cmd);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

struct sDmCmd6048 {
    int type;       // 0x0
    int a1;         // 0x4
    int a2;         // 0x8
    int id;         // 0xC
    int a3;         // 0x10
    int a4;         // 0x14
    int a5;         // 0x18
};

/**
 * Appends the script with master-table index @p scriptIndex to the NIS list, then sets up the next script if the list is
 * running.
 *
 * Returns 0 when the buffer is full (cCircularBuffer_IsFull). The entry has category 0x1C (no category), script index @p scriptIndex, no
 * handle and the given control bits and AI rider indexes. After the push the follow-up matches tNISList_AddByScriptCategory: an active
 * list with nothing pending sets the new entry up, and an ending head while playing triggers tNISList_WaitOnPendingNextScript.
 *
 * @param scriptIndex  the script's master-table index
 * @param controlBits  the entry's control bits
 * @param aiCharIndexMain  the main AI rider index
 * @param aiCharIndexSupport0  the first supporting AI rider index
 * @param aiCharIndexSupport1  the second supporting AI rider index
 * @return 1 when queued, 0 when the list is full
 */
extern "C" int tNISList_AddByScriptIndex(tNISList* self, int scriptIndex, int controlBits, int aiCharIndexMain, int aiCharIndexSupport0, int aiCharIndexSupport1)
{
    if (cCircularBuffer_IsFull(&self->mList) == 0) {
        void* list = (char*)&self->mList;
        sDmCmd6048 cmd;
        cmd.type = 0x1C;
        cmd.id = -1;
        cmd.a2 = controlBits;
        cmd.a1 = scriptIndex;
        cmd.a3 = aiCharIndexMain;
        cmd.a4 = aiCharIndexSupport0;
        cmd.a5 = aiCharIndexSupport1;
        cCircularBuffer_AddToTail(list, &cmd);
        if (self->mState != 0 && self->mNextPendingScriptIndex < 0) {
        self->mNextPendingScriptIndex = tNISList_SetupNextScript(self, cCircularBuffer_GetNumEntries_count(list) - 1, 0);
        if (self->mState == 3) {
            int ok;
            if (tNISList_IsVideo(self, cCircularBuffer_operatorIndex(list, 0)) == 0) {
                ok = *(int*)((char*)cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)self, *(int*)((char*)cCircularBuffer_operatorIndex(list, 0) + 0xC)) + 0x20) == 4;
            } else {
                ok = cVidEngine_IsVideoOnLastFrame((char*)*(void**)self + 0x500);
            }
            if (ok) {
                tNISList_WaitOnPendingNextScript(self);
            }
        }
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_TransferFromNISList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cCircularBuffer_AddToTail(void* list, void* cmd);
extern "C" int cCircularBuffer_RemoveFromHead(void* self, void* out);
extern "C" void tNISList_Reset(tNISList* self, int a1);
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

/**
 * Moves all entries of NIS list @p other to the end of this list and resets @p other.
 *
 * Refuses (returns 0) while @p other is playing (state 3). Entries are popped from @p other (cCircularBuffer_RemoveFromHead) and pushed
 * here (cCircularBuffer_AddToTail); @p other is then reset (tNISList_Reset with resetTransition 1). When this list is active with
 * nothing pending, the first moved entry is set up as pending (tNISList_SetupNextScript from the old length).
 *
 * @param other  the source NIS list
 * @return 1, or 0 when @p other is playing
 */
extern "C" int tNISList_TransferFromNISList(tNISList* self, tNISList* other)
{
    int cmd[8];
    int n = cCircularBuffer_GetNumEntries_count((char*)&self->mList);
    if (other->mState == 3) {
        return 0;
    }
    if (cCircularBuffer_GetNumEntries_count(&other->mList) != 0) {
        while (cCircularBuffer_RemoveFromHead(&other->mList, cmd) != 0) {
            cCircularBuffer_AddToTail((char*)&self->mList, cmd);
        }
        tNISList_Reset((tNISList*)other, 1);
        if (self->mState != 0 && self->mNextPendingScriptIndex < 0) {
            self->mNextPendingScriptIndex = tNISList_SetupNextScript(self, n, 0);
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_ReadyList);
#ifdef SKIP_ASM
extern "C" void tNISList_Reset(tNISList* self, int a1);
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_RemoveFromHead(tNISList* self, int index);
extern "C" void tNISList_CheckOnLoading(tNISList* self);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cBXScriptEngine_GetRequestState(void* self, int a1);
extern "C" void* cCircularBuffer_GetNumEntries(void*);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);

/**
 * Readies an idle NIS list for playing and returns its state.
 *
 * A loading list (state 1) is polled first (tNISList_CheckOnLoading); a list that is not idle returns its state. Otherwise stores
 * @p playOnReady (+0xB4), returns 0 for an empty list, and finds the first entry that can play
 * (tNISList_SetupNextScript from 0); with none the list is reset (tNISList_Reset). Entries before it are dropped
 * (tNISList_RemoveFromHead), and the state becomes 1 (loading) while the head's script request waits for data or its video has no
 * handle, else 2 (ready). The following entry is set up as the pending one (+0xBC).
 *
 * Likely bug: the pending search starts at the old index + 1 although that many head entries were just removed, so it
 * skips entries whenever the first playable one was not at index 0.
 *
 * @param playOnReady  0 = no, 1 = play with transition in, 2 = play without (On Tour ePlayOnReady)
 * @return the list state (+0xA4)
 */
extern "C" int tNISList_ReadyList(tNISList* self, int playOnReady)
{
    if (self->mState == 1) {
        tNISList_CheckOnLoading(self);
    }
    if (self->mState != 0) {
        return self->mState;
    }
    self->mPlayOnReady = playOnReady;
    void* list = (char*)&self->mList;
    if (cCircularBuffer_GetNumEntries(list) == 0) {
        return 0;
    }
    int idx = tNISList_SetupNextScript(self, 0, 0);
    if (idx < 0) {
        tNISList_Reset(self, 1);
        return self->mState;
    }
    tNISList_RemoveFromHead(self, idx);
    char* entry = (char*)cCircularBuffer_operatorIndex(list, 0);
    if (tNISList_IsVideo(self, entry) == 0) {
        if (cBXScriptEngine_GetRequestState(*(void**)self, *(int*)(entry + 0xC)) == 1) {
            self->mState = 1;
        } else {
            self->mState = 2;
        }
    } else {
        if (*(int*)(entry + 0xC) < 0) {
            self->mState = 1;
        } else {
            self->mState = 2;
        }
    }
    self->mNextPendingScriptIndex = tNISList_SetupNextScript(self, idx + 1, 0);
    return self->mState;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_Play);
#ifdef SKIP_ASM
extern char str_hud_e[];
int GetHashValue32(char* s);
void* cGame_getTransitionMan(void* self, int a1);
char* GetSSXScriptHeader(void* obj);
extern "C" void tNISList_CheckOnLoading(tNISList* self);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
// The unit declares cSSXScriptEngine_PlayRequestScript as void; it returns cBXScriptEngine_PlayScript's result.
int cSSXScriptEngine_PlayRequestScript_i(void* self, int val, int key, int b, int c) __asm__("cSSXScriptEngine_PlayRequestScript");
extern "C" int cSSXScriptEngine_MakeTransitionObject(void* self, int a1, int a2);
extern "C" void cBXScriptEngine_PauseScript(void* self, int a1);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" int cVidEngine_PlayVideo(void* self);
extern "C" void cVidEngine_UpdateVideo(void* self);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" void cTransitionMan_StartTransition(void* snd, int a1, int h, int h2, int a4, int a5, float t0, float t1, float t2);
// PORT: real parameter order of cTransitionMan_StartTransition has the floats before the two trailing ints (cf. boardwakefx).
void cTransitionMan_StartTransition_f(void* snd, int a1, int h, int h2, float t0, float t1, float t2, int a4, int a5) __asm__("cTransitionMan_StartTransition");
extern "C" void cTransitionMan_StartTransitionScreenGrab(void* snd, int a1, int h, int a3, float t);
extern "C" void cTransitionMan_EndTransition(void* p);
extern "C" void cCutSceneTransitionMan_StartCutScene(void* gm, float t);
extern "C" void cCutSceneTransitionMan_ForceToCutSceneMode(void* gm);
extern "C" void* cUIStateStack_findState(void* table, int hash);

struct sDmVEntry_6388 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

struct sDmVEntry2_6388 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

struct sDm_6388 {
    void* eng;              // 0x0
    int active;             // 0x4
    int val;                // 0x8
    char list[0xA4 - 0xC];  // 0xC
    int state;              // 0xA4
    int fA8;
    int fAC;                // 0xAC
    int fB0;                // 0xB0
    int fB4;
    int fB8;                // 0xB8
};

struct sDmFade_6388 {
    char pad_0x0[0x8];
    signed char cat;        // 0x8
    signed char key;        // 0x9
    short tIn;              // 0xA
    short tHold;            // 0xC
    short tOut;             // 0xE
};

struct sDmItem_6388 {
    int f0;
    int f4;
    int flags;              // 0x8
    int id;                 // 0xC
};

// The unit declares tNISList_Play as void; it returns the state at 0xA4.
int tNISList_Play_impl(char* self, int fade, int notify) __asm__("tNISList_Play");

/**
 * Starts the NIS list's head script (or video) once it is ready, with its transition in, and returns the list state.
 *
 * Polls a loading list first (state 1: tNISList_CheckOnLoading, On Tour CheckOnLoading) and does nothing unless the state (+0xA4)
 * is 2 (ready). It sets the state to 3 (playing) and calls virtual slot 24 of the "hud" UI state
 * (cUIStateStack_findState) with (0, 0) when that state is up.
 *
 * A script entry (tNISList_IsVideo, On Tour IsVideo, returns 0) is started with cSSXScriptEngine_PlayRequestScript (the
 * last flag is bit 3 of the entry's flags, +0x8). On the cut-scene list (+0x4) the running transition is ended
 * (cTransitionMan_EndTransition), and the script header's in/hold/out frame counts (GetSSXScriptHeader of the request's player; 1/60 s
 * each) build the transition in: type 7 is a screen grab (cTransitionMan_StartTransitionScreenGrab), any other type a full transition
 * (cTransitionMan_StartTransition with two cSSXScriptEngine_MakeTransitionObject objects). When in+hold frames remain (+0xB8) the
 * request is paused (cBXScriptEngine_PauseScript, On Tour PauseScript), +0xAC and +0xB0 (On Tour mIsStartTransition,
 * mInScriptTransition) are set and the forced update is skipped; then the cut scene starts (cCutSceneTransitionMan_StartCutScene, On Tour
 * cCutSceneTransitionMan::StartCutScene, over the transition-in time or 0.5 s). With the forced update the engine's
 * virtual slot 6 (On Tour ForcePlayUpdate) runs the request once.
 *
 * A video entry forces cut-scene mode on the cut-scene list (cCutSceneTransitionMan_ForceToCutSceneMode), starts the video engine (cVidEngine_PlayVideo on
 * engine+0x500) and, with the forced update, updates it once (cVidEngine_UpdateVideo).
 *
 * @param fade  start with the transition in (On Tour startWithTransitionIn); 0 drops its in and hold frames
 * @param notify  force an immediate update of the started script or video (On Tour forceUpdate)
 * @return the list state after the call (+0xA4)
 */
int tNISList_Play_impl(char* self, int fade, int notify)
{
    sDm_6388* dm = (sDm_6388*)self;
    if (*(int*)(self + 0xA4) == 1) {
        tNISList_CheckOnLoading((tNISList*)self);
    }
    if (*(int*)(self + 0xA4) != 2) {
        return *(int*)(self + 0xA4);
    }
    sDmItem_6388* item = (sDmItem_6388*)cCircularBuffer_operatorIndex(self + 0xC, 0);
    dm->state = 3;
    char* table = (char*)SSXApp->Game->UI + 0x18;
    char* hudState = (char*)cUIStateStack_findState(table, GetHashValue32(str_hud_e));
    if (hudState != 0) {
        sDmVEntry_6388* vtEntry = &(*(sDmVEntry_6388**)(hudState + 0x8))[24];
        vtEntry->fn(hudState + vtEntry->delta, 0, 0);
    }
    if (tNISList_IsVideo((tNISList*)self, item) == 0) {
        cSSXScriptEngine_PlayRequestScript_i(*(void**)self, *(int*)(self + 0x8), item->id, 0, (item->flags >> 3) & 1);
        if (*(int*)(self + 0x4) != 0) {
            void* snd = cGame_getTransitionMan(SSXApp->Game, 0);
            if (*(int*)((char*)snd + 0x44) != 0) {
                cTransitionMan_EndTransition(snd);
            }
            int tIn = 0;
            int tHold = 0;
            float cutSceneTime = 0.0f;
            sDmFade_6388* transitionInfo = (sDmFade_6388*)GetSSXScriptHeader(cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)self, item->id));
            if (fade) {
                tIn = transitionInfo->tIn;
                tHold = transitionInfo->tHold;
            }
            *(int*)(self + 0xB8) = 0;
            if (tIn + tHold + transitionInfo->tOut > 0) {
                if (transitionInfo->cat != 7) {
                    int transition = cSSXScriptEngine_MakeTransitionObject(*(void**)self, transitionInfo->cat, transitionInfo->key);
                    if (transition != 0) {
                        int transitionOut = cSSXScriptEngine_MakeTransitionObject(*(void**)self, transitionInfo->cat, transitionInfo->key);
                        float tt = (float)tIn * 0.01666666753590107f;
                        cTransitionMan_StartTransition_f(snd, 2, transition, transitionOut, tt, (float)tHold * 0.01666666753590107f, (float)transitionInfo->tOut * 0.01666666753590107f, 0, 0);
                        cutSceneTime = tt;
                        *(int*)(self + 0xB8) = tIn + tHold;
                    }
                } else if (tIn) {
                    int transition = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, 7, transitionInfo->key);
                    float tt = (float)tIn * 0.01666666753590107f;
                    cTransitionMan_StartTransitionScreenGrab(snd, 2, transition, 0, tt);
                    cutSceneTime = tt;
                }
                if (*(int*)(self + 0xB8) > 0) {
                    notify = 0;
                    cBXScriptEngine_PauseScript(*(void**)self, item->id);
                    *(int*)(self + 0xAC) = 1;
                    *(int*)(self + 0xB0) = 1;
                }
            }
            if (cutSceneTime <= 0.0f) {
                cutSceneTime = 0.5f;
            }
            cCutSceneTransitionMan_StartCutScene(SSXApp->Game->CutSceneTransitionMan, cutSceneTime);
        }
        if (notify) {
            char* eng = *(char**)self;
            sDmVEntry2_6388* vtEntry = &(*(sDmVEntry2_6388**)(eng + 0x2A8))[6];
            vtEntry->fn(eng + vtEntry->delta, item->id);
        }
    } else {
        if (*(int*)(self + 0x4) != 0) {
            cCutSceneTransitionMan_ForceToCutSceneMode(SSXApp->Game->CutSceneTransitionMan);
        }
        cVidEngine_PlayVideo(*(char**)self + 0x500);
        if (notify) {
            cVidEngine_UpdateVideo(*(char**)self + 0x500);
        }
    }
    return *(int*)(self + 0xA4);
}
#endif

extern "C" void* tNISList_DoPause(void*, int, int);

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_Pause__FPvi);
#ifdef SKIP_ASM
/**
 * Pauses or resumes a NIS list: tNISList_DoPause(self, @p pauseList, 0).
 *
 * @param pauseList  nonzero to pause, 0 to resume
 * @return tNISList_DoPause's return register (it returns nothing)
 */
void* tNISList_Pause(void* self, int pauseList)
{
    return tNISList_DoPause(self, pauseList, 0);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_SkipForward);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int tNISList_ReadyList(tNISList* self, int a1);
extern "C" void tNISList_Reset(tNISList* self, int a1);
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_RemoveFromHead(tNISList* self, int index);
extern "C" void tNISList_PlayNextScriptOnAvailable(tNISList* self);
// The unit declares tNISList_DoTransitionToNext as void; it returns whether it handled the update.
extern "C" int tNISList_DoTransitionToNext_r(void* self) __asm__("tNISList_DoTransitionToNext");
extern "C" void* tNISList_DoPause(void*, int, int);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cBXScriptEngine_ReleaseRequest(void* self, int id);
extern "C" void cVidEngine_ResetVideoEngine(void* p);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

struct sSeqItem_66D0 {
    char pad_0x0[0xC];
    int handle;         // 0xC
};


/**
 * Skips @p delta scripts of a NIS list and returns its state.
 *
 * The target is @p delta past the skip position (+0xC0), at least the pending index (+0xBC); an unchanged target
 * returns at once. A list that is not playing drops the skipped entries (tNISList_RemoveFromHead), goes idle and, if it was
 * active, is readied again (tNISList_ReadyList with its play-on-ready mode, +0xB4); skipping past the end resets it
 * (tNISList_Reset). A playing list stores the target; a pending entry before the target is released (script:
 * cBXScriptEngine_ReleaseRequest; video: cVidEngine_ResetVideoEngine) and pausing on loading is undone (tNISList_DoPause(self, 0, 1))
 * before the entry at the target is set up as pending. Unless it is paused on loading (+0xA8), the next script starts
 * (tNISList_PlayNextScriptOnAvailable) when @p force is 0 or tNISList_DoTransitionToNext declines; with nothing pending the list is reset.
 *
 * Likely bug: a pending video is reset only when its handle is nonzero, but a readied video has handle 0
 * (tNISList_SetupNextScript), so the readied one is left running; tNISList_DoSkip and tNISList_RemoveFromHead test for >= 0.
 *
 * @param delta  the number of scripts to skip
 * @param force  nonzero to try a transition to the next script (On Tour doTransition)
 * @return the list state (+0xA4)
 */
extern "C" int tNISList_SkipForward(tNISList* self, int delta, int force)
{
    int pos = delta + self->mNumScriptsToSkipInProgress;
    if (pos == self->mNumScriptsToSkipInProgress) {
        return self->mState;
    }
    if (pos < self->mNextPendingScriptIndex) {
        pos = self->mNextPendingScriptIndex;
    }
    int n = cCircularBuffer_GetNumEntries_count(((char*)&self->mList));
    if (self->mState != 3) {
        self->mNumScriptsToSkipInProgress = 0;
        if (pos < n) {
            tNISList_RemoveFromHead(self, pos);
            int st = self->mState;
            self->mNextPendingScriptIndex = -1;
            self->mState = 0;
            if (st != 0) {
                tNISList_ReadyList(self, self->mPlayOnReady);
            }
            return self->mState;
        }
        goto reset;
    }
    self->mNumScriptsToSkipInProgress = pos;
    if (self->mNextPendingScriptIndex > 0 && self->mNextPendingScriptIndex < pos) {
        sSeqItem_66D0* it = (sSeqItem_66D0*)cCircularBuffer_operatorIndex(((char*)&self->mList), self->mNextPendingScriptIndex);
        if (tNISList_IsVideo(self, it) == 0) {
            cBXScriptEngine_ReleaseRequest((char*)self->mpNISEngine, it->handle);
        } else if (it->handle != 0) {
            cVidEngine_ResetVideoEngine(&self->mpNISEngine->mpPlayer);
        }
        it->handle = -1;
        self->mNextPendingScriptIndex = -1;
        if (self->mPausedOnNextScriptLoading) {
            tNISList_DoPause((self), 0, 1);
        }
        self->mNextPendingScriptIndex = tNISList_SetupNextScript(self, pos, 0);
    }
    if (self->mPausedOnNextScriptLoading == 0) {
        if (force == 0 || tNISList_DoTransitionToNext_r((self)) == 0) {
            if (self->mNextPendingScriptIndex < 0) {
                goto reset;
            }
            tNISList_PlayNextScriptOnAvailable(self);
        }
    }
    return self->mState;
reset:
    tNISList_Reset(self, 1);
    return self->mState;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_Reset);
#ifdef SKIP_ASM
extern char str_hud_e[];
int GetHashValue32(char* s);
void* cGame_getTransitionMan(void* self, int a1);
extern "C" void cTransitionMan_EndTransition(void* p);
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(void* p);
extern "C" void* cUIStateStack_findState(void* table, int hash);
extern "C" void tNISList_RemoveFromHead(tNISList* self, int index);
extern "C" void* tNISList_DoPause(void*, int, int);

struct sDmVEntry6868 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

/**
 * Empties a NIS list and returns it to idle, undoing its pauses and, on the cut-scene list, its cut-scene mode.
 *
 * Undoes a pause on loading (tNISList_DoPause(self, 0, 1)) and any counted pauses (+0xC4 forced to 1, then
 * tNISList_DoPause(self, 0, 0)). When the cut-scene list (+0x4) is playing: with @p resetTransition, puts the game's
 * cut-scene transition manager back in regular mode (cCutSceneTransitionMan_ForceToRegularMode) and ends a running
 * script transition (+0xB0) with cTransitionMan_EndTransition; then calls virtual slot 24 of the "hud" UI state
 * (cUIStateStack_findState) with (1, 0). Last it drops all five entries (tNISList_RemoveFromHead) and clears the pending index
 * (-1), state and the flag and counter words at +0xAC-+0xC8.
 *
 * @param nisList  the NIS list
 * @param resetTransition  nonzero to also leave cut-scene mode and end the running transition
 */
extern "C" void tNISList_Reset(tNISList* nisList, int resetTransition)
{
    if (nisList->mPausedOnNextScriptLoading != 0) {
        tNISList_DoPause(((char*)nisList), 0, 1);
    }
    if (nisList->mNumPause != 0) {
        nisList->mNumPause = 1;
        tNISList_DoPause(((char*)nisList), 0, 0);
    }
    if (nisList->mIsCutSceneHandler != 0 && nisList->mState == 3) {
        void* gm = SSXApp->Game->CutSceneTransitionMan;
        if (resetTransition != 0) {
            cCutSceneTransitionMan_ForceToRegularMode(gm);
            if (nisList->mInScriptTransition != 0) {
                cTransitionMan_EndTransition(cGame_getTransitionMan(SSXApp->Game, 0));
            }
        }
        char* table = (char*)SSXApp->Game->UI + 0x18;
        char* hudState = (char*)cUIStateStack_findState(table, GetHashValue32(str_hud_e));
        if (hudState != 0) {
            sDmVEntry6868* vtEntry = &(*(sDmVEntry6868**)(hudState + 0x8))[24];
            vtEntry->fn(hudState + vtEntry->delta, 1, 0);
        }
    }
    tNISList_RemoveFromHead(nisList, 5);
    nisList->mNextPendingScriptIndex = -1;
    nisList->mState = 0;
    nisList->mDoSkip = 0;
    nisList->mInScriptTransition = 0;
    nisList->mTransitionWaitFrames = 0;
    nisList->mIsStartTransition = 0;
    nisList->mNumScriptsToSkipInProgress = 0;
    nisList->mNumPause = 0;
    nisList->mPlayOnReady = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetScriptCategory);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

/**
 * Returns the script category of entry @p i of a NIS list, or 0x1C (no script category) when @p i is past the end or
 * the entry is a video (category 29 and up).
 *
 * @param i  the entry index
 * @return the script category
 */
extern "C" int tNISList_GetScriptCategory(tNISList* self, int i)
{
    void* list = (char*)&self->mList;
    if (i < cCircularBuffer_GetNumEntries_count(list) && *(int*)cCircularBuffer_operatorIndex(list, i) < 0x1C) {
        return *(int*)cCircularBuffer_operatorIndex(list, i);
    }
    return 0x1C;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetVideoCategory);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

/**
 * Returns the video category of entry @p i of a NIS list, or 0x20 (no video category) when @p i is past the end or the
 * entry is a script (category below 29).
 *
 * @param i  the entry index
 * @return the video category
 */
extern "C" int tNISList_GetVideoCategory(tNISList* self, int i)
{
    void* list = (char*)&self->mList;
    if (i < cCircularBuffer_GetNumEntries_count(list) && *(int*)cCircularBuffer_operatorIndex(list, i) >= 29) {
        return *(int*)cCircularBuffer_operatorIndex(list, i);
    }
    return 0x20;
}
#endif
//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetScriptIndex);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int cBXScriptEngine_GetRequestScriptIndex(void* self, int id);
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

/**
 * Returns the master-table script index of entry @p i of a NIS list.
 *
 * Past the end or for a video entry: -1. Otherwise the entry's own index (+0x4), or, when that is unset, the index of
 * its script request (cBXScriptEngine_GetRequestScriptIndex on its handle, +0xC), or -1 without a request.
 *
 * @param i  the entry index
 * @return the script index, or -1
 */
extern "C" int tNISList_GetScriptIndex(tNISList* self, int i)
{
    void* list = (char*)&self->mList;
    if (i >= cCircularBuffer_GetNumEntries_count(list)) {
        return -1;
    }
    void* item = cCircularBuffer_operatorIndex(list, i);
    if (tNISList_IsVideo(self, item) != 0) {
        return -1;
    }
    int scriptIndex = *(int*)((char*)item + 0x4);
    if (scriptIndex < 0) {
        int id = *(int*)((char*)item + 0xC);
        scriptIndex = -1;
        if (id >= 0) {
            scriptIndex = cBXScriptEngine_GetRequestScriptIndex(*(void**)self, id);
        }
    }
    return scriptIndex;
}
#endif
//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetScriptPlayer);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

/**
 * Returns the InSeq script player of entry @p i of a NIS list (cBXScriptEngine_GetRequestPlayerByReqHandle on the
 * entry's request handle, +0xC), or 0 past the end, for a video entry or when no request exists.
 *
 * @param i  the entry index
 * @return the player, or 0
 */
extern "C" void* tNISList_GetScriptPlayer(tNISList* self, int i)
{
    void* list = (char*)&self->mList;
    if (i < cCircularBuffer_GetNumEntries_count(list)) {
        void* item = cCircularBuffer_operatorIndex(list, i);
        if (tNISList_IsVideo(self, item) == 0) {
            int id = *(int*)((char*)item + 0xC);
            if (id >= 0) {
                return cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)self, id);
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetState);
#ifdef SKIP_ASM
extern "C" void tNISList_CheckOnLoading(tNISList* self);

/**
 * Returns the NIS list's state, first polling the load while it is still loading.
 *
 * In state 1 (loading) it calls tNISList_CheckOnLoading (On Tour CheckOnLoading), which may advance the state. The state word is
 * +0xA4 (On Tour mState; tNISList_Play treats 2 as ready and 3 as playing).
 *
 * @return the list state (+0xA4)
 */
extern "C" int tNISList_GetState(tNISList* self)
{
    if (self->mState == 1) {
        tNISList_CheckOnLoading(self);
    }
    return self->mState;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetPlayState);
#ifdef SKIP_ASM
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int cBXScriptEngine_IsRequestPlaybackEnding(void* self, int a1);

/**
 * Classifies a NIS list's playback for the script engine (On Tour eNISListStatePlay).
 *
 * Returns 0 unless the list is playing (+0xA4 == 3) and 3 while it is paused (+0xC4, On Tour mNumPause). Unless a skip
 * is in progress (+0xC8, On Tour mDoSkip), a head entry with flag bit 3 (+0x8) returns 2 while a script transition is
 * running (+0xB0) with more than one wait frame left (+0xB8), and any other head entry returns 1 while cBXScriptEngine_IsRequestPlaybackEnding
 * (On Tour IsRequestPlaybackEnding) returns 0 for its request (+0xC). Otherwise it returns 4, or 5 when no next script
 * is pending (+0xBC < 0, On Tour mNextPendingScriptIndex).
 *
 * @return the play state, 0..5
 */
extern "C" int tNISList_GetPlayState(tNISList* self)
{
    if (self->mState != 3) return 0;
    if (self->mNumPause != 0) return 3;
    if (self->mDoSkip == 0) {
        void* list = (char*)&self->mList;
        if (*(int*)((char*)cCircularBuffer_operatorIndex(list, 0) + 0x8) & 8) {
            if (self->mInScriptTransition == 0) return 2;
            if (self->mTransitionWaitFrames - 1 > 0) return 2;
        } else {
            if (cBXScriptEngine_IsRequestPlaybackEnding(*(void**)self, *(int*)((char*)cCircularBuffer_operatorIndex(list, 0) + 0xC)) == 0) return 1;
        }
    }
    return self->mNextPendingScriptIndex < 0 ? 5 : 4;
}
#endif

extern "C" void* cCircularBuffer_GetNumEntries(void*);

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetLength__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of entries in a NIS list (cCircularBuffer_GetNumEntries on the buffer at +0xC).
 *
 * @return the entry count
 */
void* tNISList_GetLength(void* self)
{
    return cCircularBuffer_GetNumEntries((char*)self + 0xc);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_Update);
#ifdef SKIP_ASM
void* cGame_getTransitionMan(void* self, int a1);
extern "C" void tNISList_Play(void* self, int a1, int a2);
extern "C" void tNISList_CheckOnLoading(tNISList* self);
extern "C" void tNISList_PlayNextScriptOnAvailable(tNISList* self);
extern "C" void tNISList_PlayNextScript(tNISList* self);
extern "C" void tNISList_DoTransitionToNext(void* self);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" void cBXScriptEngine_ResumeScript(void* self, int a1);
extern "C" int cBXScriptEngine_GetRequestState(void* self, int a1);
extern "C" int cBXScriptEngine_IsRequestPlaybackEnding(void* self, int a1);
extern "C" int cVidEngine_PlayVideo(void* self);
extern "C" int cVidEngine_IsVideoOnLastFrame(void* self);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" void cTransitionMan_EndTransition(void* p);

/**
 * Advances a NIS list by one frame.
 *
 * A loading list (state 1) only polls (tNISList_CheckOnLoading). A ready list (2) with a play-on-ready mode (+0xB4) starts
 * (tNISList_Play, with the transition in for mode 1) and clears the mode. The rest runs only while playing (3) and not
 * pause-counted (+0xC4): - During a script transition (+0xB0) the transition wait (+0xB8) counts down; at 0 the next
 * script starts (tNISList_PlayNextScriptOnAvailable) unless the start-transition flag (+0xAC) is set, in which case the head resumes
 * (cBXScriptEngine_ResumeScript or cVidEngine_PlayVideo). Once the game's transition manager (cGame_getTransitionMan, +0x44) is idle
 * and the wait is over, both flags clear. - While paused on loading (+0xA8), once the pending entry (+0xBC) is ready
 * the pause is undone (tNISList_DoPause(self, 0, 1)) and tNISList_PlayNextScript plays it. - A skip in progress (+0xC8) counts down;
 * at 0 it ends a running transition (cTransitionMan_EndTransition) and starts the next script. - Otherwise the next script starts once
 * the head script's request is back to ready (2) or ending (cBXScriptEngine_IsRequestPlaybackEnding), or the head video reports done
 * (cVidEngine_IsVideoOnLastFrame), after tNISList_DoTransitionToNext.
 */
extern "C" void tNISList_Update(tNISList* self)
{
    if (self->mState == 1) {
        tNISList_CheckOnLoading(self);
        return;
    }
    if (self->mState == 2) {
        int playOnReady = self->mPlayOnReady;
        if (playOnReady != 0) {
            tNISList_Play(((char*)self), playOnReady == 1, 0);
            self->mPlayOnReady = 0;
        }
    }
    if (self->mState != 3) {
        return;
    }
    if (self->mNumPause != 0) {
        return;
    }
    if (self->mInScriptTransition != 0) {
        char* item = (char*)cCircularBuffer_operatorIndex(&self->mList, 0);
        char* obj = (char*)cGame_getTransitionMan(SSXApp->Game, 0);
        if (self->mTransitionWaitFrames != 0) {
            if (--self->mTransitionWaitFrames == 0) {
                if (self->mIsStartTransition == 0) {
                    tNISList_PlayNextScriptOnAvailable(self);
                    return;
                }
                if (tNISList_IsVideo(self, item) == 0) {
                    cBXScriptEngine_ResumeScript(*(void**)self, *(int*)(item + 0xC));
                } else {
                    cVidEngine_PlayVideo(*(char**)self + 0x500);
                }
            }
        }
        if (*(int*)(obj + 0x44) == 0 && self->mTransitionWaitFrames == 0) {
            self->mIsStartTransition = 0;
            self->mInScriptTransition = 0;
        }
    }
    if (self->mPausedOnNextScriptLoading != 0) {
        void* list = (char*)&self->mList;
        char* item = (char*)cCircularBuffer_operatorIndex(list, self->mNextPendingScriptIndex);
        int ok;
        if (tNISList_IsVideo(self, item) == 0) {
            ok = cBXScriptEngine_GetRequestState(*(void**)self, *(int*)((char*)cCircularBuffer_operatorIndex(list, self->mNextPendingScriptIndex) + 0xC)) == 2;
        } else {
            ok = *(int*)(item + 0xC) >= 0;
        }
        if (ok) {
            tNISList_DoPause(((char*)self), 0, 1);
            tNISList_PlayNextScript(self);
        }
        return;
    }
    if (self->mDoSkip != 0) {
        if (--self->mDoSkip == 0) {
            if (self->mInScriptTransition != 0) {
                cTransitionMan_EndTransition(cGame_getTransitionMan(SSXApp->Game, 0));
                self->mIsStartTransition = 0;
                self->mInScriptTransition = 0;
                self->mTransitionWaitFrames = 0;
            }
            tNISList_PlayNextScriptOnAvailable(self);
            return;
        }
    }
    char* item = (char*)cCircularBuffer_operatorIndex(&self->mList, 0);
    int done = 0;
    if (tNISList_IsVideo(self, item) == 0) {
        if (cBXScriptEngine_GetRequestState(*(void**)self, *(int*)(item + 0xC)) == 2 || cBXScriptEngine_IsRequestPlaybackEnding(*(void**)self, *(int*)(item + 0xC)) != 0) {
            done = 1;
        }
    } else if (cVidEngine_IsVideoOnLastFrame(*(char**)self + 0x500) != 0) {
        tNISList_DoTransitionToNext(((char*)self));
        done = 1;
    }
    if (done) {
        tNISList_PlayNextScriptOnAvailable(self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_DoSkip);
#ifdef SKIP_ASM
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_PlayNextScriptOnAvailable(tNISList* self);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cBXScriptEngine_ReleaseRequest(void* self, int id);
extern "C" void cVidEngine_ResetVideoEngine(void* p);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);

/**
 * Skips the playing script of a NIS list if its entry allows it.
 *
 * Only while playing (3), with no skip in progress (+0xC8) and bit 0 of the head entry's control bits (+0x8) set. Marks
 * the skip (+0xC8 = 1) and finds the next entry that is not flagged to inherit the skip (tNISList_SetupNextScript with
 * skipFlagged). When that differs from the pending one (+0xBC), the old pending entry is released (script:
 * cBXScriptEngine_ReleaseRequest; video: cVidEngine_ResetVideoEngine) and replaced; when one is found the next script starts
 * (tNISList_PlayNextScriptOnAvailable).
 *
 * Quirk: with nothing pending, the search from entry 1 runs but its result is not stored.
 */
extern "C" void tNISList_DoSkip(tNISList* self)
{
    if (self->mState != 3) {
        return;
    }
    if (self->mDoSkip != 0) {
        return;
    }
    void* list = (char*)&self->mList;
    char* entry = (char*)cCircularBuffer_operatorIndex(list, 0);
    if (!(*(int*)(entry + 0x8) & 1)) {
        return;
    }
    self->mDoSkip = 1;
    if (self->mNextPendingScriptIndex < 0) {
        tNISList_SetupNextScript(self, 1, 1);
        return;
    }
    int idx = tNISList_SetupNextScript(self, self->mNextPendingScriptIndex, 1);
    if (idx != self->mNextPendingScriptIndex) {
        char* pendingEntry = (char*)cCircularBuffer_operatorIndex(list, self->mNextPendingScriptIndex);
        if (tNISList_IsVideo(self, pendingEntry) == 0) {
            cBXScriptEngine_ReleaseRequest(*(void**)self, *(int*)(pendingEntry + 0xC));
            *(int*)(pendingEntry + 0xC) = -1;
        } else if (*(int*)(pendingEntry + 0xC) >= 0) {
            cVidEngine_ResetVideoEngine(*(char**)self + 0x500);
        }
        self->mNextPendingScriptIndex = idx;
    }
    if (idx >= 0) {
        tNISList_PlayNextScriptOnAvailable(self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsSkipAllowed);
#ifdef SKIP_ASM
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);

/**
 * Says whether the playing NIS list may be skipped now.
 *
 * True only while playing (+0xA4 == 3) when the head entry's flag bit 0 (+0x8) is set and the list is not paused
 * waiting for its next script (+0xA8, On Tour mPausedOnNextScriptLoading). cSSXScriptEngine_Update checks it before
 * honouring the skip input, and cSSXScriptEngine_IsActiveNISSkippable reports it.
 *
 * @return 1 if the list may be skipped, else 0
 */
extern "C" int tNISList_IsSkipAllowed(tNISList* self)
{
    if (self->mState != 3) {
        return 0;
    }
    int allowed = 0;
    void* headEntry = cCircularBuffer_operatorIndex((char*)&self->mList, 0);
    if (*(int*)((char*)headEntry + 0x8) & 1) {
        allowed = self->mPausedOnNextScriptLoading == 0;
    }
    return allowed;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsPauseMenuAllowed);
#ifdef SKIP_ASM
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);

/**
 * Returns whether the pause menu may open: always when the list is not playing, else bit 2 of the head entry's control
 * bits (+0x8).
 *
 * @return 1 when allowed, else 0
 */
extern "C" int tNISList_IsPauseMenuAllowed(tNISList* self)
{
    if (self->mState != 3) {
        return 1;
    }
    void* entry = cCircularBuffer_operatorIndex((char*)&self->mList, 0);
    return (*(int*)((char*)entry + 0x8) >> 2) & 1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsPendingScriptReady);
#ifdef SKIP_ASM
/**
 * Returns whether the NIS list's pending entry (+0xBC) is ready to play.
 *
 * 0 while loading (state 1); 1 when nothing is pending; for a pending video whether it has a handle (+0xC >= 0); for a
 * pending script whether its request is ready (cBXScriptEngine_GetRequestState returns 2).
 *
 * @return 1 when ready, else 0
 */
extern "C" int tNISList_IsPendingScriptReady(tNISList* self)
{
    if (self->mState == 1) {
        return 0;
    }
    int idx = self->mNextPendingScriptIndex;
    void* list = (char*)&self->mList;
    if (idx < 0) {
        return 1;
    }
    void* item = cCircularBuffer_operatorIndex(list, idx);
    if (tNISList_IsVideo(self, item) != 0) {
        return *(int*)((char*)item + 0xC) >= 0;
    }
    return cBXScriptEngine_GetRequestState(*(void**)self, *(int*)((char*)cCircularBuffer_operatorIndex(list, self->mNextPendingScriptIndex) + 0xC)) == 2;
}
#endif
//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsListFull);
#ifdef SKIP_ASM
extern "C" void cCircularBuffer_IsFull__FPv(int);

/**
 * Asks the NIS list's entry buffer (+0xC) whether it is full: cCircularBuffer_IsFull__FPv.
 *
 * The C declares no return value; the answer is left in the return register.
 *
 * @param nisList  the NIS list
 */
extern "C" void tNISList_IsListFull(int nisList) {
    cCircularBuffer_IsFull__FPv(nisList + 0xC);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsPlayingScriptOwner);
#ifdef SKIP_ASM
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);

/**
 * Returns whether @p obj is the script player of the NIS list's playing head entry.
 *
 * Requires the list to be playing (3) and the head to be a script (tNISList_IsVideo); compares @p obj with
 * cBXScriptEngine_GetRequestPlayerByReqHandle(ScriptEngine, head handle).
 *
 * @param obj  the InSeq script player (cScriptPlayerMain)
 * @return 1 when it is, else 0
 */
extern "C" int tNISList_IsPlayingScriptOwner(tNISList* self, cScriptPlayerMain* obj)
{
    if (self->mState != 3) {
        return 0;
    }
    void* headEntry = cCircularBuffer_operatorIndex((char*)&self->mList, 0);
    if (tNISList_IsVideo(self, headEntry) != 0) {
        return 0;
    }
    return ((void*)obj) == cBXScriptEngine_GetRequestPlayerByReqHandle(ScriptEngine, *(int*)((char*)headEntry + 0xC));
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsPlayingVideo);
#ifdef SKIP_ASM
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);

/**
 * Returns whether the NIS list is playing (state 3) a video entry at its head (tNISList_IsVideo).
 *
 * @return 1 when playing a video, else 0
 */
extern "C" int tNISList_IsPlayingVideo(tNISList* self)
{
    if (self->mState != 3) {
        return 0;
    }
    return tNISList_IsVideo(self, cCircularBuffer_operatorIndex(&self->mList, 0));
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsPausedPendingNextScript);
#ifdef SKIP_ASM
/**
 * Says whether the playing NIS list is waiting for its next script to finish loading.
 *
 * True while playing (+0xA4 == 3) with the paused-on-loading flag (+0xA8, On Tour mPausedOnNextScriptLoading) set.
 * cSSXScriptEngine_Update shows the load indicator (cBXLoadIndicator_Show) while it is, if the "hud" UI state is up.
 *
 * @return 1 if paused pending the next script, else 0
 */
extern "C" int tNISList_IsPausedPendingNextScript(tNISList* self)
{
    int paused = 0;
    if (self->mState == 3) {
        paused = self->mPausedOnNextScriptLoading != 0;
    }
    return paused;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsInTransition__FPv);
#ifdef SKIP_ASM
/**
 * Tells whether the NIS list is between scripts: returns its transition flag at +0xB0.
 *
 * @return the transition flag (nonzero during a transition)
 */
int tNISList_IsInTransition(void* self)
{
    return *(int*)((char*)self + 0xB0);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_OnScriptPlay);
#ifdef SKIP_ASM
extern "C" int tNISList_IsPlayingScriptOwner(tNISList* self, cScriptPlayerMain* obj);
char* GetSSXScriptHeader(void* obj);
extern "C" void cRenderStateMan_setCustomIrradiance(int a0);
extern char str_TRANSP[];

/**
 * Reacts to a script starting to play: for a script this NIS list owns, switches the custom irradiance.
 *
 * When tNISList_IsPlayingScriptOwner (On Tour IsPlayingScriptOwner) accepts the player and the script header from GetSSXScriptHeader
 * (GetSSXScriptHeader) starts with a non-zero byte, calls cRenderStateMan_setCustomIrradiance (On Tour
 * cRenderStateMan::setCustomIrradiance) with "TRANSP".
 *
 * @param requestHandle   the play request handle (On Tour requestHandle); unused
 * @param obj  the script player (On Tour cScriptPlayerMain*)
 */
extern "C" void tNISList_OnScriptPlay(tNISList* self, int requestHandle, cScriptPlayerMain* obj)
{
    if (tNISList_IsPlayingScriptOwner(self, (cScriptPlayerMain*)obj) != 0) {
        char* name = GetSSXScriptHeader(obj);
        char* msg = str_TRANSP;
        if (*name != 0) {
            cRenderStateMan_setCustomIrradiance((int)msg); // PORT: string pointer passed as int (unit declares cRenderStateMan_setCustomIrradiance(int))
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_OnScriptUpdate);
#ifdef SKIP_ASM
extern "C" int tNISList_IsPlayingScriptOwner(tNISList* self, cScriptPlayerMain* obj);
char* GetSSXScriptHeader(void* obj);
extern "C" int cScriptPlayerMain_GetNumUpdatesToEndLeft(void* obj);
extern "C" void tNISList_DoTransitionToNext(void* self);

/**
 * Per-frame script callback: starts the transition to the next NIS once the playing script nears its end.
 *
 * Only for the list's own script player (tNISList_IsPlayingScriptOwner, On Tour IsPlayingScriptOwner) on the cut-scene list (+0x4) and
 * while the player's +0x1C word is 0: when the updates left (cScriptPlayerMain_GetNumUpdatesToEndLeft() + 1) drop to
 * the script header's next-transition in-frame count (short at +0x12 of GetSSXScriptHeader's header), calls tNISList_DoTransitionToNext
 * (On Tour DoTransitionToNext). The current frame and frame delta that On Tour passes are not declared (unused).
 *
 * @param requestHandle  the request handle (unused)
 * @param obj  the cScriptPlayerMain being updated
 */
extern "C" void tNISList_OnScriptUpdate(tNISList* self, int requestHandle, cScriptPlayerMain* obj)
{
    if (tNISList_IsPlayingScriptOwner(self, (cScriptPlayerMain*)obj) != 0 && self->mIsCutSceneHandler != 0 &&
        obj->mLoopScript == 0) {
        char* name = GetSSXScriptHeader(obj);
        if (cScriptPlayerMain_GetNumUpdatesToEndLeft(obj) + 1 <= *(short*)(name + 0x12)) {
            tNISList_DoTransitionToNext(self);
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_OnScriptStopping);
#ifdef SKIP_ASM
extern "C" int tNISList_IsPlayingScriptOwner(tNISList* self, cScriptPlayerMain* obj);
extern "C" void tNISList_WaitOnPendingNextScript(tNISList* self);
extern "C" void cBXScriptEngine_PauseScript(void* self, int a1);

/**
 * Handles the NIS list's playing script starting to stop.
 *
 * Only for the list's own player (tNISList_IsPlayingScriptOwner): while a transition wait is pending (+0xB8) the request is paused
 * (cBXScriptEngine_PauseScript), then tNISList_WaitOnPendingNextScript checks whether the next script must be waited for.
 *
 * @param requestHandle  the stopping script's request handle
 * @param obj  the stopping InSeq player
 */
extern "C" void tNISList_OnScriptStopping(tNISList* self, int requestHandle, cScriptPlayerMain* obj)
{
    if (tNISList_IsPlayingScriptOwner(self, (cScriptPlayerMain*)obj) != 0) {
        if (self->mTransitionWaitFrames != 0) {
            cBXScriptEngine_PauseScript(*(void**)self, requestHandle);
        }
        tNISList_WaitOnPendingNextScript(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_OnScriptStop);
#ifdef SKIP_ASM
extern "C" int tNISList_IsPlayingScriptOwner(tNISList* self, cScriptPlayerMain* obj);
char* GetSSXScriptHeader(void* obj);
extern "C" void cRenderStateMan_setCustomIrradiance(int a0);

/**
 * Script stop callback: drops the custom irradiance the list's script had set.
 *
 * For the list's own script player (tNISList_IsPlayingScriptOwner, On Tour IsPlayingScriptOwner), when the script header
 * (GetSSXScriptHeader) starts with a non-empty name, calls cRenderStateMan_setCustomIrradiance(0) (On Tour cRenderStateMan::setCustomIrradiance)
 * to clear it.
 *
 * @param requestHandle  the request handle (unused)
 * @param obj  the cScriptPlayerMain that stopped
 */
extern "C" void tNISList_OnScriptStop(tNISList* self, int requestHandle, cScriptPlayerMain* obj)
{
    if (tNISList_IsPlayingScriptOwner(self, (cScriptPlayerMain*)obj) != 0) {
        if (*GetSSXScriptHeader(obj) != 0) {
            cRenderStateMan_setCustomIrradiance(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_SetupNextScript);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cSSXScriptEngine_RequestScriptByCategory(void* self, int category, int a, int b, int c);
extern "C" int cSSXScriptEngine_RequestScriptByIndex(void* self, int key, int a, int b, int c);
extern "C" int cVidEngine_ReadyVideo(void** self, int file);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int* tNISList_GetVideoDBRecord_p(void* self, void* ent) __asm__("tNISList_GetVideoDBRecord");
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

struct sDmEnt7450 {
    int f0;     // 0x0
    int f4;     // 0x4
    int flags;  // 0x8
    int id;     // 0xC
    int a;      // 0x10
    int b;      // 0x14
    int c;      // 0x18
};

/**
 * Finds the next entry of the NIS list that can play, requesting its script or readying its video.
 *
 * Starting at @p index, an entry already requested is returned at once; a script entry is requested by index
 * (or by category when it has none) and returned when the request succeeds; a video entry readies its video
 * when no video player exists yet and is returned.
 *
 * @param vself  the NIS list (tNISList)
 * @param index  entry to start searching at (On Tour startSearchIndex)
 * @param skipFlagged  nonzero to pass over entries with flag 2 (On Tour ignoreSkipInheritScripts)
 * @return the index of the entry set up, or -1 when none can be
 */
extern "C" int tNISList_SetupNextScript(tNISList* vself, int index, int skipFlagged)
{
    int count = cCircularBuffer_GetNumEntries_count(&vself->mList);
    int i = index;
    for (; i < count; i++) {
        sDmEnt7450* currEntry = (sDmEnt7450*)cCircularBuffer_operatorIndex(&vself->mList, i);
        if (skipFlagged && (currEntry->flags & 2))
            continue;
        if (currEntry->id >= 0)
            return i;
        if (tNISList_IsVideo(vself, currEntry) == 0) {
            void* eng = *(void**)vself;
            if (currEntry->f4 >= 0)
                currEntry->id = cSSXScriptEngine_RequestScriptByIndex(eng, currEntry->f4, currEntry->a, currEntry->b, currEntry->c);
            else
                currEntry->id = cSSXScriptEngine_RequestScriptByCategory(*(void**)vself, currEntry->f0, currEntry->a, currEntry->b, currEntry->c);
            if (currEntry->id >= 0)
                return i;
        } else {
            if (*(int*)(*(char**)vself + 0x500) == 0) {
                int* videoEntry = tNISList_GetVideoDBRecord_p(((char*)vself), currEntry);
                if (cVidEngine_ReadyVideo((void**)(*(char**)vself + 0x500), *videoEntry) != 0)
                    currEntry->id = 0;
            }
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_RemoveFromHead);
#ifdef SKIP_ASM
struct sDmEnt7DE8;
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" sDmEnt7DE8* tNISList_GetVideoDBRecord(tNISList* self, int* key);
extern "C" int cBXScriptEngine_ReleaseRequest(void* self, int id);
extern "C" void cVidEngine_ResetVideoEngine(void* p);
extern "C" int cVidEngine_IsVideoOnLastFrame(void* self);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int cCircularBuffer_IsEmpty(void* self);
extern "C" int cCircularBuffer_RemoveFromHead(void* self, void* out);
extern "C" void* cAudioInterface_bxAudio();

struct sDmCmd7598 {
    int category;   // 0x0
    int key;        // 0x4
    int flags;      // 0x8
    int id;         // 0xC
    int a;          // 0x10
    int b;          // 0x14
    int c;          // 0x18
};

struct sDmSndVEntry7598 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Removes up to @p n entries from the head of a NIS list, releasing what each one holds.
 *
 * Stops early when the buffer is empty (cCircularBuffer_IsEmpty). An entry with a handle (+0xC >= 0) releases its script request
 * (cBXScriptEngine_ReleaseRequest) or, for a video, sends the video record's music event when the video engine is playing (+0x504): the
 * end event (+0x18 of the tNISList_GetVideoDBRecord record) once cVidEngine_IsVideoOnLastFrame reports the video done, else the skip event (+0x14),
 * through virtual slot 4 of the bxAudio sound system (cAudioInterface_bxAudio, +0x118); then resets the video engine
 * (cVidEngine_ResetVideoEngine). Each entry is then popped (cCircularBuffer_RemoveFromHead).
 *
 * @param selfp  the NIS list
 * @param n  the number of entries to remove
 */
extern "C" void tNISList_RemoveFromHead(tNISList* selfp, int n)
{
    while (n != 0) {
        if (cCircularBuffer_IsEmpty(&selfp->mList) != 0) {
            return;
        }
        sDmCmd7598 cmd = *(sDmCmd7598*)cCircularBuffer_operatorIndex(&selfp->mList, 0);
        if (cmd.id >= 0) {
            if (tNISList_IsVideo(selfp, &cmd) == 0) {
                cBXScriptEngine_ReleaseRequest(*(void**)selfp, cmd.id);
            } else {
                if (*(int*)(*(char**)selfp + 0x504) != 0) {
                    int* ent = (int*)tNISList_GetVideoDBRecord(selfp, (int*)&cmd);
                    int snd;
                    if (cVidEngine_IsVideoOnLastFrame(*(char**)selfp + 0x500) != 0) {
                        snd = ent[6];
                    } else {
                        snd = ent[5];
                    }
                    if (snd >= 0) {
                        char* soundMan = (char*)cAudioInterface_bxAudio();
                        sDmSndVEntry7598* vt = *(sDmSndVEntry7598**)(soundMan + 0x5558);
                        char* sys = soundMan + 0x118;
                        vt[4].fn(sys + vt[4].delta, snd);
                    }
                }
                cVidEngine_ResetVideoEngine(*(char**)selfp + 0x500);
            }
        }
        cCircularBuffer_RemoveFromHead(&selfp->mList, 0);
        n--;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_WaitOnPendingNextScript);
#ifdef SKIP_ASM
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cBXScriptEngine_GetRequestState(void* self, int a1);

/**
 * Pauses the NIS list on loading while its pending entry (+0xBC) is still loading.
 *
 * The pending entry is still loading when its script request waits for data (cBXScriptEngine_GetRequestState returns 1) or, for a video,
 * it has no handle (+0xC < 0); then tNISList_DoPause(self, 1, 1) pauses the list. Nothing happens with nothing pending.
 */
extern "C" void tNISList_WaitOnPendingNextScript(tNISList* self)
{
    int idx = self->mNextPendingScriptIndex;
    if (idx >= 0) {
        void* list = (char*)&self->mList;
        char* nextEntry = (char*)cCircularBuffer_operatorIndex(list, idx);
        int ok;
        if (tNISList_IsVideo(self, nextEntry) == 0) {
            ok = cBXScriptEngine_GetRequestState(*(void**)self,
                               *(int*)((char*)cCircularBuffer_operatorIndex(list, self->mNextPendingScriptIndex) + 0xC)) == 1;
        } else {
            ok = *(int*)(nextEntry + 0xC) < 0;
        }
        if (ok) {
            tNISList_DoPause(self, 1, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_CheckOnLoading);
#ifdef SKIP_ASM
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cBXScriptEngine_GetRequestState(void* self, int a1);

/**
 * Moves a loading NIS list (state 1) to ready (2) once its head is ready.
 *
 * A head video counts as ready at once; a head script when its request state (cBXScriptEngine_GetRequestState) is 2.
 */
extern "C" void tNISList_CheckOnLoading(tNISList* self)
{
    if (self->mState == 1) {
        char* headEntry = (char*)cCircularBuffer_operatorIndex((char*)&self->mList, 0);
        if (tNISList_IsVideo(self, headEntry) == 0) {
            int requestState = cBXScriptEngine_GetRequestState(*(void**)self, *(int*)(headEntry + 0xC));
            if (requestState == 2) {
                self->mState = requestState;
            }
        } else {
            self->mState = 2;
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_PlayNextScriptOnAvailable);
#ifdef SKIP_ASM
extern "C" void tNISList_WaitOnPendingNextScript(tNISList* self);
extern "C" void tNISList_PlayNextScript(tNISList* self);

/**
 * Plays the NIS list's next script when it is available: tNISList_WaitOnPendingNextScript first, then tNISList_PlayNextScript unless the list is now
 * paused on loading (+0xA8).
 */
extern "C" void tNISList_PlayNextScriptOnAvailable(tNISList* self)
{
    tNISList_WaitOnPendingNextScript(self);
    if (self->mPausedOnNextScriptLoading == 0) {
        tNISList_PlayNextScript(self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_PlayNextScript);
#ifdef SKIP_ASM
void* cGame_getTransitionMan(void* self, int a1);
extern "C" void tNISList_Reset(tNISList* self, int a1);
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_RemoveFromHead(tNISList* self, int index);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" void cSSXScriptEngine_PlayRequestScript(void* self, int val, int key, int b, int c);
extern "C" int cVidEngine_PlayVideo(void* self);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" float cTransitionMan_GetTransitionTimeLeft(void* self);
extern "C" void cCutSceneTransitionMan_EndCutScene(void* self, float t);

struct sDmCmd7838 {
    int category;   // 0x0
    int key;        // 0x4
    int flags;      // 0x8
    int id;         // 0xC
};

struct sDmEngVEntry7838 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

/**
 * Moves a NIS list on to its pending entry, or ends the list when nothing is pending.
 *
 * With a pending entry (+0xBC > 0): clears the skip (+0xC8), drops the entries before it (tNISList_RemoveFromHead), clears the
 * skip position (+0xC0) and pending index, and starts the new head: a script with cSSXScriptEngine_PlayRequestScript
 * (view index +0x8, bit 3 of its control bits) and the engine's virtual slot 6 (cSSXScriptEngine_ForcePlayUpdate), a
 * video with cVidEngine_PlayVideo. A running script transition (+0xB0) sets the start-transition flag (+0xAC); the next
 * entry is set up as pending. Otherwise the list is reset without transitions (tNISList_Reset) and, on the cut-scene
 * list (+0x4), the game's fade (+0x64 of the game) gets cCutSceneTransitionMan_EndCutScene with the transition manager's remaining time
 * (cTransitionMan_GetTransitionTimeLeft) when a script transition was running and the manager is busy, else 0.5.
 *
 * @param selfp  the NIS list
 */
extern "C" void tNISList_PlayNextScript(tNISList* selfp)
{
    if (selfp->mNextPendingScriptIndex > 0) {
        selfp->mDoSkip = 0;
        tNISList_RemoveFromHead(selfp, selfp->mNextPendingScriptIndex);
        selfp->mNumScriptsToSkipInProgress = 0;
        selfp->mNextPendingScriptIndex = -1;
        sDmCmd7838* cmd = (sDmCmd7838*)cCircularBuffer_operatorIndex(&selfp->mList, 0);
        if (tNISList_IsVideo(selfp, cmd) == 0) {
            cSSXScriptEngine_PlayRequestScript(*(void**)selfp, selfp->mViewIndex, cmd->id, 0, (cmd->flags >> 3) & 1);
            char* eng = *(char**)selfp;
            sDmEngVEntry7838* vt = *(sDmEngVEntry7838**)(eng + 0x2A8);
            vt[6].fn(eng + vt[6].delta, cmd->id);
        } else {
            cVidEngine_PlayVideo(*(char**)selfp + 0x500);
        }
        if (selfp->mInScriptTransition != 0) {
            selfp->mIsStartTransition = 1;
        }
        selfp->mNextPendingScriptIndex = tNISList_SetupNextScript(selfp, 1, 0);
    } else {
        int wasOn = selfp->mInScriptTransition;
        tNISList_Reset(selfp, 0);
        if (selfp->mIsCutSceneHandler != 0) {
            cGame* app = SSXApp->Game;
            void* fade = app->CutSceneTransitionMan;
            char* transitionMan = (char*)cGame_getTransitionMan(app, 0);
            if (wasOn != 0 && *(int*)(transitionMan + 0x44) != 0) {
                cCutSceneTransitionMan_EndCutScene(fade, cTransitionMan_GetTransitionTimeLeft(transitionMan));
            } else {
                cCutSceneTransitionMan_EndCutScene(fade, 0.5f);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_DoTransitionToNext);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cGame_getTransitionMan(void* self, int a1);
char* GetSSXScriptHeader(void* obj);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
struct sDmEnt7DE8;
extern "C" sDmEnt7DE8* tNISList_GetVideoDBRecord(tNISList* self, int* key);
extern "C" void* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" void cTransitionMan_EndTransition(void* p);
extern "C" int cSSXScriptEngine_MakeTransitionObject(void* self, int a1, int a2);
extern "C" void cTransitionMan_StartTransition(void* snd, int a1, int h, int h2, int a4, int a5, float t0, float t1, float t2);
extern "C" void cTransitionMan_StartTransitionOut(void* snd, int a1, int h, float t);
extern "C" void cTransitionMan_StartTransitionScreenGrab(void* snd, int a1, int h, int a3, float t);

struct sDmFade_7980 {
    char pad_0x0[0x10];
    signed char cat;        // 0x10
    signed char key;        // 0x11
    short tIn;              // 0x12
    short tHold;            // 0x14
    short tOut;             // 0x16
};

struct sDmEnt_7980 {
    int v0;
    int cat;                // 0x4
    int key;                // 0x8
    float t;                // 0xC
};

// The unit declares tNISList_DoTransitionToNext as void; it returns whether it handled the update.
int tNISList_DoTransitionToNext_impl(char* self) __asm__("tNISList_DoTransitionToNext");

/**
 * Starts the transition from the current NIS entry into the next, or reports one already running.
 *
 * With a script transition running (+0xB0, On Tour mInScriptTransition) it returns 1, unless the start flag (+0xAC,
 * mIsStartTransition) is set: then it ends the transition (cTransitionMan_EndTransition on the game's transition manager,
 * cGame_getTransitionMan(Game, 0)) and returns 0. Otherwise it ends any transition and builds the next one. For a script head
 * entry it reads the next-transition record of the script header (GetSSXScriptHeader of the request's player: type +0x10,
 * colour +0x11, in/hold/out frame shorts +0x12..+0x16, 1/60 s each): type 7 is a screen grab (cTransitionMan_StartTransitionScreenGrab) with no
 * wait, any other type a full transition (cTransitionMan_StartTransition with two cSSXScriptEngine_MakeTransitionObject objects; returns
 * 0 if none can be made) whose in+hold frames go to +0xB8 (mTransitionWaitFrames). A video entry uses its video record
 * (tNISList_GetVideoDBRecord, On Tour GetVideoDBRecord) for a transition out (cTransitionMan_StartTransitionOut) or a screen grab. A positive wait sets
 * +0xB0 and clears +0xAC.
 *
 * @return +0xB0: 1 while a script transition is running
 */
int tNISList_DoTransitionToNext_impl(char* self)
{
    if (*(int*)(self + 0xB0) != 0) {
        if (*(int*)(self + 0xAC) != 0) {
            cTransitionMan_EndTransition(cGame_getTransitionMan(SSXApp->Game, 0));
            return 0;
        }
        return 1;
    }
    char* item = (char*)cCircularBuffer_operatorIndex(self + 0xC, 0);
    if (tNISList_IsVideo((tNISList*)self, item) == 0) {
        sDmFade_7980* transitionInfo = (sDmFade_7980*)GetSSXScriptHeader(cBXScriptEngine_GetRequestPlayerByReqHandle(ScriptEngine, *(int*)(item + 0xC)));
        void* snd = cGame_getTransitionMan(SSXApp->Game, 0);
        if (*(int*)((char*)snd + 0x44) != 0) {
            cTransitionMan_EndTransition(snd);
        }
        if (transitionInfo->tIn + transitionInfo->tHold + transitionInfo->tOut > 0) {
            if (transitionInfo->cat != 7) {
                int transition = cSSXScriptEngine_MakeTransitionObject(*(void**)self, transitionInfo->cat, transitionInfo->key);
                if (transition == 0) {
                    return 0;
                }
                int transitionOut = cSSXScriptEngine_MakeTransitionObject(*(void**)self, transitionInfo->cat, transitionInfo->key);
                cTransitionMan_StartTransition(snd, 2, transition, transitionOut, 0, 0, (float)transitionInfo->tIn * 0.01666666753590107f, (float)transitionInfo->tHold * 0.01666666753590107f, (float)transitionInfo->tOut * 0.01666666753590107f);
                *(int*)(self + 0xB8) = transitionInfo->tIn + transitionInfo->tHold;
            } else {
                int transition = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, 7, transitionInfo->key);
                cTransitionMan_StartTransitionScreenGrab(snd, 2, transition, 0, (float)transitionInfo->tIn * 0.01666666753590107f);
                *(int*)(self + 0xB8) = 0;
            }
            if (*(int*)(self + 0xB8) > 0) {
                *(int*)(self + 0xAC) = 0;
                *(int*)(self + 0xB0) = 1;
            }
        }
    } else {
        sDmEnt_7980* videoInfo = (sDmEnt_7980*)tNISList_GetVideoDBRecord((tNISList*)self, (int*)item);
        void* snd = cGame_getTransitionMan(SSXApp->Game, 0);
        if (*(int*)((char*)snd + 0x44) != 0) {
            cTransitionMan_EndTransition(snd);
        }
        if (0.0f < videoInfo->t) {
            if (videoInfo->cat != 7) {
                int transition = cSSXScriptEngine_MakeTransitionObject(*(void**)self, videoInfo->cat, videoInfo->key);
                cTransitionMan_StartTransitionOut(snd, 2, transition, videoInfo->t);
                *(int*)(self + 0xB8) = 0;
            } else {
                int transition = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, 7, videoInfo->key);
                cTransitionMan_StartTransitionScreenGrab(snd, 2, transition, 0, videoInfo->t);
                *(int*)(self + 0xB8) = 0;
            }
        }
    }
    return *(int*)(self + 0xB0);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_DoPause);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cGame_getTransitionMan(void* self, int a1);
extern "C" int tNISList_IsVideo(tNISList* self, void* a1);
extern "C" void cBXScriptEngine_PauseScript(void* self, int a1);
extern "C" void cBXScriptEngine_ResumeScript(void* self, int a1);
extern "C" int cBXScriptEngine_GetRequestState(void* self, int a1);
extern "C" void cVidEngine_PauseVideo(void* self, int on);
extern "C" void* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" void cTransitionMan_PauseTransition(void* p, int on);
extern "C" void cCutSceneTransitionMan_Pause(void* p, int on);

struct sSeqItem_7C08 {
    char pad_0x0[0xC];
    int handle;         // 0xC
};


// The unit declares tNISList_DoPause as returning void*; the body returns nothing. Bind by asm label.
extern "C" void tNISList_DoPause_impl(tNISList* self, int on, int hard) __asm__("tNISList_DoPause");

/**
 * Pauses or resumes a playing NIS list, through either the pause count or the pause-on-loading flag.
 *
 * Does nothing unless playing (3). With @p hard 0 the pause count (+0xC4) goes up or down and only the 0-to-1 or 1-to-0
 * step acts, and not while paused on loading (+0xA8). With @p hard set the pause-on-loading flag is set to @p on and
 * acts only when it changed and no counted pause is held. Acting pauses the head (a script request in states 3-5 with
 * cBXScriptEngine_PauseScript; a video with cVidEngine_PauseVideo) or resumes it (a paused request, state 5, with cBXScriptEngine_ResumeScript; a video with
 * cVidEngine_PauseVideo), and on the cut-scene list (+0x4) passes the same flag to the game's cut-scene transition manager
 * (cCutSceneTransitionMan_Pause) and, when busy (+0x44), to the transition manager (cTransitionMan_PauseTransition).
 *
 * @param on  nonzero to pause, 0 to resume
 * @param hard  nonzero for the pause-on-loading flag, 0 for the pause count
 */
extern "C" void tNISList_DoPause_impl(tNISList* self, int on, int hard)
{
    if (self->mState != 3) {
        return;
    }
    if (hard == 0) {
        int prevPauses = self->mNumPause;
        if (on) {
            self->mNumPause = prevPauses + 1;
            if (prevPauses != 0) {
                return;
            }
        } else {
            if (prevPauses <= 0) {
                return;
            }
            int remainingPauses = prevPauses - 1;
            self->mNumPause = remainingPauses;
            if (remainingPauses != 0) {
                return;
            }
        }
        if (self->mPausedOnNextScriptLoading) {
            return;
        }
    } else {
        if (self->mPausedOnNextScriptLoading == on) {
            return;
        }
        self->mPausedOnNextScriptLoading = on;
        if (self->mNumPause) {
            return;
        }
    }
    if (on) {
        sSeqItem_7C08* it = (sSeqItem_7C08*)cCircularBuffer_operatorIndex(((char*)&self->mList), 0);
        if (tNISList_IsVideo((tNISList*)self, it) == 0) {
            int st = cBXScriptEngine_GetRequestState(((char*)self->mpNISEngine), it->handle);
            if (st >= 3 && st <= 5) {
                cBXScriptEngine_PauseScript(((char*)self->mpNISEngine), it->handle);
            }
        } else {
            cVidEngine_PauseVideo(&self->mpNISEngine->mpPlayer, 1);
        }
        if (self->mIsCutSceneHandler) {
            cCutSceneTransitionMan_Pause(SSXApp->Game->CutSceneTransitionMan, 1);
            char* transitionMan = (char*)cGame_getTransitionMan(SSXApp->Game, 0);
            if (*(int*)(transitionMan + 0x44)) {
                cTransitionMan_PauseTransition(transitionMan, 1);
            }
        }
    } else {
        sSeqItem_7C08* it = (sSeqItem_7C08*)cCircularBuffer_operatorIndex(((char*)&self->mList), 0);
        if (tNISList_IsVideo((tNISList*)self, it) == 0) {
            if (cBXScriptEngine_GetRequestState(((char*)self->mpNISEngine), it->handle) == 5) {
                cBXScriptEngine_ResumeScript(((char*)self->mpNISEngine), it->handle);
            }
        } else {
            cVidEngine_PauseVideo(&self->mpNISEngine->mpPlayer, 0);
        }
        if (self->mIsCutSceneHandler) {
            cCutSceneTransitionMan_Pause(SSXApp->Game->CutSceneTransitionMan, 0);
            char* transitionMan = (char*)cGame_getTransitionMan(SSXApp->Game, 0);
            if (*(int*)(transitionMan + 0x44)) {
                cTransitionMan_PauseTransition(transitionMan, 0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsVideo);
#ifdef SKIP_ASM
/**
 * Returns whether a NIS list entry is a video: its category (+0x0) is 29 or more.
 *
 * @param entry  the list entry
 * @return 1 for a video, 0 for a script
 */
extern "C" int tNISList_IsVideo(tNISList* self, void* entry)
{
    return *(int*)entry >= 29;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetVideoDBRecord);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

struct sDmCfg7DE8 {
    unsigned int flags;
    int v[(0x288 - 4) / 4];
};

struct sDmEnt7DE8 {
    int v[7];
};

extern sDmCfg7DE8 cBEOptionInterface_mOption;
extern sDmEnt7DE8 D_004823DC[];
extern sDmEnt7DE8 D_0048247C[];

/**
 * Returns the video record (0x1C bytes: file name, transition out, colour, out time, music events) for a NIS list video
 * entry.
 *
 * Calls cBE_getInterface_Fv(cBE_getBE(), 4), copies the whole cBEOptionInterface_mOption block to the stack to test its
 * flags, and picks the table at 0x00482708 when (flags & 0x300000) == 0x200000, else the one at 0x004827A8. The symbols
 * D_004823DC and D_0048247C are those tables less 29 records, so the entry's category (+0x0, 29-31) indexes them
 * directly.
 *
 * @param key  the list entry (its first word is the video category)
 * @return the video record
 */
extern "C" sDmEnt7DE8* tNISList_GetVideoDBRecord(tNISList* self, int* key)
{
    cBE_getInterface_Fv(cBE_getBE(), 4);
    sDmCfg7DE8 cfg = cBEOptionInterface_mOption;
    if ((cfg.flags & 0x300000) != 0x200000) {
        return &D_0048247C[*key];
    }
    return &D_004823DC[*key];
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tSSXScriptBankInfo_Clear__FPv);
#ifdef SKIP_ASM
/**
 * Resets a script bank info record: zeroes its words at +0x0, +0x8 and +0xC and sets +0x4 to -1.
 */
void tSSXScriptBankInfo_Clear(void* self)
{
    *(int*)self = 0;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xc) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cSSXScriptEngine_cSSXScriptEngine);
#ifdef SKIP_ASM
extern "C" void cBXScriptEngine_cBXScriptEngine(void* self);
extern "C" void cVidEngine_cVidEngine(void* self);
extern "C" void cScriptObjCameraManager_cScriptObjCameraManager(void* self);
extern "C" void cScriptObjGS_cScriptObjGS(void* self);
extern "C" void cScriptObjEvent_cScriptObjEvent(void* self);
extern "C" void cScriptAnimBankManager_cScriptAnimBankManager(void* self, int flags, int a2, int a3);
extern "C" void cScriptSoundBankManager_cScriptSoundBankManager(void* self, int flags, int a2, int a3);
void cScriptObjCameraManager_SetViewIndex(void* self, int i);
extern void* D_00482100[];
extern void* D_004821A0[];
extern void* D_00482168[];
extern void* cSSXScriptEngine_vtbl[];
extern void* D_004A34B0;

struct sScriptSlot_7F20 {
    char data[0x120];
    sScriptSlot_7F20() { cScriptObjCameraManager_cScriptObjCameraManager(this); }
    void* operator new[](unsigned int, void* p) { return p; }
};

struct sScriptMgr_7F20 {
    char pad_0x0[0x2A8];
    void** vt2A8;               // 0x2A8
    void** vt2AC;               // 0x2AC
    char pad_0x2B0[0x518 - 0x2B0];
    void** vt518;               // 0x518
    char pad_0x51C[0x528 - 0x51C];
    void** vt528;               // 0x528
    int f52C;                   // 0x52C
    int f530;                   // 0x530
    int f534;                   // 0x534
    int f538;                   // 0x538
    int f53C;                   // 0x53C
    int f540;                   // 0x540
    int f544;                   // 0x544
    int f548;                   // 0x548
    int f54C;                   // 0x54C
    int f550;                   // 0x550
    int f554;                   // 0x554
    int ids[50];                // 0x558
    int f620;                   // 0x620
    int f624;                   // 0x624
    char cues[10][0x10];        // 0x628
    char pad_0x6C8[0x6D0 - 0x6C8];
    char slots[2][0x120];       // 0x6D0
    char pad_0x910[0xA48 - 0x910];
    int fA48;                   // 0xA48
};

/**
 * Constructs the script engine: its bases and member objects, then empty state.
 *
 * Builds the script, video and data-manager bases (256 data instances), the two object camera managers, the
 * game-state and event objects and the animation and sound bank managers; registers itself as the engine
 * instance (D_004A34B0); no NIS list playing (2), no bank attaching (10), no anchor tables and empty banks.
 *
 * @return @p self
 */
extern "C" sScriptMgr_7F20* cSSXScriptEngine_cSSXScriptEngine(sScriptMgr_7F20* self)
{
    cBXScriptEngine_cBXScriptEngine(self);
    cVidEngine_cVidEngine((char*)self + 0x500);
    cDataManager_cDataManager((cDataManager*)((char*)self + 0x51C), 0, 0x100);
    self->vt2AC = D_00482100;
    self->vt518 = D_004821A0;
    self->vt528 = D_00482168;
    self->vt2A8 = cSSXScriptEngine_vtbl;
    new ((char*)self + 0x6D0) sScriptSlot_7F20[2];
    cScriptObjGS_cScriptObjGS((char*)self + 0x910);
    cScriptObjEvent_cScriptObjEvent((char*)self + 0x92C);
    cScriptAnimBankManager_cScriptAnimBankManager((char*)self + 0xA28, 0, 10, 0xF0);
    cScriptSoundBankManager_cScriptSoundBankManager((char*)self + 0xA38, 0, 10, 17);
    self->f52C = 0;
    self->f530 = 0;
    self->f534 = 0;
    self->f538 = 0;
    self->f53C = 0;
    self->f540 = 0;
    self->f544 = 0;
    self->f548 = 0;
    D_004A34B0 = self;
    self->fA48 = 10;
    self->f550 = 2;
    self->f54C = 0;
    self->f554 = 0;
    self->f620 = 0;
    self->f624 = 0;
    int i;
    for (i = 49; i >= 0; i--) {
        self->ids[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        tSSXScriptBankInfo_Clear(self->cues[i]);
    }
    for (i = 0; i < 2; i++) {
        cScriptObjCameraManager_SetViewIndex(self->slots[i], i);
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cSSXScriptEngine__cSSXScriptEngine);
#ifdef SKIP_ASM
extern void* cScriptObjEvent_vtbl[];
extern void* D_00482298[];
extern void* cScriptObjGS_vtbl[];
extern void* D_00482318[];
extern "C" void cSSXScriptEngine_DeInitOnce(cSSXScriptEngine* self);
extern "C" void cScriptSoundBankManager__cScriptSoundBankManager(void* self, int flags);
extern "C" void cScriptAnimBankManager__cScriptAnimBankManager(void* self, int flags);
extern "C" void cScriptObject__cScriptObject(void* self, int flags);
extern "C" void cDataManager__cDataManager(cDataManager* self, int flags);
extern "C" void cVidEngine__cVidEngine(void* self, int flags);
extern "C" void cBXScriptEngine__cBXScriptEngine(void* self, int flags);

struct sVE_780B8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sSlot_780B8 {
    char pad[0xC];
    sVE_780B8* vt;              // 0xC
    char rest[0x120 - 0x10];
};

/**
 * Destroys the script engine: runs DeInitOnce() if needed, then destroys the members and bases in reverse
 * order.
 *
 * @param flags  g++ destructor flags, passed to the base destructor; bit 0 set frees the object's memory
 */
// Script manager destructor: members in reverse order, then the base.
extern "C" void cSSXScriptEngine__cSSXScriptEngine(cSSXScriptEngine* self, int flags)
{
    self->mInSeqCommVtbl = D_00482100;
    self->mVidEngineVtbl = D_004821A0;
    self->mDataManagerVtbl = D_00482168;
    self->mVtbl = cSSXScriptEngine_vtbl;
    if ((int)self->mpMasterTableHeader != 0) {
        cSSXScriptEngine_DeInitOnce(self);
    }
    D_004A34B0 = 0;
    cScriptSoundBankManager__cScriptSoundBankManager(self->mSoundBankManager, 2);
    cScriptAnimBankManager__cScriptAnimBankManager(&self->mAnimationBankManager, 2);
    self->mObjectEvent.mScriptObjectVtbl = D_00482298;
    self->mObjectEvent.mVtbl = cScriptObjEvent_vtbl;
    cScriptObject__cScriptObject(&self->mObjectEvent.mObjType, 0);
    self->mObjectGameState.mScriptObjectVtbl = D_00482318;
    self->mObjectGameState.mVtbl = cScriptObjGS_vtbl;
    cScriptObject__cScriptObject(&self->mObjectGameState.mObjType, 0);
    sSlot_780B8* cameraManager = (sSlot_780B8*)(&self->mObjectGameState);
    sSlot_780B8* begin = (sSlot_780B8*)(&self->mObjectCameraManager[0]);
    if (begin != 0) {
        do {
            while (begin != cameraManager) {
                cameraManager--;
                cameraManager->vt[1].fn((char*)cameraManager + cameraManager->vt[1].delta, 0);
            }
        } while (0);
    }
    cDataManager__cDataManager((cDataManager*)(&self->mMaxDataInstances), 0);
    cVidEngine__cVidEngine(&self->mpPlayer, 0);
    cBXScriptEngine__cBXScriptEngine(((char*)self), flags);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cSSXScriptEngine_InitOnce);
#ifdef SKIP_ASM
extern "C" void* FILE_loadz(const char* name, int flags);
extern "C" void cBXScriptEngine_SetupBXEngine(void* self, int count, int a2, int size);
extern char* kpScriptMasterTableFilename;
extern char* kpScriptMasterTableDebugFilename;

struct sDmScript8210 {
    int field_0x0;
    int field_0x4;
    int offset;             // 0x8
};

struct sDmEngine8210 {
    char pad_0x0[0x52C];
    int* hdr;                   // 0x52C
    sDmScript8210* scripts;     // 0x530
    char* base;                 // 0x534
    char pad_0x538[0x1C];
    int field_0x554;            // 0x554
    char pad_0x558[0xC8];
    int field_0x620;            // 0x620
};

/**
 * Loads the script master table (and its debug name table) and sets up the base script engine for it.
 *
 * With the debug table, each script's name offset becomes a pointer into it; without it, the names are 0.
 * Then
 * SetupBXEngine() is called with the script count, 10 and 0x4000.
 */
// PORT: script offsets are rebased into pointers held in int.
extern "C" void cSSXScriptEngine_InitOnce(sDmEngine8210* self)
{
    int* hdr = (int*)FILE_loadz(kpScriptMasterTableFilename, 0);
    self->hdr = hdr;
    self->scripts = (sDmScript8210*)(hdr + 1);
    char* base = (char*)FILE_loadz(kpScriptMasterTableDebugFilename, 0);
    self->base = base;
    if (base != 0) {
        for (int i = 0; i < *self->hdr; i++) {
            self->scripts[i].offset = (int)self->base + self->scripts[i].offset;
        }
    } else {
        for (int i = 0; i < *self->hdr; i++) {
            self->scripts[i].offset = 0;
        }
    }
    self->field_0x554 = 0;
    self->field_0x620 = 0;
    cBXScriptEngine_SetupBXEngine(self, *self->hdr, 10, 0x4000);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cSSXScriptEngine_DeInitOnce);
#ifdef SKIP_ASM
extern "C" void cBXScriptEngine_PurgeBXEngine(void* self);
extern "C" void* MEM_free(void* self);

/**
 * Releases what the script engine's InitOnce set up: the BX script engine and the script master tables.
 *
 * Purges the BX engine, frees the master table header and, when loaded, the debug table, then clears
 * mpMasterTableHeader, mpMasterTable, mpMasterTableDebug and mNumFramesNoSkip.
 */
extern "C" void cSSXScriptEngine_DeInitOnce(cSSXScriptEngine* self)
{
    cBXScriptEngine_PurgeBXEngine(self);
    MEM_free(self->mpMasterTableHeader);
    if (self->mpMasterTableDebug != 0) {
        MEM_free(self->mpMasterTableDebug);
    }
    self->mpMasterTableHeader = 0;
    self->mpMasterTable = 0;
    self->mpMasterTableDebug = 0;
    self->mNumFramesNoSkip = 0;
}
#endif

