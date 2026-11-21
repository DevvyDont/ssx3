#ifndef OBJECT_RAILSPACENODE_H
#define OBJECT_RAILSPACENODE_H

// cRailSpaceNode: the Nash (intersect/nash.cpp) space-partition node that puts one cRailModifier's bound box into the
// world octree, so rail queries (worldsphtree's spline callback -> cRailModifier splineIntersect cRailModifier_splineIntersect) find it.
// Size 0x10: retail cMemMan_alloc(0x10, "RailNode", ...) in cRailModifier_buildXform; On Tour 0x10.
// Bases (On Tour DWARF): nash Node {next 0x0, prev 0x4} (0x8) -> cTypedSpaceNode {type 0x8, eSpaceNodeType} (0xC) ->
// cRailSpaceNode {Rail 0xC}. No vptr. The constructor is inline in buildXform: type = 2 (kSNT_Rail), Rail = the modifier.
// cFlexRailSpaceNode ("FlexRailSpaceNode", type 3, FlexNode at 0xC) has the same shape. EA's header is unknown.
// Users: cRailModifier_buildXform (alloc + init), cRailModifier_update (nash_Nash_movenode), the destructor cRailModifier__cRailModifier
// (remove + operator delete); the octree code links next/prev.

#include "layout.h"

struct cRailModifier;

struct cRailSpaceNode {
    cRailSpaceNode* next;           // 0x00 [O] nash Node::next (Nash cell list link)
    cRailSpaceNode* prev;           // 0x04 [O] nash Node::prev
    int type;                       // 0x08 [O] cTypedSpaceNode::type (eSpaceNodeType); [R] buildXform stores 2 =
                                    //      kSNT_Rail
    cRailModifier* Rail;            // 0x0C [O]; [R] buildXform stores the owning cRailModifier
};

CHECK_OFFSET(cRailSpaceNode, next, 0x00);
CHECK_OFFSET(cRailSpaceNode, prev, 0x04);
CHECK_OFFSET(cRailSpaceNode, type, 0x08);
CHECK_OFFSET(cRailSpaceNode, Rail, 0x0C);
CHECK_SIZE(cRailSpaceNode, 0x10);

// cFlexRailSpaceNode: the same node for a flex rail (cFlexRailNode): [O] On Tour cFlexRailSpaceNode : cTypedSpaceNode
// {FlexNode at 0xC}, size 0x10; [R] cFlexRailNode_addSpaceHash: cMemMan_alloc(0x10, "FlexRailSpaceNode"), type 3;
// [U] view sHashNode_8B90 (object/flexrailnode).
struct cFlexRailNode;

struct cFlexRailSpaceNode {
    cFlexRailSpaceNode* next;       // 0x00 [O] nash Node::next
    cFlexRailSpaceNode* prev;       // 0x04 [O] nash Node::prev
    int type;                       // 0x08 [O] cTypedSpaceNode::type; [R] addSpaceHash stores 3
    cFlexRailNode* FlexNode;        // 0x0C [O]; [R] addSpaceHash stores the owning node
};

CHECK_OFFSET(cFlexRailSpaceNode, next, 0x00);
CHECK_OFFSET(cFlexRailSpaceNode, prev, 0x04);
CHECK_OFFSET(cFlexRailSpaceNode, type, 0x08);
CHECK_OFFSET(cFlexRailSpaceNode, FlexNode, 0x0C);
CHECK_SIZE(cFlexRailSpaceNode, 0x10);

#endif /* OBJECT_RAILSPACENODE_H */
