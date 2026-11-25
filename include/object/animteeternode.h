#ifndef OBJECT_ANIMTEETERNODE_H
#define OBJECT_ANIMTEETERNODE_H

// cAnimTeeterNode: an animated instance driven like a damped spring (a see-saw / teeter): riders pushing on its meshes
// (applyForce, slot 43) add torque about each mesh's up axis to TorqueSum; updateNode turns TorqueSum * TorqueScale,
// the spring towards PivotTime and the damping into the angular velocity W, moves the animation time CurrTime by
// AnimSpeed = W * dt (clamped to MaxAnimSpeed) and bounces it off FromTime / ToTime by BounceFactor, then runs
// cAnimNode's frame update. Node type 4. Made by MakeAnimTeeterFunc (seg/seg_1FBE38) and the replay loader. No EA source
// path in the preview asserts; retail methods: the intersect/worldsphtree splat unit (0x003421A0..0x00342764) and the
// dtor / getNodeState / getAnimDir in object/railmodifier (0x00361400..0x00361484).
// Size 0x80: [R] cMemMan_alloc(0x80, "AnimTeeter") in cReplay_restoreObject (src/replay/replay.cpp) and
// cMemMan_alloc(0x80) in MakeAnimTeeterFunc; the ctors run cAnimNode's ctor on +0x30 and cAnimNode is 0x50
// (object/animnode.h); the replay ctor reads the 0x30-byte state into +0x0 (writeToReplayFrame writes it);
// [O] On Tour 0x84 (cAnimTeeterNodeState 0x30 at 0x00 with the same offsets; the rest is not listed); [P] preview ==
// retail for 0x0-0x3C, 0x50, 0x56.
// Bases (state first, as in every EA node): cAnimTeeterNodeState (On Tour 0x30) at 0x00, then cAnimNode (0x50,
// object/animnode.h) at 0x30 = cAnimNodeState (0x14) + cMoveNode (0x2C, at 0x44, object/movenode.h) + cAnimNode's own
// members (0x70); all written out inline with those headers' member names. The vptr is cMoveNode's: 0x44 + 0xC = 0x50.
// Constructors: cAnimTeeterNode_cAnimTeeterNode1 (bucket, record, tMakeAnimTeeterData*: cAnimNode_cAnimNode(this +
// 0x30, bucket, 4, record, 0) - directly, no cAnimObjectNode - then the vtable and the state from the data, Flags |=
// RootTransformDirty) and cAnimTeeterNode_cAnimTeeterNode (bucket, cBxReplayFrame*: cAnimNode_cAnimNode1 on +0x30,
// vtable, the 0x30-byte state, Flags |= 1). Destructor cAnimTeeterNode__cAnimTeeterNode (vtable, cAnimNode__cAnimNode on
// +0x30, operator delete on flags & 1). Vtable cAnimTeeterNode_vtbl (53 slots): delta -0x44 (whole object) on 1 dtor, 12
// writeToReplayFrame, 15 updateNode, 19 getNodeState, 31 getAnimDir, 43 applyForce; 53 getAnimSpeed delta -0x30 (from
// the cAnimNode part); delta -0x14 on cAnimNode's slots (26, 27, 29, 30, 32, 33, 42, 51; this = +0x30); the rest are
// cMoveNode's / cInstanceNode's (this = +0x44).
//
// A FLAT layout like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors/dtor (src/intersect/worldsphtree.cpp, src/object/railmodifier.cpp) and the 0x80 allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 5 paired methods, 17 offsets, all equal to retail;
//   [U] how retail code uses the field (src/intersect/worldsphtree.cpp and the retail asm; for the cAnimNode part also
//       cAnimNode_* with this = +0x30);
//   [O] SSX On Tour's DWARF (cAnimTeeterNodeState, tMakeAnimTeeterData, cAnimNodeState, cAnimNode, cMoveNode chain).

#include "layout.h"
#include "bx/math3d.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;
struct cMeshAnimFrame;
struct tMeshVelocity;

