#ifndef OBJECT_PHYSICSMODIFIER_H
#define OBJECT_PHYSICSMODIFIER_H

// cPhysicsModifier: the base of the physics modifiers a cMoveNode can own (tModifierBlock::Physics; in SSX 3 only
// cBoostModifier). Size 0x8: On Tour 0x8; the vptr follows the class's only data member (g++ 2.95 puts it after the data of
// the class that first declares virtuals): retail ctor cPhysicsModifier_cPhysicsModifier1 stores Active at 0x0 and cPhysicsModifier_vtbl at 0x4.
// Methods: ctor cPhysicsModifier_cPhysicsModifier1 (railmodifier unit), replay ctor cPhysicsModifier_cPhysicsModifier and writeToReplayFrame cPhysicsModifier_writeToReplayFrame
// (src/object/multiparticle.cpp), destructor cPhysicsModifier__cPhysicsModifier, update, signal, isActive,
// getPhysicsType (one-line defaults in src/object/railmodifier.cpp); vtable 0x004913B8 (6 slots). EA's source unknown.

#include "layout.h"

struct cPhysicsModifier {
    int Active;                     // 0x00 [O] bool (4 bytes); [R] ctor cPhysicsModifier_cPhysicsModifier1 stores 1, the replay ctor
                                    //      cPhysicsModifier_cPhysicsModifier reads 4 bytes here; [U] isActive (lw 0x0)
    void** mVtbl;                   // 0x04 [R] cPhysicsModifier_vtbl (0x004913B8); cBoostModifier ctors store
                                    //      cBoostModifier_vtbl (0x00490898)
};

CHECK_OFFSET(cPhysicsModifier, Active, 0x00);
CHECK_OFFSET(cPhysicsModifier, mVtbl, 0x04);
CHECK_SIZE(cPhysicsModifier, 0x8);

#endif /* OBJECT_PHYSICSMODIFIER_H */
