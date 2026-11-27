#ifndef OBJECT_ANIMDELTANODE_H
#define OBJECT_ANIMDELTANODE_H

// cAnimDeltaNode: a cAnimObjectNode that only plays as far as a time budget allows: world-script signal 140 adds
// arg / 30 seconds to Delta, and while Delta > 0 (and the start Delay is over) updateNode runs cAnimObjectNode's update
// and spends |AnimSpeed| of Delta per frame; AnimActive follows Delta > 0. Other signals go to cAnimObjectNode_signal.
// Node type 2. Made by MakeAnimDeltaFunc (seg/seg_1FBE38; tMakeAnimDeltaData has tMakeAnimObjectData's layout) and the
// replay loader. No EA source path in the preview asserts; retail methods: the object/multiparticle splat unit
// (0x00359228..0x0035945C) and the dtor / getNodeState in object/railmodifier (0x00361DE0, 0x00361E08).
// Size 0x70: [R] cMemMan_alloc(0x70, "AnimDelta") in cReplay_restoreObject (src/replay/replay.cpp) and
// cMemMan_alloc(0x70) in MakeAnimDeltaFunc; cAnimObjectNode is 0x6C (object/animobject.h) and Delta follows at 0x6C
// (the replay ctor reads 4 bytes there after cAnimObjectNode_cAnimObjectNode; writeToReplayFrame writes them);
// [O] On Tour 0x6C (Delta 0x68: its cAnimObjectNode is 0x68); [P] the preview has Delta at 0x68 (+ 4 in retail,
// tools/layout_xlate.py, 6 votes), its other offsets equal retail.
// Base: cAnimObjectNode (0x00-0x6C, object/animobject.h: cAnimObjectNodeState 0x00, cAnimNode 0x1C = cAnimNodeState +
// cMoveNode at 0x30 + cAnimNode's members), written out inline with that header's member names. The vptr is cMoveNode's
// at 0x30 + 0xC = 0x3C.
// Constructors: cAnimDeltaNode_cAnimDeltaNode1 (bucket, record, tMakeAnimDeltaData*: cAnimObjectNode_cAnimObjectNode1
// (this, bucket, 2, record, data), vtable, Delta = 0, AnimActive = 0) and cAnimDeltaNode_cAnimDeltaNode (bucket,
// cBxReplayFrame*: cAnimObjectNode_cAnimObjectNode, vtable, Delta). Destructor cAnimDeltaNode__cAnimDeltaNode__FPv
// (vtable, cAnimObjectNode__cAnimObjectNode). Vtable cAnimDeltaNode_vtbl (53 slots): delta -0x30 (whole object) on 1 dtor, 12
// writeToReplayFrame, 15 updateNode, 19 getNodeState, 36 signal and on the inherited cAnimObjectNode slots 17 isComplete,
// 31 getAnimDir, 34 expire; 53 getAnimSpeed delta -0x1C (from the cAnimNode part); delta -0x14 on cAnimNode's slots; the
// rest are cMoveNode's (this = +0x30).
//
// A FLAT layout like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors/dtor (src/object/multiparticle.cpp, src/object/railmodifier.cpp) and the 0x70 allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 5 paired methods (0x4-0x42 equal, Delta 0x68 -> 0x6C);
//   [U] how retail code uses the field (src/object/multiparticle.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cAnimDeltaNode : cAnimObjectNode { float Delta; }, the cAnimObjectNode chain).

#include "layout.h"
#include "bx/math3d.h"
#include "wscript/wscriptnull.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;
struct cMeshAnimFrame;
struct tMeshVelocity;

struct cBxReplayFrame;

// World-script data that makes an anim delta node (On Tour tMakeAnimDeltaData, 0x2C: tMakeAnimObjectData's fields,
// object/animobject.h). [R] MakeAnimDeltaFunc (seg/seg_1FBE38) builds its static defaults (InstId -1, Type 1, FromTime
// -1, ToTime -1, Speed 30, StartTime -1) and copies them into the block the script arguments fill; [U]
// cAnimDeltaNode_cAnimDeltaNode1 passes it to cAnimObjectNode_cAnimObjectNode1.
struct tMakeAnimDeltaData {
    unsigned int InstId;            // 0x00 [O] tResourceID; [R] -1 = the script's current instance
    int Type;                       // 0x04 [O]
    int Dir;                        // 0x08 [O]
    float FromTime;                 // 0x0C [O]
    float ToTime;                   // 0x10 [O]
    float Speed;                    // 0x14 [O]
    float SpeedRange;               // 0x18 [O]
    float StartTime;                // 0x1C [O]
    int RandomStart;                // 0x20 [O]
    int ForceVisible;               // 0x24 [O]
    WScriptNullType null;           // 0x28 [O]
    char unk_029[0x3];              // 0x29 padding
};

