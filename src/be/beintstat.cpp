#include "common.h"
#include "vtable.h"
#include "be/beintaggression.h"
#include "be/beintscore.h"
#include "be/beintnewrace.h"
#include "be/beintmission.h"
#include "be/beintcharacter.h"
#include "be/beintbagt.h"
#include "be/beintoption.h"
#include "be/beintecon.h"
#include "be/beintstat.h"
#include "be/beintplayer.h"
#include "be/beintnetwork.h"
#include "be/beintreward.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_getCollisionAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_getCollisionAttrib
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_getCollisionAttrib
{
    signed char v[15];
};
extern sPlayerStatRow_getCollisionAttrib cBEStatInterface_currentStat_getCollisionAttrib[] __asm__("cBEStatInterface_currentStat");
extern sCharStat_getCollisionAttrib cBELibrary_statDefault_getCollisionAttrib[] __asm__("cBELibrary_statDefault");

/**
 * Returns a rider's collision attribute as a fraction of the character's maximum.
 *
 * Mission riders get 0.5. The level is @p value when positive, else the profile's stat level for the
 * character
 * divided by 5.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  attribute level to use instead of the stored one, when positive (On Tour overrideAttribLevel)
 * @return the attribute level divided by the character's collision maximum
 */
extern "C" float cBEStatInterface_getCollisionAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int attribLevel;
    if (value > 0)
        attribLevel = value;
    else
        attribLevel = cBEStatInterface_currentStat_getCollisionAttrib[profile].e[charID][5] / 5;
    return (float)attribLevel / (float)cBELibrary_statDefault_getCollisionAttrib[charID].v[0xD];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_getStrikingAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_cBEStatInterface_getStrikingAttrib
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_cBEStatInterface_getStrikingAttrib
{
    signed char v[15];
};
extern sPlayerStatRow_cBEStatInterface_getStrikingAttrib cBEStatInterface_currentStat_cBEStatInterface_getStrikingAttrib[] __asm__("cBEStatInterface_currentStat");
extern sCharStat_cBEStatInterface_getStrikingAttrib cBELibrary_statDefault_cBEStatInterface_getStrikingAttrib[] __asm__("cBELibrary_statDefault");

/**
 * Returns how hard a rider hits, as a fraction of the character's best.
 *
 * A mission man (cBENewPlayerInterface_isMissionMan) gets 0.5. Otherwise the level is @p value when positive, else
 * attribute 5 of the rider's profile/character stats (cBEStatInterface_currentStat) / 5, divided by the character's entry 0xD in
 * cBELibrary_statDefault.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  a level that overrides the rider's own when positive (On Tour overrideAttribLevel)
 * @return the striking attribute, usually 0 to 1
 */
extern "C" float cBEStatInterface_getStrikingAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int character = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int level;
    if (value > 0)
        level = value;
    else
        level = cBEStatInterface_currentStat_cBEStatInterface_getStrikingAttrib[profile].e[character][5] / 5;
    return (float)level / (float)cBELibrary_statDefault_cBEStatInterface_getStrikingAttrib[character].v[0xD];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_getLandingAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_cBEStatInterface_getLandingAttrib
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_cBEStatInterface_getLandingAttrib
{
    signed char v[15];
};
extern sPlayerStatRow_cBEStatInterface_getLandingAttrib cBEStatInterface_currentStat_cBEStatInterface_getLandingAttrib[] __asm__("cBEStatInterface_currentStat");
extern sCharStat_cBEStatInterface_getLandingAttrib cBELibrary_statDefault_cBEStatInterface_getLandingAttrib[] __asm__("cBELibrary_statDefault");

/**
 * Returns how well a rider lands, as a fraction of the character's best.
 *
 * A mission man gets 0.5. Otherwise the level is @p value when positive, else attribute 6 of the rider's
 * profile/character stats (cBEStatInterface_currentStat) / 5, divided by the character's entry 0xE in cBELibrary_statDefault.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  a level that overrides the rider's own when positive (On Tour overrideAttribLevel)
 * @return the landing attribute, usually 0 to 1
 */
