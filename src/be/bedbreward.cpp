#include "common.h"
#include "be/beintnewrace.h"
#include "be/berewarddb.h"
#include "be/beintnetwork.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("be/bedbreward", cBERewardDB_init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int BXFILE_sizez(int file);
extern "C" void FILE_loadatz(int file, void* data, int size);
extern void cBERewardDB_initRewards_s(void* self, void* data) __asm__("cBERewardDB_initRewards");
extern "C" int func_0019DA10(void* self, int bank, int id, int a3, int a4, int a5);
extern int gRewardsFileName;
extern char str_Rewards_File[];

/**
 * Loads the reward database "data/be/rwrdps2.dat", at once or through the front end's asynchronous loader.
 *
 * Synchronously it reads the whole file into a "Rewards File" block, parses it (cBERewardDB_initRewards) and stores -1
 * at +0x0; otherwise it stores the request id that func_0019DA10 returns for the front end's +0x1A70 object.
 *
 * @param async  0 to load now, else passed on to the asynchronous request
 */
extern "C" void cBERewardDB_init(int* self, int async)
{
    if (async == 0)
    {
        int size = BXFILE_sizez(gRewardsFileName);
        void* data = operator_new_tag(size, str_Rewards_File, 0, 0);
        FILE_loadatz(gRewardsFileName, data, size);
        cBERewardDB_initRewards_s(self, data);
        *self = -1;
    }
    else
    {
        *self = func_0019DA10((char*)SSXApp->RealFE + 0x1A70, 0, gRewardsFileName, 10, async, 0);
    }
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBERewardDB_purge);
#ifdef SKIP_ASM
extern "C" void func_0019DC20(void* self, int bank, int i);
extern "C" void func_0019D4B8(void* self, int bank, int i);
void cMemMan_free(void* p);

struct sBEDB_0015AE00
{
    int handle;
    void* data;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
    int f28;
    int f2C;
    int f30;
    int f34;
    int f38;
    int f3C;
    int f40;
    int f44;
    int f48;
    int f4C;
};

/**
 * Releases the reward database: cancels its pending file request, frees the loaded rewards file and clears the fields.
 *
 * A request id at +0x0 that is not negative is dropped on the front end's loader (SSXApp->RealFE + 0x1A70) with
 * func_0019DC20 and func_0019D4B8. The file buffer (+0x4) is freed with cMemMan_free. Then +0x0 becomes -1 and the
 * other words up to +0x4C become 0, except +0x28, which is left as it was (likely an oversight).
 */
extern "C" void cBERewardDB_purge(cBERewardDB* self)
{
    if (self->mRewardFileIndex >= 0)
    {
        char* snd = (char*)SSXApp->RealFE + 0x1A70;
        func_0019DC20(snd, 0, self->mRewardFileIndex);
        func_0019D4B8(snd, 0, self->mRewardFileIndex);
    }
    if (self->mpRewardsFile != 0)
    {
        cMemMan_free(self->mpRewardsFile);
    }
    self->mRewardFileIndex = -1;
    self->mpRewardsFile = 0;
    self->mpTrophies = 0;
    self->mpMedals = 0;
    self->mpPeakPasses = 0;
    self->mpPosters = 0;
    self->mpTradingCards = 0;
    self->mpConceptArts = 0;
    self->mpToys = 0;
    self->mpVideos = 0;
    self->mpStrings = 0;
    self->mNumTrophies = 0;
    self->mNumMedals = 0;
    self->mNumPeakPasses = 0;
    self->mNumPosters = 0;
    self->mNumTradingCards = 0;
    self->mNumConceptArts = 0;
    self->mNumToys = 0;
    self->mNumVideos = 0;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBERewardDB_update);
#ifdef SKIP_ASM
extern "C" int func_0019E238(void* self, int bank, int i);
extern "C" int func_0019E330(void* self, int bank, int i);
extern "C" void cBERewardDB_initRewards(int* self, int v);

/**
 * Finishes an asynchronous rewards-file load once the file has arrived.
 *
 * When the request id at +0x0 is not negative and the front end's loader (SSXApp->RealFE + 0x1A70) reports it done
 * (func_0019E238), parses the loaded data (cBERewardDB_initRewards with the buffer from func_0019E330), releases the request
 * (func_0019DC20) and sets +0x0 to -1. cBERewardDB_init starts such a load when called with async set.
 */
