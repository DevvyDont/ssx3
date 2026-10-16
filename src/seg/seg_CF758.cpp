#include "common.h"

INCLUDE_ASM("seg/seg_CF758", setChallengeName);

INCLUDE_ASM("seg/seg_CF758", setChallengeStats);

//100%
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_ED90(void* be, int kind) __asm__("cBE_getInterface__Fv");
signed char cBERewardInterface_getTrackMedal(void* self, int a, int b, int track, int kind);
extern "C" int cBERewardInterface_getEarningsMedal(void* self, int a, int b, int kind);
extern "C" int func_001589B0(void* self, int a, int b, int kind);
extern "C" int func_00158A50(void* self, int a, int b, int kind);
struct sReq_ED90
{
    signed char track;
    signed char kind;
};
extern sReq_ED90 D_0045AAD8_ED90[][4][5] __asm__("D_0045AAD8");

extern "C" void func_001CED90(int arg0, int arg1, int arg2, int arg3, int arg4)
{
    void* r = cBE_getInterface_ED90(cBE_getBE(), 0xD);
    if (arg3 == 2)
    {
        if (arg4 == 0)
        {
            func_001589B0(r, arg0, arg1, arg2);
            return;
        }
        func_00158A50(r, arg0, arg1, arg2);
        return;
    }
    if (arg3 != 3)
    {
        int kind = D_0045AAD8_ED90[arg2][arg3][arg4].kind;
        int track = D_0045AAD8_ED90[arg2][arg3][arg4].track;
        cBERewardInterface_getTrackMedal(r, arg0, arg1, track, kind);
        return;
    }
    cBERewardInterface_getEarningsMedal(r, arg0, arg1, arg2);
}

//100%
extern "C" void* func_0039E2A0(void* self, void* engine);
extern "C" unsigned char func_001A1CD0(void* self, signed char a1);
extern "C" int func_00146D98(void* self, int index);
extern "C" int func_00150928(void* self, int a, int b);
int cBENewPlayerInterface_getPlayerCharID(void* self, int index);
int cBELibrary_getRiderIndex(int);
extern void* D_0046A3C8[];
struct sVE_CEEA0
{
    short delta;
    short index;
    void (*fn)(void*);
};

extern "C" void* func_001CEEA0(void* self, void* engine, signed char idx, int a4, int a5)
{
    char* s = (char*)self;
    func_0039E2A0(self, engine);
    *(int*)(s + 0xC) = 0x2F;
    *(void***)(s + 0x8) = D_0046A3C8;
    *(signed char*)(s + 0x44) = idx;
    *(unsigned char*)(s + 0x15) = func_001A1CD0(**(void***)(s + 0x10), idx);
    *(int*)(s + 0x4C) = a5;
    *(int*)(s + 0x58) = 4;
    *(int*)(s + 0x5C) = 8;
    *(int*)(s + 0x8EC) = -2;
    *(int*)(s + 0x48) = 0;
    *(int*)(s + 0x64) = 0;
    *(int*)(s + 0x8F4) = 0;
    *(int*)(s + 0x8F8) = 0;
    *(int*)(s + 0x70) = 0;
    *(int*)(s + 0x74) = 0;
    *(int*)(s + 0x8FC) = 0;
    *(int*)(s + 0x900) = 0;
    *(int*)(s + 0x904) = 0;
    *(int*)(s + 0x908) = 0;
    *(int*)(s + 0x90C) = 0;
    *(int*)(s + 0x910) = 0;
    *(int*)(s + 0x914) = 0;
    *(int*)(s + 0x918) = 0;
    *(int*)(s + 0x91C) = 0;
    *(int*)(s + 0x920) = 0;
    *(int*)(s + 0x68) = 0;
    *(int*)(s + 0x6C) = 0;
    *(int*)(s + 0x78) = 0;
    int(*ta)[8] = (int(*)[8])(s + 0x924);
    int(*tb)[8] = (int(*)[8])(s + 0x9A4);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            ta[i][j] = 0;
            tb[i][j] = 0;
        }
    }
    char* pi = (char*)cBE_getInterface_ED90(cBE_getBE(), 1);
    *(int*)(s + 0x50) = cBENewPlayerInterface_getPlayerCharID(pi, *(signed char*)(s + 0x44));
    if (a4 != 0)
    {
        switch (func_00146D98(pi, cBELibrary_getRiderIndex(*(signed char*)(s + 0x44))))
        {
        case 0x11:
            *(int*)(s + 0x8F0) = 1;
            break;
        case 0x12:
            *(int*)(s + 0x8F0) = 1;
            break;
        case 0x13:
            *(int*)(s + 0x8F0) = 2;
            break;
        case 0x14:
            *(int*)(s + 0x8F0) = 2;
            break;
        case 0x15:
            *(int*)(s + 0x8F0) = 3;
            break;
        default:
            *(int*)(s + 0x8F0) = 1;
            break;
        }
    }
    else
    {
        *(int*)(s + 0x8F0) = -1;
    }
    sVE_CEEA0* e = (sVE_CEEA0*)(*(char**)(pi + 0xC) + 0x10);
    e->fn(pi + e->delta);
    *(int*)(s + 0x54) = func_00150928(cBE_getInterface_ED90(cBE_getBE(), 0xB), *(signed char*)(s + 0x44), *(int*)(s + 0x50));
    return self;
}
