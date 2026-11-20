#ifndef OBJECT_COUNTERNODE_H
#define OBJECT_COUNTERNODE_H

// cCounterNode: a world-script counter on an instance: it starts at StartCount and counts down on signals - 502 always,
// 500 once per level 1..10 (Counted keeps a bit per level seen), 501 only in order (level 1, or a level whose
// predecessor bit is set); when it reaches 0, update sets it to -1 and expires the node (slot 34, which runs the
// instance's expire script). Node type 7. Made by MakeCounterFunc (seg/seg_1FBE38) and the replay loader. No EA source
// path in the preview asserts; retail methods: object/debouncenode (0x00343768..0x00343A64) and the dtor in
// object/railmodifier (0x00361530).
// Size 0x24: [R] cMemMan_alloc(0x24, "Counter") in cReplay_restoreObject (src/replay/replay.cpp) and cMemMan_alloc(0x24)
// in MakeCounterFunc; the replay ctor reads the 8-byte cCounterNodeState into +0x1C (and writeToReplayFrame writes
// it); [O] On Tour 0x20 = cInstanceNode 0x18 + cCounterNodeState 0x8 (state after the base, as here).
// Bases: cInstanceNode (0x00-0x1C, object/instancenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode), then
// cCounterNodeState (On Tour 0x8) at 0x1C; both written out inline. The vptr is at 0xC.
// Constructors: cCounterNode_cCounterNode1 (bucket, record, tMakeCounterData*: cInstanceNode_cInstanceNode1(this,
// bucket, 7, record), vtable, Count = data StartCount, Counted = 0) and cCounterNode_cCounterNode (bucket,
// cBxReplayFrame*: cInstanceNode_cInstanceNode, vtable, the 8-byte state). Destructor cCounterNode__cCounterNode__FPv
// (vtable, cInstanceNode__cInstanceNode). Vtable cCounterNode_vtbl (0x004903F0, 46 slots: 1 dtor, 2 update, 12
// writeToReplayFrame, 36 signal; the rest are cInstanceNode's / cSortObjNode's / cTypeObjNode's / cObjNode's).
//
// A FLAT layout like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors/dtor (src/object/debouncenode.cpp, src/object/railmodifier.cpp) and the 0x24 allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 3 paired methods, 0xC / 0x1C / 0x20 (preview == retail);
//   [U] how retail code uses the field (src/object/debouncenode.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cCounterNodeState { int Count; int Counted; }, tMakeCounterData).

#include "layout.h"

struct cObjNode;
struct tInstanceRecord;

struct cCounterNode {
    // --- cInstanceNode (0x1C; object/instancenode.h): cObjNode / cTypeObjNode / cSortObjNode first
    int unk_00;                     // 0x00 cObjNode's leading word; no ctor writes it (see object/objnode.h)
    cObjNode* Next;                 // 0x04 [O] cObjNode::Next; [U] bucket list
    cObjNode* Prev;                 // 0x08 [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x0C [R] cCounterNode_vtbl (0x004903F0) in both ctors and the dtor; [P] 0xC; [U]
                                    //      update calls slot 34 (expire)
    short ObjNodeType;              // 0x10 [O] cTypeObjNodeState; [R] node type 7 (cInstanceNode_cInstanceNode1)
    unsigned short Flags;           // 0x12 [O] cTypeObjNodeState
    unsigned int SortIndex;         // 0x14 [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x18 [O] cInstanceNode::Instance; [R] cInstanceNode_cInstanceNode1
    // --- cCounterNodeState (On Tour 0x8; [R] the replay ctor reads 8 bytes here, writeToReplayFrame writes them)
    int Count;                      // 0x1C [O]; [P] preview 0x1C; [R] ctor1 = data StartCount (lh data +0x4, sw); [U]
                                    //      update: 0 -> -1 and expire; signal decrements it while > 0 (lw/sw)
    int Counted;                    // 0x20 [O]; [P] preview 0x20; [R] ctor1 0 (sw $0); [U] signal 500/501 test and set
                                    //      one bit per level 1..10 (lw/sw)
};

CHECK_OFFSET(cCounterNode, unk_00, 0x00);
CHECK_OFFSET(cCounterNode, Next, 0x04);
CHECK_OFFSET(cCounterNode, Prev, 0x08);
CHECK_OFFSET(cCounterNode, mVtbl, 0x0C);
CHECK_OFFSET(cCounterNode, ObjNodeType, 0x10);
CHECK_OFFSET(cCounterNode, Flags, 0x12);
CHECK_OFFSET(cCounterNode, SortIndex, 0x14);
CHECK_OFFSET(cCounterNode, Instance, 0x18);
CHECK_OFFSET(cCounterNode, Count, 0x1C);
CHECK_OFFSET(cCounterNode, Counted, 0x20);
CHECK_SIZE(cCounterNode, 0x24);

#endif /* OBJECT_COUNTERNODE_H */
