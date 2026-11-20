#include "common.h"
#include "be/beintnetwork.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("be/bedbreward", cBERewardDB_init);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" int BXFILE_sizez(int file);
extern "C" void FILE_loadatz(int file, void* data, int size);
extern void func_0015A818_s(void* self, void* data) __asm__("func_0015A818");
extern "C" int func_0019DA10(void* self, int bank, int id, int a3, int a4, int a5);
extern int gRewardsFileName;
extern char str_Rewards_File[];

/**
 * Loads the reward database "data/be/rwrdps2.dat", at once or through the front end's asynchronous loader.
 *
 * Synchronously it reads the whole file into a "Rewards File" block, parses it (func_0015A818) and stores -1
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
        func_0015A818_s(self, data);
        *self = -1;
    }
    else
    {
        *self = func_0019DA10((char*)SSXApp->RealFE + 0x1A70, 0, gRewardsFileName, 10, async, 0);
    }
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015AE00);
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
extern "C" void func_0015AE00(sBEDB_0015AE00* self)
{
    if (self->handle >= 0)
    {
        char* snd = (char*)SSXApp->RealFE + 0x1A70;
        func_0019DC20(snd, 0, self->handle);
        func_0019D4B8(snd, 0, self->handle);
    }
    if (self->data != 0)
    {
        cMemMan_free(self->data);
    }
    self->handle = -1;
    self->data = 0;
    self->f8 = 0;
    self->fC = 0;
    self->f10 = 0;
    self->f14 = 0;
    self->f18 = 0;
    self->f1C = 0;
    self->f20 = 0;
    self->f24 = 0;
    self->f2C = 0;
    self->f30 = 0;
    self->f34 = 0;
    self->f38 = 0;
    self->f3C = 0;
    self->f40 = 0;
    self->f44 = 0;
    self->f48 = 0;
    self->f4C = 0;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015AEC0);
#ifdef SKIP_ASM
extern "C" int func_0019E238(void* self, int bank, int i);
extern "C" int func_0019E330(void* self, int bank, int i);
extern "C" void func_0015A818(int* self, int v);

/**
 * Finishes an asynchronous rewards-file load once the file has arrived.
 *
 * When the request id at +0x0 is not negative and the front end's loader (SSXApp->RealFE + 0x1A70) reports it done
 * (func_0019E238), parses the loaded data (func_0015A818 with the buffer from func_0019E330), releases the request
 * (func_0019DC20) and sets +0x0 to -1. cBERewardDB_init starts such a load when called with async set.
 */
