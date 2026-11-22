#include "vtable.h"
#include "common.h"
#include "fe/keyboardpopup.h"
#include "movie/mcoverlaymanager.h"
#include "ui/uiengine.h"
#include "ui/uithing.h"
#include "be/beintnewrace.h"
#include "fe/ovstateprofile.h"
#include "fe/ovstateautosave.h"
#include "main/ssxapp.h"

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVStateManager_addPDATemplate();
extern "C" void cOVState_onCreateScreen(void* self);
extern "C" void cOVState_PROFILE_initGeneral(cOVState_PROFILE* self);
extern char str_121Profilesave_pda[];

/**
 * Creates the in-game profile save overlay screen ("121Profilesave_pda").
 *
 * Adds the PDA template, numbers the slots (initGeneral) and runs cOVState_onCreateScreen.
 */
extern "C" void cOVState_PROFILE_onCreateScreen(cOVState_PROFILE* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_121Profilesave_pda), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVStateManager_addPDATemplate();
    cOVState_PROFILE_initGeneral(self);
    cOVState_onCreateScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_displayStatus);
#ifdef SKIP_ASM
extern "C" void cOVState_PROFILE_displayOn(cOVState_PROFILE* self);
extern "C" void cOVState_PROFILE_displayOff(cOVState_PROFILE* self);

/**
 * Refreshes the profile overlay's memory-card display: the file list when the screen is in display state 3, the empty
 * layout otherwise.
 *
 * When the card state +0x1C0 (On Tour mState) is nonzero it first sets +0x22C, the flag that lets cOVState_PROFILE_hideActionButtons show
 * the action buttons. Display state +0x214 (On Tour mScreenStatus, set by cFEMemCard_triggerDisplayState) 3 runs
 * cOVState_PROFILE_displayOn, any other cOVState_PROFILE_displayOff. It is the displayStatus slot (+0xD4) of cOVState_PROFILE_vtbl.
 */
