#include "vtable.h"
#include "common.h"
#include "fe/popupinfo.h"
#include "fe/fepopupconfirm.h"
#include "fe/festatenethelppopup.h"
#include "movie/mcoverlaymanager.h"
#include "ui/uistate.h"
#include "ui/uiinput.h"
#include "ui/uiengine.h"
#include "fe/fememcard.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackSaveSuccess);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_kT_MEMOVSaveDone[];
extern char str_kT_CMNContinue[];
void cFEMemCard_exitSave(void* self);


struct sVEntryVS1D66B0 {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

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
    cFEMemCard_exitSave(cFEMemCard_mInstance);
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x1F0) = 0;
    if (*(int*)(fe + 0x208) == 0) {
        char* locale = (char*)SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
        void* str = ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVSaveDone));
        char* fe2 = (char*)cFEMemCard_mInstance;
        *(int*)(fe2 + 0x19C) = 5;
        sVEntryVS1D66B0* vt = *(sVEntryVS1D66B0**)(fe2 + 8);
        vt[36].fn(vthis(fe2, vt[36]), str, 1, 0, 0, 0, 0);
        char* locale2 = (char*)SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
        void* str2 = o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue));
        char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
        sVEntryVS1D66B0* mvt = *(sVEntryVS1D66B0**)(popup + 8);
        mvt[26].fn(vthis(popup, mvt[26]), str2, 0);
    } else {
        sVEntryVS1D66B0* vt = *(sVEntryVS1D66B0**)(fe + 8);
        vt[40].fn(vthis(fe, vt[40]));
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


struct sVEntryVR1D67C8 {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

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
    cFEMemCard_SetLoadPressed((cFEMemCard*)cFEMemCard_mInstance, 0);
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x1B4) = 1;
    *(int*)(fe + 0x1A8) = 0;
    if (*(int*)(fe + 0x204) == 0) {
        char* locale = (char*)SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
        void* str = ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVLoadDone));
        char* fe2 = (char*)cFEMemCard_mInstance;
        *(int*)(fe2 + 0x19C) = 6;
        sVEntryVR1D67C8* vt = *(sVEntryVR1D67C8**)(fe2 + 8);
        vt[36].fn(vthis(fe2, vt[36]), str, 1, 0, 0, 0, 0);
        char* locale2 = (char*)SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
        void* str2 = o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue));
        char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
        sVEntryVR1D67C8* mvt = *(sVEntryVR1D67C8**)(popup + 8);
        mvt[26].fn(vthis(popup, mvt[26]), str2, 0);
    } else {
        sVEntryVR1D67C8* vt = *(sVEntryVR1D67C8**)(fe + 8);
        vt[40].fn(vthis(fe, vt[40]));
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    char* memCard = *(char**)(mp + 0x434);
    sVEntry<void(void*, ...)>* svt = *(sVEntry<void(void*, ...)>**)memCard;
    svt[14].fn(vthis(memCard, svt[14]), *(int*)(mp + 0xF8), name);
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVOverWriteFile)), name);
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x19C) = 1;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(vthis(fe, vt[36]), buf, 0, 0, 0, 1, 0);
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
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* vt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    strcpy1(buf, vt[4].fn(vthis(locale, vt[4]), GetHashValue32(str_kT_POPUP200CreateEdit)));
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x19C) = 2;
    sVEntry<void(void*, ...)>* vt2 = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt2[36].fn(vthis(fe, vt2[36]), buf, 0, 0, 0, 0, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* vt3 = *(sVEntry<void(void*, ...)>**)(popup + 8);
    vt3[25].fn(vthis(popup, vt3[25]), 1);
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(locale + ovt[4].delta, GetHashValue32(str_kT_MEMOVLoadFailedPS2NGC)), name);
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x19C) = 10;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(fe + vt[36].delta, buf, 1, 0, 0, 0, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    char* mthis = popup + mvt[26].delta;
    char* locale2 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
    char* o2this = locale2 + o2vt[4].delta;
    void* str2 = o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cFEMemCard_mInstance->mPopupType = 11;
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(locale + ovt[4].delta, GetHashValue32(str_kT_MEMOVSaveFailed)), name);
    char* fe = (char*)cFEMemCard_mInstance;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(fe + vt[36].delta, buf, 1, 0, 0, 0, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    char* mthis = popup + mvt[26].delta;
    char* locale2 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
    char* o2this = locale2 + o2vt[4].delta;
    void* str2 = o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
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
    char* o = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(o + 4);
    char* othis = o + ovt[4].delta;
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVNotDetectedPS2)), name);
    cFEMemCard_mInstance->mPopupType = 9;
    if (!(*(int*)(mp + 0xBC) || *(int*)(mp + 0xC0) || (*(int (**)())(mp + 0x78))())) {
        char* fe = (char*)cFEMemCard_mInstance;
        sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
        vt[36].fn(vthis(fe, vt[36]), buf, 0, 0, 0, 0, 0);
        SetLine_1D6D88(str_kT_MEMOVRetry, 1);
        char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
        sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
        mvt[25].fn(vthis(popup, mvt[25]), 1);
        SetLine_1D6D88(str_kT_BTNCancel, 0);
    } else {
        char* fe = (char*)cFEMemCard_mInstance;
        sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
        vt[36].fn(vthis(fe, vt[36]), buf, 1, 0, 0, 0, 0);
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    char* sf = *(char**)(mp + 0x434);
    sVEntry<void(void*, ...)>* sfvt = *(sVEntry<void(void*, ...)>**)sf;
    sfvt[14].fn(sf + sfvt[14].delta, *(int*)(mp + 0xF8), name2);
    (cFEMemCard_mInstance)->mPopupType = 0xC;
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(locale + ovt[4].delta, GetHashValue32(str_kT_MEMOVGameCorruptPS2)), name);
    char* fe = (char*)cFEMemCard_mInstance;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(fe + vt[36].delta, buf, 1, 0, 0, 1, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    char* mthis = popup + mvt[26].delta;
    char* buttonLocale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(buttonLocale + 4);
    char* o2this = buttonLocale + o2vt[4].delta;
    void* str2 = o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    cSSXApp* app = SSXApp;
    cFEMemCard_mInstance->mPopupType = 13;
    char* locale = (char*)app->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMCorruptSavePS2)), name);
    char* fe = (char*)cFEMemCard_mInstance;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(vthis(fe, vt[36]), buf, 0, 0, 0, 1, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    mvt[25].fn(vthis(popup, mvt[25]), 1);
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
 * SetState(36) (manager vtable slot 1) and cMCOverlayManager_SetAction(manager, 0).
 */
extern "C" void cFEMemCard_callbackFileNotFound(void)
{
    char* fe = (char*)cFEMemCard_mInstance;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[41].fn(vthis(fe, vt[41]));
    void* app = SSXApp;
    cFEMemCard_mInstance->mIsCancelPressed = 0;
    char* obj = (char*)cSSXApp_getMCManager(app);
    sVEntry<void(void*, ...)>* vt2 = *(sVEntry<void(void*, ...)>**)(obj + 0x748);
    vt2[1].fn(vthis(obj, vt2[1]), 36);
    cMCOverlayManager_SetAction(obj, 0);
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMFormatCardPS2)), name);
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x19C) = 0xE;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(vthis(fe, vt[36]), buf, 0, 0, 0, 1, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    mvt[25].fn(vthis(popup, mvt[25]), 1);
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
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* vt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    strcpy1(buf, vt[4].fn(vthis(locale, vt[4]), GetHashValue32(str_kT_MEMOVFormatComplete)));
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x19C) = 0x10;
    sVEntry<void(void*, ...)>* vt2 = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt2[36].fn(vthis(fe, vt2[36]), buf, 1, 0, 0, 0, 0);
    char* locale2 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* vt4 = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
    void* str = vt4[4].fn(vthis(locale2, vt4[4]), GetHashValue32(str_kT_CMNContinue));
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* vt3 = *(sVEntry<void(void*, ...)>**)(popup + 8);
    vt3[26].fn(vthis(popup, vt3[26]), str, 0);
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMFEFormatFailed)), name);
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x19C) = 17;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(vthis(fe, vt[36]), buf, 1, 0, 0, 0, 0);
    char* locale2 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
    void* str2 = o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue));
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    mvt[26].fn(vthis(popup, mvt[26]), str2, 0);
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
 * slot 13 counts them): sets popup 3 (+0x19C), formats "kT_MEMConfirmDeletePS2" with the name of file
 * mFileCursor (card slot 14) into a "Temp String" buffer, opens a two-button popup that passes updates
 * through and makes button 1 (No) the default. With no files it clears +0x1A8 and +0x1E0 when the screen
 * state (+0x1C0) is 4.
 */
