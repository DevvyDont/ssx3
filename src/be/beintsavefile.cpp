#include "common.h"
#include "be/beintsavefile.h"
#include "be/beintplayer.h"
#include "main/ssxapp.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern const char str_BESaveInt[];
extern void* cBESaveInterface_vtbl[16];
extern int cBESaveInterface_bDoReplay;


//99.8%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_getThis__Fv);
#ifdef SKIP_ASM
/**
 * Returns the save interface singleton, creating it on first use.
 *
 * The first call allocates 0x10 bytes tagged "BESaveInt", clears the word at 0x8, stores the vtable cBESaveInterface_vtbl,
 * keeps the object in cBESaveInterface_instance and clears cBESaveInterface_bDoReplay (the flag set while the 512 KB save buffer from
 * cBESaveInterface_getReplayBuffer() exists).
 *
 * @return the save interface
 */
void* cBESaveInterface_getThis()
{
    if (cBESaveInterface_instance == 0) {
        cBESaveInterface* mem = (cBESaveInterface*)cMemMan_alloc(0x10, str_BESaveInt, 0, 0);
        mem->bIsActive = 0;
        (*(void**)&mem->mVtbl) = cBESaveInterface_vtbl;
        cBESaveInterface_instance = (cBESaveInterface*)(mem);
        cBESaveInterface_bDoReplay = 0;
    }
    return cBESaveInterface_instance;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_getReplayBuffer);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern const char str_save_file_buffer[];
extern char* cBESaveInterface_replayBuffer;

/**
 * Returns the replay/save buffer, allocating it on first use.
 *
 * Sets cBESaveInterface_bDoReplay and allocates cBESaveInterface_replayBuffer (0x80000 bytes, 'save file buffer') if it
 * does not exist yet.
 *
 * @return cBESaveInterface_replayBuffer
 */
extern "C" char* cBESaveInterface_getReplayBuffer(void)
{
    cBESaveInterface_bDoReplay = 1;
    if (cBESaveInterface_replayBuffer == 0)
        cBESaveInterface_replayBuffer = (char*)operator_new_tag(0x80000, str_save_file_buffer, 0x100, 0);
    return cBESaveInterface_replayBuffer;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_getGameReplayBuffer);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
extern "C" int cBE_getDataSize(void* be);
extern char* cBESaveInterface_replayBuffer;

/**
 * Returns the replay part of the save buffer, which follows the career data.
 *
 * cBESaveInterface_replayBuffer + cBE_getDataSize (cBE::getDataSize, 0xBB78); no null check.
 *
 * @return the replay data
 */
extern "C" char* cBESaveInterface_getGameReplayBuffer(void)
{
    return cBESaveInterface_replayBuffer + cBE_getDataSize(cBE_getBE());
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_releaseBuffer);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern void* cBESaveInterface_buffer;

/**
 * Frees the save-profile buffer.
 *
 * Deletes cBESaveInterface_buffer if set and sets it to null.
 */
extern "C" void cBESaveInterface_releaseBuffer(void)
{
    if (cBESaveInterface_buffer)
        cMemMan_free(cBESaveInterface_buffer);
    cBESaveInterface_buffer = 0;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_releaseReplayBuffer);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern char* cBESaveInterface_replayBuffer;

/**
 * Frees the replay buffer and turns replay mode off.
 *
 * Deletes cBESaveInterface_replayBuffer if set, then clears it and cBESaveInterface_bDoReplay.
 */
extern "C" void cBESaveInterface_releaseReplayBuffer(void)
{
    if (cBESaveInterface_replayBuffer)
        cMemMan_free(cBESaveInterface_replayBuffer);
    cBESaveInterface_replayBuffer = 0;
    cBESaveInterface_bDoReplay = 0;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_generateSaveGame);
#ifdef SKIP_ASM
int cBESaveInterface_getSaveGameSize(void* self);
// PORT: the unit defines cBESaveInterface_releaseBuffer(void); this caller passes self in $a0.
void cBESaveInterface_releaseBuffer_s(void* self) __asm__("cBESaveInterface_releaseBuffer");
extern "C" void* func_0041605C(void* dst, const void* src, int n);
extern "C" int MEM_crc16(void* buf, int n, int seed);
extern void* cBESaveInterface_buffer;
struct sSaveHdr_2758 { int w[0x288 / 4]; };
extern sSaveHdr_2758 cBELibrary_optionDB;
extern char cBELibrary_highScores[];
struct sSaveBits_2758 { unsigned a : 1; unsigned b : 8; unsigned rest : 23; };
struct sSaveBits6_2758 { sSaveBits_2758 e[6]; };

