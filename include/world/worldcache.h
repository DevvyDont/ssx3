#ifndef WORLD_WORLDCACHE_H
#define WORLD_WORLDCACHE_H

// The world cache: everything that holds the streamed open-mountain data in memory. EA declares all of these in
// ../source/world/worldcache.h (the preview's inline asserts "m_pSectionAllocator[uiSectionID]" and
// "uiSectionID<=(tUInt32)kBxMaxSections" are filed under that header); the code is d:/ssx/code/source/world/
// worldcache.cpp. Retail unit world/worldcache (0x003A7598-0x003A8F80; cWorldView's methods follow it in the same unit).
//
//   tWorldMemBlock, cWorldBlockAllocator  a pool of fixed-size memory blocks;
//   cSectionBlockAllocator                one world section's arena, carved from one pool's blocks;
//   cWorldMemoryMan                       three pools (16k/32k/128k types) + one arena per active section;
//   cHullPage                             the resources that one streamed hull page added (so it can release them);
//   tWorldCacheDataTag/Table, cWorldCacheTable  per-type resource tables (refcount + pointer per resource id);
//   cWorldCache                           the owner: .sdb header, tables, memory, cWorldView, cStreamMan, hull pages.
//
// cWorldCache: one instance, cWorld::m_pWorldCache (cWorld_cWorld: cMemMan_alloc(0x1C00, "cWorldCache")), reached as
// World->m_pWorldCache (the global cWorld::m_pTheWorld is cWorld_m_pTheWorld). No vtable, no base class. Size 0x1C00 [R]: the
// last field is 0x1BF0; 0x1BF4-0x1BFF is tail padding to the 16-byte alignment that cWorldView's t4Vectors give it
// (no retail code touches it). On Tour's cWorldCache is 0x1C10 and adds a model-cache list at 0x1BF0+ that retail lacks.
// Its static m_SectionLoadPriority[3] is D_0044C208 = {0.90, 0.91, 0.92} (cSectionMan_update indexes it).
// Hull page states (On Tour eHullPageState): 0 Empty, 1 Queued, 2 Loading, 3 Active, 4 InActive.
//
// A FLAT layout, like main/game.h: every field at its retail offset. Member names are EA's, from SSX On Tour's PSP
// DWARF, placed by:
//   [P] the July 2003 preview's assert strings in worldcache.cpp/.h (quoted per field);
//   [R] retail's constructors and allocations (tags read from the retail ELF: "WORLD BLOCK ARRAY", "WORLD MEMBLOCK",
//       "Section Allocator", "WORLD MEMMAN", "cHullPage", "cHullPage Entries", "WorldCacheTableMem", "SharedTable",
//       "SectionTables", "SectionTable");
//   [U] how retail code uses the field;
//   [O] On Tour's member order, where the fields on both sides are placed by [P], [R] or [U].
// PS2 bools are 4-byte ints here. Unnamed fields are unk_XXX; rename only with evidence.

#include "layout.h"
#include "world/worldview.h"
#include "world/streamman.h"

struct cChunkResolve;

// One pool block (On Tour tWorldMemBlock). 0x18 bytes: cWorldBlockAllocator_init's array stride.
struct tWorldMemBlock {
    char* pBaseMem;                 // 0x00 [O]; [R] new(blockSize, "WORLD MEMBLOCK")
    int iBaseSize;                  // 0x04 [P] assert m_pMRUBlock->iBaseSize; [R] = blockSize
    int iOffset;                    // 0x08 [O]; [R] = 0; [U] cSectionBlockAllocator::allocate's fill level
    int iPad;                       // 0x0C [O]; [R] = 0; [U] allocate adds the alignment padding it spent
    int bActive;                    // 0x10 [P] assert m_pFreeBlocks->bActive (a bool); [U] getFreeBlock = 1,
                                    //      releaseBlock = 0
    tWorldMemBlock* pNext;          // 0x14 [O]; [R] init chains the array; [U] free list / section block list link
};

