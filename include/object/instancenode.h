#ifndef OBJECT_INSTANCENODE_H
#define OBJECT_INSTANCENODE_H

// cInstanceNode: the scene node of one placed world instance (a tInstanceRecord). It sits in a cBucketMan bucket
// list and is the base of cMoveNode (-> cFloatingNode, cAnimNode, cDebounceNode, ...), cFlexRailNode and
// cFlexBridgeNode (their ctors call cInstanceNode_cInstanceNode1 directly), and of cHideNode, cRestoreNode, cDeadNode,
// cFlagNode, cCounterNode, cConveyorNode, cBoostNode, cDeadFadeNode and cMeshAnimNode (object/*node.h).
// Methods: src/object/instancenode.cpp, plus the one-line virtuals in src/object/railmodifier.cpp.
//
// Derives from cSortObjNode (object/objnode.h); its own member FLAT. The chain is
//   cObjNode (0x00-0x10: links + vptr) -> cTypeObjNode (0x10-0x14) -> cSortObjNode (0x14-0x18) -> cInstanceNode
// (retail ctors chain cObjNode_cObjNode__FP8cObjNodePv -> cTypeObjNode_cTypeObjNode1 -> cSortObjNode_cSortObjNode1 -> cInstanceNode_cInstanceNode1, each
// storing its own vtable at 0xC: cObjNode_vtbl, cTypeObjNode_vtbl, cSortObjNode_vtbl, cInstanceNode_vtbl = _vt$13cInstanceNode).
// Evidence tags:
//   [P] the July 2003 preview's inline accessors (getInstance lw 0x18, getSortIndex lw 0x14, getType lh 0x10,
//       is/set/resetRootTransformDirty lhu/sh 0x12 bit 0, cObjNode() zeroing 0x4/0x8); preview == retail here;
//   [R] retail constructors (src/object/objnode.cpp, src/object/floatingnode.cpp cInstanceNode_cInstanceNode1);
//   [U] how matched retail code uses the field;
//   [O] SSX On Tour's DWARF (cObjNode Next/Prev, cTypeObjNodeState, cSortObjNodeState, cInstanceNode::Instance).
// On Tour's chain is 4 bytes smaller (cObjNode 0xC: Next 0, Prev 4, vptr 8); retail has an extra leading word.

#include "layout.h"
#include "object/objnode.h"

struct cObjNode;
struct tInstanceRecord;
struct t4Vector;
struct t4Matrix;
struct cWorldIntersectType;
struct tIntersectInfo;
struct tCollideInfo;
struct cBeing;
struct tBoundBox;

