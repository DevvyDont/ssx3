#include "common.h"

INCLUDE_ASM("sound/ps2soundman", cSndPlayOpts_cSndPlayOpts);

INCLUDE_ASM("sound/ps2soundman", func_002A97C0);

//100%
INCLUDE_ASM("sound/ps2soundman", func_002A9988);
#ifdef SKIP_ASM
extern "C" void func_002A9988(void* self, float a, float b)
{
    (*(float**)((char*)self + 0x6C))[*(int*)((char*)self + 0x1C)] = a;
    if (b < 0.0f) {
        (*(float**)((char*)self + 0x68))[*(int*)((char*)self + 0x1C)] = a;
        return;
    }
    (*(float**)((char*)self + 0x68))[*(int*)((char*)self + 0x1C)] = b;
}
#endif

