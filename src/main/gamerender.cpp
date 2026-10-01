#include "common.h"

INCLUDE_ASM("main/gamerender", cGameViewMan_cGameViewMan);

INCLUDE_ASM("main/gamerender", func_0022E550);

INCLUDE_ASM("main/gamerender", func_0022E730);

INCLUDE_ASM("main/gamerender", func_0022E7C8);

INCLUDE_ASM("main/gamerender", cGameViewMan_updateAll);

INCLUDE_ASM("main/gamerender", func_0022E8B8);

//100%
INCLUDE_ASM("main/gamerender", func_0022E920);
#ifdef SKIP_ASM
struct sRenderList {
    int field_0x0;
    void* items[3];
    unsigned int count;
};

extern "C" int func_0022E920(sRenderList* self)
{
    unsigned int i;
    for (i = 0; i < self->count; i++) {
        if (*(int*)((char*)self->items[i] + 0xb4) == 0) {
            return 0;
        }
    }
    return 1;
}
#endif

INCLUDE_ASM("main/gamerender", func_0022E968);

