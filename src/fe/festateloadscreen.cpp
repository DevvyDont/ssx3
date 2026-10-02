#include "common.h"

INCLUDE_ASM("fe/festateloadscreen", cFELoadScreen_load);

INCLUDE_ASM("fe/festateloadscreen", func_00233640);

INCLUDE_ASM("fe/festateloadscreen", func_002338C8);

INCLUDE_ASM("fe/festateloadscreen", func_00233930);

INCLUDE_ASM("fe/festateloadscreen", func_002339F8);

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233A70);
#ifdef SKIP_ASM
extern "C" void func_00398038(void*);
void func_00231CB0(void*);

extern "C" void func_00233A70(void* self)
{
    func_00398038(*(void**)((char*)self + 0xC));
    func_00231CB0(self);
}
#endif

INCLUDE_ASM("fe/festateloadscreen", func_00233AA0);

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233AF0);
#ifdef SKIP_ASM
struct sVEntry_func_00233AF0 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

// PORT: the unit declares func_00233AF0 as `void* (void*)`; the body also uses $a1 (obj).
// Bound by asm label.
void* func_00233AF0_2(void* self, void* obj) __asm__("func_00233AF0");

void* func_00233AF0_2(void* self, void* obj)
{
    sVEntry_func_00233AF0* vt = *(sVEntry_func_00233AF0**)obj;
    return vt[1].fn((char*)obj + vt[1].delta, self, 4);
}
#endif

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233B28);
#ifdef SKIP_ASM
struct sVEntry_func_00233B28 {
    short delta;
    short index;
    void* (*fn)(void*, void*, int);
};

// PORT: the unit declares func_00233B28 as `void* (void*)`; the body also uses $a1 (obj).
// Bound by asm label.
void* func_00233B28_2(void* self, void* obj) __asm__("func_00233B28");

void* func_00233B28_2(void* self, void* obj)
{
    sVEntry_func_00233B28* vt = *(sVEntry_func_00233B28**)obj;
    return vt[2].fn((char*)obj + vt[2].delta, self, 4);
}
#endif

INCLUDE_ASM("fe/festateloadscreen", func_00233B60);

//100%
INCLUDE_ASM("fe/festateloadscreen", func_00233B88__FPv);
#ifdef SKIP_ASM
void func_00233B88(void* self)
{
}
#endif

extern "C" void* func_00233AF0(void* self);

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233B90__FPv);
#ifdef SKIP_ASM
void* func_00233B90(void* self)
{
    return func_00233AF0(self);
}
#endif

extern "C" void* func_00233B28(void* self);

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233BB0__FPv);
#ifdef SKIP_ASM
void* func_00233BB0(void* self)
{
    return func_00233B28(self);
}
#endif

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233BD0__FPv);
#ifdef SKIP_ASM
void* func_00233BD0(void* self)
{
    return func_00233AF0(self);
}
#endif

//99.29%
INCLUDE_ASM("fe/festateloadscreen", func_00233BF0__FPv);
#ifdef SKIP_ASM
void* func_00233BF0(void* self)
{
    return func_00233B28(self);
}
#endif

INCLUDE_ASM("fe/festateloadscreen", func_00233C10);

INCLUDE_ASM("fe/festateloadscreen", func_00233C50);

INCLUDE_ASM("fe/festateloadscreen", func_00233CD8);

INCLUDE_ASM("fe/festateloadscreen", func_00234008);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", func_002340B8);
#ifdef SKIP_ASM
struct sVEntry_func_002340B8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_002340B8(void* self, void* stream)
{
    func_00233AF0_2(self, stream);
    sVEntry_func_002340B8* e = &(*(sVEntry_func_002340B8**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_002340B8**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("fe/festateloadscreen", func_00234120);
#ifdef SKIP_ASM
struct sVEntry_func_00234120 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern "C" void func_00234120(void* self, void* stream)
{
    func_00233B28_2(self, stream);
    sVEntry_func_00234120* e = &(*(sVEntry_func_00234120**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x10, 4);
    e = &(*(sVEntry_func_00234120**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 0x14, 4);
}
#endif

INCLUDE_ASM("fe/festateloadscreen", func_00234188);

INCLUDE_ASM("fe/festateloadscreen", func_002341D0);

INCLUDE_ASM("fe/festateloadscreen", func_002343B0);

INCLUDE_ASM("fe/festateloadscreen", func_00234750);

INCLUDE_ASM("fe/festateloadscreen", func_00234910);

INCLUDE_ASM("fe/festateloadscreen", func_00234990);

INCLUDE_ASM("fe/festateloadscreen", func_00234A30);

