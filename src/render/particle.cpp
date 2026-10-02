#include "common.h"

INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_Allocate);

//100%
INCLUDE_ASM("render/particle", func_00370CF8);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

extern "C" void func_00370CF8(void* self, int reset)
{
    if (*(void**)((char*)self + 0x1A0) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x1A0));
        *(void**)((char*)self + 0x1A0) = 0;
    }
    if (*(void**)((char*)self + 0x1A4) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x1A4));
        *(void**)((char*)self + 0x1A4) = 0;
    }
    if (reset) {
        *(int*)((char*)self + 0x170) = 0;
        *(int*)((char*)self + 0x178) = -1;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_reset);
#ifdef SKIP_ASM
struct sVec4A {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sVec4A D_004FF120;

extern "C" void cBaseClass_DynamicEmitter_reset(void* self)
{
    int i;
    *(int*)((char*)self + 0x17C) = 0;
    for (i = 0; i < *(int*)((char*)self + 0x178); i++) {
        (*(sVec4A**)((char*)self + 0x1A0))[i] = D_004FF120;
        (*(sVec4A**)((char*)self + 0x1A4))[i] = D_004FF120;
        *(int*)((char*)self + 0x17C) = 0;
        *(int*)((char*)self + 0x1E0) = 0;
    }
}
#endif

INCLUDE_ASM("render/particle", func_00370DC8);

INCLUDE_ASM("render/particle", func_003710D0);

//100%
INCLUDE_ASM("render/particle", func_003712B8);
#ifdef SKIP_ASM
class cStream003712B8 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
};

extern "C" void func_003712B8(void* self, cStream003712B8* stream)
{
    struct {
        float a;
        float b;
        int c;
        int d;
        int e;
    } buf;
    buf.a = *(float*)((char*)self + 0x0);
    buf.b = *(float*)((char*)self + 0x28);
    buf.d = *(int*)((char*)self + 0x170);
    buf.c = *(int*)((char*)self + 0x174);
    buf.e = *(int*)((char*)self + 0x17C);
    stream->v01(&buf, 0x14);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00371318);
#ifdef SKIP_ASM
class cStream00371318 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01(void* buf, int size);
    virtual void v02(void* buf, int size);
};

extern "C" void func_00371318(void* self, cStream00371318* stream)
{
    struct {
        float a;
        float b;
        int c;
        int d;
        int e;
    } buf;
    stream->v02(&buf, 0x14);
    *(float*)((char*)self + 0x0) = buf.a;
    *(float*)((char*)self + 0x28) = buf.b;
    *(int*)((char*)self + 0x170) = buf.d;
    *(int*)((char*)self + 0x174) = buf.c;
    *(int*)((char*)self + 0x17C) = buf.e;
}
#endif

INCLUDE_ASM("render/particle", func_00371380);

//100%
INCLUDE_ASM("render/particle", func_003714B8);
#ifdef SKIP_ASM
extern "C" void* func_00370B60(void* self);
extern void* D_004930D0[];

extern "C" void* func_003714B8(void* self)
{
    func_00370B60(self);
    *(void***)((char*)self + 0x1F8) = D_004930D0;
    *(int*)((char*)self + 0x200) = 0;
    *(int*)((char*)self + 0x204) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003714F8);
#ifdef SKIP_ASM
extern void* D_004930D0[];
extern "C" void func_003715B0(void* self, int a1);
extern "C" void func_00370C08(void* self, int flags);

extern "C" void func_003714F8(void* self, int flags)
{
    *(void***)((char*)self + 0x1F8) = D_004930D0;
    func_003715B0(self, 0);
    func_00370C08(self, flags);
}
#endif

INCLUDE_ASM("render/particle", cDynamicColourEmitter_Allocate);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", func_003715B0);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern "C" void func_00370CF8(void* self, int a1);

extern "C" void func_003715B0(void* self, int a1)
{
    void* p = *(void**)((char*)self + 0x204);
    if (p != 0) {
        cMemMan_free(p);
        *(void**)((char*)self + 0x204) = 0;
    }
    func_00370CF8(self, a1);
}
#endif

INCLUDE_ASM("render/particle", cDynamicColourEmitter_reset);

INCLUDE_ASM("render/particle", func_00371688);

INCLUDE_ASM("render/particle", func_003717C0);

//100%
INCLUDE_ASM("render/particle", func_00371940);
#ifdef SKIP_ASM
struct sGifQuad {
    unsigned int w[4];
} __attribute__((aligned(16)));

extern sGifQuad D_0044B430;

// PORT: PS2 hardware registers (D2 = GIF DMA channel, VIF1 FIFO, GIF_MODE).
extern "C" void func_00371940(unsigned int madr, int qwc, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x1000A000 & 0x100) {
        }
    }
    *(sGifQuad*)0x10005000 = D_0044B430;
    *(volatile int*)0x10003010 = 4;
    if (madr > 0x6FFFFFFF) {
        *(volatile unsigned int*)0x1000A010 = (madr & 0x3FF0) | 0x80000000;
    } else {
        *(volatile unsigned int*)0x1000A010 = madr;
    }
    *(volatile int*)0x1000A020 = qwc;
    *(volatile int*)0x1000A000 = 0x101;
    if (flags & 2) {
        while (*(volatile int*)0x1000A000 & 0x100) {
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00371D10);
#ifdef SKIP_ASM
extern "C" void func_00371D10(unsigned int madr, int sadr, int qwc, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x1000D400 & 0x100) {
        }
    }
    *(volatile int*)0x1000D410 = madr;
    *(volatile int*)0x1000D480 = sadr & 0x3FFF;
    *(volatile int*)0x1000D420 = qwc;
    *(volatile int*)0x1000D400 = 0x100;
    if (flags & 2) {
        while (*(volatile int*)0x1000D400 & 0x100) {
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00371DD8);
#ifdef SKIP_ASM
extern "C" void func_00371DD8(int sadr, unsigned int madr, int qwc, int flags)
{
    if (flags & 4) {
        while (*(volatile int*)0x1000D000 & 0x100) {
        }
    }
    *(volatile int*)0x1000D010 = madr;
    *(volatile int*)0x1000D080 = sadr & 0x3FFF;
    *(volatile int*)0x1000D020 = qwc;
    *(volatile int*)0x1000D000 = 0x100;
    if (flags & 2) {
        while (*(volatile int*)0x1000D000 & 0x100) {
        }
    }
}
#endif

extern "C" void* func_003725B0(void* self);

//100%
INCLUDE_ASM("render/particle", func_00372500__FPv);
#ifdef SKIP_ASM
void* func_00372500(void* self)
{
    return func_003725B0(self);
}
#endif

INCLUDE_ASM("render/particle", func_00372520);

INCLUDE_ASM("render/particle", func_003725B0);

//100%
INCLUDE_ASM("render/particle", func_00372660);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int* ptr);
extern void* D_00493068[];
extern void* D_00488680[];

extern "C" void func_00372660(void* self, int flags)
{
    *(void***)self = D_00493068;
    if (*(void**)((char*)self + 0x8) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x8));
    }
    *(void***)self = D_00488680;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("render/particle", func_003726C8);

INCLUDE_ASM("render/particle", func_00372AA0);

INCLUDE_ASM("render/particle", func_00372B30);

INCLUDE_ASM("render/particle", func_00372B78);

INCLUDE_ASM("render/particle", func_003739D0);

