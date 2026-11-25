#ifndef OBJECT_PHYSICSMODIFIER_H
#define OBJECT_PHYSICSMODIFIER_H

// cPhysicsModifier: the base of the physics modifiers a cMoveNode can own (tModifierBlock::Physics; in SSX 3 only
// cBoostModifier). Size 0x8: On Tour 0x8; the vptr follows the class's only data member (g++ 2.95 puts it after the data of
// the class that first declares virtuals): retail ctor cPhysicsModifier_cPhysicsModifier1 stores Active at 0x0 and cPhysicsModifier_vtbl at 0x4.
// Methods: ctor cPhysicsModifier_cPhysicsModifier1 (railmodifier unit), replay ctor cPhysicsModifier_cPhysicsModifier and writeToReplayFrame cPhysicsModifier_writeToReplayFrame
// (src/object/multiparticle.cpp), destructor cPhysicsModifier__cPhysicsModifier, update, signal, isActive,
// getPhysicsType (one-line defaults in src/object/railmodifier.cpp); vtable 0x004913B8 (6 slots). EA's source unknown.

#include "layout.h"

struct cBxReplayFrame;

struct cPhysicsModifier {
    int Active;                     // 0x00 [O] bool (4 bytes); [R] ctor cPhysicsModifier_cPhysicsModifier1 stores 1, the replay ctor
                                    //      cPhysicsModifier_cPhysicsModifier reads 4 bytes here; [U] isActive (lw 0x0)
    // 0x04: the vptr. [R] cPhysicsModifier_vtbl (0x004913B8); cBoostModifier ctors store cBoostModifier_vtbl
    //   (0x00490898). Slots:
    virtual ~cPhysicsModifier();                                // 1  ([R] cPhysicsModifier_vtbl; types [O])
    virtual void update();                                      // 2
    virtual void signal(int signal, float arg);                 // 3
    virtual bool isActive();                                    // 4
    virtual int getPhysicsType();                               // 5
    virtual void writeToReplayFrame(cBxReplayFrame* data);      // 6

    // Methods bound to their retail symbols by asm labels.
    void * cPhysicsModifier1() __asm__("cPhysicsModifier_cPhysicsModifier1");
};

CHECK_OFFSET(cPhysicsModifier, Active, 0x00);
CHECK_SIZE(cPhysicsModifier, 0x8);

#endif /* OBJECT_PHYSICSMODIFIER_H */
