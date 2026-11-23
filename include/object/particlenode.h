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
// Derives from cMoveNode (object/movenode.h); evidence tags as there:
// [P] preview inline accessors; [R] ctors/dtor/allocation; [U] retail use; [O] On Tour.

#include "layout.h"
#include "object/movenode.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;

struct cParticleNode : public cMoveNode {
    // cMoveNode (object/movenode.h, 0x2C): the vptr at 0x0C:
    //   [R] both ctors and the dtor store cParticleNode_vtbl; [U] update's self-delete (slot 1, flags 3)
    // cParticleNode's notes on cMoveNode's fields:
    //   ObjNodeType: [R] cParticleNode_cParticleNode1 passes 0xD
    //   Instance: [U] cMoveNode_addParticle passes it to the cParticleModifier ctor
    //   ModifierBlock: [U] update: deletes the node when ModifierBlock is null or its effect chain (+0x10, cEffectLink
    //       head) is empty
};

CHECK_SIZE(cParticleNode, 0x2C);

#endif /* OBJECT_PARTICLENODE_H */
