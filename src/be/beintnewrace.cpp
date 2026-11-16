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
INCLUDE_ASM("be/beintnewrace", func_00144928);
#ifdef SKIP_ASM
struct sRaceSettings_00144928
{
    int data[0x4C / 4];
};

extern sRaceSettings_00144928 cBELibrary_raceDB;

extern "C" void func_00144928(void)
{
    cBELibrary_raceDB = *(sRaceSettings_00144928*)&cBENewRaceInterface_newRace.playerID[0];
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_001449E8);
#ifdef SKIP_ASM
extern sRaceSettings_00144928 cBELibrary_raceDB;

extern "C" void func_001449E8(void)
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
    char* g = (char*)&(*(&cBENewRaceInterface_newRace));
    strcpy(base + *(int*)(g + 0x40) * 100, name);
    char* p1 = base + 0x10;
    strcpy(p1 + *(int*)(g + 0x40) * 100, name);
    char* p2 = base - 0x30;
    strcpy(p2 + *(int*)(g + 0x40) * 100, name);
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
INCLUDE_ASM("be/beintnewrace", func_00144BF0);
#ifdef SKIP_ASM
extern "C" void* func_00144BF0()
{
    return (char*)&D_0043D954 + D_00535C08.value * 0x64;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_00144C10);
#ifdef SKIP_ASM
extern char str_Snow_Jam[];

extern "C" void* func_00144C10(void* self)
{
    return str_Snow_Jam + D_00535C08.value * 100;
}
#endif
//100%
INCLUDE_ASM("be/beintnewrace", func_00144C30);
#ifdef SKIP_ASM
extern char str_BAM[];

extern "C" void* func_00144C30(void* self, int i)
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
INCLUDE_ASM("be/beintnewrace", func_00144C78);
#ifdef SKIP_ASM
extern "C" int func_00144C78(void* self, int a1)
{
    return cBELibrary_locationDB[a1].field_0x54;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_00144C98);
#ifdef SKIP_ASM
extern "C" int func_00144C98()
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
INCLUDE_ASM("be/beintnewrace", func_00144CE0);
#ifdef SKIP_ASM
struct sRaceEntryFull_00144CE0 {
    int field_0x0;
    char pad_0x04[0x58];
    int field_0x5C;
    int field_0x60;
};

extern "C" int func_00144CE0(void* self, int a1)
{
    sRaceEntryFull_00144CE0* t = (sRaceEntryFull_00144CE0*)cBELibrary_locationDB;
    for (int i = 0; i < 23; i++) {
        if (t[i].field_0x5C == a1) {
            return t[i].field_0x0;
        }
    }
    return 22;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_00144D18);
