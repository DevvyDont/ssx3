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

//100%
INCLUDE_ASM("visualfx/angerfx", func_002D56B0);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void func_003715B0(void* p, int a1);
extern "C" void func_003714F8(void* p, int a1);

extern "C" void func_002D56B0(int* self, int flags)
{
    func_003715B0((char*)self + 0xD0, 1);
    func_003714F8((char*)self + 0xD0, 2);
    if (flags & 1) {
        operator_delete(self);
    }
}
#endif

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