extern "C" float cBEStatInterface_getLandingAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int character = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int level;
    if (value > 0)
        level = value;
    else
        level = cBEStatInterface_currentStat_cBEStatInterface_getLandingAttrib[profile].e[character][6] / 5;
    return (float)level / (float)cBELibrary_statDefault_cBEStatInterface_getLandingAttrib[character].v[0xE];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_getBalanceAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_cBEStatInterface_getBalanceAttrib
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_cBEStatInterface_getBalanceAttrib
{
    signed char v[15];
};
extern sPlayerStatRow_cBEStatInterface_getBalanceAttrib cBEStatInterface_currentStat_cBEStatInterface_getBalanceAttrib[] __asm__("cBEStatInterface_currentStat");
extern sCharStat_cBEStatInterface_getBalanceAttrib cBELibrary_statDefault_cBEStatInterface_getBalanceAttrib[] __asm__("cBELibrary_statDefault");

/**
 * Returns a rider's balance attribute as a fraction of the character's maximum stability level.
 *
 * Mission riders (cBENewPlayerInterface_isMissionMan()) get 0.5. The level is @p value when positive, else the
 * profile's stability stat for the character (cBEStatInterface_currentStat[profile][character][6]) divided by 5; the result is that level
 * over the character's cMaxStability (byte 0xE of its cBELibrary_statDefault default-stat record). cBEStatInterface_getBalanceAttrib and cBEStatInterface_getRecoveryAttrib
 * have identical bodies.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  attribute level to use instead of the stored one, when positive (On Tour overrideAttribLevel)
 * @return the attribute level divided by the character's stability maximum
 */
extern "C" float cBEStatInterface_getBalanceAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int attribLevel;
    if (value > 0)
        attribLevel = value;
    else
        attribLevel = cBEStatInterface_currentStat_cBEStatInterface_getBalanceAttrib[profile].e[charID][6] / 5;
    return (float)attribLevel / (float)cBELibrary_statDefault_cBEStatInterface_getBalanceAttrib[charID].v[0xE];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_getRecoveryAttrib);
#ifdef SKIP_ASM
/**
 * Returns a rider's recovery attribute as a fraction of the character's maximum stability level.
 *
 * Mission riders (cBENewPlayerInterface_isMissionMan()) get 0.5. The level is @p value when positive, else the
 * profile's stability stat for the character (cBEStatInterface_currentStat[profile][character][6]) divided by 5; the result is that level
 * over the character's cMaxStability (byte 0xE of its cBELibrary_statDefault default-stat record). cBEStatInterface_getBalanceAttrib and cBEStatInterface_getRecoveryAttrib
 * have identical bodies.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  attribute level to use instead of the stored one, when positive (On Tour overrideAttribLevel)
 * @return the attribute level divided by the character's stability maximum
 */
extern "C" float cBEStatInterface_getRecoveryAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int attribLevel;
    if (value > 0)
        attribLevel = value;
    else
        attribLevel = cBEStatInterface_currentStat_cBEStatInterface_getStrikingAttrib[profile].e[charID][6] / 5;
    return (float)attribLevel / (float)cBELibrary_statDefault_cBEStatInterface_getStrikingAttrib[charID].v[0xE];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_getAccelerationAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_cBEStatInterface_getAccelerationAttrib
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_cBEStatInterface_getAccelerationAttrib
{
    signed char v[15];
};
extern sPlayerStatRow_cBEStatInterface_getAccelerationAttrib cBEStatInterface_currentStat_cBEStatInterface_getAccelerationAttrib[] __asm__("cBEStatInterface_currentStat");
extern sCharStat_cBEStatInterface_getAccelerationAttrib cBELibrary_statDefault_cBEStatInterface_getAccelerationAttrib[] __asm__("cBELibrary_statDefault");

/**
 * Returns a rider's acceleration attribute as a fraction of the character's maximum acceleration level.
 *
 * Mission riders (cBENewPlayerInterface_isMissionMan()) get 0.5. The level is @p value when positive, else the
 * profile's acceleration stat for the character (cBEStatInterface_currentStat[profile][character][1]) divided by 5; the result is that
 * level over the character's cMaxAccel (byte 0x9 of its cBELibrary_statDefault default-stat record).
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  attribute level to use instead of the stored one, when positive (On Tour overrideAttribLevel)
 * @return the attribute level divided by the character's acceleration maximum
 */
