#include "common.h"

INCLUDE_ASM("main/loadscreens_prestart", cPreStartScreen_update);

INCLUDE_ASM("main/loadscreens_prestart", func_00232488);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232510);
#ifdef SKIP_ASM
extern "C" int func_00232510(void* self)
{
    if (*(float*)((char*)self + 0x4) < 0.75f) {
        return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232538);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046D970[];

extern "C" void func_00232538(void* self, int flags)
{
    *(void***)self = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("main/loadscreens_prestart", func_00232568);

INCLUDE_ASM("main/loadscreens_prestart", func_00232588);

INCLUDE_ASM("main/loadscreens_prestart", func_00232650);

INCLUDE_ASM("main/loadscreens_prestart", func_002326D8);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232710__FPv);
#ifdef SKIP_ASM
void func_00232710(void* self)
{
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232718__FPv);
#ifdef SKIP_ASM
void func_00232718(void* self)
{
}
#endif

extern void* D_0047D8A8[];

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232720__FPv);
#ifdef SKIP_ASM
void* func_00232720(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    *(int*)self = (int)(void*)D_0047D8A8;
    return self;
}
#endif

extern void* D_0047D860[];

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232738__FPv);
#ifdef SKIP_ASM
void* func_00232738(void* self)
{
    *(int*)((char*)self + 0x4) = 0;
    *(int*)self = (int)(void*)D_0047D860;
    return self;
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232750);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046D970[];

extern "C" void func_00232750(void* self, int flags)
{
    *(void***)self = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232780);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern void* D_0046D970[];

extern "C" void func_00232780(void* self, int flags)
{
    *(void***)self = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("main/loadscreens_prestart", func_002327B0);

INCLUDE_ASM("main/loadscreens_prestart", func_002327E8);

INCLUDE_ASM("main/loadscreens_prestart", func_00232820);

INCLUDE_ASM("main/loadscreens_prestart", func_002328A8);

INCLUDE_ASM("main/loadscreens_prestart", func_00232930);

INCLUDE_ASM("main/loadscreens_prestart", cPreFELoadScreen_update);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232AE0);
#ifdef SKIP_ASM
extern "C" void func_00231CD0(void* self);
extern void* D_0047D7D0[];

extern "C" void* func_00232AE0(void* self, int arg)
{
    func_00231CD0(self);
    *(void***)self = D_0047D7D0;
    *(int*)((char*)self + 0x18) = arg;
    *(int*)((char*)self + 0xC) = 0;
    return self;
}
#endif

INCLUDE_ASM("main/loadscreens_prestart", func_00232B28);

INCLUDE_ASM("main/loadscreens_prestart", func_00232C98);

extern "C" void* func_00231CF0(void* self);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232CF8__FPv);
#ifdef SKIP_ASM
void* func_00232CF8(void* self)
{
    return func_00231CF0(self);
}
#endif

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00232D18);
#ifdef SKIP_ASM
void func_00231CB0(void*);
extern "C" void func_00398038(void*);

extern "C" void func_00232D18(void* self)
{
    func_00231CB0(self);
    func_00398038(*(void**)((char*)self + 0xC));
    (*(int*)((char*)self + 0x14))++;
}
#endif

INCLUDE_ASM("main/loadscreens_prestart", func_00232D50);

INCLUDE_ASM("main/loadscreens_prestart", func_00232DA8);

INCLUDE_ASM("main/loadscreens_prestart", func_00232E20);

INCLUDE_ASM("main/loadscreens_prestart", cGameLoadScreen_loadTexture);

INCLUDE_ASM("main/loadscreens_prestart", func_00233260);

extern "C" void* func_00231C70(void* self);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_002332C8__FPv);
#ifdef SKIP_ASM
int func_002332C8(void* self)
{
    return (func_00231C70(self) != 0);
}
#endif

INCLUDE_ASM("main/loadscreens_prestart", func_002332E8);

INCLUDE_ASM("main/loadscreens_prestart", func_00233390);

//100%
INCLUDE_ASM("main/loadscreens_prestart", func_00233408);
#ifdef SKIP_ASM
extern "C" void func_00398038(void*);
void func_00231CB0(void*);

extern "C" void func_00233408(void* self)
{
    func_00398038(*(void**)((char*)self + 0xC));
    func_00231CB0(self);
}
#endif

