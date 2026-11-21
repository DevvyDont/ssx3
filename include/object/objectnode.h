#ifndef OBJECT_OBJECTNODE_H
#define OBJECT_OBJECTNODE_H

// cObjectNode: the scene node of a plain placed object (node type 17): a cMoveNode that adds no members and only
// overrides its destructor and writeToReplayFrame (which forwards to cMoveNode_writeToReplayFrame). Built by the
// script command MakeObjectFunc in src/seg/seg_1FBE38.cpp (cObjectNode_cObjectNode, 44 bytes, bucket 1, with
// Solid / forceVisible) and by the replay rebuild (src/replay/replay.cpp, node type 17: cMemMan_alloc(44, "Object") +
// cObjectNode_cObjectNode1(bucket, stream)).
// No EA path in the preview (its strings only name the class: _vt$11cObjectNode and its destructor _$_11cObjectNode), so
// the header takes the retail unit's directory; the constructors and writeToReplayFrame are in src/object/movenode.cpp
// (0x00356DB0..0x00356E98) and the destructor in src/object/railmodifier.cpp (0x00361038).
// Size 0x2C: [R] cMemMan_alloc(44, "Object") (replay.cpp) and the 44-byte allocation in seg_1FBE38.cpp; [O] On Tour
// 0x28 = its cMoveNode (no own members; On Tour's cObjNode is 4 bytes smaller, see object/instancenode.h).
// Base: cMoveNode (object/movenode.h) -> cInstanceNode -> cSortObjNode -> cTypeObjNode -> cObjNode.
// Constructors: cObjectNode_cObjectNode(bucket, inst, solid, forceVisible): cMoveNode_cMoveNode1(this, bucket, 17, inst),
// vtable, then the record flags (forceVisible or bits 1|2 -> +4 -2; solid -> +0x20 -0x40); cObjectNode_cObjectNode1:
// cMoveNode's replay ctor + vtable. Destructor cObjectNode__cObjectNode__FPv (vtable, cMoveNode__cMoveNode). Vtable
// cObjectNode_vtbl (0x00490E80, 51 slots; own: 1 dtor, 12 writeToReplayFrame; the rest are cMoveNode's /
// cInstanceNode's / cSortObjNode's / cTypeObjNode's / cObjNode's).
//
// A FLAT layout: cMoveNode's fields copied from object/movenode.h (keep them in step); evidence tags as there:
// [P] preview inline accessors; [R] ctors/dtor/allocation; [U] retail use; [O] On Tour.

#include "layout.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;

struct cObjectNode {
    // --- cObjNode (0x10)
    int unk_00;                     // 0x00 never written by any node ctor (see object/instancenode.h)
    cObjNode* Next;                 // 0x04 [O] cObjNode::Next; [P] cObjNode() zeroes it; [U] bucket list
    cObjNode* Prev;                 // 0x08 [O] cObjNode::Prev; [P]; [U] bucket list
    void** mVtbl;                   // 0x0C [R] both ctors and the dtor store cObjectNode_vtbl; [P] the preview's
                                    //      ~cObjectNode stores it at 0xC too (tools/layout_xlate.py)
    // --- cTypeObjNode / cSortObjNode / cInstanceNode (object/instancenode.h)
    short ObjNodeType;              // 0x10 [P] getType (lh 0x10); [O] cTypeObjNodeState; [R] cObjectNode_cObjectNode
                                    //      passes 17 to cMoveNode_cMoveNode1
    unsigned short Flags;           // 0x12 [O]; [P] bit 0 = RootTransformDirty; [R] cMoveNode_cMoveNode1 sets bit 0
    unsigned int SortIndex;         // 0x14 [P] getSortIndex (lw 0x14); [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x18 [P] getInstance (lw 0x18); [O]; [U] cObjectNode_cObjectNode updates the
                                    //      record's flags word (+0x8) through it (view sMoveFlags_6DB0)
    // --- cMoveNode (object/movenode.h)
    tModifierBlock* ModifierBlock;  // 0x1C [O]; [U] cMoveNode's forwarders and update / render
    int Debounce;                   // 0x20 [O] int; [U] cMoveNode setDebounce/update/onCollide; [R] replay ctor
    float Fade;                     // 0x24 [O]; [U] cMoveNode addFade, render/renderModel; [R] replay ctor
    int TransferHack;               // 0x28 [O] bool (4 bytes); [U] cMoveNode render; [R] replay ctor
};

CHECK_OFFSET(cObjectNode, unk_00, 0x00);
CHECK_OFFSET(cObjectNode, Next, 0x04);
CHECK_OFFSET(cObjectNode, Prev, 0x08);
CHECK_OFFSET(cObjectNode, mVtbl, 0x0C);
CHECK_OFFSET(cObjectNode, ObjNodeType, 0x10);
CHECK_OFFSET(cObjectNode, Flags, 0x12);
CHECK_OFFSET(cObjectNode, SortIndex, 0x14);
CHECK_OFFSET(cObjectNode, Instance, 0x18);
CHECK_OFFSET(cObjectNode, ModifierBlock, 0x1C);
CHECK_OFFSET(cObjectNode, Debounce, 0x20);
CHECK_OFFSET(cObjectNode, Fade, 0x24);
CHECK_OFFSET(cObjectNode, TransferHack, 0x28);
CHECK_SIZE(cObjectNode, 0x2C);

#endif /* OBJECT_OBJECTNODE_H */
