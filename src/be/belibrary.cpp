#include "common.h"
#include "be/bedbprofile.h"
#include "be/bedbbolt.h"
#include "be/beintbolt.h"
#include "vtable.h"
#include "be/beintreward.h"
#include "be/beprofilelib.h"
#include "be/beintplayer.h"
#include "be/beintoption.h"

extern int cBELibrary_raceDB[];


//100%
INCLUDE_ASM("be/belibrary", cBELibrary_getCharacterID__Fi);
#ifdef SKIP_ASM
/**
 * Returns the character ID of the player in a rider slot.
 *
 * Maps the slot to a player through the race settings copy at cBELibrary_raceDB and reads mCharID from that player's
 * cBELibrary_playerDB record.
 *
 * @param index  rider slot
 * @return the character ID
 */
signed char cBELibrary_getCharacterID(int index)
{
    int charID = cBELibrary_raceDB[index];
    cBEPlayerDB* entry = &cBELibrary_playerDB[charID];
    return entry->cCharacterID;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBELibrary_getCheatCharacterID);
#ifdef SKIP_ASM
/**
 * Returns the cheat character selected for a race rider, from the rider's player record.
 *
 * @param index  race rider slot (On Tour iRider)
 * @return the cheat character ID (byte 0x12 of the record)
 */
extern "C" signed char cBELibrary_getCheatCharacterID(int index)
{
    int charID = cBELibrary_raceDB[index];
    return cBELibrary_playerDB[charID].cCheatCharacterID;
}
#endif


//100%
INCLUDE_ASM("be/belibrary", cBELibrary_getProfileIndex__Fi);
#ifdef SKIP_ASM
/**
 * Returns the profile a race rider belongs to: 0 or 1 for a human player, probably 2 for a computer rider.
 *
 * @param index  race rider slot
 * @return the profile index, or 2 when the record's +0xC is kNoController (-1, a computer rider)
 */
int cBELibrary_getProfileIndex(int index)
{
    int charID = cBELibrary_raceDB[index];
    cBEPlayerDB* entry = &cBELibrary_playerDB[charID];
    if (entry->iControllerID == cBEPlayerDB::kNoController) {
        return 2;
    }
    return entry->iHumanID;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBELibrary_getRiderIndex__Fi);
#ifdef SKIP_ASM
/**
 * Finds the first race rider slot whose player record belongs to a profile.
 *
 * Computer records also have a profile bit, so this is meant for human profiles 0 and 1.
 *
 * @param flag  profile index (0 or 1)
 * @return the rider slot (0-5), or -1 when none matches
 */
int cBELibrary_getRiderIndex(int flag)
{
    int* base = cBELibrary_raceDB;
    int count = 0;
    while (1) {
        int charID = *base;
        cBEPlayerDB* entry = &cBELibrary_playerDB[charID];
        int bit = entry->iHumanID;
        if (bit == flag) {
            return count;
        }
        count++;
        if (count < 6) {
            base++;
            continue;
        }
        return -1;
    }
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBELibrary_printMemory);
#ifdef SKIP_ASM
/**
 * Does nothing (an empty function; nothing calls it).
 */
extern "C" void cBELibrary_printMemory(void) {

}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBELibrary_writeToData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
struct sBlk4C_A188 { char b[0x4C]; };
struct sBlkF88_A188 { char b[0xF88]; };
struct sBlk9B50_A188 { char b[0x9B50]; };

// The whole player table (cBELibrary_playerDB) as one block: the copy below keeps cBEPlayerDB's 4-byte alignment, which a
// memcpy from a char* would not (gcc's inline block move takes the smaller alignment of its two pointers).
struct sProfileBlk_A188 {
    cBEPlayerDB entries[6];
};

extern cBECharProfileDB cBELibrary_profileLib[][10];
extern sBlkF88_A188 D_004B07F8_A188[] __asm__("D_004B07F8");
extern sBlk9B50_A188 D_004BA348_A188 __asm__("D_004BA348");

/**
 * Writes the career data into a save buffer, in the layout cBELibrary_readFromData reads back.
 *
 * Copies, in order: the 0x4C-byte race DB (cBELibrary_raceDB), the 0xA8-byte player DB (cBELibrary_playerDB), player
 * 0's character record from profile 0 of cBELibrary_profileLib, player 1's character record from profile 1 (D_004B07F8)
 * when player 1 has a character (cCharacterID >= 0), the 0x9B50-byte profile D_004BA348 (profile 2) and the 0x20 bytes at
 * cBERules_mRules. cBE_writeToData calls it and appends the CRC.
 *
 * @param data  the save buffer to fill
 */
extern "C" void cBELibrary_writeToData(char* data)
{
    int rider;
    *(sBlk4C_A188*)data = *(sBlk4C_A188*)cBELibrary_raceDB;
    data += sizeof(sBlk4C_A188);
    *(sProfileBlk_A188*)data = *(sProfileBlk_A188*)cBELibrary_playerDB;
    data += sizeof(cBELibrary_playerDB);
    rider = cBELibrary_playerDB[0].cCharacterID;
    *(sBlkF88_A188*)data = ((sBlkF88_A188*)cBELibrary_profileLib)[rider];
    data += sizeof(sBlkF88_A188);
    rider = cBELibrary_playerDB[1].cCharacterID;
    if (rider >= 0) {
        *(sBlkF88_A188*)data = D_004B07F8_A188[rider];
        data += sizeof(sBlkF88_A188);
    }
    *(sBlk9B50_A188*)data = D_004BA348_A188;
    data += sizeof(sBlk9B50_A188);
    *(tBERuleOptionDB*)data = (*(&cBERules_mRules));
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBELibrary_readFromData);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cBEIface_14A5E0 {
public:
    char pad[0xC];
    virtual void v1();
    virtual void onLoad();
};
cBEIface_14A5E0* cBENewRaceInterface_getThis();
cBEIface_14A5E0* cBENewPlayerInterface_getThis();
cBEIface_14A5E0* cBEBAGTInterface_getThis();
cBEIface_14A5E0* cBEMissionInterface_getThis();
cBEIface_14A5E0* cBEScoreInterface_getThis();
extern "C" cBEIface_14A5E0* cBEStatInterface_getThis(void);

struct sBlk4C_14A5E0 { char d[0x4C]; };
struct sBlkF88_14A5E0 { char d[0xF88]; };
struct sBlk9B50_14A5E0 { char d[0x9B50]; };

extern sBlkF88_14A5E0 cBELibrary_profileLib_14A5E0[] __asm__("cBELibrary_profileLib");
extern sBlkF88_14A5E0 D_004B07F8[];
extern sBlk9B50_14A5E0 D_004BA348;

extern "C" void* memcpy(void*, const void*, unsigned int);

/**
 * Restores the career data from a loaded save buffer.
 *
 * Reads, in order: the 0x4C-byte race DB (then cBENewRaceInterface's virtual slot 2), the 0xA8-byte player DB (then
 * cBENewPlayerInterface's), player 0's character profile into cBELibrary_profileLib, player 1's into the next profile
 * slot when player 1 has a character, a 0x9B50-byte profile slot into D_004BA348 and 0x20 bytes into cBERules_mRules, then
 * calls slot 2 of the BAGT, mission, score and stat interfaces.
 *
 * @param data  the save buffer (On Tour data)
 */
extern "C" void cBELibrary_readFromData(char* data)
{
    cBEIface_14A5E0* beInterface;
    int idx;

    *(sBlk4C_14A5E0*)cBELibrary_raceDB = *(sBlk4C_14A5E0*)data;
    cBENewRaceInterface_getThis()->onLoad();
    data += 0x4C;
    memcpy(cBELibrary_playerDB, data, sizeof(cBELibrary_playerDB));
    cBENewPlayerInterface_getThis()->onLoad();
    data += 0xA8;
    idx = cBELibrary_playerDB[0].cCharacterID;
    cBELibrary_profileLib_14A5E0[idx] = *(sBlkF88_14A5E0*)data;
    data += 0xF88;
    idx = cBELibrary_playerDB[1].cCharacterID;
    if (idx >= 0) {
        D_004B07F8[idx] = *(sBlkF88_14A5E0*)data;
        data += 0xF88;
    }
    D_004BA348 = *(sBlk9B50_14A5E0*)data;
    data += 0x9B50;
    (*(&cBERules_mRules)) = *(tBERuleOptionDB*)data;
    beInterface = cBEBAGTInterface_getThis();
    beInterface->onLoad();
    beInterface = cBEMissionInterface_getThis();
    beInterface->onLoad();
    beInterface = cBEScoreInterface_getThis();
    beInterface->onLoad();
    beInterface = cBEStatInterface_getThis();
    beInterface->onLoad();
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBELibrary_getHighScoreTable);
#ifdef SKIP_ASM
struct sScoreTable_0014AB20 {
    char data[0x64];
};
extern sScoreTable_0014AB20 cBELibrary_highScores[];
extern "C" int cBELibrary_getScoreType(int a, int b);

/**
 * Returns the 5-entry high-score table for a location and event, or null when the pair has none.
 *
 * The index is cBELibrary_getScoreType(@p locationId, @p gameEvent); kNoScoreType (26) means none. The tables are
 * cBELibrary_highScores, 0x64 bytes each (five 0x14-byte entries: score, character ID, 12-byte name).
 *
 * @param locationId  the location (On Tour eBELocationID)
 * @param gameEvent  the game event or stat kind
 * @return the table, or null
 */
extern "C" void* cBELibrary_getHighScoreTable(int locationId, int gameEvent)
{
    int type = cBELibrary_getScoreType(locationId, gameEvent);
    if (type == kNoScoreType) {
        return 0;
    }
    return &cBELibrary_highScores[type];
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBELibrary_getScoreType);
#ifdef SKIP_ASM
struct sScoreTypeMap {
    int key;
    int type;
};
extern sScoreTypeMap cBELibrary_getScoreType_kScoreTypeMap[][2];

/**
 * Maps a location and an event or stat kind to an index into the 26-entry score tables (0x1A means none).
 *
 * Kinds 6 to 11 map to 0 to 5 at any location. Otherwise the location's row of cBELibrary_getScoreType_kScoreTypeMap (two key/type pairs)
 * gives the index: kind 0 to 3 at locations 0 to 13 gives 12 to 25, kinds 4 and 5 at locations 14 to 16 give 6
 * to 11. Callers use it to pick a score stats record in the character profile and in cBELibrary_highScores.
 *
 * @param locationId  the location (On Tour eBELocationID); above 16 gives 26
 * @param gameEvent  the event or stat kind
 * @return the score type index, or 26 when the pair has none
 */
