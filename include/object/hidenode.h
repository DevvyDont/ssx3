#ifndef OBJECT_HIDENODE_H
#define OBJECT_HIDENODE_H

// cHideNode: the node the world script's HideInstanceFunc puts on an instance to hide it: it only marks the record
// (flags +0x8: clears 0x60 and bit 1, sets bit 2) and ignores everything else (expire, loseFocus, signal and
// onCollide are empty). Node type 16, bucket 1. Adds no members. No EA source path in the preview asserts; retail
// methods: the ctors and writeToReplayFrame in object/deadfadenode (0x00350E90..0x00350F5C) and the dtor and empty
// virtuals in object/railmodifier (0x00360A70..0x00360AB4).
// Size 0x1C: [R] cMemMan_alloc(0x1C, "Hide") in cReplay_restoreObject (src/replay/replay.cpp) and cMemMan_alloc(0x1C)
// in HideInstanceFunc (seg/seg_1FBE38); [O] On Tour 0x18 = its cInstanceNode (no own members; its cObjNode is 4 bytes
// smaller).
// Base: cInstanceNode (0x00-0x1C, object/instancenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode), written
// out inline with that header's member names; the vptr is at 0xC.
// Constructors: cHideNode_cHideNode (record: cInstanceNode_cInstanceNode1(this, 1, 16, record), vtable, record flags)
// and cHideNode_cHideNode1 (bucket, cBxReplayFrame*: cInstanceNode_cInstanceNode, vtable). Destructor
// cHideNode__cHideNode__FPv (vtable, cInstanceNode__cInstanceNode). Vtable cHideNode_vtbl (0x00491800, 46 slots: 1
// dtor, 12 writeToReplayFrame, 34 expire, 35 loseFocus, 36 signal, 40 onCollide; the rest are cInstanceNode's /
// cSortObjNode's / cTypeObjNode's / cObjNode's).
//
// A FLAT layout like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors/dtor (src/object/deadfadenode.cpp, src/object/railmodifier.cpp) and the 0x1C allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 3 paired methods, only 0xC observed (preview == retail);
//   [U] how retail code uses the field (the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cHideNode: no members).

#include "layout.h"

struct cObjNode;
struct tInstanceRecord;

struct cHideNode {
    // --- cInstanceNode (0x1C; object/instancenode.h): cObjNode / cTypeObjNode / cSortObjNode first
    int unk_00;                     // 0x00 cObjNode's leading word; no ctor writes it (see object/objnode.h)
    cObjNode* Next;                 // 0x04 [O] cObjNode::Next; [U] bucket list
    cObjNode* Prev;                 // 0x08 [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x0C [R] cHideNode_vtbl (0x00491800) in both ctors and the dtor; [P] 0xC
    short ObjNodeType;              // 0x10 [O] cTypeObjNodeState; [R] node type 16 (cInstanceNode_cInstanceNode1)
    unsigned short Flags;           // 0x12 [O] cTypeObjNodeState
    unsigned int SortIndex;         // 0x14 [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x18 [O] cInstanceNode::Instance; [R] the ctor marks the record's flags (+0x8)
                                    //      through it (lw 0x18 twice)
};

CHECK_OFFSET(cHideNode, unk_00, 0x00);
CHECK_OFFSET(cHideNode, Next, 0x04);
CHECK_OFFSET(cHideNode, Prev, 0x08);
CHECK_OFFSET(cHideNode, mVtbl, 0x0C);
CHECK_OFFSET(cHideNode, ObjNodeType, 0x10);
CHECK_OFFSET(cHideNode, Flags, 0x12);
CHECK_OFFSET(cHideNode, SortIndex, 0x14);
CHECK_OFFSET(cHideNode, Instance, 0x18);
CHECK_SIZE(cHideNode, 0x1C);

#endif /* OBJECT_HIDENODE_H */
