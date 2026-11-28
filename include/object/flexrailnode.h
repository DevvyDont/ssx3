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
// Base: cInstanceNode (object/instancenode.h, 0x1C): the constructors
// call cInstanceNode_cInstanceNode1 (bucket 1) / cInstanceNode_cInstanceNode (replay) directly - there is no cMoveNode
// (no ModifierBlock at 0x1C). Its vptr at 0xC is the only one (_vt$13cFlexRailNode = cFlexRailNode_vtbl D_004900D0,
// 50 slots: 1 dtor, 2 update, 4 render, 12 writeToReplayFrame, 42 updateIntersectInfo, 43 applyForce, 46 isFlexRail,
// 47 updateNodes, 48 updateMats, 49 getMesh, 50 updateBoundBox (pure); the rest are cInstanceNode's).
//
// Derives from cInstanceNode; its own members FLAT, every field at its retail offset.
// Tags: [R] retail's constructors / destructor; [P] the July 2003 preview (no accessors; tools/layout_xlate.py sees the
// vptr at 0xC and Mats at 0x50 in the paired destructor / update, preview == retail); [U] how retail code uses the
// field; [O] SSX On Tour's DWARF (cInstanceNode chain, cFlexRailNodeState, cFlexRailNode).

#include "layout.h"
#include "object/instancenode.h"
#include "bx/math3d.h"
#include "world/worldview.h" /* tBoundBox */

struct cObjNode;
struct tInstanceRecord;
struct cFlexRailSpaceNode;
struct cSimpleTube;

// World-script data of a flexible rail (On Tour tMakeFlexRailData, 0x48; same offsets): the part common to
// tMakeFloatRailData (object/floatrailnode.h) and the spring rail's data, which start with the same 0x48 bytes.
// [O] On Tour's cFlexRailNode(cBucketList*, int, tInstanceRecord*, const tMakeFlexRailData*); [U]
// cFlexRailNode_cFlexRailNode (view sFlexDef_347D90) counts the instance ids that are not -1 into NumNodes and stores
// Flags as RailFlags.
struct tMakeFlexRailData {
    unsigned int unk_000;           // 0x00 [O] tResourceID InstId (no retail read)
    unsigned int InstId1;           // 0x04 [O] tResourceID; [U] the rail nodes' instance ids, read as an array of 8
    unsigned int InstId2;           // 0x08 [O]
    unsigned int InstId3;           // 0x0C [O]
    unsigned int InstId4;           // 0x10 [O]
    unsigned int InstId5;           // 0x14 [O]
    unsigned int InstId6;           // 0x18 [O]
    unsigned int InstId7;           // 0x1C [O]
    unsigned int InstId8;           // 0x20 [O]
    int Mesh1;                      // 0x24 [O]; [U] read by the derived ctors (tMakeFloatRailData::Mesh1..8, same words)
    int Mesh2;                      // 0x28 [O]
    int Mesh3;                      // 0x2C [O]
    int Mesh4;                      // 0x30 [O]
    int Mesh5;                      // 0x34 [O]
    int Mesh6;                      // 0x38 [O]
    int Mesh7;                      // 0x3C [O]
    int Mesh8;                      // 0x40 [O]
    int Flags;                      // 0x44 [O]; [U] -> RailFlags
};

CHECK_OFFSET(tMakeFlexRailData, unk_000, 0x00);
CHECK_OFFSET(tMakeFlexRailData, InstId1, 0x04);
CHECK_OFFSET(tMakeFlexRailData, InstId8, 0x20);
CHECK_OFFSET(tMakeFlexRailData, Mesh1, 0x24);
CHECK_OFFSET(tMakeFlexRailData, Mesh8, 0x40);
CHECK_OFFSET(tMakeFlexRailData, Flags, 0x44);
CHECK_SIZE(tMakeFlexRailData, 0x48);

struct cBxReplayFrame;

struct t4Matrix;

struct t4Vector;
struct tIntersectInfo;

struct cFlexRailNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cFlexRailNode_cFlexRailNode / _cFlexRailNode1 / __cFlexRailNode store cFlexRailNode_vtbl (D_004900D0); the
    //       subclasses then store cFloatRailNode_vtbl / cSpringRailNode_vtbl; [U] update calls slots 47 and 48,
    //       updateSpaceHash slot 50
    // cFlexRailNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] the subclasses pass 20 / 21
    //   Instance: [U] splineIntersect reads it
    // --- cFlexRailNode (cFlexRailNodeState, 0x30, read whole from a replay frame by cFlexRailNode_cFlexRailNode1)
    // Its new slots ([R] its vtable D_004900D0 / cFloatRailNode_vtbl / cSpringRailNode_vtbl, 50 in all; names from
    // the subclasses' entries, types [O] On Tour's cFlexRailNode / cFloatRailNode definitions):
    virtual void updateNodes();                                     // 47
    virtual void updateMats() __asm__("cFlexRailNode_updateMats__FPv");                                      // 48
    virtual int getMesh(int seg) __asm__("cFlexRailNode_getMesh__FPv");                                   // 49
    virtual void updateBoundBox() = 0;                              // 50 pure ([R] func_00411530 in D_004900D0)
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

    // Methods bound to their retail symbols by asm labels.
    void* cFlexRailNode1(void* bucket, cBxReplayFrame* stream) __asm__("cFlexRailNode_cFlexRailNode1");
    void updateSpaceHash() __asm__("cFlexRailNode_updateSpaceHash");
    void addSpaceHash() __asm__("cFlexRailNode_addSpaceHash");
    void remSpaceHash() __asm__("cFlexRailNode_remSpaceHash");
    void updateMat(t4Matrix* out, const float* ctrlPoint0, const float* ctrlPoint1, const float* ctrlPoint2,
                   const float* ctrlPoint3)
        __asm__("cFlexRailNode_updateMat");
    virtual void update() __asm__("cFlexRailNode_update");
    virtual void render() __asm__("cFlexRailNode_render");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cFlexRailNode_writeToReplayFrame");
    virtual bool isFlexRail() const __asm__("cFlexRailNode_isFlexRail__FPv");
    virtual void applyForce(const tIntersectInfo&, const t4Vector&) __asm__("cFlexRailNode_applyForce__FPv");
    virtual void updateIntersectInfo(tIntersectInfo&) __asm__("cFlexRailNode_updateIntersectInfo__FPv");
    void splineIntersect(const t4Vector* minBpos, const t4Vector* maxBpos, const t4Vector* bpos,
                         unsigned int splineFilterMask, int* hit, float* dist, tIntersectInfo* intInfo)
        __asm__("cFlexRailNode_splineIntersect");
};

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
