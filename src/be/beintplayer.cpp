#include "common.h"
#include "be/beintplayer.h"
#include "be/beintstat.h"
#include "be/beintnetwork.h"
#include "main/ssxapp.h"

extern int cBELibrary_raceDB[];

struct sPlayerCharEntry {
    char pad_0x00[0xc];
    int field_0xc; // 0xc
    char pad_0x10[1];
    signed char mCharID; // 0x11
    signed char field_0x12; // 0x12
    char pad_0x13[0x1C - 0x12 - 1];
};
extern sPlayerCharEntry cBENewPlayerInterface_tempPlayers[];

extern "C" void cBENewPlayerInterface_discard(cBENewPlayerInterface* self);
extern "C" void cBENewPlayerInterface_defaultCtrl(cBENewPlayerInterface* self);
extern void* cBENewPlayerInterface_vtbl[16];

struct cBENewPlayerInterfaceCtor {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_cBENewPlayerInterface__FP25cBENewPlayerInterfaceCtor);
#ifdef SKIP_ASM
/**
 * Constructs the back end's player interface and resets its player records and controller assignments.
 *
 * Zeroes the word at +0x8, stores the vtable, then calls cBENewPlayerInterface_discard() and
 * cBENewPlayerInterface_defaultCtrl().
 *
 * @return @p self
 */
cBENewPlayerInterfaceCtor* cBENewPlayerInterface_cBENewPlayerInterface(cBENewPlayerInterfaceCtor* self)
{
    self->field_0x8 = 0;
    self->vtable = cBENewPlayerInterface_vtbl;
    cBENewPlayerInterface_discard((cBENewPlayerInterface*)self);
    cBENewPlayerInterface_defaultCtrl((cBENewPlayerInterface*)self);
    return self;
}
#endif

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BENewPlayerInt[];

//99.24%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the player interface singleton, creating it on first use.
 *
 * The instance lives in cBENewPlayerInterface_instance; it is allocated as 0x10 bytes tagged "BENewPlayerInt".
 *
 * @return the player interface
 */
void* cBENewPlayerInterface_getThis()
{
    if (cBENewPlayerInterface_instance == 0) {
        void* mem = cMemMan_alloc(0x10, str_BENewPlayerInt, 0, 0);
        cBENewPlayerInterface_instance = (cBENewPlayerInterface*)(cBENewPlayerInterface_cBENewPlayerInterface((cBENewPlayerInterfaceCtor*)mem));
    }
    return cBENewPlayerInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_submit);
#ifdef SKIP_ASM
struct sRecords_59B8 {
    int w[42];
};
extern sRecords_59B8 D_00535B20_59B8 __asm__("cBELibrary_playerDB");
extern sRecords_59B8 D_00534FE0_59B8 __asm__("cBENewPlayerInterface_tempPlayers");
// PORT: cBENewPlayerInterface_ingameSubmit takes no arguments; this caller passes its own self through untouched in $a0.
extern "C" void cBENewPlayerInterface_ingameSubmit_59B8(void* self) __asm__("cBENewPlayerInterface_ingameSubmit");

/**
 * Commits the working player table cBENewPlayerInterface_tempPlayers to the library's cBELibrary_playerDB.
 *
 * When the player interface singleton's flag at +0x8 is set the whole 0xA8-byte table is copied; otherwise
 * only the names and flag bit 1 of the six entries (cBENewPlayerInterface_ingameSubmit).
 */
// Copies the 0xA8-byte character table from its backup when cBENewPlayerInterface_instance says so, otherwise rebuilds it.
extern "C" void cBENewPlayerInterface_submit(cBENewPlayerInterface* self)
{
    if (cBENewPlayerInterface_instance->bIsActive != 0) {
        D_00535B20_59B8 = D_00534FE0_59B8;
    } else {
        cBENewPlayerInterface_ingameSubmit_59B8(self);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_ingameSubmit);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);

struct sCharEntryBits_00145A98 {
    char name[0xC];          // 0x00
    int field_0xc;           // 0x0C
    unsigned int flag0 : 1;  // 0x10
    unsigned int flag1 : 1;
    unsigned int rest : 30;
    int pad_0x14[2];
};

// Bitfield views of the two 0x1C-byte character tables.
extern sCharEntryBits_00145A98 D_00534FE0_bits[] __asm__("cBENewPlayerInterface_tempPlayers");
extern sCharEntryBits_00145A98 D_00535B20_bits[] __asm__("cBELibrary_playerDB");

/**
 * Commits part of the player table: for each of the six entries, copies flag bit 1 (vibration) and the 12-byte name
 * from cBENewPlayerInterface_tempPlayers to cBELibrary_playerDB.
 *
 * cBENewPlayerInterface_submit runs it instead of the whole-table copy while the interface's word at +0x8 is clear.
 */
extern "C" void cBENewPlayerInterface_ingameSubmit(void)
{
    for (int i = 0; i < 6; i++)
    {
        D_00535B20_bits[i].flag1 = D_00534FE0_bits[i].flag1;
        strncpy(D_00535B20_bits[i].name, D_00534FE0_bits[i].name, 0xC);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_discard);
#ifdef SKIP_ASM
struct sPlayerTable_00145B20
{
    sPlayerCharEntry entries[6];
};

extern sPlayerTable_00145B20 cBELibrary_playerDB;

/**
 * Throws away player-table edits: copies cBELibrary_playerDB back over the working table cBENewPlayerInterface_tempPlayers.
 */
extern "C" void cBENewPlayerInterface_discard(cBENewPlayerInterface* self)
{
    *(sPlayerTable_00145B20*)cBENewPlayerInterface_tempPlayers = cBELibrary_playerDB;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_defaultCtrl);
#ifdef SKIP_ASM
struct sVEntry00145BD8 {
    short delta;
    short index;
    void (*fn)(void*);
};

struct sCtrlEntry00145BD8 {
    char pad_0x00[0x10];
    unsigned int flag0 : 1;
    unsigned int flag1 : 1;
    unsigned int flag2 : 1;
    unsigned int rest : 29;
    char pad_0x14[0x8];
};

/**
 * Resets the library's six player entries (byte 0x13 cleared, flag bit 1 set), then reloads the working copy.
 *
 * The reload is the interface's slot 2 (discard).
 */
extern "C" void cBENewPlayerInterface_defaultCtrl(cBENewPlayerInterface* self)
{
    int i;
    sCtrlEntry00145BD8* entry = (sCtrlEntry00145BD8*)&cBELibrary_playerDB;
    for (i = 0; i < 6; i++)
    {
        ((char*)&entry[i])[0x13] = 0;
        entry[i].flag1 = 1;
    }
    sVEntry00145BD8* vt = (sVEntry00145BD8*)self->mVtbl;
    vt[2].fn((char*)self + vt[2].delta);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isFirstTimeWC);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

/**
 * Returns a rider's first-time-in-career flag: the word at +0 of its character record in cBELibrary_profileLib.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].mCharID.
 * Starting the career with the flag set opens on location 14, where the rider starts at the back-country drop-off.
 *
 * @param index  the rider (On Tour iRider)
 * @return the flag word
 */
extern "C" int cBENewPlayerInterface_isFirstTimeWC(cBENewPlayerInterface* self, int index)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    int off = charId * 3976 + profile * 39760;
    char* charRecord = (char*)cBELibrary_profileLib + off;
    return *(int*)charRecord;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setIsFirstTimeWC);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

/**
 * Sets a rider's first-time-in-career flag: the word at +0 of its character record in cBELibrary_profileLib.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].mCharID.
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the new flag
 */
extern "C" void cBENewPlayerInterface_setIsFirstTimeWC(cBENewPlayerInterface* self, int index, int value)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    int off = charId * 3976 + profile * 39760;
    char* charRecord = (char*)cBELibrary_profileLib + off;
    *(int*)charRecord = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isFirstTimeOnTrack);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

/**
 * Tells whether a rider has never been to a location before.
 *
 * Uses the profile of @p index (cBELibrary_getProfileIndex) and the character of its player
 * (cBENewPlayerInterface_tempPlayers[getPlayerID].mCharID) to find the cBELibrary_profileLib record, then tests bit @p
 * bit of the visited mask at +0xACC.
 *
 * @param index  the rider (On Tour iRider)
 * @param bit  the location (On Tour eBELocationID locTrack)
 * @return true when the location has not been visited
 */
extern "C" int cBENewPlayerInterface_isFirstTimeOnTrack(cBENewPlayerInterface* self, int index, int bit)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int characterId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    int off = profile * 39760 + characterId * 3976;
    char* profileRec = (char*)cBELibrary_profileLib + off;
    int mask = 1 << bit;
    return (*(int*)(profileRec + 0xACC) & mask) == 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setFirstTimeOnTrack);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];
extern "C" void cBECharProfileDB_setLocationVisited(void* self, int bit, int on);

struct sProfileSlot_00145DD0 {
    char data[0xF88];
};

/**
 * Marks a location as new or already visited for a rider's character.
 *
 * Finds the rider's character profile as cBENewPlayerInterface_isFirstTimeOnTrack does and calls cBECharProfileDB_setLocationVisited
 * (cBECharProfileDB::setLocationVisited) with !@p flag.
 *
 * @param flag  On Tour value: true marks the location as not yet visited
 * @param index  the rider (On Tour iRider)
 * @param bit  the location (On Tour eBELocationID locTrack)
 */
extern "C" void cBENewPlayerInterface_setFirstTimeOnTrack(cBENewPlayerInterface* self, bool flag, int index, int bit)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int characterId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    cBECharProfileDB_setLocationVisited(&((sProfileSlot_00145DD0 (*)[10])cBELibrary_profileLib)[profile][characterId], bit, !flag);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_hasVisitedPeakWC);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];
extern "C" int cBECharProfileDB_hasVisitedPeak(void* self, int kind);

struct sProfileSlot_00145E68 {
    char data[0xF88];
};

/**
 * Returns whether a rider's character has visited any location of a peak.
 *
 * Passes the rider's character record (profile cBELibrary_getProfileIndex(@p index), character
 * tempPlayers[getPlayerID(@p index)].mCharID) to cBECharProfileDB_hasVisitedPeak, which tests its visited-location mask.
 *
 * @param index  the rider (On Tour iRider)
 * @param kind  the peak, 0-2
 * @return 1 when visited, else 0
 */
extern "C" int cBENewPlayerInterface_hasVisitedPeakWC(cBENewPlayerInterface* self, int index, int kind)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    return cBECharProfileDB_hasVisitedPeak(&((sProfileSlot_00145E68 (*)[10])cBELibrary_profileLib)[profile][charId], kind);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_hasPlayedEvent);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];
void* cBECharProfileDB_getScoreStats(void* self, int a, int b);

struct sProfileSlot_00145EF0 {
    char data[0xF88];
};

/**
 * Returns whether a rider's character has a result for an event on a location: the first byte of its score record.
 *
 * The record is cBECharProfileDB_getScoreStats(@p locationId, @p event) on the rider's character record (profile
 * cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].mCharID). There is no null
 * check: for a pair with no score record getScoreStats returns 0 and the byte is read from address 0.
 *
 * @param index  the rider (On Tour iRider)
 * @param locationId  the location (On Tour eBELocationID)
 * @param event  the game event (On Tour eBEGameEvent)
 * @return 1 when the byte is nonzero, else 0
 */
