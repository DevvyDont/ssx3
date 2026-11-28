#ifndef OBJECT_FLOATRAILNODE_H
#define OBJECT_FLOATRAILNODE_H
#pragma interface

// cFloatRailNode: a flexible rail whose nodes float on linear springs: each node (tFloatRailNode, 0x50: Force, Vel,
// Pos, OPos, InstId, Mesh) is pulled back to its rest position with spring LSpring and damping LDamp; a rider's push is
// added to the two nodes of the hit segment (applyForce). Node type 20. Built by the world script's MakeFloatRailFunc
// (tMakeFloatRailData) and the replay loader; a sibling of cSpringRailNode, both derived from cFlexRailNode
// (object/flexrailnode.h). No EA source path of its own in the preview asserts: its methods sit in
// d:/ssx/code/source/object/flexrailnode.cpp's unit (EA probably declares it in flexrailnode.h). Retail methods: the
// object/flexrailnode splat unit (0x00348D98..0x00349AD0) and getMesh in object/railmodifier.
// Size 0x80: [R] cMemMan_alloc(0x80, "FloatRail") in cReplay_restoreObject (src/replay/replay.cpp); the last member
// (Nodes) is at 0x70 and the class is 16-byte aligned (cFlexRailNode's tBoundBox); [O] On Tour 0x70 (Nodes at 0x6C);
// [P] the preview's paired methods use Nodes at 0x70 too (tools/layout_xlate.py).
// Bases: cFlexRailNode (0x00-0x60, object/flexrailnode.h: cInstanceNode 0x00-0x1C + cFlexRailNodeState at 0x20 + Mats,
// Node, Tube; a real base), then cFloatRailNodeState (On Tour 0xC) at 0x60. Its
// vptr is cInstanceNode's at 0xC.
// Constructors: cFloatRailNode_cFloatRailNode (record, tMakeFloatRailData*: cFlexRailNode_cFlexRailNode(type 20),
// vtable, the state from data +0x48/+0x4C/+0x50, Nodes = new [NumNodes] ("FloatRailNodes", 0x50 each) with InstId/Mesh
// from the data's 8 instance ids (+0x4) and meshes (+0x24), getPosition per node, updateMats, updateBoundBox,
// addSpaceHash) and cFloatRailNode_cFloatRailNode1 (bucket, cBxReplayFrame*: cFlexRailNode_cFlexRailNode1, 0xC bytes of
// state at +0x60, NumNodes * 0x50 bytes of Nodes). Destructor cFloatRailNode__cFloatRailNode (remSpaceHash, frees
// Nodes). Vtable cFloatRailNode_vtbl (50 slots: 1 dtor, 12 writeToReplayFrame, 42 updateIntersectInfo, 43 applyForce,
// 47 updateNodes, 48 updateMats, 49 getMesh, 50 updateBoundBox; 2 update, 4 render, 46 isFlexRail are cFlexRailNode's,
// the rest cInstanceNode's).
//
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/object/flexrailnode.cpp) and the 0x80 allocation;
//   [P] the July 2003 preview via tools/layout_xlate.py: preview == retail at 0xC, 0x20, 0x30-0x48, 0x50, 0x70 (no
//       accessors);
//   [U] how retail code uses the field (src/object/flexrailnode.cpp, railmodifier.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cFlexRailNodeState, cFlexRailNode, cFloatRailNodeState,
//       cFloatRailNode).

#include "layout.h"
#include "wscript/wscriptnull.h"
#include "object/flexrailnode.h"
#include "bx/math3d.h"
#include "world/worldview.h" /* tBoundBox */

struct cObjNode;
struct tInstanceRecord;
struct cFlexRailSpaceNode;
struct cSimpleTube;
struct tFloatRailNode;

