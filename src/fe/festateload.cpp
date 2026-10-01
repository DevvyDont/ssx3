#include "common.h"

INCLUDE_ASM("fe/festateload", cFEStateEventSelect_onCreateScreen);

extern "C" void* func_002009D0(void*);

//100%
INCLUDE_ASM("fe/festateload", func_00186708__FPv);
#ifdef SKIP_ASM
void* func_00186708(void* self)
{
    return func_002009D0((char*)self + 0x48);
}
#endif

extern "C" void* func_00200A70(void*);

//100%
INCLUDE_ASM("fe/festateload", func_00186728__FPv);
#ifdef SKIP_ASM
void* func_00186728(void* self)
{
    return func_00200A70((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("fe/festateload", func_00186748);
#ifdef SKIP_ASM
extern "C" void* func_0039E4C0(void* self);
extern "C" void func_00200AC0(void* tmpl);

extern "C" void func_00186748(void* self)
{
    func_0039E4C0(self);
    func_00200AC0((char*)self + 0x48);
}
#endif

//100%
INCLUDE_ASM("fe/festateload", func_00186778);
#ifdef SKIP_ASM
void* func_0039E4A0(void* self);
extern "C" void func_00200AF0(void* tmpl);

extern "C" void func_00186778(void* self)
{
    func_0039E4A0(self);
    func_00200AF0((char*)self + 0x48);
}
#endif

INCLUDE_ASM("fe/festateload", func_001867A8);

//100%
INCLUDE_ASM("fe/festateload", func_00186950);
#ifdef SKIP_ASM
extern "C" int func_00202738(void* self, int a1, int a2);

extern "C" int func_00186950(void* self, int a1, int a2)
{
    if (func_00202738((char*)self + 0x48, a1, a2) != 0) {
        return 0x101;
    }
    return 0;
}
#endif

void func_00202768(void*);

//100%
INCLUDE_ASM("fe/festateload", func_00186978);
#ifdef SKIP_ASM
extern "C" void func_00186978(void* self)
{
    func_00202768((char*)self + 0x48);
}
#endif

INCLUDE_ASM("fe/festateload", func_00186998);

//100%
INCLUDE_ASM("fe/festateload", func_001869D8);
#ifdef SKIP_ASM
extern "C" void* func_0039E510(void* self);
extern "C" void cUITemplate_MAP_onUpdate(void* tmpl);

extern "C" void func_001869D8(void* self)
{
    func_0039E510(self);
    cUITemplate_MAP_onUpdate((char*)self + 0x48);
}
#endif

INCLUDE_ASM("fe/festateload", func_00186A08);

INCLUDE_ASM("fe/festateload", func_00186B18);

//100%
INCLUDE_ASM("fe/festateload", func_00186B48__FPv);
#ifdef SKIP_ASM
int func_00186B48(void* self)
{
    return 0x100;
}
#endif

