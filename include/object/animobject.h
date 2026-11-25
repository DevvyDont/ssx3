#ifndef OBJECT_ANIMOBJECT_H
#define OBJECT_ANIMOBJECT_H

// cAnimObjectNode: a world object that plays its mesh animation between FromTime and ToTime at AnimSpeed, once,
// cycling or oscillating (AnimType 0/1/2), after a start delay; world-script signals 101..104 set the time, speed,
// upper and lower limit. Node type 1 (3 as cAnimComboNode). Built by the world script's MakeAnimObjectFunc
// (tMakeAnimObjectData) and the replay loader (also tActiveAvalanche_update); base of cAnimComboNode
// (object/animcombonode.h) and cAnimDeltaNode. EA's source: d:/ssx/code/source/object/animobject.cpp (preview assert
// path). Retail methods: the intersect/worldsphtree splat unit (0x00341AA0..0x00342150) and the one-liners in
// object/railmodifier (0x00361150..0x00361198).
// Size 0x6C: [R] cMemMan_alloc(0x6C, ...) in MakeAnimObjectFunc and cReplay_restoreObject ("AnimObject"); its last base
// member (cAnimNode::MeshVelocity) is at 0x68; [O] On Tour 0x68 (its cAnimObjectNodeState is 0x14 with byte bools and
// its cObjNode 4 bytes smaller; it pads the end to 0x68, retail has no tail gap).
// Bases (state first, as in every EA node): cAnimObjectNodeState (On Tour 0x14, retail 0x1C: the two bools are words)
// at 0x00, then cAnimNode (0x50, object/animnode.h) at 0x1C = cAnimNodeState (0x14) + cMoveNode (0x2C, at 0x30,
// object/movenode.h) + cAnimNode's own members (0x5C); all written out inline with object/animcombonode.h's member
// names (cAnimComboNode embeds this whole class at +0x18). The vptr is cMoveNode's: 0x30 + 0xC = 0x3C.
// Constructors: cAnimObjectNode_cAnimObjectNode1 (bucket, type, record, tMakeAnimObjectData*: cAnimNode_cAnimNode on
// +0x1C with ForceVisible (data +0x24), vtable, AnimType = data Type (+0x4), FromTime/ToTime from data +0xC/+0x10
// (/ 30; negative = the animation's start time / length), CurrTime = data StartTime or AIrandf(From, To) when
// RandomStart, AnimSpeed from Speed/SpeedRange (data +0x14/+0x18) / 30 / frame rate (negated for Dir == 1), CacheTime =
// CurrTime, AnimActive = 1, Delay = AnimComplete = 0, Flags |= RootTransformDirty) and cAnimObjectNode_cAnimObjectNode
// (bucket, cBxReplayFrame*: cAnimNode_cAnimNode1 on +0x1C, then the 0x1C-byte state). Destructor
// cAnimObjectNode__cAnimObjectNode. Vtable cAnimObjectNode_vtbl (53 slots): delta -0x30 (whole object) on
// 1 dtor, 12 writeToReplayFrame, 15 updateNode, 17 isComplete, 19 getNodeState, 31 getAnimDir, 34 expire, 36 signal and
// on 53 getAnimSpeed (delta -0x1C from the cAnimNode part); delta -0x14 on the inherited cAnimNode slots (this =
// +0x1C); the rest are cMoveNode's (this = +0x30).
//
// A FLAT layout like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors/dtor (src/intersect/worldsphtree.cpp) and the 0x6C allocation;
//   [P] the July 2003 preview via tools/layout_xlate.py (no accessors observed for this class);
//   [U] how retail code uses the field (src/intersect/worldsphtree.cpp, src/object/railmodifier.cpp, the retail asm;
//       for the cAnimNode part also cAnimNode_* with this = +0x1C);
//   [O] SSX On Tour's DWARF (cAnimObjectNodeState, cAnimNodeState, cAnimNode, cMoveNode chain, tMakeAnimObjectData).

#include "layout.h"
#include "bx/math3d.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;
struct cMeshAnimFrame;
struct tMeshVelocity;

