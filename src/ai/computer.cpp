#include "common.h"

INCLUDE_ASM("ai/computer", cComputer_setIndividualRiderDifficulty);

INCLUDE_ASM("ai/computer", cComputer_updateRiderDifficulty);

INCLUDE_ASM("ai/computer", func_0010C9A8);

extern "C" void cComputer_updateRiderDifficulty(void*);

//99.29% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("ai/computer", func_0010CAB8);
#ifdef SKIP_ASM
extern "C" void func_0010CAB8(void* self)
{
    cComputer_updateRiderDifficulty(self);
}
#endif

INCLUDE_ASM("ai/computer", func_0010CAD8);

INCLUDE_ASM("ai/computer", func_0010CBE0);

INCLUDE_ASM("ai/computer", func_0010CD20);

INCLUDE_ASM("ai/computer", func_0010CF68);

//100%
INCLUDE_ASM("ai/computer", func_0010D170);
#ifdef SKIP_ASM
struct sComputer_0010D170
{
    char pad[0xE7C];
    int a[15];
    int b[15];
    int c[15];
};

extern "C" void func_0010D170(sComputer_0010D170* self)
{
    for (int i = 0; i < 15; i++)
    {
        self->a[i] = 0;
        self->b[i] = 0;
        self->c[i] = 0;
    }
}
#endif

INCLUDE_ASM("ai/computer", func_0010D1A0);

INCLUDE_ASM("ai/computer", func_0010D410);

INCLUDE_ASM("ai/computer", func_0010D870);

//100%
INCLUDE_ASM("ai/computer", func_0010D8F8);
#ifdef SKIP_ASM
struct sComputer_0010D8F8
{
    int a;
    int b;
    char pad[0x1C];
};

