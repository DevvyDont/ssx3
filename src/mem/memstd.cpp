#include "common.h"

INCLUDE_ASM("mem/memstd", MEMCLASS_create);

INCLUDE_ASM("mem/memstd", MEMCLASS_link);

INCLUDE_ASM("mem/memstd", func_00251F68);

extern const char D_004800A8[];
void func_00251F68();
void MEM_printclassf(void* thing, const char* fmt, void (*cb)());

//100%
INCLUDE_ASM("mem/memstd", MEM_printclass__FPv);
#ifdef SKIP_ASM
void MEM_printclass(void* thing)
{
    MEM_printclassf(thing, D_004800A8, func_00251F68);
}
#endif

extern void* D_004A2E78;

//99.25%
INCLUDE_ASM("mem/memstd", MEM_print__Fv);
#ifdef SKIP_ASM
void MEM_print()
{
    MEM_printclass(D_004A2E78);
}
#endif

INCLUDE_ASM("mem/memstd", MEM_printclassf);

INCLUDE_ASM("mem/memstd", func_00252248);

INCLUDE_ASM("mem/memstd", func_002522B0);

//100%
INCLUDE_ASM("mem/memstd", func_002523A8);
#ifdef SKIP_ASM
void cMemMan_free(void*);

extern "C" void* func_002523A8(void* self)
{
    if (self != 0) {
        cMemMan_free(self);
    }
    return (void*)1;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00252658__FPv);
#ifdef SKIP_ASM
int func_00252658(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00252660);
#ifdef SKIP_ASM
struct sMemUnalignedInt {
    int value;
} __attribute__((packed));

// PORT: pointer arithmetic done in int; a plain `(char*)p - 0xE` makes gcc 2.95
// emit the offset as unsigned 0xFFFFFFF2 (addiu -0x8000 / 0x7ff2 split).
extern "C" int func_00252660(void* p)
{
    int r = 0;
    if (*(unsigned short*)((int)p - 0xE) & 0x800) {
        r = ((sMemUnalignedInt*)((char*)p + *(int*)((int)p - 0xC) + 4))->value;
    }
    return r;
}
#endif

INCLUDE_ASM("mem/memstd", func_002526B8);

INCLUDE_ASM("mem/memstd", func_00252980);

extern "C" void* func_00252980(void*);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", func_00252F60__FPv);
#ifdef SKIP_ASM
void* func_00252980_6(void*, int, int, int, int, int) __asm__("func_00252980");

// PORT: the project's symbol name says (void*), but callers pass and this
// forwards 5 args (see func_00253AD0); the real body is bound by asm label.
void* func_00252F60_impl(void* a0, int a1, int a2, int a3, int a4) __asm__("func_00252F60__FPv");

void* func_00252F60_impl(void* a0, int a1, int a2, int a3, int a4)
{
    return func_00252980_6(a0, a1, a2, a3, a4, 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", func_00252FA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* func_00252980_6(void*, int, int, int, int, int) __asm__("func_00252980");

extern "C" void* func_00252FA0(void* a0, int a1, int a2)
{
    return func_00252980_6(a0, a1, 0x80, 0, a2, 1);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00253390);
#ifdef SKIP_ASM
extern "C" void func_00253390(void* self)
{
    *(float*)((char*)self + 0x20) = 1.0f;
    *(int*)((char*)self + 0x1c) = 31;
    *(int*)((char*)self + 0x0) = 0;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)((char*)self + 0xc) = 0;
    *(int*)((char*)self + 0x8) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x14) = 0;
    *(int*)((char*)self + 0x18) = 0;
}
#endif

INCLUDE_ASM("mem/memstd", func_002533C8);

INCLUDE_ASM("mem/memstd", func_00253418);

INCLUDE_ASM("mem/memstd", func_002534A8);

INCLUDE_ASM("mem/memstd", func_002535F8);

INCLUDE_ASM("mem/memstd", func_002536C8);

INCLUDE_ASM("mem/memstd", func_00253860);

INCLUDE_ASM("mem/memstd", func_00253890);

INCLUDE_ASM("mem/memstd", func_00253938);

INCLUDE_ASM("mem/memstd", func_002539E0);

INCLUDE_ASM("mem/memstd", func_00253A40);

//100%
INCLUDE_ASM("mem/memstd", func_00253AA0);
#ifdef SKIP_ASM
extern "C" void* func_00253AD0(int size);
void* func_00253AF8(void* self);