CHECK_OFFSET(tMakeAnimDeltaData, InstId, 0x00);
CHECK_OFFSET(tMakeAnimDeltaData, Type, 0x04);
CHECK_OFFSET(tMakeAnimDeltaData, Dir, 0x08);
CHECK_OFFSET(tMakeAnimDeltaData, FromTime, 0x0C);
CHECK_OFFSET(tMakeAnimDeltaData, ToTime, 0x10);
CHECK_OFFSET(tMakeAnimDeltaData, Speed, 0x14);
CHECK_OFFSET(tMakeAnimDeltaData, SpeedRange, 0x18);
CHECK_OFFSET(tMakeAnimDeltaData, StartTime, 0x1C);
CHECK_OFFSET(tMakeAnimDeltaData, RandomStart, 0x20);
CHECK_OFFSET(tMakeAnimDeltaData, ForceVisible, 0x24);
CHECK_OFFSET(tMakeAnimDeltaData, null, 0x28);
CHECK_SIZE(tMakeAnimDeltaData, 0x2C);

struct cAnimDeltaNode {
    // The signal that adds to Delta (name ours): [U] signal adds its argument / 30 to Delta and passes any other signal
    // to cAnimObjectNode_signal.
    enum { kAnimDeltaSignal_AddDelta = 140 };
    // --- cAnimObjectNode (0x6C; object/animobject.h): cAnimObjectNodeState (On Tour 0x14, retail 0x1C)
    short AnimType;                 // 0x00 [O]; [R] cAnimObjectNode_cAnimObjectNode1 (data Type)
    char unk_002[0x2];              // 0x02 padding (On Tour keeps its two byte bools here)
    int AnimActive;                 // 0x04 [O] bool (word); [P] 0x4; [R] ctor1 0 (sw $0); [U] updateNode = Delta > 0
                                    //      (sw 1 / sw $0)
    int AnimComplete;               // 0x08 [O] bool (word); [P] 0x8; [U] updateNode tests it (lw)
    int Delay;                      // 0x0C [O]; [P] 0xC; [U] updateNode counts it down while Delta <= 0 (lw/sw)
    float AnimSpeed;                // 0x10 [O]; [P] 0x10; [U] updateNode spends |AnimSpeed| of Delta (lwc1, abs.s)
    float FromTime;                 // 0x14 [O]; [U] cAnimObjectNode_update*
    float ToTime;                   // 0x18 [O]; [U] cAnimObjectNode_update*
    // --- cAnimNode at +0x1C (0x50; object/animnode.h): cAnimNodeState (On Tour 0x14)
    float CurrTime;                 // 0x1C [O]; [U] cAnimObjectNode_update*
    float CacheTime;                // 0x20 [O]; [U] cAnimObjectNode_updateNode
    float FrameStart;               // 0x24 [O]; [P] 0x24; [U] updateNode 1e10 (swc1)
    float FrameEnd;                 // 0x28 [O]; [P] 0x28; [U] updateNode 1e10 (swc1)
    int MeshVelocityDirty;          // 0x2C [O] bool (word); [U] cAnimNode_cacheFrame / updateMeshVelocity
    // --- cMoveNode at +0x30 (0x2C; object/movenode.h): cObjNode / cTypeObjNode / cSortObjNode / cInstanceNode first
    int MoveNodeBase;               // 0x30 where the cMoveNode base starts (name ours): `&MoveNodeBase` is the
                                    //      cMoveNode subobject the methods pass on (cMoveNode_* calls, vtable
                                    //      thunks). The word is cObjNode's unnamed leading word (unk_00 in
                                    //      object/objnode.h); no ctor writes it
    cObjNode* Next;                 // 0x34 [O] cObjNode::Next; [U] bucket list
    cObjNode* Prev;                 // 0x38 [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x3C [R] cAnimDeltaNode_vtbl (both ctors, the dtor); [P] 0x3C; [U] updateNode calls slot
                                    //      34 (expire) with this = +0x30
    short ObjNodeType;              // 0x40 [O] cTypeObjNodeState; [R] node type 2 (cAnimObjectNode_cAnimObjectNode1)
    unsigned short Flags;           // 0x42 [O]; [P] 0x42 bit 0 = RootTransformDirty; [U] updateNode sets bit 0
                                    //      (lhu/sh)
    unsigned int SortIndex;         // 0x44 [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x48 [O] cInstanceNode::Instance
    tModifierBlock* ModifierBlock;  // 0x4C [O] cMoveNode
    int Debounce;                   // 0x50 [O] cMoveNode
    float Fade;                     // 0x54 [O] cMoveNode
    int TransferHack;               // 0x58 [O] cMoveNode (bool, 4 bytes)
    // --- cAnimNode's own members
    int NumAnimMeshes;              // 0x5C [O]; [U] cAnimNode_* (this + 0x40)
    t4Matrix* AnimMats;             // 0x60 [O]; [U] cAnimNode_* (this + 0x44)
    cMeshAnimFrame* AnimFrames;     // 0x64 [O]; [U] cAnimObjectNode_cAnimObjectNode1
    tMeshVelocity* MeshVelocity;    // 0x68 [O]; [U] cAnimNode_* (this + 0x4C)
    // --- cAnimDeltaNode (On Tour 0x68, + 4 here)
    float Delta;                    // 0x6C [O]; [P] preview 0x68; [R] ctor1 0 (sw $0); the replay ctor reads 4 bytes;
                                    //      [U] updateNode spends it (lwc1/swc1, clamps at 0), signal 140 adds arg / 30

