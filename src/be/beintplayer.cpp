#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "be/beprofilelib.h"
#include "be/bedbprofile.h"
#include "be/beintplayer.h"
#include "be/beintnewrace.h"
#include "be/beintstat.h"
#include "be/beintnetwork.h"
#include "be/beintoption.h"
#include "main/ssxapp.h"
#include "util/locale.h"
extern int cBELibrary_raceDB[];


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
    ((cBENewPlayerInterface*)self)->defaultCtrl();
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
extern "C" void* memcpy(void*, const void*, unsigned int);
// PORT: cBENewPlayerInterface_ingameSubmit takes no arguments; this caller passes its own self through untouched in $a0.
extern "C" void cBENewPlayerInterface_ingameSubmit_59B8(void* self) __asm__("cBENewPlayerInterface_ingameSubmit");

/**
 * Commits the working player table cBENewPlayerInterface_tempPlayers to the library's cBELibrary_playerDB.
 *
 * When the player interface singleton's flag at +0x8 is set the whole 0xA8-byte table is copied; otherwise
 * only the names and iUseDualShock (flag bit 1) of the six entries (cBENewPlayerInterface_ingameSubmit).
 */
extern "C" void cBENewPlayerInterface_submit(cBENewPlayerInterface* self)
{
    if (cBENewPlayerInterface_instance->bIsActive != 0) {
        memcpy(cBELibrary_playerDB, cBENewPlayerInterface_tempPlayers, sizeof(cBELibrary_playerDB));
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

/**
 * Commits part of the player table: for each of the six entries, copies iUseDualShock (pad vibration) and the 12-byte name
 * from cBENewPlayerInterface_tempPlayers to cBELibrary_playerDB.
 *
 * cBENewPlayerInterface_submit runs it instead of the whole-table copy while the interface's word at +0x8 is clear.
 */
extern "C" void cBENewPlayerInterface_ingameSubmit(void)
{
    for (int i = 0; i < 6; i++)
    {
        cBELibrary_playerDB[i].iUseDualShock = cBENewPlayerInterface_tempPlayers[i].iUseDualShock;
        strncpy(cBELibrary_playerDB[i].strUserName, cBENewPlayerInterface_tempPlayers[i].strUserName, 0xC);
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_discard);
#ifdef SKIP_ASM
/**
 * Throws away player-table edits: copies cBELibrary_playerDB back over the working table cBENewPlayerInterface_tempPlayers.
 */
extern "C" void cBENewPlayerInterface_discard(cBENewPlayerInterface* self)
{
    memcpy(cBENewPlayerInterface_tempPlayers, cBELibrary_playerDB, sizeof(cBENewPlayerInterface_tempPlayers));
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_defaultCtrl);
#ifdef SKIP_ASM

/**
 * Resets the library's six player entries (cControlMap cleared, iUseDualShock set), then reloads the working copy.
 *
 * The reload is the interface's slot 2 (discard).
 */
void cBENewPlayerInterface::defaultCtrl()
{
    int i;
    cBEPlayerDB* entry = cBELibrary_playerDB;
    for (i = 0; i < 6; i++)
    {
        entry[i].cControlMap = 0;
        entry[i].iUseDualShock = 1;
    }
    discard();
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isFirstTimeWC);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns a rider's first-time-in-career flag: bFirstTimeWorldCircuit of its character record in cBELibrary_profileLib.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].cCharacterID.
 * Starting the career with the flag set opens on location 14, where the rider starts at the back-country drop-off.
 *
 * @param index  the rider (On Tour iRider)
 * @return the flag word
 */
int cBENewPlayerInterface::isFirstTimeWC(int index)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    return cBELibrary_profileLib[profile][charId].bFirstTimeWorldCircuit;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setIsFirstTimeWC);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Sets a rider's first-time-in-career flag: bFirstTimeWorldCircuit of its character record in cBELibrary_profileLib.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].cCharacterID.
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the new flag
 */
void cBENewPlayerInterface::setIsFirstTimeWC(int index, int value)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    cBELibrary_profileLib[profile][charId].bFirstTimeWorldCircuit = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isFirstTimeOnTrack);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Tells whether a rider has never been to a location before.
 *
 * Uses the profile of @p index (cBELibrary_getProfileIndex) and the character of its player
 * (cBENewPlayerInterface_tempPlayers[getPlayerID].cCharacterID) to find the cBELibrary_profileLib record, then tests bit @p
 * bit of the visited mask mVisitLocFlags.
 *
 * @param index  the rider (On Tour iRider)
 * @param bit  the location (On Tour eBELocationID locTrack)
 * @return true when the location has not been visited
 */
bool cBENewPlayerInterface::isFirstTimeOnTrack(int index, int bit)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int characterId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    int mask = 1 << bit;
    // The profile's row first, then the character: retail adds the profile offset before the character's.
    return ((*(cBELibrary_profileLib + profile))[characterId].mVisitLocFlags & mask) == 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setFirstTimeOnTrack);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];
extern "C" void cBECharProfileDB_setLocationVisited(void* self, int bit, int on);


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
void cBENewPlayerInterface::setFirstTimeOnTrack(bool flag, int index, int bit)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int characterId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    cBECharProfileDB_setLocationVisited(&cBELibrary_profileLib[profile][characterId], bit, !flag);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_hasVisitedPeakWC);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];
extern "C" int cBECharProfileDB_hasVisitedPeak(void* self, int kind);


/**
 * Returns whether a rider's character has visited any location of a peak.
 *
 * Passes the rider's character record (profile cBELibrary_getProfileIndex(@p index), character
 * tempPlayers[getPlayerID(@p index)].cCharacterID) to cBECharProfileDB_hasVisitedPeak, which tests its visited-location mask.
 *
 * @param index  the rider (On Tour iRider)
 * @param kind  the peak, 0-2
 * @return 1 when visited, else 0
 */
int cBENewPlayerInterface::hasVisitedPeakWC(int index, int kind)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    return cBECharProfileDB_hasVisitedPeak(&cBELibrary_profileLib[profile][charId], kind);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_hasPlayedEvent);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];
void* cBECharProfileDB_getScoreStats(void* self, int a, int b);


/**
 * Returns whether a rider's character has a result for an event on a location: the first byte of its score record.
 *
 * The record is cBECharProfileDB_getScoreStats(@p locationId, @p event) on the rider's character record (profile
 * cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].cCharacterID). There is no null
 * check: for a pair with no score record getScoreStats returns 0 and the byte is read from address 0.
 *
 * @param index  the rider (On Tour iRider)
 * @param locationId  the location (On Tour eBELocationID)
 * @param event  the game event (On Tour eBEGameEvent)
 * @return true when the byte is nonzero
 */
bool cBENewPlayerInterface::hasPlayedEvent(int index, int locationId, int event)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    signed char* stats = (signed char*)cBECharProfileDB_getScoreStats(&cBELibrary_profileLib[profile][charId], locationId, event);
    return *stats != 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isPeakLocked);
#ifdef SKIP_ASM
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Tells whether a peak is locked for a character: bPeak2Locked (peak 1) or bPeak3Locked (peak 2) of its profile record.
 *
 * Peak 0 is never locked; the flags share a 64-bit unit at +0x278 of the character's profile record.
 *
 * @param iProfileIndex  profile index
 * @param iCharacterID  character id
 * @param peak  the peak (eBEPeakID), 0-2
 * @return 1 when locked, else 0
 */