extern "C" void func_0015AEC0(int* self)
{
    if (*self >= 0) {
        char* snd = (char*)SSXApp->RealFE + 0x1A70;
        if (func_0019E238(snd, 0, *self) != 0) {
            func_0015A818(self, func_0019E330(snd, 0, *self));
            func_0019DC20(snd, 0, *self);
            *self = -1;
        }
    }
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015AF40);
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
extern "C" int func_0015AF40(sRewardDB_AF40* self, int type, int idx)
{
    switch (type) {
    case 0: { int off = idx * 12; return *(int*)((char*)self->t0 + off); }
    case 1: { int off = idx * 12; return *(int*)((char*)self->t1 + off); }
    case 2: { int off = idx * 8; return *(int*)((char*)self->t2 + off); }
    case 3: { int off = idx * 16; return *(int*)((char*)self->t3 + off); }
    case 4: { int off = idx * 16; return *(int*)((char*)self->t4 + off); }
    case 5: { int off = idx * 16; return *(int*)((char*)self->t5 + off); }
    case 6: { int off = idx * 16; return *(int*)((char*)self->t6 + off); }
    case 7: { int off = idx * 16; return *(int*)((char*)self->t7 + off); }
    case 8: { int off = idx * 16; return *(int*)((char*)self->t8 + off); }
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
INCLUDE_ASM("be/bedbreward", func_0015BB00__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base back-end interface's submit (vtable slot 1 of cBEInterface's vtable D_0045AE88).
 */
void func_0015BB00(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015BB08__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the base back-end interface's discard (vtable slot 2 of cBEInterface's vtable D_0045AE88).
 */
void func_0015BB08(void* self)
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
INCLUDE_ASM("be/bedbreward", func_0015BB20);
#ifdef SKIP_ASM
extern void* cBENewRaceInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the new-race interface singleton cBENewRaceInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void func_0015BB20(void)
{
    operator_delete((int*)cBENewRaceInterface_instance);
    cBENewRaceInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BB48);
#ifdef SKIP_ASM
extern char D_00535BF8[];

/**
 * Returns the number of riders in the race being set up: cBENewRaceInterface_newRace.iNumberRiders.
 *
 * @return humans + mission riders + AI riders
 */
extern "C" int func_0015BB48(void) {
    return *(int*)D_00535BF8;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BB58);
#ifdef SKIP_ASM
extern char D_00535BFC[];

/**
 * Returns the number of human riders in the race being set up: cBENewRaceInterface_newRace.iNumberHumans.
 *
 * @return iNumberHumans
 */
extern "C" int func_0015BB58(void) {
    return *(int*)D_00535BFC;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BB68);
#ifdef SKIP_ASM
extern char D_00535C04[];

/**
 * Returns the number of AI riders in the race being set up: cBENewRaceInterface_newRace.iNumberAI.
 *
 * @return iNumberAI
 */
extern "C" int func_0015BB68(void) {
    return *(int*)D_00535C04;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BB78);
#ifdef SKIP_ASM
extern char D_00535C00[];

/**
 * Returns the number of mission riders in the race being set up: cBENewRaceInterface_newRace.iNumberMissionRiders.
 *
 * @return iNumberMissionRiders
 */
extern "C" int func_0015BB78(void) {
    return *(int*)D_00535C00;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BB88);
#ifdef SKIP_ASM
extern char D_00535C0C[];

/**
 * Sets cBENewRaceInterface_newRace.iRaceTime to @p v (@p self is unused).
 *
 * @param v  the race time
 */
extern "C" void func_0015BB88(void* self, int v) {
    *(int*)D_00535C0C = v;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BB98);
#ifdef SKIP_ASM
extern char D_00535C0C[];

/**
 * Returns cBENewRaceInterface_newRace.iRaceTime (set by func_0015BB88).
 *
 * @return the race time
 */
extern "C" int func_0015BB98(void) {
    return *(int*)D_00535C0C;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BBA8);
#ifdef SKIP_ASM
extern char D_00535C10[];

/**
 * Returns the race's game mode: the signed byte cBENewRaceInterface_newRace.cGameMode (set by
 * cBENewRaceInterface_setGameMode).
 *
 * @return cGameMode, sign-extended
 */
extern "C" int func_0015BBA8(void) {
    return *(signed char*)D_00535C10;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BBB8);
#ifdef SKIP_ASM
extern char D_00535C12[];

/**
 * Returns the race's game event: the signed byte cBENewRaceInterface_newRace.cGameEvent (set by
 * cBENewRaceInterface_setGameEvent).
 *
 * @return cGameEvent, sign-extended
 */
extern "C" int func_0015BBB8(void) {
    return *(signed char*)D_00535C12;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BBC8);
#ifdef SKIP_ASM
extern char D_00535C11[];

/**
 * Returns the race's game path: the signed byte cBENewRaceInterface_newRace.cGamePath (0, 1, 2 = online; set by
 * cBENewRaceInterface_setGamePath).
 *
 * @return cGamePath, sign-extended
 */
extern "C" int func_0015BBC8(void) {
    return *(signed char*)D_00535C11;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BBD8);
#ifdef SKIP_ASM
extern char D_00535C08[];
extern int D_004A11B8;

/**
 * Sets the race's location: cBENewRaceInterface_newRace.iLocationIdx = @p v, and sets the flag D_004A11B8 to 1.
 *
 * D_004A11B8 is read by func_0015BC00 and cleared by func_0015BC08 (it sits where On Tour keeps
 * cBENewRaceInterface::bIsBamLocation). @p self is unused.
 *
 * @param v  the location index
 */
extern "C" void func_0015BBD8(void* self, int v)
{
    *(int*)D_00535C08 = v;
    D_004A11B8 = 1;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BBF0);
#ifdef SKIP_ASM
extern int cBENewRaceInterface_mNextLocation;

/**
 * Stores @p nextLocation in cBENewRaceInterface_mNextLocation (@p self is unused).
 *
 * @param nextLocation  the location to go to next
 */
extern "C" void func_0015BBF0(void* self, int nextLocation) {
    cBENewRaceInterface_mNextLocation = nextLocation;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BBF8);
#ifdef SKIP_ASM
extern int cBENewRaceInterface_mNextLocation;

/**
 * Returns cBENewRaceInterface_mNextLocation (set by func_0015BBF0).
 *
 * @return the next location
 */
extern "C" int func_0015BBF8(void) {
    return cBENewRaceInterface_mNextLocation;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BC00);
#ifdef SKIP_ASM
extern int D_004A11B8;

/**
 * Returns the flag D_004A11B8: set by func_0015BBD8 when a location is chosen, cleared by func_0015BC08.
 *
 * @return the flag
 */
extern "C" int func_0015BC00(void) {
    return D_004A11B8;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BC08);
#ifdef SKIP_ASM
extern int D_004A11B8;

/**
 * Clears the flag D_004A11B8 (see func_0015BBD8 and func_0015BC00).
 */
extern "C" void func_0015BC08(void) {
    D_004A11B8 = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BC10);
#ifdef SKIP_ASM
extern "C" void cBXString_cBXString4(int *);
extern int D_004A4CB8;

/**
 * Re-initialises the static string D_004A4CB8 from the C string the caller passes as its second argument.
 *
 * Calls cBXString_cBXString4(&D_004A4CB8, str), with str left in the second argument register by the caller (the
 * matched body declares no parameters). The string's previous buffer is not released. func_0015BC30 returns a copy.
 */
extern "C" void func_0015BC10(void) {
    cBXString_cBXString4(&D_004A4CB8);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BC30);
#ifdef SKIP_ASM
extern "C" void cBXString_cBXString1(void*, int*);

/**
 * Returns a copy of the static string D_004A4CB8 (set by func_0015BC10): copy-constructs @p self from it.
 *
 * @return @p self, the returned cBXString
 */
extern "C" void* func_0015BC30(void* self)
{
    cBXString_cBXString1(self, &D_004A4CB8);
    return self;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BC60);
#ifdef SKIP_ASM
extern void *cBENewRaceInterface_instance;

/**
 * Sets bIsActive (+0x8) of the new-race interface singleton cBENewRaceInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void func_0015BC60(int isActive) {
    (*(int *)((char*)(cBENewRaceInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BC70);
#ifdef SKIP_ASM
extern char D_0045AE58[];

/**
 * Constructs the new-race interface in place: stores its vtable D_0045AE58 at +0xC and returns @p self.
 *
 * The vtable's submit and discard (slots 1, 2) are func_00144928 and func_001449E8; the replay slots are
 * cBEInterface's.
 *
 * @return @p self
 */
extern "C" void* func_0015BC70(void* self)
{
    *(char**)((char*)self + 0xC) = D_0045AE58;
    return self;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BC88);
#ifdef SKIP_ASM
extern void* cBENewPlayerInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the new-player interface singleton cBENewPlayerInterface_instance (operator_delete, no destructor) and clears
 * it.
 */
extern "C" void func_0015BC88(void)
{
    operator_delete((int*)cBENewPlayerInterface_instance);
    cBENewPlayerInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BCB0);
#ifdef SKIP_ASM
extern void *cBENewPlayerInterface_instance;

/**
 * Sets bIsActive (+0x8) of the new-player interface singleton cBENewPlayerInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void func_0015BCB0(int isActive) {
    (*(int *)((char*)(cBENewPlayerInterface_instance) + (8))) = isActive;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015BCC0__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the mission interface's submit (slot 1 of its vtable D_0045ADF8).
 */
void func_0015BCC0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015BCC8__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the mission interface's discard (slot 2 of its vtable D_0045ADF8).
 */
void func_0015BCC8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015BCD0);
#ifdef SKIP_ASM
extern void* cBEMissionInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the mission interface singleton cBEMissionInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void func_0015BCD0(void)
{
    operator_delete((int*)cBEMissionInterface_instance);
    cBEMissionInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BCF8);
#ifdef SKIP_ASM
/**
 * Returns the mission interface's word at +0x10, the flag func_0015BD00 sets (whether the mission was chosen in the
 * front end).
 *
 * @param missionInterface  the mission interface
 * @return the flag
 */
extern "C" int func_0015BCF8(void *missionInterface) {
    return (*(int *)((char*)(missionInterface) + (0x10)));
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BD00);
#ifdef SKIP_ASM
/**
 * Stores @p setFromFE in the mission interface's word at +0x10 (the flag func_0015BCF8 returns).
 *
 * @param missionInterface  the mission interface
 * @param setFromFE  nonzero when the mission was chosen in the front end
 */
extern "C" void func_0015BD00(void *missionInterface, int setFromFE) {
    (*(int *)((char*)(missionInterface) + (0x10))) = setFromFE;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BD08);
#ifdef SKIP_ASM
extern void *cBEMissionInterface_instance;

/**
 * Sets bIsActive (+0x8) of the mission interface singleton cBEMissionInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void func_0015BD08(int isActive) {
    (*(int *)((char*)(cBEMissionInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BD18);
#ifdef SKIP_ASM
extern char D_0045ADF8[];

/**
 * Constructs the mission interface in place: stores its vtable D_0045ADF8 at +0xC and returns @p self.
 *
 * The vtable holds func_0015BCC0 / func_0015BCC8 (submit, discard) and func_001547E8 / func_00154898 (replay write,
 * read).
 *
 * @return @p self
 */
extern "C" void* func_0015BD18(void* self)
{
    *(char**)((char*)self + 0xC) = D_0045ADF8;
    return self;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BD30);
#ifdef SKIP_ASM
extern void* cBEOptionInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the option interface singleton cBEOptionInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void func_0015BD30(void)
{
    operator_delete((int*)cBEOptionInterface_instance);
    cBEOptionInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BD58);
#ifdef SKIP_ASM
struct sBEOpt_BE10 { int v[0xA2]; };
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Returns a copy of the option block: copies all 0x288 bytes of cBEOptionInterface_mOption to @p dst.
 *
 * @param dst  the returned option block
 * @return @p dst
 */
extern "C" sBEOpt_BE10* func_0015BD58(sBEOpt_BE10* dst)
{
    *dst = cBEOptionInterface_mOption;
    return dst;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BE10);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Replaces the option block: copies the 0x288 bytes at @p src into cBEOptionInterface_mOption (@p self is unused).
 *
 * @param src  the new options
 */
extern "C" void func_0015BE10(void* self, sBEOpt_BE10* src)
{
    cBEOptionInterface_mOption = *src;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BEC0);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the metric-units option: bit 19 of the first option word (cBEOptionInterface_mOption) = @p useMetricUnits != 0.
 *
 * @param useMetricUnits  nonzero for metric units
 */
extern "C" void func_0015BEC0(void* self, int useMetricUnits)
{
    int enabled = useMetricUnits != 0;
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x80000) | (enabled << 19);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BEE8);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the widescreen option: bits 20-21 of the first option word (cBEOptionInterface_mOption) = @p widescreen & 3.
 *
 * @param widescreen  the widescreen mode, 0-3
 */
