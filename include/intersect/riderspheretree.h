#ifndef INTERSECT_RIDERSPHERETREE_H
#define INTERSECT_RIDERSPHERETREE_H

// cRiderSphereTree: a rider's (or board's) collision sphere tree: up to 20 spheres (tSphereList) placed on the bone
// poses, a bounding sphere around them and a mask of active spheres. cRiderHandler keeps the board's (BoardSphereTree,
// ai/riderhandler.h). Methods in intersect/riderspheretree: Init, Update, ApplyOffset, ApplyTransform, GetBoundBox,
// GetMaskBoundBox, IsTriCollision, Is2SidedTriCollision, IsBoxCollision, IsRiderCollision, operator=.
// Size 0x2B0 [R] (the retail allocation; On Tour's is the same). A FLAT layout, like main/game.h: member names,
// order and offsets are EA's, from SSX On Tour's PSP DWARF ([O]), and every retail access in the methods
// falls on a member start. Enums and bools are words; members of class type are byte arrays (type in
// the comment).

#include "layout.h"
#include "bx/math3d.h"

struct tPose;

struct cRiderSphereTree {
    tPose* bonePoses;               // 0x000 [O]
    char unk_004[0xC];              // 0x004 padding
    t4Vector boundCen;              // 0x010 [O]
    float boundRad;                 // 0x020 [O]
    int boundBone;                  // 0x024 [O]
    unsigned int sphMask;           // 0x028 [O]
    int listLen;                    // 0x02C [O]
    char sphList[0x280];            // 0x030 [O] tSphereList[20]
};

CHECK_OFFSET(cRiderSphereTree, bonePoses, 0x000);
CHECK_OFFSET(cRiderSphereTree, boundCen, 0x010);
CHECK_OFFSET(cRiderSphereTree, boundRad, 0x020);
CHECK_OFFSET(cRiderSphereTree, boundBone, 0x024);
CHECK_OFFSET(cRiderSphereTree, sphMask, 0x028);
CHECK_OFFSET(cRiderSphereTree, listLen, 0x02C);
CHECK_OFFSET(cRiderSphereTree, sphList, 0x030);
CHECK_SIZE(cRiderSphereTree, 0x2B0);

#endif