extern "C" void cBERewardDB_update(int* self)
{
    if (*self >= 0) {
        char* snd = (char*)SSXApp->RealFE + 0x1A70;
        if (func_0019E238(snd, 0, *self) != 0) {
            cBERewardDB_initRewards(self, func_0019E330(snd, 0, *self));
            func_0019DC20(snd, 0, *self);
            *self = -1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBERewardDB_getRewardName);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sRec12_AF40 { int id; int f4; int f8; };
struct sRec8_AF40 { int id; int f4; };
struct sRec16_AF40 { int id; int f4; int f8; int fC; };

struct sRewardDB_AF40 {
    int f0;
    int f4;
    sRec12_AF40* t0;    // 0x8
    sRec12_AF40* t1;    // 0xC
    sRec8_AF40* t2;     // 0x10
    sRec16_AF40* t3;    // 0x14
    sRec16_AF40* t4;    // 0x18
    sRec16_AF40* t5;    // 0x1C
    sRec16_AF40* t7;    // 0x20
    sRec16_AF40* t6;    // 0x24
    sRec16_AF40* t8;    // 0x28
};

/**
 * Returns the first word (the name) of reward record @p idx in the table of reward type @p type, or 0 for type 9.
 *
 * Types 0 and 1 use 12-byte records (tables at +0x8, +0xC), type 2 8-byte records (+0x10), types 3-8 16-byte records
 * (+0x14, +0x18, +0x1C, then type 6 at +0x24, type 7 at +0x20, type 8 at +0x28). No bounds check.
 *
 * @param type  the reward type, 0-9
 * @param idx  the record index
 * @return the record's first word
 */
// Returns the id of entry idx in the reward table of the given type.
extern "C" int cBERewardDB_getRewardName(cBERewardDB* self, int type, int idx)
{
    switch (type) {
    case 0: { int off = idx * 12; return *(int*)((char*)(sRec12_AF40*)self->mpTrophies + off); }
    case 1: { int off = idx * 12; return *(int*)((char*)(sRec12_AF40*)self->mpMedals + off); }
    case 2: { int off = idx * 8; return *(int*)((char*)(sRec8_AF40*)self->mpPeakPasses + off); }
    case 3: { int off = idx * 16; return *(int*)((char*)(sRec16_AF40*)self->mpPosters + off); }
    case 4: { int off = idx * 16; return *(int*)((char*)(sRec16_AF40*)self->mpTradingCards + off); }
    case 5: { int off = idx * 16; return *(int*)((char*)(sRec16_AF40*)self->mpConceptArts + off); }
    case 6: { int off = idx * 16; return *(int*)((char*)(sRec16_AF40*)self->mpVideos + off); }
    case 7: { int off = idx * 16; return *(int*)((char*)(sRec16_AF40*)self->mpToys + off); }
    case 8: { int off = idx * 16; return *(int*)((char*)(sRec16_AF40*)self->mpCheatChars + off); }
    case 9: break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015AFE8);
#ifdef SKIP_ASM
/**
 * Returns @p self; any further arguments (it is variadic) are spilled to the stack and ignored.
 *
 * Only the register save area of a varargs function remains, so this is probably a compiled-out debug helper. It sits
 * just before the unit's static initializer __sti__all_in_one_be_cpp, like func_003150B8 in animation/gameanimmap.
 *
 * @return @p self
 */
extern "C" void* func_0015AFE8(void* self, ...)
{
    return self;
}
#endif

INCLUDE_ASM("be/bedbreward", __sti__all_in_one_be_cpp);

//100%
INCLUDE_ASM("be/bedbreward", cBEInterface_submit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base back-end interface's submit (vtable slot 1 of cBEInterface's vtable D_0045AE88).
 */
void cBEInterface_submit(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEInterface_discard__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base back-end interface's discard (vtable slot 2 of cBEInterface's vtable D_0045AE88).
 */
void cBEInterface_discard(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEInterface_writeToReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Writes nothing to the replay frame: cBEInterface's writeToReplayFrame has an empty body.
 */
void cBEInterface_writeToReplayFrame(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEInterface_readFromReplayFrame__FPv);
#ifdef SKIP_ASM
/**
 * Reads nothing from the replay frame: cBEInterface's readFromReplayFrame has an empty body.
 */
void cBEInterface_readFromReplayFrame(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_purgeInterface);
#ifdef SKIP_ASM
extern void* cBENewRaceInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the new-race interface singleton cBENewRaceInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void cBENewRaceInterface_purgeInterface(void)
{
    operator_delete((int*)cBENewRaceInterface_instance);
    cBENewRaceInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_getNumberRiders);
#ifdef SKIP_ASM

/**
 * Returns the number of riders in the race being set up: cBENewRaceInterface_newRace.iNumberRiders.
 *
 * @return humans + mission riders + AI riders
 */
extern "C" int cBENewRaceInterface_getNumberRiders(void) {
    return cBENewRaceInterface_newRace.iNumberRiders;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_getNumberHumans);
#ifdef SKIP_ASM

/**
 * Returns the number of human riders in the race being set up: cBENewRaceInterface_newRace.iNumberHumans.
 *
 * @return iNumberHumans
 */
extern "C" int cBENewRaceInterface_getNumberHumans(void) {
    return cBENewRaceInterface_newRace.iNumberHumans;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_getAIRiders_Num);
#ifdef SKIP_ASM

/**
 * Returns the number of AI riders in the race being set up: cBENewRaceInterface_newRace.iNumberAI.
 *
 * @return iNumberAI
 */
extern "C" int cBENewRaceInterface_getAIRiders_Num(void) {
    return cBENewRaceInterface_newRace.iNumberAI;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_getNumberMission);
#ifdef SKIP_ASM

/**
 * Returns the number of mission riders in the race being set up: cBENewRaceInterface_newRace.iNumberMissionRiders.
 *
 * @return iNumberMissionRiders
 */
extern "C" int cBENewRaceInterface_getNumberMission(void) {
    return cBENewRaceInterface_newRace.iNumberMissionRiders;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_setRaceTime);
#ifdef SKIP_ASM

/**
 * Sets cBENewRaceInterface_newRace.iRaceTime to @p raceTime (@p self is unused).
 *
 * @param raceTime  the race time
 */
extern "C" void cBENewRaceInterface_setRaceTime(void* self, int raceTime) {
    cBENewRaceInterface_newRace.iRaceTime = raceTime;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_getRaceTime);
#ifdef SKIP_ASM

/**
 * Returns cBENewRaceInterface_newRace.iRaceTime (set by cBENewRaceInterface_setRaceTime).
 *
 * @return the race time
 */
extern "C" int cBENewRaceInterface_getRaceTime(void) {
    return cBENewRaceInterface_newRace.iRaceTime;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_getGameMode);
#ifdef SKIP_ASM

/**
 * Returns the race's game mode: the signed byte cBENewRaceInterface_newRace.cGameMode (set by
 * cBENewRaceInterface_setGameMode).
 *
 * @return cGameMode, sign-extended
 */
extern "C" int cBENewRaceInterface_getGameMode(void) {
    return cBENewRaceInterface_newRace.cGameMode;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_getGameEvent);
#ifdef SKIP_ASM

/**
 * Returns the race's game event: the signed byte cBENewRaceInterface_newRace.cGameEvent (set by
 * cBENewRaceInterface_setGameEvent).
 *
 * @return cGameEvent, sign-extended
 */
extern "C" int cBENewRaceInterface_getGameEvent(void) {
    return cBENewRaceInterface_newRace.cGameEvent;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_getGamePath);
#ifdef SKIP_ASM

/**
 * Returns the race's game path: the signed byte cBENewRaceInterface_newRace.cGamePath (0, 1, 2 = online; set by
 * cBENewRaceInterface_setGamePath).
 *
 * @return cGamePath, sign-extended
 */
extern "C" int cBENewRaceInterface_getGamePath(void) {
    return cBENewRaceInterface_newRace.cGamePath;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_setLocationID);
#ifdef SKIP_ASM
extern int D_004A11B8;

/**
 * Sets the race's location: cBENewRaceInterface_newRace.iLocationIdx = @p locationIndex, and sets the flag D_004A11B8 to 1.
 *
 * D_004A11B8 is read by cBENewRaceInterface_isDebugBAMLocation and cleared by cBENewRaceInterface_setDebugNonBAMLocation (it sits where On Tour keeps
 * cBENewRaceInterface::bIsBamLocation). @p self is unused.
 *
 * @param locationIndex  the location index
 */
extern "C" void cBENewRaceInterface_setLocationID(void* self, int locationIndex)
{
    cBENewRaceInterface_newRace.iLocationIdx = locationIndex;
    D_004A11B8 = 1;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_setNextLocationID);
#ifdef SKIP_ASM
extern int cBENewRaceInterface_mNextLocation;

/**
 * Stores @p nextLocation in cBENewRaceInterface_mNextLocation (@p self is unused).
 *
 * @param nextLocation  the location to go to next
 */
extern "C" void cBENewRaceInterface_setNextLocationID(void* self, int nextLocation) {
    cBENewRaceInterface_mNextLocation = nextLocation;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_getNextLocationID);
#ifdef SKIP_ASM
extern int cBENewRaceInterface_mNextLocation;

/**
 * Returns cBENewRaceInterface_mNextLocation (set by cBENewRaceInterface_setNextLocationID).
 *
 * @return the next location
 */
extern "C" int cBENewRaceInterface_getNextLocationID(void) {
    return cBENewRaceInterface_mNextLocation;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_isDebugBAMLocation);
#ifdef SKIP_ASM
extern int D_004A11B8;

/**
 * Returns the flag D_004A11B8: set by cBENewRaceInterface_setLocationID when a location is chosen, cleared by cBENewRaceInterface_setDebugNonBAMLocation.
 *
 * @return the flag
 */
extern "C" int cBENewRaceInterface_isDebugBAMLocation(void) {
    return D_004A11B8;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_setDebugNonBAMLocation);
#ifdef SKIP_ASM
extern int D_004A11B8;

/**
 * Clears the flag D_004A11B8 (see cBENewRaceInterface_setLocationID and cBENewRaceInterface_isDebugBAMLocation).
 */
extern "C" void cBENewRaceInterface_setDebugNonBAMLocation(void) {
    D_004A11B8 = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_setCmdLineTrack);
#ifdef SKIP_ASM
extern "C" void cBXString_operatorE1(int *);
extern int D_004A4CB8;

/**
 * Re-initialises the static string D_004A4CB8 from the C string the caller passes as its second argument.
 *
 * Calls cBXString_operatorE1(&D_004A4CB8, str), with str left in the second argument register by the caller (the
 * matched body declares no parameters). The string's previous buffer is not released. cBENewRaceInterface_getCmdLineTrack returns a copy.
 */
extern "C" void cBENewRaceInterface_setCmdLineTrack(void) {
    cBXString_operatorE1(&D_004A4CB8);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_getCmdLineTrack);
#ifdef SKIP_ASM
extern "C" void cBXString_cBXString1(void*, int*);

/**
 * Returns a copy of the static string D_004A4CB8 (set by cBENewRaceInterface_setCmdLineTrack): copy-constructs @p self from it.
 *
 * @return @p self, the returned cBXString
 */
extern "C" void* cBENewRaceInterface_getCmdLineTrack(void* self)
{
    cBXString_cBXString1(self, &D_004A4CB8);
    return self;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_setNewRaceActive);
#ifdef SKIP_ASM
extern void *cBENewRaceInterface_instance;

/**
 * Sets bIsActive (+0x8) of the new-race interface singleton cBENewRaceInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void cBENewRaceInterface_setNewRaceActive(int isActive) {
    (*(int *)((char*)(cBENewRaceInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewRaceInterface_cBENewRaceInterface);
#ifdef SKIP_ASM
extern char cBENewRaceInterface_vtbl[];

/**
 * Constructs the new-race interface in place: stores its vtable cBENewRaceInterface_vtbl at +0xC and returns @p self.
 *
 * The vtable's submit and discard (slots 1, 2) are cBENewRaceInterface_submit and cBENewRaceInterface_discard; the replay slots are
 * cBEInterface's.
 *
 * @return @p self
 */
extern "C" void* cBENewRaceInterface_cBENewRaceInterface(void* self)
{
    *(char**)((char*)self + 0xC) = cBENewRaceInterface_vtbl;
    return self;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewPlayerInterface_purgeInterface);
#ifdef SKIP_ASM
extern void* cBENewPlayerInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the new-player interface singleton cBENewPlayerInterface_instance (operator_delete, no destructor) and clears
 * it.
 */
extern "C" void cBENewPlayerInterface_purgeInterface(void)
{
    operator_delete((int*)cBENewPlayerInterface_instance);
    cBENewPlayerInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENewPlayerInterface_setNewPlayerActive);
#ifdef SKIP_ASM
extern void *cBENewPlayerInterface_instance;

/**
 * Sets bIsActive (+0x8) of the new-player interface singleton cBENewPlayerInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void cBENewPlayerInterface_setNewPlayerActive(int isActive) {
    (*(int *)((char*)(cBENewPlayerInterface_instance) + (8))) = isActive;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEMissionInterface_submit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the mission interface's submit (slot 1 of its vtable cBEMissionInterface_vtbl).
 */
void cBEMissionInterface_submit(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEMissionInterface_discard__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the mission interface's discard (slot 2 of its vtable cBEMissionInterface_vtbl).
 */
void cBEMissionInterface_discard(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEMissionInterface_purgeInterface);
#ifdef SKIP_ASM
extern void* cBEMissionInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the mission interface singleton cBEMissionInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void cBEMissionInterface_purgeInterface(void)
{
    operator_delete((int*)cBEMissionInterface_instance);
    cBEMissionInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEMissionInterface_isMissionSetFromFE);
#ifdef SKIP_ASM
/**
 * Returns the mission interface's word at +0x10, the flag func_0015BD00 sets (whether the mission was chosen in the
 * front end).
 *
 * @param missionInterface  the mission interface
 * @return the flag
 */
extern "C" int cBEMissionInterface_isMissionSetFromFE(void *missionInterface) {
    return (*(int *)((char*)(missionInterface) + (0x10)));
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BD00);
#ifdef SKIP_ASM
/**
 * Stores @p setFromFE in the mission interface's word at +0x10 (the flag cBEMissionInterface_isMissionSetFromFE returns).
 *
 * @param missionInterface  the mission interface
 * @param setFromFE  nonzero when the mission was chosen in the front end
 */
extern "C" void func_0015BD00(void *missionInterface, int setFromFE) {
    (*(int *)((char*)(missionInterface) + (0x10))) = setFromFE;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEMissionInterface_setMissionActive);
#ifdef SKIP_ASM
extern void *cBEMissionInterface_instance;

/**
 * Sets bIsActive (+0x8) of the mission interface singleton cBEMissionInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void cBEMissionInterface_setMissionActive(int isActive) {
    (*(int *)((char*)(cBEMissionInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEMissionInterface_cBEMissionInterface);
#ifdef SKIP_ASM
extern char cBEMissionInterface_vtbl[];

/**
 * Constructs the mission interface in place: stores its vtable cBEMissionInterface_vtbl at +0xC and returns @p self.
 *
 * The vtable holds cBEMissionInterface_submit / cBEMissionInterface_discard (submit, discard) and cBEMissionInterface_writeToReplayFrame / cBEMissionInterface_readFromReplayFrame (replay write,
 * read).
 *
 * @return @p self
 */
extern "C" void* cBEMissionInterface_cBEMissionInterface(void* self)
{
    *(char**)((char*)self + 0xC) = cBEMissionInterface_vtbl;
    return self;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_purgeInterface);
#ifdef SKIP_ASM
extern void* cBEOptionInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the option interface singleton cBEOptionInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void cBEOptionInterface_purgeInterface(void)
{
    operator_delete((int*)cBEOptionInterface_instance);
    cBEOptionInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getOptions);
#ifdef SKIP_ASM
struct sBEOpt_BE10 { int v[0xA2]; };
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Returns a copy of the option block: copies all 0x288 bytes of cBEOptionInterface_mOption to @p dst.
 *
 * @param dst  the returned option block
 * @return @p dst
 */
extern "C" sBEOpt_BE10* cBEOptionInterface_getOptions(sBEOpt_BE10* dst)
{
    *dst = cBEOptionInterface_mOption;
    return dst;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setOptions);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Replaces the option block: copies the 0x288 bytes at @p src into cBEOptionInterface_mOption (@p self is unused).
 *
 * @param src  the new options
 */
extern "C" void cBEOptionInterface_setOptions(void* self, sBEOpt_BE10* src)
{
    cBEOptionInterface_mOption = *src;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setUseMetricUnits);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the metric-units option: bit 19 of the first option word (cBEOptionInterface_mOption) = @p useMetricUnits != 0.
 *
 * @param useMetricUnits  nonzero for metric units
 */
extern "C" void cBEOptionInterface_setUseMetricUnits(void* self, int useMetricUnits)
{
    int enabled = useMetricUnits != 0;
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x80000) | (enabled << 19);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setUseWidescreen);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the widescreen option: bits 20-21 of the first option word (cBEOptionInterface_mOption) = @p widescreen & 3.
 *
 * @param widescreen  the widescreen mode, 0-3
 */
extern "C" void cBEOptionInterface_setUseWidescreen(void* self, int widescreen)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x300000) | ((widescreen & 3) << 20);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setLanguage);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the language option: bits 22-24 of the first option word (cBEOptionInterface_mOption) = @p language & 7.
 *
 * @param language  the language index, 0-7
 */
extern "C" void cBEOptionInterface_setLanguage(void* self, int language)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x1C00000) | ((language & 7) << 22);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setBrightness);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the brightness option: bits 25-28 of the first option word (cBEOptionInterface_mOption) = @p brightness & 0xF.
 *
 * @param brightness  the brightness level, 0-15
 */
