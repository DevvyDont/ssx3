#ifndef OBJECT_HIDENODE_H
#define OBJECT_HIDENODE_H
#pragma interface

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
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/object/deadfadenode.cpp, src/object/railmodifier.cpp) and the 0x1C allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 3 paired methods, only 0xC observed (preview == retail);
//   [U] how retail code uses the field (the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cHideNode: no members).

#include "layout.h"
#include "object/instancenode.h"

struct cObjNode;
struct tInstanceRecord;

struct cHideNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cHideNode_vtbl (0x00491800) in both ctors and the dtor; [P] 0xC
    // cHideNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] node type 16 (cInstanceNode_cInstanceNode1)
    //   Instance: [R] the ctor marks the record's flags (+0x8) through it (lw 0x18 twice)

    // Methods bound to their retail symbols by asm labels.
    virtual ~cHideNode();   // g++'s own symbol _$_9cHideNode (the retail label renamed)
    void* cHideNode1() __asm__("cHideNode_cHideNode1");
    virtual void loseFocus() __asm__("cHideNode_loseFocus__FPv");
    virtual void expire(int) __asm__("cHideNode_expire__FPv");
    virtual void signal(int, float) __asm__("cHideNode_signal__FPv");
    virtual void onCollide(const tCollideInfo*, const tIntersectInfo*, cBeing*) __asm__("cHideNode_onCollide__FPv");
    virtual void writeToReplayFrame(cBxReplayFrame*) __asm__("cHideNode_writeToReplayFrame__FPv");
};

CHECK_SIZE(cHideNode, 0x1C);
__asm__("_vt$9cHideNode = cHideNode_vtbl");   // g++'s name for the retail vtable

#endif /* OBJECT_HIDENODE_H */
