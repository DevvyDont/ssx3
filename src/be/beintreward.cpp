#include "common.h"
#include "be/beuserawards.h"
#include "be/berewarddb.h"
#include "be/beintreward.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BERewardInt[];
extern void* cBEInterface_vtbl[16];


//99.84%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the reward interface singleton, creating it on first use.
 *
 * The first call allocates 0x18 bytes tagged "BERewardInt", clears bIsActive, stores cBEInterface's vtable
 * cBEInterface_vtbl and keeps the object in cBERewardInterface_instance.
 *
 * @return the reward interface
 */
void* cBERewardInterface_getThis()
{
    if (cBERewardInterface_instance == 0) {
        cBERewardInterface* mem = (cBERewardInterface*)cMemMan_alloc(0x18, str_BERewardInt, 0, 0);
        mem->bIsActive = 0;
        mem->mVtbl = cBEInterface_vtbl;
        cBERewardInterface_instance = mem;
    }
    return cBERewardInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_loadRewards);
#ifdef SKIP_ASM
extern "C" void cBERewardDB_init(void* db, int flag);
extern void* cBELibrary_rewardDB[];

/**
 * Starts loading the reward database asynchronously: cBERewardDB_init(cBELibrary_rewardDB, 1).
 *
 * The rewards screen calls it when cBERewardInterface_isRewardsLoaded() reports the database not ready.
 */
extern "C" void cBERewardInterface_loadRewards()
{
    cBERewardDB_init(cBELibrary_rewardDB, 1);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isRewardsLoaded);
#ifdef SKIP_ASM
// PORT: compares a pointer cast to int against -1; not 64-bit safe.
extern void* cBELibrary_rewardDB[];

/**
 * Returns whether the reward database has finished loading.
 *
 * True when the first word of cBELibrary_rewardDB (On Tour cBERewardDB.mRewardFileIndex, the load request id) is -1,
 * which cBERewardDB_init stores after a synchronous load, and the second (mpRewardsFile) is non-null.
 *
 * @return 1 when loaded, else 0
 */
