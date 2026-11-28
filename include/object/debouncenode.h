#ifndef OBJECT_DEBOUNCENODE_H
#define OBJECT_DEBOUNCENODE_H
#pragma interface

// cDebounceNode: a temporary node the world script puts on an instance (MakeDebounceFunc, tMakeDebounceData) to make
// it ignore further hits for a while: it takes over the old node's modifiers (cMoveNode_transferFromOld), counts Life
// frames down in updateNode and then expires; expire / loseFocus replace it by a cDeadNode (Type 0), a cRestoreNode
// (Type 3, expire only) or nothing. The data's Visible word sets the record's visibility flags. Node type 0. Also
// made by checkActiveNode and the replay loader. No EA source path in the preview asserts; retail methods: the ctor in
// the intersect/worldsphtree splat unit (0x00342C08), the rest at the start of object/debouncenode (0x00342D10..
// 0x00342FA8) and the dtor / onCollide in object/railmodifier (0x003614D0, 0x003614F8).
// Size 0x34: [R] cMemMan_alloc(0x34, "Debounce") in cReplay_restoreObject (src/replay/replay.cpp); the last member
// (Type) is at 0x30 (the replay ctor reads 4 bytes there); [O] On Tour 0x30 (Life 0x28, Type 0x2C: its cMoveNode is 4
// bytes smaller); [P] the preview's paired methods have Life/Type at 0x28/0x2C (+ 4 in retail, tools/layout_xlate.py).
// Base: cMoveNode (0x00-0x2C, object/movenode.h: cObjNode/cTypeObjNode/cSortObjNode/cInstanceNode/cMoveNode), written
// out inline with that header's member names; the vptr is at 0xC.
// Constructors: cDebounceNode_cDebounceNode1 (bucket, record, tMakeDebounceData*, cInstanceNode* old:
// cMoveNode_cMoveNode1 type 0, vtable, Life = (int)(Time * frame rate), Type, transferFromOld(old), record flags from
// Visible) and cDebounceNode_cDebounceNode (bucket, cBxReplayFrame*: cMoveNode_cMoveNode, then Life and Type, 4 bytes
// each). Destructor cDebounceNode__cDebounceNode__FPv (vtable, cMoveNode__cMoveNode). Vtable cDebounceNode_vtbl
// (0x004906F0, 51 slots: 1 dtor, 12 writeToReplayFrame, 15 updateNode, 34 expire, 35 loseFocus, 36 signal, 40 onCollide
// (empty: no hits while debouncing); the rest are cMoveNode's / cInstanceNode's).
//
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/intersect/worldsphtree.cpp, src/object/debouncenode.cpp) and the 0x34 allocation;
//   [P] the July 2003 preview via tools/layout_xlate.py: 7 paired methods, 0xC and 0x18 equal, Life/Type + 4;
//   [U] how retail code uses the field (src/object/debouncenode.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (cMoveNode chain, cDebounceNode, tMakeDebounceData).

#include "layout.h"
#include "wscript/wscriptnull.h"
#include "object/movenode.h"

struct cObjNode;
struct tInstanceRecord;
struct tModifierBlock;

// World-script data that makes a debounce node (On Tour tMakeDebounceData, 0x14; same offsets). [O] On Tour's
// cDebounceNode(cBucketList*, tInstanceRecord*, const tMakeDebounceData*, cInstanceNode* pOldNode);
// [U] cDebounceNode_cDebounceNode1 (view sDef_342C08): Time seconds * TimerHz -> Life (+0x2C), Type -> +0x30, and
// Visible (0 / 1 / other) edits the instance record's visibility flags.
struct tMakeDebounceData {
    unsigned int InstId;            // 0x00 [O] tResourceID; [U] MakeDebounceFunc's instance lookup (-1: the script's
                                    //      current instance)
    float Time;                     // 0x04 [O]; [U] * TimerHz -> the node's life (+0x2C)
    int Type;                       // 0x08 [O]; [U] -> the node's Type (+0x30)
    int Visible;                    // 0x0C [O]; [U] 0 clears the record's low four flag bits, 1 sets bit 0 and bit 2
                                    //      (clears bit 1), else bit 1 becomes bit 2 when bits 0 and 1 are set
    WScriptNullType null;           // 0x10 [O]
    char unk_011[0x3];              // 0x11 padding

