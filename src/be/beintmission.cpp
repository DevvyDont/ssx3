#include "common.h"
#include "main/ssxapp.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BEMissionInt[];
extern void* D_0045ADF8[16];
extern void* cBEMissionInterface_instance;

struct cBEMissionInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//99.58%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the mission interface singleton, creating it on first use ("BEMissionInt").
 *
 * @return the cBEMissionInterface instance
 */
void* cBEMissionInterface_getThis()
{
    if (cBEMissionInterface_instance == 0) {
        cBEMissionInterface* mem = (cBEMissionInterface*)cMemMan_alloc(20, str_BEMissionInt, 0, 0);
        mem->field_0x8 = 0;
        mem->vtable = D_0045ADF8;
        cBEMissionInterface_instance = mem;
    }
    return cBEMissionInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001532F0);
#ifdef SKIP_ASM
extern signed char D_0043FA70[];

extern "C" int func_001532F0(void* self, int start)
{
    int sum = 0;
    for (int i = 0; i < 17; i++)
    {
        sum += D_0043FA70[start + i];
    }
    return sum;
}
#endif
//100%
INCLUDE_ASM("be/beintmission", func_00153330);
#ifdef SKIP_ASM
extern signed char D_0043FA70[];
extern int D_005305F0[];

extern "C" int func_00153330(void* self, int start)
{
    return D_0043FA70[start + D_005305F0[0]];
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153350);
#ifdef SKIP_ASM
extern signed char D_0043FA70[];

