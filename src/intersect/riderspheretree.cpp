#include "common.h"

// R5900 128-bit GPR quadword, for functions that copy a 16-byte block via a
// single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

struct cRiderSphereTree {
    char pad_0x00[0x24];
    int field_0x24;
    int field_0x28;
};

//0% - target has extra dead constant load + redundant -1 materialization not yet reproduced
INCLUDE_ASM("intersect/riderspheretree", cRiderSphereTree_cRiderSphereTree__FP16cRiderSphereTree);
#ifdef SKIP_ASM
cRiderSphereTree* cRiderSphereTree_cRiderSphereTree(cRiderSphereTree* self)
{
    self->field_0x24 = -1;
    self->field_0x28 = -1;
    return self;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_00329970);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00329970(void* self, int flags)
{
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_003299C8);
#ifdef SKIP_ASM
struct sSphereLeaf_99C8 {
    int type;                       // 0x00
    char pad_0x04[0x1C];
    float radius;                   // 0x20
    int user;                       // 0x24
    unsigned int parent;            // 0x28
    int count;                      // 0x2C
};

extern "C" void func_003299C8(sSphereLeaf_99C8* s, int type, int n, int* ids, float* radii,
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

extern "C" void func_003E6574(void* dst, void* src, int size);

extern "C" sSphereNode_9A28* func_00329A28(sSphereNode_9A28* self, sSphereNode_9A28* src)
{
    self->type = src->type;
    self->radius = src->radius;
    self->center = src->center;
    self->user = src->user;
    self->count = src->count;
    self->parent = src->parent;
    func_003E6574(self->children, src->children, self->count << 5);
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

extern "C" void func_00329A90(sSphereTreeNode* s, int n, sSphereVec4* pos, float* radii,
                              sSphereVec4* center, float radius)
{
    s->count = n;
    for (int i = 0; i < s->count; i++) {
        s->children[i].pos = pos[i];
        s->children[i].radius = radii[i];
    }
    s->center = *center;
    s->radius = radius;
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
extern "C" void func_00329B40(sSphereTreeNode* s, sSphereVec4* v)
{
    sphereVecAdd(&s->center, v);
    for (int i = 0; i < s->count; i++) {
        sphereVecAdd(&s->children[i].pos, v);
    }
}
#endif

INCLUDE_ASM("intersect/riderspheretree", func_00329B90);

INCLUDE_ASM("intersect/riderspheretree", func_00329DC8);

INCLUDE_ASM("intersect/riderspheretree", func_00329F98);

INCLUDE_ASM("intersect/riderspheretree", func_0032A1C0);

INCLUDE_ASM("intersect/riderspheretree", func_0032AA28);

INCLUDE_ASM("intersect/riderspheretree", func_0032B2B8);

INCLUDE_ASM("intersect/riderspheretree", func_0032B620);

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
    int r;
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
        : "=r"(r)
        : "r"(a), "r"(b), "r"(c), "r"(d), "0"((int)D_6E0 >> 3));
    return r != 0;
}
#endif

INCLUDE_ASM("intersect/riderspheretree", func_0032B6E0);

INCLUDE_ASM("intersect/riderspheretree", func_0032C0F8);

INCLUDE_ASM("intersect/riderspheretree", func_0032C508);

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C540);
#ifdef SKIP_ASM
extern sSphereVec4 D_004FF640[8];
extern sSphereVec4 D_004FF130;

extern "C" void func_0032C540(void* self)
{
    sSphereVec4* dst = (sSphereVec4*)self;
    for (int i = 0; i < 8; i++) {
        dst[i] = D_004FF640[i];
    }
    *(sSphereVec4*)((char*)self + 0x80) = D_004FF130;
    *(float*)((char*)self + 0x90) = 1.0f;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C590);
#ifdef SKIP_ASM
extern "C" float func_0032C590(void* self)
{
    void* p = *(void**)((char*)self + 0x98);
    void* q = *(void**)((char*)p + 0x20);
    return *(float*)q * *(float*)((char*)self + 0x90);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C5A8);
#ifdef SKIP_ASM
struct sSphereRadius12 {
    float value;
    int pad[2];
};

extern "C" float func_0032C5A8(void* self)
{
    void* p = *(void**)((char*)self + 0x98);
    int idx = *(int*)((char*)p + 0xc);
    sSphereRadius12* tab = *(sSphereRadius12**)((char*)p + 0x20);
    return tab[idx].value * *(float*)((char*)self + 0x90) * 0.5f;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C630);
