#ifndef OBJECT_BOOSTMODIFIER_H
#define OBJECT_BOOSTMODIFIER_H

// cBoostModifier: the physics modifier of a boost pad / speed zone (cMoveNode::addBoost): pushes a rider along Boost up
// to MaxSpeed while it touches the instance. Size 0x50: retail cMemMan_alloc(0x50, "BoostModifier") in cMoveNode's addBoost
// (src/object/movenode.cpp) and the replay loader; On Tour 0x50.
// Constructors cBoostModifier_cBoostModifier (tAddBoostData, instance) and cBoostModifier_cBoostModifier1 (replay frame), update cBoostModifier_update,
// signal cBoostModifier_signal and three vtable slots cBoostModifier_applyDrag/cBoostModifier_applyBoost/cBoostModifier_writeToReplayFrame (all in the
// intersect/worldsphtree splat unit, 0x342768..0x342C08); destructor and the one-line getPhysicsType/boostStart/boostEnd
// in src/object/railmodifier.cpp; vtable cBoostModifier_vtbl (0x00490898, 10 slots). EA's source file is unknown.
// A FLAT layout: cPhysicsModifier (0x0-0x8, object/physicsmodifier.h) + cBoostModifierState (0x10-0x34) + Instance;
// every On Tour offset equals retail.

#include "layout.h"
#include "bx/math3d.h"

struct tInstanceRecord;

struct cBoostModifier {
    // --- cPhysicsModifier (0x8; object/physicsmodifier.h)
    int Active;                     // 0x00 [O] bool (4 bytes); [R] ctor cPhysicsModifier_cPhysicsModifier1 stores 1, the replay ctor
                                    //      cPhysicsModifier_cPhysicsModifier reads 4 bytes here; [U] isActive (lw 0x0)
    void** mVtbl;                   // 0x04 [R] cPhysicsModifier_vtbl (0x004913B8); cBoostModifier ctors store
                                    //      cBoostModifier_vtbl (0x00490898)
    char unk_08[0x8];               // 0x08 padding (the state is 16-byte aligned)
    // --- cBoostModifierState
    t4Vector Boost;                 // 0x10 [O]; [R] ctor (sq 0x10); [U] cBoostModifier_applyBoost (lqc2 0x10)
    float MaxSpeed;                 // 0x20 [O]; [R] ctor; [U] cBoostModifier_applyBoost
    float Drag;                     // 0x24 [O]; [R] ctor; [U] update, cBoostModifier_applyDrag
    int BoostTime;                  // 0x28 [O]; [R] ctor; [U] signal
    int Time;                       // 0x2C [O]; [R] ctor; [U] update counts it, signal resets it
    int Type;                       // 0x30 [O]; [R] ctor; [U] update
    char unk_34[0xc];               // 0x34 padding (no access)
    // --- cBoostModifier
    tInstanceRecord* Instance;      // 0x40 [O]; [R] both ctors; [U] update
    char unk_44[0xc];               // 0x44 padding to the 0x50 allocation
};

CHECK_OFFSET(cBoostModifier, Active, 0x00);
CHECK_OFFSET(cBoostModifier, mVtbl, 0x04);
CHECK_OFFSET(cBoostModifier, unk_08, 0x08);
CHECK_OFFSET(cBoostModifier, Boost, 0x10);
CHECK_OFFSET(cBoostModifier, MaxSpeed, 0x20);
CHECK_OFFSET(cBoostModifier, Drag, 0x24);
CHECK_OFFSET(cBoostModifier, BoostTime, 0x28);
CHECK_OFFSET(cBoostModifier, Time, 0x2C);
CHECK_OFFSET(cBoostModifier, Type, 0x30);
CHECK_OFFSET(cBoostModifier, unk_34, 0x34);
CHECK_OFFSET(cBoostModifier, Instance, 0x40);
CHECK_OFFSET(cBoostModifier, unk_44, 0x44);
CHECK_SIZE(cBoostModifier, 0x50);

#endif /* OBJECT_BOOSTMODIFIER_H */