extern "C" void cOVState_PROFILE_displayStatus(cOVState_PROFILE* self)
{
    if (self->mState != 0) {
        self->mIsOn = 1;
    }
    if (self->mScreenStatus == 3) {
        cOVState_PROFILE_displayOn(self);
    } else {
        cOVState_PROFILE_displayOff(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_displayOff);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_PROFILE_displayOn(cOVState_PROFILE* self);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_playerpct_d_b[];

struct cSelf_11270 {
    int f0;
    int f4;
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
    virtual void setState(int s);
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual int isReady();
};
struct cCard_11270 {
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
    virtual void v52();
    virtual int isPresent();
};
struct cUIObj_11270 {
    int f0;
    int f4;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setStatic(int v);
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
    virtual void setUseStaticColour(int v);
};

/**
 * Shows the profile overlay's empty layout, or switches to the file list as soon as a card is in.
 *
 * When cFEMemCard_displayFileOK (virtual slot 42) allows the file display and the memory card (mpMemCard of cSSXApp_getMCManager,
 * its slot 53) is present, moves to display state 3 through cFEMemCard_triggerDisplayState (virtual slot 35) and runs
 * cOVState_PROFILE_displayOn. Otherwise resets the six profile buttons "player1" ... "player6" of mpScreen:
 * setUseStaticColour(0) (slot 25) and setStatic(1) (slot 8).
 *
 * @param vself  the profile overlay state (this)
 */
extern "C" void cOVState_PROFILE_displayOff(cOVState_PROFILE* vself)
{
    cSelf_11270* self = (cSelf_11270*)vself;
    if (self->isReady() != 0) {
        if ((*(cCard_11270**)((char*)cSSXApp_getMCManager(SSXApp) + 0x434))->isPresent() != 0) {
            self->setState(3);
            cOVState_PROFILE_displayOn((cOVState_PROFILE*)self);
            return;
        }
    }
    for (int i = 1; i < 7; i++) {
        char buf[64];
        sprintf(buf, str_playerpct_d_b, i);
        cUIObj_11270* playerButton = (cUIObj_11270*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
        playerButton->setUseStaticColour(0);
        playerButton->setStatic(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_displayOn);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
class cMCSub_1380 {
public:
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual void GetTitleString(int, void*);
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual int v35();
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int v42();
    virtual int v43();
    virtual int v44();
    virtual int v45();
    virtual int v46();
    virtual int v47();
    virtual int v48();
    virtual int v49();
    virtual int v50();
    virtual int v51();
    virtual int v52();
    virtual int IsAvailable1();
    virtual int v54(int);
    virtual int IsFormatted1();
    virtual int v56();
    virtual int v57();
    virtual int v58();
    virtual int v59();
    virtual int v60();
    virtual int v61();
    virtual int v62();
    virtual int v63();
    virtual void GetStatus1(void*);
    virtual int v65();
    virtual int IsFileSlotDisabled(int);
    virtual int HasFileName(int);
};

class cState_1380 {
public:
    int pad[2];
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual int v08();
    virtual int v09();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual int v25();
    virtual int v26();
    virtual int v27();
    virtual int v28();
    virtual int v29();
    virtual int v30();
    virtual int v31();
    virtual int v32();
    virtual int v33();
    virtual int v34();
    virtual void triggerDisplayState(int);
    virtual int v36();
    virtual int v37();
    virtual int v38();
    virtual int v39();
    virtual int v40();
    virtual int v41();
    virtual int displayFileOK();
    virtual void setMenuHighlight();
};

class cUIObj_1380 {
public:
    int pad[2];
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual int v04();
    virtual int v05();
    virtual int v06();
    virtual int v07();
    virtual void setStatic(int);
    virtual void setVisible(int);
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual int v14();
    virtual int v15();
    virtual int v16();
    virtual int v17();
    virtual int v18();
    virtual int v19();
    virtual int v20();
    virtual int v21();
    virtual int v22();
    virtual int v23();
    virtual int v24();
    virtual void setUseStaticColour(int);
};

struct sVE_1380 {
    short delta;
    short index;
    unsigned short* (*fn)(void*, int);
};

class cG_1380 {
public:
    int pad;
    virtual int v01();
    virtual int v02();
    virtual int v03();
    virtual void v04(int);
};

int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cFEMemCard_callbackDisplayNoCards();
extern "C" void cOVState_PROFILE_displayOff(cOVState_PROFILE* self);
extern "C" int cFEMemCard_checkNextAvailableDevice(void* self, void* mp, int dev, int a3);
extern "C" void cMCOverlayManager_GetDeviceDisplayString(void* mp, int dev, unsigned short* buf, int len);
extern "C" void cUIText_setUnicodeStringWrap(void* obj, unsigned short* s);
extern "C" void cUIText_setUnicodeString(void* obj, unsigned short* s);
extern "C" void* cSSXApp_getMCManager(void* self);
extern char str_1percentage_b[];
extern char str_playerpct_d_b[];
extern char str_Memory_card_text_b[];
extern char str_arrowleft_b[];
extern char str_arrowright_b[];

struct sCol_1380 {
    float r, g, b, a;
};

extern sCol_1380 D_004C66C8_1380 __asm__("gStaticColour");

static inline cUIObj_1380* getObj_1380(cState_1380* self, char* name)
{
    return (cUIObj_1380*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
}

/**
 * Shows the profile overlay's file list for the selected memory card: the card's name, the six profile slots and the
 * device arrows.
 *
 * With the memory-card manager (cSSXApp_getMCManager) mp: when cFEMemCard_displayFileOK (virtual slot 42) allows the file display
 * but the card (mpMemCard slot 53) is missing, it shows the no-card message (cFEMemCard_callbackDisplayNoCards, the front end's
 * callbackDisplayNoCards) and falls back to cOVState_PROFILE_displayOff; otherwise it switches to display state 3
 * (cFEMemCard_triggerDisplayState, virtual slot 35) if not there yet. Then, still allowed: writes the device name of
 * mSelectedDevice (cMCOverlayManager_GetDeviceDisplayString) into "Memory card text", stores mSelectedDevice at +0x21C
 * (On Tour mLastSelectedDevice), runs cFEMemCard_setMenuHighlight (slot 43), colours "1percentage" with gStaticColour,
 * fills it from card slot 64 and hides it (slot 9). Each "player%d" button 1-6 gets the name of card file i - 1 (card
 * slot 14, through the 16-bit buffer at +0x234), and its state from the card: when slots 67 and 66 both say no for that
 * file and slot 55 says yes, the first such button is made selectable (setStatic(0), slot 8) and later ones static
 * and greyed (setUseStaticColour(1), slot 25, and setStatic(1)); otherwise, when slot 54 accepts mSelectedDevice, a
 * button is static and greyed exactly when slot 66 says yes for its file (else selectable and normally coloured); with
 * no usable device every button is static and greyed. Finally
 * "arrowleft" / "arrowright" are shown only when cFEMemCard_checkNextAvailableDevice finds a device in that direction.
 *
 * @param profileState  the profile overlay state (this)
 */
// Refreshes the memory-card profile list: device name, then the six profile slots and the arrow hints.
extern "C" void cOVState_PROFILE_displayOn(cOVState_PROFILE* profileState)
{
    cState_1380* self = (cState_1380*)profileState;
    unsigned short buf[800];
    char name[0x40];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    if (self->displayFileOK()) {
        if (!((cMCSub_1380*)mp->mpMemCard)->IsAvailable1()) {
            cFEMemCard_callbackDisplayNoCards();
            cOVState_PROFILE_displayOff((cOVState_PROFILE*)self);
            return;
        }
        if (*(int*)((char*)self + 0x214) != 3)
            self->triggerDisplayState(3);
    }
    if (self->displayFileOK()) {
        cMCOverlayManager_GetDeviceDisplayString(mp, mp->mSelectedDevice, buf, 800);
        int first = 0;
        cUIText_setUnicodeStringWrap(getObj_1380(self, str_Memory_card_text_b), buf);
        *(int*)((char*)self + 0x21C) = mp->mSelectedDevice;
        self->setMenuHighlight();
        cUIObj_1380* textWidget = getObj_1380(self, str_1percentage_b);
        *(sCol_1380*)((char*)textWidget + 0x88) = D_004C66C8_1380;
        ((cMCSub_1380*)mp->mpMemCard)->GetStatus1(buf);
        cUIText_setUnicodeString(textWidget, buf);
        textWidget->setVisible(0);
        for (int i = 1; i < 7; i++) {
            sprintf(name, str_playerpct_d_b, i);
            textWidget = getObj_1380(self, name);
            ((cMCSub_1380*)mp->mpMemCard)->GetTitleString(i - 1, (char*)self + 0x234);
            cUIText_setUnicodeString(textWidget, (unsigned short*)((char*)self + 0x234));
            if (!((cMCSub_1380*)mp->mpMemCard)->HasFileName(i - 1) && !((cMCSub_1380*)mp->mpMemCard)->IsFileSlotDisabled(i - 1)
                && ((cMCSub_1380*)mp->mpMemCard)->IsFormatted1()) {
                if (!first) {
                    first = 1;
                    textWidget->setStatic(0);
                } else {
                    textWidget->setUseStaticColour(1);
                    textWidget->setStatic(1);
                }
            } else if (((cMCSub_1380*)mp->mpMemCard)->v54(mp->mSelectedDevice)) {
                if (((cMCSub_1380*)mp->mpMemCard)->IsFileSlotDisabled(i - 1)) {
                    textWidget->setStatic(1);
                    textWidget->setUseStaticColour(1);
                } else {
                    textWidget->setStatic(0);
                    textWidget->setUseStaticColour(0);
                }
            } else {
                textWidget->setUseStaticColour(1);
                textWidget->setStatic(1);
            }
        }
        if (cFEMemCard_checkNextAvailableDevice(self, mp, mp->mSelectedDevice, 1) == -1)
            getObj_1380(self, str_arrowleft_b)->setVisible(0);
        else
            getObj_1380(self, str_arrowleft_b)->setVisible(1);
        if (cFEMemCard_checkNextAvailableDevice(self, mp, mp->mSelectedDevice, 0) == -1)
            getObj_1380(self, str_arrowright_b)->setVisible(0);
        else
            getObj_1380(self, str_arrowright_b)->setVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_hideActionButtons);
#ifdef SKIP_ASM
class cUIObj_211850 {
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
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x_b[];
extern char str_x_save[];
extern char str_delete_text[];
extern char str_ps2sq_b[];

/**
 * Hides or shows the profile overlay's action buttons ("ps2x_b", "x_save", "delete text", "ps2sq_b"), skipping the work
 * when nothing changes.
 *
 * The buttons stay hidden while +0x22C is clear; once it is set (cOVState_PROFILE_displayStatus sets it when the card state +0x1C0 is
 * nonzero), @p show decides. The new hidden state is compared with the one cached at +0x230; on a change every button
 * gets setVisible(!hidden) (slot 9; the two button icons are null-checked, the texts are not) and the cache is updated.
 * It is the hideActionButtons slot (+0x12C) of cOVState_PROFILE_vtbl.
 *
 * @param show  nonzero hides the buttons (On Tour bHide): despite its name a true value means hidden
 */
extern "C" void cOVState_PROFILE_hideActionButtons(cOVState_PROFILE* self, bool show)
{
    bool on = true;
    if (self->mIsOn != 0) {
        on = show;
    }
    if (self->mIsHidden != on) {
        int off = !on;
        cUIObj_211850* xButton = (cUIObj_211850*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x_b));
        if (xButton != 0) {
            xButton->setVisible(off);
        }
        ((cUIObj_211850*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_x_save)))->setVisible(off);
        ((cUIObj_211850*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_delete_text)))->setVisible(off);
        cUIObj_211850* squareButton = (cUIObj_211850*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq_b));
        if (squareButton != 0) {
            squareButton->setVisible(off);
        }
        self->mIsHidden = on;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_hideDeleteButton);
#ifdef SKIP_ASM
class cUIObj_211970 {
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
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_delete_text[];
extern char str_ps2sq_b[];

/**
 * Hides or shows the profile overlay's delete button ("delete text" and "ps2sq_b") and records whether delete is
 * available.
 *
 * Both widgets get setVisible(!@p on) (slot 9, not null-checked) and +0x220 (On Tour mDeleteButtonActive) becomes
 * !@p on. It is the hideDeleteButton slot (+0x134) of cOVState_PROFILE_vtbl.
 *
 * @param on  nonzero hides the button (On Tour bHide)
 */
extern "C" void cOVState_PROFILE_hideDeleteButton(cOVState_PROFILE* self, bool on)
{
    ((cUIObj_211970*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_delete_text)))->setVisible(!on);
    ((cUIObj_211970*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq_b)))->setVisible(!on);
    self->mDeleteButtonActive = !on;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_hideMainButton);
#ifdef SKIP_ASM
class cUIObj_211A08 {
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
    virtual void setVisible(int v);
};
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x_b[];
extern char str_x_save[];

/**
 * Hides or shows the profile overlay's main (save) button ("ps2x_b" and "x_save") and records whether it is available.
 *
 * "x_save" and the X icon get setVisible(!@p on) (slot 9; neither is null-checked) and +0x224 (On Tour
 * mMainButtonActive) becomes !@p on. @p loadBlocked is not used. It is the hideMainButton slot (+0x13C) of
 * cOVState_PROFILE_vtbl.
 *
 * @param loadBlocked  unused (On Tour bLoadHide)
 * @param on  nonzero hides the button (On Tour bSaveHide)
 */
extern "C" void cOVState_PROFILE_hideMainButton(cOVState_PROFILE* self, int loadBlocked, bool on)
{
    cUIObj_211A08* xButton = (cUIObj_211A08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x_b));
    ((cUIObj_211A08*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_x_save)))->setVisible(!on);
    self->mMainButtonActive = !on;
    xButton->setVisible(!on);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_updateDone);
#ifdef SKIP_ASM
extern "C" void cGame_exit(void* game, int a);
extern "C" void cUIStateStack_popToStateInclusive(void* self);

/**
 * Leaves the profile overlay once its memory-card work is done: quits the game when it was opened for overlay 0x18,
 * otherwise pops the UI state stack.
 *
 * Runs only the first time (+0x1AC, On Tour mIsDone, clear): when +0x278 (the constructor's last argument, On Tour
 * tOVStateEnum ov) is 0x18 it calls cGame_exit(SSXApp->Game, 0), else cUIStateStack_popToStateInclusive (On Tour
 * cUIStateStack::popToStateInclusive) on mpEngine's state stack (+0x18). Then sets +0x1AC. It is the updateDone slot
 * (+0x10C) of cOVState_PROFILE_vtbl.
 */
extern "C" void cOVState_PROFILE_updateDone(cOVState_PROFILE* self)
{
    if (self->mIsDone == 0) {
        if (self->mParentOV == 0x18) {
            cGame_exit(SSXApp->Game, 0);
        } else {
            cUIStateStack_popToStateInclusive(&self->mpEngine->mStateStack);
        }
    }
    self->mIsDone = 1;
}
#endif

extern "C" void* cFEMemCard_onDestroyScreen(void*);

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_onDestroyScreen__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the profile overlay's screens: runs the shared profile screen teardown, then clears the word at
 * +0x1A4.
 *
 * The teardown, cFEMemCard_onDestroyScreen(), calls helpers of back-end interface 5, runs three memory card manager calls
 * when its word at +0x434 is set, and frees the buffer at +0x1D4 when one is held.
 */
void cOVState_PROFILE_onDestroyScreen(void* vself)
{
    cOVState_PROFILE* self = (cOVState_PROFILE*)vself;
    cFEMemCard_onDestroyScreen(self);
    self->mpKeyboard = 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_onWidgetMenu);
#ifdef SKIP_ASM
class cOvl211B38 {
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
    virtual void v52();
    virtual void v53();
    virtual int v54(void*);
};

extern "C" void* cSSXApp_getMCManager(void* app);

/**
 * Profile overlay's menu-event filter: events 6 and 8 get 0x100 when the selected memory card is not usable, event 9
 * always 0x100, everything else 0x101.
 *
 * Usability is slot 54 of the manager's mpMemCard for mSelectedDevice. It is the onWidgetMenu slot (+0xA4) of
 * cOVState_PROFILE_vtbl.
 *
 * @param menu  the menu (unused)
 * @param key  the menu event
 * @return 0x100 or 0x101 as above
 */
extern "C" int cOVState_PROFILE_onWidgetMenu(cOVState_PROFILE* self, int menu, unsigned int key)
{
    char* mgr = (char*)cSSXApp_getMCManager(SSXApp);
    switch (key) {
    case 6:
    case 8:
        if ((*(cOvl211B38**)(mgr + 0x434))->v54(*(void**)(mgr + 0x428)) == 0) return 0x100;
        break;
    case 9:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strlen(const char* s);
extern "C" void cBENewPlayerInterface_setPlayerName(void* player, int idx, char* name);
extern "C" void* cBENewPlayerInterface_getPlayerName(void* player, int idx);
extern "C" void cMCOverlayManager_writeSetFileType(void* app, int a1, int a2);
extern "C" void* cKeyboardPopup_cKeyboardPopup(void* mem, void* engine, void* owner, int a3, int a4);
extern "C" void cKeyboardPopup_setInputText(void* kb, void* name);
extern "C" void cKeyboardPopup_setMaxCharacters(void* kb, int a1);
extern "C" void cKeyboardPopup_setMinCharacters(void* kb, int a1);
extern "C" void cKeyboardPopup_setIncludeKey(void* kb, int key, int a2, int a3);
extern "C" void cKeyboardPopup_setDisableShiftNumbers(void* kb, int a1);
extern "C" void cKeyboardPopup_setStatic(void* kb, void* text);
extern "C" void cMCOverlayManager_SetAction(void* mp, int a1);
extern "C" void func_002C2540(void* dst, char* src);
extern "C" void strcpy2(char* dst, void* src);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern char str_kT_MEMPlayerName_c[];
extern char str_Keyboard_b[];
extern char str_kT_KEYUserName_b[];

struct sVEntryK211BC8 {
    short delta;
    short index;
    void* (*fn)(void*, ...);
};

struct sVEntryVK211BC8 {
    short delta;
    short index;
    void (*fn)(void*, ...);
};

#define VCALLV_K211BC8(obj, off, idx) (*(sVEntryVK211BC8**)((char*)(obj) + (off)))[idx].fn((char*)(obj) + (*(sVEntryVK211BC8**)((char*)(obj) + (off)))[idx].delta

#define VCALL_K211BC8(obj, off, idx) (*(sVEntryK211BC8**)((char*)(obj) + (off)))[idx].fn((char*)(obj) + (*(sVEntryK211BC8**)((char*)(obj) + (off)))[idx].delta



struct sMgrK211BC8 {
    char pad0[0xF8];
    int fF8;                 // 0xF8
};

static inline void* GetText_K211BC8(char* key)
{
    char* db = (char*)SSXApp->FELocale;
    sVEntryK211BC8* vt = *(sVEntryK211BC8**)(db + 4);
    return vt[4].fn(vthis(db, vt[4]), GetHashValue32(key));
}

/**
 * Handles the profile overlay's widget events: picking a profile slot, naming it on the on-screen keyboard, loading it,
 * and backing out.
 *
 * Event 13 (the keyboard finished): widens the typed name (+0x74 of the keyboard at +0x1A4) into the manager's
 * mNewFileName, gives the profile +0x1CC (On Tour mPlayerIdx) that name through cBENewPlayerInterface_setPlayerName on the player interface
 * (back-end interface 1), or, for an empty name, the localised "kT_MEMPlayerName" format with the profile number; then
 * calls the interface's virtual slot 1, starts the save with cMCOverlayManager_writeSetFileType(manager, 1, profile), and sets +0x1A8
 * (processing), +0x1DC (save pressed) and the card state +0x1C0 = 3.
 *
 * Event 5 (select) on an item, when not processing and in display state 3 (+0x214): the item's misc data becomes both
 * +0x1C4 (On Tour mSlot) and the manager's mFileCursor. In card state 1 it opens the name keyboard: on first use builds
 * a cKeyboardPopup ("Keyboard", 1092 bytes) for mpEngine with the state's controller mask, id 55, at most 8 and at
 * least 1 character, and a set of keys switched off through cKeyboardPopup_setIncludeKey; then sets its prompt to the
 * localised "kT_KEYUserName", its text to the profile's current name (cBENewPlayerInterface_getPlayerName) and pushes
 * it as a popup; +0x1A0 is set and +0x19C cleared.
 *
 * Event 6 (back), unless processing or already in card state 6: sets +0x1E4 (cancel pressed), puts the manager in state
 * 0 through its SetState slot (vtable at +0x748, slot 1) when the card state is 0, 1 or 2, and sets card state 6. Event
 * 7, under the same condition and when mpMemCard slot 54 accepts mSelectedDevice: takes the item's slot like event 5
 * and, in card state 1 when slot 67 accepts that slot, sets processing, card state 4,
 * cMCOverlayManager_SetAction(manager, 0) and manager state 0x18. Event 14 and others do nothing.
 *
 * @param vself  the profile overlay state (this)
 * @param item  the widget; its misc data (+0x18) is the profile slot
 * @param event  the widget event: 5, 6, 7, 13 or 14
 */
// Profile screen: handles menu events (5 = name picked, 6 = back, 7 = slot chosen, 13 = name entered).
extern "C" void cOVState_PROFILE_onWidgetEvent(cOVState_PROFILE* vself, cUIThing* item, int event)
{
    cOVState_PROFILE* self = vself;
    switch (event) {
    case 13: {
        char buf[0x70];
        char fmt[0x70];
        char* name = self->mpKeyboard->mszEditString;
        func_002C2540((char*)cSSXApp_getMCManager(SSXApp) + 0x276, name);
        char* player = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
        if (strlen(name) != 0) {
            cBENewPlayerInterface_setPlayerName(player, self->mPlayerIdx, name);
        } else {
            strcpy2(fmt, GetText_K211BC8(str_kT_MEMPlayerName_c));
            sprintf(buf, fmt, self->mPlayerIdx + 1);
            cBENewPlayerInterface_setPlayerName(player, self->mPlayerIdx, buf);
        }
        VCALLV_K211BC8(player, 0xC, 1));
        cMCOverlayManager_writeSetFileType(cSSXApp_getMCManager(SSXApp), 1, self->mPlayerIdx);
        self->mIsProcessing = 1;
        self->mIsSavePressed = 1;
        self->mState = 3;
        break;
    }
    case 5: {
        if (((char*)item) == 0) break;
        if (self->mIsProcessing != 0) break;
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        if (self->mScreenStatus != 3) break;
        ((sMgrK211BC8*)mp)->fF8 = self->mSlot = item->mMiscData;
        if (self->mState == 1) {
            if (((char*)self->mpKeyboard) == 0) {
                char* kb = (char*)cKeyboardPopup_cKeyboardPopup(cMemMan_alloc(1092, str_Keyboard_b, 0x100, 0), ((void*)self->mpEngine), vself, 0, self->mCtrlBits);
                (*(char**)&self->mpKeyboard) = kb;
                *(int*)(kb + 0x140) = 55;
                cKeyboardPopup_setMaxCharacters(((char*)self->mpKeyboard), 8);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 75, 1, 2);
                self->mpKeyboard->mbDisableForFirst = 1;
                cKeyboardPopup_setDisableShiftNumbers(((char*)self->mpKeyboard), 1);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 0xE, 0, 2);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 39, 0, 2);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 40, 0, 2);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 52, 0, 2);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 26, 0, 2);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 25, 0, 2);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 53, 0, 2);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 0x40, 0, 2);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 65, 0, 2);
                cKeyboardPopup_setIncludeKey(((char*)self->mpKeyboard), 66, 0, 2);
                cKeyboardPopup_setMinCharacters(((char*)self->mpKeyboard), 1);
            }
            cKeyboardPopup_setStatic(((char*)self->mpKeyboard), GetText_K211BC8(str_kT_KEYUserName_b));
            self->mpKeyboard->mbClearOnFirst = 1;
            cKeyboardPopup_setInputText(((char*)self->mpKeyboard), cBENewPlayerInterface_getPlayerName(cBE_getInterface_Fv(cBE_getBE(), 1), self->mPlayerIdx));
            cUIStateStack_pushPopup(*(char**)(*(char**)(((char*)self->mpScreen) + 0xD0) + 0x10) + 0x18, ((char*)self->mpKeyboard));
            self->mProcessingKeyboard = 1;
            self->mPopupType = 0;
        }
        break;
    }
    case 6: {
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        if (self->mIsProcessing != 1) {
            int st = self->mState;
            if (st != 6) {
                self->mIsCancelPressed = 1;
                if (st == 0 || st == 2 || st == 1) {
                    VCALLV_K211BC8(mp, 1864, 1), 0);
                }
                self->mState = 6;
            }
        }
        break;
    }
    case 7:
        if (self->mIsProcessing != 1 && self->mState != 6) {
            char* mp = (char*)cSSXApp_getMCManager(SSXApp);
            if (VCALL_K211BC8(*(char**)(mp + 1076), 0, 54), *(int*)(mp + 0x428)) != 0) {
                ((sMgrK211BC8*)mp)->fF8 = self->mSlot = item->mMiscData;
                if (self->mState == 1) {
                    if (VCALL_K211BC8(*(char**)(mp + 1076), 0, 67), self->mSlot) != 0) {
                        self->mIsProcessing = 1;
                        self->mState = 4;
                        cMCOverlayManager_SetAction(mp, 0);
                        VCALLV_K211BC8(mp, 1864, 1), 0x18);
                    }
                }
            }
        }
        break;
    case 14:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_PROFILE_initGeneral);
