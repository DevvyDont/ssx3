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

// World-script data that adds a boost modifier (On Tour tAddBoostData, 0x24; the same fields as tMakeBoostData).
// [O] On Tour's cBoostModifier(const tAddBoostData&, tInstanceRecord*); [U] cBoostModifier_cBoostModifier (view
// sSphDef_342768) stores Type, BoostTime seconds * TimerHz, (BoostX, BoostY, BoostZ, 0) as Boost, MaxSpeed * 100 and
// Drag.
struct tAddBoostData {
    unsigned int unk_000;           // 0x00 [O] tResourceID InstId (no retail read)
    short Type;                     // 0x04 [O] int Type; [U] the ctor reads its low half (lh) -> Type
    char unk_006[0x2];              // 0x06 [O] Type's high half (never read)
    float BoostTime;                // 0x08 [O]; [U] * TimerHz -> BoostTime (frames)
    float Drag;                     // 0x0C [O]; [U] -> Drag
    float MaxSpeed;                 // 0x10 [O]; [U] * 100 -> MaxSpeed
    float BoostX;                   // 0x14 [O]; [U] -> Boost.x
    float BoostY;                   // 0x18 [O]; [U] -> Boost.y
    float BoostZ;                   // 0x1C [O]; [U] -> Boost.z
    char unk_020[0x4];              // 0x20 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tAddBoostData, unk_000, 0x00);
CHECK_OFFSET(tAddBoostData, Type, 0x04);
CHECK_OFFSET(tAddBoostData, unk_006, 0x06);
CHECK_OFFSET(tAddBoostData, BoostTime, 0x08);
CHECK_OFFSET(tAddBoostData, Drag, 0x0C);
CHECK_OFFSET(tAddBoostData, MaxSpeed, 0x10);
CHECK_OFFSET(tAddBoostData, BoostX, 0x14);
CHECK_OFFSET(tAddBoostData, BoostY, 0x18);
CHECK_OFFSET(tAddBoostData, BoostZ, 0x1C);
CHECK_OFFSET(tAddBoostData, unk_020, 0x20);
CHECK_SIZE(tAddBoostData, 0x24);

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

// cBoostModifierState (On Tour, 0x30): the boost state cBoostModifier carries at +0x10 (Boost .. Type above, written
// out inline there); the boost-plane helper takes it whole (view sPlane_342A88: n = Boost, d = MaxSpeed, k = Drag).
struct cBoostModifierState {
    t4Vector Boost;                 // 0x00 [O]
    float MaxSpeed;                 // 0x10 [O]
    float Drag;                     // 0x14 [O]
    int BoostTime;                  // 0x18 [O]
    int Time;                       // 0x1C [O]
    int Type;                       // 0x20 [O]
    char unk_024[0xC];              // 0x24 padding (16-byte aligned)
};

CHECK_OFFSET(cBoostModifierState, Boost, 0x00);
CHECK_OFFSET(cBoostModifierState, MaxSpeed, 0x10);
CHECK_OFFSET(cBoostModifierState, Drag, 0x14);
CHECK_OFFSET(cBoostModifierState, BoostTime, 0x18);
CHECK_OFFSET(cBoostModifierState, Time, 0x1C);
CHECK_OFFSET(cBoostModifierState, Type, 0x20);
CHECK_SIZE(cBoostModifierState, 0x30);

#endif /* OBJECT_BOOSTMODIFIER_H */
