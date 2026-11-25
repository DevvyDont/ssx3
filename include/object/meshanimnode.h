#ifndef OBJECT_MESHANIMNODE_H
#define OBJECT_MESHANIMNODE_H

// cMeshAnimNode: an instance that breaks apart: each of its model's meshes (at most 24) flies off as debris with its
// own rotation (Qrot), spin axis / angle step (Axis / Angle) and position / velocity (Pos / Vel), under gravity scaled
// by Drag, until Life runs out (it fades during the last FadeTime); then expire either makes the instance dead (Type 0)
// or deletes the node (Type 2). The ctor takes each mesh's starting pose from the old node's animation matrices (or
// the mesh hierarchy under its root transform) and deletes the old node; init randomizes the spin and adds a random
// dispersal (Disperse) to the start velocity (InitVel). Node type 11. Made by MakeMeshAnimFunc and checkActiveNode
// (seg/seg_1FBE38) and the replay loader. EA's source: d:/ssx/code/source/object/meshanimnode.cpp (preview assert path).
// Retail methods: the object/instanceman splat unit (0x00351B40..0x00352AA4).
// Size 0x6C0: [R] cMemMan_alloc(0x6C0, "MeshAnim") in cReplay_restoreObject (src/replay/replay.cpp) and
// cMemMan_alloc(0x6C0) in MakeMeshAnimFunc / checkActiveNode; the last members Type / Drag are at 0x6B0 / 0x6B4 and the
// t4Vector arrays make the class 16-byte aligned; [O] On Tour 0x6C0 with the same member offsets (its smaller base is
// absorbed by the alignment gap before Qrot); [P] preview == retail (0xC, 0x18, 0x1C, 0x24, 0x6B0).
// Bases: cInstanceNode (0x00-0x1C, object/instancenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode), then
// cMeshAnimNodeState (On Tour 0xC) at 0x1C - after the base: the replay ctor reads the 0xC-byte state into +0x1C and
// writeToReplayFrame writes it from there (the state written out). The vptr is at 0xC.
// Constructors: cMeshAnimNode_cMeshAnimNode1 (bucket, record, tMakeMeshAnimData*, tCollideInfo*, cInstanceNode* old:
// cInstanceNode_cInstanceNode1(this, bucket, 11, record), vtable, Type / Life / FadeTime / Drag / AngVel from the data,
// per-mesh poses, delete old, Disperse, InitVel (+ the collision's velocity * VelScale), record flags, init) and
// cMeshAnimNode_cMeshAnimNode (bucket, cBxReplayFrame*: cInstanceNode_cInstanceNode, vtable, the 0xC-byte state, then
// Life = -1: a replayed node never draws). Destructor cMeshAnimNode__cMeshAnimNode__FPv (vtable,
// cInstanceNode__cInstanceNode). Vtable cMeshAnimNode_vtbl (0x0048F6D8, 46 slots: 1 dtor, 2 update, 4 render, 12
// writeToReplayFrame, 34 expire, 35 loseFocus; the rest are cInstanceNode's / cSortObjNode's / cTypeObjNode's /
// cObjNode's).
//
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/object/instanceman.cpp) and the 0x6C0 allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 5 paired methods, preview == retail;
//   [U] how retail code uses the field (src/object/instanceman.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cMeshAnimNodeState, cMeshAnimNode, tMakeMeshAnimData).
// tQuaternion is written as t4Vector (bx/math3d.h: x, y, z, w; tPose's Q is the same type).

#include "layout.h"
#include "object/instancenode.h"
#include "bx/math3d.h"

struct cObjNode;
struct tInstanceRecord;

