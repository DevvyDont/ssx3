#ifndef RENDER_BEZIERCACHE_H
#define RENDER_BEZIERCACHE_H

// cBezierCache: the bezier patch cache of the PS2 bezier manager: per patch size (4x4, 6x6, 8x8) a table of cache
// records, a free list, the tessellated positions, two UV sets and normals of the cached patches, and the basis tables
// used to evaluate them. EA's source: d:/ssx/code/source/render/beziercache.cpp (preview assert strings); the object is
// embedded in cPS2BezierMan at +0x10 (render/ps2bezierman.h, mPatchCache).
//
// Name: the class name follows EA's file name (beziercache.cpp) and the preview's member names m4x4CacheRecord /
// m6x6CacheRecord / m8x8CacheRecord; tPatchRecord::m_sCacheRecord[3] (world/patchrecord.h) holds a patch's record
// index in each table ([P] "Patch->m_sCacheRecord[Size] < 0").
// Retail code: func_00372B30 (ctor: zeroes the pointers), func_00372B78 (alloc: cMemMan_alloc of every table with the
// preview tags "4x4CacheRecord", "4x4FreeList", "4x4PosCache", "4x4UVCache", "4x4LMUVCache", "4x4NrmCache",
// "4x4FreeNrmList" and their 6x6 / 8x8 twins; cPS2BezierMan's ctor passes the sizes 900, 220, 140), func_003737D0 /
// func_003739D0 (reset / free lists), func_00374518 / func_003747A0 (patch evaluation), cPS2BezierMan_GetBezST
// (reads +0x190), cPS2BezierMan_GetBezdST (reads +0x310).
//
// Size 0x320: dbasis[3] ends at 0x31C and the t4Vector tables make the class 16-aligned; cPS2BezierMan's next member
// after it (mNearLODcm) is at cPS2BezierMan+0x33C, leaving 0xC bytes (cPS2BezierMan::unk_0330).
//
// Evidence tags:
//   [P] the July 2003 preview: assert strings "m4x4CacheRecord == NULL", "m6x6CacheRecord == NULL",
//       "m8x8CacheRecord == NULL" and the allocation tags above;
//   [R] retail allocation and evaluation code (the functions above; their views sBzCache_2B78, sPtSys_37D0,
//       sPtPools39D0, sPatchSet4518, sPMgr_47A0);
//   [U] how retail code uses the member.
// Members without an EA name keep unk_ names.

#include "layout.h"
#include "bx/math3d.h"

struct t3Vector;

struct cBezierCache {
    int mCacheSize[3];                  // 0x000 [R] the table sizes per patch size (900, 220, 140 from cPS2BezierMan)
    void* m4x4CacheRecord;           // 0x00C [P] "m4x4CacheRecord == NULL"; [R] "4x4CacheRecord" table, 12-byte records
                                     //       (word 8 = -1 when free)
    void* m6x6CacheRecord;           // 0x010 [P] "m6x6CacheRecord == NULL"; [R] "6x6CacheRecord"
    void* m8x8CacheRecord;           // 0x014 [P] "m8x8CacheRecord == NULL"; [R] "8x8CacheRecord"
    void* unk_018[3];                // 0x018 [U] the three record tables again, indexed by patch size
    int* unk_024[3];                 // 0x024 [R] the "4x4FreeList" / 6x6 / 8x8 buffers
    int* unk_030[3];                 // 0x030 [U] free-list cursors, indexed by patch size
    int mFreeListSP[3];                  // 0x03C [U] free-list stack pointers (counts), indexed by patch size
    t3Vector* unk_048[3];            // 0x048 [R] "4x4PosCache" / 6x6 / 8x8: n x n positions per record
    t2Vector* unk_054[3];            // 0x054 [R] "4x4UVCache" / 6x6 / 8x8
    t2Vector* unk_060[3];            // 0x060 [R] "4x4LMUVCache" / 6x6 / 8x8 (light-map UVs)
    char unk_06C[0x4];               // 0x06C padding (t4Vector alignment)
    t4Vector unk_070[4];             // 0x070 [U] 4x4 basis table
    t4Vector unk_0B0[6];             // 0x0B0 [U] 6x6 basis table
    t4Vector unk_110[8];             // 0x110 [U] 8x8 basis table
    t4Vector* unk_190[3];            // 0x190 [U] the three basis tables, indexed by patch size (cPS2BezierMan_GetBezST)
    int unk_19C[3];                  // 0x19C [U] the patch order per size (4, 6, 8)
    int mNrmCacheSize[3];                  // 0x1A8 [R] normal-cache sizes per patch size
    t3Vector* unk_1B4[3];            // 0x1B4 [R] "4x4NrmCache" / 6x6 / 8x8
    int* unk_1C0[3];                 // 0x1C0 [R] "4x4FreeNrmList" / 6x6 / 8x8 buffers
    int* unk_1CC[3];                 // 0x1CC [U] normal free-list cursors
    int mFreeNrmListSP[3];                  // 0x1D8 [U] normal free-list stack pointers
    char unk_1E4[0xC];               // 0x1E4 padding (t4Vector alignment)
    t4Vector unk_1F0[4];             // 0x1F0 [U] 4x4 derivative basis table
    t4Vector unk_230[6];             // 0x230 [U] 6x6 derivative basis table
    t4Vector unk_290[8];             // 0x290 [U] 8x8 derivative basis table
    t4Vector* unk_310[3];            // 0x310 [U] the three derivative tables, indexed by patch size
                                     //       (cPS2BezierMan_GetBezdST)
};

CHECK_OFFSET(cBezierCache, mCacheSize, 0x000);
CHECK_OFFSET(cBezierCache, m4x4CacheRecord, 0x00C);
CHECK_OFFSET(cBezierCache, m6x6CacheRecord, 0x010);
CHECK_OFFSET(cBezierCache, m8x8CacheRecord, 0x014);
CHECK_OFFSET(cBezierCache, unk_018, 0x018);
CHECK_OFFSET(cBezierCache, unk_024, 0x024);
CHECK_OFFSET(cBezierCache, unk_030, 0x030);
CHECK_OFFSET(cBezierCache, mFreeListSP, 0x03C);
CHECK_OFFSET(cBezierCache, unk_048, 0x048);
CHECK_OFFSET(cBezierCache, unk_054, 0x054);
CHECK_OFFSET(cBezierCache, unk_060, 0x060);
CHECK_OFFSET(cBezierCache, unk_070, 0x070);
CHECK_OFFSET(cBezierCache, unk_0B0, 0x0B0);
CHECK_OFFSET(cBezierCache, unk_110, 0x110);
CHECK_OFFSET(cBezierCache, unk_190, 0x190);
CHECK_OFFSET(cBezierCache, unk_19C, 0x19C);
CHECK_OFFSET(cBezierCache, mNrmCacheSize, 0x1A8);
CHECK_OFFSET(cBezierCache, unk_1B4, 0x1B4);
CHECK_OFFSET(cBezierCache, unk_1C0, 0x1C0);
CHECK_OFFSET(cBezierCache, unk_1CC, 0x1CC);
CHECK_OFFSET(cBezierCache, mFreeNrmListSP, 0x1D8);
CHECK_OFFSET(cBezierCache, unk_1F0, 0x1F0);
CHECK_OFFSET(cBezierCache, unk_230, 0x230);
CHECK_OFFSET(cBezierCache, unk_290, 0x290);
CHECK_OFFSET(cBezierCache, unk_310, 0x310);
CHECK_SIZE(cBezierCache, 0x320);

#endif /* RENDER_BEZIERCACHE_H */