extern "C" float cBEStatInterface_getAccelerationAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int attribLevel;
    if (value > 0)
        attribLevel = value;
    else
        attribLevel = cBEStatInterface_currentStat_cBEStatInterface_getAccelerationAttrib[profile].e[charID][1] / 5;
    return (float)attribLevel / (float)cBELibrary_statDefault_cBEStatInterface_getAccelerationAttrib[charID].v[0x9];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_getSpeedAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_cBEStatInterface_getSpeedAttrib
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_cBEStatInterface_getSpeedAttrib
{
    signed char v[15];
};
extern sPlayerStatRow_cBEStatInterface_getSpeedAttrib cBEStatInterface_currentStat_cBEStatInterface_getSpeedAttrib[] __asm__("cBEStatInterface_currentStat");
extern sCharStat_cBEStatInterface_getSpeedAttrib cBELibrary_statDefault_cBEStatInterface_getSpeedAttrib[] __asm__("cBELibrary_statDefault");

/**
 * Returns a rider's speed attribute as a fraction of the character's maximum speed level.
 *
 * Mission riders (cBENewPlayerInterface_isMissionMan()) get 0.5. The level is @p value when positive, else the
 * profile's speed stat for the character (cBEStatInterface_currentStat[profile][character][0]) divided by 5; the result is that level
 * over the character's cMaxSpeed (byte 0x8 of its cBELibrary_statDefault default-stat record).
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  attribute level to use instead of the stored one, when positive (On Tour overrideAttribLevel)
 * @return the attribute level divided by the character's speed maximum
 */
extern "C" float cBEStatInterface_getSpeedAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int attribLevel;
    if (value > 0)
        attribLevel = value;
    else
        attribLevel = cBEStatInterface_currentStat_cBEStatInterface_getSpeedAttrib[profile].e[charID][0] / 5;
    return (float)attribLevel / (float)cBELibrary_statDefault_cBEStatInterface_getSpeedAttrib[charID].v[0x8];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_getSpinAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_cBEStatInterface_getSpinAttrib
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_cBEStatInterface_getSpinAttrib
{
    signed char v[15];
};
extern sPlayerStatRow_cBEStatInterface_getSpinAttrib cBEStatInterface_currentStat_cBEStatInterface_getSpinAttrib[] __asm__("cBEStatInterface_currentStat");
extern sCharStat_cBEStatInterface_getSpinAttrib cBELibrary_statDefault_cBEStatInterface_getSpinAttrib[] __asm__("cBELibrary_statDefault");

/**
 * Returns how fast a rider spins, as a fraction of the character's best.
 *
 * A mission man gets 0.5. Otherwise the level is @p value when positive, else attribute 4 of the rider's
 * profile/character stats (cBEStatInterface_currentStat) / 5, divided by the character's entry 0xC in cBELibrary_statDefault.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  a level that overrides the rider's own when positive (On Tour overrideAttribLevel)
 * @return the spin attribute, usually 0 to 1
 */
extern "C" float cBEStatInterface_getSpinAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int character = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int level;
    if (value > 0)
        level = value;
    else
        level = cBEStatInterface_currentStat_cBEStatInterface_getSpinAttrib[profile].e[character][4] / 5;
    return (float)level / (float)cBELibrary_statDefault_cBEStatInterface_getSpinAttrib[character].v[0xC];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_getTrickAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_cBEStatInterface_getTrickAttrib
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_cBEStatInterface_getTrickAttrib
{
    signed char v[15];
};
extern sPlayerStatRow_cBEStatInterface_getTrickAttrib cBEStatInterface_currentStat_cBEStatInterface_getTrickAttrib[] __asm__("cBEStatInterface_currentStat");
extern sCharStat_cBEStatInterface_getTrickAttrib cBELibrary_statDefault_cBEStatInterface_getTrickAttrib[] __asm__("cBELibrary_statDefault");

