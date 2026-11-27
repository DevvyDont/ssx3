#ifndef OBJECT_AVASPLINEMODIFIER_H
#define OBJECT_AVASPLINEMODIFIER_H

// cAvaSplineModifier: the transform modifier that drives an avalanche prop from its instance's animation spline
// (cMoveNode::addAvaSpline). Unnamed in retail: vtable cAvaSplineModifier_vtbl (27 slots), identified by the "AvaSplineModifier" tag
// and On Tour's layout. Size 0x90: retail cMemMan_alloc(0x90, "AvaSplineModifier") in cMoveNode's addAvaSpline and the
// replay loader (type 7); On Tour 0x90. Constructors cAvaSplineModifier_cAvaSplineModifier (instance) and cAvaSplineModifier_cAvaSplineModifier1 (replay frame);
// destructor cAvaSplineModifier__cAvaSplineModifier. EA's source file is unknown.
// A FLAT layout: cTransformModifier (0x0: vptr) + cAvaSplineModifierState (0x10-0x40) + own fields (On Tour offsets).

#include "layout.h"
#include "object/transformmodifier.h"
#include "bx/math3d.h"
#include "world/worldview.h"

struct tInstanceRecord;

struct cBxReplayFrame;

struct cAvaSplineModifier : public cTransformModifier {
    // cTransformModifier (object/transformmodifier.h, 0x4): the vptr at 0x00:
    //   [R] ctors store cAvaSplineModifier_vtbl; dtor cAvaSplineModifier__cAvaSplineModifier
    char unk_04[0xc];               // 0x04 padding (the state is 16-byte aligned)
    // --- cAvaSplineModifierState
    tBoundBox BoundBox;             // 0x10 [O]; [U] getBoundBox cAvaSplineModifier_getBoundBox returns +0x10, setBoundBox cAvaSplineModifier_setBoundBox
                                    //      (sq 0x10/0x20)
    float Radius;                   // 0x30 [O]; [R] ctor zeroes it (sw $0); [U] getRadius/setRadius
                                    //      cAvaSplineModifier_getRadius/cAvaSplineModifier_setRadius
    char unk_34[0xc];               // 0x34 padding (Transform is 16-byte aligned)
    // --- cAvaSplineModifier
    t4Matrix Transform;             // 0x40 [O]; [U] getRootTransform cAvaSplineModifier_getRootTransform returns +0x40; update cAvaSplineModifier_update
                                    //      fills it from the instance
    tInstanceRecord* Instance;      // 0x80 [O]; [R] ctor stores the instance, replay ctor reads it; [U] update,
                                    //      writeToReplayFrame cAvaSplineModifier_writeToReplayFrame
    char unk_84[0xc];               // 0x84 padding to the 0x90 allocation

    // Methods bound to their retail symbols by asm labels.
    // retail's destructor cAvaSplineModifier__cAvaSplineModifier stays a C function (g++ 2.95 takes no asm label on a
    // destructor); declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cAvaSplineModifier();
    virtual void update() __asm__("cAvaSplineModifier_update");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cAvaSplineModifier_writeToReplayFrame");
    virtual const tBoundBox* getBoundBox() const __asm__("cAvaSplineModifier_getBoundBox__FPv");
    virtual float getRadius() const __asm__("cAvaSplineModifier_getRadius__FPv");
    virtual void setRadius(float val) __asm__("cAvaSplineModifier_setRadius__FPvf");
    virtual int getTransformType() const __asm__("cAvaSplineModifier_getTransformType__FPv");
    virtual const t4Matrix* getRootTransform() __asm__("cAvaSplineModifier_getRootTransform__FPv");
    virtual void setBoundBox(const tBoundBox& boundbox) __asm__("cAvaSplineModifier_setBoundBox");
};

CHECK_OFFSET(cAvaSplineModifier, unk_04, 0x04);
CHECK_OFFSET(cAvaSplineModifier, BoundBox, 0x10);
CHECK_OFFSET(cAvaSplineModifier, Radius, 0x30);
CHECK_OFFSET(cAvaSplineModifier, unk_34, 0x34);
CHECK_OFFSET(cAvaSplineModifier, Transform, 0x40);
CHECK_OFFSET(cAvaSplineModifier, Instance, 0x80);
CHECK_OFFSET(cAvaSplineModifier, unk_84, 0x84);
CHECK_SIZE(cAvaSplineModifier, 0x90);

#endif /* OBJECT_AVASPLINEMODIFIER_H */
