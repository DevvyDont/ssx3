#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "world/worldview.h"
#include "bx/math3d.h"
#include "render/occlusionman.h"
#include "render/ps2graphicsman.h"
#include "movie/mcoverlaymanager.h"
#include "main/ssxapp.h"
#include "util/bxmemcard.h"
#include "util/locale.h"

extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cMCOverlayManager_cMCOverlayManager(cMCOverlayManager* self);
extern const char str_MCManager[];
extern void* cMCOverlayManager_instance;

//99.53%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getManager__Fv);
#ifdef SKIP_ASM
/**
 * Returns the memory-card overlay manager, creating it on first use.
 *
 * The single instance (0x74C bytes, allocated as "MCManager") is kept in cMCOverlayManager_instance.
 *
 * @return the cMCOverlayManager instance
 */
void* cMCOverlayManager_getManager()
{
    if (cMCOverlayManager_instance == 0) {
        void* mem = cMemMan_alloc(1868, str_MCManager, 0, 0);
        cMCOverlayManager_instance = cMCOverlayManager_cMCOverlayManager((cMCOverlayManager*)mem);
    }
    return cMCOverlayManager_instance;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_hasManager);
#ifdef SKIP_ASM
/**
 * Tells whether the memory-card overlay manager has been created (cMCOverlayManager_instance is set).
 *
 * @return 1 when the instance exists, else 0
 */
extern "C" bool cMCOverlayManager_hasManager(void) {
    return cMCOverlayManager_instance != 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_purge);
#ifdef SKIP_ASM
/**
 * Destroys the memory card overlay manager singleton.
 *
 * Deletes cMCOverlayManager_instance and sets it to null.
 */
extern "C" void cMCOverlayManager_purge()
{
    delete (cMCOverlayManager*)cMCOverlayManager_instance;
    cMCOverlayManager_instance = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_isManagerDone);
#ifdef SKIP_ASM
/**
 * Tells whether the memory card manager has finished its operation: mState (+0x130) is 1 (On Tour kMCOverlay_Done).
 *
 * @return 1 when done, otherwise 0
 */
bool cMCOverlayManager::isManagerDone()
{
    return mState == kMCOverlay_Done;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_isManagerLoaded);
#ifdef SKIP_ASM
/**
 * Tells whether the manager's load has finished: mState is 35 (On Tour's kMCOverlay_LoadDone).
 *
 * @return 1 when loaded, else 0
 */
bool cMCOverlayManager::isManagerLoaded()
{
    return (mState) == kMCOverlay_LoadDone;
}
#endif
//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_isManagerSaved);
#ifdef SKIP_ASM
/**
 * Returns mSaved, set when the manager's last save completed.
 *
 * @return mSaved
 */
int cMCOverlayManager::isManagerSaved()
{
    return (mSaved);
}
#endif
//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_isSaveInProgress);
#ifdef SKIP_ASM
extern int cMCOverlayManager_mSaveInProgress;

/**
 * Tells whether a memory-card write is in progress (cMCOverlayManager_mSaveInProgress is set).
 *
 * @return 1 while saving, else 0
 */
extern "C" bool cMCOverlayManager_isSaveInProgress(void) {
    return cMCOverlayManager_mSaveInProgress != 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_hasCardStatusCriticalError);
#ifdef SKIP_ASM
/**
 * Tells whether the last card state has an error other than the non-critical ones.
 *
 * Masks bit 0x4 (card full) and bits 0x1E0 out of mCurrDeviceStateLast (+0x43C) and tests what is left.
 *
 * @return 1 when another bit is set, otherwise 0
 */
bool cMCOverlayManager::hasCardStatusCriticalError()
{
    // masks applied as separate statements; folding them into one
    // expression collapses the two `and` instructions into one
    int stateTest = mCurrDeviceStateLast;
    stateTest &= -5;
    stateTest &= -481;
    return stateTest != 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_readSetFileType);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_r() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" int cBESaveInterface_getSaveProfileSize();
