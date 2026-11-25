#ifndef INTERSECT_SPHERETREE_H
#define INTERSECT_SPHERETREE_H

// SphereTree: EA's octree of spheres around one mesh, placed in world space: 8 child axes (4 directions and their
// negatives), the root centre, a uniform scale, the shared packed tree data (tSphTreeInfo) and a per-query child
// visiting order. bxSphereTree (object/rollermodifier.h, 0x140) is a SphereTree + its own tSphTreeInfo staticInfo
// at 0xC0; cWorldSphTree (intersect/worldsphtree.h) wraps one for world queries, cRollerModifier embeds one.
// No EA source path in the preview's asserts (On Tour: intersect/all_in_one_intersect.cpp); the methods are in the
// retail intersect/riderspheretree splat unit (0x32C508..0x32E100).
// Size 0xC0: bxSphereTree's own member starts at 0xC0 ([R] bxSphereTree_operatorE copies 0xC0-0x140,
// bxSphereTree_Load stores staticInfo words at 0xC0/0xC4/0xD0; cMemMan_alloc(0x140, "SphereTree") in
// cWorldSphTree_cWorldSphTree), and On Tour's SphereTree is 0xC0. No vtable, no base class.
//
// A FLAT layout, like main/game.h. Evidence tags:
//   [R] retail constructor SphereTree_SphereTree (zeroes treeInfo, calls SphereTree_init) and SphereTree_init;
//   [U] how retail code uses the field (C bodies in src/intersect/riderspheretree.cpp and the retail asm);
//   [O] SSX On Tour's DWARF (class SphereTree, struct tSphTreeInfo, struct tLevel); On Tour's offsets equal
//       retail's for every member here.
// No preview accessors (tools/layout_xlate.py: none paired).
// Statics (On Tour SphereTree::buffers ... bigBufDepth): the retail globals SphereTree_buffers ..
// SphereTree_bigBufDepth
// (0x004A3FC8..0x004A3FE8), used by the static SphereTree_initCache / SphereTree_purgeCache / SphereTree_AllocBuf.

#include "layout.h"
#include "bx/math3d.h"

// One level of the packed octree (On Tour tLevel, 0xC).
struct tLevel {
    float radius;                   // 0x0 [O]; [U] SphereTree_GetRadius reads levels[0].radius, GetResolution
                                    //       levels[depth]
    float cenOffset;                // 0x4 [O]; [U] isSphereCollision/isTreeCollision: child centre = centre + axis *
                                    //       levels[l+1].cenOffset
    int indexOffset;                // 0x8 [O]; [U] child index = (i + 1) * levels[l].indexOffset + index
};

CHECK_OFFSET(tLevel, radius, 0x0);
CHECK_OFFSET(tLevel, cenOffset, 0x4);
CHECK_OFFSET(tLevel, indexOffset, 0x8);
CHECK_SIZE(tLevel, 0xC);

// The packed tree shared by every placement of one mesh (On Tour tSphTreeInfo, 0x80; bxSphereTree::staticInfo).
// t3Vector / t3Matrix members are written as float arrays.
struct tSphTreeInfo {
    int treeID;                     // 0x00 [O]; [U] SphereTree_AllocBuf compares it with the cache slot's id
    int lastBufID;                  // 0x04 [O]; [U] SphereTree_AllocBuf: last cache slot used (lw / addiu 0x4)
    int isCompressed;               // 0x08 [O]; [U] Is*Collision: nonzero -> bitTree = SphereTree_AllocBuf(treeInfo)
    int depth;                      // 0x0C [O]; [U] recursion limit in is*/check*Collision; AllocBuf picks the big
                                    //       cache by it
    int refCount;                   // 0x10 [O]
    float rootCenter[3];            // 0x14 [O] t3Vector; [U] SphereTree_AddOffset / TransformAbs read n[5..7]
    tLevel* levels;                 // 0x20 [O]; [U] GetRadius, GetResolution, the recursive tests
    unsigned char* cmpTree;         // 0x24 [O]; [U] SphereTree_AllocBuf unpacks it (sphTreeUnRLE)
    unsigned char* bitTree;         // 0x28 [O]; [U] child-occupancy bytes (masks[index]); Is*Collision store AllocBuf's
                                    //       result
    float centerOfMass[3];          // 0x2C [O] t3Vector; [U] cRollerModifier's ctors read +0x2C
    float momentOfInertia[3][3];    // 0x38 [O] t3Matrix
    float invMomentOfInertia[3][3]; // 0x5C [O] t3Matrix; [U] cRollerModifier's ctors read +0x5C
};

CHECK_OFFSET(tSphTreeInfo, treeID, 0x00);
CHECK_OFFSET(tSphTreeInfo, lastBufID, 0x04);
CHECK_OFFSET(tSphTreeInfo, isCompressed, 0x08);
CHECK_OFFSET(tSphTreeInfo, depth, 0x0C);
CHECK_OFFSET(tSphTreeInfo, refCount, 0x10);
CHECK_OFFSET(tSphTreeInfo, rootCenter, 0x14);
CHECK_OFFSET(tSphTreeInfo, levels, 0x20);
CHECK_OFFSET(tSphTreeInfo, cmpTree, 0x24);
CHECK_OFFSET(tSphTreeInfo, bitTree, 0x28);
CHECK_OFFSET(tSphTreeInfo, centerOfMass, 0x2C);
CHECK_OFFSET(tSphTreeInfo, momentOfInertia, 0x38);
CHECK_OFFSET(tSphTreeInfo, invMomentOfInertia, 0x5C);
CHECK_SIZE(tSphTreeInfo, 0x80);

