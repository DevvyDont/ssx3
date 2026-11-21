#ifndef INTERSECT_BXNASH_H
#define INTERSECT_BXNASH_H

// cBXNash: the game's world collision grid ("Nash"): a nash::Nash loose octree (eight top-level octants, each a root
// cell key + root cell) that the world's instances, patches, typed space nodes and splines are filed in, plus the AI's
// patch forward-difference cache. The game owns exactly one (cGame::BXNash, main/game.h); the rider, AI, camera and
// light code query it through cBXNash_terrainIntersect / _staticObjectIntersect / _splineIntersect / _getGroundHeight
// or through a cOverlapList bound to it (intersect/overlaplist.h). EA's source: d:/ssx/code/source/intersect/bxnash.h
// (preview asserts; the base is intersect/nash.h, nash.cpp). Retail methods: the intersect/worldsphtree splat unit
// (0x334800..0x3388E0, interleaved with static helpers); the base's are nash_Nash_* / nash_Octant_* / nash_Cell_* (intersect/aifwddiff).
// No vtable (no virtuals in either class).
// [Staged by group J: adds nash_Node, nash_NodeList and nash_Cell (On Tour Node / NodeList / Cell) before nash_Octant
// and drops the nash_Cell forward declaration; everything else is unchanged from ssx3/include/intersect/bxnash.h.]
// Size 0xA8: [R] cGame_load allocates it with cMemMan_alloc(168, "Nash") and constructs it inline (eight octant root
// cells, deletecount and TheAIFwdDiffCache zeroed); [O] On Tour's cBXNash is 0xA8 (Nash 0xA4 + the cache pointer).
// Base: nash::Nash (On Tour class Nash, 0xA4: Octant octant[2][2][2], deletecount), written out inline.
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance yet. Evidence tags:
//   [R] retail allocation and inline constructor in cGame_load (main/gameload.cpp: view sGridGL / sCellGL);
//   [U] how retail code uses the field (src/intersect/worldsphtree.cpp: the cBXNash_* methods and the cOverlapList_*
//       queries through the overlap list's nash pointer; the camera trigger walkers in cameratriggerman.cpp);
//   [O] SSX On Tour's DWARF (class Nash 0xA4, class Octant 0x14, struct CellKey 0x10, class cBXNash 0xA8): same
//       offsets as retail.

#include "layout.h"

struct cAIFwdDiffCache;

// nash::CellKey (On Tour struct CellKey, 0x10): an octree cell's level and integer grid coordinates.
// nash_CellKey_proximitytest (intersect/aifwddiff) takes one as its this.
struct nash_CellKey {
    int level;                      // 0x000 [O]
    int x;                          // 0x004 [O]
    int y;                          // 0x008 [O]
    int z;                          // 0x00C [O]
};

CHECK_OFFSET(nash_CellKey, level, 0x000);
CHECK_OFFSET(nash_CellKey, x, 0x004);
CHECK_OFFSET(nash_CellKey, y, 0x008);
CHECK_OFFSET(nash_CellKey, z, 0x00C);
CHECK_SIZE(nash_CellKey, 0x10);

// nash::Node (On Tour class Node, 0x8): the intrusive list link a filed object starts with (an instance record, a
// patch, a typed space node such as cRailSpaceNode). The Cell's lists hold these.
struct nash_Node {
    nash_Node* next;                // 0x000 [O]; [U] nash_Cell_descend1/4/5 walk the lists through it (lw 0x0);
                                    //       nash_Cell_addnode / remnode link and unlink (sItem_003284B8::next)
    nash_Node* prev;                // 0x004 [O]; [U] nash_Cell_addnode / remnode (sItem_003284B8::prev)
};

CHECK_OFFSET(nash_Node, next, 0x000);
CHECK_OFFSET(nash_Node, prev, 0x004);
CHECK_SIZE(nash_Node, 0x8);

// nash::NodeList (On Tour class NodeList, 0x4): the head of one per-type list of a Cell.
struct nash_NodeList {
    nash_Node* head;                // 0x000 [O]; [U] nash_Cell_addnode pushes, remnode unlinks (sList_003284B8::head)
};

