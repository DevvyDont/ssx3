#include "common.h"

INCLUDE_ASM("object/deadfadenode", cDeadFadeNode_cDeadFadeNode);

//100%
INCLUDE_ASM("object/deadfadenode", func_00350B98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cInstanceNode_ctor_v(void* self) __asm__("cInstanceNode_cInstanceNode");
extern char D_00491980[];

extern "C" void* func_00350B98(void* self)
{
    cInstanceNode_ctor_v(self);
    *(void**)((char*)self + 0xC) = D_00491980;
    *(unsigned int*)((char*)self + 0x1C) = 0xFFFFFFFFU;
    *(int*)((char*)self + 0x38) = 0;
    return self;
}
#endif

INCLUDE_ASM("object/deadfadenode", func_00350BE0);

INCLUDE_ASM("object/deadfadenode", func_00350C38);

//100%
INCLUDE_ASM("object/deadfadenode", func_00350C90);
#ifdef SKIP_ASM
// PORT: the project names this func_002D1CC0__FPv (one void* arg), but it is a
// pass-through wrapper and this caller passes two args (prototype mismatch).
void* func_002D1CC0_2(void*, void*) __asm__("func_002D1CC0__FPv");

extern "C" void func_00350C90(void* self)
{
    func_002D1CC0_2(*(void**)((char*)self + 0x18), (char*)self + 0x1C);
}
#endif

INCLUDE_ASM("object/deadfadenode", func_00350CB8);

extern "C" void* func_0034FE90(void* self);

//100%
INCLUDE_ASM("object/deadfadenode", func_00350E70__FPv);
#ifdef SKIP_ASM
void* func_00350E70(void* self)
{
    return func_0034FE90(self);
}
#endif

INCLUDE_ASM("object/deadfadenode", func_00350E90);

//100%
INCLUDE_ASM("object/deadfadenode", func_00350F08);
#ifdef SKIP_ASM
extern "C" void cInstanceNode_cInstanceNode(void* self);
extern char D_00491800[];

extern "C" void* func_00350F08(void* self)
{
    cInstanceNode_cInstanceNode(self);
    *(void**)((char*)self + 0xC) = D_00491800;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", func_00350F40__FPv);
#ifdef SKIP_ASM
void* func_00350F40(void* self)
{
    return func_0034FE90(self);
}
#endif

INCLUDE_ASM("object/deadfadenode", func_00350F60);

//100%
INCLUDE_ASM("object/deadfadenode", func_00350FD8);
#ifdef SKIP_ASM
extern "C" void cInstanceNode_cInstanceNode(void* self);
extern char D_00491680[];

extern "C" void* func_00350FD8(void* self)
{
    cInstanceNode_cInstanceNode(self);
    *(void**)((char*)self + 0xC) = D_00491680;
    return self;
}
#endif

//100%
INCLUDE_ASM("object/deadfadenode", func_00351010__FPv);
#ifdef SKIP_ASM
void* func_00351010(void* self)
{
    return func_0034FE90(self);
}
#endif