extern "C" void func_0015BEE8(void* self, int widescreen)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x300000) | ((widescreen & 3) << 20);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BF10);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the language option: bits 22-24 of the first option word (cBEOptionInterface_mOption) = @p language & 7.
 *
 * @param language  the language index, 0-7
 */
extern "C" void func_0015BF10(void* self, int language)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x1C00000) | ((language & 7) << 22);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BF38);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the brightness option: bits 25-28 of the first option word (cBEOptionInterface_mOption) = @p brightness & 0xF.
 *
 * @param brightness  the brightness level, 0-15
 */
extern "C" void func_0015BF38(void* self, int brightness)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x1E000000) | ((brightness & 0xF) << 25);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BF60);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the replay option: bit 29 of the first option word (cBEOptionInterface_mOption) = @p replayOption & 1.
 *
 * @param replayOption  the replay setting, 0 or 1
 */
extern "C" void func_0015BF60(void* self, int replayOption)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x20000000) | ((replayOption & 1) << 29);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BF88);
#ifdef SKIP_ASM
/**
 * Returns 0 (a constant: the option it would report is always off).
 *
 * @return 0
 */
extern "C" int func_0015BF88(void) {
    return 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BF90);
#ifdef SKIP_ASM
/**
 * Returns the language option: bits 22-24 of the first option word (cBEOptionInterface_mOption).
 *
 * @return the language index, 0-7
 */
