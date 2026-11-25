#include "common.h"
#include "be/beintnewrace.h"
#include "be/bedbprofile.h"
#include "be/beprofilelib.h"
#include "vtable.h"
#include "be/beintmission.h"
#include "replay/frameptr.h"
#include "main/ssxapp.h"
#include "replay/bxreplayframe.h"
#include "util/locale.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BEMissionInt[];
extern void* cBEMissionInterface_vtbl[16];


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
        mem->bIsActive = 0;
        (*(void**)&VPTR_OF(cBEInterface, mem)) = cBEMissionInterface_vtbl;
        cBEMissionInterface_instance = (cBEMissionInterface*)(mem);
    }
    return cBEMissionInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getTotalCollectForMnt);
#ifdef SKIP_ASM
extern signed char cBELibrary_secCollectibles[];

/**
 * Returns the number of collectibles in 17 consecutive sections starting at @p start: the sum of cBELibrary_secCollectibles[@p start]
 * .. cBELibrary_secCollectibles[@p start + 16] (the per-section totals).
 *
 * @param start  the first section
 * @return the total
 */
int cBEMissionInterface::getTotalCollectForMnt(int start)
{
    int sum = 0;
    for (int i = 0; i < 17; i++)
    {
        sum += cBELibrary_secCollectibles[start + i];
    }
    return sum;
}
#endif
//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getTotalCollectByType);
#ifdef SKIP_ASM
extern signed char cBELibrary_secCollectibles[];
extern int D_005305F0[];

/**
 * Returns the collectible total of section @p start + the race's location (cBELibrary_raceDB +0x40, D_005305F0):
 * cBELibrary_secCollectibles[@p start + location].
 *
 * @param start  the offset from the race's location
 * @return the total
 */
int cBEMissionInterface::getTotalCollectByType(int start)
{
    return cBELibrary_secCollectibles[start + D_005305F0[0]];
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getTotalCollectForSection);
#ifdef SKIP_ASM
extern signed char cBELibrary_secCollectibles[];

/**
 * Returns the number of collectibles in location @p start: cBELibrary_secCollectibles[@p start] (a one-iteration sum).
 *
 * @param start  the location
 * @return the total
 */