#ifdef SKIP_ASM
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char D_004A2138[];
extern char str_playerpct_d_b[];

/**
 * Numbers the six profile slot rows of the profile overlay.
 *
 * Sets the texts "1".."6" to their own number and gives "player1".."player6" misc data 0..5; the lookups are
 * not null-checked.
 */
extern "C" void cOVState_PROFILE_initGeneral(cOVState_PROFILE* self)
{
    char buf[16];
    int i;
    for (i = 1; i < 7; i++) {
        sprintf(buf, D_004A2138, i);
        cUIText_setAsciiString((cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf)), buf);
        sprintf(buf, str_playerpct_d_b, i);
        *(int*)((char*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(buf)) + 0x18) = i - 1;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_cOVState_AUTOSAVE);
#ifdef SKIP_ASM
extern "C" void* cFEMemCard_cFEMemCard2(void* self, void* a1, int a2);
extern void* cOVState_AUTOSAVE_vtbl[];

/**
 * Constructs the autosave overlay.
 *
 * Runs cFEMemCard_cFEMemCard2 (the memory card screen base, type 0), installs cOVState_AUTOSAVE_vtbl, sets the state id
 * (+0xC) to 0x25 and +0x208 to 1, stores @p iParent at +0x22C and clears +0x1C4 and +0x230.
 *
 * @param pEngine  the UI engine
 * @param iParent  the overlay to return to (On Tour iParent)
 * @return @p self
 */
