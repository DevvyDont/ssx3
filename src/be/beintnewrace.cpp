#include "common.h"
#include "vtable.h"
#include "be/beintnewrace.h"
#include "be/bedbprofile.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BENewRaceInt[];
extern void* cBENewRaceInterface_vtbl[16];



// padded past the 8-byte gp-relative threshold so the compiler emits
// absolute lui/lo addressing like the target, instead of assuming small data
struct sPad16 { char x; int pad[3]; };
extern sPad16 D_0043D984;

extern sPad16 D_0043D954;

extern tBELocationDB cBELibrary_locationDB[];

extern sPad16 D_0043E254;

struct sRaceEntry18 {
    char pad_0x00[0x14];
    int field_0x14;
};
extern sRaceEntry18 cBELibrary_sectionDB[];

extern tBEModeDB cBELibrary_gameModeDB[];

//99.58%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the new-race interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BENewRaceInt", clears the word at 0x8, stores the vtable
 * cBENewRaceInterface_vtbl and keeps the object in cBENewRaceInterface_instance.
 *
 * @return the new-race interface
 */
void* cBENewRaceInterface_getThis()
{
    if (cBENewRaceInterface_instance == 0) {
        cBENewRaceInterface* mem = (cBENewRaceInterface*)cMemMan_alloc(0x10, str_BENewRaceInt, 0, 0);
        mem->bIsActive = 0;
        (*(void**)&VPTR_OF(cBEInterface, mem)) = cBENewRaceInterface_vtbl;
        cBENewRaceInterface_instance = (cBENewRaceInterface*)(mem);
    }
    return cBENewRaceInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_submit);
#ifdef SKIP_ASM

extern cBERaceDB cBELibrary_raceDB;

/**
 * Commits the race being set up: copies cBENewRaceInterface_newRace (0x4C bytes) into cBELibrary_raceDB.
 */
extern "C" void cBENewRaceInterface_submit(void)
{
    cBELibrary_raceDB = *(cBERaceDB*)&cBENewRaceInterface_newRace.playerID[0];
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_discard);
#ifdef SKIP_ASM
extern cBERaceDB cBELibrary_raceDB;

/**
 * Drops changes to the race being set up: copies cBELibrary_raceDB back into cBENewRaceInterface_newRace (0x4C bytes).
 */
