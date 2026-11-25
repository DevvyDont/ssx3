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
#include "object/transformmodifier.h"
#include "world/worldview.h"
#include "ui/uithing.h"
#include "bx/math3d.h"
#include "object/spline3d.h"

struct tInstanceRecord;
struct tMultiNode;

// One node of a multi-spline modifier (On Tour struct tMultiNode, 0x60). [R] cMultiSplineModifier_allocNodes
// allocates the Nodes array 0x60 apart (element type tMultiNodeElem: this with an empty element ctor) next to the
// 0x40-byte AnimMats (tAnimMat: t4Matrix with one).
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

// World-script data that adds a multi-spline modifier (On Tour tAddMultiSplineData, 0x34; same layout). [O] On Tour's
// cMultiSplineModifier(const tAddMultiSplineData& sdata, tInstanceRecord* inst); [U]
// cMultiSplineModifier_cMultiSplineModifier (src/object/splinemodifier.cpp) reads every member from SplineId to NumModels at these offsets and widths.
struct tAddMultiSplineData {
    unsigned int unk_000;           // 0x00 [O] tResourceID InstId (no retail read)
    unsigned int SplineId;          // 0x04 [O] tResourceID; [U] cSpline_init(&Spline, SplineId)
    int Orient;                     // 0x08 [O]; [U] -> OrientType
    float Speed;                    // 0x0C [O]; [U] cMultiSplineModifier_setSpeed; its sign picks the direction
    float Heading;                  // 0x10 [O]; [U] degrees -> HeadTweak (radians)
    float StartDistance;            // 0x14 [O]; [U] -> OffsetDistance when non-zero
    float ColourA;                  // 0x18 [O]; [U] Colour's first component
    float ColourR;                  // 0x1C [O]; [U] Colour's second
    float ColourG;                  // 0x20 [O]; [U] Colour's third
    float ColourB;                  // 0x24 [O]; [U] Colour's fourth
    int DrawSpline;                 // 0x28 [O]; [U] DrawPath = DrawSpline != 0
    int NumModels;                  // 0x2C [O]; [U] -> NumModels
    char unk_030[0x4];              // 0x30 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tAddMultiSplineData, unk_000, 0x00);
CHECK_OFFSET(tAddMultiSplineData, SplineId, 0x04);
CHECK_OFFSET(tAddMultiSplineData, Orient, 0x08);
CHECK_OFFSET(tAddMultiSplineData, Speed, 0x0C);
CHECK_OFFSET(tAddMultiSplineData, Heading, 0x10);
CHECK_OFFSET(tAddMultiSplineData, StartDistance, 0x14);
CHECK_OFFSET(tAddMultiSplineData, ColourA, 0x18);
CHECK_OFFSET(tAddMultiSplineData, ColourR, 0x1C);
CHECK_OFFSET(tAddMultiSplineData, ColourG, 0x20);
CHECK_OFFSET(tAddMultiSplineData, ColourB, 0x24);
CHECK_OFFSET(tAddMultiSplineData, DrawSpline, 0x28);
CHECK_OFFSET(tAddMultiSplineData, NumModels, 0x2C);
CHECK_OFFSET(tAddMultiSplineData, unk_030, 0x30);
CHECK_SIZE(tAddMultiSplineData, 0x34);

struct cBxReplayFrame;

struct cMultiSplineModifier : public cTransformModifier {
    // cTransformModifier (object/transformmodifier.h, 0x4): the vptr at 0x00:
    //   [R] ctors store the 0x0048F168 vtable; the dtor stores it, then cTransformModifier_vtbl
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

    // Methods bound to their retail symbols by asm labels.
    int getFocusCount() __asm__("cMultiSplineModifier_getFocusCount");
    cMultiSplineModifier* cMultiSplineModifier1(cBxReplayFrame* stream)
        __asm__("cMultiSplineModifier_cMultiSplineModifier1");
    void allocNodes() __asm__("cMultiSplineModifier_allocNodes");
    void allocInstances() __asm__("cMultiSplineModifier_allocInstances");
    void setupNodes() __asm__("cMultiSplineModifier_setupNodes");
    void setSpeed(float speed) __asm__("cMultiSplineModifier_setSpeed");
    void setupOverlapSystem() __asm__("cMultiSplineModifier_setupOverlapSystem");
    void restoreOverlapSystem() __asm__("cMultiSplineModifier_restoreOverlapSystem");
    int incFocusCount() __asm__("cMultiSplineModifier_incFocusCount__FPv");
    int decFocusCount() __asm__("cMultiSplineModifier_decFocusCount__FPv");
    void* updateNodes() __asm__("cMultiSplineModifier_updateNodes");
};

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
