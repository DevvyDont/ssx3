#include "common.h"

INCLUDE_ASM("be/bedbreward", cBERewardDB_init);

INCLUDE_ASM("be/bedbreward", func_0015AE00);

INCLUDE_ASM("be/bedbreward", func_0015AF40);

INCLUDE_ASM("be/bedbreward", func_0015B028);

//100%
INCLUDE_ASM("be/bedbreward", func_0015BB00__FPv);
#ifdef SKIP_ASM
void func_0015BB00(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015BB08__FPv);
#ifdef SKIP_ASM
void func_0015BB08(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015BB10__FPv);
#ifdef SKIP_ASM
void func_0015BB10(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015BB18__FPv);
#ifdef SKIP_ASM
void func_0015BB18(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015BCC0__FPv);
#ifdef SKIP_ASM
void func_0015BCC0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015BCC8__FPv);
#ifdef SKIP_ASM
void func_0015BCC8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C228);
#ifdef SKIP_ASM
// func_003E6448 looks like memset(dst, value, size).
extern "C" void* func_003E6448(void* dst, int value, int size);
extern char D_005308B8[];

extern "C" void func_0015C228()
{
    func_003E6448(D_005308B8, 0, 0x20);
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C6C8__FPv);
#ifdef SKIP_ASM
void func_0015C6C8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C6D0__FPv);
#ifdef SKIP_ASM
void func_0015C6D0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C6D8__FPv);
#ifdef SKIP_ASM
void func_0015C6D8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C6E0__FPv);
#ifdef SKIP_ASM
void func_0015C6E0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C7D0__FPv);
#ifdef SKIP_ASM
void func_0015C7D0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C7D8__FPv);
#ifdef SKIP_ASM
void func_0015C7D8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C830);
#ifdef SKIP_ASM
extern int* D_004A1258;
void operator_delete(int* ptr);

extern "C" void func_0015C830(void)
{
    operator_delete(D_004A1258);
    D_004A1258 = 0;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C870);
#ifdef SKIP_ASM
extern int* D_004A122C;
void operator_delete(int* ptr);

extern "C" void func_0015C870(void)
{
    operator_delete(D_004A122C);
    D_004A122C = 0;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C8B0);
#ifdef SKIP_ASM
extern int* D_004A1254;
void operator_delete(int* ptr);

extern "C" void func_0015C8B0(void)
{
    operator_delete(D_004A1254);
    D_004A1254 = 0;
}
#endif

extern "C" void* func_0015B028(int, int);

//99.38%
INCLUDE_ASM("be/bedbreward", func_0015C8F0__FPv);
#ifdef SKIP_ASM
void* func_0015C8F0(void* self)
{
    return func_0015B028(1, 0xffff);
}
#endif

extern "C" void* func_0015B028(int, int);

//99.38%
INCLUDE_ASM("be/bedbreward", func_0015C910__FPv);
#ifdef SKIP_ASM
void* func_0015C910(void* self)
{
    return func_0015B028(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C930__FPv);
#ifdef SKIP_ASM
void* func_0015C930(void* self)
{
    int t0 = 0;
    *(int*)self = t0;
    *(int*)((char*)self + 0x4) = t0;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C940);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void func_0015CB08(void* self);

extern "C" void func_0015C940(int* self, int flags)
{
    func_0015CB08(self);
    if (flags & 1)
    {
        operator_delete(self);
    }
}
#endif

