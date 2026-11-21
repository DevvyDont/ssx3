#ifndef PATH_BASEPATH_H
#define PATH_BASEPATH_H

// cPath: one path of the path file: a polyline of nodes from Start (each tPathNode: a direction and a length, 0x10),
// the events along it (tPathEventNode: type, args, start / end distance; speed limits, forks, ...) and its bounding box.
// The base of cTrackPath (0x3C, cPathSys::TrackPaths) and cAIPath (0x40, cPathSys::AIPaths): cPathSys_loadTrackPaths /
// _loadAIPaths allocate the arrays ("TrackPaths" / "AIPaths") and run initPath on each element. EA's source: most likely
// d:/ssx/code/source/path/basepath.cpp / basepath.h (preview asserts; the path sources are basepath, aipath, trackpath
// and pathsys, and basepath.h holds the inline event asserts "(speedtype > -1) && (speedtype < 8)"); the methods are
// in the retail units bx/bxstringctor (0x0026A180..0x0026AD70) and path/pathsys (0x0026C3D0..0x0026C420).
// Vtable: cPath's own vt_004814A8 (unnamed in symbol_addrs; 4 slots: getEventsInRange, isForkInPath,
// populateDataWithDefaults = func_0026C410__FPv, populateData = func_0026C418__FPv; no destructor slot); cTrackPath
// D_00481448 and cAIPath D_00481478 override some. No out-of-line ctor: the array new inlines it.
// Size 0x38: [R] the vptr is at 0x34 (initPath calls slots 3 / 4 through lw 0x34; the inlined array ctors store the
// derived vtables at +0x34) and the derived classes' own members start at 0x38 (sPathB310::field_0x38,
// sPathB410::field_0x38 / 0x3C in path/pathsys.cpp: cTrackPath 0x3C, cAIPath 0x40); [O] On Tour's cPath is 0x38 (data
// up to MaxBound 0x28..0x34, then the vptr).
// Base classes: none. cPath declares the first virtuals, so its vptr follows its data (g++ 2.95).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] the inlined array ctors in cPathSys_loadTrackPaths / _loadAIPaths;
//   [P] the preview's getNumEvents__5cPath (lw 0x0) and getAllEvents__5cPath (lw 0x4); tools/layout_xlate.py: preview
//       == retail at 0x0 / 0x4;
//   [U] how retail code uses the field (src/bx/bxstringctor.cpp: views sBXStr_26A180 (initPath, the full layout),
//       sBXPath / sBXPath_A428 / _A638, sBXBox / sBXBox2, sBXRangeList, sTable_0026A920 / _0026A970, raw offsets;
//       src/path/pathsys.cpp);
//   [O] SSX On Tour's DWARF (class cPath: NumEvents, Events, NumNodes, Start, Nodes, MinBound, MaxBound): same offsets.
// t3Vector members are float[3] (ui/uithing.h has EA's t3Vector; not pulled in here).

#include "layout.h"
#include "bx/math3d.h"                // t4Vector (tPathIntersectInfo)

struct tPathEventNode;
struct tPathNode;

struct cPath {
    int NumEvents;                  // 0x000 [O]; [P] getNumEvents (lw 0x0); [U] initPath stores it; findEvent /
                                    //       findEvent1 / getEventsInRange / isForkInPath loop to it
    tPathEventNode* Events;         // 0x004 [O]; [P] getAllEvents (lw 0x4); [U] initPath points it into the path data;
                                    //       findEvent / getEventsInRange / isForkInPath walk it
    int NumNodes;                   // 0x008 [O]; [U] initPath stores it; getLength / getPathPos / getPathHeading /
                                    //       intersectPath / intersectPath1 loop to it
    float Start[3];                 // 0x00C [O] t3Vector; [U] initPath reads it (&Start); getStart / getPathPos /
                                    //       intersectPath / intersectPath1 read x / y / z (lwc1)
    tPathNode* Nodes;               // 0x018 [O]; [U] initPath stores it; getLength sums Nodes[i] length (+0xC);
                                    //       getPathPos / getPathHeading / intersectPath walk it (0x10 stride)
    float MinBound[3];              // 0x01C [O] t3Vector; [U] initPath fills it (&MinBound); getProximity /
                                    //       isPathInBox read it
    float MaxBound[3];              // 0x028 [O] t3Vector; [U] initPath fills it (&MaxBound); getProximity /
                                    //       isPathInBox read it
    void** mVtbl;                   // 0x034 [R] vt_004814A8 (cPath), D_00481448 (cTrackPath), D_00481478 (cAIPath) from
                                    //       the inlined ctors; [U] initPath calls populateDataWithDefaults /
                                    //       populateData through it (lw 0x34)
};

CHECK_OFFSET(cPath, NumEvents, 0x000);
CHECK_OFFSET(cPath, Events, 0x004);
CHECK_OFFSET(cPath, NumNodes, 0x008);
CHECK_OFFSET(cPath, Start, 0x00C);
CHECK_OFFSET(cPath, Nodes, 0x018);
CHECK_OFFSET(cPath, MinBound, 0x01C);
CHECK_OFFSET(cPath, MaxBound, 0x028);
CHECK_OFFSET(cPath, mVtbl, 0x034);
CHECK_SIZE(cPath, 0x38);

// ---- The path records (views group D) ----

// tPathEventNode (On Tour 0x10) is defined in ai/rider.h (cRider::PathEventsCur); the forward declaration above stays.

// tPathNode: one segment of the polyline. [O] On Tour struct tPathNode {t3Vector Dir; float Len;} (0x10);
// [U] cPath_getLength sums Len (+0xC), getPathPos / getPathHeading step along Dir * Len from Start.
struct tPathNode {
    float Dir[3];                   // 0x0 [O] t3Vector; [U] the unit direction of the segment
    float Len;                      // 0xC [O]; [U] getLength
};

CHECK_OFFSET(tPathNode, Dir, 0x0);
CHECK_OFFSET(tPathNode, Len, 0xC);
CHECK_SIZE(tPathNode, 0x10);

// tPathIntersectInfo: where a position meets a path. [O] On Tour struct tPathIntersectInfo {t4Vector Pos; float
// Param; int NodeID;} (0x20, 16-aligned); [U] the path-position cache in bx/bxstringctor (view sBXPathCache_A638): pos,
// the distance along the path, the node index.
struct tPathIntersectInfo {
    t4Vector Pos;                   // 0x00 [O]
    float Param;                    // 0x10 [O]; the distance along the path
    int NodeID;                     // 0x14 [O]; the node (segment) index
};

CHECK_OFFSET(tPathIntersectInfo, Pos, 0x00);
CHECK_OFFSET(tPathIntersectInfo, Param, 0x10);
CHECK_OFFSET(tPathIntersectInfo, NodeID, 0x14);
CHECK_SIZE(tPathIntersectInfo, 0x20);

#endif /* PATH_BASEPATH_H */