extern "C" void cBEOptionInterface_setBrightness(void* self, int brightness)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x1E000000) | ((brightness & 0xF) << 25);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setReplayOption);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the replay option: bit 29 of the first option word (cBEOptionInterface_mOption) = @p replayOption & 1.
 *
 * @param replayOption  the replay setting, 0 or 1
 */
extern "C" void cBEOptionInterface_setReplayOption(void* self, int replayOption)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x20000000) | ((replayOption & 1) << 29);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getUseAutoLoad);
#ifdef SKIP_ASM
/**
 * Returns 0 (a constant: the option it would report is always off).
 *
 * @return 0
 */
extern "C" int cBEOptionInterface_getUseAutoLoad(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getLanguage);
#ifdef SKIP_ASM
/**
 * Returns the language option: bits 22-24 of the first option word (cBEOptionInterface_mOption).
 *
 * @return the language index, 0-7
 */
extern "C" int cBEOptionInterface_getLanguage(void)
{
    return ((unsigned)cBEOptionInterface_mOption.v[0] >> 22) & 7;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setSoundMode);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the sound mode: bits 12-13 of the first option word (cBEOptionInterface_mOption) = @p soundMode & 3.
 *
 * @param soundMode  the sound output mode, 0-3
 */
extern "C" void cBEOptionInterface_setSoundMode(void* self, int soundMode)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x3000) | ((soundMode & 3) << 12);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setAudioMix);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the audio mix: bits 14-15 of the first option word (cBEOptionInterface_mOption) = @p audioMix & 3.
 *
 * @param audioMix  the mix setting, 0-3
 */