extern "C" int func_0015BF90(void)
{
    return ((unsigned)cBEOptionInterface_mOption.v[0] >> 22) & 7;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BFA8);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the sound mode: bits 12-13 of the first option word (cBEOptionInterface_mOption) = @p soundMode & 3.
 *
 * @param soundMode  the sound output mode, 0-3
 */
extern "C" void func_0015BFA8(void* self, int soundMode)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x3000) | ((soundMode & 3) << 12);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BFD0);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the audio mix: bits 14-15 of the first option word (cBEOptionInterface_mOption) = @p audioMix & 3.
 *
 * @param audioMix  the mix setting, 0-3
 */
extern "C" void func_0015BFD0(void* self, int audioMix)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0xC000) | ((audioMix & 3) << 14);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015BFF8);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the music (radio) volume: bits 0-3 of the first option word (cBEOptionInterface_mOption) = @p radioVolume & 0xF.
 *
 * @param radioVolume  the volume, 0-15
 */
extern "C" void func_0015BFF8(void* self, int radioVolume)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0xF) | (radioVolume & 0xF);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C018);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the character (speech) volume: bits 4-7 of the first option word (cBEOptionInterface_mOption) =
 * @p charVolume & 0xF.
 *
 * @param charVolume  the volume, 0-15
 */
extern "C" void func_0015C018(void* self, int charVolume)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0xF0) | ((charVolume & 0xF) << 4);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C040);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the sound-effects volume: bits 8-11 of the first option word (cBEOptionInterface_mOption) = @p sfxVolume & 0xF.
 *
 * @param sfxVolume  the volume, 0-15
 */
extern "C" void func_0015C040(void* self, int sfxVolume)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0xF00) | ((sfxVolume & 0xF) << 8);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C068);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the arcade sound-effects option: bit 16 of the first option word (cBEOptionInterface_mOption) =
 * @p useArcadeSfx != 0.
 *
 * @param useArcadeSfx  nonzero to enable
 */
extern "C" void func_0015C068(void* self, int useArcadeSfx)
{
    int enabled = useArcadeSfx != 0;
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x10000) | (enabled << 16);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C090);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the DJ speech option: bit 17 of the first option word (cBEOptionInterface_mOption) = @p useDJSpeech != 0.
 *
 * @param useDJSpeech  nonzero to enable
 */
extern "C" void func_0015C090(void* self, int useDJSpeech)
{
    int enabled = useDJSpeech != 0;
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x20000) | (enabled << 17);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C0B8);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets the voice-chat option: bit 18 of the first option word (cBEOptionInterface_mOption) = @p useVoip != 0.
 *
 * @param useVoip  nonzero to enable
 */
extern "C" void func_0015C0B8(void* self, int useVoip)
{
    int enabled = useVoip != 0;
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & ~0x40000) | (enabled << 18);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C0E0);
#ifdef SKIP_ASM
/**
 * Returns the sound mode: bits 12-13 of the first option word (cBEOptionInterface_mOption).
 *
 * @return the sound output mode, 0-3
 */
extern "C" int func_0015C0E0(void)
{
    return ((unsigned)cBEOptionInterface_mOption.v[0] >> 12) & 3;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C0F8);
#ifdef SKIP_ASM
/**
 * Returns the audio mix: bits 14-15 of the first option word (cBEOptionInterface_mOption).
 *
 * @return the mix setting, 0-3
 */
extern "C" int func_0015C0F8(void)
{
    return ((unsigned)cBEOptionInterface_mOption.v[0] >> 14) & 3;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C110);
#ifdef SKIP_ASM
/**
 * Returns the music (radio) volume: bits 0-3 of the first option word (cBEOptionInterface_mOption).
 *
 * @return the volume, 0-15
 */
extern "C" int func_0015C110(void)
{
    return cBEOptionInterface_mOption.v[0] & 0xF;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C120);
#ifdef SKIP_ASM
/**
 * Returns the character (speech) volume: bits 4-7 of the first option word (its first byte shifted right by 4).
 *
 * @return the volume, 0-15
 */
// PORT: 64-bit `long` (8 bytes on EE, 4 on Windows); use int64_t/uint64_t off-PS2.
extern "C" unsigned long func_0015C120(void)
{
    return (unsigned long)*(unsigned char*)&cBEOptionInterface_mOption >> 4;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C130);
#ifdef SKIP_ASM
/**
 * Returns the sound-effects volume: bits 8-11 of the first option word (cBEOptionInterface_mOption).
 *
 * @return the volume, 0-15
 */
extern "C" int func_0015C130(void)
{
    return ((unsigned)cBEOptionInterface_mOption.v[0] >> 8) & 0xF;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C148);
#ifdef SKIP_ASM
extern char D_00535612[];

/**
 * Returns the arcade sound-effects option: bit 16 of the first option word, read as bit 0 of its upper halfword
 * (D_00535612 = cBEOptionInterface_mOption + 2).
 *
 * @return 1 when enabled, else 0
 */
extern "C" int func_0015C148(void)
{
    int v = *(short*)D_00535612;
    return v & 1;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C158);
#ifdef SKIP_ASM
/**
 * Returns the DJ speech option: bit 17 of the first option word (cBEOptionInterface_mOption).
 *
 * @return 1 when enabled, else 0
 */
extern "C" int func_0015C158(void)
{
    return (cBEOptionInterface_mOption.v[0] >> 17) & 1;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C170);
#ifdef SKIP_ASM
/**
 * Returns the voice-chat option: bit 18 of the first option word (cBEOptionInterface_mOption).
 *
 * @return 1 when enabled, else 0
 */
