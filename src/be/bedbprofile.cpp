#include "common.h"
#include "be/bedbprofile.h"
#include "be/beintnewrace.h"
#include "be/beintreward.h"

extern "C" int cBELibrary_getScoreType(int a, int b);

//100%
INCLUDE_ASM("be/bedbprofile", cBECharProfileDB_getScoreStats__FPvii);
#ifdef SKIP_ASM


/**
 * Returns a character's score record for a location and event.
 *
 * The index comes from cBELibrary_getScoreType(); the 26 eight-byte records start at +0xAD0.
 *
 * @param locID  the location (On Tour: eBELocationID)
 * @param event  the event or stat kind (On Tour: eBEGameEvent)
 * @return the record, or 0 when the pair has none (kNoScoreType)
 */
void* cBECharProfileDB::getScoreStats(int locID, int event)
{
    int type = cBELibrary_getScoreType(locID, event);
    if (type == kNoScoreType) {
        return 0;
    }
    return &((&((cBECharProfileDB*)this)->mScoreStats[0]))[type];
}
#endif

//100%
INCLUDE_ASM("be/bedbprofile", cBECharProfileDB_hasVisitedPeak);
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
 * @return true when a location of that peak was visited
 */
bool cBECharProfileDB::hasVisitedPeak(int kind)
{
    sProfileMasks_001523E8 masks = D_0045A7C0;
    return ((int)mVisitLocFlags & masks.m[kind]) != 0;
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
void cBECharProfileDB::setLocationVisited(int bit, int on)
{
    int mask = 1 << bit;
    if (on)
        *(int*)&mVisitLocFlags |= mask;
    else
        *(int*)&mVisitLocFlags &= ~mask;
}
#endif

//100%
INCLUDE_ASM("be/bedbprofile", cBECharProfileDB_updateScoreStats);
#ifdef SKIP_ASM

struct sCharProfile_00152460
{
    char pad0[0xAD0];
    tBETrackScore stats[0x1A];  // 0xAD0
};

static inline int IsBetter_00152460(int stat, tBETrackScore* s, unsigned int value)
{
    switch (stat)
    {
    case 0:
    case 4:
    case 6:
    case 7:
    case 8:
        return value < s->uiHighScore;
    default:
        return s->uiHighScore < value;
    }
}

/**
 * Records @p value as the character's best for a location and event if it beats the stored one; returns true if so.
 *
 * The record is the 8-byte entry at +0xAD0 that cBELibrary_getScoreType(@p locationId, @p stat) selects (as in
 * cBECharProfileDB_getScoreStats); kNoScoreType (none) and @p value 0 return false. For @p stat 0, 4, 6, 7 and 8 lower
 * is better (times), for the rest higher (points); an empty record (0) always takes the value.
 *
 * @param locationId  the location
 * @param stat  the event kind, which also decides whether lower or higher is better
 * @param value  the new score or time
 * @return true when the record was updated
 */
bool cBECharProfileDB::updateScoreStats(int locationId, int stat, unsigned int value)
{
    int type = cBELibrary_getScoreType(locationId, stat);
    if (type == kNoScoreType)
        return false;
    if (value == 0)
        return false;
    tBETrackScore* scoreStat = &((&mScoreStats[0]))[type];
    if (scoreStat->uiHighScore == 0 || IsBetter_00152460(stat, scoreStat, value))
    {
        scoreStat->uiHighScore = value;
        return true;
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("be/bedbprofile", cBECharProfileDB_updateMedalStats);
#ifdef SKIP_ASM

struct sMedals_00152528
{
    int a;
    int b;
};

struct sCharProfile_00152528
{
    char pad0[0xAD0];
    tBETrackScore stats[0x1A];    // 0xAD0
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
 * Records finishing place @p place for an event in the character's profile; returns true when it is a new best.
 *
 * Nothing happens for @p place kMEDAL_NONE or a pair without a record (cBELibrary_getScoreType gives kNoScoreType).
 * Otherwise the record at +0xAD0 is marked done and its count incremented for places 0 and 1. A place not better than
 * the stored best returns false. The first place stored (best still kMEDAL_NONE) also bumps the medal counters of the
 * location's category (cBELibrary_locationDB[@p ev] +0x54): the first counter for @p mode 0, the second for modes 1-3.
 *
 * @param ev  the location
 * @param mode  the event kind
 * @param place  the finishing place / medal (eBEMedalID, 0 best), or kMEDAL_NONE for none
 * @return true when the best place was updated
 */
extern "C" bool cBECharProfileDB_updateMedalStats(sCharProfile_00152528* self, int ev, int mode, int place)
{
    if (place != kMEDAL_NONE)
    {
        int type = cBELibrary_getScoreType(ev, mode);
        if (type != kNoScoreType)
        {
            tBETrackScore* placeStat = &self->stats[type];
            placeStat->hasPlayed = 1;
            if ((unsigned int)place < 2)
                placeStat->iNumGoldOrPlat++;
            if (placeStat->BestMedal != kMEDAL_NONE)
            {
                if (place >= placeStat->BestMedal)
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
            placeStat->BestMedal = place;
            return true;
        }
    }
fail:
    return false;
}
#endif

