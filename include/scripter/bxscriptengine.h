#ifndef SCRIPTER_BXSCRIPTENGINE_H
#define SCRIPTER_BXSCRIPTENGINE_H

// cBXScriptEngine: the generic script (in-sequence animation) engine: a database of attached script data, the cInSeq
// player manager and 16 play requests. Its only retail instance is the cSSXScriptEngine (scripter/ssxscriptengine.h),
// which derives from it at offset 0: cSSXScriptEngine : cBXScriptEngine, cVidEngine, cDataManager. So this struct is
// exactly the first 0x500 bytes of cSSXScriptEngine (ssxscriptengine.h writes the same fields inline, with
// mScriptRequests as raw bytes; it can embed tScriptRequestInfo / tBXScriptState from here).
// EA's source is scripter/bxscriptengine.cpp (the July 2003 preview's asserts, builds/x/v2 file_hints); retail keeps
// the
// methods in scripter/bxscriptengine (0x00281F30 SetupBXEngine ... 0x00282CB0 GetRequest), the ctor / dtor in
// scripter/ssxscriptengine (0x00281E80, 0x00281ED8) and the small virtuals in scripter/all_in_one_scripter
// (0x002847E8 ... 0x00284848).
//
// Size 0x500: [R] ResetInstVars initialises 16 requests of 0x24 from 0x2C0 (to 0x500), and cSSXScriptEngine's next
// base, cVidEngine, is constructed at +0x500; [O] On Tour's cBXScriptEngine is also 0x500 with the same offsets.
// Bases (g++ 2.95 puts each base's vptr after that base's own data):
//   cScriptDatabaseObject (0x000, size 0x2AC: 0x2A8 of data, vptr at 0x2A8), cInSeqComm (0x2AC, a vptr only).
// The ctor stores _vt$15cBXScriptEngine = cBXScriptEngine_vtbl (0x004824B8, 12 slots) at 0x2A8 and
// _vt$15cBXScriptEngine$10cInSeqComm (D_00482450, 11 slots, delta -0x2AC) at 0x2AC; the cSSXScriptEngine ctor then
// overwrites both. The class static cBXScriptEngine::mpScriptEngine is cBXScriptEngine_mpScriptEngine (ctor sets it, dtor clears it,
// cBXScriptEngine_GetScriptEngine returns it; still unnamed in config/symbol_addrs.txt).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals yet.
// Evidence tags:
//   [R] retail's ctor (cBXScriptEngine_cBXScriptEngine), SetupBXEngine (allocation tags "InSeqHeap", "InSeq" 0x20,
//       "BXScriptDatabase" count * 8), ResetInstVars and ResetRequest;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cBXScriptEngine, struct tScriptRequestInfo / tBXScriptState /
//       uScriptRequestData): names, types and order; here also the same offsets.

#include "layout.h"
#include "scripter/scriptdatabaseobject.h"

struct cScriptObject;
struct cScriptPlayerMain;
struct cScriptPlayerEntity;
struct cScriptEntity;
struct cInSeq;

// A play request's state (tScriptRequestInfo::state). [O] On Tour's enum eScriptRequestState, names and values.
// [U] RequestAnimationScript: DataPending while the script data is missing, else Ready; PlayScript: Ready ->
// PlayPending; OnInSeqPlay -> Playing; PauseScript -> Paused; ReleaseRequest returns Released; Reset just before
// ResetRequest clears the record (to Invalid).
enum eScriptRequestState {
    keScriptRequestState_Invalid = 0,
    keScriptRequestState_DataPending = 1,
    keScriptRequestState_Ready = 2,
    keScriptRequestState_PlayPending = 3,
    keScriptRequestState_Playing = 4,
    keScriptRequestState_Paused = 5,
    keScriptRequestState_Released = 6,
    keScriptRequestState_Reset = 7
};

// A play request's type (tScriptRequestInfo::requestType). [O] On Tour's enum eScriptRequestType (also
// keScriptRequestType_Num = 2). [U] CreateNewRequest takes the first Unused request; every retail request is an
// Animation one.
enum eScriptRequestType {
    keScriptRequestType_Unused = 0,
    keScriptRequestType_Animation = 1
};

