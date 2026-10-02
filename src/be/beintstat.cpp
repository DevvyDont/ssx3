#include "common.h"

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

extern "C" float cBEStatInterface_getCollisionAttrib(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_getCollisionAttrib[profile].e[c][5] / 5;
    return (float)v / (float)D_005308D8_getCollisionAttrib[c].v[0xD];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_00149038);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_func_00149038
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_func_00149038
{
    signed char v[15];
};
extern sPlayerStatRow_func_00149038 D_00535538_func_00149038[] __asm__("D_00535538");
extern sCharStat_func_00149038 D_005308D8_func_00149038[] __asm__("D_005308D8");

extern "C" float func_00149038(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_func_00149038[profile].e[c][5] / 5;
    return (float)v / (float)D_005308D8_func_00149038[c].v[0xD];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_00149120);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_func_00149120
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_func_00149120
{
    signed char v[15];
};
extern sPlayerStatRow_func_00149120 D_00535538_func_00149120[] __asm__("D_00535538");
extern sCharStat_func_00149120 D_005308D8_func_00149120[] __asm__("D_005308D8");

extern "C" float func_00149120(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_func_00149120[profile].e[c][6] / 5;
    return (float)v / (float)D_005308D8_func_00149120[c].v[0xE];
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
INCLUDE_ASM("be/beintstat", func_001495A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_func_001495A8
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_func_001495A8
{
    signed char v[15];
};
extern sPlayerStatRow_func_001495A8 D_00535538_func_001495A8[] __asm__("D_00535538");
extern sCharStat_func_001495A8 D_005308D8_func_001495A8[] __asm__("D_005308D8");

extern "C" float func_001495A8(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_func_001495A8[profile].e[c][4] / 5;
    return (float)v / (float)D_005308D8_func_001495A8[c].v[0xC];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_00149690);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
int cBENewPlayerInterface_isMissionMan(int index);

struct sPlayerStatRow_func_00149690
{
    signed char e[10][7]; // 0x46 bytes
};
struct sCharStat_func_00149690
{
    signed char v[15];
};
extern sPlayerStatRow_func_00149690 D_00535538_func_00149690[] __asm__("D_00535538");
extern sCharStat_func_00149690 D_005308D8_func_00149690[] __asm__("D_005308D8");

extern "C" float func_00149690(void* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int v;
    if (value > 0)
        v = value;
    else
        v = D_00535538_func_00149690[profile].e[c][2] / 5;
    return (float)v / (float)D_005308D8_func_00149690[c].v[0xA];
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_00149778);
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

extern "C" int func_00149778(void* self, int rider, int value)
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

INCLUDE_ASM("be/beintstat", func_00149860);

INCLUDE_ASM("be/beintstat", func_001499A8);

INCLUDE_ASM("be/beintstat", func_00149A88);

INCLUDE_ASM("be/beintstat", func_00149BB8);