// One node of a floating rail (On Tour struct tFloatRailNode, 0x50). [R] the ctors allocate NumNodes x 0x50
// ("FloatRailNodes") and set InstId / Mesh from the make data (views sFlexRailElem_348FA0: InstId at 0x40 = -1 by the
// element ctor); [U] getPosition reads Pos / OPos / InstId / Mesh (view sFrNode9220), updateMats reads Pos (view
// sFrElem_92A8).
struct tFloatRailNode {
    t4Vector Force;                 // 0x00 [O]
    t4Vector Vel;                   // 0x10 [O]
    t4Vector Pos;                   // 0x20 [O]; [U] getPosition, updateMats
    t4Vector OPos;                  // 0x30 [O]; [U] getPosition
    unsigned int InstId;            // 0x40 [O] tResourceID; [R] the element ctor sets -1
    int Mesh;                       // 0x44 [O]; [U] getMesh returns Nodes[seg].Mesh
    char unk_048[0x8];              // 0x48 padding (16-byte aligned)
};

CHECK_OFFSET(tFloatRailNode, Force, 0x00);
CHECK_OFFSET(tFloatRailNode, Vel, 0x10);
CHECK_OFFSET(tFloatRailNode, Pos, 0x20);
CHECK_OFFSET(tFloatRailNode, OPos, 0x30);
CHECK_OFFSET(tFloatRailNode, InstId, 0x40);
CHECK_OFFSET(tFloatRailNode, Mesh, 0x44);
CHECK_SIZE(tFloatRailNode, 0x50);

// World-script data that makes a floating rail (On Tour tMakeFloatRailData, 0x58; same layout). [O] On Tour's
// cFloatRailNode(tInstanceRecord* pInst, const tMakeFloatRailData* data); [U] cFloatRailNode_cFloatRailNode
// (src/object/flexrailnode.cpp) reads the 8 instance ids and the 8 meshes and stores LSpring / LDamp / ForceScale into
// the members of the same names; the cFlexRailNode base ctor (src/object/flexbridgenode.cpp, view sFlexDef_347D90)
// counts the ids that are not -1 and stores Flags as RailFlags.
struct tMakeFloatRailData {
    unsigned int InstId;            // 0x00 [O] tResourceID; [U] MakeFloatRailFunc's instance lookup (-1: the script's
                                    //      current instance)
    unsigned int InstId1;           // 0x04 [O] tResourceID; [U] the rail nodes' instance ids, read as an array of 8
    unsigned int InstId2;           // 0x08 [O]
    unsigned int InstId3;           // 0x0C [O]
    unsigned int InstId4;           // 0x10 [O]
    unsigned int InstId5;           // 0x14 [O]
    unsigned int InstId6;           // 0x18 [O]
    unsigned int InstId7;           // 0x1C [O]
    unsigned int InstId8;           // 0x20 [O]
    int Mesh1;                      // 0x24 [O]; [U] the nodes' mesh numbers, read as an array of 8
    int Mesh2;                      // 0x28 [O]
    int Mesh3;                      // 0x2C [O]
    int Mesh4;                      // 0x30 [O]
    int Mesh5;                      // 0x34 [O]
    int Mesh6;                      // 0x38 [O]
    int Mesh7;                      // 0x3C [O]
    int Mesh8;                      // 0x40 [O]
    int Flags;                      // 0x44 [O]; [U] cFlexRailNode_cFlexRailNode: RailFlags = Flags
    float LSpring;                  // 0x48 [O]; [U] -> LSpring
    float LDamp;                    // 0x4C [O]; [U] -> LDamp
    float ForceScale;               // 0x50 [O]; [U] -> ForceScale
    WScriptNullType null;           // 0x54 [O]
    char unk_055[0x3];              // 0x55 padding
};

CHECK_OFFSET(tMakeFloatRailData, InstId, 0x00);
CHECK_OFFSET(tMakeFloatRailData, InstId1, 0x04);
CHECK_OFFSET(tMakeFloatRailData, InstId8, 0x20);
CHECK_OFFSET(tMakeFloatRailData, Mesh1, 0x24);
CHECK_OFFSET(tMakeFloatRailData, Mesh8, 0x40);
CHECK_OFFSET(tMakeFloatRailData, Flags, 0x44);
CHECK_OFFSET(tMakeFloatRailData, LSpring, 0x48);
CHECK_OFFSET(tMakeFloatRailData, LDamp, 0x4C);
CHECK_OFFSET(tMakeFloatRailData, ForceScale, 0x50);
CHECK_OFFSET(tMakeFloatRailData, null, 0x54);
CHECK_SIZE(tMakeFloatRailData, 0x58);

