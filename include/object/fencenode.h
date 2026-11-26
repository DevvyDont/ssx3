#ifndef OBJECT_FENCENODE_H
#define OBJECT_FENCENODE_H

// cFenceNode: the scene node of a fence instance that a rider hit: it swings (a damped force oscillating between
// +-ForceMax), bends an 8x2 grid mesh (top and bottom rows of 8 points) and pushes the neighbouring fence instances
// after a short delay. EA's source: d:/ssx/code/source/object/fencenode.cpp (preview asserts); retail methods are in
// the object/flagset splat unit (0x34C8E8..0x34D9AC).
// Size 0x10C: retail cMemMan_alloc(0x10C, "Fence") in cReplay_restoreObject (src/replay/replay.cpp) and the
// neighbour fences made by cFenceNode_makeNeighbour; On Tour 0x108 (its cInstanceNode is 4 bytes smaller).
// Constructors: cFenceNode_cFenceNode1 (hit: bucket, record, tMakeFenceData*, tIntersectInfo*), cFenceNode_cFenceNode2
// (a
// pushed neighbour: bucket, record, tMakeFenceData*, bNext, force) and cFenceNode_cFenceNode (replay: reads the
// 0x24-byte
// cFenceNodeState at +0x1C); all run cInstanceNode's ctor with node type 9 (or the replay one). Destructor
// cFenceNode__cFenceNode; vtable cFenceNode_vtbl (0x0048FA00, 46 slots: 1 dtor, 2 update, 4 render, 12
// writeToReplayFrame, 40 onCollide; the rest are cInstanceNode's).
// Derives from cInstanceNode (0x00-0x1C, object/instancenode.h), then FLAT: cFenceNodeState (0x1C-0x40: On Tour's
// members in order, each On Tour offset + 4; tMakeFenceData FenceData first) + cFenceNode's own members (+ 4).
// Evidence tags:
//   [R] retail ctors/dtor and the 0x10C allocation;
//   [P] preview offsets via tools/layout_xlate.py: preview == retail for 0xC, 0x2C-0x40, 0x104, 0x108 (no accessors);
//   [U] how retail code uses the field (src/object/flagset.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cFenceNodeState, tMakeFenceData, cFenceNode).

#include "layout.h"
#include "object/instancenode.h"
#include "ui/uithing.h"

struct cObjNode;
struct tInstanceRecord;
struct cGridMesh;

// Script data that makes a fence swing (On Tour tMakeFenceData, 0x10). [R] each ctor copies it whole (ldl/ldr +
// sdl/sdr)
// and then sets InstId to -1.
struct tMakeFenceData {
    unsigned int InstId;            // 0x0 [O] tResourceID; [R] ctors store 0xFFFFFFFF after the copy
    // A Type whose bottom row swings with the top (name ours): [U] cFenceNode::updateGrid moves both rows for it and
    // keeps the bottom row at rest for any other Type.
    enum { kFenceType_SwingBottom = 1 };
    int Type;                       // 0x4 [O]; [U] updateGrid: kFenceType_SwingBottom = the bottom row bends too
                                    //     (lw 0x20)
    float Flexibility;              // 0x8 [O]; [U] updateGrid scales the sway by it (lwc1 0x24)
    char null;                      // 0xC [O] WScriptNullType (1 byte)
    char unk_D[0x3];                // 0xD padding
};

CHECK_OFFSET(tMakeFenceData, InstId, 0x0);
CHECK_OFFSET(tMakeFenceData, Type, 0x4);
CHECK_OFFSET(tMakeFenceData, Flexibility, 0x8);
CHECK_OFFSET(tMakeFenceData, null, 0xC);
CHECK_SIZE(tMakeFenceData, 0x10);

// The replayed state of a fence (On Tour cFenceNodeState, 0x24). cFenceNode holds these members inline at 0x1C (the
// FLAT layout below); [R] the replay ctor reads 0x24 bytes into +0x1C and writeToReplayFrame writes them;
// [U] cFenceNode_update re-reads Force through this part (+0x1C, then +0x10).
struct cFenceNodeState {
    tMakeFenceData FenceData;       // 0x00 [O]
    float Force;                    // 0x10 [O]
    float ForceMax;                 // 0x14 [O]
    float ForceStep;                // 0x18 [O]
    int NextPropTime;               // 0x1C [O]
    int PrevPropTime;               // 0x20 [O]
};

