#ifndef OBJECT_SPRINGRAILNODE_H
#define OBJECT_SPRINGRAILNODE_H

// cSpringRailNode: a flexible rail whose nodes swing on angular springs: each node (tSpringRailNode, 0x60: P, V, O, W,
// T, InstId, Mesh) is twisted back with spring ASpring and damping ADamp; a rider's push torques
// the two nodes of the hit segment (applyForce). Node type 21. Built by the world script's MakeSpringRailFunc
// (tMakeSpringRailData) and the replay loader; a sibling of cFloatRailNode, both derived from cFlexRailNode
// (object/flexrailnode.h). No EA source path of its own in the preview asserts: its methods sit in
// d:/ssx/code/source/object/flexrailnode.cpp's unit (EA probably declares it in flexrailnode.h). Retail methods: the
// object/flexrailnode splat unit (0x00349B48..0x0034AC10) and getMesh in object/railmodifier.
// Size 0x80: [R] cMemMan_alloc(0x80, "SpringRail") in cReplay_restoreObject (src/replay/replay.cpp); the last member
// (Nodes) is at 0x70 and the class is 16-byte aligned (cFlexRailNode's tBoundBox); [O] On Tour 0x70 (Nodes at 0x6C);
// [P] the preview's paired methods use Nodes at 0x70 too (tools/layout_xlate.py).
// Bases: cFlexRailNode (0x00-0x60, object/flexrailnode.h: cInstanceNode 0x00-0x1C + cFlexRailNodeState at 0x20 + Mats,
// Node, Tube), written out inline with that header's member names; then cSpringRailNodeState (On Tour 0xC) at 0x60. Its
// vptr is cInstanceNode's at 0xC.
// Constructors: cSpringRailNode_cSpringRailNode (record, tMakeSpringRailData*: cFlexRailNode_cFlexRailNode(type 21),
// vtable, the state from data +0x48/+0x4C/+0x50, Nodes = new [NumNodes] ("SpringRailNodes", 0x60 each) with InstId/Mesh
// from the data's 8 instance ids (+0x4) and meshes (+0x24), getPosition per node, updateMats, updateBoundBox,
// addSpaceHash) and cSpringRailNode_cSpringRailNode1 (bucket, cBxReplayFrame*: cFlexRailNode_cFlexRailNode1, 0xC bytes
// of state at +0x60, NumNodes * 0x60 bytes of Nodes). Destructor cSpringRailNode__cSpringRailNode (remSpaceHash, frees
// Nodes). Vtable cSpringRailNode_vtbl (50 slots: 1 dtor, 12 writeToReplayFrame, 42 updateIntersectInfo, 43 applyForce,
// 47 updateNodes, 48 updateMats, 49 getMesh, 50 updateBoundBox; 2 update, 4 render, 46 isFlexRail are cFlexRailNode's,
// the rest cInstanceNode's).
//
// A FLAT layout like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors/dtor (src/object/flexrailnode.cpp) and the 0x80 allocation;
//   [P] the July 2003 preview via tools/layout_xlate.py: preview == retail at 0xC, 0x20, 0x30-0x48, 0x50, 0x70 (no
//       accessors);
//   [U] how retail code uses the field (src/object/flexrailnode.cpp, railmodifier.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cFlexRailNodeState, cFlexRailNode, cSpringRailNodeState,
//       cSpringRailNode).

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h" /* tBoundBox */

struct cObjNode;
struct tInstanceRecord;
struct cFlexRailSpaceNode;
struct cSimpleTube;
struct tSpringRailNode;

