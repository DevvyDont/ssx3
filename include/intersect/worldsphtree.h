#ifndef INTERSECT_WORLDSPHTREE_H
#define INTERSECT_WORLDSPHTREE_H

// cWorldSphTree: a world-intersection query shaped as a sphere tree (a cWorldIntersectType wrapping a bxSphereTree).
// Transform copies the wrapped tree into a private working copy and moves that into a model's space; the Is*Int
// virtuals forward to the current tree's SphereTree_* tests. cRollerModifier_collide builds one on the stack around
// its own tree. EA's source: d:/ssx/code/source/intersect/worldsphtree.cpp (preview asserts); methods are the first
// functions of the retail intersect/worldsphtree splat unit (0x3303F0..0x3309D4).
// Vtable cWorldSphTree_vtbl (0x0048E5F0, 10 slots: Transform, UnTransform, GetIdealParam, IsTriInt, Is2SidedTriInt,
// IsSphTreeInt, IsBoxInt, cWorldIntersectType_IntersectSubPatch__FPv, IsInBoxTransformed, ~cWorldSphTree), stored by
// cWorldSphTree_cWorldSphTree and the destructor cWorldSphTree__cWorldSphTree.
// Size 0x70: On Tour's cWorldSphTree is 0x70; retail's last member localTree is at 0x68 and the class is 16-byte
// aligned; cRollerModifier_collide reserves 0x70 stack bytes for it. Base: cWorldIntersectType (0x00-0x60; EA header
// intersect/worldinttypes.h), written out inline as in intersect/worldsphere.h.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail constructor cWorldSphTree_cWorldSphTree and destructor cWorldSphTree__cWorldSphTree;
//   [U] how retail code uses the field (src/intersect/worldsphtree.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (class cWorldIntersectType, class cWorldSphTree). Retail bools are words (useRejectNormal
//       at 0x8, On Tour 0x5); every other offset equals retail's.
// The preview's cWorldIntersectType was 0x10 smaller (tools/layout_xlate.py: preview 0x40/0x50/0x58 -> retail
// 0x50/0x60/0x68).

#include "layout.h"
#include "bx/math3d.h"

struct bxSphereTree;

struct t4Matrix;
struct t4Vector;
struct tIntersectInfo;

struct cWorldSphTree {
    // --- cWorldIntersectType (On Tour 0x60; data 0x00-0x50, then the vptr)
    int mPrecision;                 // 0x000 [O] tIntPrecision; [R] ctor: 0 when isLow, else 1;
                                    //       [U] GetIdealParam returns -1 when 0
    int isTransformed;              // 0x004 [O] bool (retail word); [R] ctor 0; [U] Transform 1, UnTransform 0
    int useRejectNormal;            // 0x008 [O] bool (retail word: On Tour 0x5); [R] ctor 0; [U]
                                    //       cWorldIntersectType_IsMeshInt
    char unk_00C[0x4];              // 0x00C padding
    t4Vector rejectNormal;          // 0x010 [O]; [U] cWorldIntersectType_IsMeshInt / _IsPatchInt
    t4Vector boxMax;                // 0x020 [O]; [R] ctor: tree centre + SphereTree_GetRadius (w = 1)
    t4Vector boxMin;                // 0x030 [O]; [R] ctor: tree centre - radius (w = 1)
    t4Vector boxCenter;             // 0x040 [O]; [R] ctor: the tree's transCenter
    void** mVtbl;                   // 0x050 [R] cWorldSphTree_vtbl (ctor and dtor)
    char unk_054[0xC];              // 0x054 padding (cWorldIntersectType is 16-byte aligned)
    // --- cWorldSphTree
    bxSphereTree* st;               // 0x060 [O]; [R] ctor = ast; [U] the current tree: every Is*Int and GetIdealParam
                                    //       pass it
                                    //       to SphereTree_*; Transform sets it to localTree, UnTransform back to origSt
    bxSphereTree* origSt;           // 0x064 [O]; [R] ctor = ast; [U] UnTransform restores st from it
    bxSphereTree* localTree;        // 0x068 [O]; [R] ctor: cMemMan_alloc(0x140, "SphereTree") + SphereTree::SphereTree;
                                    //       dtor operator_delete's it; [U] Transform copies st into it
                                    //       (bxSphereTree_operatorE)
    char unk_06C[0x4];              // 0x06C padding (16-byte aligned class)

    // Methods bound to their retail symbols by asm labels.
    bool IsInBoxTransformed(float* lo, float* hi) __asm__("cWorldSphTree_IsInBoxTransformed");
    float GetIdealParam() __asm__("cWorldSphTree_GetIdealParam");
    void Transform(t4Matrix* mat, float scale) __asm__("cWorldSphTree_Transform");
    bxSphereTree* UnTransform() __asm__("cWorldSphTree_UnTransform__FPv");
    int IsTriInt(int vertA, int vertB, int vertC, int n, int ipos, t4Vector* outDir, float* outLen)
        __asm__("cWorldSphTree_IsTriInt");
    int Is2SidedTriInt(int vertA, int vertB, int vertC, int n, int ipos, t4Vector* outDir, float* outLen)
        __asm__("cWorldSphTree_Is2SidedTriInt");
    int IsSphTreeInt(void* tree, tIntersectInfo* hit) __asm__("cWorldSphTree_IsSphTreeInt");
    int IsBoxInt(void* bMin, void* bMax, tIntersectInfo* hit) __asm__("cWorldSphTree_IsBoxInt");
};

CHECK_OFFSET(cWorldSphTree, mPrecision, 0x000);
CHECK_OFFSET(cWorldSphTree, isTransformed, 0x004);
CHECK_OFFSET(cWorldSphTree, useRejectNormal, 0x008);
CHECK_OFFSET(cWorldSphTree, unk_00C, 0x00C);
CHECK_OFFSET(cWorldSphTree, rejectNormal, 0x010);
CHECK_OFFSET(cWorldSphTree, boxMax, 0x020);
CHECK_OFFSET(cWorldSphTree, boxMin, 0x030);
CHECK_OFFSET(cWorldSphTree, boxCenter, 0x040);
CHECK_OFFSET(cWorldSphTree, mVtbl, 0x050);
CHECK_OFFSET(cWorldSphTree, unk_054, 0x054);
CHECK_OFFSET(cWorldSphTree, st, 0x060);
CHECK_OFFSET(cWorldSphTree, origSt, 0x064);
CHECK_OFFSET(cWorldSphTree, localTree, 0x068);
CHECK_OFFSET(cWorldSphTree, unk_06C, 0x06C);
CHECK_SIZE(cWorldSphTree, 0x70);

#endif /* INTERSECT_WORLDSPHTREE_H */