/**
 * Builds the game save (settings and high scores) in a fresh buffer and returns it.
 *
 * Releases any previous buffer (cBESaveInterface_releaseBuffer) and allocates cBESaveInterface_getSaveGameSize() bytes
 * tagged "save file buffer" into cBESaveInterface_buffer. Writes the 0x288-byte block cBELibrary_optionDB, the 26 high-score
 * tables cBELibrary_highScores (2600 bytes, the ones func_0014AB20 returns), then two bits per player of cBELibrary_playerDB (bit
 * 1 of the word at +0x10 and the byte at +0x13, six players), and ends with a CRC of everything before it
 * (MEM_crc16, seed 64490). cBESaveInterface_loadSaveGame reads this layout back.
 *
 * @return the save buffer (cBESaveInterface_buffer)
 */
extern "C" void* cBESaveInterface_generateSaveGame(cBESaveInterface* self)
{
    int size = cBESaveInterface_getSaveGameSize(self);
    cBESaveInterface_releaseBuffer_s(self);
    cBESaveInterface_buffer = operator_new_tag(size, str_save_file_buffer, 0x100, 0);
    char* cursor = (char*)cBESaveInterface_buffer;
    *(sSaveHdr_2758*)cursor = cBELibrary_optionDB;
    cursor += sizeof(sSaveHdr_2758);
    func_0041605C(cursor, cBELibrary_highScores, 2600);
    cursor += 0xA28;
    sSaveBits6_2758 bits;
    for (int i = 0; i < 6; i++) {
        bits.e[i].a = cBELibrary_playerDB[i].iUseDualShock;
        bits.e[i].b = cBELibrary_playerDB[i].cControlMap;
    }
    *(sSaveBits6_2758*)cursor = bits;
    cursor += sizeof(sSaveBits6_2758);
    int crc = MEM_crc16(cBESaveInterface_buffer, size - 4, 64490);
    __builtin_memcpy(cursor, &crc, 4);
    return cBESaveInterface_buffer;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_getSaveGameSize__FPv);
#ifdef SKIP_ASM
/**
 * Returns the size of the settings save file.
 *
 * Always 0xCCC (3276) bytes.
 *
 * @return 0xCCC
 */
int cBESaveInterface_getSaveGameSize(void* self)
{
    return 3276;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_generateSaveProfile);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern const char str_save_file_buffer[];

// PORT: the unit defines cBESaveInterface_releaseBuffer_2950(void); this caller passes self in $a0.
void cBESaveInterface_releaseBuffer_2950(void* self) __asm__("cBESaveInterface_releaseBuffer");

extern "C" int MEM_crc16(void* buf, int n, int seed);

extern void* cBESaveInterface_buffer;


extern "C" int cBESaveInterface_getSaveProfileSize_2950() __asm__("cBESaveInterface_getSaveProfileSize");
class cBEIface_2950 {
public:
    char pad[0xC];
    virtual void v1();
    virtual void discard();
};
cBEIface_2950* cBENewPlayerInterface_getThis_2950() __asm__("cBENewPlayerInterface_getThis__Fv");
cBEIface_2950* cBEBAGTInterface_getThis_2950() __asm__("cBEBAGTInterface_getThis__Fv");
cBEIface_2950* cBEMissionInterface_getThis_2950() __asm__("cBEMissionInterface_getThis__Fv");
cBEIface_2950* cBEScoreInterface_getThis_2950() __asm__("cBEScoreInterface_getThis__Fv");
cBEIface_2950* cBEStatInterface_getThis_2950() __asm__("cBEStatInterface_getThis");
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
struct sProfSub_2950 { char pad[0xF88]; };
struct sProfile_2950 { sProfSub_2950 sub[10]; };
extern sProfile_2950 D_004A6CA8_2950[] __asm__("cBELibrary_profileLib");
struct sCharFlag_2950 { unsigned char f; char pad[0x1B]; };
extern sCharFlag_2950 D_00535B31_2950[] __asm__("D_00535B31");

