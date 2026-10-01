#include "common.h"

INCLUDE_ASM("scripter/bxscriptengine", cBXScriptEngine_SetupBXEngine);

INCLUDE_ASM("scripter/bxscriptengine", func_00282020);

INCLUDE_ASM("scripter/bxscriptengine", func_002820B0);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282150);
#ifdef SKIP_ASM
struct sSlot282150 { int value; int pad; };
extern "C" int func_00282150(void* self, int value, int index)
{
    sSlot282150* slots = *(sSlot282150**)((char*)self + 0x2b0);
    if (slots[index].value != 0) {
        return 0;
    }
    slots[index].value = value;
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282178);
#ifdef SKIP_ASM
extern "C" int func_00282178(void* self, int index)
{
    sSlot282150* slots = *(sSlot282150**)((char*)self + 0x2b0);
    int old = slots[index].value;
    if (old == 0) {
        return 0;
    }
    slots[index].value = 0;
    return old;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_002821A0);

INCLUDE_ASM("scripter/bxscriptengine", func_002822A0);

INCLUDE_ASM("scripter/bxscriptengine", func_00282338);

INCLUDE_ASM("scripter/bxscriptengine", func_00282390);

INCLUDE_ASM("scripter/bxscriptengine", func_002823F0);

INCLUDE_ASM("scripter/bxscriptengine", func_00282450);

INCLUDE_ASM("scripter/bxscriptengine", func_00282540);

INCLUDE_ASM("scripter/bxscriptengine", func_002826D8);

INCLUDE_ASM("scripter/bxscriptengine", func_00282720);

INCLUDE_ASM("scripter/bxscriptengine", func_00282798);

INCLUDE_ASM("scripter/bxscriptengine", func_00282838);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_002828B8);
#ifdef SKIP_ASM
struct sScriptThread2C0 {
    int a;          // 0x00
    int b;          // 0x04
    int c;          // 0x08
    int d;          // 0x0C
    short e;        // 0x10
    short f;        // 0x12
    int g;          // 0x14
    int h;          // 0x18
    int id;         // 0x1C
    int k;          // 0x20
};
struct sScriptEngine2B8 {
    char pad[0x2b8];
    int count;                      // 0x2B8
    int unk2BC;                     // 0x2BC
    sScriptThread2C0 threads[16];   // 0x2C0
};
extern "C" void func_002828B8(sScriptEngine2B8* self)
{
    int i;
    self->count = 0;
    self->unk2BC = 0;
    for (i = 0; i < 16; i++) {
        self->threads[i].id = i;
        self->threads[i].k = 0;
        self->threads[i].a = 0;
        self->threads[i].b = 0;
        self->threads[i].e = 0;
        self->threads[i].f = 0;
        self->threads[i].h = 0;
        self->threads[i].g = -1;
    }
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282908);

INCLUDE_ASM("scripter/bxscriptengine", func_002829D0);

INCLUDE_ASM("scripter/bxscriptengine", func_00282A18);

INCLUDE_ASM("scripter/bxscriptengine", func_00282A80);

INCLUDE_ASM("scripter/bxscriptengine", func_00282B40);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282B88);
#ifdef SKIP_ASM
void func_00282C88(void* self, void* v);

extern "C" void func_00282B88(void* self, void* ctx)
{
    void* v = *(void**)((char*)ctx + 0xC);
    *(int*)((char*)v + 0x4) = 7;
    func_00282C88(self, v);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282BB0__FPvT0);
#ifdef SKIP_ASM
int func_00282BB0(void* self, void* a1)
{
    return *(int*)((char*)*(void**)((char*)a1 + 0xc) + 0xc);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282BE0__FPvT0);
#ifdef SKIP_ASM
int func_00282BE0(void* self, void* a1)
{
    return *(int*)((char*)*(void**)((char*)a1 + 0xc) + 0x1c);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282BF0);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1);

extern "C" int func_00282BF0(void* self, int a1)
{
    int* p = (int*)func_00282CB0(self, a1);
    if (p == 0) {
        return 0;
    }
    return *p;
}
#endif

