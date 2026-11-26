#include "vtable.h"
#include "common.h"
#include "ui/uiengine.h"
#include "be/beinterface.h"
#include "fe/keyboardpopup.h"
#include "movie/mcoverlaymanager.h"
#include "fe/festateprofile.h"
#include "ui/uithing.h"
#include "ui/uitext.h"
#include "ui/uiscreen.h"
#include "fe/festateriderbio.h"
#include "be/beintnewrace.h"
#include "main/ssxapp.h"
#include "util/bxmemcard.h"
#include "util/locale.h"
#include "ui/uimenu.h"

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileSelect_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_05sel_prof[];
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);

/**
 * Creates the profile select screen ("05sel_prof") and shows its first frame.
 */
void cFEStateProfileSelect::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_05sel_prof), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileSelect_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_push(void* self, void* state);

// A tARGB with EA's (A, R, G, B) constructor (ours has none).
struct tARGBInit : tARGB {
    tARGBInit(float a, float r, float g, float b) { A = a; R = r; G = g; B = b; }
};

// cUIThing_vtbl slot 11 (setColour). On Tour's takes the colour by reference (const tARGB&) and the call passes a
// temporary; the header's pointer form makes the caller build it first, so the call is by hand.
typedef sVEntry<void(void*, const tARGB&)> tSetColourSlot;

/**
 * Handles select, back and highlight events on the profile select screen.
 *
 * kONNEXT recolours the entry and pushes the next state for its misc data (custom interface slot 4); kONPREV
 * pushes the previous state (slot 5); kONGAINFOCUS and kONLOSEFOCUS set the highlight and normal colours.
 *
 * @param menu  the widget the event is for (null: nothing happens)
 * @param msg  UI event code (1, 2, 5 or 6)
 */
void cFEStateProfileSelect::onWidgetEvent(cUIThing* menu, unsigned int msg)
{
    if (((char*)menu) == 0)
        return;
    switch (msg) {
    case cUIState::kONNEXT: {
        tSetColourSlot* vt = (tSetColourSlot*)VPTR_OF(cListNode, menu);
        vt[11].fn(vthis(menu, vt[11]), tARGBInit(1.0f, 1.0f, 0.0f, 0.0f));
        cUICustomInterface* custom = mpEngine->mpCustom;
        void* nextState = custom->getNextState((cUIState*)this, menu->mMiscData);
        if (nextState != 0)
            cUIStateStack_push(&mpEngine->mStateStack, nextState);
        break;
    }
    case cUIState::kONPREV: {
        cUICustomInterface* custom = mpEngine->mpCustom;
        void* nextState = custom->getPrevState((cUIState*)this, menu->mMiscData);
        if (nextState != 0)
            cUIStateStack_push(&mpEngine->mStateStack, nextState);
        break;
    }
    case cUIState::kONGAINFOCUS: {
        tSetColourSlot* vt = (tSetColourSlot*)VPTR_OF(cListNode, menu);
        vt[11].fn(vthis(menu, vt[11]), tARGBInit(1.0f, 0.0f, 1.0f, 0.0f));
        break;
    }
    case cUIState::kONLOSEFOCUS: {
        tSetColourSlot* vt = (tSetColourSlot*)VPTR_OF(cListNode, menu);
        vt[11].fn(vthis(menu, vt[11]), tARGBInit(1.0f, 1.0f, 1.0f, 1.0f));
        break;
    }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_cFEStateProfileLoad);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewPlayerInterface_getPlayerCtrlID(void* self, int a1);
extern "C" void* cFEMemCard_cFEMemCard2(void* self, int a1, int a2);
extern void* cFEStateProfileLoad_vtbl[];
extern int cFEStateProfileLoad_mScreenAction;

/**
 * Constructs the profile load screen for the controller that asked for it.
 *
 * Runs cFEMemCard_cFEMemCard2 (the memory card screen base, with 0), sets the state id (+0xC) to 0x23 and the vtable,
 * stores @p iType in cFEStateProfileLoad_mScreenAction, clears the work fields, turns the lowest set bit of
 * @p flags (of the first three) into a controller index (+0x22C) and finds which player (one, or two in a
 * two-player race) uses that controller (cBENewPlayerInterface_getPlayerCtrlID), storing it at +0x1CC and +0x44.
 *
 * @param pEngine     the UI engine
 * @param iType     what the screen is for (On Tour iType, kept in mScreenAction)
 * @param flags  bit mask of the controller that is active
 * @return @p self
 */
extern "C" void* cFEStateProfileLoad_cFEStateProfileLoad(cFEStateProfileLoad* self, int pEngine, int iType, unsigned char flags)
{
    cFEMemCard_cFEMemCard2(self, pEngine, 0);
    self->mHashName = 35;
    VPTR_OF(cListNode, self) = cFEStateProfileLoad_vtbl;
    cFEStateProfileLoad_mScreenAction = iType;
    self->mPlayerIdx = 0;
    self->mIsOn = 0;
    self->mIsHidden = 0;
    self->mTypeOfQuit = 0;
    self->mCtrlBits = flags;
    self->miCntrlID = 0;
    for (int i = 0; i < 3; i++) {
        if (flags & 1) {
            break;
        }
        flags >>= 1;
        self->miCntrlID += 1;
    }
    void* pi = cBE_getInterface_Fv(cBE_getBE(), 1);
    cBE_getInterface_Fv(cBE_getBE(), 0);
    int n = 1;
    if (cBENewRaceInterface_newRace.cGamePath == kGAMEPATH_MULTIPLAYER) {
        n = 2;
    }
    for (int j = 0; j < n; j++) {
        if (cBENewPlayerInterface_getPlayerCtrlID(pi, j) == self->miCntrlID) {
            self->mPlayerIdx = j;
            break;
        }
    }
    int idx = *(signed char*)&self->mPlayerIdx;
    self->mUserNum = idx;
    return ((void*)self);
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_93profile_load[];
extern int cFEStateProfileLoad_mScreenAction;
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void* cUIScreen_playFrame(void* self, unsigned short frame, int flag);
extern "C" void cFEStateProfileLoad_initGeneral(cFEStateProfileLoad* self);
extern "C" void cFEStateProfileLoad_initCreateScreen(cFEStateProfileLoad* self);
extern "C" void cFEStateProfileLoad_initLoadScreen(cFEStateProfileLoad* self);
extern "C" void cFEStateProfileLoad_initSaveScreen(cFEStateProfileLoad* self);

/**
 * Creates the profile load/save screen ("93profile_load") and sets it up for the current mode.
 *
 * After initGeneral, mode cFEStateProfileLoad_mScreenAction picks initCreateScreen (kScreenActionCreate),
 * cFEStateProfileLoad_initLoadScreen (kScreenActionLoad) or cFEStateProfileLoad_initSaveScreen (kScreenActionSave),
 * probably On Tour's initLoadScreen and initSaveScreen.
 */
void cFEStateProfileLoad::onCreateScreen()
{
    void* engine = mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, this, GetHashValue32(str_93profile_load), 0);
    mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    initGeneral();
    switch (cFEStateProfileLoad_mScreenAction) {
    case cFEStateProfileLoad::kScreenActionCreate:
        initCreateScreen();
        break;
    case cFEStateProfileLoad::kScreenActionLoad:
        initLoadScreen();
        break;
    case cFEStateProfileLoad::kScreenActionSave:
        initSaveScreen();
        break;
    }
}
#endif

extern "C" void* cUIState_onRender2D(void* self);

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_onRender2D__FPv);
#ifdef SKIP_ASM
/**
 * Draws the profile load screen by running cUIState_onRender2D.
 *
 * Fills slot +0x7C (onRender2D) of cFEStateProfileLoad_vtbl.
 *
 * @return cUIState_onRender2D's result
 */
