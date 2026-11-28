#include "vtable.h"
#include "common.h"
#include "object/being.h"
#include "object/debouncenode.h"
#include "object/movenode.h"
#include "object/instancenode.h"
#include "world/patchrecord.h"
#include "world/worldrecords.h"
#include "world/instancerecord.h"
#include "intersect/worldinttypes.h"
#include "ai/rider.h"
#include "world/worldview.h"
#include "bx/appman.h"
#include "intersect/worldline.h"
#include "intersect/bxnash.h"
#include "intersect/overlaplist.h"
#include "object/animteeternode.h"
#include "object/boostnode.h"
#include "object/animobject.h"
#include "intersect/worldsphtree.h"
#include "intersect/worldsphere.h"
#include "object/rollermodifier.h"
#include "object/boostmodifier.h"
#include "world/world.h"
#include "object/railspacenode.h"
#include "replay/bxreplayframe.h"

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_cWorldSphTree);
#ifdef SKIP_ASM
extern void* cWorldSphTree_vtbl[];
extern char str_SphereTree[];
extern "C" void* __10SphereTree(void* p);
extern "C" float SphereTree_GetRadius(void* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);


/**
 * Constructs a world-intersection wrapper around a sphere tree, with a bounding box from its root sphere.
 *
 * Stores cWorldSphTree's vtable at +0x50, clears +0x4 and +0x8, sets +0x0 to 1 unless @p isLow and keeps @p
 * ast at +0x60 and +0x64. Copies the tree's transCenter to +0x40 and stores centre + r and centre - r (w = 1)
 * at +0x20 and +0x30, r being SphereTree_GetRadius()'s radius. Also allocates a fresh 0x140-byte "SphereTree"
 * (SphereTree::SphereTree) at +0x68, which the destructor cWorldSphTree__cWorldSphTree frees.
 *
 * @param ast    the bxSphereTree to wrap
 * @param isLow  nonzero for a low-detail wrapper: +0x0 is then 0 and cWorldSphTree_GetIdealParam() returns -1
 * @return @p self
 */
extern "C" void* cWorldSphTree_cWorldSphTree(cWorldSphTree* self, bxSphereTree* ast, int isLow)
{
    self->mVtbl = cWorldSphTree_vtbl;
    self->useRejectNormal = 0;
    self->isTransformed = 0;
    if (isLow != 0) {
        *(int*)self = 0;
    } else {
        *(int*)self = 1;
    }
    self->origSt = (bxSphereTree*)((void*)ast);
    self->st = (bxSphereTree*)((void*)ast);
    t4Vector center = ast->transCenter;
    float radius = SphereTree_GetRadius(ast);
    t4Vector corner;
    self->boxCenter = center;
    corner.x = center.x + radius;
    corner.y = center.y + radius;
    corner.z = center.z + radius;
    corner.w = 1.0f;
    self->boxMax = corner;
    corner.x = center.x - radius;
    corner.y = center.y - radius;
    corner.z = center.z - radius;
    corner.w = 1.0f;
    self->boxMin = corner;
    void* localTree = cMemMan_alloc(320, str_SphereTree, 0, 0);
    __10SphereTree(localTree);
    self->localTree = (bxSphereTree*)localTree;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree__cWorldSphTree);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* cWorldSphTree_vtbl[];

/**
 * Destroys a world sphere-tree primitive: restores cWorldSphTree_vtbl (+0x50), frees its working tree copy (+0x68)
 * and, if asked, itself.
 *
 * The copy is released with operator_delete only (no destructor runs).
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cWorldSphTree__cWorldSphTree(cWorldSphTree* self, int flags)
{
    self->mVtbl = cWorldSphTree_vtbl;
    operator_delete((int*)self->localTree);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_IsInBoxTransformed);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector add_330540(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector sub_330540(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

extern "C" float SphereTree_GetRadius(void* self);

/**
 * Tells whether the current sphere tree's root sphere (centre at tree + 0x80) can touch an axis-aligned box.
 *
 * The radius comes from SphereTree_GetRadius(tree), called once per axis.
 *
 * @param lo  the box's minimum corner (On Tour bMin)
 * @param hi  the box's maximum corner (On Tour bMax)
 * @return false when the sphere's bounding box lies wholly beyond one face, otherwise true
 */
bool cWorldSphTree::IsInBoxTransformed(float* lo, float* hi)
{
    t4Vector ext;
    float rx = SphereTree_GetRadius(st);
    float ry = SphereTree_GetRadius(st);
    float rz = SphereTree_GetRadius(st);
    ext.x = rx;
    ext.y = ry;
    ext.z = rz;
    ext.w = 0.0f;
    t4Vector mx = add_330540(*(&st->transCenter), ext);
    t4Vector mn = sub_330540(*(&st->transCenter), ext);
    float lx = lo[0];
    float tx = mx.x;
    if (tx < lx && mn.x < lx) return false;
    if (hi[0] < tx && hi[0] < mn.x) return false;
    float ly = lo[1];
    float ty = mx.y;
    if (ty < ly && mn.y < ly) return false;
    if (hi[1] < ty && hi[1] < mn.y) return false;
    float lz = lo[2];
    float tz = mx.z;
    if (tz < lz && mn.z < lz) return false;
    if (!(hi[2] < tz) || !(hi[2] < mn.z)) return true;
    return false;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_GetIdealParam);
#ifdef SKIP_ASM
extern "C" float SphereTree_GetRadius(void* self);

/**
 * Returns the wrapper's ideal query parameter: twice the wrapped sphere tree's radius, or -1.
 *
 * Uses SphereTree_GetRadius() on the tree at +0x60 when the flag at +0x0 is set (the constructor's isLow was 0).
 *
 * @return 2 * radius, or -1.0 for a low-detail wrapper
 */
float cWorldSphTree::GetIdealParam()
{
    float idealParam;
    if (*(int*)this != 0) {
        idealParam = SphereTree_GetRadius(st) * 2.0f;
    } else {
        idealParam = -1.0f;
    }
    return idealParam;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_Transform);
#ifdef SKIP_ASM
extern "C" void* bxSphereTree_operatorE(void* self, void* src);
extern "C" void SphereTree_TransformRel(void* p, void* a, float f);

/**
 * Transforms the sphere tree into a model's space using its working copy.
 *
 * Copies the current tree (+0x60) into the copy at +0x68 (bxSphereTree_operatorE, On Tour bxSphereTree::operator=), makes
 * the copy current, applies @p mat and @p scale (SphereTree_TransformRel) and sets the transformed flag (+0x4).
 *
 * @param mat  the transform (On Tour const t4Matrix& mat)
 * @param scale  the uniform scale (On Tour scale)
 */
void cWorldSphTree::Transform(t4Matrix* mat, float scale)
{
    bxSphereTree_operatorE(localTree, st);
    st = (bxSphereTree*)((void*)localTree);
    SphereTree_TransformRel(st, mat, scale);
    isTransformed = 1;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_UnTransform__FPv);
#ifdef SKIP_ASM
/**
 * Undoes cWorldSphTree_Transform: makes the original tree (+0x64) current again (+0x60) and clears the transformed
 * flag.
 *
 * @return the original tree (On Tour returns void)
 */
bxSphereTree* cWorldSphTree::UnTransform()
{
    bxSphereTree* original = origSt;
    isTransformed = 0;
    st = original;
    return original;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_IsTriInt);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330788(const t4Vector& v)
{
    float r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %0, $vi22\n"
        : "=r"(r)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s).