struct t3Vector;
struct t4Vector;

struct SphereTree {
    t4Vector axis[8];               // 0x000 [O]; [R] SphereTree_init copies the 8 default axes gSphTreeAlignAxis;
                                    //       [U] TransformAbs/TransformRel write axis[i] and axis[7 - i] = -axis[i];
                                    //       populateSortAxis dots axis[0..3]; the tests step to children along axis[i]
    t4Vector transCenter;           // 0x080 [O]; [R] init stores t4vect_Origin; [U] MoveOffset adds to it (lqc2/sqc2),
                                    //       AddOffset/TransformAbs/TransformRel set it; Is*Collision pass &transCenter
                                    //       as the root
    float scale;                    // 0x090 [O]; [R] init stores 1.0f; [U] GetRadius/GetResolution multiply by it,
                                    //       TransformAbs stores the scale, TransformRel multiplies it
    float worstCase;                // 0x094 [O]; [U] IsPlaneCollision/IsTriCollision clear it (0.0f),
                                    //       checkPlaneCollision/
                                    //       checkTriCollision keep the deepest penetration; < 0 means a hit
    tSphTreeInfo* treeInfo;         // 0x098 [O]; [R] SphereTree_SphereTree zeroes it; [U] every query (levels, depth,
                                    //       bitTree), bxSphereTree_Load points it at staticInfo
    int sortAxis[8];                // 0x09C [O]; [U] populateSortAxis sorts the 8 child indices into it (BXSORT_shell);
                                    //       checkPlaneCollision/checkTriCollision visit children in this order
    char unk_0BC[0x4];              // 0x0BC padding (16-byte aligned class)

    // Methods bound to their retail symbols by asm labels.
    void init() __asm__("SphereTree_init");
    float GetRadius() __asm__("SphereTree_GetRadius");
    float GetResolution() __asm__("SphereTree_GetResolution");
    void MoveOffset(t4Vector* offset) __asm__("SphereTree_MoveOffset");
    void TransformAbs(t4Vector* transMat, float scale) __asm__("SphereTree_TransformAbs");
    int IsSphereCollision(t4Vector* spherePos, float sphereRadius, t4Vector* outPush, t4Vector* outContact)
        __asm__("SphereTree_IsSphereCollision");
    int IsBoxCollision(t3Vector* boxMin, t3Vector* boxMax, t4Vector* outPos, t4Vector* outNormal)
        __asm__("SphereTree_IsBoxCollision");
    int IsTriCollision(t4Vector* point1, t4Vector* point2, t4Vector* point3, t4Vector* uNorm, t4Vector* overlap,
                       t4Vector* loc)
        __asm__("SphereTree_IsTriCollision");
    int IsTreeCollision(SphereTree* other, t4Vector* outPush, t4Vector* outContact)
        __asm__("SphereTree_IsTreeCollision");
    int isTreeCollision(SphereTree* other, t4Vector* posp, int depth, int base, t4Vector* outPush, t4Vector* outContact)
        __asm__("SphereTree_isTreeCollision");
    int isSphereCollision(t4Vector* spherePos, float sphereRadius, t4Vector* pos, int depth, int base, t4Vector* outA,
                          t4Vector* outB)
        __asm__("SphereTree_isSphereCollision");
    int isBoxCollision(t3Vector* boxMin, t3Vector* boxMax, t4Vector* pos, int depth, int base, t4Vector* outPos,
                       t4Vector* outNormal)
        __asm__("SphereTree_isBoxCollision");
    void checkPlaneCollision(void* plane, t4Vector* pos, int depth, int base, t4Vector* outA, t4Vector* outB)
        __asm__("SphereTree_checkPlaneCollision");
    int isSphereInsideTri(t4Vector* corner1, t4Vector* corner2, t4Vector* corner3, t4Vector* sphereCenter)
        __asm__("SphereTree_isSphereInsideTri");
    void populateSortAxis(t4Vector* vdir) __asm__("SphereTree_populateSortAxis");
};

CHECK_OFFSET(SphereTree, axis, 0x000);
CHECK_OFFSET(SphereTree, transCenter, 0x080);
CHECK_OFFSET(SphereTree, scale, 0x090);
CHECK_OFFSET(SphereTree, worstCase, 0x094);
CHECK_OFFSET(SphereTree, treeInfo, 0x098);
CHECK_OFFSET(SphereTree, sortAxis, 0x09C);
CHECK_OFFSET(SphereTree, unk_0BC, 0x0BC);
CHECK_SIZE(SphereTree, 0xC0);

#endif /* INTERSECT_SPHERETREE_H */
