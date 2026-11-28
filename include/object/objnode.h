#ifndef OBJECT_OBJNODE_H
#define OBJECT_OBJNODE_H
#pragma interface

// cObjNode: the root of every scene node - a member of one cBucketMan bucket list (gBucketMan; the buckets are
// updated/rendered in order) with the base set of virtuals (update, apply, render, renderModel, compare, equal,
// getSortIndex, hasParent, isType, getSectionId, write/readFromReplayFrame). Base of cTypeObjNode (-> cSortObjNode ->
// cInstanceNode / cRailManNode) and of the manager nodes cGame_load creates (cTransitionMan, cSnowFall,
// cSplashManager, cFlagManager, cSkyBox, cVisualEffectsObjNode, ...). EA's source:
// d:/ssx/code/source/object/objnode.cpp and objnode.h (preview assert paths; objnode.cpp also holds cTypeObjNode,
// cSortObjNode, which follow below). Retail methods: object/spring (cObjNode_cObjNode1),
// object/objnode (replay ctor, dtor) and the one-line virtuals in object/railmodifier (0x00360700..0x00360780).
// Size 0x10: [R] cBucketMan_init allocates 0x44-byte buckets = a Flags word + four cObjNode sentinels at +0x4, +0x14,
// +0x24, +0x34, each given cObjNode_vtbl at its +0xC (cBucketMan_purge runs cObjNode__cObjNode on the same four);
// [U] the first derived member sits at 0x10 (cTypeObjNode_cTypeObjNode1 sh 0x10; cTransitionMan's cScriptObject).
// On Tour's cObjNode is 0xC (Next 0, Prev 4, vptr 8): retail has one extra leading word that no ctor writes.
// Base classes: none. It declares the first virtuals, so the vptr follows its data (g++ 2.95): the vptr at 0xC.
// Constructors: cObjNode_cObjNode1 (bucket: stores cObjNode_vtbl, cBucketMan_add(&gBucketMan, this, bucket)),
// cObjNode_cObjNode__FP8cObjNodePv (bucket, cBxReplayFrame*: the same, the frame unused), cObjNode_cObjNode2
// (cObjNode(): Next = Prev = 0 and the vtable, no bucket; an unreferenced inline copy). Destructor cObjNode__cObjNode
// (cBucketMan_remove). Vtable cObjNode_vtbl (0x00491F00, 13 slots; slot 1 dtor .. 13 readFromReplayFrame).
//
// A real polymorphic class (its data FLAT, every field at its retail offset), like cTypeObjNode and cSortObjNode
// below, which derive from it. Evidence tags:
//   [R] retail ctors/dtor (above) and cBucketMan_init / cBucketMan_purge (src/object/bucketman.cpp);
//   [P] the July 2003 preview: cObjNode() zeroes 0x4/0x8; tools/layout_xlate.py pairs 9 methods, vptr 0xC == 0xC;
//   [U] how retail code uses the field (bucketman: cBucketMan_add/remove/unlink/first/next/prev/last);
//   [O] SSX On Tour's DWARF (cObjNode: Next, Prev).

#include "layout.h"
#include "bx/memman.h"

struct t4Matrix;
struct cBxReplayFrame;

