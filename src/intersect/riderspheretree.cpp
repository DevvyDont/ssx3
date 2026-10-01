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

INCLUDE_ASM("intersect/riderspheretree", func_00329970);

INCLUDE_ASM("intersect/riderspheretree", func_003299C8);

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

INCLUDE_ASM("intersect/riderspheretree", func_00329AE0);

INCLUDE_ASM("intersect/riderspheretree", func_00329B40);

INCLUDE_ASM("intersect/riderspheretree", func_00329B90);

INCLUDE_ASM("intersect/riderspheretree", func_00329DC8);

INCLUDE_ASM("intersect/riderspheretree", func_00329F98);

INCLUDE_ASM("intersect/riderspheretree", func_0032A1C0);

INCLUDE_ASM("intersect/riderspheretree", func_0032AA28);

INCLUDE_ASM("intersect/riderspheretree", func_0032B2B8);

INCLUDE_ASM("intersect/riderspheretree", func_0032B620);

INCLUDE_ASM("intersect/riderspheretree", func_0032B6A8);

INCLUDE_ASM("intersect/riderspheretree", func_0032B6E0);

INCLUDE_ASM("intersect/riderspheretree", func_0032C0F8);

INCLUDE_ASM("intersect/riderspheretree", func_0032C508);

INCLUDE_ASM("intersect/riderspheretree", func_0032C540);

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

INCLUDE_ASM("intersect/riderspheretree", func_0032C5A8);

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

INCLUDE_ASM("intersect/riderspheretree", func_0032F8C0);

INCLUDE_ASM("intersect/riderspheretree", func_0032F8F0);

INCLUDE_ASM("intersect/riderspheretree", func_0032F990);

INCLUDE_ASM("intersect/riderspheretree", func_0032FA30);

INCLUDE_ASM("intersect/riderspheretree", func_0032FAC0);

INCLUDE_ASM("intersect/riderspheretree", func_0032FB48);

INCLUDE_ASM("intersect/riderspheretree", func_0032FC08);

INCLUDE_ASM("intersect/riderspheretree", func_0032FD40);

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

INCLUDE_ASM("intersect/riderspheretree", func_003300F8);

INCLUDE_ASM("intersect/riderspheretree", func_00330128);

INCLUDE_ASM("intersect/riderspheretree", func_00330250);

INCLUDE_ASM("intersect/riderspheretree", func_00330360);

