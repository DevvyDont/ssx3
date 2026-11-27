#ifndef OBJECT_ANIMCOMBONODE_H
#define OBJECT_ANIMCOMBONODE_H

// cAnimComboNode: an animated world object (cAnimObjectNode) that, on signal 130, freezes its current mesh matrices
// (ComboXform) and plays a second time range on top of them (FromTime2..ToTime2 at AnimSpeed2), optionally stopping
// there. Built by the world script's MakeAnimComboFunc (src/seg/seg_1FBE38.cpp) and the replay loader. No EA source
// path in the preview's asserts (its siblings are in object/animnode.cpp); retail methods are in the
// object/multiparticle splat unit (0x358C80..0x359224) plus getNodeState/getAnimDir in object/railmodifier
// (0x361D88, 0x361D90).
// Size 0x88: retail cMemMan_alloc(0x88, "AnimCombo") in MakeAnimComboFunc and cReplay_restoreObject; On Tour 0x88.
//
// EA's node classes put each class's state block (cXxxNodeState, a base listed before the node base) in front of the
// node, so the chain cAnimComboNode -> cAnimObjectNode -> cAnimNode -> cMoveNode lays out as:
//   0x00 cAnimComboNodeState (0x18)   0x18 cAnimObjectNode = cAnimObjectNodeState (0x1C) + cAnimNode
//   0x34 cAnimNode = cAnimNodeState (0x14) + cMoveNode   0x48 cMoveNode (0x2C, object/movenode.h)
//   0x74 cAnimNode's own members      0x84 cAnimComboNode's own member
// [R] the ctors build cAnimObjectNode on +0x18 (cAnimObjectNode_cAnimObjectNode1 / _cAnimObjectNode), store the vtable
// at
// +0x54, the replay ctor and writeToReplayFrame move 0x18 bytes at +0x0, cacheFrame calls cAnimNode_cacheFrame(+0x34),
// updateIntersectInfo/loseFocus call cMoveNode/cInstanceNode methods on +0x48; the vtable cAnimComboNode_vtbl (52 slots) gives
// cAnimComboNode's slots delta -0x48, cAnimObjectNode's -0x30 and cAnimNode's -0x14 (from the cMoveNode part).
// getNodeState returns this (the state block). Retail bools are words except cAnimComboNodeState's chars, so
// cAnimObjectNodeState is 0x1C (On Tour 0x14) and cObjNode has its extra leading word: cAnimNode starts at 0x34 (On
// Tour 0x2C), cMoveNode at 0x48 (0x40), cAnimNode's own members at 0x74 (0x68) and ComboXform at 0x84 (0x80; On Tour
// pads cAnimObjectNode from 0x64 to 0x68, retail has no gap). Every retail offset is placed by [R]/[U] evidence.
// All bases are written out inline (cMoveNode with object/movenode.h's member names).
// Evidence tags:
//   [R] retail ctors cAnimComboNode_cAnimComboNode1 (bucket, record, tMakeAnimComboData*) and
// cAnimComboNode_cAnimComboNode (replay),
//       destructor cAnimComboNode__cAnimComboNode, the 0x88 allocation;
//   [P] preview offsets via tools/layout_xlate.py: preview == retail up to 0x5A, preview 0x78/0x80 -> retail 0x7C/0x84
//       (no accessors);
//   [U] how retail code uses the field (src/object/multiparticle.cpp, railmodifier.cpp and the retail asm; for the base
//       parts also cAnimObjectNode_* (this = +0x18) and cAnimNode_* (this = +0x34));
//   [O] SSX On Tour's DWARF (cAnimComboNodeState, cAnimObjectNodeState, cAnimNodeState, cAnimNode, cAnimComboNode).

#include "layout.h"
#include "wscript/wscriptnull.h"
#include "bx/math3d.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;
struct cMeshAnimFrame;
struct tMeshVelocity;

