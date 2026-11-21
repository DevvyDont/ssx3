#include "common.h"
#include "scripter/scsndmgr.h"
#include "scripter/videngine.h"
#include "render/ps2graphicsman.h"
#include "main/game.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("scripter/scsndmgr", cScriptSoundBankManager_cScriptSoundBankManager);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char cScriptSoundBankManager_vtbl[];
extern const char str_tScriptSoundBankInfo[];

struct sScSndBank3450 {
    int id;         // 0x0
    int index;      // 0x4
    int a;          // 0x8
};


/**
 * Constructs the script sound bank manager with a table of free bank slots numbered from a start bank.
 *
 * Stores the vtable (+0xC), @p flags and @p count, and allocates @p count 12-byte "tScriptSoundBankInfo"
 * slots with those memory flags; slot i gets user ID -1, bank number @p base + i and no data.
 *
 * @param flags  memory flags for the slot table, kept at +0x0 (On Tour memFlags)
 * @param count  number of bank slots (On Tour maxBanks)
 * @param base  bank number of the first slot (On Tour startBankNum)
 * @return @p self
 */
extern "C" cScriptSoundBankManager* cScriptSoundBankManager_cScriptSoundBankManager(
    cScriptSoundBankManager* self, int flags, int count, int base)
{
    *(void**)&self->mVtbl = cScriptSoundBankManager_vtbl;
    self->mMemFlags = flags;
    self->mMaxBanks = count;
    *(sScSndBank3450**)&self->mpBankInfo = (sScSndBank3450*)operator_new_tag(count * sizeof(sScSndBank3450), str_tScriptSoundBankInfo, flags, 0);
    int idx = base;
    for (int i = 0; i < self->mMaxBanks; i++) {
        ((sScSndBank3450*)self->mpBankInfo)[i].id = -1;
        ((sScSndBank3450*)self->mpBankInfo)[i].index = idx++;
        ((sScSndBank3450*)self->mpBankInfo)[i].a = 0;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", cScriptSoundBankManager__cScriptSoundBankManager);
#ifdef SKIP_ASM
void cMemMan_free(void*);
void operator_delete(int* ptr);
extern "C" void cScriptSoundBankManager_UnlinkBanksAll(cScriptSoundBankManager* self);
extern char cScriptSoundBankManager_vtbl[];

/**
 * Destroys the script sound bank manager: unlinks every bank, frees its bank table (+0x8) and frees the object when
 * bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptSoundBankManager__cScriptSoundBankManager(cScriptSoundBankManager* self, int flags)
{
    self->mVtbl = (void**)cScriptSoundBankManager_vtbl;
    cScriptSoundBankManager_UnlinkBanksAll(self);
    void* bankInfo = self->mpBankInfo;
    if (bankInfo != 0) {
        cMemMan_free(bankInfo);
    }
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", cScriptSoundBankManager_LinkBank);
#ifdef SKIP_ASM
struct sSndEntry;
struct sSndList;
extern "C" sSndEntry* cScriptSoundBankManager_GetBankInfo(sSndList* self, int id);
extern "C" sSndEntry* cScriptSoundBankManager_GetEmptyBankInfo(sSndList* self);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cBankManager_BANK_Attach(void* bank, int handle, void* data, int b);

/**
 * Links a script sound bank: loads its data into a free bank slot unless the user ID is already linked.
 *
 * When no used slot has @p id (cScriptSoundBankManager_GetBankInfo), it takes the first free slot (cScriptSoundBankManager_GetEmptyBankInfo), stores the ID
 * and data and loads the bank into the audio object's sound system under the slot's bank number
 * (cBankManager_BANK_Attach). There is no check for a full table: a null slot would be written through.
 *
 * @param id  the caller's bank ID (On Tour userID)
 * @param data  the bank data, kept by the slot
 * @param dataSize  size of the bank data in bytes
 * @return the slot's bank number
 */
extern "C" int cScriptSoundBankManager_LinkBank(cScriptSoundBankManager* self, int id, void* data, int dataSize)
{
    char* bankInfo = (char*)cScriptSoundBankManager_GetBankInfo(((sSndList*)self), id);
    if (bankInfo == 0) {
        bankInfo = (char*)cScriptSoundBankManager_GetEmptyBankInfo(((sSndList*)self));
        *(int*)(bankInfo + 0x0) = id;
        *(void**)(bankInfo + 0x8) = data;
        cBankManager_BANK_Attach(**(void***)((char*)cAudioInterface_bxAudio() + 0x118), *(int*)(bankInfo + 0x4), data, dataSize);
    }
    return *(int*)(bankInfo + 0x4);
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", cScriptSoundBankManager_UnlinkBank1);
#ifdef SKIP_ASM
struct sScSndVEntry {
    short delta;
    short index;
    void (*fn)(void*, void*);
};

struct sSndEntry;
struct sSndList;
extern "C" sSndEntry* cScriptSoundBankManager_GetBankInfo(sSndList* self, int id);

/**
 * Unlinks the sound bank with user ID @p id, if linked.
 *
 * Looks the bank up with cScriptSoundBankManager_GetBankInfo and passes it to the manager's virtual slot 2 (vtable at +0xC;
 * cScriptSoundBankManager_UnlinkBank). cSSXScriptEngine_OnDetachData calls it on mSoundBankManager (+0xA38).
 *
 * @param id  the bank's user ID
 */
extern "C" void cScriptSoundBankManager_UnlinkBank1(cScriptSoundBankManager* self, int id)
{
    sSndEntry* bankInfo = cScriptSoundBankManager_GetBankInfo(((sSndList*)self), id);
    if (bankInfo != 0) {
        sScSndVEntry* vt = (sScSndVEntry*)self->mVtbl;
        vt[2].fn((char*)self + vt[2].delta, bankInfo);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", cScriptSoundBankManager_UnlinkBanksAll);
#ifdef SKIP_ASM
struct sDmEntry283658 {
    char pad_0x00[0x8];
    int used;
    
};

struct sDmVEntry283658 {
    short delta;
    short index;
    void (*fn)(void*, sDmEntry283658*);
};


/**
 * Unlinks every bank slot in use from the script sound bank manager.
 *
 * Calls the manager's virtual slot 2 (cScriptSoundBankManager_UnlinkBank) on each used slot of the bank table (On Tour mpBankInfo);
 * it releases the bank from the audio object's sound system and resets the slot's ID to -1.
 *
 * @param self_  the script sound bank manager (this)
 */
extern "C" void cScriptSoundBankManager_UnlinkBanksAll(cScriptSoundBankManager* self_)
{
    cScriptSoundBankManager* self = self_;
    int i;
    for (i = 0; i < self->mMaxBanks; i++) {
        if (((sDmEntry283658*)self->mpBankInfo)[i].used != 0) {
            ((sDmVEntry283658*)self->mVtbl)[2].fn((char*)self + ((sDmVEntry283658*)self->mVtbl)[2].delta, &((sDmEntry283658*)self->mpBankInfo)[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", cScriptSoundBankManager_GetBankInfo);
#ifdef SKIP_ASM
struct sSndEntry {
    int id;
    int unk4;
    void* data;
};

struct sSndList {
    int unk0;
    int count;
    sSndEntry* entries;
};

/**
 * Returns the linked sound bank record (user ID, bank number, data) with user ID @p id.
 *
 * Searches the manager's records (+0x8, count at +0x4) for one with data (+0x8) and that ID (+0x0).
 *
 * @param id  the bank's user ID
 * @return the record, or 0
 */
extern "C" sSndEntry* cScriptSoundBankManager_GetBankInfo(sSndList* self, int id)
{
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i].data != 0 && self->entries[i].id == id) {
            return &self->entries[i];
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", cScriptSoundBankManager_UnlinkBank);
#ifdef SKIP_ASM
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cBankManager_BANK_UnAttach(void* bank, int handle);

/**
 * Unloads one script sound bank from the audio system and frees its slot.
 *
 * Unloads the slot's bank number (+0x4) from the audio object's sound system (cAudioInterface_bxAudio() +0x118,
 * cBankManager_BANK_UnAttach), then sets the slot's user ID to -1 and its data to 0.
 *
 * @param pBankInfo  the bank's slot (On Tour tScriptSoundBankInfo* pBankInfo: userID, bankNum, pData)
 */
extern "C" void cScriptSoundBankManager_UnlinkBank(cScriptSoundBankManager* self, sSndEntry* pBankInfo)
{
    cBankManager_BANK_UnAttach(**(void***)((char*)cAudioInterface_bxAudio() + 0x118), pBankInfo->unk4);
    pBankInfo->id = -1;
    pBankInfo->data = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", cScriptSoundBankManager_GetEmptyBankInfo);
#ifdef SKIP_ASM
/**
 * Returns the first unused sound bank record (data pointer at +0x8 is 0) of the manager's records (+0x8, count at
 * +0x4).
 *
 * @return the record, or 0 when all are in use
 */
extern "C" sSndEntry* cScriptSoundBankManager_GetEmptyBankInfo(sSndList* self)
{
    for (int i = 0; i < self->count; i++) {
        if (self->entries[i].data == 0) {
            return &self->entries[i];
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", cVidEngine_cVidEngine);
#ifdef SKIP_ASM
extern char cVidEngine_vtbl[];

/**
 * Constructs a video engine object: zeroes its first six words and stores cVidEngine's vtable at +0x18.
 *
 * @return @p self
 */
extern "C" void* cVidEngine_cVidEngine(cVidEngine* self)
{
    self->mpPlayer = 0;
    self->mIsPlaying = 0;
    self->mFrameUpdated = 0;
    self->mNumPause = 0;
    self->mSavedDisplayFormat = 0;
    self->mRestoreCutSceneMode = 0;
    self->mVtbl = (void**)cVidEngine_vtbl;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", cVidEngine__cVidEngine);
#ifdef SKIP_ASM
void operator_delete(int* ptr);
extern "C" void cVidEngine_ResetVideoEngine(cVidEngine* self);
extern char cVidEngine_vtbl[];

/**
 * Destroys the video engine: restores its vtable (+0x18), resets it (cVidEngine_ResetVideoEngine) and frees the
 * object when bit 0 of @p flags is set.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cVidEngine__cVidEngine(cVidEngine* self, int flags)
{
    self->mVtbl = (void**)cVidEngine_vtbl;
    cVidEngine_ResetVideoEngine(self);
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/scsndmgr", cVidEngine_ResetVideoEngine);
#ifdef SKIP_ASM
extern "C" void cMoviePlayer__cMoviePlayer(void* p, int flags);
extern "C" void* cAudioInterface_bxAudio();
extern "C" void cSSXAudio_ResumeSFX(void* p);
extern "C" void cCutSceneTransitionMan_ForceToCutSceneMode(void* p);
extern int D_004A2A54;
extern int cPauseState_Flags;
extern int D_005366E8[];
extern int D_004428F0[];

class cWorld_3818 {
public:
    char pad_0x0[0x10D8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void SetTVDisplayFormat(int a);
};


/**
 * Stops and deletes the video player and undoes the game-state changes made when playback started.
 *
 * When a player exists it is destroyed (cMoviePlayer__cMoviePlayer, flags 3). If it was playing (+0x4), the pause-mode
 * stack (D_005366E8, top D_004A2A54) is popped and cPauseState_Flags reloaded from D_004428F0, sound effects resume,
 * the graphics manager's virtual slot 40 gets the value saved at +0x10, and when +0x14 is set it is cleared
 * and cCutSceneTransitionMan_ForceToCutSceneMode() is called on the cutscene transition manager. The first four words are then cleared.
 */
extern "C" void cVidEngine_ResetVideoEngine(cVidEngine* self)
{
    if (self->mpPlayer != 0) {
        cMoviePlayer__cMoviePlayer(self->mpPlayer, 3);
        if (self->mIsPlaying != 0) {
            cPauseState_Flags = D_004428F0[D_005366E8[--D_004A2A54]];
            cSSXAudio_ResumeSFX(cAudioInterface_bxAudio());
            ((cWorld_3818*)GrMan)->SetTVDisplayFormat(self->mSavedDisplayFormat);
            if (self->mRestoreCutSceneMode != 0) {
                self->mRestoreCutSceneMode = 0;
                cCutSceneTransitionMan_ForceToCutSceneMode(SSXApp->Game->CutSceneTransitionMan);
            }
        }
    }
    self->mpPlayer = 0;
    self->mIsPlaying = 0;
    self->mFrameUpdated = 0;
    self->mNumPause = 0;
}
#endif

