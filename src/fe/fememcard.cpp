#include "vtable.h"
#include "common.h"
#include "fe/fepopupconfirm.h"
#include "util/locale.h"
#include "ui/uitext.h"
#include "movie/mcoverlaymanager.h"
#include "fe/festatelegal.h"
#include "fe/festatememcard.h"
#include "ui/uistate.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_06cAutoload[];
extern char str_mc_messages[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cSSXApp_getMCManager(void* p);

class cUIObjK186B50 {
public:
    char pad_0x00[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int);
};

/**
 * Creates the memory-card screen "06cAutoload" and hides its "mc messages" object.
 *
 * Also calls cSSXApp_getMCManager() and discards the result; the "mc messages" object is used without a null
 * check.
 */
extern "C" void cFEStateMemCard_onCreateScreen(cFEStateMemCard* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_06cAutoload), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cSSXApp_getMCManager(SSXApp);
    ((cUIObjK186B50*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_mc_messages)))->setVisible(0);
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_onUpdate);
#ifdef SKIP_ASM
extern char str_exclam[];
extern "C" void cFEMemCard_onUpdate(void* self);
extern "C" int cMCOverlayManager_isManagerDone(void* mp);
extern "C" void cUIThing_deleteAnimation(void* obj);

class cUIObjK186BF0 {
public:
    char pad_0x000[0x8];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void setVisible(int);
};

/**
 * Runs the memory-card screen's per-frame update, then clears its busy markers once the card work is over.
 *
 * Calls the cFEMemCard update cFEMemCard_onUpdate first. When the memory-card overlay manager (cSSXApp_getMCManager) is
 * done (cMCOverlayManager_isManagerDone) or the state word at +0x1C0 is 6, it deletes the animation of the "!"
 * object, when there is one, and hides "mc messages" (slot 9 with 0). "mc messages" is used without a null check.
 */
extern "C" void cFEStateMemCard_onUpdate(cFEStateMemCard* self)
{
    cFEMemCard_onUpdate(self);
    if (cMCOverlayManager_isManagerDone(cSSXApp_getMCManager(SSXApp)) != 0 || self->mState == 6) {
        void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_exclam));
        if (obj != 0) {
            cUIThing_deleteAnimation(obj);
        }
        ((cUIObjK186BF0*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_mc_messages)))->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", func_00186C98);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char str_kT_MEMOVNotFormattedPS2[];
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);

class cUIObjK186C98 {
public:
    char pad_0x000[0x4];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual int v04(int hash);
};

class cFEMemCardStateK186C98 {
public:
    char pad_0x000[0x8];
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
    virtual void v36(void* buf, int a, int b, int c, int d, int e);
};
extern cFEMemCardStateK186C98* D_004A14B8_K186C98 __asm__("cFEStateMemCard_mInstance");


struct sLocalK186C98 {
    int v[8];
};

/**
 * Shows the memory-card state's "card not formatted" popup, with no buttons.
 *
 * Callback that the cFEStateMemCard constructor func_00186A08 plugs into the memory-card manager's
 * callbackConfirmFormat slot (0x48). It writes the selected device's number (cMCOverlayManager_getDeviceNumberString
 * with mSelectedDevice) into the localized kT_MEMOVNotFormattedPS2 text (FELocale slot 4, then sprintf1 into an
 * 800-char buffer), sets the popup id (+0x19C) of the memory-card state cFEStateMemCard_mInstance to 4 and opens it through the
 * state's getPopup (vtable slot 36, cFEMemCard_getPopup) with yes-only and no-buttons set. Popup 4 makes cFEStateMemCard_onPopupEvent leave
 * the screen (cFEStateMemCard_exitScreen).
 */