struct cObjNode {
    int unk_00;                     // 0x00 no ctor writes it (retail cObjNode_cObjNode1/2, the replay ctor, the
                                    //      preview's cObjNode()); no On Tour twin; inside every bucket sentinel too
    cObjNode* Next;                 // 0x04 [O] cObjNode::Next; [R] cObjNode_cObjNode2 zeroes it (sw $0 0x4); [P]; [U]
                                    //      the forward link: cBucketMan_first/next read +0x4 and stop at the bucket's
                                    //      Tail sentinel (+0x14); cBucketMan_unlink zeroes it
    cObjNode* Prev;                 // 0x08 [O] cObjNode::Prev; [R] cObjNode_cObjNode2 zeroes it (sw $0 0x8); [P]; [U]
                                    //      the back link: cBucketMan_last/prev read +0x8, cBucketMan_add links it
    // 0x0C: the vptr. [R] cObjNode_vtbl (0x00491F00) in every ctor and the dtor; each derived ctor stores its own;
    // [P] 0xC; [U] cSortObjNode_compare/equal call slot 8 (lw 0xC, then 0x40/0x44 of the table). Slots [R]
    // cObjNode_vtbl (13; names from its entries cObjNode_*), types [O] (On Tour's cObjNode definitions):
    virtual ~cObjNode();   // 1; g++'s own symbol _$_8cObjNode (the retail label renamed)
    virtual void update() __asm__("cObjNode_update__FPv");                                          // 2
    virtual void apply() __asm__("cObjNode_apply__FPv");                                           // 3
    virtual void render() __asm__("cObjNode_render__FPv");                                          // 4
    virtual void renderModel(const t4Matrix* mat, int flags) __asm__("cObjNode_renderModel__FPv");       // 5
    virtual bool compare(const cObjNode* p) __asm__("cObjNode_compare__FPv");                        // 6
    virtual bool equal(const cObjNode* p) __asm__("cObjNode_equal__FPv");                          // 7
    virtual unsigned int getSortIndex() const __asm__("cObjNode_getSortIndex__FPv");                      // 8
    virtual bool hasParent() const __asm__("cObjNode_hasParent__FPv");                                 // 9
    virtual bool isType(int type) __asm__("cObjNode_isType__FPv");                                  // 10
    virtual unsigned int getSectionId();                            // 11
    virtual void writeToReplayFrame(cBxReplayFrame* data) __asm__("cObjNode_writeToReplayFrame__FPv");          // 12
    virtual void readFromReplayFrame(cBxReplayFrame* data) __asm__("cObjNode_readFromReplayFrame__FPv");         // 13

    // Methods bound to their retail symbols by asm labels.
    void* cObjNode2() __asm__("cObjNode_cObjNode2");
    cObjNode* cObjNode1(void* bucket) __asm__("cObjNode_cObjNode1");
};

CHECK_OFFSET(cObjNode, unk_00, 0x00);
CHECK_OFFSET(cObjNode, Next, 0x04);
CHECK_OFFSET(cObjNode, Prev, 0x08);
CHECK_SIZE(cObjNode, 0x10);
__asm__("_vt$8cObjNode = cObjNode_vtbl");   // g++'s name for the retail vtable

// ---- cTypeObjNode ----

// cTypeObjNode: a scene node that knows its node type (tObjNodeType: 1 anim object, 3 anim combo, 6 unsorted, 9 fence,
// 20/21 float/spring rail, ...) and keeps a per-type live count (static ObjNodeCount[23] = cTypeObjNode_ObjNodeCount), plus a
// flags word whose bit 0 is RootTransformDirty. Base of cSortObjNode (-> cInstanceNode, cRailManNode). EA's source:
// d:/ssx/code/source/object/objnode.cpp (preview assert path; EA declares it in objnode.h with cObjNode/cSortObjNode -
// all three are in this file). Retail methods: object/objnode (ctors, dtor,
// writeToReplayFrame; 0x00354720..0x00354948) and the one-liners in object/railmodifier (0x00360788..0x003607E8).
// Size 0x14: [R] the replay ctor cTypeObjNode_cTypeObjNode reads exactly 4 bytes (cTypeObjNodeState) into +0x10 and
// writeToReplayFrame writes the same 4; cSortObjNode_cSortObjNode1 stores the next member at 0x14; [O] On Tour 0x10
// (its cObjNode is 4 bytes smaller). No allocation of the class itself.
// Bases: cObjNode (0x00-0x10, above), then cTypeObjNodeState (On Tour, 4 bytes) at 0x10 (its two fields written out).
// Constructors: cTypeObjNode_cTypeObjNode1 (bucket, type: cObjNode_cObjNode1, stores the type, Flags = 0,
// ObjNodeCount[type]++), cTypeObjNode_cTypeObjNode (bucket, cBxReplayFrame*); destructor cTypeObjNode__cTypeObjNode
// (ObjNodeCount[type]--). Vtable cTypeObjNode_vtbl (0x00491E80, 14 slots: cObjNode's 13 with 1 dtor, 2 update,
// 4 render, 10 isType, 12 writeToReplayFrame overridden, and the new slot 14 shouldRebound).
//
// Derives from cObjNode (above); its own members FLAT. Evidence tags:
//   [R] retail ctors/dtor (src/object/objnode.cpp);
//   [P] the July 2003 preview's inline accessors via tools/layout_xlate.py (preview == retail): getType lh 0x10, and
//       is/set/resetRootTransformDirty lhu/sh 0x12 bit 0 (retail keeps them out of line in object/railmodifier);
//   [U] how retail code uses the field (cTypeObjNode_isType / getType / *RootTransformDirty, cMoveNode, cAnimNode);
//   [O] SSX On Tour's DWARF (cObjNode, cTypeObjNodeState { short ObjNodeType; short Flags; }).


