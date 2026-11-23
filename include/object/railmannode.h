#ifndef OBJECT_RAILMANNODE_H
#define OBJECT_RAILMANNODE_H

// cRailManNode: the rail manager scene node - one live instance (static ActiveNode = cRailManNode_ActiveNode) keeping up to 128
// switched-off rails: the world script's RailFunc calls addRail(rid, add) (cRailManNode_addRail), which creates the node on
// first use, records the rail's resource id in a free RailInfo slot and takes every particle node of that rail out of
// the world's Nash space hash (disableRail) or puts it back (enableRail; the slot goes back to -1).
// cGame_sectionUnloading calls unloadSection(section) (cRailManNode_unloadSection) to re-enable the rails of a section that goes
// away; the destructor re-enables all. Node type 18. EA's source: d:/ssx/code/source/object/railmannode.cpp (preview
// assert path). Retail methods: the object/multiparticle splat unit (0x003584F0..0x00358C30) and getSectionId in
// object/railmodifier (0x00361D78).
// Size 0x218: [R] cMemMan_alloc(0x218, "RailMan") in cRailManNode_addRail (addRail) and cReplay_restoreObject; the ctors
// fill 128 4-byte slots from 0x18 (writeToReplayFrame / the replay ctor move 0x200 bytes at +0x18); [O] On Tour
// 0x214 (RailInfo[128] at 0x14; its cObjNode is 4 bytes smaller).
// Bases: cSortObjNode (0x00-0x18, object/objnode.h: cObjNode 0x00-0x10, cTypeObjNodeState 0x10, cSortObjNodeState
// 0x14); it is not a cInstanceNode (no record). Vptr at 0xC.
// Constructors: cRailManNode_cRailManNode1 (bucket: cSortObjNode_cSortObjNode1 type 18, vtable, every RailInfo = -1,
// ActiveNode = this) and cRailManNode_cRailManNode (bucket, cBxReplayFrame*: reads RailInfo and disables every used
// slot); destructor cRailManNode__cRailManNode (enableRail on every used slot, ActiveNode = 0). Vtable
// cRailManNode_vtbl (0x0048EDE0, 14 slots: cSortObjNode's with 1 dtor, 11 getSectionId (-1), 12 writeToReplayFrame
// overridden).
//
// Derives from its scene-node base (object/objnode.h chain); its own members FLAT, every field at its retail
// offset. Evidence tags:
//   [R] retail ctors/dtor (src/object/multiparticle.cpp) and the two 0x218 allocations;
//   [P] the July 2003 preview via tools/layout_xlate.py (no accessors observed for this class);
//   [U] how retail code uses the field (enableRail / disableRail / cRailManNode_unloadSection / cRailManNode_addRail and the retail asm);
//   [O] SSX On Tour's DWARF (cObjNode chain, cRailManNode, tRailInfo).

#include "layout.h"
#include "object/objnode.h"

struct cObjNode;

// One switched-off rail (On Tour tRailInfo, 4 bytes; its default ctor sets -1 = a free slot).
struct tRailInfo {
    unsigned int RailRID;           // 0x0 [O] tResourceID (section in the low byte | index << 8); [R] the ctors store
                                    //     0xFFFFFFFF; [U] enableRail stores -1, cRailManNode_addRail stores the key;
                                    //     cRailManNode_unloadSection compares the low byte with the section (lbu)
};

CHECK_OFFSET(tRailInfo, RailRID, 0x0);
CHECK_SIZE(tRailInfo, 0x4);

struct cRailManNode : public cSortObjNode {
    // cSortObjNode (object/objnode.h, 0x18): the vptr at 0x0C:
    //   [R] cRailManNode_vtbl (0x0048EDE0) in both ctors and the dtor
    // cRailManNode's notes on cSortObjNode's fields:
    //   Next: [U] bucket list (bucket 1 from addRail)
    //   ObjNodeType: [R] cRailManNode1 passes type 18
    // --- cRailManNode
    tRailInfo RailInfo[128];        // 0x018 [O] tRailInfo RailInfo[128]; [R] ctors: 128 x 0xFFFFFFFF; the replay ctor
                                    //      reads / writeToReplayFrame writes 0x200 bytes here; [U] enableRail /
                                    //      disableRail look up RailInfo[i] in the world's resource tables, the dtor and
                                    //      cRailManNode_unloadSection scan it (lw 0x18 + 4i, lbu for the section byte)
};

CHECK_OFFSET(cRailManNode, RailInfo, 0x018);
CHECK_SIZE(cRailManNode, 0x218);

#endif /* OBJECT_RAILMANNODE_H */