extern "C" int cBENewPlayerInterface_hasPlayedEvent(cBENewPlayerInterface* self, int index, int locationId, int event)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    signed char* stats = (signed char*)cBECharProfileDB_getScoreStats(&((sProfileSlot_00145EF0 (*)[10])cBELibrary_profileLib)[profile][charId], locationId, event);
    return *stats != 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isPeakLocked);
#ifdef SKIP_ASM
extern int cBELibrary_profileLib[];

/**
 * Tells whether a peak is locked for a character: bit 12 (peak 1) or 13 (peak 2) of the record's flags.
 *
 * Peak 0 is never locked; the flags are the 64-bit word at +0x278 of the character's profile record.
 *
 * @param iProfileIndex  profile index
 * @param iCharacterID  character id
 * @param peak  the peak, 0-2
 * @return 1 when locked, else 0
 */
// PORT: 64-bit ulong flag word (bit 12 = peak 1 locked, bit 13 = peak 2).
extern "C" int cBENewPlayerInterface_isPeakLocked(cBENewPlayerInterface* self, int iProfileIndex, int iCharacterID, int peak)
{
    if (peak != 0)
    {
        if (peak == 1)
        {
            int bOff = iCharacterID * 3976;
            int aOff = iProfileIndex * 39760;
            int off = bOff + aOff;
            char* charProfile = (char*)cBELibrary_profileLib + off;
            return (int)(*(ulong*)(charProfile + 0x278) >> 12) & 1;
        }
        else
        {
            int bOff = iCharacterID * 3976;
            int aOff = iProfileIndex * 39760;
            int off = bOff + aOff;
            char* charProfile = (char*)cBELibrary_profileLib + off;
            return (int)(*(ulong*)(charProfile + 0x278) >> 13) & 1;
        }
    }
    return 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isPeakLocked1);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

/**
 * Checks whether a peak is locked for a rider given by rider index.
 *
 * Resolves the rider's profile index and character id through cBELibrary and passes them on to
 * cBENewPlayerInterface_isPeakLocked().
 *
 * @param rider  rider index passed to cBELibrary_getProfileIndex() and cBELibrary_getCharacterID()
 * @param peak   peak to test
 * @return the result of cBENewPlayerInterface_isPeakLocked()
 */
extern "C" int cBENewPlayerInterface_isPeakLocked1(cBENewPlayerInterface* self, int rider, int peak)
{
    int profile = cBELibrary_getProfileIndex(rider);
    return cBENewPlayerInterface_isPeakLocked(self, profile, cBELibrary_getCharacterID(rider), peak);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setPeakLocked);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

// PORT: 64-bit `long` bitfield (8 bytes on EE); use uint64_t off-PS2.
struct sProfBits_146070 {
    unsigned long pad : 12;
    unsigned long b12 : 1;
    unsigned long b13 : 1;
};

/**
 * Locks or unlocks a peak for a rider's character: bit 12 of its profile flags for peak 1, bit 13 for any other.
 *
 * The flags are the 64-bit word at +0x278 of the character record (profile cBELibrary_getProfileIndex(@p index),
 * character tempPlayers[getPlayerID(@p index)].mCharID), the bits cBENewPlayerInterface_isPeakLocked reads.
 *
 * @param index  the rider (On Tour iRider)
 * @param kind  the peak (0-based): 1, or 2 (any other value also writes bit 13)
 * @param value  1 to lock, 0 to unlock
 */
extern "C" void cBENewPlayerInterface_setPeakLocked(cBENewPlayerInterface* self, int index, int kind, int value)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    signed char charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    if (kind == 1) {
        char* profileRec = (char*)cBELibrary_profileLib + (charId * 0xF88 + profile * 0x9B50);
        ((sProfBits_146070*)(profileRec + 0x278))->b12 = value;
    } else {
        char* profileRec = (char*)cBELibrary_profileLib + (charId * 0xF88 + profile * 0x9B50);
        ((sProfBits_146070*)(profileRec + 0x278))->b13 = value;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isRivalChallengePlayed);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

/**
 * Returns whether a rider's character has played a peak's rival challenge.
 *
 * Reads the 64-bit flag word at +0x278 of the character record (profile cBELibrary_getProfileIndex(@p index),
 * character tempPlayers[getPlayerID(@p index)].mCharID): bit 3 + @p kind when @p flag is set, else bit @p kind.
 *
 * @param index  the rider (On Tour iRider)
 * @param kind  the peak, 0-2; any other gives 0
 * @param flag  selects bits 3-5 when nonzero, bits 0-2 when 0 (the end-of-event script passes game mode == 0)
 * @return the bit, 0 or 1
 */
// PORT: 64-bit `long` flag word (8 bytes on EE); use int64_t off-PS2.
extern "C" int cBENewPlayerInterface_isRivalChallengePlayed(cBENewPlayerInterface* self, int index, int kind, int flag)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    signed char charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    if (flag) {
        if (kind == 0) {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 3) & 1;
        } else if (kind == 1) {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 4) & 1;
        } else if (kind == 2) {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 5) & 1;
        }
    } else {
        if (kind == 0) {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 0) & 1;
        } else if (kind == 1) {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 1) & 1;
        } else if (kind == 2) {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 2) & 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setRivalChallengePlayed);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

// PORT: 64-bit `long` bitfield (8 bytes on EE); use uint64_t off-PS2.
struct sProfBits6320 {
    unsigned long b0 : 1;
    unsigned long b1 : 1;
    unsigned long b2 : 1;
    unsigned long b3 : 1;
    unsigned long b4 : 1;
    unsigned long b5 : 1;
};

/**
 * Sets or clears the played mark of a peak's rival challenge for a rider's character.
 *
 * Writes bit 3 + @p kind (when @p flag is set) or bit @p kind of the 64-bit flag word at +0x278 of the character record
 * (profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].mCharID), the bit
 * cBENewPlayerInterface_isRivalChallengePlayed reads. A peak outside 0-2 writes nothing.
 *
 * @param index  the rider (On Tour iRider)
 * @param kind  the peak, 0-2
 * @param flag  selects bits 3-5 when nonzero, bits 0-2 when 0
 * @param value  the new bit
 */
extern "C" void cBENewPlayerInterface_setRivalChallengePlayed(cBENewPlayerInterface* self, int index, int kind, int flag, int value)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    signed char charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    if (flag) {
        if (kind == 0) {
            char* profileRec = (char*)cBELibrary_profileLib + (charId * 0xF88 + profile * 0x9B50);
            ((sProfBits6320*)(profileRec + 0x278))->b3 = value;
        } else if (kind == 1) {
            char* profileRec = (char*)cBELibrary_profileLib + (charId * 0xF88 + profile * 0x9B50);
            ((sProfBits6320*)(profileRec + 0x278))->b4 = value;
        } else if (kind == 2) {
            char* profileRec = (char*)cBELibrary_profileLib + (charId * 0xF88 + profile * 0x9B50);
            ((sProfBits6320*)(profileRec + 0x278))->b5 = value;
        }
    } else {
        if (kind == 0) {
            char* profileRec = (char*)cBELibrary_profileLib + (charId * 0xF88 + profile * 0x9B50);
            ((sProfBits6320*)(profileRec + 0x278))->b0 = value;
        } else if (kind == 1) {
            char* profileRec = (char*)cBELibrary_profileLib + (charId * 0xF88 + profile * 0x9B50);
            ((sProfBits6320*)(profileRec + 0x278))->b1 = value;
        } else if (kind == 2) {
            char* profileRec = (char*)cBELibrary_profileLib + (charId * 0xF88 + profile * 0x9B50);
            ((sProfBits6320*)(profileRec + 0x278))->b2 = value;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isRivalEventLocked);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
extern int cBELibrary_profileLib[];

/**
 * Returns whether a peak's rival event is still locked for a character.
 *
 * Reads the 64-bit flag word at +0x278 of character @p character's record in the profile of @p index
 * (cBELibrary_getProfileIndex): bit 6 + @p kind when @p flag is set, else bit 9 + @p kind.
 *
 * @param index  the rider whose profile is used (On Tour iRider)
 * @param character  the character (On Tour eBECharacters)
 * @param kind  the peak, 0-2; any other gives 0
 * @param flag  selects bits 6-8 when nonzero, bits 9-11 when 0 (the map passes 1 for event kind 4, 0 for kind 5)
 * @return 1 when locked, else 0
 */
// PORT: 64-bit `long` flag word (8 bytes on EE); use int64_t off-PS2.
extern "C" int cBENewPlayerInterface_isRivalEventLocked(cBENewPlayerInterface* self, int index, int character, int kind, int flag)
{
    int profile = cBELibrary_getProfileIndex(index);
    if (flag) {
        switch (kind) {
        case 0:
            {
            int off = character * 3976 + profile * 39760;
            char* profileRec = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(profileRec + 0x278) >> 6) & 1;
        }
        case 1:
            {
            int off = character * 3976 + profile * 39760;
            char* profileRec = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(profileRec + 0x278) >> 7) & 1;
        }
        case 2:
            {
            int off = character * 3976 + profile * 39760;
            char* profileRec = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(profileRec + 0x278) >> 8) & 1;
        }
        }
    } else {
        switch (kind) {
        case 0:
            {
            int off = character * 3976 + profile * 39760;
            char* profileRec = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(profileRec + 0x278) >> 9) & 1;
        }
        case 1:
            {
            int off = character * 3976 + profile * 39760;
            char* profileRec = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(profileRec + 0x278) >> 10) & 1;
        }
        case 2:
            {
            int off = character * 3976 + profile * 39760;
            char* profileRec = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(profileRec + 0x278) >> 11) & 1;
        }
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isRivalEventLocked1);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];
struct sTrackInfo_1466A0 {
    char pad0[0x54];
    int kind;                   // 0x54
    char pad58[0x64 - 0x58];
};
extern sTrackInfo_1466A0 cBELibrary_locationDB[];

/**
 * Returns whether the rival event of the current peak is still locked for a rider's character.
 *
 * The peak is the word at +0x54 of the current location's cBELibrary_locationDB record (location: cBELibrary_raceDB
 * +0x40). When the committed game mode (cBELibrary_raceDB byte +0x48) is 5, reads bit 6 + peak of the 64-bit flag word
 * at +0x278 of the character record (profile cBELibrary_getProfileIndex(@p index), character
 * tempPlayers[getPlayerID(@p index)].mCharID), else bit 9 + peak. A peak outside 0-2 gives 0.
 *
 * @param index  the rider (On Tour iRider)
 * @return 1 when locked, else 0
 */