CHECK_OFFSET(tWorldMemBlock, pBaseMem, 0x00);
CHECK_OFFSET(tWorldMemBlock, iBaseSize, 0x04);
CHECK_OFFSET(tWorldMemBlock, iOffset, 0x08);
CHECK_OFFSET(tWorldMemBlock, iPad, 0x0C);
CHECK_OFFSET(tWorldMemBlock, bActive, 0x10);
CHECK_OFFSET(tWorldMemBlock, pNext, 0x14);
CHECK_SIZE(tWorldMemBlock, 0x18);

// A pool of equal blocks (On Tour cWorldBlockAllocator, 0x14; retail cWorldMemoryMan holds three, 0x14 apart).
// Methods: cWorldBlockAllocator_init, func_003A76C0 (dtor), func_003A7768 (getFreeBlock), func_003A7790 (releaseBlock).
// The pool sizes are cWorldMemoryMan::m_BlockMemType[3] = D_0044C1E0: {type, uiNumBlock, uiBlockSize} =
// {0, 2, 0x4000}, {1, 0, 0x8000}, {2, 0x35, 0x20000}.
struct cWorldBlockAllocator {
    unsigned int m_uiBlockSize;     // 0x00 [O]; [R] init's third argument
    unsigned int m_uiTotalBlocks;   // 0x04 [O]; [R] init's second argument; [U] the dtor's loop bound
    unsigned int m_uiNumFreeBlocks; // 0x08 [P] assert m_uiNumFreeBlocks; [R] = count; [U] get/release -1/+1
    tWorldMemBlock* m_pBlockArray;  // 0x0C [O]; [R] new(count * 0x18, "WORLD BLOCK ARRAY")
    tWorldMemBlock* m_pFreeBlocks;  // 0x10 [P] assert m_pFreeBlocks; [R] = m_pBlockArray; [U] free-list head
};

CHECK_OFFSET(cWorldBlockAllocator, m_uiBlockSize, 0x00);
CHECK_OFFSET(cWorldBlockAllocator, m_uiTotalBlocks, 0x04);
CHECK_OFFSET(cWorldBlockAllocator, m_uiNumFreeBlocks, 0x08);
CHECK_OFFSET(cWorldBlockAllocator, m_pBlockArray, 0x0C);
CHECK_OFFSET(cWorldBlockAllocator, m_pFreeBlocks, 0x10);
CHECK_SIZE(cWorldBlockAllocator, 0x14);

// One section's arena (On Tour cSectionBlockAllocator; retail cMemMan_alloc(0x14, "Section Allocator")).
// Methods: func_003A77B0 (ctor), func_003A77D0 (dtor), func_003A7818 (releaseBlocks), func_003A7878 (allocate).
struct cSectionBlockAllocator {
    cWorldBlockAllocator* m_pBlockAllocator; // 0x00 [P] assert pAllocator (ctor argument); [R] ctor
    unsigned int m_uiSectionID;     // 0x04 [O]; [R] ctor
    unsigned int m_uiNumBlocks;     // 0x08 [P] assert !m_uiNumBlocks; [R] = 0; [U] allocate +1, releaseBlocks -1
    tWorldMemBlock* m_pAllocBlocks; // 0x0C [O]; [R] = 0; [U] list of the arena's blocks
    tWorldMemBlock* m_pMRUBlock;    // 0x10 [P] assert m_pMRUBlock->iBaseSize; [R] = 0; [U] allocate's current block
};

CHECK_OFFSET(cSectionBlockAllocator, m_pBlockAllocator, 0x00);
CHECK_OFFSET(cSectionBlockAllocator, m_uiSectionID, 0x04);
CHECK_OFFSET(cSectionBlockAllocator, m_uiNumBlocks, 0x08);
CHECK_OFFSET(cSectionBlockAllocator, m_pAllocBlocks, 0x0C);
CHECK_OFFSET(cSectionBlockAllocator, m_pMRUBlock, 0x10);
CHECK_SIZE(cSectionBlockAllocator, 0x14);

