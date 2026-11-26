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

    // The level of the smallest cells (name ours): [U] FrustumCallback_operatorCall visits a cell's children (level
    // - 1) unless the cell is at level 11.
    enum { kLeafLevel = 11 };
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
                                    //       nash_Cell_addnode / remnode link and unlink (nash_NodeList::prepend /
                                    //       remove)
    nash_Node* prev;                // 0x004 [O]; [U] nash_Cell_addnode / remnode (nash_NodeList::prepend / remove)
};

CHECK_OFFSET(nash_Node, next, 0x000);
CHECK_OFFSET(nash_Node, prev, 0x004);
CHECK_SIZE(nash_Node, 0x8);

// A typed object filed in the nash (On Tour class cTypedSpaceNode : Node, 0xC): the link plus its eSpaceNodeType
// (kSNT_SplineSegment 1, kSNT_Rail 2, kSNT_FlexRail 3, kSNT_CameraTrigger 4, kSNT_AudioVolume 5, kSNT_Light 6,
// kSNT_FogVol 7, kSNT_Halo 8). cOverlapList::typed[] (intersect/overlaplist.h) holds these; the concrete records start
// with the same three words (cRailSpaceNode / cFlexRailSpaceNode in object/railspacenode.h, tSplineSegmentRecord's
// m_iIntersect[3] in world/worldrecords.h), so a query casts by type to reach their members (+0xC Rail / FlexNode,
// +0x68 m_Parent).
// cTypedSpaceNode::type. [O] On Tour's enum eSpaceNodeType, names and values; [U] as listed above.
enum eSpaceNodeType {
    kSNT_Error = 0, kSNT_SplineSegment = 1, kSNT_Rail = 2, kSNT_FlexRail = 3, kSNT_CameraTrigger = 4,
    kSNT_AudioVolume = 5, kSNT_Light = 6, kSNT_FogVol = 7, kSNT_Halo = 8, kSNT_Num = 9
};

struct cTypedSpaceNode {
    nash_Node* next;                // 0x000 [O] Node::next; [U] the nash cell walks (as nash_Node)
    nash_Node* prev;                // 0x004 [O] Node::prev
    int type;                       // 0x008 [O] eSpaceNodeType; [U] cOverlapList_splineIntersect, collect_337A48,
                                    //       objs_33ED60, nash_Cell_descend6 switch on it; cPS2ChunkResolve_resolve /
                                    //       unresolve store 1 / 6 / 7 / 8 when filing segments, lights, fog, halos
};

CHECK_OFFSET(cTypedSpaceNode, next, 0x000);
CHECK_OFFSET(cTypedSpaceNode, prev, 0x004);
CHECK_OFFSET(cTypedSpaceNode, type, 0x008);
CHECK_SIZE(cTypedSpaceNode, 0xC);

// nash::NodeList (On Tour class NodeList, 0x4): the head of one per-type list of a Cell.
struct nash_NodeList {
    nash_Node* head;                // 0x000 [O]; [U] nash_Cell_addnode pushes, remnode unlinks

    // [O] NodeList::NodeList(); [R] every Cell allocation (nash_Cell_addnode / movenode, nash_Octant_addnode) clears
    // the three heads in a loop before the children's MEM_fill.
    nash_NodeList() : head(0) {}
    // [O] NodeList::prepend; [R] nash_Cell_addnode / movenode / nash_Octant_addnode link the node in at the head.
    void prepend(nash_Node* n)
    {
        if (head) head->prev = n;
        n->next = head;
        n->prev = 0;
        head = n;
    }
    // [O] NodeList::remove; [R] nash_Cell_remnode / movenode unlink the node, moving the head on when it was first.
    void remove(nash_Node* n)
    {
        if (n->next) n->next->prev = n->prev;
        if (n->prev) {
            n->prev->next = n->next;
        } else {
            head = n->next;
        }
    }
};

CHECK_OFFSET(nash_NodeList, head, 0x000);
CHECK_SIZE(nash_NodeList, 0x4);