extern "C" void cFEMemCard_callbackConfirmDelete(void)
{
    unsigned short name[0x100];
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    char* sf = *(char**)(mp + 0x434);
    sVEntry<int(void*, ...)>* sfvt = *(sVEntry<int(void*, ...)>**)sf;
    if (sfvt[13].fn(vthis(sf, sfvt[13])) > 0) {
        unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
        cFEMemCard_mInstance->mPopupType = 3;
        char* sf2 = *(char**)(mp + 0x434);
        sVEntry<void(void*, ...)>* sfvt2 = *(sVEntry<void(void*, ...)>**)sf2;
        sfvt2[14].fn(vthis(sf2, sfvt2[14]), *(int*)(mp + 0xF8), name);
        char* locale = (char*)SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
        sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMConfirmDeletePS2)), name);
        char* fe = (char*)cFEMemCard_mInstance;
        sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
        vt[36].fn(vthis(fe, vt[36]), buf, 0, 0, 0, 1, 0);
        char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
        sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
        mvt[25].fn(vthis(popup, mvt[25]), 1);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    } else {
        char* fe = (char*)cFEMemCard_mInstance;
        if (*(int*)(fe + 0x1C0) == 4) {
            *(int*)(fe + 0x1A8) = 0;
            *(int*)(fe + 0x1E0) = 0;
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(locale + ovt[4].delta, GetHashValue32(str_kT_MEMOVNotDetectedPS2)), name);
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x19C) = 18;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(fe + vt[36].delta, buf, 0, 0, 0, 0, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    char* mthis = popup + mvt[26].delta;
    char* locale2 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
    char* o2this = locale2 + o2vt[4].delta;
    void* str2 = o2vt[4].fn(o2this, GetHashValue32(str_kT_MEMOVRetry));
    mvt[26].fn(mthis, str2, 0);
    char* popup3 = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* m3vt = *(sVEntry<void(void*, ...)>**)(popup3 + 8);
    char* m3this = popup3 + m3vt[26].delta;
    char* locale3 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o3vt = *(sVEntry<void*(void*, ...)>**)(locale3 + 4);
    char* o3this = locale3 + o3vt[4].delta;
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

struct sVEntryI_001D7998 {
    short delta;
    short index;
    int (*fn)(void*, ...);
};

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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
    cFEMemCard_mInstance->mPopupType = 19;
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    unsigned short* fmt = (unsigned short*)ovt[4].fn(locale + ovt[4].delta, GetHashValue32(str_kT_MEMFETooBigPS2));
    char* sf = *(char**)(mp + 0x434);
    sVEntryI_001D7998* sfvt = *(sVEntryI_001D7998**)sf;
    int n = sfvt[23].fn(sf + sfvt[23].delta);
    sprintf1(buf, fmt, name, n);
    char* fe = (char*)cFEMemCard_mInstance;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(fe + vt[36].delta, buf, 1, 0, 3, 1, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    char* mthis = popup + mvt[26].delta;
    char* locale2 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
    char* o2this = locale2 + o2vt[4].delta;
    void* str2 = o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
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

struct sVEntryI_001D7B18 {
    short delta;
    short index;
    int (*fn)(void*, ...);
};

struct sLocal_001D7B18 {
    int v[8];
};

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
    sLocal_001D7B18 name;
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    cFEMemCard_mInstance->mPopupType = 19;
    cMCOverlayManager_getDeviceNumberString(mp, &name, *(int*)(mp + 0x428));
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    unsigned short* fmt = (unsigned short*)ovt[4].fn(locale + ovt[4].delta, GetHashValue32(str_kT_MEMFETooBigPS2));
    char* sf = *(char**)(mp + 0x434);
    sVEntryI_001D7B18* sfvt = *(sVEntryI_001D7B18**)sf;
    int n = sfvt[23].fn(sf + sfvt[23].delta);
    sprintf1(buf, fmt, &name, n);
    char* fe = (char*)cFEMemCard_mInstance;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(fe + vt[36].delta, buf, 1, 0, 3, 1, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    char* mthis = popup + mvt[26].delta;
    char* locale2 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
    char* o2this = locale2 + o2vt[4].delta;
    void* str2 = o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
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
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x19C) = 23;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    char* locale = (char*)app->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    vt[36].fn(vthis(fe, vt[36]), ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_FENameAlreadyExists)), 1, 0, 0, 0, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    char* locale2 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
    mvt[26].fn(vthis(popup, mvt[26]), o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue)), 0);
    cFEMemCard_mInstance->mState = 1;
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    unsigned short* buf = (unsigned short*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(locale + ovt[4].delta, GetHashValue32(str_kT_MEMDifferentMemcardPS2)), name);
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x19C) = 0x18;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(fe + vt[36].delta, buf, 1, 0, 0, 0, 0);
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    char* mthis = popup + mvt[26].delta;
    char* locale2 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
    char* o2this = locale2 + o2vt[4].delta;
    void* str2 = o2vt[4].fn(o2this, GetHashValue32(str_kT_CMNContinue));
    mvt[26].fn(mthis, str2, 0);
    cFEMemCard_mInstance->mState = 1;
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
 * String" buffer and shows it as popup 7 (+0x19C). Then a screen state (+0x1C0) of 4 is set back to 1.
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
        char* locale = (char*)SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* vt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
        strcpy1(buf, vt[4].fn(vthis(locale, vt[4]), GetHashValue32(str_kT_MEMDeleteSuccess)));
        char* fe = (char*)cFEMemCard_mInstance;
        *(int*)(fe + 0x19C) = 7;
        sVEntry<void(void*, ...)>* vt2 = *(sVEntry<void(void*, ...)>**)(fe + 8);
        vt2[36].fn(vthis(fe, vt2[36]), buf, 1, 0, 0, 0, 0);
        char* locale2 = (char*)SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
        void* str2 = o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue));
        char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
        sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
        mvt[26].fn(vthis(popup, mvt[26]), str2, 0);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    }
    char* fe = (char*)cFEMemCard_mInstance;
    if (*(int*)(fe + 0x1C0) == 4) {
        *(int*)(fe + 0x1C0) = 1;
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    cMCOverlayManager_getDeviceNumberString(mp, name, *(int*)(mp + 0x428));
    char* locale = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
    sprintf1(buf, (unsigned short*)ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMOVDeleteFailedError)), name);
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x19C) = 0x8;
    sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
    vt[36].fn(vthis(fe, vt[36]), buf, 1, 0, 0, 0, 0);
    char* locale2 = (char*)SSXApp->FELocale;
    sVEntry<void*(void*, ...)>* o2vt = *(sVEntry<void*(void*, ...)>**)(locale2 + 4);
    void* str2 = o2vt[4].fn(vthis(locale2, o2vt[4]), GetHashValue32(str_kT_CMNContinue));
    char* popup = (char*)cFEMemCard_mInstance->mStatusPopup;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(popup + 8);
    mvt[26].fn(vthis(popup, mvt[26]), str2, 0);
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

