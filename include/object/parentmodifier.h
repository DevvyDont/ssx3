#ifndef OBJECT_PARENTMODIFIER_H
#define OBJECT_PARENTMODIFIER_H

// cParentModifier: a cPositionModifier that follows another instance's mesh (cMoveNode::addParent: attach an object to a
// parent instance's bone). Unnamed in retail: vtable cParentModifier_vtbl (27 slots; shares the position slots func_00361900..
// func_00361940), identified by the "ParentModifier" tag and On Tour's layout. EA's source:
// d:/ssx/code/source/object/parentmodifier.cpp (preview assert).
// Size 0xB0: retail cMemMan_alloc(0xB0, "ParentModifier") in cMoveNode's addParent and the replay loader (type 3); On
// Tour 0xA0 (ParentInst at 0x94 there; retail puts it at 0xA0: func_00357038 sw 0xA0, the replay ctor cParentModifier_cParentModifier
// reads 4 bytes into +0x90 and the pointer into +0xA0).
// Constructors func_00357038 (parent inst, mesh, offset: calls the cPositionModifier ctor func_00356EB8) and cParentModifier_cParentModifier
// (replay; calls func_00356F68); destructor cParentModifier__cParentModifier.
// A FLAT layout: cPositionModifier (0x00-0x90, object/positionmodifier.h) + cParentModifierState (Mesh) + ParentInst.

#include "layout.h"
#include "bx/math3d.h"
#include "world/worldview.h"

struct tInstanceRecord;

struct cParentModifier {
    // --- cPositionModifier (0x90; object/positionmodifier.h)
    void** mVtbl;                   // 0x00 [R] ctors store cPositionModifier_vtbl (cPositionModifier; cParentModifier: cParentModifier_vtbl);
                                    //      dtor cPositionModifier__cPositionModifier
    char unk_04[0xc];               // 0x04 padding (the state is 16-byte aligned)
    tBoundBox BoundBox;             // 0x10 [O]; [U] getBoundBox func_00361910 returns +0x10, setBoundBox func_00361918
                                    //      (sq 0x10/0x20)
    t4Vector Offset;                // 0x30 [O]; [R] ctor (t4Vector) stores the offset, ctor (t4Matrix) row 3; [U]
                                    //      updateRootTransform func_00356FF0 (lq 0x30)
    float Radius;                   // 0x40 [O]; [U] getRadius/setRadius func_00361930/func_00361938 (lwc1/swc1 0x40)
    int RootTransformDirty;         // 0x44 [O] bool (4 bytes); [R] ctors store 1; [U] func_00361900
                                    //      (isRootTransformDirty), getRootTransform func_00361940, updateRootTransform
    char unk_48[0x8];               // 0x48 padding (Transform is 16-byte aligned)
    t4Matrix Transform;             // 0x50 [O]; [R] ctors copy t4mat_Identity (or the matrix) here, then the offset into
                                    //      row 3 (sq 0x80); [U] getRootTransform returns +0x50
    // --- cParentModifierState / cParentModifier
    int Mesh;                       // 0x90 [O] cParentModifierState::Mesh; [R] ctor stores the mesh arg, replay ctor
                                    //      reads 4 bytes here; [U] getMesh func_003619C8, updateRootTransform
    char unk_94[0xc];               // 0x94 padding (no access)
    tInstanceRecord* ParentInst;    // 0xA0 [O]; [R] ctors; [U] updateRootTransform func_00357108, updateIntersectInfo
                                    //      func_00357210; func_003619D8 (unreferenced setter) stores it
    char unk_A4[0xc];               // 0xA4 padding to the 0xB0 allocation
};

CHECK_OFFSET(cParentModifier, mVtbl, 0x00);
CHECK_OFFSET(cParentModifier, unk_04, 0x04);
CHECK_OFFSET(cParentModifier, BoundBox, 0x10);
CHECK_OFFSET(cParentModifier, Offset, 0x30);
CHECK_OFFSET(cParentModifier, Radius, 0x40);
CHECK_OFFSET(cParentModifier, RootTransformDirty, 0x44);
CHECK_OFFSET(cParentModifier, unk_48, 0x48);
CHECK_OFFSET(cParentModifier, Transform, 0x50);
CHECK_OFFSET(cParentModifier, Mesh, 0x90);
CHECK_OFFSET(cParentModifier, unk_94, 0x94);
CHECK_OFFSET(cParentModifier, ParentInst, 0xA0);
CHECK_OFFSET(cParentModifier, unk_A4, 0xA4);
CHECK_SIZE(cParentModifier, 0xB0);

#endif /* OBJECT_PARENTMODIFIER_H */
