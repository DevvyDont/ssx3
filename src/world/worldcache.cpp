#include "common.h"
#include "bx/math3d.h"
#include "vtable.h"
#include "world/chunkresolve.h"
#include "bx/appman.h"
#include "render/ps2graphicsman.h"
#include "world/worldview.h"
#include "world/worldcache.h"
#include "world/patchrecord.h"

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
 * @param count      number of blocks
 * @param blockSize  size of each block in bytes
 */
void cWorldBlockAllocator::init(int count, int blockSize)
{
    sWBAlloc* self = (sWBAlloc*)this;
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
void* cWorldBlockAllocator::getFreeBlock()
{
    tWorldMemBlock* item = m_pFreeBlocks;
    m_pFreeBlocks = (tWorldMemBlock*)(item->pNext);
    *(int*)&m_uiNumFreeBlocks -= 1;
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
 * @param vitem  the tWorldMemBlock
 */
void cWorldBlockAllocator::releaseBlock(void* vitem)
{
    tWorldMemBlock* item = (tWorldMemBlock*)vitem;
    item->bActive = 0;
    item->pNext = (tWorldMemBlock*)((void*)m_pFreeBlocks);
    m_pFreeBlocks = (tWorldMemBlock*)item;
    *(int*)&m_uiNumFreeBlocks += 1;
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
void* cSectionBlockAllocator_cSectionBlockAllocator(void* vself, int allocator, int sectionId)
{
    cSectionBlockAllocator* self = (cSectionBlockAllocator*)vself;
    cSectionBlockAllocator* arena = (cSectionBlockAllocator*)self;
    arena->m_uiSectionID = sectionId;
    arena->m_pBlockAllocator = (cWorldBlockAllocator*)allocator;
    arena->m_uiNumBlocks = 0;
    arena->m_pAllocBlocks = 0;
    arena->m_pMRUBlock = 0;
    return ((void*)self);
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
extern "C" void cSectionBlockAllocator__cSectionBlockAllocator(cSectionBlockAllocator* self, int flags)
{
    self->releaseBlocks();
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
void cSectionBlockAllocator::releaseBlocks()
{
    if (m_pAllocBlocks != 0) {
        void* next;
        do {
            tWorldMemBlock* item = m_pAllocBlocks;
            next = (void*)item->pNext;
            cWorldBlockAllocator_releaseBlock(*(void**)this, item);
            m_pAllocBlocks = (tWorldMemBlock*)next;
            m_uiNumBlocks = (int)m_uiNumBlocks - 1;
        } while (next != 0);
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cSectionBlockAllocator_allocate);
#ifdef SKIP_ASM
void* cWorldBlockAllocator_getFreeBlock(void* self);



static inline void NewBlock_7878(cSectionBlockAllocator* self)
{
    tWorldMemBlock* b = (tWorldMemBlock*)cWorldBlockAllocator_getFreeBlock(((void*)self->m_pBlockAllocator));
    b->iOffset = 0;
    b->iPad = 0;
    b->pNext = (self->m_pAllocBlocks);
    *(tWorldMemBlock**)&self->m_pAllocBlocks = b;
    *(tWorldMemBlock**)&self->m_pMRUBlock = b;
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
char* cSectionBlockAllocator::allocate(int size, unsigned int flags)
{
    if (m_uiNumBlocks == 0)
        NewBlock_7878(this);
    tWorldMemBlock* first = (m_pMRUBlock);
    int align = 4 << ((flags >> 24) & 0xF);
    int pos = first->iOffset;
    int rem = pos % align;
    int alignPad = 0;
    if (rem)
        alignPad = align - rem;
    if (pos + alignPad + size < first->iBaseSize)
    {
        first->iOffset = pos + (alignPad + size);
        char* allocPtr = first->pBaseMem + pos;
        allocPtr += alignPad;
        m_pMRUBlock->iPad += alignPad;
        return allocPtr;
    }
    if (m_uiNumBlocks >= 2)
    {
        *(tWorldMemBlock**)&m_pMRUBlock = (m_pAllocBlocks);
        if ((m_pMRUBlock))
        {
            do
            {
                tWorldMemBlock* block = (m_pMRUBlock);
                if (block != first)
                {
                    int bpos = block->iOffset;
                    rem = bpos % align;
                    alignPad = 0;
                    if (rem)
                        alignPad = align - rem;
                    if (bpos + alignPad + size < block->iBaseSize)
                    {
                        block->iOffset = bpos + (alignPad + size);
                        char* allocPtr = block->pBaseMem + bpos;
                        allocPtr += alignPad;
                        m_pMRUBlock->iPad += alignPad;
                        return allocPtr;
                    }
                }
                *(tWorldMemBlock**)&m_pMRUBlock = m_pMRUBlock->pNext;
            } while ((m_pMRUBlock));
        }
    }
    NewBlock_7878(this);
    m_pMRUBlock->iOffset += size;
    return m_pMRUBlock->pBaseMem;
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
extern "C" void cWorldBlockAllocator_init(cWorldBlockAllocator* self, int count, int blockSize);

/**
 * Constructs the world memory manager: three block allocators sized from D_0044C1E0 and 64 empty section
 * allocators.
 *
 * @return @p self
 */
extern "C" void* cWorldMemoryMan_cWorldMemoryMan(cWorldMemoryMan* self)
{
    ((cWorldBlockAllocator*)self)->init(D_0044C1E0[0].count, D_0044C1E0[0].blockSize);
    self->m_BlockAllocator[1].init(D_0044C1E0[1].count, D_0044C1E0[1].blockSize);
    self->m_BlockAllocator[2].init(D_0044C1E0[2].count, D_0044C1E0[2].blockSize);
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

/**
 * Gives a world section its own "Section Allocator" drawing from one of the three block allocators.
 *
 * @param index  the section ID (On Tour uiSectionID)
 * @param section  despite the name, the block memory type that picks the allocator (On Tour
 *                 eWorldBlockMemType
 * blocktype)
 */
// PORT: the section pointer is passed as int (the callee's mangled name is __FPvii).
void cWorldMemoryMan::activateSectionMem(int index, int section)
{
    void** mem = (void**)&m_pSectionAllocator;
    void** slot = &mem[index];
    *slot = cSectionBlockAllocator_cSectionBlockAllocator(cMemMan_alloc(20, str_Section_Allocator, 0x20000000, 0),
                          (int)&m_BlockAllocator[section], index);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldMemoryMan_deactivateSectionMem);
#ifdef SKIP_ASM
extern "C" void cSectionBlockAllocator__cSectionBlockAllocator(cSectionBlockAllocator* self, int flags);

/**
 * Destroys the arena of section @p i (cSectionBlockAllocator__cSectionBlockAllocator with 3) and clears its m_pSectionAllocator slot.
 *
 * @param i  the section id
 */
void cWorldMemoryMan::deactivateSectionMem(int i)
{
    cSectionBlockAllocator** base = &m_pSectionAllocator[0];
    cSectionBlockAllocator** slot = base + i;
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
extern "C" void cHullPage__cHullPage(cHullPage* self, int flags)
{
    self->m_uiNumPageEntry = 0;
    if (self->m_pPageEntries != 0) {
        cMemMan_free(self->m_pPageEntries);
    }
    if (flags & 1) {
        operator_delete(((int*)self));
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCacheTable_cWorldCacheTable);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void* memset(void* dst, int c, int n);
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
    memset(self->entries, 0, sizeof(self->entries));
    int total = 0;
    for (int i = 0; i < 28; i++) {
        int n = self->sizes[i];
        self->entries[i].count = n;
        total += n;
    }
    self->pool = new (str_WorldCacheTableMem, 0, 0) int[total];
    memset(self->pool, 0, total * 4);
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
extern "C" void cWorldCacheTable__cWorldCacheTable(cWorldCacheTable* self, int flags)
{
    if (*(void**)self != 0) {
        cMemMan_free(*(void**)self);
    }
    if (flags & 1) {
        operator_delete(((int*)self));
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
    cAppMan* app = cAppMan_TheAppMan;
    self->mp_DataHeader = 0;
    self->m_uiNumHullPage = (unsigned int)0;
    self->mp_ChunkResolve = 0;
    self->m_pWorldMemMan = 0;
    self->m_pSharedCacheTable = 0;
    self->m_pSectionCacheTable = 0;
    self->m_iTouchStaleAge = (int)((float)app->TimerHz * 0.10000000149011612f);
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
extern "C" void cWorldCache__cWorldCache(cWorldCache* self, int flags)
{
    self->purge();
    cStreamMan__cStreamMan(&self->m_StreamMan, 2);
    cWorldView__cWorldView((cWorldView*)(&self->m_WorldView), 2);
    if (flags & 1) {
        operator_delete(((int*)self));
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
extern "C" void cWorldCacheTable__cWorldCacheTable(cWorldCacheTable* self, int flags);

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
 */
void cWorldCache::purge()
{
    cWorldCache* self = this;
    self->resetPageMap();
    cWorldCache_purge_sObj* chunkResolve = ((cWorldCache_purge_sObj*)self->mp_ChunkResolve);
    if (chunkResolve != 0) {
        chunkResolve->vt[1].fn(vthis(chunkResolve, chunkResolve->vt[1]), 3);
    }
    if (((int*)self->m_pSharedCacheTable) != 0) {
        cWorldCacheTable__cWorldCacheTable((cWorldCacheTable*)((int*)self->m_pSharedCacheTable), 3);
    }
    if (((void*)self->m_pSectionCacheTable) != 0) {
        unsigned int i;
        for (i = 0; i < (self->mp_DataHeader)->m_uiNumSections; i++) {
            if (cWorldView_isSectionUnLoaded(((char*)&self->m_WorldView), i) == 0) {
                self->deactivateSectionMem(i);
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
void cWorldCache::reset()
{
    purge();
    mp_DataHeader = 0;
    m_uiNumHullPage = (unsigned int)0;
    mp_ChunkResolve = 0;
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
extern "C" void cWorldCache_initPageMap(cWorldCache* self);
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
    hdr->m_pSection = (struct tBxSection*)&hdr[1]; // the section records follow the 0x50-byte header
    self->m_pSharedCacheTable = (cWorldCacheTable*)cWorldCacheTable_cWorldCacheTable((cWorldCacheTable*)cMemMan_alloc(232, str_SharedTable, 0, 0), &self->mp_DataHeader->m_iTable[0]);
    self->m_pSectionCacheTable = (cWorldCacheTable**)((void**)operator_new_tag((int)self->mp_DataHeader->m_uiNumSections * 4, str_SectionTables, 0, 0));
    for (unsigned int i = 0; i < self->mp_DataHeader->m_uiNumSections; i++)
        self->m_pSectionCacheTable[i] = 0;
    cChunkResolve* resolver = (cChunkResolve*)cChunkResolve_construct();
    self->mp_ChunkResolve = resolver;
    sVE_7FC0* vt = (sVE_7FC0*)resolver->mVtbl;
    vt[2].fn(vthis(resolver, vt[2]), pNash);
    if (cStreamMan_init((char*)&self->m_StreamMan, name, self) == 0)
        return 0;
    int viewOk = cWorldView_init_p((char*)&self->m_WorldView, self->mp_DataHeader, self);
    if (viewOk == 0)
        return 0;
    self->initPageMap();
    return viewOk;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldCache_activateSectionMem);
#ifdef SKIP_ASM
extern char str_SectionTable[];
extern "C" void* cWorldCacheTable_cWorldCacheTable(cWorldCacheTable* self, void* data);

// cWorldCacheTable built by a new-expression: its constructor and cMemMan_alloc as the operator new.
struct cWorldCacheTableNew : cWorldCacheTable {
    cWorldCacheTableNew(short* table) __asm__("cWorldCacheTable_cWorldCacheTable");
    void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
};

/**
 * Activates a world section's memory: its section allocator and a "SectionTable" cache table from its header
 * entry.
 *
 * @param i  the section ID (On Tour uiSectionID)
 * @param section  despite the name, the block memory type (On Tour eWorldBlockMemType blocktype)
 */
void cWorldCache::activateSectionMem(int i, int section)
{
    m_pWorldMemMan->activateSectionMem(i, section);
    m_pSectionCacheTable[i] = new (str_SectionTable, 0, 0)
        cWorldCacheTableNew(&mp_DataHeader->m_pSection[i].m_iTable[0]);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("world/worldcache", cWorldCache_deactivateSectionMem);
#ifdef SKIP_ASM
extern "C" void cWorldMemoryMan_deactivateSectionMem(cWorldMemoryMan* self, int i);
extern "C" void cWorldCacheTable__cWorldCacheTable(cWorldCacheTable* self, int flags);

/**
 * Releases a world section's allocator and deletes its cache table.
 *
 * @param i  the section ID (On Tour uiSectionID)
 */
void cWorldCache::deactivateSectionMem(int i)
{
    m_pWorldMemMan->deactivateSectionMem(i);
    int* sectionTable = ((int*)m_pSectionCacheTable[i]);
    if (sectionTable != 0) {
        cWorldCacheTable__cWorldCacheTable((cWorldCacheTable*)sectionTable, 3);
    }
    m_pSectionCacheTable[i] = 0;
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
void cWorldCache::update()
{
    m_WorldView.update();
    updatePages();
    cStreamMan_update((char*)&m_StreamMan);
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_initPageMap);
#ifdef SKIP_ASM
int cWorldView_getNumHull(void*);

/**
 * Clears the world cache's page map, sizing it from the world view.
 *
 * Stores the view's page count (cWorldView_getNumHull, a word of the view's header) in m_uiNumHullPage and zeroes
 * the six words of each m_HullPages entry.
 */
void cWorldCache::initPageMap()
{
    unsigned int i;
    m_uiNumHullPage = cWorldView_getNumHull(&m_WorldView);
    for (i = 0; i < m_uiNumHullPage; i++) {
        m_HullPages[i].m_State = 0;
        m_HullPages[i].m_bLock = 0;
        m_HullPages[i].mp_Page = 0;
        m_HullPages[i].m_iTouchTick = 0;
        m_HullPages[i].m_iRenderTick = 0;
        m_HullPages[i].m_fPriority = 0;
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
 */
void cWorldCache::resetPageMap()
{
    sWc_8330* self = (sWc_8330*)this;
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        switch (self->ents[i].state) {
        case kHullPage_Empty:
        case kHullPage_Queued:
            break;
        case kHullPage_Loading:
            cStreamMan_cancelStreamRequest((char*)self + 0x300, i);
            self->ents[i].state = kHullPage_InActive;
            ((cWorldCache*)self)->purgePage(i);
            break;
        case kHullPage_Active:
        case kHullPage_InActive:
            self->ents[i].state = kHullPage_InActive;
            ((cWorldCache*)self)->purgePage(i);
            break;
        }
        self->ents[i].state = kHullPage_Empty;
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

struct sWCSlots {
    char pad[0x3F0];
    tWorldCacheHullPage slot[256];   // 0x3F0
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
    self->slot[i].m_iTouchTick = cAppMan_TheAppMan->Tick;
    self->slot[i].m_fPriority = priority;
    self->slot[i].m_bLock = 0;
    switch (self->slot[i].m_State) {
    case kHullPage_Empty:
        self->slot[i].m_State = kHullPage_Queued;
        break;
    case kHullPage_Queued:
    case kHullPage_Loading:
    case kHullPage_Active:
        break;
    case kHullPage_InActive:
        self->slot[i].m_State = kHullPage_Active;
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
    self->slot[i].m_bLock = 0;
    self->slot[i].m_iTouchTick = cAppMan_TheAppMan->Tick - self->timeBase;
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
void cWorldCache::setPageLock(int hullPage, int lock)
{
    m_HullPages[hullPage].m_bLock = lock;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_isPageLocked);
#ifdef SKIP_ASM
/**
 * Returns hull page @p hullPage's m_bLock.
 *
 * @param hullPage  the hull page
 * @return the lock flag
 */
int cWorldCache::isPageLocked(int hullPage)
{
    return m_HullPages[hullPage].m_bLock;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_purgePage);
#ifdef SKIP_ASM
extern "C" void cWorldCache_removeBxStreamData(cWorldCache* self, int cell, int kind, tResourceID h);
extern "C" void cHullPage__cHullPage(cHullPage* self, int flags);

/**
 * Unloads a hull page: releases every stream resource it references (cWorldCache_removeBxStreamData), then deletes its list.
 *
 * @param cell  the page index (On Tour uiHullPage)
 */
// Unloads a cache cell: reports each referenced object (kind, handle) to cWorldCache_removeBxStreamData, then frees the list.
void cWorldCache::purgePage(int cell)
{
    cHullPage* list = m_HullPages[cell].mp_Page;
    tResourceID resHandle;
    *(unsigned int*)&resHandle = 0xFFFFFFFF;  // starts as the invalid handle
    for (int i = ((int)list->m_uiCurrentPageEntry) - 1; i >= 0; i--) {
        resHandle.m_bfRID.m_uiSectionID = (list->m_pPageEntries)[i].m_uiSectionID;
        resHandle.m_bfRID.m_uiUID = (list->m_pPageEntries)[i].m_uiGUID;
        cWorldCache_removeBxStreamData(this, cell, (list->m_pPageEntries)[i].m_uiType, resHandle);
    }
    if (m_HullPages[cell].mp_Page != 0)
        cHullPage__cHullPage(m_HullPages[cell].mp_Page, 3);
    m_HullPages[cell].mp_Page = 0;
    m_HullPages[cell].m_State = 0;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_isPageLoaded);
#ifdef SKIP_ASM
/**
 * Tells whether a hull page is loaded (kHullPage_Active).
 *
 * @param uiHullPage  the page index (On Tour uiHullPage)
 * @return true when loaded, otherwise false
 */
bool cWorldCache::isPageLoaded(int uiHullPage)
{
    return m_HullPages[uiHullPage].m_State == kHullPage_Active;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_isPageEmpty);
#ifdef SKIP_ASM
/**
 * Tells whether a hull page is empty (kHullPage_Empty).
 *
 * @param uiHullPage  the page index (On Tour uiHullPage)
 * @return true when empty, otherwise false
 */
bool cWorldCache::isPageEmpty(int uiHullPage)
{
    return m_HullPages[uiHullPage].m_State == kHullPage_Empty;
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
void cWorldCache::signalPageLoadComplete(int uiHullPage)
{
    m_HullPages[uiHullPage].m_State = 3;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_updatePages);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cHullPage_cHullPage(cHullPage* self, unsigned short* data);
extern "C" void* cWorldView_getHull(void* self, int i);
extern "C" void cStreamMan_addStreamRequest(cStreamMan* self, int i, int a, int b);
extern "C" void cWorldCache_purgePage(cWorldCache* self, int i);
extern char str_cHullPage[];

// PORT: g++ 2.95 new-expression; the ctor and the tagged allocator are bound to their symbols by asm label.
struct cHullPageNew : cHullPage {
    cHullPageNew(const tBxBSPHull* hull) __asm__("cHullPage_cHullPage");
    void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");
};

static inline int Frame_8668()
{
    cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
    return ps2GraphicsMan->getCurrentFrameCount();
}

/**
 * Ages the hull pages and starts streaming the highest-priority requested page when the stream manager is
 * free.
 *
 * Page states: 0 empty, 1 requested, 2 streaming, 3 loaded, 4 being purged. Unlocked pages untouched for the
 * stale
 * age drop their request or start purging; a purge completes 5 frames later.
 */
void cWorldCache::updatePages()
{
    int now = cAppMan_TheAppMan->Tick;
    int best = -1;
    float bestPri = -1.0f;
    for (unsigned int i = 0; i < m_uiNumHullPage; i++)
    {
        int st = m_HullPages[i].m_State;
        if (st == kHullPage_Empty)
            continue;
        int expired = 0;
        if (m_HullPages[i].m_bLock == 0 && now - m_HullPages[i].m_iTouchTick >= m_iTouchStaleAge)
            expired = 1;
        switch (st)
        {
        case kHullPage_Loading:
            break;
        case kHullPage_Queued:
            if (expired)
            {
                m_HullPages[i].m_State = kHullPage_Empty;
                break;
            }
            if (m_HullPages[i].m_fPriority > bestPri)
            {
                bestPri = m_HullPages[i].m_fPriority;
                best = i;
            }
            break;
        case kHullPage_Active:
            if (!expired)
                break;
            m_HullPages[i].m_State = kHullPage_InActive;
            m_HullPages[i].m_iRenderTick = Frame_8668();
            break;
        case kHullPage_InActive:
            if (Frame_8668() - m_HullPages[i].m_iRenderTick < 5)
                break;
            purgePage(i);
            m_HullPages[i].m_State = kHullPage_Empty;
            break;
        }
    }
    if (best < 0)
        return;
    cStreamMan* mgr = &m_StreamMan;
    if (m_StreamMan.m_bActive != 0)
        return;
    tBxBSPHull* hull = (tBxBSPHull*)cWorldView_getHull(&m_WorldView, best);
    m_HullPages[best].mp_Page = new (str_cHullPage, 0x20000000, 0) cHullPageNew(hull);
    cStreamMan_addStreamRequest(mgr, best, hull->m_uiNumChunk, hull->m_iStreamOffset);
    m_HullPages[best].m_State = kHullPage_Loading;
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
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
void cWorldCache::flushInActivePages()
{
    int now = cAppMan_TheAppMan->Tick;
    for (unsigned int i = 0; i < m_uiNumHullPage; i++)
    {
        int st = m_HullPages[i].m_State;
        if (st == kHullPage_Empty)
            continue;
        int expired = 0;
        if (m_HullPages[i].m_bLock == 0 && m_HullPages[i].m_iTouchTick < now)
            expired = 1;
        switch (st)
        {
        case kHullPage_Loading:
            break;
        case kHullPage_Queued:
            if (expired)
                m_HullPages[i].m_State = kHullPage_Empty;
            break;
        case kHullPage_Active:
            if (!expired)
                break;
            m_HullPages[i].m_State = kHullPage_InActive;
        case kHullPage_InActive:
            purgePage(i);
            m_HullPages[i].m_State = kHullPage_Empty;
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
 */
void cWorldCache::flushOnePage()
{
    sWc_89C8* self = (sWc_89C8*)this;
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
        ((cWorldCache*)self)->purgePage(best);
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

static inline void* bxAlloc_8AA8(cWorldMemoryMan* mem, int bank, int size, unsigned int flags)
{
    if (bank == cWorldCache::kSharedBank)
        return operator_new_bx(size, D_00494D40, flags, 0);
    return mem->m_pSectionAllocator[bank]->allocate(size, flags);
}

// Takes a reference on a data tag; nonzero when the data was already referenced (resident or loading).
static inline int addRef_8AA8(tWorldCacheDataTag* tag)
{
    if (tag->m_bfuiRefCnt != 0) {
        tag->m_bfuiRefCnt++;
        return 1;
    }
    tag->m_bfuiRefCnt++;
    return 0;
}

// Drops the reference addRef_8AA8 took; at zero the whole tag is cleared (one word store of the count).
static inline void release_8AA8(tWorldCacheDataTag* tag)
{
    if (--tag->m_bfuiRefCnt == 0)
        *(unsigned int*)tag = tag->m_bfuiRefCnt;
}

/**
 * Reserves storage for one stream resource of a hull page, or records it on the page when it is already
 * resident.
 *
 * The key holds the bank in its low byte (0xFF for the shared bank) and the resource ID above it. Memory
 * comes
 * from a pool picked by the data type; AI paths (kBxStreamData_AIPath) use a "BxStream AIPATH" allocation.
 *
 * @param slot  the hull page (On Tour uiHullPage)
 * @param type  the data type (On Tour eBxStreamDataType dataType)
 * @param key  the resource ID (On Tour tResourceID RID)
 * @param size  the data size (On Tour uiDataSize)
 * @return the storage address; 0 when it is already resident or @p size is 0; kStreamNoRoom when allocation
 *         failed
 */
// Adds one stream data item (type, bank|id<<8) to a load list, allocating its storage on first use.
unsigned int cWorldCache::addBxStreamDataTest(int slot, int type, unsigned int key, int size)
{
    int bank = key & 0xFF;
    unsigned int id = key >> 8;
    cWorldCacheTable* table;
    if (bank == kSharedBank)
        table = m_pSharedCacheTable;
    else
        table = m_pSectionCacheTable[bank];
    if (addRef_8AA8(&table->m_pCacheTable[type].m_pDataTag[id])) {
        cHullPage* page = m_HullPages[slot].mp_Page;
        page->m_pPageEntries[page->m_uiCurrentPageEntry].m_uiType = type;
        page->m_pPageEntries[page->m_uiCurrentPageEntry].m_uiSectionID = key;
        page->m_pPageEntries[page->m_uiCurrentPageEntry].m_uiGUID = key >> 8;
        page->m_uiCurrentPageEntry++;
        return 0;
    }
    if (size == 0)
        return 0;
    void* data;
    switch (type) {
    case kBxStreamData_Image:
    case kBxStreamData_LightPage:
        data = bxAlloc_8AA8(m_pWorldMemMan, bank, size, 0x45000000);
        break;
    case kBxStreamData_Patch:
    case kBxStreamData_Model:
    case kBxStreamData_Instance:
    case kBxStreamData_VisCurtain:
    case kBxStreamData_CollideModel:
        data = bxAlloc_8AA8(m_pWorldMemMan, bank, size, 0x22000000);
        break;
    case kBxStreamData_AIPath:
        data = operator_new_bx(size, str_BxStream_AIPATH, 0, 0);
        break;
    default:
        data = bxAlloc_8AA8(m_pWorldMemMan, bank, size, 0);
        break;
    }
    if (data != 0)
        return (unsigned int)data;
    release_8AA8(&table->m_pCacheTable[type].m_pDataTag[id]);
    return kStreamNoRoom;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_addBxStreamDataSubmit);
#ifdef SKIP_ASM
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
void cWorldCache::addBxStreamDataSubmit(int group, int id, unsigned int key, int chunk)
{
    cWorldCacheTable* table;
    cHullPage* page;
    unsigned int hi = key >> 8;
    if ((key & 0xFF) == kSharedBank) {
        table = m_pSharedCacheTable;
    } else {
        table = m_pSectionCacheTable[key & 0xFF];
    }
    page = m_HullPages[group].mp_Page;
    page->m_pPageEntries[page->m_uiCurrentPageEntry].m_uiType = id;
    page->m_pPageEntries[page->m_uiCurrentPageEntry].m_uiSectionID = key;
    page->m_pPageEntries[page->m_uiCurrentPageEntry].m_uiGUID = key >> 8;
    page->m_uiCurrentPageEntry++;
    if (chunk == (int)&cStreamMan_NULLCHUNK) {
        cChunkResolve* resolver = mp_ChunkResolve;
        sVEntry<int(void*, int, unsigned int, int)>* vt = (sVEntry<int(void*, int, unsigned int, int)>*)resolver->mVtbl;
        vt[3].fn(vthis(resolver, vt[3]), id, key, 0);
    } else {
        cChunkResolve* resolver = mp_ChunkResolve;
        sVEntry<int(void*, int, unsigned int, int)>* vt = (sVEntry<int(void*, int, unsigned int, int)>*)resolver->mVtbl;
        chunk = vt[3].fn(vthis(resolver, vt[3]), id, key, chunk);
    }
    table->m_pCacheTable[id].m_pDataTag[hi].m_bfuiMemLoc = chunk >> 2;
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldCache_removeBxStreamData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).



// Drops one reference; on the last one returns the stored data pointer and clears it.
static inline char* release_8E20(tWorldCacheDataTag* tag)
{
    if (--tag->m_bfuiRefCnt == 0) {
        unsigned int v = *(unsigned int*)tag;
        char* d = (char*)((v >> 8) << 2);
        *(unsigned int*)tag = v & 0xFF;
        return d;
    }
    return 0;
}

extern "C" void rel_8E20(cWorldCache* self, int cell, int type, unsigned int key) __asm__("cWorldCache_removeBxStreamData");
// mp_ChunkResolve's vtable slot 4 (unresolve): (type, tResourceID, data) -> the data to free.
typedef sVEntry<void*(void*, int, unsigned int, char*)> tUnresolveSlot;
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
    cWorldCacheTable* table;
    if (bank == cWorldCache::kSharedBank)
        table = self->m_pSharedCacheTable;
    else
        table = self->m_pSectionCacheTable[bank];
    char* chunk = release_8E20(&table->m_pCacheTable[type].m_pDataTag[id]);
    if (chunk != 0) {
        if (chunk == (char*)&cStreamMan_NULLCHUNK) {
            cChunkResolve* callbackObj = (self->mp_ChunkResolve);
            tUnresolveSlot* vt = (tUnresolveSlot*)callbackObj->mVtbl;
            chunk = (char*)vt[4].fn(vthis(callbackObj, vt[4]), type, key, 0);
        } else {
            cChunkResolve* callbackObj = (self->mp_ChunkResolve);
            tUnresolveSlot* vt = (tUnresolveSlot*)callbackObj->mVtbl;
            chunk = (char*)vt[4].fn(vthis(callbackObj, vt[4]), type, key, chunk);
        }
        if (chunk != 0 && bank == cWorldCache::kSharedBank)
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
 * The count is the section's patch count (m_iTable[1] of its tBxSection) and the records come from the section's
 * cache table (m_pCacheTable[1], kBxStreamData_Patch). There is no check for an empty tag (a missing patch would be
 * written at address 0xA).
 *
 * @param idx  the section id
 */
// Sets flag 0x40 on every object listed for cell idx.
void cWorldCache::activateSectionPatches(int idx)
{
    int n = mp_DataHeader->m_pSection[idx].m_iTable[1];
    cWorldCacheTable* list = m_pSectionCacheTable[idx];
    for (unsigned i = 0; i < n; i++) {
        tPatchRecord* object = (tPatchRecord*)conv_8F10(list->m_pCacheTable[1].m_pDataTag[i].m_bfuiMemLoc);
        object->m_sSurfaceFlags |= 0x40;
    }
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView_cWorldView);
#ifdef SKIP_ASM
inline void* operator new[](unsigned int, void* p) { return p; }
// m_StreamFocus[2] has an (empty) constructor in EA's source (its t4Vectors'); the vec_new loop is all dead but
// its counter setup.
struct tStreamFocusInfoCtor : tStreamFocusInfo {
    tStreamFocusInfoCtor() {}
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
    new (&((cWorldView*)self)->m_StreamFocus[0]) tStreamFocusInfoCtor[2];
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
    self->purge();
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
    self->sects = (sWCSect*)&hdr[1]; // the section records follow the 0x50-byte header
    data = (char*)&self->sects[hdr->numSects];
    data = (char*)(((unsigned int)data + 0xF) & 0xFFFFFFF0);
    sWCNode* nodes = (sWCNode*)data;
    self->nodes = nodes;
    self->leaves = (sWCLeaf*)&nodes[hdr->numNodes];

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
void cWorldView::purge()
{
}
#endif

//100%
INCLUDE_ASM("world/worldcache", cWorldView_reset);
#ifdef SKIP_ASM
extern t4Vector t4vect_Origin;

extern "C" void cWorldView_setFocus(void* self, unsigned int i, t4Vector* v, float f);

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
    t4Vector v220;
    t4Vector v230;
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
extern "C" int strncmp(const char* a, const char* b, int n);
struct cWorldView_recursiveFlushPages_sNode;
extern "C" void cWorldView_recursiveFlushPages(void** self, cWorldView_recursiveFlushPages_sNode* node);
extern "C" void cWorldView_recursiveIsEmptyPages(void** self, tBxBSPNode* node, int* ok);
extern "C" void flushWorldLightCache();
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


/**
 * Advances each world section's load state and refreshes the two stream focuses.
 *
 * States (eSectionState): kSection_LoadQueue (activates its memory; sky sections ASKY-ESKY use block type 0, others
 * 2), kSection_Loading, kSection_UnLoadQueue, kSection_UnLoading, kSection_Loaded and kSection_Active (counted). Newly
 * loaded sections are announced to the renderer (slot 102).
 *
 */
void cWorldView::update()
{
    sWCLoader_9258* self = (sWCLoader_9258*)this;
    if (self->info == 0)
        return;
    self->f2E0 = 0;
    self->f2E4 = 0;
    for (unsigned int i = 0; i < self->info->count; i++) {
        switch (self->entries[i].state) {
        case kSection_Empty:
            break;
        case kSection_LoadQueue:
            if (strncmp(self->sections[i].name, str_ASKY, 4) == 0
                || strncmp(self->sections[i].name, str_BSKY, 4) == 0
                || strncmp(self->sections[i].name, str_CSKY, 4) == 0
                || strncmp(self->sections[i].name, str_DSKY, 4) == 0
                || strncmp(self->sections[i].name, str_ESKY, 4) == 0)
                ((cWorldCache*)self->cache)->activateSectionMem(i, 0);
            else
                ((cWorldCache*)self->cache)->activateSectionMem(i, 2);
            cWorldCache_touchPage((sWCSlots*)self->cache, self->sections[i].id, self->entries[i].pri);
            ((cWorldCache*)self->cache)->setPageLock(self->sections[i].id, 1);
            self->entries[i].state = kSection_Loading;
            break;
        case kSection_Loading:
            if (((cWorldCache*)self->cache)->isPageLoaded(self->sections[i].id)) {
                self->entries[i].state = kSection_Loaded;
                cPS2GraphicsMan* ps2GraphicsMan = cGraphicsMan_TheGraphicsMan;
                ps2GraphicsMan->SectionLoaded(i);
            }
            break;
        case kSection_UnLoading:
            if (((cWorldCache*)self->cache)->isPageEmpty(self->sections[i].id)) {
                int ok = 1;
                cWorldView_recursiveIsEmptyPages((void**)self, (tBxBSPNode*)self->sections[i].tree, &ok);
                if (ok) {
                    self->entries[i].state = kSection_Empty;
                    ((cWorldCache*)self->cache)->deactivateSectionMem(i);
                    flushWorldLightCache();
                }
            }
            break;
        case kSection_Loaded:
            self->f2E0++;
            break;
        case kSection_Active:
            self->f2E0++;
            self->f2E4++;
            break;
        case kSection_UnLoadQueue:
            cWorldCache_flushPage_i((sWCSlots*)self->cache, self->sections[i].id);
            cWorldView_recursiveFlushPages((void**)self, (cWorldView_recursiveFlushPages_sNode*)self->sections[i].tree);
            self->entries[i].state = kSection_UnLoading;
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
extern "C" bool cWorldCache_isPageEmpty(cWorldCache* self, int a1);

/**
 * Clears @p ok when any hull page under a BSP node is not empty (cWorldCache_isPageEmpty), stopping at the first.
 *
 * @param node  the tBxBSPNode
 * @param ok  set on entry; cleared when a page is still loaded
 */
extern "C" void cWorldView_recursiveIsEmptyPages(void** self, tBxBSPNode* node, int* ok)
{
    if (*ok != 0) {
        tBxBSPHull* pageRef = node->m_pHull;
        if (((void*)pageRef) != 0 && ((cWorldCache*)(*self))->isPageEmpty(pageRef->m_uiHullIndex) == 0) {
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

// PORT: PS2-only VU0 inline asm (in-place vector subtract).
static inline void vu0SubFromWC(t4Vector& a, const t4Vector& b)
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
static inline void vu0AddToWC(t4Vector& a, const t4Vector& b)
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

// A tBoundBox built from its two corners (EA's inline constructor).
struct tBoundBoxInit : tBoundBox {
    tBoundBoxInit(const t4Vector& a, const t4Vector& b)
    {
        MinPoint = a;
        MaxPoint = b;
    }
};

// cWorldView::setFocus
/**
 * Points one of the world view's streaming foci at a position.
 *
 * For focus @p id: stores @p pos as the load position and max(@p fRadius, 0) as the load radius, sets the focus box to
 * pos +/- fRadius, sets m_bFocusUpdate and clears its safe radii and m_iFocusTick.
 *
 * @param id  which stream focus to set (On Tour eStreamFocusID)
 * @param pos  the position to stream around
 * @param fRadius  how far around it to load
 */
void cWorldView::setFocus(int id, const t4Vector& pos, float fRadius)
{
    m_StreamFocus[id].m_v4LoadPos = pos;
    m_StreamFocus[id].m_fLoadRadius = fRadius >? 0.0f;
    m_StreamFocus[id].m_FocusBox = tBoundBoxInit(pos, pos);
    {
        t4Vector ext;
        ext.x = fRadius;
        ext.y = fRadius;
        ext.z = fRadius;
        ext.w = 0.0f;
        vu0SubFromWC(m_StreamFocus[id].m_FocusBox.MinPoint, ext);
        vu0AddToWC(m_StreamFocus[id].m_FocusBox.MaxPoint, ext);
    }
    m_StreamFocus[id].m_bFocusUpdate = 1;
    m_StreamFocus[id].m_fSafeRadius = 0.0f;
    m_StreamFocus[id].m_fSafeTestRadius = 0.0f;
    m_StreamFocus[id].m_iFocusTick = 0;
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
int cWorldView::getFocus(int i, t4Vector* outPos, float* outT)
{
    if (m_StreamFocus[i].m_bActive == 0) {
        return 0;
    }
    cWorldView_refreshStreamFocus(this, i);
    int valid = 1;
    *outPos = m_StreamFocus[i].m_v4LoadPos;
    float safeRadius = m_StreamFocus[i].m_fSafeRadius - 0.5f;
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
int cWorldView::getTestFocus(int i, t4Vector* outPos, float* outT)
{
    if (m_StreamFocus[i].m_bActive == 0) {
        return 0;
    }
    cWorldView_refreshStreamFocus(this, i);
    int valid = 1;
    *outPos = m_StreamFocus[i].m_v4LoadPos;
    float safeRadius = m_StreamFocus[i].m_fSafeTestRadius - 0.5f;
    *outT = safeRadius;
    if (!(safeRadius > 0.0f)) {
        valid = 0;
    }
    return valid;
}
#endif

