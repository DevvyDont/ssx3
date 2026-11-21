#ifndef OBJECT_ROLLERMODIFIER_H
#define OBJECT_ROLLERMODIFIER_H

// cRollerModifier: the transform modifier that turns a placed object into a rolling/tumbling rigid body (the boulders
// and props a rider knocks loose; transform type 5). Size 0x2D0: retail cMemMan_alloc(0x2D0, "RollerModifier")
// (cMoveNode's add-roller path, src/object/movenode.cpp, and the replay loader, src/object/modifierblock.cpp
// type 5), On Tour 0x2D0 too. EA's source path: d:/ssx/code/source/object/rollermodifier.cpp (preview assert).
// Constructors: cRollerModifier_cRollerModifier (tAddRollerData, tIntersectInfo*, tCollideInfo*) and cRollerModifier_cRollerModifier1
// (from a replay frame); destructor cRollerModifier__cRollerModifier; vtable cRollerModifier_vtbl (0x0048F080, 27
// slots; the cTransformModifier base vtable is cTransformModifier_vtbl). Methods are in src/object/railmodifier.cpp.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. The real bases are
//   cTransformModifier (0x000: vptr only) + cRollerNodeState (0x004-0x02C) + cRigidBody (0x030-0x0D0, see
//   object/rigidbody.h, itself cRigidBodyState 0x00-0x50 + cRigidBody), then cRollerModifier's own fields.
// On Tour's DWARF lists only cRollerModifier's own members (from 0xD0); its writeToReplayFrame writes cRigidBody at
// +0x30 and then 0x28 bytes from +0x4, as retail's cRollerModifier_writeToReplayFrame does, so the 0x28-byte block at +0x4 is the
// cRollerNodeState (On Tour: 0x28 bytes, the 10 floats below in this order).
// Evidence tags:
//   [R] retail constructors cRollerModifier_cRollerModifier / cRollerModifier_cRollerModifier1, the destructor, the 0x2D0 allocation;
//   [P] the July 2003 preview's inline accessors (getRadius/setRadius lwc1/swc1 0x24); tools/layout_xlate.py finds
//       preview == retail for this class (0x4..0xD0, 0x220, 0x230 all shift 0);
//   [U] how retail code uses the field (C bodies and the asm of cRollerModifier_update, _collide, _Collision);
//   [O] SSX On Tour's DWARF (cRollerNodeState, cRigidBodyState, cRigidBody, cRollerModifier, bxSphereTree,
//       SphereTree); On Tour's offsets equal retail's for every field here.
// t3Matrix (On Tour 0x24 bytes) is written as float[3][3]; tPose comes from animation/rideranimbase.h and tBoundBox
// from world/worldview.h until both move to a bx header.

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h"

struct tInstanceRecord;
struct tSphTreeInfo;

// EA's sphere tree of one mesh, placed in world space (On Tour: bxSphereTree 0x140 = SphereTree 0xC0 + staticInfo).
// Retail methods in src/intersect/riderspheretree.cpp: SphereTree_SphereTree (ctor), SphereTree_init (reset), SphereTree_GetResolution
// (radius), SphereTree_MoveOffset (translate), SphereTree_TransformAbs (set transform); bxSphereTree_operatorE is operator=.
struct bxSphereTree {
    // --- SphereTree (On Tour 0xC0)
    t4Vector axis[8];      // 0x000 [O]; [U] SphereTree_init fills 8 qwords from gSphTreeAlignAxis, operator= copies them
    t4Vector transCenter;  // 0x080 [O]; [U] SphereTree_MoveOffset adds the offset (lqc2/sqc2 0x80); reset stores t4vect_Origin
    float scale;           // 0x090 [O]; [U] SphereTree_init stores 1.0f, SphereTree_GetResolution multiplies the radius by it
    float worstCase;       // 0x094 [O]; [U] the riderspheretree.cpp queries keep the deepest penetration here
    tSphTreeInfo* treeInfo; // 0x098 [O]; [R] SphereTree_SphereTree zeroes it; [U] cRollerModifier's ctors read the centre
                           //       (+0x2C) and body inertia (+0x5C) from it; SphereTree_GetResolution reads it
    int sortAxis[8];       // 0x09C [O]; [U] riderspheretree.cpp's views ("order[8]", child visiting order)
    char unk_0BC[0x4];     // 0x0BC padding
    // --- bxSphereTree
    char staticInfo[0x80]; // 0x0C0 [O] tSphTreeInfo staticInfo (not laid out); [U] operator= copies 0xC0-0x140
};