#ifdef SKIP_ASM
// self->(0x80) += *v  (4-wide VU0 add, macro mode)
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
extern "C" void func_0032C630(void* self, void* v)
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

INCLUDE_ASM("intersect/riderspheretree", func_0032C648);

INCLUDE_ASM("intersect/riderspheretree", func_0032C770);

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C898);
#ifdef SKIP_ASM
extern "C" int func_0032DF28(void* self);
extern "C" int func_0032CDB0(void* self, void* a1, float f, void* pos, int a4, int a5, void* a6, void* a7);

extern "C" int func_0032C898(void* self, void* a1, float f, void* a2, void* a3)
{
    void* t = *(void**)((char*)self + 0x98);
    if (*(int*)((char*)t + 0x8) != 0) {
        *(int*)((char*)*(void**)((char*)self + 0x98) + 0x28) = func_0032DF28(t);
    }
    return func_0032CDB0(self, a1, f, (char*)self + 0x80, 0, 0, a2, a3);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032C928);
#ifdef SKIP_ASM
extern "C" int func_0032DF28(void* self);
extern "C" int func_0032D028(void* self, void* a1, void* a2, void* pos, int a4, int a5, void* a6, void* a7);

extern "C" int func_0032C928(void* self, void* a1, void* a2, void* a3, void* a4)
{
    void* t = *(void**)((char*)self + 0x98);
    if (*(int*)((char*)t + 0x8) != 0) {
        *(int*)((char*)*(void**)((char*)self + 0x98) + 0x28) = func_0032DF28(t);
    }
    return func_0032D028(self, a1, a2, (char*)self + 0x80, 0, 0, a3, a4);
}
#endif

INCLUDE_ASM("intersect/riderspheretree", func_0032CA78);

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032CB58);
#ifdef SKIP_ASM
extern "C" int func_0032DF28(void* self);
extern "C" int func_0032CBF8(void* self, void* other, void* pos, int a3, int a4, void* a5, void* a6);

extern "C" int func_0032CB58(void* self, void* other, void* a2, void* a3)
{
    void* t = *(void**)((char*)self + 0x98);
    if (*(int*)((char*)t + 0x8) != 0) {
        *(int*)((char*)*(void**)((char*)self + 0x98) + 0x28) = func_0032DF28(t);
    }
    t = *(void**)((char*)other + 0x98);
    if (*(int*)((char*)t + 0x8) != 0) {
        *(int*)((char*)*(void**)((char*)other + 0x98) + 0x28) = func_0032DF28(t);
    }
    return func_0032CBF8(self, other, (char*)self + 0x80, 0, 0, a2, a3);
}
#endif

INCLUDE_ASM("intersect/riderspheretree", func_0032CBF8);

INCLUDE_ASM("intersect/riderspheretree", func_0032CDB0);

INCLUDE_ASM("intersect/riderspheretree", func_0032D028);

INCLUDE_ASM("intersect/riderspheretree", func_0032D440);

INCLUDE_ASM("intersect/riderspheretree", func_0032D470);

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032DA40);
#ifdef SKIP_ASM
extern int D_0044AF98[];

// Shell sort of keys[] (descending) carrying vals[] along, gap table D_0044AF98.
extern "C" void func_0032DA40(int n, int* keys, int* vals)
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

INCLUDE_ASM("intersect/riderspheretree", func_0032DB40);

INCLUDE_ASM("intersect/riderspheretree", func_0032DC10);

INCLUDE_ASM("intersect/riderspheretree", func_0032DE20);

INCLUDE_ASM("intersect/riderspheretree", func_0032DF28);

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032E100);
#ifdef SKIP_ASM
extern char D_0048E6A8[];

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
    self->vtbl = D_0048E6A8;
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
INCLUDE_ASM("intersect/riderspheretree", func_0032E288);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
// segment self->p0 .. p0+dir overlaps the box [mn, mx]?
extern "C" int func_0032E288(sRiderCapsule_E100* self, sSphereVec4* mn, sSphereVec4* mx)
{
    sSphereVec4 e = sphereCapsuleAdd(&self->p0, &self->dir);
    if ((self->p0.x < mn->x && e.x < mn->x) || (mx->x < self->p0.x && mx->x < e.x) ||
        (self->p0.y < mn->y && e.y < mn->y) || (mx->y < self->p0.y && mx->y < e.y) ||
        (self->p0.z < mn->z && e.z < mn->z) || (mx->z < self->p0.z && mx->z < e.z)) {
        return 0;
    }
    return 1;
}
#endif

