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
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/object/deadfadenode.cpp, src/object/railmodifier.cpp) and the 0x1C allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 3 paired methods, only 0xC observed (preview == retail);
//   [U] how retail code uses the field (the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cRestoreNode: no members).

#include "layout.h"
#include "object/instancenode.h"

struct cObjNode;
struct tInstanceRecord;

struct cRestoreNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cRestoreNode_vtbl (0x00491680) in both ctors and the dtor; [P] 0xC
    // cRestoreNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] node type 19 (cInstanceNode_cInstanceNode1)
    //   Instance: [R] the ctor marks the record's flags (+0x8) through it (lw 0x18 twice)

    // Methods bound to their retail symbols by asm labels.
    void* cRestoreNode1() __asm__("cRestoreNode_cRestoreNode1");
    // expire stays a C function (cRestoreNode_expire): declared first, it is the vtable's key function, which keeps the
    // vtable out of the units defining the other overrides
    virtual void expire(int) __asm__("cRestoreNode_expire__FPv");
    virtual void signal(int, float) __asm__("cRestoreNode_signal__FPv");
};

CHECK_SIZE(cRestoreNode, 0x1C);

#endif /* OBJECT_RESTORENODE_H */