CHECK_OFFSET(cFenceNodeState, FenceData, 0x00);
CHECK_OFFSET(cFenceNodeState, Force, 0x10);
CHECK_OFFSET(cFenceNodeState, ForceMax, 0x14);
CHECK_OFFSET(cFenceNodeState, ForceStep, 0x18);
CHECK_OFFSET(cFenceNodeState, NextPropTime, 0x1C);
CHECK_OFFSET(cFenceNodeState, PrevPropTime, 0x20);
CHECK_SIZE(cFenceNodeState, 0x24);

struct tCollideInfo;

struct cFenceNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cFenceNode_vtbl (every ctor and the dtor); [U] update calls slot 34 (expire)
    // cFenceNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] node type 9
    //   Instance: [U] setupFence marks the record (flags +0x8), cFenceNode_cFenceNode1 reads its matrix (+0x10),
    //       render
    // --- cFenceNodeState (On Tour 0x24; [R] the replay ctor reads 0x24 bytes into +0x1C, writeToReplayFrame writes
    //       them)
    tMakeFenceData FenceData;       // 0x01C [O]; [R] ctors copy the make-data, InstId = -1; [U] makeNeighbour passes
                                    //       &FenceData
                                    //       to the neighbour's ctor
    float Force;                    // 0x02C [O]; [R] ctors 0 (sw $0); [U] update adds ForceStep, clamps to +-ForceMax;
                                    //       updateGrid sways the grid by it
    float ForceMax;                 // 0x030 [O]; [R] ctors 0; [U] setForce raises it, update halves it at each swing
                                    //       end,
                                    //       onCollide ignores hits while >= 1, makeNeighbour passes 0.4 * it on
    float ForceStep;                // 0x034 [O]; [R] ctors 0; [U] setForce/update set it to +-ForceMax * 0.1
    int NextPropTime;               // 0x038 [O]; [R] cFenceNode1 0, cFenceNode2 4; [U] update counts it down, then
                                    //       pushes the
                                    //       next fence (makeNeighbour(NextInstance, 1)); onCollide/neighbourCollide set
                                    //       4
    int PrevPropTime;               // 0x03C [O]; [R] as NextPropTime; [U] the previous fence (PrevInstance, 0)
    // --- cFenceNode
    cGridMesh* GridMesh;            // 0x040 [O]; [U] setupFence creates it (graphics slot 76, 8 x 2), the dtor frees it
                                    //       (slot 77), updateGrid/render/makeNeighbour/cFenceNode1 test it
    t3Vector Verts[16];             // 0x044 [O]; [R] ctors construct 16 (empty ctor loop); [U] setupFence fills the top
                                    //       row
                                    //       (Verts[0..7]) and bottom row (Verts[8..15], &Verts[8] = +0xA4); updateGrid
                                    //       reads both
    tInstanceRecord* NextInstance;  // 0x104 [O]; [U] setupFence clears it, cFenceNode1/update/makeNeighbour use it
    tInstanceRecord* PrevInstance;  // 0x108 [O]; [U] as NextInstance

    // Methods bound to their retail symbols by asm labels.
    cFenceNode* cFenceNode1(int bucket, void* pInst, tMakeFenceData* tdata, tCollideInfo* info)
        __asm__("cFenceNode_cFenceNode1");
    void makeNeighbour(tInstanceRecord* obj, int flag) __asm__("cFenceNode_makeNeighbour");
    void neighbourCollide(int on, float dt) __asm__("cFenceNode_neighbourCollide");
    void updateGrid() __asm__("cFenceNode_updateGrid");
    void setupFence() __asm__("cFenceNode_setupFence");
    void setForce(float force) __asm__("cFenceNode_setForce");
    // retail's destructor cFenceNode__cFenceNode stays a C function (g++ 2.95 takes no asm label on a destructor);
    // declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFenceNode();
    virtual void render() __asm__("cFenceNode_render");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cFenceNode_writeToReplayFrame");
};

CHECK_OFFSET(cFenceNode, FenceData, 0x01C);
CHECK_OFFSET(cFenceNode, Force, 0x02C);
CHECK_OFFSET(cFenceNode, ForceMax, 0x030);
CHECK_OFFSET(cFenceNode, ForceStep, 0x034);
CHECK_OFFSET(cFenceNode, NextPropTime, 0x038);
CHECK_OFFSET(cFenceNode, PrevPropTime, 0x03C);
CHECK_OFFSET(cFenceNode, GridMesh, 0x040);
CHECK_OFFSET(cFenceNode, Verts, 0x044);
CHECK_OFFSET(cFenceNode, NextInstance, 0x104);
CHECK_OFFSET(cFenceNode, PrevInstance, 0x108);
CHECK_SIZE(cFenceNode, 0x10C);

#endif /* OBJECT_FENCENODE_H */