extern "C" int func_0015C170(void)
{
    return (cBEOptionInterface_mOption.v[0] >> 18) & 1;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C188);
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
extern "C" void func_0015C188(void* self, int idx, const char* name)
{
    strcpy(D_00535617 + (idx << 6), name);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C1B8);
#ifdef SKIP_ASM
extern sBEOpt_BE10 cBEOptionInterface_mOption;

/**
 * Sets bits 30-31 of the first option word (cBEOptionInterface_mOption) to @p value (shifted left by 30, not masked).
 *
 * func_0014F600, the HUD step of cBEOptionInterface_revert, resets the same two bits to 0.
 *
 * @param value  the new value of the top bits
 */
extern "C" void func_0015C1B8(void* self, int value)
{
    cBEOptionInterface_mOption.v[0] = (cBEOptionInterface_mOption.v[0] & 0x3FFFFFFF) | (value << 30);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C1E0);
#ifdef SKIP_ASM
extern char D_00535615[];

/**
 * Stores @p v as the signed byte at cBEOptionInterface_mOption + 5 (where On Tour keeps the vertical screen offset
 * iVtDelta). @p self is unused.
 *
 * @param v  the new value
 */
extern "C" void func_0015C1E0(void* self, int v) {
    *(char*)D_00535615 = v;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C1F0);
#ifdef SKIP_ASM
extern char D_00535616[];

/**
 * Stores @p v as the signed byte at cBEOptionInterface_mOption + 6 (where On Tour keeps the horizontal screen offset
 * iHzDelta). @p self is unused.
 *
 * @param v  the new value
 */
extern "C" void func_0015C1F0(void* self, int v) {
    *(char*)D_00535616 = v;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C200);
#ifdef SKIP_ASM
extern void *cBEOptionInterface_instance;

/**
 * Sets bIsActive (+0x8) of the option interface singleton cBEOptionInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void func_0015C200(int isActive) {
    (*(int *)((char*)(cBEOptionInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C210);
#ifdef SKIP_ASM
extern char D_0045ADC8[];

/**
 * Constructs the option interface in place: stores its vtable D_0045ADC8 at +0xC and returns @p self.
 *
 * The vtable's submit and discard are func_0014F2A8 and func_0014F360; the replay slots are cBEInterface's.
 *
 * @return @p self
 */
extern "C" void* func_0015C210(void* self)
{
    *(char**)((char*)self + 0xC) = D_0045ADC8;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C228);
#ifdef SKIP_ASM
// MEM_fill looks like memset(dst, value, size).
extern "C" void* MEM_fill(void* dst, int value, int size);
extern char D_005308B8[];

/**
 * Clears the 0x20-byte rules block D_005308B8 (MEM_fill with 0).
 *
 * The block holds two 12-byte bragging-rights records {flags, starting points, head-start seconds} and the "my rules"
 * pair {flags, AI head-start seconds} (On Tour tBERuleOptionDB, which has four records); be/beintoption's func_0014F7A8
 * .. func_0014F898 read and swap them. The unit's static initializer calls it.
 */
extern "C" void func_0015C228()
{
    MEM_fill(D_005308B8, 0, 0x20);
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C250);
#ifdef SKIP_ASM
void operator_delete(int* ptr);

/**
 * Frees the network interface singleton cBENetworkInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void func_0015C250(void)
{
    operator_delete((int*)cBENetworkInterface_instance);
    cBENetworkInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C278);
#ifdef SKIP_ASM

/**
 * Sets cBENetworkInterface_networkOpt.bIsNetworkGame (whether the race is an online one). @p self is unused.
 *
 * @param isNetworkGame  nonzero for an online game
 */
extern "C" void func_0015C278(void* self, int isNetworkGame) {
    cBENetworkInterface_networkOpt.bIsNetworkGame = isNetworkGame;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C288);
#ifdef SKIP_ASM

/**
 * Returns cBENetworkInterface_networkOpt.bIsNetworkGame (nonzero in an online game).
 *
 * @return bIsNetworkGame
 */
extern "C" int func_0015C288(void) {
    return cBENetworkInterface_networkOpt.bIsNetworkGame;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C298);
#ifdef SKIP_ASM
extern char D_00534B34[];

/**
 * Sets cBENetworkInterface_networkOpt.bIsRankedGame (D_00534B34). @p self is unused.
 *
 * @param v  nonzero for a ranked game
 */
extern "C" void func_0015C298(void* self, int v) {
    *(int*)D_00534B34 = v;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C2A8);
#ifdef SKIP_ASM
extern char D_00534B34[];

/**
 * Returns cBENetworkInterface_networkOpt.bIsRankedGame (D_00534B34).
 *
 * @return bIsRankedGame
 */
extern "C" int func_0015C2A8(void) {
    return *(int*)D_00534B34;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C2B8);
#ifdef SKIP_ASM
extern char D_00534B38[];

/**
 * Sets cBENetworkInterface_networkOpt.bIsHost (D_00534B38): whether this console hosts the online game. @p self is
 * unused.
 *
 * @param v  nonzero when hosting
 */
extern "C" void func_0015C2B8(void* self, int v) {
    *(int*)D_00534B38 = v;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C2C8);
#ifdef SKIP_ASM
extern char D_00534B38[];

/**
 * Returns cBENetworkInterface_networkOpt.bIsHost (D_00534B38): nonzero when this console hosts the online game.
 *
 * @return bIsHost
 */
extern "C" int func_0015C2C8(void) {
    return *(int*)D_00534B38;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C2D8);
#ifdef SKIP_ASM
extern char D_00534B3C[];