extern "C" int cBERewardInterface_isRewardsLoaded(void)
{
    int result = 0;
    if ((int)cBELibrary_rewardDB[0] == -1) {
        result = cBELibrary_rewardDB[1] != 0;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isBetterMedal__FPvii);
#ifdef SKIP_ASM
/**
 * Tells whether medal @p newMedal is better than medal @p oldMedal.
 *
 * Medal ids run from 0 (best) to 3 (lowest), and -1 means no medal: no medal is never better, and any medal is
 * better than none.
 *
 * @param oldMedal  medal to compare against, or -1
 * @param newMedal  medal to test, or -1
 * @return 1 when @p newMedal is better than @p oldMedal, else 0
 */
int cBERewardInterface_isBetterMedal(void* self, int oldMedal, int newMedal)
{
    if (newMedal == -1) {
        return 0;
    }
    if (oldMedal == -1) {
        return 1;
    }
    return newMedal < oldMedal;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrophy);
#ifdef SKIP_ASM
extern "C" void* cBERewardInterface_getDB(cBERewardInterface* self);

/**
 * Returns the trophy record for @p trophyId on @p peakId: the 12-byte entry trophyId * 3 + peakId of the reward
 * database's trophy table (cBERewardInterface_getDB +0x8, On Tour's mpTrophies).
 *
 * Its first word is the trophy name string (the trophy room prints it), the second a sound id.
 *
 * @param peakId  the peak, 0-2
 * @param trophyId  the trophy
 * @return the trophy record
 */
extern "C" char* cBERewardInterface_getTrophy(cBERewardInterface* self, int peakId, int trophyId)
{
    return *(char**)((char*)cBERewardInterface_getDB(self) + 0x8) + (trophyId * 3 + peakId) * 0xC;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getMedal);
#ifdef SKIP_ASM
extern "C" void* cBERewardInterface_getDB(cBERewardInterface* self);

/**
 * Returns the medal record for @p trophyId and @p medalId: the 12-byte entry trophyId * 4 + medalId of the reward
 * database's medal table (cBERewardInterface_getDB +0xC, On Tour's mpMedals).
 *
 * @param trophyId  first index (four medals per entry)
 * @param medalId  medal within it
 * @return the medal record
 */
extern "C" char* cBERewardInterface_getMedal(cBERewardInterface* self, int trophyId, int medalId)
{
    return *(char**)((char*)cBERewardInterface_getDB(self) + 0xC) + (trophyId * 4 + medalId) * 0xC;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getPeakPass);
#ifdef SKIP_ASM
extern "C" void* cBERewardInterface_getDB(cBERewardInterface* self);

/**
 * Returns the peak pass record for @p charId and @p peakId: the 8-byte entry charId * 3 + peakId of the reward
 * database's peak pass table (cBERewardInterface_getDB +0x10, On Tour's mpPeakPasses).
 *
 * @param peakId  the peak, 0-2
 * @param charId  the character
 * @return the peak pass record
 */
extern "C" char* cBERewardInterface_getPeakPass(cBERewardInterface* self, int peakId, int charId)
{
    return *(char**)((char*)cBERewardInterface_getDB(self) + 0x10) + (charId * 3 + peakId) * 8;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrophies);
#ifdef SKIP_ASM
/**
 * Returns the reward database's trophy table (cBERewardInterface_getDB +0x8, On Tour's mpTrophies).
 *
 * @param rewardInterface  the reward interface
 * @return the trophy records
 */
extern "C" int cBERewardInterface_getTrophies(cBERewardInterface* rewardInterface) {
    return (*(int *)((char*)(cBERewardInterface_getDB(rewardInterface)) + (8)));
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getMedals);
#ifdef SKIP_ASM
/**
 * Returns the reward database's medal table (cBERewardInterface_getDB +0xC, On Tour's mpMedals).
 *
 * @param rewardInterface  the reward interface
 * @return the medal records
 */
extern "C" int cBERewardInterface_getMedals(cBERewardInterface* rewardInterface) {
    return (*(int *)((char*)(cBERewardInterface_getDB(rewardInterface)) + (0xC)));
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getPeakPasses);
#ifdef SKIP_ASM
/**
 * Returns the reward database's peak pass table (cBERewardInterface_getDB +0x10, On Tour's mpPeakPasses).
 *
 * @param rewardInterface  the reward interface
 * @return the peak pass records
 */
extern "C" int cBERewardInterface_getPeakPasses(cBERewardInterface* rewardInterface) {
    return (*(int *)((char*)(cBERewardInterface_getDB(rewardInterface)) + (0x10)));
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getPosters);
#ifdef SKIP_ASM
/**
 * Returns the reward database's poster table (cBERewardInterface_getDB +0x14, On Tour's mpPosters).
 *
 * @param rewardInterface  the reward interface
 * @return the poster records
 */
extern "C" int cBERewardInterface_getPosters(cBERewardInterface* rewardInterface) {
    return (*(int *)((char*)(cBERewardInterface_getDB(rewardInterface)) + (0x14)));
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTradingCards);
#ifdef SKIP_ASM
/**
 * Returns the reward database's trading card table (cBERewardInterface_getDB +0x18, On Tour's mpTradingCards).
 *
 * @param rewardInterface  the reward interface
 * @return the trading card records
 */
extern "C" int cBERewardInterface_getTradingCards(cBERewardInterface* rewardInterface) {
    return (*(int *)((char*)(cBERewardInterface_getDB(rewardInterface)) + (0x18)));
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getConceptArts);
#ifdef SKIP_ASM
/**
 * Returns the reward database's concept-art table (cBERewardInterface_getDB +0x1C, On Tour's mpConceptArts).
 *
 * @param rewardInterface  the reward interface
 * @return the concept-art records
 */
extern "C" int cBERewardInterface_getConceptArts(cBERewardInterface* rewardInterface) {
    return (*(int *)((char*)(cBERewardInterface_getDB(rewardInterface)) + (0x1C)));
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getToys);
#ifdef SKIP_ASM
/**
 * Returns the reward database's toy table (cBERewardInterface_getDB +0x20, On Tour's mpToys).
 *
 * @param rewardInterface  the reward interface
 * @return the toy records
 */
extern "C" int cBERewardInterface_getToys(cBERewardInterface* rewardInterface) {
    return (*(int *)((char*)(cBERewardInterface_getDB(rewardInterface)) + (0x20)));
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getVideos);
#ifdef SKIP_ASM
/**
 * Returns the reward database's video table (cBERewardInterface_getDB +0x24, On Tour's mpVideos).
 *
 * @param rewardInterface  the reward interface
 * @return the video records
 */
extern "C" int cBERewardInterface_getVideos(cBERewardInterface* rewardInterface) {
    return (*(int *)((char*)(cBERewardInterface_getDB(rewardInterface)) + (0x24)));
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00156C70);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
struct sBELibTables;
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(sBELibTables* self, int i);
unsigned int BXrand();

struct sBEFlagEntry_6C70 {
    unsigned short field_0x0;
    unsigned short flags;
};

struct sBEEntry_6C70 {
    char pad_0x0[4];
    short id;           // 0x4
    char pad_0x6[0x2E];
    int flags;          // 0x34
};

static inline sBEFlagEntry_6C70* Flag_6C70(char* p, sBEEntry_6C70* e)
{
    short s = (*(short**)(p + 0x288))[e->id];
    if (s >= 0)
        return (sBEFlagEntry_6C70*)(p + 0x290) + s;
    return 0;
}

static inline int Match_6C70(sBEEntry_6C70* e, int type)
{
    return (type == 0 && (e->flags & 0x100)) || (type == 1 && (e->flags & 0x200)) || (type == 2 && (e->flags & 0x400));
}

/**
 * Picks a random item of one type that the character does not have yet from the character's item-database entries, or
 * returns -1.
 *
 * Walks the character's entries in the item database (func_0014BDB8; count func_0014D988, array func_0014D998, 0x38
 * bytes each). Type 0 takes entries whose flags word (+0x34) has 0x100 set, type 1 0x200, type 2 0x400. An entry counts
 * when its per-character state, found through the index table at +0x288 of the character record (cBELibrary_profileLib
 * + profile * 0x9B50 + character * 0xF88) into the 4-byte states at +0x290, has bit 0x2 clear. Picks one of the n
 * candidates with BXrand() % n and returns its id (+0x4).
 *
 * cBERewardInterface_awardUser uses it for award categories 17, 18 and 19 (types 0, 1, 2). The first pass tests byte
 * +0x35 bit 0, the same bit as flags 0x100. The second pass dereferences the state pointer without the null check the
 * first pass has.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param type  item type, 0 to 2 (flags 0x100, 0x200, 0x400)
 * @return the chosen item id, or -1 when there is none
 */
extern "C" int func_00156C70(void* self, int profileIndex, int characterId, int type)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    int n = 0;
    void* lib = func_0014BDB8_noarg();
    int count = func_0014D988(lib, characterId);
    sBEEntry_6C70* def = (sBEEntry_6C70*)func_0014D998((sBELibTables*)lib, characterId);
    int i;
    for (i = 0; i < count; i++, def++)
    {
        if (type == 0 && (*(unsigned char*)((char*)def + 0x35) & 1))
        {
            if ((Flag_6C70(charRecord, def)->flags & 2) == 0)
                n++;
        }
        else if (type == 1 && (def->flags & 0x200))
        {
            if ((Flag_6C70(charRecord, def)->flags & 2) == 0)
                n++;
        }
        else if (type == 2 && (def->flags & 0x400))
        {
            if ((Flag_6C70(charRecord, def)->flags & 2) == 0)
                n++;
        }
    }
    if (n > 0)
    {
        n = BXrand() % n;
        def = (sBEEntry_6C70*)func_0014D998((sBELibTables*)lib, characterId);
        int j;
        for (j = 0; j < count; j++, def++)
        {
            if (type == 0 && (*(unsigned char*)((char*)def + 0x35) & 1))
            {
                short id = def->id;
                short flagSlot = (*(short**)(charRecord + 0x288))[id];
                sBEFlagEntry_6C70* flagEntry;
                if (flagSlot >= 0)
                    flagEntry = (sBEFlagEntry_6C70*)(charRecord + 0x290) + flagSlot;
                else
                    flagEntry = 0;
                if ((flagEntry->flags & 2) == 0)
                {
                    if (--n < 0)
                        return id;
                }
            }
            else if (type == 1 && (def->flags & 0x200))
            {
                short id = def->id;
                short flagSlot = (*(short**)(charRecord + 0x288))[id];
                sBEFlagEntry_6C70* flagEntry;
                if (flagSlot >= 0)
                    flagEntry = (sBEFlagEntry_6C70*)(charRecord + 0x290) + flagSlot;
                else
                    flagEntry = 0;
                if ((flagEntry->flags & 2) == 0)
                {
                    if (--n < 0)
                        return id;
                }
            }
            else if (type == 2 && (def->flags & 0x400))
            {
                short id = def->id;
                short flagSlot = (*(short**)(charRecord + 0x288))[id];
                sBEFlagEntry_6C70* flagEntry;
                if (flagSlot >= 0)
                    flagEntry = (sBEFlagEntry_6C70*)(charRecord + 0x290) + flagSlot;
                else
                    flagEntry = 0;
                if ((flagEntry->flags & 2) == 0)
                {
                    if (--n < 0)
                        return id;
                }
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00156EE0);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
struct sBELibTables;
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(sBELibTables* self, int i);
unsigned int BXrand();

struct sBEFlagEntry_6EE0 {
    unsigned short field_0x0;
    unsigned short flags;
};

struct sBEEntry_6EE0 {
    char pad_0x0[4];
    short id;           // 0x4
    char pad_0x6[0x2E];
    int flags;          // 0x34
};

static inline sBEFlagEntry_6EE0* Flag_6EE0(char* p, sBEEntry_6EE0* e)
{
    short s = (*(short**)(p + 0x288))[e->id];
    if (s >= 0)
        return (sBEFlagEntry_6EE0*)(p + 0x290) + s;
    return 0;
}

/**
 * Picks a random item with flag 0x800 that the character does not have yet from the character's item-database entries,
 * or returns -1.
 *
 * Same walk as func_00156C70 with the single test flags (+0x34) & 0x800: candidates are entries whose per-character
 * state (index table at +0x288 of the character record (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88),
 * states at +0x290) has bit 0x2 clear; one is chosen with BXrand() % n. cBERewardInterface_awardUser uses it for award
 * categories 20 to 31.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the chosen item id, or -1 when there is none
 */
extern "C" int func_00156EE0(void* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    int n = 0;
    void* lib = func_0014BDB8_noarg();
    int count = func_0014D988(lib, characterId);
    sBEEntry_6EE0* def = (sBEEntry_6EE0*)func_0014D998((sBELibTables*)lib, characterId);
    int i;
    for (i = 0; i < count; i++, def++)
    {
        if (def->flags & 0x800)
        {
            if ((Flag_6EE0(charRecord, def)->flags & 2) == 0)
                n++;
        }
    }
    if (n > 0)
    {
        n = BXrand() % n;
        def = (sBEEntry_6EE0*)func_0014D998((sBELibTables*)lib, characterId);
        int j;
        for (j = 0; j < count; j++, def++)
        {
            if (def->flags & 0x800)
            {
                short id = def->id;
                short flagSlot = (*(short**)(charRecord + 0x288))[id];
                sBEFlagEntry_6EE0* flagEntry;
                if (flagSlot >= 0)
                    flagEntry = (sBEFlagEntry_6EE0*)(charRecord + 0x290) + flagSlot;
                else
                    flagEntry = 0;
                if ((flagEntry->flags & 2) == 0)
                {
                    if (--n < 0)
                        return id;
                }
            }
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getRandomLockedRewardIndex);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern int cBELibrary_profileLib[];
unsigned int BXrand();
extern "C" int cBERewardInterface_getTotalNumRewards(cBERewardInterface* self, int kind);
extern "C" int cBERewardInterface_getNumOwnedRewards(cBERewardInterface* self, int a1, int a2, int kind);

/**
 * Picks a random reward of one kind that the character does not own yet.
 *
 * Returns -1 when the owned count (cBERewardInterface_getNumOwnedRewards) has reached the total (cBERewardInterface_getTotalNumRewards). Otherwise takes n = BXrand() %
 * (total - owned) and returns the index of the n-th clear bit in the kind's ownership bits in the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88): +0xF30 posters (kind 3), +0xF36 trading cards (4),
 * +0xF45 concept arts (5), +0xF52 videos (6), +0xF53 toys (7). Other kinds leave the bit pointer null.
 * cBERewardInterface_awardUser gives the result with the matching buy function.
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param kind  reward kind, 3 to 7
 * @return the reward index, or -1 when every reward of the kind is owned
 */
extern "C" int cBERewardInterface_getRandomLockedRewardIndex(cBERewardInterface* self, int profile, int characterId, int kind)
{
    int total = cBERewardInterface_getTotalNumRewards(self, kind);
    int result = -1;
    int have = cBERewardInterface_getNumOwnedRewards(self, profile, characterId, kind);
    if (have >= total) {
        return -1;
    }
    unsigned int n = BXrand() % (unsigned int)(total - have);
    char* charRecord = (char*)cBELibrary_profileLib + profile * 0x9B50 + characterId * 3976;
    signed char* bits = 0;
    switch (kind) {
    case 3: bits = (signed char*)(charRecord + 0xF30); break;
    case 4: bits = (signed char*)(charRecord + 0xF36); break;
    case 5: bits = (signed char*)(charRecord + 0xF45); break;
    case 6: bits = (signed char*)(charRecord + 0xF52); break;
    case 7: bits = (signed char*)(charRecord + 0xF53); break;
    }
    for (int i = 0; result < 0; i++) {
        for (int k = 0; k < 8; k++) {
            if (!((bits[i] >> k) & 1)) {
                if (n == 0) {
                    result = i * 8 + k;
                    break;
                }
                n--;
            }
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumRewards);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cBERewardInterface_getTotalNumPosters(void* self);
int cBERewardInterface_getTotalNumTradingCards(void* self);
int cBERewardInterface_getTotalNumConceptArts(void* self);
int cBERewardInterface_getTotalNumVideos(void* self);
int cBERewardInterface_getTotalNumToys(void* self);

/**
 * Returns how many rewards of one kind exist.
 *
 * Kind 3 posters (cBERewardInterface_getTotalNumPosters), 4 trading cards (cBERewardInterface_getTotalNumTradingCards), 5 concept arts (cBERewardInterface_getTotalNumConceptArts), 6 videos
 * (cBERewardInterface_getTotalNumVideos), 7 toys (cBERewardInterface_getTotalNumToys), 8 returns 20 (the cheat characters), 9 returns 0; anything else 0.
 *
 * @param kind  reward kind, 3 to 9
 * @return the number of rewards of that kind
 */
extern "C" int cBERewardInterface_getTotalNumRewards(cBERewardInterface* self, int kind)
{
    switch (kind) {
    case 3:
        return cBERewardInterface_getTotalNumPosters(self);
    case 4:
        return cBERewardInterface_getTotalNumTradingCards(self);
    case 5:
        return cBERewardInterface_getTotalNumConceptArts(self);
    case 6:
        return cBERewardInterface_getTotalNumVideos(self);
    case 7:
        return cBERewardInterface_getTotalNumToys(self);
    case 8:
        return 20;
    case 9:
        return 0;
    }
    return 0;
}
#endif

extern "C" void* cBERewardInterface_getDB(cBERewardInterface* self);

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumPosters__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of posters in the reward database (word +0x3C of cBERewardInterface_getDB(), On Tour
 * cBERewardDB.mNumPosters).
 *
 * @return the posters count
 */
int cBERewardInterface_getTotalNumPosters(void* self)
{
    return *(int*)((char*)cBERewardInterface_getDB((cBERewardInterface*)self) + 0x3c);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumTradingCards__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of trading cards in the reward database (word +0x40 of cBERewardInterface_getDB(), On Tour
 * cBERewardDB.mNumTradingCards).
 *
 * @return the trading cards count
 */
int cBERewardInterface_getTotalNumTradingCards(void* self)
{
    return *(int*)((char*)cBERewardInterface_getDB((cBERewardInterface*)self) + 0x40);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumConceptArts__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of concept arts in the reward database (word +0x44 of cBERewardInterface_getDB(), On Tour
 * cBERewardDB.mNumConceptArts).
 *
 * @return the concept arts count
 */
int cBERewardInterface_getTotalNumConceptArts(void* self)
{
    return *(int*)((char*)cBERewardInterface_getDB((cBERewardInterface*)self) + 0x44);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumToys__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of toys in the reward database (word +0x48 of cBERewardInterface_getDB(), On Tour
 * cBERewardDB.mNumToys).
 *
 * @return the toys count
 */
int cBERewardInterface_getTotalNumToys(void* self)
{
    return *(int*)((char*)cBERewardInterface_getDB((cBERewardInterface*)self) + 0x48);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumVideos__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of videos in the reward database (word +0x4C of cBERewardInterface_getDB(), On Tour
 * cBERewardDB.mNumVideos).
 *
 * @return the videos count
 */
int cBERewardInterface_getTotalNumVideos(void* self)
{
    return *(int*)((char*)cBERewardInterface_getDB((cBERewardInterface*)self) + 0x4c);
}
#endif

extern "C" void* cAudioInterface_bxAudio(void* self);

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumSongs__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of songs: the word at +0x504 of the audio object (cAudioInterface_bxAudio()).
 *
 * @return the song count
 */
int cBERewardInterface_getTotalNumSongs(void* self)
{
    return *(int*)((char*)cAudioInterface_bxAudio(self) + 0x504);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumCheatChars__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of cheat characters in the reward database (word +0x50 of cBERewardInterface_getDB(), On Tour
 * cBERewardDB.mNumCheatChars).
 *
 * @return the cheat character count
 */
int cBERewardInterface_getTotalNumCheatChars(void* self)
{
    return *(int*)((char*)cBERewardInterface_getDB((cBERewardInterface*)self) + 0x50);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_countOwnedRewards);
#ifdef SKIP_ASM
/**
 * Counts the set bits among the first @p count bits of a bit array (least significant bit of each byte first).
 *
 * Used by the getNumOwned functions on the ownership bits of the character record.
 *
 * @param count  number of bits to look at (On Tour nRewards)
 * @param bits  the bit array (On Tour flags)
 * @return the number of set bits
 */
extern "C" int cBERewardInterface_countOwnedRewards(cBERewardInterface* self, int count, signed char* bits)
{
    int i = 0;
    int total = 0;
    while (i < count)
    {
        signed char bitByte = *bits++;
        for (int k = 0; k < 8 && i < count; k++)
        {
            i++;
            total += (bitByte >> k) & 1;
        }
    }
    return total;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_markRewardOwned);
#ifdef SKIP_ASM
/**
 * Sets bit @p bit of a bit array (byte bit / 8, bit bit % 8).
 *
 * The buy functions (cBERewardInterface_buyPoster and its siblings) use it to mark a reward owned in the character record.
 *
 * @param bit  index of the bit to set (On Tour rewardIndex)
 * @param bits  the bit array (On Tour flags)
 */
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits)
{
    int shift = bit % 8;
    bits[(unsigned int)bit / 8] |= 1 << shift;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isRewardOwned);
#ifdef SKIP_ASM
/**
 * Returns bit @p bit of a bit array (byte bit / 8, bit bit % 8).
 *
 * The is...Owned functions (cBERewardInterface_isPosterOwned and its siblings) use it on the ownership bits of the character record.
 *
 * @param bit  index of the bit to test (On Tour rewardIndex)
 * @param bits  the bit array (On Tour flags)
 * @return 1 when the bit is set, else 0
 */
extern "C" int cBERewardInterface_isRewardOwned(cBERewardInterface* self, int bit, char* bits)
{
    int shift = bit % 8;
    return (bits[(unsigned int)bit / 8] >> shift) & 1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isRewardOwned1);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cBERewardInterface_isPosterOwned(cBERewardInterface* self, int a1, int a2, int bit);
extern "C" int cBERewardInterface_isTradingCardOwned(cBERewardInterface* self, int a1, int a2, int bit);
extern "C" int cBERewardInterface_isConceptArtOwned(cBERewardInterface* self, int a1, int a2, int bit);
extern "C" int cBERewardInterface_isVideoOwned(cBERewardInterface* self, int a1, int a2, int bit);
extern "C" int cBERewardInterface_isToyOwned(cBERewardInterface* self, int a1, int a2, int bit);
extern "C" int cBERewardInterface_isCheatCharOwned(cBERewardInterface* self, int a1, int a2, int bit);

/**
 * Returns whether the character owns reward @p bit of one kind.
 *
 * Dispatches by kind: 3 posters (cBERewardInterface_isPosterOwned), 4 trading cards (cBERewardInterface_isTradingCardOwned), 5 concept arts (cBERewardInterface_isConceptArtOwned), 6
 * videos (cBERewardInterface_isVideoOwned), 7 toys (cBERewardInterface_isToyOwned), 8 cheat characters (cBERewardInterface_isCheatCharOwned, with @p bit + 10 since that
 * function subtracts 10). Other kinds return 0.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param kind  reward kind, 3 to 8
 * @param bit  reward index
 * @return non-zero when owned
 */
extern "C" int cBERewardInterface_isRewardOwned1(cBERewardInterface* self, int profileIndex, int characterId, int kind, int bit)
{
    switch (kind) {
    case 3: return cBERewardInterface_isPosterOwned(self, profileIndex, characterId, bit);
    case 4: return cBERewardInterface_isTradingCardOwned(self, profileIndex, characterId, bit);
    case 5: return cBERewardInterface_isConceptArtOwned(self, profileIndex, characterId, bit);
    case 6: return cBERewardInterface_isVideoOwned(self, profileIndex, characterId, bit);
    case 7: return cBERewardInterface_isToyOwned(self, profileIndex, characterId, bit);
    case 8: return cBERewardInterface_isCheatCharOwned(self, profileIndex, characterId, bit + 10);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedRewards);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cBERewardInterface_getNumOwnedPosters(cBERewardInterface* self, int a1, int a2);
extern "C" int cBERewardInterface_getNumOwnedTradingCards(cBERewardInterface* self, int a1, int a2);
extern "C" int cBERewardInterface_getNumOwnedConceptArts(cBERewardInterface* self, int a1, int a2);
extern "C" int cBERewardInterface_getNumOwnedVideos(cBERewardInterface* self, int a1, int a2);
extern "C" int cBERewardInterface_getNumOwnedToys(cBERewardInterface* self, int a1, int a2);
extern "C" int cBERewardInterface_getNumOwnedCheatChars(cBERewardInterface* self, int a1, int a2);

/**
 * Returns how many rewards of one kind the character owns.
 *
 * Kind 3 posters (cBERewardInterface_getNumOwnedPosters), 4 trading cards (cBERewardInterface_getNumOwnedTradingCards), 5 concept arts (cBERewardInterface_getNumOwnedConceptArts), 6 videos
 * (cBERewardInterface_getNumOwnedVideos), 7 toys (cBERewardInterface_getNumOwnedToys), 8 cheat characters (cBERewardInterface_getNumOwnedCheatChars); other kinds 0.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param kind  reward kind, 3 to 8
 * @return the owned count
 */
extern "C" int cBERewardInterface_getNumOwnedRewards(cBERewardInterface* self, int profileIndex, int characterId, int kind)
{
    switch (kind) {
    case 3: return cBERewardInterface_getNumOwnedPosters(self, profileIndex, characterId);
    case 4: return cBERewardInterface_getNumOwnedTradingCards(self, profileIndex, characterId);
    case 5: return cBERewardInterface_getNumOwnedConceptArts(self, profileIndex, characterId);
    case 6: return cBERewardInterface_getNumOwnedVideos(self, profileIndex, characterId);
    case 7: return cBERewardInterface_getNumOwnedToys(self, profileIndex, characterId);
    case 8: return cBERewardInterface_getNumOwnedCheatChars(self, profileIndex, characterId);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedPosters);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
extern "C" int cBERewardInterface_countOwnedRewards(cBERewardInterface* self, int count, signed char* bits);

/**
 * Returns how many posters the character owns.
 *
 * Counts (cBERewardInterface_countOwnedRewards) the first N bits at +0xF30 of the character record (cBELibrary_profileLib + profile * 0x9B50 +
 * character * 0xF88), where N is the reward database's posters count (+0x3C of cBERewardInterface_getDB()).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the number of posters owned
 */
extern "C" int cBERewardInterface_getNumOwnedPosters(cBERewardInterface* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_countOwnedRewards(self, *(int*)((char*)cBERewardInterface_getDB_noargs() + 0x3C), (signed char*)(charRecord + 0xF30));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedTradingCards);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
extern "C" int cBERewardInterface_countOwnedRewards(cBERewardInterface* self, int count, signed char* bits);

/**
 * Returns how many trading cards the character owns.
 *
 * Counts (cBERewardInterface_countOwnedRewards) the first N bits at +0xF36 of the character record (cBELibrary_profileLib + profile * 0x9B50 +
 * character * 0xF88), where N is the reward database's trading cards count (+0x40 of cBERewardInterface_getDB()).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the number of trading cards owned
 */
extern "C" int cBERewardInterface_getNumOwnedTradingCards(cBERewardInterface* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_countOwnedRewards(self, *(int*)((char*)cBERewardInterface_getDB_noargs() + 0x40), (signed char*)(charRecord + 0xF36));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedConceptArts);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
extern "C" int cBERewardInterface_countOwnedRewards(cBERewardInterface* self, int count, signed char* bits);

/**
 * Returns how many concept arts the character owns.
 *
 * Counts (cBERewardInterface_countOwnedRewards) the first N bits at +0xF45 of the character record (cBELibrary_profileLib + profile * 0x9B50 +
 * character * 0xF88), where N is the reward database's concept arts count (+0x44 of cBERewardInterface_getDB()).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the number of concept arts owned
 */
extern "C" int cBERewardInterface_getNumOwnedConceptArts(cBERewardInterface* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_countOwnedRewards(self, *(int*)((char*)cBERewardInterface_getDB_noargs() + 0x44), (signed char*)(charRecord + 0xF45));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedToys);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
extern "C" int cBERewardInterface_countOwnedRewards(cBERewardInterface* self, int count, signed char* bits);

/**
 * Returns how many toys the character owns.
 *
 * Counts (cBERewardInterface_countOwnedRewards) the first N bits at +0xF53 of the character record (cBELibrary_profileLib + profile * 0x9B50 +
 * character * 0xF88), where N is the reward database's toys count (+0x48 of cBERewardInterface_getDB()).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the number of toys owned
 */
extern "C" int cBERewardInterface_getNumOwnedToys(cBERewardInterface* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_countOwnedRewards(self, *(int*)((char*)cBERewardInterface_getDB_noargs() + 0x48), (signed char*)(charRecord + 0xF53));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedVideos);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
extern "C" int cBERewardInterface_countOwnedRewards(cBERewardInterface* self, int count, signed char* bits);

/**
 * Returns how many videos the character owns.
 *
 * Counts (cBERewardInterface_countOwnedRewards) the first N bits at +0xF52 of the character record (cBELibrary_profileLib + profile * 0x9B50 +
 * character * 0xF88), where N is the reward database's videos count (+0x4C of cBERewardInterface_getDB()).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the number of videos owned
 */
extern "C" int cBERewardInterface_getNumOwnedVideos(cBERewardInterface* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_countOwnedRewards(self, *(int*)((char*)cBERewardInterface_getDB_noargs() + 0x4C), (signed char*)(charRecord + 0xF52));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedCheatChars);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
extern "C" int cBERewardInterface_countOwnedRewards(cBERewardInterface* self, int count, signed char* bits);

/**
 * Returns how many cheat characters the character owns: the set bits among the 20 at +0xF57 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * The front end (festatelodge, festateriderbio) uses it to see whether any cheat character is unlocked.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the number of cheat characters owned
 */
extern "C" int cBERewardInterface_getNumOwnedCheatChars(cBERewardInterface* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_countOwnedRewards(self, 20, (signed char*)(charRecord + 0xF57));
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isMountainConquered);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEScoreInterface_getCurrentHighlightLevel(void* self, int i);
extern "C" int cBERewardInterface_isPeakConquered(cBERewardInterface* self, int a, int b, int i);

/**
 * Tells whether a character has conquered the whole mountain.
 *
 * Needs all three peaks conquered (cBERewardInterface_isPeakConquered()) and the score interface's current
 * highlight levels over the 8 highlight categories to add up to at least 24 (every tier of every highlight).
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @return 1 when conquered, else 0
 */
extern "C" int cBERewardInterface_isMountainConquered(cBERewardInterface* self, int iProfileIndex, int iCharacterID)
{
    for (int i = 0; i < 3; i++)
    {
        if (!cBERewardInterface_isPeakConquered(self, iProfileIndex, iCharacterID, i))
            return 0;
    }
    int total = 0;
    void* score = cBE_getInterface_Fv(cBE_getBE(), 8);
    for (int i = 0; i < 8; i++)
    {
        total += cBEScoreInterface_getCurrentHighlightLevel(score, i);
    }
    return total >= 24;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isMountainCompleted);
#ifdef SKIP_ASM
extern "C" int cBERewardInterface_isPeakCompleted(cBERewardInterface* self, int a, int b, int i);

/**
 * Tells whether a character has completed the whole mountain, that is all three peaks.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @return 1 when cBERewardInterface_isPeakCompleted() holds for peaks 0-2, else 0
 */
extern "C" int cBERewardInterface_isMountainCompleted(cBERewardInterface* self, int iProfileIndex, int iCharacterID)
{
    for (int i = 0; i < 3; i++)
    {
        if (cBERewardInterface_isPeakCompleted(self, iProfileIndex, iCharacterID, i) == 0)
            return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isPeakConquered);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char cBERewardInterface_getTrackMedal(void* self, int a, int b, int track, int kind);
int cBERewardInterface_isBetterMedal(void* self, int a, int b);
extern "C" int cBERewardInterface_getCollectiblesMedal(cBERewardInterface* self, int a, int b, int kind);
extern "C" int cBERewardInterface_getBigChallengesMedal(cBERewardInterface* self, int a, int b, int kind);

struct sRewardReq_157920
{
    signed char track;
    signed char kind;
};
extern sRewardReq_157920 D_0045AAD8_v157920[][4][5] __asm__("gMedalsInfo");

/**
 * Tells whether a character has conquered a peak: top medals in all of its events.
 *
 * Walks the peak's event table gMedalsInfo for trophies 0-2 (trophy 3, earnings, is skipped). Events on tracks
 * 4-11 need any medal, other events medal 0 or 1; trophy 2 needs the best medal (0) for both collectibles and
 * big challenges.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param k  peak id
 * @return 1 when conquered, else 0
 */
extern "C" int cBERewardInterface_isPeakConquered(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int k)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int track = D_0045AAD8_v157920[k][i][j].track;
            int kind = D_0045AAD8_v157920[k][i][j].kind;
            if (track < 0)
                break;
            if (i == 3)
                continue;
            if (i == 2)
            {
                if (cBERewardInterface_getCollectiblesMedal(self, iProfileIndex, iCharacterID, k))
                    return 0;
                if (cBERewardInterface_getBigChallengesMedal(self, iProfileIndex, iCharacterID, k))
                    return 0;
            }
            else
            {
                int medal = cBERewardInterface_getTrackMedal(self, iProfileIndex, iCharacterID, track, kind);
                if ((unsigned int)(track - 4) < 8)
                {
                    if (!cBERewardInterface_isBetterMedal(self, -1, medal))
                        return 0;
                }
                else if ((unsigned int)medal >= 2)
                    return 0;
            }
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isPeakCompleted);
#ifdef SKIP_ASM
extern "C" int cBERewardInterface_isTrophyOwned(cBERewardInterface* self, int a, int b, int c, int i);

/**
 * Tells whether a character has completed a peak, that is owns all four of its trophies.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param peakID  peak id
 * @return 1 when cBERewardInterface_isTrophyOwned() holds for trophies 0-3, else 0
 */
extern "C" int cBERewardInterface_isPeakCompleted(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int peakID)
{
    for (int i = 0; i < 4; i++)
    {
        if (cBERewardInterface_isTrophyOwned(self, iProfileIndex, iCharacterID, peakID, i) == 0)
            return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isPeakLocked);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern "C" int cBERewardInterface_isPeakLocked1(cBERewardInterface* self, int a, int b, int peak);

/**
 * Returns whether a peak is still locked for a race rider.
 *
 * Converts the rider to its profile (cBELibrary_getProfileIndex) and character (cBELibrary_getCharacterID) and calls
 * cBERewardInterface_isPeakLocked1.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param peak  the peak (On Tour peakID)
 * @return 1 when locked, else 0
 */
extern "C" int cBERewardInterface_isPeakLocked(cBERewardInterface* self, int rider, int peak)
{
    int profile = cBELibrary_getProfileIndex(rider);
    return cBERewardInterface_isPeakLocked1(self, profile, cBELibrary_getCharacterID(rider), peak);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isPeakLocked1);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

/**
 * Returns whether peak 1 or 2 is still locked for a character.
 *
 * Reads the 64-bit flags word at +0x278 of the character record (cBELibrary_profileLib + profile * 0x9B50 + character *
 * 0xF88): bit 12 for peak 1, bit 13 for peak 2. Every other peak is never locked (0). cBERewardInterface_awardUserPeakPass clears these bits.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param peak  the peak (On Tour peakID)
 * @return 1 when locked, else 0
 */
// PORT: 64-bit ulong flag word (bit 12 = peak 1 locked, bit 13 = peak 2).
extern "C" int cBERewardInterface_isPeakLocked1(cBERewardInterface* self, int profileIndex, int characterId, int peak)
{
    if (peak == 1)
    {
        int bOff = characterId * 3976;
        int aOff = profileIndex * 39760;
        int off = bOff + aOff;
        char* charRecord = (char*)cBELibrary_profileLib + off;
        return (int)(*(ulong*)(charRecord + 0x278) >> 12) & 1;
    }
    if (peak == 2)
    {
        int bOff = characterId * 3976;
        int aOff = profileIndex * 39760;
        int off = bOff + aOff;
        char* charRecord = (char*)cBELibrary_profileLib + off;
        return (int)(*(ulong*)(charRecord + 0x278) >> 13) & 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isTrophyOwned);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char cBERewardInterface_getTrackMedal(void* self, int b, int c, int d, int e);
int cBERewardInterface_isBetterMedal(void* self, int a, int b);
extern "C" int cBERewardInterface_getEarningsMedal(cBERewardInterface* self, int a, int b, int medal);
extern "C" int cBERewardInterface_getCollectiblesMedal(cBERewardInterface* self, int a, int b, int kind);
extern "C" int cBERewardInterface_getBigChallengesMedal(cBERewardInterface* self, int a, int b, int kind);

struct sRewardReq_157BF0
{
    signed char track;
    signed char kind;
};
extern sRewardReq_157BF0 D_0045AAD8_v157BF0[][4][5] __asm__("gMedalsInfo");

/**
 * Tells whether a character owns one of a peak's four trophies.
 *
 * Trophies 0 and 1 need a medal in every event listed for them in gMedalsInfo; trophy 2 needs a collectibles
 * medal and a big-challenges medal; trophy 3 needs the peak's earnings target reached (any medal from
 * cBERewardInterface_getEarningsMedal()).
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param k  peak id
 * @param i  trophy id, 0-3
 * @return 1 when the trophy is owned, 0 otherwise or for a trophy id outside 0-3
 */
extern "C" int cBERewardInterface_isTrophyOwned(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int k, int i)
{
    switch (i)
    {
    case 0:
    case 1:
        for (int j = 0; j < 5; j++)
        {
            int track = D_0045AAD8_v157BF0[k][i][j].track;
            int kind = D_0045AAD8_v157BF0[k][i][j].kind;
            if (track < 0)
                break;
            if (!cBERewardInterface_isBetterMedal(self, -1, cBERewardInterface_getTrackMedal(self, iProfileIndex, iCharacterID, track, kind)))
                return 0;
        }
        return 1;
    case 2:
    {
        int owned = 0;
        if (cBERewardInterface_isBetterMedal(self, -1, cBERewardInterface_getCollectiblesMedal(self, iProfileIndex, iCharacterID, k)))
            owned = cBERewardInterface_isBetterMedal(self, -1, cBERewardInterface_getBigChallengesMedal(self, iProfileIndex, iCharacterID, k)) != 0;
        return owned;
    }
    case 3:
        return cBERewardInterface_isBetterMedal(self, -1, cBERewardInterface_getEarningsMedal(self, iProfileIndex, iCharacterID, k));
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isPosterOwned);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
extern "C" int cBERewardInterface_isRewardOwned(cBERewardInterface* self, int bit, char* bits);

/**
 * Returns whether the character owns poster @p bit: bit @p bit of the bits at +0xF30 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * Tests with cBERewardInterface_isRewardOwned.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  poster index (On Tour rewardIndex)
 * @return 1 when owned, else 0
 */
extern "C" int cBERewardInterface_isPosterOwned(cBERewardInterface* self, int profileIndex, int characterId, int bit)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_isRewardOwned(self, bit, charRecord + 0xF30);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isTradingCardOwned);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
extern "C" int cBERewardInterface_isRewardOwned(cBERewardInterface* self, int bit, char* bits);

/**
 * Returns whether the character owns trading card @p bit: bit @p bit of the bits at +0xF36 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * Tests with cBERewardInterface_isRewardOwned.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  trading card index (On Tour rewardIndex)
 * @return 1 when owned, else 0
 */
extern "C" int cBERewardInterface_isTradingCardOwned(cBERewardInterface* self, int profileIndex, int characterId, int bit)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_isRewardOwned(self, bit, charRecord + 0xF36);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isConceptArtOwned);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
extern "C" int cBERewardInterface_isRewardOwned(cBERewardInterface* self, int bit, char* bits);

/**
 * Returns whether the character owns concept art @p bit: bit @p bit of the bits at +0xF45 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * Tests with cBERewardInterface_isRewardOwned.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  concept art index (On Tour rewardIndex)
 * @return 1 when owned, else 0
 */
extern "C" int cBERewardInterface_isConceptArtOwned(cBERewardInterface* self, int profileIndex, int characterId, int bit)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_isRewardOwned(self, bit, charRecord + 0xF45);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isToyOwned);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
extern "C" int cBERewardInterface_isRewardOwned(cBERewardInterface* self, int bit, char* bits);

/**
 * Returns whether the character owns toy @p bit: bit @p bit of the bits at +0xF53 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * Tests with cBERewardInterface_isRewardOwned.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  toy index (On Tour rewardIndex)
 * @return 1 when owned, else 0
 */
extern "C" int cBERewardInterface_isToyOwned(cBERewardInterface* self, int profileIndex, int characterId, int bit)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_isRewardOwned(self, bit, charRecord + 0xF53);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isVideoOwned);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
extern "C" int cBERewardInterface_isRewardOwned(cBERewardInterface* self, int bit, char* bits);

/**
 * Returns whether the character owns video @p bit: bit @p bit of the bits at +0xF52 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * Tests with cBERewardInterface_isRewardOwned.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  video index (On Tour rewardIndex)
 * @return 1 when owned, else 0
 */
extern "C" int cBERewardInterface_isVideoOwned(cBERewardInterface* self, int profileIndex, int characterId, int bit)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_isRewardOwned(self, bit, charRecord + 0xF52);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00157EA0);
#ifdef SKIP_ASM
/**
 * Returns bit @p bit of the 64-bit playlist word at +0xF78 of the character record (cBELibrary_profileLib + profile *
 * 0x9B50 + character * 0xF88).
 *
 * Nothing calls it. It sits where On Tour has isSongOwned (between the video and cheat-character tests), but +0xF78 is
 * the playlist (cBERewardInterface_getNumSongsInPlayList, cBERewardInterface_getPlayList, cBERewardInterface_setPlayList); the owned-song word is +0xF70
 * (cBERewardInterface_getOwnedSongList, cBERewardInterface_buySong). Either it is a playlist test or a song-ownership test reading the wrong word.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  song index
 * @return 1 when the bit is set, else 0
 */
// PORT: 64-bit ulong bit test (ld/dsrlv).
extern "C" int func_00157EA0(void* self, int profileIndex, int characterId, int bit)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return (int)((*(ulong*)(charRecord + 0xF78) >> bit) & 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isCheatCharOwned);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
extern "C" int cBERewardInterface_isRewardOwned(cBERewardInterface* self, int bit, char* bits);

/**
 * Returns whether the character owns cheat character @p bit: bit (@p bit - 10) of the bits at +0xF57 of the character
 * record (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * Cheat character ids start at 10, after the ten regular characters. The character-select screen uses it to list the
 * owned cheat characters.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  cheat character id, 10 and up (On Tour cheatCharID)
 * @return 1 when owned, else 0
 */
extern "C" int cBERewardInterface_isCheatCharOwned(cBERewardInterface* self, int profileIndex, int characterId, int bit)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_isRewardOwned(self, bit-10, charRecord + 0xF57);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyReward);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// The award handlers are declared void elsewhere in this unit; their results are returned here.
int cBERewardInterface_buyPoster_r(void* self, int a1, int a2, int bit, int give) __asm__("cBERewardInterface_buyPoster");
int cBERewardInterface_buyTradingCard_r(void* self, int a1, int a2, int bit, int give) __asm__("cBERewardInterface_buyTradingCard");
int cBERewardInterface_buyConceptArt_r(void* self, int a1, int a2, int bit, int give) __asm__("cBERewardInterface_buyConceptArt");
int cBERewardInterface_buyVideo_r(void* self, int a1, int a2, int bit, int give) __asm__("cBERewardInterface_buyVideo");
int cBERewardInterface_buyToy_r(void* self, int a1, int a2, int bit, int give) __asm__("cBERewardInterface_buyToy");
int cBERewardInterface_buyCheatChar_r(void* self, int a1, int a2, int bit, int give) __asm__("cBERewardInterface_buyCheatChar");

/**
 * Gives the character reward @p bit of one kind, optionally charging its price.
 *
 * Dispatches by kind: 3 cBERewardInterface_buyPoster (poster), 4 cBERewardInterface_buyTradingCard (trading card), 5 cBERewardInterface_buyConceptArt (concept art), 6
 * cBERewardInterface_buyVideo (video), 7 cBERewardInterface_buyToy (toy), 8 cBERewardInterface_buyCheatChar (cheat character, with @p bit + 10). The gallery's buy
 * popup calls it.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param kind  reward kind, 3 to 8
 * @param bit  reward index
 * @param give  non-zero to charge the price (On Tour payForIt)
 * @return the called function's result, or 0 for other kinds
 */
extern "C" int cBERewardInterface_buyReward(cBERewardInterface* self, int profileIndex, int characterId, int kind, int bit, int give)
{
    switch (kind) {
    case 3: return cBERewardInterface_buyPoster_r(self, profileIndex, characterId, bit, give);
    case 4: return cBERewardInterface_buyTradingCard_r(self, profileIndex, characterId, bit, give);
    case 5: return cBERewardInterface_buyConceptArt_r(self, profileIndex, characterId, bit, give);
    case 6: return cBERewardInterface_buyVideo_r(self, profileIndex, characterId, bit, give);
    case 7: return cBERewardInterface_buyToy_r(self, profileIndex, characterId, bit, give);
    case 8: return cBERewardInterface_buyCheatChar_r(self, profileIndex, characterId, bit + 10, give);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyPoster);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);
extern "C" int cBERewardInterface_getNumOwnedPosters(cBERewardInterface* self, int a1, int a2);
int cBERewardInterface_getTotalNumPosters(void* self);
extern "C" void cBERewardInterface_buyCheatChar(cBERewardInterface* self, int a1, int a2, int a3, int a4);
extern int cBELibrary_profileLib[];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");

struct sRewardDef_157FD0
{
    char pad0[0xC];
    short amount;   // 0xC
    short padE;
};

/**
 * Gives the character poster @p bit, charging its price when @p give is set.
 *
 * Marks the bit at +0xF30 of the character record (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88)
 * (cBERewardInterface_markRewardOwned). With @p give, takes the price, the short at +0xC of the poster's 16-byte record (table at +0x14 of
 * cBERewardInterface_getDB()), times 10 when positive, off the character's money (cBEEconInterface_reduceMoney on the economy
 * interface, cBE_getInterface kind 11). When the character then owns every poster (owned count equal to the database
 * count) it also gets cheat character 20 (cBERewardInterface_buyCheatChar, unpaid). Ends with cBEEconInterface_getMoney().
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  poster index (On Tour rewardIndex)
 * @param give  non-zero to charge the price (On Tour payForIt)
 */
extern "C" void cBERewardInterface_buyPoster(cBERewardInterface* self, int profileIndex, int characterId, int bit, int give)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    cBERewardInterface_markRewardOwned(self, bit, charRecord + 0xF30);
    if (give)
    {
        char* defs = *(char**)((char*)cBERewardInterface_getDB_noargs() + 0x14);
        int amount = ((sRewardDef_157FD0*)(defs + (bit << 4)))->amount;
        if (amount > 0) amount *= 10;
        cBEEconInterface_reduceMoney(econ, profileIndex, characterId, amount);
    }
    if (cBERewardInterface_getNumOwnedPosters(self, profileIndex, characterId) == cBERewardInterface_getTotalNumPosters(self))
    {
        cBERewardInterface_buyCheatChar(self, profileIndex, characterId, 20, 0);
    }
    cBEEconInterface_getMoney(econ, profileIndex, characterId);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyTradingCard);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);
extern "C" int cBERewardInterface_getNumOwnedTradingCards(cBERewardInterface* self, int a1, int a2);
int cBERewardInterface_getTotalNumTradingCards(void* self);
extern "C" void cBERewardInterface_buyCheatChar(cBERewardInterface* self, int a1, int a2, int a3, int a4);
extern int cBELibrary_profileLib[];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");

struct sRewardDef_1580F8
{
    char pad0[0xC];
    short amount;   // 0xC
    short padE;
};

/**
 * Gives the character trading card @p bit, charging its price when @p give is set.
 *
 * Marks the bit at +0xF36 of the character record (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88)
 * (cBERewardInterface_markRewardOwned). With @p give, takes the price, the short at +0xC of the trading card's 16-byte record (table at
 * +0x18 of cBERewardInterface_getDB()), times 10 when positive, off the character's money (cBEEconInterface_reduceMoney on the economy
 * interface, cBE_getInterface kind 11). When the character then owns every trading card (owned count equal to the
 * database count) it also gets cheat character 17 (cBERewardInterface_buyCheatChar, unpaid). Ends with cBEEconInterface_getMoney().
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  trading card index (On Tour rewardIndex)
 * @param give  non-zero to charge the price (On Tour payForIt)
 */
extern "C" void cBERewardInterface_buyTradingCard(cBERewardInterface* self, int profileIndex, int characterId, int bit, int give)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    cBERewardInterface_markRewardOwned(self, bit, charRecord + 0xF36);
    if (give)
    {
        char* defs = *(char**)((char*)cBERewardInterface_getDB_noargs() + 0x18);
        int amount = ((sRewardDef_1580F8*)(defs + (bit << 4)))->amount;
        if (amount > 0) amount *= 10;
        cBEEconInterface_reduceMoney(econ, profileIndex, characterId, amount);
    }
    if (cBERewardInterface_getNumOwnedTradingCards(self, profileIndex, characterId) == cBERewardInterface_getTotalNumTradingCards(self))
    {
        cBERewardInterface_buyCheatChar(self, profileIndex, characterId, 17, 0);
    }
    cBEEconInterface_getMoney(econ, profileIndex, characterId);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyConceptArt);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);
extern "C" int cBERewardInterface_getNumOwnedConceptArts(cBERewardInterface* self, int a1, int a2);
int cBERewardInterface_getTotalNumConceptArts(void* self);
extern "C" void cBERewardInterface_buyCheatChar(cBERewardInterface* self, int a1, int a2, int a3, int a4);
extern int cBELibrary_profileLib[];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");

struct sRewardDef_158220
{
    char pad0[0xC];
    short amount;   // 0xC
    short padE;
};

/**
 * Gives the character concept art @p bit, charging its price when @p give is set.
 *
 * Marks the bit at +0xF45 of the character record (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88)
 * (cBERewardInterface_markRewardOwned). With @p give, takes the price, the short at +0xC of the concept art's 16-byte record (table at +0x1C
 * of cBERewardInterface_getDB()), times 10 when positive, off the character's money (cBEEconInterface_reduceMoney on the economy
 * interface, cBE_getInterface kind 11). When the character then owns every concept art (owned count equal to the
 * database count) it also gets cheat character 22 (cBERewardInterface_buyCheatChar, unpaid). Ends with cBEEconInterface_getMoney().
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  concept art index (On Tour rewardIndex)
 * @param give  non-zero to charge the price (On Tour payForIt)
 */
extern "C" void cBERewardInterface_buyConceptArt(cBERewardInterface* self, int profileIndex, int characterId, int bit, int give)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    cBERewardInterface_markRewardOwned(self, bit, charRecord + 0xF45);
    if (give)
    {
        char* defs = *(char**)((char*)cBERewardInterface_getDB_noargs() + 0x1C);
        int amount = ((sRewardDef_158220*)(defs + (bit << 4)))->amount;
        if (amount > 0) amount *= 10;
        cBEEconInterface_reduceMoney(econ, profileIndex, characterId, amount);
    }
    if (cBERewardInterface_getNumOwnedConceptArts(self, profileIndex, characterId) == cBERewardInterface_getTotalNumConceptArts(self))
    {
        cBERewardInterface_buyCheatChar(self, profileIndex, characterId, 22, 0);
    }
    cBEEconInterface_getMoney(econ, profileIndex, characterId);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyToy);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);
extern "C" int cBERewardInterface_getNumOwnedToys(cBERewardInterface* self, int a1, int a2);
int cBERewardInterface_getTotalNumToys(void* self);
extern "C" void cBERewardInterface_buyCheatChar(cBERewardInterface* self, int a1, int a2, int a3, int a4);
extern int cBELibrary_profileLib[];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");

struct sRewardDef_158348
{
    char pad0[0xC];
    short amount;   // 0xC
    short padE;
};

/**
 * Gives the character toy @p bit, charging its price when @p give is set.
 *
 * Marks the bit at +0xF53 of the character record (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88)
 * (cBERewardInterface_markRewardOwned). With @p give, takes the price, the short at +0xC of the toy's 16-byte record (table at +0x20 of
 * cBERewardInterface_getDB()), times 10 when positive, off the character's money (cBEEconInterface_reduceMoney on the economy
 * interface, cBE_getInterface kind 11). When the character then owns every toy (owned count equal to the database
 * count) it also gets cheat character 19 (cBERewardInterface_buyCheatChar, unpaid). Ends with cBEEconInterface_getMoney().
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  toy index (On Tour rewardIndex)
 * @param give  non-zero to charge the price (On Tour payForIt)
 */
extern "C" void cBERewardInterface_buyToy(cBERewardInterface* self, int profileIndex, int characterId, int bit, int give)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    cBERewardInterface_markRewardOwned(self, bit, charRecord + 0xF53);
    if (give)
    {
        char* defs = *(char**)((char*)cBERewardInterface_getDB_noargs() + 0x20);
        int amount = ((sRewardDef_158348*)(defs + (bit << 4)))->amount;
        if (amount > 0) amount *= 10;
        cBEEconInterface_reduceMoney(econ, profileIndex, characterId, amount);
    }
    if (cBERewardInterface_getNumOwnedToys(self, profileIndex, characterId) == cBERewardInterface_getTotalNumToys(self))
    {
        cBERewardInterface_buyCheatChar(self, profileIndex, characterId, 19, 0);
    }
    cBEEconInterface_getMoney(econ, profileIndex, characterId);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyVideo);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
extern int cBELibrary_profileLib[];
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);

/**
 * Gives the character video @p idx, charging its price when @p award is set.
 *
 * Marks the bit at +0xF52 of the character record (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88)
 * (cBERewardInterface_markRewardOwned). With @p award, takes the price, the short at +0xC of the video's 16-byte record (table at +0x24 of
 * cBERewardInterface_getDB()), times 10 when positive, off the character's money (cBEEconInterface_reduceMoney on the economy
 * interface). Ends with cBEEconInterface_getMoney(). Unlike the poster, card, art and toy versions it unlocks nothing
 * for a complete set.
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param idx  video index (On Tour rewardIndex)
 * @param award  non-zero to charge the price (On Tour payForIt)
 */
extern "C" void cBERewardInterface_buyVideo(cBERewardInterface* self, int profile, int characterId, int idx, int award)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    char* charRecord = (char*)cBELibrary_profileLib + profile * 0x9b50 + characterId * 3976;
    cBERewardInterface_markRewardOwned(self, idx, charRecord + 0xF52);
    if (award)
    {
        int amount = *(short*)(*(char**)((char*)cBERewardInterface_getDB_noargs() + 0x24) + (idx << 4) + 0xC);
        if (amount > 0)
            amount *= 10;
        cBEEconInterface_reduceMoney(econ, profile, characterId, amount);
    }
    cBEEconInterface_getMoney(econ, profile, characterId);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_buySong);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern int cBELibrary_profileLib[];
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);

