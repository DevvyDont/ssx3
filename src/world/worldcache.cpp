#include "common.h"
#include "vtable.h"
#include "world/chunkresolve.h"
#include "bx/appman.h"
#include "render/ps2graphicsman.h"
#include "world/worldview.h"
#include "world/worldcache.h"

//100%
INCLUDE_ASM("world/worldcache", cWorldBlockAllocator_init);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_WORLD_BLOCK_ARRAY[];
extern char str_WORLD_MEMBLOCK[];

struct sWBABlock {
    void* data;
    int size;
    int f08;
    int f0C;
    int used;
    sWBABlock* next;
};

struct sWBAlloc {
    int blockSize;
    unsigned int count;
    int numFree;
    sWBABlock* blocks;
    sWBABlock* freeList;
};

/**
 * Sets up a world block allocator with a fixed number of equal-sized memory blocks.
 *
 * Allocates the block descriptors as "WORLD BLOCK ARRAY" and each block's memory as "WORLD MEMBLOCK" (type
 * 0x25000000, 128-byte aligned), marks every block unused and chains them all into the free list. The world memory
 * manager makes three: 2 blocks of 16 KB, none of 32 KB, and 53 of 128 KB.
 *
 * @param selfp      the allocator
 * @param count      number of blocks
 * @param blockSize  size of each block in bytes
 */
extern "C" void cWorldBlockAllocator_init(void* selfp, int count, int blockSize)
{
    sWBAlloc* self = (sWBAlloc*)selfp;
    self->blockSize = blockSize;
    self->count = count;
    self->numFree = count;
    self->blocks = new (str_WORLD_BLOCK_ARRAY, 0x20000000, 0) sWBABlock[count];
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        self->blocks[i].used = 0;
        self->blocks[i].size = self->blockSize;
        self->blocks[i].f08 = 0;
        self->blocks[i].f0C = 0;
        self->blocks[i].data = new (str_WORLD_MEMBLOCK, 0x25000000, 0) char[self->blockSize];
        self->blocks[i].next = (i < self->count - 1) ? &self->blocks[i + 1] : 0;
    }
    self->freeList = self->blocks;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldBlockAllocator__cWorldBlockAllocator);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

struct cWorldBlockAllocator__cWorldBlockAllocator_sEntry {
    void* data;
    int pad[5];
};

struct cWorldBlockAllocator__cWorldBlockAllocator_sCache {
    int f0;
    unsigned int count;
    int f8;
    cWorldBlockAllocator__cWorldBlockAllocator_sEntry* entries;
};

/**
 * Destroys a cWorldBlockAllocator: frees each block's memory (pBaseMem of the m_uiTotalBlocks entries of
 * m_pBlockArray), then the array, and deletes the object when bit 0 of @p flags is set.
 *
 * @param cache  the cWorldBlockAllocator (this)
 * @param flags  bit 0 set to delete the object
 */