/**
 * Sets cBENetworkInterface_networkOpt.iPort (D_00534B3C), the network port. @p self is unused.
 *
 * @param v  the port
 */
extern "C" void func_0015C2D8(void* self, int v) {
    *(int*)D_00534B3C = v;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C2E8);
#ifdef SKIP_ASM
extern char D_00534B3C[];

/**
 * Returns cBENetworkInterface_networkOpt.iPort (D_00534B3C), the network port.
 *
 * @return iPort
 */
extern "C" int func_0015C2E8(void) {
    return *(int*)D_00534B3C;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C2F8);
#ifdef SKIP_ASM
extern char D_00534B40[];

/**
 * Sets cBENetworkInterface_networkOpt.fLatency (D_00534B40), the measured network latency.
 *
 * @param v  the latency
 */
extern "C" void func_0015C2F8(float v) {
    *(float*)D_00534B40 = v;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C308);
#ifdef SKIP_ASM
extern char D_00534B40[];

/**
 * Returns cBENetworkInterface_networkOpt.fLatency (D_00534B40), the network latency.
 *
 * @return fLatency
 */
extern "C" float func_0015C308(void) {
    return *(float*)D_00534B40;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C318);
#ifdef SKIP_ASM
extern char D_00534D44[];

/**
 * Sets cBENetworkInterface_networkOpt.iClientID (D_00534D44). @p self is unused.
 *
 * @param v  the client id
 */
extern "C" void func_0015C318(void* self, int v) {
    *(int*)D_00534D44 = v;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C328);
#ifdef SKIP_ASM
extern char D_00534D44[];

/**
 * Returns cBENetworkInterface_networkOpt.iClientID (D_00534D44).
 *
 * @return iClientID
 */
extern "C" int func_0015C328(void) {
    return *(int*)D_00534D44;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C338);
#ifdef SKIP_ASM
extern "C" char* strcpy(char*, const char*);
extern char D_00534D48[];

/**
 * Copies @p name into cBENetworkInterface_networkOpt.strHostName (D_00534D48, 120 bytes; strcpy, no length check).
 *
 * @param name  the host name
 */
// Copies the caller's name string (a1) into a static buffer; the receiver (a0) is unused.
extern "C" void func_0015C338(void* self, const char* name)
{
    strcpy(D_00534D48, name);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C358);
#ifdef SKIP_ASM
extern char D_00534D48[];

/**
 * Returns cBENetworkInterface_networkOpt.strHostName (D_00534D48).
 *
 * @return the host name buffer
 */
extern "C" void* func_0015C358(void) {
    return D_00534D48;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C368);
#ifdef SKIP_ASM
extern "C" char* strcpy(char*, const char*);
extern char D_00534DC0[];

/**
 * Copies @p name into cBENetworkInterface_networkOpt.strGameName (D_00534DC0, 120 bytes; strcpy, no length check).
 *
 * @param name  the online game's name
 */
// Copies the caller's name string (a1) into a static buffer; the receiver (a0) is unused.
extern "C" void func_0015C368(void* self, const char* name)
{
    strcpy(D_00534DC0, name);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C388);
#ifdef SKIP_ASM
extern char D_00534DC0[];

/**
 * Returns cBENetworkInterface_networkOpt.strGameName (D_00534DC0).
 *
 * @return the game name buffer
 */
extern "C" void* func_0015C388(void) {
    return D_00534DC0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C398);
#ifdef SKIP_ASM
extern "C" char* strcpy(char*, const char*);
extern char D_00534E38[];

/**
 * Copies @p name into cBENetworkInterface_networkOpt.strPassword (D_00534E38, 30 bytes; strcpy, no length check).
 *
 * @param name  the game password
 */
// Copies the caller's name string (a1) into a static buffer; the receiver (a0) is unused.
extern "C" void func_0015C398(void* self, const char* name)
{
    strcpy(D_00534E38, name);
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C3B8);
#ifdef SKIP_ASM
extern char D_00534E38[];

/**
 * Returns cBENetworkInterface_networkOpt.strPassword (D_00534E38).
 *
 * @return the password buffer
 */
extern "C" void* func_0015C3B8(void) {
    return D_00534E38;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C3C8);
#ifdef SKIP_ASM

/**
 * Sets bIsActive of the network interface singleton cBENetworkInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void func_0015C3C8(int isActive) {
    (cBENetworkInterface_instance->bIsActive) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C3D8);
#ifdef SKIP_ASM
extern char D_00534E58[];

/**
 * Sets cBENetworkInterface_networkOpt.MyCharacterID (D_00534E58): this player's character in the online race.
 * @p self is unused.
 *
 * @param v  the character id
 */
extern "C" void func_0015C3D8(void* self, int v) {
    *(int*)D_00534E58 = v;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C3E8);
#ifdef SKIP_ASM
extern char D_00534E58[];

/**
 * Returns cBENetworkInterface_networkOpt.MyCharacterID (D_00534E58).
 *
 * @return this player's character id
 */
extern "C" int func_0015C3E8(void) {
    return *(int*)D_00534E58;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C3F8);
#ifdef SKIP_ASM
extern char D_00534E5C[];

/**
 * Sets cBENetworkInterface_networkOpt.OpponentCharacterID (D_00534E5C): the remote player's character. @p self is
 * unused.
 *
 * @param v  the character id
 */
extern "C" void func_0015C3F8(void* self, int v) {
    *(int*)D_00534E5C = v;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C408);
#ifdef SKIP_ASM
extern char D_00534E5C[];

/**
 * Returns cBENetworkInterface_networkOpt.OpponentCharacterID (D_00534E5C).
 *
 * @return the remote player's character id
 */
extern "C" int func_0015C408(void) {
    return *(int*)D_00534E5C;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C418);
