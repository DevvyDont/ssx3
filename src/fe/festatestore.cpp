#include "common.h"

INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_costVisible);

INCLUDE_ASM("fe/festatestore", func_00184780);

INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_trickVisible);

INCLUDE_ASM("fe/festatestore", func_001849B0);

INCLUDE_ASM("fe/festatestore", func_00184B70);

//100%
INCLUDE_ASM("fe/festatestore", func_00184BB8);
#ifdef SKIP_ASM
extern "C" void cWScriptMan_checkGate(void* self);

extern "C" int func_00184BB8(void* self, int on)
{
    if (on != 0) {
        cWScriptMan_checkGate(self);
    }
    return 0;
}
#endif

INCLUDE_ASM("fe/festatestore", func_00184BE0);

INCLUDE_ASM("fe/festatestore", func_00184C60);

INCLUDE_ASM("fe/festatestore", func_00184F40);

INCLUDE_ASM("fe/festatestore", cFEStateUberTrick_onWidgetCreate);

INCLUDE_ASM("fe/festatestore", func_00185268);

//100%
INCLUDE_ASM("fe/festatestore", func_001859D8);
#ifdef SKIP_ASM
extern "C" int func_001859D8(void* self, int a1, unsigned int a2)
{
    if (a1 == *(int*)((char*)self + 0x50)) {
        switch (a2) {
        case 9:
            return 0x100;
        }
    } else {
        switch (a2) {
        case 8:
        case 9:
            return 0x100;
        }
    }
    return 0x101;
}
#endif

//100%
INCLUDE_ASM("fe/festatestore", func_00185A18);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void* self);
extern void* D_0046BDF8[];

extern "C" void* func_00185A18(void* self)
{
    func_0039E2A0(self);
    *(void***)((char*)self + 0x8) = D_0046BDF8;
    *(int*)((char*)self + 0x60) = -100;
    *(int*)((char*)self + 0x64) = 1;
    *(int*)((char*)self + 0xC) = 0x24;
    *(int*)((char*)self + 0x58) = 0;
    *(int*)((char*)self + 0x5C) = 0;
    *(int*)((char*)self + 0x48) = 0;
    return self;
}
#endif

extern void* D_0046D0D0[];
extern "C" void* func_0039E390(void*);

//100%
INCLUDE_ASM("fe/festatestore", func_00185A70__FPv);
#ifdef SKIP_ASM
void* func_00185A70(void* self)
{
    *(int*)((char*)self + 0x8) = (int)(void*)D_0046D0D0;
    return func_0039E390(self);
}
#endif

