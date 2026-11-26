#include "vtable.h"
#include "common.h"
#include "ui/uithing.h"
#include "ui/uitext.h"
#include "ui/uimenu.h"
#include "util/locale.h"
#include "fe/popupinfo.h"
#include "fe/fepopupconfirm.h"
#include "fe/festatenethelppopup.h"
#include "movie/mcoverlaymanager.h"
#include "ui/uistate.h"
#include "ui/uiinput.h"
#include "ui/uiengine.h"
#include "fe/fememcard.h"
#include "main/ssxapp.h"
#include "util/bxmemcard.h"
#include "ui/uiscreen.h"

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackSaveSuccess);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_kT_MEMOVSaveDone[];
extern char str_kT_CMNContinue[];
void cFEMemCard_exitSave(void* self);



/**
 * Memory-card callback for a finished save: resets the screen's save state and reports the success.
 *
 * Works on the current memory-card screen cFEMemCard_mInstance and is plugged into the cMCOverlayManager slot of the
 * same name by cFEMemCard_initFEMemCard. Runs cFEMemCard_exitSave() (busy flag +0x1A8 cleared, screen state +0x1C0 back to 1)
 * and clears +0x1F0. Unless the flag at +0x208 is set it shows popup 5 (+0x19C), "kT_MEMOVSaveDone"; with the
 * flag set it calls the screen's vtable slot 40 instead (cFEMemCard_exit, screen state 6).
 *
 * The popup opens through the screen's getPopup (vtable slot 36, cFEMemCard_getPopup) with one button, which the
 * popup's setButtonText (slot 26) labels "kT_CMNContinue".
 */
extern "C" void cFEMemCard_callbackSaveSuccess(void)
{
    cFEMemCard_mInstance->exitSave();
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mIsOverwriteOK = 0;
    if (fe->mAutoSaveOn == 0) {
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
        unsigned short* str = (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVSaveDone));
        cFEMemCard* fe2 = cFEMemCard_mInstance;
        fe2->mPopupType = cFEMemCard::kPOP_SAVEDONE;
        fe2->getPopup(str, 1, 0, 0, 0, 0);
        cFELocale* locale2 = SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
        unsigned short* str2 = (unsigned short*)o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue));
        cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
        popup->setButtonText(str2, 0);
    } else {
        fe->exit();
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackReadSuccess);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_kT_MEMOVLoadDone[];
extern char str_kT_CMNContinue[];
extern "C" void cFEMemCard_SetLoadPressed(cFEMemCard*, int);



/**
 * Memory-card callback for a finished load: clears the screen's busy state and reports the success.
 *
 * Works on the current memory-card screen cFEMemCard_mInstance and is plugged into the cMCOverlayManager slot of the
 * same name by cFEMemCard_initFEMemCard. Clears the cached file name (cFEMemCard_SetLoadPressed(screen, 0)), sets +0x1B4 and clears
 * the busy flag +0x1A8. Unless the flag at +0x204 is set it shows popup 6 (+0x19C), "kT_MEMOVLoadDone"; with
 * the flag set it calls the screen's vtable slot 40 instead (cFEMemCard_exit, screen state 6).
 *
 * The popup opens through the screen's getPopup (vtable slot 36, cFEMemCard_getPopup) with one button, which the
 * popup's setButtonText (slot 26) labels "kT_CMNContinue".
 */
extern "C" void cFEMemCard_callbackReadSuccess(void)
{
    cFEMemCard_mInstance->SetLoadPressed(0);
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mIsLoadDoneOK = 1;
    fe->mIsProcessing = 0;
    if (fe->mAutoLoadOn == 0) {
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
        unsigned short* str = (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVLoadDone));
        cFEMemCard* fe2 = cFEMemCard_mInstance;
        fe2->mPopupType = cFEMemCard::kPOP_LOADDONE;
        fe2->getPopup(str, 1, 0, 0, 0, 0);
        cFELocale* locale2 = SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
        unsigned short* str2 = (unsigned short*)o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue));
        cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
        popup->setButtonText(str2, 0);
    } else {
        fe->exit();
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmOverwrite);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_Temp_String[];
extern char str_kT_MEMOVOverWriteFile[];



/**
 * Asks whether to overwrite the existing save file (popup 1).
 *
 * Callback that cFEMemCard_initFEMemCard plugs into the memory-card manager's callbackConfirmOverwrite slot (0x44); works on
 * cFEMemCard_mInstance. Allocates a 1600-byte "Temp String" buffer, has the card's GetTitleString (slot 14) write the
 * title of the file at mFileCursor, formats it into the localized kT_MEMOVOverWriteFile text, sets popup id 1 (+0x19C)
 * and opens it through getPopup (vtable slot 36, cFEMemCard_getPopup) with passUpdate set; getPopup makes button 1 the
 * default for popup 1. Frees the buffer. In cFEMemCard_onPopupEvent, button 0 sets screen state 3 and the overwrite word (+0x1F0)
 * that cFEMemCard_isOWPressed returns; button 1 cancels.
 */
extern "C" void cFEMemCard_callbackConfirmOverwrite(void)
{
    unsigned short name[40];
    unsigned short* buf = (unsigned short*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cMemCard* memCard = mp->mpMemCard;
    memCard->GetTitleString(mp->mFileCursor, (unsigned short*)name);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVOverWriteFile)), name);
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mPopupType = cFEMemCard::kPOP_OVERWRITE;
    fe->getPopup(buf, 0, 0, 0, 1, 0);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_confirmLoad);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* strcpy1(void* dst, void* src);
extern char str_Temp_String[];
extern char str_kT_POPUP200CreateEdit[];



/**
 * Asks the player to confirm the load (popup 2).
 *
 * Vtable slot +0x164 of the memory-card screens; works on cFEMemCard_mInstance. Copies the localized
 * kT_POPUP200CreateEdit text into a 2000-byte "Temp String" buffer, sets popup id 2 (+0x19C), opens a two-button popup
 * through getPopup (slot 36), makes button 1 the default (popup slot 25) and frees the buffer. In cFEMemCard_onPopupEvent, button
 * 0 starts the load (slot 45, cFEMemCard_startLoad); the other buttons set screen state 1.
 */
extern "C" void cFEMemCard_confirmLoad(void)
{
    void* buf = operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    strcpy1(buf, vt[4].fn(vthis(locale, vt[4]), GetHashValue32(str_kT_POPUP200CreateEdit)));
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mPopupType = cFEMemCard::kPOP_CONFIRMLOAD;
    fe->getPopup((unsigned short*)buf, 0, 0, 0, 0, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    popup->setDefaultButton(1);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmLoadFailed);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_Temp_String[];
extern char str_kT_MEMOVLoadFailedPS2NGC[];
extern char str_kT_CMNContinue[];

/**
 * Reports a failed load with a Continue button (popup 10).
 *
 * Callback for the memory-card manager's callbackConfirmLoadFailed slot (0x24), installed by cFEMemCard_initFEMemCard. Writes the
 * selected device's number into the localized kT_MEMOVLoadFailedPS2NGC text (2000-byte "Temp String" buffer), sets
 * popup id 10 (+0x19C of cFEMemCard_mInstance), opens a one-button popup through getPopup (slot 36), labels the button
 * "kT_CMNContinue" (popup slot 26) and frees the buffer. Continue cancels the operation (cFEMemCard_onPopupEvent).
 */
extern "C" void cFEMemCard_callbackConfirmLoadFailed(void)
{
    unsigned short name[0x100];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVLoadFailedPS2NGC)), name);
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mPopupType = cFEMemCard::kPOP_LOADFAILED;
    fe->getPopup(buf, 1, 0, 0, 0, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, popup);
    void* mthis = vthis(popup, mvt[26]);
    cFELocale* locale2 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
    void* o2this = vthis(locale2, o2vt[4]);
    unsigned short* str2 = (unsigned short*)o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
    mvt[26].fn(mthis, str2, 0);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmSaveFailed);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_Temp_String[];
extern char str_kT_MEMOVSaveFailed[];
extern char str_kT_CMNContinue[];

/**
 * Reports a failed save with a Continue button (popup 11).
 *
 * Callback for the memory-card manager's callbackConfirmSaveFailed slot (0x10), installed by cFEMemCard_initFEMemCard. Sets popup
 * id 11 (+0x19C of cFEMemCard_mInstance), writes the selected device's number into the localized kT_MEMOVSaveFailed
 * text (2000-byte "Temp String" buffer), opens a one-button popup through getPopup (slot 36), labels the button
 * "kT_CMNContinue" (popup slot 26) and frees the buffer. Continue clears the overwrite word (+0x1F0) and cancels
 * (cFEMemCard_onPopupEvent).
 */
extern "C" void cFEMemCard_callbackConfirmSaveFailed(void)
{
    unsigned short name[0x100];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cFEMemCard_mInstance->mPopupType = cFEMemCard::kPOP_SAVEFAILED;
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVSaveFailed)), name);
    cFEMemCard* fe = cFEMemCard_mInstance;
    sVEntry<void(void*, ...)>* vt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, fe);
    vt[36].fn(vthis(fe, vt[36]), buf, 1, 0, 0, 0, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, popup);
    void* mthis = vthis(popup, mvt[26]);
    cFELocale* locale2 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
    void* o2this = vthis(locale2, o2vt[4]);
    unsigned short* str2 = (unsigned short*)o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
    mvt[26].fn(mthis, str2, 0);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmNoCards);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_Temp_String[];
extern char str_kT_MEMOVNotDetectedPS2[];
extern char str_kT_MEMOVRetry[];
extern char str_kT_BTNCancel[];
extern char str_kT_CMNContinue[];

static inline void* Str_1D6D88(char* key)
{
    cFELocale* o = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, o);
    void* othis = vthis(o, ovt[4]);
    return ovt[4].fn(othis, GetHashValue32(key));
}

static inline void SetLine_1D6D88(char* key, int a)
{
    char* m = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(m + 8);
    char* mthis = m + mvt[26].delta;
    void* str = Str_1D6D88(key);
    mvt[26].fn(mthis, str, a);
}

/**
 * Reports that no memory card was detected (popup 9): Retry/Cancel when no operation is pending, else Continue.
 *
 * Callback for the memory-card manager's callbackConfirmNoCards slot (0x3C), installed by cFEMemCard_initFEMemCard. Writes the
 * selected device's number into the localized kT_MEMOVNotDetectedPS2 text (2000-byte "Temp String" buffer) and sets
 * popup id 9 (+0x19C of cFEMemCard_mInstance). Unless the manager has mSave or mDelete set or its
 * callbackIsLoadPressed() is true, it opens a two-button popup through getPopup (slot 36) with button 1 "kT_MEMOVRetry"
 * (the default) and button 0 "kT_BTNCancel"; otherwise a one-button popup with "kT_CMNContinue". Frees the buffer. In
 * cFEMemCard_onPopupEvent, button 0 cancels and button 1 sets the retry flag (+0x1EC).
 */
