#ifndef INTERSECT_WORLDRIDER_H
#define INTERSECT_WORLDRIDER_H

// cWorldRider: a world-intersection query shaped as a rider's sphere tree (a cWorldIntersectType wrapping a
// cRiderSphereTree). Transform copies the wrapped tree into the private localTree and moves that into a model's space;
// the Is*Int virtuals forward to the current tree's cRiderSphereTree / bxSphereTree tests. The rider's motion code
// (cGroundMotion_checkWallCollisions, cAirMotion / cRailslideMotion_checkTerrainCollisions, cWipeoutMotion_resolve*,
// cRider_checkObjectCollisions) builds one on the stack around the rider's tree and hands it to the overlap lists.
// EA's source: d:/ssx/code/source/intersect/worldrider.cpp (preview asserts); retail: the first methods of the
// intersect/riderspheretree splat unit (0x0032F650 .. 0x0032FAC0).
// Vtable cWorldRider_vtbl (0x0048E590, 10 slots: Transform, UnTransform, GetIdealParam, IsTriInt, Is2SidedTriInt,
// IsSphTreeInt, IsBoxInt, cWorldIntersectType_IntersectSubPatch__FPv, IsInBoxTransformed, ~cWorldRider), stored by
// cWorldRider_cWorldRider and the destructor cWorldRider__cWorldRider.
// Size 0x320: [O] On Tour's cWorldRider is 0x320 with localTree (cRiderSphereTree, 0x2B0) at 0x70; [R] retail's ctor
// constructs localTree at +0x70 (cRiderSphereTree_cRiderSphereTree__FP16cRiderSphereTree) and cRiderSphereTree is
// 0x2B0 (intersect/riderspheretree.h), so the class ends at 0x320; no allocation (stack objects only; the callers'
// frames hold it with room to spare and store nothing past +0x320).
// Base: cWorldIntersectType (0x00-0x60; EA header intersect/worldinttypes.h), written out inline exactly as in
// intersect/worldsphere.h and intersect/worldsphtree.h (keep the three in step).
// The preview's cWorldRider was different: its own useRejectNormal / rejectNormal / rigidRejecting at 0x310 / 0x320 /
// 0x330 (setRejectionNormal__11cWorldRiderRC8t4Vector, setRigidRejecting__11cWorldRiderb); retail and On Tour keep
// the reject normal in the base and have no rigidRejecting. tools/layout_xlate.py: preview 0x50/0x54 -> retail
// 0x60/0x64 (the base grew by 0x10).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail constructor cWorldRider_cWorldRider (stores 0x0, 0x4, 0x8, 0x20/0x30 via GetMaskBoundBox, 0x40, the vptr
//       0x50, 0x60, 0x64, constructs 0x70) and destructor cWorldRider__cWorldRider;
//   [U] how retail code uses the field (src/intersect/riderspheretree.cpp, the retail asm);
//   [O] SSX On Tour's DWARF (class cWorldIntersectType 0x60, class cWorldRider 0x320). On Tour's bools are 1 byte
//       (isTransformed 0x4, useRejectNormal 0x5); retail's are words (0x4, 0x8). Every other offset equals retail's.

#include "layout.h"
#include "bx/math3d.h"
#include "intersect/riderspheretree.h"

struct cWorldRider {
    // --- cWorldIntersectType (On Tour 0x60; data 0x00-0x50, then the vptr)
    int mPrecision;                 // 0x000 [O] tIntPrecision; [R] ctor: 0 when isLow, else 1;
                                    //       [U] GetIdealParam returns -1 when 0
    int isTransformed;              // 0x004 [O] bool (retail word); [R] ctor 0; [U] Transform 1, UnTransform 0
    int useRejectNormal;            // 0x008 [O] bool (retail word: On Tour 0x5); [R] ctor 0; [U]
                                    //       cWorldIntersectType_IsMeshInt
    char unk_00C[0x4];              // 0x00C padding
    t4Vector rejectNormal;          // 0x010 [O]; [U] cWorldIntersectType_IsMeshInt / _IsPatchInt
    t4Vector boxMax;                // 0x020 [O]; [R] ctor: cRiderSphereTree_GetMaskBoundBox(ast, &boxMin, &boxMax)
    t4Vector boxMin;                // 0x030 [O]; [R] ditto
    t4Vector boxCenter;             // 0x040 [O]; [R] ctor: (boxMin + boxMax) * 0.5
    void** mVtbl;                   // 0x050 [R] cWorldRider_vtbl (ctor and dtor); [P] 0x50 in the preview's paired dtor
    char unk_054[0xC];              // 0x054 padding (cWorldIntersectType is 16-byte aligned)
    // --- cWorldRider
    cRiderSphereTree* st;           // 0x060 [O]; [R] ctor = ast; [P] preview 0x50; [U] the current tree: every Is*Int,
                                    //       IsInBoxTransformed and GetIdealParam use it; Transform sets it to &localTree,
                                    //       UnTransform back to origSt
    cRiderSphereTree* origSt;       // 0x064 [O]; [R] ctor = ast; [P] preview 0x54; [U] UnTransform restores st from it
    char unk_068[0x8];              // 0x068 padding (localTree is 16-byte aligned)
    cRiderSphereTree localTree;     // 0x070 [O]; [R] ctor constructs it, dtor destroys it (cRiderSphereTree__cRiderSphereTree);
                                    //       [U] Transform copies *st into it (cRiderSphereTree_operatorE) and
                                    //       cRiderSphereTree_ApplyTransform's it
};

CHECK_OFFSET(cWorldRider, mPrecision, 0x000);
CHECK_OFFSET(cWorldRider, isTransformed, 0x004);
CHECK_OFFSET(cWorldRider, useRejectNormal, 0x008);
CHECK_OFFSET(cWorldRider, unk_00C, 0x00C);
CHECK_OFFSET(cWorldRider, rejectNormal, 0x010);
CHECK_OFFSET(cWorldRider, boxMax, 0x020);
CHECK_OFFSET(cWorldRider, boxMin, 0x030);
CHECK_OFFSET(cWorldRider, boxCenter, 0x040);
CHECK_OFFSET(cWorldRider, mVtbl, 0x050);
CHECK_OFFSET(cWorldRider, unk_054, 0x054);
CHECK_OFFSET(cWorldRider, st, 0x060);
CHECK_OFFSET(cWorldRider, origSt, 0x064);
CHECK_OFFSET(cWorldRider, unk_068, 0x068);
CHECK_OFFSET(cWorldRider, localTree, 0x070);
CHECK_SIZE(cWorldRider, 0x320);

#endif /* INTERSECT_WORLDRIDER_H */
