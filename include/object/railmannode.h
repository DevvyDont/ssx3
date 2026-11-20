#ifndef OBJECT_RAILMANNODE_H
#define OBJECT_RAILMANNODE_H

// cRailManNode: the rail manager scene node - one live instance (static ActiveNode = D_004A4040) keeping up to 128
// switched-off rails: the world script's RailFunc calls addRail(rid, add) (func_00358B28), which creates the node on
// first use, records the rail's resource id in a free RailInfo slot and takes every particle node of that rail out of
// the world's Nash space hash (disableRail) or puts it back (enableRail; the slot goes back to -1).
// cGame_sectionUnloading calls unloadSection(section) (func_00358700) to re-enable the rails of a section that goes
// away; the destructor re-enables all. Node type 18. EA's source: d:/ssx/code/source/object/railmannode.cpp (preview
// assert path). Retail methods: the object/multiparticle splat unit (0x003584F0..0x00358C30) and getSectionId in
// object/railmodifier (0x00361D78).
// Size 0x218: [R] cMemMan_alloc(0x218, "RailMan") in func_00358B28 (addRail) and cReplay_restoreObject; the ctors
// fill 128 4-byte slots from 0x18 (writeToReplayFrame / the replay ctor move 0x200 bytes at +0x18); [O] On Tour
// 0x214 (RailInfo[128] at 0x14; its cObjNode is 4 bytes smaller).
// Bases: cSortObjNode (0x00-0x18, object/objnode.h: cObjNode 0x00-0x10, cTypeObjNodeState 0x10, cSortObjNodeState
// 0x14), written out inline; it is not a cInstanceNode (no record). Vptr at 0xC.
// Constructors: cRailManNode_cRailManNode1 (bucket: cSortObjNode_cSortObjNode1 type 18, vtable, every RailInfo = -1,
// ActiveNode = this) and cRailManNode_cRailManNode (bucket, cBxReplayFrame*: reads RailInfo and disables every used
// slot); destructor cRailManNode__cRailManNode (enableRail on every used slot, ActiveNode = 0). Vtable
// cRailManNode_vtbl (0x0048EDE0, 14 slots: cSortObjNode's with 1 dtor, 11 getSectionId (-1), 12 writeToReplayFrame
// overridden).
//
// A FLAT layout like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] retail ctors/dtor (src/object/multiparticle.cpp) and the two 0x218 allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py (no accessors observed for this class);
//   [U] how retail code uses the field (enableRail / disableRail / func_00358700 / func_00358B28 and the retail asm);
//   [O] SSX On Tour's DWARF (cObjNode chain, cRailManNode, tRailInfo).

#include "layout.h"

struct cObjNode;

// One switched-off rail (On Tour tRailInfo, 4 bytes; its default ctor sets -1 = a free slot).
struct tRailInfo {
    unsigned int RailRID;           // 0x0 [O] tResourceID (section in the low byte | index << 8); [R] the ctors store
                                    //     0xFFFFFFFF; [U] enableRail stores -1, func_00358B28 stores the key;
                                    //     func_00358700 compares the low byte with the section (lbu)
};

CHECK_OFFSET(tRailInfo, RailRID, 0x0);
CHECK_SIZE(tRailInfo, 0x4);

struct cRailManNode {
    // --- cObjNode (0x10; object/objnode.h)
    int unk_00;                     // 0x000 cObjNode's leading word; no ctor writes it (see object/objnode.h)
    cObjNode* Next;                 // 0x004 [O] cObjNode::Next; [U] bucket list (bucket 1 from addRail)
    cObjNode* Prev;                 // 0x008 [O] cObjNode::Prev; [U] bucket list
    void** mVtbl;                   // 0x00C [R] cRailManNode_vtbl (0x0048EDE0) in both ctors and the dtor
    // --- cTypeObjNode / cSortObjNode (object/objnode.h)
    short ObjNodeType;              // 0x010 [O] cTypeObjNodeState; [R] cRailManNode1 passes type 18
    unsigned short Flags;           // 0x012 [O] cTypeObjNodeState
    unsigned int SortIndex;         // 0x014 [O] cSortObjNodeState; [P] getSortIndex (lw 0x14)
    // --- cRailManNode
    tRailInfo RailInfo[128];        // 0x018 [O] tRailInfo RailInfo[128]; [R] ctors: 128 x 0xFFFFFFFF; the replay ctor
                                    //      reads / writeToReplayFrame writes 0x200 bytes here; [U] enableRail /
                                    //      disableRail look up RailInfo[i] in the world's resource tables, the dtor and
                                    //      func_00358700 scan it (lw 0x18 + 4i, lbu for the section byte)
};

CHECK_OFFSET(cRailManNode, unk_00, 0x000);
CHECK_OFFSET(cRailManNode, Next, 0x004);
CHECK_OFFSET(cRailManNode, Prev, 0x008);
CHECK_OFFSET(cRailManNode, mVtbl, 0x00C);
CHECK_OFFSET(cRailManNode, ObjNodeType, 0x010);
CHECK_OFFSET(cRailManNode, Flags, 0x012);
CHECK_OFFSET(cRailManNode, SortIndex, 0x014);
CHECK_OFFSET(cRailManNode, RailInfo, 0x018);
CHECK_SIZE(cRailManNode, 0x218);

#endif /* OBJECT_RAILMANNODE_H */
