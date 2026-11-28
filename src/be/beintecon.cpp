#include "common.h"
#include "vtable.h"
#include "be/beinterface.h"
#include "be/beprofilelib.h"
#include "be/bedbprofile.h"
#include "be/beintecon.h"
#include "be/beintnewrace.h"
#include "be/beintreward.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BEEconInt[];
extern void* cBEEconInterface_vtbl[16];


//99.58%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the back end's economy interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BEEconInt", clears the word at +0x8 and stores the vtable.
 *
 * @return the economy interface
 */
void* cBEEconInterface_getThis()
{
    if (cBEEconInterface_instance == 0) {
        cBEEconInterface* mem = (cBEEconInterface*)cMemMan_alloc(0x10, str_BEEconInt, 0, 0);
        mem->bIsActive = 0;
        *(void**)&VPTR_OF(cBEInterface, mem) = cBEEconInterface_vtbl;
        cBEEconInterface_instance = (cBEEconInterface*)(mem);
    }
    return cBEEconInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getMoney);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns a character's cash (mMoney of its profile record).
 *
 * @param iHuman  the profile index; 2 or more gives 0
 * @param charType  the character id
 * @return the cash, or 0
 */
int cBEEconInterface::getMoney(int iHuman, int charType)
{
    if (iHuman >= 2) {
        return 0;
    }
    return cBELibrary_profileLib[iHuman][charType].mMoney;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getMoney1);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

/**
 * Returns @p rider's money: mMoney of its character profile (cBELibrary_profileLib[profile][character]).
 *
 * @param rider  the rider (selects profile slot and character)
 * @return the money
 */
int cBEEconInterface::getMoney1(int rider)
{
    int profileSlot = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    return cBELibrary_profileLib[profileSlot][characterId].mMoney;
}
#endif

extern cBECharProfileDB cBELibrary_profileLib[][10];

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getTotalMoneyEarned__FPvii);
#ifdef SKIP_ASM
/**
 * Returns the total money a character has earned (mTotalMoneyEarned of its profile record).
 *
 * @param iHuman  the profile index; 2 or more gives 0
 * @param charType  the character id
 * @return the total earned, or 0
 */
int cBEEconInterface::getTotalMoneyEarned(int iHuman, int charType)
{
    if (iHuman >= 2) {
        return 0;
    }
    return cBELibrary_profileLib[iHuman][charType].mTotalMoneyEarned;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getTotalMoneyEarned1);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

/**
 * Returns the total money earned by a rider's character, found from the rider's profile and character.
 *
 * @param rider  the rider index (On Tour: iRider)
 * @return the total earned (mTotalMoneyEarned of the profile record)
 */
int cBEEconInterface::getTotalMoneyEarned1(int rider)
{
    int profileIndex = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    return cBELibrary_profileLib[profileIndex][characterId].mTotalMoneyEarned;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_setMoney);
#ifdef SKIP_ASM
/**
 * Sets the money (mMoney) of character @p characterId in profile slot @p profileIndex; does nothing for slots >= 2.
 *
 * @param profileIndex  the profile slot (only 0 and 1 are stored)
 * @param characterId  the character
 * @param value  the new amount
 */
