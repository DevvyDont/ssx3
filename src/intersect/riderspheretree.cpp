#include "common.h"
#include "world/worldrecords.h"
#include "world/worldview.h"
#include "vtable.h"
#include "ai/rider.h"
#include "intersect/worldline.h"
#include "intersect/worldrider.h"
#include "intersect/worldsphere.h"
#include "intersect/spheretree.h"
#include "ui/uithing.h"
#include "bx/math3d.h"
#include "intersect/riderspheretree.h"
#include "intersect/worldinttypes.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));


//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_cRiderSphereTree__FP16cRiderSphereTree);
#ifdef SKIP_ASM
inline void* operator new[](unsigned int, void* p) { return p; }

// tSphereList with On Tour's (implicit) default constructor, which ours lacks: [O] On Tour's tSphereList lists one (its
// t4Vector has an empty constructor); [R] the constructors keep the dead array-constructor loop over sphList (li 0x13,
// li -1), which only a non-POD element type produces. Not in the header: cWorldRider embeds a cRiderSphereTree, and
// handplantcontrol's local cWorldRider would then get a constructor of its own.
struct tSphereListInit : tSphereList {
    tSphereListInit() {}
};

/**
 * Constructs a rider sphere tree with its 20 spheres and no source assigned (both IDs -1).
 *
 * @return @p self
 */
cRiderSphereTree* cRiderSphereTree_cRiderSphereTree(cRiderSphereTree* self)
{
    new (self->sphList) tSphereListInit[20];
    self->boundBone = -1;
    self->sphMask = 0xFFFFFFFF;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_cRiderSphereTree1);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cRiderSphereTree_operatorE_v(void* self) __asm__("cRiderSphereTree_operatorE");

/**
 * Copy-constructs a rider sphere tree: builds its sphere list and assigns the source tree to it.
 *
 * Runs the (empty) constructor over the 20 sphere entries and calls cRiderSphereTree_operatorE with the source, which arrives
 * untouched in $a1 (the declaration leaves it out). Nothing calls it.
 *
 * @return @p self
 */
// NOTE: the array's offset is unconstrained by the target: the empty-ctor loop is deleted
// (vec_mark), leaving only the dead `li 0x13; li -1` pair.
void* cRiderSphereTree::cRiderSphereTree1()
{
    new (sphList) tSphereListInit[20];
    cRiderSphereTree_operatorE_v(this);
    return ((void*)this);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree__cRiderSphereTree);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Destroys a rider sphere tree; the only work is freeing the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cRiderSphereTree__cRiderSphereTree(cRiderSphereTree* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_Init1);
#ifdef SKIP_ASM
/**
 * Sets a rider sphere tree up as a single sphere of the given radius with no bones (a cursor).
 *
 * Sets the sphere mask (+0x28) to all ones, the sphere count (+0x2C) to 1, the bounding radius (+0x20) and sphere 0's
 * radius (+0x40) to @p radius, the bound bone (+0x24) and sphere 0's bone (+0x44) to -1 and the pose array (+0x0) to 0.
 * Nothing calls it.
 *
 * @param radius  the sphere's radius (On Tour cursorRadius)
 */
void cRiderSphereTree::Init1(float radius)
{
    sphMask = 0xFFFFFFFF;
    listLen = 1;
    boundRad = radius;
    sphList[0].rad = radius;
    boundBone = -1;
    sphList[0].bone = -1;
    bonePoses = 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_Init);
#ifdef SKIP_ASM

/**
 * Sets up a rider's collision spheres: one per listed bone, plus a bounding sphere.
 *
 * Sets the parent to -1, stores the count @p n and, per sphere, the bone id and radius (+0x44/+0x40 in
 * 0x20-byte entries); then the bone pose array, the bound bone and its radius. The C calls the bound bone `user`
 * (On Tour _boundBone).
 *
 * @param poses   the rider's bone poses (On Tour _bonePoses)
 * @param n       number of spheres
 * @param ids     bone index of each sphere
 * @param radii   radius of each sphere
 * @param user    bone of the bounding sphere (On Tour _boundBone)
 * @param radius  radius of the bounding sphere
 */
void cRiderSphereTree::Init(tPose* poses, int n, int* ids, float* radii, int user, float radius)
{
    sphMask = 0xFFFFFFFF;
    listLen = n;
    for (int i = 0; i < listLen; i++) {
        sphList[i].bone = ids[i];
        sphList[i].rad = radii[i];
    }
    bonePoses = poses;
    boundRad = radius;
    boundBone = user;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_operatorE);
#ifdef SKIP_ASM


extern "C" void MEM_copy(void* dst, void* src, int size);

/**
 * Copies a rider sphere tree into this one.
 *
 * Copies the pose array pointer (+0x0), bounding sphere (+0x10, +0x20), bound bone (+0x24), sphere mask (+0x28), count
 * (+0x2C) and the count x 0x20 bytes of the sphere list (+0x30, MEM_copy). Called by cRiderSphereTree_cRiderSphereTree1 and
 * cWorldRider_Transform.
 *
 * @param src  the tree to copy (On Tour copy)
 * @return @p self
 */
cRiderSphereTree* cRiderSphereTree::operatorE(cRiderSphereTree* src)
{
    bonePoses = src->bonePoses;
    boundRad = src->boundRad;
    (*(&boundCen)) = (*(&src->boundCen));
    boundBone = src->boundBone;
    listLen = src->listLen;
    sphMask = ((int)src->sphMask);
    MEM_copy(((char*)&sphList[0]), ((char*)&src->sphList[0]), listLen << 5);
    return this;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_Update);
#ifdef SKIP_ASM

/**
 * Sets a rider sphere tree's spheres and bounding sphere from explicit centres and radii.
 *
 * Stores @p n as listLen, copies each centre and radius into sphList and sets the bounding sphere (boundCen,
 * boundRad). Bones are not touched. Called by cWipeoutMotion_updateBoardSphereTree.
 *
 * @param n  number of spheres (On Tour numSphere)
 * @param pos  the sphere centres (On Tour sphCenter)
 * @param radii  the sphere radii (On Tour sphRadius)
 * @param center  the bounding sphere's centre (On Tour boundCenter)
 * @param radius  its radius (On Tour boundRadius)
 */
void cRiderSphereTree::Update(int n, t4Vector* pos, float* radii, t4Vector* center, float radius)
{
    listLen = n;
    for (int i = 0; i < listLen; i++) {
        sphList[i].cen = pos[i];
        sphList[i].rad = radii[i];
    }
    boundCen = *center;
    boundRad = radius;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_Update1);
#ifdef SKIP_ASM
/**
 * Moves a rider sphere tree's spheres to the current positions of their bones.
 *
 * Each listed sphere's centre becomes the position (P) of its bone in the pose array (bonePoses), and the bounding
 * centre that of the bound bone (boundBone). The sphere mask
 * is ignored. Called by cRider_updateCollideInfo.
 *
 */
void cRiderSphereTree::Update1()
{
    for (int i = 0; i < listLen; i++) {
        sphList[i].cen = bonePoses[sphList[i].bone].P;
    }
    boundCen = bonePoses[boundBone].P;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_ApplyOffset);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
static inline void sphereVecAdd(t4Vector* dst, t4Vector* v)
{
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(*dst)
        : "m"(*dst), "m"(*v));
}

/**
 * Moves a rider sphere tree by an offset: its bounding centre and every sphere centre.
 *
 * Called by cRider_reactToCollision and cWipeoutMotion_resolveThrownBoard.
 *
 * @param node  the rider sphere tree (this)
 * @param offset  the offset to add (On Tour offset)
 */
// boundCen += *offset, then every sphere's cen += *offset
void cRiderSphereTree::ApplyOffset(t4Vector* offset)
{
    sphereVecAdd(&boundCen, offset);
    for (int i = 0; i < listLen; i++) {
        sphereVecAdd(&sphList[i].cen, offset);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_ApplyTransform);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
static inline t4Vector mtxMulVec_9B90(t4Vector* m, t4Vector* v)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
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
        ".set pop\n"
        : "=m"(r)
        : "m"(*v), "r"(m)
        : "memory");
    return r;
}

// (s, s, s, 1): the uniform scale as a vector.
struct tUniformScale {
    float x, y, z, w;
    tUniformScale(float s) { x = s; y = s; z = s; w = 1.0f; }
};

// The component-wise product a * b.
struct tScaledVector : t4Vector {
    tScaledVector(const t4Vector& a, const tUniformScale& b)
    {
        x = a.x * b.x;
        y = a.y * b.y;
        z = a.z * b.z;
        w = a.w * b.w;
    }
};

/**
 * Moves a sphere tree node and its child spheres by a matrix, then scales their positions and radii.
 *
 * @param mat  the transform (On Tour const t4Matrix& mat)
 * @param scale  uniform scale applied after the transform
 */
