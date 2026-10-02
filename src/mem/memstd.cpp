#include "common.h"

INCLUDE_ASM("mem/memstd", MEMCLASS_create);

INCLUDE_ASM("mem/memstd", MEMCLASS_link);

//100%
INCLUDE_ASM("mem/memstd", func_00251F68);
#ifdef SKIP_ASM
// PORT: hand-rolled EE EABI va_start (gcc 2.95 va-mips.h form); use <stdarg.h> off-PS2.
typedef char* func_00251F68_va_list;
#define func_00251F68_va_start(ap)                                       \
    (ap = (char*)__builtin_next_arg()                                    \
          - (__builtin_args_info(2) < 8 ? (8 - __builtin_args_info(2)) * 8 : 0))

extern "C" int func_004186C8(char* dst, const char* fmt, char* ap);
extern "C" void func_003E5D30(int, char*);

extern "C" void func_00251F68(const char* fmt, ...)
{
    char buf[0x200];
    func_00251F68_va_list ap;
    func_00251F68_va_start(ap);
    func_004186C8(buf, fmt, ap);
    func_003E5D30(2, buf);
}
#endif

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

//100%
INCLUDE_ASM("mem/memstd", func_002526B8);
#ifdef SKIP_ASM
extern "C" void MUTEX_lock(void* mutex);
extern "C" void MUTEX_unlock(void* mutex);
extern "C" void* cMemMan_internalResizeBlock(void* block, int size);
extern void* D_00538B00[16];

extern "C" void* func_002526B8(void* block, int size)
{
    void* r;
    MUTEX_lock(D_00538B00);
    r = cMemMan_internalResizeBlock(block, size);
    MUTEX_unlock(D_00538B00);
    return r;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00252980);
#ifdef SKIP_ASM
extern "C" int func_00320B68(int);
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");

// PORT: the unit declares func_00252980 as `void* (void*)`; existing callers bind the
// 6-argument form below by asm label (the 6th argument is unused).
void* func_00252980_6(void*, int, int, int, int, int) __asm__("func_00252980");

void* func_00252980_6(void* tag, int size, int align, int d, int flags, int unused)
{
    int lvl = func_00320B68(align) - 2 < 0 ? 0 : func_00320B68(align) - 2;
    return operator_new_tag(size, (const char*)tag, flags | (lvl << 24) | 0x10000000, d);
}
#endif

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

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", func_002533C8);
#ifdef SKIP_ASM
extern "C" void func_00253AA0();

extern "C" void* func_002533C8(void* self)
{
    func_00253AA0();
    func_00253390((char*)self + 0x14);
    *(float*)((char*)self + 0x10) = *(float*)((char*)self + 0x34);
    *(int*)((char*)self + 0x38) = 0;
    *(int*)((char*)self + 0x3C) = 0;
    *(int*)((char*)self + 0xC) = 0;
    *(int*)((char*)self + 0x40) = 0;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00253418);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void func_002535F8(void* self);

extern "C" void func_00253418(void* self, int flags)
{
    func_002535F8(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

INCLUDE_ASM("mem/memstd", func_002534A8);

INCLUDE_ASM("mem/memstd", func_002535F8);

INCLUDE_ASM("mem/memstd", func_002536C8);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("mem/memstd", func_00253860);
#ifdef SKIP_ASM
extern "C" void func_002536C8(void* self);

extern "C" int func_00253860(void* self)
{
    if (*(void**)((char*)self + 0x38) == 0) {
        return 0;
    }
    func_002536C8(self);
    return 1;
}
#endif

INCLUDE_ASM("mem/memstd", func_00253890);

INCLUDE_ASM("mem/memstd", func_00253938);

//100%
INCLUDE_ASM("mem/memstd", func_002539E0);
#ifdef SKIP_ASM
int func_003AE938(void*);

extern "C" void func_002539E0(void* self)
{
    void* p = *(void**)((char*)self + 0x38);
    if (p != 0 && *(int*)((char*)self + 0x3C) != 0 && *(int*)((char*)self + 0x0) == 0
        && *(int*)((char*)self + 0x8) == 0) {
        func_003AE938(p);
        *(int*)((char*)self + 0x8) = 1;
    }
}
#endif

//100%
INCLUDE_ASM("mem/memstd", func_00253A40);
#ifdef SKIP_ASM
void func_003AE958(void*);

extern "C" void func_00253A40(void* self)
{
    void* p = *(void**)((char*)self + 0x38);
    if (p != 0 && *(int*)((char*)self + 0x3C) != 0 && *(int*)((char*)self + 0x0) == 0
        && *(int*)((char*)self + 0x8) != 0) {
        func_003AE958(p);
        *(int*)((char*)self + 0x8) = 0;
    }
}
#endif

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

//100%
INCLUDE_ASM("mem/memstd", func_00254368);
#ifdef SKIP_ASM
extern char D_004A2EA0[];
extern "C" void* func_00254410(void* self, void* a0, int a1, int a2, int a3);

// PORT: the unit's sMemAllocFuncs declares alloc as `void* (*)(int)`; it really
// takes (tag, size, align, d, flags) (see func_00252F60 / func_00253AD0).
typedef void* (*tMemAlloc5_00254368)(void*, int, int, int, int);

extern "C" void* func_00254368(void* a0, int a1, int a2, int a3)
{
    void* p = ((tMemAlloc5_00254368)D_00509430.alloc)(D_004A2EA0, 0x54, 0, 0, D_00509430.field_0x8);
    return func_00254410(p, a0, a1, a2, a3);
}
#endif

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

//100%
INCLUDE_ASM("mem/memstd", func_00254DC0);
#ifdef SKIP_ASM
struct sMemStdVEntry {
    short delta;
    short index;
    int (*fn)(void*);
};

extern "C" void func_003B0680(void*, void*);

extern "C" void func_00254DC0(void* self)
{
    void* o;
    sMemStdVEntry* vt;
    do {
        o = *(void**)((char*)self + 0x8);
        vt = *(sMemStdVEntry**)((char*)o + 0x10D8);
    } while (vt[18].fn((char*)o + vt[18].delta) == 0);
    void* p = *(void**)((char*)self + 0xC);
    if (p != 0) {
        func_003B0680(*(void**)((char*)self + 0x0), p);
        *(void**)((char*)self + 0xC) = 0;
    }
}
#endif

INCLUDE_ASM("mem/memstd", func_00254E60);

//100%
INCLUDE_ASM("mem/memstd", func_00255638);
#ifdef SKIP_ASM
extern void* D_004802B0[];
void operator_delete(int*);

extern "C" void func_00255638(void* self, int flags)
{
    *(void***)((char*)self + 0x4) = D_004802B0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

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

//100%
INCLUDE_ASM("mem/memstd", func_00255800);
#ifdef SKIP_ASM
void operator_delete(int*);

extern "C" void func_00255800(void* self, int flags)
{
    *(void***)((char*)self + 0x8) = D_00481320;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

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