int cBEMissionInterface::getTotalCollectForSection(int start)
{
    int sum = 0;
    for (int i = 0; i < 1; i++)
    {
        sum += cBELibrary_secCollectibles[start + i];
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
extern signed char cBELibrary_secCollectibles[];

/**
 * Counts the collectibles that exist across all locations of one peak.
 *
 * Sums the per-location collectible counts (cBELibrary_secCollectibles, On Tour cBELibrary::secCollectibles) over the 22
 * entries of the location table (cBELibrary_locationDB) whose peak is @p owner.
 *
 * @param owner  the peak (On Tour eBEPeakID)
 * @return the total number of collectibles on the peak
 */
int cBEMissionInterface::getTotalCollectForPeak(int owner)
{
    int sum = 0;
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (cBELibrary_locationDB[j].owner == owner)
            {
                sum += cBELibrary_secCollectibles[i + j];
            }
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getCurrentCollectForMnt);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns how many collectibles @p rider has found in 17 consecutive sections starting at @p idx.
 *
 * Sums cTotalCollected of the rider's collect records (mCollectibles[section], tBEProfileCollectBits) for sections
 * @p idx .. @p idx + 16.
 *
 * @param rider  the rider (selects profile slot and character)
 * @param idx  the first section
 * @return the count
 */
int cBEMissionInterface::getCurrentCollectForMnt(int rider, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    int sum = 0;
    for (int i = 0; i < 17; i++)
    {
        sum += cBELibrary_profileLib[profile][characterId].mCollectibles[idx + i].cTotalCollected;
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getCurrentCollectByType);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern int D_005305F0[];

/**
 * Returns how many collectibles @p rider has found in section @p idx + the race's location (D_005305F0): the count
 * (cTotalCollected) of that collect record.
 *
 * @param rider  the rider (selects profile slot and character)
 * @param idx  the offset from the race's location
 * @return the count
 */
int cBEMissionInterface::getCurrentCollectByType(int rider, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    return cBELibrary_profileLib[profile][characterId].mCollectibles[idx + D_005305F0[0]].cTotalCollected;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getCurrentCollectForSection);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

/**
 * Returns how many collectibles @p rider has found in location @p idx: the count (cTotalCollected) of its collect
 * record (a one-iteration sum).
 *
 * @param rider  the rider (selects profile slot and character)
 * @param idx  the location
 * @return the count
 */
int cBEMissionInterface::getCurrentCollectForSection(int rider, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    int sum = 0;
    for (int i = 0; i < 1; i++)
    {
        sum += cBELibrary_profileLib[profile][characterId].mCollectibles[idx + i].cTotalCollected;
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

extern sMissionEntry_0043D950 cBELibrary_locationDB[];

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
int cBEMissionInterface::getCurrentCollectForPeak(int rider, int owner)
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
                sum += cBELibrary_profileLib[profile][character].mCollectibles[i + j].cTotalCollected;
            }
        }
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getCurrentCollectForPeak1);
#ifdef SKIP_ASM
extern sMissionEntry_0043D950 cBELibrary_locationDB[];

/**
 * Returns how many collectibles character @p character of profile slot @p profile has found on peak @p owner.
 *
 * Sums the collect-record counts (cTotalCollected) of the 22 locations whose cBELibrary_locationDB entry has
 * @p owner at +0x54.
 *
 * @param profile  the profile slot
 * @param character  the character
 * @param owner  the peak
 * @return the count
 */
int cBEMissionInterface::getCurrentCollectForPeak1(int profile, int character, int owner)
{
    int sum = 0;
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (cBELibrary_locationDB[j].owner == owner)
            {
                sum += cBELibrary_profileLib[profile][character].mCollectibles[i + j].cTotalCollected;
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
extern "C" int cBEMissionInterface_getCurrentCollectForPeak(cBEMissionInterface* self, int rider, int owner);
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
int cBEMissionInterface::getCurrentCollectForPeakByChar(int profile, int character, int owner)
{
    if ((void*)SSXApp->Game != 0)
        return getCurrentCollectForPeak(cBELibrary_getRiderIndex(profile), owner);
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
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getCollectBits);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern int D_005305F0[];

/**
 * Returns the address of the found-collectible bits (two words) of @p rider's collect record for section @p idx + the
 * race's location (D_005305F0).
 *
 * @param rider  the rider (selects profile slot and character)
 * @param idx  the offset from the race's location
 * @return the bit words
 */
char* cBEMissionInterface::getCollectBits(int rider, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    tBEProfileCollectBits* currentRecord = &cBELibrary_profileLib[profile][characterId].mCollectibles[D_005305F0[0]];
    return (char*)currentRecord[idx].uiCollectedBits;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_isBitSet);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern int D_005305F0[];
extern char D_004A6CAC[];

/**
 * Tells whether collectible @p bit (0-63) is marked found in @p rider's collect record for section @p idx + the race's
 * location (D_005305F0).
 *
 * @param rider  the rider (selects profile slot and character)
 * @param bit  the collectible's bit, 0-63
 * @param idx  the offset from the race's location
 * @return 1 if found, else 0
 */
int cBEMissionInterface::isBitSet(int rider, int bit, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    int off = profile * 39760 + characterId * 3976 + D_005305F0[0] * 12;
    char* recordCursor = D_004A6CAC + (idx * 12 + off);
    recordCursor = recordCursor + (bit / 32) * 4;
    int mask = 1 << bit;
    return (*(unsigned int*)(recordCursor + 4) & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_setBits);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

struct sBits8_1539A8 {
    unsigned int w[2];
};
extern cBERaceDB cBELibrary_raceDB;
extern char D_004A6CB0_raw[] __asm__("D_004A6CB0");

/**
 * Replaces the found-collectible bits of @p rider's collect record for section @p idx + the race's location
 * (cBELibrary_raceDB +0x40) with the two words at @p src, and sets the record's count to the number of set bits.
 *
 * @param rider  the rider (selects profile slot and character)
 * @param idx  the offset from the race's location
 * @param src  the new bit words
 */
extern "C" void cBEMissionInterface_setBits(cBEMissionInterface* self, int rider, int idx, sBits8_1539A8* src)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    int bb = cBELibrary_raceDB.iLocationIdx * 12;
    *(sBits8_1539A8*)(D_004A6CB0_raw + (idx * 12 + (characterId * 0xF88 + profile * 0x9B50 + bb))) = *src;
    int count = 0;
    for (int i = 0; i < 64; i++) {
        int off = i / 32 * 4;
        off += idx * 12;
        off += cBELibrary_raceDB.iLocationIdx * 12;
        off += characterId * 3976;
        off += profile * 39760;
        if (*(unsigned int*)(D_004A6CB0_raw + off) & (1 << i))
            count++;
    }
    int loc = idx + cBELibrary_raceDB.iLocationIdx;
    cBELibrary_profileLib[profile][characterId].mCollectibles[loc].cTotalCollected = count;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_setBit);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);
extern int cBELibrary_raceDB_3B00[] __asm__("cBELibrary_raceDB");

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

/**
 * Marks collectible @p bit found in @p rider's collect record for section @p idx + the race's location
 * (cBELibrary_raceDB +0x40) and sets the record's count to the number of set bits.
 *
 * @param rider  the rider (selects profile slot and character)
 * @param bit  the collectible's bit, 0-63
 * @param idx  the offset from the race's location
 */
void cBEMissionInterface::setBit(int rider, int bit, int idx)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int characterId = cBELibrary_getCharacterID(rider);
    int off = profile * 39760 + characterId * 3976 + cBELibrary_raceDB_3B00[16] * 12;
    // PORT: address accumulated in an int (base symbol unknown to alias analysis).
    int addr = idx * 12 + off;
    addr += (int)D_004A6CAC_bits153B00;
    addr += 4;
    addr += (bit / 32) * 4;
    *(unsigned int*)addr |= 1 << bit;
    int n = 0;
    for (int scanBit = 0; scanBit < 64; scanBit++)
    {
        if (D_004A6CAC_bits153B00[profile][characterId].recs[cBELibrary_raceDB_3B00[16]][idx].bits[scanBit / 32] & (1 << scanBit))
            n++;
    }
    cBELibrary_profileLib[profile][characterId].mCollectibles[idx + cBELibrary_raceDB_3B00[16]].cTotalCollected = n;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getMissionName);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern tBEMissionInfo cBELibrary_missionInfo[];
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Looks up a mission's name in the front-end locale.
 *
 * Returns the locale's string (unsigned short*, as in On Tour).
 *
 * @param id  mission ID (On Tour uiMissionId)
 * @return the name string from FELocale->getStringByHash, keyed by the mission record's nameHash
 */
unsigned short* cBEMissionInterface::getMissionName(int id)
{
    int i = getMissionIdx(id);
    return SSXApp->FELocale->getStringByHash(cBELibrary_missionInfo[i].nameHash);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getMissionShortDesc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern tBEMissionInfo cBELibrary_missionInfo[];
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Looks up a mission's short description in the front-end locale: SSXApp->FELocale's vtable slot 4 with the text id
 * shortDescHash of the mission's cBELibrary_missionInfo record (index from cBEMissionInterface_getMissionIdx).
 *
 * Returns the locale's string, as cBEMissionInterface_getMissionName does.
 *
 * @param id  the mission id
 * @return the short-description string from FELocale->getStringByHash, keyed by shortDescHash
 */
unsigned short* cBEMissionInterface::getMissionShortDesc(int id)
{
    int i = getMissionIdx(id);
    return SSXApp->FELocale->getStringByHash(cBELibrary_missionInfo[i].shortDescHash);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getMissionLongDesc);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern tBEMissionInfo cBELibrary_missionInfo[];
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Looks up a mission's long description in the front-end locale.
 *
 * Returns the locale's string (unsigned short*, as in On Tour).
 *
 * @param id  mission ID (On Tour uiMissionId)
 * @return the long-description string from FELocale->getStringByHash, keyed by longDescHash
 */
unsigned short* cBEMissionInterface::getMissionLongDesc(int id)
{
    int i = getMissionIdx(id);
    return SSXApp->FELocale->getStringByHash(cBELibrary_missionInfo[i].longDescHash);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_isMissionCompleted);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Tells whether rider 0's character has completed a mission (bIsCompleted of its saved mission flags in profile 0).
 *
 * @param id  mission ID (On Tour uiMissionId)
 * @return 1 when completed, otherwise 0
 */
int cBEMissionInterface::isMissionCompleted(int id)
{
    int i = getMissionIdx(id);
    int characterId = cBELibrary_getCharacterID(0);
    return cBELibrary_profileLib[0][characterId].mMissions[i].bIsCompleted;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_isMissionReady);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Tells whether a mission is ready: bIsReady of its flags (mMissions[index] of rider 0's character in profile 0).
 *
 * @param id  the mission id
 * @return 1 or 0
 */
int cBEMissionInterface::isMissionReady(int id)
{
    int i = getMissionIdx(id);
    int charId = cBELibrary_getCharacterID(0);
    return cBELibrary_profileLib[0][charId].mMissions[i].bIsReady;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_isMissionLocked);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Tells whether a mission is still locked for the player.
 *
 * Index from cBEMissionInterface_getMissionIdx (On Tour getMissionIdx); the flag is bIsLocked of mMissions[index] in
 * the profile of rider 0's character in cBELibrary_profileLib (profile 0).
 *
 * @param id  the mission id (On Tour uiMissionId)
 * @return 1 when locked, 0 when open
 */
int cBEMissionInterface::isMissionLocked(int id)
{
    int i = getMissionIdx(id);
    int characterId = cBELibrary_getCharacterID(0);
    return cBELibrary_profileLib[0][characterId].mMissions[i].bIsLocked;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_isMissionAvailable);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Tells whether a mission is available to rider 0's character (bIsAvailable of its saved mission flags in
 * profile 0).
 *
 * @param id  mission ID (On Tour uiMissionId)
 * @return 1 when available, otherwise 0
 */
int cBEMissionInterface::isMissionAvailable(int id)
{
    int i = getMissionIdx(id);
    int characterId = cBELibrary_getCharacterID(0);
    return cBELibrary_profileLib[0][characterId].mMissions[i].bIsAvailable;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_isMissionShowIcon);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Tells whether a mission's map icon is shown: bIsShowIcon of its flags (mMissions[index] of rider 0's character
 * in profile 0).
 *
 * @param id  the mission id
 * @return 1 or 0
 */
int cBEMissionInterface::isMissionShowIcon(int id)
{
    int i = getMissionIdx(id);
    int charId = cBELibrary_getCharacterID(0);
    return cBELibrary_profileLib[0][charId].mMissions[i].bIsShowIcon;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getLocationID);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern tBEMissionInfo cBELibrary_missionInfo[];
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Returns the location (locationID) of a mission's cBELibrary_missionInfo record (index from
 * cBEMissionInterface_getMissionIdx).
 *
 * @param id  the mission id
 * @return the location
 */
int cBEMissionInterface::getLocationID(int id)
{
    int i = getMissionIdx(id);
    return cBELibrary_missionInfo[i].locationID;
}
#endif
//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getPoints);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern tBEMissionInfo cBELibrary_missionInfo[];
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Returns the points of a mission's cBELibrary_missionInfo record (index from cBEMissionInterface_getMissionIdx).
 *
 * @param id  the mission id
 * @return the value
 */
int cBEMissionInterface::getPoints(int id)
{
    int i = getMissionIdx(id);
    return cBELibrary_missionInfo[i].points;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getDiffLevel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern tBEMissionInfo cBELibrary_missionInfo[];

extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Returns the difficulty level (diffLevel) of a mission's cBELibrary_missionInfo record (index from
 * cBEMissionInterface_getMissionIdx).
 *
 * @param id  the mission id
 * @return the level, 1-3
 */
short cBEMissionInterface::getDiffLevel(int id)
{
    int i = getMissionIdx(id);
    return cBELibrary_missionInfo[i].diffLevel;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNextMissionID);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern tBEMissionInfo cBELibrary_missionInfo[];

extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Returns the ID of the mission that follows a given one.
 *
 * Finds the mission's index with cBEMissionInterface_getMissionIdx() (On Tour getMissionIdx, which asserts "Mission id not found")
 * and reads the next-mission field (nextMissionID) of its mission table entry (cBELibrary_missionInfo, On Tour
 * cBELibrary::missionInfo).
 *
 * @param id  the current mission's ID
 * @return the next mission's ID
 */
int cBEMissionInterface::getNextMissionID(int id)
{
    int i = getMissionIdx(id);
    return cBELibrary_missionInfo[i].nextMissionID;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_setIsLocked);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Sets bIsLocked (bit 0) of a mission's flags (mMissions[index] of rider 0's character in profile 0) to
 * @p value & 1.
 *
 * @param id  the mission id
 * @param value  1 to lock
 */
