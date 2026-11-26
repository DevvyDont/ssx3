#ifndef OBJECT_MOVENODE_H
#define OBJECT_MOVENODE_H

// cMoveNode: the scene node of a placed instance that can move or carry modifiers (tModifierBlock: root-transform,
// UV-scroll, tex-flip, physics, effect and rail modifiers); base of cFloatingNode, cAnimNode, cParticleNode,
// cObjectNode, cDebounceNode, ... (not of cFlexRailNode / cFlexBridgeNode: their ctors call
// cInstanceNode_cInstanceNode1 directly). EA's source: d:/ssx/code/source/object/movenode.cpp.
// Size 0x2C: subclasses that add no members are allocated 0x2C in retail - cMemMan_alloc(0x2C, "ParticleNode") and
// (0x2C, "cParticleNode") for cParticleNode (cParticleNode_cParticleNode1), (0x2C, "Object") for cObjectNode (cObjectNode_cObjectNode); On Tour has
// cParticleNode/cObjectNode == cMoveNode == 0x28 (its cObjNode is 4 bytes smaller, see object/instancenode.h).
// Constructors cMoveNode_cMoveNode1 (bucket, type, inst: sets RootTransformDirty) and cMoveNode_cMoveNode, the replay-frame
// one (reads Debounce/Fade/TransferHack as three 4-byte blocks at +0x20/+0x24/+0x28, then
// tModifierBlock_readFromReplayFrame);
// destructor cMoveNode__cMoveNode; vtable cMoveNode_vtbl. Methods: src/object/movenode.cpp and the one-liners in
// src/object/railmodifier.cpp (methods.txt).
// Derives from cInstanceNode (0x00-0x1C, object/instancenode.h); its own fields FLAT: On Tour's members in On Tour's
// order, each 4 bytes later (TransferHack is a 4-byte bool).
// Evidence tags: [P] preview inline accessors (via instancenode.h); [R] ctors/dtor/allocation; [U] retail use; [O] On Tour.

#include "layout.h"
#include "object/instancenode.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;

