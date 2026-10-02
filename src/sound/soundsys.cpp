#include "common.h"

INCLUDE_ASM("sound/soundsys", cBankSys_cBankSys);

INCLUDE_ASM("sound/soundsys", func_0028FEA0);

INCLUDE_ASM("sound/soundsys", func_002906B8);

INCLUDE_ASM("sound/soundsys", func_00290B58);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_00290C10);
#ifdef SKIP_ASM
struct func_00290B58_sCurve {
    float x[5];
    float y[5];
};

extern "C" float func_00290B58(func_00290B58_sCurve* c, float v);
extern func_00290B58_sCurve D_00445898[];

extern "C" float func_00290C10(int i, float v)
{
    return func_00290B58(&D_00445898[i], v);
}
#endif

INCLUDE_ASM("sound/soundsys", func_00290C40);

INCLUDE_ASM("sound/soundsys", func_00290CC0);

INCLUDE_ASM("sound/soundsys", func_00290F58);

INCLUDE_ASM("sound/soundsys", func_00290FD0);

INCLUDE_ASM("sound/soundsys", func_002910E0);

INCLUDE_ASM("sound/soundsys", func_002913D8);

INCLUDE_ASM("sound/soundsys", func_00291438);

INCLUDE_ASM("sound/soundsys", func_00291710);

//100%
INCLUDE_ASM("sound/soundsys", func_002917B8);
#ifdef SKIP_ASM
extern "C" int func_002917B8(void* self, int a1)
{
    if (a1 > 3) {
        return 1;
    }
    return 0x10;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002917D0);

INCLUDE_ASM("sound/soundsys", func_00291C88);

INCLUDE_ASM("sound/soundsys", func_00292508);

INCLUDE_ASM("sound/soundsys", func_002929D8);

INCLUDE_ASM("sound/soundsys", func_00292A50);

INCLUDE_ASM("sound/soundsys", func_00292AE8);

INCLUDE_ASM("sound/soundsys", func_00292B48);

INCLUDE_ASM("sound/soundsys", func_00294170);

INCLUDE_ASM("sound/soundsys", func_00294678);

INCLUDE_ASM("sound/soundsys", func_002947B0);