extern "C" void cBENewRaceInterface_discard(void)
{
    *(cBERaceDB*)&cBENewRaceInterface_newRace.playerID[0] = cBELibrary_raceDB;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_setNumberHumans__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the number of human riders for the new race and resets the player/rider slot mapping.
 *
 * Writes the static race settings cBENewRaceInterface_newRace (@p self is unused): iNumberHumans = @p humans,
 * iNumberRiders = humans + iNumberAI + iNumberMissionRiders, and playerID[i] = riderID[i] = i for all six slots.
 *
 * @param humans  number of human riders
 */
void cBENewRaceInterface_setNumberHumans(void* self, int humans)
{
    cBENewRaceInterface_newRace.iNumberHumans = humans;
    cBENewRaceInterface_newRace.iNumberRiders = humans + cBENewRaceInterface_newRace.iNumberAI + cBENewRaceInterface_newRace.iNumberMissionRiders;
    for (int i = 0; i < 6; i++) {
        cBENewRaceInterface_newRace.playerID[i] = i;
        cBENewRaceInterface_newRace.riderID[i] = i;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_setNumberMission__FPvi);
#ifdef SKIP_ASM
/**
 * Sets the number of mission riders in the new race and updates the total rider count.
 *
 * Writes the static race settings cBENewRaceInterface_newRace (@p self is unused): iNumberMissionRiders =
 * @p mission and iNumberRiders = iNumberHumans + iNumberAI + @p mission.
 *
 * @param mission  number of mission riders
 */
void cBENewRaceInterface_setNumberMission(void* self, int mission)
{
    int numHumans = cBENewRaceInterface_newRace.iNumberHumans;
    int numAI = cBENewRaceInterface_newRace.iNumberAI;
    cBENewRaceInterface_newRace.iNumberMissionRiders = mission;
    cBENewRaceInterface_newRace.iNumberRiders = numHumans + numAI + mission;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_setDebugLocationFile);
#ifdef SKIP_ASM
extern "C" char* strcpy(char*, const char*);
extern char str_dbg[];

/**
 * Debug menu hook that points the current location at a different track file.
 *
 * For the location index in cBENewRaceInterface_newRace +0x40, strcpy's @p name into the three strings of its 0x64-byte
 * cBELibrary location record (D_0043D984, +0x10 and -0x30 from it) and into str_dbg.
 *
 * @param name  the track file name (On Tour fileName)
 */
void cBENewRaceInterface::setDebugLocationFile(const char* name)
{
    char* base = (char*)&D_0043D984;
    cBERaceDB* newRace = &(*(&cBENewRaceInterface_newRace));
    strcpy(base + newRace->iLocationIdx * 100, name);
    char* bigFileNames = base + 0x10;
    strcpy(bigFileNames + newRace->iLocationIdx * 100, name);
    char* locationNames = base - 0x30;
    strcpy(locationNames + newRace->iLocationIdx * 100, name);
    strcpy(str_dbg, name);
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocation);
#ifdef SKIP_ASM
/**
 * Returns the location table entry (0x64 bytes) of the race's location, newRace.iLocationIdx.
 *
 * @return the entry in cBELibrary_locationDB
 */
extern "C" void* cBENewRaceInterface_getLocation()
{
    return (char*)&cBELibrary_locationDB[cBENewRaceInterface_newRace.iLocationIdx];
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationID);
#ifdef SKIP_ASM
/**
 * Returns the id of the current race location.
 *
 * The id selects the 0x64-byte location record that cBENewRaceInterface_getLocation() returns; the profile
 * screen sets it from the player's last hub.
 *
 * @return the location id stored in newRace.iLocationIdx
 */
extern "C" int cBENewRaceInterface_getLocationID()
{
    return cBENewRaceInterface_newRace.iLocationIdx;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationName1);
#ifdef SKIP_ASM
/**
 * Returns the name of the race's location: +0x4 of its 0x64-byte cBELibrary_locationDB record (D_0043D954 +
 * newRace.iLocationIdx * 0x64).
 *
 * @return the name
 */
extern "C" void* cBENewRaceInterface_getLocationName1()
{
    return (char*)&D_0043D954 + cBENewRaceInterface_newRace.iLocationIdx * 0x64;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationShortName);
#ifdef SKIP_ASM
extern char str_Snow_Jam[];

/**
 * Returns the short name of the race's location: +0x24 of its cBELibrary_locationDB record.
 *
 * @return the short name
 */
void* cBENewRaceInterface::getLocationShortName()
{
    return str_Snow_Jam + cBENewRaceInterface_newRace.iLocationIdx * 100;
}
#endif
//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationBigFile);
#ifdef SKIP_ASM
extern char str_BAM[];

/**
 * Returns the big-file (archive) name of location @p i: +0x44 of its cBELibrary_locationDB record ("BAM" for the
 * first).
 *
 * @param i  the location
 * @return the archive name
 */
void* cBENewRaceInterface::getLocationBigFile(int i)
{
    return str_BAM + i * 100;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationFile);
#ifdef SKIP_ASM
/**
 * Returns a location's file name (+0x34 of its location table entry).
 *
 * @param iLocation  the location index
 * @return the file name
 */
void* cBENewRaceInterface::getLocationFile(int iLocation)
{
    return (char*)&D_0043D984 + iLocation * 0x64;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationName);
#ifdef SKIP_ASM
/**
 * Returns a location's name (+0x4 of its location table entry).
 *
 * @param iLocation  the location index
 * @return the name
 */
void* cBENewRaceInterface::getLocationName(int iLocation)
{
    return (char*)&D_0043D954 + iLocation * 0x64;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationPeak);
#ifdef SKIP_ASM
/**
 * Returns the peak of location @p location: the word at +0x54 of its cBELibrary_locationDB record (0-2).
 *
 * @param location  the location
 * @return the peak
 */
int cBENewRaceInterface::getLocationPeak(int location)
{
    return cBELibrary_locationDB[location].segmentID;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationPeak1);
#ifdef SKIP_ASM
/**
 * Returns the peak of the race's location: the word at +0x54 of its cBELibrary_locationDB record (0-2).
 *
 * @return the peak
 */
extern "C" int cBENewRaceInterface_getLocationPeak1()
{
    return cBELibrary_locationDB[cBENewRaceInterface_newRace.iLocationIdx].segmentID;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationType);
#ifdef SKIP_ASM
/**
 * Returns the type of a location (race course, freestyle park, and so on).
 *
 * cBELibrary_locationDB[@p iLocation].field_0x58.
 *
 * @param iLocation  the location index (On Tour iLocation)
 * @return the location type (On Tour eBELocationType)
 */
int cBENewRaceInterface::getLocationType(int iLocation)
{
    return cBELibrary_locationDB[iLocation].type;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationIDFromScript);
#ifdef SKIP_ASM
struct sRaceEntryFull_00144CE0 {
    int field_0x0;
    char pad_0x04[0x58];
    int field_0x5C;
    int field_0x60;
};

/**
 * Returns the id (+0x0) of the first of the 23 cBELibrary_locationDB records whose word at +0x5C equals @p scriptId, or 22
 * when none does.
 *
 * @param scriptId  the script id
 * @return the location id
 */
int cBENewRaceInterface::getLocationIDFromScript(int scriptId)
{
    sRaceEntryFull_00144CE0* locations = (sRaceEntryFull_00144CE0*)cBELibrary_locationDB;
    for (int i = 0; i < 23; i++) {
        if (locations[i].field_0x5C == scriptId) {
            return locations[i].field_0x0;
        }
    }
    return 22;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationDifficulty);
#ifdef SKIP_ASM
/**
 * Returns the difficulty of location @p location: the word at +0x60 of its cBELibrary_locationDB record (0-2).
 *
 * @param location  the location
 * @return the difficulty
 */
int cBENewRaceInterface::getLocationDifficulty(int location)
{
    return cBELibrary_locationDB[location].difficulty;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getSectionFile);
#ifdef SKIP_ASM
/**
 * Returns the file name of a world section.
 *
 * D_0043E254 + @p iSection * 0x18: the name in the section table, whose records start 4 bytes earlier at cBELibrary_sectionDB.
 *
 * @param iSection  the section index (On Tour iSection)
 * @return the section's file name
 */
void* cBENewRaceInterface::getSectionFile(int iSection)
{
    return (char*)&D_0043E254 + iSection * 0x18;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getSectionType);
#ifdef SKIP_ASM
/**
 * Returns the type of a world section.
 *
 * cBELibrary_sectionDB[@p iSection].field_0x14.
 *
 * @param iSection  the section index (On Tour iSection)
 * @return the section type (On Tour eBESectionType)
 */
int cBENewRaceInterface::getSectionType(int iSection)
{
    return cBELibrary_sectionDB[iSection].field_0x14;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_assignPlayer__FPvii);
#ifdef SKIP_ASM
/**
 * Puts a player at a start gate: playerID[iGate] = iPlayer and riderID[iPlayer] = iGate.
 *
 * @param iGate  the start gate, 0-5
 * @param iPlayer  the player, 0-5
 * @return the address of playerID[iGate] (no meaning; On Tour's method is void)
 */
void* cBENewRaceInterface_assignPlayer(void* self, int iGate, int iPlayer)
{
    cBENewRaceInterface_newRace.playerID[iGate] = iPlayer;
    cBENewRaceInterface_newRace.riderID[iPlayer] = iGate;
    return &cBENewRaceInterface_newRace.playerID[iGate];
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_setNumberAI);
#ifdef SKIP_ASM
// Max AI riders per [gameType][eventKind] (7 event kinds per row).
extern unsigned int uiAI[][7];

/**
 * Sets the number of AI riders in the new race and checks it against the limit for the race type.
 *
 * Stores @p count and recomputes iNumberRiders (humans + AI + mission riders) even when the count is over the
 * limit. The limit comes from the table uiAI, indexed by cGamePath and cGameMode.
 *
 * @param count  number of AI riders
 * @return true when @p count is within the limit (compared unsigned, so a negative count fails); [O] On Tour's
 *         setNumberAI returns bool
 */
bool cBENewRaceInterface::setNumberAI(int count)
{
    cBENewRaceInterface_newRace.iNumberAI = count;
    cBENewRaceInterface_newRace.iNumberRiders = cBENewRaceInterface_newRace.iNumberHumans + count + cBENewRaceInterface_newRace.iNumberMissionRiders;
    signed char eventKind = cBENewRaceInterface_newRace.cGameMode;
    signed char gameType = cBENewRaceInterface_newRace.cGamePath;
    return (unsigned int)count <= uiAI[gameType][eventKind];
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_setGameMode);
#ifdef SKIP_ASM
extern "C" bool cBENewRaceInterface_setNumberAI(cBENewRaceInterface* self, int count);
extern "C" bool cBENewRaceInterface_setGameEvent(cBENewRaceInterface* self, int event);
extern "C" void cBENewPlayerInterface_resetFromMissionMan(void* self, int a1);
extern "C" void cBENewPlayerInterface_setToMissionMan(int a0);
int cBENewPlayerInterface_isMissionMan(int a0);
void* cBENewPlayerInterface_getThis();


/**
 * Sets the race's game mode and the matching numbers of human, AI and mission riders.
 *
 * Refused when the mode is not allowed on the current game path (cBELibrary_gamePathDB). kGAMEMODE_FREERIDE gets 1
 * human and 1 mission rider and makes player 1 the mission man with player 0's character
 * (cBENewPlayerInterface_setToMissionMan), then submits; leaving it restores the humans (2 on kGAMEPATH_MULTIPLAYER)
 * and resets player 1. kGAMEMODE_RACE (5 AI, 4 multiplayer), kGAMEMODE_SLOPESTYLE (1 AI) and kGAMEMODE_HALFPIPE,
 * kGAMEMODE_TIMECHALLENGE, kGAMEMODE_PNTSCHALLENGE (1 AI for the rival events off the multiplayer path) set the
 * counts; kGAMEMODE_TIMECHALLENGE also sets kGAMEEVENT_RIVALTIME.
 *
 * @param mode  the game mode (eBEGameMode)
 * @return true when set, false when the mode is not allowed ([O] On Tour's setGameMode returns bool)
 */
bool cBENewRaceInterface::setGameMode(int mode)
{
    tBEPathDB row = cBELibrary_gamePathDB[cBENewRaceInterface_newRace.cGamePath];
    int humans = 1;
    int ai = 5;
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        humans = 2;
        ai = 4;
    }
    if (row.cModes[mode] != true) {
        return false;
    }
    if (mode == kGAMEMODE_FREERIDE) {
        cBENewRaceInterface_newRace.cGameMode = mode;
        cBENewRaceInterface_setNumberHumans(this, 1);
        setNumberAI(0);
        cBENewRaceInterface_setNumberMission(this, 1);
        cBEInterface* playerInterface = (cBEInterface*)cBENewPlayerInterface_getThis();
        cBENewPlayerInterface_setToMissionMan(1);
        playerInterface->submit();
    } else if (cBENewRaceInterface_newRace.cGameMode == kGAMEMODE_FREERIDE) {
        cBENewRaceInterface_setNumberHumans(this, humans);
        setNumberAI(0);
        cBENewRaceInterface_setNumberMission(this, 0);
        cBEInterface* playerInterface = (cBEInterface*)cBENewPlayerInterface_getThis();
        if (cBENewPlayerInterface_isMissionMan(1) != 0) {
            cBENewPlayerInterface_resetFromMissionMan(playerInterface, 1);
            playerInterface->submit();
        }
    }
    cBENewRaceInterface_newRace.cGameMode = mode;
    if (mode == kGAMEMODE_HALFPIPE || mode == kGAMEMODE_TIMECHALLENGE || mode == kGAMEMODE_PNTSCHALLENGE) {
        cBENewRaceInterface_setNumberHumans(this, humans);
        unsigned char ev = (unsigned char)cBENewRaceInterface_newRace.cGameEvent;
        if (ev >= kGAMEEVENT_RIVALTIME && ev <= kGAMEEVENT_RIVALPOINTS
            && cBENewRaceInterface_newRace.cGamePath != kGAMEPATH_MULTIPLAYER) {
            setNumberAI(1);
        } else {
            setNumberAI(0);
        }
        cBENewRaceInterface_setNumberMission(this, 0);
    }
    if (mode == kGAMEMODE_RACE) {
        cBENewRaceInterface_setNumberHumans(this, humans);
        setNumberAI(ai);
        cBENewRaceInterface_setNumberMission(this, 0);
    }
    if (mode == kGAMEMODE_SLOPESTYLE) {
        cBENewRaceInterface_setNumberHumans(this, humans);
        setNumberAI(1);
        cBENewRaceInterface_setNumberMission(this, 0);
    }
    if (mode == kGAMEMODE_TIMECHALLENGE) {
        setGameEvent(kGAMEEVENT_RIVALTIME);
    }
    return true;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_setGamePath);
#ifdef SKIP_ASM
/**
 * Chooses the game path and sets the number of human and AI riders to match.
 *
 * Path 0: 1 human, no AI. Path 1: 1 human, AI count kept but clamped to 0-5. Path 2: 2 humans, no AI. Other values are
 * only stored.
 *
 * @param mode  the game path (On Tour eBEGamePath)
 */
void cBENewRaceInterface::setGamePath(int mode)
{
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    *((char*)&beRaceDB->cGamePath) = mode;
    switch (mode)
    {
    case kGAMEPATH_WORLDCIRCUIT:
        cBENewRaceInterface_setNumberHumans(this, 1);
        setNumberAI(0);
        break;
    case kGAMEPATH_SINGLEEVENT:
        cBENewRaceInterface_setNumberHumans(this, 1);
        if (beRaceDB->iNumberAI < 0)
            setNumberAI(0);
        if (beRaceDB->iNumberAI >= 6)
            setNumberAI(5);
        break;
    case kGAMEPATH_MULTIPLAYER:
        cBENewRaceInterface_setNumberHumans(this, 2);
        setNumberAI(0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_setGameEvent);
#ifdef SKIP_ASM
extern "C" bool cBENewRaceInterface_setNumberAI(cBENewRaceInterface* self, int count);

/**
 * Sets the race's game event; the rival events (4 and 5) also set one AI rider (none on kGAMEPATH_MULTIPLAYER).
 *
 * @param event  the game event
 * @return true when the event's mode (cBELibrary_gameEventDB) matches the current game mode
 */
bool cBENewRaceInterface::setGameEvent(int event)
{
    tBEEventDB eventRec = cBELibrary_gameEventDB[event];
    cBENewRaceInterface_newRace.cGameEvent = event;
    if (event < 6)
    {
        if (event >= 4)
        {
            if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER)
                setNumberAI(0);
            else
                setNumberAI(1);
        }
    }
    return eventRec.iValidMode == cBENewRaceInterface_newRace.cGameMode;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getEventName);
#ifdef SKIP_ASM
extern sPad16 D_0043E97C;

/**
 * Returns the name of the race's game event: +0x4 of its 0x54-byte cBELibrary_gameEventDB record (D_0043E97C +
 * newRace.cGameEvent * 0x54).
 *
 * @return the event name
 */
extern "C" void* cBENewRaceInterface_getEventName()
{
    return (char*)&D_0043E97C + cBENewRaceInterface_newRace.cGameEvent * 0x54;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getEventName1);
#ifdef SKIP_ASM
/**
 * Returns the name of game event @p i: +0x4 of its cBELibrary_gameEventDB record (D_0043E97C + @p i * 0x54).
 *
 * @param i  the game event
 * @return the event name
 */
void* cBENewRaceInterface::getEventName1(int i)
{
    return (char*)&D_0043E97C + i * 0x54;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPathName);
#ifdef SKIP_ASM
extern char str_World_Circuit[]; // 0x43-byte records at an odd address

/**
 * Returns the name of the race's game path: str_World_Circuit + newRace.cGamePath * 67 (67-byte records).
 *
 * @return the path name
 */
extern "C" void* cBENewRaceInterface_getPathName()
{
    return str_World_Circuit + cBENewRaceInterface_newRace.cGamePath * 67;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getModeName);
#ifdef SKIP_ASM

/**
 * Returns the name of the race's game mode: cBELibrary_gameModeDB[newRace.cGameMode] (0x3C-byte records).
 *
 * @return the mode name
 */
extern "C" void* cBENewRaceInterface_getModeName()
{
    return cBELibrary_gameModeDB[cBENewRaceInterface_newRace.cGameMode].strModeName;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getModeName1);
#ifdef SKIP_ASM
/**
 * Returns the name of game mode @p gameMode: cBELibrary_gameModeDB[@p gameMode] (0x3C-byte records).
 *
 * @param gameMode  the game mode
 * @return the mode name
 */
void* cBENewRaceInterface::getModeName1(int gameMode)
{
    return cBELibrary_gameModeDB[gameMode].strModeName;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getSimThreshold);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" unsigned int GMrand(void);

struct sPrizeRow_1453D0
{
    short key;
    short kind;
    short prize[6];
};
// Typed view of cBELibrary_simDB (declared later in this unit with another layout).
extern sPrizeRow_1453D0 cBELibrary_simDB_1453D0[] __asm__("cBELibrary_simDB");

/**
 * Returns a simulated rider's score for round @p kind: the cBELibrary_simDB row of the race's location and that round,
 * column @p place, times 100, randomised by up to +-5% and rounded down to a multiple of 20.
 *
 * Difficulty 0 scales by 0.9 and 2 by 1.2, except for column 0. Round 0 counts as 1. -1 when no row matches.
 *
 * @param kind  the round (heat)
 * @param place  the rider column
 * @param mode  the difficulty, 0-2
 * @return the score, or -1
 */
int cBENewRaceInterface::getSimThreshold(int kind, int place, int mode)
{
    int k = 1;
    if (kind) k = kind;
    float difficultyScale = 1.0f;
    if (mode == 0 && place != 0)
        difficultyScale = 0.8999999761581421f;
    else if (mode == 2 && place != 0)
        difficultyScale = 1.2000000476837158f;
    int key = cBENewRaceInterface_newRace.iLocationIdx;
    for (int i = 0; i < 30; i++)
    {
        if (cBELibrary_simDB_1453D0[i].key == key && cBELibrary_simDB_1453D0[i].kind == k)
        {
            int base = (int)(cBELibrary_simDB_1453D0[i].prize[place] * 100 * difficultyScale);
            int jitter = GMrand() % 200;
            base += (int)(base * (jitter - 100) * 0.0005000000237487257f);
            return base - base % 20;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getTimeLimit);
#ifdef SKIP_ASM
extern "C" int cBENewRaceInterface_getPeakTimeLimit(cBENewRaceInterface* self, int a1, int a2);
extern int cBENewRaceInterface_mNextLocation;

struct sRaceLookup_00440B38
{
    short key;
    short kind;
    char pad_0x04[0xA];
    short value;
};
extern sRaceLookup_00440B38 cBELibrary_simDB[];

/**
 * Returns the time limit of round @p kind, in the units the game mode multiplies by 60.
 *
 * For game events 6-11 (newRace +0x4A) it is cBENewRaceInterface_getPeakTimeLimit(event, @p kind). Otherwise the halfword at +0x10 of the
 * cBELibrary_simDB row for the race's location and the round (round 0 counts as 1), falling back to
 * cBENewRaceInterface_mNextLocation's row; -1 when neither exists.
 *
 * @param kind  the round
 * @return the time limit, or -1
 */
int cBENewRaceInterface::getTimeLimit(int kind)
{
    cBERaceDB* newRaceDb = &(*(&cBENewRaceInterface_newRace));
    unsigned char mode = *(unsigned char*)&newRaceDb->cGameEvent;
    if (mode >= 6 && mode < 12)
    {
        return getPeakTimeLimit((signed char)mode, kind);
    }
    if (kind == 0)
    {
        kind = 1;
    }
    int key = newRaceDb->iLocationIdx;
    for (int i = 0; i < 30; i++)
    {
        if (cBELibrary_simDB[i].key == key && cBELibrary_simDB[i].kind == kind)
        {
            return cBELibrary_simDB[i].value;
        }
    }
    for (int i = 0; i < 30; i++)
    {
        if (cBELibrary_simDB[i].key == cBENewRaceInterface_mNextLocation && cBELibrary_simDB[i].kind == kind)
        {
            return cBELibrary_simDB[i].value;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPeakGoal);
#ifdef SKIP_ASM

extern tBEPeakDB cBELibrary_peakDB[];

/**
 * Returns the goal of peak event @p gameEvent at tier @p tier: the halfword at +0x4 of cBELibrary_peakDB[(@p gameEvent - 6) * 3 + @p tier].
 *
 * @param gameEvent  the game event, 6-11
 * @param tier  the tier, 0-2
 * @return the goal
 */
int cBENewRaceInterface::getPeakGoal(int gameEvent, int tier)
{
    return cBELibrary_peakDB[tier + (gameEvent - 6) * 3].goal;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPeakTimeLimit);
#ifdef SKIP_ASM
extern tBEPeakDB cBELibrary_peakDB[];

/**
 * Returns the time limit of peak event @p gameEvent at tier @p tier: the halfword at +0x6 of
 * cBELibrary_peakDB[(@p gameEvent - 6) * 3 + @p tier].
 *
 * @param gameEvent  the game event, 6-11
 * @param tier  the tier, 0-2
 * @return the time limit
 */
int cBENewRaceInterface::getPeakTimeLimit(int gameEvent, int tier)
{
    return cBELibrary_peakDB[tier + (gameEvent - 6) * 3].timeLimit;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPeakRewardAmount);
#ifdef SKIP_ASM
extern tBEPeakDB cBELibrary_peakDB[];

/**
 * Returns the reward of peak event @p gameEvent at tier @p tier: the halfword at +0x12 of cBELibrary_peakDB[(@p gameEvent - 6) * 3 + @p tier],
 * times 100.
 *
 * @param gameEvent  the game event, 6-11
 * @param tier  the tier, 0-2
 * @return the money
 */
int cBENewRaceInterface::getPeakRewardAmount(int gameEvent, int tier)
{
    return cBELibrary_peakDB[tier + (gameEvent - 6) * 3].prize * 100;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPeakSplitTimePoints);
#ifdef SKIP_ASM
extern tBEPeakDB cBELibrary_peakDB[];

/**
 * Returns the bonus of checkpoint @p checkpoint in peak event @p gameEvent at tier @p tier: halfword @p checkpoint of the five at +0x8 of
 * cBELibrary_peakDB[(@p gameEvent - 6) * 3 + @p tier].
 *
 * @param gameEvent  the game event, 6-11
 * @param tier  the tier, 0-2
 * @param checkpoint  the checkpoint, 0-4
 * @return the bonus
 */
int cBENewRaceInterface::getPeakSplitTimePoints(int gameEvent, int tier, int checkpoint)
{
    return cBELibrary_peakDB[tier + (gameEvent - 6) * 3].splitGoals[checkpoint];
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPlatinumGoal);
#ifdef SKIP_ASM
struct sRacePair_001456A0
{
    short a;
    short b;
};

extern sRacePair_001456A0 cBELibrary_platinumDB[];

/**
 * Returns the target for location @p idx and event @p mode from cBELibrary_platinumDB, or kNoPlatinumGoal (-1).
 *
 * Events 6-11 (the peaks' time and points challenges) give -1. Below location 14 it is entry @p idx's second halfword
 * (times 100 unless @p mode is 0); from 14 on, kGAMEEVENT_RIVALTIME gives entry @p idx's value,
 * kGAMEEVENT_RIVALPOINTS entry @p idx + 3's times 100, others -1.
 *
 * @param idx  the location
 * @param mode  the event (eBEGameEvent)
 * @return the target, or kNoPlatinumGoal
 */
int cBENewRaceInterface::getPlatinumGoal(int idx, int mode)
{
    if (mode == kGAMEEVENT_POINTS_PEAK1 || mode == kGAMEEVENT_POINTS_PEAK2 || mode == kGAMEEVENT_POINTS_PEAK3
        || mode == kGAMEEVENT_TIME_PEAK1 || mode == kGAMEEVENT_TIME_PEAK2 || mode == kGAMEEVENT_TIME_PEAK3)
        return kNoPlatinumGoal;
    if (idx < 14)
    {
        if (mode == 0)
            return cBELibrary_platinumDB[(short)idx].b;
        return cBELibrary_platinumDB[(short)idx].b * 100;
    }
    if (mode == kGAMEEVENT_RIVALTIME)
        return cBELibrary_platinumDB[(short)idx].b;
    if (mode == kGAMEEVENT_RIVALPOINTS)
        return cBELibrary_platinumDB[(short)idx + 3].b * 100;
    return kNoPlatinumGoal;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getRival);
#ifdef SKIP_ASM
void* cBENewPlayerInterface_getThis();
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex);
extern "C" int cBENewPlayerInterface_getHumanRider(void* self, int a1);

/**
 * Returns the rival character of the current peak (cBENewRaceInterface_getLocationPeak of the race's location): Mac
 * on peak 0, Nate on peak 1, Psymon on peak 2, or Griff, Zoe, Elise when the human rider already plays that
 * character; Mac otherwise.
 *
 * @return the rival's character id (eBECharacters; [O] On Tour's getRival returns eBECharacters)
 */
int cBENewRaceInterface::getRival()
{
    void* players = cBENewPlayerInterface_getThis();
    int humanChar = cBENewPlayerInterface_getRiderCharID(players, cBENewPlayerInterface_getHumanRider(cBENewPlayerInterface_getThis(), 0));
    switch (getLocationPeak(cBENewRaceInterface_newRace.iLocationIdx))
    {
    case kPEAK_1:
        return humanChar == kCHAR_MAC ? kCHAR_GRIFF : kCHAR_MAC;
    case kPEAK_2:
        return humanChar == kCHAR_NATE ? kCHAR_ZOE : kCHAR_NATE;
    case kPEAK_3:
        return humanChar == kCHAR_PSYMON ? kCHAR_ELISE : kCHAR_PSYMON;
    }
    return kCHAR_MAC;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationDefaultGameMode);
#ifdef SKIP_ASM
/**
 * Returns the default game mode of location @p location: race for the race tracks (0-4), slope style for 5-7, big air
 * for 8-10, half pipe for 11-13, free ride otherwise.
 *
 * @param location  the location (eBELocationID)
 * @return the game mode (eBEGameMode)
 */
int cBENewRaceInterface::getLocationDefaultGameMode(unsigned int location)
{
    switch (location) {
    case kLOCATION_ABC1:
    case kLOCATION_DBC2:
    case kLOCATION_EBC3:
        return kGAMEMODE_FREERIDE;
    case kLOCATION_ARA1:
    case kLOCATION_BRA2:
    case kLOCATION_CRA3:
    case kLOCATION_DRA4:
    case kLOCATION_ERA5:
        return kGAMEMODE_RACE;
    case kLOCATION_ASS1:
    case kLOCATION_DSS2:
    case kLOCATION_ESS3:
        return kGAMEMODE_SLOPESTYLE;
    case kLOCATION_ABA1:
    case kLOCATION_CBA2:
    case kLOCATION_EBA3:
        return kGAMEMODE_BIGAIR;
    case kLOCATION_BHP1:
    case kLOCATION_CHP2:
    case kLOCATION_EHP3:
        return kGAMEMODE_HALFPIPE;
    default:
        return kGAMEMODE_FREERIDE;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationDefaultGameEvent);
#ifdef SKIP_ASM
/**
 * Returns the default game event of location @p idx: race for the race tracks (0-4), slope style for 5-7, big air for
 * 8-10, super pipe for 11-13, free ride otherwise.
 *
 * @param idx  the location (eBELocationID)
 * @return the game event (eBEGameEvent)
 */
// From peak3project's fork (github.com/peak3project/ssx3).
int cBENewRaceInterface::getLocationDefaultGameEvent(int idx)
{
    switch (idx)
    {
    case kLOCATION_ABC1:
    case kLOCATION_DBC2:
    case kLOCATION_EBC3:
        return kGAMEEVENT_FREERIDE;
    case kLOCATION_ARA1:
    case kLOCATION_BRA2:
    case kLOCATION_CRA3:
    case kLOCATION_DRA4:
    case kLOCATION_ERA5:
        return kGAMEEVENT_RACE;
    case kLOCATION_ASS1:
    case kLOCATION_DSS2:
    case kLOCATION_ESS3:
        return kGAMEEVENT_SLOPESTYLE;
    case kLOCATION_ABA1:
    case kLOCATION_CBA2:
    case kLOCATION_EBA3:
        return kGAMEEVENT_BIGAIR;
    case kLOCATION_BHP1:
    case kLOCATION_CHP2:
    case kLOCATION_EHP3:
        return kGAMEEVENT_SUPERPIPE;
    }
    return kGAMEEVENT_FREERIDE;
}
#endif


//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getDefaultGameMode);
#ifdef SKIP_ASM
/**
 * Returns the default game mode of game event @p gameEvent: 0->0, 1->1, 2->3, 3->2, 4->5, 5->6, 12 and 13->4, others 0.
 *
 * @param gameEvent  the game event (eBEGameEvent)
 * @return the game mode (eBEGameMode)
 */
int cBENewRaceInterface::getDefaultGameMode(unsigned int gameEvent)
{
    switch (gameEvent) {
    case kGAMEEVENT_RACE:
        return kGAMEMODE_RACE;
    case kGAMEEVENT_SLOPESTYLE:
        return kGAMEMODE_SLOPESTYLE;
    case kGAMEEVENT_SUPERPIPE:
        return kGAMEMODE_HALFPIPE;
    case kGAMEEVENT_BIGAIR:
        return kGAMEMODE_BIGAIR;
    case kGAMEEVENT_RIVALTIME:
        return kGAMEMODE_TIMECHALLENGE;
    case kGAMEEVENT_RIVALPOINTS:
        return kGAMEMODE_PNTSCHALLENGE;
    case kGAMEEVENT_FREERIDE:
    case kGAMEEVENT_MISSION:
        return kGAMEMODE_FREERIDE;
    default:
        return kGAMEMODE_RACE;
    }
}
#endif
