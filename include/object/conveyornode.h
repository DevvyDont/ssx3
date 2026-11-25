#ifndef OBJECT_CONVEYORNODE_H
#define OBJECT_CONVEYORNODE_H

// cConveyorNode: a conveyor (moving walkway / lift belt) instance: it scrolls the model's texture by UVScroll per frame
// (UV wraps to -1..1; render draws the instance with the UV offset in the matrix's position row) and, for a rider
// touching mesh i, adds Vel along the contact direction: Rotate[i] != 0 pushes linearly from the mesh's position
// (impulse + torque), otherwise it spins about the mesh's up axis (torque only). Node type 15. Made by MakeConveyorFunc
// (seg/seg_1FBE38) and the replay loader. No EA source path in the preview asserts; retail methods: object/debouncenode
// (0x00343010..0x00343764) and the dtor / getNodeState in object/railmodifier (0x00361500, 0x00361528).
// Size 0x48: [R] cMemMan_alloc(0x48, "Conveyor") in cReplay_restoreObject (src/replay/replay.cpp) and cMemMan_alloc(0x48)
// in MakeConveyorFunc; the replay ctor reads the 0x2C-byte cConveyorNodeState into +0x1C (writeToReplayFrame writes
// it; getNodeState returns +0x1C), ending at 0x48; [O] On Tour 0x44 = cInstanceNode 0x18 + cConveyorNodeState 0x2C.
// Bases: cInstanceNode (0x00-0x1C, object/instancenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode), then
// cConveyorNodeState (On Tour 0x2C) at 0x1C (the state written out). The vptr is at 0xC.
// Constructors: cConveyorNode_cConveyorNode (bucket, record, tMakeConveyorData*: cInstanceNode_cInstanceNode1(this,
// bucket, 15, record), vtable, Vel, UVScroll = (UScroll, VScroll), UV = 0, Rotate[0..7] = Rotate0..7, record flags)
// and cConveyorNode_cConveyorNode1 (bucket, cBxReplayFrame*: cInstanceNode_cInstanceNode, vtable, the 0x2C-byte
// state). Destructor cConveyorNode__cConveyorNode__FPv (vtable, cInstanceNode__cInstanceNode). Vtable
// cConveyorNode_vtbl (0x00490570, 46 slots: 1 dtor, 2 update, 4 render, 12 writeToReplayFrame, 19 getNodeState, 42
// updateIntersectInfo; the rest are cInstanceNode's / cSortObjNode's / cTypeObjNode's / cObjNode's).
//
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/object/debouncenode.cpp, src/object/railmodifier.cpp) and the 0x48 allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 5 paired methods, 0xC/0x18/0x38-0x44 (preview == retail);
//   [U] how retail code uses the field (src/object/debouncenode.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cConveyorNodeState, tMakeConveyorData).
// t2Vector (2 floats) is written as a float array, as in render/font.h.

#include "layout.h"
#include "object/instancenode.h"

struct cObjNode;
struct tInstanceRecord;

// World-script data that makes a conveyor (On Tour tMakeConveyorData, 0x34; same offsets). [O] On Tour's
// cConveyorNode(cBucketList*, tInstanceRecord*, const tMakeConveyorData*); [U] cConveyorNode_cConveyorNode (view
// sDesc_3010) stores Vel, builds the UV scroll from (UScroll, VScroll) and copies Rotate0..7 (read as an array of 8).
struct tMakeConveyorData {
    unsigned int unk_000;           // 0x00 [O] tResourceID InstId (no retail read)
    float Vel;                      // 0x04 [O]; [U] -> the node's speed (+0x1C)
    float UScroll;                  // 0x08 [O]; [U] the UV scroll's u
    float VScroll;                  // 0x0C [O]; [U] the UV scroll's v
    int Rotate0;                    // 0x10 [O]; [U] the 8 rotate flags, read as an array of 8 into the node
    int Rotate1;                    // 0x14 [O]
    int Rotate2;                    // 0x18 [O]
    int Rotate3;                    // 0x1C [O]
    int Rotate4;                    // 0x20 [O]
    int Rotate5;                    // 0x24 [O]
    int Rotate6;                    // 0x28 [O]
    int Rotate7;                    // 0x2C [O]
    char unk_030[0x4];              // 0x30 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tMakeConveyorData, unk_000, 0x00);
CHECK_OFFSET(tMakeConveyorData, Vel, 0x04);
CHECK_OFFSET(tMakeConveyorData, UScroll, 0x08);
CHECK_OFFSET(tMakeConveyorData, VScroll, 0x0C);
CHECK_OFFSET(tMakeConveyorData, Rotate0, 0x10);
CHECK_OFFSET(tMakeConveyorData, Rotate7, 0x2C);
CHECK_OFFSET(tMakeConveyorData, unk_030, 0x30);
CHECK_SIZE(tMakeConveyorData, 0x34);

struct cBxReplayFrame;

struct cConveyorNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cConveyorNode_vtbl (0x00490570) in both ctors and the dtor; [P] 0xC; [U] updateIntersectInfo calls slot 24
    //       (getRootTransform)
    // cConveyorNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] node type 15 (cInstanceNode_cInstanceNode1)
    //   Instance: [R] ctor marks the record's flags (+0x8); [U] updateIntersectInfo walks its model's (+0x80) mesh
    //       hierarchy, render culls / draws it
    // --- cConveyorNodeState (On Tour 0x2C; [R] the replay ctor reads 0x2C bytes here, writeToReplayFrame writes them,
    //     getNodeState returns its address)
    float Vel;                      // 0x1C [O]; [R] ctor = data Vel (+0x4) (lwc1/swc1); [U] updateIntersectInfo scales
                                    //      the push by it (lwc1)
    char Rotate[24];                // 0x20 [O] char[24]; [R] ctor stores data Rotate0..7 into [0..7] (lbu/sb); [U]
                                    //      updateIntersectInfo: Rotate[mesh] (lb) picks a linear push (!= 0) or a spin
    float UVScroll[2];              // 0x38 [O] t2Vector; [R] ctor = (data UScroll +0x8, VScroll +0xC) (8-byte copy,
                                    //      sdl/sdr); [P] 0x38 / 0x3C; [U] update adds it to UV (lwc1)
    float UV[2];                    // 0x40 [O] t2Vector; [R] ctor 0 (sw $0); [P] 0x40 / 0x44; [U] update advances and
                                    //      wraps it (lwc1/swc1), render puts it in the draw matrix's position row

    // Methods bound to their retail symbols by asm labels.
    void* cConveyorNode1(void* bucket, cBxReplayFrame* stream) __asm__("cConveyorNode_cConveyorNode1");
};

CHECK_OFFSET(cConveyorNode, Vel, 0x1C);
CHECK_OFFSET(cConveyorNode, Rotate, 0x20);
CHECK_OFFSET(cConveyorNode, UVScroll, 0x38);
CHECK_OFFSET(cConveyorNode, UV, 0x40);
CHECK_SIZE(cConveyorNode, 0x48);

#endif /* OBJECT_CONVEYORNODE_H */
