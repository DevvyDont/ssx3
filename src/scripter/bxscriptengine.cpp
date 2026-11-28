#include "vtable.h"
#include "common.h"
#include "scripter/scriptdatabaseobject.h"
#include "scripter/scriptentitycam.h"
#include "scripter/scriptobjtransition.h"
#include "scripter/scriptobj.h"
#include "scripter/scriptentity.h"
#include "inseq/scriptplayerentity.h"
#include "inseq/scriptplayermain.h"
#include "scripter/bxscriptengine.h"
#include "inseq/inseq.h"

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetupBXEngine);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* __6cInSeqPvT1Uii(void* obj, void* buf, int size, int a3, void* out);
extern char str_InSeqHeap[];
extern char str_BXScriptDatabase[];
extern char str_InSeq[];

/**
 * Sets up the BX script engine: allocates the InSeq sequencer with its heap and an empty script database.
 *
 * Allocates @p heapSize bytes of "InSeqHeap" (mpInSeqHeap), creates the 0x20-byte "InSeq" sequencer (mpInSeq)
 * over that heap with this engine's cInSeqComm interface (+0x2AC), and allocates and zeroes @p scriptDatabaseSize
 * 8-byte "BXScriptDatabase" entries (mpScriptDatabase, count in mScriptDatabaseSize).
 *
 * @param scriptDatabaseSize  number of script database entries
 * @param maxUniqueAnimPlayers  passed to cInSeq::cInSeq() as its script limit (cInSeq's maxScripts)
 * @param heapSize  InSeq heap size in bytes
 */