extern "C" void* cOVState_AUTOSAVE_cOVState_AUTOSAVE(cOVState_AUTOSAVE* self, cUIEngine* pEngine, int iParent)
{
    cFEMemCard_cFEMemCard2(self, pEngine, 0);
    self->mVtbl = cOVState_AUTOSAVE_vtbl;
    self->mHashName = 37;
    self->mAutoSaveOn = 1;
    self->mParentScreen = iParent;
    self->mSlot = 0;
    self->mLastProcessState = 0;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" void cOVState_onCreateScreen(void* self);
extern char str_122Autosave[];

/**
 * Creates the autosave overlay screen ("122Autosave") and runs the common overlay set-up.
 */
extern "C" void cOVState_AUTOSAVE_onCreateScreen(cOVState_AUTOSAVE* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_122Autosave), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_onUpdate);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_writeSetFileType(void* app, int a1, int a2);
extern "C" void cFEMemCard_onUpdate(void* self);

// On Tour: cMCOverlayManager::sdevice (bools are 4 bytes in SSX 3).
struct sdevice_2208 {
    int bValid;            // 0x00
    int bFormatted;        // 0x04
    int bCorrectEncoding;  // 0x08
    int bPluggedIn;        // 0x0C
    int iFreeSpace;        // 0x10
    int iTotalSpace;       // 0x14
    unsigned short szName[100];
};
struct cMCOverlayManager_2208 {
    char pad[0x344];
    sdevice_2208 mDevices[1];   // 0x344
    int mMaxDevices;            // 0x424
    int mSelectedDevice;        // 0x428
};