// One attached script (On Tour tBXScriptState, 8 bytes; mpScriptDatabase is an array of mScriptDatabaseSize of them).
struct tBXScriptState {
    const char* pScriptData;        // 0x0 [O]; [R] SetupBXEngine zeroes it; [U] AttachScript stores the data,
                                    //     DetachScript returns and clears it
    unsigned short numLocks;        // 0x4 [O]; [R] SetupBXEngine zeroes it (sh); [U] ReleaseRequest counts it down
    char unk_6[0x2];                // 0x6 padding
};

CHECK_OFFSET(tBXScriptState, pScriptData, 0x0);
CHECK_OFFSET(tBXScriptState, numLocks, 0x4);
CHECK_OFFSET(tBXScriptState, unk_6, 0x6);
CHECK_SIZE(tBXScriptState, 0x8);

// On Tour tScriptAnimRequestData / uScriptRequestData (4 bytes; the union has this one member).
struct tScriptAnimRequestData {
    int animScriptIndex;            // 0x0 [O]; [U] RequestAnimationScript stores the script index;
                                    //     GetRequestScriptIndex returns it once the data is bound
};

union uScriptRequestData {
    tScriptAnimRequestData animFields; // 0x0 [O]
};

// One play request (On Tour tScriptRequestInfo, 0x24). The request handle is its index in mScriptRequests
// (cBXScriptEngine_GetRequest: self + 0x2C0 + handle * 0x24, 0 when requestType is 0).
struct tScriptRequestInfo {
    cScriptPlayerMain* pPlayer;     // 0x00 [O]; [R] ResetInstVars / ResetRequest zero it; [U] the cInSeq player
                                    //      (ForcePlayUpdate: cScriptPlayerMain_UpdateFrame(pPlayer))
    eScriptRequestState state;      // 0x04 [O] eScriptRequestState: 0 Invalid, 1 DataPending, 2 Ready, 3 PlayPending,
                                    //      4 Playing, 5 Paused, 6 Released, 7 Reset; [U] PauseScript, ForcePlayUpdate
                                    //       ...
    uScriptRequestData data;        // 0x08 [O]; [R] CreateNewRequest sets -1 for an animation request
    void* pUserData;                // 0x0C [O]; [R] ResetRequest zeroes it; [U] Set/GetRequestUserData
    short purgeAfterPlay;           // 0x10 [O]; [R] ResetInstVars / ResetRequest zero it (sh)
    short loadPending;              // 0x12 [O]; [R] ResetInstVars / ResetRequest zero it (sh)
    int loadingScriptIndex;         // 0x14 [O]; [R] ResetInstVars / ResetRequest set -1; [U] GetRequestScriptIndex
                                    //      returns it while the request waits for data (state 1)
    float playSpeedRequest;         // 0x18 [O]; [U] SetScriptPlayRate keeps the rate here (swc1) while state is 1;
                                    //      [R] ResetInstVars / ResetRequest clear it (sw $0)
    int requestHandle;              // 0x1C [O]; [R] ResetInstVars sets it to the request's index
    eScriptRequestType requestType; // 0x20 [O] eScriptRequestType: 0 Unused, 1 Animation; [U] CreateNewRequest takes
                                    //       the
                                    //      first request with 0 and stores the type; GetRequest returns 0 for 0
};

CHECK_OFFSET(tScriptRequestInfo, pPlayer, 0x00);
CHECK_OFFSET(tScriptRequestInfo, state, 0x04);
CHECK_OFFSET(tScriptRequestInfo, data, 0x08);
CHECK_OFFSET(tScriptRequestInfo, pUserData, 0x0C);
CHECK_OFFSET(tScriptRequestInfo, purgeAfterPlay, 0x10);
CHECK_OFFSET(tScriptRequestInfo, loadPending, 0x12);
CHECK_OFFSET(tScriptRequestInfo, loadingScriptIndex, 0x14);
CHECK_OFFSET(tScriptRequestInfo, playSpeedRequest, 0x18);
CHECK_OFFSET(tScriptRequestInfo, requestHandle, 0x1C);
CHECK_OFFSET(tScriptRequestInfo, requestType, 0x20);
CHECK_SIZE(tScriptRequestInfo, 0x24);

