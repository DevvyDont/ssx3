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

//100%
extern "C" long func_003158C0(void* self)
{
    return *(long*)((char*)self + 0x20);
}

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

//100%
// Copies the 0x20-byte pair of vectors from src into self at 0x30.
struct sVec4_315988
{
    float x, y, z, w;
} __attribute__((aligned(16)));
struct sPair_315988 { sVec4_315988 a, b; };

extern "C" void func_00315988(char* self, sPair_315988* src)
{
    *(sPair_315988*)(self + 0x30) = *src;
}

//100%
extern "C" void func_003159A0(void* self, long v)
{
    *(long*)((char*)self + 0x20) = v;
}

//100%
extern "C" void func_003159A8(void* self)
{
    *(long*)((char*)self + 0x20) = -1;
}

//100%
// Moves bit n from the mask at 0x0 to the mask at 0x8.
extern "C" void func_003159B8(long* self, int n)
{
    long bit = 1L << n;
    self[1] |= bit & self[0];
    self[0] &= ~bit;
}

//100%
extern "C" int func_003159E8(long* self, int bit)
{
    long mask = 1L << bit;
    return (*self & mask) != 0;
}

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