//100%
INCLUDE_ASM("render/particle", func_00374180);
#ifdef SKIP_ASM
struct sPatchVec {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct sPatchOut {
    float x, y, z;
};

struct sPatchSet {
    char pad_0x00[0x190];
    sPatchVec* basis[3];    // 0x190
    int count[3];           // 0x19C
};

// PORT: PS2-only VU0 inline asm (tensor-product patch evaluation: out[i][j] =
// sum over basis[i] (x) basis[j] of the 4x4 control block at mat+0x40).
extern "C" void func_00374180(sPatchSet* self, char* mat, sPatchOut* out, int idx)
{
    int n = self->count[idx];
    sPatchVec* basis = self->basis[idx];
    int i;
    int j;
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "lqc2      $vf5, 0x40(%0)\n"
        "lqc2      $vf6, 0x50(%0)\n"
        "lqc2      $vf7, 0x60(%0)\n"
        "lqc2      $vf8, 0x70(%0)\n"
        "lqc2      $vf9, 0x80(%0)\n"
        "lqc2      $vf10, 0x90(%0)\n"
        "lqc2      $vf11, 0xA0(%0)\n"
        "lqc2      $vf12, 0xB0(%0)\n"
        "lqc2      $vf13, 0xC0(%0)\n"
        "lqc2      $vf14, 0xD0(%0)\n"
        "lqc2      $vf15, 0xE0(%0)\n"
        "lqc2      $vf16, 0xF0(%0)\n"
        :
        : "r"(mat + 0x40));
    for (i = 0; i < n; i++) {
        __asm__ __volatile__(
            "lqc2      $vf17, 0x0(%0)\n"
            "vmulax.xyzw ACC, $vf1, $vf17x\n"
            "vmadday.xyzw ACC, $vf2, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf3, $vf17z\n"
            "vmaddw.xyzw $vf18, $vf4, $vf17w\n"
            "vmulax.xyzw ACC, $vf5, $vf17x\n"
            "vmadday.xyzw ACC, $vf6, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf7, $vf17z\n"
            "vmaddw.xyzw $vf19, $vf8, $vf17w\n"
            "vmulax.xyzw ACC, $vf9, $vf17x\n"
            "vmadday.xyzw ACC, $vf10, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf11, $vf17z\n"
            "vmaddw.xyzw $vf20, $vf12, $vf17w\n"
            "vmulax.xyzw ACC, $vf13, $vf17x\n"
            "vmadday.xyzw ACC, $vf14, $vf17y\n"
            "vmaddaz.xyzw ACC, $vf15, $vf17z\n"
            "vmaddw.xyzw $vf21, $vf16, $vf17w\n"
            :
            : "r"(&basis[i]));
        for (j = 0; j < n; j++) {
            sPatchVec t;
            __asm__(
                "lqc2      $vf22, 0x0(%1)\n"
                "vmulax.xyzw ACC, $vf18, $vf22x\n"
                "vmadday.xyzw ACC, $vf19, $vf22y\n"
                "vmaddaz.xyzw ACC, $vf20, $vf22z\n"
                "vmaddw.xyzw $vf23, $vf21, $vf22w\n"
                "sqc2      $vf23, %0\n"
                : "=m"(t)
                : "r"(&basis[j]));
            *out++ = *(sPatchOut*)&t;
        }
    }
}
#endif

INCLUDE_ASM("render/particle", func_00374298);

//100%
INCLUDE_ASM("render/particle", func_00374440);
#ifdef SKIP_ASM
struct sPtVec2 {
    float x, y;
};

struct sPtRect {
    char pad_0x0[0x10];
    sPtVec2 origin;
    sPtVec2 size;
};

extern "C" void func_00374440(char* self, sPtRect* rect, sPtVec2* out, int idx)
{
    int i, j;
    int n = *(int*)(self + (idx << 2) + 0x19C);
    sPtVec2 pos = rect->origin;
    sPtVec2 step = rect->size;
    float inv = 1.0f / (float)(n - 1);
    step.x *= inv;
    step.y *= inv;
    for (i = 0; i < n; i++) {
        sPtVec2 cur = pos;
        for (j = 0; j < n; j++) {
            *out++ = cur;
            cur.y += step.y;
        }
        pos.x += step.x;
    }
}
#endif

INCLUDE_ASM("render/particle", func_00374518);

INCLUDE_ASM("render/particle", func_003747A0);

//100%
INCLUDE_ASM("render/particle", func_00374A88);
#ifdef SKIP_ASM
struct sPartItem {
    char pad[0xC];
    short v;
    short pad2;
};

struct sPartOwner {
    char pad[0x18];
    char* lists[1];
};

