#include "vtable.h"
#include "common.h"
#include "scripter/scriptobjtransition.h"
#include "camera/camera.h"
#include "world/instancerecord.h"
#include "ui/uithing.h"
#include "visualfx/transitionobj.h"
#include "path/pathsys.h"
#include "sound/ssxaudio.h"
#include "bx/math3d.h"
#include "main/gameviewman.h"
#include "scripter/scriptentitycam.h"
#include "ui/uiengine.h"
#include "world/worldcache.h"
#include "scripter/scriptobjgs.h"
#include "scripter/scriptobjevent.h"
#include "scripter/scriptobjrider.h"
#include "scripter/scriptobjcammgr.h"
#include "scripter/nislist.h"
#include "inseq/scriptplayerentity.h"
#include "inseq/scriptplayermain.h"
#include "ai/rider.h"
#include "scripter/bxscriptengine.h"
#include "scripter/scriptobjcamera.h"
#include "scripter/scriptdatabaseobject.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "world/world.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_Load);
#ifdef SKIP_ASM
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
// PORT: cSSXScriptEngine_SyncLoadScriptBank is defined with six params; this caller passes a seventh (0) in $t2.
extern "C" void cSSXScriptEngine_SyncLoadScriptBank_7(void* self, int i, const char* name, int flags, int key, int* buf, int x)
    __asm__("cSSXScriptEngine_SyncLoadScriptBank");
extern "C" void cScriptObject_InitScriptObject(void* rig);
extern const char* D_004A349C;
extern char str_SetupInfoHeap[];
extern char str_NISLists[];
extern char str_IdleLists[];

struct sLink_278358
{
    int a;
    int b;
    int c;
    int d;
    sLink_278358* next;
};

struct sRider_278358
{
    void* owner;
    int f4;
    int f8;
    char pad[0xCC - 0xC];
    sRider_278358() __asm__("tNISList_tNISList");
    // PORT: tagged array new (operator_new__FUi); the sized delete[] makes g++ keep the element-count cookie.
    void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
    void operator delete[](void* p, unsigned int size);
};

struct sSse278358
{
    char pad_0x0[0x544];
    sLink_278358* links;            // 0x544
    sLink_278358* freeLink;         // 0x548
    sRider_278358* objsA;           // 0x54C
    char pad_0x550[0x624 - 0x550];
    sRider_278358* objsB;           // 0x624
    char pad_0x628[0x6D0 - 0x628];
    char rigs[2][0x120];            // 0x6D0
    char pad_0x910[0xC];
    char rigC[0x1C];                // 0x91C
    char rigD[0x1C];                // 0x938
};

/**
 * Sets up the script engine for a game: setup-info pool, main script bank, script objects and the NIS/idle
 * lists.
 *
 * Allocates the 10-entry setup-info free list ("SetupInfoHeap"), loads bank 0 synchronously from
 * data/scripts/scdat.big|data/scripts/scdat_main.big, initialises the four built-in script objects
 * (cScriptObject_InitScriptObject) and creates two NIS lists ("NISLists") and two idle lists ("IdleLists", one per viewport).
 */
extern "C" void cSSXScriptEngine_Load(sSse278358* self)
{
    self->links = (sLink_278358*)operator_new_tag(200, str_SetupInfoHeap, 0, 0);
    for (int i = 0; i < 10; i++)
    {
        self->links[i].a = -1;
        self->links[i].b = -1;
        self->links[i].c = -1;
        self->links[i].d = -1;
        self->links[i].next = &self->links[i + 1];
    }
    self->links[9].next = 0;
    self->freeLink = self->links;
    cSSXScriptEngine_SyncLoadScriptBank_7(self, 0, D_004A349C, 0, -1, 0, 0);
    for (int i = 0; i < 2; i++)
        cScriptObject_InitScriptObject(self->rigs[i]);
    cScriptObject_InitScriptObject(self->rigC);
    cScriptObject_InitScriptObject(self->rigD);
    sRider_278358** pa = &self->objsA;
    *pa = new (str_NISLists, 0, 0) sRider_278358[2];
    int i;
    for (i = 0; i < 2; i++)
        self->objsA[i].owner = self;
    sRider_278358** pb = &self->objsB;
    *pb = new (str_IdleLists, 0, 0) sRider_278358[2];
    for (i = 0; i < 2; i++)
    {
        self->objsB[i].owner = self;
        self->objsB[i].f4 = 0;
        self->objsB[i].f8 = i;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_Purge);
#ifdef SKIP_ASM
void cMemMan_free(void*);
extern "C" void cDataManager_DetachAll(void* self);
extern "C" void tNISList__tNISList(int* self, int flags);
extern "C" void cSSXScriptEngine_ForceEndScriptsAll(cSSXScriptEngine* self);
extern "C" void cSSXScriptEngine_PurgeTrack(cSSXScriptEngine* self);
extern "C" void cSSXScriptEngine_PurgeScriptBank(cSSXScriptEngine* self, int idx);
extern "C" void cSSXScriptEngine_ClearNISPlayListState(cSSXScriptEngine* self);
extern "C" void cBXScriptEngine_ReleaseRequestAll(void* self);
extern "C" void cScriptObject_DeInitScriptObject(void* self);
extern "C" void cVidEngine_ResetVideoEngine(void* p);
extern "C" void cRenderStateMan_setInNIS(int);

struct sSseObj8590 {
    char data[0xCC];
};

struct sSseEngine8590 {
    char pad_0x0[0x500];
    char snd[0x1C];                 // 0x500
    char anims[0x544 - 0x51C];      // 0x51C
    void* buf;                      // 0x544
    int field_0x548;                // 0x548
    sSseObj8590* objsA;             // 0x54C
    char pad_0x550[0x624 - 0x550];
    sSseObj8590* objsB;             // 0x624
    char pad_0x628[0x6D0 - 0x628];
    char rigs[2][0x120];            // 0x6D0
    char pad_0x910[0xC];
    char rigC[0x1C];                // 0x91C
    char rigD[0x1C];                // 0x938
};

/**
 * Tears down what Load() and the track loads set up: ends all scripts and frees banks, lists and objects.
 *
 * Force-ends every script, releases all requests, de-initialises the built-in script objects, purges the
 * track
 * data and all 10 banks, detaches all data, resets the video engine, destroys the NIS and idle lists, frees
 * the
 * setup-info pool and clears the NIS play state.
 */
extern "C" void cSSXScriptEngine_Purge(sSseEngine8590* self)
{
    cSSXScriptEngine_ForceEndScriptsAll((cSSXScriptEngine*)self);
    cBXScriptEngine_ReleaseRequestAll(self);
    for (int i = 0; i < 2; i++) {
        cScriptObject_DeInitScriptObject(self->rigs[i]);
    }
    cScriptObject_DeInitScriptObject(self->rigC);
    cScriptObject_DeInitScriptObject(self->rigD);
    cSSXScriptEngine_PurgeTrack((cSSXScriptEngine*)self);
    for (int i = 0; i < 10; i++) {
        cSSXScriptEngine_PurgeScriptBank((cSSXScriptEngine*)self, i);
    }
    cDataManager_DetachAll(self->anims);
    cVidEngine_ResetVideoEngine(self->snd);
    if (self->objsA != 0) {
        sSseObj8590* objPtr = self->objsA + ((int*)self->objsA)[-4];
        while (self->objsA != objPtr) {
            objPtr--;
            tNISList__tNISList((int*)objPtr, 0);
        }
        cMemMan_free((char*)self->objsA - 0x10);
    }
    if (self->buf != 0) {
        cMemMan_free(self->buf);
    }
    if (self->objsB != 0) {
        sSseObj8590* objPtr = self->objsB + ((int*)self->objsB)[-4];
        while (self->objsB != objPtr) {
            objPtr--;
            tNISList__tNISList((int*)objPtr, 0);
        }
        cMemMan_free((char*)self->objsB - 0x10);
    }
    cRenderStateMan_setInNIS(0);
    cSSXScriptEngine_ClearNISPlayListState((cSSXScriptEngine*)self);
    self->objsA = 0;
    self->buf = 0;
    self->field_0x548 = 0;
    self->objsB = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_Init__FPv);
#ifdef SKIP_ASM
/**
 * Clears the engine's NIS skip lockout, mNumFramesNoSkip (+0x554). cGame_gainFocus calls it.
 */
void cSSXScriptEngine_Init(void* vself)
{
    cSSXScriptEngine* self = (cSSXScriptEngine*)vself;
    self->mNumFramesNoSkip = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_DeInit__FPv);
#ifdef SKIP_ASM
/**
 * Leaves the script engine's per-game state: clears the NIS no-skip countdown.
 *
 * Stores 0 in mNumFramesNoSkip (+0x554), exactly what cSSXScriptEngine_Init (On Tour Init, called from cGame_gainFocus) does
 * too. cGame_loseFocus calls it on ScriptEngine.
 */
void cSSXScriptEngine_DeInit(void* vself)
{
    cSSXScriptEngine* self = (cSSXScriptEngine*)vself;
    self->mNumFramesNoSkip = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_Update);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char str_hud_e[];
struct sSsxScriptRider;
struct sScrObj05B8;
extern "C" void tNISList_Update(void* r);
extern "C" void cBXScriptEngine_Update(void* self);
extern "C" void cVidEngine_UpdateVideo(void* snd);
extern "C" void cScriptObjCameraManager_Update(cScriptObjCameraManager* slot);
extern "C" void cScriptObjEvent_Update(cScriptObjEvent* self);
extern "C" int tNISList_IsSkipAllowed(tNISList* r);
// The unit declares tNISList_GetState as void, but this caller tests its result: bind an int view by asm label.
extern "C" int tNISList_GetState_i(void* r) __asm__("tNISList_GetState");
extern "C" void tNISList_DoSkip(void* r);
extern "C" int tNISList_IsPausedPendingNextScript(void* r);
extern "C" void cSSXScriptEngine_ClearNISPlayListState(cSSXScriptEngine* self);
void* cInput_getState(void* self, int a1);
int GetHashValue32(char*);
extern "C" void* cUIStateStack_findState(void* list, int hash);
extern "C" void cBXLoadIndicator_Show(void*, int);
extern "C" void cBXLoadIndicator_Hide(void*, int);




/**
 * Runs one frame of the script engine: updates the lists, requests, video, cameras and the playing NIS.
 *
 * While a NIS list plays, a human rider pressing the skip input (0x6F) skips it when skipping is allowed and
 * the
 * no-skip countdown (mNumFramesNoSkip) is 0; the play state is cleared once the list stops playing, and with
 * the
 * "hud" UI state up the load indicator shows while the list is probably loading. Counts mNumFramesNoSkip
 * down.
 */
extern "C" void cSSXScriptEngine_Update(cSSXScriptEngine* self)
{
    for (int i = 0; i < 2; i++)
        tNISList_Update(&self->mpNISLists[i]);
    for (int i = 0; i < 2; i++)
        tNISList_Update(&self->mpIdleLists[i]);
    cBXScriptEngine_Update(self);
    cVidEngine_UpdateVideo(((char*)&self->mpPlayer));
    for (int i = 0; i < 2; i++)
        cScriptObjCameraManager_Update((cScriptObjCameraManager*)self->mObjectCameraManager[i]);
    cScriptObjEvent_Update((cScriptObjEvent*)((cScriptObjEvent*)((char*)&self->mObjectEvent)));
    if (self->mPlayingNISList != 2)
    {
        tNISList* nisList = &self->mpNISLists[self->mPlayingNISList];
        if (tNISList_IsSkipAllowed((tNISList*)nisList))
        {
            cAI* lvl = SSXApp->Game->AI;
            int n = lvl->NumberOfPlayersAllocated;
            for (int j = 0; j < n; j++)
            {
                void* obj = *(void**)(((char**)&lvl->Players[0])[j] + 0xDF0);
                if (**(int**)obj != 0 && self->mNumFramesNoSkip == 0 && cInput_getState(obj, 111))
                {
                    tNISList_DoSkip(nisList);
                    goto done;
                }
            }
        }
    done:
        if (tNISList_GetState_i(nisList) != 3)
            cSSXScriptEngine_ClearNISPlayListState((cSSXScriptEngine*)self);
        if (cUIStateStack_findState(&SSXApp->Game->UI->mStateStack, GetHashValue32(str_hud_e)))
        {
            if (tNISList_IsPausedPendingNextScript(nisList))
                cBXLoadIndicator_Show(SSXApp->Game->LoadIndicator, 1);
            else
                cBXLoadIndicator_Hide(SSXApp->Game->LoadIndicator, 1);
        }
    }
    if (self->mNumFramesNoSkip > 0)
        self->mNumFramesNoSkip--;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_UpdateLoad);
#ifdef SKIP_ASM
struct sScrRelocOwner6B0;
extern "C" int ASYNCFILE_getstatus(void*);
extern "C" void cSSXScriptEngine_ProcessAsyncLoadTrackTableComplete(cSSXScriptEngine* self, void* file);
extern "C" void cSSXScriptEngine_ProcessAsyncLoadScriptBankComplete(cSSXScriptEngine* self, void* file);


/**
 * Polls the pending asynchronous loads and processes any that have finished.
 *
 * Checks the track filter table first, then each of the 10 script banks; returns at once when mNumAsyncs is
 * 0.
 */
extern "C" void cSSXScriptEngine_UpdateLoad(cSSXScriptEngine* self)
{
    if (self->mNumAsyncs == 0) return;
    void* tableHandle = self->mFilterTableAsyncHandle;
    if (tableHandle != 0 && ASYNCFILE_getstatus(tableHandle) != 0) {
        cSSXScriptEngine_ProcessAsyncLoadTrackTableComplete(self, self->mFilterTableAsyncHandle);
    }
    tSSXScriptBankInfo* slots = (tSSXScriptBankInfo*)(&self->mScriptDataBanks[0].asyncHandle);
    for (int i = 0; i < 10; i++) {
        void* bankFile = slots[i].dataBankHandle;
        if (bankFile != 0 && ASYNCFILE_getstatus(bankFile) != 0) {
            cSSXScriptEngine_ProcessAsyncLoadScriptBankComplete((cSSXScriptEngine*)self, bankFile);
        }
    }
}
#endif

extern "C" void* cVidEngine_RenderVideo(void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_Render__FPv);
#ifdef SKIP_ASM
/**
 * Draws the script engine's video: calls cVidEngine_RenderVideo on its cVidEngine part (+0x500). cGame_render calls it each
 * frame.
 *
 * @return cVidEngine_RenderVideo's return register
 */
void* cSSXScriptEngine_Render(void* vself)
{
    cSSXScriptEngine* self = (cSSXScriptEngine*)vself;
    return cVidEngine_RenderVideo(&self->mpPlayer);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_Restart);
#ifdef SKIP_ASM
extern "C" void cSSXScriptEngine_ResetNISListAll(cSSXScriptEngine* self);
extern "C" void cSSXScriptEngine_ResetIdleListAll(cSSXScriptEngine* self);

/**
 * Restarts the script engine by resetting all NIS lists and all idle lists.
 */
extern "C" void cSSXScriptEngine_Restart(cSSXScriptEngine* self)
{
    cSSXScriptEngine_ResetNISListAll(self);
    cSSXScriptEngine_ResetIdleListAll(self);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AttachAnchorDataTable);
#ifdef SKIP_ASM
extern "C" int cSectionMan_getBESectionIDFromWorldSectionID(void* self, int a1);

/**
 * Registers a world section's anchor data table, stored by the section's BE section ID.
 *
 * @param key  world section ID (On Tour worldSectionID)
 * @param val  the anchor table (On Tour pData)
 */
extern "C" void cSSXScriptEngine_AttachAnchorDataTable(cSSXScriptEngine* self, int key, int val)
{
    *(int*)((char*)self + (cSectionMan_getBESectionIDFromWorldSectionID(SSXApp->Game->SectionMan, key) << 2) + 0x558) = val;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_DetachAnchorDataTable);
#ifdef SKIP_ASM
extern "C" int cSectionMan_getBESectionIDFromWorldSectionID(void* self, int a1);

/**
 * Forgets a world section's anchor data table.
 *
 * @param key  world section ID (On Tour worldSectionID)
 */
extern "C" void cSSXScriptEngine_DetachAnchorDataTable(cSSXScriptEngine* self, int key)
{
    *(int*)((char*)self + (cSectionMan_getBESectionIDFromWorldSectionID(SSXApp->Game->SectionMan, key) << 2) + 0x558) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ForceEndScriptsAll);
#ifdef SKIP_ASM
extern "C" void cSSXScriptEngine_ResetNISListAll(cSSXScriptEngine* self);
extern "C" void cSSXScriptEngine_ResetIdleListAll(cSSXScriptEngine* self);
extern "C" void cBXScriptEngine_StopScriptAll(void* self);

/**
 * Ends every script: resets all NIS and idle lists, then stops all running scripts.
 *
 * Same as cSSXScriptEngine_Restart() followed by cBXScriptEngine_StopScriptAll().
 */
extern "C" void cSSXScriptEngine_ForceEndScriptsAll(cSSXScriptEngine* self)
{
    cSSXScriptEngine_ResetNISListAll(self);
    cSSXScriptEngine_ResetIdleListAll(self);
    cBXScriptEngine_StopScriptAll(self);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ForcePlayUpdate);
#ifdef SKIP_ASM
extern "C" int cBXScriptEngine_ForcePlayUpdate(void* self, int key);
extern "C" int cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" void cScriptObjCameraManager_Update(cScriptObjCameraManager* slot);

struct sSsxVEntry8AD0 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

struct sScrSlot8AD0 {
    int a;
    int id;
    char pad[0x120 - 8];
};

/**
 * Forces a script request to play now and refreshes the camera managers it uses.
 *
 * After cBXScriptEngine_ForcePlayUpdate() succeeds, each of the two object camera managers that the request's player uses
 * (engine vtable slot 4) is updated with cScriptObjCameraManager_Update().
 *
 * @param key  request handle (On Tour requestHandle)
 * @return cBXScriptEngine_ForcePlayUpdate()'s result: nonzero when the request was played
 */
extern "C" int cSSXScriptEngine_ForcePlayUpdate(cSSXScriptEngine* self, int key)
{
    int result = cBXScriptEngine_ForcePlayUpdate(self, key);
    if (result != 0) {
        sScrSlot8AD0* slot = (sScrSlot8AD0*)&self->mObjectCameraManager;
        for (int i = 0; i < 2; i++, slot++) {
            sSsxVEntry8AD0* vt = (sSsxVEntry8AD0*)self->mVtbl;
            int id = slot->id;
            void* obj = vthis(self, vt[4]);
            // PORT: cBXScriptEngine_GetRequestPlayerByReqHandle returns a pointer as int
            if (vt[4].fn(obj, id, *(int*)cBXScriptEngine_GetRequestPlayerByReqHandle(self, key))) {
                cScriptObjCameraManager_Update((cScriptObjCameraManager*)slot);
            }
        }
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AsyncLoadTrack);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void* iface);

// Minimal view of cBXString; the members are bound to the game's (unmangled) symbols.
// PORT: asm labels are not accepted on destructors, so the dtor forwards to the out-of-line one (in-charge flag 2).
struct sBXStr_8B98;
extern "C" void cBXString_dtor_8B98(sBXStr_8B98* self, int flags) __asm__("cBXString__cBXString");

struct sBXStr_8B98 {
    char* p;
    int pad[3];
    sBXStr_8B98(const char* s) __asm__("cBXString_cBXString2");
    ~sBXStr_8B98() { cBXString_dtor_8B98(this, 2); }
    sBXStr_8B98& operator=(const sBXStr_8B98& s) __asm__("cBXString_operatorE");
};
sBXStr_8B98 operator+(const char* a, const sBXStr_8B98& b) __asm__("func_003186D0");
sBXStr_8B98 operator+(const sBXStr_8B98& a, const char* b) __asm__("func_00318630");
extern "C" void* ASYNCFILE_load_8B98(const char* name, int flags) __asm__("ASYNCFILE_load");
extern "C" void cSSXScriptEngine_AsyncLoadScriptBank(cSSXScriptEngine* self, int slot, const char* name, int flags, int key, int a, int b);
extern const char str_data_scripts_scdat_big_data_scripts_scfi[];
extern const char str_dat[];
extern const char str_data_scripts_scdat_big_data_scripts_scda[];
extern const char str_big_c[];

/**
 * Starts loading the current track's script data: its filter table and its script bank (bank 1).
 *
 * The files are data/scripts/scdat.big|data/scripts/scfilter<location>.dat and
 * data/scripts/scdat.big|data/scripts/scdat_<location>.big, named after the new race's location.
 */
// Loads the current profile's script file (counting it if found), then queues the profile's second script.
extern "C" void cSSXScriptEngine_AsyncLoadTrack(cSSXScriptEngine* self)
{
    sBXStr_8B98 name((char*)cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0)) + 0x34);
    sBXStr_8B98 path = str_data_scripts_scdat_big_data_scripts_scfi + name + str_dat;
    void* handle = ASYNCFILE_load_8B98(path.p, 0);
    self->mFilterTableAsyncHandle = handle;
    if (handle != 0) {
        self->mNumAsyncs += 1;
    }
    path = str_data_scripts_scdat_big_data_scripts_scda + name + str_big_c;
    cSSXScriptEngine_AsyncLoadScriptBank((cSSXScriptEngine*)(((char*)self)), 1, path.p, 0, -1, 0, 0);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_PurgeTrack);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_cancel(void* file);
extern "C" int ASYNCFILE_release(void* file, int a, int b);
extern "C" void* MEM_free(void* self);
extern "C" void cSSXScriptEngine_PurgeScriptBank(cSSXScriptEngine* self, int idx);

/**
 * Unloads the current track's scripts: script bank 1 and the category filter table.
 *
 * Cancels and releases a pending filter-table load (decrementing mNumAsyncs), purges script bank 1 (the bank
 * cSSXScriptEngine_AsyncLoadTrack loads into), frees mpFilterTableHeader and clears mFilterTableAsyncHandle,
 * mpFilterTableHeader and mpCategoryFilterTable.
 */
extern "C" void cSSXScriptEngine_PurgeTrack(cSSXScriptEngine* self)
{
    if (self->mFilterTableAsyncHandle != 0) {
        ASYNCFILE_cancel(self->mFilterTableAsyncHandle);
        ASYNCFILE_release(self->mFilterTableAsyncHandle, 0, 0);
        self->mNumAsyncs -= 1;
    }
    cSSXScriptEngine_PurgeScriptBank(self, 1);
    if (self->mpFilterTableHeader != 0) {
        MEM_free(self->mpFilterTableHeader);
    }
    self->mFilterTableAsyncHandle = 0;
    self->mpFilterTableHeader = 0;
    self->mpCategoryFilterTable = 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_IsLoadTrackComplete);
#ifdef SKIP_ASM
extern "C" void cSSXScriptEngine_UpdateLoad(cSSXScriptEngine*);
extern "C" int cSSXScriptEngine_IsBankLoading(cSSXScriptEngine*, int);

/**
 * Tells whether the track's script data has finished loading, polling the loads first.
 *
 * @return 1 when the filter table has arrived and bank 1 is no longer loading, otherwise 0
 */
extern "C" int cSSXScriptEngine_IsLoadTrackComplete(cSSXScriptEngine* self)
{
    int complete;
    cSSXScriptEngine_UpdateLoad(self);
    complete = 0;
    if ((int)self->mFilterTableAsyncHandle == 0) {
        complete = cSSXScriptEngine_IsBankLoading(self, 1) == 0;
    }
    return complete;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_IsPauseMenuAllowed);
#ifdef SKIP_ASM
extern "C" int tNISList_IsPauseMenuAllowed(void*);

/**
 * Tells whether the pause menu may open: always when no NIS list is playing, else as the playing list allows.
 *
 * @return nonzero when the pause menu is allowed
 */
extern "C" int cSSXScriptEngine_IsPauseMenuAllowed(cSSXScriptEngine* self)
{
    int i = self->mPlayingNISList;
    if (i == 2) {
        return 1;
    }
    return tNISList_IsPauseMenuAllowed(&self->mpNISLists[i]);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_DisallowNISListSkip__FPvi);
#ifdef SKIP_ASM
/**
 * Blocks NIS skipping for @p val frames: stores mNumFramesNoSkip (+0x554), which cSSXScriptEngine_Update counts down
 * while ignoring the skip button.
 *
 * @param val  the number of frames
 */
void cSSXScriptEngine_DisallowNISListSkip(void* vself, int val)
{
    cSSXScriptEngine* self = (cSSXScriptEngine*)vself;
    self->mNumFramesNoSkip = val;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_SetNISListViewIndex);
#ifdef SKIP_ASM
/**
 * Sets the view index (+0x8) of both NIS lists (mpNISLists, +0x54C, 0xCC each).
 *
 * @param scriptEngine  the script engine
 * @param viewIndex  the view index
 */
extern "C" void cSSXScriptEngine_SetNISListViewIndex(cSSXScriptEngine* scriptEngine, int viewIndex) {
    int var_3;
    int var_6;

    var_6 = 0;
    var_3 = 1;
    do {
        var_3 -= 1;
        (*(int *)((char*)((var_6 + ((int)scriptEngine->mpNISLists))) + (8))) = viewIndex;
        var_6 += 204;
    } while (var_3 >= 0);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AddToNISListByVideoCategory);
#ifdef SKIP_ASM

extern "C" void tNISList_AddByVideoCategory(tNISList* r, int a, int b);

/**
 * Adds a video of the given category to a NIS list (the list's AddByVideoCategory, tNISList_AddByVideoCategory).
 *
 * @param i  NIS list (eNISList)
 * @param category  video category (On Tour eNISVideoCategory)
 * @param scriptControlBits  script control bits
 */
extern "C" void cSSXScriptEngine_AddToNISListByVideoCategory(cSSXScriptEngine* self, int i, int category, int scriptControlBits)
{
    tNISList_AddByVideoCategory(&(self->mpNISLists)[i], category, scriptControlBits);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AddToNISListByScriptCategory);
#ifdef SKIP_ASM
extern "C" void tNISList_AddByScriptCategory(void* e, int a, int b, int c, int d, int f, int g);

/**
 * Adds a script chosen from a category to a NIS list (the list's AddByScriptCategory, tNISList_AddByScriptCategory).
 *
 * @param i  NIS list (eNISList)
 * @param scriptCategory  script category to choose from
 * @param scriptControlBits  script control bits
 * @param selectScriptNow  nonzero to pick the script now rather than when it is reached
 * @param mainCharAIIndex  AI index of the main character, or -1
 * @param supportChar0AIIndex  AI index of the first supporting character, or -1
 * @param supportChar1AIIndex  AI index of the second supporting character, or -1
 */