/**
 * Per-frame update of the autosave overlay: starts the save and watches the memory card while it runs.
 *
 * In state 1 (mState, +0x1C0) with nothing in progress (mIsProcessing, +0x1A8) it starts the save on the memory-card
 * overlay manager (cMCOverlayManager_writeSetFileType(pMC, 1, 0)), sets mIsProcessing, mIsSavePressed (+0x1DC) and mIsOverwriteOK (+0x1F0)
 * and moves to state 3. In state 3, if the selected card is no longer plugged in, valid and formatted, it clears
 * mIsSavePressed, sets mIsCancelPressed (+0x1E4), clears mIsProcessing and moves to state 6. It always finishes with
 * the memory-card base update cFEMemCard_onUpdate.
 */
// cOVState_AUTOSAVE::onUpdate (On Tour name); cFEMemCard fields: 0x1A8 mIsProcessing, 0x1C0 mState,
// 0x1DC mIsSavePressed, 0x1E4 mIsCancelPressed, 0x1F0 mIsOverwriteOK.
extern "C" void cOVState_AUTOSAVE_onUpdate(cOVState_AUTOSAVE* self)
{
    cMCOverlayManager_2208* pMC = (cMCOverlayManager_2208*)cSSXApp_getMCManager(SSXApp);
    if (self->mState == 1 && !self->mIsProcessing) {
        cMCOverlayManager_writeSetFileType(pMC, 1, 0);
        self->mIsProcessing = 1;
        self->mIsSavePressed = 1;
        self->mIsOverwriteOK = 1;
        self->mState = 3;
    } else if (self->mState == 3) {
        if (!pMC->mDevices[pMC->mSelectedDevice].bPluggedIn || !pMC->mDevices[pMC->mSelectedDevice].bValid
            || !pMC->mDevices[pMC->mSelectedDevice].bFormatted) {
            self->mIsSavePressed = 0;
            self->mIsCancelPressed = 1;
            self->mIsProcessing = 0;
            self->mState = 6;
        }
    }
    cFEMemCard_onUpdate(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_onInputBegin__FPv);
#ifdef SKIP_ASM
/**
 * Leaves controller input to the autosave overlay's screens.
 *
 * The base UI update cUIState_onUpdate() hands the input on to the state's screens when this returns 0.
 *
 * @return always 0 (input not consumed)
 */
int cOVState_AUTOSAVE_onInputBegin(void* self)
{
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_displayStatus);
#ifdef SKIP_ASM
struct sVEntry002122E8 {
    short delta;
    short index;
    void (*fn)(void*, int);
};


int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cOVState_AUTOSAVE_displayOn(cOVState_AUTOSAVE* self);
extern char str_autosave_text[];

/**
 * Shows the autosave indicator while a memory card operation is running.
 *
 * Calls displayOn when the memory card state (+0x1C0) is neither 0 nor 6, else hides "autosave_text" (virtual
 * slot 9).
 */
extern "C" void cOVState_AUTOSAVE_displayStatus(cOVState_AUTOSAVE* self)
{
    int status = self->mState;
    if (status != 0 && status != 6) {
        cOVState_AUTOSAVE_displayOn(self);
    } else {
        cUIThing* autosaveText = (cUIThing*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_autosave_text));
        ((sVEntry002122E8*)autosaveText->mVtbl)[9].fn(vthis(autosaveText, ((sVEntry002122E8*)autosaveText->mVtbl)[9]), 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_displayOn);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int cMCOverlayManager_isManagerDone(void* mp);
extern "C" void* cUIAnimationBank_getAnimationByHashName(void* bank, int hash);
extern "C" void cUIThing_playAnimation(void* obj, void* anim, int a2, int a3, int a4);
extern "C" void cUIThing_deleteAnimation(void* obj);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* text, int id);
extern char str_exclam_b[];
extern char str_at_pulsingicon[];
extern char str_kT_MEMAutosaving[];

class cUIObj_2360 {
public:
    int pad[2];
    virtual ~cUIObj_2360();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setFocus(int v);
    virtual void setVisible(int v);
};

/**
 * Updates the autosave icon and text when the processing flag changes.
 *
 * Compares the processing flag (+0x1A8) with its last shown value (+0x230). While processing, with
 * cMCOverlayManager_isManagerDone(memory card manager) 0 and state not 6, it pulses "exclam" ("@pulsingicon") and shows
 * kT_MEMAutosaving; otherwise it stops the animation and hides the text.
 */
extern "C" void cOVState_AUTOSAVE_displayOn(cOVState_AUTOSAVE* self)
{
    void* mp = cSSXApp_getMCManager(SSXApp);
    int processState = self->mIsProcessing;
    if (processState == self->mLastProcessState) {
        return;
    }
    self->mLastProcessState = processState;
    void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_exclam_b));
    if (obj == 0) {
        return;
    }
    if (self->mIsProcessing != 0 && cMCOverlayManager_isManagerDone(mp) == 0 && self->mState != 6) {
        void* bank = &self->mpEngine->mAnimBank;
        void* anim = cUIAnimationBank_getAnimationByHashName(bank, GetHashValue32(str_at_pulsingicon));
        if (anim != 0) {
            cUIThing_playAnimation(obj, anim, 9, 0, 0);
        }
        cUIObj_2360* autosaveText = (cUIObj_2360*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_autosave_text));
        autosaveText->setVisible(1);
        cUIText_setUnicodeStringByID((cUIText*)autosaveText, GetHashValue32(str_kT_MEMAutosaving));
    } else {
        cUIThing_deleteAnimation(obj);
        ((cUIObj_2360*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_autosave_text)))->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_updateDone);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_popToStateInclusive(void* self);

