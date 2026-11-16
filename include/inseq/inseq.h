#ifndef INSEQ_INSEQ_H
#define INSEQ_INSEQ_H

// cInSeq: the in-game sequence (InSeq) manager: a private script heap and a fixed pool of script players
// (cScriptPlayerMain, inseq/scriptplayermain.h), each wrapped in a tScriptMainNode on a free ("unused") or a
// playing ("used") list. ReadyPlayer takes a node off the unused list and initialises its player on a script,
// RecyclePlayer puts it back; Play/Stop/Pause/Resume/Preview forward to the player by ID. One instance:
// cBXScriptEngine::mpInSeq (scripter/ssxscriptengine.h 0x2B8). No virtual functions, no base class, no global.
// Size 0x20 [R]: cBXScriptEngine_SetupBXEngine allocates it with cMemMan_alloc(0x20, "InSeq") and runs
// cInSeq_cInSeq(it, engine->mpInSeqHeap, size, maxUniqueAnimPlayers, engine + 0x2AC = the engine's cInSeqComm
// base). Same size, order and offsets as On Tour's class cInSeq.
// EA's source: z:/main/game/source/inseq/all_in_one_inseq.cpp in SSX On Tour's DWARF (the July 2003 preview has no
// assert or out-of-line accessor for this class), hence inseq/; the header's file name is a guess. Retail code: unit
// replay/replaycache 0x00272788-0x00272D68.
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] the constructor cInSeq_cInSeq (stores every field) and the allocation above;
//   [U] how retail code uses the field (asm of the methods listed in methods.txt);
//   [O] SSX On Tour's DWARF (class cInSeq, struct tScriptMainNode): member names, types and order.

#include "layout.h"
#include "inseq/scriptheap.h"
#include "inseq/scriptplayermain.h"

struct cInSeqComm;          // On Tour: interface, vptr only; cBXScriptEngine implements it (its base at 0x2AC)

// One pool slot (On Tour tScriptMainNode, 0x50). Retail 0x58 [R]: cInSeq_cInSeq allocates maxScripts x 0x58 with
// new[] (func_002724C8, element constructor = cScriptPlayerMain_cScriptPlayerMain on node + 8) and links
// +0x50/+0x54. Retail has one more word than On Tour at the node's start (as cScriptPlayerEntity and the channel
// classes do) and its cScriptPlayerMain is 0x48 (On Tour 0x44), so On Tour's offsets are +4 / +8 here.
struct tScriptMainNode {
    int unk_00;                       // 0x00 retail-only word; no retail access seen
    int index;                        // 0x04 [O]; [R] ctor stores i (the player ID); [U] ReadyPlayer returns it,
                                      //      PlayAll/StopAll/PauseAll/ResumeAll/RecycleAll pass it back by ID
    cScriptPlayerMain script;         // 0x08 [O]; [R] new[]'s element ctor runs on node + 8; [U] GetPlayer returns
                                      //      &script when script.mpData != 0 (lw node + 0xC); UpdateFrame calls
                                      //      cScriptPlayerMain UpdateFrame (cScriptPlayerMain_UpdateFrame) on it
    tScriptMainNode* pNext;           // 0x50 [O]; [R] ctor &node[i + 1], last 0; [U] the lists' forward link
    tScriptMainNode* pPrev;           // 0x54 [O]; [R] ctor &node[i - 1], first 0; [U] RecyclePlayer unlinks by it
};

struct cInSeq {
    cInSeqComm* mpCommInterface;      // 0x00 [O]; [R] ctor stores its 5th argument (cBXScriptEngine + 0x2AC); [U]
                                      //      cScriptPlayerMain GetInSeqCommInterface falls back to
                                      //      mpManager->mpCommInterface
    unsigned int mMaxScripts;         // 0x04 [O]; [R] ctor stores maxScripts, sizes the pool by it and loops to it
                                      //      (sltu)
    cScriptHeap mHeap;                // 0x08 [O]; [R] ctor runs cScriptHeap's constructor cScriptHeap_cScriptHeap on this +
                                      //      8 and allocates the pool from it; [U] the players, entities and
                                      //      channels allocate from mpManager + 8
    tScriptMainNode* mpScriptHeap;    // 0x14 [O]; [R] ctor: the new[]'d pool; [U] GetPlayer / RecyclePlayer index it
                                      //      by ID (stride 0x58)
    tScriptMainNode* mpScriptUnused;  // 0x18 [O]; [R] ctor = mpScriptHeap (every node free); [U] ReadyPlayer pops
                                      //      from it, RecyclePlayer pushes back
    tScriptMainNode* mpScriptUsed;    // 0x1C [O]; [R] ctor 0; [U] ReadyPlayer pushes, RecyclePlayer unlinks;
                                      //      UpdateFrame and the *All methods walk it
};

CHECK_OFFSET(tScriptMainNode, unk_00, 0x00);
CHECK_OFFSET(tScriptMainNode, index, 0x04);
CHECK_OFFSET(tScriptMainNode, script, 0x08);
CHECK_OFFSET(tScriptMainNode, pNext, 0x50);
CHECK_OFFSET(tScriptMainNode, pPrev, 0x54);
CHECK_SIZE(tScriptMainNode, 0x58);

CHECK_OFFSET(cInSeq, mpCommInterface, 0x00);
CHECK_OFFSET(cInSeq, mMaxScripts, 0x04);
CHECK_OFFSET(cInSeq, mHeap, 0x08);
CHECK_OFFSET(cInSeq, mpScriptHeap, 0x14);
CHECK_OFFSET(cInSeq, mpScriptUnused, 0x18);
CHECK_OFFSET(cInSeq, mpScriptUsed, 0x1C);
CHECK_SIZE(cInSeq, 0x20);

#endif /* INSEQ_INSEQ_H */