// World-script data that the cMoveNode add* functions take (On Tour tAddFadeData / tAddDebounceData 0xC,
// tAddRailData 0x10; same layouts). [O] On Tour's cMoveNode::addFade(const tAddFadeData&), addDebounce(const
// tAddDebounceData&), addRail(const tAddRailData&) (cRailModifier(const tAddRailData&, ...)); [U] cMoveNode_addFade /
// addDebounce / addRail (src/object/movenode.cpp) and cRailModifier_cRailModifier (src/object/splinemodifier.cpp)
// read the members below.
struct tAddFadeData {
    unsigned int unk_000;           // 0x0 [O] tResourceID InstId (no retail read)
    float Fade;                     // 0x4 [O]; [U] cMoveNode_addFade's fade value
    char unk_008[0x4];              // 0x8 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tAddFadeData, unk_000, 0x0);
CHECK_OFFSET(tAddFadeData, Fade, 0x4);
CHECK_OFFSET(tAddFadeData, unk_008, 0x8);
CHECK_SIZE(tAddFadeData, 0xC);

struct tAddDebounceData {
    unsigned int unk_000;           // 0x0 [O] tResourceID InstId (no retail read)
    float Time;                     // 0x4 [O]; [U] cMoveNode_addDebounce passes it to the node's debounce setter
                                    //      (virtual slot 41)
    char unk_008[0x4];              // 0x8 [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tAddDebounceData, unk_000, 0x0);
CHECK_OFFSET(tAddDebounceData, Time, 0x4);
CHECK_OFFSET(tAddDebounceData, unk_008, 0x8);
CHECK_SIZE(tAddDebounceData, 0xC);

struct tAddRailData {
    unsigned int unk_000;           // 0x0 [O] tResourceID InstId (no retail read)
    unsigned int SplineId;          // 0x4 [O] tResourceID; [U] addRail: tModifierBlock_findRail(block, SplineId);
                                    //      cRailModifier_cRailModifier stores it as SplineId
    int Mesh;                       // 0x8 [O]; [U] cRailModifier_cRailModifier stores it as Mesh
    char unk_00C[0x4];              // 0xC [O] WScriptNullType null (1 byte) + padding
};

CHECK_OFFSET(tAddRailData, unk_000, 0x0);
CHECK_OFFSET(tAddRailData, SplineId, 0x4);
CHECK_OFFSET(tAddRailData, Mesh, 0x8);
CHECK_OFFSET(tAddRailData, unk_00C, 0xC);
CHECK_SIZE(tAddRailData, 0x10);

struct cInstanceNode;
struct cPhysicsModifier;
struct cRailModifier;
struct cTexFlipModifier;
struct cTransformModifier;
struct cUVScrollModifier;
struct t4Matrix;
struct t4Vector;

struct cMoveNode : public cInstanceNode {
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C holds [R] cMoveNode_vtbl (0x00491028) or a
    // subclass's (each ctor stores its own); [U] every virtual call (cMoveNode_apply, update, render, ...). Its new
    // slots ([R] cMoveNode_vtbl, 51 in all; names from its entries, types [O] On Tour's cMoveNode definitions):
    virtual void updateAnimState();                                 // 47
    virtual void setupOverlapSystem() __asm__("cMoveNode_setupOverlapSystem");                              // 48
    virtual void restoreOverlapSystem() __asm__("cMoveNode_restoreOverlapSystem");                            // 49
    virtual void updateOverlapSystem() __asm__("cMoveNode_updateOverlapSystem");                             // 50
    virtual void updateRootTransform() __asm__("cMoveNode_updateRootTransform");                             // 51
    // cMoveNode's notes on its bases' fields:
    //   Flags: [R] cMoveNode_cMoveNode1 sets bit 0 (RootTransformDirty); [U] cMoveNode_updateRootTransform,
    //       preUpdateModifierBlock
    //   Instance: [U] the add* builders pass it to the modifier ctors, render, getRootTransform
    // --- cMoveNode
    tModifierBlock* ModifierBlock;  // 0x1C [O]; [R] cMoveNode_addModifierBlock allocates it (cMemMan_alloc(0x28,
                                    //      "ModifierBlock")); [U] every forwarder (cMoveNode_getParentMesh..cMoveNode_fadeEffectModifier, add*/rem*,
                                    //      update, render)
    int Debounce;                   // 0x20 [O] int; [U] setDebounce stores (int)(seconds * fps) (cvt.w.s + swc1),
                                    //      update/onCollide count it down (lw/sw); [R] replay ctor reads 4 bytes
    float Fade;                     // 0x24 [O]; [U] addFade cMoveNode_addFade stores data +0x4, render/renderModel read it
                                    //      (lwc1); [R] replay ctor
    int TransferHack;               // 0x28 [O] bool (4 bytes); [U] render tests it (lw); [R] replay ctor

