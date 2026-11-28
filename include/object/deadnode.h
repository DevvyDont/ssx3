#ifndef OBJECT_DEADNODE_H
#define OBJECT_DEADNODE_H
#pragma interface

// cDeadNode: the node of a removed (collected, destroyed or expired) instance: it marks the record (flags +0x8: clears
// 0x60 and bit 1, sets bit 2) and sits in the dead bucket (8) as node type 6 (the unsorted type: its SortIndex comes
// from ObjNodeNoSortIndex); expire, loseFocus and signal are empty. Made by cInstanceNode_makeThisDead, makeDead
// (dead-data Type 0), cDebounceNode_expire / loseFocus, cCollectPool_collectInst and cReplay_restoreDeadBucket (the
// dead bucket is saved as a list of instance references, so there is no replay-frame ctor). Adds no members. No EA
// source path in the preview asserts; retail methods: the ctor in object/instancenode (0x003506D8) and the dtor and
// empty virtuals in object/railmodifier (0x00360A28..0x00360A64).
// Size 0x1C: [R] cMemMan_alloc(0x1C, "DeadNode") in cInstanceNode_makeThisDead, makeDead, cDebounceNode_expire /
// loseFocus and cCollectPool_collectInst (wscript/wscriptman), cMemMan_alloc(0x1C, "Dead Object") in
// cReplay_restoreDeadBucket (src/replay/replay.cpp); [O] On Tour 0x18 = its cInstanceNode (no own members).
// Base: cInstanceNode (0x00-0x1C, object/instancenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode), written
// out inline with that header's member names; the vptr is at 0xC.
// Constructor: cDeadNode_cDeadNode (record: cInstanceNode_cInstanceNode1(this, 8, 6, record), vtable, record flags).
// Destructor cDeadNode__cDeadNode__FPv (vtable, cInstanceNode__cInstanceNode). Vtable cDeadNode_vtbl (0x00491B00, 46
// slots: 1 dtor, 34 expire, 35 loseFocus, 36 signal; slot 12 is cInstanceNode_writeToReplayFrame; the rest are
// cInstanceNode's / cSortObjNode's / cTypeObjNode's / cObjNode's).
//
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctor/dtor (src/object/instancenode.cpp, src/object/railmodifier.cpp) and the 0x1C allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 1 paired method (the dtor), 0xC (preview == retail);
//   [U] how retail code uses the field (the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cDeadNode: no members).

#include "layout.h"
#include "object/instancenode.h"

struct cObjNode;
struct tInstanceRecord;

struct cDeadNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cDeadNode_vtbl (0x00491B00) in the ctor and the dtor; [P] 0xC
    // cDeadNode's notes on cInstanceNode's fields:
    //   Next: [U] bucket list (the dead bucket, 8)
    //   ObjNodeType: [R] node type 6 (cInstanceNode_cInstanceNode1)
    //   SortIndex: [R] type 6 draws from ObjNodeNoSortIndex
    //   Instance: [R] the ctor marks the record's flags (+0x8) through it (lw 0x18 twice)

    // Methods bound to their retail symbols by asm labels.
    virtual ~cDeadNode();   // g++'s own symbol _$_9cDeadNode (the retail label renamed)
    virtual void loseFocus() __asm__("cDeadNode_loseFocus__FPv");
    virtual void expire(int) __asm__("cDeadNode_expire__FPv");
    virtual void signal(int, float) __asm__("cDeadNode_signal__FPv");
};

CHECK_SIZE(cDeadNode, 0x1C);
__asm__("_vt$9cDeadNode = cDeadNode_vtbl");   // g++'s name for the retail vtable

#endif /* OBJECT_DEADNODE_H */