    // Methods bound to their retail symbols by asm labels.
    void* cAnimDeltaNode1(int bucket, int instRecord, int animData) __asm__("cAnimDeltaNode_cAnimDeltaNode1");
    bool updateNode() __asm__("cAnimDeltaNode_updateNode");
    void signal(int id, float signalArg) __asm__("cAnimDeltaNode_signal");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cAnimDeltaNode_writeToReplayFrame");
};

CHECK_OFFSET(cAnimDeltaNode, AnimType, 0x00);
CHECK_OFFSET(cAnimDeltaNode, unk_002, 0x02);
CHECK_OFFSET(cAnimDeltaNode, AnimActive, 0x04);
CHECK_OFFSET(cAnimDeltaNode, AnimComplete, 0x08);
CHECK_OFFSET(cAnimDeltaNode, Delay, 0x0C);
CHECK_OFFSET(cAnimDeltaNode, AnimSpeed, 0x10);
CHECK_OFFSET(cAnimDeltaNode, FromTime, 0x14);
CHECK_OFFSET(cAnimDeltaNode, ToTime, 0x18);
CHECK_OFFSET(cAnimDeltaNode, CurrTime, 0x1C);
CHECK_OFFSET(cAnimDeltaNode, CacheTime, 0x20);
CHECK_OFFSET(cAnimDeltaNode, FrameStart, 0x24);
CHECK_OFFSET(cAnimDeltaNode, FrameEnd, 0x28);
CHECK_OFFSET(cAnimDeltaNode, MeshVelocityDirty, 0x2C);
CHECK_OFFSET(cAnimDeltaNode, MoveNodeBase, 0x30);
CHECK_OFFSET(cAnimDeltaNode, Next, 0x34);
CHECK_OFFSET(cAnimDeltaNode, Prev, 0x38);
CHECK_OFFSET(cAnimDeltaNode, mVtbl, 0x3C);
CHECK_OFFSET(cAnimDeltaNode, ObjNodeType, 0x40);
CHECK_OFFSET(cAnimDeltaNode, Flags, 0x42);
CHECK_OFFSET(cAnimDeltaNode, SortIndex, 0x44);
CHECK_OFFSET(cAnimDeltaNode, Instance, 0x48);
CHECK_OFFSET(cAnimDeltaNode, ModifierBlock, 0x4C);
CHECK_OFFSET(cAnimDeltaNode, Debounce, 0x50);
CHECK_OFFSET(cAnimDeltaNode, Fade, 0x54);
CHECK_OFFSET(cAnimDeltaNode, TransferHack, 0x58);
CHECK_OFFSET(cAnimDeltaNode, NumAnimMeshes, 0x5C);
CHECK_OFFSET(cAnimDeltaNode, AnimMats, 0x60);
CHECK_OFFSET(cAnimDeltaNode, AnimFrames, 0x64);
CHECK_OFFSET(cAnimDeltaNode, MeshVelocity, 0x68);
CHECK_OFFSET(cAnimDeltaNode, Delta, 0x6C);
CHECK_SIZE(cAnimDeltaNode, 0x70);

#endif /* OBJECT_ANIMDELTANODE_H */