struct cAnimObjectNode {
    // --- cAnimObjectNodeState (On Tour 0x14, retail 0x1C; [R] the replay ctor reads it, getNodeState returns +0x0)
    short AnimType;                 // 0x00 [O]; [R] ctor1 sh the make-data Type (lhu +0x4); [U] updateNode switches on
                                    //      it (lh: 1 cycle, 2 oscillate, else once)
    char unk_002[0x2];              // 0x02 padding (On Tour keeps its two byte bools here)
    int AnimActive;                 // 0x04 [O] bool (word); [R] ctor1 1 (sw); [U] updateNode tests it (lw)
    int AnimComplete;               // 0x08 [O] bool (word); [R] ctor1 0 (sw $0); [U] isComplete returns it,
                                    //      getAnimSpeed / updateNode test it, updateOnce sets it at the end
    int Delay;                      // 0x0C [O]; [R] ctor1 0; [U] updateNode counts it down (lw/sw); signal 100 stores
                                    //      (int)(arg * frame rate) (cvt.w.s + swc1: an int through the FPU)
    float AnimSpeed;                // 0x10 [O]; [R] ctor1 (speed / 30 / frame rate, negated for Dir 1); [U]
                                    //      getAnimSpeed/getAnimDir/update*, signal 102 sets it
    float FromTime;                 // 0x14 [O]; [R] ctor1; [U] update* (lwc1), signal 104 sets it
    float ToTime;                   // 0x18 [O]; [R] ctor1; [U] update* (lwc1), signal 103 sets it
    // --- cAnimNode at +0x1C (0x50; object/animnode.h): cAnimNodeState (On Tour 0x14)
    float CurrTime;                 // 0x1C [O]; [R] ctor1 swc1; [U] update* advance it, signal 101 sets it
    float CacheTime;                // 0x20 [O]; [R] ctor1 = CurrTime; [U] updateNode (swc1)
    float FrameStart;               // 0x24 [O]; [U] updateNode (swc1)
    float FrameEnd;                 // 0x28 [O]; [U] updateNode / updateCycle / updateOscillate / updateOnce store the
                                    //      previous time (swc1)
    int MeshVelocityDirty;          // 0x2C [O] bool (word); [U] cAnimNode_cacheFrame / updateMeshVelocity (this + 0x10)
    // --- cMoveNode at +0x30 (0x2C; object/movenode.h): cObjNode / cTypeObjNode / cSortObjNode / cInstanceNode first
    int MoveNodeBase;               // 0x30 where the cMoveNode base starts (name ours): `&MoveNodeBase` is the
                                    //      cMoveNode subobject the methods pass on (cMoveNode_* calls, vtable
                                    //      thunks). The word is cObjNode's unnamed leading word (unk_00 in
                                    //      object/objnode.h); no ctor writes it
    cObjNode* Next;                 // 0x34 [O] cObjNode::Next; [U] bucket list
    cObjNode* Prev;                 // 0x38 [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x3C [R] cAnimObjectNode_vtbl (both ctors, the dtor); [U] updateNode calls slot 34 (expire)
    short ObjNodeType;              // 0x40 [O] cTypeObjNodeState; [R] the type arg passed to cAnimNode_cAnimNode (1; 3
                                    //      from cAnimComboNode)
    unsigned short Flags;           // 0x42 [O]; [P] bit 0 = RootTransformDirty; [R] ctor1 sets bit 0 (lhu/sh); [U]
                                    //      updateNode sets bit 0
    unsigned int SortIndex;         // 0x44 [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x48 [O] cInstanceNode::Instance; [U] signal 103/104 clamp to its model's (+0x80)
                                    //      animation length (+0x14)
    tModifierBlock* ModifierBlock;  // 0x4C [O] cMoveNode
    int Debounce;                   // 0x50 [O] cMoveNode
    float Fade;                     // 0x54 [O] cMoveNode
    int TransferHack;               // 0x58 [O] cMoveNode (bool, 4 bytes)
    // --- cAnimNode's own members (On Tour 0x3C..0x4C of cAnimNode, + 4 here relative to cAnimNode)
    int NumAnimMeshes;              // 0x5C [O]; [U] cAnimNode_setAnimMeshCache / cacheFrame (this + 0x40)
    t4Matrix* AnimMats;             // 0x60 [O]; [U] cAnimNode_getAnimMats / getAnimMat (this + 0x44)
    cMeshAnimFrame* AnimFrames;     // 0x64 [O]; [U] ctor1 tests it and reads the first frame's start time
                                    //      (cAnimFrame_getStartTime)
    tMeshVelocity* MeshVelocity;    // 0x68 [O]; [U] cAnimNode_updateMeshVelocity / updateIntersectInfo (this + 0x4C)

    // Methods bound to their retail symbols by asm labels.
    void updateCycle() __asm__("cAnimObjectNode_updateCycle");
    void updateOscillate() __asm__("cAnimObjectNode_updateOscillate");
    void updateOnce() __asm__("cAnimObjectNode_updateOnce");
    void expire(int runscript) __asm__("cAnimObjectNode_expire");
    void writeToReplayFrame(void* stream) __asm__("cAnimObjectNode_writeToReplayFrame");
    float getAnimSpeed() __asm__("cAnimObjectNode_getAnimSpeed");
    int getAnimDir() __asm__("cAnimObjectNode_getAnimDir");
    int isComplete() __asm__("cAnimObjectNode_isComplete__FPv");
};

