#include "common.h"

INCLUDE_ASM("seg/seg_CF758", setChallengeName);

INCLUDE_ASM("seg/seg_CF758", setChallengeStats);

//100%
INCLUDE_ASM("seg/seg_CF758", func_001CED90);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void* cBE_getBE();
void* cBE_getInterface_ED90(void* be, int kind) __asm__("cBE_getInterface__Fv");
signed char cBERewardInterface_getTrackMedal(void* self, int a, int b, int track, int kind);
extern "C" int cBERewardInterface_getEarningsMedal(void* self, int a, int b, int kind);
extern "C" int func_001589B0(void* self, int a, int b, int kind);
extern "C" int func_00158A50(void* self, int a, int b, int kind);
struct sReq_ED90
{
    signed char track;
    signed char kind;
};
extern sReq_ED90 D_0045AAD8_ED90[][4][5] __asm__("D_0045AAD8");

extern "C" void func_001CED90(int arg0, int arg1, int arg2, int arg3, int arg4)
{
    void* r = cBE_getInterface_ED90(cBE_getBE(), 0xD);
    if (arg3 == 2)
    {
        if (arg4 == 0)
        {
            func_001589B0(r, arg0, arg1, arg2);
            return;
        }
        func_00158A50(r, arg0, arg1, arg2);
        return;
    }
    if (arg3 != 3)
    {
        int kind = D_0045AAD8_ED90[arg2][arg3][arg4].kind;
        int track = D_0045AAD8_ED90[arg2][arg3][arg4].track;
        cBERewardInterface_getTrackMedal(r, arg0, arg1, track, kind);
        return;
    }
    cBERewardInterface_getEarningsMedal(r, arg0, arg1, arg2);
}
#endif

INCLUDE_ASM("seg/seg_CF758", func_001CEEA0);
