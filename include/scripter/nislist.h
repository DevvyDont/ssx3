#ifndef SCRIPTER_NISLIST_H
#define SCRIPTER_NISLIST_H

// tNISList: one queue of NIS (in-game cut-scene) scripts or videos played by the script engine. cSSXScriptEngine owns
// two NIS lists (mpNISLists) and two idle lists (mpIdleLists), each a `new tNISList[2]` in cSSXScriptEngine_Load
// ("NISLists" / "IdleLists", stride 0xCC). Load sets mpNISEngine, mIsCutSceneHandler (0 for the idle lists) and
// mViewIndex of each list.
//
// Size 0xCC: [R] Load's array stride (addiu 0xCC) and the constructor tNISList_tNISList, which writes every word from
// 0xA4 to 0xC8 (the last one). No base class, no virtual functions. EA's source: d:/ssx/code/source/scripter/
// datamanager.cpp per this repo's unit split (On Tour: all_in_one_scripter.cpp); no preview accessors exist.
//
// A FLAT layout, like main/game.h. Member names, order and types are EA's, from SSX On Tour's PSP DWARF ([O]
// struct tNISList, size 0xC4). Retail = On Tour up to mState (0xA4); On Tour's three bools packed at 0xA8-0xAA are
// words in retail (PS2 bools are 4 bytes), so everything after is On Tour + 0x8 (+0x4, +0x8): checked against On
// Tour's release constructor (only mNextPendingScriptIndex gets -1: On Tour 0xB4, retail 0xBC), DoSkip (mDoSkip: On
// Tour 0xC0, retail 0xC8; mNextPendingScriptIndex) and IsInTransition (lbu 0xAA, retail lw 0xB0).
// Enums and bools are words.

#include "layout.h"
#include "scripter/circularbuffer.h"

struct cSSXScriptEngine;

// A script entry's category (tNISListEntry::category below 29; 29 and up are video categories, tNISList_IsVideo).
// [O] On Tour's enum eSSXScriptCategory, names and values. [U] retail uses them as On Tour names them:
// cSSXScriptEngine_PlayTransportSequence queues the heli (12-15) or gondola (18-21) leave, in-transit start and loop
// scripts, the loop's mp variant when more than one player is allocated; EndTransportSequence and
// IsInTransportSequence pair 12/18, 13/19 and 14/20; AppendStartSequence queues 4 and 5 (race start gate, frame);
// AppendEndEventSequence picks 9 or 10 (champion, cheat character) for the podium and 24 / 25 for the rider
// challenge; tNISList_AddByScriptIndex stores keSSXScriptCategory_Num as "no category".
enum eSSXScriptCategory {
    keSSXScriptCategory_wcentryfirsttimearrive = 0,
    keSSXScriptCategory_wcentryfirsttimeunload = 1,
    keSSXScriptCategory_wcracestartestablish = 2,
    keSSXScriptCategory_wcracestartentrance = 3,
    keSSXScriptCategory_wcracestartgate = 4,
    keSSXScriptCategory_wcracestartframe = 5,
    keSSXScriptCategory_wcraceendpodiumplacenone = 6,
    keSSXScriptCategory_wcraceendpodiumplace2 = 7,
    keSSXScriptCategory_wcraceendpodiumplace3 = 8,
    keSSXScriptCategory_wcraceendpodiumchampion = 9,
    keSSXScriptCategory_wcraceendpodiumchampioncheat = 10,
    keSSXScriptCategory_wctransportstart = 11,
    keSSXScriptCategory_wctransporthelileave = 12,
    keSSXScriptCategory_wctransportheliintransitstart = 13,
    keSSXScriptCategory_wctransportheliintransitloop = 14,
    keSSXScriptCategory_wctransportheliintransitloopmp = 15,
    keSSXScriptCategory_wctransporthelibcarrive = 16,
    keSSXScriptCategory_wctransporthelibcunload = 17,
    keSSXScriptCategory_wctransportgondolaleave = 18,
    keSSXScriptCategory_wctransportgondolaintransitstart = 19,
    keSSXScriptCategory_wctransportgondolaintransitloop = 20,
    keSSXScriptCategory_wctransportgondolaintransitloopmp = 21,
    keSSXScriptCategory_wclodgearrive = 22,
    keSSXScriptCategory_wclodgeleave = 23,
    keSSXScriptCategory_wcriderchallengeoneventendintro = 24,
    keSSXScriptCategory_wcriderchallengeoneventendmain = 25,
    keSSXScriptCategory_wcriderchallengeonbc = 26,
    keSSXScriptCategory_wcfirsttimeintrobc = 27,
    keSSXScriptCategory_Num = 28
};

