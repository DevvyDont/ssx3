#ifndef INTERSECT_WORLDLINE_H
#define INTERSECT_WORLDLINE_H

// cWorldLine: a world-intersection query shaped as a line segment (start point + direction), a cWorldIntersectType.
// The rider, camera and AI code build one on the stack (cGroundMotion / cAirPredictor_simStep / cRider_putAt /
// cCamera's view-blocking test / the hand-plant and wipeout probes, cBXNash_getGroundHeight) and hand it to the Nash
// grid (cBXNash_terrainIntersect1, the overlap lists), which call its virtuals. EA's source:
// d:/ssx/code/source/intersect/worldline.cpp (preview asserts); retail methods are in the intersect/riderspheretree
// splat unit (0x32E100..0x32F650, before cWorldRider) plus GetIdealParam in intersect/worldsphtree (0x340A10).
// Vtable cWorldLine_vtbl (0x0048E6A8, 9 slots: Transform, UnTransform, GetIdealParam, IsTriInt, Is2SidedTriInt,
// IsSphTreeInt, IsBoxInt, IntersectSubPatch, IsInBoxTransformed; no destructor slot), stored by the only constructor
// cWorldLine_cWorldLine.
// Size 0xB0: On Tour's cWorldLine is 0xB0; retail's last member idealParam is at 0xA0 and the class is 16-byte
// aligned (t4Vector members); every retail caller reserves exactly 0xB0 (176) stack bytes for it (matched C:
// char seg[176] / char line[176] in wipeoutmotion, handplantcontrol, camera, worldsphtree; sRay_*[0xB0] in rider.cpp
// and airpredictor.cpp). Base: cWorldIntersectType (0x00-0x60; EA header intersect/worldinttypes.h, preview asserts),
// written out inline exactly as in intersect/worldsphere.h, worldsphtree.h and worldrider.h (keep them in step).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail constructor cWorldLine_cWorldLine (stores 0x0, 0x4, 0x8, 0x20, 0x30, 0x40, the vptr 0x50, 0x60, 0x70,
//       0x80, 0x90, 0xA0);
//   [U] how retail code uses the field (src/intersect/riderspheretree.cpp, the retail asm; cWorldIntersectType's own
//       methods cWorldIntersectType_IsMeshInt / _IsPatchInt for the base);
//   [O] SSX On Tour's DWARF (class cWorldIntersectType 0x60, class cWorldLine 0xB0). On Tour's bools are 1 byte
//       (isTransformed 0x4, useRejectNormal 0x5); retail's are words (0x4, 0x8). Every other offset equals retail's.

#include "layout.h"
#include "bx/math3d.h"

struct cWorldLine {
    // --- cWorldIntersectType (On Tour 0x60; data 0x00-0x50, then the vptr)
    int mPrecision;                 // 0x000 [O] tIntPrecision; [R] ctor stores the precision argument (0..2)
    int isTransformed;              // 0x004 [O] bool (retail word); [R] ctor 0; [U] Transform 1, UnTransform 0
    int useRejectNormal;            // 0x008 [O] bool (retail word: On Tour 0x5); [R] ctor 0; [U]
                                    //       cWorldIntersectType_IsMeshInt / _IsPatchInt test it (lw)
    char unk_00C[0x4];              // 0x00C padding
    t4Vector rejectNormal;          // 0x010 [O]; [U] cWorldIntersectType_IsMeshInt / _IsPatchInt load it (lqc2)
    t4Vector boxMax;                // 0x020 [O]; [R] ctor: per-axis max of the two end points (w = 1)
    t4Vector boxMin;                // 0x030 [O]; [R] ctor: per-axis min of the two end points (w = 1)
    t4Vector boxCenter;             // 0x040 [O]; [R] ctor: (boxMin + boxMax) / 2
    void** mVtbl;                   // 0x050 [R] cWorldLine_vtbl (g++ 2.95 puts the vptr after cWorldIntersectType's
                                    //       data); [U] Is2SidedTriInt calls slot 4 (IsTriInt) through it
    char unk_054[0xC];              // 0x054 padding (cWorldIntersectType is 16-byte aligned)
    // --- cWorldLine
    t4Vector p0;                    // 0x060 [O]; [R] ctor = ap0; [U] Transform = mat * origP0 * scale, UnTransform =
                                    //       origP0; IsTriInt / IsBoxInt / IntersectSubPatch / IsInBoxTransformed read it
    t4Vector dp;                    // 0x070 [O]; [R] ctor = ap1 - ap0; [U] Transform = mat * origDp * scale,
                                    //       UnTransform = origDp; Is2SidedTriInt dots it with the normal
    t4Vector origP0;                // 0x080 [O]; [R] ctor = ap0; [U] Transform / UnTransform read it
    t4Vector origDp;                // 0x090 [O]; [R] ctor = ap1 - ap0; [U] Transform / UnTransform read it
    float idealParam;               // 0x0A0 [O]; [R] ctor = aIdealParam; [U] GetIdealParam returns it (lwc1)
    char unk_0A4[0xC];              // 0x0A4 padding (16-byte aligned class)
};

CHECK_OFFSET(cWorldLine, mPrecision, 0x000);
CHECK_OFFSET(cWorldLine, isTransformed, 0x004);
CHECK_OFFSET(cWorldLine, useRejectNormal, 0x008);
CHECK_OFFSET(cWorldLine, unk_00C, 0x00C);
CHECK_OFFSET(cWorldLine, rejectNormal, 0x010);
CHECK_OFFSET(cWorldLine, boxMax, 0x020);
CHECK_OFFSET(cWorldLine, boxMin, 0x030);
CHECK_OFFSET(cWorldLine, boxCenter, 0x040);
CHECK_OFFSET(cWorldLine, mVtbl, 0x050);
CHECK_OFFSET(cWorldLine, unk_054, 0x054);
CHECK_OFFSET(cWorldLine, p0, 0x060);
CHECK_OFFSET(cWorldLine, dp, 0x070);
CHECK_OFFSET(cWorldLine, origP0, 0x080);
CHECK_OFFSET(cWorldLine, origDp, 0x090);
CHECK_OFFSET(cWorldLine, idealParam, 0x0A0);
CHECK_OFFSET(cWorldLine, unk_0A4, 0x0A4);
CHECK_SIZE(cWorldLine, 0xB0);

#endif /* INTERSECT_WORLDLINE_H */
