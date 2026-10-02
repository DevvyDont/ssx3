#include "common.h"

struct cEffectLink {
    cEffectLink* next; // 0x0
    cEffectLink* prev; // 0x4
};

//100%
INCLUDE_ASM("object/effectlink", cEffectLink_add__FP11cEffectLinkT0);
#ifdef SKIP_ASM
void cEffectLink_add(cEffectLink* link, cEffectLink* other)
{
    if (link->next != 0) {
        link->next->prev = other;
    }
    other->prev = link;
    other->next = link->next;
    link->next = other;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345720);
#ifdef SKIP_ASM
extern "C" void func_00345720(cEffectLink* link, cEffectLink* other)
{
    while (link->next != 0) {
        link = link->next;
    }
    other->next = 0;
    other->prev = link;
    link->next = other;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00345760);
#ifdef SKIP_ASM
extern "C" void func_00345760(cEffectLink* link)
{
    if (link->next != 0) {
        link->next->prev = link->prev;
    }
    if (link->prev != 0) {
        link->prev->next = link->next;
    }
    link->next = 0;
    link->prev = 0;
}
#endif

INCLUDE_ASM("object/effectlink", func_00345798);

INCLUDE_ASM("object/effectlink", func_003457C8);

INCLUDE_ASM("object/effectlink", func_00345828);

//100%
INCLUDE_ASM("object/effectlink", func_00345890);
#ifdef SKIP_ASM
struct sEffVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

extern "C" void func_00345890(void* self, void* obj)
{
    sEffVEntry* vt = *(sEffVEntry**)obj;
    vt[5].fn((char*)obj + vt[5].delta, *(void**)((char*)self + 0xC));
}
#endif

INCLUDE_ASM("object/effectlink", func_003458C0);

INCLUDE_ASM("object/effectlink", func_003459A8);

INCLUDE_ASM("object/effectlink", func_00345AD0);

INCLUDE_ASM("object/effectlink", func_00345B40);

//100%
INCLUDE_ASM("object/effectlink", func_00345BC8);
#ifdef SKIP_ASM
extern "C" void func_003708C0(void*);

