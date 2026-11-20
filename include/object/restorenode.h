#ifndef OBJECT_RESTORENODE_H
#define OBJECT_RESTORENODE_H

// cRestoreNode: the node that brings a removed instance back: it marks the record (flags +0x8: clears 0x60 and bit 1,
// sets bit 2) and otherwise does nothing (expire and signal are empty). Made by makeDead (dead-data Type 3, from the
// world script and cDeadFadeNode), by cDebounceNode_expire (Type 3) and by the replay loader. Node type 19, bucket 1.
// Adds no members. No EA source path in the preview asserts; retail methods: the ctors and writeToReplayFrame in
// object/deadfadenode (0x00350F60..0x0035102C) and the dtor and empty virtuals in object/railmodifier
// (0x00360AB8..0x00360AEC).
// Size 0x1C: [R] cMemMan_alloc(0x1C, "Restore") in cReplay_restoreObject (src/replay/replay.cpp) and
// cMemMan_alloc(0x1C, "RestoreNode") in makeDead (seg/seg_1FBE38) and cDebounceNode_expire; [O] On Tour 0x18 = its
// cInstanceNode (no own members; its cObjNode is 4 bytes smaller).
// Base: cInstanceNode (0x00-0x1C, object/instancenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode), written
// out inline with that header's member names; the vptr is at 0xC.
// Constructors: cRestoreNode_cRestoreNode (record: cInstanceNode_cInstanceNode1(this, 1, 19, record), vtable, record
// flags) and cRestoreNode_cRestoreNode1 (bucket, cBxReplayFrame*: cInstanceNode_cInstanceNode, vtable). Destructor
// cRestoreNode__cRestoreNode__FPv (vtable, cInstanceNode__cInstanceNode). Vtable cRestoreNode_vtbl (0x00491680, 46
// slots: 1 dtor, 12 writeToReplayFrame, 34 expire, 36 signal; the rest are cInstanceNode's / cSortObjNode's /
// cTypeObjNode's / cObjNode's).
//
// A FLAT layout like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors/dtor (src/object/deadfadenode.cpp, src/object/railmodifier.cpp) and the 0x1C allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 3 paired methods, only 0xC observed (preview == retail);
//   [U] how retail code uses the field (the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cRestoreNode: no members).

#include "layout.h"

struct cObjNode;
struct tInstanceRecord;

struct cRestoreNode {
    // --- cInstanceNode (0x1C; object/instancenode.h): cObjNode / cTypeObjNode / cSortObjNode first
    int unk_00;                     // 0x00 cObjNode's leading word; no ctor writes it (see object/objnode.h)
    cObjNode* Next;                 // 0x04 [O] cObjNode::Next; [U] bucket list
    cObjNode* Prev;                 // 0x08 [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x0C [R] cRestoreNode_vtbl (0x00491680) in both ctors and the dtor; [P] 0xC
    short ObjNodeType;              // 0x10 [O] cTypeObjNodeState; [R] node type 19 (cInstanceNode_cInstanceNode1)
    unsigned short Flags;           // 0x12 [O] cTypeObjNodeState
    unsigned int SortIndex;         // 0x14 [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x18 [O] cInstanceNode::Instance; [R] the ctor marks the record's flags (+0x8)
                                    //      through it (lw 0x18 twice)
};

CHECK_OFFSET(cRestoreNode, unk_00, 0x00);
CHECK_OFFSET(cRestoreNode, Next, 0x04);
CHECK_OFFSET(cRestoreNode, Prev, 0x08);
CHECK_OFFSET(cRestoreNode, mVtbl, 0x0C);
CHECK_OFFSET(cRestoreNode, ObjNodeType, 0x10);
CHECK_OFFSET(cRestoreNode, Flags, 0x12);
CHECK_OFFSET(cRestoreNode, SortIndex, 0x14);
CHECK_OFFSET(cRestoreNode, Instance, 0x18);
CHECK_SIZE(cRestoreNode, 0x1C);

#endif /* OBJECT_RESTORENODE_H */
