#ifndef OBJECT_PARTICLENODE_H
#define OBJECT_PARTICLENODE_H

// cParticleNode: the scene node of a placed instance that only carries particle effects. It is a cMoveNode that adds
// no members and deletes itself once its last effect modifier has gone (update). Size 0x2C: cMemMan_alloc(0x2C,
// "cParticleNode" / "ParticleNode") + cParticleNode_cParticleNode1 in src/seg/seg_1FBE38.cpp (checkActiveNode, MakeParticleFunc,
// AddParticleFunc), and cMemMan_alloc(0x2C, "Particle") + cParticleNode_cParticleNode(bucket, stream) in the
// replay rebuild (src/replay/replay.cpp, node type 13); On Tour 0x28 (its cObjNode is 4 bytes smaller).
// Base: cMoveNode (object/movenode.h) -> cInstanceNode -> cSortObjNode -> cTypeObjNode -> cObjNode.
// No EA path in the preview (its strings only name the class: "cParticleNode", vtable _vt$13cParticleNode), so the
// header takes the retail unit's directory; the methods are in src/object/movenode.cpp (0x3578A8-0x3579A8) and the
// destructor in src/object/railmodifier.cpp (0x361D50).
// Constructors: cParticleNode_cParticleNode1(bucket, inst, pdata, oldNode): cMoveNode ctor (cMoveNode_cMoveNode1, type 0xD), vtable,
// cMoveNode_transferFromOld(oldNode), cMoveNode_addParticle(pdata); cParticleNode_cParticleNode(bucket, stream):
// cMoveNode's replay ctor + vtable. Vtable cParticleNode_vtbl (0x0048EE60, 51 slots; own: 1 dtor, 2 update,
// 12 writeToReplayFrame; the rest are cMoveNode's / cInstanceNode's).
//
// A FLAT layout: cMoveNode's fields copied from object/movenode.h (keep them in step); evidence tags as there:
// [P] preview inline accessors; [R] ctors/dtor/allocation; [U] retail use; [O] On Tour.

#include "layout.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;

struct cParticleNode {
    // --- cObjNode (0x10)
    int unk_00;                     // 0x00 never written by any node ctor (see object/instancenode.h)
    cObjNode* Next;                 // 0x04 [O] cObjNode::Next; [P] cObjNode() zeroes it; [U] bucket list
    cObjNode* Prev;                 // 0x08 [O] cObjNode::Prev; [P]; [U] bucket list
    void** mVtbl;                   // 0x0C [R] both ctors and the dtor store cParticleNode_vtbl; [U] update's
                                    //      self-delete (slot 1, flags 3)
    // --- cTypeObjNode / cSortObjNode / cInstanceNode (object/instancenode.h)
    short ObjNodeType;              // 0x10 [P] getType (lh 0x10); [O] cTypeObjNodeState; [R] cParticleNode_cParticleNode1 passes 0xD
    unsigned short Flags;           // 0x12 [O]; [P] bit 0 = RootTransformDirty; [R] cMoveNode_cMoveNode sets bit 0
    unsigned int SortIndex;         // 0x14 [P] getSortIndex (lw 0x14); [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x18 [P] getInstance (lw 0x18); [O]; [U] cMoveNode_addParticle passes it to the
                                    //      cParticleModifier ctor
    // --- cMoveNode (object/movenode.h)
    tModifierBlock* ModifierBlock;  // 0x1C [O]; [U] update: deletes the node when ModifierBlock is null or its
                                    //      effect chain (+0x10, cEffectLink head) is empty
    int Debounce;                   // 0x20 [O] int; [U] cMoveNode setDebounce/update/onCollide; [R] replay ctor
    float Fade;                     // 0x24 [O]; [U] cMoveNode addFade, render/renderModel; [R] replay ctor
    int TransferHack;               // 0x28 [O] bool (4 bytes); [U] cMoveNode render; [R] replay ctor
};

CHECK_OFFSET(cParticleNode, unk_00, 0x00);
CHECK_OFFSET(cParticleNode, Next, 0x04);
CHECK_OFFSET(cParticleNode, Prev, 0x08);
CHECK_OFFSET(cParticleNode, mVtbl, 0x0C);
CHECK_OFFSET(cParticleNode, ObjNodeType, 0x10);
CHECK_OFFSET(cParticleNode, Flags, 0x12);
CHECK_OFFSET(cParticleNode, SortIndex, 0x14);
CHECK_OFFSET(cParticleNode, Instance, 0x18);
CHECK_OFFSET(cParticleNode, ModifierBlock, 0x1C);
CHECK_OFFSET(cParticleNode, Debounce, 0x20);
CHECK_OFFSET(cParticleNode, Fade, 0x24);
CHECK_OFFSET(cParticleNode, TransferHack, 0x28);
CHECK_SIZE(cParticleNode, 0x2C);

#endif /* OBJECT_PARTICLENODE_H */
