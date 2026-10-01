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

INCLUDE_ASM("render/ps2graphicsman", func_00367B60);

INCLUDE_ASM("render/ps2graphicsman", func_00367BC0);

INCLUDE_ASM("render/ps2graphicsman", func_00367CD0);

INCLUDE_ASM("render/ps2graphicsman", func_00367D20);

INCLUDE_ASM("render/ps2graphicsman", func_00367DB8);

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

INCLUDE_ASM("render/ps2graphicsman", func_00369130);

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

INCLUDE_ASM("render/ps2graphicsman", func_00369690);

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

INCLUDE_ASM("render/ps2graphicsman", func_003698A0);