// PORT: 64-bit `long` flag word (8 bytes on EE); use int64_t off-PS2.
extern "C" int cBENewPlayerInterface_isRivalEventLocked1(cBENewPlayerInterface* self, int index)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    signed char charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    int kind = cBELibrary_locationDB[cBELibrary_raceDB[16]].kind;
    if (*(signed char*)&cBELibrary_raceDB[18] == 5) {
        switch (kind) {
        case 0:
            {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 6) & 1;
        }
        case 1:
            {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 7) & 1;
        }
        case 2:
            {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 8) & 1;
        }
        }
    } else {
        switch (kind) {
        case 0:
            {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 9) & 1;
        }
        case 1:
            {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 10) & 1;
        }
        case 2:
            {
            int off = charId * 3976 + profile * 39760;
            char* charRecord = (char*)cBELibrary_profileLib + off;
            return (int)(*(long*)(charRecord + 0x278) >> 11) & 1;
        }
        }
    }
    return 0;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setRivalEventLocked);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
extern int cBELibrary_profileLib[];

// PORT: 64-bit `long` bitfield (8 bytes on EE); use uint64_t off-PS2.
struct sProfBits_1468B0 {
    char pad0[0x278];
    unsigned long pad_bits : 6;
    unsigned long b6 : 1;
    unsigned long b7 : 1;
    unsigned long b8 : 1;
    unsigned long b9 : 1;
    unsigned long b10 : 1;
    unsigned long b11 : 1;
    char pad1[0xF88 - 0x280];
};

/**
 * Locks or unlocks a peak's rival event for a character.
 *
 * Writes bit 6 + @p kind (when @p flag is set) or bit 9 + @p kind of the 64-bit flag word at +0x278 of character
 * @p character's record in the profile of @p index (cBELibrary_getProfileIndex), the bit cBENewPlayerInterface_isRivalEventLocked reads. A peak outside
 * 0-2 writes nothing.
 *
 * @param index  the rider whose profile is used (On Tour iRider)
 * @param character  the character (On Tour eBECharacters)
 * @param kind  the peak, 0-2
 * @param flag  selects bits 6-8 when nonzero, bits 9-11 when 0
 * @param value  1 to lock, 0 to unlock
 */