CHECK_OFFSET(bxSphereTree, axis, 0x000);
CHECK_OFFSET(bxSphereTree, transCenter, 0x080);
CHECK_OFFSET(bxSphereTree, scale, 0x090);
CHECK_OFFSET(bxSphereTree, worstCase, 0x094);
CHECK_OFFSET(bxSphereTree, treeInfo, 0x098);
CHECK_OFFSET(bxSphereTree, sortAxis, 0x09C);
CHECK_OFFSET(bxSphereTree, unk_0BC, 0x0BC);
CHECK_OFFSET(bxSphereTree, staticInfo, 0x0C0);
CHECK_SIZE(bxSphereTree, 0x140);

struct cRollerModifier {
    // --- cTransformModifier (On Tour 0x4: the vptr)
    void** mVtbl;                 // 0x000 [R] ctors store cTransformModifier_vtbl (cTransformModifier) then cRollerModifier_vtbl;
                                  //       the destructor stores cTransformModifier_vtbl back
    // --- cRollerNodeState (On Tour 0x28; [R] cRollerModifier_writeToReplayFrame writes these 0x28 bytes to the replay frame)
    float mKineticEnergy;         // 0x004 [O]; [U] cRollerModifier_Collision stores V.P + W.L; update reads/writes it
    float mMass;                  // 0x008 [O]; [R] ctor: rdata + 0x4, InvM = 1 / mMass; [U] Collision: 5000 / mMass
    float mElast;                 // 0x00C [O]; [R] ctor: rdata + 0x8
    float ViscousDrag;            // 0x010 [O]; [R] ctor stores 0 (sw $0)
    float ContactElastic;         // 0x014 [O]; [R] ctor 0.1f; [U] collide: rigidImpact's elasticity argument
    float ContactFriction;        // 0x018 [O]; [R] ctor 0.4f; [U] collide: rigidImpact's friction argument
    float StopContactSpeed;       // 0x01C [O]; [R] ctor 277.78f; [U] collide: below this speed (|V|) the hit is inelastic
    float Life;                   // 0x020 [O]; [R] ctor 0 (sw $0); [U] isComplete: Life >= 17.0f (On Tour compares
                                  //       kMaxRollerDeath); collide clamps it to 10 above 5; collide1 resets it (sw $0)
    float Radius;                 // 0x024 [O]; [P] getRadius/setRadius (lwc1/swc1 0x24, preview == retail)
    float HitFrequency;           // 0x028 [O]; [U] Collision zeroes it (sw $0), collide adds 1/60, update reads/writes it
    char unk_02C[0x4];            // 0x02C padding (cRigidBody is 16-byte aligned)
    // --- cRigidBody (object/rigidbody.h; [R] ctor cRigidBody_cRigidBody(self + 0x30), writeToReplayFrame passes self + 0x30)
    tPose Pose;                   // 0x030 [O] cRigidBodyState::Pose; [R] ctor stores the start pose (sq 0x30/0x40);
                                  //       [U] getBodyPose (cRollerModifier_getBodyPose) reads it, translateMe (cRollerModifier_translateMe) adds to
                                  //       Pose.P, updateMOI reads Pose.Q (lqc2 0x40)
    t4Vector P;                   // 0x050 [O] linear momentum; [U] Collision/collide/update (lqc2/sq 0x50)
    t4Vector L;                   // 0x060 [O] angular momentum; [U] update/collide (lqc2/sq 0x60, lwc1 0x60-0x6C)
    float InvM;                   // 0x070 [O]; [R] ctor: 1.0f / mMass; [U] Collision, update, collide (lwc1 0x70)
    char unk_074[0xC];            // 0x074 padding (cRigidBodyState ends at 0x80)
    float InvMOI[3][3];           // 0x080 [O] t3Matrix; [U] updateMOI (cRollerModifier_updateMOI) writes the world-space inverse
                                  //       inertia (sdl/sdr 0x80-0x9F + sw 0xA0); update/collide read 0x80-0xA0
    char unk_0A4[0xC];            // 0x0A4 padding
    t4Vector V;                   // 0x0B0 [O] velocity; [U] Collision/update/collide (lqc2/sq 0xB0)
    t4Vector W;                   // 0x0C0 [O] angular velocity; [U] Collision/update/collide (lqc2/sq 0xC0)
    // --- cRollerModifier
    tInstanceRecord* m_pInstance; // 0x0D0 [O]; [R] ctor: the intersect info's instance (ii + 0x50), then sets flag bits
                                  //       in its word +0x8; cRollerModifier_cRollerModifier1 reads it from the frame; [U] writeToReplayFrame
    char unk_0D4[0xC];            // 0x0D4 padding (SphTree is 16-byte aligned)
    bxSphereTree SphTree;         // 0x0E0 [O]; [R] ctor SphereTree_SphereTree(self + 0xE0), then operator= (bxSphereTree_operatorE) from
                                  //       ii + 0x48; ctors read SphTree.treeInfo (lw 0x178); [U] collide, translateMe
    tBoundBox boundbox;           // 0x220 [O]; [U] setBoundBox (sq 0x220, 0x230), getBoundBox returns its address
    t4Matrix Transform;           // 0x240 [O]; [U] ctors/update build it from the body pose (sqc2 0x240-0x270);
                                  //       getRootTransform/getRootTransform1 return its address
    float InvBodyMOI[3][3];       // 0x280 [O] t3Matrix; [R] ctors: tree info + 0x5C, each element * 0.5f;
                                  //       [U] updateMOI reads it (lwc1 0x280-0x2A0)
    char unk_2A4[0xC];            // 0x2A4 padding
    t4Vector COMOffset;           // 0x2B0 [O]; [R] ctors: tree info + 0x2C (x, y, z; w = 0); [U] getBodyPose
    char RollerHint[0x10];        // 0x2C0 [O] tIntHint RollerHint (not laid out); [U] collide passes its address to
                                  //       the world intersect query (cBXNash_terrainIntersect1)
};

