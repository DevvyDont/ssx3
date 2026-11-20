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
    if (type == 26) {
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

/**
 * Returns whether the character has visited any location of peak @p kind (0-2).
 *
 * Tests the visited-location bits at +0xACC (cBECharProfileDB_setLocationVisited) against the peak's mask: 0x64923,
 * 0x18924C or 0x21A490 (from D_0045A7C0). No range check on @p kind.
 *
 * @param kind  the peak, 0-2
 * @return 1 when a location of that peak was visited, else 0
 */
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

/**
 * Records @p value as the character's best for a location and event if it beats the stored one; returns 1 if so.
 *
 * The record is the 8-byte entry at +0xAD0 that cBELibrary_getScoreType(@p locationId, @p stat) selects (as in
 * cBECharProfileDB_getScoreStats); type 26 (none) and @p value 0 return 0. For @p stat 0, 4, 6, 7 and 8 lower is
 * better (times), for the rest higher (points); an empty record (0) always takes the value.
 *
 * @param locationId  the location
 * @param stat  the event kind, which also decides whether lower or higher is better
 * @param value  the new score or time
 * @return 1 when the record was updated, else 0
 */
extern "C" int func_00152460(sCharProfile_00152460* self, int locationId, int stat, unsigned int value)
{
    int type = cBELibrary_getScoreType(locationId, stat);
    if (type == 26)
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

/**
 * Records finishing place @p place for an event in the character's profile; returns 1 when it is a new best.
 *
 * Nothing happens for @p place -1 or a pair without a record (cBELibrary_getScoreType gives 26). Otherwise the record
 * at +0xAD0 is marked done and its count incremented for places 0 and 1. A place not better than the stored best
 * returns 0. The first place stored (best still -1) also bumps the medal counters of the location's category
 * (cBELibrary_locationDB[@p ev] +0x54): the first counter for @p mode 0, the second for modes 1-3.
 *
 * @param ev  the location
 * @param mode  the event kind
 * @param place  the finishing place / medal (0 best), or -1 for none
 * @return 1 when the best place was updated, else 0
 */
extern "C" int func_00152528(sCharProfile_00152528* self, int ev, int mode, int place)
{
    if (place != -1)
    {
        int type = cBELibrary_getScoreType(ev, mode);
        if (type != 26)
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