extern "C" void cBENewPlayerInterface_setRivalEventLocked(cBENewPlayerInterface* self, int index, int character, int kind, int flag, int value)
{
    int profile = cBELibrary_getProfileIndex(index);
    if (flag) {
        switch (kind) {
        case 0:
            ((sProfBits_1468B0*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b6 = value;
            break;
        case 1:
            ((sProfBits_1468B0*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b7 = value;
            break;
        case 2:
            ((sProfBits_1468B0*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b8 = value;
            break;
        }
    } else {
        switch (kind) {
        case 0:
            ((sProfBits_1468B0*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b9 = value;
            break;
        case 1:
            ((sProfBits_1468B0*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b10 = value;
            break;
        case 2:
            ((sProfBits_1468B0*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b11 = value;
            break;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isEventLocked);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cBELibrary_getProfileIndex(int);
extern int cBELibrary_profileLib[];

/**
 * Tells whether an event is still locked for a character.
 *
 * Reads the 64-bit flag word at +0x278 of cBELibrary_profileLib[profile of @p index][@p character]: events 6-11 map to bits
 * 14-19; other events return 0 (unlocked).
 *
 * @param index  the rider whose profile is used (On Tour iRider)
 * @param character  the character (On Tour eBECharacters character)
 * @param kind  the event (On Tour eBEGameEvent event)
 * @return 1 when locked
 */
// Returns one of the character's unlock flags (kinds 6-11, bits 14-19 of the 64-bit flag word).
// PORT: 64-bit `long` flag word (8 bytes on EE); use int64_t off-PS2.
extern "C" int cBENewPlayerInterface_isEventLocked(cBENewPlayerInterface* self, int index, int character, int kind)
{
    int profile = cBELibrary_getProfileIndex(index);
    switch (kind) {
    case 6: {
        int off = character * 3976 + profile * 39760;
        char* profileRec = (char*)cBELibrary_profileLib + off;
        return (int)(*(long*)(profileRec + 0x278) >> 14) & 1;
    }
    case 7: {
        int off = character * 3976 + profile * 39760;
        char* profileRec = (char*)cBELibrary_profileLib + off;
        return (int)(*(long*)(profileRec + 0x278) >> 15) & 1;
    }
    case 8: {
        int off = character * 3976 + profile * 39760;
        char* profileRec = (char*)cBELibrary_profileLib + off;
        return (int)(*(long*)(profileRec + 0x278) >> 16) & 1;
    }
    case 9: {
        int off = character * 3976 + profile * 39760;
        char* profileRec = (char*)cBELibrary_profileLib + off;
        return (int)(*(long*)(profileRec + 0x278) >> 17) & 1;
    }
    case 10: {
        int off = character * 3976 + profile * 39760;
        char* profileRec = (char*)cBELibrary_profileLib + off;
        return (int)(*(long*)(profileRec + 0x278) >> 18) & 1;
    }
    case 11: {
        int off = character * 3976 + profile * 39760;
        char* profileRec = (char*)cBELibrary_profileLib + off;
        return (int)(*(long*)(profileRec + 0x278) >> 19) & 1;
    }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setEventLocked);
#ifdef SKIP_ASM
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
struct sPlayerSub_6C10 {
    char pad0[0x278];
    unsigned long pad_bits : 14;
    unsigned long b14 : 1;
    unsigned long b15 : 1;
    unsigned long b16 : 1;
    unsigned long b17 : 1;
    unsigned long b18 : 1;
    unsigned long b19 : 1;
    char pad1[0xF88 - 0x280];
};

/**
 * Locks or unlocks an event for a character: bit 8 + @p event (events 6-11, bits 14-19) of its profile flags.
 *
 * Writes the 64-bit flag word at +0x278 of character @p character's record in the profile of @p rider
 * (cBELibrary_getProfileIndex), the bits cBENewPlayerInterface_isEventLocked reads. Other events write nothing.
 *
 * @param rider  the rider whose profile is used (On Tour iRider)
 * @param character  the character (On Tour eBECharacters)
 * @param event  the game event, 6-11
 * @param locked  1 to lock, 0 to unlock
 */
extern "C" void cBENewPlayerInterface_setEventLocked(cBENewPlayerInterface* self, int rider, int character, int event, int locked) {
    int profile = cBELibrary_getProfileIndex(rider);
    switch (event) {
    case 6:
        ((sPlayerSub_6C10*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b14 = locked;
        break;
    case 7:
        ((sPlayerSub_6C10*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b15 = locked;
        break;
    case 8:
        ((sPlayerSub_6C10*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b16 = locked;
        break;
    case 9:
        ((sPlayerSub_6C10*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b17 = locked;
        break;
    case 10:
        ((sPlayerSub_6C10*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b18 = locked;
        break;
    case 11:
        ((sPlayerSub_6C10*)((profile * 0x9B50 + character * 0xF88) + (char*)cBELibrary_profileLib))->b19 = locked;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getLastHub);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

/**
 * Returns the last hub location of a rider's character (+0x27C of its profile record).
 *
 * @param index  the rider (On Tour: iRider); mapped to the profile and the player's character
 * @return the location id
 */
extern "C" int cBENewPlayerInterface_getLastHub(cBENewPlayerInterface* self, int index)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charID = cBENewPlayerInterface_tempPlayers[id].mCharID;
    int off = charID * 3976 + profile * 39760;
    char* charProfile = (char*)cBELibrary_profileLib + off;
    return *(int*)(charProfile + 0x27C);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setLastHub);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

/**
 * Remembers the hub a rider's character was last at.
 *
 * Writes @p value to +0x27C of the rider's character profile record in cBELibrary_profileLib.
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the hub location (On Tour locHub)
 */
extern "C" void cBENewPlayerInterface_setLastHub(cBENewPlayerInterface* self, int index, int value)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int characterId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    int off = characterId * 3976 + profile * 39760;
    char* profileRec = (char*)cBELibrary_profileLib + off;
    *(int*)(profileRec + 0x27C) = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getHumanRider);
#ifdef SKIP_ASM
int cBENewPlayerInterface_getPlayerID(int index);

struct sPlayerSlot_00146E98
{
    int state;          // 0x0 (0xC in sPlayerCharEntry)
    char pad_0x04[0x18];
};

extern sPlayerSlot_00146E98 D_00534FEC[];

/**
 * Finds the rider slot of the first or second human player.
 *
 * A slot counts as human when its player record (cBENewPlayerInterface_tempPlayers, On Tour tempPlayers) has a controller ID of 0 or
 * more (+0xC, set by cBENewPlayerInterface_setRiderCtrlID()).
 *
 * @param second  0 for the first human, nonzero for the second (On Tour bSecond)
 * @return the rider slot (0-5), or -1 when there is no such human
 */
extern "C" int cBENewPlayerInterface_getHumanRider(cBENewPlayerInterface* self, int second)
{
    int n = 0;
    for (int i = 0; i < 6; i++)
    {
        int id = cBENewPlayerInterface_getPlayerID(i);
        if (D_00534FEC[id].state >= 0)
        {
            if (second == 0)
                return i;
            if (n == 1)
                return i;
            n++;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00146F40);
#ifdef SKIP_ASM
/**
 * Sets a rider's name: copies up to 12 characters of @p name into its player entry.
 *
 * The entry is cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(@p index)]; strncpy leaves a
 * 12-character name unterminated.
 *
 * @param index  the rider
 * @param name  the new name
 */
extern "C" void func_00146F40(cBENewPlayerInterface* self, int index, const char* name)
{
    strncpy((char*)&cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)], name, 0xC);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00146F88);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBE_getBE();
extern "C" int sprintf(char* buf, const char* fmt, ...);
int cBENewPlayerInterface_getPlayerID(int index);
extern unsigned int cBEOptionInterface_mOption[];
extern char str_PLAYER_pct_d[];
extern char str_JOUEUR_pct_d[];
extern char str_SPIELER_pct_d[];
extern char str_JUGADOR_pct_d[];

/**
 * Returns rider @p idx's display name, first giving an empty name a default "player n" text in the menu language.
 *
 * In a network game (cBENetworkInterface_networkOpt word 0 set) riders 0 and 1 use the online names: on the host
 * (bIsHost, +0x8) rider 0 is this player's name (+0x488) and rider 1 the opponent's (+0x498), on the other console the
 * reverse. Otherwise, when the name of the rider's entry (tempPlayers[getPlayerID(@p idx)]) is empty, formats
 * str_PLAYER_pct_d, str_JOUEUR_pct_d, str_SPIELER_pct_d or str_JUGADOR_pct_d with @p idx + 1 into it, by the language
 * field (bits 22-24 of cBEOptionInterface_mOption[0]; a language above 3 leaves it empty), and returns that name.
 *
 * @param idx  the rider
 * @return the name string
 */
// Returns the display name for player idx (network names in a net game, else a default "Player N").
extern "C" char* func_00146F88(cBENewPlayerInterface* self, int idx)
{
    cBE_getInterface_Fv(SSXApp->BE, 7);
    char* net = ((char*)&cBENetworkInterface_networkOpt);
    if (*(int*)net != 0 && idx < 2) {
        if (*(int*)(net + 0x8) != 0) {
            if (idx == 0)
                return net + 0x488;
            return net + 0x498;
        }
        if (idx == 0)
            return net + 0x498;
        return net + 0x488;
    }
    if (cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].pad_0x00[0] == 0) {
        cBE_getInterface_Fv(cBE_getBE(), 4);
        switch ((int)((cBEOptionInterface_mOption[0] >> 22) & 7)) {
        case 0:
            sprintf(cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].pad_0x00, str_PLAYER_pct_d, idx + 1);
            break;
        case 1:
            sprintf(cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].pad_0x00, str_JOUEUR_pct_d, idx + 1);
            break;
        case 2:
            sprintf(cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].pad_0x00, str_SPIELER_pct_d, idx + 1);
            break;
        case 3:
            sprintf(cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].pad_0x00, str_JUGADOR_pct_d, idx + 1);
            break;
        }
    }
    return cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].pad_0x00;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setPlayerName);
#ifdef SKIP_ASM
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);

/**
 * Sets a player slot's name: copies up to 12 characters of @p name into cBENewPlayerInterface_tempPlayers[@p playerIndex].
 *
 * The slot is used directly (no rider mapping); strncpy leaves a 12-character name unterminated.
 *
 * @param playerIndex  the player slot (On Tour iPlayer)
 * @param name  the new name
 */
extern "C" void cBENewPlayerInterface_setPlayerName(cBENewPlayerInterface* self, int playerIndex, const char* name)
{
    strncpy(cBENewPlayerInterface_tempPlayers[playerIndex].pad_0x00, name, 0xC);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getPlayerName);
#ifdef SKIP_ASM
extern "C" unsigned int strlen(const char* s);
int GetHashValue32(char* str);
extern "C" void sprintf1(unsigned short* dst, unsigned short* fmt, int n);
extern "C" char* strcpy2(char* dst, unsigned short* src);
extern char str_kT_MEMPlayerName[];

struct sVtEntry_00147170 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};


/**
 * Returns a player's name, filling in the default "kT_MEMPlayerName" text with the player number when empty.
 *
 * @param idx  the player slot (On Tour: iPlayer)
 * @return the name stored in the player table
 */
extern "C" char* cBENewPlayerInterface_getPlayerName(cBENewPlayerInterface* self, int idx)
{
    char* name = (char*)&cBENewPlayerInterface_tempPlayers[idx];
    if (strlen(name) == 0)
    {
        unsigned short buf[104];
        char* db = (char*)SSXApp->FELocale;
        sVtEntry_00147170* vt = *(sVtEntry_00147170**)(db + 4);
        sprintf1(buf, vt[4].fn(db + vt[4].delta, GetHashValue32(str_kT_MEMPlayerName)), idx + 1);
        strcpy2(name, buf);
    }
    return name;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setRiderCtrlID);
#ifdef SKIP_ASM
int cBENewPlayerInterface_getPlayerID(int index);

struct sPlayerCharFlags_00147220 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int rest : 29;
};

/**
 * Assigns a controller to a rider's player entry.
 *
 * A valid controller (0 or more) also clears the mission-man bit and stores the player id in the 1-bit field
 * bit 0 (so only its low bit).
 *
 * @param index  the rider; mapped to its player with cBENewPlayerInterface_getPlayerID()
 * @param ctrl  the controller id, or negative for none
 */
extern "C" void cBENewPlayerInterface_setRiderCtrlID(cBENewPlayerInterface* self, int index, int ctrl)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBENewPlayerInterface_tempPlayers[id].field_0xc = ctrl;
    if (ctrl >= 0) {
        char* entry = (char*)&cBENewPlayerInterface_tempPlayers[0] + id * 0x1c;
        sPlayerCharFlags_00147220* flags = (sPlayerCharFlags_00147220*)(entry + 0x10);
        flags->bit0 = id;
        flags->bit2 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getRiderCtrlID);
#ifdef SKIP_ASM
int cBENewPlayerInterface_getPlayerID(int index);

/**
 * Returns which controller drives a rider.
 *
 * cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)].field_0xc; negative means no controller (an AI rider), see
 * cBENewPlayerInterface_setPlayerCtrlID.
 *
 * @param index  the rider (On Tour iRider)
 * @return the controller id
 */
extern "C" int cBENewPlayerInterface_getRiderCtrlID(cBENewPlayerInterface* self, int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    return cBENewPlayerInterface_tempPlayers[id].field_0xc;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setPlayerCtrlID);
#ifdef SKIP_ASM
struct sPlayerCharFlags_001472C8 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int rest : 29;
};

/**
 * Assigns a controller to a player slot.
 *
 * cBENewPlayerInterface_tempPlayers[@p iPlayer].field_0xc = @p ctrlID; with a controller (@p ctrlID >= 0), flag bit 0 of the
 * record's +0x10 word takes the low bit of @p iPlayer and bit 2 is cleared. Storing the slot's low bit in a 1-bit flag looks
 * like it should have been a constant.
 *
 * @param iPlayer  the player slot (On Tour iPlayer)
 * @param ctrlID  the controller id, negative for none
 */
extern "C" void cBENewPlayerInterface_setPlayerCtrlID(cBENewPlayerInterface* self, int iPlayer, int ctrlID)
{
    cBENewPlayerInterface_tempPlayers[iPlayer].field_0xc = ctrlID;
    if (ctrlID >= 0) {
        char* playerRec = (char*)&cBENewPlayerInterface_tempPlayers[0] + iPlayer * 0x1c;
        sPlayerCharFlags_001472C8* charFlags = (sPlayerCharFlags_001472C8*)(playerRec + 0x10);
        charFlags->bit0 = iPlayer;
        charFlags->bit2 = 0;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getPlayerCtrlID);
#ifdef SKIP_ASM
/**
 * Returns which controller is assigned to a player slot.
 *
 * cBENewPlayerInterface_tempPlayers[@p iPlayer].field_0xc, the field cBENewPlayerInterface_setPlayerCtrlID writes.
 *
 * @param iPlayer  the player slot (On Tour iPlayer)
 * @return the controller id, negative for none
 */
extern "C" int cBENewPlayerInterface_getPlayerCtrlID(cBENewPlayerInterface* self, int iPlayer)
{
    return cBENewPlayerInterface_tempPlayers[iPlayer].field_0xc;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setRiderCharID);
#ifdef SKIP_ASM
static inline int func_00147338_isLocked(sPlayerCharEntry* e)
{
    return (*(unsigned int*)((char*)e + 0x10) >> 2) & 1;
}

/**
 * Sets the character of the player in a rider slot.
 *
 * When bit 2 of the player record's flag word (+0x10) is set, player 0's character is copied instead
 * of @p charID.
 *
 * @param index   rider slot, mapped to a player through cBENewPlayerInterface_getPlayerID()
 * @param charID  character ID to store
 */
extern "C" void cBENewPlayerInterface_setRiderCharID(cBENewPlayerInterface* self, int index, int charID)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    sPlayerCharEntry* entry = &cBENewPlayerInterface_tempPlayers[id];
    if (func_00147338_isLocked(entry) == 1)
    {
        entry->mCharID = cBENewPlayerInterface_tempPlayers[0].mCharID;
    }
    else
    {
        entry->mCharID = charID;
    }
}
#endif

int cBENewPlayerInterface_getPlayerID(int index);

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getRiderCharID__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the character ID of the player in a rider slot (mCharID of its player record).
 *
 * @param riderIndex  rider slot, mapped to a player through cBENewPlayerInterface_getPlayerID()
 * @return the character ID
 */
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex)
{
    int playerID = cBENewPlayerInterface_getPlayerID(riderIndex);
    sPlayerCharEntry* entry = &cBENewPlayerInterface_tempPlayers[playerID];
    return entry->mCharID;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setRiderCheatCharID);
#ifdef SKIP_ASM
/**
 * Sets the cheat character a rider appears as.
 *
 * cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)].field_0x12 = @p value (read back by
 * cBELibrary_getCheatCharacterID).
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the cheat character id, 0 for none
 */
extern "C" void cBENewPlayerInterface_setRiderCheatCharID(cBENewPlayerInterface* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBENewPlayerInterface_tempPlayers[id].field_0x12 = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getRiderCheatCharID);
#ifdef SKIP_ASM
/**
 * Returns the cheat character a rider appears as (0 for none).
 *
 * Signed byte +0x12 of cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)], the field
 * cBENewPlayerInterface_setRiderCheatCharID writes.
 *
 * @param index  the rider (On Tour iRider)
 * @return the cheat character id
 */
extern "C" signed char cBENewPlayerInterface_getRiderCheatCharID(cBENewPlayerInterface* self, int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    return cBENewPlayerInterface_tempPlayers[id].field_0x12;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setPlayerCharID);
#ifdef SKIP_ASM
struct sPlayerCharFlags_00147448 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int rest : 29;
};

static inline int cBENewPlayerInterface_setPlayerCharID_getBit2(char* p)
{
    return ((sPlayerCharFlags_00147448*)(p + 0x10))->bit2;
}

/**
 * Chooses the character a player slot rides as.
 *
 * Writes @p charID to byte +0x11 (mCharID) of cBENewPlayerInterface_tempPlayers[@p iPlayer]; when the slot's flag bit 2
 * (mission man, set by cBENewPlayerInterface_setToMissionMan) is set, player 0's character is stored instead.
 *
 * @param iPlayer  the player slot (On Tour iPlayer)
 * @param charID  the character to ride as
 */
extern "C" void cBENewPlayerInterface_setPlayerCharID(cBENewPlayerInterface* self, int iPlayer, int charID)
{
    char* playerRec = (char*)&cBENewPlayerInterface_tempPlayers[0] + iPlayer * 0x1c;
    if (cBENewPlayerInterface_setPlayerCharID_getBit2(playerRec) == 1) {
        *(signed char*)(playerRec + 0x11) = cBENewPlayerInterface_tempPlayers[0].mCharID;
    } else {
        *(signed char*)(playerRec + 0x11) = charID;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getPlayerCharID__FPvi);
#ifdef SKIP_ASM
/**
 * Returns a player's character id (+0x11 of its player table entry).
 *
 * @param index  the player slot
 * @return the character id
 */
signed char cBENewPlayerInterface_getPlayerCharID(void* self, int index)
{
    index *= sizeof(sPlayerCharEntry);
    return ((sPlayerCharEntry*)((char*)cBENewPlayerInterface_tempPlayers + index))->mCharID;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setPlayerCheatCharID);
#ifdef SKIP_ASM
/**
 * Sets the cheat character of player slot @p playerId: byte +0x12 of cBENewPlayerInterface_tempPlayers[@p playerId].
 *
 * @param playerId  the player slot (On Tour iPlayer)
 * @param value  the cheat character id, 0 for none
 */
extern "C" void cBENewPlayerInterface_setPlayerCheatCharID(cBENewPlayerInterface* self, int playerId, int value)
{
    char* playerEntry = (char*)&cBENewPlayerInterface_tempPlayers[0] + playerId * 0x1c;
    *(char*)(playerEntry + 0x12) = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getPlayerCheatCharID);
#ifdef SKIP_ASM
/**
 * Returns the cheat character of player slot @p playerId: signed byte +0x12 of cBENewPlayerInterface_tempPlayers.
 *
 * @param playerId  the player slot (On Tour iPlayer)
 * @return the cheat character id, 0 for none
 */
extern "C" signed char cBENewPlayerInterface_getPlayerCheatCharID(cBENewPlayerInterface* self, int playerId)
{
    char* playerEntry = (char*)&cBENewPlayerInterface_tempPlayers[0] + playerId * 0x1c;
    return *(signed char*)(playerEntry + 0x12);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getUseDualShock);
#ifdef SKIP_ASM
struct sPlayerCharFlags_0x10 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int rest : 28;
};

/**
 * Returns whether a rider's pad vibration is on: flag bit 1 at +0x10 of its player entry.
 *
 * The entry is cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]; cBENewPlayerInterface_defaultCtrl sets the
 * bit for every entry.
 *
 * @param index  the rider (On Tour iRider)
 * @return 1 when on, else 0
 */
extern "C" int cBENewPlayerInterface_getUseDualShock(cBENewPlayerInterface* self, int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    char* playerEntry = (char*)&cBENewPlayerInterface_tempPlayers[0] + id * 0x1c;
    return ((sPlayerCharFlags_0x10*)(playerEntry + 0x10))->bit1;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setUseDualShock);
#ifdef SKIP_ASM
struct sPlayerCharFlags_00147528 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int rest : 28;
};

/**
 * Turns a rider's pad vibration on or off: flag bit 1 at +0x10 of tempPlayers[getPlayerID(@p index)].
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the new setting
 */
extern "C" void cBENewPlayerInterface_setUseDualShock(cBENewPlayerInterface* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    sPlayerCharEntry* playerEntry = &cBENewPlayerInterface_tempPlayers[id];
    ((sPlayerCharFlags_00147528*)((char*)playerEntry + 0x10))->bit1 = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getHelpDone);
#ifdef SKIP_ASM
/**
 * Returns flag bit 3 at +0x10 of a rider's player entry (tempPlayers[getPlayerID(@p index)]).
 *
 * The bit is set once the world script has opened the message center (func_001E3510), after which func_001E3570 stops
 * offering it.
 *
 * @param index  the rider (On Tour iRider)
 * @return the bit, 0 or 1
 */
extern "C" int cBENewPlayerInterface_getHelpDone(cBENewPlayerInterface* self, int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    char* playerEntry = (char*)&cBENewPlayerInterface_tempPlayers[0] + id * 0x1c;
    return ((sPlayerCharFlags_0x10*)(playerEntry + 0x10))->bit3;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setHelpDone);
#ifdef SKIP_ASM
struct sPlayerCharFlags_001475C0 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int rest : 28;
};

/**
 * Sets or clears flag bit 3 at +0x10 of a rider's player entry (tempPlayers[getPlayerID(@p index)]).
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the new bit
 */
extern "C" void cBENewPlayerInterface_setHelpDone(cBENewPlayerInterface* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    sPlayerCharEntry* playerEntry = &cBENewPlayerInterface_tempPlayers[id];
    ((sPlayerCharFlags_001475C0*)((char*)playerEntry + 0x10))->bit3 = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getControllerMapping);
#ifdef SKIP_ASM
/**
 * Returns the button layout a rider's player has chosen.
 *
 * Signed byte +0x13 (pad_0x13[0] in the C view) of cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)].
 *
 * @param index  the rider (On Tour iRider)
 * @return the controller mapping id
 */
extern "C" signed char cBENewPlayerInterface_getControllerMapping(cBENewPlayerInterface* self, int index)
{
    return cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)].pad_0x13[0];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setControllerMapping);
#ifdef SKIP_ASM
/**
 * Sets a rider's button layout: byte +0x13 of cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)].
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the controller mapping id
 */
extern "C" void cBENewPlayerInterface_setControllerMapping(cBENewPlayerInterface* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBENewPlayerInterface_tempPlayers[id].pad_0x13[0] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147698);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPlayerCharEntryExt_7698 {
    char pad_0x00[0x14];
    int field_0x14;
    char pad_0x18[4];
};
extern sPlayerCharEntryExt_7698 D_00534FE0_ext[] __asm__("cBENewPlayerInterface_tempPlayers");

/**
 * Returns the word at +0x14 of a rider's player entry (cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]).
 *
 * @param index  the rider
 * @return the word
 */
extern "C" int func_00147698(cBENewPlayerInterface* self, int index)
{
    return D_00534FE0_ext[cBENewPlayerInterface_getPlayerID(index)].field_0x14;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", func_001476D8);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPlayerCharEntryExt_76D8 {
    char pad_0x00[0x14];
    int field_0x14;
    int field_0x18;
};
extern sPlayerCharEntryExt_76D8 D_00534FE0_ext_76D8[] __asm__("cBENewPlayerInterface_tempPlayers");

/**
 * Sets the word at +0x14 of a rider's player entry (cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]).
 *
 * @param index  the rider
 * @param value  the new word
 */
extern "C" void func_001476D8(cBENewPlayerInterface* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    D_00534FE0_ext_76D8[id].field_0x14 = value;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", func_00147718);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPlayerCharEntryExt_7718 {
    char pad_0x00[0x18];
    int field_0x18;
};
extern sPlayerCharEntryExt_7718 D_00534FE0_ext_7718[] __asm__("cBENewPlayerInterface_tempPlayers");

/**
 * Returns the word at +0x18 of a rider's player entry (cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]).
 *
 * @param index  the rider
 * @return the word
 */
extern "C" int func_00147718(cBENewPlayerInterface* self, int index)
{
    return D_00534FE0_ext_7718[cBENewPlayerInterface_getPlayerID(index)].field_0x18;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", func_00147758);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sPlayerCharEntryExt_7758 {
    char pad_0x00[0x18];
    int field_0x18;
};
extern sPlayerCharEntryExt_7758 D_00534FE0_ext_7758[] __asm__("cBENewPlayerInterface_tempPlayers");

/**
 * Sets the word at +0x18 of a rider's player entry (cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]).
 *
 * @param index  the rider
 * @param value  the new word
 */
extern "C" void func_00147758(cBENewPlayerInterface* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    D_00534FE0_ext_7758[id].field_0x18 = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getPlayerID__Fi);
#ifdef SKIP_ASM
/**
 * Maps a rider slot to its player: playerID[index] of the race settings copy at cBELibrary_raceDB (a cBERaceDB).
 *
 * @param index  rider slot, 0 to 5
 * @return the player ID
 */
int cBENewPlayerInterface_getPlayerID(int index)
{
    return cBELibrary_raceDB[index];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_001477B0);
#ifdef SKIP_ASM
struct sPlayerFlags_001477B0 {
    char pad_0x00[0x10];
    unsigned int flag0 : 1;
    unsigned int rest : 31;
    char pad_0x14[0x1C - 0x14];
};

/**
 * Returns flag bit 0 of player slot @p index's entry, or -1 when the slot has no controller.
 *
 * The slot indexes cBENewPlayerInterface_tempPlayers directly; a controller id (+0xC) of -1 gives -1, otherwise the
 * result is bit 0 of the flags at +0x10.
 *
 * @param index  the player slot
 * @return -1, 0 or 1
 */
extern "C" int func_001477B0(cBENewPlayerInterface* self, int index)
{
    if (cBENewPlayerInterface_tempPlayers[index].field_0xc == -1) {
        return -1;
    }
    return ((sPlayerFlags_001477B0*)cBENewPlayerInterface_tempPlayers)[index].flag0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isMissionMan__Fi);
#ifdef SKIP_ASM
struct sPlayerFlags_001477E8
{
    char pad_0x00[0x10];
    unsigned int flag0 : 1;       // 0x10 bit 0
    unsigned int flag1 : 1;       // 0x10 bit 1
    unsigned int missionMan : 1;  // 0x10 bit 2
    unsigned int rest : 29;
    char pad_0x14[0x1C - 0x14];
};

/**
 * Tells whether a rider's player is the mission man (bit 2 of the entry's flags).
 *
 * @param index  the rider; mapped to its player
 * @return 1 or 0
 */
int cBENewPlayerInterface_isMissionMan(int index)
{
    int playerID = cBENewPlayerInterface_getPlayerID(index);
    return ((sPlayerFlags_001477E8*)cBENewPlayerInterface_tempPlayers)[playerID].missionMan;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setToMissionMan);
#ifdef SKIP_ASM
/**
 * Turns a rider into a 'mission man': a copy of player 0's character used in missions.
 *
 * The rider's player record (cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]) takes player 0's mCharID and
 * gets flag bit 2 at +0x10.
 *
 * @param index  the rider (On Tour iRider)
 */
extern "C" void cBENewPlayerInterface_setToMissionMan(int index)
{
    cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)].mCharID = cBENewPlayerInterface_tempPlayers[0].mCharID;
    sPlayerCharEntry* playerRec = &cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)];
    ((sPlayerCharFlags_0x10*)((char*)playerRec + 0x10))->bit2 = 1;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_resetFromMissionMan);
#ifdef SKIP_ASM
/**
 * Turns a rider's player back from the mission man: character id 0 and the mission-man bit cleared.
 *
 * @param index  the rider; mapped to its player
 */
extern "C" void cBENewPlayerInterface_resetFromMissionMan(cBENewPlayerInterface* self, int index)
{
    cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)].mCharID = 0;
    sPlayerCharEntry* entry = &cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)];
    ((sPlayerCharFlags_0x10*)((char*)entry + 0x10))->bit2 = 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147908);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

