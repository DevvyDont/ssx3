#ifndef PATH_TRACKPATH_H
#define PATH_TRACKPATH_H

// cTrackPath: one track path of the path file (the race line a rider follows) - a cPath (path/basepath.h) plus the
// distance left from its start to the finish (BaseDistanceLeft): getEventsInRange / isForkInPath take distances left
// and turn them into path distances (BaseDistanceLeft - d) before calling cPath's. cPathSys::TrackPaths (path/pathsys.h) is
// the array cPathSys_loadTrackPaths allocates ("TrackPaths") and builds with the inlined array ctor (vtable cTrackPath_vtbl
// at +0x34), then cPath_initPath fills each one (populateDataWithDefaults / populateData: slots 3 / 4).
// cRider::TrackPath (ai/rider.h) points to one; the preview's cRider::getTrackPath returns it.
// EA's source: d:/ssx/code/source/path/trackpath.cpp (preview assert path). Retail code: unit bx/bxstringctor
// (0x0026A068..0x0026A138) and the out-of-line inline getter in path/pathsys (0x0026C430).
//
// Size 0x3C: [R] cPathSys_loadTrackPaths steps the array 0x3C (its element view sPathB310 in src/path/pathsys.cpp: vptr
// 0x34, word 0x38); [O] On Tour's cTrackPath is 0x3C (BaseDistanceLeft 0x38).
// Base: cPath (0x00-0x38, path/basepath.h, copied inline with its names; cPath's vptr at 0x34 is the only one).
// Vtable cTrackPath_vtbl (cTrackPath's; preview _vt$10cTrackPath; 4 slots: 1 cTrackPath_getEventsInRange,
// 2 cTrackPath_isForkInPath, 3 cTrackPath_populateDataWithDefaults, 4 cTrackPath_populateData).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] the inlined array ctor in cPathSys_loadTrackPaths;
//   [P] the preview's inline getter getBaseDistanceLeft__C10cTrackPath (lwc1 0x38; tools/layout_xlate.py: "seen",
//       preview == retail); cPath's getNumEvents / getAllEvents;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cTrackPath; class cPath): names, types, order and offsets.
// t3Vector members are float[3], as in path/basepath.h.

#include "layout.h"

struct tPathEventNode;
struct tPathNode;

struct cTrackPath {
    // --- cPath (path/basepath.h, 0x38; its vptr is at 0x34)
    int NumEvents;                  // 0x000 [O] cPath::NumEvents; [P] getNumEvents (lw 0x0); [U] cPath_initPath, the
                                    //       event walks (findEvent, getEventsInRange, isForkInPath, getResetPos ...)
    tPathEventNode* Events;         // 0x004 [O]; [P] getAllEvents (lw 0x4); [U] initPath, the event walks
    int NumNodes;                   // 0x008 [O]; [U] initPath, getLength / getPathPos / intersectPath
    float Start[3];                 // 0x00C [O] t3Vector; [U] initPath, getStart / getPathPos / intersectPath
    tPathNode* Nodes;               // 0x018 [O]; [U] initPath, getLength / getPathPos / getPathHeading / intersectPath
    float MinBound[3];              // 0x01C [O] t3Vector; [U] initPath, getProximity / isPathInBox
    float MaxBound[3];              // 0x028 [O] t3Vector; [U] initPath, getProximity / isPathInBox
    void** mVtbl;                   // 0x034 [R] cTrackPath_vtbl (the inlined array ctor in cPathSys_loadTrackPaths);
                                    //       [U] initPath calls slots 3 / 4 (lw 0x34)
    // --- cTrackPath
    float BaseDistanceLeft;         // 0x038 [O]; [P] getBaseDistanceLeft (lwc1 0x38); [U] populateDataWithDefaults = 0
                                    //       (sw $0), populateData(id 0) reads a float into it; getEventsInRange /
                                    //       isForkInPath subtract from it (lwc1; view sBXRangeList field_0x38)

    // Methods bound to their retail symbols by asm labels.
    void populateDataWithDefaults() __asm__("cTrackPath_populateDataWithDefaults__FPv");
    void populateData(int data, int fieldId) __asm__("cTrackPath_populateData");
    int getEventsInRange(tPathEventNode* out, int maxOut, float ldist, float hdist)
        __asm__("cTrackPath_getEventsInRange");
    int isForkInPath(float ldist, float hdist) __asm__("cTrackPath_isForkInPath");
};

CHECK_OFFSET(cTrackPath, NumEvents, 0x000);
CHECK_OFFSET(cTrackPath, Events, 0x004);
CHECK_OFFSET(cTrackPath, NumNodes, 0x008);
CHECK_OFFSET(cTrackPath, Start, 0x00C);
CHECK_OFFSET(cTrackPath, Nodes, 0x018);
CHECK_OFFSET(cTrackPath, MinBound, 0x01C);
CHECK_OFFSET(cTrackPath, MaxBound, 0x028);
CHECK_OFFSET(cTrackPath, mVtbl, 0x034);

CHECK_OFFSET(cTrackPath, BaseDistanceLeft, 0x038);
CHECK_SIZE(cTrackPath, 0x3C);

#endif /* PATH_TRACKPATH_H */