void cBEEconInterface::setMoney(int profileIndex, int characterId, int value)
{
    if (profileIndex < 2) {
        cBELibrary_profileLib[profileIndex][characterId].mMoney = value;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_addMoney);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Adds @p amount to the money (mMoney) and the total money earned (mTotalMoneyEarned) of character @p characterId in
 * profile slot @p humanIndex.
 *
 * @param humanIndex  the profile slot (0 or 1; 2 and up do nothing and return 0)
 * @param characterId  the character
 * @param amount  the money earned
 * @return the new money, or 0
 */
int cBEEconInterface::addMoney(int humanIndex, int characterId, int amount)
{
    if (humanIndex >= 2) {
        return 0;
    }
    cBELibrary_profileLib[humanIndex][characterId].mMoney += amount;
    cBELibrary_profileLib[humanIndex][characterId].mTotalMoneyEarned += amount;
    return cBELibrary_profileLib[humanIndex][characterId].mMoney;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_addMoney1);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

/**
 * Adds @p amount to @p rider's money and total earned: cBEEconInterface_addMoney with the rider's profile slot and character.
 *
 * @param rider  the rider
 * @param amount  the money earned
 * @return the new money, or 0 for profile slots >= 2
 */
int cBEEconInterface::addMoney1(int rider, int amount)
{
    int profile = cBELibrary_getProfileIndex(rider);
    return addMoney(profile, cBELibrary_getCharacterID(rider), amount);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_reduceMoney);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Subtracts @p amount from the money (mMoney) of character @p characterId in profile slot @p profileIndex.
 *
 * The total earned (mTotalMoneyEarned) is left alone; there is no check that the balance covers the amount.
 *
 * @param profileIndex  the profile slot (0 or 1; 2 and up do nothing and return 0)
 * @param characterId  the character
 * @param amount  the money spent
 * @return the money left, or 0
 */
int cBEEconInterface::reduceMoney(int profileIndex, int characterId, int amount)
{
    if (profileIndex >= 2) {
        return 0;
    }
    return cBELibrary_profileLib[profileIndex][characterId].mMoney -= amount;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getPeakPass);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Unlocks or locks peak @p peak (kPEAK_2 or kPEAK_3) for character @p characterId of profile slot @p profileIndex:
 * stores !@p unlocked in bPeak2Locked (peak 1) or bPeak3Locked (peak 2) of the character's profile record.
 *
 * Other peak numbers change nothing.
 *
 * @param profileIndex  the profile slot (0 or 1; 2 and up return 0)
 * @param characterId  the character
 * @param peak  the peak (eBEPeakID), kPEAK_2 or kPEAK_3
 * @param unlocked  nonzero to unlock
 * @return true, or false for profile slots >= 2
 */
// Sets the locked state of a peak for a profile/character: stores the inverse of `unlocked`.
bool cBEEconInterface::getPeakPass(int profileIndex, int characterId, int peak, int unlocked)
{
    if (profileIndex >= 2) {
        return false;
    }
    unlocked ^= 1;
    if (peak == kPEAK_2) {
        cBELibrary_profileLib[profileIndex][characterId].bPeak2Locked = unlocked;
    } else if (peak == kPEAK_3) {
        cBELibrary_profileLib[profileIndex][characterId].bPeak3Locked = unlocked;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_buyAttribute);
#ifdef SKIP_ASM
class cBEIface_0C20 {
public:
    char pad[0xC];
    virtual void v1();
    virtual void discard();
};
extern "C" cBEIface_0C20* cBEStatInterface_getThis();
int cBEEconInterface_getAttribCost(void* self, int a1);
/**
 * Buys one point of attribute @p attribute for character @p characterId of profile slot @p humanIndex, if affordable.
 *
 * The seven attribute bytes are the character profile's statDB. The price is cBEEconInterface_getAttribCost(points / 5
 * - 1); when the money (mMoney) covers it and the attribute is not at kAttribPointsMax (55), the money drops by the
 * price, the attribute rises by 1 and the stat interface's vtable slot 2 runs.
 *
 * @param humanIndex  the profile slot (2 and up return 0)
 * @param characterId  the character
 * @param attribute  the attribute (eBEAttributes), 0-6 (others read a null pointer)
 * @return true if bought
 */
bool cBEEconInterface::buyAttribute(int humanIndex, int characterId, unsigned int attribute)
{
    signed char *attribLevel;
    int cur;
    int cost;

    if (humanIndex >= 2) {
        return false;
    }
    {
        attribLevel = 0;
        cur = cBELibrary_profileLib[humanIndex][characterId].mMoney;
        switch (attribute) {
        case kATTRIB_SPEED:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cSpeed;
            break;
        case kATTRIB_ACCEL:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cAccel;
            break;
        case kATTRIB_TRICK:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cTrick;
            break;
        case kATTRIB_EDGING:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cEdging;
            break;
        case kATTRIB_SPIN:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cSpin;
            break;
        case kATTRIB_TOUGH:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cToughness;
            break;
        case kATTRIB_STAB:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cStability;
            break;
        }
        cost = this->getAttribCost(*attribLevel / 5 - 1);
        if (cur < cost || *attribLevel == kAttribPointsMax) {
            return false;
        }
        cBELibrary_profileLib[humanIndex][characterId].mMoney -= cost;
        *attribLevel = (unsigned char)*attribLevel + 1;
        cBEStatInterface_getThis()->discard();
        return true;
    }
}
#endif

extern void* cBELibrary_attribCost[];

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getAttribCost__FPvi);
#ifdef SKIP_ASM
struct sEconEntry {
    int key;
    int value;
};

struct sEconTable {
    sEconEntry entries[1];
};

/**
 * Returns the price of an attribute point at level @p levelIndex: the second word of entry @p levelIndex of cBELibrary_attribCost.
 *
 * @param levelIndex  the level index (callers pass level - 1)
 * @return the price
 */
int cBEEconInterface::getAttribCost(int levelIndex)
{
    return ((sEconTable*)cBELibrary_attribCost)->entries[levelIndex].value;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_sellAttribute);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sEcon0E68 { int a; int b; };
extern sEcon0E68 cBELibrary_attribCost_0E68[] __asm__("cBELibrary_attribCost");

/**
 * Refunds one point of attribute @p kind for character @p characterId of slot @p profile, unless the attribute is at
 * kAttribPointsMin (5).
 *
 * Adds the price cBELibrary_attribCost[(points - 1) / 5] to the money (mMoney), then increments the attribute byte
 * (statDB byte @p kind) instead of decrementing it (likely a bug; nothing calls this function).
 *
 * @param profile  the profile slot (2 and up return 0)
 * @param characterId  the character
 * @param kind  the attribute (eBEAttributes), 0-6
 * @return true if refunded
 */
bool cBEEconInterface::sellAttribute(int profile, int characterId, unsigned int kind)
{
    signed char* attribLevel;
    if (profile >= 2) return false;
    attribLevel = 0;
    switch (kind) {
    case kATTRIB_SPEED: attribLevel = &cBELibrary_profileLib[profile][characterId].statDB.cSpeed; break;
    case kATTRIB_ACCEL: attribLevel = &cBELibrary_profileLib[profile][characterId].statDB.cAccel; break;
    case kATTRIB_TRICK: attribLevel = &cBELibrary_profileLib[profile][characterId].statDB.cTrick; break;
    case kATTRIB_EDGING: attribLevel = &cBELibrary_profileLib[profile][characterId].statDB.cEdging; break;
    case kATTRIB_SPIN: attribLevel = &cBELibrary_profileLib[profile][characterId].statDB.cSpin; break;
    case kATTRIB_TOUGH: attribLevel = &cBELibrary_profileLib[profile][characterId].statDB.cToughness; break;
    case kATTRIB_STAB: attribLevel = &cBELibrary_profileLib[profile][characterId].statDB.cStability; break;
    }
    if (*attribLevel == kAttribPointsMin) return false;
    {
        int refund = cBELibrary_attribCost_0E68[(*attribLevel - 1) / 5].b;
        cBELibrary_profileLib[profile][characterId].mMoney += refund;
    }
    (*attribLevel)++;
    return true;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getCashReward);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getPeakRewardAmount(void* race, int place, int kind);

struct sEconPrize_151040
{
    int key;
    int prize[4];
};
extern sEconPrize_151040 cBELibrary_cashPrizeTable[];

/**
 * Returns the prize money for medal @p medal.
 *
 * For @p place 6-11: cBENewRaceInterface_getPeakRewardAmount(race, @p place, tier) with tier 2 for medals 0-1, 1 for medal 2 and 0 for medal 3.
 * Otherwise the cBELibrary_cashPrizeTable row (17 rows) whose key is @p key, column @p medal, times 100. 0 when nothing matches.
 *
 * @param medal  the medal (eBEMedalID), 0-3
 * @param key  the track key matched against cBELibrary_cashPrizeTable
 * @param place  the event (6-11 are the peak events)
 * @return the money
 */
int cBEEconInterface::getCashReward(int medal, int key, int place)
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int reward = 0;
    if ((unsigned int)(place - 6) < 6)
    {
        switch (medal)
        {
        case kMEDAL_PLATINUM:
        case kMEDAL_GOLD:
            reward = cBENewRaceInterface_getPeakRewardAmount(race, place, 2);
            break;
        case kMEDAL_SILVER:
            reward = cBENewRaceInterface_getPeakRewardAmount(race, place, 1);
            break;
        case kMEDAL_BRONZE:
            reward = cBENewRaceInterface_getPeakRewardAmount(race, place, 0);
            break;
        }
    }
    else
    {
        for (int i = 0; i < 17; i++)
        {
            if (cBELibrary_cashPrizeTable[i].key != key) continue;
            switch (medal)
            {
            case kMEDAL_PLATINUM:
                reward = cBELibrary_cashPrizeTable[i].prize[0] * 100;
                break;
            case kMEDAL_GOLD:
                reward = cBELibrary_cashPrizeTable[i].prize[1] * 100;
                break;
            case kMEDAL_SILVER:
                reward = cBELibrary_cashPrizeTable[i].prize[2] * 100;
                break;
            case kMEDAL_BRONZE:
                reward = cBELibrary_cashPrizeTable[i].prize[3] * 100;
                break;
            }
        }
    }
    return reward;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getCollectableCash);
