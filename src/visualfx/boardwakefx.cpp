#include "common.h"

INCLUDE_ASM("visualfx/boardwakefx", cBoardWakeFX_cBoardWakeFX);

INCLUDE_ASM("visualfx/boardwakefx", func_002DCDA0);

INCLUDE_ASM("visualfx/boardwakefx", func_002DCF28);

INCLUDE_ASM("visualfx/boardwakefx", func_002DD0B8);

INCLUDE_ASM("visualfx/boardwakefx", func_002DDAB8);

INCLUDE_ASM("visualfx/boardwakefx", func_002DDD30);

INCLUDE_ASM("visualfx/boardwakefx", func_002DE058);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002DE368);
#ifdef SKIP_ASM
struct sVec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

extern sVec4 D_004FF120;

extern "C" void func_002DE368(void* self)
{
    *(int*)((char*)self + 0x34) = 0;
    *(int*)((char*)self + 0x30) = 0;
    *(sVec4*)((char*)self + 0x0) = D_004FF120;
    *(sVec4*)((char*)self + 0x20) = D_004FF120;
    *(sVec4*)((char*)self + 0x10) = D_004FF120;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002DE398);

INCLUDE_ASM("visualfx/boardwakefx", func_002DE4A8);

INCLUDE_ASM("visualfx/boardwakefx", func_002DF1C8);

INCLUDE_ASM("visualfx/boardwakefx", func_002DF3B0);

INCLUDE_ASM("visualfx/boardwakefx", func_002DF448);

INCLUDE_ASM("visualfx/boardwakefx", func_002DF4D0);

INCLUDE_ASM("visualfx/boardwakefx", func_002DF920);

INCLUDE_ASM("visualfx/boardwakefx", func_002DFE88);

INCLUDE_ASM("visualfx/boardwakefx", func_002E02B8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E0EE8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E1120);

INCLUDE_ASM("visualfx/boardwakefx", func_002E1598);

INCLUDE_ASM("visualfx/boardwakefx", func_002E1A80);

INCLUDE_ASM("visualfx/boardwakefx", func_002E1F70);

INCLUDE_ASM("visualfx/boardwakefx", func_002E2260);

INCLUDE_ASM("visualfx/boardwakefx", func_002E23E0);

INCLUDE_ASM("visualfx/boardwakefx", func_002E24D0);

INCLUDE_ASM("visualfx/boardwakefx", func_002E2550);

INCLUDE_ASM("visualfx/boardwakefx", func_002E26B8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E27E8);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E2860__FPv);
#ifdef SKIP_ASM
void* func_002E2860(void* self)
{
    return self;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E2868);

INCLUDE_ASM("visualfx/boardwakefx", func_002E2B00);

INCLUDE_ASM("visualfx/boardwakefx", func_002E2E18);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E2F98__FPvi);
#ifdef SKIP_ASM
void func_002E2F98(void* self, int i)
{
    *(int*)((char*)self + (i << 2) + 0x60) = 0;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E2FA8);
#ifdef SKIP_ASM
struct sWakePoint {
    int a;
    int pad04;
    int c;
    short d;
    short e;
    char pad10[0x20];
};

struct sWakeFx {
    char pad00[0x60];
    int counts[10];
    sWakePoint points[1][256];
};

extern "C" void func_002E2FA8(sWakeFx* self, int v, int i)
{
    int n = self->counts[i];
    self->points[i][n].a = v;
    self->points[i][n].d = 0;
    self->points[i][n].e = 0;
    self->points[i][n].c = 0;
    self->counts[i]++;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E2FF8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3060);

INCLUDE_ASM("visualfx/boardwakefx", func_002E30D0);

extern "C" void* func_002E3130(void* self);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E3110__FPv);
#ifdef SKIP_ASM
void* func_002E3110(void* self)
{
    return func_002E3130(self);
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E3130);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3338);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3478);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3578);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3668);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3680);

INCLUDE_ASM("visualfx/boardwakefx", func_002E37D0);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3930);

