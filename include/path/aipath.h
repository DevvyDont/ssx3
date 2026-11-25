#ifndef PATH_AIPATH_H
#define PATH_AIPATH_H

// cAIPath: one AI path of the path file - a cPath (path/basepath.h) plus its attribute bits and whether it is a reset
// path. cPathSys::AIPaths (path/pathsys.h) is the array cPathSys_loadAIPaths allocates ("AIPaths") and builds with the
// inlined array ctor (vtable cAIPath_vtbl at +0x34), then cPath_initPath reads each one, which calls the virtual
// populateDataWithDefaults / populateData (slots 3 / 4) to fill the members below. cRider::AIPath (ai/rider.h) points
// to one; the preview's cRider::getAIPath returns it.
// EA's source: d:/ssx/code/source/path/aipath.cpp (preview assert path). Retail code: unit bx/bxstringctor
// (0x00269EC0..0x0026A000) and the out-of-line inline getters in path/pathsys (0x0026C420 / 0x0026C428).
//
// Size 0x40: [R] cPathSys_loadAIPaths steps the array 0x40 (its element view sPathB410 in src/path/pathsys.cpp: vptr
// 0x34, words 0x38 / 0x3C); [O] On Tour's cAIPath is 0x40 (PathAttributes 0x38, IsResetPath 0x3C).
// Base: cPath (0x00-0x38, path/basepath.h, copied inline with its names; cPath's vptr at 0x34 is the only one).
// Vtable cAIPath_vtbl (cAIPath's; preview _vt$7cAIPath; 4 slots: 1 cPath_getEventsInRange, 2 cPath_isForkInPath,
// 3 cAIPath_populateDataWithDefaults, 4 cAIPath_populateData).
//
// A FLAT layout, like main/game.h: every field at its retail offset, no C++ inheritance or virtuals. Evidence tags:
//   [R] the inlined array ctor in cPathSys_loadAIPaths;
//   [P] the preview's inline getters getPathAttributes__C7cAIPath (lw 0x38) and getIsResetPath__C7cAIPath (lw 0x3C)
//       (tools/layout_xlate.py: "seen", preview == retail); cPath's getNumEvents / getAllEvents;
//   [U] how retail code uses the field;
//   [O] SSX On Tour's DWARF (class cAIPath; class cPath): names, types, order and offsets.
// t3Vector members are float[3], as in path/basepath.h.

#include "layout.h"

struct tPathEventNode;
struct tPathNode;

struct cAIPath {
    // --- cPath (path/basepath.h, 0x38; its vptr is at 0x34)
    int NumEvents;                  // 0x000 [O] cPath::NumEvents; [P] getNumEvents (lw 0x0); [U] cPath_initPath, the
                                    //       event walks (findEvent, getEventsInRange, isForkInPath, getResetPos ...)
    tPathEventNode* Events;         // 0x004 [O]; [P] getAllEvents (lw 0x4); [U] initPath, the event walks
    int NumNodes;                   // 0x008 [O]; [U] initPath, getLength / getPathPos / intersectPath
    float Start[3];                 // 0x00C [O] t3Vector; [U] initPath, getStart / getPathPos / intersectPath
    tPathNode* Nodes;               // 0x018 [O]; [U] initPath, getLength / getPathPos / getPathHeading / intersectPath
    float MinBound[3];              // 0x01C [O] t3Vector; [U] initPath, getProximity / isPathInBox
    float MaxBound[3];              // 0x028 [O] t3Vector; [U] initPath, getProximity / isPathInBox
    void** mVtbl;                   // 0x034 [R] cAIPath_vtbl (the inlined array ctor in cPathSys_loadAIPaths);
                                    //       [U] initPath calls slots 3 / 4 (lw 0x34)
    // --- cAIPath
    int PathAttributes;             // 0x038 [O]; [P] getPathAttributes (lw 0x38); [U] populateDataWithDefaults = 46,
                                    //       populateData(id 100) reads a tInt32 into it; cAIPath_getPathAttributes
    int IsResetPath;                // 0x03C [O] (an int in On Tour too); [P] getIsResetPath (lw 0x3C); [U]
                                    //       populateDataWithDefaults = 1, populateData(id 101); cAIPath_getIsResetPath

    // Methods bound to their retail symbols by asm labels.
    int populateDataWithDefaults() __asm__("cAIPath_populateDataWithDefaults__FPv");
    void populateData(int data, int id) __asm__("cAIPath_populateData");
};

CHECK_OFFSET(cAIPath, NumEvents, 0x000);
CHECK_OFFSET(cAIPath, Events, 0x004);
CHECK_OFFSET(cAIPath, NumNodes, 0x008);
CHECK_OFFSET(cAIPath, Start, 0x00C);
CHECK_OFFSET(cAIPath, Nodes, 0x018);
CHECK_OFFSET(cAIPath, MinBound, 0x01C);
CHECK_OFFSET(cAIPath, MaxBound, 0x028);
CHECK_OFFSET(cAIPath, mVtbl, 0x034);

CHECK_OFFSET(cAIPath, PathAttributes, 0x038);
CHECK_OFFSET(cAIPath, IsResetPath, 0x03C);
CHECK_SIZE(cAIPath, 0x40);

#endif /* PATH_AIPATH_H */
