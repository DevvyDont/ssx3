#ifndef INTERSECT_WORLDSPHERE_H
#define INTERSECT_WORLDSPHERE_H

// cWorldSphere: a world-intersection query shaped as a sphere (centre + radius), a cWorldIntersectType. The Nash
// grid's terrain/static-object intersectors call its virtuals (Transform into a model's space, IsTriInt,
// IsSphTreeInt, IsBoxInt, ...); tAvalancheNode_calculate builds one on the stack. EA's source:
// d:/ssx/code/source/intersect/worldsphere.cpp (preview asserts); retail methods are in the intersect/riderspheretree
// splat unit (0x32FB48..0x3303F0) plus GetIdealParam in intersect/worldsphtree (0x340A18).
// Vtable cWorldSphere_vtbl (0x0048E650, 9 slots: Transform, UnTransform, GetIdealParam, IsTriInt, Is2SidedTriInt,
// IsSphTreeInt, IsBoxInt, cWorldIntersectType_IntersectSubPatch__FPv, IsInBoxTransformed; no destructor slot), stored
// by the only constructor cWorldSphere_cWorldSphere.
// Size 0xA0: On Tour's cWorldSphere is 0xA0; retail's last member origRad is at 0x90 and the class is 16-byte
// aligned (t4Vector members). Base: cWorldIntersectType (0x00-0x60; EA header intersect/worldinttypes.h, preview
// asserts), written out inline here.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail constructor cWorldSphere_cWorldSphere (stores 0x0, 0x4, 0x8, 0x20, 0x30, 0x40, the vptr 0x50, 0x60,
//       0x70, 0x80, 0x90);
//   [U] how retail code uses the field (src/intersect/riderspheretree.cpp, the retail asm; cWorldIntersectType's own
//       methods cWorldIntersectType_IsMeshInt / _IsPatchInt for the base);
//   [O] SSX On Tour's DWARF (class cWorldIntersectType 0x60, class cWorldSphere 0xA0). On Tour's bools are 1 byte
//       (isTransformed 0x4, useRejectNormal 0x5); retail's are words (0x4, 0x8). Every other offset equals retail's.
// The preview's cWorldIntersectType was 0x10 smaller (tools/layout_xlate.py: preview 0x50/0x60 -> retail 0x60/0x70).

#include "layout.h"
#include "bx/math3d.h"

struct t4Vector;
struct tIntersectInfo;

struct cWorldSphere {
    // --- cWorldIntersectType (On Tour 0x60; data 0x00-0x50, then the vptr)
    int mPrecision;                 // 0x000 [O] tIntPrecision; [R] ctor: 0 when isLow, else 1;
                                    //       [U] cWorldSphere_GetIdealParam returns -1 when 0; IsPatchInt reads it
    int isTransformed;              // 0x004 [O] bool (retail word); [R] ctor 0; [U] Transform 1, UnTransform 0
    int useRejectNormal;            // 0x008 [O] bool (retail word: On Tour 0x5); [R] ctor 0; [U] IsMeshInt/IsPatchInt
                                    //       test it (lw)
    char unk_00C[0x4];              // 0x00C padding
    t4Vector rejectNormal;          // 0x010 [O]; [U] IsMeshInt/IsPatchInt load it (lqc2)
    t4Vector boxMax;                // 0x020 [O]; [R] ctor: centre + radius (w = 1); [U] IsPatchInt reads x/y/z
    t4Vector boxMin;                // 0x030 [O]; [R] ctor: centre - radius (w = 1); [U] IsPatchInt reads x/y/z
    t4Vector boxCenter;             // 0x040 [O]; [R] ctor: the centre
    void** mVtbl;                   // 0x050 [R] cWorldSphere_vtbl (g++ 2.95 puts the vptr after cWorldIntersectType's
                                    //       data);
                                    //       [U] IsMeshInt/IsPatchInt call virtuals through it (lw 0x50)
    char unk_054[0xC];              // 0x054 padding (cWorldIntersectType is 16-byte aligned)
    // --- cWorldSphere
    t4Vector cen;                   // 0x060 [O]; [R] ctor = acen; [U] Transform = mat * origCen, UnTransform = origCen;
                                    //       IsTriInt/Is2SidedTriInt/isIntersectingPlane/edgeSphereCollision/IsInBoxTransformed read it,
                                    //       IsSphTreeInt/IsBoxInt pass &cen
    float rad;                      // 0x070 [O]; [R] ctor = arad; [U] Transform = scale * origRad, UnTransform =
                                    //       origRad;
                                    //       GetIdealParam returns 2 * rad; every test reads it (lwc1)
    char unk_074[0xC];              // 0x074 padding (t4Vector alignment)
    t4Vector origCen;               // 0x080 [O]; [R] ctor = acen; [U] Transform/UnTransform read it
    float origRad;                  // 0x090 [O]; [R] ctor = arad; [U] Transform/UnTransform read it
    char unk_094[0xC];              // 0x094 padding (16-byte aligned class)