// One queued NIS (cutscene) request (On Tour tNISListEntry, 0x1C): the cCircularBuffer<5, tNISListEntry> element of
// tNISList::mList. [U] cCircularBuffer_AddToTail copies 0x1C bytes (view sVidCmd3C80); tNISList_RemoveFromHead /
// PlayNextScript / SkipForward / DoPause read category, scriptIndex, controlBits and scriptHandle (views sDmCmd7598,
// sDmCmd7838, sSeqItem_66D0, sSeqItem_7C08).
struct tNISListEntry {
    int category;                   // 0x00 [O]; eSSXScriptCategory, or a video category (29 and up)
    int scriptIndex;                // 0x04 [O]
    int controlBits;                // 0x08 [O]
    int scriptHandle;               // 0x0C [O]; [U] the request handle (cBXScriptEngine_GetRequestPlayerByReqHandle)
    int aiCharIndexMain;            // 0x10 [O]
    int aiCharIndexSupport0;        // 0x14 [O]
    int aiCharIndexSupport1;        // 0x18 [O]
};

CHECK_OFFSET(tNISListEntry, category, 0x00);
CHECK_OFFSET(tNISListEntry, scriptIndex, 0x04);
CHECK_OFFSET(tNISListEntry, controlBits, 0x08);
CHECK_OFFSET(tNISListEntry, scriptHandle, 0x0C);
CHECK_OFFSET(tNISListEntry, aiCharIndexMain, 0x10);
CHECK_OFFSET(tNISListEntry, aiCharIndexSupport0, 0x14);
CHECK_OFFSET(tNISListEntry, aiCharIndexSupport1, 0x18);
CHECK_SIZE(tNISListEntry, 0x1C);

// One video's record in the NIS video database (On Tour struct tVideoDB, 0x1C). [O] On Tour's
// tNISList::GetVideoDBRecord(tNISListEntry*) returns a const tVideoDB*; [U] tNISList_DoTransitionToNext (view
// sDmEnt_7980) gets it from tNISList_GetVideoDBRecord and, when transitionOutTime > 0, makes the transition object from
// transitionOut / transitionColour (cSSXScriptEngine_MakeTransitionObject; transitionOut 7 = a screen grab) and starts
// it over transitionOutTime. Enums are words.
struct tVideoDB {
    const char* unk_000;            // 0x00 [O] pFileName
    int transitionOut;              // 0x04 [O] eScriptTransition; [U] MakeTransitionObject's type (7: screen grab)
    int transitionColour;           // 0x08 [O] eScriptTransitionColour; [U] MakeTransitionObject's colour
    float transitionOutTime;        // 0x0C [O]; [U] > 0: the transition-out time
    int unk_010;                    // 0x10 [O] musicEventStart
    int unk_014;                    // 0x14 [O] musicEventSkip
    int unk_018;                    // 0x18 [O] musicEventEnd
};

CHECK_OFFSET(tVideoDB, unk_000, 0x00);
CHECK_OFFSET(tVideoDB, transitionOut, 0x04);
CHECK_OFFSET(tVideoDB, transitionColour, 0x08);
CHECK_OFFSET(tVideoDB, transitionOutTime, 0x0C);
CHECK_OFFSET(tVideoDB, unk_010, 0x10);
CHECK_OFFSET(tVideoDB, unk_014, 0x14);
CHECK_OFFSET(tVideoDB, unk_018, 0x18);
CHECK_SIZE(tVideoDB, 0x1C);

// The script engine's NIS lists (cSSXScriptEngine::mPlayingNISList; keNISList_Num: none playing). [O] eNISList.
enum eNISList {
    keNISList_Main = 0,
    keNISList_AuxTransition = 1,
    keNISList_Num = 2
};

