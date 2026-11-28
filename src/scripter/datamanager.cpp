#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "be/beintoption.h"
#include "ui/uistate.h"
#include "scripter/scriptobjcammgr.h"
#include "visualfx/transitionman.h"
#include "sound/ssxaudio.h"
#include "ui/uiengine.h"
#include "scripter/datamanager.h"
#include "scripter/nislist.h"
#include "scripter/bxscriptengine.h"
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
    self->mVtbl = (void**)cDataManager_vtbl;
    self->mMaxDataInstances = count;
    self->mpDataInstanceHeap =
        (tDataInstanceInfo*)operator_new_tag(count * sizeof(tDataInstanceInfo), str_tDataInstanceInfo, flags, 0);
    self->mpEmptyDataInstanceList = 0;
    for (int i = 0; i < count; i++) {
        self->AddToEmptyInstanceList(&(self->mpDataInstanceHeap)[i]);
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
extern "C" tDataInstanceInfo* cDataManager_AttachDataFromBigfile(cDataManager* self, char* big, tDataInstanceInfo* prev);
extern int REALabortflag[];



/**
 * Registers a block of data with the data manager and returns its instance record.
 *
 * Takes a free instance (cDataManager_GetEmptyInstance, On Tour GetEmptyInstance) and stores the data (+0x8), its size
 * (+0xC) and a reference count of 1 (+0x10). BIG_sizeofheader() then tells whether the data is a BIG archive
 * (REALabortflag is cleared around the call so a non-BIG block does not abort). A BIG file calls virtual slot 4 of the
 * manager's vtable (+0xC) and attaches its members with cDataManager_AttachDataFromBigfile; any other block calls slot
 * 2. Both callbacks get the name, data, size, the record and the record's +0x4 word.
 * cSSXScriptEngine_SyncLoadScriptBank and cSSXScriptEngine_ProcessAsyncLoadScriptBankComplete attach the script banks
 * with it.
 *
 * @param pName  the data's name, passed to the callbacks
 * @param data  the data block
 * @param size  its size in bytes
 * @return the instance record (the data handle)
 */
tDataInstanceInfo* cDataManager::AttachData(int pName, void* data, int size)
{
    tDataInstanceInfo* n = (tDataInstanceInfo*)GetEmptyInstance();
    int saved = REALabortflag[0];
    n->pData = data;
    n->isRoot = 1;
    n->dataSize = size;
    REALabortflag[0] = 0;
    int big = BIG_sizeofheader(data);
    REALabortflag[0] = saved;
    if (big != 0) {
        sVEntry<void(void*, int, void*, int, void*, void*)>* vt = (sVEntry<void(void*, int, void*, int, void*,
                void*)>*)mVtbl;
        vt[4].fn(vthis(this, vt[4]), pName, data, size, n, &n->pUserData2);
        AttachDataFromBigfile((char*)data, n);
    } else {
        sVEntry<void(void*, int, void*, int, void*, void*)>* vt = (sVEntry<void(void*, int, void*, int, void*,
                void*)>*)mVtbl;
        vt[2].fn(vthis(this, vt[2]), pName, data, size, n, &n->pUserData2);
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_DetachData);
#ifdef SKIP_ASM
void cDataManager_AddToEmptyInstanceList(void* self, void* node);


// A detach hook's vtable slot: OnDetachData (slot 3) / OnDetachDataBigFile (slot 5) get the record's pData, dataSize,
// pUserData1 and pUserData2.
typedef sVEntry<void(void*, void*, int, void*, void*)> tDetachHookSlot;

// PORT: the unit declares cDataManager_DetachData as returning void; it really returns the entry's key.

/**
 * Detaches an attached data handle and every instance linked to it, recycling their records.
 *
 * Each linked instance goes through the detach callback (virtual slot 3, OnDetachData); the head then gets slot 5
 * (OnDetachDataBigFile) when it had linked instances, otherwise slot 3.
 *
 * @param entry  the data handle returned by the attach
 * @return the detached data pointer, the handle's pData (On Tour returns void* pData)
 */
void* cDataManager::DetachData(tDataInstanceInfo* entry)
{
    tDataInstanceInfo* first = entry->pNext;
    if (first != 0) {
        tDataInstanceInfo* n = first;
        do {
            tDataInstanceInfo* next = n->pNext;
            tDetachHookSlot* vt = (tDetachHookSlot*)mVtbl;
            vt[3].fn(vthis(this, vt[3]), n->pData, n->dataSize, n->pUserData1, n->pUserData2); // OnDetachData
            AddToEmptyInstanceList(n);
            n = next;
        } while (n != 0);
        tDetachHookSlot* vt = (tDetachHookSlot*)mVtbl;
        vt[5].fn(vthis(this, vt[5]), entry->pData, entry->dataSize, entry->pUserData1, entry->pUserData2);
    } else {
        tDetachHookSlot* vt = (tDetachHookSlot*)mVtbl;
        vt[3].fn(vthis(this, vt[3]), entry->pData, entry->dataSize, entry->pUserData1, entry->pUserData2);
    }
    void* data = entry->pData;
    AddToEmptyInstanceList(entry);
    return data;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", cDataManager_DetachAll);
#ifdef SKIP_ASM
extern "C" void cDataManager_DetachData(void* self, void* entry);



/**
 * Detaches every data entry of the data manager that is in use, through cDataManager_DetachData().
 */
void cDataManager::DetachAll()
{
    int i;
    for (i = 0; i < mMaxDataInstances; i++) {
        if (mpDataInstanceHeap[i].isRoot != 0) {
            cDataManager_DetachData(this, &mpDataInstanceHeap[i]);
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
const void* cDataManager::GetData(tDataInstanceInfo* dataHandle)
{
    return dataHandle->pData;
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
void cDataManager::GetDataUserData(tDataInstanceInfo* dataHandle, void** out0, void** out1)
{
    *out0 = dataHandle->pUserData1;
    *out1 = dataHandle->pUserData2;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", cDataManager_AttachDataFromBigfile);
#ifdef SKIP_ASM
void* cDataManager_GetEmptyInstance(void* self);
extern "C" int BIG_sizeofheader(void* data);
extern "C" char* BIG_locateentryz(void* big, int a1, int index, int* offset, int* size);
extern int REALabortflag[];



/**
 * Attaches every member of a BIG archive to the data manager as a chain of instance records, recursing into nested
 * archives.
 *
 * Walks the entries with BIG_locateentryz() (entry 0, then 1, 2, ... until it returns 0). A member that is itself a BIG
 * file (BIG_sizeofheader() with REALabortflag cleared) is attached recursively; any other member gets a free instance
 * (cDataManager_GetEmptyInstance) holding its address (+0x8) and size (+0xC), is linked after the previous record
 * through +0x14 and is announced through virtual slot 2 of the manager's vtable (+0xC) with its name, data, size,
 * record and record+0x4.
 *
 * Likely bug (also in On Tour): only the first BIG_locateentryz() call passes a size pointer; the later calls pass 0,
 * so every member after the first is recorded with the first member's size.
 *
 * @param big  the BIG archive in memory
 * @param prev  the record to chain the new records after
 * @return the last record of the chain
 */
tDataInstanceInfo* cDataManager::AttachDataFromBigfile(char* big, tDataInstanceInfo* prev)
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
            prev = AttachDataFromBigfile(sub, prev);
        } else {
            tDataInstanceInfo* n = (tDataInstanceInfo*)GetEmptyInstance();
            n->pData = sub;
            n->dataSize = ent[1];
            prev->pNext = n;
            prev = n;
            sVEntry<void(void*, char*, char*, int, void*, void*)>* vt = (sVEntry<void(void*, char*, char*, int, void*,
                    void*)>*)mVtbl;
            vt[2].fn(vthis(this, vt[2]), name, sub, ent[1], prev, &prev->pUserData2);
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
 * @param vnode  the instance record (On Tour tDataInstanceInfo*)
 */
void cDataManager::AddToEmptyInstanceList(void* vnode)
{
    tDataInstanceInfo* node = (tDataInstanceInfo*)vnode;
    node->pData = 0;
    node->dataSize = 0;
    node->isRoot = 0;
    node->pUserData1 = 0;
    node->pUserData2 = 0;
    node->pNext = (tDataInstanceInfo*)((void*)mpEmptyDataInstanceList);
    mpEmptyDataInstanceList = (tDataInstanceInfo*)node;
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
void* cDataManager::GetEmptyInstance()
{
    tDataInstanceInfo* head = mpEmptyDataInstanceList;
    mpEmptyDataInstanceList = (tDataInstanceInfo*)(head->pNext);
    head->pNext = 0;
    return ((void*)head);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_tNISList);
#ifdef SKIP_ASM
extern "C" void __15cCircularBuffer(void* self);
void cCircularBuffer_Reset(void* self);

/**
 * Constructs an empty NIS list.
 *
 * Initialises the entry buffer at +0xC (cCircularBuffer::cCircularBuffer, cCircularBuffer_Reset), sets the next pending
 * index (+0xBC) to -1 and the cut-scene flag (+0x4) to 1, and zeroes the engine pointer (+0x0), view index (+0x8),
 * state (+0xA4) and the flag and counter words at +0xA8-+0xC8.
 *
 * @return @p self
 */
extern "C" void* tNISList_tNISList(tNISList* self)
{
    void* list = (char*)&self->mList;
    __15cCircularBuffer(list);
    cCircularBuffer_Reset(list);
    self->mNextPendingScriptIndex = -1;
    self->mIsCutSceneHandler = 1;
    self->mState = keNISListState_Idle;
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
INCLUDE_ASM("scripter/datamanager", _$_8tNISList);
#ifdef SKIP_ASM
extern "C" void tNISList_Reset(tNISList* self, int a1);
void operator_delete(int* ptr);

/**
 * Destroys a NIS list: resets it (tNISList_Reset with resetTransition 1), clears its engine pointer (+0x0), and frees
 * it with operator_delete when bit 0 of g++'s delete flag is set.
 */
tNISList::~tNISList()
{
    ((tNISList*)this)->Reset(1);
    *((int*)this) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_AddByVideoCategory);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_WaitOnPendingNextScript(tNISList* self);
extern "C" cScriptPlayerMain* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
int cCircularBuffer_IsFull(void* self);
extern "C" bool cCircularBuffer_AddToTail(void* list, void* cmd);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
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
 * Appends a video entry of category @p videoCategory to the NIS list, then sets up the next script if the list is
 * running.
 *
 * Returns false when the five-entry buffer is full (cCircularBuffer_IsFull). The entry (category, script index -1,
 * control bits @p controlBits, no handle, AI rider indexes -1) is pushed with cCircularBuffer_AddToTail. When the list
 * is active (state +0xA4 nonzero) and nothing is pending yet (+0xBC < 0), the new entry is set up as the pending one
 * (tNISList_SetupNextScript), and if the list is playing (keNISListState_Playing) while the head's script player
 * (cBXScriptEngine_GetRequestPlayerByReqHandle) is ending (mPlayState 4), tNISList_WaitOnPendingNextScript checks
 * whether to wait for it.
 *
 * Quirk: unlike tNISList_AddByScriptCategory and tNISList_AddByScriptIndex it does not test whether the head entry is a
 * video before asking for its script player.
 *
 * @param videoCategory  the video category
 * @param controlBits  the entry's control bits
 * @return true when queued, false when the list is full
 */
bool tNISList::AddByVideoCategory(int videoCategory, int controlBits)
{
    if (cCircularBuffer_IsFull(&mList) != 0) {
        return false;
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
    void* list = (char*)&mList;
    cCircularBuffer_AddToTail(list, &cmd);
    if (mState == keNISListState_Idle) {
        return true;
    }
    if (mNextPendingScriptIndex >= none) {
        return true;
    }
    mNextPendingScriptIndex = SetupNextScript(cCircularBuffer_GetNumEntries_count(list) - 1, 0);
    if (mState == keNISListState_Playing) {
        int ok = cBXScriptEngine_GetRequestPlayerByReqHandle(mpNISEngine,
            cCircularBuffer_operatorIndex(list, 0)->scriptHandle)->mPlayState == keScriptPlayState_PlayEnding;
        if (ok)
            WaitOnPendingNextScript();
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_AddByScriptCategory);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_WaitOnPendingNextScript(tNISList* self);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cSSXScriptEngine_SelectScriptFromCategory(void* eng, int a1, int a2, int a3, int a4);
extern "C" cScriptPlayerMain* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" int cVidEngine_IsVideoOnLastFrame(void* self);
int cCircularBuffer_IsFull(void* self);
extern "C" bool cCircularBuffer_AddToTail(void* list, void* cmd);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
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
 * Appends a script entry of category @p scriptCategory to the NIS list, then sets up the next script if the list is
 * running.
 *
 * Returns false when the buffer is full (cCircularBuffer_IsFull). With @p lookup set, the script is chosen now
 * (cSSXScriptEngine_SelectScriptFromCategory with the three AI rider indexes) and a failed choice returns false;
 * otherwise the entry keeps script index -1 for tNISList_SetupNextScript to resolve. After the push
 * (cCircularBuffer_AddToTail), an active list with nothing pending sets the new entry up as pending; while playing, if
 * the head (a video: cVidEngine_IsVideoOnLastFrame on the video engine; a script: its player in mPlayState 4) is
 * ending, tNISList_WaitOnPendingNextScript checks whether to wait for it.
 *
 * @param scriptCategory  the script category
 * @param controlBits  the entry's control bits
 * @param lookup  nonzero to choose the script now
 * @param aiCharIndexMain  the main AI rider index
 * @param aiCharIndexSupport0  the first supporting AI rider index
 * @param aiCharIndexSupport1  the second supporting AI rider index
 * @return true when queued, false when full or no script was found
 */
bool tNISList::AddByScriptCategory(int scriptCategory, int controlBits, int lookup, int aiCharIndexMain,
                                  int aiCharIndexSupport0, int aiCharIndexSupport1)
{
    if (cCircularBuffer_IsFull(&mList) != 0) {
        return false;
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
        int id = cSSXScriptEngine_SelectScriptFromCategory(*(void**)this, scriptCategory, aiCharIndexMain,
                aiCharIndexSupport0, aiCharIndexSupport1);
        if (id < 0) {
            return false;
        }
        cmd.id = id;
    }
    void* list = (char*)&mList;
    cCircularBuffer_AddToTail(list, &cmd);
    if (mState == keNISListState_Idle) {
        return true;
    }
    if (mNextPendingScriptIndex >= 0) {
        return true;
    }
    mNextPendingScriptIndex = SetupNextScript(cCircularBuffer_GetNumEntries_count(list) - 1, 0);
    if (mState == keNISListState_Playing) {
        int ok;
        if (IsVideo(cCircularBuffer_operatorIndex(list, 0)) == 0) {
            ok = cBXScriptEngine_GetRequestPlayerByReqHandle(mpNISEngine,
                cCircularBuffer_operatorIndex(list, 0)->scriptHandle)->mPlayState == keScriptPlayState_PlayEnding;
        } else {
            ok = cVidEngine_IsVideoOnLastFrame(&mpNISEngine->mpPlayer);
        }
        if (ok) {
            WaitOnPendingNextScript();
        }
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_AddByScriptIndex);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_WaitOnPendingNextScript(tNISList* self);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" cScriptPlayerMain* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" int cVidEngine_IsVideoOnLastFrame(void* self);
int cCircularBuffer_IsFull(void* self);
extern "C" bool cCircularBuffer_AddToTail(void* list, void* cmd);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");


/**
 * Appends the script with master-table index @p scriptIndex to the NIS list, then sets up the next script if the list
 * is running.
 *
 * Returns false when the buffer is full (cCircularBuffer_IsFull). The entry has category 0x1C (no category), script
 * index @p scriptIndex, no handle and the given control bits and AI rider indexes. After the push the follow-up matches
 * tNISList_AddByScriptCategory: an active list with nothing pending sets the new entry up, and an ending head while
 * playing triggers tNISList_WaitOnPendingNextScript.
 *
 * @param scriptIndex  the script's master-table index
 * @param controlBits  the entry's control bits
 * @param aiCharIndexMain  the main AI rider index
 * @param aiCharIndexSupport0  the first supporting AI rider index
 * @param aiCharIndexSupport1  the second supporting AI rider index
 * @return true when queued, false when the list is full
 */
bool tNISList::AddByScriptIndex(int scriptIndex, int controlBits, int aiCharIndexMain, int aiCharIndexSupport0,
                               int aiCharIndexSupport1)
{
    if (cCircularBuffer_IsFull(&mList) == 0) {
        void* list = (char*)&mList;
        tNISListEntry cmd;
        cmd.category = 0x1C;
        cmd.scriptHandle = -1;
        cmd.controlBits = controlBits;
        cmd.scriptIndex = scriptIndex;
        cmd.aiCharIndexMain = aiCharIndexMain;
        cmd.aiCharIndexSupport0 = aiCharIndexSupport0;
        cmd.aiCharIndexSupport1 = aiCharIndexSupport1;
        cCircularBuffer_AddToTail(list, &cmd);
        if (mState != keNISListState_Idle && mNextPendingScriptIndex < 0) {
        mNextPendingScriptIndex = SetupNextScript(cCircularBuffer_GetNumEntries_count(list) - 1, 0);
        if (mState == keNISListState_Playing) {
            int ok;
            if (IsVideo(cCircularBuffer_operatorIndex(list, 0)) == 0) {
                ok = cBXScriptEngine_GetRequestPlayerByReqHandle(mpNISEngine,
                    cCircularBuffer_operatorIndex(list, 0)->scriptHandle)->mPlayState == keScriptPlayState_PlayEnding;
            } else {
                ok = cVidEngine_IsVideoOnLastFrame(&mpNISEngine->mpPlayer);
            }
            if (ok) {
                WaitOnPendingNextScript();
            }
        }
        }
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_TransferFromNISList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" bool cCircularBuffer_AddToTail(void* list, void* cmd);
extern "C" bool cCircularBuffer_RemoveFromHead(void* self, void* out);
extern "C" void tNISList_Reset(tNISList* self, int a1);
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

/**
 * Moves all entries of NIS list @p other to the end of this list and resets @p other.
 *
 * Refuses (returns false) while @p other is playing (keNISListState_Playing). Entries are popped from @p other
 * (cCircularBuffer_RemoveFromHead) and pushed here (cCircularBuffer_AddToTail); @p other is then reset (tNISList_Reset
 * with resetTransition 1). When this list is active with nothing pending, the first moved entry is set up as pending
 * (tNISList_SetupNextScript from the old length).
 *
 * @param other  the source NIS list
 * @return true, or false when @p other is playing
 */
bool tNISList::TransferFromNISList(tNISList* other)
{
    int cmd[8];
    int n = cCircularBuffer_GetNumEntries_count((char*)&mList);
    if (other->mState == keNISListState_Playing) {
        return false;
    }
    if (cCircularBuffer_GetNumEntries_count(&other->mList) != 0) {
        while (cCircularBuffer_RemoveFromHead(&other->mList, cmd) != 0) {
            cCircularBuffer_AddToTail((char*)&mList, cmd);
        }
        other->Reset(1);
        if (mState != keNISListState_Idle && mNextPendingScriptIndex < 0) {
            mNextPendingScriptIndex = SetupNextScript(n, 0);
        }
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_ReadyList);
#ifdef SKIP_ASM
extern "C" void tNISList_Reset(tNISList* self, int a1);
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_RemoveFromHead(tNISList* self, int index);
extern "C" void tNISList_CheckOnLoading(tNISList* self);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cBXScriptEngine_GetRequestState(void* self, int a1);
extern "C" void* cCircularBuffer_GetNumEntries(void*);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);

/**
 * Readies an idle NIS list for playing and returns its state.
 *
 * A loading list (keNISListState_Loading) is polled first (tNISList_CheckOnLoading); a list that is not idle returns
 * its state. Otherwise stores @p playOnReady (+0xB4), returns 0 for an empty list, and finds the first entry that can
 * play (tNISList_SetupNextScript from 0); with none the list is reset (tNISList_Reset). Entries before it are dropped
 * (tNISList_RemoveFromHead), and the state becomes keNISListState_Loading while the head's script request waits for
 * data (keScriptRequestState_DataPending) or its video has no handle, else keNISListState_Ready. The following entry is
 * set up as the pending one (+0xBC).
 *
 * Likely bug: the pending search starts at the old index + 1 although that many head entries were just removed, so it
 * skips entries whenever the first playable one was not at index 0.
 *
 * @param playOnReady  when to play once ready (tNISList::ePlayOnReady)
 * @return the list state (+0xA4)
 */
int tNISList::ReadyList(int playOnReady)
{
    if (mState == keNISListState_Loading) {
        CheckOnLoading();
    }
    if (mState != keNISListState_Idle) {
        return mState;
    }
    mPlayOnReady = playOnReady;
    void* list = (char*)&mList;
    if (cCircularBuffer_GetNumEntries(list) == 0) {
        return 0;
    }
    int idx = SetupNextScript(0, 0);
    if (idx < 0) {
        Reset(1);
        return mState;
    }
    RemoveFromHead(idx);
    tNISListEntry* entry = (tNISListEntry*)cCircularBuffer_operatorIndex(list, 0);
    if (IsVideo(entry) == 0) {
        if (cBXScriptEngine_GetRequestState(*(void**)this, entry->scriptHandle) == keScriptRequestState_DataPending) {
            mState = keNISListState_Loading;
        } else {
            mState = keNISListState_Ready;
        }
    } else {
        if (entry->scriptHandle < 0) {
            mState = keNISListState_Loading;
        } else {
            mState = keNISListState_Ready;
        }
    }
    mNextPendingScriptIndex = SetupNextScript(idx + 1, 0);
    return mState;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_Play);
#ifdef SKIP_ASM
extern char str_hud_e[];
int GetHashValue32(char* s);
cTransitionMan* cGame_getTransitionMan(void* self, int a1);
char* GetSSXScriptHeader(void* obj);
extern "C" void tNISList_CheckOnLoading(tNISList* self);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
// The unit declares cSSXScriptEngine_PlayRequestScript as void; it returns cBXScriptEngine_PlayScript's result.
int cSSXScriptEngine_PlayRequestScript_i(void* self, int val, int key, int b, int c) __asm__("cSSXScriptEngine_PlayRequestScript");
extern "C" int cSSXScriptEngine_MakeTransitionObject(void* self, int a1, int a2);
extern "C" void cBXScriptEngine_PauseScript(void* self, int a1);
extern "C" cScriptPlayerMain* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" bool cVidEngine_PlayVideo(void* self);
extern "C" void cVidEngine_UpdateVideo(void* self);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" void cTransitionMan_StartTransition(void* snd, int a1, int h, int h2, int a4, int a5, float t0, float t1, float t2);
// PORT: real parameter order of cTransitionMan_StartTransition has the floats before the two trailing ints (cf. boardwakefx).
void cTransitionMan_StartTransition_f(void* snd, int a1, int h, int h2, float t0, float t1, float t2, int a4, int a5) __asm__("cTransitionMan_StartTransition");
extern "C" void cTransitionMan_StartTransitionScreenGrab(void* snd, int a1, int h, int a3, float t);
extern "C" void cTransitionMan_EndTransition(void* p);
extern "C" void cCutSceneTransitionMan_StartCutScene(void* gm, float t);
extern "C" void cCutSceneTransitionMan_ForceToCutSceneMode(void* gm);
extern "C" cUIState* cUIStateStack_findState(void* table, int hash);


// The engine's vtable slot 6 (On Tour ForcePlayUpdate(int requestHandle)).
typedef sVEntry<int(void*, int)> tForcePlayUpdateSlot;

// The unit declares tNISList_Play as void; it returns the state at 0xA4.

/**
 * Starts the NIS list's head script (or video) once it is ready, with its transition in, and returns the list state.
 *
 * Polls a loading list first (keNISListState_Loading: tNISList_CheckOnLoading, On Tour CheckOnLoading) and does nothing
 * unless the state (+0xA4) is keNISListState_Ready. It sets the state to keNISListState_Playing and calls virtual slot
 * 24 of the "hud" UI state (cUIStateStack_findState) with (0, 0) when that state is up.
 *
 * A script entry (tNISList_IsVideo, On Tour IsVideo, returns 0) is started with cSSXScriptEngine_PlayRequestScript (the
 * last flag is bit 3 of the entry's flags, +0x8). On the cut-scene list (+0x4) the running transition is ended
 * (cTransitionMan_EndTransition), and the script header's in/hold/out frame counts (GetSSXScriptHeader of the request's
 * player; 1/60 s each) build the transition in: keScriptTransition_CrossFade is a screen grab
 * (cTransitionMan_StartTransitionScreenGrab), any other type a full transition (cTransitionMan_StartTransition with two
 * cSSXScriptEngine_MakeTransitionObject objects). When in+hold frames remain (+0xB8) the request is paused
 * (cBXScriptEngine_PauseScript, On Tour PauseScript), +0xAC and +0xB0 (On Tour mIsStartTransition, mInScriptTransition)
 * are set and the forced update is skipped; then the cut scene starts (cCutSceneTransitionMan_StartCutScene, On Tour
 * cCutSceneTransitionMan::StartCutScene, over the transition-in time or 0.5 s). With the forced update the engine's
 * virtual slot 6 (On Tour ForcePlayUpdate) runs the request once.
 *
 * A video entry forces cut-scene mode on the cut-scene list (cCutSceneTransitionMan_ForceToCutSceneMode), starts the
 * engine's video engine (cVidEngine_PlayVideo on &mpNISEngine->mpPlayer) and, with the forced update, updates it once
 * (cVidEngine_UpdateVideo).
 *
 * @param fade  start with the transition in (On Tour startWithTransitionIn); 0 drops its in and hold frames
 * @param notify  force an immediate update of the started script or video (On Tour forceUpdate)
 * @return the list state after the call (+0xA4)
 */
int tNISList::Play(int fade, int notify)
{
    tNISList* dm = (tNISList*)this;
    if (mState == keNISListState_Loading) {
        CheckOnLoading();
    }
    if (mState != keNISListState_Ready) {
        return mState;
    }
    tNISListEntry* item = (tNISListEntry*)cCircularBuffer_operatorIndex(&mList.mBuffer[0], 0);
    dm->mState = keNISListState_Playing;
    char* table = (char*)&SSXApp->Game->UI->mStateStack;
    cUIState* hudState = cUIStateStack_findState(table, GetHashValue32(str_hud_e));
    if (hudState != 0) {
        hudState->handleEvent(0, 0);
    }
    if (IsVideo(item) == 0) {
        cSSXScriptEngine_PlayRequestScript_i(*(void**)this, mViewIndex, item->scriptHandle, 0,
                (item->controlBits >> 3) & 1);
        if (mIsCutSceneHandler != 0) {
            cTransitionMan* snd = cGame_getTransitionMan(SSXApp->Game, 0);
            if (snd->mbPlayingTransition != 0) {
                cTransitionMan_EndTransition(snd);
            }
            int tIn = 0;
            int tHold = 0;
            float cutSceneTime = 0.0f;
            tSSXScriptHeader* transitionInfo = (tSSXScriptHeader*)GetSSXScriptHeader(
                cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)this, item->scriptHandle));
            if (fade) {
                tIn = transitionInfo->startFadeToTransitionFrames;
                tHold = transitionInfo->startHoldTransitionFrames;
            }
            mTransitionWaitFrames = 0;
            if (tIn + tHold + transitionInfo->startFadeFromTransitionFrames > 0) {
                if (transitionInfo->startTransitionType != keScriptTransition_CrossFade) {
                    int transition = cSSXScriptEngine_MakeTransitionObject(*(void**)this,
                        transitionInfo->startTransitionType, transitionInfo->startTransitionColour);
                    if (transition != 0) {
                        int transitionOut = cSSXScriptEngine_MakeTransitionObject(*(void**)this,
                            transitionInfo->startTransitionType, transitionInfo->startTransitionColour);
                        float tt = (float)tIn * 0.01666666753590107f;
                        cTransitionMan_StartTransition_f(snd, 2, transition, transitionOut, tt,
                            (float)tHold * 0.01666666753590107f,
                            (float)transitionInfo->startFadeFromTransitionFrames * 0.01666666753590107f, 0, 0);
                        cutSceneTime = tt;
                        mTransitionWaitFrames = tIn + tHold;
                    }
                } else if (tIn) {
                    int transition = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, keScriptTransition_CrossFade,
                        transitionInfo->startTransitionColour);
                    float tt = (float)tIn * 0.01666666753590107f;
                    cTransitionMan_StartTransitionScreenGrab(snd, 2, transition, 0, tt);
                    cutSceneTime = tt;
                }
                if (mTransitionWaitFrames > 0) {
                    notify = 0;
                    cBXScriptEngine_PauseScript(*(void**)this, item->scriptHandle);
                    mIsStartTransition = 1;
                    mInScriptTransition = 1;
                }
            }
            if (cutSceneTime <= 0.0f) {
                cutSceneTime = 0.5f;
            }
            cCutSceneTransitionMan_StartCutScene(SSXApp->Game->CutSceneTransitionMan, cutSceneTime);
        }
        if (notify) {
            cSSXScriptEngine* eng = mpNISEngine;
            tForcePlayUpdateSlot* vt = (tForcePlayUpdateSlot*)eng->mVtbl;
            tForcePlayUpdateSlot* vtEntry = &vt[6];
            vtEntry->fn(vthis(eng, *vtEntry), item->scriptHandle);
        }
    } else {
        if (mIsCutSceneHandler != 0) {
            cCutSceneTransitionMan_ForceToCutSceneMode(SSXApp->Game->CutSceneTransitionMan);
        }
        cVidEngine_PlayVideo(&mpNISEngine->mpPlayer);
        if (notify) {
            cVidEngine_UpdateVideo(&mpNISEngine->mpPlayer);
        }
    }
    return mState;
}
#endif

extern "C" void* tNISList_DoPause(void*, int, int);

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_Pause__FPvi);
#ifdef SKIP_ASM
/**
 * Pauses or resumes a NIS list: tNISList_DoPause(this, @p pauseList, 0).
 *
 * @param pauseList  nonzero to pause, 0 to resume
 * @return tNISList_DoPause's return register (it returns nothing)
 */
void* tNISList::Pause(int pauseList)
{
    return tNISList_DoPause(this, pauseList, 0);
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
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cBXScriptEngine_ReleaseRequest(void* self, int id);
extern "C" void cVidEngine_ResetVideoEngine(void* p);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");



/**
 * Skips @p delta scripts of a NIS list and returns its state.
 *
 * The target is @p delta past the skip position (+0xC0), at least the pending index (+0xBC); an unchanged target
 * returns at once. A list that is not playing drops the skipped entries (tNISList_RemoveFromHead), goes idle and, if it
 * was active, is readied again (tNISList_ReadyList with its play-on-ready mode, +0xB4); skipping past the end resets it
 * (tNISList_Reset). A playing list stores the target; a pending entry before the target is released (script:
 * cBXScriptEngine_ReleaseRequest; video: cVidEngine_ResetVideoEngine) and pausing on loading is undone
 * (tNISList_DoPause(self, 0, 1)) before the entry at the target is set up as pending. Unless it is paused on loading
 * (+0xA8), the next script starts (tNISList_PlayNextScriptOnAvailable) when @p force is 0 or
 * tNISList_DoTransitionToNext declines; with nothing pending the list is reset.
 *
 * Likely bug: a pending video is reset only when its handle is nonzero, but a readied video has handle 0
 * (tNISList_SetupNextScript), so the readied one is left running; tNISList_DoSkip and tNISList_RemoveFromHead test for
 * >= 0.
 *
 * @param delta  the number of scripts to skip
 * @param force  nonzero to try a transition to the next script (On Tour doTransition)
 * @return the list state (+0xA4)
 */
int tNISList::SkipForward(int delta, int force)
{
    int pos = delta + mNumScriptsToSkipInProgress;
    if (pos == mNumScriptsToSkipInProgress) {
        return mState;
    }
    if (pos < mNextPendingScriptIndex) {
        pos = mNextPendingScriptIndex;
    }
    int n = cCircularBuffer_GetNumEntries_count(((char*)&mList));
    if (mState != keNISListState_Playing) {
        mNumScriptsToSkipInProgress = 0;
        if (pos < n) {
            RemoveFromHead(pos);
            int st = mState;
            mNextPendingScriptIndex = -1;
            mState = keNISListState_Idle;
            if (st != 0) {
                ReadyList(mPlayOnReady);
            }
            return mState;
        }
        goto reset;
    }
    mNumScriptsToSkipInProgress = pos;
    if (mNextPendingScriptIndex > 0 && mNextPendingScriptIndex < pos) {
        tNISListEntry* it = (tNISListEntry*)cCircularBuffer_operatorIndex(((char*)&mList), mNextPendingScriptIndex);
        if (IsVideo(it) == 0) {
            cBXScriptEngine_ReleaseRequest((char*)mpNISEngine, it->scriptHandle);
        } else if (it->scriptHandle != 0) {
            cVidEngine_ResetVideoEngine(&mpNISEngine->mpPlayer);
        }
        it->scriptHandle = -1;
        mNextPendingScriptIndex = -1;
        if (mPausedOnNextScriptLoading) {
            tNISList_DoPause((this), 0, 1);
        }
        mNextPendingScriptIndex = SetupNextScript(pos, 0);
    }
    if (mPausedOnNextScriptLoading == 0) {
        if (force == 0 || tNISList_DoTransitionToNext_r((this)) == 0) {
            if (mNextPendingScriptIndex < 0) {
                goto reset;
            }
            PlayNextScriptOnAvailable();
        }
    }
    return mState;
reset:
    Reset(1);
    return mState;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_Reset);
#ifdef SKIP_ASM
extern char str_hud_e[];
int GetHashValue32(char* s);
cTransitionMan* cGame_getTransitionMan(void* self, int a1);
extern "C" void cTransitionMan_EndTransition(void* p);
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(void* p);
extern "C" cUIState* cUIStateStack_findState(void* table, int hash);
extern "C" void tNISList_RemoveFromHead(tNISList* self, int index);
extern "C" void* tNISList_DoPause(void*, int, int);


/**
 * Empties a NIS list and returns it to idle, undoing its pauses and, on the cut-scene list, its cut-scene mode.
 *
 * Undoes a pause on loading (tNISList_DoPause(self, 0, 1)) and any counted pauses (+0xC4 forced to 1, then
 * tNISList_DoPause(self, 0, 0)). When the cut-scene list (+0x4) is playing: with @p resetTransition, puts the game's
 * cut-scene transition manager back in regular mode (cCutSceneTransitionMan_ForceToRegularMode) and ends a running
 * script transition (+0xB0) with cTransitionMan_EndTransition; then calls virtual slot 24 of the "hud" UI state
 * (cUIStateStack_findState) with (1, 0). Last it drops all five entries (tNISList_RemoveFromHead) and clears the
 * pending index (-1), state and the flag and counter words at +0xAC-+0xC8.
 *
 * @param resetTransition  nonzero to also leave cut-scene mode and end the running transition
 */
void tNISList::Reset(int resetTransition)
{
    if (mPausedOnNextScriptLoading != 0) {
        tNISList_DoPause(((char*)this), 0, 1);
    }
    if (mNumPause != 0) {
        mNumPause = 1;
        tNISList_DoPause(((char*)this), 0, 0);
    }
    if (mIsCutSceneHandler != 0 && mState == keNISListState_Playing) {
        void* gm = SSXApp->Game->CutSceneTransitionMan;
        if (resetTransition != 0) {
            cCutSceneTransitionMan_ForceToRegularMode(gm);
            if (mInScriptTransition != 0) {
                cTransitionMan_EndTransition(cGame_getTransitionMan(SSXApp->Game, 0));
            }
        }
        char* table = (char*)&SSXApp->Game->UI->mStateStack;
        cUIState* hudState = cUIStateStack_findState(table, GetHashValue32(str_hud_e));
        if (hudState != 0) {
            hudState->handleEvent(1, 0);
        }
    }
    RemoveFromHead(5);
    mNextPendingScriptIndex = -1;
    mState = keNISListState_Idle;
    mDoSkip = 0;
    mInScriptTransition = 0;
    mTransitionWaitFrames = 0;
    mIsStartTransition = 0;
    mNumScriptsToSkipInProgress = 0;
    mNumPause = 0;
    mPlayOnReady = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetScriptCategory);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

/**
 * Returns the script category of entry @p i of a NIS list, or 0x1C (no script category) when @p i is past the end or
 * the entry is a video (category 29 and up).
 *
 * @param i  the entry index
 * @return the script category
 */
int tNISList::GetScriptCategory(int i)
{
    void* list = (char*)&mList;
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
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

/**
 * Returns the video category of entry @p i of a NIS list, or 0x20 (no video category) when @p i is past the end or the
 * entry is a script (category below 29).
 *
 * @param i  the entry index
 * @return the video category
 */
int tNISList::GetVideoCategory(int i)
{
    void* list = (char*)&mList;
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
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
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
int tNISList::GetScriptIndex(int i)
{
    void* list = (char*)&mList;
    if (i >= cCircularBuffer_GetNumEntries_count(list)) {
        return -1;
    }
    tNISListEntry* item = (tNISListEntry*)cCircularBuffer_operatorIndex(list, i);
    if (IsVideo(item) != 0) {
        return -1;
    }
    int scriptIndex = item->scriptIndex;
    if (scriptIndex < 0) {
        int id = item->scriptHandle;
        scriptIndex = -1;
        if (id >= 0) {
            scriptIndex = cBXScriptEngine_GetRequestScriptIndex(*(void**)this, id);
        }
    }
    return scriptIndex;
}
#endif
//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetScriptPlayer);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
// The unit declares cCircularBuffer_GetNumEntries as returning void*, but it returns the list's count.
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");

/**
 * Returns the InSeq script player of entry @p i of a NIS list (cBXScriptEngine_GetRequestPlayerByReqHandle on the
 * entry's request handle, +0xC), or 0 past the end, for a video entry or when no request exists.
 *
 * @param i  the entry index
 * @return the player, or 0
 */
void* tNISList::GetScriptPlayer(int i)
{
    void* list = (char*)&mList;
    if (i < cCircularBuffer_GetNumEntries_count(list)) {
        tNISListEntry* item = (tNISListEntry*)cCircularBuffer_operatorIndex(list, i);
        if (IsVideo(item) == 0) {
            int id = item->scriptHandle;
            if (id >= 0) {
                return cBXScriptEngine_GetRequestPlayerByReqHandle(*(void**)this, id);
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
 * In keNISListState_Loading it calls tNISList_CheckOnLoading (On Tour CheckOnLoading), which may advance the state. The
 * state word is +0xA4 (On Tour mState; tNISList_Play treats 2 as ready and 3 as playing).
 *
 * @return the list state (+0xA4)
 */
int tNISList::GetState()
{
    if (mState == keNISListState_Loading) {
        CheckOnLoading();
    }
    return mState;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetPlayState);
#ifdef SKIP_ASM
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int cBXScriptEngine_IsRequestPlaybackEnding(void* self, int a1);

/**
 * Classifies a NIS list's playback for the script engine (On Tour eNISListStatePlay).
 *
 * Returns keNISListStatePlay_Stopped unless the list is playing (+0xA4 == keNISListState_Playing) and
 * keNISListStatePlay_Paused while it is paused (+0xC4, On Tour mNumPause). Unless a skip is in progress (+0xC8, On Tour
 * mDoSkip), a head entry with flag bit 3 (controlBits) returns keNISListStatePlay_PlayLoop while a script transition is
 * running (+0xB0) with more than one wait frame left (+0xB8), and any other head entry returns
 * keNISListStatePlay_PlayOnce while cBXScriptEngine_IsRequestPlaybackEnding (On Tour IsRequestPlaybackEnding) returns 0
 * for its request (scriptHandle). Otherwise it returns keNISListStatePlay_GoingToNext, or
 * keNISListStatePlay_GoingToStop when no next script is pending (+0xBC < 0, On Tour mNextPendingScriptIndex).
 *
 * @return the play state (eNISListStatePlay)
 */
int tNISList::GetPlayState()
{
    if (mState != keNISListState_Playing) return keNISListStatePlay_Stopped;
    if (mNumPause != 0) return keNISListStatePlay_Paused;
    if (mDoSkip == 0) {
        void* list = (char*)&mList;
        if (cCircularBuffer_operatorIndex(list, 0)->controlBits & 8) {
            if (mInScriptTransition == 0) return keNISListStatePlay_PlayLoop;
            if (mTransitionWaitFrames - 1 > 0) return keNISListStatePlay_PlayLoop;
        } else {
            if (cBXScriptEngine_IsRequestPlaybackEnding(*(void**)this,
                    cCircularBuffer_operatorIndex(list, 0)->scriptHandle) == 0) return keNISListStatePlay_PlayOnce;
        }
    }
    return mNextPendingScriptIndex < 0 ? keNISListStatePlay_GoingToStop : keNISListStatePlay_GoingToNext;
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
void* tNISList_GetLength(void* vvself)
{
    tNISList* vself = (tNISList*)vvself;
    tNISList* self = (tNISList*)vself;
    return cCircularBuffer_GetNumEntries(&self->mList.mBuffer[0]);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_Update);
#ifdef SKIP_ASM
cTransitionMan* cGame_getTransitionMan(void* self, int a1);
extern "C" void tNISList_Play(void* self, int a1, int a2);
extern "C" void tNISList_CheckOnLoading(tNISList* self);
extern "C" void tNISList_PlayNextScriptOnAvailable(tNISList* self);
extern "C" void tNISList_PlayNextScript(tNISList* self);
extern "C" void tNISList_DoTransitionToNext(void* self);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" void cBXScriptEngine_ResumeScript(void* self, int a1);
extern "C" int cBXScriptEngine_GetRequestState(void* self, int a1);
extern "C" int cBXScriptEngine_IsRequestPlaybackEnding(void* self, int a1);
extern "C" bool cVidEngine_PlayVideo(void* self);
extern "C" int cVidEngine_IsVideoOnLastFrame(void* self);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" void cTransitionMan_EndTransition(void* p);

/**
 * Advances a NIS list by one frame.
 *
 * A loading list (keNISListState_Loading) only polls (tNISList_CheckOnLoading). A ready list (keNISListState_Ready)
 * with a play-on-ready mode (+0xB4) starts (tNISList_Play, with the transition in for
 * kePlayOnReady_YesWithTransitionIn) and clears the mode. The rest runs only while playing (keNISListState_Playing)
 * and not pause-counted (+0xC4): - During a script transition (+0xB0) the transition wait (+0xB8) counts down; at 0 the
 * next script starts (tNISList_PlayNextScriptOnAvailable) unless the start-transition flag (+0xAC) is set, in which
 * case the head resumes (cBXScriptEngine_ResumeScript or cVidEngine_PlayVideo). Once the game's transition manager
 * (cGame_getTransitionMan, mbPlayingTransition) is idle and the wait is over, both flags clear. - While paused on
 * loading (+0xA8), once the pending entry (+0xBC) is ready the pause is undone (tNISList_DoPause(this, 0, 1)) and
 * tNISList_PlayNextScript plays it. - A skip in progress (+0xC8) counts down; at 0 it ends a running transition
 * (cTransitionMan_EndTransition) and starts the next script. - Otherwise the next script starts once the head script's
 * request is back to keScriptRequestState_Ready or ending (cBXScriptEngine_IsRequestPlaybackEnding), or the head video
 * reports done (cVidEngine_IsVideoOnLastFrame), after tNISList_DoTransitionToNext.
 */
void tNISList::Update()
{
    if (mState == keNISListState_Loading) {
        CheckOnLoading();
        return;
    }
    if (mState == keNISListState_Ready) {
        int playOnReady = mPlayOnReady;
        if (playOnReady != tNISList::kePlayOnReady_No) {
            tNISList_Play(((char*)this), playOnReady == tNISList::kePlayOnReady_YesWithTransitionIn, 0);
            mPlayOnReady = tNISList::kePlayOnReady_No;
        }
    }
    if (mState != keNISListState_Playing) {
        return;
    }
    if (mNumPause != 0) {
        return;
    }
    if (mInScriptTransition != 0) {
        tNISListEntry* item = (tNISListEntry*)cCircularBuffer_operatorIndex(&mList, 0);
        cTransitionMan* obj = cGame_getTransitionMan(SSXApp->Game, 0);
        if (mTransitionWaitFrames != 0) {
            if (--mTransitionWaitFrames == 0) {
                if (mIsStartTransition == 0) {
                    PlayNextScriptOnAvailable();
                    return;
                }
                if (IsVideo(item) == 0) {
                    cBXScriptEngine_ResumeScript(*(void**)this, item->scriptHandle);
                } else {
                    cVidEngine_PlayVideo(&mpNISEngine->mpPlayer);
                }
            }
        }
        if (obj->mbPlayingTransition == 0 && mTransitionWaitFrames == 0) {
            mIsStartTransition = 0;
            mInScriptTransition = 0;
        }
    }
    if (mPausedOnNextScriptLoading != 0) {
        void* list = (char*)&mList;
        tNISListEntry* item = (tNISListEntry*)cCircularBuffer_operatorIndex(list, mNextPendingScriptIndex);
        int ok;
        if (IsVideo(item) == 0) {
            ok = cBXScriptEngine_GetRequestState(*(void**)this,
                     cCircularBuffer_operatorIndex(list, mNextPendingScriptIndex)->scriptHandle)
                 == keScriptRequestState_Ready;
        } else {
            ok = item->scriptHandle >= 0;
        }
        if (ok) {
            tNISList_DoPause(((char*)this), 0, 1);
            PlayNextScript();
        }
        return;
    }
    if (mDoSkip != 0) {
        if (--mDoSkip == 0) {
            if (mInScriptTransition != 0) {
                cTransitionMan_EndTransition(cGame_getTransitionMan(SSXApp->Game, 0));
                mIsStartTransition = 0;
                mInScriptTransition = 0;
                mTransitionWaitFrames = 0;
            }
            PlayNextScriptOnAvailable();
            return;
        }
    }
    tNISListEntry* item = (tNISListEntry*)cCircularBuffer_operatorIndex(&mList, 0);
    int done = 0;
    if (IsVideo(item) == 0) {
        if (cBXScriptEngine_GetRequestState(*(void**)this, item->scriptHandle) == keScriptRequestState_Ready
            || cBXScriptEngine_IsRequestPlaybackEnding(*(void**)this, item->scriptHandle) != 0) {
            done = 1;
        }
    } else if (cVidEngine_IsVideoOnLastFrame(&mpNISEngine->mpPlayer) != 0) {
        tNISList_DoTransitionToNext(((char*)this));
        done = 1;
    }
    if (done) {
        PlayNextScriptOnAvailable();
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_DoSkip);
#ifdef SKIP_ASM
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_PlayNextScriptOnAvailable(tNISList* self);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cBXScriptEngine_ReleaseRequest(void* self, int id);
extern "C" void cVidEngine_ResetVideoEngine(void* p);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);

/**
 * Skips the playing script of a NIS list if its entry allows it.
 *
 * Only while playing (3), with no skip in progress (+0xC8) and bit 0 of the head entry's control bits set. Marks
 * the skip (+0xC8 = 1) and finds the next entry that is not flagged to inherit the skip (tNISList_SetupNextScript with
 * skipFlagged). When that differs from the pending one (+0xBC), the old pending entry is released (script:
 * cBXScriptEngine_ReleaseRequest; video: cVidEngine_ResetVideoEngine) and replaced; when one is found the next script
 * starts (tNISList_PlayNextScriptOnAvailable).
 *
 * Quirk: with nothing pending, the search from entry 1 runs but its result is not stored.
 */
void tNISList::DoSkip()
{
    if (mState != keNISListState_Playing) {
        return;
    }
    if (mDoSkip != 0) {
        return;
    }
    void* list = (char*)&mList;
    tNISListEntry* entry = cCircularBuffer_operatorIndex(list, 0);
    if (!(entry->controlBits & 1)) {
        return;
    }
    mDoSkip = 1;
    if (mNextPendingScriptIndex < 0) {
        SetupNextScript(1, 1);
        return;
    }
    int idx = SetupNextScript(mNextPendingScriptIndex, 1);
    if (idx != mNextPendingScriptIndex) {
        tNISListEntry* pendingEntry = (tNISListEntry*)cCircularBuffer_operatorIndex(list, mNextPendingScriptIndex);
        if (IsVideo(pendingEntry) == 0) {
            cBXScriptEngine_ReleaseRequest(*(void**)this, pendingEntry->scriptHandle);
            pendingEntry->scriptHandle = -1;
        } else if (pendingEntry->scriptHandle >= 0) {
            cVidEngine_ResetVideoEngine(&mpNISEngine->mpPlayer);
        }
        mNextPendingScriptIndex = idx;
    }
    if (idx >= 0) {
        PlayNextScriptOnAvailable();
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsSkipAllowed);
#ifdef SKIP_ASM
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);

/**
 * Says whether the playing NIS list may be skipped now.
 *
 * True only while playing (+0xA4 == 3) when the head entry's flag bit 0 (+0x8) is set and the list is not paused
 * waiting for its next script (+0xA8, On Tour mPausedOnNextScriptLoading). cSSXScriptEngine_Update checks it before
 * honouring the skip input, and cSSXScriptEngine_IsActiveNISSkippable reports it.
 *
 * @return 1 if the list may be skipped, else 0
 */
int tNISList::IsSkipAllowed()
{
    if (mState != keNISListState_Playing) {
        return 0;
    }
    int allowed = 0;
    tNISListEntry* headEntry = (tNISListEntry*)cCircularBuffer_operatorIndex((char*)&mList, 0);
    if (headEntry->controlBits & 1) {
        allowed = mPausedOnNextScriptLoading == 0;
    }
    return allowed;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsPauseMenuAllowed);
#ifdef SKIP_ASM
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);

/**
 * Returns whether the pause menu may open: always when the list is not playing, else bit 2 of the head entry's control
 * bits (+0x8).
 *
 * @return 1 when allowed, else 0
 */
int tNISList::IsPauseMenuAllowed()
{
    if (mState != keNISListState_Playing) {
        return 1;
    }
    tNISListEntry* entry = (tNISListEntry*)cCircularBuffer_operatorIndex((char*)&mList, 0);
    return (entry->controlBits >> 2) & 1;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsPendingScriptReady);
#ifdef SKIP_ASM
/**
 * Returns whether the NIS list's pending entry (+0xBC) is ready to play.
 *
 * False while loading (keNISListState_Loading); true when nothing is pending; for a pending video whether it has a
 * handle (scriptHandle >= 0); for a pending script whether its request is ready (cBXScriptEngine_GetRequestState
 * returns keScriptRequestState_Ready).
 *
 * @return true when ready
 */
bool tNISList::IsPendingScriptReady()
{
    if (mState == keNISListState_Loading) {
        return false;
    }
    int idx = mNextPendingScriptIndex;
    void* list = (char*)&mList;
    if (idx < 0) {
        return true;
    }
    tNISListEntry* item = cCircularBuffer_operatorIndex(list, idx);
    if (IsVideo(item) != 0) {
        return item->scriptHandle >= 0;
    }
    return cBXScriptEngine_GetRequestState(*(void**)this,
               cCircularBuffer_operatorIndex(list, mNextPendingScriptIndex)->scriptHandle)
           == keScriptRequestState_Ready;
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
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" cScriptPlayerMain* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);

/**
 * Returns whether @p obj is the script player of the NIS list's playing head entry.
 *
 * Requires the list to be playing (keNISListState_Playing) and the head to be a script (tNISList_IsVideo); compares @p
 * obj with cBXScriptEngine_GetRequestPlayerByReqHandle(ScriptEngine, head handle).
 *
 * @param obj  the InSeq script player (cScriptPlayerMain)
 * @return true when it is
 */
bool tNISList::IsPlayingScriptOwner(cScriptPlayerMain* obj)
{
    if (mState != keNISListState_Playing) {
        return false;
    }
    tNISListEntry* headEntry = (tNISListEntry*)cCircularBuffer_operatorIndex((char*)&mList, 0);
    if (IsVideo(headEntry) != 0) {
        return false;
    }
    return ((void*)obj) == cBXScriptEngine_GetRequestPlayerByReqHandle(ScriptEngine, headEntry->scriptHandle);
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_IsPlayingVideo);
#ifdef SKIP_ASM
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);

/**
 * Returns whether the NIS list is playing (keNISListState_Playing) a video entry at its head (tNISList_IsVideo).
 *
 * @return 1 when playing a video, else 0
 */
int tNISList::IsPlayingVideo()
{
    if (mState != keNISListState_Playing) {
        return 0;
    }
    return IsVideo(cCircularBuffer_operatorIndex(&mList, 0));
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
int tNISList::IsPausedPendingNextScript()
{
    int paused = 0;
    if (mState == keNISListState_Playing) {
        paused = mPausedOnNextScriptLoading != 0;
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
int tNISList_IsInTransition(void* vvself)
{
    tNISList* vself = (tNISList*)vvself;
    tNISList* self = (tNISList*)vself;
    return self->mInScriptTransition;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_OnScriptPlay);
#ifdef SKIP_ASM
extern "C" int tNISList_IsPlayingScriptOwner(tNISList* self, cScriptPlayerMain* obj);
char* GetSSXScriptHeader(void* obj);
extern "C" void cRenderStateMan_setCustomIrradiance(const char* name);
extern char str_TRANSP[];

/**
 * Reacts to a script starting to play: for a script this NIS list owns, switches the custom irradiance.
 *
 * When tNISList_IsPlayingScriptOwner (On Tour IsPlayingScriptOwner) accepts the player and the script header from
 * GetSSXScriptHeader (GetSSXScriptHeader) starts with a non-zero byte, calls cRenderStateMan_setCustomIrradiance (On
 * Tour cRenderStateMan::setCustomIrradiance) with "TRANSP".
 *
 * @param requestHandle   the play request handle (On Tour requestHandle); unused
 * @param obj  the script player (On Tour cScriptPlayerMain*)
 */
void tNISList::OnScriptPlay(int requestHandle, cScriptPlayerMain* obj)
{
    if (IsPlayingScriptOwner((cScriptPlayerMain*)obj) != 0) {
        char* name = GetSSXScriptHeader(obj);
        char* msg = str_TRANSP;
        if (*name != 0) {
            cRenderStateMan_setCustomIrradiance(msg);
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
 * Only for the list's own script player (tNISList_IsPlayingScriptOwner, On Tour IsPlayingScriptOwner) on the cut-scene
 * list (+0x4) and while the player's +0x1C word is 0: when the updates left (cScriptPlayerMain_GetNumUpdatesToEndLeft()
 * + 1) drop to the script header's next-transition in-frame count (short at +0x12 of GetSSXScriptHeader's header),
 * calls tNISList_DoTransitionToNext (On Tour DoTransitionToNext). The current frame and frame delta that On Tour passes
 * are not declared (unused).
 *
 * @param requestHandle  the request handle (unused)
 * @param obj  the cScriptPlayerMain being updated
 */
void tNISList::OnScriptUpdate(int requestHandle, cScriptPlayerMain* obj)
{
    if (IsPlayingScriptOwner((cScriptPlayerMain*)obj) != 0 && mIsCutSceneHandler != 0 &&
        obj->mLoopScript == 0) {
        tSSXScriptHeader* name = (tSSXScriptHeader*)GetSSXScriptHeader(obj);
        if (cScriptPlayerMain_GetNumUpdatesToEndLeft(obj) + 1 <= name->endFadeToTransitionFrames) {
            tNISList_DoTransitionToNext(this);
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
 * Only for the list's own player (tNISList_IsPlayingScriptOwner): while a transition wait is pending (+0xB8) the
 * request is paused (cBXScriptEngine_PauseScript), then tNISList_WaitOnPendingNextScript checks whether the next script
 * must be waited for.
 *
 * @param requestHandle  the stopping script's request handle
 * @param obj  the stopping InSeq player
 */
void tNISList::OnScriptStopping(int requestHandle, cScriptPlayerMain* obj)
{
    if (IsPlayingScriptOwner((cScriptPlayerMain*)obj) != 0) {
        if (mTransitionWaitFrames != 0) {
            cBXScriptEngine_PauseScript(*(void**)this, requestHandle);
        }
        WaitOnPendingNextScript();
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/datamanager", tNISList_OnScriptStop);
#ifdef SKIP_ASM
extern "C" int tNISList_IsPlayingScriptOwner(tNISList* self, cScriptPlayerMain* obj);
char* GetSSXScriptHeader(void* obj);
extern "C" void cRenderStateMan_setCustomIrradiance(const char* name);

/**
 * Script stop callback: drops the custom irradiance the list's script had set.
 *
 * For the list's own script player (tNISList_IsPlayingScriptOwner, On Tour IsPlayingScriptOwner), when the script
 * header (GetSSXScriptHeader) starts with a non-empty name, calls cRenderStateMan_setCustomIrradiance(0) (On Tour
 * cRenderStateMan::setCustomIrradiance) to clear it.
 *
 * @param requestHandle  the request handle (unused)
 * @param obj  the cScriptPlayerMain that stopped
 */
void tNISList::OnScriptStop(int requestHandle, cScriptPlayerMain* obj)
{
    if (IsPlayingScriptOwner((cScriptPlayerMain*)obj) != 0) {
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
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cSSXScriptEngine_RequestScriptByCategory(void* self, int category, int a, int b, int c);
extern "C" int cSSXScriptEngine_RequestScriptByIndex(void* self, int key, int a, int b, int c);
extern "C" bool cVidEngine_ReadyVideo(void** self, int file);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int* tNISList_GetVideoDBRecord_p(void* self, void* ent) __asm__("tNISList_GetVideoDBRecord");
int cCircularBuffer_GetNumEntries_count(void* list) __asm__("cCircularBuffer_GetNumEntries");


/**
 * Finds the next entry of the NIS list that can play, requesting its script or readying its video.
 *
 * Starting at @p index, an entry already requested is returned at once; a script entry is requested by index
 * (or by category when it has none) and returned when the request succeeds; a video entry readies its video
 * when no video player exists yet and is returned.
 *
 * @param index  entry to start searching at (On Tour startSearchIndex)
 * @param skipFlagged  nonzero to pass over entries with flag 2 (On Tour ignoreSkipInheritScripts)
 * @return the index of the entry set up, or -1 when none can be
 */
int tNISList::SetupNextScript(int index, int skipFlagged)
{
    int count = cCircularBuffer_GetNumEntries_count(&mList);
    int i = index;
    for (; i < count; i++) {
        tNISListEntry* currEntry = (tNISListEntry*)cCircularBuffer_operatorIndex(&mList, i);
        if (skipFlagged && (currEntry->controlBits & 2))
            continue;
        if (currEntry->scriptHandle >= 0)
            return i;
        if (IsVideo(currEntry) == 0) {
            void* eng = *(void**)this;
            if (currEntry->scriptIndex >= 0)
                currEntry->scriptHandle = cSSXScriptEngine_RequestScriptByIndex(eng, currEntry->scriptIndex, currEntry->aiCharIndexMain, currEntry->aiCharIndexSupport0, currEntry->aiCharIndexSupport1);
            else
                currEntry->scriptHandle = cSSXScriptEngine_RequestScriptByCategory(*(void**)this, currEntry->category,
                        currEntry->aiCharIndexMain, currEntry->aiCharIndexSupport0, currEntry->aiCharIndexSupport1);
            if (currEntry->scriptHandle >= 0)
                return i;
        } else {
            if (mpNISEngine->mpPlayer == 0) {
                int* videoEntry = tNISList_GetVideoDBRecord_p(((char*)this), currEntry);
                if (cVidEngine_ReadyVideo((void**)&mpNISEngine->mpPlayer, *videoEntry) != 0)
                    currEntry->scriptHandle = 0;
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
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" sDmEnt7DE8* tNISList_GetVideoDBRecord(tNISList* self, int* key);
extern "C" int cBXScriptEngine_ReleaseRequest(void* self, int id);
extern "C" void cVidEngine_ResetVideoEngine(void* p);
extern "C" int cVidEngine_IsVideoOnLastFrame(void* self);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" int cCircularBuffer_IsEmpty(void* self);
extern "C" bool cCircularBuffer_RemoveFromHead(void* self, void* out);
extern "C" cSSXAudio* cAudioInterface_bxAudio();



/**
 * Removes up to @p n entries from the head of a NIS list, releasing what each one holds.
 *
 * Stops early when the buffer is empty (cCircularBuffer_IsEmpty). An entry with a handle (+0xC >= 0) releases its
 * script request (cBXScriptEngine_ReleaseRequest) or, for a video, sends the video record's music event when the video
 * engine is playing (mpNISEngine->mIsPlaying): the end event (+0x18 of the tNISList_GetVideoDBRecord record) once
 * cVidEngine_IsVideoOnLastFrame reports the video done, else the skip event (+0x14), through virtual slot 4 of the
 * bxAudio sound system (cAudioInterface_bxAudio, +0x118); then resets the video engine (cVidEngine_ResetVideoEngine).
 * Each entry is then popped (cCircularBuffer_RemoveFromHead).
 *
 * @param n  the number of entries to remove
 */
void tNISList::RemoveFromHead(int n)
{
    while (n != 0) {
        if (cCircularBuffer_IsEmpty(&mList) != 0) {
            return;
        }
        tNISListEntry cmd = *(tNISListEntry*)cCircularBuffer_operatorIndex(&mList, 0);
        if (cmd.scriptHandle >= 0) {
            if (IsVideo(&cmd) == 0) {
                cBXScriptEngine_ReleaseRequest(*(void**)this, cmd.scriptHandle);
            } else {
                if (mpNISEngine->mIsPlaying != 0) {
                    int* ent = (int*)tNISList_GetVideoDBRecord(this, (int*)&cmd);
                    int snd;
                    if (cVidEngine_IsVideoOnLastFrame(&mpNISEngine->mpPlayer) != 0) {
                        snd = ent[6];
                    } else {
                        snd = ent[5];
                    }
                    if (snd >= 0) {
                        cSSXAudio* soundMan = cAudioInterface_bxAudio();
                        sVEntry<void(void*, int)>* vt = (sVEntry<void (void*, int)>*)soundMan->MusicSys.mVtbl;
                        char* sys = (char*)&soundMan->MusicSys;
                        vt[4].fn(vthis(sys, vt[4]), snd);
                    }
                }
                cVidEngine_ResetVideoEngine(&mpNISEngine->mpPlayer);
            }
        }
        cCircularBuffer_RemoveFromHead(&mList, 0);
        n--;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_WaitOnPendingNextScript);
#ifdef SKIP_ASM
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cBXScriptEngine_GetRequestState(void* self, int a1);

/**
 * Pauses the NIS list on loading while its pending entry (+0xBC) is still loading.
 *
 * The pending entry is still loading when its script request waits for data (cBXScriptEngine_GetRequestState returns
 * keScriptRequestState_DataPending)
 * or, for a video, it has no handle (scriptHandle < 0); then tNISList_DoPause(self, 1, 1) pauses the list. Nothing
 * happens with nothing pending.
 */
void tNISList::WaitOnPendingNextScript()
{
    int idx = mNextPendingScriptIndex;
    if (idx >= 0) {
        void* list = (char*)&mList;
        tNISListEntry* nextEntry = cCircularBuffer_operatorIndex(list, idx);
        int ok;
        if (IsVideo(nextEntry) == 0) {
            ok = cBXScriptEngine_GetRequestState(*(void**)this,
                               cCircularBuffer_operatorIndex(list, mNextPendingScriptIndex)->scriptHandle)
                 == keScriptRequestState_DataPending;
        } else {
            ok = nextEntry->scriptHandle < 0;
        }
        if (ok) {
            tNISList_DoPause(this, 1, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_CheckOnLoading);
#ifdef SKIP_ASM
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" int cBXScriptEngine_GetRequestState(void* self, int a1);

/**
 * Moves a loading NIS list (keNISListState_Loading) to keNISListState_Ready once its head is ready.
 *
 * A head video counts as ready at once; a head script when its request state (cBXScriptEngine_GetRequestState) is
 * keScriptRequestState_Ready.
 */
void tNISList::CheckOnLoading()
{
    if (mState == keNISListState_Loading) {
        tNISListEntry* headEntry = (tNISListEntry*)((char*)cCircularBuffer_operatorIndex((char*)&mList, 0));
        if (IsVideo(headEntry) == 0) {
            int requestState = cBXScriptEngine_GetRequestState(*(void**)this, headEntry->scriptHandle);
            if (requestState == keScriptRequestState_Ready) {
                mState = requestState;
            }
        } else {
            mState = keNISListState_Ready;
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
 * Plays the NIS list's next script when it is available: tNISList_WaitOnPendingNextScript first, then
 * tNISList_PlayNextScript unless the list is now paused on loading (+0xA8).
 */
void tNISList::PlayNextScriptOnAvailable()
{
    WaitOnPendingNextScript();
    if (mPausedOnNextScriptLoading == 0) {
        PlayNextScript();
    }
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_PlayNextScript);
#ifdef SKIP_ASM
cTransitionMan* cGame_getTransitionMan(void* self, int a1);
extern "C" void tNISList_Reset(tNISList* self, int a1);
extern "C" int tNISList_SetupNextScript(tNISList* self, int index, int a2);
extern "C" void tNISList_RemoveFromHead(tNISList* self, int index);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" void cSSXScriptEngine_PlayRequestScript(void* self, int val, int key, int b, int c);
extern "C" bool cVidEngine_PlayVideo(void* self);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" float cTransitionMan_GetTransitionTimeLeft(void* self);
extern "C" void cCutSceneTransitionMan_EndCutScene(void* self, float t);



/**
 * Moves a NIS list on to its pending entry, or ends the list when nothing is pending.
 *
 * With a pending entry (+0xBC > 0): clears the skip (+0xC8), drops the entries before it (tNISList_RemoveFromHead),
 * clears the skip position (+0xC0) and pending index, and starts the new head: a script with
 * cSSXScriptEngine_PlayRequestScript (view index +0x8, bit 3 of its control bits) and the engine's virtual slot 6
 * (cSSXScriptEngine_ForcePlayUpdate), a video with cVidEngine_PlayVideo. A running script transition (+0xB0) sets the
 * start-transition flag (+0xAC); the next entry is set up as pending. Otherwise the list is reset without transitions
 * (tNISList_Reset) and, on the cut-scene list (+0x4), the game's fade (+0x64 of the game) gets
 * cCutSceneTransitionMan_EndCutScene with the transition manager's remaining time
 * (cTransitionMan_GetTransitionTimeLeft) when a script transition was running and the manager is busy, else 0.5.
 *
 */
void tNISList::PlayNextScript()
{
    if (mNextPendingScriptIndex > 0) {
        mDoSkip = 0;
        RemoveFromHead(mNextPendingScriptIndex);
        mNumScriptsToSkipInProgress = 0;
        mNextPendingScriptIndex = -1;
        tNISListEntry* cmd = (tNISListEntry*)cCircularBuffer_operatorIndex(&mList, 0);
        if (IsVideo(cmd) == 0) {
            cSSXScriptEngine_PlayRequestScript(*(void**)this, mViewIndex, cmd->scriptHandle, 0,
                    (cmd->controlBits >> 3) & 1);
            cSSXScriptEngine* eng = mpNISEngine;
            sVEntry<int(void*, int)>* vt = (sVEntry<int(void*, int)>*)eng->mVtbl;
            vt[6].fn(vthis(eng, vt[6]), cmd->scriptHandle); // ForcePlayUpdate
        } else {
            cVidEngine_PlayVideo(&mpNISEngine->mpPlayer);
        }
        if (mInScriptTransition != 0) {
            mIsStartTransition = 1;
        }
        mNextPendingScriptIndex = SetupNextScript(1, 0);
    } else {
        int wasOn = mInScriptTransition;
        Reset(0);
        if (mIsCutSceneHandler != 0) {
            cGame* app = SSXApp->Game;
            void* fade = app->CutSceneTransitionMan;
            cTransitionMan* transitionMan = cGame_getTransitionMan(app, 0);
            if (wasOn != 0 && transitionMan->mbPlayingTransition != 0) {
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
cTransitionMan* cGame_getTransitionMan(void* self, int a1);
char* GetSSXScriptHeader(void* obj);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
struct sDmEnt7DE8;
extern "C" sDmEnt7DE8* tNISList_GetVideoDBRecord(tNISList* self, int* key);
extern "C" cScriptPlayerMain* cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" void cTransitionMan_EndTransition(void* p);
extern "C" int cSSXScriptEngine_MakeTransitionObject(void* self, int a1, int a2);
extern "C" void cTransitionMan_StartTransition(void* snd, int a1, int h, int h2, int a4, int a5, float t0, float t1, float t2);
extern "C" void cTransitionMan_StartTransitionOut(void* snd, int a1, int h, float t);
extern "C" void cTransitionMan_StartTransitionScreenGrab(void* snd, int a1, int h, int a3, float t);



// The unit declares tNISList_DoTransitionToNext as void; it returns whether it handled the update.

/**
 * Starts the transition from the current NIS entry into the next, or reports one already running.
 *
 * With a script transition running (+0xB0, On Tour mInScriptTransition) it returns 1, unless the start flag (+0xAC,
 * mIsStartTransition) is set: then it ends the transition (cTransitionMan_EndTransition on the game's transition
 * manager, cGame_getTransitionMan(Game, 0)) and returns 0. Otherwise it ends any transition and builds the next one.
 * For a script head entry it reads the next-transition record of the script header (GetSSXScriptHeader of the request's
 * player: type +0x10, colour +0x11, in/hold/out frame shorts +0x12..+0x16, 1/60 s each): keScriptTransition_CrossFade
 * is a screen grab (cTransitionMan_StartTransitionScreenGrab) with no wait, any other type a full transition
 * (cTransitionMan_StartTransition with two cSSXScriptEngine_MakeTransitionObject objects; returns 0 if none can be
 * made) whose in+hold frames go to +0xB8 (mTransitionWaitFrames). A video entry uses its video record
 * (tNISList_GetVideoDBRecord, On Tour GetVideoDBRecord) for a transition out (cTransitionMan_StartTransitionOut) or a
 * screen grab. A positive wait sets +0xB0 and clears +0xAC.
 *
 * @return +0xB0: 1 while a script transition is running
 */
int tNISList::DoTransitionToNext()
{
    if (mInScriptTransition != 0) {
        if (mIsStartTransition != 0) {
            cTransitionMan_EndTransition(cGame_getTransitionMan(SSXApp->Game, 0));
            return 0;
        }
        return 1;
    }
    tNISListEntry* item = cCircularBuffer_operatorIndex(&mList.mBuffer[0], 0);
    if (IsVideo(item) == 0) {
        tSSXScriptHeader* transitionInfo = (tSSXScriptHeader*)GetSSXScriptHeader(
            cBXScriptEngine_GetRequestPlayerByReqHandle(ScriptEngine, item->scriptHandle));
        cTransitionMan* snd = (cTransitionMan*)cGame_getTransitionMan(SSXApp->Game, 0);
        if (snd->mbPlayingTransition != 0) {
            cTransitionMan_EndTransition(snd);
        }
        if (transitionInfo->endFadeToTransitionFrames + transitionInfo->endHoldTransitionFrames + transitionInfo->endFadeFromTransitionFrames > 0) {
            if (transitionInfo->endTransitionType != keScriptTransition_CrossFade) {
                int transition = cSSXScriptEngine_MakeTransitionObject(*(void**)this,
                        transitionInfo->endTransitionType, transitionInfo->endTransitionColour);
                if (transition == 0) {
                    return 0;
                }
                int transitionOut = cSSXScriptEngine_MakeTransitionObject(*(void**)this,
                        transitionInfo->endTransitionType, transitionInfo->endTransitionColour);
                cTransitionMan_StartTransition(snd, 2, transition, transitionOut, 0, 0, (float)transitionInfo->endFadeToTransitionFrames * 0.01666666753590107f, (float)transitionInfo->endHoldTransitionFrames * 0.01666666753590107f, (float)transitionInfo->endFadeFromTransitionFrames * 0.01666666753590107f);
                mTransitionWaitFrames = transitionInfo->endFadeToTransitionFrames +
                        transitionInfo->endHoldTransitionFrames;
            } else {
                int transition = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, keScriptTransition_CrossFade,
                    transitionInfo->endTransitionColour);
                cTransitionMan_StartTransitionScreenGrab(snd, 2, transition, 0, (float)transitionInfo->endFadeToTransitionFrames * 0.01666666753590107f);
                mTransitionWaitFrames = 0;
            }
            if (mTransitionWaitFrames > 0) {
                mIsStartTransition = 0;
                mInScriptTransition = 1;
            }
        }
    } else {
        tVideoDB* videoInfo = (tVideoDB*)tNISList_GetVideoDBRecord((tNISList*)this, (int*)item);
        cTransitionMan* snd = (cTransitionMan*)cGame_getTransitionMan(SSXApp->Game, 0);
        if (snd->mbPlayingTransition != 0) {
            cTransitionMan_EndTransition(snd);
        }
        if (0.0f < videoInfo->transitionOutTime) {
            if (videoInfo->transitionOut != keScriptTransition_CrossFade) {
                int transition = cSSXScriptEngine_MakeTransitionObject(*(void**)this, videoInfo->transitionOut,
                        videoInfo->transitionColour);
                cTransitionMan_StartTransitionOut(snd, 2, transition, videoInfo->transitionOutTime);
                mTransitionWaitFrames = 0;
            } else {
                int transition = cSSXScriptEngine_MakeTransitionObject(ScriptEngine, keScriptTransition_CrossFade,
                    videoInfo->transitionColour);
                cTransitionMan_StartTransitionScreenGrab(snd, 2, transition, 0, videoInfo->transitionOutTime);
                mTransitionWaitFrames = 0;
            }
        }
    }
    return mInScriptTransition;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_DoPause);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
cTransitionMan* cGame_getTransitionMan(void* self, int a1);
extern "C" bool tNISList_IsVideo(tNISList* self, void* a1);
extern "C" void cBXScriptEngine_PauseScript(void* self, int a1);
extern "C" void cBXScriptEngine_ResumeScript(void* self, int a1);
extern "C" int cBXScriptEngine_GetRequestState(void* self, int a1);
extern "C" void cVidEngine_PauseVideo(void* self, int on);
extern "C" tNISListEntry* cCircularBuffer_operatorIndex(void* list, int i);
extern "C" void cTransitionMan_PauseTransition(void* p, int on);
extern "C" void cCutSceneTransitionMan_Pause(void* p, int on);



// The unit declares tNISList_DoPause as returning void*; the body returns nothing. Bind by asm label.

/**
 * Pauses or resumes a playing NIS list, through either the pause count or the pause-on-loading flag.
 *
 * Does nothing unless playing (3). With @p hard 0 the pause count (+0xC4) goes up or down and only the 0-to-1 or 1-to-0
 * step acts, and not while paused on loading (+0xA8). With @p hard set the pause-on-loading flag is set to @p on and
 * acts only when it changed and no counted pause is held. Acting pauses the head (a script request in states 3-5 with
 * cBXScriptEngine_PauseScript; a video with cVidEngine_PauseVideo) or resumes it (a paused request,
 * keScriptRequestState_Paused, with cBXScriptEngine_ResumeScript; a video with cVidEngine_PauseVideo), and on the
 * cut-scene list (+0x4) passes the same flag to the game's cut-scene transition manager (cCutSceneTransitionMan_Pause)
 * and, when busy (+0x44), to the transition manager (cTransitionMan_PauseTransition).
 *
 * @param on  nonzero to pause, 0 to resume
 * @param hard  nonzero for the pause-on-loading flag, 0 for the pause count
 */
void tNISList::DoPause(int on, int hard)
{
    if (mState != keNISListState_Playing) {
        return;
    }
    if (hard == 0) {
        int prevPauses = mNumPause;
        if (on) {
            mNumPause = prevPauses + 1;
            if (prevPauses != 0) {
                return;
            }
        } else {
            if (prevPauses <= 0) {
                return;
            }
            int remainingPauses = prevPauses - 1;
            mNumPause = remainingPauses;
            if (remainingPauses != 0) {
                return;
            }
        }
        if (mPausedOnNextScriptLoading) {
            return;
        }
    } else {
        if (mPausedOnNextScriptLoading == on) {
            return;
        }
        mPausedOnNextScriptLoading = on;
        if (mNumPause) {
            return;
        }
    }
    if (on) {
        tNISListEntry* it = (tNISListEntry*)cCircularBuffer_operatorIndex(((char*)&mList), 0);
        if (IsVideo(it) == 0) {
            int st = cBXScriptEngine_GetRequestState(((char*)mpNISEngine), it->scriptHandle);
            if (st >= keScriptRequestState_PlayPending && st <= keScriptRequestState_Paused) {
                cBXScriptEngine_PauseScript(((char*)mpNISEngine), it->scriptHandle);
            }
        } else {
            cVidEngine_PauseVideo(&mpNISEngine->mpPlayer, 1);
        }
        if (mIsCutSceneHandler) {
            cCutSceneTransitionMan_Pause(SSXApp->Game->CutSceneTransitionMan, 1);
            cTransitionMan* transitionMan = (cTransitionMan*)cGame_getTransitionMan(SSXApp->Game, 0);
            if (transitionMan->mbPlayingTransition) {
                cTransitionMan_PauseTransition(transitionMan, 1);
            }
        }
    } else {
        tNISListEntry* it = (tNISListEntry*)cCircularBuffer_operatorIndex(((char*)&mList), 0);
        if (IsVideo(it) == 0) {
            int requestState = cBXScriptEngine_GetRequestState(((char*)mpNISEngine), it->scriptHandle);
            if (requestState == keScriptRequestState_Paused) {
                cBXScriptEngine_ResumeScript(((char*)mpNISEngine), it->scriptHandle);
            }
        } else {
            cVidEngine_PauseVideo(&mpNISEngine->mpPlayer, 0);
        }
        if (mIsCutSceneHandler) {
            cCutSceneTransitionMan_Pause(SSXApp->Game->CutSceneTransitionMan, 0);
            cTransitionMan* transitionMan = (cTransitionMan*)cGame_getTransitionMan(SSXApp->Game, 0);
            if (transitionMan->mbPlayingTransition) {
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
 * @return true for a video, false for a script
 */
bool tNISList::IsVideo(void* entry)
{
    return *(int*)entry >= 29;
}
#endif

//100%
INCLUDE_ASM("scripter/datamanager", tNISList_GetVideoDBRecord);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");

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
 * flags, and picks the table at 0x00482708 when the widescreen setting (tBEOptionDB::iUseWidescreen, bits 20-21) is
 * kWidescreen_Anamorphic, else the one at 0x004827A8. The symbols
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
    if ((cfg.flags & kOptionWidescreenMask) != kWidescreen_Anamorphic << kOptionWidescreenShift) {
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
void tSSXScriptBankInfo_Clear(void* vvself)
{
    tSSXScriptBankInfo* vself = (tSSXScriptBankInfo*)vvself;
    tSSXScriptBankInfo* self = (tSSXScriptBankInfo*)vself;
    *(int*)self = 0;
    self->dataIdentifier = -1;
    self->asyncHandle = 0;
    self->dynamicMem = 0;
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

// A camera manager built in place by its retail constructor (the class itself declares no constructor).
struct cScriptObjCameraManagerCtor : public cScriptObjCameraManager {
    cScriptObjCameraManagerCtor() { cScriptObjCameraManager_cScriptObjCameraManager(this); }
    void* operator new[](unsigned int, void* p) { return p; }
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
extern "C" cSSXScriptEngine* cSSXScriptEngine_cSSXScriptEngine(cSSXScriptEngine* self)
{
    cBXScriptEngine_cBXScriptEngine(self);
    cVidEngine_cVidEngine(&self->mpPlayer);
    cDataManager_cDataManager((cDataManager*)(&self->mMaxDataInstances), 0, 0x100);
    self->mInSeqCommVtbl = D_00482100;
    self->mVidEngineVtbl = D_004821A0;
    self->mDataManagerVtbl = D_00482168;
    self->mVtbl = cSSXScriptEngine_vtbl;
    new (self->mObjectCameraManager) cScriptObjCameraManagerCtor[2];
    cScriptObjGS_cScriptObjGS(&self->mObjectGameState);
    cScriptObjEvent_cScriptObjEvent(&self->mObjectEvent);
    cScriptAnimBankManager_cScriptAnimBankManager(&self->mAnimationBankManager, 0, 10, 0xF0);
    cScriptSoundBankManager_cScriptSoundBankManager(self->mSoundBankManager, 0, 10, 17);
    self->mpMasterTableHeader = 0;
    self->mpMasterTable = 0;
    self->mpMasterTableDebug = 0;
    self->mFilterTableAsyncHandle = 0;
    self->mpFilterTableHeader = 0;
    self->mpCategoryFilterTable = 0;
    self->mpSetupInfoHeap = 0;
    self->mpSetupInfoUnused = 0;
    D_004A34B0 = self;
    self->mCurrAttachingBankNum = 10;
    self->mPlayingNISList = 2;
    self->mpNISLists = 0;
    self->mNumFramesNoSkip = 0;
    self->mNumAsyncs = 0;
    self->mpIdleLists = 0;
    int i;
    for (i = 49; i >= 0; i--) {
        self->mpAnchorTableDB[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        tSSXScriptBankInfo_Clear(&self->mScriptDataBanks[i]);
    }
    for (i = 0; i < 2; i++) {
        cScriptObjCameraManager_SetViewIndex(&self->mObjectCameraManager[i], i);
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
    if (self->mpMasterTableHeader != 0) {
        self->DeInitOnce();
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
    cScriptObjCameraManager* cameraManager = (cScriptObjCameraManager*)(&self->mObjectGameState);
    cScriptObjCameraManager* begin = ((&self->mObjectCameraManager[0]));
    if (begin != 0) {
        do {
            while (begin != cameraManager) {
                cameraManager--;
                sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)cameraManager->mVtbl;
                vt[1].fn(vthis(cameraManager, vt[1]), 0);
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


/**
 * Loads the script master table (and its debug name table) and sets up the base script engine for it.
 *
 * With the debug table, each script's name offset becomes a pointer into it; without it, the names are 0.
 * Then
 * SetupBXEngine() is called with the script count, 10 and 0x4000.
 */
// PORT: the file's 32-bit name offsets are rebased in place into pointers (tSSXHiveScriptNameDebug).
void cSSXScriptEngine::InitOnce()
{
    tSSXHiveScriptMasterTableHeader* hdr =
        (tSSXHiveScriptMasterTableHeader*)FILE_loadz(kpScriptMasterTableFilename, 0);
    mpMasterTableHeader = hdr;
    mpMasterTable = &hdr->masterTableStart;
    char* base = (char*)FILE_loadz(kpScriptMasterTableDebugFilename, 0);
    mpMasterTableDebug = base;
    if (base != 0) {
        for (int i = 0; i < mpMasterTableHeader->numScripts; i++) {
            mpMasterTable[i].debugNamePtr.pScriptName =
                mpMasterTableDebug + mpMasterTable[i].debugNamePtr.scriptNameOffset;
        }
    } else {
        for (int i = 0; i < mpMasterTableHeader->numScripts; i++) {
            mpMasterTable[i].debugNamePtr.pScriptName = 0;
        }
    }
    mNumFramesNoSkip = 0;
    mNumAsyncs = 0;
    cBXScriptEngine_SetupBXEngine(this, mpMasterTableHeader->numScripts, 10, 0x4000);
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
void cSSXScriptEngine::DeInitOnce()
{
    cBXScriptEngine_PurgeBXEngine(this);
    MEM_free(mpMasterTableHeader);
    if (mpMasterTableDebug != 0) {
        MEM_free(mpMasterTableDebug);
    }
    mpMasterTableHeader = 0;
    mpMasterTable = 0;
    mpMasterTableDebug = 0;
    mNumFramesNoSkip = 0;
}
#endif

