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

struct tAIFwdDiff;

struct cAIFwdDiffCache {
    tAIFwdDiff* mCache;             // 0x000 [O]
    tAIFwdDiff* mAgeQueue;          // 0x004 [O]
    tAIFwdDiff** mPatchHash;        // 0x008 [O]
    unsigned int mHashSize;         // 0x00C [O]
};

CHECK_OFFSET(cAIFwdDiffCache, mCache, 0x000);
CHECK_OFFSET(cAIFwdDiffCache, mAgeQueue, 0x004);
CHECK_OFFSET(cAIFwdDiffCache, mPatchHash, 0x008);
CHECK_OFFSET(cAIFwdDiffCache, mHashSize, 0x00C);
CHECK_SIZE(cAIFwdDiffCache, 0x10);

#endif
