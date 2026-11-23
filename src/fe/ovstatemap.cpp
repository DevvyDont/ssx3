#include "vtable.h"
#include "common.h"
#include "be/beinterface.h"
#include "ui/uistate.h"
#include "ui/uimenu.h"
#include "util/locale.h"
#include "be/beintmission.h"
#include "ui/uitextscroll.h"
#include "ui/uiicon.h"
#include "bx/math3d.h"
#include "sound/ssxaudio.h"
#include "main/gamestate.h"
#include "ui/uitext.h"
#include "ui/uithing.h"
#include "fe/ovstatemap.h"
#include "ui/uiengine.h"
#include "be/beintnetwork.h"
#include "online/gamecomm.h"
#include "fe/ovstate.h"
#include "main/game.h"
#include "main/ssxapp.h"
#include "ai/ai.h"

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" void* cUIEngine_addScreenByHashName(void* engine, void* owner, int hash, int a3);
extern "C" void cUIScreen_playFrame(void* screen, int a1, int a2);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
struct sSelf0020A380;
extern "C" void cOVState_onCreateScreen(cOVState* self);
extern "C" int cFEAsyncManager_GetFreeAsyncFile(void* mgr);
extern "C" void cOVState_MAP_setupMapPic(cOVState_MAP* self);
extern "C" void cOVState_MAP_setupPopup(cOVState_MAP* self);
extern "C" void cOVState_MAP_setupPlayerIndicator(cOVState_MAP* self);
extern "C" void cOVState_MAP_setupLocalSessionList(cOVState_MAP* self);
extern char str_38session[];
extern char str_MapPic[];

class cUIObj_86A8 {
public:
    int pad[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setStatic(int v);
    virtual void setVisible(int v);
};

struct sMap_86A8 {
    char pad_0x0[0x10];
    void* engine;       // 0x10
    char pad_0x14[0xA8];
    int fBC;            // 0xBC
    int mSessionPointsOnPic;            // 0xC0
    int fC4;            // 0xC4
    char pad_0xC8[0x8];
    cUIObj_86A8* obj;   // 0xD0
    void* screen;       // 0xD4
    char pad_0xD8[0x4];
    int fDC;            // 0xDC
    int mpPlayerIcon;            // 0xE0
    int a[8];           // 0xE4
    int b[8];           // 0x104
    char pad_0x124[0x60];
    int c[8];           // 0x184
    int d[8];           // 0x1A4
    char pad_0x1C4[0x8];
    float scale;        // 0x1CC
    float speed;        // 0x1D0
};

/**
 * Creates the session map overlay screen ("38session") and builds its pieces.
 *
 * After cOVState_onCreateScreen it stores cFEAsyncManager_GetFreeAsyncFile(FE async manager) at +0xC4, closes the popup (+0xBC =
 * 0), sets scale 1.0 and speed 0.16, hides "MapPic" (kept at its field), clears the four 8-entry arrays, then
 * runs setupMapPic, setupPopup, setupPlayerIndicator and setupLocalSessionList.
 */
extern "C" void cOVState_MAP_onCreateScreen(sMap_86A8* self)
{
    void* engine = self->engine;
    void* screen = cUIEngine_addScreenByHashName(engine, self, GetHashValue32(str_38session), 0);
    self->screen = screen;
    if (screen != 0) {
        cUIScreen_playFrame(screen, 0, 0);
    }
    cOVState_onCreateScreen((cOVState*)((cOVState_MAP*)self));
    self->fC4 = cFEAsyncManager_GetFreeAsyncFile(SSXApp->mFEAsyncManager);
    self->fDC = 0;
    self->fBC = 0;
    self->scale = 1.0f;
    self->speed = 0.1599999964237213f;
    cUIObj_86A8* mapPic = (cUIObj_86A8*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_MapPic));
    self->obj = mapPic;
    if (mapPic != 0) {
        mapPic->setVisible(0);
    }
    self->mSessionPointsOnPic = 0;
    self->mpPlayerIcon = 0;
    for (int i = 0; i < 8; i++) {
        self->a[i] = 0;
        self->b[i] = 0;
        self->c[i] = 0;
        self->d[i] = 0;
    }
    cOVState_MAP_setupMapPic((cOVState_MAP*)self);
    cOVState_MAP_setupPopup((cOVState_MAP*)self);
    cOVState_MAP_setupPlayerIndicator((cOVState_MAP*)self);
    cOVState_MAP_setupLocalSessionList((cOVState_MAP*)self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onWidgetCreate__FPv);
#ifdef SKIP_ASM
/**
 * Map overlay's widget-creation handler; does nothing.
 *
 * It is the onWidgetCreate slot (+0x94) of cOVState_MAP_vtbl.
 */
void cOVState_MAP_onWidgetCreate(void* self)
{
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onGainTransition);
#ifdef SKIP_ASM
struct sV3_87F0 {
    float x, y, z;
};

struct sV4_87F0 {
    float x, y, z, w;
} __attribute__((aligned(16)));




class cRiderSub_87F0 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual sV4_87F0* getPosition();
};


typedef void (*VIntFn_87F0)(void*, int);
typedef void (*VPtrFn_87F0)(void*, void*);
typedef void* (*VGetFn_87F0)(void*, int);

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void cOVState_MAP_showIndicators(cOVState_MAP* self);
extern char kMAPCOLOR_REGULAR[];
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" int cBEMissionInterface_getNumberSessionsOnTrack(void* iface, int id);
extern "C" void sprintf2(void* buf, const char* fmt, void* str, int n);
extern "C" void cUIText_setUnicodeString(void* t, void* str);
extern "C" int cPathSys_getNearestSessionPoint(void* tbl, sV3_87F0* pos);
extern char str_SessionPointpct_d[];
extern char str_kT_MAPSessionPt[];
extern char str_kT_CMNTopOfRun[];
extern char str_kT_CMNBottomOfRun[];
extern char str_SessionViewPoints[];
extern char str_ConfirmMenu_Yes[];
extern char str_ConfirmMenu_No[];
extern char str_pct_s_pct_d_c[];
extern char gPaths[];

/**
 * Fills the session point list when the map overlay transitions in.
 *
 * For the 8 "SessionPoint%d" texts: points below the location's session count are enabled and labelled
 * kT_CMNTopOfRun (first), kT_CMNBottomOfRun (last, unless there are exactly 2) or "<kT_MAPSessionPt> n", with
 * misc data n; the rest are hidden and disabled. Then selects in "SessionViewPoints" the point nearest the
 * player's rider (cPathSys_getNearestSessionPoint on gPaths, minus 1), shows the indicators and gives the confirm Yes/No
 * entries misc data 0/1 and the regular colour.
 *
 * @param selfp  the cOVState_MAP (raw pointer)
 * @param on  nonzero when the transition starts; 0 does nothing
 * @return always 0
 */