// nash::Cell (On Tour class Cell, 0x2C): one octree cell of the Nash: eight child cells (0 = none) and one list per
// node type (On Tour enum NodeType: instance 0, patch 1, typed 2). Allocated per cell: [R] cMemMan_alloc(0x2C, "Cell",
// 0x20000000) in nash_Cell_addnode / nash_Cell_movenode / nash_Octant_addnode (children cleared with MEM_fill 0x20),
// deleted by nash_Cell_remnode / movenode when it becomes empty. No vtable. Methods: nash_Cell_addnode / remnode /
// movenode (intersect/aifwddiff), nash_Cell_descend .. descend6 (intersect/worldsphtree: the whole-subtree walkers of
// the queries); the nash_privy_QueryHelper_helpX* walkers take a Cell as their fifth argument.
struct OverlapEverythingCallback;
struct OverlapInstancesCallback;

struct nash_Cell {
    nash_Cell* children[2][2][2];   // 0x000 [O] Cell* children[2][2][2]; [R] cleared by the allocating code (MEM_fill
                                    //       0x20); [U] descend* recurse into each non-null child (lw 0x0..0x1C);
                                    //       addnode / remnode / movenode index it [dx][dy][dz] from the key bits
    nash_NodeList nodesinside[3];   // 0x020 [O] NodeList nodesinside[3], indexed by NodeType; [U] 0x20 = instances
                                    //       (OverlapInstancesCallback_operatorCall, descend4/5), 0x24 = patches
                                    //       (descend2/3/4), 0x28 = typed nodes (descend1/3/6)

    // [O] Cell::empty; [R] nash_Cell_remnode / movenode delete a child once it has no child cell and no node.
    bool empty()
    {
        if (children[0][0][0] || children[0][0][1] || children[0][1][0] || children[0][1][1] || children[1][0][0] ||
            children[1][0][1] || children[1][1][0] || children[1][1][1]) {
            return false;
        }
        for (int i = 0; i < 3; i++) {
            if (nodesinside[i].head) return false;
        }
        return true;
    }

    // Methods bound to their retail symbols by asm labels.
    void remnode(int idx, nash_Node* item, const nash_CellKey* target, const nash_CellKey* cur)
        __asm__("nash_Cell_remnode");
    void movenode(int type, nash_Node* item, nash_CellKey* oldkey, nash_CellKey* newkey, nash_CellKey* mykey)
        __asm__("nash_Cell_movenode");

    // Methods bound to their retail symbols by asm labels.
    void descend(OverlapInstancesCallback* ctx) __asm__("nash_Cell_descend");
    void descend2(OverlapInstancesCallback* ctx) __asm__("nash_Cell_descend2");
    void descend3(OverlapEverythingCallback* ctx) __asm__("nash_Cell_descend3");
    void descend4(void* ctx) __asm__("nash_Cell_descend4");
    void descend5(void* ctx) __asm__("nash_Cell_descend5");
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

    // Methods bound to their retail symbols by asm labels.
    void addnode(int idx, nash_Node* item, nash_CellKey* cell) __asm__("nash_Octant_addnode");
};

CHECK_OFFSET(nash_Octant, rootkey, 0x000);
CHECK_OFFSET(nash_Octant, rootcell, 0x010);
CHECK_SIZE(nash_Octant, 0x14);

struct cWorldIntersectType;
struct tIntersectInfo;