extern "C" void func_00374A88(sPartOwner* self, int count, sPartItem* items, int idx, int bit)
{
    int mask = (1 << bit) | 0xC;
    for (int i = 0; i < count; i++) {
        short v = items[i].v;
        if (v >= 0) {
            char* p = self->lists[idx] + v * 0xC;
            *(int*)(p + 4) |= mask;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00374AE0);
#ifdef SKIP_ASM
struct sPartItem14 {
    char pad[0xC];
    short v;
    short pad2;
    int pad3;
};

extern "C" void func_00374AE0(sPartOwner* self, int count, sPartItem14* items, int idx, int bit)
{
    int mask = (1 << bit) | 0xC;
    for (int i = 0; i < count; i++) {
        short v = items[i].v;
        if (v >= 0) {
            char* p = self->lists[idx] + v * 0xC;
            *(int*)(p + 4) |= mask;
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00374B38);
#ifdef SKIP_ASM
struct sPartRef {
    char pad_0x00[0x1A6];
    short slot[3];          // 0x1A6
};

struct sPartSlotEntry {
    sPartRef* owner;      // 0x00
    int flags;              // 0x04
    int slot;               // 0x08
};

struct sPartLists {
    int count[3];               // 0x00
    char pad_0x0C[0xC];
    sPartSlotEntry* entries[3];     // 0x18
    char pad_0x24[0xC];
    int* freeList[3];           // 0x30
    int freeCount[3];           // 0x3C
    char pad_0x48[0x1CC - 0x48];
    int* slotList[3];           // 0x1CC
    int slotCount[3];           // 0x1D8
};

extern "C" void func_00374B38(sPartLists* self)
{
    int i;
    for (i = 0; i < 3; i++) {
        int j;
        int n = self->count[i];
        sPartSlotEntry* list = self->entries[i];
        for (j = 0; j < n; j++) {
            sPartSlotEntry* e = &list[j];
            int f = e->flags;
            if (f & 4) {
                e->flags = f & ~4;
            } else if (f & 8) {
                e->flags = f & ~8;
            } else if (f != 0) {
                e->flags = 0;
                e->owner->slot[i] = -1;
                if (e->slot >= 0) {
                    self->slotList[i][self->slotCount[i]++] = e->slot;
                    e->slot = -1;
                }
                self->freeList[i][self->freeCount[i]++] = j;
            }
        }
    }
}
#endif

INCLUDE_ASM("render/particle", func_00374C90);

INCLUDE_ASM("render/particle", func_00374CA8);

//100%
INCLUDE_ASM("render/particle", func_00374CE0);
#ifdef SKIP_ASM
extern "C" void func_00374CE0(void* self, int a1, int a2, int a3, int a4, int a5, int a6)
{
    *(int*)((char*)self + 0x17c) = a1;
    *(int*)((char*)self + 0x180) = a2;
    *(int*)((char*)self + 0x184) = a3;
    *(int*)((char*)self + 0x188) = a4;
    *(int*)((char*)self + 0x18c) = a5;
    *(int*)((char*)self + 0x190) = a6;
}
#endif

INCLUDE_ASM("render/particle", func_00374D00);

INCLUDE_ASM("render/particle", func_00375890);

//100%
INCLUDE_ASM("render/particle", func_003758F8__FPv);
#ifdef SKIP_ASM
void* func_003758F8(void* self)
{
    *(int*)((char*)self + 0x170) = -1;
    *(int*)((char*)self + 0x174) = 0x80;
    *(int*)((char*)self + 0x178) = 0x80;
    return self;
}
#endif

INCLUDE_ASM("render/particle", func_00375918);

INCLUDE_ASM("render/particle", func_00375990);

extern void* D_00493000[];

//100%
INCLUDE_ASM("render/particle", func_003759E8__FPv);
#ifdef SKIP_ASM
void* func_003759E8(void* self)
{
    *(int*)self = (int)(void*)D_00493000;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00375A00__FPv);
#ifdef SKIP_ASM
void func_00375A00(void* self)
{
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00375A08);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_00492878[];
extern "C" void* func_00395288(void* self);

extern "C" void* func_00375A08(void)
{
    return func_00395288(cMemMan_alloc(0x75E0, D_00492878, 0, 0));
}
#endif

INCLUDE_ASM("render/particle", func_00375A40);

//100%
INCLUDE_ASM("render/particle", func_00376268);
#ifdef SKIP_ASM
extern "C" void func_00424880(int);
extern "C" void func_00423AA0(int, int);
extern "C" void func_00424950(int);
extern "C" void func_00423AD0(int, int);
extern "C" void func_00423BF0(int);
extern "C" void func_00423BB0(int);
extern "C" void func_00423DB0(int);
extern "C" void func_00367360(void*);
void* func_00361F40(void* self);
void operator_delete(int* ptr);
extern char D_005059D8[];

extern "C" void func_00376268(void* self)
{
    func_00424880(2);
    func_00423AA0(2, *(int*)((char*)self + 0x5AC0));
    func_00424950(1);
    func_00423AD0(0, *(int*)((char*)self + 0x5AC4));
    func_00423BF0(*(int*)((char*)self + 0x5AD0));
    func_00423BB0(*(int*)((char*)self + 0x5AD0));
    func_00423DB0(*(int*)((char*)self + 0x5AC8));
    func_00423DB0(*(int*)((char*)self + 0x5ACC));
    operator_delete(*(int**)((char*)self + 0x18F0));
    func_00367360(*(void**)((char*)self + 0x18F4));
    operator_delete(*(int**)((char*)self + 0x18F4));
    func_00361F40(D_005059D8);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003762F8);
#ifdef SKIP_ASM
struct sPartTexEntA4 {
    char pad_0x00[0x88];
    int width;          // 0x88
    int height;         // 0x8C
    int psm;            // 0x90
    int clutPsm;        // 0x94
    int fmt;            // 0x98
    char pad_0x9C[4];
    int id;             // 0xA0
};

struct sPartTexCache {
    char pad_0x00[0x59C4];
    int count;              // 0x59C4
    char pad_0x59C8[4];
    sPartTexEntA4* textures;     // 0x59CC
};

extern "C" int func_003762F8(sPartTexCache* self, int width, int height, unsigned int bpp,
                             unsigned int clutBpp, int fmt, int id)
{
    int i;
    sPartTexEntA4* t = self->textures;
    for (i = 0; i < self->count; i++, t++) {
        if (t->width == width && t->height == height && t->id == id) {
            int ok1 = 0;
            int ok2 = 0;
            int ok3 = 0;
            switch (bpp) {
            case 16:
                if (t->psm == 2) ok1 = 1; else ok1 = 0;
            case 24:
                if (t->psm == 1) ok1 = 1;
                break;
            case 32:
                if (t->psm == 0) ok1 = 1; else ok1 = 0;
                break;
            }
            switch (clutBpp) {
            case 16:
                if (t->clutPsm == 2) ok2 = 1; else ok2 = 0;
                break;
            case 24:
                if (t->clutPsm == 1) ok2 = 1; else ok2 = 0;
                break;
            case 32:
                if (t->clutPsm == 0) ok2 = 1; else ok2 = 0;
                break;
            }
            if (fmt == 24) {
                if (t->fmt == 0x31) ok3 = 1; else ok3 = 0;
            }
            if (ok1 && ok2 && ok3) {
                return i;
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00376468);
#ifdef SKIP_ASM
struct sVEntry00376468 {
    short delta;
    short index;
    void* fn;
};

extern "C" void* func_00376468(void* self)
{
    sVEntry00376468* vt = *(sVEntry00376468**)((char*)self + 0x10D8);
    int i = ((int (*)(void*))vt[123].fn)((char*)self + vt[123].delta);
    if (i >= 0) {
        vt = *(sVEntry00376468**)((char*)self + 0x10D8);
        return ((void* (*)(void*, int))vt[4].fn)((char*)self + vt[4].delta, i);
    }
    return 0;
}
#endif

INCLUDE_ASM("render/particle", func_003764C0);

//100%
INCLUDE_ASM("render/particle", func_00376560);
#ifdef SKIP_ASM
extern "C" void* func_00376560(void* self, int a1)
{
    return *(char**)((char*)self + 0x59cc) + a1 * 0xa4;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00376578);
#ifdef SKIP_ASM
struct sPartVEntry122 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int, int, int, int);
};

extern "C" int func_00376578(void* self, int a1)
{
    sPartVEntry122* vt = *(sPartVEntry122**)((char*)self + 0x10D8);
    return vt[122].fn((char*)self + vt[122].delta, 0x200, 0x1C0, 0x18, 0x20, 0x18, a1);
}
#endif

INCLUDE_ASM("render/particle", func_003765B8);

INCLUDE_ASM("render/particle", func_00376768);

INCLUDE_ASM("render/particle", func_00376938);

INCLUDE_ASM("render/particle", func_00376A70);

//100%
INCLUDE_ASM("render/particle", func_00376B90);
#ifdef SKIP_ASM
struct sPartVec4i {
    float x, y, z, w;
};

extern "C" sPartVec4i* func_00376B90(sPartVec4i* r, void* self)
{
    int i = *(int*)((char*)self + 0x10dc);
    float* p = (float*)((char*)self + (i * 0x230 + 0x6d20));
    int x = (int)p[0];
    int y = (int)p[1];
    int z = (int)p[2];
    int w = (int)p[3];
    r->x = (float)x;
    r->y = (float)y;
    r->z = (float)z;
    r->w = (float)w;
    return r;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00376C10);
#ifdef SKIP_ASM
extern "C" float func_00376C10(void* self)
{
    int i = *(int*)((char*)self + 0x10dc);
    char* p = (char*)self + i * 0x230;
    return *(float*)(p + 0x6d38);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00376C28);
#ifdef SKIP_ASM
struct sPartEmitter {
    char pad[0x10];
    float x;
    float y;
    float z;
    char pad2[0x230 - 0x1C];
};

struct sPartSys {
    char pad[0x10dc];
    int cur;
    char pad2[0x6d20 - 0x10e0];
    sPartEmitter emitters[1];
};

extern "C" void func_00376C28(sPartSys* self, float* a, float* b, float* c)
{
    sPartEmitter* e = &self->emitters[self->cur];
    *a = e->z;
    *b = e->x;
    *c = e->y;
}
#endif

INCLUDE_ASM("render/particle", func_00376C58);

INCLUDE_ASM("render/particle", func_00377278);

INCLUDE_ASM("render/particle", func_00377458);

//100%
INCLUDE_ASM("render/particle", func_00377950);
#ifdef SKIP_ASM
extern "C" void func_00377950(void* self, int mode)
{
    *(int*)((char*)self + 0x6b94) = mode;
    switch (mode) {
    case 0:
        *(float*)((char*)self + 0x6b98) = 0.0f;
        *(float*)((char*)self + 0x6b9c) = 1.0f;
        *(float*)((char*)self + 0x6ba0) = 1.0f;
        *(float*)((char*)self + 0x6ba4) = 1.0f;
        break;
    case 1:
        *(float*)((char*)self + 0x6b98) = 0.125f;
        *(float*)((char*)self + 0x6b9c) = 0.75f;
        *(float*)((char*)self + 0x6ba0) = 0.75f;
        *(float*)((char*)self + 0x6ba4) = 0.75f;
        break;
    case 2:
        *(float*)((char*)self + 0x6ba0) = 0.75f;
        *(float*)((char*)self + 0x6b98) = 0.0f;
        *(float*)((char*)self + 0x6b9c) = 1.0f;
        *(float*)((char*)self + 0x6ba4) = 1.0f;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003779E0);
#ifdef SKIP_ASM
struct sPart20 {
    int v[5];
};

extern "C" void func_003779E0(void* self, sPart20* src)
{
    *(sPart20*)((char*)self + 0x6b94) = *src;
}
#endif

INCLUDE_ASM("render/particle", func_00377A10);

INCLUDE_ASM("render/particle", func_00377CF0);

INCLUDE_ASM("render/particle", func_003781A0);

INCLUDE_ASM("render/particle", func_00378808);

INCLUDE_ASM("render/particle", func_00379028);

INCLUDE_ASM("render/particle", func_00379860);

INCLUDE_ASM("render/particle", func_00379BD0);

INCLUDE_ASM("render/particle", func_0037A260);

INCLUDE_ASM("render/particle", func_0037A430);

INCLUDE_ASM("render/particle", func_0037A540);

INCLUDE_ASM("render/particle", func_0037A610);

INCLUDE_ASM("render/particle", func_0037B548);

INCLUDE_ASM("render/particle", func_0037BA50);

INCLUDE_ASM("render/particle", func_0037BB10);

INCLUDE_ASM("render/particle", func_0037BC40);

INCLUDE_ASM("render/particle", func_0037BD38);

INCLUDE_ASM("render/particle", func_0037BD98);

INCLUDE_ASM("render/particle", func_0037C198);

INCLUDE_ASM("render/particle", func_0037C570);

INCLUDE_ASM("render/particle", func_0037C720);

//100%
INCLUDE_ASM("render/particle", func_0037C7C0);
#ifdef SKIP_ASM
extern "C" void func_003E6448(void* dst, int c, int n);
void func_00366FE0(void* p, int a1);

extern "C" void func_0037C7C0(void* self, int a1)
{
    func_003E6448((char*)self + 0x3838, 0, 0x1F40);
    func_00366FE0(*(void**)((char*)self + 0x18F4), a1);
}
#endif

extern "C" void* func_003E6574(void*, void*, int);

//100%
INCLUDE_ASM("render/particle", func_0037C808__FPv);
#ifdef SKIP_ASM
void* func_0037C808(void* self)
{
    return func_003E6574((char*)self + 0x18f8, (char*)self + 0x3838, 0x1f40);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037C830);
#ifdef SKIP_ASM
struct sVEntry0037C830 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_0037C830(void* self)
{
    int i;
    for (i = 0; i < 2000; i++) {
        if (((int*)((char*)self + 0x18F8))[i] != 0) {
            sVEntry0037C830* vt = *(sVEntry0037C830**)((char*)self + 0x10D8);
            vt[50].fn((char*)self + vt[50].delta, i);
        }
    }
}
#endif

// declared by mangled name so we can call it with a single argument, the way
// the target does (its real signature takes a second arg the caller leaves set)
extern "C" void func_00366FE0__FPvi(void*);

//100%
INCLUDE_ASM("render/particle", func_0037C8A0);
#ifdef SKIP_ASM
extern "C" void func_0037C8A0(void* self)
{
    func_00366FE0__FPvi(*(void**)((char*)self + 0x18f4));
}
#endif

INCLUDE_ASM("render/particle", func_0037C8C0);

//100%
INCLUDE_ASM("render/particle", func_0037CAF8);
#ifdef SKIP_ASM
extern "C" int func_00367440(void* mgr, int a1, int a2, int a3, int a4, int a5, int a6, int a7,
                             int a8, int a9, int a10, int a11);

extern "C" int func_0037CAF8(void* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7,
                              int a8, int a9, int a10, int a11)
{
    int i = func_00367440(*(void**)((char*)self + 0x18F4), a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
    *(int*)((char*)self + (i << 2) + 0x3838) = 1;
    return i;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037CB50);
#ifdef SKIP_ASM
extern "C" void func_00367BC0(void* tbl, int idx);

extern "C" void func_0037CB50(void* self, int idx)
{
    func_00367BC0(*(void**)((char*)self + 0x18F4), idx);
    *(int*)((char*)self + (idx << 2) + 0x18F8) = 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037CB90);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` bitfield (GS TEX0 register layout)
struct sGsTex0 {
    ulong TBP0 : 14;
    ulong TBW : 6;
    ulong PSM : 6;
    ulong TW : 4;
    ulong TH : 4;
    ulong TCC : 1;
    ulong TFX : 2;
    ulong CBP : 14;
    ulong CPSM : 4;
    ulong CSM : 1;
    ulong CSA : 5;
    ulong CLD : 3;
};

struct sPartTex {
    char pad[0x38];
    sGsTex0 tex0;
};

struct sPartTexTable {
    int pad[2];
    sPartTex* entries[2000];
};

extern "C" int func_0037CB90(void* self, int idx)
{
    sPartTexTable* t = *(sPartTexTable**)((char*)self + 0x18f4);
    int r;
    if (idx >= 0) {
        if (idx < 2000) {
            r = t->entries[idx] != 0;
        } else {
            r = 0;
        }
    } else {
        r = 1;
    }
    return r;
}
#endif

extern "C" void* func_003691B0(int);

//100%
INCLUDE_ASM("render/particle", func_0037CBC8__FPv);
#ifdef SKIP_ASM
void* func_0037CBC8(void* self)
{
    return func_003691B0(*(int*)((char*)self + 0x18f4));
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037CBE8);
#ifdef SKIP_ASM
struct sPartVEntry52 {
    short delta;
    short index;
    int (*fn)(void*, int, int, int);
};

extern "C" void func_0037CBE8(void* self, void* out, int a2, int a3)
{
    sPartVEntry52* vt = *(sPartVEntry52**)((char*)self + 0x10D8);
    *(int*)((char*)out + 4) = vt[52].fn((char*)self + vt[52].delta, a2, a3, 0);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037CC30);
#ifdef SKIP_ASM
struct sVEntry0037CC30 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

extern "C" void func_00367B60(void* mgr, int id);

extern "C" void func_0037CC30(void* self, void* item)
{
    if (*(int*)((char*)item + 0x4) != -1) {
        func_00367B60(*(void**)((char*)self + 0x18F4), *(int*)((char*)item + 0x4));
        sVEntry0037CC30* vt = *(sVEntry0037CC30**)((char*)self + 0x10D8);
        vt[50].fn((char*)self + vt[50].delta, *(int*)((char*)item + 0x4));
        *(int*)((char*)item + 0x4) = -1;
    }
}
#endif

INCLUDE_ASM("render/particle", func_0037CC98);

extern "C" void* func_00369130(int);

//100%
INCLUDE_ASM("render/particle", func_0037D090__FPv);
#ifdef SKIP_ASM
void* func_0037D090(void* self)
{
    return func_00369130(*(int*)((char*)self + 0x18f4));
}
#endif

INCLUDE_ASM("render/particle", func_0037D0B0);

//100%
INCLUDE_ASM("render/particle", func_0037D318);
#ifdef SKIP_ASM
extern "C" void func_00367D20(void* gm, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9);

extern "C" void func_0037D318(char* self, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9)
{
    func_00367D20(*(void**)(self + 0x18F4), a1, a2, a3, a4, a5, a6, a7, a8, a9);
}
#endif

INCLUDE_ASM("render/particle", func_0037D348);

INCLUDE_ASM("render/particle", func_0037D450);

extern "C" void* func_00367CD0(int);

//100%
INCLUDE_ASM("render/particle", func_0037D738__FPv);
#ifdef SKIP_ASM
void* func_0037D738(void* self)
{
    return func_00367CD0(*(int*)((char*)self + 0x18f4));
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037D758);
#ifdef SKIP_ASM
struct sPartQ16 {
    int w[4];
} __attribute__((aligned(16)));

struct sPartMtx {
    float m[4][4];
} __attribute__((aligned(16)));

struct sPartDrawEntry {
    sPartQ16 q;          // 0x00
    sPartMtx m0;         // 0x10
    sPartMtx m1;         // 0x50
    float f;             // 0x90
    int i;               // 0x94
};

struct sPartDrawList {
    char pad_0x0[0x13EC];
    int count;                    // 0x13EC
    sPartDrawEntry entries[1];    // 0x13F0
};

// PORT: PS2-only VU0 asm (lqc2/sqc2 4x4 matrix copy).
static inline void vu0CopyMtxP(sPartMtx* dst, sPartMtx* src)
{
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "sqc2      $vf1, 0x0(%1)\n"
        "sqc2      $vf2, 0x10(%1)\n"
        "sqc2      $vf3, 0x20(%1)\n"
        "sqc2      $vf4, 0x30(%1)\n"
        :
        : "r"(src), "r"(dst)
        : "memory");
}

extern "C" void func_0037D758(sPartDrawList* self, sPartQ16* q, sPartMtx* m0, sPartMtx* m1, float f, int i)
{
    self->entries[self->count].q = *q;
    vu0CopyMtxP(&self->entries[self->count].m0, m0);
    vu0CopyMtxP(&self->entries[self->count].m1, m1);
    self->entries[self->count].f = f;
    self->entries[self->count].i = i;
    self->count++;
}
#endif

INCLUDE_ASM("render/particle", func_0037D800);

//100%
INCLUDE_ASM("render/particle", func_0037D908);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` bitfield (GS TEX0 register layout)
extern "C" int func_0037D908(void* self, int idx)
{
    sPartTexTable* t = *(sPartTexTable**)((char*)self + 0x18f4);
    return 1 << t->entries[idx]->tex0.TW;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037D938);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` bitfield (GS TEX0 register layout)
extern "C" int func_0037D938(void* self, int idx)
{
    sPartTexTable* t = *(sPartTexTable**)((char*)self + 0x18f4);
    return 1 << t->entries[idx]->tex0.TH;
}
#endif

INCLUDE_ASM("render/particle", func_0037D968);

INCLUDE_ASM("render/particle", func_0037DBE8);

INCLUDE_ASM("render/particle", func_0037DD20);

//100%
INCLUDE_ASM("render/particle", func_0037DE88);
#ifdef SKIP_ASM
// VU0 microprogram entry (micro-memory address 0x570; resolved via undefined_syms_auto.txt)
extern char D_570[];

// PORT: PS2-only VU0 microprogram call (lqc2/ctc2/vcallmsr/cfc2); the PC port needs a C
// version of the microprogram.
extern "C" int func_0037DE88(void* self, void* a, void* b, void* m)
{
    int r1;
    int r2;
    __asm__ __volatile__(
        "lqc2      $vf15, 0x0(%2)\n"
        "lqc2      $vf16, 0x0(%3)\n"
        "lqc2      $vf10, 0x0(%4)\n"
        "lqc2      $vf11, 0x10(%4)\n"
        "lqc2      $vf12, 0x20(%4)\n"
        "lqc2      $vf13, 0x30(%4)\n"
        "ctc2.ni   %5, $vi27\n"
        "vnop\n"
        "vnop\n"
        "vcallmsr  $vi27\n"
        "cfc2.i    %0, $vi1\n"
        "cfc2.ni   %1, $vi2\n"
        : "=r"(r1), "=r"(r2)
        : "r"(a), "r"(b), "r"(m), "r"((unsigned int)D_570 >> 3));
    if (r2 != 0) {
        return 1;
    }
    if (r1 != 0) {
        return 2;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037DEE0);
#ifdef SKIP_ASM
struct sPartQVec {
    float x, y, z, w;
} __attribute__((aligned(16)));

// Transforms v by the 4x4 matrix at self+0x58A0, divides by w, then scales by the
// vector at self+0x58E0 and adds the one at self+0x58F0.
// PORT: PS2-only VU0 macro-mode asm; the PC port needs a C version.
extern "C" sPartQVec func_0037DEE0(void* self, sPartQVec* v)
{
    sPartQVec r;
    __asm__ __volatile__(
        "lqc2      $vf10, 0x0(%1)\n"
        "lqc2      $vf1, 0x0(%2)\n"
        "lqc2      $vf2, 0x10(%2)\n"
        "lqc2      $vf3, 0x20(%2)\n"
        "lqc2      $vf4, 0x30(%2)\n"
        "lqc2      $vf5, %3\n"
        "lqc2      $vf6, %4\n"
        "vmulax.xyzw  ACC, $vf1, $vf10x\n"
        "vmadday.xyzw ACC, $vf2, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf3, $vf10z\n"
        "vmaddw.xyzw  $vf11, $vf4, $vf10w\n"
        "vdiv      Q, $vf0w, $vf11w\n"
        "vwaitq\n"
        "vmulq.xyzw   $vf12, $vf11, Q\n"
        "vmula.xyzw   ACC, $vf5, $vf12\n"
        "vmaddw.xyzw  $vf13, $vf6, $vf0w\n"
        "sqc2      $vf13, %0\n"
        : "=m"(r)
        : "r"(v), "r"((char*)self + 0x58A0),
          "m"(*(sPartQVec*)((char*)self + 0x58E0)),
          "m"(*(sPartQVec*)((char*)self + 0x58F0)));
    return r;
}
#endif

INCLUDE_ASM("render/particle", func_0037DF88);

//100%
INCLUDE_ASM("render/particle", func_0037E040);
#ifdef SKIP_ASM
// PORT: 128-bit GPR quadword (TImode); the PC port needs a 16-byte struct.
typedef int cPartQuad128 __attribute__((mode(TI)));

// Appends a DMA cnt tag (qwc 5) plus a VIF UNPACK V4-32 header and the 4x4 matrix m
// to the packet at *pp.
// PORT: 64-bit `ulong` packet words and PS2-only VU0 asm (lqc2/sqc2 copy).
extern "C" void func_0037E040(void* m, char** pp)
{
    char* p = *pp;
    *(cPartQuad128*)p = 0x10000005;
    *(ulong*)(p + 0x18) = (ulong)0x6C048005 << 32;
    *(ulong*)(p + 0x10) = 0;
    __asm__ __volatile__(
        "lqc2      $vf1, 0x0(%0)\n"
        "lqc2      $vf2, 0x10(%0)\n"
        "lqc2      $vf3, 0x20(%0)\n"
        "lqc2      $vf4, 0x30(%0)\n"
        "sqc2      $vf1, 0x0(%1)\n"
        "sqc2      $vf2, 0x10(%1)\n"
        "sqc2      $vf3, 0x20(%1)\n"
        "sqc2      $vf4, 0x30(%1)\n"
        :
        : "r"(m), "r"(p + 0x20)
        : "memory");
    *pp = p + 0x60;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0037E098);
#ifdef SKIP_ASM
struct sPartPktQuad {
    int w[4];
} __attribute__((aligned(16)));

// PORT: 64-bit `ulong` packet words.
extern "C" void func_0037E098(sPartPktQuad* src, void* self, sPartPktQuad* hdr, char** pp)
{
    char* p = *pp;
    int i;
    *(ulong*)(p + 0x0) = 0x1000000F;
    *(ulong*)(p + 0x8) = 0;
    *(ulong*)(p + 0x10) = 0;
    *(ulong*)(p + 0x18) = (ulong)0x6C038009 << 32;
    ((sPartPktQuad*)(p + 0x20))[0] = hdr[0];
    ((sPartPktQuad*)(p + 0x20))[1] = hdr[1];
    ((sPartPktQuad*)(p + 0x20))[2] = hdr[2];
    *(ulong*)(p + 0x50) = 0;
    *(ulong*)(p + 0x58) = (ulong)0x6C0A800F << 32;
    p += 0x60;
    for (i = 0; i < 10; i++) {
        ((sPartPktQuad*)p)[i] = src[i];
    }
    p += 0xA0;
    *pp = p;
}
#endif

INCLUDE_ASM("render/particle", func_0037E120);

INCLUDE_ASM("render/particle", func_0037E238);

INCLUDE_ASM("render/particle", func_00380380);

INCLUDE_ASM("render/particle", func_00380518);

INCLUDE_ASM("render/particle", func_003807A0);

INCLUDE_ASM("render/particle", func_00380CE0);

INCLUDE_ASM("render/particle", func_00381310);

INCLUDE_ASM("render/particle", func_003816F0);

INCLUDE_ASM("render/particle", func_00381AD0);

INCLUDE_ASM("render/particle", func_00381F10);

INCLUDE_ASM("render/particle", func_00382170);

//100%
INCLUDE_ASM("render/particle", func_003825C0);
#ifdef SKIP_ASM
extern "C" void func_003825F8(void* arg);

extern "C" int func_003825C0(int cause, void* arg)
{
    if (cause != 2) {
        // PORT: PS2-only debug trap (assert).
        __asm__ __volatile__("break 0");
    }
    func_003825F8(arg);
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003825F8);
#ifdef SKIP_ASM
extern "C" void func_00423DD0(int sema);

extern "C" void func_003825F8(void* self)
{
    int t = ++*(int*)((char*)self + 0x5ABC);
    if (*(int*)((char*)self + 0x5A8C) == 4) {
        if (t >= *(int*)((char*)self + 0x5AB8)) {
            *(int*)((char*)self + 0x5A8C) = 5;
            *(int*)((char*)self + 0x5ABC) = 0;
            func_00423DD0(*(int*)((char*)self + 0x5AC8));
        }
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00382650);
#ifdef SKIP_ASM
extern "C" void func_00382688(void* arg);

extern "C" int func_00382650(int cause, void* arg)
{
    if (cause != 1) {
        // PORT: PS2-only debug trap (assert).
        __asm__ __volatile__("break 0");
    }
    func_00382688(arg);
    // PORT: PS2-only: re-enable interrupts (EI).
    __asm__ __volatile__("sync.l\n\tei");
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00382688);
#ifdef SKIP_ASM
extern "C" void func_00423DD0(int sema);

extern "C" void func_00382688(void* self)
{
    if (*(volatile int*)((char*)self + 0x5A8C) == 1) {
        *(volatile int*)((char*)self + 0x5A8C) = 2;
        func_00423DD0(*(int*)((char*)self + 0x5AC8));
    } else if (*(volatile int*)((char*)self + 0x5A8C) == 3) {
        *(volatile int*)((char*)self + 0x5A8C) = 4;
    }
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003826E0);
#ifdef SKIP_ASM
extern "C" int func_003826E0(void* self)
{
    volatile int* arr = (volatile int*)((char*)self + 0x5a90);
    volatile int* s = &arr[*(int*)((char*)self + 0x5a10)];
    if (*s == 0 || *s == 1) {
        if (*s == 0) {
            *s = 1;
        }
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00382730);
#ifdef SKIP_ASM
extern "C" int func_00382730(void* self)
{
    return *(unsigned int*)((char*)self + 0x5a8c) == 0;
}
#endif

extern "C" void* func_00382760(void* self);

//100%
INCLUDE_ASM("render/particle", func_00382740__FPv);
#ifdef SKIP_ASM
void* func_00382740(void* self)
{
    return func_00382760(self);
}
#endif

INCLUDE_ASM("render/particle", func_00382760);

INCLUDE_ASM("render/particle", func_00382AF0);

INCLUDE_ASM("render/particle", func_00383A10);

//100%
INCLUDE_ASM("render/particle", func_00384D98);
#ifdef SKIP_ASM
extern "C" void* func_00384D98(void* self, unsigned int* src)
{
    *(unsigned int**)((char*)self + 0x0) = src;
    *(unsigned int*)((char*)self + 0x4) = *src | 0x30000000;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0xC) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00384DC0);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` packet words.
extern "C" void func_00384DC0(void* self)
{
    if (*(int*)((char*)self + 0x10) & 1) {
        *(*(ulong**)((char*)self + 0x4))++ = 0;
    }
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x0) = ((ulong)0x14000000 << 32) | 1;
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x8) = 0;
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x10) = 0x4A;
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x18) = 0;
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x20) =
        (*(int*)((char*)self + 0x10) | 0x8000) | ((ulong)0xD000 << 46);
    *(ulong*)(*(char**)((char*)self + 0xC) + 0x28) = 0x521;
    *(char**)((char*)self + 0xC) += 0x30;
}
#endif

INCLUDE_ASM("render/particle", func_00384E50);

//100%
INCLUDE_ASM("render/particle", func_00384FD0);
#ifdef SKIP_ASM
struct sPartGsCtx {
    int field_0x0;
    ulong* p;            // 0x4
    int field_0x8;
    int field_0xc;
    int count;           // 0x10
};

struct sPartGsVtx {
    float s;             // 0x0
    float t;             // 0x4
    int field_0x8;
    int field_0xc;
    unsigned int r;      // 0x10
    unsigned int g;      // 0x14
    unsigned int b;      // 0x18
    unsigned int a;      // 0x1c
};

struct sPartGsPos {
    float x, y, z, q;
};

// PORT: 64-bit `ulong` GS packet words; float bits reinterpreted via *(int*)&f.
extern "C" void func_00384FD0(sPartGsCtx* ctx, sPartGsVtx* v, sPartGsPos* pos)
{
    ctx->p[0] = (ulong)v->r | ((ulong)v->g << 8) | ((ulong)v->b << 16) | ((ulong)v->a << 24)
              | ((ulong)*(int*)&pos->q << 32);
    union { float f; int i; } us, ut;
    us.f = v->s * pos->q;
    ut.f = v->t * pos->q;
    ctx->p[1] = (ulong)us.i | ((ulong)ut.i << 32);
    ctx->p[2] = (ulong)(int)pos->x | ((ulong)(int)pos->y << 16) | ((ulong)(int)(pos->z * pos->q) << 32);
    ctx->p += 3;
    ctx->count++;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", func_003850A8);
#ifdef SKIP_ASM
// PORT: packet pointers held in int fields (field_0x8 / field_0xc).
extern "C" void func_003850A8(sPartGsCtx* ctx, sPartGsVtx* v, sPartGsPos* pos)
{
    if (ctx->field_0x8 == 0) {
        ulong* p = ctx->p;
        ctx->field_0x8 = (int)p;
        ctx->p = p + 4;
    } else if (ctx->field_0xc != 0) {
        func_00384DC0(ctx);
    }
    ulong* q = ctx->p;
    ctx->count = 0;
    ctx->field_0xc = (int)q;
    ctx->p = q + 6;
    func_00384FD0(ctx, v, pos);
}
#endif

INCLUDE_ASM("render/particle", func_00385138);

//100%
INCLUDE_ASM("render/particle", func_00385260);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern char D_00493938[];

extern "C" void func_00385260(int* self, int flags)
{
    *(void**)((char*)self + 0x4) = D_00493938;
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", func_00385290);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
extern void* D_00493208[];

extern "C" void func_00385290(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_00493208;
    if (*(void**)((char*)self + 0x19C) != 0) {
        cMemMan_free(*(void**)((char*)self + 0x19C));
    }
    func_00385260((int*)self, flags);
}
#endif

INCLUDE_ASM("render/particle", func_003852E8);

INCLUDE_ASM("render/particle", func_00385410);

INCLUDE_ASM("render/particle", func_00385530);

INCLUDE_ASM("render/particle", func_003856B8);

//100%
INCLUDE_ASM("render/particle", func_00385A38);
#ifdef SKIP_ASM
struct sPartVec3 {
    float x, y, z;
};

extern "C" void func_00385A38(void* self, short* out, int i, sPartVec3* v)
{
    float s = *(float*)((char*)self + 0x198);
    sPartVec3 t;
    t.x = v->x * s;
    t.y = v->y * s;
    t.z = v->z * s;
    int j = i * 3;
    out[j] = (short)t.x;
    out[j + 1] = (short)t.y;
    out[j + 2] = (short)t.z;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00385AA0);
#ifdef SKIP_ASM
// PORT: g++ `<?` (min) operator.
static inline float partClampf(float x, float lo, float hi)
{
    if (x >= lo) {
        return x <? hi;
    }
    return lo;
}

extern "C" void func_00385AA0(void* self, unsigned short* buf, int idx, const float* c)
{
    unsigned char r = (int)(partClampf(c[1], 0.0f, 1.0f) * 32.0f);
    unsigned char g = (int)(partClampf(c[2], 0.0f, 1.0f) * 32.0f);
    unsigned char b = (int)(partClampf(c[3], 0.0f, 1.0f) * 32.0f);
    unsigned char a = (int)(partClampf(c[0], 0.0f, 1.0f) * 32.0f);
    if (r > 31) r = 31;
    if (g > 31) g = 31;
    if (b > 31) b = 31;
    if (a > 0) a = 1;
    buf[idx] = (a << 15) | (b << 10) | (g << 5) | r;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00385BA0);
#ifdef SKIP_ASM
struct sShortUV {
    short u;
    short v;
};

extern "C" void func_00385BA0(void* self, sShortUV* dst, int idx, float* src)
{
    dst[idx].u = (short)(src[0] * 4096.0f);
    dst[idx].v = (short)(src[1] * 4096.0f);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00385BE0);
#ifdef SKIP_ASM
class cPartVirt {
public:
    int field_0x0;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08(int, int, int);
};

extern "C" void func_00385BE0(cPartVirt* self, int a, void* b, int c)
{
    self->v08(a, **(short**)((char*)b + 4), c);
}
#endif

INCLUDE_ASM("render/particle", func_00385C10);

INCLUDE_ASM("render/particle", func_00385EB0);

INCLUDE_ASM("render/particle", func_00386128);

//100%
INCLUDE_ASM("render/particle", func_00386640);
#ifdef SKIP_ASM
extern "C" void func_0036ABA0(void* self, int a1);
extern "C" void func_00390458(void* self, int a1);
extern "C" void func_0036C740(void* self, int a1);

extern "C" void func_00386640(void* self, int a1)
{
    func_0036ABA0(self, a1);
    func_00390458(self, a1);
    func_0036C740(self, a1);
}
#endif

INCLUDE_ASM("render/particle", func_00386688);

//100%
INCLUDE_ASM("render/particle", func_003866E0__FPvii);
#ifdef SKIP_ASM
void func_003866E0(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x6d0c) = a1;
    *(int*)((char*)self + 0x6d10) = a2;
}
#endif

INCLUDE_ASM("render/particle", func_003866F0);

INCLUDE_ASM("render/particle", func_00386BD0);

//100%
INCLUDE_ASM("render/particle", func_00386CF0);
#ifdef SKIP_ASM
void func_00369FF0(void* self);

extern "C" void func_00386CF0(void* self)
{
    func_00369FF0(self);
}
#endif

INCLUDE_ASM("render/particle", func_00386D10);

INCLUDE_ASM("render/particle", func_00386DD0);

INCLUDE_ASM("render/particle", func_00386E78);

INCLUDE_ASM("render/particle", func_00387EC0);

INCLUDE_ASM("render/particle", func_003883B8);

INCLUDE_ASM("render/particle", func_003885E0);

INCLUDE_ASM("render/particle", func_003889F0);

//100%
INCLUDE_ASM("render/particle", func_00389098);
#ifdef SKIP_ASM
extern "C" int func_004139F8(float f);

extern "C" void func_00389098(void* self, const float* c)
{
    float k = 255.0f;
    int r = func_004139F8(c[0] * k);
    r |= func_004139F8(c[1] * k) << 8;
    r |= func_004139F8(c[2] * k) << 16;
    *(int*)((char*)self + 0x6AE0) = r;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389118);
#ifdef SKIP_ASM
extern "C" void func_00424880(int);
extern "C" void func_00423AA0(int, int);
extern "C" void func_00424950(int);
extern "C" void func_00423AD0(int, int);

extern "C" void func_00389118(void* self)
{
    func_00424880(2);
    func_00423AA0(2, *(int*)((char*)self + 0x5AC0));
    func_00424950(1);
    func_00423AD0(0, *(int*)((char*)self + 0x5AC4));
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389260);
#ifdef SKIP_ASM
struct sPartVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" void func_00389260(sPartVec4* v)
{
    for (int i = 0; i < 10; i++) {
        v[i].x = v[i].y = v[i].z = v[i].w = 0.0f;
    }
}
#endif

INCLUDE_ASM("render/particle", func_00389308);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("render/particle", func_00389520);
#ifdef SKIP_ASM
extern "C" void func_00389308(void* self, void* dst, float w, float x, float y, float z);

extern "C" void func_00389520(void* self, const float* v, void* dst)
{
    func_00389308(self, dst, 1.0f, v[0], v[1], v[2]);
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389558);
#ifdef SKIP_ASM
extern "C" void func_00389558(float* a, float* b)
{
    a[0] += b[0];
    a[1] += b[1];
    a[2] += b[2];
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389590);
#ifdef SKIP_ASM
extern "C" void func_00389590(sPartVec4* a, sPartVec4* b, sPartVec4* c, float s)
{
    float kx = s * c->x;
    float ky = s * c->y;
    float kz = s * c->z;
    float kw = s * c->w;
    for (int i = 0; i < 10; i++) {
        a[i].x += b[i].x * ky;
        a[i].y += b[i].y * kz;
        a[i].z += b[i].z * kw;
        a[i].w += b[i].w * kx;
    }
}
#endif

INCLUDE_ASM("render/particle", func_00389620);

//100%
INCLUDE_ASM("render/particle", func_00389730);
#ifdef SKIP_ASM
struct sPartVec4x10 {
    sPartVec4 v[10];
};

// PORT: VU0 macro-mode vector add; the PC port needs plain C.
static inline sPartVec4 sPartVec4_add(const sPartVec4& a, const sPartVec4& b)
{
    sPartVec4 r;
    __asm__(
        "lqc2       $vf3, 0x0(%1)\n"
        "lqc2       $vf4, 0x0(%2)\n"
        "vadd.xyzw  $vf5, $vf3, $vf4\n"
        "sqc2       $vf5, %0\n"
        : "=m"(r)
        : "r"(&a), "r"(&b));
    return r;
}

extern "C" sPartVec4x10 func_00389730(sPartVec4x10* a, sPartVec4x10* b)
{
    sPartVec4x10 r;
    for (int i = 0; i < 10; i++) {
        r.v[i] = sPartVec4_add(a->v[i], b->v[i]);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_003897A0);
#ifdef SKIP_ASM
// PORT: VU0 macro-mode vector scale; the PC port needs plain C.
static inline sPartVec4 sPartVec4_scale(const sPartVec4& a, float s)
{
    sPartVec4 r;
    __asm__(
        "mfc1       $2, %2\n"
        "lqc2       $vf4, 0x0(%1)\n"
        "qmtc2.ni   $2, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2       $vf5, %0\n"
        : "=m"(r)
        : "r"(&a), "f"(s)
        : "$2");
    return r;
}

extern "C" sPartVec4x10 func_003897A0(sPartVec4x10* a, float s)
{
    sPartVec4x10 r;
    for (int i = 0; i < 10; i++) {
        r.v[i] = sPartVec4_scale(a->v[i], s);
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_00389810);
#ifdef SKIP_ASM
// PORT: VU0 macro-mode vector scale (vec4 *= s); the PC port needs plain C.
extern "C" sPartVec4* func_00389810(sPartVec4* v, float s)
{
    for (int i = 0; i < 10; i++) {
        __asm__ __volatile__(
            "mfc1       $2, %1\n"
            "lqc2       $vf4, 0x0(%0)\n"
            "qmtc2.ni   $2, $vf3\n"
            "vmulx.xyzw $vf5, $vf4, $vf3x\n"
            "sqc2       $vf5, 0x0(%0)\n"
            :
            : "r"(&v[i]), "f"(s)
            : "$2", "memory");
    }
    return v;
}
#endif

INCLUDE_ASM("render/particle", func_00389840);

INCLUDE_ASM("render/particle", func_00389C38);

INCLUDE_ASM("render/particle", func_00389CB8);

//100%
INCLUDE_ASM("render/particle", func_0038A530);
#ifdef SKIP_ASM
extern "C" int func_0038A530(float* p, float* n, void* s, float* dist, float* inv, float* planeD)
{
    float d;
    float k;
    float dx = *(float*)((char*)s + 0x38) - p[0];
    float dy = *(float*)((char*)s + 0x3C) - p[1];
    float r = *(float*)((char*)s + 0x1C);
    float dz = *(float*)((char*)s + 0x40) - p[2];
    float d2 = dx * dx + dy * dy + dz * dz;
    if (r * r < d2) {
        return 0;
    }
    if (d2 != 0.0f) {
        // PORT: sqrt.s (sqrtf without errno check)
        __asm__("sqrt.s %0, %1" : "=f"(d) : "f"(d2));
        k = 1.0f / d;
    } else {
        k = 1.0f;
        d = k;
    }
    dx *= k;
    dy *= k;
    dz *= k;
    *planeD = -(dx * *(float*)((char*)s + 0x2C) + dy * *(float*)((char*)s + 0x30) + dz * *(float*)((char*)s + 0x34));
    *dist = d;
    *inv = k;
    n[0] = dx;
    n[1] = dy;
    n[2] = dz;
    n[3] = 1.0f;
    return 1;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0038A618);
#ifdef SKIP_ASM
extern "C" int func_0038A618(float* p, float* n, void* s, float* dist, float* inv)
{
    float dx = *(float*)((char*)s + 0x38) - p[0];
    float dy = *(float*)((char*)s + 0x3C) - p[1];
    float r = *(float*)((char*)s + 0x1C);
    float dz = *(float*)((char*)s + 0x40) - p[2];
    float d2 = dx * dx + dy * dy + dz * dz;
    if (r * r < d2) {
        return 0;
    }
    float d;
    // PORT: sqrt.s (sqrtf without errno check)
    __asm__("sqrt.s %0, %1" : "=f"(d) : "f"(d2));
    *dist = d;
    float k = 1.0f / d;
    *inv = k;
    dy *= k;
    dz *= k;
    dx *= k;
    n[3] = 1.0f;
    n[0] = dx;
    n[1] = dy;
    n[2] = dz;
    return 1;
}
#endif

INCLUDE_ASM("render/particle", func_0038A6A8);

//100%
INCLUDE_ASM("render/particle", func_0038ABF8);
#ifdef SKIP_ASM
struct sParticleEntryA0 {
    char pad_0x00[0xA0];
};

extern "C" sParticleEntryA0* func_0038ABF8(void* self, int i)
{
    int count = *(int*)((char*)self + 0x4);
    if (i >= count) {
        i = count - 1;
    }
    return *(sParticleEntryA0**)((char*)self + 0x8) + i;
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0038AC20);
#ifdef SKIP_ASM
extern "C" int func_0038AC50(void* self, const char* name);

extern "C" sParticleEntryA0* func_0038AC20(void* self, const char* name)
{
    return func_0038ABF8(self, func_0038AC50(self, name));
}
#endif

//100%
INCLUDE_ASM("render/particle", func_0038AC50);
#ifdef SKIP_ASM
extern "C" int func_004165A8(const void* a, const void* b);

extern "C" int func_0038AC50(void* self, const char* name)
{
    int i;
    for (i = 0; i < *(int*)((char*)self + 0x4); i++) {
        if (func_004165A8(name, *(char**)((char*)self + 0xC) + (i << 3)) == 0) {
            return i;
        }
    }
    return 0;
}
#endif

