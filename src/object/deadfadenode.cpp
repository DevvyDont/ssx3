#include "common.h"

INCLUDE_ASM("object/deadfadenode", cDeadFadeNode_cDeadFadeNode);

INCLUDE_ASM("object/deadfadenode", func_00350B98);

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

INCLUDE_ASM("object/deadfadenode", func_00350F08);

//100%
INCLUDE_ASM("object/deadfadenode", func_00350F40__FPv);
#ifdef SKIP_ASM
void* func_00350F40(void* self)
{
    return func_0034FE90(self);
}
#endif

INCLUDE_ASM("object/deadfadenode", func_00350F60);

INCLUDE_ASM("object/deadfadenode", func_00350FD8);

//100%
INCLUDE_ASM("object/deadfadenode", func_00351010__FPv);
#ifdef SKIP_ASM
void* func_00351010(void* self)
{
    return func_0034FE90(self);
}
#endif