/**
 * Leaves the autosave overlay once its memory-card work is done: pops the UI state stack the first time.
 *
 * When +0x1AC (On Tour mIsDone) is clear, calls cUIStateStack_popToStateInclusive (On Tour cUIStateStack::popToStateInclusive) on
 * mpEngine's state stack (+0x18); then sets +0x1AC. It is the updateDone slot (+0x10C) of cOVState_AUTOSAVE_vtbl.
 */
extern "C" void cOVState_AUTOSAVE_updateDone(cOVState_AUTOSAVE* self)
{
    if (self->mIsDone == 0) {
        cUIStateStack_popToStateInclusive(&self->mpEngine->mStateStack);
    }
    self->mIsDone = 1;
}
#endif

//100%
INCLUDE_ASM("fe/ovstateprofile", cOVState_AUTOSAVE_onDestroyScreen);
#ifdef SKIP_ASM
extern int cBENewRaceInterface_bIsBamLocation;
extern char str_cFEStateTitle_b[];
extern char str_cFEStateLodge_b[];
extern char str_cPreGameLoadScreen_b[];
struct cAppMan;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
void* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cGame_exit(void* game, int a);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void* cFEStateTitle_cFEStateTitle1(void* self, void* mgr);
extern "C" void* cFEStateLodge_cFEStateLodge(void* self, void* mgr);
void* cPreGameLoadScreen_cPreGameLoadScreen(void* self);
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" int cBENewRaceInterface_setGameMode(void* iface, int mode);
extern "C" int cBENewRaceInterface_setGameEvent(void* iface, int ev);
extern "C" int cBENewPlayerInterface_getLastHub(void* self, int index);
extern "C" void cUIStateStack_push(void* list, void* item);