void cRiderSphereTree::ApplyTransform(t4Matrix* mat, float scale)
{
    tUniformScale sc(scale);
    boundCen = tScaledVector(mtxMulVec_9B90(mat->row, &boundCen), sc);
    boundRad = boundRad * scale;
    for (int i = 0; i < listLen; i++) {
        sphList[i].cen = tScaledVector(mtxMulVec_9B90(mat->row, &sphList[i].cen), sc);
        sphList[i].rad = sphList[i].rad * scale;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_GetBoundBox);
#ifdef SKIP_ASM

/**
 * Computes the bounding box of all of a rider sphere tree's spheres.
 *
 * Starts the box at the bounding centre (boundCen) and grows it by every listed sphere's centre +/- radius, ignoring
 * the sphere mask (cRiderSphereTree_GetMaskBoundBox honours it). Nothing calls it.
 *
 * @param box  receives the box: min at +0x0, max at +0x10 (On Tour bb)
 */
// Bounding box of all child spheres, seeded with the node's own center.
void cRiderSphereTree::GetBoundBox(tBoundBox* box)
{
    box->MinPoint = box->MaxPoint = boundCen;
    for (int i = 0; i < listLen; i++) {
        tSphereList* child = &sphList[i];
        float childRadius = child->rad;
        float bound;
        bound = child->cen.x - childRadius;
        if (bound < box->MinPoint.x)
            box->MinPoint.x = bound;
        bound = child->cen.y - childRadius;
        if (bound < box->MinPoint.y)
            box->MinPoint.y = bound;
        bound = child->cen.z - childRadius;
        if (bound < box->MinPoint.z)
            box->MinPoint.z = bound;
        bound = child->cen.x + childRadius;
        if (box->MaxPoint.x < bound)
            box->MaxPoint.x = bound;
        bound = child->cen.y + childRadius;
        if (box->MaxPoint.y < bound)
            box->MaxPoint.y = bound;
        bound = child->cen.z + childRadius;
        if (box->MaxPoint.z < bound)
            box->MaxPoint.z = bound;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_GetMaskBoundBox);
#ifdef SKIP_ASM
struct sSphereV4_9DC8 {
    float x, y, z, w;
    sSphereV4_9DC8() {}
    sSphereV4_9DC8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sSphereKid_9DC8 {
    sSphereV4_9DC8 pos;     // 0x00
    float radius;           // 0x10
    int pad[3];
};

struct sSphereBox_9DC8 {
    char pad_0x00[0x10];
    sSphereV4_9DC8 center;  // 0x10
    float radius;           // 0x20
    int pad_0x24;
    unsigned int mask;      // 0x28
    int count;              // 0x2C
    sSphereKid_9DC8 kids[1]; // 0x30
};

// PORT: PS2-only VU0 inline asm (a - b).
static inline sSphereV4_9DC8 Sub_9DC8(const sSphereV4_9DC8& a, const sSphereV4_9DC8& b)
{
    sSphereV4_9DC8 r;
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
static inline sSphereV4_9DC8 Add_9DC8(const sSphereV4_9DC8& a, const sSphereV4_9DC8& b)
{
    sSphereV4_9DC8 r;
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

struct sVec4_2F650;

/**
 * Computes the bounding box of the rider spheres selected by the tree's mask.
 *
 * Starts from an empty box (min 1e10, max -1e10). With no bits in sphMask (+0x28) the box is the bounding sphere's
 * (boundCen +0x10, boundRad +0x20); otherwise it grows the box by each masked sphere of sphList (+0x30, listLen +0x2C
 * entries of 0x20 bytes) as centre plus and minus radius.
 *
 * @param mnp   out: the box minimum
 * @param mxp   out: the box maximum
 */
void cRiderSphereTree::GetMaskBoundBox(t4Vector* mnp, t4Vector* mxp)
{
    sSphereBox_9DC8* node = (sSphereBox_9DC8*)this;
    sSphereV4_9DC8* mn = (sSphereV4_9DC8*)mnp;
    sSphereV4_9DC8* mx = (sSphereV4_9DC8*)mxp;
    *mn = sSphereV4_9DC8(10000000000.0f, 10000000000.0f, 10000000000.0f, 1.0f);
    *mx = sSphereV4_9DC8(-10000000000.0f, -10000000000.0f, -10000000000.0f, 1.0f);
    if (node->mask == 0) {
        *mn = Sub_9DC8(node->center, sSphereV4_9DC8(node->radius, node->radius, node->radius, 0.0f));
        *mx = Add_9DC8(node->center, sSphereV4_9DC8(node->radius, node->radius, node->radius, 0.0f));
        return;
    }
    for (int i = 0; i < node->count; i++) {
        if (node->mask & (1 << i)) {
            sSphereV4_9DC8 radVect(node->kids[i].radius, node->kids[i].radius, node->kids[i].radius, 1.0f);
            sSphereV4_9DC8 corner = Sub_9DC8(node->kids[i].pos, radVect);
            if (corner.x < mn->x) mn->x = corner.x;
            if (corner.y < mn->y) mn->y = corner.y;
            if (corner.z < mn->z) mn->z = corner.z;
            corner = Add_9DC8(node->kids[i].pos, radVect);
            if (corner.x > mx->x) mx->x = corner.x;
            if (corner.y > mx->y) mx->y = corner.y;
            if (corner.z > mx->z) mx->z = corner.z;
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_IsRiderCollision);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_9F98(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_9F98(const t4Vector& v, float s)
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
static inline float Dot_9F98(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v / |v|).
static inline t4Vector Normalize_9F98(const t4Vector& v)
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

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_9F98(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

/**
 * Tests two rider sphere trees against each other and returns the first overlapping pair of spheres.
 *
 * Returns 0 when the bounding spheres are apart. Otherwise each sphere of @p nodeA that reaches @p nodeB's bounding
 * sphere is tested against each of @p nodeB's spheres; the first overlapping pair sets *@p outA = n x overlap (n the
 * unit vector from A's sphere to B's, overlap = rA + rB - distance) and *@p outB = A's centre - n x (rA - overlap), and
 * returns 1. That contact point lies on the side of A's sphere away from B, apparently a sign slip (SphereTree_isSphereCollision's
 * equivalent moves towards the other body). Sphere masks are ignored and the first hit wins, not the deepest. Called by
 * cRider_checkOpponentCollisions.
 *
 * @param nodeB  the other rider's tree (On Tour oppBoarder)
 * @param outA  out: the overlap vector (On Tour overlap)
 * @param outB  out: the contact point (On Tour loc)
 * @return true on contact, else false
 */
bool cRiderSphereTree::IsRiderCollision(cRiderSphereTree* nodeB, t4Vector* outA, t4Vector* outB)
{
    t4Vector delta;
    float sum;
    float ss;
    float dd;
    sum = nodeB->boundRad + boundRad;
    delta = Sub_9F98(nodeB->boundCen, boundCen);
    ss = sum * sum;
    dd = Dot_9F98(delta, delta);
    if (ss < dd)
        return false;
    for (int i = 0; i < listLen; i++) {
        sum = nodeB->boundRad + sphList[i].rad;
        delta = Sub_9F98(nodeB->boundCen, sphList[i].cen);
        ss = sum * sum;
        dd = Dot_9F98(delta, delta);
        if (ss < dd)
            continue;
        for (int j = 0; j < nodeB->listLen; j++) {
            sum = nodeB->sphList[j].rad + sphList[i].rad;
            delta = Sub_9F98(nodeB->sphList[j].cen, sphList[i].cen);
            ss = sum * sum;
            dd = Dot_9F98(delta, delta);
            if (!(ss < dd)) {
                t4Vector n = Normalize_9F98(delta);
                float overlap = sum - Sqrt_9F98(dd);
                *outA = Scale_9F98(n, overlap);
                *outB = Sub_9F98(sphList[i].cen, Scale_9F98(n, sphList[i].rad - overlap));
                return true;
            }
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_IsTriCollision);
#ifdef SKIP_ASM
typedef t4Vector sV_A1C0;

struct sKid_A1C0 {
    sV_A1C0 pos;        // 0x00
    float radius;       // 0x10
    int pad[3];
};

struct sNode_A1C0 {
    char pad_0x00[0x10];
    sV_A1C0 center;     // 0x10
    float radius;       // 0x20
    int pad_0x24;
    unsigned int mask;  // 0x28
    int count;          // 0x2C
    sKid_A1C0 kids[1];  // 0x30
};

// PORT: the unit declares gIsPointInsideTri with four pointer args; callers also pass a float tolerance in $f12.
extern "C" bool gIsPointInsideTri_eps(t4Vector* a, t4Vector* b, t4Vector* c, t4Vector* d,
        float eps) __asm__("gIsPointInsideTri");

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV_A1C0 Sub_A1C0(const sV_A1C0& a, const sV_A1C0& b)
{
    sV_A1C0 r;
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline sV_A1C0 Scale_A1C0(const sV_A1C0& v, float s)
{
    sV_A1C0 r;
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
static inline float Dot_A1C0(const sV_A1C0& a, const sV_A1C0& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_A1C0(const sV_A1C0& v)
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

// PORT: PS2-only sqrt.s.
static inline float Sqrt_A1C0(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// Does the segment from the point v is measured from along e come within sqrt(rr)?
static inline int SegHit_A1C0(const sV_A1C0& v, float d2, const sV_A1C0& e, float rr)
{
    float len = Length_A1C0(e);
    float t = Dot_A1C0(v, e) / len;
    if (t < 0.0f) {
        return 0;
    }
    float h = d2 - t * t;
    if (rr < h) {
        return 0;
    }
    if (len < t - Sqrt_A1C0(rr - h)) {
        return 0;
    }
    return 1;
}

static inline int Touch_A1C0(const sV_A1C0& c, float r, sV_A1C0* p, sV_A1C0* a, sV_A1C0* b, sV_A1C0* q)
{
    if (gIsPointInsideTri_eps(p, a, b, q, 0.0001f) != 0) {
        return 1;
    }
    float rr = r * r;
    sV_A1C0 v = Sub_A1C0(c, *p);
    float d2 = Dot_A1C0(v, v);
    if (d2 < rr) {
        return 1;
    }
    if (SegHit_A1C0(v, d2, Sub_A1C0(*a, *p), rr)) {
        return 1;
    }
    if (SegHit_A1C0(v, d2, Sub_A1C0(*b, *p), rr)) {
        return 1;
    }
    sV_A1C0 v2 = Sub_A1C0(c, *a);
    if (SegHit_A1C0(v2, Dot_A1C0(v2, v2), Sub_A1C0(*b, *a), rr)) {
        return 1;
    }
    return 0;
}

// PORT: the unit declares it later as (void*, int, int, int, int, t4Vector*, int); defined under its own name bound to the symbol.
extern "C" bool cRiderSphereTree_IsTriCollision_impl(sNode_A1C0* self, sV_A1C0* p, sV_A1C0* a, sV_A1C0* b, sV_A1C0* n,
        sV_A1C0* outPush, sV_A1C0* outPoint) __asm__("cRiderSphereTree_IsTriCollision");
/**
 * Tests the rider's collision spheres against a triangle (one-sided).
 *
 * Returns 0 when the bounding sphere's centre is farther from the triangle's plane than its radius; otherwise
 * checks the projected centre and each sphere against the triangle (gIsPointInsideTri) and returns the deepest
 * penetration in @p outPush and its contact point in @p outPoint.
 *
 * @param point1         first triangle corner
 * @param point2         second triangle corner
 * @param point3         third triangle corner
 * @param n         the triangle's unit normal
 * @param outPush   out: how far to push the rider out (On Tour overlap)
 * @param outPoint  out: the contact point (On Tour loc)
 * @return true on contact
 */
extern "C" bool cRiderSphereTree_IsTriCollision_impl(sNode_A1C0* self, sV_A1C0* point1, sV_A1C0* point2,
        sV_A1C0* point3, sV_A1C0* n, sV_A1C0* outPush, sV_A1C0* outPoint)
{
    float dist = Dot_A1C0(*n, Sub_A1C0(self->center, *point1));
    float sphereRadius = self->radius;
    if (sphereRadius < __builtin_fabsf(dist)) {
        return false;
    }
    sV_A1C0 contactPoint = Sub_A1C0(self->center, Scale_A1C0(*n, dist));
    if (!Touch_A1C0(self->center, sphereRadius, point1, point2, point3, &contactPoint)) {
        return false;
    }
    if (self->mask == 0) {
        *outPush = Scale_A1C0(*n, sphereRadius - dist);
        *outPoint = contactPoint;
        return true;
    }
    float best = 10000000000.0f;
    for (int i = 0; i < self->count; i++) {
        if (self->mask & (1 << i)) {
            dist = Dot_A1C0(*n, Sub_A1C0(self->kids[i].pos, *point1));
            sphereRadius = self->kids[i].radius;
            if (__builtin_fabsf(dist) < sphereRadius && dist < best) {
                contactPoint = Sub_A1C0(self->kids[i].pos, Scale_A1C0(*n, dist));
                if (Touch_A1C0(self->kids[i].pos, sphereRadius, point1, point2, point3, &contactPoint)) {
                    *outPush = Scale_A1C0(*n, sphereRadius - dist);
                    *outPoint = contactPoint;
                    best = dist;
                }
            }
        }
    }
    return best != 10000000000.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_Is2SidedTriCollision);
#ifdef SKIP_ASM
typedef t4Vector sV_AA28;

struct sKid_AA28 {
    sV_AA28 pos;        // 0x00
    float radius;       // 0x10
    int pad[3];
};

struct sNode_AA28 {
    char pad_0x00[0x10];
    sV_AA28 center;     // 0x10
    float radius;       // 0x20
    int pad_0x24;
    unsigned int mask;  // 0x28
    int count;          // 0x2C
    sKid_AA28 kids[1];  // 0x30
};

// PORT: the unit declares gIsPointInsideTri with four pointer args; callers also pass a float tolerance in $f12.
extern "C" bool gIsPointInsideTri_eps(t4Vector* a, t4Vector* b, t4Vector* c, t4Vector* d,
        float eps) __asm__("gIsPointInsideTri");

// PORT: PS2-only VU0 inline asm (a - b).
static inline sV_AA28 Sub_AA28(const sV_AA28& a, const sV_AA28& b)
{
    sV_AA28 r;
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

// PORT: PS2-only VU0 inline asm (v * s).
static inline sV_AA28 Scale_AA28(const sV_AA28& v, float s)
{
    sV_AA28 r;
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
static inline float Dot_AA28(const sV_AA28& a, const sV_AA28& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_AA28(const sV_AA28& v)
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

// PORT: PS2-only sqrt.s.
static inline float Sqrt_AA28(float x)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

// Does the segment from the point v is measured from along e come within sqrt(rr)?
static inline int SegHit_AA28(const sV_AA28& v, float d2, const sV_AA28& e, float rr)
{
    float len = Length_AA28(e);
    float t = Dot_AA28(v, e) / len;
    if (t < 0.0f) {
        return 0;
    }
    float h = d2 - t * t;
    if (rr < h) {
        return 0;
    }
    if (len < t - Sqrt_AA28(rr - h)) {
        return 0;
    }
    return 1;
}

static inline int Touch_AA28(const sV_AA28& c, float r, sV_AA28* p, sV_AA28* a, sV_AA28* b, sV_AA28* q)
{
    if (gIsPointInsideTri_eps(p, a, b, q, 0.0001f) != 0) {
        return 1;
    }
    float rr = r * r;
    sV_AA28 v = Sub_AA28(c, *p);
    float d2 = Dot_AA28(v, v);
    if (d2 < rr) {
        return 1;
    }
    if (SegHit_AA28(v, d2, Sub_AA28(*a, *p), rr)) {
        return 1;
    }
    if (SegHit_AA28(v, d2, Sub_AA28(*b, *p), rr)) {
        return 1;
    }
    sV_AA28 v2 = Sub_AA28(c, *a);
    if (SegHit_AA28(v2, Dot_AA28(v2, v2), Sub_AA28(*b, *a), rr)) {
        return 1;
    }
    return 0;
}

// PORT: the unit declares it later as (void*, int, int, int, int, t4Vector*, int); defined under its own name bound to the symbol.
extern "C" bool cRiderSphereTree_Is2SidedTriCollision_impl(sNode_AA28* self, sV_AA28* p, sV_AA28* a, sV_AA28* b,
        sV_AA28* np, sV_AA28* outPush, sV_AA28* outPoint) __asm__("cRiderSphereTree_Is2SidedTriCollision");
/**
 * Tests the rider's collision spheres against a triangle from either side.
 *
 * Like cRiderSphereTree_IsTriCollision, but the normal is flipped first when the spheres are behind the triangle.
 *
 * @param point1         first triangle corner
 * @param point2         second triangle corner
 * @param point3         third triangle corner
 * @param np        the triangle's unit normal
 * @param outPush   out: how far to push the rider out (On Tour overlap)
 * @param outPoint  out: the contact point (On Tour loc)
 * @return true on contact
 */
extern "C" bool cRiderSphereTree_Is2SidedTriCollision_impl(sNode_AA28* self, sV_AA28* point1, sV_AA28* point2,
        sV_AA28* point3, sV_AA28* np, sV_AA28* outPush, sV_AA28* outPoint)
{
    sV_AA28 n = *np;
    float dist = Dot_AA28(n, Sub_AA28(self->center, *point1));
    float sphereRadius = self->radius;
    if (sphereRadius < __builtin_fabsf(dist)) {
        return false;
    }
    if (dist < 0.0f) {
        dist = -dist;
        n = Scale_AA28(n, -1.0f);
    }
    sV_AA28 contactPoint = Sub_AA28(self->center, Scale_AA28(n, dist));
    if (!Touch_AA28(self->center, sphereRadius, point1, point2, point3, &contactPoint)) {
        return false;
    }
    if (self->mask == 0) {
        *outPush = Scale_AA28(n, sphereRadius - dist);
        *outPoint = contactPoint;
        return true;
    }
    float best = 10000000000.0f;
    for (int i = 0; i < self->count; i++) {
        if (self->mask & (1 << i)) {
            dist = Dot_AA28(n, Sub_AA28(self->kids[i].pos, *point1));
            sphereRadius = self->kids[i].radius;
            if (dist < sphereRadius && dist < best) {
                contactPoint = Sub_AA28(self->kids[i].pos, Scale_AA28(n, dist));
                if (Touch_AA28(self->kids[i].pos, sphereRadius, point1, point2, point3, &contactPoint)) {
                    *outPush = Scale_AA28(n, sphereRadius - dist);
                    *outPoint = contactPoint;
                    best = dist;
                }
            }
        }
    }
    return best != 10000000000.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_IsBoxCollision);
#ifdef SKIP_ASM
extern "C" bool gIsBoxSphereInt(void* a, void* b, void* center, void* box, void* boxMin, void* boxMax, float radius);

struct sSphereHit_B2B8 {
    char pad_0x00[0x10];
    t4Vector center; // 0x10
    float radius;       // 0x20
    int pad_0x24;
    unsigned int mask;  // 0x28
    int count;          // 0x2C
};

// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_B2B8(const t4Vector& v, float s)
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

struct sSphereHit_32FAC0;

/**
 * Tests a rider sphere tree against an axis-aligned box.
 *
 * Tests the bounding sphere (+0x10, +0x20) with gIsBoxSphereInt; a miss returns 0. With an empty sphere mask (+0x28) that
 * result is final: *@p outPos = face normal x depth and *@p hit = the contact point. Otherwise, for each masked sphere
 * index it keeps the deepest result, but every iteration re-tests the bounding sphere rather than the indexed sphere
 * (+0x30 list), so the listed spheres are never looked at: likely a bug. Called by cWorldRider_IsBoxInt.
 *
 * @param boxMin  the box minimum (On Tour bMin)
 * @param boxMax  the box maximum (On Tour bMax)
 * @param outPos  out: the push-out vector (On Tour overlap)
 * @param hit  out: the contact point (On Tour loc)
 * @return true on contact, else false
 */
bool cRiderSphereTree::IsBoxCollision(t3Vector* boxMin, t3Vector* boxMax, t4Vector* outPos, tIntersectInfo* hit)
{
    sSphereHit_B2B8* self = (sSphereHit_B2B8*)this;
    t4Vector* outNormal = (t4Vector*)hit;
    t4Vector normal;
    t4Vector dir;
    float dist;
    if (gIsBoxSphereInt(boxMin, boxMax, &self->center, &normal, &dir, &dist, self->radius) == 0) {
        return false;
    }
    if (self->mask == 0) {
        *outPos = Scale_B2B8(dir, dist);
        *outNormal = normal;
        return true;
    }
    float best = -1.0f;
    for (int i = 0; i < self->count; i++) {
        if (self->mask & (1 << i)) {
            if (gIsBoxSphereInt(boxMin, boxMax, &self->center, &normal, &dir, &dist, self->radius) && best < dist) {
                *outPos = Scale_B2B8(dir, dist);
                *outNormal = normal;
                best = dist;
            }
        }
    }
    return best != -1.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032B460);
#ifdef SKIP_ASM
/**
 * Does nothing; nothing calls it or stores its address.
 */
extern "C" void func_0032B460(void) {

}
#endif
//100%
INCLUDE_ASM("intersect/riderspheretree", sphTreeRLE);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the inner run loop wrapped in a non-empty `do { ... } while (0)`, and the stand-in local `one = 1` for the `1 < minRun` test (li + slt instead of slti); plain form 94.74% (97.65% without the wrapper only, 95.24% without `one` only). Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Run-length encodes a byte buffer in the format sphTreeUnRLE decodes.
 *
 * Writes literal blocks as a count byte -n followed by n bytes and repeated runs (at least max(2, @p minRun) bytes) as
 * n - 1 followed by the byte, both at most 0x7F long, then a 0 terminator. A run that reaches the end of the input is
 * shortened by one. Looking for a run it reads one byte past the end of @p in. Nothing calls it.
 *
 * @param start  the output buffer
 * @param in  the input bytes
 * @param len  the number of input bytes
 * @param minRun  the shortest repeat to encode as a run; values below 2 mean 2
 * @return the number of bytes written, terminator included
 */
extern "C" int sphTreeRLE(unsigned char* start, unsigned char* in, int len, int minRun)
{
    unsigned char* out = start;
    unsigned char* end = in + len;
    int one = 1;
    int min = 2;
    if (one < minRun) {
        min = minRun;
    }
    while (in < end) {
        unsigned char* cursor = in;
        int run = 0;
        int lit = 0;
        unsigned char* runEnd;
        while (run == 0 && lit < 0x7F && cursor < end) {
            unsigned char runByte = *cursor;
            run = 1;
            runEnd = cursor + 1;
            if (runByte == cursor[1] && runEnd < end) {
                do {
                    do {
                        runEnd++;
                        run++;
                    } while (runByte == *runEnd && runEnd < end && run < 0x7F);
                } while (0);
            }
            if (runEnd == end && 1 < run) {
                run--;
                runEnd = end - 1;
            }
            if (run < min) {
                lit += run;
                run = 0;
                cursor = runEnd;
            }
        }
        if (0x7F < lit) {
            lit = 0x7F;
        }
        if (lit != 0) {
            MEM_fill(out++, -lit, 1);
            while (lit-- != 0) {
                *out++ = *in++;
            }
        }
        if (run != 0) {
            run--;
            MEM_fill(out++, run, 1);
            *out++ = *in++;
            in += run;
        }
    }
    MEM_fill(out++, 0, 1);
    return out - start;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", sphTreeUnRLE);
#ifdef SKIP_ASM
// Run-length decode (PackBits style) into dst: a negative count -n copies the next n bytes, a positive count n
// repeats the next byte n+1 times, 0 terminates. Returns the number of bytes written.
// PORT: the unit declares it later as void*(void*, void*); defined under its own name bound to the symbol.
extern "C" int sphTreeUnRLE_impl(char* dst, signed char* src) __asm__("sphTreeUnRLE");
/**
 * Decodes run-length encoded sphere tree data.
 *
 * Each count byte c is followed by -c literal bytes when negative, or by one byte to repeat c + 1 times when positive;
 * 0 ends the data. A null @p src gives 0. SphereTree_AllocBuf calls it to unpack compressed trees.
 *
 * @param dst  the output buffer (On Tour dest)
 * @param src  the encoded data, or 0
 * @return the number of bytes written
 */
extern "C" int sphTreeUnRLE_impl(char* dst, signed char* src)
{
    if (src == 0)
        return 0;
    char* out = dst;
    int code;
    do {
        code = *src++;
        if (code < 0) {
            int n = -code;
            do {
                *out++ = *src++;
            } while (--n);
        } else {
            int n = code + 1;
            if (code == 0)
                break;
            do {
                *out++ = *src;
            } while (--n);
            src++;
        }
    } while (code != 0);
    return out - dst;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", gIsPointInsideTri);
#ifdef SKIP_ASM
// VU0 microprogram entry (micro-memory address 0x6E0; resolved via undefined_syms_auto.txt)
extern char D_6E0[];

/**
 * Tells whether a point lies inside a triangle, using a VU0 micro-program.
 *
 * Loads the four vectors into vf10..vf13, runs the VU0 micro-program at micro address 0x6E0 and returns whether it left
 * vi2 nonzero. A tolerance some callers pass in $f12 (SphereTree_isSphereInsideTri passes 1e-4) is not read. Called by the triangle
 * tests (cRiderSphereTree_IsTriCollision and its 2-sided form, cWorldLine_IsTriInt, cWorldSphere_IsTriInt and its
 * 2-sided form) and SphereTree_isSphereInsideTri.
 *
 * @param corner1  first corner (On Tour p1)
 * @param corner2  second corner (On Tour p2)
 * @param corner3  third corner (On Tour p3)
 * @param testPoint  the point to test (On Tour cen)
 * @return true when the point is inside, else false
 */
// Loads four vectors into vf10-vf13, runs the VU0 microprogram at D_6E0 and
// returns whether it left a nonzero result in vi2.
// PORT: PS2-only VU0 microprogram call (ctc2/vcallmsr/cfc2); the PC port needs a C version
// of the microprogram.
extern "C" bool gIsPointInsideTri(t4Vector* corner1, t4Vector* corner2, t4Vector* corner3, t4Vector* testPoint)
{
    int hit;
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf10, 0x0(%1)\n"
        "lqc2      $vf11, 0x0(%2)\n"
        "lqc2      $vf12, 0x0(%3)\n"
        "lqc2      $vf13, 0x0(%4)\n"
        "ctc2.ni   %5, $vi27\n"
        "vnop\n"
        "vnop\n"
        "vcallmsr  $vi27\n"
        "cfc2.i    %0, $vi2\n"
        ".set pop\n"
        : "=r"(hit)
        : "r"(corner1), "r"(corner2), "r"(corner3), "r"(testPoint), "0"((int)D_6E0 >> 3));
    return hit != 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldIntersectType_IsPatchInt);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sCell_B6E0 {
    t4Vector n[2];   // 0x00 triangle normals (1e10 = not computed, 1e11 = degenerate)
    t4Vector lo;     // 0x20 bounds (lo.x 1e10 = not computed)
    t4Vector hi;     // 0x30
};

struct sPatch_B6E0 {
    char pad_0x00[0x10];
    t4Vector verts[10][10];  // 0x0010
    sCell_B6E0 fine[9][9];      // 0x0650
    sCell_B6E0 coarse[3][3];    // 0x1A90
};

struct sVE_B6E0 {
    short delta;
    short pad;
    union {
        int (*test)(void* self, void* a, void* b, void* c, t4Vector* n, void* d, void* e, void* f);
        void (*uv)(void* self, void* obj, float* u, float* v, void* d, void* e, void* f);
    };
};

struct sQuery_B6E0 {
    int type;           // 0x00
    int pad_0x04;
    int facing;         // 0x08
    int pad_0x0C;
    t4Vector dir;    // 0x10
    t4Vector hi;     // 0x20
    t4Vector lo;     // 0x30
    char pad_0x40[0x10];
    sVE_B6E0* vt;       // 0x50
};

struct sHitCache_B6E0 {
    void* obj;              // 0x00
    unsigned short u;       // 0x04
    unsigned short v;       // 0x06
    int tri;                // 0x08
    int type;               // 0x0C
};

// PORT: PS2-only VU0 inline asm (bounds of four points).
static inline void Bounds_B6E0(sCell_B6E0* c, const t4Vector& a, const t4Vector& b,
                               const t4Vector& d, const t4Vector& e)
{
    __asm__(
        "lqc2      $vf2, %2\n"
        "lqc2      $vf3, %3\n"
        "lqc2      $vf4, %4\n"
        "lqc2      $vf5, %5\n"
        "vmax.xyzw $vf6, $vf2, $vf3\n"
        "vmini.xyzw $vf7, $vf2, $vf3\n"
        "vmax.xyzw $vf8, $vf4, $vf5\n"
        "vmini.xyzw $vf9, $vf4, $vf5\n"
        "vmax.xyzw $vf6, $vf6, $vf8\n"
        "vmini.xyzw $vf7, $vf7, $vf9\n"
        "sqc2      $vf6, %0\n"
        "sqc2      $vf7, %1\n"
        : "=m"(c->hi), "=m"(c->lo)
        : "m"(a), "m"(b), "m"(d), "m"(e));
}

// PORT: PS2-only VU0 inline asm (unit normal of triangle a b c; returns 1/|n|).
static inline float TriNormal_B6E0(t4Vector& n, const t4Vector& a, const t4Vector& b,
                                  const t4Vector& c)
{
    union { int i; float f; } inv;
    __asm__(
        "lqc2      $vf2, %2\n"
        "lqc2      $vf3, %3\n"
        "lqc2      $vf4, %4\n"
        "vsub.xyzw $vf5, $vf2, $vf3\n"
        "vsub.xyzw $vf6, $vf3, $vf4\n"
        "vopmula.xyz ACC, $vf5, $vf6\n"
        "vopmsub.xyz $vf7, $vf6, $vf5\n"
        "vsub.w    $vf7, $vf7, $vf7\n"
        "vaddw.x   $vf8, $vf0, $vf0w\n"
        "vmul.xyzw $vf9, $vf7, $vf7\n"
        "vadday.x  ACC, $vf9, $vf9y\n"
        "vmaddaz.x ACC, $vf8, $vf9z\n"
        "vmaddw.x  $vf9, $vf8, $vf9w\n"
        "vrsqrt    Q, $vf0w, $vf9x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf7, Q\n"
        "vaddw.x   $vf10, $vf0, $vf0w\n"
        "vmulq.xyzw $vf10, $vf10, Q\n"
        "qmfc2.ni  %1, $vf10\n"
        "sqc2      $vf5, %0\n"
        : "=m"(n), "=r"(inv.i)
        : "m"(a), "m"(b), "m"(c));
    return inv.f;
}

// does the cell's bounding box overlap the query box?
static inline int Overlap_B6E0(const sCell_B6E0* c, const sQuery_B6E0* q)
{
    return q->hi.x > c->lo.x && q->lo.x < c->hi.x
        && q->hi.y > c->lo.y && q->lo.y < c->hi.y
        && q->hi.z > c->lo.z && q->lo.z < c->hi.z;
}

// the cell's k-th triangle normal, built on first use (99999997952 marks a degenerate triangle)
static inline t4Vector& CellNormal_B6E0(sCell_B6E0* cell, int k, const t4Vector& a,
                                           const t4Vector& b, const t4Vector& c)
{
    if (cell->n[k].x == 10000000000.0f) {
        if (TriNormal_B6E0(cell->n[k], a, b, c) > 10.0f)
            cell->n[k].x = 99999997952.0f;
    }
    return cell->n[k];
}

// query against a 9x9-cell terrain patch (coarse 3x3 cells for type 0): tries the cached cell first,
// then every cell whose bounds overlap the query box; returns the hit cell's uv in uvOut
extern "C" bool cWorldIntersectType_IsPatchInt_typed(sQuery_B6E0* q, void* obj, sPatch_B6E0* patch, void* a3, void* a4,
                                   void* a5, float* uvOut, sHitCache_B6E0* cache) __asm__("cWorldIntersectType_IsPatchInt");
/**
 * Intersects a world query with a tessellated terrain patch, trying the cell remembered in a hint first.
 *
 * Low precision (query +0x0 = 0) walks the 3 x 3 coarse cells (+0x1A90 of @p patch, every third vertex); otherwise the
 * 9 x 9 fine cells (+0x650). Each cell's bounds and its two triangles' normals are computed on first use (1e10 marks
 * "not yet", a normal flagged 99999997952 a degenerate triangle). A cell whose bounds overlap the query box (+0x30 min,
 * +0x20 max) has its triangles tested with the query's virtual triangle test (slot 4), skipping triangles whose normal
 * points along the reject normal (+0x10, dot > 0.8) when the reject flag (+0x8) is set.
 *
 * On a hit the uv is the cell centre (refined by virtual slot 8 at high precision, 2), and the hint remembers patch,
 * cell and triangle. When the hinted cell misses, the hint is cleared and all cells are walked. Called by cOverlapList_terrainIntersect
 * and TerrainCallback_intersectpatch once cAIFwdDiffCache_GetFwdDiff has fetched the tessellation.
 *
 * @param query  the world intersection query (this)
 * @param obj  the terrain patch record (On Tour pPatch)
 * @param patch  its tessellation (On Tour pFwdDiff)
 * @param hitPos  out: the hit position (On Tour ipos)
 * @param hitNormal  out: the hit normal (On Tour inorm)
 * @param hitParam  out: the hit parameter (On Tour param)
 * @param uvOut  out: the hit's patch u, v (On Tour uv)
 * @param cache  the hint, or 0 (On Tour tIntHint* hint)
 * @return true on a hit, else false
 */
extern "C" bool cWorldIntersectType_IsPatchInt_typed(sQuery_B6E0* query, void* obj, sPatch_B6E0* patch, void* hitPos,
        void* hitNormal,
                                   void* hitParam, float* uvOut, sHitCache_B6E0* cache)
{
    // the cached cell's row is scaled by its own step variable: the binary keeps it in a
    // second register next to step and multiplies only the row index by it
    int rowStep;
    int step;
    int n;
    int skip;
    float scale;
    float half;
    sCell_B6E0* cell;
    if (query->type == 0) {
        step = 3;
        rowStep = 3;
        skip = 21;
        n = 3;
        scale = 1.0f / 3.0f;
        half = 1.0f / 6.0f;
        cell = patch->coarse[0];
    } else {
        step = 1;
        rowStep = 1;
        skip = 1;
        n = 9;
        scale = 1.0f / 9.0f;
        half = 1.0f / 18.0f;
        cell = patch->fine[0];
    }
    if (cache != 0 && cache->obj == obj && cache->type == query->type) {
        sCell_B6E0* cachedCell;
        if (cache->type == 0)
            cachedCell = &patch->coarse[cache->u][cache->v];
        else
            cachedCell = &patch->fine[cache->u][cache->v];
        t4Vector* corner00 = &patch->verts[cache->u * rowStep][cache->v * step];
        t4Vector* corner10 = corner00 + step * 10;
        t4Vector* corner01 = corner00 + step;
        t4Vector* corner11 = corner10 + step;
        if (cachedCell->lo.x == 10000000000.0f)
            Bounds_B6E0(cachedCell, *corner00, *corner10, *corner01, *corner11);
        if (cache->tri) {
            t4Vector& triNormal0 = CellNormal_B6E0(cachedCell, 0, *corner11, *corner10, *corner01);
            if (triNormal0.x != 99999997952.0f && (!query->facing || Dot_A1C0(triNormal0, query->dir) <= 0.8f)) {
                sVE_B6E0* vt = query->vt;
                if (vt[4].test(vthis(query, vt[4]), corner11, corner10, corner01, &triNormal0, hitPos, hitNormal, hitParam)) {
                    float hitU = cache->u * scale + half;
                    float hitV = cache->v * scale + half;
                    if (query->type == kIntPrecision_High) {
                        sVE_B6E0* vt = query->vt;
                        vt[8].uv(vthis(query, vt[8]), obj, &hitU, &hitV, hitPos, hitNormal, hitParam);
                    }
                    uvOut[0] = hitU;
                    uvOut[1] = hitV;
                    return true;
                }
            }
        } else {
            t4Vector& triNormal1 = CellNormal_B6E0(cachedCell, 1, *corner01, *corner10, *corner00);
            if (triNormal1.x != 99999997952.0f && (!query->facing || Dot_A1C0(triNormal1, query->dir) <= 0.8f)) {
                sVE_B6E0* vt = query->vt;
                if (vt[4].test(vthis(query, vt[4]), corner01, corner10, corner00, &triNormal1, hitPos, hitNormal, hitParam)) {
                    float hitU = cache->u * scale + half;
                    float hitV = cache->v * scale + half;
                    if (query->type == kIntPrecision_High) {
                        sVE_B6E0* vt = query->vt;
                        vt[8].uv(vthis(query, vt[8]), obj, &hitU, &hitV, hitPos, hitNormal, hitParam);
                    }
                    uvOut[0] = hitU;
                    uvOut[1] = hitV;
                    return true;
                }
            }
        }
        cache->obj = 0;
        cache->u = 0;
        cache->v = 0;
        cache->tri = 0;
        cache->type = 0;
    }
    t4Vector* corner00 = patch->verts[0];
    for (int cellU = 0; cellU < n; cellU++) {
        t4Vector* corner10 = corner00 + step * 10;
        t4Vector* corner01 = corner00 + step;
        t4Vector* corner11 = corner10 + step;
        for (int cellV = 0; cellV < n; cellV++) {
            if (cell->lo.x == 10000000000.0f)
                Bounds_B6E0(cell, *corner00, *corner10, *corner01, *corner11);
            if (Overlap_B6E0(cell, query)) {
                t4Vector& triNormal0 = CellNormal_B6E0(cell, 0, *corner11, *corner10, *corner01);
                if (triNormal0.x != 99999997952.0f && (!query->facing || Dot_A1C0(triNormal0, query->dir) <= 0.8f)) {
                    sVE_B6E0* vt = query->vt;
                    if (vt[4].test(vthis(query, vt[4]), corner11, corner10, corner01, &triNormal0, hitPos, hitNormal, hitParam)) {
                        float hitU = cellU * scale + half;
                        float hitV = cellV * scale + half;
                        if (query->type == kIntPrecision_High) {
                            sVE_B6E0* vt = query->vt;
                            vt[8].uv(vthis(query, vt[8]), obj, &hitU, &hitV, hitPos, hitNormal, hitParam);
                        }
                        uvOut[0] = hitU;
                        uvOut[1] = hitV;
                        if (cache) {
                            cache->obj = obj;
                            cache->u = cellU;
                            cache->v = cellV;
                            cache->tri = 1;
                            cache->type = query->type;
                        }
                        return true;
                    }
                }
                t4Vector& triNormal1 = CellNormal_B6E0(cell, 1, *corner01, *corner10, *corner00);
                if (triNormal1.x != 99999997952.0f && (!query->facing || Dot_A1C0(triNormal1, query->dir) <= 0.8f)) {
                    sVE_B6E0* vt = query->vt;
                    if (vt[4].test(vthis(query, vt[4]), corner01, corner10, corner00, &triNormal1, hitPos, hitNormal, hitParam)) {
                        float hitU = cellU * scale + half;
                        float hitV = cellV * scale + half;
                        if (query->type == kIntPrecision_High) {
                            sVE_B6E0* vt = query->vt;
                            vt[8].uv(vthis(query, vt[8]), obj, &hitU, &hitV, hitPos, hitNormal, hitParam);
                        }
                        uvOut[0] = hitU;
                        uvOut[1] = hitV;
                        if (cache) {
                            cache->obj = obj;
                            cache->u = cellU;
                            cache->v = cellV;
                            cache->tri = 0;
                            cache->type = query->type;
                        }
                        return true;
                    }
                }
            }
            corner00 += step;
            corner10 += step;
            corner01 += step;
            corner11 += step;
            cell++;
        }
        corner00 += skip;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldIntersectType_IsMeshInt);
#ifdef SKIP_ASM

extern t4Vector t4vect_Zero;

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vDot_C0F8(const t4Vector& a, const t4Vector& b)
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




struct sVE_C0F8a {
    short delta;
    short index;
    int (*fn)(void*, t4Vector*, t4Vector*, t4Vector*, t4Vector*, t4Vector*, t4Vector*, float*);
};
struct sVE_C0F8b {
    short delta;
    short index;
    int (*fn)(void*, const t3Vector&, const t3Vector&);
};

struct sTriTester_C0F8 {
    char pad0[0x8];
    int useNormal;          // 0x8
    char padC[0x4];
    t4Vector dir;           // 0x10
    char pad20[0x30];
    sVE_C0F8a* vt;          // 0x50
};

/**
 * Intersects the query with a polygon mesh and records the hits.
 *
 * For meshes of fewer than 11 triangles tests each triangle through the query's virtual slot 4 (one-sided) or 5
 * (two-sided), skipping triangles whose normal points along the query direction (dot > 0.8) when the query asks;
 * larger meshes take a second path. Each hit fills a record in @p out (type 2, position, normal). The hit
 * capacity On Tour passes as num is not declared here.
 *
 * @param mesh  the polygon mesh
 * @param flag  nonzero for two-sided triangles (On Tour is2Sided)
 * @param out   out: the hit records
 * @return the number of hits
 */
extern "C" int cWorldIntersectType_IsMeshInt(sTriTester_C0F8* self, tPolyModelMesh* mesh, int flag, tIntersectInfo* out)
{
    t4Vector pos;
    t4Vector hn;
    float hitParam;
    int n = 0;
    if (mesh->NumFaces < 11) {
        for (int i = 0; i < mesh->NumFaces; i++) {
            t4Vector* nrm = &mesh->Normal[i];
            if (self->useNormal != 0 && vDot_C0F8(*nrm, self->dir) > 0.8f) {
                continue;
            }
                        t4Vector* vertices = mesh->Vertex;
            t4Vector* corner0 = &vertices[mesh->Face[i][0]];
            t4Vector* corner1 = &vertices[mesh->Face[i][1]];
            t4Vector* corner2 = &vertices[mesh->Face[i][2]];
            int isHit;
            if (flag != 0) {
                isHit = self->vt[5].fn(vthis(self, self->vt[5]), corner0, corner1, corner2, nrm, &pos, &hn, &hitParam);
            } else {
                isHit = self->vt[4].fn(vthis(self, self->vt[4]), corner0, corner1, corner2, nrm, &pos, &hn, &hitParam);
            }
            if (isHit == 0) {
                continue;
            }
            tIntersectInfo* record = &out[n];
            record->Patch = 0;
            record->HitType = 2;
            record->Face = -1;
            record->Inst = 0;
            record->Spline = 0;
            record->Mesh = -1;
            record->Seg = -1;
            record->Stype = -1;
            (record->Ivel) = t4vect_Zero;
            (record->Iangvel) = t4vect_Zero;
            record->T = 0.0f;
            record->u = 0.0f;
            record->v = 0.0f;
            (record->Ipos) = pos;
            (record->Inorm) = hn;
            record->Iparam = hitParam;
            record->Face = i;
            record->Patch = 0;
            n++;
        }
    } else {
        int nb = (mesh->NumFaces + 9) / 10;
        for (int boxIndex = 0; boxIndex < nb; boxIndex++) {
            tPolyBoundBox* box = &mesh->Boxes[boxIndex];
            sVE_C0F8b* vt = (sVE_C0F8b*)self->vt;
            if (!vt[9].fn(vthis(self, vt[9]), box->MinPoint, box->MaxPoint)) {
                continue;
            }
            int end = (boxIndex + 1) * 10 < mesh->NumFaces ? (boxIndex + 1) * 10 : mesh->NumFaces;
            for (int i = boxIndex * 10; i < end; i++) {
                t4Vector* nrm = &mesh->Normal[i];
                if (self->useNormal != 0 && vDot_C0F8(*nrm, self->dir) > 0.8f) {
                    continue;
                }
                                t4Vector* vertices = mesh->Vertex;
                t4Vector* corner0 = &vertices[mesh->Face[i][0]];
                t4Vector* corner1 = &vertices[mesh->Face[i][1]];
                t4Vector* corner2 = &vertices[mesh->Face[i][2]];
                int isHit;
                if (flag != 0) {
                    isHit = self->vt[5].fn(vthis(self, self->vt[5]), corner0, corner1, corner2, nrm, &pos, &hn, &hitParam);
                } else {
                    isHit = self->vt[4].fn(vthis(self, self->vt[4]), corner0, corner1, corner2, nrm, &pos, &hn, &hitParam);
                }
                if (isHit == 0) {
                    continue;
                }
                tIntersectInfo* record = &out[n];
                record->Patch = 0;
                record->HitType = 2;
                record->Face = -1;
                record->Inst = 0;
                record->Spline = 0;
                record->Mesh = -1;
                record->Seg = -1;
                record->Stype = -1;
                (record->Ivel) = t4vect_Zero;
                (record->Iangvel) = t4vect_Zero;
                record->T = 0.0f;
                record->u = 0.0f;
                record->v = 0.0f;
                (record->Ipos) = pos;
                (record->Inorm) = hn;
                record->Iparam = hitParam;
                record->Face = i;
                record->Patch = 0;
                n++;
            }
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", __10SphereTree);
#ifdef SKIP_ASM
// t4Vector with On Tour's empty default constructor (ours has none): constructing axis[8] leaves only the dead loop
// setup (li 7, li -1), which only a non-POD element type produces.
struct tConstructedVector : t4Vector {
    tConstructedVector() {}
};
extern "C" void SphereTree_init(SphereTree* self);

/**
 * Constructs an empty sphere tree: no treeInfo and default axes (SphereTree_init).
 */
SphereTree::SphereTree()
{
    new (axis) tConstructedVector[8];
    treeInfo = 0;
    init();
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_init);
#ifdef SKIP_ASM
extern t4Vector gSphTreeAlignAxis[8];
extern t4Vector t4vect_Origin;

/**
 * Resets a sphere tree's placement: default axes (gSphTreeAlignAxis), centre at the origin (+0x80) and scale 1 (+0x90).
 */
void SphereTree::init()
{
    t4Vector* dst = (t4Vector*)this;
    for (int i = 0; i < 8; i++) {
        dst[i] = gSphTreeAlignAxis[i];
    }
    transCenter = t4vect_Origin;
    scale = 1.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_GetRadius);
#ifdef SKIP_ASM
/**
 * Returns a sphere tree's radius: its root level's radius times its scale.
 *
 * Reads the first level radius of the tree info (+0x98, levels at +0x20) and multiplies by the scale (+0x90). Called by
 * bxSphereTree_Is2SidedTriCollision and the cWorldSphTree functions.
 *
 * @return the radius
 */
float SphereTree::GetRadius()
{
    tSphTreeInfo* tree = treeInfo;
    void* levels = tree->levels;
    return *(float*)levels * scale;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_GetResolution);
#ifdef SKIP_ASM
struct sSphereRadius12 {
    float value;
    int pad[2];
};

/**
 * Returns the tree's collision resolution: half the radius of the level at the tree's depth, times its scale.
 *
 * Reads treeInfo (+0x98): levels[depth].radius (levels at +0x20, depth at +0xC) * scale (+0x90) * 0.5.
 *
 * @return the resolution
 */
float SphereTree::GetResolution()
{
    tSphTreeInfo* treeInfo = this->treeInfo;
    int idx = treeInfo->depth;
    sSphereRadius12* tab = (sSphereRadius12*)treeInfo->levels;
    return tab[idx].value * scale * 0.5f;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_AddOffset);
#ifdef SKIP_ASM
struct sSphereVec4_C5E0 {
    float x, y, z, w;
    sSphereVec4_C5E0() {}
    sSphereVec4_C5E0(float* n) { x = n[5]; y = n[6]; z = n[7]; w = 1.0f; }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (a + b).
static inline sSphereVec4_C5E0 Add_C5E0(const sSphereVec4_C5E0& a, const sSphereVec4_C5E0& b)
{
    sSphereVec4_C5E0 r;
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

/**
 * Places a sphere tree at its stored root centre plus an offset.
 *
 * Sets the centre (+0x80) to the tree info's root centre (info +0x14, w = 1) + @p offset, unlike SphereTree_MoveOffset,
 * which adds to the current centre. Nothing calls it.
 *
 * @param offset  the offset (On Tour offset)
 */
extern "C" void SphereTree_AddOffset(SphereTree* self, sSphereVec4_C5E0* offset)
{
    float* n = (float*)self->treeInfo;
    sSphereVec4_C5E0 newPos = Add_C5E0(sSphereVec4_C5E0(n), *offset);
    *(sSphereVec4_C5E0*)&self->transCenter = newPos;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_MoveOffset);
#ifdef SKIP_ASM
/**
 * Moves the sphere tree by an offset: adds @p offset to its centre (+0x80).
 *
 * @param offset  the offset to add
 */
// self->(0x80) += *v  (4-wide VU0 add, macro mode)
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
void SphereTree::MoveOffset(t4Vector* offset)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf3, 0x80(%0)\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, 0x80(%0)\n"
        ".set reorder\n"
        :
        : "r"(((void*)this)), "r"(((void*)offset))
        : "memory");
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_TransformAbs);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
static inline t4Vector mtxMulVec_C648(t4Vector* m, const t4Vector& v)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
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
        ".set pop\n"
        : "=m"(r)
        : "m"(v), "r"(m)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector scale_C648(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// t3Vector with On Tour's (x, y, z) constructor.
struct tVector3Init : t3Vector {
    tVector3Init(float a, float b, float c) { x = a; y = b; z = c; }
};

// t4Vector with On Tour's t4Vector(const t3vect& a, float iw) constructor.
struct tVector4FromVector3 : t4Vector {
    tVector4FromVector3(const t3Vector& v, const float& ww) { x = v.x; y = v.y; z = v.z; w = ww; }
};

extern t4Vector gSphTreeAlignAxis[8];

/**
 * Places the sphere tree with an absolute transform and scale.
 *
 * Transforms each of the four scaled default axes (gSphTreeAlignAxis) by @p transMat, storing it and its negative,
 * sets transCenter to the tree info's scaled rootCenter transformed by @p transMat, and the scale to @p scale.
 *
 * @param transMat      the transform (On Tour transMat)
 * @param scale  the scale
 */
void SphereTree::TransformAbs(t4Vector* transMat, float scale)
{
    t4Vector* pos = axis;
    t4Vector* neg = &axis[7];
    for (int i = 0; i < 4; i++) {
        pos[i] = mtxMulVec_C648(transMat, scale_C648(gSphTreeAlignAxis[i], scale));
        neg[-i] = scale_C648(pos[i], -1.0f);
    }
    tSphTreeInfo* info = treeInfo;
    transCenter = mtxMulVec_C648(transMat, tVector4FromVector3(tVector3Init(info->rootCenter[0] * scale,
        info->rootCenter[1] * scale, info->rootCenter[2] * scale), 1.0f));
    this->scale = scale;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_TransformRel);
#ifdef SKIP_ASM



/**
 * Transforms a sphere tree's four axis directions (their opposites follow) and its centre, then scales them
 * and its radius.
 *
 * @param mat  the transform (On Tour const t4Matrix& transMat)
 * @param scale  uniform scale (On Tour s)
 */
void SphereTree::TransformRel(t4Vector* mat, float scale)
{
    for (int i = 0; i < 4; i++) {
        axis[i] = Scale_B2B8(mtxMulVec_9B90(mat, &axis[i]), scale);
        axis[7 - i] = Scale_B2B8(axis[i], -1.0f);
    }
    t4Vector scaledCenter;
    t4Vector transformedCenter = mtxMulVec_9B90(mat, (t4Vector*)&transCenter);
    t4Vector sc;
    sc.w = 1.0f;
    sc.x = scale;
    sc.y = scale;
    sc.z = scale;
    scaledCenter.x = transformedCenter.x * sc.x;
    scaledCenter.y = transformedCenter.y * sc.y;
    scaledCenter.z = transformedCenter.z * sc.z;
    scaledCenter.w = transformedCenter.w * sc.w;
    transCenter = scaledCenter;
    this->scale = this->scale * scale;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_IsSphereCollision);
#ifdef SKIP_ASM
extern "C" unsigned char* SphereTree_AllocBuf(tSphTreeInfo* self);
extern "C" bool SphereTree_isSphereCollision(SphereTree* self, t4Vector* a1, float f, t4Vector* pos, int a4, int a5,
        t4Vector* a6, t4Vector* a7);

/**
 * Tests a sphere tree against a sphere.
 *
 * Unpacks the tree data when needed (SphereTree_AllocBuf) and runs SphereTree_isSphereCollision from the root (centre +0x80, level 0,
 * index 0). Called by cWorldSphere_IsSphTreeInt.
 *
 * @param spherePos  the sphere's centre (On Tour aCen)
 * @param sphereRadius  its radius (On Tour aRad)
 * @param outPush  out: the overlap vector (On Tour overlap)
 * @param outContact  out: the contact point (On Tour loc)
 * @return 1 on contact, else 0
 */
int SphereTree::IsSphereCollision(t4Vector* spherePos, float sphereRadius, t4Vector* outPush, t4Vector* outContact)
{
    tSphTreeInfo* tree = treeInfo;
    if (tree->isCompressed != 0) {
        treeInfo->bitTree = SphereTree_AllocBuf(tree);
    }
    return isSphereCollision(spherePos, sphereRadius, (t4Vector*)&transCenter, 0, 0, outPush, outContact);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_IsBoxCollision);
#ifdef SKIP_ASM
extern "C" unsigned char* SphereTree_AllocBuf(tSphTreeInfo* self);
extern "C" bool SphereTree_isBoxCollision(SphereTree* self, t3Vector* a1, t3Vector* a2, t4Vector* pos, int a4, int a5,
        t4Vector* a6, t4Vector* a7);

/**
 * Tests a sphere tree against an axis-aligned box.
 *
 * Unpacks the tree data when needed and runs SphereTree_isBoxCollision from the root. Called by cWorldSphTree_IsBoxInt.
 *
 * @param boxMin  the box minimum (On Tour bMin)
 * @param boxMax  the box maximum (On Tour bMax)
 * @param outPos  out: the push-out vector (On Tour overlap)
 * @param outNormal  out: the contact point (On Tour loc), despite the name
 * @return 1 on contact, else 0
 */
int SphereTree::IsBoxCollision(t3Vector* boxMin, t3Vector* boxMax, t4Vector* outPos, t4Vector* outNormal)
{
    tSphTreeInfo* tree = treeInfo;
    if (tree->isCompressed != 0) {
        treeInfo->bitTree = SphereTree_AllocBuf(tree);
    }
    return isBoxCollision(boxMin, boxMax, (t4Vector*)&transCenter, 0, 0, outPos, outNormal);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_IsPlaneCollision);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// Non-POD 16-byte vector: passed by reference to a caller-made copy.
struct sVecArg_C9B8 {
    float x, y, z, w;
    sVecArg_C9B8() {}
    sVecArg_C9B8(const sVecArg_C9B8& o) { *(cQuad128*)this = *(const cQuad128*)&o; }
} __attribute__((aligned(16)));

static inline sVecArg_C9B8 zeroW_C9B8(const sVecArg_C9B8& in)
{
    sVecArg_C9B8 r = in;
    r.w = 0;
    return r;
}

extern "C" void SphereTree_populateSortAxis_v(void* self, sVecArg_C9B8 v) __asm__("SphereTree_populateSortAxis");
extern "C" void SphereTree_checkPlaneCollision(SphereTree* self, void* a1, t4Vector* pos, int a3, int a4, t4Vector* a5, t4Vector* a6);

/**
 * Tests a sphere tree against a plane and returns the deepest penetration.
 *
 * Unpacks the tree data when needed, orders the child axes along the plane normal (SphereTree_populateSortAxis with w
 * cleared), resets the best depth (+0x94) to 0 and runs SphereTree_checkPlaneCollision from the root. Nothing calls it.
 *
 * @param plane  the plane: normal, offset in w (On Tour aPlane)
 * @param outPush  out: the push-out vector (On Tour overlap)
 * @param outContact  out: the contact point (On Tour loc)
 * @return true when some leaf sphere crosses the plane (best depth below 0), else false
 */
extern "C" bool SphereTree_IsPlaneCollision(SphereTree* self, sVecArg_C9B8* plane, t4Vector* outPush,
        t4Vector* outContact)
{
    tSphTreeInfo* tree = self->treeInfo;
    if (tree->isCompressed != 0) {
        self->treeInfo->bitTree = SphereTree_AllocBuf(tree);
    }
    SphereTree_populateSortAxis_v(self, zeroW_C9B8(*plane));
    self->worstCase = 0.0f;
    self->checkPlaneCollision(plane, (t4Vector*)&self->transCenter, 0, 0, outPush, outContact);
    if (self->worstCase < 0.0f) {
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_IsTriCollision);
#ifdef SKIP_ASM
extern "C" unsigned char* SphereTree_AllocBuf(tSphTreeInfo* self);
extern "C" void SphereTree_populateSortAxis(SphereTree* self, t4Vector* v);
extern "C" int SphereTree_checkTriCollision(void* self, void* a1, void* a2, void* a3, void* v, void* pos, int a6, int a7, void* a8, void* a9);

/**
 * Tests the sphere tree against a triangle (one-sided).
 *
 * Fetches the unpacked tree into the cache (SphereTree_AllocBuf) when it is compressed, orders the axes along the normal
 * (SphereTree_populateSortAxis), clears the best depth (+0x94) and runs the recursive test SphereTree_checkTriCollision from the root.
 *
 * @param point1  first triangle corner
 * @param point2  second triangle corner
 * @param point3  third triangle corner
 * @param uNorm   the triangle's unit normal
 * @param overlap  out: how far to push the tree out (On Tour overlap)
 * @param loc  out: the contact point (On Tour loc)
 * @return true on contact
 */
bool SphereTree::IsTriCollision(t4Vector* point1, t4Vector* point2, t4Vector* point3, t4Vector* uNorm,
                               t4Vector* overlap, t4Vector* loc)
{
    tSphTreeInfo* treeInfo = this->treeInfo;
    if (treeInfo->isCompressed != 0) {
        this->treeInfo->bitTree = SphereTree_AllocBuf(treeInfo);
    }
    populateSortAxis((t4Vector*)uNorm);
    worstCase = 0.0f;
    SphereTree_checkTriCollision(this, point1, point2, point3, uNorm, (char*)&transCenter, 0, 0, overlap, loc);
    if (worstCase < 0.0f) {
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_IsTreeCollision);
#ifdef SKIP_ASM
extern "C" unsigned char* SphereTree_AllocBuf(tSphTreeInfo* self);
extern "C" bool SphereTree_isTreeCollision(SphereTree* self, SphereTree* other, t4Vector* pos, int a3, int a4,
        t4Vector* a5, t4Vector* a6);

/**
 * Tests a sphere tree against another sphere tree.
 *
 * Unpacks both trees' data when needed and runs SphereTree_isTreeCollision from this tree's root. Called by
 * cWorldSphTree_IsSphTreeInt.
 *
 * @param other  the other tree (On Tour aTree)
 * @param outPush  out: the overlap vector (On Tour overlap)
 * @param outContact  out: the contact point (On Tour loc)
 * @return 1 on contact, else 0
 */
int SphereTree::IsTreeCollision(SphereTree* other, t4Vector* outPush, t4Vector* outContact)
{
    tSphTreeInfo* tree = treeInfo;
    if (tree->isCompressed != 0) {
        treeInfo->bitTree = SphereTree_AllocBuf(tree);
    }
    tree = (tSphTreeInfo*)((void*)other->treeInfo);
    if (tree->isCompressed != 0) {
        other->treeInfo->bitTree = SphereTree_AllocBuf(tree);
    }
    return isTreeCollision((SphereTree*)other, (t4Vector*)&transCenter, 0, 0, outPush, outContact);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_isTreeCollision);
#ifdef SKIP_ASM
extern "C" bool SphereTree_isSphereCollision(SphereTree* self, t4Vector* a1, float f, t4Vector* pos, int a4, int a5,
        t4Vector* a6, t4Vector* a7);
extern "C" bool SphereTree_isTreeCollision(SphereTree* self, SphereTree* other, t4Vector* pos, int a3, int a4,
        t4Vector* a5, t4Vector* a6);

struct sSphereLevel_CBF8 {
    float radius;   // 0x0
    float offset;   // 0x4
    int stride;     // 0x8
};

struct sSphereTreeInfo_CBF8 {
    char pad_0x00[0xC];
    int depth;                      // 0x0C
    char pad_0x10[0x10];
    sSphereLevel_CBF8* levels;      // 0x20
    char pad_0x24[4];
    unsigned char* masks;           // 0x28
};


// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_CBF8(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_CBF8(const t4Vector& a, const t4Vector& b)
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

/**
 * Recursively tests one sphere tree's nodes against another sphere tree.
 *
 * Each child set in this node's occupancy byte (info +0x28, index @p base) is placed at @p posp + axis i x the next
 * level's offset and tested as a sphere against @p other with SphereTree_isSphereCollision, using this level's radius x scale; a hit
 * recurses one level down. Returns 1 at the tree's depth, at level 4 (deeper levels are never tested) or for a node
 * with no children; otherwise 1 only when some child path hits.
 *
 * @param other  the other tree (On Tour aTree)
 * @param posp  this node's centre (On Tour center)
 * @param depth  this node's level (On Tour level)
 * @param base  this node's index (On Tour index)
 * @param outPush  out: the overlap vector (On Tour overlap)
 * @param outContact  out: the contact point (On Tour loc)
 * @return true on contact, else false
 */
bool SphereTree::isTreeCollision(SphereTree* other, t4Vector* posp, int depth, int base, t4Vector* outPush,
                                t4Vector* outContact)
{
    SphereTree* self = this;
    t4Vector childPos;
    unsigned char mask = ((sSphereTreeInfo_CBF8*)self->treeInfo)->masks[base];
    if (depth < ((sSphereTreeInfo_CBF8*)self->treeInfo)->depth) {
        if (depth >= 4) {
            return true;
        }
        if (mask != 0) {
            float off = ((sSphereTreeInfo_CBF8*)self->treeInfo)->levels[depth + 1].offset;
            for (int i = 0; i < 8; i++) {
                if ((mask >> i) & 1) {
                    childPos = Add_CBF8(*(t4Vector*)posp, Scale_CBF8(self->axis[i], off));
                    if (other->isSphereCollision((t4Vector*)(&childPos),
                            ((sSphereTreeInfo_CBF8*)self->treeInfo)->levels[depth].radius * self->scale,
                            (t4Vector*)(&other->transCenter), 0, 0, outPush, outContact) &&
                        self->isTreeCollision(other, (t4Vector*)(&childPos), depth + 1,
                                (i + 1) * ((sSphereTreeInfo_CBF8*)self->treeInfo)->levels[depth].stride + base,
                                outPush, outContact)) {
                        return true;
                    }
                }
            }
            return false;
        }
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_isSphereCollision);
#ifdef SKIP_ASM
struct sSphereLevel_CDB0 {
    float radius;   // 0x0
    float offset;   // 0x4
    int stride;     // 0x8
};

struct sSphereTreeInfo_CDB0 {
    char pad_0x00[0xC];
    int depth;                      // 0x0C
    char pad_0x10[0x10];
    sSphereLevel_CDB0* levels;      // 0x20
    char pad_0x24[4];
    unsigned char* masks;           // 0x28
};


// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_CDB0(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_CDB0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_CDB0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_CDB0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (v / |v|).
static inline t4Vector Normalize_CDB0(const t4Vector& v)
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

// PORT: sqrt.s (sqrtf without errno check)
static inline float Sqrt_CDB0(float v)
{
    float r;
    __asm__("sqrt.s %0, %1" : "=f"(r) : "f"(v));
    return r;
}

/**
 * Recursively tests a sphere tree node against a sphere.
 *
 * Returns 0 when the sphere misses this node's sphere (level radius x scale around @p pos). Inner nodes recurse into
 * each child in the occupancy byte (placed at pos + axis i x the next level's offset) and return 1 at the first hit. A
 * leaf (at the tree's depth, or without children) sets *@p outA = n x overlap and *@p outB = the sphere centre - n x
 * (radius - overlap), n pointing from the node to the sphere, and returns 1. Called by bxSphereTree_IsRiderTreeCollision, SphereTree_IsSphereCollision,
 * SphereTree_isTreeCollision and itself.
 *
 * @param spherePos  the sphere's centre (On Tour aCen)
 * @param sphereRadius  its radius (On Tour aRad)
 * @param pos  this node's centre (On Tour center)
 * @param depth  this node's level (On Tour level)
 * @param base  this node's index (On Tour index)
 * @param outA  out: the overlap vector (On Tour overlap)
 * @param outB  out: the contact point (On Tour loc)
 * @return true on contact, else false
 */
bool SphereTree::isSphereCollision(t4Vector* spherePos, float sphereRadius, t4Vector* pos, int depth, int base,
                                  t4Vector* outA, t4Vector* outB)
{
    SphereTree* self = this;
    float sum = sphereRadius + ((sSphereTreeInfo_CDB0*)self->treeInfo)->levels[depth].radius * self->scale;
    t4Vector delta = Sub_CDB0(*(t4Vector*)spherePos, *(t4Vector*)pos);
    float dd = Dot_CDB0(delta, delta);
    if (!(dd < sum * sum))
        return false;
    float distance;
    unsigned char mask = ((sSphereTreeInfo_CDB0*)self->treeInfo)->masks[base];
    if (depth < ((sSphereTreeInfo_CDB0*)self->treeInfo)->depth && mask != 0) {
        distance = ((sSphereTreeInfo_CDB0*)self->treeInfo)->levels[depth + 1].offset;
        for (int i = 0; i < 8; i++) {
            if ((mask >> i) & 1) {
                t4Vector childPos = Add_CDB0(*(t4Vector*)pos, Scale_CDB0(self->axis[i], distance));
                if (self->isSphereCollision(spherePos, sphereRadius, (t4Vector*)(&childPos), depth + 1,
                        (i + 1) * ((sSphereTreeInfo_CDB0*)self->treeInfo)->levels[depth].stride + base, outA, outB))
                    return true;
            }
        }
        return false;
    }
    t4Vector n = Normalize_CDB0(delta);
    distance = sum - Sqrt_CDB0(dd);
    *(t4Vector*)outA = Scale_CDB0(n, distance);
    *(t4Vector*)outB = Sub_CDB0(*(t4Vector*)spherePos, Scale_CDB0(n, sphereRadius - distance));
    return true;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_isBoxCollision);
#ifdef SKIP_ASM
extern "C" bool gIsBoxSphereInt(void* a, void* b, void* center, void* box, void* boxMin, void* boxMax, float radius);
extern "C" bool SphereTree_isBoxCollision(SphereTree* self, t3Vector* a1, t3Vector* a2, t4Vector* pos, int a4, int a5,
        t4Vector* a6, t4Vector* a7);




// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_D028(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_D028(const t4Vector& a, const t4Vector& b)
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

/**
 * Recursively tests a sphere tree node against an axis-aligned box.
 *
 * Tests this node's sphere (level radius x scale at @p pos) with gIsBoxSphereInt; a miss returns 0. Inner nodes recurse
 * into their children and return 1 at the first hit; a leaf stores the contact point in *@p outNormal and face normal x
 * depth in *@p outPos and returns 1. Called by SphereTree_IsBoxCollision and itself.
 *
 * @param boxMin  the box minimum (On Tour bMin)
 * @param boxMax  the box maximum (On Tour bMax)
 * @param pos  this node's centre (On Tour center)
 * @param depth  this node's level (On Tour level)
 * @param base  this node's index (On Tour index)
 * @param outPos  out: the push-out vector (On Tour overlap)
 * @param outNormal  out: the contact point (On Tour loc), despite the name
 * @return true on contact, else false
 */
bool SphereTree::isBoxCollision(t3Vector* boxMin, t3Vector* boxMax, t4Vector* pos, int depth, int base,
                               t4Vector* outPos, t4Vector* outNormal)
{
    SphereTree* self = this;
    t4Vector normal;
    t4Vector dir;
    float dist;
    if (gIsBoxSphereInt(boxMin, boxMax, pos, &normal, &dir, &dist, (self->treeInfo->levels)[depth].radius * self->scale) == 0) {
        return false;
    }
    unsigned char mask = self->treeInfo->bitTree[base];
    if (depth < (self->treeInfo)->depth && mask != 0) {
        float off = (self->treeInfo->levels)[depth + 1].cenOffset;
        for (int i = 0; i < 8; i++) {
            if ((mask >> i) & 1) {
                t4Vector childPos = Add_D028(*(t4Vector*)pos, Scale_D028(self->axis[i], off));
                if (self->isBoxCollision(boxMin, boxMax, (t4Vector*)(&childPos), depth + 1,
                        (i + 1) * (self->treeInfo->levels)[depth].indexOffset + base, outPos, outNormal)) {
                    return true;
                }
            }
        }
        return false;
    }
    *(t4Vector*)outNormal = normal;
    *(t4Vector*)outPos = Scale_D028(dir, dist);
    return true;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_checkPlaneCollision);
#ifdef SKIP_ASM



// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_D208(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_D208(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_D208(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_D208(const t4Vector& a, const t4Vector& b)
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

static inline t4Vector ZeroW_D208(const t4Vector& in)
{
    t4Vector r = in;
    r.w = 0;
    return r;
}

/**
 * Recursively finds a sphere tree's deepest penetration of a plane.
 *
 * The node's signed distance is the 4-component dot of @p plane with @p pos (the plane's w acting as its offset). Nodes
 * farther than their radius (level radius x scale) plus the best depth so far (+0x94) are skipped. Inner nodes visit
 * their children in the sorted order (+0x9C); a leaf whose distance - radius beats the best stores the plane normal x
 * that depth in *@p outA, the centre moved onto the plane in *@p outB, and the depth in +0x94. Called by SphereTree_IsPlaneCollision
 * and itself.
 *
 * @param plane  the plane (On Tour aPlane)
 * @param pos  this node's centre (On Tour center)
 * @param depth  this node's level (On Tour level)
 * @param base  this node's index (On Tour index)
 * @param outA  out: the push-out vector (On Tour overlap)
 * @param outB  out: the contact point (On Tour loc)
 */
void SphereTree::checkPlaneCollision(void* plane, t4Vector* pos, int depth, int base, t4Vector* outA, t4Vector* outB)
{
    SphereTree* self = this;
    float planeDist = Dot_D208(*(t4Vector*)plane, *(t4Vector*)pos);
    float nodeRadius = (self->treeInfo->levels)[depth].radius * self->scale;
    if (!(planeDist < nodeRadius + self->worstCase))
        return;
    float distance;
    unsigned char mask = self->treeInfo->bitTree[base];
    if (depth < (self->treeInfo)->depth && mask != 0) {
        distance = (self->treeInfo->levels)[depth + 1].cenOffset;
        for (int i = 0; i < 8; i++) {
            int k = self->sortAxis[i];
            if ((mask >> k) & 1) {
                t4Vector childPos = Add_D208(*(t4Vector*)pos, Scale_D208(self->axis[k], distance));
                self->checkPlaneCollision(plane, (t4Vector*)(&childPos), depth + 1,
                        (k + 1) * (self->treeInfo->levels)[depth].indexOffset + base, outA, outB);
            }
        }
        return;
    }
    distance = planeDist - nodeRadius;
    if (distance < self->worstCase) {
        t4Vector n = ZeroW_D208(*(t4Vector*)plane);
        *(t4Vector*)outA = Scale_D208(n, distance);
        *(t4Vector*)outB = Sub_D208(*(t4Vector*)pos, Scale_D208(n, planeDist));
        self->worstCase = distance;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_isSphereInsideTri);
#ifdef SKIP_ASM
// PORT: this caller passes a tolerance in $f12 that the unit's 4-arg gIsPointInsideTri never reads;
// bind the 5-arg form to the same symbol.
extern "C" bool gIsPointInsideTri_tol(t4Vector* a, t4Vector* b, t4Vector* c, t4Vector* d,
        float tol) __asm__("gIsPointInsideTri");

/**
 * Tells whether a sphere's centre lies inside a triangle: forwards to gIsPointInsideTri with tolerance 1e-4.
 *
 * A radius the caller may pass in $f12 is replaced by the tolerance. Called by SphereTree_checkTriCollision.
 *
 * @param corner1  first corner (On Tour p1)
 * @param corner2  second corner (On Tour p2)
 * @param corner3  third corner (On Tour p3)
 * @param sphereCenter  the sphere's centre (On Tour center)
 * @return 1 when the centre is inside, else 0
 */
int SphereTree::isSphereInsideTri(t4Vector* corner1, t4Vector* corner2, t4Vector* corner3, t4Vector* sphereCenter)
{
    return gIsPointInsideTri_tol(corner1, corner2, corner3, sphereCenter, 9.999999747378752e-05f);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_checkTriCollision);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (v * s).
static inline t4Vector Scale_D470(const t4Vector& v, float s)
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

// PORT: PS2-only VU0 inline asm (a + b).
static inline t4Vector Add_D470(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (a - b).
static inline t4Vector Sub_D470(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float Dot_D470(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float Length_D470(const t4Vector& v)
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

// PORT: PS2-only FPU sqrt.s helper.
static inline float Sqrt_D470(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

// Does the segment from the start point (e = center - start) along g pass within sqrt(rr) of the center?
static inline bool SegHit_D470(const t4Vector& e, const t4Vector& g, float ee, float rr)
{
    float len = Length_D470(g);
    float proj = Dot_D470(e, g) / len;
    if (proj < 0.0f) {
        return false;
    }
    float t = ee - proj * proj;
    if (rr < t) {
        return false;
    }
    if (len < proj - Sqrt_D470(rr - t)) {
        return false;
    }
    return true;
}

// PORT: the unit's 5-arg SphereTree_isSphereInsideTri also receives the radius in $f12 here.
extern "C" int SphereTree_isSphereInsideTri_r(void* self, t4Vector* a, t4Vector* b, t4Vector* c, t4Vector* d, float r) __asm__("SphereTree_isSphereInsideTri");
// PORT: the unit declares SphereTree_checkTriCollision as returning int; it returns nothing.
extern "C" void SphereTree_checkTriCollision_v(void* selfp, t4Vector* a1, t4Vector* a2, t4Vector* a3, t4Vector* v, t4Vector* pos, int depth, int base, t4Vector* outA, t4Vector* outB) __asm__("SphereTree_checkTriCollision");

/**
 * Recursive step of the sphere tree's triangle test: checks one sphere and descends into its children.
 *
 * The sphere's signed distance d from the triangle's plane is uNorm . (pos - point1); its radius r is
 * levels[depth].radius * scale (+0x90). It is skipped when it is behind the plane at the root, or when d is not below r
 * plus the deepest penetration so far (worstCase, +0x94). When the sphere's centre projected on the plane is rejected
 * by the VU0 triangle test SphereTree_isSphereInsideTri, a leaf, or a sphere smaller than 20, is skipped, and a larger one goes on
 * only if it contains point1 or touches one of the three edges.
 *
 * A non-leaf sphere (depth below both 3 and the tree's depth, with child bits in the bit tree) recurses into each
 * present child, in sortAxis (+0x9C) order, at pos + axis[k] * levels[depth + 1].cenOffset; a leaf whose d - r is below
 * worstCase stores it there, with the push-out vector uNorm * (r - d) in @p outA and the projected point in @p outB.
 *
 * @param selfp   the sphere tree (On Tour SphereTree)
 * @param point1  first triangle corner
 * @param point2  second triangle corner
 * @param point3  third triangle corner
 * @param uNorm   the triangle's unit normal
 * @param pos     this sphere's centre
 * @param depth   the tree level
 * @param base    this sphere's index at that level
 * @param outA    out: the push-out vector (On Tour overlap)
 * @param outB    out: the contact point (On Tour loc)
 */
// Sphere tree against the triangle (a1, a2, a3) moving along v: records the deepest
// penetration below self->best (*outA = push-out vector, *outB = contact point).
extern "C" void SphereTree_checkTriCollision_v(void* selfp, t4Vector* point1, t4Vector* point2, t4Vector* point3, t4Vector* uNorm, t4Vector* pos, int depth, int base, t4Vector* outA, t4Vector* outB)
{
    SphereTree* self = (SphereTree*)selfp;
    float planeDist = Dot_D470(*uNorm, Sub_D470(*pos, *point1));
    float levelRadius = self->treeInfo->levels[depth].radius * self->scale;
    if (depth == 0 && planeDist < 0.0f) {
        return;
    }
    if (!(planeDist < levelRadius + self->worstCase)) {
        return;
    }
    int leaf = 0;
    unsigned char mask = self->treeInfo->bitTree[base];
    if (!(depth < self->treeInfo->depth && depth < 3 && mask != 0)) {
        leaf = 1;
    }
    t4Vector planePoint = Sub_D470(*pos, Scale_D470(*uNorm, planeDist));
    if (SphereTree_isSphereInsideTri_r(self, point1, point2, point3, &planePoint, levelRadius) == 0) {
        if (leaf) {
            return;
        }
        if (levelRadius < 20.0f) {
            return;
        }
        t4Vector fromCorner1 = Sub_D470(*pos, *point1);
        float rr = levelRadius * levelRadius;
        float ee = Dot_D470(fromCorner1, fromCorner1);
        if (rr < ee) {
            t4Vector edge12 = Sub_D470(*point2, *point1);
            bool miss = !SegHit_D470(fromCorner1, edge12, ee, rr);
            if (miss) {
                t4Vector edge13 = Sub_D470(*point3, *point1);
                miss = !SegHit_D470(fromCorner1, edge13, ee, rr);
                if (miss) {
                    t4Vector fromCorner2 = Sub_D470(*pos, *point2);
                    float ee3 = Dot_D470(fromCorner2, fromCorner2);
                    t4Vector edge23 = Sub_D470(*point3, *point2);
                    miss = !SegHit_D470(fromCorner2, edge23, ee3, rr);
                    if (miss) {
                        return;
                    }
                }
            }
        }
    }
    if (!leaf) {
        float offset = self->treeInfo->levels[depth + 1].cenOffset;
        for (int i = 0; i < 8; i++) {
            int k = self->sortAxis[i];
            if ((mask >> k) & 1) {
                t4Vector childPos = Add_D470(*pos, Scale_D470(self->axis[k], offset));
                SphereTree_checkTriCollision_v(self, point1, point2, point3, uNorm, &childPos, depth + 1,
                    (k + 1) * self->treeInfo->levels[depth].indexOffset + base, outA, outB);
            }
        }
        return;
    }
    float offset = planeDist - levelRadius;
    if (offset < self->worstCase) {
        t4Vector n = Scale_D470(*uNorm, -offset);
        *outA = n;
        *outB = planePoint;
        self->worstCase = offset;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", BXSORT_shell);
#ifdef SKIP_ASM
extern int BXSORT_shell_inctable[];

/**
 * Shell-sorts an array of keys into descending order, moving a parallel value array with them.
 *
 * Gaps come from the table BXSORT_shell_inctable, starting with the largest gap below @p n.
 *
 * @param n     number of entries
 * @param keys  the sort keys (On Tour value)
 * @param vals  the values carried along (On Tour index)
 */
// Shell sort of keys[] (descending) carrying vals[] along, gap table BXSORT_shell_inctable.
extern "C" void BXSORT_shell(int n, int* keys, int* vals)
{
    int k = 1;
    while (BXSORT_shell_inctable[k] < n) {
        k++;
    }
    int gap;
    for (;;) {
        gap = BXSORT_shell_inctable[--k];
        for (int i = gap; i < n; i++) {
            int key = keys[i];
            int val = vals[i];
            int j = i - gap;
            do {
                if (keys[j] >= key) {
                    break;
                }
                keys[j + gap] = keys[j];
                vals[j + gap] = vals[j];
                j -= gap;
            } while (j >= 0);
            keys[j + gap] = key;
            vals[j + gap] = val;
        }
        if (gap < 2) {
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_populateSortAxis);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_DB40(const t4Vector& a, const t4Vector& b)
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
static inline t4Vector vu0Scale_DB40(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

extern "C" void BXSORT_shell(int n, int* keys, int* idx);


/**
 * Orders the tree's eight child axes by how far they point against a direction, most opposed first.
 *
 * Projects each of the four axes (+0x0) on -1000 times @p vdir (the four opposite axes get the negated value) and sorts
 * the eight indices in sortAxis (+0x9C) by it, descending, with BXSORT_shell.
 *
 * @param vdir   the direction (On Tour norm)
 */
void SphereTree::populateSortAxis(t4Vector* vdir)
{
    SphereTree* self = this;
    t4Vector* dir = (t4Vector*)vdir;
    int keys[8];
    t4Vector scaledDir = vu0Scale_DB40(*dir, -1000.0f);
    int i;
    for (i = 0; i < 4; i++) {
        int n = (int)vu0Dot_DB40(self->axis[i], scaledDir);
        keys[i] = n;
        keys[7 - i] = -n;
        self->sortAxis[i] = i;
        self->sortAxis[7 - i] = 7 - i;
    }
    BXSORT_shell(8, keys, self->sortAxis);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_initCache);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label as a placement operator new[].
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_SphereTreeCache[];
extern void** SphereTree_buffers;
extern int* SphereTree_bufID;
extern int SphereTree_bufNum;
extern int SphereTree_bufPtr;
extern void** SphereTree_bigBuffers;
extern int* SphereTree_bigBufID;
extern int SphereTree_bigBufNum;
extern int SphereTree_bigBufPtr;
extern int SphereTree_bigBufDepth;

/**
 * Allocates the sphere-tree decompression caches: small and big buffers, each holding a full octree of its
 * depth.
 *
 * All buffers start unassigned (-1).
 *
 * @param nSmall  number of small buffers (On Tour numBufs)
 * @param smallDepth  octree depth of a small buffer (On Tour bufDepth)
 * @param nBig  number of big buffers (On Tour numBigBufs)
 * @param threshold  octree depth of a big buffer, also kept in SphereTree_bigBufDepth (On Tour bigDepth)
 */
// Allocates the two decompression caches; each buffer holds a full octree of the given depth
// (1 + 8 + 64 + ... nodes).
extern "C" void SphereTree_initCache(int nSmall, int smallDepth, int nBig, int threshold)
{
    int smallSize = 0;
    int bigSize = 0;
    int i;
    int k;
    SphereTree_bigBufDepth = threshold;
    k = 1;
    for (i = 0; i <= smallDepth; i++) {
        smallSize += k;
        k <<= 3;
    }
    k = 1;
    for (i = 0; i <= SphereTree_bigBufDepth; i++) {
        bigSize += k;
        k <<= 3;
    }
    SphereTree_buffers = 0;
    SphereTree_bufID = 0;
    SphereTree_bigBuffers = 0;
    SphereTree_bigBufID = 0;
    if (nSmall != 0) {
        SphereTree_buffers = new (str_SphereTreeCache, 0, 0) void*[nSmall];
        SphereTree_bufID = new (str_SphereTreeCache, 0, 0) int[nSmall];
    }
    if (nBig != 0) {
        SphereTree_bigBuffers = new (str_SphereTreeCache, 0, 0) void*[nBig];
        SphereTree_bigBufID = new (str_SphereTreeCache, 0, 0) int[nBig];
    }
    for (i = 0; i < nSmall; i++) {
        SphereTree_buffers[i] = new (str_SphereTreeCache, 0, 0) char[smallSize];
        SphereTree_bufID[i] = -1;
    }
    for (i = 0; i < nBig; i++) {
        SphereTree_bigBuffers[i] = new (str_SphereTreeCache, 0, 0) char[bigSize];
        SphereTree_bigBufID[i] = -1;
    }
    SphereTree_bufNum = nSmall;
    SphereTree_bigBufNum = nBig;
    SphereTree_bufPtr = 0;
    SphereTree_bigBufPtr = 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_purgeCache);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern void** SphereTree_buffers;
extern int* SphereTree_bufID;
extern int SphereTree_bufNum;
extern void** SphereTree_bigBuffers;
extern int* SphereTree_bigBufID;
extern int SphereTree_bigBufNum;

/**
 * Frees the cache of unpacked sphere trees.
 *
 * Frees each buffer in SphereTree_buffers and SphereTree_bigBuffers, the arrays themselves and the id tables
 * SphereTree_bufID and SphereTree_bigBufID, clearing each pointer.
 */
extern "C" void SphereTree_purgeCache(void)
{
    if (SphereTree_buffers != 0) {
        for (int i = 0; i < SphereTree_bufNum; i++) {
            if (SphereTree_buffers[i] != 0) {
                cMemMan_free(SphereTree_buffers[i]);
            }
        }
        if (SphereTree_buffers != 0) {
            cMemMan_free(SphereTree_buffers);
        }
        SphereTree_buffers = 0;
    }
    if (SphereTree_bufID != 0) {
        cMemMan_free(SphereTree_bufID);
        SphereTree_bufID = 0;
    }
    if (SphereTree_bigBuffers != 0) {
        for (int i = 0; i < SphereTree_bigBufNum; i++) {
            if (SphereTree_bigBuffers[i] != 0) {
                cMemMan_free(SphereTree_bigBuffers[i]);
            }
        }
        if (SphereTree_bigBuffers != 0) {
            cMemMan_free(SphereTree_bigBuffers);
        }
        SphereTree_bigBuffers = 0;
    }
    if (SphereTree_bigBufID != 0) {
        cMemMan_free(SphereTree_bigBufID);
        SphereTree_bigBufID = 0;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", SphereTree_AllocBuf);
#ifdef SKIP_ASM
extern void** SphereTree_buffers;
extern int* SphereTree_bufID;
extern int SphereTree_bufNum;
extern int SphereTree_bufPtr;
extern void** SphereTree_bigBuffers;
extern int* SphereTree_bigBufID;
extern int SphereTree_bigBufNum;
extern int SphereTree_bigBufPtr;
extern int SphereTree_bigBufDepth;
extern "C" void* sphTreeUnRLE(void* dst, void* src);

/**
 * Returns the unpacked bit tree for a sphere tree, decompressing it into a cache slot when it is not cached.
 *
 * Trees whose depth is below SphereTree_bigBufDepth use the small cache (SphereTree_buffers, SphereTree_bufID),
 * deeper ones the big one (SphereTree_bigBuffers, SphereTree_bigBufID). Checks the slot remembered in lastBufID,
 * then searches the cache for treeID; on a miss unpacks cmpTree with sphTreeUnRLE into the next slot
 * (round robin through bufPtr / bigBufPtr), records the id and remembers the slot.
 *
 * @param selfp  the tree info (On Tour tSphTreeInfo)
 * @return the unpacked bit tree
 */
extern "C" unsigned char* SphereTree_AllocBuf(tSphTreeInfo* selfp)
{
    tSphTreeInfo* self = selfp;
    int key = self->treeID;
    int* pslot = &self->lastBufID;
    if (self->depth < SphereTree_bigBufDepth) {
        if (SphereTree_bufID[*pslot] == key) {
            return (unsigned char*)SphereTree_buffers[*pslot];
        }
        for (int i = 0; i < SphereTree_bufNum; i++) {
            if (SphereTree_bufID[i] == key) {
                *pslot = i;
                return (unsigned char*)SphereTree_buffers[i];
            }
        }
        sphTreeUnRLE(SphereTree_buffers[SphereTree_bufPtr], self->cmpTree);
        SphereTree_bufID[SphereTree_bufPtr] = key;
        *pslot = SphereTree_bufPtr;
        SphereTree_bufPtr++;
        SphereTree_bufPtr %= SphereTree_bufNum;
        return (unsigned char*)SphereTree_buffers[*pslot];
    } else {
        if (SphereTree_bigBufID[*pslot] == key) {
            return (unsigned char*)SphereTree_bigBuffers[*pslot];
        }
        for (int i = 0; i < SphereTree_bigBufNum; i++) {
            if (SphereTree_bigBufID[i] == key) {
                *pslot = i;
                return (unsigned char*)SphereTree_bigBuffers[i];
            }
        }
        sphTreeUnRLE(SphereTree_bigBuffers[SphereTree_bigBufPtr], self->cmpTree);
        SphereTree_bigBufID[SphereTree_bigBufPtr] = key;
        *pslot = SphereTree_bigBufPtr;
        SphereTree_bigBufPtr++;
        SphereTree_bigBufPtr %= SphereTree_bigBufNum;
        return (unsigned char*)SphereTree_bigBuffers[*pslot];
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_cWorldLine);
#ifdef SKIP_ASM
extern char cWorldLine_vtbl[];

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float subs).
static inline t4Vector sphereCapsuleSub(t4Vector* a, t4Vector* b)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(*a), "m"(*b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
static inline t4Vector sphereCapsuleAdd(t4Vector* a, t4Vector* b)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(*a), "m"(*b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (v / s).
static inline t4Vector sphereCapsuleDiv(const t4Vector& v, float s)
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

struct sSphereV4_E100 : t4Vector {
    sSphereV4_E100(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};


/**
 * Constructs a world line query from two end points.
 *
 * Installs cWorldLine_vtbl (+0x50), stores @p state as the precision (+0x0), clears the flag words (+0x4, +0x8), sets
 * the start (+0x60 and +0x80) to @p startPoint and the direction (+0x70 and +0x90) to @p endPoint - @p startPoint, stores @p idealParam at +0xA0 and
 * builds the bounding box (+0x20 max, +0x30 min, w = 1) and its centre (+0x40). Called by the rider ground and air code
 * (cGroundMotion_setupGroundVectors, cAirPredictor_simStep, cRider_putAt and others).
 *
 * @param startPoint  the start point (On Tour ap0)
 * @param endPoint  the end point (On Tour ap1)
 * @param state  the precision, 0..2 (On Tour tIntPrecision)
 * @param idealParam  the preferred hit parameter (On Tour aIdealParam)
 * @return @p self
 */
extern "C" cWorldLine* cWorldLine_cWorldLine(cWorldLine* self, t4Vector* startPoint, t4Vector* endPoint,
                                             int state, float idealParam)
{
    self->mPrecision = state;
    self->mVtbl = (void**)cWorldLine_vtbl;
    self->useRejectNormal = 0;
    self->isTransformed = 0;
    self->origP0 = *startPoint;
    self->p0 = self->origP0;
    self->origDp = sphereCapsuleSub(endPoint, startPoint);
    self->dp = self->origDp;
    self->idealParam = idealParam;
    self->boxMax = sSphereV4_E100(startPoint->x > endPoint->x ? startPoint->x : endPoint->x,
                               startPoint->y > endPoint->y ? startPoint->y : endPoint->y,
                               startPoint->z > endPoint->z ? startPoint->z : endPoint->z, 1.0f);
    self->boxMin = sSphereV4_E100(startPoint->x < endPoint->x ? startPoint->x : endPoint->x,
                               startPoint->y < endPoint->y ? startPoint->y : endPoint->y,
                               startPoint->z < endPoint->z ? startPoint->z : endPoint->z, 1.0f);
    self->boxCenter = sphereCapsuleDiv(sphereCapsuleAdd((t4Vector*)&self->boxMin, (t4Vector*)&self->boxMax), 2.0f);
    return (self);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_IsInBoxTransformed);
#ifdef SKIP_ASM
/**
 * Tells whether the line's transformed segment (p0 .. p0 + dir) can touch an axis-aligned box.
 *
 * A quick reject only: the segment is outside when both of its end points lie beyond the same face of the box.
 *
 * @param mn  the box's minimum corner (On Tour bMin)
 * @param mx  the box's maximum corner (On Tour bMax)
 * @return false when both end points are past one face, otherwise true
 */
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
// segment self->p0 .. p0+dir overlaps the box [mn, mx]?
bool cWorldLine::IsInBoxTransformed(t4Vector* mn, t4Vector* mx)
{
    t4Vector endPoint = sphereCapsuleAdd((t4Vector*)&p0, (t4Vector*)&dp);
    if ((p0.x < mn->x && endPoint.x < mn->x) || (mx->x < p0.x && mx->x < endPoint.x) ||
        (p0.y < mn->y && endPoint.y < mn->y) || (mx->y < p0.y && mx->y < endPoint.y) ||
        (p0.z < mn->z && endPoint.z < mn->z) || (mx->z < p0.z && mx->z < endPoint.z)) {
        return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_Transform);
#ifdef SKIP_ASM
// Componentwise product a * b; the base ctor takes the four products as arguments.
struct sSphereProd_E398 : sSphereV4_E100 {
    sSphereProd_E398(const t4Vector& a, const t4Vector& b)
        : sSphereV4_E100(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w)
    {
    }
};

/**
 * Transforms the line's base segment into a model's space: matrix times base point and direction, then a uniform
 * scale.
 *
 * Stores the results in p0 / dir (+0x60 / +0x70; w kept at 1 by the scale vector) and sets the transformed flag
 * (+0x4).
 *
 * @param mat  the transform (On Tour const t4Matrix& mat)
 * @param scale  the uniform scale applied after the matrix
 */
// Transforms the capsule's base segment (p0b, dirb) by a matrix and a uniform scale (w kept at 1).
void cWorldLine::Transform(t4Vector* mat, float scale)
{
    sSphereV4_E100 sc(scale, scale, scale, 1.0f);
    p0 = sSphereProd_E398(mtxMulVec_9B90(mat, (t4Vector*)&origP0), sc);
    dp = sSphereProd_E398(mtxMulVec_9B90(mat, (t4Vector*)&origDp), sc);
    isTransformed = 1;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_UnTransform);
#ifdef SKIP_ASM
/**
 * Undoes cWorldLine_Transform: copies the base point and direction (+0x80, +0x90) back to +0x60 / +0x70 and clears
 * the transformed flag.
 */
void cWorldLine::UnTransform()
{
    p0 = origP0;
    dp = origDp;
    isTransformed = 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_IsTriInt);
#ifdef SKIP_ASM
// PORT: this caller passes a tolerance in $f12 that the unit's 4-arg gIsPointInsideTri never reads;
// bind the 5-arg form to the same symbol.
extern "C" bool gIsPointInsideTri_tol(t4Vector* a, t4Vector* b, t4Vector* c, t4Vector* d,
        float tol) __asm__("gIsPointInsideTri");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_E4D0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_E4D0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector add).
static inline t4Vector vu0Add_E4D0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector vu0Scale_E4D0(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

/**
 * Intersects the line's segment with a triangle's plane and tests whether the hit lies inside the triangle.
 *
 * Solves origin + t * dir on the plane through @p vertA with normal @p n (no hit when |dir . n| < 1e-10); stores t in
 * @p outT, and for 0 <= t <= 1 the hit point and the normal, then runs the point-in-triangle test (gIsPointInsideTri,
 * tolerance 1e-4).
 *
 * @param vertA  the triangle's first corner, also the point on the plane (On Tour a)
 * @param vertB  the second corner
 * @param vertC  the third corner
 * @param n  the triangle's normal
 * @param hit  receives the intersection point (On Tour ipos)
 * @param outN  receives the hit normal, @p n (On Tour inorm)
 * @param outT  receives the line parameter t (On Tour param); written even when out of range
 * @return nonzero when the segment hits inside the triangle, otherwise 0
 */
// segment origin(0x60)+t*dir(0x70) against the plane through *p with normal *n
int cWorldLine::IsTriInt(t4Vector* vertA, t4Vector* vertB, t4Vector* vertC, t4Vector* n, t4Vector* hit, t4Vector* outN,
                         float* outT)
{
    t4Vector* origin = (t4Vector*)(&p0);
    t4Vector* dir = (t4Vector*)(&dp);
    float denom = vu0Dot_E4D0(*dir, *n);
    if (__builtin_fabsf(denom) < 1.000000013351432e-10f) {
        return 0;
    }
    float lineT;
    {
        t4Vector toVertA = vu0Sub_E4D0(*vertA, *origin);
        lineT = vu0Dot_E4D0(toVertA, *n) / denom;
    }
    *outT = lineT;
    if (lineT < 0.0f || 1.0f < lineT) {
        return 0;
    }
    *hit = vu0Add_E4D0(*origin, vu0Scale_E4D0(*dir, lineT));
    *outN = *n;
    return gIsPointInsideTri_tol(vertA, vertB, vertC, hit, 9.999999747378752e-05f);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_Is2SidedTriInt);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// cWorldLine_vtbl slot 4 (IsTriInt).
typedef sVEntry<int(void*, t4Vector*, t4Vector*, t4Vector*, t4Vector*, t4Vector*, t4Vector*, void*)> tIsTriIntSlot;

/**
 * Intersects the line with a two-sided triangle: the one-sided test with the normal flipped to face the line.
 *
 * When dir . @p normal < 0 it calls the IsTriInt virtual (slot 4) with -@p normal, otherwise with @p normal.
 *
 * @param vertA  the triangle's first corner (On Tour a)
 * @param vertB  the second corner
 * @param vertC  the third corner
 * @param normal  the triangle's normal (On Tour n)
 * @param ipos  receives the intersection point (On Tour ipos)
 * @param inorm  receives the hit normal (On Tour inorm)
 * @param hitParam  receives the line parameter (On Tour param)
 * @return the IsTriInt result
 */
int cWorldLine::Is2SidedTriInt(t4Vector* vertA, t4Vector* vertB, t4Vector* vertC, t4Vector* normal, t4Vector* ipos,
                               t4Vector* inorm, void* hitParam)
{
    float dp = vu0Dot_E4D0(this->dp, *normal);
    tIsTriIntSlot* vt;
    if (dp < 0.0f) {
        vt = (tIsTriIntSlot*)mVtbl;
        void* target = vthis(this, vt[4]);
        t4Vector n = vu0Scale_E4D0(*normal, -1.0f);
        return vt[4].fn(target, vertA, vertB, vertC, &n, ipos, inorm, hitParam);
    }
    vt = (tIsTriIntSlot*)mVtbl;
    return vt[4].fn(vthis(this, vt[4]), vertA, vertB, vertC, normal, ipos, inorm, hitParam);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_IsSphTreeInt__FPv);
#ifdef SKIP_ASM
/**
 * Intersects the line with a sphere tree: not supported for lines, always 0 (no hits).
 *
 * @return always 0
 */
int cWorldLine::IsSphTreeInt()
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_IsBoxInt);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A5978;
extern t4Vector cWorldLine_IsBoxInt_norms[];
extern t4Vector t4vect_Zero_E690[] __asm__("t4vect_Zero");


static inline void reset_E690(tIntersectInfo* h)
{
    h->HitType = 2;
    h->Patch = 0;
    h->Inst = 0;
    h->Spline = 0;
    h->Mesh = -1;
    h->Face = -1;
    h->Seg = -1;
    h->Stype = -1;
    (h->Ivel) = *t4vect_Zero_E690;
    (h->Iangvel) = *t4vect_Zero_E690;
    h->T = 0;
    h->u = 0;
    h->v = 0;
}

struct sRay_E690 {
    char pad0[0x60];
    t4Vector origin;     // 0x60
    t4Vector dir;        // 0x70
};

/**
 * Intersects the line's segment with an axis-aligned box and records the entry and exit points.
 *
 * A slab test over t in [0, 1] (tmin starts at -1, tmax at 2); an axis whose direction is under 1e-10 is skipped
 * entirely, so a segment parallel to a face but outside that slab is not rejected. The face normals come from a
 * table cWorldLine_IsBoxInt_norms built on first use (flag D_004A5978). A hit is written for the entry (tmin >= 0) and for the
 * exit (tmax < 1): normal, point and t.
 *
 * @param bmin  the box's minimum corner (On Tour bMin)
 * @param bmax  the box's maximum corner (On Tour bMax)
 * @param hits  receives up to 2 intersections (On Tour tIntersectInfo* ti); On Tour's max count is not passed
 * @return the number of hits written (0-2)
 */
int cWorldLine::IsBoxInt(float* bmin, float* bmax, tIntersectInfo* hits)
{
    float tmin = -1.0f;
    float tmax = 2.0f;
    float* pd = &dp.x;
    float* po = &p0.x;
    int fmin = -1;
    int fmax = -1;
    for (int i = 0; i < 3; i++, bmin++, bmax++, pd++, po++) {
        float dirComp = *pd;
        if (__builtin_fabsf(dirComp) > 1.000000013351432e-10f) {
            float originComp = *po;
            float tNear = (*bmin - originComp) / dirComp;
            float tFar = (*bmax - originComp) / dirComp;
            int face = i * 2;
            if (tFar < tNear) {
                float tt = tNear;
                face++;
                tNear = tFar;
                tFar = tt;
            }
            if (tmin < tNear) {
                tmin = tNear;
                fmin = face;
            }
            if (tFar < tmax) {
                tmax = tFar;
                fmax = face ^ 1;
            }
            if (tmax < tmin)
                return 0;
            if (tmax < 0.0f)
                return 0;
            if (1.0f < tmin)
                return 0;
        }
    }
    if (D_004A5978 == 0) {
        cWorldLine_IsBoxInt_norms[0].x = -1.0f;
        cWorldLine_IsBoxInt_norms[0].y = 0.0f;
        cWorldLine_IsBoxInt_norms[0].z = 0.0f;
        cWorldLine_IsBoxInt_norms[0].w = 0.0f;
        cWorldLine_IsBoxInt_norms[1].x = 1.0f;
        cWorldLine_IsBoxInt_norms[1].y = 0.0f;
        cWorldLine_IsBoxInt_norms[1].z = 0.0f;
        cWorldLine_IsBoxInt_norms[1].w = 0.0f;
        cWorldLine_IsBoxInt_norms[2].x = 0.0f;
        cWorldLine_IsBoxInt_norms[2].y = -1.0f;
        cWorldLine_IsBoxInt_norms[2].z = 0.0f;
        cWorldLine_IsBoxInt_norms[2].w = 0.0f;
        cWorldLine_IsBoxInt_norms[3].x = 0.0f;
        cWorldLine_IsBoxInt_norms[3].y = 1.0f;
        cWorldLine_IsBoxInt_norms[3].z = 0.0f;
        cWorldLine_IsBoxInt_norms[3].w = 0.0f;
        cWorldLine_IsBoxInt_norms[4].x = 0.0f;
        cWorldLine_IsBoxInt_norms[4].y = 0.0f;
        cWorldLine_IsBoxInt_norms[4].z = -1.0f;
        cWorldLine_IsBoxInt_norms[4].w = 0.0f;
        cWorldLine_IsBoxInt_norms[5].x = 0.0f;
        cWorldLine_IsBoxInt_norms[5].y = 0.0f;
        cWorldLine_IsBoxInt_norms[5].z = 1.0f;
        cWorldLine_IsBoxInt_norms[5].w = 0.0f;
        D_004A5978 = 1;
    }
    int n = 0;
    if (0.0f <= tmin) {
        tIntersectInfo* entryHit = &hits[0];
        reset_E690(entryHit);
        (entryHit->Inorm) = cWorldLine_IsBoxInt_norms[fmin];
        (entryHit->Ipos) = vu0Add_E4D0(p0, vu0Scale_E4D0(dp, tmin));
        entryHit->Iparam = tmin;
        n = 1;
    }
    if (tmax < 1.0f) {
        reset_E690(&hits[n]);
        (hits[n].Inorm) = cWorldLine_IsBoxInt_norms[fmax];
        (hits[n].Ipos) = vu0Add_E4D0(p0, vu0Scale_E4D0(dp, tmax));
        hits[n].Iparam = tmax;
        n++;
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_IntersectSubPatch);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sLine_E9A0 {
    char pad_0x00[0x60];
    t4Vector origin; // 0x60
    t4Vector dir;    // 0x70
};

// bicubic patch: one 4x4 coefficient matrix per coordinate
struct sPatch_E9A0 {
    char pad_0x00[0x40];
    t4Vector m[4][4]; // 0x40
};

struct sVec4_E9A0 : t4Vector {
    sVec4_E9A0(const float& a, const float& b, const float& c, const float& d)
    {
        x = a;
        y = b;
        z = c;
        w = d;
    }
    // direction: w = 0
    sVec4_E9A0(const float& a, const float& b, const float& c)
    {
        x = a;
        y = b;
        z = c;
        w = 0.0f;
    }
};

static inline float Elem_E9A0(const t4Vector& v, int i)
{
    return (&v.x)[i];
}

// component k of the four coefficient vectors
static inline sVec4_E9A0 Column_E9A0(const t4Vector* c, int k)
{
    return sVec4_E9A0(Elem_E9A0(c[0], k), Elem_E9A0(c[1], k), Elem_E9A0(c[2], k), Elem_E9A0(c[3], k));
}

struct sVec3_E9A0 {
    float x, y, z;
    sVec3_E9A0() {}
    sVec3_E9A0(const t4Vector& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }
};

// 3x3 matrix, column-major
struct sMat3_E9A0 {
    float m[9];
    sMat3_E9A0 operator*(float s) const
    {
        sMat3_E9A0 r;
        r.m[0] = m[0] * s;
        r.m[1] = m[1] * s;
        r.m[2] = m[2] * s;
        r.m[3] = m[3] * s;
        r.m[4] = m[4] * s;
        r.m[5] = m[5] * s;
        r.m[6] = m[6] * s;
        r.m[7] = m[7] * s;
        r.m[8] = m[8] * s;
        return r;
    }
};

static inline sVec3_E9A0 operator*(const sMat3_E9A0& a, const sVec3_E9A0& v)
{
    sVec3_E9A0 r;
    r.x = a.m[0] * v.x + a.m[3] * v.y + a.m[6] * v.z;
    r.y = a.m[1] * v.x + a.m[4] * v.y + a.m[7] * v.z;
    r.z = a.m[2] * v.x + a.m[5] * v.y + a.m[8] * v.z;
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_E9A0(const t4Vector& a, const t4Vector& b)
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

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector vu0Cross_E9A0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector vu0Normalize_E9A0(const t4Vector& v)
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
        : "m"(v));
    return r;
}

/**
 * Intersects the line with one bicubic patch by four Newton steps from a starting (u, v).
 *
 * Each step solves the 3x3 system [dP/du dP/dv -dir] * delta = origin - P(u, v) (inverse by adjugate) and moves u
 * and v; if either leaves [0, 1] it returns without writing a hit. After four steps it writes P(u, v) and the
 * normalised cross(dP/dv, dP/du). The line parameter (delta.z) is not kept (On Tour returns it in param).
 *
 * @param line  the world line (On Tour this)
 * @param patch  the patch's four basis-multiplied control matrices (On Tour tPatchRecord* pPatch)
 * @param patchU  in: the starting patch coordinate, out: the refined one (On Tour s)
 * @param patchV  in: the starting patch coordinate, out: the refined one (On Tour t)
 * @param outPos  receives the hit point (On Tour ipos)
 * @param outNrm  receives the surface normal (On Tour inorm)
 */
// Intersect the line with a bicubic patch: four Newton steps from (u, v). Writes the hit point and the
// surface normal only if (u, v) stays inside the unit square.
extern "C" void cWorldLine_IntersectSubPatch(cWorldLine* line, sPatch_E9A0* patch, float& patchU, float& patchV, t4Vector* outPos,
                              t4Vector* outNrm)
{
    t4Vector* mat0 = patch->m[0];
    t4Vector* mat1 = patch->m[1];
    t4Vector* mat2 = patch->m[2];
    t4Vector* mat3 = patch->m[3];
    t4Vector surfPos;
    t4Vector rowVecs[4];
    float vSquared = patchV * patchV;
    float vCubed = patchV * vSquared;
    float uSquared = patchU * patchU;
    float uCubed = patchU * uSquared;
    sVec4_E9A0 vPowers(vCubed, vSquared, patchV, 1.0f);
    sVec4_E9A0 uPowers(uCubed, uSquared, patchU, 1.0f);
    rowVecs[0] = mtxMulVec_9B90(mat0, &uPowers);
    rowVecs[1] = mtxMulVec_9B90(mat1, &uPowers);
    rowVecs[2] = mtxMulVec_9B90(mat2, &uPowers);
    rowVecs[3] = mtxMulVec_9B90(mat3, &uPowers);
    surfPos = sVec4_E9A0(vu0Dot_E4D0(Column_E9A0(rowVecs, 0), vPowers), vu0Dot_E4D0(Column_E9A0(rowVecs, 1), vPowers),
                   vu0Dot_E4D0(Column_E9A0(rowVecs, 2), vPowers), 1.0f);
    sVec4_E9A0 dV(3.0f * vSquared, 2.0f * patchV, 1.0f);
    sVec4_E9A0 dU(3.0f * uSquared, 2.0f * patchU, 1.0f);
    float dvx = vu0Dot_E4D0(dV, Column_E9A0(rowVecs, 0));
    float dvy = vu0Dot_E4D0(dV, Column_E9A0(rowVecs, 1));
    float dvz = vu0Dot_E4D0(dV, Column_E9A0(rowVecs, 2));
    rowVecs[0] = mtxMulVec_9B90(mat0, &dU);
    rowVecs[1] = mtxMulVec_9B90(mat1, &dU);
    rowVecs[2] = mtxMulVec_9B90(mat2, &dU);
    rowVecs[3] = mtxMulVec_9B90(mat3, &dU);
    float dux = vu0Dot_E4D0(vPowers, Column_E9A0(rowVecs, 0));
    float duy = vu0Dot_E4D0(vPowers, Column_E9A0(rowVecs, 1));
    float duz = vu0Dot_E4D0(vPowers, Column_E9A0(rowVecs, 2));
    for (int i = 0; i < 4; i++) {
        sVec3_E9A0 originDelta(vu0Sub_E9A0(line->p0, surfPos));
        // Jacobian [dP/du dP/dv -dir], filled row by row
        sMat3_E9A0 jacobian;
        jacobian.m[0] = dux;
        jacobian.m[3] = dvx;
        jacobian.m[6] = -line->dp.x;
        jacobian.m[1] = duy;
        jacobian.m[4] = dvy;
        jacobian.m[7] = -line->dp.y;
        jacobian.m[2] = duz;
        jacobian.m[5] = dvz;
        jacobian.m[8] = -line->dp.z;
        {
            sMat3_E9A0 adj;
            adj.m[0] = jacobian.m[4] * jacobian.m[8] - jacobian.m[5] * jacobian.m[7];
            adj.m[1] = jacobian.m[7] * jacobian.m[2] - jacobian.m[8] * jacobian.m[1];
            adj.m[2] = jacobian.m[1] * jacobian.m[5] - jacobian.m[2] * jacobian.m[4];
            adj.m[3] = jacobian.m[6] * jacobian.m[5] - jacobian.m[8] * jacobian.m[3];
            adj.m[4] = jacobian.m[0] * jacobian.m[8] - jacobian.m[2] * jacobian.m[6];
            adj.m[5] = jacobian.m[3] * jacobian.m[2] - jacobian.m[5] * jacobian.m[0];
            adj.m[6] = jacobian.m[3] * jacobian.m[7] - jacobian.m[4] * jacobian.m[6];
            adj.m[7] = jacobian.m[6] * jacobian.m[1] - jacobian.m[7] * jacobian.m[0];
            adj.m[8] = jacobian.m[0] * jacobian.m[4] - jacobian.m[1] * jacobian.m[3];
            float det = jacobian.m[0] * adj.m[0] + jacobian.m[3] * adj.m[1] + jacobian.m[6] * adj.m[2];
            jacobian = adj * (1.0f / det);
        }
        sVec3_E9A0 delta = jacobian * originDelta;
        patchU += delta.x;
        patchV += delta.y;
        if (patchU < 0.0f || 1.0f < patchU || patchV < 0.0f || 1.0f < patchV) {
            return;
        }
        vSquared = patchV * patchV;
        vCubed = patchV * vSquared;
        uSquared = patchU * patchU;
        uCubed = patchU * uSquared;
        vPowers = sVec4_E9A0(vCubed, vSquared, patchV, 1.0f);
        uPowers = sVec4_E9A0(uCubed, uSquared, patchU, 1.0f);
        rowVecs[0] = mtxMulVec_9B90(mat0, &uPowers);
        rowVecs[1] = mtxMulVec_9B90(mat1, &uPowers);
        rowVecs[2] = mtxMulVec_9B90(mat2, &uPowers);
        rowVecs[3] = mtxMulVec_9B90(mat3, &uPowers);
        surfPos = sVec4_E9A0(vu0Dot_E4D0(Column_E9A0(rowVecs, 0), vPowers), vu0Dot_E4D0(Column_E9A0(rowVecs, 1), vPowers),
                       vu0Dot_E4D0(Column_E9A0(rowVecs, 2), vPowers), 1.0f);
        dV = sVec4_E9A0(3.0f * vSquared, 2.0f * patchV, 1.0f);
        dU = sVec4_E9A0(3.0f * uSquared, 2.0f * patchU, 1.0f);
        dvx = vu0Dot_E4D0(dV, Column_E9A0(rowVecs, 0));
        dvy = vu0Dot_E4D0(dV, Column_E9A0(rowVecs, 1));
        dvz = vu0Dot_E4D0(dV, Column_E9A0(rowVecs, 2));
        rowVecs[0] = mtxMulVec_9B90(mat0, &dU);
        rowVecs[1] = mtxMulVec_9B90(mat1, &dU);
        rowVecs[2] = mtxMulVec_9B90(mat2, &dU);
        rowVecs[3] = mtxMulVec_9B90(mat3, &dU);
        dux = vu0Dot_E4D0(vPowers, Column_E9A0(rowVecs, 0));
        duy = vu0Dot_E4D0(vPowers, Column_E9A0(rowVecs, 1));
        duz = vu0Dot_E4D0(vPowers, Column_E9A0(rowVecs, 2));
    }
    sVec4_E9A0 du(dux, duy, duz);
    sVec4_E9A0 dv(dvx, dvy, dvz);
    t4Vector n = vu0Normalize_E9A0(vu0Cross_E9A0(dv, du));
    *outPos = surfPos;
    *outNrm = n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_cWorldRider);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
static inline t4Vector vecAdd_2F650(t4Vector* a, t4Vector* b)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(*a), "m"(*b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (v * s).
static inline t4Vector vecScale_2F650(const t4Vector& v, float s)
{
    t4Vector r;
    int t;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}


cRiderSphereTree* cRiderSphereTree_cRiderSphereTree(cRiderSphereTree* self);
extern "C" void cRiderSphereTree_GetMaskBoundBox(cRiderSphereTree* tree, t4Vector* mn, t4Vector* mx);
extern void* cWorldRider_vtbl[];

/**
 * Wraps a rider's collision spheres as a world intersection object.
 *
 * Installs cWorldRider_vtbl, constructs the embedded sphere tree, stores @p tree twice, sets the state from
 * @p flag (1 when it is 0) and caches the masked bounding box (cRiderSphereTree_GetMaskBoundBox) and its centre.
 *
 * @param tree  the rider's sphere tree
 * @param flag  On Tour isLow
 * @return @p self
 */
extern "C" cWorldRider* cWorldRider_cWorldRider(cWorldRider* self, cRiderSphereTree* tree, int flag)
{
    self->useRejectNormal = 0;
    self->isTransformed = 0;
    self->mVtbl = cWorldRider_vtbl;
    cRiderSphereTree_cRiderSphereTree(&self->localTree);
    self->origSt = (cRiderSphereTree*)((void*)tree);
    self->st = (cRiderSphereTree*)((void*)tree);
    self->mPrecision = flag == 0;
    tree->GetMaskBoundBox((&self->boxMin), (&self->boxMax));
    *(&self->boxCenter) = vecScale_2F650(vecAdd_2F650((&self->boxMin), (&self->boxMax)), 0.5f);
    return (self);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldRider__cWorldRider);
#ifdef SKIP_ASM
extern "C" void cRiderSphereTree__cRiderSphereTree(cRiderSphereTree* p, int flags);
void operator_delete(int* ptr);
extern void* cWorldRider_vtbl[];

/**
 * Destroys a rider collision primitive: restores cWorldRider_vtbl (+0x50), destroys its sphere-tree copy (+0x70)
 * and, if asked, frees it.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cWorldRider__cWorldRider(cWorldRider* self, int flags)
{
    self->mVtbl = cWorldRider_vtbl;
    cRiderSphereTree__cRiderSphereTree(&self->localTree, 2);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_IsInBoxTransformed);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float subs).
static inline t4Vector sphereVecSub(t4Vector* a, t4Vector* b)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(*a), "m"(*b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
static inline t4Vector sphereVecAddR(t4Vector* a, t4Vector* b)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        ".set pop\n"
        : "=m"(r)
        : "m"(*a), "m"(*b)
        : "memory");
    return r;
}

/**
 * Tells whether the rider's current sphere tree's root sphere overlaps an axis-aligned box.
 *
 * Uses the root (+0x60) centre +/- its radius as the sphere's bounding box.
 *
 * @param mn  the box's minimum corner (On Tour bMin)
 * @param mx  the box's maximum corner (On Tour bMax)
 * @return 1 when the boxes overlap, otherwise 0
 */
// sphere (self+0x60) overlaps the box [mn, mx]?
int cWorldRider::IsInBoxTransformed(t4Vector* mn, t4Vector* mx)
{
    t4Vector rv;
    int inside = 0;
    float rad = st->boundRad;
    rv.x = rad;
    rv.y = rad;
    rv.z = rad;
    rv.w = 0;
    t4Vector lo = sphereVecSub(&st->boundCen, &rv);
    t4Vector hi = sphereVecAddR(&st->boundCen, &rv);
    if (mx->x >= lo.x && mn->x <= hi.x && mx->y >= lo.y && mn->y <= hi.y &&
        lo.z <= mx->z && mn->z <= hi.z) {
        inside = 1;
    }
    return inside;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_Transform);
#ifdef SKIP_ASM
struct sSphereNode_9A28;
extern "C" cRiderSphereTree* cRiderSphereTree_operatorE(cRiderSphereTree* self, cRiderSphereTree* src);
extern "C" void cRiderSphereTree_ApplyTransform(cRiderSphereTree* s, t4Matrix* m, float scale);

/**
 * Transforms the rider's sphere tree into a model's space using its own copy.
 *
 * Copies the current tree (+0x60) into the embedded copy at +0x70 (cRiderSphereTree_operatorE, On Tour
 * cRiderSphereTree::operator=), makes the copy current, applies @p mat and @p scale (cRiderSphereTree_ApplyTransform)
 * and sets the transformed flag (+0x4).
 *
 * @param mat  the transform (On Tour const t4Matrix& mat)
 * @param scale  the uniform scale
 */
void cWorldRider::Transform(t4Matrix* mat, float scale)
{
    cRiderSphereTree* node = &localTree;
    node->operatorE(st);
    st = node;
    node->ApplyTransform(mat, scale);
    isTransformed = 1;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_UnTransform__FPv);
#ifdef SKIP_ASM
/**
 * Undoes cWorldRider_Transform: makes the original tree (+0x64) current again (+0x60) and clears the transformed
 * flag.
 *
 * @return the original tree pointer (declared int; On Tour returns void)
 */
int cWorldRider::UnTransform()
{
    int origTree = (int)origSt;
    isTransformed = 0;
    st = (cRiderSphereTree*)origTree;
    return origTree;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_GetIdealParam);
#ifdef SKIP_ASM
/**
 * Returns the diameter of the rider's collision sphere tree, or -1 when the object is inactive.
 *
 * The diameter is twice the radius of the root sphere that offset 0x60 points to; the inactive case is a zero
 * first word.
 *
 * @return 2 * the root sphere's radius, or -1.0 if the first word is 0
 */
float cWorldRider::GetIdealParam()
{
    if (*(int*)this == 0) {
        return -1.0f;
    }
    cRiderSphereTree* sphere = st;
    return sphere->boundRad * 2.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_IsTriInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sphereLength_32F8F0(const t4Vector& v)
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

extern "C" bool cRiderSphereTree_IsTriCollision(void* tree, int a1, int a2, int a3, int a4, t4Vector* out, int a6);

/**
 * Collides the rider's sphere tree with a triangle and returns the push-out direction and depth.
 *
 * Calls cRiderSphereTree_IsTriCollision (On Tour cRiderSphereTree::IsTriCollision) on the current tree; on a hit the penetration
 * vector is split into its length and unit direction.
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
int cWorldRider::IsTriInt(int vertA, int vertB, int vertC, int n, int ipos, t4Vector* outDir, float* outLen)
{
    t4Vector overlap;
    int result = cRiderSphereTree_IsTriCollision(st, vertA, vertB, vertC, n, &overlap, ipos);
    if (result != 0) {
        float len = sphereLength_32F8F0(overlap);
        *outLen = len;
        *outDir = sphereCapsuleDiv(overlap, len);
    }
    return result;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_Is2SidedTriInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sphereLength_32F990(const t4Vector& v)
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

extern "C" bool cRiderSphereTree_Is2SidedTriCollision(void* tree, int a1, int a2, int a3, int a4, t4Vector* out,
        int a6);

/**
 * Collides the rider's sphere tree with a two-sided triangle and returns the push-out direction and depth.
 *
 * As cWorldRider_IsTriInt with cRiderSphereTree_Is2SidedTriCollision (On Tour cRiderSphereTree::Is2SidedTriCollision).
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
int cWorldRider::Is2SidedTriInt(int vertA, int vertB, int vertC, int n, int ipos, t4Vector* outDir, float* outLen)
{
    t4Vector overlap;
    int result = cRiderSphereTree_Is2SidedTriCollision(st, vertA, vertB, vertC, n, &overlap, ipos);
    if (result != 0) {
        float len = sphereLength_32F990(overlap);
        *outLen = len;
        *outDir = sphereCapsuleDiv(overlap, len);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_IsSphTreeInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sphereLength_32FA30(const t4Vector& v)
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
static inline void sphereDivEq_32FA30(t4Vector& v, float s)
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


extern "C" bool bxSphereTree_IsRiderTreeCollision(void* a, void* tree, t4Vector* dir, tIntersectInfo* hit);

/**
 * Collides another sphere tree with the rider's current tree and normalises the hit's push-out vector.
 *
 * On a hit (bxSphereTree_IsRiderTreeCollision) the hit's direction is divided by its length, which goes to the hit's length field.
 *
 * @param tree  the other sphere tree (On Tour bxSphereTree* tree)
 * @param hit  receives the intersection (On Tour tIntersectInfo* ti)
 * @return 1 on a hit, otherwise 0
 */
int cWorldRider::IsSphTreeInt(void* tree, tIntersectInfo* hit)
{
    if (bxSphereTree_IsRiderTreeCollision(tree, st, &(hit->Inorm), hit) != 0) {
        float len = sphereLength_32FA30((hit->Inorm));
        hit->Iparam = len;
        sphereDivEq_32FA30((hit->Inorm), len);
        return 1;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_IsBoxInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sphereLength_32FAC0(const t4Vector& v)
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
static inline void sphereDivEq_32FAC0(t4Vector& v, float s)
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


extern "C" bool cRiderSphereTree_IsBoxCollision(cRiderSphereTree* tree, t3Vector* a1, t3Vector* a2, t4Vector* dir,
        tIntersectInfo* hit);

/**
 * Collides the rider's current sphere tree with an axis-aligned box and normalises the hit's push-out vector.
 *
 * @param bMin  the box's minimum corner (On Tour bMin)
 * @param bMax  the box's maximum corner (On Tour bMax)
 * @param hit  receives the intersection (On Tour tIntersectInfo* ti); direction normalised, length stored
 * @return 1 on a hit (cRiderSphereTree_IsBoxCollision), otherwise 0
 */
int cWorldRider::IsBoxInt(void* bMin, void* bMax, tIntersectInfo* hit)
{
    if (st->IsBoxCollision((t3Vector*)bMin, (t3Vector*)bMax, &(hit->Inorm), hit) != 0) {
        float len = sphereLength_32FAC0((hit->Inorm));
        hit->Iparam = len;
        sphereDivEq_32FAC0((hit->Inorm), len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_cWorldSphere);
#ifdef SKIP_ASM
extern char cWorldSphere_vtbl[];


/**
 * Constructs a world sphere query.
 *
 * Installs cWorldSphere_vtbl (+0x50), clears the flag words (+0x4, +0x8), sets the precision (+0x0) to 0 (low) when @p
 * flag is set, else 1, stores the centre (+0x60, +0x80) and radius (+0x70, +0x90), and builds the bounding box (centre
 * +/- radius, w = 1) with the centre as its centre (+0x40). Called by tAvalancheNode_calculate.
 *
 * @param sphereCenter  the centre (On Tour acen)
 * @param flag  nonzero for low precision (On Tour isLow)
 * @param sphereRadius  the radius (On Tour arad)
 * @return @p self
 */
extern "C" cWorldSphere* cWorldSphere_cWorldSphere(cWorldSphere* self, t4Vector* sphereCenter, int flag, float sphereRadius)
{
    t4Vector boxCorner;
    self->mVtbl = (void**)cWorldSphere_vtbl;
    self->useRejectNormal = 0;
    self->isTransformed = 0;
    if (flag != 0) {
        self->mPrecision = 0;
    } else {
        self->mPrecision = 1;
    }
    self->origCen = *sphereCenter;
    self->cen = self->origCen;
    self->rad = sphereRadius;
    self->origRad = sphereRadius;
    boxCorner.x = sphereCenter->x + sphereRadius;
    boxCorner.y = sphereCenter->y + sphereRadius;
    boxCorner.z = sphereCenter->z + sphereRadius;
    boxCorner.w = 1.0f;
    self->boxMax = boxCorner;
    boxCorner.x = sphereCenter->x - sphereRadius;
    boxCorner.y = sphereCenter->y - sphereRadius;
    boxCorner.z = sphereCenter->z - sphereRadius;
    self->boxMin = boxCorner;
    self->boxCenter = *sphereCenter;
    return (self);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_IsInBoxTransformed);
#ifdef SKIP_ASM
/**
 * Tells whether the transformed sphere (centre +0x60, radius +0x70) can touch an axis-aligned box.
 *
 * @param mn  the box's minimum corner (On Tour bMin)
 * @param mx  the box's maximum corner (On Tour bMax)
 * @return false when the sphere's bounding box lies wholly beyond one face, otherwise true
 */
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float subs).
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
// sphere (pos 0x60, radius 0x70) overlaps the box [mn, mx]?
bool cWorldSphere::IsInBoxTransformed(t4Vector* mn, t4Vector* mx)
{
    t4Vector rv;
    float rad = this->rad;
    rv.x = rad;
    rv.y = rad;
    rv.z = rad;
    rv.w = 0;
    t4Vector hi = sphereCapsuleAdd((t4Vector*)&cen, &rv);
    t4Vector lo = sphereCapsuleSub((t4Vector*)&cen, &rv);
    if ((mn->x > hi.x && lo.x < mn->x) || (mx->x < hi.x && mx->x < lo.x) ||
        (mn->y > hi.y && lo.y < mn->y) || (mx->y < hi.y && mx->y < lo.y) ||
        (mn->z > hi.z && lo.z < mn->z) || (mx->z < hi.z && mx->z < lo.z)) {
        return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_Transform);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
static inline t4Vector sphereMtxMulVec(t4Vector* m, t4Vector* v)
{
    t4Vector r;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
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
        ".set pop\n"
        : "=m"(r)
        : "m"(*v), "r"(m)
        : "memory");
    return r;
}

/**
 * Transforms the sphere into a model's space: centre = matrix times the base centre (+0x80), radius = scale times
 * the base radius (+0x90).
 *
 * Sets the transformed flag (+0x4).
 *
 * @param mat  the transform (On Tour const t4Matrix& mat)
 * @param scale  the uniform scale for the radius
 */
void cWorldSphere::Transform(t4Vector* mat, float scale)
{
    t4Vector newCenter = sphereMtxMulVec(mat, (t4Vector*)&origCen);
    isTransformed = 1;
    cen = newCenter;
    rad = scale * origRad;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_UnTransform);
#ifdef SKIP_ASM
/**
 * Undoes cWorldSphere_Transform: copies the base centre and radius back (+0x60, +0x70) and clears the transformed
 * flag.
 */
void cWorldSphere::UnTransform()
{
    cen = origCen;
    rad = origRad;
    isTransformed = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_IsSphTreeInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sphereLength_32FDB0(const t4Vector& v)
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
static inline void sphereDivEq_32FDB0(t4Vector& v, float s)
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


extern "C" int SphereTree_IsSphereCollision(SphereTree* self, t4Vector* a1, float f, t4Vector* a2, t4Vector* a3);

/**
 * Collides the sphere with a sphere tree and normalises the hit's push-out vector.
 *
 * @param tree  the sphere tree (On Tour bxSphereTree* tree)
 * @param hit  receives the intersection (On Tour tIntersectInfo* ti); direction normalised, length stored
 * @return 1 on a hit (SphereTree_IsSphereCollision with the centre and radius), otherwise 0
 */
int cWorldSphere::IsSphTreeInt(void* tree, tIntersectInfo* hit)
{
    if (((SphereTree*)tree)->IsSphereCollision((t4Vector*)&cen, rad, (t4Vector*)(&(hit->Inorm)),
            (t4Vector*)hit) != 0) {
        float len = sphereLength_32FDB0((hit->Inorm));
        hit->Iparam = len;
        sphereDivEq_32FDB0((hit->Inorm), len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_IsBoxInt);
#ifdef SKIP_ASM
extern "C" bool gIsBoxSphereInt(void* a, void* b, void* center, void* box, void* boxMin, void* boxMax, float radius);

/**
 * Collides the sphere with an axis-aligned box (gIsBoxSphereInt with the centre and radius).
 *
 * @param bMin  the box's minimum corner (On Tour bMin)
 * @param bMax  the box's maximum corner (On Tour bMax)
 * @param box  receives the intersection (On Tour tIntersectInfo* ti; its +0x10 and +0x40 parts are passed too)
 * @return 1 on a hit, otherwise 0
 */
int cWorldSphere::IsBoxInt(void* bMin, void* bMax, tIntersectInfo* box)
{
    return gIsBoxSphereInt(bMin, bMax, (char*)&cen, box, &box->Inorm, &box->Iparam,
                         rad) != 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_IsTriInt);
#ifdef SKIP_ASM
extern "C" bool cWorldSphere_isIntersectingPlane(cWorldSphere* self, t4Vector* plane, t4Vector* p, float* out);
extern "C" bool cWorldSphere_edgeSphereCollision(cWorldSphere* self, t4Vector* a, t4Vector* b, t4Vector* c);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector sphereScale_32FE78(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector sphereSub_32FE78(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

/**
 * Collides the sphere with a one-sided triangle: the plane test, then the face or the edges.
 *
 * cWorldSphere_isIntersectingPlane (On Tour isIntersectingPlane) gives the distance t to the plane; the centre projected onto the
 * plane goes to @p outPos. A hit is reported when that point lies inside the triangle (gIsPointInsideTri, tolerance
 * 1e-4) or the sphere touches an edge (cWorldSphere_edgeSphereCollision, On Tour edgeSphereCollision).
 *
 * @param vertA  the triangle's first corner (On Tour a)
 * @param vertB  the second corner
 * @param vertC  the third corner
 * @param plane  the triangle's normal (On Tour n)
 * @param outPos  receives the centre projected onto the plane (On Tour ipos)
 * @param outDir  receives the hit normal, @p plane (On Tour inorm)
 * @param outDist  receives the penetration depth, radius - t (On Tour param)
 * @return true on a hit, otherwise false
 */
// Like cWorldSphere_Is2SidedTriInt without the plane-side flip. The success tail is written out twice (jump2 merges the
// copies); the extra self reference keeps the target's register order for self and a.
bool cWorldSphere::IsTriInt(t4Vector* vertA, t4Vector* vertB, t4Vector* vertC, t4Vector* plane, t4Vector* outPos,
                           t4Vector* outDir, float* outDist)
{
    float dist;
    if (isIntersectingPlane(plane, vertA, &dist) == 0) {
        return false;
    }
    t4Vector hitPos = sphereSub_32FE78(cen, sphereScale_32FE78(*plane, dist));
    *outPos = hitPos;
    if (gIsPointInsideTri_tol(vertA, vertB, vertC, outPos, 9.999999747378752e-05f) != 0) {
        *outDir = *plane;
        *outDist = rad - dist;
        return true;
    }
    if (edgeSphereCollision(vertA, vertB, vertC) == 0)
        return false;
    *outDir = *plane;
    *outDist = rad - dist;
    return true;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_Is2SidedTriInt);
#ifdef SKIP_ASM
extern "C" bool cWorldSphere_isIntersectingPlane(cWorldSphere* self, t4Vector* plane, t4Vector* p, float* out);
extern "C" bool cWorldSphere_edgeSphereCollision(cWorldSphere* self, t4Vector* a, t4Vector* b, t4Vector* c);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4Vector sphereScale_32FFA0(const t4Vector& v, float s)
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
        : "m"(v), "f"(s));
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place vector times scalar).
static inline void sphereScaleEq_32FFA0(t4Vector& v, float s)
{
    int t;
    __asm__(
        "mfc1      %1, %2\n"
        "lqc2      $vf4, %0\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "+m"(v), "=&r"(t)
        : "f"(s));
}

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector sphereSub_32FFA0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

/**
 * Collides the sphere with a two-sided triangle: as cWorldSphere_IsTriInt, but a centre behind the plane uses the
 * flipped normal.
 *
 * @param vertA  the triangle's first corner (On Tour a)
 * @param vertB  the second corner
 * @param vertC  the third corner
 * @param plane  the triangle's normal (On Tour n)
 * @param outPos  receives the centre projected onto the plane (On Tour ipos)
 * @param outDir  receives the normal facing the centre (On Tour inorm)
 * @param outDist  receives the penetration depth, radius - |t| (On Tour param)
 * @return true on a hit, otherwise false
 */
bool cWorldSphere::Is2SidedTriInt(t4Vector* vertA, t4Vector* vertB, t4Vector* vertC, t4Vector* plane, t4Vector* outPos,
                                 t4Vector* outDir, float* outDist)
{
    t4Vector newNorm = *plane;
    float dist;
    if (isIntersectingPlane(plane, vertA, &dist) == 0) {
        return false;
    }
    if (dist < 0.0f) {
        dist = -dist;
        sphereScaleEq_32FFA0(newNorm, -1.0f);
    }
    t4Vector hitPos = sphereSub_32FFA0(cen, sphereScale_32FFA0(newNorm, dist));
    *outPos = hitPos;
    if (gIsPointInsideTri_tol(vertA, vertB, vertC, outPos, 9.999999747378752e-05f) != 0 || edgeSphereCollision(vertA,
            vertB, vertC) != 0) {
        *outDir = newNorm;
        *outDist = rad - dist;
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", planeDistance);
#ifdef SKIP_ASM
/**
 * Returns the plane constant of the plane through a point: -dot(@p lhs, @p rhs).
 *
 * A 4-component dot product on VU0. cWorldSphere_isIntersectingPlane adds it to the normal's dot with a sphere centre to get the signed
 * distance.
 *
 * @param lhs  the plane normal (On Tour norm)
 * @param rhs  a point on the plane (On Tour point)
 * @return the negated dot product
 */
// Returns -dot(*a, *b) (4-component dot product on VU0, macro mode).
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (-(ax*bx + ay*by + az*bz + aw*bw)).
extern "C" float planeDistance(t4Vector* lhs, t4Vector* rhs)
{
    float dot;
    __asm__ __volatile__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf3, 0x0(%1)\n"
        "lqc2      $vf5, 0x0(%2)\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %0, $vf4\n"
        ".set pop\n"
        : "=r"(dot)
        : "r"(lhs), "r"(rhs));
    return -dot;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", closestPointOnLine);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: callers use this as a by-value struct return (hidden out pointer in $4).
t4Vector closestPointOnLine_def(t4Vector* a, t4Vector* b, t4Vector* p) __asm__("closestPointOnLine");

/**
 * Returns the point of a line segment closest to a test point.
 *
 * Projects @p testPoint - @p lineStart onto the unit direction of @p lineEnd - @p lineStart: at or before 0 gives @p lineStart, at or beyond the segment's
 * length gives @p lineEnd, otherwise the projected point. Returned by value; called by cWorldSphere_edgeSphereCollision.
 *
 * @param lineStart  the segment's first end (On Tour lPoint1)
 * @param lineEnd  its second end (On Tour lPoint2)
 * @param testPoint  the test point (On Tour testPoint)
 * @return the closest point
 */
t4Vector closestPointOnLine_def(t4Vector* lineStart, t4Vector* lineEnd, t4Vector* testPoint)
{
    t4Vector toPoint = Sub_CDB0(*testPoint, *lineStart);
    t4Vector segment = Sub_CDB0(*lineEnd, *lineStart);
    t4Vector n = Normalize_CDB0(segment);
    float projLength = Dot_CDB0(n, toPoint);
    float len = sphereLength_32FDB0(segment);
    if (projLength <= 0.0f) {
        return *lineStart;
    } else if (len <= projLength) {
        return *lineEnd;
    } else {
        t4Vector projPoint = Add_CDB0(*lineStart, Scale_CDB0(n, projLength));
        return projPoint;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_edgeSphereCollision);
#ifdef SKIP_ASM
// PORT: closestPointOnLine is defined as (out, a, b, p) returning out; this caller uses it as a by-value
// struct return (hidden out pointer in $4), so bind that view to the same symbol.
t4Vector closestPointOnLine_v(t4Vector* a, t4Vector* b, t4Vector* p) __asm__("closestPointOnLine");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_30250(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__ __volatile__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float vu0Length_30250(const t4Vector& v)
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

/**
 * Tells whether a world sphere query touches any edge of a triangle.
 *
 * For each edge, finds the point closest to the sphere's centre (+0x60) with closestPointOnLine and returns 1 when it is
 * nearer than the radius (+0x70). Called by cWorldSphere_IsTriInt and cWorldSphere_Is2SidedTriInt.
 *
 * @param vert0  first corner (On Tour a)
 * @param vert1  second corner (On Tour b)
 * @param vert2  third corner (On Tour c)
 * @return true when an edge is within the radius, else false
 */
// does the sphere at self+0x60 (radius self+0x70) touch any edge of triangle a,b,c?
bool cWorldSphere::edgeSphereCollision(t4Vector* vert0, t4Vector* vert1, t4Vector* vert2)
{
    t4Vector pt;
    t4Vector tri[3];
    tri[0] = *vert0;
    tri[1] = *vert1;
    tri[2] = *vert2;
    for (int i = 0; i < 3; i++) {
        pt = closestPointOnLine_v(&tri[i], &tri[(i + 1) % 3], (t4Vector*)&cen);
        float dist = vu0Length_30250(vu0Sub_30250(cen, pt));
        if (dist < rad) {
            return true;
        }
    }
    return false;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_isIntersectingPlane);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float sphereDot_330360(const t4Vector& a, const t4Vector& b)
{
    float d;
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
        : "=f"(d), "=&r"(t)
        : "m"(a), "m"(b));
    return d;
}

// PORT: PS2-only inline asm (float absolute value).
static inline float sphereAbs_330360(float x)
{
    float r;
    __asm__("abs.s %0, %1" : "=f"(r) : "f"(x));
    return r;
}

/**
 * Tells whether a world sphere query crosses a plane, and returns its signed distance.
 *
 * *@p out = dot(@p plane, centre +0x60) + planeDistance(@p plane, @p point), the plane constant; it returns whether
 * |distance| < radius (+0x70). Called by cWorldSphere_IsTriInt and cWorldSphere_Is2SidedTriInt.
 *
 * @param plane  the plane normal (On Tour pNorm)
 * @param point  a point on the plane (On Tour pPoint)
 * @param out  out: the centre's signed distance (On Tour dist)
 * @return true when the sphere crosses the plane, else false
 */
bool cWorldSphere::isIntersectingPlane(t4Vector* plane, t4Vector* point, float* out)
{
    float planeConst = planeDistance(plane, point);
    float signedDist = sphereDot_330360(*plane, cen) + planeConst;
    *out = signedDist;
    return sphereAbs_330360(signedDist) < rad;
}
#endif