/**
 * Gives the character song @p bit, charging 5000 when @p award is set.
 *
 * Sets bit @p bit of the 64-bit owned-song word at +0xF70 of the character record (cBELibrary_profileLib + profile *
 * 0x9B50 + character * 0xF88); with @p award, takes 5000 off the character's money (cBEEconInterface_reduceMoney on the economy
 * interface). Ends with cBEEconInterface_getMoney(). The message center's request line and the cheat codes call it.
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  song index (On Tour songIndex)
 * @param award  non-zero to charge the price (On Tour payForIt)
 */
// PORT: 64-bit ulong bit set (sd/ld).
extern "C" void cBERewardInterface_buySong(cBERewardInterface* self, int profile, int characterId, int bit, int award)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    char* charRecord = (char*)cBELibrary_profileLib + profile * 0x9b50 + characterId * 3976;
    *(ulong*)(charRecord + 0xF70) |= (ulong)1 << bit;
    if (award)
    {
        cBEEconInterface_reduceMoney(econ, profile, characterId, 5000);
    }
    cBEEconInterface_getMoney(econ, profile, characterId);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyCheatChar);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
void* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
extern int cBELibrary_profileLib[];
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);

/**
 * Gives the character cheat character @p idx, charging its price when @p award is set.
 *
 * Marks bit (@p idx - 10) of the bits at +0xF57 of the character record (cBELibrary_profileLib + profile * 0x9B50 +
 * character * 0xF88) (cBERewardInterface_markRewardOwned). With @p award, takes the price, the short at +0xC of the 16-byte record (idx -
 * 10) in the table at +0x28 of cBERewardInterface_getDB(), times 10 when positive, off the character's money
 * (cBEEconInterface_reduceMoney). Ends with cBEEconInterface_getMoney(). The buy functions call it with ids 17, 19, 20 and 22 for
 * completed sets; the cheat codes and cBERewardInterface_awardUser call it too.
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param idx  cheat character id, 10 and up (On Tour cheatCharID)
 * @param award  non-zero to charge the price (On Tour payForIt)
 */