// A NIS list's load / play state (tNISList::mState, tNISList_GetState). [O] eNISListState.
enum eNISListState {
    keNISListState_Idle = 0,
    keNISListState_Loading = 1,
    keNISListState_Ready = 2,
    keNISListState_Playing = 3
};

// A playing NIS list's playback state (tNISList_GetPlayState). [O] eNISListStatePlay.
enum eNISListStatePlay {
    keNISListStatePlay_Stopped = 0,
    keNISListStatePlay_PlayOnce = 1,
    keNISListStatePlay_PlayLoop = 2,
    keNISListStatePlay_Paused = 3,
    keNISListStatePlay_GoingToNext = 4,
    keNISListStatePlay_GoingToStop = 5
};

struct cScriptPlayerMain;

struct tNISList {
    // A ready list's start mode (mPlayOnReady, ReadyList's playOnReady). [O] On Tour's enum tNISList::ePlayOnReady,
    // names and values; [U] tNISList_Update plays a ready list with its transition in for
    // kePlayOnReady_YesWithTransitionIn, without for kePlayOnReady_YesWithNoTransition.
    enum ePlayOnReady {
        kePlayOnReady_No = 0,
        kePlayOnReady_YesWithTransitionIn = 1,
        kePlayOnReady_YesWithNoTransition = 2
    };

    cSSXScriptEngine* mpNISEngine;      // 0x000 [O]; [R] the ctor zeroes it, cSSXScriptEngine_Load stores the engine;
                                        //       [U] read by every method that calls the engine (e.g. DoSkip)
    int mIsCutSceneHandler;             // 0x004 [O] bool; [R] the ctor sets 1, Load sets 0 for the idle lists
    int mViewIndex;                     // 0x008 [O]; [R] Load stores the list index; [U]
                                        //       cSSXScriptEngine_SetNISListViewIndex stores its argument
    cCircularBuffer mList;              // 0x00C [O] cCircularBuffer<5, tNISListEntry>; [R] the ctor runs
                                        //       cCircularBuffer_cCircularBuffer / _Reset on self+0xC; [U] every method
                                        //       passes self+0xC to the cCircularBuffer_* methods
    int mState;                         // 0x0A4 [O] eNISListState (0 idle, 1 loading, 2 ready, 3 playing); [U]
                                        //       tNISList_GetState returns it, IsPausedPendingNextScript tests == 3
    int mPausedOnNextScriptLoading;     // 0x0A8 [O] bool; [U] tNISList_IsPausedPendingNextScript returns it (state 3);
                                        //       DoPause sets it
    int mIsStartTransition;             // 0x0AC [O] bool; [U] set by Play / PlayNextScript / DoTransitionToNext,
                                        //       cleared by Reset and the ctor
    int mInScriptTransition;            // 0x0B0 [O] bool; [U] tNISList_IsInTransition__FPv returns it (On Tour lbu
                                        //       0xAA)
    int mPlayOnReady;                   // 0x0B4 [O] ePlayOnReady; [U] tNISList_ReadyList stores its playOnReady
                                        //       argument;
                                        //       SkipForward reads it
    int mTransitionWaitFrames;          // 0x0B8 [O]; [U] Play / DoTransitionToNext count it; GetPlayState reads it
    int mNextPendingScriptIndex;        // 0x0BC [O]; [R] the ctor's only -1 (On Tour's ctor: -1 at 0xB4); [U] DoSkip,
                                        //       SetupNextScript, PlayNextScript
    int mNumScriptsToSkipInProgress;    // 0x0C0 [O]; [U] SkipForward counts it, Reset and PlayNextScript clear it
    int mNumPause;                      // 0x0C4 [O]; [U] DoPause counts it up and down
    int mDoSkip;                        // 0x0C8 [O] (On Tour 0xC0); [U] DoSkip tests and sets it (On Tour's DoSkip
                                        //       lw/sw 0xC0 at the same statements)

