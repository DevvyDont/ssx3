#include "common.h"

INCLUDE_ASM("visualfx/angerfx", cAngerFX_cAngerFX);

INCLUDE_ASM("visualfx/angerfx", func_002D4A70);

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D4BE0);
#ifdef SKIP_ASM
extern "C" void* func_003E6448(void* dst, int c, int n);

extern "C" void func_002D4BE0(void* self)
{
    func_003E6448(*(void**)((char*)self + 4), 0, 0x58);
}
#endif

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

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D5658);
#ifdef SKIP_ASM
extern "C" void func_002D5658(unsigned char* p, float r, float g, float b, float a)
{
    p[6] = (int)(r * 127.0f);
    p[7] = (int)(g * 127.0f);
    p[8] = (int)(b * 127.0f);
    p[9] = (int)(a * 255.0f);
}
#endif

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