/**
 * Tears down the autosave overlay and moves on to where the save was started from.
 *
 * After cFEMemCard_onDestroyScreen (the shared memory card teardown) it switches on +0x22C (the constructor's parent
 * code): 1 exits the game, 6 pushes cFEStateTitle, 7 pushes cFEStateLodge, 0 sets up a new race (game mode 4,
 * event 0xC, the last hub into newRace.iLocationIdx, cBENewRaceInterface_bIsBamLocation = 1) and makes cPreGameLoadScreen the next module.
 */
extern "C" void cOVState_AUTOSAVE_onDestroyScreen(cOVState_AUTOSAVE* self)
{
    cFEMemCard_onDestroyScreen(self);
    int parentScreen = self->mParentScreen;
    if (parentScreen == 1) {
        cGame_exit(SSXApp->Game, 0);
    } else if (parentScreen == 6) {
        void* nextState = cFEStateTitle_cFEStateTitle1(cMemMan_alloc(76, str_cFEStateTitle_b, 0, 0), self->mpEngine);
        cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
    } else if (parentScreen == 7) {
        void* nextState = cFEStateLodge_cFEStateLodge(cMemMan_alloc(88, str_cFEStateLodge_b, 0, 0), self->mpEngine);
        cUIStateStack_push(&self->mpEngine->mStateStack, nextState);
    } else if (parentScreen == 0) {
        void* race = cBE_getInterface_Fv(cBE_getBE(), 0);
        void* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        cBENewRaceInterface_setGameMode(race, 4);
        cBENewRaceInterface_setGameEvent(race, 0xC);
        cBENewRaceInterface_newRace.iLocationIdx = cBENewPlayerInterface_getLastHub(player, 0);
        cBENewRaceInterface_bIsBamLocation = 1;
        sVEntry<void(void*)>* vt = *(sVEntry<void(void*)>**)((char*)race + 0xC);
        vt[1].fn(vthis(race, vt[1]));
        cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen_b, 0x100, 0)));
    }
}
#endif


