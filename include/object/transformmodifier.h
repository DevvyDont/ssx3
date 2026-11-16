#ifndef OBJECT_TRANSFORMMODIFIER_H
#define OBJECT_TRANSFORMMODIFIER_H

// cTransformModifier: the abstract base of every root-transform modifier a cMoveNode can own (tModifierBlock::
// RootTransform): cSplineModifier, cMultiSplineModifier, cPositionModifier (-> cParentModifier), cMagnetModifier,
// cAvaSplineModifier, cRollerModifier. Size 0x4 (vptr only): On Tour 0x4; every derived retail ctor stores its vtable at
// +0x0 and its own state from +0x4 (cMultiSplineModifier) or +0x10 (the 16-byte-aligned states).
// EA's header: d:/ssx/code/source/object/transformmodifier.h (preview assert string). Base vtable D_004913F8 (27 slots):
// ~cTransformModifier cTransformModifier__cTransformModifier, update cTransformModifier_update, updateRootTransform func_00360B38, postUpdate, signal,
// isComplete, clearComplete, shouldRebound, runCollide, collide, render, getBoundBox cTransformModifier_getBoundBox, setBoundBox
// cTransformModifier_setBoundBox, getRadius cTransformModifier_getRadius, setRadius cTransformModifier_setRadius, getMesh, isRootTransformDirty, getRootTransform()
// func_00360BB8, getRootTransform(inst), hasIntersect, intersect, updateIntersectInfo, getAnimMats, updateOverlapSystem,
// getParent, getTransformType func_00360BF8, writeToReplayFrame func_00360C00 (all one-line defaults, src/object/
// railmodifier.cpp 0x360B00..0x360C08; slot order = On Tour's declaration order).

#include "layout.h"

struct cTransformModifier {
    void** mVtbl;                   // 0x00 [R] cTransformModifier__cTransformModifier (dtor) stores D_004913F8; derived ctors store their own
                                    //      vtable
};

CHECK_OFFSET(cTransformModifier, mVtbl, 0x00);
CHECK_SIZE(cTransformModifier, 0x4);

#endif /* OBJECT_TRANSFORMMODIFIER_H */