extern "C" void cFEMemCard_callbackConfirmNoCards(void)
{
    unsigned short name[0x100];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVNotDetectedPS2)), name);
    cFEMemCard_mInstance->mPopupType = cFEMemCard::kPOP_NOCARD;
    if (!(mp->mSave || mp->mDelete || (mp->callbackIsLoadPressed)())) {
        cFEMemCard* fe = cFEMemCard_mInstance;
        fe->getPopup(buf, 0, 0, 0, 0, 0);
        SetLine_1D6D88(str_kT_MEMOVRetry, 1);
        cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
        popup->setDefaultButton(1);
        SetLine_1D6D88(str_kT_BTNCancel, 0);
    } else {
        cFEMemCard* fe = cFEMemCard_mInstance;
        fe->getPopup(buf, 1, 0, 0, 0, 0);
        SetLine_1D6D88(str_kT_CMNContinue, 0);
    }
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackFileCorrupt);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_Temp_String[];
extern char str_kT_MEMOVGameCorruptPS2[];
extern char str_kT_CMNContinue[];


/**
 * Reports a corrupt save file with a Continue button (popup 12).
 *
 * Callback for the memory-card manager's callbackFileCorrupt slot (0x40), installed by cFEMemCard_initFEMemCard. Writes the
 * selected device's number into the localized kT_MEMOVGameCorruptPS2 text (2000-byte "Temp String" buffer), sets popup
 * id 12 (+0x19C of cFEMemCard_mInstance), opens a one-button popup through getPopup (slot 36) with passUpdate set,
 * labels the button "kT_CMNContinue" (popup slot 26) and frees the buffer. Continue cancels the operation and sets
 * screen state 1 (cFEMemCard_onPopupEvent).
 *
 * Quirk: the card's GetTitleString (slot 14) writes the title of the file at mFileCursor into a second buffer that is
 * never used.
 */
extern "C" void cFEMemCard_callbackFileCorrupt(void)
{
    unsigned short name[0x100];
    unsigned short name2[0x100];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    mp->mpMemCard->GetTitleString(mp->mFileCursor, name2);
    (cFEMemCard_mInstance)->mPopupType = cFEMemCard::kPOP_CORRUPT;
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVGameCorruptPS2)), name);
    cFEMemCard* fe = cFEMemCard_mInstance;
    sVEntry<void(void*, ...)>* vt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, fe);
    vt[36].fn(vthis(fe, vt[36]), buf, 1, 0, 0, 1, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, popup);
    void* mthis = vthis(popup, mvt[26]);
    cFELocale* buttonLocale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, buttonLocale);
    void* o2this = vthis(buttonLocale, o2vt[4]);
    unsigned short* str2 = (unsigned short*)o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
    mvt[26].fn(mthis, str2, 0);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackSaveFileCorrupt);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_kT_MEMCorruptSavePS2[];

/**
 * Reports a corrupt save file before saving (popup 13), with button 1 as the default.
 *
 * Callback for the memory-card manager's callbackSaveFileCorrupt slot (0x8C), installed by cFEMemCard_initFEMemCard; cFEMemCard_onPopupEvent
 * also calls it for button 2 of popup 12. Sets popup id 13 (+0x19C of cFEMemCard_mInstance), writes the selected
 * device's number into the localized kT_MEMCorruptSavePS2 text (2000-byte "Temp String" buffer), opens a two-button
 * popup through getPopup (slot 36) with passUpdate set, makes button 1 the default (popup slot 25) and frees the
 * buffer. In cFEMemCard_onPopupEvent, button 0 sets screen state 3 and the overwrite word (+0x1F0); the other buttons cancel.
 */
extern "C" void cFEMemCard_callbackSaveFileCorrupt(void)
{
    unsigned short name[0x100];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    cSSXApp* app = SSXApp;
    cFEMemCard_mInstance->mPopupType = cFEMemCard::kPOP_CORRUPTDELETE;
    cFELocale* locale = app->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMCorruptSavePS2)), name);
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->getPopup(buf, 0, 0, 0, 1, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    popup->setDefaultButton(1);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackFileNotFound);
#ifdef SKIP_ASM

extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_SetAction(void*, int);

/**
 * Handles a missing file: cancels the screen and moves the memory-card manager to LoadFailed (36), action 0.
 *
 * Callback for the manager's callbackFileNotFound slot (0x38), installed by cFEMemCard_initFEMemCard. Calls the screen's cancel
 * (vtable slot 41, cFEMemCard_cancel) on cFEMemCard_mInstance, clears the cancel word (+0x1E4) that cancel just set, then
 * SetState(kMCOverlay_LoadFailed) (manager vtable slot 1) and cMCOverlayManager_SetAction(manager, 0).
 */
extern "C" void cFEMemCard_callbackFileNotFound(void)
{
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->cancel();
    void* app = SSXApp;
    cFEMemCard_mInstance->mIsCancelPressed = 0;
    cMCOverlayManager* mgr = (cMCOverlayManager*)cSSXApp_getMCManager(app);
    mgr->SetState(kMCOverlay_LoadFailed);
    cMCOverlayManager_SetAction(mgr, 0);
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmFormat);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_kT_MEMFormatCardPS2[];

/**
 * Asks whether to format the memory card (popup 14), with button 1 as the default.
 *
 * Callback for the memory-card manager's callbackConfirmFormat slot (0x48), installed by cFEMemCard_initFEMemCard. Writes the
 * selected device's number into the localized kT_MEMFormatCardPS2 text (2000-byte "Temp String" buffer), sets popup id
 * 14 (+0x19C of cFEMemCard_mInstance), opens a two-button popup through getPopup (slot 36) with passUpdate set, makes
 * button 1 the default (popup slot 25) and frees the buffer. In cFEMemCard_onPopupEvent, button 0 sets +0x1A8 and the format word
 * (+0x1E8) that cFEMemCard_isFormatPressed returns; button 1 clears it and cancels.
 */
extern "C" void cFEMemCard_callbackConfirmFormat(void)
{
    unsigned short name[0x100];
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMFormatCardPS2)), name);
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mPopupType = cFEMemCard::kPOP_FORMAT;
    fe->getPopup(buf, 0, 0, 0, 1, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    popup->setDefaultButton(1);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmFormatDone);
#ifdef SKIP_ASM
extern char str_kT_MEMOVFormatComplete[];
extern char str_kT_CMNContinue[];

/**
 * Memory-card callback for a finished format: shows "kT_MEMOVFormatComplete" as popup 0x10 (+0x19C).
 *
 * Works on the current memory-card screen cFEMemCard_mInstance and is plugged into the cMCOverlayManager slot of the
 * same name by cFEMemCard_initFEMemCard. The text is copied into a 0x7D0-byte "Temp String" buffer, freed again at the
 * end. The popup opens through the screen's getPopup (vtable slot 36, cFEMemCard_getPopup) with one button, which
 * the popup's setButtonText (slot 26) labels "kT_CMNContinue". The result of its cSSXApp_getMCManager() call
 * is unused.
 */
extern "C" void cFEMemCard_callbackConfirmFormatDone(void)
{
    void* buf = operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cSSXApp_getMCManager(SSXApp);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    strcpy1(buf, vt[4].fn(vthis(locale, vt[4]), GetHashValue32(str_kT_MEMOVFormatComplete)));
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mPopupType = cFEMemCard::kPOP_FORMATDONE;
    fe->getPopup((unsigned short*)buf, 1, 0, 0, 0, 0);
    cFELocale* locale2 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* vt4 = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
    unsigned short* str = (unsigned short*)vt4[4].fn(vthis(locale2, vt4[4]), GetHashValue32(str_kT_CMNContinue));
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    popup->setButtonText(str, 0);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmFormatFail);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_kT_MEMFEFormatFailed[];
extern char str_kT_CMNContinue[];

/**
 * Reports a failed format with a Continue button (popup 17).
 *
 * Callback for the memory-card manager's callbackConfirmFormatFail slot (0x4C), installed by cFEMemCard_initFEMemCard. Writes the
 * selected device's number into the localized kT_MEMFEFormatFailed text (2000-byte "Temp String" buffer), sets popup id
 * 17 (+0x19C of cFEMemCard_mInstance), opens a one-button popup through getPopup (slot 36), labels the button
 * "kT_CMNContinue" (popup slot 26) and frees the buffer. Continue clears the format word (+0x1E8) and cancels
 * (cFEMemCard_onPopupEvent).
 */
extern "C" void cFEMemCard_callbackConfirmFormatFail(void)
{
    unsigned short name[0x100];
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMFEFormatFailed)), name);
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mPopupType = cFEMemCard::kPOP_FORMATFAIL;
    fe->getPopup(buf, 1, 0, 0, 0, 0);
    cFELocale* locale2 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
    unsigned short* str2 = (unsigned short*)o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue));
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    popup->setButtonText(str2, 0);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmDelete);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_Temp_String[];
extern char str_kT_MEMConfirmDeletePS2[];


/**
 * Memory-card callback asking to confirm a delete: a Yes/No popup naming the file under the cursor.
 *
 * Works on the current memory-card screen cFEMemCard_mInstance and is plugged into the cMCOverlayManager slot of the
 * same name by cFEMemCard_initFEMemCard. Only when the card (cMCOverlayManager::mpMemCard) holds a save file (its vtable
 * slot 13 counts them): sets popup kPOP_DELETE (+0x19C), formats "kT_MEMConfirmDeletePS2" with the name of file
 * mFileCursor (card slot 14) into a "Temp String" buffer, opens a two-button popup that passes updates
 * through and makes button 1 (No) the default. With no files it clears +0x1A8 and +0x1E0 when the screen
 * state (+0x1C0) is kDELETING.
 */
extern "C" void cFEMemCard_callbackConfirmDelete(void)
{
    unsigned short name[0x100];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cMemCard* sf = mp->mpMemCard;
    if (sf->GetNumFiles() > 0) {
        unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
        cFEMemCard_mInstance->mPopupType = cFEMemCard::kPOP_DELETE;
        cMemCard* sf2 = mp->mpMemCard;
        sf2->GetTitleString(mp->mFileCursor, (unsigned short*)name);
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
        sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMConfirmDeletePS2)), name);
        cFEMemCard* fe = cFEMemCard_mInstance;
        fe->getPopup(buf, 0, 0, 0, 1, 0);
        cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
        popup->setDefaultButton(1);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    } else {
        cFEMemCard* fe = cFEMemCard_mInstance;
        if (fe->mState == cFEMemCard::kDELETING) {
            fe->mIsProcessing = 0;
            fe->mIsDeletePressed = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmInvalidCard);
#ifdef SKIP_ASM
extern char str_kT_MEMOVNotDetectedPS2[];
extern char str_kT_MEMOVRetry[];
extern char str_kT_BTNCancel[];

/**
 * Reports an unusable memory card with Retry and Cancel buttons (popup 18).
 *
 * Callback for the memory-card manager's callbackConfirmInvalidCard slot (0x58), installed by cFEMemCard_initFEMemCard. Writes the
 * selected device's number into the localized kT_MEMOVNotDetectedPS2 text (the same text as cFEMemCard_callbackConfirmNoCards; 2000-byte
 * "Temp String" buffer), sets popup id 18 (+0x19C of cFEMemCard_mInstance), opens a two-button popup through getPopup
 * (slot 36), labels button 0 "kT_MEMOVRetry" and button 1 "kT_BTNCancel" (popup slot 26) and frees the buffer. In
 * cFEMemCard_onPopupEvent, button 0 sets the retry flag (+0x1EC) and the other buttons cancel.
 */
extern "C" void cFEMemCard_callbackConfirmInvalidCard(void)
{
    unsigned short name[0x100];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVNotDetectedPS2)), name);
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mPopupType = cFEMemCard::kPOP_INVALID;
    fe->getPopup(buf, 0, 0, 0, 0, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, popup);
    void* mthis = vthis(popup, mvt[26]);
    cFELocale* locale2 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
    void* o2this = vthis(locale2, o2vt[4]);
    unsigned short* str2 = (unsigned short*)o2vt[4].fn(o2this, GetHashValue32(str_kT_MEMOVRetry));
    mvt[26].fn(mthis, str2, 0);
    cFEPopupConfirm* popup3 = cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* m3vt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, popup3);
    void* m3this = vthis(popup3, m3vt[26]);
    cFELocale* locale3 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o3vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale3);
    void* o3this = vthis(locale3, o3vt[4]);
    void* str3 = o3vt[4].fn(o3this, GetHashValue32(str_kT_BTNCancel));
    m3vt[26].fn(m3this, str3, 1);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmSaveTooBig);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_Temp_String[];
