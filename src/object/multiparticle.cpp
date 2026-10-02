#include "common.h"

INCLUDE_ASM("object/multiparticle", cMultiParticle_setupMultiParticle);

//100%
INCLUDE_ASM("object/multiparticle", func_00358120);
#ifdef SKIP_ASM
extern "C" void func_00370788(void* self, float dt);

extern "C" void func_00358120(void* self)
{
    func_00370788(*(void**)((char*)self + 0x10), 0.01666666753590107f);
}
#endif

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

//100%
INCLUDE_ASM("object/multiparticle", func_00358380);
#ifdef SKIP_ASM
struct sSerVEntry_58380 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

struct sMultiPart_58380 {
    void* owner;            // 0x0
    int count;              // 0x4
    void* data;             // 0x8
    int* items;             // 0xC
    void* desc;             // 0x10
};

extern "C" void func_00370AA8(void* desc, void* stream);

extern "C" void func_00358380(sMultiPart_58380* self, void* stream)
{
    sSerVEntry_58380* e = &(*(sSerVEntry_58380**)stream)[1];
    e->fn((char*)stream + e->delta, self, 4);
    e = &(*(sSerVEntry_58380**)stream)[1];
    e->fn((char*)stream + e->delta, &self->count, 4);
    e = &(*(sSerVEntry_58380**)stream)[1];
    e->fn((char*)stream + e->delta, self->data, 0xD8);
    func_00370AA8(self->desc, stream);
    e = &(*(sSerVEntry_58380**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x40, 0x10);
    e = &(*(sSerVEntry_58380**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x20, 0x20);
    if (self->count > 0) {
        e = &(*(sSerVEntry_58380**)stream)[1];
        e->fn((char*)stream + e->delta, self->items, self->count << 2);
    }
}
#endif

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

//100%
INCLUDE_ASM("object/multiparticle", func_003584F0);
#ifdef SKIP_ASM
extern void* D_004A4040;
extern void* D_0048EDE0[];
extern "C" void* func_00354850(void* self, void* a1, int type);

extern "C" void* func_003584F0(void* self, void* a1)
{
    char* s = (char*)self;
    int i;
    func_00354850(self, a1, 0x12);
    *(void***)(s + 0xC) = D_0048EDE0;
    unsigned int* p = (unsigned int*)(s + 0x18);
    for (i = 0x7F; i != -1; i--, p++) {
        *p = 0xFFFFFFFF;
    }
    D_004A4040 = self;
    for (i = 0; i < 0x80; i++) {
        ((unsigned int*)(s + 0x18))[i] = 0xFFFFFFFF;
    }
    return self;
}
#endif

INCLUDE_ASM("object/multiparticle", func_00358588);

//100%
INCLUDE_ASM("object/multiparticle", func_00358668);
#ifdef SKIP_ASM
extern void* D_004A4040;
extern void* D_0048EDE0[];
extern "C" void func_00358998(void* self, int idx);
// PORT: func_00354920__FPv is the ObjNode deleting dtor; it takes (self, flags).
extern void* func_00354920_dtor(void* self, int flags) __asm__("func_00354920__FPv");

extern "C" void func_00358668(void* self, int flags)
{
    char* s = (char*)self;
    int i;
    *(void***)(s + 0xC) = D_0048EDE0;
    for (i = 0; i < 0x80; i++) {
        if (((unsigned int*)(s + 0x18))[i] != 0xFFFFFFFF) {
            func_00358998(self, i);
        }
    }
    D_004A4040 = 0;
    func_00354920_dtor(self, flags);
}
#endif

//100%
INCLUDE_ASM("object/multiparticle", func_00358700);
#ifdef SKIP_ASM
extern void* D_004A4040;
extern "C" void func_00358998(void* self, int idx);

struct sMpSlot_8700 {
    unsigned char kind;
    unsigned char pad[3];
};

struct sMpMgr_8700 {
    char pad[0x18];
    sMpSlot_8700 slots[128];
};

extern "C" void func_00358700(int kind)
{
    int i;
    if (D_004A4040 != 0) {
        for (i = 0; i < 0x80; i++) {
            sMpMgr_8700* m = (sMpMgr_8700*)D_004A4040;
            if (((unsigned int*)((char*)m + 0x18))[i] != 0xFFFFFFFF && m->slots[i].kind == kind) {
                func_00358998(m, i);
            }
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("object/multiparticle", func_00358E50);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
void cMemMan_free(void*);
extern "C" void func_00341CF0(void* p, int flags);
extern char D_0048EC28[];

extern "C" void func_00358E50(void* self, int flags)
{
    *(void**)((char*)self + 0x54) = D_0048EC28;
    void* buf = *(void**)((char*)self + 0x84);
    if (buf != 0) {
        cMemMan_free(buf);
    }
    func_00341CF0((char*)self + 0x18, 0);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

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

//100%
INCLUDE_ASM("object/multiparticle", func_00359140);
#ifdef SKIP_ASM
struct sMpItem_9140 {
    char data[0xD0];
};

struct sMpMtx_9140 {
    float m[16];
};

struct sMpPartInfo_9140 {
    char pad_0x0[0x1C];
    int active;             // 0x1C
};

struct sMpPart_9140 {
    int field_0x0;
    int field_0x4;
    sMpPartInfo_9140* info; // 0x8
    int field_0xc;
};

struct sMpModel_9140 {
    int field_0x0;
    int count;              // 0x4
    sMpPart_9140* parts;    // 0x8
};

struct sMpInst_9140 {
    char pad_0x0[0x80];
    sMpModel_9140* model;   // 0x80
};

struct sMp_9140 {
    char pad_0x0[0x14];
    signed char f14;        // 0x14
    signed char f15;        // 0x15
    signed char f16;        // 0x16
    signed char f17;        // 0x17
    char pad_0x18[0x48];
    sMpInst_9140* inst;     // 0x60
    char pad_0x64[0x18];
    sMpItem_9140* items;    // 0x7C
    int field_0x80;
    sMpMtx_9140* mtx;       // 0x84
};

extern "C" void func_0034E348(void* self);
extern "C" void func_00351948(sMpItem_9140* item, sMpMtx_9140* mtx);

extern "C" void func_00359140(sMp_9140* self)
{
    func_0034E348((char*)self + 0x34);
    if (self->items == 0) {
        return;
    }
    if (self->f14 == 0) {
        if (self->f17 == 0) {
            return;
        }
        if (self->f15 == 0) {
            return;
        }
    }
    sMpModel_9140* model = self->inst->model;
    int n = 0;
    sMpPart_9140* p = model->parts;
    for (int i = 0; i < model->count; i++, p++) {
        if (p->info->active != 0) {
            func_00351948(&self->items[n], (sMpMtx_9140*)((char*)self->mtx + (n << 6)));
            n++;
        }
    }
}
#endif

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

//100%
INCLUDE_ASM("object/multiparticle", func_003593D0);
#ifdef SKIP_ASM
// PORT: func_00341FE8 really takes (self, id, value); the unit declares it (void*).
extern "C" void func_00341FE8_set(void* self, int id, float v) __asm__("func_00341FE8");

extern "C" void func_003593D0(void* self, int id, float v)
{
    if (id == 0x8C) {
        *(float*)((char*)self + 0x6C) += v * 0.03333333507180214f;
    } else {
        func_00341FE8_set(self, id, v);
    }
}
#endif

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

