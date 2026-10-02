#include "common.h"

INCLUDE_ASM("fe/festatecharsetup", cFEStateCharSetup_onCreateScreen);

//100%
INCLUDE_ASM("fe/festatecharsetup", func_00182C08);
#ifdef SKIP_ASM
extern "C" void func_001831D0(void* self, int a1);
void* func_0039E4A0(void* self);

extern "C" void func_00182C08(void* self)
{
    func_001831D0(self, 0);
    func_0039E4A0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharsetup", func_00182C38);
#ifdef SKIP_ASM
extern "C" void* func_0039E510(void* self);
extern "C" void func_00182EC0(void* self);

extern "C" void func_00182C38(void* self)
{
    func_0039E510(self);
    func_00182EC0(self);
}
#endif

//100%
INCLUDE_ASM("fe/festatecharsetup", func_00182C68);
#ifdef SKIP_ASM
extern "C" int func_00182C68(void* self, int a1, int a2)
{
    return a2 != 9 ? 0x101 : 0x100;
}
#endif

INCLUDE_ASM("fe/festatecharsetup", func_00182C80);

INCLUDE_ASM("fe/festatecharsetup", func_00182DB8);

INCLUDE_ASM("fe/festatecharsetup", func_00182EC0);

INCLUDE_ASM("fe/festatecharsetup", func_001831D0);

//100%
INCLUDE_ASM("fe/festatecharsetup", func_00183238);
#ifdef SKIP_ASM
extern "C" void* func_001828C0(void* self, void* engine, signed char idx);
extern "C" unsigned char func_001A1CD0(void* self, int a1);
extern void* D_0046CD90[];

extern "C" void* func_00183238(void* self, void* engine, signed char idx)
{
    func_001828C0(self, engine, idx);
    *(int*)((char*)self + 0xC) = 0xD;
    *(void***)((char*)self + 0x8) = D_0046CD90;
    *(signed char*)((char*)self + 0x44) = idx;
    *(unsigned char*)((char*)self + 0x15) = func_001A1CD0(**(void***)((char*)self + 0x10), idx);
    *(int*)((char*)self + 0x4C) = 0;
    return self;
}
#endif