    // Visible values (names ours): [U] the ctor hides the instance for kVisible_Hide (clears the record's low four
    // flag bits) and shows it for kVisible_Show (sets bits 0 and 2, clears bit 1); any other value keeps it.
    enum { kVisible_Hide = 0, kVisible_Show = 1 };
};

CHECK_OFFSET(tMakeDebounceData, InstId, 0x00);
CHECK_OFFSET(tMakeDebounceData, Time, 0x04);
CHECK_OFFSET(tMakeDebounceData, Type, 0x08);
CHECK_OFFSET(tMakeDebounceData, Visible, 0x0C);
CHECK_OFFSET(tMakeDebounceData, null, 0x10);
CHECK_SIZE(tMakeDebounceData, 0x14);

struct cDebounceNode : public cMoveNode {
    // cMoveNode (object/movenode.h, 0x2C): the vptr at 0x0C:
    //   [R] cDebounceNode_vtbl (0x004906F0) in both ctors and the dtor; [P]; [U] updateNode calls slot 34 (expire),
    //       expire / loseFocus slot 1 (delete)
    // cDebounceNode's notes on cMoveNode's fields:
    //   ObjNodeType: [R] node type 0 (cMoveNode_cMoveNode1)
    //   Instance: [R] ctor1 sets the record's flags (+0x8) from the data's Visible word; [U] expire / loseFocus hand
    //       it to the cDeadNode / cRestoreNode that replaces this node
    //   ModifierBlock: [R] taken over from the old node (cMoveNode_transferFromOld)
    // --- cDebounceNode (On Tour 0x28 / 0x2C, + 4 here)
    int Life;                       // 0x2C [O]; [P] preview 0x28; [R] ctor1 (int)(data Time * frame rate) (cvt.w.s +
                                    //      swc1: an int through the FPU); the replay ctor reads 4 bytes; [U] updateNode
                                    //      counts it down and expires at 0 (lw/sw); expire / loseFocus do nothing when
                                    //      it is negative
    // What expire leaves on the instance (names ours): [U] expire makes a cDeadNode for kExpire_Dead and a cRestoreNode
    // for kExpire_Restore and just deletes the node for any other Type; loseFocus makes a cDeadNode for kExpire_Dead.
    enum { kExpire_Dead = 0, kExpire_Restore = 3 };
    int Type;                       // 0x30 [O]; [P] preview 0x2C; [R] ctor1 = data Type (+0x8); the replay ctor reads 4
                                    //      bytes; [U] expire: 0 -> cDeadNode, 3 -> cRestoreNode, else just delete;
                                    //      loseFocus: 0 -> cDeadNode, else delete

    // Methods bound to their retail symbols by asm labels.
    virtual ~cDebounceNode();   // g++'s own symbol _$_13cDebounceNode (the retail label renamed)
    void* cDebounceNode1(void* bucket, void* instance, tMakeDebounceData* def, void* oldNode)
        __asm__("cDebounceNode_cDebounceNode1");

    // Methods bound to their retail symbols by asm labels.
    // updateNode stays a C function (cDebounceNode_updateNode): declared first, it is the vtable's key function, which
    // keeps the vtable out of the units defining the other overrides
    virtual bool updateNode() __asm__("cDebounceNode_updateNode");
    virtual void loseFocus() __asm__("cDebounceNode_loseFocus");
    virtual void expire(int check) __asm__("cDebounceNode_expire");
    virtual void onCollide(const tCollideInfo*, const tIntersectInfo*, cBeing*) __asm__("cDebounceNode_onCollide__FPv");
    virtual void signal(int, float) __asm__("cDebounceNode_signal__FPv");
    virtual void writeToReplayFrame(cBxReplayFrame* stream) __asm__("cDebounceNode_writeToReplayFrame");
};

CHECK_OFFSET(cDebounceNode, Life, 0x2C);
CHECK_OFFSET(cDebounceNode, Type, 0x30);
CHECK_SIZE(cDebounceNode, 0x34);
__asm__("_vt$13cDebounceNode = cDebounceNode_vtbl");   // g++'s name for the retail vtable

#endif /* OBJECT_DEBOUNCENODE_H */