    // Methods bound to their retail symbols by asm labels.
    int IsInBoxTransformed(t4Vector* mn, t4Vector* mx) __asm__("cWorldSphere_IsInBoxTransformed");
    void Transform(t4Vector* mat, float scale) __asm__("cWorldSphere_Transform");
    void UnTransform() __asm__("cWorldSphere_UnTransform");
    int IsSphTreeInt(void* tree, tIntersectInfo* hit) __asm__("cWorldSphere_IsSphTreeInt");
    int IsBoxInt(void* bMin, void* bMax, tIntersectInfo* box) __asm__("cWorldSphere_IsBoxInt");
    int IsTriInt(t4Vector* vertA, t4Vector* vertB, t4Vector* vertC, t4Vector* plane, t4Vector* outPos,
                 t4Vector* outDir, float* outDist)
        __asm__("cWorldSphere_IsTriInt");
    int Is2SidedTriInt(t4Vector* vertA, t4Vector* vertB, t4Vector* vertC, t4Vector* plane, t4Vector* outPos,
                       t4Vector* outDir, float* outDist)
        __asm__("cWorldSphere_Is2SidedTriInt");
    int edgeSphereCollision(t4Vector* vert0, t4Vector* vert1, t4Vector* vert2)
        __asm__("cWorldSphere_edgeSphereCollision");
    int isIntersectingPlane(t4Vector* plane, t4Vector* point, float* out) __asm__("cWorldSphere_isIntersectingPlane");
    float GetIdealParam() __asm__("cWorldSphere_GetIdealParam");
};

CHECK_OFFSET(cWorldSphere, mPrecision, 0x000);
CHECK_OFFSET(cWorldSphere, isTransformed, 0x004);
CHECK_OFFSET(cWorldSphere, useRejectNormal, 0x008);
CHECK_OFFSET(cWorldSphere, unk_00C, 0x00C);
CHECK_OFFSET(cWorldSphere, rejectNormal, 0x010);
CHECK_OFFSET(cWorldSphere, boxMax, 0x020);
CHECK_OFFSET(cWorldSphere, boxMin, 0x030);
CHECK_OFFSET(cWorldSphere, boxCenter, 0x040);
CHECK_OFFSET(cWorldSphere, mVtbl, 0x050);
CHECK_OFFSET(cWorldSphere, unk_054, 0x054);
CHECK_OFFSET(cWorldSphere, cen, 0x060);
CHECK_OFFSET(cWorldSphere, rad, 0x070);
CHECK_OFFSET(cWorldSphere, unk_074, 0x074);
CHECK_OFFSET(cWorldSphere, origCen, 0x080);
CHECK_OFFSET(cWorldSphere, origRad, 0x090);
CHECK_OFFSET(cWorldSphere, unk_094, 0x094);
CHECK_SIZE(cWorldSphere, 0xA0);

#endif /* INTERSECT_WORLDSPHERE_H */