static inline t4Vector wsDiv_330788(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        ".set pop\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

extern "C" int SphereTree_IsTriCollision(void* tree, int a1, int a2, int a3, int a4, t4Vector* out, int a6);

/**
 * Collides the sphere tree with a triangle and returns the push-out direction and depth.
 *
 * Calls SphereTree_IsTriCollision (On Tour SphereTree::IsTriCollision) on the current tree; on a hit the penetration vector is
 * split into its length and unit direction.
 *
 * @param vertA  the triangle's first corner (On Tour a)
 * @param vertB  the second corner (On Tour b)
 * @param vertC  the third corner (On Tour c)
 * @param n  the triangle's normal (On Tour n)
 * @param ipos  receives the collision point (On Tour ipos)
 * @param outDir  receives the unit push-out direction (On Tour inorm)
 * @param outLen  receives the penetration depth (On Tour param)
 * @return nonzero on a collision, otherwise 0
 */
int cWorldSphTree::IsTriInt(int vertA, int vertB, int vertC, int n, int ipos, t4Vector* outDir, float* outLen)
{
    t4Vector hitDir;
    int hit = SphereTree_IsTriCollision(st, vertA, vertB, vertC, n, &hitDir, ipos);
    if (hit != 0) {
        float len = wsLength_330788(hitDir);
        *outLen = len;
        *outDir = wsDiv_330788(hitDir, len);
    }
    return hit;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_Is2SidedTriInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330828(const t4Vector& v)
{
    float r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %0, $vi22\n"
        : "=r"(r)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (v / s).
static inline t4Vector wsDiv_330828(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %2\n"
        "vwaitq\n"
        "vmulq.xyzw $vf4, $vf4, Q\n"
        "sqc2      $vf4, %0\n"
        ".set pop\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

extern "C" int bxSphereTree_Is2SidedTriCollision(void* tree, int a1, int a2, int a3, int a4, t4Vector* out, int a6);

/**
 * Collides the sphere tree with a two-sided triangle and returns the push-out direction and depth.
 *
 * As cWorldSphTree_IsTriInt with bxSphereTree_Is2SidedTriCollision (On Tour bxSphereTree::Is2SidedTriCollision).
 *
 * @param vertA  the triangle's first corner (On Tour a)
 * @param vertB  the second corner (On Tour b)
 * @param vertC  the third corner (On Tour c)
 * @param n  the triangle's normal (On Tour n)
 * @param ipos  receives the collision point (On Tour ipos)
 * @param outDir  receives the unit push-out direction (On Tour inorm)
 * @param outLen  receives the penetration depth (On Tour param)
 * @return nonzero on a collision, otherwise 0
 */
int cWorldSphTree::Is2SidedTriInt(int vertA, int vertB, int vertC, int n, int ipos, t4Vector* outDir, float* outLen)
{
    t4Vector hitDir;
    int hit = bxSphereTree_Is2SidedTriCollision(st, vertA, vertB, vertC, n, &hitDir, ipos);
    if (hit != 0) {
        float len = wsLength_330828(hitDir);
        *outLen = len;
        *outDir = wsDiv_330828(hitDir, len);
    }
    return hit;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_IsSphTreeInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_3308C8(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void wsDivEq_3308C8(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}


extern "C" int SphereTree_IsTreeCollision(void* self, void* other, void* a2, void* a3);

/**
 * Collides another sphere tree with this one's current tree and normalises the hit's push-out vector.
 *
 * @param tree  the other sphere tree (On Tour bxSphereTree* tree)
 * @param hit  receives the intersection (On Tour tIntersectInfo* ti); direction normalised, length stored
 * @return 1 on a hit (SphereTree_IsTreeCollision), otherwise 0
 */
int cWorldSphTree::IsSphTreeInt(void* tree, tIntersectInfo* hit)
{
    if (SphereTree_IsTreeCollision(st, tree, &hit->Inorm, hit) != 0) {
        float len = wsLength_3308C8(hit->Inorm);
        hit->Iparam = len;
        wsDivEq_3308C8(hit->Inorm, len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_IsBoxInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330950(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place vector divided by scalar).
static inline void wsDivEq_330950(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vdiv      Q, $vf0w, $vf3x\n"
        "lqc2      $vf4, %0\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf4, Q\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s)
        : "memory");
}


extern "C" int SphereTree_IsBoxCollision(void* self, void* a1, void* a2, void* a3, void* a4);

/**
 * Collides the current sphere tree with an axis-aligned box and normalises the hit's push-out vector.
 *
 * @param bMin  the box's minimum corner (On Tour bMin)
 * @param bMax  the box's maximum corner (On Tour bMax)
 * @param hit  receives the intersection (On Tour tIntersectInfo* ti); direction normalised, length stored
 * @return 1 on a hit (SphereTree_IsBoxCollision), otherwise 0
 */
int cWorldSphTree::IsBoxInt(void* bMin, void* bMax, tIntersectInfo* hit)
{
    if (SphereTree_IsBoxCollision(st, bMin, bMax, &hit->Inorm, hit) != 0) {
        float len = wsLength_330950(hit->Inorm);
        hit->Iparam = len;
        wsDivEq_330950(hit->Inorm, len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", OverlapInstancesCallback_operatorCall);
#ifdef SKIP_ASM
typedef int (*Test_3309D8)(void*);


struct sBox_3309D8;
typedef tBoundBox* (*GetBox_3309D8)(void*);


static inline int overlapNode_3309D8(const tBoundBox* b, const tInstanceRecord* n)
{
    int r = 0;
    if (b->MinPoint.x <= n->m_fvMax.m[0] && b->MaxPoint.x >= n->m_fvMin.m[0] &&
        b->MinPoint.y <= n->m_fvMax.m[1] && b->MaxPoint.y >= n->m_fvMin.m[1] &&
        b->MinPoint.z <= n->m_fvMax.m[2] && b->MaxPoint.z >= n->m_fvMin.m[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_3309D8(const tBoundBox* b, const tBoundBox* o)
{
    int r = 0;
    if (b->MinPoint.x <= o->MaxPoint.x && b->MaxPoint.x >= o->MinPoint.x &&
        b->MinPoint.y <= o->MaxPoint.y && b->MaxPoint.y >= o->MinPoint.y &&
        b->MinPoint.z <= o->MaxPoint.z && b->MaxPoint.z >= o->MinPoint.z) {
        r = 1;
    }
    return r;
}

/**
 * Collects the instances of an octree cell whose bounds overlap a query box.
 *
 * Walks the cell's instance list (@p tree nodesinside[0]). An instance flagged 0x20 is tested with its stored box
 * (m_fvMin / m_fvMax); one flagged 0x40 asks its object (m_pActiveNode) through virtual slot 44 whether it takes part
 * and through slot 45 for its box. Each overlapping instance is appended at @p ctx instances[*number] and the count
 * incremented. The maximum is never checked, so a full list overflows. The octree walkers take it as their cell
 * callback: cOverlapList_getinstances, cOverlapList_getpatchesandinstances, cOverlapList_geteverything, the helpX
 * walkers nash_privy_QueryHelper_helpX1, nash_privy_QueryHelper_helpX3 and nash_privy_QueryHelper_helpX4, and
 * nash_Cell_descend, nash_Cell_descend2 and nash_Cell_descend3.
 *
 * @param tree  the octree cell (On Tour const Cell& cell)
 */
void OverlapInstancesCallback::operatorCall(nash_Cell* tree)
{
    for (tInstanceRecord* node = (tInstanceRecord*)tree->nodesinside[0].head; node != 0;
         node = (tInstanceRecord*)node->m_iIntersect.next) {
        int flags = node->m_iFlags;
        if (flags & 0x20) {
            if (overlapNode_3309D8(box, node)) {
                unsigned int* cnt = number;
                int n = *cnt;
                instances[n] = node;
                *cnt = n + 1;
            }
        } else if (flags & 0x40) {
            if (node->m_pActiveNode->hasBoundBox()) {
                tBoundBox* objBox = (tBoundBox*)node->m_pActiveNode->getBoundBox();
                if (overlapBox_3309D8(box, objBox)) {
                    unsigned int* cnt = number;
                    int n = *cnt;
                    instances[n] = node;
                    *cnt = n + 1;
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_getinstances);
#ifdef SKIP_ASM
struct sSphNode_340A40;
extern "C" void nash_Cell_descend(nash_Cell* node, OverlapInstancesCallback* ctx);
extern "C" int nash_CellKey_proximitytest(void* cell, void* box);
extern "C" void nash_privy_QueryHelper_helpX1(char* flag, OverlapInstancesCallback* ctx, OverlapInstancesCallback* ctx2, void* box, nash_Cell* tree, void* cell);


static inline void collect_330BA8(nash_Cell* node, OverlapInstancesCallback* ctx)
{
    ctx->operatorCall((nash_Cell*)node);
    if (node->children[0][0][0] != 0) node->children[0][0][0]->descend(ctx);
    if (node->children[0][0][1] != 0) node->children[0][0][1]->descend(ctx);
    if (node->children[0][1][0] != 0) node->children[0][1][0]->descend(ctx);
    if (node->children[0][1][1] != 0) node->children[0][1][1]->descend(ctx);
    if (node->children[1][0][0] != 0) node->children[1][0][0]->descend(ctx);
    if (node->children[1][0][1] != 0) node->children[1][0][1]->descend(ctx);
    if (node->children[1][1][0] != 0) node->children[1][1][0]->descend(ctx);
    if (node->children[1][1][1] != 0) node->children[1][1][1]->descend(ctx);
}

// One top-level cell: test it, then either take the whole subtree or recurse into it.
static inline void queryCell_330BA8(cBXNash* nash, int i, void* box, OverlapInstancesCallback* ctx, char* flags)
{
    nash_Octant* cell = &nash->octant[i >> 2][(i >> 1) & 1][i & 1];
    if (nash->octant[i >> 2][(i >> 1) & 1][i & 1].rootcell != 0) {
        nash_Cell* tree = cell->rootcell;
        char* flag = &flags[i];
        int r = nash_CellKey_proximitytest(cell, box);
        if (r == 0) {
            collect_330BA8(tree, ctx);
        } else if (r == 2) {
            nash_privy_QueryHelper_helpX1(flag, ctx, ctx, box, tree, cell);
        }
    }
}

/**
 * Collects the instances whose bounds overlap @p box into the overlap list, walking the Nash's eight octants.
 *
 * Copies the Nash's deletecount to the list's deletecount and clears the instance count (+0x8), then runs an instance
 * collector (array +0xC, count +0x8, maximum 0x40, @p box) over every octant that has a root cell. When
 * nash_CellKey_proximitytest answers 0 (inside), the root goes to OverlapInstancesCallback_operatorCall and its
 * children to nash_Cell_descend (whole subtrees); when it answers 2 (crossing), the cell goes to nash_privy_QueryHelper_helpX1. Finally
 * sets the patch count (+0x10C) and the typed count (+0x210) to -1, marking those lists as not collected. Nothing in
 * the code calls it directly.
 *
 * @param box  the query box: min at +0x0, max at +0x10
 */
void cOverlapList::getinstances(tBoundBox* box)
{
    OverlapInstancesCallback ctx;
    char flags[8];
    cBXNash* nash = this->nash;
    OverlapInstancesCallback* pctx = &ctx;
    deletecount = nash->deletecount;
    ctx.instances = &instances[0];
    ctx.number = &numinstances;
    ctx.maximum = 0x40;
    ctx.box = box;
    numinstances = (unsigned int)0;
    queryCell_330BA8(nash, 0, box, pctx, flags);
    queryCell_330BA8(nash, 1, box, pctx, flags);
    queryCell_330BA8(nash, 2, box, pctx, flags);
    queryCell_330BA8(nash, 3, box, pctx, flags);
    queryCell_330BA8(nash, 4, box, pctx, flags);
    queryCell_330BA8(nash, 5, box, pctx, flags);
    queryCell_330BA8(nash, 6, box, pctx, flags);
    queryCell_330BA8(nash, 7, box, pctx, flags);
    numpatches = 0xFFFFFFFF;
    numtyped = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_gettyped);
#ifdef SKIP_ASM
struct sOctCollect;
extern "C" void nash_Cell_descend1(nash_Cell* self, sOctCollect* out);
extern "C" void nash_privy_QueryHelper_helpX2(char* flag, OverlapInstancesCallback* ctx, OverlapInstancesCallback* ctx2, void* box, nash_Cell* tree, void* cell);

struct sTag_331450 {
};


static inline void collect_331450(nash_Cell* node, OverlapInstancesCallback* ctx)
{
    nash_Node* p;
    for (p = node->nodesinside[2].head; p != 0; p = p->next) {
        ((tInstanceRecord**)ctx->instances)[(*((int*)ctx->number))++] = (tInstanceRecord*)p;
    }
    if (node->children[0][0][0] != 0) nash_Cell_descend1(node->children[0][0][0], (sOctCollect*)ctx);
    if (node->children[0][0][1] != 0) nash_Cell_descend1(node->children[0][0][1], (sOctCollect*)ctx);
    if (node->children[0][1][0] != 0) nash_Cell_descend1(node->children[0][1][0], (sOctCollect*)ctx);
    if (node->children[0][1][1] != 0) nash_Cell_descend1(node->children[0][1][1], (sOctCollect*)ctx);
    if (node->children[1][0][0] != 0) nash_Cell_descend1(node->children[1][0][0], (sOctCollect*)ctx);
    if (node->children[1][0][1] != 0) nash_Cell_descend1(node->children[1][0][1], (sOctCollect*)ctx);
    if (node->children[1][1][0] != 0) nash_Cell_descend1(node->children[1][1][0], (sOctCollect*)ctx);
    if (node->children[1][1][1] != 0) nash_Cell_descend1(node->children[1][1][1], (sOctCollect*)ctx);
}

static inline int queryCell_331450(cBXNash* nash, int i, void* box, OverlapInstancesCallback* ctx)
{
    nash_Octant* cell = &nash->octant[i >> 2][(i >> 1) & 1][i & 1];
    nash_Cell* tree;
    sTag_331450 tag;
    char* flag;
    int r = 1;
    if (nash->octant[i >> 2][(i >> 1) & 1][i & 1].rootcell != 0) {
        tree = cell->rootcell;
        flag = (char*)&tag;
        r = nash_CellKey_proximitytest(cell, box);
        if (r == 0) {
            collect_331450(tree, ctx);
        } else if (r == 2) {
            nash_privy_QueryHelper_helpX2(flag, ctx, ctx, box, tree, cell);
        }
    }
    return r;
}

/**
 * Collects every typed node of the octree cells that overlap @p box into the overlap list's typed list.
 *
 * Copies the Nash's deletecount to the list's deletecount, clears the typed count (+0x210) and walks the eight octants
 * with a typed collector (array +0x214, count +0x210, maximum 0x200, @p box). An inside cell
 * (nash_CellKey_proximitytest 0) has its typed list (cell +0x28) appended and its children walked whole by
 * nash_Cell_descend1; a crossing cell (2) goes to nash_privy_QueryHelper_helpX2. The nodes themselves are not box-tested and the maximum is
 * never checked (likely bug: more than 0x200 typed nodes in the visited cells overrun the array). Sets the instance
 * count (+0x8) and the patch count (+0x10C) to -1. Called by cWorldLightMan_getRealTimeLights.
 *
 * @param box  the query box: min at +0x0, max at +0x10
 */
void cOverlapList::gettyped(tBoundBox* box)
{
    OverlapInstancesCallback ctx;
    cBXNash* nash = this->nash;
    OverlapInstancesCallback* pctx = &ctx;
    deletecount = nash->deletecount;
    ctx.instances = (tInstanceRecord**)((void**)&typed[0]);
    ctx.number = &numtyped;
    ctx.maximum = 0x200;
    ctx.box = box;
    numtyped = 0;
    queryCell_331450(nash, 0, box, pctx);
    queryCell_331450(nash, 1, box, pctx);
    queryCell_331450(nash, 2, box, pctx);
    queryCell_331450(nash, 3, box, pctx);
    queryCell_331450(nash, 4, box, pctx);
    queryCell_331450(nash, 5, box, pctx);
    queryCell_331450(nash, 6, box, pctx);
    queryCell_331450(nash, 7, box, pctx);
    numpatches = 0xFFFFFFFF;
    numinstances = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_getpatchesandinstances);
#ifdef SKIP_ASM
struct sTree_340C20;
extern "C" void nash_Cell_descend2(nash_Cell* tree, OverlapInstancesCallback* ctx);
extern "C" void nash_privy_QueryHelper_helpX3(char* flag, OverlapInstancesCallback* ctx, OverlapInstancesCallback* ctx2, tBoundBox* vbox, void* vnode, void* vcell);

struct sObj_331E90 {
    sObj_331E90* next;
    char pad04[0x154];
    float bmin[3];              // 0x158
    float bmax[3];              // 0x164
};

struct sTree_331E90 {
    sTree_331E90* child[8];
    int pad20;
    sObj_331E90* objs;          // 0x24
};



static inline int overlapObj_331E90(const tBoundBox* b, const sObj_331E90* n)
{
    int r = 0;
    if (b->MinPoint.x <= n->bmax[0] && b->MaxPoint.x >= n->bmin[0] &&
        b->MinPoint.y <= n->bmax[1] && b->MaxPoint.y >= n->bmin[1] &&
        b->MinPoint.z <= n->bmax[2] && b->MaxPoint.z >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void collect_331E90(sTree_331E90* tree, OverlapInstancesCallback* ctx)
{
    ctx->operatorCall((nash_Cell*)tree);
    OverlapInstancesCallback* q = (OverlapInstancesCallback*)((char*)ctx + 0x10);
    for (sObj_331E90* p = tree->objs; p != 0; p = p->next) {
        if (overlapObj_331E90(((tBoundBox*)q->box), p)) {
            int* cnt = ((int*)q->number);
            int n = *cnt;
            ((tInstanceRecord**)q->instances)[n] = (tInstanceRecord*)p;
            *cnt = n + 1;
        }
    }
    if (tree->child[0] != 0) ((nash_Cell*)((sTree_340C20*)tree->child[0]))->descend2(ctx);
    if (tree->child[1] != 0) ((nash_Cell*)((sTree_340C20*)tree->child[1]))->descend2(ctx);
    if (tree->child[2] != 0) ((nash_Cell*)((sTree_340C20*)tree->child[2]))->descend2(ctx);
    if (tree->child[3] != 0) ((nash_Cell*)((sTree_340C20*)tree->child[3]))->descend2(ctx);
    if (tree->child[4] != 0) ((nash_Cell*)((sTree_340C20*)tree->child[4]))->descend2(ctx);
    if (tree->child[5] != 0) ((nash_Cell*)((sTree_340C20*)tree->child[5]))->descend2(ctx);
    if (tree->child[6] != 0) ((nash_Cell*)((sTree_340C20*)tree->child[6]))->descend2(ctx);
    if (tree->child[7] != 0) ((nash_Cell*)((sTree_340C20*)tree->child[7]))->descend2(ctx);
}

struct sTag_331E90 {
};

static inline void queryCell_331E90(cBXNash* nash, int i, tBoundBox* box, OverlapInstancesCallback* ctx)
{
    nash_Octant* cell = &nash->octant[i >> 2][(i >> 1) & 1][i & 1];
    if (nash->octant[i >> 2][(i >> 1) & 1][i & 1].rootcell != 0) {
        sTree_331E90* tree = (sTree_331E90*)cell->rootcell;
        sTag_331E90 tag;
        char* flag = (char*)&tag;
        int r = nash_CellKey_proximitytest(cell, box);
        if (r == 0) {
            collect_331E90(tree, ctx);
        } else if (r == 2) {
            nash_privy_QueryHelper_helpX3(flag, ctx, ctx, box, tree, cell);
        }
    }
}

/**
 * Collects the instances and the patches whose bounds overlap @p box into the overlap list.
 *
 * Copies the Nash's deletecount to the list's deletecount and clears the instance count (+0x8) and the patch count
 * (+0x10C). For each of the eight octants with a root cell, nash_CellKey_proximitytest 0 (inside) runs
 * OverlapInstancesCallback_operatorCall on the cell, appends each patch of its patch list (+0x24) whose box (+0x158
 * min, +0x164 max) overlaps @p box to the patch array (+0x110), and walks the children whole through nash_Cell_descend2; 2
 * (crossing) hands the cell to nash_privy_QueryHelper_helpX3. The patch maximum (0x40) is stored but never checked. Sets the typed
 * count (+0x210) to -1. Nothing in the code calls it directly.
 *
 * @param box  the query box: min at +0x0, max at +0x10
 */
void cOverlapList::getpatchesandinstances(tBoundBox* box)
{
    OverlapPatchesAndInstancesCallback ctx;
    deletecount = this->nash->deletecount;
    ctx.instancecallback.instances = &instances[0];
    ctx.instancecallback.number = &numinstances;
    ctx.instancecallback.maximum = 0x40;
    ctx.instancecallback.box = box;
    ctx.patchescallback.patches = &patches[0];
    ctx.patchescallback.number = &numpatches;
    ctx.patchescallback.maximum = 0x40;
    ctx.patchescallback.box = box;
    OverlapPatchesCallback* objs = &ctx.patchescallback;
    numinstances = 0;
    *objs->number = 0;
    cBXNash* nash = this->nash;
    OverlapInstancesCallback* pctx = &ctx.instancecallback;
    queryCell_331E90(nash, 0, box, pctx);
    queryCell_331E90(nash, 1, box, pctx);
    queryCell_331E90(nash, 2, box, pctx);
    queryCell_331E90(nash, 3, box, pctx);
    queryCell_331E90(nash, 4, box, pctx);
    queryCell_331E90(nash, 5, box, pctx);
    queryCell_331E90(nash, 6, box, pctx);
    queryCell_331E90(nash, 7, box, pctx);
    numtyped = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_geteverything);
#ifdef SKIP_ASM
struct sTree_340DC0;
struct sCtx_340DC0;
extern "C" void nash_Cell_descend3(nash_Cell* self, OverlapEverythingCallback* ctx);
extern "C" void nash_privy_QueryHelper_helpX4(char* flag, OverlapEverythingCallback* ctx, OverlapEverythingCallback* ctx2, void* vbox, void* vnode, void* vcell);

struct sObj_332DB8 {
    sObj_332DB8* next;
    char pad04[0x154];
    float bmin[3];              // 0x158
    float bmax[3];              // 0x164
};

struct sTree_332DB8 {
    sTree_332DB8* child[8];
    void* nodes;                // 0x20
    sObj_332DB8* objs;          // 0x24
    void** all;                 // 0x28
};



struct sTag_332DB8 {
};

static inline int overlapObj_332DB8(const tBoundBox* b, const sObj_332DB8* n)
{
    int r = 0;
    if (b->MinPoint.x <= n->bmax[0] && b->MaxPoint.x >= n->bmin[0] &&
        b->MinPoint.y <= n->bmax[1] && b->MaxPoint.y >= n->bmin[1] &&
        b->MinPoint.z <= n->bmax[2] && b->MaxPoint.z >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void add_332DB8(OverlapInstancesCallback* s, void* p)
{
    int* cnt = ((int*)s->number);
    int n = *cnt;
    ((tInstanceRecord**)s->instances)[n] = (tInstanceRecord*)p;
    *cnt = n + 1;
}

static inline void collect_332DB8(sTree_332DB8* tree, OverlapEverythingCallback* ctx)
{
    ctx->instancecallback.operatorCall((nash_Cell*)tree);
    OverlapInstancesCallback* s = &(*(OverlapInstancesCallback*)&ctx->patchescallback);
    for (sObj_332DB8* p = tree->objs; p != 0; p = p->next) {
        if (overlapObj_332DB8(((tBoundBox*)s->box), p)) {
            add_332DB8(s, p);
        }
    }
    s = &(*(OverlapInstancesCallback*)&ctx->typedcallback);
    for (void** q = tree->all; q != 0; q = (void**)*q) {
        add_332DB8(s, q);
    }
    if (tree->child[0]) ((nash_Cell*)tree->child[0])->descend3((OverlapEverythingCallback*)ctx);
    if (tree->child[1]) ((nash_Cell*)tree->child[1])->descend3((OverlapEverythingCallback*)ctx);
    if (tree->child[2]) ((nash_Cell*)tree->child[2])->descend3((OverlapEverythingCallback*)ctx);
    if (tree->child[3]) ((nash_Cell*)tree->child[3])->descend3((OverlapEverythingCallback*)ctx);
    if (tree->child[4]) ((nash_Cell*)tree->child[4])->descend3((OverlapEverythingCallback*)ctx);
    if (tree->child[5]) ((nash_Cell*)tree->child[5])->descend3((OverlapEverythingCallback*)ctx);
    if (tree->child[6]) ((nash_Cell*)tree->child[6])->descend3((OverlapEverythingCallback*)ctx);
    if (tree->child[7]) ((nash_Cell*)tree->child[7])->descend3((OverlapEverythingCallback*)ctx);
}

static inline void queryCell_332DB8(cBXNash* nash, int i, tBoundBox* box, OverlapEverythingCallback* ctx)
{
    // octant[2][2][2] walked flat (i = 0-7) through the last index: the [i >> 2][(i >> 1) & 1][i & 1] form of the
    // other queries reorders geteverything's stack slots for the octant addresses
    nash_Octant* cell = &nash->octant[0][0][i];
    if (nash->octant[0][0][i].rootcell != 0) {
        sTree_332DB8* tree = (sTree_332DB8*)cell->rootcell;
        sTag_332DB8 tag;
        char* flag = (char*)&tag;
        int r = nash_CellKey_proximitytest(cell, box);
        if (r == 0) {
            collect_332DB8(tree, ctx);
        } else if (r == 2) {
            nash_privy_QueryHelper_helpX4(flag, (OverlapEverythingCallback*)ctx, (OverlapEverythingCallback*)ctx, box, tree, cell);
        }
    }
}

/**
 * Collects the instances, the patches and the typed nodes of the octree cells that overlap @p box into the overlap
 * list.
 *
 * Copies the Nash's deletecount to the list's deletecount and clears all three counts. Instances go through
 * OverlapInstancesCallback_operatorCall into +0xC (count +0x8), patches whose box (+0x158 min, +0x164 max) overlaps
 * @p box into +0x110 (count +0x10C), and every typed node of a visited cell (cell +0x28) into +0x214 (count +0x210)
 * with no box test and no maximum check. Inside octants (nash_CellKey_proximitytest 0) are walked whole, the children
 * through nash_Cell_descend3; crossing ones (2) go to nash_privy_QueryHelper_helpX4. Called by cRider_loadRider, cRider_putAt,
 * cRider_updateOverlapList and cRider_updateSplit1.
 *
 * @param box  the query box: min at +0x0, max at +0x10
 */
void cOverlapList::geteverything(tBoundBox* box)
{
    OverlapEverythingCallback ctx;
    deletecount = this->nash->deletecount;
    ctx.instancecallback.instances = &instances[0];
    ctx.instancecallback.number = &numinstances;
    ctx.instancecallback.maximum = 0x40;
    ctx.instancecallback.box = box;
    ctx.patchescallback.patches = &patches[0];
    ctx.patchescallback.number = &numpatches;
    ctx.patchescallback.maximum = 0x40;
    ctx.patchescallback.box = box;
    OverlapPatchesCallback* objs = &ctx.patchescallback;
    numinstances = 0;
    *objs->number = 0;
    ctx.typedcallback.typed = &typed[0];
    ctx.typedcallback.number = &numtyped;
    ctx.typedcallback.maximum = 0x200;
    ctx.typedcallback.box = box;
    OverlapTypedCallback* all = &ctx.typedcallback;
    *all->number = 0;
    cBXNash* nash = this->nash;
    OverlapEverythingCallback* pctx = &ctx;
    queryCell_332DB8(nash, 0, box, pctx);
    queryCell_332DB8(nash, 1, box, pctx);
    queryCell_332DB8(nash, 2, box, pctx);
    queryCell_332DB8(nash, 3, box, pctx);
    queryCell_332DB8(nash, 4, box, pctx);
    queryCell_332DB8(nash, 5, box, pctx);
    queryCell_332DB8(nash, 6, box, pctx);
    queryCell_332DB8(nash, 7, box, pctx);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_terrainIntersect);
#ifdef SKIP_ASM

extern t4Vector t4vect_Zero_333EF8[] __asm__("t4vect_Zero");




extern "C" void cAIFwdDiffCache_GetFwdDiff(void* cache, void* src, void** out);
struct sVec4_335960;
extern "C" int cWorldIntersectType_IsPatchInt(void* q, void* obj, void* node, t4Vector* pos, t4Vector* nrm, float* dist,
                                              t4Vector* uv, tIntHint* hint);
extern "C" int intersect_instanceIntersect(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapTri_333EF8(const cWorldIntersectType* q, const tPatchRecord* n)
{
    int r = 0;
    if (q->boxMax.x > n->m_fvMin.m[0] && q->boxMin.x < n->m_fvMax.m[0] &&
        q->boxMax.y > n->m_fvMin.m[1] && q->boxMin.y < n->m_fvMax.m[1] &&
        q->boxMax.z > n->m_fvMin.m[2] && q->boxMin.z < n->m_fvMax.m[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapNode_333EF8(const cWorldIntersectType* q, const tInstanceRecord* n)
{
    int r = 0;
    if (q->boxMax.x > n->m_fvMin.m[0] && q->boxMin.x < n->m_fvMax.m[0] &&
        q->boxMax.y > n->m_fvMin.m[1] && q->boxMin.y < n->m_fvMax.m[1] &&
        q->boxMax.z > n->m_fvMin.m[2] && q->boxMin.z < n->m_fvMax.m[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_333EF8(const cWorldIntersectType* q, const tBoundBox* o)
{
    int r = 0;
    if (q->boxMax.x > o->MinPoint.x && q->boxMin.x < o->MaxPoint.x &&
        q->boxMax.y > o->MinPoint.y && q->boxMin.y < o->MaxPoint.y &&
        q->boxMax.z > o->MinPoint.z && o->MaxPoint.z > q->boxMin.z) {
        r = 1;
    }
    return r;
}

static inline void reset_333EF8(tIntersectInfo* h)
{
    h->HitType = 2;
    h->Patch = 0;
    h->Inst = 0;
    h->Spline = 0;
    h->Mesh = -1;
    h->Face = -1;
    h->Seg = -1;
    h->Stype = -1;
    (*(&h->Ivel)) = *t4vect_Zero_333EF8;
    (*(&h->Iangvel)) = *t4vect_Zero_333EF8;
    (*(int*)&h->T) = 0;
    h->u = 0;
    h->v = 0;
}

/**
 * Intersects a query with the patches and instances already in the overlap list, storing the hits; returns their count.
 *
 * Patches (patches, count numpatches) with both bits of 0x41 set in their flags (m_sSurfaceFlags) and a box (m_fvMin,
 * m_fvMax) overlapping the query's box (boxMax, boxMin) get their forward differences from the Nash's AI cache
 * (nash->TheAIFwdDiffCache) through cAIFwdDiffCache_GetFwdDiff and are tested with cWorldIntersectType_IsPatchInt. Each
 * hit resets its 0x80-byte record (On Tour tIntersectInfo::Init) and stores position, normal, the patch's surface id
 * (m_sSurfaceType), the patch, the parameter, and u and v. Then instances (instances, count numinstances) whose
 * m_pCollideRecord is set: a flag 0x20 one is box-tested against its stored box (m_fvMin, m_fvMax), a flag 0x40 one
 * needs a non-null object (m_pActiveNode) that
 * answers true through virtual slot 44 and whose slot 45 box overlaps; each passes to intersect_instanceIntersect (mode
 * 2) with the room left. Patch hits are not checked against @p cap. Called by cOverlapList_terrainIntersect1 and
 * cGroundMotion_setupGroundVectors.
 *
 * @param query  the query (On Tour cWorldIntersectType)
 * @param hits  out: the hit records
 * @param cap  the number of records @p hits holds
 * @param hitCache  passed to cWorldIntersectType_IsPatchInt as its last argument (On Tour tIntHint *hint)
 * @return the number of hits stored
 */
int cOverlapList::terrainIntersect(cWorldIntersectType* query, tIntersectInfo* hits, int cap, tIntHint* hitCache)
{
    cOverlapList* self = this;
    int n = 0;
    for (unsigned int i = 0; i < self->numpatches; i++) {
        tPatchRecord* obj = self->patches[i];
        if ((obj->m_sSurfaceFlags & tPatchRecord::kCollideFlags) != tPatchRecord::kCollideFlags) {
            continue;
        }
        if (!overlapTri_333EF8(query, obj)) {
            continue;
        }
        t4Vector uv;
        t4Vector pos;
        t4Vector nrm;
        void* node;
        float dist;
        cAIFwdDiffCache_GetFwdDiff(self->nash->TheAIFwdDiffCache, obj, &node);
        if (!cWorldIntersectType_IsPatchInt(query, obj, node, (t4Vector*)&pos, (t4Vector*)&nrm, &dist, (t4Vector*)&uv, hitCache)) {
            continue;
        }
        reset_333EF8(&hits[n]);
        (*(&hits[n].Ipos)) = pos;
        (*(&hits[n].Inorm)) = nrm;
        hits[n].Stype = obj->m_sSurfaceType;
        hits[n].Patch = obj;
        hits[n].Iparam = dist;
        hits[n].u = uv.x;
        hits[n].v = uv.y;
        n++;
    }
    for (unsigned int j = 0; j < self->numinstances; j++) {
        tInstanceRecord* node = self->instances[j];
        if (node->m_pCollideRecord == 0) {
            continue;
        }
        int flags = node->m_iFlags;
        if (flags & 0x20) {
            if (overlapNode_333EF8(query, node)) {
                n += intersect_instanceIntersect(query, node, &hits[n], cap - n, 2);
            }
        } else if (flags & 0x40) {
            if (node->m_pActiveNode == 0) {
                continue;
            }
            if (node->m_pActiveNode->hasBoundBox()) {
                tBoundBox* objBox = (tBoundBox*)node->m_pActiveNode->getBoundBox();
                if (overlapBox_333EF8(query, objBox)) {
                    n += intersect_instanceIntersect(query, node, &hits[n], cap - n, 2);
                }
            }
        }
    }
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_terrainIntersect1);
#ifdef SKIP_ASM
// A tIntersectInfo as a stack-array element. EA's t4Vector has a (empty) default constructor, so retail runs an empty
// constructor loop over a tIntersectInfo array; ours has none, and this wrapper gives the array that constructor back.
struct tIntersectInfoElem : tIntersectInfo {
    tIntersectInfoElem() {}
};

extern "C" int cOverlapList_terrainIntersect(cOverlapList* self, void* q, tIntersectInfo* hits, int max,
        tIntHint* hint);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_003342D0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int isBetter_003342D0(tIntersectInfo* a, tIntersectInfo* b)
{
    if (a->Inst != 0) {
        if (b->Inst == 0) {
            return 1;
        }
        return a->Inst->m_RID < b->Inst->m_RID;
    }
    if (b->Inst != 0) {
        return 0;
    }
    return a->Patch->m_RID < b->Patch->m_RID;
}

/**
 * Returns the parameter of the overlap-list terrain hit nearest the query's ideal parameter, copying the hit to
 * @p out.
 *
 * Collects up to 64 hits with cOverlapList_terrainIntersect into a stack array and returns -1.0 when there are none. Otherwise picks
 * the hit whose parameter (Iparam) is closest to the query's virtual slot 3 (On Tour GetIdealParam). On a tie an
 * instance hit (Inst) beats a patch hit (Patch), two instances compare their m_RID and two patches theirs, the
 * smaller winning (On Tour orderIntersectInfo). Called by the rider motion code (cAirMotion_intersectBoard,
 * cGroundMotion_checkWallCollisions, cWipeoutMotion_intersectGround and others).
 *
 * @param query  the query (On Tour cWorldIntersectType)
 * @param out  out: the chosen hit
 * @param hitCache  passed through to cOverlapList_terrainIntersect (On Tour tIntHint *hint)
 * @return the chosen hit's parameter, or -1.0
 */
float cOverlapList::terrainIntersect1(cWorldIntersectType* query, tIntersectInfo* out, tIntHint* hitCache)
{
    tIntersectInfoElem hits[64];
    int n = terrainIntersect(query, hits, 0x40, hitCache);
    if (n == 0) {
        return -1.0f;
    }
    int best = 0;
    float ref = query->GetIdealParam();
    float bestDiff = absf_003342D0(hits[0].Iparam - ref);
    for (int i = 1; i < n; i++) {
        float diff = absf_003342D0(hits[i].Iparam - ref);
        if (diff < bestDiff) {
            bestDiff = diff;
            best = i;
        } else if (diff == bestDiff) {
            if (isBetter_003342D0(&hits[i], &hits[best])) {
                best = i;
            }
        }
    }
    *out = hits[best];
    return out->Iparam;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_staticObjectIntersect);
#ifdef SKIP_ASM
extern "C" int intersect_instanceIntersect(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapNode_334458(const cWorldIntersectType* q, const tInstanceRecord* n)
{
    int r = 0;
    if (q->boxMax.x > n->m_fvMin.m[0] && q->boxMin.x < n->m_fvMax.m[0] &&
        q->boxMax.y > n->m_fvMin.m[1] && q->boxMin.y < n->m_fvMax.m[1] &&
        q->boxMax.z > n->m_fvMin.m[2] && q->boxMin.z < n->m_fvMax.m[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_334458(const cWorldIntersectType* q, const tBoundBox* o)
{
    int r = 0;
    if (q->boxMax.x > o->MinPoint.x && q->boxMin.x < o->MaxPoint.x &&
        q->boxMax.y > o->MinPoint.y && q->boxMin.y < o->MaxPoint.y &&
        q->boxMax.z > o->MinPoint.z && o->MaxPoint.z > q->boxMin.z) {
        r = 1;
    }
    return r;
}

/**
 * Intersects a query with the instances in the overlap list, storing the hits; returns their count.
 *
 * Walks the instance array (instances, count numinstances): a flag 0x20 instance is box-tested against its stored
 * box (m_fvMin, m_fvMax), a flag 0x40 one asks its object (m_pActiveNode) through virtual slots 44 (hasBoundBox) and
 * 45 (getBoundBox). Each one that overlaps the query's box (boxMax, boxMin) goes to intersect_instanceIntersect with
 * the room left, in mode 2 when @p full is set and 1 otherwise. Unlike cOverlapList_terrainIntersect it neither skips
 * instances without a m_pCollideRecord nor checks the object for null. Called by cRider_objectIntersect.
 *
 * @param query  the query (On Tour cWorldIntersectType)
 * @param buf  out: the hit records, 0x80 bytes each
 * @param cap  the number of records @p buf holds
 * @param full  nonzero selects mode 2 instead of 1 (On Tour includeridable)
 * @return the number of hits stored
 */
int cOverlapList::staticObjectIntersect(cWorldIntersectType* query, tIntersectInfo* buf, int cap, int full)
{
    int mode = 1;
    if (full) mode = 2;
    int n = 0;
    for (unsigned int i = 0; i < numinstances; i++) {
        tInstanceRecord* node = instances[i];
        int flags = node->m_iFlags;
        if (flags & 0x20) {
            if (overlapNode_334458(query, node)) {
                n += intersect_instanceIntersect(query, node, &buf[n], cap - n, mode);
            }
        } else if (flags & 0x40) {
            if (node->m_pActiveNode->hasBoundBox()) {
                tBoundBox* objBox = (tBoundBox*)node->m_pActiveNode->getBoundBox();
                if (overlapBox_334458(query, objBox)) {
                    n += intersect_instanceIntersect(query, node, &buf[n], cap - n, mode);
                }
            }
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_splineIntersect);
#ifdef SKIP_ASM
extern "C" void cRailModifier_splineIntersect(void* a0, tBoundBox* box, tBoundBox* box2, t4Vector* pos, int mask,
        int* cnt,
                                              float* dist, tIntersectInfo* intInfo);
extern "C" void cFlexRailNode_splineIntersect(void* a0, tBoundBox* box, tBoundBox* box2, t4Vector* pos, int mask,
        int* cnt,
                                              float* dist, tIntersectInfo* intInfo);
extern "C" void intersect_staticSplineSegmentIntersect(void* obj, tBoundBox* box, t4Vector* pos, int* cnt, float* dist,
                                                       tIntersectInfo* intInfo);

// PORT: PS2-only VU0 inline asm (in-place vector subtract).
static inline void wsSubEq_334680(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (in-place vector add).
static inline void wsAddEq_334680(t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

/**
 * Finds the rails (splines) near a point among the objects in this overlap list.
 *
 * Builds a box of half-size @p testRadius around @p pos and, for each listed object (typed, count numtyped), tests
 * rails with cRailModifier_splineIntersect, flex rails with cFlexRailNode_splineIntersect and instances whose filter flags match
 * @p mask (the segment's m_Parent->m_iFlags) with intersect_staticSplineSegmentIntersect, collecting hits into @p
 * intInfo.
 *
 * @param pos   the point to test
 * @param intInfo    out: the hit list (On Tour intInfo)
 * @param mask  which spline types to accept
 * @param testRadius     the test radius
 * @return the number of hits
 */
int cOverlapList::splineIntersect(t4Vector* pos, tIntersectInfo* intInfo, int mask, float testRadius)
{
    tBoundBox box;
    t4Vector ext;
    int cnt;
    float dist;
    float zero = 0.0f;
    cnt = 0;
    dist = zero;
    box.MinPoint = *pos;
    box.MaxPoint = *pos;
    ext.x = testRadius;
    ext.y = testRadius;
    ext.z = testRadius;
    ext.w = zero;
    wsSubEq_334680(box.MinPoint, ext);
    wsAddEq_334680(box.MaxPoint, ext);
    for (unsigned int i = 0; i < numtyped; i++) {
        cTypedSpaceNode* spaceNode = (cTypedSpaceNode*)typed[i];
        int type = spaceNode->type;
        if (type == kSNT_Rail) {
            cRailModifier_splineIntersect(((cRailSpaceNode*)spaceNode)->Rail, &box, &box, pos, mask, &cnt, &dist,
                                          intInfo);
        } else if (type == kSNT_FlexRail) {
            cFlexRailNode_splineIntersect(((cFlexRailSpaceNode*)spaceNode)->FlexNode, &box, &box, pos, mask, &cnt,
                                          &dist, intInfo);
        } else if (type == kSNT_SplineSegment) {
            if (((tSplineSegmentRecord*)spaceNode)->m_Parent->m_iFlags & mask) {
                intersect_staticSplineSegmentIntersect(spaceNode, &box, pos, &cnt, &dist, intInfo);
            }
        }
    }
    return cnt;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_initTheAIFwdDiffCache);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cAIFwdDiffCache_cAIFwdDiffCache(void* self);
extern "C" void cAIFwdDiffCache_Init(void* cache, int n);
extern char str_AIFwdDiffCache[];

/**
 * Creates the nash's AI forward-difference cache: a 40-entry cAIFwdDiffCache stored at +0xA4.
 *
 * Allocates 0x10 bytes tagged "AIFwdDiffCache", runs the constructor, stores the cache at +0xA4 and calls
 * cAIFwdDiffCache_Init(cache, 40), which allocates the entries and their hash buckets.
 */
void cBXNash::initTheAIFwdDiffCache()
{
    void* cache = cAIFwdDiffCache_cAIFwdDiffCache(cMemMan_alloc(0x10, str_AIFwdDiffCache, 0, 0));
    TheAIFwdDiffCache = (cAIFwdDiffCache*)cache;
    cAIFwdDiffCache_Init(cache, 40);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_purgeTheAIFwdDiffCache);
#ifdef SKIP_ASM
extern "C" void _$_15cAIFwdDiffCache(void* p, int flags);

/**
 * Deletes the AI forward-difference cache (+0xA4) built by cBXNash_initTheAIFwdDiffCache, then clears the pointer.
 */
void cBXNash::purgeTheAIFwdDiffCache()
{
    void* fwdDiffCache = TheAIFwdDiffCache;
    if (fwdDiffCache != 0) {
        _$_15cAIFwdDiffCache(fwdDiffCache, 3);
    }
    TheAIFwdDiffCache = 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", intersect_instanceIntersect);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

// ---- Shared declarations: one type per real thing, used by the functions below ----

// g++ 2.95 vtable entry with an untyped function slot (cast at the call).
struct sWsVtSlot { short delta; short index; void* fn; };


// Constant vector table (0x004FF120; 00341388 also reads it through its ctor vector type).
extern t4Vector t4vect_Zero_4888[] __asm__("t4vect_Zero");

struct t4Vector;

// 4x4 matrix with an empty ctor and a VU0 copy (through vf1..vf4).
struct sWsMat_4888 {
    t4Vector row[4];
    sWsMat_4888() {}
    // PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
    sWsMat_4888(const sWsMat_4888& s)
    {
        __asm__ __volatile__(
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            :
            : "r"(this), "r"(&s)
            : "memory");
    }
};



// Per-bone part record of an instance definition (12 bytes).
struct sWsPart_4888 {
    char pad00[0xA];
    short id;           // 0xA
};

struct sWsInstDef_4888 {
    int type;                   // 0x0
    int pad04[3];
    sWsPart_4888 parts[1];      // 0x10
};


// A triangle hit record (0x80 bytes; empty ctor).
struct sWsHit_4888 {
    t4Vector pos;       // 0x00
    t4Vector nrm;       // 0x10
    t4Vector v20;       // 0x20
    t4Vector v30;       // 0x30
    float dist;         // 0x40
    int type;           // 0x44
    void* tri;          // 0x48
    int id;             // 0x4C
    void* inst;         // 0x50
    void* obj;          // 0x54
    int f58;            // 0x58
    int bone;           // 0x5C
    int pad60[8];
    sWsHit_4888() {}
} __attribute__((aligned(16)));


typedef int (*Test_4888)(void*);
typedef int (*Query_4888)(void*, void*, void*, int, int);
typedef sWsMat_4888* (*GetMat_4888)(void*, void*);
typedef void (*SetXf_4888)(void*, float, sWsMat_4888*);
typedef void (*Reset_4888)(void*);
typedef int (*Tris_4888)(void*, void*, void*, sWsHit_4888*, int);
typedef int (*Hit_4888)(void*, void*, void*);
typedef int (*Mesh_4888)(void*, void*, sWsHit_4888*, int);

void* cWorld_getSphereTreeInfo_4888(void* self, void* a1) __asm__("cWorld_getSphereTreeInfo__FPvT0");
void* cWorld_getPolyModelInfo_4888(void* self, void* a1) __asm__("cWorld_getPolyModelInfo__FPvT0");
extern "C" void* tSphTreeList_getMeshTree(void* self, int a1);
void* tPolyModelList_getMeshPolyModel_4888(void* self, int a1) __asm__("tPolyModelList_getMeshPolyModel__FPvi");
// PORT: callers pass the hit capacity as a fifth argument the function never reads.
extern "C" int cWorldIntersectType_IsMeshInt_cap(void* self, void* mesh, int flag, sWsHit_4888* out, int cap) __asm__("cWorldIntersectType_IsMeshInt");

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMat_4888(sWsMat_4888* d, const sWsMat_4888* a, const sWsMat_4888* b)
{
    __asm__ __volatile__(
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        :
        : "r"(d), "r"(a), "r"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy through VU0 registers).
static inline void vu0CopyMat_4888(sWsMat_4888* d, const sWsMat_4888* s)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        :
        : "r"(d), "r"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector * matrix, row-vector convention).
static inline t4Vector vu0MtxMul_4888(const sWsMat_4888* m, const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2         $vf8, %1\n"
        "lqc2         $vf4, 0x0(%2)\n"
        "lqc2         $vf5, 0x10(%2)\n"
        "lqc2         $vf6, 0x20(%2)\n"
        "lqc2         $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, %0\n"
        : "=m"(r)
        : "m"(v), "r"(m)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (d = v * s).
static inline void vu0ScaleVector_4888(t4Vector& d, const t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(d), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
}

// d = v * s
static inline void ScaleTo_4888(t4Vector& d, const t4Vector& v, float s)
{
    vu0ScaleVector_4888(d, v, s);
}

// v * s
static inline t4Vector Scale_4888(const t4Vector& v, float s)
{
    t4Vector r;
    ScaleTo_4888(r, v, s);
    return r;
}

// A vector built from four floats.
struct sVecK_4888 : public t4Vector {
    sVecK_4888(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

// Component-wise product.
static inline sVecK_4888 MulVec_4888(const t4Vector& a, const t4Vector& b)
{
    return sVecK_4888(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

static inline t4Vector Row_4888(const sWsMat_4888& m, int i)
{
    return m.row[i];
}

// In-place inverse of a rigid transform: transpose the rotation, rotate the negated translation back.
static inline void InvertOrtho_4888(sWsMat_4888& m)
{
    float f;
    f = m.row[0].y;
    m.row[0].y = m.row[1].x;
    m.row[1].x = f;
    f = m.row[0].z;
    m.row[0].z = m.row[2].x;
    m.row[2].x = f;
    f = m.row[1].z;
    m.row[1].z = m.row[2].y;
    m.row[2].y = f;
    t4Vector v = Scale_4888(Row_4888(m, 3), -1.0f);
    v.w = 0.0f;
    v = vu0MtxMul_4888(&m, v);
    v.w = 1.0f;
    m.row[3] = v;
}

// Part record i of an instance definition.
static inline sWsPart_4888* GetPart_4888(sWsInstDef_4888* def, int i)
{
    return &def->parts[i];
}

// intersect_instanceIntersect's mode (names ours; On Tour's parameter is int flags): [U] kInstIntersect_PlainParts
// tests only the meshes without a surface type (part id eST_Undefined), kInstIntersect_SurfaceParts only those with
// one, any other mode every mesh. cOverlapList_terrainIntersect passes kInstIntersect_SurfaceParts,
// staticObjectIntersect one or the other.
enum { kInstIntersect_PlainParts = 1, kInstIntersect_SurfaceParts = 2 };

// Collects the triangle hits of instance `node` against query q into buf (mode 1: unnamed parts only, mode 2: named
// parts only); bone matrices come from the instance's animated object or are built here from the skeleton.
extern "C" int func_4888(cWorldIntersectType* q, tInstanceRecord* node, sWsHit_4888* buf, int cap, int mode) __asm__("intersect_instanceIntersect");
/**
 * Intersects a world query with one instance, writing up to @p cap hits.
 *
 * If the instance has a node object whose virtual slot 38 says it handles intersection, its slot 39 does the
 * work. Otherwise, for model types 1-3, transforms the query into the instance's space and tests its sphere tree
 * or polygon meshes (cWorldIntersectType_IsMeshInt), filling the hits with this instance's data.
 *
 * @param intersectType     the world intersection query (On Tour cWorldIntersectType)
 * @param node  the instance record
 * @param buf   out: the hit array
 * @param cap   room left in @p buf
 * @param mode  intersection flags
 * @return the number of hits written
 */
extern "C" int func_4888(cWorldIntersectType* intersectType, tInstanceRecord* node, sWsHit_4888* buf, int cap, int mode)
{
    if (node->m_pActiveNode != 0) {
        sWsVtSlot* vt = ((sWsVtSlot*)VPTR_OF(cObjNode, node->m_pActiveNode));
        if (((Test_4888)vt[38].fn)(vthis(node->m_pActiveNode, vt[38]))) {
            sWsVtSlot* vt2 = ((sWsVtSlot*)VPTR_OF(cObjNode, node->m_pActiveNode));
            return ((Query_4888)vt2[39].fn)(vthis(node->m_pActiveNode, vt2[39]), intersectType, buf, cap, mode);
        }
    }
    int type = ((sWsInstDef_4888*)node->m_pCollideRecord)->type;
    if (type != CR_SphereTree && type != CR_Polygon && type != CR_Box)
        return 0;
    cInstanceNode* obj = node->m_pActiveNode;
    sWsMat_4888 mats[24];
    sWsMat_4888* pMat = &(*(sWsMat_4888*)&node->m_fmTransform);
    int useLocal = 1;
    sWsMat_4888* boneMats = mats;
    if (obj != 0) {
        sWsVtSlot* vt = ((sWsVtSlot*)VPTR_OF(cObjNode, obj));
        if (((Test_4888)vt[26].fn)(vthis(obj, vt[26]))) {
            useLocal = 0;
            sWsVtSlot* vt2 = ((sWsVtSlot*)VPTR_OF(cObjNode, obj));
            boneMats = ((GetMat_4888)vt2[28].fn)(vthis(obj, vt2[28]), node);
        } else {
            sWsVtSlot* vt2 = ((sWsVtSlot*)VPTR_OF(cObjNode, obj));
            pMat = ((GetMat_4888)vt2[25].fn)(vthis(obj, vt2[25]), node);
        }
    }
    tModelRecord* tbl = node->m_Model;
    tMeshHeirarchyRecord* bone = tbl->m_MeshHeirarchy;
    float instScale = node->m_fScale;
    t4Vector scl;
    scl.x = instScale;
    scl.y = instScale;
    scl.z = instScale;
    scl.w = 1.0f;
    int n = 0;
    sWsMat_4888* boneMat = boneMats;
    for (int i = 0, k = 0; i < tbl->m_iNumMeshes; i++, bone++, boneMat++) {
        if (useLocal) {
            int parent = bone->m_iParentID;
            if (parent != tMeshHeirarchyRecord::kNoParent) {
                sWsMat_4888 productCopy;
                sWsMat_4888 scaledLocal;
                t4Vector res;
                t4Vector translation;
                sWsMat_4888* lm = ((sWsMat_4888*)bone->m_pMeshMatrix);
                res = lm->row[0];
                scaledLocal.row[0] = res;
                res = lm->row[1];
                scaledLocal.row[1] = res;
                res = lm->row[2];
                scaledLocal.row[2] = res;
                translation = lm->row[3];
                res.x = translation.x * scl.x;
                res.y = translation.y * scl.y;
                res.z = translation.z * scl.z;
                res.w = translation.w * scl.w;
                scaledLocal.row[3] = res;
                sWsMat_4888 product;
                sWsMat_4888* pt2 = &productCopy;
                sWsMat_4888* pt = &product;
                vu0MulMat_4888(pt, &mats[parent], &scaledLocal);
                vu0CopyMat_4888(pt2, pt);
                vu0CopyMat_4888(&mats[i], pt2);
            } else {
                sWsMat_4888 productCopy;
                sWsMat_4888 scaledLocal;
                t4Vector res;
                t4Vector translation;
                sWsMat_4888* lm = ((sWsMat_4888*)bone->m_pMeshMatrix);
                res = lm->row[0];
                scaledLocal.row[0] = res;
                res = lm->row[1];
                scaledLocal.row[1] = res;
                res = lm->row[2];
                scaledLocal.row[2] = res;
                translation = lm->row[3];
                res.x = translation.x * scl.x;
                res.y = translation.y * scl.y;
                res.z = translation.z * scl.z;
                res.w = translation.w * scl.w;
                scaledLocal.row[3] = res;
                sWsMat_4888 product;
                sWsMat_4888* pt2 = &productCopy;
                sWsMat_4888* pt = &product;
                vu0MulMat_4888(pt, pMat, &scaledLocal);
                vu0CopyMat_4888(pt2, pt);
                vu0CopyMat_4888(&mats[i], pt2);
            }
        }
        if (bone->m_Mesh == 0)
            continue;
        if (mode == kInstIntersect_SurfaceParts) {
            if (GetPart_4888(((sWsInstDef_4888*)node->m_pCollideRecord), i)->id == eST_Undefined) {
                if (type != CR_Box)
                    k++;
                continue;
            }
        } else if (mode == kInstIntersect_PlainParts) {
            if (GetPart_4888(((sWsInstDef_4888*)node->m_pCollideRecord), i)->id != eST_Undefined) {
                if (type != CR_Box)
                    k++;
                continue;
            }
        }
        sWsHit_4888 ii[64];
        sWsMat_4888 inv = *boneMat;
        InvertOrtho_4888(inv);
        int inum = 0;
        void* tri = 0;
        ((SetXf_4888)((sWsVtSlot*)VPTR_AT(intersectType, 80))[1].fn)(vthis(intersectType, ((sWsVtSlot*)VPTR_AT(intersectType, 80))[1]), 1.0f / node->m_fScale, &inv);
        if (type == CR_Box) {
            inum = ((Tris_4888)((sWsVtSlot*)VPTR_AT(intersectType, 80))[7].fn)(vthis(intersectType, ((sWsVtSlot*)VPTR_AT(intersectType, 80))[7]), ((char*)bone->m_Mesh), (char*)&bone->m_Mesh->m_fvMax, ii, 64);
        } else {
            if (((Hit_4888)((sWsVtSlot*)VPTR_AT(intersectType, 80))[9].fn)(vthis(intersectType, ((sWsVtSlot*)VPTR_AT(intersectType, 80))[9]), ((char*)bone->m_Mesh), (char*)&bone->m_Mesh->m_fvMax)) {
                if (type == CR_SphereTree) {
                    tri = tSphTreeList_getMeshTree(cWorld_getPolyModelInfo_4888(cWorld_m_pTheWorld, node), k);
                    inum = ((Mesh_4888)((sWsVtSlot*)VPTR_AT(intersectType, 80))[6].fn)(vthis(intersectType, ((sWsVtSlot*)VPTR_AT(intersectType, 80))[6]), tri, ii, 64);
                } else {
                    inum = cWorldIntersectType_IsMeshInt_cap(intersectType, tPolyModelList_getMeshPolyModel_4888(cWorld_getSphereTreeInfo_4888(cWorld_m_pTheWorld, node), k),
                                             bone->m_Mesh->m_iFlags & 1, ii, 64);
                }
            }
            k++;
        }
        ((Reset_4888)((sWsVtSlot*)VPTR_AT(intersectType, 80))[2].fn)(vthis(intersectType, ((sWsVtSlot*)VPTR_AT(intersectType, 80))[2]));
        for (int j = 0; j < inum; j++) {
            ii[j].nrm = vu0MtxMul_4888(boneMat, ii[j].nrm);
            buf[n] = ii[j];
            buf[n].pos = vu0MtxMul_4888(boneMat, MulVec_4888(buf[n].pos, scl));
            buf[n].type = 1;
            buf[n].inst = node;
            buf[n].bone = i;
            buf[n].id = -1;
            buf[n].tri = tri;
            buf[n].v20 = *t4vect_Zero_4888;
            buf[n].v30 = *t4vect_Zero_4888;
            buf[n].obj = 0;
            if (GetPart_4888(((sWsInstDef_4888*)node->m_pCollideRecord), i)->id != eST_Undefined)
                buf[n].id = GetPart_4888(((sWsInstDef_4888*)node->m_pCollideRecord), i)->id;
            n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", intersect_staticSplineSegmentIntersect);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm throughout; needs C fallbacks off-PS2.
struct sV_335128
{
    float x, y, z, w;
    sV_335128() {}
    // t4vect::x()/y()/z()/w() in On Tour (component accessors returning a reference).
    float& X() { return x; }
    float& Y() { return y; }
    float& Z() { return z; }
    float& W() { return w; }
} __attribute__((aligned(16)));

extern sV_335128 t4vect_Zero_335128 __asm__("t4vect_Zero");

struct sF5_335128 {
    float v[5];
};
// Segment brackets for the golden-section refinement, by nearest sample.
extern const sF5_335128 D_0048E560_335128 __asm__("D_0048E560");
extern const sF5_335128 D_0048E578_335128 __asm__("D_0048E578");



// PORT: PS2-only VU0 macro-mode asm (4x4 matrix times vector).
static inline sV_335128 MatMulV_335128(const sV_335128* m, const sV_335128& v)
{
    sV_335128 r;
    __asm__(
        "lqc2       $vf8, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "lqc2       $vf5, 0x10(%1)\n"
        "lqc2       $vf6, 0x20(%1)\n"
        "lqc2       $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2       $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV_335128 Sub_335128(const sV_335128& a, const sV_335128& b)
{
    sV_335128 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (a + b).
static inline sV_335128 Add_335128(const sV_335128& a, const sV_335128& b)
{
    sV_335128 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sV_335128 Scale_335128(const sV_335128& v, float s)
{
    sV_335128 r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector length).
static inline float Length_335128(const sV_335128& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_335128(const sV_335128& a, const sV_335128& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline sV_335128 Normalize_335128(const sV_335128& v)
{
    sV_335128 r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2 abs.s asm helper; use fabsf on PC.
static inline float Abs_335128(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `<?` (min) operator, removed in GCC 4.3.
static inline float Clamp_335128(float x, float hi)
{
    return x >= 0.0f ? (x <? hi) : 0.0f;
}

// On Tour: splineEval(const t4Matrix& emat, float w): the point at w, (w^3, w^2, w, 1) through the coefficient rows.
// The components are set through the reference accessors, w first, as On Tour does.
static inline sV_335128 splineEval_335128(const sV_335128* emat, float w)
{
    sV_335128 v;
    v.W() = 1.0f;
    v.Z() = w;
    v.Y() = w * v.Z();
    v.X() = w * v.Y();
    return MatMulV_335128(emat, v);
}

// On Tour: splineSlope(const t4Matrix& emat, float w): the tangent at w, (3w^2, 2w, 1, 0).
static inline sV_335128 splineSlope_335128(const sV_335128* emat, float w)
{
    sV_335128 v;
    v.W() = 0.0f;
    v.Z() = 1.0f;
    v.Y() = w * 2.0f;
    v.X() = w * (w * 3.0f);
    return MatMulV_335128(emat, v);
}

static inline void reset_335128(tIntersectInfo* h)
{
    h->HitType = 2;
    h->Patch = 0;
    h->Inst = 0;
    h->Spline = 0;
    h->Mesh = -1;
    h->Face = -1;
    h->Seg = -1;
    h->Stype = -1;
    (*(sV_335128*)&h->Ivel) = t4vect_Zero_335128;
    (*(sV_335128*)&h->Iangvel) = t4vect_Zero_335128;
    h->T = 0;
    h->u = 0;
    h->v = 0;
}

// intersect::staticSplineSegmentIntersect (On Tour name): closest point on a spline segment to intersectpos,
// a coarse 4-segment scan, then a golden-section search.
// The unit declares intersect_staticSplineSegmentIntersect with an int last parameter; bind the typed definition by asm label.
extern "C" void intersect_staticSplineSegmentIntersect_typed(tSplineSegmentRecord* pSplineSeg, tBoundBox* box, sV_335128* intersectpos, bool* hit, float* intersectdist, tIntersectInfo* intersectinfo) __asm__("intersect_staticSplineSegmentIntersect");
/**
 * Finds the point of a spline segment nearest @p intersectpos and records it when it beats the best hit so far.
 *
 * Returns at once when the segment's box (+0x6C min, +0x78 max) misses @p box. Otherwise samples the cubic (coefficient
 * rows at +0x10: point = rows times (w^3, w^2, w, 1)) at w = 0, 0.25 .. 1, takes the chord nearest @p intersectpos, and
 * runs a golden-section search (ratio 0.618034, at most 24 steps, until the bracket is under 0.0005) inside the bracket
 * D_0048E560/D_0048E578 give for that chord. When no hit is recorded yet or the result is nearer than
 * @p intersectdist, sets @p hit and @p intersectdist and resets @p intersectinfo, storing the point, the normalised
 * tangent there as the normal, the owner's (+0x68) +0x28 word as the surface id, the owner as the spline record and the
 * parameter as T. Called by cOverlapList_splineIntersect, cBXNash_splineIntersect and the spline octree walkers (nash_privy_QueryHelper_helpX7,
 * nash_Cell_descend6).
 *
 * @param pSplineSeg  the spline segment
 * @param box  the query box
 * @param intersectpos  the point to test
 * @param hit  in/out: whether a hit is recorded
 * @param intersectdist  in/out: the distance of the recorded hit
 * @param intersectinfo  out: the hit record (0x80 bytes)
 */
extern "C" void intersect_staticSplineSegmentIntersect_typed(tSplineSegmentRecord* pSplineSeg, tBoundBox* box, sV_335128* intersectpos, bool* hit, float* intersectdist, tIntersectInfo* intersectinfo)
{
    if (box->MaxPoint.x < pSplineSeg->m_fvMin.m[0] || box->MinPoint.x > pSplineSeg->m_fvMax.m[0] ||
        box->MaxPoint.y < pSplineSeg->m_fvMin.m[1] || box->MinPoint.y > pSplineSeg->m_fvMax.m[1] ||
        box->MaxPoint.z < pSplineSeg->m_fvMin.m[2] || box->MinPoint.z > pSplineSeg->m_fvMax.m[2]) {
        return;
    }

    int i;
    sV_335128 lastp;
    const float kTopSplit = 0.6180340051651001f;
    const float kBottomSplit = 1.0f - kTopSplit;
    float dist0, dist1, dist2, dist3;
    float x0 = 0.0f;
    float x3 = 1.0f;
    sV_335128 nextp;
    sV_335128 tmp;
    lastp = splineEval_335128(((sV_335128*)&pSplineSeg->m_fmBasisMat), x0);
    float cd = Length_335128(Sub_335128(*intersectpos, lastp));
    int ci = 0;
    for (i = 1; i < 5; i++) {
        nextp = splineEval_335128(((sV_335128*)&pSplineSeg->m_fmBasisMat), i * 0.25f);
        tmp = Sub_335128(nextp, lastp);
        float chordFraction = Dot_335128(Sub_335128(*intersectpos, lastp), tmp) / Dot_335128(tmp, tmp);
        chordFraction = Clamp_335128(chordFraction, 1.0f);
        tmp = Add_335128(lastp, Scale_335128(tmp, chordFraction));
        float chordDist = Length_335128(Sub_335128(*intersectpos, tmp));
        if (chordDist < cd) {
            ci = i;
            cd = chordDist;
        }
        lastp = nextp;
    }
    const sF5_335128 minx = D_0048E560_335128;
    const sF5_335128 maxx = D_0048E578_335128;
    x0 = minx.v[ci];
    x3 = maxx.v[ci];
    float x1 = x0 + (x3 - x0) * kBottomSplit;
    float x2 = x0 + (x3 - x0) * kTopSplit;
    lastp = splineEval_335128(((sV_335128*)&pSplineSeg->m_fmBasisMat), x0);
    dist0 = Length_335128(Sub_335128(lastp, *intersectpos));
    lastp = splineEval_335128(((sV_335128*)&pSplineSeg->m_fmBasisMat), x1);
    dist1 = Length_335128(Sub_335128(lastp, *intersectpos));
    lastp = splineEval_335128(((sV_335128*)&pSplineSeg->m_fmBasisMat), x2);
    dist2 = Length_335128(Sub_335128(lastp, *intersectpos));
    lastp = splineEval_335128(((sV_335128*)&pSplineSeg->m_fmBasisMat), x3);
    dist3 = Length_335128(Sub_335128(lastp, *intersectpos));
    float lastd = dist2;
    float lastt = x2;
    for (i = 0; i < 24 && !(Abs_335128(x3 - x0) <= 0.0005000000237487257f); i++) {
        if (dist2 < dist1) {
            x0 = x1;
            x1 = x2;
            x2 = x1 * kTopSplit + x3 * kBottomSplit;
            lastp = splineEval_335128(((sV_335128*)&pSplineSeg->m_fmBasisMat), x2);
            float tmp = Length_335128(Sub_335128(lastp, *intersectpos));
            dist0 = dist1;
            dist1 = dist2;
            dist2 = tmp;
            lastt = x2;
            lastd = tmp;
        } else {
            x3 = x2;
            x2 = x1;
            x1 = x2 * kTopSplit + x0 * kBottomSplit;
            lastp = splineEval_335128(((sV_335128*)&pSplineSeg->m_fmBasisMat), x1);
            float tmp = Length_335128(Sub_335128(lastp, *intersectpos));
            dist3 = dist2;
            dist2 = dist1;
            dist1 = tmp;
            lastt = x1;
            lastd = tmp;
        }
    }
    if (!*hit || lastd < *intersectdist) {
        *hit = true;
        *intersectdist = lastd;
        reset_335128(intersectinfo);
        (*(sV_335128*)&intersectinfo->Ipos) = lastp;
        (*(sV_335128*)&intersectinfo->Inorm) = Normalize_335128(splineSlope_335128(((sV_335128*)&pSplineSeg->m_fmBasisMat), lastt));
        intersectinfo->Stype = pSplineSeg->m_Parent->m_eSurfaceType;
        intersectinfo->Inst = 0;
        intersectinfo->Mesh = -1;
        (*(void**)&intersectinfo->Spline) = ((char*)pSplineSeg->m_Parent);
        intersectinfo->T = lastt;
        intersectinfo->Seg = -1;
        intersectinfo->Patch = 0;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", TerrainCallback_intersectpatch);
#ifdef SKIP_ASM

extern t4Vector t4vect_Zero[];


extern "C" void cAIFwdDiffCache_GetFwdDiff(void* cache, void* src, void** out);
extern "C" int cWorldIntersectType_IsPatchInt(void* q, void* obj, void* node, t4Vector* pos, t4Vector* nrm, float* dist,
                                              t4Vector* uv, tIntHint* hint);

static inline int overlap_335960(const cWorldIntersectType* q, const tPatchRecord* n)
{
    int r = 0;
    if (q->boxMax.x > n->m_fvMin.m[0] && q->boxMin.x < n->m_fvMax.m[0] &&
        q->boxMax.y > n->m_fvMin.m[1] && q->boxMin.y < n->m_fvMax.m[1] &&
        q->boxMax.z > n->m_fvMin.m[2] && q->boxMin.z < n->m_fvMax.m[2]) {
        r = 1;
    }
    return r;
}

static inline void reset_335960(tIntersectInfo* h)
{
    h->HitType = 2;
    h->Patch = 0;
    h->Inst = 0;
    h->Spline = 0;
    h->Mesh = -1;
    h->Face = -1;
    h->Seg = -1;
    h->Stype = -1;
    h->Ivel = *t4vect_Zero;
    h->Iangvel = *t4vect_Zero;
    (*(int*)&h->T) = 0;
    h->u = 0;
    h->v = 0;
}

/**
 * Intersects one patch with a terrain query and appends the hit to the query's hit list.
 *
 * Skips the patch unless both bits of 0x41 are set in its flags (m_sSurfaceFlags) and its box (m_fvMin, m_fvMax)
 * overlaps the query's box (boxMax, boxMin). Gets its forward differences from the context's cache
 * (cAIFwdDiffCache_GetFwdDiff), tests it with cWorldIntersectType_IsPatchInt and, on a hit, resets record *count of the
 * hit array (On Tour tIntersectInfo::Init), stores position, normal, the patch's surface id (m_sSurfaceType), the
 * patch, the parameter, and u and v, and increments the count. The count is not checked against the maximum. Called per
 * patch by the Nash terrain walk (cBXNash_terrainIntersect, nash_privy_QueryHelper_helpX5, nash_privy_QueryHelper_help,
 * nash_Cell_descend4).
 *
 * @param vctx  the terrain query: query, hit array, maximum, count pointer, hint, AI forward-difference cache
 * @param vobj  the patch
 */
extern "C" void TerrainCallback_intersectpatch(void* vctx, void* vobj)
{
    TerrainCallback* ctx = (TerrainCallback*)vctx;
    tPatchRecord* obj = (tPatchRecord*)vobj;
    t4Vector pos;
    t4Vector nrm;
    t4Vector uv;
    void* node;
    float dist;
    if ((obj->m_sSurfaceFlags & tPatchRecord::kCollideFlags) != tPatchRecord::kCollideFlags) return;
    if (!overlap_335960(ctx->intersecttype, obj)) return;
    cAIFwdDiffCache_GetFwdDiff(((void*)ctx->fwddiffcache), obj, &node);
    if (!cWorldIntersectType_IsPatchInt(ctx->intersecttype, obj, node, &pos, &nrm, &dist, &uv, ctx->hint)) return;
    reset_335960(&(ctx->intersectinfo)[*ctx->numintersectinfo]);
    (ctx->intersectinfo)[*ctx->numintersectinfo].Ipos = pos;
    (ctx->intersectinfo)[*ctx->numintersectinfo].Inorm = nrm;
    (ctx->intersectinfo)[*ctx->numintersectinfo].Stype = obj->m_sSurfaceType;
    (ctx->intersectinfo)[*ctx->numintersectinfo].Patch = obj;
    (ctx->intersectinfo)[*ctx->numintersectinfo].Iparam = dist;
    (ctx->intersectinfo)[*ctx->numintersectinfo].u = uv.x;
    (ctx->intersectinfo)[*ctx->numintersectinfo].v = uv.y;
    (*ctx->numintersectinfo)++;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", TerrainCallback_intersectinstance);
#ifdef SKIP_ASM
extern "C" int intersect_instanceIntersect(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapNode_335B90(const cWorldIntersectType* q, const tInstanceRecord* n)
{
    int r = 0;
    if (q->boxMax.x > n->m_fvMin.m[0] && q->boxMin.x < n->m_fvMax.m[0] &&
        q->boxMax.y > n->m_fvMin.m[1] && q->boxMin.y < n->m_fvMax.m[1] &&
        q->boxMax.z > n->m_fvMin.m[2] && q->boxMin.z < n->m_fvMax.m[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_335B90(const cWorldIntersectType* q, const tBoundBox* o)
{
    int r = 0;
    if (q->boxMax.x > o->MinPoint.x && q->boxMin.x < o->MaxPoint.x &&
        q->boxMax.y > o->MinPoint.y && q->boxMin.y < o->MaxPoint.y &&
        q->boxMax.z > o->MinPoint.z && q->boxMin.z < o->MaxPoint.z) {
        r = 1;
    }
    return r;
}

static inline void collect_335B90(TerrainCallback* ctx, tInstanceRecord* node)
{
    int n = *ctx->numintersectinfo;
    *ctx->numintersectinfo += intersect_instanceIntersect(ctx->intersecttype, node, &ctx->intersectinfo[n], ctx->maxintersectinfo - n, 2);
}

/**
 * Intersects one instance with a terrain query and adds its hits to the query's hit list.
 *
 * Skips instances without a m_pCollideRecord. A flag 0x20 instance is box-tested against its stored box (m_fvMin,
 * m_fvMax); a flag 0x40 one needs a non-null object (m_pActiveNode) that answers true through virtual slot 44
 * (hasBoundBox) and whose slot 45 box (getBoundBox) overlaps. Each overlapping instance goes to
 * intersect_instanceIntersect (mode 2) at record *count with the room left, and the count grows by its result. Called
 * per instance by the Nash terrain walk (cBXNash_terrainIntersect, nash_privy_QueryHelper_helpX5,
 * nash_privy_QueryHelper_help, nash_Cell_descend4).
 *
 * @param vctx  the terrain query: query, hit array, maximum, count pointer
 * @param vnode  the instance
 */
extern "C" void TerrainCallback_intersectinstance(void* vctx, void* vnode)
{
    TerrainCallback* ctx = (TerrainCallback*)vctx;
    tInstanceRecord* node = (tInstanceRecord*)vnode;
    if (node->m_pCollideRecord == 0) return;
    int flags = node->m_iFlags;
    if (flags & 0x20) {
        if (overlapNode_335B90(ctx->intersecttype, node)) {
            collect_335B90(ctx, node);
        }
    } else if (flags & 0x40) {
        if (node->m_pActiveNode == 0) return;
        if (node->m_pActiveNode->hasBoundBox()) {
            tBoundBox* objBox = (tBoundBox*)node->m_pActiveNode->getBoundBox();
            if (overlapBox_335B90(ctx->intersecttype, objBox)) {
                collect_335B90(ctx, node);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_terrainIntersect);
#ifdef SKIP_ASM




extern "C" void nash_Cell_descend4(nash_Cell* self, void* ctx);
extern "C" void nash_privy_QueryHelper_helpX5(char* flag, TerrainCallback* ctx, TerrainCallback* ctx2, tBoundBox* box, nash_Cell* tree, void* cell);

static inline void collect_335D78(nash_Cell* node, TerrainCallback* ctx)
{
    nash_Node* p;
    for (p = node->nodesinside[1].head; p != 0; p = p->next) {
        TerrainCallback_intersectpatch(ctx, p);
    }
    for (p = node->nodesinside[0].head; p != 0; p = p->next) {
        TerrainCallback_intersectinstance(ctx, p);
    }
    if (node->children[0][0][0] != 0) node->children[0][0][0]->descend4(ctx);
    if (node->children[0][0][1] != 0) node->children[0][0][1]->descend4(ctx);
    if (node->children[0][1][0] != 0) node->children[0][1][0]->descend4(ctx);
    if (node->children[0][1][1] != 0) node->children[0][1][1]->descend4(ctx);
    if (node->children[1][0][0] != 0) node->children[1][0][0]->descend4(ctx);
    if (node->children[1][0][1] != 0) node->children[1][0][1]->descend4(ctx);
    if (node->children[1][1][0] != 0) node->children[1][1][0]->descend4(ctx);
    if (node->children[1][1][1] != 0) node->children[1][1][1]->descend4(ctx);
}

// Queries octant i of the Nash (retail walks octant[2][2][2] in flat order, 0-7).
static inline void queryCell_335D78(cBXNash* nash, int i, tBoundBox* box, TerrainCallback* ctx)
{
    nash_Octant* cell = &nash->octant[i >> 2][(i >> 1) & 1][i & 1];
    if (nash->octant[i >> 2][(i >> 1) & 1][i & 1].rootcell != 0) {
        nash_Cell* tree = cell->rootcell;
        nash_privy_QueryHelper helper;
        char* flag = (char*)&helper;
        int r = nash_CellKey_proximitytest(cell, box);
        if (r == 0) {
            collect_335D78(tree, ctx);
        } else if (r == 2) {
            nash_privy_QueryHelper_helpX5(flag, ctx, ctx, box, tree, cell);
        }
    }
}


/**
 * Intersects a query with the terrain of the Nash's octree, storing the hits; returns their count.
 *
 * Builds a terrain query context (the query, @p hits, @p cap, a local count, @p hitCache and the AI forward-difference
 * cache TheAIFwdDiffCache) and the query's box (boxMin, boxMax). For each of the eight octants with a root cell,
 * nash_CellKey_proximitytest 0 (inside) feeds the cell's patch list (nodesinside[1]) to TerrainCallback_intersectpatch,
 * its instance list (nodesinside[0]) to TerrainCallback_intersectinstance and its children to nash_Cell_descend4; 2
 * (crossing) goes to nash_privy_QueryHelper_helpX5. Called by cBXNash_terrainIntersect1.
 *
 * @param vq  the query (On Tour cWorldIntersectType)
 * @param hits  out: the hit records
 * @param cap  the number of records @p hits holds
 * @param hitCache  passed to cWorldIntersectType_IsPatchInt for each patch (On Tour tIntHint *hint)
 * @return the number of hits stored
 */
int cBXNash::terrainIntersect(cWorldIntersectType* vq, tIntersectInfo* hits, int cap, tIntHint* hitCache)
{
    cBXNash* nash = this;
    cWorldIntersectType* query = (cWorldIntersectType*)vq;
    TerrainCallback ctx;
    tBoundBox box;
    int count;
    ctx.intersecttype = query;
    ctx.intersectinfo = hits;
    ctx.maxintersectinfo = cap;
    ctx.numintersectinfo = &count;
    ctx.hint = hitCache;
    ctx.fwddiffcache = nash->TheAIFwdDiffCache;
    box.MinPoint = (*(&query->boxMin));
    box.MaxPoint = (*(&query->boxMax));
    TerrainCallback* pctx = &ctx;
    count = 0;
    queryCell_335D78(nash, 0, &box, pctx);
    queryCell_335D78(nash, 1, &box, pctx);
    queryCell_335D78(nash, 2, &box, pctx);
    queryCell_335D78(nash, 3, &box, pctx);
    queryCell_335D78(nash, 4, &box, pctx);
    queryCell_335D78(nash, 5, &box, pctx);
    queryCell_335D78(nash, 6, &box, pctx);
    queryCell_335D78(nash, 7, &box, pctx);
    return count;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", cBXNash_terrainIntersect1);
#ifdef SKIP_ASM
extern "C" int cBXNash_terrainIntersect(cBXNash* self, cWorldIntersectType* q, tIntersectInfo* hits, int max,
        tIntHint* hint);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_00336850(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int isBetter_00336850(tIntersectInfo* a, tIntersectInfo* b)
{
    if (a->Inst != 0) {
        if (b->Inst == 0) {
            return 1;
        }
        return a->Inst->m_RID < b->Inst->m_RID;
    }
    if (b->Inst != 0) {
        return 0;
    }
    return a->Patch->m_RID < b->Patch->m_RID;
}

/**
 * Returns the parameter of the Nash terrain hit nearest the query's ideal parameter, copying that hit to @p out.
 *
 * Collects up to 64 hits with cBXNash_terrainIntersect and returns -1.0 when there are none. Otherwise picks the hit whose
 * parameter (Iparam) is closest to the query's virtual slot 3 (On Tour GetIdealParam); ties go to an instance hit over
 * a patch hit, then to the smaller m_RID (of the instances, or of the patches) (On Tour orderIntersectInfo). Called on
 * SSXApp->Game->BXNash by the camera (cCamera_avoidGround), the rider (cRider_putAt, cRider_GetNISEndPosition), the air
 * predictor and others.
 *
 * @param intersectType  the query (On Tour cWorldIntersectType)
 * @param out  out: the chosen hit
 * @param hint  passed through to cBXNash_terrainIntersect (On Tour tIntHint *hint; cCamera_avoidGround passes its intersection hint)
 * @return the chosen hit's parameter, or -1.0
 */
float cBXNash::terrainIntersect1(cWorldIntersectType* intersectType, tIntersectInfo* out, tIntHint* hint)
{
    tIntersectInfoElem hits[64];
    int n = terrainIntersect(intersectType, hits, 0x40, hint);
    if (n == 0) {
        return -1.0f;
    }
    int best = 0;
    float ref = intersectType->GetIdealParam();
    float bestDiff = absf_00336850(hits[0].Iparam - ref);
    for (int i = 1; i < n; i++) {
        float paramDiff = absf_00336850(hits[i].Iparam - ref);
        if (paramDiff < bestDiff) {
            bestDiff = paramDiff;
            best = i;
        } else if (paramDiff == bestDiff) {
            if (isBetter_00336850(&hits[i], &hits[best])) {
                best = i;
            }
        }
    }
    *out = hits[best];
    return out->Iparam;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_getGroundHeight);
#ifdef SKIP_ASM

extern "C" void* cWorldLine_cWorldLine(void* seg, const t4Vector& a, const t4Vector& b, int n, float r);
extern "C" void nash_privy_QueryHelper_help(nash_privy_QueryHelper* helper, TerrainCallback* q, TerrainCallback* q2,
                                            VerticalLineProximityTest* test, nash_Cell* node, nash_CellKey* cell);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_003369D8(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// PORT: g++ `<?` (min) operator.
static inline float clamp_003369D8(float v, float lo, float hi)
{
    if (v >= lo) return v <? hi;
    return lo;
}

static inline int isBetter_003369D8(tIntersectInfo* a, tIntersectInfo* b)
{
    if (a->Inst != 0) {
        if (b->Inst == 0) {
            return 1;
        }
        return a->Inst->m_RID < b->Inst->m_RID;
    }
    if (b->Inst != 0) {
        return 0;
    }
    return a->Patch->m_RID < b->Patch->m_RID;
}

static inline void visit_003369D8(nash_Octant* c, nash_privy_QueryHelper* helper, TerrainCallback* q,
                                  VerticalLineProximityTest* p)
{
    helper->help(q, q, p, c->rootcell, &c->rootkey);
}

/**
 * Finds the terrain straight above or below @p pos; on a hit stores its position and normal and returns true.
 *
 * Builds a vertical cWorldLine from 100000 below to 100000 above @p pos (precision 2, ideal parameter at 20 units above
 * @p pos, clamped to 0..1) and a terrain query on it (64 hit records, no hint, the AI forward-difference cache
 * TheAIFwdDiffCache), then walks each of the eight octants that has a root cell with nash_privy_QueryHelper_help.
 * Returns false without hits; otherwise picks the hit whose height (z) is closest to @p pos's, ties going to an
 * instance hit and then to the smaller m_RID, and copies that hit's position and normal out. Called by
 * cSSXScriptEngine_GetAnchorOrientation.
 *
 * @param pos  the point (x, y, z)
 * @param outP  out: the hit position
 * @param outN  out: the hit normal
 * @return true on a hit, otherwise false
 */
bool cBXNash::getGroundHeight(float* pos, t4Vector* outP, t4Vector* outN)
{
    char seg[176];
    float lo = pos[2] - 100000.0f;
    float hi = pos[2] + 100000.0f;
    float idealParam = clamp_003369D8((pos[2] + 20.0f - lo) / (hi - lo), 0.0f, 1.0f);
    {
        t4Vector lineStart;
        lineStart.x = pos[0];
        lineStart.y = pos[1];
        lineStart.z = lo;
        lineStart.w = 1.0f;
        t4Vector lineEnd;
        lineEnd.x = pos[0];
        lineEnd.y = pos[1];
        lineEnd.z = hi;
        lineEnd.w = 1.0f;
        cWorldLine_cWorldLine(seg, lineStart, lineEnd, 2, idealParam);
    }
    tIntersectInfoElem hits[64];
    TerrainCallback terrainQuery;
    terrainQuery.intersecttype = (cWorldIntersectType*)seg;  // the cWorldLine built in seg
    terrainQuery.intersectinfo = hits;
    terrainQuery.maxintersectinfo = 0x40;
    // The walk's state, kept together as in retail: the hit count, the line test, then one empty helper per octant.
    struct {
        int count;
        VerticalLineProximityTest lineTest;
        nash_privy_QueryHelper helpers[8];
    } walk;
    terrainQuery.numintersectinfo = &walk.count;
    terrainQuery.hint = 0;
    terrainQuery.fwddiffcache = TheAIFwdDiffCache;
    walk.count = 0;
    walk.lineTest.position = (t4Vector*)pos;
    VerticalLineProximityTest* ctx = &walk.lineTest;
    nash_Octant* octant0 = &octant[0][0][0];
    nash_Octant* octant1 = &octant[0][0][1];
    nash_Octant* octant2 = &octant[0][1][0];
    nash_Octant* octant3 = &octant[0][1][1];
    nash_Octant* octant4 = &octant[1][0][0];
    nash_Octant* octant5 = &octant[1][0][1];
    nash_Octant* octant6 = &octant[1][1][0];
    nash_Octant* octant7 = &octant[1][1][1];
    if (((void*)octant[0][0][0].rootcell)) visit_003369D8(octant0, &walk.helpers[0], &terrainQuery, ctx);
    if (((void*)octant[0][0][1].rootcell)) visit_003369D8(octant1, &walk.helpers[1], &terrainQuery, ctx);
    if (((void*)octant[0][1][0].rootcell)) visit_003369D8(octant2, &walk.helpers[2], &terrainQuery, ctx);
    if (((void*)octant[0][1][1].rootcell)) visit_003369D8(octant3, &walk.helpers[3], &terrainQuery, ctx);
    if (((void*)octant[1][0][0].rootcell)) visit_003369D8(octant4, &walk.helpers[4], &terrainQuery, ctx);
    if (((void*)octant[1][0][1].rootcell)) visit_003369D8(octant5, &walk.helpers[5], &terrainQuery, ctx);
    if (((void*)octant[1][1][0].rootcell)) visit_003369D8(octant6, &walk.helpers[6], &terrainQuery, ctx);
    if (((void*)octant[1][1][1].rootcell)) visit_003369D8(octant7, &walk.helpers[7], &terrainQuery, ctx);
    if (walk.count == 0) {
        return false;
    }
    float ref = pos[2];
    int best = 0;
    float bestDiff = absf_003369D8(hits[0].Ipos.z - ref);
    for (int i = 1; i < walk.count; i++) {
        float heightDiff = absf_003369D8((hits + i)->Ipos.z - ref);
        if (heightDiff < bestDiff) {
            bestDiff = heightDiff;
            best = i;
        } else if (heightDiff == bestDiff) {
            if (isBetter_003369D8(hits + i, hits + best)) {
                best = i;
            }
        }
    }
    *outP = (hits + best)->Ipos;
    *outN = (hits + best)->Inorm;
    return true;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", StaticObjectCallback_intersectinstance);
#ifdef SKIP_ASM
extern "C" int intersect_instanceIntersect(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapNode_336D40(const cWorldIntersectType* q, const tInstanceRecord* n)
{
    int r = 0;
    if (q->boxMax.x > n->m_fvMin.m[0] && q->boxMin.x < n->m_fvMax.m[0] &&
        q->boxMax.y > n->m_fvMin.m[1] && q->boxMin.y < n->m_fvMax.m[1] &&
        q->boxMax.z > n->m_fvMin.m[2] && q->boxMin.z < n->m_fvMax.m[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_336D40(const cWorldIntersectType* q, const tBoundBox* o)
{
    int r = 0;
    if (q->boxMax.x > o->MinPoint.x && q->boxMin.x < o->MaxPoint.x &&
        q->boxMax.y > o->MinPoint.y && q->boxMin.y < o->MaxPoint.y &&
        q->boxMax.z > o->MinPoint.z && q->boxMin.z < o->MaxPoint.z) {
        r = 1;
    }
    return r;
}

static inline void collect_336D40(StaticObjectCallback* ctx, tInstanceRecord* node, int mode)
{
    int n = *ctx->numintersectinfo;
    *ctx->numintersectinfo += intersect_instanceIntersect(ctx->intersecttype, node, &ctx->intersectinfo[n], ctx->maxintersectinfo - n, mode);
}

/**
 * Intersects one instance with a static-object query and adds its hits to the query's hit list.
 *
 * A flag 0x20 instance is box-tested against its stored box (m_fvMin, m_fvMax); a flag 0x40 one asks its object
 * (m_pActiveNode) through virtual slots 44 (hasBoundBox) and 45 (getBoundBox). Each overlapping instance goes to
 * intersect_instanceIntersect at record *count with the room left, in mode 3 when the context's includeridable is set
 * and 1 otherwise, and the count grows by its result. Unlike TerrainCallback_intersectinstance it checks neither
 * m_pCollideRecord nor a null object. Called per instance by the Nash static-object walk
 * (cBXNash_staticObjectIntersect, nash_privy_QueryHelper_helpX6, nash_Cell_descend5).
 *
 * @param vctx  the static-object query: query, hit array, maximum, count pointer, include flag
 * @param vnode  the instance
 */
extern "C" void StaticObjectCallback_intersectinstance(void* vctx, void* vnode)
{
    StaticObjectCallback* ctx = (StaticObjectCallback*)vctx;
    tInstanceRecord* node = (tInstanceRecord*)vnode;
    int mode = ctx->includeridable ? 3 : 1;
    int flags = node->m_iFlags;
    if (flags & 0x20) {
        if (overlapNode_336D40(ctx->intersecttype, node)) {
            collect_336D40(ctx, node, mode);
        }
    } else if (flags & 0x40) {
        if (node->m_pActiveNode->hasBoundBox()) {
            tBoundBox* objBox = (tBoundBox*)node->m_pActiveNode->getBoundBox();
            if (overlapBox_336D40(ctx->intersecttype, objBox)) {
                collect_336D40(ctx, node, mode);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_staticObjectIntersect);
#ifdef SKIP_ASM



extern "C" void nash_Cell_descend5(nash_Cell* self, void* ctx);
extern "C" void nash_privy_QueryHelper_helpX6(char* flag, StaticObjectCallback* ctx, StaticObjectCallback* ctx2, tBoundBox* box, void* tree, void* cell);

static inline void collect_336F30(nash_Cell* node, StaticObjectCallback* ctx)
{
    nash_Node* p;
    for (p = node->nodesinside[0].head; p != 0; p = p->next) {
        StaticObjectCallback_intersectinstance(ctx, p);
    }
    if (node->children[0][0][0] != 0) node->children[0][0][0]->descend5(ctx);
    if (node->children[0][0][1] != 0) node->children[0][0][1]->descend5(ctx);
    if (node->children[0][1][0] != 0) node->children[0][1][0]->descend5(ctx);
    if (node->children[0][1][1] != 0) node->children[0][1][1]->descend5(ctx);
    if (node->children[1][0][0] != 0) node->children[1][0][0]->descend5(ctx);
    if (node->children[1][0][1] != 0) node->children[1][0][1]->descend5(ctx);
    if (node->children[1][1][0] != 0) node->children[1][1][0]->descend5(ctx);
    if (node->children[1][1][1] != 0) node->children[1][1][1]->descend5(ctx);
}

// Queries octant i of the Nash (retail walks octant[2][2][2] in flat order, 0-7).
static inline void queryCell_336F30(cBXNash* nash, int i, tBoundBox* box, StaticObjectCallback* ctx)
{
    nash_Octant* cell = &nash->octant[i >> 2][(i >> 1) & 1][i & 1];
    if (nash->octant[i >> 2][(i >> 1) & 1][i & 1].rootcell != 0) {
        nash_Cell* tree = cell->rootcell;
        nash_privy_QueryHelper helper;
        char* flag = (char*)&helper;
        int r = nash_CellKey_proximitytest(cell, box);
        if (r == 0) {
            collect_336F30(tree, ctx);
        } else if (r == 2) {
            nash_privy_QueryHelper_helpX6(flag, ctx, ctx, box, tree, cell);
        }
    }
}


/**
 * Intersects a query with the instances of the Nash's octree, storing the hits; returns their count.
 *
 * Builds a static-object query context (the query, @p hits, @p cap, a local count, @p full) and the query's box
 * (boxMin, boxMax). For each of the eight octants with a root cell, nash_CellKey_proximitytest 0 (inside) feeds the
 * cell's instance list (nodesinside[0]) to StaticObjectCallback_intersectinstance and its children to
 * nash_Cell_descend5; 2 (crossing) goes to nash_privy_QueryHelper_helpX6. Called by cBXNash_staticObjectIntersect1.
 *
 * @param vq  the query (On Tour cWorldIntersectType)
 * @param hits  out: the hit records
 * @param cap  the number of records @p hits holds
 * @param full  nonzero makes StaticObjectCallback_intersectinstance pass mode 3 instead of 1 (On Tour includeRidable)
 * @return the number of hits stored
 */
int cBXNash::staticObjectIntersect(cWorldIntersectType* vq, tIntersectInfo* hits, int cap, int full)
{
    cBXNash* nash = this;
    cWorldIntersectType* query = (cWorldIntersectType*)vq;
    StaticObjectCallback ctx;
    tBoundBox box;
    int count;
    ctx.intersecttype = query;
    ctx.intersectinfo = hits;
    ctx.maxintersectinfo = cap;
    ctx.numintersectinfo = &count;
    ctx.includeridable = full;
    box.MinPoint = (*(&query->boxMin));
    box.MaxPoint = (*(&query->boxMax));
    StaticObjectCallback* pctx = &ctx;
    count = 0;
    queryCell_336F30(nash, 0, &box, pctx);
    queryCell_336F30(nash, 1, &box, pctx);
    queryCell_336F30(nash, 2, &box, pctx);
    queryCell_336F30(nash, 3, &box, pctx);
    queryCell_336F30(nash, 4, &box, pctx);
    queryCell_336F30(nash, 5, &box, pctx);
    queryCell_336F30(nash, 6, &box, pctx);
    queryCell_336F30(nash, 7, &box, pctx);
    return count;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", cBXNash_staticObjectIntersect1);
#ifdef SKIP_ASM
extern "C" int cBXNash_staticObjectIntersect(cBXNash* self, cWorldIntersectType* q, tIntersectInfo* hits, int max,
                                             int a4);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_003378C0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int isBetter_003378C0(tIntersectInfo* a, tIntersectInfo* b)
{
    if (a->Inst != 0) {
        if (b->Inst == 0) {
            return 1;
        }
        return a->Inst->m_RID < b->Inst->m_RID;
    }
    if (b->Inst != 0) {
        return 0;
    }
    return a->Patch->m_RID < b->Patch->m_RID;
}

/**
 * Returns the parameter of the Nash static-object hit nearest the query's ideal parameter, copying that hit to @p out.
 *
 * Collects up to 128 hits with cBXNash_staticObjectIntersect and returns -1.0 when there are none. Otherwise picks the hit whose
 * parameter (Iparam) is closest to the query's virtual slot 3 (On Tour GetIdealParam); ties go to an instance hit over
 * a patch hit, then to the smaller m_RID (of the instances, or of the patches). Called by tAvalancheNode_calculate and
 * cAirPredictor_simStep.
 *
 * @param query  the query (On Tour cWorldIntersectType)
 * @param out  out: the chosen hit
 * @param full  passed through to cBXNash_staticObjectIntersect (On Tour includeRidable)
 * @return the chosen hit's parameter, or -1.0
 */
float cBXNash::staticObjectIntersect1(cWorldIntersectType* query, tIntersectInfo* out, int full)
{
    tIntersectInfoElem hits[128];
    int n = this->staticObjectIntersect(query, hits, 0x80, full);
    if (n == 0) {
        return -1.0f;
    }
    int best = 0;
    float ref = query->GetIdealParam();
    float bestDiff = absf_003378C0(hits[0].Iparam - ref);
    for (int i = 1; i < n; i++) {
        float diff = absf_003378C0(hits[i].Iparam - ref);
        if (diff < bestDiff) {
            bestDiff = diff;
            best = i;
        } else if (diff == bestDiff) {
            if (isBetter_003378C0(&hits[i], &hits[best])) {
                best = i;
            }
        }
    }
    *out = hits[best];
    return out->Iparam;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_splineIntersect);
#ifdef SKIP_ASM
extern "C" void nash_Cell_descend6(nash_Cell* tree, SplineCallback* q);


extern "C" void nash_privy_QueryHelper_helpX7(char* flag, SplineCallback* q, SplineCallback* q2, tBoundBox* box, void* tree, void* cell);

static inline void collect_337A48(nash_Cell* tree, SplineCallback* q)
{
    for (cTypedSpaceNode* o = (cTypedSpaceNode*)tree->nodesinside[2].head; o != 0; o = (cTypedSpaceNode*)o->next) {
        int type = o->type;
        if (type == 2) {
            cRailModifier_splineIntersect(((cRailSpaceNode*)o)->Rail, ((tBoundBox*)q->box), ((tBoundBox*)q->box),
                                          ((t4Vector*)q->intersectpos), ((int)q->splinefiltermask), q->hit,
                                          q->intersectdist, q->intersectinfo);
        } else if (type == 3) {
            cFlexRailNode_splineIntersect(((cFlexRailSpaceNode*)o)->FlexNode, ((tBoundBox*)q->box),
                                          ((tBoundBox*)q->box), ((t4Vector*)q->intersectpos),
                                          ((int)q->splinefiltermask), q->hit, q->intersectdist,
                                          q->intersectinfo);
        } else if (type == 1) {
            if (((tSplineSegmentRecord*)o)->m_Parent->m_iFlags & ((int)q->splinefiltermask)) {
                intersect_staticSplineSegmentIntersect(o, ((tBoundBox*)q->box), ((t4Vector*)q->intersectpos), q->hit,
                                                       q->intersectdist, q->intersectinfo);
            }
        }
    }
    if (tree->children[0][0][0] != 0) tree->children[0][0][0]->descend6(q);
    if (tree->children[0][0][1] != 0) tree->children[0][0][1]->descend6(q);
    if (tree->children[0][1][0] != 0) tree->children[0][1][0]->descend6(q);
    if (tree->children[0][1][1] != 0) tree->children[0][1][1]->descend6(q);
    if (tree->children[1][0][0] != 0) tree->children[1][0][0]->descend6(q);
    if (tree->children[1][0][1] != 0) tree->children[1][0][1]->descend6(q);
    if (tree->children[1][1][0] != 0) tree->children[1][1][0]->descend6(q);
    if (tree->children[1][1][1] != 0) tree->children[1][1][1]->descend6(q);
}

// Queries octant i of the Nash (retail walks octant[2][2][2] in flat order, 0-7).
static inline void queryCell_337A48(cBXNash* nash, int i, tBoundBox* box, SplineCallback* q)
{
    nash_Octant* cell = &nash->octant[i >> 2][(i >> 1) & 1][i & 1];
    if (nash->octant[i >> 2][(i >> 1) & 1][i & 1].rootcell != 0) {
        nash_Cell* tree = cell->rootcell;
        nash_privy_QueryHelper helper;
        char* flag = (char*)&helper;
        int r = nash_CellKey_proximitytest(cell, box);
        if (r == 0) {
            collect_337A48(tree, q);
        } else if (r == 2) {
            nash_privy_QueryHelper_helpX7(flag, q, q, box, tree, cell);
        }
    }
}

/**
 * Finds the rails and spline segments of the Nash's octree near @p pos and records the nearest in @p intInfo.
 *
 * Builds a box of half-size @p testRadius around @p pos and a spline query (the point, @p intInfo, @p mask, a hit flag, a distance,
 * the box). For each of the eight octants with a root cell, nash_CellKey_proximitytest 0 (inside) walks the cell's
 * typed list (nodesinside[2]) and its children (nash_Cell_descend6); 2 (crossing) goes to
 * nash_privy_QueryHelper_helpX7. Typed nodes of type 2 are tested with cRailModifier_splineIntersect, type 3 with
 * cFlexRailNode_splineIntersect, and type 1 (a spline segment whose owner's m_iFlags match @p mask) with
 * intersect_staticSplineSegmentIntersect. Nothing in the code calls it; cOverlapList_splineIntersect does the same over
 * an overlap list.
 *
 * @param pos  the point to test
 * @param intInfo  out: the nearest hit (On Tour intInfo)
 * @param mask  which spline types to accept (On Tour splineFilterMask)
 * @param testRadius  the test radius
 * @return nonzero when a hit was recorded
 */
int cBXNash::splineIntersect(t4Vector* pos, tIntersectInfo* intInfo, int mask, float testRadius)
{
    tBoundBox box;
    t4Vector ext;
    SplineCallback splineQuery;
    int cnt;
    float dist;
    box.MinPoint = *pos;
    box.MaxPoint = *pos;
    ext.x = testRadius;
    ext.y = testRadius;
    ext.z = testRadius;
    ext.w = 0.0f;
    wsSubEq_334680(box.MinPoint, ext);
    wsAddEq_334680(box.MaxPoint, ext);
    tBoundBox* pbox = &box;
    splineQuery.intersectpos = pos;
    splineQuery.intersectinfo = intInfo;
    splineQuery.splinefiltermask = mask;
    splineQuery.hit = &cnt;
    splineQuery.intersectdist = &dist;
    splineQuery.box = pbox;
    dist = 0.0f;
    cnt = 0;
    queryCell_337A48(this, 0, pbox, &splineQuery);
    queryCell_337A48(this, 1, pbox, &splineQuery);
    queryCell_337A48(this, 2, pbox, &splineQuery);
    queryCell_337A48(this, 3, pbox, &splineQuery);
    queryCell_337A48(this, 4, pbox, &splineQuery);
    queryCell_337A48(this, 5, pbox, &splineQuery);
    queryCell_337A48(this, 6, pbox, &splineQuery);
    queryCell_337A48(this, 7, pbox, &splineQuery);
    return cnt;
}
#endif
//100%
INCLUDE_ASM("intersect/worldsphtree", func_003388E0);
#ifdef SKIP_ASM
/**
 * Returns its first argument and ignores the rest.
 *
 * A variadic do-nothing: it only spills the remaining argument registers (a1-a7, f12-f18) into its frame. Nothing in
 * the code calls it.
 *
 * @return @p self
 */
extern "C" void* func_003388E0(void* self, ...)
{
    return self;
}
#endif
//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX1);
#ifdef SKIP_ASM

/**
 * Walks a crossing octree cell for the overlap-list instance query: collects its instances, then visits its children.
 *
 * Runs OverlapInstancesCallback_operatorCall(@p ctx) on @p vnode and stops when the cell's level is 11. Otherwise
 * splits the cell (level, x, y, z at @p vcell; size 2^level, built from the float exponent bits) into its eight
 * children one level down, each widened by 0.2 child cell on every side. Each existing child (@p vnode +0x0..+0x1C)
 * that overlaps @p vbox on all three axes is collected whole with @p ctx2 (the callback, then nash_Cell_descend for its
 * children) when the box contains it, and recursed into otherwise. Children are visited in the key order 000, 001, 011,
 * 010, 110, 111, 101, 100 (x, y, z). Called by cOverlapList_getinstances for crossing root cells.
 *
 * @param flag  the empty helper object, only passed on to the recursion (On Tour In *this)
 * @param ctx  the callback for crossing cells (On Tour oncross)
 * @param ctx2  the callback for contained cells (On Tour oninside)
 * @param vbox  the query box: min at +0x0, max at +0x10
 * @param vnode  the cell (On Tour const Cell *)
 * @param vcell  the cell's key: level, x, y, z (On Tour CellKey)
 */
extern "C" void nash_privy_QueryHelper_helpX1(char* flag, OverlapInstancesCallback* ctx, OverlapInstancesCallback* ctx2, void* vbox, nash_Cell* vnode, void* vcell)
{
    tBoundBox* box = (tBoundBox*)vbox;
    nash_Cell* node = (nash_Cell*)vnode;
    nash_CellKey* cell = (nash_CellKey*)vcell;
    ctx->operatorCall((nash_Cell*)node);
    if (cell->level == nash_CellKey::kLeafLevel) {
        return;
    }
    nash_CellKey sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    union { int i; float f; } bits;  // 2^level built from the float exponent field
    bits.i = (sub.level + 0x7F) << 23;
    float cellSize = bits.f;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->MinPoint.x < x0hi && x0lo < box->MaxPoint.x) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->MinPoint.x < x1hi && x1lo < box->MaxPoint.x) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->MinPoint.y < y0hi && y0lo < box->MaxPoint.y) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->MinPoint.y < y1hi && y1lo < box->MaxPoint.y) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->MinPoint.z < z0hi && z0lo < box->MaxPoint.z) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->MinPoint.z < z1hi && z1lo < box->MaxPoint.z) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->MinPoint.x < x0lo && x0hi < box->MaxPoint.x) cx0 = 1;
    int cx1 = 0;
    if (box->MinPoint.x < x1lo && x1hi < box->MaxPoint.x) cx1 = 1;
    int cy0 = 0;
    if (box->MinPoint.y < y0lo && y0hi < box->MaxPoint.y) cy0 = 1;
    int cy1 = 0;
    if (box->MinPoint.y < y1lo && y1hi < box->MaxPoint.y) cy1 = 1;
    int cz0 = 0;
    if (box->MinPoint.z < z0lo && z0hi < box->MaxPoint.z) cz0 = 1;
    int cz1 = 0;
    if (box->MinPoint.z < z1lo && z1hi < box->MaxPoint.z) cz1 = 1;

    nash_Cell* subNode;
    subNode = node->children[0][0][0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_330BA8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[0][0][1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_330BA8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y++;
    subNode = node->children[0][1][1];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_330BA8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[0][1][0];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_330BA8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.x++;
    subNode = node->children[1][1][0];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_330BA8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[1][1][1];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_330BA8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y--;
    subNode = node->children[1][0][1];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_330BA8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[1][0][0];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_330BA8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX2);
#ifdef SKIP_ASM

/**
 * Walks a crossing octree cell for the overlap-list typed query: collects its typed nodes, then visits its children.
 *
 * Appends every node of the cell's typed list (@p vnode nodesinside[2]) to @p ctx's list (count pointer number), with
 * no box test and no maximum check, and stops when the cell's level is 11. Otherwise splits the cell (level, x, y, z at
 * @p vcell; size 2^level) into its eight children one level down, each widened by 0.2 child cell on every side. Each
 * existing child that overlaps @p vbox on all three axes is collected whole with @p ctx2 (its typed list, then
 * nash_Cell_descend1 for its children) when the box contains it, and recursed into otherwise, in the key order 000,
 * 001, 011, 010, 110, 111, 101, 100. Called by cOverlapList_gettyped for crossing root cells.
 *
 * @param flag  the empty helper object, only passed on to the recursion (On Tour In *this)
 * @param ctx  the collector for crossing cells (On Tour oncross)
 * @param ctx2  the collector for contained cells (On Tour oninside)
 * @param vbox  the query box: min at +0x0, max at +0x10
 * @param vnode  the cell (On Tour const Cell *)
 * @param vcell  the cell's key: level, x, y, z (On Tour CellKey)
 */
extern "C" void nash_privy_QueryHelper_helpX2(char* flag, OverlapInstancesCallback* ctx, OverlapInstancesCallback* ctx2, void* vbox, nash_Cell* vnode, void* vcell)
{
    tBoundBox* box = (tBoundBox*)vbox;
    nash_Cell* node = (nash_Cell*)vnode;
    nash_CellKey* cell = (nash_CellKey*)vcell;
    nash_Node* objNode;
    for (objNode = node->nodesinside[2].head; objNode != 0; objNode = objNode->next) {
        ctx->instances[(*ctx->number)++] = (tInstanceRecord*)objNode;
    }
    if (cell->level == nash_CellKey::kLeafLevel) {
        return;
    }
    nash_CellKey sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    union { int i; float f; } bits;  // 2^level built from the float exponent field
    bits.i = (sub.level + 0x7F) << 23;
    float cellSize = bits.f;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->MinPoint.x < x0hi && x0lo < box->MaxPoint.x) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->MinPoint.x < x1hi && x1lo < box->MaxPoint.x) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->MinPoint.y < y0hi && y0lo < box->MaxPoint.y) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->MinPoint.y < y1hi && y1lo < box->MaxPoint.y) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->MinPoint.z < z0hi && z0lo < box->MaxPoint.z) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->MinPoint.z < z1hi && z1lo < box->MaxPoint.z) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->MinPoint.x < x0lo && x0hi < box->MaxPoint.x) cx0 = 1;
    int cx1 = 0;
    if (box->MinPoint.x < x1lo && x1hi < box->MaxPoint.x) cx1 = 1;
    int cy0 = 0;
    if (box->MinPoint.y < y0lo && y0hi < box->MaxPoint.y) cy0 = 1;
    int cy1 = 0;
    if (box->MinPoint.y < y1lo && y1hi < box->MaxPoint.y) cy1 = 1;
    int cz0 = 0;
    if (box->MinPoint.z < z0lo && z0hi < box->MaxPoint.z) cz0 = 1;
    int cz1 = 0;
    if (box->MinPoint.z < z1lo && z1hi < box->MaxPoint.z) cz1 = 1;

    nash_Cell* subNode;
    subNode = node->children[0][0][0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[0][0][1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y++;
    subNode = node->children[0][1][1];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[0][1][0];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.x++;
    subNode = node->children[1][1][0];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[1][1][1];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y--;
    subNode = node->children[1][0][1];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[1][0][0];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX3);
#ifdef SKIP_ASM
struct sTree_340C20;
extern "C" void nash_Cell_descend2(nash_Cell* tree, OverlapInstancesCallback* ctx);

struct sObj_33A3D8 {
    sObj_33A3D8* next;
    char pad04[0x154];
    float bmin[3];              // 0x158
    float bmax[3];              // 0x164
};


static inline int overlapObj_33A3D8(const tBoundBox* b, const sObj_33A3D8* n)
{
    int r = 0;
    if (b->MinPoint.x <= n->bmax[0] && b->MaxPoint.x >= n->bmin[0] &&
        b->MinPoint.y <= n->bmax[1] && b->MaxPoint.y >= n->bmin[1] &&
        b->MinPoint.z <= n->bmax[2] && b->MaxPoint.z >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void collectObjs_33A3D8(nash_Cell* tree, OverlapInstancesCallback* ctx)
{
    ctx->operatorCall((nash_Cell*)tree);
    OverlapInstancesCallback* q = (OverlapInstancesCallback*)((char*)ctx + 0x10);
    for (sObj_33A3D8* p = (*(sObj_33A3D8**)&tree->nodesinside[1]); p != 0; p = p->next) {
        if (overlapObj_33A3D8(((tBoundBox*)q->box), p)) {
            int* cnt = ((int*)q->number);
            int n = *cnt;
            ((tInstanceRecord**)q->instances)[n] = (tInstanceRecord*)p;
            *cnt = n + 1;
        }
    }
}

static inline void collect_33A3D8(nash_Cell* tree, OverlapInstancesCallback* ctx)
{
    collectObjs_33A3D8(tree, ctx);
    if (tree->children[0][0][0] != 0) tree->children[0][0][0]->descend2(ctx);
    if (tree->children[0][0][1] != 0) tree->children[0][0][1]->descend2(ctx);
    if (tree->children[0][1][0] != 0) tree->children[0][1][0]->descend2(ctx);
    if (tree->children[0][1][1] != 0) tree->children[0][1][1]->descend2(ctx);
    if (tree->children[1][0][0] != 0) tree->children[1][0][0]->descend2(ctx);
    if (tree->children[1][0][1] != 0) tree->children[1][0][1]->descend2(ctx);
    if (tree->children[1][1][0] != 0) tree->children[1][1][0]->descend2(ctx);
    if (tree->children[1][1][1] != 0) tree->children[1][1][1]->descend2(ctx);
}


/**
 * Walks a crossing octree cell for the overlap-list patch and instance query: collects both, then visits its children.
 *
 * Runs OverlapInstancesCallback_operatorCall(@p ctx) on @p vnode, then appends each patch of the cell's patch list
 * (+0x24) whose box (+0x158 min, +0x164 max) overlaps the patch collector's box (@p ctx +0x10) to that collector's
 * list, with no maximum check. Stops when the cell's level is 11; otherwise splits the cell (level, x, y, z at
 * @p vcell; size 2^level) into its eight children one level down, each widened by 0.2 child cell on every side. Each
 * existing child that overlaps @p vbox on all three axes is collected whole with @p ctx2 (then nash_Cell_descend2 for its
 * children) when the box contains it, and recursed into otherwise, in the key order 000, 001, 011, 010, 110, 111, 101,
 * 100. Called by cOverlapList_getpatchesandinstances for crossing root cells.
 *
 * @param flag  the empty helper object, only passed on to the recursion (On Tour In *this)
 * @param ctx  the instance and patch collectors for crossing cells (On Tour oncross)
 * @param ctx2  the collectors for contained cells (On Tour oninside)
 * @param vbox  the query box: min at +0x0, max at +0x10
 * @param vnode  the cell (On Tour const Cell *)
 * @param vcell  the cell's key: level, x, y, z (On Tour CellKey)
 */
extern "C" void nash_privy_QueryHelper_helpX3(char* flag, OverlapInstancesCallback* ctx, OverlapInstancesCallback* ctx2, tBoundBox* vbox, void* vnode, void* vcell)
{
    tBoundBox* box = (tBoundBox*)vbox;
    nash_Cell* node = (nash_Cell*)vnode;
    nash_CellKey* cell = (nash_CellKey*)vcell;
    collectObjs_33A3D8(node, ctx);
    if (cell->level == nash_CellKey::kLeafLevel) {
        return;
    }
    nash_CellKey sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    union { int i; float f; } bits;  // 2^level built from the float exponent field
    bits.i = (sub.level + 0x7F) << 23;
    float cellSize = bits.f;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->MinPoint.x < x0hi && x0lo < box->MaxPoint.x) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->MinPoint.x < x1hi && x1lo < box->MaxPoint.x) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->MinPoint.y < y0hi && y0lo < box->MaxPoint.y) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->MinPoint.y < y1hi && y1lo < box->MaxPoint.y) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->MinPoint.z < z0hi && z0lo < box->MaxPoint.z) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->MinPoint.z < z1hi && z1lo < box->MaxPoint.z) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->MinPoint.x < x0lo && x0hi < box->MaxPoint.x) cx0 = 1;
    int cx1 = 0;
    if (box->MinPoint.x < x1lo && x1hi < box->MaxPoint.x) cx1 = 1;
    int cy0 = 0;
    if (box->MinPoint.y < y0lo && y0hi < box->MaxPoint.y) cy0 = 1;
    int cy1 = 0;
    if (box->MinPoint.y < y1lo && y1hi < box->MaxPoint.y) cy1 = 1;
    int cz0 = 0;
    if (box->MinPoint.z < z0lo && z0hi < box->MaxPoint.z) cz0 = 1;
    int cz1 = 0;
    if (box->MinPoint.z < z1lo && z1hi < box->MaxPoint.z) cz1 = 1;

    nash_Cell* subNode;
    subNode = node->children[0][0][0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[0][0][1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y++;
    subNode = node->children[0][1][1];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[0][1][0];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.x++;
    subNode = node->children[1][1][0];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[1][1][1];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y--;
    subNode = node->children[1][0][1];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[1][0][0];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX4);
#ifdef SKIP_ASM
struct sBox_33B748 {
    float min[4];
    float max[4];
};

struct sSub_33B748 {
    void** list;
    int* count;
    int pad08;
    sBox_33B748* box;
};


struct sObjNode_33B748 {
    sObjNode_33B748* next;
    int pad04[85];
    float bmin[3];
    float bmax[3];
};



// PORT: alias; the unit defines nash_Cell_descend3 later with its own struct types.
extern "C" void nash_Cell_descend3_33B748(nash_Cell* self, OverlapEverythingCallback* ctx) __asm__("nash_Cell_descend3");

static inline void add_33B748(sSub_33B748* s, void* p)
{
    int* cnt = s->count;
    int n = *cnt;
    s->list[n] = p;
    *cnt = n + 1;
}

static inline int overlap_33B748(const sBox_33B748* b, const sObjNode_33B748* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void visit_33B748(nash_Cell* self, OverlapEverythingCallback* ctx)
{
    ((OverlapInstancesCallback*)ctx)->operatorCall((nash_Cell*)self);
    sSub_33B748* s = &(*(sSub_33B748*)&ctx->patchescallback);
    for (sObjNode_33B748* p = (*(sObjNode_33B748**)&self->nodesinside[1]); p != 0; p = p->next) {
        if (overlap_33B748(s->box, p)) {
            add_33B748(s, p);
        }
    }
    s = &(*(sSub_33B748*)&ctx->typedcallback);
    for (void** q = (*(void***)&self->nodesinside[2]); q != 0; q = (void**)*q) {
        add_33B748(s, q);
    }
}

static inline void collect_33B748(nash_Cell* self, OverlapEverythingCallback* ctx)
{
    visit_33B748(self, ctx);
    if (self->children[0][0][0]) nash_Cell_descend3_33B748(self->children[0][0][0], ctx);
    if (self->children[0][0][1]) nash_Cell_descend3_33B748(self->children[0][0][1], ctx);
    if (self->children[0][1][0]) nash_Cell_descend3_33B748(self->children[0][1][0], ctx);
    if (self->children[0][1][1]) nash_Cell_descend3_33B748(self->children[0][1][1], ctx);
    if (self->children[1][0][0]) nash_Cell_descend3_33B748(self->children[1][0][0], ctx);
    if (self->children[1][0][1]) nash_Cell_descend3_33B748(self->children[1][0][1], ctx);
    if (self->children[1][1][0]) nash_Cell_descend3_33B748(self->children[1][1][0], ctx);
    if (self->children[1][1][1]) nash_Cell_descend3_33B748(self->children[1][1][1], ctx);
}

struct sCtx_340DC0;

/**
 * Walks a crossing octree cell for the overlap-list everything query: collects all three lists, then its children.
 *
 * Runs OverlapInstancesCallback_operatorCall on @p vnode, appends each patch of the cell's patch list (+0x24) whose box
 * (+0x158 min, +0x164 max) overlaps the query box to the patch collector, and appends every typed node of the cell
 * (+0x28) to the typed collector with no box test; no list's maximum is checked. Stops when the cell's level is 11;
 * otherwise splits the cell (level, x, y, z at @p vcell; size 2^level) into its eight children one level down, each
 * widened by 0.2 child cell on every side. Each existing child that overlaps @p vbox on all three axes is collected
 * whole with @p vctx2 (then nash_Cell_descend3 for its children) when the box contains it, and recursed into otherwise, in
 * the key order 000, 001, 011, 010, 110, 111, 101, 100. Called by cOverlapList_geteverything for crossing root cells.
 *
 * @param flag  the empty helper object, only passed on to the recursion (On Tour In *this)
 * @param vctx  the instance, patch and typed collectors for crossing cells (On Tour oncross)
 * @param vctx2  the collectors for contained cells (On Tour oninside)
 * @param vbox  the query box: min at +0x0, max at +0x10
 * @param vnode  the cell (On Tour const Cell *)
 * @param vcell  the cell's key: level, x, y, z (On Tour CellKey)
 */
// Declared as cOverlapList_geteverything (its caller) declares it; sCtx_340DC0 has sCtx_33B748's layout.
extern "C" void nash_privy_QueryHelper_helpX4(char* flag, OverlapEverythingCallback* vctx, OverlapEverythingCallback* vctx2, void* vbox, void* vnode, void* vcell)
{
    OverlapEverythingCallback* ctx = vctx;
    OverlapEverythingCallback* ctx2 = vctx2;
    tBoundBox* box = (tBoundBox*)vbox;
    nash_Cell* node = (nash_Cell*)vnode;
    nash_CellKey* cell = (nash_CellKey*)vcell;
    visit_33B748(node, ctx);
    if (cell->level == nash_CellKey::kLeafLevel) {
        return;
    }
    nash_CellKey sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    union { int i; float f; } bits;  // 2^level built from the float exponent field
    bits.i = (sub.level + 0x7F) << 23;
    float cellSize = bits.f;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->MinPoint.x < x0hi && x0lo < box->MaxPoint.x) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->MinPoint.x < x1hi && x1lo < box->MaxPoint.x) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->MinPoint.y < y0hi && y0lo < box->MaxPoint.y) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->MinPoint.y < y1hi && y1lo < box->MaxPoint.y) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->MinPoint.z < z0hi && z0lo < box->MaxPoint.z) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->MinPoint.z < z1hi && z1lo < box->MaxPoint.z) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->MinPoint.x < x0lo && x0hi < box->MaxPoint.x) cx0 = 1;
    int cx1 = 0;
    if (box->MinPoint.x < x1lo && x1hi < box->MaxPoint.x) cx1 = 1;
    int cy0 = 0;
    if (box->MinPoint.y < y0lo && y0hi < box->MaxPoint.y) cy0 = 1;
    int cy1 = 0;
    if (box->MinPoint.y < y1lo && y1hi < box->MaxPoint.y) cy1 = 1;
    int cz0 = 0;
    if (box->MinPoint.z < z0lo && z0hi < box->MaxPoint.z) cz0 = 1;
    int cz1 = 0;
    if (box->MinPoint.z < z1lo && z1hi < box->MaxPoint.z) cz1 = 1;

    nash_Cell* subNode;
    subNode = node->children[0][0][0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[0][0][1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.y++;
    subNode = node->children[0][1][1];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[0][1][0];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.x++;
    subNode = node->children[1][1][0];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[1][1][1];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.y--;
    subNode = node->children[1][0][1];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[1][0][0];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX5);
#ifdef SKIP_ASM

/**
 * Walks a crossing octree cell for the Nash terrain query: intersects its patches and instances, then its children.
 *
 * Feeds the cell's patch list (@p vnode +0x24) to TerrainCallback_intersectpatch and its instance list (+0x20) to TerrainCallback_intersectinstance with
 * @p ctx, and stops when the cell's level is 11. Otherwise splits the cell (level, x, y, z at @p vcell; size 2^level)
 * into its eight children one level down, each widened by 0.2 child cell on every side. Each existing child that
 * overlaps @p vbox on all three axes is walked whole with @p ctx2 (its lists, then nash_Cell_descend4 for its children) when
 * the box contains it, and recursed into otherwise, in the key order 000, 001, 011, 010, 110, 111, 101, 100. Called by
 * cBXNash_terrainIntersect for crossing root cells.
 *
 * @param flag  the empty helper object, only passed on to the recursion (On Tour In *this)
 * @param ctx  the terrain query for crossing cells (On Tour oncross)
 * @param ctx2  the terrain query for contained cells (On Tour oninside)
 * @param vbox  the query box: min at +0x0, max at +0x10
 * @param vnode  the cell (On Tour const Cell *)
 * @param vcell  the cell's key: level, x, y, z (On Tour CellKey)
 */
extern "C" void nash_privy_QueryHelper_helpX5(char* flag, TerrainCallback* ctx, TerrainCallback* ctx2, tBoundBox* vbox, nash_Cell* vnode, void* vcell)
{
    tBoundBox* box = (tBoundBox*)vbox;
    nash_Cell* node = (nash_Cell*)vnode;
    nash_CellKey* cell = (nash_CellKey*)vcell;
    nash_Node* objNode;
    for (objNode = node->nodesinside[1].head; objNode != 0; objNode = objNode->next) {
        TerrainCallback_intersectpatch(ctx, objNode);
    }
    for (objNode = node->nodesinside[0].head; objNode != 0; objNode = objNode->next) {
        TerrainCallback_intersectinstance(ctx, objNode);
    }
    if (cell->level == nash_CellKey::kLeafLevel) {
        return;
    }
    nash_CellKey sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    union { int i; float f; } bits;  // 2^level built from the float exponent field
    bits.i = (sub.level + 0x7F) << 23;
    float cellSize = bits.f;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->MinPoint.x < x0hi && x0lo < box->MaxPoint.x) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->MinPoint.x < x1hi && x1lo < box->MaxPoint.x) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->MinPoint.y < y0hi && y0lo < box->MaxPoint.y) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->MinPoint.y < y1hi && y1lo < box->MaxPoint.y) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->MinPoint.z < z0hi && z0lo < box->MaxPoint.z) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->MinPoint.z < z1hi && z1lo < box->MaxPoint.z) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->MinPoint.x < x0lo && x0hi < box->MaxPoint.x) cx0 = 1;
    int cx1 = 0;
    if (box->MinPoint.x < x1lo && x1hi < box->MaxPoint.x) cx1 = 1;
    int cy0 = 0;
    if (box->MinPoint.y < y0lo && y0hi < box->MaxPoint.y) cy0 = 1;
    int cy1 = 0;
    if (box->MinPoint.y < y1lo && y1hi < box->MaxPoint.y) cy1 = 1;
    int cz0 = 0;
    if (box->MinPoint.z < z0lo && z0hi < box->MaxPoint.z) cz0 = 1;
    int cz1 = 0;
    if (box->MinPoint.z < z1lo && z1hi < box->MaxPoint.z) cz1 = 1;

    nash_Cell* subNode;
    subNode = node->children[0][0][0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[0][0][1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y++;
    subNode = node->children[0][1][1];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[0][1][0];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.x++;
    subNode = node->children[1][1][0];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[1][1][1];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y--;
    subNode = node->children[1][0][1];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[1][0][0];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_help);
#ifdef SKIP_ASM
extern "C" void TerrainCallback_intersectpatch(void* ctx, void* obj);
extern "C" void TerrainCallback_intersectinstance(void* ctx, void* obj);
extern "C" void nash_Cell_descend4(nash_Cell* self, void* ctx);

static inline int Classify_DBE8(VerticalLineProximityTest* test, nash_CellKey* c)
{
    union { int i; float f; } bits;  // 2^level built from the float exponent field
    bits.i = (c->level + 0x7F) << 23;
    float s = bits.f;
    const t4Vector* p = test->position;
    if (p->x < ((float)c->x - 0.2f) * s) {
        return 1;
    }
    if (((float)(c->x + 1) + 0.2f) * s < p->x) {
        return 1;
    }
    if (p->y < ((float)c->y - 0.2f) * s) {
        return 1;
    }
    if (((float)(c->y + 1) + 0.2f) * s < p->y) {
        return 1;
    }
    return 2;
}

/**
 * Walks an octree cell for the vertical-line terrain query: tests the cell against the line, then its children.
 *
 * Classifies the cell (level, x, y, z at @p cell; size 2^level, widened by 0.2 cell) against the vertical line through
 * the point @p test holds (position): outside when the point's x or y lies outside the cell's span, crossing otherwise.
 * The test never answers inside (0), so the branch that walks the cell whole with @p insideQuery
 * (TerrainCallback_intersectpatch, TerrainCallback_intersectinstance, nash_Cell_descend4) is dead. A crossing cell
 * feeds its patch list (nodesinside[1]) to TerrainCallback_intersectpatch and its instance list (nodesinside[0]) to
 * TerrainCallback_intersectinstance with @p query, stops when the level is 11, and otherwise recurses into each
 * existing child with its key, in the order 000, 100, 110, 010, 011, 111, 101, 001 (x, y, z). Called by
 * cBXNash_getGroundHeight for each root cell.
 *
 * @param query  the terrain query for crossing cells (On Tour oncross)
 * @param insideQuery  the terrain query for contained cells (On Tour oninside)
 * @param test  the line test: a pointer to the point (On Tour VerticalLineProximityTest)
 * @param node  the cell (On Tour const Cell *)
 * @param cell  the cell's key: level, x, y, z (On Tour CellKey)
 */
void nash_privy_QueryHelper::help(TerrainCallback* query, TerrainCallback* insideQuery,
                                  VerticalLineProximityTest* test, nash_Cell* node, nash_CellKey* cell)
{
    int overlap = Classify_DBE8(test, cell);
    if (overlap == 0) {
        nash_Node* objNode;
        for (objNode = node->nodesinside[1].head; objNode != 0; objNode = objNode->next) {
            TerrainCallback_intersectpatch(insideQuery, objNode);
        }
        for (objNode = node->nodesinside[0].head; objNode != 0; objNode = objNode->next) {
            TerrainCallback_intersectinstance(insideQuery, objNode);
        }
        if (node->children[0][0][0]) {
            node->children[0][0][0]->descend4(insideQuery);
        }
        if (node->children[0][0][1]) {
            node->children[0][0][1]->descend4(insideQuery);
        }
        if (node->children[0][1][0]) {
            node->children[0][1][0]->descend4(insideQuery);
        }
        if (node->children[0][1][1]) {
            node->children[0][1][1]->descend4(insideQuery);
        }
        if (node->children[1][0][0]) {
            node->children[1][0][0]->descend4(insideQuery);
        }
        if (node->children[1][0][1]) {
            node->children[1][0][1]->descend4(insideQuery);
        }
        if (node->children[1][1][0]) {
            node->children[1][1][0]->descend4(insideQuery);
        }
        if (node->children[1][1][1]) {
            node->children[1][1][1]->descend4(insideQuery);
        }
    } else if (overlap == kNash_Crossing) {
        nash_Node* objNode;
        for (objNode = node->nodesinside[1].head; objNode != 0; objNode = objNode->next) {
            TerrainCallback_intersectpatch(query, objNode);
        }
        for (objNode = node->nodesinside[0].head; objNode != 0; objNode = objNode->next) {
            TerrainCallback_intersectinstance(query, objNode);
        }
        if (cell->level == nash_CellKey::kLeafLevel) {
            return;
        }
        nash_CellKey sub;
        sub.level = cell->level - 1;
        sub.x = cell->x * 2;
        sub.y = cell->y * 2;
        sub.z = cell->z * 2;
        if (node->children[0][0][0]) {
            help(query, insideQuery, test, node->children[0][0][0], &sub);
        }
        sub.x++;
        if (node->children[1][0][0]) {
            help(query, insideQuery, test, node->children[1][0][0], &sub);
        }
        sub.y++;
        if (node->children[1][1][0]) {
            help(query, insideQuery, test, node->children[1][1][0], &sub);
        }
        sub.x--;
        if (node->children[0][1][0]) {
            help(query, insideQuery, test, node->children[0][1][0], &sub);
        }
        sub.z++;
        if (node->children[0][1][1]) {
            help(query, insideQuery, test, node->children[0][1][1], &sub);
        }
        sub.x++;
        if (node->children[1][1][1]) {
            help(query, insideQuery, test, node->children[1][1][1], &sub);
        }
        sub.y--;
        if (node->children[1][0][1]) {
            help(query, insideQuery, test, node->children[1][0][1], &sub);
        }
        sub.x--;
        if (node->children[0][0][1]) {
            help(query, insideQuery, test, node->children[0][0][1], &sub);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX6);
#ifdef SKIP_ASM


extern "C" void nash_Cell_descend5(nash_Cell* self, void* ctx);

static inline void collect_33DFD8(nash_Cell* node, StaticObjectCallback* ctx)
{
    nash_Node* p;
    for (p = node->nodesinside[0].head; p != 0; p = p->next) {
        StaticObjectCallback_intersectinstance(ctx, p);
    }
    if (node->children[0][0][0] != 0) node->children[0][0][0]->descend5(ctx);
    if (node->children[0][0][1] != 0) node->children[0][0][1]->descend5(ctx);
    if (node->children[0][1][0] != 0) node->children[0][1][0]->descend5(ctx);
    if (node->children[0][1][1] != 0) node->children[0][1][1]->descend5(ctx);
    if (node->children[1][0][0] != 0) node->children[1][0][0]->descend5(ctx);
    if (node->children[1][0][1] != 0) node->children[1][0][1]->descend5(ctx);
    if (node->children[1][1][0] != 0) node->children[1][1][0]->descend5(ctx);
    if (node->children[1][1][1] != 0) node->children[1][1][1]->descend5(ctx);
}

/**
 * Walks a crossing octree cell for the Nash static-object query: intersects its instances, then visits its children.
 *
 * Feeds the cell's instance list (@p vnode +0x20) to StaticObjectCallback_intersectinstance with @p ctx and stops when the cell's level is 11.
 * Otherwise splits the cell (level, x, y, z at @p vcell; size 2^level) into its eight children one level down, each
 * widened by 0.2 child cell on every side. Each existing child that overlaps @p vbox on all three axes is walked whole
 * with @p ctx2 (its instances, then nash_Cell_descend5 for its children) when the box contains it, and recursed into
 * otherwise, in the key order 000, 001, 011, 010, 110, 111, 101, 100. Called by cBXNash_staticObjectIntersect for crossing root cells.
 *
 * @param flag  the empty helper object, only passed on to the recursion (On Tour In *this)
 * @param ctx  the static-object query for crossing cells (On Tour oncross)
 * @param ctx2  the static-object query for contained cells (On Tour oninside)
 * @param vbox  the query box: min at +0x0, max at +0x10
 * @param vnode  the cell (On Tour const Cell *)
 * @param vcell  the cell's key: level, x, y, z (On Tour CellKey)
 */
extern "C" void nash_privy_QueryHelper_helpX6(char* flag, StaticObjectCallback* ctx, StaticObjectCallback* ctx2, tBoundBox* vbox, void* vnode, void* vcell)
{
    tBoundBox* box = (tBoundBox*)vbox;
    nash_Cell* node = (nash_Cell*)vnode;
    nash_CellKey* cell = (nash_CellKey*)vcell;
    nash_Node* objNode;
    for (objNode = node->nodesinside[0].head; objNode != 0; objNode = objNode->next) {
        StaticObjectCallback_intersectinstance(ctx, objNode);
    }
    if (cell->level == nash_CellKey::kLeafLevel) {
        return;
    }
    nash_CellKey sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    union { int i; float f; } bits;  // 2^level built from the float exponent field
    bits.i = (sub.level + 0x7F) << 23;
    float cellSize = bits.f;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->MinPoint.x < x0hi && x0lo < box->MaxPoint.x) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->MinPoint.x < x1hi && x1lo < box->MaxPoint.x) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->MinPoint.y < y0hi && y0lo < box->MaxPoint.y) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->MinPoint.y < y1hi && y1lo < box->MaxPoint.y) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->MinPoint.z < z0hi && z0lo < box->MaxPoint.z) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->MinPoint.z < z1hi && z1lo < box->MaxPoint.z) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->MinPoint.x < x0lo && x0hi < box->MaxPoint.x) cx0 = 1;
    int cx1 = 0;
    if (box->MinPoint.x < x1lo && x1hi < box->MaxPoint.x) cx1 = 1;
    int cy0 = 0;
    if (box->MinPoint.y < y0lo && y0hi < box->MaxPoint.y) cy0 = 1;
    int cy1 = 0;
    if (box->MinPoint.y < y1lo && y1hi < box->MaxPoint.y) cy1 = 1;
    int cz0 = 0;
    if (box->MinPoint.z < z0lo && z0hi < box->MaxPoint.z) cz0 = 1;
    int cz1 = 0;
    if (box->MinPoint.z < z1lo && z1hi < box->MaxPoint.z) cz1 = 1;

    nash_Cell* subNode;
    subNode = node->children[0][0][0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[0][0][1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y++;
    subNode = node->children[0][1][1];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[0][1][0];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.x++;
    subNode = node->children[1][1][0];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[1][1][1];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y--;
    subNode = node->children[1][0][1];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[1][0][0];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX7);
#ifdef SKIP_ASM



// PORT: alias; the unit defines nash_Cell_descend6 later with its own struct types.
extern "C" void nash_Cell_descend6_33ED60(nash_Cell* tree, SplineCallback* q) __asm__("nash_Cell_descend6");

static inline void objs_33ED60(cTypedSpaceNode* o, SplineCallback* q)
{
    for (; o != 0; o = (cTypedSpaceNode*)o->next) {
        int type = o->type;
        if (type == kSNT_Rail) {
            cRailModifier_splineIntersect(((cRailSpaceNode*)o)->Rail, ((tBoundBox*)q->box), ((tBoundBox*)q->box),
                                          ((t4Vector*)q->intersectpos), ((int)q->splinefiltermask), q->hit,
                                          q->intersectdist, q->intersectinfo);
        } else if (type == kSNT_FlexRail) {
            cFlexRailNode_splineIntersect(((cFlexRailSpaceNode*)o)->FlexNode, ((tBoundBox*)q->box),
                                          ((tBoundBox*)q->box), ((t4Vector*)q->intersectpos),
                                          ((int)q->splinefiltermask), q->hit, q->intersectdist, q->intersectinfo);
        } else if (type == kSNT_SplineSegment) {
            if (((tSplineSegmentRecord*)o)->m_Parent->m_iFlags & ((int)q->splinefiltermask)) {
                intersect_staticSplineSegmentIntersect(o, ((tBoundBox*)q->box), ((t4Vector*)q->intersectpos), q->hit,
                                                       q->intersectdist, q->intersectinfo);
            }
        }
    }
}

static inline void collect_33ED60(nash_Cell* node, SplineCallback* q)
{
    objs_33ED60((cTypedSpaceNode*)node->nodesinside[2].head, q);
    if (node->children[0][0][0] != 0) nash_Cell_descend6_33ED60(node->children[0][0][0], q);
    if (node->children[0][0][1] != 0) nash_Cell_descend6_33ED60(node->children[0][0][1], q);
    if (node->children[0][1][0] != 0) nash_Cell_descend6_33ED60(node->children[0][1][0], q);
    if (node->children[0][1][1] != 0) nash_Cell_descend6_33ED60(node->children[0][1][1], q);
    if (node->children[1][0][0] != 0) nash_Cell_descend6_33ED60(node->children[1][0][0], q);
    if (node->children[1][0][1] != 0) nash_Cell_descend6_33ED60(node->children[1][0][1], q);
    if (node->children[1][1][0] != 0) nash_Cell_descend6_33ED60(node->children[1][1][0], q);
    if (node->children[1][1][1] != 0) nash_Cell_descend6_33ED60(node->children[1][1][1], q);
}

struct sQry_337A48;

/**
 * Runs the spline query on an octree cell that crosses the query box, recursing into its children.
 *
 * First applies the crossing callback (@p vctx) to the cell's typed-node list (+0x28): rails (type 2) go to
 * cRailModifier_splineIntersect, flex rails (type 3) to cFlexRailNode_splineIntersect, and instances (type 1) whose filter word (node
 * +0x68, then +0x1C) shares a bit with the query mask to intersect_staticSplineSegmentIntersect; hits go to the query's count and distance.
 * Cells at level 11 stop there.
 *
 * Otherwise the children are 2^(level - 1) units wide; for each axis it checks which of the two child spans, widened by
 * 0.2 cell on both sides, overlap @p vbox (returning when neither does) and which lie wholly inside it. It then visits
 * the eight children in the order 0, 1, 3, 2, 6, 7, 5, 4, keeping the child key up to date: an overlapping child wholly
 * inside the box gets the inside callback (@p vctx2) on its own list and nash_Cell_descend6 on each of its children; any
 * other overlapping child is handled by a recursive call. The same walk as nash_privy_QueryHelper_helpX with the spline
 * callback inlined.
 *
 * Called by cBXNash_splineIntersect for the root cells that cross the box.
 *
 * @param flag  the query helper object (this); unused, the caller passes the address of an empty local
 * @param vctx  the crossing callback (On Tour SplineCallback): position, intInfo, mask, hit-count and distance
 *   pointers, box
 * @param vctx2  the callback for children wholly inside the box (the caller passes the same object)
 * @param vbox  the query box (min, max)
 * @param vnode  the octree cell (On Tour nash::Cell: children at +0x0, node lists from +0x20)
 * @param vcell  the cell's key: level and x, y, z cell coordinates (On Tour CellKey)
 */
// Declared as cBXNash_splineIntersect (its caller) declares it; sQry_337A48 has sQuery_33ED60's layout.
extern "C" void nash_privy_QueryHelper_helpX7(char* flag, SplineCallback* vctx, SplineCallback* vctx2, tBoundBox* vbox, void* vnode, void* vcell)
{
    SplineCallback* ctx = (SplineCallback*)vctx;
    SplineCallback* ctx2 = (SplineCallback*)vctx2;
    tBoundBox* box = (tBoundBox*)vbox;
    nash_Cell* node = (nash_Cell*)vnode;
    nash_CellKey* cell = (nash_CellKey*)vcell;
    objs_33ED60((cTypedSpaceNode*)node->nodesinside[2].head, ctx);
    if (cell->level == nash_CellKey::kLeafLevel) {
        return;
    }
    nash_CellKey sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    union { int i; float f; } bits;  // 2^level built from the float exponent field
    bits.i = (sub.level + 0x7F) << 23;
    float cellSize = bits.f;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->MinPoint.x < x0hi && x0lo < box->MaxPoint.x) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->MinPoint.x < x1hi && x1lo < box->MaxPoint.x) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->MinPoint.y < y0hi && y0lo < box->MaxPoint.y) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->MinPoint.y < y1hi && y1lo < box->MaxPoint.y) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->MinPoint.z < z0hi && z0lo < box->MaxPoint.z) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->MinPoint.z < z1hi && z1lo < box->MaxPoint.z) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->MinPoint.x < x0lo && x0hi < box->MaxPoint.x) cx0 = 1;
    int cx1 = 0;
    if (box->MinPoint.x < x1lo && x1hi < box->MaxPoint.x) cx1 = 1;
    int cy0 = 0;
    if (box->MinPoint.y < y0lo && y0hi < box->MaxPoint.y) cy0 = 1;
    int cy1 = 0;
    if (box->MinPoint.y < y1lo && y1hi < box->MaxPoint.y) cy1 = 1;
    int cz0 = 0;
    if (box->MinPoint.z < z0lo && z0hi < box->MaxPoint.z) cz0 = 1;
    int cz1 = 0;
    if (box->MinPoint.z < z1lo && z1hi < box->MaxPoint.z) cz1 = 1;

    nash_Cell* subNode;
    subNode = node->children[0][0][0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[0][0][1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.y++;
    subNode = node->children[0][1][1];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[0][1][0];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.x++;
    subNode = node->children[1][1][0];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z++;
    subNode = node->children[1][1][1];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.y--;
    subNode = node->children[1][0][1];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z--;
    subNode = node->children[1][0][0];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", func_003400D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// A 16-byte vector global constructed from four floats.
struct sV4_3400D8 {
    float x, y, z, w;
    sV4_3400D8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
    // Placement form without throw(): g++ then adds no null check (the original was a global's ctor call).
    void* operator new(unsigned int, void* p) { return p; }
};

extern sV4_3400D8 D_004FF2C0_v __asm__("D_004FF2C0");
extern sV4_3400D8 D_004FF2D0_v __asm__("D_004FF2D0");
extern sV4_3400D8 D_004FF2E0_v __asm__("D_004FF2E0");
extern sV4_3400D8 D_004FF2F0_v __asm__("D_004FF2F0");
extern sV4_3400D8 D_004FF300_v __asm__("D_004FF300");
extern sV4_3400D8 D_004FF310_v __asm__("D_004FF310");
extern sV4_3400D8 D_004FF320_v __asm__("D_004FF320");
extern sV4_3400D8 D_004FF330_v __asm__("D_004FF330");
extern sV4_3400D8 D_004FF340_v __asm__("D_004FF340");
extern sV4_3400D8 D_004FF350_v __asm__("D_004FF350");
extern sV4_3400D8 D_004FF360_v __asm__("D_004FF360");
extern sV4_3400D8 D_004FF370_v __asm__("D_004FF370");
extern sV4_3400D8 D_004FF380_v __asm__("D_004FF380");
extern sV4_3400D8 D_004FF390_v __asm__("D_004FF390");
extern sV4_3400D8 D_004FF3A0_v __asm__("D_004FF3A0");
extern sV4_3400D8 D_004FF3B0_v __asm__("D_004FF3B0");
extern sV4_3400D8 D_004FF3C0_v __asm__("D_004FF3C0");
extern sV4_3400D8 D_004FF3D0_v __asm__("D_004FF3D0");
extern sV4_3400D8 D_004FF3E0_v __asm__("D_004FF3E0");
extern sV4_3400D8 D_004FF3F0_v __asm__("D_004FF3F0");
extern sV4_3400D8 D_004FF400_v __asm__("D_004FF400");
extern sV4_3400D8 D_004FF410_v __asm__("D_004FF410");
extern sV4_3400D8 D_004FF420_v __asm__("D_004FF420");
extern sV4_3400D8 D_004FF430_v __asm__("D_004FF430");
extern sV4_3400D8 D_004FF440_v __asm__("D_004FF440");
extern sV4_3400D8 D_004FF450_v __asm__("D_004FF450");
extern sV4_3400D8 D_004FF460_v __asm__("D_004FF460");
extern sV4_3400D8 D_004FF470_v __asm__("D_004FF470");
extern sV4_3400D8 D_004FF480_v __asm__("D_004FF480");
extern sV4_3400D8 D_004FF490_v __asm__("D_004FF490");
extern sV4_3400D8 D_004FF4A0_v __asm__("D_004FF4A0");
extern sV4_3400D8 D_004FF4B0_v __asm__("D_004FF4B0");
extern sV4_3400D8 D_004FF4C0_v __asm__("D_004FF4C0");
extern sV4_3400D8 D_004FF4D0_v __asm__("D_004FF4D0");
extern sV4_3400D8 D_004FF4E0_v __asm__("D_004FF4E0");
extern sV4_3400D8 D_004FF4F0_v __asm__("D_004FF4F0");
extern sV4_3400D8 D_004FF500_v __asm__("D_004FF500");
extern sV4_3400D8 D_004FF510_v __asm__("D_004FF510");
extern sV4_3400D8 D_004FF520_v __asm__("D_004FF520");
extern sV4_3400D8 D_004FF530_v __asm__("D_004FF530");
extern sV4_3400D8 D_004FF540_v __asm__("D_004FF540");
extern sV4_3400D8 D_004FF550_v __asm__("D_004FF550");
extern sV4_3400D8 D_004FF560_v __asm__("D_004FF560");
extern sV4_3400D8 D_004FF570_v __asm__("D_004FF570");
extern sV4_3400D8 D_004FF580_v __asm__("D_004FF580");
extern sV4_3400D8 D_004FF590_v __asm__("D_004FF590");
extern sV4_3400D8 D_004FF5A0_v __asm__("D_004FF5A0");
extern sV4_3400D8 D_004FF5B0_v __asm__("D_004FF5B0");
extern sV4_3400D8 D_004FF5C0_v __asm__("D_004FF5C0");
extern sV4_3400D8 D_004FF5D0_v __asm__("D_004FF5D0");
extern sV4_3400D8 D_004FF5E0_v __asm__("D_004FF5E0");
extern sV4_3400D8 D_004FF5F0_v __asm__("D_004FF5F0");
extern sV4_3400D8 D_004FF600_v __asm__("D_004FF600");
extern sV4_3400D8 D_004FF610_v __asm__("D_004FF610");
extern sV4_3400D8 D_004FF620_v __asm__("D_004FF620");
extern sV4_3400D8 D_004FF630_v __asm__("D_004FF630");
// gSphTreeAlignAxis: the eight corners (+-1, +-1, +-1, 0), written as one 32-float block.
struct sCorners_3400D8 {
    float v[32];
    void init()
    {
        v[0] = -1.0f;
        v[1] = -1.0f;
        v[2] = -1.0f;
        v[3] = 0.0f;
        v[4] = -1.0f;
        v[5] = -1.0f;
        v[6] = 1.0f;
        v[7] = 0.0f;
        v[8] = -1.0f;
        v[9] = 1.0f;
        v[10] = -1.0f;
        v[11] = 0.0f;
        v[12] = -1.0f;
        v[13] = 1.0f;
        v[14] = 1.0f;
        v[15] = 0.0f;
        v[16] = 1.0f;
        v[17] = -1.0f;
        v[18] = -1.0f;
        v[19] = 0.0f;
        v[20] = 1.0f;
        v[21] = -1.0f;
        v[22] = 1.0f;
        v[23] = 0.0f;
        v[24] = 1.0f;
        v[25] = 1.0f;
        v[26] = -1.0f;
        v[27] = 0.0f;
        v[28] = 1.0f;
        v[29] = 1.0f;
        v[30] = 1.0f;
        v[31] = 0.0f;
    }
};
extern sCorners_3400D8 gSphTreeAlignAxis_c __asm__("gSphTreeAlignAxis");


// Static initialisation of the unit's vector globals D_004FF2C0..D_004FF630 and the corner table gSphTreeAlignAxis
// (g++ __static_initialization_and_destruction_0).
// The unit declares func_003400D8 returning void*; bind this definition by asm label.
extern "C" void func_003400D8_init(int initialize, int priority) __asm__("func_003400D8");
/**
 * Constructs the unit's static constants: 56 colours and the eight unit-cube corners.
 *
 * Runs only for @p initialize non-zero and @p priority 0xFFFF (func_00341368 passes 1, 0xFFFF). The colours
 * (D_004FF2C0..D_004FF630, four floats each) are the table also built in pathsys, memstd and all_in_one_scripter;
 * gSphTreeAlignAxis gets the corners (+-1, +-1, +-1, 0) in x-major order, the default axes riderspheretree's sphere-tree code
 * copies (g++ __static_initialization_and_destruction_0).
 *
 * @param initialize  non-zero to construct
 * @param priority  the init priority; only 0xFFFF does anything
 */
extern "C" void func_003400D8_init(int initialize, int priority)
{
    if (priority == DEFAULT_INIT_PRIORITY && initialize != 0) {
        new (&D_004FF2C0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 0.5f);
        new (&D_004FF2D0_v) sV4_3400D8(0.4998680055141449f, 1.0f, 1.0f, 0.0f);
        new (&D_004FF2E0_v) sV4_3400D8(0.12951700389385223f, 1.0f, 1.0f, 0.0f);
        new (&D_004FF2F0_v) sV4_3400D8(0.22426900267601013f, 0.9402310252189636f, 0.5425059795379639f, 0.0004030000127386302f);
        new (&D_004FF300_v) sV4_3400D8(0.4652079939842224f, 0.32170501351356506f, 0.9943439960479736f, 0.0f);
        new (&D_004FF310_v) sV4_3400D8(0.17792600393295288f, 1.0f, 1.0f, 0.0f);
        new (&D_004FF320_v) sV4_3400D8(0.4411740005016327f, 0.41388601064682007f, 0.3211260139942169f, 0.4398239850997925f);
        new (&D_004FF330_v) sV4_3400D8(0.500997006893158f, 0.24849599599838257f, 1.0f, 1.0f);
        new (&D_004FF340_v) sV4_3400D8(0.27687299251556396f, 0.6395620107650757f, 0.13423900306224823f, 0.0f);
        new (&D_004FF350_v) sV4_3400D8(0.8936439752578735f, 0.5263469815254211f, 0.5051450133323669f, 0.8687300086021423f);
        new (&D_004FF360_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF370_v) sV4_3400D8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF380_v) sV4_3400D8(1.0f, 0.550000011920929f, 0.5350000262260437f, 0.550000011920929f);
        new (&D_004FF390_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF3A0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF3B0_v) sV4_3400D8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF3C0_v) sV4_3400D8(1.0f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF3D0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF3E0_v) sV4_3400D8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FF3F0_v) sV4_3400D8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FF400_v) sV4_3400D8(1.0f, 0.5f, 1.0f, 0.5f);
        new (&D_004FF410_v) sV4_3400D8(1.0f, 0.7828119993209839f, 0.06171900033950806f, 0.03984399884939194f);
        new (&D_004FF420_v) sV4_3400D8(1.0f, 1.0f, 0.7988259792327881f, 0.0f);
        new (&D_004FF430_v) sV4_3400D8(0.9886710047721863f, 0.9230499863624573f, 0.34311699867248535f, 0.0f);
        new (&D_004FF440_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF450_v) sV4_3400D8(1.0f, 1.0f, 0.7976430058479309f, 0.0f);
        new (&D_004FF460_v) sV4_3400D8(1.0f, 0.9222670197486877f, 0.34179699420928955f, 0.0f);
        new (&D_004FF470_v) sV4_3400D8(1.0f, 0.7847639918327332f, 0.06054700165987015f, 0.04062499850988388f);
        new (&D_004FF480_v) sV4_3400D8(1.0f, 0.800000011920929f, 0.44999998807907104f, 1.0f);
        new (&D_004FF490_v) sV4_3400D8(1.0f, 0.3281260132789612f, 1.0f, 0.3539060056209564f);
        new (&D_004FF4A0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF4B0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF4C0_v) sV4_3400D8(1.0f, 0.8613290190696716f, 0.38124799728393555f, 0.0f);
        new (&D_004FF4D0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF4E0_v) sV4_3400D8(1.0f, 0.3244380056858063f, 1.0f, 0.0f);
        new (&D_004FF4F0_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF500_v) sV4_3400D8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FF510_v) sV4_3400D8(0.7007060050964355f, 0.0f, 0.0f, 0.0f);
        new (&D_004FF520_v) sV4_3400D8(1.0f, 0.7828530073165894f, 0.06140600144863129f, 0.04146299883723259f);
        new (&D_004FF530_v) sV4_3400D8(1.0f, 0.32487601041793823f, 1.0f, 0.35280099511146545f);
        new (&D_004FF540_v) sV4_3400D8(1.0f, 1.0f, 0.7977359890937805f, 0.0f);
        new (&D_004FF550_v) sV4_3400D8(1.0f, 0.4937489926815033f, 1.0f, 0.49179700016975403f);
        new (&D_004FF560_v) sV4_3400D8(1.0f, 1.0f, 1.0f, 1.0f);
        new (&D_004FF570_v) sV4_3400D8(1.0f, 0.800000011920929f, 1.0f, 0.800000011920929f);
        new (&D_004FF580_v) sV4_3400D8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FF590_v) sV4_3400D8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FF5A0_v) sV4_3400D8(1.0f, 1.0f, 0.0f, 0.0f);
        new (&D_004FF5B0_v) sV4_3400D8(1.0f, 0.3273420035839081f, 1.0f, 0.35390499234199524f);
        new (&D_004FF5C0_v) sV4_3400D8(1.0f, 1.0f, 0.061000000685453415f, 0.039000000804662704f);
        new (&D_004FF5D0_v) sV4_3400D8(1.0f, 1.0f, 0.7999979853630066f, 0.0f);
        new (&D_004FF5E0_v) sV4_3400D8(1.0f, 0.9589840173721313f, 0.6143789887428284f, 0.0f);
        new (&D_004FF5F0_v) sV4_3400D8(1.0f, 0.9285150170326233f, 0.3399839997291565f, 0.0f);
        new (&D_004FF600_v) sV4_3400D8(1.0f, 0.8738250136375427f, 0.24931700527668f, 0.017578000202775f);
        new (&D_004FF610_v) sV4_3400D8(1.0f, 0.7843739986419678f, 0.06132800132036209f, 0.03945299983024597f);
        new (&D_004FF620_v) sV4_3400D8(1.0f, 0.0f, 1.0f, 0.0f);
        new (&D_004FF630_v) sV4_3400D8(1.0f, 1.0f, 0.0f, 0.0f);
        gSphTreeAlignAxis_c.init();
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", tAIFwdDiff_tAIFwdDiff);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// The July 2003 prototype names this function tAIFwdDiff::tAIFwdDiff() (an out-of-line copy of the inline ctor).
struct tAIFwdDiffElem {
    float v;
    tAIFwdDiffElem() {}
};
class tAIFwdDiff {
public:
    tAIFwdDiffElem a[10][10];
    tAIFwdDiffElem b[9][9];
    tAIFwdDiffElem c[3][3];
    tAIFwdDiff();
};

/**
 * Constructs a tAIFwdDiff (an AI forward-difference table): there is nothing to initialise.
 *
 * The element constructors are empty, so all that is left are three empty counting loops over the rows of the 10x10,
 * 9x9 and 3x3 tables (a, b, c). cAIFwdDiffCache_Init calls it on each entry of a newly allocated cache.
 */
tAIFwdDiff::tAIFwdDiff()
{
}
// PORT: the compiler names this ctor __10tAIFwdDiff; expose it under the address-derived name.
__asm__(".globl tAIFwdDiff_tAIFwdDiff\n.type tAIFwdDiff_tAIFwdDiff,@function\ntAIFwdDiff_tAIFwdDiff = __10tAIFwdDiff\n.size tAIFwdDiff_tAIFwdDiff,0x98\n");
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldIntersectType_IntersectSubPatch__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base cWorldIntersectType default for the virtual slot cWorldLine fills with
 * cWorldLine_IntersectSubPatch.
 *
 * Slot 8 of cWorldRider_vtbl, cWorldSphTree_vtbl and cWorldSphere_vtbl (between IsBoxInt and IsInBoxTransformed), which
 * inherit it.
 */
void cWorldIntersectType_IntersectSubPatch(void* vself)
{
    cWorldIntersectType* self = (cWorldIntersectType*)vself;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldLine_GetIdealParam__FPv);
#ifdef SKIP_ASM
/**
 * Returns the world line's capsule radius.
 *
 * Unlike the sphere and rider versions it neither doubles the value nor checks the active word.
 *
 * @return the radius stored at offset 0xA0 by the line's constructor
 */
float cWorldLine::GetIdealParam()
{
    return idealParam;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphere_GetIdealParam);
#ifdef SKIP_ASM
/**
 * Returns the world sphere's diameter, or -1 when the sphere is inactive.
 *
 * The sphere's constructor sets the first word to 1 unless it is told otherwise.
 *
 * @return 2 * the radius at offset 0x70, or -1.0 if the first word is 0
 */
float cWorldSphere::GetIdealParam()
{
    if (*(int*)this == 0) {
        return -1.0f;
    }
    return rad * 2.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend);
#ifdef SKIP_ASM

/**
 * Collects the overlapping instances of an octree cell and of every cell below it.
 *
 * Runs OverlapInstancesCallback_operatorCall(@p ctx, @p node) on the cell, then calls itself on each non-null child
 * (+0x0..+0x1C). Used for cells that lie wholly inside the query box (cOverlapList_getinstances).
 *
 * @param ctx  the instance callback: list, count pointer, maximum, query box (On Tour OverlapInstancesCallback)
 */
void nash_Cell::descend(OverlapInstancesCallback* ctx)
{
    ctx->operatorCall(this);
    if (children[0][0][0] != 0) children[0][0][0]->descend(ctx);
    if (children[0][0][1] != 0) children[0][0][1]->descend(ctx);
    if (children[0][1][0] != 0) children[0][1][0]->descend(ctx);
    if (children[0][1][1] != 0) children[0][1][1]->descend(ctx);
    if (children[1][0][0] != 0) children[1][0][0]->descend(ctx);
    if (children[1][0][1] != 0) children[1][0][1]->descend(ctx);
    if (children[1][1][0] != 0) children[1][1][0]->descend(ctx);
    if (children[1][1][1] != 0) children[1][1][1]->descend(ctx);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend1);
#ifdef SKIP_ASM
struct sOctCollect {
    void** arr;
    int* count;
};

/**
 * Appends every typed node of an octree cell and of every cell below it to a list.
 *
 * Walks the cell's typed-node list (+0x28) appending each node at @p out arr[*count] and incrementing the count (no
 * limit check), then calls itself on each non-null child (+0x0..+0x1C). cOverlapList_gettyped uses it for cells wholly inside
 * the query box; the list it fills (+0x210 count, +0x214 array) is the one cOverlapList_splineIntersect reads.
 *
 * @param out  the output list: node array and count pointer
 */
extern "C" void nash_Cell_descend1(nash_Cell* self, sOctCollect* out)
{
    nash_Node* objNode;
    for (objNode = self->nodesinside[2].head; objNode != 0; objNode = objNode->next) {
        out->arr[(*out->count)++] = objNode;
    }
    if (self->children[0][0][0]) nash_Cell_descend1(self->children[0][0][0], out);
    if (self->children[0][0][1]) nash_Cell_descend1(self->children[0][0][1], out);
    if (self->children[0][1][0]) nash_Cell_descend1(self->children[0][1][0], out);
    if (self->children[0][1][1]) nash_Cell_descend1(self->children[0][1][1], out);
    if (self->children[1][0][0]) nash_Cell_descend1(self->children[1][0][0], out);
    if (self->children[1][0][1]) nash_Cell_descend1(self->children[1][0][1], out);
    if (self->children[1][1][0]) nash_Cell_descend1(self->children[1][1][0], out);
    if (self->children[1][1][1]) nash_Cell_descend1(self->children[1][1][1], out);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend2);
#ifdef SKIP_ASM
static inline int overlapObj_340C20(const tBoundBox* b, const tPatchRecord* n)
{
    int r = 0;
    if (b->MinPoint.x <= n->m_fvMax.m[0] && b->MaxPoint.x >= n->m_fvMin.m[0] &&
        b->MinPoint.y <= n->m_fvMax.m[1] && b->MaxPoint.y >= n->m_fvMin.m[1] &&
        b->MinPoint.z <= n->m_fvMax.m[2] && b->MaxPoint.z >= n->m_fvMin.m[2]) {
        r = 1;
    }
    return r;
}

/**
 * Collects the overlapping instances and patches of an octree cell and of every cell below it.
 *
 * Runs OverlapInstancesCallback_operatorCall on the cell with the instance part of @p ctx, then appends each patch on
 * the cell's patch list (nodesinside[1]) whose box (m_fvMin, m_fvMax) overlaps the query box to the patch part (the
 * OverlapPatchesAndInstancesCallback's patchescallback: list, count pointer, box), and calls itself on each non-null
 * child. Used by cOverlapList_getpatchesandinstances and nash_privy_QueryHelper_helpX3 for cells wholly inside the
 * query box.
 *
 * @param ctx  the callback: instance part (instancecallback) and patch part (patchescallback)
 */
void nash_Cell::descend2(OverlapInstancesCallback* ctx)
{
    ctx->operatorCall(this);
    // ctx is the instance part of an OverlapPatchesAndInstancesCallback; its patch part follows
    OverlapPatchesCallback* objCtx = &((OverlapPatchesAndInstancesCallback*)ctx)->patchescallback;
    for (nash_Node* node = nodesinside[1].head; node != 0; node = node->next) {
        tPatchRecord* obj = (tPatchRecord*)node;  // a patch record starts with its list link
        if (overlapObj_340C20(objCtx->box, obj)) {
            unsigned int* cnt = objCtx->number;
            int n = *cnt;
            objCtx->patches[n] = obj;
            *cnt = n + 1;
        }
    }
    if (children[0][0][0] != 0) children[0][0][0]->descend2(ctx);
    if (children[0][0][1] != 0) children[0][0][1]->descend2(ctx);
    if (children[0][1][0] != 0) children[0][1][0]->descend2(ctx);
    if (children[0][1][1] != 0) children[0][1][1]->descend2(ctx);
    if (children[1][0][0] != 0) children[1][0][0]->descend2(ctx);
    if (children[1][0][1] != 0) children[1][0][1]->descend2(ctx);
    if (children[1][1][0] != 0) children[1][1][0]->descend2(ctx);
    if (children[1][1][1] != 0) children[1][1][1]->descend2(ctx);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend3);
#ifdef SKIP_ASM
struct sCtx_3309D8;
extern "C" void OverlapInstancesCallback_operatorCall(OverlapInstancesCallback* ctx, nash_Cell* tree);




static inline void addPatch_340DC0(OverlapPatchesCallback* s, tPatchRecord* p)
{
    unsigned int* cnt = s->number;
    int n = *cnt;
    s->patches[n] = p;
    *cnt = n + 1;
}

static inline void addTyped_340DC0(OverlapTypedCallback* s, cTypedSpaceNode* p)
{
    unsigned int* cnt = s->number;
    int n = *cnt;
    s->typed[n] = p;
    *cnt = n + 1;
}

static inline int overlap_340DC0(const tBoundBox* b, const tPatchRecord* n)
{
    int r = 0;
    if (b->MinPoint.x <= n->m_fvMax.m[0] && b->MaxPoint.x >= n->m_fvMin.m[0] &&
        b->MinPoint.y <= n->m_fvMax.m[1] && b->MaxPoint.y >= n->m_fvMin.m[1] &&
        b->MinPoint.z <= n->m_fvMax.m[2] && b->MaxPoint.z >= n->m_fvMin.m[2]) {
        r = 1;
    }
    return r;
}

/**
 * Collects every instance, patch and typed node of an octree cell and of every cell below it.
 *
 * Runs OverlapInstancesCallback_operatorCall with the instance part of @p ctx (instancecallback), appends the patches
 * on the nodesinside[1] list whose box (m_fvMin / m_fvMax) overlaps the patch part's box to that part
 * (patchescallback), appends every typed node (nodesinside[2]) to the third part (typedcallback) unconditionally, then
 * calls itself on each non-null child. Used by cOverlapList_geteverything for cells wholly inside the query box.
 *
 * @param ctx  the callback: instance, patch and typed-node parts, each list, count pointer, box
 */
void nash_Cell::descend3(OverlapEverythingCallback* ctx)
{
    ((OverlapInstancesCallback*)ctx)->operatorCall(this);
    OverlapPatchesCallback* patchCtx = &ctx->patchescallback;
    nash_Node* node;
    for (node = nodesinside[1].head; node != 0; node = node->next) {
        tPatchRecord* patch = (tPatchRecord*)node;  // a patch record starts with its list link
        if (overlap_340DC0(patchCtx->box, patch)) {
            addPatch_340DC0(patchCtx, patch);
        }
    }
    OverlapTypedCallback* typedCtx = &ctx->typedcallback;
    for (node = nodesinside[2].head; node != 0; node = node->next) {
        addTyped_340DC0(typedCtx, (cTypedSpaceNode*)node);
    }
    if (children[0][0][0]) children[0][0][0]->descend3(ctx);
    if (children[0][0][1]) children[0][0][1]->descend3(ctx);
    if (children[0][1][0]) children[0][1][0]->descend3(ctx);
    if (children[0][1][1]) children[0][1][1]->descend3(ctx);
    if (children[1][0][0]) children[1][0][0]->descend3(ctx);
    if (children[1][0][1]) children[1][0][1]->descend3(ctx);
    if (children[1][1][0]) children[1][1][0]->descend3(ctx);
    if (children[1][1][1]) children[1][1][1]->descend3(ctx);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend4);
#ifdef SKIP_ASM
extern "C" void TerrainCallback_intersectpatch(void* ctx, void* obj);
extern "C" void TerrainCallback_intersectinstance(void* ctx, void* obj);

/**
 * Intersects a world query with the terrain of an octree cell and of every cell below it.
 *
 * Calls TerrainCallback_intersectpatch(@p ctx, patch) for each patch on the cell's patch list (+0x24) and TerrainCallback_intersectinstance(@p ctx,
 * instance) for each instance on its instance list (+0x20), then calls itself on each non-null child (+0x0..+0x1C).
 * Used by cBXNash_terrainIntersect and nash_privy_QueryHelper_help for cells wholly inside the query.
 *
 * @param ctx  the terrain callback (query, hit buffer and count) passed to both tests
 */
void nash_Cell::descend4(void* ctx)
{
    nash_Node* objNode;
    for (objNode = nodesinside[1].head; objNode != 0; objNode = objNode->next) {
        TerrainCallback_intersectpatch(ctx, objNode);
    }
    for (objNode = nodesinside[0].head; objNode != 0; objNode = objNode->next) {
        TerrainCallback_intersectinstance(ctx, objNode);
    }
    if (children[0][0][0]) children[0][0][0]->descend4(ctx);
    if (children[0][0][1]) children[0][0][1]->descend4(ctx);
    if (children[0][1][0]) children[0][1][0]->descend4(ctx);
    if (children[0][1][1]) children[0][1][1]->descend4(ctx);
    if (children[1][0][0]) children[1][0][0]->descend4(ctx);
    if (children[1][0][1]) children[1][0][1]->descend4(ctx);
    if (children[1][1][0]) children[1][1][0]->descend4(ctx);
    if (children[1][1][1]) children[1][1][1]->descend4(ctx);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend5);
#ifdef SKIP_ASM
extern "C" void StaticObjectCallback_intersectinstance(void* ctx, void* obj);

/**
 * Intersects a world query with the static objects of an octree cell and of every cell below it.
 *
 * Calls StaticObjectCallback_intersectinstance(@p ctx, instance) for each instance on the cell's instance list (+0x20), then itself on each
 * non-null child (+0x0..+0x1C). Used by cBXNash_staticObjectIntersect and nash_privy_QueryHelper_helpX6 for cells wholly inside the query box.
 *
 * @param ctx  the static-object callback passed to StaticObjectCallback_intersectinstance
 */
void nash_Cell::descend5(void* ctx)
{
    nash_Node* objNode;
    for (objNode = nodesinside[0].head; objNode != 0; objNode = objNode->next) {
        StaticObjectCallback_intersectinstance(ctx, objNode);
    }
    if (children[0][0][0]) children[0][0][0]->descend5(ctx);
    if (children[0][0][1]) children[0][0][1]->descend5(ctx);
    if (children[0][1][0]) children[0][1][0]->descend5(ctx);
    if (children[0][1][1]) children[0][1][1]->descend5(ctx);
    if (children[1][0][0]) children[1][0][0]->descend5(ctx);
    if (children[1][0][1]) children[1][0][1]->descend5(ctx);
    if (children[1][1][0]) children[1][1][0]->descend5(ctx);
    if (children[1][1][1]) children[1][1][1]->descend5(ctx);
}
#endif

extern "C" void* func_003400D8(int, int);

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend6);
#ifdef SKIP_ASM

/**
 * Runs the spline query on every typed node of an octree cell and of every cell below it.
 *
 * For each node on the typed list (nodesinside[2]): rails (type 2) go to cRailModifier_splineIntersect, flex rails
 * (type 3) to cFlexRailNode_splineIntersect, and instances (type 1) whose filter word (the segment's
 * m_Parent->m_iFlags) shares a bit with the query mask to intersect_staticSplineSegmentIntersect, all with the query
 * box, position, mask, hit-count and distance pointers and intInfo. It then calls itself on each non-null child. Used
 * by cBXNash_splineIntersect and nash_privy_QueryHelper_helpX7 for cells wholly inside the query box.
 *
 * @param splineQuery  the spline query (On Tour SplineCallback)
 */
void nash_Cell::descend6(SplineCallback* splineQuery)
{
    for (cTypedSpaceNode* typedNode = (cTypedSpaceNode*)nodesinside[2].head; typedNode != 0;
         typedNode = (cTypedSpaceNode*)typedNode->next) {
        int type = typedNode->type;
        if (type == kSNT_Rail) {
            cRailModifier_splineIntersect(((cRailSpaceNode*)typedNode)->Rail, ((tBoundBox*)splineQuery->box),
                                          ((tBoundBox*)splineQuery->box), ((t4Vector*)splineQuery->intersectpos),
                                          ((int)splineQuery->splinefiltermask), splineQuery->hit,
                                          splineQuery->intersectdist, splineQuery->intersectinfo);
        } else if (type == kSNT_FlexRail) {
            cFlexRailNode_splineIntersect(((cFlexRailSpaceNode*)typedNode)->FlexNode, ((tBoundBox*)splineQuery->box),
                                          ((tBoundBox*)splineQuery->box), ((t4Vector*)splineQuery->intersectpos),
                                          ((int)splineQuery->splinefiltermask), splineQuery->hit,
                                          splineQuery->intersectdist, splineQuery->intersectinfo);
        } else if (type == kSNT_SplineSegment) {
            if (((tSplineSegmentRecord*)typedNode)->m_Parent->m_iFlags & ((int)splineQuery->splinefiltermask)) {
                intersect_staticSplineSegmentIntersect(typedNode, ((tBoundBox*)splineQuery->box),
                                                       ((t4Vector*)splineQuery->intersectpos), splineQuery->hit,
                                                       splineQuery->intersectdist, splineQuery->intersectinfo);
            }
        }
    }
    if (children[0][0][0] != 0) children[0][0][0]->descend6(splineQuery);
    if (children[0][0][1] != 0) children[0][0][1]->descend6(splineQuery);
    if (children[0][1][0] != 0) children[0][1][0]->descend6(splineQuery);
    if (children[0][1][1] != 0) children[0][1][1]->descend6(splineQuery);
    if (children[1][0][0] != 0) children[1][0][0]->descend6(splineQuery);
    if (children[1][0][1] != 0) children[1][0][1]->descend6(splineQuery);
    if (children[1][1][0] != 0) children[1][1][0]->descend6(splineQuery);
    if (children[1][1][1] != 0) children[1][1][1]->descend6(splineQuery);
}
#endif

//99.38%
INCLUDE_ASM("intersect/worldsphtree", func_00341368__FPv);
#ifdef SKIP_ASM
/**
 * Runs the unit's static initialiser: func_003400D8(1, 0xFFFF).
 *
 * @return func_003400D8's result register (that function returns nothing)
 */
void* func_00341368(void* self)
{
    return func_003400D8(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_cBoostNode1);
#ifdef SKIP_ASM
// A t4Vector with EA's constructors (ours has none); the VU0 helpers below build and return it by value.
struct t4VectorXYZW : t4Vector {
    t4VectorXYZW() {}
    t4VectorXYZW(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

extern void* cBoostNode_vtbl[];
extern t4Vector t4vect_YAxis[];
extern "C" void* cInstanceNode_cInstanceNode1(void* self, void* a1, int type, void* a3);

// PORT: PS2-only VU0 inline asm (v / |v|).
static inline t4VectorXYZW vu0Normalize_341388(const t4Vector& v)
{
    t4VectorXYZW r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4VectorXYZW vu0Scale_341388(const t4Vector& v, float s)
{
    t4VectorXYZW r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (matrix * vector).
static inline t4VectorXYZW vu0Xform_341388(const t4Vector& v, const char* m)
{
    t4VectorXYZW r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

/**
 * Constructs a boost node from its script data (the record counterpart of the replay constructor
 * cBoostNode_cBoostNode).
 *
 * Runs the base constructor cInstanceNode_cInstanceNode1(self, @p bucket, @p nodeType, @p instance) and stores
 * cBoostNode_vtbl as the vptr. From @p def (On Tour tMakeBoostData) it takes the Type (read as a short); the duration
 * BoostTime times the frame rate (cAppMan_TheAppMan->TimerHz) into BoostTime as a frame count; the direction
 * BoostX/Y/Z normalised into Boost, or when it is zero the Y axis (t4vect_YAxis) for type 2 and the zero vector
 * otherwise; MaxSpeed times 27.7778 (km/h to cm/s) into MaxSpeed; and Drag. The timer (Time) starts at the duration
 * for a non-zero type, else 0. For type 2 the direction is negated and transformed by the instance's matrix
 * (Instance->m_fmTransform) and the speed is negated.
 *
 * MakeBoostFunc (a world-script maker) allocates it as 0x50 bytes tagged "Boost" and passes bucket 1, type 8.
 *
 * @param bucket  the bucket list the node joins
 * @param nodeType  the object-node type (8)
 * @param instance  the instance record
 * @param def  the script's boost data (On Tour tMakeBoostData)
 * @return @p self
 */
cBoostNode* cBoostNode::cBoostNode1(void* bucket, int nodeType, void* instance, tMakeBoostData* def)
{
    cInstanceNode_cInstanceNode1((this), bucket, nodeType, instance);
    VPTR_OF(cObjNode, this) = cBoostNode_vtbl;
    int type = (short)def->Type;
    Type = type;
    BoostTime = (int)(def->BoostTime * (float)cAppMan_TheAppMan->TimerHz);
    if (def->BoostX == 0.0f && def->BoostY == 0.0f && def->BoostZ == 0.0f) {
        if (type == kBoostType_Local) {
            Boost = *t4vect_YAxis;
        } else {
            Boost = *t4vect_Zero;
        }
    } else {
        Boost = vu0Normalize_341388(t4VectorXYZW(def->BoostX, def->BoostY, def->BoostZ, 0.0f));
    }
    MaxSpeed = def->MaxSpeed * 27.77777862548828f;
    Drag = def->Drag;
    Time = Type ? BoostTime : 0;
    if (Type == kBoostType_Local) {
        char* instMatrix = (char*)&Instance->m_fmTransform;
        Boost = vu0Xform_341388(vu0Scale_341388(Boost, -1.0f), instMatrix);
        MaxSpeed = -MaxSpeed;
    }
    return (this);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_cBoostNode);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream);
extern void* cBoostNode_vtbl[];
struct sVEntry_cBoostNode_cBoostNode {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Constructs a boost node from a replay frame (the replay counterpart of the record constructor
 * cBoostNode_cBoostNode1).
 *
 * Runs the cInstanceNode replay constructor, stores cBoostNode's vtable at +0xC, then reads the node's
 * 0x30-byte boost state (direction, speed, factor, duration, timer, type: +0x20 to +0x4F) from @p stream
 * through its read virtual (slot 2).
 *
 * @param bucket  bucket list the node joins, passed to cInstanceNode_cInstanceNode()
 * @param stream  the replay frame to read from (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cBoostNode_cBoostNode(cBoostNode* self, void* bucket, void* stream)
{
    cInstanceNode_cInstanceNode(self, bucket, stream);
    VPTR_OF(cObjNode, self) = cBoostNode_vtbl;
    sVEntry_cBoostNode_cBoostNode* vt = *(sVEntry_cBoostNode_cBoostNode**)stream;
    vt[2].fn(vthis(stream, vt[2]), (char*)&self->Boost, 0x30);
    return ((void*)self);
}
#endif

extern void* cBoostNode_vtbl[];
extern "C" void* cInstanceNode__cInstanceNode(void*);

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode__cBoostNode__FPv);
#ifdef SKIP_ASM
/**
 * Destroys a boost node: restores cBoostNode's vtable, then runs the cInstanceNode destructor.
 *
 * Only @p self is declared here; the destructor flags the caller passed in the second argument register reach
 * cInstanceNode__cInstanceNode() unchanged, and it frees the object when bit 0 is set.
 *
 * @return cInstanceNode__cInstanceNode()'s result (it returns nothing meaningful)
 */
void* cBoostNode__cBoostNode(void* vself)
{
    cBoostNode* self = (cBoostNode*)vself;
    VPTR_OF(cObjNode, self) = cBoostNode_vtbl;
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_update);
#ifdef SKIP_ASM
extern "C" int cObjectInterface_getNumBeings();
extern "C" cBeing* cObjectInterface_getBeing(int i);


/**
 * Applies the boost node to every being touching it, and expires a timed (kBoostType_Timed) node once nothing keeps it
 * alive.
 *
 * Counts the timer (Time) down. For kBoostType_Timed, or once the timer is out, each being touching the node (being
 * slot 3 with the node's Instance; type 2 skips computer riders, slot 9) is slowed (slot 48: velocity times
 * (1 + factor)) when the factor (Drag) is negative, else pushed along the boost direction (slot 47). A type
 * 1 node that no being touches and whose timer is out expires (slot 34, runscript 1); slots 49/50 (empty)
 * bracket the loop.
 */
extern "C" void cBoostNode_update(cBoostNode* self)
{
    int hit = 0;
    if (self->Type != cBoostNode::kBoostType_Timed) {
        hit = 1;
    }
    int timeLeft = self->Time;
    if (timeLeft > 0) {
        int n = timeLeft - 1;
        self->Time = n;
        if (n > 0) {
            hit = 1;
        }
    }
    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)VPTR_OF(cObjNode, self);
    vt[49].fn(vthis(self, vt[49]));
    if (self->Type == cBoostNode::kBoostType_Timed || self->Time <= 0) {
        for (int i = 0; i < cObjectInterface_getNumBeings(); i++) {
            cBeing* obj = cObjectInterface_getBeing(i);
            if (self->Type == cBoostNode::kBoostType_Local) {
                if (obj->isComputer() != 0) continue;
            }
            if (obj->isIntersecting(self->Instance) != 0) {
                hit = 1;
                if (self->Drag < 0.0f) {
                    sVEntry<void(void*, cBeing*)>* vtbl = (sVEntry<void(void*, cBeing*)>*)VPTR_OF(cObjNode, self);
                    vtbl[48].fn(vthis(self, vtbl[48]), obj);  // applyDrag
                } else {
                    sVEntry<void(void*, cBeing*)>* vtbl = (sVEntry<void(void*, cBeing*)>*)VPTR_OF(cObjNode, self);
                    vtbl[47].fn(vthis(self, vtbl[47]), obj);  // applyBoost
                }
            }
        }
    }
    sVEntry<void(void*)>* vtbl2 = (sVEntry<void(void*)>*)VPTR_OF(cObjNode, self);
    vtbl2[50].fn(vthis(self, vtbl2[50]));
    if (hit == 0) {
        sVEntry<void(void*, int)>* vtbl3 = (sVEntry<void(void*, int)>*)VPTR_OF(cObjNode, self);
        vtbl3[34].fn(vthis(self, vtbl3[34]), 1);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_applyDrag);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_341770(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

extern "C" float cObjectInterface_getUpdateTime();

/**
 * Applies the node's drag to a rider.
 *
 * Passes the rider's velocity * (1 + Drag) * cObjectInterface_getUpdateTime() to its applyBoost (cBeing
 * vtable slot 4).
 *
 * @param obj  the rider (cBeing)
 */
void cBoostNode::applyDrag(cBeing* obj)
{
    t4Vector newVel = vu0Scale_341770(vu0Scale_341770(*obj->getVelocity(), Drag + 1.0f),
            cObjectInterface_getUpdateTime());
    obj->applyBoost(&newVel);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_applyBoost);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (v / |v|).
static inline t4Vector vu0Normalize_341818(const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_341818(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_341818(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}


extern "C" float cObjectInterface_getUpdateTime();

static inline void push_341818(cBoostNodeState* p, cBeing* obj, const t4Vector& dir, float amount)
{
    t4Vector u = vu0Scale_341818(vu0Scale_341818(vu0Scale_341818(dir, amount), p->Drag), cObjectInterface_getUpdateTime());
    obj->applyBoost(&u);
}

/**
 * Applies the boost to a rider passing through the node.
 *
 * Takes the boost direction from the node (or the rider's own direction of travel when it is zero) and the rider's
 * speed along it. For node type 2 it passes direction * speed * scale * update time while the rider is not going
 * backwards; otherwise the missing speed (target minus current, when positive) * direction * scale * update time. The
 * vector goes to the rider's applyBoost (cBeing vtable slot 4).
 *
 * @param obj  the rider (cBeing) being boosted
 */
void cBoostNode::applyBoost(cBeing* obj)
{
    t4Vector vel = *obj->getVelocity();
    t4Vector dir = *(&Boost);
    if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f) {
        dir = vu0Normalize_341818(vel);
    }
    float alongSpeed = vu0Dot_341818(vel, dir);
    float diff = MaxSpeed - alongSpeed;
    cBoostNodeState* boostParams = (cBoostNodeState*)&Boost;
    if (Type == kBoostType_Local) {
        if (alongSpeed >= 0.0f) {
            t4Vector newVel = vu0Scale_341818(vu0Scale_341818(vu0Scale_341818(dir, boostParams->MaxSpeed), boostParams->Drag), cObjectInterface_getUpdateTime());
            obj->applyBoost(&newVel);
        }
    } else if (diff > 0.0f) {
        t4Vector newVel = vu0Scale_341818(vu0Scale_341818(vu0Scale_341818(dir, diff), boostParams->Drag), cObjectInterface_getUpdateTime());
        obj->applyBoost(&newVel);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_signal);
#ifdef SKIP_ASM
/**
 * Handles a signal sent to the boost node: kBoostSignal_Restart (1) restarts its timer.
 *
 * Reloads the timer (+0x3C) from the duration (+0x38); other signals are ignored. On Tour's float argument is
 * not taken.
 *
 * @param signal  the signal number
 */
extern "C" void cBoostNode_signal(cBoostNode* self, int signal)
{
    if (signal == cBoostNode::kBoostSignal_Restart) {
        self->Time = self->BoostTime;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_writeToReplayFrame);
#ifdef SKIP_ASM

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a boost node's state to a replay frame.
 *
 * Writes the instance-node part with cInstanceNode_writeToReplayFrame(), then the 0x30 bytes at offset 0x20
 * through the stream's write virtual (slot 1). The stream constructor reads the same block back.
 *
 * @param stream  replay stream to write to
 */
void cBoostNode::writeToReplayFrame(cBxReplayFrame* stream)
{
    cInstanceNode_writeToReplayFrame(this, stream);
    sVEntry<void(void*, void*, int)>* vt = *(sVEntry<void(void*, void*, int)>**)stream;
    vt[1].fn(vthis(stream, vt[1]), (char*)&Boost, 0x30);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_cAnimObjectNode1);
#ifdef SKIP_ASM
extern void* cAnimObjectNode_vtbl[];
extern "C" void cAnimNode_cAnimNode(void* p, int a1, int a2, void* obj, int a4);
extern "C" float cAnimFrame_getStartTime(void* inst);
float AIrandf(float lo, float hi);
float AIrangernd(float a, float b);


static inline void setRate_1AA0(float* dst, float v, float dflt)
{
    float r;
    if (v >= 0.0f) {
        r = v * 0.03333333507180214f;
    } else {
        r = dflt;
    }
    *dst = r;
}

static inline void setRateMax_1AA0(float* dst, float v, char* vinfo)
{
    tModelRecord* info = (tModelRecord*)vinfo;
    float r;
    if (v < 0.0f) {
        r = info->m_fAnimTime;
    } else {
        r = v * 0.03333333507180214f;
    }
    *dst = r;
}

/**
 * Constructs an animated object node from its script data (the record counterpart of the replay constructor
 * cAnimObjectNode_cAnimObjectNode).
 *
 * Builds the animation-node base at +0x1C with cAnimNode_cAnimNode(base, @p bucket, @p nodeType, @p obj, ForceVisible != 0) and stores
 * the vtable cAnimObjectNode_vtbl at +0x3C. From @p def (On Tour tMakeAnimObjectData, times in 30 fps frames converted to
 * seconds) it sets the play type (+0x0) from +0x4; min (+0x14) from FromTime (+0xC), or the animation's start time
 * (cAnimFrame_getStartTime on the animation at +0x64) when negative; max (+0x18) from ToTime (+0x10), or the animation
 * length (the instance's info at +0x80, its +0x14) when negative; and the frame (+0x1C) from StartTime (+0x1C; negative
 * means min), clamped to [min, max] or replaced by AIrandf(min, max) when RandomStart (+0x20) is set.
 *
 * The per-frame speed (+0x10) is Speed (+0x14) / 30 / the frame rate (cAppMan_TheAppMan +0x10), randomised by
 * AIrangernd with SpeedRange (+0x18) when that is non-zero, and negated for Dir (+0x8) 1. It then copies the frame
 * to +0x20, sets active (+0x4) and the dirty flag (bit 0 of +0x42), and clears +0xC (the start delay) and atLimit
 * (+0x8). cAnimObjectNode_updateNode switches on the play type (AnimType).
 *
 * @param bucket  the bucket list the node joins
 * @param nodeType  the object-node type
 * @param obj  the instance record
 * @param def  the script's animation data (On Tour tMakeAnimObjectData)
 * @return @p self
 */
// Animated float constructor: range and start value (frames -> seconds), optional random start and speed.
void* cAnimObjectNode::cAnimObjectNode1(int bucket, int nodeType, tInstanceRecord* obj, tMakeAnimObjectData* def)
{
    cAnimNode_cAnimNode((char*)&CurrTime, bucket, nodeType, obj, def->ForceVisible != 0);
    mVtbl = cAnimObjectNode_vtbl;
    float frameTime = 0.0f;
    char* info = (char*)obj->m_Model;
    if (AnimFrames != 0) {
        frameTime = cAnimFrame_getStartTime(AnimFrames);
    }
    AnimType = def->Type;
    setRate_1AA0(&FromTime, def->FromTime, frameTime);
    setRateMax_1AA0(&ToTime, def->ToTime, info);
    frameTime = def->StartTime;
    if (frameTime < 0.0f) {
        frameTime = FromTime;
    } else {
        frameTime *= 0.03333333507180214f;
    }
    if (frameTime < FromTime) {
        frameTime = FromTime;
    }
    if (ToTime < frameTime) {
        frameTime = ToTime;
    }
    if (def->RandomStart != 0) {
        frameTime = AIrandf(FromTime, ToTime);
    }
    CurrTime = frameTime;
    if (def->SpeedRange != 0.0f) {
        AnimSpeed = AIrangernd(def->Speed * 0.03333333507180214f, def->SpeedRange * 0.03333333507180214f)
                          / (float)cAppMan_TheAppMan->TimerHz;
    } else {
        AnimSpeed = def->Speed * 0.03333333507180214f / (float)cAppMan_TheAppMan->TimerHz;
    }
    if (def->Dir == tMakeAnimObjectData::kDir_Reverse) {
        AnimSpeed = -AnimSpeed;
    }
    CacheTime = CurrTime;
    AnimActive = 1;
    Flags |= 1;
    Delay = 0;
    AnimComplete = 0;
    return (this);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_cAnimObjectNode);
#ifdef SKIP_ASM

extern "C" void* cAnimNode_cAnimNode1(void* p, int a1, void* stream);
extern void* cAnimObjectNode_vtbl[];

/**
 * Constructs an animated object node from a replay frame.
 *
 * Builds the animation node base at +0x1C with cAnimNode_cAnimNode1 (its cMoveNode part at +0x30), stores this
 * class's vtable cAnimObjectNode_vtbl at +0x3C, reads the 0x1C-byte object state into +0x0 from @p stream (read
 * virtual, slot 2) and sets the root-transform-dirty flag (bit 0 of Flags, +0x42).
 *
 * @param bucket  bucket list the node joins, passed on to the base constructor
 * @param stream  the replay frame to read from (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cAnimObjectNode_cAnimObjectNode(cAnimObjectNode* self, int bucket, sVEntry<void(void*, void*, int)>** stream)
{
    cAnimNode_cAnimNode1((char*)&self->CurrTime, bucket, stream);
    self->mVtbl = cAnimObjectNode_vtbl;
    (*stream)[2].fn(vthis(stream, (*stream)[2]), self, 0x1C);
    self->Flags |= 1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode__cAnimObjectNode);
#ifdef SKIP_ASM
extern "C" void cAnimNode__cAnimNode(void* p, int flags);
void operator_delete(int* ptr);
extern void* cAnimObjectNode_vtbl[];

/**
 * Destroys an animated object node: restores its vtable and runs the animation-node destructor.
 *
 * Stores cAnimObjectNode_vtbl at +0x3C, calls cAnimNode__cAnimNode on the base at +0x1C (with flags 0) and frees the object with
 * operator_delete when bit 0 of @p flags is set. Slot 1 of cAnimObjectNode_vtbl.
 *
 * @param flags  destructor flags; bit 0 frees the memory
 */
extern "C" void cAnimObjectNode__cAnimObjectNode(cAnimObjectNode* self, int flags)
{
    self->mVtbl = cAnimObjectNode_vtbl;
    cAnimNode__cAnimNode((char*)&self->CurrTime, 0);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_updateNode);
#ifdef SKIP_ASM
struct sWrapFloat_1E48;
struct sBounceFloat_1EC0;
struct sClampFloat_1F38;
extern "C" void cAnimObjectNode_updateCycle(cAnimObjectNode* s);
extern "C" void cAnimObjectNode_updateOscillate(cAnimObjectNode* s);
extern "C" void cAnimObjectNode_updateOnce(cAnimObjectNode* s);
extern "C" int cAnimNode_frameUpdate(void* p);


/**
 * Advances an animated object node by one step and expires it once a play-once animation has reached its end.
 *
 * Resets +0x24 and +0x28 (the stepped-from and stepped-to frames) to 1e10. When atLimit (+0x8) is already set it
 * returns true at once. While active (+0x4) and the start delay (+0xC) is not negative, it counts the delay down, or
 * once it is 0 records the frame (+0x1C) in +0x24 and steps it by the play type (+0x0): kAnimType_Oscillate
 * cAnimObjectNode_updateOscillate (bounce), kAnimType_Cycle cAnimObjectNode_updateCycle (wrap), otherwise
 * cAnimObjectNode_updateOnce (clamp), returning false if cAnimNode_frameUpdate on the animation part fails.
 *
 * It then sets the dirty flag (bit 0 of +0x42) and copies the frame to +0x20. When the step set atLimit it calls the
 * node's slot 34 (cAnimObjectNode_expire) with 1 and returns false; otherwise it returns true. Slot 15 of
 * cAnimObjectNode_vtbl.
 *
 * @return false after a failed frame update or once the node expired, else true
 */
bool cAnimObjectNode::updateNode()
{
    FrameStart = 10000000000.0f;
    FrameEnd = 10000000000.0f;
    if (AnimComplete != 0) {
        return true;
    }
    if (AnimActive != 0 && Delay >= 0) {
        if (Delay > 0) {
            Delay -= 1;
        } else {
            FrameStart = CurrTime;
            switch (*(short*)this) {
            case cAnimObjectNode::kAnimType_Oscillate:
                updateOscillate();
                break;
            case cAnimObjectNode::kAnimType_Cycle:
                updateCycle();
                break;
            default:
                updateOnce();
                break;
            }
            if (cAnimNode_frameUpdate((char*)&CurrTime) == 0) {
                return false;
            }
        }
    }
    char* obj = (char*)&MoveNodeBase;
    Flags |= 1;
    CacheTime = CurrTime;
    if (AnimComplete == 0) {
        return true;
    }
    sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)mVtbl;
    vt[34].fn(vthis(obj, vt[34]), 1);
    return false;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_updateCycle);
#ifdef SKIP_ASM
struct sWrapFloat_1E48 {
    char pad_0x00[0x10];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

/**
 * Steps an animated value by its speed, wrapping it around the range (the cycling play type).
 *
 * Adds speed (+0x10) to the value (+0x1C) and stores the unwrapped result in +0x28; past max (+0x18) it continues from
 * min (+0x14) by the overshoot, below min (for a negative speed) from max.
 *
 */
void cAnimObjectNode::updateCycle()
{
    if (AnimSpeed >= 0.0f) {
        CurrTime += AnimSpeed;
        FrameEnd = CurrTime;
        if (CurrTime > ToTime) {
            CurrTime = FromTime + (CurrTime - ToTime);
        }
    } else {
        CurrTime += AnimSpeed;
        FrameEnd = CurrTime;
        if (CurrTime < FromTime) {
            CurrTime = ToTime - (FromTime - CurrTime);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_updateOscillate);
#ifdef SKIP_ASM
struct sBounceFloat_1EC0 {
    char pad_0x00[0x10];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

/**
 * Steps an animated value by its speed, bouncing it back at the ends of the range (the oscillating play type).
 *
 * Adds speed (+0x10) to the value (+0x1C) and stores the result in +0x28; past max (+0x18) or below min (+0x14) it
 * reverses the speed and reflects the value back into the range by the overshoot.
 *
 */
void cAnimObjectNode::updateOscillate()
{
    if (AnimSpeed >= 0.0f) {
        CurrTime += AnimSpeed;
        FrameEnd = CurrTime;
        if (CurrTime > ToTime) {
            AnimSpeed = -AnimSpeed;
            CurrTime = ToTime - (CurrTime - ToTime);
        }
    } else {
        CurrTime += AnimSpeed;
        FrameEnd = CurrTime;
        if (CurrTime < FromTime) {
            AnimSpeed = -AnimSpeed;
            CurrTime = FromTime + (FromTime - CurrTime);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_updateOnce);
#ifdef SKIP_ASM
struct sClampFloat_1F38 {
    char pad_0x00[0x8];
    int atLimit;    // 0x08
    char pad_0x0C[0x4];
    float speed;    // 0x10
    float min;      // 0x14
    float max;      // 0x18
    float value;    // 0x1C
    char pad_0x20[0x8];
    float last;     // 0x28
};

/**
 * Steps an animated value by its speed, clamping it at the end of the range (the play-once type).
 *
 * If the value (+0x1C) already sits at the end it is moving to (max +0x18 for a non-negative speed, else min +0x14) it
 * sets atLimit (+0x8). It then adds speed (+0x10), stores the result in +0x28 and clamps the value to that end.
 *
 */
void cAnimObjectNode::updateOnce()
{
    if (AnimSpeed >= 0.0f) {
        if (CurrTime == ToTime) {
            AnimComplete = 1;
        }
        CurrTime += AnimSpeed;
        FrameEnd = CurrTime;
        if (CurrTime > ToTime) {
            CurrTime = ToTime;
        }
    } else {
        if (CurrTime == FromTime) {
            AnimComplete = 1;
        }
        CurrTime += AnimSpeed;
        FrameEnd = CurrTime;
        if (CurrTime < FromTime) {
            CurrTime = FromTime;
        }
    }
}
#endif

void cInstanceNode_runExpireScript(void*);

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_expire);
#ifdef SKIP_ASM
/**
 * Expires an animated object node: when asked, runs its instance's expire script and does nothing else.
 *
 * Calls cInstanceNode_runExpireScript() on the node part at +0x30 (probably the on-expire script, world script type 4).
 * Unlike cInstanceNode_expire() it neither marks the instance dead nor deletes the node.
 *
 * @param runscript  nonzero to run the script
 */
void cAnimObjectNode::expire(int runscript)
{
    if (runscript != 0) {
        cInstanceNode_runExpireScript(&MoveNodeBase);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_signal);
#ifdef SKIP_ASM
extern "C" void cMoveNode_signal(void* p);

/**
 * Handles a signal sent to an animated object node: delay, frame, speed, range and play-type changes.
 *
 * Values are in 30 fps frames unless noted. Signal 100 sets the start delay (+0xC) to @p msgValue seconds times the
 * frame rate; 101 sets the frame (+0x1C) when the new value lies within [min, max]; 102 sets the per-frame speed
 * (+0x10, divided by the frame rate); 103 sets max (+0x18) and 104 min (+0x14), each limited to [0, the animation
 * length] (the instance at +0x48, its info at +0x80, +0x14) and pulling the frame inside; 105 sets the play type (+0x0)
 * to 0 (play once). Any other signal goes to cMoveNode_signal on the move-node part (+0x30). Slot 36 of cAnimObjectNode_vtbl,
 * where the teeter vtable has cMoveNode_signal.
 *
 * @param msg  the signal number
 * @param msgValue  its argument
 */
void cAnimObjectNode::signal(int msg, float msgValue)
{
    if (msg == cAnimObjectNode::kAnimSignal_SetDelay) {
        Delay = (int)(msgValue * (float)cAppMan_TheAppMan->TimerHz);
    } else if (msg == cAnimObjectNode::kAnimSignal_SetTime) {
        msgValue = msgValue * 0.03333333507180214f;
        if (FromTime <= msgValue && msgValue <= ToTime) {
            CurrTime = msgValue;
        }
    } else if (msg == cAnimObjectNode::kAnimSignal_SetSpeed) {
        msgValue = msgValue * 0.03333333507180214f;
        AnimSpeed = msgValue / (float)cAppMan_TheAppMan->TimerHz;
    } else if (msg == cAnimObjectNode::kAnimSignal_SetToTime) {
        msgValue = msgValue * 0.03333333507180214f;
        float lim = Instance->m_Model->m_fAnimTime;
        if (lim < msgValue) {
            msgValue = lim;
        }
        if (msgValue < 0.0f) {
            msgValue = 0.0f;
        }
        ToTime = msgValue;
        if (msgValue < CurrTime) {
            CurrTime = msgValue;
        }
    } else if (msg == cAnimObjectNode::kAnimSignal_SetFromTime) {
        msgValue = msgValue * 0.03333333507180214f;
        float lim = Instance->m_Model->m_fAnimTime;
        if (lim < msgValue) {
            msgValue = lim;
        }
        if (msgValue < 0.0f) {
            msgValue = 0.0f;
        }
        FromTime = msgValue;
        if (CurrTime < msgValue) {
            CurrTime = msgValue;
        }
    } else if (msg == cAnimObjectNode::kAnimSignal_PlayOnce) {
        *(short*)this = cAnimObjectNode::kAnimType_Once;
    } else {
        cMoveNode_signal(&MoveNodeBase);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void cAnimNode_writeToReplayFrame(void* p, void* stream);
struct sVEntry_cAnimObjectNode_writeToReplayFrame {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes an animated object node to a replay frame.
 *
 * Writes the animation-node part (+0x1C) with cAnimNode_writeToReplayFrame, then the 0x1C-byte object state at +0x0 (play type, flags,
 * delay, speed, range, frame) with the frame's write virtual (slot 1). Slot 12 of cAnimObjectNode_vtbl.
 *
 * @param stream  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
void cAnimObjectNode::writeToReplayFrame(void* stream)
{
    cAnimNode_writeToReplayFrame((char*)&CurrTime, stream);
    sVEntry_cAnimObjectNode_writeToReplayFrame* vt = *(sVEntry_cAnimObjectNode_writeToReplayFrame**)stream;
    vt[1].fn(vthis(stream, vt[1]), this, 0x1C);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_cAnimTeeterNode1);
#ifdef SKIP_ASM
extern void* cAnimTeeterNode_vtbl[];
extern "C" void cAnimNode_cAnimNode(void* p, int a1, int a2, void* obj, int a4);
extern "C" float cAnimFrame_getStartTime(void* inst);


struct sFloat_003421A0 {
    float v;
};

static inline void setRate_003421A0(sFloat_003421A0* dst, float v, float dflt)
{
    float r;
    if (v >= 0.0f) {
        r = v * 0.03333333507180214f;
    } else {
        r = dflt;
    }
    dst->v = r;
}

static inline void setRateMax_003421A0(sFloat_003421A0* dst, float v, char* vinfo)
{
    tModelRecord* info = (tModelRecord*)vinfo;
    float r;
    if (v < 0.0f) {
        r = info->m_fAnimTime;
    } else {
        r = v * 0.03333333507180214f;
    }
    dst->v = r;
}

/**
 * Constructs an animated teeter node from its script data (the record counterpart of the replay constructor
 * cAnimTeeterNode_cAnimTeeterNode).
 *
 * Builds the animation-node base at +0x30 with cAnimNode_cAnimNode(base, @p bucket, 4, @p obj, 0) and stores the vtable
 * cAnimTeeterNode_vtbl at +0x50. From @p def (On Tour tMakeAnimTeeterData) it sets min (+0x10) from FromTime (+0x20, a 30 fps
 * frame; negative means the animation's start time) and max (+0x14) from ToTime (+0x24; negative means the animation
 * length, the instance's info +0x80, +0x14); BounceFactor (+0x8) to +0xC; TorqueScale (+0x4) times 1e-6 to +0x8;
 * MaxSpeed (+0xC) / 30 / the frame rate to +0x18; AnimSign (+0x1C) to +0x2C; PivotTime (+0x14) / 30 to +0x24; Damp
 * (+0x18) to +0x1C; and Spring (+0x10) to +0x20. It sets the dirty flag (bit 0 of +0x56) and clears the torque (+0x0),
 * step (+0x4) and velocity (+0x28).
 *
 * MakeAnimTeeterFunc (a world-script maker) allocates it as 0x80 bytes tagged "AnimTeeter" and passes bucket 1.
 *
 * @param bucket  the bucket list the node joins
 * @param obj  the instance record
 * @param def  the script's teeter data (On Tour tMakeAnimTeeterData)
 * @return @p self
 */
void* cAnimTeeterNode::cAnimTeeterNode1(int bucket, tInstanceRecord* obj, tMakeAnimTeeterData* def)
{
    cAnimNode_cAnimNode(&CurrTime, bucket, 4, obj, 0);
    mVtbl = cAnimTeeterNode_vtbl;
    float animStart = 0.0f;
    char* info = (char*)obj->m_Model;
    if (AnimFrames != 0) {
        animStart = cAnimFrame_getStartTime(AnimFrames);
    }
    setRate_003421A0((sFloat_003421A0*)&FromTime, def->FromTime, animStart);
    setRateMax_003421A0((sFloat_003421A0*)&ToTime, def->ToTime, info);
    BounceFactor = def->BounceFactor;
    TorqueScale = def->TorqueScale * 9.999999974752427e-07f;
    MaxAnimSpeed = def->MaxSpeed * 0.03333333507180214f / (float)cAppMan_TheAppMan->TimerHz;
    AnimSign = def->AnimSign;
    PivotTime = def->PivotTime * 0.03333333507180214f;
    Damp = def->Damp;
    Spring = def->Spring;
    Flags |= 1;
    TorqueSum = 0.0f;
    AnimSpeed = 0.0f;
    W = 0.0f;
    return (this);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_cAnimTeeterNode);
#ifdef SKIP_ASM

extern "C" void* cAnimNode_cAnimNode1(void* p, int a1, void* stream);
extern void* cAnimTeeterNode_vtbl[];

/**
 * Constructs an animated teeter node from a replay frame.
 *
 * Builds the animation node base at +0x30 with cAnimNode_cAnimNode1 (its cMoveNode part at +0x44), stores this
 * class's vtable cAnimTeeterNode_vtbl at +0x50, reads the 0x30-byte teeter state into +0x0 from @p stream (read
 * virtual, slot 2) and sets the root-transform-dirty flag (bit 0 of Flags, +0x56).
 *
 * @param bucket  bucket list the node joins, passed on to the base constructor
 * @param stream  the replay frame to read from (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cAnimTeeterNode_cAnimTeeterNode(cAnimTeeterNode* self, int bucket, sVEntry<void(void*, void*, int)>** stream)
{
    cAnimNode_cAnimNode1(&self->CurrTime, bucket, stream);
    self->mVtbl = cAnimTeeterNode_vtbl;
    (*stream)[2].fn(vthis(stream, (*stream)[2]), self, 0x30);
    self->Flags |= 1;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_updateNode);
#ifdef SKIP_ASM
extern "C" float cObjectInterface_getUpdateTime();
extern "C" int cAnimNode_frameUpdate(void* p);

struct sSphCtl_00342358 {
    float force;        // 0x00
    float step;         // 0x04
    float gain;         // 0x08
    float bounce;       // 0x0C
    float min;          // 0x10
    float max;          // 0x14
    float maxStep;      // 0x18
    float damping;      // 0x1C
    float stiffness;    // 0x20
    float rest;         // 0x24
    float vel;          // 0x28
    int f2C;            // 0x2C
    float pos;          // 0x30
    float pos34;        // 0x34
    float prev;         // 0x38
    float pos3C;        // 0x3C
    char pad40[0x16];
    unsigned short flags; // 0x56
};

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_00342358(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Advances a teeter by one step: a damped spring that turns the accumulated torque into animation-frame motion.
 *
 * Saves the frame (+0x30) in +0x38 and, with dt = cObjectInterface_getUpdateTime(), computes the acceleration -Spring *
 * (frame - PivotTime) + |TorqueScale| * torque - Damp * velocity (+0x20, +0x24, +0x8, +0x0, +0x1C, +0x28). The step
 * (+0x4) is velocity * dt limited to +-MaxSpeed (+0x18); the frame advances by it (also copied to +0x3C) and the
 * velocity by acceleration * dt. Past min (+0x10) or max (+0x14) the frame is clamped and the velocity multiplied by
 * BounceFactor (+0xC), or step and velocity zeroed when |velocity| <= 0.1.
 *
 * It then clears the torque, copies the frame to +0x34 and runs cAnimNode_frameUpdate on the animation part; on success it sets
 * the dirty flag (bit 0 of +0x56). Slot 15 of cAnimTeeterNode_vtbl.
 *
 * @return cAnimNode_frameUpdate's result: true after a successful frame update, else false
 */
bool cAnimTeeterNode::updateNode()
{
    FrameStart = CurrTime;
    float dt = cObjectInterface_getUpdateTime();
    float force = TorqueScale * TorqueSum;
    if (TorqueScale < 0.0f) {
        force = -force;
    }
    float vel = W;
    float maxStep = MaxAnimSpeed;
    float step = vel * dt;
    float acc = -Spring * (CurrTime - PivotTime);
    acc += force;
    acc += -Damp * vel;
    AnimSpeed = step;
    if (maxStep < absf_00342358(step)) {
        if (step < 0.0f) {
            AnimSpeed = -maxStep;
        } else {
            AnimSpeed = maxStep;
        }
    }
    float pos = CurrTime + AnimSpeed;
    float nv = W + acc * dt;
    CurrTime = pos;
    FrameEnd = pos;
    W = nv;
    if (pos < FromTime) {
        CurrTime = FromTime;
        if (absf_00342358(nv) > 0.1f) {
            W = nv * BounceFactor;
        } else {
            AnimSpeed = 0.0f;
            W = 0.0f;
        }
    } else if (ToTime < pos) {
        CurrTime = ToTime;
        if (absf_00342358(nv) > 0.1f) {
            W = nv * BounceFactor;
        } else {
            AnimSpeed = 0.0f;
            W = 0.0f;
        }
    }
    TorqueSum = 0.0f;
    CacheTime = CurrTime;
    if (cAnimNode_frameUpdate(&CurrTime) == 0) {
        return false;
    }
    Flags |= 1;
    return true;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_getAnimSpeed);
#ifdef SKIP_ASM
/**
 * Returns the teeter's animation speed: its current step, or 0 while it rests against an end of its range.
 *
 * Returns 0 when the frame (+0x30) is at or below min (+0x10) with a negative step (+0x4), or at or above max (+0x14)
 * with a positive one. Last slot of cAnimTeeterNode_vtbl, where cAnimObjectNode_vtbl has cAnimObjectNode_getAnimSpeed.
 *
 * @return the step per update, or 0
 */
float cAnimTeeterNode::getAnimSpeed()
{
    if (CurrTime <= FromTime && AnimSpeed < 0.0f) {
        return 0.0f;
    }
    if (CurrTime >= ToTime && AnimSpeed > 0.0f) {
        return 0.0f;
    }
    return AnimSpeed;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_applyForce);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// ---- Shared declarations: one type per real thing, used by the functions below ----


struct sSelf_342538 {
    float sum;                      // 0x00
    char pad04[4];
    float dir;                      // 0x08
    char pad0C[0x20];
    int mask;                       // 0x2C
    char pad30[0x2C];
    tInstanceRecord* inf;            // 0x5C
};


extern t4Vector t4vect_YAxis_342538[] __asm__("t4vect_YAxis");

// PORT: PS2-only VU0 inline asm (vector length via vsqrt).
static inline float vu0Length_342538(const t4Vector& v)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vsqrt     Q, $vf4x\n"
        "vwaitq\n"
        "cfc2.ni   %1, $vi22\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(v));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline void vu0Sub_342538(t4Vector& r, const t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline void vu0Scale_342538(t4Vector& r, const t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (matrix times vector).
static inline void vu0Xform_342538(t4Vector& r, const t4Vector& v, const t4Matrix* m)
{
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "o"(v)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (cross product xyz).
static inline void vu0Cross_342538(t4Vector& r, const t4Vector& a, const t4Vector& b)
{
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_342538(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

/**
 * Adds the torque a force applied at a hit point exerts about the teeter's pivot axis.
 *
 * Only for a hit on a mesh (@p intInfo +0x5C >= 0) whose entry in the instance model's m_MeshHeirarchy has a non-zero
 * m_MeshAnim, and only when |@p force| > 100. It fetches the mesh's animation matrix (slot 29
 * of the move-node part at +0x44, cAnimNode_getAnimMat), takes the lever arm from the matrix's translation (row[3]) to
 * the hit position (@p intInfo +0x0), negates it when TorqueScale (+0x8) is negative and again when the mesh's bit in
 * AnimSign (+0x2C) is set, and adds dot(matrix * Y axis, cross(arm, force)) to the torque (+0x0).
 *
 * cAnimTeeterNode's update (cAnimTeeterNode_updateNode) consumes the torque. Slot of cAnimTeeterNode_vtbl where cAnimObjectNode_vtbl has
 * cMoveNode_applyForce.
 *
 * @param intInfo  the hit (On Tour tIntersectInfo: position at +0x0, mesh index at +0x5C)
 * @param force  the force vector
 */
void cAnimTeeterNode::applyForce(tIntersectInfo* intInfo, t4Vector* force)
{
    int idx = intInfo->Mesh;
    if (idx >= 0) {
        cMoveNode* sub = (cMoveNode*)(&MoveNodeBase);
        tMeshHeirarchyRecord* ents = sub->Instance->m_Model->m_MeshHeirarchy;
        if (ents[idx].m_MeshAnim != 0) {
            // slot 29: cAnimNode_getAnimMat
            sVEntry<t4Matrix*(void*, int)>* vt = (sVEntry<t4Matrix*(void*, int)>*)mVtbl;
            t4Matrix* animMat = vt[29].fn(vthis(sub, vt[29]), idx);
            t4Matrix* pivotMat = animMat;
            if (vu0Length_342538(*force) > 100.0f) {
                t4Vector leverArm;
                t4Vector scratch;
                t4Vector vuResult;
                t4Vector torqueVec;
                scratch = animMat->row[3];
                vu0Sub_342538(vuResult, (*(&intInfo->Ipos)), scratch);
                leverArm = vuResult;
                if (TorqueScale < 0.0f) {
                    vu0Scale_342538(vuResult, leverArm, -1.0f);
                    scratch = vuResult;
                    leverArm = vuResult;
                }
                if ((AnimSign >> intInfo->Mesh) & 1) {
                    vu0Scale_342538(vuResult, leverArm, -1.0f);
                    scratch = vuResult;
                    leverArm = vuResult;
                }
                vu0Xform_342538(vuResult, t4vect_YAxis_342538[0], pivotMat);
                scratch = vuResult;
                vu0Cross_342538(torqueVec, leverArm, *force);
                vuResult = torqueVec;
                TorqueSum += vu0Dot_342538(scratch, vuResult);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void cAnimNode_writeToReplayFrame(void* p, void* stream);
struct sVEntry_cAnimTeeterNode_writeToReplayFrame {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

/**
 * Writes an animated teeter node to a replay frame.
 *
 * Writes the animation-node part (+0x30) with cAnimNode_writeToReplayFrame, then the 0x30-byte teeter state at +0x0 with the frame's
 * write virtual (slot 1). Slot 12 of cAnimTeeterNode_vtbl.
 *
 * @param stream  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
void cAnimTeeterNode::writeToReplayFrame(void* stream)
{
    cAnimNode_writeToReplayFrame(&CurrTime, stream);
    sVEntry_cAnimTeeterNode_writeToReplayFrame* vt = *(sVEntry_cAnimTeeterNode_writeToReplayFrame**)stream;
    vt[1].fn(vthis(stream, vt[1]), this, 0x30);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_cBoostModifier);
#ifdef SKIP_ASM
extern void* cBoostModifier_vtbl[];

/**
 * Constructs a boost modifier from its script data and instance.
 *
 * Sets Active, stores cBoostModifier_vtbl and Instance (@p instRecord), and from @p def (On Tour tAddBoostData) takes Type
 * (+0x4, read as a short), BoostTime (+0x8 seconds times the frame rate at cAppMan_TheAppMan +0x10, in frames), Boost
 * (BoostX/Y/Z at +0x14, w = 0, not normalised), MaxSpeed (+0x10 times 100) and Drag (+0xC). Time starts at BoostTime
 * for type 1, else 0. Unlike the boost node (cBoostNode_cBoostNode1), which converts MaxSpeed from km/h, it scales by 100.
 *
 * Called by cMoveNode_addBoost.
 *
 * @param def  the script's boost data (On Tour tAddBoostData)
 * @param instRecord  the instance record the modifier belongs to
 * @return @p self
 */
extern "C" void* cBoostModifier_cBoostModifier(cBoostModifier* self, tAddBoostData* def, tInstanceRecord* instRecord)
{
    self->Active = 1;
    VPTR_OF(cPhysicsModifier, self) = cBoostModifier_vtbl;
    self->Instance = instRecord;
    int type = (short)def->Type;
    self->Type = type;
    int boostFrames = (int)(def->BoostTime * (float)cAppMan_TheAppMan->TimerHz);
    self->BoostTime = boostFrames;
    t4Vector boostDir;
    boostDir.x = def->BoostX;
    boostDir.y = def->BoostY;
    boostDir.z = def->BoostZ;
    boostDir.w = 0.0f;
    *(&self->Boost) = boostDir;
    self->MaxSpeed = def->MaxSpeed * 100.0f;
    self->Drag = def->Drag;
    self->Time = type == cBoostModifier::kBoostModType_Timed ? boostFrames : 0;
    return (self);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_cBoostModifier1);
#ifdef SKIP_ASM

extern void* cBoostModifier_vtbl[];

/**
 * Constructs a boost modifier from a replay frame.
 *
 * Sets Active, stores cBoostModifier_vtbl, reads the 0x30-byte boost state (Boost .. Type) with the frame's read
 * virtual (slot 2) and Instance with readReplayInstance (slot 3). Called by tModifierBlock_readFromReplayFrame.
 *
 * @param stream  the replay frame to read from (On Tour cBxReplayFrame* data)
 * @return @p self
 */
void* cBoostModifier::cBoostModifier1(cBxReplayFrame* stream)
{
    Active = 1;
    VPTR_OF(cPhysicsModifier, this) = cBoostModifier_vtbl;
    stream->read((char*)&Boost, 0x30);
    Instance = stream->readReplayInstance();
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_update);
#ifdef SKIP_ASM
extern "C" int cObjectInterface_getNumBeings();
extern "C" cBeing* cObjectInterface_getBeing(int i);

typedef void (*Fn0_00342880)(void*);
typedef void (*Fn1_00342880)(void*, void*);

/**
 * Applies the boost modifier to every being touching it and updates its active flag (+0x0).
 *
 * The flag starts as type == 0; the timer (+0x2C) counts down and keeps it set while running. For type 1, or
 * once the timer is out, each being whose slot 3 test accepts the instance at +0x40 sets the flag and is
 * slowed (slot 8, cBoostModifier_applyDrag) when the factor (+0x24) is negative, else pushed (slot 7, cBoostModifier_applyBoost).
 * boostStart (slot 9) and boostEnd (slot 10) bracket the loop.
 */
void cBoostModifier::update()
{
    Active = 0;
    if (Type == 0) {
        Active = 1;
    }
    int timeLeft = Time;
    if (timeLeft > 0) {
        timeLeft--;
        Time = timeLeft;
        if (timeLeft > 0) {
            Active = 1;
        }
    }
    boostStart();
    if (Type == cBoostModifier::kBoostModType_Timed || Time <= 0) {
        for (int i = 0; i < cObjectInterface_getNumBeings(); i++) {
            cBeing* obj = cObjectInterface_getBeing(i);
            if (obj->isIntersecting(Instance) != 0) {
                Active = 1;
                if (Drag < 0.0f) {
                    applyDrag(obj);
                } else {
                    applyBoost(obj);
                }
            }
        }
    }
    boostEnd();
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_applyDrag);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_3429E0(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

extern "C" float cObjectInterface_getUpdateTime();

/**
 * Applies the boost zone's drag to a rider: an impulse of velocity * (Drag + 1) * the frame time.
 *
 * Reads the velocity (slot 2) and passes the result to the being's slot 4,
 * applyBoost(const t4Vector&) in On Tour's cBeing: an added impulse.
 *
 * @param obj  the rider in the zone (On Tour cBeing* pBeing)
 */
void cBoostModifier::applyDrag(cBeing*obj)
{
    t4Vector newVelocity = vu0Scale_3429E0(vu0Scale_3429E0(*obj->getVelocity(), Drag + 1.0f),
            cObjectInterface_getUpdateTime());
    obj->applyBoost(&newVelocity);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_applyBoost);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"
#include "replay/bxreplayframe.h"

// Interface of a moving object (getVelocity / applyBoost at slots 2 and 4).
class cWsMover {
public:
    virtual void v01();
    virtual t4Vector* getVelocity();
    virtual void v03();
    virtual void applyBoost(t4Vector* v);
};

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

extern "C" float cObjectInterface_getUpdateTime();

struct t4Vector;



static inline void setVel_342A88(cWsMover* o, t4Vector* v)
{
    o->applyBoost(v);
}


struct sSelf_342A88 {
    char pad[0x10];
    cBoostModifierState plane;
};

/**
 * Pushes a rider along the boost direction while its speed along it is below MaxSpeed.
 *
 * With d = MaxSpeed - velocity . Boost > 0 it passes Boost * d * (the word at +0x24, Drag) * the frame time to the
 * being's slot 4 (On Tour cBeing::applyBoost). The view sPlane_342A88 reads
 * MaxSpeed and Drag as the plane's d and k.
 *
 * @param obj  the rider in the zone (On Tour cBeing* pBeing)
 */
extern "C" void cBoostModifier_applyBoost(cBoostModifier* self, cWsMover* obj)
{
    float speedDeficit = self->MaxSpeed - vu0Dot(*obj->getVelocity(), self->Boost);
    if (speedDeficit > 0.0f) {
        // PORT: takes the address of a temporary (g++ 2.95 accepts it).
        cBoostModifierState* pl = (cBoostModifierState*)&self->Boost;
        setVel_342A88(obj, &vu0Scale(vu0Scale(vu0Scale(pl->Boost, speedDeficit), pl->Drag), cObjectInterface_getUpdateTime()));
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_signal);
#ifdef SKIP_ASM
/**
 * Handles a signal sent to the boost modifier: kBoostModSignal_Restart (600) restarts its timer.
 *
 * Reloads the timer (+0x2C) from the duration (+0x28); other signals are ignored.
 *
 * @param signal  the signal number
 */
extern "C" void cBoostModifier_signal(cBoostModifier* self, int signal)
{
    if (signal == cBoostModifier::kBoostModSignal_Restart) {
        self->Time = self->BoostTime;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void cPhysicsModifier_writeToReplayFrame(void* self, void* stream);
/**
 * Writes the boost modifier to a replay frame: the physics-modifier part, the 0x30-byte boost state and the
 * instance.
 *
 * After cPhysicsModifier_writeToReplayFrame it writes Boost .. Type (+0x10, 0x30 bytes) with the frame's write
 * virtual (slot 1) and the Instance pointer with slot 5.
 *
 * @param stream  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
void cBoostModifier::writeToReplayFrame(cBxReplayFrame* stream)
{
    cPhysicsModifier_writeToReplayFrame(this, stream);
    stream->write((const char*)&Boost, 0x30);
    stream->writeReplayInstance((const tInstanceRecord*)Instance);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cDebounceNode_cDebounceNode1);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode1(void* self, void* a1, int type, void* a3);
extern "C" void cMoveNode_transferFromOld(void* self, void* a1);
extern void* cDebounceNode_vtbl[];




/**
 * Constructs a debounce node that takes over from an instance's old node, and sets the instance's visibility.
 *
 * Runs the base constructor cMoveNode_cMoveNode1(self, @p bucket, 0, @p instance), stores cDebounceNode_vtbl at +0xC, sets the debounce
 * time (+0x2C) to Time (+0x4) seconds times the frame rate (cAppMan_TheAppMan +0x10) and +0x30 to Type (+0x8), and
 * hands over from @p oldNode with cMoveNode_transferFromOld. Visible (+0xC) then edits the instance's flags (instance record
 * at +0x18, flags at +0x8): 0 clears the low four bits; 1 sets bit 0, clears bit 1 and sets bit 2; otherwise bit 1
 * becomes bit 2 only when bits 0 and 1 are both set.
 *
 * Callers allocate 52 bytes and pass bucket 1: MakeDebounceFunc (tag "Debounce", no old node) and checkActiveNode (tag
 * "cDebounce", passing the node being replaced).
 *
 * @param bucket  the bucket list the node joins
 * @param instance  the instance record
 * @param def  the script's debounce data (On Tour tMakeDebounceData)
 * @param oldNode  the node it replaces (On Tour pOldNode)
 * @return @p self
 */
void* cDebounceNode::cDebounceNode1(void* bucket, void* instance, tMakeDebounceData* def, void* oldNode)
{
    cMoveNode_cMoveNode1(this, bucket, 0, instance);
    VPTR_OF(cObjNode, this) = cDebounceNode_vtbl;
    Life = (int)(def->Time * (float)cAppMan_TheAppMan->TimerHz);
    Type = def->Type;
    cMoveNode_transferFromOld(this, oldNode);
    if (def->Visible == tMakeDebounceData::kVisible_Hide) {
        Instance->m_iFlags &= 0xFFFFFFF0;
    } else if (def->Visible == tMakeDebounceData::kVisible_Show) {
        Instance->m_iFlags |= 1;
        Instance->m_iFlags = (((unsigned int)(Instance)->m_iFlags) & ~2u) | 4;
    } else {
        tInstanceRecord* n = Instance;
        if ((((unsigned int)n->m_iFlags) & 3) == 3) {
            n->m_iFlags = (((unsigned int)n->m_iFlags) & ~2u) | 4;
        }
    }
    return this;
}
#endif

