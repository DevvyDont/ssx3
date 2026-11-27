#ifndef OBJECT_RIGIDBODY_H
#define OBJECT_RIGIDBODY_H

// cRigidBody: a rigid body's integrator state (pose, linear/angular momentum, inverse mass and inertia, the derived
// velocities). In SSX 3 its only user is cRollerModifier, which has it as a base at +0x30 (object/rollermodifier.h);
// the methods are in src/object/railmodifier.cpp: cRigidBody_cRigidBody (ctor, empty), cRigidBody_setW, cRigidBody_updateImplicit
// (updateImplicit), cRigidBody_stepMotion, cRigidBody_rigidImpact, func_0035D908 (applyImpulse),
// cRigidBody_writeToReplayFrame, cRigidBody_readFromReplayFrame. Size 0xA0: On Tour's DWARF, and in retail
// cRollerModifier's next field (m_pInstance) is at 0x30 + 0xA0. EA's header path is unknown (On Tour builds it in
// all_in_one_object.cpp); the preview keeps the RTTI names cRigidBody and cRigidBodyState.
//
// A FLAT layout, like main/game.h: no C++ inheritance yet. The real chain is cRigidBodyState (0x00-0x50: what the
// replay frame stores) -> cRigidBody. Evidence tags:
//   [R] retail: cRigidBody_writeToReplayFrame / cRigidBody_readFromReplayFrame copy 0x50 bytes (the cRigidBodyState part);
//   [U] how retail code uses the field (all eight methods above, asm and C, plus cRollerModifier's methods);
//   [O] SSX On Tour's DWARF: cRigidBodyState {Pose 0x0, P 0x20, L 0x30, InvM 0x40} (0x50) and cRigidBody
//       {InvMOI 0x50, V 0x80, W 0x90} (0xA0); every offset equals retail.
// t3Matrix (On Tour 0x24 bytes, members unnamed) is written as float[3][3]: retail copies it with unaligned ldl/ldr
// pairs plus one word (updateMOI, setW), i.e. 9 floats; tPose comes from animation/rideranimbase.h until it moves to
// bx/math3d.h.

#include "layout.h"
#include "bx/math3d.h"

struct cBxReplayFrame;
struct t4Vector;

struct cRigidBody {
    // --- cRigidBodyState (On Tour 0x50; the 0x50-byte replay-frame block)
    tPose Pose;            // 0x00 [O] cRigidBodyState::Pose; [U] stepMotion (cRigidBody_stepMotion) integrates Pose.P/Pose.Q
                           //      (lqc2/sqc2 0x0, 0x10, lwc1/swc1 0x10-0x1C); rigidImpact reads Pose.P
    t4Vector P;            // 0x20 [O] linear momentum; [U] updateImplicit: V = P * InvM; applyImpulse adds the impulse
    t4Vector L;            // 0x30 [O] angular momentum; [U] updateImplicit: W = InvMOI * L; setW: L = InvMOI^-1 * W
    float InvM;            // 0x40 [O]; [U] updateImplicit, rigidImpact, applyImpulse (lwc1 0x40)
    char unk_044[0xC];     // 0x44 padding to the state's 0x50 size ([R] the replay copies 0x50 bytes)
    // --- cRigidBody
    t3Matrix InvMOI;       // 0x50 [O] t3Matrix InvMOI; [U] updateImplicit/rigidImpact/applyImpulse read 0x50-0x70,
                           //      setW copies it (ldl/ldr + lw 0x70)
    char unk_074[0xC];     // 0x74 padding (V is 16-byte aligned)
    t4Vector V;            // 0x80 [O] velocity; [U] updateImplicit/applyImpulse write it (sq 0x80), stepMotion,
                           //      rigidImpact read it
    t4Vector W;            // 0x90 [O] angular velocity; [U] setW stores its argument (sq 0x90), updateImplicit writes it

    // Methods bound to their retail symbols by asm labels.
    void setW(const t4Vector* newPos) __asm__("cRigidBody_setW");
    void stepMotion(const t4Vector* force, const t4Vector* torque, float dt) __asm__("cRigidBody_stepMotion");
    void writeToReplayFrame(cBxReplayFrame* obj) __asm__("cRigidBody_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* obj) __asm__("cRigidBody_readFromReplayFrame");
    void updateImplicit() __asm__("cRigidBody_updateImplicit");
};

CHECK_OFFSET(cRigidBody, Pose, 0x00);
CHECK_OFFSET(cRigidBody, P, 0x20);
CHECK_OFFSET(cRigidBody, L, 0x30);
CHECK_OFFSET(cRigidBody, InvM, 0x40);
CHECK_OFFSET(cRigidBody, unk_044, 0x44);
CHECK_OFFSET(cRigidBody, InvMOI, 0x50);
CHECK_OFFSET(cRigidBody, unk_074, 0x74);
CHECK_OFFSET(cRigidBody, V, 0x80);
CHECK_OFFSET(cRigidBody, W, 0x90);
CHECK_SIZE(cRigidBody, 0xA0);

#endif /* OBJECT_RIGIDBODY_H */
