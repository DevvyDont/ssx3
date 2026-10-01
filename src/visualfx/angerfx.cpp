#include "common.h"

INCLUDE_ASM("visualfx/angerfx", cAngerFX_cAngerFX);

INCLUDE_ASM("visualfx/angerfx", func_002D4A70);

INCLUDE_ASM("visualfx/angerfx", func_002D4BE0);

INCLUDE_ASM("visualfx/angerfx", func_002D4C08);

INCLUDE_ASM("visualfx/angerfx", func_002D5048);

INCLUDE_ASM("visualfx/angerfx", func_002D5598);

INCLUDE_ASM("visualfx/angerfx", func_002D55C0);

INCLUDE_ASM("visualfx/angerfx", func_002D5658);

INCLUDE_ASM("visualfx/angerfx", func_002D56B0);

INCLUDE_ASM("visualfx/angerfx", func_002D5718);

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D5750);
#ifdef SKIP_ASM
extern "C" int func_002D5750(const void* a, const void* b)
{
    unsigned short x = *(const unsigned short*)a;
    unsigned short y = *(const unsigned short*)b;
    if (x < y) {
        return -1;
    }
    return y < x;
}
#endif