extern char str_kT_MEMFETooBigPS2[];
extern char str_kT_CMNContinue[];


/**
 * Reports that the save does not fit on the memory card (popup 19), with a Continue button.
 *
 * Callback for the memory-card manager's callbackConfirmSaveTooBig slot (0x60), installed by cFEMemCard_initFEMemCard. Sets popup
 * id 19 (+0x19C of cFEMemCard_mInstance), writes the selected device's number and the card's SpaceRequired1 (slot 23:
 * the space the current file size needs) into the localized kT_MEMFETooBigPS2 text (2000-byte "Temp String" buffer),
 * opens a one-button popup through getPopup (slot 36) with layer offset 3 and passUpdate set, labels the button
 * "kT_CMNContinue" (popup slot 26) and frees the buffer. In cFEMemCard_onPopupEvent, button 0 clears the overwrite word (+0x1F0)
 * and cancels.
 */
extern "C" void cFEMemCard_callbackConfirmSaveTooBig(void)
{
    unsigned short name[0x100];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cFEMemCard_mInstance->mPopupType = cFEMemCard::kPOP_FREESPACE;
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    unsigned short* fmt = (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMFETooBigPS2));
    int n = mp->mpMemCard->SpaceRequired1();
    sprintf1(buf, fmt, name, n);
    cFEMemCard* fe = cFEMemCard_mInstance;
    sVEntry<void(void*, ...)>* vt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, fe);
    vt[36].fn(vthis(fe, vt[36]), buf, 1, 0, 3, 1, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, popup);
    void* mthis = vthis(popup, mvt[26]);
    cFELocale* locale2 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
    void* o2this = vthis(locale2, o2vt[4]);
    unsigned short* str2 = (unsigned short*)o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
    mvt[26].fn(mthis, str2, 0);
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackConfirmNoFreeBlocks);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_kT_MEMFETooBigPS2[];
extern char str_kT_CMNContinue[];


/**
 * Reports that the memory card has too little free space (popup 19), with a Continue button.
 *
 * Callback for the memory-card manager's callbackConfirmNoFreeBlocks slot (0x5C), installed by cFEMemCard_initFEMemCard. Same
 * popup as cFEMemCard_callbackConfirmSaveTooBig (id 19, kT_MEMFETooBigPS2 with the device number and the card's SpaceRequired1, slot 23; layer
 * offset 3, passUpdate set, "kT_CMNContinue"), but formatted into an 800-char stack buffer instead of a heap one.
 */
extern "C" void cFEMemCard_callbackConfirmNoFreeBlocks(void)
{
    unsigned short buf[800];
    unsigned short name[16];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cFEMemCard_mInstance->mPopupType = cFEMemCard::kPOP_FREESPACE;
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    unsigned short* fmt = (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMFETooBigPS2));
    int n = mp->mpMemCard->SpaceRequired1();
    sprintf1(buf, fmt, name, n);
    cFEMemCard* fe = cFEMemCard_mInstance;
    sVEntry<void(void*, ...)>* vt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, fe);
    vt[36].fn(vthis(fe, vt[36]), buf, 1, 0, 3, 1, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, popup);
    void* mthis = vthis(popup, mvt[26]);
    cFELocale* locale2 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
    void* o2this = vthis(locale2, o2vt[4]);
    unsigned short* str2 = (unsigned short*)o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
    mvt[26].fn(mthis, str2, 0);
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackFileExists);
#ifdef SKIP_ASM
extern char str_kT_FENameAlreadyExists[];
extern char str_kT_CMNContinue[];

/**
 * Memory-card callback for a name clash: shows popup 0x17, "kT_FENameAlreadyExists", with a Continue button.
 *
 * Works on the current memory-card screen cFEMemCard_mInstance and is plugged into the cMCOverlayManager slot of the
 * same name by cFEMemCard_initFEMemCard. The popup opens through the screen's getPopup (vtable slot 36, cFEMemCard_getPopup)
 * with one button, which the popup's setButtonText (slot 26) labels "kT_CMNContinue". Then sets the screen
 * state (+0x1C0) to 1.
 */
extern "C" void cFEMemCard_callbackFileExists(void)
{
    cSSXApp* app = SSXApp;
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mPopupType = cFEMemCard::kPOP_FILEEXISTS;
    sVEntry<void(void*, ...)>* vt = (sVEntry<void (void*, ...)>*)VPTR_OF(cListNode, fe);
    cFELocale* locale = app->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    vt[36].fn(vthis(fe, vt[36]), ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_FENameAlreadyExists)), 1, 0, 0, 0, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, popup);
    cFELocale* locale2 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
    mvt[26].fn(vthis(popup, mvt[26]), o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue)), 0);
    cFEMemCard_mInstance->mState = cFEMemCard::kCHECKING;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackCardChanged);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_Temp_String[];
extern char str_kT_MEMDifferentMemcardPS2[];
extern char str_kT_CMNContinue[];

/**
 * Reports that a different memory card was inserted (popup 24), with a Continue button.
 *
 * Callback for the memory-card manager's callbackCardChanged slot (0xA8), installed by cFEMemCard_initFEMemCard. Writes the
 * selected device's number into the localized kT_MEMDifferentMemcardPS2 text (1600-byte "Temp String" buffer), sets
 * popup id 24 (+0x19C of cFEMemCard_mInstance), opens a one-button popup through getPopup (slot 36), labels the button
 * "kT_CMNContinue" (popup slot 26), sets the screen state (+0x1C0) to 1 and frees the buffer. Continue cancels
 * (cFEMemCard_onPopupEvent).
 */
extern "C" void cFEMemCard_callbackCardChanged(void)
{
    unsigned short name[0x100];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    unsigned short* buf = (unsigned short*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMDifferentMemcardPS2)), name);
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mPopupType = cFEMemCard::kPOP_CARDCHANGED;
    fe->getPopup(buf, 1, 0, 0, 0, 0);
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = (sVEntry<void(void*, ...)>*)VPTR_OF(cListNode, popup);
    void* mthis = vthis(popup, mvt[26]);
    cFELocale* locale2 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
    void* o2this = vthis(locale2, o2vt[4]);
    unsigned short* str2 = (unsigned short*)o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
    mvt[26].fn(mthis, str2, 0);
    cFEMemCard_mInstance->mState = cFEMemCard::kCHECKING;
    if (buf != 0) {
        cMemMan_free(buf);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackDeleteDone);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* strcpy1(void* dst, void* src);
extern char str_Temp_String[];
extern char str_kT_MEMDeleteSuccess[];
extern char str_kT_CMNContinue[];

/**
 * Memory-card callback for a finished delete: reports the success and resets the screen state.
 *
 * Works on the current memory-card screen cFEMemCard_mInstance and is plugged into the cMCOverlayManager slot of the
 * same name by cFEMemCard_initFEMemCard. Unless the flag at +0x204 is set it copies "kT_MEMDeleteSuccess" into a "Temp
 * String" buffer and shows it as popup kPOP_DELETEDONE (+0x19C). Then a screen state (+0x1C0) of kDELETING is set
 * back to kCHECKING.
 *
 * The popup opens through the screen's getPopup (vtable slot 36, cFEMemCard_getPopup) with one button, which the
 * popup's setButtonText (slot 26) labels "kT_CMNContinue".
 *
 * @return 1, always
 */
extern "C" int cFEMemCard_callbackDeleteDone(void)
{
    if (cFEMemCard_mInstance->mAutoLoadOn == 0) {
        void* buf = operator_new_tag(2000, str_Temp_String, 0x100, 0);
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
        strcpy1(buf, vt[4].fn(vthis(locale, vt[4]), GetHashValue32(str_kT_MEMDeleteSuccess)));
        cFEMemCard* fe = cFEMemCard_mInstance;
        fe->mPopupType = cFEMemCard::kPOP_DELETEDONE;
        fe->getPopup((unsigned short*)buf, 1, 0, 0, 0, 0);
        cFELocale* locale2 = SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
        unsigned short* str2 = (unsigned short*)o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue));
        cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
        popup->setButtonText(str2, 0);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    }
    cFEMemCard* fe = cFEMemCard_mInstance;
    if (fe->mState == cFEMemCard::kDELETING) {
        fe->mState = cFEMemCard::kCHECKING;
    }
    return 1;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackDeleteFailed);
#ifdef SKIP_ASM
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern char str_kT_MEMOVDeleteFailedError[];
extern char str_kT_CMNContinue[];

/**
 * Reports a failed delete with a Continue button (popup 8).
 *
 * Callback for the memory-card manager's callbackDeleteFailed slot (0x34), installed by cFEMemCard_initFEMemCard. Writes the
 * selected device's number into the localized kT_MEMOVDeleteFailedError text (2000-byte "Temp String" buffer), sets
 * popup id 8 (+0x19C of cFEMemCard_mInstance), opens a one-button popup through getPopup (slot 36), labels the button
 * "kT_CMNContinue" (popup slot 26) and frees the buffer. Continue cancels (cFEMemCard_onPopupEvent).
 *
 * @return always 0
 */
extern "C" int cFEMemCard_callbackDeleteFailed(void)
{
    unsigned short name[0x100];
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_getDeviceNumberString(mp, name, mp->mSelectedDevice);
    cFELocale* locale = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVDeleteFailedError)), name);
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mPopupType = cFEMemCard::kPOP_DELETEFAIL;
    fe->getPopup(buf, 1, 0, 0, 0, 0);
    cFELocale* locale2 = SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale2);
    unsigned short* str2 = (unsigned short*)o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue));
    cFEPopupConfirm* popup = cFEMemCard_mInstance->mStatusPopup;
    popup->setButtonText(str2, 0);
    if (buf != 0) {
        cMemMan_free(buf);
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackCardsChecked);
#ifdef SKIP_ASM
void cMCOverlayManager_selectDevice(void* mp, int a);
extern "C" void cMCOverlayManager_setSelectedDevice(void* mp);
extern "C" void cMCOverlayManager_OnHighlightNextDevice(void* mp);

