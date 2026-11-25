#ifndef OBJECT_FLOATINGNODE_H
#define OBJECT_FLOATINGNODE_H

// cFloatingNode: a cMoveNode that bobs and tilts on a spring (floating props on water: buoys, logs); its state is a
// damped vertical spring (Z*) plus two tilt springs (O*/W*/T* about X and Y). EA's source:
// d:/ssx/code/source/object/floatingnode.cpp. Size 0x7C: retail cMemMan_alloc(0x7C, "Floating") for both constructors
// (cFloatingNode_cFloatingNode from the world script loader src/seg/seg_1FBE38.cpp, cFloatingNode_cFloatingNode1 from the replay loader
// src/replay/replay.cpp); On Tour 0x78 (its cMoveNode is 4 bytes smaller). Destructor cFloatingNode__cFloatingNode;
// vtable cFloatingNode_vtbl (0x0048F858, 51 slots). Methods: src/object/floatingnode.cpp, ctors/dtor in the
// object/animnode splat unit, the one-liners in src/object/railmodifier.cpp (methods.txt).
// Derives from cMoveNode (0x00-0x2C, object/movenode.h), then FLAT: cFloatingNodeState (0x2C-0x74; On Tour's 18 floats
// in order, each On Tour offset + 4) + AnimMats + Info. getNodeState (0x361768) returns +0x2C, the state.

#include "layout.h"
#include "object/movenode.h"
#include "world/worldview.h"
#include "bx/math3d.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;
struct tFloatBoundBoxInfo;

struct cBxReplayFrame;

struct cFloatingNode : public cMoveNode {
    // cMoveNode (object/movenode.h, 0x2C): the vptr at 0x0C:
    //   [R] each ctor stores its vtable; [U] every virtual call (cMoveNode_apply, update, render, ...)
    // --- cFloatingNodeState (0x48; getNodeState returns +0x2C)
    float ZSpring;                  // 0x2C [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float ZDamp;                    // 0x30 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float ForceScale;               // 0x34 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float ASpring;                  // 0x38 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float ADamp;                    // 0x3C [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float TorqueScale;              // 0x40 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float Force;                    // 0x44 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float Vel;                      // 0x48 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float Zo;                       // 0x4C [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float Z;                        // 0x50 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float OX;                       // 0x54 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float OoX;                      // 0x58 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float WX;                       // 0x5C [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float TX;                       // 0x60 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float OY;                       // 0x64 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float OoY;                      // 0x68 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float WY;                       // 0x6C [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    float TY;                       // 0x70 [O] cFloatingNodeState; [R] ctor cFloatingNode_cFloatingNode; [U] updateNode
    // --- cFloatingNode
    t4Matrix* AnimMats;             // 0x74 [O]; [U] getAnimMats/getAnimMat/updateRootTransform (railmodifier.cpp), the
                                    //      dtor
    tFloatBoundBoxInfo* Info;       // 0x78 [O]; [R] cFloatingNode_initInfo: cMemMan_alloc(0x30, "FloatingBoundBox"); [U]
                                    //      getBoundBox, purgeInfo, update/restoreOverlapSystem

    // Methods bound to their retail symbols by asm labels.
    void* cFloatingNode1(void* bucket, cBxReplayFrame* stream) __asm__("cFloatingNode_cFloatingNode1");
};

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

// cFloatingNodeState (On Tour, 0x48): the spring state cFloatingNode carries at +0x2C (ZSpring .. TY above, written
// out inline there). The tilt helpers take &self->ZSpring as one (views sFloatTilt_34F930 / sFloatTilt_F120:
// pos2 = OX, pos3 = OY).
struct cFloatingNodeState {
    float ZSpring;                  // 0x00 [O]
    float ZDamp;                    // 0x04 [O]
    float ForceScale;               // 0x08 [O]
    float ASpring;                  // 0x0C [O]
    float ADamp;                    // 0x10 [O]
    float TorqueScale;              // 0x14 [O]
    float Force;                    // 0x18 [O]
    float Vel;                      // 0x1C [O]
    float Zo;                       // 0x20 [O]
    float Z;                        // 0x24 [O]
    float OX;                       // 0x28 [O]
    float OoX;                      // 0x2C [O]
    float WX;                       // 0x30 [O]
    float TX;                       // 0x34 [O]
    float OY;                       // 0x38 [O]
    float OoY;                      // 0x3C [O]
    float WY;                       // 0x40 [O]
    float TY;                       // 0x44 [O]
};

CHECK_OFFSET(cFloatingNodeState, ZSpring, 0x00);
CHECK_OFFSET(cFloatingNodeState, ZDamp, 0x04);
CHECK_OFFSET(cFloatingNodeState, ForceScale, 0x08);
CHECK_OFFSET(cFloatingNodeState, ASpring, 0x0C);
CHECK_OFFSET(cFloatingNodeState, ADamp, 0x10);
CHECK_OFFSET(cFloatingNodeState, TorqueScale, 0x14);
CHECK_OFFSET(cFloatingNodeState, Force, 0x18);
CHECK_OFFSET(cFloatingNodeState, Vel, 0x1C);
CHECK_OFFSET(cFloatingNodeState, Zo, 0x20);
CHECK_OFFSET(cFloatingNodeState, Z, 0x24);
CHECK_OFFSET(cFloatingNodeState, OX, 0x28);
CHECK_OFFSET(cFloatingNodeState, OoX, 0x2C);
CHECK_OFFSET(cFloatingNodeState, WX, 0x30);
CHECK_OFFSET(cFloatingNodeState, TX, 0x34);
CHECK_OFFSET(cFloatingNodeState, OY, 0x38);
CHECK_OFFSET(cFloatingNodeState, OoY, 0x3C);
CHECK_OFFSET(cFloatingNodeState, WY, 0x40);
CHECK_OFFSET(cFloatingNodeState, TY, 0x44);
CHECK_SIZE(cFloatingNodeState, 0x48);

// tFloatBoundBoxInfo (On Tour, 0x30): the floating bound box Info points at. [R] cMemMan_alloc(0x30,
// "FloatingBoundBox"); [U] view sFlInfo_34EF40 (the box, then the radius at 0x20).
struct tFloatBoundBoxInfo {
    tBoundBox BoundBox;             // 0x00 [O]
    float Radius;                   // 0x20 [O]
    char unk_024[0xC];              // 0x24 padding (16-byte aligned)
};

CHECK_OFFSET(tFloatBoundBoxInfo, BoundBox, 0x00);
CHECK_OFFSET(tFloatBoundBoxInfo, Radius, 0x20);
CHECK_SIZE(tFloatBoundBoxInfo, 0x30);

#endif /* OBJECT_FLOATINGNODE_H */