class cMoviePlayerK1D8160 {
public:
    char pad_0x000[0x748];
    virtual void SetState(int);
};

class cMovieStreamK1D8160 {
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40();
    virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51();
    virtual int IsAvailable(int);
};

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
    cMoviePlayerK1D8160* mp = (cMoviePlayerK1D8160*)cSSXApp_getMCManager(SSXApp);
    int next = *(int*)((char*)mp + 0x428) + 1;
    if (next < *(int*)((char*)mp + 0x424)) {
        cMCOverlayManager_selectDevice(mp, next);
        mp->SetState(49);
        return;
    }
    cMCOverlayManager_selectDevice(mp, 0);
    if ((*(cMovieStreamK1D8160**)((char*)mp + 0x434))->IsAvailable(0) == 0) {
        char* fe = (char*)cFEMemCard_mInstance;
        if (*(int*)(fe + 0x208) != 0) {
            *(int*)(fe + 0x1C0) = 6;
            mp->SetState(2);
            return;
        }
        cMCOverlayManager_OnHighlightNextDevice(mp);
    }
    cMCOverlayManager_setSelectedDevice(mp);
    mp->SetState(54);
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_callbackDisplayNoCards);
#ifdef SKIP_ASM
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIText_setUnicodeStringWrap(void* self, unsigned short* str);
extern char str_text_box_message[];
extern char str_kT_MEMNoCardsPS2[];


/**
 * Shows the "no memory cards" message in the screen's "text box message" object and switches to display state 1.
 *
 * Callback for the memory-card manager's callbackDisplayNoCards slot (0x9C), installed by cFEMemCard_initFEMemCard; many
 * memory-card screens also call it directly. Sets the screen state (+0x1C0 of cFEMemCard_mInstance) to 1; when the
 * display state (+0x214) is not 1 yet it records 1 there and calls triggerDisplayState (vtable slot 35,
 * cFEMemCard_triggerDisplayState) with 1. When the screen has a "text box message" object, the localized
 * kT_MEMNoCardsPS2 text is copied into a 2000-byte "Temp String" buffer, word-wrapped into the object
 * (cUIText_setUnicodeStringWrap) and the object is shown (slot 9 with 1).
 */
extern "C" void cFEMemCard_callbackDisplayNoCards(void)
{
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x1C0) = 1;
    if (*(int*)(fe + 0x214) != 1) {
        *(int*)(fe + 0x214) = 1;
        sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(fe + 8);
        vt[35].fn(vthis(fe, vt[35]), 1);
    }
    char* st = (char*)cFEMemCard_mInstance;
    char* obj = (char*)cUIScreen_getObjectByHashName(*(void**)(st + 0x40), GetHashValue32(str_text_box_message));
    if (obj) {
        unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
        char* locale = (char*)SSXApp->FELocale;
        sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
        strcpy1(buf, ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMNoCardsPS2)));
        cUIText_setUnicodeStringWrap(obj, buf);
        sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(obj + 8);
        vt[9].fn(vthis(obj, vt[9]), 1);
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
 * Switches the memory-card screen to display state 2 when no files were found, while the screen is still starting.
 *
 * Callback for the memory-card manager's callbackDisplayNoFiles slot (0x98), installed by cFEMemCard_initFEMemCard. Only when the
 * screen state (+0x1C0 of cFEMemCard_mInstance) is 0 and the display state (+0x214) is not already 2: records 2 and
 * calls triggerDisplayState (vtable slot 35) with 2.
 */