extern "C" void cBEOptionInterface_setAudioMix(void* self, int audioMix)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0xC000) | ((audioMix & 3) << 14);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setRadioBigVolume);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the music (radio) volume: bits 0-3 of the first option word (cBEOptionInterface_mOption) = @p radioVolume & 0xF.
 *
 * @param radioVolume  the volume, 0-15
 */
extern "C" void cBEOptionInterface_setRadioBigVolume(void* self, int radioVolume)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0xF) | (radioVolume & 0xF);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setCharacterVolume);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the character (speech) volume: bits 4-7 of the first option word (cBEOptionInterface_mOption) =
 * @p charVolume & 0xF.
 *
 * @param charVolume  the volume, 0-15
 */
extern "C" void cBEOptionInterface_setCharacterVolume(void* self, int charVolume)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0xF0) | ((charVolume & 0xF) << 4);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setSFXVolume);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the sound-effects volume: bits 8-11 of the first option word (cBEOptionInterface_mOption) = @p sfxVolume & 0xF.
 *
 * @param sfxVolume  the volume, 0-15
 */
extern "C" void cBEOptionInterface_setSFXVolume(void* self, int sfxVolume)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0xF00) | ((sfxVolume & 0xF) << 8);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setUseArcadeSFX);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the arcade sound-effects option: bit 16 of the first option word (cBEOptionInterface_mOption) =
 * @p useArcadeSfx != 0.
 *
 * @param useArcadeSfx  nonzero to enable
 */
extern "C" void cBEOptionInterface_setUseArcadeSFX(void* self, int useArcadeSfx)
{
    int enabled = useArcadeSfx != 0;
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x10000) | (enabled << 16);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setUseDJSpeechSFX);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the DJ speech option: bit 17 of the first option word (cBEOptionInterface_mOption) = @p useDJSpeech != 0.
 *
 * @param useDJSpeech  nonzero to enable
 */
extern "C" void cBEOptionInterface_setUseDJSpeechSFX(void* self, int useDJSpeech)
{
    int enabled = useDJSpeech != 0;
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x20000) | (enabled << 17);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setUseVOIP);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the voice-chat option: bit 18 of the first option word (cBEOptionInterface_mOption) = @p useVoip != 0.
 *
 * @param useVoip  nonzero to enable
 */
extern "C" void cBEOptionInterface_setUseVOIP(void* self, int useVoip)
{
    int enabled = useVoip != 0;
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x40000) | (enabled << 18);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getSoundMode);
#ifdef SKIP_ASM
/**
 * Returns the sound mode: bits 12-13 of the first option word (cBEOptionInterface_mOption).
 *
 * @return the sound output mode, 0-3
 */
extern "C" int cBEOptionInterface_getSoundMode(void)
{
    return ((unsigned)cBEOptionInterface_mOption.v[0] >> 12) & 3;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getAudioMix);
#ifdef SKIP_ASM
/**
 * Returns the audio mix: bits 14-15 of the first option word (cBEOptionInterface_mOption).
 *
 * @return the mix setting, 0-3
 */
extern "C" int cBEOptionInterface_getAudioMix(void)
{
    return ((unsigned)cBEOptionInterface_mOption.v[0] >> 14) & 3;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getRadioBigVolume);
#ifdef SKIP_ASM
/**
 * Returns the music (radio) volume: bits 0-3 of the first option word (cBEOptionInterface_mOption).
 *
 * @return the volume, 0-15
 */
extern "C" int cBEOptionInterface_getRadioBigVolume(void)
{
    return cBEOptionInterface_mOption.v[0] & 0xF;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getCharacterVolume);
#ifdef SKIP_ASM
/**
 * Returns the character (speech) volume: bits 4-7 of the first option word (its first byte shifted right by 4).
 *
 * @return the volume, 0-15
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" unsigned long cBEOptionInterface_getCharacterVolume(void)
{
    return (unsigned long)*(unsigned char*)&cBEOptionInterface_mOption >> 4;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getSFXVolume);
#ifdef SKIP_ASM
/**
 * Returns the sound-effects volume: bits 8-11 of the first option word (cBEOptionInterface_mOption).
 *
 * @return the volume, 0-15
 */
extern "C" int cBEOptionInterface_getSFXVolume(void)
{
    return ((unsigned)cBEOptionInterface_mOption.v[0] >> 8) & 0xF;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getUseArcadeSFX);
#ifdef SKIP_ASM
extern char D_00535612[];

/**
 * Returns the arcade sound-effects option: bit 16 of the first option word, read as bit 0 of its upper halfword
 * (D_00535612 = cBEOptionInterface_mOption + 2).
 *
 * @return 1 when enabled, else 0
 */
extern "C" int cBEOptionInterface_getUseArcadeSFX(void)
{
    int upperHalf = *(short*)D_00535612;
    return upperHalf & 1;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getUseDJSpeechSFX);
#ifdef SKIP_ASM
/**
 * Returns the DJ speech option: bit 17 of the first option word (cBEOptionInterface_mOption).
 *
 * @return 1 when enabled, else 0
 */
extern "C" int cBEOptionInterface_getUseDJSpeechSFX(void)
{
    return (cBEOptionInterface_mOption.v[0] >> 17) & 1;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_getUseVOIP);
#ifdef SKIP_ASM
/**
 * Returns the voice-chat option: bit 18 of the first option word (cBEOptionInterface_mOption).
 *
 * @return 1 when enabled, else 0
 */
extern "C" int cBEOptionInterface_getUseVOIP(void)
{
    return (cBEOptionInterface_mOption.v[0] >> 18) & 1;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setQuickKeyMessage);