void* cFEStateProfileLoad_onRender2D(void* self)
{
    return cUIState_onRender2D(self);
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_onWidgetMenu);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern int cFEStateProfileLoad_mScreenAction;

/**
 * Answers the profile load screen's menu events: refuses events the current screen cannot take.
 *
 * kONNEXT gives 0x100 when the card manager's mSelectedDevice fails the memory card's slot-54 check (mpMemCard,
 * cPS2MemCard_IsActionable); kONOPTION gives 0x100 on the load screen (cFEStateProfileLoad_mScreenAction
 * kScreenActionLoad) or while the delete button is inactive (+0x220 clear); kONMISC always gives 0x100; everything else
 * gives 0x101. Fills slot +0xA4 (onWidgetMenu) of cFEStateProfileLoad_vtbl.
 *
 * @param widget  the menu that sent the event (unused)
 * @param msg  the menu event
 * @return 0x100 to refuse the event, 0x101 to allow it
 */
extern "C" int cFEStateProfileLoad_onWidgetMenu(cFEStateProfileLoad* self, void* widget, unsigned int msg)
{
    cMCOverlayManager* app = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    switch (msg) {
    case cUIMenu::kONNEXT:
        if (app->mpMemCard->IsActionable(app->mSelectedDevice) == 0) {
            return 0x100;
        }
        break;
    case cUIMenu::kONOPTION:
        if (cFEStateProfileLoad_mScreenAction == cFEStateProfileLoad::kScreenActionLoad
            || self->mDeleteButtonActive == 0) {
            return 0x100;
        }
        break;
    case cUIMenu::kONMISC:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_hideActionButtons);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_ps2x[];
extern int cFEStateProfileLoad_mScreenAction;
extern char str_btext3[];
extern char str_btext3a[];
extern char str_btext5[];
extern char str_ps2sq[];
struct cUIObj_EF70 {
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
    virtual void setVisible(int on);
};

/**
 * Shows or hides the profile load screen's action buttons.
 *
 * Does nothing when +0x234 already equals @p on. Otherwise sets "ps2x" visible when @p on is 0 and hidden when 1, and
 * likewise "btext3", "btext5" and "ps2sq" unless this is the load screen (cFEStateProfileLoad_mScreenAction
 * kScreenActionLoad), where it is "btext3a"; then stores @p on in +0x234. Fills slot +0x12C of cFEStateProfileLoad_vtbl
 * (cFEMemCard_hideActionButtons's slot).
 *
 * @param on  1 to hide the buttons, 0 to show them
 */
void cFEStateProfileLoad::hideActionButtons(int on)
{
    if (mIsHidden == on)
        return;
    int off = on ^ 1;
    ((cUIObj_EF70*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ps2x)))->setVisible(off);
    if (cFEStateProfileLoad_mScreenAction != cFEStateProfileLoad::kScreenActionLoad) {
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_btext3)))->setVisible(off);
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_btext5)))->setVisible(off);
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ps2sq)))->setVisible(off);
    } else {
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_btext3a)))->setVisible(off);
    }
    mIsHidden = on;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_hideDeleteButton);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_btext5[];
