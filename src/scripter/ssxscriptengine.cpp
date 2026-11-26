#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "scripter/scriptentity.h"
#include "scripter/scriptobjtransition.h"
#include "camera/camera.h"
#include "camera/cameracontroller.h"
#include "camera/script/scriptcontroller.h"
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
#include "ai/player.h"
#include "input/input.h"
#include "input/inputpad.h"
#include "scripter/nislist.h"
#include "inseq/scriptplayerentity.h"
#include "inseq/scriptplayermain.h"
#include "ai/rider.h"
#include "scripter/bxscriptengine.h"
#include "scripter/scriptobjcamera.h"
#include "scripter/scriptobj.h"
#include "scripter/scsndmgr.h"
#include "scripter/scriptdatabaseobject.h"
#include "bx/bxstring.h"
#include "render/ps2graphicsman.h"
#include "be/beintnewrace.h"
#include "be/beintnetwork.h"
#include "world/world.h"
#include "scripter/ssxscriptengine.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"
#include "ai/computer.h"
#include "main/gamemode.h"
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


// A NIS list as new[] builds it: tNISList with its constructor (tNISList_tNISList), the tagged operator new[]
// and a sized operator delete[].
struct tNISListNew : tNISList
{
    tNISListNew() __asm__("tNISList_tNISList");
    // PORT: tagged array new (operator_new__FUi); the sized delete[] makes g++ keep the element-count cookie.
    void* operator new[](unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
    void operator delete[](void* p, unsigned int size);
};


/**
 * Sets up the script engine for a game: setup-info pool, main script bank, script objects and the NIS/idle
 * lists.
 *
 * Allocates the 10-entry setup-info free list ("SetupInfoHeap"), loads bank 0 synchronously from
 * data/scripts/scdat.big|data/scripts/scdat_main.big, initialises the four built-in script objects
 * (cScriptObject_InitScriptObject) and creates two NIS lists ("NISLists") and two idle lists ("IdleLists", one per
 * viewport).
 */
void cSSXScriptEngine::Load()
{
    mpSetupInfoHeap = (tSSXScriptSetupInfoLinked*)operator_new_tag(200, str_SetupInfoHeap, 0, 0);
    for (int i = 0; i < 10; i++)
    {
        mpSetupInfoHeap[i].mainCharAIIndex = -1;
        mpSetupInfoHeap[i].supportChar0AIIndex = -1;
        mpSetupInfoHeap[i].supportChar1AIIndex = -1;
        mpSetupInfoHeap[i].viewIndex = -1;
        mpSetupInfoHeap[i].pNext = &mpSetupInfoHeap[i + 1];
    }
    mpSetupInfoHeap[9].pNext = 0;
    mpSetupInfoUnused = mpSetupInfoHeap;
    cSSXScriptEngine_SyncLoadScriptBank_7(this, 0, D_004A349C, 0, -1, 0, 0);
    for (int i = 0; i < 2; i++)
        cScriptObject_InitScriptObject(&mObjectCameraManager[i]);
    cScriptObject_InitScriptObject(((char*)&mObjectGameState.mObjType));
    cScriptObject_InitScriptObject(((char*)&mObjectEvent.mObjType));
    tNISList** pa = &mpNISLists;
    *pa = new (str_NISLists, 0, 0) tNISListNew[2];
    int i;
    for (i = 0; i < 2; i++)
        mpNISLists[i].mpNISEngine = this;
    tNISList** pb = &mpIdleLists;
    *pb = new (str_IdleLists, 0, 0) tNISListNew[2];
    for (i = 0; i < 2; i++)
    {
        mpIdleLists[i].mpNISEngine = this;
        mpIdleLists[i].mIsCutSceneHandler = 0;
        mpIdleLists[i].mViewIndex = i;
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

// The cookie g++ 2.95's new[] keeps in front of an array whose element type has a destructor (16 bytes, the
// element count first); delete[] reads the count from it and frees from its start.
struct tArrayNewCookie {
    int count;
    int pad[3];
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
void cSSXScriptEngine::Purge()
{
    ForceEndScriptsAll();
    cBXScriptEngine_ReleaseRequestAll(this);
    for (int i = 0; i < 2; i++) {
        cScriptObject_DeInitScriptObject(&mObjectCameraManager[i]);
    }
    cScriptObject_DeInitScriptObject(((char*)&mObjectGameState.mObjType));
    cScriptObject_DeInitScriptObject(((char*)&mObjectEvent.mObjType));
    PurgeTrack();
    for (int i = 0; i < 10; i++) {
        PurgeScriptBank(i);
    }
    cDataManager_DetachAll(((char*)&mMaxDataInstances));
    cVidEngine_ResetVideoEngine(((char*)&mpPlayer));
    if (mpNISLists != 0) {
        tArrayNewCookie* cookies = (tArrayNewCookie*)mpNISLists;
        tNISList* objPtr = mpNISLists + cookies[-1].count;
        while (mpNISLists != objPtr) {
            objPtr--;
            tNISList__tNISList((int*)objPtr, 0);
        }
        tArrayNewCookie* array = (tArrayNewCookie*)mpNISLists;
        cMemMan_free(&array[-1]);
    }
    if (mpSetupInfoHeap != 0) {
        cMemMan_free(mpSetupInfoHeap);
    }
    if (mpIdleLists != 0) {
        tArrayNewCookie* cookies = (tArrayNewCookie*)mpIdleLists;
        tNISList* objPtr = mpIdleLists + cookies[-1].count;
        while (mpIdleLists != objPtr) {
            objPtr--;
            tNISList__tNISList((int*)objPtr, 0);
        }
        tArrayNewCookie* array = (tArrayNewCookie*)mpIdleLists;
        cMemMan_free(&array[-1]);
    }
    cRenderStateMan_setInNIS(0);
    ClearNISPlayListState();
    mpNISLists = 0;
    mpSetupInfoHeap = 0;
    mpSetupInfoUnused = 0;
    mpIdleLists = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_Init__FPv);
#ifdef SKIP_ASM
/**
 * Clears the engine's NIS skip lockout, mNumFramesNoSkip (+0x554). cGame_gainFocus calls it.
 */
void cSSXScriptEngine::Init()
{
    mNumFramesNoSkip = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_DeInit__FPv);
#ifdef SKIP_ASM
/**
 * Leaves the script engine's per-game state: clears the NIS no-skip countdown.
 *
 * Stores 0 in mNumFramesNoSkip (+0x554), exactly what cSSXScriptEngine_Init (On Tour Init, called from cGame_gainFocus)
 * does too. cGame_loseFocus calls it on ScriptEngine.
 */
void cSSXScriptEngine::DeInit()
{
    mNumFramesNoSkip = 0;
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
void cSSXScriptEngine::Update()
{
    for (int i = 0; i < 2; i++)
        tNISList_Update(&mpNISLists[i]);
    for (int i = 0; i < 2; i++)
        tNISList_Update(&mpIdleLists[i]);
    cBXScriptEngine_Update(this);
    cVidEngine_UpdateVideo(((char*)&mpPlayer));
    for (int i = 0; i < 2; i++)
        mObjectCameraManager[i].Update();
    mObjectEvent.Update();
    if (mPlayingNISList != keNISList_Num)
    {
        tNISList* nisList = &mpNISLists[mPlayingNISList];
        if (tNISList_IsSkipAllowed((tNISList*)nisList))
        {
            cAI* lvl = SSXApp->Game->AI;
            int n = lvl->NumberOfPlayersAllocated;
            for (int j = 0; j < n; j++)
            {
                cInput* input = lvl->Players[j]->Input;
                if (input->Pad->NumButtons != 0 && mNumFramesNoSkip == 0 && cInput_getState(input, 111))
                {
                    tNISList_DoSkip(nisList);
                    goto done;
                }
            }
        }
    done:
        if (tNISList_GetState_i(nisList) != keNISListState_Playing)
            ClearNISPlayListState();
        if (cUIStateStack_findState(&SSXApp->Game->UI->mStateStack, GetHashValue32(str_hud_e)))
        {
            if (tNISList_IsPausedPendingNextScript(nisList))
                cBXLoadIndicator_Show(SSXApp->Game->LoadIndicator, 1);
            else
                cBXLoadIndicator_Hide(SSXApp->Game->LoadIndicator, 1);
        }
    }
    if (mNumFramesNoSkip > 0)
        mNumFramesNoSkip--;
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
void cSSXScriptEngine::UpdateLoad()
{
    if (mNumAsyncs == 0) return;
    void* tableHandle = mFilterTableAsyncHandle;
    if (tableHandle != 0 && ASYNCFILE_getstatus(tableHandle) != 0) {
        ProcessAsyncLoadTrackTableComplete(mFilterTableAsyncHandle);
    }
    tSSXScriptBankInfo* slots = (tSSXScriptBankInfo*)(&mScriptDataBanks[0].asyncHandle);
    for (int i = 0; i < 10; i++) {
        void* bankFile = slots[i].dataBankHandle;
        if (bankFile != 0 && ASYNCFILE_getstatus(bankFile) != 0) {
            ProcessAsyncLoadScriptBankComplete(bankFile);
        }
    }
}
#endif

extern "C" void* cVidEngine_RenderVideo(void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_Render__FPv);
#ifdef SKIP_ASM
/**
 * Draws the script engine's video: calls cVidEngine_RenderVideo on its cVidEngine part (+0x500). cGame_render calls it
 * each frame.
 *
 * @return cVidEngine_RenderVideo's return register
 */
void* cSSXScriptEngine::Render()
{
    return cVidEngine_RenderVideo(&mpPlayer);
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
void cSSXScriptEngine::Restart()
{
    ResetNISListAll();
    ResetIdleListAll();
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
void cSSXScriptEngine::AttachAnchorDataTable(int key, unsigned int* val)
{
    mpAnchorTableDB[cSectionMan_getBESectionIDFromWorldSectionID(SSXApp->Game->SectionMan, key)] = val;
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
void cSSXScriptEngine::DetachAnchorDataTable(int key)
{
    mpAnchorTableDB[cSectionMan_getBESectionIDFromWorldSectionID(SSXApp->Game->SectionMan, key)] = 0;
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
void cSSXScriptEngine::ForceEndScriptsAll()
{
    ResetNISListAll();
    ResetIdleListAll();
    cBXScriptEngine_StopScriptAll(this);
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
 * After cBXScriptEngine_ForcePlayUpdate() succeeds, each of the two object camera managers that the request's player
 * uses (engine vtable slot 4) is updated with cScriptObjCameraManager_Update().
 *
 * @param key  request handle (On Tour requestHandle)
 * @return cBXScriptEngine_ForcePlayUpdate()'s result: nonzero when the request was played
 */
int cSSXScriptEngine::ForcePlayUpdate(int key)
{
    int result = cBXScriptEngine_ForcePlayUpdate(this, key);
    if (result != 0) {
        sScrSlot8AD0* slot = (sScrSlot8AD0*)&mObjectCameraManager;
        for (int i = 0; i < 2; i++, slot++) {
            sSsxVEntry8AD0* vt = (sSsxVEntry8AD0*)mVtbl;
            int id = slot->id;
            void* obj = vthis(this, vt[4]);
            // PORT: cBXScriptEngine_GetRequestPlayerByReqHandle returns a pointer as int
            if (vt[4].fn(obj, id, *(int*)cBXScriptEngine_GetRequestPlayerByReqHandle(this, key))) {
                ((cScriptObjCameraManager*)slot)->Update();
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" tBELocationDB* cBENewRaceInterface_getLocation(void* iface);

// A local cBXString with the members this function uses, bound to the game's (unmangled) symbols.
// PORT: asm labels are not accepted on destructors, so the dtor forwards to the out-of-line one (in-charge flag 2).
extern "C" void cBXString_dtor_local(cBXString* self, int flags) __asm__("cBXString__cBXString");

struct cBXStringLocal : cBXString {
    cBXStringLocal(const char* s) __asm__("cBXString_cBXString2");
    ~cBXStringLocal() { cBXString_dtor_local(this, 2); }
    cBXStringLocal& operator=(const cBXStringLocal& s) __asm__("cBXString_operatorE");
};
cBXStringLocal operator+(const char* a, const cBXStringLocal& b) __asm__("func_003186D0");
cBXStringLocal operator+(const cBXStringLocal& a, const char* b) __asm__("func_00318630");
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
void cSSXScriptEngine::AsyncLoadTrack()
{
    cBXStringLocal name(cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))->fileName);
    cBXStringLocal path = str_data_scripts_scdat_big_data_scripts_scfi + name + str_dat;
    void* handle = ASYNCFILE_load_8B98(path.Str, 0);
    mFilterTableAsyncHandle = handle;
    if (handle != 0) {
        mNumAsyncs += 1;
    }
    path = str_data_scripts_scdat_big_data_scripts_scda + name + str_big_c;
    AsyncLoadScriptBank(1, path.Str, 0, -1, 0, 0);
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
void cSSXScriptEngine::PurgeTrack()
{
    if (mFilterTableAsyncHandle != 0) {
        ASYNCFILE_cancel(mFilterTableAsyncHandle);
        ASYNCFILE_release(mFilterTableAsyncHandle, 0, 0);
        mNumAsyncs -= 1;
    }
    PurgeScriptBank(1);
    if (mpFilterTableHeader != 0) {
        MEM_free(mpFilterTableHeader);
    }
    mFilterTableAsyncHandle = 0;
    mpFilterTableHeader = 0;
    mpCategoryFilterTable = 0;
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
int cSSXScriptEngine::IsLoadTrackComplete()
{
    int complete;
    UpdateLoad();
    complete = 0;
    if ((int)mFilterTableAsyncHandle == 0) {
        complete = IsBankLoading(1) == 0;
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
int cSSXScriptEngine::IsPauseMenuAllowed()
{
    int i = mPlayingNISList;
    if (i == keNISList_Num) {
        return 1;
    }
    return tNISList_IsPauseMenuAllowed(&mpNISLists[i]);
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
void cSSXScriptEngine::DisallowNISListSkip(int val)
{
    mNumFramesNoSkip = val;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_SetNISListViewIndex);
#ifdef SKIP_ASM
/**
 * Sets the view index of both NIS lists (mpNISLists).
 *
 * @param viewIndex  the view index
 */
void cSSXScriptEngine::SetNISListViewIndex(int viewIndex)
{
    for (int i = 0; i < 2; i++) {
        mpNISLists[i].mViewIndex = viewIndex;
    }
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
void cSSXScriptEngine::AddToNISListByVideoCategory(int i, int category, int scriptControlBits)
{
    tNISList_AddByVideoCategory(&mpNISLists[i], category, scriptControlBits);
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
void cSSXScriptEngine::AddToNISListByScriptCategory(int i, int scriptCategory, int scriptControlBits,
                                                    int selectScriptNow, int mainCharAIIndex, int supportChar0AIIndex,
                                                    int supportChar1AIIndex)
{
    tNISList_AddByScriptCategory(&mpNISLists[i], scriptCategory, scriptControlBits, selectScriptNow, mainCharAIIndex,
            supportChar0AIIndex, supportChar1AIIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AddToNISListByScriptIndex);
#ifdef SKIP_ASM
extern "C" void tNISList_AddByScriptIndex(void* e, int a, int b, int c, int d, int f);

/**
 * Adds a script, given by its master-table index, to a NIS list (the list's AddByScriptIndex,
 * tNISList_AddByScriptIndex).
 *
 * @param i  NIS list (eNISList)
 * @param scriptIndex  script index in the master table
 * @param scriptControlBits  script control bits
 * @param mainCharAIIndex  AI index of the main character, or -1
 * @param supportChar0AIIndex  AI index of the first supporting character, or -1
 * @param supportChar1AIIndex  AI index of the second supporting character, or -1
 */
void cSSXScriptEngine::AddToNISListByScriptIndex(int i, int scriptIndex, int scriptControlBits, int mainCharAIIndex,
                                                 int supportChar0AIIndex, int supportChar1AIIndex)
{
    tNISList_AddByScriptIndex(&mpNISLists[i], scriptIndex, scriptControlBits, mainCharAIIndex, supportChar0AIIndex,
            supportChar1AIIndex);
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AddToNISListByScriptHashID);
#ifdef SKIP_ASM
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
void cSSXScriptEngine::AddToNISListByScriptHashID(int nisList, int key, int scriptControlBits, int mainCharAIIndex,
                                                  int supportChar0AIIndex, int supportChar1AIIndex)
{
    for (int i = 0; i < mpMasterTableHeader->numScripts; i++) {
        if (mpMasterTable[i].hashID == key) {
            AddToNISListByScriptIndex(nisList, i, scriptControlBits, mainCharAIIndex, supportChar0AIIndex,
                    supportChar1AIIndex);
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
void cSSXScriptEngine::ReadyNISList(int i)
{
    tNISList_ReadyList(&mpNISLists[i], 0);
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
int cSSXScriptEngine::PlayNISList(int i, int startWithTransitionIn, int forceUpdate)
{
    mPlayingNISList = i;
    cRenderStateMan_setInNIS(1);
    int state = tNISList_Play(&mpNISLists[i], startWithTransitionIn, forceUpdate);
    if (state != keNISListState_Playing) {
        mPlayingNISList = keNISList_Num;
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
int cSSXScriptEngine::TransferNISList(int destNISList, int sourceNISList)
{
    if (destNISList == sourceNISList) {
        return 1;
    }
    tNISList* base = mpNISLists;
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
void cSSXScriptEngine::PauseNISList(int i, int isPaused)
{
    tNISList_Pause(&mpNISLists[i], isPaused);
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
void cSSXScriptEngine::SkipNISListScripts(int i, int numScriptsToSkip, int doTransition)
{
    tNISList_SkipForward(&mpNISLists[i], numScriptsToSkip, doTransition);
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
void cSSXScriptEngine::ResetNISList(int i)
{
    tNISList_Reset(&mpNISLists[i], 1);
    if (mPlayingNISList == i) {
        ClearNISPlayListState();
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
void cSSXScriptEngine::ResetNISListAll()
{
    int i;
    for (i = 0; i < 2; i++) {
        ResetNISList(i);
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
void cSSXScriptEngine::GetNISListScriptCategory(int i, int listIndex)
{
    tNISList_GetScriptCategory(&mpNISLists[i], listIndex);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetNISListVideoCategory);
#ifdef SKIP_ASM
extern "C" void tNISList_GetVideoCategory(int, int);

/**
 * Looks up the video category of entry @p listIndex of NIS list @p nisList: tNISList_GetVideoCategory on mpNISLists[@p
 * nisList].
 *
 * The C declares no return value; tNISList_GetVideoCategory's answer is left in the return register.
 *
 * @param nisList  the NIS list (0 main, 1 auxiliary transition)
 * @param listIndex  the entry index
 */
void cSSXScriptEngine::GetNISListVideoCategory(int nisList, int listIndex)
{
    tNISList_GetVideoCategory(((int)mpNISLists) + (nisList * 204), listIndex);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetNISListScriptIndex);
#ifdef SKIP_ASM
extern "C" void tNISList_GetScriptIndex(int, int);

/**
 * Looks up the script index of entry @p listIndex of NIS list @p nisList: tNISList_GetScriptIndex on mpNISLists[@p
 * nisList].
 *
 * The C declares no return value; tNISList_GetScriptIndex's answer is left in the return register.
 *
 * @param nisList  the NIS list (0 main, 1 auxiliary transition)
 * @param listIndex  the entry index
 */
void cSSXScriptEngine::GetNISListScriptIndex(int nisList, int listIndex)
{
    tNISList_GetScriptIndex(((int)mpNISLists) + (nisList * 204), listIndex);
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
void cSSXScriptEngine::GetNISListState(int i)
{
    UpdateLoad();
    tNISList_GetState(&mpNISLists[i]);
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
void cSSXScriptEngine::GetNISListPlayState(int i)
{
    tNISList_GetPlayState(&mpNISLists[i]);
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
void cSSXScriptEngine::GetNISListLength(int i)
{
    tNISList_GetLength(&mpNISLists[i]);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_IsNISListNextScriptReady);
#ifdef SKIP_ASM
extern "C" void tNISList_IsPendingScriptReady(int, int);

/**
 * Asks whether the pending script of NIS list @p nisList is ready: tNISList_IsPendingScriptReady on mpNISLists[@p
 * nisList].
 *
 * The list's byte offset is passed as an extra argument the callee ignores. The C declares no return value; the answer
 * is left in the return register.
 *
 * @param nisList  the NIS list (0 main, 1 auxiliary transition)
 */
void cSSXScriptEngine::IsNISListNextScriptReady(int nisList)
{
    int temp_5;

    temp_5 = nisList * 204;
    tNISList_IsPendingScriptReady(((int)mpNISLists) + temp_5, temp_5);
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
int cSSXScriptEngine::IsActiveNISInTransition()
{
    int i = mPlayingNISList;
    if (i == keNISList_Num) {
        return 0;
    }
    return tNISList_IsInTransition(&mpNISLists[i]);
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
int cSSXScriptEngine::IsActiveNISSkippable()
{
    int idx = mPlayingNISList;
    if (idx == keNISList_Num) {
        return 1;
    }
    int skippable = 0;
    if (mNumFramesNoSkip == 0) {
        if (tNISList_IsSkipAllowed(&mpNISLists[idx]) != 0) {
            skippable = tNISList_GetPlayState_i(&mpNISLists[mPlayingNISList]) != keNISListStatePlay_Paused;
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
int cSSXScriptEngine::IsPlayingNISListPlayer(int pScriptPlayerMain)
{
    int i = mPlayingNISList;
    if (i == keNISList_Num) {
        return 0;
    }
    return tNISList_IsPlayingScriptOwner(&mpNISLists[i], pScriptPlayerMain);
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
void cSSXScriptEngine::AddToIdleListByScriptCategory(int i, int scriptCategory, int flag, int mainCharAIIndex,
                                                     int supportChar0AIIndex, int supportChar1AIIndex)
{
    tNISList_AddByScriptCategory(&mpIdleLists[i], scriptCategory, flag ? 8 : 0, 0, mainCharAIIndex,
            supportChar0AIIndex, supportChar1AIIndex);
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
void cSSXScriptEngine::ReadyIdleList(int i, int flag)
{
    tNISList_ReadyList(&mpIdleLists[i], flag ? 2 : 0);
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
void cSSXScriptEngine::PlayIdleList(int i)
{
    tNISList_Play(&mpIdleLists[i], 0, 0);
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
void cSSXScriptEngine::ResetIdleList(int i)
{
    tNISList_Reset(&mpIdleLists[i], 1);
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
void cSSXScriptEngine::ResetIdleListAll()
{
    for (int i = 0; i < 2; i++) {
        tNISList_Reset(&mpIdleLists[i], 1);
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
void cSSXScriptEngine::GetIdleListState(int i)
{
    UpdateLoad();
    tNISList_GetState(&mpIdleLists[i]);
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
int cSSXScriptEngine::RequestScriptByCategory(int category, int mainCharAIIndex, int supportChar0AIIndex,
                                              int supportChar1AIIndex)
{
    // PORT: one variable holds the script pointer and then the result
    int outcome = (int)GetScriptFromCategory(category);
    if (outcome == 0) return -1;
    int* node = (int*)AllocScriptSetupInfo();
    node[0] = mainCharAIIndex;
    node[1] = supportChar0AIIndex;
    node[2] = supportChar1AIIndex;
    outcome = cBXScriptEngine_RequestAnimationScript(this, *(int*)outcome, node);
    if (outcome < 0) {
        ReleaseScriptSetupInfo(node);
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
int cSSXScriptEngine::RequestScriptByIndex(int key, int mainCharAIIndex, int supportChar0AIIndex,
                                           int supportChar1AIIndex)
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
    int* node = (int*)AllocScriptSetupInfo();
    node[0] = mainCharAIIndex;
    node[1] = supportChar0AIIndex;
    node[2] = supportChar1AIIndex;
    int requestId = cBXScriptEngine_RequestAnimationScript(this, key, node);
    if (requestId < 0) {
        ReleaseScriptSetupInfo(node);
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
int cSSXScriptEngine::RequestScriptByHashID(int id, int mainCharAIIndex, int supportChar0AIIndex,
                                            int supportChar1AIIndex)
{
    int i = 0;
    if (*(int*)mpMasterTableHeader > 0) {
        do {
            if (((int)(mpMasterTable)[i].hashID) == id)
                return RequestScriptByIndex(i, mainCharAIIndex, supportChar0AIIndex, supportChar1AIIndex);
            i++;
        } while (i < *(int*)mpMasterTableHeader);
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
void cSSXScriptEngine::PlayRequestScript(int val, int key, int autoRecycle, int loop)
{
    tSSXScriptSetupInfo* setupInfo = (tSSXScriptSetupInfo*)cSSXScriptEngine_GetScriptSetupInfo_2(
        this, cBXScriptEngine_GetRequestPlayerByReqHandle(this, key));
    setupInfo->viewIndex = val;
    cBXScriptEngine_PlayScript(this, key, autoRecycle, loop);
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
int cSSXScriptEngine::SelectScriptFromCategory(int category)
{
    int* script = GetScriptFromCategory(category);
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
int cSSXScriptEngine::OnDataUnavailable(int key)
{
    char buf[128];
    sprintf(buf, str_pct_spct_08d_big, str_data_scripts_scdat_big_data_scripts, key);
    AsyncLoadScriptBank(GetNextAvailableAsyncBankID(), buf, 0x100, key, 0, 0);
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
void cSSXScriptEngine::OnPendingDataCancelled(int scriptDataIndex)
{
    CancelAsyncLoadScriptBank(GetBankNumberByDataIdentifier(scriptDataIndex, 2), scriptDataIndex);
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
void cSSXScriptEngine::OnScriptDataReleased(int scriptDataIndex)
{
    int idx = GetBankNumberByDataIdentifier(scriptDataIndex, 2);
    if (idx >= 0) {
        PurgeScriptBank(idx);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnAttachData);
#ifdef SKIP_ASM
extern "C" int strncmp(const char* a, const char* b, int n);
tSSXScriptHeader* GetSSXScriptHeader1(void* self);
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
 * @param kind  receives the data kind (eScriptBankDataKind)
 * @param bank  receives the bank number for animation and sound data
 */
void cSSXScriptEngine::OnAttachData(const char* ext, void* data, int dataSize, int* kind, int* bank)
{
    if (strncmp(str_scr, ext, 3) == 0) {
        tSSXScriptHeader* bankHeader = (tSSXScriptHeader*)GetSSXScriptHeader1(data);
        bankHeader->bankNum = mCurrAttachingBankNum;
        *kind = keScriptBankData_Script;
        cBXScriptEngine_AttachScript(((char*)this), data, bankHeader->scriptIndex);
    } else if (strncmp(str_anm, ext, 3) == 0) {
        *kind = keScriptBankData_Animation;
        *bank = mCurrAttachingBankNum;
        cScriptAnimBankManager_LinkBank(&mAnimationBankManager, mCurrAttachingBankNum, data);
    } else if (strncmp(str_snd, ext, 3) == 0) {
        *kind = keScriptBankData_Sound;
        *bank = mCurrAttachingBankNum;
        cScriptSoundBankManager_LinkBank(mSoundBankManager, mCurrAttachingBankNum, data, dataSize);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_OnDetachData);
#ifdef SKIP_ASM
tSSXScriptHeader* GetSSXScriptHeader1(void* self);
extern "C" void cBXScriptEngine_DetachScript(void* self, int a1);
extern "C" void cScriptAnimBankManager_UnlinkBank(void* list, int id);
extern "C" void cScriptSoundBankManager_UnlinkBank1(void* list, int id);

/**
 * Handles data detached from a script bank: unregisters the script or unlinks the animation or sound bank.
 *
 * @param obj  the detached data
 * @param dataSize  size of the data (unused)
 * @param type  data kind stored by OnAttachData() (eScriptBankDataKind)
 * @param id  bank number stored by OnAttachData()
 */
void cSSXScriptEngine::OnDetachData(void* obj, int dataSize, int type, int id)
{
    switch (type) {
    case keScriptBankData_Script:
        cBXScriptEngine_DetachScript(this, GetSSXScriptHeader1(obj)->scriptIndex);
        break;
    case keScriptBankData_Animation:
        cScriptAnimBankManager_UnlinkBank(&mAnimationBankManager, id);
        break;
    case keScriptBankData_Sound:
        cScriptSoundBankManager_UnlinkBank1((char*)&mSoundBankManager, id);
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
tSSXHeaderActor* GetSSXActorHeader(void* pPlayer);
// PORT: GetSSXActorHeader1 forwards (self, index) to cScriptPlayerEntity_GetHeader; the unit declares it with one arg.
void* GetSSXActorHeader1_9A70(void* self, int i) __asm__("GetSSXActorHeader1__FPv");
// PORT: callers elsewhere declare cSSXScriptEngine_BuildCharacterBitRoleArray with two args; it takes the three player
// slots too.
extern "C" void cSSXScriptEngine_BuildCharacterBitRoleArray_9A70(void* self, int* flags, int p0, int p1, int p2) __asm__("cSSXScriptEngine_BuildCharacterBitRoleArray");
extern "C" void* cScriptPlayerMain_GetEntity(void* self, int i);
extern "C" void cScriptPlayerEntity_SetEntityVariation(void* self, int idx);
extern int kRoleToFilterEnumMappingTable[];





/**
 * Picks a random variation for each entity of a script that is starting, then counts the play.
 *
 * Choice entities (keSSXEntityType_Actor) only pick options whose character mask matches the cast (flags built from the
 * three AI
 * indices), and a picked option tied to a cast slot uses that slot up; other entities pick any variation. The
 * script's play count in the master table is incremented.
 *
 * @param script  the script player starting the script
 * @param idx  the script's master-table index
 * @param players  the request's setup info: the three AI indices of the cast
 */
// Picks a random option for every choice event of a script, honouring the per-slot team flags.
void cSSXScriptEngine::OnStartScript(cScriptPlayerMain* script, int idx, int* players)
{
    int flags[21];
    int cands[32];
    unsigned int all = 0;
    int i;
    cSSXScriptEngine_BuildCharacterBitRoleArray_9A70(this, flags, players[0], players[1], players[2]);
    {
        int j;
        for (j = 0; j < 21; j++)
            all |= flags[j];
    }
    for (i = 0; i < (script->mpSystemHeader)->numEntities; i++) {
        void* ev = cScriptPlayerMain_GetEntity(script, i);
        if (cScriptPlayerEntity_GetType(ev) == keSSXEntityType_Actor) {
            int cnt = cScriptPlayerEntity_GetNumVariations(ev);
            if (cnt >= 2) {
                int numCands = 0;
                int k;
                for (k = 0; k < cnt; k++) {
                    tSSXHeaderActor* opt = (tSSXHeaderActor*)GetSSXActorHeader1_9A70(ev, k);
                    int slot = kRoleToFilterEnumMappingTable[opt->role];
                    if (slot >= 0) {
                        if (opt->character == 0 || (opt->character & flags[slot]) != 0)
                            cands[numCands++] = k;
                    } else {
                        if (opt->character == 0 || (opt->character & all) != 0)
                            cands[numCands++] = k;
                    }
                }
                if (numCands > 0) {
                    cScriptPlayerEntity_SetEntityVariation(ev, cands[BXrand() % numCands]);
                    tSSXHeaderActor* opt = (tSSXHeaderActor*)GetSSXActorHeader(ev);
                    int slot = kRoleToFilterEnumMappingTable[opt->role];
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
    mpMasterTable[idx].numTimesPlayed++;
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
void cSSXScriptEngine::OnReleaseRequest(int requestHandle, void* pUserData)
{
    ReleaseScriptSetupInfo(pUserData);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AddObject);
#ifdef SKIP_ASM
extern "C" int cScriptDatabaseObject_AddObject(cScriptDatabaseObject* self, void* obj);
extern "C" int cScriptObjCameraManager_AddCameraObject(cScriptObjCameraManager* self, cScriptObjCamera* obj);
extern "C" int cScriptObjCameraManager_SetTransitionObject(cScriptObjCameraManager* self, void* obj);

/**
 * Adds a script object to the database and hooks cameras and transitions up to their camera manager.
 *
 * A camera (keScriptObjectType_Camera) joins the camera manager its word at +0x7C selects; a transition
 * (keScriptObjectType_Transition) is set on the
 * camera manager its virtual slot 9 names.
 *
 * @param obj  the script object
 * @return the object's database ID
 */
int cSSXScriptEngine::AddObject(cScriptObject* obj)
{
    int result = ((cScriptDatabaseObject*)(this))->AddObject(obj);
    if (obj->mObjType == keScriptObjectType_Camera) {
        cScriptObjCamera* camera = (cScriptObjCamera*)obj;
        mObjectCameraManager[camera->mViewIndex].AddCameraObject(camera);
    } else if (obj->mObjType == keScriptObjectType_Transition) {
        // cScriptObjTransition's slot 9, GetViewIndex (pure virtual there)
        cScriptObjTransition* transition = (cScriptObjTransition*)obj;
        sVEntry<int(void*)>* vt = (sVEntry<int(void*)>*)transition->mVtbl;
        (mObjectCameraManager[vt[9].fn(vthis(transition, vt[9]))]).SetTransitionObject(obj);
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

/**
 * Removes a script object from the database, first unhooking a camera or transition from its camera manager.
 *
 * On Tour returns a bool; this declaration is void.
 *
 * @param idx  the object's database ID
 */
void cSSXScriptEngine::RemoveObject(int idx)
{
    // PORT: cScriptDatabaseObject_GetObject returns a table pointer as int
    cScriptObject* obj = (cScriptObject*)cScriptDatabaseObject_GetObject((this), idx);
    if (obj->mObjType == keScriptObjectType_Camera) {
        cScriptObjCamera* camera = (cScriptObjCamera*)obj;
        cScriptObjCameraManager_RemoveCameraObject(&mObjectCameraManager[camera->mViewIndex], obj);
    } else if (obj->mObjType == keScriptObjectType_Transition) {
        // cScriptObjTransition's slot 9, GetViewIndex (pure virtual there)
        cScriptObjTransition* transition = (cScriptObjTransition*)obj;
        sVEntry<int(void*)>* vt = (sVEntry<int(void*)>*)transition->mVtbl;
        (mObjectCameraManager[vt[9].fn(vthis(transition, vt[9]))]).SetTransitionObject(0);
    }
    ((cScriptDatabaseObject*)(this))->RemoveObject(idx);
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
 * Cameras (keScriptObjectType_Camera) and transitions (keScriptObjectType_Transition) have their own checks, others use
 * the database's. On Tour returns the answer; this declaration is void.
 *
 * @param i  the object's database ID
 * @param val  player ID
 */
void cSSXScriptEngine::IsObjInUseByPlayer(int i, int val)
{
    // PORT: cScriptDatabaseObject_GetObject returns a table pointer as int
    int* obj = (int*)cScriptDatabaseObject_GetObject(this, i);
    int type = *obj;
    if (type == keScriptObjectType_Camera) {
        ((cScriptObjCamera*)obj)->IsInUseByPlayer(val);
    } else if (type != keScriptObjectType_Transition) {
        cScriptDatabaseObject_IsObjInUseByPlayer(this, i, val);
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
 * Forwards to cScriptAnimBankManager_GetBankInfo (On Tour cScriptAnimBankManager::GetBankInfo) on mAnimationBankManager
 * (+0xA28). The C declares only the engine: the bank number arrives in $a1 and passes through
 * (cScriptObjRider_OnStartObjectUse binds a two-argument alias). On Tour asserts the bank number is in range; retail
 * does not.
 *
 * @return the bank's tScriptAnimBankInfo record, or 0 if none is linked
 */
void* cSSXScriptEngine::GetAnimationBankInfo()
{
    return cScriptAnimBankManager_GetBankInfo(&mAnimationBankManager);
}
#endif

extern "C" void* cScriptSoundBankManager_GetBankInfo(void*);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetSoundBankInfo__FPv);
#ifdef SKIP_ASM
/**
 * Returns the sound bank info of a script data bank.
 *
 * Forwards to cScriptSoundBankManager_GetBankInfo (On Tour cScriptSoundBankManager::GetBankInfo) on mSoundBankManager
 * (+0xA38). The C declares only the engine: the bank number arrives in $a1 and passes through (callers bind a
 * two-argument alias).
 *
 * @return the bank's tScriptSoundBankInfo record
 */
void* cSSXScriptEngine::GetSoundBankInfo()
{
    return cScriptSoundBankManager_GetBankInfo(mSoundBankManager);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetSubjectRider);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// The race's AI manager (an inline call: retail reloads SSXApp->Game->AI for each use).
static inline cAI* subjectAI()
{
    return SSXApp->Game->AI;
}

// A player's rider, or 0 for no player.
static inline void* playerRider(cPlayer* player)
{
    return player ? player->mRider : 0;
}

// A computer rider's rider (its first part's virtual-base pointer), or 0 for none.
static inline void* computerRider(cComputer* computer)
{
    return computer ? computer->RiderHandler.GroundMotion.mRider : 0;
}

/**
 * Returns the rider a script subject refers to.
 *
 * Subjects 0-2 are the request's main and supporting characters (by AI index), 3-4 the human players, 5-9 the
 * computer riders then the actors, 10-14 the actors then the computer riders, and 15-20 the riders by index.
 *
 * @param slot  subject type (eSSXScriptSubject)
 * @param pPlayer  the script player whose request supplies the cast
 * @return the rider, or 0 when the subject has no rider
 */
void* cSSXScriptEngine::GetSubjectRider(int slot, int pPlayer)
{
    int riderIndex;
    switch (slot) {
    case keSSXSubject_MainChar: {
        tSSXScriptSetupInfo* setupInfo = (tSSXScriptSetupInfo*)cSSXScriptEngine_GetScriptSetupInfo_2(this, pPlayer);
        if (setupInfo->mainCharAIIndex < 0) break;
        return subjectAI()->Riders[setupInfo->mainCharAIIndex];
    }
    case keSSXSubject_SupportChar0:
        riderIndex = ((tSSXScriptSetupInfo*)cSSXScriptEngine_GetScriptSetupInfo_2(this, pPlayer))->supportChar0AIIndex;
        if (riderIndex < 0) break;
        return subjectAI()->Riders[riderIndex];
    case keSSXSubject_SupportChar1:
        riderIndex = ((tSSXScriptSetupInfo*)cSSXScriptEngine_GetScriptSetupInfo_2(this, pPlayer))->supportChar1AIIndex;
        if (riderIndex < 0) break;
        return subjectAI()->Riders[riderIndex];
    case keSSXSubject_Player0: case keSSXSubject_Player1: {
        cAI* ai = subjectAI();
        int j = slot - keSSXSubject_Player0;
        if (j >= ai->NumberOfPlayersAllocated) break;
        return playerRider(ai->Players[j]);
    }
    case keSSXSubject_Computer0: case keSSXSubject_Computer1: case keSSXSubject_Computer2: case keSSXSubject_Computer3:
    case keSSXSubject_Computer4: {
        cAI* ai = subjectAI();
        int j = slot - keSSXSubject_Computer0;
        if (j >= ai->NumberOfComputersAllocated + ai->NumberOfActorsAllocated) break;
        if (j < ai->NumberOfComputersAllocated) return computerRider(ai->Computers[j]);
        return computerRider(ai->Actors[j - ai->NumberOfComputersAllocated]);
    }
    case keSSXSubject_Actor0: case keSSXSubject_Actor1: case keSSXSubject_Actor2: case keSSXSubject_Actor3:
    case keSSXSubject_Actor4: {
        cAI* ai = subjectAI();
        int j = slot - keSSXSubject_Actor0;
        if (j >= ai->NumberOfActorsAllocated + ai->NumberOfComputersAllocated) break;
        if (j < ai->NumberOfActorsAllocated) return computerRider(ai->Actors[j]);
        return computerRider(ai->Computers[j - ai->NumberOfActorsAllocated]);
    }
    case keSSXSubject_Rider0: case keSSXSubject_Rider1: case keSSXSubject_Rider2: case keSSXSubject_Rider3:
    case keSSXSubject_Rider4: case keSSXSubject_Rider5: {
        cAI* ai = subjectAI();
        int j = slot - keSSXSubject_Rider0;
        if (j >= ai->NumberOfRidersAllocated) break;
        return ai->Riders[j];
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
extern int D_004A51D8;
extern t3Vector D_004D3770[3];
// D_004D3680[20..22] is D_004D3770[0..2]; the lookup indexes by the raw id.
extern t3Vector D_004D3680[];
extern "C" void cSSXScriptEngine_FillOrientationFromStartPointInfo_v(void* self, void* obj, sVec_27A0D8* pos, int* flag, float* pitch, float* yaw)
    __asm__("cSSXScriptEngine_FillOrientationFromStartPointInfo");
extern "C" void cSSXScriptEngine_FillOrientationFromRiderOrientationInfo_v(void* self, void* obj, sVec_27A0D8* pos, int* flag, float* pitch, float* yaw)
    __asm__("cSSXScriptEngine_FillOrientationFromRiderOrientationInfo");
extern "C" void cSSXScriptEngine_FillOrientationFromObjectOrientationInfo_v(char* self, int slot, sVec_27A0D8* pos, int* flag, float* pitch, float* yaw, int cat,
                                const sVec_27A0D8* off) __asm__("cSSXScriptEngine_FillOrientationFromObjectOrientationInfo");
// PORT: a byte view of GetSSXScriptHeader's result (its first byte, read signed), bound by asm label.
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
 * the ground: z becomes the hit point's z from cBXNash_getGroundHeight on the game's BXNash; 25 first lowers z by 1000.
 * Other ids leave the cleared values.
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
        void* subjectRider = ((cSSXScriptEngine*)self)->GetSubjectRider(id - 40, (int)obj);
        if (subjectRider != 0)
            cSSXScriptEngine_FillOrientationFromRiderOrientationInfo_v(self, subjectRider, pos, baseRotX, baseRotY, baseRotZ);
        break;
    }
    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12:
    case 13: case 14: case 15: case 16: case 17: case 18: {
        void* subjectRider = ((cSSXScriptEngine*)self)->GetSubjectRider(id + 2, (int)obj);
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
        t3Vector* anchorOffset = &D_004D3680[id];
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
 * keScriptTransition_Fade and keScriptTransition_CrossFade are fades (CrossFade always uses white), WipeLeft,
 * WipeRight, WipeUp and WipeDown directional wipes and WipeCircle a circle; CameraBlend and Cut make nothing. Colour
 * keScriptTransitionColour_Black is (1, 0, 0, 0) and White all ones (tARGB's A, R, G, B).
 *
 * @param type  transition type (eScriptTransition)
 * @param mode  transition colour (eScriptTransitionColour)
 * @return the new transition object, or 0
 */
void* cSSXScriptEngine::MakeTransitionObject(int type, int mode)
{
    tARGB color;
    switch (mode) {
    case keScriptTransitionColour_Black:
        color.A = 1.0f;
        color.R = 0.0f;
        color.G = 0.0f;
        color.B = 0.0f;
        break;
    case keScriptTransitionColour_White:
        color.A = 1.0f;
        color.R = 1.0f;
        color.G = 1.0f;
        color.B = 1.0f;
        break;
    }
    switch (type) {
    case keScriptTransition_Fade: {
        cFade_TObj* fxObj = (cFade_TObj*)cTransitionObj_operator_new_sz(20);
        tARGB fxColor = color;
        return initFx_27A4A0(fxObj, fxColor);
    }
    case keScriptTransition_WipeLeft:
        return cWipe_TObj_cWipe_TObj(cTransitionObj_operator_new_sz(0x1C), &color, -1, 0);
    case keScriptTransition_WipeRight:
        return cWipe_TObj_cWipe_TObj(cTransitionObj_operator_new_sz(0x1C), &color, 1, 0);
    case keScriptTransition_WipeUp:
        return cWipe_TObj_cWipe_TObj(cTransitionObj_operator_new_sz(0x1C), &color, 0, -1);
    case keScriptTransition_WipeDown:
        return cWipe_TObj_cWipe_TObj(cTransitionObj_operator_new_sz(0x1C), &color, 0, 1);
    case keScriptTransition_WipeCircle:
        return cCircle_TObj_cCircle_TObj(cTransitionObj_operator_new_sz(0x1C), &color);
    case keScriptTransition_CrossFade: {
        cFade_TObj* fxObj = (cFade_TObj*)cTransitionObj_operator_new_sz(20);
        tARGB fxColor;
        fxColor.A = 1.0f;
        fxColor.R = 1.0f;
        fxColor.G = 1.0f;
        fxColor.B = 1.0f;
        return initFx_27A4A0(fxObj, fxColor);
    }
    case keScriptTransition_CameraBlend:
    case keScriptTransition_Cut:
        break;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetScriptSetupInfo__FPv);
#ifdef SKIP_ASM
/**
 * Returns the script setup info of a script player: the user data of its request (cBXScriptEngine::GetRequestUserData).
 *
 * @param pPlayer  the script player
 * @return the setup info
 */
tSSXScriptSetupInfo* cSSXScriptEngine::GetScriptSetupInfo(cScriptPlayerMain* pPlayer)
{
    return (tSSXScriptSetupInfo*)((cBXScriptEngine*)this)->GetRequestUserData(pPlayer);
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
void cSSXScriptEngine::OnInSeqPlay(cScriptPlayerMain* pPlayer)
{
    cBXScriptEngine_OnInSeqPlay(this, pPlayer);
    int listIndex = mPlayingNISList;
    if (listIndex != keNISList_Num) {
        tNISList_OnScriptPlay(&mpNISLists[listIndex], cBXScriptEngine_GetRequestHandleFromPlayer(this, pPlayer),
                pPlayer);
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
void cSSXScriptEngine::OnInSeqUpdate(cScriptPlayerMain* pPlayer, float x, float y)
{
    int listIndex = mPlayingNISList;
    if (listIndex != keNISList_Num) {
        tNISList_OnScriptUpdate(&mpNISLists[listIndex], cBXScriptEngine_GetRequestHandleFromPlayer(this, pPlayer),
                pPlayer, x, y);
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
void cSSXScriptEngine::OnInSeqPlayStopping(cScriptPlayerMain* pPlayer)
{
    int listIndex = mPlayingNISList;
    if (listIndex != keNISList_Num) {
        tNISList_OnScriptStopping(&mpNISLists[listIndex], cBXScriptEngine_GetRequestHandleFromPlayer(this, pPlayer),
                pPlayer);
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
void cSSXScriptEngine::OnInSeqStop(cScriptPlayerMain* pPlayer)
{
    int listIndex = mPlayingNISList;
    if (listIndex != keNISList_Num) {
        tNISList_OnScriptStop(&mpNISLists[listIndex], cBXScriptEngine_GetRequestHandleFromPlayer(this, pPlayer),
                pPlayer);
    }
    cBXScriptEngine_OnInSeqStop(this, pPlayer);
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
 * Queues and plays the transport cut-scene sequence in NIS list keNISList_AuxTransition.
 *
 * Uses the gondola categories (keSSXScriptCategory_wctransportgondola*) when @p online is set and the heli ones
 * otherwise: leave, in-transit start, then the in-transit loop, or its mp variant when more than one player is
 * allocated. On Tour names the parameters isGondolaSequence and doLeave, so @p online selects the gondola, not online
 * play.
 *
 * @param online  selects the gondola sequence (On Tour isGondolaSequence)
 * @param intro  nonzero to start with the leave script (On Tour doLeave)
 */
void cSSXScriptEngine::PlayTransportSequence(int online, int intro)
{
    if (online) {
        if (intro) {
            AddToNISListByScriptCategory(keNISList_AuxTransition, keSSXScriptCategory_wctransportgondolaleave, 1, 1,
                -1, -1, -1);
        }
        AddToNISListByScriptCategory(keNISList_AuxTransition, keSSXScriptCategory_wctransportgondolaintransitstart, 0,
            0, -1, -1, -1);
        if (sseWorldMode_A860() == 1) {
            AddToNISListByScriptCategory(keNISList_AuxTransition, keSSXScriptCategory_wctransportgondolaintransitloop,
                8, 0, -1, -1, -1);
        } else {
            AddToNISListByScriptCategory(keNISList_AuxTransition,
                keSSXScriptCategory_wctransportgondolaintransitloopmp, 8, 0, -1, -1, -1);
        }
    } else {
        if (intro) {
            AddToNISListByScriptCategory(keNISList_AuxTransition, keSSXScriptCategory_wctransporthelileave, 1, 1, -1,
                -1, -1);
        }
        AddToNISListByScriptCategory(keNISList_AuxTransition, keSSXScriptCategory_wctransportheliintransitstart, 0, 1,
            -1, -1, -1);
        GrMan->mSkipNextPageFlip = 1;
        if (sseWorldMode_A860() == 1) {
            AddToNISListByScriptCategory(keNISList_AuxTransition, keSSXScriptCategory_wctransportheliintransitloop, 8,
                1, -1, -1, -1);
        } else {
            AddToNISListByScriptCategory(keNISList_AuxTransition, keSSXScriptCategory_wctransportheliintransitloopmp,
                8, 0, -1, -1, -1);
        }
    }
    ReadyNISList(keNISList_AuxTransition);
    PlayNISList(keNISList_AuxTransition, 0, 1);
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
 * Skips the rest of the transport sequence playing in NIS list keNISList_AuxTransition.
 *
 * The number of scripts skipped depends on the category of the list's first entry: 3 for the heli or gondola leave, 2
 * for the in-transit start, 1 for the in-transit loop, none otherwise.
 *
 * @return true when scripts were skipped
 */
bool cSSXScriptEngine::EndTransportSequence()
{
    int skipMode = 0;
    switch (cSSXScriptEngine_GetNISListScriptCategory_r(this, keNISList_AuxTransition, 0)) {
    case keSSXScriptCategory_wctransporthelileave: case keSSXScriptCategory_wctransportgondolaleave:
        skipMode = 3;
        break;
    case keSSXScriptCategory_wctransportheliintransitstart: case keSSXScriptCategory_wctransportgondolaintransitstart:
        skipMode = 2;
        break;
    case keSSXScriptCategory_wctransportheliintransitloop: case keSSXScriptCategory_wctransportgondolaintransitloop:
        skipMode = 1;
        break;
    case keSSXScriptCategory_wctransportheliintransitloopmp: case keSSXScriptCategory_wctransporthelibcarrive:
    case keSSXScriptCategory_wctransporthelibcunload:
        break;
    }
    SkipNISListScripts(keNISList_AuxTransition, skipMode, 1);
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
 * Tells whether NIS list keNISList_AuxTransition is playing one of the transport sequence's first three scripts.
 *
 * @return true when that list is playing and its first entry is a heli or gondola leave, in-transit start or
 *         in-transit loop
 */
bool cSSXScriptEngine::IsInTransportSequence()
{
    if (mPlayingNISList != keNISList_AuxTransition) {
        return false;
    }
    int category = cSSXScriptEngine_GetNISListScriptCategory_int(this, keNISList_AuxTransition, 0);
    bool inTransport = false;
    if (category == keSSXScriptCategory_wctransportgondolaleave || category == keSSXScriptCategory_wctransporthelileave
        || category == keSSXScriptCategory_wctransportgondolaintransitstart
        || category == keSSXScriptCategory_wctransportheliintransitstart
        || category == keSSXScriptCategory_wctransportgondolaintransitloop
        || category == keSSXScriptCategory_wctransportheliintransitloop) {
        inTransport = true;
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
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern int* kpFlyInScriptSequenceTable[];
extern int kTrackStartSequenceMPNet[];

/**
 * Queues the race-start cut-scenes for a start situation in NIS list keNISList_Main.
 *
 * keNISStartSituation_InitEvent adds the intro scripts listed for the game path (or the online list); RestartEvent and
 * NextHeat add keSSXScriptCategory_wcracestartframe; LastHeat adds keSSXScriptCategory_wcracestartgate and then the
 * frame.
 *
 * @param state  start situation (On Tour eNISStartSituation)
 */
void cSSXScriptEngine::AppendStartSequence(int state)
{
    switch (state) {
    case keNISStartSituation_InitEvent: {
        cBE_getInterface_Fv(cBE_getBE(), 0);
        int* tbl = kpFlyInScriptSequenceTable[cBENewRaceInterface_newRace.cGamePath];
        cBE_getInterface_Fv(SSXApp->BE, 7);
        if (cBENetworkInterface_networkOpt.bIsNetworkGame != 0) {
            tbl = kTrackStartSequenceMPNet;
        }
        if (tbl != 0 && tbl[0] >= 0) {
            do {
                ScriptEngine->AddToNISListByScriptCategory(keNISList_Main, tbl[0], tbl[1], 0, -1, -1, -1);
                tbl += 2;
            } while (tbl[0] >= 0);
        }
        break;
    }
    case keNISStartSituation_RestartEvent:
    case keNISStartSituation_NextHeat:
        ScriptEngine->AddToNISListByScriptCategory(keNISList_Main, keSSXScriptCategory_wcracestartframe, 0, 0, -1, -1,
            -1);
        break;
    case keNISStartSituation_LastHeat:
        ScriptEngine->AddToNISListByScriptCategory(keNISList_Main, keSSXScriptCategory_wcracestartgate, 3, 0, -1, -1,
            -1);
        ScriptEngine->AddToNISListByScriptCategory(keNISList_Main, keSSXScriptCategory_wcracestartframe, 0, 0, -1, -1,
            -1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_AppendEndEventSequence);
#ifdef SKIP_ASM
extern "C" tBELocationDB* cBENewRaceInterface_getLocation(void* iface);
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
 * unlock scene (categories 24 and 25) depending on cBENewPlayerInterface_isRivalChallengePlayed() and
 * cBENewPlayerInterface_isRivalEventLocked(). Mode 4 and unfinished events queue nothing.
 */
void cSSXScriptEngine::AppendEndEventSequence()
{
    void* econ = cBE_getInterface_Fv(cBE_getBE(), 0);
    cBERaceDB* beRaceDB = &(*(&cBENewRaceInterface_newRace));
    int mode = beRaceDB->cGameMode;
    if (mode == kGAMEMODE_FREERIDE) {
        return;
    }
    if (SSXApp->GameModeMan->bIsLastHeat == 0) {
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
            k = keSSXScriptCategory_wcraceendpodiumchampion;
            int cheatCharID = cBELibrary_getCheatCharacterID(Call_AC60(list->riders[ids[0]], 7));
            if ((unsigned)(cheatCharID - 10) < 11 || cheatCharID == 0x1C) {
                k = keSSXScriptCategory_wcraceendpodiumchampioncheat;
            }
        }
        ScriptEngine->AddToNISListByScriptCategory(0, k, 1, 0, ids[0], ids[1], ids[2]);
    }
    if (cBENewRaceInterface_newRace.cGamePath != 0) {
        return;
    }
    if ((unsigned)mode >= 4) {
        return;
    }
    void* players = cBE_getInterface_Fv(cBE_getBE(), 1);
    tBELocationDB blk = *cBENewRaceInterface_getLocation(econ);
    int first = mode == 0;
    int idx = cBENewPlayerInterface_getHumanRider(players, 0);
    int result = cBENewPlayerInterface_isRivalChallengePlayed(players, idx, blk.segmentID, first);
    if ((unsigned)(cBELibrary_getCheatCharacterID(idx) - 10) < 11) {
        return;
    }
    int rc = cBENewPlayerInterface_getRiderCharID(players, idx);
    if (cBENewPlayerInterface_isRivalEventLocked(players, idx, rc, blk.segmentID, first)) {
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
    ScriptEngine->AddToNISListByScriptCategory(0, 0x18, 1, 0, aiIndex, -1, -1);
    ScriptEngine->AddToNISListByScriptCategory(0, 25, 3, 0, aiIndex, -1, -1);
    cBENewPlayerInterface_setRivalChallengePlayed(players, idx, blk.segmentID, first, 1);
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





/**
 * Picks a random script from a category among the least-played ones whose cast requirements are met.
 *
 * A script qualifies when each of its 21 requirement masks matches the current cast flags; among those only
 * the
 * ones with the lowest master-table play count are kept. Uses a temporary 100-entry list ("ScriptChoice").
 *
 * @param cat  script category
 * @return the chosen script's filter record (its first word is the script index), or 0
 */
int* cSSXScriptEngine::GetScriptFromCategory(int cat)
{
    cSSXScriptEngine* self = this;
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
        int pri = (self->mpMasterTable)[script->scriptIndex].numTimesPlayed;
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
void cSSXScriptEngine::SyncLoadScriptBank(int i, const char* name, int flags, int key, int* buf)
{
    tSSXScriptBankInfo* bankInfo = (&mScriptDataBanks[i]);
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
    mCurrAttachingBankNum = i;
    (*(int*)&bankInfo->dataBankHandle) = cDataManager_AttachData(&mMaxDataInstances, name, data, size);
    mCurrAttachingBankNum = 10;
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
void cSSXScriptEngine::AsyncLoadScriptBank(int i, const char* name, int flags, int key, int pMem, int memSize)
{
    tSSXScriptBankInfo* slot = &mScriptDataBanks[i];
    slot->dataIdentifier = key;
    if (pMem != 0) {
        slot->asyncHandle = ASYNCFILE_loadat(name, pMem, memSize);
        slot->dynamicMem = 0;
    } else {
        slot->asyncHandle = ASYNCFILE_load(name, flags, memSize);
        slot->dynamicMem = 1;
    }
    if (slot->asyncHandle != 0) {
        mNumAsyncs += 1;
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
void cSSXScriptEngine::PurgeScriptBank(int i)
{
    tSSXScriptBankInfo* slot = &mScriptDataBanks[i];
    int key = *(int*)slot;
    if (key != 0) {
        void* bankData = cDataManager_DetachData((char*)&mMaxDataInstances, key);
        if (slot->dynamicMem != 0 && bankData != 0) {
            cMemMan_free(bankData);
        }
    } else if (slot->asyncHandle != 0) {
        ASYNCFILE_cancel(slot->asyncHandle);
        ASYNCFILE_release(slot->asyncHandle, 0, 0);
        mNumAsyncs -= 1;
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
void cSSXScriptEngine::CancelAsyncLoadScriptBank(int i, int dataIdentifier)
{
    tSSXScriptBankInfo* slot = &mScriptDataBanks[i];
    if (slot->asyncHandle != 0) {
        ASYNCFILE_cancel(slot->asyncHandle);
        ASYNCFILE_release(slot->asyncHandle, 0, 0);
        mNumAsyncs -= 1;
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
int cSSXScriptEngine::GetNextAvailableAsyncBankID()
{
    for (int i = 2; i < 10; i++) {
        if (isSlotFree(&((&mScriptDataBanks[0]))[i])) {
            return i;
        }
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_GetBankNumberByDataIdentifier);
#ifdef SKIP_ASM
// Whether a bank slot is empty: no async load (asyncHandle, the word after dataIdentifier) and no attached data
// (dataBankHandle, the word before it).
static inline int bankIsEmpty(int* dataIdentifier)
{
    return dataIdentifier[1] == 0 && dataIdentifier[-1] == 0;
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
int cSSXScriptEngine::GetBankNumberByDataIdentifier(int key, int start)
{
    for (int i = start; i < 10; i++) {
        int* k = &mScriptDataBanks[0].dataIdentifier + i * 4;  // bank i's (4 words per bank)
        if (!bankIsEmpty(k) && *k == key) {
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
void cSSXScriptEngine::ProcessAsyncLoadScriptBankComplete(void* file)
{
    char buf[128];
    struct {
        int* data;
        int size;
    } loaded;
    for (int i = 0; i < 10; i++) {
        tSSXScriptBankInfo* ssxScriptBankInfo = &((&mScriptDataBanks[0]))[i];
        if ((void*)((int)ssxScriptBankInfo->asyncHandle) == file) {
            ASYNCFILE_release_out(file, &loaded.data, &loaded.size);
            mNumAsyncs -= 1;
            if (loaded.size == 0) {
                tSSXScriptBankInfo_Clear(ssxScriptBankInfo);
                return;
            }
            sprintf(buf, str_asyncpct_08d, ssxScriptBankInfo->dataIdentifier);
            *(int*)&ssxScriptBankInfo->asyncHandle = 0;
            mCurrAttachingBankNum = i;
            *(int*)&ssxScriptBankInfo->dataBankHandle = cDataManager_AttachData((char*)&mMaxDataInstances, buf,
                    loaded.data, loaded.size);
            mCurrAttachingBankNum = 10;
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


/**
 * Takes the loaded track filter table and turns its category offsets into pointers.
 *
 * @param file  the finished async file
 */
void cSSXScriptEngine::ProcessAsyncLoadTrackTableComplete(void* file)
{
    struct {
        int* data;
        int size;
    } loaded;
    mFilterTableAsyncHandle = 0;
    ASYNCFILE_release_out(file, &loaded.data, &loaded.size);
    mNumAsyncs -= 1;
    if (loaded.size != 0) {
        tSSXHiveScriptFilterTableHeader* header = (tSSXHiveScriptFilterTableHeader*)loaded.data;
        mpFilterTableHeader = header;
        mpCategoryFilterTable = &header->scriptFilterCategoryStart;
        for (int i = 0; i < mpFilterTableHeader->numScriptCategories; i++) {
            if (mpCategoryFilterTable[i].numScripts != 0) {
                // the file stores offsets from its start: relocate them into pointers in place
                mpCategoryFilterTable[i].scriptFilterPtr.scriptFilterOffset =
                    (int)loaded.data + mpCategoryFilterTable[i].scriptFilterPtr.scriptFilterOffset;
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

// A t4Vector built from its four components (t4Vector itself has no constructor; On Tour's has this one).
struct t4VectorXYZW : t4Vector {
    t4VectorXYZW(float x_, float y_, float z_, float w_) { x = x_; y = y_; z = z_; w = w_; }
};

// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_27B750(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Length_27B750(const t2Vector& v)
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
void cSSXScriptEngine::FillOrientationFromStartPointInfo(cRider* obj, t4Vector* pos, int* roll, float* pitch,
                                                         float* yaw)
{
    tPathStartpoint* startPoint = (tPathStartpoint*)cPathSys_getStartpoint(gPaths, 1, obj->getAIIndex());
    *pos = t4VectorXYZW(startPoint->Pos.x, startPoint->Pos.y, startPoint->Pos.z, 1.0f);
    *yaw = Atan2_27B750(startPoint->Dir.y, startPoint->Dir.x);
    float z = startPoint->Dir.z;
    t2Vector horiz;
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


// PORT: PS2 sqrt.s asm helper; use sqrtf on PC.
static inline float Sqrt_27B948(float x)
{
    float r;
    __asm__("sqrt.s %0,%1" : "=f"(r) : "f"(x));
    return r;
}

static inline float Length_27B948(const t2Vector& v)
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
 * @param pitch  receives the pitch of the rider's direction (mMatrix.row[1])
 * @param yaw  receives the heading of the rider's direction
 */
void cSSXScriptEngine::FillOrientationFromRiderOrientationInfo(cRider* obj, t4Vector* pos, int* roll, float* pitch,
                                                               float* yaw)
{
    *pos = *obj->getPosition();
    t4Vector dir = *(&obj->mMatrix.row[1]);
    *yaw = Atan2_27B948(dir.y, dir.x);
    t2Vector horiz(dir.x, dir.y);
    *pitch = Atan2_27B948(dir.z, Length_27B948(horiz));
    *roll = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cSSXScriptEngine_FillOrientationFromObjectOrientationInfo);
#ifdef SKIP_ASM
extern "C" float func_0031C128(float x);
extern "C" tBELocationDB* cBENewRaceInterface_getLocation(void* iface);
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
 * Section kAnchorSection_CurrentLocation means the current location's section. Leaves the outputs alone when the
 * section has no anchor table
 * or the entry is -1. An optional offset is rotated by the object's matrix and added to the position.
 *
 * @param slot  object type, the index into the anchor table
 * @param pos  receives the object's position
 * @param flag  receives 0 (On Tour baseRotX)
 * @param pitch  receives the object's pitch
 * @param yaw  receives the object's heading
 * @param cat  section ID, or kAnchorSection_CurrentLocation (On Tour overrideSectionID)
 * @param off  offset in the object's frame, or 0
 */
extern "C" void cSSXScriptEngine_FillOrientationFromObjectOrientationInfo(cSSXScriptEngine* self, int slot, sVec_27BB08* pos, int* flag, float* pitch, float* yaw, int cat,
                              const sVec_27BB08* off)
{
    if (cat == cSSXScriptEngine::kAnchorSection_CurrentLocation) {
        int k = cBENewRaceInterface_getLocation(cBE_getInterface_Fv(cBE_getBE(), 0))->locationID;
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
void* cSSXScriptEngine::AllocScriptSetupInfo()
{
    tSSXScriptSetupInfoLinked* head = mpSetupInfoUnused;
    mpSetupInfoUnused = (tSSXScriptSetupInfoLinked*)(head->pNext);
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
 * @param vsetupInfo  the record
 */
void cSSXScriptEngine::ReleaseScriptSetupInfo(void* vsetupInfo)
{
    tSSXScriptSetupInfoLinked* setupInfo = (tSSXScriptSetupInfoLinked*)vsetupInfo;
    setupInfo->pNext = (tSSXScriptSetupInfoLinked*)((void*)mpSetupInfoUnused);
    *(int*)setupInfo = -1;
    setupInfo->supportChar0AIIndex = -1;
    setupInfo->supportChar1AIIndex = -1;
    setupInfo->viewIndex = -1;
    mpSetupInfoUnused = (tSSXScriptSetupInfoLinked*)setupInfo;
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
 * Riders come from the game's AI rider set (SSXApp->Game->AI); a rider's bit is
 * kCharacterMappingTable[cRider_getIdentity(rider)]. Slots 0-2 take the riders at @p mainCharAIIndex, @p
 * supportChar0AIIndex and @p supportChar1AIIndex of the list at +0x28 (skipped when negative); slots 3-4 the first two
 * of the list at +0x40 (count +0x7C); 5-9 the list at +0x48 (count +0x80); 10-14 the list at +0x5C (count +0x84); 15-20
 * the list at +0x28 in order (count +0x78). The last three lists hold objects whose rider is at +0x18.
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
    for (i = 0; i < keSSXSubject_Num; i++) {
        flags[i] = 0;
        switch (i) {
        case keSSXSubject_MainChar:
            if (mainCharAIIndex >= 0)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->players[mainCharAIIndex])];
            break;
        case keSSXSubject_SupportChar0:
            if (supportChar0AIIndex >= 0)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->players[supportChar0AIIndex])];
            break;
        case keSSXSubject_SupportChar1:
            if (supportChar1AIIndex >= 0)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->players[supportChar1AIIndex])];
            break;
        case keSSXSubject_Player0:
        case keSSXSubject_Player1:
            if (i - keSSXSubject_Player0 < numA)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->groupA[i - keSSXSubject_Player0]->rider)];
            break;
        case keSSXSubject_Computer0:
        case keSSXSubject_Computer1:
        case keSSXSubject_Computer2:
        case keSSXSubject_Computer3:
        case keSSXSubject_Computer4:
            if (i - keSSXSubject_Computer0 < numB)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->groupB[i - keSSXSubject_Computer0]->rider)];
            break;
        case keSSXSubject_Actor0:
        case keSSXSubject_Actor1:
        case keSSXSubject_Actor2:
        case keSSXSubject_Actor3:
        case keSSXSubject_Actor4:
            if (i - keSSXSubject_Actor0 < set->numC)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->groupC[i - keSSXSubject_Actor0]->rider)];
            break;
        case keSSXSubject_Rider0:
        case keSSXSubject_Rider1:
        case keSSXSubject_Rider2:
        case keSSXSubject_Rider3:
        case keSSXSubject_Rider4:
        case keSSXSubject_Rider5:
            if (i - keSSXSubject_Rider0 < set->numPlayers)
                flags[i] = kCharacterMappingTable[cRider_getIdentity(set->players[i - keSSXSubject_Rider0])];
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
 * @return true while the bank's async load is pending
 */
bool cSSXScriptEngine::IsBankLoading(int i)
{
    UpdateLoad();
    return (int)mScriptDataBanks[i].asyncHandle != 0;
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
void cSSXScriptEngine::ClearNISPlayListState()
{
    if (mPlayingNISList != keNISList_Num) {
        mPlayingNISList = keNISList_Num;
        mNumFramesNoSkip = 0;
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
    cScriptObject_cScriptObject(&self->mObjType, keScriptObjectType_GameState);
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
 * Drops the game-state script object's use count: cScriptObject_OnEndObjectUse on its cScriptObject part (+0xC). The
 * player ID argument is not declared.
 */
void cScriptObjGS::OnEndObjectUse()
{
    cScriptObject_OnEndObjectUse(&mObjType);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjGS_IsMatchingEntity);
#ifdef SKIP_ASM
/**
 * Returns whether the game-state script object takes an InSeq entity: only a game state manager entity (type 6).
 *
 * @param entity  the InSeq player entity (unused)
 * @param type  the entity type
 * @return whether it is a game state manager entity
 */
bool cScriptObjGS::IsMatchingEntity(cScriptPlayerEntity* entity, int type)
{
    return type == keSSXEntityType_GameStateManager;
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
 * Calls GetSSXGameStateManagerFrame(@p entity, frame) with a 16-byte stack buffer and returns; nothing reads the
 * result. On Tour's version (GetSSXGameStateManagerFrame plus asserts on the event bits and frame count) does no more
 * in a release build. Virtual slot 3 of cScriptObjGS_vtbl (the cScriptEntity part); the event type, frame and event
 * bits (On Tour type, currFrame, eventBits) are not used.
 *
 * @param entity  the script player entity the event is for (On Tour cScriptPlayerEntity* pEntity)
 */
void cScriptObjGS::OnInSeqKeyEvent(void* entity)
{
    char buf[16];
    GetSSXGameStateManagerFrame(entity, buf);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXScriptHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the SSX script header of a script player (a cScriptPlayerMain): its mpDataHeader.
 *
 * Callers read the transition frame counts, the bank (+0x1C) and the first byte from it.
 *
 * @param pPlayer  the script player (a cScriptPlayerMain)
 * @return the header
 */
tSSXScriptHeader* GetSSXScriptHeader(void* pPlayer)
{
    cScriptPlayerMain* player = (cScriptPlayerMain*)pPlayer;
    return (tSSXScriptHeader*)player->mpDataHeader;
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
 * @param pScriptData  the raw script data
 * @return the header, or 0 when the data has none
 */
tSSXScriptHeader* GetSSXScriptHeader1(void* pScriptData)
{
    return (tSSXScriptHeader*)cScriptPlayerMain_GetHeader(pScriptData);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManualHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of a camera script entity (a cScriptPlayerEntity): its mpHeader.
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header
 */
tSSXHeaderCameraManual* GetSSXCameraManualHeader(void* pPlayer)
{
    cScriptPlayerEntity* entity = (cScriptPlayerEntity*)pPlayer;
    return (tSSXHeaderCameraManual*)entity->mpHeader;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManualHeader1);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

/**
 * Returns the custom header of variation n of a camera script entity's script: cScriptPlayerEntity_GetHeader, with the
 * variation index passing through in the second argument register (the C declares only the entity).
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header, or 0 when the variation has none
 */
extern "C" void* GetSSXCameraManualHeader1(void* pPlayer)
{
    return cScriptPlayerEntity_GetHeader(pPlayer);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManualHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in camera script data
 * @p pScriptData: finds the entity's block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through
 * in the argument registers) and reads the header with cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param pScriptData  the raw script data
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXCameraManualHeader2(void* pScriptData, void* entityIndex, int variationIndex) {
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
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @param out  receives 10 floats
 */
extern "C" void GetSSXCameraManualFrame(void* pPlayer, float* out)
{
    out[0] = *cScriptPlayerEntity_GetChannel_C0F0(pPlayer, 0)->GetValue();
    out[1] = *cScriptPlayerEntity_GetChannel_C0F0(pPlayer, 1)->GetValue();
    out[2] = *cScriptPlayerEntity_GetChannel_C0F0(pPlayer, 2)->GetValue();
    out[3] = *cScriptPlayerEntity_GetChannel_C0F0(pPlayer, 3)->GetValue();
    out[4] = *cScriptPlayerEntity_GetChannel_C0F0(pPlayer, 4)->GetValue();
    out[5] = *cScriptPlayerEntity_GetChannel_C0F0(pPlayer, 5)->GetValue();
    out[6] = *cScriptPlayerEntity_GetChannel_C0F0(pPlayer, 6)->GetValue();
    out[7] = *cScriptPlayerEntity_GetChannel_C0F0(pPlayer, 7)->GetValue();
    out[8] = *cScriptPlayerEntity_GetChannel_C0F0(pPlayer, 8)->GetValue();
    out[9] = *cScriptPlayerEntity_GetChannel_C0F0(pPlayer, 9)->GetValue();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraTargetHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of a camera script entity (a cScriptPlayerEntity): its mpHeader.
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header
 */
tSSXHeaderCameraTarget* GetSSXCameraTargetHeader(void* pPlayer)
{
    cScriptPlayerEntity* entity = (cScriptPlayerEntity*)pPlayer;
    return (tSSXHeaderCameraTarget*)entity->mpHeader;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraTargetHeader1);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

/**
 * Returns the custom header of variation n of a camera script entity's script: cScriptPlayerEntity_GetHeader, with the
 * variation index passing through in the second argument register (the C declares only the entity).
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header, or 0 when the variation has none
 */
extern "C" void* GetSSXCameraTargetHeader1(void* pPlayer)
{
    return cScriptPlayerEntity_GetHeader(pPlayer);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraTargetHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in camera script data
 * @p pScriptData: finds the entity's block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through
 * in the argument registers) and reads the header with cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param pScriptData  the raw script data
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXCameraTargetHeader2(void* pScriptData, void* entityIndex, int variationIndex) {
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
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @param out  receives 10 floats
 */
extern "C" void GetSSXCameraTargetFrame(void* pPlayer, float* out)
{
    out[0] = *cScriptPlayerEntity_GetChannel_C300(pPlayer, 0)->GetValue();
    out[1] = *cScriptPlayerEntity_GetChannel_C300(pPlayer, 1)->GetValue();
    out[2] = *cScriptPlayerEntity_GetChannel_C300(pPlayer, 2)->GetValue();
    out[3] = *cScriptPlayerEntity_GetChannel_C300(pPlayer, 3)->GetValue();
    out[4] = *cScriptPlayerEntity_GetChannel_C300(pPlayer, 4)->GetValue();
    out[5] = *cScriptPlayerEntity_GetChannel_C300(pPlayer, 5)->GetValue();
    out[6] = *cScriptPlayerEntity_GetChannel_C300(pPlayer, 6)->GetValue();
    out[7] = *cScriptPlayerEntity_GetChannel_C300(pPlayer, 7)->GetValue();
    out[8] = *cScriptPlayerEntity_GetChannel_C300(pPlayer, 8)->GetValue();
    out[9] = *cScriptPlayerEntity_GetChannel_C300(pPlayer, 9)->GetValue();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraSubjectHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of a camera script entity (a cScriptPlayerEntity): its mpHeader.
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header
 */
tSSXHeaderCameraSubject* GetSSXCameraSubjectHeader(void* pPlayer)
{
    cScriptPlayerEntity* entity = (cScriptPlayerEntity*)pPlayer;
    return (tSSXHeaderCameraSubject*)entity->mpHeader;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraSubjectHeader1);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

/**
 * Returns the custom header of variation n of a camera script entity's script: cScriptPlayerEntity_GetHeader, with the
 * variation index passing through in the second argument register (the C declares only the entity).
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header, or 0 when the variation has none
 */
extern "C" void* GetSSXCameraSubjectHeader1(void* pPlayer)
{
    return cScriptPlayerEntity_GetHeader(pPlayer);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraSubjectHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in camera script data
 * @p pScriptData: finds the entity's block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through
 * in the argument registers) and reads the header with cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param pScriptData  the raw script data
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXCameraSubjectHeader2(void* pScriptData, void* entityIndex, int variationIndex) {
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
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @param out  receives 9 floats
 */
extern "C" void GetSSXCameraSubjectFrame(void* pPlayer, float* out)
{
    out[0] = *cScriptPlayerEntity_GetChannel_C510(pPlayer, 0)->GetValue();
    out[1] = *cScriptPlayerEntity_GetChannel_C510(pPlayer, 1)->GetValue();
    out[2] = *cScriptPlayerEntity_GetChannel_C510(pPlayer, 2)->GetValue();
    out[3] = *cScriptPlayerEntity_GetChannel_C510(pPlayer, 3)->GetValue();
    out[4] = *cScriptPlayerEntity_GetChannel_C510(pPlayer, 4)->GetValue();
    out[5] = *cScriptPlayerEntity_GetChannel_C510(pPlayer, 5)->GetValue();
    out[6] = *cScriptPlayerEntity_GetChannel_C510(pPlayer, 6)->GetValue();
    out[7] = *cScriptPlayerEntity_GetChannel_C510(pPlayer, 7)->GetValue();
    out[8] = *cScriptPlayerEntity_GetChannel_C510(pPlayer, 8)->GetValue();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraAlgorithmicHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of a camera script entity (a cScriptPlayerEntity): its mpHeader.
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header
 */
tSSXHeaderCameraAlgorithmic* GetSSXCameraAlgorithmicHeader(void* pPlayer)
{
    cScriptPlayerEntity* entity = (cScriptPlayerEntity*)pPlayer;
    return (tSSXHeaderCameraAlgorithmic*)entity->mpHeader;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraAlgorithmicHeader1);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

/**
 * Returns the custom header of variation n of a camera script entity's script: cScriptPlayerEntity_GetHeader, with the
 * variation index passing through in the second argument register (the C declares only the entity).
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header, or 0 when the variation has none
 */
extern "C" void* GetSSXCameraAlgorithmicHeader1(void* pPlayer)
{
    return cScriptPlayerEntity_GetHeader(pPlayer);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraAlgorithmicHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in camera script data
 * @p pScriptData: finds the entity's block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through
 * in the argument registers) and reads the header with cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param pScriptData  the raw script data
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXCameraAlgorithmicHeader2(void* pScriptData, void* entityIndex, int variationIndex) {
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
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @param out  receives 9 floats
 */
extern "C" void GetSSXCameraAlgorithmicFrame(void* pPlayer, float* out)
{
    out[0] = *cScriptPlayerEntity_GetChannel_C6F8(pPlayer, 0)->GetValue();
    out[1] = *cScriptPlayerEntity_GetChannel_C6F8(pPlayer, 1)->GetValue();
    out[2] = *cScriptPlayerEntity_GetChannel_C6F8(pPlayer, 2)->GetValue();
    out[3] = *cScriptPlayerEntity_GetChannel_C6F8(pPlayer, 3)->GetValue();
    out[4] = *cScriptPlayerEntity_GetChannel_C6F8(pPlayer, 4)->GetValue();
    out[5] = *cScriptPlayerEntity_GetChannel_C6F8(pPlayer, 5)->GetValue();
    out[6] = *cScriptPlayerEntity_GetChannel_C6F8(pPlayer, 6)->GetValue();
    out[7] = *cScriptPlayerEntity_GetChannel_C6F8(pPlayer, 7)->GetValue();
    out[8] = *cScriptPlayerEntity_GetChannel_C6F8(pPlayer, 8)->GetValue();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManagerHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of a camera script entity (a cScriptPlayerEntity): its mpHeader.
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header
 */
tSSXHeaderCameraManager* GetSSXCameraManagerHeader(void* pPlayer)
{
    cScriptPlayerEntity* entity = (cScriptPlayerEntity*)pPlayer;
    return (tSSXHeaderCameraManager*)entity->mpHeader;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManagerHeader1);
#ifdef SKIP_ASM
extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

/**
 * Returns the custom header of variation n of a camera script entity's script: cScriptPlayerEntity_GetHeader, with the
 * variation index passing through in the second argument register (the C declares only the entity).
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header, or 0 when the variation has none
 */
extern "C" void* GetSSXCameraManagerHeader1(void* pPlayer)
{
    return cScriptPlayerEntity_GetHeader(pPlayer);
}
#endif
//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXCameraManagerHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in camera script data
 * @p pScriptData: finds the entity's block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through
 * in the argument registers) and reads the header with cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param pScriptData  the raw script data
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXCameraManagerHeader2(void* pScriptData, void* entityIndex, int variationIndex) {
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
 * Reads this frame's values of a camera-manager script entity from its channel 0: numActiveCamera is the current
 * value (virtual slot 4) and pActiveCameraList the custom word (virtual slot 5) (cScriptPlayerEntity_GetChannel).
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @param frame  the frame to fill (On Tour tSSXFrameCameraManager* pFrame)
 */
extern "C" void GetSSXCameraManagerFrame(void* pPlayer, tSSXFrameCameraManager* frame)
{
    frame->numActiveCamera = *cScriptPlayerEntity_GetChannel(pPlayer, 0)->GetValue();
    frame->pActiveCameraList = (tActiveCamera*)cScriptPlayerEntity_GetChannel(pPlayer, 0)->GetCustom();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXActorHeader__FPv);
#ifdef SKIP_ASM
/**
 * Returns the actor header of an actor script entity (a cScriptPlayerEntity): its mpHeader.
 *
 * Rider and script-object code read the anchor type, base rotations and offset and the role flags from it.
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header
 */
tSSXHeaderActor* GetSSXActorHeader(void* pPlayer)
{
    cScriptPlayerEntity* entity = (cScriptPlayerEntity*)pPlayer;
    return (tSSXHeaderActor*)entity->mpHeader;
}
#endif

extern "C" void* cScriptPlayerEntity_GetHeader(void* self);

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXActorHeader1__FPv);
#ifdef SKIP_ASM
/**
 * Returns the custom header of variation n of an actor script entity's script: cScriptPlayerEntity_GetHeader, with the
 * variation index passing through in the second argument register (the C declares only the entity).
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @return the header, or 0 when the variation has none
 */
void* GetSSXActorHeader1(void* pPlayer)
{
    return cScriptPlayerEntity_GetHeader(pPlayer);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXActorHeader2);
#ifdef SKIP_ASM
extern "C" void cScriptPlayerEntity_GetHeader1(int, int);
extern "C" int cScriptPlayerMain_GetEntityScriptData();

/**
 * Returns the custom header of variation @p variationIndex of entity @p entityIndex in actor script data
 * @p pScriptData: finds the entity's block with cScriptPlayerMain_GetEntityScriptData (the data and index pass through
 * in the argument registers) and reads the header with cScriptPlayerEntity_GetHeader1.
 *
 * The C declares no return value; cScriptPlayerEntity_GetHeader1's result is left in the return register.
 *
 * @param pScriptData  the raw script data
 * @param entityIndex  the entity index
 * @param variationIndex  the variation index
 */
extern "C" void GetSSXActorHeader2(void* pScriptData, void* entityIndex, int variationIndex) {
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
 * For channels 0..2 (cScriptPlayerEntity_GetChannel__FPvi, On Tour cScriptPlayerEntity::GetChannel) it stores the first
 * word of the channel's virtual slot-4 value and the slot-5 result (numMainAnim..pSpeechEventArray); channels 3..8 each
 * give one float through slot 4 (posX..rotZ). cRider_OnStartObjectUse, cRider_OnInSeqUpdate and cRider_OnInSeqKeyEvent
 * read the rider's scripted pose this way.
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @param frame  the actor frame to fill (On Tour tSSXFrameActor* pFrame)
 */
extern "C" void GetSSXActorFrame(void* pPlayer, tSSXFrameActor* frame)
{
    frame->numMainAnim = *cScriptPlayerEntity_GetChannel(pPlayer, 0)->GetValue();
    frame->pMainAnimList = (tMainAnim*)cScriptPlayerEntity_GetChannel(pPlayer, 0)->GetCustom();
    frame->numSound = *cScriptPlayerEntity_GetChannel(pPlayer, 1)->GetValue();
    frame->pSoundList = (tSound*)cScriptPlayerEntity_GetChannel(pPlayer, 1)->GetCustom();
    frame->numSpeechEvent = *cScriptPlayerEntity_GetChannel(pPlayer, 2)->GetValue();
    frame->pSpeechEventArray = (tSpeechEvent*)cScriptPlayerEntity_GetChannel(pPlayer, 2)->GetCustom();
    frame->posX = *cScriptPlayerEntity_GetChannel_C9B0(pPlayer, 3)->GetValue();
    frame->posY = *cScriptPlayerEntity_GetChannel_C9B0(pPlayer, 4)->GetValue();
    frame->posZ = *cScriptPlayerEntity_GetChannel_C9B0(pPlayer, 5)->GetValue();
    frame->rotX = *cScriptPlayerEntity_GetChannel_C9B0(pPlayer, 6)->GetValue();
    frame->rotY = *cScriptPlayerEntity_GetChannel_C9B0(pPlayer, 7)->GetValue();
    frame->rotZ = *cScriptPlayerEntity_GetChannel_C9B0(pPlayer, 8)->GetValue();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", GetSSXGameStateManagerFrame);
#ifdef SKIP_ASM
cScriptObj002742C8* cScriptPlayerEntity_GetChannel(void* self, int a1);

/**
 * Reads this frame's values of a game-state script entity from its channel 0: numGameState is the current value
 * (virtual slot 4) and pGameStateList the custom word (virtual slot 5) (cScriptPlayerEntity_GetChannel).
 *
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @param out  the tSSXFrameGameStateManager to fill (On Tour pFrame)
 */
extern "C" void GetSSXGameStateManagerFrame(void* pPlayer, void* out)
{
    tSSXFrameGameStateManager* frame = (tSSXFrameGameStateManager*)out;
    frame->numGameState = *cScriptPlayerEntity_GetChannel(pPlayer, 0)->GetValue();
    frame->pGameStateList = (tGameState*)cScriptPlayerEntity_GetChannel(pPlayer, 0)->GetCustom();
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
 * @param pPlayer  the script entity player (a cScriptPlayerEntity)
 * @param out  receives ten words
 */
extern "C" void GetSSXGameEventFrame(void* pPlayer, tSSXFrameGameEvent* out)
{
    out->numWorldScriptEvent = *cScriptPlayerEntity_GetChannel(pPlayer, 0)->GetValue();
    out->pWorldScriptEventArray = (tWorldScriptEvent*)cScriptPlayerEntity_GetChannel(pPlayer, 0)->GetCustom();
    out->numMusicEvent = *cScriptPlayerEntity_GetChannel(pPlayer, 1)->GetValue();
    out->pMusicEventArray = (tMusicEvent*)cScriptPlayerEntity_GetChannel(pPlayer, 1)->GetCustom();
    out->numSpeechEvent = *cScriptPlayerEntity_GetChannel(pPlayer, 2)->GetValue();
    out->pSpeechEventArray = (tSpeechEvent*)cScriptPlayerEntity_GetChannel(pPlayer, 2)->GetCustom();
    out->numSound = *cScriptPlayerEntity_GetChannel(pPlayer, 3)->GetValue();
    out->pSoundList = (tSound*)cScriptPlayerEntity_GetChannel(pPlayer, 3)->GetCustom();
    out->numEnviroState = *cScriptPlayerEntity_GetChannel(pPlayer, 4)->GetValue();
    out->pEnviroStateList = (tEnviroState*)cScriptPlayerEntity_GetChannel(pPlayer, 4)->GetCustom();
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
 * (cScriptDatabaseObject_mpDatabaseObject), clears the 20 object slots (mpObjectDatabase) and sets the 30 entity
 * records (mEntityDatabase, 0x14 each) to no entity, object -1, player -1, relation 0, reserved player -1.
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
 * Restores the cScriptDatabaseObject vtable (cScriptDatabaseObject_vtbl) at +0x2A8, calls
 * cScriptDatabaseObject_RemoveObjectAll() and zeroes cScriptDatabaseObject_mpDatabaseObject (On Tour
 * cScriptDatabaseObject::mpDatabaseObject, the object cScriptDatabaseObject_GetDatabaseObject returns), then frees the
 * object when asked. cBXScriptEngine__cBXScriptEngine chains to it.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cScriptDatabaseObject__cScriptDatabaseObject(cScriptDatabaseObject* self, int flags)
{
    self->mVtbl = (void**)cScriptDatabaseObject_vtbl;
    self->RemoveObjectAll();
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
 * Stores @p scriptObject in the first free of the 20 mpObjectDatabase slots, then registers each of its script entities
 * (its virtual GetScriptEntity, slot 7, for 0 to its slot 6 count) with cScriptDatabaseObject_AddEntity under that ID.
 *
 * Likely bug: with all 20 slots taken the object is not stored, yet its entities are registered under ID 20 and 20 is
 * returned.
 *
 * @param scriptObject  the script object
 * @return the object ID (slot index)
 */
int cScriptDatabaseObject::AddObject(void* scriptObject)
{
    sScrOwnersCEA8* self = (sScrOwnersCEA8*)this;
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
        ((cScriptDatabaseObject*)self)->AddEntity(i, obj->GetScriptEntity(k));
    }
    return i;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_RemoveObject);
#ifdef SKIP_ASM
extern "C" int cScriptDatabaseObject_RemoveEntity(cScriptDatabaseObject* self, void* msg);

// cScriptObject's virtual GetNumScriptEntities (slot 6; a pure virtual in cScriptObject).
static inline int scriptObjNumEntities(cScriptObject* obj)
{
    sVEntry<int(void*)>* vt = (sVEntry<int(void*)>*)obj->mVtbl;
    return vt[6].fn(vthis(obj, vt[6]));
}

// cScriptObject's virtual GetScriptEntity (slot 7; a pure virtual in cScriptObject).
static inline cScriptEntity* scriptObjEntity(cScriptObject* obj, int i)
{
    sVEntry<cScriptEntity*(void*, int)>* vt = (sVEntry<cScriptEntity*(void*, int)>*)obj->mVtbl;
    return vt[7].fn(vthis(obj, vt[7]), i);
}

/**
 * Removes the script object with ID @p idx from the database, unregistering its script entities.
 *
 * Each of the object's entities (its virtual GetScriptEntity, slot 7, for 0 to its slot 6 count) is removed with
 * cScriptDatabaseObject_RemoveEntity, then the mpObjectDatabase slot is cleared.
 *
 * @param idx  the object ID
 * @return true, or false when the slot was empty
 */
bool cScriptDatabaseObject::RemoveObject(int idx)
{
    cScriptObject* obj = mpObjectDatabase[idx];
    if (obj == 0) return false;
    int k = 0;
    int n = scriptObjNumEntities(obj);
    for (; k < n; k++) {
        RemoveEntity(scriptObjEntity(obj, k));
    }
    mpObjectDatabase[idx] = 0;
    return true;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_RemoveObjectAll);
#ifdef SKIP_ASM

/**
 * Removes every object registered in the script object database.
 *
 * For each of the 20 mpObjectDatabase slots that holds an object it calls virtual slot 3 (vtable at +0x2A8) with
 * the slot index. cScriptDatabaseObject's own slot 3 (cScriptDatabaseObject_RemoveObject) unregisters the object's
 * script entities and clears the slot; cSSXScriptEngine overrides it with cSSXScriptEngine_RemoveObject.
 */
void cScriptDatabaseObject::RemoveObjectAll()
{
    int i;
    for (i = 0; i < 20; i++) {
        if (mpObjectDatabase[i] != 0) {
            sVEntry<int(void*, int)>* vt = (sVEntry<int(void*, int)>*)mVtbl;
            vt[3].fn(vthis(this, vt[3]), i);
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
 * Reserves every script entity of object @p idx for player @p val (cScriptDatabaseObject_MarkEntityAsReserved on each
 * of the object's virtual GetScriptEntity results).
 *
 * Quirk: an empty object slot is dereferenced unchecked.
 *
 * @param idx  the object ID
 * @param val  the player ID
 */
void cScriptDatabaseObject::MarkObjectAsReserved(int idx, int val)
{
    cScriptOwnerD0C0* obj = (cScriptOwnerD0C0*)mpObjectDatabase[idx];
    int i = 0;
    int n = obj->GetNumScriptEntities();
    for (; i < n; i++) {
        MarkEntityAsReserved(obj->GetScriptEntity(i), val);
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_MarkObjectAsUnreserved);
#ifdef SKIP_ASM
extern "C" void cScriptDatabaseObject_MarkEntityAsUnreserved(cScriptDatabaseObject* self, void* a1);

/**
 * Clears the reservation of every script entity of object @p idx (cScriptDatabaseObject_MarkEntityAsUnreserved on each
 * of the object's virtual GetScriptEntity results).
 *
 * @param idx  the object ID
 */
void cScriptDatabaseObject::MarkObjectAsUnreserved(int idx)
{
    cScriptObject* obj = mpObjectDatabase[idx];
    int i = 0;
    int n = scriptObjNumEntities(obj);
    for (; i < n; i++) {
        MarkEntityAsUnreserved(scriptObjEntity(obj, i));
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_IsObjInUseByPlayer);
#ifdef SKIP_ASM
extern "C" int cScriptDatabaseObject_IsEntityInUseByPlayer(cScriptDatabaseObject* self, int i, int val);

// PORT: cSSXScriptEngine_IsObjInUseByPlayer declares cScriptDatabaseObject_IsObjInUseByPlayer as void; it really
// returns a bool.
bool cScriptDatabaseObject_IsObjInUseByPlayer_impl(void* self, int idx, int val)
    __asm__("cScriptDatabaseObject_IsObjInUseByPlayer");

/**
 * Returns whether any script entity of object @p idx is in use by player @p val
 * (cScriptDatabaseObject_IsEntityInUseByPlayer on each entity's database ID, mDBID).
 *
 * @param idx  the object ID
 * @param val  the player ID
 * @return true when one is
 */
bool cScriptDatabaseObject_IsObjInUseByPlayer_impl(void* vself, int idx, int val)
{
    cScriptDatabaseObject* self = (cScriptDatabaseObject*)vself;
    cScriptObject* obj = self->mpObjectDatabase[idx];
    int i = 0;
    int n = scriptObjNumEntities(obj);
    for (; i < n; i++) {
        if (self->IsEntityInUseByPlayer(scriptObjEntity(obj, i)->mDBID,
                                                        val)) {
            return true;
        }
    }
    return false;
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
 * @return whether it is
 */
bool cScriptDatabaseObject::IsEntityInUseByPlayer(int i, int val)
{
    return mEntityDatabase[i].playerID == val;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_GetDatabaseObject);
#ifdef SKIP_ASM
extern cScriptDatabaseObject* cScriptDatabaseObject_mpDatabaseObject;

/**
 * Returns the current script database object, cScriptDatabaseObject_mpDatabaseObject (set by its constructor
 * cScriptDatabaseObject_cScriptDatabaseObject).
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
int cScriptDatabaseObject::GetPlayerIDByEntity(cScriptEntity* entity)
{
    tScriptEntityRecord* record = &((&mEntityDatabase[0]))[entity->mDBID];
    if (record->relation != keScriptEntityRelation_InUse) {
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
int cScriptDatabaseObject::GetObject(int i)
{
    return (int)mpObjectDatabase[i];
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
int cScriptDatabaseObject::AddEntity(int idx, void* item)
{
    int i;
    for (i = 0; i < 30; i++) {
        int off = i * 20 + 80;
        if (*(void**)((char*)this + off) == 0) {
            *(int*)((char*)this + off + 4) = idx;
            *(void**)((char*)this + off) = item;
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
 * An entity marked used (relation 1) is first cut off from its InSeq entity (cScriptEntity_DetachEntity). The entity's
 * database ID is set to -1 and the record cleared (no entity, object -1, player -1, relation 0, reserved -1).
 *
 * @param msg  the script entity
 * @return always 1
 */
int cScriptDatabaseObject::RemoveEntity(void* msg)
{
    cScriptEntity* entity = (cScriptEntity*)msg;
    tScriptEntityRecord* record = &mEntityDatabase[entity->mDBID];
    if (record->relation == keScriptEntityRelation_InUse) {
        cScriptEntity_DetachEntity(msg);
    }
    record->pEntity->mDBID = -1;
    record->objID = -1;
    record->pEntity = 0;
    record->playerID = -1;
    record->relation = keScriptEntityRelation_None;
    record->reservedPlayerID = -1;
    return 1;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_MarkEntityAsUsed);
#ifdef SKIP_ASM



/**
 * Marks a script entity as used by player @p value.
 *
 * When the entity's object is not yet in use by that player (the database's virtual slot 4, IsObjInUseByPlayer-style,
 * cScriptDatabaseObject_IsObjInUseByPlayer in the base), the object's own virtual slot 4 (OnStartObjectUse) is called
 * with the player. Then the record (mEntityDatabase[@p key's mDBID]) takes the player ID (playerID) and relation 1.
 *
 * @param key  the script entity
 * @param value  the player ID
 */
void cScriptDatabaseObject::MarkEntityAsUsed(cScriptEntity* key, int value)
{
    sVEntry<int(void*, int, int)>* vt = (sVEntry<int(void*, int, int)>*)mVtbl;
    tScriptEntityRecord* record = &mEntityDatabase[key->mDBID];
    if (vt[4].fn(vthis(this, vt[4]), record->objID, value) == 0) {
        cScriptObject* obj = mpObjectDatabase[record->objID];
        sVEntry<void(void*, int)>* vt2 = (sVEntry<void(void*, int)>*)obj->mVtbl;
        vt2[4].fn(vthis(obj, vt2[4]), value);
    }
    record->playerID = value;
    record->relation = keScriptEntityRelation_InUse;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptDatabaseObject_MarkEntityAsFree);
#ifdef SKIP_ASM
/**
 * Marks a script entity as free again.
 *
 * Clears its record's player ID (-1) and relation (0); when the object is no longer in use by the old player (the
 * database's virtual slot 4, cScriptDatabaseObject_IsObjInUseByPlayer in the base), the object's virtual slot 5
 * (OnEndObjectUse) is called with that player.
 *
 * @param key  the script entity
 */
void cScriptDatabaseObject::MarkEntityAsFree(cScriptEntity* key)
{
    tScriptEntityRecord* record = &mEntityDatabase[key->mDBID];
    int old = record->playerID;
    record->playerID = -1;
    record->relation = keScriptEntityRelation_None;
    sVEntry<int(void*, int, int)>* vt = (sVEntry<int(void*, int, int)>*)mVtbl;
    if (vt[4].fn(vthis(this, vt[4]), record->objID, old) == 0) {
        cScriptObject* obj = mpObjectDatabase[record->objID];
        sVEntry<void(void*, int)>* vt2 = (sVEntry<void(void*, int)>*)obj->mVtbl;
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
void cScriptDatabaseObject::MarkEntityAsReserved(void* entity, int playerId)
{
    int i = ((cScriptEntity*)entity)->mDBID;
    mEntityDatabase[i].reservedPlayerID = playerId;
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
void cScriptDatabaseObject::MarkEntityAsUnreserved(void* entity)
{
    int i = ((cScriptEntity*)entity)->mDBID;
    mEntityDatabase[i].reservedPlayerID = -1;
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
int cScriptDatabaseObject::GetAvailableEntities(int key, int* out, int max)
{
    int n = 0;
    int i = 0;
    tScriptEntityRecord* record = ((&mEntityDatabase[0]));
    while (i < 30 && n != max) {
        if (((int)record->pEntity) != 0 && record->relation != keScriptEntityRelation_InUse
            && (record->reservedPlayerID < 0 || record->reservedPlayerID == key)) {
            *out++ = ((int)record->pEntity);
            n++;
        }
        i++;
        if (i < 30) record = &((&mEntityDatabase[0]))[i];
    }
    return n;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_cScriptObjCamera);
#ifdef SKIP_ASM
void* cScriptObject_cScriptObject(void*, int);
extern char cScriptObjCamera_vtbl[];
extern t4Matrix t4mat_Identity;

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
    cScriptObject_cScriptObject(self, keScriptObjectType_Camera);
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
        : "r"((char*)&self->mBaseTransform), "r"(&t4mat_Identity)
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
void cScriptObjCamera::SetCameraManager(int val)
{
    mpCameraManager = (cScriptObjCameraManager*)val;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_SetCameraEntity);
#ifdef SKIP_ASM
extern t4Matrix t4mat_Identity;


/**
 * Binds a script camera object to camera entity @p anim, or unbinds it when @p anim is 0.
 *
 * Binding: the player using the entity (cScriptDatabaseObject_GetPlayerIDByEntity on the database) becomes the object's
 * player (+0x20); an entity already bound (+0x14) first ends its use with the object's virtual slot 5 (OnEndObjectUse)
 * for the old player, and the new use starts with slot 4 (OnStartObjectUse). Unbinding a bound object resets the base
 * transform (+0x30) to t4mat_Identity and the base rotations (+0x70-+0x78) to 0, ends the use (slot 5), and clears the
 * entity and player (-1).
 *
 * @param anim  the camera entity (cScriptEntityCamera), or 0
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy of t4mat_Identity, likely identity);
// the PC port needs a plain 64-byte copy.
void cScriptObjCamera::SetCameraEntity(void* anim)
{
    if (anim != 0) {
        int id = cScriptDatabaseObject_GetDatabaseObject()->GetPlayerIDByEntity((cScriptEntity*)anim);
        if (mpCameraEntity != 0) {
            sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)mVtbl;
            vt[5].fn(vthis(this, vt[5]), mPlayerID);
        }
        mpCameraEntity = (cScriptEntityCamera*)anim;
        mPlayerID = id;
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)mVtbl;
        vt[4].fn(vthis(this, vt[4]), id);
    } else if (mpCameraEntity != 0) {
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
            : "r"((char*)&mBaseTransform), "r"(&t4mat_Identity)
            : "memory");
        *(int*)&mBaseRotX = 0;
        *(int*)&mBaseRotY = 0;
        *(int*)&mBaseRotZ = 0;
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)mVtbl;
        vt[5].fn(vthis(this, vt[5]), mPlayerID);
        mpCameraEntity = 0;
        mPlayerID = -1;
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
void cScriptObjCamera::SetCameraWeight(float val)
{
    mWeight = val;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_IsInUseByPlayer);
#ifdef SKIP_ASM
extern "C" cScriptDatabaseObject* cScriptDatabaseObject_GetDatabaseObject(void);
extern "C" int cScriptDatabaseObject_IsEntityInUseByPlayer(cScriptDatabaseObject* self, int i, int val);

/**
 * Returns whether the camera entity bound to this object (+0x14) is in use by player @p val
 * (cScriptDatabaseObject_IsEntityInUseByPlayer on the database with the entity's database ID).
 *
 * @param val  the player ID
 * @return 1 when it is, 0 otherwise or when no entity is bound
 */
int cScriptObjCamera::IsInUseByPlayer(int val)
{
    if (mpCameraEntity == 0) {
        return 0;
    }
    return cScriptDatabaseObject_GetDatabaseObject()->IsEntityInUseByPlayer(mpCameraEntity->mDBID, val);
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
void cScriptObjCamera::PauseObjScript()
{
    ((cSsxScriptVirt*)mpCameraManager)->PauseObjScript();
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCamera_ResumeObjScript);
#ifdef SKIP_ASM
/**
 * Resumes the camera object's script: calls the third virtual of the object at +0x10 (On Tour mpCameraManager, a
 * cScriptEntity; presumably ResumeEntityScript).
 */
void cScriptObjCamera::ResumeObjScript()
{
    ((cSsxScriptVirt*)mpCameraManager)->ResumeObjScript();
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
 * First counts the use with the base handler cScriptObject_OnStartObjectUse (use count mInUse). Only while a game
 * exists: sets mWeight to 1.0, builds the camera's matrix and angles from its script record
 * (cScriptObjCamera_CalculateBaseTransform), adds this object to the ScriptController of the view manager's camera
 * mViewIndex, and makes controller 3 current on that camera when the controller is active (IsActive).
 */
void cScriptObjCamera::OnStartObjectUse()
{
    cScriptObject_OnStartObjectUse(this);
    if ((void*)SSXApp->Game != 0) {
        mWeight = 1.0f;
        cScriptObjCamera_CalculateBaseTransform(this, (int)mpCameraEntity);
        cCamera* camera = SSXApp->Game->ViewMan->PlayerCamera[mViewIndex];
        cScriptCameraController_addCamera(camera->ScriptController, this);
        if (camera->ScriptController->IsActive != 0) {
            cCamera_setCurrentController(camera, 3);
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

/**
 * Releases a script camera when its sequence stops using it, returning the view's camera to the chase camera when
 * no script camera is left.
 *
 * Drops the use count (cScriptObject_OnEndObjectUse). During a game it clears mWeight, takes the camera of view
 * mViewIndex from the game's ViewMan, removes this object from the camera's ScriptController
 * (cScriptCameraController_removeCamera) and, when that controller is no longer active (IsActive 0), switches the
 * camera to controller 2 (chase), restarts the chase camera and calls the current Controller's slot 4 (snapCurrent).
 * On Tour passes a playerID; this C takes none.
 */
void cScriptObjCamera::OnEndObjectUse()
{
    cScriptObject_OnEndObjectUse(this);
    cSSXApp* app = SSXApp;
    if (app->Game != 0) {
        *(int*)&mWeight = 0;
        cCamera* camera = app->Game->ViewMan->PlayerCamera[mViewIndex];
        cScriptCameraController_removeCamera(camera->ScriptController, this);
        if (camera->ScriptController->IsActive == 0) {
            cCamera_setCurrentController(camera, 2);
            cChaseCameraController_switchCamera_Restart(camera->ChaseController);
            cCameraController* controller = camera->Controller;
            sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)controller->mVtbl;
            vt[4].fn(vthis(controller, vt[4]));  // snapCurrent
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
        sRotMat_SE rot; \
        float sinA; \
        float cosA; \
        fSinCos(&sinA, &cosA, (ANGLE)); \
        float oneMinusCos = 1.0f - cosA; \
        float x = axis[0]; \
        float y = axis[1]; \
        float z = axis[2]; \
        float tx = oneMinusCos * x; \
        float ty = oneMinusCos * y; \
        float tz = oneMinusCos * z; \
        float sx = sinA * x; \
        float sy = sinA * y; \
        float sz = sinA * z; \
        rot.m[0][0] = tx * x + cosA; \
        rot.m[1][0] = tx * y + sz; \
        rot.m[2][0] = tx * z - sy; \
        rot.m[0][1] = ty * x - sz; \
        rot.m[1][1] = ty * y + cosA; \
        rot.m[2][1] = ty * z + sx; \
        rot.m[0][2] = tz * x + sy; \
        rot.m[1][2] = tz * y - sx; \
        rot.m[2][2] = tz * z + cosA; \
        rot.m[3][0] = 0.0f; \
        rot.m[3][1] = 0.0f; \
        rot.m[3][2] = 0.0f; \
        rot.m[0][3] = 0.0f; \
        rot.m[1][3] = 0.0f; \
        rot.m[2][3] = 0.0f; \
        rot.m[3][3] = 1.0f; \
        MulTop_SE((TOP), &rot); \
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
extern t4Matrix t4mat_Identity;
extern float t4vect_XAxis_D970[] __asm__("t4vect_XAxis");
extern float t4vect_YAxis_D970[] __asm__("t4vect_YAxis");
extern float t4vect_ZAxis_D970[] __asm__("t4vect_ZAxis");
extern "C" void cSSXScriptEngine_GetAnchorOrientation_f(void* self, int type, void* a2, sVec4_SE* out, float* roll, float* pitch, float* yaw) __asm__("cSSXScriptEngine_GetAnchorOrientation");

// One camera case: offset + euler angles from the record, then the player's frame on top.
#define CAMARM_SE(SELF, OWNER, REC) \
    { \
        sVec4_SE baseTranslation; \
        sRotMat_SE localBaseTransform; \
        (SELF)->rx = *(float*)((REC) + 0x1C) * 0.01745329424738884f; \
        (SELF)->ry = *(float*)((REC) + 0x18) * 0.01745329424738884f; \
        (SELF)->rz = *(float*)((REC) + 0x14) * 0.01745329424738884f; \
        VSet_SE(&baseTranslation, *(float*)((REC) + 8), *(float*)((REC) + 0xC), *(float*)((REC) + 0x10), 1.0f); \
        sRotMat_SE* pLocalBase = &localBaseTransform; \
        CopyMat_SE(pLocalBase, &t4mat_Identity); \
        Translate_SE(pLocalBase, &baseTranslation); \
        ROTATE_SE(pLocalBase, t4vect_ZAxis_D970, -(SELF)->rz); \
        ROTATE_SE(pLocalBase, t4vect_YAxis_D970, (SELF)->ry); \
        ROTATE_SE(pLocalBase, t4vect_XAxis_D970, (SELF)->rx); \
        sVec4_SE anchorTranslation; \
        float roll; \
        float pitch; \
        float yaw; \
        cSSXScriptEngine_GetAnchorOrientation_f(ScriptEngine, *(char*)((REC) + 0x20), *(void**)((OWNER) + 8), &anchorTranslation, &roll, &pitch, &yaw); \
        Translate_SE(&(SELF)->mat, &anchorTranslation); \
        ROTATE_SE(&(SELF)->mat, t4vect_ZAxis_D970, -yaw); \
        ROTATE_SE(&(SELF)->mat, t4vect_YAxis_D970, pitch); \
        ROTATE_SE(&(SELF)->mat, t4vect_XAxis_D970, roll); \
        MulMat_SE(&(SELF)->mat, pLocalBase); \
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
 * Starts from t4mat_Identity and zero angles, then by the type (eSSXEntityType) of the entity's InSeq entity (+0x8,
 * cScriptPlayerEntity_GetType): - keSSXEntityType_CameraManual, CameraTarget and CameraSubject (header from
 * GetSSXCameraManualHeader, GetSSXCameraTargetHeader or GetSSXCameraSubjectHeader): the header's X, Y and Z angles in
 * degrees (+0x1C, +0x18, +0x14) and offset (+0x8-+0x10) form a local matrix (translate, then about Z by -z, Y, X). The
 * anchor of type header byte +0x20 (cSSXScriptEngine_GetAnchorOrientation for the entity's owner player) gives a
 * position and angles; the transform becomes the anchor's translate and rotations (Z by -angle, Y, X) times the local
 * matrix, and the anchor angles are added. - keSSXEntityType_CameraAlgorithmic (header from
 * GetSSXCameraAlgorithmicHeader): only the header's angles (baseRotX/Y/Z) as rotations about Z (negated), Y, X. Other
 * types keep the identity.
 *
 * @param cam  the camera entity (cScriptEntityCamera)
 */
extern "C" void cScriptObjCamera_CalculateBaseTransform_SE(sScrCam_SE* self, sScrCam_SE* cam)
{
    char* owner = cam->owner;
    CopyMat_SE(&self->mat, &t4mat_Identity);
    self->rx = 0.0f;
    self->ry = 0.0f;
    self->rz = 0.0f;
    switch (cScriptPlayerEntity_GetType(owner)) {
    case keSSXEntityType_CameraManual: {
        char* rec = GetSSXCameraManualHeader_D970(owner);
        CAMARM_SE(self, owner, rec);
        break;
    }
    case keSSXEntityType_CameraTarget: {
        char* rec = GetSSXCameraTargetHeader_D970(owner);
        CAMARM_SE(self, owner, rec);
        break;
    }
    case keSSXEntityType_CameraSubject: {
        char* rec = GetSSXCameraSubjectHeader_D970(owner);
        CAMARM_SE(self, owner, rec);
        break;
    }
    case keSSXEntityType_CameraAlgorithmic: {
        sRotMat_SE* mat = &self->mat;
        tSSXHeaderCameraAlgorithmic* rec = (tSSXHeaderCameraAlgorithmic*)GetSSXCameraAlgorithmicHeader_D970(owner);
        self->rx = rec->baseRotX * 0.01745329424738884f;
        self->ry = rec->baseRotY * 0.01745329424738884f;
        self->rz = rec->baseRotZ * 0.01745329424738884f;
        ROTATE_SE(mat, t4vect_ZAxis_D970, -self->rz);
        ROTATE_SE(mat, t4vect_YAxis_D970, self->ry);
        ROTATE_SE(mat, t4vect_XAxis_D970, self->rx);
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
extern t4Matrix t4mat_Identity;

/**
 * Constructs a rider's script object: a script entity plus a cScriptObject of type 3 with an identity base transform.
 *
 * Builds the cScriptEntity part (cScriptEntity_cScriptEntity) at +0x0 and the cScriptObject part at +0xC (mObjType)
 * with type 3, installs the vtables cScriptObjRider_vtbl (mVtbl) and D_00482018 (mScriptObjectVtbl), clears
 * mpAnimBankInfo and mpSoundBankInfo, copies t4mat_Identity into mBaseTransform and zeroes mBaseRotX/Y/Z.
 * cRider_cRider builds the rider's embedded one with it.
 *
 * @return the object (self)
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy of t4mat_Identity, likely identity);
// the PC port needs a plain 64-byte copy.
extern "C" void* cScriptObjRider_cScriptObjRider(cScriptObjRider* self)
{
    cScriptEntity_cScriptEntity(self);
    cScriptObject_cScriptObject(&self->mObjType, keScriptObjectType_Rider);
    self->mScriptObjectVtbl = (void**)D_00482018;
    self->mVtbl = (void**)cScriptObjRider_vtbl;
    self->mpAnimBankInfo = 0;
    self->mpSoundBankInfo = 0;
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
        : "r"(&self->mBaseTransform), "r"(&t4mat_Identity)
        : "memory");
    self->mBaseRotX = 0.0f;
    self->mBaseRotY = 0.0f;
    self->mBaseRotZ = 0.0f;
    return self;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_OnStartObjectUse);
#ifdef SKIP_ASM
void cScriptObject_OnStartObjectUse(void* self);
tSSXHeaderActor* GetSSXActorHeader(void* pPlayer);
tSSXScriptHeader* GetSSXScriptHeader(void* pPlayer);
// PORT: the real functions take (engine, id); the unit declares them with one parameter.
void* cSSXScriptEngine_GetAnimationBankInfo_2(void* engine, int id) __asm__("cSSXScriptEngine_GetAnimationBankInfo__FPv");
void* cSSXScriptEngine_GetSoundBankInfo_2(void* engine, int id) __asm__("cSSXScriptEngine_GetSoundBankInfo__FPv");
extern "C" void cSSXScriptEngine_GetAnchorOrientation(void* engine, int id, void* obj, void* pos, float* a, float* b, float* c);
extern "C" void fSinCos(float* s, float* c, float angle);
extern t4Matrix t4mat_Identity;
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
        : "r"(dst), "r"(&t4mat_Identity)
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
        float sinA; \
        float cosA; \
        fSinCos(&sinA, &cosA, angle); \
        sMat_F9F8 rot; \
        MakeRot_F9F8(&rot, axis, sinA, cosA); \
        rot.m[3][3] = 1.0f; \
        MulIn_F9F8(mat, &rot); \
    }

// Each rotation matrix is a block-scoped temporary of the caller.
#define ROTATE_F9F8(mat, axis, angle) \
    { \
        float sinA; \
        float cosA; \
        fSinCos(&sinA, &cosA, angle); \
        sMat_F9F8 rot; \
        MakeRot_F9F8(&rot, axis, sinA, cosA); \
        rot.m[3][3] = 1.0f; \
        MulMat_F9F8(mat, mat, &rot); \
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
 * Calls cScriptObject_OnStartObjectUse__FPv on the cScriptObject part (+0xC). The actor header (GetSSXActorHeader of
 * the bound player entity, mpEntity) gives the base rotations in degrees (baseRotX/Y/Z, stored as radians in
 * mBaseRotX/Y/Z) and an offset (basePosX..Z) that form a local matrix (translate, then Rz(-z), Ry, Rx).
 * cSSXScriptEngine_GetAnchorOrientation (On Tour cSSXScriptEngine::GetAnchorOrientation) gives the anchor position and
 * angles for the header's anchor type (anchorPos); the base transform (mBaseTransform) becomes the anchor's translate
 * and rotations times the local matrix, and the anchor angles are added to the base rotations. Finally it binds
 * mpAnimBankInfo and mpSoundBankInfo to the script's bank (+0x1C of the GetSSXScriptHeader header) with
 * cSSXScriptEngine_GetAnimationBankInfo__FPv and cSSXScriptEngine_GetSoundBankInfo__FPv. The player ID argument is not
 * declared (unused).
 */
void cScriptObjRider::OnStartObjectUse()
{
    cScriptObject_OnStartObjectUse(&mObjType);
    cScriptPlayerEntity* obj = mpEntity;
    t4Matrix* mat = &mBaseTransform;
    LoadIdentity_F9F8(mat);
    mBaseRotX = 0.0f;
    mBaseRotY = 0.0f;
    mBaseRotZ = 0.0f;
    tSSXHeaderActor* info = (tSSXHeaderActor*)GetSSXActorHeader(obj);
    mBaseRotX = info->baseRotX * 0.01745329424738884f;
    mBaseRotY = info->baseRotY * 0.01745329424738884f;
    mBaseRotZ = info->baseRotZ * 0.01745329424738884f;
    sVec_F9F8 baseOffset(info->basePosX, info->basePosY, info->basePosZ, 1.0f);
    sMat_F9F8 localBaseTransform;
    LoadIdentity_F9F8(&localBaseTransform);
    Translate_F9F8(&localBaseTransform, baseOffset);
    ROTATE_F9F8(&localBaseTransform, t4vect_ZAxis, -mBaseRotZ);
    ROTATE_F9F8(&localBaseTransform, t4vect_YAxis, mBaseRotY);
    ROTATE_F9F8(&localBaseTransform, t4vect_XAxis, mBaseRotX);
    sVec_F9F8 pos;
    float rx, ry, rz;
    cSSXScriptEngine_GetAnchorOrientation(ScriptEngine, info->anchorPos, obj->mpOwner, &pos, &rx, &ry, &rz);
    Translate_F9F8(mat, pos);
    ROTATE2_F9F8(mat, t4vect_ZAxis, -rz);
    ROTATE2_F9F8(mat, t4vect_YAxis, ry);
    ROTATE2_F9F8(mat, t4vect_XAxis, rx);
    MulMat_F9F8(mat, mat, &localBaseTransform);
    mBaseRotX += rx;
    mBaseRotY += ry;
    mBaseRotZ += rz;
    tSSXScriptHeader* info2 = (tSSXScriptHeader*)GetSSXScriptHeader(obj->mpOwner);
    mpAnimBankInfo = (tScriptAnimBankInfo*)cSSXScriptEngine_GetAnimationBankInfo_2(ScriptEngine, info2->bankNum);
    mpSoundBankInfo = (tScriptSoundBankInfo*)cSSXScriptEngine_GetSoundBankInfo_2(ScriptEngine, info2->bankNum);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_OnEndObjectUse);
#ifdef SKIP_ASM
void cScriptObject_OnEndObjectUse(void*);
extern t4Matrix t4mat_Identity;

/**
 * Releases the rider's script object: drops one use and resets its base transform and bank bindings.
 *
 * Calls cScriptObject_OnEndObjectUse (On Tour cScriptObject::OnEndObjectUse) on the cScriptObject part (+0xC), copies
 * t4mat_Identity into the base transform (+0x30), zeroes the base rotations (+0x70..+0x78) and clears mpAnimBankInfo
 * (+0x1C) and mpSoundBankInfo (+0x20). The player ID argument is not declared (unused).
 */
// PORT: PS2-only VU0 inline asm (lqc2/sqc2 matrix copy of t4mat_Identity, likely identity);
// the PC port needs a plain 64-byte copy.
void cScriptObjRider::OnEndObjectUse()
{
    cScriptObject_OnEndObjectUse(&mObjType);
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
        : "r"((char*)&mBaseTransform), "r"(&t4mat_Identity)
        : "memory");
    *(int*)&mBaseRotX = 0;
    *(int*)&mBaseRotY = 0;
    *(int*)&mBaseRotZ = 0;
    mpAnimBankInfo = 0;
    mpSoundBankInfo = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_IsMatchingEntity);
#ifdef SKIP_ASM
extern "C" bool cRider_isLoaded(void* self);
extern "C" int cRider_getIdentity(void* self);
extern "C" int cScriptObjRider_IsSuitableRole(cScriptObjRider* self, cRider* rider, int id, int a3);
extern int kCharacterMappingTable[];

/**
 * Tests whether a script player entity can be played by this rider script object.
 *
 * Only entity type keSSXEntityType_Actor matches. Gets the attached rider (virtual slot 6, GetAttachedRider), which
 * must be loaded (cRider_isLoaded), and its identity bit from kCharacterMappingTable[cRider_getIdentity(rider)]. The
 * entity's actor header (GetSSXActorHeader) holds a role byte and a short character mask (character): a zero mask or
 * one containing the rider's bit passes, and the final answer comes from cScriptObjRider_IsSuitableRole(self, rider,
 * role, the entity's mpOwner) (On Tour IsSuitableRole(pRider, role, pPlayerMain)).
 *
 * @param msg  the script player entity (On Tour cScriptPlayerEntity* pEntity)
 * @param type  the entity type asked for; only keSSXEntityType_Actor can match
 * @return nonzero when the entity matches this rider
 */
int cScriptObjRider::IsMatchingEntity(cScriptPlayerEntity* msg, int type)
{
    if (type != keSSXEntityType_Actor) return 0;
    sVEntry<void*(void*)>* vt = (sVEntry<void*(void*)>*)mVtbl;
    void* rider = vt[6].fn(vthis(this, vt[6]));  // GetAttachedRider
    if (cRider_isLoaded(rider) == 0) return 0;
    int team = cRider_getIdentity(rider);
    int mask = kCharacterMappingTable[team];
    tSSXHeaderActor* info = (tSSXHeaderActor*)GetSSXActorHeader(msg);
    short charMask = info->character;
    if (charMask == 0 || (charMask & mask) != 0) {
        return IsSuitableRole((cRider*)rider, info->role, (int)msg->mpOwner);
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
void cScriptObjRider::ScriptAnimIDToGameAnimInfo(int scriptAnimId, void* gameAnimIndex, void* gameAnimBankId)
{
    *(int*)gameAnimBankId = mpAnimBankInfo->bankNum;
    *(int*)gameAnimIndex = (mpAnimBankInfo->pTableAnimID)[scriptAnimId];
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjRider_ScriptSoundIDToGameSoundInfo__FPviT0T0);
#ifdef SKIP_ASM
/**
 * Maps a script sound ID to the game sound index and bank of the rider's script sound bank: the index is the ID itself,
 * the bank is the bank number (bankNum) of the rider's sound bank info (mpSoundBankInfo).
 *
 * @param scriptSoundId  the script sound ID
 * @param gameSoundIndex  receives the game sound index (int)
 * @param gameSoundBankId  receives the sound bank number (int)
 */
void cScriptObjRider::ScriptSoundIDToGameSoundInfo(int scriptSoundId, void* gameSoundIndex, void* gameSoundBankId)
{
    *(int*)gameSoundBankId = mpSoundBankInfo->bankNum;
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
 * @return whether the rider suits the role (false also for negative or larger roles)
 */
bool cScriptObjRider::IsSuitableRole(cRider* rider, int id, int playerMain)
{
    if (id != 0) {
        if (id >= 0) {
            if (id < 22) {
                return ScriptEngine->GetSubjectRider(id - 1, playerMain) == ((void*)rider);
            }
        }
    } else {
        return true;
    }
    return false;
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
    cScriptObject_cScriptObject(&self->mObjType, keScriptObjectType_Event);
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
 * cAudioInterface_bxAudio() +0x118 then +0x1D8) and cleared; any other slot is cleared once
 * cBankMonitor_BANKMONITOR_GetHandle no longer finds the sound. If the flag at +0xF4 (On Tour mCheckForStopOnSpeechEnd)
 * is set it is cleared, and when func_002B0E28(audio + 0x5560, 0) returns non-zero the playing NIS list's scripts are
 * skipped (cSSXScriptEngine_SkipNISListScripts).
 */
void cScriptObjEvent::Update()
{
    cSSXAudio* snd = cAudioInterface_bxAudio();
    for (int i = 0; i < 4; i++) {
        tScriptObjEventSound* soundSlot = &((&mSounds[0]))[i];
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
    if (mCheckForStopOnSpeechEnd != 0) {
        mCheckForStopOnSpeechEnd = 0;
        if (func_002B0E28(&snd->SpeechSys, 0) != 0) {
            ScriptEngine->SkipNISListScripts(ScriptEngine->mPlayingNISList, 1, 1);
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
 * Calls cScriptObject_OnStartObjectUse__FPv on the cScriptObject part (+0xC), clears +0xEC, +0xF0 and +0xF4 (On Tour
 * mNumAbortWorldScripts, mHasTriggeredSpeechEvents, mCheckForStopOnSpeechEnd), binds the sound bank info (+0xF8, On
 * Tour mpSoundBankInfo) with cSSXScriptEngine_GetSoundBankInfo__FPv for the bank of the playing script (+0x1C of the
 * GetSSXScriptHeader header) and empties the 4 sound slots at +0xBC (ID -1, count 0, target -1). The player ID argument
 * is not declared (unused).
 */
void cScriptObjEvent::OnStartObjectUse()
{
    int i;
    cScriptObject_OnStartObjectUse(&mObjType);
    mNumAbortWorldScripts = 0;
    mHasTriggeredSpeechEvents = 0;
    mCheckForStopOnSpeechEnd = 0;
    tSSXScriptHeader* info = (tSSXScriptHeader*)((char*)GetSSXScriptHeader(mpEntity->mpOwner));
    int key = info->bankNum;
    mpSoundBankInfo = (tScriptSoundBankInfo*)cSSXScriptEngine_GetSoundBankInfo_2(ScriptEngine, key);
    tScriptObjEventSound* soundSlot = ((&mSounds[0]));
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
extern "C" void GetSSXGameEventFrame(void* self, tSSXFrameGameEvent* out);
extern void* cWScriptMan_TheWScriptMan;
extern int cRenderStateMan_m_indoor;

/**
 * Takes the script event object out of use: runs its abort world scripts, stops its sounds and clears its event state.
 *
 * After cScriptObject_OnEndObjectUse on the cScriptObject part (+0xC): runs each recorded abort world script (+0x1C, On
 * Tour mAbortWorldScripts: hash, world section; count +0xEC, mNumAbortWorldScripts) with cWScriptMan_runFunc; when
 * speech was triggered (+0xF0, mHasTriggeredSpeechEvents) calls func_002A1BD0 on the audio interface; stops each of the
 * 4 sound slots (+0xBC, mSounds) holding a handle (cBankMonitor_BANKMONITOR_StopHandle on the audio system's bank
 * monitor) and resets all four (handle -1, end flag 0, sound ID -1). When the entity's game-event frame
 * (GetSSXGameEventFrame) has an enviro-state list (+0x24), the render-state flag cRenderStateMan_m_indoor is cleared.
 * Finally clears +0xEC, +0xF0, +0xF4 (mCheckForStopOnSpeechEnd) and the sound bank info (+0xF8).
 */
void cScriptObjEvent::OnEndObjectUse()
{
    tSSXFrameGameEvent info;
    cScriptObject_OnEndObjectUse(&mObjType);
    for (int i = 0; i < mNumAbortWorldScripts; i++) {
        cWScriptMan_runFunc(cWScriptMan_TheWScriptMan, mAbortWorldScripts[i].worldSectionID,
                            mAbortWorldScripts[i].hashID);
    }
    if (mHasTriggeredSpeechEvents != 0) {
        func_002A1BD0(cAudioInterface_bxAudio());
    }
    for (int i = 0; i < 4; i++) {
        int id = mSounds[i].soundHandle;
        if (id >= 0) {
            cBankMonitor_BANKMONITOR_StopHandle(*(char**)*(char**)&cAudioInterface_bxAudio()->MusicSys + 0x1D8, id, 1, 0.0f);
        }
        tScriptObjEventSound* soundSlot = &mSounds[i];
        soundSlot->soundID = -1;
        soundSlot->soundHandle = -1;
        soundSlot->endSound = 0;
    }
    GetSSXGameEventFrame(mpEntity, &info);
    if (((int)info.pEnviroStateList) != 0) {
        cRenderStateMan_m_indoor = 0;
    }
    mNumAbortWorldScripts = 0;
    mHasTriggeredSpeechEvents = 0;
    mCheckForStopOnSpeechEnd = 0;
    mpSoundBankInfo = 0;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjEvent_IsMatchingEntity);
#ifdef SKIP_ASM
/**
 * Returns whether the script event object takes an InSeq entity: only a game event entity (type 7).
 *
 * @param entity  the InSeq player entity (unused)
 * @param type  the entity type
 * @return whether it is a game event entity
 */
bool cScriptObjEvent::IsMatchingEntity(cScriptPlayerEntity* entity, int type)
{
    return type == keSSXEntityType_GameEvent;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjEvent_OnInSeqKeyEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void GetSSXGameEventFrame(void* self, tSSXFrameGameEvent* out);
tSSXScriptHeader* GetSSXScriptHeader(void* pPlayer);
// PORT: the unit declares cSSXScriptEngine_GetTransportWorldSectionID with no parameters; this caller passes the engine
// in $a0.
int cSSXScriptEngine_GetTransportWorldSectionID_08E8(void* engine) __asm__("cSSXScriptEngine_GetTransportWorldSectionID");
extern "C" tBELocationDB* cBENewRaceInterface_getLocation(void* iface);
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


struct sSfxEnt_08E8 {
    int id;                  // 0x0
    int when;                // 0x4
    char pad8[0xC - 0x8];
    int slot;                // 0xC
    char pad10[0x1C - 0x10];
    signed char* vol;        // 0x1C
    sSfxEnt_08E8* next;      // 0x20
};


struct sSeCmd_08E8 {
    int id;
    int pad;
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
    int mHasTriggeredSpeechEvents;             // 0xF0
    int mCheckForStopOnSpeechEnd;             // 0xF4
};

#define MIN_08E8(a, b) ((a) < (b) ? (a) : (b))
#define MAX_08E8(a, b) ((a) < (b) ? (b) : (a))

/**
 * Handles an in-sequence key event for the script event object: runs the frame's world script events, music and speech
 * events, and starts or marks its sounds.
 *
 * Reads the entity's game-event frame (GetSSXGameEventFrame; On Tour tSSXFrameGameEvent). The world section is the
 * transport section (cSSXScriptEngine_GetTransportWorldSectionID) when the script header's first byte
 * (GetSSXScriptHeader on the entity's player) is set, else the section of the BE new-race location
 * (cSectionMan_getWorldSectionIDFromBELocationID). Then: - each world script event runs with
 * cWScriptMan_runFunc(section, event ID); its abort-script hash, unless D_004A525C (no script), is recorded once per
 * (hash, section) in the abort list (+0x1C, count +0xEC); - event bit 8 clears and bit 4 sets the render-state flag
 * cRenderStateMan_m_indoor; - each music event (kMusicEventID_None skipped) goes to func_0028E8C0 on the audio
 * interface; each speech event goes to func_002A19D8 and sets +0xF0, except kSpeechEventID_None (skipped) and
 * kSpeechEventID_StopOnSpeechEnd, which sets +0xF4 (stop on speech end); - bit 2: every sound slot holding a handle
 * gets its end flag set, then the slots of the frame's sounds clear it; - bit 1: walks the frame's sound list until a
 * sound whose start frame is before @p when; a sound whose slot (track index) is free, or holds a different sound
 * (stopped first), is started: bank and index from cScriptObjEvent_ScriptSoundIDToGameSoundInfo, a new SFX record on
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
    tSSXFrameGameEvent info;
    GetSSXGameEventFrame(obj, &info);
    void* iface0 = cBE_getInterface_Fv(cBE_getBE(), 0);
    int owner;
    if (*(signed char*)GetSSXScriptHeader(obj->mpOwner) != 0) {
        owner = cSSXScriptEngine_GetTransportWorldSectionID_08E8(ScriptEngine);
    } else {
        void* mgr = SSXApp->Game->SectionMan;
        owner = cSectionMan_getWorldSectionIDFromBELocationID(mgr, cBENewRaceInterface_getLocation(iface0)->locationID);
    }
    int i;
    for (i = 0; i < info.numWorldScriptEvent; i++) {
        tWorldScriptEvent* worldEvent = &(info.pWorldScriptEventArray)[i];
        cWScriptMan_runFunc(cWScriptMan_TheWScriptMan, owner, worldEvent->eventID);
        int id = *((int*)worldEvent->pCustom);
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
    cSSXAudio* snd = cAudioInterface_bxAudio();
    for (i = 0; i < info.numMusicEvent; i++) {
        sSeCmd_08E8* audioEvent = &((sSeCmd_08E8*)info.pMusicEventArray)[i];
        int id = audioEvent->id;
        if (id != tMusicEvent::kMusicEventID_None) {
            func_0028E8C0(snd, id, 0);
        }
    }
    for (i = 0; i < info.numSpeechEvent; i++) {
        sSeCmd_08E8* audioEvent = &((sSeCmd_08E8*)info.pSpeechEventArray)[i];
        int id = audioEvent->id;
        if (id != tSpeechEvent::kSpeechEventID_None) {
            if (id == tSpeechEvent::kSpeechEventID_StopOnSpeechEnd) {
                self->mCheckForStopOnSpeechEnd = 1;
            } else {
                func_002A19D8(snd, id);
                self->mHasTriggeredSpeechEvents = 1;
            }
        }
    }
    if (flags & 2) {
        for (i = 0; i < 4; i++) {
            if (self->sounds[i].id >= 0) {
                self->sounds[i].count = 1;
            }
        }
        for (sSfxEnt_08E8* soundEntry = ((sSfxEnt_08E8*)info.pSoundList); soundEntry != 0; soundEntry = soundEntry->next) {
            self->sounds[soundEntry->slot].count = 0;
        }
    }
    if (flags & 1) {
        for (sSfxEnt_08E8* soundEntry = ((sSfxEnt_08E8*)info.pSoundList); soundEntry != 0; soundEntry = soundEntry->next) {
            if ((float)soundEntry->when < when)
                break;
            int slot = soundEntry->slot;
            int play = 1;
            if (self->sounds[slot].id >= 0) {
                play = 0;
                if (self->sounds[slot].target != soundEntry->id) {
                    cBankMonitor_BANKMONITOR_StopHandle((char*)*(*(sWtTrack_08E8***)&snd->MusicSys) + 0x1D8, self->sounds[slot].id, 1, 0.0f);
                    play = 1;
                }
            }
            if (play) {
                int gameSoundIndex, type;
                cScriptObjEvent_ScriptSoundIDToGameSoundInfo(self, soundEntry->id, &gameSoundIndex, &type);
                int tp = type;
                int ar = gameSoundIndex;
                sWtTrack_08E8* pushSystem = ((sWtTrack_08E8*)snd->mpVBankManager);
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
                snd->mBank[((sWtTrack_08E8*)snd->mpVBankManager)->sub.cur] = tp;
                snd->mPatch[((sWtTrack_08E8*)snd->mpVBankManager)->sub.cur] = ar;
                {
                    sWtTrack_08E8* volumeSystem = *(*(sWtTrack_08E8***)&snd->MusicSys);
                    sWtSub_08E8* s2 = &volumeSystem->sub;
                    s2->p38[volumeSystem->sub.cur] = *soundEntry->vol;
                    s2->ents[volumeSystem->sub.cur].b0 = MAX_08E8(MIN_08E8(s2->p38[s2->cur], 0x7F), 0);
                }
                {
                    sWtTrack_08E8** pt = (*(sWtTrack_08E8***)&snd->MusicSys);
                    int volumeSetting = cSSXAudio_GetCurrAppliedVolumePtr(snd, 4, 0);
                    (*pt)->sub.p3C[(*pt)->sub.cur] = volumeSetting;
                }
                {
                    sWtTrack_08E8* resetSystem = *(*(sWtTrack_08E8***)&snd->MusicSys);
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
 * the bank is the bank number (bankNum) of the event object's sound bank info (mpSoundBankInfo).
 *
 * @param scriptSoundId  the script sound ID
 * @param gameSoundIndex  receives the game sound index (int)
 * @param gameSoundBankId  receives the sound bank number (int)
 */
void cScriptObjEvent::ScriptSoundIDToGameSoundInfo(int scriptSoundId, void* gameSoundIndex, void* gameSoundBankId)
{
    *(int*)gameSoundBankId = mpSoundBankInfo->bankNum;
    *(int*)gameSoundIndex = scriptSoundId;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_cScriptObjCameraManager);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
#include "scripter/ssxscriptengine_classes.h"
#include "object/being.h"

/**
 * Constructs a script camera manager: a cScriptObject of type 0 plus a script entity with nine entity slots.
 *
 * Builds the cScriptObject base (type 0) and the cScriptEntity base (+0x10), constructs nine 0x14-byte slots at +0x1C
 * (cScriptEntityCamera_cScriptEntityCamera) and points each one's owner word (+0x10) at the manager, clears the two
 * camera object pointers (+0xD0) and the transition object (+0xD8), sets the view index (+0xF8) to -1 and resets the
 * script state with cScriptObjCameraManager_ResetScriptManagementVars (On Tour ResetScriptManagementVars). The C++ view
 * class here is named cScriptEngine_1018.
 */
cScriptEngine_1018::cScriptEngine_1018() : cScriptObject_1018(0)
{
    for (int i = 1; i >= 0; i--) {
        mpCameraObjects[i] = 0;
    }
    for (int i = 0; i < 9; i++) {
        slots[i].setOwner(this);
    }
    mpTransitionObject = 0;
    mViewIndex = -1;
    cScriptObjCameraManager_ResetScriptManagementVars(this);
}

// PORT: the retail names of the vtables g++ references here.
__asm__("_vt$18cScriptEngine_1018$18cScriptEntity_1018 = cScriptObjCameraManager_cScriptEntity_vtbl\n"
        "_vt$18cScriptEngine_1018 = cScriptObjCameraManager_vtbl\n");
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_Update);
#ifdef SKIP_ASM
extern "C" void cScriptObjCameraManager_SetupCameraObject(cScriptObjCameraManager* self, int a1, int a2);


/**
 * Per-frame update of a script camera manager in use: follows its view's camera and sets its cameras up once.
 *
 * Only while in use (mInUse > 0): copies the position (Pose.P) of the camera of view mViewIndex
 * (SSXApp->Game->ViewMan->PlayerCamera) into the focus box position (mFocusBox) and, while mpEntity is still 0, calls
 * cScriptObjCameraManager_SetupCameraObject(self, 0, 0) (On Tour SetupCameraObject). cSSXScriptEngine_Update runs it on
 * both managers every frame.
 */
void cScriptObjCameraManager::Update()
{
    if (mInUse > 0) {
        cCamera* camera = SSXApp->Game->ViewMan->PlayerCamera[mViewIndex];
        *(&mFocusBox) = camera->Pose.P;
        if ((int)mpEntity == 0) {
            SetupCameraObject(0, 0);
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
void cScriptObjCameraManager::SetViewIndex(int val)
{
    mViewIndex = val;
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
 * Returns script entity @p index of a script camera manager: 0 is its own cScriptEntity part (mEntityVtbl), 1-9 its
 * entity cameras (mEntityCameras, cScriptEntityCamera). The index is not range-checked.
 *
 * @param index  the entity index (0-9)
 * @return the entity
 */
void* cScriptObjCameraManager::GetScriptEntity(int index)
{
    if (index == 0) {
        return (char*)&mEntityVtbl;
    }
    return mEntityCameras[index - 1];
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
 * @return true, or false when both slots are taken
 */
bool cScriptObjCameraManager::AddCameraObject(cScriptObjCamera* obj)
{
    void** slots = (void**)&mpCameraObjects;
    for (int i = 0; i < 2; i++) {
        if (slots[i] == 0) {
            slots[i] = ((void*)obj);
            obj->SetCameraManager((int)this); // PORT: pointer passed as int (callee mangled __FPvi)
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_RemoveCameraObject);
#ifdef SKIP_ASM
void cScriptObjCamera_SetCameraManager(void* self, int val);
extern "C" void cScriptObjCamera_SetCameraEntity(cScriptObjCamera* self, void* anim);

// PORT: the unit declares cScriptObjCameraManager_RemoveCameraObject(void*, void*) returning void; it returns found/not
// found.
extern "C" bool cScriptObjCameraManager_RemoveCameraObject_impl(cScriptObjCameraManager* self, cScriptObjCamera* key)
    __asm__("cScriptObjCameraManager_RemoveCameraObject");
/**
 * Detaches a script camera object from the camera manager.
 *
 * Finds the camera slot (mpCameraObjects) holding an object with the same object ID (mObjID) as @p key. If the
 * manager is in use (mInUse) and that camera has an entity bound (mpCameraEntity), unbinds it
 * (cScriptObjCamera_SetCameraEntity with 0) and frees the attachment (mAttachedCameraObject) that pointed at the slot.
 * Then clears the camera's manager (cScriptObjCamera_SetCameraManager with 0) and the slot.
 *
 * @param key  the script camera object
 * @return true, or false when it was not attached
 */
extern "C" bool cScriptObjCameraManager_RemoveCameraObject_impl(cScriptObjCameraManager* self, cScriptObjCamera* key)
{
    int k = key->mObjID;
    for (int i = 0; i < 2; i++) {
        cScriptObjCamera* cameraObj = self->mpCameraObjects[i];
        if (cameraObj != 0 && cameraObj->mObjID == k) {
            if (self->mInUse > 0 && (int)cameraObj->mpCameraEntity != 0) {
                cameraObj->SetCameraEntity(0);
                for (int j = 0; j < 2; j++) {
                    if (self->mAttachedCameraObject[j] == i) {
                        self->mAttachedCameraObject[j] = -1;
                        break;
                    }
                }
            }
            self->mpCameraObjects[i]->SetCameraManager(0);
            self->mpCameraObjects[i] = 0;
            return true;
        }
    }
    return false;
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
 * @p obj 0 clears it: a transition in use (+0x8) is stopped (cScriptObjTransition_SetTransition with player -1 and
 * transition 9, none) and +0xE8 (in transition) cleared, then the object's camera manager is cleared
 * (cScriptObjTransition_SetCameraManager) and the slot emptied. Otherwise @p obj is stored, when no object is set yet,
 * and given this manager.
 *
 * Likely bug: clearing when no transition object is set dereferences a null pointer.
 *
 * @param obj  the script transition object, or 0 to clear
 * @return true, or false when another transition object is already set
 */
bool cScriptObjCameraManager::SetTransitionObject(void* obj)
{
    if (obj == 0) {
        cScriptObjTransition* cur = mpTransitionObject;
        if (cur->mInUse > 0) {
            cScriptObjTransition_SetTransition_f(cur, -1, 9, 0.0f, 0.0f, 0.0f, 0.0f);
            mInTransition = 0;
        }
        cScriptObjTransition_SetCameraManager_p(mpTransitionObject, 0);
        mpTransitionObject = 0;
        return true;
    }
    if (mpTransitionObject != 0) {
        return false;
    }
    mpTransitionObject = (cScriptObjTransition*)obj;
    cScriptObjTransition_SetCameraManager_p(obj, this);
    return true;
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
 * Marks the object reserved for the player in the script database (cScriptDatabaseObject_MarkObjectAsReserved on
 * cScriptDatabaseObject_GetDatabaseObject(), On Tour MarkObjectAsReserved) and resets the management state
 * (cScriptObjCameraManager_ResetScriptManagementVars). When cSSXScriptEngine_IsPlayingNISListPlayer accepts the
 * player's script player (cBXScriptEngine_GetRequestPlayerByPlayerID__FPv) it adds the focus box (+0x100) to the AI's
 * focus manager (cFocusMan_addbox, On Tour cFocusMan::addbox, on AI +0xA4) and sets the focus-box flag (+0x110). Ends
 * with cScriptObject_OnStartObjectUse__FPv (one more use).
 *
 * @param val  the player ID
 */
void cScriptObjCameraManager::OnStartObjectUse(int val)
{
    cScriptDatabaseObject_GetDatabaseObject()->MarkObjectAsReserved(mObjID, val);
    ResetScriptManagementVars();
    // PORT: pointer passed as int
    if (ScriptEngine->IsPlayingNISListPlayer((int)cBXScriptEngine_GetRequestPlayerByPlayerID_2(ScriptEngine,
            val)) != 0) {
        cFocusMan_addbox(SSXApp->Game->AI->FocusMan, (char*)&mFocusBox);
        mUseFocusBox = 1;
    }
    cScriptObject_OnStartObjectUse_2(this, val);
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

/**
 * Releases a script camera manager: unreserves it, detaches its cameras and transition and removes its focus box.
 *
 * Unreserves the object in the script database (cScriptDatabaseObject_MarkObjectAsUnreserved on
 * cScriptDatabaseObject_GetDatabaseObject(), On Tour MarkObjectAsUnreserved), clears the camera entity
 * (cScriptObjCamera_SetCameraEntity, On Tour cScriptObjCamera::SetCameraEntity) of each attached camera (indices in
 * mAttachedCameraObject into mpCameraObjects) and sets its index to -1, resets the transition object
 * (mpTransitionObject) with cScriptObjTransition_SetTransition(-1, 9, 0.0, 0.0, 0.0, 0.0) (On Tour
 * cScriptObjTransition::SetTransition), removes the focus box (mFocusBox) from the AI's focus manager when it was added
 * (mUseFocusBox; cFocusMan_removebox, On Tour cFocusMan::removebox), resets the state
 * (cScriptObjCameraManager_ResetScriptManagementVars) and drops one use (cScriptObject_OnEndObjectUse).
 *
 * @param playerID  the player ID
 */
void cScriptObjCameraManager::OnEndObjectUse(int playerID)
{
    cScriptDatabaseObject_GetDatabaseObject()->MarkObjectAsUnreserved(mObjID);
    for (int i = 0; i < 2; i++) {
        int cameraIndex = mAttachedCameraObject[i];
        if (cameraIndex >= 0) {
            mpCameraObjects[cameraIndex]->SetCameraEntity(0);
            mAttachedCameraObject[i] = -1;
        }
    }
    if (mpTransitionObject != 0) {
        cScriptObjTransition_SetTransition_f(mpTransitionObject, -1, 9, 0.0f, 0.0f, 0.0f, 0.0f);
    }
    if (mUseFocusBox != 0) {
        cAI* lvl = SSXApp->Game->AI;
        cFocusMan_removebox(lvl->FocusMan, &mFocusBox, lvl);
        mUseFocusBox = 0;
    }
    ResetScriptManagementVars();
    cScriptObject_OnEndObjectUse_2(this, playerID);
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_OnInSeqKeyEvent);
#ifdef SKIP_ASM
extern "C" void GetSSXCameraManagerFrame(void* self, tSSXFrameCameraManager* frame);
extern "C" void cScriptObjCameraManager_SetupCameraObject(cScriptObjCameraManager* self, int a1, int a2);
extern "C" void cScriptObjCameraManager_DoCameraTransition(cScriptObjCameraManager* self, tSSXFrameCameraManager* frame,
                                                          float t);
// PORT: cScriptObjTransition_SetTransition's mangled signature is (void*, int, int); it also takes four floats (same
// alias as cScriptObjCameraManager_SetTransitionObject).
void* cScriptObjTransition_SetTransition_f(void* self, int a1, int a2, float f0, float f1, float f2, float f3)
    __asm__("cScriptObjTransition_SetTransition__FPvii");

/**
 * Handles an in-sequence key event for the camera manager: starts camera transitions and releases unused tracks.
 *
 * Does nothing unless @p flags has bit 4 or 8. Reads the manager frame (GetSSXCameraManagerFrame: active camera count,
 * list). - Bit 4: starts the transition to the first active camera (cScriptObjCameraManager_DoCameraTransition), except
 * that with music sync on (the header's musicSync, GetSSXCameraManagerHeader), 2 or more active cameras, no beat yet
 * (mBeatOccurred) and that camera's transition-in 8, it only marks a cut as pending on the beat (mCutOnBeat). - Bit 8:
 * with no active cameras, stops the transition object (mpTransitionObject;
 * cScriptObjTransition_SetTransition, transition 9) and releases both tracks (cScriptObjCameraManager_SetupCameraObject
 * with 9). Otherwise runs a pending cut and clears it, then releases each track no active camera uses.
 *
 * @param obj  the InSeq player entity
 * @param type  the entity type (unused)
 * @param flags  the event bits
 * @param currFrame  the current frame
 */
void cScriptObjCameraManager::OnInSeqKeyEvent(cScriptPlayerEntity* obj, int type, int flags, float currFrame)
{
    if ((flags & 0xC) == 0)
        return;
    tSSXFrameCameraManager frame;
    GetSSXCameraManagerFrame(obj, &frame);
    if (flags & 4)
    {
        int go = 1;
        if (GetSSXCameraManagerHeader(obj)->musicSync != 0)
        {
            tActiveCamera* head = frame.pActiveCameraList;
            if (frame.numActiveCamera >= 2 && mBeatOccurred == 0
                && head->pCustom->transitionIn == keScriptTransition_Cut)
            {
                mCutOnBeat = go;
                go = 0;
            }
        }
        if (go)
            DoCameraTransition(&frame, currFrame);
    }
    if (flags & 8)
    {
        if (frame.numActiveCamera == 0)
        {
            void* fade = mpTransitionObject;
            if (fade)
                cScriptObjTransition_SetTransition_f(fade, -1, 9, 0.0f, 0.0f, 0.0f, 0.0f);
            for (int i = 0; i < 2; i++)
                SetupCameraObject(i, 9);
            return;
        }
        if (mCutOnBeat)
        {
            DoCameraTransition(&frame, currFrame);
            mCutOnBeat = 0;
        }
        int mask = 0;
        for (tActiveCamera* n = frame.pActiveCameraList; n; n = n->pNext)
            mask |= 1 << n->data.track;
        for (int i = 0; i < 2; i++)
        {
            if (((mask >> i) & 1) == 0)
                SetupCameraObject(i, 9);
        }
    }
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_OnInSeqUpdate);
#ifdef SKIP_ASM
extern "C" void GetSSXCameraManagerFrame(void* self, tSSXFrameCameraManager* frame);
void cScriptObjCamera_SetCameraWeight(void* self, float val);
extern "C" int cScriptObjCameraManager_GetCameraObjectByTrack(cScriptObjCameraManager* self, int i);
extern "C" void cScriptObjCameraManager_DoCameraTransition(cScriptObjCameraManager* self, tSSXFrameCameraManager* frame,
                                                          float t);
extern "C" void cScriptObjTransition_Update(void* self, float dt);
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" float func_002B4920(void* p);

/**
 * Updates the camera manager for this frame: tracks the music beat and advances the running camera transition.
 *
 * With music sync on (the header's musicSync, GetSSXCameraManagerHeader): clears the beat flag (mBeatOccurred); when
 * the audio's song plays (MusicSys.mSongPlaying) the beat time left (func_002B4920) rising above the last value
 * (mLastBeatTimeLeft) is a beat: the flag is set and a pending cut (mCutOnBeat) runs now
 * (cScriptObjCameraManager_DoCameraTransition). The value is stored in mLastBeatTimeLeft. Without a song the beat flag
 * is just set.
 *
 * Then, with active cameras and a transition running (mInTransition), by the first camera's transition-in
 * (eScriptTransition): - keScriptTransition_CameraBlend:
 * its track's camera gets weight (frame - start) / overlap frames and the other track's camera the rest; the transition
 * ends after start + overlap, or at once when the other track has no camera. - Fade .. WipeCircle: advances the
 * transition object
 * (cScriptObjTransition_Update) by @p dt, ending when it is no longer in use (mInUse); until then, and before start +
 * overlap, the other camera keeps weight 1 and this one 0; after, this one gets 1. - CrossFade: advances the transition
 * object as above. Cut: nothing.
 *
 * @param obj  the InSeq player entity
 * @param currFrame  the current frame
 * @param dt  the frame delta
 */
void cScriptObjCameraManager::OnInSeqUpdate(void* obj, float currFrame, float dt)
{
    tSSXFrameCameraManager frame;
    GetSSXCameraManagerFrame(obj, &frame);
    if (GetSSXCameraManagerHeader(obj)->musicSync != 0) {
        mBeatOccurred = 0;
        cSSXAudio* soundMan = cAudioInterface_bxAudio();
        void* timer = (char*)&soundMan->MusicSys;
        if (soundMan->MusicSys.mSongPlaying != 0) {
            float now = func_002B4920(timer);
            if (mLastBeatTimeLeft < now) {
                mBeatOccurred = 1;
                if (mCutOnBeat != 0) {
                    DoCameraTransition(&frame, currFrame);
                    mCutOnBeat = 0;
                }
            }
            mLastBeatTimeLeft = now;
        } else {
            mBeatOccurred = 1;
        }
    }
    if (frame.numActiveCamera == 0) {
        return;
    }
    if (mInTransition == 0) {
        return;
    }
    tActiveCamera* n = frame.pActiveCameraList;
    switch (n->pCustom->transitionIn) {
    case keScriptTransition_CameraBlend: {
        if ((float)(n->data.overlapInFrames + n->data.scriptStartFrame) < currFrame) {
            mInTransition = 0;
        }
        int camera = GetCameraObjectByTrack(n->data.track);
        int otherCamera = GetCameraObjectByTrack(1 - n->data.track);
        if (otherCamera == 0) {
            mInTransition = 0;
            break;
        }
        float w = 1.0f;
        if ((float)(n->data.overlapInFrames + n->data.scriptStartFrame) < currFrame) {
            mInTransition = 0;
        } else {
            w = (currFrame - (float)n->data.scriptStartFrame) / (float)n->data.overlapInFrames;
        }
        cScriptObjCamera_SetCameraWeight((void*)camera, w);
        cScriptObjCamera_SetCameraWeight((void*)otherCamera, 1.0f - w);
        break;
    }
    case keScriptTransition_Fade:
    case keScriptTransition_WipeLeft:
    case keScriptTransition_WipeRight:
    case keScriptTransition_WipeUp:
    case keScriptTransition_WipeDown:
    case keScriptTransition_WipeCircle: {
        int camera = GetCameraObjectByTrack(n->data.track);
        int otherCamera = GetCameraObjectByTrack(1 - n->data.track);
        cScriptObjTransition_Update(mpTransitionObject, dt);
        if (mpTransitionObject->mInUse <= 0) {
            mInTransition = 0;
        }
        if (mInTransition != 0 && otherCamera != 0 &&
            currFrame < (float)(n->data.overlapInFrames + n->data.scriptStartFrame)) {
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
    case keScriptTransition_CrossFade:
        cScriptObjTransition_Update(mpTransitionObject, dt);
        if (mpTransitionObject->mInUse <= 0) {
            mInTransition = 0;
        }
        break;
    case keScriptTransition_Cut:
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
 * Returns whether the camera manager takes an InSeq entity: entity type 4 whose script setup info
 * (cSSXScriptEngine_GetScriptSetupInfo for the entity's player, mpOwner) has this manager's view index (mViewIndex) as
 * its viewIndex.
 *
 * @param entity  the InSeq player entity
 * @param n  the entity type
 * @return whether it matches
 */
bool cScriptObjCameraManager::IsMatchingEntity(cScriptPlayerEntity* entity, int n)
{
    if (n != keSSXEntityType_CameraManager) {
        return false;
    }
    tSSXScriptSetupInfo* setupInfo =
        (tSSXScriptSetupInfo*)cSSXScriptEngine_GetScriptSetupInfo_2(ScriptEngine, (int)entity->mpOwner);
    return setupInfo->viewIndex == mViewIndex;
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_ResetScriptManagementVars);
#ifdef SKIP_ASM
/**
 * Resets a script camera manager's per-script state: no attached cameras, no transition or beat, no focus box.
 *
 * Sets both attached camera indices (mAttachedCameraObject) to -1, the beat flag (mBeatOccurred) to 1, the last beat
 * time (mLastBeatTimeLeft) to 1e9 and the focus box position (mFocusBox) to (0, 0, 0, 1), and clears the main camera
 * index (mMainCameraIndex), the in-transition flag (mInTransition), the cut-on-beat flag (mCutOnBeat) and the
 * focus-box flag (mUseFocusBox). On Tour's bools are bytes; here they are words, so the fields after mMainCameraIndex
 * sit 8 bytes later than in On Tour.
 */
void cScriptObjCameraManager::ResetScriptManagementVars()
{
    int i;
    int noCamera = -1;
    int* ids = mAttachedCameraObject;
    for (i = 1; i >= 0; i--) {
        ids[i] = noCamera;
    }
    mBeatOccurred = 1;
    mLastBeatTimeLeft = 1000000000.0f;
    mFocusBox = t4VectorXYZW(0.0f, 0.0f, 0.0f, 1.0f);
    mMainCameraIndex = 0;
    mInTransition = 0;
    mCutOnBeat = 0;
    mUseFocusBox = 0;
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
 * Points the camera of track @p ch at the playing entity camera of subtype @p anim, or releases the track for
 * kCameraSubType_None.
 *
 * A track without a camera (+0xDC holds -1) takes the first camera object (+0xD0) not in use (+0x8); with none free, or
 * for kCameraSubType_None, nothing happens. kCameraSubType_None unbinds the camera's entity
 * (cScriptObjCamera_SetCameraEntity with 0) and frees the track. Otherwise, unless the camera already shows that
 * subtype (its entity's +0xC), the first of the 9 entity cameras (+0x1C) that is playing (cScriptEntity_IsPlaying) with
 * subtype @p anim is bound (cScriptObjCamera_SetCameraEntity); when none is, the track stays as it was.
 *
 * @param ch  the track index (0 or 1)
 * @param anim  the camera subtype, or kCameraSubType_None
 */
void cScriptObjCameraManager::SetupCameraObject(int ch, int anim)
{
    sSseEngine1AC8* self = (sSseEngine1AC8*)this;
    int slot = self->chans[ch];
    if (slot < 0) {
        if (anim == kCameraSubType_None) {
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
    if (anim == kCameraSubType_None) {
        ((cScriptObjCamera*)self->rigs[slot])->SetCameraEntity(0);
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
                ((cScriptObjCamera*)self->rigs[slot])->SetCameraEntity(cameraEntity);
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
int cScriptObjCameraManager::GetCameraObjectByTrack(int i)
{
    int j = mAttachedCameraObject[i];
    if (j < 0) {
        return 0;
    }
    return (int)mpCameraObjects[j];
}
#endif

//100%
INCLUDE_ASM("scripter/ssxscriptengine", cScriptObjCameraManager_DoCameraTransition);
#ifdef SKIP_ASM
extern "C" cScriptDatabaseObject* cScriptDatabaseObject_GetDatabaseObject(void);
extern "C" int cScriptDatabaseObject_GetPlayerIDByEntity(cScriptDatabaseObject* self, cScriptEntity* a1);
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

/**
 * Starts the camera transition to the first active camera of a camera-manager frame.
 *
 * For the other track: with a transition-in of 7 or more its camera is released
 * (cScriptObjCameraManager_SetupCameraObject with 9), else its camera's weight is set to 0. The camera's own track is
 * set up for its value (cScriptObjCameraManager_SetupCameraObject). The in-transition flag (mInTransition) is then set
 * unless the transition-in is 8 (cut) or the overlap is 0 frames. For 7 and 9 the transition object
 * (mpTransitionObject) gets cScriptObjTransition_SetTransition, and for 1-6 cScriptObjTransition_SetTransition1 (with
 * colour, out and hold frames), with the player using the manager's entity (cScriptDatabaseObject_GetPlayerIDByEntity)
 * and the frames since start; with no transition object the flag is cleared. 0 (blend) needs no object.
 *
 * @param frame  the camera-manager frame (active camera count, active camera list)
 * @param currFrame  the current frame
 */
void cScriptObjCameraManager::DoCameraTransition(tSSXFrameCameraManager* frame, float currFrame)
{
    tActiveCamera* node = frame->pActiveCameraList;
    int bit = node->data.track;
    for (int i = 0; i < 2; i++)
    {
        if (i == bit)
            continue;
        if (node->pCustom->transitionIn >= 7)
        {
            SetupCameraObject(i, 9);
            continue;
        }
        void* camera = cScriptObjCameraManager_GetCameraObjectByTrack_p(this, i);
        if (camera)
            cScriptObjCamera_SetCameraWeight(camera, 0.0f);
    }
    SetupCameraObject(bit, node->data.value);
    mInTransition = 0;
    if (node->pCustom->transitionIn == keScriptTransition_Cut)
        return;
    if (node->data.overlapInFrames == 0)
        return;
    mInTransition = 1;
    int k = node->pCustom->transitionIn;
    if (k >= 7)
    {
        void** pf = (void**)&mpTransitionObject;
        if (*pf == 0)
        {
            mInTransition = 0;
            return;
        }
        int id = cScriptDatabaseObject_GetDatabaseObject()->GetPlayerIDByEntity((cScriptEntity*)((char*)&mEntityVtbl));
        cScriptObjTransition_SetTransition_f(*pf, id, node->pCustom->transitionIn,
                                             currFrame - (float)node->data.scriptStartFrame,
                                             (float)node->data.overlapInFrames, 0.0f, 0.0f);
    }
    else if (k > 0)
    {
        void** pf = (void**)&mpTransitionObject;
        if (*pf == 0)
        {
            mInTransition = 0;
            return;
        }
        int id = cScriptDatabaseObject_GetDatabaseObject()->GetPlayerIDByEntity((cScriptEntity*)((char*)&mEntityVtbl));
        cScriptObjTransition_SetTransition1(*pf, id, node->pCustom->transitionIn, node->pCustom->transitionColour,
                                            currFrame - (float)node->data.scriptStartFrame,
                                            (float)node->pCustom->transitionOutFrames,
                                            (float)node->data.overlapInFrames,
                                            (float)node->pCustom->transitionHoldFrames);
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
 * Then runs the database destructor (cScriptDatabaseObject__cScriptDatabaseObject), which frees the object when asked
 * to.
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