INCLUDE_ASM("intersect/riderspheretree", func_0032E398);

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032E4B8);
#ifdef SKIP_ASM
extern "C" void func_0032E4B8(void* self)
{
    cQuad128 a = *(cQuad128*)((char*)self + 0x80);
    cQuad128 b = *(cQuad128*)((char*)self + 0x90);
    *(cQuad128*)((char*)self + 0x60) = a;
    *(cQuad128*)((char*)self + 0x70) = b;
    *(int*)((char*)self + 0x4) = 0;
}
#endif

INCLUDE_ASM("intersect/riderspheretree", func_0032E4D0);

INCLUDE_ASM("intersect/riderspheretree", func_0032E5E8);

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032E688__FPv);
#ifdef SKIP_ASM
int func_0032E688(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("intersect/riderspheretree", func_0032E690);

INCLUDE_ASM("intersect/riderspheretree", func_0032E9A0);

INCLUDE_ASM("intersect/riderspheretree", func_0032F650);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", func_0032F708);
#ifdef SKIP_ASM
extern "C" void func_00329970(void* p, int flags);
void operator_delete(int* ptr);
extern void* D_0048E590[];

extern "C" void func_0032F708(void* self, int flags)
{
    *(void***)((char*)self + 0x50) = D_0048E590;
    func_00329970((char*)self + 0x70, 2);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032F760);
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

// sphere (self+0x60) overlaps the box [mn, mx]?
extern "C" int func_0032F760(void* self, sSphereVec4* mn, sSphereVec4* mx)
{
    sSphereVec4 rv;
    int r = 0;
    float rad = (*(sSphereTreeNode**)((char*)self + 0x60))->radius;
    rv.x = rad;
    rv.y = rad;
    rv.z = rad;
    rv.w = 0;
    sSphereVec4 lo = sphereVecSub(&(*(sSphereTreeNode**)((char*)self + 0x60))->center, &rv);
    sSphereVec4 hi = sphereVecAddR(&(*(sSphereTreeNode**)((char*)self + 0x60))->center, &rv);
    if (mx->x >= lo.x && mn->x <= hi.x && mx->y >= lo.y && mn->y <= hi.y &&
        lo.z <= mx->z && mn->z <= hi.z) {
        r = 1;
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", func_0032F840);
#ifdef SKIP_ASM
struct sSphereNode_9A28;
struct sSphereMtx;
extern "C" sSphereNode_9A28* func_00329A28(sSphereNode_9A28* self, sSphereNode_9A28* src);
extern "C" void func_00329B90(sSphereTreeNode* s, sSphereMtx* m, float scale);

extern "C" void func_0032F840(void* self, sSphereMtx* m, float scale)
{
    sSphereTreeNode* node = (sSphereTreeNode*)((char*)self + 0x70);
    func_00329A28((sSphereNode_9A28*)node, *(sSphereNode_9A28**)((char*)self + 0x60));
    *(sSphereTreeNode**)((char*)self + 0x60) = node;
    func_00329B90(node, m, scale);
    *(int*)((char*)self + 0x4) = 1;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032F8B0__FPv);
#ifdef SKIP_ASM
int func_0032F8B0(void* self)
{
    int t0 = *(int*)((char*)self + 0x64);
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x60) = t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032F8C0);
#ifdef SKIP_ASM
extern "C" float func_0032F8C0(void* self)
{
    if (*(int*)self == 0) {
        return -1.0f;
    }
    void* sphere = *(void**)((char*)self + 0x60);
    return *(float*)((char*)sphere + 0x20) * 2.0f;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", func_0032F8F0);
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

extern "C" int func_0032A1C0(void* tree, int a1, int a2, int a3, int a4, sSphereVec4* out, int a6);

extern "C" int func_0032F8F0(void* self, int a1, int a2, int a3, int a4, int a5, sSphereVec4* outDir, float* outLen)
{
    sSphereVec4 v;
    int r = func_0032A1C0(*(void**)((char*)self + 0x60), a1, a2, a3, a4, &v, a5);
    if (r != 0) {
        float len = sphereLength_32F8F0(v);
        *outLen = len;
        *outDir = sphereCapsuleDiv(v, len);
    }
    return r;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", func_0032F990);
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

extern "C" int func_0032AA28(void* tree, int a1, int a2, int a3, int a4, sSphereVec4* out, int a6);

extern "C" int func_0032F990(void* self, int a1, int a2, int a3, int a4, int a5, sSphereVec4* outDir, float* outLen)
{
    sSphereVec4 v;
    int r = func_0032AA28(*(void**)((char*)self + 0x60), a1, a2, a3, a4, &v, a5);
    if (r != 0) {
        float len = sphereLength_32F990(v);
        *outLen = len;
        *outDir = sphereCapsuleDiv(v, len);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032FA30);
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

extern "C" int func_0032FA30(void* self, void* a1, sSphereHit_32FA30* hit)
{
    if (func_00327F18(a1, *(void**)((char*)self + 0x60), &hit->dir, hit) != 0) {
        float len = sphereLength_32FA30(hit->dir);
        hit->len = len;
        sphereDivEq_32FA30(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", func_0032FAC0);
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

extern "C" int func_0032FAC0(void* self, void* a1, void* a2, sSphereHit_32FAC0* hit)
{
    if (func_0032B2B8(*(void**)((char*)self + 0x60), a1, a2, &hit->dir, hit) != 0) {
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
extern char D_0048E650[];

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
    self->vtbl = D_0048E650;
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
INCLUDE_ASM("intersect/riderspheretree", func_0032FC08);
#ifdef SKIP_ASM
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float subs).
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (4 float adds).
// sphere (pos 0x60, radius 0x70) overlaps the box [mn, mx]?
extern "C" int func_0032FC08(void* self, sSphereVec4* mn, sSphereVec4* mx)
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
INCLUDE_ASM("intersect/riderspheretree", func_0032FD40);
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

extern "C" void func_0032FD40(void* self, sSphereVec4* m, float scale)
{
    sSphereVec4 p = sphereMtxMulVec(m, (sSphereVec4*)((char*)self + 0x80));
    *(int*)((char*)self + 0x4) = 1;
    *(sSphereVec4*)((char*)self + 0x60) = p;
    *(float*)((char*)self + 0x70) = scale * *(float*)((char*)self + 0x90);
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032FD98);
#ifdef SKIP_ASM
extern "C" void func_0032FD98(void* self)
{
    cQuad128 a = *(cQuad128*)((char*)self + 0x80);
    float f = *(float*)((char*)self + 0x90);
    *(cQuad128*)((char*)self + 0x60) = a;
    *(float*)((char*)self + 0x70) = f;
    *(int*)((char*)self + 0x4) = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("intersect/riderspheretree", func_0032FDB0);
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

extern "C" int func_0032FDB0(void* self, void* a1, sSphereHit_32FDB0* hit)
{
    if (func_0032C898(a1, (char*)self + 0x60, *(float*)((char*)self + 0x70), &hit->dir, hit) != 0) {
        float len = sphereLength_32FDB0(hit->dir);
        hit->len = len;
        sphereDivEq_32FDB0(hit->dir, len);
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("intersect/riderspheretree", func_0032FE40);
#ifdef SKIP_ASM
extern "C" int func_00329590(void* a, void* b, void* center, void* box, void* boxMin, void* boxMax, float radius);

extern "C" int func_0032FE40(void* self, void* a, void* b, void* box)
{
    return func_00329590(a, b, (char*)self + 0x60, box, (char*)box + 0x10, (char*)box + 0x40,
                         *(float*)((char*)self + 0x70)) != 0;
}
#endif

INCLUDE_ASM("intersect/riderspheretree", func_0032FE78);

INCLUDE_ASM("intersect/riderspheretree", func_0032FFA0);

//100%
INCLUDE_ASM("intersect/riderspheretree", func_003300F8);
#ifdef SKIP_ASM
// Returns -dot(*a, *b) (4-component dot product on VU0, macro mode).
// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (-(ax*bx + ay*by + az*bz + aw*bw)).
extern "C" float func_003300F8(sSphereVec4* a, sSphereVec4* b)
{
    float d;
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
        : "=r"(d)
        : "r"(a), "r"(b));
    return -d;
}
#endif

INCLUDE_ASM("intersect/riderspheretree", func_00330128);

INCLUDE_ASM("intersect/riderspheretree", func_00330250);

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

extern "C" int func_00330360(void* self, sSphereVec4* plane, sSphereVec4* p, float* out)
{
    float c = func_003300F8(plane, p);
    float d = sphereDot_330360(*plane, *(sSphereVec4*)((char*)self + 0x60)) + c;
    *out = d;
    return sphereAbs_330360(d) < *(float*)((char*)self + 0x70);
}
#endif

