#include "common.h"

INCLUDE_ASM("ai/airpredictor", cAirPredictor_reset);

INCLUDE_ASM("ai/airpredictor", func_00113200);

INCLUDE_ASM("ai/airpredictor", cAirPredictor_startLaunchIntoAir);

// 16-byte aligned: the original copies these with lq/sq, which require it
// (PS2 VU-style quadword).
struct sQuad {
    int x[4];
} __attribute__((aligned(16)));

//100%
INCLUDE_ASM("ai/airpredictor", cAirPredictor_initLaunch__FPcP5sQuadT1);
#ifdef SKIP_ASM
void cAirPredictor_initLaunch(char* self, sQuad* a, sQuad* b)
{
    *(int*)(self + 0xAC) = 0;
    *(sQuad*)(self + 0x70) = *a;
    *(sQuad*)(self + 0x80) = *b;
    *(sQuad*)(self + 0x50) = *a;
    *(sQuad*)(self + 0x60) = *b;
    *(float*)(self + 0x98) = *(float*)(self + 0xA0);
}
#endif

INCLUDE_ASM("ai/airpredictor", func_00113648);

//100%
INCLUDE_ASM("ai/airpredictor", func_00113998__FPv);
#ifdef SKIP_ASM
void func_00113998(void* self)
{
    *(int*)((char*)self + 0x98) = 0;
}
#endif

INCLUDE_ASM("ai/airpredictor", func_001139A0);

INCLUDE_ASM("ai/airpredictor", func_00113AA0);

INCLUDE_ASM("ai/airpredictor", func_00113AD8);