#ifdef SKIP_ASM
extern "C" char* strcpy(char*, const char*);
extern char D_00535617[];

/**
 * Copies @p name into quick-chat message slot @p idx: the 64-byte entries starting at cBEOptionInterface_mOption + 7.
 *
 * No length or bounds check (strcpy). @p self is unused.
 *
 * @param idx  the message slot
 * @param name  the message text
 */
extern "C" void cBEOptionInterface_setQuickKeyMessage(void* self, int idx, const char* name)
{
    strcpy(D_00535617 + (idx << 6), name);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setHUDDetail);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets bits 30-31 of the first option word (cBEOptionInterface_mOption) to @p value (shifted left by 30, not masked).
 *
 * cBEOptionInterface_defaultHUD, the HUD step of cBEOptionInterface_revert, resets the same two bits to 0.
 *
 * @param value  the new value of the top bits
 */
extern "C" void cBEOptionInterface_setHUDDetail(void* self, int value)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & 0x3FFFFFFF) | (value << 30);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setVtDelta);
#ifdef SKIP_ASM
extern char D_00535615[];

/**
 * Stores @p vertDelta as the signed byte at cBEOptionInterface_mOption + 5 (where On Tour keeps the vertical screen offset
 * iVtDelta). @p self is unused.
 *
 * @param vertDelta  the new value
 */
extern "C" void cBEOptionInterface_setVtDelta(void* self, int vertDelta) {
    *(char*)D_00535615 = vertDelta;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setHzDelta);
#ifdef SKIP_ASM
extern char D_00535616[];

/**
 * Stores @p horzDelta as the signed byte at cBEOptionInterface_mOption + 6 (where On Tour keeps the horizontal screen offset
 * iHzDelta). @p self is unused.
 *
 * @param horzDelta  the new value
 */
extern "C" void cBEOptionInterface_setHzDelta(void* self, int horzDelta) {
    *(char*)D_00535616 = horzDelta;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_setOptionActive);
#ifdef SKIP_ASM
extern void *cBEOptionInterface_instance;

/**
 * Sets bIsActive (+0x8) of the option interface singleton cBEOptionInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void cBEOptionInterface_setOptionActive(int isActive) {
    (*(int *)((char*)(cBEOptionInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEOptionInterface_cBEOptionInterface);
#ifdef SKIP_ASM
extern char cBEOptionInterface_vtbl[];

/**
 * Constructs the option interface in place: stores its vtable cBEOptionInterface_vtbl at +0xC and returns @p self.
 *
 * The vtable's submit and discard are cBEOptionInterface_submit and cBEOptionInterface_discard; the replay slots are cBEInterface's.
 *
 * @return @p self
 */
extern "C" void* cBEOptionInterface_cBEOptionInterface(void* self)
{
    *(char**)((char*)self + 0xC) = cBEOptionInterface_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBERules_reset);
#ifdef SKIP_ASM
// MEM_fill looks like memset(dst, value, size).
extern "C" void* MEM_fill(void* dst, int value, int size);
extern char D_005308B8[];

/**
 * Clears the 0x20-byte rules block D_005308B8 (MEM_fill with 0).
 *
 * The block holds two 12-byte bragging-rights records {flags, starting points, head-start seconds} and the "my rules"
 * pair {flags, AI head-start seconds} (On Tour tBERuleOptionDB, which has four records); be/beintoption's cBERules_getPlayerStartDelaySecs
 * .. cBERules_swap read and swap them. The unit's static initializer calls it.
 */
extern "C" void cBERules_reset()
{
    MEM_fill(D_005308B8, 0, 0x20);
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_purgeInterface);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Frees the network interface singleton cBENetworkInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void cBENetworkInterface_purgeInterface(void)
{
    operator_delete((int*)cBENetworkInterface_instance);
    cBENetworkInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setIsNetworkGame);
#ifdef SKIP_ASM

/**
 * Sets cBENetworkInterface_networkOpt.bIsNetworkGame (whether the race is an online one). @p self is unused.
 *
 * @param isNetworkGame  nonzero for an online game
 */
extern "C" void cBENetworkInterface_setIsNetworkGame(cBENetworkInterface* self, int isNetworkGame) {
    cBENetworkInterface_networkOpt.bIsNetworkGame = isNetworkGame;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getIsNetworkGame);
#ifdef SKIP_ASM

/**
 * Returns cBENetworkInterface_networkOpt.bIsNetworkGame (nonzero in an online game).
 *
 * @return bIsNetworkGame
 */
extern "C" int cBENetworkInterface_getIsNetworkGame(void) {
    return cBENetworkInterface_networkOpt.bIsNetworkGame;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setIsRankedGame);
#ifdef SKIP_ASM
extern char D_00534B34[];

/**
 * Sets cBENetworkInterface_networkOpt.bIsRankedGame (D_00534B34). @p self is unused.
 *
 * @param isRanked  nonzero for a ranked game
 */
extern "C" void cBENetworkInterface_setIsRankedGame(cBENetworkInterface* self, int isRanked) {
    *(int*)D_00534B34 = isRanked;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getIsRankedGame);
#ifdef SKIP_ASM
extern char D_00534B34[];

/**
 * Returns cBENetworkInterface_networkOpt.bIsRankedGame (D_00534B34).
 *
 * @return bIsRankedGame
 */
extern "C" int cBENetworkInterface_getIsRankedGame(void) {
    return *(int*)D_00534B34;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setIsHost);
#ifdef SKIP_ASM
extern char D_00534B38[];

/**
 * Sets cBENetworkInterface_networkOpt.bIsHost (D_00534B38): whether this console hosts the online game. @p self is
 * unused.
 *
 * @param isHost  nonzero when hosting
 */
extern "C" void cBENetworkInterface_setIsHost(cBENetworkInterface* self, int isHost) {
    *(int*)D_00534B38 = isHost;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getIsHost);
#ifdef SKIP_ASM
extern char D_00534B38[];

/**
 * Returns cBENetworkInterface_networkOpt.bIsHost (D_00534B38): nonzero when this console hosts the online game.
 *
 * @return bIsHost
 */
extern "C" int cBENetworkInterface_getIsHost(void) {
    return *(int*)D_00534B38;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setPort);
#ifdef SKIP_ASM
extern char D_00534B3C[];

/**
 * Sets cBENetworkInterface_networkOpt.iPort (D_00534B3C), the network port. @p self is unused.
 *
 * @param port  the port
 */
extern "C" void cBENetworkInterface_setPort(cBENetworkInterface* self, int port) {
    *(int*)D_00534B3C = port;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getPort);
#ifdef SKIP_ASM
extern char D_00534B3C[];

/**
 * Returns cBENetworkInterface_networkOpt.iPort (D_00534B3C), the network port.
 *
 * @return iPort
 */
extern "C" int cBENetworkInterface_getPort(void) {
    return *(int*)D_00534B3C;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setLatency);
#ifdef SKIP_ASM
extern char D_00534B40[];

/**
 * Sets cBENetworkInterface_networkOpt.fLatency (D_00534B40), the measured network latency.
 *
 * @param latency  the latency
 */
extern "C" void cBENetworkInterface_setLatency(float latency) {
    *(float*)D_00534B40 = latency;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getLatency);
#ifdef SKIP_ASM
extern char D_00534B40[];

/**
 * Returns cBENetworkInterface_networkOpt.fLatency (D_00534B40), the network latency.
 *
 * @return fLatency
 */