struct cObjNode;

// A node's type (cTypeObjNode::ObjNodeType, isType's argument): [O] enum tObjNodeType, names and values. [U] the node
// constructors pass them (cAnimObjectNode 1, cAnimDeltaNode 2, cAnimComboNode 3, cAnimTeeterNode 4), and
// checkActiveNode (seg/seg_1FBE38) builds a cParticleNode for 13, a cMeshAnimNode for 11, a cDebounceNode for 0 and a
// cDeadFadeNode for 22.
enum tObjNodeType {
    ATYPE_DEBOUNCE = 0,
    ATYPE_ANIMOBJECT = 1,
    ATYPE_ANIMDELTA = 2,
    ATYPE_ANIMCOMBO = 3,
    ATYPE_ANIMTEETER = 4,
    ATYPE_FLEXBRIDGE = 5,
    ATYPE_DEAD = 6,
    ATYPE_COUNTER = 7,
    ATYPE_BOOST = 8,
    ATYPE_FENCE = 9,
    ATYPE_FLAG = 10,
    ATYPE_MESHANIM = 11,
    ATYPE_TRICKTRIGGER = 12,
    ATYPE_PARTICLE = 13,
    ATYPE_FLOATING = 14,
    ATYPE_CONVEYOR = 15,
    ATYPE_HIDE = 16,
    ATYPE_OBJECT = 17,
    ATYPE_RAILMAN = 18,
    ATYPE_RESTORE = 19,
    ATYPE_FLOATRAIL = 20,
    ATYPE_SPRINGRAIL = 21,
    ATYPE_DEADFADE = 22,
    kNumObjNodeType = 23
};

struct cTypeObjNode : public cObjNode {
    // cObjNode (above, 0x10): the vptr at 0x0C holds [R] cTypeObjNode_vtbl (0x00491E80) in both ctors and the dtor.
    // Its one new slot ([R] cTypeObjNode_vtbl, [O] cTypeObjNode::shouldRebound):
    virtual bool shouldRebound() __asm__("cTypeObjNode_shouldRebound__FPv");   // 14
    // --- cTypeObjNodeState (On Tour 4 bytes; [R] the replay ctor / writeToReplayFrame move these 4 bytes)
    short ObjNodeType;              // 0x10 [O] cTypeObjNodeState::ObjNodeType (tObjNodeType); [R] cTypeObjNode1 sh the
                                    //      type arg, then lh it to index ObjNodeCount; the dtor lh it to decrement;
                                    //      [P] getType (lh 0x10); [U] cTypeObjNode_isType / getType (lh 0x10)
    unsigned short Flags;           // 0x12 [O] cTypeObjNodeState::Flags (short); [R] cTypeObjNode1 zeroes it (sh $0);
                                    //      [P] bit 0 = RootTransformDirty; [U] set/is/resetRootTransformDirty (lhu/sh,
                                    //      | 1, & 1, & 0xFFFE)

    // Methods bound to their retail symbols by asm labels.
    virtual ~cTypeObjNode();   // g++'s own symbol _$_12cTypeObjNode (the retail label renamed)
    void* cTypeObjNode1(void* bucket, int type) __asm__("cTypeObjNode_cTypeObjNode1");
    short getType() __asm__("cTypeObjNode_getType");
    void setRootTransformDirty() __asm__("cTypeObjNode_setRootTransformDirty");
    int isRootTransformDirty() __asm__("cTypeObjNode_isRootTransformDirty");
    void resetRootTransformDirty() __asm__("cTypeObjNode_resetRootTransformDirty");
    virtual void writeToReplayFrame(cBxReplayFrame* obj) __asm__("cTypeObjNode_writeToReplayFrame");
    virtual void update() __asm__("cTypeObjNode_update__FPv");
    virtual void render() __asm__("cTypeObjNode_render__FPv");
    virtual bool isType(int type) __asm__("cTypeObjNode_isType");
};

CHECK_OFFSET(cTypeObjNode, ObjNodeType, 0x10);
CHECK_OFFSET(cTypeObjNode, Flags, 0x12);
CHECK_SIZE(cTypeObjNode, 0x14);
__asm__("_vt$12cTypeObjNode = cTypeObjNode_vtbl");   // g++'s name for the retail vtable

// ---- cSortObjNode ----

