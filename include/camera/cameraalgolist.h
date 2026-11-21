#ifndef CAMERA_CAMERAALGOLIST_H
#define CAMERA_CAMERAALGOLIST_H

// cCameraAlgoList: a camera controller's list of algorithms (racecontroller.h AlgorithmList); its nodes are allocated
// as "Algorithm List Node". Methods in camera/cameraalgolist and be/bedbreward.
//
// Size 0x8 (the retail allocation, cMemMan_alloc(8, "Algorithm List"); On Tour's is the same). Member names, order and
// offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access through `this` falls on a member start
// (tools/gen/self_offsets.py).

#include "layout.h"

struct cCameraAlgorithm;

// One algorithm in a camera controller's list with its blend weights (On Tour class cCameraAlgoListNode, 0x1C), a
// doubly linked list from cCameraAlgoList::Head. [Staged by views group A.]
// Size 0x1C: [R] cCameraAlgoList_insert allocates cMemMan_alloc(0x1C, "Algorithm List Node"). [O] On Tour's is 0x1C
// with the same members; its last bool is a word in retail. Tags: [R] cCameraAlgoList_insert (src/camera/
// cameraalgolist.cpp, view sAlgoNodeK2); [U] cChaseCameraController_update (view sAlgoNode_161BB8) and
// cCameraAlgoList_remove; [O] On Tour's DWARF.
struct cCameraAlgoListNode {
    cCameraAlgorithm* Algorithm;    // 0x000 [O]; [R] insert stores the algorithm; [U] the controllers call its virtuals
    float Weight;                   // 0x004 [O]; [R] insert: 1 when the target weight is exactly 1, else 0; [U] the chase
                                    //       controller's update moves it toward BlendRate's target
    float BlendRate;                // 0x008 [O]; [R] insert stores the target weight argument; [U] update reads it
    float SmoothWeight;             // 0x00C [O]; [U] cChaseCameraController_update (sAlgoNode_161BB8::blend)
    cCameraAlgoListNode* Prev;      // 0x010 [O]; [R] insert: 0, and the old head's Prev = the new node
    cCameraAlgoListNode* Next;      // 0x014 [O]; [R] insert: the old head
    int m_deleteAlgorithmOnDestruction; // 0x018 [O] bool (a word in retail); [R] insert stores its `owned` argument;
                                    //       [U] cCameraAlgoList_remove deletes the algorithm when it is set
};

CHECK_OFFSET(cCameraAlgoListNode, Algorithm, 0x000);
CHECK_OFFSET(cCameraAlgoListNode, Weight, 0x004);
CHECK_OFFSET(cCameraAlgoListNode, BlendRate, 0x008);
CHECK_OFFSET(cCameraAlgoListNode, SmoothWeight, 0x00C);
CHECK_OFFSET(cCameraAlgoListNode, Prev, 0x010);
CHECK_OFFSET(cCameraAlgoListNode, Next, 0x014);
CHECK_OFFSET(cCameraAlgoListNode, m_deleteAlgorithmOnDestruction, 0x018);
CHECK_SIZE(cCameraAlgoListNode, 0x1C);

struct cCameraAlgoList {
    cCameraAlgoListNode* Head;      // 0x000 [O]
    int Length;                     // 0x004 [O]
};

CHECK_OFFSET(cCameraAlgoList, Head, 0x000);
CHECK_OFFSET(cCameraAlgoList, Length, 0x004);
CHECK_SIZE(cCameraAlgoList, 0x8);

#endif
