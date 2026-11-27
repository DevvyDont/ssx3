#ifndef OBJECT_TRANSFORMMODIFIER_H
#define OBJECT_TRANSFORMMODIFIER_H

// cTransformModifier: the abstract base of every root-transform modifier a cMoveNode can own (tModifierBlock::
// RootTransform): cSplineModifier, cMultiSplineModifier, cPositionModifier (-> cParentModifier), cMagnetModifier,
// cAvaSplineModifier, cRollerModifier. Size 0x4 (vptr only): On Tour 0x4; every derived retail ctor stores its vtable at
// +0x0 and its own state from +0x4 (cMultiSplineModifier) or +0x10 (the 16-byte-aligned states).
// EA's header: d:/ssx/code/source/object/transformmodifier.h (preview assert string). Base vtable cTransformModifier_vtbl (27 slots):
// ~cTransformModifier cTransformModifier__cTransformModifier, update cTransformModifier_update, updateRootTransform cTransformModifier_updateRootTransform, postUpdate, signal,
// isComplete, clearComplete, shouldRebound, runCollide, collide, render, getBoundBox cTransformModifier_getBoundBox, setBoundBox
// cTransformModifier_setBoundBox, getRadius cTransformModifier_getRadius, setRadius cTransformModifier_setRadius, getMesh, isRootTransformDirty, getRootTransform()
// cTransformModifier_getRootTransform, getRootTransform(inst), hasIntersect, intersect, updateIntersectInfo, getAnimMats, updateOverlapSystem,
// getParent, getTransformType cTransformModifier_getTransformType, writeToReplayFrame cTransformModifier_writeToReplayFrame (all one-line defaults, src/object/
// railmodifier.cpp 0x360B00..0x360C08; slot order = On Tour's declaration order).

#include "layout.h"

struct tCollideInfo;
struct tIntersectInfo;
struct tBoundBox;
struct t4Matrix;
struct tInstanceRecord;
struct cWorldIntersectType;
struct cMoveNode;
struct cBxReplayFrame;

struct cTransformModifier {
    // What getTransformType (slot 26) reports (names ours, from the classes): [R] cTransformModifier_getTransformType
    // returns 0, cSplineModifier 1, cMultiSplineModifier 2, cPositionModifier 3, cParentModifier 4, cRollerModifier 5,
    // cMagnetModifier 6, cAvaSplineModifier 7; [U] cMoveNode's focus-count methods act only on kTransform_MultiSpline.
    enum eTransformType {
        kTransform_None = 0, kTransform_Spline = 1, kTransform_MultiSpline = 2, kTransform_Position = 3,
        kTransform_Parent = 4, kTransform_Roller = 5, kTransform_Magnet = 6, kTransform_AvaSpline = 7
    };

    // 0x00: the vptr. [R] cTransformModifier__cTransformModifier (dtor) stores cTransformModifier_vtbl; derived ctors
    // store their own. Slots [R] cTransformModifier_vtbl (27; names from its entries), types [O] On Tour's class
    // cTransformModifier (same order; its second getRootTransform overload is retail's getRootTransform1):
    virtual ~cTransformModifier();                                                  // 1
    virtual void update()
        __asm__("cTransformModifier_update__FPv");                                                          // 2
    virtual void updateRootTransform()
        __asm__("cTransformModifier_updateRootTransform__FPv");                                             // 3
    virtual void postUpdate()
        __asm__("cTransformModifier_postUpdate__FPv");                                                      // 4
    virtual void signal(int signal, float arg)
        __asm__("cTransformModifier_signal__FPv");                                     // 5
    virtual bool isComplete()
        __asm__("cTransformModifier_isComplete__FPv");                                                      // 6
    virtual void clearComplete()
        __asm__("cTransformModifier_clearComplete__FPv");                                                   // 7
    virtual bool shouldRebound()
        __asm__("cTransformModifier_shouldRebound__FPv");                                                   // 8
    virtual bool runCollide(int Being)
        __asm__("cTransformModifier_runCollide__FPv");                                             // 9
    virtual void collide(const tCollideInfo* tc, const tIntersectInfo* ti)
        __asm__("cTransformModifier_collide__FPv");         // 10
    virtual void render()
        __asm__("cTransformModifier_render__FPv");                                                          // 11
    virtual const tBoundBox* getBoundBox() const
        __asm__("cTransformModifier_getBoundBox__FPv");                                   // 12
    virtual void setBoundBox(const tBoundBox& boundbox)
        __asm__("cTransformModifier_setBoundBox__FPv");                            // 13
    virtual float getRadius() const
        __asm__("cTransformModifier_getRadius__FPv");                                                // 14
    virtual void setRadius(float radius)
        __asm__("cTransformModifier_setRadius__FPv");                                           // 15
    virtual int getMesh() const
        __asm__("cTransformModifier_getMesh__FPv");                                                    // 16
    virtual bool isRootTransformDirty() const
        __asm__("cTransformModifier_isRootTransformDirty__FPv");                                      // 17
    virtual const t4Matrix* getRootTransform()
        __asm__("cTransformModifier_getRootTransform__FPv");                                     // 18
    virtual const t4Matrix* getRootTransform1(tInstanceRecord* inst)
        __asm__("cTransformModifier_getRootTransform1__FPv");               // 19
    virtual bool hasIntersect() const
        __asm__("cTransformModifier_hasIntersect__FPv");                                              // 20
    virtual int intersect(cWorldIntersectType& w, tIntersectInfo* intInfo, int num, int flags)
        __asm__("cTransformModifier_intersect__FPv"); // 21
    virtual void updateIntersectInfo(tIntersectInfo& intInfo)
        __asm__("cTransformModifier_updateIntersectInfo__FPv");                      // 22
    virtual const t4Matrix* getAnimMats(tInstanceRecord* inst)
        __asm__("cTransformModifier_getAnimMats__FPv");                     // 23
    virtual void updateOverlapSystem()
        __asm__("cTransformModifier_updateOverlapSystem__FPv");                                             // 24
    virtual cMoveNode* getParent() const
        __asm__("cTransformModifier_getParent__FPv");                                           // 25
    virtual int getTransformType() const
        __asm__("cTransformModifier_getTransformType__FPv");                                           // 26
    virtual void writeToReplayFrame(cBxReplayFrame* data)
        __asm__("cTransformModifier_writeToReplayFrame__FPv");                          // 27
};

CHECK_SIZE(cTransformModifier, 0x4);

#endif /* OBJECT_TRANSFORMMODIFIER_H */
