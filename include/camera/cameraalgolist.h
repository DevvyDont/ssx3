#ifndef CAMERA_CAMERAALGOLIST_H
#define CAMERA_CAMERAALGOLIST_H

// cCameraAlgoList: a camera controller's list of algorithms (racecontroller.h AlgorithmList); its nodes are allocated
// as "Algorithm List Node". Methods in camera/cameraalgolist and be/bedbreward.
//
// Size 0x8 (the retail allocation, cMemMan_alloc(8, "Algorithm List"); On Tour's is the same). Member names, order and
// offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start
// (tools/gen/self_offsets.py).

#include "layout.h"

struct cCameraAlgoListNode;

struct cCameraAlgoList {
    cCameraAlgoListNode* Head;      // 0x000 [O]
    int Length;                     // 0x004 [O]
};

CHECK_OFFSET(cCameraAlgoList, Head, 0x000);
CHECK_OFFSET(cCameraAlgoList, Length, 0x004);
CHECK_SIZE(cCameraAlgoList, 0x8);

#endif
