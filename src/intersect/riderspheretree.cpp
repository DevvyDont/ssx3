#include "common.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

struct cRiderSphereTree {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
};

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_cRiderSphereTree__FP16cRiderSphereTree);
#ifdef SKIP_ASM
inline void* operator new[](unsigned int, void* p) { return p; }

struct sRiderSphere_329910 {
    float pos[4];
    sRiderSphere_329910() {}
};

/**
 * Constructs a rider sphere tree with its 20 spheres and no source assigned (both IDs -1).
 *
 * @return @p self
 */
cRiderSphereTree* cRiderSphereTree_cRiderSphereTree(cRiderSphereTree* self)
{
    new ((char*)self + 0x2C) sRiderSphere_329910[20];
    self->field_0x24 = -1;
    *(unsigned int*)&self->field_0x28 = 0xFFFFFFFF;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_00329938);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* func_00329A28_v(void* self) __asm__("func_00329A28");

// NOTE: the array's offset is unconstrained by the target: the empty-ctor loop is deleted
// (vec_mark), leaving only the dead `li 0x13; li -1` pair. 0x30 = sSphereNode children[].
extern "C" void* func_00329938(void* self)
{
    new ((char*)self + 0x30) sRiderSphere_329910[20];
    func_00329A28_v(self);
    return self;
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
extern "C" void cRiderSphereTree__cRiderSphereTree(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_00329998);
#ifdef SKIP_ASM
extern "C" void func_00329998(void* self, float radius)
{
    *(unsigned int*)((char*)self + 0x28) = 0xFFFFFFFF;
    *(int*)((char*)self + 0x2C) = 1;
    *(float*)((char*)self + 0x20) = radius;
    *(float*)((char*)self + 0x40) = radius;
    *(int*)((char*)self + 0x24) = -1;
    *(int*)((char*)self + 0x44) = -1;
    *(int*)((char*)self + 0x0) = 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_Init);
#ifdef SKIP_ASM
struct sSphereLeaf_99C8 {
    int type;                       // 0x00
    char pad_0x04[0x1C];
    float radius;                   // 0x20
    int user;                       // 0x24
    unsigned int parent;            // 0x28
    int count;                      // 0x2C
};

extern "C" void cRiderSphereTree_Init(sSphereLeaf_99C8* s, int type, int n, int* ids, float* radii,
                              int user, float radius)
{
    s->parent = 0xFFFFFFFF;
    s->count = n;
    for (int i = 0; i < s->count; i++) {
        *(int*)((char*)s + i * 0x20 + 0x44) = ids[i];
        *(float*)((char*)s + i * 0x20 + 0x40) = radii[i];
    }
    s->type = type;
    s->radius = radius;
    s->user = user;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_00329A28);
#ifdef SKIP_ASM
struct sVec4_9A28 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSphereNode_9A28 {
    int type;            // 0x00
    char pad_0x04[0xC];
    sVec4_9A28 center;   // 0x10
    float radius;        // 0x20
    int user;            // 0x24
    int parent;          // 0x28
    int count;           // 0x2C
    char children[1];    // 0x30
};

extern "C" void MEM_copy(void* dst, void* src, int size);

extern "C" sSphereNode_9A28* func_00329A28(sSphereNode_9A28* self, sSphereNode_9A28* src)
{
    self->type = src->type;
    self->radius = src->radius;
    self->center = src->center;
    self->user = src->user;
    self->count = src->count;
    self->parent = src->parent;
    MEM_copy(self->children, src->children, self->count << 5);
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_00329A90);
#ifdef SKIP_ASM
struct sSphereVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sSphereChild {
    sSphereVec4 pos;
    float radius;
    int pad[3];
};

struct sSphereTreeNode {
    char pad_0x00[0x10];
    sSphereVec4 center; // 0x10
    float radius;       // 0x20
    int pad_0x24;
    int pad_0x28;
    int count;          // 0x2C
    sSphereChild children[1]; // 0x30
};

extern "C" void func_00329A90(sSphereTreeNode* node, int n, sSphereVec4* pos, float* radii,
                              sSphereVec4* center, float radius)
{
    node->count = n;
    for (int i = 0; i < node->count; i++) {
        node->children[i].pos = pos[i];
        node->children[i].radius = radii[i];
    }
    node->center = *center;
    node->radius = radius;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_00329AE0);
#ifdef SKIP_ASM
struct sSphereSrc_9AE0 {
    sSphereVec4 pos;
    int pad[4];
};

extern "C" void func_00329AE0(sSphereTreeNode* s)
{
    sSphereSrc_9AE0** table = (sSphereSrc_9AE0**)s;
    for (int i = 0; i < s->count; i++) {
        s->children[i].pos = (*table)[s->children[i].pad[0]].pos;
    }
    s->center = (*table)[s->pad_0x24].pos;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_00329B40);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
static inline void sphereVecAdd(sSphereVec4* dst, sSphereVec4* v)
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

// self->center += *v, then every child's pos += *v
extern "C" void func_00329B40(sSphereTreeNode* node, sSphereVec4* offset)
{
    sphereVecAdd(&node->center, offset);
    for (int i = 0; i < node->count; i++) {
        sphereVecAdd(&node->children[i].pos, offset);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_ApplyTransform);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
static inline sSphereVec4 mtxMulVec_9B90(sSphereVec4* m, sSphereVec4* v)
{
    sSphereVec4 r;
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

struct sScale_9B90 {
    float x, y, z, w;
    sScale_9B90(float s) { x = s; y = s; z = s; w = 1.0f; }
};

struct sProd_9B90 : sSphereVec4 {
    sProd_9B90(const sSphereVec4& a, const sScale_9B90& b)
    {
        x = a.x * b.x;
        y = a.y * b.y;
        z = a.z * b.z;
        w = a.w * b.w;
    }
};

// PORT: callers declare the matrix as sSphereMtx*.
extern "C" void cRiderSphereTree_ApplyTransform_impl(sSphereTreeNode* s, sSphereVec4* m, float scale) __asm__("cRiderSphereTree_ApplyTransform");

/**
 * Moves a sphere tree node and its child spheres by a matrix, then scales their positions and radii.
 *
 * @param node  the sphere tree node
 * @param mat  the transform (On Tour const t4Matrix& mat)
 * @param scale  uniform scale applied after the transform
 */
extern "C" void cRiderSphereTree_ApplyTransform_impl(sSphereTreeNode* node, sSphereVec4* mat, float scale)
{
    sScale_9B90 sc(scale);
    node->center = sProd_9B90(mtxMulVec_9B90(mat, &node->center), sc);
    node->radius = node->radius * scale;
    for (int i = 0; i < node->count; i++) {
        node->children[i].pos = sProd_9B90(mtxMulVec_9B90(mat, &node->children[i].pos), sc);
        node->children[i].radius = node->children[i].radius * scale;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_00329CE0);
#ifdef SKIP_ASM
struct sSphereBox_9CE0 {
    sSphereVec4 mn;
    sSphereVec4 mx;
};

// Bounding box of all child spheres, seeded with the node's own center.
extern "C" void func_00329CE0(sSphereTreeNode* self, sSphereBox_9CE0* box)
{
    box->mn = box->mx = self->center;
    for (int i = 0; i < self->count; i++) {
        sSphereChild* child = &self->children[i];
        float childRadius = child->radius;
        float bound;
        bound = child->pos.x - childRadius;
        if (bound < box->mn.x)
            box->mn.x = bound;
        bound = child->pos.y - childRadius;
        if (bound < box->mn.y)
            box->mn.y = bound;
        bound = child->pos.z - childRadius;
        if (bound < box->mn.z)
            box->mn.z = bound;
        bound = child->pos.x + childRadius;
        if (box->mx.x < bound)
            box->mx.x = bound;
        bound = child->pos.y + childRadius;
        if (box->mx.y < bound)
            box->mx.y = bound;
        bound = child->pos.z + childRadius;
        if (box->mx.z < bound)
            box->mx.z = bound;
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

extern "C" void cRiderSphereTree_GetMaskBoundBox(void* tree, sVec4_2F650* mnp, sVec4_2F650* mxp)
{
    sSphereBox_9DC8* node = (sSphereBox_9DC8*)tree;
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
            sSphereV4_9DC8 r(node->kids[i].radius, node->kids[i].radius, node->kids[i].radius, 1.0f);
            sSphereV4_9DC8 t = Sub_9DC8(node->kids[i].pos, r);
            if (t.x < mn->x) mn->x = t.x;
            if (t.y < mn->y) mn->y = t.y;
            if (t.z < mn->z) mn->z = t.z;
            t = Add_9DC8(node->kids[i].pos, r);
            if (t.x > mx->x) mx->x = t.x;
            if (t.y > mx->y) mx->y = t.y;
            if (t.z > mx->z) mx->z = t.z;
        }
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_00329F98);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sElem_9F98 {
    sSphereVec4 pos;    // 0x00
    float r;            // 0x10
    char pad_0x14[0xC];
};

struct sNode_9F98 {
    char pad_0x00[0x10];
    sSphereVec4 pos;    // 0x10
    float r;            // 0x20
    char pad_0x24[0x8];
    int count;          // 0x2C
    sElem_9F98 e[1];    // 0x30
};

// PORT: PS2-only VU0 inline asm (a - b).
static inline sSphereVec4 Sub_9F98(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 Scale_9F98(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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
static inline float Dot_9F98(const sSphereVec4& a, const sSphereVec4& b)
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
static inline sSphereVec4 Normalize_9F98(const sSphereVec4& v)
{
    sSphereVec4 r;
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

extern "C" int func_00329F98(sNode_9F98* nodeA, sNode_9F98* nodeB, sSphereVec4* outA, sSphereVec4* outB)
{
    sSphereVec4 delta;
    float sum;
    float ss;
    float dd;
    sum = nodeB->r + nodeA->r;
    delta = Sub_9F98(nodeB->pos, nodeA->pos);
    ss = sum * sum;
    dd = Dot_9F98(delta, delta);
    if (ss < dd)
        return 0;
    for (int i = 0; i < nodeA->count; i++) {
        sum = nodeB->r + nodeA->e[i].r;
        delta = Sub_9F98(nodeB->pos, nodeA->e[i].pos);
        ss = sum * sum;
        dd = Dot_9F98(delta, delta);
        if (ss < dd)
            continue;
        for (int j = 0; j < nodeB->count; j++) {
            sum = nodeB->e[j].r + nodeA->e[i].r;
            delta = Sub_9F98(nodeB->e[j].pos, nodeA->e[i].pos);
            ss = sum * sum;
            dd = Dot_9F98(delta, delta);
            if (!(ss < dd)) {
                sSphereVec4 n = Normalize_9F98(delta);
                float overlap = sum - Sqrt_9F98(dd);
                *outA = Scale_9F98(n, overlap);
                *outB = Sub_9F98(nodeA->e[i].pos, Scale_9F98(n, nodeA->e[i].r - overlap));
                return 1;
            }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_IsTriCollision);
#ifdef SKIP_ASM
typedef sSphereVec4 sV_A1C0;

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

// PORT: the unit declares func_0032B6A8 with four pointer args; callers also pass a float tolerance in $f12.
extern "C" int func_0032B6A8_eps(sSphereVec4* a, sSphereVec4* b, sSphereVec4* c, sSphereVec4* d, float eps) __asm__("func_0032B6A8");

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
    if (func_0032B6A8_eps(p, a, b, q, 0.0001f) != 0) {
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

// PORT: the unit declares it later as (void*, int, int, int, int, sSphereVec4*, int); defined under its own name bound to the symbol.
extern "C" int cRiderSphereTree_IsTriCollision_impl(sNode_A1C0* self, sV_A1C0* p, sV_A1C0* a, sV_A1C0* b, sV_A1C0* n, sV_A1C0* outPush, sV_A1C0* outPoint) __asm__("cRiderSphereTree_IsTriCollision");
extern "C" int cRiderSphereTree_IsTriCollision_impl(sNode_A1C0* self, sV_A1C0* p, sV_A1C0* a, sV_A1C0* b, sV_A1C0* n, sV_A1C0* outPush, sV_A1C0* outPoint)
{
    float dist = Dot_A1C0(*n, Sub_A1C0(self->center, *p));
    float r = self->radius;
    if (r < __builtin_fabsf(dist)) {
        return 0;
    }
    sV_A1C0 d = Sub_A1C0(self->center, Scale_A1C0(*n, dist));
    if (!Touch_A1C0(self->center, r, p, a, b, &d)) {
        return 0;
    }
    if (self->mask == 0) {
        *outPush = Scale_A1C0(*n, r - dist);
        *outPoint = d;
        return 1;
    }
    float best = 10000000000.0f;
    for (int i = 0; i < self->count; i++) {
        if (self->mask & (1 << i)) {
            dist = Dot_A1C0(*n, Sub_A1C0(self->kids[i].pos, *p));
            r = self->kids[i].radius;
            if (__builtin_fabsf(dist) < r && dist < best) {
                d = Sub_A1C0(self->kids[i].pos, Scale_A1C0(*n, dist));
                if (Touch_A1C0(self->kids[i].pos, r, p, a, b, &d)) {
                    *outPush = Scale_A1C0(*n, r - dist);
                    *outPoint = d;
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
typedef sSphereVec4 sV_AA28;

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

// PORT: the unit declares func_0032B6A8 with four pointer args; callers also pass a float tolerance in $f12.
extern "C" int func_0032B6A8_eps(sSphereVec4* a, sSphereVec4* b, sSphereVec4* c, sSphereVec4* d, float eps) __asm__("func_0032B6A8");

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
    if (func_0032B6A8_eps(p, a, b, q, 0.0001f) != 0) {
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

// PORT: the unit declares it later as (void*, int, int, int, int, sSphereVec4*, int); defined under its own name bound to the symbol.
extern "C" int cRiderSphereTree_Is2SidedTriCollision_impl(sNode_AA28* self, sV_AA28* p, sV_AA28* a, sV_AA28* b, sV_AA28* np, sV_AA28* outPush, sV_AA28* outPoint) __asm__("cRiderSphereTree_Is2SidedTriCollision");
extern "C" int cRiderSphereTree_Is2SidedTriCollision_impl(sNode_AA28* self, sV_AA28* p, sV_AA28* a, sV_AA28* b, sV_AA28* np, sV_AA28* outPush, sV_AA28* outPoint)
{
    sV_AA28 n = *np;
    float dist = Dot_AA28(n, Sub_AA28(self->center, *p));
    float r = self->radius;
    if (r < __builtin_fabsf(dist)) {
        return 0;
    }
    if (dist < 0.0f) {
        dist = -dist;
        n = Scale_AA28(n, -1.0f);
    }
    sV_AA28 d = Sub_AA28(self->center, Scale_AA28(n, dist));
    if (!Touch_AA28(self->center, r, p, a, b, &d)) {
        return 0;
    }
    if (self->mask == 0) {
        *outPush = Scale_AA28(n, r - dist);
        *outPoint = d;
        return 1;
    }
    float best = 10000000000.0f;
    for (int i = 0; i < self->count; i++) {
        if (self->mask & (1 << i)) {
            dist = Dot_AA28(n, Sub_AA28(self->kids[i].pos, *p));
            r = self->kids[i].radius;
            if (dist < r && dist < best) {
                d = Sub_AA28(self->kids[i].pos, Scale_AA28(n, dist));
                if (Touch_AA28(self->kids[i].pos, r, p, a, b, &d)) {
                    *outPush = Scale_AA28(n, r - dist);
                    *outPoint = d;
                    best = dist;
                }
            }
        }
    }
    return best != 10000000000.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032B2B8);
#ifdef SKIP_ASM
extern "C" int func_00329590(void* a, void* b, void* center, void* box, void* boxMin, void* boxMax, float radius);

struct sSphereHit_B2B8 {
    char pad_0x00[0x10];
    sSphereVec4 center; // 0x10
    float radius;       // 0x20
    int pad_0x24;
    unsigned int mask;  // 0x28
    int count;          // 0x2C
};

// PORT: PS2-only VU0 inline asm (v * s).
static inline sSphereVec4 Scale_B2B8(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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

extern "C" int func_0032B2B8(void* tree, void* boxMin, void* boxMax, sSphereVec4* outPos, sSphereHit_32FAC0* hit)
{
    sSphereHit_B2B8* self = (sSphereHit_B2B8*)tree;
    sSphereVec4* outNormal = (sSphereVec4*)hit;
    sSphereVec4 normal;
    sSphereVec4 dir;
    float dist;
    if (func_00329590(boxMin, boxMax, &self->center, &normal, &dir, &dist, self->radius) == 0) {
        return 0;
    }
    if (self->mask == 0) {
        *outPos = Scale_B2B8(dir, dist);
        *outNormal = normal;
        return 1;
    }
    float best = -1.0f;
    for (int i = 0; i < self->count; i++) {
        if (self->mask & (1 << i)) {
            if (func_00329590(boxMin, boxMax, &self->center, &normal, &dir, &dist, self->radius) && best < dist) {
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
extern "C" void func_0032B460(void) {

}
#endif
//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032B468);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: the inner run loop wrapped in a non-empty `do { ... } while (0)`, and the stand-in local `one = 1` for the `1 < minRun` test (li + slt instead of slti); plain form 94.74% (97.65% without the wrapper only, 95.24% without `one` only). Byte-identical and behaves like the plain form; landed marked under
// the 2026-11-14 ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
extern "C" void MEM_fill(void* dst, int c, int n);

extern "C" int func_0032B468(unsigned char* start, unsigned char* in, int len, int minRun)
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
INCLUDE_ASM("intersect/riderspheretree", func_0032B620);
#ifdef SKIP_ASM
// Run-length decode (PackBits style) into dst: a negative count -n copies the next n bytes, a positive count n
// repeats the next byte n+1 times, 0 terminates. Returns the number of bytes written.
// PORT: the unit declares it later as void*(void*, void*); defined under its own name bound to the symbol.
extern "C" int func_0032B620_impl(char* dst, signed char* src) __asm__("func_0032B620");
extern "C" int func_0032B620_impl(char* dst, signed char* src)
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
INCLUDE_ASM("intersect/riderspheretree", func_0032B6A8);
#ifdef SKIP_ASM
// VU0 microprogram entry (micro-memory address 0x6E0; resolved via undefined_syms_auto.txt)
extern char D_6E0[];

// Loads four vectors into vf10-vf13, runs the VU0 microprogram at D_6E0 and
// returns whether it left a nonzero result in vi2.
// PORT: PS2-only VU0 microprogram call (ctc2/vcallmsr/cfc2); the PC port needs a C version
// of the microprogram.
extern "C" int func_0032B6A8(sSphereVec4* a, sSphereVec4* b, sSphereVec4* c, sSphereVec4* d)
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
        : "r"(a), "r"(b), "r"(c), "r"(d), "0"((int)D_6E0 >> 3));
    return hit != 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032B6E0);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sCell_B6E0 {
    sSphereVec4 n[2];   // 0x00 triangle normals (1e10 = not computed, 1e11 = degenerate)
    sSphereVec4 lo;     // 0x20 bounds (lo.x 1e10 = not computed)
    sSphereVec4 hi;     // 0x30
};

struct sPatch_B6E0 {
    char pad_0x00[0x10];
    sSphereVec4 verts[10][10];  // 0x0010
    sCell_B6E0 fine[9][9];      // 0x0650
    sCell_B6E0 coarse[3][3];    // 0x1A90
};

struct sVE_B6E0 {
    short delta;
    short pad;
    union {
        int (*test)(void* self, void* a, void* b, void* c, sSphereVec4* n, void* d, void* e, void* f);
        void (*uv)(void* self, void* obj, float* u, float* v, void* d, void* e, void* f);
    };
};

struct sQuery_B6E0 {
    int type;           // 0x00
    int pad_0x04;
    int facing;         // 0x08
    int pad_0x0C;
    sSphereVec4 dir;    // 0x10
    sSphereVec4 hi;     // 0x20
    sSphereVec4 lo;     // 0x30
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
static inline void Bounds_B6E0(sCell_B6E0* c, const sSphereVec4& a, const sSphereVec4& b,
                               const sSphereVec4& d, const sSphereVec4& e)
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
static inline float TriNormal_B6E0(sSphereVec4& n, const sSphereVec4& a, const sSphereVec4& b,
                                  const sSphereVec4& c)
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
static inline sSphereVec4& CellNormal_B6E0(sCell_B6E0* cell, int k, const sSphereVec4& a,
                                           const sSphereVec4& b, const sSphereVec4& c)
{
    if (cell->n[k].x == 10000000000.0f) {
        if (TriNormal_B6E0(cell->n[k], a, b, c) > 10.0f)
            cell->n[k].x = 99999997952.0f;
    }
    return cell->n[k];
}

// query against a 9x9-cell terrain patch (coarse 3x3 cells for type 0): tries the cached cell first,
// then every cell whose bounds overlap the query box; returns the hit cell's uv in uvOut
extern "C" int func_0032B6E0_typed(sQuery_B6E0* q, void* obj, sPatch_B6E0* patch, void* a3, void* a4,
                                   void* a5, float* uvOut, sHitCache_B6E0* cache) __asm__("func_0032B6E0");
extern "C" int func_0032B6E0_typed(sQuery_B6E0* query, void* obj, sPatch_B6E0* patch, void* a3, void* a4,
                                   void* a5, float* uvOut, sHitCache_B6E0* cache)
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
        sSphereVec4* corner00 = &patch->verts[cache->u * rowStep][cache->v * step];
        sSphereVec4* corner10 = corner00 + step * 10;
        sSphereVec4* corner01 = corner00 + step;
        sSphereVec4* corner11 = corner10 + step;
        if (cachedCell->lo.x == 10000000000.0f)
            Bounds_B6E0(cachedCell, *corner00, *corner10, *corner01, *corner11);
        if (cache->tri) {
            sSphereVec4& triNormal0 = CellNormal_B6E0(cachedCell, 0, *corner11, *corner10, *corner01);
            if (triNormal0.x != 99999997952.0f && (!query->facing || Dot_A1C0(triNormal0, query->dir) <= 0.8f)) {
                sVE_B6E0* vt = query->vt;
                if (vt[4].test((char*)query + vt[4].delta, corner11, corner10, corner01, &triNormal0, a3, a4, a5)) {
                    float hitU = cache->u * scale + half;
                    float hitV = cache->v * scale + half;
                    if (query->type == 2) {
                        sVE_B6E0* vt = query->vt;
                        vt[8].uv((char*)query + vt[8].delta, obj, &hitU, &hitV, a3, a4, a5);
                    }
                    uvOut[0] = hitU;
                    uvOut[1] = hitV;
                    return 1;
                }
            }
        } else {
            sSphereVec4& triNormal1 = CellNormal_B6E0(cachedCell, 1, *corner01, *corner10, *corner00);
            if (triNormal1.x != 99999997952.0f && (!query->facing || Dot_A1C0(triNormal1, query->dir) <= 0.8f)) {
                sVE_B6E0* vt = query->vt;
                if (vt[4].test((char*)query + vt[4].delta, corner01, corner10, corner00, &triNormal1, a3, a4, a5)) {
                    float hitU = cache->u * scale + half;
                    float hitV = cache->v * scale + half;
                    if (query->type == 2) {
                        sVE_B6E0* vt = query->vt;
                        vt[8].uv((char*)query + vt[8].delta, obj, &hitU, &hitV, a3, a4, a5);
                    }
                    uvOut[0] = hitU;
                    uvOut[1] = hitV;
                    return 1;
                }
            }
        }
        cache->obj = 0;
        cache->u = 0;
        cache->v = 0;
        cache->tri = 0;
        cache->type = 0;
    }
    sSphereVec4* corner00 = patch->verts[0];
    for (int cellU = 0; cellU < n; cellU++) {
        sSphereVec4* corner10 = corner00 + step * 10;
        sSphereVec4* corner01 = corner00 + step;
        sSphereVec4* corner11 = corner10 + step;
        for (int cellV = 0; cellV < n; cellV++) {
            if (cell->lo.x == 10000000000.0f)
                Bounds_B6E0(cell, *corner00, *corner10, *corner01, *corner11);
            if (Overlap_B6E0(cell, query)) {
                sSphereVec4& triNormal0 = CellNormal_B6E0(cell, 0, *corner11, *corner10, *corner01);
                if (triNormal0.x != 99999997952.0f && (!query->facing || Dot_A1C0(triNormal0, query->dir) <= 0.8f)) {
                    sVE_B6E0* vt = query->vt;
                    if (vt[4].test((char*)query + vt[4].delta, corner11, corner10, corner01, &triNormal0, a3, a4, a5)) {
                        float hitU = cellU * scale + half;
                        float hitV = cellV * scale + half;
                        if (query->type == 2) {
                            sVE_B6E0* vt = query->vt;
                            vt[8].uv((char*)query + vt[8].delta, obj, &hitU, &hitV, a3, a4, a5);
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
                        return 1;
                    }
                }
                sSphereVec4& triNormal1 = CellNormal_B6E0(cell, 1, *corner01, *corner10, *corner00);
                if (triNormal1.x != 99999997952.0f && (!query->facing || Dot_A1C0(triNormal1, query->dir) <= 0.8f)) {
                    sVE_B6E0* vt = query->vt;
                    if (vt[4].test((char*)query + vt[4].delta, corner01, corner10, corner00, &triNormal1, a3, a4, a5)) {
                        float hitU = cellU * scale + half;
                        float hitV = cellV * scale + half;
                        if (query->type == 2) {
                            sVE_B6E0* vt = query->vt;
                            vt[8].uv((char*)query + vt[8].delta, obj, &hitU, &hitV, a3, a4, a5);
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
                        return 1;
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
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldIntersectType_IsMeshInt);
#ifdef SKIP_ASM
struct sV4_C0F8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sV4_C0F8 t4vect_Zero;

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vDot_C0F8(const sV4_C0F8& a, const sV4_C0F8& b)
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

struct sTriBox_C0F8 {
    float lo[3];
    float hi[3];
};

struct sTriMesh_C0F8 {
    unsigned short numTris;     // 0x0
    short pad2;
    unsigned char* indices;     // 0x4
    sTriBox_C0F8* boxes;        // 0x8
    sV4_C0F8* verts;            // 0xC
    sV4_C0F8* normals;          // 0x10
};

struct sTriHit_C0F8 {
    sV4_C0F8 pos;       // 0x00
    sV4_C0F8 nrm;       // 0x10
    sV4_C0F8 v20;       // 0x20
    sV4_C0F8 v30;       // 0x30
    float t;            // 0x40
    int type;           // 0x44
    int f48;
    int f4C;            // 0x4C
    int f50;            // 0x50
    int f54;            // 0x54
    int f58;            // 0x58
    int f5C;            // 0x5C
    int tri;            // 0x60
    int f64;            // 0x64
    int f68;            // 0x68
    int f6C;            // 0x6C
    int f70;            // 0x70
    int pad[3];
};

struct sVE_C0F8a {
    short delta;
    short index;
    int (*fn)(void*, sV4_C0F8*, sV4_C0F8*, sV4_C0F8*, sV4_C0F8*, sV4_C0F8*, sV4_C0F8*, float*);
};
struct sVE_C0F8b {
    short delta;
    short index;
    int (*fn)(void*, float*, float*);
};

struct sTriTester_C0F8 {
    char pad0[0x8];
    int useNormal;          // 0x8
    char padC[0x4];
    sV4_C0F8 dir;           // 0x10
    char pad20[0x30];
    sVE_C0F8a* vt;          // 0x50
};

extern "C" int cWorldIntersectType_IsMeshInt(sTriTester_C0F8* self, sTriMesh_C0F8* mesh, int flag, sTriHit_C0F8* out)
{
    sV4_C0F8 pos;
    sV4_C0F8 hn;
    float t;
    int n = 0;
    if (mesh->numTris < 11) {
        for (int i = 0; i < mesh->numTris; i++) {
            sV4_C0F8* nrm = &mesh->normals[i];
            if (self->useNormal != 0 && vDot_C0F8(*nrm, self->dir) > 0.8f) {
                continue;
            }
                        sV4_C0F8* v = mesh->verts;
            sV4_C0F8* p0 = &v[mesh->indices[i * 3]];
            sV4_C0F8* p1 = &v[mesh->indices[i * 3 + 1]];
            sV4_C0F8* p2 = &v[mesh->indices[i * 3 + 2]];
            int r;
            if (flag != 0) {
                r = self->vt[5].fn((char*)self + self->vt[5].delta, p0, p1, p2, nrm, &pos, &hn, &t);
            } else {
                r = self->vt[4].fn((char*)self + self->vt[4].delta, p0, p1, p2, nrm, &pos, &hn, &t);
            }
            if (r == 0) {
                continue;
            }
            sTriHit_C0F8* h = &out[n];
            h->f54 = 0;
            h->type = 2;
            h->tri = -1;
            h->f50 = 0;
            h->f58 = 0;
            h->f5C = -1;
            h->f64 = -1;
            h->f4C = -1;
            h->v20 = t4vect_Zero;
            h->v30 = t4vect_Zero;
            h->f68 = 0;
            h->f6C = 0;
            h->f70 = 0;
            h->pos = pos;
            h->nrm = hn;
            h->t = t;
            h->tri = i;
            h->f54 = 0;
            n++;
        }
    } else {
        int nb = (mesh->numTris + 9) / 10;
        for (int b = 0; b < nb; b++) {
            sTriBox_C0F8* box = &mesh->boxes[b];
            sVE_C0F8b* vt = (sVE_C0F8b*)self->vt;
            if (!vt[9].fn((char*)self + vt[9].delta, box->lo, box->hi)) {
                continue;
            }
            int end = (b + 1) * 10 < mesh->numTris ? (b + 1) * 10 : mesh->numTris;
            for (int i = b * 10; i < end; i++) {
                sV4_C0F8* nrm = &mesh->normals[i];
                if (self->useNormal != 0 && vDot_C0F8(*nrm, self->dir) > 0.8f) {
                    continue;
                }
                                sV4_C0F8* v = mesh->verts;
                sV4_C0F8* p0 = &v[mesh->indices[i * 3]];
                sV4_C0F8* p1 = &v[mesh->indices[i * 3 + 1]];
                sV4_C0F8* p2 = &v[mesh->indices[i * 3 + 2]];
                int r;
                if (flag != 0) {
                    r = self->vt[5].fn((char*)self + self->vt[5].delta, p0, p1, p2, nrm, &pos, &hn, &t);
                } else {
                    r = self->vt[4].fn((char*)self + self->vt[4].delta, p0, p1, p2, nrm, &pos, &hn, &t);
                }
                if (r == 0) {
                    continue;
                }
                sTriHit_C0F8* h = &out[n];
                h->f54 = 0;
                h->type = 2;
                h->tri = -1;
                h->f50 = 0;
                h->f58 = 0;
                h->f5C = -1;
                h->f64 = -1;
                h->f4C = -1;
                h->v20 = t4vect_Zero;
                h->v30 = t4vect_Zero;
                h->f68 = 0;
                h->f6C = 0;
                h->f70 = 0;
                h->pos = pos;
                h->nrm = hn;
                h->t = t;
                h->tri = i;
                h->f54 = 0;
                n++;
            }
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_SphereTree);
#ifdef SKIP_ASM
// Eight-entry member array with an empty constructor (only the dead loop setup remains).
struct sTreeSlot_32C508 {
    int v;
    sTreeSlot_32C508() {}
};
extern "C" void SphereTree_init(void* self);

extern "C" void* SphereTree_SphereTree(void* self)
{
    new (self) sTreeSlot_32C508[8];
    *(int*)((char*)self + 0x98) = 0;
    SphereTree_init(self);
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_init);
#ifdef SKIP_ASM
extern sSphereVec4 D_004FF640[8];
extern sSphereVec4 t4vect_Origin;

extern "C" void SphereTree_init(void* self)
{
    sSphereVec4* dst = (sSphereVec4*)self;
    for (int i = 0; i < 8; i++) {
        dst[i] = D_004FF640[i];
    }
    *(sSphereVec4*)((char*)self + 0x80) = t4vect_Origin;
    *(float*)((char*)self + 0x90) = 1.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C590);
#ifdef SKIP_ASM
extern "C" float func_0032C590(void* self)
{
    void* tree = *(void**)((char*)self + 0x98);
    void* levels = *(void**)((char*)tree + 0x20);
    return *(float*)levels * *(float*)((char*)self + 0x90);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_GetResolution);
#ifdef SKIP_ASM
struct sSphereRadius12 {
    float value;
    int pad[2];
};

extern "C" float SphereTree_GetResolution(void* self)
{
    void* p = *(void**)((char*)self + 0x98);
    int idx = *(int*)((char*)p + 0xc);
    sSphereRadius12* tab = *(sSphereRadius12**)((char*)p + 0x20);
    return tab[idx].value * *(float*)((char*)self + 0x90) * 0.5f;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C5E0);
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

extern "C" void func_0032C5E0(void* self, sSphereVec4_C5E0* offset)
{
    float* n = *(float**)((char*)self + 0x98);
    sSphereVec4_C5E0 newPos = Add_C5E0(sSphereVec4_C5E0(n), *offset);
    *(sSphereVec4_C5E0*)((char*)self + 0x80) = newPos;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_MoveOffset);
#ifdef SKIP_ASM
// self->(0x80) += *v  (4-wide VU0 add, macro mode)
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
extern "C" void SphereTree_MoveOffset(void* self, void* v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf3, 0x80(%0)\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, 0x80(%0)\n"
        ".set reorder\n"
        :
        : "r"(self), "r"(v)
        : "memory");
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_TransformAbs);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
static inline sSphereVec4 mtxMulVec_C648(sSphereVec4* m, const sSphereVec4& v)
{
    sSphereVec4 r;
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
static inline sSphereVec4 scale_C648(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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

struct sVec3_C648 {
    float x, y, z;
    sVec3_C648(float a, float b, float c) { x = a; y = b; z = c; }
};

struct sVec4_C648 : sSphereVec4 {
    sVec4_C648(const sVec3_C648& v, const float& ww) { x = v.x; y = v.y; z = v.z; w = ww; }
};

extern sSphereVec4 D_004FF640[8];

extern "C" void SphereTree_TransformAbs(void* self, sSphereVec4* m, float scale)
{
    sSphereVec4* pos = (sSphereVec4*)self;
    sSphereVec4* neg = (sSphereVec4*)((char*)self + 0x70);
    for (int i = 0; i < 4; i++) {
        pos[i] = mtxMulVec_C648(m, scale_C648(D_004FF640[i], scale));
        neg[-i] = scale_C648(pos[i], -1.0f);
    }
    float* n = *(float**)((char*)self + 0x98);
    *(sSphereVec4*)((char*)self + 0x80) = mtxMulVec_C648(m, sVec4_C648(sVec3_C648(n[5] * scale, n[6] * scale, n[7] * scale), 1.0f));
    *(float*)((char*)self + 0x90) = scale;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_TransformRel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sOct_C770 {
    sSphereVec4 dirs[8];    // 0x00
    sSphereVec4 center;     // 0x80
    float radius;           // 0x90
};

struct sSc_C770 { float x, y, z, w; };

extern "C" void SphereTree_TransformRel_i(sOct_C770* self, sSphereVec4* m, float scale) __asm__("SphereTree_TransformRel");

/**
 * Transforms a sphere tree's four axis directions (their opposites follow) and its centre, then scales them
 * and its radius.
 *
 * @param mat  the transform (On Tour const t4Matrix& transMat)
 * @param scale  uniform scale (On Tour s)
 */
extern "C" void SphereTree_TransformRel_i(sOct_C770* self, sSphereVec4* mat, float scale)
{
    for (int i = 0; i < 4; i++) {
        self->dirs[i] = Scale_B2B8(mtxMulVec_9B90(mat, &self->dirs[i]), scale);
        self->dirs[7 - i] = Scale_B2B8(self->dirs[i], -1.0f);
    }
    sSphereVec4 scaledCenter;
    sSphereVec4 transformedCenter = mtxMulVec_9B90(mat, &self->center);
    sSc_C770 sc;
    sc.w = 1.0f;
    sc.x = scale;
    sc.y = scale;
    sc.z = scale;
    scaledCenter.x = transformedCenter.x * sc.x;
    scaledCenter.y = transformedCenter.y * sc.y;
    scaledCenter.z = transformedCenter.z * sc.z;
    scaledCenter.w = transformedCenter.w * sc.w;
    self->center = scaledCenter;
    self->radius = self->radius * scale;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C898);
#ifdef SKIP_ASM
extern "C" int SphereTree_AllocBuf(void* self);
extern "C" int func_0032CDB0(void* self, void* a1, float f, void* pos, int a4, int a5, void* a6, void* a7);

extern "C" int func_0032C898(void* self, void* spherePos, float sphereRadius, void* outPush, void* outContact)
{
    void* tree = *(void**)((char*)self + 0x98);
    if (*(int*)((char*)tree + 0x8) != 0) {
        *(int*)((char*)*(void**)((char*)self + 0x98) + 0x28) = SphereTree_AllocBuf(tree);
    }
    return func_0032CDB0(self, spherePos, sphereRadius, (char*)self + 0x80, 0, 0, outPush, outContact);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C928);
#ifdef SKIP_ASM
extern "C" int SphereTree_AllocBuf(void* self);
extern "C" int func_0032D028(void* self, void* a1, void* a2, void* pos, int a4, int a5, void* a6, void* a7);

extern "C" int func_0032C928(void* self, void* boxMin, void* boxMax, void* outPos, void* outNormal)
{
    void* tree = *(void**)((char*)self + 0x98);
    if (*(int*)((char*)tree + 0x8) != 0) {
        *(int*)((char*)*(void**)((char*)self + 0x98) + 0x28) = SphereTree_AllocBuf(tree);
    }
    return func_0032D028(self, boxMin, boxMax, (char*)self + 0x80, 0, 0, outPos, outNormal);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C9B8);
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
extern "C" void func_0032D208(void* self, void* a1, void* pos, int a3, int a4, void* a5, void* a6);

extern "C" int func_0032C9B8(void* self, sVecArg_C9B8* plane, void* outPush, void* outContact)
{
    void* tree = *(void**)((char*)self + 0x98);
    if (*(int*)((char*)tree + 0x8) != 0) {
        *(int*)((char*)*(void**)((char*)self + 0x98) + 0x28) = SphereTree_AllocBuf(tree);
    }
    SphereTree_populateSortAxis_v(self, zeroW_C9B8(*plane));
    *(float*)((char*)self + 0x94) = 0.0f;
    func_0032D208(self, plane, (char*)self + 0x80, 0, 0, outPush, outContact);
    if (*(float*)((char*)self + 0x94) < 0.0f) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_IsTriCollision);
#ifdef SKIP_ASM
extern "C" int SphereTree_AllocBuf(void* self);
extern "C" void SphereTree_populateSortAxis(void* self, void* v);
extern "C" int SphereTree_checkTriCollision(void* self, void* a1, void* a2, void* a3, void* v, void* pos, int a6, int a7, void* a8, void* a9);

extern "C" int SphereTree_IsTriCollision(void* self, void* a1, void* a2, void* a3, void* v, void* a5, void* a6)
{
    void* t = *(void**)((char*)self + 0x98);
    if (*(int*)((char*)t + 0x8) != 0) {
        *(int*)((char*)*(void**)((char*)self + 0x98) + 0x28) = SphereTree_AllocBuf(t);
    }
    SphereTree_populateSortAxis(self, v);
    *(float*)((char*)self + 0x94) = 0.0f;
    SphereTree_checkTriCollision(self, a1, a2, a3, v, (char*)self + 0x80, 0, 0, a5, a6);
    if (*(float*)((char*)self + 0x94) < 0.0f) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032CB58);
#ifdef SKIP_ASM
extern "C" int SphereTree_AllocBuf(void* self);
extern "C" int func_0032CBF8(void* self, void* other, void* pos, int a3, int a4, void* a5, void* a6);

extern "C" int func_0032CB58(void* self, void* other, void* outPush, void* outContact)
{
    void* tree = *(void**)((char*)self + 0x98);
    if (*(int*)((char*)tree + 0x8) != 0) {
        *(int*)((char*)*(void**)((char*)self + 0x98) + 0x28) = SphereTree_AllocBuf(tree);
    }
    tree = *(void**)((char*)other + 0x98);
    if (*(int*)((char*)tree + 0x8) != 0) {
        *(int*)((char*)*(void**)((char*)other + 0x98) + 0x28) = SphereTree_AllocBuf(tree);
    }
    return func_0032CBF8(self, other, (char*)self + 0x80, 0, 0, outPush, outContact);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032CBF8);
#ifdef SKIP_ASM
extern "C" int func_0032CDB0(void* self, void* a1, float f, void* pos, int a4, int a5, void* a6, void* a7);
extern "C" int func_0032CBF8(void* self, void* other, void* pos, int a3, int a4, void* a5, void* a6);

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

struct sSphereOct_CBF8 {
    sSphereVec4 dirs[8];            // 0x00
    char pad_0x80[0x10];
    float scale;                    // 0x90
    char pad_0x94[4];
    sSphereTreeInfo_CBF8* tree;     // 0x98
};

// PORT: PS2-only VU0 inline asm (v * s).
static inline sSphereVec4 Scale_CBF8(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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
static inline sSphereVec4 Add_CBF8(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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

extern "C" int func_0032CBF8(void* selfp, void* other, void* posp, int depth, int base, void* outPush, void* outContact)
{
    sSphereOct_CBF8* self = (sSphereOct_CBF8*)selfp;
    sSphereVec4 childPos;
    unsigned char mask = self->tree->masks[base];
    if (depth < self->tree->depth) {
        if (depth >= 4) {
            return 1;
        }
        if (mask != 0) {
            float off = self->tree->levels[depth + 1].offset;
            for (int i = 0; i < 8; i++) {
                if ((mask >> i) & 1) {
                    childPos = Add_CBF8(*(sSphereVec4*)posp, Scale_CBF8(self->dirs[i], off));
                    if (func_0032CDB0(other, &childPos, self->tree->levels[depth].radius * self->scale, (char*)other + 0x80, 0, 0, outPush, outContact) &&
                        func_0032CBF8(self, other, &childPos, depth + 1, (i + 1) * self->tree->levels[depth].stride + base, outPush, outContact)) {
                        return 1;
                    }
                }
            }
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032CDB0);
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

struct sSphereOct_CDB0 {
    sSphereVec4 dirs[8];            // 0x00
    char pad_0x80[0x10];
    float scale;                    // 0x90
    char pad_0x94[4];
    sSphereTreeInfo_CDB0* tree;     // 0x98
};

// PORT: PS2-only VU0 inline asm (v * s).
static inline sSphereVec4 Scale_CDB0(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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
static inline sSphereVec4 Add_CDB0(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 Sub_CDB0(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline float Dot_CDB0(const sSphereVec4& a, const sSphereVec4& b)
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
static inline sSphereVec4 Normalize_CDB0(const sSphereVec4& v)
{
    sSphereVec4 r;
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

extern "C" int func_0032CDB0(void* selfp, void* spherePos, float sphereRadius, void* pos, int depth, int base, void* outA, void* outB)
{
    sSphereOct_CDB0* self = (sSphereOct_CDB0*)selfp;
    float sum = sphereRadius + self->tree->levels[depth].radius * self->scale;
    sSphereVec4 delta = Sub_CDB0(*(sSphereVec4*)spherePos, *(sSphereVec4*)pos);
    float dd = Dot_CDB0(delta, delta);
    if (!(dd < sum * sum))
        return 0;
    float f;
    unsigned char mask = self->tree->masks[base];
    if (depth < self->tree->depth && mask != 0) {
        f = self->tree->levels[depth + 1].offset;
        for (int i = 0; i < 8; i++) {
            if ((mask >> i) & 1) {
                sSphereVec4 childPos = Add_CDB0(*(sSphereVec4*)pos, Scale_CDB0(self->dirs[i], f));
                if (func_0032CDB0(self, spherePos, sphereRadius, &childPos, depth + 1, (i + 1) * self->tree->levels[depth].stride + base, outA, outB))
                    return 1;
            }
        }
        return 0;
    }
    sSphereVec4 n = Normalize_CDB0(delta);
    f = sum - Sqrt_CDB0(dd);
    *(sSphereVec4*)outA = Scale_CDB0(n, f);
    *(sSphereVec4*)outB = Sub_CDB0(*(sSphereVec4*)spherePos, Scale_CDB0(n, sphereRadius - f));
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032D028);
#ifdef SKIP_ASM
extern "C" int func_00329590(void* a, void* b, void* center, void* box, void* boxMin, void* boxMax, float radius);
extern "C" int func_0032D028(void* self, void* a1, void* a2, void* pos, int a4, int a5, void* a6, void* a7);

struct sSphereLevel_D028 {
    float radius;   // 0x0
    float offset;   // 0x4
    int stride;     // 0x8
};

struct sSphereTreeInfo_D028 {
    char pad_0x00[0xC];
    int depth;                      // 0x0C
    char pad_0x10[0x10];
    sSphereLevel_D028* levels;      // 0x20
    char pad_0x24[4];
    unsigned char* masks;           // 0x28
};

struct sSphereOct_D028 {
    sSphereVec4 dirs[8];            // 0x00
    char pad_0x80[0x10];
    float scale;                    // 0x90
    char pad_0x94[4];
    sSphereTreeInfo_D028* tree;     // 0x98
};

// PORT: PS2-only VU0 inline asm (v * s).
static inline sSphereVec4 Scale_D028(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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
static inline sSphereVec4 Add_D028(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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

extern "C" int func_0032D028(void* selfp, void* boxMin, void* boxMax, void* pos, int depth, int base, void* outPos, void* outNormal)
{
    sSphereOct_D028* self = (sSphereOct_D028*)selfp;
    sSphereVec4 normal;
    sSphereVec4 dir;
    float dist;
    if (func_00329590(boxMin, boxMax, pos, &normal, &dir, &dist, self->tree->levels[depth].radius * self->scale) == 0) {
        return 0;
    }
    unsigned char mask = self->tree->masks[base];
    if (depth < self->tree->depth && mask != 0) {
        float off = self->tree->levels[depth + 1].offset;
        for (int i = 0; i < 8; i++) {
            if ((mask >> i) & 1) {
                sSphereVec4 childPos = Add_D028(*(sSphereVec4*)pos, Scale_D028(self->dirs[i], off));
                if (func_0032D028(self, boxMin, boxMax, &childPos, depth + 1, (i + 1) * self->tree->levels[depth].stride + base, outPos, outNormal)) {
                    return 1;
                }
            }
        }
        return 0;
    }
    *(sSphereVec4*)outNormal = normal;
    *(sSphereVec4*)outPos = Scale_D028(dir, dist);
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032D208);
#ifdef SKIP_ASM
struct sSphereLevel_D208 {
    float radius;   // 0x0
    float offset;   // 0x4
    int stride;     // 0x8
};

struct sSphereTreeInfo_D208 {
    char pad_0x00[0xC];
    int depth;                      // 0x0C
    char pad_0x10[0x10];
    sSphereLevel_D208* levels;      // 0x20
    char pad_0x24[4];
    unsigned char* masks;           // 0x28
};

struct sSphereOct_D208 {
    sSphereVec4 dirs[8];            // 0x00
    char pad_0x80[0x10];
    float scale;                    // 0x90
    float best;                     // 0x94: deepest penetration found so far
    sSphereTreeInfo_D208* tree;     // 0x98
    int order[8];                   // 0x9C: child visiting order
};

// PORT: PS2-only VU0 inline asm (v * s).
static inline sSphereVec4 Scale_D208(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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
static inline sSphereVec4 Add_D208(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 Sub_D208(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline float Dot_D208(const sSphereVec4& a, const sSphereVec4& b)
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

static inline sSphereVec4 ZeroW_D208(const sSphereVec4& in)
{
    sSphereVec4 r = in;
    r.w = 0;
    return r;
}

// Sphere tree against the plane through the origin with normal *a1: records the deepest
// penetration below self->best (*outA = push-out vector, *outB = contact point).
extern "C" void func_0032D208(void* selfp, void* plane, void* pos, int depth, int base, void* outA, void* outB)
{
    sSphereOct_D208* self = (sSphereOct_D208*)selfp;
    float planeDist = Dot_D208(*(sSphereVec4*)plane, *(sSphereVec4*)pos);
    float nodeRadius = self->tree->levels[depth].radius * self->scale;
    if (!(planeDist < nodeRadius + self->best))
        return;
    float f;
    unsigned char mask = self->tree->masks[base];
    if (depth < self->tree->depth && mask != 0) {
        f = self->tree->levels[depth + 1].offset;
        for (int i = 0; i < 8; i++) {
            int k = self->order[i];
            if ((mask >> k) & 1) {
                sSphereVec4 childPos = Add_D208(*(sSphereVec4*)pos, Scale_D208(self->dirs[k], f));
                func_0032D208(self, plane, &childPos, depth + 1, (k + 1) * self->tree->levels[depth].stride + base, outA, outB);
            }
        }
        return;
    }
    f = planeDist - nodeRadius;
    if (f < self->best) {
        sSphereVec4 n = ZeroW_D208(*(sSphereVec4*)plane);
        *(sSphereVec4*)outA = Scale_D208(n, f);
        *(sSphereVec4*)outB = Sub_D208(*(sSphereVec4*)pos, Scale_D208(n, planeDist));
        self->best = f;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032D440);
#ifdef SKIP_ASM
// PORT: this caller passes a tolerance in $f12 that the unit's 4-arg func_0032B6A8 never reads;
// bind the 5-arg form to the same symbol.
extern "C" int func_0032B6A8_tol(sSphereVec4* a, sSphereVec4* b, sSphereVec4* c, sSphereVec4* d, float tol) __asm__("func_0032B6A8");

extern "C" int func_0032D440(void* self, sSphereVec4* a, sSphereVec4* b, sSphereVec4* c, sSphereVec4* d)
{
    return func_0032B6A8_tol(a, b, c, d, 9.999999747378752e-05f);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_checkTriCollision);
#ifdef SKIP_ASM
struct sSphereLevel_D470 {
    float radius;   // 0x0
    float offset;   // 0x4
    int stride;     // 0x8
};

struct sSphereTreeInfo_D470 {
    char pad_0x00[0xC];
    int depth;                      // 0x0C
    char pad_0x10[0x10];
    sSphereLevel_D470* levels;      // 0x20
    char pad_0x24[4];
    unsigned char* masks;           // 0x28
};

struct sSphereOct_D470 {
    sSphereVec4 dirs[8];            // 0x00
    char pad_0x80[0x10];
    float scale;                    // 0x90
    float best;                     // 0x94: deepest penetration found so far
    sSphereTreeInfo_D470* tree;     // 0x98
    int order[8];                   // 0x9C: child visiting order
};

// PORT: PS2-only VU0 inline asm (v * s).
static inline sSphereVec4 Scale_D470(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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
static inline sSphereVec4 Add_D470(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 Sub_D470(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline float Dot_D470(const sSphereVec4& a, const sSphereVec4& b)
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
static inline float Length_D470(const sSphereVec4& v)
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
static inline bool SegHit_D470(const sSphereVec4& e, const sSphereVec4& g, float ee, float rr)
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

// PORT: the unit's 5-arg func_0032D440 also receives the radius in $f12 here.
extern "C" int func_0032D440_r(void* self, sSphereVec4* a, sSphereVec4* b, sSphereVec4* c, sSphereVec4* d, float r) __asm__("func_0032D440");
// PORT: the unit declares SphereTree_checkTriCollision as returning int; it returns nothing.
extern "C" void SphereTree_checkTriCollision_v(void* selfp, sSphereVec4* a1, sSphereVec4* a2, sSphereVec4* a3, sSphereVec4* v, sSphereVec4* pos, int depth, int base, sSphereVec4* outA, sSphereVec4* outB) __asm__("SphereTree_checkTriCollision");

// Sphere tree against the triangle (a1, a2, a3) moving along v: records the deepest
// penetration below self->best (*outA = push-out vector, *outB = contact point).
extern "C" void SphereTree_checkTriCollision_v(void* selfp, sSphereVec4* a1, sSphereVec4* a2, sSphereVec4* a3, sSphereVec4* v, sSphereVec4* pos, int depth, int base, sSphereVec4* outA, sSphereVec4* outB)
{
    sSphereOct_D470* self = (sSphereOct_D470*)selfp;
    float d = Dot_D470(*v, Sub_D470(*pos, *a1));
    float r = self->tree->levels[depth].radius * self->scale;
    if (depth == 0 && d < 0.0f) {
        return;
    }
    if (!(d < r + self->best)) {
        return;
    }
    int leaf = 0;
    unsigned char mask = self->tree->masks[base];
    if (!(depth < self->tree->depth && depth < 3 && mask != 0)) {
        leaf = 1;
    }
    sSphereVec4 p = Sub_D470(*pos, Scale_D470(*v, d));
    if (func_0032D440_r(self, a1, a2, a3, &p, r) == 0) {
        if (leaf) {
            return;
        }
        if (r < 20.0f) {
            return;
        }
        sSphereVec4 e = Sub_D470(*pos, *a1);
        float rr = r * r;
        float ee = Dot_D470(e, e);
        if (rr < ee) {
            sSphereVec4 g = Sub_D470(*a2, *a1);
            bool miss = !SegHit_D470(e, g, ee, rr);
            if (miss) {
                sSphereVec4 g2 = Sub_D470(*a3, *a1);
                miss = !SegHit_D470(e, g2, ee, rr);
                if (miss) {
                    sSphereVec4 e3 = Sub_D470(*pos, *a2);
                    float ee3 = Dot_D470(e3, e3);
                    sSphereVec4 g3 = Sub_D470(*a3, *a2);
                    miss = !SegHit_D470(e3, g3, ee3, rr);
                    if (miss) {
                        return;
                    }
                }
            }
        }
    }
    if (!leaf) {
        float f = self->tree->levels[depth + 1].offset;
        for (int i = 0; i < 8; i++) {
            int k = self->order[i];
            if ((mask >> k) & 1) {
                sSphereVec4 q = Add_D470(*pos, Scale_D470(self->dirs[k], f));
                SphereTree_checkTriCollision_v(self, a1, a2, a3, v, &q, depth + 1, (k + 1) * self->tree->levels[depth].stride + base, outA, outB);
            }
        }
        return;
    }
    float f = d - r;
    if (f < self->best) {
        sSphereVec4 n = Scale_D470(*v, -f);
        *outA = n;
        *outB = p;
        self->best = f;
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", BXSORT_shell);
#ifdef SKIP_ASM
extern int D_0044AF98[];

// Shell sort of keys[] (descending) carrying vals[] along, gap table D_0044AF98.
extern "C" void BXSORT_shell(int n, int* keys, int* vals)
{
    int k = 1;
    while (D_0044AF98[k] < n) {
        k++;
    }
    int gap;
    for (;;) {
        gap = D_0044AF98[--k];
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
struct sV4_DB40 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_DB40(const sV4_DB40& a, const sV4_DB40& b)
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
static inline sV4_DB40 vu0Scale_DB40(const sV4_DB40& v, float s)
{
    sV4_DB40 r;
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

struct sBox_DB40 {
    sV4_DB40 axes[4];   // 0x00
    char pad40[0x5C];
    int order[8];       // 0x9C
};

extern "C" void SphereTree_populateSortAxis(void* vself, void* vdir)
{
    sBox_DB40* self = (sBox_DB40*)vself;
    sV4_DB40* dir = (sV4_DB40*)vdir;
    int keys[8];
    sV4_DB40 v = vu0Scale_DB40(*dir, -1000.0f);
    int i;
    for (i = 0; i < 4; i++) {
        int n = (int)vu0Dot_DB40(self->axes[i], v);
        keys[i] = n;
        keys[7 - i] = -n;
        self->order[i] = i;
        self->order[7 - i] = 7 - i;
    }
    BXSORT_shell(8, keys, self->order);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", SphereTree_initCache);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label as a placement operator new[].
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_SphereTreeCache[];
extern void** SphereTree_buffers;
extern void* SphereTree_bufID;
extern int SphereTree_bufNum;
extern int SphereTree_bufPtr;
extern void** SphereTree_bigBuffers;
extern void* SphereTree_bigBufID;
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
        ((int*)SphereTree_bufID)[i] = -1;
    }
    for (i = 0; i < nBig; i++) {
        SphereTree_bigBuffers[i] = new (str_SphereTreeCache, 0, 0) char[bigSize];
        ((int*)SphereTree_bigBufID)[i] = -1;
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
extern void* SphereTree_bufID;
extern int SphereTree_bufNum;
extern void** SphereTree_bigBuffers;
extern void* SphereTree_bigBufID;
extern int SphereTree_bigBufNum;

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
extern void* SphereTree_bufID;
extern int SphereTree_bufNum;
extern int SphereTree_bufPtr;
extern void** SphereTree_bigBuffers;
extern void* SphereTree_bigBufID;
extern int SphereTree_bigBufNum;
extern int SphereTree_bigBufPtr;
extern int SphereTree_bigBufDepth;
extern "C" void* func_0032B620(void* dst, void* src);

struct sSphereData_DF28 {
    int key;        // 0x00
    int slot;       // 0x04
    int pad_0x08;
    int size;       // 0x0C
    char pad_0x10[0x14];
    void* packed;   // 0x24
};

// PORT: returns the cached buffer pointer as int (the unit declares it int-returning).
extern "C" int SphereTree_AllocBuf(void* selfp)
{
    sSphereData_DF28* self = (sSphereData_DF28*)selfp;
    int key = self->key;
    int* pslot = &self->slot;
    if (self->size < SphereTree_bigBufDepth) {
        if (((int*)SphereTree_bufID)[*pslot] == key) {
            return (int)SphereTree_buffers[*pslot];
        }
        for (int i = 0; i < SphereTree_bufNum; i++) {
            if (((int*)SphereTree_bufID)[i] == key) {
                *pslot = i;
                return (int)SphereTree_buffers[i];
            }
        }
        func_0032B620(SphereTree_buffers[SphereTree_bufPtr], self->packed);
        ((int*)SphereTree_bufID)[SphereTree_bufPtr] = key;
        *pslot = SphereTree_bufPtr;
        SphereTree_bufPtr++;
        SphereTree_bufPtr %= SphereTree_bufNum;
        return (int)SphereTree_buffers[*pslot];
    } else {
        if (((int*)SphereTree_bigBufID)[*pslot] == key) {
            return (int)SphereTree_bigBuffers[*pslot];
        }
        for (int i = 0; i < SphereTree_bigBufNum; i++) {
            if (((int*)SphereTree_bigBufID)[i] == key) {
                *pslot = i;
                return (int)SphereTree_bigBuffers[i];
            }
        }
        func_0032B620(SphereTree_bigBuffers[SphereTree_bigBufPtr], self->packed);
        ((int*)SphereTree_bigBufID)[SphereTree_bigBufPtr] = key;
        *pslot = SphereTree_bigBufPtr;
        SphereTree_bigBufPtr++;
        SphereTree_bigBufPtr %= SphereTree_bigBufNum;
        return (int)SphereTree_bigBuffers[*pslot];
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032E100);
#ifdef SKIP_ASM
extern char cWorldLine_vtbl[];

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float subs).
static inline sSphereVec4 sphereCapsuleSub(sSphereVec4* a, sSphereVec4* b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 sphereCapsuleAdd(sSphereVec4* a, sSphereVec4* b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 sphereCapsuleDiv(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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

struct sSphereV4_E100 : sSphereVec4 {
    sSphereV4_E100(float ax, float ay, float az, float aw)
    {
        x = ax;
        y = ay;
        z = az;
        w = aw;
    }
};

struct sRiderCapsule_E100 {
    int state;          // 0x00
    int f4;             // 0x04
    int f8;             // 0x08
    int pad_0xC[5];
    sSphereVec4 max;    // 0x20
    sSphereVec4 min;    // 0x30
    sSphereVec4 center; // 0x40
    void* vtbl;         // 0x50
    int pad_0x54[3];
    sSphereVec4 p0;     // 0x60
    sSphereVec4 dir;    // 0x70
    sSphereVec4 p0b;    // 0x80
    sSphereVec4 dirb;   // 0x90
    float radius;       // 0xA0
};

extern "C" sRiderCapsule_E100* func_0032E100(sRiderCapsule_E100* self, sSphereVec4* p0, sSphereVec4* p1,
                                             int state, float r)
{
    self->state = state;
    self->vtbl = cWorldLine_vtbl;
    self->f8 = 0;
    self->f4 = 0;
    self->p0b = *p0;
    self->p0 = self->p0b;
    self->dirb = sphereCapsuleSub(p1, p0);
    self->dir = self->dirb;
    self->radius = r;
    self->max = sSphereV4_E100(p0->x > p1->x ? p0->x : p1->x,
                               p0->y > p1->y ? p0->y : p1->y,
                               p0->z > p1->z ? p0->z : p1->z, 1.0f);
    self->min = sSphereV4_E100(p0->x < p1->x ? p0->x : p1->x,
                               p0->y < p1->y ? p0->y : p1->y,
                               p0->z < p1->z ? p0->z : p1->z, 1.0f);
    self->center = sphereCapsuleDiv(sphereCapsuleAdd(&self->min, &self->max), 2.0f);
    return self;
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
 * @return 0 when both end points are past one face, otherwise 1
 */
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
// segment self->p0 .. p0+dir overlaps the box [mn, mx]?
extern "C" int cWorldLine_IsInBoxTransformed(sRiderCapsule_E100* self, sSphereVec4* mn, sSphereVec4* mx)
{
    sSphereVec4 endPoint = sphereCapsuleAdd(&self->p0, &self->dir);
    if ((self->p0.x < mn->x && endPoint.x < mn->x) || (mx->x < self->p0.x && mx->x < endPoint.x) ||
        (self->p0.y < mn->y && endPoint.y < mn->y) || (mx->y < self->p0.y && mx->y < endPoint.y) ||
        (self->p0.z < mn->z && endPoint.z < mn->z) || (mx->z < self->p0.z && mx->z < endPoint.z)) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_Transform);
#ifdef SKIP_ASM
// Componentwise product a * b; the base ctor takes the four products as arguments.
struct sSphereProd_E398 : sSphereV4_E100 {
    sSphereProd_E398(const sSphereVec4& a, const sSphereVec4& b)
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
extern "C" void cWorldLine_Transform(sRiderCapsule_E100* self, sSphereVec4* mat, float scale)
{
    sSphereV4_E100 sc(scale, scale, scale, 1.0f);
    self->p0 = sSphereProd_E398(mtxMulVec_9B90(mat, &self->p0b), sc);
    self->dir = sSphereProd_E398(mtxMulVec_9B90(mat, &self->dirb), sc);
    self->f4 = 1;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_UnTransform);
#ifdef SKIP_ASM
/**
 * Undoes cWorldLine_Transform: copies the base point and direction (+0x80, +0x90) back to +0x60 / +0x70 and clears
 * the transformed flag.
 */
extern "C" void cWorldLine_UnTransform(void* self)
{
    cQuad128 origP0 = *(cQuad128*)((char*)self + 0x80);
    cQuad128 origDp = *(cQuad128*)((char*)self + 0x90);
    *(cQuad128*)((char*)self + 0x60) = origP0;
    *(cQuad128*)((char*)self + 0x70) = origDp;
    *(int*)((char*)self + 0x4) = 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_IsTriInt);
#ifdef SKIP_ASM
// PORT: this caller passes a tolerance in $f12 that the unit's 4-arg func_0032B6A8 never reads;
// bind the 5-arg form to the same symbol.
extern "C" int func_0032B6A8_tol(sSphereVec4* a, sSphereVec4* b, sSphereVec4* c, sSphereVec4* d, float tol) __asm__("func_0032B6A8");

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_E4D0(const sSphereVec4& a, const sSphereVec4& b)
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
static inline sSphereVec4 vu0Sub_E4D0(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 vu0Add_E4D0(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 vu0Scale_E4D0(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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
 * @p outT, and for 0 <= t <= 1 the hit point and the normal, then runs the point-in-triangle test (func_0032B6A8,
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
extern "C" int cWorldLine_IsTriInt(void* self, sSphereVec4* vertA, sSphereVec4* vertB, sSphereVec4* vertC, sSphereVec4* n,
                             sSphereVec4* hit, sSphereVec4* outN, float* outT)
{
    sSphereVec4* origin = (sSphereVec4*)((char*)self + 0x60);
    sSphereVec4* dir = (sSphereVec4*)((char*)self + 0x70);
    float denom = vu0Dot_E4D0(*dir, *n);
    if (__builtin_fabsf(denom) < 1.000000013351432e-10f) {
        return 0;
    }
    float lineT;
    {
        sSphereVec4 toVertA = vu0Sub_E4D0(*vertA, *origin);
        lineT = vu0Dot_E4D0(toVertA, *n) / denom;
    }
    *outT = lineT;
    if (lineT < 0.0f || 1.0f < lineT) {
        return 0;
    }
    *hit = vu0Add_E4D0(*origin, vu0Scale_E4D0(*dir, lineT));
    *outN = *n;
    return func_0032B6A8_tol(vertA, vertB, vertC, hit, 9.999999747378752e-05f);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_Is2SidedTriInt);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sVE_E5E8 {
    short delta;
    short pad;
    int (*fn)(void* self, void* a, void* b, void* c, sSphereVec4* v, void* d, void* e, void* f);
};

struct sCapsuleV_E5E8 {
    char pad_0x00[0x50];
    sVE_E5E8* vt; // 0x50
    char pad_0x54[0x1C];
    sSphereVec4 dir; // 0x70
};

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
extern "C" int cWorldLine_Is2SidedTriInt(sCapsuleV_E5E8* self, void* vertA, void* vertB, void* vertC, sSphereVec4* normal, void* ipos, void* inorm, void* hitParam)
{
    float dp = vu0Dot_E4D0(self->dir, *normal);
    sVE_E5E8* vt;
    if (dp < 0.0f) {
        vt = self->vt;
        char* target = (char*)self + vt[4].delta;
        sSphereVec4 n = vu0Scale_E4D0(*normal, -1.0f);
        return vt[4].fn(target, vertA, vertB, vertC, &n, ipos, inorm, hitParam);
    }
    vt = self->vt;
    return vt[4].fn((char*)self + vt[4].delta, vertA, vertB, vertC, normal, ipos, inorm, hitParam);
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
int cWorldLine_IsSphTreeInt(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldLine_IsBoxInt);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int D_004A5978;
extern sSphereVec4 D_004FF260[];
extern sSphereVec4 D_004FF120_E690[] __asm__("t4vect_Zero");

struct sHit_E690 {
    sSphereVec4 pos;    // 0x00
    sSphereVec4 nrm;    // 0x10
    sSphereVec4 v20;    // 0x20
    sSphereVec4 v30;    // 0x30
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
    float f68;          // 0x68
    float u;            // 0x6C
    float v;            // 0x70
    int pad74[3];
};

static inline void reset_E690(sHit_E690* h)
{
    h->type = 2;
    h->obj = 0;
    h->f50 = 0;
    h->f58 = 0;
    h->f5C = -1;
    h->f60 = -1;
    h->f64 = -1;
    h->id = -1;
    h->v20 = *D_004FF120_E690;
    h->v30 = *D_004FF120_E690;
    h->f68 = 0;
    h->u = 0;
    h->v = 0;
}

struct sRay_E690 {
    char pad0[0x60];
    sSphereVec4 origin;     // 0x60
    sSphereVec4 dir;        // 0x70
};

/**
 * Intersects the line's segment with an axis-aligned box and records the entry and exit points.
 *
 * A slab test over t in [0, 1] (tmin starts at -1, tmax at 2); an axis whose direction is under 1e-10 is skipped
 * entirely, so a segment parallel to a face but outside that slab is not rejected. The face normals come from a
 * table D_004FF260 built on first use (flag D_004A5978). A hit is written for the entry (tmin >= 0) and for the
 * exit (tmax < 1): normal, point and t.
 *
 * @param bmin  the box's minimum corner (On Tour bMin)
 * @param bmax  the box's maximum corner (On Tour bMax)
 * @param hits  receives up to 2 intersections (On Tour tIntersectInfo* ti); On Tour's max count is not passed
 * @return the number of hits written (0-2)
 */
extern "C" int cWorldLine_IsBoxInt(sRay_E690* self, float* bmin, float* bmax, sHit_E690* hits)
{
    float tmin = -1.0f;
    float tmax = 2.0f;
    float* pd = &self->dir.x;
    float* po = &self->origin.x;
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
        D_004FF260[0].x = -1.0f;
        D_004FF260[0].y = 0.0f;
        D_004FF260[0].z = 0.0f;
        D_004FF260[0].w = 0.0f;
        D_004FF260[1].x = 1.0f;
        D_004FF260[1].y = 0.0f;
        D_004FF260[1].z = 0.0f;
        D_004FF260[1].w = 0.0f;
        D_004FF260[2].x = 0.0f;
        D_004FF260[2].y = -1.0f;
        D_004FF260[2].z = 0.0f;
        D_004FF260[2].w = 0.0f;
        D_004FF260[3].x = 0.0f;
        D_004FF260[3].y = 1.0f;
        D_004FF260[3].z = 0.0f;
        D_004FF260[3].w = 0.0f;
        D_004FF260[4].x = 0.0f;
        D_004FF260[4].y = 0.0f;
        D_004FF260[4].z = -1.0f;
        D_004FF260[4].w = 0.0f;
        D_004FF260[5].x = 0.0f;
        D_004FF260[5].y = 0.0f;
        D_004FF260[5].z = 1.0f;
        D_004FF260[5].w = 0.0f;
        D_004A5978 = 1;
    }
    int n = 0;
    if (0.0f <= tmin) {
        sHit_E690* entryHit = &hits[0];
        reset_E690(entryHit);
        entryHit->nrm = D_004FF260[fmin];
        entryHit->pos = vu0Add_E4D0(self->origin, vu0Scale_E4D0(self->dir, tmin));
        entryHit->dist = tmin;
        n = 1;
    }
    if (tmax < 1.0f) {
        reset_E690(&hits[n]);
        hits[n].nrm = D_004FF260[fmax];
        hits[n].pos = vu0Add_E4D0(self->origin, vu0Scale_E4D0(self->dir, tmax));
        hits[n].dist = tmax;
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
    sSphereVec4 origin; // 0x60
    sSphereVec4 dir;    // 0x70
};

// bicubic patch: one 4x4 coefficient matrix per coordinate
struct sPatch_E9A0 {
    char pad_0x00[0x40];
    sSphereVec4 m[4][4]; // 0x40
};

struct sVec4_E9A0 : sSphereVec4 {
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

static inline float Elem_E9A0(const sSphereVec4& v, int i)
{
    return (&v.x)[i];
}

// component k of the four coefficient vectors
static inline sVec4_E9A0 Column_E9A0(const sSphereVec4* c, int k)
{
    return sVec4_E9A0(Elem_E9A0(c[0], k), Elem_E9A0(c[1], k), Elem_E9A0(c[2], k), Elem_E9A0(c[3], k));
}

struct sVec3_E9A0 {
    float x, y, z;
    sVec3_E9A0() {}
    sVec3_E9A0(const sSphereVec4& v)
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
static inline sSphereVec4 vu0Sub_E9A0(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 vu0Cross_E9A0(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 vu0Normalize_E9A0(const sSphereVec4& v)
{
    sSphereVec4 r;
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
extern "C" void cWorldLine_IntersectSubPatch(sLine_E9A0* line, sPatch_E9A0* patch, float& patchU, float& patchV, sSphereVec4* outPos,
                              sSphereVec4* outNrm)
{
    sSphereVec4* mat0 = patch->m[0];
    sSphereVec4* mat1 = patch->m[1];
    sSphereVec4* mat2 = patch->m[2];
    sSphereVec4* mat3 = patch->m[3];
    sSphereVec4 surfPos;
    sSphereVec4 rowVecs[4];
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
        sVec3_E9A0 originDelta(vu0Sub_E9A0(line->origin, surfPos));
        // Jacobian [dP/du dP/dv -dir], filled row by row
        sMat3_E9A0 jacobian;
        jacobian.m[0] = dux;
        jacobian.m[3] = dvx;
        jacobian.m[6] = -line->dir.x;
        jacobian.m[1] = duy;
        jacobian.m[4] = dvy;
        jacobian.m[7] = -line->dir.y;
        jacobian.m[2] = duz;
        jacobian.m[5] = dvz;
        jacobian.m[8] = -line->dir.z;
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
    sSphereVec4 n = vu0Normalize_E9A0(vu0Cross_E9A0(dv, du));
    *outPos = surfPos;
    *outNrm = n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_cWorldRider);
#ifdef SKIP_ASM
struct sVec4_2F650 {
    float x, y, z, w;
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
static inline sVec4_2F650 vecAdd_2F650(sVec4_2F650* a, sVec4_2F650* b)
{
    sVec4_2F650 r;
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
static inline sVec4_2F650 vecScale_2F650(const sVec4_2F650& v, float s)
{
    sVec4_2F650 r;
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

struct sRiderSphere_2F650 {
    int state;          // 0x00
    int f4;             // 0x04
    int f8;             // 0x08
    int pad_0xC[5];
    sVec4_2F650 max;    // 0x20
    sVec4_2F650 min;    // 0x30
    sVec4_2F650 center; // 0x40
    void** vtbl;        // 0x50
    int pad_0x54[3];
    void* tree;         // 0x60
    void* tree2;        // 0x64
    int pad_0x68[2];
    cRiderSphereTree node; // 0x70
};

cRiderSphereTree* cRiderSphereTree_cRiderSphereTree(cRiderSphereTree* self);
extern "C" void cRiderSphereTree_GetMaskBoundBox(void* tree, sVec4_2F650* mn, sVec4_2F650* mx);
extern void* cWorldRider_vtbl[];

extern "C" sRiderSphere_2F650* cWorldRider_cWorldRider(sRiderSphere_2F650* self, void* tree, int flag)
{
    self->f8 = 0;
    self->f4 = 0;
    self->vtbl = cWorldRider_vtbl;
    cRiderSphereTree_cRiderSphereTree(&self->node);
    self->tree2 = tree;
    self->tree = tree;
    self->state = flag == 0;
    cRiderSphereTree_GetMaskBoundBox(tree, &self->min, &self->max);
    self->center = vecScale_2F650(vecAdd_2F650(&self->min, &self->max), 0.5f);
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldRider__cWorldRider);
#ifdef SKIP_ASM
extern "C" void cRiderSphereTree__cRiderSphereTree(void* p, int flags);
void operator_delete(int* ptr);
extern void* cWorldRider_vtbl[];

/**
 * Destroys a rider collision primitive: restores cWorldRider_vtbl (+0x50), destroys its sphere-tree copy (+0x70)
 * and, if asked, frees it.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cWorldRider__cWorldRider(void* self, int flags)
{
    *(void***)((char*)self + 0x50) = cWorldRider_vtbl;
    cRiderSphereTree__cRiderSphereTree((char*)self + 0x70, 2);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_IsInBoxTransformed);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float subs).
static inline sSphereVec4 sphereVecSub(sSphereVec4* a, sSphereVec4* b)
{
    sSphereVec4 r;
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
static inline sSphereVec4 sphereVecAddR(sSphereVec4* a, sSphereVec4* b)
{
    sSphereVec4 r;
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
extern "C" int cWorldRider_IsInBoxTransformed(void* self, sSphereVec4* mn, sSphereVec4* mx)
{
    sSphereVec4 rv;
    int inside = 0;
    float rad = (*(sSphereTreeNode**)((char*)self + 0x60))->radius;
    rv.x = rad;
    rv.y = rad;
    rv.z = rad;
    rv.w = 0;
    sSphereVec4 lo = sphereVecSub(&(*(sSphereTreeNode**)((char*)self + 0x60))->center, &rv);
    sSphereVec4 hi = sphereVecAddR(&(*(sSphereTreeNode**)((char*)self + 0x60))->center, &rv);
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
struct sSphereMtx;
extern "C" sSphereNode_9A28* func_00329A28(sSphereNode_9A28* self, sSphereNode_9A28* src);
extern "C" void cRiderSphereTree_ApplyTransform(sSphereTreeNode* s, sSphereMtx* m, float scale);

/**
 * Transforms the rider's sphere tree into a model's space using its own copy.
 *
 * Copies the current tree (+0x60) into the embedded copy at +0x70 (func_00329A28, On Tour
 * cRiderSphereTree::operator=), makes the copy current, applies @p mat and @p scale (cRiderSphereTree_ApplyTransform)
 * and sets the transformed flag (+0x4).
 *
 * @param mat  the transform (On Tour const t4Matrix& mat)
 * @param scale  the uniform scale
 */
extern "C" void cWorldRider_Transform(void* self, sSphereMtx* mat, float scale)
{
    sSphereTreeNode* node = (sSphereTreeNode*)((char*)self + 0x70);
    func_00329A28((sSphereNode_9A28*)node, *(sSphereNode_9A28**)((char*)self + 0x60));
    *(sSphereTreeNode**)((char*)self + 0x60) = node;
    cRiderSphereTree_ApplyTransform(node, mat, scale);
    *(int*)((char*)self + 0x4) = 1;
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
int cWorldRider_UnTransform(void* self)
{
    int origTree = *(int*)((char*)self + 0x64);
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x60) = origTree;
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
extern "C" float cWorldRider_GetIdealParam(void* self)
{
    if (*(int*)self == 0) {
        return -1.0f;
    }
    void* sphere = *(void**)((char*)self + 0x60);
    return *(float*)((char*)sphere + 0x20) * 2.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_IsTriInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sphereLength_32F8F0(const sSphereVec4& v)
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

extern "C" int cRiderSphereTree_IsTriCollision(void* tree, int a1, int a2, int a3, int a4, sSphereVec4* out, int a6);

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
extern "C" int cWorldRider_IsTriInt(void* self, int vertA, int vertB, int vertC, int n, int ipos, sSphereVec4* outDir, float* outLen)
{
    sSphereVec4 overlap;
    int result = cRiderSphereTree_IsTriCollision(*(void**)((char*)self + 0x60), vertA, vertB, vertC, n, &overlap, ipos);
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
static inline float sphereLength_32F990(const sSphereVec4& v)
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

extern "C" int cRiderSphereTree_Is2SidedTriCollision(void* tree, int a1, int a2, int a3, int a4, sSphereVec4* out, int a6);

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
extern "C" int cWorldRider_Is2SidedTriInt(void* self, int vertA, int vertB, int vertC, int n, int ipos, sSphereVec4* outDir, float* outLen)
{
    sSphereVec4 overlap;
    int result = cRiderSphereTree_Is2SidedTriCollision(*(void**)((char*)self + 0x60), vertA, vertB, vertC, n, &overlap, ipos);
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
static inline float sphereLength_32FA30(const sSphereVec4& v)
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
static inline void sphereDivEq_32FA30(sSphereVec4& v, float s)
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

struct sSphereHit_32FA30 {
    char pad_0x00[0x10];
    sSphereVec4 dir;      // 0x10
    char pad_0x20[0x20];
    float len;            // 0x40
};

extern "C" int func_00327F18(void* a, void* tree, sSphereVec4* dir, sSphereHit_32FA30* hit);

/**
 * Collides another sphere tree with the rider's current tree and normalises the hit's push-out vector.
 *
 * On a hit (func_00327F18) the hit's direction is divided by its length, which goes to the hit's length field.
 *
 * @param tree  the other sphere tree (On Tour bxSphereTree* tree)
 * @param hit  receives the intersection (On Tour tIntersectInfo* ti)
 * @return 1 on a hit, otherwise 0
 */
extern "C" int cWorldRider_IsSphTreeInt(void* self, void* tree, sSphereHit_32FA30* hit)
{
    if (func_00327F18(tree, *(void**)((char*)self + 0x60), &hit->dir, hit) != 0) {
        float len = sphereLength_32FA30(hit->dir);
        hit->len = len;
        sphereDivEq_32FA30(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldRider_IsBoxInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sphereLength_32FAC0(const sSphereVec4& v)
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
static inline void sphereDivEq_32FAC0(sSphereVec4& v, float s)
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

struct sSphereHit_32FAC0 {
    char pad_0x00[0x10];
    sSphereVec4 dir;      // 0x10
    char pad_0x20[0x20];
    float len;            // 0x40
};

extern "C" int func_0032B2B8(void* tree, void* a1, void* a2, sSphereVec4* dir, sSphereHit_32FAC0* hit);

/**
 * Collides the rider's current sphere tree with an axis-aligned box and normalises the hit's push-out vector.
 *
 * @param bMin  the box's minimum corner (On Tour bMin)
 * @param bMax  the box's maximum corner (On Tour bMax)
 * @param hit  receives the intersection (On Tour tIntersectInfo* ti); direction normalised, length stored
 * @return 1 on a hit (func_0032B2B8), otherwise 0
 */
extern "C" int cWorldRider_IsBoxInt(void* self, void* bMin, void* bMax, sSphereHit_32FAC0* hit)
{
    if (func_0032B2B8(*(void**)((char*)self + 0x60), bMin, bMax, &hit->dir, hit) != 0) {
        float len = sphereLength_32FAC0(hit->dir);
        hit->len = len;
        sphereDivEq_32FAC0(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032FB48);
#ifdef SKIP_ASM
extern char cWorldSphere_vtbl[];

struct sRiderSphere_FB48 {
    int state;              // 0x00
    int f4;                 // 0x04
    int f8;                 // 0x08
    int pad_0xC[5];
    sSphereVec4 max;        // 0x20
    sSphereVec4 min;        // 0x30
    sSphereVec4 center;     // 0x40
    void* vtbl;             // 0x50
    int pad_0x54[3];
    sSphereVec4 pos;        // 0x60
    float radius;           // 0x70
    int pad_0x74[3];
    sSphereVec4 pos0;       // 0x80
    float radius0;          // 0x90
};

extern "C" sRiderSphere_FB48* func_0032FB48(sRiderSphere_FB48* self, sSphereVec4* p, int flag, float r)
{
    sSphereVec4 t;
    self->vtbl = cWorldSphere_vtbl;
    self->f8 = 0;
    self->f4 = 0;
    if (flag != 0) {
        self->state = 0;
    } else {
        self->state = 1;
    }
    self->pos0 = *p;
    self->pos = self->pos0;
    self->radius = r;
    self->radius0 = r;
    t.x = p->x + r;
    t.y = p->y + r;
    t.z = p->z + r;
    t.w = 1.0f;
    self->max = t;
    t.x = p->x - r;
    t.y = p->y - r;
    t.z = p->z - r;
    self->min = t;
    self->center = *p;
    return self;
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
 * @return 0 when the sphere's bounding box lies wholly beyond one face, otherwise 1
 */
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float subs).
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
// sphere (pos 0x60, radius 0x70) overlaps the box [mn, mx]?
extern "C" int cWorldSphere_IsInBoxTransformed(void* self, sSphereVec4* mn, sSphereVec4* mx)
{
    sSphereVec4 rv;
    float rad = *(float*)((char*)self + 0x70);
    rv.x = rad;
    rv.y = rad;
    rv.z = rad;
    rv.w = 0;
    sSphereVec4 hi = sphereCapsuleAdd((sSphereVec4*)((char*)self + 0x60), &rv);
    sSphereVec4 lo = sphereCapsuleSub((sSphereVec4*)((char*)self + 0x60), &rv);
    if ((mn->x > hi.x && lo.x < mn->x) || (mx->x < hi.x && mx->x < lo.x) ||
        (mn->y > hi.y && lo.y < mn->y) || (mx->y < hi.y && mx->y < lo.y) ||
        (mn->z > hi.z && lo.z < mn->z) || (mx->z < hi.z && mx->z < lo.z)) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_Transform);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4x4 matrix * vector).
static inline sSphereVec4 sphereMtxMulVec(sSphereVec4* m, sSphereVec4* v)
{
    sSphereVec4 r;
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
extern "C" void cWorldSphere_Transform(void* self, sSphereVec4* mat, float scale)
{
    sSphereVec4 newCenter = sphereMtxMulVec(mat, (sSphereVec4*)((char*)self + 0x80));
    *(int*)((char*)self + 0x4) = 1;
    *(sSphereVec4*)((char*)self + 0x60) = newCenter;
    *(float*)((char*)self + 0x70) = scale * *(float*)((char*)self + 0x90);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_UnTransform);
#ifdef SKIP_ASM
/**
 * Undoes cWorldSphere_Transform: copies the base centre and radius back (+0x60, +0x70) and clears the transformed
 * flag.
 */
extern "C" void cWorldSphere_UnTransform(void* self)
{
    cQuad128 origCen = *(cQuad128*)((char*)self + 0x80);
    float origRad = *(float*)((char*)self + 0x90);
    *(cQuad128*)((char*)self + 0x60) = origCen;
    *(float*)((char*)self + 0x70) = origRad;
    *(int*)((char*)self + 0x4) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_IsSphTreeInt);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component vector length).
static inline float sphereLength_32FDB0(const sSphereVec4& v)
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
static inline void sphereDivEq_32FDB0(sSphereVec4& v, float s)
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

struct sSphereHit_32FDB0 {
    char pad_0x00[0x10];
    sSphereVec4 dir;      // 0x10
    char pad_0x20[0x20];
    float len;            // 0x40
};

extern "C" int func_0032C898(void* self, void* a1, float f, void* a2, void* a3);

/**
 * Collides the sphere with a sphere tree and normalises the hit's push-out vector.
 *
 * @param tree  the sphere tree (On Tour bxSphereTree* tree)
 * @param hit  receives the intersection (On Tour tIntersectInfo* ti); direction normalised, length stored
 * @return 1 on a hit (func_0032C898 with the centre and radius), otherwise 0
 */
extern "C" int cWorldSphere_IsSphTreeInt(void* self, void* tree, sSphereHit_32FDB0* hit)
{
    if (func_0032C898(tree, (char*)self + 0x60, *(float*)((char*)self + 0x70), &hit->dir, hit) != 0) {
        float len = sphereLength_32FDB0(hit->dir);
        hit->len = len;
        sphereDivEq_32FDB0(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_IsBoxInt);
#ifdef SKIP_ASM
extern "C" int func_00329590(void* a, void* b, void* center, void* box, void* boxMin, void* boxMax, float radius);

/**
 * Collides the sphere with an axis-aligned box (func_00329590 with the centre and radius).
 *
 * @param bMin  the box's minimum corner (On Tour bMin)
 * @param bMax  the box's maximum corner (On Tour bMax)
 * @param box  receives the intersection (On Tour tIntersectInfo* ti; its +0x10 and +0x40 parts are passed too)
 * @return 1 on a hit, otherwise 0
 */
extern "C" int cWorldSphere_IsBoxInt(void* self, void* bMin, void* bMax, void* box)
{
    return func_00329590(bMin, bMax, (char*)self + 0x60, box, (char*)box + 0x10, (char*)box + 0x40,
                         *(float*)((char*)self + 0x70)) != 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_IsTriInt);
#ifdef SKIP_ASM
extern "C" int func_00330360(void* self, sSphereVec4* plane, sSphereVec4* p, float* out);
extern "C" int func_00330250(void* self, sSphereVec4* a, sSphereVec4* b, sSphereVec4* c);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sSphereVec4 sphereScale_32FE78(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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
static inline sSphereVec4 sphereSub_32FE78(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
 * func_00330360 (On Tour isIntersectingPlane) gives the distance t to the plane; the centre projected onto the
 * plane goes to @p outPos. A hit is reported when that point lies inside the triangle (func_0032B6A8, tolerance
 * 1e-4) or the sphere touches an edge (func_00330250, On Tour edgeSphereCollision).
 *
 * @param vertA  the triangle's first corner (On Tour a)
 * @param vertB  the second corner
 * @param vertC  the third corner
 * @param plane  the triangle's normal (On Tour n)
 * @param outPos  receives the centre projected onto the plane (On Tour ipos)
 * @param outDir  receives the hit normal, @p plane (On Tour inorm)
 * @param outDist  receives the penetration depth, radius - t (On Tour param)
 * @return 1 on a hit, otherwise 0
 */
// Like cWorldSphere_Is2SidedTriInt without the plane-side flip. The success tail is written out twice (jump2 merges the
// copies); the extra self reference keeps the target's register order for self and a.
extern "C" int cWorldSphere_IsTriInt(void* self, sSphereVec4* vertA, sSphereVec4* vertB, sSphereVec4* vertC, sSphereVec4* plane,
                             sSphereVec4* outPos, sSphereVec4* outDir, float* outDist)
{
    float dist;
    if (func_00330360(self, plane, vertA, &dist) == 0) {
        return 0;
    }
    sSphereVec4 hitPos = sphereSub_32FE78(*(sSphereVec4*)((char*)self + 0x60), sphereScale_32FE78(*plane, dist));
    *outPos = hitPos;
    if (func_0032B6A8_tol(vertA, vertB, vertC, outPos, 9.999999747378752e-05f) != 0) {
        *outDir = *plane;
        *outDist = *(float*)((char*)self + 0x70) - dist;
        return 1;
    }
    if (func_00330250(self, vertA, vertB, vertC) == 0)
        return 0;
    *outDir = *plane;
    *outDist = *(float*)((char*)self + 0x70) - dist;
    return 1;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", cWorldSphere_Is2SidedTriInt);
#ifdef SKIP_ASM
extern "C" int func_00330360(void* self, sSphereVec4* plane, sSphereVec4* p, float* out);
extern "C" int func_00330250(void* self, sSphereVec4* a, sSphereVec4* b, sSphereVec4* c);

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline sSphereVec4 sphereScale_32FFA0(const sSphereVec4& v, float s)
{
    sSphereVec4 r;
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
static inline void sphereScaleEq_32FFA0(sSphereVec4& v, float s)
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
static inline sSphereVec4 sphereSub_32FFA0(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
 * @return 1 on a hit, otherwise 0
 */
extern "C" int cWorldSphere_Is2SidedTriInt(void* self, sSphereVec4* vertA, sSphereVec4* vertB, sSphereVec4* vertC, sSphereVec4* plane,
                             sSphereVec4* outPos, sSphereVec4* outDir, float* outDist)
{
    sSphereVec4 newNorm = *plane;
    float dist;
    if (func_00330360(self, plane, vertA, &dist) == 0) {
        return 0;
    }
    if (dist < 0.0f) {
        dist = -dist;
        sphereScaleEq_32FFA0(newNorm, -1.0f);
    }
    sSphereVec4 hitPos = sphereSub_32FFA0(*(sSphereVec4*)((char*)self + 0x60), sphereScale_32FFA0(newNorm, dist));
    *outPos = hitPos;
    if (func_0032B6A8_tol(vertA, vertB, vertC, outPos, 9.999999747378752e-05f) != 0 || func_00330250(self, vertA, vertB, vertC) != 0) {
        *outDir = newNorm;
        *outDist = *(float*)((char*)self + 0x70) - dist;
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_003300F8);
#ifdef SKIP_ASM
// Returns -dot(*a, *b) (4-component dot product on VU0, macro mode).
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (-(ax*bx + ay*by + az*bz + aw*bw)).
extern "C" float func_003300F8(sSphereVec4* lhs, sSphereVec4* rhs)
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
INCLUDE_ASM("intersect/riderspheretree", func_00330128);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: callers use this as a by-value struct return (hidden out pointer in $4).
sSphereVec4 func_00330128_def(sSphereVec4* a, sSphereVec4* b, sSphereVec4* p) __asm__("func_00330128");

sSphereVec4 func_00330128_def(sSphereVec4* a, sSphereVec4* b, sSphereVec4* p)
{
    sSphereVec4 d = Sub_CDB0(*p, *a);
    sSphereVec4 e = Sub_CDB0(*b, *a);
    sSphereVec4 n = Normalize_CDB0(e);
    float t = Dot_CDB0(n, d);
    float len = sphereLength_32FDB0(e);
    if (t <= 0.0f) {
        return *a;
    } else if (len <= t) {
        return *b;
    } else {
        sSphereVec4 r = Add_CDB0(*a, Scale_CDB0(n, t));
        return r;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", func_00330250);
#ifdef SKIP_ASM
// PORT: func_00330128 is defined as (out, a, b, p) returning out; this caller uses it as a by-value
// struct return (hidden out pointer in $4), so bind that view to the same symbol.
sSphereVec4 func_00330128_v(sSphereVec4* a, sSphereVec4* b, sSphereVec4* p) __asm__("func_00330128");

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline sSphereVec4 vu0Sub_30250(const sSphereVec4& a, const sSphereVec4& b)
{
    sSphereVec4 r;
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
static inline float vu0Length_30250(const sSphereVec4& v)
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

// does the sphere at self+0x60 (radius self+0x70) touch any edge of triangle a,b,c?
extern "C" int func_00330250(void* self, sSphereVec4* vert0, sSphereVec4* vert1, sSphereVec4* vert2)
{
    sSphereVec4 pt;
    sSphereVec4 tri[3];
    tri[0] = *vert0;
    tri[1] = *vert1;
    tri[2] = *vert2;
    for (int i = 0; i < 3; i++) {
        pt = func_00330128_v(&tri[i], &tri[(i + 1) % 3], (sSphereVec4*)((char*)self + 0x60));
        float dist = vu0Length_30250(vu0Sub_30250(*(sSphereVec4*)((char*)self + 0x60), pt));
        if (dist < *(float*)((char*)self + 0x70)) {
            return 1;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", func_00330360);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float sphereDot_330360(const sSphereVec4& a, const sSphereVec4& b)
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

extern "C" int func_00330360(void* self, sSphereVec4* plane, sSphereVec4* point, float* out)
{
    float planeConst = func_003300F8(plane, point);
    float signedDist = sphereDot_330360(*plane, *(sSphereVec4*)((char*)self + 0x60)) + planeConst;
    *out = signedDist;
    return sphereAbs_330360(signedDist) < *(float*)((char*)self + 0x70);
}
#endif