extern char str_ps2sq[];
extern int cFEStateProfileLoad_mScreenAction;
struct cUIObj_F0B8 {
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
    virtual void setVisible(int on);
};

/**
 * Shows or hides the profile load screen's delete button.
 *
 * Except on the load screen (cFEStateProfileLoad_mScreenAction kScreenActionLoad) sets "btext5" and "ps2sq" visible
 * when @p on is false (cUIThing_setVisible); always stores !@p on in +0x220, which cFEStateProfileLoad_onWidgetMenu
 * tests for kONOPTION. Fills slot +0x134 of cFEStateProfileLoad_vtbl (cFEMemCard_hideDeleteButton's slot).
 *
 * @param on  true to hide the button
 */
extern "C" void cFEStateProfileLoad_hideDeleteButton(cFEStateProfileLoad* self, bool on)
{
    if (cFEStateProfileLoad_mScreenAction != cFEStateProfileLoad::kScreenActionLoad) {
        int off = !on;
        ((cUIObj_F0B8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext5)))->setVisible(off);
        ((cUIObj_F0B8*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq)))->setVisible(off);
    }
    self->mDeleteButtonActive = !on;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_hideMainButton);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_btext3[];
extern char str_btext3a[];
extern char str_ps2x[];
extern int cFEStateProfileLoad_mScreenAction;

/**
 * Shows or hides the profile load screen's main (accept) button.
 *
 * Except on the load screen (cFEStateProfileLoad_mScreenAction kScreenActionLoad) shows "btext3" and "ps2x" only when
 * @p saveBlocked is false; on the load screen shows "btext3a" and "ps2x" only when @p loadBlocked is false. The shown
 * state is also stored in mMainButtonActive, which cFEStateProfileLoad_onWidgetEvent and cFEStateProfileLoad_startSave
 * require. Fills slot +0x13C of cFEStateProfileLoad_vtbl (cFEMemCard_hideMainButton's slot).
 *
 * @param loadBlocked  true to hide the button on the load screen
 * @param saveBlocked  true to hide the button on the other screens
 */
extern "C" void cFEStateProfileLoad_hideMainButton(cFEStateProfileLoad* self, bool loadBlocked, bool saveBlocked)
{
    cUIThing* xButton = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x));
    if (cFEStateProfileLoad_mScreenAction != cFEStateProfileLoad::kScreenActionLoad) {
        int showButton = !saveBlocked;
        cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3))->setVisible(showButton);
        self->mMainButtonActive = showButton;
        xButton->setVisible(showButton);
    } else {
        int showButton = !loadBlocked;
        cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3a))->setVisible(showButton);
        self->mMainButtonActive = showButton;
        xButton->setVisible(showButton);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_displayStatus);
#ifdef SKIP_ASM
extern "C" void cFEStateProfileLoad_displayOn(cFEStateProfileLoad* self);
extern "C" void cFEStateProfileLoad_displayOff(cFEStateProfileLoad* self);

/**
 * Redraws the profile load screen's card panel for the current display state.
 *
 * Runs cFEStateProfileLoad_displayOn when the display state (+0x214) is 3, otherwise cFEStateProfileLoad_displayOff. Fills slot +0xD4 of
 * cFEStateProfileLoad_vtbl (cFEMemCard_displayStatus's slot).
 */
