#ifndef BX_BLOCKMAN_H
#define BX_BLOCKMAN_H

// cBlockMan: a block allocator over one memory range: the range, its groups of blocks and the alignment. Methods in
// bx/blockman and bx/bigfile.
//
// Size 0x14 (On Tour's; retail's methods reach Allignment at size - 4). Member names, order and offsets are EA's, from
// SSX On Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start
// (tools/gen/self_offsets.py).

#include "layout.h"

// tMemBlock: the header of a block in a group (views group D). [O] On Tour struct tMemBlock {tMemBlock* Next;
// tMemBlock* Prev; const char* Name; const char* File; int Line; int AllocSize;} (0x18, the PSP debug build's
// tracking fields); [U] retail only links free blocks through the first word (cBlockMan_alloc pops FreeBlocks->Next,
// cBlockMan_free pushes), so only Next is placed; the rest of the debug header is not in the retail build.
struct tMemBlock {
    tMemBlock* Next;                // 0x000 [O]; [U] the free-list link
};

CHECK_OFFSET(tMemBlock, Next, 0x000);

// tBlockGroup: one pool of equal-size blocks (views group D: sPool_6C00 / sPool_6C60). [O] On Tour struct tBlockGroup
// {char* MemBlock; int MemBlockSize; unsigned int Size; tMemBlock* FreeBlocks; int Num; tMemBlock* AllocBlocks;} (0x18,
// the debug build); [R] retail's Groups stride is 0x10 (sll 4 in cBlockMan_init / alloc / free) and init stores only
// the first four words, so the debug-only Num / AllocBlocks are not in the retail record.
struct tBlockGroup {
    char* MemBlock;                 // 0x000 [O]; [U] free finds the group whose [MemBlock, MemBlock + MemBlockSize)
                                    //       holds the block
    int MemBlockSize;               // 0x004 [O]
    unsigned int Size;              // 0x008 [O]; [U] alloc takes the first group with Size >= the request
    tMemBlock* FreeBlocks;          // 0x00C [O]; [U] alloc pops, free pushes
};

CHECK_OFFSET(tBlockGroup, MemBlock, 0x000);
CHECK_OFFSET(tBlockGroup, MemBlockSize, 0x004);
CHECK_OFFSET(tBlockGroup, Size, 0x008);
CHECK_OFFSET(tBlockGroup, FreeBlocks, 0x00C);
CHECK_SIZE(tBlockGroup, 0x10);

struct cBlockMan {
    char* Memory;                   // 0x000 [O]
    int MemorySize;                 // 0x004 [O]
    tBlockGroup* Groups;            // 0x008 [O]
    int NumGroups;                  // 0x00C [O]
    int Allignment;                 // 0x010 [O]

    // Methods bound to their retail symbols by asm labels.
    int allignSize(int size) __asm__("cBlockMan_allignSize");
    int isBlock(int addr) __asm__("cBlockMan_isBlock");
    int* alloc(unsigned int size) __asm__("cBlockMan_alloc");
    void free(int* ptr) __asm__("cBlockMan_free");
};

CHECK_OFFSET(cBlockMan, Memory, 0x000);
CHECK_OFFSET(cBlockMan, MemorySize, 0x004);
CHECK_OFFSET(cBlockMan, Groups, 0x008);
CHECK_OFFSET(cBlockMan, NumGroups, 0x00C);
CHECK_OFFSET(cBlockMan, Allignment, 0x010);
CHECK_SIZE(cBlockMan, 0x14);

#endif
