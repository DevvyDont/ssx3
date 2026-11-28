#ifndef OBJECT_COUNTERNODE_H
#define OBJECT_COUNTERNODE_H
#pragma interface

// cCounterNode: a world-script counter on an instance: it starts at StartCount and counts down on signals - 502 always,
// 500 once per level 1..10 (Counted keeps a bit per level seen), 501 only in order (level 1, or a level whose
// predecessor bit is set); when it reaches 0, update sets it to -1 and expires the node (slot 34, which runs the
// instance's expire script). Node type 7. Made by MakeCounterFunc (seg/seg_1FBE38) and the replay loader. No EA source
// path in the preview asserts; retail methods: object/debouncenode (0x00343768..0x00343A64) and the dtor in
// object/railmodifier (0x00361530).
// Size 0x24: [R] cMemMan_alloc(0x24, "Counter") in cReplay_restoreObject (src/replay/replay.cpp) and cMemMan_alloc(0x24)
// in MakeCounterFunc; the replay ctor reads the 8-byte cCounterNodeState into +0x1C (and writeToReplayFrame writes
// it); [O] On Tour 0x20 = cInstanceNode 0x18 + cCounterNodeState 0x8 (state after the base, as here).
// Bases: cInstanceNode (0x00-0x1C, object/instancenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode), then
// cCounterNodeState (On Tour 0x8) at 0x1C (the state written out). The vptr is at 0xC.
// Constructors: cCounterNode_cCounterNode1 (bucket, record, tMakeCounterData*: cInstanceNode_cInstanceNode1(this,
// bucket, 7, record), vtable, Count = data StartCount, Counted = 0) and cCounterNode_cCounterNode (bucket,
// cBxReplayFrame*: cInstanceNode_cInstanceNode, vtable, the 8-byte state). Destructor cCounterNode__cCounterNode__FPv
// (vtable, cInstanceNode__cInstanceNode). Vtable cCounterNode_vtbl (0x004903F0, 46 slots: 1 dtor, 2 update, 12
// writeToReplayFrame, 36 signal; the rest are cInstanceNode's / cSortObjNode's / cTypeObjNode's / cObjNode's).
//
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/object/debouncenode.cpp, src/object/railmodifier.cpp) and the 0x24 allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py: 3 paired methods, 0xC / 0x1C / 0x20 (preview == retail);
//   [U] how retail code uses the field (src/object/debouncenode.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cInstanceNode chain, cCounterNodeState { int Count; int Counted; }, tMakeCounterData).

#include "layout.h"
#include "wscript/wscriptnull.h"
#include "object/instancenode.h"

struct cObjNode;
struct tInstanceRecord;

// World-script data that makes a counter (On Tour tMakeCounterData, 0xC). [O] On Tour's cCounterNode(bucket,
// tInstanceRecord* pInst, tMakeCounterData* cdata); [U] cCounterNode_cCounterNode1 (src/object/debouncenode.cpp):
// Count = StartCount as a short (lh +0x4; On Tour and the script defaults, MakeCounterFunc's sw 3, make it an int).
struct tMakeCounterData {
    unsigned int InstId;            // 0x00 [O] tResourceID; [U] MakeCounterFunc's instance lookup (-1: the script's
                                    //      current instance)
    int StartCount;                 // 0x4 [O]; [U] -> Count as a short (lh); [R] MakeCounterFunc's default 3 (sw)
    WScriptNullType null;           // 0x8 [O]
    char unk_009[0x3];              // 0x9 padding
};

CHECK_OFFSET(tMakeCounterData, InstId, 0x0);
CHECK_OFFSET(tMakeCounterData, StartCount, 0x4);
CHECK_OFFSET(tMakeCounterData, null, 0x8);
CHECK_SIZE(tMakeCounterData, 0xC);

struct cCounterNode : public cInstanceNode {
    // The signals a counter counts (names ours): [U] cCounterNode_signal counts kCounterSignal_Level once per level
    // 1..10, kCounterSignal_LevelInOrder once per level and only in order, and kCounterSignal_Count every time; it
    // ignores any other signal.
    enum eCounterSignal {
        kCounterSignal_Level = 500, kCounterSignal_LevelInOrder = 501, kCounterSignal_Count = 502
    };
    // cInstanceNode (object/instancenode.h, 0x1C): the vptr at 0x0C:
    //   [R] cCounterNode_vtbl (0x004903F0) in both ctors and the dtor; [P] 0xC; [U] update calls slot 34 (expire)
    // cCounterNode's notes on cInstanceNode's fields:
    //   ObjNodeType: [R] node type 7 (cInstanceNode_cInstanceNode1)
    // --- cCounterNodeState (On Tour 0x8; [R] the replay ctor reads 8 bytes here, writeToReplayFrame writes them)
    int Count;                      // 0x1C [O]; [P] preview 0x1C; [R] ctor1 = data StartCount (lh data +0x4, sw); [U]
                                    //      update: 0 -> -1 and expire; signal decrements it while > 0 (lw/sw)
    int Counted;                    // 0x20 [O]; [P] preview 0x20; [R] ctor1 0 (sw $0); [U] signal 500/501 test and set
                                    //      one bit per level 1..10 (lw/sw)

    // Methods bound to their retail symbols by asm labels.
    virtual ~cCounterNode();   // g++'s own symbol _$_12cCounterNode (the retail label renamed)
    void* cCounterNode1(void* bucket, void* inst, tMakeCounterData* counterData) __asm__("cCounterNode_cCounterNode1");
    // update stays a C function (cCounterNode_update): declared first, it is the vtable's key function, which keeps the
    // vtable out of the units defining the other overrides
    virtual void update() __asm__("cCounterNode_update");
    virtual void signal(int msg, float level) __asm__("cCounterNode_signal");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cCounterNode_writeToReplayFrame");
};

CHECK_OFFSET(cCounterNode, Count, 0x1C);
CHECK_OFFSET(cCounterNode, Counted, 0x20);
CHECK_SIZE(cCounterNode, 0x24);
__asm__("_vt$12cCounterNode = cCounterNode_vtbl");   // g++'s name for the retail vtable

#endif /* OBJECT_COUNTERNODE_H */