// World-script data that makes an animation combo node (On Tour tMakeAnimComboData, 0x3C; same offsets): On Tour's
// tMakeAnimObjectData fields (0x00..0x27) followed by the second phase. [O] On Tour's cAnimComboNode(cBucketList*,
// tInstanceRecord*, const tMakeAnimComboData*); [U] cAnimComboNode_cAnimComboNode1 (view sMPDesc8C80) passes it to
// cAnimObjectNode_cAnimObjectNode1, which reads Type .. ForceVisible (object/animobject.h), then sets FromTime2 /
// ToTime2 / AnimSpeed2 / Stop / StopInPlace from the second-phase fields.
struct tMakeAnimComboData {
    unsigned int InstId;            // 0x00 [O] tResourceID; [U] MakeAnimComboFunc's instance lookup (-1: the script's
                                    //      current instance)
    int Type;                       // 0x04 [O]; [U] cAnimObjectNode ctor: -> AnimType
    int Dir;                        // 0x08 [O]; [U] cAnimObjectNode ctor: 1 negates AnimSpeed
    float FromTime;                 // 0x0C [O]; [U] cAnimObjectNode ctor: / 30 -> FromTime
    float ToTime;                   // 0x10 [O]; [U] cAnimObjectNode ctor: / 30 -> ToTime; combo ctor: FromTime2's
                                    //      fallback
    float Speed;                    // 0x14 [O]; [U] cAnimObjectNode ctor: AnimSpeed
    float SpeedRange;               // 0x18 [O]; [U] cAnimObjectNode ctor: AnimSpeed's random range
    float StartTime;                // 0x1C [O]; [U] cAnimObjectNode ctor: CurrTime
    int RandomStart;                // 0x20 [O]; [U] cAnimObjectNode ctor: CurrTime = AIrandf(From, To)
    int ForceVisible;               // 0x24 [O]; [U] cAnimObjectNode ctor: passed to cAnimNode_cAnimNode
    float FromTime2;                // 0x28 [O]; [U] / 30 -> FromTime2 (< 0: ToTime / 30)
    float ToTime2;                  // 0x2C [O]; [U] / 30 -> ToTime2 (< 0: the model's m_fAnimTime)
    float Speed2;                   // 0x30 [O]; [U] / 30 / TimerHz -> AnimSpeed2
    int Stop;                       // 0x34 [O]; [U] != 0 -> Stop, < 0 -> StopInPlace
    WScriptNullType null;           // 0x38 [O]
    char unk_039[0x3];              // 0x39 padding
};

CHECK_OFFSET(tMakeAnimComboData, InstId, 0x00);
CHECK_OFFSET(tMakeAnimComboData, Type, 0x04);
CHECK_OFFSET(tMakeAnimComboData, Dir, 0x08);
CHECK_OFFSET(tMakeAnimComboData, FromTime, 0x0C);
CHECK_OFFSET(tMakeAnimComboData, ToTime, 0x10);
CHECK_OFFSET(tMakeAnimComboData, Speed, 0x14);
CHECK_OFFSET(tMakeAnimComboData, SpeedRange, 0x18);
CHECK_OFFSET(tMakeAnimComboData, StartTime, 0x1C);
CHECK_OFFSET(tMakeAnimComboData, RandomStart, 0x20);
CHECK_OFFSET(tMakeAnimComboData, ForceVisible, 0x24);
CHECK_OFFSET(tMakeAnimComboData, FromTime2, 0x28);
CHECK_OFFSET(tMakeAnimComboData, ToTime2, 0x2C);
CHECK_OFFSET(tMakeAnimComboData, Speed2, 0x30);
CHECK_OFFSET(tMakeAnimComboData, Stop, 0x34);
CHECK_OFFSET(tMakeAnimComboData, null, 0x38);
CHECK_SIZE(tMakeAnimComboData, 0x3C);

struct cBxReplayFrame;
struct tIntersectInfo;

