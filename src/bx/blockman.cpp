#include "common.h"
#include "bx/blockman.h"

INCLUDE_ASM("bx/blockman", cBlockMan_init);


//100%
INCLUDE_ASM("bx/blockman", cBlockMan_isBlock);
#ifdef SKIP_ASM
/**
 * Returns whether an address lies inside the block manager's memory (+0x0, size +0x4).
 *
 * The test is offset > 0, so the very first block, at the start of the memory, is reported as outside; likely bug
 * (cBlockMan_free tests offset >= 0). Nothing in the C calls it.
 *
 * @param block  the block manager
 * @param addr  the address to test
 * @return 1 when inside, else 0
 */
extern "C" int cBlockMan_isBlock(cBlockMan* block, int addr) {
    int temp_5;
    int var_3;

    temp_5 = addr - ((int)block->Memory);
    var_3 = 0;
    if (temp_5 > 0) {
        var_3 = temp_5 < (block->MemorySize);
    }
    return var_3;
}
#endif
//100%
INCLUDE_ASM("bx/blockman", cBlockMan_alloc);
#ifdef SKIP_ASM

/**
 * Takes a free block from the first group whose block size fits @p size.
 *
 * Walks the groups (16 bytes each from +0x8, count +0xC): the first one with a block size (+0x8) of at least @p size
 * and a non-empty free list (+0xC) gives up the head of that list. A full group falls through to the next, larger one.
 *
 * @param size  the size needed in bytes
 * @return the block, or null when no group can serve it
 */
extern "C" int* cBlockMan_alloc(cBlockMan* self, unsigned int size) {
    int i;
    tBlockGroup* pool;
    int* head;

    for (i = 0; i < self->NumGroups; i++) {
        pool = (tBlockGroup*)((i << 4) + (int)self->Groups);
        if (pool->Size >= size) {
            head = ((int*)pool->FreeBlocks);
            if (head != 0) {
                (*(int**)&pool->FreeBlocks) = (int*)*head;
                return head;
            }
        }
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("bx/blockman", cBlockMan_free);
#ifdef SKIP_ASM

/**
 * Returns a block to the free list of the group whose memory contains it.
 *
 * Finds the group (16 bytes each from +0x8, count +0xC) whose range (+0x0, size +0x4) holds @p ptr and pushes the block
 * onto its free list (+0xC). A pointer from no group is ignored.
 *
 * @param ptr  the block to free
 */
extern "C" void cBlockMan_free(cBlockMan* self, int* ptr) {
    int i;
    int off;
    tBlockGroup* pool;

    for (i = 0; i < self->NumGroups; i++) {
        pool = (tBlockGroup*)((i << 4) + (int)self->Groups);
        off = (int)ptr - ((int)pool->MemBlock);
        if (off >= 0 && off < pool->MemBlockSize) {
            *ptr = (int)((int*)pool->FreeBlocks);
            (*(int**)&((tBlockGroup*)((i << 4) + (int)self->Groups))->FreeBlocks) = ptr;
            return;
        }
    }
}
#endif
//100%
INCLUDE_ASM("bx/blockman", func_00316CC8);
#ifdef SKIP_ASM
/**
 * Does nothing.
 *
 * Last function of the blockman unit, after the free routine cBlockMan_free. Nothing calls it.
 */
extern "C" void func_00316CC8(void) {

}
#endif