void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
/**
 * Builds the memory-card save block for a profile and returns it.
 *
 * Frees the old buffer (cBESaveInterface_releaseBuffer) and allocates cBESaveInterface_getSaveProfileSize bytes ('save file buffer'). It holds the 12-byte
 * player name from cBELibrary_playerDB[@p idx], the whole profile from cBELibrary_profileLib[@p idx] (10 characters x
 * 0xF88), the profile's flag byte from D_00535B31, then lets the player, BAGT, mission, score and stat interfaces write
 * their parts (virtual slot 2 of each getThis()), and ends with a CRC of the rest (MEM_crc16, seed 0xFBEA).
 *
 * @param idx  the profile to save (On Tour iProfile)
 * @return the new buffer (cBESaveInterface_buffer)
 */
// Builds the save block of rider profile 'idx': name, profile, character flag, CRC.
extern "C" void* cBESaveInterface_generateSaveProfile(cBESaveInterface* self, int idx)
{
    cBEIface_2950* iface;
    int size = cBESaveInterface_getSaveProfileSize_2950();
    cBESaveInterface_releaseBuffer_2950(self);
    cBESaveInterface_buffer = new (str_save_file_buffer, 0x100, 0) char[size];
    char* dataPtr = (char*)cBESaveInterface_buffer;
    strncpy(dataPtr, cBELibrary_playerDB[idx].strUserName, 0xC);
    dataPtr += 0xC;
    *(sProfile_2950*)dataPtr = D_004A6CA8_2950[idx];
    dataPtr += sizeof(sProfile_2950);
    *dataPtr++ = D_00535B31_2950[idx].f;
    iface = cBENewPlayerInterface_getThis_2950();
    iface->discard();
    iface = cBEBAGTInterface_getThis_2950();
    iface->discard();
    iface = cBEMissionInterface_getThis_2950();
    iface->discard();
    iface = cBEScoreInterface_getThis_2950();
    iface->discard();
    iface = cBEStatInterface_getThis_2950();
    iface->discard();
    int crc = MEM_crc16(cBESaveInterface_buffer, size - 4, 64490);
    __builtin_memcpy(dataPtr, &crc, 4);
    return cBESaveInterface_buffer;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_getSaveProfileSize);
#ifdef SKIP_ASM
/**
 * Returns the size of a profile's memory-card save block.
 *
 * 0x9B61 bytes: the 12-byte name, the 0x9B50-byte profile, one flag byte and the 4-byte checksum (see cBESaveInterface_generateSaveProfile).
 *
 * @return 0x9B61
 */
extern "C" int cBESaveInterface_getSaveProfileSize()
{
    return 39777;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_loadSaveGame);
#ifdef SKIP_ASM
class cBEIface_2BB0 {
public:
    char pad[0xC];
    virtual void v1();
    virtual void onLoad();
};
cBEIface_2BB0* cBEOptionInterface_getThis();
cBEIface_2BB0* cBEScoreInterface_getThis();
cBEIface_2BB0* cBENewPlayerInterface_getThis();
extern "C" void cSSXApp_initLocale(void* app);
extern "C" void cSSXApp_activateBEOptions(void* app);

/**
 * Loads a game save written by cBESaveInterface_generateSaveGame, after checking its CRC.
 *
 * Compares the CRC of the first cBESaveInterface_getSaveGameSize() - 4 bytes (MEM_crc16, seed 64490) with the
 * stored one and returns 0 on a mismatch. Otherwise restores cBELibrary_optionDB and calls the option interface's onLoad,
 * restores the high-score tables cBELibrary_highScores (2600 bytes) and calls the score interface's onLoad, restores the two
 * player bits of cBELibrary_playerDB and calls the player interface's onLoad, then re-applies the locale and options
 * (cSSXApp_initLocale, cSSXApp_activateBEOptions).
 *
 * @param buf  the save data read from the memory card
 * @return 1 when loaded, 0 when the CRC is wrong
 */
