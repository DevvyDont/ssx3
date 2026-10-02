#include "common.h"

extern "C" int cBELibrary_getScoreType(int a, int b);

//100%
INCLUDE_ASM("be/bedbprofile", cBECharProfileDB_getScoreStats__FPvii);
#ifdef SKIP_ASM
struct sScoreStat_00152398
{
    int a;
    int b;
};

struct sCharProfile_00152398
{
    char pad0[0xAD0];
    sScoreStat_00152398 stats[0x1A];  // 0xAD0
};

void* cBECharProfileDB_getScoreStats(void* self, int a, int b)
{
    int type = cBELibrary_getScoreType(a, b);
    if (type == 0x1A) {
        return 0;
    }
    return &((sCharProfile_00152398*)self)->stats[type];
}
#endif

//100%
INCLUDE_ASM("be/bedbprofile", func_001523E8);
#ifdef SKIP_ASM
// Local mask table; the original was likely `int masks[3] = { 0x64923, 0x18924C, 0x21A490 };`
// whose compiler-generated literal splat names D_0045A7C0.
struct sProfileMasks_001523E8
{
    int m[3];
};
extern const sProfileMasks_001523E8 D_0045A7C0;

extern "C" int func_001523E8(void* self, int kind)
{
    sProfileMasks_001523E8 masks = D_0045A7C0;
    return (*(int*)((char*)self + 0xACC) & masks.m[kind]) != 0;
}
#endif

//100%
INCLUDE_ASM("be/bedbprofile", func_00152430);
#ifdef SKIP_ASM
extern "C" void func_00152430(void* self, int bit, int on)
{
    int mask = 1 << bit;
    if (on)
        *(int*)((char*)self + 0xACC) |= mask;
    else
        *(int*)((char*)self + 0xACC) &= ~mask;
}
#endif

//100%
INCLUDE_ASM("be/bedbprofile", func_00152460);
#ifdef SKIP_ASM
struct sScoreStat_00152460
{
    int a;
    unsigned int value;
};

struct sCharProfile_00152460
{
    char pad0[0xAD0];
    sScoreStat_00152460 stats[0x1A];  // 0xAD0
};

static inline int IsBetter_00152460(int stat, sScoreStat_00152460* s, unsigned int value)
{
    switch (stat)
    {
    case 0:
    case 4:
    case 6:
    case 7:
    case 8:
        return value < s->value;
    default:
        return s->value < value;
    }
}

extern "C" int func_00152460(sCharProfile_00152460* self, int a, int stat, unsigned int value)
{
    int type = cBELibrary_getScoreType(a, stat);
    if (type == 0x1A)
        return 0;
    if (value == 0)
        return 0;
    sScoreStat_00152460* s = &self->stats[type];
    if (s->value == 0 || IsBetter_00152460(stat, s, value))
    {
        s->value = value;
        return 1;
    }
    return 0;
}
#endif

INCLUDE_ASM("be/bedbprofile", func_00152528);