struct cMeshAnimNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cMeshAnimNode_vtbl (0x0048F6D8) in both ctors and the dtor; [P] 0xC; [U] update calls slot 34 (expire),
    //       expire / loseFocus slot 1 (delete)
    // cMeshAnimNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] node type 11 (cInstanceNode_cInstanceNode1)
    //   Instance: [R] ctor1 reads its model (+0x80), matrix (+0x10) and scale (+0x84), marks its flags; [U] init /
    //       update / render read the model's mesh count, loseFocus resets its flags (lw)
    // --- cMeshAnimNodeState (On Tour 0xC; [R] the replay ctor reads 0xC bytes here, writeToReplayFrame writes them)
    float Life;                     // 0x01C [O]; [P] 0x1C; [R] ctor1 data Life (+0x10) (swc1), replay ctor -1.0; [U]
                                    //       update counts it down by the frame time and expires at <= 0; render
                                    //       skips <= 0 and fades with it
    float AngVel;                   // 0x020 [O]; [R] ctor1 data AngVel (+0x2C) in radians * update time (swc1); [U]
                                    //       init: Angle[i] = AngVel * rand(0.5..1) (lwc1)
    float FadeTime;                 // 0x024 [O]; [P] 0x24; [R] ctor1 data FadeTime (+0x30) (swc1); [U] render: alpha =
                                    //       Life once Life <= FadeTime (lwc1)
    char unk_028[0x8];              // 0x028 padding before the 16-byte aligned Qrot (no access)
    // --- cMeshAnimNode (On Tour offsets, unchanged)
    t4Vector Qrot[24];              // 0x030 [O] tQuaternion[24]; [R] both ctors construct 24 (empty ctor loop); ctor1
                                    //       stores each mesh pose's rotation (sq); [U] update spins it, render turns it
                                    //       into the mesh matrix
    t4Vector Axis[24];              // 0x1B0 [O]; [R] ctors construct 24; [U] init stores a random unit axis (sq),
                                    //       update reads it
    float Angle[24];                // 0x330 [O]; [U] init stores the per-mesh angle step (swc1), update reads it
    t4Vector Disperse;              // 0x390 [O]; [R] ctor1 (data DisperseX/Y/Z, sq); [U] init reads .x/.y/.z (lwc1)
    t4Vector InitVel;               // 0x3A0 [O]; [R] ctor1 (data VelX/Y/Z, plus the collision velocity * VelScale; sq);
                                    //       [U] init scales it into every Vel (lq)
    t4Vector Pos[24];               // 0x3B0 [O]; [R] ctors construct 24; ctor1 stores each mesh pose's position (sq);
                                    //       [U] update adds Vel, render uses it as the mesh matrix's position row
    t4Vector Vel[24];               // 0x530 [O]; [R] ctors construct 24; [U] init stores it (sq), update adds gravity
    int Type;                       // 0x6B0 [O]; [P] 0x6B0; [R] ctor1 data Type (+0x4) (sw); [U] expire: 0 ->
                                    //       makeThisDead, 2 -> delete; loseFocus: != 0 -> reset flags and delete (lw)
    float Drag;                     // 0x6B4 [O]; [R] ctor1 data Drag (+0x8) (swc1); [U] update: Vel.z += Drag * -980 *
                                    //       frame time (a gravity scale) (lwc1)
    char unk_6B8[0x8];              // 0x6B8 tail padding to the 16-byte alignment (no access)

    // Methods bound to their retail symbols by asm labels.
    void init() __asm__("cMeshAnimNode_init");
};

CHECK_OFFSET(cMeshAnimNode, Life, 0x01C);
CHECK_OFFSET(cMeshAnimNode, AngVel, 0x020);
CHECK_OFFSET(cMeshAnimNode, FadeTime, 0x024);
CHECK_OFFSET(cMeshAnimNode, unk_028, 0x028);
CHECK_OFFSET(cMeshAnimNode, Qrot, 0x030);
CHECK_OFFSET(cMeshAnimNode, Axis, 0x1B0);
CHECK_OFFSET(cMeshAnimNode, Angle, 0x330);
CHECK_OFFSET(cMeshAnimNode, Disperse, 0x390);
CHECK_OFFSET(cMeshAnimNode, InitVel, 0x3A0);
CHECK_OFFSET(cMeshAnimNode, Pos, 0x3B0);
CHECK_OFFSET(cMeshAnimNode, Vel, 0x530);
CHECK_OFFSET(cMeshAnimNode, Type, 0x6B0);
CHECK_OFFSET(cMeshAnimNode, Drag, 0x6B4);
CHECK_OFFSET(cMeshAnimNode, unk_6B8, 0x6B8);
CHECK_SIZE(cMeshAnimNode, 0x6C0);

#endif /* OBJECT_MESHANIMNODE_H */
