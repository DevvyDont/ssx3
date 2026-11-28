#ifndef OBJECT_OBJECTNODE_H
#define OBJECT_OBJECTNODE_H
#pragma interface

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
// Derives from cMoveNode (object/movenode.h); evidence tags as there:
// [P] preview inline accessors; [R] ctors/dtor/allocation; [U] retail use; [O] On Tour.

#include "layout.h"
#include "object/movenode.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;

struct cObjectNode : public cMoveNode {
    // cMoveNode (object/movenode.h, 0x2C): the vptr at 0x0C:
    //   [R] both ctors and the dtor store cObjectNode_vtbl; [P] the preview's ~cObjectNode stores it at 0xC too
    //       (tools/layout_xlate.py)
    // cObjectNode's notes on cMoveNode's fields:
    //   ObjNodeType: [R] cObjectNode_cObjectNode passes 17 to cMoveNode_cMoveNode1
    //   Instance: [U] cObjectNode_cObjectNode updates the record's flags word (+0x8) through it (view sMoveFlags_6DB0)

    // Methods bound to their retail symbols by asm labels.
    virtual ~cObjectNode();   // g++'s own symbol _$_11cObjectNode (the retail label renamed)
    void* cObjectNode1() __asm__("cObjectNode_cObjectNode1");
    // writeToReplayFrame stays a C function (cObjectNode_writeToReplayFrame): declared first, it is the vtable's key
    // function, which keeps the vtable out of the units defining the other overrides
    virtual void writeToReplayFrame(cBxReplayFrame*) __asm__("cObjectNode_writeToReplayFrame__FPv");
};

CHECK_SIZE(cObjectNode, 0x2C);
__asm__("_vt$11cObjectNode = cObjectNode_vtbl");   // g++'s name for the retail vtable

#endif /* OBJECT_OBJECTNODE_H */