    // Methods bound to their retail symbols by asm labels.
    void* cMoveNode1(void* bucket, int type, void* instRecord) __asm__("cMoveNode_cMoveNode1");
    void addModifierBlock() __asm__("cMoveNode_addModifierBlock__FP9cMoveNode");
    void addTransformModifier(cTransformModifier* mod) __asm__("cMoveNode_addTransformModifier");
    void addUVScrollModifier(cUVScrollModifier* modifier) __asm__("cMoveNode_addUVScrollModifier");
    void addTexFlipModifier(cTexFlipModifier* modifier) __asm__("cMoveNode_addTexFlipModifier");
    void addPhysicsModifier(cPhysicsModifier* modifier) __asm__("cMoveNode_addPhysicsModifier");
    void addEffectModifier(void* effect) __asm__("cMoveNode_addEffectModifier__FP9cMoveNodePv");
    void addRailModifier(cRailModifier* item) __asm__("cMoveNode_addRailModifier");
    void preUpdateModifierBlock() __asm__("cMoveNode_preUpdateModifierBlock");
    void postUpdateModifierBlock() __asm__("cMoveNode_postUpdateModifierBlock");
    float addFade(void* vfadeData) __asm__("cMoveNode_addFade__FPvT0");
    void addDebounce(tAddDebounceData* data) __asm__("cMoveNode_addDebounce");
    void addPosition1(t4Matrix* desc) __asm__("cMoveNode_addPosition1");
    void addMagnet(t4Matrix* mat, float radius, float magnetSpeed) __asm__("cMoveNode_addMagnet");
    void addAvaSpline() __asm__("cMoveNode_addAvaSpline");
    void addSpline1(void* desc) __asm__("cMoveNode_addSpline1");
    void addSpline(void* desc) __asm__("cMoveNode_addSpline");
    void addUVScroll(void* desc) __asm__("cMoveNode_addUVScroll");
    void addTexFlip(void* desc) __asm__("cMoveNode_addTexFlip");
    void addBoost(void* desc) __asm__("cMoveNode_addBoost");
    void addParticle(void* desc) __asm__("cMoveNode_addParticle");
    void addDynamicParticle(void* desc, int index, t4Vector* offset) __asm__("cMoveNode_addDynamicParticle");
    void addRoller(void* rdata, void* ii, void* ci) __asm__("cMoveNode_addRoller");
    void addRail(tAddRailData* desc) __asm__("cMoveNode_addRail");
    void addHalo(void* desc) __asm__("cMoveNode_addHalo");
    void transferFromOld(cInstanceNode* srcv) __asm__("cMoveNode_transferFromOld");
    void transferEffectModifier(cMoveNode* other) __asm__("cMoveNode_transferEffectModifier");
    int getParentMesh() __asm__("cMoveNode_getParentMesh");
    int getTransformModifier() __asm__("cMoveNode_getTransformModifier");
    int hasTextureMat() __asm__("cMoveNode_hasTextureMat");
    int hasMaterialBlock() __asm__("cMoveNode_hasMaterialBlock");
    int getMaterialBlock() __asm__("cMoveNode_getMaterialBlock");
    void* getTextureMat() __asm__("cMoveNode_getTextureMat");
    void remTransformModifier() __asm__("cMoveNode_remTransformModifier");
    void remUVScrollModifier() __asm__("cMoveNode_remUVScrollModifier");
    void remTexFlipModifier() __asm__("cMoveNode_remTexFlipModifier");
    void remPhysicsModifier() __asm__("cMoveNode_remPhysicsModifier");
    void remHaloModifier() __asm__("cMoveNode_remHaloModifier");
    void remEffectModifier() __asm__("cMoveNode_remEffectModifier");
    void fadeEffectModifier() __asm__("cMoveNode_fadeEffectModifier");
    virtual bool shouldRebound() __asm__("cMoveNode_shouldRebound");
    virtual void setDebounce(float secs) __asm__("cMoveNode_setDebounce");
    virtual void update() __asm__("cMoveNode_update");
    virtual void render() __asm__("cMoveNode_render");
    virtual bool hasBoundBox() __asm__("cMoveNode_hasBoundBox");
    virtual bool hasIntersect() const __asm__("cMoveNode_hasIntersect");
    virtual bool hasFocusCount() const __asm__("cMoveNode_hasFocusCount");
    virtual int getFocusCount() const __asm__("cMoveNode_getFocusCount");
};

CHECK_OFFSET(cMoveNode, ModifierBlock, 0x1C);
CHECK_OFFSET(cMoveNode, Debounce, 0x20);
CHECK_OFFSET(cMoveNode, Fade, 0x24);
CHECK_OFFSET(cMoveNode, TransferHack, 0x28);
CHECK_SIZE(cMoveNode, 0x2C);

#endif /* OBJECT_MOVENODE_H */