extern "C" int cBELibrary_getScoreType(int locationId, int gameEvent)
{
    int i;
    if (locationId > 16) {
        return 26;
    }
    if ((unsigned)(gameEvent - 6) < 3) {
        return gameEvent - 6;
    }
    if ((unsigned)(gameEvent - 9) < 3) {
        return gameEvent - 6;
    }
    sScoreTypeMap* row = cBELibrary_getScoreType_kScoreTypeMap[locationId];
    for (i = 0; i < 2; i++) {
        if (row[i].key == gameEvent) {
            return row[i].type;
        }
    }
    return 26;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getThis);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BEBoltInt[];
extern void* cBEInterface_vtbl[16];


/**
 * Returns the bolt (equipment) interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BEBoltInt" (cMemMan_alloc), clears the word at +0x8, stores
 * cBEInterface's vtable cBEInterface_vtbl and keeps the object in cBEBoltInterface_instance. It is BE interface 9 (cBE_getInterface(.., 9));
 * cBELibrary_cBELibrary creates it with the other interfaces.
 *
 * @return the interface
 */
extern "C" cBEBoltInterface* cBEBoltInterface_getThis(void)
{
    if (cBEBoltInterface_instance == 0) {
        cBEBoltInterface* mem = (cBEBoltInterface*)cMemMan_alloc(sizeof(cBEBoltInterface), str_BEBoltInt, 0, 0);
        mem->bIsActive = 0;
        mem->mVtbl = (void**)cBEInterface_vtbl;
        cBEBoltInterface_instance = mem;
    }
    return cBEBoltInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getCharacterID);
#ifdef SKIP_ASM
/**
 * Returns a race rider's character ID (cBELibrary_getCharacterID()).
 *
 * @param rider  race rider slot
 * @return the character ID
 */
extern "C" signed char cBEBoltInterface_getCharacterID(int rider)
{
    return cBELibrary_getCharacterID(rider);
}
#endif

//99.29% - identical instructions; jal addend differs only because the
// callee sits at a different .text offset in our object than in the target
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getProfileIndex);
#ifdef SKIP_ASM
/**
 * Returns a race rider's profile index (cBELibrary_getProfileIndex()).
 *
 * @param rider  race rider slot
 * @return the profile index
 */
extern "C" int cBEBoltInterface_getProfileIndex(int rider)
{
    return cBELibrary_getProfileIndex(rider);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getProfileAndCharacter);
#ifdef SKIP_ASM
/**
 * Returns a race rider's profile index and stores its character ID through @p outChar.
 *
 * @param rider  race rider slot
 * @param outChar  receives the character ID
 * @return the profile index
 */
int cBEBoltInterface::getProfileAndCharacter(int rider, int* outChar)
{
    *outChar = cBELibrary_getCharacterID(rider);
    return cBELibrary_getProfileIndex(rider);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_restoreBolts);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
extern "C" void* MEM_copy(void* dst, void* src, int n);

/**
 * Restores a character's bolt entries: copies 2100 bytes from @p src to +0x290 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * The equip screen saves the 525 entries, tries changes, and puts them back with this.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @param src  the saved entries
 */
void cBEBoltInterface::restoreBolts(int profileIndex, int characterId, void* src)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    MEM_copy(&charRecord->mBolts[0], src, 2100);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_buildBoltList);
#ifdef SKIP_ASM
extern "C" void* cBEBoltInterface_getCharProfile(cBEBoltInterface* self, int a1, int a2);
extern "C" void cBECharProfileDB_buildBoltList(void*, int);

/**
 * Reloads a character's bolt data: cBECharProfileDB_buildBoltList on the character record (cBEBoltInterface_getCharProfile) for that character.
 *
 * Nothing calls it.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 */
