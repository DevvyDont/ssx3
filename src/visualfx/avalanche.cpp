#include "common.h"

INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_getFrameData);

INCLUDE_ASM("visualfx/avalanche", func_002D6378);

INCLUDE_ASM("visualfx/avalanche", func_002D6410);

INCLUDE_ASM("visualfx/avalanche", func_002D64D8);

INCLUDE_ASM("visualfx/avalanche", tAvalancheNode_calculate);

//100%
INCLUDE_ASM("visualfx/avalanche", tActiveAvalancheNode_calculateScale);
#ifdef SKIP_ASM
struct sAvalancheData {
    char pad[0xF0];
    unsigned short type;
    char padF2[0xE];
    float f100;
    float f104;
    float f108;
};

extern "C" void tActiveAvalancheNode_calculateScale(void* self, float t)
{
    sAvalancheData* d = *(sAvalancheData**)((char*)self + 0x2E0);
    float x = t * 30.0f;
    if (x < d->f104) {
        *(float*)((char*)self + 0xB0) = 1.0f;
        *(float*)((char*)self + 0xB4) = x / d->f104;
        return;
    }
    if (x < d->f100 - d->f108) {
        *(float*)((char*)self + 0xB4) = 1.0f;
        *(float*)((char*)self + 0xB0) = 1.0f;
        return;
    }
    if (d->type != 2) {
        float v = (d->f100 - x) / d->f108;
        *(float*)((char*)self + 0xB4) = v;
        *(float*)((char*)self + 0xB0) = v;
        if (v > 0.0f) {
            // PORT: sqrt.s (sqrtf without errno check)
            __asm__("sqrt.s %0, %1" : "=f"(v) : "f"(v));
            *(float*)((char*)self + 0xB0) = v;
        }
    }
}
#endif

INCLUDE_ASM("visualfx/avalanche", func_002D7CA8);

INCLUDE_ASM("visualfx/avalanche", func_002D7DD8);

INCLUDE_ASM("visualfx/avalanche", tActiveAvalanche_buildArray);

INCLUDE_ASM("visualfx/avalanche", func_002D7EF8);

INCLUDE_ASM("visualfx/avalanche", func_002D81B0);

INCLUDE_ASM("visualfx/avalanche", func_002D8258);

INCLUDE_ASM("visualfx/avalanche", func_002D82A0);

INCLUDE_ASM("visualfx/avalanche", func_002D83B8);

INCLUDE_ASM("visualfx/avalanche", func_002D87D0);

INCLUDE_ASM("visualfx/avalanche", func_002D8948);

INCLUDE_ASM("visualfx/avalanche", func_002D8A00);

INCLUDE_ASM("visualfx/avalanche", func_002D8EA8);

INCLUDE_ASM("visualfx/avalanche", func_002D9130);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_addAvalancheNode);

INCLUDE_ASM("visualfx/avalanche", func_002D9538);

INCLUDE_ASM("visualfx/avalanche", func_002D9660);

INCLUDE_ASM("visualfx/avalanche", func_002D96E0);

INCLUDE_ASM("visualfx/avalanche", func_002D9738);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_triggerAvalanche);

INCLUDE_ASM("visualfx/avalanche", func_002D9A80);

INCLUDE_ASM("visualfx/avalanche", func_002D9B40);

INCLUDE_ASM("visualfx/avalanche", func_002D9C00);

INCLUDE_ASM("visualfx/avalanche", func_002D9CB0);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_readFromReplayFrame);

INCLUDE_ASM("visualfx/avalanche", func_002D9FB8);

INCLUDE_ASM("visualfx/avalanche", cAvalanche_resolveDataPointers);

INCLUDE_ASM("visualfx/avalanche", func_002DA1C0);

