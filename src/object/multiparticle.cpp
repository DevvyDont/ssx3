#include "common.h"

INCLUDE_ASM("object/multiparticle", cMultiParticle_setupMultiParticle);

INCLUDE_ASM("object/multiparticle", func_00358120);

//100%
INCLUDE_ASM("object/multiparticle", func_00358140);
#ifdef SKIP_ASM
struct sMoveList {
    int field_0x0;          // 0x0
    int count;              // 0x4
    int field_0x8;          // 0x8
    unsigned int* entries;  // 0xC
};

extern "C" void func_00358140(sMoveList* self, unsigned int key)
{
    int i;
    for (i = 0; i < self->count; i++) {
        if ((self->entries[i] & 0xFF) == key) {
            self->count--;
            if (i < self->count) {
                self->entries[i] = self->entries[self->count];
                i--;
            } else {
                self->entries[i] = 0xFFFFFFFF;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", func_003581B8);
#ifdef SKIP_ASM
struct sParticleList {
    int capacity; // 0x0
    int count;    // 0x4
    int field_0x8;
    int* items;   // 0xc
};

extern "C" void func_003581B8(sParticleList* list, int item)
{
    int n = list->count;
    if (n < list->capacity) {
        list->items[n] = item;
        list->count = n + 1;
    }
}
#endif

INCLUDE_ASM("object/multiparticle", func_003581F0);

INCLUDE_ASM("object/multiparticle", func_00358260);

INCLUDE_ASM("object/multiparticle", func_00358380);

INCLUDE_ASM("object/multiparticle", func_003584B8);

INCLUDE_ASM("object/multiparticle", func_003584F0);

INCLUDE_ASM("object/multiparticle", func_00358588);

INCLUDE_ASM("object/multiparticle", func_00358668);

INCLUDE_ASM("object/multiparticle", func_00358700);

INCLUDE_ASM("object/multiparticle", func_00358780);

INCLUDE_ASM("object/multiparticle", func_00358998);

INCLUDE_ASM("object/multiparticle", func_00358B28);

INCLUDE_ASM("object/multiparticle", func_00358C30);

INCLUDE_ASM("object/multiparticle", func_00358C80);

INCLUDE_ASM("object/multiparticle", func_00358DB0);

INCLUDE_ASM("object/multiparticle", func_00358E50);

INCLUDE_ASM("object/multiparticle", func_00358EB8);

INCLUDE_ASM("object/multiparticle", func_00358F90);

INCLUDE_ASM("object/multiparticle", func_00358FD0);

INCLUDE_ASM("object/multiparticle", func_00359030);

//100%
INCLUDE_ASM("object/multiparticle", func_003590C8);
#ifdef SKIP_ASM
extern "C" void func_0034FD90(void*);

extern "C" void func_003590C8(void* self)
{
    if (*(signed char*)((char*)self + 0x15) == 0) {
        func_0034FD90((char*)self + 0x48);
    }
}
#endif

INCLUDE_ASM("object/multiparticle", func_003590F0);

INCLUDE_ASM("object/multiparticle", func_00359140);

INCLUDE_ASM("object/multiparticle", func_00359228);

INCLUDE_ASM("object/multiparticle", func_00359270);

INCLUDE_ASM("object/multiparticle", func_003592D0);

INCLUDE_ASM("object/multiparticle", func_003593D0);

INCLUDE_ASM("object/multiparticle", func_00359410);

INCLUDE_ASM("object/multiparticle", func_00359460);