/**
 * Returns the message list of a rider's character: +0xE38 of its character record in cBELibrary_profileLib.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].mCharID.
 *
 * @param index  the rider
 * @return a pointer into the character record
 */
extern "C" void* func_00147908(cBENewPlayerInterface* self, int index)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    char* charRecord = (char*)cBELibrary_profileLib + profile * 0x9B50 + charId * 3976;
    return charRecord + 0xE38;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147980);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

/**
 * Tests message flag @p bit of a rider's character: bit @p bit of the bit array at +0xF08 of its character record.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].mCharID;
 * the word is @p bit / 32 and the mask 1 << @p bit (shift taken mod 32). No range check.
 *
 * @param index  the rider
 * @param bit  the flag number
 * @return 1 when set, else 0
 */
extern "C" int func_00147980(cBENewPlayerInterface* self, int index, int bit)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    int off = profile * 39760 + charId * 3976;
    char* charRecord = (char*)cBELibrary_profileLib + off;
    charRecord = charRecord + (bit / 32) * 4;
    int mask = 1 << bit;
    return (*(unsigned int*)(charRecord + 0xF08) & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147A30);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

/**
 * Sets or clears message flag @p bit of a rider's character, in the bit array at +0xF08 of its character record.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].mCharID.
 * No range check.
 *
 * @param index  the rider
 * @param bit  the flag number
 * @param on  nonzero sets the flag, 0 clears it
 */
extern "C" void func_00147A30(cBENewPlayerInterface* self, int index, int bit, int on)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    int off = profile * 39760 + charId * 3976;
    char* charRecord = (char*)cBELibrary_profileLib + off;
    charRecord += 0xF08;
    int w = bit / 32;
    int bitInWord = bit - w * 32;
    if (on)
        *(unsigned int*)(charRecord + (w << 2)) |= 1 << bitInWord;
    else
        *(unsigned int*)(charRecord + (w << 2)) &= ~(1 << bitInWord);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147B20);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

struct sPair_func_00147B20 {
    int a;
    int b;
};
struct sProfileSlot_func_00147B20 {
    char pad[0xBA0];
    sPair_func_00147B20 e[1];
};

