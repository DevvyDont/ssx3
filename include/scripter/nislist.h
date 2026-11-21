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

// One queued NIS (cutscene) request (On Tour tNISListEntry, 0x1C): the cCircularBuffer<5, tNISListEntry> element of
// tNISList::mList. [U] cCircularBuffer_AddToTail copies 0x1C bytes (view sVidCmd3C80); tNISList_RemoveFromHead /
// PlayNextScript / SkipForward / DoPause read category, scriptIndex, controlBits and scriptHandle (views sDmCmd7598,
// sDmCmd7838, sSeqItem_66D0, sSeqItem_7C08).
struct tNISListEntry {
    int category;                   // 0x00 [O]
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

struct tNISList {
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