// cWorldMemoryMan: the world's memory manager (one instance: cWorldCache::m_pWorldMemMan, cWorldCache_init:
// cMemMan_alloc(0x13C, "WORLD MEMMAN")). On Tour 0x13C, same layout. Methods: cWorldMemoryMan_cWorldMemoryMan,
// cWorldMemoryMan__cWorldMemoryMan (dtor: destroys the three pools), cWorldMemoryMan_activateSectionMem, func_003A7B98
// (deactivateSectionMem); allocate() is inline (cWorldCache_addBxStreamDataTest: m_pSectionAllocator[section]).
struct cWorldMemoryMan {
    cWorldBlockAllocator m_BlockAllocator[3]; // 0x000 [O]; [R] ctor inits the three from m_BlockMemType;
                                    //       [U] activateSectionMem hands &m_BlockAllocator[blocktype] to the arena
    cSectionBlockAllocator* m_pSectionAllocator[64]; // 0x03C [P] assert !m_pSectionAllocator[uiSectionID];
                                    //       [R] ctor zeroes 64; [U] activate/deactivateSectionMem, allocate
};

CHECK_OFFSET(cWorldMemoryMan, m_BlockAllocator, 0x000);
CHECK_OFFSET(cWorldMemoryMan, m_pSectionAllocator, 0x03C);
CHECK_SIZE(cWorldMemoryMan, 0x13C);

// One resource added by a hull page (On Tour tPageEntry).
struct tPageEntry {
    unsigned char m_uiType;         // 0x0 [O] eBxStreamDataType; [U] purgePage/addBxStreamDataSubmit (byte)
    unsigned char m_uiSectionID;    // 0x1 [O]; [U] the low byte of the resource id (0xFF = shared)
    unsigned short m_uiGUID;        // 0x2 [O]; [U] the resource index (id >> 8)
};

CHECK_OFFSET(tPageEntry, m_uiType, 0x0);
CHECK_OFFSET(tPageEntry, m_uiSectionID, 0x1);
CHECK_OFFSET(tPageEntry, m_uiGUID, 0x2);
CHECK_SIZE(tPageEntry, 0x4);

// cHullPage (On Tour 0x10; retail cMemMan_alloc(0x10, "cHullPage") in cWorldCache_updatePages).
// Methods: cHullPage_cHullPage, func_003A7C30 (dtor); add/getEntry are inline.
struct cHullPage {
    const tBxBSPHull* m_pBSPHull;   // 0x0 [P] assert pBSPHull (ctor argument); [R] ctor
    unsigned int m_uiNumPageEntry;  // 0x4 [P] assert m_uiNumPageEntry <= 0xFFFF; [R] = hull->m_uiNumChunk; dtor = 0
    unsigned int m_uiCurrentPageEntry; // 0x8 [P] assert m_uiCurrentPageEntry < m_uiNumPageEntry; [R] = 0;
                                    //     [U] addBxStreamDataTest/Submit append, purgePage walks it down
    tPageEntry* m_pPageEntries;     // 0xC [P] assert m_pPageEntries; [R] new(n * 4, "cHullPage Entries")
};

CHECK_OFFSET(cHullPage, m_pBSPHull, 0x0);
CHECK_OFFSET(cHullPage, m_uiNumPageEntry, 0x4);
CHECK_OFFSET(cHullPage, m_uiCurrentPageEntry, 0x8);
CHECK_OFFSET(cHullPage, m_pPageEntries, 0xC);
CHECK_SIZE(cHullPage, 0x10);

// One resource slot (On Tour tWorldCacheDataTag): refcount in the low byte, (pointer >> 2) in the high 24 bits.
// Retail reads the count with lbu and the pointer as (word >> 8) << 2; cWorld_resetMap's view is the same bitfield.
struct tWorldCacheDataTag {
    unsigned int m_bfuiRefCnt : 8;  // [P] assert m_pCacheTable[dataType].m_pDataTag[uiUID].m_bfuiRefCnt
    unsigned int m_bfuiMemLoc : 24; // [O]
};

CHECK_SIZE(tWorldCacheDataTag, 0x4);

