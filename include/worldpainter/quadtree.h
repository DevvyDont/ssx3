#ifndef WORLDPAINTER_QUADTREE_H
#define WORLDPAINTER_QUADTREE_H

// cQuadTree: the world painter's 2D quad tree over the mountain's x/y plane. Each world-painter layer
// (cWorldPainterLayer, worldpainter/worldpainterlayer.h) owns one; a leaf's key is the region index the layer's
// regions table is looked up with (cWorldPainterLayer::getQuadTreeContents -> cQuadTree::getLeaf -> m_key).
// Positions are mapped to 15-bit grid coordinates: ux = (x - m_offsetX) * m_scale, uy = (y - m_offsetY) * m_scale,
// both < 0x8000 (m_scale = 32768 / areaWidth). Nodes live in one array (m_nodes, 8 bytes each, addressed by
// 16-bit index); free nodes are chained through their first short, and the array grows by 1024 nodes
// ("QuadTreeNodeList"). In the streamed world-painter data (tWPHeader) a tree is stored as the 0x28-byte object
// followed by its nodes; cQuadTree::resolve points m_nodes/m_freeNodes at this + 1.
//
// Size 0x28 [R]: cQuadTree_exportTree writes the object as a 0x28-byte header (*size = m_numUsedNodes * 8 + 0x28)
// and copies it with five 8-byte moves; resolve's node array starts at this + 0x28; On Tour: size 0x28, same ten
// members. No vtable, no base class. EA's source: d:/ssx/code/source/worldpainter/quadtree.cpp (preview assert
// "numexportednodes == m_numUsedNodes"; allocation tags "QuadTreeNodeList", "QuadTreeRemapTable",
// "QuadTreeOutputBuffer").
//
// Methods (EA quadtree.cpp = retail 0x002C16A8-0x002C2200; the first five sit in the worldpaintermanquery split;
// names from On Tour's cQuadTree / tQuadTreeNode methods): see methods.txt.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail's constructor cQuadTree_cQuadTree (cQuadTree(minx, miny, areaWidth, defaultKey, defaultData)),
//       cQuadTree_exportTree, cQuadTree_resolve (resolve);
//   [P] the July 2003 preview's assert string (quadtree.cpp);
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (cQuadTree, tQuadTreeNode, tQuadTreeLeafNode, tQuadTreeSubNode, tQuadTreeFreeNode,
//       tQuadTreeNodeFlags): same members, same offsets (no bools, no vectors).

#include "layout.h"

// A leaf: the data pointer and its key (On Tour: tQuadTreeLeafNode, 8 bytes). m_data is word-aligned, so bit 0 of
// the node's first short is 0 for a leaf.
struct tQuadTreeLeafNode {
    void* m_data;                   // 0x0 [O]; [R] setLeafNode (tQuadTreeNode_setLeafNode) stores data here, clear (tQuadTreeNode_init) 0
    unsigned int m_key;             // 0x4 [O]; [R] setLeafNode stores key; [U] getQuadTreeContents (func_002BAF90) returns it
};

CHECK_OFFSET(tQuadTreeLeafNode, m_data, 0x0);
CHECK_OFFSET(tQuadTreeLeafNode, m_key, 0x4);
CHECK_SIZE(tQuadTreeLeafNode, 0x8);

// An inner node: four child indices, each stored as (index << 1) | 1 in the first one (the sub-node flag) and
// (index << 1) | (old bit 0) in the others (setSubNodes, tQuadTreeNode_setSubNodes). Child order: (ux bit, uy bit) =
// 00, 01, 10, 11 (getLeaf, cQuadTree_getLeaf).
struct tQuadTreeSubNode {
    unsigned short m_children[4];   // 0x0 [O]; [R] tQuadTreeNode_setSubNodes (setSubNodes); [U] getLeaf, exportNode, calcUsedNodes
};

CHECK_OFFSET(tQuadTreeSubNode, m_children, 0x0);
CHECK_SIZE(tQuadTreeSubNode, 0x8);

// A free node: the next free index (0 ends the list) in the first short.
struct tQuadTreeFreeNode {
    unsigned short m_next;          // 0x0 [O]; [R] getFreeNode chains new nodes through it; setFreeNodes (cQuadTree_setFreeNodes)
    unsigned short pad[3];          // 0x2 [O]
};

CHECK_OFFSET(tQuadTreeFreeNode, m_next, 0x0);
CHECK_OFFSET(tQuadTreeFreeNode, pad, 0x2);
CHECK_SIZE(tQuadTreeFreeNode, 0x8);

// The node flags view: bit 0 of m_flags[0] set = sub node (isSubNode).
struct tQuadTreeNodeFlags {
    unsigned short m_flags[4];      // 0x0 [O]; [U] every walker tests m_flags[0] & 1
};

CHECK_OFFSET(tQuadTreeNodeFlags, m_flags, 0x0);
CHECK_SIZE(tQuadTreeNodeFlags, 0x8);

// One node of the array (On Tour: tQuadTreeNode, 8 bytes, a single anonymous union of the four views above).
// The union's member names are ours (On Tour's union is anonymous; its accessors are getLeaf/getSubNode/getNext).
struct tQuadTreeNode {
    union {
        tQuadTreeLeafNode m_leaf;   // 0x0 [O] type; name ours
        tQuadTreeSubNode m_sub;     // 0x0 [O] type; name ours
        tQuadTreeFreeNode m_free;   // 0x0 [O] type; name ours
        tQuadTreeNodeFlags m_bits;  // 0x0 [O] type; name ours
    };
    // a free node keeps the next free node's index in its first short (cQuadTree_getFreeNode)
    void setNext(unsigned short next) { m_free.m_next = next; }
    unsigned short getNext() const { return m_free.m_next; }
};

CHECK_OFFSET(tQuadTreeNode, m_leaf, 0x0);
CHECK_OFFSET(tQuadTreeNode, m_sub, 0x0);
CHECK_OFFSET(tQuadTreeNode, m_free, 0x0);
CHECK_OFFSET(tQuadTreeNode, m_bits, 0x0);
CHECK_SIZE(tQuadTreeNode, 0x8);

struct cQuadTree {
    float m_scale;                  // 0x00 [O]; [R] ctor = 32768 / areaWidth; [U] convertX/convertY multiply by it
    float m_offsetX;                // 0x04 [O]; [R] ctor = minx; [U] setData/getLeaf subtract it from x
    float m_offsetY;                // 0x08 [O]; [R] ctor = miny; [U] subtract it from y
    int m_numUsedNodes;             // 0x0C [O]; [P] "numexportednodes == m_numUsedNodes"; [R] ctor 0, getFreeNode ++,
                                    //       setFreeNodes -= 4; exportTree sizes its buffer by it
    int m_numNodes;                 // 0x10 [O]; [R] ctor 0, getFreeNode += 1024; exportTree's remap table has this many
                                    //       entries and the exported copy stores 0
    unsigned short m_root;          // 0x14 [O]; [R] ctor = getFreeNode(); [U] setData/getLeaf start at m_nodes[m_root]
    unsigned short m_pad;           // 0x16 [O]; [R] ctor stores 0
    tQuadTreeLeafNode m_defaultNode; // 0x18 [O]; [R] ctor stores defaultData (0x18) and defaultKey (0x1C); getLeaf
                                    //       returns &m_defaultNode outside the grid; exportTree fixes up both words
    tQuadTreeNode* m_nodes;         // 0x20 [O]; [R] ctor 0, getFreeNode reallocates it ("QuadTreeNodeList"),
                                    //       resolve = this + 1
    tQuadTreeNode* m_freeNodes;     // 0x24 [O]; [R] ctor 0, getFreeNode pops it, resolve = this + 1; exported copy 0
};

CHECK_OFFSET(cQuadTree, m_scale, 0x00);
CHECK_OFFSET(cQuadTree, m_offsetX, 0x04);
CHECK_OFFSET(cQuadTree, m_offsetY, 0x08);
CHECK_OFFSET(cQuadTree, m_numUsedNodes, 0x0C);
CHECK_OFFSET(cQuadTree, m_numNodes, 0x10);
CHECK_OFFSET(cQuadTree, m_root, 0x14);
CHECK_OFFSET(cQuadTree, m_pad, 0x16);
CHECK_OFFSET(cQuadTree, m_defaultNode, 0x18);
CHECK_OFFSET(cQuadTree, m_nodes, 0x20);
CHECK_OFFSET(cQuadTree, m_freeNodes, 0x24);
CHECK_SIZE(cQuadTree, 0x28);

#endif /* WORLDPAINTER_QUADTREE_H */