extern "C" float cBENetworkInterface_getLatency(void) {
    return *(float*)D_00534B40;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setClientID);
#ifdef SKIP_ASM
extern char D_00534D44[];

/**
 * Sets cBENetworkInterface_networkOpt.iClientID (D_00534D44). @p self is unused.
 *
 * @param clientID  the client id
 */
extern "C" void cBENetworkInterface_setClientID(cBENetworkInterface* self, int clientID) {
    *(int*)D_00534D44 = clientID;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getClientID);
#ifdef SKIP_ASM
extern char D_00534D44[];

/**
 * Returns cBENetworkInterface_networkOpt.iClientID (D_00534D44).
 *
 * @return iClientID
 */
extern "C" int cBENetworkInterface_getClientID(void) {
    return *(int*)D_00534D44;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setHostName);
#ifdef SKIP_ASM
extern "C" char* strcpy(char*, const char*);
extern char D_00534D48[];

/**
 * Copies @p name into cBENetworkInterface_networkOpt.strHostName (D_00534D48, 120 bytes; strcpy, no length check).
 *
 * @param name  the host name
 */
// Copies the caller's name string (a1) into a static buffer; the receiver (a0) is unused.
extern "C" void cBENetworkInterface_setHostName(cBENetworkInterface* self, const char* name)
{
    strcpy(D_00534D48, name);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getHostName);
#ifdef SKIP_ASM
extern char D_00534D48[];

/**
 * Returns cBENetworkInterface_networkOpt.strHostName (D_00534D48).
 *
 * @return the host name buffer
 */
extern "C" void* cBENetworkInterface_getHostName(void) {
    return D_00534D48;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setGameName);
#ifdef SKIP_ASM
extern "C" char* strcpy(char*, const char*);
extern char D_00534DC0[];

/**
 * Copies @p name into cBENetworkInterface_networkOpt.strGameName (D_00534DC0, 120 bytes; strcpy, no length check).
 *
 * @param name  the online game's name
 */
// Copies the caller's name string (a1) into a static buffer; the receiver (a0) is unused.
extern "C" void cBENetworkInterface_setGameName(cBENetworkInterface* self, const char* name)
{
    strcpy(D_00534DC0, name);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getGameName);
#ifdef SKIP_ASM
extern char D_00534DC0[];

/**
 * Returns cBENetworkInterface_networkOpt.strGameName (D_00534DC0).
 *
 * @return the game name buffer
 */
extern "C" void* cBENetworkInterface_getGameName(void) {
    return D_00534DC0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setPassword);
#ifdef SKIP_ASM
extern "C" char* strcpy(char*, const char*);
extern char D_00534E38[];

/**
 * Copies @p name into cBENetworkInterface_networkOpt.strPassword (D_00534E38, 30 bytes; strcpy, no length check).
 *
 * @param name  the game password
 */
// Copies the caller's name string (a1) into a static buffer; the receiver (a0) is unused.
extern "C" void cBENetworkInterface_setPassword(cBENetworkInterface* self, const char* name)
{
    strcpy(D_00534E38, name);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getPassword);
#ifdef SKIP_ASM
extern char D_00534E38[];

/**
 * Returns cBENetworkInterface_networkOpt.strPassword (D_00534E38).
 *
 * @return the password buffer
 */
extern "C" void* cBENetworkInterface_getPassword(void) {
    return D_00534E38;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setNetworkActive);
#ifdef SKIP_ASM

/**
 * Sets bIsActive of the network interface singleton cBENetworkInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void cBENetworkInterface_setNetworkActive(int isActive) {
    (cBENetworkInterface_instance->bIsActive) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setMyCharacterID);
#ifdef SKIP_ASM
extern char D_00534E58[];

/**
 * Sets cBENetworkInterface_networkOpt.MyCharacterID (D_00534E58): this player's character in the online race.
 * @p self is unused.
 *
 * @param characterID  the character id
 */
extern "C" void cBENetworkInterface_setMyCharacterID(cBENetworkInterface* self, int characterID) {
    *(int*)D_00534E58 = characterID;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getMyCharacterID);
#ifdef SKIP_ASM
extern char D_00534E58[];

/**
 * Returns cBENetworkInterface_networkOpt.MyCharacterID (D_00534E58).
 *
 * @return this player's character id
 */
extern "C" int cBENetworkInterface_getMyCharacterID(void) {
    return *(int*)D_00534E58;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setOpponentCharacterID);
#ifdef SKIP_ASM
extern char D_00534E5C[];

/**
 * Sets cBENetworkInterface_networkOpt.OpponentCharacterID (D_00534E5C): the remote player's character. @p self is
 * unused.
 *
 * @param characterID  the character id
 */
extern "C" void cBENetworkInterface_setOpponentCharacterID(cBENetworkInterface* self, int characterID) {
    *(int*)D_00534E5C = characterID;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getOpponentCharacterID);
#ifdef SKIP_ASM
extern char D_00534E5C[];

/**
 * Returns cBENetworkInterface_networkOpt.OpponentCharacterID (D_00534E5C).
 *
 * @return the remote player's character id
 */
extern "C" int cBENetworkInterface_getOpponentCharacterID(void) {
    return *(int*)D_00534E5C;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setMyStats);
#ifdef SKIP_ASM
struct sBEBlk_C418 { int v[0x2B]; };
extern sBEBlk_C418 D_00534E60;

/**
 * Copies the 0xAC-byte rider stats at @p src into cBENetworkInterface_networkOpt.MyStats (D_00534E60). @p self is
 * unused.
 *
 * @param src  this player's rider stats (tRiderStats)
 */
extern "C" void cBENetworkInterface_setMyStats(cBENetworkInterface* self, sBEBlk_C418* src)
{
    D_00534E60 = *src;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getMyStats);
#ifdef SKIP_ASM
extern sBEBlk_C418 D_00534E60;

/**
 * Returns the address of cBENetworkInterface_networkOpt.MyStats (D_00534E60), this player's rider stats.
 *
 * @return the stats block
 */
extern "C" sBEBlk_C418* cBENetworkInterface_getMyStats(void) {
    return &D_00534E60;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setOpponentStats);
#ifdef SKIP_ASM
extern sBEBlk_C418 D_00534F0C;

/**
 * Copies the 0xAC-byte rider stats at @p src into cBENetworkInterface_networkOpt.OpponentStats (D_00534F0C). @p self
 * is unused.
 *
 * @param src  the remote player's rider stats (tRiderStats)
 */
extern "C" void cBENetworkInterface_setOpponentStats(cBENetworkInterface* self, sBEBlk_C418* src)
{
    D_00534F0C = *src;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getOpponentStats);
#ifdef SKIP_ASM
extern sBEBlk_C418 D_00534F0C;

/**
 * Returns the address of cBENetworkInterface_networkOpt.OpponentStats (D_00534F0C), the remote player's rider stats.
 *
 * @return the stats block
 */
extern "C" sBEBlk_C418* cBENetworkInterface_getOpponentStats(void) {
    return &D_00534F0C;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C5A8);
#ifdef SKIP_ASM
extern char D_00534FC8[];

/**
 * Returns the address of the 16-byte name buffer at cBENetworkInterface_networkOpt +0x498 (D_00534FC8), the
 * opponent's name (filled by func_0014EBB8; prefixes received chat lines).
 *
 * @return the name buffer
 */
extern "C" void* func_0015C5A8(void) {
    return D_00534FC8;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C5B8);
