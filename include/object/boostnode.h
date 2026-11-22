#ifndef OBJECT_BOOSTNODE_H
#define OBJECT_BOOSTNODE_H

// cBoostNode: a boost (or drag) zone on an instance: update counts Time down and (for Type 1 always, otherwise once
// Time has run out) calls, for every being whose intersect test (being slot 3) hits the instance, applyDrag (Drag < 0:
// scales the being's velocity by Drag + 1) or applyBoost (pushes the being along Boost - or along its own velocity when
// Boost is zero - towards MaxSpeed, scaled by Drag); Type 2 (an instance-relative boost: Boost transformed by the
// record's matrix, MaxSpeed negated) skips beings for which being slot 9 is true; the node expires (slot 34) when
// nothing kept it alive. Signal 1 restarts Time from BoostTime. Node type: the caller's (MakeBoostFunc passes 8). Made by MakeBoostFunc (seg/seg_1FBE38) and the
// replay loader. No EA source path in the preview asserts; retail methods: the intersect/worldsphtree splat unit
// (0x00341388..0x00341A9C) and the empty boostStart / boostEnd in object/railmodifier (0x00360AF0, 0x00360AF8).
// Size 0x50: [R] cMemMan_alloc(0x50, "Boost") in cReplay_restoreObject (src/replay/replay.cpp) and cMemMan_alloc(0x50)
// in MakeBoostFunc; the replay ctor reads the 0x30-byte cBoostNodeState into +0x20 (writeToReplayFrame writes it),
// ending at 0x50; [O] On Tour 0x50 (cInstanceNode 0x18, the 16-aligned state at 0x20).
// Bases: cInstanceNode (0x00-0x1C, object/instancenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode), a 4-byte
// gap (the state holds a t4Vector, so it is 16-byte aligned), then cBoostNodeState (On Tour 0x30) at 0x20; all written
// out inline. The vptr is at 0xC.
// Constructors: cBoostNode_cBoostNode1 (bucket, type, record, tMakeBoostData*: cInstanceNode_cInstanceNode1(this,
// bucket, type, record), vtable, Type, BoostTime = (int)(data BoostTime * frame rate), Boost = normalized (BoostX, Y,
// Z) or YAxis (Type 2) / Zero, MaxSpeed = data MaxSpeed * 27.78, Drag, Time = Type ? BoostTime : 0, Type 2 transform)
// and cBoostNode_cBoostNode (bucket, cBxReplayFrame*: cInstanceNode_cInstanceNode, vtable, the 0x30-byte state).
// Destructor cBoostNode__cBoostNode__FPv (vtable, cInstanceNode__cInstanceNode). Vtable cBoostNode_vtbl (0x004914E0,
// 50 slots: 1 dtor, 2 update, 12 writeToReplayFrame, 36 signal; the rest of 1..46 are cInstanceNode's /
// cSortObjNode's / cTypeObjNode's / cObjNode's; new virtuals 47 applyBoost, 48 applyDrag, 49 boostStart, 50 boostEnd).
//
// A FLAT layout like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors/dtor (src/intersect/worldsphtree.cpp) and the 0x50 allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 8 paired methods, 0xC/0x18/0x20/0x34-0x40 (preview == retail);
//   [U] how retail code uses the field (src/intersect/worldsphtree.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cBoostNodeState, tMakeBoostData).

#include "layout.h"
#include "bx/math3d.h"

struct cObjNode;
struct tInstanceRecord;

