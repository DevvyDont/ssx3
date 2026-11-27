#ifndef OBJECT_FLAGSET_H
#define OBJECT_FLAGSET_H

// cFlagSet: one set of cloth flags that share a model and wind parameters: up to 60 instances (tInstanceRecord) drawn
// with one grid mesh whose vertices updateGrid animates from the set's sine-wave parameters (tFlagState) and the
// manager's wind. cFlagManager (a cObjNode; cGame_getFlagManager) holds 15 of them at +0x20 (stride 0x188) and routes
// cFlagNode's AddFlag / RemoveFlag to a set of the same kind. EA's source: d:/ssx/code/source/object/flagnode.cpp
// (the preview's asserts "A flag set has to be empty before you can destroy it", "mGridMesh==NULL", "mVerts==NULL",
// "SameKind(tData,pInst)", "mFlagVertsHorizontal < kMaxVerticiesHorizontal",
// "mInstances[i]->m_Model.m_pModel->m_RID == mModelID" follow that path); EA's header is likely flagnode.h - this one
// is named after the splat unit (the cFlagNode staging already uses object/flagnode.h; merge if wanted).
// Retail: splat units object/flexrailnode (tFlagState_tFlagState 0x0034AC88 .. cFlagSet_RemoveFlag 0x0034B168),
// object/flagset (CreateMesh 0x0034B228 .. readFromReplayFrame 0x0034C378, then cFlagManager) and the inline
// cFlagSet_Empty in object/railmodifier (0x00361750).
//
// Size 0x188: [R] cFlagManager_cFlagManager constructs 15 cFlagSet from +0x20 with stride 0x188; [O] On Tour's
// cFlagSet is 0x188 with the same member offsets (no bools; t3Vector is 12 bytes in both).
// Base: tFlagSetState (0x60, On Tour struct; the ctor runs tFlagSetState_tFlagSetState(this) first), written out
// inline; it holds the set's tFlagState (0x58) and model id. cFlagSet declares the first virtual (~cFlagSet), so its
// vptr follows its own data: mVtbl at 0x184 = cFlagSet_vtbl (D_0048FBF8, 1 slot: the dtor).
// tFlagState (0x58) is also built on the stack by cFlagNode's ctor and copied into a set by AddFlag.
//
// FLAT layouts, like main/game.h (cFlagManager derives from cObjNode). Tags: [R] retail ctors (cFlagSet_cFlagSet, tFlagSetState_tFlagSetState,
// tFlagState_tFlagState), the dtor, the allocations; [P] member names in the preview's assert strings (above) and the
// out-of-line inline Empty__8cFlagSet; [U] how retail code uses the field; [O] SSX On Tour's DWARF (tFlagState,
// tFlagSetState, cFlagSet, cFlagManager: member names, order and the same offsets). t3Vector members are float[3].

#include "layout.h"
#include "object/objnode.h"

struct t3Vector;

struct cGridMesh;          // render/ps2gridmesh.h (the mesh is a cPS2GridMesh)
struct tInstanceRecord;

// A flag set's animation parameters (On Tour struct tFlagState, 0x58).
struct tFlagState {
    char mBaseSideHeld;             // 0x000 [O]; [R] tFlagState_tFlagState (sb); [U] updateGrid (lb; also one lhu over
                                    //       0x0-0x1 testing both flags at once)
    char mOppositeSideHeld;         // 0x001 [O]; [R] (sb); [U] updateGrid
    char mPerpSideHeld;             // 0x002 [O]; [R] (sb); [U] updateGrid
    char mOverSaturate;             // 0x003 [O]; [R] (sb); [U] render (lb)
    float mSpeed[4];                // 0x004 [O]; [R] (swc1 x4); [U] updateGrid
    float mMagnitude[4];            // 0x014 [O]; [R]; [U] updateGrid; CreateMesh sizes the grid from [2], [3]
    float mFrequency[4];            // 0x024 [O]; [R]; [U] updateGrid
    float mLowWindMovement[3];      // 0x034 [O] t3Vector; [R]; [U] updateGrid
    float mHighWindMovement[3];     // 0x040 [O] t3Vector; [R]; [U] updateGrid
    float mMinWindFactor;           // 0x04C [O]; [R]; [U] updateGrid
    float mUScrollRate;             // 0x050 [O]; [R]; [U] update
    float mVScrollRate;             // 0x054 [O]; [R]; [U] update
};

CHECK_OFFSET(tFlagState, mBaseSideHeld, 0x000);
CHECK_OFFSET(tFlagState, mOppositeSideHeld, 0x001);
CHECK_OFFSET(tFlagState, mPerpSideHeld, 0x002);
CHECK_OFFSET(tFlagState, mOverSaturate, 0x003);
CHECK_OFFSET(tFlagState, mSpeed, 0x004);
CHECK_OFFSET(tFlagState, mMagnitude, 0x014);
CHECK_OFFSET(tFlagState, mFrequency, 0x024);
CHECK_OFFSET(tFlagState, mLowWindMovement, 0x034);
CHECK_OFFSET(tFlagState, mHighWindMovement, 0x040);
CHECK_OFFSET(tFlagState, mMinWindFactor, 0x04C);
CHECK_OFFSET(tFlagState, mUScrollRate, 0x050);
CHECK_OFFSET(tFlagState, mVScrollRate, 0x054);
CHECK_SIZE(tFlagState, 0x058);