#ifdef SKIP_ASM
extern char D_00534FB8[];

/**
 * Returns the address of the 16-byte name buffer at cBENetworkInterface_networkOpt +0x488 (D_00534FB8), this
 * player's online name (filled by func_0014EBD8; prefixes our own chat lines).
 *
 * @return the name buffer
 */
extern "C" void* func_0015C5B8(void) {
    return D_00534FB8;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_setOpponentAttributes);
#ifdef SKIP_ASM
struct sBEBlk7_C5C8 { char c[7]; };
extern unsigned char D_00534FD8[];

/**
 * Copies the 7 attribute bytes at @p src into cBENetworkInterface_networkOpt.oppCharacterAttributes (D_00534FD8).
 * @p self is unused.
 *
 * @param src  the remote rider's seven attribute levels
 */
extern "C" void cBENetworkInterface_setOpponentAttributes(cBENetworkInterface* self, sBEBlk7_C5C8* src)
{
    *(sBEBlk7_C5C8*)D_00534FD8 = *src;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getOpponentAttributes);
#ifdef SKIP_ASM
extern unsigned char D_00534FD8[];

/**
 * Returns cBENetworkInterface_networkOpt.oppCharacterAttributes (D_00534FD8), the remote rider's 7 attribute bytes.
 *
 * @return the attribute bytes
 */
extern "C" unsigned char* cBENetworkInterface_getOpponentAttributes(void) {
    return D_00534FD8;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getRankedGameCharacterLevel);
#ifdef SKIP_ASM
/**
 * Returns the constant 8.
 *
 * @return 8
 */
extern "C" int cBENetworkInterface_getRankedGameCharacterLevel(void) {
    return 8;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_getOverallOpponentLevel);
#ifdef SKIP_ASM
extern unsigned char D_00534FD8[];

/**
 * Returns the opponent's overall level: the sum of the 7 bytes of oppCharacterAttributes (D_00534FD8) divided by 35.
 *
 * @return the overall level
 */
extern "C" int cBENetworkInterface_getOverallOpponentLevel(void)
{
    int sum = 0;
    for (int i = 0; i < 7; i++) {
        sum += D_00534FD8[i];
    }
    return sum / 35;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBENetworkInterface_cBENetworkInterface);
#ifdef SKIP_ASM
extern char cBENetworkInterface_vtbl[];

/**
 * Constructs the network interface in place: stores its vtable cBENetworkInterface_vtbl at +0xC and returns @p self.
 *
 * The vtable's submit and discard are cBENetworkInterface_submit / cBENetworkInterface_discard (copy networkOpt to and from the library).
 *
 * @return @p self
 */
extern "C" void* cBENetworkInterface_cBENetworkInterface(cBENetworkInterface* self)
{
    self->mVtbl = (void**)cBENetworkInterface_vtbl;
    return ((void*)self);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEStatInterface_purgeInterface);
#ifdef SKIP_ASM
extern void* cBEStatInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the stat interface singleton cBEStatInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void cBEStatInterface_purgeInterface(void)
{
    operator_delete((int*)cBEStatInterface_instance);
    cBEStatInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEStatInterface_setStatActive);
#ifdef SKIP_ASM
extern void *cBEStatInterface_instance;

/**
 * Sets bIsActive (+0x8) of the stat interface singleton cBEStatInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void cBEStatInterface_setStatActive(int isActive) {
    (*(int *)((char*)(cBEStatInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEStatInterface_cBEStatInterface);
#ifdef SKIP_ASM
extern char cBEStatInterface_vtbl[];

/**
 * Constructs the stat interface in place: stores cBEStatInterface_vtbl at +0xC and returns @p self.
 *
 * @return @p self
 */