void cBEMissionInterface::setIsLocked(int id, int value)
{
    int i = getMissionIdx(id);
    int charId = cBELibrary_getCharacterID(0);
    cBELibrary_profileLib[0][charId].mMissions[i].bIsLocked = value;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_setIsReady);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Sets bIsReady (bit 1) of a mission's flags (mMissions[index] of rider 0's character in profile 0) to
 * @p value & 1.
 *
 * @param id  the mission id
 * @param value  1 for ready
 */
void cBEMissionInterface::setIsReady(int id, int value)
{
    int i = getMissionIdx(id);
    int charId = cBELibrary_getCharacterID(0);
    cBELibrary_profileLib[0][charId].mMissions[i].bIsReady = value;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_setIsAvailable);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Sets bIsAvailable (bit 2) of a mission's flags (mMissions[index] of rider 0's character in profile 0) to
 * @p value & 1.
 *
 * @param id  the mission id
 * @param value  1 for available
 */
void cBEMissionInterface::setIsAvailable(int id, int value)
{
    int i = getMissionIdx(id);
    int charId = cBELibrary_getCharacterID(0);
    cBELibrary_profileLib[0][charId].mMissions[i].bIsAvailable = value;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_setIsCompleted);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Sets bIsCompleted (bit 3) of a mission's flags (mMissions[index] of rider 0's character in profile 0) to
 * @p value & 1.
 *
 * @param id  the mission id
 * @param value  1 for completed
 */