struct cBxReplayFrame;

struct cFlagSet {
    // --- tFlagSetState (On Tour struct, 0x60)
    tFlagState mFlagData;           // 0x000 [O]; [U] AddFlag copies the caller's tFlagState into it (ldl/ldr, 0x58
                                    //       bytes); SameKind compares it; update / render / updateGrid / CreateMesh read it
    unsigned int mModelID;          // 0x058 [O] tResourceID; [P] assert "...m_RID == mModelID"; [R] tFlagSetState
                                    //       ctor: -1; [U] SameKind, AddFlag, RemoveFlag
    int mCurrentNumInstances;       // 0x05C [O]; [R] tFlagSetState ctor: 0; [P] Empty__8cFlagSet; [U] Empty, AddFlag,
                                    //       RemoveFlag, update, render, write/readFromReplayFrame
    // --- cFlagSet
    cGridMesh* mGridMesh;           // 0x060 [O]; [P] assert "mGridMesh==NULL"; [R] ctor: 0; [U] CreateMesh:
                                    //       GrMan->CreateGridMesh(horizontal, vertical, ...); PurgeMesh; render
    int mFlagVertsHorizontal;       // 0x064 [O]; [P] assert "mFlagVertsHorizontal < kMaxVerticiesHorizontal"; [U]
                                    //       CreateMesh, updateGrid
    int mFlagVertsVertical;         // 0x068 [O]; [P] assert; [U] CreateMesh, updateGrid
    float mfFlagVertsHorizontalMinusOne; // 0x06C [O]; [U] CreateMesh (swc1), updateGrid
    float mfFlagVertsVerticalMinusOne;   // 0x070 [O]; [U] CreateMesh, updateGrid
    t3Vector* mVerts;               // 0x074 [O] t3Vector*; [P] assert "mVerts==NULL"; [R] ctor: 0; [U] CreateMesh:
                                    //       operator new[](h * v * 0xC, "Flag Verts"); PurgeMesh frees it; updateGrid
    float mFlagTime[4];             // 0x078 [O]; [U] CreateMesh randomizes (BXrand), updateGrid
    float mScrollU;                 // 0x088 [O]; [U] CreateMesh zeroes; update advances; render (lwc1)
    float mScrollV;                 // 0x08C [O]; [U] ditto
    int mFrameToUpdateGrid;         // 0x090 [O]; [R] ctor: the static counter mgFrameToUpdateGridCounter (cFlagSet_mgFrameToUpdateGridCounter,
                                    //       cycles 0..1)
    tInstanceRecord* mInstances[60]; // 0x094 [O]; [P] assert "mInstances[i]->m_Model..."; [R] ctor zeroes 60 words
                                    //       (0x180 down to 0x94); [U] AddFlag / RemoveFlag / render / replay
    void** mVtbl;                   // 0x184 [R] cFlagSet_cFlagSet and __cFlagSet store cFlagSet_vtbl (D_0048FBF8)

    // Methods bound to their retail symbols by asm labels.
    void CreateMesh(tInstanceRecord* item) __asm__("cFlagSet_CreateMesh");
    void PurgeMesh() __asm__("cFlagSet_PurgeMesh");
    void update() __asm__("cFlagSet_update");
    void render() __asm__("cFlagSet_render");
    void updateGrid(t3Vector* vout) __asm__("cFlagSet_updateGrid");
    void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cFlagSet_writeToReplayFrame");
    void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cFlagSet_readFromReplayFrame");
    bool SameKind(void* data, tInstanceRecord* ctx) __asm__("cFlagSet_SameKind");
    void AddFlag(tFlagState* def, tInstanceRecord* mesh) __asm__("cFlagSet_AddFlag");
    bool Empty() __asm__("cFlagSet_Empty");
    bool RemoveFlag(tInstanceRecord* rider) __asm__("cFlagSet_RemoveFlag");
};

CHECK_OFFSET(cFlagSet, mFlagData, 0x000);
CHECK_OFFSET(cFlagSet, mModelID, 0x058);
CHECK_OFFSET(cFlagSet, mCurrentNumInstances, 0x05C);
CHECK_OFFSET(cFlagSet, mGridMesh, 0x060);
CHECK_OFFSET(cFlagSet, mFlagVertsHorizontal, 0x064);
CHECK_OFFSET(cFlagSet, mFlagVertsVertical, 0x068);
CHECK_OFFSET(cFlagSet, mfFlagVertsHorizontalMinusOne, 0x06C);
CHECK_OFFSET(cFlagSet, mfFlagVertsVerticalMinusOne, 0x070);
CHECK_OFFSET(cFlagSet, mVerts, 0x074);
CHECK_OFFSET(cFlagSet, mFlagTime, 0x078);
CHECK_OFFSET(cFlagSet, mScrollU, 0x088);
CHECK_OFFSET(cFlagSet, mScrollV, 0x08C);
CHECK_OFFSET(cFlagSet, mFrameToUpdateGrid, 0x090);
CHECK_OFFSET(cFlagSet, mInstances, 0x094);
CHECK_OFFSET(cFlagSet, mVtbl, 0x184);
CHECK_SIZE(cFlagSet, 0x188);