struct cBXScriptEngine {
    // --- cScriptDatabaseObject (primary base; ctor cScriptDatabaseObject_cScriptDatabaseObject, vtable slots 2-4 are
    //     its AddObject / RemoveObject / IsObjInUseByPlayer)
    cScriptObject* mpObjectDatabase[20];    // 0x000 [O]; [R] cScriptDatabaseObject_cScriptDatabaseObject zeroes 20
                                            //       words
    tScriptEntityRecord mEntityDatabase[30]; // 0x050 [O] tScriptEntityRecord[30] {pEntity, objID, playerID, relation,
                                            //       reservedPlayerID}; [R] the base ctor sets each to {0, -1, -1, 0,
                                            //       -1}
    void** mVtbl;                           // 0x2A8 [R] cBXScriptEngine_vtbl (_vt$15cBXScriptEngine), stored by the
                                            //       ctor
                                            //       and dtor; [U] OnInSeqPlay, ReleaseRequest and
                                            //       RequestAnimationScript
                                            //       make virtual calls through it
    // --- cInSeqComm (second base: a vptr only)
    void** mInSeqCommVtbl;                  // 0x2AC [R] _vt$15cBXScriptEngine$10cInSeqComm (D_00482450); [U]
                                            //       SetupBXEngine
                                            //       hands self + 0x2AC to cInSeq_cInSeq as the comm interface
    // --- cBXScriptEngine
    tBXScriptState* mpScriptDatabase;       // 0x2B0 [O]; [R] SetupBXEngine: new (count * 8, "BXScriptDatabase");
                                            //       PurgeBXEngine frees it
    int mScriptDatabaseSize;                // 0x2B4 [O]; [R] SetupBXEngine stores the count; [U] GetScriptDatabaseSize
    cInSeq* mpInSeq;                        // 0x2B8 [O]; [R] SetupBXEngine: cInSeq_cInSeq(cMemMan_alloc(0x20,
                                            //       "InSeq"));
                                            //       [U] PlayScript, StopScript, PauseAll, Update,
                                            //       GetRequestPlayerByPlayerID
    char* mpInSeqHeap;                      // 0x2BC [O]; [R] SetupBXEngine: new (heapSize, "InSeqHeap"), handed to
                                            //       cInSeq
    tScriptRequestInfo mScriptRequests[16]; // 0x2C0 [O]; [R] ResetInstVars: 16 x 0x24