extern "C" void func_00186C98(void)
{
    unsigned short buf[800];
    sLocalK186C98 loc;
    void* mp = cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_getDeviceNumberString(mp, &loc, *(int*)((char*)mp + 0x428));
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<unsigned short*(void*, int)>* vt = *(sVEntry<unsigned short*(void*, int)>**)(locale + 4);
    unsigned short* formatText = vt[4].fn(vthis(locale, vt[4]), GetHashValue32(str_kT_MEMOVNotFormattedPS2));
    sprintf1(buf, formatText, &loc);
    cFEMemCardStateK186C98* st = D_004A14B8_K186C98;
    *(int*)((char*)st + 0x19C) = 4;
    st->v36(buf, 1, 1, 0, 0, 0);
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", func_00186D60);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBESaveInterface_getSaveProfileSize();
extern char str_kT_MEMFENotDetectedStartUpPS2[];
extern char str_kT_CMNYes[];
extern char str_kT_MEMOVRetry[];
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
// Typed view of cFEStateMemCard_mInstance (other functions in this unit declare it as void*).
extern char* D_004A14B8_86D60 __asm__("cFEStateMemCard_mInstance");

struct sVtB_86D60 { short delta; short index; void (*fn)(void*, void*, int, int, int, int, int); };
struct sVtW_86D60 { short delta; short index; void (*fn)(void*, unsigned short*, int); };
struct sVtV_86D60 { short delta; short index; void (*fn)(void*, int); };
struct sLocal_86D60 { int v[8]; };

/**
 * Shows the memory-card state's "no card detected" popup with Yes and Retry buttons.
 *
 * Callback that the cFEStateMemCard constructor func_00186A08 plugs into the manager's callbackConfirmNoCards slot
 * (0x3C). The localized kT_MEMFENotDetectedStartUpPS2 text gets the selected device's number and the card space a
 * profile needs (the card's SpaceRequired, slot 22, of cBESaveInterface_getSaveProfileSize()); the
 * cBE_getInterface(cBE_getBE(), 5) result is discarded. Sets popup id 9 (+0x19C of cFEStateMemCard_mInstance) and opens a two-button
 * popup through getPopup (slot 36); the popup's setButtonText (slot 26) labels button 0 "kT_CMNYes" and button 1
 * "kT_MEMOVRetry", and setDefaultButton (slot 25) picks button 1. In cFEMemCard_onPopupEvent, button 0 cancels and button 1 sets
 * the retry flag (+0x1EC).
 */
extern "C" void func_00186D60(void)
{
    sLocal_86D60 loc;
    unsigned short buf[800];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_getDeviceNumberString(mp, &loc, mp->mSelectedDevice);
    cBE_getInterface_Fv(cBE_getBE(), 5);
    char* memCard = (char*)mp->mpMemCard;
    sVEntry<int(void*, int)>* vt = *(sVEntry<int(void*, int)>**)memCard;
    memCard += vt[22].delta;
    int need = vt[22].fn(memCard, cBESaveInterface_getSaveProfileSize());
    unsigned short* popupText = buf;
    char* lo = (char*)SSXApp->FELocale;
    sVEntry<unsigned short*(void*, int)>* lvt = *(sVEntry<unsigned short*(void*, int)>**)(lo + 4);
    lo += lvt[4].delta;
    unsigned short* fmt = lvt[4].fn(lo, GetHashValue32(str_kT_MEMFENotDetectedStartUpPS2));
    sprintf1(popupText, fmt, &loc, need);
    cFEStateMemCard* st = (cFEStateMemCard*)D_004A14B8_86D60;
    st->mPopupType = 9;
    sVtB_86D60* svt = (sVtB_86D60*)st->mVtbl;
    svt[36].fn(vthis(st, svt[36]), popupText, 0, 0, 0, 0, 0);

    {
        char* w = *(char**)(D_004A14B8_86D60 + 0x218);
        char* lo2 = (char*)SSXApp->FELocale;
        sVtW_86D60* wvt = *(sVtW_86D60**)(w + 8);
        sVEntry<unsigned short*(void*, int)>* lvt2 = *(sVEntry<unsigned short*(void*, int)>**)(lo2 + 4);
        w += wvt[26].delta;
        lo2 += lvt2[4].delta;
        wvt[26].fn(w, lvt2[4].fn(lo2, GetHashValue32(str_kT_CMNYes)), 0);
    }
    {
        char* w = *(char**)(D_004A14B8_86D60 + 0x218);
        char* lo2 = (char*)SSXApp->FELocale;
        sVtW_86D60* wvt = *(sVtW_86D60**)(w + 8);
        sVEntry<unsigned short*(void*, int)>* lvt2 = *(sVEntry<unsigned short*(void*, int)>**)(lo2 + 4);
        w += wvt[26].delta;
        lo2 += lvt2[4].delta;
        wvt[26].fn(w, lvt2[4].fn(lo2, GetHashValue32(str_kT_MEMOVRetry)), 1);
    }
    char* w2 = *(char**)(D_004A14B8_86D60 + 0x218);
    sVtV_86D60* vvt = *(sVtV_86D60**)(w2 + 8);
    vvt[25].fn(vthis(w2, vvt[25]), 1);
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_callbackAcknowledgeCardStatusChange);
#ifdef SKIP_ASM
extern int cFEStateMemCard_mStage;
extern "C" void* cSSXApp_getMCManager(void* p);
extern "C" int cMCOverlayManager_hasCardStatusCriticalError(void* game);

/**
 * Tells the memory-card manager whether a card status change needs acknowledging during the start-up reads.
 *
 * Callback for the manager's callbackAcknowledgeCardStatusChange slot (0xB0), installed by func_00186A08. True only
 * when the stage cFEStateMemCard_mStage is 4 or more (reading the settings file or the profile) and
 * cMCOverlayManager_hasCardStatusCriticalError reports a critical error.
 *
 * @return true when the stage is at least 4 and the card has a critical error, else false
 */
extern "C" bool cFEStateMemCard_callbackAcknowledgeCardStatusChange(void)
{
    void* game = cSSXApp_getMCManager(SSXApp);
    if (cFEStateMemCard_mStage >= 4) {
        if (cMCOverlayManager_hasCardStatusCriticalError(game)) {
            return true;
        }
    }
    return false;
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_callbackLoadReady);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBESaveInterface_getSaveProfileSize();
struct sFEMemCardK187270;
extern "C" void cFEStateMemCard_terminateFreeSpaceCheck(cFEStateMemCard* self);
extern "C" void* cSSXApp_getMCManager(void* p);
extern "C" int cMCOverlayManager_hasCardStatusCriticalError(void* mp);
extern "C" int cMCOverlayManager_GetFreeSpace(void* mp, int card);
// Typed view of cFEStateMemCard_mInstance (other functions in this unit declare it as void*).
extern char* D_004A14B8_86F90 __asm__("cFEStateMemCard_mInstance");
extern int cFEStateMemCard_mStage;

struct sVtV_86F90 { short delta; short index; void (*fn)(void*, int); };
struct sVt0_86F90 { short delta; short index; int (*fn)(void*); };

/**
 * Checks that the selected card has room for the missing profile, or ends the free-space check and starts the reads.
 *
 * Callback for the manager's callbackLoadReady slot (0xA4), installed by func_00186A08; works on the memory-card state
 * cFEStateMemCard_mInstance. When the free-space check is already done (+0x234 set) it only acts from stage 4 on (cFEStateMemCard_mStage >= 4):
 * the card's GetTitleString (slot 14) writes the title of the file at mFileCursor into the screen's name buffer
 * (+0x9C). Otherwise, unless cMCOverlayManager_hasCardStatusCriticalError reports an error, it totals the space needed
 * into +0x230: when the profile is missing (bit 0 of the missing-file flags +0x22C) that is the card's SpaceRequired
 * (slot 22) of cBESaveInterface_getSaveProfileSize(), and one directory entry. If the selected device's free space
 * (cMCOverlayManager_GetFreeSpace, mDevices[].iFreeSpace) is smaller, or the card's free directory entries (slot 25, read twice) are
 * known (not -1) and fewer than needed, it sets +0x234 and moves the manager to NoFreeBlocks (23, SetState through
 * vtable slot 1). In all other cases cFEStateMemCard_terminateFreeSpaceCheck ends the check.
 *
 * PS2's GetFreeDirEntries always returns -1, so the directory-entry test never fails on this platform.
 */
extern "C" void cFEStateMemCard_callbackLoadReady(void)
{
    if (*(int*)(D_004A14B8_86F90 + 0x234) != 0) {
        if (cFEStateMemCard_mStage < 4)
            return;
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        char* memCard = (char*)mp->mpMemCard;
        sVEntry<void(void*, int, void*)>* vt = *(sVEntry<void(void*, int, void*)>**)memCard;
        vt[14].fn(memCard + vt[14].delta, mp->mFileCursor, D_004A14B8_86F90 + 0x9C);
        return;
    }
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    if (cMCOverlayManager_hasCardStatusCriticalError(mp) == 0) {
        int fail = 0;
        cBE_getInterface_Fv(cBE_getBE(), 5);
        int extra = 0;
        int avail = cMCOverlayManager_GetFreeSpace(mp, mp->mSelectedDevice);
        cFEStateMemCard* st = (cFEStateMemCard*)D_004A14B8_86F90;
        int flags = st->mMissingSavedFileTypeFlags;
        st->mTotalSpaceRequired = 0;
        if (flags & 1) {
            extra = 1;
            char* memCard = (char*)mp->mpMemCard;
            sVEntry<int(void*, int)>* vt = *(sVEntry<int(void*, int)>**)memCard;
            char* thisp = memCard + vt[22].delta;
            int profileSpace = vt[22].fn(thisp, cBESaveInterface_getSaveProfileSize());
            *(int*)(D_004A14B8_86F90 + 0x230) += profileSpace;
        }
        char* memCard;
        if (avail < *(int*)(D_004A14B8_86F90 + 0x230)
            || ((memCard = (char*)mp->mpMemCard, (*(sVt0_86F90**)memCard)[25].fn(memCard + (*(sVt0_86F90**)memCard)[25].delta) != -1)
                && (memCard = (char*)mp->mpMemCard, (*(sVt0_86F90**)memCard)[25].fn(memCard + (*(sVt0_86F90**)memCard)[25].delta) < extra)))
            fail = 1;
        if (fail) {
            *(int*)(D_004A14B8_86F90 + 0x234) = 1;
            sVtV_86F90* vt = (sVtV_86F90*)mp->mVtbl;
            vt[1].fn(((char*)mp) + vt[1].delta, 23);
            return;
        }
    }
    cFEStateMemCard_terminateFreeSpaceCheck((cFEStateMemCard*)((sFEMemCardK187270*)D_004A14B8_86F90));
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_callbackFileNotFound);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cFEMemCardStateK187148 {
public:
    char pad_0x000[0x8];
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
};
// Typed view of cFEStateMemCard_mInstance (other functions in this unit declare it as void*).
extern cFEMemCardStateK187148* D_004A14B8_K187148 __asm__("cFEStateMemCard_mInstance");

/**
 * Handles a missing file on the memory-card state: cancels the card operation and marks the screen done.
 *
 * Callback for the manager's callbackFileNotFound slot (0x38), installed by func_00186A08. Calls the state's cancel
 * (vtable slot 41, cFEMemCard_cancel, which leaves screen state 2) and then sets the screen state (+0x1C0 of cFEStateMemCard_mInstance) to
 * 6.
 */
extern "C" void cFEStateMemCard_callbackFileNotFound(void)
{
    D_004A14B8_K187148->v41();
    *(int*)((char*)D_004A14B8_K187148 + 0x1C0) = 6;
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_callbackFileChecked);
#ifdef SKIP_ASM
extern void* cFEStateMemCard_mInstance;
extern "C" void cFEMemCard_callbackFileChecked(void);
extern "C" void cFEStateMemCard_updateFreeSpaceCheckState(cFEStateMemCard* self);

/**
 * Handles a finished file check on the memory-card state: runs the base handler, then advances the free-space check.
 *
 * Callback for the manager's callbackFileChecked slot (0x94), installed by func_00186A08. Calls cFEMemCard's
 * file-checked handler cFEMemCard_callbackFileChecked, then cFEStateMemCard_updateFreeSpaceCheckState on the memory-card state cFEStateMemCard_mInstance.
 */
extern "C" void cFEStateMemCard_callbackFileChecked(void)
{
    cFEMemCard_callbackFileChecked();
    cFEStateMemCard_updateFreeSpaceCheckState((cFEStateMemCard*)cFEStateMemCard_mInstance);
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_callbackCardsChecked);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
struct sFEMemCardK1871A8 {
    char pad_0x000[0x1BC];
    int state;     // 0x1BC
    char pad_0x1C0[0x22C - 0x1C0];
    int f22C;      // 0x22C
    int pad_0x230;
    int f234;      // 0x234
};
// Typed view of cFEStateMemCard_mInstance (other functions in this unit declare it as void*).
extern sFEMemCardK1871A8* D_004A14B8_K1871A8 __asm__("cFEStateMemCard_mInstance");
extern int cFEStateMemCard_mStage;
extern "C" void* cSSXApp_getMCManager(void* p);
void cMCOverlayManager_selectDevice(void* mp, int a);
extern "C" void cMCOverlayManager_setSelectedDevice(void* mp);
extern "C" void cMCOverlayManager_readSetFileType(void* mp, int mode);

class cMoviePlayerK1871A8 {
public:
    char pad_0x000[0x748];
    virtual void SetState(int);
};

/**
 * Starts the memory-card state's file checks on the first device once the cards are checked.
 *
 * Callback for the manager's callbackCardsChecked slot (0x90), installed by func_00186A08. Selects device 0
 * (cMCOverlayManager_selectDevice, then cMCOverlayManager_setSelectedDevice) and moves the manager to InitFileCheckDevice (54, SetState
 * through vtable slot 1). Then restarts the check: stage cFEStateMemCard_mStage = 0, every file marked missing (+0x22C of
 * cFEStateMemCard_mInstance = -1), +0x234 cleared, the manager's file type set to 2 (cMCOverlayManager_readSetFileType) and the screen's file type
 * (+0x1BC) to 2.
 */
extern "C" void cFEStateMemCard_callbackCardsChecked(void)
{
    cMoviePlayerK1871A8* mp = (cMoviePlayerK1871A8*)cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_selectDevice(mp, 0);
    cMCOverlayManager_setSelectedDevice(mp);
    mp->SetState(54);
    cFEStateMemCard_mStage = 0;
    D_004A14B8_K1871A8->f22C = -1;
    D_004A14B8_K1871A8->f234 = 0;
    cMCOverlayManager_readSetFileType(mp, 2);
    D_004A14B8_K1871A8->state = 2;
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_callbackSysRecheckFile);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern int cFEStateMemCard_mStage;
struct sFEMemCardK187230 {
    char pad_0x000[0x22C];
    int f22C;      // 0x22C
    int pad_0x230;
    int f234;      // 0x234
};
// Typed view of cFEStateMemCard_mInstance (other functions in this unit declare it as void*).
extern sFEMemCardK187230* D_004A14B8_K187230 __asm__("cFEStateMemCard_mInstance");
extern "C" void* cSSXApp_getMCManager(void* p);
extern "C" void cMCOverlayManager_readSetFileType(void* mp, int mode);

/**
 * Restarts the memory-card state's free-space check when the manager rechecks the files.
 *
 * Callback for the manager's callbackSysRecheckFile slot (0xAC; cMCOverlayManager_InitiateFileCheck calls it),
 * installed by func_00186A08. Resets the stage cFEStateMemCard_mStage to 0, marks every file missing (+0x22C of cFEStateMemCard_mInstance = -1),
 * clears +0x234 and sets the manager's file type to 2 (cMCOverlayManager_readSetFileType).
 */
extern "C" void cFEStateMemCard_callbackSysRecheckFile(void)
{
    void* mp = cSSXApp_getMCManager(SSXApp);
    cFEStateMemCard_mStage = 0;
    D_004A14B8_K187230->f22C = -1;
    D_004A14B8_K187230->f234 = 0;
    cMCOverlayManager_readSetFileType(mp, 2);
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_terminateFreeSpaceCheck);
#ifdef SKIP_ASM
extern "C" void cFEStateMemCard_exitScreen(cFEStateMemCard* self);
extern "C" void cFEStateMemCard_startProfileRead(void);
extern "C" void cFEStateMemCard_startSettingsRead(void);

struct sFEMemCardK187270 {
    char pad_0x000[0x234];
    int f234;      // 0x234
    int flags;     // 0x238
};

/**
 * Ends the free-space check and starts reading the settings file or the profile from the first card, or leaves the
 * screen.
 *
 * Sets +0x234 (check done). The missing-file flags saved for the first card (+0x238) decide: settings file present (bit
 * 1 clear) selects device 0 (cMCOverlayManager_selectDevice, cMCOverlayManager_setSelectedDevice) and runs cFEStateMemCard_startSettingsRead; else profile
 * present (bit 0 clear) selects device 0 and runs cFEStateMemCard_startProfileRead; else cFEStateMemCard_exitScreen leaves the screen.
 */
extern "C" void cFEStateMemCard_terminateFreeSpaceCheck(cFEStateMemCard* self)
{
    self->mFreespaceCheckDone = 1;
    void* mp = cSSXApp_getMCManager(SSXApp);
    int flags = self->mCard1MissingSavedFileTypeFlags;
    if (!(flags & 2)) {
        cMCOverlayManager_selectDevice(mp, 0);
        cMCOverlayManager_setSelectedDevice(mp);
        cFEStateMemCard_startSettingsRead();
    } else if (!(flags & 1)) {
        cMCOverlayManager_selectDevice(mp, 0);
        cMCOverlayManager_setSelectedDevice(mp);
        cFEStateMemCard_startProfileRead();
    } else {
        cFEStateMemCard_exitScreen(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_exitScreen);
#ifdef SKIP_ASM
extern "C" void cSSXApp_startFELoad(void* p);

/**
 * Leaves the memory-card state: sets the screen state (+0x1C0) to 6 and starts the front-end load once.
 *
 * Calls cSSXApp_startFELoad(SSXApp) only while the done flag (+0x1AC) is clear, then sets it.
 */
extern "C" void cFEStateMemCard_exitScreen(cFEStateMemCard* self)
{
    self->mState = 6;
    if (self->mIsDone == 0)
        cSSXApp_startFELoad(SSXApp);
    self->mIsDone = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_callbackConfirmNoFreeBlocks);
#ifdef SKIP_ASM
extern void* cFEStateMemCard_mInstance;
extern char str_kT_MEMNoSpaceStartPS2[];
extern char str_kT_MEMOVRetry[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBESaveInterface_getSaveProfileSize();
extern "C" void* cSSXApp_getMCManager(void* p);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);



struct sVtB_00187360 {
    short delta;
    short index;
    void (*fn)(void*, int, int);
};

struct sVtV_00187360 {
    short delta;
    short index;
    void (*fn)(void*, int);
};

struct sLocal_00187360 {
    int v[8];
};

/**
 * Shows the memory-card state's "not enough space" popup, with a Retry button.
 *
 * Callback for the manager's callbackConfirmNoFreeBlocks slot (0x5C), installed by func_00186A08. Sets popup id 20
 * (+0x19C of cFEStateMemCard_mInstance) and writes the selected device's number and the profile's needed space (the card's
 * SpaceRequired, slot 22, of cBESaveInterface_getSaveProfileSize()) into the localized kT_MEMNoSpaceStartPS2 text; the
 * cBE_getInterface(cBE_getBE(), 5) result is discarded. Opens a two-button popup through getPopup (slot 36), labels
 * button 1 "kT_MEMOVRetry" (setButtonText, slot 26; button 0 keeps its default label) and makes it the default (slot
 * 25). cFEStateMemCard_onPopupEvent handles the answer: button 0 continues (cancel, then cFEStateMemCard_terminateFreeSpaceCheck), button 1 sets the retry flag.
 */
extern "C" void cFEStateMemCard_callbackConfirmNoFreeBlocks(void)
{
    unsigned short buf[800];
    sLocal_00187360 loc;
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cBE_getInterface_Fv(cBE_getBE(), 5);
    *(int*)((char*)cFEStateMemCard_mInstance + 0x19C) = 20;
    char* sf = (char*)mp->mpMemCard;
    sVEntry<int(void*, int)>* sfvt = *(sVEntry<int(void*, int)>**)sf;
    int size = sfvt[22].fn(sf + sfvt[22].delta, cBESaveInterface_getSaveProfileSize());
    cMCOverlayManager_getDeviceNumberString(mp, &loc, mp->mSelectedDevice);
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<unsigned short*(void*, int)>* vt = *(sVEntry<unsigned short*(void*, int)>**)(locale + 4);
    unsigned short* fmt = vt[4].fn(locale + vt[4].delta, GetHashValue32(str_kT_MEMNoSpaceStartPS2));
    sprintf1(buf, fmt, &loc, size);
    D_004A14B8_K186C98->v36(buf, 0, 0, 0, 0, 0);
    char* box = *(char**)((char*)cFEStateMemCard_mInstance + 0x218);
    sVtB_00187360* bvt = *(sVtB_00187360**)(box + 8);
    char* bthis = box + bvt[26].delta;
    char* buttonLocale = (char*)SSXApp->FELocale;
    sVEntry<int(void*, int)>* vt2 = *(sVEntry<int(void*, int)>**)(buttonLocale + 4);
    char* o2this = buttonLocale + vt2[4].delta;
    int id = vt2[4].fn(o2this, GetHashValue32(str_kT_MEMOVRetry));
    bvt[26].fn(bthis, id, 1);
    char* box2 = *(char**)((char*)cFEStateMemCard_mInstance + 0x218);
    sVtV_00187360* bvt2 = *(sVtV_00187360**)(box2 + 8);
    bvt2[25].fn(box2 + bvt2[25].delta, 1);
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_updateDone);
#ifdef SKIP_ASM
extern int cFEStateMemCard_mStage;
extern "C" void* cSSXApp_getMCManager(void* p);
extern "C" void cMCOverlayManager_ReturnToDevSelectScreen(void* mp);
extern "C" void cFEStateMemCard_startProfileRead(void);
extern "C" void cFEStateMemCard_exitScreen(cFEStateMemCard* self);

static inline int moviePlaying_K1874E8(void* vvmp)
{
    cMCOverlayManager* vmp = (cMCOverlayManager*)vvmp;
    cMCOverlayManager* mp = (cMCOverlayManager*)vmp;
    int r = 0;
    if (mp->mCurrDeviceStateAck == 0)
        r = mp->mCurrDeviceStateLast != 0;
    return r;
}

/**
 * Finishes the memory-card state's work: chains into the profile read after the settings read, or leaves the screen.
 *
 * The memory-card state's override of vtable slot +0x10C (cFEMemCard_updateDone in the base vtable cFEMemCard_vtbl). Does
 * nothing once the done flag (+0x1AC) is set or while the manager has an unacknowledged card state change
 * (mCurrDeviceStateAck clear and mCurrDeviceStateLast set). When the stage cFEStateMemCard_mStage is 4 (settings file read) and the
 * first card has a profile (bit 0 of +0x238 clear), it returns the manager to device selection
 * (cMCOverlayManager_ReturnToDevSelectScreen) and starts the profile read (cFEStateMemCard_startProfileRead); otherwise cFEStateMemCard_exitScreen
 * leaves the screen.
 */
extern "C" void cFEStateMemCard_updateDone(cFEStateMemCard* self)
{
    if (self->mIsDone != 0)
        return;
    void* mp = cSSXApp_getMCManager(SSXApp);
    if (moviePlaying_K1874E8(mp))
        return;
    if (cFEStateMemCard_mStage == 4 && ((self->mCard1MissingSavedFileTypeFlags ^ 1) & 1)) {
        cMCOverlayManager_ReturnToDevSelectScreen(mp);
        cFEStateMemCard_startProfileRead();
        return;
    }
    cFEStateMemCard_exitScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_onPopupEvent);
#ifdef SKIP_ASM
// FAKEMATCH: `st` is reused as the temporary for the constant 1 stored to +0x1EC (permuter); plain form 99.86%. Byte-identical and behaves like the plain form; landed marked under the 2026-11-14
// ruling (portable, behaviour-preserving steering only); replace it when an honest spelling matches.







extern "C" void cFEMemCard_onPopupEvent(void *self, void *a1, int key);
extern "C" void cMCOverlayManager_update(void *mp);
/**
 * Handles a popup answer on the memory-card state: runs cFEMemCard's handler, then the state's own popups.
 *
 * The memory-card state's override of vtable slot +0x8C. Calls cFEMemCard_onPopupEvent first, then switches on the popup id
 * (+0x19C):
 * - 21: selects device 0 (cMCOverlayManager_selectDevice, cMCOverlayManager_setSelectedDevice) and reads the settings file
 *   (cFEStateMemCard_startSettingsRead) or the profile (cFEStateMemCard_startProfileRead) of the first card as in cFEStateMemCard_terminateFreeSpaceCheck, or leaves the screen
 *   (cFEStateMemCard_exitScreen).
 * - 10 with @p key 0xF (button 0), or 4: leaves the screen (cFEStateMemCard_exitScreen).
 * - 20 (no space): button 0 (0xF) cancels (vtable slot 41), updates the manager (cMCOverlayManager_update) and ends the
 *   free-space check (cFEStateMemCard_terminateFreeSpaceCheck); button 1 (0x10) sets the retry flag (+0x1EC).
 *
 * @param popup  the popup state that sent the event (passed on to cFEMemCard_onPopupEvent)
 * @param key  the popup event: 0xF / 0x10 / 0x11 for buttons 0-2, 0x16 when the popup closes
 */
extern "C" void cFEStateMemCard_onPopupEvent(cFEStateMemCard* self, cUIState* popup, int key)
{
  cFEMemCard_onPopupEvent(self, popup, key);
  int st = self->mPopupType;
  if (st == 21)
  {
    void *mp = cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_selectDevice(mp, 0);
    cMCOverlayManager_setSelectedDevice(mp);
    int flags = self->mCard1MissingSavedFileTypeFlags;
    if (!(flags & 2))
    {
      cFEStateMemCard_startSettingsRead();
      return;
    }
    if (!(flags & 1))
    {
      cFEStateMemCard_startProfileRead();
      return;
    }
    cFEStateMemCard_exitScreen(self);
    return;
  }
  if ((st == 10) && (key == 0xF))
  {
    cFEStateMemCard_exitScreen(self);
    return;
  }
  if ((self->mPopupType) == 4)
  {
    cFEStateMemCard_exitScreen(self);
    return;
  }
  if (((key == 0xF) || (key == 0x10)) && ((self->mPopupType) == 20))
  {
    int cancel = key == 0x10;
    if (!cancel)
    {
      sVEntry<void(void *)> *vt = (sVEntry<void(void *)>*)self->mVtbl;
      vt[41].fn(vthis(self, vt[41]));
      cMCOverlayManager_update(cSSXApp_getMCManager(SSXApp));
      cFEStateMemCard_terminateFreeSpaceCheck(self);
    }
    else
    {
      st = 1;
      self->mIsRetryPressed = st;
    }
  }
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_displayStatus);
#ifdef SKIP_ASM
extern void* cFEStateMemCard_mInstance;
extern "C" void cMCOverlayManager_GetMessage(void*, unsigned short*, int);
extern "C" void cUIText_setUnicodeStringWrap(void* self, unsigned short* str);


/**
 * Shows the memory-card manager's status message in the screen's "mc messages" text while a load or a card check runs.
 *
 * The memory-card state's override of vtable slot +0xD4 (cFEMemCard_displayStatus in the base vtable). When the
 * manager's mState is a load state (31-34) or a check state (2, 3, 6, 49-54: On Tour's CheckCardsWait, CheckCards,
 * ReadingCard, InitCardsCheck..InitFileCheckDevice), it fetches the message (cMCOverlayManager_GetMessage, 800 chars),
 * word-wraps it into the object (cUIText_setUnicodeStringWrap) and shows it (slot 9 with 1); otherwise it hides the
 * object (slot 9 with 0). The object is used without a null check.
 */
extern "C" void cFEStateMemCard_displayStatus(void)
{
    void* st = cFEStateMemCard_mInstance;
    cUIText* obj = (cUIText*)cUIScreen_getObjectByHashName(*(void**)((char*)st + 0x40), GetHashValue32(str_mc_messages));
    void* mp = cSSXApp_getMCManager(SSXApp);
    int managerState = *(int*)((char*)mp + 0x130);
    if (managerState == 0x1F || managerState == 33 || managerState == 0x20 || managerState == 34 || managerState == 49 || managerState == 50 || managerState == 51
        || managerState == 53 || managerState == 54 || managerState == 52 || managerState == 2 || managerState == 3 || managerState == 6) {
        unsigned short buf[800];
        cMCOverlayManager_GetMessage(mp, buf, 800);
        cUIText_setUnicodeStringWrap(obj, buf);
        sVEntry<void(void*, int)>* vt = (sVEntry<void (void*, int)>*)obj->mVtbl;
        vt[9].fn(vthis(obj, vt[9]), 1);
    } else {
        sVEntry<void(void*, int)>* vt = (sVEntry<void (void*, int)>*)obj->mVtbl;
        vt[9].fn(vthis(obj, vt[9]), 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_startProfileRead);
#ifdef SKIP_ASM
extern void* cFEStateMemCard_mInstance;
extern int cFEStateMemCard_mStage;
extern int cFEStateMemCard_mProfileFile;
extern char str_exclam[];
extern char str_pct_s_pct_s_c[];
extern char str_at_loadanim[];
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv really takes (be, kind); bound by asm label
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBESaveInterface_getSaveProfileSize();
extern "C" void* cSSXApp_getMCManager(void* p);
extern "C" void cMCOverlayManager_readSetSize(void* mp, int a);
extern "C" void cMCOverlayManager_readSetFileType(void* mp, int mode);
extern "C" void func_002C2540(void*, void*);
// PORT: callers pass more args than the unit's declarations of these take.
extern "C" void cMCOverlayManager_getFileName_r(void* self, int mode, void* buf, int size) __asm__("cMCOverlayManager_getFileName");
extern "C" void cMCOverlayManager_getProductCode_r(void* buf, int size) __asm__("cMCOverlayManager_getProductCode");
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void* cUIScreen_getObjectByHashName(void* screen, int hash);
extern "C" void* cUIAnimationBank_getAnimationByHashName(void* bank, int hash);
extern "C" void cUIThing_playAnimation(void* obj, void* anim, int a, int b, int c);
extern "C" void cFEMemCard_SetLoadPressed(void*, int);


/**
 * Starts reading the profile from the memory card (stage 5).
 *
 * Sets the stage cFEStateMemCard_mStage to 5 and the screen state (+0x1C0 of cFEStateMemCard_mInstance) to 5 (loading), the screen's file type
 * (+0x1BC) to 0, the manager's read size to cBESaveInterface_getSaveProfileSize() (cMCOverlayManager_readSetSize) and its file type to
 * 1 (cMCOverlayManager_readSetFileType). The profile file slot cFEStateMemCard_mProfileFile goes into the manager's mFileCursor and the screen's slot
 * (+0x1C4), and the manager moves to InitFileCheckWait (53). The 16-bit string at +0x11C becomes the wildcard "%s-%s*"
 * of the product code (cMCOverlayManager_getProductCode) and the profile file name (cMCOverlayManager_getFileName with type 1). Then
 * cFEMemCard_SetLoadPressed(state, 1), +0x1A8 = 1, and "@loadanim" plays on the "exclam" object when both exist. The
 * cBE_getInterface(cBE_getBE(), 5) result is discarded; the stage and +0x1BC are stored twice.
 */
extern "C" void cFEStateMemCard_startProfileRead(void)
{
    unsigned short name[20];
    unsigned short desc[0x10];
    unsigned short fmt[8];
    int five = 5;
    cFEStateMemCard_mStage = five;
    *(int*)((char*)cFEStateMemCard_mInstance + 0x1C0) = five;
    cBE_getInterface_Fv(cBE_getBE(), 5);
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    *(int*)((char*)cFEStateMemCard_mInstance + 0x1BC) = 0;
    cFEStateMemCard_mStage = five;
    cMCOverlayManager_readSetSize(mp, cBESaveInterface_getSaveProfileSize());
    cMCOverlayManager_readSetFileType(mp, 1);
    cFEStateMemCard* memCardState = (cFEStateMemCard*)cFEStateMemCard_mInstance;
    int profileFile = cFEStateMemCard_mProfileFile;
    mp->mFileCursor = profileFile;
    memCardState->mSlot = profileFile;
    memCardState->mType = 0;
    sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)mp->mVtbl;
    vt[1].fn(vthis(mp, vt[1]), 53);
    cMCOverlayManager_getFileName_r(mp, 1, name, 20);
    func_002C2540(fmt, str_pct_s_pct_s_c);
    cMCOverlayManager_getProductCode_r(desc, 0x10);
    sprintf1((unsigned short*)((char*)cFEStateMemCard_mInstance + 0x11C), fmt, desc, name);
    cFEMemCard_SetLoadPressed(cFEStateMemCard_mInstance, 1);
    cFEStateMemCard* st = (cFEStateMemCard*)cFEStateMemCard_mInstance;
    st->mIsProcessing = 1;
    void* obj = cUIScreen_getObjectByHashName(st->mpScreen, GetHashValue32(str_exclam));
    if (obj) {
        void* bank = (char*)*(void**)((char*)cFEStateMemCard_mInstance + 0x10) + 0x50;
        void* anim = cUIAnimationBank_getAnimationByHashName(bank, GetHashValue32(str_at_loadanim));
        if (anim)
            cUIThing_playAnimation(obj, anim, 9, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_startSettingsRead);
#ifdef SKIP_ASM
extern char str_at_loadanim[];
extern "C" void* cBE_getBE();
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
// PORT: cBESaveInterface_getSaveGameSize__FPv ignores its argument; this caller passes none.
int cBESaveInterface_getSaveGameSize_K187920() __asm__("cBESaveInterface_getSaveGameSize__FPv");
extern "C" void cMCOverlayManager_readSetSize(void* mp, int a);
extern "C" void* cUIAnimationBank_getAnimationByHashName(void* bank, int hash);
extern "C" void cUIThing_playAnimation(void* obj, void* anim, int a, int b, int c);
extern "C" void cFEMemCard_SetLoadPressed(void*, int);


/**
 * Starts reading the settings file from the memory card (stage 4).
 *
 * Sets the stage cFEStateMemCard_mStage to 4, the screen state (+0x1C0 of cFEStateMemCard_mInstance) to 5 (loading), the screen's file type
 * (+0x1BC) to 2 and its slot (+0x1C4) to 0, the manager's read size to the save-game size
 * (cBESaveInterface_getSaveGameSize, cMCOverlayManager_readSetSize) and its file type to 2 (cMCOverlayManager_readSetFileType), and moves the manager to
 * InitFileCheckWait (53). Plays "@loadanim" on the "exclam" object when both exist, then
 * cFEMemCard_SetLoadPressed(state, 1) and +0x1A8 = 1. The cBE_getInterface(cBE_getBE(), 5) result is discarded.
 */
extern "C" void cFEStateMemCard_startSettingsRead(void)
{
    int two = 2;
    cFEStateMemCard_mStage = 4;
    *(int*)((char*)cFEStateMemCard_mInstance + 0x1C0) = 5;
    cBE_getInterface_Fv(cBE_getBE(), 5);
    void* mp = cSSXApp_getMCManager(SSXApp);
    cFEStateMemCard* memCardState = (cFEStateMemCard*)cFEStateMemCard_mInstance;
    memCardState->mType = two;
    memCardState->mSlot = 0;
    cMCOverlayManager_readSetSize(mp, cBESaveInterface_getSaveGameSize_K187920());
    cMCOverlayManager_readSetFileType(mp, 2);
    sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)((char*)mp + 0x748);
    vt[1].fn(vthis(mp, vt[1]), 53);
    cFEStateMemCard* st = (cFEStateMemCard*)cFEStateMemCard_mInstance;
    st->mType = two;
    void* obj = cUIScreen_getObjectByHashName(st->mpScreen, GetHashValue32(str_exclam));
    if (obj) {
        void* bank = (char*)*(void**)((char*)cFEStateMemCard_mInstance + 0x10) + 0x50;
        void* anim = cUIAnimationBank_getAnimationByHashName(bank, GetHashValue32(str_at_loadanim));
        if (anim)
            cUIThing_playAnimation(obj, anim, 9, 0, 0);
    }
    cFEMemCard_SetLoadPressed(cFEStateMemCard_mInstance, 1);
    *(int*)((char*)cFEStateMemCard_mInstance + 0x1A8) = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_updateFreeSpaceCheckState);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* p);
extern "C" int cMCOverlayManager_hasCardStatusCriticalError(void* mp);
extern "C" void cMCOverlayManager_readSetFileType(void* mp, int mode);
extern int cFEStateMemCard_mStage;
extern int cFEStateMemCard_mProfileFile;

struct sVt0_87A38 { short delta; short index; int (*fn)(void*); };
struct sVtV_87A38 { short delta; short index; void (*fn)(void*, int); };
struct sMcFlags_87A38 { unsigned b0 : 1; unsigned b1 : 1; unsigned b2 : 1; unsigned rest : 29; };

static inline int cardCall_87A38(char* o, int slot)
{
    sVt0_87A38* vt = *(sVt0_87A38**)o;
    return vt[slot].fn(vthis(o, vt[slot]));
}

static inline void mpNotify_87A38(char* mp, int msg)
{
    sVtV_87A38* vt = *(sVtV_87A38**)(mp + 0x748);
    vt[1].fn(vthis(mp, vt[1]), msg);
}

/**
 * Advances the memory-card state's free-space check by one file type.
 *
 * Sets the stage cFEStateMemCard_mStage to 3 when cMCOverlayManager_hasCardStatusCriticalError reports an error, then switches on
 * it; "present" means the card's GetNumFiles (slot 13) is not 0:
 * - 0: settings file present clears bit 1 of the missing-file flags (+0x22C); file type 1 is next (cMCOverlayManager_readSetFileType), the
 *   manager goes to InitFileCheck (51), stage 1, screen file type (+0x1BC) 0.
 * - 1: profile present clears bit 0; on device 0 the profile file slot cFEStateMemCard_mProfileFile becomes the card's GetLastSavedFile
 *   (slot 17; 0 when it returns -1); file type 0 is next, InitFileCheck (51), stage 2, screen file type 1.
 * - 2: file type 0 present clears bit 2; stage 3, then as 3.
 * - 3: on device 0 the flags are copied into +0x238 (the first card's flags); +0x234 is cleared.
 *
 * @param memCardState  the memory-card state (cFEStateMemCard_mInstance)
 */
extern "C" void cFEStateMemCard_updateFreeSpaceCheckState(cFEStateMemCard* memCardState)
{
    cFEStateMemCard* self = memCardState;
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    char* card = (char*)mp->mpMemCard;
    if (cMCOverlayManager_hasCardStatusCriticalError(mp))
        cFEStateMemCard_mStage = 3;
    switch (cFEStateMemCard_mStage) {
    case 0:
        if (cardCall_87A38(card, 13))
            (*(sMcFlags_87A38*)&self->mMissingSavedFileTypeFlags).b1 = 0;
        cMCOverlayManager_readSetFileType(mp, 1);
        mpNotify_87A38(((char*)mp), 51);
        cFEStateMemCard_mStage++;
        self->mType = 0;
        break;
    case 1:
        if (cardCall_87A38(card, 13))
            (*(sMcFlags_87A38*)&self->mMissingSavedFileTypeFlags).b0 = 0;
        if (mp->mSelectedDevice == 0) {
            int lastSavedFile = cardCall_87A38((char*)mp->mpMemCard, 17);
            cFEStateMemCard_mProfileFile = lastSavedFile;
            if (lastSavedFile == -1)
                cFEStateMemCard_mProfileFile = 0;
        }
        cMCOverlayManager_readSetFileType(mp, 0);
        mpNotify_87A38(((char*)mp), 51);
        cFEStateMemCard_mStage++;
        self->mType = 1;
        break;
    case 2:
        if (cardCall_87A38(card, 13))
            (*(sMcFlags_87A38*)&self->mMissingSavedFileTypeFlags).b2 = 0;
        cFEStateMemCard_mStage++;
    case 3:
        if (mp->mSelectedDevice == 0)
            *(sMcFlags_87A38*)&self->mCard1MissingSavedFileTypeFlags = *(sMcFlags_87A38*)&self->mMissingSavedFileTypeFlags;
        self->mFreespaceCheckDone = 0;
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcard", cFEStateMemCard_onInputBegin__FPv);
#ifdef SKIP_ASM
/**
 * Leaves controller input to the memory-card state's screens.
 *
 * The base UI update cUIState_onUpdate() hands the input on to the state's screens when this returns 0.
 *
 * @return always 0 (input not consumed)
 */
int cFEStateMemCard_onInputBegin(void* self)
{
    return 0;
}
#endif


//100%
INCLUDE_ASM("fe/fememcard", func_00187C18);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState1(void* self);
extern char cFEStateLegal_vtbl[];

/**
 * Constructs a legal-screen state: runs the cUIState constructor, clears +0x48 and stores cFEStateLegal_vtbl at +0x8.
 *
 * The cUIState(engine, parent) constructor cUIState_cUIState1 gets the caller's second and third argument registers
 * untouched (the C definition declares only @p self); +0x48 is On Tour's cFEStateLegal mDisplayTicks.
 *
 * @return @p self
 */
extern "C" void* func_00187C18(cFEStateLegal* self)
{
    cUIState_cUIState1(self);
    self->mDisplayTicks = 0;
    self->mVtbl = (void**)cFEStateLegal_vtbl;
    return ((void*)self);
}
#endif