#ifdef SKIP_ASM
struct sBEBlk_C418 { int v[0x2B]; };
extern sBEBlk_C418 D_00534E60;

/**
 * Copies the 0xAC-byte rider stats at @p src into cBENetworkInterface_networkOpt.MyStats (D_00534E60). @p self is
 * unused.
 *
 * @param src  this player's rider stats (tRiderStats)
 */
extern "C" void func_0015C418(void* self, sBEBlk_C418* src)
{
    D_00534E60 = *src;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C4D0);
#ifdef SKIP_ASM
extern sBEBlk_C418 D_00534E60;

/**
 * Returns the address of cBENetworkInterface_networkOpt.MyStats (D_00534E60), this player's rider stats.
 *
 * @return the stats block
 */
extern "C" sBEBlk_C418* func_0015C4D0(void) {
    return &D_00534E60;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C4E0);
#ifdef SKIP_ASM
extern sBEBlk_C418 D_00534F0C;

/**
 * Copies the 0xAC-byte rider stats at @p src into cBENetworkInterface_networkOpt.OpponentStats (D_00534F0C). @p self
 * is unused.
 *
 * @param src  the remote player's rider stats (tRiderStats)
 */
extern "C" void func_0015C4E0(void* self, sBEBlk_C418* src)
{
    D_00534F0C = *src;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C598);
#ifdef SKIP_ASM
extern sBEBlk_C418 D_00534F0C;

/**
 * Returns the address of cBENetworkInterface_networkOpt.OpponentStats (D_00534F0C), the remote player's rider stats.
 *
 * @return the stats block
 */