//100%
INCLUDE_ASM("sound/soundsys", func_00294880);
#ifdef SKIP_ASM
extern "C" void func_00294880(void* self, int i, int v)
{
    if (i < 6) {
        *(int*)((char*)self + (i << 2) + 0x59e8) = v;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002948A0);
#ifdef SKIP_ASM
extern "C" void func_002948A0(void* self)
{
    int i;
    for (i = 5; i >= 0; i--) {
        *(int*)((char*)self + (i << 2) + 0x59e8) = 0;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002948D0);

//100%
INCLUDE_ASM("sound/soundsys", func_00294F48);
#ifdef SKIP_ASM
struct sSndVEntry294F48 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

extern "C" int func_00294F48(void* self)
{
    sSndVEntry294F48* vt = *(sSndVEntry294F48**)((char*)self + 0xC);
    return vt[1].fn((char*)self + vt[1].delta, 0, 0);
}
#endif

INCLUDE_ASM("sound/soundsys", func_00294F78);

INCLUDE_ASM("sound/soundsys", func_00295028);

INCLUDE_ASM("sound/soundsys", func_00295208);

INCLUDE_ASM("sound/soundsys", func_00295628);

INCLUDE_ASM("sound/soundsys", func_00295950);

INCLUDE_ASM("sound/soundsys", func_00296088);

INCLUDE_ASM("sound/soundsys", func_002961F0);

INCLUDE_ASM("sound/soundsys", func_00296310);

INCLUDE_ASM("sound/soundsys", func_00296868);

INCLUDE_ASM("sound/soundsys", func_00296E20);

INCLUDE_ASM("sound/soundsys", func_00296E80);

INCLUDE_ASM("sound/soundsys", func_00297438);

INCLUDE_ASM("sound/soundsys", func_002974A0);

INCLUDE_ASM("sound/soundsys", func_00297950);

INCLUDE_ASM("sound/soundsys", func_00297EB8);

//100%
INCLUDE_ASM("sound/soundsys", func_00297F70);
#ifdef SKIP_ASM
extern "C" void func_00297F70(void* self)
{
    int i;
    for (i = 29; i >= 0; i--) {
        *(int*)((char*)self + i * 0x30 + 0x5a00) = 0;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_00297FA0);

INCLUDE_ASM("sound/soundsys", func_002980B0);

INCLUDE_ASM("sound/soundsys", func_00298138);

INCLUDE_ASM("sound/soundsys", func_00298488);

INCLUDE_ASM("sound/soundsys", func_002989A8);

INCLUDE_ASM("sound/soundsys", func_00298D00);

INCLUDE_ASM("sound/soundsys", func_00298D90);

INCLUDE_ASM("sound/soundsys", func_002992D8);

INCLUDE_ASM("sound/soundsys", func_00299368);

INCLUDE_ASM("sound/soundsys", func_00299638);

INCLUDE_ASM("sound/soundsys", func_002997B8);

INCLUDE_ASM("sound/soundsys", func_00299B70);

INCLUDE_ASM("sound/soundsys", func_00299E28);

INCLUDE_ASM("sound/soundsys", func_0029A220);

INCLUDE_ASM("sound/soundsys", func_0029A530);

INCLUDE_ASM("sound/soundsys", func_0029A7D8);

//100%
INCLUDE_ASM("sound/soundsys", func_0029AB08);
#ifdef SKIP_ASM
extern "C" void func_0029AB08(void* self, void* obj)
{
    bool ok = *(int*)((char*)obj + 0x874) && *(int*)((char*)obj + 0x87C);
    if (ok) {
        *(short*)((char*)self + (*(int*)((char*)obj + 0x870) << 1) + 0x5FE8) = 0x1000;
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029AB40);

INCLUDE_ASM("sound/soundsys", func_0029B0E0);

INCLUDE_ASM("sound/soundsys", func_0029B3C0);

INCLUDE_ASM("sound/soundsys", func_0029B430);

INCLUDE_ASM("sound/soundsys", func_0029B738);

//100%
INCLUDE_ASM("sound/soundsys", func_0029B7E0);
#ifdef SKIP_ASM
extern "C" void func_002A3DE0(void* self, void* obj, int ev);

extern "C" void func_0029B7E0(void* self, void* obj)
{
    int ok = 0;
    if (*(int*)((char*)obj + 0x874) != 0) {
        ok = *(int*)((char*)obj + 0x87C) != 0;
    }
    if (ok) {
        func_002A3DE0(self, obj, 8);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029B818);

//100%
INCLUDE_ASM("sound/soundsys", func_0029B960__FPv);
#ifdef SKIP_ASM
void func_0029B960(void* self)
{
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029B968);

INCLUDE_ASM("sound/soundsys", func_0029BCF8);

INCLUDE_ASM("sound/soundsys", func_0029C088);

//100%
INCLUDE_ASM("sound/soundsys", func_0029C418__FPv);
#ifdef SKIP_ASM
void func_0029C418(void* self)
{
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029C420);

INCLUDE_ASM("sound/soundsys", func_0029C7B0);

INCLUDE_ASM("sound/soundsys", func_0029CCA8);

INCLUDE_ASM("sound/soundsys", func_0029CE28);

INCLUDE_ASM("sound/soundsys", func_0029CE70);

INCLUDE_ASM("sound/soundsys", func_0029CED8);

INCLUDE_ASM("sound/soundsys", func_0029D290);

INCLUDE_ASM("sound/soundsys", func_0029D370);

INCLUDE_ASM("sound/soundsys", func_0029D610);

INCLUDE_ASM("sound/soundsys", func_0029D678);

//100%
INCLUDE_ASM("sound/soundsys", func_0029D6D0__FPv);
#ifdef SKIP_ASM
void func_0029D6D0(void* self)
{
    *(int*)((char*)self + 0x5FD8) = 1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029D6E0);

INCLUDE_ASM("sound/soundsys", func_0029D8E0);

INCLUDE_ASM("sound/soundsys", func_0029DBB0);

INCLUDE_ASM("sound/soundsys", func_0029DC48);

INCLUDE_ASM("sound/soundsys", func_0029DEF0);

INCLUDE_ASM("sound/soundsys", func_0029E438);

//100%
INCLUDE_ASM("sound/soundsys", func_0029E560__FPv);
#ifdef SKIP_ASM
void func_0029E560(void* self)
{
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029E578);
#ifdef SKIP_ASM
extern "C" void func_0029E578(void* self, void* a1)
{
    int idx = *(int*)((char*)a1 + 0x870);
    self = (char*)self + idx * 4;
    *(int*)((char*)self + 0x6080) = 0;
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029E590);

INCLUDE_ASM("sound/soundsys", func_0029E970);

INCLUDE_ASM("sound/soundsys", func_0029ED18);

INCLUDE_ASM("sound/soundsys", func_0029ED90);

INCLUDE_ASM("sound/soundsys", func_0029EEE0);

INCLUDE_ASM("sound/soundsys", func_0029F000);

//100%
INCLUDE_ASM("sound/soundsys", func_0029F088);
#ifdef SKIP_ASM
extern "C" int func_0029F088(void* self, int a1)
{
    if (a1 == 0) {
        return *(int*)((char*)self + 0x5738);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F0A0);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F0A0(void* self)
{
    if (func_00287920(self, 4) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029F0D8);

//100%
INCLUDE_ASM("sound/soundsys", func_0029F128);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F128(void* self)
{
    if (func_00287920(self, 3) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_0029F160);
#ifdef SKIP_ASM
extern "C" float func_00287920(void* self, int ch);

extern "C" int func_0029F160(void* self)
{
    if (func_00287920(self, 0xA) == 0.0f) {
        return 0;
    }
    return 1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029F198);

INCLUDE_ASM("sound/soundsys", func_0029F2B0);

INCLUDE_ASM("sound/soundsys", func_0029F330);

INCLUDE_ASM("sound/soundsys", func_0029F378);

INCLUDE_ASM("sound/soundsys", func_0029F3F8);

INCLUDE_ASM("sound/soundsys", func_0029F5E0);

//100%
INCLUDE_ASM("sound/soundsys", func_0029F620);
#ifdef SKIP_ASM
struct sSndVEntry29F620 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_0029F620(void* self, void* obj)
{
    void* sub = (char*)obj + 0x6C0;
    sSndVEntry29F620* vt = *(sSndVEntry29F620**)sub;
    if (vt[7].fn((char*)sub + vt[7].delta) == 0) {
        return 0xE;
    }
    return 0xF;
}
#endif

INCLUDE_ASM("sound/soundsys", func_0029F660);

INCLUDE_ASM("sound/soundsys", func_0029FCC8);

INCLUDE_ASM("sound/soundsys", func_0029FF80);

INCLUDE_ASM("sound/soundsys", func_002A02D8);

INCLUDE_ASM("sound/soundsys", func_002A0560);

INCLUDE_ASM("sound/soundsys", func_002A0A30);

INCLUDE_ASM("sound/soundsys", func_002A0E70);

INCLUDE_ASM("sound/soundsys", func_002A1028);

INCLUDE_ASM("sound/soundsys", func_002A10C0);

INCLUDE_ASM("sound/soundsys", func_002A1138);

INCLUDE_ASM("sound/soundsys", func_002A1280);

INCLUDE_ASM("sound/soundsys", func_002A1388);

INCLUDE_ASM("sound/soundsys", func_002A1400);

INCLUDE_ASM("sound/soundsys", func_002A1560);

INCLUDE_ASM("sound/soundsys", func_002A16B0);

INCLUDE_ASM("sound/soundsys", func_002A1778);

INCLUDE_ASM("sound/soundsys", func_002A1820);

INCLUDE_ASM("sound/soundsys", func_002A19D8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A1B58);
#ifdef SKIP_ASM
extern "C" void func_002A1400(void* self, int a);

extern "C" int func_002A1B58(void* self, int msg, int a)
{
    if (msg == 2) {
        func_002A1400(self, a);
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A1B88);

//100%
INCLUDE_ASM("sound/soundsys", func_002A1BD0__FPv);
#ifdef SKIP_ASM
int func_002A1BD0(void* self)
{
    return 0x1;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A1BD8);

INCLUDE_ASM("sound/soundsys", func_002A1D48);

INCLUDE_ASM("sound/soundsys", func_002A1DA0);

INCLUDE_ASM("sound/soundsys", func_002A1E20);

INCLUDE_ASM("sound/soundsys", func_002A1E68);

INCLUDE_ASM("sound/soundsys", func_002A2018);

INCLUDE_ASM("sound/soundsys", func_002A20D0);

INCLUDE_ASM("sound/soundsys", func_002A2188);

INCLUDE_ASM("sound/soundsys", func_002A2260);

INCLUDE_ASM("sound/soundsys", func_002A2318);

INCLUDE_ASM("sound/soundsys", func_002A23D0);

INCLUDE_ASM("sound/soundsys", func_002A24B0);

INCLUDE_ASM("sound/soundsys", func_002A2568);

INCLUDE_ASM("sound/soundsys", func_002A2638);

INCLUDE_ASM("sound/soundsys", func_002A26F0);

INCLUDE_ASM("sound/soundsys", func_002A27D8);

INCLUDE_ASM("sound/soundsys", func_002A2860);

INCLUDE_ASM("sound/soundsys", func_002A2938);

INCLUDE_ASM("sound/soundsys", func_002A2A10);

INCLUDE_ASM("sound/soundsys", func_002A2AD0);

INCLUDE_ASM("sound/soundsys", func_002A2B88);

INCLUDE_ASM("sound/soundsys", func_002A2C30);

INCLUDE_ASM("sound/soundsys", func_002A2CF0);

INCLUDE_ASM("sound/soundsys", func_002A2DA0);

INCLUDE_ASM("sound/soundsys", func_002A2E50);

INCLUDE_ASM("sound/soundsys", func_002A3170);

INCLUDE_ASM("sound/soundsys", func_002A31C0);

INCLUDE_ASM("sound/soundsys", func_002A32B0);

INCLUDE_ASM("sound/soundsys", func_002A3358);

INCLUDE_ASM("sound/soundsys", func_002A3400);

INCLUDE_ASM("sound/soundsys", func_002A34D0);

INCLUDE_ASM("sound/soundsys", func_002A3708);

INCLUDE_ASM("sound/soundsys", func_002A3860);

INCLUDE_ASM("sound/soundsys", func_002A39E0);

INCLUDE_ASM("sound/soundsys", func_002A3B18);

INCLUDE_ASM("sound/soundsys", func_002A3C00);

INCLUDE_ASM("sound/soundsys", func_002A3CE8);

INCLUDE_ASM("sound/soundsys", func_002A3DE0);

INCLUDE_ASM("sound/soundsys", func_002A3EB8);

INCLUDE_ASM("sound/soundsys", func_002A3F90);

//100%
INCLUDE_ASM("sound/soundsys", func_002A3FD8);
#ifdef SKIP_ASM
extern "C" int func_002A3FD8(void* self, unsigned int a1)
{
    return a1 - 0x11 < 5;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A3FE8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4030);
#ifdef SKIP_ASM
extern "C" int func_002A4030(void* self, unsigned int a1)
{
    return a1 - 0xe < 3;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4040);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C10[];

extern "C" int func_002A4040(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    return D_00535C10[0] == 4;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A4078);

//100%
INCLUDE_ASM("sound/soundsys", func_002A40E0);
#ifdef SKIP_ASM
extern "C" int func_002A40E0(void* self, unsigned int a1)
{
    return a1 < 5;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A40E8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4158);
#ifdef SKIP_ASM
extern "C" int func_002A4158(void* self, unsigned int a1)
{
    return a1 - 5 < 3;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A4168);

//100%
INCLUDE_ASM("sound/soundsys", func_002A41C8);
#ifdef SKIP_ASM
extern "C" int func_002A41C8(void* self, unsigned int a1)
{
    return a1 - 8 < 3;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A41D8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4238);
#ifdef SKIP_ASM
extern "C" int func_002A4238(void* self, unsigned int a1)
{
    return a1 - 0xb < 3;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A4248);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4290);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern signed char D_00535C12[];

extern "C" int func_002A4290(void* self)
{
    cBE_getInterface_Fv(cBE_getBE(), 0);
    return D_00535C12[0] == 4;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A42C8);

INCLUDE_ASM("sound/soundsys", func_002A4318);

INCLUDE_ASM("sound/soundsys", func_002A4368);

INCLUDE_ASM("sound/soundsys", func_002A43B8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4550);
#ifdef SKIP_ASM
extern "C" void func_002A4550(void* self)
{
    *(int*)((char*)self + 0x5740) = 0;
    *(int*)((char*)self + 0x5744) = 0;
    *(int*)((char*)self + 0x574C) = 0;
    *(int*)((char*)self + 0x5750) = 0;
    *(int*)((char*)self + 0x5754) = 0;
    *(int*)((char*)self + 0x5758) = 0;
    *(int*)((char*)self + 0x5760) = 0;
    *(int*)((char*)self + 0x5768) = 0;
    *(int*)((char*)self + 0x576C) = 0;
    *(int*)((char*)self + 0x5770) = 0;
    *(int*)((char*)self + 0x5774) = 0;
    *(int*)((char*)self + 0x577C) = 0;
    *(int*)((char*)self + 0x5784) = 0;
    *(int*)((char*)self + 0x5788) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A4590);
#ifdef SKIP_ASM
extern "C" void func_002A4590(void* self, int flag)
{
    *(int*)((char*)self + 0x57F8) = 0;
    *(int*)((char*)self + 0x57FC) = 0;
    *(int*)((char*)self + 0x5804) = 0;
    *(int*)((char*)self + 0x5808) = 0;
    *(int*)((char*)self + 0x5800) = -1;
    *(int*)((char*)self + 0x5810) = -1;
    if (flag) {
        *(int*)((char*)self + 0x580C) = -1;
    }
    *(int*)((char*)self + 0x5814) = 0x17;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A45C0);

INCLUDE_ASM("sound/soundsys", func_002A4660);

INCLUDE_ASM("sound/soundsys", func_002A4718);

INCLUDE_ASM("sound/soundsys", func_002A4770);

INCLUDE_ASM("sound/soundsys", func_002A49E8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4A38);
#ifdef SKIP_ASM
extern "C" int func_002A10C0(void* self, int id);

extern "C" int func_002A4A38(void* self)
{
    if (*(int*)((char*)self + 0x5818) != 0) {
        return func_002A10C0(self, 10) != 0;
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A4A78);

INCLUDE_ASM("sound/soundsys", func_002A4B68);

//100%
INCLUDE_ASM("sound/soundsys", func_002A4BA8);
#ifdef SKIP_ASM
extern "C" void* func_002A4BA8(void* self)
{
    int i;
    *(int*)self = 0;
    for (i = 4; i >= 0; i--) {
        *(int*)((char*)self + (i << 2) + 4) = 0;
    }
    return self;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A4BE0);

INCLUDE_ASM("sound/soundsys", func_002A4C28);

INCLUDE_ASM("sound/soundsys", func_002A4CA0);

INCLUDE_ASM("sound/soundsys", func_002A4CF8);

INCLUDE_ASM("sound/soundsys", func_002A4D68);

INCLUDE_ASM("sound/soundsys", func_002A4E38);

INCLUDE_ASM("sound/soundsys", func_002A4E88);

//100%
INCLUDE_ASM("sound/soundsys", func_002A5CD8);
#ifdef SKIP_ASM
struct s2A5CD8Entry {
    int active;
    int value;
    int pad;
};

struct s2A5CD8 {
    char pad[0x20];
    s2A5CD8Entry entries[1];
};

extern "C" void func_002A5CD8(s2A5CD8* self, int i, int v)
{
    if (i >= 0) {
        if (self->entries[i].active) {
            self->entries[i].value = v;
        }
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A5D08);

INCLUDE_ASM("sound/soundsys", func_002A62F0);

INCLUDE_ASM("sound/soundsys", func_002A6430);

INCLUDE_ASM("sound/soundsys", func_002A64A0);

INCLUDE_ASM("sound/soundsys", func_002A6648);

//100%
INCLUDE_ASM("sound/soundsys", func_002A67F0);
#ifdef SKIP_ASM
extern "C" void func_002A67F0(void* self)
{
    if (*(int*)((char*)self + 0x180) == 0) {
        *(int*)((char*)self + 0x180) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A6808);
#ifdef SKIP_ASM
extern "C" void func_002A6848(void* self);

extern "C" void func_002A6808(void* self)
{
    if (*(int*)((char*)self + 0x180) == 2) {
        func_002A6848(self);
    }
    *(int*)((char*)self + 0x180) = 0;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A6848);

INCLUDE_ASM("sound/soundsys", func_002A69A0);

INCLUDE_ASM("sound/soundsys", func_002A6B50);

INCLUDE_ASM("sound/soundsys", func_002A6D18);

INCLUDE_ASM("sound/soundsys", func_002A6D78);

INCLUDE_ASM("sound/soundsys", func_002A6F38);

INCLUDE_ASM("sound/soundsys", func_002A7040);

//100%
INCLUDE_ASM("sound/soundsys", func_002A72C0);
#ifdef SKIP_ASM
extern "C" void func_002A72C0(void* self, int a1, int* a2, int* a3)
{
    *a2 = a1 & 0xff;
    *a3 = a1 >> 8;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A72D8);

INCLUDE_ASM("sound/soundsys", func_002A7340);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A73A8);
#ifdef SKIP_ASM
extern "C" void func_002A64A0(void* voice, int a1);

struct sSndVoice190 {
    char data[0x190];
};

extern "C" void func_002A73A8(void* self, int a1, int i)
{
    sSndVoice190* v = *(sSndVoice190**)((char*)self + 0x18);
    if (v != 0) {
        func_002A64A0(&v[i], a1);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A73D8);
#ifdef SKIP_ASM
extern "C" void func_002A6648(void* voice, int a1);

extern "C" void func_002A73D8(void* self, int a1, int i)
{
    sSndVoice190* v = *(sSndVoice190**)((char*)self + 0x18);
    if (v != 0) {
        func_002A6648(&v[i], a1);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7408);
#ifdef SKIP_ASM
extern "C" void func_002A67F0(void* voice);

extern "C" void func_002A7408(void* self, int i)
{
    sSndVoice190* v = *(sSndVoice190**)((char*)self + 0x18);
    if (v != 0) {
        func_002A67F0(&v[i]);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A7438);
#ifdef SKIP_ASM
extern "C" void func_002A6808(void* voice);

extern "C" void func_002A7438(void* self, int i)
{
    sSndVoice190* v = *(sSndVoice190**)((char*)self + 0x18);
    if (v != 0) {
        func_002A6808(&v[i]);
    }
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A7468);

INCLUDE_ASM("sound/soundsys", func_002A7678);

INCLUDE_ASM("sound/soundsys", func_002A7718);

//100%
INCLUDE_ASM("sound/soundsys", func_002A77C8);
#ifdef SKIP_ASM
struct s2A77C8Item {
    char pad[0x18C];
    int value;
};

extern "C" int func_002A77C8(void* self, int i)
{
    if (i < *(int*)((char*)self + 0x14)) {
        return (*(s2A77C8Item**)((char*)self + 0x18))[i].value;
    }
    return 0;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A77F8);

INCLUDE_ASM("sound/soundsys", func_002A7890);

//100%
INCLUDE_ASM("sound/soundsys", func_002A78E0__FPv);
#ifdef SKIP_ASM
signed char func_002A78E0(void* self)
{
    return *(*(signed char**)((char*)self + 0x8))++;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A78F8);

INCLUDE_ASM("sound/soundsys", func_002A79C8);

INCLUDE_ASM("sound/soundsys", func_002A7A20);

INCLUDE_ASM("sound/soundsys", func_002A7A90);

INCLUDE_ASM("sound/soundsys", func_002A7B08);

INCLUDE_ASM("sound/soundsys", func_002A7BF8);

INCLUDE_ASM("sound/soundsys", func_002A7C68);

INCLUDE_ASM("sound/soundsys", func_002A7CF0);

INCLUDE_ASM("sound/soundsys", func_002A7DA0);

INCLUDE_ASM("sound/soundsys", func_002A7E60);

INCLUDE_ASM("sound/soundsys", func_002A7EA8);

INCLUDE_ASM("sound/soundsys", func_002A7EF0);

INCLUDE_ASM("sound/soundsys", func_002A7F90);

INCLUDE_ASM("sound/soundsys", func_002A7FF8);

INCLUDE_ASM("sound/soundsys", func_002A8070);

INCLUDE_ASM("sound/soundsys", func_002A80D8);

INCLUDE_ASM("sound/soundsys", func_002A8140);

INCLUDE_ASM("sound/soundsys", func_002A82D8);

INCLUDE_ASM("sound/soundsys", func_002A8450);

INCLUDE_ASM("sound/soundsys", func_002A86B8);

//100%
INCLUDE_ASM("sound/soundsys", func_002A8C20);
#ifdef SKIP_ASM
extern char D_004835F8[];

extern "C" void* func_002A8C20(void* self)
{
    *(void**)self = D_004835F8;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x30C) = -1;
    *(int*)((char*)self + 0x310) = -1;
    *(int*)((char*)self + 0x31C) = 0;
    *(int*)((char*)self + 0x318) = -1;
    return self;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A8C50);

INCLUDE_ASM("sound/soundsys", func_002A8CB0);

INCLUDE_ASM("sound/soundsys", func_002A8D90);

INCLUDE_ASM("sound/soundsys", func_002A8F50);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("sound/soundsys", func_002A8FA0);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* func_002A8C20(void* self);
extern const char D_00483168[];

extern "C" void* func_002A8FA0()
{
    return func_002A8C20(cMemMan_alloc(0x320, D_00483168, 0, 0));
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A8FD8);

INCLUDE_ASM("sound/soundsys", func_002A9128);

//100%
INCLUDE_ASM("sound/soundsys", func_002A9250);
#ifdef SKIP_ASM
extern "C" void func_003B5B20(int a, int b);

struct sSndPoolEntry2A9250 {
    int unk0;
    int unk4;
    int unk8;
    char pad[0x14 - 0xC];
    int next;
};

struct sSndPool2A9250 {
    char pad[0xC];
    sSndPoolEntry2A9250 entries[32];
    int unk30C;
    int freeHead;
    char pad2[0x31C - 0x314];
    int unk31C;
};

extern "C" void func_002A9250(sSndPool2A9250* self, int i)
{
    self->unk31C = 1;
    sSndPoolEntry2A9250* e = &self->entries[i];
    func_003B5B20(e->unk4, e->unk8);
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A9288);
#ifdef SKIP_ASM
extern "C" void func_003B5B20(int a, int b);

extern "C" void func_002A9288(void* self)
{
    func_003B5B20(-1, *(int*)((char*)self + 0x30C));
    *(int*)((char*)self + 0x31C) = 0;
}
#endif

//100%
INCLUDE_ASM("sound/soundsys", func_002A92B8);
#ifdef SKIP_ASM
struct s2A92B8Entry {
    char pad[0x14];
    int next;
};

struct s2A92B8 {
    char pad[0xC];
    s2A92B8Entry entries[32];
    int unk30C;
    int freeHead;
};

extern "C" s2A92B8Entry* func_002A92B8(s2A92B8* self)
{
    int i = self->freeHead;
    if (i < 0) {
        return 0;
    }
    s2A92B8Entry* e = &self->entries[i];
    self->freeHead = e->next;
    e->next = -1;
    return e;
}
#endif

INCLUDE_ASM("sound/soundsys", func_002A92F8);