/**
 * Returns a rider's trick skill, as a fraction of the character's best.
 *
 * A mission man gets 0.5. Otherwise the level is @p value when positive, else attribute 2 of the rider's
 * profile/character stats (cBEStatInterface_currentStat) / 5, divided by the character's entry 0xA in cBELibrary_statDefault.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  a level that overrides the rider's own when positive (On Tour overrideAttribLevel)
 * @return the trick attribute, usually 0 to 1
 */
extern "C" float cBEStatInterface_getTrickAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int character = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int level;
    if (value > 0)
        level = value;
    else
        level = cBEStatInterface_currentStat_cBEStatInterface_getTrickAttrib[profile].e[character][2] / 5;
    return (float)level / (float)cBELibrary_statDefault_cBEStatInterface_getTrickAttrib[character].v[0xA];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_shouldCelebrateTrick);
#ifdef SKIP_ASM
int cBENewPlayerInterface_isMissionMan(int index);
extern "C" int cBEStatInterface_getStatLevel(void* iface, int id);

// Local threshold table; the original was likely an `int t[11] = {...};` whose
// compiler-generated initializer splat names D_0045A2B0.
struct sLevelTable_00149778
{
    int v[11];
};
extern const sLevelTable_00149778 D_0045A2B0;

/**
 * Decides whether a trick scored enough, for this rider's level, to deserve a celebration.
 *
 * Mission men never celebrate. The threshold is D_0045A2B0[clamp(cBEStatInterface_getStatLevel level - 1, 0, 10)] (On Tour getStatLevel
 * and bracket).
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the trick's points (On Tour iPoints)
 * @return 1 when the points reach the threshold
 */
extern "C" int cBEStatInterface_shouldCelebrateTrick(cBEStatInterface* self, int rider, int value)
{
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0;
    sLevelTable_00149778 pointThresholds = D_0045A2B0;
    int n = cBEStatInterface_getStatLevel(self, rider) - 1;
    int lvl;
    int hi = 10;
    if (n >= 0)
    {
        lvl = n;
        if (lvl > hi)
            lvl = hi;
    }
    else
        lvl = 0;
    return value >= pointThresholds.v[lvl];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBELibrary_cBELibrary);
#ifdef SKIP_ASM
extern "C" void cBELibrary_loadData();
extern "C" void* cBECharacterInterface_getThis();
extern "C" void* cBEStatInterface_getThis(void);
extern "C" void* cBEBoltInterface_getThis();
void* cBENewRaceInterface_getThis();
void* cBENewPlayerInterface_getThis();
void* cBEOptionInterface_getThis();
void* cBESaveInterface_getThis();
void* cBEBAGTInterface_getThis();
void* cBENetworkInterface_getThis();
void* cBEScoreInterface_getThis();
void* cBEMissionInterface_getThis();
void* cBEEconInterface_getThis();
void* cBEAggressionInterface_getThis();
void* cBERewardInterface_getThis();

struct sStatDefaults_149860
{
    int v[0xA28 / 4];
};
extern sStatDefaults_149860 cBELibrary_highScores;
extern sStatDefaults_149860 kHighScoresDefault;

/**
 * Constructs the BE library: resets its data tables and creates every BE interface singleton.
 *
 * Calls cBELibrary_loadData() (resets the character, player and profile tables), copies the default score tables (26
 * entries of 0x64 bytes, kHighScoresDefault) into cBELibrary_highScores, then calls each interface's getThis(): new race, new
 * player, character, stat, option, save, BAGT, network, score, bolt (cBEBoltInterface_getThis), mission, econ, aggression
 * and reward.
 *
 * @return @p self
 */
extern "C" void* cBELibrary_cBELibrary(void* self)
{
    cBELibrary_loadData();
    cBELibrary_highScores = kHighScoresDefault;
    cBENewRaceInterface_getThis();
    cBENewPlayerInterface_getThis();
    cBECharacterInterface_getThis();
    cBEStatInterface_getThis();
    cBEOptionInterface_getThis();
    cBESaveInterface_getThis();
    cBEBAGTInterface_getThis();
    cBENetworkInterface_getThis();
    cBEScoreInterface_getThis();
    cBEBoltInterface_getThis();
    cBEMissionInterface_getThis();
    cBEEconInterface_getThis();
    cBEAggressionInterface_getThis();
    cBERewardInterface_getThis();
    return self;
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBELibrary__cBELibrary);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void cBESaveInterface_purgeInterface(void);
extern int* cBEBoltInterface_instance;

