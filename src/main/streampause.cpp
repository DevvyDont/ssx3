#include "common.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char D_0047C520[];
extern void* D_0047D0E8[16];

struct cStreamPause {
    void* vtable;
};

//99.67%
cStreamPause* cStreamPause_construct()
{
    cStreamPause* self = (cStreamPause*)cMemMan_alloc(4, D_0047C520, 0x20000000, 0);
    self->vtable = D_0047D0E8;
    return self;
}

extern "C" void* func_00242D50(void* p, ...)
{
    return p;
}

extern "C" int func_00242D90(const signed char* a, const signed char* b, const signed char* c, const signed char* d)
{
    for (; a != b && c != d; ++a, ++c) {
        if (*a < *c) {
            return 1;
        }
        if (*c < *a) {
            return 0;
        }
    }
    return a == b && c != d;
}

extern "C" int func_00242DF8(const signed char* a, const signed char* b, const signed char* c, const signed char* d)
{
    while (a != b && c != d) {
        if (*a < *c) {
            return -1;
        }
        if (*c < *a) {
            return 1;
        }
        ++a;
        ++c;
    }
    if (c == d) {
        return !(a == b);
    }
    return -1;
}

void operator_delete(int* ptr);

struct sSPNode2E58 {
    sSPNode2E58* next;
    sSPNode2E58* prev;
};

extern "C" void func_00242E58(char* self)
{
    sSPNode2E58* n = (*(sSPNode2E58**)(self + 4))->next;
    while (n != *(sSPNode2E58**)(self + 4)) {
        sSPNode2E58* t = n;
        n = n->next;
        operator_delete((int*)t);
    }
    (*(sSPNode2E58**)(self + 4))->next = *(sSPNode2E58**)(self + 4);
    (*(sSPNode2E58**)(self + 4))->prev = *(sSPNode2E58**)(self + 4);
}

INCLUDE_ASM("main/streampause", func_00242EB8);

//100%
void operator_delete(int* ptr);
extern void* D_0047D6E8[];
extern void* D_0046D970[];

extern "C" void func_00243838(void* self, int flags)
{
    void** m = D_0047D6E8;
    *(void***)((char*)self + 0x0) = D_0046D970;
    *(void***)((char*)self + 0x1F8) = m;
    *(void***)((char*)self + 0x1E8) = m;
    *(void***)((char*)self + 0x1C4) = m;
    *(void***)((char*)self + 0x1A8) = m;
    *(void***)((char*)self + 0x198) = m;
    *(void***)((char*)self + 0x184) = m;
    *(void***)((char*)self + 0x164) = m;
    *(void***)((char*)self + 0x154) = m;
    *(void***)((char*)self + 0x144) = m;
    *(void***)((char*)self + 0x134) = m;
    *(void***)((char*)self + 0x124) = m;
    *(void***)((char*)self + 0x10C) = m;
    *(void***)((char*)self + 0xFC) = m;
    *(void***)((char*)self + 0xEC) = m;
    *(void***)((char*)self + 0xDC) = m;
    *(void***)((char*)self + 0xBC) = m;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}


extern "C" void func_002438B0(void* self, int flags)
{
    *(void***)self = D_0046D970;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}

extern "C" void func_002438E0()
{
}

extern "C" void func_002438E8()
{
}