// One type's slots in a table (On Tour tWorldCacheDataTable).
struct tWorldCacheDataTable {
    tWorldCacheDataTag* m_pDataTag; // 0x0 [P] assert m_pCacheTable[dataType].m_pDataTag[uiUID]; [R] ctor
    unsigned int m_uiNumTag;        // 0x4 [O]; [R] ctor = the type's count (iTable[type])
};

CHECK_OFFSET(tWorldCacheDataTable, m_pDataTag, 0x0);
CHECK_OFFSET(tWorldCacheDataTable, m_uiNumTag, 0x4);
CHECK_SIZE(tWorldCacheDataTable, 0x8);

// cWorldCacheTable: the resource table of one section (cWorldCache_activateSectionMem: cMemMan_alloc(0xE8,
// "SectionTable")) or of the shared data (cWorldCache_init: cMemMan_alloc(0xE8, "SharedTable")). On Tour 0xE8, same
// layout. Methods: cWorldCacheTable_cWorldCacheTable, func_003A7D80 (dtor); get/set/testIncrement/testDecrement are
// inline: ~85 retail functions index World->m_pWorldCache->m_pSectionCacheTable[id & 0xFF]->m_pCacheTable[type].
struct cWorldCacheTable {
    char* m_pCacheTableMem;         // 0x00 [O]; [R] new(total * 4, "WorldCacheTableMem"); the dtor frees it
    tWorldCacheDataTable m_pCacheTable[28]; // 0x04 [P] assert m_pCacheTable[dataType] (kBxStreamData_Num = 28 types);
                                    //      [R] ctor clears and fills it
    short* m_piTable;               // 0xE4 [O]; [R] ctor stores its argument (the section's or header's iTable)
};

CHECK_OFFSET(cWorldCacheTable, m_pCacheTableMem, 0x00);
CHECK_OFFSET(cWorldCacheTable, m_pCacheTable, 0x04);
CHECK_OFFSET(cWorldCacheTable, m_piTable, 0xE4);
CHECK_SIZE(cWorldCacheTable, 0xE8);

// One hull page slot (On Tour tWorldCacheHullPage). 0x18 bytes.
struct tWorldCacheHullPage {
    int m_State;                    // 0x00 [P] assert m_HullPages[uiHullPage].m_State == kHullPage_Loading (states
                                    //      above); [U] touchPage, updatePages, isPageLoaded (== 3), isPageEmpty (== 0)
    int m_bLock;                    // 0x04 [O] (a bool); [U] func_003A84F8 (setPageLock), func_003A8510 (isPageLocked);
                                    //      touchPage/flushPage = 0; updatePages skips locked pages
    cHullPage* mp_Page;             // 0x08 [P] assert !m_HullPages[i].mp_Page; [U] updatePages allocates it,
                                    //      purgePage frees it (several views type it int)
    float m_fPriority;              // 0x0C [O]; [U] touchPage stores it (swc1); updatePages/flushOnePage compare it
    int m_iTouchTick;               // 0x10 [O]; [U] touchPage = the app tick; flushPage = tick - m_iTouchStaleAge
    int m_iRenderTick;              // 0x14 [O]; [U] updatePages stores the frame when a page goes InActive and frees it
                                    //      5 frames later
};

CHECK_OFFSET(tWorldCacheHullPage, m_State, 0x00);
CHECK_OFFSET(tWorldCacheHullPage, m_bLock, 0x04);
CHECK_OFFSET(tWorldCacheHullPage, mp_Page, 0x08);
CHECK_OFFSET(tWorldCacheHullPage, m_fPriority, 0x0C);
CHECK_OFFSET(tWorldCacheHullPage, m_iTouchTick, 0x10);
CHECK_OFFSET(tWorldCacheHullPage, m_iRenderTick, 0x14);
CHECK_SIZE(tWorldCacheHullPage, 0x18);

