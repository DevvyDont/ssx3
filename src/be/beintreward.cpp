#include "common.h"
#include "vtable.h"
#include "be/beintnewrace.h"
#include "be/beprofilelib.h"
#include "be/bedbprofile.h"
#include "sound/ssxaudio.h"
#include "be/beuserawards.h"
#include "be/berewarddb.h"
#include "be/beintreward.h"
#include "fe/feboltfilemanager.h"

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
        VPTR_OF(cBEInterface, mem) = cBEInterface_vtbl;
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
 * True when the first word of cBELibrary_rewardDB (On Tour cBERewardDB.mRewardFileIndex, the load request id) is
 * kNoBoltFile (-1), which cBERewardDB_init stores after a synchronous load, and the second (mpRewardsFile) is non-null.
 *
 * @return 1 when loaded, else 0
 */
extern "C" int cBERewardInterface_isRewardsLoaded(void)
{
    int result = 0;
    if ((int)cBELibrary_rewardDB[0] == kNoBoltFile) {
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
 * Medal ids (eBEMedalID) run from 0 (best) to 3 (lowest), and kMEDAL_NONE (-1) means no medal: no medal is never
 * better, and any medal is better than none.
 *
 * @param oldMedal  medal to compare against, or kMEDAL_NONE
 * @param newMedal  medal to test, or kMEDAL_NONE
 * @return true when @p newMedal is better than @p oldMedal
 */
bool cBERewardInterface::isBetterMedal(int oldMedal, int newMedal)
{
    if (newMedal == kMEDAL_NONE) {
        return false;
    }
    if (oldMedal == kMEDAL_NONE) {
        return true;
    }
    return newMedal < oldMedal;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrophy);
#ifdef SKIP_ASM
extern "C" cBERewardDB* cBERewardInterface_getDB(cBERewardInterface* self);

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
char* cBERewardInterface::getTrophy(int peakId, int trophyId)
{
    return (char*)&getDB()->mpTrophies[trophyId * 3 + peakId];
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getMedal);
#ifdef SKIP_ASM
extern "C" cBERewardDB* cBERewardInterface_getDB(cBERewardInterface* self);

/**
 * Returns the medal record for @p trophyId and @p medalId: the 12-byte entry trophyId * 4 + medalId of the reward
 * database's medal table (cBERewardInterface_getDB +0xC, On Tour's mpMedals).
 *
 * @param trophyId  first index (four medals per entry)
 * @param medalId  medal within it
 * @return the medal record
 */
char* cBERewardInterface::getMedal(int trophyId, int medalId)
{
    return (char*)&getDB()->mpMedals[trophyId * 4 + medalId];
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getPeakPass);
#ifdef SKIP_ASM
extern "C" cBERewardDB* cBERewardInterface_getDB(cBERewardInterface* self);

/**
 * Returns the peak pass record for @p charId and @p peakId: the 8-byte entry charId * 3 + peakId of the reward
 * database's peak pass table (cBERewardInterface_getDB +0x10, On Tour's mpPeakPasses).
 *
 * @param peakId  the peak, 0-2
 * @param charId  the character
 * @return the peak pass record
 */
char* cBERewardInterface::getPeakPass(int peakId, int charId)
{
    return (char*)&getDB()->mpPeakPasses[charId * 3 + peakId];
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrophies);
#ifdef SKIP_ASM
/**
 * Returns the reward database's trophy table (cBERewardInterface_getDB +0x8, On Tour's mpTrophies).
 *
 * @return the trophy records
 */
int cBERewardInterface::getTrophies()
{
    return ((int)getDB()->mpTrophies);
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getMedals);
#ifdef SKIP_ASM
/**
 * Returns the reward database's medal table (cBERewardInterface_getDB +0xC, On Tour's mpMedals).
 *
 * @return the medal records
 */
int cBERewardInterface::getMedals()
{
    return ((int)getDB()->mpMedals);
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getPeakPasses);
#ifdef SKIP_ASM
/**
 * Returns the reward database's peak pass table (cBERewardInterface_getDB +0x10, On Tour's mpPeakPasses).
 *
 * @return the peak pass records
 */
int cBERewardInterface::getPeakPasses()
{
    return ((int)getDB()->mpPeakPasses);
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getPosters);
#ifdef SKIP_ASM
/**
 * Returns the reward database's poster table (cBERewardInterface_getDB +0x14, On Tour's mpPosters).
 *
 * @return the poster records
 */
int cBERewardInterface::getPosters()
{
    return ((int)getDB()->mpPosters);
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTradingCards);
#ifdef SKIP_ASM
/**
 * Returns the reward database's trading card table (cBERewardInterface_getDB +0x18, On Tour's mpTradingCards).
 *
 * @return the trading card records
 */
int cBERewardInterface::getTradingCards()
{
    return ((int)getDB()->mpTradingCards);
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getConceptArts);
#ifdef SKIP_ASM
/**
 * Returns the reward database's concept-art table (cBERewardInterface_getDB +0x1C, On Tour's mpConceptArts).
 *
 * @return the concept-art records
 */
int cBERewardInterface::getConceptArts()
{
    return ((int)getDB()->mpConceptArts);
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getToys);
#ifdef SKIP_ASM
/**
 * Returns the reward database's toy table (cBERewardInterface_getDB +0x20, On Tour's mpToys).
 *
 * @return the toy records
 */
int cBERewardInterface::getToys()
{
    return ((int)getDB()->mpToys);
}
#endif
//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getVideos);
#ifdef SKIP_ASM
/**
 * Returns the reward database's video table (cBERewardInterface_getDB +0x24, On Tour's mpVideos).
 *
 * @return the video records
 */
int cBERewardInterface::getVideos()
{
    return ((int)getDB()->mpVideos);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00156C70);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
struct sBELibTables;
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(sBELibTables* self, int i);
unsigned int BXrand();


struct sBEEntry_6C70 {
    char pad_0x0[4];
    short id;           // 0x4
    char pad_0x6[0x2E];
    int flags;          // 0x34
};

static inline tBEProfileBolt* Flag_6C70(char* p, sBEEntry_6C70* e)
{
    short s = (*(short**)(p + 0x288))[e->id];
    if (s >= 0)
        return (tBEProfileBolt*)(p + 0x290) + s;
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
 * cBERewardInterface_awardUser uses it for award categories 17, 18 and 19 (the event medals of peaks 1-3: types
 * kPEAK_1, kPEAK_2, kPEAK_3), so the type is the peak whose bolt pool to draw from. The first pass tests byte
 * +0x35 bit 0, the same bit as flags 0x100. The second pass dereferences the state pointer without the null check the
 * first pass has.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param type  item type, the peak (eBEPeakID) 0 to 2 (flags 0x100, 0x200, 0x400)
 * @return the chosen item id, or -1 when there is none
 */
extern "C" int func_00156C70(cBERewardInterface* self, int profileIndex, int characterId, int type)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    int n = 0;
    void* lib = func_0014BDB8_noarg();
    int count = func_0014D988(lib, characterId);
    sBEEntry_6C70* def = (sBEEntry_6C70*)func_0014D998((sBELibTables*)lib, characterId);
    int i;
    for (i = 0; i < count; i++, def++)
    {
        if (type == kPEAK_1 && (*(unsigned char*)((char*)def + 0x35) & 1))
        {
            if ((Flag_6C70(((char*)charRecord), def)->flags & 2) == 0)
                n++;
        }
        else if (type == kPEAK_2 && (def->flags & 0x200))
        {
            if ((Flag_6C70(((char*)charRecord), def)->flags & 2) == 0)
                n++;
        }
        else if (type == kPEAK_3 && (def->flags & 0x400))
        {
            if ((Flag_6C70(((char*)charRecord), def)->flags & 2) == 0)
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
            if (type == kPEAK_1 && (*(unsigned char*)((char*)def + 0x35) & 1))
            {
                short id = def->id;
                short flagSlot = (charRecord->mpBoltsMap)[id];
                tBEProfileBolt* flagEntry;
                if (flagSlot >= 0)
                    flagEntry = ((&charRecord->mBolts[0])) + flagSlot;
                else
                    flagEntry = 0;
                if ((flagEntry->flags & 2) == 0)
                {
                    if (--n < 0)
                        return id;
                }
            }
            else if (type == kPEAK_2 && (def->flags & 0x200))
            {
                short id = def->id;
                short flagSlot = (charRecord->mpBoltsMap)[id];
                tBEProfileBolt* flagEntry;
                if (flagSlot >= 0)
                    flagEntry = ((&charRecord->mBolts[0])) + flagSlot;
                else
                    flagEntry = 0;
                if ((flagEntry->flags & 2) == 0)
                {
                    if (--n < 0)
                        return id;
                }
            }
            else if (type == kPEAK_3 && (def->flags & 0x400))
            {
                short id = def->id;
                short flagSlot = (charRecord->mpBoltsMap)[id];
                tBEProfileBolt* flagEntry;
                if (flagSlot >= 0)
                    flagEntry = ((&charRecord->mBolts[0])) + flagSlot;
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
extern cBECharProfileDB cBELibrary_profileLib[][10];
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
extern "C" int func_00156EE0(cBERewardInterface* self, int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    int n = 0;
    void* lib = func_0014BDB8_noarg();
    int count = func_0014D988(lib, characterId);
    sBEEntry_6EE0* def = (sBEEntry_6EE0*)func_0014D998((sBELibTables*)lib, characterId);
    int i;
    for (i = 0; i < count; i++, def++)
    {
        if (def->flags & 0x800)
        {
            if ((Flag_6EE0(((char*)charRecord), def)->flags & 2) == 0)
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
                short flagSlot = (charRecord->mpBoltsMap)[id];
                sBEFlagEntry_6EE0* flagEntry;
                if (flagSlot >= 0)
                    flagEntry = (sBEFlagEntry_6EE0*)(&charRecord->mBolts[0]) + flagSlot;
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
extern cBECharProfileDB cBELibrary_profileLib[][10];
unsigned int BXrand();
extern "C" int cBERewardInterface_getTotalNumRewards(cBERewardInterface* self, int kind);
extern "C" int cBERewardInterface_getNumOwnedRewards(cBERewardInterface* self, int a1, int a2, int kind);

/**
 * Picks a random reward of one kind that the character does not own yet.
 *
 * Returns -1 when the owned count (cBERewardInterface_getNumOwnedRewards) has reached the total (cBERewardInterface_getTotalNumRewards). Otherwise takes n = BXrand() %
 * (total - owned) and returns the index of the n-th clear bit in the kind's ownership bits in the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88): +0xF30 posters (kRewardPoster), +0xF36 trading cards
 * (kRewardCard), +0xF45 concept arts (kRewardArt), +0xF52 videos (kRewardVideo), +0xF53 toys (kRewardToy). Other kinds
 * leave the bit pointer null.
 * cBERewardInterface_awardUser gives the result with the matching buy function.
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param kind  reward kind (tRewardType), kRewardPoster to kRewardToy
 * @return the reward index, or -1 when every reward of the kind is owned
 */
int cBERewardInterface::getRandomLockedRewardIndex(int profile, int characterId, int kind)
{
    int total = getTotalNumRewards(kind);
    int result = -1;
    int have = getNumOwnedRewards(profile, characterId, kind);
    if (have >= total) {
        return -1;
    }
    unsigned int n = BXrand() % (unsigned int)(total - have);
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profile][characterId];
    signed char* bits = 0;
    switch (kind) {
    case kRewardPoster: bits = (signed char*)charRecord->mRewards.bPosters; break;
    case kRewardCard: bits = (signed char*)charRecord->mRewards.bTradingCards; break;
    case kRewardArt: bits = (signed char*)charRecord->mRewards.bConceptArts; break;
    case kRewardVideo: bits = (signed char*)charRecord->mRewards.bVideos; break;
    case kRewardToy: bits = (signed char*)charRecord->mRewards.bToys; break;
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
 * Kind (tRewardType) 3 posters (cBERewardInterface_getTotalNumPosters), 4 trading cards
 * (cBERewardInterface_getTotalNumTradingCards), 5 concept arts (cBERewardInterface_getTotalNumConceptArts), 6 videos
 * (cBERewardInterface_getTotalNumVideos), 7 toys (cBERewardInterface_getTotalNumToys), 8 returns 20 (the cheat
 * characters), 9 (bolts) returns 0; anything else 0.
 *
 * @param kind  reward kind (tRewardType), 3 to 9
 * @return the number of rewards of that kind
 */
int cBERewardInterface::getTotalNumRewards(int kind)
{
    switch (kind) {
    case kRewardPoster:
        return this->getTotalNumPosters();
    case kRewardCard:
        return this->getTotalNumTradingCards();
    case kRewardArt:
        return this->getTotalNumConceptArts();
    case kRewardVideo:
        return this->getTotalNumVideos();
    case kRewardToy:
        return this->getTotalNumToys();
    case kRewardCheatChar:
        return 20;
    case kRewardBolt:
        return 0;
    }
    return 0;
}
#endif

extern "C" cBERewardDB* cBERewardInterface_getDB(cBERewardInterface* self);

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumPosters__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of posters in the reward database (word +0x3C of cBERewardInterface_getDB(), On Tour
 * cBERewardDB.mNumPosters).
 *
 * @return the posters count
 */
int cBERewardInterface::getTotalNumPosters()
{
    return ((cBERewardInterface*)this)->getDB()->mNumPosters;
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
int cBERewardInterface::getTotalNumTradingCards()
{
    return ((cBERewardInterface*)this)->getDB()->mNumTradingCards;
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
int cBERewardInterface::getTotalNumConceptArts()
{
    return ((cBERewardInterface*)this)->getDB()->mNumConceptArts;
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
int cBERewardInterface::getTotalNumToys()
{
    return ((cBERewardInterface*)this)->getDB()->mNumToys;
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
int cBERewardInterface::getTotalNumVideos()
{
    return ((cBERewardInterface*)this)->getDB()->mNumVideos;
}
#endif

extern "C" cSSXAudio* cAudioInterface_bxAudio(void* self);

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTotalNumSongs__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of songs: the word at +0x504 of the audio object (cAudioInterface_bxAudio()).
 *
 * @return the song count
 */
int cBERewardInterface_getTotalNumSongs(void* vself)
{
    cBERewardInterface* self = (cBERewardInterface*)vself;
    return cAudioInterface_bxAudio(self)->MusicSys.mNumFESongs;
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
int cBERewardInterface::getTotalNumCheatChars()
{
    return ((cBERewardInterface*)this)->getDB()->mNumCheatChars;
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
int cBERewardInterface::countOwnedRewards(int count, signed char* bits)
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
void cBERewardInterface::markRewardOwned(int bit, char* bits)
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
int cBERewardInterface::isRewardOwned(int bit, char* bits)
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
 * @param kind  reward kind (tRewardType), kRewardPoster to kRewardCheatChar
 * @param bit  reward index
 * @return non-zero when owned
 */
int cBERewardInterface::isRewardOwned1(int profileIndex, int characterId, int kind, int bit)
{
    switch (kind) {
    case kRewardPoster: return isPosterOwned(profileIndex, characterId, bit);
    case kRewardCard: return isTradingCardOwned(profileIndex, characterId, bit);
    case kRewardArt: return isConceptArtOwned(profileIndex, characterId, bit);
    case kRewardVideo: return isVideoOwned(profileIndex, characterId, bit);
    case kRewardToy: return isToyOwned(profileIndex, characterId, bit);
    case kRewardCheatChar: return isCheatCharOwned(profileIndex, characterId, bit + 10);
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
 * @param kind  reward kind (tRewardType), kRewardPoster to kRewardCheatChar
 * @return the owned count
 */
int cBERewardInterface::getNumOwnedRewards(int profileIndex, int characterId, int kind)
{
    switch (kind) {
    case kRewardPoster: return getNumOwnedPosters(profileIndex, characterId);
    case kRewardCard: return getNumOwnedTradingCards(profileIndex, characterId);
    case kRewardArt: return getNumOwnedConceptArts(profileIndex, characterId);
    case kRewardVideo: return getNumOwnedVideos(profileIndex, characterId);
    case kRewardToy: return getNumOwnedToys(profileIndex, characterId);
    case kRewardCheatChar: return getNumOwnedCheatChars(profileIndex, characterId);
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedPosters);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
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
int cBERewardInterface::getNumOwnedPosters(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return countOwnedRewards(cBERewardInterface_getDB_noargs()->mNumPosters,
            (signed char*)charRecord->mRewards.bPosters);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedTradingCards);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
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
int cBERewardInterface::getNumOwnedTradingCards(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return countOwnedRewards(cBERewardInterface_getDB_noargs()->mNumTradingCards,
            (signed char*)charRecord->mRewards.bTradingCards);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedConceptArts);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
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
int cBERewardInterface::getNumOwnedConceptArts(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return countOwnedRewards(cBERewardInterface_getDB_noargs()->mNumConceptArts,
            (signed char*)charRecord->mRewards.bConceptArts);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedToys);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
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
int cBERewardInterface::getNumOwnedToys(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return countOwnedRewards(cBERewardInterface_getDB_noargs()->mNumToys, (signed char*)charRecord->mRewards.bToys);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedVideos);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
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
int cBERewardInterface::getNumOwnedVideos(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return countOwnedRewards(cBERewardInterface_getDB_noargs()->mNumVideos, (signed char*)charRecord->mRewards.bVideos);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedCheatChars);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
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
int cBERewardInterface::getNumOwnedCheatChars(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return countOwnedRewards(20, (signed char*)charRecord->mRewards.bCheatChars);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isMountainConquered);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
 * @return true when conquered
 */
bool cBERewardInterface::isMountainConquered(int iProfileIndex, int iCharacterID)
{
    for (int i = 0; i < 3; i++)
    {
        if (!isPeakConquered(iProfileIndex, iCharacterID, i))
            return false;
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
 * @return true when cBERewardInterface_isPeakCompleted() holds for peaks 0-2
 */
bool cBERewardInterface::isMountainCompleted(int iProfileIndex, int iCharacterID)
{
    for (int i = 0; i < 3; i++)
    {
        if (isPeakCompleted(iProfileIndex, iCharacterID, i) == 0)
            return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isPeakConquered);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char cBERewardInterface_getTrackMedal(void* self, int a, int b, int track, int kind);
bool cBERewardInterface_isBetterMedal(void* self, int a, int b);
extern "C" int cBERewardInterface_getCollectiblesMedal(cBERewardInterface* self, int a, int b, int kind);
extern "C" int cBERewardInterface_getBigChallengesMedal(cBERewardInterface* self, int a, int b, int kind);

struct sRewardReq_157920
{
    signed char track;
    signed char kind;
};
extern sRewardReq_157920 gMedalsInfo_v157920[][4][5] __asm__("gMedalsInfo");

/**
 * Tells whether a character has conquered a peak: top medals in all of its events.
 *
 * Walks the peak's event table gMedalsInfo for trophies 0-2 (kTROPHY_EARNINGS is skipped). Events on tracks
 * 4-11 need any medal, other events medal 0 or 1; kTROPHY_EXPLORE needs the best medal (0) for both collectibles and
 * big challenges.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param k  peak id
 * @return true when conquered
 */
bool cBERewardInterface::isPeakConquered(int iProfileIndex, int iCharacterID, int k)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            int track = gMedalsInfo_v157920[k][i][j].track;
            int kind = gMedalsInfo_v157920[k][i][j].kind;
            if (track < 0)
                break;
            if (i == kTROPHY_EARNINGS)
                continue;
            if (i == kTROPHY_EXPLORE)
            {
                if (getCollectiblesMedal(iProfileIndex, iCharacterID, k))
                    return false;
                if (getBigChallengesMedal(iProfileIndex, iCharacterID, k))
                    return false;
            }
            else
            {
                int medal = this->getTrackMedal(iProfileIndex, iCharacterID, track, kind);
                if ((unsigned int)(track - 4) < 8)
                {
                    if (!this->isBetterMedal(-1, medal))
                        return false;
                }
                else if ((unsigned int)medal >= 2)
                    return false;
            }
        }
    }
    return true;
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
 * @return true when cBERewardInterface_isTrophyOwned() holds for trophies 0-3
 */
bool cBERewardInterface::isPeakCompleted(int iProfileIndex, int iCharacterID, int peakID)
{
    for (int i = 0; i < 4; i++)
    {
        if (isTrophyOwned(iProfileIndex, iCharacterID, peakID, i) == 0)
            return false;
    }
    return true;
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
int cBERewardInterface::isPeakLocked(int rider, int peak)
{
    int profile = cBELibrary_getProfileIndex(rider);
    return isPeakLocked1(profile, cBELibrary_getCharacterID(rider), peak);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isPeakLocked1);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns whether peak 1 or 2 is still locked for a character.
 *
 * Reads the character record's (cBELibrary_profileLib[profileIndex][characterId]) peak flags: bPeak2Locked for peak 1
 * (kPEAK_2), bPeak3Locked for peak 2 (kPEAK_3). Every other peak is never locked (0).
 * cBERewardInterface_awardUserPeakPass clears these bits.
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param peak  the peak (On Tour peakID)
 * @return 1 when locked, else 0
 */
// PORT: the peak flags share a 64-bit unit with locLastHub (ld at +0x278).
int cBERewardInterface::isPeakLocked1(int profileIndex, int characterId, int peak)
{
    if (peak == kPEAK_2)
    {
        return cBELibrary_profileLib[profileIndex][characterId].bPeak2Locked;
    }
    if (peak == kPEAK_3)
    {
        return cBELibrary_profileLib[profileIndex][characterId].bPeak3Locked;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isTrophyOwned);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
signed char cBERewardInterface_getTrackMedal(void* self, int b, int c, int d, int e);
bool cBERewardInterface_isBetterMedal(void* self, int a, int b);
extern "C" int cBERewardInterface_getEarningsMedal(cBERewardInterface* self, int a, int b, int medal);
extern "C" int cBERewardInterface_getCollectiblesMedal(cBERewardInterface* self, int a, int b, int kind);
extern "C" int cBERewardInterface_getBigChallengesMedal(cBERewardInterface* self, int a, int b, int kind);

struct sRewardReq_157BF0
{
    signed char track;
    signed char kind;
};
extern sRewardReq_157BF0 gMedalsInfo_v157BF0[][4][5] __asm__("gMedalsInfo");

/**
 * Tells whether a character owns one of a peak's four trophies.
 *
 * kTROPHY_RACE and kTROPHY_FREESTYLE need a medal in every event listed for them in gMedalsInfo; kTROPHY_EXPLORE
 * needs a collectibles medal and a big-challenges medal; kTROPHY_EARNINGS needs the peak's earnings target reached
 * (any medal from cBERewardInterface_getEarningsMedal()).
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param k  peak id
 * @param i  trophy id (eBETrophyID), 0-3
 * @return 1 when the trophy is owned, 0 otherwise or for a trophy id outside 0-3
 */
int cBERewardInterface::isTrophyOwned(int iProfileIndex, int iCharacterID, int k, int i)
{
    switch (i)
    {
    case kTROPHY_RACE:
    case kTROPHY_FREESTYLE:
        for (int j = 0; j < 5; j++)
        {
            int track = gMedalsInfo_v157BF0[k][i][j].track;
            int kind = gMedalsInfo_v157BF0[k][i][j].kind;
            if (track < 0)
                break;
            if (!this->isBetterMedal(-1, this->getTrackMedal(iProfileIndex,
                    iCharacterID, track, kind)))
                return 0;
        }
        return 1;
    case kTROPHY_EXPLORE:
    {
        int owned = 0;
        if (this->isBetterMedal(-1, getCollectiblesMedal(iProfileIndex, iCharacterID, k)))
            owned = this->isBetterMedal(-1, getBigChallengesMedal(iProfileIndex, iCharacterID,
                    k)) != 0;
        return owned;
    }
    case kTROPHY_EARNINGS:
        return this->isBetterMedal(-1, getEarningsMedal(iProfileIndex, iCharacterID, k));
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isPosterOwned);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
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
int cBERewardInterface::isPosterOwned(int profileIndex, int characterId, int bit)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return isRewardOwned(bit, charRecord->mRewards.bPosters);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isTradingCardOwned);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
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
int cBERewardInterface::isTradingCardOwned(int profileIndex, int characterId, int bit)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return isRewardOwned(bit, charRecord->mRewards.bTradingCards);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isConceptArtOwned);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
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
int cBERewardInterface::isConceptArtOwned(int profileIndex, int characterId, int bit)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return isRewardOwned(bit, charRecord->mRewards.bConceptArts);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isToyOwned);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
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
int cBERewardInterface::isToyOwned(int profileIndex, int characterId, int bit)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return isRewardOwned(bit, charRecord->mRewards.bToys);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isVideoOwned);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
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
int cBERewardInterface::isVideoOwned(int profileIndex, int characterId, int bit)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return isRewardOwned(bit, charRecord->mRewards.bVideos);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", func_00157EA0);
#ifdef SKIP_ASM
/**
 * Returns bit @p bit of the 64-bit playlist word mRewards.iPlayList (+0xF78) of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
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
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return (int)((charRecord->mRewards.iPlayList >> bit) & 1);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_isCheatCharOwned);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
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
int cBERewardInterface::isCheatCharOwned(int profileIndex, int characterId, int bit)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return isRewardOwned(bit-10, charRecord->mRewards.bCheatChars);
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
 * @param kind  reward kind (tRewardType), kRewardPoster to kRewardCheatChar
 * @param bit  reward index
 * @param give  non-zero to charge the price (On Tour payForIt)
 * @return the called function's result, or 0 for other kinds
 */
int cBERewardInterface::buyReward(int profileIndex, int characterId, int kind, int bit, int give)
{
    switch (kind) {
    case kRewardPoster: return cBERewardInterface_buyPoster_r(this, profileIndex, characterId, bit, give);
    case kRewardCard: return cBERewardInterface_buyTradingCard_r(this, profileIndex, characterId, bit, give);
    case kRewardArt: return cBERewardInterface_buyConceptArt_r(this, profileIndex, characterId, bit, give);
    case kRewardVideo: return cBERewardInterface_buyVideo_r(this, profileIndex, characterId, bit, give);
    case kRewardToy: return cBERewardInterface_buyToy_r(this, profileIndex, characterId, bit, give);
    case kRewardCheatChar: return cBERewardInterface_buyCheatChar_r(this, profileIndex, characterId, bit + 10, give);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyPoster);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);
extern "C" int cBERewardInterface_getNumOwnedPosters(cBERewardInterface* self, int a1, int a2);
int cBERewardInterface_getTotalNumPosters(void* self);
extern "C" void cBERewardInterface_buyCheatChar(cBERewardInterface* self, int a1, int a2, int a3, int a4);
extern cBECharProfileDB cBELibrary_profileLib[][10];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");


/**
 * Gives the character poster @p bit, charging its price when @p give is set.
 *
 * Marks the bit in the character record's mRewards.bPosters (cBELibrary_profileLib[profileIndex][characterId])
 * (cBERewardInterface_markRewardOwned). With @p give, takes the price, the poster's mCost (cBERewardInterface_getDB()
 * ->mpPosters[bit]), times 10 when positive, off the character's money (cBEEconInterface_reduceMoney on the economy
 * interface, cBE_getInterface kind 11). When the character then owns every poster (owned count equal to the database
 * count) it also gets cheat character 20 (cBERewardInterface_buyCheatChar, unpaid). Ends with
 * cBEEconInterface_getMoney().
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  poster index (On Tour rewardIndex)
 * @param give  non-zero to charge the price (On Tour payForIt)
 */
void cBERewardInterface::buyPoster(int profileIndex, int characterId, int bit, int give)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    markRewardOwned(bit, charRecord->mRewards.bPosters);
    if (give)
    {
        tPosterInfo* poster = &cBERewardInterface_getDB_noargs()->mpPosters[bit];
        int amount = poster->mCost;
        if (amount > 0) amount *= 10;
        cBEEconInterface_reduceMoney(econ, profileIndex, characterId, amount);
    }
    if (getNumOwnedPosters(profileIndex, characterId) == this->getTotalNumPosters())
    {
        buyCheatChar(profileIndex, characterId, 20, 0);
    }
    cBEEconInterface_getMoney(econ, profileIndex, characterId);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyTradingCard);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);
extern "C" int cBERewardInterface_getNumOwnedTradingCards(cBERewardInterface* self, int a1, int a2);
int cBERewardInterface_getTotalNumTradingCards(void* self);
extern "C" void cBERewardInterface_buyCheatChar(cBERewardInterface* self, int a1, int a2, int a3, int a4);
extern cBECharProfileDB cBELibrary_profileLib[][10];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");


/**
 * Gives the character trading card @p bit, charging its price when @p give is set.
 *
 * Marks the bit in the character record's mRewards.bTradingCards (cBELibrary_profileLib[profileIndex][characterId])
 * (cBERewardInterface_markRewardOwned). With @p give, takes the price, the trading card's mCost
 * (cBERewardInterface_getDB()->mpTradingCards[bit]), times 10 when positive, off the character's money
 * (cBEEconInterface_reduceMoney on the economy interface, cBE_getInterface kind 11). When the character then owns every
 * trading card (owned count equal to the database count) it also gets cheat character 17
 * (cBERewardInterface_buyCheatChar, unpaid). Ends with cBEEconInterface_getMoney().
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  trading card index (On Tour rewardIndex)
 * @param give  non-zero to charge the price (On Tour payForIt)
 */
void cBERewardInterface::buyTradingCard(int profileIndex, int characterId, int bit, int give)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    markRewardOwned(bit, charRecord->mRewards.bTradingCards);
    if (give)
    {
        tTradingCardInfo* card = &cBERewardInterface_getDB_noargs()->mpTradingCards[bit];
        int amount = card->mCost;
        if (amount > 0) amount *= 10;
        cBEEconInterface_reduceMoney(econ, profileIndex, characterId, amount);
    }
    if (getNumOwnedTradingCards(profileIndex, characterId) == this->getTotalNumTradingCards())
    {
        buyCheatChar(profileIndex, characterId, 17, 0);
    }
    cBEEconInterface_getMoney(econ, profileIndex, characterId);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyConceptArt);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);
extern "C" int cBERewardInterface_getNumOwnedConceptArts(cBERewardInterface* self, int a1, int a2);
int cBERewardInterface_getTotalNumConceptArts(void* self);
extern "C" void cBERewardInterface_buyCheatChar(cBERewardInterface* self, int a1, int a2, int a3, int a4);
extern cBECharProfileDB cBELibrary_profileLib[][10];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");


/**
 * Gives the character concept art @p bit, charging its price when @p give is set.
 *
 * Marks the bit in the character record's mRewards.bConceptArts (cBELibrary_profileLib[profileIndex][characterId])
 * (cBERewardInterface_markRewardOwned). With @p give, takes the price, the concept art's mCost
 * (cBERewardInterface_getDB()->mpConceptArts[bit]), times 10 when positive, off the character's money
 * (cBEEconInterface_reduceMoney on the economy interface, cBE_getInterface kind 11). When the character then owns every
 * concept art (owned count equal to the database count) it also gets cheat character 22
 * (cBERewardInterface_buyCheatChar, unpaid). Ends with cBEEconInterface_getMoney().
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  concept art index (On Tour rewardIndex)
 * @param give  non-zero to charge the price (On Tour payForIt)
 */
void cBERewardInterface::buyConceptArt(int profileIndex, int characterId, int bit, int give)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    markRewardOwned(bit, charRecord->mRewards.bConceptArts);
    if (give)
    {
        tConceptArtInfo* art = &cBERewardInterface_getDB_noargs()->mpConceptArts[bit];
        int amount = art->mCost;
        if (amount > 0) amount *= 10;
        cBEEconInterface_reduceMoney(econ, profileIndex, characterId, amount);
    }
    if (getNumOwnedConceptArts(profileIndex, characterId) == this->getTotalNumConceptArts())
    {
        buyCheatChar(profileIndex, characterId, 22, 0);
    }
    cBEEconInterface_getMoney(econ, profileIndex, characterId);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyToy);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);
extern "C" int cBERewardInterface_getNumOwnedToys(cBERewardInterface* self, int a1, int a2);
int cBERewardInterface_getTotalNumToys(void* self);
extern "C" void cBERewardInterface_buyCheatChar(cBERewardInterface* self, int a1, int a2, int a3, int a4);
extern cBECharProfileDB cBELibrary_profileLib[][10];
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");


/**
 * Gives the character toy @p bit, charging its price when @p give is set.
 *
 * Marks the bit in the character record's mRewards.bToys (cBELibrary_profileLib[profileIndex][characterId])
 * (cBERewardInterface_markRewardOwned). With @p give, takes the price, the toy's mCost (cBERewardInterface_getDB()
 * ->mpToys[bit]), times 10 when positive, off the character's money (cBEEconInterface_reduceMoney on the economy
 * interface, cBE_getInterface kind 11). When the character then owns every toy (owned count equal to the database
 * count) it also gets cheat character 19 (cBERewardInterface_buyCheatChar, unpaid). Ends with
 * cBEEconInterface_getMoney().
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  toy index (On Tour rewardIndex)
 * @param give  non-zero to charge the price (On Tour payForIt)
 */
void cBERewardInterface::buyToy(int profileIndex, int characterId, int bit, int give)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    markRewardOwned(bit, charRecord->mRewards.bToys);
    if (give)
    {
        tToyInfo* toy = &cBERewardInterface_getDB_noargs()->mpToys[bit];
        int amount = toy->mCost;
        if (amount > 0) amount *= 10;
        cBEEconInterface_reduceMoney(econ, profileIndex, characterId, amount);
    }
    if (getNumOwnedToys(profileIndex, characterId) == this->getTotalNumToys())
    {
        buyCheatChar(profileIndex, characterId, 19, 0);
    }
    cBEEconInterface_getMoney(econ, profileIndex, characterId);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_buyVideo);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
extern cBECharProfileDB cBELibrary_profileLib[][10];
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);

/**
 * Gives the character video @p idx, charging its price when @p award is set.
 *
 * Marks the bit in the character record's mRewards.bVideos (cBELibrary_profileLib[profile][characterId])
 * (cBERewardInterface_markRewardOwned). With @p award, takes the price, the video's mCost (cBERewardInterface_getDB()
 * ->mpVideos[idx]), times 10 when positive, off the character's money (cBEEconInterface_reduceMoney on the economy
 * interface). Ends with cBEEconInterface_getMoney(). Unlike the poster, card, art and toy versions it unlocks nothing
 * for a complete set.
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param idx  video index (On Tour rewardIndex)
 * @param award  non-zero to charge the price (On Tour payForIt)
 */
void cBERewardInterface::buyVideo(int profile, int characterId, int idx, int award)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profile][characterId];
    markRewardOwned(idx, charRecord->mRewards.bVideos);
    if (award)
    {
        tVideoInfo* video = &cBERewardInterface_getDB_noargs()->mpVideos[idx];
        int amount = video->mCost;
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern cBECharProfileDB cBELibrary_profileLib[][10];
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);

/**
 * Gives the character song @p bit, charging 5000 when @p award is set.
 *
 * Sets bit @p bit of the 64-bit owned-song word mRewards.iOwnedSongList (+0xF70) of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88); with @p award, takes 5000 off the character's money
 * (cBEEconInterface_reduceMoney on the economy interface). Ends with cBEEconInterface_getMoney(). The message center's
 * request line and the cheat codes call it.
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param bit  song index (On Tour songIndex)
 * @param award  non-zero to charge the price (On Tour payForIt)
 */
// PORT: 64-bit ulong bit set (sd/ld).
void cBERewardInterface::buySong(int profile, int characterId, int bit, int award)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profile][characterId];
    charRecord->mRewards.iOwnedSongList |= (ulong)1 << bit;
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: the unit declares cBERewardInterface_getDB(void*), but it takes no args (returns &cBELibrary_rewardDB).
cBERewardDB* cBERewardInterface_getDB_noargs() __asm__("cBERewardInterface_getDB");
extern cBECharProfileDB cBELibrary_profileLib[][10];
extern "C" void cBERewardInterface_markRewardOwned(cBERewardInterface* self, int bit, char* bits);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" int cBEEconInterface_reduceMoney(void* self, int a, int b, int amount);

/**
 * Gives the character cheat character @p idx, charging its price when @p award is set.
 *
 * Marks bit (@p idx - 10) of the character record's mRewards.bCheatChars (cBELibrary_profileLib[profile][characterId])
 * (cBERewardInterface_markRewardOwned). With @p award, takes the price, the mCost of record (idx - 10) in
 * cBERewardInterface_getDB()->mpCheatChars, times 10 when positive, off the character's money
 * (cBEEconInterface_reduceMoney). Ends with cBEEconInterface_getMoney(). The buy functions call it with ids 17, 19, 20
 * and 22 for completed sets; the cheat codes and cBERewardInterface_awardUser call it too.
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param idx  cheat character id, 10 and up (On Tour cheatCharID)
 * @param award  non-zero to charge the price (On Tour payForIt)
 */
void cBERewardInterface::buyCheatChar(int profile, int characterId, int idx, int award)
{
    idx -= 10;
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profile][characterId];
    markRewardOwned(idx, charRecord->mRewards.bCheatChars);
    if (award)
    {
        tCheatCharInfo* cheatChar = &cBERewardInterface_getDB_noargs()->mpCheatChars[idx];
        int amount = cheatChar->mCost;
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
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns the character's radio mode: the word at +0xF80 of the character record (cBELibrary_profileLib + profile *
 * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the radio mode
 */
int cBERewardInterface::getRadioMode(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return charRecord->mRadioMode;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getOwnedSongList);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns the character's owned-song list: mRewards.iOwnedSongList, the 64-bit word at +0xF70 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the owned-song bits
 */
long cBERewardInterface::getOwnedSongList(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return charRecord->mRewards.iOwnedSongList;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getPlayList);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns the character's playlist: mRewards.iPlayList, the 64-bit word at +0xF78 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the playlist bits
 */
long cBERewardInterface::getPlayList(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return charRecord->mRewards.iPlayList;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumOwnedSongs);
#ifdef SKIP_ASM
/**
 * Returns how many songs the character owns: the set bits among the 64 of mRewards.iOwnedSongList (+0xF70) of the
 * character record (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @return the number of owned songs
 */
int cBERewardInterface::getNumOwnedSongs(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    return countOwnedRewards(0x40, (signed char*)&charRecord->mRewards.iOwnedSongList);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBERewardInterface_getNumSongsInPlayList);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
extern "C" int cBERewardInterface_countOwnedRewards(cBERewardInterface* self, int count, signed char* bits);

/**
 * Counts the songs in a character's playlist: the set bits among the 64 of mRewards.iPlayList (+0xF78) of the profile
 * record.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @return the number of songs
 */
int cBERewardInterface::getNumSongsInPlayList(int iProfileIndex, int iCharacterID)
{
    cBECharProfileDB* charProfile = &cBELibrary_profileLib[iProfileIndex][iCharacterID];
    return countOwnedRewards(0x40, (signed char*)&charProfile->mRewards.iPlayList);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_setRadioMode);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Sets the character's radio mode: the word at +0xF80 of the character record (cBELibrary_profileLib + profile * 0x9B50
 * + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param value  the radio mode (On Tour radioMode)
 */
void cBERewardInterface::setRadioMode(int profileIndex, int characterId, int value)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    charRecord->mRadioMode = value;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_setOwnedSongList);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Sets the character's owned-song list: mRewards.iOwnedSongList, the 64-bit word at +0xF70 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param value  the owned-song bits (On Tour iOwnedSongList)
 */
void cBERewardInterface::setOwnedSongList(int profileIndex, int characterId, long value)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    charRecord->mRewards.iOwnedSongList = value;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_setPlayList);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t off-PS2.
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Sets the character's playlist: mRewards.iPlayList, the 64-bit word at +0xF78 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param value  the playlist bits (On Tour iPlayList)
 */
void cBERewardInterface::setPlayList(int profileIndex, int characterId, long value)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    charRecord->mRewards.iPlayList = value;
}
#endif

extern int gCollectiblesMedalLevel[][4];

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
int cBERewardInterface::getCollectibleMedalTarget(int peakID, int medal)
{
    return gCollectiblesMedalLevel[peakID][medal];
}
#endif

extern int gBigChallengesMedalLevel[][4];

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
int cBERewardInterface::getChallengesMedalTarget(int peakID, int medal)
{
    return gBigChallengesMedalLevel[peakID][medal];
}
#endif

extern unsigned int gEarningsMedalLevel[];

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getEarningsTarget__FPvi);
#ifdef SKIP_ASM
/**
 * Returns a peak's earnings target from gEarningsMedalLevel.
 *
 * @param peakID  the peak id
 * @return the target amount
 */
int cBERewardInterface::getEarningsTarget(int peakID)
{
    return gEarningsMedalLevel[peakID];
}
#endif

// The record of location a, event b (cBECharProfileDB.mScoreStats), or null (be/bedbprofile).
tBETrackScore* cBECharProfileDB_getScoreStats(void* self, int a, int b);
extern cBECharProfileDB cBELibrary_profileLib[][10];

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
int cBERewardInterface::calculateCollectibleMedal(int idx, int numCollected)
{
    int* row = gCollectiblesMedalLevel[idx];
    for (int i = 0; i < 4; i++) {
        if (numCollected >= row[i]) return i;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrackMedal__FPviiii);
#ifdef SKIP_ASM

/**
 * Returns a character's medal for one event: the BestMedal of its score record.
 *
 * The record comes from cBECharProfileDB_getScoreStats() with @p location and @p eventID.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param eventID  passed as the score record's event kind
 * @param location  passed as the score record's location
 * @return the medal (0 best to 3), or -1 when there is no record
 */
signed char cBERewardInterface::getTrackMedal(int iProfileIndex, int iCharacterID, int eventID, int location)
{
    tBETrackScore* result =
        cBECharProfileDB_getScoreStats(&cBELibrary_profileLib[iProfileIndex][iCharacterID], location, eventID);
    if (result == 0) {
        return -1;
    }
    return result->BestMedal;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrackHighScore);
#ifdef SKIP_ASM
/**
 * Returns a character's best result for one event: the uiHighScore of its score record.
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
int cBERewardInterface::getTrackHighScore(int profileIndex, int characterId, int event, int locationId)
{
    tBETrackScore* result =
        cBECharProfileDB_getScoreStats(&cBELibrary_profileLib[profileIndex][characterId], locationId, event);
    if (result == 0) {
        return 0;
    }
    return result->uiHighScore;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getCollectiblesMedal);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
int cBERewardInterface::getCollectiblesMedal(int iProfileIndex, int iCharacterID, int kind)
{
    int collected = cBEMissionInterface_getCurrentCollectForPeakByChar(cBE_getInterface_Fv(cBE_getBE(), 10), iProfileIndex, iCharacterID, kind);
    int* thresholds = gCollectiblesMedalLevel[kind];
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
int cBERewardInterface::getBigChallengesMedal(int iProfileIndex, int iCharacterID, int kind)
{
    int completed = cBEMissionInterface_getNumberCompletedMissionsOnPeakByChar(cBE_getInterface_Fv(cBE_getBE(), 10), iProfileIndex, iCharacterID, kind);
    int* thresholds = gBigChallengesMedalLevel[kind];
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int cBEEconInterface_getTotalMoneyEarned(void* self, int a, int b);

/**
 * Tells whether a character's total earnings reach a peak's earnings target.
 *
 * @param iProfileIndex  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param medal  the peak id (an index into gEarningsMedalLevel), despite the parameter's name
 * @return 1 (a medal) when the target is reached, else -1
 */
int cBERewardInterface::getEarningsMedal(int iProfileIndex, int iCharacterID, int medal)
{
    unsigned int earned = cBEEconInterface_getTotalMoneyEarned(cBE_getInterface_Fv(cBE_getBE(), 11), iProfileIndex, iCharacterID);
    if (earned >= gEarningsMedalLevel[medal])
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
int cBERewardInterface::calculateBigChallengeMedal(int idx, int numCompleted)
{
    int* row = gBigChallengesMedalLevel[idx];
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
int cBERewardInterface::calculateEarningMedal(int idx, unsigned int totalCash)
{
    return (totalCash < gEarningsMedalLevel[idx]) ? -1 : 1;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_calculateNextEarningsLevel);
#ifdef SKIP_ASM
bool cBERewardInterface_isBetterMedal(void* self, int a, int b);

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
void* cBERewardInterface::calculateNextEarningsLevel(int iProfileIndex, int iCharacterID)
{
    for (int i = 0; i < 3; i++)
    {
        if (!this->isBetterMedal(-1, getEarningsMedal(iProfileIndex, iCharacterID, i)))
            return (void*)gEarningsMedalLevel[i];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_calculateNextCollectiblesLevel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cBERewardInterface_getCollectiblesMedal(cBERewardInterface* self, int a, int b, int kind);
extern int gCollectiblesMedalLevel_2d[][4] __asm__("gCollectiblesMedalLevel");

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
int cBERewardInterface::calculateNextCollectiblesLevel(int iProfileIndex, int iCharacterID, int kind)
{
    int medal = getCollectiblesMedal(iProfileIndex, iCharacterID, kind);
    int best = 0;
    for (int i = 0; i < 4; i++)
    {
        if (this->isBetterMedal(medal, i))
        {
            if (best == 0 || gCollectiblesMedalLevel_2d[kind][i] < best)
                best = gCollectiblesMedalLevel_2d[kind][i];
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
extern int gBigChallengesMedalLevel_2d[][4] __asm__("gBigChallengesMedalLevel");

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
int cBERewardInterface::calculateNextBigChallengesLevel(int iProfileIndex, int iCharacterID, int kind)
{
    int medal = getBigChallengesMedal(iProfileIndex, iCharacterID, kind);
    int best = 0;
    for (int i = 0; i < 4; i++)
    {
        if (this->isBetterMedal(medal, i))
        {
            if (best == 0 || gBigChallengesMedalLevel_2d[kind][i] < best)
                best = gBigChallengesMedalLevel_2d[kind][i];
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
void cBERewardInterface::clearUserAwards()
{
    ((cBEUserAwards*)cBELibrary_userAwards)->clear();
    int charID = cBELibrary_getCharacterID(cBELibrary_getRiderIndex(0));
    gNextEarningsLimit = calculateNextEarningsLevel(0, charID);
    for (int i = 0; i < 3; i++)
        gNextCollectiblesLimit[i] = calculateNextCollectiblesLevel(0, charID, i);
    for (int i = 0; i < 3; i++)
        gNextBigChallengesLimit[i] = calculateNextBigChallengesLevel(0, charID, i);
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
void* cBERewardInterface::getUserAwards()
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
void* cBERewardInterface::getTotalNumUserAwards()
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
extern "C" void cBEUserAwards_setAward(cBEUserAwards* self, int row, int col, int value);
extern void* cBELibrary_userAwards[];
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Unlocks peak 1 (mode kPEAK_2) or peaks 2 and 1 (mode kPEAK_3) for the character and logs each newly unlocked peak as
 * a peak-pass award.
 *
 * Mode kPEAK_3 clears bPeak3Locked of the character record (cBELibrary_profileLib[profile][characterId]) when set and
 * logs it with cBEUserAwards_setAward(cBELibrary_userAwards, @p row, kRewardPeakPass, character * 3 + 2); it then falls
 * through to mode kPEAK_2, which does the same for bPeak2Locked with character * 3 + 1. Mode kPEAK_1 does nothing
 * ([O] On Tour types the mode eBEPeakID peakID). These are the locks
 * cBERewardInterface_isPeakLocked1 tests. cBERewardInterface_awardUser calls it for peak 0's trophy categories (mode 1)
 * and peak 1's (mode 2).
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param characterId  character ID (On Tour iCharacterID)
 * @param row  the award category the log entry goes under (On Tour awardCategory)
 * @param mode  the highest peak to unlock (eBEPeakID): kPEAK_2 or kPEAK_3
 */
void cBERewardInterface::awardUserPeakPass(int profile, int characterId, int row, int mode)
{
    cBEUserAwards* awards = (cBEUserAwards*)cBELibrary_userAwards;
    switch (mode)
    {
    case kPEAK_1:
        break;
    case kPEAK_3:
        if (cBELibrary_profileLib[profile][characterId].bPeak3Locked)
        {
            cBELibrary_profileLib[profile][characterId].bPeak3Locked = 0;
            awards->setAward(row, kRewardPeakPass, characterId * 3 + 2);
        }
    case kPEAK_2:
        if (cBELibrary_profileLib[profile][characterId].bPeak2Locked)
        {
            cBELibrary_profileLib[profile][characterId].bPeak2Locked = 0;
            awards->setAward(row, kRewardPeakPass, characterId * 3 + 1);
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
 * The category (eBEAwardCategory) is 5 + 3 * @p tier + @p kind, so kAWARD_PEAK1_RACE_TROPHY to
 * kAWARD_PEAK3_EARNINGS_TROPHY for peaks 0-2 and trophies 0-3. The profile and character arguments are not used.
 *
 * @param iProfileIndex    profile index (unused)
 * @param iCharacterID    character id (unused)
 * @param kind  peak id (eBEPeakID), 0-2
 * @param tier  trophy id (eBETrophyID), 0-3
 * @return the award category, or -1 for a peak or trophy id out of range
 */
int cBERewardInterface::getAwardTrophyCategory(int iProfileIndex, int iCharacterID, int kind, int tier)
{
    int category = -1;
    switch (tier)
    {
    case kTROPHY_RACE:
        if (kind == kPEAK_1)
            category = kAWARD_PEAK1_RACE_TROPHY;
        else if (kind == kPEAK_2)
            category = kAWARD_PEAK2_RACE_TROPHY;
        else if (kind == kPEAK_3)
            category = kAWARD_PEAK3_RACE_TROPHY;
        break;
    case kTROPHY_FREESTYLE:
        if (kind == kPEAK_1)
            category = kAWARD_PEAK1_FREESTYLE_TROPHY;
        else if (kind == kPEAK_2)
            category = kAWARD_PEAK2_FREESTYLE_TROPHY;
        else if (kind == kPEAK_3)
            category = kAWARD_PEAK3_FREESTYLE_TROPHY;
        break;
    case kTROPHY_EXPLORE:
        if (kind == kPEAK_1)
            category = kAWARD_PEAK1_EXPLORE_TROPHY;
        else if (kind == kPEAK_2)
            category = kAWARD_PEAK2_EXPLORE_TROPHY;
        else if (kind == kPEAK_3)
            category = kAWARD_PEAK3_EXPLORE_TROPHY;
        break;
    case kTROPHY_EARNINGS:
        if (kind == kPEAK_1)
            category = kAWARD_PEAK1_EARNINGS_TROPHY;
        else if (kind == kPEAK_2)
            category = kAWARD_PEAK2_EARNINGS_TROPHY;
        else if (kind == kPEAK_3)
            category = kAWARD_PEAK3_EARNINGS_TROPHY;
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
void cBERewardInterface::awardUserTrophy(int iProfileIndex, int iCharacterID, int kind, int tier)
{
    int bit = getAwardTrophyCategory(iProfileIndex, iCharacterID, kind, tier);
    if (isAwardReceived(iProfileIndex, iCharacterID, bit) == 0)
    {
        awardUser(iProfileIndex, iCharacterID, bit);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_calculateUserTrackMedalRewards);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
signed char cBERewardInterface_getTrackMedal(void* self, int b, int c, int d, int e);
bool cBERewardInterface_isBetterMedal(void* self, int a, int b);
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
extern sProfileChar_1591E8 cBELibrary_profileLib_1591E8[][10] __asm__("cBELibrary_profileLib");
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
extern sRewardReq_1591E8 gMedalsInfo_1591E8[][4][5] __asm__("gMedalsInfo");

extern cBERaceDB cBELibrary_raceDB_1591E8 __asm__("cBELibrary_raceDB");
extern signed char D_005305F9[];


/**
 * Hands out the rewards for an event result: medal cash, the award category, trophies, and the unlocks of the peak's
 * next events.
 *
 * The peak (eBEPeakID) comes from cBERewardInterface_getTrackPeakID (the location's peak; the time and points
 * challenges of peaks 1, 2 and 3 (eBEGameEvent 6/9, 7/10, 8/11) force kPEAK_1, kPEAK_2 and kPEAK_3) and the trophy from
 * cBERewardInterface_getTrackTrophyID. The cash is cBEEconInterface_getCashReward(economy interface, @p medal, @p
 * event, @p mode). The award category (eBEAwardCategory) is the peak plus kAWARD_PEAK1_EVENT_MEDAL (events 0-3),
 * kAWARD_PEAK1_RIVALTIME_MEDAL (4), kAWARD_PEAK1_RIVALPOINTS_MEDAL (5), kAWARD_PEAK1_TIMECHALLENGE_MEDAL (6-8) or
 * kAWARD_PEAK1_POINTSCHALLENGE_MEDAL (9-11). A medal of 0 or 1 calls cBERewardInterface_awardUser for it on the first
 * result (score-record count 0); events 6-11 with kMEDAL_BRONZE or kMEDAL_SILVER call func_001E1DD0 instead, unless
 * raceDB byte +0x49 (D_005305F9) is set.
 *
 * When the medal beats the stored one (cBERewardInterface_isBetterMedal), pays the full cash (cBERewardInterface_awardUserCash1), records
 * the medal (cBECharProfileDB_updateMedalStats) and gives the trophy with cBERewardInterface_awardUserTrophy if it became owned; otherwise
 * records the medal and pays half. A conquered mountain gets award 0 once. Stores the medal and cash in mLastEventMedal
 * and mLastEventCashReward of cBERewardInterface_instance.
 *
 * With any medal it then walks the trophy's five requirements in gMedalsInfo[peak][trophy] (event byte, location byte;
 * kMEDALSINFO_END ends) and checks each for a medal with cBERewardInterface_getTrackMedal. If they hold, it clears a
 * lock bit in the character's flags (+0x278: 0x40/0x80/0x100 for trophy 0, 0x200/0x400/0x800 otherwise, by peak) and
 * calls func_001E2370; the requirements on events 4 and 5 gate a second set of bits (0x4000-0x80000). Events 12 and 13
 * leave the category at -1, which would still reach cBERewardInterface_awardUser (possible bug).
 * @param profile  profile index (On Tour iProfileIndex)
 * @param charID  character ID (On Tour iCharacterID)
 * @param mode  the game event, 0 to 13 (On Tour eventID)
 * @param event  the location (On Tour locationID)
 * @param medal  the medal earned, 0 best to 3, -1 none (On Tour medalID)
 */
void cBERewardInterface::calculateUserTrackMedalRewards(int profile, int charID, int mode, int event, int medal)
{
    sProfileChar_1591E8* pc = &cBELibrary_profileLib_1591E8[profile][charID];
    int bit = -1;
    void* score = cBE_getInterface_Fv(cBE_getBE(), 11);
    sScoreStat_1591E8* stats = cBECharProfileDB_getScoreStats_1591E8(pc, event, mode);
    int tier = getTrackTrophyID(mode, event);
    int group = getTrackPeakID(event);
    unsigned int money;
    int oldMedal = stats->medal;
    if (mode == kGAMEEVENT_TIME_PEAK1 || mode == kGAMEEVENT_POINTS_PEAK1)
        group = kPEAK_1;
    else if (mode == kGAMEEVENT_TIME_PEAK2 || mode == kGAMEEVENT_POINTS_PEAK2)
        group = kPEAK_2;
    else if (mode == kGAMEEVENT_TIME_PEAK3 || mode == kGAMEEVENT_POINTS_PEAK3)
        group = kPEAK_3;
    money = cBEEconInterface_getCashReward(score, medal, event, mode);
    switch (mode) {
    case kGAMEEVENT_RACE:
    case kGAMEEVENT_SLOPESTYLE:
    case kGAMEEVENT_SUPERPIPE:
    case kGAMEEVENT_BIGAIR:
        bit = group + kAWARD_PEAK1_EVENT_MEDAL;
        break;
    case kGAMEEVENT_RIVALTIME:
        bit = group + kAWARD_PEAK1_RIVALTIME_MEDAL;
        break;
    case kGAMEEVENT_RIVALPOINTS:
        bit = group + kAWARD_PEAK1_RIVALPOINTS_MEDAL;
        break;
    case kGAMEEVENT_TIME_PEAK1:
    case kGAMEEVENT_TIME_PEAK2:
    case kGAMEEVENT_TIME_PEAK3:
        bit = group + kAWARD_PEAK1_TIMECHALLENGE_MEDAL;
        break;
    case kGAMEEVENT_POINTS_PEAK1:
    case kGAMEEVENT_POINTS_PEAK2:
    case kGAMEEVENT_POINTS_PEAK3:
        bit = group + kAWARD_PEAK1_POINTSCHALLENGE_MEDAL;
        break;
    case kGAMEEVENT_FREERIDE:
    case kGAMEEVENT_MISSION:
        break;
    }
    if ((unsigned int)medal < 2) {
        if (stats->count == 0)
            awardUser(profile, charID, bit);
    } else if ((unsigned int)(mode - 6) < 6 && D_005305F9[0] == 0) {
        if (medal == kMEDAL_BRONZE)
            func_001E1DD0(group, mode, 0);
        else if (medal == kMEDAL_SILVER)
            func_001E1DD0(group, mode, 1);
    }
    if (this->isBetterMedal(oldMedal, medal)) {
        int had = isTrophyOwned(profile, charID, group, tier);
        awardUserCash1(profile, charID, money);
        cBECharProfileDB_updateMedalStats(pc, event, mode, medal);
        if (isTrophyOwned(profile, charID, group, tier) && !had)
            awardUserTrophy(profile, charID, group, tier);
    } else {
        cBECharProfileDB_updateMedalStats(pc, event, mode, medal);
        money >>= 1;
        awardUserCash1(profile, charID, money);
    }
    if (isMountainConquered(profile, charID) && isAwardReceived(profile, charID, 0) == 0)
        awardUser(profile, charID, 0);
    (cBERewardInterface_instance)->mLastEventMedal = medal;
    (cBERewardInterface_instance)->mLastEventCashReward = money;
    if (!this->isBetterMedal(-1, medal))
        return;
    int any = 0;
    int okA = 1;
    int okB = 1;
    for (int i = 0; i < 5; i++) {
        int kind = gMedalsInfo_1591E8[group][tier][i].kind;
        int track = gMedalsInfo_1591E8[group][tier][i].track;
        if (track == kMEDALSINFO_END) {
            if (cBELibrary_raceDB_1591E8.cGamePath == 0)
                func_001E1C10(group);
            break;
        }
        if ((unsigned int)(track - 4) < 2) {
            any = 1;
            if (!this->isBetterMedal(-1, this->getTrackMedal(profile, charID,
                    track, kind))) {
                okB = 0;
                break;
            }
        } else {
            if (!this->isBetterMedal(-1, this->getTrackMedal(profile, charID,
                    track, kind))) {
                if (!any) {
                    if (cBELibrary_raceDB_1591E8.cGamePath == 0)
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
            if (group == kPEAK_1)
                pc->flags &= ~0x40L;
            else if (group == kPEAK_2)
                pc->flags &= ~0x80L;
            else if (group == kPEAK_3)
                pc->flags &= ~0x100L;
        } else {
            if (group == kPEAK_1)
                pc->flags &= ~0x200L;
            else if (group == kPEAK_2)
                pc->flags &= ~0x400L;
            else if (group == kPEAK_3)
                pc->flags &= ~0x800L;
        }
        if (D_005305F9[0] == 0)
            func_001E2370(group);
    }
    if (okB) {
        if (tier == 0) {
            if (group == kPEAK_1)
                pc->flags &= ~0x4000L;
            else if (group == kPEAK_2)
                pc->flags &= ~0x8000L;
            else if (group == kPEAK_3)
                pc->flags &= ~0x10000L;
        } else {
            if (group == kPEAK_1)
                pc->flags &= ~0x20000L;
            else if (group == kPEAK_2)
                pc->flags &= ~0x40000L;
            else if (group == kPEAK_3)
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
void cBERewardInterface::awardUserCash(int rider, int money)
{
    int profile = cBELibrary_getProfileIndex(rider);
    awardUserCash1(profile, cBELibrary_getCharacterID(rider), money);
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_awardUserCash1);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
void cBERewardInterface::awardUserCash1(int profileIndex, int characterId, int amount)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 11);
    *(int*)cBELibrary_userAwards += amount;
    if (gNextEarningsLimit != 0 && (unsigned int)(cBEEconInterface_getTotalMoneyEarned(econ, profileIndex, characterId) + amount) >= (unsigned int)gNextEarningsLimit)
    {
        int before[3];
        int i;
        for (i = 0; i < 3; i++)
            before[i] = isTrophyOwned(profileIndex, characterId, i, 3);
        cBEEconInterface_addMoney(econ, profileIndex, characterId, amount);
        gNextEarningsLimit = calculateNextEarningsLevel(profileIndex, characterId);
        for (i = 0; i < 3; i++)
        {
            if (before[i] == 0 && isTrophyOwned(profileIndex, characterId, i, 3))
                awardUserTrophy(profileIndex, characterId, i, 3);
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern int gCollectiblesMedalLevel_2d[][4] __asm__("gCollectiblesMedalLevel");

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
void cBERewardInterface::awardUserCollectible(int rider, int k)
{
    int cur = cBEMissionInterface_getCurrentCollectForPeak(cBE_getInterface_Fv(cBE_getBE(), 10), rider, k);
    int* nextLevel = &gNextCollectiblesLimit[k];
    if (*nextLevel <= 0)
        return;
    if (cur < *nextLevel)
        return;
    int prof = cBELibrary_getProfileIndex(rider);
    int ch = cBELibrary_getCharacterID(rider);
    int was = isTrophyOwned(prof, ch, k, 2);
    *nextLevel = calculateNextCollectiblesLevel(prof, ch, k);
    if (was && cur == gCollectiblesMedalLevel_2d[k][3])
        awardUserTrophy(prof, ch, k, 2);
    if (isMountainConquered(prof, ch))
    {
        if (isAwardReceived(prof, ch, 0) == 0)
            awardUser(prof, ch, 0);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_awardUserBigChallenge);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
extern int gBigChallengesMedalLevel_2d[][4] __asm__("gBigChallengesMedalLevel");

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
void cBERewardInterface::awardUserBigChallenge(int rider, int k)
{
    int cur = cBEMissionInterface_getNumberCompletedMissionsOnPeak(cBE_getInterface_Fv(cBE_getBE(), 10), k);
    int* nextLimit = &gNextBigChallengesLimit[k];
    if (*nextLimit <= 0)
        return;
    if (cur < *nextLimit)
        return;
    int prof = cBELibrary_getProfileIndex(rider);
    int ch = cBELibrary_getCharacterID(rider);
    int was = isTrophyOwned(prof, ch, k, 2);
    *nextLimit = calculateNextBigChallengesLevel(prof, ch, k);
    if (was && cur == gBigChallengesMedalLevel_2d[k][3])
        awardUserTrophy(prof, ch, k, 2);
    if (isMountainConquered(prof, ch))
    {
        if (isAwardReceived(prof, ch, 0) == 0)
            awardUser(prof, ch, 0);
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
void cBERewardInterface::awardUser1(int rider, int bit)
{
    int profile = cBELibrary_getProfileIndex(rider);
    awardUser(profile, cBELibrary_getCharacterID(rider), bit);
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
 * Except for categories 17-19 (the event medals) an award already given is skipped. Categories 0-4 (mountain, peaks)
 * call a front-end notice (func_001E2578, func_001E2518, func_001E2428) and give a fixed cheat character; peak 0's
 * trophies give random trading cards and a poster and unlock peak 1, peak 1's give a toy and concept arts and unlock
 * peaks 1 and 2, peak 2's give a fixed cheat character; 17-31 give a bolt item. The category bit is set (+0xF28), the
 * rewards go into the log cBELibrary_userAwards, and the mountain, peak and trophy awards are re-checked.
 *
 * @param profile  profile index (On Tour: iProfileIndex)
 * @param iCharacterID  character id (On Tour: iCharacterID)
 * @param bit  the award category (eBEAwardCategory), 0-31 (On Tour: awardCategory)
 */
void cBERewardInterface::awardUser(int profile, int iCharacterID, int bit)
{
    cBECharProfileDB* rec = &cBELibrary_profileLib[profile][iCharacterID];
    cBEUserAwards* rewardLog = (cBEUserAwards*)cBELibrary_userAwards;
    sSlots4_00159CD0 slots4 = D_0045AA28;
    sSlots2_00159CD0 slots2 = D_004A1268[0];
    int item = kNoBoltID;
    int value = 0;

    if (bit < kAWARD_PEAK1_EVENT_MEDAL || bit > kAWARD_PEAK3_EVENT_MEDAL) {
        if (this->isAwardReceived(profile, iCharacterID, bit))
            return;
    }

    switch (bit) {
    case kAWARD_MOUNTAIN_CONQUERED:
        func_001E2578();
        value = 29;
        break;
    case kAWARD_MOUNTAIN_COMPLETED:
        func_001E2518();
        value = 0x18;
        break;
    case kAWARD_PEAK1_COMPLETED:
        func_001E2428(0);
        value = 18;
        break;
    case kAWARD_PEAK2_COMPLETED:
        func_001E2428(1);
        value = 25;
        break;
    case kAWARD_PEAK3_COMPLETED:
        func_001E2428(2);
        value = 26;
        break;
    case kAWARD_PEAK1_RACE_TROPHY:
    case kAWARD_PEAK1_FREESTYLE_TROPHY:
    case kAWARD_PEAK1_EXPLORE_TROPHY:
    case kAWARD_PEAK1_EARNINGS_TROPHY:
    {
        int i;
        for (i = 0; i < 4 && (slots4.v[i] = this->getRandomLockedRewardIndex(profile, iCharacterID, kRewardCard)) >= 0;
             i++) {
            this->buyTradingCard(profile, iCharacterID, slots4.v[i], 0);
            rewardLog->setAward(bit, kRewardCard, slots4.v[i]);
        }
        int rewardBit = this->getRandomLockedRewardIndex(profile, iCharacterID, kRewardPoster);
        if (rewardBit >= 0) {
            this->buyPoster(profile, iCharacterID, rewardBit, 0);
            rewardLog->setAward(bit, kRewardPoster, rewardBit);
        }
        if (bit == kAWARD_PEAK1_EXPLORE_TROPHY)
            func_001E38B8(profile);
        this->awardUserPeakPass(profile, iCharacterID, bit, kPEAK_2);
        break;
    }
    case kAWARD_PEAK2_RACE_TROPHY:
    case kAWARD_PEAK2_FREESTYLE_TROPHY:
    case kAWARD_PEAK2_EXPLORE_TROPHY:
    case kAWARD_PEAK2_EARNINGS_TROPHY:
    {
        int i;
        int rewardBit = this->getRandomLockedRewardIndex(profile, iCharacterID, kRewardToy);
        if (rewardBit >= 0) {
            this->buyToy(profile, iCharacterID, rewardBit, 0);
            rewardLog->setAward(bit, kRewardToy, rewardBit);
        }
        for (i = 0; i < 2 && (slots2.v[i] = this->getRandomLockedRewardIndex(profile, iCharacterID, kRewardArt)) >= 0;
             i++) {
            this->buyConceptArt(profile, iCharacterID, slots2.v[i], 0);
            rewardLog->setAward(bit, kRewardArt, slots2.v[i]);
        }
        if (bit == kAWARD_PEAK2_EXPLORE_TROPHY)
            func_001E38B8(profile);
        this->awardUserPeakPass(profile, iCharacterID, bit, kPEAK_3);
        break;
    }
    case kAWARD_PEAK3_RACE_TROPHY:
        value = 27;
        break;
    case kAWARD_PEAK3_FREESTYLE_TROPHY:
        value = 23;
        break;
    case kAWARD_PEAK3_EXPLORE_TROPHY:
        value = 0x1C;
        break;
    case kAWARD_PEAK3_EARNINGS_TROPHY:
        value = 21;
        break;
    case kAWARD_PEAK1_EVENT_MEDAL:
        item = func_00156C70(this, profile, iCharacterID, kPEAK_1);
        break;
    case kAWARD_PEAK2_EVENT_MEDAL:
        item = func_00156C70(this, profile, iCharacterID, kPEAK_2);
        break;
    case kAWARD_PEAK3_EVENT_MEDAL:
        item = func_00156C70(this, profile, iCharacterID, kPEAK_3);
        break;
    case kAWARD_PEAK1_RIVALTIME_MEDAL:
    case kAWARD_PEAK2_RIVALTIME_MEDAL:
    case kAWARD_PEAK3_RIVALTIME_MEDAL:
    case kAWARD_PEAK1_RIVALPOINTS_MEDAL:
    case kAWARD_PEAK2_RIVALPOINTS_MEDAL:
    case kAWARD_PEAK3_RIVALPOINTS_MEDAL:
    case kAWARD_PEAK1_TIMECHALLENGE_MEDAL:
    case kAWARD_PEAK2_TIMECHALLENGE_MEDAL:
    case kAWARD_PEAK3_TIMECHALLENGE_MEDAL:
    case kAWARD_PEAK1_POINTSCHALLENGE_MEDAL:
    case kAWARD_PEAK2_POINTSCHALLENGE_MEDAL:
    case kAWARD_PEAK3_POINTSCHALLENGE_MEDAL:
        item = func_00156EE0(this, profile, iCharacterID);
        break;
    }

    this->setBit(bit, (unsigned char*)(&rec->mAwardsReceived[0]));
    if (value != 0) {
        rewardLog->setAward(bit, kRewardCheatChar, value);
        this->buyCheatChar(profile, iCharacterID, value, 0);
    }
    if (item != kNoBoltID) {
        void* stats = cBE_getInterface_Fv(cBE_getBE(), 9);
        rewardLog->setAward(bit, kRewardBolt, item);
        func_0014B560(stats, profile, iCharacterID, item, 0);
    }
    if (this->isMountainConquered(profile, iCharacterID)) {
        if (!this->isAwardReceived(profile, iCharacterID, kAWARD_MOUNTAIN_CONQUERED))
            awardUser(profile, iCharacterID, kAWARD_MOUNTAIN_CONQUERED);
    }
    if (this->isMountainCompleted(profile, iCharacterID)) {
        if (!this->isAwardReceived(profile, iCharacterID, kAWARD_MOUNTAIN_COMPLETED))
            awardUser(profile, iCharacterID, kAWARD_MOUNTAIN_COMPLETED);
    }
    for (int i = 0; i < 3; i++) {
        if (this->isPeakCompleted(profile, iCharacterID, i)) {
            if (!this->isAwardReceived(profile, iCharacterID, i + kAWARD_PEAK1_COMPLETED))
                awardUser(profile, iCharacterID, i + kAWARD_PEAK1_COMPLETED);
        }
    }
    for (int kind = 0; kind < 3; kind++) {
        for (int tier = 0; tier < 4; tier++) {
            int trophyCategory = this->getAwardTrophyCategory(profile, iCharacterID, kind, tier);
            if (this->isTrophyOwned(profile, iCharacterID, kind, tier)) {
                if (!this->isAwardReceived(profile, iCharacterID, trophyCategory))
                    this->awardUserTrophy(profile, iCharacterID, kind, tier);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_isAwardReceived);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
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
int cBERewardInterface::isAwardReceived(int iProfileIndex, int iCharacterID, int bit)
{
    cBECharProfileDB* profileRec = &cBELibrary_profileLib[iProfileIndex][iCharacterID];
    return isBitSet(bit, (unsigned char*)(&profileRec->mAwardsReceived[0]));
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
int cBERewardInterface::isBitSet(int bit, unsigned char* bits)
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
void cBERewardInterface::setBit(int bit, unsigned char* bits)
{
    int shift = bit % 8;
    bits[(unsigned int)bit / 8] |= 1 << shift;
}
#endif

extern tBELocationDB cBELibrary_locationDB[];

//100%
INCLUDE_ASM("be/beintreward", cBERewardInterface_getTrackPeakID);
#ifdef SKIP_ASM
/**
 * Returns the peak a location belongs to: the word at +0x54 of its 0x64-byte cBELibrary_locationDB entry.
 *
 * @param locationID  the location (On Tour locationID)
 * @return the peak ID
 */
int cBERewardInterface::getTrackPeakID(int locationID)
{
    return cBELibrary_locationDB[locationID].segmentID;
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
int cBERewardInterface::getTrackTrophyID(int eventID, int locationID)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                if (gMedalsInfo[i][j][k].a == eventID)
                {
                    if (gMedalsInfo[i][j][k].b == locationID || eventID == kGAMEEVENT_TIME_PEAK1
                        || eventID == kGAMEEVENT_TIME_PEAK2 || eventID == kGAMEEVENT_TIME_PEAK3
                        || eventID == kGAMEEVENT_POINTS_PEAK1 || eventID == kGAMEEVENT_POINTS_PEAK2
                        || eventID == kGAMEEVENT_POINTS_PEAK3)
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
cBERewardDB* cBERewardInterface::getDB()
{
    return (cBERewardDB*)(void*)cBELibrary_rewardDB;
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
        self->mpAwardBits[i] = 0;
    self->mpAwardBits[0] = (unsigned char*)(&self->mAwards.bTrophies[0]);
    self->mpAwardBits[2] = (unsigned char*)(&self->mAwards.bPeakPasses[0]);
    self->mpAwardBits[3] = (unsigned char*)(&self->mAwards.bPosters[0]);
    self->mpAwardBits[4] = (unsigned char*)(&self->mAwards.bTradingCards[0]);
    self->mpAwardBits[5] = (unsigned char*)(&self->mAwards.bConceptArts[0]);
    self->mpAwardBits[7] = (unsigned char*)(&self->mAwards.bToys[0]);
    self->mpAwardBits[8] = (unsigned char*)(&self->mAwards.bCheatChars[0]);
    self->clear();
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
void cBEUserAwards::clear()
{
    MEM_fill(this, 0, 372);
    mAwards.iCharBoltID = -1;
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
int cBEUserAwards::coutBits(int n, unsigned char* bits)
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
void cBEUserAwards::setBit(int bit, unsigned char* bits)
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
int cBEUserAwards::isBitSet(int bit, unsigned char* bits)
{
    int shift = bit % 8;
    return (bits[(unsigned int)bit / 8] >> shift) & 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintreward", cBEUserAwards_setAward);
#ifdef SKIP_ASM
/**
 * Records one reward in the user-awards log under an award category.
 *
 * Columns 0-7 (reward types) set bit @p value in the type's bits (mpAwardBits[col], cBEUserAwards_setBit) and count it
 * in iNumAwards[row][col]. Column kRewardCheatChar (8) stores @p value as iNumAwards[row][8] (a cheat character's id);
 * column kRewardBolt (9) sets iNumAwards[row][9] to 1 and iCharBoltID to @p value (an item id).
 * cBERewardInterface_awardUser and cBERewardInterface_awardUserPeakPass call it on cBELibrary_userAwards.
 *
 * @param row  the award category (On Tour awardCategory)
 * @param col  the reward type (tRewardType; On Tour rewardType)
 * @param value  the reward index or id (On Tour index)
 */
void cBEUserAwards::setAward(int row, int col, int value)
{
    switch (col)
    {
    case kRewardTrophy:
    case kRewardMedal:
    case kRewardPeakPass:
    case kRewardPoster:
    case kRewardCard:
    case kRewardArt:
    case kRewardVideo:
    case kRewardToy:
        setBit(value, mpAwardBits[col]);
        mAwards.iNumAwards[row][col]++;
        break;
    case kRewardCheatChar:
        mAwards.iNumAwards[row][kRewardCheatChar] = value;
        break;
    case kRewardBolt:
        mAwards.iNumAwards[row][kRewardBolt] = 1;
        mAwards.iCharBoltID = value;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBEUserAwards_getTotalNumAwards);
#ifdef SKIP_ASM
// PORT: the unit declares cBEUserAwards_getTotalNumAwards as returning void* (line ~378, used by
// cBERewardInterface_getTotalNumUserAwards); the body returns an int sum, so it is bound by asm label.
// The lander should change that declaration to int instead.

/**
 * Counts all awards recorded in a user-awards table.
 *
 * Sums the 32 x 10 signed bytes of iNumAwards. The C definition is cBEUserAwards_getTotalNumAwards (bound to the symbol
 * by asm label) because the unit declares cBEUserAwards_getTotalNumAwards as returning void *.
 *
 * @return the total award count
 */
int cBEUserAwards::getTotalNumAwards()
{
    int sum = 0;
    for (int i = 0; i < 32; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            sum += mAwards.iNumAwards[i][j];
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintreward", cBEUserAwards_getNumAwards);
#ifdef SKIP_ASM
/**
 * Returns one count of the user-awards log: iNumAwards[@p row][@p column], the signed byte for award category
 * @p row and reward type @p column.
 *
 * @param row  the award category (On Tour awardCategory)
 * @param column  the reward type (On Tour rewardType)
 * @return the count
 */
signed char cBEUserAwards::getNumAwards(int row, int column)
{
    return mAwards.iNumAwards[row][column];
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
int cBEUserAwards::getAwardBits(int i)
{
    return (int)mpAwardBits[i];
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
extern "C" void cBERewardDB__cBERewardDB(cBERewardDB* self, int flags)
{
    cBERewardDB_purge(self);
    if (flags & 1)
    {
        operator_delete(((int*)self));
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
 * Stores @p data at +0x4 (On Tour mpRewardsFile). The file starts with nine (type (tRewardType), count) word pairs,
 * each followed by its records: type 0 trophies and 1 medals (3 string words each), 2 peak passes (2), 3 posters, 4
 * trading cards, 5 concept arts, 6 videos, 7 toys and 8 cheat characters (16-byte records, 3 strings). The tables and
 * counts land at On Tour cBERewardDB's mpTrophies..mpCheatChars (+0x8 to +0x28) and mNumTrophies..mNumCheatChars (+0x30
 * to +0x50).
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
            case kRewardTrophy:
                self->t0 = (sRewardStr3*)cursor;
                self->n0 = count;
                cursor = (int*)((sRewardStr3*)cursor + count);
                break;
            case kRewardMedal:
                self->t1 = (sRewardStr3*)cursor;
                self->n1 = count;
                cursor = (int*)((sRewardStr3*)cursor + count);
                break;
            case kRewardPeakPass:
                self->t2 = (sRewardStr2*)cursor;
                self->n2 = count;
                cursor = (int*)((sRewardStr2*)cursor + count);
                break;
            case kRewardPoster:
                self->t3 = (sRewardStr4*)cursor;
                self->n3 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            case kRewardCard:
                self->t4 = (sRewardStr4*)cursor;
                self->n4 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            case kRewardArt:
                self->t5 = (sRewardStr4*)cursor;
                self->n5 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            case kRewardVideo:
                self->t6 = (sRewardStr4*)cursor;
                self->n6 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            case kRewardToy:
                self->t7 = (sRewardStr4*)cursor;
                self->n7 = count;
                cursor = (int*)((sRewardStr4*)cursor + count);
                break;
            case kRewardCheatChar:
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