extern "C" void* cMCOverlayManager_SetAction(cMCOverlayManager* self, int mode);
extern "C" void strcpy3(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void cMCOverlayManager_getFileName_r(void* self, int mode, void* buf, int size) __asm__("cMCOverlayManager_getFileName");
extern "C" void cMCOverlayManager_getProductCode_r(void* buf, int size) __asm__("cMCOverlayManager_getProductCode");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern char str_pct_s_pct_s_e[];


/**
 * Prepares a memory-card read of file type @p mode (tMCFileType: replay, profile or settings).
 *
 * Stores mFileType, switches to the device-select action (kMCOVAction_DeviceSelect), sets mSaveProfileID to -1
 * and mSave to 0, sets mMaxFiles (4 replays, 6 profiles, 1 settings file) and the card's data size (mpMemCard slot 19:
 * 0x80000 for replays, else cBESaveInterface_getSaveProfileSize / cBESaveInterface_getSaveGameSize), then writes the
 * search pattern "<product code>-<type>*" (cMCOverlayManager_getProductCode, cMCOverlayManager_getFileName) into mMatch.
 *
 * @param mode  the file type
 */
void cMCOverlayManager::readSetFileType(int mode)
{
    char desc[0x200];
    char name[0x200];
    char fmt[0x200];
    mFileType = mode;
    SetAction(kMCOVAction_DeviceSelect);
    mSaveProfileID = -1;
    mSave = 0;
    int fileType = mFileType;
    if (fileType == kMCFileType_Replay) {
        mMaxFiles = 4;
        cMemCard* memCard = mpMemCard;
        memCard->SetDataSize(0x80000);
    } else if (fileType == kMCFileType_Profile) {
        mMaxFiles = 6;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int fileSize = cBESaveInterface_getSaveProfileSize();
        cMemCard* memCard = mpMemCard;
        memCard->SetDataSize(fileSize);
    } else {
        mMaxFiles = 1;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int fileSize = cBESaveInterface_getSaveGameSize_r();
        cMemCard* memCard = mpMemCard;
        memCard->SetDataSize(fileSize);
    }
    strcpy3(fmt, str_pct_s_pct_s_e);
    cMCOverlayManager_getProductCode_r(desc, 0x200);
    cMCOverlayManager_getFileName_r(this, mFileType, name, 0x100);
    sprintf1_v(&mMatch[0], fmt, desc, name);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_readFile);
#ifdef SKIP_ASM
/**
 * Starts reading a save file from the memory card.
 *
 * Calls virtual slot 29 of the memory card object (+0x434) with the three arguments and stores @p size at +0xE8.
 *
 * @param file  which file
 * @param data  destination buffer
 * @param size  bytes to read
 */
void cMCOverlayManager::readFile(int file, int data, int size)
{
    cMemCard* obj = mpMemCard;
    obj->StartFileSlotRead(file, data, size);
    mLoadedSize = size;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_readClose);
#ifdef SKIP_ASM
/**
 * Closes the current card read: calls mpMemCard's vtable slot 32 (On Tour's cMemCard::SetReadClose by declaration
 * order).
 */
void cMCOverlayManager::readClose()
{
    cMemCard* obj = mpMemCard;
    obj->SetReadClose();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_readSetSize);
#ifdef SKIP_ASM
// PORT: the unit declares cMCOverlayManager_readSetSize(void* self), but the body takes a
// second argument (callers pass it in $5); the real body is bound by asm label.

/**
 * Sets the size of the file to read or write: passes @p size to mpMemCard's slot 19 (SetDataSize by On Tour's cMemCard
 * order) and stores it in mSize.
 *
 * @param size  the file's data size
 */
void cMCOverlayManager::readSetSize(int size)
{
    cMemCard* obj = mpMemCard;
    obj->SetDataSize(size);
    mSize = size;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_writeSetFileType);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_r() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" int cBESaveInterface_getSaveProfileSize();
extern "C" void* cMCOverlayManager_SetAction(cMCOverlayManager* self, int mode);
extern "C" void strcpy3(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void cMCOverlayManager_getFileName_r(void* self, int mode, void* buf, int size) __asm__("cMCOverlayManager_getFileName");
extern "C" void cMCOverlayManager_getProductCode_r(void* buf, int size) __asm__("cMCOverlayManager_getProductCode");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern char str_pct_s_pct_s_e[];


/**
 * Prepares a memory-card write of file type @p mode for profile @p profileId.
 *
 * As cMCOverlayManager_readSetFileType, but sets mSave to 1 and mSaveProfileID to @p profileId: stores mFileType, switches to the
 * device-select action, sets mMaxFiles and the card's data size by type and builds mMatch.
 *
 * @param mode  the file type (tMCFileType)
 * @param profileId  the profile to save, or -1
 */
void cMCOverlayManager::writeSetFileType(int mode, int profileId)
{
    char desc[0x200];
    char name[0x200];
    char fmt[0x200];
    mFileType = mode;
    SetAction(kMCOVAction_DeviceSelect);
    mSaveProfileID = profileId;
    mSave = 1;
    int fileType = mFileType;
    if (fileType == kMCFileType_Replay) {
        mMaxFiles = 4;
        cMemCard* memCard = mpMemCard;
        memCard->SetDataSize(0x80000);
    } else if (fileType == kMCFileType_Profile) {
        mMaxFiles = 6;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int fileSize = cBESaveInterface_getSaveProfileSize();
        cMemCard* memCard = mpMemCard;
        memCard->SetDataSize(fileSize);
    } else {
        mMaxFiles = 1;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int fileSize = cBESaveInterface_getSaveGameSize_r();
        cMemCard* memCard = mpMemCard;
        memCard->SetDataSize(fileSize);
    }
    strcpy3(fmt, str_pct_s_pct_s_e);
    cMCOverlayManager_getProductCode_r(desc, 0x200);
    cMCOverlayManager_getFileName_r(this, mFileType, name, 0x100);
    sprintf1_v(&mMatch[0], fmt, desc, name);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_writeClose);
#ifdef SKIP_ASM
/**
 * Closes the current card write when one was started (mWriteInProgress): calls mpMemCard's slot 43 (SetWriteClose by On
 * Tour's cMemCard order).
 */
void cMCOverlayManager::writeClose()
{
    if (mWriteInProgress != 0) {
        cMemCard* obj = mpMemCard;
        obj->SetWriteClose();
    }
}
#endif

extern "C" void* cMCOverlayManager_readSetSize(void* self);

//99.29%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_writeSetSize__FPv);
#ifdef SKIP_ASM
/**
 * Sets the size of the file to write: forwards to cMCOverlayManager_readSetSize (the size arrives in the second argument register).
 *
 * @return whatever cMCOverlayManager_readSetSize left in the return register
 */
void* cMCOverlayManager::writeSetSize()
{
    return cMCOverlayManager_readSetSize(this);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetTitleString);
#ifdef SKIP_ASM
extern "C" void strcpy3(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void cMCOverlayManager_getFileName_r(void* self, int mode, void* buf, int size) __asm__("cMCOverlayManager_getFileName");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void* strcpy1(void* dst, void* src);
int GetHashValue32(char* s);
extern char str_pct_spct_d_b[];
extern char str_kT_OVGameOptions[];


/**
 * Writes the title of file slot @p n, as a 16-bit string, into @p dst.
 *
 * For replays (kMCFileType_Replay) and profiles (kMCFileType_Profile) prints "<type name> <n + 1>" (str_pct_spct_d_b
 * widened, the type name from cMCOverlayManager_getFileName); for settings (kMCFileType_Settings) copies the localised
 * "kT_OVGameOptions" text (FELocale slot 4 with its hash).
 *
 * @param n  the file slot
 * @param dst  16-bit destination buffer
 */
void cMCOverlayManager::GetTitleString(int n, char* dst)
{
    char name[0x100];
    char fmt[0x200];
    switch (mFileType) {
    case kMCFileType_Replay:
        strcpy3(fmt, str_pct_spct_d_b);
        cMCOverlayManager_getFileName_r(this, mFileType, name, 0x80);
        sprintf1_v(dst, fmt, name, n + 1);
        break;
    case kMCFileType_Settings: {
        cFELocale* loc = SSXApp->FELocale;
        sVEntry<void*(void*, int)>* vt = (sVEntry<void*(void*, int)>*)VPTR_OF(cUILocaleInterface, loc);
        void* thisp = vthis(loc, vt[4]);
        strcpy1(dst, vt[4].fn(thisp, GetHashValue32(str_kT_OVGameOptions)));
        break;
    }
    case kMCFileType_Profile:
        strcpy3(fmt, str_pct_spct_d_b);
        cMCOverlayManager_getFileName_r(this, mFileType, name, 0x80);
        sprintf1_v(dst, fmt, name, n + 1);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_setTitleString);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void* strcpy1(void* dst, void* src);
extern "C" void cMCOverlayManager_GetTitleString(cMCOverlayManager* self, int n, char* dst);
extern "C" void* cBENewPlayerInterface_getPlayerName(void* iface, int id);
int GetHashValue32(char* s);
extern char str_kT_OVGameOptions[];
extern char str_SSX_3_pct_s[];
extern char str_kT_MEMFileNameReplay[];



/**
 * Builds the save-file title for a file slot and hands it to the memory card.
 *
 * The title depends on mFileType: type 0 formats the localized kT_MEMFileNameReplay text with @p n, type 1
 * uses
 * the profile name of mSaveProfileID, others use kT_OVGameOptions, each inside "SSX 3 %s". The result goes to
 * the
 * card object (vtable slot 45) and into mActiveTitleString.
 *
 * @param n  file slot number (On Tour calls it file)
 */
void cMCOverlayManager::setTitleString(int n)
{
    char out[0x200];
    char title[0x200];
    char fmt[0x200];
    char name[0x200];
    GetTitleString(n, title);
    strcpy3(fmt, str_SSX_3_pct_s);
    switch (mFileType) {
    case kMCFileType_Replay: {
        cFELocale* loc = SSXApp->FELocale;
        sVEntry<void*(void*, int)>* vt = (sVEntry<void*(void*, int)>*)VPTR_OF(cUILocaleInterface, loc);
        void* thisp = vthis(loc, vt[4]);
        sprintf1_v(title, fmt, vt[4].fn(thisp, GetHashValue32(str_kT_MEMFileNameReplay)));
        sprintf1_v(out, title, n);
        break;
    }
    case kMCFileType_Profile: {
        void* iface = cBE_getInterface_Fv(cBE_getBE(), 1);
        strcpy3(name, cBENewPlayerInterface_getPlayerName(iface, mSaveProfileID));
        sprintf1_v(out, fmt, name);
        break;
    }
    default: {
        cFELocale* loc = SSXApp->FELocale;
        sVEntry<void*(void*, int)>* vt = (sVEntry<void*(void*, int)>*)VPTR_OF(cUILocaleInterface, loc);
        void* thisp = vthis(loc, vt[4]);
        strcpy1(title, vt[4].fn(thisp, GetHashValue32(str_kT_OVGameOptions)));
        sprintf1_v(out, fmt, title);
        break;
    }
    }
    char* memCard = (char*)mpMemCard;
    sVEntry<void(void*, char*)>* mvt = *(sVEntry<void(void*, char*)>**)memCard;
    mvt[45].fn(memCard + mvt[45].delta, out);
    strcpy1((char*)&mActiveTitleString, out);
}
#endif

INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_SetState);

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_SetAction);
#ifdef SKIP_ASM
/**
 * Switches the memory-card manager to another action, starting the display timers for kMCOVAction_DeviceSelect.
 *
 * Stores @p mode in mAction, which cMCOverlayManager_update() switches on. kMCOVAction_DeviceSelect also sets mFlash to
 * 1.0, mFlashStep to 1/60 and mDisplayWaitTicks to 60; any other value only stores mAction.
 *
 * @param mode  new action (tMCOverlayManagerAction): kMCOVAction_LoadSave runs the load/save state machine,
 *              kMCOVAction_DeviceSelect the initial card and file checks
 * @return nothing meaningful; the body has no return statement
 */
void* cMCOverlayManager::SetAction(int mode)
{
    mAction = mode;
    switch (mode) {
    case kMCOVAction_LoadSave:
        break;
    case kMCOVAction_DeviceSelect:
        mFlash = 1.0f;
        mFlashStep = 0.016669999808073044f;
        mDisplayWaitTicks = 0x3C;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ManagerGainState);
#ifdef SKIP_ASM
extern "C" void* cMemCard_CreateInstance(void);

/**
 * Creates the manager's memory card (cMemCard_CreateInstance into mpMemCard), zeroes mStackDepth and sets mFlash to 1.0
 * and mFlashStep to 1/60.
 */
void cMCOverlayManager::ManagerGainState()
{
    void* memCard = cMemCard_CreateInstance();
//START
    mpMemCard = (cMemCard*)memCard;
    mStackDepth = 0;
    mFlash = 1.0f;
    mFlashStep = 0.016669999808073044f;
//END
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ManagerLoseState);
#ifdef SKIP_ASM
extern "C" void cMemCard_DeleteInstance(void*);

/**
 * Releases the memory card object when the memory card screen goes away.
 *
 * Deletes +0x434 with cMemCard_DeleteInstance (On Tour cMemCard::DeleteInstance) and clears it.
 */
void cMCOverlayManager::ManagerLoseState()
{
    cMemCard_DeleteInstance(mpMemCard);
    mpMemCard = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_update);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_ManagerLoadSaveUpdate(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_managerDeviceSelectUpdate(cMCOverlayManager* self);

/**
 * Runs one update of the memory-card manager for its current action.
 *
 * kMCOVAction_LoadSave calls cMCOverlayManager_ManagerLoadSaveUpdate(); kMCOVAction_DeviceSelect calls
 * cMCOverlayManager_managerDeviceSelectUpdate(), which drives the initial card and file checks (UpdateInitCardsCheck,
 * UpdateInitFileCheck, UpdateInitFileCheckDevice) and can switch back to kMCOVAction_LoadSave. Other actions do
 * nothing.
 */
void cMCOverlayManager::update()
{
    switch (mAction) {
    case kMCOVAction_LoadSave:
        ManagerLoadSaveUpdate();
        break;
    case kMCOVAction_DeviceSelect:
        managerDeviceSelectUpdate();
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_managerDeviceSelectUpdate);
#ifdef SKIP_ASM
extern "C" int cMCOverlayManager_getMaxDevicesSupported(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitCardsCheck(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFileCheckDevice(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFileCheck(cMCOverlayManager* self);


static inline bool isState_23D660(char* s, int k)
{
    return *(int*)(s + 0x130) == k;
}

/**
 * Runs the manager's device-select action for one frame (cMCOverlayManager_update calls it for
 * kMCOVAction_DeviceSelect).
 *
 * Updates the card (mpMemCard slot 3); while mCheckCard is set it waits for the card to go idle (slot 11) and then
 * clears it. Fills mMaxDevices on first use (cMCOverlayManager_getMaxDevicesSupported). Once the card is idle it
 * switches back to the load/save action (kMCOVAction_LoadSave) and, unless mState is one of the init checks
 * (kMCOverlay_InitCardsCheck .. kMCOverlay_InitFileCheckDone, kMCOverlay_InitFileCheckDevice), sets
 * kMCOverlay_CheckCards through vtable slot 1 (cMCOverlayManager_SetState). Then runs the init state handlers:
 * cMCOverlayManager_UpdateInitCardsCheck, cMCOverlayManager_UpdateInitFileCheck or
 * cMCOverlayManager_UpdateInitFileCheckDevice, for the states of the same names.
 */
void cMCOverlayManager::managerDeviceSelectUpdate()
{
    cMemCard* memCard = mpMemCard;
    memCard->Update();
    if (mCheckCard != 0) {
        memCard = mpMemCard;
        if (memCard->IsBusy() != 0) {
            return;
        }
        mCheckCard = 0;
    }
    if (mMaxDevices == kMaxDevicesUnknown) {
        mMaxDevices = getMaxDevicesSupported();
    }
    memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        SetAction(kMCOVAction_LoadSave);
        int ok = !(isState_23D660(((char*)this), kMCOverlay_InitCardsCheck)
                   || isState_23D660(((char*)this), kMCOverlay_InitCardsCheckDone)
                   || isState_23D660(((char*)this), kMCOverlay_InitFileCheck)
                   || isState_23D660(((char*)this), kMCOverlay_InitFileCheckDone)
                   || isState_23D660(((char*)this), kMCOverlay_InitFileCheckDevice));
        if (ok) {
            SetState(kMCOverlay_CheckCards);
        }
    }
    int st = mState;
    if (st == kMCOverlay_InitCardsCheck) {
        UpdateInitCardsCheck();
    } else if (st == kMCOverlay_InitFileCheck) {
        UpdateInitFileCheck();
    } else if (st == kMCOverlay_InitFileCheckDevice) {
        UpdateInitFileCheckDevice();
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_isLoadCardStatusErrorUnacknowledged);
#ifdef SKIP_ASM
/**
 * Tells whether a card status error that matters to a load is unacknowledged: mCurrDeviceStateAck is clear and
 * mCurrDeviceStateLast has a bit set other than "full" (4) and "unformatted" (0x10).
 *
 * @return true for an unacknowledged error
 */
bool cMCOverlayManager::isLoadCardStatusErrorUnacknowledged()
{
    int deviceState = mCurrDeviceStateLast;
    deviceState &= ~4;
    deviceState &= ~0x10;
    return mCurrDeviceStateAck == 0 && deviceState != 0;
}
#endif

extern "C" void cMCOverlayManager_OnHighlightNextDevice(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateCardDamaged(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateFormatConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitDeleteConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadCorrupt(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadDoneWait(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateNoCardsError(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateNoFreeBlocks(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSave(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveClosing(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveCorrupt(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveTooBig(cMCOverlayManager* self);

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ManagerLoadSaveUpdate);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
// FAKEMATCH: an empty ASSERT macro (`do { } while (0)`) in the isState inline, perhaps EA's compiled-out assert but unconfirmed; plain form 99.28%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.
// g++ 2.95 vtable slot: this-pointer adjustment, index, function (F is the slot's function type).



// PORT: the unit declares cMCOverlayManager_GatherDeviceInfo1 as returning void*, but it returns nothing; bind the void body by asm label.
extern "C" void cMCOverlayManager_GatherDeviceInfo1_v(void* self, int i) __asm__("cMCOverlayManager_GatherDeviceInfo1");
extern "C" void MEM_fill(void* p, int c, int n);
extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_checkDevice(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_UpdateInitFileCheckWait(cMCOverlayManager* self);
void cMCOverlayManager_UpdateInitCardsCheckDone(void* self);
void cMCOverlayManager_UpdateInitFileCheckDone(void* self);
extern "C" void cMCOverlayManager_UpdateCheckCardsWait(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateCheckCards(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateNoCardsError(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateCardInvalid(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateReadingCard(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateFormatConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateFormatProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateFormatFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveCorrupt(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveDone(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveTooBig(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateNoFreeBlocks(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdatePreSaveFile(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdatePreSaveFileDelete(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateCreateFile(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveFile(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSave(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateSaveClosing(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadDoneWait(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadDone(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadCorrupt(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadStart(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoad(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadClosing(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateLoadFileNotFound(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteDoneWait(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteDone(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateDeleteFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateOverwriteConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFormatConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFormatProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFormatFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitFormatDone(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitDeleteConfirm(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitDeleteProgress(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitDeleteFailed(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateInitDeleteDone(cMCOverlayManager* self);
void* cMCOverlayManager_UpdateCardUnusable(void* self);
extern "C" void cMCOverlayManager_UpdateCardDamaged(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateFileExists(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_UpdateCardChanged(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_OnHighlightNextDevice(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_OnHighlightPrevDevice(cMCOverlayManager* self);

// The player's decoder at 0x434, called through its vtable slots by hand (re-read for every call).

static inline int isBusy_7D8(cMCOverlayManager* s) { return s->mpMemCard->IsBusy(); }
// Release-build assert: compiles to an empty do { } while (0) (its loop notes still shape the code).
#define ASSERT_7D8(cond) do { } while (0)

static inline bool isState_7D8(cMCOverlayManager* s, int k)
{
    ASSERT_7D8(k > 0);
    return s->mState == k;
}
static inline bool isActive_7D8(cMCOverlayManager* s)
{
    return !(isState_7D8(s, 0x14) || isState_7D8(s, 0x24) || isState_7D8(s, 0x25) || isState_7D8(s, 0x1C) || isState_7D8(s, 0xC));
}
static inline bool canLoad_7D8(cMCOverlayManager* s)
{
    return !(isState_7D8(s, 0x14) || isState_7D8(s, 0x24) || isState_7D8(s, 0x25) || isState_7D8(s, 0x1C)
        || isState_7D8(s, 0xC) || isState_7D8(s, 0x31) || isState_7D8(s, 0x32) || isState_7D8(s, 0x33)
        || isState_7D8(s, 0x34) || isState_7D8(s, 0x35) || isState_7D8(s, 0x36) || s->mLastCheckDevice < 0);
}
// Decoder capability tests, written to the player's flag words.
static inline void canPlay_7D8(cMCOverlayManager* s, int& out)
{
    out = s->mpMemCard->IsAvailable1() && !s->mpMemCard->IsFileSlotBroken(s->mFileCursor) && !s->mpMemCard->IsFileSlotDisabled(s->mFileCursor);
}
static inline void canSkip_7D8(cMCOverlayManager* s, int& out)
{
    out = s->mpMemCard->IsFormatted() && s->mpMemCard->IsEncodingCorrect1() && !s->mpMemCard->IsDamaged1() && !s->mpMemCard->IsWrongDevice()
        && s->mpMemCard->HasFileName(s->mFileCursor) && !s->mpMemCard->IsFileSlotBroken(s->mFileCursor) && !s->mpMemCard->IsFileSlotDisabled(s->mFileCursor);
}

/**
 * Runs one frame of the memory-card manager: polls the devices, then runs the handler for the current state.
 *
 * Recomputes the save/load/delete/cancel permissions, notices card changes, regathers device info for the
 * device
 * being polled, handles next/previous-device input in the Save (7) and Load (29) states, and every few frames
 * picks
 * the next device to check. The state numbers dispatched here match On Tour's tMCOverlayManagerState names.
 */
extern "C" void cMCOverlayManager_ManagerLoadSaveUpdate(cMCOverlayManager* self)
{
    if (self->mState == 1)
        return;
    self->mSaveOk = 0;
    self->mLoadOk = 0;
    self->mDeleteOk = 0;
    self->mCancelOk = 0;
    self->mpMemCard->Update();
    if (isBusy_7D8(self) || isActive_7D8(self)) {
        int st = self->mState;
        if (st == 7 || st == 29) {
            self->mCancelOk = 1;
            canPlay_7D8(self, self->mSaveOk);
            canSkip_7D8(self, self->mLoadOk);
            if (self->mState == 29)
                self->mDeleteOk = 0;
            else
                self->mDeleteOk = self->mpMemCard->HasFileName(self->mFileCursor);
        }
        if (self->mpMemCard->CardChanged()) {
            self->mCardChanged = 1;
            self->mpMemCard->ClearCardChanged();
        }
    }
    if (!isBusy_7D8(self)) {
        if (canLoad_7D8(self)) {
            cMCOverlayManager* mgr = self;
            int idx = self->mLastCheckDevice;
            if (((&mgr->mDevices[0]))[idx].bPluggedIn == 0) {
                if (self->mpMemCard->IsAvailable(idx)) {
                    if (self->mLastCheckDevice == self->mSelectedDevice) {
                        self->SetState(49);
                    } else {
                        cMCOverlayManager_GatherDeviceInfo1_v(self, self->mLastCheckDevice);
                    }
                } else {
                    MEM_fill(&((&mgr->mDevices[0]))[self->mLastCheckDevice], 0, 0xE0);
                    if (self->mSelectedDevice == self->mLastCheckDevice && !(self->callbackIsDeviceSelected)()
                        && !(self->callbackIsSavePressed)() && !(self->callbackIsLoadPressed)() && !(self->callbackIsDeletePressed)()
                        && !(self->callbackIsFormatPressed)() && self->mSave == 0 && self->mDelete == 0
                        && !(self->callbackIsLoadPressed)())
                        cMCOverlayManager_OnHighlightNextDevice(self);
                }
            } else {
                if (!self->mpMemCard->IsAvailable(idx))
                    MEM_fill(&((&mgr->mDevices[0]))[self->mLastCheckDevice], 0, 0xE0);
                else
                    cMCOverlayManager_GatherDeviceInfo1_v(self, self->mLastCheckDevice);
            }
        }
    }
    if (!isBusy_7D8(self)) {
        if (self->mCheckCard != 0) {
            self->mCheckCard = 0;
            self->mCheckDeviceTicks = 5;
        }
        int st = self->mState;
        if (st == 7 || st == 29) {
            if ((self->callbackIsNextDevicePressed)()) {
                int old = self->mSelectedDevice;
                cMCOverlayManager_OnHighlightNextDevice(self);
                if (old != self->mSelectedDevice)
                    cMCOverlayManager_InitiateFileCheck(self);
            } else if ((self->callbackIsPrevDevicePressed)()) {
                int old = self->mSelectedDevice;
                cMCOverlayManager_OnHighlightPrevDevice(self);
                if (old != self->mSelectedDevice)
                    cMCOverlayManager_InitiateFileCheck(self);
            }
        }
    }
    int go = 0;
    int st = self->mState;
    if (st == 7 || st == 22 || st == 20 || st == 29 || st == 4 || st == 23 || st == 0x18 || st == 9 || st == 10) {
        (self->mCheckDeviceTicks)--;
        if (self->mCheckCard == 0 && self->mCheckDeviceTicks <= 0)
            go = !isBusy_7D8(self);
    }
    if (go) {
        bool changed = false;
        for (int i = 0; i < cMCOverlayManager_getMaxDevicesSupported(self); i++) {
            if (self->mpMemCard->IsDeviceChanged(i)) {
                if (self->mLastCheckDevice < i || self->mLastCheckDevice == cMCOverlayManager_getMaxDevicesSupported(self) - 1) {
                    self->mCurrCheckDevice = i;
                    self->mLastCheckDevice = i;
                    if (i != self->mSelectedDevice) changed = true; else changed = false;
                    self->mpMemCard->CheckDevice(i);
                    self->mCheckCard = 1;
                    self->mpMemCard->ClearDeviceChanges(self->mCurrCheckDevice);
                    break;
                }
            }
        }
        if (changed)
            cMCOverlayManager_checkDevice(self, self->mSelectedDevice);
    }
    switch (self->mState) {
    case 49:
        cMCOverlayManager_UpdateInitCardsCheck(self);
        break;
    case 50:
        cMCOverlayManager_UpdateInitCardsCheckDone(self);
        break;
    case 53:
        cMCOverlayManager_UpdateInitFileCheckWait(self);
        break;
    case 54:
        cMCOverlayManager_UpdateInitFileCheckDevice(self);
        break;
    case 51:
        cMCOverlayManager_UpdateInitFileCheck(self);
        break;
    case 52:
        cMCOverlayManager_UpdateInitFileCheckDone(self);
        break;
    case 2:
        cMCOverlayManager_UpdateCheckCardsWait(self);
        break;
    case 3:
        cMCOverlayManager_UpdateCheckCards(self);
        break;
    case 4:
        cMCOverlayManager_UpdateNoCardsError(self);
        break;
    case 5:
        cMCOverlayManager_UpdateCardInvalid(self);
        break;
    case 6:
        cMCOverlayManager_UpdateReadingCard(self);
        break;
    case 10:
        cMCOverlayManager_UpdateFormatConfirm(self);
        break;
    case 11:
        cMCOverlayManager_UpdateFormatProgress(self);
        break;
    case 12:
        cMCOverlayManager_UpdateFormatFailed(self);
        break;
    case 22:
        cMCOverlayManager_UpdateSaveCorrupt(self);
        break;
    case 19:
        cMCOverlayManager_UpdateSaveDone(self);
        break;
    case 20:
        cMCOverlayManager_UpdateSaveFailed(self);
        break;
    case 21:
        cMCOverlayManager_UpdateSaveTooBig(self);
        break;
    case 23:
        cMCOverlayManager_UpdateNoFreeBlocks(self);
        break;
    case 13:
        cMCOverlayManager_UpdatePreSaveFile(self);
        break;
    case 14:
        cMCOverlayManager_UpdatePreSaveFileDelete(self);
        break;
    case 15:
        cMCOverlayManager_UpdateCreateFile(self);
        break;
    case 16:
        cMCOverlayManager_UpdateSaveFile(self);
        break;
    case 17:
        cMCOverlayManager_UpdateSaveProgress(self);
        break;
    case 7:
        cMCOverlayManager_UpdateSave(self);
        break;
    case 18:
        cMCOverlayManager_UpdateSaveClosing(self);
        break;
    case 34:
        cMCOverlayManager_UpdateLoadDoneWait(self);
        break;
    case 35:
        cMCOverlayManager_UpdateLoadDone(self);
        break;
    case 36:
        cMCOverlayManager_UpdateLoadFailed(self);
        break;
    case 37:
        cMCOverlayManager_UpdateLoadCorrupt(self);
        break;
    case 30:
        cMCOverlayManager_UpdateLoadConfirm(self);
        break;
    case 31:
        cMCOverlayManager_UpdateLoadStart(self);
        break;
    case 32:
        cMCOverlayManager_UpdateLoadProgress(self);
        break;
    case 29:
        cMCOverlayManager_UpdateLoad(self);
        break;
    case 33:
        cMCOverlayManager_UpdateLoadClosing(self);
        break;
    case 38:
        cMCOverlayManager_UpdateLoadFileNotFound(self);
        break;
    case 24:
        cMCOverlayManager_UpdateDeleteConfirm(self);
        break;
    case 25:
        cMCOverlayManager_UpdateDeleteProgress(self);
        break;
    case 26:
        cMCOverlayManager_UpdateDeleteDoneWait(self);
        break;
    case 27:
        cMCOverlayManager_UpdateDeleteDone(self);
        break;
    case 28:
        cMCOverlayManager_UpdateDeleteFailed(self);
        break;
    case 9:
        cMCOverlayManager_UpdateOverwriteConfirm(self);
        break;
    case 39:
        cMCOverlayManager_UpdateInitFormatConfirm(self);
        break;
    case 40:
        cMCOverlayManager_UpdateInitFormatProgress(self);
        break;
    case 41:
        cMCOverlayManager_UpdateInitFormatFailed(self);
        break;
    case 42:
        cMCOverlayManager_UpdateInitFormatDone(self);
        break;
    case 43:
        cMCOverlayManager_UpdateInitDeleteConfirm(self);
        break;
    case 44:
        cMCOverlayManager_UpdateInitDeleteProgress(self);
        break;
    case 45:
        cMCOverlayManager_UpdateInitDeleteFailed(self);
        break;
    case 46:
        cMCOverlayManager_UpdateInitDeleteDone(self);
        break;
    case 47:
        cMCOverlayManager_UpdateCardUnusable(self);
        break;
    case 48:
        cMCOverlayManager_UpdateCardDamaged(self);
        break;
    case 55:
        cMCOverlayManager_UpdateFileExists(self);
        break;
    case 56:
        cMCOverlayManager_UpdateCardChanged(self);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitCardsCheck);
#ifdef SKIP_ASM


/**
 * Waits for the initial card check to finish, then moves to InitCardsCheckDone (state 50).
 *
 * The card is idle once its vtable slot 11 (busy) returns 0; the state change goes through the virtual
 * SetState
 * (vtable slot 1).
 */
void cMCOverlayManager::UpdateInitCardsCheck()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        SetState(50);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitCardsCheckDone__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the memory-card overlay's init-cards-check-done update has an empty body.
 */
void cMCOverlayManager::UpdateInitCardsCheckDone()
{
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFileCheckWait);
#ifdef SKIP_ASM
/**
 * Waits for the card to go idle, then moves to InitFileCheck (state 51).
 */
void cMCOverlayManager::UpdateInitFileCheckWait()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        SetState(51);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFileCheckDevice);
#ifdef SKIP_ASM


extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self);

/**
 * Once the card is idle, starts the file check on the selected device or reports that no card is present.
 *
 * If the card says the selected device is not present (card slot 52), moves to CheckCardsWait (2) and calls
 * callbackDisplayNoCards; otherwise starts InitiateFileCheck() and calls callbackDisplayNoFiles.
 */
void cMCOverlayManager::UpdateInitFileCheckDevice()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        cMemCard* card = mpMemCard;
        if (card->IsAvailable(mSelectedDevice) == 0) {
            SetState(2);
            (callbackDisplayNoCards)();
        } else {
            InitiateFileCheck();
            (callbackDisplayNoFiles)();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateFileExists);
#ifdef SKIP_ASM
/**
 * Waits on the "file exists" message: once the card is idle, cancelling returns to CheckCards (state 3).
 */
void cMCOverlayManager::UpdateFileExists()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsCancelPressed)() != 0) {
            SetState(3);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCardChanged);
#ifdef SKIP_ASM

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self);

/**
 * Waits on the "card changed" message: once the card is idle, cancelling aborts the operation and rechecks
 * the card.
 *
 * Calls CancelCardOp(0), clears mCardChanged and calls RecheckCard().
 */
void cMCOverlayManager::UpdateCardChanged()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(0);
            mCardChanged = 0;
            RecheckCard();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFileCheck);
#ifdef SKIP_ASM


/**
 * Waits for the initial file check to finish, then moves to InitFileCheckDone (state 52).
 */
void cMCOverlayManager::UpdateInitFileCheck()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        SetState(52);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFileCheckDone__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing: the memory-card overlay's init-file-check-done update has an empty body.
 */
void cMCOverlayManager::UpdateInitFileCheckDone()
{
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCheckCardsWait);
#ifdef SKIP_ASM
/**
 * Waits out the memory-card check display delay, then moves on with state code 3.
 *
 * Decrements mDisplayWaitTicks (SetAction(1) sets it to 60); once it was already 0 or less, passes code 3 to
 * the manager's virtual state handler (vtable slot 1, cMCOverlayManager_SetState).
 */
void cMCOverlayManager::UpdateCheckCardsWait()
{
    if ((mDisplayWaitTicks)-- <= 0) {
        SetState(3);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCheckCards);
#ifdef SKIP_ASM
extern "C" int cMCOverlayManager_updateDeviceStateMonitor(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_TriggerPollNoCardError(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self);

#define S_E540 ((char*)self)

/**
 * Polls the memory cards while checking them and passes the next state code to the virtual state handler.
 *
 * Waits while the card object is busy (its virtual slot 11). A pending save, delete or load press means the
 * user wants to leave: a changed card with mShowCardChangeNotify set gives 0x38. On a device state change it
 * raises the no-card error (TriggerPollNoCardError) when polling for it, posts 5 (state bit 1) or 0x27
 * (unformatted, when mPollForUnformatted), or starts a file check when a card arrives or its state clears.
 * Without a pending action it rechecks a changed card or posts 0x31 for no card. Otherwise it posts 6 when the
 * card object's slots 55 and 58 report true and 51 and 59 false, else 7 when saving or 0x1D when not.
 */
void cMCOverlayManager::UpdateCheckCards()
{
    if (mpMemCard->IsBusy())
        return;
    int quit = 0;
    if (mSave || mDelete || (callbackIsLoadPressed)())
        quit = 1;
    if (mShowCardChangeNotify && mCardChanged && quit) {
        SetState(0x38);
        return;
    }
    int held = mCurrDeviceStateLast;
    if (updateDeviceStateMonitor()) {
        if ((mPollForNoCard || quit) && (mCurrDeviceStateLast & 8)) {
            TriggerPollNoCardError();
            return;
        }
        if (!(mCurrDeviceStateLast & 8)) {
            if (mCurrDeviceStateLast & 1) {
                SetState(5);
                return;
            }
            if (mPollForUnformatted && (mCurrDeviceStateLast & 0x10)) {
                SetState(39);
                return;
            }
        }
        int rel;
        if ((!(mCurrDeviceStateLast & 8) && (held & 8)) || (rel = held & ~4, mCurrDeviceStateLast == 0 && rel)) {
            InitiateFileCheck();
            return;
        }
    }
    if (!quit) {
        if (mCardChanged) {
            mCardChanged = 0;
            RecheckCard();
            return;
        }
        if (mCurrDeviceStateLast & 8) {
            SetState(49);
            return;
        }
    }
    if (mSave) {
        if (mpMemCard->IsFormatted() && mpMemCard->IsEncodingCorrect1() && !mpMemCard->IsDamaged1() && !mpMemCard->IsWrongDevice())
            SetState(6);
        else
            SetState(7);
    } else {
        if (mpMemCard->IsFormatted() && mpMemCard->IsEncodingCorrect1() && !mpMemCard->IsDamaged1() && !mpMemCard->IsWrongDevice())
            SetState(6);
        else
            SetState(29);
    }
}
#undef S_E540
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateNoCardsError);
#ifdef SKIP_ASM

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "no memory card" error: once the card is idle, cancel aborts the operation and retry retries
 * it.
 */
void cMCOverlayManager::UpdateNoCardsError()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(0);
        } else if ((callbackIsRetryPressed)() != 0) {
            RetryCardOp();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_CheckForCard);
#ifdef SKIP_ASM
/**
 * Checks the selected card and moves the manager to the matching state through vtable slot 1
 * (cMCOverlayManager_SetState).
 *
 * Uses mpMemCard's status slots (by On Tour's cMemCard order: 53 IsAvailable, 49 IsInvalid, 51 IsDamaged, 59
 * IsWrongDevice, 55 IsFormatted, 58 IsEncodingCorrect). With no usable card: kMCOverlay_CardInvalid when it is
 * invalid, damaged or the wrong device, else kMCOverlay_NoCardsError unless already there. With a formatted,
 * correctly encoded, undamaged card: kMCOverlay_ReadingCard when @p flag is set. Otherwise (or when not)
 * kMCOverlay_Save for a save (mSave) or kMCOverlay_Load for a load.
 *
 * @param flag  nonzero to go to kMCOverlay_ReadingCard when the card is good
 */
void cMCOverlayManager::CheckForCard(int flag)
{
    cMCOverlayManager* manager = (cMCOverlayManager*)((char*)this);

    if (manager->mpMemCard->IsAvailable1() == 0) {
        if (manager->mpMemCard->IsInvalid1() != 0 || manager->mpMemCard->IsDamaged1() != 0 || manager->mpMemCard->IsWrongDevice() != 0) {
            manager->SetState(kMCOverlay_CardInvalid);
        } else {
            int ok = ((cMCOverlayManager*)manager)->mState != kMCOverlay_NoCardsError;
            if (ok) {
                manager->SetState(kMCOverlay_NoCardsError);
            }
        }
    } else {
        if (manager->mpMemCard->IsFormatted() != 0 && manager->mpMemCard->IsEncodingCorrect1() != 0 && manager->mpMemCard->IsDamaged1() == 0 && manager->mpMemCard->IsWrongDevice() == 0) {
            if (flag != 0) {
                manager->SetState(kMCOverlay_ReadingCard);
            }
        } else if (((cMCOverlayManager*)manager)->mSave != 0) {
            manager->SetState(kMCOverlay_Save);
        } else {
            manager->SetState(kMCOverlay_Load);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_CancelCardOp);
#ifdef SKIP_ASM
extern int cMCOverlayManager_mSaveInProgress;
extern "C" void cMCOverlayManager_WriteFailed(cMCOverlayManager* self, int a1);

/**
 * Abandons the current memory-card operation and returns the manager to Done (state 1).
 *
 * A save in progress is reported as failed through WriteFailed(); also clears the global save-in-progress
 * flag,
 * mDelete and the buffered format press, and marks the device state as acknowledged.
 *
 * @param showFailMessage  passed to WriteFailed() when a save is being abandoned: show the write-failed popup
 */
void cMCOverlayManager::CancelCardOp(int showFailMessage)
{
    int on = mSave;
    cMCOverlayManager_mSaveInProgress = 0;
    if (on != 0) {
        mSave = 0;
        WriteFailed(showFailMessage);
    }
    mDelete = 0;
    mCurrDeviceStateAck = 1;
    mFormatPressedBuffered = 0;
    SetState(1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_RetryCardOp);
#ifdef SKIP_ASM
extern "C" bool cMCOverlayManager_hasCardStatusCriticalError(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_setSelectedDevice(cMCOverlayManager* self);
void cMCOverlayManager_selectDevice(void* self, int val);
extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self);
extern int cMCOverlayManager_mSaveInProgress;

/**
 * Retries the operation that failed: a save restarts from CheckCardsWait, anything else rechecks the card.
 *
 * For a save, an unfinished card write is first stopped (card vtable slot 44). Otherwise, with mRetryToInit
 * set it
 * reselects device 0 and returns to InitCardsCheck (49); else it restarts the file check when the device
 * state
 * shows no problem besides a full card (cMCOverlayManager_hasCardStatusCriticalError()), or rechecks the card.
 */
void cMCOverlayManager::RetryCardOp()
{
    if (mSave != 0) {
        int playing = mWriteInProgress;
        cMCOverlayManager_mSaveInProgress = 0;
        if (playing != 0) {
            cMemCard* memCard = mpMemCard;
            memCard->SetWriteFailed();
        }
        SetState(2);
    } else {
        mCurrDeviceStateAck = 0;
        if (mRetryToInit != 0) {
            selectDevice(0);
            setSelectedDevice();
            SetState(49);
        } else if (hasCardStatusCriticalError() == 0) {
            InitiateFileCheck();
        } else {
            RecheckCard();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_TriggerPollNoCardError);
#ifdef SKIP_ASM
/**
 * Reports a missing memory card by passing code 4 to the manager's virtual state handler (vtable slot 1).
 */
void cMCOverlayManager::TriggerPollNoCardError()
{
    SetState(4);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCardInvalid);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "invalid card" error: once the card is idle, cancel aborts (with the fail message) and retry
 * retries.
 */
void cMCOverlayManager::UpdateCardInvalid()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(1);
        } else if ((callbackIsRetryPressed)() != 0) {
            RetryCardOp();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateReadingCard);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_TriggerPollNoCardError(cMCOverlayManager* self);
extern "C" int cMCOverlayManager_updateDeviceStateMonitor(cMCOverlayManager* self);
// PORT: the target calls cMCOverlayManager_GatherDeviceInfo as a void function here (the unit defines it returning void*).
void cMCOverlayManager_GatherDeviceInfo_v(void* self) __asm__("cMCOverlayManager_GatherDeviceInfo__FPv");

/**
 * Waits while the card is being read, then picks the next state from the device status and the pending
 * operation.
 *
 * After the display delay and once the card is idle: a card change goes to CardChanged (56) or back to
 * ReadingCard; a save goes to SaveFailed (20), SaveTooBig (21) or Save (7); a delete or load goes to the
 * no-card
 * error, LoadFailed (36), CardInvalid (5) or Load (29). Device status bits come from mCurrDeviceStateLast
 * (1 error, 8 no card).
 */
void cMCOverlayManager::UpdateReadingCard()
{
    int ready = updateDeviceStateMonitor();
    if ((mDisplayWaitTicks)-- > 0)
        return;
    if (mpMemCard->IsBusy())
        return;
    if (mCardChanged) {
        int busy = 0;
        if (mSave || mDelete || (callbackIsLoadPressed)())
            busy = 1;
        if (mShowCardChangeNotify && busy) {
            SetState(0x38);
            return;
        }
        if (mpMemCard->IsFormatted() && mpMemCard->IsEncodingCorrect1() && !mpMemCard->IsDamaged1() && !mpMemCard->IsWrongDevice()) {
            SetState(6);
            return;
        }
    }
    if (mSave) {
        int deviceState = mCurrDeviceStateLast;
        if (!(deviceState & 8)) {
            if (deviceState & 1) {
                SetState(20);
                return;
            }
            if (mpMemCard->IsFormatted() && mpMemCard->IsEncodingCorrect1() && !mpMemCard->IsDamaged1() && !mpMemCard->IsWrongDevice() && !mpMemCard->GetNumFiles()) {
                int neededSpace = mpMemCard->SpaceRequired1();
                if (mpMemCard->GetFreeSpace() < neededSpace || !mpMemCard->GetFreeDirEntries()) {
                    SetState(21);
                    return;
                }
            }
        } else if (ready) {
            TriggerPollNoCardError();
            return;
        }
        cMCOverlayManager_GatherDeviceInfo_v(this);
        SetState(7);
        return;
    }
    {
        int busy = 0;
        if (mDelete || (callbackIsLoadPressed)())
            busy = 1;
        if (mPollForNoCard || busy) {
            if (mCurrDeviceStateLast & 8) {
                TriggerPollNoCardError();
                return;
            }
        } else if (mCurrDeviceStateLast & 8) {
            goto fail;
        }
        if (mCurrDeviceStateLast & 1) {
            if ((callbackIsLoadPressed)())
                SetState(36);
            else
                SetState(5);
            return;
        }
    fail:
        cMCOverlayManager_GatherDeviceInfo_v(this);
        SetState(29);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateFormatConfirm);
#ifdef SKIP_ASM
class cMoviePlayerVt {
public:
    char pad[0x748];
    virtual void SetState(int);
};

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);

/**
 * Handles the format-card confirmation: starts the format once confirmed, or reports a missing or failed
 * card.
 *
 * When no recheck is pending: a missing card goes to NoCardsError (4), a card error to FormatFailed (12), and
 * a
 * buffered format press to FormatProgress (11). Cancel aborts; the format button is buffered in
 * mFormatPressedBuffered for the next frame.
 */
void cMCOverlayManager::UpdateFormatConfirm()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    if (mCheckCard == 0) {
        if (mpMemCard->IsAvailable1() == 0) {
            me->SetState(4);
            return;
        }
        if (mpMemCard->DidFormatFail() != 0) {
            me->SetState(0xC);
            return;
        }
        if (mFormatPressedBuffered != 0) {
            mFormatPressedBuffered = 0;
            me->SetState(11);
            return;
        }
    }
    if ((callbackIsCancelPressed)() != 0) {
        CancelCardOp(0);
        return;
    }
    if ((callbackIsFormatPressed)() != 0) {
        mFormatPressedBuffered = 1;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateFormatProgress);
#ifdef SKIP_ASM
/**
 * Waits for a card format to finish, then moves to FormatFailed (12) on error or PreSaveFile (13) on success.
 */
void cMCOverlayManager::UpdateFormatProgress()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    int waitTicks = (mDisplayWaitTicks)--;
    if (waitTicks <= 0) {
        if (mpMemCard->IsBusy() == 0) {
            if (mpMemCard->DidFormatFail() != 0) {
                me->SetState(0xC);
            } else {
                me->SetState(13);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateFormatFailed);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_checkDevice(cMCOverlayManager* self, int v);

/**
 * Handles the format-failed card state: once cancel is pressed, abandons the operation and rechecks the card.
 *
 * When callbackIsCancelPressed() returns nonzero: cMCOverlayManager_CancelCardOp(self, 0),
 * cMCOverlayManager_checkDevice() on mSelectedDevice, and mCheckCard = 1. Otherwise it keeps waiting.
 */
void cMCOverlayManager::UpdateFormatFailed()
{
    if ((callbackIsCancelPressed)() != 0) {
        CancelCardOp(0);
        checkDevice(mSelectedDevice);
        mCheckCard = 1;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveFailed);
#ifdef SKIP_ASM

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "save failed" message: once the card is idle, cancel aborts and retry restarts the save.
 */
void cMCOverlayManager::UpdateSaveFailed()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(0);
        } else if ((callbackIsRetryPressed)() != 0) {
            mSave = 1;
            RetryCardOp();
            mCurrDeviceStateAck = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveTooBig);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "not enough space" message: once the card is idle, cancel aborts and retry retries the
 * operation.
 */
void cMCOverlayManager::UpdateSaveTooBig()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(0);
        } else if ((callbackIsRetryPressed)() != 0) {
            RetryCardOp();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateNoFreeBlocks);
#ifdef SKIP_ASM

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "no free blocks" message: once the card is idle, retry retries and cancel aborts.
 */
void cMCOverlayManager::UpdateNoFreeBlocks()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsRetryPressed)() != 0) {
            RetryCardOp();
        } else if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveDone);
#ifdef SKIP_ASM
/**
 * Finishes a completed save by passing code 1 to the manager's virtual state handler (vtable slot 1).
 */
void cMCOverlayManager::UpdateSaveDone()
{
    SetState(1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdatePreSaveFile);
#ifdef SKIP_ASM


/**
 * Decides whether a save must first delete the existing file in the chosen slot.
 *
 * If the card reports the slot in use (card slots 67 or 65 for mFileCursor), moves to PreSaveFileDelete (14),
 * otherwise straight to CreateFile (15).
 */
void cMCOverlayManager::UpdatePreSaveFile()
{
    cMemCard* memCard = mpMemCard;
    cMemCard* card;
    sVEntry<int(void*, int)>* vtb;
    if (memCard->HasFileName(mFileCursor) != 0
        || (card = mpMemCard, vtb = *(sVEntry<int(void*, int)>**)card,
            card->IsFileSlotBroken(mFileCursor) != 0)) {
        SetState(0xE);
    } else {
        SetState(0xF);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdatePreSaveFileDelete);
#ifdef SKIP_ASM
/**
 * Waits for the old save file to be deleted, then moves to CreateFile (15), or SaveFailed (20) on a card
 * error.
 */
void cMCOverlayManager::UpdatePreSaveFileDelete()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        cMemCard* card = mpMemCard;
        if (card->DidErrorOccur() != 0) {
            SetState(20);
        } else {
            SetState(0xF);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCreateFile);
#ifdef SKIP_ASM
/**
 * Waits for the save file to be created, then moves to SaveFile (16), or reports the error.
 *
 * A card error goes to FileExists (55) when card slot 74 says so, otherwise to SaveFailed (20).
 */
void cMCOverlayManager::UpdateCreateFile()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    if (mpMemCard->IsBusy() == 0) {
        if (mpMemCard->DidErrorOccur() != 0) {
            if (mpMemCard->FileExistsError() != 0) {
                me->SetState(55);
            } else {
                me->SetState(20);
            }
        } else {
            me->SetState(0x10);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveFile);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_WriteFailed(cMCOverlayManager* self, int a1);

/**
 * Updates the save progress and moves to SaveProgress (17), or fails the write when the card reports an
 * error.
 */
void cMCOverlayManager::UpdateSaveFile()
{
    cMemCard* memCard;
    mProgress = (float)mSavedSize / (float)mSize;
    memCard = mpMemCard;
    if (memCard->DidErrorOccur() != 0) {
        WriteFailed(1);
    } else {
        SetState(17);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveProgress);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_writeClose(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_WriteFailed(cMCOverlayManager* self, int a1);

/**
 * Feeds the save data to the card until it is all written, then finishes the write and moves to SaveClosing
 * (18).
 *
 * Updates mProgress = mSavedSize / mSize; while data remains, callbackWriteUpdate gets the remaining byte
 * count
 * and a 0 result fails the write, as does a card error.
 */
void cMCOverlayManager::UpdateSaveProgress()
{
    mDisplayWaitTicks -= 1;
    mProgress = (float)mSavedSize / (float)mSize;
    if (mpMemCard->DidErrorOccur() != 0) {
        WriteFailed(1);
        return;
    }
    if (mpMemCard->IsWriteWaiting() == true) {
        int savedSize = mSavedSize;
        int totalSize = mSize;
        if (savedSize < totalSize) {
            if ((callbackWriteUpdate)(totalSize - savedSize) == 0) {
                WriteFailed(1);
            }
        } else {
            writeClose();
            (callbackWriteEnd)();
            SetState(18);
        }
    } else {
        if (mpMemCard->DidErrorOccur() != 0) {
            WriteFailed(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSave);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_TriggerPollNoCardError(cMCOverlayManager* self);
extern "C" int cMCOverlayManager_updateDeviceStateMonitor(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self);
extern "C" int USTR_compare(void* a, void* b);

/**
 * Runs the save screen (state Save, 7): reacts to card status changes and starts a save, overwrite or delete.
 *
 * Card problems found by updateDeviceStateMonitor() send it to the no-card error, InitFormatConfirm (39),
 * CardDamaged (48), CardInvalid (5) or NoFreeBlocks (23). Pressing save checks the card is usable and has
 * room,
 * refuses a name already used by another slot (FileExists, 55), and goes to OverwriteConfirm (9) for a used
 * slot
 * or PreSaveFile (13) for an empty one; delete goes to DeleteConfirm (24) and cancel to Done (1).
 */
void cMCOverlayManager::UpdateSave()
{
    if (mCheckCard)
        return;
    if (mShowCardChangeNotify && mCardChanged) {
        SetState(0x38);
        return;
    }
    int ready = updateDeviceStateMonitor();
    if (!(callbackIsSavePressed)() && ready) {
        int deviceState = mCurrDeviceStateLast;
        if (deviceState & 8) {
            TriggerPollNoCardError();
            return;
        }
        if (deviceState & 0x10) {
            SetState(39);
            return;
        }
        if (deviceState & 2) {
            SetState(0x30);
            return;
        }
        if (deviceState & 1) {
            SetState(5);
            return;
        }
        if (deviceState & 4) {
            SetState(23);
            return;
        }
    }
    if (mCardChanged) {
        mCardChanged = 0;
        RecheckCard();
        return;
    }
    if ((callbackIsCancelPressed)()) {
        SetState(1);
        return;
    }
    if ((callbackIsSavePressed)()) {
        mFlash = 1.0f;
        mFlashStep = 0.016669999808073044f;
        if (mSaveOk) {
            if (mpMemCard->IsFormatted() && mpMemCard->IsEncodingCorrect1() && !mpMemCard->IsDamaged1() && !mpMemCard->IsWrongDevice()) {
                if (!mpMemCard->HasFileName(mFileCursor)) {
                    int neededSpace = mpMemCard->SpaceRequired1();
                    if (mpMemCard->GetFreeSpace() < neededSpace || !mpMemCard->GetFreeDirEntries()) {
                        SetState(21);
                        return;
                    }
                }
                for (int i = 0; i < 6; i++) {
                    if (mpMemCard->HasFileName(i)) {
                        unsigned short buf[0x100];
                        mpMemCard->GetTitleString(i, buf);
                        if (USTR_compare(&mNewFileName[0], buf) == 0 && i != mFileCursor) {
                            SetState(55);
                            return;
                        }
                    }
                }
                if (mpMemCard->HasFileName(mFileCursor)) {
                    mpMemCard->GetTitleString(mFileCursor, &mActiveTitleString[0]);
                    SetState(9);
                    return;
                }
                SetState(13);
            } else {
                SetState(10);
                return;
            }
        } else if (mpMemCard->IsFileSlotBroken(mFileCursor)) {
            SetState(22);
        } else if (mpMemCard->IsFileSlotDisabled(mFileCursor)) {
            SetState(23);
        } else if (!mpMemCard->IsAvailable1()) {
            SetState(20);
        }
    }
    if ((callbackIsDeletePressed)() && mDeleteOk) {
        mpMemCard->GetTitleString(mFileCursor, &mActiveTitleString[0]);
        SetState(0x18);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_setSelectedDevice);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_checkDevice(cMCOverlayManager* self, int v);

/**
 * Makes the highlighted memory card device the selected one and checks it.
 *
 * When the device index (+0x428) differs from the last selected one (+0x444), stores it there and clears the
 * card status words (+0x43C, +0x440); then calls cMCOverlayManager_checkDevice for it.
 */
void cMCOverlayManager::setSelectedDevice()
{
    int cur = mSelectedDevice;
    if (mLastMountedDevice != cur) {
        mLastMountedDevice = cur;
        mCurrDeviceStateLast = 0;
        mCurrDeviceStateAck = 0;
    }
    checkDevice(mSelectedDevice);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_selectDevice__FPvi);
#ifdef SKIP_ASM
/**
 * Sets mSelectedDevice to @p val.
 *
 * @param val  the device index
 */
void cMCOverlayManager::selectDevice(int val)
{
    mSelectedDevice = val;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_checkDevice);
#ifdef SKIP_ASM
/**
 * Starts a status check of one memory-card device: records it in mCurrCheckDevice and asks the card to check
 * it.
 *
 * @param iSlot  device (card slot) to check
 */
void cMCOverlayManager::checkDevice(int iSlot)
{
    mCurrCheckDevice = iSlot;
    cMemCard* obj = mpMemCard;
    obj->CheckDevice(iSlot);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ReturnToDevSelectScreen);
#ifdef SKIP_ASM
extern "C" void* cMCOverlayManager_SetAction(cMCOverlayManager* self, int v);

/**
 * Returns the memory-card screens to device selection, dropping any pending save, delete or format.
 *
 * Resets mLastCheckDevice to -1 and mCurrCheckDevice to 0, clears mSave, mDelete and mFormatPressedBuffered,
 * passes code 0 to the virtual state handler (vtable slot 1), then calls cMCOverlayManager_SetAction(self, 1).
 *
 * @return no meaningful value (cMCOverlayManager_SetAction returns nothing)
 */
void* cMCOverlayManager::ReturnToDevSelectScreen()
{
    mLastCheckDevice = -1;
    mCurrCheckDevice = 0;
    mSave = 0;
    mDelete = 0;
    mFormatPressedBuffered = 0;
    SetState(0);
    return SetAction(1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveClosing);
#ifdef SKIP_ASM
/**
 * Waits for the save file to close: on success calls callbackSaveSuccess and moves to SaveDone (19).
 *
 * A card error goes to SaveFailed (20) instead; the display delay (mDisplayWaitTicks) must also have run out.
 */
void cMCOverlayManager::UpdateSaveClosing()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    (mDisplayWaitTicks)--;
    if (mpMemCard->IsBusy() == 0) {
        if (mpMemCard->DidErrorOccur() != 0) {
            me->SetState(20);
            return;
        }
        if (mDisplayWaitTicks <= 0) {
            (callbackSaveSuccess)();
            me->SetState(19);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadFailed);
#ifdef SKIP_ASM
/**
 * Handles the "load failed" message: once the card is idle, cancel goes to CheckCards (3), retry to
 * ReadingCard (6).
 */
void cMCOverlayManager::UpdateLoadFailed()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsCancelPressed)() != 0) {
            SetState(3);
        } else if ((callbackIsRetryPressed)() != 0) {
            SetState(6);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadFileNotFound);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);

/**
 * Handles the load-file-not-found card state: cancels the card operation and notifies the front end.
 *
 * Calls cMCOverlayManager_CancelCardOp(self, 1), then the callbackFileNotFound handler.
 */
void cMCOverlayManager::UpdateLoadFileNotFound()
{
    CancelCardOp(1);
    (callbackFileNotFound)();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateSaveCorrupt);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles a corrupt save file found while saving: offers cancel, retry, delete of the file, or overwrite.
 *
 * Once the card is idle: a missing card goes to NoCardsError (4); delete asks the card to delete the slot's
 * file
 * (mFileCursor) and moves to DeleteProgress (25); overwrite goes to PreSaveFile (13).
 */
void cMCOverlayManager::UpdateSaveCorrupt()
{
    if (mpMemCard->IsBusy() != 0) {
        return;
    }
    if (mCheckCard == 0 && mpMemCard->IsAvailable1() == 0) {
        SetState(4);
        return;
    }
    if ((callbackIsCancelPressed)() != 0) {
        CancelCardOp(0);
        return;
    }
    if ((callbackIsRetryPressed)() != 0) {
        RetryCardOp();
        return;
    }
    if ((callbackIsDeletePressed)() != 0) {
        mpMemCard->GetTitleString(mFileCursor, &mActiveTitleString[0]);
        SetState(25);
        return;
    }
    if ((callbackIsOverwritePressed)() != 0) {
        SetState(13);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadCorrupt);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles a corrupt save file found while loading: cancel aborts with the fail message, retry retries.
 *
 * Otherwise it calls callbackIsDeletePressed and ignores the result.
 */
void cMCOverlayManager::UpdateLoadCorrupt()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(1);
        } else if ((callbackIsRetryPressed)() != 0) {
            RetryCardOp();
        } else {
            ((void (*)())callbackIsDeletePressed)();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadDoneWait);
#ifdef SKIP_ASM
/**
 * Waits out the display delay after a load, then reports the result to the front end.
 *
 * A card error goes to LoadFailed (36) and calls callbackReadFailed; otherwise moves to LoadDone (35) and
 * calls
 * callbackReadSuccess.
 */
void cMCOverlayManager::UpdateLoadDoneWait()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    int n = mDisplayWaitTicks;
    if (n > 0) {
        mDisplayWaitTicks = n - 1;
        return;
    }
    if (mpMemCard->IsBusy() == 0) {
        if (mpMemCard->DidErrorOccur() != 0) {
            me->SetState(36);
            (callbackReadFailed)();
        } else {
            me->SetState(35);
            (callbackReadSuccess)();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadDone);
#ifdef SKIP_ASM
/**
 * Finishes a completed load by passing code 1 to the manager's virtual state handler (vtable slot 1).
 */
void cMCOverlayManager::UpdateLoadDone()
{
    SetState(1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadConfirm);
#ifdef SKIP_ASM
/**
 * Handles the load confirmation: load starts the read (LoadStart, 31) and cancel returns to Load (29).
 *
 * A missing card goes to NoCardsError (4).
 */
void cMCOverlayManager::UpdateLoadConfirm()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    if (mpMemCard->IsBusy() == 0) {
        if (mpMemCard->IsAvailable1() == 0) {
            me->SetState(4);
            return;
        }
        if ((callbackIsLoadPressed)() != 0) {
            me->SetState(0x1F);
            return;
        }
        if ((callbackIsCancelPressed)() != 0) {
            me->SetState(29);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadStart);
#ifdef SKIP_ASM


/**
 * Starts the load: moves to LoadProgress (32), or to LoadFailed (36) with callbackReadFailed on a card error.
 */
void cMCOverlayManager::UpdateLoadStart()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->DidErrorOccur() != 0) {
        SetState(36);
        (callbackReadFailed)();
    } else {
        SetState(0x20);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadProgress);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_readClose(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_ReadFailed(cMCOverlayManager* self);

/**
 * Tracks a save-file read and passes the data to the front end until the whole file is loaded.
 *
 * mProgress is the card's byte count (card slot 18) over mSize. When the card's read completes, the front end
 * gets
 * callbackReadUpdate with the remaining size, then callbackReadEnd; a 0 from either fails the load through
 * cMCOverlayManager_ReadFailed(), otherwise the manager moves to LoadClosing (33). A card error goes to LoadFailed (36).
 */
void cMCOverlayManager::UpdateLoadProgress()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    mDisplayWaitTicks -= 1;
    int n = mpMemCard->GetDataSize();
    mProgress = (float)n / (float)mSize;
    if (mpMemCard->DidErrorOccur() != 0) {
        me->SetState(36);
        (callbackReadFailed)();
        return;
    }
    if (mpMemCard->IsReadWaiting() == true) {
        int loadedSize = mLoadedSize;
        int totalSize = mSize;
        if (loadedSize < totalSize) {
            if ((callbackReadUpdate)(totalSize - loadedSize) == 0)
                ReadFailed();
        } else if ((callbackReadEnd)() == 0) {
            mpMemCard->SetBrokenFile(mFileCursor);
            ReadFailed();
        } else {
            readClose();
            me->SetState(33);
        }
        return;
    }
    if (mpMemCard->DidErrorOccur() != 0) {
        (callbackReadFailed)();
        me->SetState(36);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoad);
#ifdef SKIP_ASM
extern "C" int cMCOverlayManager_updateDeviceStateMonitor(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_TriggerPollNoCardError(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_InitiateFileCheck(cMCOverlayManager* self);
extern "C" void cMCOverlayManager_RecheckCard(cMCOverlayManager* self);


#define S_02D0 ((char*)this)
#define CB_02D0(off) (*(int (**)())(S_02D0 + (off)))()

/**
 * Runs the load screen (state Load, 29): reacts to card status changes and starts a load or a delete.
 *
 * Card problems go to the no-card error, InitFormatConfirm (39), CardInvalid (5) or NoFreeBlocks (23) as far
 * as
 * the mPollFor* flags allow; a card inserted or recovered restarts the file check. After callbackLoadReady,
 * load
 * goes to LoadStart (31), LoadFailed (36) or LoadCorrupt (37), delete to DeleteConfirm (24), cancel to Done
 * (1).
 */
void cMCOverlayManager::UpdateLoad()
{
    if (mCheckCard)
        return;
    int quit = 0;
    if (mSave || mDelete || CB_02D0(0x78))
        quit = 1;
    int held = mCurrDeviceStateLast;
    int ready = updateDeviceStateMonitor();
    if (!CB_02D0(0x78) && ready) {

        if (mCurrDeviceStateLast & 8) {
            if (mPollForNoCard || CB_02D0(0x78) || CB_02D0(104) || quit) {
                TriggerPollNoCardError();
                return;
            }
        } else {
            if (mPollForUnformatted && (mCurrDeviceStateLast & 0x10)) {
                SetState(39);
                return;
            }
            if (mCurrDeviceStateLast & 1) {
                SetState(5);
                return;
            }
            if (mPollForFullCard && (mCurrDeviceStateLast & 4)) {
                SetState(23);
                return;
            }
        }
        if (!(mCurrDeviceStateLast & 8) && (held & 8)) {
            InitiateFileCheck();
            return;
        }
        int rel = held & ~4;
        if (mCurrDeviceStateLast == 0 && rel) {
            InitiateFileCheck();
            return;
        }
    }
    if (!quit) {
        if (mCardChanged) {
            mCardChanged = 0;
            RecheckCard();
        }
        if (mCurrDeviceStateLast & 8) {
            SetState(49);
            return;
        }
    }
    (callbackLoadReady)();
    if ((mState ^ 0x1D) != 0)
        return;
    if (CB_02D0(0x78)) {
        if (mLoadOk) {
            SetState(0x1F);
            return;
        }
        if (!mpMemCard->IsAvailable1()) {
            SetState(36);
            return;
        }
        if (!mpMemCard->HasFileName(mFileCursor)) {
            SetState(37);
            return;
        }
        if (!mpMemCard->IsFileSlotBroken(mFileCursor))
            return;
        SetState(37);
        return;
    }
    if (CB_02D0(104)) {
        if (!mDeleteOk)
            return;
        mpMemCard->GetTitleString(mFileCursor, mActiveTitleString);
        SetState(0x18);
        return;
    }
    if (CB_02D0(108))
        SetState(1);
}
#undef CB_02D0
#undef S_02D0
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateLoadClosing);
#ifdef SKIP_ASM
/**
 * Waits for the loaded file to close, then moves to LoadDoneWait (34), or to LoadFailed (36) on a card error.
 */
void cMCOverlayManager::UpdateLoadClosing()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    (mDisplayWaitTicks)--;
    if (mpMemCard->IsBusy() == 0) {
        if (mpMemCard->DidErrorOccur() != 0) {
            me->SetState(36);
            (callbackReadFailed)();
            return;
        }
        if (mDisplayWaitTicks <= 0) {
            me->SetState(34);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateDeleteConfirm);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);

/**
 * Handles the delete confirmation: delete moves to DeleteProgress (25) and cancel aborts with the fail
 * message.
 *
 * Once the card is idle, a missing card goes to NoCardsError (4) and a card error to DeleteFailed (28).
 */
void cMCOverlayManager::UpdateDeleteConfirm()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    if (mpMemCard->IsBusy() == 0) {
        if (mpMemCard->IsAvailable1() == 0) {
            me->SetState(4);
            return;
        }
        if (mpMemCard->DidErrorOccur() != 0) {
            me->SetState(0x1C);
            return;
        }
        if ((callbackIsDeletePressed)() != 0) {
            me->SetState(25);
            return;
        }
        if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateDeleteProgress);
#ifdef SKIP_ASM


/**
 * Waits for a file delete to finish, then moves to DeleteDoneWait (26), or DeleteFailed (28) on a card error.
 */
void cMCOverlayManager::UpdateDeleteProgress()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        cMemCard* card = mpMemCard;
        if (card->DidErrorOccur() != 0) {
            SetState(0x1C);
        } else {
            SetState(26);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateDeleteFailed);
#ifdef SKIP_ASM

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);

/**
 * Handles the "delete failed" message: once the card is idle, cancel aborts the operation.
 */
void cMCOverlayManager::UpdateDeleteFailed()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateDeleteDoneWait);
#ifdef SKIP_ASM
/**
 * Waits out the display delay after a delete, then moves to DeleteDone (27), or DeleteFailed (28) on a card
 * error.
 *
 * While the delay runs, each idle frame asks the card to recheck the selected device.
 */
void cMCOverlayManager::UpdateDeleteDoneWait()
{
    cMemCard* memCard = mpMemCard;
    if (mDisplayWaitTicks > 0) {
        if (memCard->IsBusy() == 0) {
            if (mpMemCard->DidErrorOccur() != 0) {
                SetState(0x1C);
                return;
            }
            mpMemCard->CheckDevice(mSelectedDevice);
        }
        mDisplayWaitTicks -= 1;
    } else {
        if (memCard->IsBusy() != 0) {
            return;
        }
        if (mpMemCard->DidErrorOccur() != 0) {
            SetState(0x1C);
        } else {
            SetState(27);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateDeleteDone);
#ifdef SKIP_ASM


/**
 * Finishes a delete: once the card is idle and callbackDeleteDone accepts, rereads the card (ReadingCard, 6).
 */
void cMCOverlayManager::UpdateDeleteDone()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackDeleteDone)() != 0) {
            SetState(6);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateOverwriteConfirm);
#ifdef SKIP_ASM
/**
 * Handles the overwrite confirmation: overwrite goes to PreSaveFile (13) and cancel to Done (1).
 *
 * Once the card is idle, a missing card goes to NoCardsError (4) and a card error to SaveFailed (20).
 */
void cMCOverlayManager::UpdateOverwriteConfirm()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    if (mpMemCard->IsBusy() == 0) {
        if (mpMemCard->IsAvailable1() == 0) {
            me->SetState(4);
            return;
        }
        if (mpMemCard->DidErrorOccur() != 0) {
            me->SetState(20);
            return;
        }
        if ((callbackIsOverwritePressed)() != 0) {
            me->SetState(13);
            return;
        }
        if ((callbackIsCancelPressed)() != 0) {
            me->SetState(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFormatConfirm);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the format prompt for an unformatted card: format starts it (InitFormatProgress, 40).
 *
 * Once the card is idle: a missing card goes to NoCardsError (4), cancel aborts with the fail message, retry
 * retries.
 */
void cMCOverlayManager::UpdateInitFormatConfirm()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    if (mpMemCard->IsBusy() == 0) {
        if (mpMemCard->IsAvailable1() == 0) {
            me->SetState(4);
            return;
        }
        if ((callbackIsFormatPressed)() != 0) {
            me->SetState(40);
            return;
        }
        if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(1);
            return;
        }
        if ((callbackIsRetryPressed)() != 0) {
            RetryCardOp();
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFormatProgress);
#ifdef SKIP_ASM
/**
 * Waits for the card format to finish, then moves to InitFormatFailed (41) or InitFormatDone (42).
 */
void cMCOverlayManager::UpdateInitFormatProgress()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    int waitTicks = (mDisplayWaitTicks)--;
    if (waitTicks <= 0) {
        if (mpMemCard->IsBusy() == 0) {
            if (mpMemCard->DidFormatFail() != 0) {
                me->SetState(41);
            } else {
                me->SetState(42);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFormatFailed);
#ifdef SKIP_ASM

extern "C" void cMCOverlayManager_checkDevice(cMCOverlayManager* self, int v);

/**
 * Handles a failed format during the initial card check: once cancel is pressed, rechecks the selected card.
 *
 * When callbackIsCancelPressed() returns nonzero: passes code 1 to the virtual state handler (vtable slot 1),
 * calls cMCOverlayManager_checkDevice() on mSelectedDevice and sets mCheckCard. Otherwise it keeps waiting.
 */
void cMCOverlayManager::UpdateInitFormatFailed()
{
    if ((callbackIsCancelPressed)() != 0) {
        SetState(1);
        checkDevice(mSelectedDevice);
        mCheckCard = 1;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitFormatDone);
#ifdef SKIP_ASM
/**
 * Finishes a format during the initial card check by passing code 6 to the virtual state handler (vtable slot
 * 1).
 */
void cMCOverlayManager::UpdateInitFormatDone()
{
    SetState(6);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitDeleteConfirm);
#ifdef SKIP_ASM
/**
 * Handles the prompt to delete the card's files: delete starts it (InitDeleteProgress, 44), cancel rereads
 * the card.
 *
 * Once the card is idle a missing card goes to NoCardsError (4); while waiting it keeps asking the card to
 * recheck
 * the selected device.
 */
void cMCOverlayManager::UpdateInitDeleteConfirm()
{
    cMoviePlayerVt* me = (cMoviePlayerVt*)this;
    if (mpMemCard->IsBusy() == 0) {
        if (mpMemCard->IsAvailable1() == 0) {
            me->SetState(4);
            return;
        }
        if ((callbackIsDeletePressed)() != 0) {
            me->SetState(44);
            return;
        }
        if ((callbackIsCancelPressed)() != 0) {
            me->SetState(6);
            return;
        }
        mpMemCard->CheckDevice(mSelectedDevice);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitDeleteProgress);
#ifdef SKIP_ASM
/**
 * Deletes the card's files one per frame, then moves to InitDeleteDone (46), or InitDeleteFailed (45) on an
 * error.
 *
 * mCurrDeleteFile walks the card's file list; each entry the card says can be deleted is deleted. The local
 * view
 * calls these card methods getCount, canSeek and seek (names copied from the movie-decoder views).
 */
void cMCOverlayManager::UpdateInitDeleteProgress()
{
    mDisplayWaitTicks -= 1;
    if (mpMemCard->IsBusy() != 0) {
        return;
    }
    if (mDisplayWaitTicks > 0) {
        return;
    }
    if (mpMemCard->DidErrorOccur() != 0) {
        SetState(45);
        return;
    }
    if (mCurrDeleteFile < mpMemCard->GetNumFiles()) {
        if (mpMemCard->IsFileSlotBroken(mCurrDeleteFile) != 0) {
            mpMemCard->DeleteFileSlot(mCurrDeleteFile);
        }
        mCurrDeleteFile += 1;
    } else {
        SetState(46);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitDeleteFailed);
#ifdef SKIP_ASM
/**
 * Handles a failed delete during the initial check by passing code 1 to the virtual state handler (vtable slot
 * 1).
 */
void cMCOverlayManager::UpdateInitDeleteFailed()
{
    SetState(1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateInitDeleteDone);
#ifdef SKIP_ASM
/**
 * Finishes a delete during the initial check by passing code 6 to the virtual state handler (vtable slot 1).
 */
void cMCOverlayManager::UpdateInitDeleteDone()
{
    SetState(6);
}
#endif

extern "C" void* cMCOverlayManager_ReturnToDevSelectScreen(cMCOverlayManager* self);

//99.29%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCardUnusable__FPv);
#ifdef SKIP_ASM
/**
 * Handles an unusable memory card by returning to the device-select screen.
 *
 * @return the result of cMCOverlayManager_ReturnToDevSelectScreen()
 */
void* cMCOverlayManager::UpdateCardUnusable()
{
    return ((cMCOverlayManager*)this)->ReturnToDevSelectScreen();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_UpdateCardDamaged);
#ifdef SKIP_ASM

extern "C" void cMCOverlayManager_CancelCardOp(cMCOverlayManager* self, int v);
extern "C" void cMCOverlayManager_RetryCardOp(cMCOverlayManager* self);

/**
 * Handles the "damaged card" message: once the card is idle, retry retries and cancel aborts with the fail
 * message.
 */
void cMCOverlayManager::UpdateCardDamaged()
{
    cMemCard* memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        if ((callbackIsRetryPressed)() != 0) {
            RetryCardOp();
        } else if ((callbackIsCancelPressed)() != 0) {
            CancelCardOp(1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_WriteFile1);
#ifdef SKIP_ASM
/**
 * Starts writing a file to the card: sets cMCOverlayManager_mSaveInProgress, calls mpMemCard's slot 36 (the first of On
 * Tour's three cMemCard::StartFileSlotWrite overloads) with @p filename, @p data and @p size, and stores @p size in
 * mSavedSize.
 *
 * @param filename  the file name (8-bit string)
 * @param data  the data
 * @param size  the data size
 */
void cMCOverlayManager::WriteFile1(const char* filename, int data, int size)
{
    cMemCard* memCard = mpMemCard;
    cMCOverlayManager_mSaveInProgress = 1;
    memCard->StartFileSlotWrite(filename, data, size);
    mSavedSize = size;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_WriteFile);
#ifdef SKIP_ASM
extern int cMCOverlayManager_mSaveInProgress;


/**
 * Starts writing a file to the card under a 16-bit name: sets cMCOverlayManager_mSaveInProgress, calls mpMemCard's slot
 * 37 (a cMemCard::StartFileSlotWrite overload) with @p filename, @p data and @p size, and stores @p size in mSavedSize.
 *
 * @param filename  the file name (16-bit string)
 * @param data  the data
 * @param size  the data size
 */
void cMCOverlayManager::WriteFile(unsigned short* filename, int data, int size)
{
    cMemCard* memCard = mpMemCard;
    cMCOverlayManager_mSaveInProgress = 1;
    memCard->StartFileSlotWrite1(filename, data, size);
    mSavedSize = size;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_WriteFile2);
#ifdef SKIP_ASM

/**
 * Starts writing a file to the card: sets cMCOverlayManager_mSaveInProgress, calls mpMemCard's slot 38 (the third
 * cMemCard::StartFileSlotWrite overload) with @p file, @p data and @p size, and stores @p size in mSavedSize.
 *
 * @param file  the file to write (the slot's first argument)
 * @param data  the data
 * @param size  the data size
 */
void cMCOverlayManager::WriteFile2(int file, int data, int size)
{
    cMemCard* memCard = mpMemCard;
    cMCOverlayManager_mSaveInProgress = 1;
    memCard->StartFileSlotWrite2(file, data, size);
    mSavedSize = size;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_cleanUp);
#ifdef SKIP_ASM
/**
 * Tells the memory card object (+0x434) to clean up, through its virtual slot 12.
 */
void cMCOverlayManager::cleanUp()
{
    cMemCard* obj = mpMemCard;
    obj->CleanUp();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetMaxFiles);
#ifdef SKIP_ASM

/**
 * Returns the number of file slots: mMaxFiles, capped at the card's own limit (mpMemCard slot 68, GetMaxFiles by On
 * Tour's cMemCard order).
 *
 * @return the usable file count
 */
int cMCOverlayManager::GetMaxFiles()
{
    cMemCard* memCard = mpMemCard;
    if (mMaxFiles < memCard->GetMaxFiles()) {
        return mMaxFiles;
    }
    cMemCard* memCardAgain = mpMemCard;
    return memCardAgain->GetMaxFiles();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_WriteData);
#ifdef SKIP_ASM
extern int cMCOverlayManager_mSaveInProgress;


/**
 * Writes the next block of save data: adds @p size to mSavedSize, sets cMCOverlayManager_mSaveInProgress and passes
 * @p data and @p size to mpMemCard's slot 41 (WriteData by On Tour's cMemCard order).
 *
 * @param data  the block
 * @param size  its size
 */
void cMCOverlayManager::WriteData(int data, int size)
{
    cMemCard* memCard = mpMemCard;
    int one = 1;
    mSavedSize += size;
    cMCOverlayManager_mSaveInProgress = one;
    memCard->WriteData(data, size);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_WriteFailed);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cMCOverlayManager_mSaveInProgress;
extern "C" void cMCOverlayManager_SetAction_v(void* self, int v) __asm__("cMCOverlayManager_SetAction");



/**
 * Ends a failed save: stops the card write if one is in progress and optionally shows the failure.
 *
 * Clears the global save-in-progress flag; with @p showWriteFailPopup it also calls SetAction(0) and moves to
 * SaveFailed (20).
 *
 * @param showWriteFailPopup  nonzero to show the save-failed state
 */
void cMCOverlayManager::WriteFailed(int showWriteFailPopup)
{
    int on = mWriteInProgress;
    cMCOverlayManager_mSaveInProgress = 0;
    if (on != 0) {
        cMemCard* memCard = mpMemCard;
        memCard->SetWriteFailed();
    }
    if (showWriteFailPopup != 0) {
        cMCOverlayManager_SetAction_v(this, 0);
        SetState(20);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ReadData);
#ifdef SKIP_ASM
/**
 * Reads the next chunk of the file being loaded.
 *
 * Adds @p n to the running byte count (+0xE8) and calls virtual slot 30 of the memory card object (+0x434).
 *
 * @param data  destination buffer
 * @param n  bytes to read
 */
void cMCOverlayManager::ReadData(int data, int n)
{
    mLoadedSize += n;
    cMemCard* obj = mpMemCard;
    obj->ReadData(data, n);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ReadFailed);
#ifdef SKIP_ASM


/**
 * Fails the current read: tells the card (mpMemCard slot 33, SetReadFailed by On Tour's cMemCard order), sets state 36
 * through vtable slot 1 (cMCOverlayManager_SetState) and calls callbackReadFailed.
 */
void cMCOverlayManager::ReadFailed()
{
    cMemCard* memCard = mpMemCard;
    memCard->SetReadFailed();
    SetState(36);
    (callbackReadFailed)();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_SettingsWriteStart);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBESaveInterface_generateSaveGame(void* iface);
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_r() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" void strcpy3(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void cMCOverlayManager_getFileName_r(void* self, int mode, void* buf, int size) __asm__("cMCOverlayManager_getFileName");
extern "C" void cMCOverlayManager_getProductCode_r(void* buf, int size) __asm__("cMCOverlayManager_getProductCode");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void cMCOverlayManager_WriteFile_r(void* self, char* text, int a, int b) __asm__("cMCOverlayManager_WriteFile");
extern int cMCOverlayManager_mSaveInProgress;
extern char str_pct_s_pct_spct_04d_b[];

/**
 * Saves the game settings: sets cMCOverlayManager_mSaveInProgress, generates the save data
 * (cBESaveInterface_generateSaveGame) and its size, and starts writing it (cMCOverlayManager_WriteFile) under the title "<product
 * code>-<type name><file cursor + 1>" (str_pct_s_pct_spct_04d_b; type name of file type 2).
 */
void cMCOverlayManager::SettingsWriteStart()
{
    char text[130];
    char name[130];
    char desc[0x200];
    char fmt[0x200];
    cMCOverlayManager_mSaveInProgress = 1;
    int saveData = cBESaveInterface_generateSaveGame(cBE_getInterface_Fv(cBE_getBE(), 5));
    int saveSize = cBESaveInterface_getSaveGameSize_r();
    strcpy3(fmt, str_pct_s_pct_spct_04d_b);
    cMCOverlayManager_getFileName_r(this, 2, name, 130);
    cMCOverlayManager_getProductCode_r(desc, 0x200);
    sprintf1_v(text, fmt, desc, name, mFileCursor + 1);
    cMCOverlayManager_WriteFile_r(this, text, saveData, saveSize);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_ManagerStart);
#ifdef SKIP_ASM

/**
 * Starts the manager's card check.
 *
 * Sets mCheckAllowed, clears mSaved, mCurrCheckDevice and mFileCursor, sets mLastCheckDevice, mSaveProfileID and
 * mLastMountedDevice to -1, selects device 0 (cMCOverlayManager_selectDevice, cMCOverlayManager_setSelectedDevice), sets mCheckCard,
 * puts mState to 2 (On Tour's kMCOverlay_CheckCardsWait) and updates the card (mpMemCard slot 3).
 */
void cMCOverlayManager::ManagerStart()
{
    int one = 1;
    cMemCard* memCard;
    mCheckAllowed = one;
    mSaved = 0;
    mCurrCheckDevice = 0;
    mFileCursor = 0;
    mLastCheckDevice = -1;
    mSaveProfileID = -1;
    mLastMountedDevice = -1;
    selectDevice(0);
    setSelectedDevice();
    mCheckCard = one;
    mState = 2;
    memCard = mpMemCard;
    memCard->Update();
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_OnHighlightNextDevice);
#ifdef SKIP_ASM
class cMovieDecoder_241400 {
public:
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
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual int hasChapter(int i);  // slot 52 (0x1A0)
};

extern "C" void cMCOverlayManager_setSelectedDevice(cMCOverlayManager* self);
void cMCOverlayManager_selectDevice(void* self, int val);

static inline int nextChapter_241400(char* self, int i)
{
    if (i < *(int*)(self + 0x424) - 1) {
        return i + 1;
    }
    if (*(int*)(self + 0x100) != 0) {
        return 0;
    }
    return i;
}

/**
 * Moves the memory card device highlight to the next device that has a card.
 *
 * Does nothing when no device count is known (+0x424 == kMaxDevicesUnknown). Steps from the current device (+0x428) to
 * the next one the memory card object (+0x434) reports present, stopping at the last device unless wrapping is allowed
 * (+0x100); falls back to the current device or device 0. A new device is selected with cMCOverlayManager_selectDevice
 * and cMCOverlayManager_setSelectedDevice; +0xE0 is cleared. (The C's helper type names,
 * cMovieDecoder_241400/hasChapter, are leftovers: this is the memory card device list.)
 */
void cMCOverlayManager::OnHighlightNextDevice()
{
    int i = mSelectedDevice;
    if (mMaxDevices == kMaxDevicesUnknown) {
        return;
    }
    while (i = nextChapter_241400(((char*)this), i), mpMemCard->IsAvailable(i) == 0) {
        if (i == mMaxDevices - 1 || mWrapItems != 0) {
            if (i == mSelectedDevice) {
                break;
            }
            if (mWrapItems != true) {
                break;
            }
        }
    }
    if (mWrapItems == 0 && i == mMaxDevices - 1 && mpMemCard->IsAvailable(i) == 0) {
        i = mSelectedDevice;
        if (mpMemCard->IsAvailable(i) == 0) {
            i = 0;
        }
    }
    if (mSelectedDevice != i) {
        cMCOverlayManager_selectDevice(((char*)this), i);
        setSelectedDevice();
    }
    mGetDir = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_OnHighlightPrevDevice);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_setSelectedDevice(cMCOverlayManager* self);
void cMCOverlayManager_selectDevice(void* self, int val);

static inline int prevChapter_241540(char* self, int i)
{
    if (i > 0) {
        return i - 1;
    }
    if (*(int*)(self + 0x100) != 0) {
        return *(int*)(self + 0x424) - 1;
    }
    return i;
}

/**
 * Moves the device highlight to the previous memory-card device that the card object reports as available.
 *
 * Does nothing while mMaxDevices is kMaxDevicesUnknown. Steps mSelectedDevice down, wrapping to mMaxDevices - 1 when
 * mWrapItems is set, until the card object's virtual slot 52 accepts the device or the search gives up. Without
 * wrapping, an unavailable device 0 keeps the current device (or 0 if that one is unavailable too). A new selection
 * goes through cMCOverlayManager_selectDevice() and cMCOverlayManager_setSelectedDevice(), which resets the device
 * state monitor. Always clears mGetDir.
 */
void cMCOverlayManager::OnHighlightPrevDevice()
{
    int i = mSelectedDevice;
    if (mMaxDevices == kMaxDevicesUnknown) {
        return;
    }
    while (i = prevChapter_241540(((char*)this), i), mpMemCard->IsAvailable(i) == 0) {
        if (i == 0 || mWrapItems != 0) {
            if (i == mSelectedDevice) {
                break;
            }
            if (mWrapItems != true) {
                break;
            }
        }
    }
    if (mWrapItems == 0 && i == 0 && mpMemCard->IsAvailable(0) == 0) {
        i = mSelectedDevice;
        if (mpMemCard->IsAvailable(i) == 0) {
            i = 0;
        }
    }
    if (mSelectedDevice != i) {
        cMCOverlayManager_selectDevice(((char*)this), i);
        setSelectedDevice();
    }
    mGetDir = 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_OnHighlightPrevFile);
#ifdef SKIP_ASM
/**
 * Moves the file cursor (mFileCursor) back one slot; at slot 0 it wraps to the last slot (cMCOverlayManager_GetMaxFiles - 1) when
 * mWrapItems is set.
 */
void cMCOverlayManager::OnHighlightPrevFile()
{
    int n = mFileCursor;
    if (n > 0) {
        mFileCursor = n - 1;
    } else if (mWrapItems != 0) {
        mFileCursor = GetMaxFiles() - 1;
    }
}
#endif
//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_OnHighlightNextFile);
#ifdef SKIP_ASM
/**
 * Moves the file cursor (mFileCursor) forward one slot; at the last slot (cMCOverlayManager_GetMaxFiles - 1) it wraps to 0 when
 * mWrapItems is set.
 */
void cMCOverlayManager::OnHighlightNextFile()
{
    int n = GetMaxFiles() - 1;
    int cur = mFileCursor;
    if (cur < n) {
        mFileCursor = cur + 1;
    } else if (mWrapItems != 0) {
        mFileCursor = 0;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetDeviceDisplayString);
#ifdef SKIP_ASM
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern "C" void* strcpy1(void* dst, void* src);
extern "C" int strlen1(void* s);
extern "C" void cMCOverlayManager_getDeviceNumberString(cMCOverlayManager* self, void* dst, int n);
int GetHashValue32(char* s);
extern char str_kT_MEMOVNotDetectedPS2_b[];
extern char str_kT_MEMOVNotFormattedPS2_b[];
extern char str_kT_MEMMemDevicePS2[];

struct sLocVEntry_241710 {
    short delta;
    short index;
    void* (*fn)(void*, int);
};



static inline void* Loc_241710(char* key)
{
    cFELocale* loc = SSXApp->FELocale;
    sLocVEntry_241710* vt = (sLocVEntry_241710*)VPTR_OF(cUILocaleInterface, loc);
    void* thisp = vthis(loc, vt[4]);
    return vt[4].fn(thisp, GetHashValue32(key));
}

/**
 * Builds the on-screen label for a memory-card device.
 *
 * A missing or failing device gets the localized kT_MEMOVNotDetectedPS2 text and an unformatted one
 * kT_MEMOVNotFormattedPS2, both formatted with the device number (idx + 1). A usable device shows its card
 * name, or
 * kT_MEMMemDevicePS2 when the name is empty. A formatted card with the wrong encoding leaves @p dst
 * untouched.
 *
 * @param idx  device index
 * @param dst  receives the label (16-bit string)
 */
void cMCOverlayManager::GetDeviceDisplayString(int idx, char* dst)
{
    char buf[0x200];
    if (mpMemCard->IsAvailable(idx))
    {
        if (mpMemCard->IsInvalid(idx) || mpMemCard->IsDamaged(idx) || mpMemCard->IsWrongDevice1(idx))
        {
            getDeviceNumberString(buf, idx);
            sprintf1_v(dst, Loc_241710(str_kT_MEMOVNotDetectedPS2_b), buf);
        }
        else if (!mpMemCard->IsFormatted1(idx))
        {
            getDeviceNumberString(buf, idx);
            sprintf1_v(dst, Loc_241710(str_kT_MEMOVNotFormattedPS2_b), buf);
        }
        else if (mpMemCard->IsEncodingCorrect(idx))
        {
            getDeviceNumberString(buf, idx);
            char* name = ((char*)&((&mDevices[0]))[idx].szName[0]);
            if (strlen1(name) == 0)
                sprintf1_v(dst, Loc_241710(str_kT_MEMMemDevicePS2), buf);
            else
                strcpy1(dst, name);
        }
    }
    else
    {
        getDeviceNumberString(buf, idx);
        sprintf1_v(dst, Loc_241710(str_kT_MEMOVNotDetectedPS2_b), buf);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetDeviceFreeString);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From candidtheartist's fork (github.com/candidtheartist/ssx3), via its pull request to this repository.
extern "C" unsigned short* strcpy3_r(void*, void*) __asm__("strcpy3");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern char str_Free[];
extern char str_pct_d_pct_s[];

/**
 * Prints device @p i's free space as "<iFreeSpace> Free" (str_pct_d_pct_s with str_Free, both widened) into @p out,
 * when the device is valid, formatted, correctly encoded and plugged in (mDevices[i]).
 *
 * @param i  the device
 * @param out  16-bit destination buffer
 */
void cMCOverlayManager::GetDeviceFreeString(int i, void* out)
{
    if (mDevices[i].bPluggedIn != 0 && mDevices[i].bValid != 0 && mDevices[i].bFormatted != 0
        && mDevices[i].bCorrectEncoding != 0) {
        char name[0x200];
        char path[0x200];
        strcpy3_r(name, str_Free);
        strcpy3_r(path, str_pct_d_pct_s);
        sprintf1_v(out, path, mDevices[i].iFreeSpace, name);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetFreeSpace);
#ifdef SKIP_ASM
/**
 * Returns mDevices[@p device].iFreeSpace.
 *
 * @param device  the device
 * @return its free space
 */
int cMCOverlayManager::GetFreeSpace(int device)
{
    return mDevices[device].iFreeSpace;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetDeviceTotalString);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" unsigned short* strcpy3_r(void*, void*) __asm__("strcpy3");
extern "C" void sprintf1_v(void* dst, void* fmt, ...) __asm__("sprintf1");
extern char str_Total[];
extern char str_pct_d_pct_s[];

struct sMCOverlayMgr {
    char pad[0x344];
    cMCOverlayManager_sdevice dev[1];
};

/**
 * Writes a device's total space as "<total> Total" into a 16-bit string, when the device is usable.
 *
 * Writes nothing unless the device is plugged in, valid, formatted and correctly encoded; the format "%d %s"
 * and "Total" are converted to 16-bit strings first.
 *
 * @param idx  index of the device record (cMCOverlayManager_sdevice)
 * @param dst  destination 16-bit string
 */
extern "C" void cMCOverlayManager_GetDeviceTotalString(sMCOverlayMgr* self, int idx, void* dst)
{
    unsigned short str[256];
    unsigned short fmt[256];
    if (self->dev[idx].bPluggedIn == 0) return;
    if (self->dev[idx].bValid && self->dev[idx].bFormatted && self->dev[idx].bCorrectEncoding) {
        strcpy3_r(str, str_Total);
        strcpy3_r(fmt, str_pct_d_pct_s);
        sprintf1_v(dst, fmt, self->dev[idx].iTotalSpace, str);
    }
}
#endif

struct sPad16 { char x; int pad[3]; };
extern sPad16 D_0047C3A8;
extern "C" void strcpy3(void*, void*);

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getProductCode);
#ifdef SKIP_ASM
/**
 * Writes the game's product code "BASLUS-20772" (D_0047C3A8), widened to 16 bits (strcpy3), into @p self.
 *
 * Callers pass a buffer size as well, which is ignored.
 */
void cMCOverlayManager::getProductCode()
{
    strcpy3(this, &D_0047C3A8);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getMaxDevicesSupported);
#ifdef SKIP_ASM
/**
 * Returns how many memory-card devices the manager uses, asking the card once and caching the answer.
 *
 * The card's count (card vtable slot 69) is clamped to 1, so only the first device is ever used.
 *
 * @return the number of devices (cached in mMaxDevices)
 */
int cMCOverlayManager::getMaxDevicesSupported()
{
    if (mMaxDevices < 0) {
        cMemCard* memCard = mpMemCard;
        int n = memCard->GetMaxDevices();
        mMaxDevices = n;
        if (n >= 2) {
            mMaxDevices = 1;
        }
    }
    return mMaxDevices;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getDeviceState);
#ifdef SKIP_ASM
struct sMovieFlags_1B20
{
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int d : 1;
    unsigned int e : 1;
    unsigned int f : 4;
    unsigned int rest : 23;
};

// PORT: the unit declares cMCOverlayManager_getDeviceState as returning int; it really returns this 4-byte flags struct in $v0.
extern "C" sMovieFlags_1B20 cMCOverlayManager_getDeviceState_s(void* self) __asm__("cMCOverlayManager_getDeviceState");

/**
 * Returns the status flags of the selected memory-card device.
 *
 * While another device is being checked it returns the last stored state (mCurrDeviceStateLast). Otherwise it
 * asks
 * the card and builds the flags: error, damaged, full (free space below what the save needs), no card,
 * unformatted.
 *
 * @return the device state (bits 1 error, 2 damaged, 4 full, 8 no card, 0x10 unformatted in
 *         mCurrDeviceStateLast)
 */
extern "C" sMovieFlags_1B20 cMCOverlayManager_getDeviceState_s(void* vself)
{
    cMCOverlayManager* self = (cMCOverlayManager*)vself;
    if (self->mCurrCheckDevice != self->mSelectedDevice)
        return *(sMovieFlags_1B20*)&self->mCurrDeviceStateLast;
    sMovieFlags_1B20 state = {0, 0, 0, 0, 0, 0, 0};
    state.a = self->mpMemCard->IsInvalid1();
    int isDamaged = self->mpMemCard->IsDamaged1() || self->mpMemCard->IsWrongDevice();
    state.b = isDamaged;
    int isFull = !self->mpMemCard->GetNumFiles() && self->mpMemCard->GetFreeSpace() < self->mpMemCard->SpaceRequired1();
    state.c = isFull;
    state.d = !self->mpMemCard->IsAvailable1();
    int isUnformatted = !self->mpMemCard->IsFormatted() || !self->mpMemCard->IsEncodingCorrect1();
    state.e = isUnformatted;
    state.f = 0;
    return state;
}
#endif

//100% - objdiff report; single-function view differs only in a relocation name
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_updateDeviceStateMonitor);
#ifdef SKIP_ASM
extern "C" int cMCOverlayManager_getDeviceState(void* self);

/**
 * Polls the selected device's state and tells whether an unacknowledged state change is waiting.
 *
 * Stores the state from cMCOverlayManager_getDeviceState() in mCurrDeviceStateLast. On a change, the front
 * end's callbackAcknowledgeCardStatusChange() decides whether it is acknowledged (mCurrDeviceStateAck).
 *
 * @return 1 when the current state is not acknowledged, otherwise 0
 */
int cMCOverlayManager::updateDeviceStateMonitor()
{
    int newState = cMCOverlayManager_getDeviceState(this);
    int changed = mCurrDeviceStateLast != newState;
    if (mCurrDeviceStateAck == 0 || changed) {
        mCurrDeviceStateLast = newState;
        if (changed) {
            mCurrDeviceStateAck = (callbackAcknowledgeCardStatusChange)(newState);
        }
        return mCurrDeviceStateAck ^ 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getFileName);
#ifdef SKIP_ASM
extern "C" void strcpy3(void*, void*);
extern char str_REP[];
extern char str_GAM[];
extern char str_SET[];

/**
 * Writes the three-letter name of file type @p mode, widened to 16 bits, into @p buf: "REP" (kMCFileType_Replay),
 * "GAM" (kMCFileType_Profile) or "SET" (kMCFileType_Settings); other types leave it untouched.
 *
 * @param mode  the file type (tMCFileType)
 * @param buf  16-bit destination buffer
 */
void cMCOverlayManager::getFileName(int mode, void* buf)
{
    switch (mode) {
    case kMCFileType_Replay:
        strcpy3(buf, str_REP);
        break;
    case kMCFileType_Profile:
        strcpy3(buf, str_GAM);
        break;
    case kMCFileType_Settings:
        strcpy3(buf, str_SET);
        break;
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", func_00241DC0);
#ifdef SKIP_ASM
/**
 * Returns 0x30 (the character '0').
 *
 * @return 0x30
 */
extern "C" int func_00241DC0(void) {
    return 0x30;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_getDeviceNumberString);
#ifdef SKIP_ASM
extern "C" void sprintf1(void* dst, void* src, int n);
extern char str_pct_d_g[];

/**
 * Writes a device's number for display ("1", "2", ...) as a wide string.
 *
 * @param dst  the destination wide-string buffer
 * @param n    the zero-based device index; the text shows @p n + 1
 */
void cMCOverlayManager::getDeviceNumberString(void* dst, int n)
{
    unsigned short buf[256];
    strcpy3(buf, str_pct_d_g);
    sprintf1(dst, buf, n + 1);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GetMessage);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
int GetHashValue32(char* str);
extern "C" void cMCOverlayManager_getDeviceNumberString(cMCOverlayManager* self, void* dst, int n);
extern "C" void* strcpy1(void* dst, void* src);
extern char D_004A29E8[];
extern char str_kT_MEMFECheckingCardPS2_b[];
extern char str_kT_MEMOVNotDetectedPS2_b[];
extern char str_kT_MEMFormattingPS2_b[];
extern char str_kT_MEMOVSaveSavingPS2NGC_b[];
extern char str_kT_MEMOVLoadingPS2NGC[];
extern char str_kT_MEMDeletingPS2_b[];

struct sVEntry_00241E18 {
    short delta;
    short index;
    int (*fn)(void*, int);
};

static inline int Loc_00241E18(char* key)
{
    cFELocale* obj = SSXApp->FELocale;
    sVEntry_00241E18* vt = (sVEntry_00241E18*)VPTR_OF(cUILocaleInterface, obj);
    return vt[4].fn(vthis(obj, vt[4]), GetHashValue32(key));
}

static inline void SetText_00241E18(void* out, char* key, void* arg)
{
    int str = Loc_00241E18(key);
    sprintf1_v(out, (void*)str, arg);
}

/**
 * Fills a wide-string buffer with the status message for the memory card manager's current state.
 *
 * Starts from an empty string, gets the device number text (cMCOverlayManager_getDeviceNumberString) and, by state (+0x130), formats
 * kT_MEMFECheckingCardPS2, kT_MEMOVNotDetectedPS2, kT_MEMFormattingPS2, kT_MEMOVSaveSavingPS2NGC or
 * kT_MEMOVLoadingPS2NGC with it, or copies kT_MEMDeletingPS2; kMCOverlay_FileExists and others leave it empty.
 *
 * @param out  the destination wide-string buffer
 */
void cMCOverlayManager::GetMessage(void* out)
{
    unsigned short num[256];
    unsigned short num2[256];
    strcpy3(out, D_004A29E8);
    getDeviceNumberString(num, mSelectedDevice);
    switch (mState) {
    case kMCOverlay_Idle: case kMCOverlay_CheckCardsWait: case kMCOverlay_CheckCards: case kMCOverlay_ReadingCard:
    case kMCOverlay_Save: case kMCOverlay_PreSaveFile: case kMCOverlay_Load:
    case kMCOverlay_InitCardsCheck: case kMCOverlay_InitCardsCheckDone: case kMCOverlay_InitFileCheck:
    case kMCOverlay_InitFileCheckDone: case kMCOverlay_InitFileCheckWait: case kMCOverlay_InitFileCheckDevice:
        SetText_00241E18(out, str_kT_MEMFECheckingCardPS2_b, num);
        break;
    case kMCOverlay_NoCardsError:
        getDeviceNumberString(num2, mSelectedDevice);
        SetText_00241E18(out, str_kT_MEMOVNotDetectedPS2_b, num2);
        break;
    case kMCOverlay_FormatConfirm: case kMCOverlay_FormatProgress:
    case kMCOverlay_InitFormatConfirm: case kMCOverlay_InitFormatProgress:
        SetText_00241E18(out, str_kT_MEMFormattingPS2_b, num);
        break;
    case kMCOverlay_CreateFile: case kMCOverlay_SaveFile: case kMCOverlay_SaveProgress: case kMCOverlay_SaveClosing:
        SetText_00241E18(out, str_kT_MEMOVSaveSavingPS2NGC_b, num);
        break;
    case kMCOverlay_LoadStart: case kMCOverlay_LoadProgress: case kMCOverlay_LoadClosing: case kMCOverlay_LoadDoneWait:
        SetText_00241E18(out, str_kT_MEMOVLoadingPS2NGC, num);
        break;
    case kMCOverlay_DeleteProgress: case kMCOverlay_DeleteDoneWait: case kMCOverlay_InitDeleteProgress:
        strcpy1(out, (void*)Loc_00241E18(str_kT_MEMDeletingPS2_b));
        break;
    case kMCOverlay_FileExists:
        break;
    }
}
#endif

extern "C" void* cMCOverlayManager_GatherDeviceInfo1(void*, int);

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GatherDeviceInfo__FPv);
#ifdef SKIP_ASM
/**
 * Refreshes the info record of the device being checked (mCurrCheckDevice) through GatherDeviceInfo1().
 *
 * @return no meaningful value (cMCOverlayManager_GatherDeviceInfo1 returns nothing)
 */
void* cMCOverlayManager::GatherDeviceInfo()
{
    return cMCOverlayManager_GatherDeviceInfo1(this, mCurrCheckDevice);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_InitiateFileCheck);
#ifdef SKIP_ASM
/**
 * Starts looking for the game's files on the card: calls callbackSysRecheckFile and moves to a file-check
 * state.
 *
 * Goes to InitFileCheck (51) when the card is idle, otherwise InitFileCheckWait (53).
 */
void cMCOverlayManager::InitiateFileCheck()
{
    cMemCard* memCard;
    (callbackSysRecheckFile)();
    memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        SetState(51);
    } else {
        SetState(53);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_RecheckCard);
#ifdef SKIP_ASM


/**
 * Starts a fresh card check: clears mCheckCard and moves to CheckCardsWait (2), or CheckCards (3) if the card
 * is busy.
 */
void cMCOverlayManager::RecheckCard()
{
    cMemCard* memCard;
    mCheckCard = 0;
    memCard = mpMemCard;
    if (memCard->IsBusy() == 0) {
        SetState(2);
    } else {
        SetState(3);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_GatherDeviceInfo1);
#ifdef SKIP_ASM
extern "C" void MEM_fill(void* p, int c, int n);

struct sSlot_20C8
{
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    char name[200];
};

struct sMP_20C8
{
    char pad0[0x344];
    sSlot_20C8 slots[1];
    int f424;
    int f428;
    int cur;
};

// PORT: the unit declares cMCOverlayManager_GatherDeviceInfo1 as returning void*, but it returns nothing; bind the void body by asm label.
extern "C" void cMCOverlayManager_GatherDeviceInfo1_v(void* self, int i) __asm__("cMCOverlayManager_GatherDeviceInfo1");

/**
 * Refreshes the record for one memory-card device from the card.
 *
 * Marks the device valid, formatted, correctly encoded and plugged in, then clears whichever flag the card
 * contradicts and stops there. For a usable card it reads the card's name (100 characters), and for the
 * device
 * being checked (mCurrCheckDevice) also its free space.
 *
 * @param i  device index
 */
void cMCOverlayManager::GatherDeviceInfo1(int i)
{
    sMP_20C8* me = (sMP_20C8*)this;
    me->slots[i].a = 1;
    char* bb = (char*)&mDevices[0].bFormatted;
    int* pb = (int*)(bb + i * 0xE0);
    char* bc = (char*)&mDevices[0].bCorrectEncoding;
    int* pc = (int*)(bc + i * 0xE0);
    *pb = 1;
    *pc = 1;
    char* name = me->slots[i].name;
    me->slots[i].d = 1;
    MEM_fill(name, 0, 200);
    if (mpMemCard->IsInvalid(i) || mpMemCard->IsDamaged(i) || mpMemCard->IsWrongDevice1(i))
    {
        me->slots[i].a = 0;
        return;
    }
    if (!mpMemCard->IsFormatted1(i))
    {
        *pb = 0;
        return;
    }
    if (!mpMemCard->IsEncodingCorrect(i))
    {
        *pc = 0;
        return;
    }
    if (i == me->cur)
        me->slots[me->cur].e = mpMemCard->GetFreeSpace();
    mpMemCard->GetDeviceName(i, (unsigned short*)name, 100);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager_cMCOverlayManager);
#ifdef SKIP_ASM
// From peak3project's fork (github.com/peak3project/ssx3).
void cMCOverlayManager_CallbackMemcard(void*);
void cMCOverlayManager_CallbackFalse(void*);
void cMCOverlayManager_CallbackTrue(void*);
void cMCOverlayManager_Callbackpdate(void*);
extern "C" void MEM_fill(void* p, int c, int n);
extern "C" void* cMCOverlayManager_SetAction(cMCOverlayManager* self, int mode);
extern void* cMCOverlayManager_vtbl[];
extern int cMCOverlayManager_mSaveInProgress;

/**
 * Constructs the memory-card overlay manager: resets every state field and installs the default callbacks.
 *
 * Sets mFileType 2 and SetAction(1), and fills the 45 front-end callback slots with stubs that do nothing or
 * return 0 or 1, as fitting each slot. The device records are zeroed and mMaxDevices is -1 (not yet asked).
 *
 * @return @p self
 */
extern "C" void* cMCOverlayManager_cMCOverlayManager(cMCOverlayManager* self)
{
    int* w = (int*)self;
    void** callbacks = (void**)self;
    cMCOverlayManager_sdevice* dev = ((sMCOverlayMgr*)self)->dev;
    w[45] = 2;
    w[47] = 0;
    w[0x30] = 0;
    w[49] = 0;
    w[466] = (int)cMCOverlayManager_vtbl;
    self->SetAction(1);
    w[76] = 0;
    w[0x3E] = 0;
    w[0x3F] = -1;
    w[67] = 0;
    w[65] = 0;
    w[66] = 0;
    w[50] = 0;
    w[51] = 0;
    w[52] = 0;
    w[53] = 0;
    w[54] = 0;
    w[0x38] = 0;
    w[0x3C] = 0;
    w[58] = 0;
    w[57] = 0;
    w[61] = 0;
    w[55] = 0;
    w[59] = 0;
    w[265] = -1;
    w[0x40] = 0;
    w[75] = 0;
    w[70] = 0;
    w[71] = 0;
    w[72] = 0;
    w[73] = 0;
    w[74] = 1;
    self->mFlash = 1.0f;
    self->mFlashStep = 0.016669999808073044f;
    MEM_fill(dev, 0, 0xE0);
    w[273] = -1;
    w[266] = 0;
    w[267] = 0;
    w[271] = 0;
    w[272] = 0;
    MEM_fill(w + 0x8D, 0, 66);
    w[269] = 0;
    w[207] = 5;
    w[208] = 0;
    w[206] = 0;
    cMCOverlayManager_mSaveInProgress = 0;
    w[270] = 0;
    callbacks[0] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[1] = (void*)cMCOverlayManager_Callbackpdate;
    callbacks[2] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[3] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[4] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[5] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[6] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[7] = (void*)cMCOverlayManager_Callbackpdate;
    callbacks[8] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[9] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[10] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[11] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[12] = (void*)cMCOverlayManager_CallbackTrue;
    callbacks[13] = (void*)cMCOverlayManager_CallbackTrue;
    callbacks[14] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[15] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[16] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[17] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[18] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[19] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[20] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[21] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[22] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[23] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[24] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[25] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[26] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[27] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[28] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[29] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[30] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[31] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[32] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[33] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[34] = (void*)cMCOverlayManager_CallbackFalse;
    callbacks[35] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[36] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[37] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[38] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[39] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[40] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[41] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[42] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[43] = (void*)cMCOverlayManager_CallbackMemcard;
    callbacks[44] = (void*)cMCOverlayManager_CallbackFalse;
    for (int i = 0; i < 1; i++) {   // every device record (sMCOverlayMgr::dev has one)
        MEM_fill(&dev[i], 0, 0xE0);
    }
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cMCOverlayManager__cMCOverlayManager);
#ifdef SKIP_ASM
extern int cMCOverlayManager_mSaveInProgress;
extern void* cMCOverlayManager_vtbl[];
void operator_delete(int*);

/**
 * Destroys the memory-card overlay manager: restores its vtable, clears the save-in-progress flag, frees on
 * request.
 *
 * @param flags  g++ destructor flags; bit 0 set frees the object's memory
 */
extern "C" void cMCOverlayManager__cMCOverlayManager(cMCOverlayManager* self, int flags)
{
    VPTR_OF(cMCOverlayManager, self) = cMCOverlayManager_vtbl;
    cMCOverlayManager_mSaveInProgress = 0;
    if (flags & 1) {
        operator_delete((int*)self);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cOcclusionMan_cOcclusionMan__FPv);
#ifdef SKIP_ASM
/**
 * Constructs the occlusion manager by zeroing its words at +0x0, +0x4, +0x108 and +0x10C.
 *
 * @return @p self
 */
void* cOcclusionMan_cOcclusionMan(void* vself)
{
    cOcclusionMan* self = (cOcclusionMan*)vself;
    int zeroValue = 0;
    self->m_iNumPlanes = zeroValue;
    self->m_iNumInView = zeroValue;
    self->m_iNumActive = zeroValue;
    *(int*)self = zeroValue;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", _$_13cOcclusionMan);
#ifdef SKIP_ASM
void operator_delete(int*);

/**
 * Destroys the occlusion manager, which has nothing to release: frees it (operator_delete) when g++'s delete flag bit 0
 * is set.
 */
cOcclusionMan::~cOcclusionMan()
{
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cOcclusionMan_addVisCurtain);
#ifdef SKIP_ASM
/**
 * Adds an occlusion curtain to the front of the manager's plane list and increments the count.
 *
 * @param curtain  the curtain record
 */
void cOcclusionMan::addVisCurtain(tVisCurtainRecord* curtain)
{
    curtain->next = m_pPlanes;
    curtain->prev = 0;
    if (m_pPlanes != 0) {
        m_pPlanes->prev = curtain;
    }
    m_pPlanes = curtain;
    m_iNumPlanes++;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cOcclusionMan_removeVisCurtain);
#ifdef SKIP_ASM
/**
 * Unlinks an occlusion curtain from the manager's plane list (updating the head when it was first) and decrements the
 * count.
 *
 * @param curtain  the curtain record
 */
void cOcclusionMan::removeVisCurtain(tVisCurtainRecord* curtain)
{
    if (curtain->prev != 0) {
        curtain->prev->next = curtain->next;
    }
    if (curtain->next != 0) {
        curtain->next->prev = curtain->prev;
    }
    if (curtain == m_pPlanes) {
        m_pPlanes = curtain->next;
    }
    m_iNumPlanes--;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cOcclusionMan_updateView);
#ifdef SKIP_ASM
extern "C" void cOcclusionMan_updateVisPlane(cOcclusionMan* self, t4Vector* eye, tVisCurtainRecord* frustum);
extern "C" void vu0_storeVectorAndMatrix(int index, void* v, void* m);
extern "C" void qsort(void* base, int n, int size, int (*cmp)(const void*, const void*));
extern "C" int cOcclusionMan__VisSortFunc(const void* a, const void* b);

// t4Vector with constructors (the vectors here are built and returned by value).
struct t4VectorInit : t4Vector {
    t4VectorInit() {}
    t4VectorInit(float ax, float ay, float az, float aw) { x = ax; y = ay; z = az; w = aw; }
};

// The graphics manager's ClipBoundBox (slot 93; int in render/graphicsman.h) as this caller reads it: as a bool (the
// int form compiles differently).
typedef sVEntry<bool(void*, t4Vector*, t4Vector*, t4Matrix*)> tClipBoundBoxSlot;

// PORT: PS2-only VU0 inline asm (vector times scalar).
static inline t4VectorInit vu0Scale_25C0(const t4VectorInit& v, float s)
{
    t4VectorInit r;
    int t;
    __asm__(
        "mfc1      %1, %3\n"
        "lqc2      $vf4, %2\n"
        "qmtc2.ni  %1, $vf3\n"
        "vmulx.xyzw $vf5, $vf4, $vf3x\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r), "=&r"(t)
        : "m"(v), "f"(s)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_25C0(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

// PORT: PS2-only VU0 inline asm (squared 3D distance).
static inline float vu0DistSq_25C0(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf4, %3\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf7, $vf5, $vf5\n"
        "vadday.x  ACC, $vf7, $vf7y\n"
        "vmaddz.x  $vf7, $vf6, $vf7z\n"
        "qmfc2.ni  %1, $vf7\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

static inline t4VectorInit rowPos_25C0(const t4Matrix* m)
{
    return *(const t4VectorInit*)&m->row[3];
}

static inline t4VectorInit mulT_25C0(const t4Matrix* m, const t4VectorInit& v)
{
    return t4VectorInit(v.x * m->row[0].x + v.y * m->row[0].y + v.z * m->row[0].z + v.w * m->row[0].w,
                        v.x * m->row[1].x + v.y * m->row[1].y + v.z * m->row[1].z + v.w * m->row[1].w,
                        v.x * m->row[2].x + v.y * m->row[2].y + v.z * m->row[2].z + v.w * m->row[2].w,
                        v.x * m->row[3].x + v.y * m->row[3].y + v.z * m->row[3].z + v.w * m->row[3].w);
}

static inline t4Matrix* camMtx_25C0()
{
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    return (t4Matrix*)ps2GraphicsMan->GetView();
}

/**
 * Picks the occlusion curtains for this view: the nearest two that the camera box @p bb overlaps and that face the
 * camera.
 *
 * Clears the active count (m_iNumActive) and mask (m_uiVisMask); returns when no curtains are registered
 * (m_iNumPlanes). Takes the eye position from the graphics manager's view (slot 35, cPS2GraphicsMan_GetView) and a
 * matrix from slot 43 (GetNetMatrix). Each curtain whose bounds (m_fvMin..m_fvMax) overlap @p bb, survive
 * cPS2GraphicsMan_ClipBoundBox (slot 93) and face the eye (the dot product with its plane m_fvPlaneCoeffs is not
 * positive) goes into the in-view table (m_InView, count m_iNumInView) with its squared distance to m_fvBSphere. The
 * table is sorted nearest first (qsort with cOcclusionMan__VisSortFunc); for the first two it sets the mask bit, builds
 * the clip planes (cOcclusionMan_updateVisPlane) and hands them to vu0_storeVectorAndMatrix. Likely bug: the in-view table has 32
 * entries and the count is not checked.
 *
 * @param bb  the camera's bounding box
 */
void cOcclusionMan::updateView(tBoundBox* bb)
{
    m_iNumActive = 0;
    m_uiVisMask = 0;
    if (m_iNumPlanes == 0)
        return;
    cPS2GraphicsMan* ps2GraphicsMan = GrMan;
    t4Matrix mtx = ps2GraphicsMan->GetNetMatrix();
    t4VectorInit eye = vu0Scale_25C0(rowPos_25C0(camMtx_25C0()), -1.0f);
    eye = mulT_25C0(camMtx_25C0(), eye);
    m_iNumInView = 0;
    eye.w = 1.0f;
    tVisCurtainRecord* it = m_pPlanes;
    while (it != 0) {
        tVisCurtainRecord* candidate = it;
        it = it->next;
        tBoundBox box;
        box.MinPoint.x = candidate->m_fvMin[0];
        box.MinPoint.y = candidate->m_fvMin[1];
        box.MinPoint.z = candidate->m_fvMin[2];
        box.MinPoint.w = 1.0f;
        box.MaxPoint.x = candidate->m_fvMax[0];
        box.MaxPoint.y = candidate->m_fvMax[1];
        box.MaxPoint.z = candidate->m_fvMax[2];
        box.MaxPoint.w = 1.0f;
        int ov = bb->MinPoint.x <= box.MaxPoint.x && box.MinPoint.x <= bb->MaxPoint.x
              && bb->MinPoint.y <= box.MaxPoint.y && box.MinPoint.y <= bb->MaxPoint.y
              && bb->MinPoint.z <= box.MaxPoint.z && box.MinPoint.z <= bb->MaxPoint.z;
        if (ov) {
            cPS2GraphicsMan* graphics = GrMan;
            tClipBoundBoxSlot* vt = (tClipBoundBoxSlot*)VPTR_OF(cGraphicsMan, graphics);
            bool vis = !vt[93].fn(vthis(graphics, vt[93]), &box.MinPoint, &box.MaxPoint, &mtx); // slot 93: ClipBoundBox
            if (vis) {
                if (!(0.0f < vu0Dot_25C0(eye, candidate->m_fvPlaneCoeffs))) {
                    float distSq = vu0DistSq_25C0(eye, candidate->m_fvBSphere);
                    m_InView[m_iNumInView].pVis = candidate;
                    m_InView[m_iNumInView].fDist = distSq;
                    m_iNumInView++;
                }
            }
        }
    }
    if (m_iNumInView == 0)
        return;
    qsort(m_InView, m_iNumInView, 8, cOcclusionMan__VisSortFunc);
    int n = m_iNumInView;
    if (n > 2)
        n = 2;
    m_iNumActive = n;
    for (int i = 0; i < m_iNumActive; i++) {
        m_uiVisMask |= 1 << i;
        updateVisPlane(&eye, m_InView[i].pVis);
        vu0_storeVectorAndMatrix(i, &m_InView[i].pVis->m_fvPlaneCoeffs, &m_InView[i].pVis->m_fvVisFrustum);
    }
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cOcclusionMan__VisSortFunc);
#ifdef SKIP_ASM
/**
 * Compares two in-view entries (tVisInView) by their distance (fDist), nearest first, for the sort in
 * cOcclusionMan_updateView.
 *
 * @param lhs  first entry
 * @param rhs  second entry
 * @return -1, 0 or 1
 */
extern "C" int cOcclusionMan__VisSortFunc(const void* lhs, const void* rhs)
{
    float fa = ((const tVisInView*)lhs)->fDist;
    float fb = ((const tVisInView*)rhs)->fDist;
    if (fa < fb) {
        return -1;
    }
    if (fb < fa) {
        return 1;
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("movie/movieplayer", cOcclusionMan_updateVisPlane);
#ifdef SKIP_ASM

// PORT: PS2-only VU0 inline asm (vector subtract).
static inline t4Vector vu0Sub_29B0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "lqc2      $vf4, %2\n"
        "vsub.xyzw $vf5, $vf3, $vf4\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (cross product, w = 0).
static inline t4Vector vu0Cross_29B0(const t4Vector& a, const t4Vector& b)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf4, %1\n"
        "lqc2      $vf5, %2\n"
        "vopmula.xyz ACC, $vf4, $vf5\n"
        "vopmsub.xyz $vf6, $vf5, $vf4\n"
        "vsub.w    $vf6, $vf6, $vf6\n"
        "sqc2      $vf6, %0\n"
        : "=m"(r)
        : "m"(a), "m"(b)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (normalize via rsqrt).
static inline t4Vector vu0Normalize_29B0(const t4Vector& v)
{
    t4Vector r;
    __asm__(
        "lqc2      $vf3, %1\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf3\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "vrsqrt    Q, $vf0w, $vf4x\n"
        "vwaitq\n"
        "vmulq.xyzw $vf5, $vf3, Q\n"
        "sqc2      $vf5, %0\n"
        : "=m"(r)
        : "m"(v)
        : "memory");
    return r;
}

// PORT: PS2-only VU0 inline asm (4-component dot product).
static inline float vu0Dot_29B0(const t4Vector& a, const t4Vector& b)
{
    float r;
    int t;
    __asm__(
        "lqc2      $vf3, %2\n"
        "lqc2      $vf5, %3\n"
        "vaddw.x   $vf6, $vf0, $vf0w\n"
        "vmul.xyzw $vf4, $vf3, $vf5\n"
        "vadday.x  ACC, $vf4, $vf4y\n"
        "vmaddaz.x ACC, $vf6, $vf4z\n"
        "vmaddw.x  $vf4, $vf6, $vf4w\n"
        "qmfc2.ni  %1, $vf4\n"
        "mtc1      %1, %0\n"
        : "=f"(r), "=&r"(t)
        : "m"(a), "m"(b));
    return r;
}

static inline void makePlane_29B0(tVisCurtainRecord* f, int i, const t4Vector& eye, const t4Vector& c0,
                                  const t4Vector& c1)
{
    t4Vector a;
    t4Vector b;
    t4Vector n;
    a = vu0Sub_29B0(c1, eye);
    b = vu0Sub_29B0(c0, eye);
    n = vu0Normalize_29B0(vu0Cross_29B0(a, b));
    n.w = -vu0Dot_29B0(eye, n);
    f->m_fvVisFrustum[i].m[0] = n.x;
    f->m_fvVisFrustum[i].m[1] = n.y;
    f->m_fvVisFrustum[i].m[2] = n.z;
    f->m_fvVisFrustum[i].m[3] = n.w;
}

/**
 * Builds the four clip planes of an occlusion curtain (m_fvVisFrustum) from the eye position and the curtain's corners
 * (m_fvCorners, one plane per edge: corner 0-1, 1-2, 2-3, 3-0).
 *
 * @param eyep  the eye position
 * @param frustumPtr  the curtain record
 */
void cOcclusionMan::updateVisPlane(t4Vector* eyep, tVisCurtainRecord* frustumPtr)
{
    const t4Vector& eye = *eyep;
    tVisCurtainRecord* frustum = frustumPtr;
    makePlane_29B0(frustum, 0, eye, frustum->m_fvCorners[0], frustum->m_fvCorners[1]);
    makePlane_29B0(frustum, 1, eye, frustum->m_fvCorners[1], frustum->m_fvCorners[2]);
    makePlane_29B0(frustum, 2, eye, frustum->m_fvCorners[2], frustum->m_fvCorners[3]);
    makePlane_29B0(frustum, 3, eye, frustum->m_fvCorners[3], frustum->m_fvCorners[0]);
}
#endif