extern "C" int func_0010D8F8(void* self)
{
    sComputer_0010D8F8* p = *(sComputer_0010D8F8**)((char*)self + 0x18);
    for (int i = 0; i < 6; i++, p++)
    {
        if (p->a != 0 && p->b != 0)
            return i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010D9E8);
#ifdef SKIP_ASM
extern "C" int func_0010D9E8(void* self, int a1)
{
    void* p = *(void**)((char*)self + 0x18);
    int r = 0;
    if (*(int*)((char*)p + 0xf0) != 0) {
        r = *(int*)((char*)p + 0xf8) == a1;
    }
    return r;
}
#endif

INCLUDE_ASM("ai/computer", func_0010DA10);

INCLUDE_ASM("ai/computer", func_0010DBF0);

INCLUDE_ASM("ai/computer", func_0010DEB0);

INCLUDE_ASM("ai/computer", func_0010DEF0);

INCLUDE_ASM("ai/computer", func_0010E028);

INCLUDE_ASM("ai/computer", func_0010E098);

INCLUDE_ASM("ai/computer", func_0010E228);

INCLUDE_ASM("ai/computer", func_0010E2E8);

INCLUDE_ASM("ai/computer", func_0010E3A8);

INCLUDE_ASM("ai/computer", func_0010E468);

INCLUDE_ASM("ai/computer", func_0010E558);

INCLUDE_ASM("ai/computer", func_0010E5D8);

//100%
INCLUDE_ASM("ai/computer", func_0010E770);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_0029CED8(void*, int, void*, float);
extern "C" void func_002A3B18(void*, void*, int);

extern "C" void func_0010E770(void* self, float amount)
{
    *(float*)((char*)self + 0x2E8) += amount;
    func_0029CED8(func_0028B180(), 0, self, 1.0f);
    func_002A3B18(func_0028B180(), self, 1);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010E7D0);
#ifdef SKIP_ASM
extern "C" void* func_0028B180();
extern "C" void func_0029CED8(void*, int, void*, float);
extern "C" void func_002A3B18(void*, void*, int);

extern "C" void func_0010E7D0(void* self, float amount)
{
    *(float*)((char*)self + 0x2EC) += amount;
    func_0029CED8(func_0028B180(), 1, self, 1.0f);
    func_002A3B18(func_0028B180(), self, 2);
}
#endif

INCLUDE_ASM("ai/computer", func_0010E830);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", func_0010E8B8);
#ifdef SKIP_ASM
extern "C" void func_0010E098(void*, int, float);
extern "C" float func_00119608(int);
extern "C" void* func_0028B180();
extern "C" void func_0029CED8(void*, int, void*, float);

extern "C" void func_0010E8B8(void* self)
{
    func_0010E098(self, 0x10, func_00119608(*(int*)((char*)self + 0x790)));
    func_0029CED8(func_0028B180(), 3, self, 1.0f);
}
#endif

INCLUDE_ASM("ai/computer", func_0010E910);

INCLUDE_ASM("ai/computer", func_0010EB30);

INCLUDE_ASM("ai/computer", func_0010F1C0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("ai/computer", func_0010F280);
#ifdef SKIP_ASM
extern "C" float func_00119BB0(int);
extern "C" void func_0010E098(void*, int, float);

extern "C" void func_0010F280(void* self)
{
    func_0010E098(self, 1, func_00119BB0(*(int*)((char*)self + 0x790)));
}
#endif

extern "C" void* func_0011A0E0(int);

//100%
INCLUDE_ASM("ai/computer", func_0010F2B8__FPv);
#ifdef SKIP_ASM
void* func_0010F2B8(void* self)
{
    return func_0011A0E0(*(int*)((char*)self + 0x790));
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010F2D8);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_0011A110(int, int);
extern "C" void func_00159B08(void*, int, int);

extern "C" void func_0010F2D8(void* self, int a, int b)
{
    func_0011A110(*(int*)((char*)self + 0x790), b);
    func_00159B08(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(int*)((char*)self + 0x86C), a);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_0010F338);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void func_00119EF8(int, int);
extern "C" void func_001599A0(void*, int, int);

extern "C" void func_0010F338(void* self, int a)
{
    func_00119EF8(*(int*)((char*)self + 0x790), 3);
    func_001599A0(cBE_getInterface_Fv(cBE_getBE(), 0xD), *(int*)((char*)self + 0x86C), a);
}
#endif

extern "C" void* func_0010F3B8(void* self);

//100%
INCLUDE_ASM("ai/computer", func_0010F398__FPv);
#ifdef SKIP_ASM
void* func_0010F398(void* self)
{
    return func_0010F3B8(self);
}
#endif

INCLUDE_ASM("ai/computer", func_0010F3B8);

INCLUDE_ASM("ai/computer", func_0010F560);

INCLUDE_ASM("ai/computer", func_0010F878);

INCLUDE_ASM("ai/computer", func_0010F998);

INCLUDE_ASM("ai/computer", func_0010FC30);

INCLUDE_ASM("ai/computer", func_0010FCD8);

//100%
INCLUDE_ASM("ai/computer", func_001112B8);
#ifdef SKIP_ASM
extern "C" void func_001112F8();
extern "C" void func_00111380(void*, int);

extern "C" void func_001112B8(void* self, int v)
{
    func_001112F8();
    int old = *(int*)((char*)self + 0xDE0);
    *(int*)((char*)self + 0xDE0) = v;
    func_00111380(self, old);
}
#endif

INCLUDE_ASM("ai/computer", func_001112F8);

INCLUDE_ASM("ai/computer", func_00111380);

INCLUDE_ASM("ai/computer", func_00111408);

INCLUDE_ASM("ai/computer", func_001114A0);

//100%
INCLUDE_ASM("ai/computer", func_00111538);
#ifdef SKIP_ASM
extern "C" void func_00111578();
extern "C" void func_00111630(void*, int);

extern "C" void func_00111538(void* self, int v)
{
    func_00111578();
    int old = *(int*)((char*)self + 0xDE4);
    *(int*)((char*)self + 0xDE4) = v;
    func_00111630(self, old);
}
#endif

INCLUDE_ASM("ai/computer", func_00111578);

INCLUDE_ASM("ai/computer", func_00111630);

INCLUDE_ASM("ai/computer", func_00111728);

INCLUDE_ASM("ai/computer", func_00111890);

extern "C" void* func_002E23E0(void*);

//100%
INCLUDE_ASM("ai/computer", func_00111AA0__FPv);
#ifdef SKIP_ASM
// PORT: prototype mismatch. The project names this func_00111AA0__FPv (one
// void* param), but every caller passes (self, vecA, vecB, int, float) and the
// body forwards them to func_002E23E0 with an extra 0 flag. The unit declares
// func_002E23E0 as (void*), so both are bound by asm label.
void func_002E23E0_impl(void* self, void* a, void* b, int c, int flag, float x) __asm__("func_002E23E0");

void func_00111AA0_impl(void* self, void* a, void* b, int c, float x) __asm__("func_00111AA0__FPv");

void func_00111AA0_impl(void* self, void* a, void* b, int c, float x)
{
    func_002E23E0_impl((char*)self + 0xB40, a, b, c, 0, x);
}
#endif

INCLUDE_ASM("ai/computer", func_00111AC0);

INCLUDE_ASM("ai/computer", func_00111D98);

INCLUDE_ASM("ai/computer", func_00112180);

INCLUDE_ASM("ai/computer", func_00112338);

//100%
INCLUDE_ASM("ai/computer", func_00112588);
#ifdef SKIP_ASM
extern "C" float func_00112588(void* self, int arg1)
{
    float v = 796.0f;
    if (arg1 == 0) {
        v = 200.0f;
    }
    return v;
}
#endif

// R5900 128-bit GPR quadword, for functions that copy/return a 16-byte
// block via a single lq/sq pair instead of word-by-word.
typedef int cQuad128 __attribute__((mode(TI)));

//100%
INCLUDE_ASM("ai/computer", func_001125A8);
#ifdef SKIP_ASM
extern "C" cQuad128 func_001125A8(void* self)
{
    cQuad128 v = *(cQuad128*)((char*)self + 0x4A0);
    *(cQuad128*)((char*)self + 0x4B0) = v;
    return v;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_001125B8__FPv);
#ifdef SKIP_ASM
int func_001125B8(void* self)
{
    return 0;
}
#endif

INCLUDE_ASM("ai/computer", func_001125C0);

INCLUDE_ASM("ai/computer", func_001127F0);

INCLUDE_ASM("ai/computer", func_00112A50);

INCLUDE_ASM("ai/computer", func_00112D58);

INCLUDE_ASM("ai/computer", func_00112FB0);

//100%
INCLUDE_ASM("ai/computer", func_00113128__FPv);
#ifdef SKIP_ASM
float func_00113128(void* self)
{
    return *(float*)((char*)self + 0x4D0);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00113130__FPv);
#ifdef SKIP_ASM
float func_00113130(void* self)
{
    return *(float*)((char*)self + 0x4D8);
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00113138);
#ifdef SKIP_ASM
extern "C" void cAirPredictor_reset(char* self);

extern "C" void* func_00113138(void* self)
{
    *(unsigned int*)((char*)self + 0x30) = 0xFFFFFFFF;
    cAirPredictor_reset((char*)self);
    return self;
}
#endif

//100%
INCLUDE_ASM("ai/computer", func_00113170);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

extern "C" void func_00113170(void* self, int flags)
{
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

