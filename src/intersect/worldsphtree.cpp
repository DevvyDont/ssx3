#include "common.h"
#include "object/boostmodifier.h"
#include "world/world.h"

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_cWorldSphTree);
#ifdef SKIP_ASM
extern void* cWorldSphTree_vtbl[];
extern char str_SphereTree[];
extern "C" void* SphereTree_SphereTree(void* p);
extern "C" float SphereTree_GetRadius(void* self);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);

struct sSphV4 { float x, y, z, w; } __attribute__((aligned(16)));

/**
 * Constructs a world-intersection wrapper around a sphere tree, with a bounding box from its root sphere.
 *
 * Stores cWorldSphTree's vtable at +0x50, clears +0x4 and +0x8, sets +0x0 to 1 unless @p isLow and keeps @p
 * ast at +0x60 and +0x64. Copies the tree's transCenter to +0x40 and stores centre + r and centre - r (w = 1)
 * at +0x20 and +0x30, r being SphereTree_GetRadius()'s radius. Also allocates a fresh 0x140-byte "SphereTree"
 * (SphereTree_SphereTree) at +0x68, which the destructor cWorldSphTree__cWorldSphTree frees.
 *
 * @param ast    the bxSphereTree to wrap
 * @param isLow  nonzero for a low-detail wrapper: +0x0 is then 0 and cWorldSphTree_GetIdealParam() returns -1
 * @return @p self
 */
