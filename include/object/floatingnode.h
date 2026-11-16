#ifndef OBJECT_FLOATINGNODE_H
#define OBJECT_FLOATINGNODE_H

// cFloatingNode: a cMoveNode that bobs and tilts on a spring (floating props on water: buoys, logs); its state is a
// damped vertical spring (Z*) plus two tilt springs (O*/W*/T* about X and Y). EA's source:
// d:/ssx/code/source/object/floatingnode.cpp. Size 0x7C: retail cMemMan_alloc(0x7C, "Floating") for both constructors
// (func_0034EC58 from the world script loader src/seg/seg_1FBE38.cpp, func_0034ED88 from the replay loader
// src/replay/replay.cpp); On Tour 0x78 (its cMoveNode is 4 bytes smaller). Destructor cFloatingNode__cFloatingNode;
// vtable cFloatingNode_vtbl (0x0048F858, 51 slots). Methods: src/object/floatingnode.cpp, ctors/dtor in the
// object/animnode splat unit, the one-liners in src/object/railmodifier.cpp (methods.txt).
// A FLAT layout: cMoveNode (0x00-0x2C, object/movenode.h, inline) + cFloatingNodeState (0x2C-0x74; On Tour's 18 floats
// in order, each On Tour offset + 4) + AnimMats + Info. getNodeState (0x361768) returns +0x2C, the state.

#include "layout.h"
#include "bx/math3d.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;
struct tFloatBoundBoxInfo;

struct cFloatingNode {
    // --- cMoveNode (0x2C; object/movenode.h)
    int unk_00;                     // 0x00 never written by any node ctor (see object/instancenode.h)
    cObjNode* Next;                 // 0x04 [O] cObjNode::Next; [P] cObjNode() zeroes it; [U] bucket list
    cObjNode* Prev;                 // 0x08 [O] cObjNode::Prev; [P]; [U] bucket list
    void** mVtbl;                   // 0x0C [R] each ctor stores its vtable; [U] every virtual call (cMoveNode_apply,
                                    //      update, render, ...)
    short ObjNodeType;              // 0x10 [P] getType (lh 0x10); [O] cTypeObjNodeState
    unsigned short Flags;           // 0x12 [O]; [P] bit 0 = RootTransformDirty; [R] cMoveNode_cMoveNode sets bit 0; [U]
                                    //      cMoveNode_updateRootTransform, preUpdateModifierBlock
    unsigned int SortIndex;         // 0x14 [P] getSortIndex (lw 0x14); [O] cSortObjNodeState
    tInstanceRecord* Instance;      // 0x18 [P] getInstance (lw 0x18); [O]; [U] the add* builders pass it to the modifier
                                    //      ctors, render, getRootTransform
    tModifierBlock* ModifierBlock;  // 0x1C [O]; [R] cMoveNode_addModifierBlock allocates it (cMemMan_alloc(0x28,
                                    //      "ModifierBlock")); [U] every forwarder (func_00360DE8..func_00361010, add*/rem*,
                                    //      update, render)
    int Debounce;                   // 0x20 [O] int; [U] setDebounce stores (int)(seconds * fps) (cvt.w.s + swc1),
                                    //      update/onCollide count it down (lw/sw); [R] replay ctor reads 4 bytes
    float Fade;                     // 0x24 [O]; [U] addFade func_00355878 stores data +0x4, render/renderModel read it
                                    //      (lwc1); [R] replay ctor
    int TransferHack;               // 0x28 [O] bool (4 bytes); [U] render tests it (lw); [R] replay ctor
    // --- cFloatingNodeState (0x48; getNodeState returns +0x2C)
    float ZSpring;                  // 0x2C [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float ZDamp;                    // 0x30 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float ForceScale;               // 0x34 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float ASpring;                  // 0x38 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float ADamp;                    // 0x3C [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float TorqueScale;              // 0x40 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float Force;                    // 0x44 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float Vel;                      // 0x48 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float Zo;                       // 0x4C [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float Z;                        // 0x50 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float OX;                       // 0x54 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float OoX;                      // 0x58 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float WX;                       // 0x5C [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float TX;                       // 0x60 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float OY;                       // 0x64 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float OoY;                      // 0x68 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float WY;                       // 0x6C [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    float TY;                       // 0x70 [O] cFloatingNodeState; [R] ctor func_0034EC58; [U] updateNode
    // --- cFloatingNode
    t4Matrix* AnimMats;             // 0x74 [O]; [U] getAnimMats/getAnimMat/updateRootTransform (railmodifier.cpp), the
                                    //      dtor
    tFloatBoundBoxInfo* Info;       // 0x78 [O]; [R] cFloatingNode_initInfo: cMemMan_alloc(0x30, "FloatingBoundBox"); [U]
                                    //      getBoundBox, purgeInfo, update/restoreOverlapSystem
};

CHECK_OFFSET(cFloatingNode, unk_00, 0x00);
CHECK_OFFSET(cFloatingNode, Next, 0x04);
CHECK_OFFSET(cFloatingNode, Prev, 0x08);
CHECK_OFFSET(cFloatingNode, mVtbl, 0x0C);
CHECK_OFFSET(cFloatingNode, ObjNodeType, 0x10);
CHECK_OFFSET(cFloatingNode, Flags, 0x12);
CHECK_OFFSET(cFloatingNode, SortIndex, 0x14);
CHECK_OFFSET(cFloatingNode, Instance, 0x18);
CHECK_OFFSET(cFloatingNode, ModifierBlock, 0x1C);
CHECK_OFFSET(cFloatingNode, Debounce, 0x20);
CHECK_OFFSET(cFloatingNode, Fade, 0x24);
CHECK_OFFSET(cFloatingNode, TransferHack, 0x28);
CHECK_OFFSET(cFloatingNode, ZSpring, 0x2C);
CHECK_OFFSET(cFloatingNode, ZDamp, 0x30);
CHECK_OFFSET(cFloatingNode, ForceScale, 0x34);
CHECK_OFFSET(cFloatingNode, ASpring, 0x38);
CHECK_OFFSET(cFloatingNode, ADamp, 0x3C);
CHECK_OFFSET(cFloatingNode, TorqueScale, 0x40);
CHECK_OFFSET(cFloatingNode, Force, 0x44);
CHECK_OFFSET(cFloatingNode, Vel, 0x48);
CHECK_OFFSET(cFloatingNode, Zo, 0x4C);
CHECK_OFFSET(cFloatingNode, Z, 0x50);
CHECK_OFFSET(cFloatingNode, OX, 0x54);
CHECK_OFFSET(cFloatingNode, OoX, 0x58);
CHECK_OFFSET(cFloatingNode, WX, 0x5C);
CHECK_OFFSET(cFloatingNode, TX, 0x60);
CHECK_OFFSET(cFloatingNode, OY, 0x64);
CHECK_OFFSET(cFloatingNode, OoY, 0x68);
CHECK_OFFSET(cFloatingNode, WY, 0x6C);
CHECK_OFFSET(cFloatingNode, TY, 0x70);
CHECK_OFFSET(cFloatingNode, AnimMats, 0x74);
CHECK_OFFSET(cFloatingNode, Info, 0x78);
CHECK_SIZE(cFloatingNode, 0x7C);

#endif /* OBJECT_FLOATINGNODE_H */
