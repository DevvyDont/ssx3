#include "common.h"

extern "C" void func_003E6574(void* dst, void* src, int size);

struct sInstanceStruct {
    char pad_0x00[0x8];
    int field_0x8;
    char pad_0xC[0x78 - 0xC];
    void* field_0x78;
};

//100%
INCLUDE_ASM("object/instanceman", cInstanceMan_copyInstance__FPvP15sInstanceStructT0);
#ifdef SKIP_ASM
void cInstanceMan_copyInstance(void* self, sInstanceStruct* a, void* b)
{
    void* saved = a->field_0x78;
    func_003E6574(a, b, 0xA0);
    a->field_0x78 = saved;
    a->field_0x8 |= 0x2000;
}
#endif

//100%
INCLUDE_ASM("object/instanceman", func_003512C0);
#ifdef SKIP_ASM
struct sInstHandle {
    unsigned int gen : 8;
    unsigned int index : 24;
};

struct sInstSlot {
    char pad_0x00[0x8];
    int flags;              // 0x08
    char pad_0x0C[0x6C];    // 0x0C
    sInstHandle handle;     // 0x78
    char pad_0x7C[0x34];    // 0x7C
};

struct sInstPool {
    int gen;                // 0x0
    char pad_0x4[0x8];      // 0x4
    sInstSlot* slots;       // 0xC
};

extern "C" sInstSlot* func_003512C0(sInstPool* self, unsigned int handle)
{
    if ((handle & 0xFF) == self->gen) {
        sInstSlot* slot = &self->slots[handle >> 8];
        slot->handle.gen = self->gen;
        slot->handle.index = slot - self->slots;
        slot->flags |= 0x2000;
        return slot;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("object/instanceman", func_00351398);
#ifdef SKIP_ASM
extern "C" void func_00351398(void* self, float* src)
{
    *(float**)((char*)self + 0x40) = src;
    *(float*)((char*)self + 0x44) = src[0];
    *(float*)((char*)self + 0x48) = src[1];
    *(float*)((char*)self + 0x4c) = src[2];
    *(float*)((char*)self + 0x50) = src[3];
    *(float*)((char*)self + 0x54) = src[4];
    *(float*)((char*)self + 0x58) = src[5];
}
#endif

//100%
INCLUDE_ASM("object/instanceman", func_003513D0);
#ifdef SKIP_ASM
extern "C" void* func_003E6448(void* dst, int value, int size);

extern "C" void func_003513D0(void* self)
{
    func_003E6448(self, 0, 0x40);
    func_003E6448((char*)self + 0x44, 0, 0x40);
    *(int*)((char*)self + 0x40) = 0;
}
#endif

INCLUDE_ASM("object/instanceman", func_00351508);

INCLUDE_ASM("object/instanceman", func_00351538);

INCLUDE_ASM("object/instanceman", func_00351660);

INCLUDE_ASM("object/instanceman", func_00351800);

//100%
INCLUDE_ASM("object/instanceman", func_00351948);
#ifdef SKIP_ASM
struct sImMat44 {
    float m[4][4];
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply, d = b * a).
static inline void vu0MulMatIM(sImMat44* d, const sImMat44* a, const sImMat44* b)
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
static inline void vu0CopyMatIM(sImMat44* d, const sImMat44* s)
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

extern "C" void func_00351948(void* self, sImMat44* m)
{
    sImMat44* dst = (sImMat44*)((char*)self + 0x90);
    sImMat44 t2;
    sImMat44 t;
    vu0MulMatIM(&t, m, dst);
    vu0CopyMatIM(&t2, &t);
    vu0CopyMatIM(dst, &t2);
}
#endif

extern "C" void* func_00351538(void* self);

//99.29%
INCLUDE_ASM("object/instanceman", func_00351A60__FPv);
#ifdef SKIP_ASM
void* func_00351A60(void* self)
{
    return func_00351538(self);
}
#endif

//100%
INCLUDE_ASM("object/instanceman", func_00351A80);
#ifdef SKIP_ASM
struct sImRange {
    char pad_0x00[0x10];
    float lo;   // 0x10
    float hi;   // 0x14
};

struct sImRangeTable {
    int count;          // 0x0
    sImRange* entries;  // 0x4
};

static inline bool sImRange_contains(sImRange* e, float x)
{
    return e->lo <= x && x < e->hi;
}

static inline bool sImRange_below(sImRange* e, float x)
{
    return x < e->hi;
}

extern "C" sImRange* func_00351A80(sImRangeTable* t, int* cur, float x)
{
    sImRange* e;
    int i = ++*cur;
    if (i < t->count) {
        e = &t->entries[i];
        if (sImRange_contains(e, x)) {
            return e;
        }
    }
    e = t->entries;
    for (*cur = 0; *cur < t->count - 1; ++*cur, e++) {
        if (sImRange_below(e, x)) {
            return e;
        }
    }
    return e;
}
#endif

INCLUDE_ASM("object/instanceman", func_00351B40);

INCLUDE_ASM("object/instanceman", func_00352168);

extern void* D_0048F6D8[];
extern "C" void* func_0034FBF0(void*);

//100%
INCLUDE_ASM("object/instanceman", func_00352208__FPv);
#ifdef SKIP_ASM
void* func_00352208(void* self)
{
    *(int*)((char*)self + 0xc) = (int)(void*)D_0048F6D8;
    return func_0034FBF0(self);
}
#endif

INCLUDE_ASM("object/instanceman", func_00352230);

INCLUDE_ASM("object/instanceman", func_00352500);

INCLUDE_ASM("object/instanceman", func_00352708);

INCLUDE_ASM("object/instanceman", func_00352780);

INCLUDE_ASM("object/instanceman", func_00352810);

//100%
INCLUDE_ASM("object/instanceman", func_00352A58);
#ifdef SKIP_ASM
struct sSerVEntry_00352A58 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_0034FE90(void* self, void* stream);

extern "C" void func_00352A58(void* self, void* stream)
{
    func_0034FE90(self, stream);
    sSerVEntry_00352A58* vt = *(sSerVEntry_00352A58**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x1C, 0xC);
}
#endif

