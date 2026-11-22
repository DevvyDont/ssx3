#include "vtable.h"
#include "common.h"
#include "scripter/scriptentitycam.h"
#include "scripter/scriptobjtransition.h"
#include "scripter/scriptobj.h"
#include "scripter/scriptentity.h"
#include "inseq/scriptplayerentity.h"
#include "inseq/scriptplayermain.h"
#include "scripter/bxscriptengine.h"
#include "scripter/ssxscriptengine.h"

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetupBXEngine);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cInSeq_cInSeq(void* obj, void* buf, int size, int a3, void* out);
extern char str_InSeqHeap[];
extern char str_BXScriptDatabase[];
extern char str_InSeq[];

struct sBXSlot {
    int a;
    short b;
};

/**
 * Sets up the BX script engine: allocates the InSeq sequencer with its heap and an empty script database.
 *
 * Allocates @p size bytes of "InSeqHeap" (mpInSeqHeap), creates the 0x20-byte "InSeq" sequencer (mpInSeq)
 * over that heap with this engine's cInSeqComm interface (+0x2AC), and allocates and zeroes @p count 8-byte
 * "BXScriptDatabase" entries (mpScriptDatabase, count in mScriptDatabaseSize).
 *
 * @param count  number of script database entries (On Tour scriptDatabaseSize)
 * @param maxUniqueAnimPlayers  passed to cInSeq_cInSeq() as its script limit (cInSeq's maxScripts)
 * @param size  InSeq heap size in bytes (On Tour heapSize)
 */