extern "C" void cSSXScriptEngine_AddToNISListByScriptCategory(cSSXScriptEngine* self, int i, int scriptCategory, int scriptControlBits, int selectScriptNow, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
{
    tNISList_AddByScriptCategory(&self->mpNISLists[i], scriptCategory, scriptControlBits, selectScriptNow, mainCharAIIndex, supportChar0AIIndex, supportChar1AIIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AddToNISListByScriptIndex);
#ifdef SKIP_ASM
extern "C" void tNISList_AddByScriptIndex(void* e, int a, int b, int c, int d, int f);

/**
 * Adds a script, given by its master-table index, to a NIS list (the list's AddByScriptIndex, tNISList_AddByScriptIndex).
 *
 * @param i  NIS list (eNISList)
 * @param scriptIndex  script index in the master table
 * @param scriptControlBits  script control bits
 * @param mainCharAIIndex  AI index of the main character, or -1
 * @param supportChar0AIIndex  AI index of the first supporting character, or -1
 * @param supportChar1AIIndex  AI index of the second supporting character, or -1
 */
extern "C" void cSSXScriptEngine_AddToNISListByScriptIndex(cSSXScriptEngine* self, int i, int scriptIndex, int scriptControlBits, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
{
    tNISList_AddByScriptIndex(&self->mpNISLists[i], scriptIndex, scriptControlBits, mainCharAIIndex, supportChar0AIIndex, supportChar1AIIndex);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AddToNISListByScriptHashID);
#ifdef SKIP_ASM
struct sScriptKeyEntry {
    int unk0;
    int key;
    int unk8;
};

/**
 * Adds a script, given by its hash ID, to a NIS list; does nothing if no master-table script has that ID.
 *
 * @param nisList  NIS list (eNISList)
 * @param key  script hash ID
 * @param scriptControlBits  script control bits
 * @param mainCharAIIndex  AI index of the main character, or -1
 * @param supportChar0AIIndex  AI index of the first supporting character, or -1
 * @param supportChar1AIIndex  AI index of the second supporting character, or -1
 */
extern "C" void cSSXScriptEngine_AddToNISListByScriptHashID(cSSXScriptEngine* self, int nisList, int key, int scriptControlBits, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
{
    for (int i = 0; i < *(int*)self->mpMasterTableHeader; i++) {
        if (((sScriptKeyEntry*)self->mpMasterTable)[i].key == key) {
            cSSXScriptEngine_AddToNISListByScriptIndex(self, nisList, i, scriptControlBits, mainCharAIIndex, supportChar0AIIndex, supportChar1AIIndex);
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ReadyNISList);
#ifdef SKIP_ASM
extern "C" void tNISList_ReadyList(tNISList* r, int a);

/**
 * Prepares a NIS list for playing (tNISList_ReadyList(list, 0)).
 *
 * On Tour's ReadyNISList returns the list state; this declaration is void.
 *
 * @param i  NIS list (eNISList)
 */
extern "C" void cSSXScriptEngine_ReadyNISList(cSSXScriptEngine* self, int i)
{
    tNISList_ReadyList(&(self->mpNISLists)[i], 0);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_PlayNISList);
#ifdef SKIP_ASM
extern "C" void cRenderStateMan_setInNIS(int);
extern "C" int tNISList_Play(tNISList* r, int a, int b);

/**
 * Starts playing a NIS list and makes it the engine's playing list.
 *
 * If the list does not end up in state 3 (probably playing), the engine goes back to no playing list (2);
 * cRenderStateMan_setInNIS() is told 1 when play starts and 0 when it does not.
 *
 * @param i  NIS list (eNISList)
 * @param startWithTransitionIn  nonzero to start with the transition in
 * @param forceUpdate  nonzero to force an immediate update
 * @return the list's state after the play call
 */
extern "C" int cSSXScriptEngine_PlayNISList(cSSXScriptEngine* self, int i, int startWithTransitionIn, int forceUpdate)
{
    self->mPlayingNISList = i;
    cRenderStateMan_setInNIS(1);
    int state = tNISList_Play(&(self->mpNISLists)[i], startWithTransitionIn, forceUpdate);
    if (state != 3) {
        self->mPlayingNISList = 2;
        cRenderStateMan_setInNIS(0);
    }
    return state;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_TransferNISList);
#ifdef SKIP_ASM
struct sSsxScriptRider;
extern "C" int tNISList_TransferFromNISList(tNISList* a, tNISList* b);

/**
 * Moves the contents of one NIS list into another (tNISList_TransferFromNISList).
 *
 * @param destNISList  destination NIS list
 * @param sourceNISList  source NIS list
 * @return 1 when the lists are the same, otherwise tNISList_TransferFromNISList()'s result
 */
extern "C" int cSSXScriptEngine_TransferNISList(cSSXScriptEngine* self, int destNISList, int sourceNISList)
{
    if (destNISList == sourceNISList) {
        return 1;
    }
    tNISList* base = self->mpNISLists;
    return tNISList_TransferFromNISList(&base[destNISList], &base[sourceNISList]);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_PauseNISList);
#ifdef SKIP_ASM
void tNISList_Pause(void* r, int a);

/**
 * Pauses or resumes a NIS list (tNISList_Pause).
 *
 * @param i  NIS list (eNISList)
 * @param isPaused  nonzero to pause, 0 to resume
 */
extern "C" void cSSXScriptEngine_PauseNISList(cSSXScriptEngine* self, int i, int isPaused)
{
    tNISList_Pause(&(self->mpNISLists)[i], isPaused);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_SkipNISListScripts);
#ifdef SKIP_ASM
extern "C" void tNISList_SkipForward(tNISList* r, int a, int b);

/**
 * Skips scripts in a NIS list (tNISList_SkipForward).
 *
 * On Tour's version returns the list state; this declaration is void.
 *
 * @param i  NIS list (eNISList)
 * @param numScriptsToSkip  number of scripts to skip
 * @param doTransition  nonzero to transition into the next script
 */
extern "C" void cSSXScriptEngine_SkipNISListScripts(cSSXScriptEngine* self, int i, int numScriptsToSkip, int doTransition)
{
    tNISList_SkipForward(&(self->mpNISLists)[i], numScriptsToSkip, doTransition);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ResetNISList);
#ifdef SKIP_ASM
extern "C" void tNISList_Reset(void* self, int a1);
extern "C" void cSSXScriptEngine_ClearNISPlayListState(cSSXScriptEngine* self);

/**
 * Resets one of the engine's two NIS lists, clearing the NIS play state if that list is playing.
 *
 * Calls the NIS list reset routine tNISList_Reset(list, 1) on mpNISLists[@p i]; when @p i equals mPlayingNISList
 * it then calls cSSXScriptEngine_ClearNISPlayListState().
 *
 * @param i  which NIS list to reset (On Tour's eNISList: 0 or 1)
 */
extern "C" void cSSXScriptEngine_ResetNISList(cSSXScriptEngine* self, int i)
{
    tNISList_Reset(&self->mpNISLists[i], 1);
    if (self->mPlayingNISList == i) {
        cSSXScriptEngine_ClearNISPlayListState(self);
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ResetNISListAll);
#ifdef SKIP_ASM
extern "C" void cSSXScriptEngine_ResetNISList(cSSXScriptEngine*, int);

/**
 * Resets both NIS lists (indices 0 and 1) with cSSXScriptEngine_ResetNISList().
 */
extern "C" void cSSXScriptEngine_ResetNISListAll(cSSXScriptEngine* self)
{
    int i;
    for (i = 0; i < 2; i++) {
        cSSXScriptEngine_ResetNISList(self, i);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetNISListScriptCategory);
#ifdef SKIP_ASM
extern "C" void tNISList_GetScriptCategory(tNISList* r, int a);

/**
 * Looks up the script category of one entry of a NIS list (tNISList_GetScriptCategory).
 *
 * On Tour returns the category; this declaration is void, so the result is only left in the return register
 * (callers use the _r / _int aliases).
 *
 * @param i  NIS list (eNISList)
 * @param listIndex  entry in the list
 */
extern "C" void cSSXScriptEngine_GetNISListScriptCategory(cSSXScriptEngine* self, int i, int listIndex)
{
    tNISList_GetScriptCategory(&(self->mpNISLists)[i], listIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetNISListVideoCategory);
#ifdef SKIP_ASM
extern "C" void tNISList_GetVideoCategory(int, int);

/**
 * Looks up the video category of entry @p listIndex of NIS list @p nisList: tNISList_GetVideoCategory on mpNISLists[@p nisList].
 *
 * The C declares no return value; tNISList_GetVideoCategory's answer is left in the return register.
 *
 * @param scriptEngine  the script engine
 * @param nisList  the NIS list (0 main, 1 auxiliary transition)
 * @param listIndex  the entry index
 */
extern "C" void cSSXScriptEngine_GetNISListVideoCategory(cSSXScriptEngine* scriptEngine, int nisList, int listIndex) {
    tNISList_GetVideoCategory(((int)scriptEngine->mpNISLists) + (nisList * 204), listIndex);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetNISListScriptIndex);
#ifdef SKIP_ASM
extern "C" void tNISList_GetScriptIndex(int, int);

/**
 * Looks up the script index of entry @p listIndex of NIS list @p nisList: tNISList_GetScriptIndex on mpNISLists[@p nisList].
 *
 * The C declares no return value; tNISList_GetScriptIndex's answer is left in the return register.
 *
 * @param scriptEngine  the script engine
 * @param nisList  the NIS list (0 main, 1 auxiliary transition)
 * @param listIndex  the entry index
 */
extern "C" void cSSXScriptEngine_GetNISListScriptIndex(cSSXScriptEngine* scriptEngine, int nisList, int listIndex) {
    tNISList_GetScriptIndex(((int)scriptEngine->mpNISLists) + (nisList * 204), listIndex);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetNISListState);
#ifdef SKIP_ASM
extern "C" void cSSXScriptEngine_UpdateLoad(cSSXScriptEngine*);
extern "C" void tNISList_GetState(void*);

/**
 * Polls the pending loads, then queries a NIS list's state (tNISList_GetState).
 *
 * On Tour returns the state; this declaration is void.
 *
 * @param i  NIS list (eNISList)
 */
extern "C" void cSSXScriptEngine_GetNISListState(cSSXScriptEngine* self, int i)
{
    cSSXScriptEngine_UpdateLoad(self);
    tNISList_GetState(&self->mpNISLists[i]);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetNISListPlayState);
#ifdef SKIP_ASM
extern "C" void tNISList_GetPlayState(void*);

/**
 * Queries a NIS list's play state (tNISList_GetPlayState); On Tour returns it, this declaration is void.
 *
 * @param i  NIS list (eNISList)
 */
extern "C" void cSSXScriptEngine_GetNISListPlayState(cSSXScriptEngine* self, int i)
{
    tNISList_GetPlayState(&self->mpNISLists[i]);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetNISListLength);
#ifdef SKIP_ASM
void tNISList_GetLength(void*);

/**
 * Queries the number of entries in a NIS list (tNISList_GetLength); On Tour returns it, this declaration is void.
 *
 * @param i  NIS list (eNISList)
 */
extern "C" void cSSXScriptEngine_GetNISListLength(cSSXScriptEngine* self, int i)
{
    tNISList_GetLength(&self->mpNISLists[i]);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_IsNISListNextScriptReady);
#ifdef SKIP_ASM
extern "C" void tNISList_IsPendingScriptReady(int, int);

/**
 * Asks whether the pending script of NIS list @p nisList is ready: tNISList_IsPendingScriptReady on mpNISLists[@p nisList].
 *
 * The list's byte offset is passed as an extra argument the callee ignores. The C declares no return value; the answer
 * is left in the return register.
 *
 * @param scriptEngine  the script engine
 * @param nisList  the NIS list (0 main, 1 auxiliary transition)
 */
extern "C" void cSSXScriptEngine_IsNISListNextScriptReady(cSSXScriptEngine* scriptEngine, int nisList) {
    int temp_5;

    temp_5 = nisList * 204;
    tNISList_IsPendingScriptReady(((int)scriptEngine->mpNISLists) + temp_5, temp_5);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_IsActiveNISInTransition);
#ifdef SKIP_ASM
int tNISList_IsInTransition(void*);

/**
 * Tells whether the NIS list that is playing is in a transition.
 *
 * @return 0 when no list is playing (mPlayingNISList is 2), otherwise tNISList_IsInTransition() of the playing
 *     list
 */
extern "C" int cSSXScriptEngine_IsActiveNISInTransition(cSSXScriptEngine* self)
{
    int i = self->mPlayingNISList;
    if (i == 2) {
        return 0;
    }
    return tNISList_IsInTransition(&self->mpNISLists[i]);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_IsActiveNISSkippable);
#ifdef SKIP_ASM
extern "C" int tNISList_IsSkipAllowed(tNISList* r);
// PORT: the unit declares tNISList_GetPlayState as returning void, but this caller uses
// its int result; bind an int-returning alias by asm label.
extern "C" int tNISList_GetPlayState_i(tNISList* r) __asm__("tNISList_GetPlayState");

/**
 * Tells whether the player may skip the NIS that is playing.
 *
 * @return 1 when no NIS list plays; otherwise 1 only when the no-skip countdown is 0, the list allows
 *         skipping and
 *         its play state is not 3
 */
extern "C" int cSSXScriptEngine_IsActiveNISSkippable(cSSXScriptEngine* self)
{
    int idx = self->mPlayingNISList;
    if (idx == 2) {
        return 1;
    }
    int skippable = 0;
    if (self->mNumFramesNoSkip == 0) {
        if (tNISList_IsSkipAllowed(&(self->mpNISLists)[idx]) != 0) {
            skippable = tNISList_GetPlayState_i(&(self->mpNISLists)[self->mPlayingNISList]) != 3;
        }
    }
    return skippable;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_IsPlayingNISListPlayer);
#ifdef SKIP_ASM
extern "C" int tNISList_IsPlayingScriptOwner(void*, int);

/**
 * Tells whether a script player belongs to the NIS list that is playing.
 *
 * @param pScriptPlayerMain  the script player
 * @return 0 when no NIS list plays, otherwise tNISList_IsPlayingScriptOwner()'s answer
 */
extern "C" int cSSXScriptEngine_IsPlayingNISListPlayer(cSSXScriptEngine* self, int pScriptPlayerMain)
{
    int i = self->mPlayingNISList;
    if (i == 2) {
        return 0;
    }
    return tNISList_IsPlayingScriptOwner(&self->mpNISLists[i], pScriptPlayerMain);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AddToIdleListByScriptCategory);
#ifdef SKIP_ASM
extern "C" void tNISList_AddByScriptCategory(void* e, int a, int b, int c, int d, int f, int g);

/**
 * Adds a script chosen from a category to a viewport's idle list.
 *
 * @param i  viewport index
 * @param scriptCategory  script category
 * @param flag  nonzero to play the script in a loop (control bits 8)
 * @param mainCharAIIndex  AI index of the main character, or -1
 * @param supportChar0AIIndex  AI index of the first supporting character, or -1
 * @param supportChar1AIIndex  AI index of the second supporting character, or -1
 */
extern "C" void cSSXScriptEngine_AddToIdleListByScriptCategory(cSSXScriptEngine* self, int i, int scriptCategory, int flag, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
{
    tNISList_AddByScriptCategory(&self->mpIdleLists[i], scriptCategory, flag ? 8 : 0, 0, mainCharAIIndex, supportChar0AIIndex, supportChar1AIIndex);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ReadyIdleList);
#ifdef SKIP_ASM
struct sSsxScriptRider;
extern "C" void tNISList_ReadyList(tNISList* r, int a);

/**
 * Prepares a viewport's idle list for playing.
 *
 * @param i  viewport index
 * @param flag  nonzero to play the list as soon as it is ready (On Tour playOnReady)
 */
extern "C" void cSSXScriptEngine_ReadyIdleList(cSSXScriptEngine* self, int i, int flag)
{
    tNISList_ReadyList(&self->mpIdleLists[i], flag ? 2 : 0);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_PlayIdleList);
#ifdef SKIP_ASM
struct sSsxScriptRider;
extern "C" int tNISList_Play(tNISList* r, int a, int b);

/**
 * Starts playing a viewport's idle list, without a transition in.
 *
 * @param i  viewport index
 */
extern "C" void cSSXScriptEngine_PlayIdleList(cSSXScriptEngine* self, int i)
{
    tNISList_Play(&self->mpIdleLists[i], 0, 0);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ResetIdleList);
#ifdef SKIP_ASM
extern "C" void tNISList_Reset(void* self, int a1);

/**
 * Resets a viewport's idle list (tNISList_Reset(list, 1)).
 *
 * @param i  viewport index
 */
extern "C" void cSSXScriptEngine_ResetIdleList(cSSXScriptEngine* self, int i)
{
    tNISList_Reset(&self->mpIdleLists[i], 1);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ResetIdleListAll);
#ifdef SKIP_ASM
extern "C" void tNISList_Reset(void* self, int a1);

/**
 * Resets both of the engine's idle lists with the NIS list reset routine tNISList_Reset(list, 1).
 *
 * Unlike cSSXScriptEngine_ResetNISList(), it leaves the NIS play state alone.
 */
extern "C" void cSSXScriptEngine_ResetIdleListAll(cSSXScriptEngine* self)
{
    for (int i = 0; i < 2; i++) {
        tNISList_Reset(&(self->mpIdleLists)[i], 1);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetIdleListState);
#ifdef SKIP_ASM
extern "C" void cSSXScriptEngine_UpdateLoad(cSSXScriptEngine*);
extern "C" void tNISList_GetState(void*);

/**
 * Polls the pending loads, then queries a viewport's idle-list state; On Tour returns it, this is void.
 *
 * @param i  viewport index
 */
extern "C" void cSSXScriptEngine_GetIdleListState(cSSXScriptEngine* self, int i)
{
    cSSXScriptEngine_UpdateLoad(self);
    tNISList_GetState(&self->mpIdleLists[i]);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_RequestScriptByCategory);
#ifdef SKIP_ASM
extern "C" int* cSSXScriptEngine_GetScriptFromCategory(cSSXScriptEngine*, int);
void* cSSXScriptEngine_AllocScriptSetupInfo(void* self);
void cSSXScriptEngine_ReleaseScriptSetupInfo(void*, void*);
extern "C" int cBXScriptEngine_RequestAnimationScript(void* self, int key, void* node);

/**
 * Requests a script chosen from a category, with up to three characters cast in it.
 *
 * The AI indices go into a setup-info node from the pool, which becomes the request's user data and returns
 * to
 * the pool if the request fails.
 *
 * @param category  script category
 * @param mainCharAIIndex  AI index of the main character, or -1
 * @param supportChar0AIIndex  AI index of the first supporting character, or -1
 * @param supportChar1AIIndex  AI index of the second supporting character, or -1
 * @return the request handle, or a negative value when no script qualifies or the request fails
 */
extern "C" int cSSXScriptEngine_RequestScriptByCategory(cSSXScriptEngine* self, int category, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
{
    // PORT: one variable holds the script pointer and then the result
    int outcome = (int)cSSXScriptEngine_GetScriptFromCategory((cSSXScriptEngine*)self, category);
    if (outcome == 0) return -1;
    int* node = (int*)cSSXScriptEngine_AllocScriptSetupInfo(self);
    node[0] = mainCharAIIndex;
    node[1] = supportChar0AIIndex;
    node[2] = supportChar1AIIndex;
    outcome = cBXScriptEngine_RequestAnimationScript(self, *(int*)outcome, node);
    if (outcome < 0) {
        cSSXScriptEngine_ReleaseScriptSetupInfo(self, node);
    }
    return outcome;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_RequestScriptByIndex);
#ifdef SKIP_ASM
void* cSSXScriptEngine_AllocScriptSetupInfo(void* self);
void cSSXScriptEngine_ReleaseScriptSetupInfo(void*, void*);
extern "C" int cBXScriptEngine_RequestAnimationScript(void* self, int key, void* node);

/**
 * Requests a script by its master-table index, with up to three characters cast in it.
 *
 * AI indices outside the allocated riders become -1.
 *
 * @param key  script index in the master table
 * @param mainCharAIIndex  AI index of the main character, or -1
 * @param supportChar0AIIndex  AI index of the first supporting character, or -1
 * @param supportChar1AIIndex  AI index of the second supporting character, or -1
 * @return the request handle, or a negative value when the request fails
 */
extern "C" int cSSXScriptEngine_RequestScriptByIndex(cSSXScriptEngine* self, int key, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
{
    int n = SSXApp->Game->AI->NumberOfRidersAllocated;
    if (mainCharAIIndex < 0 || mainCharAIIndex >= n) {
        mainCharAIIndex = -1;
    }
    if (supportChar0AIIndex < 0 || supportChar0AIIndex >= n) {
        supportChar0AIIndex = -1;
    }
    if (supportChar1AIIndex < 0 || supportChar1AIIndex >= n) {
        supportChar1AIIndex = -1;
    }
    int* node = (int*)cSSXScriptEngine_AllocScriptSetupInfo(self);
    node[0] = mainCharAIIndex;
    node[1] = supportChar0AIIndex;
    node[2] = supportChar1AIIndex;
    int requestId = cBXScriptEngine_RequestAnimationScript(self, key, node);
    if (requestId < 0) {
        cSSXScriptEngine_ReleaseScriptSetupInfo(self, node);
    }
    return requestId;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_RequestScriptByHashID);
#ifdef SKIP_ASM
extern "C" int cSSXScriptEngine_RequestScriptByIndex(cSSXScriptEngine* self, int key, int a, int b, int c);


/**
 * Requests the master-table script with the given hash ID, with up to three characters cast in it.
 *
 * @param id  script hash ID
 * @param mainCharAIIndex  AI index of the main character, or -1
 * @param supportChar0AIIndex  AI index of the first supporting character, or -1
 * @param supportChar1AIIndex  AI index of the second supporting character, or -1
 * @return the request handle, or -1 when no script has that ID or the request fails
 */
extern "C" int cSSXScriptEngine_RequestScriptByHashID(cSSXScriptEngine* self, int id, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
{
    int i = 0;
    if (*(int*)self->mpMasterTableHeader > 0) {
        do {
            if (((int)(self->mpMasterTable)[i].hashID) == id)
                return cSSXScriptEngine_RequestScriptByIndex(self, i, mainCharAIIndex, supportChar0AIIndex, supportChar1AIIndex);
            i++;
        } while (i < *(int*)self->mpMasterTableHeader);
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_PlayRequestScript);
#ifdef SKIP_ASM
extern "C" int cBXScriptEngine_GetRequestPlayerByReqHandle(void* self, int a1);
extern "C" void* cBXScriptEngine_PlayScript(void* self, int key, int b, int c);
// PORT: cSSXScriptEngine_GetScriptSetupInfo is defined in this unit as (void*), but this caller passes
// (self, index); bind the 2-arg form to the symbol.
void* cSSXScriptEngine_GetScriptSetupInfo_2(void* self, int idx) __asm__("cSSXScriptEngine_GetScriptSetupInfo__FPv");

/**
 * Plays a requested script in a given view: stores the view in the request's setup info, then plays it.
 *
 * @param val  view index (On Tour viewIndex)
 * @param key  request handle
 * @param autoRecycle  passed to cBXScriptEngine_PlayScript(); On Tour calls it autoRecycle
 * @param loop  passed to cBXScriptEngine_PlayScript(); On Tour calls it loop
 */
extern "C" void cSSXScriptEngine_PlayRequestScript(cSSXScriptEngine* self, int val, int key, int autoRecycle, int loop)
{
    *(int*)((char*)cSSXScriptEngine_GetScriptSetupInfo_2(self, cBXScriptEngine_GetRequestPlayerByReqHandle(self, key)) + 0xC) = val;
    cBXScriptEngine_PlayScript(self, key, autoRecycle, loop);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_SelectScriptFromCategory);
#ifdef SKIP_ASM
extern "C" int* cSSXScriptEngine_GetScriptFromCategory(cSSXScriptEngine*, int);

/**
 * Picks a script from a category with GetScriptFromCategory() and returns its index.
 *
 * @param category  script category
 * @return the chosen script's master-table index, or -1 when none qualifies
 */
extern "C" int cSSXScriptEngine_SelectScriptFromCategory(cSSXScriptEngine* self, int category)
{
    int* script = cSSXScriptEngine_GetScriptFromCategory((cSSXScriptEngine*)self, category);
    if (script == 0)
        return -1;
    return *script;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnDataUnavailable);
#ifdef SKIP_ASM
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct sScriptEng0628;
extern "C" int cSSXScriptEngine_GetNextAvailableAsyncBankID(cSSXScriptEngine* self);
extern "C" void cSSXScriptEngine_AsyncLoadScriptBank(cSSXScriptEngine* self, int slot, const char* name, int flags, int key, int a, int b);
extern const char str_pct_spct_08d_big[];
extern const char str_data_scripts_scdat_big_data_scripts[];

/**
 * Starts an asynchronous load of the script data bank file that holds a missing data identifier.
 *
 * The file name is "data/scripts/scdat.big|data/scripts/" followed by @p key as eight digits and ".big".
 * cSSXScriptEngine_AsyncLoadScriptBank() loads it with memory flags 0x100 and @p key as the data identifier into
 * the first free bank from 2 up (cSSXScriptEngine_GetNextAvailableAsyncBankID, which gives -1 when all are in use).
 *
 * @param key  the script data identifier that is not available (On Tour: scriptDataIndex)
 * @return always 1
 */
extern "C" int cSSXScriptEngine_OnDataUnavailable(cSSXScriptEngine* self, int key)
{
    char buf[128];
    sprintf(buf, str_pct_spct_08d_big, str_data_scripts_scdat_big_data_scripts, key);
    cSSXScriptEngine_AsyncLoadScriptBank(self, cSSXScriptEngine_GetNextAvailableAsyncBankID(self), buf, 0x100, key, 0, 0);
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnPendingDataCancelled);
#ifdef SKIP_ASM
extern "C" int cSSXScriptEngine_GetBankNumberByDataIdentifier(cSSXScriptEngine* self, int a1, int a2);
extern "C" void cSSXScriptEngine_CancelAsyncLoadScriptBank(cSSXScriptEngine* self, int idx, int a);

/**
 * Cancels the pending asynchronous load of the script bank that holds a data identifier.
 *
 * Finds the bank with cSSXScriptEngine_GetBankNumberByDataIdentifier(), searching from bank 2, and passes it to
 * cSSXScriptEngine_CancelAsyncLoadScriptBank(), which acts only if that bank has an async handle. A -1 (not
 * found) result is not checked.
 *
 * @param scriptDataIndex  the script data identifier whose load was cancelled (On Tour: scriptDataIndex)
 */
extern "C" void cSSXScriptEngine_OnPendingDataCancelled(cSSXScriptEngine* self, int scriptDataIndex)
{
    cSSXScriptEngine_CancelAsyncLoadScriptBank(self, cSSXScriptEngine_GetBankNumberByDataIdentifier(self, scriptDataIndex, 2), scriptDataIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnScriptDataReleased);
#ifdef SKIP_ASM
extern "C" int cSSXScriptEngine_GetBankNumberByDataIdentifier(cSSXScriptEngine* self, int a1, int a2);
extern "C" void cSSXScriptEngine_PurgeScriptBank(cSSXScriptEngine* self, int idx);

/**
 * Purges the script bank that holds a data identifier once that data has been released.
 *
 * Finds the bank with cSSXScriptEngine_GetBankNumberByDataIdentifier(), searching from bank 2, and calls
 * cSSXScriptEngine_PurgeScriptBank() only when one was found.
 *
 * @param scriptDataIndex  the released script data identifier (On Tour: scriptDataIndex)
 */
extern "C" void cSSXScriptEngine_OnScriptDataReleased(cSSXScriptEngine* self, int scriptDataIndex)
{
    int idx = cSSXScriptEngine_GetBankNumberByDataIdentifier(self, scriptDataIndex, 2);
    if (idx >= 0) {
        cSSXScriptEngine_PurgeScriptBank(self, idx);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnAttachData);
#ifdef SKIP_ASM
extern "C" int strncmp(const char* a, const char* b, int n);
void* GetSSXScriptHeader1(void* self);
extern "C" void cBXScriptEngine_AttachScript(void* self, void* data, int a2);
extern "C" void cScriptAnimBankManager_LinkBank(void* mgr, int bank, void* data);
extern "C" void cScriptSoundBankManager_LinkBank(void* mgr, int bank, void* data, int a3);
extern char str_scr[];
extern char str_anm[];
extern char str_snd[];

/**
 * Handles data attached from a script bank, by file extension: scripts, animation banks or sound banks.
 *
 * "scr" data is registered as a script and tagged with the bank being attached (mCurrAttachingBankNum);
 * "anm" and "snd" data is linked into the animation or sound bank manager under that bank number.
 *
 * @param ext  the data's name, compared by its first three characters
 * @param data  the attached data
 * @param dataSize  size of the data in bytes
 * @param kind  receives the data kind: 0 script, 1 animation, 2 sound
 * @param bank  receives the bank number for animation and sound data
 */
extern "C" void cSSXScriptEngine_OnAttachData(cSSXScriptEngine* self, const char* ext, void* data, int dataSize, int* kind, int* bank)
{
    if (strncmp(str_scr, ext, 3) == 0) {
        tSSXScriptHeader* bankHeader = (tSSXScriptHeader*)GetSSXScriptHeader1(data);
        bankHeader->bankNum = self->mCurrAttachingBankNum;
        *kind = 0;
        cBXScriptEngine_AttachScript(((char*)self), data, bankHeader->scriptIndex);
    } else if (strncmp(str_anm, ext, 3) == 0) {
        *kind = 1;
        *bank = self->mCurrAttachingBankNum;
        cScriptAnimBankManager_LinkBank(&self->mAnimationBankManager, self->mCurrAttachingBankNum, data);
    } else if (strncmp(str_snd, ext, 3) == 0) {
        *kind = 2;
        *bank = self->mCurrAttachingBankNum;
        cScriptSoundBankManager_LinkBank(self->mSoundBankManager, self->mCurrAttachingBankNum, data, dataSize);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnDetachData);
#ifdef SKIP_ASM
void* GetSSXScriptHeader1(void* self);
extern "C" void cBXScriptEngine_DetachScript(void* self, int a1);
extern "C" void cScriptAnimBankManager_UnlinkBank(void* list, int id);
extern "C" void cScriptSoundBankManager_UnlinkBank1(void* list, int id);

/**
 * Handles data detached from a script bank: unregisters the script or unlinks the animation or sound bank.
 *
 * @param obj  the detached data
 * @param dataSize  size of the data (unused)
 * @param type  data kind stored by OnAttachData(): 0 script, 1 animation, 2 sound
 * @param id  bank number stored by OnAttachData()
 */
extern "C" void cSSXScriptEngine_OnDetachData(cSSXScriptEngine* self, void* obj, int dataSize, int type, int id)
{
    switch (type) {
    case 0:
        cBXScriptEngine_DetachScript(self, *(int*)((char*)GetSSXScriptHeader1(obj) + 0x18));
        break;
    case 1:
        cScriptAnimBankManager_UnlinkBank(&self->mAnimationBankManager, id);
        break;
    case 2:
        cScriptSoundBankManager_UnlinkBank1((char*)&self->mSoundBankManager, id);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnStartScript);
#ifdef SKIP_ASM
unsigned int BXrand();
unsigned char cScriptPlayerEntity_GetType(void* self);
int cScriptPlayerEntity_GetNumVariations(void* self);
int GetSSXActorHeader(void* self);
// PORT: GetSSXActorHeader1 forwards (self, index) to cScriptPlayerEntity_GetHeader; the unit declares it with one arg.
void* GetSSXActorHeader1_9A70(void* self, int i) __asm__("GetSSXActorHeader1__FPv");
// PORT: callers elsewhere declare cSSXScriptEngine_BuildCharacterBitRoleArray with two args; it takes the three player
// slots too.
extern "C" void cSSXScriptEngine_BuildCharacterBitRoleArray_9A70(void* self, int* flags, int p0, int p1, int p2) __asm__("cSSXScriptEngine_BuildCharacterBitRoleArray");
extern "C" void* cScriptPlayerMain_GetEntity(void* self, int i);
extern "C" void cScriptPlayerEntity_SetEntityVariation(void* self, int idx);
extern int D_00482608[];

struct sScriptHdr_9A70 {
    char pad_0x0;
    unsigned char numEvents;        // 0x1
};

struct sScript_9A70 {
    char pad_0x0[0x10];
    sScriptHdr_9A70* hdr;           // 0x10
};

struct sScriptOpt_9A70 {
    signed char slot;               // 0x0
    char pad_0x1;
    short mask;                     // 0x2
};


/**
 * Picks a random variation for each entity of a script that is starting, then counts the play.
 *
 * Choice entities (type 5) only pick options whose character mask matches the cast (flags built from the
 * three AI
 * indices), and a picked option tied to a cast slot uses that slot up; other entities pick any variation. The
 * script's play count in the master table is incremented.
 *
 * @param script  the script player starting the script
 * @param idx  the script's master-table index
 * @param players  the request's setup info: the three AI indices of the cast
 */
// Picks a random option for every choice event of a script, honouring the per-slot team flags.
extern "C" void cSSXScriptEngine_OnStartScript(cSSXScriptEngine* self, sScript_9A70* script, int idx, int* players)
{
    int flags[21];
    int cands[32];
    unsigned int all = 0;
    int i;
    cSSXScriptEngine_BuildCharacterBitRoleArray_9A70(self, flags, players[0], players[1], players[2]);
    {
        int j;
        for (j = 0; j < 21; j++)
            all |= flags[j];
    }
    for (i = 0; i < script->hdr->numEvents; i++) {
        void* ev = cScriptPlayerMain_GetEntity(script, i);
        if (cScriptPlayerEntity_GetType(ev) == 5) {
            int cnt = cScriptPlayerEntity_GetNumVariations(ev);
            if (cnt >= 2) {
                int numCands = 0;
                int k;
                for (k = 0; k < cnt; k++) {
                    sScriptOpt_9A70* opt = (sScriptOpt_9A70*)GetSSXActorHeader1_9A70(ev, k);
                    int slot = D_00482608[opt->slot];
                    if (slot >= 0) {
                        if (opt->mask == 0 || (opt->mask & flags[slot]) != 0)
                            cands[numCands++] = k;
                    } else {
                        if (opt->mask == 0 || (opt->mask & all) != 0)
                            cands[numCands++] = k;
                    }
                }
                if (numCands > 0) {
                    cScriptPlayerEntity_SetEntityVariation(ev, cands[BXrand() % numCands]);
                    sScriptOpt_9A70* opt = (sScriptOpt_9A70*)GetSSXActorHeader(ev);
                    int slot = D_00482608[opt->slot];
                    if (slot >= 0) {
                        all &= ~flags[slot];
                        flags[slot] = 0;
                    }
                }
            }
        } else {
            int cnt = cScriptPlayerEntity_GetNumVariations(ev);
            if (cnt >= 2)
                cScriptPlayerEntity_SetEntityVariation(ev, BXrand() % cnt);
        }
    }
    (self->mpMasterTable)[idx].numTimesPlayed++;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnReleaseRequest);
#ifdef SKIP_ASM
void cSSXScriptEngine_ReleaseScriptSetupInfo(void*, void*);

/**
 * Returns a released request's setup-info node to the engine's pool.
 *
 * @param requestHandle  request handle (unused)
 * @param pUserData  the request's user data: its setup-info node
 */
extern "C" void cSSXScriptEngine_OnReleaseRequest(cSSXScriptEngine* self, int requestHandle, void* pUserData)
{
    cSSXScriptEngine_ReleaseScriptSetupInfo(self, pUserData);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AddObject);
#ifdef SKIP_ASM
extern "C" int cScriptDatabaseObject_AddObject(cScriptDatabaseObject* self, void* obj);
extern "C" int cScriptObjCameraManager_AddCameraObject(cScriptObjCameraManager* self, cScriptObjCamera* obj);
extern "C" int cScriptObjCameraManager_SetTransitionObject(cScriptObjCameraManager* self, void* obj);

class cScriptOwner9D00 {
public:
    int state;      // 0x0
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int v06();
    virtual void* v07(int i);
    virtual void v08();
    virtual int v09();
};

struct sScrSlot9D00 {
    char pad[0x120];
};


/**
 * Adds a script object to the database and hooks cameras and transitions up to their camera manager.
 *
 * A camera (type 1) joins the camera manager its word at +0x7C selects; a transition (type 2) is set on the
 * camera manager its virtual slot 9 names.
 *
 * @param obj  the script object
 * @return the object's database ID
 */
extern "C" int cSSXScriptEngine_AddObject(cSSXScriptEngine* self, cScriptOwner9D00* obj)
{
    int result = cScriptDatabaseObject_AddObject((cScriptDatabaseObject*)(self), obj);
    if (obj->state == 1) {
        cScriptObjCameraManager_AddCameraObject((cScriptObjCameraManager*)(&((sScrSlot9D00*)&self->mObjectCameraManager[0])[*(int*)((char*)obj + 0x7C)]), (cScriptObjCamera*)obj);
    } else if (obj->state == 2) {
        cScriptObjCameraManager_SetTransitionObject((cScriptObjCameraManager*)(&((sScrSlot9D00*)&self->mObjectCameraManager[0])[obj->v09()]), obj);
    }
    return result;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_RemoveObject);
#ifdef SKIP_ASM
int cScriptDatabaseObject_GetObject(void* self, int i);
extern "C" void cScriptObjCameraManager_RemoveCameraObject(void* slot, void* obj);
extern "C" int cScriptObjCameraManager_SetTransitionObject(cScriptObjCameraManager* self, void* obj);
extern "C" int cScriptDatabaseObject_RemoveObject(cScriptDatabaseObject* self, int idx);

class cScriptOwner9DA8 {
public:
    int state;      // 0x0
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int v06();
    virtual void* v07(int i);
    virtual void v08();
    virtual int v09();
};

struct sScrSlot9DA8 {
    char pad[0x120];
};


/**
 * Removes a script object from the database, first unhooking a camera or transition from its camera manager.
 *
 * On Tour returns a bool; this declaration is void.
 *
 * @param idx  the object's database ID
 */
extern "C" void cSSXScriptEngine_RemoveObject(cSSXScriptEngine* self, int idx)
{
    // PORT: cScriptDatabaseObject_GetObject returns a table pointer as int
    cScriptOwner9DA8* obj = (cScriptOwner9DA8*)cScriptDatabaseObject_GetObject((self), idx);
    if (obj->state == 1) {
        cScriptObjCameraManager_RemoveCameraObject(&((sScrSlot9DA8*)&self->mObjectCameraManager[0])[*(int*)((char*)obj + 0x7C)], obj);
    } else if (obj->state == 2) {
        cScriptObjCameraManager_SetTransitionObject((cScriptObjCameraManager*)(&((sScrSlot9DA8*)&self->mObjectCameraManager[0])[obj->v09()]), 0);
    }
    cScriptDatabaseObject_RemoveObject((cScriptDatabaseObject*)(self), idx);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_IsObjInUseByPlayer);
#ifdef SKIP_ASM
int cScriptDatabaseObject_GetObject(void* self, int i);
extern "C" int cScriptObjCamera_IsInUseByPlayer(cScriptObjCamera* self, int val);
extern "C" void cScriptDatabaseObject_IsObjInUseByPlayer(void* self, int i, int val);
extern "C" void cScriptObjTransition_IsInUseByPlayer(void* obj, int val);

/**
 * Asks whether a script object is in use by a player, through the check for the object's type.
 *
 * Cameras (type 1) and transitions (type 2) have their own checks, others use the database's. On Tour returns
 * the answer; this declaration is void.
 *
 * @param i  the object's database ID
 * @param val  player ID
 */
extern "C" void cSSXScriptEngine_IsObjInUseByPlayer(cSSXScriptEngine* self, int i, int val)
{
    // PORT: cScriptDatabaseObject_GetObject returns a table pointer as int
    int* obj = (int*)cScriptDatabaseObject_GetObject(self, i);
    int type = *obj;
    if (type == 1) {
        cScriptObjCamera_IsInUseByPlayer((cScriptObjCamera*)obj, val);
    } else if (type != 2) {
        cScriptDatabaseObject_IsObjInUseByPlayer(self, i, val);
    } else {
        cScriptObjTransition_IsInUseByPlayer(obj, val);
    }
}
#endif

extern "C" void* cScriptAnimBankManager_GetBankInfo(void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetAnimationBankInfo__FPv);
#ifdef SKIP_ASM
/**
 * Returns the animation bank info of a script data bank.
 *
 * Forwards to cScriptAnimBankManager_GetBankInfo (On Tour cScriptAnimBankManager::GetBankInfo) on mAnimationBankManager (+0xA28). The C
 * declares only the engine: the bank number arrives in $a1 and passes through (cScriptObjRider_OnStartObjectUse binds a two-argument
 * alias). On Tour asserts the bank number is in range; retail does not.
 *
 * @return the bank's tScriptAnimBankInfo record, or 0 if none is linked
 */
void* cSSXScriptEngine_GetAnimationBankInfo(void* vself)
{
    cSSXScriptEngine* self = (cSSXScriptEngine*)vself;
    return cScriptAnimBankManager_GetBankInfo(&self->mAnimationBankManager);
}
#endif

extern "C" void* cScriptSoundBankManager_GetBankInfo(void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetSoundBankInfo__FPv);
#ifdef SKIP_ASM
/**
 * Returns the sound bank info of a script data bank.
 *
 * Forwards to cScriptSoundBankManager_GetBankInfo (On Tour cScriptSoundBankManager::GetBankInfo) on mSoundBankManager (+0xA38). The C
 * declares only the engine: the bank number arrives in $a1 and passes through (callers bind a two-argument alias).
 *
 * @return the bank's tScriptSoundBankInfo record
 */
void* cSSXScriptEngine_GetSoundBankInfo(void* vself)
{
    cSSXScriptEngine* self = (cSSXScriptEngine*)vself;
    return cScriptSoundBankManager_GetBankInfo(self->mSoundBankManager);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetSubjectRider);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
struct sLvlObj_279F18 {
    char pad0[0x18];
    void* obj;       // 0x18
};

struct sLvl_279F18 {
    char pad0[0x28];
    void* riders[6];             // 0x28
    sLvlObj_279F18* a40[2];      // 0x40
    sLvlObj_279F18* a48[5];      // 0x48
    sLvlObj_279F18* a5C[5];      // 0x5C
    char pad70[8];
    int n78;                     // 0x78
    int n7C;                     // 0x7C
    int n80;                     // 0x80
    int n84;                     // 0x84
};

static inline sLvl_279F18* Lvl_279F18()
{
    return (sLvl_279F18*)SSXApp->Game->AI;
}

static inline void* ObjOf_279F18(sLvlObj_279F18* o)
{
    return o ? o->obj : 0;
}

/**
 * Returns the rider a script subject refers to.
 *
 * Subjects 0-2 are the request's main and supporting characters (by AI index), 3-4 the human players, 5-9 the
 * computer riders then the actors, 10-14 the actors then the computer riders, and 15-20 the riders by index.
 *
 * @param slot  subject type
 * @param pPlayer  the script player whose request supplies the cast
 * @return the rider, or 0 when the subject has no rider
 */
extern "C" void* cSSXScriptEngine_GetSubjectRider(cSSXScriptEngine* self, int slot, int pPlayer)
{
    int riderIndex;
    switch (slot) {
    case 0: {
        int* riderIds = (int*)cSSXScriptEngine_GetScriptSetupInfo_2(self, pPlayer);
        if (riderIds[0] < 0) break;
        return Lvl_279F18()->riders[riderIds[0]];
    }
    case 1:
        riderIndex = ((int*)cSSXScriptEngine_GetScriptSetupInfo_2(self, pPlayer))[1];
        if (riderIndex < 0) break;
        return Lvl_279F18()->riders[riderIndex];
    case 2:
        riderIndex = ((int*)cSSXScriptEngine_GetScriptSetupInfo_2(self, pPlayer))[2];
        if (riderIndex < 0) break;
        return Lvl_279F18()->riders[riderIndex];
    case 3: case 4: {
        sLvl_279F18* lvl = Lvl_279F18();
        int j = slot - 3;
        if (j >= lvl->n7C) break;
        return ObjOf_279F18(lvl->a40[j]);
    }
    case 5: case 6: case 7: case 8: case 9: {
        sLvl_279F18* lvl = Lvl_279F18();
        int j = slot - 5;
        if (j >= lvl->n80 + lvl->n84) break;
        if (j < lvl->n80) return ObjOf_279F18(lvl->a48[j]);
        return ObjOf_279F18(lvl->a5C[j - lvl->n80]);
    }
    case 10: case 11: case 12: case 13: case 14: {
        sLvl_279F18* lvl = Lvl_279F18();
        int j = slot - 10;
        if (j >= lvl->n84 + lvl->n80) break;
        if (j < lvl->n84) return ObjOf_279F18(lvl->a5C[j]);
        return ObjOf_279F18(lvl->a48[j - lvl->n84]);
    }
    case 15: case 16: case 17: case 18: case 19: case 20: {
        sLvl_279F18* lvl = Lvl_279F18();
        int j = slot - 15;
        if (j >= lvl->n78) break;
        return lvl->riders[j];
    }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetAnchorOrientation);
#ifdef SKIP_ASM
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
struct sVec_27A0D8 {
    float x, y, z, w;
    sVec_27A0D8() {}
    sVec_27A0D8(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
} __attribute__((aligned(16)));
struct sV3_27A0D8 {
    float x, y, z;
};
extern int D_004A51D8;
extern sV3_27A0D8 D_004D3770[3];
// D_004D3680[20..22] is D_004D3770[0..2]; the lookup indexes by the raw id.
extern sV3_27A0D8 D_004D3680[];
extern "C" void cSSXScriptEngine_FillOrientationFromStartPointInfo_v(void* self, void* obj, sVec_27A0D8* pos, int* flag, float* pitch, float* yaw)
    __asm__("cSSXScriptEngine_FillOrientationFromStartPointInfo");
extern "C" void cSSXScriptEngine_FillOrientationFromRiderOrientationInfo_v(void* self, void* obj, sVec_27A0D8* pos, int* flag, float* pitch, float* yaw)
    __asm__("cSSXScriptEngine_FillOrientationFromRiderOrientationInfo");
extern "C" void cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(char* self, int slot, sVec_27A0D8* pos, int* flag, float* pitch, float* yaw, int cat,
                                const sVec_27A0D8* off) __asm__("cSSXScriptEngine_FillOrientationFromObjectOrientationInfo");
// PORT: GetSSXScriptHeader is defined returning int, but it returns a pointer here; bound by asm label.
signed char* GetSSXScriptHeader_p(void* self) __asm__("GetSSXScriptHeader__FPv");
extern "C" int cBXNash_getGroundHeight_v(void* cell, sVec_27A0D8* pos, sVec_27A0D8* outP, sVec_27A0D8* outN)
    __asm__("cBXNash_getGroundHeight");

extern "C" void cSSXScriptEngine_GetAnchorOrientation_impl(char* self, int id, void* obj, sVec_27A0D8* pos, int* baseRotX, float* baseRotY, float* baseRotZ) __asm__("cSSXScriptEngine_GetAnchorOrientation");
/**
 * Fills the base position and rotations of a script anchor.
 *
 * Clears the three angles and sets @p pos to (0, 0, 0, 1), then by @p id: - 1-18: the start point of subject rider @p
 * id + 2 (cSSXScriptEngine_GetSubjectRider, cSSXScriptEngine_FillOrientationFromStartPointInfo); 40-60: the orientation
 * of subject rider @p id - 40 (cSSXScriptEngine_FillOrientationFromRiderOrientationInfo). - Everything else uses
 * cSSXScriptEngine_FillOrientationFromObjectOrientationInfo with object type 0 (20-23), 1-4 (25-28), 5-6 (29-30), 7
 * (19), 8 (24), 9-11 (31-33) or 12-17 (34-39), category 50, or 43 for 29-30 when the player's script header
 * (GetSSXScriptHeader) starts with a nonzero byte. - 20-22 add a fixed offset from D_004D3770, filled once (guard
 * D_004A51D8): (-20, 0, 340), (-20, -275, 280), (-20, 275, 280). - 19, 23, 25-28, 31 and 34-36 drop the position onto
 * the ground: z becomes the hit point's z from cBXNash_getGroundHeight on the game's BXNash; 25 first lowers z by 1000. Other ids
 * leave the cleared values.
 *
 * @param id  the anchor type
 * @param obj  the script player (cScriptPlayerMain)
 * @param pos  receives the base position
 * @param baseRotX  receives the base X rotation
 * @param baseRotY  receives the base Y rotation
 * @param baseRotZ  receives the base Z rotation
 */
extern "C" void cSSXScriptEngine_GetAnchorOrientation_impl(char* self, int id, void* obj, sVec_27A0D8* pos, int* baseRotX, float* baseRotY, float* baseRotZ)
{
    *baseRotX = 0;
    *baseRotY = 0;
    *baseRotZ = 0;
    *pos = sVec_27A0D8(0.0f, 0.0f, 0.0f, 1.0f);
    switch (id) {
    case 40: case 41: case 42: case 43: case 44: case 45: case 46: case 47: case 48: case 49: case 50:
    case 51: case 52: case 53: case 54: case 55: case 56: case 57: case 58: case 59: case 60: {
        void* subjectRider = cSSXScriptEngine_GetSubjectRider((cSSXScriptEngine*)self, id - 40, (int)obj);
        if (subjectRider != 0)
            cSSXScriptEngine_FillOrientationFromRiderOrientationInfo_v(self, subjectRider, pos, baseRotX, baseRotY, baseRotZ);
        break;
    }
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12:
    case 13: case 14: case 15: case 16: case 17: case 18: {
        void* subjectRider = cSSXScriptEngine_GetSubjectRider((cSSXScriptEngine*)self, id + 2, (int)obj);
        if (subjectRider != 0)
            cSSXScriptEngine_FillOrientationFromStartPointInfo_v(self, subjectRider, pos, baseRotX, baseRotY, baseRotZ);
        break;
    }
    case 25:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 1, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        ((float*)pos)[2] -= 1000.0f;
        {
            sVec_27A0D8 n;
            sVec_27A0D8 groundHit;
            if (cBXNash_getGroundHeight_v(SSXApp->Game->BXNash, pos, &groundHit, &n))
                pos->z = groundHit.z;
        }
        break;
    case 26:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 2, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        {
            sVec_27A0D8 n;
            sVec_27A0D8 groundHit;
            if (cBXNash_getGroundHeight_v(SSXApp->Game->BXNash, pos, &groundHit, &n))
                pos->z = groundHit.z;
        }
        break;
    case 27:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 3, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        {
            sVec_27A0D8 n;
            sVec_27A0D8 groundHit;
            if (cBXNash_getGroundHeight_v(SSXApp->Game->BXNash, pos, &groundHit, &n))
                pos->z = groundHit.z;
        }
        break;
    case 28:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 4, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        {
            sVec_27A0D8 n;
            sVec_27A0D8 groundHit;
            if (cBXNash_getGroundHeight_v(SSXApp->Game->BXNash, pos, &groundHit, &n))
                pos->z = groundHit.z;
        }
        break;
    case 29:
    {
        int cat = 50;
        if (*GetSSXScriptHeader_p(obj) != 0)
            cat = 43;
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 5, pos, baseRotX, baseRotY, baseRotZ, cat, 0);
    }
        break;
    case 30:
    {
        int cat = 50;
        if (*GetSSXScriptHeader_p(obj) != 0)
            cat = 43;
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 6, pos, baseRotX, baseRotY, baseRotZ, cat, 0);
    }
        break;
    case 31:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 9, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        {
            sVec_27A0D8 n;
            sVec_27A0D8 groundHit;
            if (cBXNash_getGroundHeight_v(SSXApp->Game->BXNash, pos, &groundHit, &n))
                pos->z = groundHit.z;
        }
        break;
    case 32:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 10, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        break;
    case 33:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 11, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        break;
    case 34: case 35: case 36:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, id - 22, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        {
            sVec_27A0D8 n;
            sVec_27A0D8 groundHit;
            if (cBXNash_getGroundHeight_v(SSXApp->Game->BXNash, pos, &groundHit, &n))
                pos->z = groundHit.z;
        }
        break;
    case 37: case 38: case 39:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, id - 22, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        break;
    case 19:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 7, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        {
            sVec_27A0D8 n;
            sVec_27A0D8 groundHit;
            if (cBXNash_getGroundHeight_v(SSXApp->Game->BXNash, pos, &groundHit, &n))
                pos->z = groundHit.z;
        }
        break;
    case 20: case 21: case 22: {
        if (D_004A51D8 == 0) {
            D_004D3770[0].x = -20.0f;
            D_004D3770[0].y = 0.0f;
            D_004D3770[0].z = 340.0f;
            D_004D3770[1].x = -20.0f;
            D_004D3770[1].y = -275.0f;
            D_004D3770[1].z = 280.0f;
            D_004D3770[2].x = -20.0f;
            D_004D3770[2].y = 275.0f;
            D_004D3770[2].z = 280.0f;
            D_004A51D8 = 1;
        }
        sV3_27A0D8* anchorOffset = &D_004D3680[id];
        sVec_27A0D8 off(anchorOffset->x, anchorOffset->y, anchorOffset->z, 0.0f);
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 0, pos, baseRotX, baseRotY, baseRotZ, 50, &off);
        break;
    }
    case 23:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 0, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        {
            sVec_27A0D8 n;
            sVec_27A0D8 groundHit;
            if (cBXNash_getGroundHeight_v(SSXApp->Game->BXNash, pos, &groundHit, &n))
                pos->z = groundHit.z;
        }
        break;
    case 24:
        cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(self, 8, pos, baseRotX, baseRotY, baseRotZ, 50, 0);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_MakeTransitionObject);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void* cTransitionObj_cTransitionObj(void* self);
extern void* cFade_TObj_vtbl[];
// PORT: the wake-fx pool allocator ignores its argument; callers pass the object size.
extern "C" void* cTransitionObj_operator_new_sz(unsigned size) __asm__("cTransitionObj_operator_new");



extern "C" void* cWipe_TObj_cWipe_TObj(void* self, const tARGB* col, int dx, int dy);
extern "C" void* cCircle_TObj_cCircle_TObj(void* self, const tARGB* col);

static inline cFade_TObj* initFx_27A4A0(cFade_TObj* p, const tARGB& col)
{
    cTransitionObj_cTransitionObj(p);
    p->mVtbl = cFade_TObj_vtbl;
    (*(&p->mColour)) = col;
    return p;
}

/**
 * Creates a screen-transition effect object of the given type and colour.
 *
 * Types 1 and 7 are fades (7 always uses all-ones colour), 2-5 directional wipes and 6 another effect; 0 and
 * 8
 * make nothing. Colour 0 is (1, 0, 0, 0) and 1 is all ones, probably black and white.
 *
 * @param type  transition type
 * @param mode  transition colour, 0 or 1
 * @return the new transition object, or 0
 */
extern "C" void* cSSXScriptEngine_MakeTransitionObject(cSSXScriptEngine* self, int type, int mode)
{
    tARGB color;
    switch (mode) {
    case 0:
        color.A = 1.0f;
        color.R = 0.0f;
        color.G = 0.0f;
        color.B = 0.0f;
        break;
    case 1:
        color.A = 1.0f;
        color.R = 1.0f;
        color.G = 1.0f;
        color.B = 1.0f;
        break;
    }
    switch (type) {
    case 1: {
        cFade_TObj* fxObj = (cFade_TObj*)cTransitionObj_operator_new_sz(20);
        tARGB fxColor = color;
        return initFx_27A4A0(fxObj, fxColor);
    }
    case 2:
        return cWipe_TObj_cWipe_TObj(cTransitionObj_operator_new_sz(0x1C), &color, -1, 0);
    case 3:
        return cWipe_TObj_cWipe_TObj(cTransitionObj_operator_new_sz(0x1C), &color, 1, 0);
    case 4:
        return cWipe_TObj_cWipe_TObj(cTransitionObj_operator_new_sz(0x1C), &color, 0, -1);
    case 5:
        return cWipe_TObj_cWipe_TObj(cTransitionObj_operator_new_sz(0x1C), &color, 0, 1);
    case 6:
        return cCircle_TObj_cCircle_TObj(cTransitionObj_operator_new_sz(0x1C), &color);
    case 7: {
        cFade_TObj* fxObj = (cFade_TObj*)cTransitionObj_operator_new_sz(20);
        tARGB fxColor;
        fxColor.A = 1.0f;
        fxColor.R = 1.0f;
        fxColor.G = 1.0f;
        fxColor.B = 1.0f;
        return initFx_27A4A0(fxObj, fxColor);
    }
    case 0:
    case 8:
        break;
    }
    return 0;
}
#endif

// PORT: the real function is cBXScriptEngine_GetRequestUserData__FPvT0 (void*, void*); this caller passes only $a0.
void* cBXScriptEngine_GetRequestUserData(void* self) __asm__("cBXScriptEngine_GetRequestUserData__FPvT0");

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetScriptSetupInfo__FPv);
#ifdef SKIP_ASM
/**
 * Returns the script setup info of a script player: the user data of its request (cBXScriptEngine_GetRequestUserData).
 *
 * The C declares only the engine; the player arrives in the second argument register and passes through.
 *
 * @return the setup info
 */
void* cSSXScriptEngine_GetScriptSetupInfo(void* self)
{
    return cBXScriptEngine_GetRequestUserData(self);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetTransportWorldSectionID);
#ifdef SKIP_ASM
extern "C" int cSectionMan_getWorldSectionIDFromBESectionID(void* self, int a1);

/**
 * Returns the world section ID of BE section 43: cSectionMan_getWorldSectionIDFromBESectionID on the game's SectionMan.
 *
 * @return the world section ID
 */
extern "C" int cSSXScriptEngine_GetTransportWorldSectionID()
{
    return cSectionMan_getWorldSectionIDFromBESectionID(SSXApp->Game->SectionMan, 43);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnInSeqPlay);
#ifdef SKIP_ASM
extern "C" void cBXScriptEngine_OnInSeqPlay(void* self, void* a1);
extern "C" void tNISList_OnScriptPlay(void* self, int a1, void* obj);
int cBXScriptEngine_GetRequestHandleFromPlayer(void* self, void* a1);

/**
 * Handles a script player starting: runs cBXScriptEngine_OnInSeqPlay() and tells the playing NIS list.
 *
 * @param pPlayer  the script player
 */
extern "C" void cSSXScriptEngine_OnInSeqPlay(cSSXScriptEngine* self, cScriptPlayerMain* pPlayer)
{
    cBXScriptEngine_OnInSeqPlay(self, pPlayer);
    int listIndex = self->mPlayingNISList;
    if (listIndex != 2) {
        tNISList_OnScriptPlay(&self->mpNISLists[listIndex], cBXScriptEngine_GetRequestHandleFromPlayer(self, pPlayer), pPlayer);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnInSeqUpdate);
#ifdef SKIP_ASM
extern "C" void tNISList_OnScriptUpdate(void* self, int a1, void* obj, float x, float y);
int cBXScriptEngine_GetRequestHandleFromPlayer(void* self, void* a1);

/**
 * Passes a script player's frame update on to the playing NIS list, if any.
 *
 * @param pPlayer  the script player
 * @param x  current frame (On Tour currFrame)
 * @param y  frame delta (On Tour frameDelta)
 */
extern "C" void cSSXScriptEngine_OnInSeqUpdate(cSSXScriptEngine* self, cScriptPlayerMain* pPlayer, float x, float y)
{
    int listIndex = self->mPlayingNISList;
    if (listIndex != 2) {
        tNISList_OnScriptUpdate(&self->mpNISLists[listIndex], cBXScriptEngine_GetRequestHandleFromPlayer(self, pPlayer), pPlayer, x, y);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnInSeqPlayStopping);
#ifdef SKIP_ASM
extern "C" void tNISList_OnScriptStopping(void* self, int a1, void* obj);
int cBXScriptEngine_GetRequestHandleFromPlayer(void* self, void* a1);

/**
 * Tells the playing NIS list, if any, that a script player is stopping.
 *
 * @param pPlayer  the script player
 */
extern "C" void cSSXScriptEngine_OnInSeqPlayStopping(cSSXScriptEngine* self, cScriptPlayerMain* pPlayer)
{
    int listIndex = self->mPlayingNISList;
    if (listIndex != 2) {
        tNISList_OnScriptStopping(&self->mpNISLists[listIndex], cBXScriptEngine_GetRequestHandleFromPlayer(self, pPlayer), pPlayer);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnInSeqStop);
#ifdef SKIP_ASM
extern "C" void tNISList_OnScriptStop(void* self, int a1, void* obj);
extern "C" void cBXScriptEngine_OnInSeqStop(void* self, void* a1);
int cBXScriptEngine_GetRequestHandleFromPlayer(void* self, void* a1);

/**
 * Handles a script player stopping: tells the playing NIS list, then runs cBXScriptEngine_OnInSeqStop().
 *
 * @param pPlayer  the script player
 */
extern "C" void cSSXScriptEngine_OnInSeqStop(cSSXScriptEngine* self, cScriptPlayerMain* pPlayer)
{
    int listIndex = self->mPlayingNISList;
    if (listIndex != 2) {
        tNISList_OnScriptStop(&self->mpNISLists[listIndex], cBXScriptEngine_GetRequestHandleFromPlayer(self, pPlayer), pPlayer);
    }
    cBXScriptEngine_OnInSeqStop(self, pPlayer);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_PlayTransportSequence);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" void cSSXScriptEngine_AddToNISListByScriptCategory(cSSXScriptEngine* self, int i, int a, int b, int c, int d, int f, int g);
extern "C" void cSSXScriptEngine_ReadyNISList(cSSXScriptEngine* self, int i);
extern "C" int cSSXScriptEngine_PlayNISList(cSSXScriptEngine* self, int i, int a, int b);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028E8C0(void* self, int id, int a2);
struct sSseGame_A860 {
    char pad_0x0[0xF44];
    int field_0xF44;        // 0xF44
};

static inline int sseWorldMode_A860()
{
    return SSXApp->Game->AI->NumberOfPlayersAllocated;
}

/**
 * Queues and plays the transport cut-scene sequence in NIS list 1.
 *
 * Uses categories 18-21 when @p online is set and 12-15 otherwise, the last one picked by the world mode. On
 * Tour
 * names the parameters isGondolaSequence and doLeave, so @p online probably selects the gondola, not online
 * play.
 *
 * @param online  selects the 18-21 sequence (On Tour isGondolaSequence)
 * @param intro  nonzero to start with the first script (category 18 or 12; On Tour doLeave)
 */
extern "C" void cSSXScriptEngine_PlayTransportSequence(cSSXScriptEngine* self, int online, int intro)
{
    if (online) {
        if (intro) {
            cSSXScriptEngine_AddToNISListByScriptCategory(self, 1, 18, 1, 1, -1, -1, -1);
        }
        cSSXScriptEngine_AddToNISListByScriptCategory(self, 1, 19, 0, 0, -1, -1, -1);
        if (sseWorldMode_A860() == 1) {
            cSSXScriptEngine_AddToNISListByScriptCategory(self, 1, 20, 8, 0, -1, -1, -1);
        } else {
            cSSXScriptEngine_AddToNISListByScriptCategory(self, 1, 21, 8, 0, -1, -1, -1);
        }
    } else {
        if (intro) {
            cSSXScriptEngine_AddToNISListByScriptCategory(self, 1, 0xC, 1, 1, -1, -1, -1);
        }
        cSSXScriptEngine_AddToNISListByScriptCategory(self, 1, 13, 0, 1, -1, -1, -1);
        GrMan->mSkipNextPageFlip = 1;
        if (sseWorldMode_A860() == 1) {
            cSSXScriptEngine_AddToNISListByScriptCategory(self, 1, 0xE, 8, 1, -1, -1, -1);
        } else {
            cSSXScriptEngine_AddToNISListByScriptCategory(self, 1, 0xF, 8, 0, -1, -1, -1);
        }
    }
    cSSXScriptEngine_ReadyNISList(self, 1);
    cSSXScriptEngine_PlayNISList(self, 1, 0, 1);
    func_0028E8C0(cAudioInterface_bxAudio(), 20, 1);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_EndTransportSequence);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
int cSSXScriptEngine_GetNISListScriptCategory_r(void* self, int a1, int a2) __asm__("cSSXScriptEngine_GetNISListScriptCategory");
extern "C" void cSSXScriptEngine_SkipNISListScripts(cSSXScriptEngine* self, int a1, int a2, int a3);

/**
 * Skips the rest of the transport sequence playing in NIS list 1.
 *
 * The number of scripts skipped depends on the category of the list's first entry: 3 for 12/18, 2 for 13/19,
 * 1
 * for 14/20, none otherwise.
 *
 * @return true when scripts were skipped
 */
extern "C" bool cSSXScriptEngine_EndTransportSequence(cSSXScriptEngine* self)
{
    int skipMode = 0;
    switch (cSSXScriptEngine_GetNISListScriptCategory_r(self, 1, 0)) {
    case 12: case 18:
        skipMode = 3;
        break;
    case 13: case 19:
        skipMode = 2;
        break;
    case 14: case 20:
        skipMode = 1;
        break;
    case 15: case 16: case 17:
        break;
    }
    cSSXScriptEngine_SkipNISListScripts(self, 1, skipMode, 1);
    return skipMode != 0;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_IsInTransportSequence);
#ifdef SKIP_ASM
// PORT: the unit defines cSSXScriptEngine_GetNISListScriptCategory as void, but it returns tNISList_GetScriptCategory's
// int result (a tail call); bind an int-returning form to the symbol.
int cSSXScriptEngine_GetNISListScriptCategory_int(void* self, int i, int a) __asm__("cSSXScriptEngine_GetNISListScriptCategory");

/**
 * Tells whether NIS list 1 is playing one of the transport sequence's first three scripts.
 *
 * @return 1 when list 1 is playing and its first entry has category 12, 13, 14, 18, 19 or 20, otherwise 0
 */
extern "C" int cSSXScriptEngine_IsInTransportSequence(cSSXScriptEngine* self)
{
    if (self->mPlayingNISList != 1) {
        return 0;
    }
    int category = cSSXScriptEngine_GetNISListScriptCategory_int(self, 1, 0);
    int inTransport = 0;
    if (category == 18 || category == 0xC || category == 19 || category == 13 || category == 20 || category == 0xE) {
        inTransport = 1;
    }
    return inTransport;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AppendStartSequence);
#ifdef SKIP_ASM
extern "C" void cSSXScriptEngine_AddToNISListByScriptCategory(cSSXScriptEngine* self, int i, int a, int b, int c, int d, int f, int g);
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern int* D_00445488[];
extern int D_00481E80[];

/**
 * Queues the race-start cut-scenes for a start situation in NIS list 0.
 *
 * Situation 0 adds the intro scripts listed for the game path (or the online list); 1 and 2 add category 5;
 * 3 adds category 4 and then 5.
 *
 * @param state  start situation (On Tour eNISStartSituation)
 */
extern "C" void cSSXScriptEngine_AppendStartSequence(cSSXScriptEngine* self, int state)
{
    switch (state) {
    case 0: {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        int* tbl = D_00445488[cBENewRaceInterface_newRace.cGamePath];
        cBE_getInterface_Fv(SSXApp->BE, 7);
        if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
            tbl = D_00481E80;
        }
        if (tbl != 0 && tbl[0] >= 0) {
            do {
                cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 0, tbl[0], tbl[1], 0, -1, -1, -1);
                tbl += 2;
            } while (tbl[0] >= 0);
        }
        break;
    }
    case 1:
    case 2:
        cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 0, 5, 0, 0, -1, -1, -1);
        break;
    case 3:
        cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 0, 4, 3, 0, -1, -1, -1);
        cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 0, 5, 0, 0, -1, -1, -1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AppendEndEventSequence);
#ifdef SKIP_ASM
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" int cBENewPlayerInterface_isRivalChallengePlayed(void* self, int index, int kind, int flag);
extern "C" void cBENewPlayerInterface_setRivalChallengePlayed(void* self, int index, int kind, int flag, int value);
extern "C" int cBENewPlayerInterface_isRivalEventLocked(void* self, int index, int rc, int kind, int flag);
extern "C" int cBENewPlayerInterface_getHumanRider(void* iface, int idx);
extern "C" signed char cBELibrary_getCheatCharacterID(int index);
int cBENewPlayerInterface_getRiderCharID(void* self, int index);


struct sScVt_AC60 {
    short delta;
    short index;
    int (*fn)(void*);
};

struct sScSub_AC60 {
    sScVt_AC60* vt;
};

struct sScRider_AC60 {
    char pad_0x0[0x6C0];
    sScSub_AC60 sub;                // 0x6C0
};

struct sScSlot_AC60 {
    char pad_0x0[0x18];
    sScRider_AC60* rider;           // 0x18
};

struct sScList_AC60 {
    char pad_0x0[0x28];
    sScRider_AC60* riders[6];       // 0x28
    sScSlot_AC60* slots[2];         // 0x40
    sScSlot_AC60* first;            // 0x48
    char pad_0x4C[0x5C - 0x4C];
    sScSlot_AC60* other;            // 0x5C
    char pad_0x60[0x78 - 0x60];
    int nriders;                    // 0x78
    int nslots;                     // 0x7C
    int nfirst;                     // 0x80
    int nother;                     // 0x84
};

struct sScTbl_AC60 {
    int v[4];
};

struct sScBlk_AC60 {
    int w[0x54 / 4];
    int kind;                       // 0x54
    int w2[(0x64 - 0x58) / 4];
};

extern int cGameModeMan_iRanking[];
extern int cGameModeMan_iCharRank[];
extern sScTbl_AC60 D_00481E90;

static inline int Call_AC60(sScRider_AC60* r, int slot)
{
    sScSub_AC60* s = &r->sub;
    return s->vt[slot].fn(vthis(s, s->vt[slot]));
}

/**
 * Queues the end-of-event cut-scenes in NIS list 0 when an event finishes.
 *
 * In the career path, a human in the top three queues a podium script cast with the top three riders
 * (category
 * by the best place; some cheat characters get category 10). Then, for race modes below 4, it probably queues
 * an
 * unlock scene (categories 24 and 25) depending on cBENewPlayerInterface_isRivalChallengePlayed() and cBENewPlayerInterface_isRivalEventLocked(). Mode 4 and unfinished
 * events queue nothing.
 */
extern "C" void cSSXScriptEngine_AppendEndEventSequence(cSSXScriptEngine* self)
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 0);
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    int mode = beRaceDB->cGameMode;
    if (mode == 4) {
        return;
    }
    if (*(int*)((char*)SSXApp->GameModeMan + 0x98) == 0) {
        return;
    }
    cGame* x = SSXApp->Game;
    if ((int)x->ViewMan->m_uiNumView != 1) {
        return;
    }
    int found = 0;
    sScList_AC60* list = (sScList_AC60*)x->AI;
    if (beRaceDB->cGamePath == 0) {
        for (int i = 0; i < list->nslots; i++) {
            if (cGameModeMan_iRanking[Call_AC60(list->slots[i]->rider, 7)] < 3) {
                found = 1;
                goto done;
            }
        }
    }
done:
    if (found) {
        int ids[3];
        int best = 3;
        for (int j = 0; j < 3; j++) {
            int aiIndex = cGameModeMan_iCharRank[j];
            ids[j] = aiIndex;
            if (aiIndex >= list->nriders) {
                ids[j] = -1;
            } else if (Call_AC60(list->riders[aiIndex], 8)) {
                best = j;
            }
        }
        sScTbl_AC60 tbl = D_00481E90;
        int k = tbl.v[best];
        if (best == 0) {
            k = 9;
            int cheatCharID = cBELibrary_getCheatCharacterID(Call_AC60(list->riders[ids[0]], 7));
            if ((unsigned)(cheatCharID - 10) < 11 || cheatCharID == 0x1C) {
                k = 10;
            }
        }
        cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 0, k, 1, 0, ids[0], ids[1], ids[2]);
    }
    if (cBENewRaceInterface_newRace.cGamePath != 0) {
        return;
    }
    if ((unsigned)mode >= 4) {
        return;
    }
    void* players = cBE_getInterface_Fv(cBE_getBE(), 1);
    sScBlk_AC60 blk = *(sScBlk_AC60*)cBENewRaceInterface_getLocation(econ);
    int first = mode == 0;
    int idx = cBENewPlayerInterface_getHumanRider(players, 0);
    int result = cBENewPlayerInterface_isRivalChallengePlayed(players, idx, blk.kind, first);
    if ((unsigned)(cBELibrary_getCheatCharacterID(idx) - 10) < 11) {
        return;
    }
    int rc = cBENewPlayerInterface_getRiderCharID(players, idx);
    if (cBENewPlayerInterface_isRivalEventLocked(players, idx, rc, blk.kind, first)) {
        return;
    }
    if (result != 0) {
        return;
    }
    int aiIndex = -1;
    if (list->nfirst > 0) {
        aiIndex = Call_AC60(list->first->rider, 7);
    } else if (list->nother > 0) {
        aiIndex = Call_AC60(list->other->rider, 7);
    }
    cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 0, 0x18, 1, 0, aiIndex, -1, -1);
    cSSXScriptEngine_AddToNISListByScriptCategory(ScriptEngine, 0, 25, 3, 0, aiIndex, -1, -1);
    cBENewPlayerInterface_setRivalChallengePlayed(players, idx, blk.kind, first, 1);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetScriptFromCategory);
#ifdef SKIP_ASM
void cMemMan_free(void*);
unsigned int BXrand();
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label as the tagged operator new[].
void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern "C" void cSSXScriptEngine_BuildCharacterBitRoleArray(void* self, int* flags);
extern char str_ScriptChoice[];



struct sPri_27B0C0
{
    int pri;
    int a;
    int b;
};


/**
 * Picks a random script from a category among the least-played ones whose cast requirements are met.
 *
 * A script qualifies when each of its 21 requirement masks matches the current cast flags; among those only
 * the
 * ones with the lowest master-table play count are kept. Uses a temporary 100-entry list ("ScriptChoice").
 *
 * @param engine  the script engine
 * @param cat  script category
 * @return the chosen script's filter record (its first word is the script index), or 0
 */
extern "C" int* cSSXScriptEngine_GetScriptFromCategory(cSSXScriptEngine* engine, int cat)
{
    cSSXScriptEngine* self = engine;
    if (((int)self->mpFilterTableHeader) == 0 || (self->mpCategoryFilterTable)[cat].numScripts == 0)
        return 0;
    int flags[21];
    cSSXScriptEngine_BuildCharacterBitRoleArray(self, flags);
    int best = 0xFFFF;
    int n = 0;
    tSSXHiveScriptFilter** list = new (str_ScriptChoice, 0x100, 0) tSSXHiveScriptFilter*[100];
    int count = (self->mpCategoryFilterTable)[cat].numScripts;
    tSSXHiveScriptFilter* scripts = (self->mpCategoryFilterTable)[cat].scriptFilterPtr.pScriptFilter;
    for (int i = 0; i < count; i++)
    {
        tSSXHiveScriptFilter* script = &scripts[i];
        int pri = ((sPri_27B0C0*)self->mpMasterTable)[script->scriptIndex].pri;
        if (pri <= best)
        {
            int fail = 0;
            for (int j = 0; j < 21; j++)
            {
                if (script->charFilters[j] != 0 && (flags[j] & script->charFilters[j]) == 0)
                {
                    fail = 1;
                    break;
                }
            }
            if (!fail)
            {
                if (pri < best)
                {
                    best = pri;
                    n = 0;
                }
                list[n++] = script;
            }
        }
    }
    tSSXHiveScriptFilter* chosen = n ? list[BXrand() % n] : 0;
    if (list)
        cMemMan_free(list);
    return (int*)chosen;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_SyncLoadScriptBank);
#ifdef SKIP_ASM
void tSSXScriptBankInfo_Clear(void* slot);
extern "C" int cDataManager_AttachData(void* self, const char* name, int* data, int size);
extern "C" int BXFILE_size(const char* name);
extern "C" int* FILE_loadz(const char* name, int flags);
extern "C" int FILE_loadatz(const char* name, int* buf, int a2);


/**
 * Loads a script bank file now and attaches its contents, unless the bank is already loaded.
 *
 * With @p buf the file is read into that buffer, otherwise it is loaded with FILE_loadz() and owned by the
 * bank.
 * mCurrAttachingBankNum is set to @p i during the attach so OnAttachData() can tag the data.
 *
 * @param i  bank number
 * @param name  file name
 * @param flags  memory flags for FILE_loadz()
 * @param key  data identifier stored in the bank
 * @param buf  caller buffer to load into, or 0
 */
extern "C" void cSSXScriptEngine_SyncLoadScriptBank(cSSXScriptEngine* self, int i, const char* name, int flags, int key, int* buf)
{
    tSSXScriptBankInfo* bankInfo = (&self->mScriptDataBanks[i]);
    if (((int)bankInfo->dataBankHandle) != 0) {
        return;
    }
    bankInfo->dataIdentifier = key;
    int size = 0;
    int* data;
    if (buf != 0) {
        data = 0;
        if (FILE_loadatz(name, buf, 0) != 0) {
            data = buf;
        }
    } else {
        data = FILE_loadz(name, flags);
        if (data == 0) {
            tSSXScriptBankInfo_Clear(bankInfo);
            return;
        }
        size = BXFILE_size(name);
    }
    if (data == 0) {
        tSSXScriptBankInfo_Clear(bankInfo);
        return;
    }
    self->mCurrAttachingBankNum = i;
    (*(int*)&bankInfo->dataBankHandle) = cDataManager_AttachData(&self->mMaxDataInstances, name, data, size);
    self->mCurrAttachingBankNum = 10;
    bankInfo->dynamicMem = 1;
    if (buf != 0) {
        bankInfo->dynamicMem = 0;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AsyncLoadScriptBank);
#ifdef SKIP_ASM
extern "C" void* ASYNCFILE_loadat(const char* name, int a, int b);
extern "C" void* ASYNCFILE_load(const char* name, int flags, int b);
void tSSXScriptBankInfo_Clear(void* slot);

/**
 * Starts an asynchronous load of a script bank file; the bank is cleared if the file cannot be opened.
 *
 * @param i  bank number
 * @param name  file name
 * @param flags  memory flags used when the engine allocates the buffer
 * @param key  data identifier stored in the bank
 * @param pMem  caller buffer to load into, or 0 to allocate
 * @param memSize  size of the buffer
 */
extern "C" void cSSXScriptEngine_AsyncLoadScriptBank(cSSXScriptEngine* self, int i, const char* name, int flags, int key, int pMem, int memSize)
{
    tSSXScriptBankInfo* slot = &self->mScriptDataBanks[i];
    slot->dataIdentifier = key;
    if (pMem != 0) {
        slot->asyncHandle = ASYNCFILE_loadat(name, pMem, memSize);
        slot->dynamicMem = 0;
    } else {
        slot->asyncHandle = ASYNCFILE_load(name, flags, memSize);
        slot->dynamicMem = 1;
    }
    if (slot->asyncHandle != 0) {
        self->mNumAsyncs += 1;
    }
    if (slot->asyncHandle == 0) {
        tSSXScriptBankInfo_Clear(slot);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_PurgeScriptBank);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_cancel(void* file);
extern "C" int ASYNCFILE_release(void* file, int a, int b);
void tSSXScriptBankInfo_Clear(void* slot);
void cMemMan_free(void*);
extern "C" void* cDataManager_DetachData(void* self, int key);

/**
 * Empties a script bank: detaches its data (freeing it if the engine owns it) or cancels its pending load.
 *
 * @param i  bank number
 */
extern "C" void cSSXScriptEngine_PurgeScriptBank(cSSXScriptEngine* self, int i)
{
    tSSXScriptBankInfo* slot = &self->mScriptDataBanks[i];
    int key = *(int*)slot;
    if (key != 0) {
        void* bankData = cDataManager_DetachData((char*)&self->mMaxDataInstances, key);
        if (slot->dynamicMem != 0 && bankData != 0) {
            cMemMan_free(bankData);
        }
    } else if (slot->asyncHandle != 0) {
        ASYNCFILE_cancel(slot->asyncHandle);
        ASYNCFILE_release(slot->asyncHandle, 0, 0);
        self->mNumAsyncs -= 1;
    }
    tSSXScriptBankInfo_Clear(slot);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_CancelAsyncLoadScriptBank);
#ifdef SKIP_ASM
extern "C" void ASYNCFILE_cancel(void* file);
extern "C" int ASYNCFILE_release(void* file, int a, int b);
void tSSXScriptBankInfo_Clear(void* slot);

/**
 * Cancels a bank's pending asynchronous load and clears the bank; does nothing if no load is pending.
 *
 * @param i  bank number
 * @param dataIdentifier  unused
 */
extern "C" void cSSXScriptEngine_CancelAsyncLoadScriptBank(cSSXScriptEngine* self, int i, int dataIdentifier)
{
    tSSXScriptBankInfo* slot = &self->mScriptDataBanks[i];
    if (slot->asyncHandle != 0) {
        ASYNCFILE_cancel(slot->asyncHandle);
        ASYNCFILE_release(slot->asyncHandle, 0, 0);
        self->mNumAsyncs -= 1;
        tSSXScriptBankInfo_Clear(slot);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetNextAvailableAsyncBankID);
#ifdef SKIP_ASM


static inline int isSlotFree(tSSXScriptBankInfo* ssxScriptBankInfo)
{
    if (((int)ssxScriptBankInfo->asyncHandle) == 0) {
        if (((int)ssxScriptBankInfo->dataBankHandle) == 0) return 1;
    }
    return 0;
}

/**
 * Finds a free script bank for an asynchronous load; banks 0 and 1 are never handed out.
 *
 * @return the first free bank number from 2 to 9, or -1 when all are in use
 */
extern "C" int cSSXScriptEngine_GetNextAvailableAsyncBankID(cSSXScriptEngine* self)
{
    for (int i = 2; i < 10; i++) {
        if (isSlotFree(&((&self->mScriptDataBanks[0]))[i])) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetBankNumberByDataIdentifier);
#ifdef SKIP_ASM
static inline int IsFree27B578(int* key)
{
    return key[1] == 0 && key[-1] == 0;
}

/**
 * Finds the in-use script data bank that holds a given data identifier.
 *
 * Scans mScriptDataBanks from @p start to bank 9. A bank counts as in use when it has a data bank handle or a
 * pending async load.
 *
 * @param key    data identifier to look for
 * @param start  first bank number to check
 * @return the bank number, or -1 if no bank in use holds @p key
 */
// Finds the next used entry (from `start`) whose key matches; returns its index or -1.
extern "C" int cSSXScriptEngine_GetBankNumberByDataIdentifier(cSSXScriptEngine* self, int key, int start)
{
    for (int i = start; i < 10; i++) {
        int* k = (int*)((char*)self + (i * 16 + 0x62C));
        if (!IsFree27B578(k) && *k == key) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ProcessAsyncLoadScriptBankComplete);
#ifdef SKIP_ASM
// PORT: the unit declares ASYNCFILE_release(void*, int, int); this caller passes
// out-pointers, so bind a pointer-typed alias by asm label.
extern "C" int ASYNCFILE_release_out(void* file, int** data, int* size) __asm__("ASYNCFILE_release");
extern "C" int sprintf(char* buf, const char* fmt, ...);
void tSSXScriptBankInfo_Clear(void* slot);
extern "C" int cDataManager_AttachData(void* self, const char* name, int* data, int size);
extern const char str_asyncpct_08d[];

/**
 * Attaches the data of a finished bank load, under the name "async<data identifier>".
 *
 * The bank whose async handle matches @p file is used; a load that returned no data clears the bank.
 *
 * @param file  the finished async file
 */
extern "C" void cSSXScriptEngine_ProcessAsyncLoadScriptBankComplete(cSSXScriptEngine* self, void* file)
{
    char buf[128];
    struct {
        int* data;
        int size;
    } loaded;
    for (int i = 0; i < 10; i++) {
        tSSXScriptBankInfo* ssxScriptBankInfo = &((&(self)->mScriptDataBanks[0]))[i];
        if ((void*)((int)ssxScriptBankInfo->asyncHandle) == file) {
            ASYNCFILE_release_out(file, &loaded.data, &loaded.size);
            self->mNumAsyncs -= 1;
            if (loaded.size == 0) {
                tSSXScriptBankInfo_Clear(ssxScriptBankInfo);
                return;
            }
            sprintf(buf, str_asyncpct_08d, ssxScriptBankInfo->dataIdentifier);
            *(int*)&ssxScriptBankInfo->asyncHandle = 0;
            self->mCurrAttachingBankNum = i;
            *(int*)&ssxScriptBankInfo->dataBankHandle = cDataManager_AttachData((char*)&self->mMaxDataInstances, buf, loaded.data, loaded.size);
            self->mCurrAttachingBankNum = 10;
            return;
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ProcessAsyncLoadTrackTableComplete);
#ifdef SKIP_ASM
// PORT: the unit declares ASYNCFILE_release(void*, int, int); this caller passes
// out-pointers, so bind a pointer-typed alias by asm label.
extern "C" int ASYNCFILE_release_out(void* file, int** data, int* size) __asm__("ASYNCFILE_release");

struct sScrReloc6B0 {
    int used;
    int offset;
};


/**
 * Takes the loaded track filter table and turns its category offsets into pointers.
 *
 * @param file  the finished async file
 */
extern "C" void cSSXScriptEngine_ProcessAsyncLoadTrackTableComplete(cSSXScriptEngine* self, void* file)
{
    struct {
        int* data;
        int size;
    } loaded;
    self->mFilterTableAsyncHandle = 0;
    ASYNCFILE_release_out(file, &loaded.data, &loaded.size);
    self->mNumAsyncs -= 1;
    if (loaded.size != 0) {
        *(int**)&self->mpFilterTableHeader = loaded.data;
        *(sScrReloc6B0**)&self->mpCategoryFilterTable = (sScrReloc6B0*)(loaded.data + 1);
        for (int i = 0; i < *((int*)self->mpFilterTableHeader); i++) {
            if (((sScrReloc6B0*)self->mpCategoryFilterTable)[i].used != 0) {
                // PORT: offset relocated to an absolute pointer held in int
                ((sScrReloc6B0*)self->mpCategoryFilterTable)[i].offset = (int)loaded.data + ((sScrReloc6B0*)self->mpCategoryFilterTable)[i].offset;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_FillOrientationFromStartPointInfo);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);
extern "C" void* cPathSys_getStartpoint(void*, int, int);
extern char gPaths[];

struct sVec4_27B750
{
    float x, y, z, w;
    sVec4_27B750(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
} __attribute__((aligned(16)));

struct sVec2_27B750
{
    float x, y;
    sVec2_27B750() {}
};



// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_27B750(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Length_27B750(const sVec2_27B750& v)
{
    return Sqrt_27B750(v.x * v.x + v.y * v.y);
}

static inline float Atan2_27B750(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

/**
 * Gives the base position and rotation of a rider's start point.
 *
 * The start point comes from the path system (cPathSys_getStartpoint) for the rider's start index; yaw and pitch come
 * from its direction, and roll is 0.
 *
 * @param obj  the rider
 * @param pos  receives the start position
 * @param roll  receives 0 (On Tour baseRotX)
 * @param pitch  receives the pitch of the start direction
 * @param yaw  receives the heading of the start direction
 */
extern "C" void cSSXScriptEngine_FillOrientationFromStartPointInfo(cSSXScriptEngine* self, cRider* obj, sVec4_27B750* pos, int* roll, float* pitch, float* yaw)
{
    char* sub = (char*)&obj->mVtbl;
    sVEntry<int(void*)>* vtEntry = &(*(sVEntry<int(void*)>**)sub)[7];
    tPathStartpoint* startPoint = (tPathStartpoint*)cPathSys_getStartpoint(gPaths, 1, vtEntry->fn(vthis(sub, *vtEntry)));
    *pos = sVec4_27B750(startPoint->Pos.x, startPoint->Pos.y, startPoint->Pos.z, 1.0f);
    *yaw = Atan2_27B750(startPoint->Dir.y, startPoint->Dir.x);
    float z = startPoint->Dir.z;
    sVec2_27B750 horiz;
    horiz.x = startPoint->Dir.x;
    horiz.y = startPoint->Dir.y;
    *pitch = Atan2_27B750(z, Length_27B750(horiz));
    *roll = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_FillOrientationFromRiderOrientationInfo);
#ifdef SKIP_ASM
extern "C" float func_0031C228(float x);


struct sVec2_27B948
{
    float x, y;
    sVec2_27B948(float a, float b) : x(a), y(b) {}
};


// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_27B948(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Length_27B948(const sVec2_27B948& v)
{
    return Sqrt_27B948(v.x * v.x + v.y * v.y);
}

static inline float Atan2_27B948(float y, float x)
{
    if (x == 0.0f)
    {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float a = func_0031C228(y / x);
    if (x < 0.0f)
    {
        if (y > 0.0f)
            a += 3.1415927410125732f;
        else
            a -= 3.1415927410125732f;
    }
    return a;
}

/**
 * Gives the base position and rotation of a rider: its position and the heading and pitch of its direction.
 *
 * @param obj  the rider
 * @param pos  receives the rider's position
 * @param roll  receives 0 (On Tour baseRotX)
 * @param pitch  receives the pitch of the rider's direction (+0x1B0)
 * @param yaw  receives the heading of the rider's direction
 */
extern "C" void cSSXScriptEngine_FillOrientationFromRiderOrientationInfo(cSSXScriptEngine* self, cRider* obj, t4Vector* pos, int* roll, float* pitch, float* yaw)
{
    char* sub = (char*)&obj->mVtbl;
    sVEntry<t4Vector*(void*)>* vtEntry = &(*(sVEntry<t4Vector*(void*)>**)sub)[5];
    *pos = *vtEntry->fn(vthis(sub, *vtEntry));
    t4Vector dir = *(&obj->mMatrix.row[1]);
    *yaw = Atan2_27B948(dir.y, dir.x);
    sVec2_27B948 horiz(dir.x, dir.y);
    *pitch = Atan2_27B948(dir.z, Length_27B948(horiz));
    *roll = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_FillOrientationFromObjectOrientationInfo);
#ifdef SKIP_ASM
extern "C" float func_0031C128(float x);
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern int D_00482660[];

struct sVec_27BB08 {
    float x, y, z, w;
    sVec_27BB08() {}
    sVec_27BB08(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
} __attribute__((aligned(16)));

struct sMat_27BB08 {
    float m[16];
    // PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy); the PC port needs a plain 64-byte copy.
    sMat_27BB08& operator=(const sMat_27BB08& o)
    {
        __asm__ __volatile__(
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            :
            : "r"(this), "r"(&o)
            : "memory");
        return *this;
    }
} __attribute__((aligned(16)));

// PORT: PS2-only VU0 inline asm (4x4 matrix * vector); the PC port needs a C fallback.
static inline sVec_27BB08 MulMat_27BB08(const sMat_27BB08* m, const sVec_27BB08& v)
{
    sVec_27BB08 r;
    __asm__(
        "lqc2      $vf8, %2\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "sqc2      $vf12, %0\n"
        : "=m"(r)
        : "r"(m), "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (in-place a += b).
static inline void AddEq_27BB08(sVec_27BB08& a, const sVec_27BB08& b)
{
    __asm__(
        "lqc2      $vf3, %0\n"
        "lqc2      $vf4, %1\n"
        "vadd.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "+m"(a)
        : "m"(b)
        : "memory");
}

static inline float Atan2_27BB08(float y, float x)
{
    if (x == 0.0f) {
        if (y == 0.0f)
            return y;
        if (y >= 0.0f)
            return 1.5707963705062866f;
        return -1.5707963705062866f;
    }
    float r = func_0031C228(y / x);
    if (x < 0.0f) {
        if (y > 0.0f)
            r += 3.1415927410125732f;
        else
            r -= 3.1415927410125732f;
    }
    return r;
}

static inline char* IdxToPtr_27BB08(unsigned int i)
{
    return (char*)(i << 2);
}

static inline char* RefToPtr_27BB08(unsigned int v)
{
    unsigned int i = v >> 8;
    if (i == 0)
        return 0;
    return IdxToPtr_27BB08(i);
}

static inline char* Bone_27BB08(char* model, unsigned int code)
{
    if (model == 0)
        return 0;
    return RefToPtr_27BB08(((unsigned int*)*(char**)(model + 0x1C))[code >> 8]);
}

/**
 * Gives the base position and rotation of a world object named in a section's anchor table.
 *
 * Section 50 means the current location's section. Leaves the outputs alone when the section has no anchor
 * table
 * or the entry is -1. An optional offset is rotated by the object's matrix and added to the position.
 *
 * @param slot  object type, the index into the anchor table
 * @param pos  receives the object's position
 * @param flag  receives 0 (On Tour baseRotX)
 * @param pitch  receives the object's pitch
 * @param yaw  receives the object's heading
 * @param cat  section ID, or 50 for the current location's (On Tour overrideSectionID)
 * @param off  offset in the object's frame, or 0
 */
extern "C" void cSSXScriptEngine_FillOrientationFromObjectOrientationInfo(cSSXScriptEngine* self, int slot, sVec_27BB08* pos, int* flag, float* pitch, float* yaw, int cat,
                              const sVec_27BB08* off)
{
    if (cat == 50) {
        int k = *cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0));
        cat = D_00482660[k];
    }
    int* tbl = (int*)self->mpAnchorTableDB[cat];
    if (tbl == 0)
        return;
    unsigned int code = tbl[slot];
    if (~code == 0)
        return;
    char* model = ((char**)(char*)cWorld_m_pTheWorld->m_pWorldCache->m_pSectionCacheTable)[code & 0xFF];
    tInstanceRecord* bone = (tInstanceRecord*)Bone_27BB08(model, code);
    *pos = sVec_27BB08(bone->m_fmTransform.m[3][0], bone->m_fmTransform.m[3][1], bone->m_fmTransform.m[3][2], 1.0f);
    *yaw = Atan2_27BB08(bone->m_fmTransform.m[0][1], bone->m_fmTransform.m[0][0]);
    *pitch = func_0031C128(bone->m_fmTransform.m[0][2]);
    *flag = 0;
    if (off) {
        sMat_27BB08 rot;
        rot = *(sMat_27BB08*)&bone->m_fmTransform;
        sVec_27BB08 rotatedOff = *off;
        rotatedOff.w = 0.0f;
        rotatedOff = MulMat_27BB08(&rot, rotatedOff);
        AddEq_27BB08(*pos, rotatedOff);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AllocScriptSetupInfo__FPv);
#ifdef SKIP_ASM
/**
 * Takes a script setup info record off the free list mpSetupInfoUnused (+0x548; next link at +0x10) and returns it with
 * its link cleared.
 *
 * Quirk: the list is not checked for being empty.
 *
 * @return the record
 */
void* cSSXScriptEngine_AllocScriptSetupInfo(void* vself)
{
    cSSXScriptEngine* self = (cSSXScriptEngine*)vself;
    tSSXScriptSetupInfoLinked* head = self->mpSetupInfoUnused;
    self->mpSetupInfoUnused = (tSSXScriptSetupInfoLinked*)(head->pNext);
    head->pNext = 0;
    return ((void*)head);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ReleaseScriptSetupInfo__FPvT0);
#ifdef SKIP_ASM
/**
 * Puts a script setup info record back on the free list mpSetupInfoUnused (+0x548), setting its first four words to -1.
 *
 * @param setupInfo  the record
 */
void cSSXScriptEngine_ReleaseScriptSetupInfo(void* vself, void* vsetupInfo)
{
    tSSXScriptSetupInfoLinked* setupInfo = (tSSXScriptSetupInfoLinked*)vsetupInfo;
    cSSXScriptEngine* self = (cSSXScriptEngine*)vself;
    setupInfo->pNext = (tSSXScriptSetupInfoLinked*)((void*)self->mpSetupInfoUnused);
    *(int*)setupInfo = -1;
    setupInfo->supportChar0AIIndex = -1;
    setupInfo->supportChar1AIIndex = -1;
    setupInfo->viewIndex = -1;
    self->mpSetupInfoUnused = (tSSXScriptSetupInfoLinked*)setupInfo;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_BuildCharacterBitRoleArray);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cRider_getIdentity(void* self);
extern int kCharacterMappingTable[];

struct sRiderObj_BDB8 {
    char pad_0x0[0x18];
    void* rider;                    // 0x18
};

struct sRiderSet_BDB8 {
    char pad_0x0[0x28];
    void* players[6];               // 0x28
    sRiderObj_BDB8* groupA[2];      // 0x40
    sRiderObj_BDB8* groupB[5];      // 0x48
    sRiderObj_BDB8* groupC[5];      // 0x5C
    char pad_0x70[0x8];
    int numPlayers;                 // 0x78
    int numA;                       // 0x7C
    int numB;                       // 0x80
    int numC;                       // 0x84
};

// PORT: callers pass only (self, flags); the three player slots arrive in a2..a4.
extern "C" void cSSXScriptEngine_BuildCharacterBitRoleArray_def(void* self, int* flags, int p0, int p1, int p2) __asm__("cSSXScriptEngine_BuildCharacterBitRoleArray");

/**
 * Fills @p flags[0..20] with the character bit of the rider in each script role slot, 0 for an empty slot.
 *
 * Riders come from the game's AI rider set (SSXApp->Game->AI); a rider's bit is kCharacterMappingTable[cRider_getIdentity(rider)].
 * Slots 0-2 take the riders at @p mainCharAIIndex, @p supportChar0AIIndex and @p supportChar1AIIndex of the list at +0x28 (skipped when negative); slots 3-4 the first
 * two of the list at +0x40 (count +0x7C); 5-9 the list at +0x48 (count +0x80); 10-14 the list at +0x5C (count +0x84);
 * 15-20 the list at +0x28 in order (count +0x78). The last three lists hold objects whose rider is at +0x18.
 *
 * @param flags  receives 21 character bits
 * @param mainCharAIIndex  the main character's AI index
 * @param supportChar0AIIndex  the first supporting character's AI index
 * @param supportChar1AIIndex  the second supporting character's AI index
 */
// Fills the 21 per-slot team flags of the script engine from the current rider set.
extern "C" void cSSXScriptEngine_BuildCharacterBitRoleArray_def(void* self, int* flags, int mainCharAIIndex, int supportChar0AIIndex, int supportChar1AIIndex)
{
    sRiderSet_BDB8* set = (sRiderSet_BDB8*)SSXApp->Game->AI;
    int numA = set->numA;
    int numB = set->numB;
    int i;
    for (i = 0; i < 21; i++) {
        flags[i] = 0;
        switch (i) {
        case 0:
            if (mainCharAIIndex >= 0)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->players[mainCharAIIndex])];
            break;
        case 1:
            if (supportChar0AIIndex >= 0)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->players[supportChar0AIIndex])];
            break;
        case 2:
            if (supportChar1AIIndex >= 0)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->players[supportChar1AIIndex])];
            break;
        case 3:
        case 4:
            if (i - 3 < numA)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->groupA[i - 3]->rider)];
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            if (i - 5 < numB)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->groupB[i - 5]->rider)];
            break;
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            if (i - 10 < set->numC)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->groupC[i - 10]->rider)];
            break;
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
            if (i - 15 < set->numPlayers)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->players[i - 15])];
            break;
        default:
            break;
        }
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_IsBankLoading);
#ifdef SKIP_ASM
extern "C" void cSSXScriptEngine_UpdateLoad(cSSXScriptEngine*);

/**
 * Tells whether a script bank still has a load pending, polling the loads first.
 *
 * @param i  bank number
 * @return 1 while the bank's async load is pending, otherwise 0
 */
extern "C" int cSSXScriptEngine_IsBankLoading(cSSXScriptEngine* self, int i)
{
    cSSXScriptEngine_UpdateLoad(self);
    return (int)self->mScriptDataBanks[i].asyncHandle != 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_ClearNISPlayListState);
#ifdef SKIP_ASM
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028E888(void*);
extern "C" void cRenderStateMan_setInNIS(int);

/**
 * Marks no NIS list as playing, if one was.
 *
 * When mPlayingNISList is not 2 (none): sets it to 2, clears mNumFramesNoSkip, calls func_0028E888() on the game
 * audio object and cRenderStateMan_setInNIS(0), which clears a render-state pointer and its enabled flag. Does nothing
 * otherwise.
 */
extern "C" void cSSXScriptEngine_ClearNISPlayListState(cSSXScriptEngine* self)
{
    if (self->mPlayingNISList != 2) {
        self->mPlayingNISList = 2;
        self->mNumFramesNoSkip = 0;
        func_0028E888(cAudioInterface_bxAudio());
        cRenderStateMan_setInNIS(0);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjGS_cScriptObjGS);
#ifdef SKIP_ASM
void cScriptEntity_cScriptEntity(void*);
void* cScriptObject_cScriptObject(void*, int);
extern void* D_00482318[];
extern void* cScriptObjGS_vtbl[];

/**
 * Constructs the game-state script object: a script entity plus a type-4 script object.
 *
 * @return @p self
 */
extern "C" void* cScriptObjGS_cScriptObjGS(cScriptObjGS* self)
{
    cScriptEntity_cScriptEntity(self);
    cScriptObject_cScriptObject(&self->mObjType, 4);
    self->mScriptObjectVtbl = D_00482318;
    *(void***)self = cScriptObjGS_vtbl;
    return ((void*)self);
}
#endif

void cScriptObject_OnEndObjectUse(void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjGS_OnEndObjectUse);
#ifdef SKIP_ASM
/**
 * Drops the game-state script object's use count: cScriptObject_OnEndObjectUse on its cScriptObject part (+0xC). The player ID
 * argument is not declared.
 */
extern "C" void cScriptObjGS_OnEndObjectUse(cScriptObjGS* self)
{
    cScriptObject_OnEndObjectUse(&self->mObjType);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjGS_IsMatchingEntity);
#ifdef SKIP_ASM
/**
 * Returns whether the game-state script object takes an InSeq entity: only entity type 6.
 *
 * @param entity  the InSeq player entity (unused)
 * @param type  the entity type
 * @return 1 for type 6, else 0
 */
extern "C" int cScriptObjGS_IsMatchingEntity(cScriptObjGS* self, cScriptPlayerEntity* entity, int type)
{
    return type == 6;
}
#endif

extern "C" void GetSSXGameStateManagerFrame(void*, void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjGS_OnInSeqKeyEvent);
#ifdef SKIP_ASM
/**
 * Handles an in-sequence key event for the game-state script object: fetches the entity's game-state frame and ignores
 * it.
 *
 * Calls GetSSXGameStateManagerFrame(@p entity, frame) with a 16-byte stack buffer and returns; nothing reads the result. On Tour's
 * version (GetSSXGameStateManagerFrame plus asserts on the event bits and frame count) does no more in a release build.
 * Virtual slot 3 of cScriptObjGS_vtbl (the cScriptEntity part); the event type, frame and event bits (On Tour type,
 * currFrame, eventBits) are not used.
 *
 * @param entity  the script player entity the event is for (On Tour cScriptPlayerEntity* pEntity)
 */
extern "C" void cScriptObjGS_OnInSeqKeyEvent(cScriptObjGS* self, void* entity)
{
    char buf[16];
    GetSSXGameStateManagerFrame(entity, buf);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXScriptHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the SSX script header of a script player: its mpDataHeader (+0x14).
 *
 * Callers read the transition frame counts, the bank (+0x1C) and the first byte from it.
 *
 * @return the header
 */
int GetSSXScriptHeader(void* self)
{
    return *(int*)((char*)self + 0x14);
}
#endif

extern "C" void* cScriptPlayerMain_GetHeader(void* self);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXScriptHeader1__FPv);
#ifdef SKIP_ASM
/**
 * Returns the SSX script header of raw script data: cScriptPlayerMain_GetHeader.
 *
 * cSSXScriptEngine_OnAttachData and cSSXScriptEngine_OnDetachData read the bank's script index (+0x18) from it.
 *
 * @return the header, or 0 when the data has none
 */
void* GetSSXScriptHeader1(void* self)
{
    return cScriptPlayerMain_GetHeader(self);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManualHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of a camera script entity: its mpHeader (+0x20).
 *
 * @return the header
 */
int GetSSXCameraManualHeader(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManualHeader1);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

/**
 * Returns the custom header of variation n of a camera script entity's script: cScriptPlayerEntity_GetHeader, with the variation index
 * passing through in the second argument register (the C declares only the entity).
 *
 * @return the header, or 0 when the variation has none
 */
extern "C" void* GetSSXCameraManualHeader1(void* self)
{
    return cScriptPlayerEntity_GetHeader(self);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManualHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in camera script data @p self: finds the entity's
 * block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through in the argument registers) and reads the header with
 * cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXCameraManualHeader2(void* self, void* entityIndex, int variationIndex) {
    cScriptPlayerEntity_GetHeader1(cScriptPlayerMain_GetEntityScriptData(), variationIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManualFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cScriptObjC0F0 {
public:
    int unk_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual float* GetValue();
};

// Same symbol as the unit's cScriptPlayerEntity_GetChannel (cScriptObj002742C8*), viewed with a float getter.
cScriptObjC0F0* cScriptPlayerEntity_GetChannel_C0F0(void* self, int a1) __asm__("cScriptPlayerEntity_GetChannel__FPvi");

/**
 * Reads this frame's values of a camera script entity: @p out[i] is the current value of channel i for i = 0-9
 * (cScriptPlayerEntity_GetChannel, then the channel's virtual slot 4, the value getter).
 *
 * @param out  receives 10 floats
 */
extern "C" void GetSSXCameraManualFrame(void* self, float* out)
{
    out[0] = *cScriptPlayerEntity_GetChannel_C0F0(self, 0)->GetValue();
    out[1] = *cScriptPlayerEntity_GetChannel_C0F0(self, 1)->GetValue();
    out[2] = *cScriptPlayerEntity_GetChannel_C0F0(self, 2)->GetValue();
    out[3] = *cScriptPlayerEntity_GetChannel_C0F0(self, 3)->GetValue();
    out[4] = *cScriptPlayerEntity_GetChannel_C0F0(self, 4)->GetValue();
    out[5] = *cScriptPlayerEntity_GetChannel_C0F0(self, 5)->GetValue();
    out[6] = *cScriptPlayerEntity_GetChannel_C0F0(self, 6)->GetValue();
    out[7] = *cScriptPlayerEntity_GetChannel_C0F0(self, 7)->GetValue();
    out[8] = *cScriptPlayerEntity_GetChannel_C0F0(self, 8)->GetValue();
    out[9] = *cScriptPlayerEntity_GetChannel_C0F0(self, 9)->GetValue();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraTargetHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of a camera script entity: its mpHeader (+0x20).
 *
 * @return the header
 */
int GetSSXCameraTargetHeader(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraTargetHeader1);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

/**
 * Returns the custom header of variation n of a camera script entity's script: cScriptPlayerEntity_GetHeader, with the variation index
 * passing through in the second argument register (the C declares only the entity).
 *
 * @return the header, or 0 when the variation has none
 */
extern "C" void* GetSSXCameraTargetHeader1(void* self)
{
    return cScriptPlayerEntity_GetHeader(self);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraTargetHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in camera script data @p self: finds the entity's
 * block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through in the argument registers) and reads the header with
 * cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXCameraTargetHeader2(void* self, void* entityIndex, int variationIndex) {
    cScriptPlayerEntity_GetHeader1(cScriptPlayerMain_GetEntityScriptData(), variationIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraTargetFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cScriptObjC300 {
public:
    int unk_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual float* GetValue();
};

// Same symbol as the unit's cScriptPlayerEntity_GetChannel (cScriptObj002742C8*), viewed with a float getter.
cScriptObjC300* cScriptPlayerEntity_GetChannel_C300(void* self, int a1) __asm__("cScriptPlayerEntity_GetChannel__FPvi");

/**
 * Reads this frame's values of a camera script entity: @p out[i] is the current value of channel i for i = 0-9
 * (cScriptPlayerEntity_GetChannel, then the channel's virtual slot 4, the value getter).
 *
 * @param out  receives 10 floats
 */
extern "C" void GetSSXCameraTargetFrame(void* self, float* out)
{
    out[0] = *cScriptPlayerEntity_GetChannel_C300(self, 0)->GetValue();
    out[1] = *cScriptPlayerEntity_GetChannel_C300(self, 1)->GetValue();
    out[2] = *cScriptPlayerEntity_GetChannel_C300(self, 2)->GetValue();
    out[3] = *cScriptPlayerEntity_GetChannel_C300(self, 3)->GetValue();
    out[4] = *cScriptPlayerEntity_GetChannel_C300(self, 4)->GetValue();
    out[5] = *cScriptPlayerEntity_GetChannel_C300(self, 5)->GetValue();
    out[6] = *cScriptPlayerEntity_GetChannel_C300(self, 6)->GetValue();
    out[7] = *cScriptPlayerEntity_GetChannel_C300(self, 7)->GetValue();
    out[8] = *cScriptPlayerEntity_GetChannel_C300(self, 8)->GetValue();
    out[9] = *cScriptPlayerEntity_GetChannel_C300(self, 9)->GetValue();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraSubjectHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of a camera script entity: its mpHeader (+0x20).
 *
 * @return the header
 */
int GetSSXCameraSubjectHeader(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraSubjectHeader1);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

/**
 * Returns the custom header of variation n of a camera script entity's script: cScriptPlayerEntity_GetHeader, with the variation index
 * passing through in the second argument register (the C declares only the entity).
 *
 * @return the header, or 0 when the variation has none
 */
extern "C" void* GetSSXCameraSubjectHeader1(void* self)
{
    return cScriptPlayerEntity_GetHeader(self);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraSubjectHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in camera script data @p self: finds the entity's
 * block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through in the argument registers) and reads the header with
 * cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXCameraSubjectHeader2(void* self, void* entityIndex, int variationIndex) {
    cScriptPlayerEntity_GetHeader1(cScriptPlayerMain_GetEntityScriptData(), variationIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraSubjectFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cScriptObjC510 {
public:
    int unk_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual float* GetValue();
};

// Same symbol as the unit's cScriptPlayerEntity_GetChannel (cScriptObj002742C8*), viewed with a float getter.
cScriptObjC510* cScriptPlayerEntity_GetChannel_C510(void* self, int a1) __asm__("cScriptPlayerEntity_GetChannel__FPvi");

/**
 * Reads this frame's values of a camera script entity: @p out[i] is the current value of channel i for i = 0-8
 * (cScriptPlayerEntity_GetChannel, then the channel's virtual slot 4, the value getter).
 *
 * @param out  receives 9 floats
 */
extern "C" void GetSSXCameraSubjectFrame(void* self, float* out)
{
    out[0] = *cScriptPlayerEntity_GetChannel_C510(self, 0)->GetValue();
    out[1] = *cScriptPlayerEntity_GetChannel_C510(self, 1)->GetValue();
    out[2] = *cScriptPlayerEntity_GetChannel_C510(self, 2)->GetValue();
    out[3] = *cScriptPlayerEntity_GetChannel_C510(self, 3)->GetValue();
    out[4] = *cScriptPlayerEntity_GetChannel_C510(self, 4)->GetValue();
    out[5] = *cScriptPlayerEntity_GetChannel_C510(self, 5)->GetValue();
    out[6] = *cScriptPlayerEntity_GetChannel_C510(self, 6)->GetValue();
    out[7] = *cScriptPlayerEntity_GetChannel_C510(self, 7)->GetValue();
    out[8] = *cScriptPlayerEntity_GetChannel_C510(self, 8)->GetValue();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraAlgorithmicHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of a camera script entity: its mpHeader (+0x20).
 *
 * @return the header
 */
int GetSSXCameraAlgorithmicHeader(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraAlgorithmicHeader1);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

/**
 * Returns the custom header of variation n of a camera script entity's script: cScriptPlayerEntity_GetHeader, with the variation index
 * passing through in the second argument register (the C declares only the entity).
 *
 * @return the header, or 0 when the variation has none
 */
extern "C" void* GetSSXCameraAlgorithmicHeader1(void* self)
{
    return cScriptPlayerEntity_GetHeader(self);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraAlgorithmicHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in camera script data @p self: finds the entity's
 * block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through in the argument registers) and reads the header with
 * cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXCameraAlgorithmicHeader2(void* self, void* entityIndex, int variationIndex) {
    cScriptPlayerEntity_GetHeader1(cScriptPlayerMain_GetEntityScriptData(), variationIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraAlgorithmicFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cScriptObjC6F8 {
public:
    int unk_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual float* GetValue();
};

// Same symbol as the unit's cScriptPlayerEntity_GetChannel (cScriptObj002742C8*), viewed with a float getter.
cScriptObjC6F8* cScriptPlayerEntity_GetChannel_C6F8(void* self, int a1) __asm__("cScriptPlayerEntity_GetChannel__FPvi");

/**
 * Reads this frame's values of a camera script entity: @p out[i] is the current value of channel i for i = 0-8
 * (cScriptPlayerEntity_GetChannel, then the channel's virtual slot 4, the value getter).
 *
 * @param out  receives 9 floats
 */
extern "C" void GetSSXCameraAlgorithmicFrame(void* self, float* out)
{
    out[0] = *cScriptPlayerEntity_GetChannel_C6F8(self, 0)->GetValue();
    out[1] = *cScriptPlayerEntity_GetChannel_C6F8(self, 1)->GetValue();
    out[2] = *cScriptPlayerEntity_GetChannel_C6F8(self, 2)->GetValue();
    out[3] = *cScriptPlayerEntity_GetChannel_C6F8(self, 3)->GetValue();
    out[4] = *cScriptPlayerEntity_GetChannel_C6F8(self, 4)->GetValue();
    out[5] = *cScriptPlayerEntity_GetChannel_C6F8(self, 5)->GetValue();
    out[6] = *cScriptPlayerEntity_GetChannel_C6F8(self, 6)->GetValue();
    out[7] = *cScriptPlayerEntity_GetChannel_C6F8(self, 7)->GetValue();
    out[8] = *cScriptPlayerEntity_GetChannel_C6F8(self, 8)->GetValue();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManagerHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of a camera script entity: its mpHeader (+0x20).
 *
 * @return the header
 */
int GetSSXCameraManagerHeader(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManagerHeader1);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

/**
 * Returns the custom header of variation n of a camera script entity's script: cScriptPlayerEntity_GetHeader, with the variation index
 * passing through in the second argument register (the C declares only the entity).
 *
 * @return the header, or 0 when the variation has none
 */
extern "C" void* GetSSXCameraManagerHeader1(void* self)
{
    return cScriptPlayerEntity_GetHeader(self);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManagerHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in camera script data @p self: finds the entity's
 * block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through in the argument registers) and reads the header with
 * cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXCameraManagerHeader2(void* self, void* entityIndex, int variationIndex) {
    cScriptPlayerEntity_GetHeader1(cScriptPlayerMain_GetEntityScriptData(), variationIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManagerFrame);
#ifdef SKIP_ASM
class cScriptObj002742C8 {
public:
    int unk_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual int* GetValue();
    virtual int GetCustom();
};

cScriptObj002742C8* cScriptPlayerEntity_GetChannel(void* self, int a1);

/**
 * Reads this frame's values of a camera-manager script entity from its channel 0: @p out[0] is the current value
 * (virtual slot 4) and @p out[1] the custom word (virtual slot 5) (cScriptPlayerEntity_GetChannel).
 *
 * @param out  receives two words
 */
extern "C" void GetSSXCameraManagerFrame(void* self, int* out)
{
    out[0] = *cScriptPlayerEntity_GetChannel(self, 0)->GetValue();
    out[1] = cScriptPlayerEntity_GetChannel(self, 0)->GetCustom();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXActorHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the actor header of an actor script entity: its mpHeader (+0x20).
 *
 * Rider and script-object code read the anchor type, base rotations and offset and the role flags from it.
 *
 * @return the header
 */
int GetSSXActorHeader(void* self)
{
    return *(int*)((char*)self + 0x20);
}
#endif

extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXActorHeader1__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of variation n of an actor script entity's script: cScriptPlayerEntity_GetHeader, with the variation index
 * passing through in the second argument register (the C declares only the entity).
 *
 * @return the header, or 0 when the variation has none
 */
void* GetSSXActorHeader1(void* self)
{
    return cScriptPlayerEntity_GetHeader(self);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXActorHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in actor script data @p self: finds the entity's
 * block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through in the argument registers) and reads the header with
 * cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXActorHeader2(void* self, void* entityIndex, int variationIndex) {
    cScriptPlayerEntity_GetHeader1(cScriptPlayerMain_GetEntityScriptData(), variationIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXActorFrame);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cScriptObjC9B0 {
public:
    int unk_0x0;
    // vptr at 0x4; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual float* GetValue();
};

cScriptObj002742C8* cScriptPlayerEntity_GetChannel(void* self, int a1);
// Same symbol as the unit's cScriptPlayerEntity_GetChannel (cScriptObj002742C8*), viewed with a float getter.
cScriptObjC9B0* cScriptPlayerEntity_GetChannel_C9B0(void* self, int a1) __asm__("cScriptPlayerEntity_GetChannel__FPvi");

/**
 * Samples an actor script entity's channels into an actor frame.
 *
 * For channels 0..2 (cScriptPlayerEntity_GetChannel__FPvi, On Tour cScriptPlayerEntity::GetChannel) it stores the first word of the channel's
 * virtual slot-4 value and the slot-5 result (out[0..5]); channels 3..8 each give one float through slot 4
 * (out[6..11]). cRider_OnStartObjectUse, cRider_OnInSeqUpdate and cRider_OnInSeqKeyEvent read the rider's scripted pose
 * this way. The first parameter is the cScriptPlayerEntity (On Tour pPlayer), not an object of this function's own.
 *
 * @param out  the tSSXFrameActor to fill (12 words)
 */
extern "C" void GetSSXActorFrame(void* self, void* out)
{
    ((int*)out)[0] = *cScriptPlayerEntity_GetChannel(self, 0)->GetValue();
    ((int*)out)[1] = cScriptPlayerEntity_GetChannel(self, 0)->GetCustom();
    ((int*)out)[2] = *cScriptPlayerEntity_GetChannel(self, 1)->GetValue();
    ((int*)out)[3] = cScriptPlayerEntity_GetChannel(self, 1)->GetCustom();
    ((int*)out)[4] = *cScriptPlayerEntity_GetChannel(self, 2)->GetValue();
    ((int*)out)[5] = cScriptPlayerEntity_GetChannel(self, 2)->GetCustom();
    ((float*)out)[6] = *cScriptPlayerEntity_GetChannel_C9B0(self, 3)->GetValue();
    ((float*)out)[7] = *cScriptPlayerEntity_GetChannel_C9B0(self, 4)->GetValue();
    ((float*)out)[8] = *cScriptPlayerEntity_GetChannel_C9B0(self, 5)->GetValue();
    ((float*)out)[9] = *cScriptPlayerEntity_GetChannel_C9B0(self, 6)->GetValue();
    ((float*)out)[10] = *cScriptPlayerEntity_GetChannel_C9B0(self, 7)->GetValue();
    ((float*)out)[11] = *cScriptPlayerEntity_GetChannel_C9B0(self, 8)->GetValue();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXGameStateManagerFrame);
#ifdef SKIP_ASM
cScriptObj002742C8* cScriptPlayerEntity_GetChannel(void* self, int a1);

/**
 * Reads this frame's values of a game-state script entity from its channel 0: @p out[0] is the current value (virtual
 * slot 4) and @p out[1] the custom word (virtual slot 5) (cScriptPlayerEntity_GetChannel).
 *
 * @param out  receives two words
 */
extern "C" void GetSSXGameStateManagerFrame(void* self, void* out)
{
    ((int*)out)[0] = *cScriptPlayerEntity_GetChannel(self, 0)->GetValue();
    ((int*)out)[1] = cScriptPlayerEntity_GetChannel(self, 0)->GetCustom();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXGameEventFrame);
#ifdef SKIP_ASM
cScriptObj002742C8* cScriptPlayerEntity_GetChannel(void* self, int a1);

/**
 * Reads this frame's values of a game-event script entity: for channels 0-4, @p out[2i] is the current value (virtual
 * slot 4) and @p out[2i + 1] the custom word (virtual slot 5) (cScriptPlayerEntity_GetChannel).
 *
 * @param out  receives ten words
 */
extern "C" void GetSSXGameEventFrame(void* self, void* out)
{
    ((int*)out)[0] = *cScriptPlayerEntity_GetChannel(self, 0)->GetValue();
    ((int*)out)[1] = cScriptPlayerEntity_GetChannel(self, 0)->GetCustom();
    ((int*)out)[2] = *cScriptPlayerEntity_GetChannel(self, 1)->GetValue();
    ((int*)out)[3] = cScriptPlayerEntity_GetChannel(self, 1)->GetCustom();
    ((int*)out)[4] = *cScriptPlayerEntity_GetChannel(self, 2)->GetValue();
    ((int*)out)[5] = cScriptPlayerEntity_GetChannel(self, 2)->GetCustom();
    ((int*)out)[6] = *cScriptPlayerEntity_GetChannel(self, 3)->GetValue();
    ((int*)out)[7] = cScriptPlayerEntity_GetChannel(self, 3)->GetCustom();
    ((int*)out)[8] = *cScriptPlayerEntity_GetChannel(self, 4)->GetValue();
    ((int*)out)[9] = cScriptPlayerEntity_GetChannel(self, 4)->GetCustom();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_cScriptDatabaseObject);
#ifdef SKIP_ASM
struct sScrSlot_CDD0 {
    int a;  // 0x0
    int b;  // 0x4
    int c;  // 0x8
    int d;  // 0xC
    int e;  // 0x10
};

struct sScrEngine_CDD0 {
    int vars[20];               // 0x0
    sScrSlot_CDD0 slots[30];    // 0x50
    void* vtbl;                 // 0x2A8
};

extern char cScriptDatabaseObject_vtbl[];
extern cScriptDatabaseObject* cScriptDatabaseObject_mpDatabaseObject;

/**
 * Constructs the script database: installs cScriptDatabaseObject_vtbl (+0x2A8), makes it the current database
 * (cScriptDatabaseObject_mpDatabaseObject), clears the 20 object slots (mpObjectDatabase) and sets the 30 entity records (mEntityDatabase, 0x14
 * each) to no entity, object -1, player -1, relation 0, reserved player -1.
 *
 * @param self_  the script database (the base of the script engine)
 * @return @p self_
 */
extern "C" void* cScriptDatabaseObject_cScriptDatabaseObject(cScriptDatabaseObject* self_)
{
    sScrEngine_CDD0* self = (sScrEngine_CDD0*)self_;
    int i;
    self->vtbl = cScriptDatabaseObject_vtbl;
    cScriptDatabaseObject_mpDatabaseObject = self_;
    for (i = 19; i >= 0; i--) {
        self->vars[i] = 0;
    }
    for (i = 0; i < 30; i++) {
        self->slots[i].a = 0;
        self->slots[i].b = -1;
        self->slots[i].c = -1;
        self->slots[i].d = 0;
        self->slots[i].e = -1;
    }
    return self;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject__cScriptDatabaseObject);
#ifdef SKIP_ASM
extern "C" void cScriptDatabaseObject_RemoveObjectAll(cScriptDatabaseObject* self);
void operator_delete(int*);
extern char cScriptDatabaseObject_vtbl[];
extern cScriptDatabaseObject* cScriptDatabaseObject_mpDatabaseObject;

/**
 * Destroys the script database: removes every registered object and clears the global database pointer.
 *
 * Restores the cScriptDatabaseObject vtable (cScriptDatabaseObject_vtbl) at +0x2A8, calls cScriptDatabaseObject_RemoveObjectAll() and
 * zeroes cScriptDatabaseObject_mpDatabaseObject (On Tour cScriptDatabaseObject::mpDatabaseObject, the object cScriptDatabaseObject_GetDatabaseObject returns), then frees the
 * object when asked. cBXScriptEngine__cBXScriptEngine chains to it.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptDatabaseObject__cScriptDatabaseObject(cScriptDatabaseObject* self, int flags)
{
    self->mVtbl = (void**)cScriptDatabaseObject_vtbl;
    cScriptDatabaseObject_RemoveObjectAll(self);
    cScriptDatabaseObject_mpDatabaseObject = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_AddObject);
#ifdef SKIP_ASM
extern "C" int cScriptDatabaseObject_AddEntity(cScriptDatabaseObject* self, int idx, void* item);

class cScriptOwnerCEA8 {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int GetNumScriptEntities();
    virtual void* GetScriptEntity(int i);
};

struct sScrOwnersCEA8 {
    cScriptOwnerCEA8* owners[20];
};

/**
 * Adds a script object to the database and registers its script entities; returns the object's ID.
 *
 * Stores @p scriptObject in the first free of the 20 mpObjectDatabase slots, then registers each of its script entities (its
 * virtual GetScriptEntity, slot 7, for 0 to its slot 6 count) with cScriptDatabaseObject_AddEntity under that ID.
 *
 * Likely bug: with all 20 slots taken the object is not stored, yet its entities are registered under ID 20 and 20 is
 * returned.
 *
 * @param engine  the script database (the script engine)
 * @param scriptObject  the script object
 * @return the object ID (slot index)
 */
extern "C" int cScriptDatabaseObject_AddObject(cScriptDatabaseObject* engine, void* scriptObject)
{
    sScrOwnersCEA8* self = (sScrOwnersCEA8*)engine;
    cScriptOwnerCEA8* obj = (cScriptOwnerCEA8*)scriptObject;
    int i;
    for (i = 0; i < 20; i++) {
        if (self->owners[i] == 0) {
            self->owners[i] = obj;
            break;
        }
    }
    int k = 0;
    int n = obj->GetNumScriptEntities();
    for (; k < n; k++) {
        cScriptDatabaseObject_AddEntity((cScriptDatabaseObject*)self, i, obj->GetScriptEntity(k));
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_RemoveObject);
#ifdef SKIP_ASM
extern "C" int cScriptDatabaseObject_RemoveEntity(cScriptDatabaseObject* self, void* msg);

class cScriptOwnerCF88 {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int GetNumScriptEntities();
    virtual void* GetScriptEntity(int i);
};

/**
 * Removes the script object with ID @p idx from the database, unregistering its script entities.
 *
 * Each of the object's entities (its virtual GetScriptEntity, slot 7, for 0 to its slot 6 count) is removed with
 * cScriptDatabaseObject_RemoveEntity, then the mpObjectDatabase slot is cleared.
 *
 * @param idx  the object ID
 * @return 1, or 0 when the slot was empty
 */
extern "C" int cScriptDatabaseObject_RemoveObject(cScriptDatabaseObject* self, int idx)
{
    cScriptOwnerCF88* obj = *(cScriptOwnerCF88**)((char*)self + (idx << 2));
    if (obj == 0) return 0;
    int k = 0;
    int n = obj->GetNumScriptEntities();
    for (; k < n; k++) {
        cScriptDatabaseObject_RemoveEntity(self, obj->GetScriptEntity(k));
    }
    *(cScriptOwnerCF88**)((char*)self + (idx << 2)) = 0;
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_RemoveObjectAll);
#ifdef SKIP_ASM

/**
 * Removes every object registered in the script object database.
 *
 * For each of the 20 mpObjectDatabase slots that holds an object it calls virtual slot 3 (vtable at +0x2A8) with
 * the slot index. cScriptDatabaseObject's own slot 3 (cScriptDatabaseObject_RemoveObject) unregisters the object's script entities
 * and clears the slot; cSSXScriptEngine overrides it with cSSXScriptEngine_RemoveObject.
 */
extern "C" void cScriptDatabaseObject_RemoveObjectAll(cScriptDatabaseObject* self)
{
    int i;
    for (i = 0; i < 20; i++) {
        if (((int*)self)[i] != 0) {
            sVEntry<int(void*, int)>* vt = (sVEntry<int(void*, int)>*)self->mVtbl;
            vt[3].fn(vthis(self, vt[3]), i);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_MarkObjectAsReserved);
#ifdef SKIP_ASM
extern "C" void cScriptDatabaseObject_MarkEntityAsReserved(cScriptDatabaseObject* self, void* a1, int a2);

class cScriptOwnerD0C0 {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int GetNumScriptEntities();
    virtual void* GetScriptEntity(int i);
};

/**
 * Reserves every script entity of object @p idx for player @p val (cScriptDatabaseObject_MarkEntityAsReserved on each of the object's virtual
 * GetScriptEntity results).
 *
 * Quirk: an empty object slot is dereferenced unchecked.
 *
 * @param idx  the object ID
 * @param val  the player ID
 */
extern "C" void cScriptDatabaseObject_MarkObjectAsReserved(cScriptDatabaseObject* self, int idx, int val)
{
    cScriptOwnerD0C0* obj = (cScriptOwnerD0C0*)self->mpObjectDatabase[idx];
    int i = 0;
    int n = obj->GetNumScriptEntities();
    for (; i < n; i++) {
        cScriptDatabaseObject_MarkEntityAsReserved(self, obj->GetScriptEntity(i), val);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_MarkObjectAsUnreserved);
#ifdef SKIP_ASM
class cScriptOwnerD170 {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int GetNumScriptEntities();
    virtual void* GetScriptEntity(int i);
};

extern "C" void cScriptDatabaseObject_MarkEntityAsUnreserved(cScriptDatabaseObject* self, void* a1);

/**
 * Clears the reservation of every script entity of object @p idx (cScriptDatabaseObject_MarkEntityAsUnreserved on each of the object's virtual
 * GetScriptEntity results).
 *
 * @param idx  the object ID
 */
extern "C" void cScriptDatabaseObject_MarkObjectAsUnreserved(cScriptDatabaseObject* self, int idx)
{
    cScriptOwnerD170* obj = (cScriptOwnerD170*)self->mpObjectDatabase[idx];
    int i = 0;
    int n = obj->GetNumScriptEntities();
    for (; i < n; i++) {
        cScriptDatabaseObject_MarkEntityAsUnreserved(self, obj->GetScriptEntity(i));
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_IsObjInUseByPlayer);
#ifdef SKIP_ASM
extern "C" int cScriptDatabaseObject_IsEntityInUseByPlayer(cScriptDatabaseObject* self, int i, int val);

class cScriptOwnerD210 {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    // vptr at 0xC; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual int GetNumScriptEntities();
    virtual int* GetScriptEntity(int i);
};

// PORT: cSSXScriptEngine_IsObjInUseByPlayer declares cScriptDatabaseObject_IsObjInUseByPlayer as void; it really
// returns 0/1.
int cScriptDatabaseObject_IsObjInUseByPlayer_impl(void* self, int idx, int val) __asm__("cScriptDatabaseObject_IsObjInUseByPlayer");

/**
 * Returns whether any script entity of object @p idx is in use by player @p val (cScriptDatabaseObject_IsEntityInUseByPlayer on each entity's
 * database ID, +0x4).
 *
 * @param idx  the object ID
 * @param val  the player ID
 * @return 1 when one is, else 0
 */
int cScriptDatabaseObject_IsObjInUseByPlayer_impl(void* vself, int idx, int val)
{
    cScriptDatabaseObject* self = (cScriptDatabaseObject*)vself;
    cScriptOwnerD210* obj = (cScriptOwnerD210*)self->mpObjectDatabase[idx];
    int i = 0;
    int n = obj->GetNumScriptEntities();
    for (; i < n; i++) {
        if (cScriptDatabaseObject_IsEntityInUseByPlayer((cScriptDatabaseObject*)self, obj->GetScriptEntity(i)[1], val)) {
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_IsEntityInUseByPlayer);
#ifdef SKIP_ASM
/**
 * Returns whether entity record @p i of the database (mEntityDatabase, 0x14 each) is used by player @p val (its player
 * ID, +0x8 of the record).
 *
 * @param i  the entity's database ID
 * @param val  the player ID
 * @return 1 when it is, else 0
 */
extern "C" int cScriptDatabaseObject_IsEntityInUseByPlayer(cScriptDatabaseObject* self, int i, int val)
{
    char* record = (char*)self + i * 0x14;
    return *(int*)(record + 0x58) == val;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_GetDatabaseObject);
#ifdef SKIP_ASM
extern cScriptDatabaseObject* cScriptDatabaseObject_mpDatabaseObject;

/**
 * Returns the current script database object, cScriptDatabaseObject_mpDatabaseObject (set by its constructor cScriptDatabaseObject_cScriptDatabaseObject).
 *
 * @return the database
 */
extern "C" cScriptDatabaseObject* cScriptDatabaseObject_GetDatabaseObject()
{
    return cScriptDatabaseObject_mpDatabaseObject;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_GetPlayerIDByEntity);
#ifdef SKIP_ASM


/**
 * Returns the player using a script entity: its record's player ID (+0x8) when the record's relation (+0xC) is 1
 * (used), else -1. The record is mEntityDatabase[the entity's database ID, +0x4].
 *
 * @param entity  the script entity
 * @return the player ID, or -1
 */
extern "C" int cScriptDatabaseObject_GetPlayerIDByEntity(cScriptDatabaseObject* self, void* entity)
{
    tScriptEntityRecord* record = &((&self->mEntityDatabase[0]))[*(int*)((char*)entity + 0x4)];
    if (record->relation != 1) {
        return -1;
    }
    return record->playerID;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_GetObject__FPvi);
#ifdef SKIP_ASM
/**
 * Returns the script object with ID @p i: mpObjectDatabase[@p i].
 *
 * @param i  the object ID
 * @return the object, or 0
 */
int cScriptDatabaseObject_GetObject(void* vself, int i)
{
    cScriptDatabaseObject* self = (cScriptDatabaseObject*)vself;
    return (int)self->mpObjectDatabase[i];
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_AddEntity);
#ifdef SKIP_ASM
/**
 * Registers a script entity of object @p idx in the first free of the 30 entity records and gives the entity that
 * record's index as its database ID (+0x4).
 *
 * Likely bug: with all 30 records taken nothing is stored and 30 is returned, leaving the entity's ID unchanged.
 *
 * @param idx  the owning object's ID
 * @param item  the script entity
 * @return the record index
 */
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" int cScriptDatabaseObject_AddEntity(cScriptDatabaseObject* self, int idx, void* item)
{
    int i;
    for (i = 0; i < 30; i++) {
        int off = i * 20 + 80;
        if (*(void**)((char*)self + off) == 0) {
            *(int*)((char*)self + off + 4) = idx;
            *(void**)((char*)self + off) = item;
            ((int*)item)[1] = i;
            break;
        }
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_RemoveEntity);
#ifdef SKIP_ASM
extern "C" void cScriptEntity_DetachEntity(void* self);


/**
 * Unregisters a script entity: frees its record (mEntityDatabase[its database ID, +0x4]).
 *
 * An entity marked used (relation 1) is first cut off from its InSeq entity (cScriptEntity_DetachEntity). The entity's database ID
 * is set to -1 and the record cleared (no entity, object -1, player -1, relation 0, reserved -1).
 *
 * @param msg  the script entity
 * @return always 1
 */
extern "C" int cScriptDatabaseObject_RemoveEntity(cScriptDatabaseObject* self, void* msg)
{
    tScriptEntityRecord* record = &self->mEntityDatabase[*(int*)((char*)msg + 0x4)];
    if (record->relation == 1) {
        cScriptEntity_DetachEntity(msg);
    }
    ((int*)record->pEntity)[1] = -1;
    record->objID = -1;
    (*(int**)&record->pEntity) = 0;
    record->playerID = -1;
    record->relation = 0;
    record->reservedPlayerID = -1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_MarkEntityAsUsed);
#ifdef SKIP_ASM

struct sScrVEntryD400b {
    short delta;
    short index;
    void (*fn)(void*, int);
};


struct sScrOwnerD400 {
    void* objs[0x14];           // 0x0
    tScriptEntityRecord slots[1];      // 0x50
};

/**
 * Marks a script entity as used by player @p value.
 *
 * When the entity's object is not yet in use by that player (the database's virtual slot 4, IsObjInUseByPlayer-style,
 * cScriptDatabaseObject_IsObjInUseByPlayer in the base), the object's own virtual slot 4 (OnStartObjectUse) is called with the player. Then the
 * record (mEntityDatabase[@p key's database ID, +0x4]) takes the player ID (+0x8) and relation 1 (+0xC).
 *
 * @param key  the script entity
 * @param value  the player ID
 */
extern "C" void cScriptDatabaseObject_MarkEntityAsUsed(sScrOwnerD400* self, int* key, int value)
{
    sVEntry<int(void*, int, int)>* vt = *(sVEntry<int(void*, int, int)>**)((char*)self + 0x2A8);
    tScriptEntityRecord* record = &self->slots[key[1]];
    if (vt[4].fn(vthis(self, vt[4]), record->objID, value) == 0) {
        char* obj = (char*)self->objs[record->objID];
        sScrVEntryD400b* vt2 = *(sScrVEntryD400b**)(obj + 0xC);
        vt2[4].fn(vthis(obj, vt2[4]), value);
    }
    record->playerID = value;
    record->relation = 1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_MarkEntityAsFree);
#ifdef SKIP_ASM
struct sScrVEntryD4A0 {
    short delta;
    short index;
    int (*fn)(void*, int, int);
};

struct sScrVEntryD4A0b {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sScrSlotD4A0 {
    int field_0x0;
    int id;         // 0x4
    int value;      // 0x8
    int set;        // 0xC
    int field_0x10;
};

struct sScrOwnerD4A0 {
    void* objs[0x14];           // 0x0
    sScrSlotD4A0 slots[1];      // 0x50
};

/**
 * Marks a script entity as free again.
 *
 * Clears its record's player ID (-1) and relation (0); when the object is no longer in use by the old player (the
 * database's virtual slot 4, cScriptDatabaseObject_IsObjInUseByPlayer in the base), the object's virtual slot 5 (OnEndObjectUse) is called with
 * that player.
 *
 * @param key  the script entity
 */
extern "C" void cScriptDatabaseObject_MarkEntityAsFree(sScrOwnerD4A0* self, int* key)
{
    sScrSlotD4A0* record = &self->slots[key[1]];
    int old = record->value;
    record->value = -1;
    record->set = 0;
    sScrVEntryD4A0* vt = *(sScrVEntryD4A0**)((char*)self + 0x2A8);
    if (vt[4].fn(vthis(self, vt[4]), record->id, old) == 0) {
        char* obj = (char*)self->objs[record->id];
        sScrVEntryD4A0b* vt2 = *(sScrVEntryD4A0b**)(obj + 0xC);
        vt2[5].fn(vthis(obj, vt2[5]), old);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_MarkEntityAsReserved);
#ifdef SKIP_ASM
/**
 * Reserves a script entity for player @p playerId: its record's reserved player ID (+0x10).
 *
 * @param entity  the script entity
 * @param playerId  the player ID
 */
extern "C" void cScriptDatabaseObject_MarkEntityAsReserved(cScriptDatabaseObject* self, void* entity, int playerId)
{
    int i = *(int*)((char*)entity + 0x4);
    char* record = (char*)self + i * 0x14;
    *(int*)(record + 0x60) = playerId;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_MarkEntityAsUnreserved);
#ifdef SKIP_ASM
/**
 * Clears a script entity's reservation: its record's reserved player ID (+0x10) becomes -1.
 *
 * @param entity  the script entity
 */
extern "C" void cScriptDatabaseObject_MarkEntityAsUnreserved(cScriptDatabaseObject* self, void* entity)
{
    int i = *(int*)((char*)entity + 0x4);
    char* record = (char*)self + i * 0x14;
    *(int*)(record + 0x60) = -1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_GetAvailableEntities);
#ifdef SKIP_ASM
/**
 * Lists up to @p max script entities available to player @p key: registered, not in use (relation not 1) and not
 * reserved for another player (reserved ID negative or @p key).
 *
 * @param key  the player ID
 * @param out  receives the entities
 * @param max  the most to list
 * @return the number listed
 */
extern "C" int cScriptDatabaseObject_GetAvailableEntities(cScriptDatabaseObject* self, int key, int* out, int max)
{
    int n = 0;
    int i = 0;
    tScriptEntityRecord* record = ((&self->mEntityDatabase[0]));
    while (i < 30 && n != max) {
        if (((int)record->pEntity) != 0 && record->relation != 1 && (record->reservedPlayerID < 0 || record->reservedPlayerID == key)) {
            *out++ = ((int)record->pEntity);
            n++;
        }
        i++;
        if (i < 30) record = &((&self->mEntityDatabase[0]))[i];
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_cScriptObjCamera);
#ifdef SKIP_ASM
void* cScriptObject_cScriptObject(void*, int);
extern char cScriptObjCamera_vtbl[];
extern char t4mat_Identity[];

/**
 * Constructs a script camera object for one view.
 *
 * Builds the cScriptObject base with object type 1 (cScriptObject_cScriptObject), installs cScriptObjCamera_vtbl
 * (+0xC), clears the camera manager, camera entity and weight words (+0x10..+0x18), sets the player ID (+0x20) to -1,
 * copies t4mat_Identity into the base transform (+0x30, VU0 quad copy), zeroes the base rotations (+0x70..+0x78) and
 * stores the view index (+0x7C).
 *
 * @param viewIndex  the view (viewport) index
 * @return the object (self)
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy of t4mat_Identity, likely identity);
// the PC port needs a plain 64-byte copy.
extern "C" void* cScriptObjCamera_cScriptObjCamera(cScriptObjCamera* self, int viewIndex)
{
    cScriptObject_cScriptObject(self, 1);
    self->mVtbl = (void**)cScriptObjCamera_vtbl;
    self->mPlayerID = -1;
    self->mpCameraManager = 0;
    self->mpCameraEntity = 0;
    *(int*)&self->mWeight = 0;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"((char*)&self->mBaseTransform), "r"(t4mat_Identity)
        : "memory");
    self->mViewIndex = viewIndex;
    *(int*)&self->mBaseRotX = 0;
    *(int*)&self->mBaseRotY = 0;
    *(int*)&self->mBaseRotZ = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_SetCameraManager__FPvi);
#ifdef SKIP_ASM
/**
 * Sets a script camera object's camera manager (+0x10).
 *
 * @param val  the camera manager, or 0
 */
void cScriptObjCamera_SetCameraManager(void* vself, int val)
{
    cScriptObjCamera* self = (cScriptObjCamera*)vself;
    self->mpCameraManager = (cScriptObjCameraManager*)val;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_SetCameraEntity);
#ifdef SKIP_ASM
extern char t4mat_Identity[];


/**
 * Binds a script camera object to camera entity @p anim, or unbinds it when @p anim is 0.
 *
 * Binding: the player using the entity (cScriptDatabaseObject_GetPlayerIDByEntity on the database) becomes the object's player (+0x20); an entity
 * already bound (+0x14) first ends its use with the object's virtual slot 5 (OnEndObjectUse) for the old player, and
 * the new use starts with slot 4 (OnStartObjectUse). Unbinding a bound object resets the base transform (+0x30) to
 * t4mat_Identity and the base rotations (+0x70-+0x78) to 0, ends the use (slot 5), and clears the entity and player
 * (-1).
 *
 * @param anim  the camera entity (cScriptEntityCamera), or 0
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy of t4mat_Identity, likely identity);
// the PC port needs a plain 64-byte copy.
extern "C" void cScriptObjCamera_SetCameraEntity(cScriptObjCamera* self, void* anim)
{
    if (anim != 0) {
        int id = cScriptDatabaseObject_GetPlayerIDByEntity((cScriptDatabaseObject*)(cScriptDatabaseObject_GetDatabaseObject()), anim);
        if (self->mpCameraEntity != 0) {
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)self->mVtbl;
            vt[5].fn(vthis(self, vt[5]), self->mPlayerID);
        }
        self->mpCameraEntity = (cScriptEntityCamera*)anim;
        self->mPlayerID = id;
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)self->mVtbl;
        vt[4].fn(vthis(self, vt[4]), id);
    } else if (self->mpCameraEntity != 0) {
        __asm__ __volatile__(
            ".set noreorder\n"
            "lqc2      $vf1, 0x0(%1)\n"
            "lqc2      $vf2, 0x10(%1)\n"
            "lqc2      $vf3, 0x20(%1)\n"
            "lqc2      $vf4, 0x30(%1)\n"
            "sqc2      $vf1, 0x0(%0)\n"
            "sqc2      $vf2, 0x10(%0)\n"
            "sqc2      $vf3, 0x20(%0)\n"
            "sqc2      $vf4, 0x30(%0)\n"
            ".set reorder\n"
            :
            : "r"((char*)&self->mBaseTransform), "r"(t4mat_Identity)
            : "memory");
        *(int*)&self->mBaseRotX = 0;
        *(int*)&self->mBaseRotY = 0;
        *(int*)&self->mBaseRotZ = 0;
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)self->mVtbl;
        vt[5].fn(vthis(self, vt[5]), self->mPlayerID);
        self->mpCameraEntity = 0;
        self->mPlayerID = -1;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_SetCameraWeight__FPvf);
#ifdef SKIP_ASM
/**
 * Sets a script camera object's blend weight (float +0x18).
 *
 * @param val  the weight (0-1)
 */
void cScriptObjCamera_SetCameraWeight(void* vself, float val)
{
    cScriptObjCamera* self = (cScriptObjCamera*)vself;
    self->mWeight = val;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_IsInUseByPlayer);
#ifdef SKIP_ASM
extern "C" cScriptDatabaseObject* cScriptDatabaseObject_GetDatabaseObject(void);
extern "C" int cScriptDatabaseObject_IsEntityInUseByPlayer(cScriptDatabaseObject* self, int i, int val);

/**
 * Returns whether the camera entity bound to this object (+0x14) is in use by player @p val (cScriptDatabaseObject_IsEntityInUseByPlayer on the
 * database with the entity's database ID).
 *
 * @param val  the player ID
 * @return 1 when it is, 0 otherwise or when no entity is bound
 */
extern "C" int cScriptObjCamera_IsInUseByPlayer(cScriptObjCamera* self, int val)
{
    if (self->mpCameraEntity == 0) {
        return 0;
    }
    return cScriptDatabaseObject_IsEntityInUseByPlayer(cScriptDatabaseObject_GetDatabaseObject(), self->mpCameraEntity->mDBID, val);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_PauseObjScript);
#ifdef SKIP_ASM
class cSsxScriptVirt {
public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    virtual void v01();
    virtual void PauseObjScript();
    virtual void ResumeObjScript();
};

/**
 * Pauses the camera object's script: calls the second virtual of the object at +0x10 (On Tour mpCameraManager, a
 * cScriptEntity; presumably PauseEntityScript).
 */
extern "C" void cScriptObjCamera_PauseObjScript(cScriptObjCamera* self)
{
    ((cSsxScriptVirt*)self->mpCameraManager)->PauseObjScript();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_ResumeObjScript);
#ifdef SKIP_ASM
/**
 * Resumes the camera object's script: calls the third virtual of the object at +0x10 (On Tour mpCameraManager, a
 * cScriptEntity; presumably ResumeEntityScript).
 */
extern "C" void cScriptObjCamera_ResumeObjScript(cScriptObjCamera* self)
{
    ((cSsxScriptVirt*)self->mpCameraManager)->ResumeObjScript();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_OnStartObjectUse);
#ifdef SKIP_ASM
void cScriptObject_OnStartObjectUse(void* self);
extern "C" void cScriptObjCamera_CalculateBaseTransform(void* self, int a1);
extern "C" void cScriptCameraController_addCamera(void* ctrl, void* script);
extern "C" void cCamera_setCurrentController(void* self, int mode);

/**
 * Starts using a script camera: hands it to the script camera controller of the camera it belongs to.
 *
 * First counts the use with the base handler cScriptObject_OnStartObjectUse (use count at +0x8). Only while a game exists: sets
 * the float at +0x18 to 1.0, builds the camera's matrix and angles from its script record (cScriptObjCamera_CalculateBaseTransform), adds
 * this object to the cScriptCameraController (+0xAC) of the view manager's camera whose index is in the word at
 * +0x7C, and makes controller 3 current on that camera when the controller's word at +0x18 is set.
 */
extern "C" void cScriptObjCamera_OnStartObjectUse(cScriptObjCamera* self)
{
    cScriptObject_OnStartObjectUse(self);
    if ((void*)SSXApp->Game != 0) {
        self->mWeight = 1.0f;
        cScriptObjCamera_CalculateBaseTransform(self, (int)self->mpCameraEntity);
        char* riders = (char*)SSXApp->Game->ViewMan;
        cCamera* rider = (cCamera*)(*(char**)(riders + (self->mViewIndex << 2) + 4));
        cScriptCameraController_addCamera(rider->ScriptController, self);
        if (*(int*)((char*)rider->ScriptController + 0x18) != 0) {
            cCamera_setCurrentController(rider, 3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_OnEndObjectUse);
#ifdef SKIP_ASM
void cScriptObject_OnEndObjectUse(void*);
extern "C" void cScriptCameraController_removeCamera(void* self, void* algo);
extern "C" void cCamera_setCurrentController(void* self, int mode);
extern "C" void* cChaseCameraController_switchCamera_Restart(void* self);

class cScrObj_D8D8 {
public:
    char pad[0x14];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void snapCurrent();
};

/**
 * Releases a script camera when its sequence stops using it, returning the view's camera to the chase camera when
 * no script camera is left.
 *
 * Drops the use count (cScriptObject_OnEndObjectUse). During a game it clears mWeight (+0x18), takes the camera of view mViewIndex
 * (+0x7C) from the game's ViewMan, removes this object from the camera's ScriptController (cScriptCameraController_removeCamera) and, when
 * that controller's count (+0x18) reaches 0, switches the camera to controller 2 (chase), restarts the chase camera
 * and calls the current Controller's slot 4. The local view calls the camera a rider. On Tour passes a playerID;
 * this C takes none.
 */
extern "C" void cScriptObjCamera_OnEndObjectUse(cScriptObjCamera* self)
{
    cScriptObject_OnEndObjectUse(self);
    cSSXApp* app = SSXApp;
    if (app->Game != 0) {
        *(int*)&self->mWeight = 0;
        char* riders = (char*)app->Game->ViewMan;
        cCamera* rider = (cCamera*)(*(char**)(riders + (self->mViewIndex << 2) + 4));
        cScriptCameraController_removeCamera(rider->ScriptController, self);
        if (*(int*)((char*)rider->ScriptController + 0x18) == 0) {
            cCamera_setCurrentController(rider, 2);
            cChaseCameraController_switchCamera_Restart(rider->ChaseController);
            ((cScrObj_D8D8*)rider->Controller)->snapCurrent();
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_CalculateBaseTransform);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// Matrix helpers and the camera script object shared by cScriptObjCamera_CalculateBaseTransform and
// cScriptObjRider_OnStartObjectUse.

struct sRotMat_SE {
    float m[4][4];
} __attribute__((aligned(16)));

struct sVec4_SE {
    sVec4_SE() {}
    void set(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
    float x, y, z, w;
} __attribute__((aligned(16)));

extern "C" void fSinCos(float* s, float* c, float angle);

static inline void VSet_SE(sVec4_SE* v, float x, float y, float z, float w)
{
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
}

// PORT: PS2-only VU0 inline asm (4x4 matrix copy).
static inline void CopyMat_SE(sRotMat_SE* dst, const void* src)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(src)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply: top = top * r).
static inline void MulTop_SE(sRotMat_SE* top, sRotMat_SE* r)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        ".set reorder\n"
        :
        : "r"(top), "r"(r)
        : "memory");
    __asm__ __volatile__(
        ".set noreorder\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(top)
        : "memory");
}

// PORT: PS2-only VU0 inline asm (4x4 matrix multiply in one block: top = top * r).
static inline void MulMat_SE(sRotMat_SE* top, sRotMat_SE* r)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(top), "r"(r)
        : "memory");
}

// Multiply the matrix at TOP by a rotation of ANGLE about AXIS (an inlined Rotate():
// the axis argument is taken once on entry, before the sin/cos call).
#define ROTATE_SE(TOP, AXIS, ANGLE) \
    { \
        const float* axis = (AXIS); \
        sRotMat_SE r; \
        float s; \
        float c; \
        fSinCos(&s, &c, (ANGLE)); \
        float t = 1.0f - c; \
        float x = axis[0]; \
        float y = axis[1]; \
        float z = axis[2]; \
        float tx = t * x; \
        float ty = t * y; \
        float tz = t * z; \
        float sx = s * x; \
        float sy = s * y; \
        float sz = s * z; \
        r.m[0][0] = tx * x + c; \
        r.m[1][0] = tx * y + sz; \
        r.m[2][0] = tx * z - sy; \
        r.m[0][1] = ty * x - sz; \
        r.m[1][1] = ty * y + c; \
        r.m[2][1] = ty * z + sx; \
        r.m[0][2] = tz * x + sy; \
        r.m[1][2] = tz * y - sx; \
        r.m[2][2] = tz * z + c; \
        r.m[3][0] = 0.0f; \
        r.m[3][1] = 0.0f; \
        r.m[3][2] = 0.0f; \
        r.m[0][3] = 0.0f; \
        r.m[1][3] = 0.0f; \
        r.m[2][3] = 0.0f; \
        r.m[3][3] = 1.0f; \
        MulTop_SE((TOP), &r); \
    }

// PORT: PS2-only VU0 inline asm (matrix * vector into row 3).
static inline void Translate_SE(sRotMat_SE* top, sVec4_SE* v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(top), "m"(*v)
        : "memory");
}

// The camera script object (cScriptObjCamera_CalculateBaseTransform and cScriptObjRider_OnStartObjectUse set up its
// matrix).
struct sScrCam_SE {
    char pad0[0x8];
    char* owner;            // 0x8
    char padC[0x1C - 0xC];
    void* f1C;              // 0x1C
    void* f20;              // 0x20
    char pad24[0x30 - 0x24];
    sRotMat_SE mat;         // 0x30
    float rx;               // 0x70
    float ry;               // 0x74
    float rz;               // 0x78
};

unsigned char cScriptPlayerEntity_GetType(void* self);
char* GetSSXCameraManualHeader_D970(void* self) __asm__("GetSSXCameraManualHeader__FPv");
char* GetSSXCameraTargetHeader_D970(void* self) __asm__("GetSSXCameraTargetHeader__FPv");
char* GetSSXCameraSubjectHeader_D970(void* self) __asm__("GetSSXCameraSubjectHeader__FPv");
char* GetSSXCameraAlgorithmicHeader_D970(void* self) __asm__("GetSSXCameraAlgorithmicHeader__FPv");
extern char t4mat_Identity[];
extern float D_004FF140_D970[] __asm__("t4vect_XAxis");
extern float D_004FF150_D970[] __asm__("t4vect_YAxis");
extern float D_004FF160_D970[] __asm__("t4vect_ZAxis");
extern "C" void cSSXScriptEngine_GetAnchorOrientation_f(void* self, int type, void* a2, sVec4_SE* out, float* roll, float* pitch, float* yaw) __asm__("cSSXScriptEngine_GetAnchorOrientation");

// One camera case: offset + euler angles from the record, then the player's frame on top.
#define CAMARM_SE(SELF, OWNER, REC) \
    { \
        sVec4_SE v; \
        sRotMat_SE M; \
        (SELF)->rx = *(float*)((REC) + 0x1C) * 0.01745329424738884f; \
        (SELF)->ry = *(float*)((REC) + 0x18) * 0.01745329424738884f; \
        (SELF)->rz = *(float*)((REC) + 0x14) * 0.01745329424738884f; \
        VSet_SE(&v, *(float*)((REC) + 8), *(float*)((REC) + 0xC), *(float*)((REC) + 0x10), 1.0f); \
        sRotMat_SE* m = &M; \
        CopyMat_SE(m, t4mat_Identity); \
        Translate_SE(m, &v); \
        ROTATE_SE(m, D_004FF160_D970, -(SELF)->rz); \
        ROTATE_SE(m, D_004FF150_D970, (SELF)->ry); \
        ROTATE_SE(m, D_004FF140_D970, (SELF)->rx); \
        sVec4_SE out; \
        float roll; \
        float pitch; \
        float yaw; \
        cSSXScriptEngine_GetAnchorOrientation_f(ScriptEngine, *(char*)((REC) + 0x20), *(void**)((OWNER) + 8), &out, &roll, &pitch, &yaw); \
        Translate_SE(&(SELF)->mat, &out); \
        ROTATE_SE(&(SELF)->mat, D_004FF160_D970, -yaw); \
        ROTATE_SE(&(SELF)->mat, D_004FF150_D970, pitch); \
        ROTATE_SE(&(SELF)->mat, D_004FF140_D970, roll); \
        MulMat_SE(&(SELF)->mat, m); \
        (SELF)->rx = (SELF)->rx + roll; \
        (SELF)->ry = (SELF)->ry + pitch; \
        (SELF)->rz = (SELF)->rz + yaw; \
    }

// Declared earlier in the unit as (void*, int).
extern "C" void cScriptObjCamera_CalculateBaseTransform_SE(sScrCam_SE* self, sScrCam_SE* cam) __asm__("cScriptObjCamera_CalculateBaseTransform");

/**
 * Builds a script camera object's base transform (+0x30) and base rotations (+0x70, +0x74, +0x78) from the header of
 * camera entity @p cam.
 *
 * Starts from t4mat_Identity and zero angles, then by the type of the entity's InSeq entity (+0x8, cScriptPlayerEntity_GetType): -
 * 0-2 (header from GetSSXCameraManualHeader, GetSSXCameraTargetHeader or GetSSXCameraSubjectHeader): the header's X, Y and Z angles in degrees (+0x1C,
 * +0x18, +0x14) and offset (+0x8-+0x10) form a local matrix (translate, then about Z by -z, Y, X). The anchor of type
 * header byte +0x20 (cSSXScriptEngine_GetAnchorOrientation for the entity's owner player) gives a position and angles; the transform becomes
 * the anchor's translate and rotations (Z by -angle, Y, X) times the local matrix, and the anchor angles are added. - 3
 * (header from GetSSXCameraAlgorithmicHeader): only the header's angles (+0x10, +0xC, +0x8) as rotations about Z (negated), Y, X. Other
 * types keep the identity.
 *
 * @param cam  the camera entity (cScriptEntityCamera)
 */
extern "C" void cScriptObjCamera_CalculateBaseTransform_SE(sScrCam_SE* self, sScrCam_SE* cam)
{
    char* owner = cam->owner;
    CopyMat_SE(&self->mat, t4mat_Identity);
    self->rx = 0.0f;
    self->ry = 0.0f;
    self->rz = 0.0f;
    switch (cScriptPlayerEntity_GetType(owner)) {
    case 0: {
        char* rec = GetSSXCameraManualHeader_D970(owner);
        CAMARM_SE(self, owner, rec);
        break;
    }
    case 1: {
        char* rec = GetSSXCameraTargetHeader_D970(owner);
        CAMARM_SE(self, owner, rec);
        break;
    }
    case 2: {
        char* rec = GetSSXCameraSubjectHeader_D970(owner);
        CAMARM_SE(self, owner, rec);
        break;
    }
    case 3: {
        sRotMat_SE* mat = &self->mat;
        char* rec = GetSSXCameraAlgorithmicHeader_D970(owner);
        self->rx = *(float*)(rec + 0x10) * 0.01745329424738884f;
        self->ry = *(float*)(rec + 0xC) * 0.01745329424738884f;
        self->rz = *(float*)(rec + 8) * 0.01745329424738884f;
        ROTATE_SE(mat, D_004FF160_D970, -self->rz);
        ROTATE_SE(mat, D_004FF150_D970, self->ry);
        ROTATE_SE(mat, D_004FF140_D970, self->rx);
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_cScriptObjRider);
#ifdef SKIP_ASM
void cScriptEntity_cScriptEntity(void*);
void* cScriptObject_cScriptObject(void*, int);
extern char cScriptObjRider_vtbl[];
extern char D_00482018[];
extern char t4mat_Identity[];

/**
 * Constructs a rider's script object: a script entity plus a cScriptObject of type 3 with an identity base transform.
 *
 * Builds the cScriptEntity part (cScriptEntity_cScriptEntity) at +0x0 and the cScriptObject part at +0xC with type 3,
 * installs the vtables cScriptObjRider_vtbl (+0x0) and D_00482018 (+0x18), clears the animation and sound bank infos (+0x1C,
 * +0x20), copies t4mat_Identity into the base transform (+0x30) and zeroes the base rotations (+0x70..+0x78).
 * cRider_cRider builds the rider's embedded one with it.
 *
 * @return the object (self)
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy of t4mat_Identity, likely identity);
// the PC port needs a plain 64-byte copy.
extern "C" void* cScriptObjRider_cScriptObjRider(void* self)
{
    cScriptEntity_cScriptEntity(self);
    cScriptObject_cScriptObject((char*)self + 0xC, 3);
    *(void**)((char*)self + 0x18) = D_00482018;
    *(void**)((char*)self + 0x0) = cScriptObjRider_vtbl;
    *(int*)((char*)self + 0x1C) = 0;
    *(int*)((char*)self + 0x20) = 0;
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"((char*)self + 0x30), "r"(t4mat_Identity)
        : "memory");
    *(int*)((char*)self + 0x70) = 0;
    *(int*)((char*)self + 0x74) = 0;
    *(int*)((char*)self + 0x78) = 0;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_OnStartObjectUse);
#ifdef SKIP_ASM
void cScriptObject_OnStartObjectUse(void* self);
int GetSSXActorHeader(void* self);
int GetSSXScriptHeader(void* self);
// PORT: the real functions take (engine, id); the unit declares them with one parameter.
void* cSSXScriptEngine_GetAnimationBankInfo_2(void* engine, int id) __asm__("cSSXScriptEngine_GetAnimationBankInfo__FPv");
void* cSSXScriptEngine_GetSoundBankInfo_2(void* engine, int id) __asm__("cSSXScriptEngine_GetSoundBankInfo__FPv");
extern "C" void cSSXScriptEngine_GetAnchorOrientation(void* engine, int id, void* obj, void* pos, float* a, float* b, float* c);
extern "C" void fSinCos(float* s, float* c, float angle);
extern char t4mat_Identity[];
extern float t4vect_XAxis[4];
extern float t4vect_YAxis[4];
extern float t4vect_ZAxis[4];

struct sMat_F9F8 {
    float m[4][4];
} __attribute__((aligned(16)));

struct sVec_F9F8 {
    float x, y, z, w;
    sVec_F9F8() {}
    sVec_F9F8(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
} __attribute__((aligned(16)));

struct sScriptObj_F9F8 {
    char pad_0x00[0x1C];
    void* anim0;                // 0x1C
    void* anim1;                // 0x20
};

// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy of t4mat_Identity, the identity).
static inline void LoadIdentity_F9F8(void* dst)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(dst), "r"(t4mat_Identity)
        : "memory");
}

// Fill r with the rotation about the unit vector `axis` whose sine and cosine are s and c.
static inline void MakeRot_F9F8(sMat_F9F8* r, float* axis, float s, float c)
{
    float t = 1.0f - c;
    float x = axis[0];
    float y = axis[1];
    float z = axis[2];
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    r->m[3][0] = 0.0f;
    r->m[3][1] = 0.0f;
    r->m[3][2] = 0.0f;
    r->m[0][3] = 0.0f;
    r->m[1][3] = 0.0f;
    r->m[2][3] = 0.0f;
    r->m[0][0] = tx * x + c;
    r->m[1][0] = tx * y + sz;
    r->m[2][0] = tx * z - sy;
    r->m[0][1] = ty * x - sz;
    r->m[1][1] = ty * y + c;
    r->m[2][1] = ty * z + sx;
    r->m[0][2] = tz * x + sy;
    r->m[1][2] = tz * y - sx;
    r->m[2][2] = tz * z + c;
}

// out = a * b.
// PORT: PS2-only VU0 inline asm (4x4 matrix multiply).
static inline void MulMat_F9F8(void* out, const void* a, const void* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%1)\n"
        "lqc2      $vf5, 0x10(%1)\n"
        "lqc2      $vf6, 0x20(%1)\n"
        "lqc2      $vf7, 0x30(%1)\n"
        "lqc2      $vf8, 0x0(%2)\n"
        "lqc2      $vf9, 0x10(%2)\n"
        "lqc2      $vf10, 0x20(%2)\n"
        "lqc2      $vf11, 0x30(%2)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(out), "r"(a), "r"(b)
        : "memory");
}

// m = m * b.
// PORT: PS2-only VU0 inline asm (4x4 matrix multiply).
static inline void MulIn_F9F8(void* m, const void* b)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf4, 0x0(%0)\n"
        "lqc2      $vf5, 0x10(%0)\n"
        "lqc2      $vf6, 0x20(%0)\n"
        "lqc2      $vf7, 0x30(%0)\n"
        "lqc2      $vf8, 0x0(%1)\n"
        "lqc2      $vf9, 0x10(%1)\n"
        "lqc2      $vf10, 0x20(%1)\n"
        "lqc2      $vf11, 0x30(%1)\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vmulax.xyzw  ACC, $vf4, $vf9x\n"
        "vmadday.xyzw ACC, $vf5, $vf9y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf9z\n"
        "vmaddw.xyzw  $vf13, $vf7, $vf9w\n"
        "vmulax.xyzw  ACC, $vf4, $vf10x\n"
        "vmadday.xyzw ACC, $vf5, $vf10y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf10z\n"
        "vmaddw.xyzw  $vf14, $vf7, $vf10w\n"
        "vmulax.xyzw  ACC, $vf4, $vf11x\n"
        "vmadday.xyzw ACC, $vf5, $vf11y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf11z\n"
        "vmaddw.xyzw  $vf15, $vf7, $vf11w\n"
        "sqc2      $vf12, 0x0(%0)\n"
        "sqc2      $vf13, 0x10(%0)\n"
        "sqc2      $vf14, 0x20(%0)\n"
        "sqc2      $vf15, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(m), "r"(b)
        : "memory");
}

#define ROTATE2_F9F8(mat, axis, angle) \
    { \
        float s; \
        float c; \
        fSinCos(&s, &c, angle); \
        sMat_F9F8 r; \
        MakeRot_F9F8(&r, axis, s, c); \
        r.m[3][3] = 1.0f; \
        MulIn_F9F8(mat, &r); \
    }

// Each rotation matrix is a block-scoped temporary of the caller.
#define ROTATE_F9F8(mat, axis, angle) \
    { \
        float s; \
        float c; \
        fSinCos(&s, &c, angle); \
        sMat_F9F8 r; \
        MakeRot_F9F8(&r, axis, s, c); \
        r.m[3][3] = 1.0f; \
        MulMat_F9F8(mat, mat, &r); \
    }

// PORT: PS2-only VU0 inline asm (matrix-vector product into row 3).
static inline void Translate_F9F8(void* mat, const sVec_F9F8& v)
{
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2         $vf4, 0x0(%0)\n"
        "lqc2         $vf5, 0x10(%0)\n"
        "lqc2         $vf6, 0x20(%0)\n"
        "lqc2         $vf7, 0x30(%0)\n"
        "lqc2         $vf8, %1\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "sqc2         $vf12, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"(mat), "m"(v)
        : "memory");
}

/**
 * Takes the rider's script object into use: builds its base transform from the actor header and the script's anchor and
 * binds the script's animation and sound banks.
 *
 * Calls cScriptObject_OnStartObjectUse__FPv on the cScriptObject part (+0xC). The actor header (GetSSXActorHeader of the bound player entity,
 * +0x8) gives the base rotations in degrees (+0x1C/+0x18/+0x14, stored as radians in +0x70/+0x74/+0x78) and an offset
 * (+0x8..+0x10) that form a local matrix (translate, then Rz(-z), Ry, Rx). cSSXScriptEngine_GetAnchorOrientation (On Tour
 * cSSXScriptEngine::GetAnchorOrientation) gives the anchor position and angles for the header's anchor type (+0x1); the
 * base transform (+0x30) becomes the anchor's translate and rotations times the local matrix, and the anchor angles are
 * added to the base rotations. Finally it binds mpAnimBankInfo (+0x1C) and mpSoundBankInfo (+0x20) to the script's bank
 * (+0x1C of the GetSSXScriptHeader header) with cSSXScriptEngine_GetAnimationBankInfo__FPv and cSSXScriptEngine_GetSoundBankInfo__FPv. The player ID argument is not declared
 * (unused).
 */
extern "C" void cScriptObjRider_OnStartObjectUse(char* self)
{
    cScriptObject_OnStartObjectUse(self + 0xC);
    cScriptPlayerEntity* obj = (cScriptPlayerEntity*)(*(void**)(self + 0x8));
    char* mat = self + 0x30;
    LoadIdentity_F9F8(mat);
    *(int*)(self + 0x70) = 0;
    *(int*)(self + 0x74) = 0;
    *(int*)(self + 0x78) = 0;
    tSSXHeaderActor* info = (tSSXHeaderActor*)GetSSXActorHeader(obj);
    *(float*)(self + 0x70) = info->baseRotX * 0.01745329424738884f;
    *(float*)(self + 0x74) = info->baseRotY * 0.01745329424738884f;
    *(float*)(self + 0x78) = info->baseRotZ * 0.01745329424738884f;
    sVec_F9F8 baseOffset(info->basePosX, info->basePosY, info->basePosZ, 1.0f);
    sMat_F9F8 m;
    LoadIdentity_F9F8(&m);
    Translate_F9F8(&m, baseOffset);
    ROTATE_F9F8(&m, t4vect_ZAxis, -*(float*)(self + 0x78));
    ROTATE_F9F8(&m, t4vect_YAxis, *(float*)(self + 116));
    ROTATE_F9F8(&m, t4vect_XAxis, *(float*)(self + 0x70));
    sVec_F9F8 pos;
    float rx, ry, rz;
    cSSXScriptEngine_GetAnchorOrientation(ScriptEngine, info->anchorPos, obj->mpOwner, &pos, &rx, &ry, &rz);
    Translate_F9F8(mat, pos);
    ROTATE2_F9F8(mat, t4vect_ZAxis, -rz);
    ROTATE2_F9F8(mat, t4vect_YAxis, ry);
    ROTATE2_F9F8(mat, t4vect_XAxis, rx);
    MulMat_F9F8(mat, mat, &m);
    *(float*)(self + 0x70) += rx;
    *(float*)(self + 0x74) += ry;
    *(float*)(self + 0x78) += rz;
    tSSXScriptHeader* info2 = (tSSXScriptHeader*)GetSSXScriptHeader(obj->mpOwner);
    ((sScriptObj_F9F8*)self)->anim0 = cSSXScriptEngine_GetAnimationBankInfo_2(ScriptEngine, info2->bankNum);
    ((sScriptObj_F9F8*)self)->anim1 = cSSXScriptEngine_GetSoundBankInfo_2(ScriptEngine, info2->bankNum);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_OnEndObjectUse);
#ifdef SKIP_ASM
void cScriptObject_OnEndObjectUse(void*);
extern char t4mat_Identity[];

/**
 * Releases the rider's script object: drops one use and resets its base transform and bank bindings.
 *
 * Calls cScriptObject_OnEndObjectUse (On Tour cScriptObject::OnEndObjectUse) on the cScriptObject part (+0xC), copies t4mat_Identity
 * into the base transform (+0x30), zeroes the base rotations (+0x70..+0x78) and clears mpAnimBankInfo (+0x1C) and
 * mpSoundBankInfo (+0x20). The player ID argument is not declared (unused).
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy of t4mat_Identity, likely identity);
// the PC port needs a plain 64-byte copy.
extern "C" void cScriptObjRider_OnEndObjectUse(cScriptObjRider* self)
{
    cScriptObject_OnEndObjectUse(&self->mObjType);
    __asm__ __volatile__(
        ".set noreorder\n"
        "lqc2      $vf1, 0x0(%1)\n"
        "lqc2      $vf2, 0x10(%1)\n"
        "lqc2      $vf3, 0x20(%1)\n"
        "lqc2      $vf4, 0x30(%1)\n"
        "sqc2      $vf1, 0x0(%0)\n"
        "sqc2      $vf2, 0x10(%0)\n"
        "sqc2      $vf3, 0x20(%0)\n"
        "sqc2      $vf4, 0x30(%0)\n"
        ".set reorder\n"
        :
        : "r"((char*)&self->mBaseTransform), "r"(t4mat_Identity)
        : "memory");
    *(int*)&self->mBaseRotX = 0;
    *(int*)&self->mBaseRotY = 0;
    *(int*)&self->mBaseRotZ = 0;
    self->mpAnimBankInfo = 0;
    self->mpSoundBankInfo = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_IsMatchingEntity);
#ifdef SKIP_ASM
extern "C" int cRider_isLoaded(void* self);
extern "C" int cRider_getIdentity(void* self);
extern "C" int cScriptObjRider_IsSuitableRole(cScriptObjRider* self, cRider* rider, int id, int a3);
extern int kCharacterMappingTable[];

class cScriptCtx0458 {
public:
    // vptr at 0x0; slot N at vtable offset N*8
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void* v06();
};

/**
 * Tests whether a script player entity can be played by this rider script object.
 *
 * Only entity type 5 matches. Gets the attached rider (virtual slot 6), which must be loaded (cRider_isLoaded), and its
 * identity bit from kCharacterMappingTable[cRider_getIdentity(rider)]. The entity's actor header (GetSSXActorHeader) holds a role byte
 * and a short character mask at +2: a zero mask or one containing the rider's bit passes, and the final answer comes
 * from cScriptObjRider_IsSuitableRole(self, rider, role, the entity's +0x8 word) (On Tour IsSuitableRole(pRider, role, pPlayerMain)).
 *
 * @param msg  the script player entity (On Tour cScriptPlayerEntity* pEntity)
 * @param type  the entity type asked for; only 5 can match
 * @return nonzero when the entity matches this rider
 */
extern "C" int cScriptObjRider_IsMatchingEntity(cScriptObjRider* self, cScriptPlayerEntity* msg, int type)
{
    if (type != 5) return 0;
    void* rider = ((cScriptCtx0458*)self)->v06();
    if (cRider_isLoaded(rider) == 0) return 0;
    int team = cRider_getIdentity(rider);
    int mask = kCharacterMappingTable[team];
    // PORT: GetSSXActorHeader returns a record pointer as int
    char* info = (char*)GetSSXActorHeader(msg);
    short charMask = *(short*)(info + 2);
    if (charMask == 0 || (charMask & mask) != 0) {
        return cScriptObjRider_IsSuitableRole(self, (cRider*)rider, *(signed char*)info, (int)msg->mpOwner);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_ScriptAnimIDToGameAnimInfo__FPviT0T0);
#ifdef SKIP_ASM
/**
 * Maps a script animation ID to the game animation index and bank of the rider's script anim bank.
 *
 * The anim bank info at +0x1C gives the bank number (+0x4) and the table of game animation IDs (+0x10), indexed by
 * @p scriptAnimId.
 *
 * @param scriptAnimId  the script animation ID
 * @param gameAnimIndex  receives the game animation index (int)
 * @param gameAnimBankId  receives the anim bank number (int)
 */
void cScriptObjRider_ScriptAnimIDToGameAnimInfo(void* vself, int scriptAnimId, void* gameAnimIndex, void* gameAnimBankId)
{
    cScriptObjRider* self = (cScriptObjRider*)vself;
    *(int*)gameAnimBankId = self->mpAnimBankInfo->bankNum;
    *(int*)gameAnimIndex = (self->mpAnimBankInfo->pTableAnimID)[scriptAnimId];
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_ScriptSoundIDToGameSoundInfo__FPviT0T0);
#ifdef SKIP_ASM
/**
 * Maps a script sound ID to the game sound index and bank of the rider's script sound bank: the index is the ID itself,
 * the bank is the bank number (+0x4) of the sound bank info at +0x20.
 *
 * @param scriptSoundId  the script sound ID
 * @param gameSoundIndex  receives the game sound index (int)
 * @param gameSoundBankId  receives the sound bank number (int)
 */
void cScriptObjRider_ScriptSoundIDToGameSoundInfo(void* vself, int scriptSoundId, void* gameSoundIndex, void* gameSoundBankId)
{
    cScriptObjRider* self = (cScriptObjRider*)vself;
    *(int*)gameSoundBankId = *(int*)((char*)self->mpSoundBankInfo + 0x4);
    *(int*)gameSoundIndex = scriptSoundId;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_IsSuitableRole);
#ifdef SKIP_ASM
extern "C" void* cSSXScriptEngine_GetSubjectRider(cSSXScriptEngine* self, int i, int a2);

/**
 * Returns whether a rider suits role @p id of a script: role 0 takes any rider; roles 1-21 take only the subject rider
 * of slot @p id - 1 for the script player (cSSXScriptEngine_GetSubjectRider on ScriptEngine).
 *
 * @param rider  the rider
 * @param id  the role (0 any, 1-21 a subject slot)
 * @param playerMain  the script player (cScriptPlayerMain)
 * @return 1 when the rider suits the role, else 0 (also for negative or larger roles)
 */
extern "C" int cScriptObjRider_IsSuitableRole(cScriptObjRider* self, cRider* rider, int id, int playerMain)
{
    if (id != 0) {
        if (id >= 0) {
            if (id < 22) {
                return cSSXScriptEngine_GetSubjectRider(ScriptEngine, id - 1, playerMain) == ((void*)rider);
            }
        }
    } else {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjEvent_cScriptObjEvent);
#ifdef SKIP_ASM
void cScriptEntity_cScriptEntity(void*);
void* cScriptObject_cScriptObject(void*, int);
extern void* D_00482298[];
extern void* cScriptObjEvent_vtbl[];



/**
 * Constructs the event script object: a script entity plus a type-5 script object with its triggers cleared.
 *
 * The four triggers get target -1, ID -1 and count 0.
 *
 * @return @p self
 */
extern "C" void* cScriptObjEvent_cScriptObjEvent(cScriptObjEvent* self)
{
    cScriptEntity_cScriptEntity((self));
    cScriptObject_cScriptObject(&self->mObjType, 5);
    self->mScriptObjectVtbl = D_00482298;
    *(void***)self = cScriptObjEvent_vtbl;
    self->mNumAbortWorldScripts = 0;
    self->mHasTriggeredSpeechEvents = 0;
    self->mCheckForStopOnSpeechEnd = 0;
    self->mpSoundBankInfo = 0;
    for (int i = 0; i < 4; i++) {
        tScriptObjEventSound* trigger = &((&self->mSounds[0]))[i];
        trigger->soundID = -1;
        trigger->soundHandle = -1;
        trigger->endSound = 0;
    }
    return (self);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjEvent_Update);
#ifdef SKIP_ASM
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* bank, int id, int a, float f);
extern "C" int cBankMonitor_BANKMONITOR_GetHandle(void* bank, int id, int a);
extern "C" int func_002B0E28(void* p, int i);

/**
 * Per-frame update of the script event object: stops or forgets its event sounds and handles a pending stop on speech
 * end.
 *
 * For each of the 4 sound slots (+0xBC; ID, count, target; On Tour mSounds) holding a sound ID (>= 0): a slot whose
 * count word is non-zero is stopped (cBankMonitor_BANKMONITOR_StopHandle on the audio system's bank monitor,
 * cAudioInterface_bxAudio() +0x118 then +0x1D8) and cleared; any other slot is cleared once cBankMonitor_BANKMONITOR_GetHandle no
 * longer finds the sound. If the flag at +0xF4 (On Tour mCheckForStopOnSpeechEnd) is set it is cleared, and when
 * func_002B0E28(audio + 0x5560, 0) returns non-zero the playing NIS list's scripts are skipped
 * (cSSXScriptEngine_SkipNISListScripts).
 */
extern "C" void cScriptObjEvent_Update(cScriptObjEvent* self)
{
    cSSXAudio* snd = cAudioInterface_bxAudio();
    for (int i = 0; i < 4; i++) {
        tScriptObjEventSound* soundSlot = &((&self->mSounds[0]))[i];
        if (soundSlot->soundHandle >= 0) {
            if (soundSlot->endSound != 0) {
                cBankMonitor_BANKMONITOR_StopHandle(**(char***)&snd->MusicSys + 0x1D8, soundSlot->soundHandle, 1, 0.0f);
                soundSlot->soundID = -1;
                soundSlot->soundHandle = -1;
                soundSlot->endSound = 0;
            } else if (cBankMonitor_BANKMONITOR_GetHandle(**(char***)&snd->MusicSys + 0x1D8, soundSlot->soundHandle, 1) == 0) {
                soundSlot->soundID = -1;
                soundSlot->soundHandle = -1;
                soundSlot->endSound = 0;
            }
        }
    }
    if (self->mCheckForStopOnSpeechEnd != 0) {
        self->mCheckForStopOnSpeechEnd = 0;
        if (func_002B0E28(&snd->SpeechSys, 0) != 0) {
            cSSXScriptEngine_SkipNISListScripts(ScriptEngine, ScriptEngine->mPlayingNISList, 1, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjEvent_OnStartObjectUse);
#ifdef SKIP_ASM
void cScriptObject_OnStartObjectUse(void* self);
// PORT: cSSXScriptEngine_GetSoundBankInfo is declared (void*) but forwards a key in $5.
void* cSSXScriptEngine_GetSoundBankInfo_2(void* self, int key) __asm__("cSSXScriptEngine_GetSoundBankInfo__FPv");


/**
 * Takes the script event object into use: resets its event state and binds the script's sound bank.
 *
 * Calls cScriptObject_OnStartObjectUse__FPv on the cScriptObject part (+0xC), clears +0xEC, +0xF0 and +0xF4 (On Tour mNumAbortWorldScripts,
 * mHasTriggeredSpeechEvents, mCheckForStopOnSpeechEnd), binds the sound bank info (+0xF8, On Tour mpSoundBankInfo) with
 * cSSXScriptEngine_GetSoundBankInfo__FPv for the bank of the playing script (+0x1C of the GetSSXScriptHeader header) and empties the 4 sound slots at
 * +0xBC (ID -1, count 0, target -1). The player ID argument is not declared (unused).
 */
extern "C" void cScriptObjEvent_OnStartObjectUse(cScriptObjEvent* self)
{
    int i;
    cScriptObject_OnStartObjectUse(&self->mObjType);
    self->mNumAbortWorldScripts = 0;
    self->mHasTriggeredSpeechEvents = 0;
    self->mCheckForStopOnSpeechEnd = 0;
    // PORT: GetSSXScriptHeader returns a pointer as int
    char* info = (char*)GetSSXScriptHeader(self->mpEntity->mpOwner);
    int key = *(int*)(info + 0x1C);
    self->mpSoundBankInfo = (tScriptSoundBankInfo*)cSSXScriptEngine_GetSoundBankInfo_2(ScriptEngine, key);
    tScriptObjEventSound* soundSlot = ((&self->mSounds[0]));
    for (i = 3; i >= 0; i--) {
        soundSlot->soundID = -1;
        soundSlot->soundHandle = -1;
        soundSlot->endSound = 0;
        soundSlot++;
    }
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjEvent_OnEndObjectUse);
#ifdef SKIP_ASM
void cScriptObject_OnEndObjectUse(void*);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
void func_002A1BD0(void* p);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* bank, int id, int a, float f);
extern "C" void cWScriptMan_runFunc(void* mgr, int a, int b);
extern "C" void GetSSXGameEventFrame(void* self, void* out);
extern void* cWScriptMan_TheWScriptMan;
extern int cRenderStateMan_m_indoor;

struct sSePair07B0 {
    int a;
    int b;
};

struct sSeSound07B0 {
    int id;
    int field_0x4;
    int field_0x8;
};

struct sSeInfo07B0 {
    int v[9];
    int active;     // 0x24
    int pad[2];
};

struct sSeEngine07B0 {
    char pad_0x0[0x8];
    void* owner;                // 0x8
    char pad_0xC[0x10];
    sSePair07B0 pairs[(0xBC - 0x1C) / 8];   // 0x1C
    sSeSound07B0 sounds[4];     // 0xBC
    int pairCount;              // 0xEC
    int field_0xF0;             // 0xF0
    int field_0xF4;             // 0xF4
    int field_0xF8;             // 0xF8
};

/**
 * Takes the script event object out of use: runs its abort world scripts, stops its sounds and clears its event state.
 *
 * After cScriptObject_OnEndObjectUse on the cScriptObject part (+0xC): runs each recorded abort world script (+0x1C, On Tour
 * mAbortWorldScripts: hash, world section; count +0xEC, mNumAbortWorldScripts) with cWScriptMan_runFunc; when speech
 * was triggered (+0xF0, mHasTriggeredSpeechEvents) calls func_002A1BD0 on the audio interface; stops each of the 4
 * sound slots (+0xBC, mSounds) holding a handle (cBankMonitor_BANKMONITOR_StopHandle on the audio system's bank
 * monitor) and resets all four (handle -1, end flag 0, sound ID -1). When the entity's game-event frame (GetSSXGameEventFrame)
 * has an enviro-state list (+0x24), the render-state flag cRenderStateMan_m_indoor is cleared. Finally clears +0xEC, +0xF0, +0xF4
 * (mCheckForStopOnSpeechEnd) and the sound bank info (+0xF8).
 */
extern "C" void cScriptObjEvent_OnEndObjectUse(sSeEngine07B0* self)
{
    sSeInfo07B0 info;
    cScriptObject_OnEndObjectUse((char*)self + 0xC);
    for (int i = 0; i < self->pairCount; i++) {
        cWScriptMan_runFunc(cWScriptMan_TheWScriptMan, self->pairs[i].b, self->pairs[i].a);
    }
    if (self->field_0xF0 != 0) {
        func_002A1BD0(cAudioInterface_bxAudio());
    }
    for (int i = 0; i < 4; i++) {
        int id = self->sounds[i].id;
        if (id >= 0) {
            cBankMonitor_BANKMONITOR_StopHandle(*(char**)*(char**)&cAudioInterface_bxAudio()->MusicSys + 0x1D8, id, 1, 0.0f);
        }
        sSeSound07B0* soundSlot = &self->sounds[i];
        soundSlot->field_0x8 = -1;
        soundSlot->id = -1;
        soundSlot->field_0x4 = 0;
    }
    GetSSXGameEventFrame(self->owner, &info);
    if (info.active != 0) {
        cRenderStateMan_m_indoor = 0;
    }
    self->pairCount = 0;
    self->field_0xF0 = 0;
    self->field_0xF4 = 0;
    self->field_0xF8 = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjEvent_IsMatchingEntity);
#ifdef SKIP_ASM
/**
 * Returns whether the script event object takes an InSeq entity: only entity type 7.
 *
 * @param entity  the InSeq player entity (unused)
 * @param type  the entity type
 * @return 1 for type 7, else 0
 */
extern "C" int cScriptObjEvent_IsMatchingEntity(cScriptObjEvent* self, cScriptPlayerEntity* entity, int type)
{
    return type == 7;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjEvent_OnInSeqKeyEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void GetSSXGameEventFrame(void* self, void* out);
int GetSSXScriptHeader(void* self);
// PORT: the unit declares cSSXScriptEngine_GetTransportWorldSectionID with no parameters; this caller passes the engine
// in $a0.
int cSSXScriptEngine_GetTransportWorldSectionID_08E8(void* engine) __asm__("cSSXScriptEngine_GetTransportWorldSectionID");
extern "C" int* cBENewRaceInterface_getLocation(void* iface);
extern "C" int cSectionMan_getWorldSectionIDFromBELocationID(void* mgr, int id);
extern "C" void cWScriptMan_runFunc(void* mgr, int a, int b);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028E8C0(void* self, int id, int a2);
extern "C" void func_002A19D8(void* self, int id);
extern "C" void cBankMonitor_BANKMONITOR_StopHandle(void* bank, int id, int a, float f);
void cScriptObjEvent_ScriptSoundIDToGameSoundInfo(void* self, int a1, void* a2, void* a3);
extern "C" int cSSXAudio_GetCurrAppliedVolumePtr(void* self, int a1, int a2);
extern "C" int cBankSys_SFX_End(void* self);
extern "C" void* memset(void* dst, int c, int n);
extern void* cWScriptMan_TheWScriptMan;
extern int cRenderStateMan_m_indoor;
// The "no link" hash: a const global with a dynamic initializer (set by the scripter's static init).
extern const int D_004A525C; // const with a dynamic initializer: set once by __sti__all_in_one_scripter_cpp (=
                             //       GetHashValue32(str_NoScript))

struct sWtEnt_08E8 {
    signed char b0;
    char pad1[3];
    int f4[5];
};

struct sWtPair_08E8 {
    int a;
    int b;
};

struct sWtV16_08E8 {
    int f0;
    int f4;
    int f8;
    int fC;
};

struct sWtSub_08E8 {
    int f0;
    sWtEnt_08E8 cfg;         // 0x04
    volatile int cur;        // 0x1C
    sWtEnt_08E8* ents;       // 0x20
    int arr24[4];            // 0x24
    int* p34;                // 0x34
    int* p38;                // 0x38
    int* p3C;                // 0x3C
    sWtPair_08E8 arr40[4];   // 0x40
    int* p60;                // 0x60
    int* p64;                // 0x64
    int* p68;                // 0x68
    int* p6C;                // 0x6C
    sWtV16_08E8* p70;        // 0x70
    int* p74;                // 0x74
    int* p78;                // 0x78
    int* p7C;                // 0x7C
    int* p80;                // 0x80
    int* p84;                // 0x84
};

struct sWtTrack_08E8 {
    char pad0[0x1D8];
    sWtSub_08E8 sub;         // 0x1D8
};

struct sWtSnd_08E8 {
    sWtTrack_08E8* track;    // 0x0
    int* types;              // 0x4
    int* args;               // 0x8
    char pad[0x10C];
    sWtTrack_08E8** cur;     // 0x118
};

struct sSfxEnt_08E8 {
    int id;                  // 0x0
    int when;                // 0x4
    char pad8[0xC - 0x8];
    int slot;                // 0xC
    char pad10[0x1C - 0x10];
    signed char* vol;        // 0x1C
    sSfxEnt_08E8* next;      // 0x20
};

struct sSeLink_08E8 {
    int a;
    int* b;
};

struct sSeCmd_08E8 {
    int id;
    int pad;
};

// Filled by GetSSXGameEventFrame.
struct sSeInfo_08E8 {
    int nLinks;                 // 0x00
    sSeLink_08E8* links;        // 0x04
    int nStops;                 // 0x08
    sSeCmd_08E8* stops;         // 0x0C
    int nCmds;                  // 0x10
    sSeCmd_08E8* cmds;          // 0x14
    int pad18;
    sSfxEnt_08E8* sfx;          // 0x1C
    int pad20;
    int active;                 // 0x24
    int pad28[2];
};

struct sSePair_08E8 {
    int a;
    int b;
};

struct sSeSound_08E8 {
    int id;
    int count;
    int target;
};

struct sSeEngine_08E8 {
    char pad_0x0[0x1C];
    sSePair_08E8 pairs[(0xBC - 0x1C) / 8];   // 0x1C
    sSeSound_08E8 sounds[4];    // 0xBC
    int pairCount;              // 0xEC
    int field_0xF0;             // 0xF0
    int field_0xF4;             // 0xF4
};

#define MIN_08E8(a, b) ((a) < (b) ? (a) : (b))
#define MAX_08E8(a, b) ((a) < (b) ? (b) : (a))

/**
 * Handles an in-sequence key event for the script event object: runs the frame's world script events, music and speech
 * events, and starts or marks its sounds.
 *
 * Reads the entity's game-event frame (GetSSXGameEventFrame; On Tour tSSXFrameGameEvent). The world section is the transport
 * section (cSSXScriptEngine_GetTransportWorldSectionID) when the script header's first byte (GetSSXScriptHeader on the entity's player) is set, else the
 * section of the BE new-race location (cSectionMan_getWorldSectionIDFromBELocationID). Then: - each world script event
 * runs with cWScriptMan_runFunc(section, event ID); its abort-script hash, unless D_004A525C (no script), is recorded
 * once per (hash, section) in the abort list (+0x1C, count +0xEC); - event bit 8 clears and bit 4 sets the render-state
 * flag cRenderStateMan_m_indoor; - each music event (ID 0xFF skipped) goes to func_0028E8C0 on the audio interface; each speech event
 * goes to func_002A19D8 and sets +0xF0, except ID 17 (skipped) and ID 16, which sets +0xF4 (stop on speech end); - bit
 * 2: every sound slot holding a handle gets its end flag set, then the slots of the frame's sounds clear it; - bit 1:
 * walks the frame's sound list until a sound whose start frame is before @p when; a sound whose slot (track index) is
 * free, or holds a different sound (stopped first), is started: bank and index from cScriptObjEvent_ScriptSoundIDToGameSoundInfo, a new SFX record on
 * the audio system's bank monitor with default settings, the sound's volume (clamped to 0-127), the volume setting
 * cSSXAudio_GetCurrAppliedVolumePtr(4, 0), then cBankSys_SFX_End, whose handle goes into the slot.
 *
 * Likely bug: the abort list holds 20 entries and its count is never checked.
 *
 * @param obj  the InSeq player entity
 * @param entityType  the entity type (unused)
 * @param when  the current frame
 * @param flags  the event bits
 */
extern "C" void cScriptObjEvent_OnInSeqKeyEvent(sSeEngine_08E8* self, cScriptPlayerEntity* obj, int entityType, float when, int flags)
{
    sSeInfo_08E8 info;
    GetSSXGameEventFrame(obj, &info);
    void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
    int owner;
    if (*(signed char*)GetSSXScriptHeader(obj->mpOwner) != 0) {
        owner = cSSXScriptEngine_GetTransportWorldSectionID_08E8(ScriptEngine);
    } else {
        void* mgr = SSXApp->Game->SectionMan;
        owner = cSectionMan_getWorldSectionIDFromBELocationID(mgr, *cBENewRaceInterface_getLocation(iface0));
    }
    int i;
    for (i = 0; i < info.nLinks; i++) {
        sSeLink_08E8* worldEvent = &info.links[i];
        cWScriptMan_runFunc(cWScriptMan_TheWScriptMan, owner, worldEvent->a);
        int id = *worldEvent->b;
        if (id != D_004A525C) {
            int found = 0;
            int j;
            for (j = 0; j < self->pairCount; j++) {
                if (id == self->pairs[j].a && owner == self->pairs[j].b) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                self->pairs[self->pairCount].a = id;
                self->pairs[self->pairCount].b = owner;
                self->pairCount++;
            }
        }
    }
    if (flags & 8) {
        cRenderStateMan_m_indoor = 0;
    }
    if (flags & 4) {
        cRenderStateMan_m_indoor = 1;
    }
    sWtSnd_08E8* snd = (sWtSnd_08E8*)cAudioInterface_bxAudio();
    for (i = 0; i < info.nStops; i++) {
        sSeCmd_08E8* audioEvent = &info.stops[i];
        int id = audioEvent->id;
        if (id != 0xFF) {
            func_0028E8C0(snd, id, 0);
        }
    }
    for (i = 0; i < info.nCmds; i++) {
        sSeCmd_08E8* audioEvent = &info.cmds[i];
        int id = audioEvent->id;
        if (id != 17) {
            if (id == 0x10) {
                self->field_0xF4 = 1;
            } else {
                func_002A19D8(snd, id);
                self->field_0xF0 = 1;
            }
        }
    }
    if (flags & 2) {
        for (i = 0; i < 4; i++) {
            if (self->sounds[i].id >= 0) {
                self->sounds[i].count = 1;
            }
        }
        for (sSfxEnt_08E8* soundEntry = info.sfx; soundEntry != 0; soundEntry = soundEntry->next) {
            self->sounds[soundEntry->slot].count = 0;
        }
    }
    if (flags & 1) {
        for (sSfxEnt_08E8* soundEntry = info.sfx; soundEntry != 0; soundEntry = soundEntry->next) {
            if ((float)soundEntry->when < when)
                break;
            int slot = soundEntry->slot;
            int play = 1;
            if (self->sounds[slot].id >= 0) {
                play = 0;
                if (self->sounds[slot].target != soundEntry->id) {
                    cBankMonitor_BANKMONITOR_StopHandle((char*)*snd->cur + 0x1D8, self->sounds[slot].id, 1, 0.0f);
                    play = 1;
                }
            }
            if (play) {
                int gameSoundIndex, type;
                cScriptObjEvent_ScriptSoundIDToGameSoundInfo(self, soundEntry->id, &gameSoundIndex, &type);
                int tp = type;
                int ar = gameSoundIndex;
                sWtTrack_08E8* pushSystem = snd->track;
                sWtSub_08E8* sb = &pushSystem->sub;
                pushSystem->sub.cur++;
                sb->ents[pushSystem->sub.cur] = sb->cfg;
                pushSystem->sub.arr24[pushSystem->sub.cur] = 0;
                sb->p34[pushSystem->sub.cur] = 0;
                sb->p38[pushSystem->sub.cur] = sb->ents[pushSystem->sub.cur].b0;
                sb->p3C[pushSystem->sub.cur] = 0;
                memset((char*)sb + pushSystem->sub.cur * 8 + 0x40, 0, 8);
                sb->p60[pushSystem->sub.cur] = 0;
                sb->p64[pushSystem->sub.cur] = 0;
                sb->p68[pushSystem->sub.cur] = 0;
                sb->p6C[pushSystem->sub.cur] = 0;
                sb->p70[pushSystem->sub.cur].f4 = 0;
                sb->p70[pushSystem->sub.cur].f0 = 100;
                sb->p70[pushSystem->sub.cur].f8 = 90;
                sb->p70[pushSystem->sub.cur].fC = 50;
                sb->p74[pushSystem->sub.cur] = 0;
                sb->p80[pushSystem->sub.cur] = 0;
                sb->p84[pushSystem->sub.cur] = 0;
                sb->p78[pushSystem->sub.cur] = 0x7F;
                sb->p7C[pushSystem->sub.cur] = 1;
                snd->types[snd->track->sub.cur] = tp;
                snd->args[snd->track->sub.cur] = ar;
                {
                    sWtTrack_08E8* volumeSystem = *snd->cur;
                    sWtSub_08E8* s2 = &volumeSystem->sub;
                    s2->p38[volumeSystem->sub.cur] = *soundEntry->vol;
                    s2->ents[volumeSystem->sub.cur].b0 = MAX_08E8(MIN_08E8(s2->p38[s2->cur], 0x7F), 0);
                }
                {
                    sWtTrack_08E8** pt = snd->cur;
                    int volumeSetting = cSSXAudio_GetCurrAppliedVolumePtr(snd, 4, 0);
                    (*pt)->sub.p3C[(*pt)->sub.cur] = volumeSetting;
                }
                {
                    sWtTrack_08E8* resetSystem = *snd->cur;
                    resetSystem->sub.p64[resetSystem->sub.cur] = 0;
                }
                self->sounds[slot].id = cBankSys_SFX_End(snd);
                self->sounds[slot].target = soundEntry->id;
                self->sounds[slot].count = 0;
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjEvent_ScriptSoundIDToGameSoundInfo__FPviT0T0);
#ifdef SKIP_ASM
/**
 * Maps a script sound ID to the game sound index and bank of the event object's sound bank: the index is the ID itself,
 * the bank is the bank number (+0x4) of the sound bank info at +0xF8.
 *
 * @param scriptSoundId  the script sound ID
 * @param gameSoundIndex  receives the game sound index (int)
 * @param gameSoundBankId  receives the sound bank number (int)
 */
void cScriptObjEvent_ScriptSoundIDToGameSoundInfo(void* vself, int scriptSoundId, void* gameSoundIndex, void* gameSoundBankId)
{
    cScriptObjEvent* self = (cScriptObjEvent*)vself;
    *(int*)gameSoundBankId = *(int*)((char*)self->mpSoundBankInfo + 0x4);
    *(int*)gameSoundIndex = scriptSoundId;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_cScriptObjCameraManager);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "scripter/ssxscriptengine_classes.h"

/**
 * Constructs a script camera manager: a cScriptObject of type 0 plus a script entity with nine entity slots.
 *
 * Builds the cScriptObject base (type 0) and the cScriptEntity base (+0x10), constructs nine 0x14-byte slots at +0x1C
 * (cScriptEntityCamera_cScriptEntityCamera) and points each one's owner word (+0x10) at the manager, clears the two camera object pointers
 * (+0xD0) and the transition object (+0xD8), sets the view index (+0xF8) to -1 and resets the script state with
 * cScriptObjCameraManager_ResetScriptManagementVars (On Tour ResetScriptManagementVars). The C++ view class here is named cScriptEngine_1018.
 */
cScriptEngine_1018::cScriptEngine_1018() : cScriptObject_1018(0)
{
    for (int i = 1; i >= 0; i--) {
        f[i] = 0;
    }
    for (int i = 0; i < 9; i++) {
        slots[i].setOwner(this);
    }
    fD8 = 0;
    fF8 = -1;
    cScriptObjCameraManager_ResetScriptManagementVars(this);
}

// PORT: the retail names of the vtables g++ references here.
__asm__("_vt$18cScriptEngine_1018$18cScriptEntity_1018 = D_00482360\n"
        "_vt$18cScriptEngine_1018 = cScriptObjCameraManager_vtbl\n");
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_Update);
#ifdef SKIP_ASM
extern "C" void cScriptObjCameraManager_SetupCameraObject(cScriptObjCameraManager* self, int a1, int a2);


/**
 * Per-frame update of a script camera manager in use: follows its view's camera and sets its cameras up once.
 *
 * Only while in use (+0x8 > 0, On Tour mInUse): copies the position (+0x20) of the camera of view +0xF8
 * (SSXApp->Game->ViewMan, array at +0x4) into the focus box position (+0x100) and, while +0x18 is still 0, calls
 * cScriptObjCameraManager_SetupCameraObject(self, 0, 0) (On Tour SetupCameraObject). cSSXScriptEngine_Update runs it on both managers every frame.
 */
extern "C" void cScriptObjCameraManager_Update(cScriptObjCameraManager* self)
{
    if (self->mInUse > 0) {
        char* riders = (char*)SSXApp->Game->ViewMan;
        char* rider = *(char**)(riders + (self->mViewIndex << 2) + 4);
        *(&self->mFocusBox) = *(t4Vector*)(rider + 0x20);
        if ((int)self->mpEntity == 0) {
            cScriptObjCameraManager_SetupCameraObject(self, 0, 0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_SetViewIndex__FPvi);
#ifdef SKIP_ASM
/**
 * Sets a script camera manager's view index (+0xF8).
 *
 * @param val  the view index
 */
void cScriptObjCameraManager_SetViewIndex(void* vself, int val)
{
    cScriptObjCameraManager* self = (cScriptObjCameraManager*)vself;
    self->mViewIndex = val;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_GetNumScriptEntities__FPv);
#ifdef SKIP_ASM
/**
 * Returns the number of script entities of a script camera manager: 10 (itself and its 9 entity cameras).
 *
 * @return 10
 */
int cScriptObjCameraManager_GetNumScriptEntities(void* self)
{
    return 10;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_GetScriptEntity);
#ifdef SKIP_ASM
/**
 * Returns script entity @p index of a script camera manager: 0 is its own cScriptEntity part (+0x10), 1-9 its entity
 * cameras (+0x1C, 0x14 bytes each). The index is not range-checked.
 *
 * @param index  the entity index (0-9)
 * @return the entity
 */
extern "C" void* cScriptObjCameraManager_GetScriptEntity(cScriptObjCameraManager* self, int index)
{
    if (index == 0) {
        return (char*)&self->mEntityVtbl;
    }
    return (char*)self + (index * 0x14 + 0x8);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_AddCameraObject);
#ifdef SKIP_ASM
void cScriptObjCamera_SetCameraManager(void* self, int val);

/**
 * Attaches a script camera object to the camera manager: stores it in the first free of the 2 camera slots (+0xD0) and
 * sets its camera manager (cScriptObjCamera_SetCameraManager).
 *
 * @param obj  the script camera object
 * @return 1, or 0 when both slots are taken
 */
extern "C" int cScriptObjCameraManager_AddCameraObject(cScriptObjCameraManager* self, cScriptObjCamera* obj)
{
    void** slots = (void**)&self->mpCameraObjects;
    for (int i = 0; i < 2; i++) {
        if (slots[i] == 0) {
            slots[i] = ((void*)obj);
            cScriptObjCamera_SetCameraManager(obj, (int)self); // PORT: pointer passed as int (callee mangled __FPvi)
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_RemoveCameraObject);
#ifdef SKIP_ASM
void cScriptObjCamera_SetCameraManager(void* self, int val);
extern "C" void cScriptObjCamera_SetCameraEntity(cScriptObjCamera* self, void* anim);

struct sSeEng11E8 {
    char pad_0x0[0x8];
    int count;              // 0x8
    char pad_0xC[0xC4];
    char* objs[2];          // 0xD0
    char pad_0xD8[0x4];
    int slots[2];           // 0xDC
};

// PORT: the unit declares cScriptObjCameraManager_RemoveCameraObject(void*, void*) returning void; it returns found/not
// found.
extern "C" int cScriptObjCameraManager_RemoveCameraObject_impl(sSeEng11E8* self, char* key) __asm__("cScriptObjCameraManager_RemoveCameraObject");
/**
 * Detaches a script camera object from the camera manager.
 *
 * Finds the camera slot (+0xD0) holding an object with the same object ID (+0x4) as @p key. If the manager is in use
 * (+0x8) and that camera has an entity bound (+0x14), unbinds it (cScriptObjCamera_SetCameraEntity with 0) and frees the attachment
 * (+0xDC) that pointed at the slot. Then clears the camera's manager (cScriptObjCamera_SetCameraManager with 0) and the slot.
 *
 * @param key  the script camera object
 * @return 1, or 0 when it was not attached
 */
extern "C" int cScriptObjCameraManager_RemoveCameraObject_impl(sSeEng11E8* self, char* key)
{
    int k = *(int*)(key + 4);
    for (int i = 0; i < 2; i++) {
        cScriptObjCamera* cameraObj = (cScriptObjCamera*)self->objs[i];
        if (((char*)cameraObj) != 0 && *(int*)(((char*)cameraObj) + 4) == k) {
            if (self->count > 0 && (int)cameraObj->mpCameraEntity != 0) {
                cScriptObjCamera_SetCameraEntity((cScriptObjCamera*)cameraObj, 0);
                for (int j = 0; j < 2; j++) {
                    if (self->slots[j] == i) {
                        self->slots[j] = -1;
                        break;
                    }
                }
            }
            cScriptObjCamera_SetCameraManager(self->objs[i], 0);
            self->objs[i] = 0;
            return 1;
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_SetTransitionObject);
#ifdef SKIP_ASM
// PORT: cScriptObjTransition_SetTransition forwards four float args in $f12..$f15 (declared (void*, int, int)
// in bxscriptengine); cScriptObjTransition_SetCameraManager stores an owner pointer in an int field. Both bound
// by asm label with the real argument types.
void* cScriptObjTransition_SetTransition_f(void* self, int a1, int a2, float f0, float f1, float f2, float f3)
    __asm__("cScriptObjTransition_SetTransition__FPvii");
void cScriptObjTransition_SetCameraManager_p(void* self, void* owner) __asm__("cScriptObjTransition_SetCameraManager__FPvi");

/**
 * Sets or clears the camera manager's transition object (+0xD8).
 *
 * @p obj 0 clears it: a transition in use (+0x8) is stopped (cScriptObjTransition_SetTransition with player -1 and transition 9, none) and
 * +0xE8 (in transition) cleared, then the object's camera manager is cleared (cScriptObjTransition_SetCameraManager) and the slot emptied.
 * Otherwise @p obj is stored, when no object is set yet, and given this manager.
 *
 * Likely bug: clearing when no transition object is set dereferences a null pointer.
 *
 * @param obj  the script transition object, or 0 to clear
 * @return 1, or 0 when another transition object is already set
 */
extern "C" int cScriptObjCameraManager_SetTransitionObject(cScriptObjCameraManager* self, void* obj)
{
    if (obj == 0) {
        char* cur = (char*)self->mpTransitionObject;
        if (*(int*)(cur + 0x8) > 0) {
            cScriptObjTransition_SetTransition_f(cur, -1, 9, 0.0f, 0.0f, 0.0f, 0.0f);
            self->mInTransition = 0;
        }
        cScriptObjTransition_SetCameraManager_p(self->mpTransitionObject, 0);
        self->mpTransitionObject = 0;
        return 1;
    }
    if (self->mpTransitionObject != 0) {
        return 0;
    }
    self->mpTransitionObject = (cScriptObjTransition*)obj;
    cScriptObjTransition_SetCameraManager_p(obj, self);
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_OnStartObjectUse);
#ifdef SKIP_ASM
extern "C" cScriptDatabaseObject* cScriptDatabaseObject_GetDatabaseObject(void);
extern "C" void cScriptObjCameraManager_ResetScriptManagementVars(void* self);
extern "C" void cFocusMan_addbox(void* self, void* pos);
// PORT: these two are declared (void*) but called with a second argument.
void* cBXScriptEngine_GetRequestPlayerByPlayerID_2(void* self, int idx) __asm__("cBXScriptEngine_GetRequestPlayerByPlayerID__FPv");
void cScriptObject_OnStartObjectUse_2(void* self, int val) __asm__("cScriptObject_OnStartObjectUse__FPv");

/**
 * Takes a script camera manager into use for a player: reserves it, resets its state and adds its focus box when that
 * player is playing the NIS.
 *
 * Marks the object reserved for the player in the script database (cScriptDatabaseObject_MarkObjectAsReserved on cScriptDatabaseObject_GetDatabaseObject(), On Tour
 * MarkObjectAsReserved) and resets the management state (cScriptObjCameraManager_ResetScriptManagementVars). When cSSXScriptEngine_IsPlayingNISListPlayer
 * accepts the player's script player (cBXScriptEngine_GetRequestPlayerByPlayerID__FPv) it adds the focus box (+0x100) to the AI's focus manager
 * (cFocusMan_addbox, On Tour cFocusMan::addbox, on AI +0xA4) and sets the focus-box flag (+0x110). Ends with cScriptObject_OnStartObjectUse__FPv
 * (one more use).
 *
 * @param val  the player ID
 */
extern "C" void cScriptObjCameraManager_OnStartObjectUse(cScriptObjCameraManager* self, int val)
{
    cScriptDatabaseObject_MarkObjectAsReserved(cScriptDatabaseObject_GetDatabaseObject(), self->mObjID, val);
    cScriptObjCameraManager_ResetScriptManagementVars(self);
    // PORT: pointer passed as int
    if (cSSXScriptEngine_IsPlayingNISListPlayer(ScriptEngine, (int)cBXScriptEngine_GetRequestPlayerByPlayerID_2(ScriptEngine, val)) != 0) {
        cFocusMan_addbox(SSXApp->Game->AI->FocusMan, (char*)&self->mFocusBox);
        self->mUseFocusBox = 1;
    }
    cScriptObject_OnStartObjectUse_2(self, val);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_OnEndObjectUse);
#ifdef SKIP_ASM
extern "C" cScriptDatabaseObject* cScriptDatabaseObject_GetDatabaseObject(void);
extern "C" void cScriptDatabaseObject_MarkObjectAsUnreserved(cScriptDatabaseObject* mgr, int id);
extern "C" void cScriptObjCamera_SetCameraEntity(cScriptObjCamera* self, void* anim);
extern "C" void cScriptObjCameraManager_ResetScriptManagementVars(void* self);
extern "C" void cFocusMan_removebox(void* a, void* b, void* c);
// PORT: cScriptObjTransition_SetTransition's mangled signature is (void*, int, int); it also takes four floats (same
// alias as cScriptObjCameraManager_SetTransitionObject).
void* cScriptObjTransition_SetTransition_f(void* self, int a1, int a2, float f0, float f1, float f2, float f3)
    __asm__("cScriptObjTransition_SetTransition__FPvii");
// PORT: the unit declares cScriptObject_OnEndObjectUse(void*); this caller passes a second argument.
void cScriptObject_OnEndObjectUse_2(void* self, int a1) __asm__("cScriptObject_OnEndObjectUse__FPv");

struct sSeEng1400 {
    char pad_0x0[0x4];
    int id;                 // 0x4
    char pad_0x8[0xC8];
    char* objs[2];          // 0xD0
    void* fade;             // 0xD8
    int slots[2];           // 0xDC
    char pad_0xE4[0x1C];
    char area[0x10];        // 0x100
    int pending;            // 0x110
};

/**
 * Releases a script camera manager: unreserves it, detaches its cameras and transition and removes its focus box.
 *
 * Unreserves the object in the script database (cScriptDatabaseObject_MarkObjectAsUnreserved on cScriptDatabaseObject_GetDatabaseObject(), On Tour MarkObjectAsUnreserved),
 * clears the camera entity (cScriptObjCamera_SetCameraEntity, On Tour cScriptObjCamera::SetCameraEntity) of each attached camera (indices
 * at +0xDC into the camera pointers at +0xD0) and sets its index to -1, resets the transition object (+0xD8) with
 * cScriptObjTransition_SetTransition(-1, 9, 0.0, 0.0, 0.0, 0.0) (On Tour cScriptObjTransition::SetTransition), removes the focus box
 * (+0x100) from the AI's focus manager when it was added (+0x110; cFocusMan_removebox, On Tour cFocusMan::removebox), resets
 * the state (cScriptObjCameraManager_ResetScriptManagementVars) and drops one use (cScriptObject_OnEndObjectUse).
 *
 * @param playerID  the player ID
 */
extern "C" void cScriptObjCameraManager_OnEndObjectUse(sSeEng1400* self, int playerID)
{
    cScriptDatabaseObject_MarkObjectAsUnreserved(cScriptDatabaseObject_GetDatabaseObject(), self->id);
    for (int i = 0; i < 2; i++) {
        int cameraIndex = self->slots[i];
        if (cameraIndex >= 0) {
            cScriptObjCamera_SetCameraEntity((cScriptObjCamera*)self->objs[cameraIndex], 0);
            self->slots[i] = -1;
        }
    }
    if (self->fade != 0) {
        cScriptObjTransition_SetTransition_f(self->fade, -1, 9, 0.0f, 0.0f, 0.0f, 0.0f);
    }
    if (self->pending != 0) {
        cAI* lvl = SSXApp->Game->AI;
        cFocusMan_removebox(lvl->FocusMan, self->area, lvl);
        self->pending = 0;
    }
    cScriptObjCameraManager_ResetScriptManagementVars(self);
    cScriptObject_OnEndObjectUse_2(self, playerID);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_OnInSeqKeyEvent);
#ifdef SKIP_ASM
extern "C" void GetSSXCameraManagerFrame(void* self, int* out);
// PORT: GetSSXCameraManagerHeader is defined returning int, but it returns a pointer here; bound by asm label.
signed char* GetSSXCameraManagerHeader_p(void* self) __asm__("GetSSXCameraManagerHeader__FPv");
extern "C" void cScriptObjCameraManager_SetupCameraObject(cScriptObjCameraManager* self, int a1, int a2);
extern "C" void cScriptObjCameraManager_DoCameraTransition(cScriptObjCameraManager* self, int* info, float t);
// PORT: cScriptObjTransition_SetTransition's mangled signature is (void*, int, int); it also takes four floats (same
// alias as cScriptObjCameraManager_SetTransitionObject).
void* cScriptObjTransition_SetTransition_f(void* self, int a1, int a2, float f0, float f1, float f2, float f3)
    __asm__("cScriptObjTransition_SetTransition__FPvii");

struct sNode_2814F8
{
    char pad0[0x4];
    int bit;
    char pad8[0xC];
    signed char* kind;
    sNode_2814F8* next;
};

/**
 * Handles an in-sequence key event for the camera manager: starts camera transitions and releases unused tracks.
 *
 * Does nothing unless @p flags has bit 4 or 8. Reads the manager frame (GetSSXCameraManagerFrame: active camera count, list). -
 * Bit 4: starts the transition to the first active camera (cScriptObjCameraManager_DoCameraTransition), except that with music sync on (the header's
 * first byte, GetSSXCameraManagerHeader), 2 or more active cameras, no beat yet (+0xF0) and that camera's transition-in 8, it only
 * marks a cut as pending on the beat (+0xEC). - Bit 8: with no active cameras, stops the transition object (+0xD8;
 * cScriptObjTransition_SetTransition, transition 9) and releases both tracks (cScriptObjCameraManager_SetupCameraObject with 9). Otherwise runs a pending cut and clears
 * it, then releases each track no active camera uses.
 *
 * @param obj  the InSeq player entity
 * @param type  the entity type (unused)
 * @param flags  the event bits
 * @param currFrame  the current frame
 */
extern "C" void cScriptObjCameraManager_OnInSeqKeyEvent(cScriptObjCameraManager* self, cScriptPlayerEntity* obj, int type, int flags, float currFrame)
{
    if ((flags & 0xC) == 0)
        return;
    int info[2];
    GetSSXCameraManagerFrame(obj, info);
    if (flags & 4)
    {
        int go = 1;
        if (*GetSSXCameraManagerHeader_p(obj) != 0)
        {
            sNode_2814F8* head = (sNode_2814F8*)info[1];
            if (info[0] >= 2 && self->mBeatOccurred == 0 && *head->kind == 8)
            {
                self->mCutOnBeat = go;
                go = 0;
            }
        }
        if (go)
            cScriptObjCameraManager_DoCameraTransition(self, info, currFrame);
    }
    if (flags & 8)
    {
        if (info[0] == 0)
        {
            void* fade = self->mpTransitionObject;
            if (fade)
                cScriptObjTransition_SetTransition_f(fade, -1, 9, 0.0f, 0.0f, 0.0f, 0.0f);
            for (int i = 0; i < 2; i++)
                cScriptObjCameraManager_SetupCameraObject(self, i, 9);
            return;
        }
        if (self->mCutOnBeat)
        {
            cScriptObjCameraManager_DoCameraTransition(self, info, currFrame);
            self->mCutOnBeat = 0;
        }
        int mask = 0;
        for (sNode_2814F8* n = (sNode_2814F8*)info[1]; n; n = n->next)
            mask |= 1 << n->bit;
        for (int i = 0; i < 2; i++)
        {
            if (((mask >> i) & 1) == 0)
                cScriptObjCameraManager_SetupCameraObject(self, i, 9);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_OnInSeqUpdate);
#ifdef SKIP_ASM
extern "C" void GetSSXCameraManagerFrame(void* self, int* out);
// PORT: GetSSXCameraManagerHeader is defined returning int, but it returns a pointer here; bound by asm label.
signed char* GetSSXCameraManagerHeader_p(void* self) __asm__("GetSSXCameraManagerHeader__FPv");
void cScriptObjCamera_SetCameraWeight(void* self, float val);
extern "C" int cScriptObjCameraManager_GetCameraObjectByTrack(cScriptObjCameraManager* self, int i);
extern "C" void cScriptObjCameraManager_DoCameraTransition(cScriptObjCameraManager* self, int* info, float t);
extern "C" void cScriptObjTransition_Update(void* self, float dt);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" float func_002B4920(void* p);

struct sNode_2816A0
{
    char pad0[0x4];
    int side;           // 0x4
    int start;          // 0x8
    char padC[0x4];
    unsigned int len;   // 0x10
    signed char* kind;  // 0x14
};

/**
 * Updates the camera manager for this frame: tracks the music beat and advances the running camera transition.
 *
 * With music sync on (the header's first byte, GetSSXCameraManagerHeader): clears the beat flag (+0xF0); when the audio's beat
 * timer runs (+0x530 of the audio interface) the beat time left (func_002B4920) rising above the last value (+0xF4) is
 * a beat: the flag is set and a pending cut (+0xEC) runs now (cScriptObjCameraManager_DoCameraTransition). The value is stored in +0xF4. Without the
 * timer the beat flag is just set.
 *
 * Then, with active cameras and a transition running (+0xE8), by the first camera's transition-in: - 0 (blend): its
 * track's camera gets weight (frame - start) / overlap frames and the other track's camera the rest; the transition
 * ends after start + overlap, or at once when the other track has no camera. - 1-6: advances the transition object
 * (cScriptObjTransition_Update) by @p dt, ending when it is no longer in use (+0x8); until then, and before start + overlap, the
 * other camera keeps weight 1 and this one 0; after, this one gets 1. - 7: advances the transition object as above. 8:
 * nothing.
 *
 * @param obj  the InSeq player entity
 * @param currFrame  the current frame
 * @param dt  the frame delta
 */
extern "C" void cScriptObjCameraManager_OnInSeqUpdate(cScriptObjCameraManager* self, void* obj, float currFrame, float dt)
{
    int info[2];
    GetSSXCameraManagerFrame(obj, info);
    if (*GetSSXCameraManagerHeader_p(obj) != 0) {
        self->mBeatOccurred = 0;
        cSSXAudio* soundMan = cAudioInterface_bxAudio();
        void* timer = (char*)&soundMan->MusicSys;
        if (soundMan->MusicSys.mSongPlaying != 0) {
            float now = func_002B4920(timer);
            if (self->mLastBeatTimeLeft < now) {
                self->mBeatOccurred = 1;
                if (self->mCutOnBeat != 0) {
                    cScriptObjCameraManager_DoCameraTransition(self, info, currFrame);
                    self->mCutOnBeat = 0;
                }
            }
            self->mLastBeatTimeLeft = now;
        } else {
            self->mBeatOccurred = 1;
        }
    }
    if (info[0] == 0) {
        return;
    }
    if (self->mInTransition == 0) {
        return;
    }
    // PORT: info[1] holds a node pointer
    sNode_2816A0* n = (sNode_2816A0*)info[1];
    switch (*n->kind) {
    case 0: {
        if ((float)(n->len + n->start) < currFrame) {
            self->mInTransition = 0;
        }
        int camera = cScriptObjCameraManager_GetCameraObjectByTrack(self, n->side);
        int otherCamera = cScriptObjCameraManager_GetCameraObjectByTrack(self, 1 - n->side);
        if (otherCamera == 0) {
            self->mInTransition = 0;
            break;
        }
        float w = 1.0f;
        if ((float)(n->len + n->start) < currFrame) {
            self->mInTransition = 0;
        } else {
            w = (currFrame - (float)n->start) / (float)n->len;
        }
        cScriptObjCamera_SetCameraWeight((void*)camera, w);
        cScriptObjCamera_SetCameraWeight((void*)otherCamera, 1.0f - w);
        break;
    }
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6: {
        int camera = cScriptObjCameraManager_GetCameraObjectByTrack(self, n->side);
        int otherCamera = cScriptObjCameraManager_GetCameraObjectByTrack(self, 1 - n->side);
        cScriptObjTransition_Update(self->mpTransitionObject, dt);
        if (self->mpTransitionObject->mInUse <= 0) {
            self->mInTransition = 0;
        }
        if (self->mInTransition != 0 && otherCamera != 0 && currFrame < (float)(n->len + n->start)) {
            cScriptObjCamera_SetCameraWeight((void*)otherCamera, 1.0f);
            cScriptObjCamera_SetCameraWeight((void*)camera, 0.0f);
        } else {
            if (otherCamera != 0) {
                cScriptObjCamera_SetCameraWeight((void*)otherCamera, 0.0f);
            }
            cScriptObjCamera_SetCameraWeight((void*)camera, 1.0f);
        }
        break;
    }
    case 7:
        cScriptObjTransition_Update(self->mpTransitionObject, dt);
        if (self->mpTransitionObject->mInUse <= 0) {
            self->mInTransition = 0;
        }
        break;
    case 8:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_IsMatchingEntity);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
void* cSSXScriptEngine_GetScriptSetupInfo_2(void* self, int idx) __asm__("cSSXScriptEngine_GetScriptSetupInfo__FPv");

/**
 * Returns whether the camera manager takes an InSeq entity: entity type 4 whose script setup info (cSSXScriptEngine_GetScriptSetupInfo for
 * the entity's player, +0x8) has this manager's view index (+0xF8) at +0xC.
 *
 * @param entity  the InSeq player entity
 * @param n  the entity type
 * @return 1 when it matches, else 0
 */
extern "C" int cScriptObjCameraManager_IsMatchingEntity(cScriptObjCameraManager* self, cScriptPlayerEntity* entity, int n)
{
    if (n != 4) {
        return 0;
    }
    return *(int*)((char*)cSSXScriptEngine_GetScriptSetupInfo_2(ScriptEngine, (int)entity->mpOwner) + 0xC) ==
           self->mViewIndex;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_ResetScriptManagementVars);
#ifdef SKIP_ASM
struct sVec4_1A50 {
    float x, y, z, w;
    sVec4_1A50(float a, float b, float c, float d)
    {
        x = a;
        y = b;
        z = c;
        w = d;
    }
} __attribute__((aligned(16)));

/**
 * Resets a script camera manager's per-script state: no attached cameras, no transition or beat, no focus box.
 *
 * Sets both attached camera indices (+0xDC, On Tour mAttachedCameraObject) to -1, the beat flag (+0xF0, On Tour
 * mBeatOccurred) to 1, the last beat time (+0xF4, mLastBeatTimeLeft) to 1e9 and the focus box position (+0x100) to
 * (0, 0, 0, 1), and clears the main camera index (+0xE4, mMainCameraIndex), the in-transition flag (+0xE8,
 * mInTransition), the cut-on-beat flag (+0xEC, mCutOnBeat) and the focus-box flag (+0x110, mUseFocusBox). On Tour's
 * bools are bytes; here they are words, so the fields after mMainCameraIndex sit 8 bytes later than in On Tour.
 */
extern "C" void cScriptObjCameraManager_ResetScriptManagementVars(void* self)
{
    int i;
    int noCamera = -1;
    int* ids = (int*)((char*)self + 0xDC);
    for (i = 1; i >= 0; i--) {
        ids[i] = noCamera;
    }
    *(int*)((char*)self + 0xF0) = 1;
    *(float*)((char*)self + 0xF4) = 1000000000.0f;
    *(sVec4_1A50*)((char*)self + 0x100) = sVec4_1A50(0.0f, 0.0f, 0.0f, 1.0f);
    *(int*)((char*)self + 0xE4) = 0;
    *(int*)((char*)self + 0xE8) = 0;
    *(int*)((char*)self + 0xEC) = 0;
    *(int*)((char*)self + 0x110) = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_SetupCameraObject);
#ifdef SKIP_ASM
extern "C" void cScriptObjCamera_SetCameraEntity(cScriptObjCamera* self, void* anim);
int cScriptEntity_IsPlaying(void* self);

struct sSseAnim1AC8 {
    char pad_0x0[0xC];
    int id;                 // 0xC
    char pad_0x10[0x4];
};

struct sSseRig1AC8 {
    char pad_0x0[0x8];
    int busy;               // 0x8
    char pad_0xC[0x8];
    sSseAnim1AC8* cur;      // 0x14
};

struct sSseEngine1AC8 {
    char pad_0x0[0x1C];
    sSseAnim1AC8 anims[9];  // 0x1C
    char pad_0xD0[0x0];
    sSseRig1AC8* rigs[2];   // 0xD0
    char pad_0xD8[0x4];
    int chans[1];           // 0xDC
};

/**
 * Points the camera of track @p ch at the playing entity camera of subtype @p anim, or releases the track for 9.
 *
 * A track without a camera (+0xDC holds -1) takes the first camera object (+0xD0) not in use (+0x8); with none free, or
 * for 9, nothing happens. Subtype 9 unbinds the camera's entity (cScriptObjCamera_SetCameraEntity with 0) and frees the track. Otherwise,
 * unless the camera already shows that subtype (its entity's +0xC), the first of the 9 entity cameras (+0x1C) that is
 * playing (cScriptEntity_IsPlaying) with subtype @p anim is bound (cScriptObjCamera_SetCameraEntity); when none is, the track stays as it was.
 *
 * @param selfp  the script camera manager
 * @param ch  the track index (0 or 1)
 * @param anim  the camera subtype, or 9 for none
 */
extern "C" void cScriptObjCameraManager_SetupCameraObject(cScriptObjCameraManager* selfp, int ch, int anim)
{
    sSseEngine1AC8* self = (sSseEngine1AC8*)selfp;
    int slot = self->chans[ch];
    if (slot < 0) {
        if (anim == 9) {
            return;
        }
        for (int i = 0; i < 2; i++) {
            sSseRig1AC8* camera = self->rigs[i];
            if (camera != 0 && camera->busy <= 0) {
                slot = i;
                break;
            }
        }
        if (slot < 0) {
            return;
        }
    }
    if (anim == 9) {
        cScriptObjCamera_SetCameraEntity((cScriptObjCamera*)self->rigs[slot], 0);
        self->chans[ch] = -1;
        return;
    }
    sSseAnim1AC8* cur = self->rigs[slot]->cur;
    if (cur == 0 || cur->id != anim) {
        int j = 0;
        for (sSseAnim1AC8* cameraEntity = self->anims; ; cameraEntity++, j++) {
            if (j >= 9) {
                return;
            }
            if (cScriptEntity_IsPlaying(cameraEntity) != 0 && cameraEntity->id == anim) {
                cScriptObjCamera_SetCameraEntity((cScriptObjCamera*)self->rigs[slot], cameraEntity);
                break;
            }
        }
    }
    self->chans[ch] = slot;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_GetCameraObjectByTrack);
#ifdef SKIP_ASM
/**
 * Returns the camera object attached to track @p i: the camera slot (+0xD0) named by the track's attachment (+0xDC).
 *
 * @param i  the track index
 * @return the script camera object, or 0 when none is attached
 */
extern "C" int cScriptObjCameraManager_GetCameraObjectByTrack(cScriptObjCameraManager* self, int i)
{
    int j = self->mAttachedCameraObject[i];
    if (j < 0) {
        return 0;
    }
    return (int)self->mpCameraObjects[j];
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_DoCameraTransition);
#ifdef SKIP_ASM
extern "C" cScriptDatabaseObject* cScriptDatabaseObject_GetDatabaseObject(void);
extern "C" int cScriptDatabaseObject_GetPlayerIDByEntity(cScriptDatabaseObject* self, void* a1);
void cScriptObjCamera_SetCameraWeight(void* self, float val);
extern "C" void cScriptObjCameraManager_SetupCameraObject(cScriptObjCameraManager* self, int a1, int a2);
// PORT: cScriptObjCameraManager_GetCameraObjectByTrack is defined returning int, but it returns a pointer here; bound
// by asm label.
extern "C" void* cScriptObjCameraManager_GetCameraObjectByTrack_p(void* self, int i) __asm__("cScriptObjCameraManager_GetCameraObjectByTrack");
// PORT: cScriptObjTransition_SetTransition's mangled signature is (void*, int, int); it also takes four floats (same
// alias as cScriptObjCameraManager_SetTransitionObject).
void* cScriptObjTransition_SetTransition_f(void* self, int a1, int a2, float f0, float f1, float f2, float f3)
    __asm__("cScriptObjTransition_SetTransition__FPvii");
extern "C" void cScriptObjTransition_SetTransition1(void* self, int id, int a, int b, float f0, float f1, float f2, float f3);

struct sKind_281C68
{
    signed char a;
    signed char b;
    short c;
    short d;
};

struct sNode_281C68
{
    int mode;
    int bit;
    int start;
    char padC[0x4];
    unsigned int len;
    sKind_281C68* kind;
};

/**
 * Starts the camera transition to the first active camera of a camera-manager frame.
 *
 * For the other track: with a transition-in of 7 or more its camera is released (cScriptObjCameraManager_SetupCameraObject with 9), else its
 * camera's weight is set to 0. The camera's own track is set up for its value (cScriptObjCameraManager_SetupCameraObject). The in-transition flag
 * (+0xE8) is then set unless the transition-in is 8 (cut) or the overlap is 0 frames. For 7 and 9 the transition object
 * (+0xD8) gets cScriptObjTransition_SetTransition, and for 1-6 cScriptObjTransition_SetTransition1 (with colour, out and hold frames), with the player using the
 * manager's entity (cScriptDatabaseObject_GetPlayerIDByEntity) and the frames since start; with no transition object the flag is cleared. 0 (blend)
 * needs no object.
 *
 * @param info  the frame (active camera count, active camera list)
 * @param currFrame  the current frame
 */
extern "C" void cScriptObjCameraManager_DoCameraTransition(cScriptObjCameraManager* self, int* info, float currFrame)
{
    sNode_281C68* node = (sNode_281C68*)info[1];
    int bit = node->bit;
    for (int i = 0; i < 2; i++)
    {
        if (i == bit)
            continue;
        if (node->kind->a >= 7)
        {
            cScriptObjCameraManager_SetupCameraObject(self, i, 9);
            continue;
        }
        void* camera = cScriptObjCameraManager_GetCameraObjectByTrack_p(self, i);
        if (camera)
            cScriptObjCamera_SetCameraWeight(camera, 0.0f);
    }
    cScriptObjCameraManager_SetupCameraObject(self, bit, node->mode);
    self->mInTransition = 0;
    if (node->kind->a == 8)
        return;
    if (node->len == 0)
        return;
    self->mInTransition = 1;
    int k = node->kind->a;
    if (k >= 7)
    {
        void** pf = (void**)&self->mpTransitionObject;
        if (*pf == 0)
        {
            self->mInTransition = 0;
            return;
        }
        int id = cScriptDatabaseObject_GetPlayerIDByEntity((cScriptDatabaseObject*)(cScriptDatabaseObject_GetDatabaseObject()), (char*)&self->mEntityVtbl);
        cScriptObjTransition_SetTransition_f(*pf, id, node->kind->a, currFrame - (float)node->start, (float)node->len, 0.0f, 0.0f);
    }
    else if (k > 0)
    {
        void** pf = (void**)&self->mpTransitionObject;
        if (*pf == 0)
        {
            self->mInTransition = 0;
            return;
        }
        int id = cScriptDatabaseObject_GetPlayerIDByEntity((cScriptDatabaseObject*)(cScriptDatabaseObject_GetDatabaseObject()), (char*)&self->mEntityVtbl);
        cScriptObjTransition_SetTransition1(*pf, id, node->kind->a, node->kind->b, currFrame - (float)node->start, (float)node->kind->d, (float)node->len, (float)node->kind->c);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cBXScriptEngine_cBXScriptEngine);
#ifdef SKIP_ASM
extern "C" void* cScriptDatabaseObject_cScriptDatabaseObject(cScriptDatabaseObject* self);
extern "C" void cBXScriptEngine_ResetInstVars(void* self);
extern char D_00482450[];
extern char cBXScriptEngine_vtbl[];
extern void* cBXScriptEngine_mpScriptEngine;

/**
 * Constructs the base script engine: database part, vtables, global instance pointer, empty script database.
 *
 * @return @p self
 */
extern "C" void* cBXScriptEngine_cBXScriptEngine(cBXScriptEngine* self)
{
    cScriptDatabaseObject_cScriptDatabaseObject((cScriptDatabaseObject*)self);
    self->mInSeqCommVtbl = (void**)D_00482450;
    self->mVtbl = (void**)cBXScriptEngine_vtbl;
    cBXScriptEngine_mpScriptEngine = ((void*)self);
    self->mpScriptDatabase = 0;
    self->mScriptDatabaseSize = 0;
    cBXScriptEngine_ResetInstVars(self);
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cBXScriptEngine__cBXScriptEngine);
#ifdef SKIP_ASM
extern "C" void cScriptDatabaseObject__cScriptDatabaseObject(cScriptDatabaseObject* self, int flags);
extern "C" void cBXScriptEngine_PurgeBXEngine(void* self);
extern char D_00482450[];
extern char cBXScriptEngine_vtbl[];
extern void* cBXScriptEngine_mpScriptEngine;

/**
 * Destroys the base script engine: restores its vtables, purges it and clears the global instance pointer.
 *
 * Then runs the database destructor (cScriptDatabaseObject__cScriptDatabaseObject), which frees the object when asked to.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cBXScriptEngine__cBXScriptEngine(cBXScriptEngine* self, int flags)
{
    self->mInSeqCommVtbl = (void**)D_00482450;
    self->mVtbl = (void**)cBXScriptEngine_vtbl;
    cBXScriptEngine_PurgeBXEngine(self);
    cBXScriptEngine_mpScriptEngine = 0;
    cScriptDatabaseObject__cScriptDatabaseObject((cScriptDatabaseObject*)self, flags);
}
#endif