CHECK_OFFSET(cAnimObjectNode, AnimType, 0x00);
CHECK_OFFSET(cAnimObjectNode, unk_002, 0x02);
CHECK_OFFSET(cAnimObjectNode, AnimActive, 0x04);
CHECK_OFFSET(cAnimObjectNode, AnimComplete, 0x08);
CHECK_OFFSET(cAnimObjectNode, Delay, 0x0C);
CHECK_OFFSET(cAnimObjectNode, AnimSpeed, 0x10);
CHECK_OFFSET(cAnimObjectNode, FromTime, 0x14);
CHECK_OFFSET(cAnimObjectNode, ToTime, 0x18);
CHECK_OFFSET(cAnimObjectNode, CurrTime, 0x1C);
CHECK_OFFSET(cAnimObjectNode, CacheTime, 0x20);
CHECK_OFFSET(cAnimObjectNode, FrameStart, 0x24);
CHECK_OFFSET(cAnimObjectNode, FrameEnd, 0x28);
CHECK_OFFSET(cAnimObjectNode, MeshVelocityDirty, 0x2C);
CHECK_OFFSET(cAnimObjectNode, MoveNodeBase, 0x30);
CHECK_OFFSET(cAnimObjectNode, Next, 0x34);
CHECK_OFFSET(cAnimObjectNode, Prev, 0x38);
CHECK_OFFSET(cAnimObjectNode, mVtbl, 0x3C);
CHECK_OFFSET(cAnimObjectNode, ObjNodeType, 0x40);
CHECK_OFFSET(cAnimObjectNode, Flags, 0x42);
CHECK_OFFSET(cAnimObjectNode, SortIndex, 0x44);
CHECK_OFFSET(cAnimObjectNode, Instance, 0x48);
CHECK_OFFSET(cAnimObjectNode, ModifierBlock, 0x4C);
CHECK_OFFSET(cAnimObjectNode, Debounce, 0x50);
CHECK_OFFSET(cAnimObjectNode, Fade, 0x54);
CHECK_OFFSET(cAnimObjectNode, TransferHack, 0x58);
CHECK_OFFSET(cAnimObjectNode, NumAnimMeshes, 0x5C);
CHECK_OFFSET(cAnimObjectNode, AnimMats, 0x60);
CHECK_OFFSET(cAnimObjectNode, AnimFrames, 0x64);
CHECK_OFFSET(cAnimObjectNode, MeshVelocity, 0x68);
CHECK_SIZE(cAnimObjectNode, 0x6C);

// World-script data that makes an animated object node (On Tour tMakeAnimObjectData; the first 0x28 bytes of
// tMakeAnimComboData, object/animcombonode.h, with the same names). [R] MakeAnimObjectFunc (seg/seg_1FBE38) fills a
// 44-byte block of it from its static defaults and the script arguments; [U] cAnimObjectNode_cAnimObjectNode1 reads
// Type .. ForceVisible.
struct tMakeAnimObjectData {
    unsigned int unk_000;           // 0x00 [O] tResourceID InstId (no retail read here)
    int Type;                       // 0x04 [O]; [U] cAnimObjectNode ctor: -> AnimType (lhu)
    int Dir;                        // 0x08 [O]; [U] cAnimObjectNode ctor: 1 negates AnimSpeed
    float FromTime;                 // 0x0C [O]; [U] cAnimObjectNode ctor: / 30 -> FromTime
    float ToTime;                   // 0x10 [O]; [U] cAnimObjectNode ctor: / 30 -> ToTime
    float Speed;                    // 0x14 [O]; [U] cAnimObjectNode ctor: AnimSpeed
    float SpeedRange;               // 0x18 [O]; [U] cAnimObjectNode ctor: AnimSpeed's random range
    float StartTime;                // 0x1C [O]; [U] cAnimObjectNode ctor: CurrTime
    int RandomStart;                // 0x20 [O]; [U] cAnimObjectNode ctor: CurrTime = AIrandf(From, To)
    int ForceVisible;               // 0x24 [O]; [U] cAnimObjectNode ctor: passed to cAnimNode_cAnimNode
    char unk_028[0x4];              // 0x28 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tMakeAnimObjectData, unk_000, 0x00);
CHECK_OFFSET(tMakeAnimObjectData, Type, 0x04);
CHECK_OFFSET(tMakeAnimObjectData, Dir, 0x08);
CHECK_OFFSET(tMakeAnimObjectData, FromTime, 0x0C);
CHECK_OFFSET(tMakeAnimObjectData, ToTime, 0x10);
CHECK_OFFSET(tMakeAnimObjectData, Speed, 0x14);
CHECK_OFFSET(tMakeAnimObjectData, SpeedRange, 0x18);
CHECK_OFFSET(tMakeAnimObjectData, StartTime, 0x1C);
CHECK_OFFSET(tMakeAnimObjectData, RandomStart, 0x20);
CHECK_OFFSET(tMakeAnimObjectData, ForceVisible, 0x24);
CHECK_OFFSET(tMakeAnimObjectData, unk_028, 0x28);
CHECK_SIZE(tMakeAnimObjectData, 0x2C);

#endif /* OBJECT_ANIMOBJECT_H */
