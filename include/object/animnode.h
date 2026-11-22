#ifndef OBJECT_ANIMNODE_H
#define OBJECT_ANIMNODE_H

// cAnimNode: a cMoveNode whose instance model plays a mesh animation: one cMeshAnimFrame (0xD0) per animated mesh, the
// per-mesh animation matrices and per-mesh velocities (for riders standing on it), a current/cached time and a
// frame-event window. Base of cAnimObjectNode (-> cAnimComboNode, cAnimDeltaNode) and cAnimTeeterNode. EA's source:
// d:/ssx/code/source/object/animnode.cpp (preview assert path). Retail methods: object/animnode (0x0034E448..
// 0x0034EBE0), object/flagset (ctors, dtor, cacheFrame, makeAnimMats, ... 0x0034D9B0..0x0034E3D8) and the one-liners
// in object/railmodifier (0x00361060..0x00361140).
// Size 0x50: [R] cAnimObjectNode (cMemMan_alloc(0x6C, "AnimObject") in cReplay_restoreObject) is its 0x1C-byte state
// block + cAnimNode at +0x1C (cAnimObjectNode_cAnimObjectNode1 runs cAnimNode_cAnimNode on +0x1C); cAnimNode's last
// member MeshVelocity is at 0x4C (setAnimMeshCache sw 0x4C; dtor frees it); [O] On Tour 0x50 (its members are 4 bytes
// earlier and end at 0x4C; it pads to 0x50). No allocation of the class itself.
// Bases (EA lists the state first, like every node class): cAnimNodeState (On Tour 0x14) at 0x00, then cMoveNode
// (0x2C, object/movenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode/cMoveNode) at 0x14; both written out
// inline with object/movenode.h's / object/animcombonode.h's member names. The vptr is cMoveNode's (0x14 + 0xC = 0x20).
// Constructors: cAnimNode_cAnimNode (bucket, type, record, forceVisible: cMoveNode_cMoveNode1 on +0x14, vtable,
// AnimMats = new t4Matrix[model mesh count] ("AnimMats"), setAnimMeshCache, CurrTime = CacheTime = start time,
// FrameStart = FrameEnd = 1e10, MeshVelocityDirty = 1, cacheFrame) and cAnimNode_cAnimNode1 (bucket, cBxReplayFrame*:
// cMoveNode_cMoveNode on +0x14, reads the 0x14-byte state into +0x0 and skips one word). Destructor
// cAnimNode__cAnimNode. Vtable cAnimNode_vtbl (53 slots; delta -0x14 on the overrides: 1 dtor, 12
// writeToReplayFrame, 15 updateNode, 26 hasAnimMats, 27 getAnimMats, 29 getAnimMat, 30 getAnimFrame, 32
// frameUpdateEvent, 33 makeAnimMats, 42 updateIntersectInfo, 51 updateRootTransform; the new virtuals 52 cacheFrame and
// 53 getAnimSpeed carry delta 0, relative to the cAnimNode subobject - cAnimObjectNode's table gives slot 53 delta
// -0x1C). Every method takes the whole object (this = the cAnimNodeState at +0x0).
//
// A FLAT layout like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors/dtor (src/object/flagset.cpp) and the 0x6C cAnimObjectNode allocation;
//   [P] the July 2003 preview via tools/layout_xlate.py (no accessors observed for this class);
//   [U] how retail code uses the field (src/object/animnode.cpp, flagset.cpp, railmodifier.cpp, the retail asm; also
//       cAnimObjectNode_* / cAnimComboNode_* through their cAnimNode part);
//   [O] SSX On Tour's DWARF (cAnimNodeState, cAnimNode, cMoveNode chain).

#include "layout.h"
#include "bx/math3d.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;
struct cMeshAnimFrame;
struct tMeshVelocity;

