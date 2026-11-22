#include "common.h"
#include "be/beprofilelib.h"
#include "be/bedbprofile.h"
#include "be/beintecon.h"

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
        *(void**)&mem->mVtbl = cBEEconInterface_vtbl;
        cBEEconInterface_instance = (cBEEconInterface*)(mem);
    }
    return cBEEconInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getMoney);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

/**
 * Returns a character's cash (+0xAC4 of its profile record).
 *
 * @param iHuman  the profile index; 2 or more gives 0
 * @param charType  the character id
 * @return the cash, or 0
 */
extern "C" int cBEEconInterface_getMoney(cBEEconInterface* self, int iHuman, int charType)
{
    if (iHuman >= 2) {
        return 0;
    }
    int bOff = charType * 3976;
    int aOff = iHuman * 39760;
    int off = bOff + aOff;
    char* profile = (char*)cBELibrary_profileLib + off;
    return *(int*)(profile + 0xAC4);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getMoney1);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

/**
 * Returns @p rider's money: the word at +0xAC4 of its character profile (cBELibrary_profileLib, 0x9B50 bytes per
 * profile slot, 0xF88 per character).
 *
 * @param rider  the rider (selects profile slot and character)
 * @return the money
 */
extern "C" int cBEEconInterface_getMoney1(cBEEconInterface* self, int rider)
{
    int profileSlot = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    int bOff = characterId * 3976;
    int aOff = profileSlot * 39760;
    int off = bOff + aOff;
    char* charProfile = (char*)cBELibrary_profileLib + off;
    return *(int*)(charProfile + 0xAC4);
}
#endif

extern int cBELibrary_profileLib[];

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getTotalMoneyEarned__FPvii);
#ifdef SKIP_ASM
/**
 * Returns the total money a character has earned (+0xAC8 of its profile record).
 *
 * @param iHuman  the profile index; 2 or more gives 0
 * @param charType  the character id
 * @return the total earned, or 0
 */