struct cBxReplayFrame;

struct cFloatRailNode : public cFlexRailNode {
    // cFlexRailNode (object/flexrailnode.h, 0x60): the vptr at 0x0C:
    //   [R] cFloatRailNode_vtbl in both ctors and the dtor (after cFlexRailNode's own); [P]
    // cFloatRailNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] node type 20 (passed to cFlexRailNode_cFlexRailNode)
    // cFloatRailNode's notes on cFlexRailNode's fields:
    //   NumNodes: [U] the ctors size Nodes by it, every update/getMesh/updateBoundBox loop (lw 0x20)
    //   BoundBox: [U] updateBoundBox recomputes it from the node positions (lwc1/swc1 0x30-0x38,
    //       0x40-0x48; lq/sq 0x30/0x40)
    //   Mats: [U] updateMats fills one matrix per segment
    // --- cFloatRailNodeState (On Tour 0xC; [R] the replay ctor reads / writeToReplayFrame writes 0xC bytes at +0x60)
    float LSpring;                  // 0x060 [O]; [R] ctor: data +0x48 (swc1); [U] updateNodes (lwc1)
    float LDamp;                    // 0x064 [O]; [R] ctor: data +0x4C (swc1); [U] updateNodes (lwc1)
    float ForceScale;               // 0x068 [O]; [R] ctor: data +0x50 (swc1); no retail code reads it (applyForce uses
                                    //      the hit's segment weight only)
    char unk_06C[0x4];              // 0x06C never accessed: On Tour puts Nodes here (0x6C); retail and the preview have
                                    //      it at 0x70
    // --- cFloatRailNode
    tFloatRailNode* Nodes;          // 0x070 [O]; [P]; [R] ctors: new tFloatRailNode[NumNodes] (0x50 each,
                                    //      "FloatRailNodes"), dtor frees it; [U] every method walks it (lw 0x70);
                                    //      getMesh returns Nodes[seg].Mesh
    char unk_074[0xC];              // 0x074 tail padding to the 16-byte-aligned size 0x80 (never accessed)

    // Methods bound to their retail symbols by asm labels.
    void* cFloatRailNode1(void* bucket, cBxReplayFrame* stream) __asm__("cFloatRailNode_cFloatRailNode1");
    void getPosition(tFloatRailNode* node) __asm__("cFloatRailNode_getPosition");
    virtual ~cFloatRailNode();   // g++'s own symbol _$_14cFloatRailNode (the retail label renamed)
    virtual void updateBoundBox() __asm__("cFloatRailNode_updateBoundBox");
    virtual void updateMats() __asm__("cFloatRailNode_updateMats");
    virtual void updateNodes() __asm__("cFloatRailNode_updateNodes");
    virtual int getMesh(int segIndex) __asm__("cFloatRailNode_getMesh");
    virtual void applyForce(const tIntersectInfo& node, const t4Vector& offset) __asm__("cFloatRailNode_applyForce");
    virtual void updateIntersectInfo(tIntersectInfo& node) __asm__("cFloatRailNode_updateIntersectInfo");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cFloatRailNode_writeToReplayFrame");
};

CHECK_OFFSET(cFloatRailNode, LSpring, 0x060);
CHECK_OFFSET(cFloatRailNode, LDamp, 0x064);
CHECK_OFFSET(cFloatRailNode, ForceScale, 0x068);
CHECK_OFFSET(cFloatRailNode, unk_06C, 0x06C);
CHECK_OFFSET(cFloatRailNode, Nodes, 0x070);
CHECK_OFFSET(cFloatRailNode, unk_074, 0x074);
CHECK_SIZE(cFloatRailNode, 0x080);
__asm__("_vt$14cFloatRailNode = cFloatRailNode_vtbl");   // g++'s name for the retail vtable

#endif /* OBJECT_FLOATRAILNODE_H */
