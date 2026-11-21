#ifndef OBJECT_MULTISPLINEMODIFIER_H
#define OBJECT_MULTISPLINEMODIFIER_H

// cMultiSplineModifier: the transform modifier that runs a train of N instances along one spline (transform type 2;
// chair lifts, conveyor cars): one tMultiNode (0x60) per model and one spawned tInstanceRecord per car.
// Size 0x58: retail cMemMan_alloc(0x58, "MultiSplineModifier") in cMoveNode's addSpline (multi) and 0x58 in the replay
// loader; On Tour is 0x70 (its state is 16-byte aligned at +0x10, retail's packs at +0x4).
// EA's source: in d:/ssx/code/source/object/splinemodifier.cpp (its methods sit in that unit). Constructors
// cMultiSplineModifier_cMultiSplineModifier (tAddMultiSplineData) and _cMultiSplineModifier1 (replay frame: reads 0x34
// bytes into +0x4, the cSpline, the Instance pointer, then NumModels instance ids); destructor
// cMultiSplineModifier__cMultiSplineModifier (frees AnimMats/Nodes, releases NodeInst[i]); vtable 0x0048F168 (27 slots).
// A FLAT layout: cTransformModifier (0x0: vptr) + cMultiSplineModifierState (0x4-0x38, the replay block) + own fields.
// State member order is On Tour's; retail bools are 4 bytes and tARGB is 4-byte aligned (ui/uithing.h), which packs
// the 0x40-byte On Tour state into 0x34 bytes. Own fields keep On Tour's order, 0x18 lower.
// Evidence tags: [R] ctors/dtor/allocation; [U] retail use (asm of every method in methods.txt); [O] On Tour.

#include "layout.h"
#include "world/worldview.h"
#include "ui/uithing.h"
#include "bx/math3d.h"
#include "object/spline3d.h"

struct tInstanceRecord;
struct tMultiNode;

// One node of a multi-spline modifier (On Tour struct tMultiNode, 0x60). [R] cMultiSplineModifier_allocNodes
// allocates the Nodes array 0x60 apart (view sSplineSeg_A330, with an empty element ctor) next to the 0x40-byte
// AnimMats (view sSplineNode_A330 = t4Matrix).
struct tMultiNode {
    t4Vector Pos;                   // 0x00 [O]
    t4Vector Dir;                   // 0x10 [O]
    t4Vector DDir;                  // 0x20 [O]
    tBoundBox BoundBox;             // 0x30 [O]
    float X2;                       // 0x50 [O]
    float Y2;                       // 0x54 [O]
    char unk_058[0x8];              // 0x58 padding (16-byte aligned)
};

CHECK_OFFSET(tMultiNode, Pos, 0x00);
CHECK_OFFSET(tMultiNode, Dir, 0x10);
CHECK_OFFSET(tMultiNode, DDir, 0x20);
CHECK_OFFSET(tMultiNode, BoundBox, 0x30);
CHECK_OFFSET(tMultiNode, X2, 0x50);
CHECK_OFFSET(tMultiNode, Y2, 0x54);
CHECK_SIZE(tMultiNode, 0x60);

struct cMultiSplineModifier {
    // --- cTransformModifier (vptr only)
    void** mVtbl;                   // 0x00 [R] ctors store the 0x0048F168 vtable; the dtor stores it, then cTransformModifier_vtbl
    // --- cMultiSplineModifierState (0x34 bytes; [R] the replay ctor reads 0x34 bytes here)
    int NumModels;                  // 0x04 [O]; [R] ctor; [U] every per-car loop (allocNodes, setupNodes, render,
                                    //      writeToReplayFrame, dtor)
    int OrientType;                 // 0x08 [O]; [R] ctor; [U] updateNodes
    float HeadTweak;                // 0x0C [O]; [R] ctor; [U] updateNodes
    float OffsetDistance;           // 0x10 [O]; [R] ctor; [U] updateNodes advances it
    float Velocity;                 // 0x14 [O]; [U] cMultiSplineModifier_setSpeed stores it, updateIntersectInfo reads it
    float Radius;                   // 0x18 [O]; [P]/[U] getRadius/setRadius (lwc1/swc1 0x18), updateOverlapSystem; [R]
                                    //      ctor zeroes it (sw)
    int DrawPath;                   // 0x1C [O] bool; [R] ctor; [U] render tests it
    tARGB Colour;                   // 0x20 [O]; [R] ctor copies it with ldl/ldr + sdl/sdr pairs (4-byte aligned)
    int RootTransformDirty;         // 0x30 [O] bool; [R] ctors store 1; [U] getRootTransform/getRootTransform1, render,
                                    //      updateNodes
    int FocusCount;                 // 0x34 [O]; [R] ctor; [U] cMultiSplineModifier_getFocusCount (get), cMultiSplineModifier_incFocusCount/cMultiSplineModifier_decFocusCount (inc/dec)
    // --- cMultiSplineModifier
    t4Matrix* AnimMats;             // 0x38 [O]; [R] allocNodes: new[] of 0x40-byte matrices (one per anim mesh of the
                                    //      owner model), tag "MultiAnimMats"; [U] getAnimMats/render pass it to the model;
                                    //      the dtor frees it
    tMultiNode* Nodes;              // 0x3C [O]; [R] allocNodes: new[NumModels] of 0x60-byte tMultiNode, tag
                                    //      "MultiNodes"; [U] getBoundBox/setBoundBox, setupOverlapSystem, updateNodes; the
                                    //      dtor frees it
    tInstanceRecord* Instance;      // 0x40 [O]; [R] ctor stores the owner, the replay ctor reads it; [U] getAnimMats,
                                    //      allocNodes, render
    tInstanceRecord** NodeInst;     // 0x44 [O]; [U] allocNodes allocates it, allocInstances/setupNodes/render/dtor walk
                                    //      it
    cSpline Spline;                 // 0x48 [O]; [R] ctors: SplineId -1, SegIndex 0, Seg 0, Length 0; replay ctor reads
                                    //      it
};

CHECK_OFFSET(cMultiSplineModifier, mVtbl, 0x00);
CHECK_OFFSET(cMultiSplineModifier, NumModels, 0x04);
CHECK_OFFSET(cMultiSplineModifier, OrientType, 0x08);
CHECK_OFFSET(cMultiSplineModifier, HeadTweak, 0x0C);
CHECK_OFFSET(cMultiSplineModifier, OffsetDistance, 0x10);
CHECK_OFFSET(cMultiSplineModifier, Velocity, 0x14);
CHECK_OFFSET(cMultiSplineModifier, Radius, 0x18);
CHECK_OFFSET(cMultiSplineModifier, DrawPath, 0x1C);
CHECK_OFFSET(cMultiSplineModifier, Colour, 0x20);
CHECK_OFFSET(cMultiSplineModifier, RootTransformDirty, 0x30);
CHECK_OFFSET(cMultiSplineModifier, FocusCount, 0x34);
CHECK_OFFSET(cMultiSplineModifier, AnimMats, 0x38);
CHECK_OFFSET(cMultiSplineModifier, Nodes, 0x3C);
CHECK_OFFSET(cMultiSplineModifier, Instance, 0x40);
CHECK_OFFSET(cMultiSplineModifier, NodeInst, 0x44);
CHECK_OFFSET(cMultiSplineModifier, Spline, 0x48);
CHECK_SIZE(cMultiSplineModifier, 0x58);

#endif /* OBJECT_MULTISPLINEMODIFIER_H */
