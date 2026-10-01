#include "common.h"

INCLUDE_ASM("visualfx/angerfx", cAngerFX_cAngerFX);

INCLUDE_ASM("visualfx/angerfx", func_002D4A70);

INCLUDE_ASM("visualfx/angerfx", func_002D4BE0);

INCLUDE_ASM("visualfx/angerfx", func_002D4C08);

INCLUDE_ASM("visualfx/angerfx", func_002D5048);

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D5598);
#ifdef SKIP_ASM
extern "C" void func_002D5598(unsigned char* rgb, float r, float g, float b)
{
    rgb[0] = (int)r;
    rgb[1] = (int)g;
    rgb[2] = (int)b;
}
#endif

INCLUDE_ASM("visualfx/angerfx", func_002D55C0);

INCLUDE_ASM("visualfx/angerfx", func_002D5658);

INCLUDE_ASM("visualfx/angerfx", func_002D56B0);

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D5718);
#ifdef SKIP_ASM
extern "C" int func_002D5718(const void* a, const void* b)
{
    float x = *(const float*)a;
    float y = *(const float*)b;
    if (x < y) {
        return -1;
    }
    if (y < x) {
        return 1;
    }
    return 0;
}
#endif

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