struct cBXNash {
    // --- nash::Nash (On Tour 0xA4)
    nash_Octant octant[2][2][2];    // 0x000 [O] Octant octant[2][2][2] (x, y, z order unknown: retail walks them as a
                                    //       flat [8]); [R] cGame_load constructs the eight (rootcell = 0); [U] every
                                    //       cBXNash_* query reads octant[i].rootcell (lw 0x10 + 0x14 * i) and passes
                                    //       &octant[i] (addiu 0x14 * i)
    unsigned int deletecount;       // 0x0A0 [O]; [R] cGame_load 0; [U] cOverlapList_get* copy it into the overlap
                                    //       list's deletecount
    // --- cBXNash
    cAIFwdDiffCache* TheAIFwdDiffCache; // 0x0A4 [O]; [R] cGame_load 0; [U] initTheAIFwdDiffCache stores a new
                                    //       cAIFwdDiffCache, purgeTheAIFwdDiffCache deletes and zeroes it;
                                    //       terrainIntersect / getGroundHeight / cOverlapList_terrainIntersect pass it
                                    //       to the patch callbacks (cAIFwdDiffCache_GetFwdDiff)

    // Methods bound to their retail symbols by asm labels.
    void initTheAIFwdDiffCache() __asm__("cBXNash_initTheAIFwdDiffCache");
    void purgeTheAIFwdDiffCache() __asm__("cBXNash_purgeTheAIFwdDiffCache");
    int terrainIntersect(cWorldIntersectType* vq, tIntersectInfo* hits, int cap, int hitCache)
        __asm__("cBXNash_terrainIntersect");
    float terrainIntersect1(cWorldIntersectType* intersectType, tIntersectInfo* out, int hint)
        __asm__("cBXNash_terrainIntersect1");
    float staticObjectIntersect1(cWorldIntersectType* query, tIntersectInfo* out, int full)
        __asm__("cBXNash_staticObjectIntersect1");
};

CHECK_OFFSET(cBXNash, octant, 0x000);
CHECK_OFFSET(cBXNash, deletecount, 0x0A0);
CHECK_OFFSET(cBXNash, TheAIFwdDiffCache, 0x0A4);
CHECK_SIZE(cBXNash, 0xA8);

// The cBXNash query functors (On Tour's DWARF: structs TerrainCallback 0x18, StaticObjectCallback 0x14, SplineCallback
// 0x18, VerticalLineProximityTest 0x4; their operator()s are retail's TerrainCallback_intersectpatch /
// _intersectinstance, StaticObjectCallback_intersectinstance, ...). A query builds one on the stack and hands it to
// nash_privy_QueryHelper_* / nash_Cell_descend*, which call it per object in the overlapped cells. [P] the preview's
// asserts "numintersectinfo < maxintersectinfo" / "inum >= 0 && inum <= maxintersectinfo - numintersectinfo"
// (d:/ssx/code/source/intersect/overlaplist.cpp) name the count members. On Tour's reference members are pointers
// here (one word each); retail bools are words.
struct cWorldIntersectType;     // intersect/worldinttypes.h
struct tIntersectInfo;          // ai/rider.h (EA: intersect/intersectinfo.h)
struct tIntHint;
struct t4Vector;
struct tBoundBox;

struct TerrainCallback {
    cWorldIntersectType* intersecttype; // 0x00 [O] cWorldIntersectType&; [U] passed to the patch / instance tests
                                    //      (views sCtx_335960 q, sCtx_335B90 q, sQuery_369D8 seg)
    tIntersectInfo* intersectinfo;  // 0x04 [O]; [U] the hit array (sCtx_335960 hits)
    int maxintersectinfo;           // 0x08 [O]; [P] assert; [U] the hit cap
    int* numintersectinfo;          // 0x0C [O] int&; [P] assert; [U] the hit count, incremented per hit
    tIntHint* hint;                 // 0x10 [O]; [U] passed through (sCtx_335960 arg)
    cAIFwdDiffCache* fwddiffcache;  // 0x14 [O]; [U] passed to the patch test (sCtx_335960 cache)
};

CHECK_OFFSET(TerrainCallback, intersecttype, 0x00);
CHECK_OFFSET(TerrainCallback, intersectinfo, 0x04);
CHECK_OFFSET(TerrainCallback, maxintersectinfo, 0x08);
CHECK_OFFSET(TerrainCallback, numintersectinfo, 0x0C);
CHECK_OFFSET(TerrainCallback, hint, 0x10);
CHECK_OFFSET(TerrainCallback, fwddiffcache, 0x14);
CHECK_SIZE(TerrainCallback, 0x18);