int cBEEconInterface_getTotalMoneyEarned(void* self, int iHuman, int charType)
{
    if (iHuman >= 2) {
        return 0;
    }
    int bOff = charType * 3976;
    int aOff = iHuman * 39760;
    int off = bOff + aOff;
    char* profile = (char*)cBELibrary_profileLib + off;
    return *(int*)(profile + 0xAC8);
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
 * @return the total earned (+0xAC8 of the profile record)
 */
extern "C" int cBEEconInterface_getTotalMoneyEarned1(cBEEconInterface* self, int rider)
{
    int profileIndex = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    int bOff = characterId * 3976;
    int aOff = profileIndex * 39760;
    int off = bOff + aOff;
    char* profile = (char*)cBELibrary_profileLib + off;
    return *(int*)(profile + 0xAC8);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_setMoney);
#ifdef SKIP_ASM
/**
 * Sets the money (+0xAC4) of character @p characterId in profile slot @p profileIndex; does nothing for slots >= 2.
 *
 * @param profileIndex  the profile slot (only 0 and 1 are stored)
 * @param characterId  the character
 * @param value  the new amount
 */
extern "C" void cBEEconInterface_setMoney(cBEEconInterface* self, int profileIndex, int characterId, int value)
{
    if (profileIndex < 2) {
        int bOff = characterId * 3976;
        int aOff = profileIndex * 39760;
        int off = bOff + aOff;
        char* profile = (char*)cBELibrary_profileLib + off;
        *(int*)(profile + 0xAC4) = value;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_addMoney);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

/**
 * Adds @p amount to the money (+0xAC4) and the total money earned (+0xAC8) of character @p characterId in profile slot @p humanIndex.
 *
 * @param humanIndex  the profile slot (0 or 1; 2 and up do nothing and return 0)
 * @param characterId  the character
 * @param amount  the money earned
 * @return the new money, or 0
 */
extern "C" int cBEEconInterface_addMoney(cBEEconInterface* self, int humanIndex, int characterId, int amount)
{
    if (humanIndex >= 2) {
        return 0;
    }
    int bOff = characterId * 3976;
    int aOff = humanIndex * 39760;
    int off = bOff + aOff;
    cBECharProfileDB* charProfile = (cBECharProfileDB*)((char*)cBELibrary_profileLib + off);
    charProfile->mMoney += amount;
    *(int*)&charProfile->mTotalMoneyEarned += amount;
    return charProfile->mMoney;
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
extern "C" int cBEEconInterface_addMoney1(cBEEconInterface* self, int rider, int amount)
{
    int profile = cBELibrary_getProfileIndex(rider);
    return cBEEconInterface_addMoney(self, profile, cBELibrary_getCharacterID(rider), amount);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_reduceMoney);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

/**
 * Subtracts @p amount from the money (+0xAC4) of character @p characterId in profile slot @p profileIndex.
 *
 * The total earned (+0xAC8) is left alone; there is no check that the balance covers the amount.
 *
 * @param profileIndex  the profile slot (0 or 1; 2 and up do nothing and return 0)
 * @param characterId  the character
 * @param amount  the money spent
 * @return the money left, or 0
 */
extern "C" int cBEEconInterface_reduceMoney(cBEEconInterface* self, int profileIndex, int characterId, int amount)
{
    if (profileIndex >= 2) {
        return 0;
    }
    int bOff = characterId * 3976;
    int aOff = profileIndex * 39760;
    int off = bOff + aOff;
    char* profile = (char*)cBELibrary_profileLib + off;
    return *(int*)(profile + 0xAC4) -= amount;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getPeakPass);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

// PORT: 64-bit unsigned long flag word (bit 12 = peak 1 locked, bit 13 = peak 2).
struct sPeakBits_0B88 {
    unsigned long low : 12;
    unsigned long peak1 : 1;
    unsigned long peak2 : 1;
    unsigned long rest : 50;
};

/**
 * Unlocks or locks peak @p peak (1 or 2) for character @p characterId of profile slot @p profileIndex: stores
 * !@p unlocked in bit 12 (peak 1) or 13 (peak 2) of the profile's flag word at +0x278.
 *
 * Other peak numbers change nothing.
 *
 * @param profileIndex  the profile slot (0 or 1; 2 and up return 0)
 * @param characterId  the character
 * @param peak  the peak, 1 or 2
 * @param unlocked  nonzero to unlock
 * @return 1, or 0 for profile slots >= 2
 */
// Sets the locked state of a peak for a profile/character: stores the inverse of `unlocked`.
extern "C" int cBEEconInterface_getPeakPass(cBEEconInterface* self, int profileIndex, int characterId, int peak, int unlocked)
{
    if (profileIndex >= 2) {
        return 0;
    }
    unlocked ^= 1;
    if (peak == 1) {
        int bOff = characterId * 3976;
        int aOff = profileIndex * 39760;
        int off = bOff + aOff;
        char* profile = (char*)cBELibrary_profileLib + off;
        ((sPeakBits_0B88*)(profile + 0x278))->peak1 = unlocked;
    } else if (peak == 2) {
        int bOff = characterId * 3976;
        int aOff = profileIndex * 39760;
        int off = bOff + aOff;
        char* profile = (char*)cBELibrary_profileLib + off;
        ((sPeakBits_0B88*)(profile + 0x278))->peak2 = unlocked;
    }
    return 1;
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
 * The seven attribute bytes start at +0xBDF of the character profile. The price is cBEEconInterface_getAttribCost(points / 5 - 1);
 * when the money (+0xAC4) covers it and the attribute is not at 55, the money drops by the price, the attribute rises
 * by 1 and the stat interface's vtable slot 2 runs.
 *
 * @param humanIndex  the profile slot (2 and up return 0)
 * @param characterId  the character
 * @param attribute  the attribute, 0-6 (others read a null pointer)
 * @return 1 if bought, else 0
 */
extern "C" int cBEEconInterface_buyAttribute(cBEEconInterface* self, int humanIndex, int characterId, unsigned int attribute) {
    signed char *attribLevel;
    int cur;
    int cost;

    if (humanIndex >= 2) {
        return 0;
    }
    {
        attribLevel = 0;
        cur = ((cBECharProfileDB*)((humanIndex * 0x9B50 + characterId * 0xF88) + (char*)cBELibrary_profileLib))->mMoney;
        switch (attribute) {
        case 0:
            attribLevel = &(&((cBECharProfileDB(*)[10])cBELibrary_profileLib)[humanIndex][characterId].statDB)->cSpeed;
            break;
        case 1:
            attribLevel = &(&((cBECharProfileDB(*)[10])cBELibrary_profileLib)[humanIndex][characterId].statDB)->cAccel;
            break;
        case 2:
            attribLevel = &(&((cBECharProfileDB(*)[10])cBELibrary_profileLib)[humanIndex][characterId].statDB)->cTrick;
            break;
        case 3:
            attribLevel = &(&((cBECharProfileDB(*)[10])cBELibrary_profileLib)[humanIndex][characterId].statDB)->cEdging;
            break;
        case 4:
            attribLevel = &(&((cBECharProfileDB(*)[10])cBELibrary_profileLib)[humanIndex][characterId].statDB)->cSpin;
            break;
        case 5:
            attribLevel = &(&((cBECharProfileDB(*)[10])cBELibrary_profileLib)[humanIndex][characterId].statDB)->cToughness;
            break;
        case 6:
            attribLevel = &(&((cBECharProfileDB(*)[10])cBELibrary_profileLib)[humanIndex][characterId].statDB)->cStability;
            break;
        }
        cost = cBEEconInterface_getAttribCost(self, *attribLevel / 5 - 1);
        if (cur < cost || *attribLevel == 55) {
            return 0;
        }
        cBECharProfileDB* charProfile = (cBECharProfileDB*)((characterId * 0xF88 + humanIndex * 0x9B50) + (char*)cBELibrary_profileLib);
        charProfile->mMoney = charProfile->mMoney - cost;
        *attribLevel = (unsigned char)*attribLevel + 1;
        cBEStatInterface_getThis()->discard();
        return 1;
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
int cBEEconInterface_getAttribCost(void* self, int levelIndex)
{
    return ((sEconTable*)cBELibrary_attribCost)->entries[levelIndex].value;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_sellAttribute);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sProf0E68 { char s[0xF88]; };
struct sEcon0E68 { int a; int b; };
extern sEcon0E68 cBELibrary_attribCost_0E68[] __asm__("cBELibrary_attribCost");

/**
 * Refunds one point of attribute @p kind for character @p characterId of slot @p profile, unless the attribute is at 5.
 *
 * Adds the price cBELibrary_attribCost[(points - 1) / 5] to the money (+0xAC4), then increments the attribute byte (+0xBDF +
 * @p kind) instead of decrementing it (likely a bug; nothing calls this function).
 *
 * @param profile  the profile slot (2 and up return 0)
 * @param characterId  the character
 * @param kind  the attribute, 0-6
 * @return 1 if refunded, else 0
 */
extern "C" int cBEEconInterface_sellAttribute(cBEEconInterface* self, int profile, int characterId, unsigned int kind)
{
    char* attribLevel;
    if (profile >= 2) return 0;
    attribLevel = 0;
    switch (kind) {
    case 0: attribLevel = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][characterId].s[0xBDF]; break;
    case 1: attribLevel = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][characterId].s[0xBE0]; break;
    case 2: attribLevel = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][characterId].s[0xBE1]; break;
    case 3: attribLevel = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][characterId].s[0xBE2]; break;
    case 4: attribLevel = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][characterId].s[0xBE3]; break;
    case 5: attribLevel = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][characterId].s[0xBE4]; break;
    case 6: attribLevel = &((sProf0E68 (*)[10])cBELibrary_profileLib)[profile][characterId].s[0xBE5]; break;
    }
    if (*attribLevel == 5) return 0;
    {
        int refund = cBELibrary_attribCost_0E68[(*attribLevel - 1) / 5].b;
        int off = characterId * 3976 + profile * 39760;
        char* charProfile = (char*)cBELibrary_profileLib + off;
        *(int*)(charProfile + 0xAC4) += refund;
    }
    (*attribLevel)++;
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBEEconInterface_getCashReward);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_getPeakRewardAmount(void* race, int place, int kind);

