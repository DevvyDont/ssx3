#ifndef OBJECT_FLEXRAILNODE_H
#define OBJECT_FLEXRAILNODE_H

// cFlexRailNode: the abstract scene node of a flexible rail (a chain of up to 8 instances bent along cubic segments):
// the base of cFloatRailNode (node type 20) and cSpringRailNode (type 21), both in object/flexrailnode. It keeps the
// rail state (node count, flags, bound box), one segment matrix per pair of nodes, the rail's node in the world's
// Nash space hash (addSpaceHash / remSpaceHash / updateSpaceHash) and a cSimpleTube for drawing, and answers the
// rider's
// spline queries (splineIntersect). EA's source: d:/ssx/code/source/object/flexrailnode.cpp (preview assert path);
// retail keeps ctors .. writeToReplayFrame in object/flexbridgenode (0x00347D90 .. 0x00348B40), add/remSpaceHash in
// object/flexrailnode and the one-line virtuals in object/railmodifier (0x00361578 .. 0x003615D8).
//
// Size 0x60: [U] both subclasses store their first own fields at 0x60 (cFloatRailNode_cFloatRailNode /
// cSpringRailNode_cSpringRailNode: swc1 0x60, 0x64, 0x68, then their node array at 0x70); the last base field (Tube)
// is at 0x58 and the class is 16-byte aligned (tBoundBox). [O] On Tour's cFlexRailNode is 0x60 with Mats / Node /
// Tube at 0x50 / 0x54 / 0x58 and cFlexRailNodeState (0x30) at 0x20. No allocation of the abstract class itself
// (its vtable D_004900D0 has the pure-virtual stub func_00411530 in slot 50, updateBoundBox).
// Base: cInstanceNode (object/instancenode.h, 0x1C), written out inline with the same member names: the constructors
// call cInstanceNode_cInstanceNode1 (bucket 1) / cInstanceNode_cInstanceNode (replay) directly - there is no cMoveNode
// (no ModifierBlock at 0x1C). Its vptr at 0xC is the only one (_vt$13cFlexRailNode = cFlexRailNode_vtbl D_004900D0,
// 50 slots: 1 dtor, 2 update, 4 render, 12 writeToReplayFrame, 42 updateIntersectInfo, 43 applyForce, 46 isFlexRail,
// 47 updateNodes, 48 updateMats, 49 getMesh, 50 updateBoundBox (pure); the rest are cInstanceNode's).
//
// A FLAT layout: every field at its retail offset, no C++ inheritance or virtuals.
// Tags: [R] retail's constructors / destructor; [P] the July 2003 preview (no accessors; tools/layout_xlate.py sees the
// vptr at 0xC and Mats at 0x50 in the paired destructor / update, preview == retail); [U] how retail code uses the
// field; [O] SSX On Tour's DWARF (cInstanceNode chain, cFlexRailNodeState, cFlexRailNode).

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h" /* tBoundBox */

struct cObjNode;
struct tInstanceRecord;
struct cFlexRailSpaceNode;
struct cSimpleTube;

