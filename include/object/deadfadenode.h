#ifndef OBJECT_DEADFADENODE_H
#define OBJECT_DEADFADENODE_H

// cDeadFadeNode: a dying instance that fades out and sinks while keeping the pose it had when it died: the ctor saves
// one matrix per model mesh (AnimMats: the old node's animation matrices, or the mesh hierarchy under its root
// transform) and deletes the old node; update lowers it by DistRate per frame and fades it by FadeRate; at Fade <= 0
// (or on loseFocus) cObjectInterface_makeDead -> makeDead(Instance, &Data) replaces it (Data.Type 0 -> cDeadNode, 3 ->
// cRestoreNode, 1 -> no node, record flags reset);
// render draws the instance with Fade as alpha, translated down by Dist. Node type 22. Made by MakeDeadFadeFunc and
// checkActiveNode (seg/seg_1FBE38); the replay loader builds one and deletes it at once. No EA source path in the
// preview asserts; retail methods: object/deadfadenode (0x00350750..0x00350E8C) and the empty signal in
// object/railmodifier (0x00360A68).
// Size 0x3C: [R] cMemMan_alloc(0x3C, "DeadFade") in cReplay_restoreObject (src/replay/replay.cpp) and cMemMan_alloc(0x3C)
// in MakeDeadFadeFunc / checkActiveNode; the last member (AnimMats) is at 0x38 (both ctors store it, the dtor frees
// it); [O] On Tour 0x38 (Data 0x18 .. AnimMats 0x34: its cInstanceNode is 4 bytes smaller); [P] preview == retail.
// Base: cInstanceNode (0x00-0x1C, object/instancenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode), written
// out inline with that header's member names; the vptr is at 0xC. On Tour lists no state class for it.
// Constructors: cDeadFadeNode_cDeadFadeNode (bucket, record, tMakeDeadFadeData*, cInstanceNode* old:
// cInstanceNode_cInstanceNode1(this, bucket, 22, record), vtable, Data = the first 12 bytes of the fade data (after
// InstId = -1), FadeRate = update time / FadeTime, DistRate = ZVel * update time, Dist = 0, Fade = 1, AnimMats = new
// t4Matrix[mesh count] ("DeadFadeAnimMats"), delete old, record flags) and cDeadFadeNode_cDeadFadeNode1 (bucket,
// cBxReplayFrame*: cInstanceNode_cInstanceNode, vtable, Data.InstId = -1, AnimMats = 0; reads no state). Destructor
// cDeadFadeNode__cDeadFadeNode (vtable, frees AnimMats, cInstanceNode__cInstanceNode). Vtable cDeadFadeNode_vtbl
// (0x00491980, 46 slots: 1 dtor, 2 update, 4 render, 12 writeToReplayFrame, 35 loseFocus, 36 signal; the rest are
// cInstanceNode's / cSortObjNode's / cTypeObjNode's / cObjNode's).
//
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/object/deadfadenode.cpp) and the 0x3C allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 5 paired methods, 0xC/0x18/0x28-0x38 (preview == retail);
//   [U] how retail code uses the field (src/object/deadfadenode.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cDeadFadeNode, tMakeDeadData, tMakeDeadFadeData).

#include "layout.h"
#include "object/instancenode.h"
#include "bx/math3d.h"

struct cObjNode;
struct tInstanceRecord;

// World-script dead data (On Tour tMakeDeadData, 0xC): what replaces the node when it is done. [R] the cDeadFadeNode
// ctor copies the first 12 bytes of its tMakeDeadFadeData (InstId, Type, FadeTime's word) over it (ldl/ldr + sdl/sdr
// for 8 bytes, lw/sw for the third word); [U] makeDead (seg/seg_1FBE38; also MakeDeadFunc's) switches on Type.
struct tMakeDeadData {
    unsigned int InstId;            // 0x0 [O] tResourceID; [R] ctors store 0xFFFFFFFF (the default id) first
    int Type;                       // 0x4 [O]; [U] makeDead (lw +0x4): 0 -> cDeadNode, 1 -> delete the node and reset
                                    //     the record flags, 3 -> cRestoreNode
    char null;                      // 0x8 [O] WScriptNullType (1 byte)
    char unk_9[0x3];                // 0x9 padding
};

