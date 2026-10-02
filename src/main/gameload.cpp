#include "common.h"

INCLUDE_ASM("main/gameload", cGame_load);

INCLUDE_ASM("main/gameload", cGame_loadTrack);

INCLUDE_ASM("main/gameload", func_0022FA98);

INCLUDE_ASM("main/gameload", func_00230050);

INCLUDE_ASM("main/gameload", func_002300F0);

INCLUDE_ASM("main/gameload", func_00230180);

INCLUDE_ASM("main/gameload", cGame_restart);

//100%
INCLUDE_ASM("main/gameload", func_00230338);
#ifdef SKIP_ASM
extern "C" void func_00230338(void* self)
{
    void* a = *(void**)((char*)self + 0xC);
    if (a != 0) {
        void* b = *(void**)((char*)a + 0xA4);
        if (b != 0) {
            *(int*)((char*)b + 0xD0) = -1;
        }
    }
}
#endif

INCLUDE_ASM("main/gameload", func_00230360);

INCLUDE_ASM("main/gameload", func_00230430);

INCLUDE_ASM("main/gameload", cGame_exit);

INCLUDE_ASM("main/gameload", func_002305C8);

INCLUDE_ASM("main/gameload", func_00230640);

//100%
INCLUDE_ASM("main/gameload", func_00230698__FPvi);
#ifdef SKIP_ASM
int func_00230698(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x5C);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_002306A8__FPvi);
#ifdef SKIP_ASM
int func_002306A8(void* self, int i)
{
    return *(int*)((char*)self + (i << 2) + 0x68);
}
#endif

INCLUDE_ASM("main/gameload", func_002306B8);

INCLUDE_ASM("main/gameload", func_00230E98);

INCLUDE_ASM("main/gameload", func_00230F40);

//100%
INCLUDE_ASM("main/gameload", func_00231250);
#ifdef SKIP_ASM
extern "C" void func_00230F40(void* self);

extern "C" void func_00231250(void* self, int a, int b, int refresh)
{
    *(int*)((char*)self + 0x210) = a;
    *(int*)((char*)self + 0x21C) = b;
    if (refresh != 0) {
        func_00230F40(self);
    }
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231278__FPvi);
#ifdef SKIP_ASM
void func_00231278(void* self, int val)
{
    *(int*)((char*)self + 0x208) = val;
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_002312D8);
#ifdef SKIP_ASM
struct sVEntry_002312D8 {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" int func_002312D8(void* self)
{
    if (*(int*)((char*)self + 0x208) != 0) {
        return 0;
    }
    void* p = *(void**)((char*)self + 0x204);
    if (p == 0) {
        return 1;
    }
    sVEntry_002312D8* vt = *(sVEntry_002312D8**)((char*)p + 0xC);
    return vt[8].fn((char*)p + vt[8].delta);
}
#endif

//100%
INCLUDE_ASM("main/gameload", func_00231320);
#ifdef SKIP_ASM
extern "C" void func_00233AA0(void* p);

extern "C" void func_00231320(void* self)
{
    void* p = *(void**)((char*)self + 0x200);
    if (p != 0) {
        func_00233AA0(p);
    }
}
#endif

INCLUDE_ASM("main/gameload", func_00231348);

INCLUDE_ASM("main/gameload", func_002314D0);

INCLUDE_ASM("main/gameload", func_00231840);

INCLUDE_ASM("main/gameload", func_00231AB8);

INCLUDE_ASM("main/gameload", func_00231C70);

extern "C" void* func_00231D18(void* self);

//100%
INCLUDE_ASM("main/gameload", func_00231CB0__FPv);
#ifdef SKIP_ASM
void* func_00231CB0(void* self)
{
    return func_00231D18(self);
}
#endif

INCLUDE_ASM("main/gameload", func_00231CD0);

INCLUDE_ASM("main/gameload", func_00231CF0);

INCLUDE_ASM("main/gameload", func_00231D18);

INCLUDE_ASM("main/gameload", func_00231D60);

INCLUDE_ASM("main/gameload", func_00231F80);

INCLUDE_ASM("main/gameload", func_00231FC0);

extern void* D_0047D938[];

//100%
INCLUDE_ASM("main/gameload", func_00232328__FPv);
#ifdef SKIP_ASM
void* func_00232328(void* self)
{
    int t0 = 0;
    *(int*)((char*)self + 0x4) = t0;
    *(int*)self = (int)(void*)D_0047D938;
    *(int*)((char*)self + 0x8) = t0;
    return self;
}
#endif

