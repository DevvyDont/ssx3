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

// tResourceID: a world resource handle, one word: the section in the low 8 bits, the resource's index in that
// section's tables in the high 24 (0xFF = the shared table). Invalid() is 0xFFFFFFFF.
// Name and bitfield: [P] the preview's asserts "RID.m_bfRID.m_uiSectionID < cWorldCache::mp_DataHeader->
// m_uiNumSections" (an inline of this header), "instId.m_bfRID.m_uiUID < (unsigned int)NumInstances",
// "FlashInst[index] != tResourceID::Invalid()", "instanceid != tResourceID::Invalid()", and its mangled
// ...PC11tResourceID parameters; [O] On Tour struct tResourceID : _tResourceID (0x4, an anonymous union), with
// operator==, operator!=, tResourceID(), tResourceID(unsigned int rid), tResourceID(unsigned int sid, unsigned int uid),
// static Invalid(). Bit order: [U] the matched HandleToModel_229910 (src/main/ssxapp.cpp) and tResourceID_352810
// (src/object/instanceman.cpp) views read m_uiSectionID as the low byte. EA's header for it is not known; it sits here
// because the world cache resolves it (flattened: the base's union is inline).
// operator== / != compare the whole word ([U] cWorldLightMan's overlap-node search: one word compare against -1, the
// view tResourceID_5A70); the word member's name is not known (m_uiRID is ours; [U] cMultiParticle_render and
// cRailManNode_disableRail / enableRail split one loaded word into section (& 0xFF) and index (>> 8)).
struct tResourceID {
    union {
        struct {
            unsigned int m_uiSectionID : 8; // [P] asserts above; [U] the low byte (the section)
            unsigned int m_uiUID : 24;      // [P] asserts above; [U] the high 24 bits (the index)
        } m_bfRID;                          // 0x0 [P] "RID.m_bfRID.m_uiSectionID"
        unsigned int m_uiRID;               // 0x0 the whole word (On Tour tResourceID(unsigned int rid) stores it)
    };

    tResourceID() {}                                    // [O]
    tResourceID(unsigned int rid) { m_uiRID = rid; }    // [O]
    tResourceID(unsigned int sid, unsigned int uid)     // [O]
    {
        m_bfRID.m_uiSectionID = sid;
        m_bfRID.m_uiUID = uid;
    }
    bool operator==(const tResourceID& a) const { return m_uiRID == a.m_uiRID; } // [O]
    bool operator!=(const tResourceID& a) const { return m_uiRID != a.m_uiRID; } // [O]
    static const tResourceID Invalid() { return tResourceID(0xFFFFFFFF); }      // [O] [P] "tResourceID::Invalid()"
    enum { kInvalidRID = 0xFFFFFFFF };                  // Invalid()'s word (name ours), for word compares
};

CHECK_OFFSET(tResourceID, m_bfRID, 0x0);
CHECK_OFFSET(tResourceID, m_uiRID, 0x0);
CHECK_SIZE(tResourceID, 0x4);

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
// Methods: cWorldBlockAllocator_init, cWorldBlockAllocator__cWorldBlockAllocator (dtor), cWorldBlockAllocator_getFreeBlock, cWorldBlockAllocator_releaseBlock.
// The pool sizes are cWorldMemoryMan::m_BlockMemType[3] = D_0044C1E0: {type, uiNumBlock, uiBlockSize} =
// {0, 2, 0x4000}, {1, 0, 0x8000}, {2, 0x35, 0x20000}.
struct cWorldBlockAllocator {
    unsigned int m_uiBlockSize;     // 0x00 [O]; [R] init's third argument
    unsigned int m_uiTotalBlocks;   // 0x04 [O]; [R] init's second argument; [U] the dtor's loop bound
    unsigned int m_uiNumFreeBlocks; // 0x08 [P] assert m_uiNumFreeBlocks; [R] = count; [U] get/release -1/+1
    tWorldMemBlock* m_pBlockArray;  // 0x0C [O]; [R] new(count * 0x18, "WORLD BLOCK ARRAY")
    tWorldMemBlock* m_pFreeBlocks;  // 0x10 [P] assert m_pFreeBlocks; [R] = m_pBlockArray; [U] free-list head

    // Methods bound to their retail symbols by asm labels.
    void* getFreeBlock() __asm__("cWorldBlockAllocator_getFreeBlock__FPv");
    void releaseBlock(void* vitem) __asm__("cWorldBlockAllocator_releaseBlock__FPvT0");
    void init(int count, int blockSize) __asm__("cWorldBlockAllocator_init");
};

CHECK_OFFSET(cWorldBlockAllocator, m_uiBlockSize, 0x00);
CHECK_OFFSET(cWorldBlockAllocator, m_uiTotalBlocks, 0x04);
CHECK_OFFSET(cWorldBlockAllocator, m_uiNumFreeBlocks, 0x08);
CHECK_OFFSET(cWorldBlockAllocator, m_pBlockArray, 0x0C);
CHECK_OFFSET(cWorldBlockAllocator, m_pFreeBlocks, 0x10);
CHECK_SIZE(cWorldBlockAllocator, 0x14);