// PORT: the peak flags share a 64-bit unit with locLastHub (ld at +0x278).
int cBENewPlayerInterface::isPeakLocked(int iProfileIndex, int iCharacterID, int peak)
{
    if (peak != kPEAK_1)
    {
        if (peak == kPEAK_2)
        {
            return cBELibrary_profileLib[iProfileIndex][iCharacterID].bPeak2Locked;
        }
        else
        {
            return cBELibrary_profileLib[iProfileIndex][iCharacterID].bPeak3Locked;
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
int cBENewPlayerInterface::isPeakLocked1(int rider, int peak)
{
    int profile = cBELibrary_getProfileIndex(rider);
    return isPeakLocked(profile, cBELibrary_getCharacterID(rider), peak);
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setPeakLocked);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Locks or unlocks a peak for a rider's character: bPeak2Locked of its profile record for peak 1, bPeak3Locked for
 * any other.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character
 * tempPlayers[getPlayerID(@p index)].cCharacterID; the bits cBENewPlayerInterface_isPeakLocked reads.
 *
 * @param index  the rider (On Tour iRider)
 * @param kind  the peak (eBEPeakID): kPEAK_2, or kPEAK_3 (any other value also writes bit 13)
 * @param value  1 to lock, 0 to unlock
 */
void cBENewPlayerInterface::setPeakLocked(int index, int kind, int value)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    signed char charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    if (kind == kPEAK_2) {
        cBELibrary_profileLib[profile][charId].bPeak2Locked = value;
    } else {
        cBELibrary_profileLib[profile][charId].bPeak3Locked = value;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isRivalChallengePlayed);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns whether a rider's character has played a peak's rival challenge.
 *
 * Reads bPeak<n>RivalTimeChallengePlayed (when @p flag is set) or bPeak<n>RivalPtsChallengePlayed (n = @p kind + 1)
 * of the character record (profile cBELibrary_getProfileIndex(@p index), character
 * tempPlayers[getPlayerID(@p index)].cCharacterID): bit 3 + @p kind of the flags at +0x278, else bit @p kind.
 *
 * @param index  the rider (On Tour iRider)
 * @param kind  the peak (eBEPeakID), 0-2; any other gives 0
 * @param flag  selects bits 3-5 when nonzero, bits 0-2 when 0 (the end-of-event script passes game mode == 0)
 * @return the bit, 0 or 1
 */
// PORT: the peak flags share a 64-bit unit with locLastHub (ld at +0x278).
int cBENewPlayerInterface::isRivalChallengePlayed(int index, int kind, int flag)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    signed char charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    if (flag) {
        if (kind == kPEAK_1) {
            return cBELibrary_profileLib[profile][charId].bPeak1RivalTimeChallengePlayed;
        } else if (kind == kPEAK_2) {
            return cBELibrary_profileLib[profile][charId].bPeak2RivalTimeChallengePlayed;
        } else if (kind == kPEAK_3) {
            return cBELibrary_profileLib[profile][charId].bPeak3RivalTimeChallengePlayed;
        }
    } else {
        if (kind == kPEAK_1) {
            return cBELibrary_profileLib[profile][charId].bPeak1RivalPtsChallengePlayed;
        } else if (kind == kPEAK_2) {
            return cBELibrary_profileLib[profile][charId].bPeak2RivalPtsChallengePlayed;
        } else if (kind == kPEAK_3) {
            return cBELibrary_profileLib[profile][charId].bPeak3RivalPtsChallengePlayed;
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
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Sets or clears the played mark of a peak's rival challenge for a rider's character.
 *
 * Writes bPeak<n>RivalTimeChallengePlayed (when @p flag is set) or bPeak<n>RivalPtsChallengePlayed (n = @p kind + 1)
 * of the character record (profile cBELibrary_getProfileIndex(@p index), character
 * tempPlayers[getPlayerID(@p index)].cCharacterID), the bit cBENewPlayerInterface_isRivalChallengePlayed reads. A peak
 * outside 0-2 writes nothing.
 *
 * @param index  the rider (On Tour iRider)
 * @param kind  the peak (eBEPeakID), 0-2
 * @param flag  selects bits 3-5 when nonzero, bits 0-2 when 0
 * @param value  the new bit
 */
void cBENewPlayerInterface::setRivalChallengePlayed(int index, int kind, int flag, int value)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    signed char charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    if (flag) {
        if (kind == kPEAK_1) {
            cBELibrary_profileLib[profile][charId].bPeak1RivalTimeChallengePlayed = value;
        } else if (kind == kPEAK_2) {
            cBELibrary_profileLib[profile][charId].bPeak2RivalTimeChallengePlayed = value;
        } else if (kind == kPEAK_3) {
            cBELibrary_profileLib[profile][charId].bPeak3RivalTimeChallengePlayed = value;
        }
    } else {
        if (kind == kPEAK_1) {
            cBELibrary_profileLib[profile][charId].bPeak1RivalPtsChallengePlayed = value;
        } else if (kind == kPEAK_2) {
            cBELibrary_profileLib[profile][charId].bPeak2RivalPtsChallengePlayed = value;
        } else if (kind == kPEAK_3) {
            cBELibrary_profileLib[profile][charId].bPeak3RivalPtsChallengePlayed = value;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isRivalEventLocked);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns whether a peak's rival event is still locked for a character.
 *
 * Reads bPeak<n>TimeRCLocked (when @p flag is set) or bPeak<n>FreeRCLocked (n = @p kind + 1) of character
 * @p character's record in the profile of @p index (cBELibrary_getProfileIndex): bit 6 + @p kind of the flags at
 * +0x278, else bit 9 + @p kind.
 *
 * @param index  the rider whose profile is used (On Tour iRider)
 * @param character  the character (On Tour eBECharacters)
 * @param kind  the peak (eBEPeakID), 0-2; any other gives 0
 * @param flag  selects bits 6-8 when nonzero, bits 9-11 when 0 (the map passes 1 for event kind 4, 0 for kind 5)
 * @return 1 when locked, else 0
 */
// PORT: the peak flags share a 64-bit unit with locLastHub (ld at +0x278).
int cBENewPlayerInterface::isRivalEventLocked(int index, int character, int kind, int flag)
{
    int profile = cBELibrary_getProfileIndex(index);
    if (flag) {
        switch (kind) {
        case kPEAK_1:
            {
            return cBELibrary_profileLib[profile][character].bPeak1TimeRCLocked;
        }
        case kPEAK_2:
            {
            return cBELibrary_profileLib[profile][character].bPeak2TimeRCLocked;
        }
        case kPEAK_3:
            {
            return cBELibrary_profileLib[profile][character].bPeak3TimeRCLocked;
        }
        }
    } else {
        switch (kind) {
        case kPEAK_1:
            {
            return cBELibrary_profileLib[profile][character].bPeak1FreeRCLocked;
        }
        case kPEAK_2:
            {
            return cBELibrary_profileLib[profile][character].bPeak2FreeRCLocked;
        }
        case kPEAK_3:
            {
            return cBELibrary_profileLib[profile][character].bPeak3FreeRCLocked;
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
extern cBECharProfileDB cBELibrary_profileLib[][10];
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
 * +0x40). When the committed game mode (cBELibrary_raceDB byte +0x48) is kGAMEMODE_TIMECHALLENGE, reads
 * bPeak<n>TimeRCLocked (n = peak + 1) of the character record (profile cBELibrary_getProfileIndex(@p index), character
 * tempPlayers[getPlayerID(@p index)].cCharacterID), else bPeak<n>FreeRCLocked. A peak outside 0-2 gives 0.
 *
 * @param index  the rider (On Tour iRider)
 * @return 1 when locked, else 0
 */
// PORT: the peak flags share a 64-bit unit with locLastHub (ld at +0x278).
int cBENewPlayerInterface::isRivalEventLocked1(int index)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    signed char charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    int kind = cBELibrary_locationDB[cBELibrary_raceDB[16]].kind;
    if (*(signed char*)&cBELibrary_raceDB[18] == kGAMEMODE_TIMECHALLENGE) {
        switch (kind) {
        case kPEAK_1:
            {
            return cBELibrary_profileLib[profile][charId].bPeak1TimeRCLocked;
        }
        case kPEAK_2:
            {
            return cBELibrary_profileLib[profile][charId].bPeak2TimeRCLocked;
        }
        case kPEAK_3:
            {
            return cBELibrary_profileLib[profile][charId].bPeak3TimeRCLocked;
        }
        }
    } else {
        switch (kind) {
        case kPEAK_1:
            {
            return cBELibrary_profileLib[profile][charId].bPeak1FreeRCLocked;
        }
        case kPEAK_2:
            {
            return cBELibrary_profileLib[profile][charId].bPeak2FreeRCLocked;
        }
        case kPEAK_3:
            {
            return cBELibrary_profileLib[profile][charId].bPeak3FreeRCLocked;
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
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Locks or unlocks a peak's rival event for a character.
 *
 * Writes bPeak<n>TimeRCLocked (when @p flag is set) or bPeak<n>FreeRCLocked (n = @p kind + 1) of character
 * @p character's record in the profile of @p index (cBELibrary_getProfileIndex), the bit
 * cBENewPlayerInterface_isRivalEventLocked reads. A peak outside 0-2 writes nothing.
 *
 * @param index  the rider whose profile is used (On Tour iRider)
 * @param character  the character (On Tour eBECharacters)
 * @param kind  the peak (eBEPeakID), 0-2
 * @param flag  selects bits 6-8 when nonzero, bits 9-11 when 0
 * @param value  1 to lock, 0 to unlock
 */
void cBENewPlayerInterface::setRivalEventLocked(int index, int character, int kind, int flag, int value)
{
    int profile = cBELibrary_getProfileIndex(index);
    if (flag) {
        switch (kind) {
        case kPEAK_1:
            cBELibrary_profileLib[profile][character].bPeak1TimeRCLocked = value;
            break;
        case kPEAK_2:
            cBELibrary_profileLib[profile][character].bPeak2TimeRCLocked = value;
            break;
        case kPEAK_3:
            cBELibrary_profileLib[profile][character].bPeak3TimeRCLocked = value;
            break;
        }
    } else {
        switch (kind) {
        case kPEAK_1:
            cBELibrary_profileLib[profile][character].bPeak1FreeRCLocked = value;
            break;
        case kPEAK_2:
            cBELibrary_profileLib[profile][character].bPeak2FreeRCLocked = value;
            break;
        case kPEAK_3:
            cBELibrary_profileLib[profile][character].bPeak3FreeRCLocked = value;
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
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Tells whether an event is still locked for a character.
 *
 * Reads cBELibrary_profileLib[profile of @p index][@p character]'s lock flags: events 6-11 map to bPeak1TCLocked ..
 * bPeak3TCLocked and bPeak1FCLocked .. bPeak3FCLocked (bits 14-19); other events return 0 (unlocked).
 *
 * @param index  the rider whose profile is used (On Tour iRider)
 * @param character  the character (On Tour eBECharacters character)
 * @param kind  the event (On Tour eBEGameEvent event)
 * @return 1 when locked
 */
// PORT: the peak flags share a 64-bit unit with locLastHub (ld at +0x278).
int cBENewPlayerInterface::isEventLocked(int index, int character, int kind)
{
    int profile = cBELibrary_getProfileIndex(index);
    switch (kind) {
    case kGAMEEVENT_TIME_PEAK1: {
        return cBELibrary_profileLib[profile][character].bPeak1TCLocked;
    }
    case kGAMEEVENT_TIME_PEAK2: {
        return cBELibrary_profileLib[profile][character].bPeak2TCLocked;
    }
    case kGAMEEVENT_TIME_PEAK3: {
        return cBELibrary_profileLib[profile][character].bPeak3TCLocked;
    }
    case kGAMEEVENT_POINTS_PEAK1: {
        return cBELibrary_profileLib[profile][character].bPeak1FCLocked;
    }
    case kGAMEEVENT_POINTS_PEAK2: {
        return cBELibrary_profileLib[profile][character].bPeak2FCLocked;
    }
    case kGAMEEVENT_POINTS_PEAK3: {
        return cBELibrary_profileLib[profile][character].bPeak3FCLocked;
    }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setEventLocked);
#ifdef SKIP_ASM
/**
 * Locks or unlocks an event for a character: bit 8 + @p event (events 6-11, bits 14-19) of its profile flags.
 *
 * Writes bPeak1TCLocked .. bPeak3FCLocked of character @p character's record in the profile of @p rider
 * (cBELibrary_getProfileIndex), the bits cBENewPlayerInterface_isEventLocked reads. Other events write nothing.
 *
 * @param rider  the rider whose profile is used (On Tour iRider)
 * @param character  the character (On Tour eBECharacters)
 * @param event  the game event (eBEGameEvent), kGAMEEVENT_TIME_PEAK1 to kGAMEEVENT_POINTS_PEAK3
 * @param locked  1 to lock, 0 to unlock
 */
void cBENewPlayerInterface::setEventLocked(int rider, int character, int event, int locked)
{
    int profile = cBELibrary_getProfileIndex(rider);
    switch (event) {
    case kGAMEEVENT_TIME_PEAK1:
        cBELibrary_profileLib[profile][character].bPeak1TCLocked = locked;
        break;
    case kGAMEEVENT_TIME_PEAK2:
        cBELibrary_profileLib[profile][character].bPeak2TCLocked = locked;
        break;
    case kGAMEEVENT_TIME_PEAK3:
        cBELibrary_profileLib[profile][character].bPeak3TCLocked = locked;
        break;
    case kGAMEEVENT_POINTS_PEAK1:
        cBELibrary_profileLib[profile][character].bPeak1FCLocked = locked;
        break;
    case kGAMEEVENT_POINTS_PEAK2:
        cBELibrary_profileLib[profile][character].bPeak2FCLocked = locked;
        break;
    case kGAMEEVENT_POINTS_PEAK3:
        cBELibrary_profileLib[profile][character].bPeak3FCLocked = locked;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getLastHub);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns the last hub location of a rider's character (locLastHub of its profile record).
 *
 * @param index  the rider (On Tour: iRider); mapped to the profile and the player's character
 * @return the location id
 */
int cBENewPlayerInterface::getLastHub(int index)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charID = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    return cBELibrary_profileLib[profile][charID].locLastHub;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setLastHub);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Remembers the hub a rider's character was last at.
 *
 * Writes @p value to locLastHub of the rider's character profile record in cBELibrary_profileLib.
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the hub location (On Tour locHub)
 */
void cBENewPlayerInterface::setLastHub(int index, int value)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int characterId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    cBELibrary_profileLib[profile][characterId].locLastHub = value;
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
int cBENewPlayerInterface::getHumanRider(int second)
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
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
 * field (eOptionSpeechLanguage, bits 22-24 of cBEOptionInterface_mOption[0]; a language above 3 leaves it empty), and
 * returns that name.
 *
 * @param idx  the rider
 * @return the name string
 */
// Returns the display name for player idx (network names in a net game, else a default "Player N").
extern "C" char* func_00146F88(cBENewPlayerInterface* self, int idx)
{
    cBE_getInterface_Fv(SSXApp->BE, 7);
    tBENetworkDB* net = &cBENetworkInterface_networkOpt;
    if (*(int*)net != 0 && idx < 2) {
        if (net->bIsHost != 0) {
            if (idx == 0)
                return ((char*)net) + 0x488;
            return ((char*)net) + 0x498;
        }
        if (idx == 0)
            return ((char*)net) + 0x498;
        return ((char*)net) + 0x488;
    }
    if (cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].strUserName[0] == 0) {
        cBE_getInterface_Fv(cBE_getBE(), 4);
        switch ((int)((cBEOptionInterface_mOption[0] >> 22) & 7)) {
        case kOPTLANG_English:
            sprintf(cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].strUserName, str_PLAYER_pct_d, idx + 1);
            break;
        case kOPTLANG_French:
            sprintf(cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].strUserName, str_JOUEUR_pct_d, idx + 1);
            break;
        case kOPTLANG_German:
            sprintf(cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].strUserName, str_SPIELER_pct_d, idx + 1);
            break;
        case kOPTLANG_Spanish:
            sprintf(cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].strUserName, str_JUGADOR_pct_d, idx + 1);
            break;
        }
    }
    return cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(idx)].strUserName;
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
void cBENewPlayerInterface::setPlayerName(int playerIndex, const char* name)
{
    strncpy(cBENewPlayerInterface_tempPlayers[playerIndex].strUserName, name, 0xC);
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



/**
 * Returns a player's name, filling in the default "kT_MEMPlayerName" text with the player number when empty.
 *
 * @param idx  the player slot (On Tour: iPlayer)
 * @return the name stored in the player table
 */
char* cBENewPlayerInterface::getPlayerName(int idx)
{
    char* name = (char*)&cBENewPlayerInterface_tempPlayers[idx];
    if (strlen(name) == 0)
    {
        unsigned short buf[104];
        cFELocale* db = SSXApp->FELocale;
        sVEntry<unsigned short*(void*, int)>* vt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, db);
        sprintf1(buf, vt[4].fn(vthis(db, vt[4]), GetHashValue32(str_kT_MEMPlayerName)), idx + 1);
        strcpy2(name, buf);
    }
    return name;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setRiderCtrlID);
#ifdef SKIP_ASM
int cBENewPlayerInterface_getPlayerID(int index);

/**
 * Assigns a controller to a rider's player entry.
 *
 * A valid controller (0 or more) also clears the mission-man bit and stores the player id in the 1-bit field
 * bit 0 (so only its low bit).
 *
 * @param index  the rider; mapped to its player with cBENewPlayerInterface_getPlayerID()
 * @param ctrl  the controller id, or negative for none
 */
void cBENewPlayerInterface::setRiderCtrlID(int index, int ctrl)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBENewPlayerInterface_tempPlayers[id].iControllerID = ctrl;
    if (ctrl >= 0) {
        cBEPlayerDB* entry = &cBENewPlayerInterface_tempPlayers[id];
        entry->iHumanID = id;
        entry->iIsMissionMan = 0;
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
 * cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)].iControllerID; negative means no controller (an AI rider), see
 * cBENewPlayerInterface_setPlayerCtrlID.
 *
 * @param index  the rider (On Tour iRider)
 * @return the controller id
 */
int cBENewPlayerInterface::getRiderCtrlID(int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    return cBENewPlayerInterface_tempPlayers[id].iControllerID;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setPlayerCtrlID);
#ifdef SKIP_ASM
/**
 * Assigns a controller to a player slot.
 *
 * cBENewPlayerInterface_tempPlayers[@p iPlayer].iControllerID = @p ctrlID; with a controller (@p ctrlID >= 0), iHumanID (flag bit 0) of the
 * record's +0x10 word takes the low bit of @p iPlayer and bit 2 is cleared. Storing the slot's low bit in a 1-bit flag looks
 * like it should have been a constant.
 *
 * @param iPlayer  the player slot (On Tour iPlayer)
 * @param ctrlID  the controller id, negative for none
 */
void cBENewPlayerInterface::setPlayerCtrlID(int iPlayer, int ctrlID)
{
    cBENewPlayerInterface_tempPlayers[iPlayer].iControllerID = ctrlID;
    if (ctrlID >= 0) {
        cBEPlayerDB* playerRec = &cBENewPlayerInterface_tempPlayers[iPlayer];
        playerRec->iHumanID = iPlayer;
        playerRec->iIsMissionMan = 0;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getPlayerCtrlID);
#ifdef SKIP_ASM
/**
 * Returns which controller is assigned to a player slot.
 *
 * cBENewPlayerInterface_tempPlayers[@p iPlayer].iControllerID, the field cBENewPlayerInterface_setPlayerCtrlID writes.
 *
 * @param iPlayer  the player slot (On Tour iPlayer)
 * @return the controller id, negative for none
 */
int cBENewPlayerInterface::getPlayerCtrlID(int iPlayer)
{
    return cBENewPlayerInterface_tempPlayers[iPlayer].iControllerID;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setRiderCharID);
#ifdef SKIP_ASM
static inline int func_00147338_isLocked(cBEPlayerDB* e)
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
void cBENewPlayerInterface::setRiderCharID(int index, int charID)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBEPlayerDB* entry = &cBENewPlayerInterface_tempPlayers[id];
    if (func_00147338_isLocked(entry) == true)
    {
        entry->cCharacterID = cBENewPlayerInterface_tempPlayers[0].cCharacterID;
    }
    else
    {
        entry->cCharacterID = charID;
    }
}
#endif

int cBENewPlayerInterface_getPlayerID(int index);

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getRiderCharID__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the character ID of the player in a rider slot (cCharacterID of its player record).
 *
 * @param riderIndex  rider slot, mapped to a player through cBENewPlayerInterface_getPlayerID()
 * @return the character ID
 */
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex)
{
    int playerID = cBENewPlayerInterface_getPlayerID(riderIndex);
    cBEPlayerDB* entry = &cBENewPlayerInterface_tempPlayers[playerID];
    return entry->cCharacterID;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setRiderCheatCharID);
#ifdef SKIP_ASM
/**
 * Sets the cheat character a rider appears as.
 *
 * cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)].cCheatCharacterID = @p value (read back by
 * cBELibrary_getCheatCharacterID).
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the cheat character id, 0 for none
 */
void cBENewPlayerInterface::setRiderCheatCharID(int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBENewPlayerInterface_tempPlayers[id].cCheatCharacterID = value;
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
signed char cBENewPlayerInterface::getRiderCheatCharID(int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    return cBENewPlayerInterface_tempPlayers[id].cCheatCharacterID;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setPlayerCharID);
#ifdef SKIP_ASM
// The bit is read through an int-returning helper: that is what keeps setPlayerCharID's compare as retail has it.
static inline int cBENewPlayerInterface_setPlayerCharID_isMissionMan(cBEPlayerDB* p)
{
    return p->iIsMissionMan;
}

/**
 * Chooses the character a player slot rides as.
 *
 * Writes @p charID to byte +0x11 (cCharacterID) of cBENewPlayerInterface_tempPlayers[@p iPlayer]; when the slot's iIsMissionMan (flag bit 2)
 * (mission man, set by cBENewPlayerInterface_setToMissionMan) is set, player 0's character is stored instead.
 *
 * @param iPlayer  the player slot (On Tour iPlayer)
 * @param charID  the character to ride as
 */
void cBENewPlayerInterface::setPlayerCharID(int iPlayer, int charID)
{
    cBEPlayerDB* playerRec = &cBENewPlayerInterface_tempPlayers[iPlayer];
    if (cBENewPlayerInterface_setPlayerCharID_isMissionMan(playerRec) == true) {
        playerRec->cCharacterID = cBENewPlayerInterface_tempPlayers[0].cCharacterID;
    } else {
        playerRec->cCharacterID = charID;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getPlayerCharID__FPvi);
#ifdef SKIP_ASM
/**
 * Returns a player's character id (cCharacterID, +0x11 of its player table entry).
 *
 * @param index  the player slot
 * @return the character id
 */
signed char cBENewPlayerInterface_getPlayerCharID(void* self, int index)
{
    return cBENewPlayerInterface_tempPlayers[index].cCharacterID;
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
void cBENewPlayerInterface::setPlayerCheatCharID(int playerId, int value)
{
    cBEPlayerDB* playerEntry = &cBENewPlayerInterface_tempPlayers[playerId];
    playerEntry->cCheatCharacterID = value;
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
signed char cBENewPlayerInterface::getPlayerCheatCharID(int playerId)
{
    cBEPlayerDB* playerEntry = &cBENewPlayerInterface_tempPlayers[playerId];
    return playerEntry->cCheatCharacterID;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getUseDualShock);
#ifdef SKIP_ASM
/**
 * Returns whether a rider's pad vibration is on: iUseDualShock (flag bit 1) at +0x10 of its player entry.
 *
 * The entry is cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]; cBENewPlayerInterface_defaultCtrl sets the
 * bit for every entry.
 *
 * @param index  the rider (On Tour iRider)
 * @return 1 when on, else 0
 */
int cBENewPlayerInterface::getUseDualShock(int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBEPlayerDB* playerEntry = &cBENewPlayerInterface_tempPlayers[id];
    return playerEntry->iUseDualShock;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setUseDualShock);
#ifdef SKIP_ASM
/**
 * Turns a rider's pad vibration on or off: iUseDualShock (flag bit 1) at +0x10 of tempPlayers[getPlayerID(@p index)].
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the new setting
 */
void cBENewPlayerInterface::setUseDualShock(int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBEPlayerDB* playerEntry = &cBENewPlayerInterface_tempPlayers[id];
    playerEntry->iUseDualShock = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getHelpDone);
#ifdef SKIP_ASM
/**
 * Returns iIsHelpDone (flag bit 3) at +0x10 of a rider's player entry (tempPlayers[getPlayerID(@p index)]).
 *
 * The bit is set once the world script has opened the message center (func_001E3510), after which func_001E3570 stops
 * offering it.
 *
 * @param index  the rider (On Tour iRider)
 * @return the bit, 0 or 1
 */
int cBENewPlayerInterface::getHelpDone(int index)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBEPlayerDB* playerEntry = &cBENewPlayerInterface_tempPlayers[id];
    return playerEntry->iIsHelpDone;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setHelpDone);
#ifdef SKIP_ASM
/**
 * Sets or clears iIsHelpDone (flag bit 3) at +0x10 of a rider's player entry (tempPlayers[getPlayerID(@p index)]).
 *
 * @param index  the rider (On Tour iRider)
 * @param value  the new bit
 */
void cBENewPlayerInterface::setHelpDone(int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBEPlayerDB* playerEntry = &cBENewPlayerInterface_tempPlayers[id];
    playerEntry->iIsHelpDone = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getControllerMapping);
#ifdef SKIP_ASM
/**
 * Returns the button layout a rider's player has chosen.
 *
 * Signed byte +0x13 (cControlMap) of cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)].
 *
 * @param index  the rider (On Tour iRider)
 * @return the controller mapping id
 */
signed char cBENewPlayerInterface::getControllerMapping(int index)
{
    return cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)].cControlMap;
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
void cBENewPlayerInterface::setControllerMapping(int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBENewPlayerInterface_tempPlayers[id].cControlMap = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getCamera);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Returns the word at +0x14 of a rider's player entry (cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]).
 *
 * @param index  the rider
 * @return the word
 */
int cBENewPlayerInterface::getCamera(int index)
{
    return cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)].iCamera;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setCamera);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Sets the word at +0x14 of a rider's player entry (cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]).
 *
 * @param index  the rider
 * @param value  the new word
 */
void cBENewPlayerInterface::setCamera(int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBENewPlayerInterface_tempPlayers[id].iCamera = value;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", func_00147718);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Returns the word at +0x18 of a rider's player entry (cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]).
 *
 * @param index  the rider
 * @return the word
 */
extern "C" int func_00147718(cBENewPlayerInterface* self, int index)
{
    return cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)].unk_018;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", func_00147758);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.

/**
 * Sets the word at +0x18 of a rider's player entry (cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]).
 *
 * @param index  the rider
 * @param value  the new word
 */
extern "C" void func_00147758(cBENewPlayerInterface* self, int index, int value)
{
    int id = cBENewPlayerInterface_getPlayerID(index);
    cBENewPlayerInterface_tempPlayers[id].unk_018 = value;
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
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getRiderHumanIndex);
#ifdef SKIP_ASM
/**
 * Returns iHumanID (flag bit 0) of player slot @p index's entry, or -1 when the slot has no controller.
 *
 * The slot indexes cBENewPlayerInterface_tempPlayers directly; a controller id (+0xC) of kNoController (-1) gives -1,
 * otherwise the
 * result is bit 0 of the flags at +0x10.
 *
 * @param index  the player slot
 * @return -1, 0 or 1
 */
int cBENewPlayerInterface::getRiderHumanIndex(int index)
{
    if (cBENewPlayerInterface_tempPlayers[index].iControllerID == cBEPlayerDB::kNoController) {
        return -1;
    }
    return cBENewPlayerInterface_tempPlayers[index].iHumanID;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_isMissionMan__Fi);
#ifdef SKIP_ASM
/**
 * Tells whether a rider's player is the mission man (bit 2 of the entry's flags).
 *
 * @param index  the rider; mapped to its player
 * @return 1 or 0
 */
int cBENewPlayerInterface_isMissionMan(int index)
{
    int playerID = cBENewPlayerInterface_getPlayerID(index);
    return cBENewPlayerInterface_tempPlayers[playerID].iIsMissionMan;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setToMissionMan);
#ifdef SKIP_ASM
/**
 * Turns a rider into a 'mission man': a copy of player 0's character used in missions.
 *
 * The rider's player record (cBENewPlayerInterface_tempPlayers[getPlayerID(@p index)]) takes player 0's cCharacterID and
 * gets iIsMissionMan (flag bit 2) at +0x10.
 *
 * @param index  the rider (On Tour iRider)
 */
extern "C" void cBENewPlayerInterface_setToMissionMan(int index)
{
    cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)].cCharacterID = cBENewPlayerInterface_tempPlayers[0].cCharacterID;
    cBEPlayerDB* playerRec = &cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)];
    playerRec->iIsMissionMan = 1;
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
void cBENewPlayerInterface::resetFromMissionMan(int index)
{
    cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)].cCharacterID = 0;
    cBEPlayerDB* entry = &cBENewPlayerInterface_tempPlayers[cBENewPlayerInterface_getPlayerID(index)];
    entry->iIsMissionMan = 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147908);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns the message list of a rider's character: +0xE38 of its character record in cBELibrary_profileLib.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].cCharacterID.
 *
 * @param index  the rider
 * @return a pointer into the character record
 */
extern "C" void* func_00147908(cBENewPlayerInterface* self, int index)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    cBECharProfileDB* charRecord = &cBELibrary_profileLib[profile][charId];
    return (char*)&charRecord->unk_E38[0][0];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147980);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Tests message flag @p bit of a rider's character: bit @p bit of the bit array unk_F08 of its character record.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].cCharacterID;
 * the word is @p bit / 32 and the mask 1 << @p bit (shift taken mod 32). No range check.
 *
 * @param index  the rider
 * @param bit  the flag number
 * @return true when set
 */
extern "C" bool func_00147980(cBENewPlayerInterface* self, int index, int bit)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    int mask = 1 << bit;
    // The profile's row first, then the character: retail adds the profile offset before the character's.
    return ((*(cBELibrary_profileLib + profile))[charId].unk_F08[bit / 32] & mask) != 0;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147A30);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Sets or clears message flag @p bit of a rider's character, in the bit array at +0xF08 of its character record.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].cCharacterID.
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
    int charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
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
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns uiNumRaceMedals of pair @p slot of mPeakMainEventMedals in a rider's character record.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].cCharacterID.
 *
 * @param index  the rider
 * @param slot  the pair index (no range check)
 * @return mPeakMainEventMedals[@p slot].uiNumRaceMedals
 */
extern "C" int func_00147B20(cBENewPlayerInterface* self, int index, int slot)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    // The profile's row first, then the character: retail adds the profile offset before the character's.
    return (*(cBELibrary_profileLib + profile))[charId].mPeakMainEventMedals[slot].uiNumRaceMedals;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", func_00147BB0);
#ifdef SKIP_ASM
int cBELibrary_getProfileIndex(int);
int cBENewPlayerInterface_getPlayerID(int index);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns uiNumFreeStyleMedals of pair @p slot of mPeakMainEventMedals in a rider's character record.
 *
 * The record is profile cBELibrary_getProfileIndex(@p index), character tempPlayers[getPlayerID(@p index)].cCharacterID.
 *
 * @param index  the rider
 * @param slot  the pair index (no range check)
 * @return mPeakMainEventMedals[@p slot].uiNumFreeStyleMedals
 */
extern "C" int func_00147BB0(cBENewPlayerInterface* self, int index, int slot)
{
    int profile = cBELibrary_getProfileIndex(index);
    int id = cBENewPlayerInterface_getPlayerID(index);
    int charId = cBENewPlayerInterface_tempPlayers[id].cCharacterID;
    // The profile's row first, then the character: retail adds the profile offset before the character's.
    return (*(cBELibrary_profileLib + profile))[charId].mPeakMainEventMedals[slot].uiNumFreeStyleMedals;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_setDifficultyLevel);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Sets a difficulty level of the first profile's current character and resets its win/loss streak.
 *
 * The character is cBELibrary_getCharacterID(0) in profile 0 of cBELibrary_profileLib. With @p flag set, stores
 * @p value in mDifficulty and clears mDifference; otherwise the pair unk_284 / unk_286.
 *
 * @param value  the level, 0-2 (func_00147D20 caps it at 2)
 * @param flag  picks the pair: nonzero mDifficulty, 0 unk_284
 */
void cBENewPlayerInterface::setDifficultyLevel(int value, int flag)
{
    int charId = cBELibrary_getCharacterID(0);
    if (flag != 0) {
        cBELibrary_profileLib[0][charId].mDifficulty = value;
        cBELibrary_profileLib[0][charId].mDifference = 0;
    } else {
        cBELibrary_profileLib[0][charId].unk_284 = value;
        cBELibrary_profileLib[0][charId].unk_286 = 0;
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
 * mDifficulty of the character record when @p which is set, else unk_284.
 *
 * @param which  picks the pair: nonzero mDifficulty, 0 unk_284 (cComputer passes game mode == 0)
 * @return the level, 0-2
 */
short cBENewPlayerInterface::getDifficultyLevel(int which)
{
    int charId = cBELibrary_getCharacterID(0);
    if (which != 0)
    {
        return cBELibrary_profileLib[0][charId].mDifficulty;
    }
    else
    {
        return cBELibrary_profileLib[0][charId].unk_284;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147D20);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern cBECharProfileDB cBELibrary_profileLib[][10];


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
        level = cBELibrary_profileLib[profile][charId].mDifficulty;
        difference = cBELibrary_profileLib[profile][charId].mDifference;
    }
    else
    {
        level = cBELibrary_profileLib[profile][charId].unk_284;
        difference = cBELibrary_profileLib[profile][charId].unk_286;
    }
    difference++;
    if (difference >= 2)
    {
        if (level < 2) level++;
        difference = 0;
    }
    if (which)
    {
        cBELibrary_profileLib[profile][charId].mDifficulty = level;
        cBELibrary_profileLib[profile][charId].mDifference = difference;
    }
    else
    {
        cBELibrary_profileLib[profile][charId].unk_284 = level;
        cBELibrary_profileLib[profile][charId].unk_286 = difference;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_00147E18);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern cBECharProfileDB cBELibrary_profileLib[][10];


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
        level = cBELibrary_profileLib[profile][charId].mDifficulty;
        difference = cBELibrary_profileLib[profile][charId].mDifference;
    }
    else
    {
        level = cBELibrary_profileLib[profile][charId].unk_284;
        difference = cBELibrary_profileLib[profile][charId].unk_286;
    }
    difference--;
    if (difference < -1)
    {
        if (level != 0) level--;
        difference = 0;
    }
    if (which)
    {
        cBELibrary_profileLib[profile][charId].mDifficulty = level;
        cBELibrary_profileLib[profile][charId].mDifference = difference;
    }
    else
    {
        cBELibrary_profileLib[profile][charId].unk_284 = level;
        cBELibrary_profileLib[profile][charId].unk_286 = difference;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBENewPlayerInterface_getDifficultyDifference);
#ifdef SKIP_ASM
int cBELibrary_getCharacterID(int);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns the win/loss streak beside a difficulty level of the first profile's current character.
 *
 * mDifference (when @p flag is set) or unk_286 of the record of cBELibrary_getCharacterID(0) in profile 0; the win
 * and loss counters func_00147D20 and func_00147E18 move it.
 *
 * @param flag  picks the pair: nonzero mDifference, 0 unk_286
 * @return the streak, -1 to 1
 */
short cBENewPlayerInterface::getDifficultyDifference(int flag)
{
    int charId = cBELibrary_getCharacterID(0);
    short result;
    if (flag == 0) {
        result = cBELibrary_profileLib[0][charId].unk_286;
    } else {
        result = cBELibrary_profileLib[0][charId].mDifference;
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getThis);
#ifdef SKIP_ASM
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BECharInt[];
extern sVEntry<void(void*)> cBEStatInterface_vtbl[];

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
        sVEntry<void(void*)>* vt = cBEStatInterface_vtbl;
        short thisAdjust = vt[2].delta; // slot 2: discard, read before the vptr store
        void (*fn)(void*) = vt[2].fn;
        (*(void**)&VPTR_OF(cBEInterface, mem)) = vt;
        mem->bIsActive = 0;
        fn(vthis(mem, thisAdjust));
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
extern sPlayerStatRow cBEStatInterface_currentStat[];
extern sPlayerSlot_00147FD8 D_004A7887[];
extern sPlayerSlot_00147FD8 D_004B13D7[];
extern sPlayerSlot_00147FD8 D_004BAF27[];

/**
 * Commits the working character stats cBEStatInterface_currentStat to the profiles.
 *
 * Copies the 7 stat bytes of each of 10 characters in 3 profiles into +0xBDF of the character records.
 */
extern "C" void cBEStatInterface_submit(void)
{
    for (int i = 0; i < 10; i++)
    {
        D_004A7887[i].ctrl = ((sCtrl7_00147FD8 (*)[10])cBEStatInterface_currentStat)[0][i];
        D_004B13D7[i].ctrl = ((sCtrl7_00147FD8 (*)[10])cBEStatInterface_currentStat)[1][i];
        D_004BAF27[i].ctrl = ((sCtrl7_00147FD8 (*)[10])cBEStatInterface_currentStat)[2][i];
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_discard);
#ifdef SKIP_ASM
struct sPlayerStatRow;
extern sPlayerStatRow cBEStatInterface_currentStat[];
extern sPlayerSlot_00147FD8 D_004A7887[];
extern sPlayerSlot_00147FD8 D_004B13D7[];
extern sPlayerSlot_00147FD8 D_004BAF27[];

/**
 * Reloads the working character stats cBEStatInterface_currentStat from the profiles (the reverse of submit).
 */
extern "C" void cBEStatInterface_discard(void)
{
    for (int i = 0; i < 10; i++)
    {
        ((sCtrl7_00147FD8 (*)[10])cBEStatInterface_currentStat)[0][i] = D_004A7887[i].ctrl;
        ((sCtrl7_00147FD8 (*)[10])cBEStatInterface_currentStat)[1][i] = D_004B13D7[i].ctrl;
        ((sCtrl7_00147FD8 (*)[10])cBEStatInterface_currentStat)[2][i] = D_004BAF27[i].ctrl;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getStatFromProfile);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int cBELibrary_getCharacterID(int);
extern cBECharProfileDB cBELibrary_profileLib[][10];

/**
 * Returns one of the seven attribute bytes of a profile's current character, or -1 for an unknown attribute.
 *
 * Reads byte @p stat of statDB in the record of character cBELibrary_getCharacterID(@p profile) in profile
 * @p profile of cBELibrary_profileLib.
 *
 * @param profile  the human, used both as the profile index and to look up the character (On Tour iHuman)
 * @param stat  the attribute (eBEAttributes), 0-6
 * @return the attribute value, or -1
 */
// Returns one of a character's seven stat levels for the given profile, or -1.
int cBEStatInterface::getStatFromProfile(int profile, unsigned int stat)
{
    int charId = cBELibrary_getCharacterID(profile);
    switch (stat) {
    case kATTRIB_SPEED: {
        return cBELibrary_profileLib[profile][charId].statDB.cSpeed;
    }
    case kATTRIB_ACCEL: {
        return cBELibrary_profileLib[profile][charId].statDB.cAccel;
    }
    case kATTRIB_TRICK: {
        return cBELibrary_profileLib[profile][charId].statDB.cTrick;
    }
    case kATTRIB_EDGING: {
        return cBELibrary_profileLib[profile][charId].statDB.cEdging;
    }
    case kATTRIB_SPIN: {
        return cBELibrary_profileLib[profile][charId].statDB.cSpin;
    }
    case kATTRIB_TOUGH: {
        return cBELibrary_profileLib[profile][charId].statDB.cToughness;
    }
    case kATTRIB_STAB: {
        return cBELibrary_profileLib[profile][charId].statDB.cStability;
    }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", func_001482D0);
#ifdef SKIP_ASM
extern tBEStatDefault cBELibrary_statDefault_001482D0[] __asm__("cBELibrary_statDefault");

/**
 * Copies record @p character of cBELibrary_statDefault, the 15-byte per-character default-stat table, into @p dst.
 *
 * The struct is returned by value: @p dst is the caller's buffer (g++ passes it first) and @p iface the stat
 * interface. The record has the size of On Tour's tBEStatDefault (character id, start stats, maximum stats).
 *
 * @param dst  the result buffer
 * @param iface  the stat interface (not used)
 * @param character  the character
 * @return @p dst
 */
extern "C" tBEStatDefault* func_001482D0(tBEStatDefault* dst, int iface, int character)
{
    *dst = cBELibrary_statDefault_001482D0[character];
    return dst;
}
#endif
//100%
INCLUDE_ASM("be/beintplayer", func_00148328);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getCharacterID(int);

extern tBEStatDefault cBELibrary_statDefault_00148328[] __asm__("cBELibrary_statDefault");

/**
 * Copies a rider's record of cBELibrary_statDefault, the 15-byte per-character default-stat table, into @p dst.
 *
 * The record is the one of character cBELibrary_getCharacterID(@p rider). The struct is returned by value: @p dst is
 * the caller's buffer (g++ passes it first) and @p iface the stat interface.
 *
 * @param dst  the result buffer
 * @param iface  the stat interface (not used)
 * @param rider  the rider
 * @return @p dst
 */
extern "C" tBEStatDefault* func_00148328(tBEStatDefault* dst, int iface, int rider)
{
    int charId = cBELibrary_getCharacterID(rider);
    *dst = cBELibrary_statDefault_00148328[charId];
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
extern sPlayerStatRowView_001483A0 cBEStatInterface_currentStat_view001483A0[] __asm__("cBEStatInterface_currentStat");

/**
 * Sets a rider's speed stat: byte 0 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 0 is On Tour's tBEStatDB::cSpeed. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
void cBEStatInterface::setSpeed(int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    cBEStatInterface_currentStat_view001483A0[profile].e[charId].v[0] = value;
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
extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Returns a rider's speed stat: byte 0 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 0 is On Tour's tBEStatDB::cSpeed.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
signed char cBEStatInterface::getSpeed(int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return cBEStatInterface_currentStat_view[profile].e[charId].v[0];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setAccel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Sets a rider's acceleration stat: byte 1 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 1 is On Tour's tBEStatDB::cAccel. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
void cBEStatInterface::setAccel(int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    cBEStatInterface_currentStat_view[profile].e[charId].v[1] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getAccel);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Returns a rider's acceleration stat: byte 1 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 1 is On Tour's tBEStatDB::cAccel.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
signed char cBEStatInterface::getAccel(int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return cBEStatInterface_currentStat_view[profile].e[charId].v[1];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setTrick);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Sets a rider's trick stat: byte 2 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 2 is On Tour's tBEStatDB::cTrick. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
void cBEStatInterface::setTrick(int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    cBEStatInterface_currentStat_view[profile].e[charId].v[2] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getTrick);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Returns a rider's trick stat: byte 2 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 2 is On Tour's tBEStatDB::cTrick.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
signed char cBEStatInterface::getTrick(int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return cBEStatInterface_currentStat_view[profile].e[charId].v[2];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setEdging);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Sets a rider's edging stat: byte 3 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 3 is On Tour's tBEStatDB::cEdging. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
void cBEStatInterface::setEdging(int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    cBEStatInterface_currentStat_view[profile].e[charId].v[3] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getEdging);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Returns a rider's edging stat: byte 3 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 3 is On Tour's tBEStatDB::cEdging.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
signed char cBEStatInterface::getEdging(int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return cBEStatInterface_currentStat_view[profile].e[charId].v[3];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setSpin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Sets a rider's spin stat: byte 4 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 4 is On Tour's tBEStatDB::cSpin. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
void cBEStatInterface::setSpin(int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    cBEStatInterface_currentStat_view[profile].e[charId].v[4] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getSpin);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Returns a rider's spin stat: byte 4 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 4 is On Tour's tBEStatDB::cSpin.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
signed char cBEStatInterface::getSpin(int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return cBEStatInterface_currentStat_view[profile].e[charId].v[4];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setToughness);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Sets a rider's toughness stat: byte 5 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 5 is On Tour's tBEStatDB::cToughness. cBEStatInterface_setStatLevel stores level x 5 here.
 *
 * @param rider  the rider (On Tour iRider)
 * @param value  the stat value
 */
void cBEStatInterface::setToughness(int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    cBEStatInterface_currentStat_view[profile].e[charId].v[5] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getToughness);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Returns a rider's toughness stat: byte 5 of its 7-byte record in cBEStatInterface_currentStat.
 *
 * The record is [cBELibrary_getProfileIndex(@p rider)][cBELibrary_getCharacterID(@p rider)] (10 characters per
 * profile); byte 5 is On Tour's tBEStatDB::cToughness.
 *
 * @param rider  the rider (On Tour iRider)
 * @return the stat value
 */
signed char cBEStatInterface::getToughness(int rider)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charId = cBELibrary_getCharacterID(rider);
    return cBEStatInterface_currentStat_view[profile].e[charId].v[5];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setStability);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Sets a rider's stability stat (the seventh entry of its character's stat record) in the current-stat table.
 *
 * Looks up the rider's profile with cBELibrary_getProfileIndex() and character with cBELibrary_getCharacterID(), then
 * stores @p value as a byte in cBEStatInterface_currentStat[profile][character][6] (On Tour tBEStatDB.cStability).
 * cBEStatInterface_setStatLevel() calls it with level * 5.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  new stat value (On Tour var)
 */
void cBEStatInterface::setStability(int rider, int value)
{
    int profileIndex = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    cBEStatInterface_currentStat_view[profileIndex].e[charID].v[6] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getStability);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBELibrary_getProfileIndex(int);
int cBELibrary_getCharacterID(int);

extern sPlayerStatRowView_00148410 cBEStatInterface_currentStat_view[] __asm__("cBEStatInterface_currentStat");

/**
 * Returns a rider's stability stat (the seventh entry of its character's stat record) from the current-stat table.
 *
 * Reads cBEStatInterface_currentStat[profile][character][6] (On Tour tBEStatDB.cStability), with the profile from
 * cBELibrary_getProfileIndex() and the character from cBELibrary_getCharacterID().
 *
 * @param rider  race rider slot (On Tour iRider)
 * @return the stability stat (5 per level)
 */
signed char cBEStatInterface::getStability(int rider)
{
    int profileIndex = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    return cBEStatInterface_currentStat_view[profileIndex].e[charID].v[6];
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
int cBEStatInterface::getStatLevel(int rider)
{
    int statValue;
    statValue = getAccel(rider);
    int sum = statValue / 5;
    statValue = getEdging(rider);
    sum += statValue / 5;
    statValue = getSpeed(rider);
    sum += statValue / 5;
    statValue = getSpin(rider);
    sum += statValue / 5;
    statValue = getToughness(rider);
    sum += statValue / 5;
    statValue = getStability(rider);
    sum += statValue / 5;
    statValue = getTrick(rider);
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
void cBEStatInterface::setStatLevel(int rider, int value)
{
    int statPoints = value * 5;
    setAccel(rider, statPoints);
    setEdging(rider, statPoints);
    setSpeed(rider, statPoints);
    setSpin(rider, statPoints);
    setToughness(rider, statPoints);
    setStability(rider, statPoints);
    setTrick(rider, statPoints);
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
extern sPlayerStatRowView_00148B50 cBEStatInterface_currentStat_view00148B50[] __asm__("cBEStatInterface_currentStat");

/**
 * Stores a profile's speed stat for one character (byte 0 of the character's stat record) in the current-stat table.
 *
 * Writes @p value to cBEStatInterface_currentStat[@p humanIndex][@p charType][0] (On Tour tBEStatDB.cSpeed). Unlike the rider-slot setters, it takes
 * the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
void cBEStatInterface::setHumanSpeed(int humanIndex, int value, int charType)
{
    cBEStatInterface_currentStat_view00148B50[humanIndex].e[charType].v[0] = value;
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
extern sPlayerStatRow cBEStatInterface_currentStat[];

/**
 * Returns a profile's speed stat for one character (byte 0 of the character's stat record) from the current-stat table.
 *
 * Reads cBEStatInterface_currentStat[@p humanIndex][@p charType][0] (On Tour tBEStatDB.cSpeed).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the speed stat (5 per level)
 */
signed char cBEStatInterface::getHumanSpeed(int humanIndex, int charType)
{
    return cBEStatInterface_currentStat[humanIndex].e[charType].v[0];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanAccel);
#ifdef SKIP_ASM
/**
 * Stores a profile's acceleration stat for one character (byte 1 of the character's stat record) in the current-stat
 * table.
 *
 * Writes @p value to cBEStatInterface_currentStat[@p humanIndex][@p charType][1] (On Tour tBEStatDB.cAccel). Unlike the rider-slot setters, it takes
 * the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
void cBEStatInterface::setHumanAccel(int humanIndex, int value, int charType)
{
    cBEStatInterface_currentStat[humanIndex].e[charType].v[1] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanAccel);
#ifdef SKIP_ASM
/**
 * Returns a profile's acceleration stat for one character (byte 1 of the character's stat record) from the current-stat
 * table.
 *
 * Reads cBEStatInterface_currentStat[@p humanIndex][@p charType][1] (On Tour tBEStatDB.cAccel).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the acceleration stat (5 per level)
 */
signed char cBEStatInterface::getHumanAccel(int humanIndex, int charType)
{
    return cBEStatInterface_currentStat[humanIndex].e[charType].v[1];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanTrick);
#ifdef SKIP_ASM
/**
 * Stores a profile's trick stat for one character (byte 2 of the character's stat record) in the current-stat table.
 *
 * Writes @p value to cBEStatInterface_currentStat[@p humanIndex][@p charType][2] (On Tour tBEStatDB.cTrick). Unlike the rider-slot setters, it takes
 * the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
void cBEStatInterface::setHumanTrick(int humanIndex, int value, int charType)
{
    cBEStatInterface_currentStat[humanIndex].e[charType].v[2] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanTrick);
#ifdef SKIP_ASM
/**
 * Returns a profile's trick stat for one character (byte 2 of the character's stat record) from the current-stat table.
 *
 * Reads cBEStatInterface_currentStat[@p humanIndex][@p charType][2] (On Tour tBEStatDB.cTrick).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the trick stat (5 per level)
 */
signed char cBEStatInterface::getHumanTrick(int humanIndex, int charType)
{
    return cBEStatInterface_currentStat[humanIndex].e[charType].v[2];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanEdging);
#ifdef SKIP_ASM
/**
 * Stores a profile's edging stat for one character (byte 3 of the character's stat record) in the current-stat table.
 *
 * Writes @p value to cBEStatInterface_currentStat[@p humanIndex][@p charType][3] (On Tour tBEStatDB.cEdging). Unlike the rider-slot setters, it takes
 * the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
void cBEStatInterface::setHumanEdging(int humanIndex, int value, int charType)
{
    cBEStatInterface_currentStat[humanIndex].e[charType].v[3] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanEdging);
#ifdef SKIP_ASM
/**
 * Returns a profile's edging stat for one character (byte 3 of the character's stat record) from the current-stat
 * table.
 *
 * Reads cBEStatInterface_currentStat[@p humanIndex][@p charType][3] (On Tour tBEStatDB.cEdging).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the edging stat (5 per level)
 */
signed char cBEStatInterface::getHumanEdging(int humanIndex, int charType)
{
    return cBEStatInterface_currentStat[humanIndex].e[charType].v[3];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanSpin);
#ifdef SKIP_ASM
/**
 * Stores a profile's spin stat for one character (byte 4 of the character's stat record) in the current-stat table.
 *
 * Writes @p value to cBEStatInterface_currentStat[@p humanIndex][@p charType][4] (On Tour tBEStatDB.cSpin). Unlike the rider-slot setters, it takes the
 * profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
void cBEStatInterface::setHumanSpin(int humanIndex, int value, int charType)
{
    cBEStatInterface_currentStat[humanIndex].e[charType].v[4] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanSpin);
#ifdef SKIP_ASM
/**
 * Returns a profile's spin stat for one character (byte 4 of the character's stat record) from the current-stat table.
 *
 * Reads cBEStatInterface_currentStat[@p humanIndex][@p charType][4] (On Tour tBEStatDB.cSpin).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the spin stat (5 per level)
 */
signed char cBEStatInterface::getHumanSpin(int humanIndex, int charType)
{
    return cBEStatInterface_currentStat[humanIndex].e[charType].v[4];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanToughness);
#ifdef SKIP_ASM
/**
 * Stores a profile's toughness stat for one character (byte 5 of the character's stat record) in the current-stat
 * table.
 *
 * Writes @p value to cBEStatInterface_currentStat[@p humanIndex][@p charType][5] (On Tour tBEStatDB.cToughness). Unlike the rider-slot setters, it
 * takes the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
void cBEStatInterface::setHumanToughness(int humanIndex, int value, int charType)
{
    cBEStatInterface_currentStat[humanIndex].e[charType].v[5] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanToughness);
#ifdef SKIP_ASM
/**
 * Returns a profile's toughness stat for one character (byte 5 of the character's stat record) from the current-stat
 * table.
 *
 * Reads cBEStatInterface_currentStat[@p humanIndex][@p charType][5] (On Tour tBEStatDB.cToughness).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the toughness stat (5 per level)
 */
signed char cBEStatInterface::getHumanToughness(int humanIndex, int charType)
{
    return cBEStatInterface_currentStat[humanIndex].e[charType].v[5];
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_setHumanStability);
#ifdef SKIP_ASM
/**
 * Stores a profile's stability stat for one character (byte 6 of the character's stat record) in the current-stat
 * table.
 *
 * Writes @p value to cBEStatInterface_currentStat[@p humanIndex][@p charType][6] (On Tour tBEStatDB.cStability). Unlike the rider-slot setters, it
 * takes the profile and the character directly.
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param value  new stat value (On Tour var)
 * @param charType  character ID (On Tour charType)
 */
void cBEStatInterface::setHumanStability(int humanIndex, int value, int charType)
{
    cBEStatInterface_currentStat[humanIndex].e[charType].v[6] = value;
}
#endif

//100%
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getHumanStability);
#ifdef SKIP_ASM
/**
 * Returns a profile's stability stat for one character (byte 6 of the character's stat record) from the current-stat
 * table.
 *
 * Reads cBEStatInterface_currentStat[@p humanIndex][@p charType][6] (On Tour tBEStatDB.cStability).
 *
 * @param humanIndex  profile index (On Tour iHuman)
 * @param charType  character ID (On Tour charType)
 * @return the stability stat (5 per level)
 */
signed char cBEStatInterface::getHumanStability(int humanIndex, int charType)
{
    return cBEStatInterface_currentStat[humanIndex].e[charType].v[6];
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getBrakingAttrib);
#ifdef SKIP_ASM
int cBENewPlayerInterface_isMissionMan(int index);

extern tBEStatDefault cBELibrary_statDefault[];

/**
 * Returns a rider's braking attribute as a fraction of the character's maximum edging level.
 *
 * Mission riders (cBENewPlayerInterface_isMissionMan()) get 0.5. The level is @p value when positive, else the
 * profile's edging stat for the character (cBEStatInterface_currentStat[profile][character][3], On Tour cEdging) divided by 5; the result
 * is that level over the character's cMaxEdging (byte 0xB of its cBELibrary_statDefault default-stat record). cBEStatInterface_getBrakingAttrib and
 * cBEStatInterface_getEdgingAttrib have identical bodies.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  attribute level to use instead of the stored one, when positive (On Tour overrideAttribLevel)
 * @return the attribute level divided by the character's edging maximum
 */
float cBEStatInterface::getBrakingAttrib(int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int attribLevel;
    if (value > 0)
        attribLevel = value;
    else
        attribLevel = cBEStatInterface_currentStat[profile].e[charID].v[3] / 5;
    return (float)attribLevel / (float)cBELibrary_statDefault[charID].cMaxEdging;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("be/beintplayer", cBEStatInterface_getEdgingAttrib);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
int cBENewPlayerInterface_isMissionMan(int index);

extern tBEStatDefault cBELibrary_statDefault[];

/**
 * Returns a rider's edging attribute as a fraction of the character's maximum edging level.
 *
 * Mission riders (cBENewPlayerInterface_isMissionMan()) get 0.5. The level is @p value when positive, else the
 * profile's edging stat for the character (cBEStatInterface_currentStat[profile][character][3], On Tour cEdging) divided by 5; the result
 * is that level over the character's cMaxEdging (byte 0xB of its cBELibrary_statDefault default-stat record). cBEStatInterface_getBrakingAttrib and
 * cBEStatInterface_getEdgingAttrib have identical bodies.
 *
 * @param rider  race rider slot (On Tour iRider)
 * @param value  attribute level to use instead of the stored one, when positive (On Tour overrideAttribLevel)
 * @return the attribute level divided by the character's edging maximum
 */
float cBEStatInterface::getEdgingAttrib(int rider, int value)
{
    int profile = cBELibrary_getProfileIndex(rider);
    int charID = cBELibrary_getCharacterID(rider);
    if (cBENewPlayerInterface_isMissionMan(rider))
        return 0.5f;
    int attribLevel;
    if (value > 0)
        attribLevel = value;
    else
        attribLevel = cBEStatInterface_currentStat[profile].e[charID].v[3] / 5;
    return (float)attribLevel / (float)cBELibrary_statDefault[charID].cMaxEdging;
}
#endif