/**
 * Moves the card check to the next device, or picks the device to use once every device is checked.
 *
 * Callback for the memory-card manager's callbackCardsChecked slot (0x90), installed by cFEMemCard_initFEMemCard. While
 * mSelectedDevice + 1 is below mMaxDevices it selects that device (cMCOverlayManager_selectDevice) and goes to InitCardsCheck (49,
 * SetState through vtable slot 1). Otherwise it selects device 0; when the card's IsAvailable (slot 52) says device 0
 * has no card, an auto-save screen (+0x208 of cFEMemCard_mInstance set) gets screen state 6 and the manager
 * CheckCardsWait (2), and any other screen moves the highlight to the next device with a card
 * (cMCOverlayManager_OnHighlightNextDevice). Then cMCOverlayManager_setSelectedDevice and InitFileCheckDevice (54).
 */
extern "C" void cFEMemCard_callbackCardsChecked(void)
{
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    int next = mp->mSelectedDevice + 1;
    if (next < mp->mMaxDevices) {
        cMCOverlayManager_selectDevice(mp, next);
        mp->SetState(kMCOverlay_InitCardsCheck);
        return;
    }
    cMCOverlayManager_selectDevice(mp, 0);
    if (mp->mpMemCard->IsAvailable(0) == 0) {
        cFEMemCard* fe = cFEMemCard_mInstance;
        if (fe->mAutoSaveOn != 0) {
            fe->mState = cFEMemCard::kDONE;
            mp->SetState(kMCOverlay_CheckCardsWait);
            return;
        }
        cMCOverlayManager_OnHighlightNextDevice(mp);
    }
    cMCOverlayManager_setSelectedDevice(mp);
    mp->SetState(kMCOverlay_InitFileCheckDevice);
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackDisplayNoCards);
#ifdef SKIP_ASM
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIText_setUnicodeStringWrap(void* self, unsigned short* str);
extern char str_text_box_message[];
extern char str_kT_MEMNoCardsPS2[];


/**
 * Shows the "no memory cards" message in the screen's "text box message" object and switches to display state
 * kDISPLAY_OFF.
 *
 * Callback for the memory-card manager's callbackDisplayNoCards slot (0x9C), installed by cFEMemCard_initFEMemCard; many
 * memory-card screens also call it directly. Sets the screen state (+0x1C0 of cFEMemCard_mInstance) to kCHECKING;
 * when the display state (+0x214) is not kDISPLAY_OFF yet it records kDISPLAY_OFF there and calls triggerDisplayState
 * (vtable slot 35, cFEMemCard_triggerDisplayState) with it. When the screen has a "text box message" object, the
 * localized kT_MEMNoCardsPS2 text is copied into a 2000-byte "Temp String" buffer, word-wrapped into the object
 * (cUIText_setUnicodeStringWrap) and the object is shown (slot 9 with 1).
 */
extern "C" void cFEMemCard_callbackDisplayNoCards(void)
{
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mState = cFEMemCard::kCHECKING;
    if (fe->mScreenStatus != cFEMemCard::kDISPLAY_OFF) {
        fe->mScreenStatus = cFEMemCard::kDISPLAY_OFF;
        fe->triggerDisplayState(cFEMemCard::kDISPLAY_OFF);
    }
    cUIThing* obj = cUIScreen_getObjectByHashName(cFEMemCard_mInstance->mpScreen, GetHashValue32(str_text_box_message));
    if (obj) {
        unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
        strcpy1(buf, ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMNoCardsPS2)));
        cUIText_setUnicodeStringWrap(obj, buf);
        obj->setVisible(1);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackDisplayNoFiles);
#ifdef SKIP_ASM


/**
 * Switches the memory-card screen to display state kDISPLAY_FILES when no files were found, while the screen is still
 * starting.
 *
 * Callback for the memory-card manager's callbackDisplayNoFiles slot (0x98), installed by cFEMemCard_initFEMemCard. Only when the
 * screen state (+0x1C0 of cFEMemCard_mInstance) is kSTART and the display state (+0x214) is not already
 * kDISPLAY_FILES: records kDISPLAY_FILES and calls triggerDisplayState (vtable slot 35) with it.
 */