void cBEMissionInterface::setIsCompleted(int id, int value)
{
    int i = getMissionIdx(id);
    int charId = cBELibrary_getCharacterID(0);
    cBELibrary_profileLib[0][charId].mMissions[i].bIsCompleted = value;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_setIsShowIcon);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern "C" int cBEMissionInterface_getMissionIdx(cBEMissionInterface* self, int id);

/**
 * Sets bIsShowIcon (bit 4) of a mission's flags (mMissions[index] of rider 0's character in profile 0) to
 * @p value & 1.
 *
 * @param id  the mission id
 * @param value  1 to show the icon
 */
void cBEMissionInterface::setIsShowIcon(int id, int value)
{
    int i = getMissionIdx(id);
    int charId = cBELibrary_getCharacterID(0);
    cBELibrary_profileLib[0][charId].mMissions[i].bIsShowIcon = value;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getMissionIdx);
#ifdef SKIP_ASM
extern tBEMissionInfo cBELibrary_missionInfo[];

/**
 * Returns the index of mission @p id in cBELibrary_missionInfo (88 entries), or -1 when it is not there.
 *
 * @param id  the mission id
 * @return the index, or -1
 */
int cBEMissionInterface::getMissionIdx(int id)
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
extern "C" int cBEMissionInterface_getNumberMissionsOnTrack(cBEMissionInterface* self, int group);
extern int D_005305F0[];