#ifdef SKIP_ASM
extern "C" int func_00144D18(void* self, int a1)
{
    return cBELibrary_locationDB[a1].field_0x60;
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
INCLUDE_ASM("be/beintnewrace", func_00145340);
#ifdef SKIP_ASM
extern signed char D_00535C12[];
extern sPad16 D_0043E97C;

extern "C" void* func_00145340()
{
    return (char*)&D_0043E97C + D_00535C12[0] * 0x54;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_00145360);
#ifdef SKIP_ASM
extern "C" void* func_00145360(void* self, int i)
{
    return (char*)&D_0043E97C + i * 0x54;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_00145378);
#ifdef SKIP_ASM
extern signed char D_00535C11[];
extern char str_World_Circuit[]; // 0x43-byte records at an odd address

extern "C" void* func_00145378()
{
    return str_World_Circuit + D_00535C11[0] * 67;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_00145398);
#ifdef SKIP_ASM
extern signed char D_00535C10[];

extern "C" void* func_00145398()
{
    return (char*)&D_0043E7D0 + D_00535C10[0] * 0x3c;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_001453B8);
#ifdef SKIP_ASM
extern "C" void* func_001453B8(void* self, int a1)
{
    return (char*)&D_0043E7D0 + a1 * 0x3c;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_001453D0);
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

extern "C" int func_001453D0(void* self, int kind, int place, int mode)
{
    int k = 1;
    if (kind) k = kind;
    float m = 1.0f;
    if (mode == 0 && place != 0)
        m = 0.8999999761581421f;
    else if (mode == 2 && place != 0)
        m = 1.2000000476837158f;
    int key = D_00535C08.value;
    for (int i = 0; i < 30; i++)
    {
        if (D_00440B38_1453D0[i].key == key && D_00440B38_1453D0[i].kind == k)
        {
            int base = (int)(D_00440B38_1453D0[i].prize[place] * 100 * m);
            int r = GMrand() % 200;
            base += (int)(base * (r - 100) * 0.0005000000237487257f);
            return base - base % 20;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_001454F8);
#ifdef SKIP_ASM
extern "C" int func_00145600(void* self, int a1, int a2);
extern int cBENewRaceInterface_mNextLocation;

struct sRaceLookup_00440B38
{
    short key;
    short kind;
    char pad_0x04[0xA];
    short value;
};
extern sRaceLookup_00440B38 D_00440B38[];

extern "C" int func_001454F8(void* self, int kind)
{
    char* g = (char*)&(*(&cBENewRaceInterface_newRace));
    unsigned char mode = *(unsigned char*)(g + 0x4A);
    if (mode >= 6 && mode < 12)
    {
        return func_00145600(self, (signed char)mode, kind);
    }
    if (kind == 0)
    {
        kind = 1;
    }
    int key = *(int*)(g + 0x40);
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
INCLUDE_ASM("be/beintnewrace", func_001455D0);
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

extern "C" int func_001455D0(void* self, int a1, int a2)
{
    return D_00440D18[a2 + (a1 - 6) * 3].field_0x4;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_00145600);
#ifdef SKIP_ASM
extern sRaceTableEntry_00440D18 D_00440D18[];

extern "C" int func_00145600(void* self, int a1, int a2)
{
    return D_00440D18[a2 + (a1 - 6) * 3].field_0x6;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_00145630);
#ifdef SKIP_ASM
extern sRaceTableEntry_00440D18 D_00440D18[];

extern "C" int func_00145630(void* self, int a1, int a2)
{
    return D_00440D18[a2 + (a1 - 6) * 3].field_0x12 * 100;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_00145668);
#ifdef SKIP_ASM
extern sRaceTableEntry_00440D18 D_00440D18[];

extern "C" int func_00145668(void* self, int a1, int a2, int a3)
{
    return D_00440D18[a2 + (a1 - 6) * 3].field_0x8[a3];
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_001456A0);
#ifdef SKIP_ASM
struct sRacePair_001456A0
{
    short a;
    short b;
};

extern sRacePair_001456A0 D_00440E80[];

extern "C" int func_001456A0(void* self, int idx, int mode)
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
INCLUDE_ASM("be/beintnewrace", func_00145750);
#ifdef SKIP_ASM
void* cBENewPlayerInterface_getThis();
signed char cBENewPlayerInterface_getRiderCharID(void* self, int riderIndex);
extern "C" int cBENewPlayerInterface_getHumanRider(void* self, int a1);

extern "C" int func_00145750(void* self)
{
    void* players = cBENewPlayerInterface_getThis();
    int c = cBENewPlayerInterface_getRiderCharID(players, cBENewPlayerInterface_getHumanRider(cBENewPlayerInterface_getThis(), 0));
    switch (func_00144C78(self, D_00535C08.value))
    {
    case 0:
        return c == 3 ? 5 : 3;
    case 1:
        return c == 7 ? 4 : 7;
    case 2:
        return c == 8 ? 6 : 8;
    }
    return 3;
}
#endif

//100%
INCLUDE_ASM("be/beintnewrace", func_00145818);
#ifdef SKIP_ASM
extern "C" int func_00145818(void* self, unsigned int arg1) {
    switch (arg1) {
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
INCLUDE_ASM("be/beintnewrace", func_00145870);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int func_00145870(void* self, int idx)
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
INCLUDE_ASM("be/beintnewrace", func_001458C8);
#ifdef SKIP_ASM
extern "C" int func_001458C8(void* self, unsigned int arg1) {
    switch (arg1) {
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
