#ifndef OBJECT_INSTANCENODE_H
#define OBJECT_INSTANCENODE_H

// cInstanceNode: the scene node of one placed world instance (a tInstanceRecord). It sits in a cBucketMan bucket
// list and is the base of cDeadNode, cMoveNode (-> cFloatingNode, cFlexRailNode, cFlexBridgeNode, ...), the flag-set
// and debounce nodes. Methods: src/object/instancenode.cpp, plus the one-line virtuals in src/object/railmodifier.cpp.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. The real chain is
//   cObjNode (0x00-0x10: links + vptr) -> cTypeObjNode (0x10-0x14) -> cSortObjNode (0x14-0x18) -> cInstanceNode
// (retail ctors chain cObjNode_cObjNode__FP8cObjNodePv -> func_00354720 -> func_00354850 -> func_0034FB00, each
// storing its own vtable at 0xC: D_00491F00, D_00491E80, D_00491E00, D_00491C80 = _vt$13cInstanceNode).
// Evidence tags:
//   [P] the July 2003 preview's inline accessors (getInstance lw 0x18, getSortIndex lw 0x14, getType lh 0x10,
//       is/set/resetRootTransformDirty lhu/sh 0x12 bit 0, cObjNode() zeroing 0x4/0x8); preview == retail here;
//   [R] retail constructors (src/object/objnode.cpp, src/object/floatingnode.cpp func_0034FB00);
//   [U] how matched retail code uses the field;
//   [O] SSX On Tour's DWARF (cObjNode Next/Prev, cTypeObjNodeState, cSortObjNodeState, cInstanceNode::Instance).
// On Tour's chain is 4 bytes smaller (cObjNode 0xC: Next 0, Prev 4, vptr 8); retail has an extra leading word.

#include "layout.h"

struct cObjNode;
struct tInstanceRecord;

struct cInstanceNode {
    // --- cObjNode (retail 0x10 bytes: bucket-list sentinels are 0x10 apart, cBucketMan_init / func_00354B38) ---
    int unk_00;                  // 0x000 never written by any node ctor (retail or preview cObjNode()); no On Tour twin
    cObjNode* Next;              // 0x004 [O] cObjNode::Next; [P] cObjNode() zeroes it; [U] forward link:
                                 //       cBucketMan_first/cBucketMan_next walk +0x4 from the head sentinel
    cObjNode* Prev;              // 0x008 [O] cObjNode::Prev; [P] cObjNode() zeroes it; [U] back link (cBucketMan_add,
                                 //       func_00354F20 walk +0x8 towards the head)
    void** mVtbl;                // 0x00C [R] each ctor's vtable (D_00491C80 in func_0034FB00/cInstanceNode_cInstanceNode)
    // --- cTypeObjNode (cTypeObjNodeState) ---
    short ObjNodeType;           // 0x010 [P] getType/isType (lh 0x10); [O] cTypeObjNodeState::ObjNodeType; [R] func_00354720
                                 //       stores the tObjNodeType arg and counts it in D_0044AFF0 (cTypeObjNode::ObjNodeCount)
    unsigned short Flags;        // 0x012 [O] cTypeObjNodeState::Flags; [P] RootTransformDirty = bit 0 (lhu/sh);
                                 //       [R] func_00354720 zeroes it; [U] cMoveNode_cMoveNode sets bit 0
    // --- cSortObjNode (cSortObjNodeState) ---
    unsigned int SortIndex;      // 0x014 [P] getSortIndex (lw 0x14); [O] cSortObjNodeState::SortIndex; [R] func_00354850
                                 //       takes it from ObjNodeSortIndex/ObjNodeNoSortIndex (D_004A5B78/7C); [U] unsigned
                                 //       compare in cSortObjNode_compare
    // --- cInstanceNode ---
    tInstanceRecord* Instance;   // 0x018 [P] getInstance (lw 0x18); [O] cInstanceNode::Instance; [R] func_0034FB00 stores
                                 //       its record arg; [U] record flags at +0x8, root transform at +0x10, model at +0x80
};

CHECK_OFFSET(cInstanceNode, unk_00, 0x00);
CHECK_OFFSET(cInstanceNode, Next, 0x04);
CHECK_OFFSET(cInstanceNode, Prev, 0x08);
CHECK_OFFSET(cInstanceNode, mVtbl, 0x0C);
CHECK_OFFSET(cInstanceNode, ObjNodeType, 0x10);
CHECK_OFFSET(cInstanceNode, Flags, 0x12);
CHECK_OFFSET(cInstanceNode, SortIndex, 0x14);
CHECK_OFFSET(cInstanceNode, Instance, 0x18);
// [R] cDeadNode (adds no fields: its ctor func_003506D8 writes only 0xC and the record) is allocated with
// cMemMan_alloc(0x1C, "DeadNode") in func_0034FE28, debouncenode.cpp and wscriptman.cpp; [U] cMoveNode's first own
// field (ModifierBlock, On Tour 0x18) is at 0x1C in cMoveNode_cMoveNode.
CHECK_SIZE(cInstanceNode, 0x1C);

#endif /* OBJECT_INSTANCENODE_H */
