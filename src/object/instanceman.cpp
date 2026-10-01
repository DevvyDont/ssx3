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

INCLUDE_ASM("object/instanceman", func_003513D0);

INCLUDE_ASM("object/instanceman", func_00351508);

INCLUDE_ASM("object/instanceman", func_00351538);

INCLUDE_ASM("object/instanceman", func_00351660);

INCLUDE_ASM("object/instanceman", func_00351800);

INCLUDE_ASM("object/instanceman", func_00351948);

extern "C" void* func_00351538(void* self);

//99.29%
INCLUDE_ASM("object/instanceman", func_00351A60__FPv);
#ifdef SKIP_ASM
void* func_00351A60(void* self)
{
    return func_00351538(self);
}
#endif

INCLUDE_ASM("object/instanceman", func_00351A80);

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

INCLUDE_ASM("object/instanceman", func_00352A58);

