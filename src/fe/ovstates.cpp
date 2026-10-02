#include "common.h"

INCLUDE_ASM("fe/ovstates", cFEStateTitle_onCreateScreen);

INCLUDE_ASM("fe/ovstates", func_001947F8);

INCLUDE_ASM("fe/ovstates", func_001948A8);

//100%
INCLUDE_ASM("fe/ovstates", func_00194980__FPv);
#ifdef SKIP_ASM
void func_00194980(void* self)
{
}
#endif

INCLUDE_ASM("fe/ovstates", func_00194988);

INCLUDE_ASM("fe/ovstates", func_001949E0);

//100%
INCLUDE_ASM("fe/ovstates", func_00194A48);
#ifdef SKIP_ASM
extern "C" int func_00194A48(void* self, int a1, int a2)
{
    return a2 != 6 ? 0x100 : 0x101;
}
#endif

INCLUDE_ASM("fe/ovstates", func_00194A60);

INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onCreateScreen);

INCLUDE_ASM("fe/ovstates", func_00194BF0);

INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onWidgetCreate);

INCLUDE_ASM("fe/ovstates", func_00194DD0);

INCLUDE_ASM("fe/ovstates", cFEStateMainMenu_onWidgetEvent);

//100%
INCLUDE_ASM("fe/ovstates", func_001952E8);
#ifdef SKIP_ASM
extern "C" int func_001952E8(void* self, int a1, int a2)
{
    switch (a2) {
    case 6:
        return *(int*)((char*)self + 0x48) == 0 ? 1 : 0x101;
    case 9:
        return 0x100;
    }
    return 0x101;
}
#endif

INCLUDE_ASM("fe/ovstates", func_00195328);

//100%
INCLUDE_ASM("fe/ovstates", func_00195498);
#ifdef SKIP_ASM
extern "C" void* func_0039E2A0(void*);
extern char D_0046B918[];

extern "C" void* func_00195498(void* self)
{
    func_0039E2A0(self);
    *(void**)((char*)self + 0x8) = D_0046B918;
    *(int*)((char*)self + 0xC) = 5;
    return self;
}
#endif

