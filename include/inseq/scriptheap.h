#ifndef INSEQ_SCRIPTHEAP_H
#define INSEQ_SCRIPTHEAP_H

// cScriptHeap: the in-game sequence (InSeq) system's private first-fit heap. cInSeq embeds one (cInSeq::mHeap,
// inseq/inseq.h) over the buffer cBXScriptEngine allocates ("InSeqHeap"); the script players, entities and channels
// allocate from it through EA's operator new(size, cScriptHeap*) / new[] (operator_new3 / operator_new_array), which
// store the heap pointer in the first kScriptHeapPointerSize (kScriptHeapPointerSize = 4) bytes of each allocation so that
// operator delete (operator_delete1) can find it again. No virtual functions, no base class.
// Size 0xC [R]: cInSeq's members around it are at 0x4 (mMaxScripts) and 0x14 (mpScriptHeap), and cInSeq::cInSeq
// runs the constructor cScriptHeap_cScriptHeap on cInSeq + 0x8. Same size, order and offsets as On Tour's class cScriptHeap.
// EA's source: z:/main/game/source/inseq/all_in_one_inseq.cpp in SSX On Tour's DWARF (the July 2003 preview has no
// assert or out-of-line accessor for it), hence inseq/; the header's file name is a guess. Retail code: unit
// replay/replaycache 0x002722E0-0x00272488.
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] the constructor cScriptHeap_cScriptHeap (stores all three fields) and cInSeq::cInSeq;
//   [U] how retail code uses the field: AllocMem cScriptHeap_AllocMem, FreeMem cScriptHeap_FreeMem;
//   [O] SSX On Tour's DWARF (class cScriptHeap, struct tFreeBlock): member names, types and order.

#include "layout.h"

// A free block's header (On Tour tFreeBlock, 0xC). Allocated blocks keep the header; the memory handed out starts
// at blockStart (header + 8).
struct tFreeBlock {
    unsigned int size;        // 0x0 [O]; [R] ctor: heapSize - 8 for the single initial block; [U] AllocMem
                              //     compares it (sltu) and splits the block, FreeMem coalesces neighbours by it
    tFreeBlock* pNext;        // 0x4 [O]; [R] ctor 0; [U] the free list's link (AllocMem/FreeMem); AllocMem zeroes
                              //     it on the block it returns
    char blockStart;          // 0x8 [O]; [U] AllocMem returns &blockStart (block + 8); FreeMem takes pMem - 8
    char padding[3];          // 0x9 [O]
};

struct cScriptHeap {
    char* mpHeap;             // 0x0 [O]; [R] ctor stores pHeap; no other retail access seen
    int mHeapSize;            // 0x4 [O]; [R] ctor stores heapSize; no other retail access seen (On Tour GetSize()
                              //     returns it, not emitted in retail)
    tFreeBlock* mpTopFreeBlock; // 0x8 [O]; [R] ctor = pHeap (one block of heapSize - 8); [U] head of the
                              //     address-ordered free list: AllocMem unlinks from it, FreeMem inserts/merges

    // Methods bound to their retail symbols by asm labels.
    void FreeMem(void* pMem) __asm__("cScriptHeap_FreeMem");
    void* AllocMem(unsigned int size) __asm__("cScriptHeap_AllocMem");
};

CHECK_OFFSET(tFreeBlock, size, 0x0);
CHECK_OFFSET(tFreeBlock, pNext, 0x4);
CHECK_OFFSET(tFreeBlock, blockStart, 0x8);
CHECK_OFFSET(tFreeBlock, padding, 0x9);
CHECK_SIZE(tFreeBlock, 0xC);

CHECK_OFFSET(cScriptHeap, mpHeap, 0x0);
CHECK_OFFSET(cScriptHeap, mHeapSize, 0x4);
CHECK_OFFSET(cScriptHeap, mpTopFreeBlock, 0x8);
CHECK_SIZE(cScriptHeap, 0xC);

#endif /* INSEQ_SCRIPTHEAP_H */
