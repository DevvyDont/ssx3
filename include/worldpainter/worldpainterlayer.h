#ifndef WORLDPAINTER_WORLDPAINTERLAYER_H
#define WORLDPAINTER_WORLDPAINTERLAYER_H

// The streamed world-painter data of one world section: a tWPHeader followed by m_iNumLayers layer slots
// (tWPLayerPtr), each pointing at a cWorldPainterLayer = a quad tree (worldpainter/quadtree.h) plus a regions table.
// The file stores offsets; cWorldPainterSection::resolve (func_002BB5F0) turns each slot into a pointer
// (relative to the header) and calls cWorldPainterLayer::resolve (func_002BAF08), which does the same for the
// layer's tree (relative to the layer, then cQuadTree::resolve) and for every region's data pointer
// (-1 -> 0). A layer is selected by tWorldPainterRegionType::m_layer; a quad-tree leaf's key is a region index;
// the region's m_iRegionType is compared with tWorldPainterRegionType::m_id and its data (a tWPData_*,
// worldpainter/worldpainterquery.h) is blended into the query's tWPInGameData (cWorldPainterQuery::queryPosition).
//
// Sizes [R]/[O]: tWPHeader 0x8 (layer slots start at header + 8: func_002BB5F0, func_002C0778), cWorldPainterLayer
// 0xC (On Tour: cWorldPainterLayer and tWPLayer both 0xC), tWPRegion 0x8 (8-byte stride in func_002BAF08 and
// func_002C0A10), tWPLayerPtr 0x4. No vtables, no bases. EA's source: d:/ssx/code/source/worldpainter/
// worldpainterlayer.h / worldpainterlayer.cpp (preview asserts "m_iQuadTreeOffset >= 0", "m_iRegionOffset >= 0",
// "this->getQuadTree()", "iRegionIndex < getNumRegions()"). cWorldPainterLayer methods: see methods.txt.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail's resolve functions func_002BAF08 (cWorldPainterLayer::resolve) and func_002BB5F0
//       (cWorldPainterSection::resolve);
//   [P] the July 2003 preview's assert strings (worldpainterlayer.h/.cpp, worldpainterman.cpp);
//   [U] how retail code uses the field (getQuadTreeContents func_002BAF90, lookupRegion func_002C0A10,
//       queryPosition func_002C0778, cWorldPainterMan::render func_002BB6E0);
//   [O] SSX On Tour's DWARF (tWPHeader, tWPLayer, tWPLayerPtr, tWPRegion): same members and offsets; its
//       unions are anonymous, so the union member names below that are not in an assert are ours.

#include "layout.h"

struct cQuadTree;
struct cWorldPainterLayer;
struct tWPData;

// One painted region: its type id and its data (On Tour: tWPRegion, 8 bytes).
struct tWPRegion {
    int m_iRegionType;              // 0x0 [O]; [U] queryPosition compares it with the query's m_regionType.m_id
    union {
        int m_iDataOffset;          // 0x4 [O] anonymous union; name ours: the file's offset (-1 = none), relative to
                                    //       the layer
        const tWPData* m_pData;     // 0x4 name ours; [R] resolve stores layer + offset (or 0); [U] queryPosition passes
                                    //       it to the tWPInGameData's blend/isEqual
    };
};

CHECK_OFFSET(tWPRegion, m_iRegionType, 0x0);
CHECK_OFFSET(tWPRegion, m_iDataOffset, 0x4);
CHECK_OFFSET(tWPRegion, m_pData, 0x4);
CHECK_SIZE(tWPRegion, 0x8);

// One layer (On Tour: tWPLayer's data; cWorldPainterLayer is the same 0xC object with the methods).
struct cWorldPainterLayer {
    union {
        int m_iQuadTreeOffset;      // 0x0 [P] "m_iQuadTreeOffset >= 0"; [O] anonymous union; offset from this
        cQuadTree* m_pQuadTree;     // 0x0 name ours ([P] getQuadTree()); [R] resolve stores this + offset, then
                                    //       resolves the tree; [U] getQuadTreeContents, cWorldPainterMan::render
    };
    int m_iNumRegions;              // 0x4 [O]; [P] getNumRegions() ("iRegionIndex < getNumRegions()"); [R] resolve's loop count
    union {
        int m_iRegionOffset;        // 0x8 [P] "m_iRegionOffset >= 0"; [O] anonymous union; offset from this
        tWPRegion* m_pRegions;      // 0x8 name ours ([O] getRegions()); [R] resolve stores this + offset;
                                    //       [U] lookupRegion returns &m_pRegions[getQuadTreeContents(x, y)]
    };
};

CHECK_OFFSET(cWorldPainterLayer, m_iQuadTreeOffset, 0x0);
CHECK_OFFSET(cWorldPainterLayer, m_pQuadTree, 0x0);
CHECK_OFFSET(cWorldPainterLayer, m_iNumRegions, 0x4);
CHECK_OFFSET(cWorldPainterLayer, m_iRegionOffset, 0x8);
CHECK_OFFSET(cWorldPainterLayer, m_pRegions, 0x8);
CHECK_SIZE(cWorldPainterLayer, 0xC);

// One layer slot after the header (On Tour: union tWPLayerPtr; its m_pLayer is a tWPLayer*, the same object).
union tWPLayerPtr {
    cWorldPainterLayer* m_pLayer;   // 0x0 [O]; [R] cWorldPainterSection::resolve stores header + offset (or 0)
    int m_iOffset;                  // 0x0 [O]; the file's offset from the header (-1 = no layer)
};

CHECK_OFFSET(tWPLayerPtr, m_pLayer, 0x0);
CHECK_OFFSET(tWPLayerPtr, m_iOffset, 0x0);
CHECK_SIZE(tWPLayerPtr, 0x4);

// The section's world-painter file header; m_iNumLayers tWPLayerPtr follow it (On Tour: getWPLayers() = this + 1).
struct tWPHeader {
    int m_iVersion;                 // 0x0 [O]; [R] cWorldPainterSection::resolve drops the data unless it is 0x10
    int m_iNumLayers;               // 0x4 [O]; [P] getNumWPLayers() ("getNumWPLayers() == kWPLT_NumLayers");
                                    //       [R] resolve's loop count; [U] queryPosition and render bound the layer index
};

CHECK_OFFSET(tWPHeader, m_iVersion, 0x0);
CHECK_OFFSET(tWPHeader, m_iNumLayers, 0x4);
CHECK_SIZE(tWPHeader, 0x8);

#endif /* WORLDPAINTER_WORLDPAINTERLAYER_H */
