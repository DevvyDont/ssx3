#ifndef SCRIPTER_SSXSCRIPTENGINE_H
#define SCRIPTER_SSXSCRIPTENGINE_H

// cSSXScriptEngine: the NIS / in-game cut-scene script engine (one instance: ssxapp's
// `cSSXScriptEngine_cSSXScriptEngine(cMemMan_alloc(0xA50, "ScriptEngine", 0, 0))`; the class static
// cSSXScriptEngine::mpScriptEngine is D_004A34B0, set by the constructor and cleared by the destructor).
// Its methods live in scripter/datamanager (ctor, dtor, InitOnce, DeInitOnce) and scripter/ssxscriptengine
// (0x00278358 Load ... 0x0027BF90 ClearNISPlayListState).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet.
// The real class (On Tour DWARF; retail vtables in builds/x/v2/vtables_v2.tsv) is
//   cSSXScriptEngine : cBXScriptEngine, cVidEngine, cDataManager
//   cBXScriptEngine  : cScriptDatabaseObject (0x000, size 0x2AC, vptr at 0x2A8), cInSeqComm (0x2AC, vptr only)
// and g++ 2.95 puts each base's vptr after that base's own data, so the object holds four vtable pointers:
//   0x2A8 _vt$16cSSXScriptEngine (primary), 0x2AC ...$10cInSeqComm, 0x518 ...$10cVidEngine, 0x528 ...$12cDataManager.
// The bases' fields are written out below (each block says which base it is); member objects
// (camera managers, game-state / event script objects, bank managers) stay raw bytes.
//
// Member names are EA's, from SSX On Tour's PSP DWARF (class cSSXScriptEngine and its bases), placed by:
//   [P] the July 2003 preview: GetActiveNIS = lw 0x548 (retail 0x550, tools/layout_xlate.py "seen"),
//       IsBankAvailable reads the bank array at 0x628 (same offset as retail);
//   [R] retail's own initialisation: the constructor (cSSXScriptEngine_cSSXScriptEngine), InitOnce, Load and the
//       base constructors, with allocation tags read from the retail ELF ("SetupInfoHeap", "NISLists", "IdleLists",
//       "BXScriptDatabase", "InSeq", "InSeqHeap", "tDataInstanceInfo");
//   [U] how retail code uses the field;
//   [O] On Tour's member order. Retail's own block starts at 0x52C, On Tour's at 0x520 (retail's cVidEngine is
//       0x1C, not 0x10), and retail = On Tour + 0xC up to mNumFramesNoSkip; retail then has 50 anchor tables
//       (On Tour 60), 2 camera managers (On Tour 4) and a 0xFC-byte cScriptObjEvent (On Tour 0xF8).
// Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"

struct cScriptObject;
struct cInSeq;
struct cMoviePlayer;
struct tBXScriptState;
struct tDataInstanceInfo;
struct tSSXHiveScriptMasterTableHeader;
struct tSSXHiveScriptMaster;
struct tSSXHiveScriptFilterTableHeader;
struct tSSXHiveScriptFilterCategory;
struct tSSXScriptSetupInfoLinked;
struct tNISList;

// One script data bank (On Tour struct tSSXScriptBankInfo, 0x10; retail clears it with tSSXScriptBankInfo_Clear).
struct tSSXScriptBankInfo {
    void* dataBankHandle; // 0x0 [O]; [R] cDataManager attach handle (SyncLoadScriptBank); [U] detached by PurgeScriptBank
    int dataIdentifier;   // 0x4 [O]; [R] the bank's key (SyncLoadScriptBank, cSSXScriptEngine_AsyncLoadScriptBank); Clear sets -1
    void* asyncHandle;    // 0x8 [O] (int in On Tour); [U] async file, compared with the completed file
                          //     (ProcessAsyncLoadScriptBankComplete), ASYNCFILE_cancel/release
    int dynamicMem;       // 0xC [O] (a bool in On Tour); [R] sw 1 when the engine allocated the data, 0 for a caller buffer
};

CHECK_OFFSET(tSSXScriptBankInfo, dataBankHandle, 0x0);
CHECK_OFFSET(tSSXScriptBankInfo, dataIdentifier, 0x4);
CHECK_OFFSET(tSSXScriptBankInfo, asyncHandle, 0x8);
CHECK_OFFSET(tSSXScriptBankInfo, dynamicMem, 0xC);
CHECK_SIZE(tSSXScriptBankInfo, 0x10);

