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
        v = D_00535538_func_00149038[profile].e[c][6] / 5;
    return (float)v / (float)D_005308D8_func_00149038[c].v[0xE];
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

//100%
INCLUDE_ASM("be/beintstat", func_00149860);
#ifdef SKIP_ASM
extern "C" void func_00149BB8();
extern "C" void* func_0014EDD8();
extern "C" void* func_00147F78(void);
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

extern "C" void* func_00149860(void* self)
{
    func_00149BB8();
    D_00535C18 = D_0043FB28;
    cBENewRaceInterface_getThis();
    cBENewPlayerInterface_getThis();
    func_0014EDD8();
    func_00147F78();
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
INCLUDE_ASM("be/beintstat", func_001499A8);
#ifdef SKIP_ASM
void operator_delete(int*);
extern "C" void func_00153050(void);
extern int* D_004A11B4;
extern int* D_004A11C0;
extern int* D_004A11C4;
extern int* D_004A11C8;
extern int* D_004A1204;
extern int* D_004A1208;
extern int* D_004A120C;
extern int* D_004A1210;
extern int* D_004A1214;
extern int* D_004A1238;
extern int* D_004A1248;
extern int* D_004A124C;
extern int* D_004A1264;

extern "C" void func_001499A8(void* self, int flags)
{
    operator_delete(D_004A11B4);
    D_004A11B4 = 0;
    operator_delete(D_004A11C0);
    D_004A11C0 = 0;
    operator_delete(D_004A1208);
    D_004A1208 = 0;
    operator_delete(D_004A11C4);
    D_004A11C4 = 0;
    operator_delete(D_004A120C);
    D_004A120C = 0;
    func_00153050();
    operator_delete(D_004A1210);
    D_004A1210 = 0;
    operator_delete(D_004A1204);
    D_004A1204 = 0;
    operator_delete(D_004A1248);
    D_004A1248 = 0;
    operator_delete(D_004A11C8);
    D_004A11C8 = 0;
    operator_delete(D_004A1238);
    D_004A1238 = 0;
    operator_delete(D_004A1214);
    D_004A1214 = 0;
    operator_delete(D_004A124C);
    D_004A124C = 0;
    operator_delete(D_004A1264);
    D_004A1264 = 0;
    if (flags & 1)
        operator_delete((int*)self);
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_00149A88);
#ifdef SKIP_ASM
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* func_002C2580(char* buf, int id);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void func_001567B8(void* p, int v);
extern "C" void* func_00147F78(void);
int GetHashValue32(char*);
void* cBENewPlayerInterface_getThis();
void* cBEBAGTInterface_getThis();
extern char* D_004A28A8;
extern char D_0045A268[];
extern char D_00535B20[];
extern char D_004A6CA8[];

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
    sRiderEntry_149A88* e = (sRiderEntry_149A88*)(D_00535B20 + idx * 0x1C);
    func_00416210(e, 0, 0x1C);
    e->f1 = 1;
    e->c11 = 4;
    char* o = *(char**)(D_004A28A8 + 0x8C);
    sVEntry_149A88* vt = *(sVEntry_149A88**)(o + 4);
    func_002C2580(buf, vt[4].fn(o + vt[4].delta, GetHashValue32(D_0045A268)));
    sprintf((char*)e, buf, idx + 1);
    func_001567B8(D_004A6CA8 + idx * 0x9B50, 1);
    vreset_149A88((char*)cBENewPlayerInterface_getThis());
    vreset_149A88((char*)func_00147F78());
    vreset_149A88((char*)cBEBAGTInterface_getThis());
}
#endif

