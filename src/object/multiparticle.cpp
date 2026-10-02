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

//100%
INCLUDE_ASM("object/multiparticle", func_003584B8);
#ifdef SKIP_ASM
class func_003584B8_cObj {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

extern "C" void func_003584B8(void* self, func_003584B8_cObj* obj)
{
    obj->v01(self, 4);
}
#endif

INCLUDE_ASM("object/multiparticle", func_003584F0);

INCLUDE_ASM("object/multiparticle", func_00358588);

INCLUDE_ASM("object/multiparticle", func_00358668);

INCLUDE_ASM("object/multiparticle", func_00358700);

INCLUDE_ASM("object/multiparticle", func_00358780);

INCLUDE_ASM("object/multiparticle", func_00358998);

INCLUDE_ASM("object/multiparticle", func_00358B28);

//100%
INCLUDE_ASM("object/multiparticle", func_00358C30);
#ifdef SKIP_ASM
struct sSerVEntry_00358C30 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00354980(void* self, void* stream);

extern "C" void func_00358C30(void* self, void* stream)
{
    func_00354980(self, stream);
    sSerVEntry_00358C30* vt = *(sSerVEntry_00358C30**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x18, 0x200);
}
#endif

INCLUDE_ASM("object/multiparticle", func_00358C80);

INCLUDE_ASM("object/multiparticle", func_00358DB0);

INCLUDE_ASM("object/multiparticle", func_00358E50);

INCLUDE_ASM("object/multiparticle", func_00358EB8);

//100%
INCLUDE_ASM("object/multiparticle", func_00358F90);
#ifdef SKIP_ASM
struct sMpQuad {
    float v[4];
} __attribute__((aligned(16)));

extern sMpQuad D_004FF120;
extern "C" void func_00356AE0(void* self, void* out);

extern "C" void func_00358F90(void* self, void* out)
{
    func_00356AE0((char*)self + 0x48, out);
    *(sMpQuad*)((char*)out + 0x20) = D_004FF120;
    *(sMpQuad*)((char*)out + 0x30) = D_004FF120;
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", func_00358FD0);
#ifdef SKIP_ASM
extern "C" void func_00341FE8(void*);
extern "C" void func_00359030(void*);

extern "C" void func_00358FD0(void* self, int msg)
{
    if (msg == 0x82) {
        if (*(unsigned short*)((char*)self + 0x14) == 0) {
            *(float*)((char*)self + 0x10) = *(float*)((char*)self + 0x38);
            func_00359030(self);
            *(char*)((char*)self + 0x14) = 1;
            *(float*)((char*)self + 0x4) = *(float*)((char*)self + 0x8);
        }
    } else {
        func_00341FE8((char*)self + 0x18);
    }
}
#endif

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

//100%
INCLUDE_ASM("object/multiparticle", func_003590F0);
#ifdef SKIP_ASM
struct sVEntry003590F0 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00342150(void* self, void* stream);

extern "C" void func_003590F0(void* self, void* obj)
{
    func_00342150((char*)self + 0x18, obj);
    sVEntry003590F0* vt = *(sVEntry003590F0**)obj;
    vt[1].fn((char*)obj + vt[1].delta, self, 0x18);
}
#endif

INCLUDE_ASM("object/multiparticle", func_00359140);

//100%
INCLUDE_ASM("object/multiparticle", func_00359228);
#ifdef SKIP_ASM
extern "C" void func_00341AA0(void* self, int a, int type, int b, int c);
extern void* D_0048EA70[];

extern "C" void* func_00359228(void* self, int a, int b, int c)
{
    func_00341AA0(self, a, 2, b, c);
    *(void***)((char*)self + 0x3C) = D_0048EA70;
    *(int*)((char*)self + 0x6C) = 0;
    *(int*)((char*)self + 0x4) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", func_00359270);
#ifdef SKIP_ASM
struct sVEntry00359270 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void* func_00341C80(void* self, void* a, void* stream);
extern void* D_0048EA70[];

extern "C" void* func_00359270(void* self, void* a, void* stream)
{
    func_00341C80(self, a, stream);
    *(void***)((char*)self + 0x3C) = D_0048EA70;
    sVEntry00359270* vt = *(sVEntry00359270**)stream;
    vt[2].fn((char*)stream + vt[2].delta, (char*)self + 0x6C, 4);
    return self;
}
#endif

INCLUDE_ASM("object/multiparticle", func_003592D0);

INCLUDE_ASM("object/multiparticle", func_003593D0);

//100%
INCLUDE_ASM("object/multiparticle", func_00359410);
#ifdef SKIP_ASM
struct sSerVEntry_00359410 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00342150(void* self, void* stream);

extern "C" void func_00359410(void* self, void* stream)
{
    func_00342150(self, stream);
    sSerVEntry_00359410* vt = *(sSerVEntry_00359410**)stream;
    vt[1].fn((char*)stream + vt[1].delta, (char*)self + 0x6C, 0x4);
}
#endif

INCLUDE_ASM("object/multiparticle", func_00359460);

