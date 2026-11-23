#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "fe/keyboardpopup.h"
#include "movie/mcoverlaymanager.h"
#include "fe/festateprofile.h"
#include "ui/uithing.h"
#include "fe/festateriderbio.h"
#include "be/beintnewrace.h"
#include "main/ssxapp.h"

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
extern "C" void cFEStateProfileSelect_onCreateScreen(cFEStateProfileSelect* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_05sel_prof), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileSelect_onWidgetEvent);
#ifdef SKIP_ASM
extern "C" void cUIStateStack_push(void* self, void* state);

struct sColorK18EB40 {
    float r, g, b, a;
    sColorK18EB40(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
};
class cUIObjK18EB40 {
public:
    int f0, f4;
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
    virtual void setColor(const sColorK18EB40& c);
};


/**
 * Handles select, back and highlight events on the profile select screen.
 *
 * Event 5 recolours the entry and pushes the next state for its misc data (custom interface slot 4); event 6
 * pushes the previous state (slot 5); events 1 and 2 set the highlight and normal colours.
 *
 * @param menu  the widget the event is for (null: nothing happens)
 * @param msg  UI event code (1, 2, 5 or 6)
 */
extern "C" void cFEStateProfileSelect_onWidgetEvent(cFEStateProfileSelect* self, cUIThing* menu, unsigned int msg)
{
    if (((char*)menu) == 0)
        return;
    switch (msg) {
    case 5: {
        ((cUIObjK18EB40*)menu)->setColor(sColorK18EB40(1.0f, 1.0f, 0.0f, 0.0f));
        char* custom = *(char**)self->mpEngine;
        sVEntry<void*(void*, void*, int)>* vt = *(sVEntry<void*(void*, void*, int)>**)(custom + 4);
        void* nextState = vt[4].fn(vthis(custom, vt[4]), self, menu->mMiscData);
        if (nextState != 0)
            cUIStateStack_push((char*)self->mpEngine + 0x18, nextState);
        break;
    }
    case 6: {
        char* custom = *(char**)self->mpEngine;
        sVEntry<void*(void*, void*, int)>* vt = *(sVEntry<void*(void*, void*, int)>**)(custom + 4);
        void* nextState = vt[5].fn(vthis(custom, vt[5]), self, menu->mMiscData);
        if (nextState != 0)
            cUIStateStack_push((char*)self->mpEngine + 0x18, nextState);
        break;
    }
    case 1: {
        ((cUIObjK18EB40*)menu)->setColor(sColorK18EB40(1.0f, 0.0f, 1.0f, 0.0f));
        break;
    }
    case 2: {
        ((cUIObjK18EB40*)menu)->setColor(sColorK18EB40(1.0f, 1.0f, 1.0f, 1.0f));
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
    self->mVtbl = cFEStateProfileLoad_vtbl;
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
    signed char* newRace = (signed char*)((char*)&cBENewRaceInterface_newRace);
    int n = 1;
    if (newRace[0x49] == 2) {
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
 * After initGeneral, mode cFEStateProfileLoad_mScreenAction picks initCreateScreen (1), cFEStateProfileLoad_initLoadScreen (2) or cFEStateProfileLoad_initSaveScreen (3),
 * probably On Tour's initLoadScreen and initSaveScreen.
 */
extern "C" void cFEStateProfileLoad_onCreateScreen(cFEStateProfileLoad* self)
{
    void* engine = self->mpEngine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_93profile_load), 0);
    self->mpScreen = (cUIScreen*)screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cFEStateProfileLoad_initGeneral(self);
    switch (cFEStateProfileLoad_mScreenAction) {
    case 1:
        cFEStateProfileLoad_initCreateScreen(self);
        break;
    case 2:
        cFEStateProfileLoad_initLoadScreen(self);
        break;
    case 3:
        cFEStateProfileLoad_initSaveScreen(self);
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
struct cList0018EEB8 {
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
    virtual int v54(int a);
};
extern "C" void* cSSXApp_getMCManager(void* app);
extern int cFEStateProfileLoad_mScreenAction;

/**
 * Answers the profile load screen's menu events: refuses events the current screen cannot take.
 *
 * Event 6 gives 0x100 when the card manager's mSelectedDevice fails the memory card's slot-54 check (mpMemCard,
 * func_002D3840); event 8 gives 0x100 on the load screen (cFEStateProfileLoad_mScreenAction 2) or while the delete
 * button is inactive (+0x220 clear); event 9 always gives 0x100; everything else gives 0x101. Fills slot +0xA4
 * (onWidgetMenu) of cFEStateProfileLoad_vtbl.
 *
 * @param widget  the menu that sent the event (unused)
 * @param msg  the menu event
 * @return 0x100 to refuse the event, 0x101 to allow it
 */
extern "C" int cFEStateProfileLoad_onWidgetMenu(cFEStateProfileLoad* self, void* widget, unsigned int msg)
{
    char* app = (char*)cSSXApp_getMCManager(SSXApp);
    switch (msg) {
    case 6:
        if ((*(cList0018EEB8**)(app + 0x434))->v54(*(int*)(app + 0x428)) == 0) {
            return 0x100;
        }
        break;
    case 8:
        if (cFEStateProfileLoad_mScreenAction == 2 || self->mDeleteButtonActive == 0) {
            return 0x100;
        }
        break;
    case 9:
        return 0x100;
    }
    return 257;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_hideActionButtons);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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
 * likewise "btext3", "btext5" and "ps2sq" unless this is the load screen (cFEStateProfileLoad_mScreenAction 2), where
 * it is "btext3a"; then stores @p on in +0x234. Fills slot +0x12C of cFEStateProfileLoad_vtbl
 * (cFEMemCard_hideActionButtons's slot).
 *
 * @param on  1 to hide the buttons, 0 to show them
 */
extern "C" void cFEStateProfileLoad_hideActionButtons(cFEStateProfileLoad* self, int on)
{
    if (self->mIsHidden == on)
        return;
    int off = on ^ 1;
    ((cUIObj_EF70*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x)))->setVisible(off);
    if (cFEStateProfileLoad_mScreenAction != 2) {
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3)))->setVisible(off);
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext5)))->setVisible(off);
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq)))->setVisible(off);
    } else {
        ((cUIObj_EF70*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3a)))->setVisible(off);
    }
    self->mIsHidden = on;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_hideDeleteButton);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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
 * Except on the load screen (cFEStateProfileLoad_mScreenAction 2) sets "btext5" and "ps2sq" visible when @p on is false
 * (cUIThing_setVisible); always stores !@p on in +0x220, which cFEStateProfileLoad_onWidgetMenu tests
 * for event 8. Fills slot +0x134 of cFEStateProfileLoad_vtbl (cFEMemCard_hideDeleteButton's slot).
 *
 * @param on  true to hide the button
 */
extern "C" void cFEStateProfileLoad_hideDeleteButton(cFEStateProfileLoad* self, bool on)
{
    if (cFEStateProfileLoad_mScreenAction != 2) {
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
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_btext3[];
extern char str_btext3a[];
extern char str_ps2x[];
extern int cFEStateProfileLoad_mScreenAction;

struct cUIObj_F168 {
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
 * Shows or hides the profile load screen's main (accept) button.
 *
 * Except on the load screen (cFEStateProfileLoad_mScreenAction 2) shows "btext3" and "ps2x" only when @p saveBlocked is
 * false; on the load screen shows "btext3a" and "ps2x" only when @p loadBlocked is false. The shown state is also
 * stored in +0x224, which cFEStateProfileLoad_onWidgetEvent and cFEStateProfileLoad_startSave require. Fills slot +0x13C of cFEStateProfileLoad_vtbl
 * (cFEMemCard_hideMainButton's slot).
 *
 * @param loadBlocked  true to hide the button on the load screen
 * @param saveBlocked  true to hide the button on the other screens
 */
extern "C" void cFEStateProfileLoad_hideMainButton(cFEStateProfileLoad* self, bool loadBlocked, bool saveBlocked)
{
    cUIObj_F168* xButton = (cUIObj_F168*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2x));
    if (cFEStateProfileLoad_mScreenAction != 2) {
        int showButton = !saveBlocked;
        ((cUIObj_F168*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3)))->setVisible(showButton);
        self->mMainButtonActive = showButton;
        xButton->setVisible(showButton);
    } else {
        int showButton = !loadBlocked;
        ((cUIObj_F168*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3a)))->setVisible(showButton);
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
extern "C" void cFEStateProfileLoad_displayStatus(cFEStateProfileLoad* self)
{
    if (self->mScreenStatus == 3) {
        cFEStateProfileLoad_displayOn(self);
    } else {
        cFEStateProfileLoad_displayOff(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_displayOn);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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

struct sColor_F2B8 {
    int c[4];
};
extern sColor_F2B8 gStaticColour;

#define V_F2B8(n) virtual void v##n();
class cMCSub_F2B8 {
public:
    V_F2B8(01) V_F2B8(02) V_F2B8(03) V_F2B8(04) V_F2B8(05) V_F2B8(06) V_F2B8(07) V_F2B8(08) V_F2B8(09) V_F2B8(10)
    V_F2B8(11) V_F2B8(12) V_F2B8(13)
    virtual void GetTitleString(int, void*);
    virtual void GetFileDate(int, void*);
    virtual void GetFileTime(int, void*);
    V_F2B8(17) V_F2B8(18) V_F2B8(19) V_F2B8(20)
    V_F2B8(21) V_F2B8(22) V_F2B8(23) V_F2B8(24) V_F2B8(25) V_F2B8(26) V_F2B8(27) V_F2B8(28) V_F2B8(29) V_F2B8(30)
    V_F2B8(31) V_F2B8(32) V_F2B8(33) V_F2B8(34) V_F2B8(35) V_F2B8(36) V_F2B8(37) V_F2B8(38) V_F2B8(39) V_F2B8(40)
    V_F2B8(41) V_F2B8(42) V_F2B8(43) V_F2B8(44) V_F2B8(45) V_F2B8(46) V_F2B8(47) V_F2B8(48) V_F2B8(49) V_F2B8(50)
    V_F2B8(51) V_F2B8(52)
    virtual int IsAvailable1();
    virtual int v54(int);
    virtual int IsFormatted1();
    V_F2B8(56) V_F2B8(57) V_F2B8(58) V_F2B8(59) V_F2B8(60)
    V_F2B8(61) V_F2B8(62) V_F2B8(63) V_F2B8(64) V_F2B8(65)
    virtual int IsFileSlotDisabled(int);
    virtual int HasFileName(int);
};

class cState_F2B8 {
public:
    int pad[2];
    V_F2B8(01) V_F2B8(02) V_F2B8(03) V_F2B8(04) V_F2B8(05) V_F2B8(06) V_F2B8(07) V_F2B8(08) V_F2B8(09) V_F2B8(10)
    V_F2B8(11) V_F2B8(12) V_F2B8(13) V_F2B8(14) V_F2B8(15) V_F2B8(16) V_F2B8(17) V_F2B8(18) V_F2B8(19) V_F2B8(20)
    V_F2B8(21) V_F2B8(22) V_F2B8(23) V_F2B8(24) V_F2B8(25) V_F2B8(26) V_F2B8(27) V_F2B8(28) V_F2B8(29) V_F2B8(30)
    V_F2B8(31) V_F2B8(32) V_F2B8(33) V_F2B8(34)
    virtual void triggerDisplayState(int);
    V_F2B8(36) V_F2B8(37) V_F2B8(38) V_F2B8(39) V_F2B8(40) V_F2B8(41)
    virtual int displayFileOK();
    virtual void setMenuHighlight();
};

class cUIObj_F2B8 {
public:
    int pad[2];
    V_F2B8(01) V_F2B8(02) V_F2B8(03) V_F2B8(04) V_F2B8(05) V_F2B8(06) V_F2B8(07)
    virtual void setStatic(int);
    virtual void setVisible(int);
    V_F2B8(10) V_F2B8(11) V_F2B8(12) V_F2B8(13) V_F2B8(14) V_F2B8(15) V_F2B8(16) V_F2B8(17) V_F2B8(18) V_F2B8(19)
    V_F2B8(20) V_F2B8(21) V_F2B8(22) V_F2B8(23) V_F2B8(24)
    virtual void setUseStaticColour(int);
};
#undef V_F2B8

static inline cUIObj_F2B8* getObj_F2B8(cState_F2B8* self, char* name)
{
    return (cUIObj_F2B8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(name));
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
 * @param vself  the cFEStateProfileLoad
 */
// Memory-card profile slots: refreshes the device name and the six slot rows (name, the two detail lines).
extern "C" void cFEStateProfileLoad_displayOn(cFEStateProfileLoad* vself)
{
    cState_F2B8* self = (cState_F2B8*)vself;
    unsigned short buf[800];
    char name[0x40];
    cMCOverlayManager* mp = (cMCOverlayManager*)cSSXApp_getMCManager(SSXApp);
    if (self->displayFileOK()) {
        if (!((cMCSub_F2B8*)mp->mpMemCard)->IsAvailable1()) {
            cFEMemCard_callbackDisplayNoCards();
            cFEStateProfileLoad_displayOff((cFEStateProfileLoad*)self);
            return;
        } else if (*(int*)((char*)self + 0x214) != 3) {
            self->triggerDisplayState(3);
        }
    }
    if (self->displayFileOK()) {
        int sel = 0;
        cMCOverlayManager_GetDeviceDisplayString(mp, mp->mSelectedDevice, buf, 800);
        cUIText_setUnicodeStringWrap(getObj_F2B8(self, str_Memory_card_text), buf);
        *(int*)((char*)self + 0x21C) = mp->mSelectedDevice;
        self->setMenuHighlight();
        cUIObj_F2B8* widget = getObj_F2B8(self, str_1percentage);
        widget->setVisible(0);
        for (int i = 1; i < 7; i++) {
            sprintf(name, str_playerpct_d, i);
            widget = getObj_F2B8(self, name);
            *(sColor_F2B8*)((char*)widget + 0x88) = gStaticColour;
            if (!((cMCSub_F2B8*)mp->mpMemCard)->HasFileName(i - 1) && !((cMCSub_F2B8*)mp->mpMemCard)->IsFileSlotDisabled(i - 1)
                && ((cMCSub_F2B8*)mp->mpMemCard)->IsFormatted1()) {
                ((cMCSub_F2B8*)mp->mpMemCard)->GetTitleString(i - 1, (char*)self + 0x238);
                cUIText_setUnicodeString(widget, (unsigned short*)((char*)self + 0x238));
                if (cFEStateProfileLoad_mScreenAction != 2 && sel == 0) {
                    sel = 1;
                    widget->setStatic(0);
                    widget->setUseStaticColour(0);
                } else {
                    widget->setUseStaticColour(1);
                    widget->setStatic(1);
                }
                sprintf(name, str_datepct_d, i);
                widget = getObj_F2B8(self, name);
                widget->setVisible(0);
                sprintf(name, str_timepct_d, i);
                widget = getObj_F2B8(self, name);
                widget->setVisible(0);
            } else if (((cMCSub_F2B8*)mp->mpMemCard)->v54(mp->mSelectedDevice)) {
                ((cMCSub_F2B8*)mp->mpMemCard)->GetTitleString(i - 1, (char*)self + 0x238);
                cUIText_setUnicodeString(widget, (unsigned short*)((char*)self + 0x238));
                if (((cMCSub_F2B8*)mp->mpMemCard)->IsFileSlotDisabled(i - 1)) {
                    widget->setStatic(1);
                    widget->setUseStaticColour(1);
                } else {
                    widget->setStatic(0);
                    widget->setUseStaticColour(0);
                }
                sprintf(name, str_datepct_d, i);
                widget = getObj_F2B8(self, name);
                ((cMCSub_F2B8*)mp->mpMemCard)->GetFileDate(i - 1, buf);
                cUIText_setUnicodeString(widget, buf);
                widget->setVisible(1);
                sprintf(name, str_timepct_d, i);
                widget = getObj_F2B8(self, name);
                ((cMCSub_F2B8*)mp->mpMemCard)->GetFileTime(i - 1, buf);
                cUIText_setUnicodeString(widget, buf);
                widget->setVisible(1);
            } else {
                ((cMCSub_F2B8*)mp->mpMemCard)->GetTitleString(i - 1, (char*)self + 0x238);
                cUIText_setUnicodeString(widget, (unsigned short*)((char*)self + 0x238));
                widget->setUseStaticColour(1);
                widget->setStatic(1);
                sprintf(name, str_datepct_d, i);
                widget = getObj_F2B8(self, name);
                widget->setVisible(0);
                sprintf(name, str_timepct_d, i);
                widget = getObj_F2B8(self, name);
                widget->setVisible(0);
            }
        }
        if (cFEMemCard_checkNextAvailableDevice(self, mp, mp->mSelectedDevice, 1) == -1)
            getObj_F2B8(self, str_arrw_left)->setVisible(0);
        else
            getObj_F2B8(self, str_arrw_left)->setVisible(1);
        if (cFEMemCard_checkNextAvailableDevice(self, mp, mp->mSelectedDevice, 0) == -1)
            getObj_F2B8(self, str_arrw_right)->setVisible(0);
        else
            getObj_F2B8(self, str_arrw_right)->setVisible(1);
    }
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_displayOff);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cFEStateProfileLoad_displayOn(cFEStateProfileLoad* self);
extern "C" void* cSSXApp_getMCManager(void* app);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern char str_playerpct_d[];

struct cSelf_F9D8 {
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
struct cCard_F9D8 {
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
struct cUIObj_F9D8 {
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
 * Resets the profile rows when no card is ready, or hands over to cFEStateProfileLoad_displayOn when one is.
 *
 * When this state's vtable slot 42 (cFEMemCard_displayFileOK) passes and the memory card (the card manager's mpMemCard) passes its
 * slot-53 check (cPS2MemCard_IsAvailable1), it switches the display state to 3 (slot 35, cFEMemCard_triggerDisplayState) and runs
 * cFEStateProfileLoad_displayOn. Otherwise it resets the six rows "player1" to "player6": slot 25
 * (cUIText_setUseStaticColour for a text) with 0 and setStatic(1). Called by cFEStateProfileLoad_displayStatus and by
 * cFEStateProfileLoad_displayOn when no card is found.
 *
 * @param vself  the profile load state
 */
extern "C" void cFEStateProfileLoad_displayOff(cFEStateProfileLoad* vself)
{
    cSelf_F9D8* self = (cSelf_F9D8*)vself;
    void* app = cSSXApp_getMCManager(SSXApp);
    if (self->isReady() != 0) {
        if ((*(cCard_F9D8**)((char*)app + 0x434))->isPresent() != 0) {
            self->setState(3);
            cFEStateProfileLoad_displayOn((cFEStateProfileLoad*)self);
            return;
        }
    }
    for (int i = 1; i < 7; i++) {
        char buf[64];
        sprintf(buf, str_playerpct_d, i);
        cUIObj_F9D8* playerButton = (cUIObj_F9D8*)cUIScreen_getObjectByHashName(*(void**)((char*)self + 0x40), GetHashValue32(buf));
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
 * Runs once: unless +0x1AC is already set it switches on +0x27C (On Tour mTypeOfQuit). 0 asks the engine owner (the
 * first word of the engine at +0x10) for the next state through its vtable slot 5 (cFECustom_getPrevState in cFECustom_vtbl)
 * with this state and the mode (+0x1BC), and pushes it with cUIStateStack_pushCheck. 1 sets up a race on the new-race
 * interface (game mode 4, event 0xC), sets cBENewRaceInterface_bIsBamLocation to 1 and the location (cBENewRaceInterface_newRace.iLocationIdx) to the player's last hub
 * (cBENewPlayerInterface_getLastHub), submits it (the interface's vtable slot 1, cBENewRaceInterface_submit) and makes a new
 * cPreGameLoadScreen the next app module. 2 clears gInLodge and pushes a new title state (cFEStateTitle_cFEStateTitle1,
 * "cFEStateTitle"). Sets +0x1AC afterwards. Fills slot +0x10C of cFEStateProfileLoad_vtbl (cFEMemCard_updateDone's
 * slot).
 */
extern "C" void cFEStateProfileLoad_updateDone(cFEStateProfileLoad* self)
{
    if (self->mIsDone == 0) {
        int sel = self->mTypeOfQuit;
        if (sel == 0) {
            char* frontEnd = *(char**)self->mpEngine;
            sVEntry<void*(void*, void*, int)>* vt = *(sVEntry<void*(void*, void*, int)>**)(frontEnd + 4);
            void* nextState = vt[5].fn(vthis(frontEnd, vt[5]), self, self->mType);
            if (nextState != 0)
                cUIStateStack_pushCheck((char*)self->mpEngine + 0x18, nextState);
        } else if (sel == 1) {
            cBEInterface* race = cBE_getInterface_Fv(cBE_getBE(), 0);
            void* pl = cBE_getInterface_Fv(cBE_getBE(), 1);
            cBENewRaceInterface_setGameMode(race, 4);
            cBENewRaceInterface_setGameEvent(race, 0xC);
            int x = cBENewPlayerInterface_getLastHub(pl, 0);
            cBENewRaceInterface_bIsBamLocation = 1;
            cBENewRaceInterface_newRace.iLocationIdx = x;
            race->submit();
            cAppMan_setNextModule((cAppMan*)SSXApp, (unsigned int)cPreGameLoadScreen_cPreGameLoadScreen(cMemMan_alloc(8, str_cPreGameLoadScreen, 0x100, 0)));
        } else if (sel == 2) {
            gInLodge = 0;
            void* st = cFEStateTitle_cFEStateTitle1(cMemMan_alloc(76, str_cFEStateTitle, 0, 0), self->mpEngine);
            cUIStateStack_push((char*)self->mpEngine + 0x18, st);
        }
    }
    self->mIsDone = 1;
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
void cFEStateProfileLoad_onDestroyScreen(void* vself)
{
    cFEStateProfileLoad* self = (cFEStateProfileLoad*)vself;
    cFEMemCard_onDestroyScreen(self);
    self->mpKeyboard = 0;
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

struct cSelf_FC98 {
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
    virtual void v35();
    virtual void showMessage(unsigned short* text, int a2, int a3, int a4, int a5, int a6);
};

/**
 * Starts saving the profile to the selected card slot when the save button is active.
 *
 * Only with the main button active (+0x224): starts a save of file type 1 for the player (+0x1CC) with cMCOverlayManager_writeSetFileType,
 * sets the state (+0x1C0) to 3, the busy flag (+0x1A8) and +0x1DC. Unless the status popup is already up (+0x210) or
 * +0x204 is set, it sets the popup type (+0x19C) to 22, fetches the card manager's message
 * (cMCOverlayManager_GetMessage, 800 characters into a 1600-byte "Temp String" buffer), shows it in the status popup
 * (this state's vtable slot 36, cFEMemCard_getPopup, with 1, 1, 0, 0, 1), sets +0x210 and frees the buffer. Called by
 * cFEStateProfileLoad_onWidgetEvent once a profile name has been entered.
 *
 * @param vself  the profile load state
 */
extern "C" void cFEStateProfileLoad_startSave(cFEStateProfileLoad* vself)
{
    cSelf_FC98* self = (cSelf_FC98*)vself;
    if (*(int*)((char*)self + 0x224) != 0) {
        cMCOverlayManager_writeSetFileType(cSSXApp_getMCManager(SSXApp), 1, *(int*)((char*)self + 0x1CC));
        *(int*)((char*)self + 0x1C0) = 3;
        *(int*)((char*)self + 0x1A8) = 1;
        *(int*)((char*)self + 0x1DC) = 1;
        if (*(int*)((char*)self + 0x210) == 0 && *(int*)((char*)self + 0x204) == 0) {
            *(int*)((char*)self + 0x19C) = 22;
            unsigned short* buf = (unsigned short*)operator_new_tag(1600, str_Temp_String, 0x100, 0);
            cMCOverlayManager_GetMessage(cSSXApp_getMCManager(SSXApp), buf, 800);
            self->showMessage(buf, 1, 1, 0, 0, 1);
            *(int*)((char*)self + 0x210) = 1;
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

struct sVEntryK18FD90 {
    short delta;
    short index;
    void* (*fn)(void*, ...);
};


#define VCALLV_K18FD90(obj, off, idx) (*(sVEntry<void(void*, ...)>**)((char*)(obj) + (off)))[idx].fn((char*)(obj) + (*(sVEntry<void(void*, ...)>**)((char*)(obj) + (off)))[idx].delta

#define VCALL_K18FD90(obj, off, idx) (*(sVEntryK18FD90**)((char*)(obj) + (off)))[idx].fn((char*)(obj) + (*(sVEntryK18FD90**)((char*)(obj) + (off)))[idx].delta




static inline void* GetText_K18FD90(char* key)
{
    char* db = (char*)SSXApp->FELocale;
    sVEntryK18FD90* vt = *(sVEntryK18FD90**)(db + 4);
    return vt[4].fn(vthis(db, vt[4]), GetHashValue32(key));
}

/**
 * Handles the profile load screen's widget events: picking a slot, entering a name, backing out and the event-7 action.
 *
 * Event 5 (a slot widget, not busy, display state 3): takes the slot from the widget (+0x18) into +0x1C4 and the card
 * manager's mFileCursor. On the create and save screens (cFEStateProfileLoad_mScreenAction 1 or 3), in state 1 with the
 * main button active (+0x224), it opens the name keyboard: the first time it builds a cKeyboardPopup ("Keyboard", 1092
 * bytes) with miCurrentKey 55, at most 8 and at least 1 character, key 75 (space) included in both key sets,
 * mbDisableForFirst set, keys 0xF-0x18 excluded from the alternate set (cKeyboardPopup_setDisableShiftNumbers) and keys 14, 25, 26, 39, 40, 52,
 * 53, 64, 65 and 66 from both; each time it sets the "kT_KEYUserName" label, mbClearOnFirst and the player's current
 * name, pushes it as a popup on the engine's state stack (cUIStateStack_pushPopup) and sets +0x1A0 and clears +0x19C.
 * On the load screen (2) with the main button active it starts a load of file type 1 (cMCOverlayManager_readSetFileType) and calls this
 * state's vtable slot 44 (cFEMemCard_confirmLoad).
 *
 * Event 13 (the keyboard is done): stores the entered name (mszEditString) as the player's name (cBENewPlayerInterface_setPlayerName, for the
 * player at +0x1CC) and in the manager's mNewFileName, or "kT_MEMPlayerName" formatted with the player number when it
 * is empty; submits the player interface (vtable slot 1, cBENewPlayerInterface_submit) and starts the save
 * (cFEStateProfileLoad_startSave).
 *
 * Event 6 (back): unless busy is 1 or state is 6, sets +0x1E4, resets the manager (cMCOverlayManager_SetState, state 0)
 * when state is 0, 1 or 2, and sets state 6. Event 7: unless busy, in state 6, on the load screen or with the delete
 * button inactive (+0x220), and only when the selected device passes the card's slot-54 check (func_002D3840): takes
 * the widget's slot as for event 5; in state 1 with a file in it (cPS2MemCard_HasFileName) sets busy and state 4,
 * cMCOverlayManager_SetAction(0) and manager state 0x18. Event 14 does nothing. Fills slot +0x9C (onWidgetEvent) of
 * cFEStateProfileLoad_vtbl.
 *
 * @param vself  the profile load state
 * @param item  the widget that sent the event (its +0x18 word is the slot index)
 * @param event  the widget event
 */
extern "C" void cFEStateProfileLoad_onWidgetEvent(cFEStateProfileLoad* vself, cUIThing* item, int event)
{
    cFEStateProfileLoad* self = vself;
    switch (event) {
    case 13: {
        char buf[0x70];
        char fmt[0x70];
        char* name = self->mpKeyboard->mszEditString;
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        char* player = (char*)cBE_getInterface_Fv(cBE_getBE(), 1);
        if (strlen(name) != 0) {
            cBENewPlayerInterface_setPlayerName(player, self->mPlayerIdx, name);
            func_002C2540(mp + 0x276, name);
        } else {
            strcpy2(fmt, GetText_K18FD90(str_kT_MEMPlayerName_b));
            sprintf(buf, fmt, self->mPlayerIdx + 1);
            cBENewPlayerInterface_setPlayerName(player, self->mPlayerIdx, buf);
            func_002C2540(mp + 0x276, buf);
        }
        VCALLV_K18FD90(player, 0xC, 1));
        cFEStateProfileLoad_startSave(vself);
        break;
    }
    case 5: {
        if (((char*)item) == 0) break;
        if (self->mIsProcessing != 0) break;
        char* mp = (char*)cSSXApp_getMCManager(SSXApp);
        if (self->mScreenStatus != 3) break;
        ((cMCOverlayManager*)mp)->mFileCursor = self->mSlot = item->mMiscData;
        switch (cFEStateProfileLoad_mScreenAction) {
        case 1:
        case 3:
            if (self->mState == 1 && self->mMainButtonActive != 0) {
                if (((char*)self->mpKeyboard) == 0) {
                    char* kb = (char*)cKeyboardPopup_cKeyboardPopup(cMemMan_alloc(1092, str_Keyboard, 0x100, 0), ((void*)self->mpEngine), vself, 0, self->mCtrlBits);
                    *(char**)&self->mpKeyboard = kb;
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
                cKeyboardPopup_setStatic(((char*)self->mpKeyboard), GetText_K18FD90(str_kT_KEYUserName));
                self->mpKeyboard->mbClearOnFirst = 1;
                cKeyboardPopup_setInputText(((char*)self->mpKeyboard), cBENewPlayerInterface_getPlayerName(cBE_getInterface_Fv(cBE_getBE(), 1), self->mPlayerIdx));
                cUIStateStack_pushPopup(*(char**)(*(char**)(((char*)self->mpScreen) + 0xD0) + 0x10) + 0x18, ((char*)self->mpKeyboard));
                self->mProcessingKeyboard = 1;
                self->mPopupType = 0;
            }
            break;
        case 2:
            if (self->mMainButtonActive != 0) {
                cMCOverlayManager_readSetFileType(mp, 1);
                VCALLV_K18FD90(self, 8, 44));
            }
            break;
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
                    VCALLV_K18FD90(mp, 1864, 1), 0);
                }
                self->mState = 6;
            }
        }
        break;
    }
    case 7:
        if (self->mIsProcessing != 1 && self->mState != 6 && cFEStateProfileLoad_mScreenAction != 2 && self->mDeleteButtonActive != 0) {
            char* mp = (char*)cSSXApp_getMCManager(SSXApp);
            if (VCALL_K18FD90(*(char**)(mp + 1076), 0, 54), *(int*)(mp + 0x428)) != 0) {
                ((cMCOverlayManager*)mp)->mFileCursor = self->mSlot = item->mMiscData;
                if (self->mState == 1) {
                    if (VCALL_K18FD90(*(char**)(mp + 1076), 0, 67), self->mSlot) != 0) {
                        self->mIsProcessing = 1;
                        self->mState = 4;
                        cMCOverlayManager_SetAction(mp, 0);
                        VCALLV_K18FD90(mp, 1864, 1), 0x18);
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
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initGeneral);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
struct cUIText;
void cUIText_setAsciiString(cUIText* self, const char* str);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
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

struct cUIObj_02C0 {
    int pad[2];
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void setFocus(int on);
    virtual void v8();
    virtual void setVisible(int on);
};

static inline cUIObj_02C0* getObj_02C0(char* self, char* name)
{
    return (cUIObj_02C0*)cUIScreen_getObjectByHashName(*(void**)(self + 0x40), GetHashValue32(name));
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
extern "C" void cFEStateProfileLoad_initGeneral(cFEStateProfileLoad* selfp)
{
    cUIObj_02C0* widget;
    widget = getObj_02C0(((char*)selfp), str_Title_SaveProfile);
    widget->setVisible(0);
    widget = getObj_02C0(((char*)selfp), str_Title_LoadProfile);
    widget->setVisible(0);
    widget = getObj_02C0(((char*)selfp), str_helptext_save);
    widget->setVisible(0);
    widget = getObj_02C0(((char*)selfp), str_helptext_load);
    widget->setVisible(0);
    widget = getObj_02C0(((char*)selfp), str_btext3);
    widget->setVisible(0);
    widget = getObj_02C0(((char*)selfp), str_btext3a);
    widget->setVisible(0);
    widget = getObj_02C0(((char*)selfp), str_btext5);
    widget->setVisible(0);
    char buf[0x10];
    for (int i = 1; i < 7; i++) {
        sprintf(buf, str_pct_d_b, i);
        widget = getObj_02C0(((char*)selfp), buf);
        cUIText_setAsciiString((cUIText*)widget, buf);
        sprintf(buf, str_playerpct_d, i);
        widget = getObj_02C0(((char*)selfp), buf);
        *(int*)((char*)widget + 0x18) = i - 1;
    }
    getObj_02C0(((char*)selfp), str_onmenu)->setFocus(0);
    getObj_02C0(((char*)selfp), str_offmenu)->setFocus(1);
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern char str_Title_SaveProfile[];
extern char str_helptext_save[];
extern char str_kT_FEProfileNew[];
extern char str_kT_HELPCreateSaveNewFile[];
extern char str_btext3[];
extern char str_btext5[];

struct cUIObj_0540 {
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
 * Sets the profile screen up for creating a new profile save.
 *
 * Activates "Title_SaveProfile" (kT_FEProfileNew), "helptext_save" (kT_HELPCreateSaveNewFile) and the buttons
 * "btext3" and "btext5", and sets the words at +0x220 and +0x224 to 1.
 */
extern "C" void cFEStateProfileLoad_initCreateScreen(cFEStateProfileLoad* self)
{
    cUIObj_0540* titleText = (cUIObj_0540*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Title_SaveProfile));
    titleText->setVisible(1);
    cUIText_setUnicodeStringByID((cUIText*)titleText, GetHashValue32(str_kT_FEProfileNew));
    cUIObj_0540* helpText = (cUIObj_0540*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_save));
    helpText->setVisible(1);
    cUIText_setUnicodeStringByID((cUIText*)helpText, GetHashValue32(str_kT_HELPCreateSaveNewFile));
    ((cUIObj_0540*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3)))->setVisible(1);
    ((cUIObj_0540*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext5)))->setVisible(1);
    self->mDeleteButtonActive = 1;
    self->mMainButtonActive = 1;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initLoadScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void* cSSXApp_getMCManager(void* app);
extern char str_Title_LoadProfile[];
extern char str_helptext_load[];
extern char str_btext3a[];
extern char str_ps2sq[];

struct cUIObj_0670 {
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
 * Sets up the profile screen as the load screen.
 *
 * Shows "Title_LoadProfile", "helptext_load" and "btext3a", hides "ps2sq" (cUIThing_setVisible), clears the delete-button flag (+0x220), sets the main-button flag (+0x224) and clears the card manager's
 * mPollForFullCard. cFEStateProfileLoad_onCreateScreen runs it when cFEStateProfileLoad_mScreenAction is 2.
 */
extern "C" void cFEStateProfileLoad_initLoadScreen(cFEStateProfileLoad* self)
{
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Title_LoadProfile)))->setVisible(1);
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_load)))->setVisible(1);
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3a)))->setVisible(1);
    ((cUIObj_0670*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_ps2sq)))->setVisible(0);
    (self)->mDeleteButtonActive = 0;
    (self)->mMainButtonActive = 1;
    ((cMCOverlayManager*)cSSXApp_getMCManager(SSXApp))->mPollForFullCard = 0;
}
#endif

//100%
INCLUDE_ASM("fe/festateprofile", cFEStateProfileLoad_initSaveScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_Title_SaveProfile[];
extern char str_helptext_save[];
extern char str_btext3[];
extern char str_btext5[];
struct cUIObj_0768 {
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
 * Sets up the profile screen as the save screen.
 *
 * Shows "Title_SaveProfile", "helptext_save", "btext3" and "btext5" (cUIThing_setVisible) and sets the delete-button (+0x220) and main-button (+0x224) flags. cFEStateProfileLoad_onCreateScreen
 * runs it when cFEStateProfileLoad_mScreenAction is 3.
 */
extern "C" void cFEStateProfileLoad_initSaveScreen(cFEStateProfileLoad* self)
{
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_Title_SaveProfile)))->setVisible(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_save)))->setVisible(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext3)))->setVisible(1);
    ((cUIObj_0768*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_btext5)))->setVisible(1);
    self->mDeleteButtonActive = 1;
    self->mMainButtonActive = 1;
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
    self->mVtbl = cFEStateRiderBio_vtbl;
    self->mHashName = 0xF;
    self->mUserNum = idx;
    *(char*)&self->mCtrlBits = cFECustom_getPlayerCtrlMask(*(void**)self->mpEngine, idx);
    return ((void*)self);
}
#endif