CHECK_OFFSET(nash_NodeList, head, 0x000);
CHECK_SIZE(nash_NodeList, 0x4);

// nash::Cell (On Tour class Cell, 0x2C): one octree cell of the Nash: eight child cells (0 = none) and one list per
// node type (On Tour enum NodeType: instance 0, patch 1, typed 2). Allocated per cell: [R] cMemMan_alloc(0x2C, "Cell",
// 0x20000000) in nash_Cell_addnode / nash_Cell_movenode / nash_Octant_addnode (children cleared with MEM_fill 0x20),
// deleted by nash_Cell_remnode / movenode when it becomes empty. No vtable. Methods: nash_Cell_addnode / remnode /
// movenode (intersect/aifwddiff), nash_Cell_descend .. descend6 (intersect/worldsphtree: the whole-subtree walkers of
// the queries); the nash_privy_QueryHelper_helpX* walkers take a Cell as their fifth argument.
struct nash_Cell {
    nash_Cell* children[2][2][2];   // 0x000 [O] Cell* children[2][2][2]; [R] cleared by the allocating code (MEM_fill
                                    //       0x20); [U] descend* recurse into each non-null child (lw 0x0..0x1C);
                                    //       addnode / remnode / movenode index it [dx][dy][dz] from the key bits
    nash_NodeList nodesinside[3];   // 0x020 [O] NodeList nodesinside[3], indexed by NodeType; [U] 0x20 = instances
                                    //       (OverlapInstancesCallback_operatorCall, descend4/5), 0x24 = patches
                                    //       (descend2/3/4), 0x28 = typed nodes (descend1/3/6)
};

CHECK_OFFSET(nash_Cell, children, 0x000);
CHECK_OFFSET(nash_Cell, nodesinside, 0x020);
CHECK_SIZE(nash_Cell, 0x2C);

// nash::Octant (On Tour class Octant, 0x14): one top-level octant of the Nash: the key of its root cell and the root
// cell itself (0 while the octant is empty).
struct nash_Octant {
    nash_CellKey rootkey;           // 0x000 [O]; [U] the cBXNash_* / cOverlapList_* queries pass &octant[i].rootkey to
                                    //       nash_CellKey_proximitytest and the helpX walkers
    nash_Cell* rootcell;            // 0x010 [O]; [R] cGame_load zeroes it (sCellGL ctor); [U] the queries skip octants
                                    //       whose rootcell is 0
};

CHECK_OFFSET(nash_Octant, rootkey, 0x000);
CHECK_OFFSET(nash_Octant, rootcell, 0x010);
CHECK_SIZE(nash_Octant, 0x14);

struct cBXNash {
    // --- nash::Nash (On Tour 0xA4)
    nash_Octant octant[2][2][2];    // 0x000 [O] Octant octant[2][2][2] (x, y, z order unknown: retail walks them as a
                                    //       flat [8]); [R] cGame_load constructs the eight (rootcell = 0); [U] every
                                    //       cBXNash_* query reads octant[i].rootcell (lw 0x10 + 0x14 * i) and passes
                                    //       &octant[i] (addiu 0x14 * i)
    unsigned int deletecount;       // 0x0A0 [O]; [R] cGame_load 0; [U] cOverlapList_get* copy it into the overlap
                                    //       list's deletecount (sGrid_330BA8::unkA0)
    // --- cBXNash
    cAIFwdDiffCache* TheAIFwdDiffCache; // 0x0A4 [O]; [R] cGame_load 0; [U] initTheAIFwdDiffCache stores a new
                                    //       cAIFwdDiffCache, purgeTheAIFwdDiffCache deletes and zeroes it;
                                    //       terrainIntersect / getGroundHeight / cOverlapList_terrainIntersect pass it
                                    //       to the patch callbacks (cAIFwdDiffCache_GetFwdDiff)
};

CHECK_OFFSET(cBXNash, octant, 0x000);
CHECK_OFFSET(cBXNash, deletecount, 0x0A0);
CHECK_OFFSET(cBXNash, TheAIFwdDiffCache, 0x0A4);
CHECK_SIZE(cBXNash, 0xA8);

#endif /* INTERSECT_BXNASH_H */