// World-script data that makes a boost node (On Tour tMakeBoostData, 0x24; same offsets). [O] On Tour's
// cBoostNode(cBucketList*, int, tInstanceRecord*, const tMakeBoostData*); [U] cBoostNode_cBoostNode1 (view
// sDef_341388) stores Type, BoostTime seconds * TimerHz, MaxSpeed * 27.78 and Drag, and normalises (BoostX, BoostY,
// BoostZ) into Boost (all zero: a default direction by Type). cBoostModifier's tAddBoostData (object/boostmodifier.h)
// has the same layout.
struct tMakeBoostData {
    unsigned int unk_000;           // 0x00 [O] tResourceID InstId (no retail read)
    short Type;                     // 0x04 [O] int Type; [U] the ctor reads its low half (lh) -> Type
    char unk_006[0x2];              // 0x06 [O] Type's high half (never read)
    float BoostTime;                // 0x08 [O]; [U] * TimerHz -> BoostTime (frames)
    float Drag;                     // 0x0C [O]; [U] -> Drag
    float MaxSpeed;                 // 0x10 [O]; [U] * 27.78 -> MaxSpeed
    float BoostX;                   // 0x14 [O]; [U] the boost direction, normalised into Boost
    float BoostY;                   // 0x18 [O]; [U] ditto
    float BoostZ;                   // 0x1C [O]; [U] ditto
    char unk_020[0x4];              // 0x20 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tMakeBoostData, unk_000, 0x00);
CHECK_OFFSET(tMakeBoostData, Type, 0x04);
CHECK_OFFSET(tMakeBoostData, unk_006, 0x06);
CHECK_OFFSET(tMakeBoostData, BoostTime, 0x08);
CHECK_OFFSET(tMakeBoostData, Drag, 0x0C);
CHECK_OFFSET(tMakeBoostData, MaxSpeed, 0x10);
CHECK_OFFSET(tMakeBoostData, BoostX, 0x14);
CHECK_OFFSET(tMakeBoostData, BoostY, 0x18);
CHECK_OFFSET(tMakeBoostData, BoostZ, 0x1C);
CHECK_OFFSET(tMakeBoostData, unk_020, 0x20);
CHECK_SIZE(tMakeBoostData, 0x24);

struct cBoostNode {
    // --- cInstanceNode (0x1C; object/instancenode.h): cObjNode / cTypeObjNode / cSortObjNode first
    int unk_00;                     // 0x00 cObjNode's leading word; no ctor writes it (see object/objnode.h)
    cObjNode* Next;                 // 0x04 [O] cObjNode::Next; [U] bucket list
    cObjNode* Prev;                 // 0x08 [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x0C [R] cBoostNode_vtbl (0x004914E0) in both ctors and the dtor; [P] 0xC; [U]
                                    //      update calls slots 49 / 47 / 48 / 50 / 34
    short ObjNodeType;              // 0x10 [O] cTypeObjNodeState; [R] the type arg (cInstanceNode_cInstanceNode1)
    unsigned short Flags;           // 0x12 [O] cTypeObjNodeState
    unsigned int SortIndex;         // 0x14 [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x18 [O] cInstanceNode::Instance; [P] 0x18 (update); [R] ctor1 transforms a Type
                                    //      2 Boost by its matrix (+0x10); [U] update passes it to each being's
                                    //      intersect test (lw)
    char unk_01C[0x4];              // 0x1C padding before the 16-byte aligned state (no access)
    // --- cBoostNodeState (On Tour 0x30; [R] the replay ctor reads 0x30 bytes here, writeToReplayFrame writes them)
    t4Vector Boost;                 // 0x20 [O]; [P] 0x20; [R] ctor1 (sq); [U] applyBoost reads it (lq)
    float MaxSpeed;                 // 0x30 [O]; [R] ctor1 data MaxSpeed (+0x10) * 27.78, negated for Type 2 (swc1);
                                    //      [U] applyBoost (lwc1)
    float Drag;                     // 0x34 [O]; [P] 0x34; [R] ctor1 data Drag (+0xC) (swc1); [U] update: < 0 ->
                                    //      applyDrag, else applyBoost; applyDrag (Drag + 1) / applyBoost scale by it
    int BoostTime;                  // 0x38 [O]; [P] 0x38; [R] ctor1 (int)(data BoostTime * frame rate) (cvt.w.s +
                                    //      swc1: an int through the FPU); [U] ctor1 / signal 1 copy it to Time (lw)
    int Time;                       // 0x3C [O]; [P] 0x3C; [R] ctor1 Type ? BoostTime : 0 (sw); [U] update counts it
                                    //      down (lw/sw), signal 1 restarts it
    int Type;                       // 0x40 [O]; [P] 0x40; [R] ctor1 data Type (lh +0x4, sw); [U] update / applyBoost /
                                    //      ctor1 test 1 / 2 (lw)
    char unk_044[0xC];              // 0x44 the state's tail padding (On Tour cBoostNodeState is 0x30; no access)
};

CHECK_OFFSET(cBoostNode, unk_00, 0x00);
CHECK_OFFSET(cBoostNode, Next, 0x04);
CHECK_OFFSET(cBoostNode, Prev, 0x08);
CHECK_OFFSET(cBoostNode, mVtbl, 0x0C);
CHECK_OFFSET(cBoostNode, ObjNodeType, 0x10);
CHECK_OFFSET(cBoostNode, Flags, 0x12);
CHECK_OFFSET(cBoostNode, SortIndex, 0x14);
CHECK_OFFSET(cBoostNode, Instance, 0x18);
CHECK_OFFSET(cBoostNode, unk_01C, 0x1C);
CHECK_OFFSET(cBoostNode, Boost, 0x20);
CHECK_OFFSET(cBoostNode, MaxSpeed, 0x30);
CHECK_OFFSET(cBoostNode, Drag, 0x34);
CHECK_OFFSET(cBoostNode, BoostTime, 0x38);
CHECK_OFFSET(cBoostNode, Time, 0x3C);
CHECK_OFFSET(cBoostNode, Type, 0x40);
CHECK_OFFSET(cBoostNode, unk_044, 0x44);
CHECK_SIZE(cBoostNode, 0x50);

// cBoostNodeState (On Tour, 0x30): the boost state cBoostNode carries at +0x20 (Boost .. Type above, written out
// inline there); cBoostNode_applyBoost reads it as one (view sParams_341818, inside sSph_341818 at 0x20).
struct cBoostNodeState {
    t4Vector Boost;                 // 0x00 [O]
    float MaxSpeed;                 // 0x10 [O]
    float Drag;                     // 0x14 [O]
    int BoostTime;                  // 0x18 [O]
    int Time;                       // 0x1C [O]
    int Type;                       // 0x20 [O]
    char unk_024[0xC];              // 0x24 padding (16-byte aligned)
};

CHECK_OFFSET(cBoostNodeState, Boost, 0x00);
CHECK_OFFSET(cBoostNodeState, MaxSpeed, 0x10);
CHECK_OFFSET(cBoostNodeState, Drag, 0x14);
CHECK_OFFSET(cBoostNodeState, BoostTime, 0x18);
CHECK_OFFSET(cBoostNodeState, Time, 0x1C);
CHECK_OFFSET(cBoostNodeState, Type, 0x20);
CHECK_SIZE(cBoostNodeState, 0x30);

#endif /* OBJECT_BOOSTNODE_H */