struct cAnimComboNode {
    // The signal that starts the second time range (name ours): [U] signal handles it and passes any other signal to
    // cAnimObjectNode_signal.
    enum { kAnimComboSignal_Start2 = 130 };
    // --- cAnimComboNodeState (On Tour 0x18; getNodeState returns +0x0; replay ctor/writeToReplayFrame move these 0x18
    //       bytes)
    float AnimSpeed2;               // 0x00 [O]; [R] ctor: Speed2 / 30 / frame rate; [U] updateNode adds it to
                                    //       CurrTime2,
                                    //       getAnimDir tests its sign while AnimActive2
    float CurrTime2;                // 0x04 [O]; [R] ctor 0 (sw $0); [U] updateNode advances/clamps it, signal sets it
                                    //       to FromTime2
    float FromTime2;                // 0x08 [O]; [R] ctor: FromTime2 / 30 (or ToTime when negative); [U] signal reads it
    float ToTime2;                  // 0x0C [O]; [R] ctor: ToTime2 / 30 (or the animation length); [U] updateNode: end
                                    //       of the range
    float ComboTime;                // 0x10 [O]; [U] signal stores cAnimNodeState::CacheTime (+0x38) into it
    char AnimActive2;               // 0x14 [O] char; [R] ctor 0 (sb); [U] signal sets 1, updateNode clears it at the
                                    //       end;
                                    //       cacheFrame/getAnimDir/updateNode test it (lb)
    char AnimComplete2;             // 0x15 [O] char; [R] ctor 0; [U] updateNode sets it when Stop; loseFocus/cacheFrame
                                    //       test it
    char Stop;                      // 0x16 [O] char; [R] ctor: Stop != 0; [U] updateNode
    char StopInPlace;               // 0x17 [O] char; [R] ctor: Stop < 0; [U] cacheFrame
    // --- cAnimObjectNode at +0x18: cAnimObjectNodeState (On Tour 0x14, retail 0x1C)
    short AnimType;                 // 0x18 [O]; [U] cAnimObjectNode_cAnimObjectNode1/signal sh, updateNode lh (this +
                                    //       0x0)
    char unk_01A[0x2];              // 0x1A padding
    int AnimActive;                 // 0x1C [O] bool (word); [U] cAnimObjectNode_updateNode/cAnimObjectNode1 (this +
                                    //       0x4)
    int AnimComplete;               // 0x20 [O] bool (word); [U] cAnimObjectNode_isComplete (this + 0x8)
    int Delay;                      // 0x24 [O]; [U] cAnimComboNode_updateNode counts it down (lw/sw);
                                    //       cAnimObjectNode_updateNode (this + 0xC)
    float AnimSpeed;                // 0x28 [O]; [U] getAnimDir tests its sign; cAnimObjectNode_getAnimSpeed/update*
                                    //       (this + 0x10)
    float FromTime;                 // 0x2C [O]; [U] cAnimObjectNode_update* (this + 0x14)
    float ToTime;                   // 0x30 [O]; [U] cAnimObjectNode_update* (this + 0x18)
    // --- cAnimNode at +0x34: cAnimNodeState (On Tour 0x14)
    float CurrTime;                 // 0x34 [O]; [U] updateNode copies it to FrameStart/FrameEnd; cAnimNode_getAnimFrame
                                    //       (this + 0x0)
    float CacheTime;                // 0x38 [O]; [U] signal copies it to ComboTime; cAnimNode_cacheFrame (this + 0x4)
    float FrameStart;               // 0x3C [O]; [U] updateNode stores 1e10 / CurrTime; cAnimNode_clearFrameEvent (this
                                    //       + 0x8)
    float FrameEnd;                 // 0x40 [O]; [U] as FrameStart (this + 0xC)
    int MeshVelocityDirty;          // 0x44 [O] bool (word); [U] cAnimNode_cacheFrame / updateMeshVelocity (this + 0x10)
    // --- cMoveNode at +0x48 (0x2C; object/movenode.h): cObjNode / cTypeObjNode / cSortObjNode / cInstanceNode first
    int MoveNodeBase;               // 0x48 where the cMoveNode base starts (name ours): `&MoveNodeBase` is the
                                    //      cMoveNode subobject the methods pass on (cMoveNode_* calls, vtable
                                    //      thunks). The word is cObjNode's unnamed leading word (unk_00 in
                                    //      object/objnode.h); no ctor writes it
    cObjNode* Next;                 // 0x4C [O] cObjNode::Next; [U] bucket list
    cObjNode* Prev;                 // 0x50 [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x54 [R] cAnimComboNode_vtbl (both ctors and the dtor)
    short ObjNodeType;              // 0x58 [P] getType (lh); [O] cTypeObjNodeState; [R] node type 3 (passed to
                                    //       cAnimObjectNode)
    unsigned short Flags;           // 0x5A [O]; [P] bit 0 = RootTransformDirty; [U] updateNode sets bit 0 (lhu/sh)
    unsigned int SortIndex;         // 0x5C [P] getSortIndex; [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x60 [P] getInstance; [O]; [U] setComboXform/cacheFrame read its model (+0x80)
    tModifierBlock* ModifierBlock;  // 0x64 [O] cMoveNode
    int Debounce;                   // 0x68 [O] cMoveNode
    float Fade;                     // 0x6C [O] cMoveNode
    int TransferHack;               // 0x70 [O] cMoveNode (bool, 4 bytes)
    // --- cAnimNode's own members (On Tour 0x3C..0x4C, + 4 here relative to cAnimNode)
    int NumAnimMeshes;              // 0x74 [O]; [U] ctors size ComboXform by it (n << 6), setComboXform returns when <=
                                    //       0
    t4Matrix* AnimMats;             // 0x78 [O]; [U] cAnimNode_getAnimMats/getAnimMat (this + 0x44)
    cMeshAnimFrame* AnimFrames;     // 0x7C [O]; [U] cacheFrame tests it and transforms each frame (0xD0 each);
                                    //       setComboXform copies each frame's matrix (+0x90)
    tMeshVelocity* MeshVelocity;    // 0x80 [O]; [U] cAnimNode_updateMeshVelocity / updateIntersectInfo (this + 0x4C)
    // --- cAnimComboNode
    t4Matrix* ComboXform;           // 0x84 [O]; [R] ctors: operator new(NumAnimMeshes * 0x40, "ComboXform"); dtor frees
                                    //       it;
                                    //       [U] setComboXform fills it, cacheFrame applies it
                                    //       (cMeshAnimFrame_transform)