extern "C" void cBXScriptEngine_SetupBXEngine(cBXScriptEngine* self, int count, int maxUniqueAnimPlayers, int size)
{
    self->mpInSeqHeap = (char*)operator_new_tag(size, str_InSeqHeap, 0, 0);
    self->mpInSeq = (cInSeq*)cInSeq_cInSeq(cMemMan_alloc(0x20, str_InSeq, 0, 0),
        self->mpInSeqHeap, size, maxUniqueAnimPlayers, (char*)&self->mInSeqCommVtbl);
    self->mScriptDatabaseSize = count;
    self->mpScriptDatabase = (tBXScriptState*)((sBXSlot*)operator_new_tag(count * 8, str_BXScriptDatabase, 0, 0));
    for (int i = 0; i < self->mScriptDatabaseSize; i++) {
        ((sBXSlot*)self->mpScriptDatabase)[i].a = 0;
        ((sBXSlot*)self->mpScriptDatabase)[i].b = 0;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_PurgeBXEngine);
#ifdef SKIP_ASM
struct sScriptEngine282390;
struct sScriptEngine2823F0;
struct sScriptEngine2B8;
extern "C" void cBXScriptEngine_StopScriptAll(cBXScriptEngine* self);
extern "C" void cBXScriptEngine_ReleaseRequestAll(cBXScriptEngine* self);
extern "C" void cBXScriptEngine_ResetInstVars(sScriptEngine2B8* self);
extern "C" void cScriptDatabaseObject_RemoveObjectAll(void* self);
extern "C" void cInSeq__cInSeq(void* obj, int flags);
void cMemMan_free(void*);

/**
 * Tears down the BX script engine's script database, InSeq sequencer and InSeq heap.
 *
 * Does nothing unless mpScriptDatabase was allocated (cBXScriptEngine_SetupBXEngine). Otherwise it stops every
 * script, releases every script request, removes every database object, frees the database and zeroes its size,
 * destroys mpInSeq (destructor flags 3), frees mpInSeqHeap, and finally calls cBXScriptEngine_ResetInstVars().
 */
extern "C" void cBXScriptEngine_PurgeBXEngine(cBXScriptEngine* self)
{
    if (self->mpScriptDatabase != 0) {
        cBXScriptEngine_StopScriptAll(self);
        cBXScriptEngine_ReleaseRequestAll(self);
        cScriptDatabaseObject_RemoveObjectAll(self);
        if (self->mpScriptDatabase != 0) {
            cMemMan_free(self->mpScriptDatabase);
        }
        self->mpScriptDatabase = 0;
        self->mScriptDatabaseSize = 0;
        if (self->mpInSeq != 0) {
            cInSeq__cInSeq(self->mpInSeq, 3);
        }
        self->mpInSeq = 0;
        if (self->mpInSeqHeap != 0) {
            cMemMan_free(self->mpInSeqHeap);
        }
        self->mpInSeqHeap = 0;
        cBXScriptEngine_ResetInstVars((sScriptEngine2B8*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_Update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cInSeq_UpdateFrame(void* obj);
extern "C" void cBXScriptEngine_UpdatePendingRequestState(cBXScriptEngine* self, tScriptRequestInfo* script);



// Retail builds compile the per-state debug hooks to nothing.
static inline void slotNop_2820B0(void* self, void* slot) {}
/**
 * Per-frame update of the script engine.
 *
 * Calls cInSeq_UpdateFrame() on the sequencer at +0x2B8, then for each of the 16 requests in state 1 runs its
 * pending-state handler by request type: type 1 goes to cBXScriptEngine_UpdatePendingRequestState, the others to slotNop_2820B0 (On Tour does this
 * in UpdatePendingRequestState).
 */
extern "C" void cBXScriptEngine_Update(cBXScriptEngine* self)
{
    int i;
    cInSeq_UpdateFrame(self->mpInSeq);
    for (i = 0; i < 16; i++) {
        tScriptRequestInfo* request = &((&self->mScriptRequests[0]))[i];
        if (request->requestType == 0) {
            continue;
        }
        if (request->requestType == 1) {
            switch (request->state) {
            case 0: slotNop_2820B0((self), request); break;
            case 1: cBXScriptEngine_UpdatePendingRequestState(self, (tScriptRequestInfo*)request); break;
            case 2: slotNop_2820B0((self), request); break;
            case 3: slotNop_2820B0((self), request); break;
            case 4: slotNop_2820B0((self), request); break;
            case 5: slotNop_2820B0((self), request); break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_AttachScript);
#ifdef SKIP_ASM
/**
 * Attaches script data to script database slot @p index, unless the slot already holds data.
 *
 * Writes @p value into mpScriptDatabase[@p index] (+0x2B0, 8-byte entries; first word = the script data).
 * cSSXScriptEngine_OnAttachData calls it for each loaded bank.
 *
 * @param value  the script data
 * @param index  the script index
 * @return 1 when attached, 0 when the slot was taken
 */
extern "C" int cBXScriptEngine_AttachScript(cBXScriptEngine* self, int value, int index)
{
    tBXScriptState* slots = self->mpScriptDatabase;
    if (((int)slots[index].pScriptData) != 0) {
        return 0;
    }
    *(int*)&slots[index].pScriptData = value;
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_DetachScript);
#ifdef SKIP_ASM
/**
 * Detaches the script data from script database slot @p index and returns it.
 *
 * Clears the data word of mpScriptDatabase[@p index] (+0x2B0). cSSXScriptEngine_OnDetachData calls it.
 *
 * @param index  the script index
 * @return the detached script data, or 0 when the slot was empty
 */
extern "C" int cBXScriptEngine_DetachScript(cBXScriptEngine* self, int index)
{
    tBXScriptState* slots = self->mpScriptDatabase;
    int old = ((int)slots[index].pScriptData);
    if (old == 0) {
        return 0;
    }
    *(int*)&slots[index].pScriptData = 0;
    return old;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_RequestAnimationScript);
#ifdef SKIP_ASM
struct sBxsThreadHdr;
extern "C" sBxsThreadHdr* cBXScriptEngine_CreateNewRequest(cBXScriptEngine* self, int a1);
extern "C" int cInSeq_ReadyPlayer(void* self, void* src);
// PORT: the unit declares cInSeq_GetPlayer(int), but it really takes (self, id).
void* cInSeq_GetPlayer_2(void* self, int id) __asm__("cInSeq_GetPlayer");
// PORT: the setter's mangled signature is (void*, int); a pointer is passed as int.
int cScriptPlayerMain_SetUserData(void* self, int a1);
void cBXScriptEngine_ResetRequest(void* self, void* v);

struct sBxsVEntry21A0 {
    short delta;
    short index;
    int (*fn)(void*, int);
};


struct sBXThread21A0 {
    void* owner;    // 0x0
    int state;      // 0x4
    int slot;       // 0x8
    int arg;        // 0xC
    short pad_10;   // 0x10
    short f12;      // 0x12
    int pending;    // 0x14
    float time;     // 0x18
    int result;     // 0x1C
};

/**
 * Creates an animation script request (request type 1) for script @p idx and returns its handle.
 *
 * Takes a free request from mScriptRequests with cBXScriptEngine_CreateNewRequest. When mpScriptDatabase[@p idx] (+0x2B0) holds no data
 * yet, the request waits for it: state 1 (data pending), the index at +0x14 and the pending flag at +0x12; the engine's
 * virtual at vtable slot 7 (cSSXScriptEngine_OnDataUnavailable) is asked to fetch it, and on refusal the request goes
 * to state 7, is reset with cBXScriptEngine_ResetRequest, and -1 is returned. Otherwise the request is ready (state 2, index at +0x8):
 * an InSeq player is readied (cInSeq_ReadyPlayer, cInSeq_GetPlayer on mpInSeq), stored at +0x0 and given the request as
 * user data (cScriptPlayerMain_SetUserData), and the script's lock count (+0x4) goes up. @p userData is stored at +0xC.
 *
 * Likely bug: cBXScriptEngine_CreateNewRequest returns 0 when all 16 requests are in use, and the result is used unchecked.
 *
 * @param idx  the script index
 * @param userData  user data stored with the request
 * @return the request handle (+0x1C), or -1 when the data cannot be had
 */
extern "C" int cBXScriptEngine_RequestAnimationScript(cBXScriptEngine* self, int idx, int userData)
{
    tBXScriptState* scriptState = &(self->mpScriptDatabase)[idx];
    sBXThread21A0* request = (sBXThread21A0*)cBXScriptEngine_CreateNewRequest(self, 1);
    if (((void*)scriptState->pScriptData) == 0) {
        int one = 1;
        request->pending = idx;
        request->state = one;
        request->f12 = one;
        sBxsVEntry21A0* vtableEntry = &((sBxsVEntry21A0*)self->mVtbl)[7];
        if (vtableEntry->fn(vthis(self, *vtableEntry), idx) == 0) {
            request->state = 7;
            cBXScriptEngine_ResetRequest(self, request);
            return -1;
        }
    } else {
        request->slot = idx;
        request->state = 2;
        int id = cInSeq_ReadyPlayer(self->mpInSeq, ((void*)scriptState->pScriptData));
        void* obj = cInSeq_GetPlayer_2(self->mpInSeq, id);
        request->owner = obj;
        cScriptPlayerMain_SetUserData(obj, (int)request);
        scriptState->numLocks++;
    }
    request->arg = userData;
    return request->result;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_PlayScript);
#ifdef SKIP_ASM
extern "C" void* cBXScriptEngine_GetRequest(cBXScriptEngine* self, int a1);
extern "C" void cInSeq_Play(void* cache, void* data, int a2);

/**
 * Starts playing a ready script request and returns its state.
 *
 * Looks the request up with cBXScriptEngine_GetRequest (0 for a free handle). Only a request in state 2 (ready) is started: @p value
 * goes to +0x10 (released after play), and an animation request (type 1) moves to state 3 (play pending) and its player
 * is started with cInSeq_Play on mpInSeq. Any other state is returned unchanged.
 *
 * @param id  the request handle
 * @param value  nonzero to release the request when it finishes playing
 * @param loop  passed to cInSeq_Play
 * @return the request state afterwards, or 0 for a free handle
 */
extern "C" int cBXScriptEngine_PlayScript(cBXScriptEngine* self, int id, int value, int loop)
{
    char* request = (char*)cBXScriptEngine_GetRequest(self, id);
    if (request == 0) {
        return 0;
    }
    int state = *(int*)(request + 0x4);
    if (state != 2) {
        return state;
    }
    *(short*)(request + 0x10) = value;
    if (*(int*)(request + 0x20) == 1) {
        *(int*)(request + 0x4) = 3;
        cInSeq_Play(self->mpInSeq, **(void***)request, loop);
    }
    return *(int*)(request + 0x4);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_StopScript);
#ifdef SKIP_ASM
extern "C" void* cBXScriptEngine_GetRequest(cBXScriptEngine* self, int a1);
extern "C" void cInSeq_Stop(void* cache, void* data);

/**
 * Stops a script request's InSeq player if it is running.
 *
 * Looks the request up with cBXScriptEngine_GetRequest and calls cInSeq_Stop on mpInSeq for its player (+0x0) when the player
 * exists, its mPlayState (+0x20) is not 0 (stopped), and the request is an animation request (type 1).
 *
 * Quirk: the lookup result is not checked, so a free handle would be dereferenced; the callers only pass live requests.
 *
 * @param requestHandle  the request handle
 */
extern "C" void cBXScriptEngine_StopScript(cBXScriptEngine* self, int requestHandle)
{
    char* request = (char*)cBXScriptEngine_GetRequest(self, requestHandle);
    char* obj = *(char**)request;
    if (obj != 0 && *(int*)(obj + 0x20) != 0 && *(int*)(request + 0x20) == 1) {
        cInSeq_Stop(self->mpInSeq, *(void**)obj);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_StopScriptAll);
#ifdef SKIP_ASM
extern "C" void cBXScriptEngine_StopScript(cBXScriptEngine* self, int id);


struct sScriptEngine282390 {
    char pad_0x000[0x2C0];
    tScriptRequestInfo slots[16];
};

/**
 * Stops the script of every script request that has a script object.
 *
 * Walks the 16 mScriptRequests (0x24 bytes each) and, for each whose first word (its script object) is set,
 * calls cBXScriptEngine_StopScript() with the request's id (+0x1C), which stops that script in the InSeq sequencer when it
 * is running.
 */
extern "C" void cBXScriptEngine_StopScriptAll(cBXScriptEngine* self)
{
    for (int i = 0; i < 16; i++) {
        tScriptRequestInfo* slot = &((&self->mScriptRequests[0]))[i];
        if (((int)slot->pPlayer) != 0) {
            cBXScriptEngine_StopScript(self, slot->requestHandle);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ReleaseRequestAll);
#ifdef SKIP_ASM
extern "C" void cBXScriptEngine_ReleaseRequest(void* self, int id);



/**
 * Releases every script request that is still in use.
 *
 * Walks the 16 mScriptRequests and calls cBXScriptEngine_ReleaseRequest() with the id (+0x1C) of each whose word at +0x20 is
 * set; that runs the request type's cleanup, releases its script object and database reference and returns
 * the slot to the free list.
 */
extern "C" void cBXScriptEngine_ReleaseRequestAll(cBXScriptEngine* self)
{
    for (int i = 0; i < 16; i++) {
        tScriptRequestInfo* slot = &((&self->mScriptRequests[0]))[i];
        if (slot->requestType != 0) {
            cBXScriptEngine_ReleaseRequest((self), slot->requestHandle);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ForcePlayUpdate);
#ifdef SKIP_ASM
extern "C" void* cBXScriptEngine_GetRequest(cBXScriptEngine* self, int a1);
extern "C" void cScriptPlayerMain_UpdateFrame(void* p);

/**
 * Forces an immediate frame update of a playing script request.
 *
 * Finds the request record with cBXScriptEngine_GetRequest (On Tour GetRequest). When it exists and its state (+0x4) is 3 or 4 (play
 * pending or playing; cBXScriptEngine_OnInSeqPlay moves 3 to 4), runs cScriptPlayerMain_UpdateFrame (On Tour cScriptPlayerMain::UpdateFrame) on the
 * request's player (+0x0). cSSXScriptEngine_ForcePlayUpdate calls it before updating its camera managers.
 *
 * @param requestHandle  the request handle
 * @return 1 if the player was updated, 0 for an unknown or idle request
 */
extern "C" int cBXScriptEngine_ForcePlayUpdate(cBXScriptEngine* self, int requestHandle)
{
    void* request = cBXScriptEngine_GetRequest(self, requestHandle);
    if (request == 0) {
        return 0;
    }
    int type = *(int*)((char*)request + 0x4);
    if (type == 3 || type == 4) {
        cScriptPlayerMain_UpdateFrame(*(void**)request);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetScriptPlayRate);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBXScriptEngine_GetRequest_v(void) __asm__("cBXScriptEngine_GetRequest");
void cScriptPlayerMain_SetPlaySpeed(void* self, float t);

/**
 * Sets the play rate of a script request, or remembers it until the request's data arrives.
 *
 * The engine and the request handle arrive untouched in the first two argument registers and go straight to
 * cBXScriptEngine_GetRequest. Rates below 0.01 are refused. A request in state 1 (data pending) keeps the rate at +0x18 for
 * cBXScriptEngine_UpdatePendingRequestState to apply; otherwise the rate goes to its player with cScriptPlayerMain_SetPlaySpeed.
 *
 * @param playRate  the new play rate
 * @return 1 when set or stored, 0 for a rate below 0.01 or a free handle
 */
extern "C" int cBXScriptEngine_SetScriptPlayRate(float playRate)
{
    if (playRate < 0.01f) return 0;
    char* request = (char*)cBXScriptEngine_GetRequest_v();
    if (request == 0) return 0;
    if (*(int*)(request + 4) == 1) {
        *(float*)(request + 0x18) = playRate;
        return 1;
    }
    cScriptPlayerMain_SetPlaySpeed(*(void**)request, playRate);
    return 1;
}
#endif
//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetRequestUserData);
#ifdef SKIP_ASM
/**
 * Replaces the user data of the request bound to InSeq player @p obj and returns the old value.
 *
 * The request is the player's mpUserData (+0xC, set by cScriptPlayerMain_SetUserData); its user data word is at +0xC.
 *
 * @param obj  the InSeq player (cScriptPlayerMain)
 * @param val  the new user data
 * @return the previous user data
 */
extern "C" int cBXScriptEngine_SetRequestUserData(cBXScriptEngine* self, char* obj, int val)
{
    char* request = *(char**)(obj + 0xC);
    int old = *(int*)(request + 0xC);
    *(int*)(request + 0xC) = val;
    return old;
}
#endif
//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetRequestUserData1);
#ifdef SKIP_ASM
/**
 * Replaces the user data (+0xC) of script request @p requestHandle and returns the old value.
 *
 * Quirk: the cBXScriptEngine_GetRequest result is not checked, so a free handle would be dereferenced.
 *
 * @param requestHandle  the request handle
 * @param val  the new user data
 * @return the previous user data
 */
extern "C" int cBXScriptEngine_SetRequestUserData1(cBXScriptEngine* self, int requestHandle, int val)
{
    char* request = (char*)cBXScriptEngine_GetRequest(self, requestHandle);
    int old = *(int*)(request + 0xC);
    *(int*)(request + 0xC) = val;
    return old;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ReleaseRequest);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cBXScriptEngine_StopScript(cBXScriptEngine* self, int id);
int cScriptPlayerMain_SetUserData(void* obj, int a);
extern "C" void cInSeq_RecyclePlayer(void* mgr, int handle);
void cBXScriptEngine_ResetRequest(void* self, void* slot);

struct sVEnt_282540 {
    short delta;
    short index;
    void* fn;
};
typedef void (*tVFn1_282540)(void*, int);
typedef void (*tVFn2_282540)(void*, int, int);

struct sTblEnt_282540 {
    int f0;                 // 0x0
    unsigned short refs;    // 0x4
    short pad6;
};

struct sScriptSlot_282540 {
    int* obj;               // 0x00
    int state;               // 0x04
    int tblIdx;             // 0x08
    int fC;                 // 0x0C
    short f10;              // 0x10
    short pad12;
    int f14;                // 0x14
    char pad18[8];
    int type;              // 0x20
};

struct sScriptEngine_282540 {
    char pad0[0x2A8];
    sVEnt_282540* vt;       // 0x2A8
    char pad2AC[4];
    sTblEnt_282540* tbl;    // 0x2B0
    char pad2B4[4];
    void* mgr;              // 0x2B8
};

sScriptSlot_282540* cBXScriptEngine_GetRequest_282540(sScriptEngine_282540* self, int id) __asm__("cBXScriptEngine_GetRequest");

#define VCALL1(SELF, SLOT, A) ((tVFn1_282540)(SELF)->vt[SLOT].fn)((char*)(SELF) + (SELF)->vt[SLOT].delta, A)
#define VCALL2(SELF, SLOT, A, B) ((tVFn2_282540)(SELF)->vt[SLOT].fn)((char*)(SELF) + (SELF)->vt[SLOT].delta, A, B)

// Finishes a script slot: per-type cleanup, release of its object and
// table reference, then returns the slot to the free list.
// PORT: the unit declares this as void(void*, int); defined under an asm label with its real type
extern "C" int cBXScriptEngine_ReleaseRequest_impl(sScriptEngine_282540* self, int id) __asm__("cBXScriptEngine_ReleaseRequest");
/**
 * Releases a script request: stops it, unbinds and recycles its player, drops its script lock, and frees the record.
 *
 * Does nothing but return 6 for a free handle. Clears the release-after-play flag (+0x10), then by state (+0x4): a
 * request still waiting for data (1) calls the engine's virtual at vtable slot 8
 * (cSSXScriptEngine_OnPendingDataCancelled) with the pending index (+0x14) while that script has no data; a request
 * that is starting, playing or paused (3-5) is stopped with cBXScriptEngine_StopScript. It then calls the virtual at slot 12
 * (cSSXScriptEngine_OnReleaseRequest) with the handle and the user data (+0xC). A player (+0x0) gets its user data
 * cleared (cScriptPlayerMain_SetUserData) and goes back to the InSeq pool (cInSeq_RecyclePlayer with its ID). The lock count of the
 * request's script (index at +0x8) in mpScriptDatabase drops, and at zero the virtual at slot 9
 * (cSSXScriptEngine_OnScriptDataReleased) is called. Last, the record is cleared with cBXScriptEngine_ResetRequest (its state 7 store
 * is overwritten there).
 *
 * @param id  the request handle
 * @return always 6 (released)
 */
extern "C" int cBXScriptEngine_ReleaseRequest_impl(sScriptEngine_282540* self, int id)
{
    sScriptSlot_282540* s = cBXScriptEngine_GetRequest_282540(self, id);
    if (s == 0 || s->type == 0) {
        return 6;
    }
    s->f10 = 0;
    switch (s->state) {
    case 1:
        {
            sTblEnt_282540* loadingState = self->tbl + s->f14;
            if (loadingState->f0 == 0) {
                VCALL1(self, 8, s->f14);
            }
        }
        break;
    case 3:
    case 4:
    case 5:
        cBXScriptEngine_StopScript((cBXScriptEngine*)self, id);
        break;
    case 0:
    case 2:
    case 6:
        break;
    }
    VCALL2(self, 12, id, s->fC);
    if (s->obj != 0) {
        cScriptPlayerMain_SetUserData(s->obj, 0);
        cInSeq_RecyclePlayer(self->mgr, *s->obj);
    }
    if (s->tblIdx >= 0) {
        sTblEnt_282540* scriptState = self->tbl + s->tblIdx;
        if (--scriptState->refs == 0) {
            VCALL1(self, 9, s->tblIdx);
        }
    }
    s->state = 7;
    cBXScriptEngine_ResetRequest(self, s);
    return 6;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_PauseAll);
#ifdef SKIP_ASM
extern "C" void cInSeq_PauseAll(int);

/**
 * Pauses every InSeq script player: cInSeq_PauseAll on mpInSeq (+0x2B8).
 *
 * @param engine  the script engine
 */
extern "C" void cBXScriptEngine_PauseAll(cBXScriptEngine* engine) {
    cInSeq_PauseAll(((int)engine->mpInSeq));
}
#endif
//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ResumeAll);
#ifdef SKIP_ASM
extern "C" void cInSeq_ResumeAll(int);

/**
 * Resumes every InSeq script player: cInSeq_ResumeAll on mpInSeq (+0x2B8).
 *
 * @param engine  the script engine
 */
extern "C" void cBXScriptEngine_ResumeAll(cBXScriptEngine* engine) {
    cInSeq_ResumeAll(((int)engine->mpInSeq));
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_PauseScript);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Pause(void* self);

/**
 * Pauses script request @p requestHandle.
 *
 * Calls cScriptPlayerMain_Pause on the request's player (+0x0) and sets the request state (+0x4) to 5 (paused) when the
 * player's mNumPaused (+0x2C) is above 0. The request is indexed directly in mScriptRequests (+0x2C0, 0x24 each)
 * without checking that it is live.
 *
 * @param requestHandle  the request handle (0-15)
 */
extern "C" void cBXScriptEngine_PauseScript(cBXScriptEngine* self, int requestHandle)
{
    char* request = (char*)&self->mScriptRequests[requestHandle];
    cScriptPlayerMain_Pause(*(void**)request);
    if (*(int*)(*(char**)request + 0x2C) > 0) {
        *(int*)(request + 0x4) = 5;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ResumeScript);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Resume(void* self);

/**
 * Resumes script request @p requestHandle.
 *
 * Calls cScriptPlayerMain_Resume on the request's player (+0x0). A paused request (state 5) whose player is no longer
 * paused (mNumPaused, +0x2C, at or below 0) goes back to state 4 (playing) when the player's mPlayState (+0x20) is 3 or
 * 4 (playing or ending), else to state 3 (play pending). The request is indexed directly in mScriptRequests.
 *
 * @param requestHandle  the request handle (0-15)
 */
extern "C" void cBXScriptEngine_ResumeScript(cBXScriptEngine* self, int requestHandle)
{
    char* request = (char*)&self->mScriptRequests[requestHandle];
    cScriptPlayerMain_Resume(*(void**)request);
    if (*(int*)(request + 0x4) == 5) {
        char* player = *(char**)request;
        if (*(int*)(player + 0x2C) <= 0) {
            int playState = *(int*)(player + 0x20);
            if (playState == 3 || playState == 4) {
                *(int*)(request + 0x4) = 4;
            } else {
                *(int*)(request + 0x4) = 3;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetScriptEngine);
#ifdef SKIP_ASM
extern int cBXScriptEngine_mpScriptEngine;

/**
 * Returns the base script engine instance, cBXScriptEngine_mpScriptEngine (set by cBXScriptEngine_cBXScriptEngine, cleared by its
 * destructor).
 *
 * @return the engine, or 0 when none exists
 */
extern "C" int cBXScriptEngine_GetScriptEngine(void) {
    return cBXScriptEngine_mpScriptEngine;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestState);
#ifdef SKIP_ASM
extern "C" void* cBXScriptEngine_GetRequest(cBXScriptEngine* self, int a1);
extern "C" void cBXScriptEngine_UpdatePendingRequestState(cBXScriptEngine* self, tScriptRequestInfo* script);

/**
 * Returns the state of script request @p requestHandle, first promoting it if its data has arrived.
 *
 * A request in state 1 (data pending) is passed to cBXScriptEngine_UpdatePendingRequestState, which makes it ready (state 2) when its script data
 * is attached.
 *
 * @param requestHandle  the request handle
 * @return the request state (+0x4), or 0 for a free handle
 */
extern "C" int cBXScriptEngine_GetRequestState(cBXScriptEngine* self, int requestHandle)
{
    tScriptRequestInfo* request = (tScriptRequestInfo*)((char*)cBXScriptEngine_GetRequest(self, requestHandle));
    if (((char*)request) == 0) {
        return 0;
    }
    if (request->state == 1) {
        cBXScriptEngine_UpdatePendingRequestState(self, (tScriptRequestInfo*)request);
    }
    return request->state;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestScriptIndex);
#ifdef SKIP_ASM
/**
 * Returns the script index of script request @p requestHandle.
 *
 * While the request waits for data (state 1) that is the pending index (+0x14); otherwise the bound index (+0x8).
 *
 * @param requestHandle  the request handle
 * @return the script index, or -1 for a free handle
 */
extern "C" int cBXScriptEngine_GetRequestScriptIndex(cBXScriptEngine* self, int requestHandle)
{
    char* request = (char*)cBXScriptEngine_GetRequest(self, requestHandle);
    if (request == 0) return -1;
    if (*(int*)(request + 4) == 1) return *(int*)(request + 0x14);
    return *(int*)(request + 8);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_IsRequestPlaybackEnding);
#ifdef SKIP_ASM
extern "C" void* cBXScriptEngine_GetRequest(cBXScriptEngine* self, int a1);
extern "C" void cBXScriptEngine_UpdatePendingRequestState(cBXScriptEngine* self, tScriptRequestInfo* script);

/**
 * Returns whether script request @p requestHandle is playing its end.
 *
 * A data-pending request is first updated with cBXScriptEngine_UpdatePendingRequestState. Only a playing or paused request (state 4 or 5) can be
 * ending: then the result is whether its player's mPlayState (+0x20) is 4.
 *
 * @param requestHandle  the request handle
 * @return 1 when ending, else 0 (also for a free handle)
 */
extern "C" int cBXScriptEngine_IsRequestPlaybackEnding(cBXScriptEngine* self, int requestHandle)
{
    tScriptRequestInfo* request = (tScriptRequestInfo*)((char*)cBXScriptEngine_GetRequest(self, requestHandle));
    if (((char*)request) == 0) {
        return 0;
    }
    if (request->state == 1) {
        cBXScriptEngine_UpdatePendingRequestState(self, (tScriptRequestInfo*)request);
    }
    if (request->state != 4 && request->state != 5) {
        return 0;
    }
    return *(int*)(*(char**)request + 0x20) == 4;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ResetInstVars);
#ifdef SKIP_ASM
struct sScriptThread2C0 {
    int a;          // 0x00
    int b;          // 0x04
    int c;          // 0x08
    int d;          // 0x0C
    short e;        // 0x10
    short f;        // 0x12
    int g;          // 0x14
    int h;          // 0x18
    int id;         // 0x1C
    int k;          // 0x20
};
struct sScriptEngine2B8 {
    char pad[0x2b8];
    int count;                      // 0x2B8
    int unk2BC;                     // 0x2BC
    sScriptThread2C0 threads[16];   // 0x2C0
};
/**
 * Resets the BX script engine's instance variables: the InSeq pointers and the 16 script request records.
 *
 * Zeroes mpInSeq and mpInSeqHeap without freeing them (cBXScriptEngine_PurgeBXEngine frees them first). Each
 * 0x24-byte request record gets its index at +0x1C, -1 at +0x14, and 0 in the words at +0x0, +0x4, +0x18 and
 * +0x20 and the halfwords at +0x10 and +0x12; +0x8 and +0xC are left as they were. +0x0 and +0x20 are the flags
 * cBXScriptEngine_StopScriptAll() and cBXScriptEngine_ReleaseRequestAll() test.
 */
extern "C" void cBXScriptEngine_ResetInstVars(sScriptEngine2B8* self)
{
    int i;
    self->count = 0;
    self->unk2BC = 0;
    for (i = 0; i < 16; i++) {
        self->threads[i].id = i;
        self->threads[i].k = 0;
        self->threads[i].a = 0;
        self->threads[i].b = 0;
        self->threads[i].e = 0;
        self->threads[i].f = 0;
        self->threads[i].h = 0;
        self->threads[i].g = -1;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_UpdatePendingRequestState);
#ifdef SKIP_ASM
extern "C" int cInSeq_ReadyPlayer(void* self, void* src);
// PORT: the unit declares cInSeq_GetPlayer(int), but it really takes (self, id).
void* cInSeq_GetPlayer_2(void* self, int id) __asm__("cInSeq_GetPlayer");
// PORT: the setter's mangled signature is (void*, int); a pointer is passed as int.
int cScriptPlayerMain_SetUserData(void* self, int a1);
void cScriptPlayerMain_SetPlaySpeed(void* self, float t);



/**
 * Makes a data-pending script request ready once its script data has been attached.
 *
 * Does nothing while mpScriptDatabase[pending index (+0x14)] has no data. Otherwise binds the index (+0x8), clears the
 * pending index (-1) and flag (+0x12), sets state 2 (ready), readies an InSeq player for the data (cInSeq_ReadyPlayer,
 * cInSeq_GetPlayer on mpInSeq), stores it at +0x0 with the request as its user data (cScriptPlayerMain_SetUserData), applies and clears
 * a stored play rate (+0x18, cScriptPlayerMain_SetPlaySpeed) and raises the script's lock count.
 *
 * @param script  the request record (an mScriptRequests entry)
 */
extern "C" void cBXScriptEngine_UpdatePendingRequestState(cBXScriptEngine* self, tScriptRequestInfo* script)
{
    tScriptRequestInfo* request = script;
    int idx = request->loadingScriptIndex;
    tBXScriptState* scriptState = &(self->mpScriptDatabase)[idx];
    if (((void*)scriptState->pScriptData) == 0) return;
    request->data.animFields.animScriptIndex = idx;
    request->loadingScriptIndex = -1;
    request->state = 2;
    request->loadPending = 0;
    int id = cInSeq_ReadyPlayer(self->mpInSeq, ((void*)scriptState->pScriptData));
    void* obj = cInSeq_GetPlayer_2(self->mpInSeq, id);
    *(void**)&request->pPlayer = obj;
    cScriptPlayerMain_SetUserData(obj, (int)request);
    if (request->playSpeedRequest > 0.0f) {
        cScriptPlayerMain_SetPlaySpeed(((void*)request->pPlayer), request->playSpeedRequest);
        request->playSpeedRequest = 0.0f;
    }
    scriptState->numLocks++;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqPlay);
#ifdef SKIP_ASM
struct sBxsVEntry10 {
    short delta;
    short index;
    void (*fn)(void*, void*, int, int);
};

/**
 * Sequencer callback for a script player that starts: notifies the owner and marks its request playing.
 *
 * The player's request record is at player+0xC. Calls virtual slot 10 of the engine's vtable at +0x2A8 with the player
 * and the request's +0x8 and +0xC words, then sets the request state (+0x4) to 4 (playing). On Tour asserts the
 * sequencer exists and the request was play-pending (3); retail does not. cSSXScriptEngine_OnInSeqPlay forwards here.
 *
 * @param pPlayer  the cScriptPlayerMain that started
 */
extern "C" void cBXScriptEngine_OnInSeqPlay(cBXScriptEngine* self, cScriptPlayerMain* pPlayer)
{
    char* request = (char*)pPlayer->mpUserData;
    sBxsVEntry10* vt = (sBxsVEntry10*)self->mVtbl;
    vt[10].fn(vthis(self, vt[10]), pPlayer, *(int*)(request + 0x8), *(int*)(request + 0xC));
    *(int*)(request + 0x4) = 4;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqStop);
#ifdef SKIP_ASM

extern "C" void cBXScriptEngine_ReleaseRequest(void* self, int id);

/**
 * Sequencer callback for a script player that stops: notifies the owner, marks the request stopped and recycles it if
 * asked.
 *
 * Calls virtual slot 11 of the engine's vtable at +0x2A8 with the player and its request's +0x8 / +0xC words, sets the
 * request state (+0x4) to 2 and, when the request's short at +0x10 is set (probably PlayScript's autoRecycle), releases
 * the request by its handle (+0x1C) with cBXScriptEngine_ReleaseRequest (On Tour ReleaseRequest). cSSXScriptEngine_OnInSeqStop forwards
 * here.
 *
 * @param msg  the cScriptPlayerMain that stopped
 */
extern "C" void cBXScriptEngine_OnInSeqStop(cBXScriptEngine* self, cScriptPlayerMain* msg)
{
    char* request = (char*)msg->mpUserData;
    sVEntry<void(void*, void*, int, int)>* vt = (sVEntry<void(void*, void*, int, int)>*)self->mVtbl;
    vt[11].fn(vthis(self, vt[11]), msg, *(int*)(request + 0x8), *(int*)(request + 0xC));
    *(int*)(request + 0x4) = 2;
    if (*(short*)(request + 0x10) != 0) {
        cBXScriptEngine_ReleaseRequest(self, *(int*)(request + 0x1C));
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqRequestEntityCommInterface);
#ifdef SKIP_ASM
extern "C" int cScriptDatabaseObject_GetAvailableEntities(void* self, int key, void** out, int max);

class cBxsHandler2A80 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual int IsMatchingEntity(void* ctx, int arg);
};

/**
 * Finds the script entity that should drive InSeq player entity @p ctx.
 *
 * Lists up to 30 candidate entities for the entity's owner player (its mpOwner's mID) with cScriptDatabaseObject_GetAvailableEntities and returns
 * the first whose virtual at slot 5 (the entity match test, e.g. cScriptObjRider_IsMatchingEntity) accepts (@p ctx,
 * @p entityType).
 *
 * @param ctx  the InSeq player entity (cScriptPlayerEntity)
 * @param entityType  the entity type passed to the match test
 * @return the matching script entity, or 0
 */
extern "C" cBxsHandler2A80* cBXScriptEngine_OnInSeqRequestEntityCommInterface(cBXScriptEngine* self, cScriptPlayerEntity* ctx, int entityType)
{
    cBxsHandler2A80* buf[30];
    cBxsHandler2A80* found = 0;
    int n = cScriptDatabaseObject_GetAvailableEntities(self, *(int*)ctx->mpOwner, (void**)buf, 30);
    for (int i = 0; i < n; i++) {
        cBxsHandler2A80* candidate = buf[i];
        if (candidate->IsMatchingEntity(ctx, entityType)) {
            found = candidate;
            break;
        }
    }
    return found;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqRemoteScriptPlayerReady);
#ifdef SKIP_ASM
struct sBxsThreadHdr {
    void* owner;    // 0x0
    int state;      // 0x4
    int handle;     // 0x8
};

extern "C" sBxsThreadHdr* cBXScriptEngine_CreateNewRequest(cBXScriptEngine* self, int a1);
// PORT: the setter's mangled signature is (void*, int); a pointer is passed as int.
int cScriptPlayerMain_SetUserData(void* self, int a1);

/**
 * Wraps a remotely started InSeq player in a new ready script request.
 *
 * Takes a request of type 1 (cBXScriptEngine_CreateNewRequest), sets state 2 (ready), no script index (-1 at +0x8) and @p obj as its
 * player, and binds the request as the player's user data (cScriptPlayerMain_SetUserData).
 *
 * Likely bug: the request is used unchecked when all 16 are in use.
 *
 * @param obj  the InSeq player (cScriptPlayerMain)
 */
extern "C" void cBXScriptEngine_OnInSeqRemoteScriptPlayerReady(cBXScriptEngine* self, cScriptPlayerMain* obj)
{
    sBxsThreadHdr* request = cBXScriptEngine_CreateNewRequest(self, 1);
    request->state = 2;
    request->handle = -1;
    request->owner = ((void*)obj);
    cScriptPlayerMain_SetUserData(obj, (int)request);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqRemoteScriptPlayerDiscard);
#ifdef SKIP_ASM
void cBXScriptEngine_ResetRequest(void* self, void* v);

/**
 * Frees the script request bound to a discarded InSeq player.
 *
 * Takes the request from the player's mpUserData (+0xC), sets its state to 7 and resets it with cBXScriptEngine_ResetRequest.
 *
 * @param ctx  the InSeq player (cScriptPlayerMain)
 */
extern "C" void cBXScriptEngine_OnInSeqRemoteScriptPlayerDiscard(cBXScriptEngine* self, cScriptPlayerMain* ctx)
{
    void* request = ctx->mpUserData;
    *(int*)((char*)request + 0x4) = 7;
    cBXScriptEngine_ResetRequest(self, request);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestUserData__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the user data (+0xC) of the script request bound to InSeq player @p player (its mpUserData, +0xC).
 *
 * @param player  the InSeq player (cScriptPlayerMain)
 * @return the request's user data
 */
int cBXScriptEngine_GetRequestUserData(void* self, void* player)
{
    return *(int*)((char*)*(void**)((char*)player + 0xc) + 0xc);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestUserData1);
#ifdef SKIP_ASM
/**
 * Returns the user data (+0xC) of script request @p requestHandle.
 *
 * Quirk: the cBXScriptEngine_GetRequest result is not checked, so a free handle would be dereferenced.
 *
 * @param requestHandle  the request handle
 * @return the request's user data
 */
extern "C" int cBXScriptEngine_GetRequestUserData1(cBXScriptEngine* self, int requestHandle)
{
    return *(int*)((char*)cBXScriptEngine_GetRequest(self, requestHandle) + 0xC);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestHandleFromPlayer__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the handle (+0x1C) of the script request bound to InSeq player @p player (its mpUserData, +0xC).
 *
 * cSSXScriptEngine's InSeq hooks (cSSXScriptEngine_OnInSeqPlay and others) use it to find the NIS list entry.
 *
 * @param player  the InSeq player (cScriptPlayerMain)
 * @return the request handle
 */
int cBXScriptEngine_GetRequestHandleFromPlayer(void* self, void* player)
{
    return *(int*)((char*)*(void**)((char*)player + 0xc) + 0x1c);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestPlayerByReqHandle);
#ifdef SKIP_ASM
extern "C" void* cBXScriptEngine_GetRequest(cBXScriptEngine* self, int a1);

/**
 * Returns the script player of a request handle.
 *
 * cBXScriptEngine_GetRequest (On Tour GetRequest) finds the request record; its first word is the cScriptPlayerMain. The tNISList
 * code and cSSXScriptEngine_ForcePlayUpdate use it to reach a request's script header.
 *
 * @param requestHandle  the request handle
 * @return the request's player (an int in this view), or 0 for an unknown handle
 */
extern "C" int cBXScriptEngine_GetRequestPlayerByReqHandle(cBXScriptEngine* self, int requestHandle)
{
    int* request = (int*)cBXScriptEngine_GetRequest(self, requestHandle);
    if (request == 0) {
        return 0;
    }
    return *request;
}
#endif

extern "C" void* cInSeq_GetPlayer(int);

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestPlayerByPlayerID__FPv);
#ifdef SKIP_ASM
/**
 * Returns the sequencer's script player for a player ID.
 *
 * Calls cInSeq_GetPlayer on the engine's sequencer (+0x2B8, On Tour mpInSeq). The C declares only the engine: the
 * player ID arrives in $a1 and passes through to cInSeq_GetPlayer untouched (cScriptObjCameraManager_OnStartObjectUse binds a two-argument alias
 * to call it).
 *
 * @return the cScriptPlayerMain cInSeq_GetPlayer returns
 */
void* cBXScriptEngine_GetRequestPlayerByPlayerID(void* vself)
{
    cBXScriptEngine* self = (cBXScriptEngine*)vself;
    return cInSeq_GetPlayer((int)self->mpInSeq);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_CreateNewRequest);
#ifdef SKIP_ASM
struct sBxsThreadSlot2C0 {
    sBxsThreadHdr hdr; // 0x00
    int unk0C;         // 0x0C
    short unk10;       // 0x10
    short unk12;       // 0x12
    int unk14;         // 0x14
    int unk18;         // 0x18
    int id;            // 0x1C
    int type;          // 0x20
};

struct sBxsEngineThreads {
    char pad[0x2C0];
    sBxsThreadSlot2C0 threads[16]; // 0x2C0
};

/**
 * Claims the first free script request record (request type 0 at +0x20) of the 16 in mScriptRequests.
 *
 * Stores @p requestType; an animation request (type 1) also gets no script index (-1 at +0x8).
 *
 * @param requestType  the request type (1 = animation)
 * @return the record, or 0 when all 16 are in use
 */
extern "C" sBxsThreadHdr* cBXScriptEngine_CreateNewRequest(cBXScriptEngine* self, int requestType)
{
    sBxsEngineThreads* engine = (sBxsEngineThreads*)self;
    int i = 0;
    do {
        if (engine->threads[i].type == 0) {
            engine->threads[i].type = requestType;
            switch (requestType) {
            case 1:
                engine->threads[i].hdr.handle = -1;
                break;
            }
            return &engine->threads[i].hdr;
        }
        i++;
    } while (i < 16);
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ResetRequest__FPvT0);
#ifdef SKIP_ASM
/**
 * Clears a script request record so it is free again.
 *
 * Zeroes the player (+0x0), state (+0x4), type (+0x20), release and pending flags (+0x10, +0x12), stored play rate
 * (+0x18) and user data (+0xC), and sets the pending index (+0x14) to -1. The script index (+0x8) and handle (+0x1C)
 * stay.
 *
 * @param request  the request record
 */
void cBXScriptEngine_ResetRequest(void* self, void* request)
{
    *(int*)((char*)request + 0x0) = 0;
    *(int*)((char*)request + 0x4) = 0;
    *(int*)((char*)request + 0x20) = 0;
    *(short*)((char*)request + 0x10) = 0;
    *(short*)((char*)request + 0x12) = 0;
    *(int*)((char*)request + 0x14) = -1;
    *(int*)((char*)request + 0x18) = 0;
    *(int*)((char*)request + 0xc) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequest);
#ifdef SKIP_ASM
/**
 * Returns script request record @p requestHandle (mScriptRequests, +0x2C0, 0x24 each) when it is in use (type at +0x20
 * nonzero).
 *
 * The handle is not range-checked.
 *
 * @param requestHandle  the request handle (0-15)
 * @return the record, or 0 when it is free
 */
extern "C" void* cBXScriptEngine_GetRequest(cBXScriptEngine* self, int requestHandle)
{
    char* request = (char*)&self->mScriptRequests[requestHandle];
    return *(int*)(request + 0x20) != 0 ? request : 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_cScriptObjTransition);
#ifdef SKIP_ASM
void* cScriptObject_cScriptObject(void* self, int type);
extern void* cScriptObjTransition_vtbl[];

/**
 * Constructs a script transition object with no transition running.
 *
 * Runs cScriptObject_cScriptObject with object type 2, installs cScriptObjTransition_vtbl (+0xC), clears the camera
 * manager (+0x10), sets the player ID (+0x14) to -1 and the transition (+0x18) to 9 (none), and zeroes the in, hold and
 * out times (+0x1C-+0x24), the colour (+0x28) and the clock (+0x2C).
 *
 * @return @p self
 */
extern "C" void* cScriptObjTransition_cScriptObjTransition(cScriptObjTransition* self)
{
    cScriptObject_cScriptObject(self, 2);
    self->mVtbl = cScriptObjTransition_vtbl;
    self->mpCameraManager = 0;
    *(int*)&self->mInfo.currTime = 0;
    self->mPlayerID = -1;
    self->mInfo.transition = 9;
    *(int*)&self->mInfo.inTime = 0;
    *(int*)&self->mInfo.holdTime = 0;
    *(int*)&self->mInfo.outTime = 0;
    self->mInfo.transitionColour = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_Update);
#ifdef SKIP_ASM
// PORT: the callee also forwards four float args ($f12-$f15); bound to its (void*, int, int) symbol.
void* cScriptObjTransition_SetTransition_f4(void* self, int a1, int a2, float f0, float f1, float f2, float f3) __asm__("cScriptObjTransition_SetTransition__FPvii");

/**
 * Advances a running transition and ends it once its time is up.
 *
 * While a player owns the transition (ID at +0x14 >= 0), adds @p dt / 60 to the clock (+0x2C); once the clock reaches
 * the sum of the in, hold and out times (+0x1C, +0x20, +0x24) less 0.01, calls cScriptObjTransition_SetTransition with transition 9 (none).
 *
 * @param dt  the frame delta in frames
 */
extern "C" void cScriptObjTransition_Update(cScriptObjTransition* self, float dt)
{
    int id = self->mPlayerID;
    if (id >= 0) {
        float currTime = self->mInfo.currTime + dt * 0.01666666753590107f;
        self->mInfo.currTime = currTime;
        if (self->mInfo.inTime + self->mInfo.holdTime + self->mInfo.outTime - 0.009999999776482582f <= currTime) {
            cScriptObjTransition_SetTransition_f4(self, id, 9, 0.0f, 0.0f, 0.0f, 0.0f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_SetCameraManager__FPvi);
#ifdef SKIP_ASM
/**
 * Sets a script transition object's camera manager (+0x10).
 *
 * @param val  the camera manager (cScriptObjCameraManager)
 */
void cScriptObjTransition_SetCameraManager(void* vself, int val)
{
    cScriptObjTransition* self = (cScriptObjTransition*)vself;
    self->mpCameraManager = (cScriptObjCameraManager*)val;
}
#endif

extern "C" void* cScriptObjTransition_SetTransition1(void*, int, int, int);

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_SetTransition__FPvii);
#ifdef SKIP_ASM
/**
 * Starts or ends a transition, keeping the current colour: calls cScriptObjTransition_SetTransition1 with the colour at +0x28.
 *
 * The four float arguments (current frame, out, in and hold frames) are not declared here; they pass through untouched
 * in their registers.
 *
 * @param playerID  the player that owns the transition
 * @param transition  the transition kind (9 = none)
 * @return whatever cScriptObjTransition_SetTransition1 leaves in the return register (it returns nothing)
 */
void* cScriptObjTransition_SetTransition(void* vself, int playerID, int transition)
{
    cScriptObjTransition* self = (cScriptObjTransition*)vself;
    return cScriptObjTransition_SetTransition1(self, playerID, transition, self->mInfo.transitionColour);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_SetTransition1);
#ifdef SKIP_ASM
struct sBxsVEntry2DD0 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sBxsFade2DD0 {
    char pad_0x0[0xC];
    sBxsVEntry2DD0* vtable;     // 0xC
    char pad_0x10[0x4];
    int id;                     // 0x14
    int kind;                   // 0x18
    float rateA;                // 0x1C
    float rateB;                // 0x20
    float rateC;                // 0x24
    int arg;                    // 0x28
    int time;                   // 0x2C
};

// PORT: the unit declares cScriptObjTransition_SetTransition1(void*, int, int, int) returning void*; the body also takes four floats and returns nothing.
extern "C" void cScriptObjTransition_SetTransition1_impl(sBxsFade2DD0* self, int id, int kind, int arg, float t, float c, float a, float b) __asm__("cScriptObjTransition_SetTransition1");
/**
 * Ends the running transition and starts a new one, or just ends it.
 *
 * When a transition is running (kind at +0x18 not 9) the object's virtual at slot 5 (cScriptObject_OnEndObjectUse in
 * cScriptObjTransition_vtbl) is called with the owning player (+0x14). When @p kind is 9, or the in, hold and out
 * frames together do not exceed @p currFrame, everything is cleared: kind 9, player -1, times and clock 0. Otherwise
 * the new kind, colour (+0x28) and player are stored, the clock restarts at 0, the in, hold and out frames are stored
 * as seconds (/ 60, at +0x1C, +0x20, +0x24), and the virtual at slot 4 (cScriptObject_OnStartObjectUse__FPv) is called
 * with the player.
 *
 * @param id  the player that owns the transition
 * @param kind  the transition kind (9 = none)
 * @param transitionColour  the transition colour
 * @param currFrame  the current frame within the transition; only decides whether it is already over
 * @param outFrames  the fade-out length in frames
 * @param inFrames  the fade-in length in frames
 * @param holdFrames  the hold length in frames
 */
extern "C" void cScriptObjTransition_SetTransition1_impl(sBxsFade2DD0* self, int id, int kind, int transitionColour, float currFrame, float outFrames, float inFrames, float holdFrames)
{
    if (self->kind != 9) {
        sBxsVEntry2DD0* vt = self->vtable;
        vt[5].fn(vthis(self, vt[5]), self->id);
    }
    if (kind == 9 || inFrames + holdFrames + outFrames <= currFrame) {
        self->kind = 9;
        self->rateA = 0.0f;
        self->rateB = 0.0f;
        self->id = -1;
        self->rateC = 0.0f;
        self->time = 0;
    } else {
        self->kind = kind;
        self->arg = transitionColour;
        self->time = 0;
        self->id = id;
        self->rateA = inFrames * 0.01666666753590107f;
        self->rateB = holdFrames * 0.01666666753590107f;
        self->rateC = outFrames * 0.01666666753590107f;
        sBxsVEntry2DD0* vt = self->vtable;
        vt[4].fn(vthis(self, vt[4]), self->id);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_IsInUseByPlayer);
#ifdef SKIP_ASM
/**
 * Returns whether the transition is owned by player @p playerID (ID at +0x14).
 *
 * cSSXScriptEngine_IsObjInUseByPlayer calls it.
 *
 * @param playerID  the player ID
 * @return 1 when owned by that player, else 0
 */
extern "C" int cScriptObjTransition_IsInUseByPlayer(cScriptObjTransition* self, int playerID)
{
    return self->mPlayerID == playerID;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_PauseObjScript);
#ifdef SKIP_ASM
class cBxScriptVirt {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    virtual void v01();
    virtual void PauseObjScript();
    virtual void ResumeObjScript();
};

/**
 * Pauses the transition object's script: calls the second virtual of the object at +0x10 (On Tour mpCameraManager,
 * a cScriptEntity; presumably PauseEntityScript).
 */
extern "C" void cScriptObjTransition_PauseObjScript(cScriptObjTransition* self)
{
    ((cBxScriptVirt*)self->mpCameraManager)->PauseObjScript();
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_ResumeObjScript);
#ifdef SKIP_ASM
/**
 * Resumes the transition object's script: calls the third virtual of the object at +0x10 (On Tour mpCameraManager,
 * a cScriptEntity; presumably ResumeEntityScript).
 */
extern "C" void cScriptObjTransition_ResumeObjScript(cScriptObjTransition* self)
{
    ((cBxScriptVirt*)self->mpCameraManager)->ResumeObjScript();
}
#endif

extern void* cScriptEntity_vtbl[];

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_cScriptEntity__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a script entity: installs cScriptEntity's vtable and initialises its two fields.
 *
 * The word at +0x4 is set to -1 and the word at +0x8 to 0.
 *
 * @return @p self
 */
void* cScriptEntity_cScriptEntity(void* vself)
{
    cScriptEntity* self = (cScriptEntity*)vself;
    *(void***)self = cScriptEntity_vtbl;
    self->mDBID = -1;
    self->mpEntity = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Returns whether a script entity is bound to an InSeq player entity (+0x8, set while a script has it locked).
 *
 * @return 1 when bound, else 0
 */
int cScriptEntity_IsPlaying(void* vself)
{
    cScriptEntity* self = (cScriptEntity*)vself;
    return ((int)self->mpEntity != 0);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_DetachEntity);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_SetEntityComm(void* p, int a1);

/**
 * Cuts the bound InSeq player entity (+0x8) off from this script entity: cScriptPlayerEntity_SetEntityComm(entity, 0).
 *
 * Does nothing when no entity is bound.
 */
extern "C" void cScriptEntity_DetachEntity(cScriptEntity* self)
{
    void* entity = self->mpEntity;
    if (entity != 0) {
        cScriptPlayerEntity_SetEntityComm(entity, 0);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_OnInSeqLock);
#ifdef SKIP_ASM
extern "C" void* cScriptDatabaseObject_GetDatabaseObject(void);
extern "C" void cScriptDatabaseObject_MarkEntityAsUsed(void* mgr, void* self, int a2);

/**
 * Locks this script entity to InSeq player entity @p entity.
 *
 * Stores @p entity at +0x8 and registers the lock with the script database object (cScriptDatabaseObject_GetDatabaseObject, cScriptDatabaseObject_mpDatabaseObject) through
 * cScriptDatabaseObject_MarkEntityAsUsed, under the ID (mID) of the entity's owner player (mpOwner, +0x8).
 *
 * @param entity  the InSeq player entity (cScriptPlayerEntity)
 */
extern "C" void cScriptEntity_OnInSeqLock(cScriptEntity* self, cScriptEntity* entity)
{
    self->mpEntity = (cScriptPlayerEntity*)entity;
    cScriptDatabaseObject_MarkEntityAsUsed(cScriptDatabaseObject_GetDatabaseObject(), self, *(int*)entity->mpEntity);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_OnInSeqRelease);
#ifdef SKIP_ASM
extern "C" void* cScriptDatabaseObject_GetDatabaseObject(void);
extern "C" void cScriptDatabaseObject_MarkEntityAsFree(void* mgr, void* obj);

/**
 * Releases this script entity's InSeq lock: unregisters it from the script database object (cScriptDatabaseObject_MarkEntityAsFree on
 * cScriptDatabaseObject_GetDatabaseObject()) and clears the bound entity (+0x8).
 */
extern "C" void cScriptEntity_OnInSeqRelease(cScriptEntity* self)
{
    cScriptDatabaseObject_MarkEntityAsFree(cScriptDatabaseObject_GetDatabaseObject(), self);
    self->mpEntity = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_PauseEntityScript);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Pause(void* self);

/**
 * Pauses the script driving this entity: cScriptPlayerMain_Pause on the bound entity's owner player (mpOwner, +0x8).
 *
 * Does nothing when no entity is bound (+0x8).
 */
extern "C" void cScriptEntity_PauseEntityScript(cScriptEntity* self)
{
    cScriptPlayerEntity* entity = self->mpEntity;
    if (((void*)entity) != 0) {
        cScriptPlayerMain_Pause(entity->mpOwner);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_ResumeEntityScript);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Resume(void* self);

/**
 * Resumes the script driving this entity: cScriptPlayerMain_Resume on the bound entity's owner player (mpOwner, +0x8).
 *
 * Does nothing when no entity is bound (+0x8).
 */
extern "C" void cScriptEntity_ResumeEntityScript(cScriptEntity* self)
{
    cScriptPlayerEntity* entity = self->mpEntity;
    if (((void*)entity) != 0) {
        cScriptPlayerMain_Resume(entity->mpOwner);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntityCamera_cScriptEntityCamera);
#ifdef SKIP_ASM
extern void* cScriptEntityCamera_vtbl[];
void* cScriptEntity_cScriptEntity(void* self);

/**
 * Constructs a camera script entity: cScriptEntity_cScriptEntity, then the vtable cScriptEntityCamera_vtbl, sub-type (+0xC) 9 and no
 * camera manager (+0x10).
 *
 * @return @p self
 */
extern "C" void* cScriptEntityCamera_cScriptEntityCamera(cScriptEntityCamera* self)
{
    cScriptEntity_cScriptEntity(self);
    *(void***)self = cScriptEntityCamera_vtbl;
    self->mSubType = 9;
    self->mpCamManager = 0;
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntityCamera_OnInSeqLock);
#ifdef SKIP_ASM
extern "C" void cScriptEntity_OnInSeqLock(cScriptEntity* self, cScriptEntity* a1);
int cScriptPlayerEntity_GetType(void* self);
char* GetSSXCameraManualHeader(void* self);
char* GetSSXCameraTargetHeader(void* self);
char* GetSSXCameraSubjectHeader(void* self);
char* GetSSXCameraAlgorithmicHeader(void* self);

/**
 * Locks this camera script entity to InSeq player entity @p obj and takes its camera sub-type from the entity's data.
 *
 * Runs the base lock (cScriptEntity_OnInSeqLock), then reads the signed sub-type byte into +0xC by the entity's type
 * (cScriptPlayerEntity_GetType): types 0-2 from +0x21 of the header from GetSSXCameraManualHeader, GetSSXCameraTargetHeader or GetSSXCameraSubjectHeader, type 3 from
 * +0x1C of GetSSXCameraAlgorithmicHeader's. Other types leave it unchanged.
 *
 * @param obj  the InSeq player entity (cScriptPlayerEntity)
 */
extern "C" void cScriptEntityCamera_OnInSeqLock(cScriptEntityCamera* self, void* obj)
{
    cScriptEntity_OnInSeqLock((cScriptEntity*)self, (cScriptEntity*)obj);
    switch (cScriptPlayerEntity_GetType(obj)) {
    case 0:
        self->mSubType = *(signed char*)(GetSSXCameraManualHeader(obj) + 0x21);
        break;
    case 1:
        self->mSubType = *(signed char*)(GetSSXCameraTargetHeader(obj) + 0x21);
        break;
    case 2:
        self->mSubType = *(signed char*)(GetSSXCameraSubjectHeader(obj) + 0x21);
        break;
    case 3:
        self->mSubType = *(signed char*)(GetSSXCameraAlgorithmicHeader(obj) + 0x1C);
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntityCamera_OnInSeqRelease);
#ifdef SKIP_ASM
extern "C" void cScriptEntity_OnInSeqRelease(cScriptEntity* self);

/**
 * Releases this camera script entity's InSeq lock (cScriptEntity_OnInSeqRelease) and resets its sub-type (+0xC) to 9.
 */
extern "C" void cScriptEntityCamera_OnInSeqRelease(cScriptEntityCamera* self)
{
    cScriptEntity_OnInSeqRelease((cScriptEntity*)self);
    self->mSubType = 9;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntityCamera_IsMatchingEntity);
#ifdef SKIP_ASM
// PORT: cSSXScriptEngine_GetScriptSetupInfo is declared (void*) but takes (self, idx).
void* cSSXScriptEngine_GetScriptSetupInfo_2(void* self, int idx) __asm__("cSSXScriptEngine_GetScriptSetupInfo__FPv");

/**
 * Returns whether InSeq player entity @p entity belongs to this camera's view.
 *
 * Types 4 and above never match. Otherwise compares word +0xC of the request user data of the entity's owner player
 * (cSSXScriptEngine_GetScriptSetupInfo on ScriptEngine) with the view index (+0xF8) of the camera manager (+0x10).
 *
 * @param entity  the InSeq player entity (cScriptPlayerEntity)
 * @param n  the entity type
 * @return 1 on a match, else 0
 */
extern "C" int cScriptEntityCamera_IsMatchingEntity(cScriptEntityCamera* self, cScriptPlayerEntity* entity, int n)
{
    if (n >= 4) {
        return 0;
    }
    return *(int*)((char*)cSSXScriptEngine_GetScriptSetupInfo_2(ScriptEngine, (int)entity->mpOwner) + 0xC) ==
           *(int*)((char*)self->mpCamManager + 0xF8);
}
#endif

extern void* cScriptObject_vtbl[];

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_cScriptObject__FPvi);
#ifdef SKIP_ASM
/**
 * Constructs a script object of a given type that is not yet registered in the script database.
 *
 * Stores cScriptObject's vtable at +0xC, @p objType at +0x0, -1 (no database handle) at +0x4 and 0 at +0x8.
 *
 * @param objType  the object's script type
 * @return @p self
 */
void* cScriptObject_cScriptObject(void* self, int objType)
{
    *(void***)((char*)self + 0xc) = cScriptObject_vtbl;
    *(int*)self = objType;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject__cScriptObject);
#ifdef SKIP_ASM
extern "C" void cScriptObject_DeInitScriptObject(cScriptObject* self);
void operator_delete(int* ptr);

/**
 * Destroys a script object, first removing it from the script database if it is still registered.
 *
 * Restores cScriptObject's vtable (at +0xC). When the database handle at +0x4 is neither -1 nor 0 it calls
 * cScriptObject_DeInitScriptObject() and sets the handle to -1.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptObject__cScriptObject(cScriptObject* self, int flags)
{
    self->mVtbl = cScriptObject_vtbl;
    if (self->mObjID != -1 && self->mObjID != 0) {
        cScriptObject_DeInitScriptObject(self);
        self->mObjID = -1;
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_InitScriptObject);
#ifdef SKIP_ASM
struct sBxsVEntryA {
    short delta;
    short index;
    int (*fn)(void*, void*);
};

/**
 * Registers a script object with the script database and stores the handle it gets.
 *
 * Calls virtual slot 2 of the database object returned by cScriptDatabaseObject_GetDatabaseObject (On Tour
 * cScriptDatabaseObject::GetDatabaseObject) through its vtable at +0x2A8, passing the object, and stores the result in
 * the object's database handle (+0x4, On Tour mObjID); cScriptObject_DeInitScriptObject undoes it with slot 3. On Tour
 * asserts the handle is >= 0 afterwards; retail does not. cSSXScriptEngine_Load registers its script objects with it,
 * and cRider_cRider the rider's.
 */
extern "C" void cScriptObject_InitScriptObject(cScriptObject* self)
{
    char* mgr = (char*)cScriptDatabaseObject_GetDatabaseObject();
    sBxsVEntryA* vt = *(sBxsVEntryA**)(mgr + 0x2A8);
    self->mObjID = vt[2].fn(vthis(mgr, vt[2]), self);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_DeInitScriptObject);
#ifdef SKIP_ASM
struct sBxsVEntryB {
    short delta;
    short index;
    int (*fn)(void*, int);
};

/**
 * Removes the script object from the script engine's object database and marks it unregistered.
 *
 * Calls virtual slot 3 of the global script engine (cScriptDatabaseObject_GetDatabaseObject()) through its cScriptDatabaseObject vtable at
 * +0x2A8, passing the object's database handle (+0x4); cScriptDatabaseObject_RemoveObjectAll() uses the same
 * slot to remove each entry. The handle is then set to -1.
 */
extern "C" void cScriptObject_DeInitScriptObject(cScriptObject* self)
{
    char* mgr = (char*)cScriptDatabaseObject_GetDatabaseObject();
    sBxsVEntryB* vt = *(sBxsVEntryB**)(mgr + 0x2A8);
    vt[3].fn(vthis(mgr, vt[3]), self->mObjID);
    self->mObjID = -1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_PauseObjScript);
#ifdef SKIP_ASM
class cScriptOwner3320 {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int GetNumScriptEntities();
    virtual void* GetScriptEntity(int i);
};

/**
 * Pauses the scripts of every script entity the object owns.
 *
 * Loops over the object's virtual GetNumScriptEntities() (slot 6) and calls cScriptEntity_PauseEntityScript (On Tour
 * cScriptEntity::PauseEntityScript) on each virtual GetScriptEntity(i) (slot 7). It is the base implementation that
 * classes without their own PauseObjScript inherit (cScriptObjRider's vtable D_00482018). On Tour asserts each entity
 * is non-null; retail does not.
 */
extern "C" void cScriptObject_PauseObjScript(cScriptObject* self)
{
    int i = 0;
    int n = ((cScriptOwner3320*)self)->GetNumScriptEntities();
    for (; i < n; i++) {
        cScriptEntity_PauseEntityScript((cScriptEntity*)((cScriptOwner3320*)self)->GetScriptEntity(i));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_ResumeObjScript);
#ifdef SKIP_ASM
class cScriptOwner33A8 {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int GetNumScriptEntities();
    virtual void* GetScriptEntity(int i);
};

/**
 * Resumes the scripts of every script entity the object owns.
 *
 * Loops over the object's virtual GetNumScriptEntities() (slot 6) and calls cScriptEntity_ResumeEntityScript (On Tour
 * cScriptEntity::ResumeEntityScript) on each virtual GetScriptEntity(i) (slot 7); the base implementation that
 * cScriptObjRider (vtable D_00482018) inherits. The counterpart of cScriptObject_PauseObjScript.
 */
extern "C" void cScriptObject_ResumeObjScript(cScriptObject* self)
{
    int i = 0;
    int n = ((cScriptOwner33A8*)self)->GetNumScriptEntities();
    for (; i < n; i++) {
        cScriptEntity_ResumeEntityScript((cScriptEntity*)((cScriptOwner33A8*)self)->GetScriptEntity(i));
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_OnStartObjectUse__FPv);
#ifdef SKIP_ASM
/**
 * Marks a script object as taken by one more user: increments its use count.
 *
 * Adds 1 to the in-use count at +0x8 (On Tour mInUse). The player ID argument is ignored, so the C declares only the
 * object. The subclass overrides (cScriptObjCamera_OnStartObjectUse, cScriptObjRider_OnStartObjectUse, cScriptObjEvent_OnStartObjectUse, cScriptObjCameraManager_OnStartObjectUse) call
 * it on their cScriptObject part.
 *
 * @return the new use count (left in $v0 by the code; On Tour's method is void)
 */
void* cScriptObject_OnStartObjectUse(void* vself)
{
    cScriptObject* self = (cScriptObject*)vself;
    void* useCount = (char*)self->mInUse + 0x1;
    self->mInUse = (int)useCount;
    return useCount;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_OnEndObjectUse__FPv);
#ifdef SKIP_ASM
/**
 * Drops a script object's use count (+0x8) by one.
 *
 * @return the new use count
 */
void* cScriptObject_OnEndObjectUse(void* vself)
{
    cScriptObject* self = (cScriptObject*)vself;
    void* useCount = (char*)self->mInUse - 0x1;
    self->mInUse = (int)useCount;
    return useCount;
}
#endif