extern "C" void cBERewardInterface_buyCheatChar(cBERewardInterface* self, int profile, int characterId, int idx, int award)
{
    idx -= 10;
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    char* charRecord = (char*)cBELibrary_profileLib + profile * 0x9b50 + characterId * 3976;
    cBERewardInterface_markRewardOwned(self, idx, charRecord + 0xF57);
    if (award)
    {
        int amount = *(short*)(*(char**)((char*)cBERewardInterface_getDB_noargs() + 0x28) + (idx << 4) + 0xC);
        if (amount > 0)
            amount *= 10;
        cBEEconInterface_reduceMoney(econ, profile, characterId, amount);
    }
    cBEEconInterface_getMoney(econ, profile, characterId);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getRadioMode);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

/**
 * Returns the character's radio mode: the word at +0xF80 of the character record (cBELibrary_profileLib + profile *
 * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the radio mode
 */
extern "C" int cBERewardInterface_getRadioMode(cBERewardInterface* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return *(int*)(charRecord + 0xf80);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getOwnedSongList);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern int cBELibrary_profileLib[];

/**
 * Returns the character's owned-song list: the 64-bit word at +0xF70 of the character record (cBELibrary_profileLib +
 * profile * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the owned-song bits
 */
extern "C" long cBERewardInterface_getOwnedSongList(cBERewardInterface* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return *(long*)(charRecord + 0xf70);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getPlayList);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern int cBELibrary_profileLib[];

/**
 * Returns the character's playlist: the 64-bit word at +0xF78 of the character record (cBELibrary_profileLib + profile
 * * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the playlist bits
 */
extern "C" long cBERewardInterface_getPlayList(cBERewardInterface* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return *(long*)(charRecord + 0xf78);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedSongs);
#ifdef SKIP_ASM
/**
 * Returns how many songs the character owns: the set bits among the 64 at +0xF70 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the number of owned songs
 */
extern "C" int cBERewardInterface_getNumOwnedSongs(cBERewardInterface* self, int profileIndex, int characterId)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    return cBERewardInterface_countOwnedRewards(self, 0x40, (signed char*)(charRecord + 0xF70));
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumSongsInPlayList);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
extern "C" int cBERewardInterface_countOwnedRewards(cBERewardInterface* self, int count, signed char* bits);

/**
 * Counts the songs in a character's playlist: the set bits among the 64 at +0xF78 of the profile record.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @return the number of songs
 */
extern "C" int cBERewardInterface_getNumSongsInPlayList(cBERewardInterface* self, int iProfileIndex, int iCharacterID)
{
    char* charProfile = (char*)cBELibrary_profileLib + iProfileIndex * 0x9b50 + iCharacterID * 3976;
    return cBERewardInterface_countOwnedRewards(self, 0x40, (signed char*)(charProfile + 0xF78));
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_setRadioMode);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

/**
 * Sets the character's radio mode: the word at +0xF80 of the character record (cBELibrary_profileLib + profile * 0x9B50
 * + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param value  the radio mode (On Tour radioMode)
 */
extern "C" void cBERewardInterface_setRadioMode(cBERewardInterface* self, int profileIndex, int characterId, int value)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    *(int*)(charRecord + 0xf80) = value;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_setOwnedSongList);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern int cBELibrary_profileLib[];

/**
 * Sets the character's owned-song list: the 64-bit word at +0xF70 of the character record (cBELibrary_profileLib +
 * profile * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param value  the owned-song bits (On Tour iOwnedSongList)
 */
extern "C" void cBERewardInterface_setOwnedSongList(cBERewardInterface* self, int profileIndex, int characterId, long value)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    *(long*)(charRecord + 0xf70) = value;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_setPlayList);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern int cBELibrary_profileLib[];

/**
 * Sets the character's playlist: the 64-bit word at +0xF78 of the character record (cBELibrary_profileLib + profile *
 * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param value  the playlist bits (On Tour iPlayList)
 */