struct cInstanceNode : public cSortObjNode {
    // cSortObjNode (object/objnode.h, 0x18): the vptr at 0x0C holds [R] cInstanceNode_vtbl (_vt$13cInstanceNode) in
    // cInstanceNode_cInstanceNode1 / cInstanceNode_cInstanceNode. Its new slots ([R] cInstanceNode_vtbl, 46 in all:
    // names from its entries cInstanceNode_*; types [O] On Tour's cInstanceNode / cMoveNode definitions):
    virtual bool updateNode();                                                      // 15
    virtual bool isMoveNode()
        __asm__("cInstanceNode_isMoveNode__FPv");                                                      // 16
    virtual bool isComplete()
        __asm__("cInstanceNode_isComplete__FPv");                                                      // 17
    virtual void throwMe(const t4Vector& vel)
        __asm__("cInstanceNode_throwMe__FPv");                                      // 18
    virtual void* getNodeState()
        __asm__("cInstanceNode_getNodeState__FPv");                                                   // 19
    virtual bool hasFocusCount() const
        __asm__("cInstanceNode_hasFocusCount__FPv");                                             // 20
    virtual int incFocusCount(tInstanceRecord* pInst)
        __asm__("cInstanceNode_incFocusCount__FPv");                              // 21
    virtual int decFocusCount(tInstanceRecord* pInst)
        __asm__("cInstanceNode_decFocusCount__FPv");                              // 22
    virtual int getFocusCount() const
        __asm__("cInstanceNode_getFocusCount__FPv");                                              // 23
    virtual const t4Matrix* getRootTransform()
        __asm__("cInstanceNode_getRootTransform__FPv");                                     // 24
    virtual const t4Matrix* getRootTransform1(tInstanceRecord* inst)
        __asm__("cInstanceNode_getRootTransform1__FPv");               // 25 On Tour's second
                                                                                    //    getRootTransform overload
    virtual bool hasAnimMats() const
        __asm__("cInstanceNode_hasAnimMats__FPv");                                               // 26
    virtual const t4Matrix* getAnimMats()
        __asm__("cInstanceNode_getAnimMats__FPv");                                          // 27
    virtual const t4Matrix* getInstAnimMats(tInstanceRecord* inst)
        __asm__("cInstanceNode_getInstAnimMats__FPv");                 // 28
    virtual const t4Matrix* getAnimMat(int animmat)
        __asm__("cInstanceNode_getAnimMat__FPv");                                // 29
    virtual float getAnimFrame() const
        __asm__("cInstanceNode_getAnimFrame__FPv");                                             // 30
    virtual int getAnimDir() const
        __asm__("cInstanceNode_getAnimDir__FPv");                                                 // 31
    virtual int frameUpdateEvent(float frame)
        __asm__("cInstanceNode_frameUpdateEvent__FPv");                                      // 32
    virtual void makeAnimMats(const t4Matrix* root, t4Matrix* animMats)
        __asm__("cInstanceNode_makeAnimMats");            // 33
    virtual void expire(int runscript)
        __asm__("cInstanceNode_expire");                                             // 34 On Tour bool
    virtual void loseFocus()
        __asm__("cInstanceNode_loseFocus");                                                       // 35
    virtual void signal(int signal, float arg)
        __asm__("cInstanceNode_signal__FPv");                                     // 36
    virtual void makeActive()
        __asm__("cInstanceNode_makeActive__FPv");                                                      // 37
    virtual bool hasIntersect() const
        __asm__("cInstanceNode_hasIntersect__FPv");                                              // 38
    virtual int intersect(cWorldIntersectType& w, tIntersectInfo* intInfo, int num, int flags)
        __asm__("cInstanceNode_intersect__FPv"); // 39
    virtual void onCollide(const tCollideInfo* tc, const tIntersectInfo* ti, cBeing* pBeing)
        __asm__("cInstanceNode_onCollide__FPv"); // 40
    virtual void setDebounce(float seconds)
        __asm__("cInstanceNode_setDebounce__FPv");                                        // 41
    virtual void updateIntersectInfo(tIntersectInfo& info)
        __asm__("cInstanceNode_updateIntersectInfo__FPv");                         // 42
    virtual void applyForce(const tIntersectInfo& intInfo, const t4Vector& force)
        __asm__("cInstanceNode_applyForce__FPv");  // 43
    virtual bool hasBoundBox()
        __asm__("cInstanceNode_hasBoundBox__FPv");                                                     // 44
    virtual const tBoundBox* getBoundBox() const
        __asm__("cInstanceNode_getBoundBox__FPv");                                   // 45
    virtual bool isFlexRail() const
        __asm__("cInstanceNode_isFlexRail__FPv");                                                // 46
    // --- cInstanceNode ---
    tInstanceRecord* Instance;   // 0x018 [P] getInstance (lw 0x18); [O] cInstanceNode::Instance; [R] cInstanceNode_cInstanceNode1 stores
                                 //       its record arg; [U] record flags at +0x8, root transform at +0x10, model at +0x80

    // Methods bound to their retail symbols by asm labels.
    void* cInstanceNode1(void* bucket, int type, cInstanceNode* node) __asm__("cInstanceNode_cInstanceNode1");
    void setActiveNode(tInstanceRecord* inst) __asm__("cInstanceNode_setActiveNode");
    void clearActiveNode(tInstanceRecord* inst) __asm__("cInstanceNode_clearActiveNode");
    void runInitScript() __asm__("cInstanceNode_runInitScript");
    void* runExpireScript() __asm__("cInstanceNode_runExpireScript__FPv");
    void* runOnLoseFocusScript() __asm__("cInstanceNode_runOnLoseFocusScript__FPv");
    void makeThisDead() __asm__("cInstanceNode_makeThisDead");
    tInstanceRecord* getInstance() __asm__("cInstanceNode_getInstance");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cInstanceNode_writeToReplayFrame");
    void* runCollideScript(int collideInfo, int intersectInfo, int pBeing)
        __asm__("cInstanceNode_runCollideScript__FPviii");
    virtual unsigned int getSectionId() __asm__("cInstanceNode_getSectionId__FPv");
    virtual void update() __asm__("cInstanceNode_update");
};

CHECK_OFFSET(cInstanceNode, Instance, 0x18);
// [R] cDeadNode (adds no fields: its ctor cDeadNode_cDeadNode writes only 0xC and the record) is allocated with
// cMemMan_alloc(0x1C, "DeadNode") in cInstanceNode_makeThisDead, debouncenode.cpp and wscriptman.cpp; [U] cMoveNode's first own
// field (ModifierBlock, On Tour 0x18) is at 0x1C in cMoveNode_cMoveNode.
CHECK_SIZE(cInstanceNode, 0x1C);

#endif /* OBJECT_INSTANCENODE_H */