/**
 * Counts the missions on the current race's location.
 *
 * @return cBEMissionInterface_getNumberMissionsOnTrack() for the race's location (D_005305F0, iLocationIdx of
 *         the cBERaceDB copy at cBELibrary_raceDB)
 */
int cBEMissionInterface::getNumberMissions()
{
    return getNumberMissionsOnTrack(D_005305F0[0]);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNumberMissionsOnTrack);
#ifdef SKIP_ASM
extern tBEMissionInfo cBELibrary_missionInfo[];

/**
 * Counts the missions set on one location (track).
 *
 * @param group  the location (On Tour eBELocationID), compared with each of the 88 mission table entries
 * @return the number of missions on that location
 */
int cBEMissionInterface::getNumberMissionsOnTrack(int group)
{
    int n = 0;
    for (int i = 0; i < 88; i++) {
        if (cBELibrary_missionInfo[i].locationID == group) {
            n++;
        }
    }
    return n;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNumberCompletedMissionsOnTrack);
#ifdef SKIP_ASM
extern "C" int cBEMissionInterface_isMissionCompleted(cBEMissionInterface* self, int id);

/**
 * Counts the missions of location @p group (record +0x10) that rider 0's character has completed
 * (cBEMissionInterface_isMissionCompleted).
 *
 * @param group  the location
 * @return the number completed
 */