struct cSSXScriptEngine {
    // --- cScriptDatabaseObject (base of cBXScriptEngine; ctor cScriptDatabaseObject_cScriptDatabaseObject, vtable _vt$21cScriptDatabaseObject)
    cScriptObject* mpObjectDatabase[20];         // 0x000 [O]; [R] cScriptDatabaseObject_cScriptDatabaseObject zeroes 20 words
    char mEntityDatabase[30][0x14];              // 0x050 [O] tScriptEntityRecord[30]; [R] cScriptDatabaseObject_cScriptDatabaseObject sets 30
                                                 //       0x14-byte records to {0, -1, -1, 0, -1}
    void** mVtbl;                                // 0x2A8 [R] _vt$16cSSXScriptEngine (cSSXScriptEngine_vtbl), stored by the ctor
                                                 //       over _vt$15cBXScriptEngine; [U] cSSXScriptEngine_ForcePlayUpdate calls slot 4
    // --- cInSeqComm (second base of cBXScriptEngine: a vptr only)
    void** mInSeqCommVtbl;                       // 0x2AC [R] _vt$16cSSXScriptEngine$10cInSeqComm (D_00482100);
                                                 //       SetupBXEngine hands self+0x2AC to cInSeq_cInSeq
    // --- cBXScriptEngine (ctor cBXScriptEngine_cBXScriptEngine; methods in scripter/bxscriptengine)
    tBXScriptState* mpScriptDatabase;            // 0x2B0 [O]; [R] SetupBXEngine: new (count * 8, "BXScriptDatabase")
    int mScriptDatabaseSize;                     // 0x2B4 [O]; [R] SetupBXEngine stores the count
    cInSeq* mpInSeq;                             // 0x2B8 [O]; [R] SetupBXEngine: cInSeq_cInSeq(alloc(0x20, "InSeq"))
    char* mpInSeqHeap;                           // 0x2BC [O]; [R] SetupBXEngine: new (size, "InSeqHeap")
    char mScriptRequests[16][0x24];              // 0x2C0 [O] tScriptRequestInfo[16]; [R] ResetInstVars: 16 x 0x24
    // --- cVidEngine (ctor cVidEngine_cVidEngine(self+0x500), which zeroes 0x0-0x14 and sets the vptr at 0x18)
    cMoviePlayer* mpPlayer;                      // 0x500 [O]; [R] cVidEngine_ReadyVideo allocates the 0x44-byte player
    int mIsPlaying;                              // 0x504 [O] (a bool in On Tour); [U] cVidEngine_PlayVideo sets 1;
                                                 //       tested by ssxapp.cpp and all_in_one_fe.cpp
    int mFrameUpdated;                           // 0x508 [O] (a bool in On Tour); [U] cVidEngine_UpdateVideo sets 1 while
                                                 //       playing and not paused
    int mNumPause;                               // 0x50C [O]; [U] cVidEngine_PauseVideo counts it up and down
    int unk_510;                                 // 0x510 [U] cVidEngine_PlayVideo saves a GrMan virtual-call result
    int unk_514;                                 // 0x514 [U] cVidEngine_PlayVideo: 1 unless the game's state object is 0 or 2
    void** mVidEngineVtbl;                       // 0x518 [R] _vt$16cSSXScriptEngine$10cVidEngine (D_004821A0)
    // --- cDataManager (ctor cDataManager_cDataManager(self+0x51C, 0, 0x100))
    int mMaxDataInstances;                       // 0x51C [O]; [R] the ctor stores 0x100
    tDataInstanceInfo* mpDataInstanceHeap;       // 0x520 [O]; [R] new (0x100 * 0x18, "tDataInstanceInfo")
    tDataInstanceInfo* mpEmptyDataInstanceList;  // 0x524 [O]; [R] zeroed, then filled by AddToEmptyInstanceList
    void** mDataManagerVtbl;                     // 0x528 [R] _vt$16cSSXScriptEngine$12cDataManager (D_00482168)
    // --- cSSXScriptEngine's own members
    tSSXHiveScriptMasterTableHeader* mpMasterTableHeader; // 0x52C [O]; [R] InitOnce: FILE_loadz(kpScriptMasterTableFilename);
                                                 //       [U] its first word is the script count
    tSSXHiveScriptMaster* mpMasterTable;         // 0x530 [O]; [R] InitOnce: header + 4; [U] 0xC-byte records
                                                 //       {numTimesPlayed, hashID, name}: cSSXScriptEngine_OnStartScript counts a play,
                                                 //       cSSXScriptEngine_AddToNISListByScriptHashID / cSSXScriptEngine_RequestScriptByHashID search by hashID
    char* mpMasterTableDebug;                    // 0x534 [O]; [R] InitOnce: FILE_loadz(kpScriptMasterTableDebugFilename)
    void* mFilterTableAsyncHandle;               // 0x538 [O] (int in On Tour); [R] AsyncLoadTrack opens
                                                 //       ".../scfilter<location>" into it; [U] ASYNCFILE_* (PurgeTrack)
    tSSXHiveScriptFilterTableHeader* mpFilterTableHeader; // 0x53C [O]; [U] cSSXScriptEngine_ProcessAsyncLoadTrackTableComplete stores the loaded table,
                                                 //       PurgeTrack frees it, GetScriptFromCategory tests it
    tSSXHiveScriptFilterCategory* mpCategoryFilterTable; // 0x540 [O]; [U] cSSXScriptEngine_ProcessAsyncLoadTrackTableComplete: header + 4; 8-byte
                                                 //       {numScripts, scripts} records (GetScriptFromCategory)
    tSSXScriptSetupInfoLinked* mpSetupInfoHeap;  // 0x544 [O]; [R] Load: new (0xC8, "SetupInfoHeap"), 10 x 0x14
    tSSXScriptSetupInfoLinked* mpSetupInfoUnused; // 0x548 [O]; [U] free list (next at +0x10): cSSXScriptEngine_AllocScriptSetupInfo pops,
                                                 //       cSSXScriptEngine_ReleaseScriptSetupInfo pushes
    tNISList* mpNISLists;                        // 0x54C [O]; [R] Load: new[2] ("NISLists"), 0xCC each
    int mPlayingNISList;                         // 0x550 [P] GetActiveNIS (preview 0x548; IsNISActive: != 2);
                                                 //       [O] eNISList; [R] the ctor sets 2 (none)
    int mNumFramesNoSkip;                        // 0x554 [O]; [U] cSSXScriptEngine_DisallowNISListSkip stores its arg;
                                                 //       Update counts it down and ignores the skip button until 0
    unsigned int* mpAnchorTableDB[50];           // 0x558 [O] (tResourceID*[60] in On Tour); [R] the ctor zeroes 50;
                                                 //       [U] cSSXScriptEngine_AttachAnchorDataTable / cSSXScriptEngine_DetachAnchorDataTable (Attach/DetachAnchorDataTable)
                                                 //       index it by world section; cSSXScriptEngine_FillOrientationFromObjectOrientationInfo reads codes from it
    int mNumAsyncs;                              // 0x620 [O]; [U] +1 per async load started (AsyncLoadTrack,
                                                 //       cSSXScriptEngine_AsyncLoadScriptBank), -1 when it completes or is cancelled
    tNISList* mpIdleLists;                       // 0x624 [O]; [R] Load: new[2] ("IdleLists"), 0xCC each
    tSSXScriptBankInfo mScriptDataBanks[10];     // 0x628 [O]; [R] the ctor clears 10 with tSSXScriptBankInfo_Clear;
                                                 //       [P] IsBankAvailable reads 0x628 / 0x630 for bank i
    char unk_6C8[0x8];                           // 0x6C8 untouched; likely alignment (On Tour has the same gap
                                                 //       before its 16-byte-aligned camera managers)
    char mObjectCameraManager[2][0x120];         // 0x6D0 [O] cScriptObjCameraManager (4 in On Tour); [R] the ctor
                                                 //       builds 2 x 0x120 (cScriptObjCameraManager_cScriptObjCameraManager); vptr at +0xC (dtor)
    char mObjectGameState[0x1C];                 // 0x910 [O] cScriptObjGS; [R] cScriptObjGS_cScriptObjGS(self+0x910);
                                                 //       vptrs at +0x0 and +0x18, cScriptObject part at +0xC (dtor)
    char mObjectEvent[0xFC];                     // 0x92C [O] cScriptObjEvent (0xF8 in On Tour); [R]
                                                 //       cScriptObjEvent_cScriptObjEvent(self+0x92C) writes up to +0xF8
    char mAnimationBankManager[0x10];            // 0xA28 [O] cScriptAnimBankManager; [R] its ctor (self+0xA28, 0, 10, 0xF0)
    char mSoundBankManager[0x10];                // 0xA38 [O] cScriptSoundBankManager; [R] its ctor (self+0xA38, 0, 10, 0x11)
    int mCurrAttachingBankNum;                   // 0xA48 [O]; [R] the ctor sets 10 (none); [U] SyncLoadScriptBank and
                                                 //       ProcessAsyncLoadScriptBankComplete set it around the cDataManager
                                                 //       attach, OnAttachData reads it
    char unk_A4C[0x4];                           // 0xA4C untouched; tail padding (On Tour also pads 0xCA8 -> 0xCB0)
};