extern "C" void cWorldBlockAllocator__cWorldBlockAllocator(void* cache, int flags)
{
    cWorldBlockAllocator__cWorldBlockAllocator_sCache* self = (cWorldBlockAllocator__cWorldBlockAllocator_sCache*)cache;
    for (unsigned int i = 0; i < self->count; i++) {
        if (self->entries[i].data != 0) {
            cMemMan_free(self->entries[i].data);
        }
    }
    if (self->entries != 0) {
        cMemMan_free(self->entries);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldBlockAllocator_getFreeBlock__FPv);
#ifdef SKIP_ASM
/**
 * Takes a block from the pool's free list: pops m_pFreeBlocks, decrements m_uiNumFreeBlocks and sets the block's
 * bActive.
 *
 * There is no check for an empty free list.
 *
 * @return the tWorldMemBlock
 */
void* cWorldBlockAllocator_getFreeBlock(void* vself)
{
    cWorldBlockAllocator* self = (cWorldBlockAllocator*)vself;
    tWorldMemBlock* item = self->m_pFreeBlocks;
    self->m_pFreeBlocks = (tWorldMemBlock*)(item->pNext);
    *(int*)&self->m_uiNumFreeBlocks -= 1;
    item->bActive = 1;
    return ((void*)item);
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldBlockAllocator_releaseBlock__FPvT0);
#ifdef SKIP_ASM
/**
 * Returns a block to the pool's free list: clears its bActive, pushes it on m_pFreeBlocks and increments
 * m_uiNumFreeBlocks.
 *
 * @param item  the tWorldMemBlock
 */
void cWorldBlockAllocator_releaseBlock(void* vself, void* vitem)
{
    tWorldMemBlock* item = (tWorldMemBlock*)vitem;
    cWorldBlockAllocator* self = (cWorldBlockAllocator*)vself;
    item->bActive = 0;
    item->pNext = (tWorldMemBlock*)((void*)self->m_pFreeBlocks);
    self->m_pFreeBlocks = (tWorldMemBlock*)item;
    *(int*)&self->m_uiNumFreeBlocks += 1;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cSectionBlockAllocator_cSectionBlockAllocator__FPvii);
#ifdef SKIP_ASM
/**
 * Constructs a cSectionBlockAllocator: stores the pool (@p allocator) and the section id (@p sectionId) and clears m_uiNumBlocks,
 * m_pAllocBlocks and m_pMRUBlock.
 *
 * @param allocator  the cWorldBlockAllocator to take blocks from
 * @param sectionId  the section id
 * @return @p self
 */
void* cSectionBlockAllocator_cSectionBlockAllocator(void* self, int allocator, int sectionId)
{
    int zero = 0;
    *(int*)((char*)self + 0x4) = sectionId;
    *(int*)self = allocator;
    *(int*)((char*)self + 0x8) = zero;
    *(int*)((char*)self + 0xc) = zero;
    *(int*)((char*)self + 0x10) = zero;
    return self;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cSectionBlockAllocator__cSectionBlockAllocator);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void cSectionBlockAllocator_releaseBlocks(cSectionBlockAllocator* self);

/**
 * Destroys a cSectionBlockAllocator: returns its blocks (cSectionBlockAllocator_releaseBlocks) and deletes it when bit 0 of @p flags is set.
 *
 * @param flags  bit 0 set to delete the object
 */
extern "C" void cSectionBlockAllocator__cSectionBlockAllocator(void* self, int flags)
{
    cSectionBlockAllocator_releaseBlocks((cSectionBlockAllocator*)self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cSectionBlockAllocator_releaseBlocks);
#ifdef SKIP_ASM
void cWorldBlockAllocator_releaseBlock(void* self, void* item);

/**
 * Returns all of a section arena's blocks to its pool (cWorldBlockAllocator_releaseBlock), counting m_uiNumBlocks down.
 *
 * m_pAllocBlocks ends up null; m_pMRUBlock is not cleared.
 */
extern "C" void cSectionBlockAllocator_releaseBlocks(cSectionBlockAllocator* self)
{
    if (self->m_pAllocBlocks != 0) {
        void* next;
        do {
            void* item = self->m_pAllocBlocks;
            next = *(void**)((char*)item + 0x14);
            cWorldBlockAllocator_releaseBlock(*(void**)self, item);
            self->m_pAllocBlocks = (tWorldMemBlock*)next;
            self->m_uiNumBlocks = (int)self->m_uiNumBlocks - 1;
        } while (next != 0);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cSectionBlockAllocator_allocate);
#ifdef SKIP_ASM
void* cWorldBlockAllocator_getFreeBlock(void* self);

struct sWCBlock_7878 {
    char* data;
    int size;
    int pos;
    int pad;
    int used;
    sWCBlock_7878* next;
};


static inline void NewBlock_7878(cSectionBlockAllocator* self)
{
    sWCBlock_7878* b = (sWCBlock_7878*)cWorldBlockAllocator_getFreeBlock(((void*)self->m_pBlockAllocator));
    b->pos = 0;
    b->pad = 0;
    b->next = ((sWCBlock_7878*)self->m_pAllocBlocks);
    *(sWCBlock_7878**)&self->m_pAllocBlocks = b;
    *(sWCBlock_7878**)&self->m_pMRUBlock = b;
    self->m_uiNumBlocks++;
}

/**
 * Allocates @p size bytes from a section's arena, taking a new block from its pool when no block has room.
 *
 * The alignment is 4 << ((flags >> 24) & 0xF). The most recently used block (m_pMRUBlock) is tried first, then (with
 * two or more blocks) every other block on m_pAllocBlocks; a block fits only when offset + padding + size is strictly
 * below iBaseSize, and the padding is added to its iPad. Otherwise cWorldBlockAllocator_getFreeBlock gives a new block, pushed on the list
 * and made the MRU, and the allocation starts at its base. Nothing checks that @p size fits in an empty block or that
 * the pool has a free block.
 *
 * @param size  the bytes wanted
 * @param flags  bits 24-27 select the alignment
 * @return the memory
 */
extern "C" char* cSectionBlockAllocator_allocate(cSectionBlockAllocator* self, int size, unsigned int flags)
{
    if (self->m_uiNumBlocks == 0)
        NewBlock_7878(self);
    sWCBlock_7878* first = ((sWCBlock_7878*)self->m_pMRUBlock);
    int align = 4 << ((flags >> 24) & 0xF);
    int pos = first->pos;
    int rem = pos % align;
    int alignPad = 0;
    if (rem)
        alignPad = align - rem;
    if (pos + alignPad + size < first->size)
    {
        first->pos = pos + (alignPad + size);
        char* allocPtr = first->data + pos;
        allocPtr += alignPad;
        ((sWCBlock_7878*)self->m_pMRUBlock)->pad += alignPad;
        return allocPtr;
    }
    if (self->m_uiNumBlocks >= 2)
    {
        *(sWCBlock_7878**)&self->m_pMRUBlock = ((sWCBlock_7878*)self->m_pAllocBlocks);
        if (((sWCBlock_7878*)self->m_pMRUBlock))
        {
            do
            {
                sWCBlock_7878* block = ((sWCBlock_7878*)self->m_pMRUBlock);
                if (block != first)
                {
                    int bpos = block->pos;
                    rem = bpos % align;
                    alignPad = 0;
                    if (rem)
                        alignPad = align - rem;
                    if (bpos + alignPad + size < block->size)
                    {
                        block->pos = bpos + (alignPad + size);
                        char* allocPtr = block->data + bpos;
                        allocPtr += alignPad;
                        ((sWCBlock_7878*)self->m_pMRUBlock)->pad += alignPad;
                        return allocPtr;
                    }
                }
                *(sWCBlock_7878**)&self->m_pMRUBlock = ((sWCBlock_7878*)self->m_pMRUBlock)->next;
            } while (((sWCBlock_7878*)self->m_pMRUBlock));
        }
    }
    NewBlock_7878(self);
    ((sWCBlock_7878*)self->m_pMRUBlock)->pos += size;
    return ((sWCBlock_7878*)self->m_pMRUBlock)->data;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldMemoryMan_cWorldMemoryMan);
#ifdef SKIP_ASM
struct cWorldMemoryMan_cWorldMemoryMan_sCfg {
    int id;
    int count;     // blocks in the pool (e.g. 53)
    int blockSize; // bytes per block (e.g. 128 KB)
};
extern cWorldMemoryMan_cWorldMemoryMan_sCfg D_0044C1E0[];
extern "C" void cWorldBlockAllocator_init(void* self, int count, int blockSize);

/**
 * Constructs the world memory manager: three block allocators sized from D_0044C1E0 and 64 empty section
 * allocators.
 *
 * @return @p self
 */
extern "C" void* cWorldMemoryMan_cWorldMemoryMan(cWorldMemoryMan* self)
{
    cWorldBlockAllocator_init(self, D_0044C1E0[0].count, D_0044C1E0[0].blockSize);
    cWorldBlockAllocator_init(&self->m_BlockAllocator[1], D_0044C1E0[1].count, D_0044C1E0[1].blockSize);
    cWorldBlockAllocator_init(&self->m_BlockAllocator[2], D_0044C1E0[2].count, D_0044C1E0[2].blockSize);
    int i;
    for (i = 63; i >= 0; i--) {
        ((int*)&self->m_pSectionAllocator)[i] = 0;
    }
    return ((void*)self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldMemoryMan__cWorldMemoryMan);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void cWorldBlockAllocator__cWorldBlockAllocator(void* self, int flags);

struct cWorldMemoryMan__cWorldMemoryMan_sElem {
    char pad[0x14];
};

/**
 * Destroys the world memory manager and its memory pools.
 *
 * Unless this is null, runs cWorldBlockAllocator__cWorldBlockAllocator (with flag 0) on the three 0x14-byte entries from the last to the first, then
 * calls operator delete when bit 0 of @p flags is set.
 *
 * @param flags  g++ in-charge flags; bit 0 deletes the object
 */
extern "C" void cWorldMemoryMan__cWorldMemoryMan(cWorldMemoryMan__cWorldMemoryMan_sElem* self, int flags)
{
    if (self != 0) {
        cWorldMemoryMan__cWorldMemoryMan_sElem* blockAllocator = self + 3;
        while (self != blockAllocator) {
            blockAllocator--;
            cWorldBlockAllocator__cWorldBlockAllocator(blockAllocator, 0);
        }
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldMemoryMan_activateSectionMem);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cSectionBlockAllocator_cSectionBlockAllocator(void* self, int section, int index);
extern char str_Section_Allocator[];

struct cWorldMemoryMan_sSection {
    char pad[0x14];
};

/**
 * Gives a world section its own "Section Allocator" drawing from one of the three block allocators.
 *
 * @param index  the section ID (On Tour uiSectionID)
 * @param section  despite the name, the block memory type that picks the allocator (On Tour
 *                 eWorldBlockMemType
 * blocktype)
 */
// PORT: the section pointer is passed as int (the callee's mangled name is __FPvii).
extern "C" void cWorldMemoryMan_activateSectionMem(cWorldMemoryMan* self, int index, int section)
{
    void** mem = (void**)&self->m_pSectionAllocator;
    void** slot = &mem[index];
    *slot = cSectionBlockAllocator_cSectionBlockAllocator(cMemMan_alloc(20, str_Section_Allocator, 0x20000000, 0),
                          (int)((cWorldMemoryMan_sSection*)self + section), index);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldMemoryMan_deactivateSectionMem);
#ifdef SKIP_ASM
extern "C" void cSectionBlockAllocator__cSectionBlockAllocator(void* self, int flags);

/**
 * Destroys the arena of section @p i (cSectionBlockAllocator__cSectionBlockAllocator with 3) and clears its m_pSectionAllocator slot.
 *
 * @param i  the section id
 */
extern "C" void cWorldMemoryMan_deactivateSectionMem(cWorldMemoryMan* self, int i)
{
    char* base = (char*)&self->m_pSectionAllocator[0];
    void** slot = (void**)(base + (i << 2));
    if (*slot != 0) {
        cSectionBlockAllocator__cSectionBlockAllocator(*slot, 3);
    }
    *slot = 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cHullPage_cHullPage);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_cHullPage_Entries[];

/**
 * Constructs a hull page for a BSP hull's data, with an empty entry table sized by the hull's first word
 * ("cHullPage Entries").
 *
 * @param data  the hull data (On Tour const tBxBSPHull* pBSPHull)
 * @return @p self
 */
extern "C" void* cHullPage_cHullPage(cHullPage* self, unsigned short* data)
{
    *(unsigned short**)self = data;
    unsigned short n = *data;
    self->m_uiCurrentPageEntry = 0;
    self->m_uiNumPageEntry = (unsigned int)n;
    self->m_pPageEntries = (tPageEntry*)operator_new_tag(n * 4, str_cHullPage_Entries, 0x20000000, 0);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cHullPage__cHullPage);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

/**
 * Destroys a cHullPage: sets m_uiNumPageEntry to 0, frees m_pPageEntries and deletes the page when bit 0 of @p flags is
 * set.
 *
 * @param flags  bit 0 set to delete the object
 */
extern "C" void cHullPage__cHullPage(int* self, int flags)
{
    *(int*)((char*)self + 0x4) = 0;
    if (*(void**)((char*)self + 0xC) != 0) {
        cMemMan_free(*(void**)((char*)self + 0xC));
    }
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCacheTable_cWorldCacheTable);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* func_00416210(void* dst, int c, int n);
extern char str_WorldCacheTableMem[];

struct sWCTEntry {
    int* ptr;
    int count;
};

struct sWCTable {
    int* pool;
    sWCTEntry entries[28];
    short* sizes;
};

/**
 * Constructs a world cache table with 28 slot lists carved out of one pool.
 *
 * Reads the 28 list lengths from @p data, allocates one zeroed int pool of their total size named
 * "WorldCacheTableMem", and points every non-empty list at its slice of the pool.
 *
 * @param selfp  the table
 * @param data   28 shorts giving each list's length (from the world data header); kept by the table
 * @return @p selfp
 */
extern "C" void* cWorldCacheTable_cWorldCacheTable(cWorldCacheTable* selfp, void* data)
{
    sWCTable* self = (sWCTable*)selfp;
    self->sizes = (short*)data;
    func_00416210(self->entries, 0, sizeof(self->entries));
    int total = 0;
    for (int i = 0; i < 28; i++) {
        int n = self->sizes[i];
        self->entries[i].count = n;
        total += n;
    }
    self->pool = new (str_WorldCacheTableMem, 0, 0) int[total];
    func_00416210(self->pool, 0, total * 4);
    int off = 0;
    int* pool = self->pool;
    for (int j = 0; j < 28; j++) {
        if (self->entries[j].count != 0) {
            self->entries[j].ptr = pool + off;
            off += self->entries[j].count;
        }
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCacheTable__cWorldCacheTable);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

/**
 * Destroys a cWorldCacheTable: frees m_pCacheTableMem and deletes the table when bit 0 of @p flags is set.
 *
 * @param flags  bit 0 set to delete the object
 */
extern "C" void cWorldCacheTable__cWorldCacheTable(int* self, int flags)
{
    if (*(void**)self != 0) {
        cMemMan_free(*(void**)self);
    }
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_cWorldCache);
#ifdef SKIP_ASM
extern "C" void cWorldView_cWorldView(void* self);
extern "C" void cStreamMan_cStreamMan(void* self);

/**
 * Constructs the world cache with its world view and stream manager, nothing loaded, and a touch-stale age of
 * 0.1 s.
 *
 * @return @p self
 */
extern "C" void* cWorldCache_cWorldCache(cWorldCache* self)
{
    cWorldView_cWorldView((char*)&self->m_WorldView);
    cStreamMan_cStreamMan((char*)&self->m_StreamMan);
    char* app = (char*)cAppMan_TheAppMan;
    self->mp_DataHeader = 0;
    self->m_uiNumHullPage = (unsigned int)0;
    self->mp_ChunkResolve = 0;
    self->m_pWorldMemMan = 0;
    self->m_pSharedCacheTable = 0;
    self->m_pSectionCacheTable = 0;
    self->m_iTouchStaleAge = (int)((float)*(int*)(app + 0x10) * 0.10000000149011612f);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache__cWorldCache);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void cWorldCache_purge(cWorldCache* self);
extern "C" void cStreamMan__cStreamMan(void* p, int flags);
extern "C" void cWorldView__cWorldView(cWorldView* p, int flags);

/**
 * Destroys the world cache and its embedded stream manager and world view.
 *
 * Purges the cache, then runs cStreamMan__cStreamMan() on offset 0x300 and cWorldView__cWorldView() on offset 0x10
 * (both with flags 2, so nothing is freed), and frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cWorldCache__cWorldCache(int* self, int flags)
{
    cWorldCache_purge((cWorldCache*)self);
    cStreamMan__cStreamMan((char*)self + 0x300, 2);
    cWorldView__cWorldView((cWorldView*)((char*)self + 0x10), 2);
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_purge);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void cWorldCache_resetPageMap(cWorldCache* self);
extern "C" int cWorldView_isSectionUnLoaded(void* view, int i);
extern "C" void cWorldCache_deactivateSectionMem(cWorldCache* self, int i);
extern "C" void cWorldCacheTable__cWorldCacheTable(int* self, int flags);

struct cWorldCache_purge_sVEntry {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct cWorldCache_purge_sObj {
    int f0;
    int f4;
    cWorldCache_purge_sVEntry* vt;
};



/**
 * Unloads everything the world cache holds: pages, chunk resolver, cache tables, active sections, header and
 * memory manager.
 *
 * @param worldCache  the world cache
 */
extern "C" void cWorldCache_purge(cWorldCache* worldCache)
{
    cWorldCache* self = worldCache;
    cWorldCache_resetPageMap((cWorldCache*)self);
    cWorldCache_purge_sObj* chunkResolve = ((cWorldCache_purge_sObj*)self->mp_ChunkResolve);
    if (chunkResolve != 0) {
        chunkResolve->vt[1].fn(vthis(chunkResolve, chunkResolve->vt[1]), 3);
    }
    if (((int*)self->m_pSharedCacheTable) != 0) {
        cWorldCacheTable__cWorldCacheTable(((int*)self->m_pSharedCacheTable), 3);
    }
    if (((void*)self->m_pSectionCacheTable) != 0) {
        unsigned int i;
        for (i = 0; i < (self->mp_DataHeader)->m_uiNumSections; i++) {
            if (cWorldView_isSectionUnLoaded(((char*)&self->m_WorldView), i) == 0) {
                cWorldCache_deactivateSectionMem((cWorldCache*)self, i);
            }
        }
        if (((void*)self->m_pSectionCacheTable) != 0) {
            cMemMan_free(((void*)self->m_pSectionCacheTable));
        }
    }
    if ((self->mp_DataHeader) != 0) {
        cMemMan_free((self->mp_DataHeader));
    }
    if (((cWorldMemoryMan__cWorldMemoryMan_sElem*)self->m_pWorldMemMan) != 0) {
        cWorldMemoryMan__cWorldMemoryMan(((cWorldMemoryMan__cWorldMemoryMan_sElem*)self->m_pWorldMemMan), 3);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_reset);
#ifdef SKIP_ASM
extern "C" void cWorldCache_purge(cWorldCache* self);

/**
 * Purges the world cache, then clears its data header, hull page count and chunk resolver pointer.
 *
 * cWorldCache_purge() deletes the chunk resolver and the cache tables; reset then zeroes mp_DataHeader,
 * m_uiNumHullPage and mp_ChunkResolve.
 */
extern "C" void cWorldCache_reset(cWorldCache* self)
{
    cWorldCache_purge(self);
    self->mp_DataHeader = 0;
    self->m_uiNumHullPage = (unsigned int)0;
    self->mp_ChunkResolve = 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cWorldMemoryMan_cWorldMemoryMan(cWorldMemoryMan* self);
extern "C" void* cWorldCacheTable_cWorldCacheTable(cWorldCacheTable* self, void* data);
extern "C" void* FILE_load(const char* name, int flags);
extern "C" void* cChunkResolve_construct(void);
extern "C" int cStreamMan_init(void* self, const char* name, void* owner);
// PORT: the unit defines cWorldView_init(sWorldCacheMap*, sWCHdr*, int id); this caller passes the cache in the
// third argument. Bound by asm label with pointer parameters.
extern "C" int cWorldView_init_p(void* self, void* data, void* owner) __asm__("cWorldView_init");
struct cWorldCache_initPageMap_sCache;
extern "C" void cWorldCache_initPageMap(cWorldCache_initPageMap_sCache* self);
extern char str_WORLD_MEMMAN[];
extern char str_SharedTable[];
extern char str_SectionTables[];
extern char str_bar_e[];
extern char str_sdb[];

struct sBXStr_7FC0
{
    char* p;
    sBXStr_7FC0() {}
    sBXStr_7FC0(const sBXStr_7FC0&);
};

extern "C" void* cBXString_cBXString2(sBXStr_7FC0* self, const char* s);
extern "C" sBXStr_7FC0* func_00318630(sBXStr_7FC0* self, sBXStr_7FC0* a, const char* str);
extern "C" sBXStr_7FC0* func_003186D0(sBXStr_7FC0* self, const char* str, sBXStr_7FC0* b);
extern "C" void cBXString__cBXString(sBXStr_7FC0* self, int flags);

struct sVE_7FC0 { short delta; short index; void (*fn)(void*, int); };

/**
 * Loads a world's stream-data header (.sdb) and sets up its memory, cache tables, chunk resolver, streaming
 * and view.
 *
 * @param name  the world's base file name (On Tour baseFileName)
 * @param pNash  the overlap system the chunk resolver uses (On Tour Nash*, passed as int)
 * @return nonzero when the stream manager and the world view started, otherwise 0
 */
extern "C" int cWorldCache_init(cWorldCache* self, const char* name, int pNash)
{
    self->m_pWorldMemMan = (cWorldMemoryMan*)cWorldMemoryMan_cWorldMemoryMan((cWorldMemoryMan*)cMemMan_alloc(316, str_WORLD_MEMMAN, 0, 0));
    sBXStr_7FC0 path;
    sBXStr_7FC0 full;
    sBXStr_7FC0 base;
    func_003186D0(&full, str_bar_e, (cBXString_cBXString2(&base, name), &base));
    func_00318630(&path, &full, str_sdb);
    *(void**)self = FILE_load(path.p, 0x2000000);
    cBXString__cBXString(&path, 2);
    cBXString__cBXString(&full, 2);
    cBXString__cBXString(&base, 2);
    tStreamDataHeader* hdr = self->mp_DataHeader;
    hdr->m_pSection = (struct tBxSection*)(((char*)hdr) + 0x50);
    self->m_pSharedCacheTable = (cWorldCacheTable*)cWorldCacheTable_cWorldCacheTable((cWorldCacheTable*)cMemMan_alloc(232, str_SharedTable, 0, 0), &self->mp_DataHeader->m_iTable[0]);
    self->m_pSectionCacheTable = (cWorldCacheTable**)((void**)operator_new_tag((int)self->mp_DataHeader->m_uiNumSections * 4, str_SectionTables, 0, 0));
    for (unsigned int i = 0; i < self->mp_DataHeader->m_uiNumSections; i++)
        ((void**)self->m_pSectionCacheTable)[i] = 0;
    char* x = (char*)cChunkResolve_construct();
    self->mp_ChunkResolve = (cChunkResolve*)x;
    sVE_7FC0* vt = *(sVE_7FC0**)(x + 8);
    vt[2].fn(vthis(x, vt[2]), pNash);
    if (cStreamMan_init((char*)&self->m_StreamMan, name, self) == 0)
        return 0;
    int viewOk = cWorldView_init_p((char*)&self->m_WorldView, self->mp_DataHeader, self);
    if (viewOk == 0)
        return 0;
    cWorldCache_initPageMap((cWorldCache_initPageMap_sCache*)self);
    return viewOk;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldCache_activateSectionMem);
#ifdef SKIP_ASM
extern char str_SectionTable[];
extern "C" void* cWorldCacheTable_cWorldCacheTable(cWorldCacheTable* self, void* data);

/**
 * Activates a world section's memory: its section allocator and a "SectionTable" cache table from its header
 * entry.
 *
 * @param i  the section ID (On Tour uiSectionID)
 * @param section  despite the name, the block memory type (On Tour eWorldBlockMemType blocktype)
 */
extern "C" void cWorldCache_activateSectionMem(cWorldCache* self, int i, int section)
{
    cWorldMemoryMan_activateSectionMem(self->m_pWorldMemMan, i, section);
    // PORT: pointer held in int (index-first address arithmetic)
    void** slot = (void**)(i * 4 + (int)self->m_pSectionCacheTable);
    *slot = cWorldCacheTable_cWorldCacheTable((cWorldCacheTable*)cMemMan_alloc(232, str_SectionTable, 0, 0),
        &self->mp_DataHeader->m_pSection[i].m_iTable[0]);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldCache_deactivateSectionMem);
#ifdef SKIP_ASM
extern "C" void cWorldMemoryMan_deactivateSectionMem(cWorldMemoryMan* self, int i);
extern "C" void cWorldCacheTable__cWorldCacheTable(int* self, int flags);

/**
 * Releases a world section's allocator and deletes its cache table.
 *
 * @param i  the section ID (On Tour uiSectionID)
 */
extern "C" void cWorldCache_deactivateSectionMem(cWorldCache* self, int i)
{
    cWorldMemoryMan_deactivateSectionMem(self->m_pWorldMemMan, i);
    int* sectionTable = ((int**)self->m_pSectionCacheTable)[i];
    if (sectionTable != 0) {
        cWorldCacheTable__cWorldCacheTable(sectionTable, 3);
    }
    ((int**)self->m_pSectionCacheTable)[i] = 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_update);
#ifdef SKIP_ASM
extern "C" void cWorldView_update(cWorldView* p);
extern "C" void cWorldCache_updatePages(cWorldCache* self);
extern "C" void cStreamMan_update(void* p);

/**
 * Runs the world cache's per-frame update.
 *
 * Updates the world view (offset 0x10), the cache pages, then the stream manager (offset 0x300) with
 * cStreamMan_update().
 */
extern "C" void cWorldCache_update(cWorldCache* self)
{
    cWorldView_update((cWorldView*)&self->m_WorldView);
    cWorldCache_updatePages(self);
    cStreamMan_update((char*)&self->m_StreamMan);
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_initPageMap);
#ifdef SKIP_ASM
int cWorldView_getNumHull(void*);

struct cWorldCache_initPageMap_sEntry {
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
};

struct cWorldCache_initPageMap_sCache {
    char pad[0x3EC];
    unsigned int count;   // 0x3EC
    cWorldCache_initPageMap_sEntry entries[1]; // 0x3F0
};

/**
 * Clears the world cache's page map, sizing it from the world view.
 *
 * Stores the view's page count (cWorldView_getNumHull, a word of the view's header) at offset 0x3EC and zeroes the six
 * words of each page entry from offset 0x3F0.
 */
extern "C" void cWorldCache_initPageMap(cWorldCache_initPageMap_sCache* self)
{
    unsigned int i;
    self->count = cWorldView_getNumHull((char*)self + 0x10);
    for (i = 0; i < self->count; i++) {
        self->entries[i].a = 0;
        self->entries[i].b = 0;
        self->entries[i].c = 0;
        self->entries[i].e = 0;
        self->entries[i].f = 0;
        self->entries[i].d = 0;
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_resetPageMap);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void cStreamMan_cancelStreamRequest(void* p, int i);
extern "C" void cWorldCache_purgePage(cWorldCache* self, int i);

struct sWcEnt_8330 {
    int state;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
};

struct sWc_8330 {
    char pad[0x3EC];
    unsigned int count;         // 0x3EC
    sWcEnt_8330 ents[1];        // 0x3F0
};

/**
 * Empties every hull page: cancels pages being streamed, purges loaded ones, and clears each entry.
 *
 * @param vself  the world cache
 */
extern "C" void cWorldCache_resetPageMap(cWorldCache* vself)
{
    sWc_8330* self = (sWc_8330*)vself;
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        switch (self->ents[i].state) {
        case 0:
        case 1:
            break;
        case 2:
            cStreamMan_cancelStreamRequest((char*)self + 0x300, i);
            self->ents[i].state = 4;
            cWorldCache_purgePage((cWorldCache*)self, i);
            break;
        case 3:
        case 4:
            self->ents[i].state = 4;
            cWorldCache_purgePage((cWorldCache*)self, i);
            break;
        }
        self->ents[i].state = 0;
        self->ents[i].f8 = 0;
        self->ents[i].f4 = 0;
        self->ents[i].f10 = 0;
        self->ents[i].f14 = 0;
        self->ents[i].fC = 0;
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_touchPage);
#ifdef SKIP_ASM
struct sWCSlot {
    int state;   // 0x0
    int f4;      // 0x4
    int f8;      // 0x8
    float fC;    // 0xC
    int time;    // 0x10
    int f14;     // 0x14
};

struct sWCSlots {
    char pad[0x3F0];
    sWCSlot slot[256];   // 0x3F0
    int timeBase;        // 0x1BF0
};


/**
 * Marks hull page @p i as wanted: stores the app tick (cAppMan_TheAppMan + 0x1C) in m_iTouchTick and @p priority in
 * m_fPriority, clears m_bLock and moves Empty to Queued and InActive back to Active.
 *
 * @param i  the hull page
 * @param priority  the page's priority
 */
extern "C" void cWorldCache_touchPage(sWCSlots* self, int i, float priority)
{
    self->slot[i].time = cAppMan_TheAppMan->Tick;
    self->slot[i].fC = priority;
    self->slot[i].f4 = 0;
    switch (self->slot[i].state) {
    case 0:
        self->slot[i].state = 1;
        break;
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        self->slot[i].state = 3;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_flushPage);
#ifdef SKIP_ASM

// PORT: callers declare (void*, unsigned short); the body never masks the id
extern "C" void cWorldCache_flushPage_i(sWCSlots* self, int i) __asm__("cWorldCache_flushPage");
/**
 * Lets hull page @p i expire: clears m_bLock and backdates m_iTouchTick to the app tick minus m_iTouchStaleAge.
 *
 * @param i  the hull page
 */
extern "C" void cWorldCache_flushPage_i(sWCSlots* self, int i)
{
    self->slot[i].f4 = 0;
    self->slot[i].time = cAppMan_TheAppMan->Tick - self->timeBase;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_setPageLock);
#ifdef SKIP_ASM
/**
 * Sets hull page @p hullPage's m_bLock to @p lock.
 *
 * @param hullPage  the hull page
 * @param lock  the lock flag
 */
extern "C" void cWorldCache_setPageLock(void* self, int hullPage, int lock)
{
    self = (char*)self + hullPage * 0x18;
    *(int*)((char*)self + 0x3f4) = lock;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_isPageLocked);
#ifdef SKIP_ASM
/**
 * Returns hull page @p hullPage's m_bLock.
 *
 * @param worldCache  the cWorldCache (this)
 * @param hullPage  the hull page
 * @return the lock flag
 */
extern "C" int cWorldCache_isPageLocked(int worldCache, int hullPage) {
    return (*(int *)((char*)(((hullPage * 0x18) + worldCache)) + (0x3F4)));
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_purgePage);
#ifdef SKIP_ASM
struct sHandle_8528 {
    unsigned int type : 8;
    unsigned int index : 24;
};
struct sRef_8528 {
    unsigned char kind;
    unsigned char type;
    unsigned short index;
};
struct sCell_8528 {
    int loaded;             // 0x0
    int f4;
    cHullPage* list;    // 0x8
    int pad[3];
};
struct sCache_8528 {
    char pad[0x3F0];
    sCell_8528 cells[1];    // 0x3F0
};
extern "C" void cWorldCache_removeBxStreamData(void* self, int cell, int kind, sHandle_8528 h);
extern "C" void cHullPage__cHullPage(int* self, int flags);

/**
 * Unloads a hull page: releases every stream resource it references (cWorldCache_removeBxStreamData), then deletes its list.
 *
 * @param vself  the world cache
 * @param cell  the page index (On Tour uiHullPage)
 */
// Unloads a cache cell: reports each referenced object (kind, handle) to cWorldCache_removeBxStreamData, then frees the list.
extern "C" void cWorldCache_purgePage(cWorldCache* vself, int cell)
{
    sCache_8528* self = (sCache_8528*)vself;
    cHullPage* list = self->cells[cell].list;
    sHandle_8528 resHandle;
    *(unsigned int*)&resHandle = 0xFFFFFFFF;  // starts as the invalid handle
    for (int i = ((int)list->m_uiCurrentPageEntry) - 1; i >= 0; i--) {
        resHandle.type = ((sRef_8528*)list->m_pPageEntries)[i].type;
        resHandle.index = ((sRef_8528*)list->m_pPageEntries)[i].index;
        cWorldCache_removeBxStreamData(self, cell, ((sRef_8528*)list->m_pPageEntries)[i].kind, resHandle);
    }
    if (self->cells[cell].list != 0)
        cHullPage__cHullPage((int*)self->cells[cell].list, 3);
    self->cells[cell].list = 0;
    self->cells[cell].loaded = 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_isPageLoaded);
#ifdef SKIP_ASM
/**
 * Tells whether a hull page is loaded (state 3).
 *
 * @param uiHullPage  the page index (On Tour uiHullPage)
 * @return 1 when loaded, otherwise 0
 */
extern "C" int cWorldCache_isPageLoaded(cWorldCache* self, int uiHullPage)
{
    char* pageEntry = (char*)self + uiHullPage * 0x18;
    return *(int*)(pageEntry + 0x3f0) == 3;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_isPageEmpty);
#ifdef SKIP_ASM
/**
 * Tells whether a hull page is empty (state 0).
 *
 * @param uiHullPage  the page index (On Tour uiHullPage)
 * @return 1 when empty, otherwise 0
 */
extern "C" int cWorldCache_isPageEmpty(cWorldCache* self, int uiHullPage)
{
    char* pageEntry = (char*)self + uiHullPage * 0x18;
    return *(int*)(pageEntry + 0x3f0) == 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_signalPageLoadComplete);
#ifdef SKIP_ASM
/**
 * Marks a hull page as loaded (state 3).
 *
 * @param uiHullPage  the page index (On Tour uiHullPage)
 */
extern "C" void cWorldCache_signalPageLoadComplete(cWorldCache* self, int uiHullPage)
{
    char* pageEntry = (char*)self + uiHullPage * 0x18;
    *(int*)(pageEntry + 0x3f0) = 3;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_updatePages);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cHullPage_cHullPage(cHullPage* self, unsigned short* data);
extern "C" void* cWorldView_getHull(void* self, int i);
extern "C" void cStreamMan_addStreamRequest(void* self, int i, int a, int b);
extern "C" void cWorldCache_purgePage(cWorldCache* self, int i);
extern char str_cHullPage[];

// PORT: g++ 2.95 new-expression; the ctor and the tagged allocator are bound to their symbols by asm label.
struct sHullPage_8668 {
    char data[0x10];
    sHullPage_8668(unsigned short* d) __asm__("cHullPage_cHullPage");
    void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
};

struct sWCPSlot_8668 {
    int state;   // 0x0
    int locked;  // 0x4
    sHullPage_8668* page;  // 0x8
    float pri;   // 0xC
    int time;    // 0x10
    int stamp;   // 0x14
};

struct sWCPCache_8668 {
    char pad0[0x390];
    int busy;                   // 0x390
    char pad394[0x3EC - 0x394];
    unsigned int count;         // 0x3EC
    sWCPSlot_8668 slot[256];    // 0x3F0
    int timeBase;               // 0x1BF0
};

struct sVE_8668 { short delta; short index; int (*fn)(void*); };

static inline int Frame_8668()
{
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    sVE_8668* vt = (sVE_8668*)ps2GraphicsMan->mVtbl;
    return vt[114].fn(vthis(ps2GraphicsMan, vt[114]));
}

/**
 * Ages the hull pages and starts streaming the highest-priority requested page when the stream manager is
 * free.
 *
 * Page states: 0 empty, 1 requested, 2 streaming, 3 loaded, 4 being purged. Unlocked pages untouched for the
 * stale
 * age drop their request or start purging; a purge completes 5 frames later.
 *
 * @param worldCache  the world cache
 */
extern "C" void cWorldCache_updatePages(cWorldCache* worldCache)
{
    sWCPCache_8668* self = (sWCPCache_8668*)worldCache;
    int now = cAppMan_TheAppMan->Tick;
    int best = -1;
    float bestPri = -1.0f;
    for (unsigned int i = 0; i < self->count; i++)
    {
        int st = self->slot[i].state;
        if (st == 0)
            continue;
        int expired = 0;
        if (self->slot[i].locked == 0 && now - self->slot[i].time >= self->timeBase)
            expired = 1;
        switch (st)
        {
        case 2:
            break;
        case 1:
            if (expired)
            {
                self->slot[i].state = 0;
                break;
            }
            if (self->slot[i].pri > bestPri)
            {
                bestPri = self->slot[i].pri;
                best = i;
            }
            break;
        case 3:
            if (!expired)
                break;
            self->slot[i].state = 4;
            self->slot[i].stamp = Frame_8668();
            break;
        case 4:
            if (Frame_8668() - self->slot[i].stamp < 5)
                break;
            cWorldCache_purgePage((cWorldCache*)self, i);
            self->slot[i].state = 0;
            break;
        }
    }
    if (best < 0)
        return;
    void* mgr = (char*)self + 0x300;
    if (self->busy != 0)
        return;
    unsigned short* data = (unsigned short*)cWorldView_getHull((char*)self + 0x10, best);
    self->slot[best].page = new (str_cHullPage, 0x20000000, 0) sHullPage_8668(data);
    cStreamMan_addStreamRequest(mgr, best, data[0], *(int*)(data + 2));
    self->slot[best].state = 2;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_flushInActivePages);
#ifdef SKIP_ASM
/**
 * Frees the hull pages that have gone stale.
 *
 * A page that is not locked and whose m_iTouchTick is before the app tick has expired. Queued pages that expired go
 * back to Empty; Active ones that expired become InActive and, like every InActive page, are purged
 * (cWorldCache_purgePage) and set Empty. Loading pages are left alone. Called by cGame_update and cGame_updateStreamLoading.
 *
 * @param worldCache  the cWorldCache
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" void cWorldCache_flushInActivePages(cWorldCache* worldCache)
{
    sWCPCache_8668* self = (sWCPCache_8668*)worldCache;
    int now = cAppMan_TheAppMan->Tick;
    for (unsigned int i = 0; i < self->count; i++)
    {
        int st = self->slot[i].state;
        if (st == 0)
            continue;
        int expired = 0;
        if (self->slot[i].locked == 0 && self->slot[i].time < now)
            expired = 1;
        switch (st)
        {
        case 2:
            break;
        case 1:
            if (expired)
                self->slot[i].state = 0;
            break;
        case 3:
            if (!expired)
                break;
            self->slot[i].state = 4;
        case 4:
            cWorldCache_purgePage((cWorldCache*)self, i);
            self->slot[i].state = 0;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_flushOnePage);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sWcEnt_89C8 {
    int state;
    int f4;
    int f8;
    float fC;
    int f10;
    int f14;
};

struct sWc_89C8 {
    char pad[0x3EC];
    unsigned int count;         // 0x3EC
    sWcEnt_89C8 ents[1];        // 0x3F0
};

extern "C" void cWorldCache_purgePage(cWorldCache* self, int i);

/**
 * Purges the lowest-priority hull page that is Active or InActive, unlocked and below priority 1.0.
 *
 * The page is set InActive, purged (cWorldCache_purgePage) and set Empty. Does nothing when no page qualifies. No C
 * caller in the source.
 *
 * @param vself  the cWorldCache
 */
extern "C" void cWorldCache_flushOnePage(cWorldCache* vself)
{
    sWc_89C8* self = (sWc_89C8*)vself;
    int best = -1;
    float bestPri = 1.0f;
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        int st = self->ents[i].state;
        if (st == 0)
            continue;
        if (st <= 0)
            continue;
        if (st < 3)
            continue;
        if (st >= 5)
            continue;
        if (self->ents[i].f4 != 0)
            continue;
        if (self->ents[i].fC < bestPri) {
            bestPri = self->ents[i].fC;
            best = i;
        }
    }
    if (best >= 0) {
        self->ents[best].state = 4;
        cWorldCache_purgePage((cWorldCache*)self, best);
        self->ents[best].state = 0;
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_addBxStreamDataTest);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sWCPool_7878;
extern "C" char* cSectionBlockAllocator_allocate(cSectionBlockAllocator* self, int size, unsigned int flags);
// PORT: operator_new__FUi really takes (size, tag, flags, d), like cMemMan_alloc.
void* operator_new_bx(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char D_00494D40[];
extern char str_BxStream_AIPATH[];

struct sBxRef_8AA8 {
    unsigned char refs;
    unsigned char pad;
    unsigned short pad2;
};
struct sBxTypeTbl_8AA8 {
    sBxRef_8AA8* refs;
    int count;
};
struct sBxBank_8AA8 {
    int f0;
    sBxTypeTbl_8AA8 types[1];       // 0x4
};
struct sBxItem_8AA8 {
    unsigned char type;
    unsigned char bank;
    unsigned short id;
};
struct sBxList_8AA8 {
    char pad0[0x8];
    int count;                      // 0x8
    sBxItem_8AA8* items;            // 0xC
};
struct sBxSlot_8AA8 {
    sBxList_8AA8* list;
    char pad4[0x14];
};
struct sBxPools_8AA8 {
    char pad0[0x3C];
    cSectionBlockAllocator* pools[1];         // 0x3C
};
struct sWorldCache_8AA8 {
    int f0;
    sBxBank_8AA8* global;           // 0x4
    sBxBank_8AA8** banks;           // 0x8
    sBxPools_8AA8* pools;           // 0xC
    char pad10[0x3E8];
    sBxSlot_8AA8 slots[1];          // 0x3F8
};

static inline void* bxAlloc_8AA8(sBxPools_8AA8* pl, int bank, int size, unsigned int flags)
{
    if (bank == 0xFF)
        return operator_new_bx(size, D_00494D40, flags, 0);
    return cSectionBlockAllocator_allocate(pl->pools[bank], size, flags);
}

static inline int addRef_8AA8(sBxRef_8AA8* r)
{
    if (r->refs != 0) {
        r->refs++;
        return 1;
    }
    r->refs++;
    return 0;
}

static inline void release_8AA8(sBxRef_8AA8* r)
{
    if (--r->refs == 0)
        *(unsigned int*)r = r->refs;
}

/**
 * Reserves storage for one stream resource of a hull page, or records it on the page when it is already
 * resident.
 *
 * The key holds the bank in its low byte (0xFF for the shared bank) and the resource ID above it. Memory
 * comes
 * from a pool picked by the data type; AI paths (type 14) use a "BxStream AIPATH" allocation.
 *
 * @param slot  the hull page (On Tour uiHullPage)
 * @param type  the data type (On Tour eBxStreamDataType dataType)
 * @param key  the resource ID (On Tour tResourceID RID)
 * @param size  the data size (On Tour uiDataSize)
 * @return the storage address; 0 when it is already resident or @p size is 0; 0xFFFFFFFF when allocation
 *         failed
 */
// Adds one stream data item (type, bank|id<<8) to a load list, allocating its storage on first use.
extern "C" unsigned int cWorldCache_addBxStreamDataTest(sWorldCache_8AA8* self, int slot, int type, unsigned int key, int size)
{
    int bank = key & 0xFF;
    unsigned int id = key >> 8;
    sBxBank_8AA8* bankData;
    if (bank == 0xFF)
        bankData = self->global;
    else
        bankData = self->banks[bank];
    if (addRef_8AA8(&bankData->types[type].refs[id])) {
        sBxList_8AA8* slotList = self->slots[slot].list;
        slotList->items[slotList->count].type = type;
        slotList->items[slotList->count].bank = key;
        slotList->items[slotList->count].id = key >> 8;
        slotList->count++;
        return 0;
    }
    if (size == 0)
        return 0;
    void* data;
    switch (type) {
    case 9:
    case 10:
        data = bxAlloc_8AA8(self->pools, bank, size, 0x45000000);
        break;
    case 1:
    case 2:
    case 3:
    case 11:
    case 12:
        data = bxAlloc_8AA8(self->pools, bank, size, 0x22000000);
        break;
    case 14:
        data = operator_new_bx(size, str_BxStream_AIPATH, 0, 0);
        break;
    default:
        data = bxAlloc_8AA8(self->pools, bank, size, 0);
        break;
    }
    if (data != 0)
        return (unsigned int)data;
    release_8AA8(&bankData->types[type].refs[id]);
    return 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_addBxStreamDataSubmit);
#ifdef SKIP_ASM
struct sWCEntry8CD0 {
    unsigned char id;           // 0x0
    unsigned char lo;           // 0x1
    unsigned short hi;          // 0x2
};

struct sWCList8CD0 {
    int f0;
    int f4;
    int count;                  // 0x8
    sWCEntry8CD0* entries;      // 0xC
};

struct sWCGroup8CD0 {
    sWCList8CD0* list;          // 0x0
    char pad4[0x14];
};

struct sWCVt8CD0 {
    short delta;
    short index;
    int (*fn)(void*, int, unsigned int, int);
};

struct sWCObj8CD0 {
    int f0;
    int f4;
    sWCVt8CD0* vt;              // 0x8
};

struct sWCSlot8CD0 {
    int f0;
    int* bits;                  // 0x4
};

struct sWorldCache8CD0 {
    int f0;
    sWCSlot8CD0* deflt;         // 0x4
    sWCSlot8CD0** tables;       // 0x8
    char padC[0x3E8 - 0xC];
    sWCObj8CD0* obj;            // 0x3E8
    char pad3EC[0xC];
    sWCGroup8CD0 groups[1];     // 0x3F8
};

extern int cStreamMan_NULLCHUNK;

/**
 * Registers a streamed resource of hull page @p group: records it in the page's cHullPage, resolves it and stores its
 * address in the cache table.
 *
 * The table is m_pSharedCacheTable when the RID's section byte is 0xFF, else m_pSectionCacheTable[section]. The entry
 * (type, section byte, uid) is appended at m_uiCurrentPageEntry of the page's mp_Page. mp_ChunkResolve's virtual slot 3
 * resolves the data (with 0 when @p chunk is cStreamMan_NULLCHUNK, whose address is then stored); the result >> 2 goes
 * into the high 24 bits of the m_pCacheTable[type] tag, keeping its refcount byte.
 *
 * @param group  the hull page
 * @param id  the eBxStreamDataType
 * @param key  the tResourceID (section byte, uid << 8)
 * @param chunk  the data, or cStreamMan_NULLCHUNK
 */
// PORT: a4 is a pointer carried in an int (compared against &cStreamMan_NULLCHUNK, then shifted).
extern "C" void cWorldCache_addBxStreamDataSubmit(sWorldCache8CD0* self, int group, int id, unsigned int key, int chunk)
{
    sWCSlot8CD0* tbl;
    sWCList8CD0* list;
    unsigned int hi = key >> 8;
    if ((key & 0xFF) == 0xFF) {
        tbl = self->deflt;
    } else {
        tbl = self->tables[key & 0xFF];
    }
    list = self->groups[group].list;
    list->entries[list->count].id = id;
    list->entries[list->count].lo = key;
    list->entries[list->count].hi = key >> 8;
    list->count++;
    if (chunk == (int)&cStreamMan_NULLCHUNK) {
        sWCObj8CD0* callbackObj = self->obj;
        callbackObj->vt[3].fn(vthis(callbackObj, callbackObj->vt[3]), id, key, 0);
    } else {
        sWCObj8CD0* callbackObj = self->obj;
        chunk = callbackObj->vt[3].fn(vthis(callbackObj, callbackObj->vt[3]), id, key, chunk);
    }
    {
        sWCSlot8CD0* slot = &tbl[id];
        slot->bits[hi] = (unsigned char)slot->bits[hi] | ((chunk >> 2) << 8);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_removeBxStreamData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sWCVt_8E20 {
    short delta;
    short index;
    void* (*fn)(void*, int, unsigned int, char*);
};



// Drops one reference; on the last one returns the stored data pointer and clears it.
static inline char* release_8E20(sBxRef_8AA8* r)
{
    if (--r->refs == 0) {
        unsigned int v = *(unsigned int*)r;
        char* d = (char*)((v >> 8) << 2);
        *(unsigned int*)r = v & 0xFF;
        return d;
    }
    return 0;
}

extern "C" void rel_8E20(cWorldCache* self, int cell, int type, unsigned int key) __asm__("cWorldCache_removeBxStreamData");
/**
 * Releases one streamed resource: drops its tag's refcount and, at zero, clears the address and unresolves the data.
 *
 * The table is chosen by the RID's section byte as in cWorldCache_addBxStreamDataSubmit. mp_ChunkResolve's virtual slot 4 gets the type,
 * RID and data (0 for cStreamMan_NULLCHUNK); a non-null result for a shared (0xFF) resource is freed with cMemMan_free.
 * @p cell is not used.
 *
 * @param cell  the hull page (unused)
 * @param type  the eBxStreamDataType
 * @param key  the tResourceID
 */
extern "C" void rel_8E20(cWorldCache* self, int cell, int type, unsigned int key)
{
    int bank = key & 0xFF;
    unsigned int id = key >> 8;
    sBxBank_8AA8* bankData;
    if (bank == 0xFF)
        bankData = ((sBxBank_8AA8*)self->m_pSharedCacheTable);
    else
        bankData = ((sBxBank_8AA8**)self->m_pSectionCacheTable)[bank];
    char* chunk = release_8E20(&bankData->types[type].refs[id]);
    if (chunk != 0) {
        if (chunk == (char*)&cStreamMan_NULLCHUNK) {
            cChunkResolve* callbackObj = (self->mp_ChunkResolve);
            chunk = (char*)((sWCVt_8E20*)callbackObj->mVtbl)[4].fn(vthis(callbackObj, ((sWCVt_8E20*)callbackObj->mVtbl)[4]), type, key, 0);
        } else {
            cChunkResolve* callbackObj = (self->mp_ChunkResolve);
            chunk = (char*)((sWCVt_8E20*)callbackObj->mVtbl)[4].fn(vthis(callbackObj, ((sWCVt_8E20*)callbackObj->mVtbl)[4]), type, key, chunk);
        }
        if (chunk != 0 && bank == 0xFF)
            cMemMan_free(chunk);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_activateSectionPatches);
#ifdef SKIP_ASM
static inline char *refToPtr_8F10(unsigned i) { return (char*)(i << 2); }
static inline char *conv_8F10(unsigned i) { if (i == 0) return 0; return refToPtr_8F10(i); }
/**
 * Marks every patch of section @p idx active: sets bit 0x40 of the halfword at +0xA of each patch record.
 *
 * The count is the section's patch count (m_iTable[1] of its tBxSection, +0x22) and the records come from the section's
 * cache table (m_pCacheTable[1], kBxStreamData_Patch). There is no check for an empty tag (a missing patch would be
 * written at address 0xA).
 *
 * @param idx  the section id
 */
// Sets flag 0x40 on every object listed for cell idx.
extern "C" void cWorldCache_activateSectionPatches(char** self, int idx)
{
    int n = *(short*)(*(char**)(self[0] + 0x14) + idx * 0x58 + 0x22);
    char* list = ((char**)self[2])[idx];
    for (unsigned i = 0; i < n; i++) {
        char* object = conv_8F10(((unsigned*)*(char**)(list + 0xC))[i] >> 8);
        *(unsigned short*)(object + 0xA) |= 0x40;
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView_cWorldView);
#ifdef SKIP_ASM
inline void* operator new[](unsigned int, void* p) { return p; }
// The cache's leading two-entry member array has an empty constructor; the
// vec_mark compiler leaves only its dead loop setup behind.
struct sCacheHead_3A8F80 {
    int v;
    sCacheHead_3A8F80() {}
};
extern "C" void cWorldView_reset_8F80(void* self) __asm__("cWorldView_reset");

// PORT: the unit declares this as void(void*); defined under an asm label with its real type
extern "C" void* cWorldView_cWorldView_impl(void* self) __asm__("cWorldView_cWorldView");
/**
 * Constructs a world view by clearing its state.
 *
 * cWorldView_reset() zeroes the view's counters and its 64 entry pairs, and sets both of its view slots and its two
 * position vectors to t4vect_Origin.
 *
 * @return @p self
 */
extern "C" void* cWorldView_cWorldView_impl(void* self)
{
    new (self) sCacheHead_3A8F80[2];
    cWorldView_reset_8F80(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView__cWorldView);
#ifdef SKIP_ASM
void operator_delete(int*);
void cWorldView_purge(void* self);

/**
 * Destroys a world view: purges it, then frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cWorldView__cWorldView(cWorldView* self, int flags)
{
    cWorldView_purge(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView_init);
#ifdef SKIP_ASM
struct sWCQuad {
    float v[4];
} __attribute__((aligned(16)));

struct sWCNode {
    sWCQuad min;
    sWCQuad max;
    char pad20[0x30];
    int left;
    int right;
    int leaf;
    char pad5C[4];
};

struct sWCLeaf {
    char pad00[0x44];
};

struct sWCSect {
    char pad00[0x1C];
    int node;
    char pad20[0x38];
};

struct sWCHdr {
    char pad00[8];
    unsigned int numSects;
    unsigned int numNodes;
    char pad10[0x40];
};

struct sWCPair {
    int a;
    int b;
};

struct sWorldCacheMap {
    int id;
    sWCHdr* hdr;
    sWCSect* sects;
    sWCNode* nodes;
    sWCLeaf* leaves;
    sWCPair pairs[65];
    char pad21C[4];
    sWCQuad min;
    sWCQuad max;
};

/**
 * Sets the world view up over the stream-data header, turning the section, BSP-node and leaf indices into
 * pointers in place.
 *
 * The world's bounds are taken from the first section's root node.
 *
 * @param hdr  the stream-data header (On Tour tStreamDataHeader* pHeader)
 * @param id  the world cache (On Tour cWorldCache* pWorldCache, passed as int)
 * @return 1
 */
// PORT: offsets in the loaded data are rewritten in place as pointers (pointer in int)
extern "C" int cWorldView_init(sWorldCacheMap* self, sWCHdr* hdr, int id)
{
    unsigned int i;

    char* data;
    self->id = id;
    self->hdr = hdr;
    self->sects = (sWCSect*)((char*)hdr + 0x50);
    data = (char*)self->sects + hdr->numSects * 0x58;
    data = (char*)(((unsigned int)data + 0xF) & 0xFFFFFFF0);
    self->nodes = (sWCNode*)data;
    self->leaves = (sWCLeaf*)(data + hdr->numNodes * 0x60);

    for (i = 0; i < self->hdr->numSects; i++) {
        self->sects[i].node = (int)&self->nodes[self->sects[i].node];
        self->pairs[i].a = 0;
        self->pairs[i].b = 0;
    }

    for (i = 0; i < self->hdr->numNodes; i++) {
        if (self->nodes[i].left >= 0) {
            self->nodes[i].left = (int)&self->nodes[self->nodes[i].left];
        } else {
            self->nodes[i].left = 0;
        }
        if (self->nodes[i].right >= 0) {
            self->nodes[i].right = (int)&self->nodes[self->nodes[i].right];
        } else {
            self->nodes[i].right = 0;
        }
        if (self->nodes[i].leaf >= 0) {
            self->nodes[i].leaf = (int)&self->leaves[self->nodes[i].leaf];
        } else {
            self->nodes[i].leaf = 0;
        }
    }

    self->min = ((sWCNode*)self->sects[0].node)->min;
    self->max = ((sWCNode*)self->sects[0].node)->max;
    return 1;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView_purge__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: this cWorldView_purge has an empty body.
 */
void cWorldView_purge(void* self)
{
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView_reset);
#ifdef SKIP_ASM
struct cWorldView_reset_sVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));
extern cWorldView_reset_sVec4 t4vect_Origin;

extern "C" void cWorldView_setFocus(void* self, unsigned int i, cWorldView_reset_sVec4* v, float f);

struct cWorldView_reset_sPair {
    int a;
    int b;
};

struct cWorldView_reset_sSlot {
    int f0;
    char pad[0x4C];
};

struct cWorldView_reset_sCache {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    cWorldView_reset_sPair pairs[64];
    char pad214[0x220 - 0x214];
    cWorldView_reset_sVec4 v220;
    cWorldView_reset_sVec4 v230;
    cWorldView_reset_sSlot slots[2];
    int f2E0;
    int f2E4;
};

/**
 * Resets a world view to an empty state focused on the origin.
 *
 * Clears +0x0 to +0x10, sets the vectors at +0x220 and +0x230 to t4vect_Origin, zeroes the 64 pairs, clears each of the
 * two focus slots and calls cWorldView_setFocus(self, slot, origin, 0.0f) for it, and clears +0x2E0 and +0x2E4.
 */
extern "C" void cWorldView_reset(cWorldView_reset_sCache* self)
{
    unsigned int i;
    self->f0 = 0;
    self->f4 = 0;
    self->f8 = 0;
    self->fC = 0;
    self->f10 = 0;
    self->v220 = t4vect_Origin;
    self->v230 = t4vect_Origin;
    for (i = 0; i < 64; i++) {
        self->pairs[i].a = 0;
        self->pairs[i].b = 0;
    }
    for (i = 0; i < 2; i++) {
        self->slots[i].f0 = 0;
        cWorldView_setFocus(self, i, &t4vect_Origin, 0.0f);
    }
    self->f2E0 = 0;
    self->f2E4 = 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView_update);
#ifdef SKIP_ASM
extern "C" int func_00416B18(const char* a, const char* b, int n);
struct cWorldView_recursiveFlushPages_sNode;
extern "C" void cWorldView_recursiveFlushPages(void** self, cWorldView_recursiveFlushPages_sNode* node);
extern "C" void cWorldView_recursiveIsEmptyPages(void** self, tBxBSPNode* node, int* ok);
extern "C" void func_003AD1A8();
extern "C" void cWorldView_refreshStreamFocus(void* self, int i);
extern char str_ASKY[];
extern char str_BSKY[];
extern char str_CSKY[];
extern char str_DSKY[];
extern char str_ESKY[];

struct sWCSection_9258 {
    char name[0x18];
    int id;             // 0x18
    void* tree;         // 0x1C
    char pad20[0x58 - 0x20];
};

struct sWCInfo_9258 {
    int f0;
    int f4;
    unsigned int count; // 0x8
};

struct sWCEntry_9258 {
    int state;
    float pri;
};

struct sWCLoader_9258 {
    void* cache;                // 0x0
    sWCInfo_9258* info;         // 0x4
    sWCSection_9258* sections;  // 0x8
    int fC;
    int f10;
    sWCEntry_9258 entries[64];  // 0x14
    char pad214[0x2E0 - 0x214];
    int f2E0;                   // 0x2E0
    int f2E4;                   // 0x2E4
};

struct sVE_9258 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Advances each world section's load state and refreshes the two stream focuses.
 *
 * States: 1 load requested (activates its memory; sky sections ASKY-ESKY use block type 0, others 2), 2
 * loading,
 * 3 unload requested, 4 unloading, 5 and 6 loaded (counted). Newly loaded sections are announced to the
 * renderer
 * (slot 102).
 *
 * @param worldView  the world view
 */
extern "C" void cWorldView_update(cWorldView* worldView)
{
    sWCLoader_9258* self = (sWCLoader_9258*)worldView;
    if (self->info == 0)
        return;
    self->f2E0 = 0;
    self->f2E4 = 0;
    for (unsigned int i = 0; i < self->info->count; i++) {
        switch (self->entries[i].state) {
        case 0:
            break;
        case 1:
            if (func_00416B18(self->sections[i].name, str_ASKY, 4) == 0
                || func_00416B18(self->sections[i].name, str_BSKY, 4) == 0
                || func_00416B18(self->sections[i].name, str_CSKY, 4) == 0
                || func_00416B18(self->sections[i].name, str_DSKY, 4) == 0
                || func_00416B18(self->sections[i].name, str_ESKY, 4) == 0)
                cWorldCache_activateSectionMem((cWorldCache*)self->cache, i, 0);
            else
                cWorldCache_activateSectionMem((cWorldCache*)self->cache, i, 2);
            cWorldCache_touchPage((sWCSlots*)self->cache, self->sections[i].id, self->entries[i].pri);
            cWorldCache_setPageLock(self->cache, self->sections[i].id, 1);
            self->entries[i].state = 2;
            break;
        case 2:
            if (cWorldCache_isPageLoaded((cWorldCache*)self->cache, self->sections[i].id)) {
                self->entries[i].state = 5;
                cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
                sVE_9258* vt = (sVE_9258*)ps2GraphicsMan->mVtbl;
                vt[102].fn(vthis(ps2GraphicsMan, vt[102]), i);
            }
            break;
        case 4:
            if (cWorldCache_isPageEmpty((cWorldCache*)self->cache, self->sections[i].id)) {
                int ok = 1;
                cWorldView_recursiveIsEmptyPages((void**)self, (tBxBSPNode*)self->sections[i].tree, &ok);
                if (ok) {
                    self->entries[i].state = 0;
                    cWorldCache_deactivateSectionMem((cWorldCache*)self->cache, i);
                    func_003AD1A8();
                }
            }
            break;
        case 5:
            self->f2E0++;
            break;
        case 6:
            self->f2E0++;
            self->f2E4++;
            break;
        case 3:
            cWorldCache_flushPage_i((sWCSlots*)self->cache, self->sections[i].id);
            cWorldView_recursiveFlushPages((void**)self, (cWorldView_recursiveFlushPages_sNode*)self->sections[i].tree);
            self->entries[i].state = 4;
            break;
        }
    }
    for (int j = 0; j < 2; j++)
        cWorldView_refreshStreamFocus(self, j);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldView_recursiveFlushPages);
#ifdef SKIP_ASM
extern "C" void cWorldCache_flushPage(void* cache, unsigned short id);

struct cWorldView_recursiveFlushPages_sNode {
    char pad_0x00[0x50];
    cWorldView_recursiveFlushPages_sNode* left;   // 0x50
    cWorldView_recursiveFlushPages_sNode* right;  // 0x54
    unsigned short* section;     // 0x58
};

/**
 * Flushes every hull page under a BSP node (cWorldCache_flushPage), walking both children (+0x50, +0x54).
 *
 * The node's hull (+0x58) gives the page id (m_uiHullIndex). Called by cWorldView_update for an unloading section.
 *
 * @param node  the tBxBSPNode
 */
extern "C" void cWorldView_recursiveFlushPages(void** self, cWorldView_recursiveFlushPages_sNode* node)
{
    if (node->section != 0) {
        cWorldCache_flushPage(*self, node->section[1]);
    }
    if (node->left != 0) {
        cWorldView_recursiveFlushPages(self, node->left);
    }
    if (node->right != 0) {
        cWorldView_recursiveFlushPages(self, node->right);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldView_recursiveIsEmptyPages);
#ifdef SKIP_ASM
extern "C" int cWorldCache_isPageEmpty(cWorldCache* self, int a1);

/**
 * Clears @p ok when any hull page under a BSP node is not empty (cWorldCache_isPageEmpty), stopping at the first.
 *
 * @param node  the tBxBSPNode
 * @param ok  set on entry; cleared when a page is still loaded
 */
extern "C" void cWorldView_recursiveIsEmptyPages(void** self, tBxBSPNode* node, int* ok)
{
    if (*ok != 0) {
        void* pageRef = node->m_pHull;
        if (pageRef != 0 && cWorldCache_isPageEmpty((cWorldCache*)(*self), *(unsigned short*)((char*)pageRef + 2)) == 0) {
            *ok = 0;
            return;
        }
        if (node->m_pInChild != 0) {
            cWorldView_recursiveIsEmptyPages(self, (tBxBSPNode*)(node->m_pInChild), ok);
        }
        if (node->m_pOutChild != 0 && *ok != 0) {
            cWorldView_recursiveIsEmptyPages(self, (tBxBSPNode*)(node->m_pOutChild), ok);
        }
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView_setFocus);
#ifdef SKIP_ASM
struct sWCVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (in-place vector subtract).
static inline void vu0SubFromWC(sWCVec4& a, const sWCVec4& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void vu0AddToWC(sWCVec4& a, const sWCVec4& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(a)
        : "m"(a), "m"(b)
        : "memory");
}

struct tBoundBox_9658 {
    sWCVec4 mn;
    sWCVec4 mx;
    tBoundBox_9658(const sWCVec4& a, const sWCVec4& b) : mn(a), mx(b) {}
};

struct tStreamFocusInfo_9658 {
    int pad0;
    int m_bActive;              // 0x4
    int m_bFocusUpdate;         // 0x8
    int padC;
    sWCVec4 m_v4LoadPos;        // 0x10
    tBoundBox_9658 m_FocusBox;  // 0x20
    float m_fLoadRadius;        // 0x40
    float m_fSafeRadius;        // 0x44
    float m_fSafeTestRadius;    // 0x48
    int pad4C;
};

struct cWorldView_9658 {
    char pad0[0x240];
    tStreamFocusInfo_9658 m_StreamFocus[2];  // 0x240
};

// cWorldView::setFocus
extern "C" void cWorldView_setFocus_impl(cWorldView_9658* self, int id, const sWCVec4& pos, float fRadius) __asm__("cWorldView_setFocus");
/**
 * Points one of the world view's streaming foci at a position.
 *
 * For focus @p id: stores @p pos as the load position and max(@p fRadius, 0) as the load radius, sets the focus box to
 * pos +/- fRadius, marks the focus active and clears its safe radii and update flag.
 *
 * @param id  which stream focus to set (On Tour eStreamFocusID)
 * @param pos  the position to stream around
 * @param fRadius  how far around it to load
 */
extern "C" void cWorldView_setFocus_impl(cWorldView_9658* self, int id, const sWCVec4& pos, float fRadius)
{
    self->m_StreamFocus[id].m_v4LoadPos = pos;
    self->m_StreamFocus[id].m_fLoadRadius = fRadius >? 0.0f;
    self->m_StreamFocus[id].m_FocusBox = tBoundBox_9658(pos, pos);
    {
        sWCVec4 ext;
        ext.x = fRadius;
        ext.y = fRadius;
        ext.z = fRadius;
        ext.w = 0.0f;
        vu0SubFromWC(self->m_StreamFocus[id].m_FocusBox.mn, ext);
        vu0AddToWC(self->m_StreamFocus[id].m_FocusBox.mx, ext);
    }
    self->m_StreamFocus[id].m_bActive = 1;
    self->m_StreamFocus[id].m_fSafeRadius = 0.0f;
    self->m_StreamFocus[id].m_fSafeTestRadius = 0.0f;
    self->m_StreamFocus[id].m_bFocusUpdate = 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView_getFocus);
#ifdef SKIP_ASM
extern "C" void cWorldView_refreshStreamFocus(void* self, int i);

/**
 * Gets a stream focus's current position and safe radius (less a 0.5 margin).
 *
 * @param i  the focus (On Tour eStreamFocusID id)
 * @param outPos  receives the position (On Tour pos)
 * @param outT  receives the safe radius (On Tour fsafeRadius)
 * @return 1 when the focus is active and the radius positive, otherwise 0
 */
extern "C" int cWorldView_getFocus(cWorldView* self, int i, sWCQuad* outPos, float* outT)
{
    char* focusEntry = (char*)self + i * 0x50;
    if (*(int*)(focusEntry + 0x240) == 0) {
        return 0;
    }
    cWorldView_refreshStreamFocus(self, i);
    int valid = 1;
    *outPos = *(sWCQuad*)(focusEntry + 0x250);
    float safeRadius = *(float*)(focusEntry + 0x284) - 0.5f;
    *outT = safeRadius;
    if (!(safeRadius > 0.0f)) {
        valid = 0;
    }
    return valid;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView_getTestFocus);
#ifdef SKIP_ASM
extern "C" void cWorldView_refreshStreamFocus(void* self, int i);

/**
 * Gets a stream focus's position and its test safe radius (less a 0.5 margin).
 *
 * @param i  the focus (On Tour eStreamFocusID id)
 * @param outPos  receives the position (On Tour pos)
 * @param outT  receives the test safe radius (On Tour fsafeRadius)
 * @return 1 when the focus is active and the radius positive, otherwise 0
 */
extern "C" int cWorldView_getTestFocus(cWorldView* self, int i, sWCQuad* outPos, float* outT)
{
    char* focusEntry = (char*)self + i * 0x50;
    if (*(int*)(focusEntry + 0x240) == 0) {
        return 0;
    }
    cWorldView_refreshStreamFocus(self, i);
    int valid = 1;
    *outPos = *(sWCQuad*)(focusEntry + 0x250);
    float safeRadius = *(float*)(focusEntry + 0x288) - 0.5f;
    *outT = safeRadius;
    if (!(safeRadius > 0.0f)) {
        valid = 0;
    }
    return valid;
}
#endif