extern "C" void* cBEStatInterface_cBEStatInterface(void* self)
{
    *(char**)((char*)self + 0xC) = cBEStatInterface_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBECharacterInterface_submit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the character interface's submit (slot 1 of its vtable D_0045AD38).
 */
void cBECharacterInterface_submit(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBECharacterInterface_discard__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the character interface's discard (slot 2 of its vtable D_0045AD38).
 */
void cBECharacterInterface_discard(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBESaveInterface_submit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the save interface's submit (slot 1 of its vtable D_0045AD08).
 */
void cBESaveInterface_submit(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBESaveInterface_discard__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the save interface's discard (slot 2 of its vtable D_0045AD08).
 */
void cBESaveInterface_discard(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEBAGTInterface_purgeInterface);
#ifdef SKIP_ASM
extern void* cBEBAGTInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the BAGT (board and grab tricks) interface singleton cBEBAGTInterface_instance (operator_delete, no
 * destructor) and clears it.
 */
extern "C" void cBEBAGTInterface_purgeInterface(void)
{
    operator_delete((int*)cBEBAGTInterface_instance);
    cBEBAGTInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEBAGTInterface_setBAGTActive);
#ifdef SKIP_ASM
extern void *cBEBAGTInterface_instance;

/**
 * Sets bIsActive (+0x8) of the BAGT interface singleton cBEBAGTInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void cBEBAGTInterface_setBAGTActive(int isActive) {
    (*(int *)((char*)(cBEBAGTInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEBAGTInterface_cBEBAGTInterface);
#ifdef SKIP_ASM
extern char cBEBAGTInterface_vtbl[];

/**
 * Constructs the BAGT interface in place: stores its vtable cBEBAGTInterface_vtbl at +0xC and returns @p self.
 *
 * The vtable's submit and discard are cBEBAGTInterface_submit and cBEBAGTInterface_discard.
 *
 * @return @p self
 */
extern "C" void* cBEBAGTInterface_cBEBAGTInterface(void* self)
{
    *(char**)((char*)self + 0xC) = cBEBAGTInterface_vtbl;
    return self;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEScoreInterface_purgeInterface);
#ifdef SKIP_ASM
extern void* cBEScoreInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the score interface singleton cBEScoreInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void cBEScoreInterface_purgeInterface(void)
{
    operator_delete((int*)cBEScoreInterface_instance);
    cBEScoreInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEScoreInterface_isNewPersonalHighScore);
#ifdef SKIP_ASM
/**
 * Returns 1 if either profile set a new personal high score: the words at +0x10 or +0x14 are nonzero.
 *
 * cBEScoreInterface_discard clears both words.
 *
 * @param scoreInterface  the score interface
 * @return 1 or 0
 */
extern "C" int cBEScoreInterface_isNewPersonalHighScore(void *scoreInterface) {
    int var_3;

    var_3 = 0;
    if (((*(int *)((char*)(scoreInterface) + (0x10))) != 0) || ((*(int *)((char*)(scoreInterface) + (0x14))) != 0)) {
        var_3 = 1;
    }
    return var_3;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEScoreInterface_isNewTrackHighScore);
#ifdef SKIP_ASM
/**
 * Returns 1 if either profile set a new track high score: the index words at +0x18 or +0x1C are not negative.
 *
 * cBEScoreInterface_discard resets both to -1.
 *
 * @param scoreInterface  the score interface
 * @return 1 or 0
 */
extern "C" int cBEScoreInterface_isNewTrackHighScore(void *scoreInterface) {
    int var_3;

    var_3 = 0;
    if (((*(int *)((char*)(scoreInterface) + (0x18))) >= 0) || ((*(int *)((char*)(scoreInterface) + (0x1C))) >= 0)) {
        var_3 = 1;
    }
    return var_3;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEScoreInterface_GetRiderNewTrackHighScoreIndex);
#ifdef SKIP_ASM
/**
 * Returns the new track high-score index of profile @p profileIndex: the word at +0x18 + @p profileIndex * 4.
 *
 * @param scoreInterface  the score interface
 * @param profileIndex  the profile, 0 or 1
 * @return the high-score table index, or -1 when the profile set none
 */
extern "C" int cBEScoreInterface_GetRiderNewTrackHighScoreIndex(int scoreInterface, int profileIndex) {
    return (*(int *)(((char*)(scoreInterface) + (profileIndex << 2)) + (0x18)));
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEScoreInterface_setScoreActive);
#ifdef SKIP_ASM
extern void *cBEScoreInterface_instance;

/**
 * Sets bIsActive (+0x8) of the score interface singleton cBEScoreInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void cBEScoreInterface_setScoreActive(int isActive) {
    (*(int *)((char*)(cBEScoreInterface_instance) + (8))) = isActive;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEEconInterface_submit__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the economy interface's submit (slot 1 of its vtable D_0045AC78).
 */
void cBEEconInterface_submit(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEEconInterface_discard__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the economy interface's discard (slot 2 of its vtable D_0045AC78).
 */
void cBEEconInterface_discard(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEAggressionInterface_purgeInterface);
#ifdef SKIP_ASM
extern void* cBEAggressionInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the aggression interface singleton cBEAggressionInterface_instance (operator_delete, no destructor) and
 * clears it.
 */
extern "C" void cBEAggressionInterface_purgeInterface(void)
{
    operator_delete((int*)cBEAggressionInterface_instance);
    cBEAggressionInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEAggressionInterface_setAggressionActive);
#ifdef SKIP_ASM
extern void *cBEAggressionInterface_instance;

/**
 * Sets bIsActive (+0x8) of the aggression interface singleton cBEAggressionInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void cBEAggressionInterface_setAggressionActive(int isActive) {
    (*(int *)((char*)(cBEAggressionInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", cBEAggressionInterface_cBEAggressionInterface);
#ifdef SKIP_ASM
extern char cBEAggressionInterface_vtbl[];

/**
 * Constructs the aggression interface in place: stores its vtable cBEAggressionInterface_vtbl at +0xC and returns @p self.
 *
 * @return @p self
 */
extern "C" void* cBEAggressionInterface_cBEAggressionInterface(void* self)
{
    *(char**)((char*)self + 0xC) = cBEAggressionInterface_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEPreRaceState_purgeState);
#ifdef SKIP_ASM
extern int* D_004A1258;
void operator_delete(int* ptr);

/**
 * Frees the cBEPreRaceState singleton and clears its pointer.
 *
 * The object goes to operator delete without a destructor call; D_004A1258 is set to 0 so the next
 * cBEPreRaceState_getState() creates a new one.
 */
extern "C" void cBEPreRaceState_purgeState(void)
{
    operator_delete(D_004A1258);
    D_004A1258 = 0;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEPreRaceState_cBEPreRaceState);
#ifdef SKIP_ASM
extern char cBEPreRaceState_vtbl[];

/**
 * Constructs the pre-race back-end state in place: stores cBEPreRaceState_vtbl at +0x4 and returns @p self.
 *
 * @return @p self
 */
extern "C" void* cBEPreRaceState_cBEPreRaceState(void* self)
{
    *(char**)((char*)self + 4) = cBEPreRaceState_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBERaceState_purgeState);
#ifdef SKIP_ASM
extern int* cBERaceState_instance;
void operator_delete(int* ptr);

/**
 * Frees the cBERaceState singleton and clears its pointer.
 *
 * The object goes to operator delete without a destructor call; cBERaceState_instance is set to 0 so the next
 * cBERaceState_getState() creates a new one.
 */
extern "C" void cBERaceState_purgeState(void)
{
    operator_delete(cBERaceState_instance);
    cBERaceState_instance = 0;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBERaceState_cBERaceState);
#ifdef SKIP_ASM
extern char cBERaceState_vtbl[];

/**
 * Constructs the race back-end state in place: stores cBERaceState_vtbl at +0x4 and returns @p self.
 *
 * @return @p self
 */
extern "C" void* cBERaceState_cBERaceState(void* self)
{
    *(char**)((char*)self + 4) = cBERaceState_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cBEPostRaceState_purgeState);
#ifdef SKIP_ASM
extern int* D_004A1254;
void operator_delete(int* ptr);

/**
 * Frees the cBEPostRaceState singleton and clears its pointer.
 *
 * The object goes to operator delete without a destructor call; D_004A1254 is set to 0 so the next
 * cBEPostRaceState_getState() creates a new one.
 */
extern "C" void cBEPostRaceState_purgeState(void)
{
    operator_delete(D_004A1254);
    D_004A1254 = 0;
}
#endif

extern "C" void* __sti__all_in_one_be_cpp(int, int);

//100%
INCLUDE_ASM("be/bedbreward", cBEPostRaceState_cBEPostRaceState);
#ifdef SKIP_ASM
extern char cBEPostRaceState_vtbl[];

/**
 * Constructs the post-race back-end state in place: stores cBEPostRaceState_vtbl at +0x4 and returns @p self.
 *
 * @return @p self
 */
extern "C" void* cBEPostRaceState_cBEPostRaceState(void* self)
{
    *(char**)((char*)self + 4) = cBEPostRaceState_vtbl;
    return self;
}
#endif

//99.38%
INCLUDE_ASM("be/bedbreward", func_0015C8F0__FPv);
#ifdef SKIP_ASM
/**
 * Runs the unit's static initializer: __sti__all_in_one_be_cpp(1, 0xFFFF) (an entry of the global constructor list
 * D_0043CE38).
 *
 * @return what __sti__all_in_one_be_cpp leaves in the return register (unused)
 */
void* func_0015C8F0(void* self)
{
    return __sti__all_in_one_be_cpp(1, 0xffff);
}
#endif

extern "C" void* __sti__all_in_one_be_cpp(int, int);

//99.38%
INCLUDE_ASM("be/bedbreward", func_0015C910__FPv);
#ifdef SKIP_ASM
/**
 * Runs the unit's static destructors: __sti__all_in_one_be_cpp(0, 0xFFFF) (an entry of the global destructor list
 * D_0043CEE4).
 *
 * @return what __sti__all_in_one_be_cpp leaves in the return register (unused)
 */
void* func_0015C910(void* self)
{
    return __sti__all_in_one_be_cpp(0, 0xffff);
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cCameraAlgoList_cCameraAlgoList__FPv);
#ifdef SKIP_ASM
/**
 * Constructs an empty camera algorithm list: Head and Length are 0.
 *
 * @return @p self
 */
void* cCameraAlgoList_cCameraAlgoList(void* self)
{
    int initValue = 0;
    *(int*)self = initValue;
    *(int*)((char*)self + 0x4) = initValue;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", cCameraAlgoList__cCameraAlgoList);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cCameraAlgoList_removeAll(void* self);

/**
 * Destroys a camera algorithm list: frees its nodes, then the object itself when asked to.
 *
 * cCameraAlgoList_removeAll() deletes every node and the algorithms the list owns.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cCameraAlgoList__cCameraAlgoList(int* self, int flags)
{
    cCameraAlgoList_removeAll(self);
    if (flags & 1)
    {
        operator_delete(self);
    }
}
#endif

