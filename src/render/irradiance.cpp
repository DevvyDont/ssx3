#include "common.h"

INCLUDE_ASM("render/irradiance", cIrradianceDataBase_Load);

//100%
INCLUDE_ASM("render/irradiance", func_0038ADB0);
#ifdef SKIP_ASM
void cMemMan_free(void*);

extern "C" void func_0038ADB0(void* self)
{
    if (*(int*)((char*)self + 0x4) != 0) {
        if (*(void**)((char*)self + 0xC) != 0) {
            cMemMan_free(*(void**)((char*)self + 0xC));
        }
        if (*(void**)((char*)self + 0x8) != 0) {
            cMemMan_free(*(void**)((char*)self + 0x8));
        }
        *(void**)((char*)self + 0xC) = 0;
        *(void**)((char*)self + 0x8) = 0;
        *(int*)((char*)self + 0x4) = 0;
    }
}
#endif