extern "C" int cOVState_MAP_onGainTransition(cOVState_MAP* selfp, int on)
{
    if (on != 0) {
        int i = 0;
        cUIThing* menuItem;
        void* ifGame = cBE_getInterface_Fv(cBE_getBE(), 0);
        void* ifMap = cBE_getInterface_Fv(cBE_getBE(), 10);
        int n = cBEMissionInterface_getNumberSessionsOnTrack(ifMap, *cBENewRaceInterface_getLocation(ifGame));
        for (i = 0; i < 8; i++) {
            char buf[0x20];
            unsigned short text[40];
            sprintf(buf, str_SessionPointpct_d, i);
            menuItem = (cUIThing*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(buf));
            if (i < n) {
                (selfp)->mbSessionUnlocked[i] = 1;
                cFELocale* locale = SSXApp->FELocale;
                sVEntry<void>* vt = ((sVEntry<void>*)VPTR_OF(cUILocaleInterface, locale));
                void* str = ((VGetFn_87F0)vt[4].fn)(vthis(locale, vt[4]), GetHashValue32(str_kT_MAPSessionPt));
                if (str == 0) {
                    continue;
                }
                sprintf2(text, str_pct_s_pct_d_c, str, i + 1);
                if (menuItem == 0) {
                    continue;
                }
                if (i == 0) {
                    cUIText_setUnicodeStringByID((cUIText*)menuItem, GetHashValue32(str_kT_CMNTopOfRun));
                } else if (i == n - 1 && n != 2) {
                    cUIText_setUnicodeStringByID((cUIText*)menuItem, GetHashValue32(str_kT_CMNBottomOfRun));
                } else {
                    cUIText_setUnicodeString(menuItem, text);
                }
                menuItem->mMiscData = i + 1;
            } else {
                (selfp)->mbSessionUnlocked[i] = 0;
                menuItem->setVisible(0);
                menuItem->setStatic(1);
            }
        }
        void* menu = cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_SessionViewPoints));
        cGame* game = SSXApp->Game;
        if (((char*)game) != 0) {
            char* rider = (char*)game->AI->Riders[0];
            sV4_87F0 riderPos = *((cRiderSub_87F0*)(rider + 0x6C0))->getPosition();
            sV3_87F0 riderPos3;
            riderPos3.x = riderPos.x;
            riderPos3.y = riderPos.y;
            riderPos3.z = riderPos.z;
            int nearestPointId = cPathSys_getNearestSessionPoint(gPaths, &riderPos3);
            int idx = nearestPointId - 1;
            if (nearestPointId == 0) {
                idx = 0;
            }
            cUIMenu_setSelectedByIndex(menu, idx);
        }
        cOVState_MAP_showIndicators(selfp);
        menuItem = (cUIThing*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_ConfirmMenu_Yes));
        if (menuItem != 0) {
            menuItem->mMiscData = 0;
            menuItem->setColour((const tARGB*)kMAPCOLOR_REGULAR);
        }
        menuItem = (cUIThing*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_ConfirmMenu_No));
        if (menuItem != 0) {
            menuItem->mMiscData = 1;
            menuItem->setColour((const tARGB*)kMAPCOLOR_REGULAR);
        }
    }
    return 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onGainFocus);
#ifdef SKIP_ASM


int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern "C" void cUIState_onGainFocus(void* self, int a1);
extern char str_ConfirmMenu[];

/**
 * Runs the base gain-focus handling, then resets the "ConfirmMenu" object.
 *
 * Calls the object's virtual slots 7 and 9 with 0 (probably disable and hide).
 *
 * @param pPrevState  the state that had focus before, passed on to cUIState_onGainFocus
 */
