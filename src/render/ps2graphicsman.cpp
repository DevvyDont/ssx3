#include "common.h"

INCLUDE_ASM("render/ps2graphicsman", cPSPGraphicsMan_NewNonBindTexID);

INCLUDE_ASM("render/ps2graphicsman", func_003671C8);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367230);
#ifdef SKIP_ASM
extern "C" void func_00367230(void)
{
    for (int i = 0; i < 500; i++) {
    }
}
#endif

INCLUDE_ASM("render/ps2graphicsman", cPSPGraphicsMan_NewBindTexID);

INCLUDE_ASM("render/ps2graphicsman", func_003672C0);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367310);
#ifdef SKIP_ASM
extern "C" void func_00367310(void)
{
    for (int i = 0; i < 1500; i++) {
    }
}
#endif

extern "C" void* func_00365E40(void*, int, int, void*);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367340__FPvii);
#ifdef SKIP_ASM
void* func_00367340(void* self, int a1, int a2)
{
    return func_00365E40((char*)self + 0x4350, a1, a2, (char*)self + 0x8);
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_00367360);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003673D0__FPv);
#ifdef SKIP_ASM
void func_003673D0(void* self)
{
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_003673D8);

INCLUDE_ASM("render/ps2graphicsman", func_00367440);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367B60);
#ifdef SKIP_ASM
extern "C" void func_00367B60(void* self, int i)
{
    char* obj = *(char**)((char*)self + (i << 2) + 8);
    if (*(int*)(obj + 0x28) != -1) {
        char* sub = *(int*)(obj + 0xC) != 9 ? (char*)self + 0x1F60 : (char*)self + 0x4350;
        char* e = *(char**)(sub + 0x1FF0) + *(int*)(obj + 0x30) * 0x1C;
        *(int*)e &= ~2;
    }
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_00367BC0);

INCLUDE_ASM("render/ps2graphicsman", func_00367CD0);

INCLUDE_ASM("render/ps2graphicsman", func_00367D20);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00367DB8);
#ifdef SKIP_ASM
// PORT: 64-bit GS register words (ulong is 64-bit here).
struct sGsTex0Bits {
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

struct sPs2RenderTex {
    char pad_0x00[0x28];
    int addr;               // 0x28
    char pad_0x2C[0xC];
    sGsTex0Bits tex0;       // 0x38
};

struct sPs2TexSet {
    char pad_0x00[0x8];
    sPs2RenderTex* tex[1];  // 0x08
};

static inline ulong gsSetFrame367DB8(int fbp, int fbw, int psm, int fbmsk)
{
    return (ulong)fbp | ((ulong)fbw << 16) | ((ulong)psm << 24) | ((ulong)fbmsk << 32);
}

extern "C" void func_00367DB8(sPs2TexSet* self, int idx, ulong** pp)
{
    sPs2RenderTex* t = self->tex[idx];
    ulong* p = *pp;
    p[0] = 0x10000005;
    p[1] = 0;
    p[2] = (ulong)0x8800 << 45;
    p[3] = (ulong)0x50000004 << 32;
    p[4] = ((ulong)0x10000000 << 32) | 0x8003;
    p[5] = 0xE;
    p[6] = gsSetFrame367DB8(t->addr >> 5, (1 << t->tex0.TW) >> 6, t->tex0.PSM, 0);
    p[7] = 0x4C;
    p[8] = ((ulong)((1 << t->tex0.TW) - 1) << 16) | ((ulong)((1 << t->tex0.TH) - 1) << 48);
    p[9] = 0x40;
    p[10] = (ulong)((0x800 - ((1 << t->tex0.TW) >> 1)) << 4) |
            ((ulong)((0x800 - ((1 << t->tex0.TH) >> 1)) << 4) << 32);
    p[11] = 0x18;
    p += 12;
    *pp = p;
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_00367F18);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00368138);
#ifdef SKIP_ASM
extern char D_0044B200[];

// PORT: 64-bit `ulong` DMA tag, pointer packed into the upper word
extern "C" void func_00368138(void* self, ulong** pkt)
{
    (*pkt)[0] = ((ulong)(int)D_0044B200 << 32) | 0x30000022;
    (*pkt)[1] = 0;
    *pkt += 2;
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_00368170);

INCLUDE_ASM("render/ps2graphicsman", func_003684F0);

INCLUDE_ASM("render/ps2graphicsman", func_00368660);

INCLUDE_ASM("render/ps2graphicsman", func_00368970);

INCLUDE_ASM("render/ps2graphicsman", func_00369098);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369130);
#ifdef SKIP_ASM
// PORT: 64-bit `ulong` bitfield (GS TEX1 register layout)
struct sGsTex1 {
    ulong LCM : 1;
    ulong pad0 : 1;
    ulong MXL : 3;
    ulong MMAG : 1;
    ulong MMIN : 3;
    ulong MTBA : 1;
    ulong pad1 : 9;
    ulong L : 2;
    ulong pad2 : 11;
    ulong K : 12;
    ulong pad3 : 20;
};

struct sGfxTex1 {
    char pad[0x40];
    sGsTex1 tex1;
};

struct sGfxTexTable1 {
    int pad[2];
    sGfxTex1* entries[1];
};

extern "C" void func_00369130(sGfxTexTable1* self, int idx, int mmin, int mmag, int l, int k)
{
    sGfxTex1* e = self->entries[idx];
    sGsTex1 t = e->tex1;
    t.L = l;
    t.K = k;
    t.MMIN = mmin;
    t.MMAG = mmag;
    e->tex1 = t;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003691B0);
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

struct sGfxTex {
    char pad[0x38];
    sGsTex0 tex0;
};

struct sGfxTexTable {
    int pad[2];
    sGfxTex* entries[1];
};

extern "C" void func_003691B0(sGfxTexTable* self, int idx, int tfx)
{
    if (idx != -1) {
        self->entries[idx]->tex0.TFX = tfx;
    }
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_003691F8);

INCLUDE_ASM("render/ps2graphicsman", func_003695D8);

INCLUDE_ASM("render/ps2graphicsman", func_00369610);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369690);
#ifdef SKIP_ASM
extern "C" void* func_003E6574(void*, void*, int);

extern "C" void func_00369690(void* self, int count, void* src)
{
    *(int*)((char*)self + 0xE80) = count;
    func_003E6574((char*)self + 0x280, src, count * 0xC);
}
#endif

INCLUDE_ASM("render/ps2graphicsman", func_003696C8);

//100%
INCLUDE_ASM("render/ps2graphicsman", func_00369890__FPvii);
#ifdef SKIP_ASM
void func_00369890(void* self, int a1, int a2)
{
    *(int*)((char*)self + 0x27c) = a2;
    *(int*)((char*)self + 0x278) = a1;
    *(int*)((char*)self + 0xe80) = 0;
}
#endif

//100%
INCLUDE_ASM("render/ps2graphicsman", func_003698A0);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

extern "C" void func_003698A0(void* self)
{
    void* p = *(void**)((char*)self + 0xF48);
    if (p != 0) {
        cMemMan_free(p);
    }
    *(void**)((char*)self + 0xF48) = 0;
    *(int*)((char*)self + 0xF4C) = 0;
}
#endif