struct StaticObjectCallback {
    cWorldIntersectType* intersecttype; // 0x00 [O] cWorldIntersectType&; [U] StaticObjectCallback_intersectinstance
                                    //      (view sCtx_336D40 q)
    tIntersectInfo* intersectinfo;  // 0x04 [O]; [U] the hit array (sCtx_336D40 buf)
    int maxintersectinfo;           // 0x08 [O]; [U] cap
    int* numintersectinfo;          // 0x0C [O] int&; [U] count
    int includeridable;             // 0x10 [O] bool (retail word); [U] sCtx_336D40 full
};

CHECK_OFFSET(StaticObjectCallback, intersecttype, 0x00);
CHECK_OFFSET(StaticObjectCallback, intersectinfo, 0x04);
CHECK_OFFSET(StaticObjectCallback, maxintersectinfo, 0x08);
CHECK_OFFSET(StaticObjectCallback, numintersectinfo, 0x0C);
CHECK_OFFSET(StaticObjectCallback, includeridable, 0x10);
CHECK_SIZE(StaticObjectCallback, 0x14);

struct SplineCallback {
    const t4Vector* intersectpos;   // 0x00 [O] const t4Vector&; [U] the query point (views sQuery_33ED60 / sQry_337A48 pos)
    tIntersectInfo* intersectinfo;  // 0x04 [O]; [U] passed through (views: a7)
    unsigned int splinefiltermask;  // 0x08 [O]; [U] tested against the rail's mask (views: mask)
    int* hit;                       // 0x0C [O] bool& (retail word); [U] set on a hit (views: cnt)
    float* intersectdist;           // 0x10 [O] float&; [U] the best distance so far (views: dist)
    const tBoundBox* box;           // 0x14 [O] const tBoundBox&; [U] the overlap box (views: box)
};

CHECK_OFFSET(SplineCallback, intersectpos, 0x00);
CHECK_OFFSET(SplineCallback, intersectinfo, 0x04);
CHECK_OFFSET(SplineCallback, splinefiltermask, 0x08);
CHECK_OFFSET(SplineCallback, hit, 0x0C);
CHECK_OFFSET(SplineCallback, intersectdist, 0x10);
CHECK_OFFSET(SplineCallback, box, 0x14);
CHECK_SIZE(SplineCallback, 0x18);

struct VerticalLineProximityTest {
    const t4Vector* position;       // 0x00 [O] const t4Vector&; [U] the test point (view sCtx_369D8 pos; the flags
                                    //      after it in that view are the caller's locals)
};

CHECK_OFFSET(VerticalLineProximityTest, position, 0x00);
CHECK_SIZE(VerticalLineProximityTest, 0x4);

// nash::privy::QueryHelper<callback, callback, query> (views group D: sQH_75D0). [P] the preview's linkonce instances
// helpX__Q34nash5privyt11QueryHelper3Z..Z..Z9tBoundBox (const callback&, const callback&, const tBoundBox&,
// const nash::Cell*, const nash::CellKey&): a template class with no data whose member helpX walks a cell for a box
// query; [R] retail keeps the instances' code under nash_privy_QueryHelper_helpX* (camera/trigger
// cameratriggerfactory) and calls them with the address of an empty helper as `this` (func_002B83F8 in
// sound/icepick/worldtriggermanager). Flat stand-in for every instance: an empty struct (size 1 in g++).
struct nash_privy_QueryHelper {

    // Methods bound to their retail symbols by asm labels.
    void help(TerrainCallback* query, TerrainCallback* insideQuery, VerticalLineProximityTest* test, nash_Cell* node,
              nash_CellKey* cell)
        __asm__("nash_privy_QueryHelper_help");
};

CHECK_SIZE(nash_privy_QueryHelper, 0x1);

#endif /* INTERSECT_BXNASH_H */