    // Methods bound to their retail symbols by asm labels.
    bool updateNode() __asm__("cAnimComboNode_updateNode");
    void updateIntersectInfo(tIntersectInfo* out) __asm__("cAnimComboNode_updateIntersectInfo");
    void signal(int msg) __asm__("cAnimComboNode_signal");
    void setComboXform() __asm__("cAnimComboNode_setComboXform");
    void loseFocus() __asm__("cAnimComboNode_loseFocus");
    void writeToReplayFrame(cBxReplayFrame* obj) __asm__("cAnimComboNode_writeToReplayFrame");
    void cacheFrame() __asm__("cAnimComboNode_cacheFrame");
    int getAnimDir() __asm__("cAnimComboNode_getAnimDir");
    void* getNodeState() __asm__("cAnimComboNode_getNodeState__FPv");
};

CHECK_OFFSET(cAnimComboNode, AnimSpeed2, 0x00);
CHECK_OFFSET(cAnimComboNode, CurrTime2, 0x04);
CHECK_OFFSET(cAnimComboNode, FromTime2, 0x08);
CHECK_OFFSET(cAnimComboNode, ToTime2, 0x0C);
CHECK_OFFSET(cAnimComboNode, ComboTime, 0x10);
CHECK_OFFSET(cAnimComboNode, AnimActive2, 0x14);
CHECK_OFFSET(cAnimComboNode, AnimComplete2, 0x15);
CHECK_OFFSET(cAnimComboNode, Stop, 0x16);
CHECK_OFFSET(cAnimComboNode, StopInPlace, 0x17);
CHECK_OFFSET(cAnimComboNode, AnimType, 0x18);
CHECK_OFFSET(cAnimComboNode, unk_01A, 0x1A);
CHECK_OFFSET(cAnimComboNode, AnimActive, 0x1C);
CHECK_OFFSET(cAnimComboNode, AnimComplete, 0x20);
CHECK_OFFSET(cAnimComboNode, Delay, 0x24);
CHECK_OFFSET(cAnimComboNode, AnimSpeed, 0x28);
CHECK_OFFSET(cAnimComboNode, FromTime, 0x2C);
CHECK_OFFSET(cAnimComboNode, ToTime, 0x30);
CHECK_OFFSET(cAnimComboNode, CurrTime, 0x34);
CHECK_OFFSET(cAnimComboNode, CacheTime, 0x38);
CHECK_OFFSET(cAnimComboNode, FrameStart, 0x3C);
CHECK_OFFSET(cAnimComboNode, FrameEnd, 0x40);
CHECK_OFFSET(cAnimComboNode, MeshVelocityDirty, 0x44);
CHECK_OFFSET(cAnimComboNode, MoveNodeBase, 0x48);
CHECK_OFFSET(cAnimComboNode, Next, 0x4C);
CHECK_OFFSET(cAnimComboNode, Prev, 0x50);
CHECK_OFFSET(cAnimComboNode, mVtbl, 0x54);
CHECK_OFFSET(cAnimComboNode, ObjNodeType, 0x58);
CHECK_OFFSET(cAnimComboNode, Flags, 0x5A);
CHECK_OFFSET(cAnimComboNode, SortIndex, 0x5C);
CHECK_OFFSET(cAnimComboNode, Instance, 0x60);
CHECK_OFFSET(cAnimComboNode, ModifierBlock, 0x64);
CHECK_OFFSET(cAnimComboNode, Debounce, 0x68);
CHECK_OFFSET(cAnimComboNode, Fade, 0x6C);
CHECK_OFFSET(cAnimComboNode, TransferHack, 0x70);
CHECK_OFFSET(cAnimComboNode, NumAnimMeshes, 0x74);
CHECK_OFFSET(cAnimComboNode, AnimMats, 0x78);
CHECK_OFFSET(cAnimComboNode, AnimFrames, 0x7C);
CHECK_OFFSET(cAnimComboNode, MeshVelocity, 0x80);
CHECK_OFFSET(cAnimComboNode, ComboXform, 0x84);
CHECK_SIZE(cAnimComboNode, 0x88);

#endif /* OBJECT_ANIMCOMBONODE_H */