CHECK_OFFSET(cRollerModifier, mVtbl, 0x000);
CHECK_OFFSET(cRollerModifier, mKineticEnergy, 0x004);
CHECK_OFFSET(cRollerModifier, mMass, 0x008);
CHECK_OFFSET(cRollerModifier, mElast, 0x00C);
CHECK_OFFSET(cRollerModifier, ViscousDrag, 0x010);
CHECK_OFFSET(cRollerModifier, ContactElastic, 0x014);
CHECK_OFFSET(cRollerModifier, ContactFriction, 0x018);
CHECK_OFFSET(cRollerModifier, StopContactSpeed, 0x01C);
CHECK_OFFSET(cRollerModifier, Life, 0x020);
CHECK_OFFSET(cRollerModifier, Radius, 0x024);
CHECK_OFFSET(cRollerModifier, HitFrequency, 0x028);
CHECK_OFFSET(cRollerModifier, unk_02C, 0x02C);
CHECK_OFFSET(cRollerModifier, Pose, 0x030);
CHECK_OFFSET(cRollerModifier, P, 0x050);
CHECK_OFFSET(cRollerModifier, L, 0x060);
CHECK_OFFSET(cRollerModifier, InvM, 0x070);
CHECK_OFFSET(cRollerModifier, unk_074, 0x074);
CHECK_OFFSET(cRollerModifier, InvMOI, 0x080);
CHECK_OFFSET(cRollerModifier, unk_0A4, 0x0A4);
CHECK_OFFSET(cRollerModifier, V, 0x0B0);
CHECK_OFFSET(cRollerModifier, W, 0x0C0);
CHECK_OFFSET(cRollerModifier, m_pInstance, 0x0D0);
CHECK_OFFSET(cRollerModifier, unk_0D4, 0x0D4);
CHECK_OFFSET(cRollerModifier, SphTree, 0x0E0);
CHECK_OFFSET(cRollerModifier, boundbox, 0x220);
CHECK_OFFSET(cRollerModifier, Transform, 0x240);
CHECK_OFFSET(cRollerModifier, InvBodyMOI, 0x280);
CHECK_OFFSET(cRollerModifier, unk_2A4, 0x2A4);
CHECK_OFFSET(cRollerModifier, COMOffset, 0x2B0);
CHECK_OFFSET(cRollerModifier, RollerHint, 0x2C0);
CHECK_SIZE(cRollerModifier, 0x2D0);

#endif /* OBJECT_ROLLERMODIFIER_H */