extern "C" void cFEMemCard_callbackDisplayNoFiles(void)
{
    cFEMemCard* fe = cFEMemCard_mInstance;
    if (fe->mScreenStatus != cFEMemCard::kDISPLAY_FILES && fe->mState == 0) {
        fe->mScreenStatus = cFEMemCard::kDISPLAY_FILES;
        fe->triggerDisplayState(cFEMemCard::kDISPLAY_FILES);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackFileChecked);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void* strcpy1(void* dst, void* src);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIText_setUnicodeStringWrap(void* self, unsigned short* str);
extern char str_Temp_String[];
extern char str_text_box_message[];
extern char str_kT_MEMNoCardsPS2[];


static inline void SetMode_001D83A8(char* vfe, int mode)
{
    cFEMemCard* fe = (cFEMemCard*)vfe;
    if (fe->mScreenStatus != mode) {
        fe->mScreenStatus = mode;
        fe->triggerDisplayState(mode);
    }
}

/**
 * Handles a finished file check: back to the checking state, then display state 3 with a card, or 1 and the "no cards"
 * text without.
 *
 * Callback for the memory-card manager's callbackFileChecked slot (0x94), installed by cFEMemCard_initFEMemCard. Sets the screen
 * state (+0x1C0 of cFEMemCard_mInstance) to 1 and moves the manager to CheckCardsWait (2, SetState through vtable slot
 * 1). When the card's slot 53 (IsAvailable for its own device) is true it switches to display state 3 (+0x214 and
 * triggerDisplayState, vtable slot 35, when it changes); otherwise to display state 1 and, when the screen has a "text
 * box message" object, shows kT_MEMNoCardsPS2 in it as cFEMemCard_callbackDisplayNoCards does.
 */
extern "C" void cFEMemCard_callbackFileChecked(void)
{
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    cFEMemCard_mInstance->mState = cFEMemCard::kCHECKING;
    mp->SetState(kMCOverlay_CheckCardsWait);
    cMemCard* sf = mp->mpMemCard;
    int busy = sf->IsAvailable1();
    if (busy) {
        SetMode_001D83A8((char*)cFEMemCard_mInstance, 3);
    } else {
        SetMode_001D83A8((char*)cFEMemCard_mInstance, 1);
        cUIThing* obj = cUIScreen_getObjectByHashName(cFEMemCard_mInstance->mpScreen,
                                                      GetHashValue32(str_text_box_message));
        if (obj) {
            unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
            cFELocale* locale = SSXApp->FELocale;
            sVEntry<void*(void*, ...)>* ovt = (sVEntry<void*(void*, ...)>*)VPTR_OF(cUILocaleInterface, locale);
            strcpy1(buf, ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMNoCardsPS2)));
            cUIText_setUnicodeStringWrap(obj, buf);
            obj->setVisible(1);
            if (buf != 0) {
                cMemMan_free(buf);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_getPopup);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern char str_cFEPopupConfirm[];
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEPopupConfirm_cFEPopupConfirm_8530(void* self, void* engine, void* owner, unsigned short* text,
    int yesOnly, int noButtons, int layerOffset, int passUpdate, int snowFlake)
    __asm__("cFEPopupConfirm_cFEPopupConfirm");
extern "C" void cUIStateStack_pushPopup(void* list, void* item);


/**
 * Opens the memory-card screen's confirmation popup, or re-initialises the one already open.
 *
 * Vtable slot +0x124 of the memory-card screens. Clears the status-popup flag (+0x210) unless the popup id (+0x19C of
 * cFEMemCard_mInstance) is kPOP_STATUS. An existing popup (+0x218) gets initPopup (popup vtable slot 28) with the six
 * arguments; otherwise a cFEPopupConfirm is allocated (cMemMan_alloc(3068, "cFEPopupConfirm", 0x100, 0)), constructed
 * with the instance's engine (+0x10) and the instance as parent, stored in +0x218 and pushed onto the engine's state
 * stack (+0x18, cUIStateStack_pushPopup). For popup id kPOP_OVERWRITE it makes button 1 the default (popup slot 25).
 *
 * Reads the popup id through cFEMemCard_mInstance but writes +0x210 and +0x218 through @p self_; callers pass the
 * instance.
 *
 * @param displayText  the 16-bit message text
 * @param yesOnly  nonzero for a single (yes) button
 * @param noButtons  nonzero for no buttons
 * @param layerOffset  layer offset of the popup
 * @param passUpdate  passUpdate: nonzero lets updates pass to the screen below
 * @param snowFlake  nonzero for the snowflake variant
 */
void cFEMemCard::getPopup(unsigned short*displayText, int yesOnly, int noButtons, int layerOffset, int passUpdate,
                          int snowFlake)
{
    if (cFEMemCard_mInstance->mPopupType != cFEMemCard::kPOP_STATUS) {
        mIsStatusPopupOn = 0;
    }
    cFEPopupConfirm* existingPopup = mStatusPopup;
    if (existingPopup != 0) {
        existingPopup->initPopup(displayText, yesOnly, noButtons, layerOffset, passUpdate, snowFlake);
    } else {
        void* mem = cMemMan_alloc(3068, str_cFEPopupConfirm, 0x100, 0);
        void* engine = cFEMemCard_mInstance->mpEngine;
        cFEPopupConfirm* n = (cFEPopupConfirm*)cFEPopupConfirm_cFEPopupConfirm_8530(mem, engine, cFEMemCard_mInstance,
            displayText, yesOnly, noButtons, layerOffset, passUpdate, snowFlake);
        mStatusPopup = n;
        cUIStateStack_pushPopup(&mpEngine->mStateStack, n);
    }
    if (cFEMemCard_mInstance->mPopupType == cFEMemCard::kPOP_OVERWRITE) {
        mStatusPopup->setDefaultButton(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_exitDelete);
#ifdef SKIP_ASM

/**
 * Leaves the delete flow: screen state kDELETING goes back to kCHECKING, and +0x1A8 and the delete word (+0x1E0)
 * are cleared.
 *
 * Vtable slot +0x174 of the memory-card screens; works on cFEMemCard_mInstance. +0x1E0 is the word
 * cFEMemCard_isDeletePressed returns.
 */
extern "C" void cFEMemCard_exitDelete(void)
{
    if (cFEMemCard_mInstance->mState == cFEMemCard::kDELETING) {
        cFEMemCard_mInstance->mState = cFEMemCard::kCHECKING;
    }
    cFEMemCard* fe = cFEMemCard_mInstance;
    fe->mIsProcessing = 0;
    fe->mIsDeletePressed = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_exit__FPv);
#ifdef SKIP_ASM
/**
 * Sets the memory-card screen's state (+0x1C0) to 6.
 */
void cFEMemCard::exit()
{
    mState = cFEMemCard::kDONE;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_startLoad);
#ifdef SKIP_ASM

extern "C" void cFEMemCard_SetLoadPressed(cFEMemCard*, int);

/**
 * Starts the load: records the load request, sets +0x1A8 and screen state 5 (loading), and shows the status popup.
 *
 * Calls cFEMemCard_SetLoadPressed(@p self, 1), then vtable slot 47 (cFEMemCard_displayStatusPopup).
 */
void cFEMemCard::startLoad()
{
    SetLoadPressed(1);
    mIsProcessing = 1;
    mState = cFEMemCard::kLOADING;
    displayStatusPopup();
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_onPopupEvent);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cFEMemCard_SetLoadPressed(cFEMemCard*, int);
extern "C" void cFEMemCard_callbackSaveFileCorrupt(void);
extern "C" void cUIStateStack_pop(void* list, int a1);


#define VCALL_K1D8700(obj, idx) ((*(sVEntry<void(void*)>**)((char*)(obj) + 8))[idx].fn((char*)(obj) + (*(sVEntry<void(void*)>**)((char*)(obj) + 8))[idx].delta))

static inline void notifyK1D8700(char* vself, char* mp)
{
    cFEMemCard* self = (cFEMemCard*)vself;
    if (self->mAutoLoadOn == 1) {
        *(int*)(mp + 0x440) = self->mAutoLoadOn;
        VCALL_K1D8700(self, 40);
    }
}

/**
 * Handles a button on the memory-card screen's popup, by popup id (+0x19C).
 *
 * Buttons kONSCREENPOPUPOPTION0 .. kONSCREENPOPUPOPTION2 are choices 0, 1 and 2. "Cancel" is vtable slot 41
 * (cFEMemCard_cancel); "finish" means that on an auto-load screen (+0x204 is 1) the manager's mCurrDeviceStateAck is
 * set to 1 and vtable slot 40 (cFEMemCard_exit, state kDONE) runs. Per popup:
 * - kPOP_CONFIRMLOAD: choice 0 starts the load (slot 45), else screen state (+0x1C0) kCHECKING.
 * - kPOP_FREESPACE: choice 0 clears the overwrite word (+0x1F0), cancels, finishes; else sets the retry word (+0x1EC).
 * - kPOP_OVERWRITE and kPOP_CORRUPTDELETE: choice 0 sets state kSAVING and +0x1F0 (overwrite), else cancel and finish.
 * - kPOP_NOCARD: choice 0 cancels and finishes, else retry. kPOP_INVALID: choice 0 retries, else cancel and finish.
 * - kPOP_LOADFAILED: choice 0 cancels and finishes. kPOP_SAVEFAILED: clears +0x1F0, cancels, finishes.
 * - kPOP_MESSAGE, kPOP_SAVEDONE, kPOP_LOADDONE: slot 40.
 * - kPOP_DELETE: clears +0x1B0; choice 0 sets +0x1A8 and the delete word (+0x1E0), else cancel and finish.
 * - kPOP_CORRUPT: choice 0 cancels, sets state kCHECKING, finishes; choice 1 retries; choice 2 opens popup
 *   kPOP_CORRUPTDELETE (cFEMemCard_callbackSaveFileCorrupt) and returns at once.
 * - kPOP_DELETEDONE: clears +0x1A8 and +0x1E0, cancels when the load word (+0x1F4) is set, sets +0x1A8 again when the
 *   save word (+0x1DC) is set, finishes.
 * - kPOP_FORMATDONE: clears the format word (+0x1E8) and the load request (cFEMemCard_SetLoadPressed 0), sets +0x1A8
 *   only when the save word (+0x1DC) is set, finishes.
 * - kPOP_FORMATFAIL: clears +0x1E8 and cancels. kPOP_FORMAT and kPOP_FORMATNGC: clear +0x1B0; choice 0 sets +0x1A8
 *   and +0x1E8, choice 1 clears +0x1E8, cancels, finishes.
 * - kPOP_DELETEFAIL, kPOP_FILEEXISTS, kPOP_CARDCHANGED: cancel.
 *
 * Unless the popup id is kPOP_STATUS, cUIStateStack_pop then flags the top state of the engine's state stack (+0x18) to
 * close; +0x210 is cleared. kONPOPUPDESTROY (popup closed) clears the words +0x1A4 and +0x1A0 when +0x1A0 is set;
 * otherwise it clears +0x210 (popup id kPOP_STATUS only) and the instance's popup pointer (+0x218).
 *
 * @param self_  the memory-card screen
 * @param popup  the popup state that sent the event (unused)
 * @param button  the popup event: kONSCREENPOPUPOPTION0 .. kONSCREENPOPUPOPTION2 for buttons 0-2, kONPOPUPDESTROY
 *                when the popup closes
 */
extern "C" void cFEMemCard_onPopupEvent(cFEMemCard* self_, int popup, int button)
{
    cFEMemCard* self = (cFEMemCard*)self_;
    if (button >= cUIState::kONSCREENPOPUPOPTION0 && button <= cUIState::kONSCREENPOPUPOPTION2) {
        int choice = 0;
        if (button == cUIState::kONSCREENPOPUPOPTION1) {
            choice = 1;
        } else if (button == cUIState::kONSCREENPOPUPOPTION2) {
            choice = 2;
        }
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        switch (self->mPopupType) {
        case cFEMemCard::kPOP_CONFIRMLOAD:
            if (choice == 0) {
                VCALL_K1D8700(self, 45);
            } else {
                self->mState = cFEMemCard::kCHECKING;
            }
            break;
        case cFEMemCard::kPOP_FREESPACE:
            if (choice == 0) {
                self->mIsOverwriteOK = 0;
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            } else {
                self->mIsRetryPressed = 1;
            }
            break;
        case cFEMemCard::kPOP_OVERWRITE:
            if (choice != 0) {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            } else {
                self->mState = cFEMemCard::kSAVING;
                self->mIsOverwriteOK = 1;
            }
            break;
        case cFEMemCard::kPOP_NOCARD:
            if (choice == 0) {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            } else {
                self->mIsRetryPressed = 1;
            }
            break;
        case cFEMemCard::kPOP_INVALID:
            if (choice == 0) {
                self->mIsRetryPressed = 1;
            } else {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            }
            break;
        case cFEMemCard::kPOP_LOADFAILED:
            if (choice == 0) {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            }
            break;
        case cFEMemCard::kPOP_SAVEFAILED:
            self->mIsOverwriteOK = 0;
            VCALL_K1D8700(self, 41);
            notifyK1D8700(((char*)self), mp);
            break;
        case cFEMemCard::kPOP_MESSAGE:
        case cFEMemCard::kPOP_SAVEDONE:
        case cFEMemCard::kPOP_LOADDONE:
            VCALL_K1D8700(self, 40);
            break;
        case cFEMemCard::kPOP_DELETE:
            self->mIsLoadWait = 0;
            if (choice == 0) {
                self->mIsProcessing = 1;
                self->mIsDeletePressed = 1;
            } else {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            }
            break;
        case cFEMemCard::kPOP_CORRUPT:
            if (choice == 0) {
                VCALL_K1D8700(self, 41);
                self->mState = cFEMemCard::kCHECKING;
                notifyK1D8700(((char*)self), mp);
            } else if (choice == 1) {
                self->mIsRetryPressed = 1;
            } else {
                cFEMemCard_callbackSaveFileCorrupt();
                return;
            }
            break;
        case cFEMemCard::kPOP_CORRUPTDELETE:
            if (choice == 0) {
                self->mState = cFEMemCard::kSAVING;
                self->mIsOverwriteOK = 1;
            } else {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            }
            break;
        case cFEMemCard::kPOP_DELETEDONE:
            self->mIsProcessing = 0;
            self->mIsDeletePressed = 0;
            if (self->mIsLoadPressed != 0) {
                VCALL_K1D8700(self, 41);
            }
            if (self->mIsSavePressed != 0) {
                self->mIsProcessing = 1;
            }
            notifyK1D8700(((char*)self), mp);
            break;
        case cFEMemCard::kPOP_FORMATDONE:
            self->mIsFormatPressed = 0;
            self->SetLoadPressed(0);
            if (self->mIsSavePressed != 0) {
                self->mIsProcessing = 1;
            } else {
                self->mIsProcessing = 0;
            }
            notifyK1D8700(((char*)self), mp);
            break;
        case cFEMemCard::kPOP_FORMATFAIL:
            self->mIsFormatPressed = 0;
            VCALL_K1D8700(self, 41);
            break;
        case cFEMemCard::kPOP_FORMAT:
        case cFEMemCard::kPOP_FORMATNGC:
            self->mIsLoadWait = 0;
            if (choice == 0) {
                self->mIsProcessing = 1;
                self->mIsFormatPressed = 1;
            } else if (choice == 1) {
                self->mIsFormatPressed = 0;
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            }
            break;
        case cFEMemCard::kPOP_DELETEFAIL:
        case cFEMemCard::kPOP_FILEEXISTS:
            VCALL_K1D8700(self, 41);
            break;
        case cFEMemCard::kPOP_CARDCHANGED:
            VCALL_K1D8700(self, 41);
            break;
        }
        if (self->mPopupType != cFEMemCard::kPOP_STATUS) {
            cUIStateStack_pop(&cFEMemCard_mInstance->mpEngine->mStateStack, 1);
        }
        self->mIsStatusPopupOn = 0;
    } else if (button == cUIState::kONPOPUPDESTROY) {
        cFEMemCard* fe = cFEMemCard_mInstance;
        if (fe->mProcessingKeyboard != 0) {
            fe->mpKeyboard = 0;
            fe->mProcessingKeyboard = 0;
        } else {
            if (self->mPopupType == button) {
                self->mIsStatusPopupOn = 0;
            }
            *(int*)&cFEMemCard_mInstance->mStatusPopup = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_cancel);
#ifdef SKIP_ASM
extern "C" void cFEMemCard_SetLoadPressed(cFEMemCard*, int);

/**
 * Cancels the memory-card screen's operation: clears its request words, sets the cancel word and screen state 2.
 *
 * Clears +0x1A8, the overwrite (+0x1F0), retry (+0x1EC), +0x1B0, delete (+0x1E0), format (+0x1E8) and save (+0x1DC)
 * words and the load request (cFEMemCard_SetLoadPressed 0); sets the cancel word (+0x1E4) that
 * cFEMemCard_isCancelPressed returns and the screen state (+0x1C0) to 2.
 */
void cFEMemCard::cancel()
{
    mIsProcessing = 0;
    mIsOverwriteOK = 0;
    mIsRetryPressed = 0;
    mIsLoadWait = 0;
    mIsCancelPressed = 1;
    SetLoadPressed(0);
    mState = cFEMemCard::kSKIP;
    mIsDeletePressed = 0;
    mIsFormatPressed = 0;
    mIsSavePressed = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_exitSave__FPv);
#ifdef SKIP_ASM
/**
 * Leaves the memory card save flow after a successful save.
 *
 * Clears the flags at +0x1A8 and +0x1DC and sets +0x1C0 to 1.
 */
void cFEMemCard::exitSave()
{
    mIsProcessing = 0;
    mState = kCHECKING;
    mIsSavePressed = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_onInputBegin);
#ifdef SKIP_ASM
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
int GetHashValue32(char* str);
extern char str_left_arr[];
extern char str_right_arr[];


/**
 * Handles left/right on the memory-card screen: requests the previous or next card device, pulsing its arrow.
 *
 * Ignores all input while the busy flag (+0x1A8) is 1. Left (input vtable slot 19, probably isLeft) sets
 * +0x1FC, which callbackIsPrevDevicePressed (cFEMemCard_isPrevMCPressed) reads and clears; right (slot 20) sets +0x1F8 for
 * callbackIsNextDevicePressed (cFEMemCard_isNextMCPressed). Either one plays the "left arr" or "right arr" frame label
 * when the screen (+0x40) exists.
 *
 * @param input  the UI input (On Tour cUIInput* pInput)
 * @return 1 when left or right was pressed, otherwise 0
 */
bool cFEMemCard::onInputBegin(cUIInput* input)
{
    int handled = 0;
    if (mIsProcessing == true)
        return false;
    if (input->isLeft()) {
        handled = 1;
        mDoPrevMemCard = handled;
        if (mpScreen) {
            unsigned short frame = cUIScreen_getFrameByLabel(mpScreen, GetHashValue32(str_left_arr));
            if (frame != cUIScreen::kNoFrame)
                cUIScreen_playFrame(mpScreen, frame, 1);
        }
    } else {
        if (input->isRight()) {
            handled = 1;
            mDoNextMemCard = handled;
            if (mpScreen) {
                unsigned short frame = cUIScreen_getFrameByLabel(mpScreen, GetHashValue32(str_right_arr));
                if (frame != cUIScreen::kNoFrame)
                    cUIScreen_playFrame(mpScreen, frame, 1);
            }
        }
    }
    return handled;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_displayStatusPopup);
#ifdef SKIP_ASM

extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_GetMessage(void*, unsigned short*, int);

/**
 * Opens the memory-card status popup (popup 22) with the manager's current message and no buttons.
 *
 * Sets popup id 22 (+0x19C), fills an 800-char buffer with cMCOverlayManager_GetMessage and opens it through getPopup
 * (vtable slot 36) with yes-only, no-buttons and snowflake set, then sets the status-popup flag (+0x210); cFEMemCard_onUpdate
 * refreshes the text every frame while that flag is set.
 */
void cFEMemCard::displayStatusPopup()
{
    unsigned short buf[800];
    void* obj = cSSXApp_getMCManager(SSXApp);
    mPopupType = cFEMemCard::kPOP_STATUS;
    cMCOverlayManager_GetMessage(obj, buf, 800);
    getPopup(buf, 1, 1, 0, 0, 1);
    mIsStatusPopupOn = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_onUpdate);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_GetMessage(void*, unsigned short*, int);
extern "C" void cOVState_onUpdate(void);
extern "C" void cUIState_onUpdate(void*);
extern "C" void cMCOverlayManager_ManagerStart(void*);
extern "C" bool cMCOverlayManager_isManagerDone(void*);
extern "C" void cMCOverlayManager_update(void*);
extern char str_Temp_String[];
extern char str_onmenu[];

// The card work is still running: a load waiting on the manager, or the card itself busy.
#define MEMCARD_BUSY(self, mp) (((self)->mIsLoadWait == 1 && cMCOverlayManager_isManagerDone(mp) == 0) \
        || ((self)->mIsLoadWait == 0 && (mp)->mpMemCard->IsBusy() != 0))

/**
 * Runs the memory-card screen's per-frame update: UI input, the memory-card manager, the screen-state handler and the
 * status display.
 *
 * Without a popup (mStatusPopup) and with no keyboard (mpKeyboard) it runs the base update: cOVState_onUpdate in game
 * (SSXApp->Game set), cUIState_onUpdate otherwise. The first time (mIsManagerStarted clear) it starts the manager
 * (cMCOverlayManager_ManagerStart), sets mIsManagerStarted, display state kDISPLAY_OFF (mScreenStatus and
 * triggerDisplayState) and the manager's kMCOverlay_InitCardsCheck. cMCOverlayManager_update runs unless mState is
 * kDONE and the card work is over;
 * busy means mIsLoadWait is 1 and cMCOverlayManager_isManagerDone is false, or mIsLoadWait is 0 and the card's IsBusy
 * is true. mState picks the handler: kSTART updateStart, kCHECKING updateChecking, kSKIP updateSkip, kSAVING
 * updateSaving, kDELETING updateDeleting, kLOADING updateLoading, kDONE updateDone (only when not busy). With no status
 * popup (mIsStatusPopupOn) and neither auto-load nor auto-save set, a manager mState in a format, save, load or delete
 * progress state (kMCOverlay_FormatProgress, kMCOverlay_InitFormatProgress, kMCOverlay_CreateFile ..
 * kMCOverlay_SaveClosing, kMCOverlay_DeleteProgress, kMCOverlay_DeleteDoneWait, kMCOverlay_InitDeleteProgress,
 * kMCOverlay_LoadStart .. kMCOverlay_LoadDoneWait) opens the status popup (displayStatusPopup); while it is open its
 * text is refreshed (cMCOverlayManager_GetMessage, then the popup's setMessage). Then displayStatus. Without a popup
 * and when the card's IsFileInfoValid (the last operation's outcome) is set, the selected slot (the "onmenu" menu's
 * mScrollItemCurrent, else mSlot) decides hideDeleteButton and hideMainButton: no file in the slot (the card's
 * HasFileName) or no card (IsAvailable1).
 *
 */
void cFEMemCard::onUpdate()
{
    if (mStatusPopup == 0 && mpKeyboard == 0) {
        if ((int)SSXApp->Game != 0) {
            cOVState_onUpdate();
        } else {
            cUIState_onUpdate(this);
        }
    }
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    if (mIsManagerStarted == 0) {
        cMCOverlayManager_ManagerStart(mp);
        mIsManagerStarted = 1;
        mScreenStatus = cFEMemCard::kDISPLAY_OFF;
        triggerDisplayState(cFEMemCard::kDISPLAY_OFF);
        mp->SetState(kMCOverlay_InitCardsCheck);
    }
    if (mState != cFEMemCard::kDONE || MEMCARD_BUSY(this, mp)) {
        cMCOverlayManager_update(mp);
    }
    switch (mState) {
    case cFEMemCard::kSTART:
        updateStart();
        break;
    case cFEMemCard::kCHECKING:
        updateChecking();
        break;
    case cFEMemCard::kSKIP:
        updateSkip();
        break;
    case cFEMemCard::kSAVING:
        updateSaving();
        break;
    case cFEMemCard::kDELETING:
        updateDeleting();
        break;
    case cFEMemCard::kLOADING:
        updateLoading();
        break;
    case cFEMemCard::kDONE:
        {
            cMCOverlayManager* mp2 = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
            if (!MEMCARD_BUSY(this, mp2))
                updateDone();
        }
        break;
    }
    if (cFEMemCard_mInstance->mIsStatusPopupOn == 0 && cFEMemCard_mInstance->mAutoSaveOn == 0 && cFEMemCard_mInstance->mAutoLoadOn == 0) {
        int managerState = mp->mState;
        if (managerState == kMCOverlay_InitFormatProgress || managerState == kMCOverlay_FormatProgress
            || managerState == kMCOverlay_CreateFile || managerState == kMCOverlay_SaveFile
            || managerState == kMCOverlay_SaveProgress || managerState == kMCOverlay_SaveClosing
            || managerState == kMCOverlay_LoadStart || managerState == kMCOverlay_LoadClosing
            || managerState == kMCOverlay_LoadProgress || managerState == kMCOverlay_LoadDoneWait
            || managerState == kMCOverlay_InitDeleteProgress || managerState == kMCOverlay_DeleteProgress
            || managerState == kMCOverlay_DeleteDoneWait) {
            displayStatusPopup();
        }
    }
    if (cFEMemCard_mInstance->mIsStatusPopupOn != 0) {
        unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
        cMCOverlayManager_GetMessage(mp, buf, 800);
        mStatusPopup->setMessage(buf);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    }
    displayStatus();
    if (mStatusPopup == 0 && mp->mpMemCard->IsFileInfoValid() != 0) {
        cUIMenu* menu = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_onmenu));
        int sel = mSlot;
        if (menu != 0) {
            sel = menu->mScrollItemCurrent;
        }
        // hideDeleteButton (slot 38) through its entry: retail keeps &vt[38].fn live across the two card calls, a real
        // call keeps &vt[38] (lw 4(entry) instead of lw 0(fn)).
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)VPTR_OF(cListNode, this);
        void (**pf)(void*, int) = &vt[38].fn;
        (*pf)(vthis(this, vt[38]), mp->mpMemCard->HasFileName(sel) == 0 || mp->mpMemCard->IsAvailable1() == 0);
        bool slotEmpty = !mp->mpMemCard->HasFileName(sel);
        bool cardMissing = !mp->mpMemCard->IsAvailable1();
        hideMainButton(slotEmpty, cardMissing);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_setMenuHighlight);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_onmenu[];
extern "C" unsigned char cUIMenu_findFirstHighlightable(void* menu, unsigned char start);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);

/**
 * Highlights a valid entry of the memory card screen's menu.
 *
 * Finds the "onmenu" menu on the screen (+0x40). When it has no current item or the current one is disabled
 * (flag bit 5), picks the first highlightable item; selects that index with cUIMenu_setSelectedByIndex (index 0
 * when none is highlightable) and stores it at +0x1C4.
 */
void cFEMemCard::setMenuHighlight()
{
    cUIMenu* menu = (cUIMenu*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_onmenu));
    if (((void*)menu) != 0) {
        cUIThing* item = menu->mpSelected;
        int idx = menu->mScrollItemCurrent;
        if (((void*)item) == 0 || ((item->mAttr >> 5) & 1)) {
            idx = cUIMenu_findFirstHighlightable(menu, 0);
        }
        if (idx != cUIMenu::kNoItem) {
            cUIMenu_setSelectedByIndex(menu, idx);
            mSlot = idx;
        } else {
            cUIMenu_setSelectedByIndex(menu, 0);
            mSlot = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_SetLoadPressed);
#ifdef SKIP_ASM

extern "C" void* cSSXApp_getMCManager(void* app);

/**
 * Records whether a load was requested, and remembers the last loaded file's name when it was.
 *
 * Stores @p on at +0x1F4. When set, calls virtual slot 14 of the memory card manager's object (+0x434) with its
 * +0xF8 and this screen's name buffer (+0x9C); otherwise clears the buffer's first character.
 *
 * @param on  nonzero when load was pressed
 */
void cFEMemCard::SetLoadPressed(int on)
{
    mIsLoadPressed = on;
    if (on) {
        cMCOverlayManager* mcManager = (cMCOverlayManager*)((char*)cSSXApp_getMCManager(SSXApp));
        cMemCard* obj = mcManager->mpMemCard;
        obj->GetTitleString(mcManager->mFileCursor, (unsigned short*)&mLastLoadFileName);
    } else {
        *(short*)&mLastLoadFileName[0] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateStart);
#ifdef SKIP_ASM

/**
 * Switches the memory-card screen to display state kDISPLAY_OFF unless it is already there.
 *
 * When the display state (+0x214) is not kDISPLAY_OFF, sets it and calls vtable entry 35
 * (cFEMemCard_triggerDisplayState) with it. Fills vtable entry 31 (+0xFC) of the memory-card states (cFEStateMemCard,
 * cFEStateProfileLoad, cFEStateOptionsDeviceSelect), between the empty update entries and the skip handler.
 */
void cFEMemCard::updateStart()
{
    if (mScreenStatus != cFEMemCard::kDISPLAY_OFF) {
        mScreenStatus = cFEMemCard::kDISPLAY_OFF;
        triggerDisplayState(cFEMemCard::kDISPLAY_OFF);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateChecking__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the memory-card screen's empty update handler in vtable entry 27, +0xDC).
 */
void cFEMemCard::updateChecking()
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateSkip__FPv);
#ifdef SKIP_ASM
/**
 * Sets the memory-card screen state (+0x1C0) to 1.
 */
void cFEMemCard::updateSkip()
{
    mState = cFEMemCard::kCHECKING;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateSaving__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the memory-card screen's empty update handler in vtable entry 28, +0xE4).
 */
void cFEMemCard::updateSaving()
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateDeleting__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the memory-card screen's empty update handler in vtable entry 30, +0xF4).
 */
void cFEMemCard::updateDeleting()
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateLoading__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the memory-card screen's empty update handler in vtable entry 29, +0xEC).
 */
void cFEMemCard::updateLoading()
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_onLoseTransition);
#ifdef SKIP_ASM
/**
 * Reports whether the memory-card screen may finish its leaving transition: the screen state (+0x1C0) is kDONE.
 *
 * Installed in the cUIState onLoseTransition entry (vtable entry 10) of the memory-card states.
 *
 * @return true when the screen state is kDONE
 */
bool cFEMemCard::onLoseTransition()
{
    return mState == cFEMemCard::kDONE;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_triggerArrowPulse);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cUIAnimationBank_getAnimationByHashName(void* self, int hash);
extern "C" void cUIThing_playAnimation(void* obj, void* anim, int a, int b, int c);

/**
 * Plays an animation on a named widget of the memory-card screen.
 *
 * Looks up the widget @p objName on the state's screen (mpScreen, +0x40) and the animation @p animName in the UI
 * engine's animation bank (mpEngine + 0x50, cUIAnimationBank_getAnimationByHashName), both by GetHashValue32(), and
 * plays it with cUIThing_playAnimation(widget, anim, 3, 0, 1). Does nothing when either is missing.
 *
 * @param objName   name of the widget to animate
 * @param animName  name of the animation to play
 */
extern "C" void cFEMemCard_triggerArrowPulse(cFEMemCard* self, char* objName, char* animName)
{
    void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(objName));
    if (obj != 0) {
        void* bank = &self->mpEngine->mAnimBank;
        void* anim = cUIAnimationBank_getAnimationByHashName(bank, GetHashValue32(animName));
        if (anim != 0) {
            cUIThing_playAnimation(obj, anim, 3, 0, 1);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_triggerDisplayState);
#ifdef SKIP_ASM
struct cUIScreen;
unsigned short cUIScreen_getFrameByLabel(cUIScreen* self, int label);
void cUIScreen_jumpToFrame(void* self, unsigned short frame);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_getDeviceNumberString(void* mp, void* out, int a2);
extern "C" void sprintf1(unsigned short* dst, const unsigned short* fmt, ...);
extern "C" void cUIText_setUnicodeStringWrap(void* self, unsigned short* str);
extern char str_Temp_String[];
extern char str_onmenu[];
extern char str_offmenu[];
extern char str_off[];
extern char str_on[];
extern char str_text_box_message[];
extern char str_file_display[];
extern char str_kT_MEMFECheckingCardPS2[];



/**
 * Switches the memory-card screen's display state: moves the menu focus and jumps to that state's frame.
 *
 * Stores @p state at +0x214. kDISPLAY_OFF and kDISPLAY_FILES take the focus from "onmenu" to "offmenu", jump to the
 * "off" or "file display" frame label and show "kT_MEMFECheckingCardPS2" for the selected slot (mSelectedDevice + 1)
 * in "text box message". kDISPLAY_ON focuses "onmenu" and jumps to "on"; any other value is only stored.
 *
 * @param state  the display state, kDISPLAY_OFF .. kDISPLAY_ON (On Tour gotoScreenState)
 */
void cFEMemCard::triggerDisplayState(int state)
{
    unsigned short buf[0x100];
    int frame = 0xFFFF;
    mScreenStatus = state;
    switch (state) {
    case cFEMemCard::kDISPLAY_OFF: {
        cUIThing* widget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_onmenu));
        if (widget) {
            widget->setFocus(0);
        }
        widget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_offmenu));
        if (widget) {
            widget->setFocus(1);
        }
        frame = cUIScreen_getFrameByLabel(mpScreen, GetHashValue32(str_off));
        cUIThing* text = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_text_box_message));
        if (text) {
            unsigned short* str = (unsigned short*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
            cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
            cMCOverlayManager_getDeviceNumberString(mp, buf, mp->mSelectedDevice);
            cFELocale* locale = SSXApp->FELocale;
            sVEntry<unsigned short*(void*, int)>* mvt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, locale);
            sprintf1(str, mvt[4].fn(vthis(locale, mvt[4]), GetHashValue32(str_kT_MEMFECheckingCardPS2)), buf);
            cUIText_setUnicodeStringWrap(text, str);
            text->setVisible(1);
            if (str)
                cMemMan_free(str);
        }
        break;
    }
    case cFEMemCard::kDISPLAY_FILES: {
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        cUIThing* widget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_onmenu));
        if (widget) {
            widget->setFocus(0);
        }
        widget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_offmenu));
        if (widget) {
            widget->setFocus(1);
        }
        frame = cUIScreen_getFrameByLabel(mpScreen, GetHashValue32(str_file_display));
        cUIThing* text = cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_text_box_message));
        if (text) {
            unsigned short* str = (unsigned short*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
            cMCOverlayManager_getDeviceNumberString(mp, buf, mp->mSelectedDevice);
            cFELocale* locale = SSXApp->FELocale;
            sVEntry<unsigned short*(void*, int)>* mvt = (sVEntry<unsigned short*(void*, int)>*)VPTR_OF(cUILocaleInterface, locale);
            sprintf1(str, mvt[4].fn(vthis(locale, mvt[4]), GetHashValue32(str_kT_MEMFECheckingCardPS2)), buf);
            cUIText_setUnicodeStringWrap(text, str);
            text->setVisible(1);
            if (str)
                cMemMan_free(str);
        }
        break;
    }
    case cFEMemCard::kDISPLAY_ON: {
        cUIThing* widget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_onmenu));
        if (widget) {
            widget->setFocus(1);
        }
        widget = (cUIThing*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_offmenu));
        if (widget) {
            widget->setFocus(0);
        }
        frame = cUIScreen_getFrameByLabel(mpScreen, GetHashValue32(str_on));
        break;
    }
    }
    if (frame != cUIScreen::kNoFrame)
        cUIScreen_jumpToFrame(mpScreen, frame);
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_checkNextAvailableDevice);
#ifdef SKIP_ASM
/**
 * Tells whether there is another memory card device in the given direction of the device list.
 *
 * @param list  the memory card overlay manager holding the device list (On Tour cMCOverlayManager*); the device
 *              count is at +0x424
 * @param idx   the current device index
 * @param back  nonzero to step backwards
 * @return 1 when idx + 1 (or idx - 1) is a valid device index, otherwise -1
 */
