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
// Retail code: cBezierCache::cBezierCache (ctor: zeroes the pointers), cBezierCache_allocateCaches (alloc: cMemMan_alloc of every table with the
// preview tags "4x4CacheRecord", "4x4FreeList", "4x4PosCache", "4x4UVCache", "4x4LMUVCache", "4x4NrmCache",
// "4x4FreeNrmList" and their 6x6 / 8x8 twins; cPS2BezierMan's ctor passes the sizes 900, 220, 140), cBezierCache_freeCaches /
// cBezierCache_clearRecords (reset / free lists), cBezierCache_computePatchNormals / cBezierCache_cachePatch (patch evaluation), cPS2BezierMan_GetBezST
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
// Members without an EA name are named from use and the allocation tags: "(name ours)".

#include "layout.h"
#include "bx/math3d.h"

struct t3Vector;

struct cBezierCache {
    int mCacheSize[3];                  // 0x000 [R] the table sizes per patch size (900, 220, 140 from cPS2BezierMan)
    void* m4x4CacheRecord;           // 0x00C [P] "m4x4CacheRecord == NULL"; [R] "4x4CacheRecord" table, 12-byte records
                                     //       (word 8 = -1 when free)
    void* m6x6CacheRecord;           // 0x010 [P] "m6x6CacheRecord == NULL"; [R] "6x6CacheRecord"
    void* m8x8CacheRecord;           // 0x014 [P] "m8x8CacheRecord == NULL"; [R] "8x8CacheRecord"
    void* mCacheRecord[3];           // 0x018 (name ours, after the tags) [U] the three record tables again, indexed by
                                     //       patch size
    int* mFreeListBuffer[3];         // 0x024 (name ours) [R] the "4x4FreeList" / 6x6 / 8x8 buffers
    int* mFreeList[3];               // 0x030 (name ours, after the tags) [U] the same free lists, indexed by patch
                                     //       size (cBezierCache_cachePatch pops mFreeList[Size][--mFreeListSP[Size]])
    int mFreeListSP[3];                  // 0x03C [U] free-list stack pointers (counts), indexed by patch size
    t3Vector* mPosCache[3];          // 0x048 (name ours, after the tags) [R] "4x4PosCache" / 6x6 / 8x8: n x n
                                     //       positions per record
    t2Vector* mUVCache[3];           // 0x054 (name ours, after the tags) [R] "4x4UVCache" / 6x6 / 8x8
    t2Vector* mLMUVCache[3];         // 0x060 (name ours, after the tags) [R] "4x4LMUVCache" / 6x6 / 8x8 (light-map
                                     //       UVs)
    char unk_06C[0x4];               // 0x06C padding (t4Vector alignment)
    t4Vector m4x4Basis[4];           // 0x070 (name ours) [U] 4x4 basis table (t^3, t^2, t, 1)
    t4Vector m6x6Basis[6];           // 0x0B0 (name ours) [U] 6x6 basis table
    t4Vector m8x8Basis[8];           // 0x110 (name ours) [U] 8x8 basis table
    t4Vector* mBasis[3];             // 0x190 (name ours) [U] the three basis tables, indexed by patch size
                                     //       (cPS2BezierMan_GetBezST)
    int mPatchOrder[3];              // 0x19C (name ours) [U] the patch order per size (4, 6, 8): the grid side n
    int mNrmCacheSize[3];                  // 0x1A8 [R] normal-cache sizes per patch size
    t3Vector* mNrmCache[3];          // 0x1B4 (name ours, after the tags) [R] "4x4NrmCache" / 6x6 / 8x8
    int* mFreeNrmListBuffer[3];      // 0x1C0 (name ours) [R] "4x4FreeNrmList" / 6x6 / 8x8 buffers
    int* mFreeNrmList[3];            // 0x1CC (name ours, after the tags) [U] the same normal free lists, indexed by
                                     //       patch size
    int mFreeNrmListSP[3];                  // 0x1D8 [U] normal free-list stack pointers
    char unk_1E4[0xC];               // 0x1E4 padding (t4Vector alignment)
    t4Vector m4x4DBasis[4];          // 0x1F0 (name ours) [U] 4x4 derivative basis table (3t^2, 2t, 1, 0)
    t4Vector m6x6DBasis[6];          // 0x230 (name ours) [U] 6x6 derivative basis table
    t4Vector m8x8DBasis[8];          // 0x290 (name ours) [U] 8x8 derivative basis table
    t4Vector* mDBasis[3];            // 0x310 (name ours) [U] the three derivative tables, indexed by patch size
                                     //       (cPS2BezierMan_GetBezdST)

    // Constructor / destructor: g++ 2.95 names their symbols itself (__<n>cX, _$_<n>cX).
    cBezierCache();
};

CHECK_OFFSET(cBezierCache, mCacheSize, 0x000);
CHECK_OFFSET(cBezierCache, m4x4CacheRecord, 0x00C);
CHECK_OFFSET(cBezierCache, m6x6CacheRecord, 0x010);
CHECK_OFFSET(cBezierCache, m8x8CacheRecord, 0x014);
CHECK_OFFSET(cBezierCache, mCacheRecord, 0x018);
CHECK_OFFSET(cBezierCache, mFreeListBuffer, 0x024);
CHECK_OFFSET(cBezierCache, mFreeList, 0x030);
CHECK_OFFSET(cBezierCache, mFreeListSP, 0x03C);
CHECK_OFFSET(cBezierCache, mPosCache, 0x048);
CHECK_OFFSET(cBezierCache, mUVCache, 0x054);
CHECK_OFFSET(cBezierCache, mLMUVCache, 0x060);
CHECK_OFFSET(cBezierCache, m4x4Basis, 0x070);
CHECK_OFFSET(cBezierCache, m6x6Basis, 0x0B0);
CHECK_OFFSET(cBezierCache, m8x8Basis, 0x110);
CHECK_OFFSET(cBezierCache, mBasis, 0x190);
CHECK_OFFSET(cBezierCache, mPatchOrder, 0x19C);
CHECK_OFFSET(cBezierCache, mNrmCacheSize, 0x1A8);
CHECK_OFFSET(cBezierCache, mNrmCache, 0x1B4);
CHECK_OFFSET(cBezierCache, mFreeNrmListBuffer, 0x1C0);
CHECK_OFFSET(cBezierCache, mFreeNrmList, 0x1CC);
CHECK_OFFSET(cBezierCache, mFreeNrmListSP, 0x1D8);
CHECK_OFFSET(cBezierCache, m4x4DBasis, 0x1F0);
CHECK_OFFSET(cBezierCache, m6x6DBasis, 0x230);
CHECK_OFFSET(cBezierCache, m8x8DBasis, 0x290);
CHECK_OFFSET(cBezierCache, mDBasis, 0x310);
CHECK_SIZE(cBezierCache, 0x320);

#endif /* RENDER_BEZIERCACHE_H */
