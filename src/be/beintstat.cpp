#include "common.h"
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
extern sPlayerStatRow_getCollisionAttrib D_00535538_getCollisionAttrib[] __asm__("D_00535538");
extern sCharStat_getCollisionAttrib D_005308D8_getCollisionAttrib[] __asm__("D_005308D8");

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
extern "C" float cBEStatInterface_getCollisionAttrib(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int attribLevel;
    if (value > 0)
        attribLevel = value;
    else
        attribLevel = D_00535538_getCollisionAttrib[profile].e[charID][5] / 5;
    return (float)attribLevel / (float)D_005308D8_getCollisionAttrib[charID].v[0xD];
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
extern sPlayerStatRow_cBEStatInterface_getStrikingAttrib D_00535538_cBEStatInterface_getStrikingAttrib[] __asm__("D_00535538");
extern sCharStat_cBEStatInterface_getStrikingAttrib D_005308D8_cBEStatInterface_getStrikingAttrib[] __asm__("D_005308D8");

/**
 * Returns how hard a rider hits, as a fraction of the character's best.
 *
 * A mission man (cBENewPlayerInterface_isMissionMan) gets 0.5. Otherwise the level is @p value when positive, else
 * attribute 5 of the rider's profile/character stats (D_00535538) / 5, divided by the character's entry 0xD in
 * D_005308D8.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  a level that overrides the rider's own when positive (On Tour overrideAttribLevel)
 * @return the striking attribute, usually 0 to 1
 */
extern "C" float cBEStatInterface_getStrikingAttrib(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_cBEStatInterface_getStrikingAttrib[profile].e[c][5] / 5;
    return (float)v / (float)D_005308D8_cBEStatInterface_getStrikingAttrib[c].v[0xD];
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
extern sPlayerStatRow_cBEStatInterface_getLandingAttrib D_00535538_cBEStatInterface_getLandingAttrib[] __asm__("D_00535538");
extern sCharStat_cBEStatInterface_getLandingAttrib D_005308D8_cBEStatInterface_getLandingAttrib[] __asm__("D_005308D8");

/**
 * Returns how well a rider lands, as a fraction of the character's best.
 *
 * A mission man gets 0.5. Otherwise the level is @p value when positive, else attribute 6 of the rider's
 * profile/character stats (D_00535538) / 5, divided by the character's entry 0xE in D_005308D8.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  a level that overrides the rider's own when positive (On Tour overrideAttribLevel)
 * @return the landing attribute, usually 0 to 1
 */
extern "C" float cBEStatInterface_getLandingAttrib(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_cBEStatInterface_getLandingAttrib[profile].e[c][6] / 5;
    return (float)v / (float)D_005308D8_cBEStatInterface_getLandingAttrib[c].v[0xE];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_00149208);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_func_00149208
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_func_00149208
{
    signed char v[15];
};
extern sPlayerStatRow_func_00149208 D_00535538_func_00149208[] __asm__("D_00535538");
extern sCharStat_func_00149208 D_005308D8_func_00149208[] __asm__("D_005308D8");

extern "C" float func_00149208(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_func_00149208[profile].e[c][6] / 5;
    return (float)v / (float)D_005308D8_func_00149208[c].v[0xE];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_001492F0);
#ifdef SKIP_ASM
extern "C" float func_001492F0(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_cBEStatInterface_getStrikingAttrib[profile].e[c][6] / 5;
    return (float)v / (float)D_005308D8_cBEStatInterface_getStrikingAttrib[c].v[0xE];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_001493D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_func_001493D8
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_func_001493D8
{
    signed char v[15];
};
extern sPlayerStatRow_func_001493D8 D_00535538_func_001493D8[] __asm__("D_00535538");
extern sCharStat_func_001493D8 D_005308D8_func_001493D8[] __asm__("D_005308D8");

extern "C" float func_001493D8(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_func_001493D8[profile].e[c][1] / 5;
    return (float)v / (float)D_005308D8_func_001493D8[c].v[0x9];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_001494C0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_func_001494C0
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_func_001494C0
{
    signed char v[15];
};
extern sPlayerStatRow_func_001494C0 D_00535538_func_001494C0[] __asm__("D_00535538");
extern sCharStat_func_001494C0 D_005308D8_func_001494C0[] __asm__("D_005308D8");

extern "C" float func_001494C0(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_func_001494C0[profile].e[c][0] / 5;
    return (float)v / (float)D_005308D8_func_001494C0[c].v[0x8];
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
extern sPlayerStatRow_cBEStatInterface_getSpinAttrib D_00535538_cBEStatInterface_getSpinAttrib[] __asm__("D_00535538");
extern sCharStat_cBEStatInterface_getSpinAttrib D_005308D8_cBEStatInterface_getSpinAttrib[] __asm__("D_005308D8");

/**
 * Returns how fast a rider spins, as a fraction of the character's best.
 *
 * A mission man gets 0.5. Otherwise the level is @p value when positive, else attribute 4 of the rider's
 * profile/character stats (D_00535538) / 5, divided by the character's entry 0xC in D_005308D8.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  a level that overrides the rider's own when positive (On Tour overrideAttribLevel)
 * @return the spin attribute, usually 0 to 1
 */
extern "C" float cBEStatInterface_getSpinAttrib(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_cBEStatInterface_getSpinAttrib[profile].e[c][4] / 5;
    return (float)v / (float)D_005308D8_cBEStatInterface_getSpinAttrib[c].v[0xC];
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
extern sPlayerStatRow_cBEStatInterface_getTrickAttrib D_00535538_cBEStatInterface_getTrickAttrib[] __asm__("D_00535538");
extern sCharStat_cBEStatInterface_getTrickAttrib D_005308D8_cBEStatInterface_getTrickAttrib[] __asm__("D_005308D8");

/**
 * Returns a rider's trick skill, as a fraction of the character's best.
 *
 * A mission man gets 0.5. Otherwise the level is @p value when positive, else attribute 2 of the rider's
 * profile/character stats (D_00535538) / 5, divided by the character's entry 0xA in D_005308D8.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  a level that overrides the rider's own when positive (On Tour overrideAttribLevel)
 * @return the trick attribute, usually 0 to 1
 */
extern "C" float cBEStatInterface_getTrickAttrib(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_cBEStatInterface_getTrickAttrib[profile].e[c][2] / 5;
    return (float)v / (float)D_005308D8_cBEStatInterface_getTrickAttrib[c].v[0xA];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBEStatInterface_shouldCelebrateTrick);
#ifdef SKIP_ASM
int cBENewPlayerInterface_isMissionMan(int index);
extern "C" int func_00148950(void* iface, int id);

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
 * Mission men never celebrate. The threshold is D_0045A2B0[clamp(func_00148950 level - 1, 0, 10)] (On Tour getStatLevel
 * and bracket).
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the trick's points (On Tour iPoints)
 * @return 1 when the points reach the threshold
 */
extern "C" int cBEStatInterface_shouldCelebrateTrick(void* self, int rider, int value)
{
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0;
    sLevelTable_00149778 t = D_0045A2B0;
    int n = func_00148950(self, rider) - 1;
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
    return value >= t.v[lvl];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", cBELibrary_cBELibrary);
#ifdef SKIP_ASM
extern "C" void cBELibrary_loadData();
extern "C" void* cBECharacterInterface_getThis();
extern "C" void* cBEStatInterface_getThis(void);
extern "C" void* func_0014ABE0();
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
extern sStatDefaults_149860 D_00535C18;
extern sStatDefaults_149860 D_0043FB28;

/**
 * Constructs the BE library: resets its data tables and creates every BE interface singleton.
 *
 * Calls cBELibrary_loadData() (resets the character, player and profile tables), copies the default score tables (26
 * entries of 0x64 bytes, D_0043FB28) into D_00535C18, then calls each interface's getThis(): new race, new
 * player, character, stat, option, save, BAGT, network, score, bolt (func_0014ABE0), mission, econ, aggression
 * and reward.
 *
 * @return @p self
 */
extern "C" void* cBELibrary_cBELibrary(void* self)
{
    cBELibrary_loadData();
    D_00535C18 = D_0043FB28;
    cBENewRaceInterface_getThis();
    cBENewPlayerInterface_getThis();
    cBECharacterInterface_getThis();
    cBEStatInterface_getThis();
    cBEOptionInterface_getThis();
    cBESaveInterface_getThis();
    cBEBAGTInterface_getThis();
    cBENetworkInterface_getThis();
    cBEScoreInterface_getThis();
    func_0014ABE0();
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
extern int* cBENewRaceInterface_instance;
extern int* cBENewPlayerInterface_instance;
extern int* cBEStatInterface_instance;
extern int* D_004A11C8;
extern int* cBECharacterInterface_instance;
extern int* cBEOptionInterface_instance;
extern int* cBEBAGTInterface_instance;
extern int* cBEEconInterface_instance;
extern int* cBEMissionInterface_instance;
extern int* cBEScoreInterface_instance;
extern int* cBEAggressionInterface_instance;

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
    operator_delete(cBENewRaceInterface_instance);
    cBENewRaceInterface_instance = 0;
    operator_delete(cBENewPlayerInterface_instance);
    cBENewPlayerInterface_instance = 0;
    operator_delete(cBECharacterInterface_instance);
    cBECharacterInterface_instance = 0;
    operator_delete(cBEStatInterface_instance);
    cBEStatInterface_instance = 0;
    operator_delete(cBEOptionInterface_instance);
    cBEOptionInterface_instance = 0;
    cBESaveInterface_purgeInterface();
    operator_delete(cBEBAGTInterface_instance);
    cBEBAGTInterface_instance = 0;
    operator_delete((int*)cBENetworkInterface_instance);
    cBENetworkInterface_instance = 0;
    operator_delete(cBEScoreInterface_instance);
    cBEScoreInterface_instance = 0;
    operator_delete(D_004A11C8);
    D_004A11C8 = 0;
    operator_delete(cBEMissionInterface_instance);
    cBEMissionInterface_instance = 0;
    operator_delete(cBEEconInterface_instance);
    cBEEconInterface_instance = 0;
    operator_delete(cBEAggressionInterface_instance);
    cBEAggressionInterface_instance = 0;
    operator_delete((int*)cBERewardInterface_instance);
    cBERewardInterface_instance = 0;
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_00149A88);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* strcpy2(char* buf, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cBEProfileLib_init(void* p, int v);
extern "C" void* cBEStatInterface_getThis(void);
int GetHashValue32(char*);
void* cBENewPlayerInterface_getThis();
void* cBEBAGTInterface_getThis();
extern char str_kT_MEMPlayerName[];
extern char cBELibrary_playerDB[];
extern char cBELibrary_profileLib[];

struct sVEntry_149A88
{
    short delta;
    short index;
    int (*fn)(void*, ...);
};

struct sRiderEntry_149A88
{
    char name[0x10];
    unsigned f0 : 1;
    unsigned f1 : 1;
    unsigned pad : 6;
    unsigned char c11;
};

static inline void vreset_149A88(char* o)
{
    sVEntry_149A88* vt = *(sVEntry_149A88**)(o + 0xC);
    vt[2].fn(o + vt[2].delta);
}

extern "C" void func_00149A88(void* self, signed char idx)
{
    char buf[64];
    sRiderEntry_149A88* e = (sRiderEntry_149A88*)(cBELibrary_playerDB + idx * 0x1C);
    func_00416210(e, 0, 0x1C);
    e->f1 = 1;
    e->c11 = 4;
    char* o = (char*)SSXApp->FELocale;
    sVEntry_149A88* vt = *(sVEntry_149A88**)(o + 4);
    strcpy2(buf, vt[4].fn(o + vt[4].delta, GetHashValue32(str_kT_MEMPlayerName)));
    sprintf((char*)e, buf, idx + 1);
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
extern "C" void* func_00416210(void* dst, int c, int n);
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

struct sRiderEntry_149BB8
{
    char name[0x10];
    unsigned f0 : 1;
    unsigned f1 : 1;
    unsigned pad : 6;
    unsigned char c11;
    char pad12[0x1C - 0x12];
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
extern sRiderEntry_149BB8 D_00535B20_149BB8[6] __asm__("cBELibrary_playerDB");
extern const char str_data_be_charDB_dbl[];
extern char D_00530970[];
extern sCharStat_149BB8 D_005308D8[10];
extern sCharSrc_149BB8 D_00440F00[10];
extern char D_00535898[];

extern int cBELibrary_grabPointsDB[0x2A8 / 4];

/**
 * Sets up the career databases at boot: rewards, race and player setup, character data, profiles and grab points.
 *
 * func_0014C6A0 and cBERewardDB_init first; the race DB gets riders 0-5 in order and the 6 player entries are cleared
 * (character slot flag set, byte +0x11 = 4). The character file is copied into D_00530970 (0x550 bytes) and freed;
 * D_005308D8 gets each character's id, its 7 base stats from D_00440F00 plus 5, and 11 for the maxima. D_00535898 is
 * cleared, the profiles of the 3 profile slots are initialised (cBEProfileLib_init with reload), and cBELibrary_grabPointsDB
 * is filled with the {points, points per second} pairs.
 */
extern "C" void cBELibrary_loadData(void)
{
    int i;
    void* buf;
    int* p;

    func_0014C6A0(D_004A6750);
    cBERewardDB_init(cBELibrary_rewardDB, 0);
    func_00416210(&cBELibrary_raceDB, 0, 76);
    for (i = 0; i < 6; i++) {
        cBELibrary_raceDB.a[i] = i;
        cBELibrary_raceDB.b[i] = i;
    }
    func_00416210(D_00535B20_149BB8, 0, 168);
    for (i = 0; i < 6; i++) {
        D_00535B20_149BB8[i].name[0] = 0;
        D_00535B20_149BB8[i].f1 = 1;
        D_00535B20_149BB8[i].c11 = 4;
    }
    buf = FILE_load(str_data_be_charDB_dbl, 0x100);
    func_0041605C(D_00530970, buf, 1360);
    if (buf != 0) {
        cMemMan_free(buf);
    }
    for (i = 0; i < 10; i++) {
        D_005308D8[i].v[0] = i;
        D_005308D8[i].v[1] = D_00440F00[i].v[0] + 5;
        D_005308D8[i].v[2] = D_00440F00[i].v[1] + 5;
        D_005308D8[i].v[3] = D_00440F00[i].v[2] + 5;
        D_005308D8[i].v[4] = D_00440F00[i].v[3] + 5;
        D_005308D8[i].v[5] = D_00440F00[i].v[4] + 5;
        D_005308D8[i].v[6] = D_00440F00[i].v[5] + 5;
        D_005308D8[i].v[7] = D_00440F00[i].v[6] + 5;
        D_005308D8[i].v[9] = 11;
        D_005308D8[i].v[11] = 11;
        D_005308D8[i].v[8] = 11;
        D_005308D8[i].v[12] = 11;
        D_005308D8[i].v[14] = 11;
        D_005308D8[i].v[13] = 11;
        D_005308D8[i].v[10] = 11;
    }
    func_00416210(D_00535898, 0, 648);
    for (i = 0; i < 3; i++) {
        cBEProfileLib_init(cBELibrary_profileLib + i * 39760, 1);
    }
    p = cBELibrary_grabPointsDB;
    p[0x0 / 4] = 0;
    p[0x4 / 4] = 0;
    p[0x18 / 4] = 200;
    p[0x1C / 4] = 400;
    p[0x20 / 4] = 200;
    p[36 / 4] = 400;
    p[40 / 4] = 200;
    p[44 / 4] = 400;
    p[0x30 / 4] = 200;
    p[52 / 4] = 400;
    p[0x38 / 4] = 250;
    p[0x3C / 4] = 400;
    p[0x40 / 4] = 250;
    p[68 / 4] = 400;
    p[72 / 4] = 250;
    p[76 / 4] = 400;
    p[80 / 4] = 300;
    p[84 / 4] = 400;
    p[88 / 4] = 300;
    p[92 / 4] = 400;
    p[0x60 / 4] = 300;
    p[100 / 4] = 400;
    p[104 / 4] = 300;
    p[108 / 4] = 400;
    p[0x70 / 4] = 350;
    p[116 / 4] = 400;
    p[0x78 / 4] = 300;
    p[0x7C / 4] = 400;
    p[0x80 / 4] = 300;
    p[132 / 4] = 400;
    p[136 / 4] = 300;
    p[140 / 4] = 400;
    p[152 / 4] = 200;
    p[156 / 4] = 500;
    p[160 / 4] = 200;
    p[164 / 4] = 500;
    p[168 / 4] = 200;
    p[172 / 4] = 500;
    p[176 / 4] = 200;
    p[180 / 4] = 500;
    p[184 / 4] = 200;
    p[188 / 4] = 500;
    p[0xC0 / 4] = 200;
    p[196 / 4] = 500;
    p[200 / 4] = 200;
    p[204 / 4] = 500;
    p[208 / 4] = 200;
    p[212 / 4] = 500;
    p[216 / 4] = 200;
    p[220 / 4] = 500;
    p[0xE0 / 4] = 200;
    p[228 / 4] = 500;
    p[232 / 4] = 200;
    p[236 / 4] = 500;
    p[0xF0 / 4] = 200;
    p[244 / 4] = 500;
    p[0xF8 / 4] = 200;
    p[0xFC / 4] = 500;
    p[0x100 / 4] = 200;
    p[260 / 4] = 500;
    p[264 / 4] = 200;
    p[268 / 4] = 500;
    p[328 / 4] = 1000;
    p[332 / 4] = 3000;
    p[344 / 4] = 2000;
    p[348 / 4] = 3000;
    p[360 / 4] = 2500;
    p[364 / 4] = 3000;
    p[336 / 4] = 1000;
    p[340 / 4] = 3000;
    p[352 / 4] = 2000;
    p[356 / 4] = 3000;
    p[368 / 4] = 2500;
    p[372 / 4] = 3000;
    p[376 / 4] = 1000;
    p[380 / 4] = 3000;
    p[392 / 4] = 2000;
    p[396 / 4] = 3000;
    p[408 / 4] = 2500;
    p[412 / 4] = 3000;
    p[0x180 / 4] = 1000;
    p[388 / 4] = 3000;
    p[400 / 4] = 2000;
    p[404 / 4] = 3000;
    p[416 / 4] = 2500;
    p[420 / 4] = 3000;
    p[424 / 4] = 1000;
    p[428 / 4] = 3000;
    p[440 / 4] = 2000;
    p[444 / 4] = 3000;
    p[456 / 4] = 2500;
    p[460 / 4] = 3000;
    p[432 / 4] = 1000;
    p[436 / 4] = 3000;
    p[0x1C0 / 4] = 2000;
    p[452 / 4] = 3000;
    p[464 / 4] = 2500;
    p[468 / 4] = 3000;
    p[280 / 4] = 1000;
    p[284 / 4] = 3000;
    p[296 / 4] = 2000;
    p[300 / 4] = 3000;
    p[312 / 4] = 2500;
    p[316 / 4] = 3000;
    p[288 / 4] = 1000;
    p[292 / 4] = 3000;
    p[304 / 4] = 2000;
    p[308 / 4] = 3000;
    p[320 / 4] = 2500;
    p[324 / 4] = 3000;
    p[520 / 4] = 1000;
    p[524 / 4] = 3000;
    p[536 / 4] = 2000;
    p[540 / 4] = 3000;
    p[552 / 4] = 2500;
    p[556 / 4] = 3000;
    p[528 / 4] = 1000;
    p[532 / 4] = 3000;
    p[544 / 4] = 2000;
    p[548 / 4] = 3000;
    p[560 / 4] = 2500;
    p[564 / 4] = 3000;
    p[472 / 4] = 1000;
    p[476 / 4] = 3000;
    p[488 / 4] = 2000;
    p[492 / 4] = 3000;
    p[0x1F8 / 4] = 2500;
    p[0x1FC / 4] = 3000;
    p[0x1E0 / 4] = 1000;
    p[484 / 4] = 3000;
    p[0x1F0 / 4] = 2000;
    p[500 / 4] = 3000;
    p[0x200 / 4] = 2500;
    p[516 / 4] = 3000;
    p[568 / 4] = 3000;
    p[572 / 4] = 4000;
    p[576 / 4] = 3000;
    p[580 / 4] = 4000;
    p[584 / 4] = 3000;
    p[588 / 4] = 4000;
    p[592 / 4] = 3000;
    p[596 / 4] = 4000;
    p[600 / 4] = 3000;
    p[604 / 4] = 4000;
    p[608 / 4] = 3000;
    p[612 / 4] = 4000;
    p[616 / 4] = 3000;
    p[620 / 4] = 4000;
    p[624 / 4] = 3000;
    p[628 / 4] = 4000;
    p[632 / 4] = 3000;
    p[636 / 4] = 4000;
    p[640 / 4] = 3000;
    p[644 / 4] = 4000;
    p[648 / 4] = 5000;
    p[652 / 4] = 5000;
    p[656 / 4] = 5000;
    p[660 / 4] = 5000;
    p[664 / 4] = 5000;
    p[668 / 4] = 5000;
    p[672 / 4] = 5000;
    p[676 / 4] = 5000;
}
#endif


//100%
INCLUDE_ASM("be/beintstat", func_0014A078);
#ifdef SKIP_ASM
extern "C" void func_0014A078(void) {

}
#endif
