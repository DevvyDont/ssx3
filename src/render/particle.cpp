#include "common.h"

INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_Allocate);

INCLUDE_ASM("render/particle", func_00370CF8);

INCLUDE_ASM("render/particle", cBaseClass_DynamicEmitter_reset);

INCLUDE_ASM("render/particle", func_00370DC8);

INCLUDE_ASM("render/particle", func_003710D0);

INCLUDE_ASM("render/particle", func_003712B8);

INCLUDE_ASM("render/particle", func_00371318);

INCLUDE_ASM("render/particle", func_00371380);

INCLUDE_ASM("render/particle", func_003714B8);

INCLUDE_ASM("render/particle", func_003714F8);

INCLUDE_ASM("render/particle", cDynamicColourEmitter_Allocate);

INCLUDE_ASM("render/particle", func_003715B0);

INCLUDE_ASM("render/particle", cDynamicColourEmitter_reset);

INCLUDE_ASM("render/particle", func_00371688);

INCLUDE_ASM("render/particle", func_003717C0);

INCLUDE_ASM("render/particle", func_00371940);

INCLUDE_ASM("render/particle", func_00371D10);

INCLUDE_ASM("render/particle", func_00371DD8);

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

INCLUDE_ASM("render/particle", func_00372660);

INCLUDE_ASM("render/particle", func_003726C8);

INCLUDE_ASM("render/particle", func_00372AA0);

INCLUDE_ASM("render/particle", func_00372B30);

INCLUDE_ASM("render/particle", func_00372B78);

INCLUDE_ASM("render/particle", func_003739D0);

INCLUDE_ASM("render/particle", func_00374180);

INCLUDE_ASM("render/particle", func_00374298);

INCLUDE_ASM("render/particle", func_00374440);

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

INCLUDE_ASM("render/particle", func_00374B38);

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

INCLUDE_ASM("render/particle", func_00375A08);

INCLUDE_ASM("render/particle", func_00375A40);

INCLUDE_ASM("render/particle", func_00376268);

INCLUDE_ASM("render/particle", func_003762F8);

INCLUDE_ASM("render/particle", func_00376468);

INCLUDE_ASM("render/particle", func_003764C0);

//100%
INCLUDE_ASM("render/particle", func_00376560);
#ifdef SKIP_ASM
extern "C" void* func_00376560(void* self, int a1)
{
    return *(char**)((char*)self + 0x59cc) + a1 * 0xa4;
}
#endif

INCLUDE_ASM("render/particle", func_00376578);

INCLUDE_ASM("render/particle", func_003765B8);

INCLUDE_ASM("render/particle", func_00376768);

INCLUDE_ASM("render/particle", func_00376938);

INCLUDE_ASM("render/particle", func_00376A70);

INCLUDE_ASM("render/particle", func_00376B90);

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

INCLUDE_ASM("render/particle", func_00377950);

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

INCLUDE_ASM("render/particle", func_0037C7C0);

extern "C" void* func_003E6574(void*, void*, int);

//100%
INCLUDE_ASM("render/particle", func_0037C808__FPv);
#ifdef SKIP_ASM
void* func_0037C808(void* self)
{
    return func_003E6574((char*)self + 0x18f8, (char*)self + 0x3838, 0x1f40);
}
#endif

INCLUDE_ASM("render/particle", func_0037C830);

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

INCLUDE_ASM("render/particle", func_0037CAF8);

INCLUDE_ASM("render/particle", func_0037CB50);

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

INCLUDE_ASM("render/particle", func_0037CBE8);

INCLUDE_ASM("render/particle", func_0037CC30);

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

INCLUDE_ASM("render/particle", func_0037D318);

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

INCLUDE_ASM("render/particle", func_0037D758);

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

INCLUDE_ASM("render/particle", func_0037DE88);

INCLUDE_ASM("render/particle", func_0037DEE0);

INCLUDE_ASM("render/particle", func_0037DF88);

INCLUDE_ASM("render/particle", func_0037E040);

INCLUDE_ASM("render/particle", func_0037E098);

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

INCLUDE_ASM("render/particle", func_003825C0);

INCLUDE_ASM("render/particle", func_003825F8);

INCLUDE_ASM("render/particle", func_00382650);

INCLUDE_ASM("render/particle", func_00382688);

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

INCLUDE_ASM("render/particle", func_00384DC0);

INCLUDE_ASM("render/particle", func_00384E50);

INCLUDE_ASM("render/particle", func_00384FD0);

INCLUDE_ASM("render/particle", func_003850A8);

INCLUDE_ASM("render/particle", func_00385138);

INCLUDE_ASM("render/particle", func_00385260);

INCLUDE_ASM("render/particle", func_00385290);

INCLUDE_ASM("render/particle", func_003852E8);

INCLUDE_ASM("render/particle", func_00385410);

INCLUDE_ASM("render/particle", func_00385530);

INCLUDE_ASM("render/particle", func_003856B8);

INCLUDE_ASM("render/particle", func_00385A38);

INCLUDE_ASM("render/particle", func_00385AA0);

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

INCLUDE_ASM("render/particle", func_00385BE0);

INCLUDE_ASM("render/particle", func_00385C10);

INCLUDE_ASM("render/particle", func_00385EB0);

INCLUDE_ASM("render/particle", func_00386128);

INCLUDE_ASM("render/particle", func_00386640);

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

INCLUDE_ASM("render/particle", func_00386CF0);

INCLUDE_ASM("render/particle", func_00386D10);

INCLUDE_ASM("render/particle", func_00386DD0);

INCLUDE_ASM("render/particle", func_00386E78);

INCLUDE_ASM("render/particle", func_00387EC0);

INCLUDE_ASM("render/particle", func_003883B8);

INCLUDE_ASM("render/particle", func_003885E0);

INCLUDE_ASM("render/particle", func_003889F0);

INCLUDE_ASM("render/particle", func_00389098);

INCLUDE_ASM("render/particle", func_00389118);

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

INCLUDE_ASM("render/particle", func_00389520);

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

INCLUDE_ASM("render/particle", func_00389590);

INCLUDE_ASM("render/particle", func_00389620);

INCLUDE_ASM("render/particle", func_00389730);

INCLUDE_ASM("render/particle", func_003897A0);

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

INCLUDE_ASM("render/particle", func_0038A530);

INCLUDE_ASM("render/particle", func_0038A618);

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

INCLUDE_ASM("render/particle", func_0038AC20);

INCLUDE_ASM("render/particle", func_0038AC50);