INCLUDE_ASM("visualfx/boardwakefx", func_002E39D8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E3AF8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E4228);

INCLUDE_ASM("visualfx/boardwakefx", func_002E42D0);

INCLUDE_ASM("visualfx/boardwakefx", func_002E4370);

INCLUDE_ASM("visualfx/boardwakefx", func_002E44F0);

INCLUDE_ASM("visualfx/boardwakefx", func_002E4540);

INCLUDE_ASM("visualfx/boardwakefx", func_002E4578);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4638);
#ifdef SKIP_ASM
extern "C" void func_002E4638(void* self, int up)
{
    if (*(int*)((char*)self + 0x44) == 0) {
        *(int*)((char*)self + 0x40) = 0;
        return;
    }
    if (up) {
        *(int*)((char*)self + 0x40) += 1;
        return;
    }
    if (*(int*)((char*)self + 0x40) != 0) {
        *(int*)((char*)self + 0x40) -= 1;
    }
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4678);
#ifdef SKIP_ASM
extern "C" float func_002E4678(void* self)
{
    if (*(int*)((char*)self + 0x44) == 0) {
        return 0.0f;
    }
    float v = *(float*)((char*)self + 0x54) + *(float*)((char*)self + 0x58)
            + *(float*)((char*)self + 0x5C) - *(float*)((char*)self + 0x48);
    if (v < 0.0f) {
        return 0.0f;
    }
    return v;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E46C8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E47E8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E4B98);

INCLUDE_ASM("visualfx/boardwakefx", func_002E4C60);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4C90);
#ifdef SKIP_ASM
extern "C" int func_002E4C90(void* self)
{
    int r = 0;
    if (*(int*)((char*)self + 0x60) != 0) {
        r = *(int*)((char*)self + 0x64) != 0;
    }
    return r;
}
#endif

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4CB0);
#ifdef SKIP_ASM
extern int D_00538850[];

extern "C" void func_002E4CB0(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        D_00538850[i] = 0;
    }
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E4CE8);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4D30);
#ifdef SKIP_ASM
struct sWakeSlot {
    char pad[0x64];
};

extern sWakeSlot D_005382C0[];
extern int D_00538850[];

extern "C" void func_002E4D30(void* p)
{
    int i;
    if (p == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (&D_005382C0[i] == p) {
            D_00538850[i] = 0;
            return;
        }
    }
}
#endif

extern void* D_004880E0[];

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4D70__FPv);
#ifdef SKIP_ASM
void* func_002E4D70(void* self)
{
    *(int*)self = (int)(void*)D_004880E0;
    return self;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E4D88);

//100%
INCLUDE_ASM("visualfx/boardwakefx", func_002E4F10__FPv);
#ifdef SKIP_ASM
void func_002E4F10(void* self)
{
    *(int*)((char*)self + 0x94) = 0;
    *(int*)((char*)self + 0xb0) = 1;
    *(int*)((char*)self + 0x90) = 0;
}
#endif

INCLUDE_ASM("visualfx/boardwakefx", func_002E4F28);

INCLUDE_ASM("visualfx/boardwakefx", func_002E4F50);

INCLUDE_ASM("visualfx/boardwakefx", func_002E5430);

INCLUDE_ASM("visualfx/boardwakefx", func_002E55D8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E5920);

INCLUDE_ASM("visualfx/boardwakefx", func_002E5D18);

INCLUDE_ASM("visualfx/boardwakefx", func_002E5DA0);

INCLUDE_ASM("visualfx/boardwakefx", func_002E6008);

INCLUDE_ASM("visualfx/boardwakefx", func_002E62C8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E6320);

INCLUDE_ASM("visualfx/boardwakefx", func_002E64C0);

INCLUDE_ASM("visualfx/boardwakefx", func_002E6640);

INCLUDE_ASM("visualfx/boardwakefx", func_002E66B8);

INCLUDE_ASM("visualfx/boardwakefx", func_002E6C08);

INCLUDE_ASM("visualfx/boardwakefx", func_002E7A10);

