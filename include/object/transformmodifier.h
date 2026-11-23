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
    // 0x00: the vptr. [R] cTransformModifier__cTransformModifier (dtor) stores cTransformModifier_vtbl; derived ctors
    // store their own. Slots [R] cTransformModifier_vtbl (27; names from its entries), types [O] On Tour's class
    // cTransformModifier (same order; its second getRootTransform overload is retail's getRootTransform1):
    virtual ~cTransformModifier();                                                  // 1
    virtual void update();                                                          // 2
    virtual void updateRootTransform();                                             // 3
    virtual void postUpdate();                                                      // 4
    virtual void signal(int signal, float arg);                                     // 5
    virtual bool isComplete();                                                      // 6
    virtual void clearComplete();                                                   // 7
    virtual bool shouldRebound();                                                   // 8
    virtual bool runCollide(int Being);                                             // 9
    virtual void collide(const tCollideInfo* tc, const tIntersectInfo* ti);         // 10
    virtual void render();                                                          // 11
    virtual const tBoundBox* getBoundBox() const;                                   // 12
    virtual void setBoundBox(const tBoundBox& boundbox);                            // 13
    virtual float getRadius() const;                                                // 14
    virtual void setRadius(float radius);                                           // 15
    virtual int getMesh() const;                                                    // 16
    virtual bool isRootTransformDirty() const;                                      // 17
    virtual const t4Matrix* getRootTransform();                                     // 18
    virtual const t4Matrix* getRootTransform1(tInstanceRecord* inst);               // 19
    virtual bool hasIntersect() const;                                              // 20
    virtual int intersect(cWorldIntersectType& w, tIntersectInfo* intInfo, int num, int flags); // 21
    virtual void updateIntersectInfo(tIntersectInfo& intInfo);                      // 22
    virtual const t4Matrix* getAnimMats(tInstanceRecord* inst);                     // 23
    virtual void updateOverlapSystem();                                             // 24
    virtual cMoveNode* getParent() const;                                           // 25
    virtual int getTransformType() const;                                           // 26
    virtual void writeToReplayFrame(cBxReplayFrame* data);                          // 27
};

CHECK_SIZE(cTransformModifier, 0x4);

#endif /* OBJECT_TRANSFORMMODIFIER_H */
