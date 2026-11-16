#ifndef OBJECT_SPLINEMODIFIER_H
#define OBJECT_SPLINEMODIFIER_H

// cSplineModifier: the transform modifier that moves a placed instance along a world spline (transform type 1;
// lifts, gondolas, moving props). Size 0xF0: retail cMemMan_alloc(0xF0, "SplineModifier") in cMoveNode's addSpline
// (src/object/movenode.cpp) and the replay loader (src/object/modifierblock.cpp); On Tour 0xF0.
// EA's source: d:/ssx/code/source/object/splinemodifier.cpp (preview assert).
// Constructors cSplineModifier_cSplineModifier1 (tAddSplineData; 0x359460, src/object/multiparticle.cpp) and
// cSplineModifier_cSplineModifier (replay frame; reads 0x50 bytes into +0x10, then the cSpline); destructor
// cSplineModifier__cSplineModifier; vtable cSplineModifier_vtbl (0x0048F250, 27 slots).
// A FLAT layout: cTransformModifier (0x0: vptr) + cSplineModifierState (0x10-0x60, the replay block) + own fields.
// On Tour's state members are in the same order; its three bools are 1 byte, retail's are 4 (lw/sw), so Complete/Start/
// RootTransformDirty sit at 0x50/0x54/0x58 instead of 0x50/0x51/0x52. Own fields: every On Tour offset equals retail.
// Evidence tags: [R] ctors/dtor/allocation; [U] retail use (asm of every method listed in methods.txt); [O] On Tour.

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h"
#include "object/spline3d.h"

struct cSplineModifier {
    // --- cTransformModifier (vptr only)
    void** mVtbl;                   // 0x00 [R] ctors store cSplineModifier_vtbl; the dtor stores it, then the base
                                    //      D_004913F8
    char unk_04[0xc];               // 0x04 padding (the state is 16-byte aligned)
    // --- cSplineModifierState (0x50 bytes; [R] the replay ctor reads 0x50 bytes here)
    tBoundBox BoundBox;             // 0x10 [O]; [U] getBoundBox returns +0x10, setBoundBox (sq 0x10/0x20)
    int SequenceType;               // 0x30 [O]; [R] ctor1 from the add data; [U] update switches on it
    int OrientType;                 // 0x34 [O]; [R] ctor1; [U] updateRootTransform switches on it
    float HeadTweak;                // 0x38 [O]; [R] ctor1; [U] updateRootTransform
    float OffsetDistance;           // 0x3C [O]; [R] ctor1; [U] update advances it, updateRootTransform reads it
    float Force;                    // 0x40 [O]; [R] ctor1; [U] update
    float ForceTime;                // 0x44 [O]; [R] ctor1; [U] update counts it down
    float Velocity;                 // 0x48 [O]; [U] cSplineModifier_setSpeed (speed * 27.78), signal, update,
                                    //      updateIntersectInfo
    float Radius;                   // 0x4C [O]; [P]/[U] getRadius/setRadius (lwc1/swc1 0x4C); [R] ctor1 zeroes it
    int Complete;                   // 0x50 [O] bool (4 bytes in retail); [U] isComplete/clearComplete, update, signal
    int Start;                      // 0x54 [O] bool; [U] signal sets it, update tests it
    int RootTransformDirty;         // 0x58 [O] bool; [R] ctors store 1; [U] isRootTransformDirty, getRootTransform,
                                    //      updateRootTransform clears it
    char unk_5C[0x4];               // 0x5C padding (Transform is 16-byte aligned)
    // --- cSplineModifier
    t4Matrix Transform;             // 0x60 [O]; [U] updateRootTransform writes it; getRootTransform returns +0x60
    t4Vector Pos;                   // 0xA0 [O]; [U] updateRootTransform, updateIntersectInfo (lqc2 0xA0)
    t4Vector Dir;                   // 0xB0 [O]; [R] ctors zero it (sq); [U] updateRootTransform, updateIntersectInfo
    t4Vector DDir;                  // 0xC0 [O]; [R] ctors zero it; [U] updateIntersectInfo
    float X2;                       // 0xD0 [O]; [R] ctors zero it; [U] updateRootTransform, updateIntersectInfo
    float Y2;                       // 0xD4 [O]; [R] ctors zero it; [U] updateRootTransform, updateIntersectInfo
    cSpline Spline;                 // 0xD8 [O]; [R] ctors: SplineId -1, SegIndex 0, Seg 0, Length 0; the replay ctor
                                    //      calls cSpline_readFromReplayFrame(+0xD8)
    char unk_E8[0x8];               // 0xE8 padding to the 0xF0 allocation
};

CHECK_OFFSET(cSplineModifier, mVtbl, 0x00);
CHECK_OFFSET(cSplineModifier, unk_04, 0x04);
CHECK_OFFSET(cSplineModifier, BoundBox, 0x10);
CHECK_OFFSET(cSplineModifier, SequenceType, 0x30);
CHECK_OFFSET(cSplineModifier, OrientType, 0x34);
CHECK_OFFSET(cSplineModifier, HeadTweak, 0x38);
CHECK_OFFSET(cSplineModifier, OffsetDistance, 0x3C);
CHECK_OFFSET(cSplineModifier, Force, 0x40);
CHECK_OFFSET(cSplineModifier, ForceTime, 0x44);
CHECK_OFFSET(cSplineModifier, Velocity, 0x48);
CHECK_OFFSET(cSplineModifier, Radius, 0x4C);
CHECK_OFFSET(cSplineModifier, Complete, 0x50);
CHECK_OFFSET(cSplineModifier, Start, 0x54);
CHECK_OFFSET(cSplineModifier, RootTransformDirty, 0x58);
CHECK_OFFSET(cSplineModifier, unk_5C, 0x5C);
CHECK_OFFSET(cSplineModifier, Transform, 0x60);
CHECK_OFFSET(cSplineModifier, Pos, 0xA0);
CHECK_OFFSET(cSplineModifier, Dir, 0xB0);
CHECK_OFFSET(cSplineModifier, DDir, 0xC0);
CHECK_OFFSET(cSplineModifier, X2, 0xD0);
CHECK_OFFSET(cSplineModifier, Y2, 0xD4);
CHECK_OFFSET(cSplineModifier, Spline, 0xD8);
CHECK_OFFSET(cSplineModifier, unk_E8, 0xE8);
CHECK_SIZE(cSplineModifier, 0xF0);

#endif /* OBJECT_SPLINEMODIFIER_H */