// cWorldCache methods (world/worldcache 0x003A7DD0-0x003A8F80): cWorldCache_cWorldCache, cWorldCache__cWorldCache,
// cWorldCache_purge, cWorldCache_reset, cWorldCache_init, cWorldCache_activateSectionMem, cWorldCache_deactivateSectionMem
// (deactivateSectionMem), cWorldCache_update, cWorldCache_initPageMap, cWorldCache_resetPageMap (resetPageMap), func_003A8448
// (touchPage), func_003A84C8 (flushPage), func_003A84F8 (setPageLock), func_003A8510 (isPageLocked), cWorldCache_purgePage
// (purgePage), cWorldCache_isPageLoaded (isPageLoaded), cWorldCache_isPageEmpty (isPageEmpty), cWorldCache_signalPageLoadComplete (signalPageLoadComplete),
// cWorldCache_updatePages, func_003A88A8 (flushInActivePages), func_003A89C8 (flushOnePage),
// cWorldCache_addBxStreamDataTest, func_003A8CD0 (addBxStreamDataSubmit), func_003A8E20 (removeBxStreamData),
// func_003A8F10 (activateSectionPatches). The section/focus/bandwidth wrappers are inlined into their callers.
struct cWorldCache {
    tStreamDataHeader* mp_DataHeader;  // 0x0000 [P] assert mp_DataHeader->m_uiNumSections; [R] init: FILE_load of
                                    //        "|<track>.sdb"; [U] purge frees it
    cWorldCacheTable* m_pSharedCacheTable; // 0x0004 [O]; [R] init: cMemMan_alloc(0xE8, "SharedTable"); [U] resources
                                    //        whose section byte is 0xFF
    cWorldCacheTable** m_pSectionCacheTable; // 0x0008 [P] assert !m_pSectionCacheTable[uiSectionID]; [R] init:
                                    //        new(numSections * 4, "SectionTables"); [U] activate/deactivateSectionMem
    cWorldMemoryMan* m_pWorldMemMan;   // 0x000C [O]; [R] init: cMemMan_alloc(0x13C, "WORLD MEMMAN")
    cWorldView m_WorldView;            // 0x0010 [O]; [R] ctor: cWorldView_cWorldView(this + 0x10)
    cStreamMan m_StreamMan;            // 0x0300 [O]; [R] ctor: cStreamMan_cStreamMan(this + 0x300)
    cChunkResolve* mp_ChunkResolve;    // 0x03E8 [O]; [R] init = cChunkResolve_construct(); [U] addBxStreamDataSubmit /
                                    //        removeBxStreamData call its vtable (slots 3, 4); purge deletes it
    unsigned int m_uiNumHullPage;      // 0x03EC [P] assert m_uiNumHullPage<(tUInt32)kBxBSPMaxHulls; [R] initPageMap =
                                    //        getNumHull()
    tWorldCacheHullPage m_HullPages[256]; // 0x03F0 [P] assert m_HullPages[uiHullPage]; [R] initPageMap clears them
    int m_iTouchStaleAge;              // 0x1BF0 [O]; [R] ctor = (int)(ticks per second * 0.1); [U] updatePages,
                                    //        flushPage
};

CHECK_OFFSET(cWorldCache, mp_DataHeader, 0x0000);
CHECK_OFFSET(cWorldCache, m_pSharedCacheTable, 0x0004);
CHECK_OFFSET(cWorldCache, m_pSectionCacheTable, 0x0008);
CHECK_OFFSET(cWorldCache, m_pWorldMemMan, 0x000C);
CHECK_OFFSET(cWorldCache, m_WorldView, 0x0010);
CHECK_OFFSET(cWorldCache, m_StreamMan, 0x0300);
CHECK_OFFSET(cWorldCache, mp_ChunkResolve, 0x03E8);
CHECK_OFFSET(cWorldCache, m_uiNumHullPage, 0x03EC);
CHECK_OFFSET(cWorldCache, m_HullPages, 0x03F0);
CHECK_OFFSET(cWorldCache, m_iTouchStaleAge, 0x1BF0);
CHECK_SIZE(cWorldCache, 0x1C00);

#endif /* WORLD_WORLDCACHE_H */