void cBXScriptEngine::SetupBXEngine(int scriptDatabaseSize, int maxUniqueAnimPlayers, int heapSize)
{
    mpInSeqHeap = (char*)operator_new_tag(heapSize, str_InSeqHeap, 0, 0);
    mpInSeq = (cInSeq*)__6cInSeqPvT1Uii(cMemMan_alloc(0x20, str_InSeq, 0, 0),
        mpInSeqHeap, heapSize, maxUniqueAnimPlayers, (char*)&mInSeqCommVtbl);
    mScriptDatabaseSize = scriptDatabaseSize;
    mpScriptDatabase = (tBXScriptState*)operator_new_tag(scriptDatabaseSize * 8, str_BXScriptDatabase, 0, 0);
    for (int i = 0; i < mScriptDatabaseSize; i++) {
        mpScriptDatabase[i].pScriptData = 0;
        mpScriptDatabase[i].numLocks = 0;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_PurgeBXEngine);
#ifdef SKIP_ASM
extern "C" void cScriptDatabaseObject_RemoveObjectAll(void* self);
extern "C" void _$_6cInSeq(void* obj, int flags);
void cMemMan_free(void*);

/**
 * Tears down the BX script engine's script database, InSeq sequencer and InSeq heap.
 *
 * Does nothing unless mpScriptDatabase was allocated (SetupBXEngine). Otherwise it stops every script, releases every
 * script request, removes every database object, frees the database and zeroes its size, destroys mpInSeq
 * (destructor flags 3), frees mpInSeqHeap, and finally calls ResetInstVars().
 */
void cBXScriptEngine::PurgeBXEngine()
{
    if (mpScriptDatabase != 0) {
        StopScriptAll();
        ReleaseRequestAll();
        cScriptDatabaseObject_RemoveObjectAll(this);
        if (mpScriptDatabase != 0) {
            cMemMan_free(mpScriptDatabase);
        }
        mpScriptDatabase = 0;
        mScriptDatabaseSize = 0;
        delete mpInSeq;
        mpInSeq = 0;
        if (mpInSeqHeap != 0) {
            cMemMan_free(mpInSeqHeap);
        }
        mpInSeqHeap = 0;
        ResetInstVars();
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_Update);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cInSeq_UpdateFrame(cInSeq* inSeq);



// Retail builds compile the per-state debug hooks to nothing.
static inline void slotNop_2820B0(void* self, void* slot) {}
/**
 * Per-frame update of the script engine.
 *
 * Calls cInSeq_UpdateFrame() on the sequencer (mpInSeq), then for each of the 16 requests that is data pending runs its
 * pending-state handler by request type: an animation request goes to UpdatePendingRequestState, the other states to
 * slotNop_2820B0 (On Tour does this in UpdatePendingRequestState).
 */
void cBXScriptEngine::Update()
{
    int i;
    cInSeq_UpdateFrame(mpInSeq);
    for (i = 0; i < 16; i++) {
        tScriptRequestInfo* request = &((&mScriptRequests[0]))[i];
        if (request->requestType == keScriptRequestType_Unused) {
            continue;
        }
        if (request->requestType == keScriptRequestType_Animation) {
            switch (request->state) {
            case keScriptRequestState_Invalid: slotNop_2820B0(this, request); break;
            case keScriptRequestState_DataPending: UpdatePendingRequestState(request); break;
            case keScriptRequestState_Ready: slotNop_2820B0(this, request); break;
            case keScriptRequestState_PlayPending: slotNop_2820B0(this, request); break;
            case keScriptRequestState_Playing: slotNop_2820B0(this, request); break;
            case keScriptRequestState_Paused: slotNop_2820B0(this, request); break;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_AttachScript);
#ifdef SKIP_ASM
/**
 * Attaches script data to script database slot @p scriptIndex, unless the slot already holds data.
 *
 * Stores @p pScriptData in mpScriptDatabase[@p scriptIndex]. cSSXScriptEngine_OnAttachData calls it for each loaded
 * bank.
 *
 * @param pScriptData  the script data
 * @param scriptIndex  the script index
 * @return true when attached, false when the slot was taken
 */
bool cBXScriptEngine::AttachScript(void* pScriptData, int scriptIndex)
{
    if (mpScriptDatabase[scriptIndex].pScriptData != 0) {
        return false;
    }
    mpScriptDatabase[scriptIndex].pScriptData = (const char*)pScriptData;
    return true;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_DetachScript);
#ifdef SKIP_ASM
/**
 * Detaches the script data from script database slot @p scriptIndex and returns it.
 *
 * Clears the data of mpScriptDatabase[@p scriptIndex]. cSSXScriptEngine_OnDetachData calls it.
 *
 * @param scriptIndex  the script index
 * @return the detached script data, or 0 when the slot was empty
 */
void* cBXScriptEngine::DetachScript(int scriptIndex)
{
    tBXScriptState* slots = mpScriptDatabase;
    void* pScriptData = (void*)slots[scriptIndex].pScriptData;
    if (pScriptData == 0) {
        return 0;
    }
    slots[scriptIndex].pScriptData = 0;
    return pScriptData;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_RequestAnimationScript);
#ifdef SKIP_ASM
extern "C" int cInSeq_ReadyPlayer(cInSeq* inSeq, const char* pScriptData);
extern "C" cScriptPlayerMain* cInSeq_GetPlayer(cInSeq* inSeq, int playerID);
// cScriptPlayerMain::SetUserData (unit replay/replaycache, declared (void*, int) there), bound to its retail symbol.
void* cScriptPlayerMain_SetUserData(cScriptPlayerMain* player, void* pUserData)
    __asm__("cScriptPlayerMain_SetUserData__FPvi");




/**
 * Creates an animation script request for script @p scriptIndex and returns its handle.
 *
 * Takes a free request from mScriptRequests with CreateNewRequest. When mpScriptDatabase[@p scriptIndex] holds no data
 * yet, the request waits for it (data pending, with loadingScriptIndex and loadPending set); the engine's virtual at
 * vtable slot 7 (cSSXScriptEngine_OnDataUnavailable) is asked to fetch it, and on refusal the request is reset with
 * ResetRequest and -1 is returned. Otherwise the request is ready: an InSeq player is readied (cInSeq_ReadyPlayer,
 * cInSeq_GetPlayer on mpInSeq), stored in pPlayer and given the request as user data (cScriptPlayerMain_SetUserData),
 * and the script's lock count goes up. @p pUserData is stored in the request.
 *
 * Likely bug: CreateNewRequest returns 0 when all 16 requests are in use, and the result is used unchecked.
 *
 * @param scriptIndex  the script index
 * @param pUserData  user data stored with the request
 * @return the request handle, or -1 when the data cannot be had
 */
int cBXScriptEngine::RequestAnimationScript(int scriptIndex, void* pUserData)
{
    tBXScriptState* scriptState = &mpScriptDatabase[scriptIndex];
    tScriptRequestInfo* request = CreateNewRequest(keScriptRequestType_Animation);
    if (scriptState->pScriptData == 0) {
        request->loadingScriptIndex = scriptIndex;
        request->state = keScriptRequestState_DataPending;
        request->loadPending = 1;
        // vtable slot 7: OnDataUnavailable
        sVEntry<int(void*, int)>* vtable = (sVEntry<int(void*, int)>*)mVtbl;
        sVEntry<int(void*, int)>* vtableEntry = &vtable[7];
        if (vtableEntry->fn(vthis(this, *vtableEntry), scriptIndex) == 0) {
            request->state = keScriptRequestState_Reset;
            ResetRequest(request);
            return -1;
        }
    } else {
        request->data.animFields.animScriptIndex = scriptIndex;
        request->state = keScriptRequestState_Ready;
        int playerID = cInSeq_ReadyPlayer(mpInSeq, scriptState->pScriptData);
        cScriptPlayerMain* player = cInSeq_GetPlayer(mpInSeq, playerID);
        request->pPlayer = player;
        cScriptPlayerMain_SetUserData(player, request);
        scriptState->numLocks++;
    }
    request->pUserData = pUserData;
    return request->requestHandle;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_PlayScript);
#ifdef SKIP_ASM
extern "C" void cInSeq_Play(cInSeq* inSeq, int playerID, int loop);

/**
 * Starts playing a ready script request and returns its state.
 *
 * Looks the request up with GetRequest (0 for a free handle). Only a ready request is started: @p autoRecycle goes to
 * purgeAfterPlay, and an animation request becomes play pending and its player is started with cInSeq_Play on
 * mpInSeq. Any other state is returned unchanged.
 *
 * @param requestHandle  the request handle
 * @param autoRecycle  nonzero to release the request when it finishes playing
 * @param loop  passed to cInSeq_Play
 * @return the request state afterwards, or keScriptRequestState_Invalid for a free handle
 */
eScriptRequestState cBXScriptEngine::PlayScript(int requestHandle, int autoRecycle, int loop)
{
    tScriptRequestInfo* request = GetRequest(requestHandle);
    if (request == 0) {
        return keScriptRequestState_Invalid;
    }
    eScriptRequestState state = request->state;
    if (state != keScriptRequestState_Ready) {
        return state;
    }
    request->purgeAfterPlay = autoRecycle;
    if (request->requestType == keScriptRequestType_Animation) {
        request->state = keScriptRequestState_PlayPending;
        cInSeq_Play(mpInSeq, request->pPlayer->mID, loop);
    }
    return request->state;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_StopScript);
#ifdef SKIP_ASM
extern "C" void cInSeq_Stop(cInSeq* inSeq, int playerID);

/**
 * Stops a script request's InSeq player if it is running.
 *
 * Looks the request up with GetRequest and calls cInSeq_Stop on mpInSeq for its player's mID when the player (pPlayer)
 * exists, its mPlayState is not 0 (stopped), and the request is an animation request.
 *
 * Quirk: the lookup result is not checked, so a free handle would be dereferenced; the callers only pass live requests.
 *
 * @param requestHandle  the request handle
 */
void cBXScriptEngine::StopScript(int requestHandle)
{
    tScriptRequestInfo* request = GetRequest(requestHandle);
    cScriptPlayerMain* player = request->pPlayer;
    if (player != 0 && player->mPlayState != 0 && request->requestType == keScriptRequestType_Animation) {
        cInSeq_Stop(mpInSeq, player->mID);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_StopScriptAll);
#ifdef SKIP_ASM
/**
 * Stops the script of every script request that has a player.
 *
 * Walks the 16 mScriptRequests and, for each whose pPlayer is set, calls StopScript() with the request's handle, which
 * stops that script in the InSeq sequencer when it is running.
 */
void cBXScriptEngine::StopScriptAll()
{
    for (int i = 0; i < 16; i++) {
        tScriptRequestInfo* slot = &((&mScriptRequests[0]))[i];
        if (slot->pPlayer != 0) {
            StopScript(slot->requestHandle);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ReleaseRequestAll);
#ifdef SKIP_ASM
/**
 * Releases every script request that is still in use.
 *
 * Walks the 16 mScriptRequests and calls ReleaseRequest() with the handle of each whose requestType is not Unused;
 * that runs the request type's cleanup, releases its player and database reference and returns the slot to the free
 * list.
 */
void cBXScriptEngine::ReleaseRequestAll()
{
    for (int i = 0; i < 16; i++) {
        tScriptRequestInfo* slot = &((&mScriptRequests[0]))[i];
        if (slot->requestType != keScriptRequestType_Unused) {
            ReleaseRequest(slot->requestHandle);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ForcePlayUpdate);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_UpdateFrame(cScriptPlayerMain* player);

/**
 * Forces an immediate frame update of a playing script request.
 *
 * Finds the request record with GetRequest. When it exists and is play pending or playing (OnInSeqPlay moves the
 * first to the second), runs cScriptPlayerMain_UpdateFrame (On Tour cScriptPlayerMain::UpdateFrame) on the request's
 * player. cSSXScriptEngine_ForcePlayUpdate calls it before updating its camera managers.
 *
 * @param requestHandle  the request handle
 * @return true if the player was updated, false for an unknown or idle request
 */
bool cBXScriptEngine::ForcePlayUpdate(int requestHandle)
{
    tScriptRequestInfo* request = GetRequest(requestHandle);
    if (request == 0) {
        return false;
    }
    eScriptRequestState state = request->state;
    if (state == keScriptRequestState_PlayPending || state == keScriptRequestState_Playing) {
        cScriptPlayerMain_UpdateFrame(request->pPlayer);
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetScriptPlayRate);
#ifdef SKIP_ASM
void cScriptPlayerMain_SetPlaySpeed(void* self, float t);

/**
 * Sets the play rate of a script request, or remembers it until the request's data arrives.
 *
 * Rates below 0.01 are refused. A data-pending request keeps the rate in playSpeedRequest for
 * UpdatePendingRequestState to apply; otherwise the rate goes to its player with cScriptPlayerMain_SetPlaySpeed.
 *
 * @param requestHandle  the request handle
 * @param playRate  the new play rate
 * @return true when set or stored, false for a rate below 0.01 or a free handle
 */
bool cBXScriptEngine::SetScriptPlayRate(int requestHandle, float playRate)
{
    if (playRate < 0.01f) return false;
    tScriptRequestInfo* request = GetRequest(requestHandle);
    if (request == 0) return false;
    if (request->state == keScriptRequestState_DataPending) {
        request->playSpeedRequest = playRate;
        return true;
    }
    cScriptPlayerMain_SetPlaySpeed(request->pPlayer, playRate);
    return true;
}
#endif
//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetRequestUserData);
#ifdef SKIP_ASM
/**
 * Replaces the user data of the request bound to InSeq player @p pPlayer and returns the old value.
 *
 * The request is the player's mpUserData (set by cScriptPlayerMain_SetUserData).
 *
 * @param pPlayer  the InSeq player
 * @param pUserData  the new user data
 * @return the previous user data
 */
void* cBXScriptEngine::SetRequestUserData(cScriptPlayerMain* pPlayer, void* pUserData)
{
    tScriptRequestInfo* request = (tScriptRequestInfo*)pPlayer->mpUserData;
    void* previous = request->pUserData;
    request->pUserData = pUserData;
    return previous;
}
#endif
//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetRequestUserData1);
#ifdef SKIP_ASM
/**
 * Replaces the user data of script request @p requestHandle and returns the old value.
 *
 * Quirk: the GetRequest result is not checked, so a free handle would be dereferenced.
 *
 * @param requestHandle  the request handle
 * @param pUserData  the new user data
 * @return the previous user data
 */
void* cBXScriptEngine::SetRequestUserData(int requestHandle, void* pUserData)
{
    tScriptRequestInfo* request = GetRequest(requestHandle);
    void* previous = request->pUserData;
    request->pUserData = pUserData;
    return previous;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ReleaseRequest);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cInSeq_RecyclePlayer(cInSeq* inSeq, int playerID);

/**
 * Releases a script request: stops it, unbinds and recycles its player, drops its script lock, and frees the record.
 *
 * Does nothing but return Released for a free handle. Clears purgeAfterPlay, then by state: a request still waiting
 * for data calls the engine's virtual at vtable slot 8 (cSSXScriptEngine_OnPendingDataCancelled) with the pending
 * index while that script has no data; a request that is starting, playing or paused is stopped with StopScript. It
 * then calls the virtual at slot 12 (cSSXScriptEngine_OnReleaseRequest) with the handle and the user data. A player
 * gets its user data cleared (cScriptPlayerMain_SetUserData) and goes back to the InSeq pool (cInSeq_RecyclePlayer
 * with its ID). The lock count of the request's script in mpScriptDatabase drops, and at zero the virtual at slot 9
 * (cSSXScriptEngine_OnScriptDataReleased) is called. Last, the record is cleared with ResetRequest (its Reset state
 * store is overwritten there).
 *
 * @param requestHandle  the request handle
 * @return always keScriptRequestState_Released
 */
eScriptRequestState cBXScriptEngine::ReleaseRequest(int requestHandle)
{
    tScriptRequestInfo* request = GetRequest(requestHandle);
    if (request == 0 || request->requestType == keScriptRequestType_Unused) {
        return keScriptRequestState_Released;
    }
    request->purgeAfterPlay = 0;
    switch (request->state) {
    case keScriptRequestState_DataPending:
        {
            tBXScriptState* loadingState = mpScriptDatabase + request->loadingScriptIndex;
            if (loadingState->pScriptData == 0) {
                // vtable slot 8: OnPendingDataCancelled
                sVEntry<void(void*, int)>* cancelVtable = (sVEntry<void(void*, int)>*)mVtbl;
                cancelVtable[8].fn(vthis(this, cancelVtable[8]), request->loadingScriptIndex);
            }
        }
        break;
    case keScriptRequestState_PlayPending:
    case keScriptRequestState_Playing:
    case keScriptRequestState_Paused:
        StopScript(requestHandle);
        break;
    case keScriptRequestState_Invalid:
    case keScriptRequestState_Ready:
    case keScriptRequestState_Released:
        break;
    }
    // vtable slot 12: OnReleaseRequest
    sVEntry<void(void*, int, void*)>* releaseVtable = (sVEntry<void(void*, int, void*)>*)mVtbl;
    releaseVtable[12].fn(vthis(this, releaseVtable[12]), requestHandle, request->pUserData);
    if (request->pPlayer != 0) {
        cScriptPlayerMain_SetUserData(request->pPlayer, 0);
        cInSeq_RecyclePlayer(mpInSeq, request->pPlayer->mID);
    }
    if (request->data.animFields.animScriptIndex >= 0) {
        tBXScriptState* scriptState = mpScriptDatabase + request->data.animFields.animScriptIndex;
        if (--scriptState->numLocks == 0) {
            // vtable slot 9: OnScriptDataReleased
            sVEntry<void(void*, int)>* releasedVtable = (sVEntry<void(void*, int)>*)mVtbl;
            releasedVtable[9].fn(vthis(this, releasedVtable[9]), request->data.animFields.animScriptIndex);
        }
    }
    request->state = keScriptRequestState_Reset;
    ResetRequest(request);
    return keScriptRequestState_Released;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_PauseAll);
#ifdef SKIP_ASM
extern "C" void cInSeq_PauseAll(cInSeq* inSeq);

/**
 * Pauses every InSeq script player: cInSeq_PauseAll on mpInSeq.
 */
void cBXScriptEngine::PauseAll() {
    cInSeq_PauseAll(mpInSeq);
}
#endif
//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ResumeAll);
#ifdef SKIP_ASM
extern "C" void cInSeq_ResumeAll(cInSeq* inSeq);

/**
 * Resumes every InSeq script player: cInSeq_ResumeAll on mpInSeq.
 */
void cBXScriptEngine::ResumeAll() {
    cInSeq_ResumeAll(mpInSeq);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_PauseScript);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerMain_Pause(void* self);

/**
 * Pauses script request @p requestHandle.
 *
 * Calls cScriptPlayerMain_Pause on the request's player (pPlayer) and marks the request paused when the player's
 * mNumPaused is above 0. The request is indexed directly in mScriptRequests without checking that it is live.
 *
 * @param requestHandle  the request handle (0-15)
 */
void cBXScriptEngine::PauseScript(int requestHandle)
{
    tScriptRequestInfo* request = &mScriptRequests[requestHandle];
    cScriptPlayerMain_Pause(request->pPlayer);
    if (request->pPlayer->mNumPaused > 0) {
        request->state = keScriptRequestState_Paused;
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
 * Calls cScriptPlayerMain_Resume on the request's player. A paused request whose player is no longer paused
 * (mNumPaused at or below 0) goes back to playing when the player's mPlayState is Playing or PlayEnding, else to
 * play pending. The request is indexed directly in mScriptRequests.
 *
 * @param requestHandle  the request handle (0-15)
 */
void cBXScriptEngine::ResumeScript(int requestHandle)
{
    tScriptRequestInfo* request = &mScriptRequests[requestHandle];
    cScriptPlayerMain_Resume(request->pPlayer);
    if (request->state == keScriptRequestState_Paused) {
        cScriptPlayerMain* player = request->pPlayer;
        if (player->mNumPaused <= 0) {
            int playState = player->mPlayState;
            if (playState == keScriptPlayState_Playing || playState == keScriptPlayState_PlayEnding) {
                request->state = keScriptRequestState_Playing;
            } else {
                request->state = keScriptRequestState_PlayPending;
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
/**
 * Returns the state of script request @p requestHandle, first promoting it if its data has arrived.
 *
 * A data-pending request is passed to UpdatePendingRequestState, which makes it ready when its script data is
 * attached.
 *
 * @param requestHandle  the request handle
 * @return the request state, or keScriptRequestState_Invalid for a free handle
 */
eScriptRequestState cBXScriptEngine::GetRequestState(int requestHandle)
{
    tScriptRequestInfo* request = GetRequest(requestHandle);
    if (request == 0) {
        return keScriptRequestState_Invalid;
    }
    if (request->state == keScriptRequestState_DataPending) {
        UpdatePendingRequestState(request);
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
 * While the request waits for data that is the pending index (loadingScriptIndex); otherwise the bound index.
 *
 * @param requestHandle  the request handle
 * @return the script index, or -1 for a free handle
 */
int cBXScriptEngine::GetRequestScriptIndex(int requestHandle)
{
    tScriptRequestInfo* request = GetRequest(requestHandle);
    if (request == 0) return -1;
    if (request->state == keScriptRequestState_DataPending) return request->loadingScriptIndex;
    return request->data.animFields.animScriptIndex;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_IsRequestPlaybackEnding);
#ifdef SKIP_ASM
/**
 * Returns whether script request @p requestHandle is playing its end.
 *
 * A data-pending request is first updated with UpdatePendingRequestState. Only a playing or paused request can be
 * ending: then the result is whether its player's mPlayState is PlayEnding.
 *
 * @param requestHandle  the request handle
 * @return true when ending, else false (also for a free handle)
 */
bool cBXScriptEngine::IsRequestPlaybackEnding(int requestHandle)
{
    tScriptRequestInfo* request = GetRequest(requestHandle);
    if (request == 0) {
        return false;
    }
    if (request->state == keScriptRequestState_DataPending) {
        UpdatePendingRequestState(request);
    }
    if (request->state != keScriptRequestState_Playing && request->state != keScriptRequestState_Paused) {
        return false;
    }
    return request->pPlayer->mPlayState == keScriptPlayState_PlayEnding;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_ResetInstVars);
#ifdef SKIP_ASM
/**
 * Resets the BX script engine's instance variables: the InSeq pointers and the 16 script request records.
 *
 * Zeroes mpInSeq and mpInSeqHeap without freeing them (PurgeBXEngine frees them first). Each request record gets its
 * index as its handle, no pending index (-1), and no player, state, type, flags or stored play rate; the script index
 * and the user data are left as they were.
 */
void cBXScriptEngine::ResetInstVars()
{
    int i;
    mpInSeq = 0;
    mpInSeqHeap = 0;
    for (i = 0; i < 16; i++) {
        mScriptRequests[i].requestHandle = i;
        mScriptRequests[i].requestType = keScriptRequestType_Unused;
        mScriptRequests[i].pPlayer = 0;
        mScriptRequests[i].state = keScriptRequestState_Invalid;
        mScriptRequests[i].purgeAfterPlay = 0;
        mScriptRequests[i].loadPending = 0;
        mScriptRequests[i].playSpeedRequest = 0.0f;
        mScriptRequests[i].loadingScriptIndex = -1;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_UpdatePendingRequestState);
#ifdef SKIP_ASM



/**
 * Makes a data-pending script request ready once its script data has been attached.
 *
 * Does nothing while mpScriptDatabase[pending index] has no data. Otherwise binds the index, clears the pending index
 * (-1) and flag, marks the request ready, readies an InSeq player for the data (cInSeq_ReadyPlayer, cInSeq_GetPlayer
 * on mpInSeq), stores it in pPlayer with the request as its user data (cScriptPlayerMain_SetUserData), applies and
 * clears a stored play rate (cScriptPlayerMain_SetPlaySpeed) and raises the script's lock count.
 *
 * @param pRequest  the request record (an mScriptRequests entry)
 */
void cBXScriptEngine::UpdatePendingRequestState(tScriptRequestInfo* pRequest)
{
    tScriptRequestInfo* request = pRequest;
    int scriptIndex = request->loadingScriptIndex;
    tBXScriptState* scriptState = &mpScriptDatabase[scriptIndex];
    if (scriptState->pScriptData == 0) return;
    request->data.animFields.animScriptIndex = scriptIndex;
    request->loadingScriptIndex = -1;
    request->state = keScriptRequestState_Ready;
    request->loadPending = 0;
    int playerID = cInSeq_ReadyPlayer(mpInSeq, scriptState->pScriptData);
    cScriptPlayerMain* player = cInSeq_GetPlayer(mpInSeq, playerID);
    request->pPlayer = player;
    cScriptPlayerMain_SetUserData(player, request);
    if (request->playSpeedRequest > 0.0f) {
        cScriptPlayerMain_SetPlaySpeed(request->pPlayer, request->playSpeedRequest);
        request->playSpeedRequest = 0.0f;
    }
    scriptState->numLocks++;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqPlay);
#ifdef SKIP_ASM
/**
 * Sequencer callback for a script player that starts: notifies the owner and marks its request playing.
 *
 * The player's request record is its mpUserData. Calls virtual slot 10 of the engine's vtable (OnStartScript) with
 * the player and the request's script index and user data, then marks the request playing. On Tour asserts the
 * sequencer exists and the request was play pending; retail does not. cSSXScriptEngine_OnInSeqPlay forwards here.
 *
 * @param pPlayer  the cScriptPlayerMain that started
 */
void cBXScriptEngine::OnInSeqPlay(cScriptPlayerMain* pPlayer)
{
    tScriptRequestInfo* request = (tScriptRequestInfo*)pPlayer->mpUserData;
    // vtable slot 10: OnStartScript
    sVEntry<void(void*, cScriptPlayerMain*, int, void*)>* vt =
        (sVEntry<void(void*, cScriptPlayerMain*, int, void*)>*)mVtbl;
    vt[10].fn(vthis(this, vt[10]), pPlayer, request->data.animFields.animScriptIndex, request->pUserData);
    request->state = keScriptRequestState_Playing;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqStop);
#ifdef SKIP_ASM
/**
 * Sequencer callback for a script player that stops: notifies the owner, marks the request stopped and recycles it if
 * asked.
 *
 * Calls virtual slot 11 of the engine's vtable (OnStopScript) with the player and its request's script index and user
 * data, marks the request ready again and, when the request's purgeAfterPlay is set (PlayScript's autoRecycle),
 * releases the request by its handle with ReleaseRequest. cSSXScriptEngine_OnInSeqStop forwards here.
 *
 * @param pPlayer  the cScriptPlayerMain that stopped
 */
void cBXScriptEngine::OnInSeqStop(cScriptPlayerMain* pPlayer)
{
    tScriptRequestInfo* request = (tScriptRequestInfo*)pPlayer->mpUserData;
    // vtable slot 11: OnStopScript
    sVEntry<void(void*, cScriptPlayerMain*, int, void*)>* vt =
        (sVEntry<void(void*, cScriptPlayerMain*, int, void*)>*)mVtbl;
    vt[11].fn(vthis(this, vt[11]), pPlayer, request->data.animFields.animScriptIndex, request->pUserData);
    request->state = keScriptRequestState_Ready;
    if (request->purgeAfterPlay != 0) {
        ReleaseRequest(request->requestHandle);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqRequestEntityCommInterface);
#ifdef SKIP_ASM
extern "C" int cScriptDatabaseObject_GetAvailableEntities(void* self, int key, void** out, int max);

/**
 * Finds the script entity that should drive InSeq player entity @p pEntity.
 *
 * Lists up to 30 candidate entities for the entity's owner player (its mpOwner's mID) with
 * cScriptDatabaseObject_GetAvailableEntities and returns the first whose virtual at slot 5 (the entity match test,
 * e.g. cScriptObjRider_IsMatchingEntity) accepts (@p pEntity, @p type).
 *
 * @param pEntity  the InSeq player entity
 * @param type  the entity type passed to the match test
 * @return the matching script entity, or 0
 */
cScriptEntity* cBXScriptEngine::OnInSeqRequestEntityCommInterface(cScriptPlayerEntity* pEntity, int type)
{
    cScriptEntity* buf[30];
    cScriptEntity* found = 0;
    int n = cScriptDatabaseObject_GetAvailableEntities(this, pEntity->mpOwner->mID, (void**)buf, 30);
    for (int i = 0; i < n; i++) {
        cScriptEntity* candidate = buf[i];
        // vtable slot 5: IsMatchingEntity
        sVEntry<int(void*, cScriptPlayerEntity*, int)>* vtable =
            (sVEntry<int(void*, cScriptPlayerEntity*, int)>*)candidate->mVtbl;
        if (vtable[5].fn(vthis(candidate, vtable[5]), pEntity, type)) {
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
/**
 * Wraps a remotely started InSeq player in a new ready script request.
 *
 * Takes an animation request (CreateNewRequest), marks it ready with no script index (-1) and @p pPlayer as its
 * player, and binds the request as the player's user data (cScriptPlayerMain_SetUserData).
 *
 * Likely bug: the request is used unchecked when all 16 are in use.
 *
 * @param pPlayer  the InSeq player
 */
void cBXScriptEngine::OnInSeqRemoteScriptPlayerReady(cScriptPlayerMain* pPlayer)
{
    tScriptRequestInfo* request = CreateNewRequest(keScriptRequestType_Animation);
    request->state = keScriptRequestState_Ready;
    request->data.animFields.animScriptIndex = -1;
    request->pPlayer = pPlayer;
    cScriptPlayerMain_SetUserData(pPlayer, request);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_OnInSeqRemoteScriptPlayerDiscard);
#ifdef SKIP_ASM
/**
 * Frees the script request bound to a discarded InSeq player.
 *
 * Takes the request from the player's mpUserData, marks it Reset and clears it with ResetRequest.
 *
 * @param pPlayer  the InSeq player
 */
void cBXScriptEngine::OnInSeqRemoteScriptPlayerDiscard(cScriptPlayerMain* pPlayer)
{
    tScriptRequestInfo* request = (tScriptRequestInfo*)pPlayer->mpUserData;
    request->state = keScriptRequestState_Reset;
    ResetRequest(request);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestUserData__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the user data of the script request bound to InSeq player @p pPlayer (its mpUserData).
 *
 * @param pPlayer  the InSeq player
 * @return the request's user data
 */
void* cBXScriptEngine::GetRequestUserData(cScriptPlayerMain* pPlayer)
{
    tScriptRequestInfo* request = (tScriptRequestInfo*)pPlayer->mpUserData;
    return request->pUserData;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestUserData1);
#ifdef SKIP_ASM
/**
 * Returns the user data of script request @p requestHandle.
 *
 * Quirk: the GetRequest result is not checked, so a free handle would be dereferenced.
 *
 * @param requestHandle  the request handle
 * @return the request's user data
 */
void* cBXScriptEngine::GetRequestUserData(int requestHandle)
{
    return GetRequest(requestHandle)->pUserData;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestHandleFromPlayer__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns the handle (requestHandle) of the script request bound to InSeq player @p pPlayer (its mpUserData).
 *
 * cSSXScriptEngine's InSeq hooks (cSSXScriptEngine_OnInSeqPlay and others) use it to find the NIS list entry.
 *
 * @param pPlayer  the InSeq player
 * @return the request handle
 */
int cBXScriptEngine::GetRequestHandleFromPlayer(cScriptPlayerMain* pPlayer)
{
    tScriptRequestInfo* request = (tScriptRequestInfo*)pPlayer->mpUserData;
    return request->requestHandle;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestPlayerByReqHandle);
#ifdef SKIP_ASM
/**
 * Returns the script player of a request handle.
 *
 * GetRequest finds the request record. The tNISList code and cSSXScriptEngine_ForcePlayUpdate use it to reach a
 * request's script header.
 *
 * @param requestHandle  the request handle
 * @return the request's player, or 0 for an unknown handle
 */
cScriptPlayerMain* cBXScriptEngine::GetRequestPlayerByReqHandle(int requestHandle)
{
    tScriptRequestInfo* request = GetRequest(requestHandle);
    if (request == 0) {
        return 0;
    }
    return request->pPlayer;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequestPlayerByPlayerID__FPv);
#ifdef SKIP_ASM
/**
 * Returns the sequencer's script player for a player ID: cInSeq_GetPlayer on mpInSeq.
 *
 * @param playerID  the InSeq player ID
 * @return the cScriptPlayerMain cInSeq_GetPlayer returns
 */
cScriptPlayerMain* cBXScriptEngine::GetRequestPlayerByPlayerID(int playerID)
{
    return cInSeq_GetPlayer(mpInSeq, playerID);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_CreateNewRequest);
#ifdef SKIP_ASM
/**
 * Claims the first free script request record (requestType Unused) of the 16 in mScriptRequests.
 *
 * Stores @p requestType; an animation request also gets no script index (-1).
 *
 * @param requestType  the request type
 * @return the record, or 0 when all 16 are in use
 */
tScriptRequestInfo* cBXScriptEngine::CreateNewRequest(eScriptRequestType requestType)
{
    int i = 0;
    do {
        if (mScriptRequests[i].requestType == keScriptRequestType_Unused) {
            mScriptRequests[i].requestType = requestType;
            switch (requestType) {
            case keScriptRequestType_Animation:
                mScriptRequests[i].data.animFields.animScriptIndex = -1;
                break;
            }
            return &mScriptRequests[i];
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
 * Zeroes the player, state, type, release and pending flags, stored play rate and user data, and sets the pending
 * index to -1. The script index and the handle stay.
 *
 * @param pRequest  the request record
 */
void cBXScriptEngine::ResetRequest(tScriptRequestInfo* pRequest)
{
    pRequest->pPlayer = 0;
    pRequest->state = keScriptRequestState_Invalid;
    pRequest->requestType = keScriptRequestType_Unused;
    pRequest->purgeAfterPlay = 0;
    pRequest->loadPending = 0;
    pRequest->loadingScriptIndex = -1;
    pRequest->playSpeedRequest = 0.0f;
    pRequest->pUserData = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_GetRequest);
#ifdef SKIP_ASM
/**
 * Returns script request record @p requestHandle (in mScriptRequests) when it is in use (requestType not Unused).
 *
 * The handle is not range-checked.
 *
 * @param requestHandle  the request handle (0-15)
 * @return the record, or 0 when it is free
 */
tScriptRequestInfo* cBXScriptEngine::GetRequest(int requestHandle)
{
    tScriptRequestInfo* request = &mScriptRequests[requestHandle];
    return request->requestType != keScriptRequestType_Unused ? request : 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_cScriptObjTransition);
#ifdef SKIP_ASM
// cScriptObject's constructor (below), called as the base-class part: the flat layout has no C++ base class.
void* cScriptObject_cScriptObject(void* self, int objType);
extern void* cScriptObjTransition_vtbl[];

/**
 * Constructs a script transition object with no transition running.
 *
 * Runs the cScriptObject constructor with object type 2 (transition), installs cScriptObjTransition_vtbl, clears the
 * camera manager, sets the player ID to -1 and the transition to Num (none), and zeroes the in, hold and out times,
 * the colour and the clock.
 */
cScriptObjTransition::cScriptObjTransition()
{
    cScriptObject_cScriptObject(this, keScriptObjectType_Transition);
    mVtbl = cScriptObjTransition_vtbl;
    mpCameraManager = 0;
    mInfo.currTime = 0.0f;
    mPlayerID = -1;
    mInfo.transition = keScriptTransition_Num;
    mInfo.inTime = 0.0f;
    mInfo.holdTime = 0.0f;
    mInfo.outTime = 0.0f;
    mInfo.transitionColour = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_Update);
#ifdef SKIP_ASM
/**
 * Advances a running transition and ends it once its time is up.
 *
 * While a player owns the transition (mPlayerID >= 0), adds @p frameDelta / 60 to the clock (mInfo.currTime); once
 * the clock reaches the sum of the in, hold and out times less 0.01, calls SetTransition with transition Num (none).
 *
 * @param frameDelta  the frame delta in frames
 */
void cScriptObjTransition::Update(float frameDelta)
{
    int playerID = mPlayerID;
    if (playerID >= 0) {
        float currTime = mInfo.currTime + frameDelta * 0.01666666753590107f;
        mInfo.currTime = currTime;
        if (mInfo.inTime + mInfo.holdTime + mInfo.outTime - 0.009999999776482582f <= currTime) {
            SetTransition(playerID, keScriptTransition_Num, 0.0f, 0.0f, 0.0f, 0.0f);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_SetCameraManager__FPvi);
#ifdef SKIP_ASM
/**
 * Sets a script transition object's camera manager (mpCameraManager).
 *
 * @param pManager  the camera manager
 */
void cScriptObjTransition::SetCameraManager(cScriptObjCameraManager* pManager)
{
    mpCameraManager = pManager;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_SetTransition__FPvii);
#ifdef SKIP_ASM
/**
 * Starts or ends a transition, keeping the current colour: the full SetTransition with mInfo.transitionColour.
 *
 * @param playerID  the player that owns the transition
 * @param transition  the transition kind (Num = none)
 * @param currFrame  the current frame within the transition
 * @param outFrames  the fade-out length in frames
 * @param inFrames  the fade-in length in frames
 * @param holdFrames  the hold length in frames
 */
void cScriptObjTransition::SetTransition(int playerID, eScriptTransition transition, float currFrame, float outFrames,
                                         float inFrames, float holdFrames)
{
    SetTransition(playerID, transition, mInfo.transitionColour, currFrame, outFrames, inFrames, holdFrames);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_SetTransition1);
#ifdef SKIP_ASM
/**
 * Ends the running transition and starts a new one, or just ends it.
 *
 * When a transition is running (not Num) the object's virtual at slot 5 (cScriptObject_OnEndObjectUse in
 * cScriptObjTransition_vtbl) is called with the owning player. When @p transition is Num, or the in, hold and out
 * frames together do not exceed @p currFrame, everything is cleared: transition Num, player -1, times and clock 0.
 * Otherwise the new transition, colour and player are stored, the clock restarts at 0, the in, hold and out frames are
 * stored as seconds (/ 60), and the virtual at slot 4 (cScriptObject_OnStartObjectUse__FPv) is called with the player.
 *
 * @param playerID  the player that owns the transition
 * @param transition  the transition kind (Num = none)
 * @param transitionColour  the transition colour
 * @param currFrame  the current frame within the transition; only decides whether it is already over
 * @param outFrames  the fade-out length in frames
 * @param inFrames  the fade-in length in frames
 * @param holdFrames  the hold length in frames
 */
void cScriptObjTransition::SetTransition(int playerID, eScriptTransition transition, int transitionColour,
                                         float currFrame, float outFrames, float inFrames, float holdFrames)
{
    if (mInfo.transition != keScriptTransition_Num) {
        // vtable slot 5: OnEndObjectUse
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)mVtbl;
        vt[5].fn(vthis(this, vt[5]), mPlayerID);
    }
    if (transition == keScriptTransition_Num || inFrames + holdFrames + outFrames <= currFrame) {
        mInfo.transition = keScriptTransition_Num;
        mInfo.inTime = 0.0f;
        mInfo.holdTime = 0.0f;
        mPlayerID = -1;
        mInfo.outTime = 0.0f;
        mInfo.currTime = 0.0f;
    } else {
        mInfo.transition = transition;
        mInfo.transitionColour = transitionColour;
        mInfo.currTime = 0.0f;
        mPlayerID = playerID;
        mInfo.inTime = inFrames * 0.01666666753590107f;
        mInfo.holdTime = holdFrames * 0.01666666753590107f;
        mInfo.outTime = outFrames * 0.01666666753590107f;
        // vtable slot 4: OnStartObjectUse
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)mVtbl;
        vt[4].fn(vthis(this, vt[4]), mPlayerID);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_IsInUseByPlayer);
#ifdef SKIP_ASM
/**
 * Returns whether the transition is owned by player @p playerID (mPlayerID).
 *
 * cSSXScriptEngine_IsObjInUseByPlayer calls it.
 *
 * @param playerID  the player ID
 * @return true when owned by that player
 */
bool cScriptObjTransition::IsInUseByPlayer(int playerID)
{
    return mPlayerID == playerID;
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
void cScriptObjTransition::PauseObjScript()
{
    ((cBxScriptVirt*)mpCameraManager)->PauseObjScript();
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObjTransition_ResumeObjScript);
#ifdef SKIP_ASM
/**
 * Resumes the transition object's script: calls the third virtual of the object at +0x10 (On Tour mpCameraManager,
 * a cScriptEntity; presumably ResumeEntityScript).
 */
void cScriptObjTransition::ResumeObjScript()
{
    ((cBxScriptVirt*)mpCameraManager)->ResumeObjScript();
}
#endif

extern void* cScriptEntity_vtbl[];

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_cScriptEntity__FPv);
#ifdef SKIP_ASM
/**
 * Constructs a script entity: installs cScriptEntity's vtable, no database ID (-1) and no bound InSeq entity.
 */
cScriptEntity::cScriptEntity()
{
    mVtbl = cScriptEntity_vtbl;
    mDBID = -1;
    mpEntity = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_IsPlaying__FPv);
#ifdef SKIP_ASM
/**
 * Returns whether a script entity is bound to an InSeq player entity (mpEntity, set while a script has it locked).
 *
 * @return true when bound
 */
bool cScriptEntity::IsPlaying()
{
    return mpEntity != 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_DetachEntity);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_SetEntityComm(cScriptPlayerEntity* entity, cScriptEntity* comm);

/**
 * Cuts the bound InSeq player entity (mpEntity) off from this script entity: cScriptPlayerEntity_SetEntityComm(entity,
 * 0).
 *
 * Does nothing when no entity is bound.
 */
void cScriptEntity::DetachEntity()
{
    cScriptPlayerEntity* entity = mpEntity;
    if (entity != 0) {
        cScriptPlayerEntity_SetEntityComm(entity, 0);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_OnInSeqLock);
#ifdef SKIP_ASM
extern "C" void* cScriptDatabaseObject_GetDatabaseObject(void);
extern "C" void cScriptDatabaseObject_MarkEntityAsUsed(void* database, cScriptEntity* entity, int playerID);

/**
 * Locks this script entity to InSeq player entity @p pEntity.
 *
 * Stores @p pEntity in mpEntity and registers the lock with the script database object
 * (cScriptDatabaseObject_GetDatabaseObject) through cScriptDatabaseObject_MarkEntityAsUsed, under the ID (mID) of the
 * entity's owner player (mpOwner).
 *
 * @param pEntity  the InSeq player entity
 */
void cScriptEntity::OnInSeqLock(cScriptPlayerEntity* pEntity)
{
    mpEntity = pEntity;
    cScriptDatabaseObject_MarkEntityAsUsed(cScriptDatabaseObject_GetDatabaseObject(), this, pEntity->mpOwner->mID);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_OnInSeqRelease);
#ifdef SKIP_ASM
extern "C" void cScriptDatabaseObject_MarkEntityAsFree(void* database, cScriptEntity* entity);

/**
 * Releases this script entity's InSeq lock: unregisters it from the script database object
 * (cScriptDatabaseObject_MarkEntityAsFree on cScriptDatabaseObject_GetDatabaseObject()) and clears the bound entity.
 */
void cScriptEntity::OnInSeqRelease()
{
    cScriptDatabaseObject_MarkEntityAsFree(cScriptDatabaseObject_GetDatabaseObject(), this);
    mpEntity = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_PauseEntityScript);
#ifdef SKIP_ASM
/**
 * Pauses the script driving this entity: cScriptPlayerMain_Pause on the bound entity's owner player (mpOwner).
 *
 * Does nothing when no entity is bound.
 */
void cScriptEntity::PauseEntityScript()
{
    cScriptPlayerEntity* entity = mpEntity;
    if (entity != 0) {
        cScriptPlayerMain_Pause(entity->mpOwner);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntity_ResumeEntityScript);
#ifdef SKIP_ASM
/**
 * Resumes the script driving this entity: cScriptPlayerMain_Resume on the bound entity's owner player (mpOwner).
 *
 * Does nothing when no entity is bound.
 */
void cScriptEntity::ResumeEntityScript()
{
    cScriptPlayerEntity* entity = mpEntity;
    if (entity != 0) {
        cScriptPlayerMain_Resume(entity->mpOwner);
    }
}
#endif

// EA's scriptentitycam.cpp starts here: this retail unit is several EA source files in a row (bxscriptengine.cpp,
// scriptobjtransition.cpp, scriptentity.cpp, scriptentitycam.cpp, scriptobj.cpp; see scripter/scriptentitycam.h).
// The camera entity needs scripter/ssxscriptengine.h (the camera headers, tSSXScriptSetupInfo, ScriptEngine and
// cScriptObjCameraManager), and it can only be included here: when it comes first, the inline t2Vector methods it
// reaches through bx/math3d.h make g++ 2.95 compile cBXScriptEngine::Update and ReleaseRequest differently.
#include "scripter/ssxscriptengine.h"

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntityCamera_cScriptEntityCamera);
#ifdef SKIP_ASM
extern void* cScriptEntityCamera_vtbl[];
// cScriptEntity's constructor (above), called as the base-class part: the flat layout has no C++ base class.
void* cScriptEntity_cScriptEntity(void* self);

/**
 * Constructs a camera script entity: the cScriptEntity constructor, then the vtable cScriptEntityCamera_vtbl, sub-type
 * 9 (none) and no camera manager.
 */
cScriptEntityCamera::cScriptEntityCamera()
{
    cScriptEntity_cScriptEntity(this);
    mVtbl = cScriptEntityCamera_vtbl;
    mSubType = 9;
    mpCamManager = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntityCamera_OnInSeqLock);
#ifdef SKIP_ASM
int cScriptPlayerEntity_GetType(void* self);
tSSXHeaderCameraManual* GetSSXCameraManualHeader(void* self);
tSSXHeaderCameraTarget* GetSSXCameraTargetHeader(void* self);
tSSXHeaderCameraSubject* GetSSXCameraSubjectHeader(void* self);
tSSXHeaderCameraAlgorithmic* GetSSXCameraAlgorithmicHeader(void* self);

/**
 * Locks this camera script entity to InSeq player entity @p pEntity and takes its camera sub-type from the entity's
 * data.
 *
 * Runs the base lock (cScriptEntity::OnInSeqLock), then copies the sub-type byte of the entity's camera header into
 * mSubType, picking the header by the entity's type (cScriptPlayerEntity_GetType): GetSSXCameraManualHeader,
 * GetSSXCameraTargetHeader, GetSSXCameraSubjectHeader or GetSSXCameraAlgorithmicHeader. Other types leave it
 * unchanged.
 *
 * @param pEntity  the InSeq player entity
 */
void cScriptEntityCamera::OnInSeqLock(cScriptPlayerEntity* pEntity)
{
    ((cScriptEntity*)this)->OnInSeqLock(pEntity);
    switch (cScriptPlayerEntity_GetType(pEntity)) {
    case keSSXEntityType_CameraManual:
        mSubType = GetSSXCameraManualHeader(pEntity)->subType;
        break;
    case keSSXEntityType_CameraTarget:
        mSubType = GetSSXCameraTargetHeader(pEntity)->subType;
        break;
    case keSSXEntityType_CameraSubject:
        mSubType = GetSSXCameraSubjectHeader(pEntity)->subType;
        break;
    case keSSXEntityType_CameraAlgorithmic:
        mSubType = GetSSXCameraAlgorithmicHeader(pEntity)->subType;
        break;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntityCamera_OnInSeqRelease);
#ifdef SKIP_ASM
/**
 * Releases this camera script entity's InSeq lock (cScriptEntity::OnInSeqRelease) and resets its sub-type to 9
 * (none).
 */
void cScriptEntityCamera::OnInSeqRelease()
{
    ((cScriptEntity*)this)->OnInSeqRelease();
    mSubType = 9;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptEntityCamera_IsMatchingEntity);
#ifdef SKIP_ASM
/**
 * Returns whether InSeq player entity @p pEntity belongs to this camera's view.
 *
 * Only camera entities (manual, target, subject, algorithmic) can match: then the view index of the script setup info
 * of the entity's owner player (ScriptEngine's GetScriptSetupInfo) must be the camera manager's mViewIndex.
 *
 * @param pEntity  the InSeq player entity
 * @param type  the entity type (eSSXEntityType)
 * @return true on a match
 */
bool cScriptEntityCamera::IsMatchingEntity(cScriptPlayerEntity* pEntity, int type)
{
    if (type > keSSXEntityType_CameraAlgorithmic) {
        return false;
    }
    return ScriptEngine->GetScriptSetupInfo(pEntity->mpOwner)->viewIndex == mpCamManager->mViewIndex;
}
#endif

extern void* cScriptObject_vtbl[];

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_cScriptObject__FPvi);
#ifdef SKIP_ASM
/**
 * Constructs a script object of a given type that is not yet registered in the script database.
 *
 * Stores cScriptObject's vtable (mVtbl), @p objType (mObjType), no database handle (kScriptObjectID_None) and a use
 * count (mInUse) of 0.
 *
 * @param objType  the object's script type
 */
cScriptObject::cScriptObject(int objType)
{
    mVtbl = cScriptObject_vtbl;
    mObjType = objType;
    mObjID = kScriptObjectID_None;
    mInUse = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject__cScriptObject);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys a script object, first removing it from the script database if it is still registered.
 *
 * Restores cScriptObject's vtable. When the database handle (mObjID) is neither -1 nor 0 it calls
 * DeInitScriptObject() and sets the handle to -1.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptObject__cScriptObject(cScriptObject* self, int flags)
{
    self->mVtbl = cScriptObject_vtbl;
    if (self->mObjID != kScriptObjectID_None && self->mObjID != 0) {
        self->DeInitScriptObject();
        self->mObjID = kScriptObjectID_None;
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_InitScriptObject);
#ifdef SKIP_ASM
/**
 * Registers a script object with the script database and stores the handle it gets.
 *
 * Calls virtual slot 2 (AddObject) of the database object returned by cScriptDatabaseObject_GetDatabaseObject (On
 * Tour cScriptDatabaseObject::GetDatabaseObject), passing the object, and stores the result in the object's database
 * handle (mObjID); DeInitScriptObject undoes it with slot 3. On Tour asserts the handle is >= 0 afterwards; retail
 * does not. cSSXScriptEngine_Load registers its script objects with it, and cRider_cRider the rider's.
 */
void cScriptObject::InitScriptObject()
{
    cScriptDatabaseObject* database = (cScriptDatabaseObject*)cScriptDatabaseObject_GetDatabaseObject();
    // vtable slot 2: AddObject
    sVEntry<int(void*, cScriptObject*)>* vt = (sVEntry<int(void*, cScriptObject*)>*)database->mVtbl;
    mObjID = vt[2].fn(vthis(database, vt[2]), this);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_DeInitScriptObject);
#ifdef SKIP_ASM
/**
 * Removes the script object from the script engine's object database and marks it unregistered.
 *
 * Calls virtual slot 3 (RemoveObject) of the script database object (cScriptDatabaseObject_GetDatabaseObject()),
 * passing the object's database handle (mObjID); cScriptDatabaseObject_RemoveObjectAll() uses the same slot to remove
 * each entry. The handle is then set to -1.
 */
void cScriptObject::DeInitScriptObject()
{
    cScriptDatabaseObject* database = (cScriptDatabaseObject*)cScriptDatabaseObject_GetDatabaseObject();
    // vtable slot 3: RemoveObject
    sVEntry<int(void*, int)>* vt = (sVEntry<int(void*, int)>*)database->mVtbl;
    vt[3].fn(vthis(database, vt[3]), mObjID);
    mObjID = kScriptObjectID_None;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_PauseObjScript);
#ifdef SKIP_ASM
/**
 * Pauses the scripts of every script entity the object owns.
 *
 * Loops over the object's virtual GetNumScriptEntities() (slot 6) and calls cScriptEntity::PauseEntityScript on each
 * virtual GetScriptEntity(i) (slot 7). It is the base implementation that classes without their own PauseObjScript
 * inherit (cScriptObjRider's vtable cScriptObjRider_cScriptObject_vtbl). On Tour asserts each entity is non-null; retail does not.
 */
void cScriptObject::PauseObjScript()
{
    int i = 0;
    // vtable slot 6: GetNumScriptEntities
    sVEntry<int(void*)>* countVtable = (sVEntry<int(void*)>*)mVtbl;
    int n = countVtable[6].fn(vthis(this, countVtable[6]));
    for (; i < n; i++) {
        // vtable slot 7: GetScriptEntity
        sVEntry<cScriptEntity*(void*, int)>* entityVtable = (sVEntry<cScriptEntity*(void*, int)>*)mVtbl;
        entityVtable[7].fn(vthis(this, entityVtable[7]), i)->PauseEntityScript();
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_ResumeObjScript);
#ifdef SKIP_ASM
/**
 * Resumes the scripts of every script entity the object owns.
 *
 * Loops over the object's virtual GetNumScriptEntities() (slot 6) and calls cScriptEntity::ResumeEntityScript on each
 * virtual GetScriptEntity(i) (slot 7); the base implementation that cScriptObjRider (vtable cScriptObjRider_cScriptObject_vtbl) inherits. The
 * counterpart of PauseObjScript.
 */
void cScriptObject::ResumeObjScript()
{
    int i = 0;
    // vtable slot 6: GetNumScriptEntities
    sVEntry<int(void*)>* countVtable = (sVEntry<int(void*)>*)mVtbl;
    int n = countVtable[6].fn(vthis(this, countVtable[6]));
    for (; i < n; i++) {
        // vtable slot 7: GetScriptEntity
        sVEntry<cScriptEntity*(void*, int)>* entityVtable = (sVEntry<cScriptEntity*(void*, int)>*)mVtbl;
        entityVtable[7].fn(vthis(this, entityVtable[7]), i)->ResumeEntityScript();
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_OnStartObjectUse__FPv);
#ifdef SKIP_ASM
/**
 * Marks a script object as taken by one more user: increments its use count.
 *
 * Adds 1 to the in-use count (mInUse). The player ID argument is ignored, so the C declares only the object. The
 * subclass overrides (cScriptObjCamera_OnStartObjectUse, cScriptObjRider_OnStartObjectUse,
 * cScriptObjEvent_OnStartObjectUse, cScriptObjCameraManager_OnStartObjectUse) call it on their cScriptObject part.
 *
 * @return the new use count (left in $v0 by the code; On Tour's method is void)
 */
int cScriptObject::OnStartObjectUse()
{
    int useCount = mInUse + 1;
    mInUse = useCount;
    return useCount;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", cScriptObject_OnEndObjectUse__FPv);
#ifdef SKIP_ASM
/**
 * Drops a script object's use count (mInUse) by one.
 *
 * @return the new use count
 */
int cScriptObject::OnEndObjectUse()
{
    int useCount = mInUse - 1;
    mInUse = useCount;
    return useCount;
}
#endif
