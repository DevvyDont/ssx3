#ifndef OBJECT_MOVENODE_H
#define OBJECT_MOVENODE_H

// cMoveNode: the scene node of a placed instance that can move or carry modifiers (tModifierBlock: root-transform,
// UV-scroll, tex-flip, physics, effect and rail modifiers); base of cFloatingNode, cAnimNode, cFlexBridgeNode,
// cFlexRailNode, cParticleNode, cObjectNode, cDebounceNode, ... EA's source: d:/ssx/code/source/object/movenode.cpp.
// Size 0x2C: subclasses that add no members are allocated 0x2C in retail - cMemMan_alloc(0x2C, "ParticleNode") and
// (0x2C, "cParticleNode") for cParticleNode (func_003578A8), (0x2C, "Object") for cObjectNode (func_00356DB0); On Tour has
// cParticleNode/cObjectNode == cMoveNode == 0x28 (its cObjNode is 4 bytes smaller, see object/instancenode.h).
// Constructors func_00355280 (bucket, type, inst: sets RootTransformDirty) and cMoveNode_cMoveNode, the replay-frame
// one (reads Debounce/Fade/TransferHack as three 4-byte blocks at +0x20/+0x24/+0x28, then
// tModifierBlock_readFromReplayFrame);
// destructor cMoveNode__cMoveNode; vtable cMoveNode_vtbl. Methods: src/object/movenode.cpp and the one-liners in
// src/object/railmodifier.cpp (methods.txt).
// A FLAT layout: cInstanceNode (0x00-0x1C, object/instancenode.h, copied inline) + cMoveNode's own fields, which are
// On Tour's members in On Tour's order, each 4 bytes later (TransferHack is a 4-byte bool).
// Evidence tags: [P] preview inline accessors (via instancenode.h); [R] ctors/dtor/allocation; [U] retail use; [O] On Tour.

#include "layout.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;

struct cMoveNode {
    // --- cObjNode (0x10)
    int unk_00;                     // 0x00 never written by any node ctor (see object/instancenode.h)
    cObjNode* Next;                 // 0x04 [O] cObjNode::Next; [P] cObjNode() zeroes it; [U] bucket list
    cObjNode* Prev;                 // 0x08 [O] cObjNode::Prev; [P]; [U] bucket list
    void** mVtbl;                   // 0x0C [R] each ctor stores its vtable; [U] every virtual call (cMoveNode_apply,
                                    //      update, render, ...)
    // --- cTypeObjNode / cSortObjNode / cInstanceNode (object/instancenode.h)
    short ObjNodeType;              // 0x10 [P] getType (lh 0x10); [O] cTypeObjNodeState
    unsigned short Flags;           // 0x12 [O]; [P] bit 0 = RootTransformDirty; [R] cMoveNode_cMoveNode sets bit 0; [U]
                                    //      cMoveNode_updateRootTransform, preUpdateModifierBlock
    unsigned int SortIndex;         // 0x14 [P] getSortIndex (lw 0x14); [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x18 [P] getInstance (lw 0x18); [O]; [U] the add* builders pass it to the modifier
                                    //      ctors, render, getRootTransform
    // --- cMoveNode
    tModifierBlock* ModifierBlock;  // 0x1C [O]; [R] cMoveNode_addModifierBlock allocates it (cMemMan_alloc(0x28,
                                    //      "ModifierBlock")); [U] every forwarder (func_00360DE8..func_00361010, add*/rem*,
                                    //      update, render)
    int Debounce;                   // 0x20 [O] int; [U] setDebounce stores (int)(seconds * fps) (cvt.w.s + swc1),
                                    //      update/onCollide count it down (lw/sw); [R] replay ctor reads 4 bytes
    float Fade;                     // 0x24 [O]; [U] addFade func_00355878 stores data +0x4, render/renderModel read it
                                    //      (lwc1); [R] replay ctor
    int TransferHack;               // 0x28 [O] bool (4 bytes); [U] render tests it (lw); [R] replay ctor
};

CHECK_OFFSET(cMoveNode, unk_00, 0x00);
CHECK_OFFSET(cMoveNode, Next, 0x04);
CHECK_OFFSET(cMoveNode, Prev, 0x08);
CHECK_OFFSET(cMoveNode, mVtbl, 0x0C);
CHECK_OFFSET(cMoveNode, ObjNodeType, 0x10);
CHECK_OFFSET(cMoveNode, Flags, 0x12);
CHECK_OFFSET(cMoveNode, SortIndex, 0x14);
CHECK_OFFSET(cMoveNode, Instance, 0x18);
CHECK_OFFSET(cMoveNode, ModifierBlock, 0x1C);
CHECK_OFFSET(cMoveNode, Debounce, 0x20);
CHECK_OFFSET(cMoveNode, Fade, 0x24);
CHECK_OFFSET(cMoveNode, TransferHack, 0x28);
CHECK_SIZE(cMoveNode, 0x2C);

#endif /* OBJECT_MOVENODE_H */