struct cSpringRailNode {
    // --- cFlexRailNode (0x60; object/flexrailnode.h): cInstanceNode 0x00-0x1C (object/instancenode.h)
    int unk_00;                     // 0x000 cObjNode's leading word; no ctor writes it (see object/objnode.h)
    cObjNode* Next;                 // 0x004 [O] cObjNode::Next; [U] bucket list
    cObjNode* Prev;                 // 0x008 [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x00C [R] cSpringRailNode_vtbl in both ctors and the dtor (after cFlexRailNode's
                                    //      own); [P]
    short ObjNodeType;              // 0x010 [O] cTypeObjNodeState; [R] node type 21 (passed to
                                    //      cFlexRailNode_cFlexRailNode)
    unsigned short Flags;           // 0x012 [O] cTypeObjNodeState
    unsigned int SortIndex;         // 0x014 [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x018 [O] cInstanceNode::Instance
    // --- cFlexRailNodeState (On Tour 0x30, 16-byte aligned; read whole by cFlexRailNode_cFlexRailNode1)
    char unk_01C[0x4];              // 0x01C alignment gap before the 16-byte-aligned state (never accessed)
    int NumNodes;                   // 0x020 [O] cFlexRailNodeState::NumNodes; [P]; [U] the ctors size Nodes by it,
                                    //      every update/getMesh/updateBoundBox loop (lw 0x20)
    int RailFlags;                  // 0x024 [O] cFlexRailNodeState::RailFlags
    char unk_028[0x8];              // 0x028 alignment gap before BoundBox (never accessed)
    tBoundBox BoundBox;             // 0x030 [O] cFlexRailNodeState::BoundBox; [P]; [U] updateBoundBox recomputes it
                                    //      from the node positions (lwc1/swc1 0x30-0x38, 0x40-0x48; lq/sq 0x30/0x40)
    // --- cFlexRailNode
    t4Matrix* Mats;                 // 0x050 [O] cFlexRailNode::Mats; [P]; [U] updateMats fills one matrix per segment
    cFlexRailSpaceNode* Node;       // 0x054 [O] cFlexRailNode::Node (Nash space-hash node; addSpaceHash/remSpaceHash)
    cSimpleTube* Tube;              // 0x058 [O] cFlexRailNode::Tube
    char unk_05C[0x4];              // 0x05C cFlexRailNode's tail padding (16-byte-aligned size 0x60; never accessed)
    // --- cSpringRailNodeState (On Tour 0xC; [R] the replay ctor reads / writeToReplayFrame writes 0xC bytes at +0x60)
    float ASpring;                  // 0x060 [O]; [R] ctor: data +0x48 (swc1); [U] updateNodes (lwc1)
    float ADamp;                    // 0x064 [O]; [R] ctor: data +0x4C (swc1); [U] updateNodes (lwc1)
    float TorqueScale;              // 0x068 [O]; [R] ctor: data +0x50 (swc1); no retail code reads it (applyForce uses
                                    //      the hit's segment weight only)
    char unk_06C[0x4];              // 0x06C never accessed: On Tour puts Nodes here (0x6C); retail and the preview have
                                    //      it at 0x70
    // --- cSpringRailNode
    tSpringRailNode* Nodes;         // 0x070 [O]; [P]; [R] ctors: new tSpringRailNode[NumNodes] (0x60 each,
                                    //      "SpringRailNodes"), dtor frees it; [U] every method walks it (lw 0x70);
                                    //      getMesh returns Nodes[seg].Mesh
    char unk_074[0xC];              // 0x074 tail padding to the 16-byte-aligned size 0x80 (never accessed)
};

CHECK_OFFSET(cSpringRailNode, unk_00, 0x000);
CHECK_OFFSET(cSpringRailNode, Next, 0x004);
CHECK_OFFSET(cSpringRailNode, Prev, 0x008);
CHECK_OFFSET(cSpringRailNode, mVtbl, 0x00C);
CHECK_OFFSET(cSpringRailNode, ObjNodeType, 0x010);
CHECK_OFFSET(cSpringRailNode, Flags, 0x012);
CHECK_OFFSET(cSpringRailNode, SortIndex, 0x014);
CHECK_OFFSET(cSpringRailNode, Instance, 0x018);
CHECK_OFFSET(cSpringRailNode, unk_01C, 0x01C);
CHECK_OFFSET(cSpringRailNode, NumNodes, 0x020);
CHECK_OFFSET(cSpringRailNode, RailFlags, 0x024);
CHECK_OFFSET(cSpringRailNode, unk_028, 0x028);
CHECK_OFFSET(cSpringRailNode, BoundBox, 0x030);
CHECK_OFFSET(cSpringRailNode, Mats, 0x050);
CHECK_OFFSET(cSpringRailNode, Node, 0x054);
CHECK_OFFSET(cSpringRailNode, Tube, 0x058);
CHECK_OFFSET(cSpringRailNode, unk_05C, 0x05C);
CHECK_OFFSET(cSpringRailNode, ASpring, 0x060);
CHECK_OFFSET(cSpringRailNode, ADamp, 0x064);
CHECK_OFFSET(cSpringRailNode, TorqueScale, 0x068);
CHECK_OFFSET(cSpringRailNode, unk_06C, 0x06C);
CHECK_OFFSET(cSpringRailNode, Nodes, 0x070);
CHECK_OFFSET(cSpringRailNode, unk_074, 0x074);
CHECK_SIZE(cSpringRailNode, 0x080);

#endif /* OBJECT_SPRINGRAILNODE_H */