struct cAnimNode {
    // --- cAnimNodeState (On Tour 0x14; [R] the replay ctor reads these 0x14 bytes, writeToReplayFrame writes them)
    float CurrTime;                 // 0x00 [O]; [R] ctor: the first frame's start time; [U] getAnimFrame returns it *
                                    //      30
    float CacheTime;                // 0x04 [O]; [R] ctor = CurrTime; [U] cacheFrame / updateMeshVelocity pass it to the
                                    //      cMeshAnimFrame calls (lwc1 0x4)
    float FrameStart;               // 0x08 [O]; [R] ctor 1e10; [U] clearFrameEvent 1e10, frameUpdateEvent (lwc1 0x8)
    float FrameEnd;                 // 0x0C [O]; [R] ctor 1e10; [U] clearFrameEvent 1e10, frameUpdateEvent (lwc1 0xC)
    int MeshVelocityDirty;          // 0x10 [O] bool (word); [R] ctor 1; [U] cacheFrame sets 1, updateMeshVelocity
                                    //      clears it, updateIntersectInfo tests it (lw/sw 0x10)
    // --- cMoveNode at +0x14 (0x2C; object/movenode.h): cObjNode / cTypeObjNode / cSortObjNode / cInstanceNode first
    int unk_014;                    // 0x14 cObjNode's leading word (unk_00 in object/objnode.h); no ctor writes it
    cObjNode* Next;                 // 0x18 [O] cObjNode::Next; [U] bucket list
    cObjNode* Prev;                 // 0x1C [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x20 [R] cAnimNode_vtbl (both ctors, the dtor); [U] getAnimMats/getAnimMat/
                                    //      updateRootTransform/updateMeshVelocity call slots 33 / 53 through it
    short ObjNodeType;              // 0x24 [O] cTypeObjNodeState; [R] the type arg passed to cMoveNode_cMoveNode1
    unsigned short Flags;           // 0x26 [O]; [P] bit 0 = RootTransformDirty; [U] getAnimMats/getAnimMat/
                                    //      updateMeshVelocity test bit 0 (lhu 0x26), updateRootTransform sets it (sh)
    unsigned int SortIndex;         // 0x28 [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x2C [O] cInstanceNode::Instance; [U] the ctors / setAnimMeshCache / makeAnimMats
                                    //      read its model (+0x80); calcPosition compares it with its record argument;
                                    //      cAnimNode_frameUpdate (frameUpdate) runs its frame-update script
    tModifierBlock* ModifierBlock;  // 0x30 [O] cMoveNode
    int Debounce;                   // 0x34 [O] cMoveNode
    float Fade;                     // 0x38 [O] cMoveNode
    int TransferHack;               // 0x3C [O] cMoveNode (bool, 4 bytes)
    // --- cAnimNode (On Tour 0x3C..0x4C, + 4 here)
    int NumAnimMeshes;              // 0x40 [O]; [U] setAnimMeshCache counts the model's animated meshes into it (sw /
                                    //      lw 0x40); cacheFrame loops over it
    t4Matrix* AnimMats;             // 0x44 [O]; [R] ctors: operator new(model mesh count * 0x40, "AnimMats"); dtor
                                    //      frees it; [U] getAnimMats returns it, getAnimMat / updateMeshVelocity index
                                    //      it
    cMeshAnimFrame* AnimFrames;     // 0x48 [O]; [U] setAnimMeshCache: new cMeshAnimFrame[NumAnimMeshes] (0xD0 each,
                                    //      tag "IntersectInfo") or 0; the dtor deletes the array; cacheFrame,
                                    //      getFrameMat, the ctor (cAnimFrame_getStartTime) read it
    tMeshVelocity* MeshVelocity;    // 0x4C [O]; [U] setAnimMeshCache: new 0x20-byte entries[model mesh count]
                                    //      ("MeshVelocity"); updateMeshVelocity fills it, updateIntersectInfo reads it;
                                    //      the dtor frees it
};

CHECK_OFFSET(cAnimNode, CurrTime, 0x00);
CHECK_OFFSET(cAnimNode, CacheTime, 0x04);
CHECK_OFFSET(cAnimNode, FrameStart, 0x08);
CHECK_OFFSET(cAnimNode, FrameEnd, 0x0C);
CHECK_OFFSET(cAnimNode, MeshVelocityDirty, 0x10);
CHECK_OFFSET(cAnimNode, unk_014, 0x14);
CHECK_OFFSET(cAnimNode, Next, 0x18);
CHECK_OFFSET(cAnimNode, Prev, 0x1C);
CHECK_OFFSET(cAnimNode, mVtbl, 0x20);
CHECK_OFFSET(cAnimNode, ObjNodeType, 0x24);
CHECK_OFFSET(cAnimNode, Flags, 0x26);
CHECK_OFFSET(cAnimNode, SortIndex, 0x28);
CHECK_OFFSET(cAnimNode, Instance, 0x2C);
CHECK_OFFSET(cAnimNode, ModifierBlock, 0x30);
CHECK_OFFSET(cAnimNode, Debounce, 0x34);
CHECK_OFFSET(cAnimNode, Fade, 0x38);
CHECK_OFFSET(cAnimNode, TransferHack, 0x3C);
CHECK_OFFSET(cAnimNode, NumAnimMeshes, 0x40);
CHECK_OFFSET(cAnimNode, AnimMats, 0x44);
CHECK_OFFSET(cAnimNode, AnimFrames, 0x48);
CHECK_OFFSET(cAnimNode, MeshVelocity, 0x4C);
CHECK_SIZE(cAnimNode, 0x50);

#endif /* OBJECT_ANIMNODE_H */
