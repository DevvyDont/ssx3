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

INCLUDE_ASM("intersect/riderspheretree", func_00329A28);

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

INCLUDE_ASM("intersect/riderspheretree", func_0032C898);

INCLUDE_ASM("intersect/riderspheretree", func_0032C928);

INCLUDE_ASM("intersect/riderspheretree", func_0032CA78);

INCLUDE_ASM("intersect/riderspheretree", func_0032CB58);

INCLUDE_ASM("intersect/riderspheretree", func_0032CBF8);

INCLUDE_ASM("intersect/riderspheretree", func_0032CDB0);

INCLUDE_ASM("intersect/riderspheretree", func_0032D028);

INCLUDE_ASM("intersect/riderspheretree", func_0032D440);

INCLUDE_ASM("intersect/riderspheretree", func_0032D470);

INCLUDE_ASM("intersect/riderspheretree", func_0032DA40);

INCLUDE_ASM("intersect/riderspheretree", func_0032DB40);

INCLUDE_ASM("intersect/riderspheretree", func_0032DC10);

INCLUDE_ASM("intersect/riderspheretree", func_0032DE20);

INCLUDE_ASM("intersect/riderspheretree", func_0032DF28);

INCLUDE_ASM("intersect/riderspheretree", func_0032E100);

INCLUDE_ASM("intersect/riderspheretree", func_0032E288);

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

INCLUDE_ASM("intersect/riderspheretree", func_0032F708);

INCLUDE_ASM("intersect/riderspheretree", func_0032F760);

INCLUDE_ASM("intersect/riderspheretree", func_0032F840);

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

INCLUDE_ASM("intersect/riderspheretree", func_0032F8F0);

INCLUDE_ASM("intersect/riderspheretree", func_0032F990);

INCLUDE_ASM("intersect/riderspheretree", func_0032FA30);

INCLUDE_ASM("intersect/riderspheretree", func_0032FAC0);

INCLUDE_ASM("intersect/riderspheretree", func_0032FB48);

INCLUDE_ASM("intersect/riderspheretree", func_0032FC08);

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

INCLUDE_ASM("intersect/riderspheretree", func_0032FDB0);

INCLUDE_ASM("intersect/riderspheretree", func_0032FE40);

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

INCLUDE_ASM("intersect/riderspheretree", func_00330360);