int cBEMissionInterface::getNumberCompletedMissionsOnTrack(int group)
{
    int n = 0;
    for (int i = 0; i < 88; i++)
    {
        if (cBELibrary_missionInfo[i].locationID == group)
        {
            if (isMissionCompleted(cBELibrary_missionInfo[i].id))
                n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getNumberMissionsOnPeak);
#ifdef SKIP_ASM
extern sMissionEntry_0043D950 cBELibrary_locationDB[];
extern tBEMissionInfo cBELibrary_missionInfo[];

/**
 * Counts the missions set on any location of one peak.
 *
 * @param owner  the peak (On Tour eBEPeakID)
 * @return the number of missions whose location belongs to the peak
 */
int cBEMissionInterface::getNumberMissionsOnPeak(int owner)
{
    int n = 0;
    for (int i = 0; i < 88; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (cBELibrary_locationDB[j].owner == owner)
            {
                if (cBELibrary_missionInfo[i].locationID == j)
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
int cBEMissionInterface::getNumberCompletedMissionsOnPeak(int owner)
{
    int n = 0;
    for (int i = 0; i < 88; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            if (cBELibrary_missionInfo[i].locationID == j)
            {
                if (cBELibrary_locationDB[j].owner == owner)
                {
                    if (isMissionCompleted(cBELibrary_missionInfo[i].id))
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
extern "C" int cBEMissionInterface_getNumberCompletedMissionsOnPeak(cBEMissionInterface* self, int owner);

// View of cBELibrary_locationDB[].owner (offset 0x54); splat labels that address D_0043D9A4.
struct sMissionOwnerView_001544D0
{
    int owner;
    char pad_0x04[0x60];
};
extern sMissionOwnerView_001544D0 D_0043D9A4[];

/**
 * Counts the missions one profile's character has completed on a peak.
 *
 * While a game is running (SSXApp->Game set) it returns cBEMissionInterface_getNumberCompletedMissionsOnPeak()
 * instead. Otherwise it counts the character's bIsCompleted mission flags in cBELibrary_profileLib for each
 * mission whose location belongs to the peak.
 *
 * @param profile    profile index
 * @param character  character ID within the profile
 * @param owner      the peak (On Tour eBEPeakID)
 * @return the number of completed missions
 */
int cBEMissionInterface::getNumberCompletedMissionsOnPeakByChar(int profile, int character, int owner)
{
    if ((void*)SSXApp->Game != 0)
    {
        return getNumberCompletedMissionsOnPeak(owner);
    }
    int n = 0;
    for (int i = 0; i < 88; i++)
    {
        if (D_0043D9A4[cBELibrary_missionInfo[i].locationID].owner == owner)
        {
            n += cBELibrary_profileLib[profile][character].mMissions[i].bIsCompleted;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getMissionID);
#ifdef SKIP_ASM
extern "C" int cBEMissionInterface_getMissionIDOnTrack(cBEMissionInterface* self, int a, int group);
extern int D_005305F0[];

/**
 * Returns the id of the n-th mission at the current location.
 *
 * cBEMissionInterface_getMissionIDOnTrack with the current location id from D_005305F0.
 *
 * @param iIdx  the mission's index at this location (On Tour iIdx)
 * @return the mission id, 0 when there are not that many
 */
int cBEMissionInterface::getMissionID(int iIdx)
{
    return getMissionIDOnTrack(iIdx, D_005305F0[0]);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getMissionIDOnTrack);
#ifdef SKIP_ASM
extern tBEMissionInfo cBELibrary_missionInfo[];

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
int cBEMissionInterface::getMissionIDOnTrack(int index, int group)
{
    int n = 0;
    for (int i = 0; i < 88; i++) {
        if (cBELibrary_missionInfo[i].locationID == group) {
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
extern tBEPictureDB cBELibrary_pictureDB[];

/**
 * Looks up the number of sessions for a track id in the 22-entry cBELibrary_pictureDB table.
 *
 * The table's name (picture DB) does not fit a session count, so either it or this function is likely misnamed.
 *
 * @param id  track id to find
 * @return the value of the entry with that id, or 0 if there is none
 */
int cBEMissionInterface::getNumberSessionsOnTrack(int id)
{
    for (int i = 0; i < 22; i++) {
        if (cBELibrary_pictureDB[i].iLocationID == id) {
            return cBELibrary_pictureDB[i].numSessionPoints;
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
int cBEMissionInterface::isSessionPoint(int value, int id)
{
    for (int i = 0; i < 22; i++) {
        if (cBELibrary_pictureDB[i].iLocationID == id && cBELibrary_pictureDB[i].numSessionPoints >= value) {
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
extern "C" tBEPictureDB cBEMissionInterface_getPictureDBRecord(cBEMissionInterface* self, int id)
{
    for (int i = 0; i < 22; i++)
    {
        if (cBELibrary_pictureDB[i].iLocationID == id)
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
extern "C" void* cBEMissionInterface_getPictureFileName(cBEMissionInterface* self, int id)
{
    for (int i = 0; i < 22; i++) {
        if (cBELibrary_pictureDB[i].iLocationID == id) {
            return cBELibrary_pictureDB[i].strFileName;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_getAudioType);
#ifdef SKIP_ASM
extern tBEMissionInfo cBELibrary_missionInfo[];

/**
 * Returns the audioType of mission @p id's cBELibrary_missionInfo record, or 0 when the id is unknown.
 *
 * @param id  the mission id
 * @return the value (the sound system uses it as the mission's audio kind)
 */
extern "C" int cBEMissionInterface_getAudioType(cBEMissionInterface* self, int id)
{
    for (int i = 0; i < 88; i++) {
        if (cBELibrary_missionInfo[i].id == id) {
            return cBELibrary_missionInfo[i].audioType;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_addStreamData__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty stub that returns at once.
 *
 * world/worldview.cpp calls it on BE interface 10 with three more arguments when a streamed resource arrives (the
 * resource id byte, a pointer into the resource and its first word); this build ignores them.
 */
void cBEMissionInterface_addStreamData(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_remStreamData__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: an empty stub that returns at once.
 *
 * world/worldview.cpp calls it on BE interface 10 with the resource id byte when a streamed resource goes away; this
 * build ignores it.
 */
void cBEMissionInterface_remStreamData(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_writeToReplayFrame);
#ifdef SKIP_ASM
extern char D_004A6CAC[];

/**
 * Writes the mission state of rider 0's character (profile 0) to a replay stream: its 23 collect records (0x114
 * bytes from profile +0x4), its 88 mission flag words (0x160 bytes from +0x118), then the interface's word at +0x8.
 *
 * Each write is vtable slot 1 of @p stream; cBEMissionInterface_readFromReplayFrame reads the same layout.
 *
 * @param stream  the replay frame stream
 */
extern "C" void cBEMissionInterface_writeToReplayFrame(cBEMissionInterface* self, cBxReplayFrame* stream)
{
    int characterId = cBELibrary_getCharacterID(0);
    char* base = D_004A6CAC;
    int off = characterId * 3976;
    stream->write(base + off, 276);
    char* base2 = base + 0x114;
    stream->write(base2 + off, 352);
    stream->write((const char*)&self->bIsActive, 4);
}
#endif

//100%
INCLUDE_ASM("be/beintmission", cBEMissionInterface_readFromReplayFrame);
#ifdef SKIP_ASM
extern char D_004A6CAC[];

/**
 * Reads the mission state of rider 0's character (profile 0) back from a replay stream: 23 collect records (0x114
 * bytes), 88 mission flag words (0x160 bytes), then the interface's word at +0x8.
 *
 * Each read is vtable slot 2 of @p stream.
 *
 * @param stream  the replay frame stream
 */
extern "C" void cBEMissionInterface_readFromReplayFrame(cBEMissionInterface* self, cBxReplayFrame* stream)
{
    int characterId = cBELibrary_getCharacterID(0);
    char* base = D_004A6CAC;
    int off = characterId * 3976;
    stream->read(base + off, 276);
    char* base2 = base + 0x114;
    stream->read(base2 + off, 352);
    stream->read((char*)&self->bIsActive, 4);
}
#endif