    // Methods bound to their retail symbols by asm labels.
    bool AddByVideoCategory(int videoCategory, int controlBits) __asm__("tNISList_AddByVideoCategory");
    bool AddByScriptCategory(int scriptCategory, int controlBits, int lookup, int aiCharIndexMain,
                            int aiCharIndexSupport0, int aiCharIndexSupport1)
        __asm__("tNISList_AddByScriptCategory");
    bool AddByScriptIndex(int scriptIndex, int controlBits, int aiCharIndexMain, int aiCharIndexSupport0,
                         int aiCharIndexSupport1)
        __asm__("tNISList_AddByScriptIndex");
    bool TransferFromNISList(tNISList* other) __asm__("tNISList_TransferFromNISList");
    int ReadyList(int playOnReady) __asm__("tNISList_ReadyList");
    int SkipForward(int delta, int force) __asm__("tNISList_SkipForward");
    void Reset(int resetTransition) __asm__("tNISList_Reset");
    int GetScriptCategory(int i) __asm__("tNISList_GetScriptCategory");
    int GetVideoCategory(int i) __asm__("tNISList_GetVideoCategory");
    int GetScriptIndex(int i) __asm__("tNISList_GetScriptIndex");
    void* GetScriptPlayer(int i) __asm__("tNISList_GetScriptPlayer");
    int GetState() __asm__("tNISList_GetState");
    int GetPlayState() __asm__("tNISList_GetPlayState");
    void DoSkip() __asm__("tNISList_DoSkip");
    int IsSkipAllowed() __asm__("tNISList_IsSkipAllowed");
    int IsPauseMenuAllowed() __asm__("tNISList_IsPauseMenuAllowed");
    bool IsPendingScriptReady() __asm__("tNISList_IsPendingScriptReady");
    bool IsPlayingScriptOwner(cScriptPlayerMain* obj) __asm__("tNISList_IsPlayingScriptOwner");
    int IsPlayingVideo() __asm__("tNISList_IsPlayingVideo");
    int IsPausedPendingNextScript() __asm__("tNISList_IsPausedPendingNextScript");
    void OnScriptPlay(int requestHandle, cScriptPlayerMain* obj) __asm__("tNISList_OnScriptPlay");
    void OnScriptUpdate(int requestHandle, cScriptPlayerMain* obj) __asm__("tNISList_OnScriptUpdate");
    void OnScriptStopping(int requestHandle, cScriptPlayerMain* obj) __asm__("tNISList_OnScriptStopping");
    void OnScriptStop(int requestHandle, cScriptPlayerMain* obj) __asm__("tNISList_OnScriptStop");
    int SetupNextScript(int index, int skipFlagged) __asm__("tNISList_SetupNextScript");
    void RemoveFromHead(int n) __asm__("tNISList_RemoveFromHead");
    void WaitOnPendingNextScript() __asm__("tNISList_WaitOnPendingNextScript");
    void CheckOnLoading() __asm__("tNISList_CheckOnLoading");
    void PlayNextScriptOnAvailable() __asm__("tNISList_PlayNextScriptOnAvailable");
    void PlayNextScript() __asm__("tNISList_PlayNextScript");
    bool IsVideo(void* entry) __asm__("tNISList_IsVideo");
};

CHECK_OFFSET(tNISList, mpNISEngine, 0x000);
CHECK_OFFSET(tNISList, mIsCutSceneHandler, 0x004);
CHECK_OFFSET(tNISList, mViewIndex, 0x008);
CHECK_OFFSET(tNISList, mList, 0x00C);
CHECK_OFFSET(tNISList, mState, 0x0A4);
CHECK_OFFSET(tNISList, mPausedOnNextScriptLoading, 0x0A8);
CHECK_OFFSET(tNISList, mIsStartTransition, 0x0AC);
CHECK_OFFSET(tNISList, mInScriptTransition, 0x0B0);
CHECK_OFFSET(tNISList, mPlayOnReady, 0x0B4);
CHECK_OFFSET(tNISList, mTransitionWaitFrames, 0x0B8);
CHECK_OFFSET(tNISList, mNextPendingScriptIndex, 0x0BC);
CHECK_OFFSET(tNISList, mNumScriptsToSkipInProgress, 0x0C0);
CHECK_OFFSET(tNISList, mNumPause, 0x0C4);
CHECK_OFFSET(tNISList, mDoSkip, 0x0C8);
CHECK_SIZE(tNISList, 0xCC);

#endif /* SCRIPTER_NISLIST_H */