//100%
INCLUDE_ASM("be/beintstat", func_00149BB8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0014C6A0(void*);
extern "C" void cBERewardDB_init(void*, int);
extern "C" void* func_00416210(void* dst, int c, int n);
extern "C" void* FILE_load(const char* name, int flags);
extern "C" void func_0041605C(void* dst, void* src, int n);
void cMemMan_free(void*);
extern "C" void func_001567B8(void* p, int v);

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
extern char D_004C3E98[];
extern sCharSlot_149BB8 D_005305B0;
extern sRiderEntry_149BB8 D_00535B20_149BB8[6] __asm__("D_00535B20");
extern const char D_0045A2E0[];
extern char D_00530970[];
extern sCharStat_149BB8 D_005308D8[10];
extern sCharSrc_149BB8 D_00440F00[10];
extern char D_00535898[];

extern int D_00530600[0x2A8 / 4];

extern "C" void func_00149BB8(void)
{
    int i;
    void* buf;
    int* p;

    func_0014C6A0(D_004A6750);
    cBERewardDB_init(D_004C3E98, 0);
    func_00416210(&D_005305B0, 0, 0x4C);
    for (i = 0; i < 6; i++) {
        D_005305B0.a[i] = i;
        D_005305B0.b[i] = i;
    }
    func_00416210(D_00535B20_149BB8, 0, 0xA8);
    for (i = 0; i < 6; i++) {
        D_00535B20_149BB8[i].name[0] = 0;
        D_00535B20_149BB8[i].f1 = 1;
        D_00535B20_149BB8[i].c11 = 4;
    }
    buf = FILE_load(D_0045A2E0, 0x100);
    func_0041605C(D_00530970, buf, 0x550);
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
        D_005308D8[i].v[9] = 0xB;
        D_005308D8[i].v[11] = 0xB;
        D_005308D8[i].v[8] = 0xB;
        D_005308D8[i].v[12] = 0xB;
        D_005308D8[i].v[14] = 0xB;
        D_005308D8[i].v[13] = 0xB;
        D_005308D8[i].v[10] = 0xB;
    }
    func_00416210(D_00535898, 0, 0x288);
    for (i = 0; i < 3; i++) {
        func_001567B8(D_004A6CA8 + i * 0x9B50, 1);
    }
    p = D_00530600;
    p[0x0 / 4] = 0;
    p[0x4 / 4] = 0;
    p[0x18 / 4] = 0xC8;
    p[0x1C / 4] = 0x190;
    p[0x20 / 4] = 0xC8;
    p[0x24 / 4] = 0x190;
    p[0x28 / 4] = 0xC8;
    p[0x2C / 4] = 0x190;
    p[0x30 / 4] = 0xC8;
    p[0x34 / 4] = 0x190;
    p[0x38 / 4] = 0xFA;
    p[0x3C / 4] = 0x190;
    p[0x40 / 4] = 0xFA;
    p[0x44 / 4] = 0x190;
    p[0x48 / 4] = 0xFA;
    p[0x4C / 4] = 0x190;
    p[0x50 / 4] = 0x12C;
    p[0x54 / 4] = 0x190;
    p[0x58 / 4] = 0x12C;
    p[0x5C / 4] = 0x190;
    p[0x60 / 4] = 0x12C;
    p[0x64 / 4] = 0x190;
    p[0x68 / 4] = 0x12C;
    p[0x6C / 4] = 0x190;
    p[0x70 / 4] = 0x15E;
    p[0x74 / 4] = 0x190;
    p[0x78 / 4] = 0x12C;
    p[0x7C / 4] = 0x190;
    p[0x80 / 4] = 0x12C;
    p[0x84 / 4] = 0x190;
    p[0x88 / 4] = 0x12C;
    p[0x8C / 4] = 0x190;
    p[0x98 / 4] = 0xC8;
    p[0x9C / 4] = 0x1F4;
    p[0xA0 / 4] = 0xC8;
    p[0xA4 / 4] = 0x1F4;
    p[0xA8 / 4] = 0xC8;
    p[0xAC / 4] = 0x1F4;
    p[0xB0 / 4] = 0xC8;
    p[0xB4 / 4] = 0x1F4;
    p[0xB8 / 4] = 0xC8;
    p[0xBC / 4] = 0x1F4;
    p[0xC0 / 4] = 0xC8;
    p[0xC4 / 4] = 0x1F4;
    p[0xC8 / 4] = 0xC8;
    p[0xCC / 4] = 0x1F4;
    p[0xD0 / 4] = 0xC8;
    p[0xD4 / 4] = 0x1F4;
    p[0xD8 / 4] = 0xC8;
    p[0xDC / 4] = 0x1F4;
    p[0xE0 / 4] = 0xC8;
    p[0xE4 / 4] = 0x1F4;
    p[0xE8 / 4] = 0xC8;
    p[0xEC / 4] = 0x1F4;
    p[0xF0 / 4] = 0xC8;
    p[0xF4 / 4] = 0x1F4;
    p[0xF8 / 4] = 0xC8;
    p[0xFC / 4] = 0x1F4;
    p[0x100 / 4] = 0xC8;
    p[0x104 / 4] = 0x1F4;
    p[0x108 / 4] = 0xC8;
    p[0x10C / 4] = 0x1F4;
    p[0x148 / 4] = 0x3E8;
    p[0x14C / 4] = 0xBB8;
    p[0x158 / 4] = 0x7D0;
    p[0x15C / 4] = 0xBB8;
    p[0x168 / 4] = 0x9C4;
    p[0x16C / 4] = 0xBB8;
    p[0x150 / 4] = 0x3E8;
    p[0x154 / 4] = 0xBB8;
    p[0x160 / 4] = 0x7D0;
    p[0x164 / 4] = 0xBB8;
    p[0x170 / 4] = 0x9C4;
    p[0x174 / 4] = 0xBB8;
    p[0x178 / 4] = 0x3E8;
    p[0x17C / 4] = 0xBB8;
    p[0x188 / 4] = 0x7D0;
    p[0x18C / 4] = 0xBB8;
    p[0x198 / 4] = 0x9C4;
    p[0x19C / 4] = 0xBB8;
    p[0x180 / 4] = 0x3E8;
    p[0x184 / 4] = 0xBB8;
    p[0x190 / 4] = 0x7D0;
    p[0x194 / 4] = 0xBB8;
    p[0x1A0 / 4] = 0x9C4;
    p[0x1A4 / 4] = 0xBB8;
    p[0x1A8 / 4] = 0x3E8;
    p[0x1AC / 4] = 0xBB8;
    p[0x1B8 / 4] = 0x7D0;
    p[0x1BC / 4] = 0xBB8;
    p[0x1C8 / 4] = 0x9C4;
    p[0x1CC / 4] = 0xBB8;
    p[0x1B0 / 4] = 0x3E8;
    p[0x1B4 / 4] = 0xBB8;
    p[0x1C0 / 4] = 0x7D0;
    p[0x1C4 / 4] = 0xBB8;
    p[0x1D0 / 4] = 0x9C4;
    p[0x1D4 / 4] = 0xBB8;
    p[0x118 / 4] = 0x3E8;
    p[0x11C / 4] = 0xBB8;
    p[0x128 / 4] = 0x7D0;
    p[0x12C / 4] = 0xBB8;
    p[0x138 / 4] = 0x9C4;
    p[0x13C / 4] = 0xBB8;
    p[0x120 / 4] = 0x3E8;
    p[0x124 / 4] = 0xBB8;
    p[0x130 / 4] = 0x7D0;
    p[0x134 / 4] = 0xBB8;
    p[0x140 / 4] = 0x9C4;
    p[0x144 / 4] = 0xBB8;
    p[0x208 / 4] = 0x3E8;
    p[0x20C / 4] = 0xBB8;
    p[0x218 / 4] = 0x7D0;
    p[0x21C / 4] = 0xBB8;
    p[0x228 / 4] = 0x9C4;
    p[0x22C / 4] = 0xBB8;
    p[0x210 / 4] = 0x3E8;
    p[0x214 / 4] = 0xBB8;
    p[0x220 / 4] = 0x7D0;
    p[0x224 / 4] = 0xBB8;
    p[0x230 / 4] = 0x9C4;
    p[0x234 / 4] = 0xBB8;
    p[0x1D8 / 4] = 0x3E8;
    p[0x1DC / 4] = 0xBB8;
    p[0x1E8 / 4] = 0x7D0;
    p[0x1EC / 4] = 0xBB8;
    p[0x1F8 / 4] = 0x9C4;
    p[0x1FC / 4] = 0xBB8;
    p[0x1E0 / 4] = 0x3E8;
    p[0x1E4 / 4] = 0xBB8;
    p[0x1F0 / 4] = 0x7D0;
    p[0x1F4 / 4] = 0xBB8;
    p[0x200 / 4] = 0x9C4;
    p[0x204 / 4] = 0xBB8;
    p[0x238 / 4] = 0xBB8;
    p[0x23C / 4] = 0xFA0;
    p[0x240 / 4] = 0xBB8;
    p[0x244 / 4] = 0xFA0;
    p[0x248 / 4] = 0xBB8;
    p[0x24C / 4] = 0xFA0;
    p[0x250 / 4] = 0xBB8;
    p[0x254 / 4] = 0xFA0;
    p[0x258 / 4] = 0xBB8;
    p[0x25C / 4] = 0xFA0;
    p[0x260 / 4] = 0xBB8;
    p[0x264 / 4] = 0xFA0;
    p[0x268 / 4] = 0xBB8;
    p[0x26C / 4] = 0xFA0;
    p[0x270 / 4] = 0xBB8;
    p[0x274 / 4] = 0xFA0;
    p[0x278 / 4] = 0xBB8;
    p[0x27C / 4] = 0xFA0;
    p[0x280 / 4] = 0xBB8;
    p[0x284 / 4] = 0xFA0;
    p[0x288 / 4] = 0x1388;
    p[0x28C / 4] = 0x1388;
    p[0x290 / 4] = 0x1388;
    p[0x294 / 4] = 0x1388;
    p[0x298 / 4] = 0x1388;
    p[0x29C / 4] = 0x1388;
    p[0x2A0 / 4] = 0x1388;
    p[0x2A4 / 4] = 0x1388;
}
#endif


//100%
INCLUDE_ASM("be/beintstat", func_0014A078);
#ifdef SKIP_ASM
extern "C" void func_0014A078(void) {

}
#endif