    // Methods ([O] On Tour's names and parameters). Each keeps its retail symbol through an asm label; the virtual
    // ones (the cInSeqComm hooks) are declared as plain members, since the layout above is flat.
    void SetupBXEngine(int scriptDatabaseSize, int maxUniqueAnimPlayers, int heapSize)
        __asm__("cBXScriptEngine_SetupBXEngine");
    void PurgeBXEngine() __asm__("cBXScriptEngine_PurgeBXEngine");
    void Update() __asm__("cBXScriptEngine_Update");
    bool AttachScript(void* pScriptData, int scriptIndex) __asm__("cBXScriptEngine_AttachScript");
    void* DetachScript(int scriptIndex) __asm__("cBXScriptEngine_DetachScript");
    int RequestAnimationScript(int scriptIndex, void* pUserData) __asm__("cBXScriptEngine_RequestAnimationScript");
    eScriptRequestState PlayScript(int requestHandle, int autoRecycle, int loop) __asm__("cBXScriptEngine_PlayScript");
    void StopScript(int requestHandle) __asm__("cBXScriptEngine_StopScript");
    void StopScriptAll() __asm__("cBXScriptEngine_StopScriptAll");
    eScriptRequestState ReleaseRequest(int requestHandle) __asm__("cBXScriptEngine_ReleaseRequest");
    void ReleaseRequestAll() __asm__("cBXScriptEngine_ReleaseRequestAll");
    bool ForcePlayUpdate(int requestHandle) __asm__("cBXScriptEngine_ForcePlayUpdate");
    bool SetScriptPlayRate(int requestHandle, float playRate) __asm__("cBXScriptEngine_SetScriptPlayRate");
    void* SetRequestUserData(cScriptPlayerMain* pPlayer, void* pUserData) __asm__("cBXScriptEngine_SetRequestUserData");
    void* SetRequestUserData(int requestHandle, void* pUserData) __asm__("cBXScriptEngine_SetRequestUserData1");
    void PauseAll() __asm__("cBXScriptEngine_PauseAll");
    void ResumeAll() __asm__("cBXScriptEngine_ResumeAll");
    void PauseScript(int requestHandle) __asm__("cBXScriptEngine_PauseScript");
    void ResumeScript(int requestHandle) __asm__("cBXScriptEngine_ResumeScript");
    eScriptRequestState GetRequestState(int requestHandle) __asm__("cBXScriptEngine_GetRequestState");
    int GetRequestScriptIndex(int requestHandle) __asm__("cBXScriptEngine_GetRequestScriptIndex");
    bool IsRequestPlaybackEnding(int requestHandle) __asm__("cBXScriptEngine_IsRequestPlaybackEnding");
    void* GetRequestUserData(cScriptPlayerMain* pPlayer) __asm__("cBXScriptEngine_GetRequestUserData__FPvT0");
    void* GetRequestUserData(int requestHandle) __asm__("cBXScriptEngine_GetRequestUserData1");
    int GetRequestHandleFromPlayer(cScriptPlayerMain* pPlayer)
        __asm__("cBXScriptEngine_GetRequestHandleFromPlayer__FPvT0");
    cScriptPlayerMain* GetRequestPlayerByReqHandle(int requestHandle)
        __asm__("cBXScriptEngine_GetRequestPlayerByReqHandle");
    cScriptPlayerMain* GetRequestPlayerByPlayerID(int playerID)
        __asm__("cBXScriptEngine_GetRequestPlayerByPlayerID__FPv");
    void OnInSeqPlay(cScriptPlayerMain* pPlayer) __asm__("cBXScriptEngine_OnInSeqPlay");
    void OnInSeqStop(cScriptPlayerMain* pPlayer) __asm__("cBXScriptEngine_OnInSeqStop");
    cScriptEntity* OnInSeqRequestEntityCommInterface(cScriptPlayerEntity* pEntity, int type)
        __asm__("cBXScriptEngine_OnInSeqRequestEntityCommInterface");
    void OnInSeqRemoteScriptPlayerReady(cScriptPlayerMain* pPlayer)
        __asm__("cBXScriptEngine_OnInSeqRemoteScriptPlayerReady");
    void OnInSeqRemoteScriptPlayerDiscard(cScriptPlayerMain* pPlayer)
        __asm__("cBXScriptEngine_OnInSeqRemoteScriptPlayerDiscard");
    void ResetInstVars() __asm__("cBXScriptEngine_ResetInstVars");
    void UpdatePendingRequestState(tScriptRequestInfo* pRequest) __asm__("cBXScriptEngine_UpdatePendingRequestState");
    tScriptRequestInfo* CreateNewRequest(eScriptRequestType requestType) __asm__("cBXScriptEngine_CreateNewRequest");
    void ResetRequest(tScriptRequestInfo* pRequest) __asm__("cBXScriptEngine_ResetRequest__FPvT0");
    tScriptRequestInfo* GetRequest(int requestHandle) __asm__("cBXScriptEngine_GetRequest");
};

CHECK_OFFSET(cBXScriptEngine, mpObjectDatabase, 0x000);
CHECK_OFFSET(cBXScriptEngine, mEntityDatabase, 0x050);
CHECK_OFFSET(cBXScriptEngine, mVtbl, 0x2A8);
CHECK_OFFSET(cBXScriptEngine, mInSeqCommVtbl, 0x2AC);
CHECK_OFFSET(cBXScriptEngine, mpScriptDatabase, 0x2B0);
CHECK_OFFSET(cBXScriptEngine, mScriptDatabaseSize, 0x2B4);
CHECK_OFFSET(cBXScriptEngine, mpInSeq, 0x2B8);
CHECK_OFFSET(cBXScriptEngine, mpInSeqHeap, 0x2BC);
CHECK_OFFSET(cBXScriptEngine, mScriptRequests, 0x2C0);
CHECK_SIZE(cBXScriptEngine, 0x500);

#endif /* SCRIPTER_BXSCRIPTENGINE_H */
