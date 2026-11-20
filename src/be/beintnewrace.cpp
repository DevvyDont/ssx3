#include "common.h"
#include "be/beintnewrace.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BENewRaceInt[];
extern void* D_0045AE58[16];
extern void* cBENewRaceInterface_instance;

struct cBENewRaceInterface {
    char pad_0x00[8];
    int field_0x8;
    void* vtable;
};


// padded past the 8-byte gp-relative threshold so the compiler emits
// absolute lui/lo addressing like the target, instead of assuming small data
struct sD_00535C08 { int value; int pad[2]; };
extern sD_00535C08 D_00535C08;

// padded past the 8-byte gp-relative threshold so the compiler emits
// absolute lui/lo addressing like the target, instead of assuming small data
struct sPad16 { char x; int pad[3]; };
extern sPad16 D_0043D984;

extern sPad16 D_0043D954;

// 0x64-byte array elements; arr[i].field indexing is what makes GCC emit the
// target's base-first addu (manual pointer arithmetic reverses the operands)
struct sRaceEntry {
    char pad_0x00[0x54];
    int field_0x54;
    int field_0x58;
    char pad_0x5c[4];
    int field_0x60;
};
extern sRaceEntry cBELibrary_locationDB[];

extern sPad16 D_0043E254;

struct sRaceEntry18 {
    char pad_0x00[0x14];
    int field_0x14;
};
extern sRaceEntry18 D_0043E250[];

extern sPad16 D_0043E7D0;

//99.58%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the new-race interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BENewRaceInt", clears the word at 0x8, stores the vtable
 * D_0045AE58 and keeps the object in cBENewRaceInterface_instance.
 *
 * @return the new-race interface
 */
void* cBENewRaceInterface_getThis()
{
    if (cBENewRaceInterface_instance == 0) {
        cBENewRaceInterface* mem = (cBENewRaceInterface*)cMemMan_alloc(0x10, str_BENewRaceInt, 0, 0);
        mem->field_0x8 = 0;
        mem->vtable = D_0045AE58;
        cBENewRaceInterface_instance = mem;
    }
    return cBENewRaceInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_submit);
#ifdef SKIP_ASM
struct sRaceSettings_00144928
{
    int data[0x4C / 4];
};

extern sRaceSettings_00144928 cBELibrary_raceDB;

/**
 * Commits the race being set up: copies cBENewRaceInterface_newRace (0x4C bytes) into cBELibrary_raceDB.
 */
extern "C" void cBENewRaceInterface_submit(void)
{
    cBELibrary_raceDB = *(sRaceSettings_00144928*)&cBENewRaceInterface_newRace.playerID[0];
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_discard);
#ifdef SKIP_ASM
extern sRaceSettings_00144928 cBELibrary_raceDB;

/**
 * Drops changes to the race being set up: copies cBELibrary_raceDB back into cBENewRaceInterface_newRace (0x4C bytes).
 */
