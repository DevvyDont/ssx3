#ifndef OBJECT_PARENTMODIFIER_H
#define OBJECT_PARENTMODIFIER_H

// cParentModifier: a cPositionModifier that follows another instance's mesh (cMoveNode::addParent: attach an object to a
// parent instance's bone). Unnamed in retail: vtable cParentModifier_vtbl (27 slots; shares the position slots cPositionModifier_isRootTransformDirty..
// cPositionModifier_getRootTransform), identified by the "ParentModifier" tag and On Tour's layout. EA's source:
// d:/ssx/code/source/object/parentmodifier.cpp (preview assert).
// Size 0xB0: retail cMemMan_alloc(0xB0, "ParentModifier") in cMoveNode's addParent and the replay loader (type 3); On
// Tour 0xA0 (ParentInst at 0x94 there; retail puts it at 0xA0: cParentModifier_cParentModifier1 sw 0xA0, the replay ctor cParentModifier_cParentModifier
// reads 4 bytes into +0x90 and the pointer into +0xA0).
// Constructors cParentModifier_cParentModifier1 (parent inst, mesh, offset: calls the cPositionModifier ctor cPositionModifier_cPositionModifier) and cParentModifier_cParentModifier
// (replay; calls cPositionModifier_cPositionModifier2); destructor cParentModifier__cParentModifier.
// A FLAT layout: cPositionModifier (0x00-0x90, object/positionmodifier.h) + cParentModifierState (Mesh) + ParentInst.

#include "layout.h"
#include "object/positionmodifier.h"
#include "bx/math3d.h"
#include "world/worldview.h"

struct tInstanceRecord;

struct t4Vector;

struct cParentModifier : public cPositionModifier {
    // cPositionModifier (object/positionmodifier.h, 0x90): the vptr at 0x00:
    //   [R] ctors store cPositionModifier_vtbl (cPositionModifier; cParentModifier: cParentModifier_vtbl); dtor
    //       cPositionModifier__cPositionModifier
    // --- cParentModifierState / cParentModifier
    int Mesh;                       // 0x90 [O] cParentModifierState::Mesh; [R] ctor stores the mesh arg, replay ctor
                                    //      reads 4 bytes here; [U] getMesh cParentModifier_getMesh, updateRootTransform
    char unk_94[0xc];               // 0x94 padding (no access)
    tInstanceRecord* ParentInst;    // 0xA0 [O]; [R] ctors; [U] updateRootTransform cParentModifier_updateRootTransform, updateIntersectInfo
                                    //      cParentModifier_updateIntersectInfo; cParentModifier_setParent (unreferenced setter) stores it
    char unk_A4[0xc];               // 0xA4 padding to the 0xB0 allocation

    // Methods bound to their retail symbols by asm labels.
    void setParent(tInstanceRecord* parentInst) __asm__("cParentModifier_setParent");
    // retail's destructor cParentModifier__cParentModifier stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cParentModifier();
    virtual void updateRootTransform() __asm__("cParentModifier_updateRootTransform");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cParentModifier_writeToReplayFrame");
    void* cParentModifier1(tInstanceRecord* parentInstance, int meshIndex, const t4Vector* pos)
        __asm__("cParentModifier_cParentModifier1");
    virtual void update() __asm__("cParentModifier_update__FPv");
    virtual int getMesh() const __asm__("cParentModifier_getMesh__FPv");
    virtual int getTransformType() const __asm__("cParentModifier_getTransformType__FPv");
    virtual void updateIntersectInfo(tIntersectInfo& obj) __asm__("cParentModifier_updateIntersectInfo");
};

CHECK_OFFSET(cParentModifier, Mesh, 0x90);
CHECK_OFFSET(cParentModifier, unk_94, 0x94);
CHECK_OFFSET(cParentModifier, ParentInst, 0xA0);
CHECK_OFFSET(cParentModifier, unk_A4, 0xA4);
CHECK_SIZE(cParentModifier, 0xB0);

#endif /* OBJECT_PARENTMODIFIER_H */