/**
 * Destroys the BE library: frees every BE interface singleton, then the object itself when asked to.
 *
 * Each interface goes to operator delete (no destructor runs) and its pointer is cleared; cBESaveInterface_purgeInterface() does
 * the save interface and frees its save buffer.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cBELibrary__cBELibrary(void* self, int flags)
{
    operator_delete((int*)cBENewRaceInterface_instance);
    cBENewRaceInterface_instance = 0;
    operator_delete((int*)cBENewPlayerInterface_instance);
    cBENewPlayerInterface_instance = 0;
    operator_delete((int*)cBECharacterInterface_instance);
    cBECharacterInterface_instance = 0;
    operator_delete((int*)cBEStatInterface_instance);
    cBEStatInterface_instance = 0;
    operator_delete((int*)cBEOptionInterface_instance);
    cBEOptionInterface_instance = 0;
    cBESaveInterface_purgeInterface();
    operator_delete((int*)cBEBAGTInterface_instance);
    cBEBAGTInterface_instance = 0;
    operator_delete((int*)cBENetworkInterface_instance);
    cBENetworkInterface_instance = 0;
    operator_delete((int*)cBEScoreInterface_instance);
    cBEScoreInterface_instance = 0;
    operator_delete(cBEBoltInterface_instance);
    cBEBoltInterface_instance = 0;
    operator_delete((int*)cBEMissionInterface_instance);
    cBEMissionInterface_instance = 0;
    operator_delete((int*)cBEEconInterface_instance);
    cBEEconInterface_instance = 0;
    operator_delete((int*)cBEAggressionInterface_instance);
    cBEAggressionInterface_instance = 0;
    operator_delete((int*)cBERewardInterface_instance);
    cBERewardInterface_instance = 0;
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBELibrary_newProfile);
#ifdef SKIP_ASM
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* strcpy2(char* buf, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cBEProfileLib_init(void* p, int v);
extern "C" void* cBEStatInterface_getThis(void);
int GetHashValue32(char*);
void* cBENewPlayerInterface_getThis();
void* cBEBAGTInterface_getThis();
extern char str_kT_MEMPlayerName[];
extern char cBELibrary_profileLib[];

struct sVEntry_149A88
{
    short delta;
    short index;
    int (*fn)(void*, ...);
};


static inline void vreset_149A88(char* o)
{
    sVEntry_149A88* vt = *(sVEntry_149A88**)(o + 0xC);
    vt[2].fn(vthis(o, vt[2]));
}

/**
 * Resets a player profile to a new one: clears its player entry, names it "Player n", re-initialises its character
 * records and resets the player, stat and BAGT interfaces.
 *
 * Zeroes the 0x1C-byte entry @p idx of cBELibrary_playerDB, sets iUseDualShock and cCharacterID 4,
 * and formats the localised kT_MEMPlayerName string (looked up through the FE locale's vtable) with @p idx + 1 as its
 * name. Then calls cBEProfileLib_init on profile @p idx of cBELibrary_profileLib and the discard virtual (vtable slot
 * 2) of the player (cBENewPlayerInterface_getThis), stat and BAGT interfaces. cBE's wrapper cBE_newProfile calls it on
 * cBE_library.
 *
 * @param idx  the profile slot (On Tour index)
 */
