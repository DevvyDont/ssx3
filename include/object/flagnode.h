#ifndef OBJECT_FLAGNODE_H
#define OBJECT_FLAGNODE_H
#pragma interface

// cFlagNode: the node of a cloth flag instance. It owns no flag state: the ctor marks the record (flags +0x8: clears
// bit 1, sets bit 2), builds a tFlagState from the world script's tMakeFlagData on the stack and registers the record
// with the flag manager (cFlagManager_AddFlag(cObjectInterface_getFlagManager(), record, &state)); the dtor removes it
// again (cFlagManager_RemoveFlag(manager, Instance)). render is empty (the flag manager draws the cloth). Node type 10.
// Made by MakeFlagFunc (seg/seg_1FBE38) and the replay loader. Adds no members. EA's source:
// d:/ssx/code/source/object/flagnode.cpp (preview assert path; it may also hold cFlagManager). Retail methods: the
// object/flexrailnode splat unit (0x0034ADD8..0x0034AF1C).
// Size 0x1C: [R] cMemMan_alloc(0x1C, "Flag") in cReplay_restoreObject (src/replay/replay.cpp) and cMemMan_alloc(0x1C)
// in MakeFlagFunc; [O] On Tour 0x18 = its cInstanceNode (no own members; its cObjNode is 4 bytes smaller).
// Base: cInstanceNode (0x00-0x1C, object/instancenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode), written
// out inline with that header's member names; the vptr is at 0xC.
// Constructors: cFlagNode_cFlagNode1 (bucket, record, tMakeFlagData*: cInstanceNode_cInstanceNode1(this, bucket, 10,
// record), vtable, record flags, tFlagState_tFlagState, cFlagManager_AddFlag) and cFlagNode_cFlagNode (bucket,
// cBxReplayFrame*: cInstanceNode_cInstanceNode, vtable). Destructor cFlagNode__cFlagNode (vtable,
// cFlagManager_RemoveFlag, cInstanceNode__cInstanceNode). Vtable cFlagNode_vtbl (0x0048FC10, 46 slots: 1 dtor, 4
// render, 12 writeToReplayFrame; the rest are cInstanceNode's / cSortObjNode's / cTypeObjNode's / cObjNode's).
//
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/object/flexrailnode.cpp) and the 0x1C allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 3 paired methods, 0xC and 0x18 (preview == retail);
//   [U] how retail code uses the field (the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cFlagNode: no members).

#include "layout.h"
#include "object/instancenode.h"

struct cObjNode;
struct tInstanceRecord;

struct cFlagNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cFlagNode_vtbl (0x0048FC10) in both ctors and the dtor; [P] 0xC
    // cFlagNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] node type 10 (cInstanceNode_cInstanceNode1)
    //   Instance: [R] the dtor passes it to cFlagManager_RemoveFlag (lw 0x18)

    // Methods bound to their retail symbols by asm labels.
    void* cFlagNode1(void* bucket, tInstanceRecord* node, void* src) __asm__("cFlagNode_cFlagNode1");
    virtual ~cFlagNode();   // g++'s own symbol _$_9cFlagNode (the retail label renamed)
    virtual void render() __asm__("cFlagNode_render__FPv");
    virtual void writeToReplayFrame(cBxReplayFrame*) __asm__("cFlagNode_writeToReplayFrame__FPv");
};

CHECK_SIZE(cFlagNode, 0x1C);
__asm__("_vt$9cFlagNode = cFlagNode_vtbl");   // g++'s name for the retail vtable

#endif /* OBJECT_FLAGNODE_H */