// One section's arena (On Tour cSectionBlockAllocator; retail cMemMan_alloc(0x14, "Section Allocator")).
// Methods: cSectionBlockAllocator_cSectionBlockAllocator (ctor), cSectionBlockAllocator__cSectionBlockAllocator (dtor), cSectionBlockAllocator_releaseBlocks, cSectionBlockAllocator_allocate.
struct cSectionBlockAllocator {
    cWorldBlockAllocator* m_pBlockAllocator; // 0x00 [P] assert pAllocator (ctor argument); [R] ctor
    unsigned int m_uiSectionID;     // 0x04 [O]; [R] ctor
    unsigned int m_uiNumBlocks;     // 0x08 [P] assert !m_uiNumBlocks; [R] = 0; [U] allocate +1, releaseBlocks -1
    tWorldMemBlock* m_pAllocBlocks; // 0x0C [O]; [R] = 0; [U] list of the arena's blocks
    tWorldMemBlock* m_pMRUBlock;    // 0x10 [P] assert m_pMRUBlock->iBaseSize; [R] = 0; [U] allocate's current block

    // Methods bound to their retail symbols by asm labels.
    void releaseBlocks() __asm__("cSectionBlockAllocator_releaseBlocks");
    char* allocate(int size, unsigned int flags) __asm__("cSectionBlockAllocator_allocate");
};

CHECK_OFFSET(cSectionBlockAllocator, m_pBlockAllocator, 0x00);
CHECK_OFFSET(cSectionBlockAllocator, m_uiSectionID, 0x04);
CHECK_OFFSET(cSectionBlockAllocator, m_uiNumBlocks, 0x08);
CHECK_OFFSET(cSectionBlockAllocator, m_pAllocBlocks, 0x0C);
CHECK_OFFSET(cSectionBlockAllocator, m_pMRUBlock, 0x10);
CHECK_SIZE(cSectionBlockAllocator, 0x14);

// cWorldMemoryMan: the world's memory manager (one instance: cWorldCache::m_pWorldMemMan, cWorldCache_init:
// cMemMan_alloc(0x13C, "WORLD MEMMAN")). On Tour 0x13C, same layout. Methods: cWorldMemoryMan_cWorldMemoryMan,
// cWorldMemoryMan__cWorldMemoryMan (dtor: destroys the three pools), cWorldMemoryMan_activateSectionMem, cWorldMemoryMan_deactivateSectionMem
// (deactivateSectionMem); allocate() is inline (cWorldCache_addBxStreamDataTest: m_pSectionAllocator[section]).
struct cWorldMemoryMan {
    cWorldBlockAllocator m_BlockAllocator[3]; // 0x000 [O]; [R] ctor inits the three from m_BlockMemType;
                                    //       [U] activateSectionMem hands &m_BlockAllocator[blocktype] to the arena
    cSectionBlockAllocator* m_pSectionAllocator[64]; // 0x03C [P] assert !m_pSectionAllocator[uiSectionID];
                                    //       [R] ctor zeroes 64; [U] activate/deactivateSectionMem, allocate

