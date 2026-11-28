#ifndef OBJECT_POSITIONMODIFIER_H
#define OBJECT_POSITIONMODIFIER_H
#pragma interface

// cPositionModifier: the transform modifier that gives a placed instance a fixed offset / matrix (cMoveNode::addPosition;
// also the base of cParentModifier). Unnamed in retail: vtable cPositionModifier_vtbl (27 slots), identified by the
// "PositionModifier" allocation tag and On Tour's cPositionModifier/cPositionModifierState layout, which retail matches.
// Size 0x90: retail cMemMan_alloc(0x90, "PositionModifier") in cMoveNode's two addPosition paths (src/object/movenode.cpp)
// and the replay loader (src/object/modifierblock.cpp type 4); On Tour 0x90.
// Constructors (src/object/movenode.cpp): cPositionModifier_cPositionModifier (t4Vector offset), cPositionModifier_cPositionModifier1 (t4Matrix), cPositionModifier_cPositionModifier2 (replay
// frame); destructor cPositionModifier__cPositionModifier; the rest are vtable slots (methods.txt). EA's source file is unknown (no preview
// assert; On Tour builds it in all_in_one_object.cpp).
// A FLAT layout: cTransformModifier (0x0: vptr) + cPositionModifierState (0x10-0x48) + Transform.
// Evidence tags: [R] ctors/dtor/allocation; [U] retail use; [O] On Tour (every offset equals retail; bools are 4 bytes).

#include "layout.h"
#include "object/transformmodifier.h"
#include "bx/math3d.h"
#include "world/worldview.h"

struct cBxReplayFrame;
struct t4Matrix;

struct cPositionModifier : public cTransformModifier {
    // cTransformModifier (object/transformmodifier.h, 0x4): the vptr at 0x00:
    //   [R] ctors store cPositionModifier_vtbl (cPositionModifier; cParentModifier: cParentModifier_vtbl); dtor
    //       cPositionModifier__cPositionModifier
    char unk_04[0xc];               // 0x04 padding (the state is 16-byte aligned)
    // --- cPositionModifierState
    tBoundBox BoundBox;             // 0x10 [O]; [U] getBoundBox cPositionModifier_getBoundBox returns +0x10, setBoundBox cPositionModifier_setBoundBox
                                    //      (sq 0x10/0x20)
    t4Vector Offset;                // 0x30 [O]; [R] ctor (t4Vector) stores the offset, ctor (t4Matrix) row 3; [U]
                                    //      updateRootTransform cPositionModifier_updateRootTransform (lq 0x30)
    float Radius;                   // 0x40 [O]; [U] getRadius/setRadius cPositionModifier_getRadius/cPositionModifier_setRadius (lwc1/swc1 0x40)
    int RootTransformDirty;         // 0x44 [O] bool (4 bytes); [R] ctors store 1; [U] cPositionModifier_isRootTransformDirty
                                    //      (isRootTransformDirty), getRootTransform cPositionModifier_getRootTransform, updateRootTransform
    char unk_48[0x8];               // 0x48 padding (Transform is 16-byte aligned)
    // --- cPositionModifier
    t4Matrix Transform;             // 0x50 [O]; [R] ctors copy t4mat_Identity (or the matrix) here, then the offset into
                                    //      row 3 (sq 0x80); [U] getRootTransform returns +0x50

    // Methods bound to their retail symbols by asm labels.
    // inline: cParentModifier's destructor inlines it (retail's out-of-line copy cPositionModifier__cPositionModifier
    // stays a C function)
    virtual ~cPositionModifier() {}
    virtual void updateRootTransform() __asm__("cPositionModifier_updateRootTransform");
    virtual void writeToReplayFrame(cBxReplayFrame* obj) __asm__("cPositionModifier_writeToReplayFrame");
    virtual void update() __asm__("cPositionModifier_update__FPv");
    virtual int getTransformType() const __asm__("cPositionModifier_getTransformType__FPv");
    virtual const tBoundBox* getBoundBox() const __asm__("cPositionModifier_getBoundBox__FPv");
    virtual float getRadius() const __asm__("cPositionModifier_getRadius__FPv");
    virtual void setRadius(float val) __asm__("cPositionModifier_setRadius__FPvf");
    virtual void setBoundBox(const tBoundBox& boundbox) __asm__("cPositionModifier_setBoundBox");
    virtual const t4Matrix* getRootTransform() __asm__("cPositionModifier_getRootTransform");
};

CHECK_OFFSET(cPositionModifier, unk_04, 0x04);
CHECK_OFFSET(cPositionModifier, BoundBox, 0x10);
CHECK_OFFSET(cPositionModifier, Offset, 0x30);
CHECK_OFFSET(cPositionModifier, Radius, 0x40);
CHECK_OFFSET(cPositionModifier, RootTransformDirty, 0x44);
CHECK_OFFSET(cPositionModifier, unk_48, 0x48);
CHECK_OFFSET(cPositionModifier, Transform, 0x50);
CHECK_SIZE(cPositionModifier, 0x90);
__asm__("_vt$17cPositionModifier = cPositionModifier_vtbl");   // g++'s name for the retail vtable

#endif /* OBJECT_POSITIONMODIFIER_H */