extern "C" void* cWorldSphTree_cWorldSphTree(void* self, void* ast, int isLow)
{
    *(void***)((char*)self + 0x50) = cWorldSphTree_vtbl;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x4) = 0;
    if (isLow != 0) {
        *(int*)self = 0;
    } else {
        *(int*)self = 1;
    }
    *(void**)((char*)self + 0x64) = ast;
    *(void**)((char*)self + 0x60) = ast;
    sSphV4 center = *(sSphV4*)((char*)ast + 0x80);
    float radius = SphereTree_GetRadius(ast);
    sSphV4 corner;
    *(sSphV4*)((char*)self + 0x40) = center;
    corner.x = center.x + radius;
    corner.y = center.y + radius;
    corner.z = center.z + radius;
    corner.w = 1.0f;
    *(sSphV4*)((char*)self + 0x20) = corner;
    corner.x = center.x - radius;
    corner.y = center.y - radius;
    corner.z = center.z - radius;
    corner.w = 1.0f;
    *(sSphV4*)((char*)self + 0x30) = corner;
    void* localTree = cMemMan_alloc(320, str_SphereTree, 0, 0);
    SphereTree_SphereTree(localTree);
    *(void**)((char*)self + 0x68) = localTree;
    return self;
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
extern "C" void cWorldSphTree__cWorldSphTree(void* self, int flags)
{
    *(void***)((char*)self + 0x50) = cWorldSphTree_vtbl;
    operator_delete(*(int**)((char*)self + 0x68));
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_IsInBoxTransformed);
#ifdef SKIP_ASM
struct sV4_330540 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector add).
static inline sV4_330540 add_330540(const sV4_330540& a, const sV4_330540& b)
{
    sV4_330540 r;
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
static inline sV4_330540 sub_330540(const sV4_330540& a, const sV4_330540& b)
{
    sV4_330540 r;
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
 * @return 0 when the sphere's bounding box lies wholly beyond one face, otherwise 1
 */
extern "C" int cWorldSphTree_IsInBoxTransformed(void* self, float* lo, float* hi)
{
    sV4_330540 ext;
    float rx = SphereTree_GetRadius(*(void**)((char*)self + 0x60));
    float ry = SphereTree_GetRadius(*(void**)((char*)self + 0x60));
    float rz = SphereTree_GetRadius(*(void**)((char*)self + 0x60));
    ext.x = rx;
    ext.y = ry;
    ext.z = rz;
    ext.w = 0.0f;
    sV4_330540 mx = add_330540(*(sV4_330540*)(*(char**)((char*)self + 0x60) + 0x80), ext);
    sV4_330540 mn = sub_330540(*(sV4_330540*)(*(char**)((char*)self + 0x60) + 0x80), ext);
    float lx = lo[0];
    float tx = mx.x;
    if (tx < lx && mn.x < lx) return 0;
    if (hi[0] < tx && hi[0] < mn.x) return 0;
    float ly = lo[1];
    float ty = mx.y;
    if (ty < ly && mn.y < ly) return 0;
    if (hi[1] < ty && hi[1] < mn.y) return 0;
    float lz = lo[2];
    float tz = mx.z;
    if (tz < lz && mn.z < lz) return 0;
    if (!(hi[2] < tz) || !(hi[2] < mn.z)) return 1;
    return 0;
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
extern "C" float cWorldSphTree_GetIdealParam(void* self)
{
    float idealParam;
    if (*(int*)self != 0) {
        idealParam = SphereTree_GetRadius(*(void**)((char*)self + 0x60)) * 2.0f;
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
extern "C" void cWorldSphTree_Transform(void* self, void* mat, float scale)
{
    bxSphereTree_operatorE(*(void**)((char*)self + 0x68), *(void**)((char*)self + 0x60));
    *(void**)((char*)self + 0x60) = *(void**)((char*)self + 0x68);
    SphereTree_TransformRel(*(void**)((char*)self + 0x60), mat, scale);
    *(int*)((char*)self + 0x4) = 1;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_UnTransform__FPv);
#ifdef SKIP_ASM
/**
 * Undoes cWorldSphTree_Transform: makes the original tree (+0x64) current again (+0x60) and clears the transformed
 * flag.
 *
 * @return the original tree pointer (declared int; On Tour returns void)
 */
int cWorldSphTree_UnTransform(void* self)
{
    int original = *(int*)((char*)self + 0x64);
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x60) = original;
    return original;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_IsTriInt);
#ifdef SKIP_ASM
struct sWsVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330788(const sWsVec4& v)
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
static inline sWsVec4 wsDiv_330788(const sWsVec4& v, float s)
{
    sWsVec4 r;
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

extern "C" int SphereTree_IsTriCollision(void* tree, int a1, int a2, int a3, int a4, sWsVec4* out, int a6);

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
extern "C" int cWorldSphTree_IsTriInt(void* self, int vertA, int vertB, int vertC, int n, int ipos, sWsVec4* outDir, float* outLen)
{
    sWsVec4 hitDir;
    int hit = SphereTree_IsTriCollision(*(void**)((char*)self + 0x60), vertA, vertB, vertC, n, &hitDir, ipos);
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
static inline float wsLength_330828(const sWsVec4& v)
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
static inline sWsVec4 wsDiv_330828(const sWsVec4& v, float s)
{
    sWsVec4 r;
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

extern "C" int bxSphereTree_Is2SidedTriCollision(void* tree, int a1, int a2, int a3, int a4, sWsVec4* out, int a6);

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
extern "C" int cWorldSphTree_Is2SidedTriInt(void* self, int vertA, int vertB, int vertC, int n, int ipos, sWsVec4* outDir, float* outLen)
{
    sWsVec4 hitDir;
    int hit = bxSphereTree_Is2SidedTriCollision(*(void**)((char*)self + 0x60), vertA, vertB, vertC, n, &hitDir, ipos);
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
static inline float wsLength_3308C8(const sWsVec4& v)
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
static inline void wsDivEq_3308C8(sWsVec4& v, float s)
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

struct sWsHit_3308C8 {
    char pad_0x00[0x10];
    sWsVec4 dir;      // 0x10
    char pad_0x20[0x20];
    float len;            // 0x40
};

extern "C" int SphereTree_IsTreeCollision(void* self, void* other, void* a2, void* a3);

/**
 * Collides another sphere tree with this one's current tree and normalises the hit's push-out vector.
 *
 * @param tree  the other sphere tree (On Tour bxSphereTree* tree)
 * @param hit  receives the intersection (On Tour tIntersectInfo* ti); direction normalised, length stored
 * @return 1 on a hit (SphereTree_IsTreeCollision), otherwise 0
 */
extern "C" int cWorldSphTree_IsSphTreeInt(void* self, void* tree, sWsHit_3308C8* hit)
{
    if (SphereTree_IsTreeCollision(*(void**)((char*)self + 0x60), tree, &hit->dir, hit) != 0) {
        float len = wsLength_3308C8(hit->dir);
        hit->len = len;
        wsDivEq_3308C8(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cWorldSphTree_IsBoxInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float wsLength_330950(const sWsVec4& v)
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
static inline void wsDivEq_330950(sWsVec4& v, float s)
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

struct sWsHit_330950 {
    char pad_0x00[0x10];
    sWsVec4 dir;      // 0x10
    char pad_0x20[0x20];
    float len;            // 0x40
};

extern "C" int SphereTree_IsBoxCollision(void* self, void* a1, void* a2, void* a3, void* a4);

/**
 * Collides the current sphere tree with an axis-aligned box and normalises the hit's push-out vector.
 *
 * @param bMin  the box's minimum corner (On Tour bMin)
 * @param bMax  the box's maximum corner (On Tour bMax)
 * @param hit  receives the intersection (On Tour tIntersectInfo* ti); direction normalised, length stored
 * @return 1 on a hit (SphereTree_IsBoxCollision), otherwise 0
 */
extern "C" int cWorldSphTree_IsBoxInt(void* self, void* bMin, void* bMax, sWsHit_330950* hit)
{
    if (SphereTree_IsBoxCollision(*(void**)((char*)self + 0x60), bMin, bMax, &hit->dir, hit) != 0) {
        float len = wsLength_330950(hit->dir);
        hit->len = len;
        wsDivEq_330950(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", OverlapInstancesCallback_operatorCall);
#ifdef SKIP_ASM
struct sVEnt_3309D8 { short delta; short index; void* fn; };
typedef int (*Test_3309D8)(void*);

struct sBox_3309D8 {
    float min[4];
    float max[4];
};

struct sBox_3309D8;
typedef sBox_3309D8* (*GetBox_3309D8)(void*);
struct sObj_3309D8 {
    int pad00[3];
    sVEnt_3309D8* vt;
};

struct sNode_3309D8 {
    sNode_3309D8* next;
    int pad04;
    int flags;
    sObj_3309D8* obj;
    int pad10[20];
    float bmin[3];
    float bmax[3];
};

struct sCtx_3309D8 {
    sNode_3309D8** list;
    int* count;
    int pad08;
    sBox_3309D8* box;
};

static inline int overlapNode_3309D8(const sBox_3309D8* b, const sNode_3309D8* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_3309D8(const sBox_3309D8* b, const sBox_3309D8* o)
{
    int r = 0;
    if (b->min[0] <= o->max[0] && b->max[0] >= o->min[0] &&
        b->min[1] <= o->max[1] && b->max[1] >= o->min[1] &&
        b->min[2] <= o->max[2] && b->max[2] >= o->min[2]) {
        r = 1;
    }
    return r;
}

/**
 * Collects the instances of an octree cell whose bounds overlap a query box.
 *
 * Walks the cell's instance list (@p tree +0x20). An instance flagged 0x20 is tested with its stored box (+0x60 min,
 * +0x6C max); one flagged 0x40 asks its object (+0xC) through virtual slot 44 whether it takes part and through slot 45
 * for its box. Each overlapping instance is appended at @p ctx list[*count] (+0x0, +0x4) and the count incremented. The
 * maximum (+0x8) is never checked, so a full list overflows. The octree walkers take it as their cell callback:
 * cOverlapList_getinstances, cOverlapList_getpatchesandinstances, cOverlapList_geteverything, the helpX walkers nash_privy_QueryHelper_helpX1, nash_privy_QueryHelper_helpX3 and nash_privy_QueryHelper_helpX4, and
 * nash_Cell_descend, nash_Cell_descend2 and nash_Cell_descend3.
 *
 * @param ctx  the callback: instance list, count pointer, maximum, query box (On Tour OverlapInstancesCallback, this)
 * @param tree  the octree cell (On Tour const Cell& cell)
 */
extern "C" void OverlapInstancesCallback_operatorCall(sCtx_3309D8* ctx, void* tree)
{
    for (sNode_3309D8* node = *(sNode_3309D8**)((char*)tree + 0x20); node != 0; node = node->next) {
        int flags = node->flags;
        if (flags & 0x20) {
            if (overlapNode_3309D8(ctx->box, node)) {
                int* cnt = ctx->count;
                int n = *cnt;
                ctx->list[n] = node;
                *cnt = n + 1;
            }
        } else if (flags & 0x40) {
            sVEnt_3309D8* vt = node->obj->vt;
            if (((Test_3309D8)vt[44].fn)((char*)node->obj + vt[44].delta)) {
                sVEnt_3309D8* vt2 = node->obj->vt;
                sBox_3309D8* objBox = ((GetBox_3309D8)vt2[45].fn)((char*)node->obj + vt2[45].delta);
                if (overlapBox_3309D8(ctx->box, objBox)) {
                    int* cnt = ctx->count;
                    int n = *cnt;
                    ctx->list[n] = node;
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
extern "C" void nash_Cell_descend(sSphNode_340A40* node, sCtx_3309D8* ctx);
extern "C" int nash_CellKey_proximitytest(void* cell, void* box);
extern "C" void nash_privy_QueryHelper_helpX1(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* box, void* tree, void* cell);

struct sOctNode_330BA8 {
    sSphNode_340A40* child[8];
};

struct sCell_330BA8 {
    int level;
    int x, y, z;
    sOctNode_330BA8* tree;
};

struct sGrid_330BA8 {
    sCell_330BA8 cells[8];
    int unkA0;
};

struct sQuery_330BA8 {
    sGrid_330BA8* grid;             // 0x0
    int unk04;                      // 0x4
    int count;                      // 0x8
    sNode_3309D8* items[0x40];      // 0xC
    unsigned int ntris;             // 0x10C
    void* tris[0x40];               // 0x110
    unsigned int nobjs;             // 0x210
};

static inline void collect_330BA8(sOctNode_330BA8* node, sCtx_3309D8* ctx)
{
    OverlapInstancesCallback_operatorCall(ctx, node);
    if (node->child[0] != 0) nash_Cell_descend(node->child[0], ctx);
    if (node->child[1] != 0) nash_Cell_descend(node->child[1], ctx);
    if (node->child[2] != 0) nash_Cell_descend(node->child[2], ctx);
    if (node->child[3] != 0) nash_Cell_descend(node->child[3], ctx);
    if (node->child[4] != 0) nash_Cell_descend(node->child[4], ctx);
    if (node->child[5] != 0) nash_Cell_descend(node->child[5], ctx);
    if (node->child[6] != 0) nash_Cell_descend(node->child[6], ctx);
    if (node->child[7] != 0) nash_Cell_descend(node->child[7], ctx);
}

// One top-level cell: test it, then either take the whole subtree or recurse into it.
static inline void queryCell_330BA8(sGrid_330BA8* grid, int i, void* box, sCtx_3309D8* ctx, char* flags)
{
    sCell_330BA8* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sOctNode_330BA8* tree = cell->tree;
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
 * Copies the Nash's (+0x0) delete count (+0xA0) to +0x4 and clears the instance count (+0x8), then runs an instance
 * collector (array +0xC, count +0x8, maximum 0x40, @p box) over every octant that has a root cell. When
 * nash_CellKey_proximitytest answers 0 (inside), the root goes to OverlapInstancesCallback_operatorCall and its
 * children to nash_Cell_descend (whole subtrees); when it answers 2 (crossing), the cell goes to nash_privy_QueryHelper_helpX1. Finally
 * sets the patch count (+0x10C) and the typed count (+0x210) to -1, marking those lists as not collected. Nothing in
 * the code calls it directly.
 *
 * @param box  the query box: min at +0x0, max at +0x10
 */
extern "C" void cOverlapList_getinstances(sQuery_330BA8* self, sBox_3309D8* box)
{
    sCtx_3309D8 ctx;
    char flags[8];
    sGrid_330BA8* grid = self->grid;
    sCtx_3309D8* pctx = &ctx;
    self->unk04 = grid->unkA0;
    ctx.list = self->items;
    ctx.count = &self->count;
    ctx.pad08 = 0x40;
    ctx.box = box;
    self->count = 0;
    queryCell_330BA8(grid, 0, box, pctx, flags);
    queryCell_330BA8(grid, 1, box, pctx, flags);
    queryCell_330BA8(grid, 2, box, pctx, flags);
    queryCell_330BA8(grid, 3, box, pctx, flags);
    queryCell_330BA8(grid, 4, box, pctx, flags);
    queryCell_330BA8(grid, 5, box, pctx, flags);
    queryCell_330BA8(grid, 6, box, pctx, flags);
    queryCell_330BA8(grid, 7, box, pctx, flags);
    self->ntris = 0xFFFFFFFF;
    self->nobjs = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_gettyped);
#ifdef SKIP_ASM
struct sOctCollect;
extern "C" void nash_Cell_descend1(void* self, sOctCollect* out);
extern "C" void nash_privy_QueryHelper_helpX2(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* box, void* tree, void* cell);

struct sTag_331450 {
};

struct sOctNode_331450 {
    void* child[8];         // 0x00
    int pad20[2];           // 0x20
    void** objs;            // 0x28
};

struct sCell_331450 {
    int level;
    int x, y, z;
    sOctNode_331450* tree;
};

struct sGrid_331450 {
    sCell_331450 cells[8];
    int unkA0;
};

struct sQuery_331450 {
    sGrid_331450* grid;             // 0x0
    int unk04;                      // 0x4
    unsigned int count;             // 0x8
    void* items[0x40];              // 0xC
    unsigned int ntris;             // 0x10C
    void* tris[0x40];               // 0x110
    unsigned int nobjs;             // 0x210
    void* objs[0x200];              // 0x214
};

static inline void collect_331450(sOctNode_331450* node, sCtx_3309D8* ctx)
{
    void** p;
    for (p = node->objs; p != 0; p = (void**)*p) {
        ctx->list[(*ctx->count)++] = (sNode_3309D8*)p;
    }
    if (node->child[0] != 0) nash_Cell_descend1(node->child[0], (sOctCollect*)ctx);
    if (node->child[1] != 0) nash_Cell_descend1(node->child[1], (sOctCollect*)ctx);
    if (node->child[2] != 0) nash_Cell_descend1(node->child[2], (sOctCollect*)ctx);
    if (node->child[3] != 0) nash_Cell_descend1(node->child[3], (sOctCollect*)ctx);
    if (node->child[4] != 0) nash_Cell_descend1(node->child[4], (sOctCollect*)ctx);
    if (node->child[5] != 0) nash_Cell_descend1(node->child[5], (sOctCollect*)ctx);
    if (node->child[6] != 0) nash_Cell_descend1(node->child[6], (sOctCollect*)ctx);
    if (node->child[7] != 0) nash_Cell_descend1(node->child[7], (sOctCollect*)ctx);
}

static inline int queryCell_331450(sGrid_331450* grid, int i, void* box, sCtx_3309D8* ctx)
{
    sCell_331450* cell = &grid->cells[i];
    sOctNode_331450* tree;
    sTag_331450 tag;
    char* flag;
    int r = 1;
    if (grid->cells[i].tree != 0) {
        tree = cell->tree;
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
 * Copies the Nash's (+0x0) delete count (+0xA0) to +0x4, clears the typed count (+0x210) and walks the eight octants
 * with a typed collector (array +0x214, count +0x210, maximum 0x200, @p box). An inside cell
 * (nash_CellKey_proximitytest 0) has its typed list (cell +0x28) appended and its children walked whole by
 * nash_Cell_descend1; a crossing cell (2) goes to nash_privy_QueryHelper_helpX2. The nodes themselves are not box-tested and the maximum is
 * never checked (likely bug: more than 0x200 typed nodes in the visited cells overrun the array). Sets the instance
 * count (+0x8) and the patch count (+0x10C) to -1. Called by cWorldLightMan_getRealTimeLights.
 *
 * @param box  the query box: min at +0x0, max at +0x10
 */
extern "C" void cOverlapList_gettyped(sQuery_331450* self, sBox_3309D8* box)
{
    sCtx_3309D8 ctx;
    sGrid_331450* grid = self->grid;
    sCtx_3309D8* pctx = &ctx;
    self->unk04 = grid->unkA0;
    ctx.list = (sNode_3309D8**)self->objs;
    ctx.count = (int*)&self->nobjs;
    ctx.pad08 = 0x200;
    ctx.box = box;
    self->nobjs = 0;
    queryCell_331450(grid, 0, box, pctx);
    queryCell_331450(grid, 1, box, pctx);
    queryCell_331450(grid, 2, box, pctx);
    queryCell_331450(grid, 3, box, pctx);
    queryCell_331450(grid, 4, box, pctx);
    queryCell_331450(grid, 5, box, pctx);
    queryCell_331450(grid, 6, box, pctx);
    queryCell_331450(grid, 7, box, pctx);
    self->ntris = 0xFFFFFFFF;
    self->count = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_getpatchesandinstances);
#ifdef SKIP_ASM
struct sTree_340C20;
extern "C" void nash_Cell_descend2(sTree_340C20* tree, sCtx_3309D8* ctx);
extern "C" void nash_privy_QueryHelper_helpX3(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* vbox, void* vnode, void* vcell);

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

struct sCell_331E90 {
    int level;
    int x, y, z;
    sTree_331E90* tree;
};

struct sGrid_331E90 {
    sCell_331E90 cells[8];
    int unkA0;
};

struct sQuery_331E90 {
    sGrid_331E90* grid;             // 0x0
    int unk04;                      // 0x4
    int count;                      // 0x8
    sNode_3309D8* items[0x40];      // 0xC
    int ntris;                      // 0x10C
    sNode_3309D8* tris[0x40];       // 0x110
    unsigned int nobjs;             // 0x210
};

struct sCtx2_331E90 {
    sCtx_3309D8 nodes;
    sCtx_3309D8 objs;
};

static inline int overlapObj_331E90(const sBox_3309D8* b, const sObj_331E90* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void collect_331E90(sTree_331E90* tree, sCtx_3309D8* ctx)
{
    OverlapInstancesCallback_operatorCall(ctx, tree);
    sCtx_3309D8* q = (sCtx_3309D8*)((char*)ctx + 0x10);
    for (sObj_331E90* p = tree->objs; p != 0; p = p->next) {
        if (overlapObj_331E90(q->box, p)) {
            int* cnt = q->count;
            int n = *cnt;
            q->list[n] = (sNode_3309D8*)p;
            *cnt = n + 1;
        }
    }
    if (tree->child[0] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[0], ctx);
    if (tree->child[1] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[1], ctx);
    if (tree->child[2] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[2], ctx);
    if (tree->child[3] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[3], ctx);
    if (tree->child[4] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[4], ctx);
    if (tree->child[5] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[5], ctx);
    if (tree->child[6] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[6], ctx);
    if (tree->child[7] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[7], ctx);
}

struct sTag_331E90 {
};

static inline void queryCell_331E90(sGrid_331E90* grid, int i, sBox_3309D8* box, sCtx_3309D8* ctx)
{
    sCell_331E90* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sTree_331E90* tree = cell->tree;
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
 * Copies the Nash's (+0x0) delete count (+0xA0) to +0x4 and clears the instance count (+0x8) and the patch count
 * (+0x10C). For each of the eight octants with a root cell, nash_CellKey_proximitytest 0 (inside) runs
 * OverlapInstancesCallback_operatorCall on the cell, appends each patch of its patch list (+0x24) whose box (+0x158
 * min, +0x164 max) overlaps @p box to the patch array (+0x110), and walks the children whole through nash_Cell_descend2; 2
 * (crossing) hands the cell to nash_privy_QueryHelper_helpX3. The patch maximum (0x40) is stored but never checked. Sets the typed
 * count (+0x210) to -1. Nothing in the code calls it directly.
 *
 * @param box  the query box: min at +0x0, max at +0x10
 */
extern "C" void cOverlapList_getpatchesandinstances(sQuery_331E90* self, sBox_3309D8* box)
{
    sCtx2_331E90 ctx;
    self->unk04 = self->grid->unkA0;
    ctx.nodes.list = self->items;
    ctx.nodes.count = &self->count;
    ctx.nodes.pad08 = 0x40;
    ctx.nodes.box = box;
    ctx.objs.list = self->tris;
    ctx.objs.count = &self->ntris;
    ctx.objs.pad08 = 0x40;
    ctx.objs.box = box;
    sCtx_3309D8* objs = &ctx.objs;
    self->count = 0;
    *objs->count = 0;
    sGrid_331E90* grid = self->grid;
    sCtx_3309D8* pctx = &ctx.nodes;
    queryCell_331E90(grid, 0, box, pctx);
    queryCell_331E90(grid, 1, box, pctx);
    queryCell_331E90(grid, 2, box, pctx);
    queryCell_331E90(grid, 3, box, pctx);
    queryCell_331E90(grid, 4, box, pctx);
    queryCell_331E90(grid, 5, box, pctx);
    queryCell_331E90(grid, 6, box, pctx);
    queryCell_331E90(grid, 7, box, pctx);
    self->nobjs = 0xFFFFFFFF;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_geteverything);
#ifdef SKIP_ASM
struct sTree_340DC0;
struct sCtx_340DC0;
extern "C" void nash_Cell_descend3(sTree_340DC0* self, sCtx_340DC0* ctx);
extern "C" void nash_privy_QueryHelper_helpX4(char* flag, sCtx_340DC0* ctx, sCtx_340DC0* ctx2, void* vbox, void* vnode, void* vcell);

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

struct sCell_332DB8 {
    int level;
    int x, y, z;
    sTree_332DB8* tree;
};

struct sGrid_332DB8 {
    sCell_332DB8 cells[8];
    int unkA0;
};

struct sQuery_332DB8 {
    sGrid_332DB8* grid;             // 0x0
    int unk04;                      // 0x4
    int count;                      // 0x8
    sNode_3309D8* items[0x40];      // 0xC
    int ntris;                      // 0x10C
    sNode_3309D8* tris[0x40];       // 0x110
    int nobjs;                      // 0x210
    sNode_3309D8* objs[0x200];      // 0x214
};

struct sCtx3_332DB8 {
    sCtx_3309D8 nodes;
    sCtx_3309D8 objs;
    sCtx_3309D8 all;
};

struct sTag_332DB8 {
};

static inline int overlapObj_332DB8(const sBox_3309D8* b, const sObj_332DB8* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void add_332DB8(sCtx_3309D8* s, void* p)
{
    int* cnt = s->count;
    int n = *cnt;
    s->list[n] = (sNode_3309D8*)p;
    *cnt = n + 1;
}

static inline void collect_332DB8(sTree_332DB8* tree, sCtx3_332DB8* ctx)
{
    OverlapInstancesCallback_operatorCall(&ctx->nodes, tree);
    sCtx_3309D8* s = &ctx->objs;
    for (sObj_332DB8* p = tree->objs; p != 0; p = p->next) {
        if (overlapObj_332DB8(s->box, p)) {
            add_332DB8(s, p);
        }
    }
    s = &ctx->all;
    for (void** q = tree->all; q != 0; q = (void**)*q) {
        add_332DB8(s, q);
    }
    if (tree->child[0]) nash_Cell_descend3((sTree_340DC0*)tree->child[0], (sCtx_340DC0*)ctx);
    if (tree->child[1]) nash_Cell_descend3((sTree_340DC0*)tree->child[1], (sCtx_340DC0*)ctx);
    if (tree->child[2]) nash_Cell_descend3((sTree_340DC0*)tree->child[2], (sCtx_340DC0*)ctx);
    if (tree->child[3]) nash_Cell_descend3((sTree_340DC0*)tree->child[3], (sCtx_340DC0*)ctx);
    if (tree->child[4]) nash_Cell_descend3((sTree_340DC0*)tree->child[4], (sCtx_340DC0*)ctx);
    if (tree->child[5]) nash_Cell_descend3((sTree_340DC0*)tree->child[5], (sCtx_340DC0*)ctx);
    if (tree->child[6]) nash_Cell_descend3((sTree_340DC0*)tree->child[6], (sCtx_340DC0*)ctx);
    if (tree->child[7]) nash_Cell_descend3((sTree_340DC0*)tree->child[7], (sCtx_340DC0*)ctx);
}

static inline void queryCell_332DB8(sGrid_332DB8* grid, int i, sBox_3309D8* box, sCtx3_332DB8* ctx)
{
    sCell_332DB8* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sTree_332DB8* tree = cell->tree;
        sTag_332DB8 tag;
        char* flag = (char*)&tag;
        int r = nash_CellKey_proximitytest(cell, box);
        if (r == 0) {
            collect_332DB8(tree, ctx);
        } else if (r == 2) {
            nash_privy_QueryHelper_helpX4(flag, (sCtx_340DC0*)ctx, (sCtx_340DC0*)ctx, box, tree, cell);
        }
    }
}

/**
 * Collects the instances, the patches and the typed nodes of the octree cells that overlap @p box into the overlap
 * list.
 *
 * Copies the Nash's (+0x0) delete count (+0xA0) to +0x4 and clears all three counts. Instances go through
 * OverlapInstancesCallback_operatorCall into +0xC (count +0x8), patches whose box (+0x158 min, +0x164 max) overlaps
 * @p box into +0x110 (count +0x10C), and every typed node of a visited cell (cell +0x28) into +0x214 (count +0x210)
 * with no box test and no maximum check. Inside octants (nash_CellKey_proximitytest 0) are walked whole, the children
 * through nash_Cell_descend3; crossing ones (2) go to nash_privy_QueryHelper_helpX4. Called by cRider_loadRider, cRider_putAt,
 * cRider_updateOverlapList and cRider_updateSplit1.
 *
 * @param box  the query box: min at +0x0, max at +0x10
 */
extern "C" void cOverlapList_geteverything(sQuery_332DB8* self, sBox_3309D8* box)
{
    sCtx3_332DB8 ctx;
    self->unk04 = self->grid->unkA0;
    ctx.nodes.list = self->items;
    ctx.nodes.count = &self->count;
    ctx.nodes.pad08 = 0x40;
    ctx.nodes.box = box;
    ctx.objs.list = self->tris;
    ctx.objs.count = &self->ntris;
    ctx.objs.pad08 = 0x40;
    ctx.objs.box = box;
    sCtx_3309D8* objs = &ctx.objs;
    self->count = 0;
    *objs->count = 0;
    ctx.all.list = self->objs;
    ctx.all.count = &self->nobjs;
    ctx.all.pad08 = 0x200;
    ctx.all.box = box;
    sCtx_3309D8* all = &ctx.all;
    *all->count = 0;
    sGrid_332DB8* grid = self->grid;
    sCtx3_332DB8* pctx = &ctx;
    queryCell_332DB8(grid, 0, box, pctx);
    queryCell_332DB8(grid, 1, box, pctx);
    queryCell_332DB8(grid, 2, box, pctx);
    queryCell_332DB8(grid, 3, box, pctx);
    queryCell_332DB8(grid, 4, box, pctx);
    queryCell_332DB8(grid, 5, box, pctx);
    queryCell_332DB8(grid, 6, box, pctx);
    queryCell_332DB8(grid, 7, box, pctx);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_terrainIntersect);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sVec4_333EF8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sVec4_333EF8 D_004FF120_333EF8[] __asm__("t4vect_Zero");

struct sQuery_333EF8 {
    char pad00[0x20];
    float max[4];
    float min[4];
};

struct sTriObj_333EF8 {
    char pad00[8];
    short id;
    unsigned short flags;
    char pad0C[0x14C];
    float bmin[3];
    float bmax[3];
};

struct sHit_333EF8 {
    sVec4_333EF8 pos;   // 0x00
    sVec4_333EF8 nrm;   // 0x10
    sVec4_333EF8 v20;   // 0x20
    sVec4_333EF8 v30;   // 0x30
    float dist;         // 0x40
    int type;           // 0x44
    int pad48;
    int id;             // 0x4C
    int f50;            // 0x50
    void* obj;          // 0x54
    int f58;            // 0x58
    int f5C;            // 0x5C
    int f60;            // 0x60
    int f64;            // 0x64
    int f68;            // 0x68
    float u;            // 0x6C
    float v;            // 0x70
    int pad74[3];
};

struct sVEnt_333EF8 { short delta; short index; void* fn; };
struct sBox_333EF8;
typedef int (*Test_333EF8)(void*);
typedef sBox_333EF8* (*GetBox_333EF8)(void*);

struct sBox_333EF8 {
    float min[4];
    float max[4];
};

struct sObj_333EF8 {
    int pad00[3];
    sVEnt_333EF8* vt;
};

struct sNode_333EF8 {
    void* next;
    int pad04;
    int flags;
    sObj_333EF8* obj;
    int pad10[20];
    float bmin[3];
    float bmax[3];
    int pad78[4];
    int active;
};

struct sOwner_333EF8 {
    char pad00[0xA4];
    void* cache;
};

struct sTree_333EF8 {
    sOwner_333EF8* owner;           // 0x0
    int pad04;
    unsigned int count;             // 0x8
    sNode_333EF8* items[0x40];      // 0xC
    unsigned int ntris;             // 0x10C
    sTriObj_333EF8* tris[1];        // 0x110
};

extern "C" void cAIFwdDiffCache_GetFwdDiff(void* cache, void* src, void** out);
struct sVec4_335960;
struct sHit_003342D0;
extern "C" int cWorldIntersectType_IsPatchInt(void* q, void* obj, void* node, sVec4_335960* pos, sVec4_335960* nrm, float* dist, sVec4_335960* uv, int arg);
extern "C" int intersect_instanceIntersect(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapTri_333EF8(const sQuery_333EF8* q, const sTriObj_333EF8* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapNode_333EF8(const sQuery_333EF8* q, const sNode_333EF8* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_333EF8(const sQuery_333EF8* q, const sBox_333EF8* o)
{
    int r = 0;
    if (q->max[0] > o->min[0] && q->min[0] < o->max[0] &&
        q->max[1] > o->min[1] && q->min[1] < o->max[1] &&
        q->max[2] > o->min[2] && o->max[2] > q->min[2]) {
        r = 1;
    }
    return r;
}

static inline void reset_333EF8(sHit_333EF8* h)
{
    h->type = 2;
    h->obj = 0;
    h->f50 = 0;
    h->f58 = 0;
    h->f5C = -1;
    h->f60 = -1;
    h->f64 = -1;
    h->id = -1;
    h->v20 = *D_004FF120_333EF8;
    h->v30 = *D_004FF120_333EF8;
    h->f68 = 0;
    h->u = 0;
    h->v = 0;
}

/**
 * Intersects a query with the patches and instances already in the overlap list, storing the hits; returns their count.
 *
 * Patches (array +0x110, count +0x10C) with both bits of 0x41 set in their flags (+0xA) and a box (+0x158 min, +0x164
 * max) overlapping the query's box (max +0x20, min +0x30) get their forward differences from the Nash's (+0x0) AI cache
 * (+0xA4) through cAIFwdDiffCache_GetFwdDiff and are tested with cWorldIntersectType_IsPatchInt. Each hit resets its
 * 0x80-byte record (On Tour tIntersectInfo::Init) and stores position, normal, the patch's surface id (+0x8), the
 * patch, the parameter, and u and v. Then instances (array +0xC, count +0x8) whose +0x88 word is set: a flag 0x20 one
 * is box-tested against its stored box (+0x60 min, +0x6C max), a flag 0x40 one needs a non-null object (+0xC) that
 * answers true through virtual slot 44 and whose slot 45 box overlaps; each passes to intersect_instanceIntersect (mode
 * 2) with the room left. Patch hits are not checked against @p cap. Called by cOverlapList_terrainIntersect1 and
 * cGroundMotion_setupGroundVectors.
 *
 * @param vself  the overlap list
 * @param vq  the query (On Tour cWorldIntersectType)
 * @param vhits  out: the hit records, 0x80 bytes each
 * @param cap  the number of records @p vhits holds
 * @param hitCache  passed to cWorldIntersectType_IsPatchInt as its last argument (On Tour tIntHint *hint)
 * @return the number of hits stored
 */
extern "C" int cOverlapList_terrainIntersect(void* vself, void* vq, sHit_003342D0* vhits, int cap, int hitCache)
{
    sTree_333EF8* self = (sTree_333EF8*)vself;
    sQuery_333EF8* query = (sQuery_333EF8*)vq;
    sHit_333EF8* hits = (sHit_333EF8*)vhits;
    int n = 0;
    for (unsigned int i = 0; i < self->ntris; i++) {
        sTriObj_333EF8* obj = self->tris[i];
        if ((obj->flags & 0x41) != 65) {
            continue;
        }
        if (!overlapTri_333EF8(query, obj)) {
            continue;
        }
        sVec4_333EF8 uv;
        sVec4_333EF8 pos;
        sVec4_333EF8 nrm;
        void* node;
        float dist;
        cAIFwdDiffCache_GetFwdDiff(self->owner->cache, obj, &node);
        if (!cWorldIntersectType_IsPatchInt(query, obj, node, (sVec4_335960*)&pos, (sVec4_335960*)&nrm, &dist, (sVec4_335960*)&uv, hitCache)) {
            continue;
        }
        reset_333EF8(&hits[n]);
        hits[n].pos = pos;
        hits[n].nrm = nrm;
        hits[n].id = obj->id;
        hits[n].obj = obj;
        hits[n].dist = dist;
        hits[n].u = uv.x;
        hits[n].v = uv.y;
        n++;
    }
    for (unsigned int j = 0; j < self->count; j++) {
        sNode_333EF8* node = self->items[j];
        if (node->active == 0) {
            continue;
        }
        int flags = node->flags;
        if (flags & 0x20) {
            if (overlapNode_333EF8(query, node)) {
                n += intersect_instanceIntersect(query, node, (char*)hits + n * 0x80, cap - n, 2);
            }
        } else if (flags & 0x40) {
            if (node->obj == 0) {
                continue;
            }
            sVEnt_333EF8* vt = node->obj->vt;
            if (((Test_333EF8)vt[44].fn)((char*)node->obj + vt[44].delta)) {
                sVEnt_333EF8* vt2 = node->obj->vt;
                sBox_333EF8* objBox = ((GetBox_333EF8)vt2[45].fn)((char*)node->obj + vt2[45].delta);
                if (overlapBox_333EF8(query, objBox)) {
                    n += intersect_instanceIntersect(query, node, (char*)hits + n * 0x80, cap - n, 2);
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
struct sHit_003342D0 {
    float pad0[16];
    float dist;             // 0x40
    int pad44[3];
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[10];
    sHit_003342D0() {}
} __attribute__((aligned(16)));

struct sVEnt_003342D0 { short delta; short index; float (*fn)(void*); };
extern "C" int cOverlapList_terrainIntersect(void* self, void* q, sHit_003342D0* hits, int max, int a4);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_003342D0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int isBetter_003342D0(sHit_003342D0* a, sHit_003342D0* b)
{
    if (a->obj != 0) {
        if (b->obj == 0) {
            return 1;
        }
        return *(unsigned int*)(a->obj + 0x78) < *(unsigned int*)(b->obj + 0x78);
    }
    if (b->obj != 0) {
        return 0;
    }
    return *(unsigned int*)(a->tri + 0x150) < *(unsigned int*)(b->tri + 0x150);
}

/**
 * Returns the parameter of the overlap-list terrain hit nearest the query's ideal parameter, copying the hit to
 * @p out.
 *
 * Collects up to 64 hits with cOverlapList_terrainIntersect into a stack array and returns -1.0 when there are none. Otherwise picks
 * the hit whose parameter (+0x40) is closest to the query's virtual slot 3 (On Tour GetIdealParam). On a tie an
 * instance hit (+0x50) beats a patch hit (+0x54), two instances compare their +0x78 words and two patches their +0x150
 * words, the smaller winning (On Tour orderIntersectInfo). Called by the rider motion code (cAirMotion_intersectBoard,
 * cGroundMotion_checkWallCollisions, cWipeoutMotion_intersectGround and others).
 *
 * @param query  the query (On Tour cWorldIntersectType)
 * @param out  out: the chosen hit (0x80 bytes)
 * @param hitCache  passed through to cOverlapList_terrainIntersect (On Tour tIntHint *hint)
 * @return the chosen hit's parameter, or -1.0
 */
extern "C" float cOverlapList_terrainIntersect1(void* self, char* query, sHit_003342D0* out, int hitCache)
{
    sHit_003342D0 hits[64];
    int n = cOverlapList_terrainIntersect(self, query, hits, 0x40, hitCache);
    if (n == 0) {
        return -1.0f;
    }
    int best = 0;
    sVEnt_003342D0* vt = *(sVEnt_003342D0**)(query + 0x50);
    float ref = vt[3].fn(query + vt[3].delta);
    float bestDiff = absf_003342D0(hits[0].dist - ref);
    for (int i = 1; i < n; i++) {
        float diff = absf_003342D0(hits[i].dist - ref);
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
    return out->dist;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cOverlapList_staticObjectIntersect);
#ifdef SKIP_ASM
struct sVEnt_334458 { short delta; short index; void* fn; };
struct sBox_334458;
typedef int (*Test_334458)(void*);
typedef sBox_334458* (*GetBox_334458)(void*);

struct sBox_334458 {
    float min[4];
    float max[4];
};

struct sQuery_334458 {
    char pad00[0x20];
    float max[4];
    float min[4];
};

struct sObj_334458 {
    int pad00[3];
    sVEnt_334458* vt;
};

struct sNode_334458 {
    void* next;
    int pad04;
    int flags;
    sObj_334458* obj;
    int pad10[20];
    float bmin[3];
    float bmax[3];
};

struct sList_334458 {
    int pad00[2];
    unsigned int count;
    sNode_334458* items[1];
};

extern "C" int intersect_instanceIntersect(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapNode_334458(const sQuery_334458* q, const sNode_334458* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_334458(const sQuery_334458* q, const sBox_334458* o)
{
    int r = 0;
    if (q->max[0] > o->min[0] && q->min[0] < o->max[0] &&
        q->max[1] > o->min[1] && q->min[1] < o->max[1] &&
        q->max[2] > o->min[2] && o->max[2] > q->min[2]) {
        r = 1;
    }
    return r;
}

/**
 * Intersects a query with the instances in the overlap list, storing the hits; returns their count.
 *
 * Walks the instance array (+0xC, count +0x8): a flag 0x20 instance is box-tested against its stored box (+0x60 min,
 * +0x6C max), a flag 0x40 one asks its object (+0xC) through virtual slots 44 (takes part) and 45 (box). Each one that
 * overlaps the query's box (max +0x20, min +0x30) goes to intersect_instanceIntersect with the room left, in mode 2
 * when @p full is set and 1 otherwise. Unlike cOverlapList_terrainIntersect it neither skips instances whose +0x88 word is 0 nor checks
 * the object for null. Called by cRider_objectIntersect.
 *
 * @param query  the query (On Tour cWorldIntersectType)
 * @param buf  out: the hit records, 0x80 bytes each
 * @param cap  the number of records @p buf holds
 * @param full  nonzero selects mode 2 instead of 1 (On Tour includeridable)
 * @return the number of hits stored
 */
extern "C" int cOverlapList_staticObjectIntersect(sList_334458* self, sQuery_334458* query, char* buf, int cap, int full)
{
    int mode = 1;
    if (full) mode = 2;
    int n = 0;
    for (unsigned int i = 0; i < self->count; i++) {
        sNode_334458* node = self->items[i];
        int flags = node->flags;
        if (flags & 0x20) {
            if (overlapNode_334458(query, node)) {
                n += intersect_instanceIntersect(query, node, buf + n * 0x80, cap - n, mode);
            }
        } else if (flags & 0x40) {
            sVEnt_334458* vt = node->obj->vt;
            if (((Test_334458)vt[44].fn)((char*)node->obj + vt[44].delta)) {
                sVEnt_334458* vt2 = node->obj->vt;
                sBox_334458* objBox = ((GetBox_334458)vt2[45].fn)((char*)node->obj + vt2[45].delta);
                if (overlapBox_334458(query, objBox)) {
                    n += intersect_instanceIntersect(query, node, buf + n * 0x80, cap - n, mode);
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
struct sBox_00334680 {
    sWsVec4 min;
    sWsVec4 max;
};
extern "C" void cRailModifier_splineIntersect(void* a0, sBox_00334680* box, sBox_00334680* box2, sWsVec4* pos, int mask, int* cnt, float* dist, int a7);
extern "C" void cFlexRailNode_splineIntersect(void* a0, sBox_00334680* box, sBox_00334680* box2, sWsVec4* pos, int mask, int* cnt, float* dist, int a7);
extern "C" void intersect_staticSplineSegmentIntersect(void* obj, sBox_00334680* box, sWsVec4* pos, int* cnt, float* dist, int a5);

// PORT: PS2-only VU0 inline asm (in-place vector subtract).
static inline void wsSubEq_334680(sWsVec4& a, const sWsVec4& b)
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
static inline void wsAddEq_334680(sWsVec4& a, const sWsVec4& b)
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
 * Builds a box of half-size @p testRadius around @p pos and, for each listed object (+0x210 count, +0x214 array), tests
 * rails with cRailModifier_splineIntersect, flex rails with cFlexRailNode_splineIntersect and instances whose filter flags match
 * @p mask with intersect_staticSplineSegmentIntersect, collecting hits into @p intInfo.
 *
 * @param pos   the point to test
 * @param intInfo    out: the hit list (On Tour intInfo)
 * @param mask  which spline types to accept
 * @param testRadius     the test radius
 * @return the number of hits
 */
extern "C" int cOverlapList_splineIntersect(char* self, sWsVec4* pos, int intInfo, int mask, float testRadius)
{
    sBox_00334680 box;
    sWsVec4 ext;
    int cnt;
    float dist;
    float zero = 0.0f;
    cnt = 0;
    dist = zero;
    box.min = *pos;
    box.max = *pos;
    ext.x = testRadius;
    ext.y = testRadius;
    ext.z = testRadius;
    ext.w = zero;
    wsSubEq_334680(box.min, ext);
    wsAddEq_334680(box.max, ext);
    for (unsigned int i = 0; i < *(unsigned int*)(self + 0x210); i++) {
        char* spaceNode = ((char**)(self + 0x214))[i];
        int type = *(int*)(spaceNode + 8);
        if (type == 2) {
            cRailModifier_splineIntersect(*(void**)(spaceNode + 0xC), &box, &box, pos, mask, &cnt, &dist, intInfo);
        } else if (type == 3) {
            cFlexRailNode_splineIntersect(*(void**)(spaceNode + 0xC), &box, &box, pos, mask, &cnt, &dist, intInfo);
        } else if (type == 1) {
            if (*(int*)(*(char**)(spaceNode + 0x68) + 0x1C) & mask) {
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
extern "C" void cBXNash_initTheAIFwdDiffCache(void* self)
{
    void* cache = cAIFwdDiffCache_cAIFwdDiffCache(cMemMan_alloc(0x10, str_AIFwdDiffCache, 0, 0));
    *(void**)((char*)self + 0xA4) = cache;
    cAIFwdDiffCache_Init(cache, 40);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_purgeTheAIFwdDiffCache);
#ifdef SKIP_ASM
extern "C" void cAIFwdDiffCache__cAIFwdDiffCache(void* p, int flags);

/**
 * Deletes the AI forward-difference cache (+0xA4) built by cBXNash_initTheAIFwdDiffCache, then clears the pointer.
 */
extern "C" void cBXNash_purgeTheAIFwdDiffCache(void* self)
{
    void* fwdDiffCache = *(void**)((char*)self + 0xA4);
    if (fwdDiffCache != 0) {
        cAIFwdDiffCache__cAIFwdDiffCache(fwdDiffCache, 3);
    }
    *(void**)((char*)self + 0xA4) = 0;
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

// A polymorphic world object as the list walkers see it (vtable pointer at 0xC).
struct sWsVObj {
    int pad00[3];
    sWsVtSlot* vt;
};

// Constant vector table (0x004FF120; 00341388 also reads it through its ctor vector type).
extern t4Vector D_004FF120_4888[] __asm__("t4vect_Zero");

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

// A bone of an instance's skeleton (16 bytes): parent index, collision mesh, local matrix.
struct sWsBone_4888 {
    int parent;         // 0x0
    char* mesh;         // 0x4
    int pad08;
    sWsMat_4888* mat;   // 0xC
};

struct sWsBoneTbl_4888 {
    int pad00;
    int count;              // 0x4
    sWsBone_4888* bones;    // 0x8
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

// A world instance: collision object, world matrix, skeleton, scale, definition.
struct sWsInst_4888 {
    char pad00[0xC];
    sWsVObj* obj;               // 0xC
    sWsMat_4888 mat;            // 0x10
    char pad50[0x30];
    sWsBoneTbl_4888* tbl;       // 0x80
    float scale;                // 0x84
    sWsInstDef_4888* def;       // 0x88
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

// The query object (vtable at 0x50).
struct sWsQuery_4888 {
    char pad00[0x50];
    sWsVtSlot* vt;              // 0x50
};

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

// Collects the triangle hits of instance `node` against query q into buf (mode 1: unnamed parts only, mode 2: named
// parts only); bone matrices come from the instance's animated object or are built here from the skeleton.
extern "C" int func_4888(sWsQuery_4888* q, sWsInst_4888* node, sWsHit_4888* buf, int cap, int mode) __asm__("intersect_instanceIntersect");
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
extern "C" int func_4888(sWsQuery_4888* intersectType, sWsInst_4888* node, sWsHit_4888* buf, int cap, int mode)
{
    if (node->obj != 0) {
        sWsVtSlot* vt = node->obj->vt;
        if (((Test_4888)vt[38].fn)((char*)node->obj + vt[38].delta)) {
            sWsVtSlot* vt2 = node->obj->vt;
            return ((Query_4888)vt2[39].fn)((char*)node->obj + vt2[39].delta, intersectType, buf, cap, mode);
        }
    }
    int type = node->def->type;
    if (type != 3 && type != 1 && type != 2)
        return 0;
    sWsVObj* obj = node->obj;
    sWsMat_4888 mats[24];
    sWsMat_4888* pMat = &node->mat;
    int useLocal = 1;
    sWsMat_4888* boneMats = mats;
    if (obj != 0) {
        sWsVtSlot* vt = obj->vt;
        if (((Test_4888)vt[26].fn)((char*)obj + vt[26].delta)) {
            useLocal = 0;
            sWsVtSlot* vt2 = obj->vt;
            boneMats = ((GetMat_4888)vt2[28].fn)((char*)obj + vt2[28].delta, node);
        } else {
            sWsVtSlot* vt2 = obj->vt;
            pMat = ((GetMat_4888)vt2[25].fn)((char*)obj + vt2[25].delta, node);
        }
    }
    sWsBoneTbl_4888* tbl = node->tbl;
    sWsBone_4888* bone = tbl->bones;
    float instScale = node->scale;
    t4Vector scl;
    scl.x = instScale;
    scl.y = instScale;
    scl.z = instScale;
    scl.w = 1.0f;
    int n = 0;
    sWsMat_4888* boneMat = boneMats;
    for (int i = 0, k = 0; i < tbl->count; i++, bone++, boneMat++) {
        if (useLocal) {
            int parent = bone->parent;
            if (parent != -1) {
                sWsMat_4888 productCopy;
                sWsMat_4888 scaledLocal;
                t4Vector res;
                t4Vector translation;
                sWsMat_4888* lm = bone->mat;
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
                sWsMat_4888* lm = bone->mat;
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
        if (bone->mesh == 0)
            continue;
        if (mode == 2) {
            if (GetPart_4888(node->def, i)->id == -1) {
                if (type != 2)
                    k++;
                continue;
            }
        } else if (mode == 1) {
            if (GetPart_4888(node->def, i)->id != -1) {
                if (type != 2)
                    k++;
                continue;
            }
        }
        sWsHit_4888 ii[64];
        sWsMat_4888 inv = *boneMat;
        InvertOrtho_4888(inv);
        int inum = 0;
        void* tri = 0;
        ((SetXf_4888)intersectType->vt[1].fn)((char*)intersectType + intersectType->vt[1].delta, 1.0f / node->scale, &inv);
        if (type == 2) {
            inum = ((Tris_4888)intersectType->vt[7].fn)((char*)intersectType + intersectType->vt[7].delta, bone->mesh, bone->mesh + 0xC, ii, 64);
        } else {
            if (((Hit_4888)intersectType->vt[9].fn)((char*)intersectType + intersectType->vt[9].delta, bone->mesh, bone->mesh + 0xC)) {
                if (type == 3) {
                    tri = tSphTreeList_getMeshTree(cWorld_getPolyModelInfo_4888(cWorld_m_pTheWorld, node), k);
                    inum = ((Mesh_4888)intersectType->vt[6].fn)((char*)intersectType + intersectType->vt[6].delta, tri, ii, 64);
                } else {
                    inum = cWorldIntersectType_IsMeshInt_cap(intersectType, tPolyModelList_getMeshPolyModel_4888(cWorld_getSphereTreeInfo_4888(cWorld_m_pTheWorld, node), k),
                                             *(int*)(bone->mesh + 0x18) & 1, ii, 64);
                }
            }
            k++;
        }
        ((Reset_4888)intersectType->vt[2].fn)((char*)intersectType + intersectType->vt[2].delta);
        for (int j = 0; j < inum; j++) {
            ii[j].nrm = vu0MtxMul_4888(boneMat, ii[j].nrm);
            buf[n] = ii[j];
            buf[n].pos = vu0MtxMul_4888(boneMat, MulVec_4888(buf[n].pos, scl));
            buf[n].type = 1;
            buf[n].inst = node;
            buf[n].bone = i;
            buf[n].id = -1;
            buf[n].tri = tri;
            buf[n].v20 = *D_004FF120_4888;
            buf[n].v30 = *D_004FF120_4888;
            buf[n].obj = 0;
            if (GetPart_4888(node->def, i)->id != -1)
                buf[n].id = GetPart_4888(node->def, i)->id;
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

extern sV_335128 D_004FF120_335128 __asm__("t4vect_Zero");

struct sF5_335128 {
    float v[5];
};
// Segment brackets for the golden-section refinement, by nearest sample.
extern const sF5_335128 D_0048E560_335128 __asm__("D_0048E560");
extern const sF5_335128 D_0048E578_335128 __asm__("D_0048E578");

// A cubic curve object: coefficient rows at 0x10 (point = M * (t^3, t^2, t, 1)) and a bounding box.
struct sCurveObj_335128 {
    char pad00[0x10];
    sV_335128 m[4];         // 0x10
    char pad50[0x18];
    char* owner;            // 0x68 (id at +0x28)
    float bmin[3];          // 0x6C
    float bmax[3];          // 0x78
};

struct sHit_335128 {
    sV_335128 pos;      // 0x00
    sV_335128 nrm;      // 0x10
    sV_335128 v20;      // 0x20
    sV_335128 v30;      // 0x30
    float dist;         // 0x40
    int type;           // 0x44
    int pad48;
    int id;             // 0x4C
    int f50;            // 0x50
    void* obj;          // 0x54
    void* f58;          // 0x58
    int f5C;            // 0x5C
    int f60;            // 0x60
    int f64;            // 0x64
    float t;            // 0x68
    float u;            // 0x6C
    float v;            // 0x70
};

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

static inline void reset_335128(sHit_335128* h)
{
    h->type = 2;
    h->obj = 0;
    h->f50 = 0;
    h->f58 = 0;
    h->f5C = -1;
    h->f60 = -1;
    h->f64 = -1;
    h->id = -1;
    h->v20 = D_004FF120_335128;
    h->v30 = D_004FF120_335128;
    h->t = 0;
    h->u = 0;
    h->v = 0;
}

// intersect::staticSplineSegmentIntersect (On Tour name): closest point on a spline segment to intersectpos,
// a coarse 4-segment scan, then a golden-section search.
// The unit declares intersect_staticSplineSegmentIntersect with an int last parameter; bind the typed definition by asm label.
extern "C" void intersect_staticSplineSegmentIntersect_typed(sCurveObj_335128* pSplineSeg, sBox_00334680* box, sV_335128* intersectpos, bool* hit, float* intersectdist, sHit_335128* intersectinfo) __asm__("intersect_staticSplineSegmentIntersect");
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
extern "C" void intersect_staticSplineSegmentIntersect_typed(sCurveObj_335128* pSplineSeg, sBox_00334680* box, sV_335128* intersectpos, bool* hit, float* intersectdist, sHit_335128* intersectinfo)
{
    if (box->max.x < pSplineSeg->bmin[0] || box->min.x > pSplineSeg->bmax[0] ||
        box->max.y < pSplineSeg->bmin[1] || box->min.y > pSplineSeg->bmax[1] ||
        box->max.z < pSplineSeg->bmin[2] || box->min.z > pSplineSeg->bmax[2]) {
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
    lastp = splineEval_335128(pSplineSeg->m, x0);
    float cd = Length_335128(Sub_335128(*intersectpos, lastp));
    int ci = 0;
    for (i = 1; i < 5; i++) {
        nextp = splineEval_335128(pSplineSeg->m, i * 0.25f);
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
    lastp = splineEval_335128(pSplineSeg->m, x0);
    dist0 = Length_335128(Sub_335128(lastp, *intersectpos));
    lastp = splineEval_335128(pSplineSeg->m, x1);
    dist1 = Length_335128(Sub_335128(lastp, *intersectpos));
    lastp = splineEval_335128(pSplineSeg->m, x2);
    dist2 = Length_335128(Sub_335128(lastp, *intersectpos));
    lastp = splineEval_335128(pSplineSeg->m, x3);
    dist3 = Length_335128(Sub_335128(lastp, *intersectpos));
    float lastd = dist2;
    float lastt = x2;
    for (i = 0; i < 24 && !(Abs_335128(x3 - x0) <= 0.0005000000237487257f); i++) {
        if (dist2 < dist1) {
            x0 = x1;
            x1 = x2;
            x2 = x1 * kTopSplit + x3 * kBottomSplit;
            lastp = splineEval_335128(pSplineSeg->m, x2);
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
            lastp = splineEval_335128(pSplineSeg->m, x1);
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
        intersectinfo->pos = lastp;
        intersectinfo->nrm = Normalize_335128(splineSlope_335128(pSplineSeg->m, lastt));
        intersectinfo->id = *(int*)(pSplineSeg->owner + 0x28);
        intersectinfo->f50 = 0;
        intersectinfo->f5C = -1;
        intersectinfo->f58 = pSplineSeg->owner;
        intersectinfo->t = lastt;
        intersectinfo->f64 = -1;
        intersectinfo->obj = 0;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", TerrainCallback_intersectpatch);
#ifdef SKIP_ASM
struct sVec4_335960 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sVec4_335960 t4vect_Zero[];

struct sQuery_335960 {
    char pad00[0x20];
    float max[4];
    float min[4];
};

struct sTriObj_335960 {
    char pad00[8];
    short id;
    unsigned short flags;
    char pad0C[0x14C];
    float bmin[3];
    float bmax[3];
};

struct sHit_335960 {
    sVec4_335960 pos;   // 0x00
    sVec4_335960 nrm;   // 0x10
    sVec4_335960 v20;   // 0x20
    sVec4_335960 v30;   // 0x30
    float dist;         // 0x40
    int type;           // 0x44
    int pad48;
    int id;             // 0x4C
    int f50;            // 0x50
    void* obj;          // 0x54
    int f58;            // 0x58
    int f5C;            // 0x5C
    int f60;            // 0x60
    int f64;            // 0x64
    int f68;            // 0x68
    float u;            // 0x6C
    float v;            // 0x70
    int pad74[3];
};

struct sCtx_335960 {
    sQuery_335960* q;
    sHit_335960* hits;
    int cap;
    int* count;
    int arg;
    void* cache;
};

extern "C" void cAIFwdDiffCache_GetFwdDiff(void* cache, void* src, void** out);
extern "C" int cWorldIntersectType_IsPatchInt(void* q, void* obj, void* node, sVec4_335960* pos, sVec4_335960* nrm, float* dist, sVec4_335960* uv, int arg);

static inline int overlap_335960(const sQuery_335960* q, const sTriObj_335960* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline void reset_335960(sHit_335960* h)
{
    h->type = 2;
    h->obj = 0;
    h->f50 = 0;
    h->f58 = 0;
    h->f5C = -1;
    h->f60 = -1;
    h->f64 = -1;
    h->id = -1;
    h->v20 = *t4vect_Zero;
    h->v30 = *t4vect_Zero;
    h->f68 = 0;
    h->u = 0;
    h->v = 0;
}

/**
 * Intersects one patch with a terrain query and appends the hit to the query's hit list.
 *
 * Skips the patch unless both bits of 0x41 are set in its flags (+0xA) and its box (+0x158 min, +0x164 max) overlaps
 * the query's box (max +0x20, min +0x30). Gets its forward differences from the context's cache
 * (cAIFwdDiffCache_GetFwdDiff), tests it with cWorldIntersectType_IsPatchInt and, on a hit, resets record *count of the
 * hit array (On Tour tIntersectInfo::Init), stores position, normal, the patch's surface id (+0x8), the patch, the
 * parameter, and u and v, and increments the count. The count is not checked against the maximum. Called per patch by
 * the Nash terrain walk (cBXNash_terrainIntersect, nash_privy_QueryHelper_helpX5, nash_privy_QueryHelper_help, nash_Cell_descend4).
 *
 * @param vctx  the terrain query: query, hit array, maximum, count pointer, hint, AI forward-difference cache
 * @param vobj  the patch
 */
extern "C" void TerrainCallback_intersectpatch(void* vctx, void* vobj)
{
    sCtx_335960* ctx = (sCtx_335960*)vctx;
    sTriObj_335960* obj = (sTriObj_335960*)vobj;
    sVec4_335960 pos;
    sVec4_335960 nrm;
    sVec4_335960 uv;
    void* node;
    float dist;
    if ((obj->flags & 0x41) != 65) return;
    if (!overlap_335960(ctx->q, obj)) return;
    cAIFwdDiffCache_GetFwdDiff(ctx->cache, obj, &node);
    if (!cWorldIntersectType_IsPatchInt(ctx->q, obj, node, &pos, &nrm, &dist, &uv, ctx->arg)) return;
    reset_335960(&ctx->hits[*ctx->count]);
    ctx->hits[*ctx->count].pos = pos;
    ctx->hits[*ctx->count].nrm = nrm;
    ctx->hits[*ctx->count].id = obj->id;
    ctx->hits[*ctx->count].obj = obj;
    ctx->hits[*ctx->count].dist = dist;
    ctx->hits[*ctx->count].u = uv.x;
    ctx->hits[*ctx->count].v = uv.y;
    (*ctx->count)++;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", TerrainCallback_intersectinstance);
#ifdef SKIP_ASM
struct sVEnt_335B90 { short delta; short index; void* fn; };
struct sBox_335B90;
typedef int (*Test_335B90)(void*);
typedef sBox_335B90* (*GetBox_335B90)(void*);

struct sBox_335B90 {
    float min[4];
    float max[4];
};

struct sQuery_335B90 {
    char pad00[0x20];
    float max[4];
    float min[4];
};

struct sObj_335B90 {
    int pad00[3];
    sVEnt_335B90* vt;
};

struct sNode_335B90 {
    void* next;
    int pad04;
    int flags;
    sObj_335B90* obj;
    int pad10[20];
    float bmin[3];
    float bmax[3];
    int pad78[4];
    int active;
};

struct sCtx_335B90 {
    sQuery_335B90* q;
    char* buf;
    int cap;
    int* count;
};

extern "C" int intersect_instanceIntersect(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapNode_335B90(const sQuery_335B90* q, const sNode_335B90* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_335B90(const sQuery_335B90* q, const sBox_335B90* o)
{
    int r = 0;
    if (q->max[0] > o->min[0] && q->min[0] < o->max[0] &&
        q->max[1] > o->min[1] && q->min[1] < o->max[1] &&
        q->max[2] > o->min[2] && q->min[2] < o->max[2]) {
        r = 1;
    }
    return r;
}

static inline void collect_335B90(sCtx_335B90* ctx, sNode_335B90* node)
{
    int n = *ctx->count;
    *ctx->count += intersect_instanceIntersect(ctx->q, node, ctx->buf + n * 0x80, ctx->cap - n, 2);
}

/**
 * Intersects one instance with a terrain query and adds its hits to the query's hit list.
 *
 * Skips instances whose +0x88 word is 0. A flag 0x20 instance is box-tested against its stored box (+0x60 min, +0x6C
 * max); a flag 0x40 one needs a non-null object (+0xC) that answers true through virtual slot 44 and whose slot 45 box
 * overlaps. Each overlapping instance goes to intersect_instanceIntersect (mode 2) at record *count with the room left,
 * and the count grows by its result. Called per instance by the Nash terrain walk (cBXNash_terrainIntersect, nash_privy_QueryHelper_helpX5,
 * nash_privy_QueryHelper_help, nash_Cell_descend4).
 *
 * @param vctx  the terrain query: query, hit array, maximum, count pointer
 * @param vnode  the instance
 */
extern "C" void TerrainCallback_intersectinstance(void* vctx, void* vnode)
{
    sCtx_335B90* ctx = (sCtx_335B90*)vctx;
    sNode_335B90* node = (sNode_335B90*)vnode;
    if (node->active == 0) return;
    int flags = node->flags;
    if (flags & 0x20) {
        if (overlapNode_335B90(ctx->q, node)) {
            collect_335B90(ctx, node);
        }
    } else if (flags & 0x40) {
        if (node->obj == 0) return;
        sVEnt_335B90* vt = node->obj->vt;
        if (((Test_335B90)vt[44].fn)((char*)node->obj + vt[44].delta)) {
            sVEnt_335B90* vt2 = node->obj->vt;
            sBox_335B90* objBox = ((GetBox_335B90)vt2[45].fn)((char*)node->obj + vt2[45].delta);
            if (overlapBox_335B90(ctx->q, objBox)) {
                collect_335B90(ctx, node);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_terrainIntersect);
#ifdef SKIP_ASM
struct sQuad_335D78 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sQry_335D78 {
    char pad00[0x20];
    sQuad_335D78 max;   // 0x20
    sQuad_335D78 min;   // 0x30
};

struct sBox_335D78 {
    sQuad_335D78 min;
    sQuad_335D78 max;
};

struct sOct_335D78 {
    void* child[8];
    void* listB;        // 0x20
    void* listA;        // 0x24
};

struct sCell_335D78 {
    int level;
    int x, y, z;
    sOct_335D78* tree;
};

struct sGrid_335D78 {
    sCell_335D78 cells[8];
    int unkA0;
    void* cache;        // 0xA4
};

struct sTag_335D78 {
};

extern "C" void nash_Cell_descend4(void* self, void* ctx);
extern "C" void nash_privy_QueryHelper_helpX5(char* flag, sCtx_335960* ctx, sCtx_335960* ctx2, void* box, void* tree, void* cell);

static inline void collect_335D78(sOct_335D78* node, sCtx_335960* ctx)
{
    void** p;
    for (p = (void**)node->listA; p != 0; p = (void**)*p) {
        TerrainCallback_intersectpatch(ctx, p);
    }
    for (p = (void**)node->listB; p != 0; p = (void**)*p) {
        TerrainCallback_intersectinstance(ctx, p);
    }
    if (node->child[0] != 0) nash_Cell_descend4(node->child[0], ctx);
    if (node->child[1] != 0) nash_Cell_descend4(node->child[1], ctx);
    if (node->child[2] != 0) nash_Cell_descend4(node->child[2], ctx);
    if (node->child[3] != 0) nash_Cell_descend4(node->child[3], ctx);
    if (node->child[4] != 0) nash_Cell_descend4(node->child[4], ctx);
    if (node->child[5] != 0) nash_Cell_descend4(node->child[5], ctx);
    if (node->child[6] != 0) nash_Cell_descend4(node->child[6], ctx);
    if (node->child[7] != 0) nash_Cell_descend4(node->child[7], ctx);
}

static inline void queryCell_335D78(sGrid_335D78* grid, int i, sBox_335D78* box, sCtx_335960* ctx)
{
    sCell_335D78* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sOct_335D78* tree = cell->tree;
        sTag_335D78 tag;
        char* flag = (char*)&tag;
        int r = nash_CellKey_proximitytest(cell, box);
        if (r == 0) {
            collect_335D78(tree, ctx);
        } else if (r == 2) {
            nash_privy_QueryHelper_helpX5(flag, ctx, ctx, box, tree, cell);
        }
    }
}

struct sHit_00336850;

/**
 * Intersects a query with the terrain of the Nash's octree, storing the hits; returns their count.
 *
 * Builds a terrain query context (the query, @p hits, @p cap, a local count, @p hitCache and the AI forward-difference
 * cache at +0xA4) and the query's box (min +0x30, max +0x20). For each of the eight octants with a root cell,
 * nash_CellKey_proximitytest 0 (inside) feeds the cell's patch list (+0x24) to TerrainCallback_intersectpatch, its instance list (+0x20)
 * to TerrainCallback_intersectinstance and its children to nash_Cell_descend4; 2 (crossing) goes to nash_privy_QueryHelper_helpX5. Called by cBXNash_terrainIntersect1.
 *
 * @param vself  the Nash
 * @param vq  the query (On Tour cWorldIntersectType)
 * @param hits  out: the hit records, 0x80 bytes each
 * @param cap  the number of records @p hits holds
 * @param hitCache  passed to cWorldIntersectType_IsPatchInt for each patch (On Tour tIntHint *hint)
 * @return the number of hits stored
 */
extern "C" int cBXNash_terrainIntersect(void* vself, void* vq, sHit_00336850* hits, int cap, int hitCache)
{
    sGrid_335D78* grid = (sGrid_335D78*)vself;
    sQry_335D78* query = (sQry_335D78*)vq;
    sCtx_335960 ctx;
    sBox_335D78 box;
    int count;
    ctx.q = (sQuery_335960*)query;
    ctx.hits = (sHit_335960*)hits;
    ctx.cap = cap;
    ctx.count = &count;
    ctx.arg = hitCache;
    ctx.cache = grid->cache;
    box.min = query->min;
    box.max = query->max;
    sCtx_335960* pctx = &ctx;
    count = 0;
    queryCell_335D78(grid, 0, &box, pctx);
    queryCell_335D78(grid, 1, &box, pctx);
    queryCell_335D78(grid, 2, &box, pctx);
    queryCell_335D78(grid, 3, &box, pctx);
    queryCell_335D78(grid, 4, &box, pctx);
    queryCell_335D78(grid, 5, &box, pctx);
    queryCell_335D78(grid, 6, &box, pctx);
    queryCell_335D78(grid, 7, &box, pctx);
    return count;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", cBXNash_terrainIntersect1);
#ifdef SKIP_ASM
struct sHit_00336850 {
    float pad0[16];
    float dist;             // 0x40
    int pad44[3];
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[10];
    sHit_00336850() {}
} __attribute__((aligned(16)));

struct sVEnt_00336850 { short delta; short index; float (*fn)(void*); };
extern "C" int cBXNash_terrainIntersect(void* self, void* q, sHit_00336850* hits, int max, int a4);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_00336850(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int isBetter_00336850(sHit_00336850* a, sHit_00336850* b)
{
    if (a->obj != 0) {
        if (b->obj == 0) {
            return 1;
        }
        return *(unsigned int*)(a->obj + 0x78) < *(unsigned int*)(b->obj + 0x78);
    }
    if (b->obj != 0) {
        return 0;
    }
    return *(unsigned int*)(a->tri + 0x150) < *(unsigned int*)(b->tri + 0x150);
}

/**
 * Returns the parameter of the Nash terrain hit nearest the query's ideal parameter, copying that hit to @p out.
 *
 * Collects up to 64 hits with cBXNash_terrainIntersect and returns -1.0 when there are none. Otherwise picks the hit whose
 * parameter (+0x40) is closest to the query's virtual slot 3 (On Tour GetIdealParam); ties go to an instance hit over a
 * patch hit, then to the smaller +0x78 (instances) or +0x150 (patches) word (On Tour orderIntersectInfo). Called on
 * SSXApp->Game->BXNash by the camera (cCamera_avoidGround), the rider (cRider_putAt, cRider_GetNISEndPosition), the air
 * predictor and others.
 *
 * @param intersectType  the query (On Tour cWorldIntersectType)
 * @param out  out: the chosen hit (0x80 bytes)
 * @param hint  passed through to cBXNash_terrainIntersect (On Tour tIntHint *hint; cCamera_avoidGround passes its intersection hint)
 * @return the chosen hit's parameter, or -1.0
 */
extern "C" float cBXNash_terrainIntersect1(void* self, char* intersectType, sHit_00336850* out, int hint)
{
    sHit_00336850 hits[64];
    int n = cBXNash_terrainIntersect(self, intersectType, hits, 0x40, hint);
    if (n == 0) {
        return -1.0f;
    }
    int best = 0;
    sVEnt_00336850* vt = *(sVEnt_00336850**)(intersectType + 0x50);
    float ref = vt[3].fn(intersectType + vt[3].delta);
    float bestDiff = absf_00336850(hits[0].dist - ref);
    for (int i = 1; i < n; i++) {
        float paramDiff = absf_00336850(hits[i].dist - ref);
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
    return out->dist;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_getGroundHeight);
#ifdef SKIP_ASM
struct sQuad_369D8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sHit_003369D8 {
    sQuad_369D8 p;          // 0x00
    sQuad_369D8 n;          // 0x10
    float pad20[8];         // 0x20
    float dist;             // 0x40
    int pad44[3];
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[10];
    sHit_003369D8() {}
} __attribute__((aligned(16)));

struct sQuery_369D8 {
    void* seg;              // 0x00
    sHit_003369D8* hits;    // 0x04
    int max;                // 0x08
    int* count;             // 0x0C
    int f10;                // 0x10
    void* f14;              // 0x14
};

struct sCell_369D8 {
    int f0, f4, f8, fC;
    void* node;             // 0x10
};

extern "C" void* cWorldLine_cWorldLine(void* seg, const sQuad_369D8& a, const sQuad_369D8& b, int n, float r);
struct sCtx_369D8 {
    float* pos;             // 0x0
    char flags[8];          // 0x4
};

struct sTrav_369D8 {
    int count;              // 0x0
    sCtx_369D8 ctx;         // 0x4
};

extern "C" void nash_privy_QueryHelper_help(char* flag, sQuery_369D8* q, sQuery_369D8* q2, sCtx_369D8* ctx, void* node, sCell_369D8* cell);

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

static inline int isBetter_003369D8(sHit_003369D8* a, sHit_003369D8* b)
{
    if (a->obj != 0) {
        if (b->obj == 0) {
            return 1;
        }
        return *(unsigned int*)(a->obj + 0x78) < *(unsigned int*)(b->obj + 0x78);
    }
    if (b->obj != 0) {
        return 0;
    }
    return *(unsigned int*)(a->tri + 0x150) < *(unsigned int*)(b->tri + 0x150);
}

static inline void visit_003369D8(sCell_369D8* c, char* flag, sQuery_369D8* q, sCtx_369D8* p)
{
    nash_privy_QueryHelper_help(flag, q, q, p, c->node, c);
}

/**
 * Finds the terrain straight above or below @p pos; on a hit stores its position and normal and returns 1.
 *
 * Builds a vertical cWorldLine from 100000 below to 100000 above @p pos (precision 2, ideal parameter at 20 units above
 * @p pos, clamped to 0..1) and a terrain query on it (64 hit records, no hint, the AI forward-difference cache at
 * +0xA4), then walks each of the eight octants that has a root cell with nash_privy_QueryHelper_help. Returns 0 without hits;
 * otherwise picks the hit whose height (z) is closest to @p pos's, ties going to an instance hit and then to the
 * smaller +0x78 / +0x150 word, and copies that hit's position and normal out. Called by cSSXScriptEngine_GetAnchorOrientation.
 *
 * @param pos  the point (x, y, z)
 * @param outP  out: the hit position
 * @param outN  out: the hit normal
 * @return 1 on a hit, otherwise 0
 */
extern "C" int cBXNash_getGroundHeight(sCell_369D8* self, float* pos, sQuad_369D8* outP, sQuad_369D8* outN)
{
    char seg[176];
    float lo = pos[2] - 100000.0f;
    float hi = pos[2] + 100000.0f;
    float idealParam = clamp_003369D8((pos[2] + 20.0f - lo) / (hi - lo), 0.0f, 1.0f);
    {
        sQuad_369D8 lineStart;
        lineStart.x = pos[0];
        lineStart.y = pos[1];
        lineStart.z = lo;
        lineStart.w = 1.0f;
        sQuad_369D8 lineEnd;
        lineEnd.x = pos[0];
        lineEnd.y = pos[1];
        lineEnd.z = hi;
        lineEnd.w = 1.0f;
        cWorldLine_cWorldLine(seg, lineStart, lineEnd, 2, idealParam);
    }
    sHit_003369D8 hits[64];
    sQuery_369D8 terrainQuery;
    terrainQuery.seg = seg;
    terrainQuery.hits = hits;
    terrainQuery.max = 0x40;
    sTrav_369D8 tr;
    terrainQuery.count = &tr.count;
    terrainQuery.f10 = 0;
    terrainQuery.f14 = *(void**)((char*)self + 0xA4);
    tr.count = 0;
    tr.ctx.pos = pos;
    sCtx_369D8* ctx = &tr.ctx;
    sCell_369D8* octant0 = &self[0];
    sCell_369D8* octant1 = &self[1];
    sCell_369D8* octant2 = &self[2];
    sCell_369D8* octant3 = &self[3];
    sCell_369D8* octant4 = &self[4];
    sCell_369D8* octant5 = &self[5];
    sCell_369D8* octant6 = &self[6];
    sCell_369D8* octant7 = &self[7];
    if (self[0].node) visit_003369D8(octant0, &tr.ctx.flags[0], &terrainQuery, ctx);
    if (self[1].node) visit_003369D8(octant1, &tr.ctx.flags[1], &terrainQuery, ctx);
    if (self[2].node) visit_003369D8(octant2, &tr.ctx.flags[2], &terrainQuery, ctx);
    if (self[3].node) visit_003369D8(octant3, &tr.ctx.flags[3], &terrainQuery, ctx);
    if (self[4].node) visit_003369D8(octant4, &tr.ctx.flags[4], &terrainQuery, ctx);
    if (self[5].node) visit_003369D8(octant5, &tr.ctx.flags[5], &terrainQuery, ctx);
    if (self[6].node) visit_003369D8(octant6, &tr.ctx.flags[6], &terrainQuery, ctx);
    if (self[7].node) visit_003369D8(octant7, &tr.ctx.flags[7], &terrainQuery, ctx);
    if (tr.count == 0) {
        return 0;
    }
    float ref = pos[2];
    int best = 0;
    float bestDiff = absf_003369D8(hits[0].p.z - ref);
    for (int i = 1; i < tr.count; i++) {
        float heightDiff = absf_003369D8((hits + i)->p.z - ref);
        if (heightDiff < bestDiff) {
            bestDiff = heightDiff;
            best = i;
        } else if (heightDiff == bestDiff) {
            if (isBetter_003369D8(hits + i, hits + best)) {
                best = i;
            }
        }
    }
    *outP = (hits + best)->p;
    *outN = (hits + best)->n;
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", StaticObjectCallback_intersectinstance);
#ifdef SKIP_ASM
struct sVEnt_336D40 { short delta; short index; void* fn; };
struct sBox_336D40;
typedef int (*Test_336D40)(void*);
typedef sBox_336D40* (*GetBox_336D40)(void*);

struct sBox_336D40 {
    float min[4];
    float max[4];
};

struct sQuery_336D40 {
    char pad00[0x20];
    float max[4];
    float min[4];
};

struct sObj_336D40 {
    int pad00[3];
    sVEnt_336D40* vt;
};

struct sNode_336D40 {
    void* next;
    int pad04;
    int flags;
    sObj_336D40* obj;
    int pad10[20];
    float bmin[3];
    float bmax[3];
    int pad78[4];
    int active;
};

struct sCtx_336D40 {
    sQuery_336D40* q;
    char* buf;
    int cap;
    int* count;
    int full;
};

extern "C" int intersect_instanceIntersect(void* q, void* node, void* buf, int cap, int mode);

static inline int overlapNode_336D40(const sQuery_336D40* q, const sNode_336D40* n)
{
    int r = 0;
    if (q->max[0] > n->bmin[0] && q->min[0] < n->bmax[0] &&
        q->max[1] > n->bmin[1] && q->min[1] < n->bmax[1] &&
        q->max[2] > n->bmin[2] && q->min[2] < n->bmax[2]) {
        r = 1;
    }
    return r;
}

static inline int overlapBox_336D40(const sQuery_336D40* q, const sBox_336D40* o)
{
    int r = 0;
    if (q->max[0] > o->min[0] && q->min[0] < o->max[0] &&
        q->max[1] > o->min[1] && q->min[1] < o->max[1] &&
        q->max[2] > o->min[2] && q->min[2] < o->max[2]) {
        r = 1;
    }
    return r;
}

static inline void collect_336D40(sCtx_336D40* ctx, sNode_336D40* node, int mode)
{
    int n = *ctx->count;
    *ctx->count += intersect_instanceIntersect(ctx->q, node, ctx->buf + n * 0x80, ctx->cap - n, mode);
}

/**
 * Intersects one instance with a static-object query and adds its hits to the query's hit list.
 *
 * A flag 0x20 instance is box-tested against its stored box (+0x60 min, +0x6C max); a flag 0x40 one asks its object
 * (+0xC) through virtual slots 44 (takes part) and 45 (box). Each overlapping instance goes to
 * intersect_instanceIntersect at record *count with the room left, in mode 3 when the context's flag (+0x10) is set and
 * 1 otherwise, and the count grows by its result. Unlike TerrainCallback_intersectinstance it checks neither the +0x88 word nor a null
 * object. Called per instance by the Nash static-object walk (cBXNash_staticObjectIntersect, nash_privy_QueryHelper_helpX6, nash_Cell_descend5).
 *
 * @param vctx  the static-object query: query, hit array, maximum, count pointer, include flag
 * @param vnode  the instance
 */
extern "C" void StaticObjectCallback_intersectinstance(void* vctx, void* vnode)
{
    sCtx_336D40* ctx = (sCtx_336D40*)vctx;
    sNode_336D40* node = (sNode_336D40*)vnode;
    int mode = ctx->full ? 3 : 1;
    int flags = node->flags;
    if (flags & 0x20) {
        if (overlapNode_336D40(ctx->q, node)) {
            collect_336D40(ctx, node, mode);
        }
    } else if (flags & 0x40) {
        sVEnt_336D40* vt = node->obj->vt;
        if (((Test_336D40)vt[44].fn)((char*)node->obj + vt[44].delta)) {
            sVEnt_336D40* vt2 = node->obj->vt;
            sBox_336D40* objBox = ((GetBox_336D40)vt2[45].fn)((char*)node->obj + vt2[45].delta);
            if (overlapBox_336D40(ctx->q, objBox)) {
                collect_336D40(ctx, node, mode);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_staticObjectIntersect);
#ifdef SKIP_ASM
struct sQuad_336F30 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sQry_336F30 {
    char pad00[0x20];
    sQuad_336F30 max;   // 0x20
    sQuad_336F30 min;   // 0x30
};

struct sBox_336F30 {
    sQuad_336F30 min;
    sQuad_336F30 max;
};

struct sOct_336F30 {
    void* child[8];
    void* list;         // 0x20
};

struct sCell_336F30 {
    int level;
    int x, y, z;
    sOct_336F30* tree;
};

struct sGrid_336F30 {
    sCell_336F30 cells[8];
};

struct sTag_336F30 {
};

extern "C" void nash_Cell_descend5(void* self, void* ctx);
extern "C" void nash_privy_QueryHelper_helpX6(char* flag, sCtx_336D40* ctx, sCtx_336D40* ctx2, void* box, void* tree, void* cell);

static inline void collect_336F30(sOct_336F30* node, sCtx_336D40* ctx)
{
    void** p;
    for (p = (void**)node->list; p != 0; p = (void**)*p) {
        StaticObjectCallback_intersectinstance(ctx, p);
    }
    if (node->child[0] != 0) nash_Cell_descend5(node->child[0], ctx);
    if (node->child[1] != 0) nash_Cell_descend5(node->child[1], ctx);
    if (node->child[2] != 0) nash_Cell_descend5(node->child[2], ctx);
    if (node->child[3] != 0) nash_Cell_descend5(node->child[3], ctx);
    if (node->child[4] != 0) nash_Cell_descend5(node->child[4], ctx);
    if (node->child[5] != 0) nash_Cell_descend5(node->child[5], ctx);
    if (node->child[6] != 0) nash_Cell_descend5(node->child[6], ctx);
    if (node->child[7] != 0) nash_Cell_descend5(node->child[7], ctx);
}

static inline void queryCell_336F30(sGrid_336F30* grid, int i, sBox_336F30* box, sCtx_336D40* ctx)
{
    sCell_336F30* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sOct_336F30* tree = cell->tree;
        sTag_336F30 tag;
        char* flag = (char*)&tag;
        int r = nash_CellKey_proximitytest(cell, box);
        if (r == 0) {
            collect_336F30(tree, ctx);
        } else if (r == 2) {
            nash_privy_QueryHelper_helpX6(flag, ctx, ctx, box, tree, cell);
        }
    }
}

struct sHit_003378C0;

/**
 * Intersects a query with the instances of the Nash's octree, storing the hits; returns their count.
 *
 * Builds a static-object query context (the query, @p hits, @p cap, a local count, @p full) and the query's box (min
 * +0x30, max +0x20). For each of the eight octants with a root cell, nash_CellKey_proximitytest 0 (inside) feeds the
 * cell's instance list (+0x20) to StaticObjectCallback_intersectinstance and its children to nash_Cell_descend5; 2 (crossing) goes to nash_privy_QueryHelper_helpX6.
 * Called by cBXNash_staticObjectIntersect1.
 *
 * @param vself  the Nash
 * @param vq  the query (On Tour cWorldIntersectType)
 * @param hits  out: the hit records, 0x80 bytes each
 * @param cap  the number of records @p hits holds
 * @param full  nonzero makes StaticObjectCallback_intersectinstance pass mode 3 instead of 1 (On Tour includeRidable)
 * @return the number of hits stored
 */
extern "C" int cBXNash_staticObjectIntersect(void* vself, void* vq, sHit_003378C0* hits, int cap, int full)
{
    sGrid_336F30* grid = (sGrid_336F30*)vself;
    sQry_336F30* query = (sQry_336F30*)vq;
    sCtx_336D40 ctx;
    sBox_336F30 box;
    int count;
    ctx.q = (sQuery_336D40*)query;
    ctx.buf = (char*)hits;
    ctx.cap = cap;
    ctx.count = &count;
    ctx.full = full;
    box.min = query->min;
    box.max = query->max;
    sCtx_336D40* pctx = &ctx;
    count = 0;
    queryCell_336F30(grid, 0, &box, pctx);
    queryCell_336F30(grid, 1, &box, pctx);
    queryCell_336F30(grid, 2, &box, pctx);
    queryCell_336F30(grid, 3, &box, pctx);
    queryCell_336F30(grid, 4, &box, pctx);
    queryCell_336F30(grid, 5, &box, pctx);
    queryCell_336F30(grid, 6, &box, pctx);
    queryCell_336F30(grid, 7, &box, pctx);
    return count;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/worldsphtree", cBXNash_staticObjectIntersect1);
#ifdef SKIP_ASM
struct sHit_003378C0 {
    float pad0[16];
    float dist;             // 0x40
    int pad44[3];
    char* obj;              // 0x50
    char* tri;              // 0x54
    int pad58[10];
    sHit_003378C0() {}
} __attribute__((aligned(16)));

struct sVEnt_003378C0 { short delta; short index; float (*fn)(void*); };
extern "C" int cBXNash_staticObjectIntersect(void* self, void* q, sHit_003378C0* hits, int max, int a4);

// PORT: PS2 FPU abs.s via inline asm; use fabsf off-PS2.
static inline float absf_003378C0(float x)
{
    float r;
    __asm__("abs.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline int isBetter_003378C0(sHit_003378C0* a, sHit_003378C0* b)
{
    if (a->obj != 0) {
        if (b->obj == 0) {
            return 1;
        }
        return *(unsigned int*)(a->obj + 0x78) < *(unsigned int*)(b->obj + 0x78);
    }
    if (b->obj != 0) {
        return 0;
    }
    return *(unsigned int*)(a->tri + 0x150) < *(unsigned int*)(b->tri + 0x150);
}

/**
 * Returns the parameter of the Nash static-object hit nearest the query's ideal parameter, copying that hit to @p out.
 *
 * Collects up to 128 hits with cBXNash_staticObjectIntersect and returns -1.0 when there are none. Otherwise picks the hit whose
 * parameter (+0x40) is closest to the query's virtual slot 3 (On Tour GetIdealParam); ties go to an instance hit over a
 * patch hit, then to the smaller +0x78 (instances) or +0x150 (patches) word. Called by tAvalancheNode_calculate and
 * cAirPredictor_simStep.
 *
 * @param query  the query (On Tour cWorldIntersectType)
 * @param out  out: the chosen hit (0x80 bytes)
 * @param full  passed through to cBXNash_staticObjectIntersect (On Tour includeRidable)
 * @return the chosen hit's parameter, or -1.0
 */
extern "C" float cBXNash_staticObjectIntersect1(void* self, char* query, sHit_003378C0* out, int full)
{
    sHit_003378C0 hits[128];
    int n = cBXNash_staticObjectIntersect(self, query, hits, 0x80, full);
    if (n == 0) {
        return -1.0f;
    }
    int best = 0;
    sVEnt_003378C0* vt = *(sVEnt_003378C0**)(query + 0x50);
    float ref = vt[3].fn(query + vt[3].delta);
    float bestDiff = absf_003378C0(hits[0].dist - ref);
    for (int i = 1; i < n; i++) {
        float diff = absf_003378C0(hits[i].dist - ref);
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
    return out->dist;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBXNash_splineIntersect);
#ifdef SKIP_ASM
struct sTree_3411B8;
struct sQuery_3411B8;
extern "C" void nash_Cell_descend6(sTree_3411B8* tree, sQuery_3411B8* q);

struct sQry_337A48 {
    sWsVec4* pos;               // 0x00
    int a7;                     // 0x04
    int mask;                   // 0x08
    int* cnt;                   // 0x0C
    float* dist;                // 0x10
    sBox_00334680* box;         // 0x14
};

struct sOct_337A48 {
    sOct_337A48* child[8];
    int pad20[2];
    char* objs;                 // 0x28
};

struct sCell_337A48 {
    int level;
    int x, y, z;
    sOct_337A48* tree;
};

struct sGrid_337A48 {
    sCell_337A48 cells[8];
};

struct sTag_337A48 {
};

extern "C" void nash_privy_QueryHelper_helpX7(char* flag, sQry_337A48* q, sQry_337A48* q2, sBox_00334680* box, void* tree, void* cell);

static inline void collect_337A48(sOct_337A48* tree, sQry_337A48* q)
{
    for (char* o = tree->objs; o != 0; o = *(char**)o) {
        int type = *(int*)(o + 8);
        if (type == 2) {
            cRailModifier_splineIntersect(*(void**)(o + 0xC), q->box, q->box, q->pos, q->mask, q->cnt, q->dist, q->a7);
        } else if (type == 3) {
            cFlexRailNode_splineIntersect(*(void**)(o + 0xC), q->box, q->box, q->pos, q->mask, q->cnt, q->dist, q->a7);
        } else if (type == 1) {
            if (*(int*)(*(char**)(o + 0x68) + 0x1C) & q->mask) {
                intersect_staticSplineSegmentIntersect(o, q->box, q->pos, q->cnt, q->dist, q->a7);
            }
        }
    }
    if (tree->child[0] != 0) nash_Cell_descend6((sTree_3411B8*)tree->child[0], (sQuery_3411B8*)q);
    if (tree->child[1] != 0) nash_Cell_descend6((sTree_3411B8*)tree->child[1], (sQuery_3411B8*)q);
    if (tree->child[2] != 0) nash_Cell_descend6((sTree_3411B8*)tree->child[2], (sQuery_3411B8*)q);
    if (tree->child[3] != 0) nash_Cell_descend6((sTree_3411B8*)tree->child[3], (sQuery_3411B8*)q);
    if (tree->child[4] != 0) nash_Cell_descend6((sTree_3411B8*)tree->child[4], (sQuery_3411B8*)q);
    if (tree->child[5] != 0) nash_Cell_descend6((sTree_3411B8*)tree->child[5], (sQuery_3411B8*)q);
    if (tree->child[6] != 0) nash_Cell_descend6((sTree_3411B8*)tree->child[6], (sQuery_3411B8*)q);
    if (tree->child[7] != 0) nash_Cell_descend6((sTree_3411B8*)tree->child[7], (sQuery_3411B8*)q);
}

static inline void queryCell_337A48(sGrid_337A48* grid, int i, sBox_00334680* box, sQry_337A48* q)
{
    sCell_337A48* cell = &grid->cells[i];
    if (grid->cells[i].tree != 0) {
        sOct_337A48* tree = cell->tree;
        sTag_337A48 tag;
        char* flag = (char*)&tag;
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
 * typed list (+0x28) and its children (nash_Cell_descend6); 2 (crossing) goes to nash_privy_QueryHelper_helpX7. Typed nodes of type 2 are
 * tested with cRailModifier_splineIntersect, type 3 with cFlexRailNode_splineIntersect, and type 1 (a spline segment whose owner's
 * +0x1C flags match @p mask) with intersect_staticSplineSegmentIntersect. Nothing in the code calls it; cOverlapList_splineIntersect does the
 * same over an overlap list.
 *
 * @param grid  the Nash
 * @param pos  the point to test
 * @param intInfo  out: the nearest hit (On Tour intInfo)
 * @param mask  which spline types to accept (On Tour splineFilterMask)
 * @param testRadius  the test radius
 * @return nonzero when a hit was recorded
 */
extern "C" int cBXNash_splineIntersect(sGrid_337A48* grid, sWsVec4* pos, int intInfo, int mask, float testRadius)
{
    sBox_00334680 box;
    sWsVec4 ext;
    sQry_337A48 splineQuery;
    int cnt;
    float dist;
    box.min = *pos;
    box.max = *pos;
    ext.x = testRadius;
    ext.y = testRadius;
    ext.z = testRadius;
    ext.w = 0.0f;
    wsSubEq_334680(box.min, ext);
    wsAddEq_334680(box.max, ext);
    sBox_00334680* pbox = &box;
    splineQuery.pos = pos;
    splineQuery.a7 = intInfo;
    splineQuery.mask = mask;
    splineQuery.cnt = &cnt;
    splineQuery.dist = &dist;
    splineQuery.box = pbox;
    dist = 0.0f;
    cnt = 0;
    queryCell_337A48(grid, 0, pbox, &splineQuery);
    queryCell_337A48(grid, 1, pbox, &splineQuery);
    queryCell_337A48(grid, 2, pbox, &splineQuery);
    queryCell_337A48(grid, 3, pbox, &splineQuery);
    queryCell_337A48(grid, 4, pbox, &splineQuery);
    queryCell_337A48(grid, 5, pbox, &splineQuery);
    queryCell_337A48(grid, 6, pbox, &splineQuery);
    queryCell_337A48(grid, 7, pbox, &splineQuery);
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
struct sCell_338920 {
    int level;
    int x, y, z;
};

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
extern "C" void nash_privy_QueryHelper_helpX1(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* vbox, void* vnode, void* vcell)
{
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sOctNode_330BA8* node = (sOctNode_330BA8*)vnode;
    sCell_338920* cell = (sCell_338920*)vcell;
    OverlapInstancesCallback_operatorCall(ctx, node);
    if (cell->level == 11) {
        return;
    }
    sCell_338920 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float cellSize = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sSphNode_340A40* subNode;
    subNode = node->child[0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_330BA8((sOctNode_330BA8*)subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->child[1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_330BA8((sOctNode_330BA8*)subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y++;
    subNode = node->child[3];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_330BA8((sOctNode_330BA8*)subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->child[2];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_330BA8((sOctNode_330BA8*)subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.x++;
    subNode = node->child[6];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_330BA8((sOctNode_330BA8*)subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->child[7];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_330BA8((sOctNode_330BA8*)subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y--;
    subNode = node->child[5];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_330BA8((sOctNode_330BA8*)subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->child[4];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_330BA8((sOctNode_330BA8*)subNode, ctx2);
        else nash_privy_QueryHelper_helpX1(flag, ctx, ctx2, box, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX2);
#ifdef SKIP_ASM
struct sCell_339598 {
    int level;
    int x, y, z;
};

/**
 * Walks a crossing octree cell for the overlap-list typed query: collects its typed nodes, then visits its children.
 *
 * Appends every node of the cell's typed list (@p vnode +0x28) to @p ctx's list (count pointer +0x4), with no box test
 * and no maximum check, and stops when the cell's level is 11. Otherwise splits the cell (level, x, y, z at @p vcell;
 * size 2^level) into its eight children one level down, each widened by 0.2 child cell on every side. Each existing
 * child that overlaps @p vbox on all three axes is collected whole with @p ctx2 (its typed list, then nash_Cell_descend1 for
 * its children) when the box contains it, and recursed into otherwise, in the key order 000, 001, 011, 010, 110, 111,
 * 101, 100. Called by cOverlapList_gettyped for crossing root cells.
 *
 * @param flag  the empty helper object, only passed on to the recursion (On Tour In *this)
 * @param ctx  the collector for crossing cells (On Tour oncross)
 * @param ctx2  the collector for contained cells (On Tour oninside)
 * @param vbox  the query box: min at +0x0, max at +0x10
 * @param vnode  the cell (On Tour const Cell *)
 * @param vcell  the cell's key: level, x, y, z (On Tour CellKey)
 */
extern "C" void nash_privy_QueryHelper_helpX2(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* vbox, void* vnode, void* vcell)
{
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sOctNode_331450* node = (sOctNode_331450*)vnode;
    sCell_339598* cell = (sCell_339598*)vcell;
    void** objNode;
    for (objNode = node->objs; objNode != 0; objNode = (void**)*objNode) {
        ctx->list[(*ctx->count)++] = (sNode_3309D8*)objNode;
    }
    if (cell->level == 11) {
        return;
    }
    sCell_339598 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float cellSize = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sOctNode_331450* subNode;
    subNode = (sOctNode_331450*)node->child[0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = (sOctNode_331450*)node->child[1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y++;
    subNode = (sOctNode_331450*)node->child[3];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = (sOctNode_331450*)node->child[2];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.x++;
    subNode = (sOctNode_331450*)node->child[6];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = (sOctNode_331450*)node->child[7];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y--;
    subNode = (sOctNode_331450*)node->child[5];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_331450(subNode, ctx2);
        else nash_privy_QueryHelper_helpX2(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = (sOctNode_331450*)node->child[4];
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
extern "C" void nash_Cell_descend2(sTree_340C20* tree, sCtx_3309D8* ctx);

struct sObj_33A3D8 {
    sObj_33A3D8* next;
    char pad04[0x154];
    float bmin[3];              // 0x158
    float bmax[3];              // 0x164
};

struct sTree_33A3D8 {
    sTree_33A3D8* child[8];
    int pad20;
    sObj_33A3D8* objs;          // 0x24
};

static inline int overlapObj_33A3D8(const sBox_3309D8* b, const sObj_33A3D8* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

static inline void collectObjs_33A3D8(sTree_33A3D8* tree, sCtx_3309D8* ctx)
{
    OverlapInstancesCallback_operatorCall(ctx, tree);
    sCtx_3309D8* q = (sCtx_3309D8*)((char*)ctx + 0x10);
    for (sObj_33A3D8* p = tree->objs; p != 0; p = p->next) {
        if (overlapObj_33A3D8(q->box, p)) {
            int* cnt = q->count;
            int n = *cnt;
            q->list[n] = (sNode_3309D8*)p;
            *cnt = n + 1;
        }
    }
}

static inline void collect_33A3D8(sTree_33A3D8* tree, sCtx_3309D8* ctx)
{
    collectObjs_33A3D8(tree, ctx);
    if (tree->child[0] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[0], ctx);
    if (tree->child[1] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[1], ctx);
    if (tree->child[2] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[2], ctx);
    if (tree->child[3] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[3], ctx);
    if (tree->child[4] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[4], ctx);
    if (tree->child[5] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[5], ctx);
    if (tree->child[6] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[6], ctx);
    if (tree->child[7] != 0) nash_Cell_descend2((sTree_340C20*)tree->child[7], ctx);
}

struct sCell_33A3D8 {
    int level;
    int x, y, z;
};

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
extern "C" void nash_privy_QueryHelper_helpX3(char* flag, sCtx_3309D8* ctx, sCtx_3309D8* ctx2, void* vbox, void* vnode, void* vcell)
{
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sTree_33A3D8* node = (sTree_33A3D8*)vnode;
    sCell_33A3D8* cell = (sCell_33A3D8*)vcell;
    collectObjs_33A3D8(node, ctx);
    if (cell->level == 11) {
        return;
    }
    sCell_33A3D8 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float cellSize = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sTree_33A3D8* subNode;
    subNode = node->child[0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->child[1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y++;
    subNode = node->child[3];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->child[2];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.x++;
    subNode = node->child[6];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->child[7];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y--;
    subNode = node->child[5];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33A3D8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX3(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->child[4];
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

struct sCtx_33B748 {
    sSub_33B748 nodes;
    sSub_33B748 objs;
    sSub_33B748 all;
};

struct sObjNode_33B748 {
    sObjNode_33B748* next;
    int pad04[85];
    float bmin[3];
    float bmax[3];
};

struct sTree_33B748 {
    sTree_33B748* child[8];
    void* nodes;
    sObjNode_33B748* objs;
    void** all;
};

struct sCell_33B748 {
    int level;
    int x, y, z;
};

// PORT: alias; the unit defines nash_Cell_descend3 later with its own struct types.
extern "C" void nash_Cell_descend3_33B748(sTree_33B748* self, sCtx_33B748* ctx) __asm__("nash_Cell_descend3");

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

static inline void visit_33B748(sTree_33B748* self, sCtx_33B748* ctx)
{
    OverlapInstancesCallback_operatorCall((sCtx_3309D8*)ctx, self);
    sSub_33B748* s = &ctx->objs;
    for (sObjNode_33B748* p = self->objs; p != 0; p = p->next) {
        if (overlap_33B748(s->box, p)) {
            add_33B748(s, p);
        }
    }
    s = &ctx->all;
    for (void** q = self->all; q != 0; q = (void**)*q) {
        add_33B748(s, q);
    }
}

static inline void collect_33B748(sTree_33B748* self, sCtx_33B748* ctx)
{
    visit_33B748(self, ctx);
    if (self->child[0]) nash_Cell_descend3_33B748(self->child[0], ctx);
    if (self->child[1]) nash_Cell_descend3_33B748(self->child[1], ctx);
    if (self->child[2]) nash_Cell_descend3_33B748(self->child[2], ctx);
    if (self->child[3]) nash_Cell_descend3_33B748(self->child[3], ctx);
    if (self->child[4]) nash_Cell_descend3_33B748(self->child[4], ctx);
    if (self->child[5]) nash_Cell_descend3_33B748(self->child[5], ctx);
    if (self->child[6]) nash_Cell_descend3_33B748(self->child[6], ctx);
    if (self->child[7]) nash_Cell_descend3_33B748(self->child[7], ctx);
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
extern "C" void nash_privy_QueryHelper_helpX4(char* flag, sCtx_340DC0* vctx, sCtx_340DC0* vctx2, void* vbox, void* vnode, void* vcell)
{
    sCtx_33B748* ctx = (sCtx_33B748*)vctx;
    sCtx_33B748* ctx2 = (sCtx_33B748*)vctx2;
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sTree_33B748* node = (sTree_33B748*)vnode;
    sCell_33B748* cell = (sCell_33B748*)vcell;
    visit_33B748(node, ctx);
    if (cell->level == 11) {
        return;
    }
    sCell_33B748 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float cellSize = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sTree_33B748* subNode;
    subNode = node->child[0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z++;
    subNode = node->child[1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.y++;
    subNode = node->child[3];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z--;
    subNode = node->child[2];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.x++;
    subNode = node->child[6];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z++;
    subNode = node->child[7];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.y--;
    subNode = node->child[5];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z--;
    subNode = node->child[4];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_33B748(subNode, ctx2);
        else nash_privy_QueryHelper_helpX4(flag, vctx, vctx2, vbox, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX5);
#ifdef SKIP_ASM
struct sCell_33CCF8 {
    int level;
    int x, y, z;
};

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
extern "C" void nash_privy_QueryHelper_helpX5(char* flag, sCtx_335960* ctx, sCtx_335960* ctx2, void* vbox, void* vnode, void* vcell)
{
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sOct_335D78* node = (sOct_335D78*)vnode;
    sCell_33CCF8* cell = (sCell_33CCF8*)vcell;
    void** objNode;
    for (objNode = (void**)node->listA; objNode != 0; objNode = (void**)*objNode) {
        TerrainCallback_intersectpatch(ctx, objNode);
    }
    for (objNode = (void**)node->listB; objNode != 0; objNode = (void**)*objNode) {
        TerrainCallback_intersectinstance(ctx, objNode);
    }
    if (cell->level == 11) {
        return;
    }
    sCell_33CCF8 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float cellSize = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sOct_335D78* subNode;
    subNode = (sOct_335D78*)node->child[0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = (sOct_335D78*)node->child[1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y++;
    subNode = (sOct_335D78*)node->child[3];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = (sOct_335D78*)node->child[2];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.x++;
    subNode = (sOct_335D78*)node->child[6];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = (sOct_335D78*)node->child[7];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y--;
    subNode = (sOct_335D78*)node->child[5];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = (sOct_335D78*)node->child[4];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_335D78(subNode, ctx2);
        else nash_privy_QueryHelper_helpX5(flag, ctx, ctx2, box, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_help);
#ifdef SKIP_ASM
struct sQuery_369D8;
struct sCtx_369D8;
struct sCell_369D8;
extern "C" void TerrainCallback_intersectpatch(void* ctx, void* obj);
extern "C" void TerrainCallback_intersectinstance(void* ctx, void* obj);
extern "C" void nash_Cell_descend4(void* self, void* ctx);

struct sOct_DBE8 {
    sOct_DBE8* child[8];            // 0x00
    void** listB;                   // 0x20
    void** listA;                   // 0x24
};

struct sCell_DBE8 {
    int level;                      // 0x0
    int x;                          // 0x4
    int y;                          // 0x8
    int z;                          // 0xC
};

struct sCtx_DBE8 {
    float* pos;                     // 0x0
};

static inline int Classify_DBE8(sCtx_DBE8* ctx, sCell_DBE8* c)
{
    int bits = (c->level + 0x7F) << 23;
    float s = *(float*)&bits;
    float* p = ctx->pos;
    if (p[0] < ((float)c->x - 0.2f) * s) {
        return 1;
    }
    if (((float)(c->x + 1) + 0.2f) * s < p[0]) {
        return 1;
    }
    if (p[1] < ((float)c->y - 0.2f) * s) {
        return 1;
    }
    if (((float)(c->y + 1) + 0.2f) * s < p[1]) {
        return 1;
    }
    return 2;
}

/**
 * Walks an octree cell for the vertical-line terrain query: tests the cell against the line, then its children.
 *
 * Classifies the cell (level, x, y, z at @p vcell; size 2^level, widened by 0.2 cell) against the vertical line through
 * the point @p vctx holds: outside when the point's x or y lies outside the cell's span, crossing otherwise. The test
 * never answers inside (0), so the branch that walks the cell whole with @p insideQuery (TerrainCallback_intersectpatch, TerrainCallback_intersectinstance,
 * nash_Cell_descend4) is dead. A crossing cell feeds its patch list (+0x24) to TerrainCallback_intersectpatch and its instance list (+0x20)
 * to TerrainCallback_intersectinstance with @p query, stops when the level is 11, and otherwise recurses into each existing child with its
 * key, in the order 000, 100, 110, 010, 011, 111, 101, 001 (x, y, z). Called by cBXNash_getGroundHeight for each root cell.
 *
 * @param flag  the empty helper object, only passed on to the recursion (On Tour In *this)
 * @param query  the terrain query for crossing cells (On Tour oncross)
 * @param insideQuery  the terrain query for contained cells (On Tour oninside)
 * @param vctx  the line test: a pointer to the point (On Tour VerticalLineProximityTest)
 * @param vnode  the cell (On Tour const Cell *)
 * @param vcell  the cell's key: level, x, y, z (On Tour CellKey)
 */
extern "C" void nash_privy_QueryHelper_help(char* flag, sQuery_369D8* query, sQuery_369D8* insideQuery, sCtx_369D8* vctx, void* vnode, sCell_369D8* vcell)
{
    sCtx_DBE8* ctx = (sCtx_DBE8*)vctx;
    sOct_DBE8* node = (sOct_DBE8*)vnode;
    sCell_DBE8* cell = (sCell_DBE8*)vcell;
    int overlap = Classify_DBE8(ctx, cell);
    if (overlap == 0) {
        void** objNode;
        for (objNode = node->listA; objNode != 0; objNode = (void**)*objNode) {
            TerrainCallback_intersectpatch(insideQuery, objNode);
        }
        for (objNode = node->listB; objNode != 0; objNode = (void**)*objNode) {
            TerrainCallback_intersectinstance(insideQuery, objNode);
        }
        if (node->child[0]) {
            nash_Cell_descend4(node->child[0], insideQuery);
        }
        if (node->child[1]) {
            nash_Cell_descend4(node->child[1], insideQuery);
        }
        if (node->child[2]) {
            nash_Cell_descend4(node->child[2], insideQuery);
        }
        if (node->child[3]) {
            nash_Cell_descend4(node->child[3], insideQuery);
        }
        if (node->child[4]) {
            nash_Cell_descend4(node->child[4], insideQuery);
        }
        if (node->child[5]) {
            nash_Cell_descend4(node->child[5], insideQuery);
        }
        if (node->child[6]) {
            nash_Cell_descend4(node->child[6], insideQuery);
        }
        if (node->child[7]) {
            nash_Cell_descend4(node->child[7], insideQuery);
        }
    } else if (overlap == 2) {
        void** objNode;
        for (objNode = node->listA; objNode != 0; objNode = (void**)*objNode) {
            TerrainCallback_intersectpatch(query, objNode);
        }
        for (objNode = node->listB; objNode != 0; objNode = (void**)*objNode) {
            TerrainCallback_intersectinstance(query, objNode);
        }
        if (cell->level == 11) {
            return;
        }
        sCell_DBE8 sub;
        sub.level = cell->level - 1;
        sub.x = cell->x * 2;
        sub.y = cell->y * 2;
        sub.z = cell->z * 2;
        if (node->child[0]) {
            nash_privy_QueryHelper_help(flag, query, insideQuery, vctx, node->child[0], (sCell_369D8*)&sub);
        }
        sub.x++;
        if (node->child[4]) {
            nash_privy_QueryHelper_help(flag, query, insideQuery, vctx, node->child[4], (sCell_369D8*)&sub);
        }
        sub.y++;
        if (node->child[6]) {
            nash_privy_QueryHelper_help(flag, query, insideQuery, vctx, node->child[6], (sCell_369D8*)&sub);
        }
        sub.x--;
        if (node->child[2]) {
            nash_privy_QueryHelper_help(flag, query, insideQuery, vctx, node->child[2], (sCell_369D8*)&sub);
        }
        sub.z++;
        if (node->child[3]) {
            nash_privy_QueryHelper_help(flag, query, insideQuery, vctx, node->child[3], (sCell_369D8*)&sub);
        }
        sub.x++;
        if (node->child[7]) {
            nash_privy_QueryHelper_help(flag, query, insideQuery, vctx, node->child[7], (sCell_369D8*)&sub);
        }
        sub.y--;
        if (node->child[5]) {
            nash_privy_QueryHelper_help(flag, query, insideQuery, vctx, node->child[5], (sCell_369D8*)&sub);
        }
        sub.x--;
        if (node->child[1]) {
            nash_privy_QueryHelper_help(flag, query, insideQuery, vctx, node->child[1], (sCell_369D8*)&sub);
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX6);
#ifdef SKIP_ASM
struct sCell_33DFD8 {
    int level;
    int x, y, z;
};

struct sOct_33DFD8 {
    sOct_33DFD8* child[8];
    void* list;         // 0x20
};

extern "C" void nash_Cell_descend5(void* self, void* ctx);

static inline void collect_33DFD8(sOct_33DFD8* node, sCtx_336D40* ctx)
{
    void** p;
    for (p = (void**)node->list; p != 0; p = (void**)*p) {
        StaticObjectCallback_intersectinstance(ctx, p);
    }
    if (node->child[0] != 0) nash_Cell_descend5(node->child[0], ctx);
    if (node->child[1] != 0) nash_Cell_descend5(node->child[1], ctx);
    if (node->child[2] != 0) nash_Cell_descend5(node->child[2], ctx);
    if (node->child[3] != 0) nash_Cell_descend5(node->child[3], ctx);
    if (node->child[4] != 0) nash_Cell_descend5(node->child[4], ctx);
    if (node->child[5] != 0) nash_Cell_descend5(node->child[5], ctx);
    if (node->child[6] != 0) nash_Cell_descend5(node->child[6], ctx);
    if (node->child[7] != 0) nash_Cell_descend5(node->child[7], ctx);
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
extern "C" void nash_privy_QueryHelper_helpX6(char* flag, sCtx_336D40* ctx, sCtx_336D40* ctx2, void* vbox, void* vnode, void* vcell)
{
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sOct_33DFD8* node = (sOct_33DFD8*)vnode;
    sCell_33DFD8* cell = (sCell_33DFD8*)vcell;
    void** objNode;
    for (objNode = (void**)node->list; objNode != 0; objNode = (void**)*objNode) {
        StaticObjectCallback_intersectinstance(ctx, objNode);
    }
    if (cell->level == 11) {
        return;
    }
    sCell_33DFD8 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float cellSize = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sOct_33DFD8* subNode;
    subNode = node->child[0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->child[1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y++;
    subNode = node->child[3];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->child[2];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.x++;
    subNode = node->child[6];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z++;
    subNode = node->child[7];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.y--;
    subNode = node->child[5];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
    sub.z--;
    subNode = node->child[4];
    if (subNode && ox1 && oy0 && oz0) {
        if (cx1 && cy0 && cz0) collect_33DFD8(subNode, ctx2);
        else nash_privy_QueryHelper_helpX6(flag, ctx, ctx2, box, subNode, &sub);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_privy_QueryHelper_helpX7);
#ifdef SKIP_ASM
struct sQuery_33ED60 {
    sWsVec4* pos;               // 0x00
    int a7;                     // 0x04
    int mask;                   // 0x08
    int* cnt;                   // 0x0C
    float* dist;                // 0x10
    sBox_00334680* box;         // 0x14
};

struct sCell_33ED60 {
    int level;
    int x, y, z;
};

struct sOct_33ED60 {
    sOct_33ED60* child[8];
    int pad20[2];
    char* objs;                 // 0x28
};

// PORT: alias; the unit defines nash_Cell_descend6 later with its own struct types.
extern "C" void nash_Cell_descend6_33ED60(sOct_33ED60* tree, sQuery_33ED60* q) __asm__("nash_Cell_descend6");

static inline void objs_33ED60(char* o, sQuery_33ED60* q)
{
    for (; o != 0; o = *(char**)o) {
        int type = *(int*)(o + 8);
        if (type == 2) {
            cRailModifier_splineIntersect(*(void**)(o + 0xC), q->box, q->box, q->pos, q->mask, q->cnt, q->dist, q->a7);
        } else if (type == 3) {
            cFlexRailNode_splineIntersect(*(void**)(o + 0xC), q->box, q->box, q->pos, q->mask, q->cnt, q->dist, q->a7);
        } else if (type == 1) {
            if (*(int*)(*(char**)(o + 0x68) + 0x1C) & q->mask) {
                intersect_staticSplineSegmentIntersect(o, q->box, q->pos, q->cnt, q->dist, q->a7);
            }
        }
    }
}

static inline void collect_33ED60(sOct_33ED60* node, sQuery_33ED60* q)
{
    objs_33ED60(node->objs, q);
    if (node->child[0] != 0) nash_Cell_descend6_33ED60(node->child[0], q);
    if (node->child[1] != 0) nash_Cell_descend6_33ED60(node->child[1], q);
    if (node->child[2] != 0) nash_Cell_descend6_33ED60(node->child[2], q);
    if (node->child[3] != 0) nash_Cell_descend6_33ED60(node->child[3], q);
    if (node->child[4] != 0) nash_Cell_descend6_33ED60(node->child[4], q);
    if (node->child[5] != 0) nash_Cell_descend6_33ED60(node->child[5], q);
    if (node->child[6] != 0) nash_Cell_descend6_33ED60(node->child[6], q);
    if (node->child[7] != 0) nash_Cell_descend6_33ED60(node->child[7], q);
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
extern "C" void nash_privy_QueryHelper_helpX7(char* flag, sQry_337A48* vctx, sQry_337A48* vctx2, sBox_00334680* vbox, void* vnode, void* vcell)
{
    sQuery_33ED60* ctx = (sQuery_33ED60*)vctx;
    sQuery_33ED60* ctx2 = (sQuery_33ED60*)vctx2;
    sBox_3309D8* box = (sBox_3309D8*)vbox;
    sOct_33ED60* node = (sOct_33ED60*)vnode;
    sCell_33ED60* cell = (sCell_33ED60*)vcell;
    objs_33ED60(node->objs, ctx);
    if (cell->level == 11) {
        return;
    }
    sCell_33ED60 sub;
    sub.level = cell->level - 1;
    sub.x = cell->x * 2;
    sub.y = cell->y * 2;
    sub.z = cell->z * 2;
    int bits = (sub.level + 0x7F) << 23;
    float cellSize = *(float*)&bits;

    float x0lo = ((float)sub.x - 0.2f) * cellSize;
    float x0hi = ((float)(sub.x + 1) + 0.2f) * cellSize;
    int ox0 = 0;
    if (box->min[0] < x0hi && x0lo < box->max[0]) ox0 = 1;
    float x1lo = ((float)(sub.x + 1) - 0.2f) * cellSize;
    float x1hi = ((float)(sub.x + 2) + 0.2f) * cellSize;
    int ox1 = 0;
    if (box->min[0] < x1hi && x1lo < box->max[0]) ox1 = 1;
    if (!ox0 && !ox1) {
        return;
    }
    float y0lo = ((float)sub.y - 0.2f) * cellSize;
    float y0hi = ((float)(sub.y + 1) + 0.2f) * cellSize;
    int oy0 = 0;
    if (box->min[1] < y0hi && y0lo < box->max[1]) oy0 = 1;
    float y1lo = ((float)(sub.y + 1) - 0.2f) * cellSize;
    float y1hi = ((float)(sub.y + 2) + 0.2f) * cellSize;
    int oy1 = 0;
    if (box->min[1] < y1hi && y1lo < box->max[1]) oy1 = 1;
    if (!oy0 && !oy1) {
        return;
    }
    float z0lo = ((float)sub.z - 0.2f) * cellSize;
    float z0hi = ((float)(sub.z + 1) + 0.2f) * cellSize;
    int oz0 = 0;
    if (box->min[2] < z0hi && z0lo < box->max[2]) oz0 = 1;
    float z1lo = ((float)(sub.z + 1) - 0.2f) * cellSize;
    float z1hi = ((float)(sub.z + 2) + 0.2f) * cellSize;
    int oz1 = 0;
    if (box->min[2] < z1hi && z1lo < box->max[2]) oz1 = 1;
    if (!oz0 && !oz1) {
        return;
    }
    int cx0 = 0;
    if (box->min[0] < x0lo && x0hi < box->max[0]) cx0 = 1;
    int cx1 = 0;
    if (box->min[0] < x1lo && x1hi < box->max[0]) cx1 = 1;
    int cy0 = 0;
    if (box->min[1] < y0lo && y0hi < box->max[1]) cy0 = 1;
    int cy1 = 0;
    if (box->min[1] < y1lo && y1hi < box->max[1]) cy1 = 1;
    int cz0 = 0;
    if (box->min[2] < z0lo && z0hi < box->max[2]) cz0 = 1;
    int cz1 = 0;
    if (box->min[2] < z1lo && z1hi < box->max[2]) cz1 = 1;

    sOct_33ED60* subNode;
    subNode = node->child[0];
    if (subNode && ox0 && oy0 && oz0) {
        if (cx0 && cy0 && cz0) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z++;
    subNode = node->child[1];
    if (subNode && ox0 && oy0 && oz1) {
        if (cx0 && cy0 && cz1) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.y++;
    subNode = node->child[3];
    if (subNode && ox0 && oy1 && oz1) {
        if (cx0 && cy1 && cz1) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z--;
    subNode = node->child[2];
    if (subNode && ox0 && oy1 && oz0) {
        if (cx0 && cy1 && cz0) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.x++;
    subNode = node->child[6];
    if (subNode && ox1 && oy1 && oz0) {
        if (cx1 && cy1 && cz0) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z++;
    subNode = node->child[7];
    if (subNode && ox1 && oy1 && oz1) {
        if (cx1 && cy1 && cz1) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.y--;
    subNode = node->child[5];
    if (subNode && ox1 && oy0 && oz1) {
        if (cx1 && cy0 && cz1) collect_33ED60(subNode, ctx2);
        else nash_privy_QueryHelper_helpX7(flag, vctx, vctx2, vbox, subNode, &sub);
    }
    sub.z--;
    subNode = node->child[4];
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
// D_004FF640: the eight corners (+-1, +-1, +-1, 0), written as one 32-float block.
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
extern sCorners_3400D8 D_004FF640_c __asm__("D_004FF640");


// Static initialisation of the unit's vector globals D_004FF2C0..D_004FF630 and the corner table D_004FF640
// (g++ __static_initialization_and_destruction_0).
// The unit declares func_003400D8 returning void*; bind this definition by asm label.
extern "C" void func_003400D8_init(int initialize, int priority) __asm__("func_003400D8");
/**
 * Constructs the unit's static constants: 56 colours and the eight unit-cube corners.
 *
 * Runs only for @p initialize non-zero and @p priority 0xFFFF (func_00341368 passes 1, 0xFFFF). The colours
 * (D_004FF2C0..D_004FF630, four floats each) are the table also built in pathsys, memstd and all_in_one_scripter;
 * D_004FF640 gets the corners (+-1, +-1, +-1, 0) in x-major order, the default axes riderspheretree's sphere-tree code
 * copies (g++ __static_initialization_and_destruction_0).
 *
 * @param initialize  non-zero to construct
 * @param priority  the init priority; only 0xFFFF does anything
 */
extern "C" void func_003400D8_init(int initialize, int priority)
{
    if (priority == 0xFFFF && initialize != 0) {
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
        D_004FF640_c.init();
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
void cWorldIntersectType_IntersectSubPatch(void* self)
{
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
float cWorldLine_GetIdealParam(void* self)
{
    return *(float*)((char*)self + 0xA0);
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
extern "C" float cWorldSphere_GetIdealParam(void* self)
{
    if (*(int*)self == 0) {
        return -1.0f;
    }
    return *(float*)((char*)self + 0x70) * 2.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend);
#ifdef SKIP_ASM
struct sSphNode_340A40 {
    sSphNode_340A40* child[8];
};

/**
 * Collects the overlapping instances of an octree cell and of every cell below it.
 *
 * Runs OverlapInstancesCallback_operatorCall(@p ctx, @p node) on the cell, then calls itself on each non-null child
 * (+0x0..+0x1C). Used for cells that lie wholly inside the query box (cOverlapList_getinstances).
 *
 * @param node  the octree cell (On Tour nash::Cell)
 * @param ctx  the instance callback: list, count pointer, maximum, query box (On Tour OverlapInstancesCallback)
 */
extern "C" void nash_Cell_descend(sSphNode_340A40* node, sCtx_3309D8* ctx)
{
    OverlapInstancesCallback_operatorCall(ctx, node);
    if (node->child[0] != 0) nash_Cell_descend(node->child[0], ctx);
    if (node->child[1] != 0) nash_Cell_descend(node->child[1], ctx);
    if (node->child[2] != 0) nash_Cell_descend(node->child[2], ctx);
    if (node->child[3] != 0) nash_Cell_descend(node->child[3], ctx);
    if (node->child[4] != 0) nash_Cell_descend(node->child[4], ctx);
    if (node->child[5] != 0) nash_Cell_descend(node->child[5], ctx);
    if (node->child[6] != 0) nash_Cell_descend(node->child[6], ctx);
    if (node->child[7] != 0) nash_Cell_descend(node->child[7], ctx);
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
extern "C" void nash_Cell_descend1(void* self, sOctCollect* out)
{
    void** objNode;
    for (objNode = *(void***)((char*)self + 0x28); objNode != 0; objNode = (void**)*objNode) {
        out->arr[(*out->count)++] = objNode;
    }
    if (*(void**)((char*)self + 0x0)) nash_Cell_descend1(*(void**)((char*)self + 0x0), out);
    if (*(void**)((char*)self + 0x4)) nash_Cell_descend1(*(void**)((char*)self + 0x4), out);
    if (*(void**)((char*)self + 0x8)) nash_Cell_descend1(*(void**)((char*)self + 0x8), out);
    if (*(void**)((char*)self + 0xC)) nash_Cell_descend1(*(void**)((char*)self + 0xC), out);
    if (*(void**)((char*)self + 0x10)) nash_Cell_descend1(*(void**)((char*)self + 0x10), out);
    if (*(void**)((char*)self + 0x14)) nash_Cell_descend1(*(void**)((char*)self + 0x14), out);
    if (*(void**)((char*)self + 0x18)) nash_Cell_descend1(*(void**)((char*)self + 0x18), out);
    if (*(void**)((char*)self + 0x1C)) nash_Cell_descend1(*(void**)((char*)self + 0x1C), out);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend2);
#ifdef SKIP_ASM
struct sObj_340C20 {
    sObj_340C20* next;
    char pad04[0x154];
    float bmin[3];              // 0x158
    float bmax[3];              // 0x164
};

struct sTree_340C20 {
    sTree_340C20* child[8];
    int pad20;
    sObj_340C20* objs;          // 0x24
};

static inline int overlapObj_340C20(const sBox_3309D8* b, const sObj_340C20* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

/**
 * Collects the overlapping instances and patches of an octree cell and of every cell below it.
 *
 * Runs OverlapInstancesCallback_operatorCall on the cell with the instance part of @p ctx, then appends each patch on
 * the cell's patch list (+0x24) whose box (+0x158 min, +0x164 max) overlaps the query box to the patch part (@p ctx
 * +0x10: list, count pointer, box at +0xC), and calls itself on each non-null child. Used by cOverlapList_getpatchesandinstances and
 * nash_privy_QueryHelper_helpX3 for cells wholly inside the query box.
 *
 * @param tree  the octree cell (On Tour nash::Cell)
 * @param ctx  the callback: instance part (+0x0) and patch part (+0x10)
 */
extern "C" void nash_Cell_descend2(sTree_340C20* tree, sCtx_3309D8* ctx)
{
    OverlapInstancesCallback_operatorCall(ctx, tree);
    sCtx_3309D8* objCtx = (sCtx_3309D8*)((char*)ctx + 0x10);
    for (sObj_340C20* obj = tree->objs; obj != 0; obj = obj->next) {
        if (overlapObj_340C20(objCtx->box, obj)) {
            int* cnt = objCtx->count;
            int n = *cnt;
            objCtx->list[n] = (sNode_3309D8*)obj;
            *cnt = n + 1;
        }
    }
    if (tree->child[0] != 0) nash_Cell_descend2(tree->child[0], ctx);
    if (tree->child[1] != 0) nash_Cell_descend2(tree->child[1], ctx);
    if (tree->child[2] != 0) nash_Cell_descend2(tree->child[2], ctx);
    if (tree->child[3] != 0) nash_Cell_descend2(tree->child[3], ctx);
    if (tree->child[4] != 0) nash_Cell_descend2(tree->child[4], ctx);
    if (tree->child[5] != 0) nash_Cell_descend2(tree->child[5], ctx);
    if (tree->child[6] != 0) nash_Cell_descend2(tree->child[6], ctx);
    if (tree->child[7] != 0) nash_Cell_descend2(tree->child[7], ctx);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend3);
#ifdef SKIP_ASM
struct sCtx_3309D8;
extern "C" void OverlapInstancesCallback_operatorCall(sCtx_3309D8* ctx, void* tree);

struct sBox_340DC0 {
    float min[4];
    float max[4];
};

struct sSub_340DC0 {
    void** list;
    int* count;
    int pad08;
    sBox_340DC0* box;
};

struct sCtx_340DC0 {
    sSub_340DC0 nodes;
    sSub_340DC0 objs;
    sSub_340DC0 all;
};

struct sObjNode_340DC0 {
    sObjNode_340DC0* next;
    int pad04[85];
    float bmin[3];
    float bmax[3];
};

struct sTree_340DC0 {
    sTree_340DC0* child[8];
    void* nodes;
    sObjNode_340DC0* objs;
    void** all;
};

static inline void add_340DC0(sSub_340DC0* s, void* p)
{
    int* cnt = s->count;
    int n = *cnt;
    s->list[n] = p;
    *cnt = n + 1;
}

static inline int overlap_340DC0(const sBox_340DC0* b, const sObjNode_340DC0* n)
{
    int r = 0;
    if (b->min[0] <= n->bmax[0] && b->max[0] >= n->bmin[0] &&
        b->min[1] <= n->bmax[1] && b->max[1] >= n->bmin[1] &&
        b->min[2] <= n->bmax[2] && b->max[2] >= n->bmin[2]) {
        r = 1;
    }
    return r;
}

/**
 * Collects every instance, patch and typed node of an octree cell and of every cell below it.
 *
 * Runs OverlapInstancesCallback_operatorCall with the instance part of @p ctx (+0x0), appends the patches on the +0x24
 * list whose box (+0x158/+0x164) overlaps the patch part's box to that part (+0x10), appends every typed node (+0x28)
 * to the third part (+0x20) unconditionally, then calls itself on each non-null child. Used by cOverlapList_geteverything for cells
 * wholly inside the query box.
 *
 * @param ctx  the callback: instance (+0x0), patch (+0x10) and typed-node (+0x20) parts, each list, count pointer, box
 */
extern "C" void nash_Cell_descend3(sTree_340DC0* self, sCtx_340DC0* ctx)
{
    OverlapInstancesCallback_operatorCall((sCtx_3309D8*)ctx, self);
    sSub_340DC0* bucket = &ctx->objs;
    for (sObjNode_340DC0* objNode = self->objs; objNode != 0; objNode = objNode->next) {
        if (overlap_340DC0(bucket->box, objNode)) {
            add_340DC0(bucket, objNode);
        }
    }
    bucket = &ctx->all;
    for (void** allNode = self->all; allNode != 0; allNode = (void**)*allNode) {
        add_340DC0(bucket, allNode);
    }
    if (self->child[0]) nash_Cell_descend3(self->child[0], ctx);
    if (self->child[1]) nash_Cell_descend3(self->child[1], ctx);
    if (self->child[2]) nash_Cell_descend3(self->child[2], ctx);
    if (self->child[3]) nash_Cell_descend3(self->child[3], ctx);
    if (self->child[4]) nash_Cell_descend3(self->child[4], ctx);
    if (self->child[5]) nash_Cell_descend3(self->child[5], ctx);
    if (self->child[6]) nash_Cell_descend3(self->child[6], ctx);
    if (self->child[7]) nash_Cell_descend3(self->child[7], ctx);
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
extern "C" void nash_Cell_descend4(void* self, void* ctx)
{
    void** objNode;
    for (objNode = *(void***)((char*)self + 0x24); objNode != 0; objNode = (void**)*objNode) {
        TerrainCallback_intersectpatch(ctx, objNode);
    }
    for (objNode = *(void***)((char*)self + 0x20); objNode != 0; objNode = (void**)*objNode) {
        TerrainCallback_intersectinstance(ctx, objNode);
    }
    if (*(void**)((char*)self + 0x0)) nash_Cell_descend4(*(void**)((char*)self + 0x0), ctx);
    if (*(void**)((char*)self + 0x4)) nash_Cell_descend4(*(void**)((char*)self + 0x4), ctx);
    if (*(void**)((char*)self + 0x8)) nash_Cell_descend4(*(void**)((char*)self + 0x8), ctx);
    if (*(void**)((char*)self + 0xC)) nash_Cell_descend4(*(void**)((char*)self + 0xC), ctx);
    if (*(void**)((char*)self + 0x10)) nash_Cell_descend4(*(void**)((char*)self + 0x10), ctx);
    if (*(void**)((char*)self + 0x14)) nash_Cell_descend4(*(void**)((char*)self + 0x14), ctx);
    if (*(void**)((char*)self + 0x18)) nash_Cell_descend4(*(void**)((char*)self + 0x18), ctx);
    if (*(void**)((char*)self + 0x1C)) nash_Cell_descend4(*(void**)((char*)self + 0x1C), ctx);
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
extern "C" void nash_Cell_descend5(void* self, void* ctx)
{
    void** objNode;
    for (objNode = *(void***)((char*)self + 0x20); objNode != 0; objNode = (void**)*objNode) {
        StaticObjectCallback_intersectinstance(ctx, objNode);
    }
    if (*(void**)((char*)self + 0x0)) nash_Cell_descend5(*(void**)((char*)self + 0x0), ctx);
    if (*(void**)((char*)self + 0x4)) nash_Cell_descend5(*(void**)((char*)self + 0x4), ctx);
    if (*(void**)((char*)self + 0x8)) nash_Cell_descend5(*(void**)((char*)self + 0x8), ctx);
    if (*(void**)((char*)self + 0xC)) nash_Cell_descend5(*(void**)((char*)self + 0xC), ctx);
    if (*(void**)((char*)self + 0x10)) nash_Cell_descend5(*(void**)((char*)self + 0x10), ctx);
    if (*(void**)((char*)self + 0x14)) nash_Cell_descend5(*(void**)((char*)self + 0x14), ctx);
    if (*(void**)((char*)self + 0x18)) nash_Cell_descend5(*(void**)((char*)self + 0x18), ctx);
    if (*(void**)((char*)self + 0x1C)) nash_Cell_descend5(*(void**)((char*)self + 0x1C), ctx);
}
#endif

extern "C" void* func_003400D8(int, int);

//100%
INCLUDE_ASM("intersect/worldsphtree", nash_Cell_descend6);
#ifdef SKIP_ASM
struct sQuery_3411B8 {
    sWsVec4* pos;               // 0x00
    int a7;                     // 0x04
    int mask;                   // 0x08
    int* cnt;                   // 0x0C
    float* dist;                // 0x10
    sBox_00334680* box;         // 0x14
};

struct sTree_3411B8 {
    sTree_3411B8* child[8];
    int pad20[2];
    char* objs;                 // 0x28
};

/**
 * Runs the spline query on every typed node of an octree cell and of every cell below it.
 *
 * For each node on the typed list (+0x28): rails (type 2) go to cRailModifier_splineIntersect, flex rails (type 3) to
 * cFlexRailNode_splineIntersect, and instances (type 1) whose filter word (node +0x68, then +0x1C) shares a bit with the query mask to
 * intersect_staticSplineSegmentIntersect, all with the query box, position, mask, hit-count and distance pointers and intInfo. It then calls
 * itself on each non-null child. Used by cBXNash_splineIntersect and nash_privy_QueryHelper_helpX7 for cells wholly inside the query box.
 *
 * @param tree  the octree cell (On Tour nash::Cell)
 * @param splineQuery  the spline query (On Tour SplineCallback)
 */
extern "C" void nash_Cell_descend6(sTree_3411B8* tree, sQuery_3411B8* splineQuery)
{
    for (char* typedNode = tree->objs; typedNode != 0; typedNode = *(char**)typedNode) {
        int type = *(int*)(typedNode + 8);
        if (type == 2) {
            cRailModifier_splineIntersect(*(void**)(typedNode + 0xC), splineQuery->box, splineQuery->box, splineQuery->pos, splineQuery->mask, splineQuery->cnt, splineQuery->dist, splineQuery->a7);
        } else if (type == 3) {
            cFlexRailNode_splineIntersect(*(void**)(typedNode + 0xC), splineQuery->box, splineQuery->box, splineQuery->pos, splineQuery->mask, splineQuery->cnt, splineQuery->dist, splineQuery->a7);
        } else if (type == 1) {
            if (*(int*)(*(char**)(typedNode + 0x68) + 0x1C) & splineQuery->mask) {
                intersect_staticSplineSegmentIntersect(typedNode, splineQuery->box, splineQuery->pos, splineQuery->cnt, splineQuery->dist, splineQuery->a7);
            }
        }
    }
    if (tree->child[0] != 0) nash_Cell_descend6(tree->child[0], splineQuery);
    if (tree->child[1] != 0) nash_Cell_descend6(tree->child[1], splineQuery);
    if (tree->child[2] != 0) nash_Cell_descend6(tree->child[2], splineQuery);
    if (tree->child[3] != 0) nash_Cell_descend6(tree->child[3], splineQuery);
    if (tree->child[4] != 0) nash_Cell_descend6(tree->child[4], splineQuery);
    if (tree->child[5] != 0) nash_Cell_descend6(tree->child[5], splineQuery);
    if (tree->child[6] != 0) nash_Cell_descend6(tree->child[6], splineQuery);
    if (tree->child[7] != 0) nash_Cell_descend6(tree->child[7], splineQuery);
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
struct sVec4_341388 {
    float x, y, z, w;
    sVec4_341388() {}
    sVec4_341388(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sDef_341388 {
    int f00;
    short type;
    short pad06;
    float time;
    float f0C;
    float f10;
    float x, y, z;
};

struct sObj_341388 {
    int pad00[3];
    void** vt;
    int pad10[2];
    char* mtx;
    int pad1C;
    sVec4_341388 pos;
    float speed;
    float f34;
    int t38;
    int t3C;
    int type;
};

extern void* cBoostNode_vtbl[];
extern char* cAppMan_TheAppMan;
extern sVec4_341388 D_004FF120_341388[] __asm__("t4vect_Zero");
extern sVec4_341388 D_004FF150_341388[] __asm__("t4vect_YAxis");
extern "C" void* cInstanceNode_cInstanceNode1(void* self, void* a1, int type, void* a3);

// PORT: PS2-only VU0 inline asm (v / |v|).
static inline sVec4_341388 vu0Normalize_341388(const sVec4_341388& v)
{
    sVec4_341388 r;
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
static inline sVec4_341388 vu0Scale_341388(const sVec4_341388& v, float s)
{
    sVec4_341388 r;
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
static inline sVec4_341388 vu0Xform_341388(const sVec4_341388& v, const char* m)
{
    sVec4_341388 r;
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
 * Runs the base constructor cInstanceNode_cInstanceNode1(self, @p bucket, @p nodeType, @p instance) and stores cBoostNode_vtbl at +0xC. From @p def
 * (On Tour tMakeBoostData) it takes the type (+0x4, read as a short) into +0x40; the duration BoostTime (+0x8) times
 * the frame rate (cAppMan_TheAppMan +0x10) into +0x38 as a frame count; the direction BoostX/Y/Z (+0x14) normalised
 * into +0x20, or when it is zero the Y axis (t4vect_YAxis) for type 2 and the zero vector otherwise; MaxSpeed (+0x10)
 * times 27.7778 (km/h to cm/s) into +0x30; and Drag (+0xC) into +0x34. The timer (+0x3C) starts at the duration for a
 * non-zero type, else 0. For type 2 the direction is negated and transformed by the instance's matrix (instance record
 * at +0x18, matrix at its +0x10) and the speed is negated.
 *
 * MakeBoostFunc (a world-script maker) allocates it as 0x50 bytes tagged "Boost" and passes bucket 1, type 8.
 *
 * @param bucket  the bucket list the node joins
 * @param nodeType  the object-node type (8)
 * @param instance  the instance record
 * @param def  the script's boost data (On Tour tMakeBoostData)
 * @return @p self
 */
extern "C" sObj_341388* cBoostNode_cBoostNode1(sObj_341388* self, void* bucket, int nodeType, void* instance, sDef_341388* def)
{
    cInstanceNode_cInstanceNode1(self, bucket, nodeType, instance);
    self->vt = cBoostNode_vtbl;
    int type = def->type;
    self->type = type;
    self->t38 = (int)(def->time * (float)*(int*)(cAppMan_TheAppMan + 0x10));
    if (def->x == 0.0f && def->y == 0.0f && def->z == 0.0f) {
        if (type == 2) {
            self->pos = *D_004FF150_341388;
        } else {
            self->pos = *D_004FF120_341388;
        }
    } else {
        self->pos = vu0Normalize_341388(sVec4_341388(def->x, def->y, def->z, 0.0f));
    }
    self->speed = def->f10 * 27.77777862548828f;
    self->f34 = def->f0C;
    self->t3C = self->type ? self->t38 : 0;
    if (self->type == 2) {
        char* instMatrix = self->mtx + 0x10;
        self->pos = vu0Xform_341388(vu0Scale_341388(self->pos, -1.0f), instMatrix);
        self->speed = -self->speed;
    }
    return self;
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
extern "C" void* cBoostNode_cBoostNode(void* self, void* bucket, void* stream)
{
    cInstanceNode_cInstanceNode(self, bucket, stream);
    *(void***)((char*)self + 0xC) = cBoostNode_vtbl;
    sVEntry_cBoostNode_cBoostNode* vt = *(sVEntry_cBoostNode_cBoostNode**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x20, 0x30);
    return self;
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
void* cBoostNode__cBoostNode(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)cBoostNode_vtbl;
    return cInstanceNode__cInstanceNode(self);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_update);
#ifdef SKIP_ASM
extern "C" int func_002D1AF0();
extern "C" void** cObjectInterface_getBeing(int i);

struct sVEnt_003415D0 { short delta; short index; void* fn; };
typedef void (*Fn0_003415D0)(void*);
typedef void (*Fn1_003415D0)(void*, void*);
typedef void (*FnI_003415D0)(void*, int);
typedef int (*Test0_003415D0)(void*);
typedef int (*Test_003415D0)(void*, int);

struct sNode_003415D0 {
    int pad00[3];
    sVEnt_003415D0* vt;
    int pad10[2];
    int a18;
    int pad1C[6];
    float f34;
    int pad38;
    int t3C;
    int type;
};

/**
 * Applies the boost node to every being touching it, and expires a timed (type 1) node once nothing keeps it
 * alive.
 *
 * Counts the timer (+0x3C) down. For type 1, or once the timer is out, each being touching the node (being
 * slot 3 with the node's Instance; type 2 skips computer riders, slot 9) is slowed (slot 48: velocity times
 * (1 + factor)) when the factor (+0x34) is negative, else pushed along the boost direction (slot 47). A type
 * 1 node that no being touches and whose timer is out expires (slot 34, runscript 1); slots 49/50 (empty)
 * bracket the loop.
 */
extern "C" void cBoostNode_update(sNode_003415D0* self)
{
    int hit = 0;
    if (self->type != 1) {
        hit = 1;
    }
    int timeLeft = self->t3C;
    if (timeLeft > 0) {
        int n = timeLeft - 1;
        self->t3C = n;
        if (n > 0) {
            hit = 1;
        }
    }
    sVEnt_003415D0* vt = self->vt;
    ((Fn0_003415D0)vt[49].fn)((char*)self + vt[49].delta);
    if (self->type == 1 || self->t3C <= 0) {
        for (int i = 0; i < func_002D1AF0(); i++) {
            void** obj = cObjectInterface_getBeing(i);
            if (self->type == 2) {
                sVEnt_003415D0* ovt = (sVEnt_003415D0*)*obj;
                if (((Test0_003415D0)ovt[9].fn)((char*)obj + ovt[9].delta) != 0) continue;
            }
            sVEnt_003415D0* ovt = (sVEnt_003415D0*)*obj;
            if (((Test_003415D0)ovt[3].fn)((char*)obj + ovt[3].delta, self->a18) != 0) {
                hit = 1;
                if (self->f34 < 0.0f) {
                    sVEnt_003415D0* vtbl = self->vt;
                    ((Fn1_003415D0)vtbl[48].fn)((char*)self + vtbl[48].delta, obj);
                } else {
                    sVEnt_003415D0* vtbl = self->vt;
                    ((Fn1_003415D0)vtbl[47].fn)((char*)self + vtbl[47].delta, obj);
                }
            }
        }
    }
    sVEnt_003415D0* vtbl2 = self->vt;
    ((Fn0_003415D0)vtbl2[50].fn)((char*)self + vtbl2[50].delta);
    if (hit == 0) {
        sVEnt_003415D0* vtbl3 = self->vt;
        ((FnI_003415D0)vtbl3[34].fn)((char*)self + vtbl3[34].delta, 1);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_applyDrag);
#ifdef SKIP_ASM
struct sVec4_341770 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_341770 vu0Scale_341770(const sVec4_341770& v, float s)
{
    sVec4_341770 r;
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

class cMover_341770 {
public:
    virtual void v01();
    virtual sVec4_341770* getVelocity();
    virtual void v03();
    virtual void applyBoost(sVec4_341770* v);
};

extern "C" float cObjectInterface_getUpdateTime();

/**
 * Applies the node's drag to a rider.
 *
 * Passes the rider's velocity * (1 + the drag at +0x34) * cObjectInterface_getUpdateTime() to its applyBoost (cBeing
 * vtable slot 4).
 *
 * @param obj  the rider (cBeing)
 */
extern "C" void cBoostNode_applyDrag(void* self, cMover_341770* obj)
{
    sVec4_341770 newVel = vu0Scale_341770(vu0Scale_341770(*obj->getVelocity(), *(float*)((char*)self + 0x34) + 1.0f), cObjectInterface_getUpdateTime());
    obj->applyBoost(&newVel);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_applyBoost);
#ifdef SKIP_ASM
struct sVec4_341818 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (v / |v|).
static inline sVec4_341818 vu0Normalize_341818(const sVec4_341818& v)
{
    sVec4_341818 r;
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
static inline float vu0Dot_341818(const sVec4_341818& a, const sVec4_341818& b)
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
static inline sVec4_341818 vu0Scale_341818(const sVec4_341818& v, float s)
{
    sVec4_341818 r;
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

class cMover_341818 {
public:
    virtual void v01();
    virtual sVec4_341818* getVelocity();
    virtual void v03();
    virtual void applyBoost(sVec4_341818* v);
};

extern "C" float cObjectInterface_getUpdateTime();

struct sParams_341818 {
    sVec4_341818 dir;
    float speed;
    float scale;
};

struct sSph_341818 {
    char pad00[0x20];
    sParams_341818 params;
    int type;
};

static inline void push_341818(sParams_341818* p, cMover_341818* obj, const sVec4_341818& dir, float amount)
{
    sVec4_341818 u = vu0Scale_341818(vu0Scale_341818(vu0Scale_341818(dir, amount), p->scale), cObjectInterface_getUpdateTime());
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
extern "C" void cBoostNode_applyBoost(sSph_341818* self, cMover_341818* obj)
{
    sVec4_341818 vel = *obj->getVelocity();
    sVec4_341818 dir = self->params.dir;
    if (dir.x == 0.0f && dir.y == 0.0f && dir.z == 0.0f) {
        dir = vu0Normalize_341818(vel);
    }
    float alongSpeed = vu0Dot_341818(vel, dir);
    float diff = self->params.speed - alongSpeed;
    sParams_341818* boostParams = &self->params;
    if (self->type == 2) {
        if (alongSpeed >= 0.0f) {
            sVec4_341818 newVel = vu0Scale_341818(vu0Scale_341818(vu0Scale_341818(dir, boostParams->speed), boostParams->scale), cObjectInterface_getUpdateTime());
            obj->applyBoost(&newVel);
        }
    } else if (diff > 0.0f) {
        sVec4_341818 newVel = vu0Scale_341818(vu0Scale_341818(vu0Scale_341818(dir, diff), boostParams->scale), cObjectInterface_getUpdateTime());
        obj->applyBoost(&newVel);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_signal);
#ifdef SKIP_ASM
/**
 * Handles a signal sent to the boost node: signal 1 restarts its timer.
 *
 * Reloads the timer (+0x3C) from the duration (+0x38); other signals are ignored. On Tour's float argument is
 * not taken.
 *
 * @param signal  the signal number
 */
extern "C" void cBoostNode_signal(void* self, int signal)
{
    if (signal == 1) {
        *(int*)((char*)self + 0x3c) = *(int*)((char*)self + 0x38);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostNode_writeToReplayFrame);
#ifdef SKIP_ASM
struct sSerVEntry_00341A50 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void cInstanceNode_writeToReplayFrame(void* self, void* stream);

/**
 * Writes a boost node's state to a replay frame.
 *
 * Writes the instance-node part with cInstanceNode_writeToReplayFrame(), then the 0x30 bytes at offset 0x20
 * through the stream's write virtual (slot 1). The stream constructor reads the same block back.
 *
 * @param stream  replay stream to write to
 */
extern "C" void cBoostNode_writeToReplayFrame(void* self, void* stream)
{
    cInstanceNode_writeToReplayFrame(self, stream);
    sSerVEntry_00341A50* vt = *(sSerVEntry_00341A50**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x20, 0x30);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_cAnimObjectNode1);
#ifdef SKIP_ASM
extern char* cAppMan_TheAppMan;
extern void* D_00490B10[];
extern "C" void cAnimNode_cAnimNode(void* p, int a1, int a2, void* obj, int a4);
extern "C" float cAnimFrame_getStartTime(void* inst);
float AIrandf(float lo, float hi);
float func_00317890(float a, float b);

struct sSphFloat_1AA0 {
    unsigned short id;  // 0x00
    char pad02[0x2];
    int active;         // 0x04
    int atLimit;        // 0x08
    int fC;             // 0x0C
    float speed;        // 0x10
    float min;          // 0x14
    float max;          // 0x18
    float value;        // 0x1C
    float start;        // 0x20
    char pad24[0x18];
    void** vt;          // 0x3C
    char pad40[0x2];
    unsigned short flags; // 0x42
    char pad44[0x20];
    void* inst;         // 0x64
};

struct sSphFloatDef_1AA0 {
    int f0;
    unsigned short id;  // 0x04
    char pad06[0x2];
    int dir;            // 0x08
    float min;          // 0x0C
    float max;          // 0x10
    float speed;        // 0x14
    float speedVar;     // 0x18
    float value;        // 0x1C
    int random;         // 0x20
    int f24;            // 0x24
};

struct sFloat_1AA0 {
    float v;
};

static inline void setRate_1AA0(sFloat_1AA0* dst, float v, float dflt)
{
    float r;
    if (v >= 0.0f) {
        r = v * 0.03333333507180214f;
    } else {
        r = dflt;
    }
    dst->v = r;
}

static inline void setRateMax_1AA0(sFloat_1AA0* dst, float v, char* info)
{
    float r;
    if (v < 0.0f) {
        r = *(float*)(info + 0x14);
    } else {
        r = v * 0.03333333507180214f;
    }
    dst->v = r;
}

/**
 * Constructs an animated object node from its script data (the record counterpart of the replay constructor
 * cAnimObjectNode_cAnimObjectNode).
 *
 * Builds the animation-node base at +0x1C with cAnimNode_cAnimNode(base, @p bucket, @p nodeType, @p obj, ForceVisible != 0) and stores
 * the vtable D_00490B10 at +0x3C. From @p def (On Tour tMakeAnimObjectData, times in 30 fps frames converted to
 * seconds) it sets the play type (+0x0) from +0x4; min (+0x14) from FromTime (+0xC), or the animation's start time
 * (cAnimFrame_getStartTime on the animation at +0x64) when negative; max (+0x18) from ToTime (+0x10), or the animation
 * length (the instance's info at +0x80, its +0x14) when negative; and the frame (+0x1C) from StartTime (+0x1C; negative
 * means min), clamped to [min, max] or replaced by AIrandf(min, max) when RandomStart (+0x20) is set.
 *
 * The per-frame speed (+0x10) is Speed (+0x14) / 30 / the frame rate (cAppMan_TheAppMan +0x10), randomised by
 * func_00317890 with SpeedRange (+0x18) when that is non-zero, and negated for Dir (+0x8) 1. It then copies the frame
 * to +0x20, sets active (+0x4) and the dirty flag (bit 0 of +0x42), and clears +0xC (the start delay) and atLimit
 * (+0x8). The struct calls the play type at +0x0 "id"; cAnimObjectNode_updateNode switches on it.
 *
 * @param bucket  the bucket list the node joins
 * @param nodeType  the object-node type
 * @param obj  the instance record
 * @param def  the script's animation data (On Tour tMakeAnimObjectData)
 * @return @p self
 */
// Animated float constructor: range and start value (frames -> seconds), optional random start and speed.
extern "C" void* cAnimObjectNode_cAnimObjectNode1(sSphFloat_1AA0* self, int bucket, int nodeType, char* obj, sSphFloatDef_1AA0* def)
{
    cAnimNode_cAnimNode((char*)self + 0x1C, bucket, nodeType, obj, def->f24 != 0);
    self->vt = D_00490B10;
    float frameTime = 0.0f;
    char* info = *(char**)(obj + 0x80);
    if (self->inst != 0) {
        frameTime = cAnimFrame_getStartTime(self->inst);
    }
    self->id = def->id;
    setRate_1AA0((sFloat_1AA0*)&self->min, def->min, frameTime);
    setRateMax_1AA0((sFloat_1AA0*)&self->max, def->max, info);
    frameTime = def->value;
    if (frameTime < 0.0f) {
        frameTime = self->min;
    } else {
        frameTime *= 0.03333333507180214f;
    }
    if (frameTime < self->min) {
        frameTime = self->min;
    }
    if (self->max < frameTime) {
        frameTime = self->max;
    }
    if (def->random != 0) {
        frameTime = AIrandf(self->min, self->max);
    }
    self->value = frameTime;
    if (def->speedVar != 0.0f) {
        self->speed = func_00317890(def->speed * 0.03333333507180214f, def->speedVar * 0.03333333507180214f) / (float)*(int*)(cAppMan_TheAppMan + 0x10);
    } else {
        self->speed = def->speed * 0.03333333507180214f / (float)*(int*)(cAppMan_TheAppMan + 0x10);
    }
    if (def->dir == 1) {
        self->speed = -self->speed;
    }
    self->start = self->value;
    self->active = 1;
    self->flags |= 1;
    self->fC = 0;
    self->atLimit = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_cAnimObjectNode);
#ifdef SKIP_ASM
struct sVEntry00341C80 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* cAnimNode_cAnimNode1(void* p, int a1, void* stream);
extern void* D_00490B10[];

/**
 * Constructs an animated object node from a replay frame.
 *
 * Builds the animation node base at +0x1C with cAnimNode_cAnimNode1 (its cMoveNode part at +0x30), stores this
 * class's vtable D_00490B10 at +0x3C, reads the 0x1C-byte object state into +0x0 from @p stream (read
 * virtual, slot 2) and sets the root-transform-dirty flag (bit 0 of Flags, +0x42).
 *
 * @param bucket  bucket list the node joins, passed on to the base constructor
 * @param stream  the replay frame to read from (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cAnimObjectNode_cAnimObjectNode(void* self, int bucket, sVEntry00341C80** stream)
{
    cAnimNode_cAnimNode1((char*)self + 0x1C, bucket, stream);
    *(void***)((char*)self + 0x3C) = D_00490B10;
    (*stream)[2].fn((char*)stream + (*stream)[2].delta, self, 0x1C);
    *(unsigned short*)((char*)self + 0x42) |= 1;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode__cAnimObjectNode);
#ifdef SKIP_ASM
extern "C" void cAnimNode__cAnimNode(void* p, int flags);
void operator_delete(int* ptr);
extern void* D_00490B10[];

/**
 * Destroys an animated object node: restores its vtable and runs the animation-node destructor.
 *
 * Stores D_00490B10 at +0x3C, calls cAnimNode__cAnimNode on the base at +0x1C (with flags 0) and frees the object with
 * operator_delete when bit 0 of @p flags is set. Slot 1 of D_00490B10.
 *
 * @param flags  destructor flags; bit 0 frees the memory
 */
extern "C" void cAnimObjectNode__cAnimObjectNode(void* self, int flags)
{
    *(void***)((char*)self + 0x3C) = D_00490B10;
    cAnimNode__cAnimNode((char*)self + 0x1C, 0);
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
extern "C" void cAnimObjectNode_updateCycle(sWrapFloat_1E48* s);
extern "C" void cAnimObjectNode_updateOscillate(sBounceFloat_1EC0* s);
extern "C" void cAnimObjectNode_updateOnce(sClampFloat_1F38* s);
extern "C" int func_0034EBA0(void* p);

struct sVEntry_00341D48 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Advances an animated object node by one step and expires it once a play-once animation has reached its end.
 *
 * Resets +0x24 and +0x28 (the stepped-from and stepped-to frames) to 1e10. When atLimit (+0x8) is already set it
 * returns 1 at once. While active (+0x4) and the start delay (+0xC) is not negative, it counts the delay down, or once
 * it is 0 records the frame (+0x1C) in +0x24 and steps it by the play type (+0x0): 2 cAnimObjectNode_updateOscillate (bounce), 1
 * cAnimObjectNode_updateCycle (wrap), otherwise cAnimObjectNode_updateOnce (clamp), returning 0 if func_0034EBA0 on the animation part fails.
 *
 * It then sets the dirty flag (bit 0 of +0x42) and copies the frame to +0x20. When the step set atLimit it calls the
 * node's slot 34 (cAnimObjectNode_expire) with 1 and returns 0; otherwise it returns 1. Slot 15 of D_00490B10.
 *
 * @return 0 after a failed frame update or once the node expired, else 1
 */
extern "C" int cAnimObjectNode_updateNode(void* self)
{
    *(float*)((char*)self + 0x24) = 10000000000.0f;
    *(float*)((char*)self + 0x28) = 10000000000.0f;
    if (*(int*)((char*)self + 0x8) != 0) {
        return 1;
    }
    if (*(int*)((char*)self + 0x4) != 0 && *(int*)((char*)self + 0xC) >= 0) {
        if (*(int*)((char*)self + 0xC) > 0) {
            *(int*)((char*)self + 0xC) -= 1;
        } else {
            *(float*)((char*)self + 0x24) = *(float*)((char*)self + 0x1C);
            switch (*(short*)self) {
            case 2:
                cAnimObjectNode_updateOscillate((sBounceFloat_1EC0*)self);
                break;
            case 1:
                cAnimObjectNode_updateCycle((sWrapFloat_1E48*)self);
                break;
            default:
                cAnimObjectNode_updateOnce((sClampFloat_1F38*)self);
                break;
            }
            if (func_0034EBA0((char*)self + 0x1C) == 0) {
                return 0;
            }
        }
    }
    char* obj = (char*)self + 0x30;
    *(unsigned short*)((char*)self + 0x42) |= 1;
    *(float*)((char*)self + 0x20) = *(float*)((char*)self + 0x1C);
    if (*(int*)((char*)self + 0x8) == 0) {
        return 1;
    }
    sVEntry_00341D48* vt = *(sVEntry_00341D48**)((char*)self + 0x3C);
    vt[34].fn(obj + vt[34].delta, 1);
    return 0;
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
 * @param wrapValue  the animated object node
 */
extern "C" void cAnimObjectNode_updateCycle(sWrapFloat_1E48* wrapValue)
{
    if (wrapValue->speed >= 0.0f) {
        wrapValue->value += wrapValue->speed;
        wrapValue->last = wrapValue->value;
        if (wrapValue->value > wrapValue->max) {
            wrapValue->value = wrapValue->min + (wrapValue->value - wrapValue->max);
        }
    } else {
        wrapValue->value += wrapValue->speed;
        wrapValue->last = wrapValue->value;
        if (wrapValue->value < wrapValue->min) {
            wrapValue->value = wrapValue->max - (wrapValue->min - wrapValue->value);
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
 * @param bounceValue  the animated object node
 */
extern "C" void cAnimObjectNode_updateOscillate(sBounceFloat_1EC0* bounceValue)
{
    if (bounceValue->speed >= 0.0f) {
        bounceValue->value += bounceValue->speed;
        bounceValue->last = bounceValue->value;
        if (bounceValue->value > bounceValue->max) {
            bounceValue->speed = -bounceValue->speed;
            bounceValue->value = bounceValue->max - (bounceValue->value - bounceValue->max);
        }
    } else {
        bounceValue->value += bounceValue->speed;
        bounceValue->last = bounceValue->value;
        if (bounceValue->value < bounceValue->min) {
            bounceValue->speed = -bounceValue->speed;
            bounceValue->value = bounceValue->min + (bounceValue->min - bounceValue->value);
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
 * @param clampValue  the animated object node
 */
extern "C" void cAnimObjectNode_updateOnce(sClampFloat_1F38* clampValue)
{
    if (clampValue->speed >= 0.0f) {
        if (clampValue->value == clampValue->max) {
            clampValue->atLimit = 1;
        }
        clampValue->value += clampValue->speed;
        clampValue->last = clampValue->value;
        if (clampValue->value > clampValue->max) {
            clampValue->value = clampValue->max;
        }
    } else {
        if (clampValue->value == clampValue->min) {
            clampValue->atLimit = 1;
        }
        clampValue->value += clampValue->speed;
        clampValue->last = clampValue->value;
        if (clampValue->value < clampValue->min) {
            clampValue->value = clampValue->min;
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
extern "C" void cAnimObjectNode_expire(void* self, int runscript)
{
    if (runscript != 0) {
        cInstanceNode_runExpireScript((char*)self + 0x30);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimObjectNode_signal);
#ifdef SKIP_ASM
extern char* cAppMan_TheAppMan;
extern "C" void cMoveNode_signal(void* p);

/**
 * Handles a signal sent to an animated object node: delay, frame, speed, range and play-type changes.
 *
 * Values are in 30 fps frames unless noted. Signal 100 sets the start delay (+0xC) to @p msgValue seconds times the
 * frame rate; 101 sets the frame (+0x1C) when the new value lies within [min, max]; 102 sets the per-frame speed
 * (+0x10, divided by the frame rate); 103 sets max (+0x18) and 104 min (+0x14), each limited to [0, the animation
 * length] (the instance at +0x48, its info at +0x80, +0x14) and pulling the frame inside; 105 sets the play type (+0x0)
 * to 0 (play once). Any other signal goes to cMoveNode_signal on the move-node part (+0x30). Slot 36 of D_00490B10,
 * where the teeter vtable has cMoveNode_signal.
 *
 * @param msg  the signal number
 * @param msgValue  its argument
 */
extern "C" void cAnimObjectNode_signal(sClampFloat_1F38* self, int msg, float msgValue)
{
    if (msg == 100) {
        *(int*)((char*)self + 0xC) = (int)(msgValue * (float)*(int*)(cAppMan_TheAppMan + 0x10));
    } else if (msg == 101) {
        msgValue = msgValue * 0.03333333507180214f;
        if (self->min <= msgValue && msgValue <= self->max) {
            self->value = msgValue;
        }
    } else if (msg == 102) {
        msgValue = msgValue * 0.03333333507180214f;
        self->speed = msgValue / (float)*(int*)(cAppMan_TheAppMan + 0x10);
    } else if (msg == 103) {
        msgValue = msgValue * 0.03333333507180214f;
        float lim = *(float*)(*(char**)(*(char**)((char*)self + 0x48) + 0x80) + 0x14);
        if (lim < msgValue) {
            msgValue = lim;
        }
        if (msgValue < 0.0f) {
            msgValue = 0.0f;
        }
        self->max = msgValue;
        if (msgValue < self->value) {
            self->value = msgValue;
        }
    } else if (msg == 104) {
        msgValue = msgValue * 0.03333333507180214f;
        float lim = *(float*)(*(char**)(*(char**)((char*)self + 0x48) + 0x80) + 0x14);
        if (lim < msgValue) {
            msgValue = lim;
        }
        if (msgValue < 0.0f) {
            msgValue = 0.0f;
        }
        self->min = msgValue;
        if (self->value < msgValue) {
            self->value = msgValue;
        }
    } else if (msg == 105) {
        *(short*)self = 0;
    } else {
        cMoveNode_signal((char*)self + 0x30);
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
 * delay, speed, range, frame) with the frame's write virtual (slot 1). Slot 12 of D_00490B10.
 *
 * @param stream  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
extern "C" void cAnimObjectNode_writeToReplayFrame(void* self, void* stream)
{
    cAnimNode_writeToReplayFrame((char*)self + 0x1C, stream);
    sVEntry_cAnimObjectNode_writeToReplayFrame* vt = *(sVEntry_cAnimObjectNode_writeToReplayFrame**)stream;
    vt[1].fn((char*)stream + vt[1].delta, self, 0x1C);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_cAnimTeeterNode1);
#ifdef SKIP_ASM
extern char* cAppMan_TheAppMan;
extern void* D_004908F8[];
extern "C" void cAnimNode_cAnimNode(void* p, int a1, int a2, void* obj, int a4);
extern "C" float cAnimFrame_getStartTime(void* inst);

struct sSphCtl_003421A0 {
    int f00;            // 0x00
    int f04;            // 0x04
    float f08;          // 0x08
    float f0C;          // 0x0C
    float f10;          // 0x10
    float f14;          // 0x14
    float f18;          // 0x18
    float f1C;          // 0x1C
    float f20;          // 0x20
    float f24;          // 0x24
    int f28;            // 0x28
    int f2C;            // 0x2C
    char pad30[0x20];
    void** vt;          // 0x50
    char pad54[2];
    unsigned short flags; // 0x56
    char pad58[0x20];
    void* inst;         // 0x78
};

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

static inline void setRateMax_003421A0(sFloat_003421A0* dst, float v, char* info)
{
    float r;
    if (v < 0.0f) {
        r = *(float*)(info + 0x14);
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
 * D_004908F8 at +0x50. From @p def (On Tour tMakeAnimTeeterData) it sets min (+0x10) from FromTime (+0x20, a 30 fps
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
extern "C" void* cAnimTeeterNode_cAnimTeeterNode1(sSphCtl_003421A0* self, int bucket, char* obj, char* def)
{
    cAnimNode_cAnimNode((char*)self + 0x30, bucket, 4, obj, 0);
    self->vt = D_004908F8;
    float animStart = 0.0f;
    char* info = *(char**)(obj + 0x80);
    if (self->inst != 0) {
        animStart = cAnimFrame_getStartTime(self->inst);
    }
    setRate_003421A0((sFloat_003421A0*)&self->f10, *(float*)(def + 0x20), animStart);
    setRateMax_003421A0((sFloat_003421A0*)&self->f14, *(float*)(def + 0x24), info);
    self->f0C = *(float*)(def + 0x8);
    self->f08 = *(float*)(def + 0x4) * 9.999999974752427e-07f;
    self->f18 = *(float*)(def + 0xC) * 0.03333333507180214f / (float)*(int*)(cAppMan_TheAppMan + 0x10);
    self->f2C = *(int*)(def + 0x1C);
    self->f24 = *(float*)(def + 0x14) * 0.03333333507180214f;
    self->f1C = *(float*)(def + 0x18);
    self->f20 = *(float*)(def + 0x10);
    self->flags |= 1;
    self->f00 = 0;
    self->f04 = 0;
    self->f28 = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_cAnimTeeterNode);
#ifdef SKIP_ASM
struct sVEntry003422E8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* cAnimNode_cAnimNode1(void* p, int a1, void* stream);
extern void* D_004908F8[];

/**
 * Constructs an animated teeter node from a replay frame.
 *
 * Builds the animation node base at +0x30 with cAnimNode_cAnimNode1 (its cMoveNode part at +0x44), stores this
 * class's vtable D_004908F8 at +0x50, reads the 0x30-byte teeter state into +0x0 from @p stream (read
 * virtual, slot 2) and sets the root-transform-dirty flag (bit 0 of Flags, +0x56).
 *
 * @param bucket  bucket list the node joins, passed on to the base constructor
 * @param stream  the replay frame to read from (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cAnimTeeterNode_cAnimTeeterNode(void* self, int bucket, sVEntry003422E8** stream)
{
    cAnimNode_cAnimNode1((char*)self + 0x30, bucket, stream);
    *(void***)((char*)self + 0x50) = D_004908F8;
    (*stream)[2].fn((char*)stream + (*stream)[2].delta, self, 0x30);
    *(unsigned short*)((char*)self + 0x56) |= 1;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_updateNode);
#ifdef SKIP_ASM
extern "C" float cObjectInterface_getUpdateTime();
extern "C" int func_0034EBA0(void* p);

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
 * It then clears the torque, copies the frame to +0x34 and runs func_0034EBA0 on the animation part; on success it sets
 * the dirty flag (bit 0 of +0x56). Slot 15 of D_004908F8.
 *
 * @param spring  the teeter node
 * @return func_0034EBA0's result: 1 after a successful frame update, else 0
 */
extern "C" int cAnimTeeterNode_updateNode(sSphCtl_00342358* spring)
{
    spring->prev = spring->pos;
    float dt = cObjectInterface_getUpdateTime();
    float force = spring->gain * spring->force;
    if (spring->gain < 0.0f) {
        force = -force;
    }
    float vel = spring->vel;
    float maxStep = spring->maxStep;
    float step = vel * dt;
    float acc = -spring->stiffness * (spring->pos - spring->rest);
    acc += force;
    acc += -spring->damping * vel;
    spring->step = step;
    if (maxStep < absf_00342358(step)) {
        if (step < 0.0f) {
            spring->step = -maxStep;
        } else {
            spring->step = maxStep;
        }
    }
    float pos = spring->pos + spring->step;
    float nv = spring->vel + acc * dt;
    spring->pos = pos;
    spring->pos3C = pos;
    spring->vel = nv;
    if (pos < spring->min) {
        spring->pos = spring->min;
        if (absf_00342358(nv) > 0.1f) {
            spring->vel = nv * spring->bounce;
        } else {
            spring->step = 0.0f;
            spring->vel = 0.0f;
        }
    } else if (spring->max < pos) {
        spring->pos = spring->max;
        if (absf_00342358(nv) > 0.1f) {
            spring->vel = nv * spring->bounce;
        } else {
            spring->step = 0.0f;
            spring->vel = 0.0f;
        }
    }
    spring->force = 0.0f;
    spring->pos34 = spring->pos;
    if (func_0034EBA0(&spring->pos) == 0) {
        return 0;
    }
    spring->flags |= 1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_getAnimSpeed);
#ifdef SKIP_ASM
/**
 * Returns the teeter's animation speed: its current step, or 0 while it rests against an end of its range.
 *
 * Returns 0 when the frame (+0x30) is at or below min (+0x10) with a negative step (+0x4), or at or above max (+0x14)
 * with a positive one. Last slot of D_004908F8, where D_00490B10 has cAnimObjectNode_getAnimSpeed.
 *
 * @return the step per update, or 0
 */
extern "C" float cAnimTeeterNode_getAnimSpeed(void* self)
{
    if (*(float*)((char*)self + 0x30) <= *(float*)((char*)self + 0x10) && *(float*)((char*)self + 0x4) < 0.0f) {
        return 0.0f;
    }
    if (*(float*)((char*)self + 0x30) >= *(float*)((char*)self + 0x14) && *(float*)((char*)self + 0x4) > 0.0f) {
        return 0.0f;
    }
    return *(float*)((char*)self + 0x4);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cAnimTeeterNode_applyForce);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// ---- Shared declarations: one type per real thing, used by the functions below ----

// g++ 2.95 vtable entry with an untyped function slot (cast at the call).
struct sVec4_342538 {
    float x, y, z, w;
    sVec4_342538() {}
} __attribute__((aligned(16)));

typedef char* (*GetMat_342538)(void*, int);

struct sBodyTbl_342538 { char pad00[8]; char* ents; };
struct sBodyInf_342538 { char pad00[0x80]; sBodyTbl_342538* tbl; };
struct sSub_342538 { char pad00[0x18]; sBodyInf_342538* inf; };

struct sSelf_342538 {
    float sum;                      // 0x00
    char pad04[4];
    float dir;                      // 0x08
    char pad0C[0x20];
    int mask;                       // 0x2C
    char pad30[0x2C];
    sBodyInf_342538* inf;            // 0x5C
};

struct sObj_342538 {
    sVec4_342538 pos;               // 0x00
    char pad10[0x4C];
    int idx;                        // 0x5C
};

extern sVec4_342538 D_004FF150_342538[] __asm__("t4vect_YAxis");

// PORT: PS2-only VU0 inline asm (vector length via vsqrt).
static inline float vu0Length_342538(const sVec4_342538& v)
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
static inline void vu0Sub_342538(sVec4_342538& r, const sVec4_342538& a, const sVec4_342538& b)
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
static inline void vu0Scale_342538(sVec4_342538& r, const sVec4_342538& v, float s)
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
static inline void vu0Xform_342538(sVec4_342538& r, const sVec4_342538& v, const char* m)
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
static inline void vu0Cross_342538(sVec4_342538& r, const sVec4_342538& a, const sVec4_342538& b)
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
static inline float vu0Dot_342538(const sVec4_342538& a, const sVec4_342538& b)
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
 * Only for a hit on a mesh (@p intInfo +0x5C >= 0) whose entry in the instance's mesh table (instance info +0x80, table +0x8,
 * 16-byte entries, word +0x8) is non-zero, and only when |@p force| > 100. It fetches the mesh's animation matrix (slot 29
 * of the move-node part at +0x44, cAnimNode_getAnimMat), takes the lever arm from the matrix's translation (+0x30) to
 * the hit position (@p intInfo +0x0), negates it when TorqueScale (+0x8) is negative and again when the mesh's bit in
 * AnimSign (+0x2C) is set, and adds dot(matrix * Y axis, cross(arm, force)) to the torque (+0x0).
 *
 * cAnimTeeterNode's update (cAnimTeeterNode_updateNode) consumes the torque. Slot of D_004908F8 where D_00490B10 has
 * cMoveNode_applyForce.
 *
 * @param intInfo  the hit (On Tour tIntersectInfo: position at +0x0, mesh index at +0x5C)
 * @param force  the force vector
 */
extern "C" void cAnimTeeterNode_applyForce(sSelf_342538* self, sObj_342538* intInfo, sVec4_342538* force)
{
    int idx = intInfo->idx;
    if (idx >= 0) {
        sSub_342538* sub = (sSub_342538*)((char*)self + 0x44);
        char* ents = sub->inf->tbl->ents;
        if (*(int*)(ents + idx * 16 + 8) != 0) {
            sWsVtSlot* vt = *(sWsVtSlot**)((char*)self + 0x50);
            char* animMat = ((GetMat_342538)vt[29].fn)((char*)sub + vt[29].delta, idx);
            char* pivotMat = animMat;
            if (vu0Length_342538(*force) > 100.0f) {
                sVec4_342538 leverArm;
                sVec4_342538 a;
                sVec4_342538 vuResult;
                sVec4_342538 torqueVec;
                a = *(sVec4_342538*)(animMat + 0x30);
                vu0Sub_342538(vuResult, intInfo->pos, a);
                leverArm = vuResult;
                if (self->dir < 0.0f) {
                    vu0Scale_342538(vuResult, leverArm, -1.0f);
                    a = vuResult;
                    leverArm = vuResult;
                }
                if ((self->mask >> intInfo->idx) & 1) {
                    vu0Scale_342538(vuResult, leverArm, -1.0f);
                    a = vuResult;
                    leverArm = vuResult;
                }
                vu0Xform_342538(vuResult, D_004FF150_342538[0], pivotMat);
                a = vuResult;
                vu0Cross_342538(torqueVec, leverArm, *force);
                vuResult = torqueVec;
                self->sum += vu0Dot_342538(a, vuResult);
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
 * write virtual (slot 1). Slot 12 of D_004908F8.
 *
 * @param stream  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
extern "C" void cAnimTeeterNode_writeToReplayFrame(void* self, void* stream)
{
    cAnimNode_writeToReplayFrame((char*)self + 0x30, stream);
    sVEntry_cAnimTeeterNode_writeToReplayFrame* vt = *(sVEntry_cAnimTeeterNode_writeToReplayFrame**)stream;
    vt[1].fn((char*)stream + vt[1].delta, self, 0x30);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_cBoostModifier);
#ifdef SKIP_ASM
struct sVec4_342768 {
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sSphDef_342768 {
    int f00;
    short type;
    short pad06;
    float time;
    float f0C;
    float f10;
    float x, y, z;
};
extern void* cBoostModifier_vtbl[];
extern char* cAppMan_TheAppMan;

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
extern "C" void* cBoostModifier_cBoostModifier(cBoostModifier* self, sSphDef_342768* def, int instRecord)
{
    self->Active = 1;
    self->mVtbl = cBoostModifier_vtbl;
    self->Instance = (tInstanceRecord*)instRecord;
    int type = def->type;
    self->Type = type;
    int boostFrames = (int)(def->time * (float)*(int*)(cAppMan_TheAppMan + 0x10));
    self->BoostTime = boostFrames;
    sVec4_342768 boostDir;
    boostDir.x = def->x;
    boostDir.y = def->y;
    boostDir.z = def->z;
    boostDir.w = 0.0f;
    (*(sVec4_342768*)&self->Boost) = boostDir;
    self->MaxSpeed = def->f10 * 100.0f;
    self->Drag = def->f0C;
    self->Time = type == 1 ? boostFrames : 0;
    return (self);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_cBoostModifier1);
#ifdef SKIP_ASM
struct sVEntry00342808 {
    short delta;
    short index;
    void* fn;
};

typedef void (*ReadFn00342808)(void*, void*, int);
typedef int (*ReadIntFn00342808)(void*);

extern void* cBoostModifier_vtbl[];

/**
 * Constructs a boost modifier from a replay frame.
 *
 * Sets Active, stores cBoostModifier_vtbl, reads the 0x30-byte boost state (Boost .. Type, +0x10) with the frame's read
 * virtual (slot 2) and Instance with its int-read virtual (slot 3). Called by tModifierBlock_readFromReplayFrame.
 *
 * @param stream  the replay frame to read from (On Tour cBxReplayFrame* data)
 * @return @p self
 */
extern "C" void* cBoostModifier_cBoostModifier1(cBoostModifier* self, sVEntry00342808** stream)
{
    self->Active = 1;
    self->mVtbl = cBoostModifier_vtbl;
    ((ReadFn00342808)(*stream)[2].fn)((char*)stream + (*stream)[2].delta, (char*)&self->Boost, 0x30);
    self->Instance = (tInstanceRecord*)((ReadIntFn00342808)(*stream)[3].fn)((char*)stream + (*stream)[3].delta);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_update);
#ifdef SKIP_ASM
extern "C" int func_002D1AF0();
extern "C" void** cObjectInterface_getBeing(int i);

struct sVEnt_00342880 { short delta; short index; void* fn; };
typedef void (*Fn0_00342880)(void*);
typedef void (*Fn1_00342880)(void*, void*);
typedef int (*Test_00342880)(void*, int);

/**
 * Applies the boost modifier to every being touching it and updates its active flag (+0x0).
 *
 * The flag starts as type == 0; the timer (+0x2C) counts down and keeps it set while running. For type 1, or
 * once the timer is out, each being whose slot 3 test accepts the instance at +0x40 sets the flag and is
 * slowed (slot 8, cBoostModifier_applyDrag) when the factor (+0x24) is negative, else pushed (slot 7, cBoostModifier_applyBoost).
 * boostStart (slot 9) and boostEnd (slot 10) bracket the loop.
 */
extern "C" void cBoostModifier_update(cBoostModifier* self)
{
    self->Active = 0;
    if (self->Type == 0) {
        self->Active = 1;
    }
    int timeLeft = self->Time;
    if (timeLeft > 0) {
        timeLeft--;
        self->Time = timeLeft;
        if (timeLeft > 0) {
            self->Active = 1;
        }
    }
    sVEnt_00342880* vt = (sVEnt_00342880*)self->mVtbl;
    ((Fn0_00342880)vt[9].fn)((char*)self + vt[9].delta);
    if (self->Type == 1 || self->Time <= 0) {
        for (int i = 0; i < func_002D1AF0(); i++) {
            void** obj = cObjectInterface_getBeing(i);
            sVEnt_00342880* ovt = (sVEnt_00342880*)*obj;
            if (((Test_00342880)ovt[3].fn)((char*)obj + ovt[3].delta, (int)self->Instance) != 0) {
                self->Active = 1;
                if (self->Drag < 0.0f) {
                    sVEnt_00342880* vtbl = (sVEnt_00342880*)self->mVtbl;
                    ((Fn1_00342880)vtbl[8].fn)((char*)self + vtbl[8].delta, obj);
                } else {
                    sVEnt_00342880* vtbl = (sVEnt_00342880*)self->mVtbl;
                    ((Fn1_00342880)vtbl[7].fn)((char*)self + vtbl[7].delta, obj);
                }
            }
        }
    }
    sVEnt_00342880* vtbl2 = (sVEnt_00342880*)self->mVtbl;
    ((Fn0_00342880)vtbl2[10].fn)((char*)self + vtbl2[10].delta);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_applyDrag);
#ifdef SKIP_ASM
struct sVec4_3429E0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sVec4_3429E0 vu0Scale_3429E0(const sVec4_3429E0& v, float s)
{
    sVec4_3429E0 r;
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

class cMover_3429E0 {
public:
    virtual void v01();
    virtual sVec4_3429E0* getVelocity();
    virtual void v03();
    virtual void applyBoost(sVec4_3429E0* v);
};

extern "C" float cObjectInterface_getUpdateTime();

/**
 * Applies the boost zone's drag to a rider: an impulse of velocity * (Drag + 1) * the frame time.
 *
 * Reads the velocity (slot 2) and passes the result to the being's slot 4,
 * applyBoost(const t4Vector&) in On Tour's cBeing: an added impulse.
 *
 * @param obj  the rider in the zone (On Tour cBeing* pBeing)
 */
extern "C" void cBoostModifier_applyDrag(cBoostModifier* self, cMover_3429E0* obj)
{
    sVec4_3429E0 newVelocity = vu0Scale_3429E0(vu0Scale_3429E0(*obj->getVelocity(), self->Drag + 1.0f), cObjectInterface_getUpdateTime());
    obj->applyBoost(&newVelocity);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_applyBoost);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "bx/math3d.h"

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

struct sPlane_342A88 {
    t4Vector n;
    float d;
    float k;
};

struct sSelf_342A88 {
    char pad[0x10];
    sPlane_342A88 plane;
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
        sPlane_342A88* pl = &(*(sPlane_342A88*)&self->Boost);
        setVel_342A88(obj, &vu0Scale(vu0Scale(vu0Scale(pl->n, speedDeficit), pl->k), cObjectInterface_getUpdateTime()));
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_signal);
#ifdef SKIP_ASM
/**
 * Handles a signal sent to the boost modifier: signal 0x258 (600) restarts its timer.
 *
 * Reloads the timer (+0x2C) from the duration (+0x28); other signals are ignored.
 *
 * @param signal  the signal number
 */
extern "C" void cBoostModifier_signal(cBoostModifier* self, int signal)
{
    if (signal == 600) {
        self->Time = self->BoostTime;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cBoostModifier_writeToReplayFrame);
#ifdef SKIP_ASM
extern "C" void cPhysicsModifier_writeToReplayFrame(void* self, void* stream);
struct sVEntry_cBoostModifier_writeToReplayFrame {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};
struct sVEntry1_cBoostModifier_writeToReplayFrame {
    short delta;
    short index;
    void (*fn)(void*, int);
};

/**
 * Writes the boost modifier to a replay frame: the physics-modifier part, the 0x30-byte boost state and the
 * instance.
 *
 * After cPhysicsModifier_writeToReplayFrame it writes Boost .. Type (+0x10, 0x30 bytes) with the frame's write
 * virtual (slot 1) and the Instance pointer with slot 5.
 *
 * @param stream  the replay frame to write to (On Tour cBxReplayFrame* data)
 */
extern "C" void cBoostModifier_writeToReplayFrame(cBoostModifier* self, void* stream)
{
    cPhysicsModifier_writeToReplayFrame(self, stream);
    sVEntry_cBoostModifier_writeToReplayFrame* vt = *(sVEntry_cBoostModifier_writeToReplayFrame**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)&self->Boost, 0x30);
    sVEntry1_cBoostModifier_writeToReplayFrame* vt1 = *(sVEntry1_cBoostModifier_writeToReplayFrame**)stream;
    vt1[5].fn((char*)stream + vt1[5].delta, (int)self->Instance);
}
#endif

//100%
INCLUDE_ASM("intersect/worldsphtree", cDebounceNode_cDebounceNode1);
#ifdef SKIP_ASM
extern "C" void* cMoveNode_cMoveNode1(void* self, void* a1, int type, void* a3);
extern "C" void cMoveNode_transferFromOld(void* self, void* a1);
extern void* cDebounceNode_vtbl[];

struct sNode_342C08 {
    int pad00[2];
    unsigned int flags;
};

struct sDef_342C08 {
    int f00;
    float time;
    int f08;
    int mode;
};

struct sSelf_342C08 {
    int pad00[3];
    void** vt;
    int pad10[2];
    sNode_342C08* node;
    int pad1C[4];
    int t2C;
    int t30;
};

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
extern "C" void* cDebounceNode_cDebounceNode1(sSelf_342C08* self, void* bucket, void* instance, sDef_342C08* def, void* oldNode)
{
    cMoveNode_cMoveNode1(self, bucket, 0, instance);
    self->vt = cDebounceNode_vtbl;
    self->t2C = (int)(def->time * (float)*(int*)(cAppMan_TheAppMan + 0x10));
    self->t30 = def->f08;
    cMoveNode_transferFromOld(self, oldNode);
    if (def->mode == 0) {
        self->node->flags &= 0xFFFFFFF0;
    } else if (def->mode == 1) {
        self->node->flags |= 1;
        self->node->flags = (self->node->flags & ~2u) | 4;
    } else {
        sNode_342C08* n = self->node;
        if ((n->flags & 3) == 3) {
            n->flags = (n->flags & ~2u) | 4;
        }
    }
    return self;
}
#endif