    // Methods bound to their retail symbols by asm labels.
    void activateSectionMem(int index, int section) __asm__("cWorldMemoryMan_activateSectionMem");
    void deactivateSectionMem(int i) __asm__("cWorldMemoryMan_deactivateSectionMem");
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
// Methods: cHullPage_cHullPage, cHullPage__cHullPage (dtor); add/getEntry are inline.
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

// The kinds of streamed world data (the type cPS2ChunkResolve::resolve / unresolve switch on, the index of a
// cWorldCacheTable's m_pCacheTable). [O] On Tour's enum eBxStreamDataType, names and values; [U] retail's resolve
// files patches (1), instances (3), lights (6), halos (7) and spline segments (8) in the space hash, hands images (9)
// and light pages (10) to the graphics manager as "strm_tex" / "strm_lpg", the collide models (12) to
// resolveCollideModel, the world painter (15), the world-script cache (16), the anchor table (18) and the AI path
// data (14) to their owners.
enum eBxStreamDataType {
    kBxStreamData_Material = 0, kBxStreamData_Patch = 1, kBxStreamData_Model = 2, kBxStreamData_Instance = 3,
    kBxStreamData_ParticleModel = 4, kBxStreamData_ParticleInstance = 5, kBxStreamData_RealTimeLight = 6,
    kBxStreamData_HaloLight = 7, kBxStreamData_Spline = 8, kBxStreamData_Image = 9, kBxStreamData_LightPage = 10,
    kBxStreamData_VisCurtain = 11, kBxStreamData_CollideModel = 12, kBxStreamData_IcePick = 13,
    kBxStreamData_AIPath = 14, kBxStreamData_WorldPainter = 15, kBxStreamData_ColdSpell = 16,
    kBxStreamData_CameraFile = 17, kBxStreamData_NISTable = 18, kBxStreamData_Mission = 19,
    kBxStreamData_AudioBank = 20, kBxStreamData_Radar = 21, kBxStreamData_Avalanche = 22,
    kBxStreamData_ModelBuffer = 23, kBxStreamData_RGBBuffer = 24, kBxStreamData_CoordBuffer = 25,
    kBxStreamData_NormalBuffer = 26, kBxStreamData_UVBuffer = 27, kBxStreamData_Num = 28
};

// cWorldCacheTable: the resource table of one section (cWorldCache_activateSectionMem: cMemMan_alloc(0xE8,
// "SectionTable")) or of the shared data (cWorldCache_init: cMemMan_alloc(0xE8, "SharedTable")). On Tour 0xE8, same
// layout. Methods: cWorldCacheTable_cWorldCacheTable, cWorldCacheTable__cWorldCacheTable (dtor); get/set/testIncrement/testDecrement are
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

// tWorldCacheHullPage::m_State. [O] On Tour's enum cWorldCache::eHullPageState, names and values; [P] the preview's
// asserts name kHullPage_Queued, kHullPage_Loading and kHullPage_InActive; [U] touchPage, updatePages,
// flushInActivePages and resetPageMap step the pages through them.
enum eHullPageState {
    kHullPage_Empty = 0, kHullPage_Queued = 1, kHullPage_Loading = 2, kHullPage_Active = 3, kHullPage_InActive = 4
};

// One hull page slot (On Tour tWorldCacheHullPage). 0x18 bytes.
struct tWorldCacheHullPage {
    int m_State;                    // 0x00 [P] assert m_HullPages[uiHullPage].m_State == kHullPage_Loading (states
                                    //      above); [U] touchPage, updatePages, isPageLoaded (== 3), isPageEmpty (== 0)
    int m_bLock;                    // 0x04 [O] (a bool); [U] cWorldCache_setPageLock, cWorldCache_isPageLocked;
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
// (deactivateSectionMem), cWorldCache_update, cWorldCache_initPageMap, cWorldCache_resetPageMap, cWorldCache_touchPage
// (touchPage), cWorldCache_flushPage, cWorldCache_setPageLock, cWorldCache_isPageLocked, cWorldCache_purgePage
// (purgePage), cWorldCache_isPageLoaded, cWorldCache_isPageEmpty, cWorldCache_signalPageLoadComplete,
// cWorldCache_updatePages, cWorldCache_flushInActivePages, cWorldCache_flushOnePage,
// cWorldCache_addBxStreamDataTest, cWorldCache_addBxStreamDataSubmit, cWorldCache_removeBxStreamData,
// cWorldCache_activateSectionPatches. The section/focus/bandwidth wrappers are inlined into their callers.
struct cWorldCache {
    // A tResourceID's bank byte for the shared data (name ours). [U] addBxStreamDataTest / addBxStreamDataSubmit /
    // removeBxStreamData use m_pSharedCacheTable for it and m_pSectionCacheTable[bank] otherwise.
    enum { kSharedBank = 0xFF };
    // addBxStreamDataTest's result when the bank has no room for the block yet (name ours). [U]
    // cStreamMan::processChunk then stops and the stream manager retries the block on its next update.
    enum { kStreamNoRoom = 0xFFFFFFFF };
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

    // Methods bound to their retail symbols by asm labels.
    void purge() __asm__("cWorldCache_purge");
    void reset() __asm__("cWorldCache_reset");
    void activateSectionMem(int i, int section) __asm__("cWorldCache_activateSectionMem");
    void deactivateSectionMem(int i) __asm__("cWorldCache_deactivateSectionMem");
    void update() __asm__("cWorldCache_update");
    void initPageMap() __asm__("cWorldCache_initPageMap");
    void resetPageMap() __asm__("cWorldCache_resetPageMap");
    void touchPage(int i, float priority) __asm__("cWorldCache_touchPage");
    void flushPage(int i) __asm__("cWorldCache_flushPage");
    void setPageLock(int hullPage, int lock) __asm__("cWorldCache_setPageLock");
    int isPageLocked(int hullPage) __asm__("cWorldCache_isPageLocked");
    void purgePage(int cell) __asm__("cWorldCache_purgePage");
    bool isPageLoaded(int uiHullPage) __asm__("cWorldCache_isPageLoaded");
    bool isPageEmpty(int uiHullPage) __asm__("cWorldCache_isPageEmpty");
    void signalPageLoadComplete(int uiHullPage) __asm__("cWorldCache_signalPageLoadComplete");
    void updatePages() __asm__("cWorldCache_updatePages");
    void flushInActivePages() __asm__("cWorldCache_flushInActivePages");
    void flushOnePage() __asm__("cWorldCache_flushOnePage");
    void* addBxStreamDataTest(int slot, int type, unsigned int key, int size)
        __asm__("cWorldCache_addBxStreamDataTest");
    void addBxStreamDataSubmit(int group, int id, unsigned int key, void* chunk)
        __asm__("cWorldCache_addBxStreamDataSubmit");
    void activateSectionPatches(int idx) __asm__("cWorldCache_activateSectionPatches");
    int init(const char* name, int pNash) __asm__("cWorldCache_init");
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