extern "C" sBEBlk_C418* func_0015C598(void) {
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
INCLUDE_ASM("be/bedbreward", func_0015C5C8);
#ifdef SKIP_ASM
struct sBEBlk7_C5C8 { char c[7]; };
extern unsigned char D_00534FD8[];

/**
 * Copies the 7 attribute bytes at @p src into cBENetworkInterface_networkOpt.oppCharacterAttributes (D_00534FD8).
 * @p self is unused.
 *
 * @param src  the remote rider's seven attribute levels
 */
extern "C" void func_0015C5C8(void* self, sBEBlk7_C5C8* src)
{
    *(sBEBlk7_C5C8*)D_00534FD8 = *src;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C600);
#ifdef SKIP_ASM
extern unsigned char D_00534FD8[];

/**
 * Returns cBENetworkInterface_networkOpt.oppCharacterAttributes (D_00534FD8), the remote rider's 7 attribute bytes.
 *
 * @return the attribute bytes
 */
extern "C" unsigned char* func_0015C600(void) {
    return D_00534FD8;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C610);
#ifdef SKIP_ASM
/**
 * Returns the constant 8.
 *
 * @return 8
 */
extern "C" int func_0015C610(void) {
    return 8;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C618);
#ifdef SKIP_ASM
extern unsigned char D_00534FD8[];

/**
 * Returns the opponent's overall level: the sum of the 7 bytes of oppCharacterAttributes (D_00534FD8) divided by 35.
 *
 * @return the overall level
 */
extern "C" int func_0015C618(void)
{
    int sum = 0;
    for (int i = 0; i < 7; i++) {
        sum += D_00534FD8[i];
    }
    return sum / 35;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C660);
#ifdef SKIP_ASM
extern char D_0045AD98[];

/**
 * Constructs the network interface in place: stores its vtable D_0045AD98 at +0xC and returns @p self.
 *
 * The vtable's submit and discard are func_0014E130 / func_0014E1F8 (copy networkOpt to and from the library).
 *
 * @return @p self
 */
extern "C" void* func_0015C660(void* self)
{
    *(char**)((char*)self + 0xC) = D_0045AD98;
    return self;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C678);
#ifdef SKIP_ASM
extern void* cBEStatInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the stat interface singleton cBEStatInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void func_0015C678(void)
{
    operator_delete((int*)cBEStatInterface_instance);
    cBEStatInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C6A0);
#ifdef SKIP_ASM
extern void *cBEStatInterface_instance;

/**
 * Sets bIsActive (+0x8) of the stat interface singleton cBEStatInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void func_0015C6A0(int isActive) {
    (*(int *)((char*)(cBEStatInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C6B0);
#ifdef SKIP_ASM
extern char cBEStatInterface_vtbl[];

/**
 * Constructs the stat interface in place: stores cBEStatInterface_vtbl at +0xC and returns @p self.
 *
 * @return @p self
 */
extern "C" void* func_0015C6B0(void* self)
{
    *(char**)((char*)self + 0xC) = cBEStatInterface_vtbl;
    return self;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C6C8__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the character interface's submit (slot 1 of its vtable D_0045AD38).
 */
void func_0015C6C8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C6D0__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the character interface's discard (slot 2 of its vtable D_0045AD38).
 */
void func_0015C6D0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C6D8__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the save interface's submit (slot 1 of its vtable D_0045AD08).
 */
void func_0015C6D8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C6E0__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the save interface's discard (slot 2 of its vtable D_0045AD08).
 */
void func_0015C6E0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C6E8);
#ifdef SKIP_ASM
extern void* cBEBAGTInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the BAGT (board and grab tricks) interface singleton cBEBAGTInterface_instance (operator_delete, no
 * destructor) and clears it.
 */
extern "C" void func_0015C6E8(void)
{
    operator_delete((int*)cBEBAGTInterface_instance);
    cBEBAGTInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C710);
#ifdef SKIP_ASM
extern void *cBEBAGTInterface_instance;

/**
 * Sets bIsActive (+0x8) of the BAGT interface singleton cBEBAGTInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void func_0015C710(int isActive) {
    (*(int *)((char*)(cBEBAGTInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C720);
#ifdef SKIP_ASM
extern char D_0045ACD8[];

/**
 * Constructs the BAGT interface in place: stores its vtable D_0045ACD8 at +0xC and returns @p self.
 *
 * The vtable's submit and discard are func_0014F960 and func_0014FAD0.
 *
 * @return @p self
 */
extern "C" void* func_0015C720(void* self)
{
    *(char**)((char*)self + 0xC) = D_0045ACD8;
    return self;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C738);
#ifdef SKIP_ASM
extern void* cBEScoreInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the score interface singleton cBEScoreInterface_instance (operator_delete, no destructor) and clears it.
 */
extern "C" void func_0015C738(void)
{
    operator_delete((int*)cBEScoreInterface_instance);
    cBEScoreInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C760);
#ifdef SKIP_ASM
/**
 * Returns 1 if either profile set a new personal high score: the words at +0x10 or +0x14 are nonzero.
 *
 * cBEScoreInterface_discard clears both words.
 *
 * @param scoreInterface  the score interface
 * @return 1 or 0
 */
extern "C" int func_0015C760(void *scoreInterface) {
    int var_3;

    var_3 = 0;
    if (((*(int *)((char*)(scoreInterface) + (0x10))) != 0) || ((*(int *)((char*)(scoreInterface) + (0x14))) != 0)) {
        var_3 = 1;
    }
    return var_3;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C788);
#ifdef SKIP_ASM
/**
 * Returns 1 if either profile set a new track high score: the index words at +0x18 or +0x1C are not negative.
 *
 * cBEScoreInterface_discard resets both to -1.
 *
 * @param scoreInterface  the score interface
 * @return 1 or 0
 */
extern "C" int func_0015C788(void *scoreInterface) {
    int var_3;

    var_3 = 0;
    if (((*(int *)((char*)(scoreInterface) + (0x18))) >= 0) || ((*(int *)((char*)(scoreInterface) + (0x1C))) >= 0)) {
        var_3 = 1;
    }
    return var_3;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C7B0);
#ifdef SKIP_ASM
/**
 * Returns the new track high-score index of profile @p profileIndex: the word at +0x18 + @p profileIndex * 4.
 *
 * @param scoreInterface  the score interface
 * @param profileIndex  the profile, 0 or 1
 * @return the high-score table index, or -1 when the profile set none
 */
extern "C" int func_0015C7B0(int scoreInterface, int profileIndex) {
    return (*(int *)(((char*)(scoreInterface) + (profileIndex << 2)) + (0x18)));
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C7C0);
#ifdef SKIP_ASM
extern void *cBEScoreInterface_instance;

/**
 * Sets bIsActive (+0x8) of the score interface singleton cBEScoreInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void func_0015C7C0(int isActive) {
    (*(int *)((char*)(cBEScoreInterface_instance) + (8))) = isActive;
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C7D0__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the economy interface's submit (slot 1 of its vtable D_0045AC78).
 */
void func_0015C7D0(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C7D8__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the economy interface's discard (slot 2 of its vtable D_0045AC78).
 */
void func_0015C7D8(void* self)
{
}
#endif

//100%
INCLUDE_ASM("be/bedbreward", func_0015C7E0);
#ifdef SKIP_ASM
extern void* cBEAggressionInterface_instance;
void operator_delete(int* ptr);

/**
 * Frees the aggression interface singleton cBEAggressionInterface_instance (operator_delete, no destructor) and
 * clears it.
 */
extern "C" void func_0015C7E0(void)
{
    operator_delete((int*)cBEAggressionInterface_instance);
    cBEAggressionInterface_instance = 0;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C808);
#ifdef SKIP_ASM
extern void *cBEAggressionInterface_instance;

/**
 * Sets bIsActive (+0x8) of the aggression interface singleton cBEAggressionInterface_instance to @p isActive.
 *
 * @param isActive  1 while the interface may be used, 0 otherwise
 */
extern "C" void func_0015C808(int isActive) {
    (*(int *)((char*)(cBEAggressionInterface_instance) + (8))) = isActive;
}
#endif
//100%
INCLUDE_ASM("be/bedbreward", func_0015C818);
#ifdef SKIP_ASM
extern char D_0045AC48[];

/**
 * Constructs the aggression interface in place: stores its vtable D_0045AC48 at +0xC and returns @p self.
 *
 * @return @p self
 */
extern "C" void* func_0015C818(void* self)
{
    *(char**)((char*)self + 0xC) = D_0045AC48;
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
INCLUDE_ASM("be/bedbreward", func_0015C858);
#ifdef SKIP_ASM
extern char cBEPreRaceState_vtbl[];

/**
 * Constructs the pre-race back-end state in place: stores cBEPreRaceState_vtbl at +0x4 and returns @p self.
 *
 * @return @p self
 */
extern "C" void* func_0015C858(void* self)
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
INCLUDE_ASM("be/bedbreward", func_0015C898);
#ifdef SKIP_ASM
extern char cBERaceState_vtbl[];

/**
 * Constructs the race back-end state in place: stores cBERaceState_vtbl at +0x4 and returns @p self.
 *
 * @return @p self
 */
extern "C" void* func_0015C898(void* self)
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
INCLUDE_ASM("be/bedbreward", func_0015C8D8);
#ifdef SKIP_ASM
extern char cBEPostRaceState_vtbl[];

/**
 * Constructs the post-race back-end state in place: stores cBEPostRaceState_vtbl at +0x4 and returns @p self.
 *
 * @return @p self
 */
extern "C" void* func_0015C8D8(void* self)
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