CHECK_OFFSET(cSSXScriptEngine, mpObjectDatabase, 0x000);
CHECK_OFFSET(cSSXScriptEngine, mEntityDatabase, 0x050);
CHECK_OFFSET(cSSXScriptEngine, mVtbl, 0x2A8);
CHECK_OFFSET(cSSXScriptEngine, mInSeqCommVtbl, 0x2AC);
CHECK_OFFSET(cSSXScriptEngine, mpScriptDatabase, 0x2B0);
CHECK_OFFSET(cSSXScriptEngine, mScriptDatabaseSize, 0x2B4);
CHECK_OFFSET(cSSXScriptEngine, mpInSeq, 0x2B8);
CHECK_OFFSET(cSSXScriptEngine, mpInSeqHeap, 0x2BC);
CHECK_OFFSET(cSSXScriptEngine, mScriptRequests, 0x2C0);
CHECK_OFFSET(cSSXScriptEngine, mpPlayer, 0x500);
CHECK_OFFSET(cSSXScriptEngine, mIsPlaying, 0x504);
CHECK_OFFSET(cSSXScriptEngine, mFrameUpdated, 0x508);
CHECK_OFFSET(cSSXScriptEngine, mNumPause, 0x50C);
CHECK_OFFSET(cSSXScriptEngine, unk_510, 0x510);
CHECK_OFFSET(cSSXScriptEngine, unk_514, 0x514);
CHECK_OFFSET(cSSXScriptEngine, mVidEngineVtbl, 0x518);
CHECK_OFFSET(cSSXScriptEngine, mMaxDataInstances, 0x51C);
CHECK_OFFSET(cSSXScriptEngine, mpDataInstanceHeap, 0x520);
CHECK_OFFSET(cSSXScriptEngine, mpEmptyDataInstanceList, 0x524);
CHECK_OFFSET(cSSXScriptEngine, mDataManagerVtbl, 0x528);
CHECK_OFFSET(cSSXScriptEngine, mpMasterTableHeader, 0x52C);
CHECK_OFFSET(cSSXScriptEngine, mpMasterTable, 0x530);
CHECK_OFFSET(cSSXScriptEngine, mpMasterTableDebug, 0x534);
CHECK_OFFSET(cSSXScriptEngine, mFilterTableAsyncHandle, 0x538);
CHECK_OFFSET(cSSXScriptEngine, mpFilterTableHeader, 0x53C);
CHECK_OFFSET(cSSXScriptEngine, mpCategoryFilterTable, 0x540);
CHECK_OFFSET(cSSXScriptEngine, mpSetupInfoHeap, 0x544);
CHECK_OFFSET(cSSXScriptEngine, mpSetupInfoUnused, 0x548);
CHECK_OFFSET(cSSXScriptEngine, mpNISLists, 0x54C);
CHECK_OFFSET(cSSXScriptEngine, mPlayingNISList, 0x550);
CHECK_OFFSET(cSSXScriptEngine, mNumFramesNoSkip, 0x554);
CHECK_OFFSET(cSSXScriptEngine, mpAnchorTableDB, 0x558);
CHECK_OFFSET(cSSXScriptEngine, mNumAsyncs, 0x620);
CHECK_OFFSET(cSSXScriptEngine, mpIdleLists, 0x624);
CHECK_OFFSET(cSSXScriptEngine, mScriptDataBanks, 0x628);
CHECK_OFFSET(cSSXScriptEngine, unk_6C8, 0x6C8);
CHECK_OFFSET(cSSXScriptEngine, mObjectCameraManager, 0x6D0);
CHECK_OFFSET(cSSXScriptEngine, mObjectGameState, 0x910);
CHECK_OFFSET(cSSXScriptEngine, mObjectEvent, 0x92C);
CHECK_OFFSET(cSSXScriptEngine, mAnimationBankManager, 0xA28);
CHECK_OFFSET(cSSXScriptEngine, mSoundBankManager, 0xA38);
CHECK_OFFSET(cSSXScriptEngine, mCurrAttachingBankNum, 0xA48);
CHECK_OFFSET(cSSXScriptEngine, unk_A4C, 0xA4C);
CHECK_SIZE(cSSXScriptEngine, 0xA50);

// The one instance (On Tour: ScriptEngine), created and destroyed by cSSXApp ("ScriptEngine", 0xA50).
extern cSSXScriptEngine* ScriptEngine;

#endif /* SCRIPTER_SSXSCRIPTENGINE_H */