extern "C" void cBENewRaceInterface_discard(void)
{
    *(sRaceSettings_00144928*)&cBENewRaceInterface_newRace.playerID[0] = cBELibrary_raceDB;
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
extern "C" void cBENewRaceInterface_setDebugLocationFile(void* self, const char* name)
{
    char* base = (char*)&D_0043D984;
    char* newRace = (char*)&(*(&cBENewRaceInterface_newRace));
    strcpy(base + *(int*)(newRace + 0x40) * 100, name);
    char* p1 = base + 0x10;
    strcpy(p1 + *(int*)(newRace + 0x40) * 100, name);
    char* p2 = base - 0x30;
    strcpy(p2 + *(int*)(newRace + 0x40) * 100, name);
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
    return (char*)&cBELibrary_locationDB[0] + D_00535C08.value * 0x64;
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
 * @return the location id stored in D_00535C08
 */
extern "C" int cBENewRaceInterface_getLocationID()
{
    return D_00535C08.value;
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
    return (char*)&D_0043D954 + D_00535C08.value * 0x64;
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
extern "C" void* cBENewRaceInterface_getLocationShortName(void* self)
{
    return str_Snow_Jam + D_00535C08.value * 100;
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
extern "C" void* cBENewRaceInterface_getLocationBigFile(void* self, int i)
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
extern "C" void* cBENewRaceInterface_getLocationFile(void* self, int iLocation)
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
extern "C" void* cBENewRaceInterface_getLocationName(void* self, int iLocation)
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
extern "C" int cBENewRaceInterface_getLocationPeak(void* self, int location)
{
    return cBELibrary_locationDB[location].field_0x54;
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
    return cBELibrary_locationDB[D_00535C08.value].field_0x54;
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
extern "C" int cBENewRaceInterface_getLocationType(void* self, int iLocation)
{
    return cBELibrary_locationDB[iLocation].field_0x58;
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
extern "C" int cBENewRaceInterface_getLocationIDFromScript(void* self, int scriptId)
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
extern "C" int cBENewRaceInterface_getLocationDifficulty(void* self, int location)
{
    return cBELibrary_locationDB[location].field_0x60;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getSectionFile);
#ifdef SKIP_ASM
/**
 * Returns the file name of a world section.
 *
 * D_0043E254 + @p iSection * 0x18: the name in the section table, whose records start 4 bytes earlier at D_0043E250.
 *
 * @param iSection  the section index (On Tour iSection)
 * @return the section's file name
 */
extern "C" void* cBENewRaceInterface_getSectionFile(void* self, int iSection)
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
 * D_0043E250[@p iSection].field_0x14.
 *
 * @param iSection  the section index (On Tour iSection)
 * @return the section type (On Tour eBESectionType)
 */
extern "C" int cBENewRaceInterface_getSectionType(void* self, int iSection)
{
    return D_0043E250[iSection].field_0x14;
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
    *(int*)((char*)(void*)&(*(&cBENewRaceInterface_newRace)) + iGate * 4) = iPlayer;
    *(int*)((char*)((char*)(void*)&(*(&cBENewRaceInterface_newRace)) + iPlayer * 4) + 0x18) = iGate;
    return ((char*)(void*)&(*(&cBENewRaceInterface_newRace)) + iGate * 4);
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_setNumberAI);
#ifdef SKIP_ASM
// Max AI riders per [gameType][eventKind] (7 event kinds per row).
extern unsigned int D_0043D8F8[][7];

/**
 * Sets the number of AI riders in the new race and checks it against the limit for the race type.
 *
 * Stores @p count and recomputes iNumberRiders (humans + AI + mission riders) even when the count is over the
 * limit. The limit comes from the table D_0043D8F8, indexed by cGamePath and cGameMode.
 *
 * @param count  number of AI riders
 * @return 1 when @p count is within the limit (compared unsigned, so a negative count fails), else 0
 */
extern "C" int cBENewRaceInterface_setNumberAI(void* self, int count)
{
    cBENewRaceInterface_newRace.iNumberAI = count;
    cBENewRaceInterface_newRace.iNumberRiders = cBENewRaceInterface_newRace.iNumberHumans + count + cBENewRaceInterface_newRace.iNumberMissionRiders;
    signed char eventKind = cBENewRaceInterface_newRace.cGameMode;
    signed char gameType = cBENewRaceInterface_newRace.cGamePath;
    return (unsigned int)count <= D_0043D8F8[gameType][eventKind];
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_setGameMode);
#ifdef SKIP_ASM
extern "C" int cBENewRaceInterface_setNumberAI(void* self, int count);
extern "C" int cBENewRaceInterface_setGameEvent(void* self, int event);
extern "C" void cBENewPlayerInterface_resetFromMissionMan(void* self, int a1);
extern "C" void cBENewPlayerInterface_setToMissionMan(int a0);
int cBENewPlayerInterface_isMissionMan(int a0);
void* cBENewPlayerInterface_getThis();

// Allowed game modes per game type (one byte per mode).
struct sModeRow_144DF0 {
    signed char allowed[0x43];
};
extern sModeRow_144DF0 D_0043E700[];

struct sVE_144DF0 {
    short delta;
    short index;
    void (*fn)(void*);
};

static inline char* Race_144DF0()
{
    return (char*)&(*(&cBENewRaceInterface_newRace));
}

/**
 * Sets the race's game mode and the matching numbers of human, AI and mission riders.
 *
 * Refused when the mode is not allowed on the current game path (D_0043E700). Mode 4 gets 1 human and 1
 * mission rider and makes player 1 the mission man with player 0's character (cBENewPlayerInterface_setToMissionMan), then submits;
 * leaving mode 4 restores the humans (2 on path 2) and resets player 1. Modes 0 (5 AI, 4 on path 2), 1 (1 AI)
 * and 3, 5, 6 (1 AI for events 4-5 off path 2) set the counts; mode 5 also sets event 4.
 *
 * @param mode  the game mode
 * @return 1 when set, 0 when the mode is not allowed
 */
extern "C" int cBENewRaceInterface_setGameMode(void* self, int mode)
{
    sModeRow_144DF0 row = D_0043E700[*((signed char*)Race_144DF0() + 0x49)];
    int humans = 1;
    int ai = 5;
    if (*((signed char*)Race_144DF0() + 0x49) == 2) {
        humans = 2;
        ai = 4;
    }
    if (row.allowed[mode] != 1) {
        return 0;
    }
    if (mode == 4) {
        *((signed char*)Race_144DF0() + 0x48) = mode;
        cBENewRaceInterface_setNumberHumans(self, 1);
        cBENewRaceInterface_setNumberAI(self, 0);
        cBENewRaceInterface_setNumberMission(self, 1);
        char* playerInterface = (char*)cBENewPlayerInterface_getThis();
        cBENewPlayerInterface_setToMissionMan(1);
        sVE_144DF0* vt = *(sVE_144DF0**)(playerInterface + 0xC);
        vt[1].fn(playerInterface + vt[1].delta);
    } else if (*((signed char*)Race_144DF0() + 0x48) == 4) {
        cBENewRaceInterface_setNumberHumans(self, humans);
        cBENewRaceInterface_setNumberAI(self, 0);
        cBENewRaceInterface_setNumberMission(self, 0);
        char* playerInterface = (char*)cBENewPlayerInterface_getThis();
        if (cBENewPlayerInterface_isMissionMan(1) != 0) {
            cBENewPlayerInterface_resetFromMissionMan(playerInterface, 1);
            sVE_144DF0* vt = *(sVE_144DF0**)(playerInterface + 0xC);
            vt[1].fn(playerInterface + vt[1].delta);
        }
    }
    *((signed char*)Race_144DF0() + 0x48) = mode;
    if (mode == 3 || mode == 5 || mode == 6) {
        cBENewRaceInterface_setNumberHumans(self, humans);
        unsigned char ev = *((unsigned char*)Race_144DF0() + 0x4A);
        if (ev >= 4 && ev <= 5 && *((signed char*)Race_144DF0() + 0x49) != 2) {
            cBENewRaceInterface_setNumberAI(self, 1);
        } else {
            cBENewRaceInterface_setNumberAI(self, 0);
        }
        cBENewRaceInterface_setNumberMission(self, 0);
    }
    if (mode == 0) {
        cBENewRaceInterface_setNumberHumans(self, humans);
        cBENewRaceInterface_setNumberAI(self, ai);
        cBENewRaceInterface_setNumberMission(self, 0);
    }
    if (mode == 1) {
        cBENewRaceInterface_setNumberHumans(self, humans);
        cBENewRaceInterface_setNumberAI(self, 1);
        cBENewRaceInterface_setNumberMission(self, 0);
    }
    if (mode == 5) {
        cBENewRaceInterface_setGameEvent(self, 4);
    }
    return 1;
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
extern "C" void cBENewRaceInterface_setGamePath(void* self, int mode)
{
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    *((char*)beRaceDB + 0x49) = mode;
    switch (mode)
    {
    case 0:
        cBENewRaceInterface_setNumberHumans(self, 1);
        cBENewRaceInterface_setNumberAI(self, 0);
        break;
    case 1:
        cBENewRaceInterface_setNumberHumans(self, 1);
        if (beRaceDB->iNumberAI < 0)
            cBENewRaceInterface_setNumberAI(self, 0);
        if (beRaceDB->iNumberAI >= 6)
            cBENewRaceInterface_setNumberAI(self, 5);
        break;
    case 2:
        cBENewRaceInterface_setNumberHumans(self, 2);
        cBENewRaceInterface_setNumberAI(self, 0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_setGameEvent);
#ifdef SKIP_ASM
extern "C" int cBENewRaceInterface_setNumberAI(void* self, int count);

struct sGameEvent_001451E8
{
    int kind;
    char rest[0x50];
};
extern sGameEvent_001451E8 cBELibrary_gameEventDB[];

static inline char* Race_1451E8()
{
    return (char*)&(*(&cBENewRaceInterface_newRace));
}

/**
 * Sets the race's game event; events 4 and 5 also set one AI rider (none on game path 2).
 *
 * @param event  the game event
 * @return whether the event's mode (cBELibrary_gameEventDB) matches the current game mode
 */
extern "C" int cBENewRaceInterface_setGameEvent(void* self, int event)
{
    sGameEvent_001451E8 eventRec = cBELibrary_gameEventDB[event];
    *((signed char*)Race_1451E8() + 0x4A) = event;
    if (event < 6)
    {
        if (event >= 4)
        {
            if (*((signed char*)Race_1451E8() + 0x49) == 2)
                cBENewRaceInterface_setNumberAI(self, 0);
            else
                cBENewRaceInterface_setNumberAI(self, 1);
        }
    }
    return eventRec.kind == *((signed char*)Race_1451E8() + 0x48);
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getEventName);
#ifdef SKIP_ASM
extern signed char D_00535C12[];
extern sPad16 D_0043E97C;

/**
 * Returns the name of the race's game event: +0x4 of its 0x54-byte cBELibrary_gameEventDB record (D_0043E97C +
 * newRace.cGameEvent * 0x54).
 *
 * @return the event name
 */
extern "C" void* cBENewRaceInterface_getEventName()
{
    return (char*)&D_0043E97C + D_00535C12[0] * 0x54;
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
extern "C" void* cBENewRaceInterface_getEventName1(void* self, int i)
{
    return (char*)&D_0043E97C + i * 0x54;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPathName);
#ifdef SKIP_ASM
extern signed char D_00535C11[];
extern char str_World_Circuit[]; // 0x43-byte records at an odd address

/**
 * Returns the name of the race's game path: str_World_Circuit + newRace.cGamePath * 67 (67-byte records).
 *
 * @return the path name
 */
extern "C" void* cBENewRaceInterface_getPathName()
{
    return str_World_Circuit + D_00535C11[0] * 67;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getModeName);
#ifdef SKIP_ASM
extern signed char D_00535C10[];

/**
 * Returns the record (name first) of the race's game mode: D_0043E7D0 + newRace.cGameMode * 0x3C.
 *
 * @return the mode name
 */
extern "C" void* cBENewRaceInterface_getModeName()
{
    return (char*)&D_0043E7D0 + D_00535C10[0] * 0x3c;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getModeName1);
#ifdef SKIP_ASM
/**
 * Returns the record (name first) of game mode @p gameMode: D_0043E7D0 + @p gameMode * 0x3C.
 *
 * @param gameMode  the game mode
 * @return the mode name
 */
extern "C" void* cBENewRaceInterface_getModeName1(void* self, int gameMode)
{
    return (char*)&D_0043E7D0 + gameMode * 0x3c;
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
// Typed view of D_00440B38 (declared later in this unit with another layout).
extern sPrizeRow_1453D0 D_00440B38_1453D0[] __asm__("D_00440B38");

/**
 * Returns a simulated rider's score for round @p kind: the D_00440B38 row of the race's location and that round,
 * column @p place, times 100, randomised by up to +-5% and rounded down to a multiple of 20.
 *
 * Difficulty 0 scales by 0.9 and 2 by 1.2, except for column 0. Round 0 counts as 1. -1 when no row matches.
 *
 * @param kind  the round (heat)
 * @param place  the rider column
 * @param mode  the difficulty, 0-2
 * @return the score, or -1
 */
extern "C" int cBENewRaceInterface_getSimThreshold(void* self, int kind, int place, int mode)
{
    int k = 1;
    if (kind) k = kind;
    float difficultyScale = 1.0f;
    if (mode == 0 && place != 0)
        difficultyScale = 0.8999999761581421f;
    else if (mode == 2 && place != 0)
        difficultyScale = 1.2000000476837158f;
    int key = D_00535C08.value;
    for (int i = 0; i < 30; i++)
    {
        if (D_00440B38_1453D0[i].key == key && D_00440B38_1453D0[i].kind == k)
        {
            int base = (int)(D_00440B38_1453D0[i].prize[place] * 100 * difficultyScale);
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
extern "C" int cBENewRaceInterface_getPeakTimeLimit(void* self, int a1, int a2);
extern int cBENewRaceInterface_mNextLocation;

struct sRaceLookup_00440B38
{
    short key;
    short kind;
    char pad_0x04[0xA];
    short value;
};
extern sRaceLookup_00440B38 D_00440B38[];

/**
 * Returns the time limit of round @p kind, in the units the game mode multiplies by 60.
 *
 * For game events 6-11 (newRace +0x4A) it is cBENewRaceInterface_getPeakTimeLimit(event, @p kind). Otherwise the halfword at +0x10 of the
 * D_00440B38 row for the race's location and the round (round 0 counts as 1), falling back to
 * cBENewRaceInterface_mNextLocation's row; -1 when neither exists.
 *
 * @param kind  the round
 * @return the time limit, or -1
 */
extern "C" int cBENewRaceInterface_getTimeLimit(void* self, int kind)
{
    char* newRaceDb = (char*)&(*(&cBENewRaceInterface_newRace));
    unsigned char mode = *(unsigned char*)(newRaceDb + 0x4A);
    if (mode >= 6 && mode < 12)
    {
        return cBENewRaceInterface_getPeakTimeLimit(self, (signed char)mode, kind);
    }
    if (kind == 0)
    {
        kind = 1;
    }
    int key = *(int*)(newRaceDb + 0x40);
    for (int i = 0; i < 30; i++)
    {
        if (D_00440B38[i].key == key && D_00440B38[i].kind == kind)
        {
            return D_00440B38[i].value;
        }
    }
    for (int i = 0; i < 30; i++)
    {
        if (D_00440B38[i].key == cBENewRaceInterface_mNextLocation && D_00440B38[i].kind == kind)
        {
            return D_00440B38[i].value;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPeakGoal);
#ifdef SKIP_ASM
struct sRaceTableEntry_00440D18
{
    char pad_0x00[4];
    unsigned short field_0x4;
    unsigned short field_0x6;
    unsigned short field_0x8[5];
    unsigned short field_0x12;
};

extern sRaceTableEntry_00440D18 D_00440D18[];

/**
 * Returns the goal of peak event @p gameEvent at tier @p tier: the halfword at +0x4 of D_00440D18[(@p gameEvent - 6) * 3 + @p tier].
 *
 * @param gameEvent  the game event, 6-11
 * @param tier  the tier, 0-2
 * @return the goal
 */
extern "C" int cBENewRaceInterface_getPeakGoal(void* self, int gameEvent, int tier)
{
    return D_00440D18[tier + (gameEvent - 6) * 3].field_0x4;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPeakTimeLimit);
#ifdef SKIP_ASM
extern sRaceTableEntry_00440D18 D_00440D18[];

/**
 * Returns the time limit of peak event @p gameEvent at tier @p tier: the halfword at +0x6 of
 * D_00440D18[(@p gameEvent - 6) * 3 + @p tier].
 *
 * @param gameEvent  the game event, 6-11
 * @param tier  the tier, 0-2
 * @return the time limit
 */
extern "C" int cBENewRaceInterface_getPeakTimeLimit(void* self, int gameEvent, int tier)
{
    return D_00440D18[tier + (gameEvent - 6) * 3].field_0x6;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPeakRewardAmount);
#ifdef SKIP_ASM
extern sRaceTableEntry_00440D18 D_00440D18[];

/**
 * Returns the reward of peak event @p gameEvent at tier @p tier: the halfword at +0x12 of D_00440D18[(@p gameEvent - 6) * 3 + @p tier],
 * times 100.
 *
 * @param gameEvent  the game event, 6-11
 * @param tier  the tier, 0-2
 * @return the money
 */
extern "C" int cBENewRaceInterface_getPeakRewardAmount(void* self, int gameEvent, int tier)
{
    return D_00440D18[tier + (gameEvent - 6) * 3].field_0x12 * 100;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getPeakSplitTimePoints);
#ifdef SKIP_ASM
extern sRaceTableEntry_00440D18 D_00440D18[];

/**
 * Returns the bonus of checkpoint @p checkpoint in peak event @p gameEvent at tier @p tier: halfword @p checkpoint of the five at +0x8 of
 * D_00440D18[(@p gameEvent - 6) * 3 + @p tier].
 *
 * @param gameEvent  the game event, 6-11
 * @param tier  the tier, 0-2
 * @param checkpoint  the checkpoint, 0-4
 * @return the bonus
 */
extern "C" int cBENewRaceInterface_getPeakSplitTimePoints(void* self, int gameEvent, int tier, int checkpoint)
{
    return D_00440D18[tier + (gameEvent - 6) * 3].field_0x8[checkpoint];
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

extern sRacePair_001456A0 D_00440E80[];

/**
 * Returns the target for location @p idx and event @p mode from D_00440E80, or -1.
 *
 * Events 6-11 give -1. Below location 14 it is entry @p idx's second halfword (times 100 unless @p mode is 0); from
 * 14 on, @p mode 4 gives entry @p idx's value, @p mode 5 entry @p idx + 3's times 100, others -1.
 *
 * @param idx  the location
 * @param mode  the event
 * @return the target, or -1
 */
extern "C" int cBENewRaceInterface_getPlatinumGoal(void* self, int idx, int mode)
{
    if (mode == 9 || mode == 10 || mode == 11 || mode == 6 || mode == 7 || mode == 8)
        return -1;
    if (idx < 14)
    {
        if (mode == 0)
            return D_00440E80[(short)idx].b;
        return D_00440E80[(short)idx].b * 100;
    }
    if (mode == 4)
        return D_00440E80[(short)idx].b;
    if (mode == 5)
        return D_00440E80[(short)idx + 3].b * 100;
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getRival);
#ifdef SKIP_ASM
void* cBENewPlayerInterface_getThis();
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex);
extern "C" int cBENewPlayerInterface_getHumanRider(void* self, int a1);

/**
 * Returns the rival character of the current peak (cBENewRaceInterface_getLocationPeak of the race's location): 3 on peak 0, 7 on peak 1,
 * 8 on peak 2, or 5, 4, 6 when the human rider already plays that character; 3 otherwise.
 *
 * @return the rival's character id
 */
extern "C" int cBENewRaceInterface_getRival(void* self)
{
    void* players = cBENewPlayerInterface_getThis();
    int humanChar = cBENewPlayerInterface_getRiderCharID(players, cBENewPlayerInterface_getHumanRider(cBENewPlayerInterface_getThis(), 0));
    switch (cBENewRaceInterface_getLocationPeak(self, D_00535C08.value))
    {
    case 0:
        return humanChar == 3 ? 5 : 3;
    case 1:
        return humanChar == 7 ? 4 : 7;
    case 2:
        return humanChar == 8 ? 6 : 8;
    }
    return 3;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationDefaultGameMode);
#ifdef SKIP_ASM
/**
 * Returns the default game mode of location @p location: 0 for locations 0-4, 1 for 5-7, 2 for 8-10, 3 for 11-13, 4
 * otherwise.
 *
 * @param location  the location
 * @return the game mode
 */
extern "C" int cBENewRaceInterface_getLocationDefaultGameMode(void* self, unsigned int location) {
    switch (location) {
    case 14:
    case 15:
    case 16:
        return 4;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return 0;
    case 5:
    case 6:
    case 7:
        return 1;
    case 8:
    case 9:
    case 10:
        return 2;
    case 11:
    case 12:
    case 13:
        return 3;
    default:
        return 4;
    }
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getLocationDefaultGameEvent);
#ifdef SKIP_ASM
/**
 * Returns the default game event of location @p idx: 0 for locations 0-4, 1 for 5-7, 3 for 8-10, 2 for 11-13, 12
 * otherwise.
 *
 * @param idx  the location
 * @return the game event
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cBENewRaceInterface_getLocationDefaultGameEvent(void* self, int idx)
{
    switch (idx)
    {
    case 14:
    case 15:
    case 16:
        return 12;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return 0;
    case 5:
    case 6:
    case 7:
        return 1;
    case 8:
    case 9:
    case 10:
        return 3;
    case 11:
    case 12:
    case 13:
        return 2;
    }
    return 12;
}
#endif


//100%
INCLUDE_ASM("be/beintnewrace", cBENewRaceInterface_getDefaultGameMode);
#ifdef SKIP_ASM
/**
 * Returns the default game mode of game event @p gameEvent: 0->0, 1->1, 2->3, 3->2, 4->5, 5->6, 12 and 13->4, others 0.
 *
 * @param gameEvent  the game event
 * @return the game mode
 */
extern "C" int cBENewRaceInterface_getDefaultGameMode(void* self, unsigned int gameEvent) {
    switch (gameEvent) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 3;
    case 3:
        return 2;
    case 4:
        return 5;
    case 5:
        return 6;
    case 12:
    case 13:
        return 4;
    default:
        return 0;
    }
}
#endif