extern "C" int cBESaveInterface_loadSaveGame(cBESaveInterface* self, char* buf)
{
    int size = cBESaveInterface_getSaveGameSize(self) - 4;
    int crc = MEM_crc16(buf, size, 64490);
    int stored = 0;
    __builtin_memcpy(&stored, buf + size, 4);
    if (crc != stored) {
        return 0;
    }
    char* cursor = buf;
    cBEIface_2BB0* iface;
    cBELibrary_optionDB = *(sSaveHdr_2758*)cursor;
    iface = cBEOptionInterface_getThis();
    cursor += sizeof(sSaveHdr_2758);
    iface->onLoad();
    func_0041605C(cBELibrary_highScores, cursor, 2600);
    cursor += 0xA28;
    iface = cBEScoreInterface_getThis();
    iface->onLoad();
    sSaveBits6_2758 bits = *(sSaveBits6_2758*)cursor;
    for (int i = 0; i < 6; i++) {
        cBELibrary_playerDB[i].iUseDualShock = bits.e[i].a;
        cBELibrary_playerDB[i].cControlMap = bits.e[i].b;
    }
    iface = cBENewPlayerInterface_getThis();
    iface->onLoad();
    cSSXApp_initLocale(SSXApp);
    cSSXApp_activateBEOptions(SSXApp);
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_loadSaveProfile);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
cBEIface_2BB0* cBEStatInterface_getThis_2DE0() __asm__("cBEStatInterface_getThis");
cBEIface_2BB0* cBEBAGTInterface_getThis_2DE0() __asm__("cBEBAGTInterface_getThis__Fv");
extern "C" char* strncpy(char* dst, const char* src, unsigned int n);
extern "C" void func_001515B8(void* p);
struct sProfSub_2DE0 { char pad[0x288]; int keep; char pad28C[0xF88 - 0x28C]; };
struct sProfile_2DE0 { sProfSub_2DE0 sub[10]; };
extern sProfile_2DE0 D_004A6CA8_2DE0[] __asm__("cBELibrary_profileLib");
struct sCharFlag_2DE0 { unsigned char f; char pad[0x1B]; };
extern sCharFlag_2DE0 D_00535B31[];

/**
 * Loads one rider profile from a profile save, after checking its CRC.
 *
 * Compares the CRC of the first cBESaveInterface_getSaveProfileSize() - 4 bytes (MEM_crc16, seed 64490) with the
 * stored one and returns 0 on a mismatch. Otherwise copies the 12-byte name into cBELibrary_playerDB[@p idx], the ten
 * character records into cBELibrary_profileLib[@p idx] and one byte into playerDB +0x11, then calls the onLoad of the
 * player, stat and BAGT interfaces. The word at +0x288 of each character record (the item index table pointer that
 * func_00156C70 reads) is kept across the load, and func_001515B8 is called on each record afterwards.
 *
 * @param buf  the save data read from the memory card
 * @param idx  the profile slot to load into (On Tour iProfile)
 * @return 1 when loaded, 0 when the CRC is wrong
 */
extern "C" int cBESaveInterface_loadSaveProfile(cBESaveInterface* self, char* buf, int idx)
{
    int size = cBESaveInterface_getSaveProfileSize() - 4;
    int crc = MEM_crc16(buf, size, 64490);
    int stored = 0;
    __builtin_memcpy(&stored, buf + size, 4);
    if (crc != stored) {
        return 0;
    }
    int keep[10];
    for (int i = 0; i < 10; i++) {
        keep[i] = D_004A6CA8_2DE0[idx].sub[i].keep;
    }
    char* cursor = buf;
    strncpy(cBELibrary_playerDB[idx].strUserName, cursor, 0xC);
    cursor += 0xC;
    D_004A6CA8_2DE0[idx] = *(sProfile_2DE0*)cursor;
    cursor += sizeof(sProfile_2DE0);
    D_00535B31[idx].f = *cursor;
    cBENewPlayerInterface_getThis()->onLoad();
    cBEStatInterface_getThis_2DE0()->onLoad();
    cBEBAGTInterface_getThis_2DE0()->onLoad();
    for (int i = 0; i < 10; i++) {
        D_004A6CA8_2DE0[idx].sub[i].keep = keep[i];
        func_001515B8(&D_004A6CA8_2DE0[idx].sub[i]);
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("be/beintsavefile", cBESaveInterface_purgeInterface);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
void cMemMan_free(void*);
extern void* cBESaveInterface_buffer;

/**
 * Destroys the save interface and frees its save buffer.
 *
 * Deletes cBESaveInterface_instance and clears it, then frees cBESaveInterface_buffer if set and clears it.
 */
extern "C" void cBESaveInterface_purgeInterface(void)
{
    operator_delete((int*)cBESaveInterface_instance);
    cBESaveInterface_instance = 0;
    if (cBESaveInterface_buffer)
        cMemMan_free(cBESaveInterface_buffer);
    cBESaveInterface_buffer = 0;
}
#endif