//100%
INCLUDE_ASM("fe/ovstateprofile", func_00212680);
#ifdef SKIP_ASM
/**
 * Returns its first argument; the variadic definition only spills the other argument registers.
 *
 * Nothing in the program calls it.
 *
 * @return @p self
 */
extern "C" void* func_00212680(void* self, ...)
{
    return self;
}
#endif
//100%
INCLUDE_ASM("fe/ovstateprofile", func_002126C0);
#ifdef SKIP_ASM
/**
 * Tells whether the character range [@p first1, @p last1) sorts before [@p first2, @p last2), comparing signed chars.
 *
 * The same algorithm as the SGI STL's lexicographical_compare: the first differing character decides; when one range is
 * a prefix of the other, the shorter one is less.
 *
 * @param first1  start of the first range
 * @param last1  end of the first range
 * @param first2  start of the second range
 * @param last2  end of the second range
 * @return 1 when the first range is less, else 0
 */
extern "C" int func_002126C0(const char* first1, const char* last1, const char* first2, const char* last2)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) return 1;
        if (*first2 < *first1) return 0;
    }
    return first1 == last1 && first2 != last2;
}
#endif
//100%
INCLUDE_ASM("fe/ovstateprofile", func_00212728);
#ifdef SKIP_ASM
/**
 * Three-way compares the character ranges [@p first1, @p last1) and [@p first2, @p last2), comparing signed chars.
 *
 * The same algorithm as the SGI STL's __lexicographical_compare_3way: the first differing character decides; a range
 * that is a prefix of the other is the smaller one.
 *
 * @param first1  start of the first range
 * @param last1  end of the first range
 * @param first2  start of the second range
 * @param last2  end of the second range
 * @return -1, 0 or 1 as the first range is less than, equal to or greater than the second
 */
extern "C" int func_00212728(const char* first1, const char* last1, const char* first2, const char* last2)
{
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2) return -1;
        if (*first2 < *first1) return 1;
    }
    if (first2 == last2) return first1 != last1;
    return -1;
}
#endif
//100%
INCLUDE_ASM("fe/ovstateprofile", func_00212788);
#ifdef SKIP_ASM
void operator_delete(int*);

struct sListNode_00212788 {
    sListNode_00212788* next;
    sListNode_00212788* prev;
};
struct sList_00212788 {
    int pad0;
    sListNode_00212788* node;
};

/**
 * Empties a doubly linked list: deletes every node and leaves the head node (+0x4) linked to itself.
 *
 * Walks next pointers from the head, freeing each node with operator delete, then sets the head's next and prev back to
 * the head: the shape of the SGI STL list's clear().
 */
extern "C" void func_00212788(sList_00212788* self)
{
    sListNode_00212788* cur = self->node->next;
    while (cur != self->node) {
        sListNode_00212788* deadNode = cur;
        cur = cur->next;
        operator_delete((int*)deadNode);
    }
    self->node->next = self->node;
    self->node->prev = self->node;
}
#endif