extern "C" void func_00345BC8(void* self)
{
    if (*(int*)((char*)self + 0x1E4) == 0) {
        func_003708C0((char*)self + 0x50);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", func_00345BF0);
#ifdef SKIP_ASM
struct sEffectLinkVEntry5BF0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00370AA8(void* self, void* stream);

extern "C" void func_00345BF0(void* self, void* stream)
{
    func_00345890(self, stream);
    sEffectLinkVEntry5BF0* e = &(*(sEffectLinkVEntry5BF0**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x1E4, 4);
    if (*(int*)((char*)self + 0x1E4) == 0) {
        e = &(*(sEffectLinkVEntry5BF0**)stream)[1];
        e->fn((char*)stream + e->delta, *(void**)((char*)self + 0x1E0), 0xD8);
        e = &(*(sEffectLinkVEntry5BF0**)stream)[1];
        e->fn((char*)stream + e->delta, (char*)self + 0x10, 0x40);
        func_00370AA8((char*)self + 0x50, stream);
    }
}
#endif

INCLUDE_ASM("object/effectlink", func_00345C90);

INCLUDE_ASM("object/effectlink", func_00345D80);

INCLUDE_ASM("object/effectlink", func_00345E88);

//100%
INCLUDE_ASM("object/effectlink", func_00345EF8);
#ifdef SKIP_ASM
struct sEffVec4_5EF8 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sEffMtx_5EF8 {
    sEffVec4_5EF8 r[4];
};

struct sEffMtx_62A0;
extern "C" void func_0034FED8(void* a, int b, sEffMtx_62A0* out);

// PORT: PS2-only VU0 inline asm; the PC port needs a C fallback (matrix * vector).
static inline sEffVec4_5EF8 effMtxApply5EF8(sEffMtx_5EF8* m, sEffVec4_5EF8* v)
{
    sEffVec4_5EF8 out;
    __asm__(
        ".set push\n"
        ".set noreorder\n"
        "lqc2      $vf8, %1\n"
        "lqc2      $vf4, 0x0(%2)\n"
        "lqc2      $vf5, 0x10(%2)\n"
        "lqc2      $vf6, 0x20(%2)\n"
        "lqc2      $vf7, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        ".set pop\n"
        : "=m"(out)
        : "m"(*v), "r"(m)
        : "memory");
    return out;
}

struct sEffectLink5EF8 {
    char pad_0x0[0xC];
    void* model;            // 0xC
    int bone;               // 0x10
    char pad_0x14[0xC];
    sEffVec4_5EF8 localA;   // 0x20
    sEffVec4_5EF8 worldA;   // 0x30
    sEffVec4_5EF8 worldB;   // 0x40
    sEffVec4_5EF8 localB;   // 0x50
};

extern "C" void func_00345EF8(sEffectLink5EF8* self)
{
    sEffMtx_5EF8 m;
    func_0034FED8(self->model, self->bone, (sEffMtx_62A0*)&m);
    self->worldA = effMtxApply5EF8(&m, &self->localA);
    self->worldB = effMtxApply5EF8(&m, &self->localB);
}
#endif

INCLUDE_ASM("object/effectlink", func_00345F90);

//100%
INCLUDE_ASM("object/effectlink", func_00346060__FPv);
#ifdef SKIP_ASM
void func_00346060(void* self)
{
    *(int*)((char*)self + 0x14) = 1;
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_00346070);
#ifdef SKIP_ASM
extern "C" void func_00371380(void*, int);

extern "C" void func_00346070(void* self)
{
    if (*(int*)((char*)self + 0x18) == 0) {
        func_00371380((char*)self + 0x60, 7);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", func_003460A0);
#ifdef SKIP_ASM
struct sEffectLinkVEntry60A0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_003712B8(void* self, void* stream);

extern "C" void func_003460A0(void* self, void* stream)
{
    func_00345890(self, stream);
    sEffectLinkVEntry60A0* e = &(*(sEffectLinkVEntry60A0**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 0x50);
    if (*(int*)((char*)self + 0x18) == 0) {
        e = &(*(sEffectLinkVEntry60A0**)stream)[1];
        e->fn((char*)stream + e->delta, *(void**)((char*)self + 0x260), 0xD8);
        func_003712B8((char*)self + 0x60, stream);
    }
}
#endif

INCLUDE_ASM("object/effectlink", func_00346120);

INCLUDE_ASM("object/effectlink", func_003461C0);

INCLUDE_ASM("object/effectlink", func_00346228);

//100%
INCLUDE_ASM("object/effectlink", func_00346258);
#ifdef SKIP_ASM
// Advance a rotation angle (degrees) by an integer step, wrapping to 0 at +/-360.
extern "C" void func_00346258(void* self)
{
    float a = *(float*)((char*)self + 0x30) + (float)*(int*)((char*)self + 0x2c);
    *(float*)((char*)self + 0x30) = a;
    if (a >= 360.0f || a <= -360.0f) {
        *(float*)((char*)self + 0x30) = 0.0f;
    }
}
#endif

//100%
INCLUDE_ASM("object/effectlink", func_003462A0);
#ifdef SKIP_ASM
struct sEffVec4_62A0 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sEffMtx_62A0 {
    sEffVec4_62A0 r[4];
};

extern "C" void func_0034FED8(void* a, int b, sEffMtx_62A0* out);
extern "C" void func_002D1D10(sEffVec4_62A0* pos, void* p, int n, float a, float b);

extern "C" void func_003462A0(void* self)
{
    sEffMtx_62A0 m;
    void* p = (char*)self + 0x10;
    func_0034FED8(*(void**)((char*)self + 0xC), *(int*)((char*)self + 0x28), &m);
    sEffVec4_62A0 v = m.r[3];
    func_002D1D10(&v, p, *(int*)((char*)self + 0x24),
                  *(float*)((char*)self + 0x20), *(float*)((char*)self + 0x30));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("object/effectlink", func_00346300);
#ifdef SKIP_ASM
struct sEffectLinkVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00345890(void* self, void* stream);

extern "C" void func_00346300(void* self, void* stream)
{
    func_00345890(self, stream);
    sEffectLinkVEntry* vt = *(sEffectLinkVEntry**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x10, 0x30);
}
#endif

INCLUDE_ASM("object/effectlink", func_00346350);

//100%
INCLUDE_ASM("object/effectlink", func_003464E0);
#ifdef SKIP_ASM
extern "C" void* cInstanceNode_cInstanceNode(void* self, void* a1, void* stream);
extern "C" void* func_00353E80(void* self, void* stream);
extern "C" void cFlexBridgeNode_setupGrid(void* self);
extern "C" void func_003475D8(void*);
extern void* D_00490270[];

struct sEffectLinkVEntry64E0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_003464E0(void* self, void* a1, void* stream)
{
    void* p = (char*)self + 0x20;
    cInstanceNode_cInstanceNode(self, a1, stream);
    *(void***)((char*)self + 0xC) = D_00490270;
    func_00353E80((char*)self + 0x50, stream);
    cFlexBridgeNode_setupGrid(self);
    sEffectLinkVEntry64E0* e = &(*(sEffectLinkVEntry64E0**)stream)[2];
    e->fn((char*)stream + e->delta, p, 0x30);
    func_003475D8(self);
    return self;
}
#endif

INCLUDE_ASM("object/effectlink", func_00346568);