// cSortObjNode: a typed scene node with a creation-order sort index, so the bucket manager can keep a bucket sorted
// (cBucketMan_mergeBucket calls compare/equal, virtual slots 6/7). The index comes from two static counters
// (cSortObjNode_ObjNodeSortIndex 0x004A5B78, cSortObjNode_ObjNodeNoSortIndex 0x004A5B7C; type 6 nodes draw from the
// second); init/nextSortIndex/nextNoSortIndex/get-/setStatic*SortIndex are static functions (no `this`). Base of
// cInstanceNode and cRailManNode. EA's source: d:/ssx/code/source/object/objnode.cpp (preview assert path; declared
// in objnode.h with cObjNode/cTypeObjNode). Retail methods: object/objnode (ctors, dtor, writeToReplayFrame;
// 0x00354850..0x00354980) and object/railmodifier (0x00360800..0x003608E0).
// Size 0x18: [R] the replay ctor cSortObjNode_cSortObjNode reads exactly 4 bytes (cSortObjNodeState) into +0x14 after
// the cTypeObjNode part; cInstanceNode_cInstanceNode1 stores its own first member (Instance) at 0x18; cRailManNode's
// state starts at 0x18; [O] On Tour 0x14 (its cObjNode is 4 bytes smaller). No allocation of the class itself.
// Bases: cTypeObjNode (0x00-0x14, above; itself cObjNode 0x00-0x10 + cTypeObjNodeState), then
// cSortObjNodeState (On Tour, 4 bytes) at 0x14 (its field written out).
// Constructors: cSortObjNode_cSortObjNode1 (bucket, type: SortIndex = ObjNodeNoSortIndex++ for type 6, else
// ObjNodeSortIndex++), cSortObjNode_cSortObjNode (bucket, cBxReplayFrame*); destructor cSortObjNode__cSortObjNode__FPv.
// Vtable cSortObjNode_vtbl (0x00491E00, 14 slots: cTypeObjNode's with 1 dtor, 6 compare, 7 equal, 8 getSortIndex,
// 12 writeToReplayFrame overridden).
//
// Derives from cTypeObjNode (above); its own members FLAT. Evidence tags:
//   [R] retail ctors/dtor (src/object/objnode.cpp);
//   [P] the July 2003 preview's inline accessor getSortIndex (lw 0x14) via tools/layout_xlate.py (preview == retail);
//   [U] how retail code uses the field (cSortObjNode_compare / equal / getSortIndex);
//   [O] SSX On Tour's DWARF (cObjNode, cTypeObjNodeState, cSortObjNodeState { unsigned int SortIndex; }).


struct cObjNode;

struct cSortObjNode : public cTypeObjNode {
    // cTypeObjNode (above, 0x14): the vptr at 0x0C holds [R] cSortObjNode_vtbl (0x00491E00) in both ctors and the
    // dtor (no new slots: it overrides compare, equal, getSortIndex and writeToReplayFrame); [U] compare / equal call
    // the other node's getSortIndex().
    // --- cSortObjNodeState (On Tour 4 bytes; [R] the replay ctor / writeToReplayFrame move these 4 bytes)
    unsigned int SortIndex;         // 0x14 [O] cSortObjNodeState::SortIndex; [R] cSortObjNode1 sw the counter's old
                                    //      value; [P] getSortIndex (lw 0x14); [U] compare: other.getSortIndex() >=
                                    //      SortIndex (sltu: unsigned), equal: ==

    // Methods bound to their retail symbols by asm labels.
    virtual ~cSortObjNode();   // g++'s own symbol _$_12cSortObjNode (the retail label renamed)
    void* cSortObjNode1(void* bucket, int type) __asm__("cSortObjNode_cSortObjNode1");
    // getSortIndex stays a C function (cSortObjNode_getSortIndex): declared first, it is the vtable's key function,
    // which keeps the vtable out of the units defining the other overrides
    virtual unsigned int getSortIndex() const __asm__("cSortObjNode_getSortIndex__FPv");
    virtual bool compare(const cObjNode* obj) __asm__("cSortObjNode_compare");
    virtual bool equal(const cObjNode* obj) __asm__("cSortObjNode_equal");
    virtual void writeToReplayFrame(cBxReplayFrame* obj) __asm__("cSortObjNode_writeToReplayFrame");
};

CHECK_OFFSET(cSortObjNode, SortIndex, 0x14);
CHECK_SIZE(cSortObjNode, 0x18);
__asm__("_vt$12cSortObjNode = cSortObjNode_vtbl");   // g++'s name for the retail vtable

#endif /* OBJECT_OBJNODE_H */