// World-script data that makes an animated teeter (On Tour tMakeAnimTeeterData, 0x2C; same layout). [O] On Tour's
// cAnimTeeterNode(bucket, tInstanceRecord* pInstance, const tMakeAnimTeeterData* adata); [U]
// cAnimTeeterNode_cAnimTeeterNode1 (src/intersect/worldsphtree.cpp) reads every member from TorqueScale to ToTime into the node members of the same
// names (MaxSpeed into MaxAnimSpeed).
struct tMakeAnimTeeterData {
    unsigned int unk_000;           // 0x00 [O] tResourceID InstId (no retail read)
    float TorqueScale;              // 0x04 [O]; [U] -> TorqueScale (x 1e-6)
    float BounceFactor;             // 0x08 [O]; [U] -> BounceFactor
    float MaxSpeed;                 // 0x0C [O]; [U] -> MaxAnimSpeed (/ 30 / TimerHz)
    float Spring;                   // 0x10 [O]; [U] -> Spring
    float PivotTime;                // 0x14 [O]; [U] -> PivotTime (/ 30)
    float Damp;                     // 0x18 [O]; [U] -> Damp
    int AnimSign;                   // 0x1C [O]; [U] -> AnimSign
    float FromTime;                 // 0x20 [O]; [U] FromTime's rate setup
    float ToTime;                   // 0x24 [O]; [U] ToTime's rate setup
    char unk_028[0x4];              // 0x28 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tMakeAnimTeeterData, unk_000, 0x00);
CHECK_OFFSET(tMakeAnimTeeterData, TorqueScale, 0x04);
CHECK_OFFSET(tMakeAnimTeeterData, BounceFactor, 0x08);
CHECK_OFFSET(tMakeAnimTeeterData, MaxSpeed, 0x0C);
CHECK_OFFSET(tMakeAnimTeeterData, Spring, 0x10);
CHECK_OFFSET(tMakeAnimTeeterData, PivotTime, 0x14);
CHECK_OFFSET(tMakeAnimTeeterData, Damp, 0x18);
CHECK_OFFSET(tMakeAnimTeeterData, AnimSign, 0x1C);
CHECK_OFFSET(tMakeAnimTeeterData, FromTime, 0x20);
CHECK_OFFSET(tMakeAnimTeeterData, ToTime, 0x24);
CHECK_OFFSET(tMakeAnimTeeterData, unk_028, 0x28);
CHECK_SIZE(tMakeAnimTeeterData, 0x2C);

struct t4Vector;
struct tIntersectInfo;

struct cAnimTeeterNode {
    // --- cAnimTeeterNodeState (On Tour 0x30, same offsets; [R] the replay ctor reads 0x30 bytes at +0x0)
    float TorqueSum;                // 0x00 [O]; [P]; [R] ctor1 0 (sw $0); [U] applyForce adds each push's torque,
                                    //      updateNode uses it and clears it (lwc1/swc1)
    float AnimSpeed;                // 0x04 [O]; [P]; [R] ctor1 0; [U] updateNode = W * dt clamped to MaxAnimSpeed;
                                    //      getAnimSpeed / getAnimDir read it (lwc1)
    float TorqueScale;              // 0x08 [O]; [P]; [R] ctor1 data TorqueScale (+0x4) * 1e-6; [U] updateNode, its sign
                                    //      flips the lever arm in applyForce (lwc1)
    float BounceFactor;             // 0x0C [O]; [P]; [R] ctor1 data BounceFactor (+0x8); [U] updateNode scales W at the
                                    //      limits
    float FromTime;                 // 0x10 [O]; [P]; [R] ctor1 data FromTime (+0x20) / 30, or the animation's start time
                                    //      when negative; [U] updateNode lower limit, getAnimSpeed
    float ToTime;                   // 0x14 [O]; [P]; [R] ctor1 data ToTime (+0x24) / 30, or the model's animation length
                                    //      (+0x14) when negative; [U] updateNode upper limit, getAnimSpeed
    float MaxAnimSpeed;             // 0x18 [O]; [P]; [R] ctor1 data MaxSpeed (+0xC) / 30 / frame rate; [U] updateNode
                                    //      clamps AnimSpeed
    float Damp;                     // 0x1C [O]; [P]; [R] ctor1 data Damp (+0x18); [U] updateNode
    float Spring;                   // 0x20 [O]; [P]; [R] ctor1 data Spring (+0x10); [U] updateNode
    float PivotTime;                // 0x24 [O]; [P]; [R] ctor1 data PivotTime (+0x14) / 30; [U] updateNode (spring rest)
    float W;                        // 0x28 [O]; [P]; [R] ctor1 0 (sw $0); [U] updateNode integrates it (lwc1/swc1)
    int AnimSign;                   // 0x2C [O]; [R] ctor1 data AnimSign (+0x1C) (sw); [U] applyForce: bit i flips the
                                    //      lever arm of mesh i ((AnimSign >> mesh) & 1, lw)
    // --- cAnimNode at +0x30 (0x50; object/animnode.h): cAnimNodeState (On Tour 0x14)
    float CurrTime;                 // 0x30 [O]; [P] 0x30; [U] updateNode advances and clamps it, getAnimSpeed reads it;
                                    //      updateNode passes &CurrTime (the cAnimNode part) to cAnimNode_frameUpdate
    float CacheTime;                // 0x34 [O]; [U] updateNode = CurrTime (swc1)
    float FrameStart;               // 0x38 [O]; [U] updateNode = the previous CurrTime (swc1)
    float FrameEnd;                 // 0x3C [O]; [U] updateNode = the new CurrTime (swc1)
    int MeshVelocityDirty;          // 0x40 [O] bool (word); [U] cAnimNode_cacheFrame / updateMeshVelocity (this + 0x10)
    // --- cMoveNode at +0x44 (0x2C; object/movenode.h): cObjNode / cTypeObjNode / cSortObjNode / cInstanceNode first
    int MoveNodeBase;               // 0x44 where the cMoveNode base starts (name ours): `&MoveNodeBase` is the
                                    //      cMoveNode subobject the methods pass on (cMoveNode_* calls, vtable
                                    //      thunks). The word is cObjNode's unnamed leading word (unk_00 in
                                    //      object/objnode.h); no ctor writes it
    cObjNode* Next;                 // 0x48 [O] cObjNode::Next; [U] bucket list
    cObjNode* Prev;                 // 0x4C [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x50 [R] cAnimTeeterNode_vtbl (both ctors, the dtor); [P] 0x50; [U] applyForce calls slot 29
                                    //      (getAnimMat) with this = +0x44
    short ObjNodeType;              // 0x54 [O] cTypeObjNodeState; [R] node type 4 (cAnimNode_cAnimNode)
    unsigned short Flags;           // 0x56 [O]; [P] 0x56 bit 0 = RootTransformDirty; [R] both ctors set bit 0; [U]
                                    //      updateNode sets bit 0 (lhu/sh)
    unsigned int SortIndex;         // 0x58 [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x5C [O] cInstanceNode::Instance; [U] applyForce reads its model's (+0x80) mesh
                                    //      table (via the cMoveNode part, +0x44 + 0x18)
    tModifierBlock* ModifierBlock;  // 0x60 [O] cMoveNode
    int Debounce;                   // 0x64 [O] cMoveNode
    float Fade;                     // 0x68 [O] cMoveNode
    int TransferHack;               // 0x6C [O] cMoveNode (bool, 4 bytes)
    // --- cAnimNode's own members (relative to cAnimNode + 0x40)
    int NumAnimMeshes;              // 0x70 [O]; [U] cAnimNode_setAnimMeshCache / cacheFrame (this + 0x40)
    t4Matrix* AnimMats;             // 0x74 [O]; [U] cAnimNode_getAnimMats / getAnimMat (this + 0x44)
    cMeshAnimFrame* AnimFrames;     // 0x78 [O]; [U] ctor1 tests it and reads the first frame's start time
                                    //      (cAnimFrame_getStartTime, lw 0x78)
    tMeshVelocity* MeshVelocity;    // 0x7C [O]; [U] cAnimNode_updateMeshVelocity / updateIntersectInfo (this + 0x4C)

