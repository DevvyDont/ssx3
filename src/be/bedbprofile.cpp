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

/**
 * Returns a character's score record for a location and event.
 *
 * The index comes from cBELibrary_getScoreType(); the 26 eight-byte records start at +0xAD0.
 *
 * @param locID  the location (On Tour: eBELocationID)
 * @param event  the event or stat kind (On Tour: eBEGameEvent)
 * @return the record, or 0 when the pair has none (type 26)
 */
void* cBECharProfileDB_getScoreStats(void* self, int locID, int event)
{
    int type = cBELibrary_getScoreType(locID, event);
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
INCLUDE_ASM("be/bedbprofile", cBECharProfileDB_setLocationVisited);
#ifdef SKIP_ASM
/**
 * Marks a location as visited or not visited in a character profile.
 *
 * Sets or clears bit @p bit of the visited mask at +0xACC; the release build does not range-check it.
 *
 * @param bit  the location (On Tour eBELocationID locID)
 * @param on  On Tour visited: nonzero marks it visited
 */
extern "C" void cBECharProfileDB_setLocationVisited(void* self, int bit, int on)
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

extern "C" int func_00152460(sCharProfile_00152460* self, int locationId, int stat, unsigned int value)
{
    int type = cBELibrary_getScoreType(locationId, stat);
    if (type == 0x1A)
        return 0;
    if (value == 0)
        return 0;
    sScoreStat_00152460* scoreStat = &self->stats[type];
    if (scoreStat->value == 0 || IsBetter_00152460(stat, scoreStat, value))
    {
        scoreStat->value = value;
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/bedbprofile", func_00152528);
#ifdef SKIP_ASM
struct sPlaceStat_00152528
{
    unsigned char done;     // 0x0
    signed char best;       // 0x1
    unsigned short count;   // 0x2
    int pad4;
};

struct sMedals_00152528
{
    int a;
    int b;
};

struct sCharProfile_00152528
{
    char pad0[0xAD0];
    sPlaceStat_00152528 stats[0x1A];    // 0xAD0
    sMedals_00152528 medals[1];         // 0xBA0
};

struct sEventDef_00152528
{
    char pad0[0x54];
    int category;       // 0x54
    char pad58[0xC];
};
extern sEventDef_00152528 cBELibrary_locationDB[];

extern "C" int func_00152528(sCharProfile_00152528* self, int ev, int mode, int place)
{
    if (place != -1)
    {
        int type = cBELibrary_getScoreType(ev, mode);
        if (type != 0x1A)
        {
            sPlaceStat_00152528* placeStat = &self->stats[type];
            placeStat->done = 1;
            if ((unsigned int)place < 2)
                placeStat->count++;
            if (placeStat->best != -1)
            {
                if (place >= placeStat->best)
                    goto fail;
            }
            else
            {
                int cat = cBELibrary_locationDB[ev].category;
                if (mode == 0)
                    self->medals[cat].a++;
                else if ((unsigned int)(mode - 1) < 3)
                    self->medals[cat].b++;
            }
            placeStat->best = place;
            return 1;
        }
    }
fail:
    return 0;
}
#endif

