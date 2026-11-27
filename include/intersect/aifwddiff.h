#ifndef INTERSECT_AIFWDDIFF_H
#define INTERSECT_AIFWDDIFF_H

// cAIFwdDiffCache: the AI's forward-difference cache of tessellated patches (intersect/aifwddiff): the cache entries,
// their age queue and a hash of patches. Methods: Init, InitializeCache, GetFwdDiff, TesselatePatch, AddToHash /
// RemoveFromHash, MoveToAgeQueueTail.
//
// Size 0x10 ([R] the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names, order
// and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` in the methods falls
// on a member start (tools/gen/self_offsets.py).

#include "layout.h"
#include "bx/math3d.h"

struct tAIFwdDiff;

struct tPatchRecord;

struct cAIFwdDiffCache {
    tAIFwdDiff* mCache;             // 0x000 [O]
    tAIFwdDiff* mAgeQueue;          // 0x004 [O]
    tAIFwdDiff** mPatchHash;        // 0x008 [O]
    unsigned int mHashSize;         // 0x00C [O]

    // Methods bound to their retail symbols by asm labels.
    void Init(int n) __asm__("cAIFwdDiffCache_Init");
    void GetFwdDiff(tPatchRecord* src, tAIFwdDiff** out) __asm__("cAIFwdDiffCache_GetFwdDiff");
    void AddToHash(tAIFwdDiff* node) __asm__("cAIFwdDiffCache_AddToHash");
    void MoveToAgeQueueTail(tAIFwdDiff* node) __asm__("cAIFwdDiffCache_MoveToAgeQueueTail");
    void TesselatePatch(tPatchRecord* patch, tAIFwdDiff* fwdDiff) __asm__("cAIFwdDiffCache_TesselatePatch");
    void InitializeCache(tAIFwdDiff* patch) __asm__("cAIFwdDiffCache_InitializeCache");
    void RemoveFromHash(tAIFwdDiff* node) __asm__("cAIFwdDiffCache_RemoveFromHash");
};

CHECK_OFFSET(cAIFwdDiffCache, mCache, 0x000);
CHECK_OFFSET(cAIFwdDiffCache, mAgeQueue, 0x004);
CHECK_OFFSET(cAIFwdDiffCache, mPatchHash, 0x008);
CHECK_OFFSET(cAIFwdDiffCache, mHashSize, 0x00C);
CHECK_SIZE(cAIFwdDiffCache, 0x10);

// One cached patch evaluation (On Tour struct tAIFwdInfo, 0x40): the normals and bounds of one sub-quad.
struct tAIFwdInfo {
    t4Vector mNorm0;                // 0x00 [O]
    t4Vector mNorm1;                // 0x10 [O]
    t4Vector mMin;                  // 0x20 [O]
    t4Vector mMax;                  // 0x30 [O]
};

CHECK_OFFSET(tAIFwdInfo, mNorm0, 0x00);
CHECK_OFFSET(tAIFwdInfo, mNorm1, 0x10);
CHECK_OFFSET(tAIFwdInfo, mMin, 0x20);
CHECK_OFFSET(tAIFwdInfo, mMax, 0x30);
CHECK_SIZE(tAIFwdInfo, 0x40);

// One forward-differenced patch in the cache (On Tour struct tAIFwdDiff, 0x1CD0): its key, the age-queue and
// hash links, the 10x10 vertex grid and the 9x9 / 3x3 sub-quad infos. [R] retail's cache entries are 0x1CD0 apart
// (view sCacheEntry_7890); [U] the cache code (src/intersect/aifwddiff.cpp, views sCacheNode_279D0 / sCacheNode_7B30)
// links mNext / mPrev / mHashLink and compares mKey.
struct tAIFwdDiff {
    // mKey of an entry that holds no patch (name ours): [U] cAIFwdDiffCache::Init stores it in every entry,
    // cAIFwdDiffCache_RemoveFromHash skips such an entry.
    enum { kUnusedKey = 0xFFFFFFFF };
    unsigned int mKey;              // 0x0000 [O]; [U] compared with the patch key
    tAIFwdDiff* mNext;              // 0x0004 [O]; [U] age-queue link
    tAIFwdDiff* mPrev;              // 0x0008 [O]; [U] age-queue link
    tAIFwdDiff* mHashLink;          // 0x000C [O]; [U] hash-chain link (cAIFwdDiffCache::mPatchHash)
    t4Vector mVertex[10][10];       // 0x0010 [O]
    tAIFwdInfo mInfo[9][9];         // 0x0650 [O]
    tAIFwdInfo mLowInfo[3][3];      // 0x1A90 [O]
};

CHECK_OFFSET(tAIFwdDiff, mKey, 0x0000);
CHECK_OFFSET(tAIFwdDiff, mNext, 0x0004);
CHECK_OFFSET(tAIFwdDiff, mPrev, 0x0008);
CHECK_OFFSET(tAIFwdDiff, mHashLink, 0x000C);
CHECK_OFFSET(tAIFwdDiff, mVertex, 0x0010);
CHECK_OFFSET(tAIFwdDiff, mInfo, 0x0650);
CHECK_OFFSET(tAIFwdDiff, mLowInfo, 0x1A90);
CHECK_SIZE(tAIFwdDiff, 0x1CD0);

#endif
