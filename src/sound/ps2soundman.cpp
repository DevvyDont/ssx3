#include "common.h"

INCLUDE_ASM("sound/ps2soundman", cSndPlayOpts_cSndPlayOpts);

//100%
INCLUDE_ASM("sound/ps2soundman", func_002A97C0);
#ifdef SKIP_ASM
void cMemMan_free(void* p);
void operator_delete(int* ptr);
extern "C" void func_002ADEE8(void* self, int flags);

struct sSmVEntry97C0 {
    short delta;
    short index;
    void* fn;
};
struct sSmVtbl97C0 {
    sSmVEntry97C0 e[4];
} __attribute__((aligned(8)));
extern const sSmVtbl97C0 D_00483BE0;

// PORT: g++ 2.95 virtual-base destruction with a stack vtable this-adjust fix-up, written out by hand.
extern "C" void func_002A97C0(char* self, int flags)
{
    *(const sSmVtbl97C0**)(*(char**)self + 4) = &D_00483BE0;
    if (flags == 0) {
        sSmVtbl97C0 t1 = D_00483BE0;
        *(sSmVtbl97C0**)(*(char**)self + 4) = &t1;
        char* base = *(char**)self - 0x88;
        int d = self - base;
        t1.e[1].delta = D_00483BE0.e[1].delta + d;
    }
    if (*(void**)(self + 0x20) != 0) {
        cMemMan_free(*(void**)(self + 0x20));
    }
    if (*(void**)(self + 0x34) != 0) {
        cMemMan_free(*(void**)(self + 0x34));
    }
    if (*(void**)(self + 0x38) != 0) {
        cMemMan_free(*(void**)(self + 0x38));
    }
    if (*(void**)(self + 0x3C) != 0) {
        cMemMan_free(*(void**)(self + 0x3C));
    }
    if (*(void**)(self + 0x60) != 0) cMemMan_free(*(void**)(self + 0x60));
    if (*(void**)(self + 0x64) != 0) cMemMan_free(*(void**)(self + 0x64));
    if (*(void**)(self + 0x68) != 0) cMemMan_free(*(void**)(self + 0x68));
    if (*(void**)(self + 0x6C) != 0) cMemMan_free(*(void**)(self + 0x6C));
    if (*(void**)(self + 0x70) != 0) cMemMan_free(*(void**)(self + 0x70));
    if (*(void**)(self + 0x74) != 0) cMemMan_free(*(void**)(self + 0x74));
    if (*(void**)(self + 0x78) != 0) cMemMan_free(*(void**)(self + 0x78));
    if (*(void**)(self + 0x7C) != 0) cMemMan_free(*(void**)(self + 0x7C));
    if (*(void**)(self + 0x80) != 0) cMemMan_free(*(void**)(self + 0x80));
    if (*(void**)(self + 0x84) != 0) cMemMan_free(*(void**)(self + 0x84));
    if (flags & 2) {
        func_002ADEE8(*(void**)self, 0);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("sound/ps2soundman", func_002A9988);
#ifdef SKIP_ASM
extern "C" void func_002A9988(void* self, float a, float b)
{
    (*(float**)((char*)self + 0x6C))[*(int*)((char*)self + 0x1C)] = a;
    if (b < 0.0f) {
        (*(float**)((char*)self + 0x68))[*(int*)((char*)self + 0x1C)] = a;
        return;
    }
    (*(float**)((char*)self + 0x68))[*(int*)((char*)self + 0x1C)] = b;
}
#endif