void cBEBoltInterface::buildBoltList(int profileIndex, int characterId)
{
    cBECharProfileDB_buildBoltList(getCharProfile(profileIndex, characterId), characterId);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getCharProfile);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns the character record (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @return the 0xF88-byte character record
 */
void* cBEBoltInterface::getCharProfile(int profileIndex, int characterId)
{
    return (char*)&cBELibrary_profileLib[profileIndex][characterId];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getRiderCharProfile);
#ifdef SKIP_ASM
/**
 * Returns a race rider's character record: cBEBoltInterface_getCharProfile for its profile (cBEBoltInterface_getProfileIndex) and character (cBEBoltInterface_getCharacterID).
 *
 * @param rider  race rider slot
 * @return the character record
 */
void* cBEBoltInterface::getRiderCharProfile(int rider)
{
    int profile = cBEBoltInterface_getProfileIndex(rider);
    return getCharProfile(profile, cBEBoltInterface_getCharacterID(rider));
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getBoltEntry);
#ifdef SKIP_ASM
/**
 * Returns the address of bolt entry @p slot of a character: +0x290 + slot * 4 of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88).
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @param slot  entry index
 * @return the entry
 */
void* cBEBoltInterface::getBoltEntry(int profileIndex, int characterId, int slot)
{
    int off = profileIndex * 39760 + characterId * 3976;
    char* charRecord = (char*)cBELibrary_profileLib + off;
    return charRecord + (slot * 4 + 656);
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getRiderBoltEntry);
#ifdef SKIP_ASM
extern "C" int cBEBoltInterface_getProfileAndCharacter(cBEBoltInterface* self, int rider, int* outChar);
extern "C" void* cBEBoltInterface_getBoltEntry(cBEBoltInterface* self, int a1, int a2, int a3);

/**
 * Returns the address of bolt entry @p slot of a race rider's character (cBEBoltInterface_getProfileAndCharacter, then cBEBoltInterface_getBoltEntry).
 *
 * Nothing calls it.
 *
 * @param rider  race rider slot
 * @param slot  entry index
 * @return the entry
 */
void* cBEBoltInterface::getRiderBoltEntry(int rider, int slot)
{
    int ch = 0;
    int profile = getProfileAndCharacter(rider, &ch);
    return getBoltEntry(profile, ch, slot);
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getBolts);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// One character's flag table inside a profile record: cBELibrary_profileLib is
// [profile][character] of these (0x9B50 = 10 * 0xF88).
struct sBEFlagEntryPC {
    short id;
    unsigned short flags;
};

struct sBEProfileChar {
    char pad_0x000[0x288];
    short* index;                       // 0x288
    int count;                          // 0x28C
    sBEFlagEntryPC entries[0x20D];      // 0x290
    char pad_0xAC4[0xF88 - 0xAC4];

    int getCount() { return count; }
    sBEFlagEntryPC* getEntries() { return entries; }
};

extern sBEProfileChar cBELibrary_profileLib_pc[][10] __asm__("cBELibrary_profileLib");

/**
 * Returns a character's bolt entries (+0x290 of the character record (cBELibrary_profileLib + profile * 0x9B50 +
 * character * 0xF88)) and stores their count (+0x28C) through @p out.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @param out  receives the number of entries
 * @return the first entry
 */
void* cBEBoltInterface::getBolts(int profileIndex, int characterId, int* out)
{
    *out = cBELibrary_profileLib_pc[profileIndex][characterId].getCount();
    return cBELibrary_profileLib_pc[profileIndex][characterId].getEntries();
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getRiderBolts);
#ifdef SKIP_ASM
extern "C" int cBEBoltInterface_getProfileAndCharacter(cBEBoltInterface* self, int rider, int* outChar);
extern "C" void* cBEBoltInterface_getBolts(cBEBoltInterface* self, int a1, int a2, int* out);

/**
 * Returns a race rider's bolt entries and stores their count through @p out (cBEBoltInterface_getProfileAndCharacter, then cBEBoltInterface_getBolts).
 *
 * Nothing calls it.
 *
 * @param rider  race rider slot
 * @param out  receives the number of entries
 * @return the first entry
 */
void* cBEBoltInterface::getRiderBolts(int rider, int* out)
{
    int ch;
    int profile = getProfileAndCharacter(rider, &ch);
    return getBolts(profile, ch, out);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_storeVisibleFlags);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

struct sBEFlagEntry {
    unsigned short field_0x0;
    short flags;
};

/**
 * Copies flag 0x10 of each of a character's bolt entries into flag 0x4.
 *
 * Walks the character record's bolt entries (+0x290, 4 bytes each: id, flags; count at +0x28C) of the character record
 * (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88). The equip screen calls it on entry and after a
 * successful toggle (cBEBoltInterface_trySetBoltVisible changes flag 0x10).
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 */
void cBEBoltInterface::storeVisibleFlags(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    int count = charRecord->mNumBolts;
    sBEFlagEntry* flagEntry = (sBEFlagEntry*)(&charRecord->mBolts[0]);
    int i;
    for (i = 0; i < count; i++, flagEntry++) {
        if (flagEntry->flags & 0x10) {
            flagEntry->flags |= 4;
        } else {
            flagEntry->flags &= ~4;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_restoreVisibleFlags);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Copies flag 0x4 of every bolt entry into flag 0x10, for all ten characters of profiles 0 to 2.
 *
 * The reverse of cBEBoltInterface_storeVisibleFlags. The front end's load sequence (fereal) calls it on the bolt interface.
 */
extern "C" void cBEBoltInterface_restoreVisibleFlags(void)
{
    int profileIndex;
    int characterId;
    for (profileIndex = 0; profileIndex < 3; profileIndex++) {
        for (characterId = 0; characterId < 10; characterId++) {
            cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
            int count = charRecord->mNumBolts;
            sBEFlagEntry* flagEntry = (sBEFlagEntry*)(&charRecord->mBolts[0]);
            int i;
            for (i = 0; i < count; i++, flagEntry++) {
                if (flagEntry->flags & 4) {
                    flagEntry->flags |= 0x10;
                } else {
                    flagEntry->flags &= ~0x10;
                }
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_trySetBoltVisible);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
extern "C" bool cBECharProfileDB_setBoltVisible(void* prof, int idx, int on);
extern "C" int cBECharProfileDB_showDefaultBolts(void* prof);
extern "C" bool cBECharProfileDB_reapplyBoltRules(void* prof, int* out);

static inline tBEProfileBolt* getEntry_14AFB0(cBECharProfileDB* p, int idx)
{
    short k = p->mpBoltsMap[idx];
    if (k >= 0)
        return &p->mBolts[k];
    return 0;
}

/**
 * Tries to equip (@p on) or take off bolt @p idx for a character, keeping the change only when the result is valid.
 *
 * Saves the character's 525 bolt entries (2100 bytes at +0x290 of the character record (cBELibrary_profileLib + profile
 * * 0x9B50 + character * 0xF88)), then asks cBECharProfileDB_setBoltVisible(record, @p idx, @p on). If it agrees, sets or clears flag
 * 0x10 of the bolt's entry, and keeps the change when cBECharProfileDB_showDefaultBolts(record) passes and cBECharProfileDB_reapplyBoltRules(record, &n)
 * succeeds with n == 0 or a second cBECharProfileDB_showDefaultBolts check passing. Otherwise restores the saved entries.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @param idx  the bolt id
 * @param on  1 to equip, 0 to take off
 * @return true when the change was kept
 */
bool cBEBoltInterface::trySetBoltVisible(int profileIndex, int characterId, int idx, int on)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    tBEProfileBolt save[525];

    MEM_copy(save, charRecord->mBolts, 2100);
    if (cBECharProfileDB_setBoltVisible(charRecord, idx, on)) {
        tBEProfileBolt* flagEntry = getEntry_14AFB0(charRecord, idx);
        if (on)
            flagEntry->flags |= 0x10;
        else
            flagEntry->flags &= ~0x10;
        if (cBECharProfileDB_showDefaultBolts(charRecord)) {
            int n = 0;
            if (cBECharProfileDB_reapplyBoltRules(charRecord, &n)) {
                if (n == 0 || cBECharProfileDB_showDefaultBolts(charRecord))
                    return true;
            }
        }
    }
    MEM_copy(charRecord->mBolts, save, 2100);
    return false;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_toggleBoltVisible);
#ifdef SKIP_ASM
/**
 * Toggles bolt @p idx for a character: cBEBoltInterface_trySetBoltVisible with on = 1 unless the bolt's entry already has flag 0x10.
 *
 * Nothing calls it.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @param idx  the bolt id
 */
void cBEBoltInterface::toggleBoltVisible(int profileIndex, int characterId, int idx)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    tBEProfileBolt* flagEntry = getEntry_14AFB0(charRecord, idx);
    int on = 1;
    if (flagEntry->flags & 0x10) on = 0;
    trySetBoltVisible(profileIndex, characterId, idx, on);
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_hasBoltFlags);
#ifdef SKIP_ASM
extern "C" void cBECharProfileDB_hasBoltFlags(void*, int, int);

/**
 * Tests flags of one bolt of a character: cBECharProfileDB_hasBoltFlags(the character record (cBELibrary_profileLib + profile * 0x9B50
 * + character * 0xF88), @p boltId, @p flagMask).
 *
 * cBECharProfileDB_hasBoltFlags is true when every bit of the mask is set in the bolt's entry flags. Nothing calls it; the result is
 * dropped.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @param boltId  the bolt id
 * @param flagMask  the flag mask
 */
void cBEBoltInterface::hasBoltFlags(int profileIndex, int characterId, int boltId, int flagMask)
{
    cBECharProfileDB_hasBoltFlags((char*)&cBELibrary_profileLib[profileIndex][characterId], boltId, flagMask);
}
#endif
//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/belibrary", cBEBoltInterface_riderHasBoltFlags);
#ifdef SKIP_ASM
extern "C" int cBEBoltInterface_getProfileAndCharacter(cBEBoltInterface* self, int rider, int* outChar);
extern "C" void cBECharProfileDB_hasBoltFlags(void*, int, int);

/**
 * Tests flags of one bolt of a race rider's character: cBECharProfileDB_hasBoltFlags on the rider's character record (found with
 * cBEBoltInterface_getProfileAndCharacter).
 *
 * Nothing calls it; the result is dropped.
 *
 * @param rider  race rider slot
 * @param boltId  the bolt id
 * @param flagMask  the flag mask
 */
void cBEBoltInterface::riderHasBoltFlags(int rider, int boltId, int flagMask)
{
    int ch = 0;
    int profile = getProfileAndCharacter(rider, &ch);
    cBECharProfileDB_hasBoltFlags((char*)&cBELibrary_profileLib[profile][ch], boltId, flagMask);
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getBoltsWithFlags);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// One character's flag table inside a profile record: cBELibrary_profileLib is
// [profile][character] of these (0x9B50 = 10 * 0xF88).
extern sBEProfileChar cBELibrary_profileLib_pc[][10] __asm__("cBELibrary_profileLib");

/**
 * Copies every bolt entry of a character whose flags contain all of @p mask to @p out and returns how many.
 *
 * Walks the character record's bolt entries (+0x290, 4 bytes each: id, flags; count at +0x28C) of
 * cBELibrary_profileLib[@p profileIndex][@p characterId]. Nothing calls it.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @param mask  flags an entry must have
 * @param out  receives the matching 4-byte entries
 * @return the number of entries copied
 */
extern "C" int cBEBoltInterface_getBoltsWithFlags(cBEBoltInterface* self, int profileIndex, int characterId,
                                                  unsigned int mask, sBEFlagEntryPC* out)
{
    int count = cBELibrary_profileLib_pc[profileIndex][characterId].getCount();
    sBEFlagEntryPC* flagEntry = cBELibrary_profileLib_pc[profileIndex][characterId].getEntries();
    int n = 0;
    int i;
    for (i = 0; i < count; i++, flagEntry++) {
        if ((flagEntry->flags & mask) == mask) {
            *out++ = *flagEntry;
            n++;
        }
    }
    return n;
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_countBoltsWithFlags);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
// One character's flag table inside a profile record: cBELibrary_profileLib is
// [profile][character] of these (0x9B50 = 10 * 0xF88).
extern sBEProfileChar cBELibrary_profileLib_pc[][10] __asm__("cBELibrary_profileLib");

/**
 * Counts the bolt entries of a character whose flags contain all of @p mask.
 *
 * Walks the character record's bolt entries (+0x290, 4 bytes each: id, flags; count at +0x28C) of
 * cBELibrary_profileLib[@p profileIndex][@p characterId]. Nothing calls it.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @param mask  flags an entry must have
 * @return the number of matching entries
 */
int cBEBoltInterface::countBoltsWithFlags(int profileIndex, int characterId, unsigned int mask)
{
    int count = cBELibrary_profileLib_pc[profileIndex][characterId].getCount();
    sBEFlagEntryPC* flagEntry = cBELibrary_profileLib_pc[profileIndex][characterId].getEntries();
    int n = 0;
    int i;
    for (i = 0; i < count; i++, flagEntry++) {
        if ((flagEntry->flags & mask) == mask) {
            n++;
        }
    }
    return n;
}
#endif
//100%
INCLUDE_ASM("be/belibrary", func_0014B2F8);
#ifdef SKIP_ASM
int cBEBoltDB_getNumCharBolts(void* self, int i);
extern "C" tBEBoltDef* cBEBoltDB_getCharBolts(cBEBoltDB* self, int i);

struct sBEEntry38_B2F8 {
    char pad_0x00;
    signed char used; // 0x1
    char pad_0x02[0x36];
};

/**
 * Counts a character's bolt definitions whose byte +0x1 is zero.
 *
 * Walks the character's 0x38-byte definitions in the bolt database cBELibrary_boltDB (cBEBoltDB_getCharBolts, count cBEBoltDB_getNumCharBolts).
 * Nothing calls it.
 *
 * @param characterId  character ID (the bolt database group)
 * @return the number of definitions with byte +0x1 == 0
 */
extern "C" int func_0014B2F8(cBEBoltInterface* self, int characterId)
{
    int count = cBELibrary_boltDB.getNumCharBolts(characterId);
    sBEEntry38_B2F8* boltDef = (sBEEntry38_B2F8*)cBELibrary_boltDB.getCharBolts(characterId);
    int n = 0;
    int i;
    for (i = 0; i < count; i++, boltDef++) {
        if (boltDef->used == 0)
            n++;
    }
    return n;
}
#endif
//100%
INCLUDE_ASM("be/belibrary", func_0014B378);
#ifdef SKIP_ASM
int cBEBoltDB_getNumCharBolts(void* self, int i);
extern "C" tBEBoltDef* cBEBoltDB_getCharBolts(cBEBoltDB* self, int i);

struct sBEEntry38_B378 {
    char pad_0x00;
    signed char used; // 0x1
    char pad_0x02[0x36];
};

/**
 * Copies a character's bolt definitions whose byte +0x1 is zero to @p out and returns how many.
 *
 * Walks the character's 0x38-byte definitions in the bolt database cBELibrary_boltDB (cBEBoltDB_getCharBolts, count cBEBoltDB_getNumCharBolts).
 * Nothing calls it.
 *
 * @param characterId  character ID (the bolt database group)
 * @param out  receives the 0x38-byte definitions
 * @return the number of definitions copied
 */
extern "C" int func_0014B378(cBEBoltInterface* self, int characterId, sBEEntry38_B378* out)
{
    int count = cBELibrary_boltDB.getNumCharBolts(characterId);
    sBEEntry38_B378* boltDef = (sBEEntry38_B378*)cBELibrary_boltDB.getCharBolts(characterId);
    int n = 0;
    int i;
    for (i = 0; i < count; i++, boltDef++) {
        if (boltDef->used == 0) {
            *out++ = *boltDef;
            n++;
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getUsedEquipPoints);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];
int cBEBoltDB_getNumCharBolts(void* self, int i);
extern "C" tBEBoltDef* cBEBoltDB_getCharBolts(cBEBoltDB* self, int i);

struct sBEFlagEntryB {
    unsigned short field_0x0;
    unsigned short flags;
};

struct sBEEntry38B {
    signed char group; // 0x0
    char pad_0x01[3];
    short id;          // 0x4 (tBEBoltDef's names; the rest ours, from use)
    short parentId;    // 0x6
    char pad_0x08[4];
    short equipPoints; // 0xC
    char pad_0x0E[0x2A];
};

/**
 * Returns the equip points a character's equipped bolts use: the sum of the short at +0xC of each bolt definition whose
 * entry has flag 0x10.
 *
 * Walks the character's definitions in the bolt database cBELibrary_boltDB and finds each one's entry through the lookup table
 * at +0x288 of the character record (cBELibrary_profileLib + profile * 0x9B50 + character * 0xF88) (no null check). The
 * equip screen shows it as the points spent.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @return the total equip points
 */
int cBEBoltInterface::getUsedEquipPoints(int profileIndex, int characterId)
{
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
    int sum = 0;
    int count = cBELibrary_boltDB.getNumCharBolts(characterId);
    sBEEntry38B* boltDef = (sBEEntry38B*)cBELibrary_boltDB.getCharBolts(characterId);
    int i;
    for (i = 0; i < count; i++, boltDef++) {
        short entryIndex = (charRecord->mpBoltsMap)[boltDef->id];
        sBEFlagEntryB* flagEntry;
        if (entryIndex >= 0)
            flagEntry = (sBEFlagEntryB*)(&charRecord->mBolts[0]) + entryIndex;
        else
            flagEntry = 0;
        if (flagEntry->flags & 0x10)
            sum += boltDef->equipPoints;
    }
    return sum;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_buyBolt);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cBEBoltInterface_getBoltDB__FPv ignores its argument; callers pass none.
cBEBoltDB* cBEBoltInterface_getBoltDB_0() __asm__("cBEBoltInterface_getBoltDB__FPv");
extern "C" int cBEBoltDB_findBoltIndex(cBEBoltDB* self, int g, int key);
extern "C" void cBECharProfileDB_setBoltBought(void* prof, int idx, int on);
extern "C" void cBEEconInterface_reduceMoney(void* iface, int a, int b, int v);
extern "C" void cBEEconInterface_getMoney(void* iface, int a, int b);

static inline tBEBoltDef* entry_14B560(cBEBoltDB* t, int i)
{
    if (i < 0)
        return 0;
    return &t->mpCharBolts[i];
}

static inline int scale10_14B560(int v)
{
    if (v > 0) v *= 10;
    return v;
}

/**
 * Gives a character bolt @p key and the bolts bundled with it, charging its price when @p flag is set.
 *
 * Marks the bolt owned (flag 0x2, cBECharProfileDB_setBoltBought) in the character record (cBELibrary_profileLib + profile * 0x9B50 +
 * character * 0xF88), and also every bolt of the character whose price (short +0xE, times 10 when positive) equals -@p
 * key. With @p flag, takes the bolt's price (its definition found with cBEBoltDB_findBoltIndex) times 10 off the character's
 * money (cBEEconInterface_reduceMoney on the economy interface); otherwise just calls cBEEconInterface_getMoney. The store's buy
 * action and cBERewardInterface_awardUser (unpaid, for item awards) call it.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @param key  the bolt id
 * @param flag  non-zero to charge the price
 */
void cBEBoltInterface::buyBolt(int profileIndex, int characterId, int key, int flag)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 11);
    char* charRecord = (char*)&cBELibrary_profileLib[profileIndex][characterId];
    cBECharProfileDB_setBoltBought(charRecord, key, 1);
    tBEBoltDef* boltDef = cBEBoltInterface_getBoltDB_0()->getCharBolts(characterId);
    int n = cBEBoltInterface_getBoltDB_0()->getNumCharBolts(characterId);
    for (int j = 0; j < n; j++, boltDef++) {
        if (scale10_14B560(boltDef->price) == -key) {
            cBECharProfileDB_setBoltBought(charRecord, boltDef->id, 1);
        }
    }
    cBEBoltDB* boltDB = cBEBoltInterface_getBoltDB_0();
    boltDef = entry_14B560(boltDB, boltDB->findBoltIndex(characterId, key));
    if (flag) {
        cBEEconInterface_reduceMoney(iface, profileIndex, characterId, scale10_14B560(boltDef->price));
    } else {
        cBEEconInterface_getMoney(iface, profileIndex, characterId);
    }
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getBigIconFileName);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern char str_data_char_mobytxp_big_moby_icons_ssh[];
extern char str_data_char_kaoritxp_big_kaori_icons_ssh[];
extern char str_data_char_allegtxp_big_arielle_icons_ssh[];
extern char str_data_char_mactxp_big_mac_icons_ssh[];
extern char str_data_char_zoetxp_big_zoe_icons_ssh[];
extern char str_data_char_grifftxp_big_grommet_icons_ssh[];
extern char str_data_char_elisetxp_big_elise_icons_ssh[];
extern char str_data_char_natetxp_big_rocco_icons_ssh[];
extern char str_data_char_psymotxp_big_psymon_icons_ssh[];
extern char str_data_char_viggotxp_big_deiter_icons_ssh[];
/**
 * Returns the file name of a character's big icon texture, e.g. "data/char/mobytxp/big_moby_icons.ssh" for character 0.
 *
 * Characters 0-9 map to the moby, kaori, alleg (arielle), mac, zoe, griff (grommet), elise, nate (rocco), psymo
 * (psymon) and viggo (deiter) files. The equip screen builds its icon file names from it.
 *
 * @param x  character ID (eBECharacters), 0 to 9
 * @return the file name, or null for other IDs
 */
char* cBEBoltInterface::getBigIconFileName(unsigned int x)
{
    switch (x) {
    case kCHAR_MOBY: return str_data_char_mobytxp_big_moby_icons_ssh;
    case kCHAR_KAORI: return str_data_char_kaoritxp_big_kaori_icons_ssh;
    case kCHAR_ALLEGRA: return str_data_char_allegtxp_big_arielle_icons_ssh;
    case kCHAR_MAC: return str_data_char_mactxp_big_mac_icons_ssh;
    case kCHAR_ZOE: return str_data_char_zoetxp_big_zoe_icons_ssh;
    case kCHAR_GRIFF: return str_data_char_grifftxp_big_grommet_icons_ssh;
    case kCHAR_ELISE: return str_data_char_elisetxp_big_elise_icons_ssh;
    case kCHAR_NATE: return str_data_char_natetxp_big_rocco_icons_ssh;
    case kCHAR_PSYMON: return str_data_char_psymotxp_big_psymon_icons_ssh;
    case kCHAR_VIGGO: return str_data_char_viggotxp_big_deiter_icons_ssh;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getVisibleModelFiles);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int strcmp(void* a, void* b);

/**
 * Lists the distinct model file names of a character's equipped bolts, 256 bytes per name, and returns how many.
 *
 * For each of the character's bolt definitions with a file name (+0x2C) whose entry has flag 0x10, builds the name with
 * cBEBoltInterface_buildBoltModelFileName into the next 256-byte slot of @p out; a name equal (strcmp) to an earlier one is dropped.
 *
 * @param profile  profile index
 * @param ch  character ID
 * @param out  receives the names, 256 bytes each
 * @return the number of names
 */
int cBEBoltInterface::getVisibleModelFiles(int profile, int ch, char* out)
{
    int cnt = 0;
    cBEBoltDB* tables = cBEBoltInterface_getBoltDB_0();
    cBECharProfileDB* prof = (cBECharProfileDB*)getCharProfile(profile, ch);
    tBEBoltDef* def = tables->getCharBolts(ch);
    int n = tables->getNumCharBolts(ch);
    int i;
    for (i = 0; i < n; i++, def++) {
        if (def->modelFileName != 0) {
            tBEProfileBolt* flagEntry = getEntry_14AFB0(prof, def->id);
            if (flagEntry->flags & 0x10) {
                char* dst = out + (cnt << 8);
                if (buildBoltModelFileName(profile, ch, def, dst)) {
                    int prev = cnt - 1;
                    int k;
                    for (k = prev; k >= 0; k--) {
                        if (strcmp(dst, out + (k << 8)) == 0) {
                            cnt = prev;
                            break;
                        }
                    }
                    cnt++;
                }
            }
        }
    }
    return cnt;
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getRiderVisibleModelFiles);
#ifdef SKIP_ASM
extern "C" int cBEBoltInterface_getVisibleModelFiles(cBEBoltInterface* self, int profile, int ch, char* out);

/**
 * Lists the distinct model file names of a race rider's equipped bolts (cBEBoltInterface_getVisibleModelFiles for its profile and character).
 *
 * Nothing calls it.
 *
 * @param rider  race rider slot
 * @param out  receives the names, 256 bytes each
 * @return the number of names
 */
int cBEBoltInterface::getRiderVisibleModelFiles(int rider, char* out)
{
    int profile = cBEBoltInterface_getProfileIndex(rider);
    return getVisibleModelFiles(profile, cBEBoltInterface_getCharacterID(rider), out);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_buildBoltModelFileName);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strchr(const char* s, int c);

/**
 * Builds the file name of one bolt for a character, filling its '$' wildcards from the other equipped bolts of the same
 * group.
 *
 * Copies the definition's name (+0x2C) to @p out; a definition with group (+0x3) kNoBoltGroup (-1) or no name gives an
 * empty string and 0. When the name has '$' characters, walks the character's bolts of the same group that have a name,
 * another id (+0x4) and flag 0x10 in the character record (cBELibrary_profileLib + profile * 0x9B50 + character *
 * 0xF88), and copies each one's characters into the '$' positions before the '.' until none is left. The rider loader
 * and the AI rider code use it to queue model files.
 *
 * @param profileIndex  profile index
 * @param characterId  character ID
 * @param boltDef  the bolt definition (0x38 bytes)
 * @param out  receives the file name
 * @return true when a name was written
 */
bool cBEBoltInterface::buildBoltModelFileName(int profileIndex, int characterId, tBEBoltDef* boltDef, char* out)
{
    signed char grp = boltDef->modelGroup;
    if (grp == kNoBoltGroup || boltDef->modelFileName == 0) {
        *out = 0;
        return false;
    }
    cBEBoltDB* tbl = &cBELibrary_boltDB;
    cBEBoltInterface* boltInterface = (cBEBoltInterface*)cBE_getInterface_Fv(cBE_getBE(), kInt_Bolt);
    cBECharProfileDB* prof = (cBECharProfileDB*)boltInterface->getCharProfile(profileIndex, characterId);
    strcpy(out, boltDef->modelFileName);
    char* wildcard = strchr(out, '$');
    if (wildcard == 0) return true;
    int n = tbl->getNumCharBolts(characterId);
    tBEBoltDef* otherDef = tbl->getCharBolts(characterId);
    for (int i = 0; i < n; i++, otherDef++) {
        if (otherDef->modelGroup != grp) continue;
        char* otherName = otherDef->modelFileName;
        if (otherName == 0) continue;
        if (otherDef->id == boltDef->id) continue;
        if (!(getEntry_14AFB0(prof, otherDef->id)->flags & 0x10)) continue;
        char* cursor = wildcard;
        int k = wildcard - out;
        while (*cursor != 0 && *cursor != '.') {
            if (otherName[k] != '$' && *cursor == '$') *cursor = otherName[k];
            cursor++;
            k++;
        }
        while (*wildcard != 0 && *wildcard != '$' && *wildcard != '.') wildcard++;
        if (*wildcard != '$') return true;
    }
    return true;
}
#endif

extern "C" void* cBEBoltDB_load(cBEBoltDB*);

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_buildRiderBoltModelFileName);
#ifdef SKIP_ASM
/**
 * Builds the file name of one bolt for a race rider's character (cBEBoltInterface_buildBoltModelFileName for its profile and character).
 *
 * Nothing calls it.
 *
 * @param rider  race rider slot
 * @param def  the bolt definition
 * @param out  receives the file name
 * @return 1 when a name was written, 0 for none
 */
int cBEBoltInterface::buildRiderBoltModelFileName(int rider, tBEBoltDef* def, char* out)
{
    int profile = cBEBoltInterface_getProfileIndex(rider);
    return buildBoltModelFileName(profile, cBEBoltInterface_getCharacterID(rider), def, out);
}
#endif
//100%
INCLUDE_ASM("be/belibrary", func_0014BC18);
#ifdef SKIP_ASM
extern "C" int strcmp(void* a, void* b);
extern "C" char* strcpy(char* dst, const char* src);

struct sBEEntry38_BC18 {
    char pad_0x00[4];
    short slot;         // 0x4
    char pad_0x06[0x22];
    char* name;         // 0x28
    char pad_0x2C[0xC];
};

/**
 * Lists the distinct second names (+0x28) of a character's equipped bolts, 256 bytes per name, and returns how many.
 *
 * For each of the character's bolt definitions with a name at +0x28 whose entry has flag 0x10, copies the name into the
 * next 256-byte slot of @p out unless an earlier slot holds the same string (strcmp). Nothing calls it.
 *
 * @param profile  profile index
 * @param ch  character ID
 * @param out  receives the names, 256 bytes each
 * @return the number of names
 */
extern "C" int func_0014BC18(cBEBoltInterface* self, int profile, int ch, char* out)
{
    int cnt = 0;
    cBEBoltDB* tables = cBEBoltInterface_getBoltDB_0();
    cBECharProfileDB* prof = (cBECharProfileDB*)self->getCharProfile(profile, ch);
    int n = tables->getNumCharBolts(ch);
    sBEEntry38_BC18* def = (sBEEntry38_BC18*)tables->getCharBolts(ch);
    int i;
    for (i = 0; i < n; i++, def++) {
        char* name = def->name;
        if (name != 0) {
            tBEProfileBolt* flagEntry = getEntry_14AFB0(prof, def->slot);
            if (flagEntry->flags & 0x10) {
                int k;
                for (k = cnt - 1; k >= 0; k--) {
                    if (strcmp(name, out + (k << 8)) == 0)
                        break;
                }
                if (k < 0) {
                    strcpy(out + (cnt << 8), name);
                    cnt++;
                }
            }
        }
    }
    return cnt;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_loadBoltDB__FPv);
#ifdef SKIP_ASM
/**
 * Loads the bolt database (cBELibrary_boltDB) unless it is already loaded and rebuilds its indexes (cBEBoltDB_load).
 *
 * The front end's load sequence (fereal) calls it on the bolt interface before cBEBoltInterface_restoreVisibleFlags.
 *
 * @return no meaningful value (cBEBoltDB_load sets none)
 */
void* cBEBoltInterface::loadBoltDB()
{
    return cBELibrary_boltDB.load();
}
#endif

extern "C" void* cBEBoltDB_refreshVisibleBolts(void*);

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_refreshVisibleBolts__FPv);
#ifdef SKIP_ASM
/**
 * Re-derives every character's equipped bolt flags and rebuilds the bolt database's indexes (cBEBoltDB_refreshVisibleBolts on
 * cBELibrary_boltDB).
 *
 * Called from the game loader (gameload).
 *
 * @return no meaningful value (cBEBoltDB_refreshVisibleBolts returns nothing)
 */
void* cBEBoltInterface::refreshVisibleBolts()
{
    return cBEBoltDB_refreshVisibleBolts(&cBELibrary_boltDB);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_getBoltDB__FPv);
#ifdef SKIP_ASM
/**
 * Returns the bolt (equipment item) database, cBELibrary_boltDB.
 *
 * Its 0x38-byte bolt definitions are grouped by character (cBEBoltDB_getNumCharBolts count, cBEBoltDB_getCharBolts first). The argument is
 * not read.
 *
 * @return the bolt database
 */
void* cBEBoltInterface_getBoltDB(void* self)
{
    return &cBELibrary_boltDB;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_setBoltModelPart);
#ifdef SKIP_ASM
extern "C" void cBEBoltDB_setBoltModelPart(cBEBoltDB* db, int groupId, int boltId, int value);

/**
 * Sets byte +0x10 of a bolt definition in the bolt database cBELibrary_boltDB (cBEBoltDB_setBoltModelPart).
 *
 * Nothing calls it.
 *
 * @param groupId  the group (a character ID)
 * @param boltId  the bolt id
 * @param value  the new value
 */
extern "C" void cBEBoltInterface_setBoltModelPart(int groupId, int boltId, int value) {
    cBELibrary_boltDB.setBoltModelPart(groupId, boltId, value);
}
#endif
//100%
INCLUDE_ASM("be/belibrary", func_0014BE00);
#ifdef SKIP_ASM
extern "C" void func_0014C590(cBEBoltDB* db, int groupId, int boltId, int value);

/**
 * Sets byte +0x1 of a bolt definition in the bolt database cBELibrary_boltDB (func_0014C590).
 *
 * Nothing calls it.
 *
 * @param groupId  the group (a character ID)
 * @param boltId  the bolt id
 * @param value  the new value
 */
extern "C" void func_0014BE00(int groupId, int boltId, int value) {
    func_0014C590(&cBELibrary_boltDB, groupId, boltId, value);
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltInterface_setBoltEquipPoints);
#ifdef SKIP_ASM
extern "C" void cBEBoltDB_setBoltEquipPoints(cBEBoltDB* db, int groupId, int boltId, int value);

/**
 * Sets short +0xC (equip points) of a bolt definition in the bolt database cBELibrary_boltDB (cBEBoltDB_setBoltEquipPoints).
 *
 * Nothing calls it.
 *
 * @param groupId  the group (a character ID)
 * @param boltId  the bolt id
 * @param value  the new value
 */
extern "C" void cBEBoltInterface_setBoltEquipPoints(int groupId, int boltId, int value) {
    cBELibrary_boltDB.setBoltEquipPoints(groupId, boltId, value);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_loadFile);
#ifdef SKIP_ASM
extern "C" int BXFILE_sizez(const char* name);
extern "C" int FILE_loadatz(const char* name, void* buf, int size);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char* cBEBoltDB_fileName;
extern int kBolt_Num;
extern const char str_BoltsFile[];

/**
 * Loads the bolts file and sets up the bolt database's tables and string pointers.
 *
 * Reads the whole file named by cBEBoltDB_fileName (BXFILE_sizez, FILE_loadatz) into a "BoltsFile" block kept at +0x0. The file
 * holds the id count (stored in kBolt_Num, rounded up to even), then four counted tables: the 0x38-byte bolt
 * definitions (+0x4, count +0x1C), 12-byte records (+0x8, +0x20), 8-byte records (+0xC, +0x24) and 8-byte records
 * (+0x10, +0x28), then one word and the string pool (+0x14).
 *
 * Every non-zero string field of a definition (+0x14, the four at +0x18, +0x28, +0x2C, +0x30) is an offset + 1 into the
 * pool and becomes a pointer. cBEBoltDB_load calls it.
 *
 */
void cBEBoltDB::loadFile()
{
    int size = BXFILE_sizez(cBEBoltDB_fileName);
    char* buf = new (str_BoltsFile, 0, 0) char[size];
    mpBoltsFile = buf;
    FILE_loadatz(cBEBoltDB_fileName, buf, size);
    char* fileData = mpBoltsFile;
    kBolt_Num = *(int*)fileData;
    int numBolts = *(int*)(fileData + 4);
    mpCharBolts = (tBEBoltDef*)(fileData + 8);
    mTotalNumCharBolts = numBolts;
    int off = 8 + numBolts * 0x38;
    int numLinks = *(int*)(fileData + off);
    off += 4;
    mpCharRules = (tBEBoltRule*)(fileData + off);
    mTotalNumCharRules = numLinks;
    off += numLinks * 0xC;
    int numPairs = *(int*)(fileData + off);
    off += 4;
    mpCharPairs = (tBEBoltPair*)(fileData + off);
    mTotalNumCharPairs = numPairs;
    off += numPairs * 8;
    int numGroups = *(int*)(fileData + off);
    off += 4;
    mpCharGroups = (tBEBoltGroup*)(fileData + off);
    mTotalNumCharGroups = numGroups;
    off += numGroups * 8;
    off += 4;
    mpStringPool = fileData + off;
    if (kBolt_Num & 1) kBolt_Num++;
    for (int i = 0; i < mTotalNumCharBolts; i++) {
        if (mpCharBolts[i].name) mpCharBolts[i].name = mpStringPool + ((int)mpCharBolts[i].name - 1);
        if (mpCharBolts[i].modelPartFile)
            mpCharBolts[i].modelPartFile = mpStringPool + ((int)mpCharBolts[i].modelPartFile - 1);
        if (mpCharBolts[i].modelFileName)
            mpCharBolts[i].modelFileName = mpStringPool + ((int)mpCharBolts[i].modelFileName - 1);
        if (mpCharBolts[i].pictureName)
            mpCharBolts[i].pictureName = mpStringPool + ((int)mpCharBolts[i].pictureName - 1);
        for (int j = 0; j < 4; j++) {
            if (mpCharBolts[i].lodPartNames[j])
                mpCharBolts[i].lodPartNames[j] = mpStringPool + ((int)mpCharBolts[i].lodPartNames[j] - 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_buildChildTables);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);
extern char str_child_bolts_data[];

/**
 * Builds the bolt database's per-group child tables: id to definition index, and each parent's child count and first
 * child.
 *
 * Allocates a "child bolts data" block of 90 * kBolt_Num shorts (kBolt_Num is the number of bolt ids) and carves 30
 * count tables (+0x11C), 30 first-child tables (+0x194) and 30 index tables (+0x4DC) out of it, one per group. The
 * index tables map each definition's id (+0x4) to its index; the count and first tables are keyed by the parent id
 * (+0x6, kNoBoltID for none). The run tracker that sets the first child starts at kBolt_Num instead of -1 and is shared
 * across groups. Nothing in the C calls it.
 */
void cBEBoltDB::buildChildTables()
{
    int last = kBolt_Num;
    short* cursor;

    mpChildBoltsData = new (str_child_bolts_data, 0, 0) short[last * 90];
    cursor = mpChildBoltsData;
    for (int i = 0; i < 30; i++) {
        mpNumChildBolts[i] = cursor;
        cursor += kBolt_Num;
    }
    for (int i = 0; i < 30; i++) {
        mpFirstChildBolt[i] = cursor;
        cursor += kBolt_Num;
    }
    for (int i = 0; i < 30; i++) {
        mpBoltIndex[i] = cursor;
        cursor += kBolt_Num;
        MEM_fill(mpBoltIndex[i], 0, kBolt_Num * 2);
    }
    for (int i = 0; i < mTotalNumCharBolts; i++) {
        mpBoltIndex[mpCharBolts[i].charID][mpCharBolts[i].id] = i;
    }
    for (int i = 0; i < 30; i++) {
        MEM_fill(mpFirstChildBolt[i], 0, kBolt_Num * 2);
        MEM_fill(mpNumChildBolts[i], 0, kBolt_Num * 2);
    }
    for (int i = 0; i < mTotalNumCharBolts; i++) {
        int parentId = mpCharBolts[i].parentId;
        int charGroup = mpCharBolts[i].charID;
        if (parentId != kNoBoltID) {
            mpNumChildBolts[charGroup][parentId]++;
            if (last != parentId) {
                last = parentId;
                mpFirstChildBolt[charGroup][parentId] = i;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_indexCharBolts);
#ifdef SKIP_ASM
/**
 * Counts the bolt definitions per group and records each group's first index (+0x2C and +0xA4, by byte +0x0).
 *
 * Assumes the definitions are sorted by group. Unlike its three siblings it does not clear the counts first;
 * cBEBoltDB_freeData clears them before the first load.
 */
void cBEBoltDB::indexCharBolts()
{
    int last = -1;
    int i;
    for (i = 0; i < mTotalNumCharBolts; i++) {
        int groupId = mpCharBolts[i].charID;
        mNumCharBolts[groupId]++;
        if (last != groupId) {
            last = groupId;
            mCharBoltsStart[groupId] = i;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_indexRules);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Counts the bolt database's 12-byte records (+0x8, count +0x20) per group and records each group's first index (+0x20C
 * and +0x284, by byte +0x0).
 *
 * Clears the counts first; assumes the records are sorted by group.
 */
void cBEBoltDB::indexRules()
{
    int last = 30;
    int i;
    MEM_fill(mNumCharRules, 0, sizeof(mNumCharRules));
    for (i = 0; i < mTotalNumCharRules; i++) {
        int groupId = mpCharRules[i].group;
        mNumCharRules[groupId]++;
        if (last != groupId) {
            last = groupId;
            mCharRulesStart[groupId] = i;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_indexPairs);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Counts the bolt database's 8-byte records at +0xC (count +0x24) per group and records each group's first index
 * (+0x2FC and +0x374, by byte +0x0).
 *
 * Clears both tables first; assumes the records are sorted by group.
 */
void cBEBoltDB::indexPairs()
{
    int last = -1;
    int i;
    MEM_fill(mNumCharPairs, 0, sizeof(mNumCharPairs));
    MEM_fill(mCharPairsStart, 0, sizeof(mCharPairsStart));
    for (i = 0; i < mTotalNumCharPairs; i++) {
        int groupId = mpCharPairs[i].group;
        mNumCharPairs[groupId]++;
        if (last != groupId) {
            last = groupId;
            mCharPairsStart[groupId] = i;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_indexGroups);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* dst, int c, int n);

/**
 * Counts the bolt database's 8-byte records at +0x10 (count +0x28) per group and records each group's first index
 * (+0x3EC and +0x464, by byte +0x0).
 *
 * Clears both tables first; assumes the records are sorted by group.
 */
void cBEBoltDB::indexGroups()
{
    int last = -1;
    int i;
    MEM_fill(mNumCharGroups, 0, sizeof(mNumCharGroups));
    MEM_fill(mCharGroupsStart, 0, sizeof(mCharGroupsStart));
    for (i = 0; i < mTotalNumCharGroups; i++) {
        int groupId = mpCharGroups[i].group;
        mNumCharGroups[groupId]++;
        if (last != groupId) {
            last = groupId;
            mCharGroupsStart[groupId] = i;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_setBoltModelPart);
#ifdef SKIP_ASM
/**
 * Sets byte +0x10 of the bolt definition with id @p key in group @p groupId (found with cBEBoltDB_findBoltIndex).
 *
 * No check for a missing bolt. Only cBEBoltInterface_setBoltModelPart calls it.
 *
 * @param groupId  the group (a character ID)
 * @param key  the bolt id
 * @param value  the new value
 */
void cBEBoltDB::setBoltModelPart(int groupId, int key, int value)
{
    mpCharBolts[findBoltIndex(groupId, key)].modelPart = value;
}
#endif
//100%
INCLUDE_ASM("be/belibrary", func_0014C590);
#ifdef SKIP_ASM
/**
 * Sets byte +0x1 of the bolt definition with id @p key in group @p groupId (found with cBEBoltDB_findBoltIndex).
 *
 * No check for a missing bolt. Only func_0014BE00 calls it.
 *
 * @param groupId  the group (a character ID)
 * @param key  the bolt id
 * @param value  the new value
 */
extern "C" void func_0014C590(cBEBoltDB* self, int groupId, int key, int value)
{
    self->mpCharBolts[self->findBoltIndex(groupId, key)].variant = value;
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_setBoltEquipPoints);
#ifdef SKIP_ASM
/**
 * Sets short +0xC (equip points) of the bolt definition with id @p key in group @p groupId (found with cBEBoltDB_findBoltIndex).
 *
 * No check for a missing bolt. Only cBEBoltInterface_setBoltEquipPoints calls it.
 *
 * @param groupId  the group (a character ID)
 * @param key  the bolt id
 * @param value  the new value
 */
void cBEBoltDB::setBoltEquipPoints(int groupId, int key, int value)
{
    mpCharBolts[findBoltIndex(groupId, key)].equipPoints = value;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_cBEBoltDB);
#ifdef SKIP_ASM
/**
 * Constructs an empty bolt database: clears its pointers and counts (+0x0 to +0x28).
 *
 * Nothing calls it.
 *
 * @return @p self
 */
extern "C" cBEBoltDB* cBEBoltDB_cBEBoltDB(cBEBoltDB* self)
{
    self->mpBoltsFile = 0;
    self->mpCharBolts = 0;
    self->mpCharRules = 0;
    self->mpCharPairs = 0;
    self->mpCharGroups = 0;
    self->mpStringPool = 0;
    self->mpChildBoltsData = 0;
    self->mTotalNumCharBolts = 0;
    self->mTotalNumCharRules = 0;
    self->mTotalNumCharPairs = 0;
    self->mTotalNumCharGroups = 0;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB__cBEBoltDB);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cBEBoltDB_freeData(cBEBoltDB* self);

/**
 * Destroys a bolt database: frees its data with cBEBoltDB_freeData and deletes the object when bit 0 of @p flags is set.
 *
 * Nothing calls it.
 *
 * @param flags  destructor flags; bit 0 frees the object
 */
extern "C" void cBEBoltDB__cBEBoltDB(cBEBoltDB* self, int flags)
{
    self->freeData();
    if (flags & 1)
    {
        operator_delete(((int*)self));
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/belibrary", cBEBoltDB_load);
#ifdef SKIP_ASM
extern "C" void cBEBoltDB_loadFile(cBEBoltDB* self);

/**
 * Loads the bolt database unless it is already loaded (+0x10 set), then rebuilds its group indexes.
 *
 * Frees any old data (cBEBoltDB_freeData), loads the bolts file (cBEBoltDB_loadFile) and runs cBEBoltDB_indexCharBolts, cBEBoltDB_indexRules,
 * cBEBoltDB_indexPairs and cBEBoltDB_indexGroups. When definitions were already present (+0x4 non-null before), it then copies flag
 * 0x4 into flag 0x10 of every bolt entry of profiles 0-2, characters 0-9, as cBEBoltInterface_restoreVisibleFlags does. cBELibrary_loadData
 * and cBEBoltInterface_loadBoltDB call it.
 *
 * @return no meaningful value; the body sets none
 */
void* cBEBoltDB::load()
{
    if (mpCharGroups == 0) {
        int none = mpCharBolts == 0;
        freeData();
        loadFile();
        indexCharBolts();
        indexRules();
        indexPairs();
        indexGroups();
        if (!none) {
            int profileIndex;
            int characterId;
            for (profileIndex = 0; profileIndex < 3; profileIndex++) {
                for (characterId = 0; characterId < 10; characterId++) {
                    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
                    int count = charRecord->mNumBolts;
                    sBEFlagEntry* flagEntry = (sBEFlagEntry*)(&charRecord->mBolts[0]);
                    int i;
                    for (i = 0; i < count; i++, flagEntry++) {
                        if (flagEntry->flags & 4) {
                            flagEntry->flags |= 0x10;
                        } else {
                            flagEntry->flags &= ~0x10;
                        }
                    }
                }
            }
        }
    }
}
#endif

INCLUDE_ASM("be/belibrary", func_0014C7A0);

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/belibrary", cBEBoltDB_refreshVisibleBolts);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void func_0014C7A0(void* self);
int cBEBoltDB_getNumCharPairs(void* self, int i);
extern "C" tBEBoltPair* cBEBoltDB_getCharPairs(cBEBoltDB* self, int i);


/**
 * Re-derives the equipped flags of every character's bolts and rebuilds the bolt database's indexes.
 *
 * For profiles 0-2 and characters 0-9: sets flag 0x10 on every bolt entry that has flag 0x4; for each of the
 * character's 8-byte records at +0xC (cBEBoltDB_getCharPairs, count cBEBoltDB_getNumCharPairs), when the bolt in its first short (+0x4) has
 * flag 0x4, sets 0x10 on the bolt in its second (+0x6) unless that is -1; then sets 0x10 on bolt 4. Finally calls
 * func_0014C7A0 and the four group builders (cBEBoltDB_indexCharBolts, cBEBoltDB_indexRules, cBEBoltDB_indexPairs, cBEBoltDB_indexGroups). The game
 * loader calls it through cBEBoltInterface_refreshVisibleBolts.
 */
void cBEBoltDB::refreshVisibleBolts()
{
    int profileIndex;
    int characterId;
    for (profileIndex = 0; profileIndex < 3; profileIndex++) {
        for (characterId = 0; characterId < 10; characterId++) {
            tBEBoltPair* req = getCharPairs(characterId);
            int n = getNumCharPairs(characterId);
            cBECharProfileDB* charRecord = &cBELibrary_profileLib[profileIndex][characterId];
            int i;
            int count = charRecord->mNumBolts;
            tBEProfileBolt* flagEntry = charRecord->mBolts;
            for (i = 0; i < count; i++, flagEntry++) {
                if (flagEntry->flags & 4)
                    flagEntry->flags |= 0x10;
            }
            for (i = 0; i < n; i++, req++) {
                flagEntry = getEntry_14AFB0(charRecord, req->bolts[0]);
                if (flagEntry->flags & 4) {
                    if (req->bolts[1] >= 0)
                        getEntry_14AFB0(charRecord, req->bolts[1])->flags |= 0x10;
                }
            }
            flagEntry = getEntry_14AFB0(charRecord, 4);
            flagEntry->flags |= 0x10;
        }
    }
    func_0014C7A0(this);
    indexCharBolts();
    indexRules();
    indexPairs();
    indexGroups();
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_freeData);
#ifdef SKIP_ASM
void cMemMan_free(void* p);

/**
 * Frees the bolt database's data and clears its tables.
 *
 * Frees the file block (+0x0) and the child table block (+0x18) with cMemMan_free, zeroes the pointers and counts at
 * +0x4 to +0x28 and the eleven 30-word tables from +0x2C.
 */
void cBEBoltDB::freeData()
{
    if (mpBoltsFile)
        cMemMan_free(mpBoltsFile);
    mpBoltsFile = 0;
    mpCharBolts = 0;
    mpCharRules = 0;
    mpCharPairs = 0;
    mpCharGroups = 0;
    mpStringPool = 0;
    mTotalNumCharBolts = 0;
    mTotalNumCharRules = 0;
    mTotalNumCharPairs = 0;
    mTotalNumCharGroups = 0;
    MEM_fill(mNumCharBolts, 0, sizeof(mNumCharBolts));
    MEM_fill(mCharBoltsStart, 0, sizeof(mCharBoltsStart));
    MEM_fill(mpNumChildBolts, 0, sizeof(mpNumChildBolts));
    MEM_fill(mpFirstChildBolt, 0, sizeof(mpFirstChildBolt));
    MEM_fill(mNumCharRules, 0, sizeof(mNumCharRules));
    MEM_fill(mCharRulesStart, 0, sizeof(mCharRulesStart));
    MEM_fill(mNumCharPairs, 0, sizeof(mNumCharPairs));
    MEM_fill(mCharPairsStart, 0, sizeof(mCharPairsStart));
    MEM_fill(mNumCharGroups, 0, sizeof(mNumCharGroups));
    MEM_fill(mCharGroupsStart, 0, sizeof(mCharGroupsStart));
    MEM_fill(mpBoltIndex, 0, sizeof(mpBoltIndex));
    if (mpChildBoltsData)
        cMemMan_free(mpChildBoltsData);
    mpChildBoltsData = 0;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getBoltDefIndex);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
/**
 * Returns the index of a bolt definition in the bolt database.
 *
 * A pointer into the definition table gives its index directly. Otherwise searches the definition's group (byte +0x0)
 * for one with the same id (+0x4), byte +0x1 and words +0x28, +0x18 and +0x2C. Nothing calls it.
 *
 * @param item  the bolt definition (0x38 bytes)
 * @return the index, or -1 when not found
 */
int cBEBoltDB::getBoltDefIndex(tBEBoltDef* item)
{
    tBEBoltDef* items = mpCharBolts;
    int idx = item - items;
    if (idx < 0 || idx >= mTotalNumCharBolts) {
        int start = mCharBoltsStart[item->charID];
        int end = start + mNumCharBolts[item->charID];
        int i = start;
        if (i < end) {
            tBEBoltDef* candidate = &items[start];
            do {
                if (candidate->id == item->id && candidate->variant == item->variant &&
                    candidate->modelPartFile == item->modelPartFile &&
                    candidate->lodPartNames[0] == item->lodPartNames[0] &&
                    candidate->modelFileName == item->modelFileName) {
                    idx = i;
                    goto done;
                }
                i++;
                candidate++;
            } while (i < end);
        }
        idx = -1;
    }
done:
    return idx;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_findBoltIndex);
#ifdef SKIP_ASM
/**
 * Returns the index of the bolt definition with id @p key in group @p groupId.
 *
 * Uses the group's id-to-index table (+0x4DC, built by cBEBoltDB_buildChildTables) when it exists, else searches the group (+0x2C
 * count, +0xA4 first) for a matching id (+0x4).
 *
 * @param groupId  the group (a character ID)
 * @param key  the bolt id
 * @return the index, or -1 when the search finds none
 */
int cBEBoltDB::findBoltIndex(int groupId, int key)
{
    short* lookup = mpBoltIndex[groupId];
    if (lookup == 0) {
        int i = mCharBoltsStart[groupId];
        int end = i + mNumCharBolts[groupId];
        tBEBoltDef* boltDef = &mpCharBolts[i];
        for (; i < end; i++, boltDef++) {
            if (boltDef->id == key) {
                return i;
            }
        }
        return -1;
    }
    return lookup[key];
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_countChildBolts);
#ifdef SKIP_ASM
/**
 * Returns how many bolt definitions in group @p groupId have bolt @p key as their parent (short +0x6).
 *
 * Uses the group's child count table (+0x11C) when it exists, else counts in the group, stopping at the first larger
 * parent id (the definitions are sorted by parent).
 *
 * @param groupId  the group (a character ID)
 * @param key  the parent bolt id
 * @return the number of children
 */
int cBEBoltDB::countChildBolts(int groupId, int key)
{
    short* lookup = mpNumChildBolts[groupId];
    if (lookup == 0) {
        int n = 0;
        int i = mCharBoltsStart[groupId];
        int end = i + mNumCharBolts[groupId];
        tBEBoltDef* boltDef = &mpCharBolts[i];
        for (; i < end; boltDef++, i++) {
            if (boltDef->parentId == key) {
                n++;
            } else if (boltDef->parentId > key) {
                break;
            }
        }
        return n;
    }
    return lookup[key];
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getFirstChildBolt);
#ifdef SKIP_ASM
/**
 * Returns the first bolt definition in group @p groupId whose parent (short +0x6) is @p key.
 *
 * For @p key kNoBoltID (-1), or without the group's first-child table (+0x194), searches the group. Otherwise returns
 * null when the child count (+0x11C) is 0 or less, else the definition the first-child table names.
 *
 * @param groupId  the group (a character ID)
 * @param key  the parent bolt id, or kNoBoltID
 * @return the definition, or null
 */
tBEBoltDef* cBEBoltDB::getFirstChildBolt(int groupId, int key)
{
    short* lookup;
    if (key == kNoBoltID || (lookup = mpFirstChildBolt[groupId]) == 0) {
        int i = mCharBoltsStart[groupId];
        int end = i + mNumCharBolts[groupId];
        tBEBoltDef* boltDef = &mpCharBolts[i];
        for (; i < end; i++, boltDef++) {
            if (boltDef->parentId == key) {
                return boltDef;
            }
        }
        return 0;
    }
    if (mpNumChildBolts[groupId][key] <= 0) {
        return 0;
    }
    return &mpCharBolts[lookup[key]];
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_collectChildBolts);
#ifdef SKIP_ASM
/**
 * Collects the bolt definitions under bolt @p key in group @p groupId into @p out and returns how many.
 *
 * Takes every definition whose parent (short +0x6) is @p key and, while @p depth is not 0, recurses into its children
 * (by its id, +0x4) with depth - 1; a definition with flag 0x20 (+0x34) is only descended into when @p all is set. Uses
 * the child tables (cBEBoltDB_getFirstChildBolt, +0x11C) when present and @p key is not kNoBoltID, else searches the group, stopping
 * at a larger parent id. beintecon passes depth -1 (no limit) and all 1.
 *
 * @param groupId  the group (a character ID)
 * @param key  the parent bolt id, or kNoBoltID for the top level
 * @param out  receives the definition pointers
 * @param depth  levels to descend; negative for no limit
 * @param all  non-zero to descend into flag-0x20 definitions too
 * @return the number of pointers written
 */
int cBEBoltDB::collectChildBolts(int groupId, int key, tBEBoltDef** out, int depth, int all)
{
    int count = 0;
    if (key == kNoBoltID || mpNumChildBolts[groupId] == 0) {
        int i = mCharBoltsStart[groupId];
        int end = i + mNumCharBolts[groupId];
        tBEBoltDef* boltDef = &mpCharBolts[i];
        for (; i < end; i++, boltDef++) {
            if (boltDef->parentId == key) {
                *out++ = boltDef;
                count++;
                if (depth != 0) {
                    if (!(boltDef->flags & 0x20) || all) {
                        int childCount = collectChildBolts(groupId, boltDef->id, out, depth - 1, all);
                        count += childCount;
                        out += childCount;
                    }
                }
            } else if (key < boltDef->parentId) {
                return count;
            }
        }
    } else {
        tBEBoltDef* boltDef = getFirstChildBolt(groupId, key);
        int n = mpNumChildBolts[groupId][key];
        for (int j = 0; j < n; j++, boltDef++) {
            *out++ = boltDef;
            count++;
            if (depth != 0) {
                if (!(boltDef->flags & 0x20) || all) {
                    int childCount = collectChildBolts(groupId, boltDef->id, out, depth - 1, all);
                    count += childCount;
                    out += childCount;
                }
            }
        }
    }
    return count;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_collectCategoryBolts);
#ifdef SKIP_ASM
/**
 * Collects the bolt definitions under @p key in group @p groupId by the short at +0x8 into @p out and returns how many.
 *
 * Takes every definition of the group whose short +0x8 equals @p key and, while @p depth is not 0, recurses with its id
 * (+0x4) and depth - 1; one with flag 0x20 (+0x34) is only descended into when @p all is set. Unlike cBEBoltDB_collectChildBolts it
 * always scans the whole group. The equip screen lists a category's items with it (depth -1).
 *
 * @param groupId  the group (a character ID)
 * @param key  the category id to list
 * @param out  receives the definition pointers
 * @param depth  levels to descend; negative for no limit
 * @param all  non-zero to descend into flag-0x20 definitions too
 * @return the number of pointers written
 */
int cBEBoltDB::collectCategoryBolts(int groupId, int key, tBEBoltDef** out, int depth, int all)
{
    int n = 0;
    int i = mCharBoltsStart[groupId];
    int end = i + mNumCharBolts[groupId];
    tBEBoltDef* node = &mpCharBolts[i];
    for (; i < end; i++, node++) {
        if (node->categoryId == key) {
            *out++ = node;
            n++;
            if (depth != 0 && (!(node->flags & 0x20) || all)) {
                int k = collectCategoryBolts(groupId, node->id, out, depth - 1, all);
                n += k;
                out += k;
            }
        }
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_isBoltAncestor);
#ifdef SKIP_ASM
/**
 * Returns whether bolt @p key is an ancestor of a bolt definition by the parent links (short +0x6).
 *
 * Walks up from @p boltDef: its parent is found with cBEBoltDB_findBoltIndex in the definition's group (byte +0x0); the walk stops at
 * parent kNoBoltID or a missing definition. The cheat codes use it to find the items under kBolt_boardGroup.
 *
 * @param boltDef  the bolt definition to start from, or null
 * @param key  the bolt id to look for
 * @return 1 when found, else 0
 */
int cBEBoltDB::isBoltAncestor(tBEBoltDef* boltDef, int key)
{
    if (boltDef == 0)
        return 0;
    if (boltDef->parentId == key)
        return 1;
    if (boltDef->parentId == kNoBoltID)
        return 0;
    int i = findBoltIndex(boltDef->charID, boltDef->parentId);
    tBEBoltDef* parent;
    if (i < 0)
        parent = 0;
    else
        parent = &mpCharBolts[i];
    return isBoltAncestor(parent, key);
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getNumCharBolts__FPvi);
#ifdef SKIP_ASM
/**
 * Returns how many bolt definitions group @p i has (+0x2C + i * 4 of the bolt database).
 *
 * @param i  the group (a character ID)
 * @return the count
 */
int cBEBoltDB::getNumCharBolts(int i)
{
    return mNumCharBolts[i];
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getCharBolts);
#ifdef SKIP_ASM
/**
 * Returns the first bolt definition of group @p i, or null when the group is empty.
 *
 * The definitions are 0x38 bytes from +0x4; the group's count and first index are at +0x2C and +0xA4.
 *
 * @param i  the group (a character ID)
 * @return the first definition, or null
 */
tBEBoltDef* cBEBoltDB::getCharBolts(int i)
{
    if (mNumCharBolts[i] <= 0) {
        return 0;
    }
    return &mpCharBolts[mCharBoltsStart[i]];
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getCharBolts1);
#ifdef SKIP_ASM
/**
 * Returns the first bolt definition of group @p i, or null when the group is empty (same body as cBEBoltDB_getCharBolts).
 *
 * Nothing calls it.
 *
 * @param i  the group (a character ID)
 * @return the first definition, or null
 */
tBEBoltDef* cBEBoltDB::getCharBolts1(int i)
{
    if (mNumCharBolts[i] <= 0) {
        return 0;
    }
    return &mpCharBolts[mCharBoltsStart[i]];
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getCharBoltsStart);
#ifdef SKIP_ASM
/**
 * Returns the index of group @p groupId's first bolt definition (+0xA4 + groupId * 4 of the bolt database).
 *
 * Nothing calls it.
 *
 * @param groupId  the group (a character ID)
 * @return the index
 */
int cBEBoltDB::getCharBoltsStart(int groupId)
{
    return mCharBoltsStart[groupId];
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getNumCharRules);
#ifdef SKIP_ASM
/**
 * Returns how many 12-byte records (the table at +0x8) group @p groupId has (+0x20C + groupId * 4 of the bolt
 * database).
 *
 * Nothing calls it.
 *
 * @param groupId  the group (a character ID)
 * @return the count
 */
int cBEBoltDB::getNumCharRules(int groupId)
{
    return mNumCharRules[groupId];
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_countBoltRules);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
/**
 * Counts group @p groupId's 12-byte records (the table at +0x8) whose key (short +0x2) is @p key.
 *
 * The records are sorted by key within the group; the count stops at the first larger key. Nothing calls it.
 *
 * @param groupId  the group (a character ID)
 * @param key  the key (a bolt id)
 * @return the number of matching records
 */
int cBEBoltDB::countBoltRules(int groupId, int key)
{
    int n;
    int i;
    if (mNumCharRules[groupId] <= 0) {
        return 0;
    }
    n = 0;
    int end = mCharRulesStart[groupId] + mNumCharRules[groupId];
    for (i = mCharRulesStart[groupId]; i < end; i++) {
        if (mpCharRules[i].boltId == key) {
            n++;
        } else if (key < mpCharRules[i].boltId) {
            break;
        }
    }
    return n;
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_findBoltRule);
#ifdef SKIP_ASM
extern "C" void* cBEBoltDB_findBoltRules(void*, int, int, int*);

/**
 * Returns group @p groupId's first 12-byte record with key @p key (cBEBoltDB_findBoltRules, dropping the count).
 *
 * Nothing calls it.
 *
 * @param groupId  the group (a character ID)
 * @param key  the key (a bolt id)
 * @return the record, or null
 */
void* cBEBoltDB::findBoltRule(int groupId, int key)
{
    int count;
    return cBEBoltDB_findBoltRules(this, groupId, key, &count);
}
#endif
//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_findBoltRules);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
// Finds the first entry with this key in a sorted bucket; also counts matches.
/**
 * Returns group @p groupId's first 12-byte record (the table at +0x8) whose key (short +0x2) is @p wantKey, and counts
 * the matches.
 *
 * The records are sorted by key within the group (count +0x20C, first +0x284); the scan stops at the first larger key.
 *
 * @param groupId  the group (a character ID)
 * @param wantKey  the key (a bolt id)
 * @param out  receives the number of matching records
 * @return the first matching record, or 0
 */
int cBEBoltDB::findBoltRules(int groupId, int wantKey, int* out)
{
    *out = 0;
    int n = mNumCharRules[groupId];
    if (n <= 0) {
        return 0;
    }
    int start = mCharRulesStart[groupId];
    int end = start + n;
    int i = start;
    tBEBoltRule* found = 0;
    if (i < end) {
        do {
            int off = i * 12;
            tBEBoltRule* entry = (tBEBoltRule*)(off + (int)mpCharRules);
            if (entry->boltId == wantKey) {
                if (found == 0) {
                    found = entry;
                }
                (*out)++;
            } else if (wantKey < entry->boltId) {
                break;
            }
            i++;
        } while (i < end);
    }
    return (int)found;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getBoltRules);
#ifdef SKIP_ASM
/**
 * Returns group @p groupId's first 12-byte record with key @p key whose flag byte (+0x1) is set or clear as @p want
 * asks, and counts the run.
 *
 * Scans the group's records (table +0x8, count +0x20C, first +0x284), sorted by key, stopping at a larger key or at the
 * first non-matching flag after a match. The economy code (beintecon) uses it for an item's links.
 *
 * @param groupId  the group (a character ID)
 * @param key  the key (a bolt id)
 * @param want  1 for records with the flag set, 0 for clear
 * @param out  receives the number of matching records
 * @return the first matching record, or null
 */
tBEBoltRule* cBEBoltDB::getBoltRules(int groupId, int key, int want, int* out)
{
    tBEBoltRule* result;
    int i;
    *out = 0;
    if (mNumCharRules[groupId] <= 0) {
        return 0;
    }
    result = 0;
    for (i = mCharRulesStart[groupId]; i < mCharRulesStart[groupId] + mNumCharRules[groupId]; i++) {
        if (mpCharRules[i].boltId == key) {
            if ((mpCharRules[i].on != 0) == want) {
                if (result == 0) {
                    result = &mpCharRules[i];
                }
                (*out)++;
            } else if (result != 0) {
                break;
            }
        } else if (key < mpCharRules[i].boltId) {
            break;
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getNumCharPairs__FPvi);
#ifdef SKIP_ASM
/**
 * Returns how many 8-byte records of the table at +0xC group @p i has (+0x2FC + i * 4 of the bolt database).
 *
 * @param i  the group (a character ID)
 * @return the count
 */
int cBEBoltDB::getNumCharPairs(int i)
{
    return mNumCharPairs[i];
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getCharPairs);
#ifdef SKIP_ASM
/**
 * Returns group @p i's first 8-byte record of the table at +0xC, or null when it has none.
 *
 * cBEBoltDB_refreshVisibleBolts reads the records as (bolt, bolt) pairs at +0x4 and +0x6.
 *
 * @param i  the group (a character ID)
 * @return the first record, or null
 */
tBEBoltPair* cBEBoltDB::getCharPairs(int i)
{
    if (mNumCharPairs[i] > 0) {
        return &mpCharPairs[mCharPairsStart[i]];
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getNumCharGroups__FPvi);
#ifdef SKIP_ASM
/**
 * Returns how many 8-byte records of the table at +0x10 group @p i has (+0x3EC + i * 4 of the bolt database).
 *
 * @param i  the group (a character ID)
 * @return the count
 */
int cBEBoltDB::getNumCharGroups(int i)
{
    return mNumCharGroups[i];
}
#endif

//100%
INCLUDE_ASM("be/belibrary", cBEBoltDB_getCharGroups);
#ifdef SKIP_ASM
/**
 * Returns group @p i's first 8-byte record of the table at +0x10, or null when it has none.
 *
 * The economy code (beintecon) reads these records as item groups.
 *
 * @param i  the group (a character ID)
 * @return the first record, or null
 */
tBEBoltGroup* cBEBoltDB::getCharGroups(int i)
{
    if (mNumCharGroups[i] > 0) {
        return &mpCharGroups[mCharGroupsStart[i]];
    }
    return 0;
}
#endif