// ---- cFlagManager ----

// cFlagManager: the scene node (a cObjNode in bucket 2) that owns the 15 cFlagSets and the shared wind: update advances
// a random walk of the wind factor (mWindBase + mWindDelta * mWindTime, re-rolled once a second with an amplitude from
// the peak number) and updates the sets that have instances; render draws them; cFlagNode's ctor / dtor route
// AddFlag / RemoveFlag through it (cObjectInterface_getFlagManager). The game owns one (cGame::FlagManager,
// main/game.h). Retail methods: object/flagset (0x0034C428..0x0034C8E8) and the inline GetWindFactor in
// object/railmodifier (0x00361760). Vtable cFlagManager_vtbl (0x0048FB80, 13 slots: cObjNode's with 1 dtor, 2 update,
// 4 render, 12 writeToReplayFrame, 13 readFromReplayFrame overridden).
// Size 0x1718: [R] cGame_load allocates operator new(5912 = 0x1718, "cFlagManager") + cFlagManager_cFlagManager(p, 2),
// and the ctor builds 15 cFlagSet at +0x20 with stride 0x188 (0x20 + 15 * 0x188 = 0x1718); the dtor's array cleanup
// walks back from +0x1718. [O] On Tour's cFlagManager is 0x1714 with mFlagSets at 0x1C: its cObjNode is 0xC, retail's
// 0x10 (object/objnode.h), so every member sits 4 bytes later in retail.
// Base: cObjNode (0x00-0x10, object/objnode.h), written out inline. cObjNode declares the virtuals, so the vptr is
// cObjNode's at 0xC.
// Tags as above, plus [P] the preview's GetWindFactor__12cFlagManager (lwc1 0x10; tools/layout_xlate.py: preview ==
// retail for 0xC..0x1C).

struct cObjNode;

struct cFlagNode;

struct cFlagManager : public cObjNode {
    // cObjNode (object/objnode.h, 0x10): the vptr at 0x0C:
    //   [R] cFlagManager_vtbl (0x0048FB80) in the ctor and the dtor
    // --- cFlagManager
    float mCurrentWindFactor;       // 0x010 [O]; [P] GetWindFactor (lwc1 0x10); [R] ctor 0 (sw $0); [U] update stores
                                    //       mWindBase + mWindDelta * mWindTime; cFlagManager_GetWindFactor returns it
    float mWindBase;                // 0x014 [O]; [R] ctor 0.5; [U] update adds mWindDelta once a second
    float mWindDelta;               // 0x018 [O]; [R] ctor 0.25; [U] update re-rolls it in [-amp, amp), clamped so
                                    //       base + delta stays in [0, 1]
    float mWindTime;                // 0x01C [O]; [R] ctor 0 (sw $0); [U] update adds 1 / frame rate and wraps at 1
    cFlagSet mFlagSets[15];         // 0x020 [O] kNumFlagSets = 15; [R] ctor: cFlagSet_cFlagSet on each (stride 0x188);
                                    //       dtor: each set's virtual dtor (slot 1, vptr at +0x184) in reverse;
                                    //       [U] AddFlag / RemoveFlag / update / render test mCurrentNumInstances (+0x5C);
                                    //       write/readFromReplayFrame call the sets' replay methods

    // Methods bound to their retail symbols by asm labels.
    void AddFlag(cFlagNode* node, void* desc) __asm__("cFlagManager_AddFlag");
    void RemoveFlag(tInstanceRecord* instRecord) __asm__("cFlagManager_RemoveFlag");
    float GetWindFactor() __asm__("cFlagManager_GetWindFactor");
    // retail's destructor cFlagManager__cFlagManager stays a C function (g++ 2.95 takes no asm label on a destructor);
    // declared first, it keeps the vtable out of the units defining the overrides
    virtual ~cFlagManager();
    virtual void update() __asm__("cFlagManager_update");
    virtual void render() __asm__("cFlagManager_render");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cFlagManager_writeToReplayFrame");
    virtual void readFromReplayFrame(cBxReplayFrame* stream) __asm__("cFlagManager_readFromReplayFrame");
};

CHECK_OFFSET(cFlagManager, mCurrentWindFactor, 0x010);
CHECK_OFFSET(cFlagManager, mWindBase, 0x014);
CHECK_OFFSET(cFlagManager, mWindDelta, 0x018);
CHECK_OFFSET(cFlagManager, mWindTime, 0x01C);
CHECK_OFFSET(cFlagManager, mFlagSets, 0x020);
CHECK_SIZE(cFlagManager, 0x1718);

#endif /* OBJECT_FLAGSET_H */