CHECK_OFFSET(tMakeDeadData, InstId, 0x0);
CHECK_OFFSET(tMakeDeadData, Type, 0x4);
CHECK_OFFSET(tMakeDeadData, null, 0x8);
CHECK_SIZE(tMakeDeadData, 0xC);

// World-script data that makes a dead-fade node (On Tour tMakeDeadFadeData, 0x14; same offsets). [O] On Tour's
// cDeadFadeNode(cBucketList*, tInstanceRecord*, const tMakeDeadFadeData*, cInstanceNode* pOldNode); [U]
// cDeadFadeNode_cDeadFadeNode (view sDfDesc_0750) copies the first 12 bytes over Data (as a tMakeDeadData), then
// FadeRate = update time / FadeTime and DistRate = ZVel * update time.
struct tMakeDeadFadeData {
    unsigned int InstId;            // 0x00 [O] tResourceID; [U] copied with Type into Data (12-byte copy)
    int Type;                       // 0x04 [O]; [U] ditto (Data.Type)
    float FadeTime;                 // 0x08 [O]; [U] FadeRate = update time / FadeTime (its word also lands in Data.null)
    float ZVel;                     // 0x0C [O]; [U] DistRate = ZVel * update time
    char unk_010[0x4];              // 0x10 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tMakeDeadFadeData, InstId, 0x00);
CHECK_OFFSET(tMakeDeadFadeData, Type, 0x04);
CHECK_OFFSET(tMakeDeadFadeData, FadeTime, 0x08);
CHECK_OFFSET(tMakeDeadFadeData, ZVel, 0x0C);
CHECK_OFFSET(tMakeDeadFadeData, unk_010, 0x10);
CHECK_SIZE(tMakeDeadFadeData, 0x14);

struct cDeadFadeNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cDeadFadeNode_vtbl (0x00491980) in both ctors and the dtor; [P] 0xC
    // cDeadFadeNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] node type 22 (cInstanceNode_cInstanceNode1)
    //   Instance: [R] ctor reads its model (+0x80), matrix (+0x10) and scale (+0x84), marks its flags; [U] update /
    //       loseFocus pass it to makeDead, render culls and draws it (lw)
    // --- cDeadFadeNode (On Tour 0x18..0x38, + 4 here)
    tMakeDeadData Data;             // 0x1C [O]; [R] both ctors store InstId = -1 (sw 0x1C), ctor copies 12 bytes (sdl/
                                    //      sdr 0x1C, sw 0x24); [U] update / loseFocus pass &Data to makeDead
    float Fade;                     // 0x28 [O]; [P] 0x28; [R] ctor 1.0 (swc1); [U] update fades it (lwc1/swc1), render
                                    //      passes it as alpha
    float FadeRate;                 // 0x2C [O]; [P] 0x2C; [R] ctor update time / data FadeTime (+0x8) (swc1); [U]
                                    //      update (lwc1)
    float Dist;                     // 0x30 [O]; [P] 0x30; [R] ctor 0 (sw $0); [U] update adds DistRate (lwc1/swc1),
                                    //      render translates by -Dist
    float DistRate;                 // 0x34 [O]; [P] 0x34; [R] ctor data ZVel (+0xC) * update time (swc1); [U] update
    t4Matrix* AnimMats;             // 0x38 [O]; [P] 0x38 (dtor); [R] ctor operator new(mesh count * 0x40,
                                    //      "DeadFadeAnimMats"), filled per mesh; replay ctor 0; dtor frees it; [U]
                                    //      render passes it as the mesh matrices
};

CHECK_OFFSET(cDeadFadeNode, Data, 0x1C);
CHECK_OFFSET(cDeadFadeNode, Fade, 0x28);
CHECK_OFFSET(cDeadFadeNode, FadeRate, 0x2C);
CHECK_OFFSET(cDeadFadeNode, Dist, 0x30);
CHECK_OFFSET(cDeadFadeNode, DistRate, 0x34);
CHECK_OFFSET(cDeadFadeNode, AnimMats, 0x38);
CHECK_SIZE(cDeadFadeNode, 0x3C);

#endif /* OBJECT_DEADFADENODE_H */