struct sMemAllocFuncs {
    void* (*alloc)(int);    // 0x0
    void* (*free)(void*);   // 0x4
    int field_0x8;          // 0x8
};
extern sMemAllocFuncs D_00509430;

extern "C" void func_00253AA0()
{
    D_00509430.alloc = func_00253AD0;
    D_00509430.free = func_00253AF8;
    D_00509430.field_0x8 = 0x400;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", func_00253AD0);
#ifdef SKIP_ASM
void* func_00252F60_5(void* a0, int a1, int a2, int a3, int a4) __asm__("func_00252F60__FPv");

// PORT: the unit declares this as (int size); the body uses 5 args (prototype mismatch).
void* func_00253AD0_impl(void* a0, int a1, int align, int a3, int a4) __asm__("func_00253AD0");

void* func_00253AD0_impl(void* a0, int a1, int align, int a3, int a4)
{
    if (align < 64) {
        align = 64;
    }
    return func_00252F60_5(a0, a1, align, a3, a4);
}
#endif

extern "C" void* func_002523A8(void* self);

//99.29%
INCLUDE_ASM("mem/memstd", func_00253AF8__FPv);
#ifdef SKIP_ASM
void* func_00253AF8(void* self)
{
    return func_002523A8(self);
}
#endif

INCLUDE_ASM("mem/memstd", func_00253B58);

extern "C" void* func_00253B58(int, int);

//99.38%
INCLUDE_ASM("mem/memstd", func_00254330__FPv);
#ifdef SKIP_ASM
void* func_00254330(void* self)
{
    return func_00253B58(1, 0xffff);
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00254350);
#ifdef SKIP_ASM
extern "C" int func_00254350(int x)
{
    float fv = (float)x;
    int bits = *(int*)&fv;
    return (bits >> 23) - 0x7f;
}
#endif

INCLUDE_ASM("mem/memstd", func_00254368);

//100%
INCLUDE_ASM("mem/memstd", func_002543F0);
#ifdef SKIP_ASM
extern "C" void func_002543F0(void* self, float a, float b, float c)
{
    *(float*)((char*)self + 0x40) = a;
    *(float*)((char*)self + 0x44) = b;
    *(float*)((char*)self + 0x48) = c;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00254400__FPvff);
#ifdef SKIP_ASM
void func_00254400(void* self, float f0, float f1)
{
    *(float*)((char*)self + 0x4c) = f0;
    *(float*)((char*)self + 0x50) = f1;
}
#endif

INCLUDE_ASM("mem/memstd", func_00254410);

INCLUDE_ASM("mem/memstd", func_002547B8);

INCLUDE_ASM("mem/memstd", func_002548D0);

INCLUDE_ASM("mem/memstd", func_00254C48);

extern "C" void* func_002548D0(void* self);

//99.29%
INCLUDE_ASM("mem/memstd", func_00254DA0__FPv);
#ifdef SKIP_ASM
void* func_00254DA0(void* self)
{
    return func_002548D0(self);
}
#endif

INCLUDE_ASM("mem/memstd", func_00254DC0);

INCLUDE_ASM("mem/memstd", func_00254E60);

INCLUDE_ASM("mem/memstd", func_00255638);

//100%
INCLUDE_ASM("mem/memstd", func_00255668__FPv);
#ifdef SKIP_ASM
void func_00255668(void* self)
{
}
#endif

extern "C" void* func_00254E60(int, int);

//99.38%
INCLUDE_ASM("mem/memstd", func_002557C0__FPv);
#ifdef SKIP_ASM
void* func_002557C0(void* self)
{
    return func_00254E60(1, 0xffff);
}
#endif

extern void* D_00481320[];

//100%
INCLUDE_ASM("mem/memstd", func_002557E0__FPv);
#ifdef SKIP_ASM
void* func_002557E0(void* self)
{
    int t0 = 0;
    *(int*)self = t0;
    *(int*)((char*)self + 0x8) = (int)(void*)D_00481320;
    *(int*)((char*)self + 0x4) = t0;
    return self;
}
#endif

INCLUDE_ASM("mem/memstd", func_00255800);

//100%
INCLUDE_ASM("mem/memstd", func_00255830__FPv);
#ifdef SKIP_ASM
int func_00255830(void* self)
{
    int t0 = 1;
    *(int*)((char*)self + 0x4) = 0;
    *(int*)self = t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00255840);
#ifdef SKIP_ASM
extern "C" void func_00255840(void* self)
{
    if (*(int*)self != 0) {
        *(int*)self = 0;
        *(int*)((char*)self + 0x4) = 0;
    }
}
#endif