#ifdef SKIP_ASM
/**
 * Returns the cash a collectible is worth on peak @p level: 1000 for 1, 2000 for 2, otherwise 500.
 *
 * @param level  the peak (eBEPeakID; [O] On Tour eBEPeakID peak), 0-2
 * @return the cash
 */
int cBEEconInterface::getCollectableCash(int level)
{
    switch (level)
    {
    case kPEAK_1:
    default:
        return 500;
    case kPEAK_2:
        return 1000;
    case kPEAK_3:
        return 2000;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getMissionCash);
#ifdef SKIP_ASM
/**
 * Returns the cash a mission of difficulty @p level pays: 4000 for 2, 6000 for 3, otherwise 2000.
 *
 * @param level  the mission level, 1-3
 * @return the cash
 */
int cBEEconInterface::getMissionCash(int level)
{
    switch (level)
    {
    case 1:
    default:
        return 2000;
    case 2:
        return 4000;
    case 3:
        return 6000;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getCostForNextIncrement);
#ifdef SKIP_ASM
/**
 * Returns the attribute-cost table price for the next point of attribute @p attribute of character @p characterId in slot @p humanIndex:
 * cBELibrary_attribCost[(points + 1) / 5].
 *
 * Note cBEEconInterface_buyAttribute charges cBELibrary_attribCost[points / 5 - 1] instead.
 *
 * @param humanIndex  the profile slot (2 and up return 0)
 * @param characterId  the character
 * @param attribute  the attribute (eBEAttributes), 0-6
 * @return the price
 */
int cBEEconInterface::getCostForNextIncrement(int humanIndex, int characterId, unsigned int attribute)
{
    signed char *attribLevel;
    if (humanIndex >= 2) {
        return 0;
    }
    {
        attribLevel = 0;
        switch (attribute) {
        case kATTRIB_SPEED:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cSpeed;
            break;
        case kATTRIB_ACCEL:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cAccel;
            break;
        case kATTRIB_TRICK:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cTrick;
            break;
        case kATTRIB_EDGING:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cEdging;
            break;
        case kATTRIB_SPIN:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cSpin;
            break;
        case kATTRIB_TOUGH:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cToughness;
            break;
        case kATTRIB_STAB:
            attribLevel = &cBELibrary_profileLib[humanIndex][characterId].statDB.cStability;
            break;
        }
        return ((sEconTable*)cBELibrary_attribCost)->entries[(*attribLevel + 1) / 5].value;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", _$_16cBECharProfileDB);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

/**
 * Destroys a character profile: frees its item lookup table (+0x288, from cBECharProfileDB_buildBoltList) and clears the pointer; with
 * g++'s delete flag & 1 also frees the object.
 */
cBECharProfileDB::~cBECharProfileDB()
{
    void* buffer = mpBoltsMap;
    if (buffer != 0)
    {
        cMemMan_free(buffer);
    }
    mpBoltsMap = 0;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_applyStartingBolts);
#ifdef SKIP_ASM
int cBEBoltDB_getNumCharGroups(void* self, int kind);
extern "C" void* cBEBoltDB_getCharGroups(void* self, int kind);
int cBEBoltDB_getNumCharBolts(void* self, int i);
extern "C" void* cBEBoltDB_getCharBolts(void* self, int i);
extern "C" bool cBECharProfileDB_setBoltVisible(cBECharProfileDB* self, int id, int on);
extern char cBELibrary_boltDB[];



struct sEconGroup_1513B8
{
    int f0;
    short id;       // 0x4
    short prize;    // 0x6
};

struct sEconDef_1513B8
{
    int f0;
    short id;       // 0x4
    char pad6[0x8];
    short price;    // 0xE
    char pad10[0x28];
};

static inline tBEProfileBolt* Find_1513B8(cBECharProfileDB* self, int id)
{
    int idx = self->mpBoltsMap[id];
    if (idx >= 0)
        return &((&self->mBolts[0]))[idx];
    return 0;
}

static inline int Price_1513B8(sEconDef_1513B8* d)
{
    short v = d->price;
    int p = v;
    if (v > 0)
        p = v * 10;
    return p;
}

/**
 * Applies a character profile's starting item ownership: grants each group prize, marks owned items, and marks free
 * or built-in items.
 *
 * For the character kind (+0xBC0) from the item database cBELibrary_boltDB: owns each group's prize item (cBECharProfileDB_setBoltVisible);
 * owned items (flag 0x10) get flags 0x26; items priced 0 get flag 2, and items priced kBoltPriceBuiltIn (-1) also
 * become owned. If any did, owned items get flags 6 and the others lose flag 4. cBECharProfileDB_init and cBECharProfileDB_buildBoltList
 * call it.
 *
 */
void cBECharProfileDB::applyStartingBolts()
{
    cBECharProfileDB* self = this;
    void* db = cBELibrary_boltDB;
    int n = cBEBoltDB_getNumCharGroups(db, self->mCharacterID);
    sEconGroup_1513B8* prizeGroup = (sEconGroup_1513B8*)cBEBoltDB_getCharGroups(db, self->mCharacterID);
    for (int i = 0; i < n; i++, prizeGroup++)
    {
        self->setBoltVisible(prizeGroup->prize, 1);
    }
    int j;
    for (j = 0; j < self->mNumBolts; j++)
    {
        tBEProfileBolt* it = &((&self->mBolts[0]))[j];
        if ((*(short*)&it->flags) & 0x10)
            *(short*)&it->flags |= 0x26;
    }
    int numDefs = cBEBoltDB_getNumCharBolts(db, self->mCharacterID);
    sEconDef_1513B8* itemDef = (sEconDef_1513B8*)cBEBoltDB_getCharBolts(db, self->mCharacterID);
    int changed = 0;
    for (int i = 0; i < numDefs; i++, itemDef++)
    {
        if (Price_1513B8(itemDef) == 0)
        {
            (*(short*)&Find_1513B8(self, itemDef->id)->flags) |= 2;
        }
        else if (Price_1513B8(itemDef) == kBoltPriceBuiltIn)
        {
            (*(short*)&Find_1513B8(self, itemDef->id)->flags) |= 2;
            self->setBoltVisible(itemDef->id, 1);
            changed = 1;
        }
    }
    if (changed)
    {
        int k;
        for (k = 0; k < self->mNumBolts; k++)
        {
            tBEProfileBolt* it = &((&self->mBolts[0]))[k];
            if ((*(short*)&it->flags) & 0x10)
                *(short*)&it->flags |= 6;
            else
                *(short*)&it->flags &= ~4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_restoreVisibleFlags);
#ifdef SKIP_ASM

struct sEconItems_001515B8
{
    char pad[0x28C];
    int count;
    tBEProfileBolt items[1];
};

/**
 * Copies each item's flag 4 into its flag 0x10 for a character profile's item list (+0x28C count, items at +0x290).
 *
 * The save-file loader calls it for every loaded profile.
 */
void cBECharProfileDB::restoreVisibleFlags()
{
    int n = mNumBolts;
    tBEProfileBolt* item = ((&mBolts[0]));
    for (int i = 0; i < n; i++, item++)
    {
        if ((*(short*)&item->flags) & 4)
            *(short*)&item->flags |= 0x10;
        else
            *(short*)&item->flags &= ~0x10;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_init);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sEconBits_1600 {
    unsigned long bPeak1RivalPtsChallengePlayed : 1;
    unsigned long bPeak2RivalPtsChallengePlayed : 1;
    unsigned long bPeak3RivalPtsChallengePlayed : 1;
    unsigned long bPeak1RivalTimeChallengePlayed : 1;
    unsigned long bPeak2RivalTimeChallengePlayed : 1;
    unsigned long bPeak3RivalTimeChallengePlayed : 1;
    unsigned long bPeak1TimeRCLocked : 1;
    unsigned long bPeak2TimeRCLocked : 1;
    unsigned long bPeak3TimeRCLocked : 1;
    unsigned long bPeak1FreeRCLocked : 1;
    unsigned long bPeak2FreeRCLocked : 1;
    unsigned long bPeak3FreeRCLocked : 1;
    unsigned long bPeak2Locked : 1;
    unsigned long bPeak3Locked : 1;
    unsigned long bPeak1TCLocked : 1;
    unsigned long bPeak2TCLocked : 1;
    unsigned long bPeak3TCLocked : 1;
    unsigned long bPeak1FCLocked : 1;
    unsigned long bPeak2FCLocked : 1;
    unsigned long bPeak3FCLocked : 1;
};

struct sEconSlot_1600 {
    char b[8];
};

struct sEconEnt_1600 {
    signed char flag;               // 0x0
    char pad1[3];
    int data[2];                    // 0x4
};

struct sEconFlags_1600 {
    unsigned int f0 : 1;
    unsigned int f1 : 1;
    unsigned int f2 : 1;
    unsigned int f3 : 1;
    unsigned int f4 : 1;
};

struct sEconDef_1600 {
    char pad0[0x1A];
    signed char a;                  // 0x1A
    signed char b;                  // 0x1B
    char pad1C[0x8];
};

struct sEconCol_1600 {
    char b[7];
};

struct sEconColRec_1600 {
    sEconCol_1600 col;
    char pad[8];
};

struct sEcon_1600 {
    int f0;                         // 0x0
    sEconEnt_1600 ents[23][1];      // 0x4
    sEconFlags_1600 flags[0x58];    // 0x118
    char pad_0x278[0x0];
    sEconBits_1600 bits;            // 0x278
    char pad_0x280[0x0];
    short h280;                     // 0x280
    short h282;                     // 0x282
    short h284;                     // 0x284
    short h286;                     // 0x286
    char pad_0x288[0xAC4 - 0x288];
    int fAC4;                       // 0xAC4
    int fAC8;                       // 0xAC8
    int fACC;                       // 0xACC
    sEconSlot_1600 slots[26];       // 0xAD0
    char fBA0[0x18];                // 0xBA0
    char fBB8[0x8];                 // 0xBB8
    signed char kind;               // 0xBC0
    char pad_0xBC1[0xBDF - 0xBC1];
    sEconCol_1600 col;              // 0xBDF
    char fBE6[0xDE4 - 0xBE6];       // 0xBE6
    char fDE4[0x54];                // 0xDE4
    int pad_0xE38_[0];
    struct {
        int v;
        int pad;
    } fE38[25];                    // 0xE38
    char pad_0xF00_[0xF00 - 0xE38 - 25 * 8];
    unsigned int fF00;              // 0xF00
    signed char fF04;               // 0xF04
    char pad_0xF05[3];
    unsigned int fF08[2];           // 0xF08
    char pad_0xF10[0xF28 - 0xF10];
    char fF28[4];                   // 0xF28
    char pad_0xF2C[4];
    char fF30[0x50];                // 0xF30
    int fF80;                       // 0xF80
};

extern "C" void* memset(void* dst, int c, int n);
extern "C" void MEM_fill(void* dst, int c, int n);
struct sEconItems_151A88;
struct sEcon_001519E0;
extern "C" void cBECharProfileDB_buildBoltList(cBECharProfileDB* self, int k);
extern "C" void cBECharProfileDB_resetRelations(sEcon_001519E0* self, int k);
extern "C" void be_initStartingTricks(int kind, void* dst);
extern sEconDef_1600 cBELibrary_missionInfo_v1600[] __asm__("cBELibrary_missionInfo");
extern sEconColRec_1600 D_005308D9[];

/**
 * Resets one character's career profile to its starting state.
 *
 * With @p reload, cBECharProfileDB_buildBoltList first reloads the item data for character @p k. Then: the unlock bits at +0x278 get
 * their defaults, money (+0xAC4, +0xAC8) and the visited-locations mask (+0xACC) are zeroed, the 26 slots are blanked,
 * the 23 entries cleared, the 0x58 mission flags (+0x118) take their initial lock state from cBELibrary_missionInfo, the outfit
 * colours come from D_005308D9[@p k], cBECharProfileDB_resetRelations and be_initStartingTricks set the character kind's defaults, and the
 * remaining records are cleared; +0x27C ends as 0x11. Unlike On Tour's init(void) the retail version takes the
 * character and a reload flag.
 *
 * @param k  the character the profile belongs to
 * @param reload  nonzero to reload the character's item data first (cBECharProfileDB_buildBoltList)
 */
extern "C" void cBECharProfileDB_init(sEcon_1600* self, int k, int reload)
{
    if (reload) {
        ((cBECharProfileDB*)((sEconItems_151A88*)self))->buildBoltList(k);
    }
    self->bits.bPeak1RivalPtsChallengePlayed = 0;
    self->bits.bPeak2RivalPtsChallengePlayed = 0;
    self->bits.bPeak3RivalPtsChallengePlayed = 0;
    self->bits.bPeak1RivalTimeChallengePlayed = 0;
    self->bits.bPeak2RivalTimeChallengePlayed = 0;
    self->bits.bPeak3RivalTimeChallengePlayed = 0;
    self->bits.bPeak2Locked = 1;
    self->bits.bPeak3Locked = 1;
    self->bits.bPeak1TimeRCLocked = 1;
    self->bits.bPeak2TimeRCLocked = 1;
    self->bits.bPeak3TimeRCLocked = 1;
    self->bits.bPeak1FreeRCLocked = 1;
    self->bits.bPeak2FreeRCLocked = 1;
    self->bits.bPeak3FreeRCLocked = 1;
    self->bits.bPeak1TCLocked = 1;
    self->bits.bPeak2TCLocked = 1;
    self->bits.bPeak3TCLocked = 1;
    self->bits.bPeak1FCLocked = 1;
    self->bits.bPeak2FCLocked = 1;
    self->bits.bPeak3FCLocked = 1;
    self->f0 = 1;
    self->h280 = 1;
    self->h284 = 1;
    self->h282 = 0;
    self->h286 = 0;
    self->fACC = 0;
    memset(self->fBA0, 0, 0x18);
    sEconSlot_1600 blank;
    memset(&blank, 0, 8);
    blank.b[1] = -1;
    int i;
    for (i = 0; i < 17; i++) {
        for (int j = 0; j < 26; j++) {
            self->slots[j] = blank;
        }
    }
    for (i = 0; i < 1; i++) {
        for (int j = 0; j < 23; j++) {
            memset(self->ents[j][i].data, 0, 8);
            self->ents[j][i].flag = 0;
        }
    }
    for (i = 0; i < 88; i++) {
        sEconDef_1600* missionDef = &cBELibrary_missionInfo_v1600[i];
        self->flags[i].f0 = missionDef->a;
        self->flags[i].f1 = 1;
        self->flags[i].f2 = 0;
        self->flags[i].f3 = 0;
        self->flags[i].f4 = missionDef->b;
    }
    memset(&self->col, 1, 7);
    self->col = D_005308D9[k].col;
    self->fAC4 = 0;
    self->fAC8 = 0;
    cBECharProfileDB_resetRelations((sEcon_001519E0*)self, self->kind);
    be_initStartingTricks(self->kind, self->fBE6);
    memset(self->fBB8, 0, 8);
    memset(self->fDE4, 0, 84);
    self->fF04 = 0;
    for (int i = 0; i < 25; i++) {
        *(int*)((char*)self + 0xE38 + i * 8) = 0;
        self->fF00 &= ~(1 << i);
    }
    for (int i = 0; i < 61; i++) {
        self->fF08[i / 32] &= ~(1 << (i % 32));
    }
    MEM_fill(self->fF28, 0, 4);
    MEM_fill(self->fF30, 0, 80);
    self->fF80 = 0;
    *(int*)((char*)self + 0x27C) = 17;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_resetRelations);
#ifdef SKIP_ASM
struct sEconColor_001519E0
{
    signed char a;
    signed char b;
    signed char c;
};

struct sEconRange_001519E0
{
    int lo;
    int hi;
    int pad;
};

extern sEconColor_001519E0 D_004A1218[];
extern sEconRange_001519E0 D_004406FC[];

struct sEcon_001519E0
{
    char pad0[0xBC1];
    sEconColor_001519E0 colors[10];  // 0xBC1
};

/**
 * Resets a character profile's ten relation records (+0xBC1, 3 bytes each) to the default D_004A1218, then marks a
 * friend and an enemy.
 *
 * Record D_004406FC[@p k].lo becomes state 0, level 0; record D_004406FC[@p k].hi state 3, level 2 (the aggression
 * states of cBEAggressionInterface_applyAggression).
 *
 * @param k  the character the profile belongs to
 */
extern "C" void cBECharProfileDB_resetRelations(sEcon_001519E0* self, int k)
{
    sEconColor_001519E0 def = D_004A1218[0];
    for (int i = 0; i < 10; i++)
    {
        self->colors[i] = def;
        if (i == D_004406FC[k].lo)
        {
            self->colors[i].a = 0;
            self->colors[i].b = 0;
        }
        else if (i == D_004406FC[k].hi)
        {
            self->colors[i].a = 3;
            self->colors[i].b = 2;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintecon", cBECharProfileDB_buildBoltList);
#ifdef SKIP_ASM
int cBEBoltDB_getNumCharBolts(void* self, int i);
extern "C" void* cBEBoltDB_getCharBolts(void* self, int i);
extern "C" void cBECharProfileDB_applyStartingBolts(cBECharProfileDB* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern int kBolt_Num;
extern const char str_Profile_Bolts_Map[];
extern char cBELibrary_boltDB[];

struct sEconDef_151A88
{
    int f0;
    short id;           // 0x4
    char pad[0x32];
};


struct sEconItems_151A88
{
    char pad[0x288];
    short* lookup;              // 0x288
    int count;                  // 0x28C
    tBEProfileBolt items[0x20D]; // 0x290
    char pad2[0xBC0 - 0x290 - 0x20D * 4];
    signed char kind;           // 0xBC0
};

/**
 * Rebuilds a character profile's item list for character @p k from the item database and applies the starting
 * ownership.
 *
 * Sets the kind (+0xBC0) and count (+0x28C) from cBELibrary_boltDB; allocates the id-to-slot lookup (+0x288, kBolt_Num
 * shorts, "Profile_Bolts_Map") on first use and fills it with -1; gives each item a slot with flags 0 and clears the
 * rest of the 525 slots to id -1. Ends with cBECharProfileDB_applyStartingBolts.
 *
 * @param k  the character
 */
void cBECharProfileDB::buildBoltList(int k)
{
    void* db = cBELibrary_boltDB;
    sEconDef_151A88* itemDef = (sEconDef_151A88*)cBEBoltDB_getCharBolts(db, k);
    int n = cBEBoltDB_getNumCharBolts(db, k);
    mCharacterID = k;
    mNumBolts = n;
    if (mpBoltsMap == 0)
        mpBoltsMap = (short*)operator_new_tag(kBolt_Num * 2, str_Profile_Bolts_Map, 0, 0);
    int i;
    for (i = 0; i < kBolt_Num; i++)
        mpBoltsMap[i] = -1;
    for (i = 0; i < n; i++, itemDef++)
    {
        tBEProfileBolt* itemSlot = &((&mBolts[0]))[i];
        itemSlot->flags = 0;
        int id = itemDef->id;
        itemSlot->id = id;
        mpBoltsMap[itemDef->id] = i;
    }
    for (i = n; i < 525; i++)
    {
        tBEProfileBolt* emptySlot = &((&mBolts[0]))[i];
        emptySlot->id = -1;
        emptySlot->flags = 0;
    }
    applyStartingBolts();
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_normalizeBoltFlags);
#ifdef SKIP_ASM

struct sEconItems_151BD0
{
    char pad[0x288];
    short* lookup;                  // 0x288
    int count;                      // 0x28C
    tBEProfileBolt items[0x20D];  // 0x290
};

static inline unsigned int clearBits_151BD0(unsigned int f, unsigned int m)
{
    return f & ~m;
}

/**
 * Normalises a character profile's item flags: items with flag 0x20 get flags 0x16; the others lose flags 0x10 and 4.
 */
void cBECharProfileDB::normalizeBoltFlags()
{
    for (int i = 0; i < mNumBolts; i++) {
        tBEProfileBolt* item = &((&mBolts[0]))[i];
        unsigned int oldFlags = item->flags;
        item->flags = (oldFlags & 0x20) ? (oldFlags | 0x16) : clearBits_151BD0(clearBits_151BD0(oldFlags, 0x10), 4);
    }
}
#endif
//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_getBoltIdAt);
#ifdef SKIP_ASM
/**
 * Returns the item id in slot @p i of a profile's item list (+0x290), or -1 when @p i is past the count (+0x28C).
 *
 * @param i  the slot
 * @return the item id, or -1
 */
short cBECharProfileDB::getBoltIdAt(int i)
{
    if (i >= mNumBolts)
        return -1;
    return *(short*)((i << 2) + (int)this + 0x290);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_setBoltBought);
#ifdef SKIP_ASM

struct sEconItems_00151C48
{
    char pad[0x288];
    short* lookup;
    int count;
    tBEProfileBolt items[1];
};

/**
 * Sets or clears flag 2 of item @p id in a character profile's item list (through the lookup at +0x288).
 *
 * An id with no slot dereferences a null pointer.
 *
 * @param id  the item id
 * @param on  nonzero to set the flag
 */
void cBECharProfileDB::setBoltBought(int id, int on)
{
    int idx = mpBoltsMap[id];
    tBEProfileBolt* item;
    if (idx >= 0)
        item = &((&mBolts[0]))[idx];
    else
        item = 0;
    if (on)
        *(short*)&item->flags |= 2;
    else
        *(short*)&item->flags &= ~2;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_setBoltVisible);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: cBEBoltInterface_getBoltDB is declared (void*) but never reads its argument; called here with none
void* cBEBoltInterface_getBoltDB_1C90() __asm__("cBEBoltInterface_getBoltDB__FPv");
extern "C" int cBEBoltDB_findBoltIndex(void* db, int kind, int id);
extern "C" void* cBEBoltDB_getFirstChildBolt(void* db, int kind, int id);
extern "C" int cBEBoltDB_countChildBolts(void* db, int kind, int id);
extern "C" bool cBECharProfileDB_applyBoltRules(cBECharProfileDB* self, int id, int on);



struct sEconRel_151C90
{
    char pad0[0x4];
    short other;        // 0x4
    short pad6;
    char pad8[0x2C];
    int flags;          // 0x34
};

struct sEconDef_151C90
{
    char pad0[0x6];
    short rel;          // 0x6
    char pad8[0x2C];
    int flags;          // 0x34
};

struct sEconDefs_151C90
{
    char pad0[0x4];
    sEconDef_151C90* defs;  // 0x4
};

static inline tBEProfileBolt* getItem_151C90(cBECharProfileDB* self, int id)
{
    int idx = self->mpBoltsMap[id];
    if (idx >= 0)
        return &((&self->mBolts[0]))[idx];
    return 0;
}

static inline int isOwned_151C90(tBEProfileBolt* p)
{
    return p->flags & 0x10;
}

static inline int hasFlag_151C90(sEconDef_151C90* d, int mask)
{
    return d->flags & mask;
}

static inline sEconDef_151C90* getDef_151C90(sEconDefs_151C90* db, int i)
{
    if (i < 0)
        return 0;
    return &db->defs[i];
}

/**
 * Marks item @p id owned or not (flag 0x10) in a character profile, cascading to related items.
 *
 * Owning an item whose definition has flag 1 first un-owns the other members of its group (those owned or with flag
 * 8); un-owning one that was owned (or has flag 8) un-owns its dependants. A real change then applies the item's links
 * (cBECharProfileDB_applyBoltRules). Returns false as soon as a nested change fails.
 *
 * @param selfp  the character profile
 * @param id  the item id
 * @param on  nonzero to own
 * @return true on success, false on failure
 */
// Sets item id owned/unowned; turning on cascades turn-offs to conflicting items (def flag 1), turning off
// cascades to dependent items (owned before or def flag 8). Then applies the item's links; 0 on failure.
bool cBECharProfileDB::setBoltVisible(int id, int on)
{
    cBECharProfileDB* self = this;
    sEconDefs_151C90* db = (sEconDefs_151C90*)cBEBoltInterface_getBoltDB_1C90();
    sEconDef_151C90* def = getDef_151C90(db, cBEBoltDB_findBoltIndex(db, self->mCharacterID, id));
    tBEProfileBolt* item = getItem_151C90(self, id);
    int bit1 = hasFlag_151C90(def, 1) != 0;
    int bit8 = hasFlag_151C90(def, 8) != 0;
    int wasOn = isOwned_151C90(item) != 0;
    if (on)
        item->flags |= 0x10;
    else
        item->flags &= ~0x10;
    if (on && wasOn)
        return true;
    if (on && bit1)
    {
        int rid = def->rel;
        sEconRel_151C90* rel = (sEconRel_151C90*)cBEBoltDB_getFirstChildBolt(db, self->mCharacterID, rid);
        int n = cBEBoltDB_countChildBolts(db, self->mCharacterID, rid);
        for (int i = 0; i < n; i++, rel++)
        {
            int sid = rel->other;
            if (sid != id)
            {
                int j = self->mpBoltsMap[sid];
                if (j >= 0 && (isOwned_151C90(&((&self->mBolts[0]))[j]) || (rel->flags & 8)))
                {
                    if (self->setBoltVisible(sid, 0) == 0)
                        return false;
                }
            }
        }
    }
    else if (!on && (wasOn || bit8))
    {
        sEconRel_151C90* rel = (sEconRel_151C90*)cBEBoltDB_getFirstChildBolt(db, self->mCharacterID, id);
        int n = cBEBoltDB_countChildBolts(db, self->mCharacterID, id);
        for (int i = 0; i < n; i++, rel++)
        {
            if (self->setBoltVisible(rel->other, 0) == 0)
                return false;
        }
    }
    if ((on && !wasOn) || (!on && wasOn))
    {
        if (self->applyBoltRules(id, on) == 0)
            return false;
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_applyBoltRules);
#ifdef SKIP_ASM
// PORT: cBEBoltInterface_getBoltDB is declared (void*) but never reads its argument; called here with none
void* cBEBoltInterface_getBoltDB_1EF0() __asm__("cBEBoltInterface_getBoltDB__FPv");
struct sEconLink_151EF0;
extern "C" sEconLink_151EF0* cBEBoltDB_getBoltRules(void* db, int kind, int id, int on, int* n);
extern "C" int cBEBoltDB_findBoltIndex(void* db, int kind, int id);
extern "C" bool cBECharProfileDB_setBoltVisible(cBECharProfileDB* self, int id, int on);



// One dependency: when condId is set, applies only if its owned state equals cond.
struct sEconLink_151EF0
{
    char pad0[0x5];
    signed char cond;   // 0x5
    short condId;       // 0x6
    char pad8;
    signed char on;     // 0x9
    short target;       // 0xA
};

struct sEconDef_151EF0
{
    char pad0[0x34];
    int flags;          // 0x34
};

struct sEconDefs_151EF0
{
    char pad0[0x4];
    sEconDef_151EF0* defs;  // 0x4
};

static inline tBEProfileBolt* getItem_151EF0(cBECharProfileDB* self, int id)
{
    int idx = self->mpBoltsMap[id];
    if (idx >= 0)
        return &((&self->mBolts[0]))[idx];
    return 0;
}

static inline int isOwned_151EF0(tBEProfileBolt* p)
{
    return p->flags & 0x10;
}

static inline sEconDef_151EF0* getDef_151EF0(sEconDefs_151EF0* db, int i)
{
    if (i < 0)
        return 0;
    return &db->defs[i];
}

/**
 * Applies the dependency links of item @p id for the given owned state: each link may own or un-own another item.
 *
 * Links come from cBEBoltDB_getBoltRules. A link is skipped when its target is kNoBoltID (-1), when its condition item's owned
 * state differs from the link's, or when the target is already in the wanted state (unless it is owned and its
 * definition has flag 8). A link to @p id itself only sets its flag; others go through cBECharProfileDB_setBoltVisible.
 *
 * @param selfp  the character profile
 * @param id  the item id
 * @param on  the item's new owned state
 * @return true on success, false if a nested change failed
 */
// Sets item id owned/unowned and applies its dependency links (cascading through cBECharProfileDB_setBoltVisible); 0 on failure.
bool cBECharProfileDB::applyBoltRules(int id, int on)
{
    cBECharProfileDB* self = this;
    sEconDefs_151EF0* db = (sEconDefs_151EF0*)cBEBoltInterface_getBoltDB_1EF0();
    int n = 0;
    sEconLink_151EF0* link = cBEBoltDB_getBoltRules(db, self->mCharacterID, id, on, &n);
    tBEProfileBolt* item = getItem_151EF0(self, id);
    for (int i = 0; i < n; i++, link++)
    {
        if (link->target == kNoBoltID)
            continue;
        if (link->condId != kNoBoltID)
        {
            if ((isOwned_151EF0(getItem_151EF0(self, link->condId)) != 0) != (link->cond != 0))
                continue;
        }
        tBEProfileBolt* targetItem = &((&self->mBolts[0]))[self->mpBoltsMap[link->target]];
        int has = isOwned_151EF0(targetItem) != 0;
        if (has == (link->on != 0))
        {
            if (has)
                continue;
            if (!(getDef_151EF0(db, cBEBoltDB_findBoltIndex(db, self->mCharacterID, link->target))->flags & 8))
                continue;
        }
        if (targetItem->id == id)
        {
            if (link->on)
                item->flags |= 0x10;
            else
                item->flags &= ~0x10;
        }
        else if (self->setBoltVisible(targetItem->id, link->on != 0) == 0)
        {
            return false;
        }
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_reapplyBoltRules);
#ifdef SKIP_ASM
void* cBEBoltInterface_getBoltDB(void* self);
int cBEBoltDB_getNumCharBolts(void* self, int i);
extern "C" void* cBEBoltDB_getCharBolts(void* self, int i);
extern "C" bool cBECharProfileDB_applyBoltRules(cBECharProfileDB* self, int id, int on);



struct sEconDef_001520E8
{
    char pad0[0x4];
    short id;           // 0x4
    char pad6[0x2E];
    int flags;          // 0x34
};

static inline tBEProfileBolt* getItem_001520E8(cBECharProfileDB* self, int id)
{
    int idx = self->mpBoltsMap[id];
    if (idx >= 0)
        return &((&self->mBolts[0]))[idx];
    return 0;
}

/**
 * Re-applies the links of every owned item whose definition has flag 0x1000, and reports whether there were any.
 *
 * @param changed  receives 1 when at least one such item was owned, else 0
 * @return true on success, false if applying links failed
 */
bool cBECharProfileDB::reapplyBoltRules(int* changed)
{
    void* db = cBEBoltInterface_getBoltDB((this));
    int n = cBEBoltDB_getNumCharBolts(db, mCharacterID);
    sEconDef_001520E8* def = (sEconDef_001520E8*)cBEBoltDB_getCharBolts(db, mCharacterID);
    *changed = 0;
    for (int i = 0; i < n; i++, def++)
    {
        if (def->flags & 0x1000)
        {
            if (getItem_001520E8((this), def->id)->flags & 0x10)
            {
                *changed = 1;
                if (applyBoltRules(def->id, 1) == 0)
                    return false;
            }
        }
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_showDefaultBolts);
#ifdef SKIP_ASM
// PORT: cBEBoltInterface_getBoltDB is declared (void*) but never reads its argument; called here with none
void* cBEBoltInterface_getBoltDB_noarg() __asm__("cBEBoltInterface_getBoltDB__FPv");
int cBEBoltDB_getNumCharGroups(void* self, int kind);
extern "C" void* cBEBoltDB_getCharGroups(void* self, int kind);
extern "C" int cBEBoltDB_collectChildBolts(void* self, int kind, int id, void** out, int a, int b);
extern "C" bool cBECharProfileDB_setBoltVisible(cBECharProfileDB* self, int id, int on);



struct sEconGroup_1521F0
{
    int f0;
    short id;       // 0x4
    short prize;    // 0x6
};

struct sEconDef_1521F0
{
    int f0;
    short id;       // 0x4
    char pad[0x2E];
    int flags;      // 0x34
};

static inline tBEProfileBolt* Find_1521F0(cBECharProfileDB* self, int id)
{
    int idx = self->mpBoltsMap[id];
    if (idx >= 0)
        return &((&self->mBolts[0]))[idx];
    return 0;
}

/**
 * Gives a character profile each group's prize item when none of the group's owned items has definition flag 0x10.
 *
 * For each group of the character kind: lists the group's items (cBEBoltDB_collectChildBolts) and, if no owned item has flag 0x10,
 * owns the group's prize (cBECharProfileDB_setBoltVisible).
 *
 * @return 1, or 0 when a prize could not be owned
 */
int cBECharProfileDB::showDefaultBolts()
{
    int i = 0;
    int ok = 1;
    void* db = cBEBoltInterface_getBoltDB_noarg();
    sEconGroup_1521F0* group = (sEconGroup_1521F0*)cBEBoltDB_getCharGroups(db, mCharacterID);
    int n = cBEBoltDB_getNumCharGroups(db, mCharacterID);
    for (; i < n; i++, group++)
    {
        sEconDef_1521F0* list[528];
        int cnt = cBEBoltDB_collectChildBolts(db, mCharacterID, group->id, (void**)list, -1, 1);
        int j;
        for (j = 0; j < cnt; j++)
        {
            if (!(Find_1521F0((this), list[j]->id)->flags & 0x10))
                continue;
            if (list[j]->flags & 0x10)
                break;
        }
        if (j == cnt)
        {
            if (setBoltVisible(group->prize, 1) == 0)
            {
                ok = 0;
                break;
            }
        }
    }
    return ok;
}
#endif


//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_isBoltVisible);
#ifdef SKIP_ASM
static inline bool testFlag_152340(unsigned int flags, unsigned int mask)
{
    return (flags & mask) != 0;
}

/**
 * Tells whether item @p id is owned (flag 0x10) in a character profile's item list (no check for a missing id).
 *
 * @param id  the item id
 * @return true when owned
 */
bool cBECharProfileDB::isBoltVisible(int id)
{
    int idx = mpBoltsMap[id];
    unsigned int flags = *(unsigned short*)((idx << 2) + (int)this + 0x292);
    return testFlag_152340(flags, 0x10);
}
#endif
//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB_hasBoltFlags);
#ifdef SKIP_ASM
/**
 * Tells whether item @p id has every flag in @p mask set in a character profile's item list (no check for a missing
 * id).
 *
 * @param id  the item id
 * @param mask  the flags to test
 * @return true when all are set
 */
bool cBECharProfileDB::hasBoltFlags(int id, unsigned int mask)
{
    int idx = mpBoltsMap[id];
    unsigned int flags = *(unsigned short*)((idx << 2) + (int)this + 0x292);
    return (~flags & mask) == 0;
}
#endif