struct sEconPrize_151040
{
    int key;
    int prize[4];
};
extern sEconPrize_151040 D_004405A0[];

/**
 * Returns the prize money for medal @p medal.
 *
 * For @p place 6-11: cBENewRaceInterface_getPeakRewardAmount(race, @p place, tier) with tier 2 for medals 0-1, 1 for medal 2 and 0 for medal 3.
 * Otherwise the D_004405A0 row (17 rows) whose key is @p key, column @p medal, times 100. 0 when nothing matches.
 *
 * @param medal  the medal, 0-3
 * @param key  the track key matched against D_004405A0
 * @param place  the event (6-11 are the peak events)
 * @return the money
 */
extern "C" int cBEEconInterface_getCashReward(cBEEconInterface* self, int medal, int key, int place)
{
    void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
    int reward = 0;
    if ((unsigned int)(place - 6) < 6)
    {
        switch (medal)
        {
        case 0:
        case 1:
            reward = cBENewRaceInterface_getPeakRewardAmount(race, place, 2);
            break;
        case 2:
            reward = cBENewRaceInterface_getPeakRewardAmount(race, place, 1);
            break;
        case 3:
            reward = cBENewRaceInterface_getPeakRewardAmount(race, place, 0);
            break;
        }
    }
    else
    {
        for (int i = 0; i < 17; i++)
        {
            if (D_004405A0[i].key != key) continue;
            switch (medal)
            {
            case 0:
                reward = D_004405A0[i].prize[0] * 100;
                break;
            case 1:
                reward = D_004405A0[i].prize[1] * 100;
                break;
            case 2:
                reward = D_004405A0[i].prize[2] * 100;
                break;
            case 3:
                reward = D_004405A0[i].prize[3] * 100;
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
 * @param level  the peak, 0-2
 * @return the cash
 */
extern "C" int cBEEconInterface_getCollectableCash(cBEEconInterface* self, int level)
{
    switch (level)
    {
    case 0:
    default:
        return 500;
    case 1:
        return 1000;
    case 2:
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
extern "C" int cBEEconInterface_getMissionCash(cBEEconInterface* self, int level)
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
struct sEconRec {
    char pad0[0xBDF];
    signed char f[7];
    char pad1[0xF88 - 0xBDF - 7];
};
/**
 * Returns the attribute-cost table price for the next point of attribute @p attribute of character @p characterId in slot @p humanIndex:
 * cBELibrary_attribCost[(points + 1) / 5].
 *
 * Note cBEEconInterface_buyAttribute charges cBELibrary_attribCost[points / 5 - 1] instead.
 *
 * @param humanIndex  the profile slot (2 and up return 0)
 * @param characterId  the character
 * @param attribute  the attribute, 0-6
 * @return the price
 */
extern "C" int cBEEconInterface_getCostForNextIncrement(cBEEconInterface* self, int humanIndex, int characterId, unsigned int attribute) {
    signed char *attribLevel;
    if (humanIndex >= 2) {
        return 0;
    }
    {
        attribLevel = 0;
        switch (attribute) {
        case 0:
            attribLevel = &((sEconRec(*)[10])cBELibrary_profileLib)[humanIndex][characterId].f[0];
            break;
        case 1:
            attribLevel = &((sEconRec(*)[10])cBELibrary_profileLib)[humanIndex][characterId].f[1];
            break;
        case 2:
            attribLevel = &((sEconRec(*)[10])cBELibrary_profileLib)[humanIndex][characterId].f[2];
            break;
        case 3:
            attribLevel = &((sEconRec(*)[10])cBELibrary_profileLib)[humanIndex][characterId].f[3];
            break;
        case 4:
            attribLevel = &((sEconRec(*)[10])cBELibrary_profileLib)[humanIndex][characterId].f[4];
            break;
        case 5:
            attribLevel = &((sEconRec(*)[10])cBELibrary_profileLib)[humanIndex][characterId].f[5];
            break;
        case 6:
            attribLevel = &((sEconRec(*)[10])cBELibrary_profileLib)[humanIndex][characterId].f[6];
            break;
        }
        return ((sEconTable*)cBELibrary_attribCost)->entries[(*attribLevel + 1) / 5].value;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", cBECharProfileDB__cBECharProfileDB);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int*);

/**
 * Destroys a character profile: frees its item lookup table (+0x288, from func_00151A88) and clears the pointer; with
 * @p flags & 1 also frees the object.
 *
 * @param flags  bit 0: operator_delete the object
 */
extern "C" void cBECharProfileDB__cBECharProfileDB(cBECharProfileDB* self, int flags)
{
    void* buffer = self->mpBoltsMap;
    if (buffer != 0)
    {
        cMemMan_free(buffer);
    }
    self->mpBoltsMap = 0;
    if (flags & 1)
    {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001513B8);
#ifdef SKIP_ASM
int func_0014DC40(void* self, int kind);
extern "C" void* func_0014DC50(void* self, int kind);
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(void* self, int i);
extern "C" int func_00151C90(cBECharProfileDB* self, int id, int on);
extern char D_004A6750[];



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
 * For the character kind (+0xBC0) from the item database D_004A6750: owns each group's prize item (func_00151C90);
 * owned items (flag 0x10) get flags 0x26; items priced 0 get flag 2, and items priced -1 also become owned. If any
 * did, owned items get flags 6 and the others lose flag 4. cBECharProfileDB_init and func_00151A88 call it.
 *
 * @param vself  the character profile
 */
extern "C" void func_001513B8(cBECharProfileDB* vself)
{
    cBECharProfileDB* self = vself;
    void* db = D_004A6750;
    int n = func_0014DC40(db, self->unk_BC0);
    sEconGroup_1513B8* prizeGroup = (sEconGroup_1513B8*)func_0014DC50(db, self->unk_BC0);
    for (int i = 0; i < n; i++, prizeGroup++)
    {
        func_00151C90((cBECharProfileDB*)self, prizeGroup->prize, 1);
    }
    int j;
    for (j = 0; j < self->mNumBolts; j++)
    {
        tBEProfileBolt* it = &((&self->mBolts[0]))[j];
        if ((*(short*)&it->flags) & 0x10)
            *(short*)&it->flags |= 0x26;
    }
    int numDefs = func_0014D988(db, self->unk_BC0);
    sEconDef_1513B8* itemDef = (sEconDef_1513B8*)func_0014D998(db, self->unk_BC0);
    int changed = 0;
    for (int i = 0; i < numDefs; i++, itemDef++)
    {
        if (Price_1513B8(itemDef) == 0)
        {
            (*(short*)&Find_1513B8(self, itemDef->id)->flags) |= 2;
        }
        else if (Price_1513B8(itemDef) == -1)
        {
            (*(short*)&Find_1513B8(self, itemDef->id)->flags) |= 2;
            func_00151C90((cBECharProfileDB*)self, itemDef->id, 1);
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
INCLUDE_ASM("be/beintecon", func_001515B8);
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
extern "C" void func_001515B8(cBECharProfileDB* self)
{
    int n = self->mNumBolts;
    tBEProfileBolt* item = ((&self->mBolts[0]));
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
extern "C" void func_00151A88(cBECharProfileDB* self, int k);
extern "C" void func_001519E0(sEcon_001519E0* self, int k);
extern "C" void func_00150558(int kind, void* dst);
extern sEconDef_1600 cBELibrary_missionInfo_v1600[] __asm__("cBELibrary_missionInfo");
extern sEconColRec_1600 D_005308D9[];

/**
 * Resets one character's career profile to its starting state.
 *
 * With @p reload, func_00151A88 first reloads the item data for character @p k. Then: the unlock bits at +0x278 get
 * their defaults, money (+0xAC4, +0xAC8) and the visited-locations mask (+0xACC) are zeroed, the 26 slots are blanked,
 * the 23 entries cleared, the 0x58 mission flags (+0x118) take their initial lock state from cBELibrary_missionInfo, the outfit
 * colours come from D_005308D9[@p k], func_001519E0 and func_00150558 set the character kind's defaults, and the
 * remaining records are cleared; +0x27C ends as 0x11. Unlike On Tour's init(void) the retail version takes the
 * character and a reload flag.
 *
 * @param k  the character the profile belongs to
 * @param reload  nonzero to reload the character's item data first (func_00151A88)
 */
extern "C" void cBECharProfileDB_init(sEcon_1600* self, int k, int reload)
{
    if (reload) {
        func_00151A88((cBECharProfileDB*)((sEconItems_151A88*)self), k);
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
    func_001519E0((sEcon_001519E0*)self, self->kind);
    func_00150558(self->kind, self->fBE6);
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
INCLUDE_ASM("be/beintecon", func_001519E0);
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
extern "C" void func_001519E0(sEcon_001519E0* self, int k)
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
INCLUDE_ASM("be/beintecon", func_00151A88);
#ifdef SKIP_ASM
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(void* self, int i);
extern "C" void func_001513B8(cBECharProfileDB* self);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern int kBolt_Num;
extern const char str_Profile_Bolts_Map[];
extern char D_004A6750[];

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
 * Sets the kind (+0xBC0) and count (+0x28C) from D_004A6750; allocates the id-to-slot lookup (+0x288, kBolt_Num
 * shorts, "Profile_Bolts_Map") on first use and fills it with -1; gives each item a slot with flags 0 and clears the
 * rest of the 525 slots to id -1. Ends with func_001513B8.
 *
 * @param k  the character
 */
extern "C" void func_00151A88(cBECharProfileDB* self, int k)
{
    void* db = D_004A6750;
    sEconDef_151A88* itemDef = (sEconDef_151A88*)func_0014D998(db, k);
    int n = func_0014D988(db, k);
    self->unk_BC0 = k;
    self->mNumBolts = n;
    if (self->mpBoltsMap == 0)
        self->mpBoltsMap = (short*)operator_new_tag(kBolt_Num * 2, str_Profile_Bolts_Map, 0, 0);
    int i;
    for (i = 0; i < kBolt_Num; i++)
        self->mpBoltsMap[i] = -1;
    for (i = 0; i < n; i++, itemDef++)
    {
        tBEProfileBolt* itemSlot = &((&self->mBolts[0]))[i];
        itemSlot->flags = 0;
        int id = itemDef->id;
        itemSlot->id = id;
        self->mpBoltsMap[itemDef->id] = i;
    }
    for (i = n; i < 525; i++)
    {
        tBEProfileBolt* emptySlot = &((&self->mBolts[0]))[i];
        emptySlot->id = -1;
        emptySlot->flags = 0;
    }
    func_001513B8(self);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151BD0);
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
extern "C" void func_00151BD0(cBECharProfileDB* self)
{
    for (int i = 0; i < self->mNumBolts; i++) {
        tBEProfileBolt* item = &((&self->mBolts[0]))[i];
        unsigned int oldFlags = item->flags;
        item->flags = (oldFlags & 0x20) ? (oldFlags | 0x16) : clearBits_151BD0(clearBits_151BD0(oldFlags, 0x10), 4);
    }
}
#endif
//100%
INCLUDE_ASM("be/beintecon", func_00151C20);
#ifdef SKIP_ASM
/**
 * Returns the item id in slot @p i of a profile's item list (+0x290), or -1 when @p i is past the count (+0x28C).
 *
 * @param i  the slot
 * @return the item id, or -1
 */
extern "C" short func_00151C20(cBECharProfileDB* self, int i)
{
    if (i >= self->mNumBolts)
        return -1;
    return *(short*)((i << 2) + (int)self + 0x290);
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151C48);
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
extern "C" void func_00151C48(cBECharProfileDB* self, int id, int on)
{
    int idx = self->mpBoltsMap[id];
    tBEProfileBolt* item;
    if (idx >= 0)
        item = &((&self->mBolts[0]))[idx];
    else
        item = 0;
    if (on)
        *(short*)&item->flags |= 2;
    else
        *(short*)&item->flags &= ~2;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151C90);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_1C90() __asm__("func_0014BDB8__FPv");
extern "C" int func_0014D448(void* db, int kind, int id);
extern "C" void* func_0014D558(void* db, int kind, int id);
extern "C" int func_0014D4C8(void* db, int kind, int id);
extern "C" int func_00151EF0(cBECharProfileDB* self, int id, int on);



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
 * (func_00151EF0). Returns 0 as soon as a nested change fails.
 *
 * @param selfp  the character profile
 * @param id  the item id
 * @param on  nonzero to own
 * @return 1 on success, 0 on failure
 */
// Sets item id owned/unowned; turning on cascades turn-offs to conflicting items (def flag 1), turning off
// cascades to dependent items (owned before or def flag 8). Then applies the item's links; 0 on failure.
extern "C" int func_00151C90(cBECharProfileDB* selfp, int id, int on)
{
    cBECharProfileDB* self = selfp;
    sEconDefs_151C90* db = (sEconDefs_151C90*)func_0014BDB8_1C90();
    sEconDef_151C90* def = getDef_151C90(db, func_0014D448(db, self->unk_BC0, id));
    tBEProfileBolt* item = getItem_151C90(self, id);
    int bit1 = hasFlag_151C90(def, 1) != 0;
    int bit8 = hasFlag_151C90(def, 8) != 0;
    int wasOn = isOwned_151C90(item) != 0;
    if (on)
        item->flags |= 0x10;
    else
        item->flags &= ~0x10;
    if (on && wasOn)
        return 1;
    if (on && bit1)
    {
        int rid = def->rel;
        sEconRel_151C90* rel = (sEconRel_151C90*)func_0014D558(db, self->unk_BC0, rid);
        int n = func_0014D4C8(db, self->unk_BC0, rid);
        for (int i = 0; i < n; i++, rel++)
        {
            int sid = rel->other;
            if (sid != id)
            {
                int j = self->mpBoltsMap[sid];
                if (j >= 0 && (isOwned_151C90(&((&self->mBolts[0]))[j]) || (rel->flags & 8)))
                {
                    if (func_00151C90((cBECharProfileDB*)self, sid, 0) == 0)
                        return 0;
                }
            }
        }
    }
    else if (!on && (wasOn || bit8))
    {
        sEconRel_151C90* rel = (sEconRel_151C90*)func_0014D558(db, self->unk_BC0, id);
        int n = func_0014D4C8(db, self->unk_BC0, id);
        for (int i = 0; i < n; i++, rel++)
        {
            if (func_00151C90((cBECharProfileDB*)self, rel->other, 0) == 0)
                return 0;
        }
    }
    if ((on && !wasOn) || (!on && wasOn))
    {
        if (func_00151EF0((cBECharProfileDB*)self, id, on) == 0)
            return 0;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_00151EF0);
#ifdef SKIP_ASM
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_1EF0() __asm__("func_0014BDB8__FPv");
struct sEconLink_151EF0;
extern "C" sEconLink_151EF0* func_0014DB40(void* db, int kind, int id, int on, int* n);
extern "C" int func_0014D448(void* db, int kind, int id);
extern "C" int func_00151C90(cBECharProfileDB* self, int id, int on);



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
 * Links come from func_0014DB40. A link is skipped when its target is -1, when its condition item's owned state
 * differs from the link's, or when the target is already in the wanted state (unless it is owned and its definition
 * has flag 8). A link to @p id itself only sets its flag; others go through func_00151C90.
 *
 * @param selfp  the character profile
 * @param id  the item id
 * @param on  the item's new owned state
 * @return 1 on success, 0 if a nested change failed
 */
// Sets item id owned/unowned and applies its dependency links (cascading through func_00151C90); 0 on failure.
extern "C" int func_00151EF0(cBECharProfileDB* selfp, int id, int on)
{
    cBECharProfileDB* self = selfp;
    sEconDefs_151EF0* db = (sEconDefs_151EF0*)func_0014BDB8_1EF0();
    int n = 0;
    sEconLink_151EF0* link = func_0014DB40(db, self->unk_BC0, id, on, &n);
    tBEProfileBolt* item = getItem_151EF0(self, id);
    for (int i = 0; i < n; i++, link++)
    {
        if (link->target == -1)
            continue;
        if (link->condId != -1)
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
            if (!(getDef_151EF0(db, func_0014D448(db, self->unk_BC0, link->target))->flags & 8))
                continue;
        }
        if (targetItem->id == id)
        {
            if (link->on)
                item->flags |= 0x10;
            else
                item->flags &= ~0x10;
        }
        else if (func_00151C90((cBECharProfileDB*)self, targetItem->id, link->on != 0) == 0)
        {
            return 0;
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001520E8);
#ifdef SKIP_ASM
void* func_0014BDB8(void* self);
int func_0014D988(void* self, int i);
extern "C" void* func_0014D998(void* self, int i);
extern "C" int func_00151EF0(cBECharProfileDB* self, int id, int on);



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
 * @return 1 on success, 0 if applying links failed
 */
extern "C" int func_001520E8(cBECharProfileDB* self, int* changed)
{
    void* db = func_0014BDB8((self));
    int n = func_0014D988(db, *((char*)&self->unk_BC0));
    sEconDef_001520E8* def = (sEconDef_001520E8*)func_0014D998(db, *((char*)&self->unk_BC0));
    *changed = 0;
    for (int i = 0; i < n; i++, def++)
    {
        if (def->flags & 0x1000)
        {
            if (getItem_001520E8((self), def->id)->flags & 0x10)
            {
                *changed = 1;
                if (func_00151EF0(self, def->id, 1) == 0)
                    return 0;
            }
        }
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintecon", func_001521F0);
#ifdef SKIP_ASM
// PORT: func_0014BDB8 is declared (void*) but never reads its argument; called here with none
void* func_0014BDB8_noarg() __asm__("func_0014BDB8__FPv");
int func_0014DC40(void* self, int kind);
extern "C" void* func_0014DC50(void* self, int kind);
extern "C" int func_0014D608(void* self, int kind, int id, void** out, int a, int b);
extern "C" int func_00151C90(cBECharProfileDB* self, int id, int on);



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
 * For each group of the character kind: lists the group's items (func_0014D608) and, if no owned item has flag 0x10,
 * owns the group's prize (func_00151C90).
 *
 * @return 1, or 0 when a prize could not be owned
 */
extern "C" int func_001521F0(cBECharProfileDB* self)
{
    int i = 0;
    int ok = 1;
    void* db = func_0014BDB8_noarg();
    sEconGroup_1521F0* group = (sEconGroup_1521F0*)func_0014DC50(db, self->unk_BC0);
    int n = func_0014DC40(db, self->unk_BC0);
    for (; i < n; i++, group++)
    {
        sEconDef_1521F0* list[528];
        int cnt = func_0014D608(db, self->unk_BC0, group->id, (void**)list, -1, 1);
        int j;
        for (j = 0; j < cnt; j++)
        {
            if (!(Find_1521F0((self), list[j]->id)->flags & 0x10))
                continue;
            if (list[j]->flags & 0x10)
                break;
        }
        if (j == cnt)
        {
            if (func_00151C90(self, group->prize, 1) == 0)
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
INCLUDE_ASM("be/beintecon", func_00152340);
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
extern "C" bool func_00152340(cBECharProfileDB* self, int id)
{
    int idx = (self->mpBoltsMap)[id];
    unsigned int flags = *(unsigned short*)((idx << 2) + (int)self + 0x292);
    return testFlag_152340(flags, 0x10);
}
#endif
//100%
INCLUDE_ASM("be/beintecon", func_00152368);
#ifdef SKIP_ASM
/**
 * Tells whether item @p id has every flag in @p mask set in a character profile's item list (no check for a missing
 * id).
 *
 * @param id  the item id
 * @param mask  the flags to test
 * @return true when all are set
 */
extern "C" bool func_00152368(cBECharProfileDB* self, int id, unsigned int mask)
{
    int idx = (self->mpBoltsMap)[id];
    unsigned int flags = *(unsigned short*)((idx << 2) + (int)self + 0x292);
    return (~flags & mask) == 0;
}
#endif