void cFEStateProfileLoad::displayStatus()
{
    if (mScreenStatus == cFEMemCard::kDISPLAY_ON) {
        displayOn();
    } else {
        displayOff();
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_displayOn);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cFEStateProfileLoad_displayOff(cFEStateProfileLoad* self);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cFEMemCard_callbackDisplayNoCards();
extern "C" int cFEMemCard_checkNextAvailableDevice(void* self, void* mp, int dev, int a3);
extern "C" void cMCOverlayManager_GetDeviceDisplayString(void* mp, int dev, unsigned short* buf, int len);
extern "C" void cUIText_setUnicodeStringWrap(void* obj, unsigned short* s);
extern "C" void cUIText_setUnicodeString(void* obj, unsigned short* s);
extern int cFEStateProfileLoad_mScreenAction;
extern char str_1percentage[];
extern char str_Memory_card_text[];
extern char str_playerpct_d[];
extern char str_arrw_left[];
extern char str_arrw_right[];
extern char str_datepct_d[];
extern char str_timepct_d[];

extern tARGB gStaticColour;

static inline cUIText* getText_F2B8(cFEStateProfileLoad* self, char* name)
{
    return (cUIText*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
}

/**
 * Refreshes the memory card panel and the six profile slot rows.
 *
 * Does nothing unless this state's virtual slot 42 returns true (probably: a card is ready). If the memory
 * card manager's profile object (+0x434) then fails its slot 53, calls cFEMemCard_callbackDisplayNoCards and cFEStateProfileLoad_displayOff and
 * returns; otherwise it moves to state 3 (slot 35) if needed, shows the device name, fills
 * "player1".."player6" with the slot names (plus the "date%d"/"time%d" lines for used slots) and shows the
 * left/right device arrows when another device is available.
 *
 */
// Memory-card profile slots: refreshes the device name and the six slot rows (name, the two detail lines).
void cFEStateProfileLoad::displayOn()
{
    unsigned short buf[800];
    char name[0x40];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    if (displayFileOK()) {
        if (!mp->mpMemCard->IsAvailable1()) {
            cFEMemCard_callbackDisplayNoCards();
            displayOff();
            return;
        } else if (mScreenStatus != cFEMemCard::kDISPLAY_ON) {
            triggerDisplayState(3);
        }
    }
    if (displayFileOK()) {
        int sel = 0;
        cMCOverlayManager_GetDeviceDisplayString(mp, mp->mSelectedDevice, buf, 800);
        cUIText_setUnicodeStringWrap(getText_F2B8(this, str_Memory_card_text), buf);
        mLastSelectedDevice = mp->mSelectedDevice;
        setMenuHighlight();
        cUIText* widget = getText_F2B8(this, str_1percentage);
        widget->setVisible(0);
        for (int i = 1; i < 7; i++) {
            sprintf(name, str_playerpct_d, i);
            widget = getText_F2B8(this, name);
            widget->mStaticColour = gStaticColour;
            if (!mp->mpMemCard->HasFileName(i - 1) && !mp->mpMemCard->IsFileSlotDisabled(i - 1)
                && mp->mpMemCard->IsFormatted()) {
                mp->mpMemCard->GetTitleString(i - 1, mTitle);
                cUIText_setUnicodeString(widget, mTitle);
                if (cFEStateProfileLoad_mScreenAction != cFEStateProfileLoad::kScreenActionLoad && sel == 0) {
                    sel = 1;
                    widget->setStatic(0);
                    widget->setUseStaticColour(0);
                } else {
                    widget->setUseStaticColour(1);
                    widget->setStatic(1);
                }
                sprintf(name, str_datepct_d, i);
                widget = getText_F2B8(this, name);
                widget->setVisible(0);
                sprintf(name, str_timepct_d, i);
                widget = getText_F2B8(this, name);
                widget->setVisible(0);
            } else if (mp->mpMemCard->IsActionable(mp->mSelectedDevice)) {
                mp->mpMemCard->GetTitleString(i - 1, mTitle);
                cUIText_setUnicodeString(widget, mTitle);
                if (mp->mpMemCard->IsFileSlotDisabled(i - 1)) {
                    widget->setStatic(1);
                    widget->setUseStaticColour(1);
                } else {
                    widget->setStatic(0);
                    widget->setUseStaticColour(0);
                }
                sprintf(name, str_datepct_d, i);
                widget = getText_F2B8(this, name);
                mp->mpMemCard->GetFileDate(i - 1, buf);
                cUIText_setUnicodeString(widget, buf);
                widget->setVisible(1);
                sprintf(name, str_timepct_d, i);
                widget = getText_F2B8(this, name);
                mp->mpMemCard->GetFileTime(i - 1, buf);
                cUIText_setUnicodeString(widget, buf);
                widget->setVisible(1);
            } else {
                mp->mpMemCard->GetTitleString(i - 1, mTitle);
                cUIText_setUnicodeString(widget, mTitle);
                widget->setUseStaticColour(1);
                widget->setStatic(1);
                sprintf(name, str_datepct_d, i);
                widget = getText_F2B8(this, name);
                widget->setVisible(0);
                sprintf(name, str_timepct_d, i);
                widget = getText_F2B8(this, name);
                widget->setVisible(0);
            }
        }
        if (cFEMemCard_checkNextAvailableDevice(this, mp, mp->mSelectedDevice, 1) == cFEMemCard::kNoNextDevice)
            getText_F2B8(this, str_arrw_left)->setVisible(0);
        else
            getText_F2B8(this, str_arrw_left)->setVisible(1);
        if (cFEMemCard_checkNextAvailableDevice(this, mp, mp->mSelectedDevice, 0) == cFEMemCard::kNoNextDevice)
            getText_F2B8(this, str_arrw_right)->setVisible(0);
        else
            getText_F2B8(this, str_arrw_right)->setVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_displayOff);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cFEStateProfileLoad_displayOn(cFEStateProfileLoad* self);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_playerpct_d[];

/**
 * Resets the profile rows when no card is ready, or hands over to cFEStateProfileLoad_displayOn when one is.
 *
 * When this state's vtable slot 42 (cFEMemCard_displayFileOK) passes and the memory card (the card manager's mpMemCard) passes its
 * slot-53 check (cPS2MemCard_IsAvailable1), it switches the display state to 3 (slot 35, cFEMemCard_triggerDisplayState) and runs
 * cFEStateProfileLoad_displayOn. Otherwise it resets the six rows "player1" to "player6": slot 25
 * (cUIText_setUseStaticColour for a text) with 0 and setStatic(1). Called by cFEStateProfileLoad_displayStatus and by
 * cFEStateProfileLoad_displayOn when no card is found.
 */
void cFEStateProfileLoad::displayOff()
{
    cMCOverlayManager* app = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    if (displayFileOK() != 0) {
        if (app->mpMemCard->IsAvailable1() != 0) {
            triggerDisplayState(3);
            displayOn();
            return;
        }
    }
    for (int i = 1; i < 7; i++) {
        char buf[64];
        sprintf(buf, str_playerpct_d, i);
        cUIText* playerButton = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(buf));
        playerButton->setUseStaticColour(0);
        playerButton->setStatic(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_updateDone);
#ifdef SKIP_ASM
struct cAppMan;
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int cBENewRaceInterface_setGameMode(void* iface, int mode);
extern "C" int cBENewRaceInterface_setGameEvent(void* iface, int ev);
extern "C" int cBENewPlayerInterface_getLastHub(void* iface, int a1);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
void* cPreGameLoadScreen_cPreGameLoadScreen(void* self);
void cAppMan_setNextModule(cAppMan* self, unsigned int module);
extern "C" void* cFEStateTitle_cFEStateTitle1(void* self, void* engine);
extern "C" void cUIStateStack_push(void* self, void* state);
extern "C" void cUIStateStack_pushCheck(void* self, void* state);
extern int cBENewRaceInterface_bIsBamLocation;
extern int gInLodge;
extern char str_cPreGameLoadScreen[];
extern char str_cFEStateTitle[];


/**
 * Leaves the profile load screen once its card work is done, going where +0x27C says.
 *
 * Runs once: unless +0x1AC is already set it switches on +0x27C (On Tour mTypeOfQuit). kQuitToPrevious asks the engine
 * owner (the first word of the engine at +0x10) for the next state through its vtable slot 5 (cFECustom_getPrevState in
 * cFECustom_vtbl) with this state and the mode (+0x1BC), and pushes it with cUIStateStack_pushCheck. kQuitToHub sets up
 * a race on the new-race interface (game mode 4, event 0xC), sets cBENewRaceInterface_bIsBamLocation to 1 and the
 * location (cBENewRaceInterface_newRace.iLocationIdx) to the player's last hub (cBENewPlayerInterface_getLastHub),
 * submits it (the interface's vtable slot 1, cBENewRaceInterface_submit) and makes a new cPreGameLoadScreen the next
 * app module. kQuitToTitle clears gInLodge and pushes a new title state (cFEStateTitle_cFEStateTitle1,
 * "cFEStateTitle"). Sets +0x1AC afterwards. Fills slot +0x10C of cFEStateProfileLoad_vtbl (cFEMemCard_updateDone's
 * slot).
 */
void cFEStateProfileLoad::updateDone()
{
    if (mIsDone == 0) {
        int sel = mTypeOfQuit;
        if (sel == cFEStateProfileLoad::kQuitToPrevious) {
            cUICustomInterface* frontEnd = mpEngine->mpCustom;
            void* nextState = frontEnd->getPrevState((cUIState*)this, mType);
            if (nextState != 0)
                cUIStateStack_pushCheck(&mpEngine->mStateStack, nextState);
        } else if (sel == cFEStateProfileLoad::kQuitToHub) {
            cBEInterface* race = cBE_getInterface_Fv(cBE_getBE(), 0);
            void* pl = cBE_getInterface_Fv(cBE_getBE(), 1);
            cBENewRaceInterface_setGameMode(race, 4);
            cBENewRaceInterface_setGameEvent(race, 0xC);
            int x = cBENewPlayerInterface_getLastHub(pl, 0);
            cBENewRaceInterface_bIsBamLocation = 1;
            cBENewRaceInterface_newRace.iLocationIdx = x;
            race->submit();
            cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
        } else if (sel == cFEStateProfileLoad::kQuitToTitle) {
            gInLodge = 0;
            void* st = cFEStateTitle_cFEStateTitle1(cMemMan_alloc(76, str_cFEStateTitle, 0, 0), mpEngine);
            cUIStateStack_push(&mpEngine->mStateStack, st);
        }
    }
    mIsDone = 1;
}
#endif

extern "C" void* cFEMemCard_onDestroyScreen(void*);

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_onDestroyScreen__FPv);
#ifdef SKIP_ASM
/**
 * Destroys the profile-load screen: runs the shared profile screen teardown, then clears the word at +0x1A4.
 *
 * The teardown, cFEMemCard_onDestroyScreen(), calls helpers of back-end interface 5, runs three memory card manager calls
 * when its word at +0x434 is set, and frees the buffer at +0x1D4 when one is held.
 */
void cFEStateProfileLoad::onDestroyScreen()
{
    cFEMemCard_onDestroyScreen(this);
    mpKeyboard = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_startSave);
#ifdef SKIP_ASM
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void cMCOverlayManager_writeSetFileType(void* app, int a1, int a2);
extern "C" void cMCOverlayManager_GetMessage(void*, unsigned short*, int);
void cMemMan_free(void* p);
// PORT: operator_new__FUi really takes (size, tag, flags, d); bound by asm label
void* operator_new_tag(unsigned int size, const char* tag, int flags, int d) __asm__("operator_new__FUi");
extern char str_Temp_String[];

/**
 * Starts saving the profile to the selected card slot when the save button is active.
 *
 * Only with the main button active (mMainButtonActive): starts a save of file type 1 for the player (mPlayerIdx)
 * with cMCOverlayManager_writeSetFileType, sets mState to 3 (saving), mIsProcessing and mIsSavePressed. Unless the
 * status popup is already up (mIsStatusPopupOn) or mAutoLoadOn is set, it sets mPopupType to 22, fetches the card
 * manager's message (cMCOverlayManager_GetMessage, 800 characters into a 1600-byte "Temp String" buffer), shows it in
 * the status popup (getPopup, with 1, 1, 0, 0, 1), sets mIsStatusPopupOn and frees the buffer. Called by
 * cFEStateProfileLoad_onWidgetEvent once a profile name has been entered.
 */
void cFEStateProfileLoad::startSave()
{
    if (mMainButtonActive != 0) {
        cMCOverlayManager_writeSetFileType(cSSXApp_getMCManager(SSXApp), 1, mPlayerIdx);
        mState = 3;
        mIsProcessing = 1;
        mIsSavePressed = 1;
        if (mIsStatusPopupOn == 0 && mAutoLoadOn == 0) {
            mPopupType = 22;
            unsigned short* buf = (unsigned short*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
            cMCOverlayManager_GetMessage(cSSXApp_getMCManager(SSXApp), buf, 800);
            getPopup(buf, 1, 1, 0, 0, 1);
            mIsStatusPopupOn = 1;
            if (buf != 0) {
                cMemMan_free(buf);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
int GetHashValue32(char* str);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strlen(const char* s);
extern "C" void cBENewPlayerInterface_setPlayerName(void* player, int idx, char* name);
extern "C" void* cBENewPlayerInterface_getPlayerName(void* player, int idx);
extern "C" void cFEStateProfileLoad_startSave(cFEStateProfileLoad* p);
extern "C" void* cKeyboardPopup_cKeyboardPopup(void* mem, void* engine, void* owner, int a3, int a4);
extern "C" void cKeyboardPopup_setInputText(void* kb, void* name);
extern "C" void cKeyboardPopup_setMaxCharacters(void* kb, int a1);
extern "C" void cKeyboardPopup_setMinCharacters(void* kb, int a1);
extern "C" void cKeyboardPopup_setIncludeKey(void* kb, int key, int a2, int a3);
extern "C" void cKeyboardPopup_setDisableShiftNumbers(void* kb, int a1);
extern "C" void cKeyboardPopup_setStatic(void* kb, void* text);
extern "C" void cMCOverlayManager_readSetFileType(void* mp, int a1);
extern "C" void cMCOverlayManager_SetAction(void* mp, int a1);
extern "C" void func_002C2540(void* dst, char* src);
extern "C" void strcpy2(char* dst, void* src);
extern "C" void cUIStateStack_pushPopup(void* list, void* item);
extern int cFEStateProfileLoad_mScreenAction;
extern char str_kT_MEMPlayerName_b[];
extern char str_Keyboard[];
extern char str_kT_KEYUserName[];

static inline void* GetText_K18FD90(char* key)
{
    cFELocale* db = SSXApp->FELocale;
    // slot 4: getStringByHash (the virtual call through cFELocale* adds an addiu for &vt[4])
    sVEntry<unsigned short*(void*, unsigned int)>* vt =
        (sVEntry<unsigned short*(void*, unsigned int)>*)VPTR_OF(cUILocaleInterface, db);
    return vt[4].fn(vthis(db, vt[4]), GetHashValue32(key));
}

/**
 * Handles the profile load screen's widget events: picking a slot, entering a name, backing out and the kONOPTION
 * action.
 *
 * kONNEXT (a slot widget, not busy, display state kDISPLAY_ON): takes the slot from the widget (+0x18) into +0x1C4 and
 * the card manager's mFileCursor. On the create and save screens (cFEStateProfileLoad_mScreenAction kScreenActionCreate
 * or kScreenActionSave), in state kCHECKING with the main button active (+0x224), it opens the name keyboard: the first
 * time it builds a cKeyboardPopup ("Keyboard", 1092 bytes) with miCurrentKey 55, at most 8 and at least 1 character,
 * key 75 (space) included in both key sets, mbDisableForFirst set, keys 0xF-0x18 excluded from the alternate set
 * (cKeyboardPopup_setDisableShiftNumbers) and keys 14, 25, 26, 39, 40, 52, 53, 64, 65 and 66 from both; each time it
 * sets the "kT_KEYUserName" label, mbClearOnFirst and the player's current name, pushes it as a popup on the engine's
 * state stack (cUIStateStack_pushPopup) and sets +0x1A0 and clears +0x19C. On the load screen (kScreenActionLoad) with
 * the main button active it starts a load of file type 1 (cMCOverlayManager_readSetFileType) and calls this state's
 * vtable slot 44 (cFEMemCard_confirmLoad).
 *
 * kONKEYBOARDEND (the keyboard is done): stores the entered name (mszEditString) as the player's name
 * (cBENewPlayerInterface_setPlayerName, for the player at +0x1CC) and in the manager's mNewFileName, or
 * "kT_MEMPlayerName" formatted with the player number when it is empty; submits the player interface (vtable slot 1,
 * cBENewPlayerInterface_submit) and starts the save (cFEStateProfileLoad_startSave).
 *
 * kONPREV (back): unless busy is 1 or state is kDONE, sets +0x1E4, resets the manager (cMCOverlayManager_SetState,
 * kMCOverlay_Idle) when state is kSTART, kCHECKING or kSKIP, and sets state kDONE. kONOPTION: unless busy, in state
 * kDONE, on the load screen or with the delete
 * button inactive (+0x220), and only when the selected device passes the card's slot-54 check (cPS2MemCard_IsActionable): takes
 * the widget's slot as for kONNEXT; in state kCHECKING with a file in it (cPS2MemCard_HasFileName) sets busy and state
 * kDELETING, cMCOverlayManager_SetAction(0) and manager state kMCOverlay_DeleteConfirm. kONKEYBOARDCANCEL does nothing.
 * Fills slot +0x9C (onWidgetEvent) of cFEStateProfileLoad_vtbl.
 *
 * @param vself  the profile load state
 * @param item  the widget that sent the event (its +0x18 word is the slot index)
 * @param event  the widget event
 */
extern "C" void cFEStateProfileLoad_onWidgetEvent(cFEStateProfileLoad* vself, cUIThing* item, int event)
{
    cFEStateProfileLoad* self = vself;
    switch (event) {
    case cUIState::kONKEYBOARDEND: {
        char buf[0x70];
        char fmt[0x70];
        char* name = self->mpKeyboard->mszEditString;
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        cBEInterface* player = cBE_getInterface_Fv(cBE_getBE(), 1);
        if (strlen(name) != 0) {
            cBENewPlayerInterface_setPlayerName(player, self->mPlayerIdx, name);
            func_002C2540(mp->mNewFileName, name);
        } else {
            strcpy2(fmt, GetText_K18FD90(str_kT_MEMPlayerName_b));
            sprintf(buf, fmt, self->mPlayerIdx + 1);
            cBENewPlayerInterface_setPlayerName(player, self->mPlayerIdx, buf);
            func_002C2540(mp->mNewFileName, buf);
        }
        player->submit();
        vself->startSave();
        break;
    }
    case cUIState::kONNEXT: {
        if (((char*)item) == 0) break;
        if (self->mIsProcessing != 0) break;
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        if (self->mScreenStatus != cFEMemCard::kDISPLAY_ON) break;
        mp->mFileCursor = self->mSlot = item->mMiscData;
        switch (cFEStateProfileLoad_mScreenAction) {
        case cFEStateProfileLoad::kScreenActionCreate:
        case cFEStateProfileLoad::kScreenActionSave:
            if (self->mState == cFEMemCard::kCHECKING && self->mMainButtonActive != 0) {
                if (((char*)self->mpKeyboard) == 0) {
                    cKeyboardPopup* kb = (cKeyboardPopup*)cKeyboardPopup_cKeyboardPopup(
                        cMemMan_alloc(1092, str_Keyboard, 0x100, 0), ((void*)self->mpEngine), vself, 0,
                        self->mCtrlBits);
                    self->mpKeyboard = kb;
                    kb->miCurrentKey = 55;
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
                cKeyboardPopup_setStatic(((char*)self->mpKeyboard), GetText_K18FD90(str_kT_KEYUserName));
                self->mpKeyboard->mbClearOnFirst = 1;
                cKeyboardPopup_setInputText(((char*)self->mpKeyboard), cBENewPlayerInterface_getPlayerName(cBE_getInterface_Fv(cBE_getBE(), 1), self->mPlayerIdx));
                cUIStateStack_pushPopup(&self->mpScreen->mpState->mpEngine->mStateStack, ((char*)self->mpKeyboard));
                self->mProcessingKeyboard = 1;
                self->mPopupType = cFEMemCard::kPOP_KEYBOARD;
            }
            break;
        case cFEStateProfileLoad::kScreenActionLoad:
            if (self->mMainButtonActive != 0) {
                cMCOverlayManager_readSetFileType(mp, 1);
                self->confirmLoad();
            }
            break;
        }
        break;
    }
    case cUIState::kONPREV: {
        cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
        if (self->mIsProcessing != true) {
            int st = self->mState;
            if (st != cFEMemCard::kDONE) {
                self->mIsCancelPressed = 1;
                if (st == cFEMemCard::kSTART || st == cFEMemCard::kSKIP || st == cFEMemCard::kCHECKING) {
                    mp->SetState(kMCOverlay_Idle);
                }
                self->mState = cFEMemCard::kDONE;
            }
        }
        break;
    }
    case cUIState::kONOPTION:
        if (self->mIsProcessing != true && self->mState != cFEMemCard::kDONE
            && cFEStateProfileLoad_mScreenAction != cFEStateProfileLoad::kScreenActionLoad
            && self->mDeleteButtonActive != 0) {
            cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
            if (mp->mpMemCard->IsActionable(mp->mSelectedDevice) != 0) {
                mp->mFileCursor = self->mSlot = item->mMiscData;
                if (self->mState == cFEMemCard::kCHECKING) {
                    if (mp->mpMemCard->HasFileName(self->mSlot) != 0) {
                        self->mIsProcessing = 1;
                        self->mState = cFEMemCard::kDELETING;
                        cMCOverlayManager_SetAction(mp, 0);
                        mp->SetState(kMCOverlay_DeleteConfirm);
                    }
                }
            }
        }
        break;
    case cUIState::kONKEYBOARDCANCEL:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initGeneral);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Title_SaveProfile[];
extern char str_Title_LoadProfile[];
extern char str_helptext_save[];
extern char str_helptext_load[];
extern char str_btext3[];
extern char str_btext3a[];
extern char str_btext5[];
extern char str_pct_d_b[];
extern char str_playerpct_d[];
extern char str_onmenu[];
extern char str_offmenu[];

static inline cUIThing* getObj_02C0(cFEStateProfileLoad* self, char* name)
{
    return cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(name));
}

/**
 * Hides the profile panels, numbers the six slot rows and sets up the menus.
 *
 * Hides the save/load titles and help texts and the buttons "btext3", "btext3a", "btext5"; numbers "1".."6",
 * gives "player1".."player6" misc data 0..5, disables "onmenu" and enables "offmenu".
 *
 * @param selfp  the cFEStateProfileLoad
 */
// Hides the profile panels, numbers the six slot rows and enables the confirm buttons.
void cFEStateProfileLoad::initGeneral()
{
    cUIThing* widget;
    widget = getObj_02C0(this, str_Title_SaveProfile);
    widget->setVisible(0);
    widget = getObj_02C0(this, str_Title_LoadProfile);
    widget->setVisible(0);
    widget = getObj_02C0(this, str_helptext_save);
    widget->setVisible(0);
    widget = getObj_02C0(this, str_helptext_load);
    widget->setVisible(0);
    widget = getObj_02C0(this, str_btext3);
    widget->setVisible(0);
    widget = getObj_02C0(this, str_btext3a);
    widget->setVisible(0);
    widget = getObj_02C0(this, str_btext5);
    widget->setVisible(0);
    char buf[0x10];
    for (int i = 1; i < 7; i++) {
        sprintf(buf, str_pct_d_b, i);
        widget = getObj_02C0(this, buf);
        cUIText_setAsciiString((cUIText*)widget, buf);
        sprintf(buf, str_playerpct_d, i);
        widget = getObj_02C0(this, buf);
        widget->mMiscData = i - 1;
    }
    getObj_02C0(this, str_onmenu)->setFocus(0);
    getObj_02C0(this, str_offmenu)->setFocus(1);
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern char str_Title_SaveProfile[];
extern char str_helptext_save[];
extern char str_kT_FEProfileNew[];
extern char str_kT_HELPCreateSaveNewFile[];
extern char str_btext3[];
extern char str_btext5[];

/**
 * Sets the profile screen up for creating a new profile save.
 *
 * Activates "Title_SaveProfile" (kT_FEProfileNew), "helptext_save" (kT_HELPCreateSaveNewFile) and the buttons
 * "btext3" and "btext5", and sets mDeleteButtonActive and mMainButtonActive.
 */
void cFEStateProfileLoad::initCreateScreen()
{
    cUIText* titleText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Title_SaveProfile));
    titleText->setVisible(1);
    cUIText_setUnicodeStringByID(titleText, GetHashValue32(str_kT_FEProfileNew));
    cUIText* helpText = (cUIText*)cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_helptext_save));
    helpText->setVisible(1);
    cUIText_setUnicodeStringByID(helpText, GetHashValue32(str_kT_HELPCreateSaveNewFile));
    cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_btext3))->setVisible(1);
    cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_btext5))->setVisible(1);
    mDeleteButtonActive = 1;
    mMainButtonActive = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initLoadScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cSSXApp_getMCManager(void* app);
extern char str_Title_LoadProfile[];
extern char str_helptext_load[];
extern char str_btext3a[];
extern char str_ps2sq[];


/**
 * Sets up the profile screen as the load screen.
 *
 * Shows "Title_LoadProfile", "helptext_load" and "btext3a", hides "ps2sq" (cUIThing_setVisible), clears
 * mDeleteButtonActive, sets mMainButtonActive and clears the card manager's mPollForFullCard.
 * cFEStateProfileLoad_onCreateScreen runs it when cFEStateProfileLoad_mScreenAction is kScreenActionLoad.
 */
void cFEStateProfileLoad::initLoadScreen()
{
    cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Title_LoadProfile))->setVisible(1);
    cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_helptext_load))->setVisible(1);
    cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_btext3a))->setVisible(1);
    cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_ps2sq))->setVisible(0);
    mDeleteButtonActive = 0;
    mMainButtonActive = 1;
    ((cMCOverlayManager*)cSSXApp_getMCManager(SSXApp))->mPollForFullCard = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initSaveScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Title_SaveProfile[];