extern "C" void cOVState_MAP_onGainFocus(cOVState_MAP* self, int pPrevState)
{
    cUIThing* confirmMenu;
    cUIState_onGainFocus(self, pPrevState);
    confirmMenu = (cUIThing*)cUIScreen_getObjectByHashName(self->mpMapScreen, GetHashValue32(str_ConfirmMenu));
    if (confirmMenu != 0) {
        confirmMenu->setFocus(0);
        confirmMenu->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onDestroyScreen);
#ifdef SKIP_ASM
extern "C" void cFEAsyncManager_UnloadFEAsyncFile(void* mgr, int file);
extern "C" void cOVState_MAP_cleanupIndicators(cOVState_MAP* self);
extern "C" void cOVState_onDestroyScreen(cOVState* self);

/**
 * Tears down the map overlay: unloads its map picture, deletes its marker widgets, then runs the base teardown.
 *
 * Unloads the async file slot kept at 0xC4, deletes the cursor and the two sets of 8 widgets (cOVState_MAP_cleanupIndicators()),
 * then calls cOVState_onDestroyScreen().
 */
extern "C" void cOVState_MAP_onDestroyScreen(cOVState_MAP* self)
{
    cFEAsyncManager_UnloadFEAsyncFile(SSXApp->mFEAsyncManager, self->mFEAsyncMapFileIndex);
    cOVState_MAP_cleanupIndicators(self);
    cOVState_onDestroyScreen((cOVState*)self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupScreenExit);
#ifdef SKIP_ASM


/**
 * Starts leaving the map overlay: flags the exit and hides its two pop-up widgets.
 *
 * Sets +0xDC to 1 and +0xC8 to 0, then calls virtual slot 9 with 0 on the widgets at +0xD0 and +0xE0 when they
 * exist.
 */
extern "C" void cOVState_MAP_setupScreenExit(cOVState_MAP* self)
{
    cUIThing* popupWidget;
    self->mScreenExiting = 1;
    self->mMapTextureSet = 0;
    popupWidget = (cUIThing*)self->mpMap;
    if (popupWidget != 0) {
        popupWidget->setVisible(0);
    }
    popupWidget = (cUIThing*)self->mpPlayerIcon;
    if (popupWidget != 0) {
        popupWidget->setVisible(0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onWidgetEvent);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
// From peak3project's fork (github.com/peak3project/ssx3).
extern "C" cSSXAudio* cAudioInterface_bxAudio();
extern "C" void func_0028FAE0(void* self);
extern "C" void cGame_setState(void* self, int a, int b, int refresh);
extern "C" void cOVState_MAP_setWidgetHiliteColor(cOVState_MAP* self, cUIText* obj, int on);
extern "C" void cOVState_MAP_setWidgetUnHiliteColor(cOVState_MAP* self, cUIText* obj, int on);
void* cUIStateStack_pop_8C28(void* self, int a1) __asm__("cUIStateStack_pop");
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern int cPauseState_StackIdx;
extern int cPauseState_Flags;
extern int cOVStateManager_mStatus;
extern int cPauseState_Stack[];
extern int cPauseState_FlagsTable[];
extern char str_ConfirmMenu_Yes_x[];
extern char str_ConfirmMenu_No_x[];
extern char str_ConfirmMenu_Yes_a[];
extern char str_ConfirmMenu_No_a[];
extern char str_ConfirmMenu_Yes_na[];
extern char str_ConfirmMenu_No_na[];

struct cVec_00208C28 {
    float x, y, z, w;
    cVec_00208C28(float a, float b, float c, float e) { x = a; y = b; z = c; w = e; }
};

class cObjV_00208C28 {
public:
    int pad[2];
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void set(const cVec_00208C28& v);
};

/**
 * Handles the session map's widget events: pick a session, confirm or cancel, and button highlights.
 *
 * Event 5 with the popup closed opens it for the picked session (misc data into +0xD8). With it open, "No"
 * closes it; "Yes" stores the session in mission interface (10) + 0x10, sets cOVStateManager_mStatus = 3, pops the
 * pause-mode stack (cPauseState_Stack/cPauseState_StackIdx into cPauseState_Flags), calls cGame_setState(game, 0xF, 0, 0) and closes
 * the overlay (cOVState_MAP_cleanupIndicators, cOVState_MAP_setupScreenExit).
 *
 * Event 6 closes the popup, or else closes the overlay, pops the state stack and marks this state pending
 * delete with mode 7. Events 1 and 2 (highlight on/off) recolour the popup's Yes/No buttons.
 *
 * @param selfp  the cOVState_MAP (raw pointer)
 * @param pWidget  the widget the event is for (null: nothing happens)
 * @param Event  UI event code (1, 2, 5, 6 or 7)
 */
// Map overlay message handler: confirm/cancel of the map popup and toggling of the legend objects.
extern "C" void cOVState_MAP_onWidgetEvent(cOVState_MAP* selfp, cUIThing* pWidget, int Event)
{
    if (((void*)pWidget) == 0) {
        return;
    }
    switch (Event) {
    case 5:
        if (selfp->mSessionPopupActive != 0) {
            if (pWidget->mMiscData != 0) {
                selfp->mSessionPopupActive = 0;
                cOVState_MAP_setupPopup(selfp);
                break;
            }
            void* iface = cBE_getInterface_Fv(cBE_getBE(), 10);
            *(int*)((char*)iface + 0x10) = selfp->mSelectedSessionPoint;
            func_0028FAE0(cAudioInterface_bxAudio());
            void* game = SSXApp->Game;
            cOVStateManager_mStatus = 3;
            cPauseState_Flags = cPauseState_FlagsTable[cPauseState_Stack[--cPauseState_StackIdx]];
            cGame_setState(game, 0xF, 0, 0);
            cOVState_MAP_cleanupIndicators(selfp);
            cOVState_MAP_setupScreenExit(selfp);
        } else {
            selfp->mSessionPopupActive = 1;
            selfp->mSelectedSessionPoint = pWidget->mMiscData;
            cOVState_MAP_setupPopup(selfp);
            cObjV_00208C28* confirmNo = (cObjV_00208C28*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_ConfirmMenu_No));
            confirmNo->set(cVec_00208C28(1.0f, 0.0f, 0.0f, 0.0f));
        }
        break;
    case 6:
        if (selfp->mSessionPopupActive != 0) {
            selfp->mSessionPopupActive = 0;
            cOVState_MAP_setupPopup(selfp);
        } else {
            cOVState_MAP_cleanupIndicators(selfp);
            cOVState_MAP_setupScreenExit(selfp);
            cUIStateStack_pop_8C28(&selfp->mpEngine->mStateStack, 1);
            selfp->mAttrState = (selfp->mAttrState & ~0x3F00) | 0x780;
        }
        break;
    case 1:
        if (selfp->mSessionPopupActive != 0) {
            cOVState_MAP_setWidgetHiliteColor(selfp, (cUIText*)pWidget, 1);
            if (pWidget->mMiscData == 0) {
                cUIState_showObjSafe(((char*)selfp), str_ConfirmMenu_Yes_x);
                cUIState_hideObjSafe(((char*)selfp), str_ConfirmMenu_No_x);
                cUIState_showObjSafe(((char*)selfp), str_ConfirmMenu_Yes_a);
                cUIState_hideObjSafe(((char*)selfp), str_ConfirmMenu_No_a);
                cUIState_showObjSafe(((char*)selfp), str_ConfirmMenu_Yes_na);
                cUIState_hideObjSafe(((char*)selfp), str_ConfirmMenu_No_na);
            } else {
                cUIState_showObjSafe(((char*)selfp), str_ConfirmMenu_No_x);
                cUIState_hideObjSafe(((char*)selfp), str_ConfirmMenu_Yes_x);
                cUIState_showObjSafe(((char*)selfp), str_ConfirmMenu_No_a);
                cUIState_hideObjSafe(((char*)selfp), str_ConfirmMenu_Yes_a);
                cUIState_showObjSafe(((char*)selfp), str_ConfirmMenu_No_na);
                cUIState_hideObjSafe(((char*)selfp), str_ConfirmMenu_Yes_na);
            }
        }
        break;
    case 2:
        if (selfp->mSessionPopupActive != 0) {
            cOVState_MAP_setWidgetUnHiliteColor(selfp, (cUIText*)pWidget, 1);
        }
        break;
    case 7:
        break;
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onWidgetMenu);
#ifdef SKIP_ASM
/**
 * Map overlay's menu-event filter: events 8 and 9 get 0, every other event 0x101 (animation and sound).
 *
 * It is the onWidgetMenu slot (+0xA4) of cOVState_MAP_vtbl.
 *
 * @param menu  unused
 * @param event  the menu event
 * @return 0 for events 8 and 9, else 0x101
 */
extern "C" int cOVState_MAP_onWidgetMenu(cOVState_MAP* self, int menu, int event)
{
    return ((unsigned int)(event - 8) < 2) ? 0 : 257;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupMapPic);
#ifdef SKIP_ASM
extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void cFEAsyncManager_UnloadFEAsyncFile(void* mgr, int file);
extern "C" int* cBENewRaceInterface_getLocation(void*);
extern "C" void cFEAsyncManager_LoadLocationPic(void* mgr, int a1, int file, int a3);

/**
 * Starts loading the map picture for the current race location.
 *
 * Unloads the map's async file slot (0xC4), queues cFEAsyncManager_LoadLocationPic() for the new-race
 * interface's location into the same slot, and clears the loaded flag at 0xC8 so the map's update fades the new
 * picture in once it has loaded.
 */
extern "C" void cOVState_MAP_setupMapPic(cOVState_MAP* self)
{
    void* iface = cBE_getInterface_Fv(cBE_getBE(), 0);
    void* mgr = SSXApp->mFEAsyncManager;
    cFEAsyncManager_UnloadFEAsyncFile(mgr, self->mFEAsyncMapFileIndex);
    cFEAsyncManager_LoadLocationPic(mgr, *cBENewRaceInterface_getLocation(iface), self->mFEAsyncMapFileIndex, 1);
    self->mMapTextureSet = 0;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupPopup);
#ifdef SKIP_ASM



typedef void (*VIntFn_9010)(void*, int);
typedef void* (*VGetFn_9010)(void*, int);

extern "C" void* cBE_getBE();
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int sprintf(char* buf, const char* fmt, ...);
struct cUIText;
void cUIText_setUnicodeStringByID(cUIText* t, int id);
void cUIText_setAsciiString(cUIText* self, const char* str);
int cBELibrary_getCharacterID(int);
extern "C" int cBEEconInterface_getMoney(void* self, int a, int b);
extern "C" void cUIMenu_setSelectedByIndex(void* menu, unsigned char idx);
extern "C" void sprintf2(void* buf, const char* fmt, void* str, int n);
extern "C" void cUIText_setUnicodeString(void* t, void* str);
extern "C" void cUIState_showObjSafe(void* self, char* name);
extern "C" void cUIState_hideObjSafe(void* self, char* name);
extern char str_ConfirmMenu[];
extern char str_SessionViewPoints[];
extern char str_ConfirmPopup[];
extern char str_Popup1[];
extern char str_Popup2[];
extern char str_kT_MAPSessionPt[];
extern char str_pct_s_pct_d_d[];
extern char str_PopupTitle[];
extern char str_pct_d_f[];
extern char str_P1_CashAvailable[];
extern char str_P1_SessionCost[];
extern char str_P1_ExtraText[];
extern char str_kT_OVRCMNSessArea[];
extern char str_ConfirmMenu_No_x[];
extern char str_ConfirmMenu_Yes[];
extern char str_ConfirmMenu_No[];
extern char str_kT_CMNYes_b[];

/**
 * Shows or hides the session purchase popup according to the popup flag (+0xBC).
 *
 * When open: shows "ConfirmPopup"/"Popup1", disables the session list, enables "ConfirmMenu", sets the title
 * "<kT_MAPSessionPt>: n" for the selected point, the player's money as "$%d", a cost of $500 and
 * kT_OVRCMNSessArea. When closed: hides the popup, re-enables the list and disables the menu.
 *
 * @param selfp  the cOVState_MAP
 */
extern "C" void cOVState_MAP_setupPopup(cOVState_MAP* selfp)
{
    cUIMenu* menu = (cUIMenu*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_ConfirmMenu));
    cUIMenu* list = (cUIMenu*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_SessionViewPoints));
    if (selfp->mSessionPopupActive != 0) {
        unsigned short text[40];
        char buf[0x10];
        cUIState_showObjSafe(((char*)selfp), str_ConfirmPopup);
        if (menu != 0) {
            cUIMenu_setSelectedByIndex(menu, 0);
        }
        unsigned char n = 0;
        cUIState_showObjSafe(((char*)selfp), str_Popup1);
        cUIState_hideObjSafe(((char*)selfp), str_Popup2);
        if (list != 0) {
            list->setFocus(0);
            n = list->mScrollItemCurrent;
        }
        if (menu != 0) {
            menu->setFocus(1);
            menu->setVisible(1);
        }
        cFELocale* locale = SSXApp->FELocale;
        sVEntry<void>* vt = ((sVEntry<void>*)VPTR_OF(cUILocaleInterface, locale));
        void* str = ((VGetFn_9010)vt[4].fn)(vthis(locale, vt[4]), GetHashValue32(str_kT_MAPSessionPt));
        if (str != 0) {
            sprintf2(text, str_pct_s_pct_d_d, str, n + 1);
            void* label = cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_PopupTitle));
            if (label != 0) {
                cUIText_setUnicodeString(label, text);
            }
        }
        int id = cBEEconInterface_getMoney(cBE_getInterface_Fv(cBE_getBE(), 11), 0, cBELibrary_getCharacterID(0));
        sprintf(buf, str_pct_d_f, id);
        cUIText* label = (cUIText*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_P1_CashAvailable));
        if (label != 0) {
            cUIText_setAsciiString(label, buf);
        }
        sprintf(buf, str_pct_d_f, 500);
        label = (cUIText*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_P1_SessionCost));
        if (label != 0) {
            cUIText_setAsciiString(label, buf);
        }
        label = (cUIText*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_P1_ExtraText));
        if (label != 0) {
            cUIText_setUnicodeStringByID(label, GetHashValue32(str_kT_OVRCMNSessArea));
        }
        cUIState_hideObjSafe(((char*)selfp), str_ConfirmMenu_No_x);
        cUIText* confirmYes = (cUIText*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_ConfirmMenu_Yes));
        cUIMenu* confirmNo = (cUIMenu*)cUIScreen_getObjectByHashName(selfp->mpMapScreen, GetHashValue32(str_ConfirmMenu_No));
        if (confirmYes != 0) {
            cUIText_setUnicodeStringByID(confirmYes, GetHashValue32(str_kT_CMNYes_b));
        }
        if (confirmNo != 0) {
            confirmNo->setVisible(1);
            confirmNo->setStatic(0);
        }
        cUIState_showObjSafe(((char*)selfp), str_ConfirmMenu_No);
    } else {
        cUIState_hideObjSafe(((char*)selfp), str_ConfirmPopup);
        if (list != 0) {
            list->setFocus(1);
        }
        if (menu != 0) {
            menu->setFocus(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onInputAlways);
#ifdef SKIP_ASM


extern "C" void cOVState_MAP_showIndicators(cOVState_MAP* self);

/**
 * Refreshes the map indicators when either of two input checks fires.
 *
 * The checks are the input's virtual slots 17 and 18 (probably up/down pressed).
 *
 * @param pInput  the UI input object
 */
extern "C" void cOVState_MAP_onInputAlways(cOVState_MAP* self, cUIThing* pInput)
{
    if (((sVEntry<int(void*)>*)VPTR_OF(cListNode, pInput))[17].fn(vthis(pInput, ((sVEntry<int(void*)>*)VPTR_OF(cListNode, pInput))[17])) != 0 || ((sVEntry<int(void*)>*)VPTR_OF(cListNode, pInput))[18].fn(vthis(pInput, ((sVEntry<int(void*)>*)VPTR_OF(cListNode, pInput))[18])) != 0) {
        cOVState_MAP_showIndicators(self);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_onUpdate);
#ifdef SKIP_ASM

struct sV3_9370 {
    float x, y, z;
    sV3_9370() {}
    sV3_9370(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

class cFEObj_9370 {
public:
    int pad_0x0[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void SetVisible(int on);
    virtual void v10();
    virtual void SetColor(const tARGB* c);
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void SetScale(const sV3_9370& s);
};


struct sAsyncFile_9370 {
    char pad_0x0[0x110];
    int data;                   // 0x110
    char pad_0x114[0x8];
};

struct sMap_9370 {
    char pad_0x0[0xC4];
    int file;                   // 0xC4
    int loaded;                 // 0xC8
    float fade;                 // 0xCC
    cFEObj_9370* img;           // 0xD0
    char pad_0xD4[0x8];
    int busy;                   // 0xDC
    char pad_0xE0[0x24];
    cFEObj_9370* icons[8];      // 0x104
    sV3_9370 iconPos[8];        // 0x124
    char pad_0x184[0x48];
    float bob;                  // 0x1CC
    float bobVel;               // 0x1D0
};

struct cFEAsyncManager;
int cFEAsyncManager_GetFileStatus(cFEAsyncManager* self, int index);
extern "C" void cOVState_onUpdate(void* self);

/**
 * Per-frame update of the mountain map overlay: fades the map picture in once it has loaded and pulses the player
 * indicators.
 *
 * Calls cOVState_onUpdate() first and stops there while +0xDC is set. Once the picture is loaded (+0xC8) its alpha
 * (+0xCC) rises by 0.1 per frame up to 1.0 and the picture object (+0xD0) is shown; until then the alpha is 0 and
 * the object hidden. When the async file manager reports file +0xC4 with status 3 and the picture is not yet
 * loaded, the file's data is attached to the picture object and it is marked loaded. The indicator size (+0x1CC)
 * swings between 18 and 21.5 at 0.16 per frame (+0x1D0); each of the 8 indicator objects (+0x104) is placed at its
 * base position (+0x124) minus half the growth, so it stays centred, and scaled to that size.
 */
extern "C" void cOVState_MAP_onUpdate(sMap_9370* self)
{
    cOVState_onUpdate(self);
    if (self->busy != 0) {
        return;
    }
    if (self->loaded != 0) {
        if (self->fade < 1.0f) {
            self->fade += 0.10000000149011612f;
        }
        cFEObj_9370* feObj = self->img;
        tARGB colour = (*(&((cUIIcon*)feObj)->mColour));
        colour.A = self->fade;
        feObj->SetColor(&colour);
        self->img->SetVisible(1);
    } else {
        self->fade = 0.0f;
        self->img->SetVisible(0);
    }
    sAsyncFile_9370* am = (sAsyncFile_9370*)SSXApp->mFEAsyncManager;
    if (cFEAsyncManager_GetFileStatus((cFEAsyncManager*)am, self->file) == 3 && self->loaded == 0) {
        cFEObj_9370* feObj = self->img;
        if (feObj != 0) {
            int data = am[self->file].data;
            (*(int*)&((cUIIcon*)feObj)->mpTextureElem) = 0;
            ((cUIIcon*)feObj)->mTextureID = data;
            self->loaded = 1;
            self->fade = 0.0f;
        }
    }
    if (0.0f < self->bobVel) {
        if (21.5f < self->bob) {
            self->bobVel = -0.1599999964237213f;
        }
    } else if (self->bob < 18.0f) {
        self->bobVel = 0.1599999964237213f;
    }
    self->bob += self->bobVel;
    int i;
    for (i = 0; i < 8; i++) {
        cFEObj_9370* feObj = self->icons[i];
        if (feObj != 0) {
            sV3_9370* bp = &self->iconPos[i];
            float halfGrowth = (self->bob - 18.0f) * 0.5f;
            sV3_9370 placedPos;
            placedPos.x = bp->x - halfGrowth;
            placedPos.y = bp->y - halfGrowth;
            (*(sV3_9370*)&((cUIIcon*)feObj)->mPosition) = placedPos;
            self->icons[i]->SetScale(sV3_9370(self->bob, self->bob, 0.0f));
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setWidgetHiliteColor);
#ifdef SKIP_ASM
extern char kMAPCOLOR_HIGHLIGHTED[];
extern char kMAPCOLOR_HIGHLIGHTED_LOCKED[];

/**
 * Colours a highlighted map entry: the normal highlight when unlocked, the locked highlight otherwise.
 *
 * Calls the text's virtual slot 11 (set colour) with kMAPCOLOR_HIGHLIGHTED when @p on is nonzero, else with
 * kMAPCOLOR_HIGHLIGHTED_LOCKED.
 *
 * @param obj  the text widget
 * @param on   nonzero when the entry is unlocked
 */
extern "C" void cOVState_MAP_setWidgetHiliteColor(cOVState_MAP* self, cUIText* obj, int on)
{
    if (on != 0) {
        obj->setColour((const tARGB*)kMAPCOLOR_HIGHLIGHTED);
    } else {
        obj->setColour((const tARGB*)kMAPCOLOR_HIGHLIGHTED_LOCKED);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setWidgetUnHiliteColor);
#ifdef SKIP_ASM
extern char kMAPCOLOR_REGULAR[];
extern char kMAPCOLOR_REGULAR_LOCKED[];

/**
 * Gives a map text widget its normal (not highlighted) colour.
 *
 * Calls setColour (vtable slot 11) with kMAPCOLOR_REGULAR, or with kMAPCOLOR_REGULAR_LOCKED when @p on is 0.
 *
 * @param obj  text widget to colour (On Tour: pText)
 * @param on   nonzero for an unlocked entry (On Tour: unlocked)
 */
extern "C" void cOVState_MAP_setWidgetUnHiliteColor(cOVState_MAP* self, cUIText* obj, int on)
{
    if (on != 0) {
        obj->setColour((const tARGB*)kMAPCOLOR_REGULAR);
    } else {
        obj->setColour((const tARGB*)kMAPCOLOR_REGULAR_LOCKED);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupPlayerIndicator);
#ifdef SKIP_ASM
struct sV3_96A8 {
    float x, y, z;
    sV3_96A8() {}
    sV3_96A8(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
    sV3_96A8(const float& ax, const float& ay) : x(ax), y(ay), z(0.0f) {}
};



class cFEObj_96A8 {
public:
    int pad_0x0[2];
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void SetVisible(int on);
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
    virtual void SetScale(const sV3_96A8& s);
};


class cRiderSub_96A8 {
public:
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual t4Vector* getPosition();
};


struct cList;
struct cListNode;
void cList_addToEnd(cList* list, cListNode* node);
extern "C" void* cMemMan_alloc(int size, const char* tag, unsigned int flags, int d);
extern "C" void cBEMissionInterface_getPictureDBRecord(tBEPictureDB* out, void* iface, int id);
extern "C" void* cUIThing_cUIThing(void* self, void* parent, int a2);
extern "C" void func_0021CCC8(void* p);
extern "C" void cUIIcon_setTextureByHashValue(void* obj, int hash);
extern char str_Player_Indicator_Icon[];
extern char str_location[];
extern void* cUIIcon_vtbl[];

static inline float Absf_96A8(float v)
{
    if (v < 0.0f) {
        v = -v;
    }
    return v;
}

/**
 * Creates the hidden map icon that marks the player's position.
 *
 * Maps the player's rider position into the location's map bounds (cBEMissionInterface_getPictureDBRecord) as a 356 x 266 picture,
 * then adds a cUIIcon ("location" texture, 17 x 17, offset to the map at 252, 97) to the screen's object
 * list.
 *
 * @param selfp  the cOVState_MAP
 */
extern "C" void cOVState_MAP_setupPlayerIndicator(cOVState_MAP* selfp)
{
    cOVState_MAP* self = selfp;
    void* ifMap = cBE_getInterface_Fv(cBE_getBE(), 10);
    void* ifGame = cBE_getInterface_Fv(cBE_getBE(), 0);
    sV3_96A8 mapPos = (*(sV3_96A8*)&((cUIThing*)((cFEObj_96A8*)self->mpMap))->mPosition);
    char* rider = (char*)SSXApp->Game->AI->Riders[0];
    t4Vector pp = *((cRiderSub_96A8*)(rider + 0x6C0))->getPosition();
    tBEPictureDB bounds;
    cBEMissionInterface_getPictureDBRecord(&bounds, ifMap, *cBENewRaceInterface_getLocation(ifGame));
    float fx = Absf_96A8((pp.x - bounds.world_topleft_x) / (bounds.world_bottomright_x - bounds.world_topleft_x));
    int px = (int)(fx * 356.0f);
    float fy = Absf_96A8((pp.y - bounds.world_topleft_y) / (bounds.world_bottomright_y - bounds.world_topleft_y));
    int py = (int)(fy * 266.0f);
    cUIIcon* ind = (cUIIcon*)cMemMan_alloc(136, str_Player_Indicator_Icon, 0x20000000, 0);
    cUIThing_cUIThing(ind, ((char*)self->mpMapScreen), 0);
    VPTR_OF(cListNode, ind) = cUIIcon_vtbl;
    func_0021CCC8(&ind->mAttrIcon);
    ind->mTextureID = -1;
    (*(cFEObj_96A8**)&self->mpPlayerIcon) = (cFEObj_96A8*)ind;
    ind->mpTextureElem = 0;
    {
        sV3_96A8 indicatorPos((float)(unsigned int)(px + 0xFC) - 5.0f, (float)(unsigned int)(py + 97) - 5.0f, mapPos.z);
        (*(sV3_96A8*)&((cUIThing*)((cFEObj_96A8*)self->mpPlayerIcon))->mPosition) = indicatorPos;
    }
    ((cFEObj_96A8*)self->mpPlayerIcon)->SetScale(sV3_96A8(17.0f, 17.0f));
    cUIIcon_setTextureByHashValue(((cFEObj_96A8*)self->mpPlayerIcon), GetHashValue32(str_location));
    ((cFEObj_96A8*)self->mpPlayerIcon)->SetVisible(0);
    cUIThing* indicatorData = (cUIThing*)((cFEObj_96A8*)self->mpPlayerIcon);
    indicatorData->mAttr = (indicatorData->mAttr & ~0x1F00) | 0xD00;
    cList_addToEnd((cList*)(((char*)self->mpMapScreen) + 0xB4), (cListNode*)((cFEObj_96A8*)self->mpPlayerIcon));
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_setupLocalSessionList);
#ifdef SKIP_ASM
// PORT: PS2-only inline asm; needs a C fallback off-PS2.
extern "C" int cBEMissionInterface_getNumberSessionsOnTrack(void* iface, int id);
extern "C" int cPathSys_isStartpoint(void* tbl, int kind, int idx);
extern "C" char* cPathSys_getStartpoint(void* tbl, int kind, int idx);
extern char gPaths[];
extern char str_Map_Indicator_Icon[];
extern char str_dot_visited[];
extern char str_Map_Indicator_Highlight[];
extern char str_indicator[];

struct sMapSt_9970 {
    char pad_0x0[0xC0];
    int nSessions;              // 0xC0
    char pad_0xC4[0xC];
    cFEObj_96A8* img;           // 0xD0
    char* screen;               // 0xD4
    char pad_0xD8[0xC];
    cFEObj_96A8* markA[8];      // 0xE4
    cFEObj_96A8* markB[8];      // 0x104
    sV3_96A8 posB[8];           // 0x124
};

void* operator new(unsigned int size, const char* tag, unsigned int flags, int d) __asm__("cMemMan_alloc");

// The inlined cUIIcon ctor: cUIThing(screen, 0), vtable, member at 0x74, two fields.
struct cUIIcon_9970 {
    char d[0x88];
    cUIIcon_9970(char* screen)
    {
        cUIThing_cUIThing(this, screen, 0);
        *(void***)((char*)this + 0x8) = cUIIcon_vtbl;
        func_0021CCC8((char*)this + 0x74);
        *(int*)((char*)this + 0x78) = -1;
        *(int*)((char*)this + 0x7C) = 0;
    }
};

// cUIThing::setPosition(const t3Vector&), inlined.
struct sIconPos_9970 {
    char pad_0x0[0x44];
    sV3_96A8 pos;               // 0x44
    void setPosition(const sV3_96A8& p) { pos = p; }
};

/**
 * Creates the hidden map icons for every session point on the current location.
 *
 * For each point that exists (cPathSys_isStartpoint/cPathSys_getStartpoint on gPaths), maps its position into the 356 x
 * 266 picture and adds two cUIIcons: a "dot_visited" dot and an "indicator" highlight (18 x 18), keeping the
 * highlight's position too.
 *
 * @param selfp  the cOVState_MAP
 */
extern "C" void cOVState_MAP_setupLocalSessionList(cOVState_MAP* selfp)
{
    sMapSt_9970* self = (sMapSt_9970*)selfp;
    void* ifMap = cBE_getInterface_Fv(cBE_getBE(), 10);
    void* ifGame = cBE_getInterface_Fv(cBE_getBE(), 0);
    self->nSessions = cBEMissionInterface_getNumberSessionsOnTrack(ifMap, *cBENewRaceInterface_getLocation(ifGame));
    sV3_96A8 mapPos = (*(sV3_96A8*)&((cUIThing*)self->img)->mPosition);
    for (int i = 0; i < self->nSessions; i++) {
        if (cPathSys_isStartpoint(gPaths, 2, i + 1) != 0) {
        const char* sp = (const char*)cPathSys_getStartpoint(gPaths, 2, i + 1);
        sV3_96A8 spPos = *(sV3_96A8*)(sp + 0x8);
        tBEPictureDB pictureRecord;
        cBEMissionInterface_getPictureDBRecord(&pictureRecord, ifMap, *cBENewRaceInterface_getLocation(ifGame));
        unsigned int sessionPoint_x;
        {
            float across = Absf_96A8((spPos.x - pictureRecord.world_topleft_x) / (pictureRecord.world_bottomright_x - pictureRecord.world_topleft_x));
            sessionPoint_x = (int)(across * 356.0f);
        }
        unsigned int sessionPoint_y;
        {
            float down = Absf_96A8((spPos.y - pictureRecord.world_topleft_y) / (pictureRecord.world_bottomright_y - pictureRecord.world_topleft_y));
            sessionPoint_y = (int)(down * 266.0f);
        }

        self->markA[i] = (cFEObj_96A8*)new (str_Map_Indicator_Icon, 0x20000000, 0) cUIIcon_9970(self->screen);
        ((sIconPos_9970*)self->markA[i])->setPosition(sV3_96A8((float)(sessionPoint_x + 0xFC) - 5.0f, (float)(sessionPoint_y + 97) - 5.0f, mapPos.z));
        self->markA[i]->SetScale(sV3_96A8(18.0f, 18.0f));
        cUIIcon_setTextureByHashValue(self->markA[i], GetHashValue32(str_dot_visited));
        self->markA[i]->SetVisible(0);
        cUIThing* da = (cUIThing*)self->markA[i];
        da->mAttr = (da->mAttr & ~0x1F00) | 0xB00;
        cList_addToEnd((cList*)(self->screen + 0xB4), (cListNode*)self->markA[i]);

        self->markB[i] = (cFEObj_96A8*)new (str_Map_Indicator_Highlight, 0x20000000, 0) cUIIcon_9970(self->screen);
        self->posB[i] = sV3_96A8((float)(sessionPoint_x + 0xFC) - 5.0f, (float)(sessionPoint_y + 97) - 5.0f, mapPos.z);
        ((sIconPos_9970*)self->markB[i])->setPosition(self->posB[i]);
        self->markB[i]->SetScale(sV3_96A8(18.0f, 18.0f));
        cUIIcon_setTextureByHashValue(self->markB[i], GetHashValue32(str_indicator));
        self->markB[i]->SetVisible(0);
        cUIThing* db = (cUIThing*)self->markB[i];
        db->mAttr = (db->mAttr & ~0x1F00) | 0xC00;
        cList_addToEnd((cList*)(self->screen + 0xB4), (cListNode*)self->markB[i]);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_showIndicators);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern "C" cUIThing* cUIScreen_getObjectByHashName(void* self, int hash);
extern char str_SessionViewPoints[];

class cUIObj_9E78 {
public:
    int pad[2];
    virtual ~cUIObj_9E78();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setFocus(int v);
    virtual void setVisible(int v);
};
struct sMap_9E78 {
    char pad_0x0[0xC0];
    int count;              // 0xC0
    char pad_0xC4[0x10];
    void* screen;           // 0xD4
    char pad_0xD8[0x8];
    cUIObj_9E78* cursor;    // 0xE0
    cUIObj_9E78* a[8];      // 0xE4
    cUIObj_9E78* b[8];      // 0x104
};

/**
 * Shows the player icon and highlights the selected session point on the map.
 *
 * The selected point is the misc data of "SessionViewPoints"'s current item (default 1); it gets its
 * highlight icon and every other point its dot. The object lookup is not null-checked.
 *
 * @param mapState  the cOVState_MAP (raw pointer)
 */
extern "C" void cOVState_MAP_showIndicators(cOVState_MAP* mapState)
{
    sMap_9E78* self = (sMap_9E78*)mapState;
    if (self->cursor != 0) {
        self->cursor->setVisible(1);
    }
    cUIThing* list = ((cUIMenu*)cUIScreen_getObjectByHashName(self->screen, GetHashValue32(str_SessionViewPoints)))->mpSelected;
    int sel = 1;
    if (list != 0) {
        sel = list->mMiscData;
    }
    for (int i = 0; i < self->count; i++) {
        int cur = sel - 1;
        if (i == cur) {
            if (self->a[i] != 0) {
                self->a[i]->setVisible(0);
            }
            if (self->b[i] != 0) {
                self->b[i]->setVisible(1);
            }
        } else {
            if (self->a[i] != 0) {
                self->a[i]->setVisible(1);
            }
            if (self->b[i] != 0) {
                self->b[i]->setVisible(0);
            }
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_hideIndicators);
#ifdef SKIP_ASM
class cUIObj_9FD0 {
public:
    int pad[2];
    virtual ~cUIObj_9FD0();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setFocus(int v);
    virtual void setVisible(int v);
};
struct sMap_9FD0 {
    char pad_0x0[0xC0];
    int count;              // 0xC0
    char pad_0xC4[0x1C];
    cUIObj_9FD0* cursor;    // 0xE0
    cUIObj_9FD0* a[8];      // 0xE4
    cUIObj_9FD0* b[8];      // 0x104
};

/**
 * Hides the map overlay's indicators without deleting them: the player icon (+0xE0) and, for each of the +0xC0 session
 * points, its icon (+0xE4 array) and highlight (+0x104 array).
 *
 * Each present widget gets setVisible(0) (virtual slot 9). cOVState_MAP_showIndicators shows the same widgets and
 * cOVState_MAP_cleanupIndicators deletes them; nothing in the program calls this one.
 *
 * @param vself  the map overlay state (this)
 */
extern "C" void cOVState_MAP_hideIndicators(cOVState_MAP* vself)
{
    sMap_9FD0* self = (sMap_9FD0*)vself;
    if (self->cursor != 0) {
        self->cursor->setVisible(0);
    }
    for (int i = 0; i < self->count; i++) {
        if (self->a[i] != 0) {
            self->a[i]->setVisible(0);
        }
        if (self->b[i] != 0) {
            self->b[i]->setVisible(0);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_MAP_cleanupIndicators);
#ifdef SKIP_ASM
class cUIObj_A088 {
public:
    int pad[2];
    virtual ~cUIObj_A088();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void setFocus(int v);
    virtual void setVisible(int v);
};

struct sMap_A088 {
    char pad_0x0[0xE0];
    cUIObj_A088* cursor;    // 0xE0
    cUIObj_A088* a[8];      // 0xE4
    cUIObj_A088* b[8];      // 0x104
};

/**
 * Removes the map overlay's indicators: hides and deletes every indicator widget.
 *
 * For the player icon (+0xE0) and each of the two eight-entry icon arrays (+0xE4 and +0x104; On Tour mpPlayerIcon,
 * mpSessionIcons, mpSessionHighlights) it hides a present widget (setVisible(0)), deletes it and clears the pointer.
 *
 * @param mapOverlay  the map overlay state (this)
 */
extern "C" void cOVState_MAP_cleanupIndicators(cOVState_MAP* mapOverlay)
{
    sMap_A088* self = (sMap_A088*)mapOverlay;
    if (self->cursor != 0) {
        self->cursor->setVisible(0);
        delete self->cursor;
        self->cursor = 0;
    }
    for (int i = 0; i < 8; i++) {
        if (self->a[i] != 0) {
            self->a[i]->setVisible(0);
            delete self->a[i];
            self->a[i] = 0;
        }
        if (self->b[i] != 0) {
            self->b[i]->setVisible(0);
            delete self->b[i];
            self->b[i] = 0;
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", func_0020A1A8);
#ifdef SKIP_ASM
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" int sprintf(char* buf, const char* fmt, ...);
// PORT: SN soft-float float->double conversion (the source passed floats to sprintf).
extern "C" double fptodp(float f);
extern char D_004C6848[];
extern char str_pct_02_0f_pct_02_0f_b[];
extern char str_pct_02_0f_pct_02_0f_c[];

struct sGameSettings_A1A8 {
    unsigned int flags;     // 0x0, bits 22..24 = language
    int pad[0x288 / 4 - 1];
};
extern sGameSettings_A1A8 cBEOptionInterface_mOption_A1A8 __asm__("cBEOptionInterface_mOption");

/**
 * Formats a time in seconds as minutes and seconds in a static buffer, with the separator the language setting asks
 * for.
 *
 * Fetches back-end interface 4 (result unused), splits @p secs into minutes and seconds, and prints them as floats with
 * "%02.0f:%02.0f" when the language field (bits 22-24 of cBEOptionInterface_mOption) is 0 or 2, or "%02.0f'%02.0f\""
 * when it is 1 or 3, into D_004C6848. Other values leave the buffer as it was. Used by the map, pause, HUD-popup and
 * load-hint screens.
 *
 * @param secs  the time in whole seconds
 * @return D_004C6848, the formatted text (overwritten by the next call)
 */
// Format a time in seconds as minutes/seconds for the current language.
extern "C" char* func_0020A1A8(int secs)
{
    cBE_getInterface_Fv(cBE_getBE(), 4);
    int minuteCount = secs / 60;
    int secondCount = secs % 60;
    sGameSettings_A1A8 set = cBEOptionInterface_mOption_A1A8;
    switch ((set.flags >> 22) & 7) {
    case 0:
    case 2:
        sprintf(D_004C6848, str_pct_02_0f_pct_02_0f_b, fptodp((float)minuteCount), fptodp((float)secondCount));
        break;
    case 1:
    case 3:
        sprintf(D_004C6848, str_pct_02_0f_pct_02_0f_c, fptodp((float)minuteCount), fptodp((float)secondCount));
        break;
    }
    return D_004C6848;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_onCreateScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_hud_b[];
extern "C" void cCutSceneTransitionMan_ForceToRegularMode(void* self);
extern "C" cUIState* cUIStateStack_findState(void* list, int hash);


/**
 * Common overlay set-up: leaves cut-scene TV mode and tells the HUD an overlay opened.
 *
 * If the game's cut-scene transition manager (+0x64) is not in regular mode, forces regular mode
 * (cCutSceneTransitionMan_ForceToRegularMode) and sets mRestoreCutSceneMode when it was in, or going to, cut-scene mode. Then calls
 * virtual slot 24 of the "hud" state with (3, 0).
 */
extern "C" void cOVState_onCreateScreen(cOVState* self)
{
    self->mRestoreCutSceneMode = 0;
    cGame* game = SSXApp->Game;
    if (((char*)game) != 0) {
        char* cutSceneMan = (char*)game->CutSceneTransitionMan;
        int mode = *(int*)(cutSceneMan + 0x10);
        if (mode != 0) {
            cCutSceneTransitionMan_ForceToRegularMode(cutSceneMan);
            if (mode == 1 || mode == 3) {
                self->mRestoreCutSceneMode = 1;
            }
        }
    }
    cUIState* obj = cUIStateStack_findState(&self->mpEngine->mStateStack, GetHashValue32(str_hud_b));
    if (obj != 0) {
        obj->handleEvent(3, 0);
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_onDestroyScreen);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
extern char str_hud_b[];
extern "C" void cCutSceneTransitionMan_ForceToCutSceneMode(void* self);
extern "C" cUIState* cUIStateStack_findState(void* list, int hash);
extern "C" void cOVStateManager_removePDATemplate();
void* cUIState_onDestroyScreen(void* self);

/**
 * Common overlay tear-down: restores cut-scene TV mode, tells the HUD, removes the PDA template.
 *
 * Undoes cOVState_onCreateScreen (cCutSceneTransitionMan_ForceToCutSceneMode when mRestoreCutSceneMode is set; "hud" slot 24 with (4, 0)),
 * calls cOVStateManager_removePDATemplate when mRemovePDA is set, then cUIState_onDestroyScreen.
 */
extern "C" void cOVState_onDestroyScreen(cOVState* self)
{
    if (self->mRestoreCutSceneMode != 0) {
        cGame* game = SSXApp->Game;
        if (((char*)game) != 0) {
            cCutSceneTransitionMan_ForceToCutSceneMode(game->CutSceneTransitionMan);
            self->mRestoreCutSceneMode = 0;
        }
    }
    cUIState* obj = cUIStateStack_findState(&self->mpEngine->mStateStack, GetHashValue32(str_hud_b));
    if (obj != 0) {
        obj->handleEvent(4, 0);
    }
    if (self->mRemovePDA != 0) {
        cOVStateManager_removePDATemplate();
    }
    cUIState_onDestroyScreen(self);
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_onUpdate_HandleIncomingChat);
#ifdef SKIP_ASM
int GetHashValue32(char* str);
// PORT: cBE_getInterface__Fv is called with (be, kind) here; bind the 2-arg form to that symbol.
cBEInterface* cBE_getInterface_Fv(void* be, int kind) __asm__("cBE_getInterface__Fv");
extern "C" void* cBXString_cBXString2(void* self, const char* s);
extern "C" void* cBXString_Concat(void* self, const char* str);
extern "C" void cBXString__cBXString(void* self, int flags);
extern "C" void cUITextScroll_addAsciiString(void* text, char* str);
extern "C" void func_001A83D8(void* list, int keep, unsigned int sel, int a3, int pos, int a5, int a6, int trim);
extern "C" int cGameComm_getQueuedChat(int a, char* name, int len);
extern char str_helptext_b[];
extern char str_helptexts[];
extern char str_textScrollChat_b[];
extern char str_colon_sp_b[];

struct sBXString_A4E0 {
    char* str;
    sBXString_A4E0() {}
    sBXString_A4E0(const sBXString_A4E0& o);
};

/**
 * Prints an incoming in-race chat message on an in-game state's screen.
 *
 * Only online (the first word of cBENetworkInterface_networkOpt set) with a screen and a connected cGameComm. When a
 * chat packet is queued (cGameComm_getQueuedChat into mszChatBuff), hides "helptext" and "helptexts" and shows "textScrollChat"
 * with "<opponent's name>: <message>" (the name from the network options + 0x498), scrolled by func_001A83D8 from the
 * scroll's counters at +0xC4/+0xC6/+0xC8. The 0x41 size passed for the 0x40-byte buffer is not checked by
 * cGameComm_readPacket, which copies the whole payload (up to 236 bytes).
 *
 * @return 1 when a chat message was shown, else 0 (On Tour returns bool)
 */
// PORT: the session pointer is passed through cGameComm_getQueuedChat's int parameter.
extern "C" int cOVState_onUpdate_HandleIncomingChat(cOVState* self)
{
    int ret = 0;
    cBE_getInterface_Fv(SSXApp->BE, 7);
    tBENetworkDB* net = &cBENetworkInterface_networkOpt;
    if (*(int*)net != 0 && self->mpScreen != 0) {
        cGameComm* sess = cGameComm_mGameComm;
        if (sess != 0 && sess->mConnected != 0) {
            char* name = self->mszChatBuff;
            if (cGameComm_getQueuedChat((int)sess, name, 65) != 0) {
                cUIThing* helpText = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptext_b));
                if (helpText != 0)
                    helpText->setVisible(0);
                cUIThing* helpTexts = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_helptexts));
                if (helpTexts != 0)
                    helpTexts->setVisible(0);
                ret = 1;
                cUITextScroll* obj = (cUITextScroll*)cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(str_textScrollChat_b));
                unsigned short origNumLines = obj->mLineSize;
                unsigned short selectedIndex = obj->mSelectedIndex;
                unsigned short linesOffset = obj->mLinesOffset;
                int none = origNumLines == 0;
                sBXString_A4E0 chatLine;
                cBXString_cBXString2(&chatLine, ((char*)net) + 0x498);
                cBXString_Concat(&chatLine, str_colon_sp_b);
                cBXString_Concat(&chatLine, name);
                cUITextScroll_addAsciiString(obj, chatLine.str);
                func_001A83D8(obj, 40, 4, origNumLines, selectedIndex, linesOffset, none, 1);
                obj->setVisible(1);
                cBXString__cBXString(&chatLine, 2);
            }
        }
    }
    return ret;
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_onWidgetEvent);
#ifdef SKIP_ASM


/**
 * Forwards an overlay widget event to the game's current state.
 *
 * Calls virtual slot 9 of Game->mCurrentState (+0x200) with the same arguments; nothing when there is no game
 * or state.
 *
 * @param pWidget  the widget the event is for, passed through
 * @param Event  UI event code, passed through
 */
extern "C" void cOVState_onWidgetEvent(cOVState* self, int pWidget, int Event)
{
    cGame* w = SSXApp->Game;
    if (((char*)w) != 0) {
        cGameState* currentState = w->mCurrentState;
        if (((char*)currentState) != 0) {
            sVEntry<void(void*, int, int)>* vt = (sVEntry<void(void*, int, int)>*)currentState->mVtbl;
            vt[9].fn(vthis(currentState, vt[9]), pWidget, Event);
        }
    }
}
#endif

//100%
INCLUDE_ASM("fe/ovstatemap", cOVState_triggerTextFade);
#ifdef SKIP_ASM
extern "C" void* cUIAnimationBank_getAnimationByHashName(void* self, int hash);
extern "C" void cUIThing_playAnimation(void* self, void* anim, int mode, int v, int a4);

/**
 * Plays a named animation on a named object of the overlay's screen.
 *
 * Looks both up by hash (the animation in the engine's animation bank at +0x50) and plays it with mode 3.
 *
 * @param objName  name of the screen object
 * @param animName  name of the animation
 */
extern "C" void cOVState_triggerTextFade(cOVState* self, char* objName, char* animName)
{
    void* obj = cUIScreen_getObjectByHashName(self->mpScreen, GetHashValue32(objName));
    if (obj != 0) {
        void* bank = &self->mpEngine->mAnimBank;
        void* anim = cUIAnimationBank_getAnimationByHashName(bank, GetHashValue32(animName));
        if (anim != 0) {
            cUIThing_playAnimation(obj, anim, 3, 0, 0);
        }
    }
}
#endif

