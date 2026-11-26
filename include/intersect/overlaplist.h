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
//   [U] how retail code uses the field (src/intersect/worldsphtree.cpp: the cOverlapList_get* methods, sTree_333EF8,
//       sList_334458, the raw offsets in cOverlapList_splineIntersect; the retail asm);
//   [O] SSX On Tour's DWARF (class cOverlapList 0xA14): same names, types and offsets as retail.

#include "layout.h"

struct cBXNash;
struct tInstanceRecord;
struct tPatchRecord;
struct cTypedSpaceNode;

struct cWorldIntersectType;
struct t4Vector;
struct tBoundBox;
struct tIntersectInfo;
struct tIntHint;

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

    // Methods bound to their retail symbols by asm labels.
    void getinstances(tBoundBox* box) __asm__("cOverlapList_getinstances");
    void gettyped(tBoundBox* box) __asm__("cOverlapList_gettyped");
    void getpatchesandinstances(tBoundBox* box) __asm__("cOverlapList_getpatchesandinstances");
    void geteverything(tBoundBox* box) __asm__("cOverlapList_geteverything");
    int terrainIntersect(cWorldIntersectType* query, tIntersectInfo* hits, int cap, tIntHint* hitCache)
        __asm__("cOverlapList_terrainIntersect");
    float terrainIntersect1(cWorldIntersectType* query, tIntersectInfo* out, tIntHint* hitCache)
        __asm__("cOverlapList_terrainIntersect1");
    int staticObjectIntersect(cWorldIntersectType* query, tIntersectInfo* buf, int cap, int full)
        __asm__("cOverlapList_staticObjectIntersect");
    int splineIntersect(t4Vector* pos, tIntersectInfo* intInfo, int mask, float testRadius)
        __asm__("cOverlapList_splineIntersect");
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

// cOverlapList's gather functors (On Tour's DWARF: OverlapInstancesCallback / OverlapPatchesCallback /
// OverlapTypedCallback 0x10, OverlapPatchesAndInstancesCallback 0x20, OverlapEverythingCallback 0x30). The overlap
// queries build one on the stack; nash_privy_QueryHelper_helpX* / nash_Cell_descend* call its operator() (retail
// OverlapInstancesCallback_operatorCall ...) for every object whose box overlaps, appending it while number < maximum
// ([P] assert "number < maximum", d:/ssx/code/source/intersect/overlaplist.cpp). Reference members are pointers here.
struct tBoundBox;               // world/worldview.h

struct nash_Cell;

struct OverlapInstancesCallback {
    tInstanceRecord** instances;    // 0x0 [O]; [U] the output array (view sCtx_3309D8 list)
    unsigned int* number;           // 0x4 [O] unsigned int&; [P] assert number < maximum; [U] the count
    unsigned int maximum;           // 0x8 [O]; [P] ditto
    const tBoundBox* box;           // 0xC [O] const tBoundBox&; [U] the overlap test (view sCtx_3309D8 box)

    // Methods bound to their retail symbols by asm labels.
    void operatorCall(nash_Cell* tree) __asm__("OverlapInstancesCallback_operatorCall");
};

CHECK_OFFSET(OverlapInstancesCallback, instances, 0x0);
CHECK_OFFSET(OverlapInstancesCallback, number, 0x4);
CHECK_OFFSET(OverlapInstancesCallback, maximum, 0x8);
CHECK_OFFSET(OverlapInstancesCallback, box, 0xC);
CHECK_SIZE(OverlapInstancesCallback, 0x10);

struct OverlapPatchesCallback {
    tPatchRecord** patches;         // 0x0 [O]; [U] the output array
    unsigned int* number;           // 0x4 [O] unsigned int&; [P]
    unsigned int maximum;           // 0x8 [O]; [P]
    const tBoundBox* box;           // 0xC [O]
};

CHECK_OFFSET(OverlapPatchesCallback, patches, 0x0);
CHECK_OFFSET(OverlapPatchesCallback, number, 0x4);
CHECK_OFFSET(OverlapPatchesCallback, maximum, 0x8);
CHECK_OFFSET(OverlapPatchesCallback, box, 0xC);
CHECK_SIZE(OverlapPatchesCallback, 0x10);

struct OverlapTypedCallback {
    cTypedSpaceNode** typed;        // 0x0 [O]; [U] the output array
    unsigned int* number;           // 0x4 [O] unsigned int&; [P]
    unsigned int maximum;           // 0x8 [O]; [P]
    const tBoundBox* box;           // 0xC [O]
};

CHECK_OFFSET(OverlapTypedCallback, typed, 0x0);
CHECK_OFFSET(OverlapTypedCallback, number, 0x4);
CHECK_OFFSET(OverlapTypedCallback, maximum, 0x8);
CHECK_OFFSET(OverlapTypedCallback, box, 0xC);
CHECK_SIZE(OverlapTypedCallback, 0x10);

struct OverlapPatchesAndInstancesCallback {
    OverlapInstancesCallback instancecallback; // 0x00 [O]; [U] the instance part (helpX3 / descend2)
    OverlapPatchesCallback patchescallback;    // 0x10 [O]; [U] the patch part (ctx + 0x10)
};

CHECK_OFFSET(OverlapPatchesAndInstancesCallback, instancecallback, 0x00);
CHECK_OFFSET(OverlapPatchesAndInstancesCallback, patchescallback, 0x10);
CHECK_SIZE(OverlapPatchesAndInstancesCallback, 0x20);

struct OverlapEverythingCallback {
    OverlapInstancesCallback instancecallback; // 0x00 [O]; [U] views sCtx_340DC0 / sCtx_33B748 nodes
    OverlapPatchesCallback patchescallback;    // 0x10 [O]; [U] ditto objs
    OverlapTypedCallback typedcallback;        // 0x20 [O]; [U] ditto all
};

CHECK_OFFSET(OverlapEverythingCallback, instancecallback, 0x00);
CHECK_OFFSET(OverlapEverythingCallback, patchescallback, 0x10);
CHECK_OFFSET(OverlapEverythingCallback, typedcallback, 0x20);
CHECK_SIZE(OverlapEverythingCallback, 0x30);

#endif /* INTERSECT_OVERLAPLIST_H */