extern char str_helptext_save[];
extern char str_btext3[];
extern char str_btext5[];
/**
 * Sets up the profile screen as the save screen.
 *
 * Shows "Title_SaveProfile", "helptext_save", "btext3" and "btext5" (cUIThing_setVisible) and sets
 * mDeleteButtonActive and mMainButtonActive. cFEStateProfileLoad_onCreateScreen
 * runs it when cFEStateProfileLoad_mScreenAction is kScreenActionSave.
 */
void cFEStateProfileLoad::initSaveScreen()
{
    cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_Title_SaveProfile))->setVisible(1);
    cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_helptext_save))->setVisible(1);
    cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_btext3))->setVisible(1);
    cUIScreen_getObjectByHashName(mpScreen, GetHashValue32(str_btext5))->setVisible(1);
    mDeleteButtonActive = 1;
    mMainButtonActive = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateRiderBio_cFEStateRiderBio);
#ifdef SKIP_ASM
extern void* cFEStateRiderBio_vtbl[];
extern "C" void* cUIState_cUIState(void* self);
extern "C" unsigned char cFECustom_getPlayerCtrlMask(void* self, int a1);

/**
 * Constructs the rider bio state (state id 0xF) for a user.
 *
 * Runs the cUIState constructor (cUIState_cUIState) on @p engine, installs cFEStateRiderBio_vtbl, stores @p userNum (as a
 * signed char) at +0x44 and the user's controller bits at +0x15 (cFECustom_getPlayerCtrlMask on the engine's owner).
 * cFECustom_getNextState allocates it as "cFEStateRiderBio" (80 bytes) with the current state's user number.
 *
 * @param engine  the UI engine
 * @param userNum  the user number
 * @return @p self
 */
extern "C" void* cFEStateRiderBio_cFEStateRiderBio(cFEStateRiderBio* self, int engine, int userNum)
{
    signed char idx = userNum;
    cUIState_cUIState(self);
    VPTR_OF(cListNode, self) = cFEStateRiderBio_vtbl;
    self->mHashName = 0xF;
    self->mUserNum = idx;
    *(char*)&self->mCtrlBits = cFECustom_getPlayerCtrlMask((void*)self->mpEngine->mpCustom, idx);
    return ((void*)self);
}
#endif