extern "C" void cFEMemCard_callbackDisplayNoFiles(void)
{
    char* fe = (char*)cFEMemCard_mInstance;
    if (*(int*)(fe + 0x214) != 2 && *(int*)(fe + 0x1C0) == 0) {
        *(int*)(fe + 0x214) = 2;
        sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(fe + 8);
        vt[35].fn(vthis(fe, vt[35]), 2);
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
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIText_setUnicodeStringWrap(void* self, unsigned short* str);
extern char str_Temp_String[];
extern char str_text_box_message[];
extern char str_kT_MEMNoCardsPS2[];


static inline void SetMode_001D83A8(char* fe, int mode)
{
    if (*(int*)(fe + 0x214) != mode) {
        *(int*)(fe + 0x214) = mode;
        sVEntry<void(void*, ...)>* vt = *(sVEntry<void(void*, ...)>**)(fe + 8);
        vt[35].fn(vthis(fe, vt[35]), mode);
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
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    cFEMemCard_mInstance->mState = 1;
    sVEntry<void(void*, ...)>* mvt = *(sVEntry<void(void*, ...)>**)(mp + 0x748);
    mvt[1].fn(vthis(mp, mvt[1]), 2);
    char* sf = *(char**)(mp + 0x434);
    sVEntry<int(void*, ...)>* sfvt = *(sVEntry<int(void*, ...)>**)sf;
    int busy = sfvt[53].fn(vthis(sf, sfvt[53]));
    if (busy) {
        SetMode_001D83A8((char*)cFEMemCard_mInstance, 3);
    } else {
        SetMode_001D83A8((char*)cFEMemCard_mInstance, 1);
        char* st = (char*)cFEMemCard_mInstance;
        void* obj = cUIScreen_getObjectByHashName(*(void**)(st + 0x40), GetHashValue32(str_text_box_message));
        if (obj) {
            unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
            char* locale = (char*)SSXApp->FELocale;
            sVEntry<void*(void*, ...)>* ovt = *(sVEntry<void*(void*, ...)>**)(locale + 4);
            strcpy1(buf, ovt[4].fn(vthis(locale, ovt[4]), GetHashValue32(str_kT_MEMNoCardsPS2)));
            cUIText_setUnicodeStringWrap(obj, buf);
            sVEntry<void(void*, ...)>* bvt = *(sVEntry<void(void*, ...)>**)((char*)obj + 8);
            bvt[9].fn(vthis(obj, bvt[9]), 1);
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
extern "C" void* cFEPopupConfirm_cFEPopupConfirm_8530(void* self, void* engine, void* owner, int a3, int a4, int a5, int a6, int a7, int a8) __asm__("cFEPopupConfirm_cFEPopupConfirm");
extern "C" void cUIStateStack_pushPopup(void* list, void* item);

class cPopup_8530 {
public:
    int pad[2];
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
    virtual void setDefaultButton(int);
    virtual void v26();
    virtual void v27();
    virtual void initPopup(int, int, int, int, int, int);
};

/**
 * Opens the memory-card screen's confirmation popup, or re-initialises the one already open.
 *
 * Vtable slot +0x124 of the memory-card screens. Clears the status-popup flag (+0x210) unless the popup id (+0x19C of
 * cFEMemCard_mInstance) is 22. An existing popup (+0x218) gets initPopup (popup vtable slot 28) with the six arguments;
 * otherwise a cFEPopupConfirm is allocated (cMemMan_alloc(3068, "cFEPopupConfirm", 0x100, 0)), constructed with the
 * instance's engine (+0x10) and the instance as parent, stored in +0x218 and pushed onto the engine's state stack
 * (+0x18, cUIStateStack_pushPopup). For popup id 1 it makes button 1 the default (popup slot 25).
 *
 * Reads the popup id through cFEMemCard_mInstance but writes +0x210 and +0x218 through @p self_; callers pass the
 * instance.
 *
 * @param self_  the memory-card screen
 * @param displayText  the 16-bit message text
 * @param yesOnly  nonzero for a single (yes) button
 * @param noButtons  nonzero for no buttons
 * @param layerOffset  layer offset of the popup
 * @param passUpdate  passUpdate: nonzero lets updates pass to the screen below
 * @param snowFlake  nonzero for the snowflake variant
 */
extern "C" void cFEMemCard_getPopup(cFEMemCard* self_, int displayText, int yesOnly, int noButtons, int layerOffset, int passUpdate, int snowFlake)
{
    if (cFEMemCard_mInstance->mPopupType != 22) {
        self_->mIsStatusPopupOn = 0;
    }
    cPopup_8530* existingPopup = (cPopup_8530*)self_->mStatusPopup;
    if (existingPopup != 0) {
        existingPopup->initPopup(displayText, yesOnly, noButtons, layerOffset, passUpdate, snowFlake);
    } else {
        void* mem = cMemMan_alloc(3068, str_cFEPopupConfirm, 0x100, 0);
        void* engine = cFEMemCard_mInstance->mpEngine;
        cPopup_8530* n = (cPopup_8530*)cFEPopupConfirm_cFEPopupConfirm_8530(mem, engine, cFEMemCard_mInstance, displayText, yesOnly, noButtons, layerOffset, passUpdate, snowFlake);
        self_->mStatusPopup = (cFEPopupConfirm*)n;
        cUIStateStack_pushPopup(&self_->mpEngine->mStateStack, n);
    }
    if (cFEMemCard_mInstance->mPopupType == 1) {
        ((cPopup_8530*)self_->mStatusPopup)->setDefaultButton(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_exitDelete);
#ifdef SKIP_ASM

/**
 * Leaves the delete flow: screen state 4 (deleting) goes back to 1, and +0x1A8 and the delete word (+0x1E0) are
 * cleared.
 *
 * Vtable slot +0x174 of the memory-card screens; works on cFEMemCard_mInstance. +0x1E0 is the word
 * cFEMemCard_isDeletePressed returns.
 */
extern "C" void cFEMemCard_exitDelete(void)
{
    if (cFEMemCard_mInstance->mState == 4) {
        cFEMemCard_mInstance->mState = 1;
    }
    char* fe = (char*)cFEMemCard_mInstance;
    *(int*)(fe + 0x1A8) = 0;
    *(int*)(fe + 0x1E0) = 0;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_exit__FPv);
#ifdef SKIP_ASM
/**
 * Sets the memory-card screen's state (+0x1C0) to 6.
 */
void cFEMemCard_exit(void* vself)
{
    cFEMemCard* self = (cFEMemCard*)vself;
    self->mState = 6;
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
extern "C" void cFEMemCard_startLoad(cFEMemCard* self)
{
    cFEMemCard_SetLoadPressed(self, 1);
    self->mIsProcessing = 1;
    sVEntry<void(void*)>* vt = (sVEntry<void(void*)>*)self->mVtbl;
    self->mState = 5;
    vt[47].fn(vthis(self, vt[47]));
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

static inline void notifyK1D8700(char* self, char* mp)
{
    if (*(int*)(self + 0x204) == 1) {
        *(int*)(mp + 0x440) = *(int*)(self + 0x204);
        VCALL_K1D8700(self, 40);
    }
}

/**
 * Handles a button on the memory-card screen's popup, by popup id (+0x19C).
 *
 * Buttons 0xF, 0x10 and 0x11 are choices 0, 1 and 2. "Cancel" is vtable slot 41 (cFEMemCard_cancel); "finish" means that on
 * an auto-load screen (+0x204 is 1) the manager's mCurrDeviceStateAck is set to 1 and vtable slot 40 (cFEMemCard_exit,
 * state 6) runs. Per popup:
 * - 2: choice 0 starts the load (slot 45), else screen state (+0x1C0) 1.
 * - 19: choice 0 clears the overwrite word (+0x1F0), cancels, finishes; else sets the retry word (+0x1EC).
 * - 1 and 13: choice 0 sets state 3 and +0x1F0 (overwrite), else cancel and finish.
 * - 9: choice 0 cancels and finishes, else retry. 18: choice 0 retries, else cancel and finish.
 * - 10: choice 0 cancels and finishes. 11: clears +0x1F0, cancels, finishes.
 * - 4, 5, 6: slot 40.
 * - 3: clears +0x1B0; choice 0 sets +0x1A8 and the delete word (+0x1E0), else cancel and finish.
 * - 12: choice 0 cancels, sets state 1, finishes; choice 1 retries; choice 2 opens popup 13 (cFEMemCard_callbackSaveFileCorrupt) and returns
 *   at once.
 * - 7: clears +0x1A8 and +0x1E0, cancels when the load word (+0x1F4) is set, sets +0x1A8 again when the save word
 *   (+0x1DC) is set, finishes.
 * - 16: clears the format word (+0x1E8) and the load request (cFEMemCard_SetLoadPressed 0), sets +0x1A8 only when the
 *   save word (+0x1DC) is set, finishes.
 * - 17: clears +0x1E8 and cancels. 14 and 15: clear +0x1B0; choice 0 sets +0x1A8 and +0x1E8, choice 1 clears +0x1E8,
 *   cancels, finishes.
 * - 8, 23, 24: cancel.
 *
 * Unless the popup id is 22, cUIStateStack_pop then flags the top state of the engine's state stack (+0x18) to close;
 * +0x210 is cleared. Event 22 (popup closed) clears the words +0x1A4 and +0x1A0 when +0x1A0 is set; otherwise it clears
 * +0x210 (popup id 22 only) and the instance's popup pointer (+0x218).
 *
 * @param self_  the memory-card screen
 * @param popup  the popup state that sent the event (unused)
 * @param button  the popup event: 0xF / 0x10 / 0x11 for buttons 0-2, 22 when the popup closes
 */
extern "C" void cFEMemCard_onPopupEvent(void* self_, int popup, int button)
{
    cFEMemCard* self = (cFEMemCard*)((char*)self_);
    if (button >= 0xF && button <= 17) {
        int choice = 0;
        if (button == 0x10) {
            choice = 1;
        } else if (button == 17) {
            choice = 2;
        }
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        switch (self->mPopupType) {
        case 2:
            if (choice == 0) {
                VCALL_K1D8700(self, 45);
            } else {
                self->mState = 1;
            }
            break;
        case 19:
            if (choice == 0) {
                self->mIsOverwriteOK = 0;
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            } else {
                self->mIsRetryPressed = 1;
            }
            break;
        case 1:
            if (choice != 0) {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            } else {
                self->mState = 3;
                self->mIsOverwriteOK = 1;
            }
            break;
        case 9:
            if (choice == 0) {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            } else {
                self->mIsRetryPressed = 1;
            }
            break;
        case 18:
            if (choice == 0) {
                self->mIsRetryPressed = 1;
            } else {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            }
            break;
        case 10:
            if (choice == 0) {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            }
            break;
        case 11:
            self->mIsOverwriteOK = 0;
            VCALL_K1D8700(self, 41);
            notifyK1D8700(((char*)self), mp);
            break;
        case 4:
        case 5:
        case 6:
            VCALL_K1D8700(self, 40);
            break;
        case 3:
            self->mIsLoadWait = 0;
            if (choice == 0) {
                self->mIsProcessing = 1;
                self->mIsDeletePressed = 1;
            } else {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            }
            break;
        case 12:
            if (choice == 0) {
                VCALL_K1D8700(self, 41);
                self->mState = 1;
                notifyK1D8700(((char*)self), mp);
            } else if (choice == 1) {
                self->mIsRetryPressed = 1;
            } else {
                cFEMemCard_callbackSaveFileCorrupt();
                return;
            }
            break;
        case 13:
            if (choice == 0) {
                self->mState = 3;
                self->mIsOverwriteOK = 1;
            } else {
                VCALL_K1D8700(self, 41);
                notifyK1D8700(((char*)self), mp);
            }
            break;
        case 7:
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
        case 16:
            self->mIsFormatPressed = 0;
            cFEMemCard_SetLoadPressed((cFEMemCard*)self, 0);
            if (self->mIsSavePressed != 0) {
                self->mIsProcessing = 1;
            } else {
                self->mIsProcessing = 0;
            }
            notifyK1D8700(((char*)self), mp);
            break;
        case 17:
            self->mIsFormatPressed = 0;
            VCALL_K1D8700(self, 41);
            break;
        case 14:
        case 15:
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
        case 8:
        case 23:
            VCALL_K1D8700(self, 41);
            break;
        case 24:
            VCALL_K1D8700(self, 41);
            break;
        }
        if (self->mPopupType != 22) {
            cUIStateStack_pop(&cFEMemCard_mInstance->mpEngine->mStateStack, 1);
        }
        self->mIsStatusPopupOn = 0;
    } else if (button == 22) {
        char* fe = (char*)cFEMemCard_mInstance;
        if (*(int*)(fe + 0x1A0) != 0) {
            *(int*)(fe + 0x1A4) = 0;
            *(int*)(fe + 0x1A0) = 0;
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
extern "C" void cFEMemCard_cancel(cFEMemCard* self)
{
    self->mIsProcessing = 0;
    self->mIsOverwriteOK = 0;
    self->mIsRetryPressed = 0;
    self->mIsLoadWait = 0;
    self->mIsCancelPressed = 1;
    cFEMemCard_SetLoadPressed(self, 0);
    self->mState = 2;
    self->mIsDeletePressed = 0;
    self->mIsFormatPressed = 0;
    self->mIsSavePressed = 0;
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
void cFEMemCard_exitSave(void* vself)
{
    cFEMemCard* self = (cFEMemCard*)vself;
    self->mIsProcessing = 0;
    self->mState = 1;
    self->mIsSavePressed = 0;
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
extern "C" int cFEMemCard_onInputBegin(cFEMemCard* self, cUIInput* input)
{
    int handled = 0;
    if (self->mIsProcessing == 1)
        return 0;
    sVEntry<int(void*)>* vt = (sVEntry<int(void*)>*)input->mVtbl;
    if (vt[19].fn(vthis(input, vt[19]))) {
        handled = 1;
        self->mDoPrevMemCard = handled;
        if (self->mpScreen) {
            unsigned short frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_left_arr));
            if (frame != 0xFFFF)
                cUIScreen_playFrame(self->mpScreen, frame, 1);
        }
    } else {
        sVEntry<int(void*)>* vt2 = (sVEntry<int(void*)>*)input->mVtbl;
        if (vt2[20].fn(vthis(input, vt2[20]))) {
            handled = 1;
            self->mDoNextMemCard = handled;
            if (self->mpScreen) {
                unsigned short frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_right_arr));
                if (frame != 0xFFFF)
                    cUIScreen_playFrame(self->mpScreen, frame, 1);
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
extern "C" void cFEMemCard_displayStatusPopup(cFEMemCard* self)
{
    unsigned short buf[800];
    void* obj = cSSXApp_getMCManager(SSXApp);
    self->mPopupType = 22;
    cMCOverlayManager_GetMessage(obj, buf, 800);
    sVEntry<void(void*, unsigned short*, int, int, int, int, int)>* vt = (sVEntry<void(void*, unsigned short*, int, int, int, int, int)>*)self->mVtbl;
    vt[36].fn(vthis(self, vt[36]), buf, 1, 1, 0, 0, 1);
    self->mIsStatusPopupOn = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_onUpdate);
#ifdef SKIP_ASM
// PORT: operator_new really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
void cMemMan_free(void* p);
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_GetMessage(void*, unsigned short*, int);
extern "C" void cOVState_onUpdate(void);
extern "C" void cUIState_onUpdate(void*);
extern "C" void cMCOverlayManager_ManagerStart(void*);
extern "C" int cMCOverlayManager_isManagerDone(void*);
extern "C" void cMCOverlayManager_update(void*);
extern char str_Temp_String[];
extern char str_onmenu[];

class cStreamK1D8DE0 {
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual int IsBusy();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52();
    virtual bool IsAvailable1();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66();
    virtual bool HasFileName(int);
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void v74();
    virtual int v75();
};

class cSubK1D8DE0 {
public:
    virtual void v01(int);
};

class cPopupK1D8DE0 {
public:
    int pad[2];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26();
    virtual void setMessage(unsigned short*);
};

class cMemCardK1D8DE0 {
public:
    int pad[2];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
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
    virtual void v35(int);
    virtual void v36(); virtual void v37();
    virtual void v38(int);
    virtual void v39(int, int);
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46();
    virtual void v47();
};

struct sVEntryK1D8DE0 {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

#define VCALL_K1D8DE0(obj, off, idx) (*(sVEntryK1D8DE0**)((char*)(obj) + (off)))[idx].fn((char*)(obj) + (*(sVEntryK1D8DE0**)((char*)(obj) + (off)))[idx].delta

#define BUSY_K1D8DE0(self, mp) ((*(int*)((self) + 0x1B0) == 1 && cMCOverlayManager_isManagerDone(mp) == 0) \
        || (*(int*)((self) + 0x1B0) == 0 && (*(cStreamK1D8DE0**)((mp) + 0x434))->IsBusy() != 0))

/**
 * Runs the memory-card screen's per-frame update: UI input, the memory-card manager, the screen-state handler and the
 * status display.
 *
 * Without a popup (+0x218) and with +0x1A4 clear it runs the base update: cOVState_onUpdate in game (SSXApp->Game set),
 * cUIState_onUpdate otherwise. The first time (+0x1D8 clear) it starts the manager (cMCOverlayManager_ManagerStart), sets +0x1D8,
 * display state 1 (+0x214 and triggerDisplayState, slot 35) and InitCardsCheck (49). cMCOverlayManager_update runs
 * unless the screen state (+0x1C0) is 6 and the card work is over; busy means +0x1B0 is 1 and
 * cMCOverlayManager_isManagerDone is false, or +0x1B0 is 0 and the card's IsBusy (slot 11) is true. The screen state
 * picks a vtable slot: 0 slot 31, 1 slot 27, 2 slot 32, 3 slot 28, 4 slot 30, 5 slot 29, 6 slot 33 (only when not
 * busy). With no status popup (+0x210) and neither auto-load (+0x204) nor auto-save (+0x208) set, a manager mState of
 * 11, 15-18, 25, 26, 31-34, 40 or 44 (format, save, load and delete progress) opens the status popup (slot 47); while
 * it is open its text is refreshed (cMCOverlayManager_GetMessage, popup slot 27). Then slot 26 (display status).
 * Without a popup and when the card's slot 75 (last operation's outcome) is nonzero, the selected slot (the "onmenu"
 * object's byte +0x95, else +0x1C4) decides hideDeleteButton (slot 38) and hideMainButton (slot 39): no file in the
 * slot (the card's HasFileName, slot 67) or no card (slot 53).
 *
 * @param card  the memory-card screen
 */
extern "C" void cFEMemCard_onUpdate(cMemCardK1D8DE0* card)
{
    char* self = (char*)card;
    if (*(void**)(self + 0x218) == 0 && *(int*)(self + 0x1A4) == 0) {
        if ((int)SSXApp->Game != 0) {
            cOVState_onUpdate();
        } else {
            cUIState_onUpdate(self);
        }
    }
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    if (*(int*)(self + 0x1D8) == 0) {
        cMCOverlayManager_ManagerStart(mp);
        *(int*)(self + 0x1D8) = 1;
        *(int*)(self + 0x214) = 1;
        VCALL_K1D8DE0(self, 8, 35), 1);
        VCALL_K1D8DE0(mp, 1864, 1), 49);
    }
    if (*(int*)(self + 0x1C0) != 6 || BUSY_K1D8DE0(self, mp)) {
        cMCOverlayManager_update(mp);
    }
    switch (*(int*)(self + 0x1C0)) {
    case 0:
        VCALL_K1D8DE0(self, 8, 31));
        break;
    case 1:
        VCALL_K1D8DE0(self, 8, 27));
        break;
    case 2:
        VCALL_K1D8DE0(self, 8, 32));
        break;
    case 3:
        VCALL_K1D8DE0(self, 8, 28));
        break;
    case 4:
        VCALL_K1D8DE0(self, 8, 30));
        break;
    case 5:
        VCALL_K1D8DE0(self, 8, 29));
        break;
    case 6:
        {
            char* mp2 = (char*)cSSXApp_getMCManager(SSXApp);
            if (!BUSY_K1D8DE0(self, mp2))
                VCALL_K1D8DE0(self, 8, 33));
        }
        break;
    }
    if (cFEMemCard_mInstance->mIsStatusPopupOn == 0 && cFEMemCard_mInstance->mAutoSaveOn == 0 && cFEMemCard_mInstance->mAutoLoadOn == 0) {
        int managerState = *(int*)(mp + 0x130);
        if (managerState == 40 || managerState == 11 || managerState == 0xF || managerState == 0x10 || managerState == 17 || managerState == 18 || managerState == 0x1F
            || managerState == 33 || managerState == 0x20 || managerState == 34 || managerState == 44 || managerState == 25 || managerState == 26) {
            VCALL_K1D8DE0(self, 8, 47));
        }
    }
    if (cFEMemCard_mInstance->mIsStatusPopupOn != 0) {
        unsigned short* buf = (unsigned short*)operator_new_tag(2000, str_Temp_String, 0x100, 0);
        cMCOverlayManager_GetMessage(mp, buf, 800);
        (*(cPopupK1D8DE0**)(self + 0x218))->setMessage(buf);
        if (buf != 0) {
            cMemMan_free(buf);
        }
    }
    VCALL_K1D8DE0(self, 8, 26));
    if (*(void**)(self + 0x218) == 0 && (*(cStreamK1D8DE0**)(mp + 0x434))->v75() != 0) {
        void* menu = cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(str_onmenu));
        int sel = *(int*)(self + 0x1C4);
        if (menu != 0) {
            sel = *(unsigned char*)((char*)menu + 0x95);
        }
        sVEntryK1D8DE0* vt = *(sVEntryK1D8DE0**)(self + 8);
        void (**pf)(void*, ...) = &vt[38].fn;
        (*pf)(vthis(self, vt[38]), (*(cStreamK1D8DE0**)(mp + 0x434))->HasFileName(sel) == 0 || (*(cStreamK1D8DE0**)(mp + 0x434))->IsAvailable1() == 0);
        bool slotEmpty = !(*(cStreamK1D8DE0**)(mp + 0x434))->HasFileName(sel);
        bool cardMissing = !(*(cStreamK1D8DE0**)(mp + 0x434))->IsAvailable1();
        VCALL_K1D8DE0(self, 8, 39), slotEmpty, cardMissing);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_setMenuHighlight);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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
extern "C" void cFEMemCard_setMenuHighlight(cFEMemCard* self)
{
    void* menu = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_onmenu));
    if (menu != 0) {
        void* item = *(void**)((char*)menu + 0xA0);
        int idx = *(unsigned char*)((char*)menu + 0x95);
        if (item == 0 || ((*(int*)((char*)item + 0x14) >> 5) & 1)) {
            idx = cUIMenu_findFirstHighlightable(menu, 0);
        }
        if (idx != 0xFF) {
            cUIMenu_setSelectedByIndex(menu, idx);
            self->mSlot = idx;
        } else {
            cUIMenu_setSelectedByIndex(menu, 0);
            self->mSlot = 0;
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
extern "C" void cFEMemCard_SetLoadPressed(cFEMemCard* self, int on)
{
    self->mIsLoadPressed = on;
    if (on) {
        char* mcManager = (char*)cSSXApp_getMCManager(SSXApp);
        char* obj = *(char**)(mcManager + 0x434);
        sVEntry<void(void*, void*, void*)>* vt = *(sVEntry<void(void*, void*, void*)>**)obj;
        vt[14].fn(vthis(obj, vt[14]), *(void**)(mcManager + 0xF8), (char*)&self->mLastLoadFileName);
    } else {
        *(short*)&self->mLastLoadFileName[0] = 0;
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateStart);
#ifdef SKIP_ASM

/**
 * Switches the memory-card screen to display state 1 unless it is already there.
 *
 * When the display state (+0x214) is not 1, sets it to 1 and calls vtable entry 35 (cFEMemCard_triggerDisplayState)
 * with 1. Fills vtable entry 31 (+0xFC) of the memory-card states (cFEStateMemCard, cFEStateProfileLoad,
 * cFEStateOptionsDeviceSelect), between the empty update entries and the skip handler.
 */
extern "C" void cFEMemCard_updateStart(cFEMemCard* self)
{
    if (self->mScreenStatus != 1) {
        self->mScreenStatus = 1;
        sVEntry<void(void*, int)>* vt = (sVEntry<void(void*, int)>*)self->mVtbl;
        vt[35].fn(vthis(self, vt[35]), 1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateChecking__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the memory-card screen's empty update handler in vtable entry 27, +0xDC).
 */
void cFEMemCard_updateChecking(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateSkip__FPv);
#ifdef SKIP_ASM
/**
 * Sets the memory-card screen state (+0x1C0) to 1.
 */
void cFEMemCard_updateSkip(void* vself)
{
    cFEMemCard* self = (cFEMemCard*)vself;
    self->mState = 1;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateSaving__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the memory-card screen's empty update handler in vtable entry 28, +0xE4).
 */
void cFEMemCard_updateSaving(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateDeleting__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the memory-card screen's empty update handler in vtable entry 30, +0xF4).
 */
void cFEMemCard_updateDeleting(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_updateLoading__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the memory-card screen's empty update handler in vtable entry 29, +0xEC).
 */
void cFEMemCard_updateLoading(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_onLoseTransition);
#ifdef SKIP_ASM
/**
 * Reports whether the memory-card screen may finish its leaving transition: the screen state (+0x1C0) is 6.
 *
 * Installed in the cUIState onLoseTransition entry (vtable entry 10) of the memory-card states.
 *
 * @return 1 when the screen state is 6, else 0
 */
extern "C" int cFEMemCard_onLoseTransition(cFEMemCard* self)
{
    return self->mState == 6;
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_triggerArrowPulse);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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


struct sVEntryF1D9470 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

/**
 * Switches the memory-card screen's display state: moves the menu focus and jumps to that state's frame.
 *
 * Stores @p state at +0x214. States 1 and 2 take the focus from "onmenu" to "offmenu", jump to the "off" or
 * "file display" frame label and show "kT_MEMFECheckingCardPS2" for the selected slot (mSelectedDevice + 1)
 * in "text box message". State 3 focuses "onmenu" and jumps to "on"; any other value is only stored.
 *
 * @param state  the display state, 1 to 3 (On Tour gotoScreenState)
 */
extern "C" void cFEMemCard_triggerDisplayState(cFEMemCard* self, int state)
{
    unsigned short buf[0x100];
    int frame = 0xFFFF;
    self->mScreenStatus = state;
    switch (state) {
    case 1: {
        char* widget = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_onmenu));
        if (widget) {
            sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(widget + 8);
            vt[7].fn(vthis(widget, vt[7]), 0);
        }
        widget = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_offmenu));
        if (widget) {
            sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(widget + 8);
            vt[7].fn(vthis(widget, vt[7]), 1);
        }
        frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_off));
        char* text = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_text_box_message));
        if (text) {
            unsigned short* str = (unsigned short*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
            char* mp = (char*)cSSXApp_getMCManager(SSXApp);
            cMCOverlayManager_getDeviceNumberString(mp, buf, *(int*)(mp + 0x428));
            char* locale = (char*)SSXApp->FELocale;
            sVEntryF1D9470* mvt = *(sVEntryF1D9470**)(locale + 4);
            sprintf1(str, mvt[4].fn(vthis(locale, mvt[4]), GetHashValue32(str_kT_MEMFECheckingCardPS2)), buf);
            cUIText_setUnicodeStringWrap(text, str);
            sVEntry<void(void*, int)>* tvt = *(sVEntry<void(void*, int)>**)(text + 8);
            tvt[9].fn(vthis(text, tvt[9]), 1);
            if (str)
                cMemMan_free(str);
        }
        break;
    }
    case 2: {
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        char* widget = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_onmenu));
        if (widget) {
            sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(widget + 8);
            vt[7].fn(vthis(widget, vt[7]), 0);
        }
        widget = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_offmenu));
        if (widget) {
            sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(widget + 8);
            vt[7].fn(vthis(widget, vt[7]), 1);
        }
        frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_file_display));
        char* text = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_text_box_message));
        if (text) {
            unsigned short* str = (unsigned short*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
            cMCOverlayManager_getDeviceNumberString(mp, buf, *(int*)(mp + 0x428));
            char* locale = (char*)SSXApp->FELocale;
            sVEntryF1D9470* mvt = *(sVEntryF1D9470**)(locale + 4);
            sprintf1(str, mvt[4].fn(vthis(locale, mvt[4]), GetHashValue32(str_kT_MEMFECheckingCardPS2)), buf);
            cUIText_setUnicodeStringWrap(text, str);
            sVEntry<void(void*, int)>* tvt = *(sVEntry<void(void*, int)>**)(text + 8);
            tvt[9].fn(vthis(text, tvt[9]), 1);
            if (str)
                cMemMan_free(str);
        }
        break;
    }
    case 3: {
        char* widget = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_onmenu));
        if (widget) {
            sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(widget + 8);
            vt[7].fn(vthis(widget, vt[7]), 1);
        }
        widget = (char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_offmenu));
        if (widget) {
            sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)(widget + 8);
            vt[7].fn(vthis(widget, vt[7]), 0);
        }
        frame = cUIScreen_getFrameByLabel(self->mpScreen, GetHashValue32(str_on));
        break;
    }
    }
    if (frame != 0xFFFF)
        cUIScreen_jumpToFrame(self->mpScreen, frame);
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
extern "C" int cFEMemCard_checkNextAvailableDevice(cFEMemCard* self, cMCOverlayManager* list, int idx, int back)
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
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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
    void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_text_box_message));
    if (obj != 0) {
        sVEntry<void(void*, int)>* vt = *(sVEntry<void(void*, int)>**)((char*)obj + 8);
        vt[9].fn(vthis(obj, vt[9]), 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_onLoseFocus__FPv);
#ifdef SKIP_ASM
/**
 * Does nothing (the memory-card screen's onLoseFocus handler, vtable entry 11).
 */
void cFEMemCard_onLoseFocus(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/fememcardcallbacks", cFEMemCard_displayFileOK);
#ifdef SKIP_ASM
class cMovieStreamK1D9880 {
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10();
    virtual int IsBusy();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual int v75();
};

static inline bool IsModeK1D9880(char* mp, int m) { return *(int*)(mp + 0x130) == m; }

/**
 * Reports whether the memory-card screen may show the file display now.
 *
 * True only when the screen state (+0x1C0) is neither 0 nor 6, the memory-card overlay manager
 * (cSSXApp_getMCManager(SSXApp)) has mCheckAllowed set and mGetDir clear, its card (mpMemCard) answers 0 from virtual
 * slot 11, the manager's mState is neither 6 nor 1, and the card answers non-zero from virtual slot 75.
 *
 * @return 1 when all the checks pass, else 0
 */
extern "C" int cFEMemCard_displayFileOK(cFEMemCard* self)
{
    int allowed = 0;
    int one = 1;
    char* mp = (char*)cSSXApp_getMCManager(SSXApp);
    int st = self->mState;
    if (st != 0 && st != 6 && *(int*)(mp + 0x340) != 0 && *(int*)(mp + 0xE0) == 0
        && (*(cMovieStreamK1D9880**)(mp + 0x434))->IsBusy() == 0) {
        if (!IsModeK1D9880(mp, 6) && !IsModeK1D9880(mp, 1)) {
            if ((*(cMovieStreamK1D9880**)(mp + 0x434))->v75() != 0) {
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
 * buffer") goes to +0x1D0 and +0x1D4, and +0x228 is set to 1.
 *
 * @param size  size of the buffer in bytes (On Tour iSize)
 */
extern "C" void cFEMemCard_createReadBuffer(cFEMemCard* self, unsigned int size)
{
    if (self->mBufferState == 1) {
        void* buf = self->mpStartReadBuffer;
        if (buf != 0) {
            cMemMan_free(buf);
        }
        self->mpStartReadBuffer = 0;
        self->mBufferState = 0;
    }
    void* readBuffer = operator_new_tag(size, str_read_buffer, 0x100, 0);
    self->mpReadBuffer = (char*)readBuffer;
    self->mpStartReadBuffer = (char*)readBuffer;
    self->mBufferState = 1;
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
    self->mVtbl = (void**)cFEPopupConfirm_vtbl;
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
extern char D_004A1408[];

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
        func_002C2540(displayText, D_004A1408);
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
extern "C" void cFEPopupConfirm_setButtonText(cFEPopupConfirm* self, void* src, int mode)
{
    void* slots = (char*)&self->info;
    if (mode == 0) {
        void* buttonText = (char*)&self->mYesText[0];
        strcpy1(buttonText, src);
        func_001C57A0(slots, 0, (int)buttonText); // PORT: pointer passed as int
    } else if (mode == 1) {
        if (self->mNumButtons >= 2) {
            void* buttonText = (char*)&self->mNoText[0];
            strcpy1(buttonText, src);
            func_001C57A0(slots, 1, (int)buttonText);
        }
    } else {
        void* buttonText = (char*)&self->mAltText[0];
        self->mNumButtons = 3;
        self->info.meType = 3;
        strcpy1(buttonText, src);
        func_001C57A0(slots, 2, (int)buttonText);
    }
    if ((int)self->mpScreen != 0) {
        cScreenPopup_redrawPopup_self(self);
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
extern "C" void cFEPopupConfirm_setMessage(cFEPopupConfirm* self, void* src)
{
    if (src != 0 && strlen1(src) != 0) {
        void* dst = (char*)&self->mDisplayText[0];
        strcpy1(dst, src);
        self->info.mtMessage.pustr = (const unsigned short*)dst;
        self->info.mtMessage.eType = 1;
        if ((int)self->mpScreen != 0) {
            cScreenPopup_redrawPopup_self(self);
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
    self->mVtbl = (void**)cFEStateNetHelpPopup_vtbl;
    new ((char*)&self->muiScreenName) sMcSlot9D48[7];
    self->miReleasedCount = 0;
    self->mbReferEventsToParent = 0;
    self->mHashName = 76;
    cFEStateNetHelpPopup_clear(self);
    return ((void*)self);
}
#endif

