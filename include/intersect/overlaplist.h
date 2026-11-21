#ifndef INTERSECT_OVERLAPLIST_H
#define INTERSECT_OVERLAPLIST_H

// cOverlapList: a cached result of a box query on the world's Nash grid (cBXNash, intersect/bxnash.h): up to 64
// instances, 64 patches and 512 typed space nodes that overlap the box. The get* methods fill one or more of the three
// lists and mark the others as not collected (count -1); terrainIntersect / staticObjectIntersect / splineIntersect
// then test a cWorldIntersectType (or a point) against the collected objects only. Each rider owns one
// (cRider::mOverlapList, "OverlapList"); the world light manager keeps eight ("RTLOverlapList"). EA's source:
// d:/ssx/code/source/intersect/overlaplist.cpp (preview asserts); retail methods are in the intersect/worldsphtree
// splat unit (0x330BA8..0x334800). No vtable.
// Size 0xA14: [R] both allocations: cRider_cRider (ai/rider.cpp: operator new(sizeof(sRiderAux_B698) = 0xA14,
// "OverlapList")) and cWorldLightMan_initLightCache (visualfx/worldlightman.cpp: operator new(2580, "RTLOverlapList")),
// each followed by the inlined constructor (nash, deletecount = 0, the three counts = -1); [O] On Tour's cOverlapList
// is 0xA14. No base class.
//
// A FLAT layout, like main/game.h: every field at its retail offset. Evidence tags:
//   [R] the two allocations and inlined constructors above;
//   [U] how retail code uses the field (src/intersect/worldsphtree.cpp: views sQuery_330BA8 / _331450 / _331E90 /
//       _332DB8, sTree_333EF8, sList_334458, the raw offsets in cOverlapList_splineIntersect; the retail asm);
//   [O] SSX On Tour's DWARF (class cOverlapList 0xA14): same names, types and offsets as retail.

#include "layout.h"

struct cBXNash;
struct tInstanceRecord;
struct tPatchRecord;
struct cTypedSpaceNode;

struct cOverlapList {
    cBXNash* nash;                  // 0x000 [O]; [R] ctor = the game's Nash (cRider_cRider) / the light manager's
                                    //       m_pNash (initLightCache); [U] the get* walk its octants;
                                    //       terrainIntersect passes nash->TheAIFwdDiffCache
    unsigned int deletecount;       // 0x004 [O]; [R] ctor 0; [U] the get* copy nash->deletecount into it
    unsigned int numinstances;      // 0x008 [O]; [R] ctor -1 (not collected); [U] getinstances / getpatchesandinstances /
                                    //       geteverything reset it to 0 and count into it (max 64); gettyped sets -1
    tInstanceRecord* instances[64]; // 0x00C [O]; [U] the instance callback's list (OverlapInstancesCallback);
                                    //       terrainIntersect / staticObjectIntersect walk it
    unsigned int numpatches;        // 0x10C [O]; [R] ctor -1; [U] getpatchesandinstances / geteverything count into it;
                                    //       getinstances / gettyped set -1; terrainIntersect walks it
    tPatchRecord* patches[64];      // 0x110 [O]; [U] the patch lists of getpatchesandinstances / geteverything
    unsigned int numtyped;          // 0x210 [O]; [R] ctor -1; [U] gettyped / geteverything count into it (max 512);
                                    //       splineIntersect walks it; the other get* set -1
    cTypedSpaceNode* typed[512];    // 0x214 [O]; [U] gettyped / geteverything fill it; splineIntersect reads it
};

CHECK_OFFSET(cOverlapList, nash, 0x000);
CHECK_OFFSET(cOverlapList, deletecount, 0x004);
CHECK_OFFSET(cOverlapList, numinstances, 0x008);
CHECK_OFFSET(cOverlapList, instances, 0x00C);
CHECK_OFFSET(cOverlapList, numpatches, 0x10C);
CHECK_OFFSET(cOverlapList, patches, 0x110);
CHECK_OFFSET(cOverlapList, numtyped, 0x210);
CHECK_OFFSET(cOverlapList, typed, 0x214);
CHECK_SIZE(cOverlapList, 0xA14);

#endif /* INTERSECT_OVERLAPLIST_H */