extern "C" void cBELibrary_newProfile(void* self, signed char idx)
{
    char buf[64];
    cBEPlayerDB* playerEntry = &cBELibrary_playerDB[idx];
    memset(playerEntry, 0, sizeof(cBEPlayerDB));
    playerEntry->iUseDualShock = 1;
    playerEntry->cCharacterID = 4;
    char* locale = (char*)SSXApp->FELocale;
    sVEntry_149A88* vt = *(sVEntry_149A88**)(locale + 4);
    strcpy2(buf, vt[4].fn(vthis(locale, vt[4]), GetHashValue32(str_kT_MEMPlayerName)));
    sprintf(playerEntry->strUserName, buf, idx + 1);
    cBEProfileLib_init(cBELibrary_profileLib + idx * 39760, 1);
    vreset_149A88((char*)cBENewPlayerInterface_getThis());
    vreset_149A88((char*)cBEStatInterface_getThis());
    vreset_149A88((char*)cBEBAGTInterface_getThis());
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBELibrary_loadData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0014C6A0(void*);
extern "C" void cBERewardDB_init(void*, int);
extern "C" void* memset(void* dst, int c, int n);
extern "C" void* FILE_load(const char* name, int flags);
extern "C" void func_0041605C(void* dst, void* src, int n);
void cMemMan_free(void*);
extern "C" void cBEProfileLib_init(void* p, int v);

struct sCharSlot_149BB8
{
    int a[6];
    int b[6];
    char pad[0x4C - 0x30];
};


struct sCharStat_149BB8
{
    unsigned char v[15];
};

struct sCharSrc_149BB8
{
    unsigned char v[7];
};

extern char D_004A6750[];
extern char cBELibrary_rewardDB[];
extern sCharSlot_149BB8 cBELibrary_raceDB;
extern const char str_data_be_charDB_dbl[];
extern char cBELibrary_characterDB[];
extern sCharStat_149BB8 cBELibrary_statDefault[10];
extern sCharSrc_149BB8 D_00440F00[10];
extern char cBELibrary_optionDB[];

extern int cBELibrary_grabPointsDB[0x2A8 / 4];

/**
 * Sets up the career databases at boot: rewards, race and player setup, character data, profiles and grab points.
 *
 * func_0014C6A0 and cBERewardDB_init first; the race DB gets riders 0-5 in order and the 6 player entries are cleared
 * (character slot flag set, byte +0x11 = 4). The character file is copied into cBELibrary_characterDB (0x550 bytes) and freed;
 * cBELibrary_statDefault gets each character's id, its 7 base stats from D_00440F00 plus 5, and 11 for the maxima. cBELibrary_optionDB is
 * cleared, the profiles of the 3 profile slots are initialised (cBEProfileLib_init with reload), and cBELibrary_grabPointsDB
 * is filled with the {points, points per second} pairs.
 */
extern "C" void cBELibrary_loadData(void)
{
    int i;
    void* buf;
    int* grabPoints;

    func_0014C6A0(D_004A6750);
    cBERewardDB_init(cBELibrary_rewardDB, 0);
    memset(&cBELibrary_raceDB, 0, 76);
    for (i = 0; i < 6; i++) {
        cBELibrary_raceDB.a[i] = i;
        cBELibrary_raceDB.b[i] = i;
    }
    memset(cBELibrary_playerDB, 0, sizeof(cBELibrary_playerDB));
    for (i = 0; i < 6; i++) {
        cBELibrary_playerDB[i].strUserName[0] = 0;
        cBELibrary_playerDB[i].iUseDualShock = 1;
        cBELibrary_playerDB[i].cCharacterID = 4;
    }
    buf = FILE_load(str_data_be_charDB_dbl, 0x100);
    func_0041605C(cBELibrary_characterDB, buf, 1360);
    if (buf != 0) {
        cMemMan_free(buf);
    }
    for (i = 0; i < 10; i++) {
        cBELibrary_statDefault[i].v[0] = i;
        cBELibrary_statDefault[i].v[1] = D_00440F00[i].v[0] + 5;
        cBELibrary_statDefault[i].v[2] = D_00440F00[i].v[1] + 5;
        cBELibrary_statDefault[i].v[3] = D_00440F00[i].v[2] + 5;
        cBELibrary_statDefault[i].v[4] = D_00440F00[i].v[3] + 5;
        cBELibrary_statDefault[i].v[5] = D_00440F00[i].v[4] + 5;
        cBELibrary_statDefault[i].v[6] = D_00440F00[i].v[5] + 5;
        cBELibrary_statDefault[i].v[7] = D_00440F00[i].v[6] + 5;
        cBELibrary_statDefault[i].v[9] = 11;
        cBELibrary_statDefault[i].v[11] = 11;
        cBELibrary_statDefault[i].v[8] = 11;
        cBELibrary_statDefault[i].v[12] = 11;
        cBELibrary_statDefault[i].v[14] = 11;
        cBELibrary_statDefault[i].v[13] = 11;
        cBELibrary_statDefault[i].v[10] = 11;
    }
    memset(cBELibrary_optionDB, 0, 648);
    for (i = 0; i < 3; i++) {
        cBEProfileLib_init(cBELibrary_profileLib + i * 39760, 1);
    }
    grabPoints = cBELibrary_grabPointsDB;
    grabPoints[0x0 / 4] = 0;
    grabPoints[0x4 / 4] = 0;
    grabPoints[0x18 / 4] = 200;
    grabPoints[0x1C / 4] = 400;
    grabPoints[0x20 / 4] = 200;
    grabPoints[36 / 4] = 400;
    grabPoints[40 / 4] = 200;
    grabPoints[44 / 4] = 400;
    grabPoints[0x30 / 4] = 200;
    grabPoints[52 / 4] = 400;
    grabPoints[0x38 / 4] = 250;
    grabPoints[0x3C / 4] = 400;
    grabPoints[0x40 / 4] = 250;
    grabPoints[68 / 4] = 400;
    grabPoints[72 / 4] = 250;
    grabPoints[76 / 4] = 400;
    grabPoints[80 / 4] = 300;
    grabPoints[84 / 4] = 400;
    grabPoints[88 / 4] = 300;
    grabPoints[92 / 4] = 400;
    grabPoints[0x60 / 4] = 300;
    grabPoints[100 / 4] = 400;
    grabPoints[104 / 4] = 300;
    grabPoints[108 / 4] = 400;
    grabPoints[0x70 / 4] = 350;
    grabPoints[116 / 4] = 400;
    grabPoints[0x78 / 4] = 300;
    grabPoints[0x7C / 4] = 400;
    grabPoints[0x80 / 4] = 300;
    grabPoints[132 / 4] = 400;
    grabPoints[136 / 4] = 300;
    grabPoints[140 / 4] = 400;
    grabPoints[152 / 4] = 200;
    grabPoints[156 / 4] = 500;
    grabPoints[160 / 4] = 200;
    grabPoints[164 / 4] = 500;
    grabPoints[168 / 4] = 200;
    grabPoints[172 / 4] = 500;
    grabPoints[176 / 4] = 200;
    grabPoints[180 / 4] = 500;
    grabPoints[184 / 4] = 200;
    grabPoints[188 / 4] = 500;
    grabPoints[0xC0 / 4] = 200;
    grabPoints[196 / 4] = 500;
    grabPoints[200 / 4] = 200;
    grabPoints[204 / 4] = 500;
    grabPoints[208 / 4] = 200;
    grabPoints[212 / 4] = 500;
    grabPoints[216 / 4] = 200;
    grabPoints[220 / 4] = 500;
    grabPoints[0xE0 / 4] = 200;
    grabPoints[228 / 4] = 500;
    grabPoints[232 / 4] = 200;
    grabPoints[236 / 4] = 500;
    grabPoints[0xF0 / 4] = 200;
    grabPoints[244 / 4] = 500;
    grabPoints[0xF8 / 4] = 200;
    grabPoints[0xFC / 4] = 500;
    grabPoints[0x100 / 4] = 200;
    grabPoints[260 / 4] = 500;
    grabPoints[264 / 4] = 200;
    grabPoints[268 / 4] = 500;
    grabPoints[328 / 4] = 1000;
    grabPoints[332 / 4] = 3000;
    grabPoints[344 / 4] = 2000;
    grabPoints[348 / 4] = 3000;
    grabPoints[360 / 4] = 2500;
    grabPoints[364 / 4] = 3000;
    grabPoints[336 / 4] = 1000;
    grabPoints[340 / 4] = 3000;
    grabPoints[352 / 4] = 2000;
    grabPoints[356 / 4] = 3000;
    grabPoints[368 / 4] = 2500;
    grabPoints[372 / 4] = 3000;
    grabPoints[376 / 4] = 1000;
    grabPoints[380 / 4] = 3000;
    grabPoints[392 / 4] = 2000;
    grabPoints[396 / 4] = 3000;
    grabPoints[408 / 4] = 2500;
    grabPoints[412 / 4] = 3000;
    grabPoints[0x180 / 4] = 1000;
    grabPoints[388 / 4] = 3000;
    grabPoints[400 / 4] = 2000;
    grabPoints[404 / 4] = 3000;
    grabPoints[416 / 4] = 2500;
    grabPoints[420 / 4] = 3000;
    grabPoints[424 / 4] = 1000;
    grabPoints[428 / 4] = 3000;
    grabPoints[440 / 4] = 2000;
    grabPoints[444 / 4] = 3000;
    grabPoints[456 / 4] = 2500;
    grabPoints[460 / 4] = 3000;
    grabPoints[432 / 4] = 1000;
    grabPoints[436 / 4] = 3000;
    grabPoints[0x1C0 / 4] = 2000;
    grabPoints[452 / 4] = 3000;
    grabPoints[464 / 4] = 2500;
    grabPoints[468 / 4] = 3000;
    grabPoints[280 / 4] = 1000;
    grabPoints[284 / 4] = 3000;
    grabPoints[296 / 4] = 2000;
    grabPoints[300 / 4] = 3000;
    grabPoints[312 / 4] = 2500;
    grabPoints[316 / 4] = 3000;
    grabPoints[288 / 4] = 1000;
    grabPoints[292 / 4] = 3000;
    grabPoints[304 / 4] = 2000;
    grabPoints[308 / 4] = 3000;
    grabPoints[320 / 4] = 2500;
    grabPoints[324 / 4] = 3000;
    grabPoints[520 / 4] = 1000;
    grabPoints[524 / 4] = 3000;
    grabPoints[536 / 4] = 2000;
    grabPoints[540 / 4] = 3000;
    grabPoints[552 / 4] = 2500;
    grabPoints[556 / 4] = 3000;
    grabPoints[528 / 4] = 1000;
    grabPoints[532 / 4] = 3000;
    grabPoints[544 / 4] = 2000;
    grabPoints[548 / 4] = 3000;
    grabPoints[560 / 4] = 2500;
    grabPoints[564 / 4] = 3000;
    grabPoints[472 / 4] = 1000;
    grabPoints[476 / 4] = 3000;
    grabPoints[488 / 4] = 2000;
    grabPoints[492 / 4] = 3000;
    grabPoints[0x1F8 / 4] = 2500;
    grabPoints[0x1FC / 4] = 3000;
    grabPoints[0x1E0 / 4] = 1000;
    grabPoints[484 / 4] = 3000;
    grabPoints[0x1F0 / 4] = 2000;
    grabPoints[500 / 4] = 3000;
    grabPoints[0x200 / 4] = 2500;
    grabPoints[516 / 4] = 3000;
    grabPoints[568 / 4] = 3000;
    grabPoints[572 / 4] = 4000;
    grabPoints[576 / 4] = 3000;
    grabPoints[580 / 4] = 4000;
    grabPoints[584 / 4] = 3000;
    grabPoints[588 / 4] = 4000;
    grabPoints[592 / 4] = 3000;
    grabPoints[596 / 4] = 4000;
    grabPoints[600 / 4] = 3000;
    grabPoints[604 / 4] = 4000;
    grabPoints[608 / 4] = 3000;
    grabPoints[612 / 4] = 4000;
    grabPoints[616 / 4] = 3000;
    grabPoints[620 / 4] = 4000;
    grabPoints[624 / 4] = 3000;
    grabPoints[628 / 4] = 4000;
    grabPoints[632 / 4] = 3000;
    grabPoints[636 / 4] = 4000;
    grabPoints[640 / 4] = 3000;
    grabPoints[644 / 4] = 4000;
    grabPoints[648 / 4] = 5000;
    grabPoints[652 / 4] = 5000;
    grabPoints[656 / 4] = 5000;
    grabPoints[660 / 4] = 5000;
    grabPoints[664 / 4] = 5000;
    grabPoints[668 / 4] = 5000;
    grabPoints[672 / 4] = 5000;
    grabPoints[676 / 4] = 5000;
}
#endif


//100%
INCLUDE_ASM("be/beintstat", cBELibrary_debugReloadData);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty function; cBE_reloadData calls it with cBE_library).
 */
extern "C" void cBELibrary_debugReloadData(void) {

}
#endif