    // Methods bound to their retail symbols by asm labels.
    void* cAnimTeeterNode1(int bucket, tInstanceRecord* obj, tMakeAnimTeeterData* def)
        __asm__("cAnimTeeterNode_cAnimTeeterNode1");
    int updateNode() __asm__("cAnimTeeterNode_updateNode");
    float getAnimSpeed() __asm__("cAnimTeeterNode_getAnimSpeed");
    void applyForce(tIntersectInfo* intInfo, t4Vector* force) __asm__("cAnimTeeterNode_applyForce");
    void writeToReplayFrame(void* stream) __asm__("cAnimTeeterNode_writeToReplayFrame");
    int getAnimDir() __asm__("cAnimTeeterNode_getAnimDir");
};

CHECK_OFFSET(cAnimTeeterNode, TorqueSum, 0x00);
CHECK_OFFSET(cAnimTeeterNode, AnimSpeed, 0x04);
CHECK_OFFSET(cAnimTeeterNode, TorqueScale, 0x08);
CHECK_OFFSET(cAnimTeeterNode, BounceFactor, 0x0C);
CHECK_OFFSET(cAnimTeeterNode, FromTime, 0x10);
CHECK_OFFSET(cAnimTeeterNode, ToTime, 0x14);
CHECK_OFFSET(cAnimTeeterNode, MaxAnimSpeed, 0x18);
CHECK_OFFSET(cAnimTeeterNode, Damp, 0x1C);
CHECK_OFFSET(cAnimTeeterNode, Spring, 0x20);
CHECK_OFFSET(cAnimTeeterNode, PivotTime, 0x24);
CHECK_OFFSET(cAnimTeeterNode, W, 0x28);
CHECK_OFFSET(cAnimTeeterNode, AnimSign, 0x2C);
CHECK_OFFSET(cAnimTeeterNode, CurrTime, 0x30);
CHECK_OFFSET(cAnimTeeterNode, CacheTime, 0x34);
CHECK_OFFSET(cAnimTeeterNode, FrameStart, 0x38);
CHECK_OFFSET(cAnimTeeterNode, FrameEnd, 0x3C);
CHECK_OFFSET(cAnimTeeterNode, MeshVelocityDirty, 0x40);
CHECK_OFFSET(cAnimTeeterNode, MoveNodeBase, 0x44);
CHECK_OFFSET(cAnimTeeterNode, Next, 0x48);
CHECK_OFFSET(cAnimTeeterNode, Prev, 0x4C);
CHECK_OFFSET(cAnimTeeterNode, mVtbl, 0x50);
CHECK_OFFSET(cAnimTeeterNode, ObjNodeType, 0x54);
CHECK_OFFSET(cAnimTeeterNode, Flags, 0x56);
CHECK_OFFSET(cAnimTeeterNode, SortIndex, 0x58);
CHECK_OFFSET(cAnimTeeterNode, Instance, 0x5C);
CHECK_OFFSET(cAnimTeeterNode, ModifierBlock, 0x60);
CHECK_OFFSET(cAnimTeeterNode, Debounce, 0x64);
CHECK_OFFSET(cAnimTeeterNode, Fade, 0x68);
CHECK_OFFSET(cAnimTeeterNode, TransferHack, 0x6C);
CHECK_OFFSET(cAnimTeeterNode, NumAnimMeshes, 0x70);
CHECK_OFFSET(cAnimTeeterNode, AnimMats, 0x74);
CHECK_OFFSET(cAnimTeeterNode, AnimFrames, 0x78);
CHECK_OFFSET(cAnimTeeterNode, MeshVelocity, 0x7C);
CHECK_SIZE(cAnimTeeterNode, 0x80);

#endif /* OBJECT_ANIMTEETERNODE_H */