struct cFlexRailNode {
    // --- cInstanceNode (object/instancenode.h): cObjNode 0x00-0x10, cTypeObjNode 0x10, cSortObjNode 0x14,
    //       cInstanceNode 0x18
    int unk_00;                  // 0x000 never written by any node ctor (as in cInstanceNode)
    cObjNode* Next;              // 0x004 [O] cObjNode::Next; [U] the bucket list's forward link
    cObjNode* Prev;              // 0x008 [O] cObjNode::Prev; [U] the bucket list's back link
    void** mVtbl;                // 0x00C [R] cFlexRailNode_cFlexRailNode / _cFlexRailNode1 / __cFlexRailNode store
                                 //       cFlexRailNode_vtbl (D_004900D0); the subclasses then store cFloatRailNode_vtbl
                                 //       / cSpringRailNode_vtbl; [U] update calls slots 47 and 48, updateSpaceHash slot
                                 //       50
    short ObjNodeType;           // 0x010 [O] cTypeObjNodeState::ObjNodeType; [R] the subclasses pass 20 / 21
    unsigned short Flags;        // 0x012 [O] cTypeObjNodeState::Flags
    unsigned int SortIndex;      // 0x014 [O] cSortObjNodeState::SortIndex
    tInstanceRecord* Instance;   // 0x018 [O] cInstanceNode::Instance; [R] cInstanceNode_cInstanceNode1's record
                                 //       argument; [U] splineIntersect reads it
    // --- cFlexRailNode (cFlexRailNodeState, 0x30, read whole from a replay frame by cFlexRailNode_cFlexRailNode1)
    char unk_01C[0x4];           // 0x01C alignment gap: the state is 16-byte aligned (its tBoundBox); On Tour has it
                                 //       too (its cInstanceNode ends at 0x18, the state still starts at 0x20)
    int NumNodes;                // 0x020 [O] cFlexRailNodeState::NumNodes; [R] the ctor counts the make-data's instance
                                 //       ids that are not -1 (of 8); Mats gets NumNodes - 1 entries, the Tube NumNodes
                                 //       * 8 - 9 segments; [U] render / splineIntersect loop bounds, the subclasses'
                                 //       node arrays
    int RailFlags;               // 0x024 [O] cFlexRailNodeState::RailFlags; [R] the ctor copies the make-data's +0x44;
                                 //       [U] splineIntersect tests it
    char unk_028[0x8];           // 0x028 alignment gap before BoundBox (On Tour state +0x8..+0x10)
    tBoundBox BoundBox;          // 0x030 [O] cFlexRailNodeState::BoundBox; [U] addSpaceHash / remSpaceHash pass it to
                                 //       nash_Nash_findkey; updateSpaceHash copies it (lq 0x30 / 0x40) before slot 50
                                 //       (updateBoundBox) recomputes it and moves the hash node; render culls with it
    t4Matrix* Mats;              // 0x050 [O]; [P] preview dtor 0x50; [R] new[NumNodes - 1] 0x40-byte matrices
                                 //       ("FlexNodeMats"); [U] the destructor frees it; splineIntersect evaluates
                                 //       segment [seg]
    cFlexRailSpaceNode* Node;    // 0x054 [O]; [U] addSpaceHash: cMemMan_alloc(0x10, "FlexRailSpaceNode"), type 3 at
                                 //       +0x8, this at +0xC (On Tour cFlexRailSpaceNode::FlexNode), inserted in Nash
                                 //       list 2; remSpaceHash / updateSpaceHash pass it to nash_Cell_remnode /
                                 //       nash_Nash_movenode
    cSimpleTube* Tube;           // 0x058 [O]; [R] cSimpleTube_cSimpleTube(cMemMan_alloc(0xC, "Tube"), NumNodes * 8 -
                                 //       9); [U] the destructor calls its slot 1 (flags 3)
    char unk_05C[0x4];           // 0x05C tail padding to the 16-byte-aligned size 0x60 (never accessed)
};

CHECK_OFFSET(cFlexRailNode, unk_00, 0x000);
CHECK_OFFSET(cFlexRailNode, Next, 0x004);
CHECK_OFFSET(cFlexRailNode, Prev, 0x008);
CHECK_OFFSET(cFlexRailNode, mVtbl, 0x00C);
CHECK_OFFSET(cFlexRailNode, ObjNodeType, 0x010);
CHECK_OFFSET(cFlexRailNode, Flags, 0x012);
CHECK_OFFSET(cFlexRailNode, SortIndex, 0x014);
CHECK_OFFSET(cFlexRailNode, Instance, 0x018);
CHECK_OFFSET(cFlexRailNode, unk_01C, 0x01C);
CHECK_OFFSET(cFlexRailNode, NumNodes, 0x020);
CHECK_OFFSET(cFlexRailNode, RailFlags, 0x024);
CHECK_OFFSET(cFlexRailNode, unk_028, 0x028);
CHECK_OFFSET(cFlexRailNode, BoundBox, 0x030);
CHECK_OFFSET(cFlexRailNode, Mats, 0x050);
CHECK_OFFSET(cFlexRailNode, Node, 0x054);
CHECK_OFFSET(cFlexRailNode, Tube, 0x058);
CHECK_OFFSET(cFlexRailNode, unk_05C, 0x05C);
CHECK_SIZE(cFlexRailNode, 0x060);

#endif /* OBJECT_FLEXRAILNODE_H */