extern "C" int func_00153350(void* self, int start)
{
    int sum = 0;
    for (int i = 0; i < 1; i++)
    {
        sum += D_0043FA70[start + i];
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getTotalCollectForPeak);
#ifdef SKIP_ASM
struct sMissionEntry_0043D950
{
    char pad_0x00[0x54];
    int owner;          // 0x54
    char pad_0x58[0xC];
};

extern sMissionEntry_0043D950 cBELibrary_locationDB[];
extern signed char D_0043FA70[];

/**
 * Counts the collectibles that exist across all locations of one peak.
 *
 * Sums the per-location collectible counts (D_0043FA70, On Tour cBELibrary::secCollectibles) over the 22
 * entries of the location table (cBELibrary_locationDB) whose peak is @p owner.
 *
 * @param owner  the peak (On Tour eBEPeakID)
 * @return the total number of collectibles on the peak
 */
extern "C" int cBEMissionInterface_getTotalCollectForPeak(void* self, int owner)
{
    int sum = 0;
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (cBELibrary_locationDB[j].owner == owner)
            {
                sum += D_0043FA70[i + j];
            }
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001533F8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern char D_004A6CA8_raw[] __asm__("cBELibrary_profileLib");

extern "C" int func_001533F8(void* self, int rider, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    int sum = 0;
    int off = idx * 12 + c * 3976 + profile * 39760;
    char* p = D_004A6CA8_raw + off;
    for (int i = 0; i < 17; i++)
    {
        sum += *(signed char*)(p + 4);
        p += 12;
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153498);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern int D_005305F0[];
// Byte view of the profile table; the unit declares cBELibrary_profileLib later with a struct type.
extern char D_004A6CA8_raw[] __asm__("cBELibrary_profileLib");

extern "C" int func_00153498(void* self, int rider, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    int off = (idx + D_005305F0[0]) * 12 + c * 3976 + profile * 39760;
    char* p = D_004A6CA8_raw + off;
    return *(signed char*)(p + 4);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153520);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
// Byte view of the profile table; the unit declares cBELibrary_profileLib later with a struct type.
extern char D_004A6CA8_raw[] __asm__("cBELibrary_profileLib");

extern "C" int func_00153520(void* self, int rider, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    int sum = 0;
    int off = idx * 12 + c * 3976 + profile * 39760;
    char* p = D_004A6CA8_raw + off;
    for (int i = 0; i < 1; i++)
    {
        sum += *(signed char*)(p + i * 12 + 4);
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getCurrentCollectForPeak);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

struct sEconRecord_001535C0
{
    int id;
    signed char count; // 0x4
    char pad_0x05[0x7];
};

// One character's economy block (0xF88 bytes); 10 per profile (0x9B50).
struct sEconChar_001535C0
{
    sEconRecord_001535C0 records[22];
    char pad[0xF88 - 22 * 12];
};

extern sMissionEntry_0043D950 cBELibrary_locationDB[];
extern sEconChar_001535C0 D_004A6CA8_econ[][10] __asm__("cBELibrary_profileLib");

/**
 * Counts the collectibles a rider's character has picked up across all locations of one peak.
 *
 * Looks up the rider's profile and character, then sums that character's collected count for each of the 22
 * locations whose peak is @p owner (cBELibrary_profileLib records).
 *
 * @param rider  the rider whose profile and character are used
 * @param owner  the peak (On Tour eBEPeakID)
 * @return the number of collectibles collected on the peak
 */
extern "C" int cBEMissionInterface_getCurrentCollectForPeak(void* self, int rider, int owner)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int character = cBELibrary_getCharacterID(rider);
    int sum = 0;
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (cBELibrary_locationDB[j].owner == owner)
            {
                sum += D_004A6CA8_econ[profile][character].records[i + j].count;
            }
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153688);
#ifdef SKIP_ASM
struct sEconRecord_00153688
{
    int id;
    signed char count; // 0x4
    char pad_0x05[0x7];
};

// One character's economy block (0xF88 bytes); 10 per profile (0x9B50).
struct sEconChar_00153688
{
    sEconRecord_00153688 records[22];
    char pad[0xF88 - 22 * 12];
};

extern sMissionEntry_0043D950 cBELibrary_locationDB[];
extern sEconChar_00153688 cBELibrary_profileLib[][10];

extern "C" int func_00153688(void* self, int profile, int character, int owner)
{
    int sum = 0;
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (cBELibrary_locationDB[j].owner == owner)
            {
                sum += cBELibrary_profileLib[profile][character].records[i + j].count;
            }
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getCurrentCollectForPeakByChar);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getRiderIndex(int);
extern "C" int cBEMissionInterface_getCurrentCollectForPeak(void* self, int rider, int owner);
extern char D_004A6CAC[];

// View of cBELibrary_locationDB[].owner (offset 0x54); splat labels that address D_0043D9A4.
struct sMissionOwnerView_00153708
{
    int owner;
    char pad_0x04[0x60];
};
extern sMissionOwnerView_00153708 D_0043D9A4_v153708[] __asm__("D_0043D9A4");

/**
 * Counts the collectibles a profile's character has found on a peak.
 *
 * In a game it asks getCurrentCollectForPeak() for the profile's rider. Otherwise it counts the set bits (64
 * per
 * set) of the 22 collectible sets belonging to the peak in the saved profile data.
 *
 * @param profile  profile index (On Tour iProfileIndex)
 * @param character  character ID (On Tour iCharacterID)
 * @param owner  the peak (On Tour eBEPeakID peak)
 * @return the number of collectibles found
 */
extern "C" int cBEMissionInterface_getCurrentCollectForPeakByChar(void* self, int profile, int character, int owner)
{
    if ((void*)SSXApp->Game != 0)
        return cBEMissionInterface_getCurrentCollectForPeak(self, cBELibrary_getRiderIndex(profile), owner);
    int sum = 0;
    int off = profile * 39760 + character * 3976;
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (D_0043D9A4_v153708[j].owner == owner)
            {
                char* collectEntry = D_004A6CAC + (i * 12 + (j * 12 + off));
                char* collectBits = collectEntry + 4;
                int n = 0;
                for (int bitIndex = 0; bitIndex < 64; bitIndex++)
                {
                    unsigned int w = *(unsigned int*)(collectBits + ((bitIndex / 32) << 2));
                    if (w & (1 << (bitIndex % 32)))
                        n++;
                }
                sum += n;
            }
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153858);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern int D_005305F0[];
extern char D_004A6CA8_raw[] __asm__("cBELibrary_profileLib");

extern "C" char* func_00153858(void* self, int rider, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    char* b = D_004A6CA8_raw + c * 3976;
    // PORT: pointer held as int (index-first addu)
    char* p = (char*)(profile * 0x9B50 + (int)b);
    char* q = p + (D_005305F0[0] * 12 + 4);
    return q + idx * 12 + 4;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001538E8);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern int D_005305F0[];
extern char D_004A6CAC[];

extern "C" int func_001538E8(void* self, int rider, int bit, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    int off = profile * 39760 + c * 3976 + D_005305F0[0] * 12;
    char* p = D_004A6CAC + (idx * 12 + off);
    p = p + (bit / 32) * 4;
    int mask = 1 << bit;
    return (*(unsigned int*)(p + 4) & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001539A8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

struct sBits8_1539A8 {
    unsigned int w[2];
};
struct sCfg_1539A8 {
    char pad[0x40];
    int base;           // 0x40
};
extern sCfg_1539A8 cBELibrary_raceDB;
extern char D_004A6CB0_raw[] __asm__("D_004A6CB0");
extern char D_004A6CA8_raw[] __asm__("cBELibrary_profileLib");

extern "C" void func_001539A8(void* self, int rider, int idx, sBits8_1539A8* src)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    int bb = cBELibrary_raceDB.base * 12;
    *(sBits8_1539A8*)(D_004A6CB0_raw + (idx * 12 + (c * 0xF88 + profile * 0x9B50 + bb))) = *src;
    int count = 0;
    for (int i = 0; i < 64; i++) {
        int off = i / 32 * 4;
        off += idx * 12;
        off += cBELibrary_raceDB.base * 12;
        off += c * 3976;
        off += profile * 39760;
        if (*(unsigned int*)(D_004A6CB0_raw + off) & (1 << i))
            count++;
    }
    char* p = D_004A6CA8_raw + ((idx + cBELibrary_raceDB.base) * 12 + c * 3976 + profile * 39760);
    *(signed char*)(p + 4) = count;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153B00);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern int D_005305B0_3B00[] __asm__("cBELibrary_raceDB");

struct sEconRecord_00153B00
{
    int id;
    signed char count; // 0x4
    char pad_0x05[0x7];
};

// One character's economy block (0xF88 bytes); 10 per profile (0x9B50).
struct sEconChar_00153B00
{
    sEconRecord_00153B00 records[22];
    char pad[0xF88 - 22 * 12];
};
extern sEconChar_00153B00 D_004A6CA8_econ153B00[][10] __asm__("cBELibrary_profileLib");

// Bit-set view of the same records, based at D_004A6CAC (record + 4).
struct sEconBits_00153B00
{
    int pad;
    unsigned int bits[2];
};
struct sEconBitsChar_00153B00
{
    sEconBits_00153B00 recs[22][1];
    char pad[0xF88 - 22 * 12];
};
extern sEconBitsChar_00153B00 D_004A6CAC_bits153B00[][10] __asm__("D_004A6CAC");

extern "C" void func_00153B00(void* self, int rider, int bit, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int c = cBELibrary_getCharacterID(rider);
    int off = profile * 39760 + c * 3976 + D_005305B0_3B00[16] * 12;
    // PORT: address accumulated in an int (base symbol unknown to alias analysis).
    int addr = idx * 12 + off;
    addr += (int)D_004A6CAC_bits153B00;
    addr += 4;
    addr += (bit / 32) * 4;
    *(unsigned int*)addr |= 1 << bit;
    int n = 0;
    for (int b = 0; b < 64; b++)
    {
        if (D_004A6CAC_bits153B00[profile][c].recs[D_005305B0_3B00[16]][idx].bits[b / 32] & (1 << b))
            n++;
    }
    D_004A6CA8_econ153B00[profile][c].records[idx + D_005305B0_3B00[16]].count = n;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getMissionName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMissionDefView_00153C88
{
    int id;
    int f4;
    char pad_0x08[0x1C];
};
extern sMissionDefView_00153C88 D_0043EE10_v00153C88[] __asm__("cBELibrary_missionInfo");
extern "C" int func_00154240(void* self, int id);

class cMissionObj_00153C88
{
public:
    char pad_0x00[0x4];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(int a);
};

/**
 * Looks up a mission's name in the front-end locale.
 *
 * Declared void here, but On Tour returns the unsigned-short string; the locale call's result is left in $v0.
 *
 * @param id  mission ID (On Tour uiMissionId)
 */
extern "C" void cBEMissionInterface_getMissionName(void* self, int id)
{
    int i = func_00154240(self, id);
    ((cMissionObj_00153C88*)(void*)SSXApp->FELocale)->v4(D_0043EE10_v00153C88[i].f4);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153CD8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMissionDefView_00153CD8
{
    int id;
    int f4;
    int f8;
    char pad_0x0C[0x18];
};
extern sMissionDefView_00153CD8 D_0043EE10_v00153CD8[] __asm__("cBELibrary_missionInfo");
extern "C" int func_00154240(void* self, int id);

class cMissionObj_00153CD8
{
public:
    char pad_0x00[0x4];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(int a);
};

extern "C" void func_00153CD8(void* self, int id)
{
    int i = func_00154240(self, id);
    ((cMissionObj_00153CD8*)(void*)SSXApp->FELocale)->v4(D_0043EE10_v00153CD8[i].f8);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getMissionLongDesc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMissionDefView_00153D28
{
    int id;
    int f4;
    int f8;
    int fC;
    char pad_0x10[0x14];
};
extern sMissionDefView_00153D28 D_0043EE10_v00153D28[] __asm__("cBELibrary_missionInfo");
extern "C" int func_00154240(void* self, int id);

class cMissionObj_00153D28
{
public:
    char pad_0x00[0x4];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(int a);
};

/**
 * Looks up a mission's long description in the front-end locale.
 *
 * Declared void here, but On Tour returns the unsigned-short string; the locale call's result is left in $v0.
 *
 * @param id  mission ID (On Tour uiMissionId)
 */
extern "C" void cBEMissionInterface_getMissionLongDesc(void* self, int id)
{
    int i = func_00154240(self, id);
    ((cMissionObj_00153D28*)(void*)SSXApp->FELocale)->v4(D_0043EE10_v00153D28[i].fC);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_isMissionCompleted);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int func_00154240(void* self, int id);

/**
 * Tells whether rider 0's character has completed a mission (bit 3 of its saved mission flags in profile 0).
 *
 * @param id  mission ID (On Tour uiMissionId)
 * @return 1 when completed, otherwise 0
 */
extern "C" int cBEMissionInterface_isMissionCompleted(void* self, int id)
{
    int i = func_00154240(self, id);
    int characterId = cBELibrary_getCharacterID(0);
    int off = (i << 2) + characterId * 3976;
    char* profile = (char*)cBELibrary_profileLib + off;
    return (*(unsigned int*)(profile + 0x118) >> 3) & 1;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153DD0);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int func_00154240(void* self, int id);

extern "C" int func_00153DD0(void* self, int id)
{
    int i = func_00154240(self, id);
    int charId = cBELibrary_getCharacterID(0);
    int off = (i << 2) + charId * 3976;
    char* recordBase = (char*)cBELibrary_profileLib + off;
    return (*(unsigned int*)(recordBase + 0x118) >> 1) & 1;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_isMissionLocked);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int func_00154240(void* self, int id);

/**
 * Tells whether a mission is still locked for the player.
 *
 * Index from func_00154240 (On Tour getMissionIdx); the flag word is at +0x118 + index * 4 of the profile of rider 0's
 * character in cBELibrary_profileLib (profile 0).
 *
 * @param id  the mission id (On Tour uiMissionId)
 * @return 1 when locked, 0 when open
 */
extern "C" int cBEMissionInterface_isMissionLocked(void* self, int id)
{
    int i = func_00154240(self, id);
    int characterId = cBELibrary_getCharacterID(0);
    int off = (i << 2) + characterId * 3976;
    char* profile = (char*)cBELibrary_profileLib + off;
    return (*(unsigned int*)(profile + 0x118)) & 1;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_isMissionAvailable);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int func_00154240(void* self, int id);

/**
 * Tells whether a mission is available to rider 0's character (bit 2 of its saved mission flags in profile
 * 0).
 *
 * @param id  mission ID (On Tour uiMissionId)
 * @return 1 when available, otherwise 0
 */
extern "C" int cBEMissionInterface_isMissionAvailable(void* self, int id)
{
    int i = func_00154240(self, id);
    int characterId = cBELibrary_getCharacterID(0);
    int off = (i << 2) + characterId * 3976;
    char* profile = (char*)cBELibrary_profileLib + off;
    return (*(unsigned int*)(profile + 0x118) >> 2) & 1;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153ED8);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int func_00154240(void* self, int id);

extern "C" int func_00153ED8(void* self, int id)
{
    int i = func_00154240(self, id);
    int charId = cBELibrary_getCharacterID(0);
    int off = (i << 2) + charId * 3976;
    char* recordBase = (char*)cBELibrary_profileLib + off;
    return (*(unsigned int*)(recordBase + 0x118) >> 4) & 1;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153F30);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMissionDefView_00153F30
{
    int id;
    char pad_0x04[0xC];
    int f10;
    int f14;
    char pad_0x18[0xC];
};
extern sMissionDefView_00153F30 D_0043EE10_v00153F30[] __asm__("cBELibrary_missionInfo");
extern "C" int func_00154240(void* self, int id);

extern "C" int func_00153F30(void* self, int id)
{
    int i = func_00154240(self, id);
    return D_0043EE10_v00153F30[i].f10;
}
#endif
//100%
INCLUDE_ASM("be/beintmission", func_00153F68);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sMissionDefView_00153F68
{
    int id;
    char pad_0x04[0xC];
    int f10;
    int f14;
    char pad_0x18[0xC];
};
extern sMissionDefView_00153F68 D_0043EE10_v00153F68[] __asm__("cBELibrary_missionInfo");
extern "C" int func_00154240(void* self, int id);

extern "C" int func_00153F68(void* self, int id)
{
    int i = func_00154240(self, id);
    return D_0043EE10_v00153F68[i].f14;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00153FA0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's sMissionDef_0043EE10 is defined later (with func_00154240) and lacks
// the fields at 0x18/0x1C; this local view of the same table binds to cBELibrary_missionInfo.
struct sMissionDefView_0043EE10
{
    int id;             // 0x00
    char pad_0x04[0xC];
    int group;          // 0x10
    char pad_0x14[0x4];
    short field_0x18;   // 0x18
    char pad_0x1A[0x2];
    int field_0x1C;     // 0x1C
    char pad_0x20[0x2];
    short value;        // 0x22
};
extern sMissionDefView_0043EE10 D_0043EE10_view[] __asm__("cBELibrary_missionInfo");

extern "C" int func_00154240(void* self, int id);

extern "C" short func_00153FA0(void* self, int id)
{
    int i = func_00154240(self, id);
    return D_0043EE10_view[i].field_0x18;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNextMissionID);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// The unit's sMissionDef_0043EE10 is defined later (with func_00154240) and lacks
// the fields at 0x18/0x1C; this local view of the same table binds to cBELibrary_missionInfo.
extern sMissionDefView_0043EE10 D_0043EE10_view[] __asm__("cBELibrary_missionInfo");

extern "C" int func_00154240(void* self, int id);

/**
 * Returns the ID of the mission that follows a given one.
 *
 * Finds the mission's index with func_00154240() (On Tour getMissionIdx, which asserts "Mission id not found")
 * and reads the next-mission field (+0x1C) of its mission table entry (cBELibrary_missionInfo, On Tour
 * cBELibrary::missionInfo).
 *
 * @param id  the current mission's ID
 * @return the next mission's ID
 */
extern "C" int cBEMissionInterface_getNextMissionID(void* self, int id)
{
    int i = func_00154240(self, id);
    return D_0043EE10_view[i].field_0x1C;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00154010);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int func_00154240(void* self, int id);

extern "C" void func_00154010(void* self, int id, int value)
{
    int i = func_00154240(self, id);
    int charId = cBELibrary_getCharacterID(0);
    int off = (i << 2) + charId * 3976;
    char* recordBase = (char*)cBELibrary_profileLib + off;
    *(int*)(recordBase + 0x118) = (*(int*)(recordBase + 0x118) & ~1) | (value & 1);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00154080);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int func_00154240(void* self, int id);

extern "C" void func_00154080(void* self, int id, int value)
{
    int i = func_00154240(self, id);
    int charId = cBELibrary_getCharacterID(0);
    int off = (i << 2) + charId * 3976;
    char* recordBase = (char*)cBELibrary_profileLib + off;
    *(int*)(recordBase + 0x118) = (*(int*)(recordBase + 0x118) & ~(1 << 1)) | ((value & 1) << 1);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001540F0);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int func_00154240(void* self, int id);

extern "C" void func_001540F0(void* self, int id, int value)
{
    int i = func_00154240(self, id);
    int charId = cBELibrary_getCharacterID(0);
    int off = (i << 2) + charId * 3976;
    char* recordBase = (char*)cBELibrary_profileLib + off;
    *(int*)(recordBase + 0x118) = (*(int*)(recordBase + 0x118) & ~(1 << 2)) | ((value & 1) << 2);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00154160);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int func_00154240(void* self, int id);

extern "C" void func_00154160(void* self, int id, int value)
{
    int i = func_00154240(self, id);
    int charId = cBELibrary_getCharacterID(0);
    int off = (i << 2) + charId * 3976;
    char* recordBase = (char*)cBELibrary_profileLib + off;
    *(int*)(recordBase + 0x118) = (*(int*)(recordBase + 0x118) & ~(1 << 3)) | ((value & 1) << 3);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001541D0);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int func_00154240(void* self, int id);

extern "C" void func_001541D0(void* self, int id, int value)
{
    int i = func_00154240(self, id);
    int charId = cBELibrary_getCharacterID(0);
    int off = (i << 2) + charId * 3976;
    char* recordBase = (char*)cBELibrary_profileLib + off;
    *(int*)(recordBase + 0x118) = (*(int*)(recordBase + 0x118) & ~(1 << 4)) | ((value & 1) << 4);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00154240);
#ifdef SKIP_ASM
struct sMissionDef_0043EE10
{
    int id;             // 0x00
    char pad_0x04[0xC];
    int group;          // 0x10
    char pad_0x14[0xE];
    short value;        // 0x22
};

extern sMissionDef_0043EE10 cBELibrary_missionInfo[];

extern "C" int func_00154240(void* self, int id)
{
    for (int i = 0; i < 88; i++) {
        if (cBELibrary_missionInfo[i].id == id) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNumberMissions);
#ifdef SKIP_ASM
extern "C" int cBEMissionInterface_getNumberMissionsOnTrack(void* self, int group);
extern int D_005305F0[];

/**
 * Counts the missions on the current race's location.
 *
 * @return cBEMissionInterface_getNumberMissionsOnTrack() for the race's location (D_005305F0, iLocationIdx of
 *         the cBERaceDB copy at cBELibrary_raceDB)
 */
extern "C" int cBEMissionInterface_getNumberMissions(void* self)
{
    return cBEMissionInterface_getNumberMissionsOnTrack(self, D_005305F0[0]);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNumberMissionsOnTrack);
#ifdef SKIP_ASM
extern sMissionDef_0043EE10 cBELibrary_missionInfo[];

/**
 * Counts the missions set on one location (track).
 *
 * @param group  the location (On Tour eBELocationID), compared with each of the 88 mission table entries
 * @return the number of missions on that location
 */
extern "C" int cBEMissionInterface_getNumberMissionsOnTrack(void* self, int group)
{
    int n = 0;
    for (int i = 0; i < 88; i++) {
        if (cBELibrary_missionInfo[i].group == group) {
            n++;
        }
    }
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintmission", func_001542E0);
#ifdef SKIP_ASM
extern "C" int cBEMissionInterface_isMissionCompleted(void* self, int id);

extern "C" int func_001542E0(void* self, int group)
{
    int n = 0;
    for (int i = 0; i < 88; i++)
    {
        if (cBELibrary_missionInfo[i].group == group)
        {
            if (cBEMissionInterface_isMissionCompleted(self, cBELibrary_missionInfo[i].id))
                n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNumberMissionsOnPeak);
#ifdef SKIP_ASM
// One 0x24-byte entry of the mission table at cBELibrary_missionInfo.
struct sMissionDef_00154368
{
    int id;             // 0x00
    char pad_0x04[0xC];
    int group;          // 0x10
    char pad_0x14[0x10];
};

extern sMissionEntry_0043D950 cBELibrary_locationDB[];
extern sMissionDef_00154368 D_0043EE10_00154368[] __asm__("cBELibrary_missionInfo");

/**
 * Counts the missions set on any location of one peak.
 *
 * @param owner  the peak (On Tour eBEPeakID)
 * @return the number of missions whose location belongs to the peak
 */
extern "C" int cBEMissionInterface_getNumberMissionsOnPeak(void* self, int owner)
{
    int n = 0;
    for (int i = 0; i < 88; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (cBELibrary_locationDB[j].owner == owner)
            {
                if (D_0043EE10_00154368[i].group == j)
                {
                    n++;
                }
            }
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNumberCompletedMissionsOnPeak);
#ifdef SKIP_ASM
/**
 * Counts the completed missions on one peak for the current game.
 *
 * Counts the missions whose location belongs to the peak and that cBEMissionInterface_isMissionCompleted()
 * reports as done.
 *
 * @param owner  the peak (On Tour eBEPeakID)
 * @return the number of completed missions on the peak
 */
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Counts missions of an owner's groups that cBEMissionInterface_isMissionCompleted reports as done.
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeak(void* self, int owner)
{
    int n = 0;
    for (int i = 0; i < 88; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (cBELibrary_missionInfo[i].group == j)
            {
                if (cBELibrary_locationDB[j].owner == owner)
                {
                    if (cBEMissionInterface_isMissionCompleted(self, cBELibrary_missionInfo[i].id))
                        n++;
                }
            }
        }
    }
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNumberCompletedMissionsOnPeakByChar);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeak(void* self, int owner);

// View of cBELibrary_locationDB[].owner (offset 0x54); splat labels that address D_0043D9A4.
struct sMissionOwnerView_001544D0
{
    int owner;
    char pad_0x04[0x60];
};
extern sMissionOwnerView_001544D0 D_0043D9A4[];

// One character's economy block (0xF88 bytes); mission flag words start at 0x118.
struct sEconFlags_001544D0
{
    char pad_0x00[0x118];
    unsigned int flags[88];
    char pad_0x278[0xF88 - 0x118 - 88 * 4];
};
extern sEconFlags_001544D0 D_004A6CA8_flags[][10] __asm__("cBELibrary_profileLib");

/**
 * Counts the missions one profile's character has completed on a peak.
 *
 * While a game is running (SSXApp->Game set) it returns cBEMissionInterface_getNumberCompletedMissionsOnPeak()
 * instead. Otherwise it counts bit 3 of the character's mission flag words in cBELibrary_profileLib for each
 * mission whose location belongs to the peak.
 *
 * @param profile    profile index
 * @param character  character ID within the profile
 * @param owner      the peak (On Tour eBEPeakID)
 * @return the number of completed missions
 */
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeakByChar(void* self, int profile, int character, int owner)
{
    if ((void*)SSXApp->Game != 0)
    {
        return cBEMissionInterface_getNumberCompletedMissionsOnPeak(self, owner);
    }
    int n = 0;
    for (int i = 0; i < 88; i++)
    {
        if (D_0043D9A4[cBELibrary_missionInfo[i].group].owner == owner)
        {
            n += (D_004A6CA8_flags[profile][character].flags[i] >> 3) & 1;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getMissionID);
#ifdef SKIP_ASM
extern "C" int cBEMissionInterface_getMissionIDOnTrack(void* self, int a, int group);
extern int D_005305F0[];

/**
 * Returns the id of the n-th mission at the current location.
 *
 * cBEMissionInterface_getMissionIDOnTrack (getMissionIDOnTrack) with the current location id from D_005305F0.
 *
 * @param iIdx  the mission's index at this location (On Tour iIdx)
 * @return the mission id, 0 when there are not that many
 */
extern "C" int cBEMissionInterface_getMissionID(void* self, int iIdx)
{
    return cBEMissionInterface_getMissionIDOnTrack(self, iIdx, D_005305F0[0]);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getMissionIDOnTrack);
#ifdef SKIP_ASM
extern sMissionDef_0043EE10 cBELibrary_missionInfo[];

/**
 * Returns the id of the n-th mission at a given location.
 *
 * Counts entries of cBELibrary_missionInfo (88) whose location (group) is @p group; release returns 0 where On Tour
 * asserts 'Mission index invalid'.
 *
 * @param index  the mission's index at that location (On Tour iIdx)
 * @param group  the location (On Tour eBELocationID locID)
 * @return the mission id, or 0
 */
extern "C" int cBEMissionInterface_getMissionIDOnTrack(void* self, int index, int group)
{
    int n = 0;
    for (int i = 0; i < 88; i++) {
        if (cBELibrary_missionInfo[i].group == group) {
            if (n == index) {
                return cBELibrary_missionInfo[i].id;
            }
            n++;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNumberSessionsOnTrack);
#ifdef SKIP_ASM
struct sMissionEntry {
    int id;              // 0x00
    char pad_0x04[0x14];
    int value;           // 0x18
    char pad_0x1c[0x10];
};

extern sMissionEntry cBELibrary_pictureDB[];

/**
 * Looks up the number of sessions for a track id in the 22-entry cBELibrary_pictureDB table.
 *
 * The table's name (picture DB) does not fit a session count, so either it or this function is likely misnamed.
 *
 * @param id  track id to find
 * @return the value of the entry with that id, or 0 if there is none
 */
extern "C" int cBEMissionInterface_getNumberSessionsOnTrack(void* self, int id)
{
    for (int i = 0; i < 22; i++) {
        if (cBELibrary_pictureDB[i].id == id) {
            return cBELibrary_pictureDB[i].value;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_isSessionPoint);
#ifdef SKIP_ASM
/**
 * Tells whether a location has a session point with at least the given index.
 *
 * True when any of the 22 cBELibrary_pictureDB records has id @p id and value >= @p value.
 *
 * @param value  the session point index (On Tour iIdx)
 * @param id  the location (On Tour eBELocationID locID)
 * @return 1 when such a session point exists
 */
extern "C" int cBEMissionInterface_isSessionPoint(void* self, int value, int id)
{
    for (int i = 0; i < 22; i++) {
        if (cBELibrary_pictureDB[i].id == id && cBELibrary_pictureDB[i].value >= value) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getPictureDBRecord);
#ifdef SKIP_ASM
/**
 * Returns a copy of a location's picture database record.
 *
 * First of the 22 cBELibrary_pictureDB records with id @p id; record 0 when none matches.
 *
 * @param id  the location (On Tour eBELocationID locID)
 * @return the record (by value)
 */
extern "C" sMissionEntry cBEMissionInterface_getPictureDBRecord(void* self, int id)
{
    for (int i = 0; i < 22; i++)
    {
        if (cBELibrary_pictureDB[i].id == id)
        {
            return cBELibrary_pictureDB[i];
        }
    }
    return cBELibrary_pictureDB[0];
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getPictureFileName);
#ifdef SKIP_ASM
/**
 * Returns the picture file name for a location from the picture table (cBELibrary_pictureDB, 22 entries).
 *
 * @param id  the location ID (On Tour eBELocationID)
 * @return the file name stored after the matching entry's ID, or 0 when no entry matches
 */
extern "C" void* cBEMissionInterface_getPictureFileName(void* self, int id)
{
    for (int i = 0; i < 22; i++) {
        if (cBELibrary_pictureDB[i].id == id) {
            return cBELibrary_pictureDB[i].pad_0x04;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001547A0);
#ifdef SKIP_ASM
extern sMissionDef_0043EE10 cBELibrary_missionInfo[];

extern "C" int func_001547A0(void* self, int id)
{
    for (int i = 0; i < 88; i++) {
        if (cBELibrary_missionInfo[i].id == id) {
            return cBELibrary_missionInfo[i].value;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001547D8__FPv);
#ifdef SKIP_ASM
void func_001547D8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001547E0__FPv);
#ifdef SKIP_ASM
void func_001547E0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_001547E8);
#ifdef SKIP_ASM
struct sSerVEntry_001547E8 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern char D_004A6CAC[];

extern "C" void func_001547E8(void* self, void* stream)
{
    int c = cBELibrary_getCharacterID(0);
    char* base = D_004A6CAC;
    int off = c * 3976;
    sSerVEntry_001547E8* e = &(*(sSerVEntry_001547E8**)stream)[1];
    e->fn((char*)stream + e->delta, base + off, 276);
    e = &(*(sSerVEntry_001547E8**)stream)[1];
    char* base2 = base + 0x114;
    e->fn((char*)stream + e->delta, base2 + off, 352);
    e = &(*(sSerVEntry_001547E8**)stream)[1];
    e->fn((char*)stream + e->delta, (char*)self + 8, 4);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", func_00154898);
#ifdef SKIP_ASM
struct sSerVEntry_00154898 {
    short delta;
    short index;
    void (*fn)(void*, void*, int);
};

extern char D_004A6CAC[];

extern "C" void func_00154898(void* self, void* stream)
{
    int c = cBELibrary_getCharacterID(0);
    char* base = D_004A6CAC;
    int off = c * 3976;
    sSerVEntry_00154898* e = &(*(sSerVEntry_00154898**)stream)[2];
    e->fn((char*)stream + e->delta, base + off, 276);
    e = &(*(sSerVEntry_00154898**)stream)[2];
    char* base2 = base + 0x114;
    e->fn((char*)stream + e->delta, base2 + off, 352);
    e = &(*(sSerVEntry_00154898**)stream)[2];
    e->fn((char*)stream + e->delta, (char*)self + 8, 4);
}
#endif