int cFEMemCard::checkNextAvailableDevice(cMCOverlayManager* list, int idx, int back)
{
    int step = 1;
    if (back) {
        step = -1;
    }
    idx += step;
    if (idx >= 0 && idx <= list->mMaxDevices - 1) {
        return 1;
    }
    return -1;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_onGainFocus);
#ifdef SKIP_ASM

extern "C" void* cUIState_onGainFocus(void* self);
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_text_box_message[];

/**
 * Gains focus for the memory-card screen and hides its "text box message" widget.
 *
 * Runs the base cUIState_onGainFocus(), then looks up "text box message" (str_text_box_message) on mpScreen (+0x40) and
 * calls its setVisible (cUIThing vtable slot 9) with 0.
 */
extern "C" void cFEMemCard_onGainFocus(cFEMemCard* self)
{
    cUIState_onGainFocus(self);
    cUIThing* obj = (cUIThing*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_text_box_message));
    if (((void*)obj) != 0) {
        obj->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the memory-card screen's onLoseFocus handler, vtable entry 11).
 */
void cFEMemCard_onLoseFocus(void* vself)
{
    cFEMemCard* self = (cFEMemCard*)vself;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_displayFileOK);
#ifdef SKIP_ASM
static inline bool IsModeK1D9880(char* mp, int m) { return *(int*)(mp + 0x130) == m; }

/**
 * Reports whether the memory-card screen may show the file display now.
 *
 * True only when the screen state (+0x1C0) is neither kSTART nor kDONE, the memory-card overlay manager
 * (cSSXApp_getMCManager(SSXApp)) has mCheckAllowed set and mGetDir clear, its card (mpMemCard) answers 0 from virtual
 * slot 11, the manager's mState is neither kMCOverlay_ReadingCard nor kMCOverlay_Done, and the card answers non-zero
 * from virtual slot 75.
 *
 * @return 1 when all the checks pass, else 0
 */
bool cFEMemCard::displayFileOK()
{
    int allowed = 0;
    int one = 1;
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    int st = mState;
    if (st != cFEMemCard::kSTART && st != cFEMemCard::kDONE && mp->mCheckAllowed != 0 && mp->mGetDir == 0
        && mp->mpMemCard->IsBusy() == 0) {
        if (!IsModeK1D9880(((char*)mp), kMCOverlay_ReadingCard) && !IsModeK1D9880(((char*)mp), kMCOverlay_Done)) {
            if (mp->mpMemCard->IsFileInfoValid() != 0) {
                allowed = one;
            }
        }
    }
    return allowed;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_createReadBuffer);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
extern char str_read_buffer[];

/**
 * Allocates the memory-card screen's read buffer, freeing the one it owns first.
 *
 * When +0x228 says a buffer is owned, frees the block at +0x1D4 and clears both. The new block (tag "read
 * buffer") goes to +0x1D0 and +0x1D4, and +0x228 is set to keBufferState_Valid.
 *
 * @param size  size of the buffer in bytes (On Tour iSize)
 */
void cFEMemCard::createReadBuffer(unsigned int size)
{
    if (mBufferState == keBufferState_Valid) {
        void* buf = mpStartReadBuffer;
        if (buf != 0) {
            cMemMan_free(buf);
        }
        mpStartReadBuffer = 0;
        mBufferState = keBufferState_Invalid;
    }
    void* readBuffer = operator_new_tag(size, str_read_buffer, 0x100, 0);
    mpReadBuffer = (char*)readBuffer;
    mpStartReadBuffer = (char*)readBuffer;
    mBufferState = keBufferState_Valid;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEPopupConfirm_cFEPopupConfirm);
#ifdef SKIP_ASM
extern "C" void* cScreenPopup_cScreenPopup(void* mem, void* engine, void* owner);
extern "C" void cFEPopupConfirm_initPopup(cFEPopupConfirm* self, int a1, int a2, int a3, int a4, int a5, int a6);
extern char cFEPopupConfirm_vtbl[];
extern int cFEPopupConfirm_mUsePreFE;


/**
 * Constructs a confirmation popup: builds the cScreenPopup base, picks its background, then runs initPopup().
 *
 * The background (info.meBackgroundType, +0x17C) is 1 (blue) while cFEPopupConfirm_mUsePreFE is 0 (cSSXApp_startFELoad
 * clears it, so probably in the front end), else 2 (black) with layer offset 4 (info.miLayerOffset, +0x14C),
 * which initPopup() then overwrites with @p layerOffset.
 *
 * @param engine  the UI engine (On Tour pEngine)
 * @param owner  the parent state (On Tour pParentState)
 * @param ustrDisplay  the message, a 16-bit string (declared int here)
 * @param bYesOnly  nonzero for a single button
 * @param bNoButtons  nonzero for no buttons
 * @param layerOffset  draw-layer offset of the popup
 * @param passUpdate  nonzero to keep updating the parent state while the popup is up
 * @param bSnowFlake  passed on to initPopup(), which ignores it
 * @return @p self
 */
extern "C" cFEPopupConfirm* cFEPopupConfirm_cFEPopupConfirm(cFEPopupConfirm* self, cUIEngine* engine, cUIState* owner, int ustrDisplay, int bYesOnly, int bNoButtons, int layerOffset, int passUpdate, int bSnowFlake)
{
    cScreenPopup_cScreenPopup((self), engine, owner);
    VPTR_OF(cListNode, self) = (void**)cFEPopupConfirm_vtbl;
    if (cFEPopupConfirm_mUsePreFE == 0) {
        self->info.meBackgroundType = 1;
    } else {
        self->info.meBackgroundType = 2;
        self->info.miLayerOffset = 4;
    }
    cFEPopupConfirm_initPopup(self, ustrDisplay, bYesOnly, bNoButtons, layerOffset, passUpdate, bSnowFlake);
    return (self);
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEPopupConfirm_initPopup);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
void func_001C57C0(void* self, int a1, int a2);
// PORT: cScreenPopup_redrawPopup takes only self; the unit's later (void*, int) declaration is a guess. Bound by asm label.
void cScreenPopup_redrawPopup_self(void* self) __asm__("cScreenPopup_redrawPopup");
extern "C" void* strcpy1(void* dst, void* src);
extern "C" void func_002C2540(void*, void*);
extern char str_kT_CMNYes[];
extern char str_kT_CMNNo[];
extern char str_empty_d[];

/**
 * Sets up a confirmation popup's info: Yes/No options, message, button count and layer, then redraws it.
 *
 * Names options 0 and 1 "kT_CMNYes" and "kT_CMNNo" (popup type 2), sets the default option to 0, clears the
 * pass-update and receive-previous flags, copies the message to +0x360 and points info.mtMessage at the copy.
 * No buttons: type 0, button count (+0xBF8) 0, pass-update on; yes only: type 1, count 1; otherwise count 2.
 * Redraws when the popup has a screen (+0x40).
 *
 * The message is copied twice; a null @p ustrDisplay converts "" into the buffer and then still copies from
 * the null pointer.
 *
 * @param ustrDisplay  the message, a 16-bit string (declared int here)
 * @param bYesOnly  nonzero for a single button
 * @param bNoButtons  nonzero for no buttons (wins over @p bYesOnly)
 * @param layerOffset  draw-layer offset, stored in info.miLayerOffset
 * @param passUpdate  stored in info.mbPassUpdate for the one- and two-button kinds
 * @param bSnowFlake  unused
 */
// PORT: the unit declares the string pointer as int; a6 is unused
extern "C" void cFEPopupConfirm_initPopup(cFEPopupConfirm* self, int ustrDisplay, int bYesOnly, int bNoButtons, int layerOffset, int passUpdate, int bSnowFlake)
{
    void* str = (void*)ustrDisplay;
    cPopupInfo* sub = &self->info;
    func_001C57C0(sub, 0, GetHashValue32(str_kT_CMNYes));
    func_001C57C0(sub, 1, GetHashValue32(str_kT_CMNNo));
    self->info.meType = 2;
    self->info.miLayerOffset = layerOffset;
    self->info.miDefaultOption = 0;
    self->info.mbPassUpdate = 0;
    self->info.mbReceivePrev = 0;
    char* displayText;
    if (str) {
        strcpy1(&self->mDisplayText[0], str);
        displayText = (char*)&self->mDisplayText[0];
    } else {
        displayText = (char*)&self->mDisplayText[0];
        func_002C2540(displayText, str_empty_d);
    }
    strcpy1(displayText, str);
    if (bNoButtons) {
        self->mNumButtons = 0;
        sub->mbPassUpdate = 1;
        sub->meType = 0;
    } else if (bYesOnly) {
        self->mNumButtons = 1;
        sub->meType = 1;
        sub->mbPassUpdate = passUpdate;
    } else {
        self->mNumButtons = 2;
        sub->mbPassUpdate = passUpdate;
    }
    sub->mtMessage.pustr = (const unsigned short*)displayText;
    sub->mtMessage.eType = 1;
    if ((int)self->mpScreen) {
        cScreenPopup_redrawPopup_self(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEPopupConfirm_setButtonText);
#ifdef SKIP_ASM
extern "C" void* strcpy1(void* dst, void* src);
void func_001C57A0(void* slots, int idx, int data);
// PORT: cScreenPopup_redrawPopup takes only self; the unit's later (void*, int) declaration is a guess. Bound by asm label.
void cScreenPopup_redrawPopup_self(void* self) __asm__("cScreenPopup_redrawPopup");

/**
 * Sets the label of one of a confirmation popup's buttons, then redraws the popup if it has a screen.
 *
 * Button 0 copies the text to +0x9A0 and button 1 (only when the popup has two or more buttons, +0xBF8) to
 * +0xA68; any other value makes it a three-button popup (type and count 3) and copies to +0xB30. The copy
 * becomes that option's name (setPopupOptionNames).
 *
 * @param src  the label, a 16-bit string (On Tour ustrText)
 * @param mode  which button: 0, 1, or anything else for a third one (On Tour uiButton)
 */
void cFEPopupConfirm::setButtonText(unsigned short*src, unsigned int mode)
{
    void* slots = (char*)&info;
    if (mode == 0) {
        void* buttonText = (char*)&mYesText[0];
        strcpy1(buttonText, src);
        func_001C57A0(slots, 0, (int)buttonText); // PORT: pointer passed as int
    } else if (mode == 1) {
        if (mNumButtons >= 2) {
            void* buttonText = (char*)&mNoText[0];
            strcpy1(buttonText, src);
            func_001C57A0(slots, 1, (int)buttonText);
        }
    } else {
        void* buttonText = (char*)&mAltText[0];
        mNumButtons = 3;
        info.meType = 3;
        strcpy1(buttonText, src);
        func_001C57A0(slots, 2, (int)buttonText);
    }
    if ((int)mpScreen != 0) {
        cScreenPopup_redrawPopup_self(this);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEPopupConfirm_setMessage);
#ifdef SKIP_ASM
extern "C" int strlen1(void* p);
extern "C" void* strcpy1(void* dst, void* src);
// PORT: cScreenPopup_redrawPopup takes only self; the unit's later (void*, int) declaration is a guess. Bound by asm label.
void cScreenPopup_redrawPopup_self(void* self) __asm__("cScreenPopup_redrawPopup");

/**
 * Sets the confirm popup's message text, redrawing the popup when it is already up.
 *
 * Copies the 16-bit string into the popup's own buffer (+0x360, On Tour mDisplayText), points the popup info's
 * text (+0xC4) at it and sets +0xC0 to 1; when +0x40 is set, calls cScreenPopup_redrawPopup(). A null or empty
 * string changes nothing.
 *
 * @param src  the new 16-bit message
 */
void cFEPopupConfirm::setMessage(unsigned short*src)
{
    if (src != 0 && strlen1(src) != 0) {
        void* dst = (char*)&mDisplayText[0];
        strcpy1(dst, src);
        info.mtMessage.pustr = (const unsigned short*)dst;
        info.mtMessage.eType = 1;
        if ((int)mpScreen != 0) {
            cScreenPopup_redrawPopup_self(this);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEPopupConfirm_setDefaultButton);
#ifdef SKIP_ASM
extern "C" void cScreenPopup_redrawPopup(void*, int);

/**
 * Sets a confirmation popup's default button, then redraws the popup if it has a screen.
 *
 * Stores @p uiButton in the popup info's miDefaultOption (+0x168). The redraw is declared with a second
 * argument that cScreenPopup_redrawPopup does not take.
 *
 * @param uiButton  index of the default button
 */
extern "C" void cFEPopupConfirm_setDefaultButton(cFEPopupConfirm* self, int uiButton)
{
    self->info.miDefaultOption = uiButton;
    if ((int)self->mpScreen != 0) {
        cScreenPopup_redrawPopup(self, uiButton);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEStateNetHelpPopup_cFEStateNetHelpPopup);
#ifdef SKIP_ASM
extern "C" void* cUIState_cUIState1(void* self, void* engine, void* owner);
extern "C" void cFEStateNetHelpPopup_clear(void* self);
extern char cFEStateNetHelpPopup_vtbl[];

inline void* operator new[](unsigned int, void* p) { return p; }

struct sMcSlot9D48 {
    int v[2];
    sMcSlot9D48() {}
};

/**
 * Constructs a network help popup state owned by another state.
 *
 * Runs the cUIState constructor cUIState_cUIState1(self, engine, owner), stores cFEStateNetHelpPopup_vtbl, runs the empty
 * constructor of a 7-element array at +0x54 (8-byte elements), clears +0x4C and +0x50 (the flag that makes
 * cFEStateNetHelpPopup_handleEvent forward events to the owner), sets mHashName (+0xC) to 76 and clears the popup's
 * texts and options with cFEStateNetHelpPopup_clear().
 *
 * @param engine  the UI engine
 * @param owner   the parent state (mpParentState)
 * @return @p self
 */
extern "C" void* cFEStateNetHelpPopup_cFEStateNetHelpPopup(cFEStateNetHelpPopup* self, cUIEngine* engine, cUIState* owner)
{
    cUIState_cUIState1(self, engine, owner);
    VPTR_OF(cListNode, self) = (void**)cFEStateNetHelpPopup_vtbl;
    new ((char*)&self->muiScreenName) sMcSlot9D48[7];
    self->miReleasedCount = 0;
    self->mbReferEventsToParent = 0;
    self->mHashName = 76;
    cFEStateNetHelpPopup_clear(self);
    return ((void*)self);
}
#endif