extern "C" void cBERewardInterface_setPlayList(cBERewardInterface* self, int profileIndex, int characterId, long value)
{
    char* charRecord = (char*)cBELibrary_profileLib + profileIndex * 0x9b50 + characterId * 3976;
    *(long*)(charRecord + 0xf78) = value;
}
#endif

extern int gCollectiblesMedalLevel[];

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getCollectibleMedalTarget);
#ifdef SKIP_ASM
/**
 * Returns the collectibles count a peak needs for a medal (gCollectiblesMedalLevel, four targets per peak).
 *
 * @param peakID  the peak id
 * @param medal  the medal, 0 (best) to 3
 * @return the target count
 */
extern "C" int cBERewardInterface_getCollectibleMedalTarget(cBERewardInterface* self, int peakID, int medal)
{
    return *(int*)((char*)gCollectiblesMedalLevel + ((medal << 2) + (peakID << 4)));
}
#endif

extern int gBigChallengesMedalLevel[];

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getChallengesMedalTarget);
#ifdef SKIP_ASM
/**
 * Returns the completed big-challenge count a peak needs for a medal (gBigChallengesMedalLevel, four per peak).
 *
 * @param peakID  the peak id
 * @param medal  the medal, 0 (best) to 3
 * @return the target count
 */
extern "C" int cBERewardInterface_getChallengesMedalTarget(cBERewardInterface* self, int peakID, int medal)
{
    return *(int*)((char*)gBigChallengesMedalLevel + ((medal << 2) + (peakID << 4)));
}
#endif

extern void* gEarningsMedalLevel[];

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getEarningsTarget__FPvi);
#ifdef SKIP_ASM
/**
 * Returns a peak's earnings target from gEarningsMedalLevel.
 *
 * @param peakID  the peak id
 * @return the target amount
 */
int cBERewardInterface_getEarningsTarget(void* self, int peakID)
{
    return *(int*)((char*)(void*)gEarningsMedalLevel + peakID * 4);
}
#endif

void* cBECharProfileDB_getScoreStats(void* self, int a, int b);
extern int cBELibrary_profileLib[];

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_calculateCollectibleMedal);
#ifdef SKIP_ASM
/**
 * Returns the best collectibles medal a count reaches on a peak.
 *
 * Compares @p numCollected with the peak's four targets in gCollectiblesMedalLevel (the table cBERewardInterface_getCollectibleMedalTarget
 * reads), best medal first.
 *
 * @param idx  the peak (On Tour peakID)
 * @param numCollected  number of collectibles found (On Tour numCollected)
 * @return the first medal (0 best to 3) whose target @p numCollected reaches, or -1
 */
