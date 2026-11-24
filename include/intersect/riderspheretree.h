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

// One sphere of a rider's collision list (On Tour struct tSphereList, 0x20): its centre in bone space, radius and bone.
// [U] cRiderSphereTree's methods walk sphList 0x20 apart. On Tour's has an (implicit) default constructor; the
// constructors model it with a local derived type (tSphereListInit in intersect/riderspheretree.cpp), not here: a
// constructor here would give cWorldRider (which embeds a cRiderSphereTree) one, and its locals would run it.
struct tSphereList {
    t4Vector cen;                   // 0x00 [O]
    float rad;                      // 0x10 [O]
    int bone;                       // 0x14 [O]
};

CHECK_OFFSET(tSphereList, cen, 0x00);
CHECK_OFFSET(tSphereList, rad, 0x10);
CHECK_OFFSET(tSphereList, bone, 0x14);
CHECK_SIZE(tSphereList, 0x20);

struct cRiderSphereTree {
    tPose* bonePoses;               // 0x000 [O]
    char unk_004[0xC];              // 0x004 padding
    t4Vector boundCen;              // 0x010 [O]
    float boundRad;                 // 0x020 [O]
    int boundBone;                  // 0x024 [O]
    unsigned int sphMask;           // 0x028 [O]
    int listLen;                    // 0x02C [O]
    tSphereList sphList[20];        // 0x030 [O] tSphereList[20] (0x280 bytes)
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