/**
 * Returns the first word of pair @p slot in the 8-byte pair array at +0xBA0 of a rider's character record.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].mCharID.
 *
 * @param index  the rider
 * @param slot  the pair index (no range check)
 * @return the word at +0xBA0 + @p slot * 8
 */
extern "C" int func_00147B20(cBENewPlayerInterface* self, int index, int slot)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    sProfileSlot_func_00147B20* profileSlot = (sProfileSlot_func_00147B20*)((profile * 0x9B50 + charId * 0xF88) + (char*)cBELibrary_profileLib);
    return profileSlot->e[slot].a;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", func_00147BB0);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern int cBELibrary_profileLib[];

struct sPair_func_00147BB0 {
    int a;
    int b;
};
struct sProfileSlot_func_00147BB0 {
    char pad[0xBA0];
    sPair_func_00147BB0 e[1];
};

/**
 * Returns the second word of pair @p slot in the 8-byte pair array at +0xBA0 of a rider's character record.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].mCharID.
 *
 * @param index  the rider
 * @param slot  the pair index (no range check)
 * @return the word at +0xBA4 + @p slot * 8
 */
extern "C" int func_00147BB0(cBENewPlayerInterface* self, int index, int slot)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].mCharID;
    sProfileSlot_func_00147BB0* profileSlot = (sProfileSlot_func_00147BB0*)((profile * 0x9B50 + charId * 0xF88) + (char*)cBELibrary_profileLib);
    return profileSlot->e[slot].b;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setDifficultyLevel);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern int cBELibrary_profileLib[];

/**
 * Sets a difficulty level of the first profile's current character and resets its win/loss streak.
 *
 * The character is cBELibrary_getCharacterID(0) in profile 0 of cBELibrary_profileLib. With @p flag set, stores
 * @p value in the short at +0x280 and clears +0x282; otherwise the pair at +0x284 / +0x286.
 *
 * @param value  the level, 0-2 (func_00147D20 caps it at 2)
 * @param flag  picks the pair: nonzero +0x280, 0 +0x284
 */
extern "C" void cBENewPlayerInterface_setDifficultyLevel(cBENewPlayerInterface* self, int value, int flag)
{
    int charId = cBELibrary_getCharacterID(0);
    if (flag != 0) {
        char* charRecord = (char*)cBELibrary_profileLib + charId * 0xF88;
        *(short*)(charRecord + 0x280) = value;
        *(short*)(charRecord + 0x282) = 0;
    } else {
        char* charRecord = (char*)cBELibrary_profileLib + charId * 0xF88;
        *(short*)(charRecord + 0x284) = value;
        *(short*)(charRecord + 0x286) = 0;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getDifficultyLevel);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);

/**
 * Returns a difficulty level of the first profile's current character (cBELibrary_getCharacterID(0), profile 0).
 *
 * The short at +0x280 of the character record when @p which is set, else the one at +0x284.
 *
 * @param which  picks the pair: nonzero +0x280, 0 +0x284 (cComputer passes game mode == 0)
 * @return the level, 0-2
 */
