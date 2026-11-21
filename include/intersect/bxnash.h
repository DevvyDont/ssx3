#ifndef INTERSECT_BXNASH_H
#define INTERSECT_BXNASH_H

// cBXNash: the game's world collision grid ("Nash"): a nash::Nash loose octree (eight top-level octants, each a root
// cell key + root cell) that the world's instances, patches, typed space nodes and splines are filed in, plus the AI's
// patch forward-difference cache. The game owns exactly one (cGame::BXNash, main/game.h); the rider, AI, camera and
// light code query it through cBXNash_terrainIntersect / _staticObjectIntersect / _splineIntersect / _getGroundHeight
// or through a cOverlapList bound to it (intersect/overlaplist.h). EA's source: d:/ssx/code/source/intersect/bxnash.h
// (preview asserts; the base is intersect/nash.h, nash.cpp). Retail methods: the intersect/worldsphtree splat unit
// (0x334800..0x3388E0, interleaved with static helpers); the base's are nash_Nash_* / nash_Octant_* / nash_Cell_* (intersect/aifwddiff).
// No vtable (no virtuals in either class).
// Size 0xA8: [R] cGame_load allocates it with cMemMan_alloc(168, "Nash") and constructs it inline (eight octant root
// cells, deletecount and TheAIFwdDiffCache zeroed); [O] On Tour's cBXNash is 0xA8 (Nash 0xA4 + the cache pointer).
// Base: nash::Nash (On Tour class Nash, 0xA4: Octant octant[2][2][2], deletecount), written out inline.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail allocation and inline constructor in cGame_load (main/gameload.cpp: view sGridGL / sCellGL);
//   [U] how retail code uses the field (src/intersect/worldsphtree.cpp: the cBXNash_* methods and the cOverlapList_*
//       queries through the overlap list's nash pointer; the camera trigger walkers in cameratriggerman.cpp);
//   [O] SSX On Tour's DWARF (class Nash 0xA4, class Octant 0x14, struct CellKey 0x10, class cBXNash 0xA8): same
//       offsets as retail.

#include "layout.h"

struct nash_Cell;
struct cAIFwdDiffCache;

// nash::CellKey (On Tour struct CellKey, 0x10): an octree cell's level and integer grid coordinates.
// nash_CellKey_proximitytest (intersect/aifwddiff) takes one as its this.
struct nash_CellKey {
    int level;                      // 0x000 [O]
    int x;                          // 0x004 [O]
    int y;                          // 0x008 [O]
    int z;                          // 0x00C [O]
};

CHECK_OFFSET(nash_CellKey, level, 0x000);
CHECK_OFFSET(nash_CellKey, x, 0x004);
CHECK_OFFSET(nash_CellKey, y, 0x008);
CHECK_OFFSET(nash_CellKey, z, 0x00C);
CHECK_SIZE(nash_CellKey, 0x10);

// nash::Octant (On Tour class Octant, 0x14): one top-level octant of the Nash: the key of its root cell and the root
// cell itself (0 while the octant is empty).
struct nash_Octant {
    nash_CellKey rootkey;           // 0x000 [O]; [U] the cBXNash_* / cOverlapList_* queries pass &octant[i].rootkey to
                                    //       nash_CellKey_proximitytest and the helpX walkers
    nash_Cell* rootcell;            // 0x010 [O]; [R] cGame_load zeroes it (sCellGL ctor); [U] the queries skip octants
                                    //       whose rootcell is 0
};

CHECK_OFFSET(nash_Octant, rootkey, 0x000);
CHECK_OFFSET(nash_Octant, rootcell, 0x010);
CHECK_SIZE(nash_Octant, 0x14);

struct cBXNash {
    // --- nash::Nash (On Tour 0xA4)
    nash_Octant octant[2][2][2];    // 0x000 [O] Octant octant[2][2][2] (x, y, z order unknown: retail walks them as a
                                    //       flat [8]); [R] cGame_load constructs the eight (rootcell = 0); [U] every
                                    //       cBXNash_* query reads octant[i].rootcell (lw 0x10 + 0x14 * i) and passes
                                    //       &octant[i] (addiu 0x14 * i)
    unsigned int deletecount;       // 0x0A0 [O]; [R] cGame_load 0; [U] cOverlapList_get* copy it into the overlap
                                    //       list's deletecount (sGrid_330BA8::unkA0)
    // --- cBXNash
    cAIFwdDiffCache* TheAIFwdDiffCache; // 0x0A4 [O]; [R] cGame_load 0; [U] initTheAIFwdDiffCache stores a new
                                    //       cAIFwdDiffCache, purgeTheAIFwdDiffCache deletes and zeroes it;
                                    //       terrainIntersect / getGroundHeight / cOverlapList_terrainIntersect pass it
                                    //       to the patch callbacks (cAIFwdDiffCache_GetFwdDiff)
};

CHECK_OFFSET(cBXNash, octant, 0x000);
CHECK_OFFSET(cBXNash, deletecount, 0x0A0);
CHECK_OFFSET(cBXNash, TheAIFwdDiffCache, 0x0A4);
CHECK_SIZE(cBXNash, 0xA8);

#endif /* INTERSECT_BXNASH_H */