extern "C" void* func_00272CC0(int);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282C18__FPv);
#ifdef SKIP_ASM
void* func_00282C18(void* self)
{
    return func_00272CC0(*(int*)((char*)self + 0x2b8));
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282C38);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282C88__FPvT0);
#ifdef SKIP_ASM
void func_00282C88(void* self, void* a1)
{
    *(int*)((char*)a1 + 0x0) = 0;
    *(int*)((char*)a1 + 0x4) = 0;
    *(int*)((char*)a1 + 0x20) = 0;
    *(short*)((char*)a1 + 0x10) = 0;
    *(short*)((char*)a1 + 0x12) = 0;
    *(int*)((char*)a1 + 0x14) = -1;
    *(int*)((char*)a1 + 0x18) = 0;
    *(int*)((char*)a1 + 0xc) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282CB0);
#ifdef SKIP_ASM
extern "C" void* func_00282CB0(void* self, int a1)
{
    char* p = (char*)self + (a1 * 0x24 + 0x2c0);
    return *(int*)(p + 0x20) != 0 ? p : 0;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282CD0);

INCLUDE_ASM("scripter/bxscriptengine", func_00282D30);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282DA8__FPvi);
#ifdef SKIP_ASM
void func_00282DA8(void* self, int val)
{
    *(int*)((char*)self + 0x10) = val;
}
#endif

extern "C" void* func_00282DD0(void*, int, int, int);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282DB0__FPvii);
#ifdef SKIP_ASM
void* func_00282DB0(void* self, int a1, int a2)
{
    return func_00282DD0(self, a1, a2, *(int*)((char*)self + 0x28));
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282DD0);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282EF0);
#ifdef SKIP_ASM
extern "C" int func_00282EF0(void* self, int a1)
{
    return *(int*)((char*)self + 0x14) == a1;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282F00);

INCLUDE_ASM("scripter/bxscriptengine", func_00282F30);

extern void* D_00482418[];

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282F60__FPv);
#ifdef SKIP_ASM
void* func_00282F60(void* self)
{
    *(void***)self = D_00482418;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282F80__FPv);
#ifdef SKIP_ASM
int func_00282F80(void* self)
{
    return (*(int*)((char*)self + 0x8) != 0);
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00282F90);
#ifdef SKIP_ASM
extern "C" void func_00274240(void* p, int a1);

extern "C" void func_00282F90(void* self)
{
    void* p = *(void**)((char*)self + 0x8);
    if (p != 0) {
        func_00274240(p, 0);
    }
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00282FB8);

INCLUDE_ASM("scripter/bxscriptengine", func_00283000);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283038);
#ifdef SKIP_ASM
extern "C" void func_002749E8(void* self);

extern "C" void func_00283038(void* self)
{
    void* p = *(void**)((char*)self + 0x8);
    if (p != 0) {
        func_002749E8(*(void**)((char*)p + 0x8));
    }
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283060);
#ifdef SKIP_ASM
extern "C" void func_00274A08(void* self);

extern "C" void func_00283060(void* self)
{
    void* p = *(void**)((char*)self + 0x8);
    if (p != 0) {
        func_00274A08(*(void**)((char*)p + 0x8));
    }
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00283088);

INCLUDE_ASM("scripter/bxscriptengine", func_002830C8);

INCLUDE_ASM("scripter/bxscriptengine", func_00283180);

INCLUDE_ASM("scripter/bxscriptengine", func_002831B0);

extern void* D_00482558[];

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283200__FPvi);
#ifdef SKIP_ASM
void* func_00283200(void* self, int a1)
{
    *(void***)((char*)self + 0xc) = D_00482558;
    *(int*)self = a1;
    *(int*)((char*)self + 0x4) = -1;
    *(int*)((char*)self + 0x8) = 0;
    return self;
}
#endif

INCLUDE_ASM("scripter/bxscriptengine", func_00283228);

INCLUDE_ASM("scripter/bxscriptengine", func_00283298);

INCLUDE_ASM("scripter/bxscriptengine", func_002832D8);

INCLUDE_ASM("scripter/bxscriptengine", func_00283320);

INCLUDE_ASM("scripter/bxscriptengine", func_002833A8);

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283430__FPv);
#ifdef SKIP_ASM
void* func_00283430(void* self)
{
    void* t0 = (char*)*(void**)((char*)self + 0x8) + 0x1;
    *(int*)((char*)self + 0x8) = (int)t0;
    return t0;
}
#endif

//100%
INCLUDE_ASM("scripter/bxscriptengine", func_00283440__FPv);
#ifdef SKIP_ASM
void* func_00283440(void* self)
{
    void* t0 = (char*)*(void**)((char*)self + 0x8) - 0x1;
    *(int*)((char*)self + 0x8) = (int)t0;
    return t0;
}
#endif