extern "C" short cBENewPlayerInterface_getDifficultyLevel(cBENewPlayerInterface* self, int which)
{
    int charId = cBELibrary_getCharacterID(0);
    if (which != 0)
    {
        char* charRecord = (char*)cBELibrary_profileLib + charId * 0xF88;
        return *(short*)(charRecord + 0x280);
    }
    else
    {
        char* charRecord = (char*)cBELibrary_profileLib + charId * 0xF88;
        return *(short*)(charRecord + 0x284);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147D20);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern int cBELibrary_profileLib[];

struct sCharRec_147D20 {
    char pad0[0x280];
    short b0;       // 0x280
    short a0;       // 0x282
    short b1;       // 0x284
    short a1;       // 0x286
    char pad288[0xF88 - 0x288];
};

/**
 * Records a win toward a difficulty level of the first profile's current character.
 *
 * Adds 1 to the streak short (+0x282 when @p which is set, else +0x286) of the character cBELibrary_getCharacterID(0)
 * in profile 0; when it reaches 2 the streak resets to 0 and the level before it (+0x280 / +0x284) goes up by one, to
 * at most 2. The game mode code calls it after a career win.
 *
 * @param which  picks the pair: nonzero +0x280 / +0x282, 0 +0x284 / +0x286
 */
extern "C" void func_00147D20(cBENewPlayerInterface* self, int which)
{
    int profile = 0;
    int charId = cBELibrary_getCharacterID(0);
    short difference, level;
    if (which)
    {
        level = (*(sCharRec_147D20*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).b0;
        difference = (*(sCharRec_147D20*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).a0;
    }
    else
    {
        level = (*(sCharRec_147D20*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).b1;
        difference = (*(sCharRec_147D20*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).a1;
    }
    difference++;
    if (difference >= 2)
    {
        if (level < 2) level++;
        difference = 0;
    }
    if (which)
    {
        (*(sCharRec_147D20*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).b0 = level;
        (*(sCharRec_147D20*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).a0 = difference;
    }
    else
    {
        (*(sCharRec_147D20*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).b1 = level;
        (*(sCharRec_147D20*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).a1 = difference;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147E18);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern int cBELibrary_profileLib[];

struct sCharRec_147E18 {
    char pad0[0x280];
    short b0;       // 0x280
    short a0;       // 0x282
    short b1;       // 0x284
    short a1;       // 0x286
    char pad288[0xF88 - 0x288];
};

/**
 * Records a loss toward a difficulty level of the first profile's current character.
 *
 * Takes 1 from the streak short (+0x282 when @p which is set, else +0x286) of the character
 * cBELibrary_getCharacterID(0) in profile 0; when it falls to -2 the streak resets to 0 and the level before it
 * (+0x280 / +0x284) goes down by one, to no less than 0. The game mode code calls it after a career loss.
 *
 * @param which  picks the pair: nonzero +0x280 / +0x282, 0 +0x284 / +0x286
 */
extern "C" void func_00147E18(cBENewPlayerInterface* self, int which)
{
    int profile = 0;
    int charId = cBELibrary_getCharacterID(0);
    short difference, level;
    if (which)
    {
        level = (*(sCharRec_147E18*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).b0;
        difference = (*(sCharRec_147E18*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).a0;
    }
    else
    {
        level = (*(sCharRec_147E18*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).b1;
        difference = (*(sCharRec_147E18*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).a1;
    }
    difference--;
    if (difference < -1)
    {
        if (level != 0) level--;
        difference = 0;
    }
    if (which)
    {
        (*(sCharRec_147E18*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).b0 = level;
        (*(sCharRec_147E18*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).a0 = difference;
    }
    else
    {
        (*(sCharRec_147E18*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).b1 = level;
        (*(sCharRec_147E18*)((char*)cBELibrary_profileLib + (profile * 0x9B50 + charId * 0xF88))).a1 = difference;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getDifficultyDifference);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern int cBELibrary_profileLib[];

/**
 * Returns the win/loss streak beside a difficulty level of the first profile's current character.
 *
 * The short at +0x282 (when @p flag is set) or +0x286 of the record of cBELibrary_getCharacterID(0) in profile 0; the
 * win and loss counters func_00147D20 and func_00147E18 move it.
 *
 * @param flag  picks the pair: nonzero +0x282, 0 +0x286
 * @return the streak, -1 to 1
 */
extern "C" short cBENewPlayerInterface_getDifficultyDifference(cBENewPlayerInterface* self, int flag)
{
    int charId = cBELibrary_getCharacterID(0);
    short result;
    if (flag == 0) {
        char* charRecord = (char*)cBELibrary_profileLib + charId * 0xF88;
        result = *(short*)(charRecord + 0x286);
    } else {
        char* charRecord = (char*)cBELibrary_profileLib + charId * 0xF88;
        result = *(short*)(charRecord + 0x282);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getThis);
#ifdef SKIP_ASM
struct sVTablePlayer_00147F78
{
    char pad_0x00[0x10];
    short delta;
    char pad_0x12[2];
    void (*fn)(void*);
};
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BECharInt[];
extern sVTablePlayer_00147F78 cBEStatInterface_vtbl;

/**
 * Returns the stat interface singleton, creating it on first use.
 *
 * The instance lives in cBEStatInterface_instance: 0x10 bytes tagged "BECharInt", with its vtable stored and slot 2
 * (discard) run on it.
 *
 * @return the stat interface
 */
extern "C" void* cBEStatInterface_getThis(void)
{
    if (cBEStatInterface_instance == 0) {
        cBEStatInterface* mem = (cBEStatInterface*)cMemMan_alloc(0x10, str_BECharInt, 0, 0);
        cBEStatInterface_instance = (cBEStatInterface*)(mem);
        sVTablePlayer_00147F78* vt = &cBEStatInterface_vtbl;
        short thisAdjust = vt->delta;
        void (*fn)(void*) = vt->fn;
        (*(void**)&mem->mVtbl) = vt;
        mem->bIsActive = 0;
        fn((char*)mem + thisAdjust);
    }
    return cBEStatInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_submit);
#ifdef SKIP_ASM
struct sCtrl7_00147FD8
{
    signed char b[7];
};

struct sPlayerSlot_00147FD8
{
    sCtrl7_00147FD8 ctrl;
    char pad[0xF88 - 7];
};

struct sPlayerStatRow;
extern sPlayerStatRow D_00535538[];
extern sPlayerSlot_00147FD8 D_004A7887[];
extern sPlayerSlot_00147FD8 D_004B13D7[];
extern sPlayerSlot_00147FD8 D_004BAF27[];

/**
 * Commits the working character stats D_00535538 to the profiles.
 *
 * Copies the 7 stat bytes of each of 10 characters in 3 profiles into +0xBDF of the character records.
 */
extern "C" void cBEStatInterface_submit(void)
{
    for (int i = 0; i < 10; i++)
    {
        D_004A7887[i].ctrl = ((sCtrl7_00147FD8 (*)[10])D_00535538)[0][i];
        D_004B13D7[i].ctrl = ((sCtrl7_00147FD8 (*)[10])D_00535538)[1][i];
        D_004BAF27[i].ctrl = ((sCtrl7_00147FD8 (*)[10])D_00535538)[2][i];
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_discard);
#ifdef SKIP_ASM
struct sPlayerStatRow;
extern sPlayerStatRow D_00535538[];
extern sPlayerSlot_00147FD8 D_004A7887[];
extern sPlayerSlot_00147FD8 D_004B13D7[];
extern sPlayerSlot_00147FD8 D_004BAF27[];

/**
 * Reloads the working character stats D_00535538 from the profiles (the reverse of submit).
 */
extern "C" void cBEStatInterface_discard(void)
{
    for (int i = 0; i < 10; i++)
    {
        ((sCtrl7_00147FD8 (*)[10])D_00535538)[0][i] = D_004A7887[i].ctrl;
        ((sCtrl7_00147FD8 (*)[10])D_00535538)[1][i] = D_004B13D7[i].ctrl;
        ((sCtrl7_00147FD8 (*)[10])D_00535538)[2][i] = D_004BAF27[i].ctrl;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getStatFromProfile);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cBELibrary_getCharacterID(int);
extern int cBELibrary_profileLib[];

/**
 * Returns one of the seven attribute bytes of a profile's current character, or -1 for an unknown attribute.
 *
 * Reads signed byte +0xBDF + @p stat of the record of character cBELibrary_getCharacterID(@p profile) in profile
 * @p profile of cBELibrary_profileLib.
 *
 * @param profile  the human, used both as the profile index and to look up the character (On Tour iHuman)
 * @param stat  the attribute, 0-6 (On Tour eBEAttributes)
 * @return the attribute value, or -1
 */
// Returns one of a character's seven stat levels for the given profile, or -1.
extern "C" int cBEStatInterface_getStatFromProfile(cBEStatInterface* self, int profile, unsigned int stat)
{
    int charId = cBELibrary_getCharacterID(profile);
    switch (stat) {
    case 0: {
        int off = charId * 3976 + profile * 39760;
        char* charRecord = (char*)cBELibrary_profileLib + off;
        return *(signed char*)(charRecord + 0xBDF);
    }
    case 1: {
        int off = charId * 3976 + profile * 39760;
        char* charRecord = (char*)cBELibrary_profileLib + off;
        return *(signed char*)(charRecord + 0xBE0);
    }
    case 2: {
        int off = charId * 3976 + profile * 39760;
        char* charRecord = (char*)cBELibrary_profileLib + off;
        return *(signed char*)(charRecord + 0xBE1);
    }
    case 3: {
        int off = charId * 3976 + profile * 39760;
        char* charRecord = (char*)cBELibrary_profileLib + off;
        return *(signed char*)(charRecord + 0xBE2);
    }
    case 4: {
        int off = charId * 3976 + profile * 39760;
        char* charRecord = (char*)cBELibrary_profileLib + off;
        return *(signed char*)(charRecord + 0xBE3);
    }
    case 5: {
        int off = charId * 3976 + profile * 39760;
        char* charRecord = (char*)cBELibrary_profileLib + off;
        return *(signed char*)(charRecord + 0xBE4);
    }
    case 6: {
        int off = charId * 3976 + profile * 39760;
        char* charRecord = (char*)cBELibrary_profileLib + off;
        return *(signed char*)(charRecord + 0xBE5);
    }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_001482D0);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sCharStat_001482D0 {
    signed char v[15];
};
extern sCharStat_001482D0 D_005308D8_001482D0[] __asm__("D_005308D8");

/**
 * Copies record @p character of D_005308D8, the 15-byte per-character default-stat table, into @p dst.
 *
 * The struct is returned by value: @p dst is the caller's buffer (g++ passes it first) and @p iface the stat
 * interface. The record has the size of On Tour's tBEStatDefault (character id, start stats, maximum stats).
 *
 * @param dst  the result buffer
 * @param iface  the stat interface (not used)
 * @param character  the character
 * @return @p dst
 */
extern "C" sCharStat_001482D0* func_001482D0(sCharStat_001482D0* dst, int iface, int character)
{
    *dst = D_005308D8_001482D0[character];
    return dst;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", func_00148328);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getCharacterID(int);

struct sCharStat_00148328 {
    signed char v[15];
};
extern sCharStat_00148328 D_005308D8_00148328[] __asm__("D_005308D8");

/**
 * Copies a rider's record of D_005308D8, the 15-byte per-character default-stat table, into @p dst.
 *
 * The record is the one of character cBELibrary_getCharacterID(@p rider). The struct is returned by value: @p dst is
 * the caller's buffer (g++ passes it first) and @p iface the stat interface.
 *
 * @param dst  the result buffer
 * @param iface  the stat interface (not used)
 * @param rider  the rider
 * @return @p dst
 */
extern "C" sCharStat_00148328* func_00148328(sCharStat_00148328* dst, int iface, int rider)
{
    int charId = cBELibrary_getCharacterID(rider);
    *dst = D_005308D8_00148328[charId];
    return dst;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setSpeed);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

// Same layout as the unit's sPlayerStatRowView_00148410 (defined later in the unit).
struct sPlayerStat7View_001483A0 {
    signed char v[7];
};
struct sPlayerStatRowView_001483A0 {
    sPlayerStat7View_001483A0 e[10];
};
extern sPlayerStatRowView_001483A0 D_00535538_view001483A0[] __asm__("D_00535538");

/**
 * Sets a rider's speed stat: byte 0 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 0 is On Tour's tBEStatDB::cSpeed. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
extern "C" void cBEStatInterface_setSpeed(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    D_00535538_view001483A0[profile].e[charId].v[0] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getSpeed);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

struct sPlayerStat7View_00148410 {
    signed char v[7];
};
struct sPlayerStatRowView_00148410 {
    sPlayerStat7View_00148410 e[10];
};
extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Returns a rider's speed stat: byte 0 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 0 is On Tour's tBEStatDB::cSpeed.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
extern "C" signed char cBEStatInterface_getSpeed(cBEStatInterface* self, int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return D_00535538_view[profile].e[charId].v[0];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setAccel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Sets a rider's acceleration stat: byte 1 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 1 is On Tour's tBEStatDB::cAccel. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
extern "C" void cBEStatInterface_setAccel(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    D_00535538_view[profile].e[charId].v[1] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getAccel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Returns a rider's acceleration stat: byte 1 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 1 is On Tour's tBEStatDB::cAccel.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
extern "C" signed char cBEStatInterface_getAccel(cBEStatInterface* self, int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return D_00535538_view[profile].e[charId].v[1];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setTrick);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Sets a rider's trick stat: byte 2 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 2 is On Tour's tBEStatDB::cTrick. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
extern "C" void cBEStatInterface_setTrick(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    D_00535538_view[profile].e[charId].v[2] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getTrick);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Returns a rider's trick stat: byte 2 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 2 is On Tour's tBEStatDB::cTrick.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
extern "C" signed char cBEStatInterface_getTrick(cBEStatInterface* self, int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return D_00535538_view[profile].e[charId].v[2];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setEdging);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Sets a rider's edging stat: byte 3 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 3 is On Tour's tBEStatDB::cEdging. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
extern "C" void cBEStatInterface_setEdging(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    D_00535538_view[profile].e[charId].v[3] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getEdging);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Returns a rider's edging stat: byte 3 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 3 is On Tour's tBEStatDB::cEdging.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
extern "C" signed char cBEStatInterface_getEdging(cBEStatInterface* self, int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return D_00535538_view[profile].e[charId].v[3];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setSpin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Sets a rider's spin stat: byte 4 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 4 is On Tour's tBEStatDB::cSpin. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
extern "C" void cBEStatInterface_setSpin(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    D_00535538_view[profile].e[charId].v[4] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getSpin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Returns a rider's spin stat: byte 4 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 4 is On Tour's tBEStatDB::cSpin.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
extern "C" signed char cBEStatInterface_getSpin(cBEStatInterface* self, int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return D_00535538_view[profile].e[charId].v[4];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setToughness);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Sets a rider's toughness stat: byte 5 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 5 is On Tour's tBEStatDB::cToughness. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
extern "C" void cBEStatInterface_setToughness(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    D_00535538_view[profile].e[charId].v[5] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getToughness);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Returns a rider's toughness stat: byte 5 of its 7-byte record in D_00535538.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 5 is On Tour's tBEStatDB::cToughness.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
extern "C" signed char cBEStatInterface_getToughness(cBEStatInterface* self, int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return D_00535538_view[profile].e[charId].v[5];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setStability);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Sets a rider's stability stat (the seventh entry of its character's stat record) in the current-stat table.
 *
 * Looks up the rider's profile with cBELibrary_getProfileIndex() and character with cBELibrary_getCharacterID(), then
 * stores @p value as a byte in D_00535538[profile][character][6] (On Tour tBEStatDB.cStability).
 * cBEStatInterface_setStatLevel() calls it with level * 5.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  new stat value (On Tour var)
 */
extern "C" void cBEStatInterface_setStability(cBEStatInterface* self, int rider, int value)
{
    int profileIndex = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    D_00535538_view[profileIndex].e[charID].v[6] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getStability);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 D_00535538_view[] __asm__("D_00535538");

/**
 * Returns a rider's stability stat (the seventh entry of its character's stat record) from the current-stat table.
 *
 * Reads D_00535538[profile][character][6] (On Tour tBEStatDB.cStability), with the profile from
 * cBELibrary_getProfileIndex() and the character from cBELibrary_getCharacterID().
 *
 * @param rider  race rider slot (On Tour iRider)
 * @return the stability stat (5 per level)
 */
extern "C" signed char cBEStatInterface_getStability(cBEStatInterface* self, int rider)
{
    int profileIndex = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    return D_00535538_view[profileIndex].e[charID].v[6];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getStatLevel);
#ifdef SKIP_ASM
extern "C" signed char cBEStatInterface_getSpeed(cBEStatInterface* self, int rider);
extern "C" signed char cBEStatInterface_getAccel(cBEStatInterface* self, int rider);
extern "C" signed char cBEStatInterface_getTrick(cBEStatInterface* self, int rider);
extern "C" signed char cBEStatInterface_getEdging(cBEStatInterface* self, int rider);
extern "C" signed char cBEStatInterface_getSpin(cBEStatInterface* self, int rider);
extern "C" signed char cBEStatInterface_getToughness(cBEStatInterface* self, int rider);
extern "C" signed char cBEStatInterface_getStability(cBEStatInterface* self, int rider);

/**
 * Returns a rider's overall stat level: the average of its seven stat levels, at least 1.
 *
 * Reads the speed, acceleration, trick, edging, spin, toughness and stability stats (cBEStatInterface_getSpeed, cBEStatInterface_getAccel,
 * cBEStatInterface_getTrick, cBEStatInterface_getEdging, cBEStatInterface_getSpin, cBEStatInterface_getToughness, cBEStatInterface_getStability), divides each by 5 (5 stat points per
 * level), sums the seven levels and divides by 7, rounding down. The inverse of cBEStatInterface_setStatLevel(), which
 * stores level * 5 in each stat. The front end shows it as the rider's level (ovstatelodge, festateloadhint).
 *
 * @param rider  race rider slot (On Tour iRider)
 * @return the average level, or 1 when it would be 0 or less
 */
extern "C" int cBEStatInterface_getStatLevel(cBEStatInterface* self, int rider)
{
    int statValue;
    statValue = cBEStatInterface_getAccel(self, rider);
    int sum = statValue / 5;
    statValue = cBEStatInterface_getEdging(self, rider);
    sum += statValue / 5;
    statValue = cBEStatInterface_getSpeed(self, rider);
    sum += statValue / 5;
    statValue = cBEStatInterface_getSpin(self, rider);
    sum += statValue / 5;
    statValue = cBEStatInterface_getToughness(self, rider);
    sum += statValue / 5;
    statValue = cBEStatInterface_getStability(self, rider);
    sum += statValue / 5;
    statValue = cBEStatInterface_getTrick(self, rider);
    sum += statValue / 5;
    int average = sum / 7;
    if (average > 0)
        return average;
    return 1;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setStatLevel);
#ifdef SKIP_ASM
/**
 * Sets every attribute of a rider to the same stat level.
 *
 * Each of cBEStatInterface_setAccel, cBEStatInterface_setEdging, cBEStatInterface_setSpeed, cBEStatInterface_setSpin, cBEStatInterface_setToughness, cBEStatInterface_setStability and cBEStatInterface_setTrick
 * (On Tour setAccel, setEdging, setSpeed, setSpin, setToughness, setStability, setTrick) gets @p value * 5. The release
 * build drops On Tour's range and read-back asserts.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the level (On Tour iLevel), 1 to the number of levels
 */
extern "C" void cBEStatInterface_setStatLevel(cBEStatInterface* self, int rider, int value)
{
    int statPoints = value * 5;
    cBEStatInterface_setAccel(self, rider, statPoints);
    cBEStatInterface_setEdging(self, rider, statPoints);
    cBEStatInterface_setSpeed(self, rider, statPoints);
    cBEStatInterface_setSpin(self, rider, statPoints);
    cBEStatInterface_setToughness(self, rider, statPoints);
    cBEStatInterface_setStability(self, rider, statPoints);
    cBEStatInterface_setTrick(self, rider, statPoints);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanSpeed);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// Same layout as the unit's sPlayerStatRow (defined at cBEStatInterface_getHumanSpeed, later in the unit).
struct sPlayerStat7View_00148B50 {
    signed char v[7];
};
struct sPlayerStatRowView_00148B50 {
    sPlayerStat7View_00148B50 e[10];
};
extern sPlayerStatRowView_00148B50 D_00535538_view00148B50[] __asm__("D_00535538");

/**
 * Stores a profile's speed stat for one character (byte 0 of the character's stat record) in the current-stat table.
 *
 * Writes @p value to D_00535538[@p humanIndex][@p charType][0] (On Tour tBEStatDB.cSpeed). Unlike the rider-slot setters, it takes
 * the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
extern "C" void cBEStatInterface_setHumanSpeed(cBEStatInterface* self, int humanIndex, int value, int charType)
{
    D_00535538_view00148B50[humanIndex].e[charType].v[0] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanSpeed);
#ifdef SKIP_ASM
struct sPlayerStat7 {
    signed char v[7];
};
struct sPlayerStatRow {
    sPlayerStat7 e[10]; // 0x46 bytes
};
extern sPlayerStatRow D_00535538[];

/**
 * Returns a profile's speed stat for one character (byte 0 of the character's stat record) from the current-stat table.
 *
 * Reads D_00535538[@p humanIndex][@p charType][0] (On Tour tBEStatDB.cSpeed).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the speed stat (5 per level)
 */
extern "C" signed char cBEStatInterface_getHumanSpeed(cBEStatInterface* self, int humanIndex, int charType)
{
    return D_00535538[humanIndex].e[charType].v[0];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanAccel);
#ifdef SKIP_ASM
/**
 * Stores a profile's acceleration stat for one character (byte 1 of the character's stat record) in the current-stat
 * table.
 *
 * Writes @p value to D_00535538[@p humanIndex][@p charType][1] (On Tour tBEStatDB.cAccel). Unlike the rider-slot setters, it takes
 * the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
extern "C" void cBEStatInterface_setHumanAccel(cBEStatInterface* self, int humanIndex, int value, int charType)
{
    D_00535538[humanIndex].e[charType].v[1] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanAccel);
#ifdef SKIP_ASM
/**
 * Returns a profile's acceleration stat for one character (byte 1 of the character's stat record) from the current-stat
 * table.
 *
 * Reads D_00535538[@p humanIndex][@p charType][1] (On Tour tBEStatDB.cAccel).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the acceleration stat (5 per level)
 */
extern "C" signed char cBEStatInterface_getHumanAccel(cBEStatInterface* self, int humanIndex, int charType)
{
    return D_00535538[humanIndex].e[charType].v[1];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanTrick);
#ifdef SKIP_ASM
/**
 * Stores a profile's trick stat for one character (byte 2 of the character's stat record) in the current-stat table.
 *
 * Writes @p value to D_00535538[@p humanIndex][@p charType][2] (On Tour tBEStatDB.cTrick). Unlike the rider-slot setters, it takes
 * the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
extern "C" void cBEStatInterface_setHumanTrick(cBEStatInterface* self, int humanIndex, int value, int charType)
{
    D_00535538[humanIndex].e[charType].v[2] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanTrick);
#ifdef SKIP_ASM
/**
 * Returns a profile's trick stat for one character (byte 2 of the character's stat record) from the current-stat table.
 *
 * Reads D_00535538[@p humanIndex][@p charType][2] (On Tour tBEStatDB.cTrick).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the trick stat (5 per level)
 */
extern "C" signed char cBEStatInterface_getHumanTrick(cBEStatInterface* self, int humanIndex, int charType)
{
    return D_00535538[humanIndex].e[charType].v[2];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanEdging);
#ifdef SKIP_ASM
/**
 * Stores a profile's edging stat for one character (byte 3 of the character's stat record) in the current-stat table.
 *
 * Writes @p value to D_00535538[@p humanIndex][@p charType][3] (On Tour tBEStatDB.cEdging). Unlike the rider-slot setters, it takes
 * the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
extern "C" void cBEStatInterface_setHumanEdging(cBEStatInterface* self, int humanIndex, int value, int charType)
{
    D_00535538[humanIndex].e[charType].v[3] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanEdging);
#ifdef SKIP_ASM
/**
 * Returns a profile's edging stat for one character (byte 3 of the character's stat record) from the current-stat
 * table.
 *
 * Reads D_00535538[@p humanIndex][@p charType][3] (On Tour tBEStatDB.cEdging).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the edging stat (5 per level)
 */
extern "C" signed char cBEStatInterface_getHumanEdging(cBEStatInterface* self, int humanIndex, int charType)
{
    return D_00535538[humanIndex].e[charType].v[3];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanSpin);
#ifdef SKIP_ASM
/**
 * Stores a profile's spin stat for one character (byte 4 of the character's stat record) in the current-stat table.
 *
 * Writes @p value to D_00535538[@p humanIndex][@p charType][4] (On Tour tBEStatDB.cSpin). Unlike the rider-slot setters, it takes the
 * profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
extern "C" void cBEStatInterface_setHumanSpin(cBEStatInterface* self, int humanIndex, int value, int charType)
{
    D_00535538[humanIndex].e[charType].v[4] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanSpin);
#ifdef SKIP_ASM
/**
 * Returns a profile's spin stat for one character (byte 4 of the character's stat record) from the current-stat table.
 *
 * Reads D_00535538[@p humanIndex][@p charType][4] (On Tour tBEStatDB.cSpin).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the spin stat (5 per level)
 */
extern "C" signed char cBEStatInterface_getHumanSpin(cBEStatInterface* self, int humanIndex, int charType)
{
    return D_00535538[humanIndex].e[charType].v[4];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanToughness);
#ifdef SKIP_ASM
/**
 * Stores a profile's toughness stat for one character (byte 5 of the character's stat record) in the current-stat
 * table.
 *
 * Writes @p value to D_00535538[@p humanIndex][@p charType][5] (On Tour tBEStatDB.cToughness). Unlike the rider-slot setters, it
 * takes the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
extern "C" void cBEStatInterface_setHumanToughness(cBEStatInterface* self, int humanIndex, int value, int charType)
{
    D_00535538[humanIndex].e[charType].v[5] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanToughness);
#ifdef SKIP_ASM
/**
 * Returns a profile's toughness stat for one character (byte 5 of the character's stat record) from the current-stat
 * table.
 *
 * Reads D_00535538[@p humanIndex][@p charType][5] (On Tour tBEStatDB.cToughness).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the toughness stat (5 per level)
 */
extern "C" signed char cBEStatInterface_getHumanToughness(cBEStatInterface* self, int humanIndex, int charType)
{
    return D_00535538[humanIndex].e[charType].v[5];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanStability);
#ifdef SKIP_ASM
/**
 * Stores a profile's stability stat for one character (byte 6 of the character's stat record) in the current-stat
 * table.
 *
 * Writes @p value to D_00535538[@p humanIndex][@p charType][6] (On Tour tBEStatDB.cStability). Unlike the rider-slot setters, it
 * takes the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
extern "C" void cBEStatInterface_setHumanStability(cBEStatInterface* self, int humanIndex, int value, int charType)
{
    D_00535538[humanIndex].e[charType].v[6] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanStability);
#ifdef SKIP_ASM
/**
 * Returns a profile's stability stat for one character (byte 6 of the character's stat record) from the current-stat
 * table.
 *
 * Reads D_00535538[@p humanIndex][@p charType][6] (On Tour tBEStatDB.cStability).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the stability stat (5 per level)
 */
extern "C" signed char cBEStatInterface_getHumanStability(cBEStatInterface* self, int humanIndex, int charType)
{
    return D_00535538[humanIndex].e[charType].v[6];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getBrakingAttrib);
#ifdef SKIP_ASM
int cBENewPlayerInterface_isMissionMan(int index);

struct sCharStat_005308D8
{
    signed char v[15];
};
extern sCharStat_005308D8 D_005308D8[];

/**
 * Returns a rider's braking attribute as a fraction of the character's maximum edging level.
 *
 * Mission riders (cBENewPlayerInterface_isMissionMan()) get 0.5. The level is @p value when positive, else the
 * profile's edging stat for the character (D_00535538[profile][character][3], On Tour cEdging) divided by 5; the result
 * is that level over the character's cMaxEdging (byte 0xB of its D_005308D8 default-stat record). cBEStatInterface_getBrakingAttrib and
 * cBEStatInterface_getEdgingAttrib have identical bodies.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  attribute level to use instead of the stored one, when positive (On Tour overrideAttribLevel)
 * @return the attribute level divided by the character's edging maximum
 */
extern "C" float cBEStatInterface_getBrakingAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int attribLevel;
    if (value > 0)
        attribLevel = value;
    else
        attribLevel = D_00535538[profile].e[charID].v[3] / 5;
    return (float)attribLevel / (float)D_005308D8[charID].v[0xB];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getEdgingAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBENewPlayerInterface_isMissionMan(int index);

struct sCharStat_00148E68
{
    signed char v[15];
};
extern sCharStat_00148E68 D_005308D8_00148E68[] __asm__("D_005308D8");

/**
 * Returns a rider's edging attribute as a fraction of the character's maximum edging level.
 *
 * Mission riders (cBENewPlayerInterface_isMissionMan()) get 0.5. The level is @p value when positive, else the
 * profile's edging stat for the character (D_00535538[profile][character][3], On Tour cEdging) divided by 5; the result
 * is that level over the character's cMaxEdging (byte 0xB of its D_005308D8 default-stat record). cBEStatInterface_getBrakingAttrib and
 * cBEStatInterface_getEdgingAttrib have identical bodies.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  attribute level to use instead of the stored one, when positive (On Tour overrideAttribLevel)
 * @return the attribute level divided by the character's edging maximum
 */
extern "C" float cBEStatInterface_getEdgingAttrib(cBEStatInterface* self, int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int attribLevel;
    if (value > 0)
        attribLevel = value;
    else
        attribLevel = D_00535538[profile].e[charID].v[3] / 5;
    return (float)attribLevel / (float)D_005308D8_00148E68[charID].v[0xB];
}
#endif