extern "C" int cBERewardInterface_calculateCollectibleMedal(cBERewardInterface* self, int idx, int numCollected)
{
    int* row = gCollectiblesMedalLevel + idx * 4;
    for (int i = 0; i < 4; i++) {
        if (numCollected >= row[i]) return i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrackMedal__FPviiii);
#ifdef SKIP_ASM
struct sProfileSlot_00158910
{
    char data[0xF88];
};

/**
 * Returns a character's medal for one event: byte 1 of its score record.
 *
 * The record comes from cBECharProfileDB_getScoreStats() with @p location and @p eventID.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param eventID  passed as the score record's event kind
 * @param location  passed as the score record's location
 * @return the medal (0 best to 3), or -1 when there is no record
 */
signed char cBERewardInterface_getTrackMedal(void* self, int iProfileIndex, int iCharacterID, int eventID, int location)
{
    void* result = cBECharProfileDB_getScoreStats(&((sProfileSlot_00158910 (*)[10])cBELibrary_profileLib)[iProfileIndex][iCharacterID], location, eventID);
    if (result == 0) {
        return -1;
    }
    return ((signed char*)result)[1];
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrackHighScore);
#ifdef SKIP_ASM
/**
 * Returns a character's best result for one event: the word at +0x4 of its score record.
 *
 * The record comes from cBECharProfileDB_getScoreStats() with @p locationId and @p event, as in
 * cBERewardInterface_getTrackMedal (which reads the medal byte of the same record). The trophy room shows it as the
 * best time (seconds) or best score.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param event  the game event (On Tour eventID)
 * @param locationId  the location (On Tour location)
 * @return the best time or score, or 0 when there is no record
 */
extern "C" int cBERewardInterface_getTrackHighScore(cBERewardInterface* self, int profileIndex, int characterId, int event, int locationId)
{
    void* result = cBECharProfileDB_getScoreStats(&((sProfileSlot_00158910 (*)[10])cBELibrary_profileLib)[profileIndex][characterId], locationId, event);
    if (result == 0) {
        return 0;
    }
    return ((int*)result)[1];
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getCollectiblesMedal);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBEMissionInterface_getCurrentCollectForPeakByChar(void* iface, int a, int b, int c);

/**
 * Returns the collectibles medal a character has on a peak.
 *
 * Compares the mission interface's collectibles count for the peak with the peak's four targets (gCollectiblesMedalLevel).
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param kind  the peak id (On Tour: peakID)
 * @return the best medal reached (0 best to 3), or -1
 */
extern "C" int cBERewardInterface_getCollectiblesMedal(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int kind)
{
    int collected = cBEMissionInterface_getCurrentCollectForPeakByChar(cBE_getInterface_Fv(cBE_getBE(), 10), iProfileIndex, iCharacterID, kind);
    int* thresholds = gCollectiblesMedalLevel + kind * 4;
    for (int i = 0; i < 4; i++)
    {
        if (collected >= thresholds[i])
            return i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getBigChallengesMedal);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeakByChar(void* iface, int a, int b, int c);

/**
 * Returns the big-challenges medal a character has on a peak.
 *
 * Compares the mission interface's completed-mission count on the peak with its four targets (gBigChallengesMedalLevel).
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param kind  the peak id (On Tour: peakID)
 * @return the best medal reached (0 best to 3), or -1
 */
extern "C" int cBERewardInterface_getBigChallengesMedal(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int kind)
{
    int completed = cBEMissionInterface_getNumberCompletedMissionsOnPeakByChar(cBE_getInterface_Fv(cBE_getBE(), 10), iProfileIndex, iCharacterID, kind);
    int* thresholds = gBigChallengesMedalLevel + kind * 4;
    for (int i = 0; i < 4; i++)
    {
        if (completed >= thresholds[i])
            return i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getEarningsMedal);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEEconInterface_getTotalMoneyEarned(void* self, int a, int b);

/**
 * Tells whether a character's total earnings reach a peak's earnings target.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param medal  the peak id (an index into gEarningsMedalLevel), despite the parameter's name
 * @return 1 (a medal) when the target is reached, else -1
 */
extern "C" int cBERewardInterface_getEarningsMedal(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int medal)
{
    unsigned int earned = cBEEconInterface_getTotalMoneyEarned(cBE_getInterface_Fv(cBE_getBE(), 11), iProfileIndex, iCharacterID);
    if (earned >= *(unsigned int*)((char*)(void*)gEarningsMedalLevel + medal * 4))
        return 1;
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_calculateBigChallengeMedal);
#ifdef SKIP_ASM
/**
 * Returns the best big-challenges medal a completed-challenge count reaches on a peak.
 *
 * Compares @p numCompleted with the peak's four targets in gBigChallengesMedalLevel (the table cBERewardInterface_getChallengesMedalTarget
 * reads), best medal first.
 *
 * @param idx  the peak (On Tour peakID)
 * @param numCompleted  number of completed big challenges (On Tour numCompletedChallenges)
 * @return the first medal (0 best to 3) whose target @p numCompleted reaches, or -1
 */
extern "C" int cBERewardInterface_calculateBigChallengeMedal(cBERewardInterface* self, int idx, int numCompleted)
{
    int* row = gBigChallengesMedalLevel + idx * 4;
    for (int i = 0; i < 4; i++) {
        if (numCompleted >= row[i]) return i;
    }
    return -1;
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_calculateEarningMedal);
#ifdef SKIP_ASM
/**
 * Returns the earnings medal for a cash total on a peak: 1 when it reaches the peak's target, else -1.
 *
 * The target is gEarningsMedalLevel[@p idx] (what cBERewardInterface_getEarningsTarget returns), compared unsigned.
 *
 * @param idx  the peak (On Tour peakID)
 * @param totalCash  total cash earned (On Tour totalCashEarned)
 * @return 1 when @p totalCash reaches the target, else -1
 */
extern "C" int cBERewardInterface_calculateEarningMedal(cBERewardInterface* self, int idx, unsigned int totalCash)
{
    return (totalCash < (unsigned int)gEarningsMedalLevel[idx]) ? -1 : 1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_calculateNextEarningsLevel);
#ifdef SKIP_ASM
int cBERewardInterface_isBetterMedal(void* self, int a, int b);

/**
 * Returns the next earnings target the character has not reached yet.
 *
 * Checks the per-peak earnings targets gEarningsMedalLevel (100,000, 250,000 and 1,000,000) in order against the econ
 * interface's total money earned.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @return the first unreached target (as a pointer-typed value), or 0 when all three are reached
 */
extern "C" void* cBERewardInterface_calculateNextEarningsLevel(cBERewardInterface* self, int iProfileIndex, int iCharacterID)
{
    for (int i = 0; i < 3; i++)
    {
        if (!cBERewardInterface_isBetterMedal(self, -1, cBERewardInterface_getEarningsMedal(self, iProfileIndex, iCharacterID, i)))
            return gEarningsMedalLevel[i];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_calculateNextCollectiblesLevel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cBERewardInterface_getCollectiblesMedal(cBERewardInterface* self, int a, int b, int kind);
extern int D_0045AFE8_2d[][4] __asm__("gCollectiblesMedalLevel");

/**
 * Returns the collectibles count needed for the next better medal on a peak.
 *
 * Takes the smallest target in the peak's row of gCollectiblesMedalLevel among the medals better than the current
 * collectibles medal.
 *
 * @param iProfileIndex     profile index (On Tour: iProfileIndex)
 * @param iCharacterID     character id (On Tour: iCharacterID)
 * @param kind  peak id
 * @return the next target, or 0 when the best medal is already held
 */
extern "C" int cBERewardInterface_calculateNextCollectiblesLevel(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int kind)
{
    int medal = cBERewardInterface_getCollectiblesMedal(self, iProfileIndex, iCharacterID, kind);
    int best = 0;
    for (int i = 0; i < 4; i++)
    {
        if (cBERewardInterface_isBetterMedal(self, medal, i))
        {
            if (best == 0 || D_0045AFE8_2d[kind][i] < best)
                best = D_0045AFE8_2d[kind][i];
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_calculateNextBigChallengesLevel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cBERewardInterface_getBigChallengesMedal(cBERewardInterface* self, int a, int b, int kind);
extern int D_0045B018_2d[][4] __asm__("gBigChallengesMedalLevel");

/**
 * Returns the completed big-challenge count needed for the next better medal on a peak.
 *
 * Takes the smallest target in the peak's row of gBigChallengesMedalLevel among the medals better than the current
 * big-challenges medal.
 *
 * @param iProfileIndex     profile index (On Tour: iProfileIndex)
 * @param iCharacterID     character id (On Tour: iCharacterID)
 * @param kind  peak id
 * @return the next target, or 0 when the best medal is already held
 */
extern "C" int cBERewardInterface_calculateNextBigChallengesLevel(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int kind)
{
    int medal = cBERewardInterface_getBigChallengesMedal(self, iProfileIndex, iCharacterID, kind);
    int best = 0;
    for (int i = 0; i < 4; i++)
    {
        if (cBERewardInterface_isBetterMedal(self, medal, i))
        {
            if (best == 0 || D_0045B018_2d[kind][i] < best)
                best = D_0045B018_2d[kind][i];
        }
    }
    return best;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_clearUserAwards);
#ifdef SKIP_ASM
int cBELibrary_getRiderIndex(int);
extern void* gNextEarningsLimit;
extern int gNextCollectiblesLimit[];
extern int gNextBigChallengesLimit[];
extern void* cBELibrary_userAwards[];
extern "C" void cBEUserAwards_clear(cBEUserAwards* self);

/**
 * Clears the awards log and caches the next earnings, collectibles and big-challenge targets.
 *
 * Clears cBELibrary_userAwards (cBEUserAwards_clear()) and computes the targets for rider 0's character on profile 0
 * into gNextEarningsLimit, gNextCollectiblesLimit[3] and gNextBigChallengesLimit[3]. Then zeroes +0x10 and stores -1 at +0x14 of
 * cBERewardInterface_instance (not of @p self).
 */
extern "C" void cBERewardInterface_clearUserAwards(cBERewardInterface* self)
{
    cBEUserAwards_clear((cBEUserAwards*)cBELibrary_userAwards);
    int charID = cBELibrary_getCharacterID(cBELibrary_getRiderIndex(0));
    gNextEarningsLimit = cBERewardInterface_calculateNextEarningsLevel(self, 0, charID);
    for (int i = 0; i < 3; i++)
        gNextCollectiblesLimit[i] = cBERewardInterface_calculateNextCollectiblesLevel(self, 0, charID, i);
    for (int i = 0; i < 3; i++)
        gNextBigChallengesLimit[i] = cBERewardInterface_calculateNextBigChallengesLevel(self, 0, charID, i);
    cBERewardInterface* rewardIface = cBERewardInterface_instance;
    rewardIface->mLastEventCashReward = 0;
    rewardIface->mLastEventMedal = -1;
}
#endif

extern void* cBELibrary_userAwards[];

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getUserAwards__FPv);
#ifdef SKIP_ASM
/**
 * Returns the table of awards the players have earned.
 *
 * @return cBELibrary_userAwards
 */
void* cBERewardInterface_getUserAwards(void* self)
{
    return (void*)cBELibrary_userAwards;
}
#endif

extern "C" void* cBEUserAwards_getTotalNumAwards(void*);

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumUserAwards__FPv);
#ifdef SKIP_ASM
/**
 * Returns how many awards the players have earned in total.
 *
 * cBEUserAwards_getTotalNumAwards (cBEUserAwards::getTotalNumAwards) on cBELibrary_userAwards. The C declares the result as void *; it is
 * the int count.
 *
 * @return the award count
 */
void* cBERewardInterface_getTotalNumUserAwards(void* self)
{
    return cBEUserAwards_getTotalNumAwards((void*)cBELibrary_userAwards);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getAwardedCash);
#ifdef SKIP_ASM
/**
 * Returns the cash recorded in the user-awards log: the first word of cBELibrary_userAwards (On Tour
 * tUserAwardsData.iCash).
 *
 * @return the awarded cash
 */
extern "C" void *cBERewardInterface_getAwardedCash(void) {
    return *cBELibrary_userAwards;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_awardUserPeakPass);
#ifdef SKIP_ASM
struct sReward_0015A628;
extern "C" void cBEUserAwards_setAward(sReward_0015A628* self, int row, int col, int value);
extern void* cBELibrary_userAwards[];
extern int cBELibrary_profileLib[];

// PORT: 64-bit `long` flags word (ld/sd).
struct sCharRec_158F60 {
    char pad0[0x278];
    long flags;     // 0x278
    char pad280[0xF88 - 0x280];
};

/**
 * Unlocks peak 1 (mode 1) or peaks 2 and 1 (mode 2) for the character and logs each newly unlocked peak as a peak-pass
 * award.
 *
 * Mode 2 clears bit 13 of the 64-bit flags at +0x278 of the character record (cBELibrary_profileLib + profile * 0x9B50
 * + character * 0xF88) when set and logs it with cBEUserAwards_setAward(cBELibrary_userAwards, @p row, 2, character * 3 + 2); it
 * then falls through to mode 1, which does the same for bit 12 with character * 3 + 1. Mode 0 does nothing. Bits 12 and
 * 13 are the locks cBERewardInterface_isPeakLocked1 tests. cBERewardInterface_awardUser calls it for peak 0's trophy categories (mode 1)
 * and peak 1's (mode 2).
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param row  the award category the log entry goes under (On Tour awardCategory)
 * @param mode  which peaks to unlock: 1 or 2
 */
extern "C" void cBERewardInterface_awardUserPeakPass(cBERewardInterface* self, int profile, int characterId, int row, int mode)
{
    sReward_0015A628* awards = (sReward_0015A628*)cBELibrary_userAwards;
    switch (mode)
    {
    case 0:
        break;
    case 2:
        if ((int)((*(sCharRec_158F60*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + characterId * 0xF88))).flags >> 13) & 1)
        {
            (*(sCharRec_158F60*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + characterId * 0xF88))).flags &= ~0x2000L;
            cBEUserAwards_setAward(awards, row, 2, characterId * 3 + 2);
        }
    case 1:
        if ((int)((*(sCharRec_158F60*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + characterId * 0xF88))).flags >> 12) & 1)
        {
            (*(sCharRec_158F60*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + characterId * 0xF88))).flags &= ~0x1000L;
            cBEUserAwards_setAward(awards, row, 2, characterId * 3 + 1);
        }
        break;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getAwardTrophyCategory);
#ifdef SKIP_ASM
/**
 * Maps a peak's trophy to its award category number.
 *
 * The category is 5 + 3 * @p tier + @p kind, so 5-16 for peaks 0-2 and trophies 0-3. The profile and character
 * arguments are not used.
 *
 * @param iProfileIndex    profile index (unused)
 * @param iCharacterID    character id (unused)
 * @param kind  peak id, 0-2
 * @param tier  trophy id, 0-3
 * @return the award category, or -1 for a peak or trophy id out of range
 */
extern "C" int cBERewardInterface_getAwardTrophyCategory(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int kind, int tier)
{
    int category = -1;
    switch (tier)
    {
    case 0:
        if (kind == 0)
            category = 5;
        else if (kind == 1)
            category = 6;
        else if (kind == 2)
            category = 7;
        break;
    case 1:
        if (kind == 0)
            category = 8;
        else if (kind == 1)
            category = 9;
        else if (kind == 2)
            category = 10;
        break;
    case 2:
        if (kind == 0)
            category = 11;
        else if (kind == 1)
            category = 12;
        else if (kind == 2)
            category = 13;
        break;
    case 3:
        if (kind == 0)
            category = 14;
        else if (kind == 1)
            category = 15;
        else if (kind == 2)
            category = 16;
        break;
    }
    return category;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_awardUserTrophy);
#ifdef SKIP_ASM
extern "C" int cBERewardInterface_isAwardReceived(cBERewardInterface* self, int a1, int a2, int bit);
extern "C" void cBERewardInterface_awardUser(cBERewardInterface* self, int a1, int a2, int bit);

/**
 * Awards a peak trophy to a character unless it was already awarded.
 *
 * Maps the trophy to its award category with cBERewardInterface_getAwardTrophyCategory(), and calls
 * cBERewardInterface_awardUser() only when cBERewardInterface_isAwardReceived() finds that category's bit clear in the character's
 * award bits.
 *
 * @param iProfileIndex    profile index (On Tour: iProfileIndex)
 * @param iCharacterID    character id (On Tour: iCharacterID)
 * @param kind  peak id
 * @param tier  trophy id
 */
extern "C" void cBERewardInterface_awardUserTrophy(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int kind, int tier)
{
    int bit = cBERewardInterface_getAwardTrophyCategory(self, iProfileIndex, iCharacterID, kind, tier);
    if (cBERewardInterface_isAwardReceived(self, iProfileIndex, iCharacterID, bit) == 0)
    {
        cBERewardInterface_awardUser(self, iProfileIndex, iCharacterID, bit);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_calculateUserTrackMedalRewards);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
signed char cBERewardInterface_getTrackMedal(void* self, int b, int c, int d, int e);
int cBERewardInterface_isBetterMedal(void* self, int a, int b);
extern "C" int cBERewardInterface_isTrophyOwned(cBERewardInterface* self, int a, int b, int c, int i);
extern "C" void cBERewardInterface_awardUserCash1(cBERewardInterface* self, int a, int b, int c);
extern "C" int cBERewardInterface_isMountainConquered(cBERewardInterface* self, int a, int b);
extern "C" unsigned int cBEEconInterface_getCashReward(void* score, int medal, int event, int mode);
extern "C" void func_001E1DD0(int group, int mode, int flag);
extern "C" void func_001E1C10(int group);
extern "C" void func_001E2370(int group);
extern "C" int cBERewardInterface_getTrackTrophyID(cBERewardInterface* self, int a, int b);
extern "C" int cBERewardInterface_getTrackPeakID(cBERewardInterface* self, int a1);

// PORT: 64-bit `long` flags word (ld/sd).
struct sProfileChar_1591E8 {
    char pad0[0x278];
    long flags;     // 0x278
    char pad280[0xF88 - 0x280];
};
extern sProfileChar_1591E8 D_004A6CA8_1591E8[][10] __asm__("cBELibrary_profileLib");
extern "C" int cBECharProfileDB_updateMedalStats(sProfileChar_1591E8* pc, int event, int mode, int medal);

struct sScoreStat_1591E8 {
    char pad0;
    signed char medal;      // 0x1
    unsigned short count;   // 0x2
};
sScoreStat_1591E8* cBECharProfileDB_getScoreStats_1591E8(void* self, int a, int b) __asm__("cBECharProfileDB_getScoreStats__FPvii");

struct sRewardReq_1591E8
{
    signed char track;
    signed char kind;
};
extern sRewardReq_1591E8 D_0045AAD8_1591E8[][4][5] __asm__("gMedalsInfo");

struct sRaceSettings_1591E8 {
    char pad_0x00[0x49];
    signed char b49;    // 0x49
};
extern sRaceSettings_1591E8 D_005305B0_1591E8 __asm__("cBELibrary_raceDB");
extern signed char D_005305F9[];


/**
 * Hands out the rewards for an event result: medal cash, the award category, trophies, and the unlocks of the peak's
 * next events.
 *
 * The peak comes from cBERewardInterface_getTrackPeakID (the location's peak; events 6/9, 7/10 and 8/11 force peaks 0, 1 and 2) and the
 * trophy from cBERewardInterface_getTrackTrophyID. The cash is cBEEconInterface_getCashReward(economy interface, @p medal, @p event, @p mode). The award
 * category is the peak plus 17 (events 0-3), 20 (4), 23 (5), 26 (6-8) or 29 (9-11). A medal of 0 or 1 calls
 * cBERewardInterface_awardUser for it on the first result (score-record count 0); events 6-11 with medal 3 or 2 call
 * func_001E1DD0 instead, unless raceDB byte +0x49 (D_005305F9) is set.
 *
 * When the medal beats the stored one (cBERewardInterface_isBetterMedal), pays the full cash (cBERewardInterface_awardUserCash1), records
 * the medal (cBECharProfileDB_updateMedalStats) and gives the trophy with cBERewardInterface_awardUserTrophy if it became owned; otherwise
 * records the medal and pays half. A conquered mountain gets award 0 once. Stores the medal and cash in mLastEventMedal
 * and mLastEventCashReward of cBERewardInterface_instance.
 *
 * With any medal it then walks the trophy's five requirements in gMedalsInfo[peak][trophy] (event byte, location byte;
 * event -1 ends) and checks each for a medal with cBERewardInterface_getTrackMedal. If they hold, it clears a lock bit
 * in the character's flags (+0x278: 0x40/0x80/0x100 for trophy 0, 0x200/0x400/0x800 otherwise, by peak) and calls
 * func_001E2370; the requirements on events 4 and 5 gate a second set of bits (0x4000-0x80000). Events 12 and 13 leave
 * the category at -1, which would still reach cBERewardInterface_awardUser (possible bug).
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param charID  character ID (On Tour iCharacterID)
 * @param mode  the game event, 0 to 13 (On Tour eventID)
 * @param event  the location (On Tour locationID)
 * @param medal  the medal earned, 0 best to 3, -1 none (On Tour medalID)
 */
extern "C" void cBERewardInterface_calculateUserTrackMedalRewards(cBERewardInterface* self, int profile, int charID, int mode, int event, int medal)
{
    sProfileChar_1591E8* pc = &D_004A6CA8_1591E8[profile][charID];
    int bit = -1;
    void* score = cBE_getInterface_Fv(cBE_getBE(), 11);
    sScoreStat_1591E8* stats = cBECharProfileDB_getScoreStats_1591E8(pc, event, mode);
    int tier = cBERewardInterface_getTrackTrophyID(self, mode, event);
    int group = cBERewardInterface_getTrackPeakID(self, event);
    unsigned int money;
    int oldMedal = stats->medal;
    if (mode == 6 || mode == 9)
        group = 0;
    else if (mode == 7 || mode == 10)
        group = 1;
    else if (mode == 8 || mode == 11)
        group = 2;
    money = cBEEconInterface_getCashReward(score, medal, event, mode);
    switch (mode) {
    case 0:
    case 1:
    case 2:
    case 3:
        bit = group + 17;
        break;
    case 4:
        bit = group + 20;
        break;
    case 5:
        bit = group + 23;
        break;
    case 6:
    case 7:
    case 8:
        bit = group + 26;
        break;
    case 9:
    case 10:
    case 11:
        bit = group + 29;
        break;
    case 12:
    case 13:
        break;
    }
    if ((unsigned int)medal < 2) {
        if (stats->count == 0)
            cBERewardInterface_awardUser((cBERewardInterface*)self, profile, charID, bit);
    } else if ((unsigned int)(mode - 6) < 6 && D_005305F9[0] == 0) {
        if (medal == 3)
            func_001E1DD0(group, mode, 0);
        else if (medal == 2)
            func_001E1DD0(group, mode, 1);
    }
    if (cBERewardInterface_isBetterMedal(self, oldMedal, medal)) {
        int had = cBERewardInterface_isTrophyOwned((cBERewardInterface*)self, profile, charID, group, tier);
        cBERewardInterface_awardUserCash1(self, profile, charID, money);
        cBECharProfileDB_updateMedalStats(pc, event, mode, medal);
        if (cBERewardInterface_isTrophyOwned((cBERewardInterface*)self, profile, charID, group, tier) && !had)
            cBERewardInterface_awardUserTrophy((cBERewardInterface*)self, profile, charID, group, tier);
    } else {
        cBECharProfileDB_updateMedalStats(pc, event, mode, medal);
        money >>= 1;
        cBERewardInterface_awardUserCash1(self, profile, charID, money);
    }
    if (cBERewardInterface_isMountainConquered((cBERewardInterface*)self, profile, charID) && cBERewardInterface_isAwardReceived(self, profile, charID, 0) == 0)
        cBERewardInterface_awardUser((cBERewardInterface*)self, profile, charID, 0);
    (cBERewardInterface_instance)->mLastEventMedal = medal;
    (cBERewardInterface_instance)->mLastEventCashReward = money;
    if (!cBERewardInterface_isBetterMedal(self, -1, medal))
        return;
    int any = 0;
    int okA = 1;
    int okB = 1;
    for (int i = 0; i < 5; i++) {
        int kind = D_0045AAD8_1591E8[group][tier][i].kind;
        int track = D_0045AAD8_1591E8[group][tier][i].track;
        if (track == -1) {
            if (D_005305B0_1591E8.b49 == 0)
                func_001E1C10(group);
            break;
        }
        if ((unsigned int)(track - 4) < 2) {
            any = 1;
            if (!cBERewardInterface_isBetterMedal(self, -1, cBERewardInterface_getTrackMedal(self, profile, charID, track, kind))) {
                okB = 0;
                break;
            }
        } else {
            if (!cBERewardInterface_isBetterMedal(self, -1, cBERewardInterface_getTrackMedal(self, profile, charID, track, kind))) {
                if (!any) {
                    if (D_005305B0_1591E8.b49 == 0)
                        func_001E1C10(group);
                    okA = 0;
                    okB = 0;
                }
                break;
            }
        }
    }
    if (okA) {
        if (tier == 0) {
            if (group == 0)
                pc->flags &= ~0x40L;
            else if (group == 1)
                pc->flags &= ~0x80L;
            else if (group == 2)
                pc->flags &= ~0x100L;
        } else {
            if (group == 0)
                pc->flags &= ~0x200L;
            else if (group == 1)
                pc->flags &= ~0x400L;
            else if (group == 2)
                pc->flags &= ~0x800L;
        }
        if (D_005305F9[0] == 0)
            func_001E2370(group);
    }
    if (okB) {
        if (tier == 0) {
            if (group == 0)
                pc->flags &= ~0x4000L;
            else if (group == 1)
                pc->flags &= ~0x8000L;
            else if (group == 2)
                pc->flags &= ~0x10000L;
        } else {
            if (group == 0)
                pc->flags &= ~0x20000L;
            else if (group == 1)
                pc->flags &= ~0x40000L;
            else if (group == 2)
                pc->flags &= ~0x80000L;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_awardUserCash);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern "C" void cBERewardInterface_awardUserCash1(cBERewardInterface* self, int a, int b, int c);

/**
 * Gives a race rider cash: converts the rider to profile and character and calls cBERewardInterface_awardUserCash1.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param money  the amount (On Tour cash)
 */
extern "C" void cBERewardInterface_awardUserCash(cBERewardInterface* self, int rider, int money)
{
    int profile = cBELibrary_getProfileIndex(rider);
    cBERewardInterface_awardUserCash1(self, profile, cBELibrary_getCharacterID(rider), money);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_awardUserCash1);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEEconInterface_getTotalMoneyEarned(void* self, int a, int b);
extern "C" int cBERewardInterface_isTrophyOwned(cBERewardInterface* self, int a, int b, int c, int i);
extern "C" void cBEEconInterface_addMoney(void* econ, int a, int b, int amount);
extern "C" void* cBERewardInterface_calculateNextEarningsLevel(cBERewardInterface* self, int a, int b);
extern "C" void cBERewardInterface_awardUserTrophy(cBERewardInterface* self, int a1, int a2, int kind, int tier);
extern void* gNextEarningsLimit;
extern void* cBELibrary_userAwards[];

/**
 * Gives a character cash, logging it and awarding any earnings trophies the new total earns.
 *
 * Adds @p amount to the awards log's cash (the first word of cBELibrary_userAwards), then to the character's money and
 * total earned (cBEEconInterface_addMoney on the economy interface). When gNextEarningsLimit is non-zero and the total earned
 * (cBEEconInterface_getTotalMoneyEarned) plus @p amount reaches it, it notes trophy 3 of the three peaks
 * (cBERewardInterface_isTrophyOwned) before paying, moves the limit on with
 * cBERewardInterface_calculateNextEarningsLevel, and awards each trophy 3 that became owned
 * (cBERewardInterface_awardUserTrophy).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param amount  the cash (On Tour cash)
 */
extern "C" void cBERewardInterface_awardUserCash1(cBERewardInterface* self, int profileIndex, int characterId, int amount)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    *(int*)cBELibrary_userAwards += amount;
    if (gNextEarningsLimit != 0 && (unsigned int)(cBEEconInterface_getTotalMoneyEarned(econ, profileIndex, characterId) + amount) >= (unsigned int)gNextEarningsLimit)
    {
        int before[3];
        int i;
        for (i = 0; i < 3; i++)
            before[i] = cBERewardInterface_isTrophyOwned((cBERewardInterface*)self, profileIndex, characterId, i, 3);
        cBEEconInterface_addMoney(econ, profileIndex, characterId, amount);
        gNextEarningsLimit = cBERewardInterface_calculateNextEarningsLevel((cBERewardInterface*)self, profileIndex, characterId);
        for (i = 0; i < 3; i++)
        {
            if (before[i] == 0 && cBERewardInterface_isTrophyOwned((cBERewardInterface*)self, profileIndex, characterId, i, 3))
                cBERewardInterface_awardUserTrophy((cBERewardInterface*)self, profileIndex, characterId, i, 3);
        }
    }
    else
    {
        cBEEconInterface_addMoney(econ, profileIndex, characterId, amount);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_awardUserCollectible);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBEMissionInterface_getCurrentCollectForPeak(void* self, int rider, int owner);
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern "C" int cBERewardInterface_isMountainConquered(cBERewardInterface* self, int a, int b);
extern "C" int cBERewardInterface_isTrophyOwned(cBERewardInterface* self, int a, int b, int c, int i);
extern "C" int cBERewardInterface_calculateNextCollectiblesLevel(cBERewardInterface* self, int a, int b, int kind);
extern "C" void cBERewardInterface_awardUserTrophy(cBERewardInterface* self, int a1, int a2, int kind, int tier);
extern "C" int cBERewardInterface_isAwardReceived(cBERewardInterface* self, int a1, int a2, int bit);
extern "C" void cBERewardInterface_awardUser(cBERewardInterface* self, int a1, int a2, int bit);
extern int gNextCollectiblesLimit[];
extern int D_0045AFE8_2d[][4] __asm__("gCollectiblesMedalLevel");

/**
 * Checks a rider's collectibles count on a peak against the next target and awards what it unlocks.
 *
 * Does nothing unless the cached target gNextCollectiblesLimit[k] is positive and reached. Then the target moves to the
 * next level; when trophy 2 was owned and the count equals the peak's lowest target, the trophy is awarded; a
 * newly conquered mountain awards category 0.
 *
 * @param rider  the rider index (On Tour: iRider)
 * @param k  the peak id
 */
extern "C" void cBERewardInterface_awardUserCollectible(cBERewardInterface* self, int rider, int k)
{
    int cur = cBEMissionInterface_getCurrentCollectForPeak(cBE_getInterface_Fv(cBE_getBE(), 10), rider, k);
    int* nextLevel = &gNextCollectiblesLimit[k];
    if (*nextLevel <= 0)
        return;
    if (cur < *nextLevel)
        return;
    int prof = cBELibrary_getProfileIndex(rider);
    int ch = cBELibrary_getCharacterID(rider);
    int was = cBERewardInterface_isTrophyOwned(self, prof, ch, k, 2);
    *nextLevel = cBERewardInterface_calculateNextCollectiblesLevel(self, prof, ch, k);
    if (was && cur == D_0045AFE8_2d[k][3])
        cBERewardInterface_awardUserTrophy(self, prof, ch, k, 2);
    if (cBERewardInterface_isMountainConquered(self, prof, ch))
    {
        if (cBERewardInterface_isAwardReceived(self, prof, ch, 0) == 0)
            cBERewardInterface_awardUser(self, prof, ch, 0);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_awardUserBigChallenge);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeak(void* self, int owner);
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern "C" int cBERewardInterface_isMountainConquered(cBERewardInterface* self, int a, int b);
extern "C" int cBERewardInterface_isTrophyOwned(cBERewardInterface* self, int a, int b, int c, int i);
extern "C" int cBERewardInterface_calculateNextBigChallengesLevel(cBERewardInterface* self, int a, int b, int kind);
extern "C" void cBERewardInterface_awardUserTrophy(cBERewardInterface* self, int a1, int a2, int kind, int tier);
extern "C" int cBERewardInterface_isAwardReceived(cBERewardInterface* self, int a1, int a2, int bit);
extern "C" void cBERewardInterface_awardUser(cBERewardInterface* self, int a1, int a2, int bit);
extern int gNextBigChallengesLimit[];
extern int D_0045B018_2d[][4] __asm__("gBigChallengesMedalLevel");

/**
 * Checks a peak's completed big challenges after a mission and hands out the trophy and awards they earn.
 *
 * Does nothing until the missions completed on peak @p k (cBEMissionInterface_getNumberCompletedMissionsOnPeak) reach
 * the positive limit gNextBigChallengesLimit[@p k]. Then the limit moves on
 * (cBERewardInterface_calculateNextBigChallengesLevel) and, if the trophy-2 check passed before and the count equals
 * gBigChallengesMedalLevel[@p k][3], awards trophy 2 for the peak; a conquered mountain (cBERewardInterface_isMountainConquered) also
 * grants award 0 if not yet received (cBERewardInterface_isAwardReceived). Note that the trophy is given when isTrophyOwned was already true
 * before the update, the opposite of cBERewardInterface_awardUserCash1's 'newly owned' test; likely a bug or an inverted name.
 *
 * @param rider  the rider (On Tour iRider)
 * @param k  the peak (On Tour peakID)
 */
extern "C" void cBERewardInterface_awardUserBigChallenge(cBERewardInterface* self, int rider, int k)
{
    int cur = cBEMissionInterface_getNumberCompletedMissionsOnPeak(cBE_getInterface_Fv(cBE_getBE(), 10), k);
    int* nextLimit = &gNextBigChallengesLimit[k];
    if (*nextLimit <= 0)
        return;
    if (cur < *nextLimit)
        return;
    int prof = cBELibrary_getProfileIndex(rider);
    int ch = cBELibrary_getCharacterID(rider);
    int was = cBERewardInterface_isTrophyOwned((cBERewardInterface*)self, prof, ch, k, 2);
    *nextLimit = cBERewardInterface_calculateNextBigChallengesLevel((cBERewardInterface*)self, prof, ch, k);
    if (was && cur == D_0045B018_2d[k][3])
        cBERewardInterface_awardUserTrophy((cBERewardInterface*)self, prof, ch, k, 2);
    if (cBERewardInterface_isMountainConquered((cBERewardInterface*)self, prof, ch))
    {
        if (cBERewardInterface_isAwardReceived(self, prof, ch, 0) == 0)
            cBERewardInterface_awardUser((cBERewardInterface*)self, prof, ch, 0);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_awardUser1);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

/**
 * Gives a race rider an award category: converts the rider to profile and character and calls
 * cBERewardInterface_awardUser.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param bit  the award category (On Tour awardCategory)
 */
extern "C" void cBERewardInterface_awardUser1(cBERewardInterface* self, int rider, int bit)
{
    int profile = cBELibrary_getProfileIndex(rider);
    cBERewardInterface_awardUser((cBERewardInterface*)self, profile, cBELibrary_getCharacterID(rider), bit);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_awardUser);
#ifdef SKIP_ASM
extern "C" int cBERewardInterface_isAwardReceived(cBERewardInterface* self, int a1, int a2, int bit);
extern "C" void cBERewardInterface_setBit(cBERewardInterface* self, int bit, unsigned char* bits);
extern "C" void func_0014B560(void* stats, int profile, int c, int value, int flag);
extern "C" void func_001E2428(int which);
extern "C" void func_001E2518();
extern "C" void func_001E2578();
extern "C" void func_001E38B8(int profile);

struct sSlots4_00159CD0 {
    int v[4];
};
struct sSlots2_00159CD0 {
    int v[2];
};
extern sSlots4_00159CD0 D_0045AA28;
extern sSlots2_00159CD0 D_004A1268[];

/**
 * Gives a character an award category's rewards, logs them, and awards anything that unlocks as a result.
 *
 * Except for categories 17-19 an award already given is skipped. Categories 0-4 (mountain, peaks) call a
 * front-end notice (func_001E2578, func_001E2518, func_001E2428) and give a fixed reward; peak 0's trophies
 * give random rewards of kinds 4 and 3 and unlock peak 1, peak 1's give kinds 7 and 5 and unlock peaks 1 and
 * 2, peak 2's give a fixed reward; 17-31 give an item. The category bit is set (+0xF28), the rewards go into
 * the log cBELibrary_userAwards, and the mountain, peak and trophy awards are re-checked.
 *
 * @param profile  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param bit  the award category, 0-31 (On Tour: awardCategory)
 */
extern "C" void cBERewardInterface_awardUser(cBERewardInterface* self, int profile, int iCharacterID, int bit)
{
    char* rec = (char*)cBELibrary_profileLib + profile * 0x9B50 + iCharacterID * 3976;
    sReward_0015A628* rewardLog = (sReward_0015A628*)cBELibrary_userAwards;
    sSlots4_00159CD0 slots4 = D_0045AA28;
    sSlots2_00159CD0 slots2 = D_004A1268[0];
    int item = -1;
    int value = 0;

    if (bit < 17 || bit > 19) {
        if (cBERewardInterface_isAwardReceived(self, profile, iCharacterID, bit))
            return;
    }

    switch (bit) {
    case 0:
        func_001E2578();
        value = 29;
        break;
    case 1:
        func_001E2518();
        value = 0x18;
        break;
    case 2:
        func_001E2428(0);
        value = 18;
        break;
    case 3:
        func_001E2428(1);
        value = 25;
        break;
    case 4:
        func_001E2428(2);
        value = 26;
        break;
    case 5:
    case 8:
    case 11:
    case 14:
    {
        int i;
        for (i = 0; i < 4 && (slots4.v[i] = cBERewardInterface_getRandomLockedRewardIndex(self, profile, iCharacterID, 4)) >= 0; i++) {
            cBERewardInterface_buyTradingCard(self, profile, iCharacterID, slots4.v[i], 0);
            cBEUserAwards_setAward(rewardLog, bit, 4, slots4.v[i]);
        }
        int rewardBit = cBERewardInterface_getRandomLockedRewardIndex(self, profile, iCharacterID, 3);
        if (rewardBit >= 0) {
            cBERewardInterface_buyPoster(self, profile, iCharacterID, rewardBit, 0);
            cBEUserAwards_setAward(rewardLog, bit, 3, rewardBit);
        }
        if (bit == 11)
            func_001E38B8(profile);
        cBERewardInterface_awardUserPeakPass(self, profile, iCharacterID, bit, 1);
        break;
    }
    case 6:
    case 9:
    case 12:
    case 15:
    {
        int i;
        int rewardBit = cBERewardInterface_getRandomLockedRewardIndex(self, profile, iCharacterID, 7);
        if (rewardBit >= 0) {
            cBERewardInterface_buyToy(self, profile, iCharacterID, rewardBit, 0);
            cBEUserAwards_setAward(rewardLog, bit, 7, rewardBit);
        }
        for (i = 0; i < 2 && (slots2.v[i] = cBERewardInterface_getRandomLockedRewardIndex(self, profile, iCharacterID, 5)) >= 0; i++) {
            cBERewardInterface_buyConceptArt(self, profile, iCharacterID, slots2.v[i], 0);
            cBEUserAwards_setAward(rewardLog, bit, 5, slots2.v[i]);
        }
        if (bit == 12)
            func_001E38B8(profile);
        cBERewardInterface_awardUserPeakPass(self, profile, iCharacterID, bit, 2);
        break;
    }
    case 7:
        value = 27;
        break;
    case 10:
        value = 23;
        break;
    case 13:
        value = 0x1C;
        break;
    case 16:
        value = 21;
        break;
    case 17:
        item = func_00156C70(self, profile, iCharacterID, 0);
        break;
    case 18:
        item = func_00156C70(self, profile, iCharacterID, 1);
        break;
    case 19:
        item = func_00156C70(self, profile, iCharacterID, 2);
        break;
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
        item = func_00156EE0(self, profile, iCharacterID);
        break;
    }

    cBERewardInterface_setBit(self, bit, (unsigned char*)(rec + 0xF28));
    if (value != 0) {
        cBEUserAwards_setAward(rewardLog, bit, 8, value);
        cBERewardInterface_buyCheatChar(self, profile, iCharacterID, value, 0);
    }
    if (item != -1) {
        void* stats = cBE_getInterface_Fv(cBE_getBE(), 9);
        cBEUserAwards_setAward(rewardLog, bit, 9, item);
        func_0014B560(stats, profile, iCharacterID, item, 0);
    }
    if (cBERewardInterface_isMountainConquered(self, profile, iCharacterID)) {
        if (!cBERewardInterface_isAwardReceived(self, profile, iCharacterID, 0))
            cBERewardInterface_awardUser(self, profile, iCharacterID, 0);
    }
    if (cBERewardInterface_isMountainCompleted(self, profile, iCharacterID)) {
        if (!cBERewardInterface_isAwardReceived(self, profile, iCharacterID, 1))
            cBERewardInterface_awardUser(self, profile, iCharacterID, 1);
    }
    for (int i = 0; i < 3; i++) {
        if (cBERewardInterface_isPeakCompleted(self, profile, iCharacterID, i)) {
            if (!cBERewardInterface_isAwardReceived(self, profile, iCharacterID, i + 2))
                cBERewardInterface_awardUser(self, profile, iCharacterID, i + 2);
        }
    }
    for (int kind = 0; kind < 3; kind++) {
        for (int tier = 0; tier < 4; tier++) {
            int trophyCategory = cBERewardInterface_getAwardTrophyCategory(self, profile, iCharacterID, kind, tier);
            if (cBERewardInterface_isTrophyOwned(self, profile, iCharacterID, kind, tier)) {
                if (!cBERewardInterface_isAwardReceived(self, profile, iCharacterID, trophyCategory))
                    cBERewardInterface_awardUserTrophy(self, profile, iCharacterID, kind, tier);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isAwardReceived);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];
extern "C" int cBERewardInterface_isBitSet(cBERewardInterface* self, int bit, unsigned char* bits);

/**
 * Tells whether a character profile has already received an award.
 *
 * cBERewardInterface_isBitSet (On Tour isBitSet) on bit @p bit of the bytes at +0xF28 of cBELibrary_profileLib[@p iProfileIndex][@p iCharacterID].
 *
 * @param iProfileIndex  the profile (On Tour iProfileIndex)
 * @param iCharacterID  the character (On Tour iCharacterID)
 * @param bit  the award (On Tour awardCategory)
 * @return nonzero when received
 */
extern "C" int cBERewardInterface_isAwardReceived(cBERewardInterface* self, int iProfileIndex, int iCharacterID, int bit)
{
    char* profileRec = (char*)cBELibrary_profileLib + iProfileIndex * 0x9b50 + iCharacterID * 3976;
    return cBERewardInterface_isBitSet(self, bit, (unsigned char*)(profileRec + 0xF28));
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isBitSet);
#ifdef SKIP_ASM
/**
 * Returns bit @p bit of a bit array (byte bit / 8, bit bit % 8).
 *
 * cBERewardInterface_isAwardReceived uses it on the award-category bits at +0xF28 of the character record.
 *
 * @param bit  bit index (On Tour bitIndex)
 * @param bits  the bit array (On Tour pBitsArray)
 * @return 1 when set, else 0
 */
extern "C" int cBERewardInterface_isBitSet(cBERewardInterface* self, int bit, unsigned char* bits)
{
    int shift = bit % 8;
    return (bits[(unsigned int)bit / 8] >> shift) & 1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_setBit);
#ifdef SKIP_ASM
/**
 * Sets bit @p bit of a bit array (byte bit / 8, bit bit % 8).
 *
 * cBERewardInterface_awardUser uses it to mark an award category received (+0xF28 of the character record).
 *
 * @param bit  bit index (On Tour bitIndex)
 * @param bits  the bit array (On Tour pBitsArray)
 */
extern "C" void cBERewardInterface_setBit(cBERewardInterface* self, int bit, unsigned char* bits)
{
    int shift = bit % 8;
    bits[(unsigned int)bit / 8] |= 1 << shift;
}
#endif

// 0x64-byte array elements; arr[i].field indexing reproduces the target's
// base-first addu
struct sBERewardEntry {
    char pad_0x00[0x54];
    int field_0x54;
    char pad_0x58[0xc];
};
extern sBERewardEntry cBELibrary_locationDB[];

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrackPeakID);
#ifdef SKIP_ASM
/**
 * Returns the peak a location belongs to: the word at +0x54 of its 0x64-byte cBELibrary_locationDB entry.
 *
 * @param locationID  the location (On Tour locationID)
 * @return the peak ID
 */
extern "C" int cBERewardInterface_getTrackPeakID(cBERewardInterface* self, int locationID)
{
    return cBELibrary_locationDB[locationID].field_0x54;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrackTrophyID);
#ifdef SKIP_ASM
struct sRewardPair_0015A3B8
{
    signed char a;
    signed char b;
};

extern sRewardPair_0015A3B8 gMedalsInfo[3][4][5];

/**
 * Returns which trophy of its peak an event at a location counts toward.
 *
 * Searches the requirement table gMedalsInfo[3 peaks][4 trophies][5] (event byte, location byte) for an entry with event
 * @p eventID and location @p locationID; for events 6 to 11 the event alone matches. Returns the trophy index of the first hit.
 *
 * @param eventID  the game event (On Tour eventID)
 * @param locationID  the location (On Tour locationID)
 * @return the trophy index, 0 to 3, or -1 when no entry matches
 */
extern "C" int cBERewardInterface_getTrackTrophyID(cBERewardInterface* self, int eventID, int locationID)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                if (gMedalsInfo[i][j][k].a == eventID)
                {
                    if (gMedalsInfo[i][j][k].b == locationID || eventID == 6 || eventID == 7 || eventID == 8 || eventID == 9 || eventID == 10 || eventID == 11)
                        return j;
                }
            }
        }
    }
    return -1;
}
#endif

extern void* cBELibrary_rewardDB[];

INCLUDE_ASM("be/beintreward", cBERewardInterface_getDB);
#ifdef SKIP_ASM
/**
 * Returns the reward database (posters, toys, cards, videos and so on).
 *
 * @return cBELibrary_rewardDB
 */
void* cBERewardInterface_getDB(cBERewardInterface* self)
{
    return (void*)cBELibrary_rewardDB;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBEUserAwards_cBEUserAwards);
#ifdef SKIP_ASM
extern "C" void cBEUserAwards_clear(cBEUserAwards* self);


/**
 * Constructs the awards log: points its per-kind bit sets into its own storage, then clears it.
 *
 * The pointers at +0x174 for kinds 0, 2, 3, 4, 5, 7 and 8 get areas inside the object (+0x146 to +0x170);
 * kinds 1, 6 and 9 stay 0.
 *
 * @return @p self
 */
extern "C" void* cBEUserAwards_cBEUserAwards(cBEUserAwards* self)
{
    for (int i = 0; i < 10; i++)
        ((char**)&self->mpAwardBits[0])[i] = 0;
    ((char**)&self->mpAwardBits[0])[0] = ((char*)self) + 0x146;
    ((char**)&self->mpAwardBits[0])[2] = ((char*)self) + 0x170;
    ((char**)&self->mpAwardBits[0])[3] = ((char*)self) + 0x147;
    ((char**)&self->mpAwardBits[0])[4] = ((char*)self) + 0x14D;
    ((char**)&self->mpAwardBits[0])[5] = ((char*)self) + 0x15C;
    ((char**)&self->mpAwardBits[0])[7] = ((char*)self) + 0x169;
    ((char**)&self->mpAwardBits[0])[8] = ((char*)self) + 0x16D;
    cBEUserAwards_clear(self);
    return (self);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBEUserAwards_clear);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Clears the awards log: zeroes its first 0x174 bytes and sets the short at +0x4 to -1.
 */
extern "C" void cBEUserAwards_clear(cBEUserAwards* self)
{
    MEM_fill(self, 0, 372);
    *(short*)((char*)self + 0x4) = -1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBEUserAwards_coutBits);
#ifdef SKIP_ASM
/**
 * Counts the set bits among the first @p n bits of a bit array (least significant bit of each byte first).
 *
 * Nothing calls it.
 *
 * @param n  number of bits to look at (On Tour nMaxBits)
 * @param bits  the bit array (On Tour pBitsArray)
 * @return the number of set bits
 */
extern "C" int cBEUserAwards_coutBits(cBEUserAwards* self, int n, unsigned char* bits)
{
    int i = 0;
    int bi = 0;
    int count = 0;
    while (i < n) {
        unsigned char bitByte = bits[bi];
        int j;
        bi++;
        for (j = 0; j < 8 && i < n; j++) {
            count += (bitByte >> j) & 1;
            i++;
        }
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBEUserAwards_setBit);
#ifdef SKIP_ASM
/**
 * Sets bit @p bit of a bit array (byte bit / 8, bit bit % 8).
 *
 * cBEUserAwards_setAward uses it on the awards log's per-type bits.
 *
 * @param bit  bit index (On Tour bitIndex)
 * @param bits  the bit array (On Tour pBitsArray)
 */
extern "C" void cBEUserAwards_setBit(cBEUserAwards* self, int bit, unsigned char* bits)
{
    int shift = bit % 8;
    bits[(unsigned int)bit / 8] |= 1 << shift;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBEUserAwards_isBitSet);
#ifdef SKIP_ASM
/**
 * Returns bit @p bit of a bit array (byte bit / 8, bit bit % 8).
 *
 * Nothing calls it.
 *
 * @param bit  bit index (On Tour bitIndex)
 * @param bits  the bit array (On Tour pBitsArray)
 * @return 1 when set, else 0
 */
extern "C" int cBEUserAwards_isBitSet(cBEUserAwards* self, int bit, unsigned char* bits)
{
    int shift = bit % 8;
    return (bits[(unsigned int)bit / 8] >> shift) & 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBEUserAwards_setAward);
#ifdef SKIP_ASM
struct sRewardRow_0015A628
{
    unsigned char counts[8]; // 0x0
    unsigned char value;     // 0x8
    unsigned char flag;      // 0x9
};

struct sReward_0015A628
{
    char pad_0x000[0x4];
    short mValue;                    // 0x4
    sRewardRow_0015A628 mRows[0x1D]; // 0x6
    char pad_0x128[0x174 - 0x128];
    unsigned char* mBits[10];        // 0x174
};

/**
 * Records one reward in the user-awards log under an award category.
 *
 * Columns 0-7 (reward types) set bit @p value in the type's bits (mBits[col] at +0x174, cBEUserAwards_setBit) and count it in
 * row @p row's counts (+0x6 + row * 10). Column 8 stores @p value as the row's byte 8 (a cheat character's id); column
 * 9 sets the row's byte 9 to 1 and the short at +0x4 (On Tour iCharBoltID) to @p value (an item id).
 * cBERewardInterface_awardUser and cBERewardInterface_awardUserPeakPass call it on cBELibrary_userAwards.
 *
 * @param row  the award category (On Tour awardCategory)
 * @param col  the reward type (On Tour rewardType)
 * @param value  the reward index or id (On Tour index)
 */
extern "C" void cBEUserAwards_setAward(sReward_0015A628* self, int row, int col, int value)
{
    switch (col)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        cBEUserAwards_setBit((cBEUserAwards*)self, value, self->mBits[col]);
        self->mRows[row].counts[col]++;
        break;
    case 8:
        self->mRows[row].value = value;
        break;
    case 9:
        self->mRows[row].flag = 1;
        self->mValue = value;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBEUserAwards_getTotalNumAwards);
#ifdef SKIP_ASM
struct sRewardGrid_0015A6F0
{
    char pad_0x00[0x6];
    signed char vals[32][10]; // 0x6 (cBEUserAwards_getNumAwards reads vals[a1][a2])
};

// PORT: the unit declares cBEUserAwards_getTotalNumAwards as returning void* (line ~378, used by
// cBERewardInterface_getTotalNumUserAwards); the body returns an int sum, so it is bound by asm label.
// The lander should change that declaration to int instead.
int cBEUserAwards_getTotalNumAwards_impl(sRewardGrid_0015A6F0* self) __asm__("cBEUserAwards_getTotalNumAwards");

/**
 * Counts all awards recorded in a user-awards table.
 *
 * Sums the 32 x 10 signed bytes starting at +0x6. The C definition is cBEUserAwards_getTotalNumAwards (bound to the symbol by asm
 * label) because the unit declares cBEUserAwards_getTotalNumAwards as returning void *.
 *
 * @return the total award count
 */
int cBEUserAwards_getTotalNumAwards_impl(sRewardGrid_0015A6F0* self)
{
    int sum = 0;
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            sum += self->vals[i][j];
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBEUserAwards_getNumAwards);
#ifdef SKIP_ASM
/**
 * Returns one count of the user-awards log: the signed byte for award category @p row and reward type @p column (+0x6 +
 * row * 10 + column).
 *
 * @param row  the award category (On Tour awardCategory)
 * @param column  the reward type (On Tour rewardType)
 * @return the count
 */
extern "C" signed char cBEUserAwards_getNumAwards(cBEUserAwards* self, int row, int column)
{
    return *(signed char*)((char*)self + (row * 0xa + column) + 0x6);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBEUserAwards_getAwardBits__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the user-awards log's bit array for one reward type: mBits[@p i] at +0x174 (On Tour mpAwardBits).
 *
 * @param i  the reward type (On Tour rewardType)
 * @return the bit array, or null for types without one
 */
int cBEUserAwards_getAwardBits(void* vself, int i)
{
    cBEUserAwards* self = (cBEUserAwards*)vself;
    return *(int*)((char*)self + (i << 2) + 0x174);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardDB_cBERewardDB);
#ifdef SKIP_ASM
/**
 * Constructs the reward database: sets the load handle (+0x0) to -1 and zeroes the table pointers and counts.
 *
 * Zeroes +0x4 to +0x24 and +0x2C to +0x4C. It skips +0x28 and +0x50, the cheat-character table and count that
 * cBERewardDB_initRewards fills and cBERewardInterface_getTotalNumCheatChars reads (an oversight; the one instance, cBELibrary_rewardDB, is a zeroed global
 * anyway).
 *
 * @return @p self
 */
extern "C" void* cBERewardDB_cBERewardDB(cBERewardDB* self)
{
    self->mpRewardsFile = 0;
    self->mRewardFileIndex = -1;
    self->mpTrophies = 0;
    self->mpMedals = 0;
    self->mpPeakPasses = 0;
    self->mpPosters = 0;
    self->mpTradingCards = 0;
    self->mpConceptArts = 0;
    self->mpToys = 0;
    self->mpVideos = 0;
    self->mpStrings = 0;
    self->mNumTrophies = 0;
    self->mNumMedals = 0;
    self->mNumPeakPasses = 0;
    self->mNumPosters = 0;
    self->mNumTradingCards = 0;
    self->mNumConceptArts = 0;
    self->mNumToys = 0;
    self->mNumVideos = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardDB__cBERewardDB);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cBERewardDB_purge(void* self);

/**
 * Destroys the reward database: frees its data with cBERewardDB_purge and deletes the object when bit 0 of @p flags is set.
 *
 * @param flags  destructor flags; bit 0 frees the object
 */
extern "C" void cBERewardDB__cBERewardDB(int* self, int flags)
{
    cBERewardDB_purge(self);
    if (flags & 1)
    {
        operator_delete(self);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardDB_initRewards);
#ifdef SKIP_ASM
struct sRewardStr3 {
    char* s[3];
};
struct sRewardStr2 {
    char* s[2];
};
struct sRewardStr4 {
    char* s[3];
    int pad;
};
struct sRewardData {
    int unk0;
    int* base;
    sRewardStr3* t0;
    sRewardStr3* t1;
    sRewardStr2* t2;
    sRewardStr4* t3;
    sRewardStr4* t4;
    sRewardStr4* t5;
    sRewardStr4* t7;
    sRewardStr4* t6;
    sRewardStr4* t8;
    char* strings;
    int n0;
    int n1;
    int n2;
    int n3;
    int n4;
    int n5;
    int n7;
    int n6;
    int n8;
};

#define FIXUP(e) if (e) e = self->strings + ((int)e - 1)

/**
 * Parses a loaded reward file into the reward database's tables and turns its string offsets into pointers.
 *
 * Stores @p data at +0x4 (On Tour mpRewardsFile). The file starts with nine (type, count) word pairs, each followed by
 * its records: type 0 trophies and 1 medals (3 string words each), 2 peak passes (2), 3 posters, 4 trading cards, 5
 * concept arts, 6 videos, 7 toys and 8 cheat characters (16-byte records, 3 strings). The tables and counts land at On
 * Tour cBERewardDB's mpTrophies..mpCheatChars (+0x8 to +0x28) and mNumTrophies..mNumCheatChars (+0x30 to +0x50).
 *
 * After the pairs comes one word, then the string pool (+0x2C, mpStrings). Every non-zero string word is an offset + 1
 * into the pool and becomes strings + (offset - 1). cBERewardDB_init calls it after a synchronous load and
 * cBERewardDB_update calls it once an asynchronous load has finished.
 *
 * @param data  the loaded reward file (On Tour pFileData)
 */
extern "C" void cBERewardDB_initRewards(sRewardData* self, int* data)
{
    int n;
    int* cursor = data;
    self->base = data;
    for (n = 0; n < 9; n++) {
        unsigned int type = *cursor++;
        int count = *cursor++;
        if (count != 0) {
            switch (type) {
            case 0:
                self->t0 = (sRewardStr3*)cursor;
                self->n0 = count;
                cursor = (int*)((sRewardStr3*)cursor + count);
                break;
            case 1:
                self->t1 = (sRewardStr3*)cursor;
                self->n1 = count;
                cursor = (int*)((sRewardStr3*)cursor + count);
                break;
            case 2:
                self->t2 = (sRewardStr2*)cursor;
                self->n2 = count;
                cursor = (int*)((sRewardStr2*)cursor + count);
                break;
            case 3:
                self->t3 = (sRewardStr4*)cursor;
                self->n3 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            case 4:
                self->t4 = (sRewardStr4*)cursor;
                self->n4 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            case 5:
                self->t5 = (sRewardStr4*)cursor;
                self->n5 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            case 6:
                self->t6 = (sRewardStr4*)cursor;
                self->n6 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            case 7:
                self->t7 = (sRewardStr4*)cursor;
                self->n7 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            case 8:
                self->t8 = (sRewardStr4*)cursor;
                self->n8 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            }
        }
    }
    cursor++;
    self->strings = (char*)cursor;
    for (int i = 0; i < self->n0; i++) {
        sRewardStr3* e = &self->t0[i];
        FIXUP(e->s[0]);
        FIXUP(e->s[1]);
        FIXUP(e->s[2]);
    }
    for (int i = 0; i < self->n1; i++) {
        sRewardStr3* e = &self->t1[i];
        FIXUP(e->s[0]);
        FIXUP(e->s[1]);
        FIXUP(e->s[2]);
    }
    for (int i = 0; i < self->n2; i++) {
        sRewardStr2* e = &self->t2[i];
        FIXUP(e->s[0]);
        FIXUP(e->s[1]);
    }
    for (int i = 0; i < self->n3; i++) {
        sRewardStr4* e = &self->t3[i];
        FIXUP(e->s[0]);
        FIXUP(e->s[1]);
        FIXUP(e->s[2]);
    }
    for (int i = 0; i < self->n4; i++) {
        sRewardStr4* e = &self->t4[i];
        FIXUP(e->s[0]);
        FIXUP(e->s[1]);
        FIXUP(e->s[2]);
    }
    for (int i = 0; i < self->n5; i++) {
        sRewardStr4* e = &self->t5[i];
        FIXUP(e->s[0]);
        FIXUP(e->s[1]);
        FIXUP(e->s[2]);
    }
    for (int i = 0; i < self->n6; i++) {
        sRewardStr4* e = &self->t6[i];
        FIXUP(e->s[0]);
        FIXUP(e->s[1]);
        FIXUP(e->s[2]);
    }
    for (int i = 0; i < self->n7; i++) {
        sRewardStr4* e = &self->t7[i];
        FIXUP(e->s[0]);
        FIXUP(e->s[1]);
        FIXUP(e->s[2]);
    }
    for (int i = 0; i < self->n8; i++) {
        sRewardStr4* e = &self->t8[i];
        FIXUP(e->s[0]);
        FIXUP(e->s[1]);
        FIXUP(e->s[2]);
    }
}
#endif

