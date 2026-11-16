#ifndef OBJECT_MAGNETMODIFIER_H
#define OBJECT_MAGNETMODIFIER_H

// cMagnetModifier: the transform modifier that pulls an object towards a being (rider) inside a radius (cMoveNode::addMagnet).
// Unnamed in retail: vtable cMagnetModifier_vtbl (27 slots), identified by the "MagnetModifier" tag and On Tour's layout.
// Size 0xB0: retail cMemMan_alloc(0xB0, "MagnetModifier") in cMoveNode's addMagnet and the replay loader (type 6); On Tour 0xB0.
// Constructors func_003572E0 (matrix, radius, speed; src/object/movenode.cpp) and func_00357358 (replay frame);
// destructor cMagnetModifier__cMagnetModifier. EA's source file is unknown.
// A FLAT layout: cTransformModifier (0x0: vptr) + cMagnetModifierState (0x10-0x58) + own fields; On Tour's state order,
// with 4-byte bools (RootTransformDirty 0x50, AllowCollide 0x54 instead of 0x50/0x51).

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h"

struct tInstanceRecord;

struct cMagnetModifier {
    // --- cTransformModifier (vptr only)
    void** mVtbl;                   // 0x00 [R] ctors store cMagnetModifier_vtbl; dtor cMagnetModifier__cMagnetModifier
    char unk_04[0xc];               // 0x04 padding (the state is 16-byte aligned)
    // --- cMagnetModifierState
    t4Vector Offset;                // 0x10 [O]; [R] ctor copies Transform row 3 here; [U] update cMagnetModifier_update,
                                    //      updateRootTransform cMagnetModifier_updateRootTransform
    tBoundBox BoundBox;             // 0x20 [O]; [U] getBoundBox cMagnetModifier_getBoundBox returns +0x20, setBoundBox cMagnetModifier_setBoundBox
                                    //      (sq 0x20/0x30), intersect cMagnetModifier_intersect
    float Radius;                   // 0x40 [O]; [R] ctor ($f12); [U] getRadius cMagnetModifier_getRadius
    int Being;                      // 0x44 [O]; [R] ctor; [U] update, runCollide cMagnetModifier_runCollide
    float Speed;                    // 0x48 [O]; [R] ctor ($f13); [U] update
    float Time;                     // 0x4C [O]; [R] ctor zeroes it; [U] update
    int RootTransformDirty;         // 0x50 [O] bool; [R] ctors; [U] cMagnetModifier_isRootTransformDirty (isRootTransformDirty),
                                    //      getRootTransform cMagnetModifier_getRootTransform, update
    int AllowCollide;               // 0x54 [O] bool; [R] ctor zeroes it; [U] update, runCollide
    char unk_58[0x8];               // 0x58 padding (Transform is 16-byte aligned)
    // --- cMagnetModifier
    t4Matrix Transform;             // 0x60 [O]; [R] ctor copies the matrix arg; [U] getRootTransform returns +0x60,
                                    //      updateRootTransform writes row 3 (sq 0x90)
    tInstanceRecord* Instance;      // 0xA0 [O]; [R] ctor stores the instance arg, the replay ctor reads it; [U]
                                    //      intersect, writeToReplayFrame cMagnetModifier_writeToReplayFrame
    char unk_A4[0xc];               // 0xA4 padding to the 0xB0 allocation
};

CHECK_OFFSET(cMagnetModifier, mVtbl, 0x00);
CHECK_OFFSET(cMagnetModifier, unk_04, 0x04);
CHECK_OFFSET(cMagnetModifier, Offset, 0x10);
CHECK_OFFSET(cMagnetModifier, BoundBox, 0x20);
CHECK_OFFSET(cMagnetModifier, Radius, 0x40);
CHECK_OFFSET(cMagnetModifier, Being, 0x44);
CHECK_OFFSET(cMagnetModifier, Speed, 0x48);
CHECK_OFFSET(cMagnetModifier, Time, 0x4C);
CHECK_OFFSET(cMagnetModifier, RootTransformDirty, 0x50);
CHECK_OFFSET(cMagnetModifier, AllowCollide, 0x54);
CHECK_OFFSET(cMagnetModifier, unk_58, 0x58);
CHECK_OFFSET(cMagnetModifier, Transform, 0x60);
CHECK_OFFSET(cMagnetModifier, Instance, 0xA0);
CHECK_OFFSET(cMagnetModifier, unk_A4, 0xA4);
CHECK_SIZE(cMagnetModifier, 0xB0);

#endif /* OBJECT_MAGNETMODIFIER_H */
