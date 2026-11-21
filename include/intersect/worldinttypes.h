#ifndef INTERSECT_WORLDINTTYPES_H
#define INTERSECT_WORLDINTTYPES_H

// cWorldIntersectType: the base of every world-intersection query shape (cWorldSphTree, cWorldSphere, cWorldRider):
// precision, transformed flag, an optional reject normal and the query's world bound box, then the vptr. The nash
// queries (cBXNash_terrainIntersect / _staticObjectIntersect, cOverlapList_terrainIntersect / _staticObjectIntersect)
// take one and read its box (boxMax / boxMin) for the overlap test.
// EA's header: ../source/intersect/worldinttypes.h ([P] the preview's inline assert "!isTransformed" files under it).
// Size 0x60: [O] On Tour's cWorldIntersectType is 0x60; [R] the derived classes (intersect/worldsphtree.h,
// worldsphere.h, worldrider.h) put their first member at 0x60 and write this same base out inline.
// Retail bools are words (useRejectNormal at 0x8, On Tour 0x5); every other offset equals On Tour's.
//
// A FLAT layout, like main/game.h. Evidence tags: [O] On Tour's DWARF; [P] the preview; [R] the derived ctors;
// [U] how retail code uses the field.

#include "layout.h"
#include "bx/math3d.h"

struct cWorldIntersectType {
    int mPrecision;                 // 0x000 [O] tIntPrecision; [R] cWorldSphTree ctor: 0 when isLow, else 1
    int isTransformed;              // 0x004 [O] bool (retail word); [P] assert !isTransformed; [U] Transform 1,
                                    //       UnTransform 0
    int useRejectNormal;            // 0x008 [O] bool (retail word: On Tour 0x5); [U] cWorldIntersectType_IsMeshInt
    char unk_00C[0x4];              // 0x00C padding
    t4Vector rejectNormal;          // 0x010 [O]; [U] cWorldIntersectType_IsMeshInt / _IsPatchInt
    t4Vector boxMax;                // 0x020 [O]; [R] the derived ctors; [U] the nash queries' box (views sQuery_334458 /
                                    //       sQuery_333EF8 / sQry_336F30 / sQry_335D78 max at 0x20)
    t4Vector boxMin;                // 0x030 [O]; [R] ditto; [U] ditto (min at 0x30)
    t4Vector boxCenter;             // 0x040 [O]; [R] the derived ctors
    void** mVtbl;                   // 0x050 [R] the derived ctors / dtors store their vtable here
    char unk_054[0xC];              // 0x054 padding (16-byte aligned class)
};

CHECK_OFFSET(cWorldIntersectType, mPrecision, 0x000);
CHECK_OFFSET(cWorldIntersectType, isTransformed, 0x004);
CHECK_OFFSET(cWorldIntersectType, useRejectNormal, 0x008);
CHECK_OFFSET(cWorldIntersectType, unk_00C, 0x00C);
CHECK_OFFSET(cWorldIntersectType, rejectNormal, 0x010);
CHECK_OFFSET(cWorldIntersectType, boxMax, 0x020);
CHECK_OFFSET(cWorldIntersectType, boxMin, 0x030);
CHECK_OFFSET(cWorldIntersectType, boxCenter, 0x040);
CHECK_OFFSET(cWorldIntersectType, mVtbl, 0x050);
CHECK_OFFSET(cWorldIntersectType, unk_054, 0x054);
CHECK_SIZE(cWorldIntersectType, 0x60);

#endif /* INTERSECT_WORLDINTTYPES_H */
