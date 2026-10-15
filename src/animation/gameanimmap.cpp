#include "common.h"

//100%
void cGameAnimMap_testResolve()
{
}

extern "C" void* func_003150B8(void* p, ...)
{
    return p;
}

INCLUDE_ASM("animation/gameanimmap", func_003150F8);

extern "C" void* func_003150F8(int, int);

extern "C" float func_003158B8(void* self)
{
    return *(float*)((char*)self + 0x1C);
}

INCLUDE_ASM("animation/gameanimmap", func_003158C0);

extern "C" void* func_003158C8(void* self)
{
    return (char*)self + 0x30;
}

extern "C" int func_003158D0(void* self)
{
    return *(int*)((char*)self + 0x18);
}

extern "C" void func_003158D8(void* self, int value)
{
    *(int*)((char*)self + 0x18) = value;
}

extern "C" void func_003158E0(void* self, float value)
{
    *(float*)((char*)self + 0x1C) = value;
}

INCLUDE_ASM("animation/gameanimmap", func_003158E8);

INCLUDE_ASM("animation/gameanimmap", func_00315988);

INCLUDE_ASM("animation/gameanimmap", func_003159A0);

INCLUDE_ASM("animation/gameanimmap", func_003159A8);

INCLUDE_ASM("animation/gameanimmap", func_003159B8);

INCLUDE_ASM("animation/gameanimmap", func_003159E8);

//99.38%
void* func_00315A00(void* self)
{
    return func_003150F8(1, 0xffff);
}

INCLUDE_ASM("animation/gameanimmap", func_00315A20);

extern "C" void* func_00315A20(int, int);

//99.38%
void* func_00316878(void* self)
{
    return func_00315A20(1, 0xffff);
}

