#include "common.h"

INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onCreateScreen);

INCLUDE_ASM("fe/festatecharselect", func_00181238);

INCLUDE_ASM("fe/festatecharselect", func_001812A8);

INCLUDE_ASM("fe/festatecharselect", func_00181308);

extern "C" void* func_0039E6B8(void* self);

//100%
INCLUDE_ASM("fe/festatecharselect", func_00181400__FPv);
#ifdef SKIP_ASM
void* func_00181400(void* self)
{
    return func_0039E6B8(self);
}
#endif

INCLUDE_ASM("fe/festatecharselect", func_00181420);

INCLUDE_ASM("fe/festatecharselect", func_00181450);

INCLUDE_ASM("fe/festatecharselect", cFEStateCharSelect_onWidgetCreate);

INCLUDE_ASM("fe/festatecharselect", func_001817E8);

INCLUDE_ASM("fe/festatecharselect", func_00181BD0);

INCLUDE_ASM("fe/festatecharselect", func_00181EF0);

INCLUDE_ASM("fe/festatecharselect", func_00182220);

INCLUDE_ASM("fe/festatecharselect", func_00182420);

INCLUDE_ASM("fe/festatecharselect", cFEStateCheatCharSelect_onWidgetCreate);

INCLUDE_ASM("fe/festatecharselect", func_00182690);

INCLUDE_ASM("fe/festatecharselect", func_00182808);

//100%
INCLUDE_ASM("fe/festatecharselect", func_00182870);
#ifdef SKIP_ASM
extern "C" void func_00182870(void* self)
{
    int i;
    int idx = *(unsigned char*)(*(char**)((char*)self + 0x6c) + 0x98);
    for (i = 0; i < 6; i++) {
        void* it = ((void**)((char*)self + 0x54))[i];
        if (it != 0) {
            int v = ((int*)((char*)self + 0xf4))[idx + i];
            *(int*)((char*)it + 0x78) = -1;
            *(int*)((char*)it + 0x7c) = v;
        }
    }
}
#endif

INCLUDE_ASM("fe/festatecharselect", func_001828C0);

